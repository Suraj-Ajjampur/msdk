/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   AFE_DAC Example
 * @details This example configures the AFE DAC 12 to output 1V
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mxc_device.h"
#include "board.h"
#include "mxc_delay.h"
#include "afe.h"

/***** Definitions *****/
#define AFE_TIMER_INSTANCE MXC_TMR1

/***** Globals *****/

/***** Functions *****/

static void dump_dac_regs(void)
{
    uint32_t read_val = 0;

    afe_read_register(MXC_R_AFE_DAC_CTRL, &read_val);
    printf("AFE DAC CTRL: \t\t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_RATE, &read_val);
    printf("AFE DAC RATE: \t\t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_INT, &read_val);
    printf("AFE DAC INT: \t\t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_REG, &read_val);
    printf("AFE DAC REG: \t\t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_TRIM, &read_val);
    printf("AFE DAC TRIM: \t\t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_VREF_CTRL, &read_val);
    printf("AFE DAC VREF CTRL: \t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_FIFO, &read_val);
    printf("AFE DAC FIFO: \t\t0x%08X\n", read_val);

    afe_read_register(MXC_R_AFE_DAC_VREF_TRIM, &read_val);
    printf("AFE DAC VREF TRIM: \t0x%08X\n", read_val);
}

// *****************************************************************************
int main(void)
{
    int status = E_NO_ERROR;
    uint32_t read_val = 0;

    status = afe_load_trims(AFE_TIMER_INSTANCE);
    if (status != E_NO_ERROR) {
        printf("Error during afe load trims: %d\n", status);
        while (1) {}
    }

    printf("\n\n\n\n\nMAX32675 AFE DAC Example\n\n");
    printf("This example configures the AFE's DAC to output a static 1.0V\n");

    //
    // Reset the DAC
    //
    printf("Reseting DAC\n");
    afe_write_register(MXC_R_AFE_DAC_CTRL, MXC_F_AFE_DAC_CTRL_RESET);

    // Wait till reset is done
    while (1) {
        afe_read_register(MXC_R_AFE_DAC_CTRL, &read_val);

        if (!(read_val & MXC_F_AFE_DAC_CTRL_RESET)) {
            // Reset is cleared.  Done
            break;
        }
    }

    //
    // Configure to use Internal DAC Reference
    //
    afe_read_register(MXC_R_AFE_DAC_VREF_CTRL, &read_val);
    read_val |= MXC_F_AFE_DAC_VREF_CTRL_REF_PU | MXC_F_AFE_DAC_VREF_CTRL_REFDAC_OUTEN;
    // Clear out ref selections bits.
    read_val &= ~MXC_F_AFE_DAC_VREF_CTRL_DACREFSEL;
    read_val |= MXC_S_AFE_DAC_VREF_CTRL_DACREFSEL_VOLTS_2_048;

    afe_write_register(MXC_R_AFE_DAC_VREF_CTRL, read_val);

    //
    // Configure the DAC
    // Set to output values when written to FIFO. DAC will hold last value
    // Assuming default power level of 48uA is adequate
    //
    afe_read_register(MXC_R_AFE_DAC_CTRL, &read_val);

    read_val |= MXC_F_AFE_DAC_CTRL_POWER_ON;
    read_val |= MXC_F_AFE_DAC_CTRL_CLOCK_GATE_EN;

    read_val &= ~MXC_F_AFE_DAC_CTRL_OP_MODE;
    read_val |= MXC_S_AFE_DAC_CTRL_OP_MODE_OUTPUT_WHEN_FIFO_AVAIL;

    // Enable DAC output P and N channel FETs
    // NOTE: These bits [18 and 19] are currently undocumented, and always read as 0
    read_val |= 0x000C0000;

    read_val &= ~MXC_F_AFE_DAC_CTRL_START_MODE;
    read_val |= MXC_S_AFE_DAC_CTRL_START_MODE_START_WHEN_FIFO_NOT_EMPTY;

    afe_write_register(MXC_R_AFE_DAC_CTRL, read_val);

    //
    // Set output voltage
    // Ref is 2.048V and DAC is 12 bits.
    // 2^12 is 4096 LSBs. Each LSB is 2.048/4096 = 500uV
    // To get 1V 1/.0005 = 2000
    //
    // Write the FIFO one time to set to static voltage.
    //
    // NOTE: values written to the FIFO must be LEFT justified, so shift over 4 bits
    //

    afe_write_register(MXC_R_AFE_DAC_FIFO, (2000 << 4));

    printf("DAC Configured for 1.0V output. Current AFE DAC registers:\n");
    dump_dac_regs();

    printf("Done. Halting...\n");

    return 0;
}
