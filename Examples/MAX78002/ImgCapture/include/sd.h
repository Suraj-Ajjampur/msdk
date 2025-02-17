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
* @file sd.h
* @brief This file exposes some higher-level SD card interface functions that
    wrap around the low-level operations provided by the fatFS file system 
    (http://elm-chan.org/fsw/ff/00index_e.html).
*****************************************************************************/

#ifndef EXAMPLES_MAX78002_IMGCAPTURE_INCLUDE_SD_H_
#define EXAMPLES_MAX78002_IMGCAPTURE_INCLUDE_SD_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __cplusplus
// This is only to get the linter happy...
#include <string>
#endif

#include "ff.h"

// Some global working variables and buffers are exposed below.  These hook up to
// the sd.c internals, and present convenient access points and buffers for more
// complex fatFS operations.  Most fatFS operations expect pointers to a global
// file system struct, file struct, directory struct, etc.
#define MAXLEN 256

extern FATFS *sd_fs; //FFat Filesystem Object
extern FIL sd_file; //FFat File Object
extern FRESULT sd_err; //FFat Result (Struct)
extern FILINFO sd_fno; //FFat File Information Object
extern DIR sd_dir; //FFat Directory Object
extern TCHAR sd_message[MAXLEN], sd_directory[MAXLEN], sd_cwd[MAXLEN], sd_filename[MAXLEN],
    sd_volume_label[24], sd_volume;
extern DWORD sd_clusters_free, sd_sectors_free, sd_sectors_total, sd_volume_sn;
extern UINT sd_bytes_written, sd_bytes_read, sd_mounted;
extern BYTE sd_work[4096];
extern TCHAR *FR_ERRORS[20];

/**
* @brief Mount the SD card.  If the SD card is blank (no volume name), format the card
* with FAT32 and give it the name "MAXIM-SD"
* @return FR_OK if successful, FR_xxx error code if unsucessful.
* @details
****************************************************************************/
FRESULT sd_mount();

/**
* @brief Unmount the SD card.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_unmount();

/**
* @brief Get the size and free space available on the SD card.  Sets them to the 
global "sd_sectors_total" and "sd_sectors_free" variables, respectively.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_get_size();

/**
* @brief Get the current working directory and saves it to the "sd_cwd" global variable.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_get_cwd();

/**
* @brief Change directory.
* @param[in] dir Target directory.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_cd(const char *dir);

/**
* @brief List the contents of the current directory with printf.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_ls();

/**
* @brief Make a directory.  Similar to "mkdir" on linux.
* @param[in] dir Directory path.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_mkdir(const char *dir);

/**
* @brief Remove a file or empty directory.  Similar to "rm" on linux.
* @param[in] item Item to remove.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_rm(const char *item);

/**
* @brief Create an empty file.  Similar to the "touch" command on linux.
* @param[in] filepath Target file path (must not already exist).
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_touch(const char *filepath);

/**
* @brief Write a string to a file.
* @param[in] filepath Target file path (must already exist).
* @param[in] string String to write to the file.  Must be null terminated '\0'
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_write_string(const char *filepath, const char *string);

/**
* @brief Write bytes to a file.
* @param[in] filepath Target file path (must already exist).
* @param[in] data Bytes to write to the file.
* @param[in] len Number of bytes to write.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_write(const char *filepath, const uint8_t *data, int len);
/**
* @brief Print the contents of a file.  Similar to the "cat" command on linux.
* @param[in] filename Directory path.
* @return FR_OK if successful, FR_xxx error code if unsucessful.
****************************************************************************/
FRESULT sd_cat(const char *filename);

// Supporting function for use with f_forward (http://elm-chan.org/fsw/ff/doc/forward.html)
// The actual implementation of this function is in console.c, which
// streams to the serial console.
UINT out_stream(const BYTE *p, UINT btf);

#endif // EXAMPLES_MAX78002_IMGCAPTURE_INCLUDE_SD_H_
