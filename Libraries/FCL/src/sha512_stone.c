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

#include <ucl/ucl_hash.h>
#ifdef HASH_SHA512

#include "ucl/ucl_config.h"
#include "ucl/ucl_types.h"
#include "ucl/ucl_defs.h"
#include "ucl/ucl_retdefs.h"
#include "ucl/ucl_sha512.h"

#define ROTR(x, n)   (((x) >> (n)) | ((x) << (64 - n)))
#define SHR(x, n)    ((x) >> (n))
#define CH(x, y, z)   ((z) ^ ((x) & ((y) ^ (z))))
#define MAJ(x, y, z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BIGSIGMA1(x)   (((ROTR(x, 14)) ^ (ROTR(x, 18)) ^ (ROTR(x, 41))))
#define BIGSIGMA0(x)   (((ROTR(x, 28)) ^ (ROTR(x, 34)) ^ (ROTR(x, 39))))
#define SIGMA0(x)   (((ROTR(x, 1)) ^ (ROTR(x, 8)) ^ (SHR(x, 7))))
#define SIGMA1(x)   ((ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6)))
#define ROUND(a, b, c, e, f, g, h, i)                                  \
                    T1 = h + BIGSIGMA1(e) + CH(e, f, g) + K[i] + W[i]; \
                    T2 = BIGSIGMA0(a) + MAJ(a, b, c);

static const u64 K[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL,
};

void sha512_stone(u64 hash[8], u64 stone[16])
{
    int i;
    u64 W[80];
    u64 A, B, C, D, E, F, G, H, T1, T2;

    A = hash[0];
    B = hash[1];
    C = hash[2];
    D = hash[3];
    E = hash[4];
    F = hash[5];
    G = hash[6];
    H = hash[7];

    for (i = 0; i < 16; i++) {
        W[i] = stone[i];
    }

    for (; i < 80; i++) {
        W[i] = SIGMA1(W[i-2]) + W[i-7] + SIGMA0(W[i-15]) + W[i-16];
    }

    for (i = 0; i < 80; i++) {
        ROUND(A, B, C, E, F, G, H, i);
        H = G;
        G = F;
        F = E;
        E = D + T1;
        D = C;
        C = B;
        B = A;
        A = T1 + T2;
    }

    hash[0] += A;
    hash[1] += B;
    hash[2] += C;
    hash[3] += D;
    hash[4] += E;
    hash[5] += F;
    hash[6] += G;
    hash[7] += H;
}
#endif//HASH_SHA512
