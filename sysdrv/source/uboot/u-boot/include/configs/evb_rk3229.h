/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __RK322X_CONFIG_H
#define __RK322X_CONFIG_H

#include <configs/rk322x_common.h>

#define ROCKCHIP_DEVICE_SETTINGS \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

/* Store env in emmc */
#define CONFIG_SYS_MMC_ENV_DEV          0
#define CONFIG_SYS_MMC_ENV_PART         0
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

#ifndef CONFIG_SPL_BUILD

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND
#endif

#endif
