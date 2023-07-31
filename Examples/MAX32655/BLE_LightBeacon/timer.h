#ifndef __TIMER_MODULE_FOR_LIGHT_SENSOR__
#define __TIMER_MODULE_FOR_LIGHT_SENSOR__

/**************************************************************************************************
  Header Files
**************************************************************************************************/
#include "mxc_device.h"
#include "mxc_sys.h"
#include "nvic_table.h"
#include "gpio.h"
#include "board.h"
#include "tmr.h"
#include "led.h"
#include "pb.h"
#include "lp.h"
#include "lpgcr_regs.h"
#include "gcr_regs.h"
#include "pwrseq_regs.h"
#include "temt6000.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define CONT_FREQ         1
#define CONT_TIMER        MXC_TMR1
#define CONT_CLOCK_SOURCE MXC_TMR_8M_CLK

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
void ContinuousTimerHandler(void);
void ContinuousTimer(void);

#endif // __TIMER_MODULE_FOR_LIGHT_SENSOR__