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
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "gpio.h"

/***** Definitions *****/
/*
 *  Due to limited push button on EvKit
 *  please select one of below use case
 */
#define GPIO_INT_USECASE 1
#define GPIO_POOL_USECASE 0

#if (!(GPIO_INT_USECASE || GPIO_POOL_USECASE))
#error "You must set either Interrupt or Pooling usecase to 1."
#endif

#if (GPIO_INT_USECASE && GPIO_POOL_USECASE)
#error "You must select either GPIO_INT_USECASE or GPIO_POOL_USECASE, not both."
#endif

#define BUTTON_PORT MXC_GPIO0
#define BUTTON_PIN MXC_GPIO_PIN_12

#define LED_PORT MXC_GPIO0
#define LED_PIN MXC_GPIO_PIN_13

/***** Globals *****/

/***** Functions *****/
#if GPIO_INT_USECASE
void gpio_isr(void *cbdata)
{
    mxc_gpio_cfg_t *cfg = cbdata;
    MXC_GPIO_OutToggle(cfg->port, cfg->mask);
}
#endif

int main(void)
{
    printf("\n\n************************* GPIO Example ***********************\n\n");
#if GPIO_POOL_USECASE
    printf("GPIO polling use case:\n");
    printf("SW2 (P0.12) status will be checked periodically,\n");
    printf("if push button pressed the LED (P0.13) will be on.\n");
    printf("if push button released the LED (P0.13) will be off.\n");
#endif

#if GPIO_INT_USECASE
    printf("GPIO interrupt use case:\n");
    printf("An interrupt is set to occur when SW2 is pressed (P0.12).\n");
    printf("In the ISR LED (P0.13) is toggled for every switch press.\n");
#endif

#if GPIO_INT_USECASE
    mxc_gpio_cfg_t gpio_interrupt;
    mxc_gpio_cfg_t gpio_interrupt_status;

    /* Setup interrupt status pin as an output so we can toggle it on each interrupt. */
    gpio_interrupt_status.port = LED_PORT;
    gpio_interrupt_status.mask = LED_PIN;
    gpio_interrupt_status.pad = MXC_GPIO_PAD_NONE;
    gpio_interrupt_status.func = MXC_GPIO_FUNC_OUT;
    gpio_interrupt_status.vssel = MXC_GPIO_VSSEL_VDDIO;
    gpio_interrupt_status.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_interrupt_status);

    /*
	 *   Set up interrupt the gpio.
	 *   Switch on EV kit is open when non-pressed, and grounded when pressed.
	 *   Use an internal pull-up so pin reads high when button is not pressed.
	 */
    gpio_interrupt.port = BUTTON_PORT;
    gpio_interrupt.mask = BUTTON_PIN;
    gpio_interrupt.pad = MXC_GPIO_PAD_PULL_UP;
    gpio_interrupt.func = MXC_GPIO_FUNC_IN;
    gpio_interrupt.vssel = MXC_GPIO_VSSEL_VDDIOH;
    gpio_interrupt.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_interrupt);
    MXC_GPIO_RegisterCallback(&gpio_interrupt, gpio_isr, &gpio_interrupt_status);
    MXC_GPIO_IntConfig(&gpio_interrupt, MXC_GPIO_INT_FALLING);
    MXC_GPIO_EnableInt(gpio_interrupt.port, gpio_interrupt.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(BUTTON_PORT)));

    while (1) {
        {
        }
        // waiting GPIO interrupt
    }
#endif

#if GPIO_POOL_USECASE
    mxc_gpio_cfg_t gpio_in;
    mxc_gpio_cfg_t gpio_out;

    /* Setup input pin. */
    gpio_in.port = BUTTON_PORT;
    gpio_in.mask = BUTTON_PIN;
    gpio_in.pad = MXC_GPIO_PAD_PULL_UP;
    gpio_in.func = MXC_GPIO_FUNC_IN;
    gpio_in.vssel = MXC_GPIO_VSSEL_VDDIO;
    gpio_in.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_in);

    /* Setup output pin. */
    gpio_out.port = LED_PORT;
    gpio_out.mask = LED_PIN;
    gpio_out.pad = MXC_GPIO_PAD_NONE;
    gpio_out.func = MXC_GPIO_FUNC_OUT;
    gpio_out.vssel = MXC_GPIO_VSSEL_VDDIO;
    gpio_out.drvstr = MXC_GPIO_DRVSTR_0;
    MXC_GPIO_Config(&gpio_out);

    while (1) {
        /* Read state of the input pin. */
        if (MXC_GPIO_InGet(gpio_in.port, gpio_in.mask)) {
            /* Input pin was high, clear the output pin. */
            MXC_GPIO_OutClr(gpio_out.port, gpio_out.mask);
        } else {
            /* Input pin was low, set the output pin. */
            MXC_GPIO_OutSet(gpio_out.port, gpio_out.mask);
        }
        MXC_Delay(MXC_DELAY_MSEC(50));
    }
#endif

    return 0;
}
