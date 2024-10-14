/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 */

#ifndef _SPL_DISPLAY_H_
#define _SPL_DISPLAY_H_

#include <common.h>
#include <drm_modes.h>
#include <mp_boot.h>

/* SPL display */
#define RK3528_VOP_BASE				0xff840000
#define RK3528_HDMI_BASE			0xff8d0000
#define RK3528_HDMIPHY_BASE			0xffe00000
#define RK3528_CRU_BASE				0xff4a0000
#define RK3528_GPIO0_IOC_BASE			0xff540000
#define RK3528_GPIO_BASE			0xff610000

struct spl_display_info {
	struct drm_display_mode mode;
	u32 bus_format;
	u32 enabled;
};
#endif

