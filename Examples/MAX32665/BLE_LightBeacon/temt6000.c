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

/**************************************************************************************************
  Header Files
**************************************************************************************************/
#include "temt6000.h"
#include "adc.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
volatile unsigned int adc_done = 0;
static uint16_t adc_val;

/**************************************************************************************************
  Function Definitions
**************************************************************************************************/
void adc_complete_cb(void *req, int error)
{
    adc_done = 1;
    return;
}
void ADC_IRQHandler(void)
{
    MXC_ADC_Handler();
}

void temt6000_Init(){
  if (MXC_ADC_Init() != E_NO_ERROR) {
    printf("Error Bad ADC Parameter\n");
    while (1) {}
  }

  /* Configure Inputs as ADC inputs */
  MXC_GPIO_Config(&gpio_cfg_adc2);
  
  MXC_ADC_SetMonitorChannel(MXC_ADC_MONITOR_2, MXC_ADC_CH_2);
  MXC_ADC_EnableMonitor(MXC_ADC_MONITOR_2);

  NVIC_EnableIRQ(ADC_IRQn);
  NVIC_SetPriority(ADC_IRQn, LIGHT_SENSOR_PRIORITY);  // Set High Priority
  printf("Light Sensor Init \n");

}

uint16_t readSensorValue(){
    adc_done = 0;
    MXC_ADC_StartConversionAsync(MXC_ADC_CH_2, adc_complete_cb);
    while (!adc_done) {}
    MXC_ADC_GetData(&adc_val);

    return adc_val;
}