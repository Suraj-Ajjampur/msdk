#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

/**************************************************************************************************
  Header Files
**************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_buf.h"
#include "wsf_nvm.h"
#include "wsf_timer.h"
#include "hci_api.h"
#include "sec_api.h"
#include "dm_api.h"
#include "smp_api.h"
#include "att_api.h"
#include "app_api.h"
#include "app_main.h"
#include "app_db.h"
#include "app_ui.h"
#include "svc_ch.h"
#include "svc_core.h"
#include "svc_wp.h"
#include "util/calc128.h"
#include "dats_api.h"
#include "wut.h"
#include "trimsir_regs.h"
#include "pal_btn.h"
#include "pal_uart.h"
#include "tmr.h"
#include "svc_sds.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define LIGHT_SENSOR_PRIORITY 0

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief  Function to initialize TEMT6000 Light Sensor
 *
 *  \return None.
 */
/*************************************************************************************************/
void temt6000_Init();

/*************************************************************************************************/
/*!
 *  \brief  Function to initiate ADC conversion get the sensor value
 *
 *  \return 16 bit Digital Value
 */
/*************************************************************************************************/
uint16_t readSensorValue();

#endif // LIGHT_SENSOR_H