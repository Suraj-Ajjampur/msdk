
#ifndef EXAMPLES_MAX32665_BLE_TEMPBEACON_TIMER_H
#define EXAMPLES_MAX32665_BLE_TEMPBEACON_TIMER_H


#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_sys.h"
#include "nvic_table.h"
#include "gpio.h"
#include "board.h"
#include "tmr.h"
#include "led.h"
#include "pb.h"
#include "lp.h"
//#include "lpgcr_regs.h"
#include "gcr_regs.h"
#include "pwrseq_regs.h"
#include "temp_sensor.h"
#include "dats_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define INTERVAL_TIME_CONT 1 // (s) will toggle after every interval
#define CONT_TIMER MXC_TMR1 // Can be MXC_TMR0 through MXC_TMR5
#define CONT_TIMER_IRQn TMR1_IRQn

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
void ContinuousTimerHandler(void);

void ContinuousTimer(void);

#endif //EXAMPLES_MAX32665_BLE_TEMPBEACON_TIMER_H
