/**
 * \file bn_mul.h
 *
 * \brief  Multi-precision integer library
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 *      Multiply source vector [s] with b, add result
 *       to destination vector [d] and set carry c.
 *
 *      Currently supports:
 *
 *         . IA-32 (386+)         . AMD64 / EM64T
 *         . IA-32 (SSE2)         . Motorola 68000
 *         . PowerPC, 32-bit      . MicroBlaze
 *         . PowerPC, 64-bit      . TriCore
 *         . SPARC v8             . ARM v3+
 *         . Alpha                . MIPS32
 *         . C, longlong          . C, generic
 */
#ifndef MBEDTLS_BN_MUL_H
#define MBEDTLS_BN_MUL_H

#include "bignum.h"

#if !defined(MULADDC_CORE)
#if defined(MBEDTLS_HAVE_UDBL)

#define MULADDC_INIT                    \
{                                       \
    mbedtls_t_udbl r;                           \
    mbedtls_mpi_uint r0, r1;

#define MULADDC_CORE                    \
    r   = *(s++) * (mbedtls_t_udbl) b;          \
    r0  = (mbedtls_mpi_uint) r;                   \
    r1  = (mbedtls_mpi_uint)( r >> biL );         \
    r0 += c;  r1 += (r0 <  c);          \
    r0 += *d; r1 += (r0 < *d);          \
    c = r1; *(d++) = r0;

#define MULADDC_STOP                    \
}

#else
#define MULADDC_INIT                    \
{                                       \
    mbedtls_mpi_uint s0, s1, b0, b1;              \
    mbedtls_mpi_uint r0, r1, rx, ry;              \
    b0 = ( b << biH ) >> biH;           \
    b1 = ( b >> biH );

#define MULADDC_CORE                    \
    s0 = ( *s << biH ) >> biH;          \
    s1 = ( *s >> biH ); s++;            \
    rx = s0 * b1; r0 = s0 * b0;         \
    ry = s1 * b0; r1 = s1 * b1;         \
    r1 += ( rx >> biH );                \
    r1 += ( ry >> biH );                \
    rx <<= biH; ry <<= biH;             \
    r0 += rx; r1 += (r0 < rx);          \
    r0 += ry; r1 += (r0 < ry);          \
    r0 +=  c; r1 += (r0 <  c);          \
    r0 += *d; r1 += (r0 < *d);          \
    c = r1; *(d++) = r0;

#define MULADDC_STOP                    \
}

#endif /* C (generic)  */
#endif /* C (longlong) */


#endif /* bn_mul.h */
