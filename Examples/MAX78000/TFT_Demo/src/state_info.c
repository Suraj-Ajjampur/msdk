/*
 * @file state_info.c
 *
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
#include <string.h>

#include "bitmap.h"
#include "keypad.h"
#include "state.h"
#include "utils.h"
#include "example_config.h"

//
#define urw_gothic_16_white_bg_grey 0

/********************************** Type Defines  *****************************/
typedef void (*ScreenFunc)(void);

/************************************ VARIABLES ******************************/
static void screen_info(void);
static void screen_info1(void);
static void screen_info2(void);
static void screen_info3(void);

static ScreenFunc info_screens[] = { screen_info, screen_info1, screen_info2, screen_info3 };
static unsigned int screen_index = 0;

static text_t screen_msg[] = {
    // info
    { (char *)"INFORMATION", 11 },
    { (char *)"Scroll through pages", 20 },
    { (char *)"TEXT 1", 6 },
    { (char *)"TEXT 2", 6 },
    { (char *)"TEXT 3", 6 },
    // pages 1
    { (char *)"Cortex M4 @ 100MHz", 18 },
    { (char *)"512KB Flash, 128KB SRAM", 23 },
    { (char *)"32-bit RISC-V @ 60MHz", 21 },
    // pages 2
    { (char *)"CNN accelerator", 15 },
    { (char *)"Hardware crypto blocks", 22 },
    { (char *)"Secure Bootloader", 17 },
    // pages 3
    { (char *)"CNN examples", 12 },
    { (char *)"Camera support", 14 },
    { (char *)"FreeRTOS OS", 11 }
};

/********************************* Static Functions **************************/
static void screen_info(void)
{
    MXC_TFT_SetPalette(logo_white_bg_darkgrey_bmp);
    MXC_TFT_SetBackGroundColor(4);

    MXC_TFT_ShowImage(11, 7, logo_white_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 80, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 110, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 140, check_success_bg_darkgrey_bmp);

    MXC_TFT_PrintFont(110, 12, urw_gothic_16_white_bg_grey, &screen_msg[0], NULL); // information
    MXC_TFT_PrintFont(50, 50, urw_gothic_12_white_bg_grey, &screen_msg[1],
                      NULL); // scroll through pages
    // texts
    MXC_TFT_PrintFont(46, 80, urw_gothic_16_white_bg_grey, &screen_msg[2], NULL); // text 1
    MXC_TFT_PrintFont(46, 110, urw_gothic_16_white_bg_grey, &screen_msg[3], NULL); // text 2
    MXC_TFT_PrintFont(46, 140, urw_gothic_16_white_bg_grey, &screen_msg[4], NULL); // text 3

    MXC_TFT_ShowImage(12, 191, left_arrow_bmp);
    MXC_TFT_ShowImage(135, 191, home_bmp);
    MXC_TFT_ShowImage(259, 191, right_arrow_bmp);

#ifdef ENABLE_TS
    MXC_TS_RemoveAllButton();
    MXC_TS_AddButton(12, 191, 12 + 48, 191 + 39, KEY_1);
    MXC_TS_AddButton(135, 191, 135 + 48, 191 + 39, KEY_2);
    MXC_TS_AddButton(259, 191, 259 + 48, 191 + 39, KEY_3);
#endif
}

static void screen_info1(void)
{
    MXC_TFT_SetBackGroundColor(4);

    MXC_TFT_ShowImage(11, 7, logo_white_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 80, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 110, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 140, check_success_bg_darkgrey_bmp);

    MXC_TFT_PrintFont(110, 12, urw_gothic_16_white_bg_grey, &screen_msg[0], NULL); // information
    MXC_TFT_PrintFont(50, 50, urw_gothic_12_white_bg_grey, &screen_msg[1],
                      NULL); // scroll through pages
    // texts
    MXC_TFT_PrintFont(46, 80, urw_gothic_12_white_bg_grey, &screen_msg[5], NULL); // text 1
    MXC_TFT_PrintFont(46, 110, urw_gothic_12_white_bg_grey, &screen_msg[6], NULL); // text 2
    MXC_TFT_PrintFont(46, 140, urw_gothic_12_white_bg_grey, &screen_msg[7], NULL); // text 3

    MXC_TFT_ShowImage(12, 191, left_arrow_bmp);
    MXC_TFT_ShowImage(135, 191, home_bmp);
    MXC_TFT_ShowImage(259, 191, right_arrow_bmp);
#ifdef ENABLE_TS
    MXC_TS_RemoveAllButton();
    MXC_TS_AddButton(12, 191, 12 + 48, 191 + 39, KEY_1);
    MXC_TS_AddButton(135, 191, 135 + 48, 191 + 39, KEY_2);
    MXC_TS_AddButton(259, 191, 259 + 48, 191 + 39, KEY_3);
#endif
}

static void screen_info2(void)
{
    MXC_TFT_SetBackGroundColor(4);

    MXC_TFT_ShowImage(11, 7, logo_white_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 80, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 110, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 140, check_success_bg_darkgrey_bmp);

    MXC_TFT_PrintFont(110, 12, urw_gothic_16_white_bg_grey, &screen_msg[0], NULL); // information
    MXC_TFT_PrintFont(50, 50, urw_gothic_12_white_bg_grey, &screen_msg[1],
                      NULL); // scroll through pages
    // texts
    MXC_TFT_PrintFont(46, 80, urw_gothic_12_white_bg_grey, &screen_msg[8], NULL); // text 1
    MXC_TFT_PrintFont(46, 110, urw_gothic_12_white_bg_grey, &screen_msg[9], NULL); // text 2
    MXC_TFT_PrintFont(46, 140, urw_gothic_12_white_bg_grey, &screen_msg[10], NULL); // text 3

    MXC_TFT_ShowImage(12, 191, left_arrow_bmp);
    MXC_TFT_ShowImage(135, 191, home_bmp);
    MXC_TFT_ShowImage(259, 191, right_arrow_bmp);

#ifdef ENABLE_TS
    MXC_TS_RemoveAllButton();
    MXC_TS_AddButton(12, 191, 12 + 48, 191 + 39, KEY_1);
    MXC_TS_AddButton(135, 191, 135 + 48, 191 + 39, KEY_2);
    MXC_TS_AddButton(259, 191, 259 + 48, 191 + 39, KEY_3);
#endif
}

static void screen_info3(void)
{
    MXC_TFT_SetBackGroundColor(4);

    MXC_TFT_ShowImage(11, 7, logo_white_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 80, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 110, check_success_bg_darkgrey_bmp);
    MXC_TFT_ShowImage(16, 140, check_success_bg_darkgrey_bmp);

    MXC_TFT_PrintFont(110, 12, urw_gothic_16_white_bg_grey, &screen_msg[0], NULL); // information
    MXC_TFT_PrintFont(50, 50, urw_gothic_12_white_bg_grey, &screen_msg[1],
                      NULL); // scroll through pages
    // texts
    MXC_TFT_PrintFont(46, 80, urw_gothic_12_white_bg_grey, &screen_msg[11], NULL); // text 1
    MXC_TFT_PrintFont(46, 110, urw_gothic_12_white_bg_grey, &screen_msg[12], NULL); // text 2
    MXC_TFT_PrintFont(46, 140, urw_gothic_12_white_bg_grey, &screen_msg[13], NULL); // text 3

    MXC_TFT_ShowImage(12, 191, left_arrow_bmp);
    MXC_TFT_ShowImage(135, 191, home_bmp);
    MXC_TFT_ShowImage(259, 191, right_arrow_bmp);

#ifdef ENABLE_TS
    MXC_TS_RemoveAllButton();
    MXC_TS_AddButton(12, 191, 12 + 48, 191 + 39, KEY_1);
    MXC_TS_AddButton(135, 191, 135 + 48, 191 + 39, KEY_2);
    MXC_TS_AddButton(259, 191, 259 + 48, 191 + 39, KEY_3);
#endif
}

static int init(void)
{
    screen_info();
    screen_index = 0;
    return 0;
}

static int key_process(int key)
{
    switch (key) {
    case KEY_1:
        if (screen_index > 0) {
            --screen_index;
            info_screens[screen_index]();
        }

        break;

    case KEY_2:
        state_set_current(get_home_state());
        break;

    case KEY_3:
        if (screen_index < (ARRAY_SIZE(info_screens) - 1)) {
            ++screen_index;
            info_screens[screen_index]();
        }

        break;

    case KEY_A:
        break;

    case KEY_B:
        break;

    case KEY_C:
        state_set_current(get_home_state());
        break;

    case KEY_D:
        break;

    case KEY_E:
        break;

    case KEY_F:
        break;

    default:
        break;
    }

    return 0;
}

static State g_state = { "info", init, key_process, NULL, 0 };

/********************************* Public Functions **************************/
State *get_info_state(void)
{
    return &g_state;
}
