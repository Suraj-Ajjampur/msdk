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

#ifndef LIBRARIES_FCL_INCLUDE_UCL_UCL_SHA512_H_
#define LIBRARIES_FCL_INCLUDE_UCL_UCL_SHA512_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#include <ucl/ucl_types.h>
#include <ucl/ucl_hash.h>
#include <ucl/ucl_sha512.h>

/** @file ucl_sha512.h
 * @defgroup UCL_SHA512 SHA512
 * Secure Hash Algorithm 512, from FIPS 180-2.
 *
 * @par Header:
 * @link ucl_sha512.h ucl_sha512.h @endlink
 *
 * SHA-512 is a data-digest algorithm and a component of
 * the Standard FIPS 180-2.
 * The algorithm takes as input a data of arbitrary length and
 * produces as output a 512-bit "fingerprint" or "data digest"
 * of the input.@n
 * @n
 * Given a data of any length, a single "1" bit is appended at the end of the
 * data. This bit must always be added. Then, a set of "0" bits are also
 * appended so that the data length is 448 modulo 512. Finally, the length
 * of the data is appended. @n
 *@n
 *
 * <b>SHA512 Descriptor:</b> @n
 * @li Block length : 512 bits
 * @li Hash length : 512 bits
 *
 * @ingroup UCL_HASH
 */


/** <b>The SHA512 context</b>.
 * This structure is associated to the 'step by step' process.
 *
 * @ingroup UCL_SHA512
 */

struct ucl_sha512_ctx
{
    // Intermediate and then final hash.
    u64 state[8];
    // Counter in bits.
    u64 count[2];
    // Buffer
    u8 buffer[128];
};

/** <b>The SHA512 context</b>.
 * @ingroup UCL_SHA512
 */

typedef struct ucl_sha512_ctx ucl_sha512_ctx_t;


/** <b>Core block size</b>.
 * Byte size of a SHA512 core block.
 *
 * @ingroup UCL_SHA512
 */
#define UCL_SHA512_BLOCKSIZE 128
/** <b>Hash size</b>.
 * Byte size of the output of SHA512.
 *
 * @ingroup UCL_SHA512
 */
#define UCL_SHA512 4
#define UCL_SHA512_HASHSIZE 64
/** <b>Hash size</b>.
 * 32-bits word size of the output of SHA512.
 *
 * @ingroup UCL_SHA512
 */
#define UCL_SHA512_HASHW32SIZE 16


/** <b>SHA512</b>.
 * The complete process of SHA512.
 *
 * @pre Hash byte length is equal to 32
 *
 * @param[out] hash         Pointer to the digest
 * @param[in]  data         Pointer to the data
 * @param[in]  data_byteLen Data byte length
 *
 * @return Error code
 *
 * @retval #UCL_OK             if no error occurred
 * @retval #UCL_INVALID_OUTPUT if @p hash is #NULL
 *
 * @ingroup UCL_SHA512
 */
int ucl_sha512(u8 *hash, u8 *data, u32 data_byteLen);


/** <b>SHA512 Init</b>.
 * The initialisation of SHA512.
 *
 * @param[in, out] context Pointer to the context
 *
 * @return Error code
 *
 * @retval #UCL_OK            if no error occurred
 * @retval #UCL_INVALID_INPUT if @p context is #NULL
 *
 * @ingroup UCL_SHA512
 */
int  ucl_sha512_init(ucl_sha512_ctx_t *context);

/** <b>SHA512 Core</b>.
 * The core of SHA512.
 *
 * @param[in, out] context      Pointer to the context
 * @param[in]     data         Pointer to the data
 * @param[in]     data_byteLen Data byte length
 *
 * @warning #ucl_sha512_init must be processed before, and
 *          #ucl_sha512_finish should be processed to get the final hash.
 *
 * @return Error code
 *
 * @retval #UCL_OK  if no error occurred
 * @retval #UCL_NOP if @p data_byteLen = 0 or if @p data is the pointer #NULL
 *
 * @ingroup UCL_SHA512
 */
int  ucl_sha512_core(ucl_sha512_ctx_t *context, u8 *data,
                    u32 data_byteLen);

/** <b>SHA512 Finish</b>.
 * Finish the process of SHA512.
 *
 * @pre Hash byte length is equal to 64
 *
 * @param[out]    hash Pointer to the digest
 * @param[in, out] context Pointer to the context
 *
 * @warning #ucl_sha512_init and #ucl_sha512_core must be processed before.
 *
 * @return Error code
 *
 * @retval #UCL_OK             if no error occurred
 * @retval #UCL_INVALID_OUTPUT if @p hash is the pointer #NULL
 * @retval #UCL_INVALID_INPUT  if @p context is the pointer #NULL
 *
 * @ingroup UCL_SHA512
 */
int  ucl_sha512_finish(u8 *hash, ucl_sha512_ctx_t *context);


void sha512_stone(u64 hash[8], u64 stone[16]);

int ucl_hmac_sha512(u8 *mac, u32 mac_byteLen, u8 *message, u32 message_byteLen,
            u8 *key, u32 key_byteLen);
int ucl_hmac_sha512_init(ucl_sha512_ctx_t *context , u8 *key, u32 key_byteLen);
int ucl_hmac_sha512_core(ucl_sha512_ctx_t *context, u8 *data, u32 byteLen);
int ucl_hmac_sha512_finish(u8 *mac, u32 mac_byteLen, ucl_sha512_ctx_t *context,
                           u8 *key, u32 key_byteLen);
#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif // LIBRARIES_FCL_INCLUDE_UCL_UCL_SHA512_H_

