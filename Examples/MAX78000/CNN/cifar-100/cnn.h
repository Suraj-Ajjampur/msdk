/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/*
 * This header file was automatically generated for the cifar-100 network from a template.
 * Please do not edit; instead, edit the template and regenerate.
 */

#ifndef __CNN_H__
#define __CNN_H__

#include <stdint.h>
typedef int32_t q31_t;
typedef int16_t q15_t;

/* Return codes */
#define CNN_FAIL 0
#define CNN_OK 1

/*
  SUMMARY OF OPS
  Hardware: 36,527,616 ops (36,227,072 macc; 300,544 comp; 0 add; 0 mul; 0 bitwise)
    Layer 0: 1,835,008 ops (1,769,472 macc; 65,536 comp; 0 add; 0 mul; 0 bitwise)
    Layer 1: 2,129,920 ops (2,097,152 macc; 32,768 comp; 0 add; 0 mul; 0 bitwise)
    Layer 2: 18,939,904 ops (18,874,368 macc; 65,536 comp; 0 add; 0 mul; 0 bitwise)
    Layer 3: 4,792,320 ops (4,718,592 macc; 73,728 comp; 0 add; 0 mul; 0 bitwise)
    Layer 4: 540,672 ops (524,288 macc; 16,384 comp; 0 add; 0 mul; 0 bitwise)
    Layer 5: 4,743,168 ops (4,718,592 macc; 24,576 comp; 0 add; 0 mul; 0 bitwise)
    Layer 6: 1,056,768 ops (1,048,576 macc; 8,192 comp; 0 add; 0 mul; 0 bitwise)
    Layer 7: 1,188,864 ops (1,179,648 macc; 9,216 comp; 0 add; 0 mul; 0 bitwise)
    Layer 8: 1,181,696 ops (1,179,648 macc; 2,048 comp; 0 add; 0 mul; 0 bitwise)
    Layer 9: 68,096 ops (65,536 macc; 2,560 comp; 0 add; 0 mul; 0 bitwise)
    Layer 10: 51,200 ops (51,200 macc; 0 comp; 0 add; 0 mul; 0 bitwise)

  RESOURCE USAGE
  Weight memory: 347,840 bytes out of 442,368 bytes total (79%)
  Bias memory:   932 bytes out of 2,048 bytes total (46%)
*/

/* Number of outputs for this network */
#define CNN_NUM_OUTPUTS 100

/* Use this timer to time the inference */
#define CNN_INFERENCE_TIMER MXC_TMR0

/* Port pin actions used to signal that processing is active */

#define CNN_START LED_On(1)
#define CNN_COMPLETE LED_Off(1)
#define SYS_START LED_On(0)
#define SYS_COMPLETE LED_Off(0)

/* Run software SoftMax on unloaded data */
void softmax_q17p14_q15(const q31_t *vec_in, const uint16_t dim_vec, q15_t *p_out);
/* Shift the input, then calculate SoftMax */
void softmax_shift_q17p14_q15(q31_t *vec_in, const uint16_t dim_vec, uint8_t in_shift,
                              q15_t *p_out);

/* Stopwatch - holds the runtime when accelerator finishes */
extern volatile uint32_t cnn_time;

/* Custom memcopy routines used for weights and data */
void memcpy32(uint32_t *dst, const uint32_t *src, int n);
void memcpy32_const(uint32_t *dst, int n);

/* Enable clocks and power to accelerator, enable interrupt */
int cnn_enable(uint32_t clock_source, uint32_t clock_divider);

/* Disable clocks and power to accelerator */
int cnn_disable(void);

/* Perform minimum accelerator initialization so it can be configured */
int cnn_init(void);

/* Configure accelerator for the given network */
int cnn_configure(void);

/* Load accelerator weights */
int cnn_load_weights(void);

/* Verify accelerator weights (debug only) */
int cnn_verify_weights(void);

/* Load accelerator bias values (if needed) */
int cnn_load_bias(void);

/* Start accelerator processing */
int cnn_start(void);

/* Force stop accelerator */
int cnn_stop(void);

/* Continue accelerator after stop */
int cnn_continue(void);

/* Unload results from accelerator */
int cnn_unload(uint32_t *out_buf);

/* Turn on the boost circuit */
int cnn_boost_enable(mxc_gpio_regs_t *port, uint32_t pin);

/* Turn off the boost circuit */
int cnn_boost_disable(mxc_gpio_regs_t *port, uint32_t pin);

#endif // __CNN_H__
