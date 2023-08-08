/*
 * (C) Copyright 2015 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>
#include <configs/rk3036_common.h>

/* Store env in emmc */
#define CONFIG_SYS_MMC_ENV_DEV          0
#define CONFIG_SYS_MMC_ENV_PART         0
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

#ifndef CONFIG_SPL_BUILD
/* Enable gpt partition table */
#undef CONFIG_PREBOOT
#define CONFIG_PREBOOT
#define CONFIG_SYS_BOOT_RAMDISK_HIGH

/* Store env in emmc */
#define CONFIG_SYS_MMC_ENV_DEV		0 /* emmc */
#define CONFIG_SYS_MMC_ENV_PART		0 /* user area */

/* Enable atags */
#define CONFIG_SYS_BOOTPARAMS_LEN	(64*1024)
#define CONFIG_INITRD_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND

#endif

#endif
