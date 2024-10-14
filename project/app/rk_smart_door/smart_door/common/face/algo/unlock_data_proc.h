// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __UNLOCK_DATA_PROC_H__
#define __UNLOCK_DATA_PROC_H__

#ifdef __cplusplus
extern "C" {
#endif

void unlock_raw16_to_raw8(short *src, uint8_t *dst,
		int w, int h, uint8_t blk);

void unlock_ir_binning(short *src, uint8_t *dst,
		int w, int h, uint8_t blk, int8_t shift_bit);

void unlock_pro_binning(unsigned short *src,
		unsigned short *dst, int w, int h);

void unlock_pro_8x8_binning(uint16_t *src,
		uint8_t *dst, int w, int h);

void unlock_pro_4x4_binning(uint16_t *src,
		uint8_t *dst, int w, int h);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
