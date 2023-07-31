
#include "timer.h"

volatile uint32_t sensorValue;

void ContinuousTimerHandler(void)
{
    // Clear interrupt
    MXC_TMR_ClearFlags(CONT_TIMER);
    MXC_GPIO_OutToggle(led_pin[0].port, led_pin[0].mask);
    sensorValue = readSensorValue();
    updateLightValue(sensorValue);
}

void ContinuousTimer(void)
{
    // Declare variables
    mxc_tmr_cfg_t tmr;
    uint32_t periodTicks = MXC_TMR_GetPeriod(CONT_TIMER, CONT_CLOCK_SOURCE, 128, CONT_FREQ);

    MXC_TMR_Shutdown(CONT_TIMER);

    tmr.pres = TMR_PRES_128;
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.bitMode = TMR_BIT_MODE_16B;
    tmr.clock = CONT_CLOCK_SOURCE;
    tmr.cmp_cnt = periodTicks;
    tmr.pol = 0;

    if (MXC_TMR_Init(CONT_TIMER, &tmr, true) != E_NO_ERROR) {
        printf("Failed Continuous timer Initialization.\n");
        return;
    }
}