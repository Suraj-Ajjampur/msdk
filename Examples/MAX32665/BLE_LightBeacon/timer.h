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
#include "gcr_regs.h"
#include "pwrseq_regs.h"
#include "temt6000.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define INTERVAL_TIME_CONT 1 // (s) will toggle after every interval
#define CONT_TIMER MXC_TMR1 // Can be MXC_TMR0 through MXC_TMR5
#define CONT_TIMER_IRQn TMR1_IRQn
#define TIMER_PRIORITY 1 
#define CLOCK_DIVISION_SCALE 4

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief  Continuous Timer Handler to read the Light sensor value
 *
 *  \return None.
 */
/*************************************************************************************************/
void ContinuousTimerHandler(void);

/*************************************************************************************************/
/*!
 *  \brief  Function to initialize Continuous Timer
 *
 *  \return None.
 */
/*************************************************************************************************/
void ContinuousTimerInit(void);

#endif // __TIMER_MODULE_FOR_LIGHT_SENSOR__