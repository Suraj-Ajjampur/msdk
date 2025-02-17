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
 * @file    board.h
 * @brief   Board support package API.
 */

#ifndef LIBRARIES_BOARDS_MAX78000_EVKIT_V1_INCLUDE_BOARD_H_
#define LIBRARIES_BOARDS_MAX78000_EVKIT_V1_INCLUDE_BOARD_H_

#include <stdio.h>
#include "tft_ssd2119.h"
#include "tsc2046.h"
#include "led.h"
#include "pb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_EVKIT_V1

#ifndef CONSOLE_UART
#define CONSOLE_UART 0 /// UART instance to use for console
#endif

#ifndef CONSOLE_BAUD
#define CONSOLE_BAUD 115200 /// Console baud rate
#endif

#define SCCB_SCL_PORT MXC_GPIO0 /// SCCB clock port
#define SCCB_SCL_PIN MXC_GPIO_PIN_30 /// SCCB clock pin
#define SCCB_SDA_PORT MXC_GPIO0 /// SCCB data port
#define SCCB_SDA_PIN MXC_GPIO_PIN_31 /// SCCB data pin

#ifndef __riscv // RISC-V does not have access to SPI0
#define TFT_SPI MXC_SPI0 // SPI port to use for TFT display
#define TFT_SPI_PORT MXC_GPIO0 /**< GPIO port for SPI peripheral pins. */
#define TFT_SPI_PINS                                   \
    MXC_GPIO_PIN_5 | MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7 | \
        MXC_GPIO_PIN_11 /**< GPIO pins for SPI peripheral. */

#define TFT_DC_PORT MXC_GPIO0 /**< GPIO port for Data/Command signal. */
#define TFT_DC_PIN MXC_GPIO_PIN_8 /**< GPIO pin for Data/Command signal. */

#define TS_SPI MXC_SPI0
#define TS_SPI_FREQ 1000000
#endif // __riscv

#ifdef LED_OFF
#undef LED_OFF
#endif
#define LED_OFF 1 /// Override inactive state of LEDs

#ifdef LED_ON
#undef LED_ON
#endif
#define LED_ON 0 /// Override active state of LEDs

/**
 *  A reference to LED1 of the board.
 *  Can be used with the LED_On, LED_Off, and LED_Toggle functions.
 */
#define LED1 0
#define LED_GREEN LED1

/**
 *  A reference to LED2 of the board.
 *  Can be used with the LED_On, LED_Off, and LED_Toggle functions.
 */
#define LED2 1
#define LED_RED LED2

/**
 * \brief   Initialize the BSP and board interfaces.
 * \returns #E_NO_ERROR if everything is successful
 */
int Board_Init(void);

/**
 * \brief   Initialize or reinitialize the console. This may be necessary if the
 *          system clock rate is changed.
 * \returns #E_NO_ERROR if everything is successful
 */
int Console_Init(void);

/**
 * \brief   Shutdown the console.
 * \returns #E_NO_ERROR if everything is successful
 */
int Console_Shutdown(void);

/**
 * \brief   Attempt to prepare the console for sleep.
 * \returns #E_NO_ERROR if ready to sleep, #E_BUSY if not ready for sleep.
 */
int Console_PrepForSleep(void);

#ifdef __riscv
/**
 * \brief   Set up RISCV JTAG
 * \returns #E_NO_ERROR if successful
 */
int Debug_Init(void);
#endif // __riscv

/**
 * \brief   Microphone power control.
 *
 * \param   on          1 for ON, 0 for OFF
 *
 * \return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
#define POWER_OFF 0
#define POWER_ON 1
int Microphone_Power(int on);

/**
 * \brief   Camera power control.
 *
 * \param   on          1 for ON, 0 for OFF
 *
 * \return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int Camera_Power(int on);

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_BOARDS_MAX78000_EVKIT_V1_INCLUDE_BOARD_H_
