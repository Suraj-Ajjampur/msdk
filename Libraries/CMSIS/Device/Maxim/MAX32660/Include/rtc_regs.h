/**
 * @file    rtc_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the RTC Peripheral Module.
 * @note    This file is @generated.
 * @ingroup rtc_registers
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

#ifndef LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32660_INCLUDE_RTC_REGS_H_
#define LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32660_INCLUDE_RTC_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__ICCARM__)
  #pragma system_include
#endif

#if defined (__CC_ARM)
  #pragma anon_unions
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
 * @ingroup     rtc
 * @defgroup    rtc_registers RTC_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the RTC Peripheral Module.
 * @details     Real Time Clock and Alarm.
 */

/**
 * @ingroup rtc_registers
 * Structure type to access the RTC Registers.
 */
typedef struct {
    __IO uint32_t sec;                  /**< <tt>\b 0x00:</tt> RTC SEC Register */
    __IO uint32_t ssec;                 /**< <tt>\b 0x04:</tt> RTC SSEC Register */
    __IO uint32_t ras;                  /**< <tt>\b 0x08:</tt> RTC RAS Register */
    __IO uint32_t rssa;                 /**< <tt>\b 0x0C:</tt> RTC RSSA Register */
    __IO uint32_t ctrl;                 /**< <tt>\b 0x10:</tt> RTC CTRL Register */
    __IO uint32_t trim;                 /**< <tt>\b 0x14:</tt> RTC TRIM Register */
    __IO uint32_t oscctrl;              /**< <tt>\b 0x18:</tt> RTC OSCCTRL Register */
} mxc_rtc_regs_t;

/* Register offsets for module RTC */
/**
 * @ingroup    rtc_registers
 * @defgroup   RTC_Register_Offsets Register Offsets
 * @brief      RTC Peripheral Register Offsets from the RTC Base Peripheral Address.
 * @{
 */
#define MXC_R_RTC_SEC                      ((uint32_t)0x00000000UL) /**< Offset from RTC Base Address: <tt> 0x0000</tt> */
#define MXC_R_RTC_SSEC                     ((uint32_t)0x00000004UL) /**< Offset from RTC Base Address: <tt> 0x0004</tt> */
#define MXC_R_RTC_RAS                      ((uint32_t)0x00000008UL) /**< Offset from RTC Base Address: <tt> 0x0008</tt> */
#define MXC_R_RTC_RSSA                     ((uint32_t)0x0000000CUL) /**< Offset from RTC Base Address: <tt> 0x000C</tt> */
#define MXC_R_RTC_CTRL                     ((uint32_t)0x00000010UL) /**< Offset from RTC Base Address: <tt> 0x0010</tt> */
#define MXC_R_RTC_TRIM                     ((uint32_t)0x00000014UL) /**< Offset from RTC Base Address: <tt> 0x0014</tt> */
#define MXC_R_RTC_OSCCTRL                  ((uint32_t)0x00000018UL) /**< Offset from RTC Base Address: <tt> 0x0018</tt> */
/**@} end of group rtc_registers */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_SEC RTC_SEC
 * @brief    RTC Second Counter. This register contains the 32-bit second counter.
 * @{
 */
#define MXC_F_RTC_SEC_SEC_POS                          0 /**< SEC_SEC Position */
#define MXC_F_RTC_SEC_SEC                              ((uint32_t)(0xFFFFFFFFUL << MXC_F_RTC_SEC_SEC_POS)) /**< SEC_SEC Mask */

/**@} end of group RTC_SEC_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_SSEC RTC_SSEC
 * @brief    RTC Sub-second Counter. This counter increments at 256Hz. RTC_SEC is incremented
 *           when this register rolls over from 0xFF to 0x00.
 * @{
 */
#define MXC_F_RTC_SSEC_RTSS_POS                        0 /**< SSEC_RTSS Position */
#define MXC_F_RTC_SSEC_RTSS                            ((uint32_t)(0xFFUL << MXC_F_RTC_SSEC_RTSS_POS)) /**< SSEC_RTSS Mask */

/**@} end of group RTC_SSEC_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_RAS RTC_RAS
 * @brief    Time-of-day Alarm.
 * @{
 */
#define MXC_F_RTC_RAS_RAS_POS                          0 /**< RAS_RAS Position */
#define MXC_F_RTC_RAS_RAS                              ((uint32_t)(0xFFFFFUL << MXC_F_RTC_RAS_RAS_POS)) /**< RAS_RAS Mask */

/**@} end of group RTC_RAS_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_RSSA RTC_RSSA
 * @brief    RTC sub-second alarm.  This register contains the reload value for the sub-
 *           second alarm.
 * @{
 */
#define MXC_F_RTC_RSSA_RSSA_POS                        0 /**< RSSA_RSSA Position */
#define MXC_F_RTC_RSSA_RSSA                            ((uint32_t)(0xFFFFFFFFUL << MXC_F_RTC_RSSA_RSSA_POS)) /**< RSSA_RSSA Mask */

/**@} end of group RTC_RSSA_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_CTRL RTC_CTRL
 * @brief    RTC Control Register.
 * @{
 */
#define MXC_F_RTC_CTRL_RTCE_POS                        0 /**< CTRL_RTCE Position */
#define MXC_F_RTC_CTRL_RTCE                            ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_RTCE_POS)) /**< CTRL_RTCE Mask */

#define MXC_F_RTC_CTRL_ADE_POS                         1 /**< CTRL_ADE Position */
#define MXC_F_RTC_CTRL_ADE                             ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_ADE_POS)) /**< CTRL_ADE Mask */

#define MXC_F_RTC_CTRL_ASE_POS                         2 /**< CTRL_ASE Position */
#define MXC_F_RTC_CTRL_ASE                             ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_ASE_POS)) /**< CTRL_ASE Mask */

#define MXC_F_RTC_CTRL_BUSY_POS                        3 /**< CTRL_BUSY Position */
#define MXC_F_RTC_CTRL_BUSY                            ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_BUSY_POS)) /**< CTRL_BUSY Mask */

#define MXC_F_RTC_CTRL_RDY_POS                         4 /**< CTRL_RDY Position */
#define MXC_F_RTC_CTRL_RDY                             ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_RDY_POS)) /**< CTRL_RDY Mask */

#define MXC_F_RTC_CTRL_RDYE_POS                        5 /**< CTRL_RDYE Position */
#define MXC_F_RTC_CTRL_RDYE                            ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_RDYE_POS)) /**< CTRL_RDYE Mask */

#define MXC_F_RTC_CTRL_ALDF_POS                        6 /**< CTRL_ALDF Position */
#define MXC_F_RTC_CTRL_ALDF                            ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_ALDF_POS)) /**< CTRL_ALDF Mask */

#define MXC_F_RTC_CTRL_ALSF_POS                        7 /**< CTRL_ALSF Position */
#define MXC_F_RTC_CTRL_ALSF                            ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_ALSF_POS)) /**< CTRL_ALSF Mask */

#define MXC_F_RTC_CTRL_SQE_POS                         8 /**< CTRL_SQE Position */
#define MXC_F_RTC_CTRL_SQE                             ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_SQE_POS)) /**< CTRL_SQE Mask */

#define MXC_F_RTC_CTRL_FT_POS                          9 /**< CTRL_FT Position */
#define MXC_F_RTC_CTRL_FT                              ((uint32_t)(0x3UL << MXC_F_RTC_CTRL_FT_POS)) /**< CTRL_FT Mask */
#define MXC_V_RTC_CTRL_FT_FREQ1HZ                      ((uint32_t)0x0UL) /**< CTRL_FT_FREQ1HZ Value */
#define MXC_S_RTC_CTRL_FT_FREQ1HZ                      (MXC_V_RTC_CTRL_FT_FREQ1HZ << MXC_F_RTC_CTRL_FT_POS) /**< CTRL_FT_FREQ1HZ Setting */
#define MXC_V_RTC_CTRL_FT_FREQ512HZ                    ((uint32_t)0x1UL) /**< CTRL_FT_FREQ512HZ Value */
#define MXC_S_RTC_CTRL_FT_FREQ512HZ                    (MXC_V_RTC_CTRL_FT_FREQ512HZ << MXC_F_RTC_CTRL_FT_POS) /**< CTRL_FT_FREQ512HZ Setting */
#define MXC_V_RTC_CTRL_FT_FREQ4KHZ                     ((uint32_t)0x2UL) /**< CTRL_FT_FREQ4KHZ Value */
#define MXC_S_RTC_CTRL_FT_FREQ4KHZ                     (MXC_V_RTC_CTRL_FT_FREQ4KHZ << MXC_F_RTC_CTRL_FT_POS) /**< CTRL_FT_FREQ4KHZ Setting */
#define MXC_V_RTC_CTRL_FT_CLKDIV8                      ((uint32_t)0x3UL) /**< CTRL_FT_CLKDIV8 Value */
#define MXC_S_RTC_CTRL_FT_CLKDIV8                      (MXC_V_RTC_CTRL_FT_CLKDIV8 << MXC_F_RTC_CTRL_FT_POS) /**< CTRL_FT_CLKDIV8 Setting */

#define MXC_F_RTC_CTRL_X32KMD_POS                      11 /**< CTRL_X32KMD Position */
#define MXC_F_RTC_CTRL_X32KMD                          ((uint32_t)(0x3UL << MXC_F_RTC_CTRL_X32KMD_POS)) /**< CTRL_X32KMD Mask */
#define MXC_V_RTC_CTRL_X32KMD_NOISEIMMUNEMODE          ((uint32_t)0x0UL) /**< CTRL_X32KMD_NOISEIMMUNEMODE Value */
#define MXC_S_RTC_CTRL_X32KMD_NOISEIMMUNEMODE          (MXC_V_RTC_CTRL_X32KMD_NOISEIMMUNEMODE << MXC_F_RTC_CTRL_X32KMD_POS) /**< CTRL_X32KMD_NOISEIMMUNEMODE Setting */
#define MXC_V_RTC_CTRL_X32KMD_QUIETMODE                ((uint32_t)0x1UL) /**< CTRL_X32KMD_QUIETMODE Value */
#define MXC_S_RTC_CTRL_X32KMD_QUIETMODE                (MXC_V_RTC_CTRL_X32KMD_QUIETMODE << MXC_F_RTC_CTRL_X32KMD_POS) /**< CTRL_X32KMD_QUIETMODE Setting */
#define MXC_V_RTC_CTRL_X32KMD_QUIETINSTOPWITHWARMUP    ((uint32_t)0x2UL) /**< CTRL_X32KMD_QUIETINSTOPWITHWARMUP Value */
#define MXC_S_RTC_CTRL_X32KMD_QUIETINSTOPWITHWARMUP    (MXC_V_RTC_CTRL_X32KMD_QUIETINSTOPWITHWARMUP << MXC_F_RTC_CTRL_X32KMD_POS) /**< CTRL_X32KMD_QUIETINSTOPWITHWARMUP Setting */
#define MXC_V_RTC_CTRL_X32KMD_QUIETINSTOPNOWARMUP      ((uint32_t)0x3UL) /**< CTRL_X32KMD_QUIETINSTOPNOWARMUP Value */
#define MXC_S_RTC_CTRL_X32KMD_QUIETINSTOPNOWARMUP      (MXC_V_RTC_CTRL_X32KMD_QUIETINSTOPNOWARMUP << MXC_F_RTC_CTRL_X32KMD_POS) /**< CTRL_X32KMD_QUIETINSTOPNOWARMUP Setting */

#define MXC_F_RTC_CTRL_WE_POS                          15 /**< CTRL_WE Position */
#define MXC_F_RTC_CTRL_WE                              ((uint32_t)(0x1UL << MXC_F_RTC_CTRL_WE_POS)) /**< CTRL_WE Mask */

/**@} end of group RTC_CTRL_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_TRIM RTC_TRIM
 * @brief    RTC Trim Register.
 * @{
 */
#define MXC_F_RTC_TRIM_TRIM_POS                        0 /**< TRIM_TRIM Position */
#define MXC_F_RTC_TRIM_TRIM                            ((uint32_t)(0xFFUL << MXC_F_RTC_TRIM_TRIM_POS)) /**< TRIM_TRIM Mask */

#define MXC_F_RTC_TRIM_VBATTMR_POS                     8 /**< TRIM_VBATTMR Position */
#define MXC_F_RTC_TRIM_VBATTMR                         ((uint32_t)(0xFFFFFFUL << MXC_F_RTC_TRIM_VBATTMR_POS)) /**< TRIM_VBATTMR Mask */

/**@} end of group RTC_TRIM_Register */

/**
 * @ingroup  rtc_registers
 * @defgroup RTC_OSCCTRL RTC_OSCCTRL
 * @brief    RTC Oscillator Control Register.
 * @{
 */
#define MXC_F_RTC_OSCCTRL_FLITER_EN_POS                0 /**< OSCCTRL_FLITER_EN Position */
#define MXC_F_RTC_OSCCTRL_FLITER_EN                    ((uint32_t)(0x1UL << MXC_F_RTC_OSCCTRL_FLITER_EN_POS)) /**< OSCCTRL_FLITER_EN Mask */

#define MXC_F_RTC_OSCCTRL_IBIAS_SEL_POS                1 /**< OSCCTRL_IBIAS_SEL Position */
#define MXC_F_RTC_OSCCTRL_IBIAS_SEL                    ((uint32_t)(0x1UL << MXC_F_RTC_OSCCTRL_IBIAS_SEL_POS)) /**< OSCCTRL_IBIAS_SEL Mask */

#define MXC_F_RTC_OSCCTRL_HYST_EN_POS                  2 /**< OSCCTRL_HYST_EN Position */
#define MXC_F_RTC_OSCCTRL_HYST_EN                      ((uint32_t)(0x1UL << MXC_F_RTC_OSCCTRL_HYST_EN_POS)) /**< OSCCTRL_HYST_EN Mask */

#define MXC_F_RTC_OSCCTRL_IBIAS_EN_POS                 3 /**< OSCCTRL_IBIAS_EN Position */
#define MXC_F_RTC_OSCCTRL_IBIAS_EN                     ((uint32_t)(0x1UL << MXC_F_RTC_OSCCTRL_IBIAS_EN_POS)) /**< OSCCTRL_IBIAS_EN Mask */

#define MXC_F_RTC_OSCCTRL_BYPASS_POS                   4 /**< OSCCTRL_BYPASS Position */
#define MXC_F_RTC_OSCCTRL_BYPASS                       ((uint32_t)(0x1UL << MXC_F_RTC_OSCCTRL_BYPASS_POS)) /**< OSCCTRL_BYPASS Mask */

#define MXC_F_RTC_OSCCTRL_OUT32K_POS                   5 /**< OSCCTRL_OUT32K Position */
#define MXC_F_RTC_OSCCTRL_OUT32K                       ((uint32_t)(0x1UL << MXC_F_RTC_OSCCTRL_OUT32K_POS)) /**< OSCCTRL_OUT32K Mask */

/**@} end of group RTC_OSCCTRL_Register */

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32660_INCLUDE_RTC_REGS_H_
