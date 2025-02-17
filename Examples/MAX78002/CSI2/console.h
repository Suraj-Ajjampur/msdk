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
* @file console.h
* @brief Serial console header file
*****************************************************************************/

#ifndef EXAMPLES_MAX78002_CSI2_CONSOLE_H_
#define EXAMPLES_MAX78002_CSI2_CONSOLE_H_
#include "uart.h"
#include "board.h"

#define SERIAL_BUFFER_SIZE 256
#define CON_BAUD 921600 // UART baudrate used for sending data to PC

typedef enum {
    CMD_NOT_READY = -2,
    CMD_UNKNOWN = -1,
    CMD_HELP = 0,
    CMD_RESET,
    CMD_CAPTURE,
    CMD_SETREG,
    CMD_GETREG
} cmd_t;

extern char *cmd_table[];
extern char *help_table[];

static mxc_uart_regs_t *Con_Uart = MXC_UART_GET_UART(CONSOLE_UART);
extern char g_serial_buffer[SERIAL_BUFFER_SIZE];
extern int g_buffer_index;
extern int g_num_commands;

extern void service_console(cmd_t cmd);

int MXC_UART_WriteBytes(mxc_uart_regs_t *uart, const uint8_t *bytes, int len);

int console_init();
int send_msg(const char *msg);
int recv_msg(char *buffer);
int recv_cmd(cmd_t *out_cmd);
void clear_serial_buffer(void);
void print_help(void);

#endif // EXAMPLES_MAX78002_CSI2_CONSOLE_H_
