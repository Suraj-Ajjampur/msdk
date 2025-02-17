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

/*============================================================================
 *
 * Purpose : SHA384
 *
 *========================================================================== */

#include <string.h>
#include <ucl/ucl_hash.h>
#ifdef HASH_SHA384

#include <ucl/ucl_config.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_types.h>

#include <ucl/ucl_sha384.h>
#include <ucl/ucl_sys.h>
#include <ucl/bignum_ecdsa_generic_api.h>

extern int using_sha_hardware;

int __API__ ucl_sha384_init(ucl_sha384_ctx_t *ctx)
{
    if (ctx == NULL) {
        return UCL_INVALID_INPUT;
    }

    ctx->state[0] = 0xcbbb9d5dc1059ed8ULL;
    ctx->state[1] = 0x629a292a367cd507ULL;
    ctx->state[2] = 0x9159015a3070dd17ULL;
    ctx->state[3] = 0x152fecd8f70e5939ULL;
    ctx->state[4] = 0x67332667ffc00b31ULL;
    ctx->state[5] = 0x8eb44a8768581511ULL;
    ctx->state[6] = 0xdb0c2e0d64f98fa7ULL;
    ctx->state[7] = 0x47b5481dbefa4fa4ULL;

    ctx->count[0] = 0;
    ctx->count[1] = 0;

    return UCL_OK;
}

int ucl_sha384_core(ucl_sha384_ctx_t *ctx, u8 *data, u32 dataLen)
{
    return(ucl_sha512_core(ctx, data, dataLen));
}

int ucl_sha384_finish(u8 *hash, ucl_sha384_ctx_t *ctx)
{
    u8 sha512_hash[64];
    int i;

    ucl_sha512_finish(sha512_hash, ctx);
    for (i = 0; i < UCL_SHA384_HASHSIZE; i++) {
        hash[i] = sha512_hash[i];
    }

    return UCL_OK;
}
int ucl_sha384(u8 *hash, u8 *message, u32 byteLength)
{
    ucl_sha384_ctx_t ctx;

    if (hash == NULL) {
        return UCL_INVALID_OUTPUT;
    }

    ucl_sha384_init(&ctx);
    ucl_sha384_core(&ctx, message, byteLength);
    ucl_sha384_finish(hash, &ctx);

    return UCL_OK;
}

#endif//HASH_SHA384
