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
/**
* @file cnn_memutils.h
* @brief Utility functions for accessing CNN data SRAM as a virtually contiguous block
        of memory.
*****************************************************************************/

#ifndef EXAMPLES_MAX78002_IMGCAPTURE_INCLUDE_CNN_MEMUTILS_H_
#define EXAMPLES_MAX78002_IMGCAPTURE_INCLUDE_CNN_MEMUTILS_H_
#include <stdint.h>
#include "mxc.h"
#include "gcfr_regs.h"
#include "max78002.h"

#define CNN_QUAD0_DSRAM_START 0x51800000
#define CNN_QUAD0_DSRAM_END 0x51873FFF
#define CNN_QUAD1_DSRAM_START 0x52800000
#define CNN_QUAD1_DSRAM_END 0x52873FFF
#define CNN_QUAD2_DSRAM_START 0x53800000
#define CNN_QUAD2_DSRAM_END 0x53873FFF
#define CNN_QUAD3_DSRAM_START 0x54800000
#define CNN_QUAD3_DSRAM_END 0x54873FFF

// Enables all 4 CNN quadrants and their memories.
int cnn_enable(uint32_t clock_source, uint32_t clock_divider)
{
    // Reset all domains, restore power to CNN
    MXC_GCFR->reg3 = 0xf; // Reset
    MXC_GCFR->reg1 = 0xf; // Mask memory
    MXC_GCFR->reg0 = 0xf; // Power
    MXC_Delay(MSEC(10)); // Wait for load switches
    MXC_GCFR->reg2 = 0x0; // Iso
    MXC_GCFR->reg3 = 0x0; // Reset

    if (clock_source == MXC_S_GCR_PCLKDIV_CNNCLKSEL_IPLL)
        while ((MXC_GCR->ipll_ctrl & MXC_F_GCR_IPLL_CTRL_RDY) != MXC_F_GCR_IPLL_CTRL_RDY) {}
    // Wait for PLL

    MXC_GCR->pclkdiv =
        (MXC_GCR->pclkdiv & ~(MXC_F_GCR_PCLKDIV_CNNCLKDIV | MXC_F_GCR_PCLKDIV_CNNCLKSEL)) |
        clock_divider | clock_source;
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CNN); // Enable CNN clock

    return 1;
}

int cnn_disable(void)
{
    // Disable CNN clock
    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_CNN);

    // Disable power to CNN
    MXC_GCFR->reg3 = 0xf; // Reset
    MXC_GCFR->reg2 = 0xf; // Iso
    MXC_GCFR->reg0 = 0x0; // Power
    MXC_GCFR->reg1 = 0x0; // Mask memory
    MXC_GCFR->reg3 = 0x0; // Reset

    return 1;
}

// Initializes all 4 CNN quadrants with max speed SRAM
int cnn_init(void)
{
    *((volatile uint32_t *)0x50001000) = 0x00000000; // AON control
    // Quadrant 0
    *((volatile uint32_t *)0x51000000) = 0x00100008; // Stop SM
    *((volatile uint32_t *)0x51000004) = 0x0000040e; // SRAM control
    // Quadrant 1
    *((volatile uint32_t *)0x52000000) = 0x00100008; // Stop SM
    *((volatile uint32_t *)0x52000004) = 0x0000040e; // SRAM control
    // Quadrant 2
    *((volatile uint32_t *)0x53000000) = 0x00100008; // Stop SM
    *((volatile uint32_t *)0x53000004) = 0x0000040e; // SRAM control
    // Quadrant 3
    *((volatile uint32_t *)0x54000000) = 0x00100008; // Stop SM
    *((volatile uint32_t *)0x54000004) = 0x0000040e; // SRAM control

    return 1;
}

// Utility function for incrementing a pointer in CNN data SRAM.
// There are 4 quadrants, so when memory addresses are "stitched"
// together the boundaries must be checked.
// Returns a new address pointer, or NULL if the address overflowed.
static inline uint32_t *increment_cnn_sram_ptr(uint32_t *ptr)
{
    int val = (int)ptr;
    if (val != CNN_QUAD0_DSRAM_END && val != CNN_QUAD1_DSRAM_END && val != CNN_QUAD2_DSRAM_END &&
        val != CNN_QUAD3_DSRAM_END) {
        return ptr + 1;
    } else if (val == CNN_QUAD0_DSRAM_END) { // Quadrant 0 end
        return (uint32_t *)CNN_QUAD1_DSRAM_START; // Quadrant 1 start
    } else if (val == CNN_QUAD1_DSRAM_END) { // Quadrant 1 end
        return (uint32_t *)CNN_QUAD2_DSRAM_START; // Quadrant 2 start
    } else if (val == CNN_QUAD2_DSRAM_END) { // Quadrant 2 end
        return (uint32_t *)CNN_QUAD3_DSRAM_START; // Quadrant 3 start
    } else if (val >= CNN_QUAD3_DSRAM_END) { // Quadrant 3 end
        return NULL; // End of CNN SRAM, return NULL
    } else {
        return NULL;
    }
}

// Union for doing "in-place" type-casting of 4 bytes
// into a 32-bit word.  Using a union for this increasing
// the speed of the conversion in "write_bytes_to_cnn_sram"
// by almost 50%.
union bytes_to_word {
    uint8_t *b;
    uint32_t *word;
};

// Write 'len' bytes from 'bytes' to the CNN data SRAM pointer 'addr'.
// Returns the next-most empty address in CNN data SRAM.
static inline uint32_t *write_bytes_to_cnn_sram(uint8_t *bytes, int len, uint32_t *addr)
{
    int i = 0;
    union bytes_to_word u;

    while (i < len) {
        u.b = &bytes[i];
        *addr = *u.word;
        // ^ De-reference using the typecast through the union.
        // Bytes do not need to be reversed in this case, but if
        // necessary the built-in "__REV" instruction can be used
        i += 4;
        addr = increment_cnn_sram_ptr(addr);
    }

    return addr;
}

// Read 'len' bytes from the CNN data SRAM pointer 'addr' into the 'out_bytes' array.
// Returns the next-most CNN data SRAM address after all bytes have been read.
static inline uint32_t *read_bytes_from_cnn_sram(uint8_t *out_bytes, int len, uint32_t *addr)
{
    int i = 0;
    uint32_t word = *addr;

    while (i < len) {
        out_bytes[i] = word & 0xFF;
        out_bytes[i + 1] = (word >> 8) & 0xFF;
        out_bytes[i + 2] = (word >> 16) & 0xFF;
        out_bytes[i + 3] = (word >> 24) & 0xFF;
        addr = increment_cnn_sram_ptr(addr);
        word = *addr;
        i += 4;
    }

    return addr;
}

#endif // EXAMPLES_MAX78002_IMGCAPTURE_INCLUDE_CNN_MEMUTILS_H_
