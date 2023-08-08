/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (C) Rockchip Electronics Co.Ltd
 * Author:
 *      Zhang Yubing <yubing.zhang@rock-chips.com>
 */

#ifndef _ROCKCHIP_POST_CSC_H
#define _ROCKCHIP_POST_CSC_H

#include <linux/kernel.h>
#include <edid.h>

struct post_csc_coef {
	s32 csc_coef00;
	s32 csc_coef01;
	s32 csc_coef02;
	s32 csc_coef10;
	s32 csc_coef11;
	s32 csc_coef12;
	s32 csc_coef20;
	s32 csc_coef21;
	s32 csc_coef22;

	s32 csc_dc0;
	s32 csc_dc1;
	s32 csc_dc2;

	u32 range_type;
};

int rockchip_calc_post_csc(struct csc_info *csc, struct post_csc_coef *csc_coef,
			   int csc_mode, bool is_input_yuv, bool is_output_yuv);

#endif
