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

#ifndef LIBRARIES_MAXUSB_INCLUDE_DEVCLASS_HID_KBD_H_
#define LIBRARIES_MAXUSB_INCLUDE_DEVCLASS_HID_KBD_H_

/**
 * @file  hid_kbd.h
 * @brief Human Interface Device Class (Keyboard) over USB
 */

#include <stdint.h>
#include "hid.h"
#include "usb_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 *  \brief    Initialize the class driver
 *  \details  Initialize the class driver.
 *  \param    hid_descriptor      pointer to the descriptor to be used in response to getdescriptor requests
 *  \param    report_descriptor   pointer to the descriptor to be used in response to getdescriptor requests
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidkbd_init(const MXC_USB_interface_descriptor_t *if_desc, const hid_descriptor_t *hid_descriptor, const uint8_t *report_descriptor);

/** 
 *  \brief    Set the specified configuration
 *  \details  Configures the class and endpoint(s) and starts operation. This function should be
 *            called upon configuration from the host.
 *  \param    cfg   configuration to be set
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidkbd_configure(unsigned int ep);

/**
 *  \brief    Clear the current configuration and resets endpoint(s)
 *  \details  Clear the current configuration and resets endpoint(s).
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidkbd_deconfigure(void);

/** 
 *  \brief    Send one ASCII value (key) encoded as a keyboard code to the host
 *  \param    key   ASCII value
 *  \return   Zero (0) for success, non-zero for failure
 */
int hidkbd_keypress(uint8_t key);

/**
 *  \brief    ASCII-to-HID mapping (partial)
 *  \details  HID Keyboard/Keypad Page codes do not map directly to ASCII, unfortunately. This partial mapping of
 *            the most commonly used codes is provided for demonstration. Please refer to the HID Usage Tables
 *            1.12 section 10 "Keyboard/Keypad Page (0x07)" section for a complete listing.
 *  \param    rpt   Encoded report. Must point to 3 * num bytes of storage, as each report is 3 bytes
 *  \param    ascii Characters to encode
 *  \param    num   Number of characters to encode
 *  \return   size of the encoded report
 */
int hidkbd_encode_report(uint8_t *rpt, uint8_t *ascii, int num);

#ifdef __cplusplus
}
#endif

#endif //LIBRARIES_MAXUSB_INCLUDE_DEVCLASS_HID_KBD_H_
