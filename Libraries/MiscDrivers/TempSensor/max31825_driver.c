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

#include "max31825_driver.h"

static float temp_data_format[] = {
    0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64 //Add signed bit values further here.
};

int Convert_T(void){

    uint8_t buffer[8];

    /* Error if presence pulse not detected. */
    if (MXC_OWM_Reset() == 0) {
        return -2;
    }

    /* Send a skip rom command  */
    buffer[0] = SKIP_ROM;
    MXC_OWM_Write(buffer, 1);

    /* Send a convert T command  */
    buffer[0] = COMMAND_T;
    MXC_OWM_Write(buffer, 1);
    //printf("Convert T Command\n\r");

    return 0;
}

int OWM_Connect(void){

    uint8_t buffer[9];

    /* Error if presence pulse not detected. */
    if (MXC_OWM_Reset() == 0) {
        return -2;
    }

    /* Send a skip rom command  */
    buffer[0] = SKIP_ROM;
    MXC_OWM_Write(buffer, 1);
    printf("Skip ROM command\n\r");

    /* Send a skip rom command  */
    buffer[0] = READ_SCRATCHPAD;
    MXC_OWM_Write(buffer, 1);
    printf("Skip ROM command\n\r");

    /* Read the Scratchpad */
    memset(buffer, 0, sizeof(buffer));
    if (MXC_OWM_Read(buffer, 9) < 0) {
        return -5;
    }
    //Add some stuff on CRC here.
    /*
    Read one byte, perform CRC on this byte based on the previous 8 bytes of values read.
    */
    return 0;
}

void print_binary(uint16_t num) {
    int num_bits = sizeof(uint16_t) * 8;
    for (int i = num_bits - 1; i >= 0; i--) {
        int set_bit = (num >> i) & 1;
        printf(" %d",set_bit);
    }
    printf("\n");
}


float temp_in_c = 0;
void Get_Temp(uint16_t num) {
    //Reset value of temp in c as 0
    temp_in_c = 0;
    int num_bits = sizeof(num) * 8;
    
    for (int i = 0; i < num_bits; i++) {
        if ((num >> i) & 1) {
            //printf("Bit at position %d is set\n", i);
            temp_in_c = temp_in_c + temp_data_format[i];
        }
    }
}

int ReadScratchPad(void){
    uint8_t buffer[9];

    //int i;
    //uint8_t crc8;

    /* Error if presence pulse not detected. */
    if (MXC_OWM_Reset() == 0) {
        return -2;
    }

    /* Send a skip rom command  */
    buffer[0] = SKIP_ROM;
    MXC_OWM_Write(buffer, 1);

    buffer[0] = READ_SCRATCHPAD;
    MXC_OWM_Write(buffer, 1);
    //printf("Read Scratchpad command\n\r");

    /* Read the Scratchpad */
    memset(buffer, 0, sizeof(buffer));
    if (MXC_OWM_Read(buffer, 9) < 0) {
        return -5;
    }

    //Bit formatting 
    uint16_t curr_temp = (buffer[0] + (buffer[1] << 8));

    Get_Temp(curr_temp);
    
/*
    //  Check CRC8 of received Temperature 
    // setcrc8(0);
    // for (i = 0; i < 8; i++) {
    //     crc8 = docrc8(buffer[i]);
    // }

    // if (crc8 != 0x00) {
    //     printf("CRC Error");
    //     return -7;
    // }
*/


    return 0;

}

int Write_Scratchpad(uint8_t *data_buffer){

    uint8_t cmd_buffer[1];
    //uint8_t data_buffer[5]; //6 in all cases

    /* Error if presence pulse not detected. */
    if (MXC_OWM_Reset() == 0) {
        return -2;
    }

    /* Send a skip rom command  */
    cmd_buffer[0] = SKIP_ROM;
    MXC_OWM_Write(cmd_buffer, 1);

    /* Send a convert T command  */
    cmd_buffer[0] = WRITE_SCRATCHPAD;
    MXC_OWM_Write(cmd_buffer, 1);

    if (MXC_OWM_Write(data_buffer, 5) < 0) {
        return -88;
    }

    return 0;
    
}

int32_t OW_MAX31825_Test(void){

    int32_t err;
    /* Set 1-Wire to standard speed */
    MXC_OWM_SetOverdrive(0);

    //Startup requence
    err = ReadScratchPad();
    if ( err != 0){
        return err;
    }

    MXC_Delay(20000);

    err = Convert_T();
    if ( err != 0){
        return err;
    }
    
    MXC_Delay(175000);

    //Read the temp value in C
    err = ReadScratchPad();
    if (err!= 0){
        return err;
    }

    return 0;
}



//getter function for max31825 temp
double get_max31825_temp(void){
    return (double)temp_in_c;
}

//Max31825 init

//Read temp - Sequence of commands send to continously read the temperature.

int32_t Read_Max31825_temp(void){

    int32_t err;

    //Read the temp value in C
    err = ReadScratchPad();
    if (err!= 0){
        return err;
    }

    return 0;
}


int DetectAddress(void){
    uint8_t Status_Register[8];

    /* Error if presence pulse not detected. */
    if (MXC_OWM_Reset() == 0) {
        return -2;
    }

    /* Send a skip rom command  */
    Status_Register[0] = SKIP_ROM;
    MXC_OWM_Write(Status_Register, 1);

    Status_Register[0] = DETECT_ADDRESS;
    MXC_OWM_Write(Status_Register, 1);

    /* Read the Scratchpad */
    memset(Status_Register, 0, sizeof(Status_Register));
    if (MXC_OWM_Read(Status_Register, 8) < 0) {
        return -33;
    }

    return 0;
}

int OW_MAX31825_Test1(void){
    int32_t err;

    /* Set 1-Wire to standard speed */
    MXC_OWM_SetOverdrive(0);

    //Startup requence
    err = OWM_Connect();
    if ( err != 0){
        return err;
    }

    uint8_t buf[] = {0x75,0x07,0xFF,0x90,0xFC}; //Writing into config bits
    err = Write_Scratchpad(buf);
    if ( err != 0){
        return err;
    }

    err = ReadScratchPad();
    if ( err != 0){
        return err;
    }

    return 0;
}