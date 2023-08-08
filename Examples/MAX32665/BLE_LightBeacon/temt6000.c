#include "temt6000.h"
//#include <mxc.h>
#include "adc.h"

volatile unsigned int adc_done = 0;
static uint16_t adc_val;
uint8_t overflow;

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