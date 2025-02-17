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
 * @file        main.c
 * @brief       Demonstrates the alarm functionality of the Real-Time Clock (RTC)
 * @details     THE RTC is configured to wake the device from backup mode every
 * 				TIME_OF_DAY seconds. On wakeup, the device will print the current
 * 				time, rearm the alarm, and
 */

/***** Includes *****/
#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "led.h"
#include "lp.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "nvic_table.h"
#include "rtc.h"
#include "uart.h"

/***** Definitions *****/
#define LED_TODA 0
#define TIME_OF_DAY_SEC 7

#define MSEC_TO_RSSA(x) \
    (0 - ((x * 4096) /  \
          1000)) // Converts a time in milleseconds to the equivalent RSSA register value.
#define SECS_PER_MIN 60
#define SECS_PER_HR (60 * SECS_PER_MIN)
#define SECS_PER_DAY (24 * SECS_PER_HR)

/***** Globals *****/

/***** Functions *****/
// *****************************************************************************
void RTC_IRQHandler(void) {}

// *****************************************************************************
void rescheduleAlarm()
{
    uint32_t time;
    int flags = MXC_RTC_GetFlags();

    // Check for TOD alarm flag
    if (flags & MXC_F_RTC_CTRL_TOD_ALARM) {
        MXC_RTC_ClearFlags(MXC_F_RTC_CTRL_TOD_ALARM);

        MXC_RTC_GetSeconds(&time); // Get Current time (s)

        // Disable interrupt while re-arming RTC alarm
        while (MXC_RTC_DisableInt(MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {}

        // Reset TOD alarm for TIME_OF_DAY_SEC in the future
        if (MXC_RTC_SetTimeofdayAlarm(time + TIME_OF_DAY_SEC) != E_NO_ERROR) {
            /* Handle Error */
        }

        // Re-enable TOD alarm interrupt
        while (MXC_RTC_EnableInt(MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {}
    }

    // Enable RTC as a wakeup source from low power modes
    MXC_LP_EnableRTCAlarmWakeup();
}

// *****************************************************************************
void printTime()
{
    uint32_t day, hr, min, sec;

    // Get current time (don't care about Sub-second count here)
    while (MXC_RTC_GetSeconds(&sec) != E_NO_ERROR) {}

    day = sec / SECS_PER_DAY;
    sec -= day * SECS_PER_DAY;

    hr = sec / SECS_PER_HR;
    sec -= hr * SECS_PER_HR;

    min = sec / SECS_PER_MIN;
    sec -= min * SECS_PER_MIN;

    printf("\nCurrent Time (dd:hh:mm:ss): %02d:%02d:%02d:%02d\n", day, hr, min, sec);
}

// *****************************************************************************
int configureRTC()
{
    int rtcTrim;

    printf("\n\n***************** RTC Wake from Backup Example *****************\n\n");
    printf("The time-of-day alarm is set to wake the device every %d seconds.\n", TIME_OF_DAY_SEC);
    printf("When the alarm goes off it will print the current time to the console.\n\n");

    // Initialize RTC
    if (MXC_RTC_Init(0, 0) != E_NO_ERROR) {
        printf("Failed RTC Initialization\n");
        printf("Example Failed\n");
        while (1) {}
    }

    // Start RTC
    if (MXC_RTC_Start() != E_NO_ERROR) {
        printf("Failed RTC_Start\n");
        printf("Example Failed\n");
        while (1) {}
    }

    printf("RTC started\n");

    // Trim RTC
    rtcTrim = MXC_RTC_TrimCrystal(MXC_TMR0);
    if (rtcTrim < 0) {
        printf("Error trimming RTC %d\n", rtcTrim);
    }

    // Set the Time of Day Alarm
    if (MXC_RTC_SetTimeofdayAlarm(TIME_OF_DAY_SEC) != E_NO_ERROR) {
        printf("Failed RTC_SetTimeofdayAlarm\n");
        printf("Example Failed\n");
        while (1) {}
    }

    // Enable Time of Day Alarm interrupt
    if (MXC_RTC_EnableInt(MXC_F_RTC_CTRL_TOD_ALARM_IE) == E_BUSY) {
        return E_BUSY;
    }

    // Re-start RTC
    if (MXC_RTC_Start() != E_NO_ERROR) {
        printf("Failed RTC_Start\n");
        printf("Example Failed\n");
        while (1) {}
    }

    return E_NO_ERROR;
}

// *****************************************************************************
int main(void)
{
    if (!(MXC_PWRSEQ->lppwkst & MXC_F_PWRSEQ_LPPWKST_BACKUP)) {
        // Did not wake from backup mode --> start RTC
        if (configureRTC() != E_NO_ERROR) {
            printf("Example Failed\n");
            while (1) {}
        }
    } else {
        // Woke up from backup mode --> Reset backup status and print time
        MXC_PWRSEQ->lppwkst |= MXC_F_PWRSEQ_LPPWKST_BACKUP;

        LED_On(LED_TODA); // RTC alarm fired off. Perform periodic task here
        printTime();
    }

    // (Re)arm RTC TOD alarm
    rescheduleAlarm();
    MXC_Delay(MXC_DELAY_SEC(1)); // Prevent bricks

    // Prepare for entering backup mode
    LED_Off(LED_TODA);
    while (MXC_UART_ReadyForSleep(MXC_UART_GET_UART(CONSOLE_UART)) != E_NO_ERROR) {}

    // Enter backup mode and wait for RTC to send wakeup signal
    MXC_LP_EnterBackupMode();
    while (1) {}

    return 0;
}
