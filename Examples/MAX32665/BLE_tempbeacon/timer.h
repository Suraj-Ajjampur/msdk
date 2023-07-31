
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
#include "temp_sensor.h"
#include "dats_api.h"


/***** Definitions *****/

// Parameters for PWM output
#define PORT_PWM MXC_GPIO0 //port
#define PIN_PWM MXC_GPIO_PIN_16 //pin
#define FREQ 200 // (Hz)
#define DUTY_CYCLE 75 // (%)
#define PWM_TIMER MXC_TMR4 // must change PWM_PORT and PWM_PIN if changed

// Parameters for Continuous timer
#define INTERVAL_TIME_CONT 1 // (s) will toggle after every interval
#define CONT_TIMER MXC_TMR0 // Can be MXC_TMR0 through MXC_TMR5
#define CONT_TIMER_IRQn TMR0_IRQn

// Parameters for One-shot timer
#define INTERVAL_TIME_OST 3 // (s)
#define OST_TIMER MXC_TMR1 // Can be MXC_TMR0 through MXC_TMR5
#define OST_TIMER_IRQn TMR1_IRQn

void ContinuousTimerHandler(void);

void ContinuousTimer(void);

#endif //EXAMPLES_MAX32665_BLE_TEMPBEACON_TIMER_H