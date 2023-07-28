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

#define PWM_CLOCK_SOURCE MXC_TMR_APB_CLK // \ref mxc_tmr_clock_t
#define FREQ 1000 // (Hz)
#define DUTY_CYCLE 50 // (%)
#define CONT_FREQ 2 // (Hz)
#define CONT_TIMER MXC_TMR1
#define CONT_CLOCK_SOURCE MXC_TMR_8M_CLK

void ContinuousTimerHandler(void);

void ContinuousTimer(void);