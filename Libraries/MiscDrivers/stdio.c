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

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION)
#include <errno.h>
#else
#include <sys/errno.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#include <unistd.h>
#include <sys/stat.h>
#endif /* ( __GNUC__ ) && ( __ARMCC_VERSION ) */

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#include <rt_misc.h>

#if (__ARMCC_VERSION < 6000000)
#pragma import(__use_no_semihosting_swi)
#endif /* __ARMCC_VERSION < 6000000 */

#if (__ARMCC_VERSION < 6000000)
struct __FILE {
    int handle;
};
#endif /* __ARMCC_VERSION < 6000000 */

FILE __stdout;
FILE __stdin;

/**
 * Integer to store the last character read from the FILE using fgetc().
 * Only valid if fgetc() was the last function called on the stream.
 */
int g_lastChar = 0;
/**
 * Global variable set to TRUE if fgetc() was previously called, false otherwise.
 * This variable is necessary for implementing __backspace(FILE *f) in the MDK microlib.
 */
int g_readChar = 0;

#endif /* __CC_ARM || __ARMCC_VERSION */

/* Defines - Compiler Specific */
#if defined(__ICCARM__)
#define STDIN_FILENO 0 // Defines that are not included in the DLIB.
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define EBADF -1
#endif /* __ICCARM__ */

#include "mxc_device.h"
#include "mxc_sys.h"
#include "board.h"
#include "uart.h"

#define MXC_UARTn MXC_UART_GET_UART(CONSOLE_UART)
#define UART_FIFO MXC_UART_GET_FIFO(CONSOLE_UART)
/* The following libc stub functions are required for a proper link with printf().
 * These can be tailored for a complete stdio implementation.
 * GNUC requires all functions below. IAR & KEIL only use read and write.
 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
int _open(const char *name, int flags, int mode)
{
    return -1;
}
int _close(int file)
{
    return -1;
}
int _isatty(int file)
{
    return -1;
}
int _lseek(int file, off_t offset, int whence)
{
    return -1;
}
int _fstat(int file, struct stat *st)
{
    return -1;
}
#endif /* __GNUC__ */

/* Handle IAR and ARM/Keil Compilers for _read/_write. Keil uses fputc and
   fgetc for stdio */

#if defined(__ICCARM__) || (defined(__GNUC__) && !defined(__ARMCC_VERSION))

#if defined(__GNUC__) && !defined(__ARMCC_VERSION) // GNUC _read function prototype
int _read(int file, char *ptr, int len)
{
    int n;
#elif defined(__ICCARM__) // IAR Compiler _read function prototype
int __read(int file, unsigned char *ptr, size_t len)
{
    size_t n;
#endif /*  */

    int num = 0; // count of number received.

    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {
            *ptr = MXC_UART_ReadCharacter(MXC_UARTn); // read a byte.
            MXC_UART_WriteCharacter(MXC_UARTn, *ptr); // echo the byte.
            if (*ptr == '\r') { // check for end of line.
                *ptr = '\n';
                num++;
                ptr++;

                break;
            } else {
                ptr++;
                num++;
            }
        }

        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}

/* newlib/libc printf() will eventually call write() to get the data to the stdout */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
// GNUC _write function prototype
int _write(int file, char *ptr, int len)
{
    int n;
#elif defined(__ICCARM__) // IAR Compiler _read function prototype
// IAR EW _write function prototype
int __write(int file, const unsigned char *ptr, size_t len)
{
    size_t n;
#endif /* __GNUC__ */

    switch (file) {
    case STDOUT_FILENO:
    case STDERR_FILENO:
        for (n = 0; n < len; n++) {
            if (*ptr == '\n') {
                MXC_UART_WriteCharacter(MXC_UARTn, '\r');
            }
            MXC_UART_WriteCharacter(MXC_UARTn, *ptr++);
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }

    return len;
}

#endif /* ( __ICCARM__ ) || (( __GNUC__ ) && !(__ARMCC_VERSION)) */

/* Handle Keil/ARM Compiler which uses fputc and fgetc for stdio */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
int fputc(int c, FILE *f)
{
    if (c != '\n') {
        MXC_UART_WriteCharacter(MXC_UARTn, c);
    } else {
        MXC_UART_WriteCharacter(MXC_UARTn, '\r');
        MXC_UART_WriteCharacter(MXC_UARTn, '\n');
    }

    return 0;
}

int __backspace(FILE *f)
{
    if (g_readChar) {
        return g_lastChar;
    } else {
        return EOF;
    }
}

int fgetc(FILE *f)
{
    g_lastChar = (int)MXC_UART_ReadCharacter(
        MXC_UARTn); /* Read the byte and save it to global for backspace */
    g_readChar = 1; /* set global to indicate g_lastChar is valid. */
    return g_lastChar;
}

int ferror(FILE *f)
{
    g_readChar = 0;
    return EOF;
}

void _ttywrch(int c)
{
    if (c != '\n') {
        MXC_UART_WriteCharacter(MXC_UARTn, c);
    } else {
        MXC_UART_WriteCharacter(MXC_UARTn, '\r');
        MXC_UART_WriteCharacter(MXC_UARTn, '\n');
    }
}

void _sys_exit(int return_code)
{
    while (1) {}
}

#endif /* __CC_ARM || __ARMCC_VERSION */
