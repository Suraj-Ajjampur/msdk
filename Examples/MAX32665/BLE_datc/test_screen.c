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

#include "test_screen.h"
#include<stdio.h>

// LVGL SCREEN VARIABLES
static lv_obj_t *label1;
static lv_obj_t *img1;
static lv_obj_t *temp;
static lv_obj_t *val;

lv_obj_t *chart;
lv_chart_series_t *ser;

//Fetch decrypted temperature value
extern int temp_in_c;
extern uint32_t LightValue;

/**
 * Display 1000 data points with zooming and scrolling.
 * See how the chart changes drawing mode (draw only vertical lines) when
 * the points get too crowded.
 */
void test_screen(void)
{
    LV_IMG_DECLARE(adi_logo);
    img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &adi_logo);
    lv_obj_align(img1, LV_ALIGN_CENTER, -40, -45);

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, "Analog\nDevices");
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 10, -45);

/***** Temperature value update *****************/
    //  Need to convert the temp_in_c integer into a string in order to pass into lv_label_set_text()
    //  Calculate length of the size of string 

    char Temp_str[5] = "TEMP";

    int length = snprintf( NULL, 0, "%d", temp_in_c );
    char* str = (char *)malloc( length + 1 );
    snprintf( str, length + 1, "%d", temp_in_c );

    temp = lv_label_create(lv_scr_act());
    lv_label_set_text(temp, Temp_str);
    lv_obj_set_style_text_align(temp, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp, LV_ALIGN_CENTER, 0, 0);

    lv_obj_clean(temp);
 
    val = lv_label_create(lv_scr_act());
    lv_label_set_text(val, str);
    lv_obj_set_style_text_align(val, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(val, LV_ALIGN_CENTER, 0, 20);

    lv_obj_clean(val);
    free(str);
}

void update_sensor_value(char* text_str, int value, int horText, int horValue){

    temp = lv_label_create(lv_scr_act());
    lv_label_set_text(temp, text_str);
    lv_obj_set_style_text_align(temp, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(temp, LV_ALIGN_CENTER, 0, horText);

    int length = snprintf( NULL, 0, "%d", value );
    char* str = (char *)malloc( length + 1 );
    snprintf( str, length + 1, "%d", value );

    val = lv_label_create(lv_scr_act());
    lv_label_set_text(val, str);
    lv_obj_set_style_text_align(val, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(val, LV_ALIGN_CENTER, 0, horValue);
    free(str);

}

void update_screen(void){

    lv_obj_clean(lv_scr_act()); //Clear the screen

    LV_IMG_DECLARE(adi_logo);
    img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &adi_logo);
    lv_obj_align(img1, LV_ALIGN_CENTER, -40, -45);

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, "Analog\nDevices");
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 10, -45);
    char temp_str[5] = "TEMP";
    char light_str[6] = "LIGHT";
    update_sensor_value(light_str, LightValue, -10, 5);
    update_sensor_value(temp_str, temp_in_c, 20, 35);

}