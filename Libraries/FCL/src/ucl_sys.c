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

#include "ucl/ucl_config.h"
#include "ucl/ucl_defs.h"
#include "ucl/ucl_retdefs.h"
#include "ucl/ucl_types.h"

#include <ucl/ucl_hash.h>
#ifdef HASH_SHA256
#include <ucl/ucl_sha256.h>
#endif
#ifdef HASH_SHA384
#include <ucl/ucl_sha384.h>
#endif
#ifdef HASH_SHA512
#include <ucl/ucl_sha512.h>
#endif
#ifdef HASH_SIA256
#include <ucl/ucl_sia256.h>
#endif

#ifdef HASH_SHA3
#include <ucl/ucl_sha3.h>
#endif

__API__ int hash_size[MAX_HASH_FUNCTIONS];

int __API__ ucl_init(void)
{
#ifdef HASH_SHA256
    hash_size[UCL_SHA256] = UCL_SHA256_HASHSIZE;
#endif
#ifdef HASH_SHA512
    hash_size[UCL_SHA512] = UCL_SHA256_HASHSIZE;
#endif
#ifdef HASH_SHA384
    hash_size[UCL_SHA384] = UCL_SHA256_HASHSIZE;
#endif
#ifdef HASH_SHA3
    hash_size[UCL_SHA3] = UCL_SHA3_512_HASHSIZE;
#endif
#ifdef HASH_SIA256
    hash_size[UCL_SIA256] = UCL_SIA256_HASHSIZE;
#endif
    return(UCL_OK);
}

