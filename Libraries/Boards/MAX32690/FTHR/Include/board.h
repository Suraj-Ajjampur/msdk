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

#include <stdio.h>

#ifndef LIBRARIES_BOARDS_MAX32690_FTHR_INCLUDE_BOARD_H_
#define LIBRARIES_BOARDS_MAX32690_FTHR_INCLUDE_BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_FTHR

#ifndef CONSOLE_UART
#define CONSOLE_UART 0 /// UART instance to use for console
#endif

#ifndef CONSOLE_BAUD
#define CONSOLE_BAUD 115200 /// Console baud rate
#endif

#ifndef HCI_UART
#define HCI_UART 3
#endif

#ifndef TERMINAL_UART
#define TERMINAL_UART CONSOLE_UART
#endif

#ifndef USER_UART
#define USER_UART 1
#endif

#ifdef LED_OFF
#undef LED_OFF
#endif
#define LED_OFF 1 /// Override inactive state of LEDs

#ifdef LED_ON
#undef LED_ON
#endif
#define LED_ON 0 /// Override active state of LEDs

#ifndef EXT_FLASH_BAUD
#define EXT_FLASH_BAUD 4000000
#endif

/**
 *  A reference to LED1 (RED LED in the RGB LED) of the board.
 *  Can be used with the LED_On, LED_Off, and LED_Toggle functions.
 */
#define LED1 0
#define LED_RED LED1
#define LED2 1
#define LED_GREEN LED2
#define LED3 2
#define LED_BLUE LED3

/**
 * @brief   Initialize the BSP and board interfaces.
 * @return  #E_NO_ERROR if everything is successful
 */
int Board_Init(void);

/**
 * @brief   Initialize or reinitialize the console. This may be necessary if the
 *          system clock rate is changed.
 * @return  #E_NO_ERROR if everything is successful
 */
int Console_Init(void);

/**
 * @brief   Shutdown the console.
 * @returns #E_NO_ERROR if everything is successful
 */
int Console_Shutdown(void);

/**
 * @brief   Attempt to prepare the console for sleep.
 * @return  #E_NO_ERROR if ready to sleep, #E_BUSY if not ready for sleep.
 */
int Console_PrepForSleep(void);

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_BOARDS_MAX32690_FTHR_INCLUDE_BOARD_H_
