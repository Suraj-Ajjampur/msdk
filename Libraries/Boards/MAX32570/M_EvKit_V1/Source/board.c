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

#include <stdio.h>
#include "mxc_device.h"
#include "mxc_sys.h"
#include "mxc_assert.h"
#include "board.h"
#include "uart.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "led.h"
#include "pb.h"
#include "spixf.h"
#include "i2c.h"
#include "Ext_Flash.h"
#include "tft_ssd2119.h"
#include "tsc2046.h"

/***** Global Variables *****/
mxc_uart_regs_t *ConsoleUart = MXC_UART_GET_UART(CONSOLE_UART);
extern uint32_t SystemCoreClock;

// clang-format off
const mxc_gpio_cfg_t pb_pin[] = { { MXC_GPIO0, MXC_GPIO_PIN_16, MXC_GPIO_FUNC_IN,
                                    MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO, MXC_GPIO_DRVSTR_0 } };
const unsigned int num_pbs = (sizeof(pb_pin) / sizeof(mxc_gpio_cfg_t));

const mxc_gpio_cfg_t led_pin[] = { { MXC_GPIO2, MXC_GPIO_PIN_17, MXC_GPIO_FUNC_OUT,
                                     MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO, MXC_GPIO_DRVSTR_0 } };
const unsigned int num_leds = (sizeof(led_pin) / sizeof(mxc_gpio_cfg_t));

/* Touch screen controller interrupt signal */
const mxc_gpio_cfg_t ts_int_pin = { MXC_GPIO0, MXC_GPIO_PIN_0, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE,
                                    MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 };
/* Touch screen controller busy signal */
const mxc_gpio_cfg_t ts_busy_pin = { MXC_GPIO0, MXC_GPIO_PIN_1, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE,
                                     MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 };
// clang-format on

/******************************************************************************/
/** 
 * NOTE: This weak definition is included to support Push Button/Touchscreen interrupts
 *       in case the user does not define this interrupt handler in their application.
 **/
__weak void GPIO0_IRQHandler(void)
{
    MXC_GPIO_Handler(MXC_GPIO_GET_IDX(MXC_GPIO0));
}

/******************************************************************************/
static int ext_flash_board_init(void)
{
    int err;
    err = MXC_SPIXF_Init(0x0B, EXT_FLASH_BAUD);
    if (err == E_NO_ERROR) {
        MXC_GPIO_SetVSSEL(gpio_cfg_spixf.port, MXC_GPIO_VSSEL_VDDIOH, gpio_cfg_spixf.mask);
        MXC_SPIXF_Enable();
    }
    return err;
}

/******************************************************************************/
static int ext_flash_board_read(uint8_t *read, unsigned len, unsigned deassert,
                                Ext_Flash_DataLine_t width)
{
    mxc_spixf_req_t req = { deassert, 0, NULL, read, (mxc_spixf_width_t)width, len, 0, 0, NULL };
    if (MXC_SPIXF_Transaction(&req) != len) {
        return E_COMM_ERR;
    }
    return E_NO_ERROR;
}

/******************************************************************************/
static int ext_flash_board_write(const uint8_t *write, unsigned len, unsigned deassert,
                                 Ext_Flash_DataLine_t width)
{
    mxc_spixf_req_t req = { deassert, 0, write, NULL, (mxc_spixf_width_t)width, len, 0, 0, NULL };
    if (MXC_SPIXF_Transaction(&req) != len) {
        return E_COMM_ERR;
    }
    return E_NO_ERROR;
}

/******************************************************************************/
static int ext_flash_clock(unsigned len, unsigned deassert)
{
    return MXC_SPIXF_Clocks(len, deassert);
}

/******************************************************************************/
void mxc_assert(const char *expr, const char *file, int line)
{
    printf("MXC_ASSERT %s #%d: (%s)\n", file, line, expr);

    while (1) {}
}

void TS_SPI_Init(void)
{
    MXC_SPI_Init(TS_SPI, true, false, 2, 0, TS_SPI_FREQ);
    MXC_GPIO_SetVSSEL(MXC_GPIO0, MXC_GPIO_VSSEL_VDDIOH,
                      MXC_GPIO_PIN_2 | MXC_GPIO_PIN_3 | MXC_GPIO_PIN_4 | MXC_GPIO_PIN_5);
    MXC_SPI_SetDataSize(TS_SPI, 8);
    MXC_SPI_SetWidth(TS_SPI, SPI_WIDTH_STANDARD);
}

void TS_SPI_Transmit(uint8_t datain, uint16_t *dataout)
{
    int i;
    uint8_t rx[2] = { 0, 0 };
    mxc_spi_req_t request;

    request.spi = TS_SPI;
    request.ssIdx = 0;
    request.ssDeassert = 0;
    request.txData = (uint8_t *)(&datain);
    request.rxData = NULL;
    request.txLen = 1;
    request.rxLen = 0;

    MXC_SPI_SetFrequency(TS_SPI, TS_SPI_FREQ);
    MXC_SPI_SetDataSize(TS_SPI, 8);

    MXC_SPI_MasterTransaction(&request);

    // Wait to clear TS busy signal
    for (i = 0; i < 100; i++) {
        __asm volatile("nop\n");
    }

    request.ssDeassert = 1;
    request.txData = NULL;
    request.rxData = (uint8_t *)(rx);
    request.txLen = 0;
    request.rxLen = 2;

    MXC_SPI_MasterTransaction(&request);

    if (dataout != NULL) {
        *dataout = (rx[1] | (rx[0] << 8)) >> 4;
    }
}

/******************************************************************************/
int Board_Init(void)
{
    int err;
    Ext_Flash_Config_t exf_cfg = { .init = ext_flash_board_init,
                                   .read = ext_flash_board_read,
                                   .write = ext_flash_board_write,
                                   .clock = ext_flash_clock };

    if ((err = Ext_Flash_Configure(&exf_cfg)) != E_NO_ERROR) {
        return err;
    }

    if ((err = Console_Init()) < E_NO_ERROR) {
        return err;
    }

    if ((err = PB_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

    if ((err = LED_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

    /* Set HW related configureation to TFT display */
    mxc_tft_spi_config tft_spi_config = {
        .regs = MXC_SPI1,
        .gpio = { MXC_GPIO0, MXC_GPIO_PIN_3 | MXC_GPIO_PIN_4 | MXC_GPIO_PIN_5 | MXC_GPIO_PIN_31,
                  MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 },
        .freq = 12000000,
        .ss_idx = 0,
    };

    mxc_gpio_cfg_t tft_reset_pin = { MXC_GPIO1,         MXC_GPIO_PIN_17,       MXC_GPIO_FUNC_OUT,
                                     MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 };
    mxc_gpio_cfg_t tft_bl_pin = { MXC_GPIO1,         MXC_GPIO_PIN_16,       MXC_GPIO_FUNC_OUT,
                                  MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 };

    MXC_TFT_PreInit(&tft_spi_config, &tft_reset_pin, &tft_bl_pin);

    MXC_TS_AssignInterruptPin(ts_int_pin);

    return E_NO_ERROR;
}

/******************************************************************************/
int Console_Init(void)
{
    int err;

    if ((err = MXC_UART_Init(ConsoleUart, CONSOLE_BAUD)) != E_NO_ERROR) {
        return err;
    }

    return E_NO_ERROR;
}

int Console_Shutdown(void)
{
    int err;

    if ((err = MXC_UART_Shutdown(ConsoleUart)) != E_NO_ERROR) {
        return err;
    }

    return E_NO_ERROR;
}
/******************************************************************************/
__weak void NMI_Handler(void)
{
    __NOP();
}
