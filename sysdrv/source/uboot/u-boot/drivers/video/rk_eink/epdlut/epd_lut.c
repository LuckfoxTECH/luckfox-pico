// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Zorro Liu <zorro.liu@rock-chips.com>
 */
#include <stdio.h>
#include <common.h>
#include <stdlib.h>

#include "epd_lut.h"

int epd_lut_from_mem_init(void *waveform, struct epd_lut_ops *ops)
{
	int ret = -1;

	ret = rkf_wf_input(waveform);
	if (ret < 0) {
		printf("[lut]: failed to input RKF waveform\n");
	} else {
		printf("[lut]: RKF waveform\n");
		ops->lut_get = rkf_wf_get_lut;
		return 0;
	}

	ret = pvi_wf_input(waveform);
	if (ret < 0) {
		printf("[lut]: failed to input PVI waveform\n");
	} else {
		printf("[lut]: PVI waveform\n");
		ops->lut_get = pvi_wf_get_lut;
		return 0;
	}

	return ret;
}

const char *epd_lut_get_wf_version(void)
{
	if (rkf_wf_get_version())
		return rkf_wf_get_version();
	if (pvi_wf_get_version())
		return pvi_wf_get_version();
	return NULL;
}
