/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * Copyright (c) 2018 Rockchip Electronics Co., Ltd
 */

#ifndef __CONFIGS_RK3368_EVB_H
#define __CONFIGS_RK3368_EVB_H

#include <configs/rk3368_common.h>

#ifndef CONFIG_SPL_BUILD

#undef ROCKCHIP_DEVICE_SETTINGS
#define ROCKCHIP_DEVICE_SETTINGS \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND
#endif

/* rockchip ohci host driver */
#define CONFIG_USB_OHCI_NEW
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	1

#endif
