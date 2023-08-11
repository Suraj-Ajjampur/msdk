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


/**
 * @ingroup  MAX31825_opcodes
 * @brief    MAX31825_opcodes
 * @{
 */
#define SKIP_ROM            0xCC
#define COMMAND_T           0x44
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCRATCHPAD    0x4E
#define DETECT_ADDRESS      0x88
#define SELECT_ADDRESS      0x70

/**
 * @ingroup  MAX31825_Conv_rate
 * @brief    MAX31825 Conversion_Rate
 * @{
 */
#define SHUTDOWN                0
#define ONCE_IN_64_SEC          1
#define ONCE_IN_32_SEC          2
#define ONCE_IN_16_SEC          3
#define ONCE_IN_4_SEC           4
#define ONCE_IN_1_SEC           5
#define FOUR_IN_1_SEC           6
#define EIGHT_IN_1_SEC          7
/**@} end of group  MXC_F_MAX31825_CONV_RATE*/

/**
 * @ingroup  MAX31825_CONFIG_REGISTER
 * @brief    MAX31825 Configuration register
 * @{
 */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS                      0 /**< CONFIG_REGISTER_CONV_RATE Position */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_SHUTDOWN                 ((uint8_t)(SHUTDOWN << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_ONCE_IN_64_SEC           ((uint8_t)(ONCE_IN_64_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_ONCE_IN_32_SEC           ((uint8_t)(ONCE_IN_32_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_ONCE_IN_16_SEC           ((uint8_t)(ONCE_IN_16_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_ONCE_IN_4_SEC            ((uint8_t)(ONCE_IN_4_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_ONCE_IN_1_SEC            ((uint8_t)(ONCE_IN_1_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_FOUR_IN_1_SEC            ((uint8_t)(FOUR_IN_1_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_EIGHT_IN_1_SEC           ((uint8_t)(EIGHT_IN_1_SEC << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */
#define MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_MASK                     ((uint8_t)(7 << MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_POS))  /**< CONFIG_REGISTER_CONV_RATE Mask */

#define MXC_MAX31825_CONFIG_REGISTER_COMPT_INT_POS          4 /**< CONFIG_REGISTER_COMPT_INT Position */
#define MXC_MAX31825_CONFIG_REGISTER_COMPT_INT              ((uint8_t)(1 << MXC_MAX31825_CONFIG_REGISTER_COMPT_INT_POS))  /**< CONFIG_REGISTER_COMPT_INT Mask */

#define MXC_MAX31825_CONFIG_REGISTER_RESOLUTION_POS         5 /**< CONFIG_REGISTER_RESOLUTION Position */
#define MXC_MAX31825_CONFIG_REGISTER_RESOLUTION_MASK       ((uint8_t)(3 << MXC_MAX31825_CONFIG_REGISTER_RESOLUTION_POS)) /**< CONFIG_REGISTER_RESOLUTION Mask */

#define MXC_MAX31825_RESOLUTION_12_BITS                    3 
#define MXC_MAX31825_RESOLUTION_10_BITS                    2
#define MXC_MAX31825_RESOLUTION_9_BITS                     1
#define MXC_MAX31825_RESOLUTION_8_BITS                     0 

#define RESOLUTION(x)                                       ((uint8_t)(x << MXC_MAX31825_CONFIG_REGISTER_RESOLUTION_POS))

#define MXC_MAX31825_CONFIG_REGISTER_FORMAT_POS             7 /**< CONFIG_REGISTER_FORMAT Position */
#define MXC_MAX31825_CONFIG_REGISTER_FORMAT                 ((uint8_t)(1 << MXC_MAX31825_CONFIG_REGISTER_FORMAT_POS))     /**< CONFIG_REGISTER_FORMAT Mask */

/**@} end of group  MXC_F_MAX31825_CONFIG_REGISTER*/



/** Scratchpad containing control and data registers of the sensor
 * 
*/
typedef struct{
    uint16_t temp;
    uint16_t TH;
    uint16_t TL;
    uint8_t config;
    uint8_t crc;
} scratchpad_t;

/************************* Function Pointers ****************************/

int32_t OW_MAX31825_Test(void);

/** Reads the 9-byte scratchpad including the CRC byte
 * 
 * @return error code
*/
int ReadScratchPad(void);

/** Getter function for the temperature value in celcius
 * 
*/
uint16_t get_max31825_temp(void);

/** Issues the read scratchpad command
 * 
*/
int32_t Read_Max31825_temp(void);

int OW_MAX31825_Test1(void);

int DetectAddress(void);

#endif // LIBRARIES_MISCDRIVERS_TEMPSENSOR_MAX31825_DRIVER_H_
