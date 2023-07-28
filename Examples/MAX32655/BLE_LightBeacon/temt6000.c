#include "temt6000.h"
#include <mxc.h>
#include "adc.h"

volatile unsigned int adc_done = 0;
static uint16_t adc_val;
uint8_t overflow;

void adc_complete_cb(void *req, int adcRead)
{
    overflow = (adcRead == E_OVERFLOW ? 1 : 0);
    adc_val = adcRead;
    adc_done = 1;
    return;
}

void ADC_IRQHandler(void)
{
    MXC_ADC_Handler();
}

void temt6000_Init(){
  if (MXC_ADC_Init() != E_NO_ERROR) {
    printf("Error ADC Init failed\n");

    while (1) {}
  }
  
  /* Set up LIMIT0 to monitor high and low trip points */
  MXC_ADC_SetMonitorChannel(MXC_ADC_MONITOR_0, MXC_ADC_CH_0);
  MXC_ADC_SetMonitorHighThreshold(MXC_ADC_MONITOR_0, 0x300);
  MXC_ADC_SetMonitorLowThreshold(MXC_ADC_MONITOR_0, 0x25);
  MXC_ADC_EnableMonitor(MXC_ADC_MONITOR_0);
  
  NVIC_EnableIRQ(ADC_IRQn);
  printf("Light Sensor Init \n");

}

uint16_t readSensorValue(){
    adc_val = MXC_ADC_StartConversion(MXC_ADC_CH_0);
    overflow = (adc_val == E_OVERFLOW ? 1 : 0);
    // adc_done = 0;
    // MXC_ADC_StartConversionAsync(MXC_ADC_CH_0, adc_complete_cb);
    // while (!adc_done) {
    //     printf("wait \n\r");
    // }
    // if (MXC_ADC_GetFlags() & (MXC_F_ADC_INTR_LO_LIMIT_IF | MXC_F_ADC_INTR_HI_LIMIT_IF)) {
    //   //printf(" %s Limit on AIN0 ", (MXC_ADC_GetFlags() & MXC_F_ADC_INTR_LO_LIMIT_IF) ? "Low" : "High");
    //   MXC_ADC_ClearFlags(MXC_F_ADC_INTR_LO_LIMIT_IF | MXC_F_ADC_INTR_HI_LIMIT_IF);
    // }
    return adc_val;
}