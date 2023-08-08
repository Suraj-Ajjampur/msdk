#ifndef __ENCRYPT_MODULE_FOR_LIGHT_SENSOR__
#define __ENCRYPT_MODULE_FOR_LIGHT_SENSOR__

/**************************************************************************************************
  Header Files
**************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "board.h"
#include "tpu.h"

/**************************************************************************************************
  Local Functions
**************************************************************************************************/
char* AES128_ECB_enc(char* inputData);

#endif // __ENCRYPT_MODULE_FOR_LIGHT_SENSOR__