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

#include <ucl/ucl_types.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_retdefs.h>

#include <ucl/ucl_aes.h>
#include <ucl/ucl_aes_ecb.h>
#include <ucl/ucl_aes_cbc_mac.h>
#include <ucl/ucl_aes_cbc.h>
#include <string.h>

//used to not increment the *dst pointer in cbc mode when cbc-mac is targeted
int _ucl_increment_dst;

int ucl_aes_cbc(u8 *dst, u8 *src, u32 len, u8 *key, u32 keylen, u8 *IV, int mode)
{
    ucl_aes_ctx_t ctx;
    int ret;
    if ((src == NULL) || (key == NULL) || (IV == NULL)) {
        return UCL_INVALID_INPUT;
    }

    if ((dst == NULL)) {
        return UCL_INVALID_OUTPUT;
    }

    if ((mode != UCL_CIPHER_DECRYPT) && (mode != UCL_CIPHER_ENCRYPT)) {
        return UCL_INVALID_MODE;
    }

    if ((len % UCL_AES_BLOCKSIZE) != 0) {
        return UCL_INVALID_ARG;
    }

    if ((keylen != UCL_AES_KEYLEN_128) && (keylen != UCL_AES_KEYLEN_192) && (keylen != UCL_AES_KEYLEN_256)) {
        return UCL_INVALID_ARG;
    }

    ucl_aes_cbc_init(&ctx, key, keylen, IV, mode);

    ret = ucl_aes_cbc_core(dst, &ctx, src, len);
    //fault testing
    if (UCL_OK != ret) {
        return(ret);
    }

    ucl_aes_cbc_finish(&ctx);

    return UCL_OK;
}

int ucl_aes_cbc_init(ucl_aes_ctx_t *ctx, u8 *key, u32 keylen, u8 *IV, int mode)
{
    int resu;
    int i;

    if (ctx == NULL) {
        return UCL_INVALID_OUTPUT;
    }

    if (key == NULL) {
        return UCL_INVALID_INPUT;
    }

    if ((keylen != UCL_AES_KEYLEN_128) && (keylen != UCL_AES_KEYLEN_192) && (keylen != UCL_AES_KEYLEN_256)) {
        return UCL_INVALID_ARG;
    }

    _ucl_increment_dst = UCL_OK;

    for (i = 0; i < (int)keylen; i++) {
        ctx->origin_key[i] = key[i];
    }

    ctx->origin_keylen = (int)keylen;

    if (mode == UCL_CIPHER_ENCRYPT) {
        aes_set_ekey(&ctx->key, key, keylen);
    } else if (mode == UCL_CIPHER_DECRYPT) {
        aes_set_dkey(&ctx->key, key, keylen);
    } else {
        return UCL_INVALID_MODE;
    }

    memcpy(ctx->memory, IV, UCL_AES_BLOCKSIZE);
    ctx->mode = mode;
    return UCL_OK;
}

int ucl_aes_cbc_core(u8 *dst, ucl_aes_ctx_t *ctx, u8 *src, u32 len)
{
    int ret;
    u32 i;
    u8 block[UCL_AES_BLOCKSIZE];
    u8 *data_end;
    if ((ctx == NULL) || (dst == NULL)) {
        return UCL_INVALID_OUTPUT;
    }

    if (src == NULL) {
        return UCL_INVALID_INPUT;
    }

    if ((len % UCL_AES_BLOCKSIZE) != 0) {
        return UCL_INVALID_ARG;
    }

    data_end = len + src;

    if (ctx->mode == UCL_CIPHER_ENCRYPT) {
        for (i = 0; i < UCL_AES_BLOCKSIZE; i++) {
            block[i] = src[i] ^ ctx->memory[i];
        }

        aes_encrypt(dst, block, &ctx->key);
        src += UCL_AES_BLOCKSIZE;

        while (src != data_end) {
            for (i = 0; i < UCL_AES_BLOCKSIZE; i++) {
                block[i] = src[i] ^ dst[i];
            }

            if (UCL_OK == _ucl_increment_dst) {
                dst += UCL_AES_BLOCKSIZE;
            }

            src += UCL_AES_BLOCKSIZE;
            aes_encrypt(dst, block, &ctx->key);
        }

        memcpy(ctx->memory, dst, UCL_AES_BLOCKSIZE);

    } else if (ctx->mode == UCL_CIPHER_DECRYPT) {
        u8 save_src[UCL_AES_BLOCKSIZE];
        aes_decrypt(block, src, &ctx->key);

        // Xor dest with IV
        for (i = 0; i < UCL_AES_BLOCKSIZE; i++) {
            u8 s = src[i];
            dst[i] = block[i] ^ ctx->memory[i];
            save_src[i] = s;
        }

        // Go to next dest block
        dst += UCL_AES_BLOCKSIZE;
        while ((src + UCL_AES_BLOCKSIZE) != data_end) {
            aes_decrypt(block, (src + UCL_AES_BLOCKSIZE), &ctx->key);

            // Xor
            for (i = 0; i < UCL_AES_BLOCKSIZE; i++) {
                u8 s = src[UCL_AES_BLOCKSIZE+i];
                dst[i] = block[i] ^ save_src[i];
                save_src[i] = s;
            }

            dst += UCL_AES_BLOCKSIZE;
            src += UCL_AES_BLOCKSIZE;
        }

        // Save IV for next call
        memcpy(ctx->memory, src, UCL_AES_BLOCKSIZE);
    }
    return UCL_OK;
}

int ucl_aes_cbc_finish(ucl_aes_ctx_t *ctx)
{
    if (ctx == NULL) {
        return UCL_INVALID_OUTPUT;
    }

    memset((unsigned char *)ctx, 0, sizeof(*ctx));

    return UCL_OK;
}
