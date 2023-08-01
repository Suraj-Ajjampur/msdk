
#include "timer.h"

void ContinuousTimerHandler(void)
{
    static double sensorValue;
    // Clear interrupt
    MXC_TMR_ClearFlags(CONT_TIMER);
    MXC_GPIO_OutToggle(led_pin[0].port, led_pin[0].mask);
    sensorValue = ReadTempVal();
    printf("Temp Val %f \n\r", sensorValue);
    updateTempValue(sensorValue);
}

void ContinuousTimer()
{
    // Declare variables
    mxc_tmr_cfg_t tmr;
    uint32_t periodTicks = PeripheralClock / 4 * INTERVAL_TIME_CONT; //Change to 1s

    //printf("periodTicks is %u",periodTicks);

    MXC_TMR_Shutdown(CONT_TIMER);

    tmr.pres = TMR_PRES_4;
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.cmp_cnt = periodTicks; //SystemCoreClock*(1/interval_time);
    tmr.pol = 0;

    MXC_TMR_Init(CONT_TIMER, &tmr);

    MXC_TMR_Start(CONT_TIMER);

    printf("Continuous timer started.\n\n");
}