/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

/* One wire API Test, for use with DS2401 */

#include <stdio.h>
#include <string.h>

#include "mxc_device.h"
#include "owm.h"
#include "board.h"
#include "max31825_driver.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "max32665.h"

#define INTERRUPT_MODE              1

#if (INTERRUPT_MODE)
#define OWM_INTEN_RX_TX_POS         1
#define OWM_INT_EN                  (3 << OWM_INTEN_RX_TX_POS)
mxc_owm_req_t trans_req;
#define WRITE_SIZE  5
#define READ_SIZE   9
#endif

volatile int TX_RX_FLAG;
static uint8_t Write_Buffer[WRITE_SIZE];
static uint8_t Read_buffer[READ_SIZE];

void OWM_Handler(void)
{
    MXC_OWM_AsyncHandler(trans_req->owm);
}

void OWMCallback(mxc_owm_req_t *req, int error)
{
    //Store 1-Wire values into a buffer
    TX_RX_FLAG = error;
    if (MXC_OWM_TransactionAsync(trans_req) != E_NO_ERROR) {}
}

int32_t ow_romid_test(uint8_t od)
{
    uint8_t buffer[8];
    uint8_t crc8;
    int i;

    /* Set 1-Wire to standard speed */
    MXC_OWM_SetOverdrive(0);
    /* Error if presence pulse not detected. */
    if (MXC_OWM_Reset() == 0) {
        return -2;
    }

    if (od) {
        /* Send the overdrive command */
        buffer[0] = OD_SKIP_ROM_COMMAND;
        MXC_OWM_Write(buffer, 1);
        MXC_OWM_SetOverdrive(1);
        /* Error if presence pulse not detected. */
        if (MXC_OWM_Reset() == 0) {
            return -4;
        }
    }

    /* Send read ROMID command */
    buffer[0] = READ_ROM_COMMAND;
    MXC_OWM_Write(buffer, 1);

    /* Read the ROM ID */
    memset(buffer, 0, sizeof(buffer));
    if (MXC_OWM_Read(buffer, 8) < 0) {
        return -5;
    }

    printf("ROMID: ");
    for (i = 0; i < 8; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    /* Check for zero family code in ROM ID */
    if (buffer[0] == 0) {
        return -6;
    }

    /* Check CRC8 of received ROM ID */
    setcrc8(0);
    for (i = 0; i < 8; i++) {
        crc8 = docrc8(buffer[i]);
    }

    if (crc8 != 0x00) {
        return -7;
    }

    return 0;
}

int main(void)
{
    int retval = 0;
    printf("***** 1-Wire Temp Sensor (MAX31825) Example *****\n");
    printf("Connect 1-Wire pin, VCC and GND to the target\n");
    printf("   - If you have FTHR or FTHR2 board 1-Wire pin: P0.12\n");
    printf("   - If you have EvKit_V1 board      1-Wire pin: P0.24\n");
    printf("For more information please take a look at your board schematic\n");
    printf("\n\n");

#if (INTERRUPT_MODE == 1)
    //Set up RX Data Ready function to trigger an interrupt each time a byte of Data is read
    NVIC_ClearPendingIRQ(OWM_IRQn);
    NVIC_DisableIRQ(OWM_IRQn);
    NVIC_EnableIRQ(OWM_IRQn);
    MXC_NVIC_SetVector(OWM_IRQn,OWM_TX_RX_Handler);
    NVIC_SetPriority(OWM_IRQn,0);
#endif

    mxc_owm_cfg_t owm_cfg;
    owm_cfg.int_pu_en = 1;
    owm_cfg.ext_pu_mode = MXC_OWM_EXT_PU_ACT_HIGH;
    owm_cfg.long_line_mode = 0;


#if defined(BOARD_FTHR) || defined(BOARD_FTHR2)
    MXC_OWM_Init(&owm_cfg, MAP_B); // 1-Wire pins P0.12/13
#else
    MXC_OWM_Init(&owm_cfg, MAP_C); // 1-Wire pins P0.24/25

    trans_req.owm = MXC_BASE_OWM;
    trans_req.rxData = Read_buffer;
    trans_req.rxLen = READ_SIZE;
    trans_req.txData = Write_Buffer;
    trans_req.txLen = WRITE_SIZE;
    trans_req.callback = OWMCallback;
    if (MXC_OWM_TransactionAsync(trans_req) != E_NO_ERROR) {}

    
#endif
    static int count = 0;
    retval = OW_MAX31825_Test();

    while(1){

    /* Test overdrive */
    retval = Read_Max31825_temp();
    MXC_Delay(1000000);
       
    if (retval) {
        printf("Error Code: %d",retval);
        printf("Example Failed\n");
        return E_FAIL;
    }
    else{
        printf("\n\rTemp Value %04f", get_max31825_temp());
        break;
    }
    count++;
    }
    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}

