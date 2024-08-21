/*
 * Copyright © 2016 Siarhei Siamashka <siarhei.siamashka@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __AARCH64_ASM_H__
#define __AARCH64_ASM_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void aligned_block_copy_ldpstp_x_aarch64(int64_t * __restrict dst,
                                         int64_t * __restrict src,
                                         int                  size);
void aligned_block_copy_ldpstp_q_aarch64(int64_t * __restrict dst,
                                         int64_t * __restrict src,
                                         int                  size);
void aligned_block_copy_ld1st1_aarch64(int64_t * __restrict dst,
                                       int64_t * __restrict src,
                                       int                  size);

void aligned_block_copy_ldpstp_q_pf32_l2strm_aarch64(int64_t * __restrict dst,
                                                     int64_t * __restrict src,
                                                     int                  size);
void aligned_block_copy_ldpstp_q_pf64_l2strm_aarch64(int64_t * __restrict dst,
                                                     int64_t * __restrict src,
                                                     int                  size);
void aligned_block_copy_ldpstp_q_pf32_l1keep_aarch64(int64_t * __restrict dst,
                                                     int64_t * __restrict src,
                                                     int                  size);
void aligned_block_copy_ldpstp_q_pf64_l1keep_aarch64(int64_t * __restrict dst,
                                                     int64_t * __restrict src,
                                                     int                  size);

void aligned_block_fill_stp_x_aarch64(int64_t * __restrict dst,
                                      int64_t * __restrict src,
                                      int                  size);
void aligned_block_fill_stp_q_aarch64(int64_t * __restrict dst,
                                      int64_t * __restrict src,
                                      int                  size);

void aligned_block_fill_stnp_x_aarch64(int64_t * __restrict dst,
                                       int64_t * __restrict src,
                                       int                  size);
void aligned_block_fill_stnp_q_aarch64(int64_t * __restrict dst,
                                       int64_t * __restrict src,
                                       int                  size);

#ifdef __cplusplus
}
#endif

#endif
