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
 * @brief   Bootloader
 * @details Simple bootloader to verify and replace images.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "mxc_device.h"
#include "led.h"
#include "pb.h"
#include "board.h"
#include "mxc_delay.h"
#include "flc.h"
#include "Ext_Flash.h"
/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Addresses for the flash sections, defined in the linker file */
extern uint32_t _flash0;
extern uint32_t _flash1;

#define FLASH0_START ((uint32_t)&_flash0)
#define FLASH1_START ((uint32_t)&_flash1)
#define FLASH_ERASED_WORD 0xFFFFFFFF
#define CRC32_LEN 4
#define EXT_FLASH_BLOCK_SIZE 224

#define DELAY(loopCount) \
    for (i = 0; i < loopCount; i++) {}

/**************************************************************************************************
  Local Variables
**************************************************************************************************/
typedef struct {
    uint32_t fileLen;
    uint32_t fileCRC;
} fileHeader_t;
fileHeader_t fileHeader;

typedef enum { COPY_FILE_OP, CALC_CRC32_OP } externFileOp_t;
/**************************************************************************************************
  Functions
**************************************************************************************************/

/* Defined in boot_lower.S */
extern void Boot_Lower(void);

void ledSuccessPattern(void)
{
    /* Green LED blinks */
    volatile int i, j;
    for (j = 0; j < 10; j++) {
        LED_Toggle(1);
        DELAY(0xFFFFF);
    }
}
void ledFailPattern(void)
{
    /* Red LED blinks */
    volatile int i, j;
    for (j = 0; j < 10; j++) {
        LED_Toggle(0);
        DELAY(0xFFFFF);
    }
}

// http://home.thep.lu.se/~bjorn/crc/
/*************************************************************************************************/
/*!
 *  \brief  Create the CRC32 table.
 *
 *  \param  r       Index into the table
 *
 *  \return None.
 */
/*************************************************************************************************/
uint32_t crc32_for_byte(uint32_t r)
{
    for (int j = 0; j < 8; ++j) r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
    return r ^ (uint32_t)0xFF000000L;
}

/*************************************************************************************************/
/*!
 *  \brief  Calculate the CRC32 value for the given buffer.
 *
 *  \param  data    Pointer to the data.
 *  \param  n_bytes Number of bytes in the buffer.
 *  \param  crc     Pointer to store the result.
 *
 *  \return None.
 */
/*************************************************************************************************/
static uint32_t table[0x100] = { 0 };
void crc32(const void *data, size_t n_bytes, uint32_t *crc)
{
    if (!*table) {
        for (size_t i = 0; i < 0x100; ++i) {
            table[i] = crc32_for_byte(i);
        }
    }

    for (size_t i = 0; i < n_bytes; ++i) {
        *crc = table[(uint8_t)*crc ^ ((uint8_t *)data)[i]] ^ *crc >> 8;
    }
}

void bootError(void)
{
    /* Flash the failure LED */
    ledFailPattern();
    NVIC_SystemReset();
}

int flashPageErased(uint32_t *addr)
{
    /* Determine if this page is totally erased */
    int i;
    for (i = 0; i < (MXC_FLASH_PAGE_SIZE / 4); i++) {
        if (*(addr + i) != FLASH_ERASED_WORD) {
            return 0;
        }
    }

    return 1;
}

static int multiPageErase(uint8_t *address, uint32_t pages)
{
    int err;
    volatile uint32_t address32 = (uint32_t)address;
    address32 &= 0xFFFFF;

    while (pages) {
        err = MXC_FLC_PageErase((uint32_t)address);
        if (err != E_NO_ERROR) {
            return err;
        }

        address += MXC_FLASH_PAGE_SIZE;
        pages--;
    }

    return E_NO_ERROR;
}

static int flashWrite(uint32_t *address, uint32_t *data, uint32_t len)
{
    int err;

    while ((len / 16) > 0) {
        err = MXC_FLC_Write128((uint32_t)address, data);
        if (err != E_NO_ERROR) {
            return err;
        }
        len -= 16;
        address += 4;
        data += 4;
    }
    while (len) {
        err = MXC_FLC_Write32((uint32_t)address, *data);
        if (err != E_NO_ERROR) {
            return err;
        }
        len -= 4;
        address += 1;
        data += 1;
    }
    return E_NO_ERROR;
}

uint32_t externFileOperation(externFileOp_t fileOperation)
{
    uint32_t internalFlashStartingAddress = FLASH0_START;
    uint8_t extFlashBlockBuff[EXT_FLASH_BLOCK_SIZE] = { 0 };
    uint32_t startingAddress = 0x00000000 + sizeof(fileHeader_t);
    uint32_t fileLen = fileHeader.fileLen;
    uint32_t crcResult = 0;
    uint32_t err = 0;
    /* Read blocks from ext flash and perform desired fileOperation */
    while (fileLen >= EXT_FLASH_BLOCK_SIZE) {
        Ext_Flash_Read(startingAddress, extFlashBlockBuff, EXT_FLASH_BLOCK_SIZE,
                       Ext_Flash_DataLine_Quad);
        if (fileOperation == CALC_CRC32_OP) {
            crc32(extFlashBlockBuff, EXT_FLASH_BLOCK_SIZE, &crcResult);
        } else if (fileOperation == COPY_FILE_OP) {
            err += flashWrite((uint32_t *)internalFlashStartingAddress,
                              (uint32_t *)extFlashBlockBuff, EXT_FLASH_BLOCK_SIZE);
            internalFlashStartingAddress += EXT_FLASH_BLOCK_SIZE;
        }
        fileLen -= EXT_FLASH_BLOCK_SIZE;
        startingAddress += EXT_FLASH_BLOCK_SIZE;
    }
    /* Read remaining data that did not fill a block */
    if (fileLen) {
        Ext_Flash_Read(startingAddress, extFlashBlockBuff, fileLen, Ext_Flash_DataLine_Quad);
        if (fileOperation == CALC_CRC32_OP) {
            crc32(extFlashBlockBuff, fileLen, &crcResult);
        } else if (fileOperation == COPY_FILE_OP) {
            err += flashWrite((uint32_t *)internalFlashStartingAddress,
                              (uint32_t *)extFlashBlockBuff, fileLen);
        }
    }
    if (fileOperation == COPY_FILE_OP)
        return err;

    return crcResult;
}

int main(void)
{
    volatile int i;
    int err = 0x00000000;
    uint32_t startingAddress = 0x00000000;
    uint32_t crcResult = 0x00000000;
    int numLedsBlink;

    /* Limit the number of LED blinks */
    if (num_leds > 2) {
        numLedsBlink = 2;
    } else {
        numLedsBlink = num_leds;
    }

    /* Prevent bricks */
    if (numLedsBlink == 0) {
        DELAY(0x3FFFFF);
    }

    LED_Init();
    for (int led = 0; led < numLedsBlink; led++) {
        LED_On(led);
        DELAY(0x1FFFFF);
        LED_Off(led);
        DELAY(0x1FFFFF);
    }

    /* disable interrupts to prevent these operations from being interrupted */
    __disable_irq();

    /* init external flash */
    err += Ext_Flash_Init();
    err += Ext_Flash_Quad(1);

    if (err == 0) {
        /* Get header from ext flash */
        Ext_Flash_Read(startingAddress, (uint8_t *)&fileHeader, sizeof(fileHeader_t),
                       Ext_Flash_DataLine_Quad);

        /* Verify header integrity */
        if (fileHeader.fileLen != 0xFFFFFFFF && fileHeader.fileCRC != 0xFFFFFFFF) {
            crcResult = externFileOperation(CALC_CRC32_OP);
            /* Check the calculated digest against what was received */
            if (fileHeader.fileCRC != crcResult) {
                ledFailPattern();
            } else {
                /* Calculate how many pages the new image will occupy, +1 for remainder */
                uint32_t pagesToErase = (fileHeader.fileLen / MXC_FLASH_PAGE_SIZE) + 1;
                /* Erase the destination pages */
                if (multiPageErase((uint8_t *)FLASH0_START, pagesToErase) != E_NO_ERROR) {
                    /* Failed to erase pages */
                    bootError();
                }
                /* copy external file */
                err = externFileOperation(COPY_FILE_OP);
                if (err) {
                    bootError();
                }
                /* check what was written to flash */
                crcResult = 0;
                crc32((const void *)FLASH0_START, fileHeader.fileLen, &crcResult);
                if (crcResult != fileHeader.fileCRC) {
                    /* Bad firmware was written to internal flash */
                    while (1) {
                        ledFailPattern();
                    }
                }
                /* As long as first sector is erased so the bootloader does not try to reload its contents */
                Ext_Flash_Erase(0x00000000, Ext_Flash_Erase_64K);
                if (err == 0) {
                    ledSuccessPattern();
                }
            }
        }
    } else {
        /* external flash init error */
        ledFailPattern();
        bootError();
    }

    /* Boot from lower image */
    Boot_Lower();

    while (1) {}

    return 0;
}
