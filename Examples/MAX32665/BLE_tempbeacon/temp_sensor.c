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

#include "temp_sensor.h"

//[Change] - Add description Initializing the Temperature Sensor - OWM
void Init_max31825(void){
    mxc_owm_cfg_t owm_cfg;
    owm_cfg.int_pu_en = 1;
    owm_cfg.ext_pu_mode = MXC_OWM_EXT_PU_ACT_HIGH;
    owm_cfg.long_line_mode = 0;

    #if defined(BOARD_FTHR) || defined(BOARD_FTHR2)
    MXC_OWM_Init(&owm_cfg, MAP_B); // 1-Wire pins P0.12/13
    #else
    MXC_OWM_Init(&owm_cfg, MAP_C); // 1-Wire pins P0.24/25
    #endif

    OW_MAX31825_Test1();
    
}

//[Change] - Add description Read the Temp Value 
uint16_t ReadTempVal(void)
{
    int retval = 0;

    retval = Read_Max31825_temp();   
    if (retval) {
        printf("Error Code: %d",retval);
        printf("Example Failed\n");
        return E_FAIL;
    }

    retval = Convert_T();
    if ( retval != 0){
        return retval;
    }

    return get_max31825_temp();
}
