/**
 * @file        main.c
 * @brief       GPIO Example
 * @details
 */

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

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_delay.h"
#include "mxc_device.h"
#include "nvic_table.h"
#include "board.h"
#include "gpio.h"

/***** Definitions *****/
#if defined(BOARD_FTHR)
#define MXC_GPIO_PORT_OUT MXC_GPIO0
#define MXC_GPIO_PIN_OUT MXC_GPIO_PIN_2

#define MXC_GPIO_PORT_INTERRUPT_IN MXC_GPIO0
#define MXC_GPIO_PIN_INTERRUPT_IN MXC_GPIO_PIN_10

#define MXC_GPIO_PORT_INTERRUPT_STATUS MXC_GPIO0
#define MXC_GPIO_PIN_INTERRUPT_STATUS MXC_GPIO_PIN_3
#else
#define MXC_GPIO_PORT_OUT MXC_GPIO0
#define MXC_GPIO_PIN_OUT MXC_GPIO_PIN_22

#define MXC_GPIO_PORT_INTERRUPT_IN MXC_GPIO0
#define MXC_GPIO_PIN_INTERRUPT_IN MXC_GPIO_PIN_18

#define MXC_GPIO_PORT_INTERRUPT_STATUS MXC_GPIO0
#define MXC_GPIO_PIN_INTERRUPT_STATUS MXC_GPIO_PIN_23
#endif
/***** Globals *****/

/***** Functions *****/
void gpio_isr(void *cbdata)
{
    MXC_Delay(MXC_DELAY_MSEC(10));
    mxc_gpio_cfg_t *cfg = cbdata;
    MXC_GPIO_OutToggle(cfg->port, cfg->mask);
}

int main(void)
{
    mxc_gpio_cfg_t gpio_out;
    mxc_gpio_cfg_t gpio_interrupt;
    mxc_gpio_cfg_t gpio_interrupt_status;

    printf("\n\n************************* GPIO Example *************************\n");
#if defined(BOARD_FTHR)
    printf("\nThis example controls the state of the LEDs based on the state of\n");
    printf("the push button P0.10 (SW2). An interrupt is setup so that when\n");
    printf("the button is pressed P0.3 (LED1) will toggle. P0.2 (LED0) is set\n");
    printf("up to mirror the state of the button; when the button is depressed\n");
    printf("LED0 is illuminated and when it is released LED0 is turned off.\n\n");
#else
    printf("\nThis example controls the state of the LEDs based on the state of\n");
    printf("the push button P0.18 (SW3). An interrupt is setup so that when\n");
    printf("the button is pressed P0.23 (LED1) will toggle. P0.22 (LED0) is set\n");
    printf("up to mirror the state of the button; when the button is depressed\n");
    printf("LED0 is illuminated and when it is released LED0 is turned off.\n\n");
#endif

    /* Setup interrupt status pin as an output so we can toggle it on each interrupt. */
    gpio_interrupt_status.port = MXC_GPIO_PORT_INTERRUPT_STATUS;
    gpio_interrupt_status.mask = MXC_GPIO_PIN_INTERRUPT_STATUS;
    gpio_interrupt_status.pad = MXC_GPIO_PAD_NONE;
    gpio_interrupt_status.func = MXC_GPIO_FUNC_OUT;
    gpio_interrupt_status.vssel = MXC_GPIO_VSSEL_VDDIOH;
    gpio_interrupt_status.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_interrupt_status);

    /*
     *   Set up interrupt on P0.18.
     *   Switch on EV kit is open when non-pressed, and grounded when pressed.  Use an internal pull-up so pin
     *     reads high when button is not pressed.
     */
    gpio_interrupt.port = MXC_GPIO_PORT_INTERRUPT_IN;
    gpio_interrupt.mask = MXC_GPIO_PIN_INTERRUPT_IN;
    gpio_interrupt.pad = MXC_GPIO_PAD_PULL_UP;
    gpio_interrupt.func = MXC_GPIO_FUNC_IN;
    gpio_interrupt.vssel = MXC_GPIO_VSSEL_VDDIOH;
    gpio_interrupt.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_interrupt);
    MXC_GPIO_RegisterCallback(&gpio_interrupt, gpio_isr, &gpio_interrupt_status);
    MXC_GPIO_IntConfig(&gpio_interrupt, MXC_GPIO_INT_FALLING);
    MXC_GPIO_EnableInt(gpio_interrupt.port, gpio_interrupt.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(MXC_GPIO_PORT_INTERRUPT_IN)));

    /* Setup output pin. */
    gpio_out.port = MXC_GPIO_PORT_OUT;
    gpio_out.mask = MXC_GPIO_PIN_OUT;
    gpio_out.pad = MXC_GPIO_PAD_NONE;
    gpio_out.func = MXC_GPIO_FUNC_OUT;
    gpio_out.vssel = MXC_GPIO_VSSEL_VDDIO;
    gpio_out.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_out);

    while (1) {
        /* Read state of the input pin. */
        if (!MXC_GPIO_InGet(gpio_interrupt.port, gpio_interrupt.mask)) {
            /* Input pin was high, set the output pin. */
            MXC_GPIO_OutClr(gpio_out.port, gpio_out.mask);
        } else {
            /* Input pin was low, clear the output pin. */
            MXC_GPIO_OutSet(gpio_out.port, gpio_out.mask);
        }
    }

    return 0;
}
