/**
 * @file    bbtm_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the BBTM Peripheral Module.
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

#ifndef _BBTM_REGS_H_
#define _BBTM_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     bbtm
 * @defgroup    bbtm_registers Registers
 * @brief       Registers, Bit Masks and Bit Positions for the BBTM Peripheral Module.
 * @description Battery-Backed Test Modes
 */

/**
 * @ingroup bbtm_registers
 * Structure type to access the BBTM Registers.
 */
typedef struct {
    __IO uint32_t reg0;                 /**< <tt>\b 0x00:<\tt> BBTM REG0 Register */
    __IO uint32_t reg1;                 /**< <tt>\b 0x04:<\tt> BBTM REG1 Register */
} mxc_bbtm_regs_t;

/* Register offsets for module BBTM */
/**
 * @ingroup    bbtm_registers
 * @defgroup   BBTM_Register_Offsets Register Offsets
 * @brief      BBTM Peripheral Register Offsets from the BBTM Base Peripheral Address.
 * @{
 */
#define MXC_R_BBTM_REG0                    ((uint32_t)0x00000000UL) /**< Offset from BBTM Base Address: <tt> 0x0x000 */
#define MXC_R_BBTM_REG1                    ((uint32_t)0x00000004UL) /**< Offset from BBTM Base Address: <tt> 0x0x004 */
/**@} end of group bbtm_registers */

/**
 * @ingroup  bbtm_registers
 * @defgroup REG0_Register
 * @brief    Register 0
 * @{
 */
#define MXC_F_BBTM_REG0_ABUS_PAGE_POS                  (0) /**< REG0_ABUS_PAGE Position */
#define MXC_F_BBTM_REG0_ABUS_PAGE                      ((uint32_t)(0xF << MXC_F_BBTM_REG0_ABUS_PAGE_POS)) /**< REG0_ABUS_PAGE Mask */

#define MXC_F_BBTM_REG0_ABUS_PAGE_EN_POS               (4) /**< REG0_ABUS_PAGE_EN Position */
#define MXC_F_BBTM_REG0_ABUS_PAGE_EN                   ((uint32_t)(0x1 << MXC_F_BBTM_REG0_ABUS_PAGE_EN_POS)) /**< REG0_ABUS_PAGE_EN Mask */

/**@} end of group REG0_Register */

/**
 * @ingroup  bbtm_registers
 * @defgroup REG1_Register
 * @brief    Register 1
 * @{
 */
#define MXC_F_BBTM_REG1_ISOZ_VDDC_EVENT_POS            (0) /**< REG1_ISOZ_VDDC_EVENT Position */
#define MXC_F_BBTM_REG1_ISOZ_VDDC_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_ISOZ_VDDC_EVENT_POS)) /**< REG1_ISOZ_VDDC_EVENT Mask */

#define MXC_F_BBTM_REG1_ISOZ_VDDA_EVENT_POS            (1) /**< REG1_ISOZ_VDDA_EVENT Position */
#define MXC_F_BBTM_REG1_ISOZ_VDDA_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_ISOZ_VDDA_EVENT_POS)) /**< REG1_ISOZ_VDDA_EVENT Mask */

#define MXC_F_BBTM_REG1_ISOZ_VDDIO_EVENT_POS           (2) /**< REG1_ISOZ_VDDIO_EVENT Position */
#define MXC_F_BBTM_REG1_ISOZ_VDDIO_EVENT               ((uint32_t)(0x1 << MXC_F_BBTM_REG1_ISOZ_VDDIO_EVENT_POS)) /**< REG1_ISOZ_VDDIO_EVENT Mask */

#define MXC_F_BBTM_REG1_ISOZ_VDDIOH_EVENT_POS          (3) /**< REG1_ISOZ_VDDIOH_EVENT Position */
#define MXC_F_BBTM_REG1_ISOZ_VDDIOH_EVENT              ((uint32_t)(0x1 << MXC_F_BBTM_REG1_ISOZ_VDDIOH_EVENT_POS)) /**< REG1_ISOZ_VDDIOH_EVENT Mask */

#define MXC_F_BBTM_REG1_ISOZ_VIO_EVENT_POS             (4) /**< REG1_ISOZ_VIO_EVENT Position */
#define MXC_F_BBTM_REG1_ISOZ_VIO_EVENT                 ((uint32_t)(0x1 << MXC_F_BBTM_REG1_ISOZ_VIO_EVENT_POS)) /**< REG1_ISOZ_VIO_EVENT Mask */

#define MXC_F_BBTM_REG1_PORZ_VDDC_EVENT_POS            (5) /**< REG1_PORZ_VDDC_EVENT Position */
#define MXC_F_BBTM_REG1_PORZ_VDDC_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_PORZ_VDDC_EVENT_POS)) /**< REG1_PORZ_VDDC_EVENT Mask */

#define MXC_F_BBTM_REG1_PORZ_VDDA_EVENT_POS            (6) /**< REG1_PORZ_VDDA_EVENT Position */
#define MXC_F_BBTM_REG1_PORZ_VDDA_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_PORZ_VDDA_EVENT_POS)) /**< REG1_PORZ_VDDA_EVENT Mask */

#define MXC_F_BBTM_REG1_PORZ_VDDB_EVENT_POS            (7) /**< REG1_PORZ_VDDB_EVENT Position */
#define MXC_F_BBTM_REG1_PORZ_VDDB_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_PORZ_VDDB_EVENT_POS)) /**< REG1_PORZ_VDDB_EVENT Mask */

#define MXC_F_BBTM_REG1_PORZ_VRTC_EVENT_POS            (8) /**< REG1_PORZ_VRTC_EVENT Position */
#define MXC_F_BBTM_REG1_PORZ_VRTC_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_PORZ_VRTC_EVENT_POS)) /**< REG1_PORZ_VRTC_EVENT Mask */

#define MXC_F_BBTM_REG1_RSTZ_VDDC_EVENT_POS            (9) /**< REG1_RSTZ_VDDC_EVENT Position */
#define MXC_F_BBTM_REG1_RSTZ_VDDC_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_RSTZ_VDDC_EVENT_POS)) /**< REG1_RSTZ_VDDC_EVENT Mask */

#define MXC_F_BBTM_REG1_RSTZ_VDDA_EVENT_POS            (10) /**< REG1_RSTZ_VDDA_EVENT Position */
#define MXC_F_BBTM_REG1_RSTZ_VDDA_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_RSTZ_VDDA_EVENT_POS)) /**< REG1_RSTZ_VDDA_EVENT Mask */

#define MXC_F_BBTM_REG1_RSTZ_VDDB_EVENT_POS            (11) /**< REG1_RSTZ_VDDB_EVENT Position */
#define MXC_F_BBTM_REG1_RSTZ_VDDB_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_RSTZ_VDDB_EVENT_POS)) /**< REG1_RSTZ_VDDB_EVENT Mask */

#define MXC_F_BBTM_REG1_RSTZ_VDDIO_EVENT_POS           (12) /**< REG1_RSTZ_VDDIO_EVENT Position */
#define MXC_F_BBTM_REG1_RSTZ_VDDIO_EVENT               ((uint32_t)(0x1 << MXC_F_BBTM_REG1_RSTZ_VDDIO_EVENT_POS)) /**< REG1_RSTZ_VDDIO_EVENT Mask */

#define MXC_F_BBTM_REG1_RSTZ_VDDIOH_EVENT_POS          (13) /**< REG1_RSTZ_VDDIOH_EVENT Position */
#define MXC_F_BBTM_REG1_RSTZ_VDDIOH_EVENT              ((uint32_t)(0x1 << MXC_F_BBTM_REG1_RSTZ_VDDIOH_EVENT_POS)) /**< REG1_RSTZ_VDDIOH_EVENT Mask */

#define MXC_F_BBTM_REG1_RSTZ_VRTC_EVENT_POS            (14) /**< REG1_RSTZ_VRTC_EVENT Position */
#define MXC_F_BBTM_REG1_RSTZ_VRTC_EVENT                ((uint32_t)(0x1 << MXC_F_BBTM_REG1_RSTZ_VRTC_EVENT_POS)) /**< REG1_RSTZ_VRTC_EVENT Mask */

/**@} end of group REG1_Register */

/*******************************************************************************/
/*                                                        Battery Backed Test */
#define MXC_BASE_BBTM                    ((uint32_t)0x40005C00UL)
#define MXC_BBTM                         ((mxc_bbtm_regs_t*)MXC_BASE_BBTM)

#ifdef __cplusplus
}
#endif

#endif /* _BBTM_REGS_H_ */
