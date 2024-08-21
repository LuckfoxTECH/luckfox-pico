// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <unistd.h>
#include <arm_neon.h>

#include "unlock_data_proc.h"

#define CLIP(x, min, max) (x)<(min)?(min):((x)>(max)?(max):(x))
#define MAX(x, max) (x > max?max:x)

void unlock_raw16_to_raw8(short *src, uint8_t *dst,
		int w, int h, uint8_t blk) __attribute__((optimize("O3")));
void unlock_ir_binning(short *src, uint8_t *dst,
		int w, int h, uint8_t blk, uint8_t shift_bit) __attribute__((optimize("O3")));
void unlock_pro_binning(unsigned short *src,
		unsigned short *dst, int w, int h) __attribute__((optimize("O3")));

void unlock_pro_8x8_binning(uint16_t *src,
		uint8_t *dst, int w, int h) __attribute__((optimize("O3")));

void unlock_pro_4x4_binning(uint16_t *src,
		uint8_t *dst, int w, int h) __attribute__((optimize("O3")));

//5ms
void unlock_raw16_to_raw8(short *src, uint8_t *dst,
		int w, int h, uint8_t blk)
{
	int16x8_t vIR_BLC = vdupq_n_s16(blk);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j+=8) {
			int wh = i * w + j;
			int16x8_t vsrc = vld1q_s16(&src[wh]);
			vsrc = vsubq_s16(vsrc, vIR_BLC);
			uint8x8_t vdst = vqmovun_s16(vsrc);
			vst1_u8(&dst[wh], vdst);
		}
	}
}

//12ms
void unlock_ir_binning(short *src, uint8_t *dst,
		int w, int h, uint8_t blk, int8_t shift_bit)
{
    int16x8_t vIR_BLC = vdupq_n_s16(blk * 4);

	int16x8_t vshift = vdupq_n_s16(-shift_bit);

	for (int i = 0; i < h; i+=2) {
		for (int j = 0; j < w; j+=(2 * 8)) {

			int src_index_1 = i * w + j;
			int src_index_2 = (i + 1) * w + j;
			int dst_index = (i / 2) * (w / 2) + (j / 2);

			int16x8_t vsum_1 = vdupq_n_s16(0);
			int16x8_t vsum_2 = vdupq_n_s16(0);
			int16x8_t vsum = vdupq_n_s16(0);

			int16x8x2_t vsrc_1 = vld2q_s16(&src[src_index_1]);
			int16x8x2_t vsrc_2 = vld2q_s16(&src[src_index_2]);

			vsum_1 = vaddq_s16(vsrc_1.val[0], vsrc_1.val[1]);
			vsum_2 = vaddq_s16(vsrc_2.val[0], vsrc_2.val[1]);
			vsum = vaddq_s16(vsum_1, vsum_2);
			vsum = vsubq_s16(vsum, vIR_BLC);
			//vsum = vshrq_n_s16(vsum, 2);
			vsum = vshlq_s16(vsum, vshift);

			uint8x8_t vdst = vqmovun_s16(vsum);
			vst1_u8(&dst[dst_index], vdst);
		}
	}
}

//12ms
void unlock_pro_binning(unsigned short *src,
		unsigned short *dst, int w, int h)
{
	for (int i = 0; i < h; i+=2){
		for (int j = 0; j < w; j+=(2 * 8)){

			int src_index_1 = i * w + j;
			int src_index_2 = (i + 1) * w + j;
			int dst_index = (i / 2) * (w / 2) + (j / 2);

			uint16x8_t vsum_1 = vdupq_n_u16(0);
			uint16x8_t vsum_2 = vdupq_n_u16(0);
			uint16x8_t vsum = vdupq_n_u16(0);

			uint16x8x2_t vsrc_1 = vld2q_u16(&src[src_index_1]);
			uint16x8x2_t vsrc_2 = vld2q_u16(&src[src_index_2]);

			vsum_1 = vaddq_u16(vsrc_1.val[0], vsrc_1.val[1]);
			vsum_2 = vaddq_u16(vsrc_2.val[0], vsrc_2.val[1]);
			vsum = vaddq_u16(vsum_1, vsum_2);
			vsum = vshrq_n_u16(vsum, 2);
			vst1q_u16(&dst[dst_index], vsum);
		}
	}
}

void unlock_pro_8x8_binning(uint16_t *src,
		uint8_t *dst, int w, int h)
{
	int dst_w = w / 8;
	int dst_h = h / 8;
	int w_index;
	int h_index;
	int index;

	printf("%s w:%d h:%d\n", __func__, w, h);

	uint16x8_t sum_vec;
	for (int i = 0; i < dst_h; i++){

		h_index = i * 8;
		for (int j = 0; j < dst_w; j++){

			w_index = j * 8;
			sum_vec = vdupq_n_u16(0);

			for (int h = 0; h < 8; h++) {
				index = (h_index + h) * w + w_index;
				uint16x8_t src_vec = vld1q_u16(&src[index]);
				sum_vec = vaddq_u16(sum_vec, src_vec);
			}

			uint16x4_t sum_vec_wide = vadd_u16(vget_low_u16(sum_vec), vget_high_u16(sum_vec));
			uint32x2_t sum_vec_wider = vpaddl_u16(sum_vec_wide);
			uint32_t sum = vget_lane_u32(sum_vec_wider, 0) + vget_lane_u32(sum_vec_wider, 1);
			uint32_t tmp = sum >> 6; // sum / 64
			dst[i * dst_w + j] = MAX(tmp, 255);
		}
	}
}

void unlock_pro_4x4_binning(uint16_t *src,
		uint8_t *dst, int w, int h)
{
	int dst_w = w / 8;
	int dst_h = h / 4;
	int w_index;
	int h_index;
	int index;
	int dst_h_index;

	printf("%s w:%d h:%d\n", __func__, w, h);

	if ((w & 0x7) != 0) {
		printf("not support\n");
		return;
	}

	uint16x8_t sum_vec;
	uint32x2_t MAX_VAL = vdup_n_u32(255);
	for (int i = 0; i < dst_h; i++){

		h_index = i * 4;
		dst_h_index = i * dst_w * 2;
		for (int j = 0; j < dst_w; j++){

			w_index = j * 8;
			sum_vec = vdupq_n_u16(0);

			for (int h = 0; h < 4; h++) {
				index = (h_index + h) * w + w_index;
				uint16x8_t src_vec = vld1q_u16(&src[index]);
				sum_vec = vaddq_u16(sum_vec, src_vec);
			}

			uint32x4_t sum_vec_wide = vpaddlq_u16(sum_vec);
			uint64x2_t sum_vec_wider = vpaddlq_u32(sum_vec_wide);

			uint32x2_t sum_vec_wider_1 = vqmovn_u64(sum_vec_wider);
			uint32x2_t sum_vec_wider_2 = vshr_n_u32(sum_vec_wider_1, 4);
			sum_vec_wider_2 = vmin_u32(sum_vec_wider_2, MAX_VAL);
			dst[dst_h_index + j * 2] = vget_lane_u32(sum_vec_wider_2, 0);
			dst[dst_h_index + j * 2 + 1] = vget_lane_u32(sum_vec_wider_2, 1);
		}
	}
}
