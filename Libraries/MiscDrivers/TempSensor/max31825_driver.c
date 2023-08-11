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


#define NORMAL_DATA_FORMAT 11 
#define EXTENDED_DATA_FORMAT 12 

#define DATA_FORMAT NORMAL_DATA_FORMAT

#define SIGNED_BIT_MASK  (31 << DATA_FORMAT)


#define INTERRUPT_MODE          1
#define COMPARATOR_MODE         0
// Local global variables
uint8_t utilcrc8;
float temp_in_c = 0;
scratchpad_t control_regs;

/** Lookup table that is accessed directly for any 8-bit value currently
  * stored in the CRC register and any 8-bit pattern of new data
*/
static uint8_t dscrc_table[] = {
    0,   94,  188, 226, 97,  63,  221, 131, 194, 156, 126, 32,  163, 253, 31,  65,  157, 195, 33,
    127, 252, 162, 64,  30,  95,  1,   227, 189, 62,  96,  130, 220, 35,  125, 159, 193, 66,  28,
    254, 160, 225, 191, 93,  3,   128, 222, 60,  98,  190, 224, 2,   92,  223, 129, 99,  61,  124,
    34,  192, 158, 29,  67,  161, 255, 70,  24,  250, 164, 39,  121, 155, 197, 132, 218, 56,  102,
    229, 187, 89,  7,   219, 133, 103, 57,  186, 228, 6,   88,  25,  71,  165, 251, 120, 38,  196,
    154, 101, 59,  217, 135, 4,   90,  184, 230, 167, 249, 27,  69,  198, 152, 122, 36,  248, 166,
    68,  26,  153, 199, 37,  123, 58,  100, 134, 216, 91,  5,   231, 185, 140, 210, 48,  110, 237,
    179, 81,  15,  78,  16,  242, 172, 47,  113, 147, 205, 17,  79,  173, 243, 112, 46,  204, 146,
    211, 141, 111, 49,  178, 236, 14,  80,  175, 241, 19,  77,  206, 144, 114, 44,  109, 51,  209,
    143, 12,  82,  176, 238, 50,  108, 142, 208, 83,  13,  239, 177, 240, 174, 76,  18,  145, 207,
    45,  115, 202, 148, 118, 40,  171, 245, 23,  73,  8,   86,  180, 234, 105, 55,  213, 139, 87,
    9,   235, 181, 54,  104, 138, 212, 149, 203, 41,  119, 244, 170, 72,  22,  233, 183, 85,  11,
    136, 214, 52,  106, 43,  117, 151, 201, 74,  20,  246, 168, 116, 42,  200, 150, 21,  75,  169,
    247, 182, 232, 10,  84,  215, 137, 107, 53
};

/** Reset crc8 to the value passed in
 * 
 * @param reset data to set crc8 to
*/
void setcrc8(uint8_t reset)
{
    utilcrc8 = reset;
    return;
}

/** Update the Dallas Semiconductor One Wire CRC (utilcrc8) from the global variable utilcrc8 and the argument.
 * 
 * @param int data byte to calculate the 8-bit crc from
 * 
 * @return updated utilcrc8
*/
uint8_t docrc8(uint8_t x)
{
    utilcrc8 = dscrc_table[utilcrc8 ^ x];
    return utilcrc8;
}

#if (DATA_FORMAT == NORMAL_DATA_FORMAT)
/** Normal_data format produces temperature data upto 128C - 1 LSB
 * 
*/
static float temp_data_format[] = {
    0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32
};

#else

/** Extended_data format produces temperature data up to and beyond 145C operating limit
*/
static float temp_data_format[] = {
    0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32, 64 
};

#endif

/*

Most common CRC errors in 1-wire devices.
1. Any odd number of errors anywhere within the 64-bit number.
2. All double-bit errors anywhere within the 64-bit number.
3. Any cluster of errors that can be contained within an 8-bit "window" (1-8 bits incorrect).
4. Most larger clusters of errors.
*/


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

/** Gets the value of the raw temperature value in celcius
 * 
 * @param num 
*/
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
uint16_t curr_temp = 0;

int ReadScratchPad(void){
    uint8_t buffer[9];

    int i;
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
    curr_temp = (buffer[0] + (buffer[1] << 8));

    //Get_Temp(curr_temp);
    

    //  Check CRC8 of received Temperature 
     setcrc8(0);

     for (i = 0; i < 8; i++) {
        setcrc8(docrc8(buffer[i]));
     }

     if (utilcrc8 != buffer[i]) {
         printf("CRC Error");
         return -7;
     }


    return 0;

}

// int Write_Scratchpad(uint8_t *data_buffer,uint8_t config,int high_threshhold,int low_threshold);

int Write_Scratchpad(uint8_t *data_buffer){

    uint8_t cmd_buffer[1];

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
uint16_t get_max31825_temp(void){
    return curr_temp;
}

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

/** Selects the rate for automatic continuous conversions, Apply 
 *  only to external power used. 
 * 
 *  @param rate conversion rate required
*/
void Set_conversion_rate(uint8_t rate){

    //Clear the registers
    control_regs.config &= ~(MXC_MAX31825_CONFIG_REGISTER_CONV_RATE_MASK);

    //Set the conversion rate register
    control_regs.config |= rate;
}


/** Load the 
 * 
*/


/** Sets the High Threshold to trigger an alarm
 * 
 * @param high_temp Temperature to set 
*/
void Set_High_Alarm_Threshold(float high_temp) {
    
    //Reset the value of the register
    control_regs.TH = 0;

    //Check for negative value
    if (high_temp < 0){
        //Set the signed bits
        control_regs.TL |= (SIGNED_BIT_MASK);
        //Change the sign of the temp value
        high_temp = -high_temp;
    }
    
    for (int i = 0; i < NORMAL_DATA_FORMAT; i++) {
        if (high_temp >= temp_data_format[i]) {
            high_temp -= temp_data_format[i];
            control_regs.TH |= (1 << i);
        }
    }
}

/** Sets the Low Threshold to trigger an alarm in the control register structure 
 * 
 * @param low_temp Temperature to set 
*/
void Set_Low_Alarm_Threshold(float low_temp) {
    
    //Reset the value of the register
    control_regs.TL = 0;

    //Check for negative value
    if (low_temp < 0){
        //Set the signed bits
        control_regs.TL |= (SIGNED_BIT_MASK);
        //Change the sign of the temp value
        low_temp = -low_temp;
    }
    
    for (int i = 0; i < NORMAL_DATA_FORMAT; i++) {
        if (low_temp >= temp_data_format[i]) {
            low_temp -= temp_data_format[i];
            control_regs.TL |= (1 << i);
        }
    }
}

/** The configuration register provides control over several operating parameters, including data format, 
 * conversion resolution, the ALARM output mode, and the continuous conversion rate.
 * 
*/
void MXC_Config_MAX31825(uint8_t resolution_bits, uint8_t interrupt,uint8_t conv_rate){

//Logic for the format of operations
#if (DATA_FORMAT == NORMAL_DATA_FORMAT)

control_regs.config |= (MXC_MAX31825_CONFIG_REGISTER_FORMAT);
#else
control_regs.config &= ~(MXC_MAX31825_CONFIG_REGISTER_FORMAT);
#endif

    //Set_resolution_bits(resolution_bits);



    Set_conversion_rate(conv_rate);

}

// void set_comp_int(uint8_t){
//     //Clear interrupt bits
//     control_regs.config &= ~(MXC_MAX31825_CONFIG_REGISTER_COMPT_INT);

//     if (interrupt == 1){
//         control_regs.config |= MXC_MAX31825_CONFIG_REGISTER_COMPT_INT;
//     }
// }
/** The resolution bits (D6:D5) select the conversion resolution. The conversion time doubles with every bit of increased resolution.
 * 
*/
// Set_resolution_bits(uint8_t resolution_bits){
//     control_regs.config &= ~(MXC_MAX31825_CONFIG_REGISTER_RESOLUTION_MASK);

//     control_regs.config |= (MXC_MAX31825_RESOLUTION_12_BITS);
// }


int OW_MAX31825_Test1(void){
    int32_t err;

    /* Set 1-Wire to standard speed */
    MXC_OWM_SetOverdrive(0);

    //Startup requence
    err = OWM_Connect();
    if ( err != 0){
        return err;
    }


    // Set_High_Alarm_Threshold(28.0);

    // Set_Low_Alarm_Threshold(20.0);

    //MXC_Config_MAX31825(MXC_MAX31825_RESOLUTION_12_BITS,INTERRUPT_MODE);
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