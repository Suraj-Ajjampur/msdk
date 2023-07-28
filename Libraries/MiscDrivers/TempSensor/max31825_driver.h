/**
 * @file    max31825_driver.h
 * @brief   MAX31825 IC driver header
 * @details Defines MAX31825 registers
 *          Implements helper macros
 **/

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

#include <stdio.h>
#include <string.h>

#include "mxc_device.h"
#include "owm.h"
#include "board.h"
#include "mxc_delay.h"

#ifndef LIBRARIES_MISCDRIVERS_TEMPSENSOR_MAX31825_DRIVER_H_
#define LIBRARIES_MISCDRIVERS_TEMPSENSOR_MAX31825_DRIVER_H_


// MAX31825 Commands
#define SKIP_ROM            0xCC
#define COMMAND_T           0x44
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCRATCHPAD    0x4E
#define DETECT_ADDRESS      0x88
#define SELECT_ADDRESS      0x70

int32_t OW_MAX31825_Test(void);


#endif // LIBRARIES_MISCDRIVERS_TEMPSENSOR_MAX31825_DRIVER_H_
