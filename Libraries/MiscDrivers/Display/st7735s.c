/**
 * @file    st7735s_drv.c
 * @brief   Sitronix ST7735S LCD controller driver
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

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "st7735s.h"

static st7735s_cfg_t panel_cfg;

int st7735s_pixel(uint32_t x, uint32_t y, uint32_t z)
{
    uint8_t tx_data[4];

    /* Memory Write */
    tx_data[0] = ST7735S_RAMWR;
    tx_data[1] = (x & 0x3f) << 2;
    tx_data[2] = (y & 0x3f) << 2;
    tx_data[3] = (z & 0x3f) << 2;

    return panel_cfg.sendfn(tx_data, 1, tx_data + 1, 3);
}

int st7735s_write_pixels(uint8_t *data, unsigned int len)
{
    uint8_t cmd = ST7735S_RAMWR;

    return panel_cfg.sendfn(&cmd, 1, data, len);
}

int st7735s_xyloc(uint8_t row, uint8_t col)
{
    int ret;
    uint8_t tx_data[5];

    /* Column Address Set */
    tx_data[0] = ST7735S_CASET;
    tx_data[1] = 0;
    tx_data[2] = 0x02 + col;
    tx_data[3] = 0;
    tx_data[4] = 0x81;

    ret = panel_cfg.sendfn(tx_data, 1, tx_data + 1, 4);
    if (ret != 0) {
        return ret;
    }

    /* Row Address Set */
    tx_data[0] = ST7735S_RASET;
    tx_data[1] = 0;
    tx_data[2] = 0x02 + row;
    tx_data[3] = 0;
    tx_data[4] = 0x81;

    ret = panel_cfg.sendfn(tx_data, 1, tx_data + 1, 4);

    return ret;
}

int st7735s_init(st7735s_cfg_t *cfg)
{
    unsigned int i;
    st7735s_regcfg_t *rc;

    if (cfg == NULL) {
        return -1;
    } else {
        memcpy(&panel_cfg, cfg, sizeof(st7735s_cfg_t));
    }

    /* External hardware reset chip needs some time before releasing the line.  */

    cfg->delayfn(500);

    /* Step through the register configuration */
    i = cfg->ncfgs;
    rc = cfg->regcfg;

    while (i != 0) {
        if (panel_cfg.sendfn(&rc->cmd, 1, rc->data, rc->len) != 0) {
            return -1;
        }
        if (rc->delay > 0) {
            cfg->delayfn(rc->delay);
        }
        rc++;
        i--;
    }

    return 0;
}
