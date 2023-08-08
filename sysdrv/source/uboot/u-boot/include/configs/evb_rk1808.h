/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 *
 */

#ifndef __EVB_RK1808_H
#define __EVB_RK1808_H

#include <configs/rk1808_common.h>

#define ROCKCHIP_DEVICE_SETTINGS \
			"stdout=serial,vidconsole\0" \
			"stderr=serial,vidconsole\0"

#define CONFIG_SYS_MMC_ENV_DEV		0

#ifndef CONFIG_SPL_BUILD
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND
#endif

#endif
