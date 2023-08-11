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

#ifndef __TIMER_MODULE_FOR_LIGHT_SENSOR__
#define __TIMER_MODULE_FOR_LIGHT_SENSOR__

/**************************************************************************************************
  Header Files
**************************************************************************************************/
#include "mxc_device.h"
#include "mxc_sys.h"
#include "nvic_table.h"
#include "gpio.h"
#include "board.h"
#include "tmr.h"
#include "led.h"
#include "pb.h"
#include "lp.h"
#include "gcr_regs.h"
#include "pwrseq_regs.h"
#include "temt6000.h"
#include "dats_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define INTERVAL_TIME_CONT 1 // (s) will toggle after every interval
#define CONT_TIMER MXC_TMR1 // Can be MXC_TMR0 through MXC_TMR5
#define CONT_TIMER_IRQn TMR1_IRQn
#define TIMER_PRIORITY 1 
#define CLOCK_DIVISION_SCALE 4

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief  Continuous Timer Handler to read the Light sensor value
 *
 *  \return None.
 */
/*************************************************************************************************/
void ContinuousTimerHandler(void);

/*************************************************************************************************/
/*!
 *  \brief  Function to initialize Continuous Timer
 *
 *  \return None.
 */
/*************************************************************************************************/
void ContinuousTimerInit(void);

#endif // __TIMER_MODULE_FOR_LIGHT_SENSOR__