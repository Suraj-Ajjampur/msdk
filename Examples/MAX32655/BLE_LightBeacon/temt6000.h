#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

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
#include "gatt/gatt_api.h"
#include "dats_api.h"
#include "wut.h"
#include "trimsir_regs.h"
#include "pal_btn.h"
#include "pal_uart.h"
#include "tmr.h"
#include "svc_sds.h"
void temt6000_Init();

uint16_t readSensorValue();

#endif // LIGHT_SENSOR_H