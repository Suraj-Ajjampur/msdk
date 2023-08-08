/**************************************************************************************************
  Include Files
**************************************************************************************************/
#include "timer.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
volatile uint32_t sensorValue;

/**************************************************************************************************
  Function Definitions
**************************************************************************************************/
void ContinuousTimerHandler(void)
{
    /* Clear interrupt */
    MXC_TMR_ClearFlags(CONT_TIMER);
    MXC_GPIO_OutToggle(led_pin[0].port, led_pin[0].mask);
    sensorValue = readSensorValue();
    updateLightValue(sensorValue);
}

void ContinuousTimerInit(void)
{   
    /* Configure Timer */
    MXC_NVIC_SetVector(CONT_TIMER_IRQn, ContinuousTimerHandler);
    NVIC_EnableIRQ(CONT_TIMER_IRQn);
    NVIC_SetPriority(CONT_TIMER_IRQn, TIMER_PRIORITY); // Set Low Priority

    /* Declare variables */
    mxc_tmr_cfg_t tmr;
    uint32_t periodTicks = PeripheralClock / CLOCK_DIVISION_SCALE * INTERVAL_TIME_CONT;
    
    MXC_TMR_Shutdown(CONT_TIMER);

    tmr.pres = TMR_PRES_4;
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.cmp_cnt = periodTicks; // SystemCoreClock*(1/interval_time);
    tmr.pol = 0;

    MXC_TMR_Init(CONT_TIMER, &tmr);

    MXC_TMR_Start(CONT_TIMER);

    printf("Continuous timer started.\n\n");
}