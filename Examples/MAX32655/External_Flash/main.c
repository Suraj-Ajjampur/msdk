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
 * @file    main.c
 * @brief   External flash example.
 * @details Demonstrate how external flash can be driven over SPI interface. 
 *          To see low level SPI function call, see board.c file
 *          Erases, writes, and then verifies the data. 
 *          EXT_FLASH_BAUD, EXT_FLASH_ADDR, and EXT_FLASH_SPIXFC_WIDTH
 *          can be changed to alter the communication between the devices. Refer
 *          to the schematic for the pinout and ensure that there are no switches
 *          blocking the communication to the external flash.
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_sys.h"
#include "mxc_errors.h"
#include "max32655.h"
#include "Ext_Flash.h"
#include "board.h"
#include "led.h"

/***** Definitions *****/

#define EXT_FLASH_ADDR 0
#define EXT_FLASH_SPIXFC_WIDTH Ext_Flash_DataLine_Quad

#define BUFF_SIZE 64

/***** Functions *****/

static char data_to_write[] = { "Analog Devices" };
static char data_to_read[32];

/******************************************************************************/
int main(void)
{
    int fail = 0;
    uint32_t id;
    int rx_len = sizeof(data_to_write) / sizeof(char);
    int remain = rx_len;

    printf("\n\n********************* External Flash Example *********************\n");
    printf("This example communicates with an %s flash over SPI (Quad mode)\n\n", EXT_FLASH_NAME);

    printf("SPI Clock: %d Hz\n\n", EXT_FLASH_BAUD);

    // Initialize external flash
    if (Ext_Flash_Init() != E_NO_ERROR) {
        printf("Board Init Failed\n");
        printf("Example Failed\n");
        while (1) {}
    }
    printf("External flash Initialized.\n\n");

    Ext_Flash_Reset();

    // Get the ID of the external flash
    id = Ext_Flash_ID();
    printf("External flash ID: 0x%x\n", id);

    int err;

    // Erase Test Sector
    printf("Erasing first 64k sector\n");
    Ext_Flash_Erase(0x00000, Ext_Flash_Erase_64K);
    printf("Erased\n\n");

    // Enable Quad mode if we are using quad
    if (EXT_FLASH_SPIXFC_WIDTH == Ext_Flash_DataLine_Quad) {
        if (Ext_Flash_Quad(1) != E_NO_ERROR) {
            printf("Error enabling quad mode\n\n");
            fail++;
        } else {
            printf("Quad mode enabled\n\n");
        }
    } else {
        if (Ext_Flash_Quad(0) != E_NO_ERROR) {
            printf("Error disabling quad mode\n\n");
            fail++;
        } else {
            printf("Quad mode disabled\n\n");
        }
    }

    // Program the external flash
    printf("Programming function (%d bytes @ 0x%08x) into external flash\n",
           (uint32_t)(sizeof(data_to_write)), data_to_write);

    if ((err = Ext_Flash_Program_Page(EXT_FLASH_ADDR, (uint8_t *)data_to_write,
                                      (uint32_t)(sizeof(data_to_write) / sizeof(char)),
                                      EXT_FLASH_SPIXFC_WIDTH)) != E_NO_ERROR) {
        printf("Error Programming: %d\n", err);
        fail++;
    } else {
        printf("Programmed\n");
        printf("Written Data:%s\n\n", data_to_write);
    }

    printf("Verifying external flash\n");
    while (remain) {
        int chunk = ((remain > BUFF_SIZE) ? BUFF_SIZE : remain);
        if ((err = Ext_Flash_Read(EXT_FLASH_ADDR + rx_len - remain, (uint8_t *)data_to_read, chunk,
                                  EXT_FLASH_SPIXFC_WIDTH)) != E_NO_ERROR) {
            printf("Error verifying data %d\n", err);
            fail++;
            break;
        } else if (memcmp(data_to_read, data_to_write + rx_len - remain, chunk) != E_NO_ERROR) {
            printf("Error invalid data\n");
            fail++;
            break;
        } else if (remain == chunk) {
            printf("Verified\n");
            printf("Read Data:%s\n\n", data_to_read);
        }
        remain -= chunk;
    }

    if (fail != 0) {
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
