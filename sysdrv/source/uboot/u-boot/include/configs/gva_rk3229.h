/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIGS_GVA_RK3229_H
#define __CONFIGS_GVA_RK3229_H

#include <configs/rk322x_common.h>

/* Read CPUID and set "serial#" env. */
#define CONFIG_MISC_INIT_R
#define CONFIG_SERIAL_TAG
#define CONFIG_ENV_OVERWRITE

/* Store env in emmc */
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE                 (32 << 10)
#define CONFIG_SYS_MMC_ENV_DEV          0
#define CONFIG_SYS_MMC_ENV_PART         0
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

#ifndef CONFIG_SPL_BUILD
/* Enable gpt partition table */
#undef PARTS_DEFAULT
#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=bootloader_a,start=4M,size=4M,uuid=${uuid_gpt_loader};" \
	"name=bootloader_b,size=4M,uuid=${uuid_gpt_reserved};" \
	"name=tos_a,size=4M,uuid=${uuid_gpt_reserved};" \
	"name=tos_b,size=4M,uuid=${uuid_gpt_reserved};" \
	"name=misc,size=4M,uuid=${uuid_gpt_misc};" \
	"name=metadata,size=16M,uuid=${uuid_gpt_metadata};" \
	"name=boot_a,size=32M,uuid=${uuid_gpt_boot_a};" \
	"name=boot_b,size=32M,uuid=${uuid_gpt_boot_b};" \
	"name=system_a,size=512M,uuid=${uuid_gpt_system_a};" \
	"name=system_b,size=512M,uuid=${uuid_gpt_system_b};" \
	"name=vendor_a,size=50M,uuid=${uuid_gpt_vendor_a};" \
	"name=vendor_b,size=50M,uuid=${uuid_gpt_vendor_b};" \
	"name=oem_a,size=32M,uuid=${uuid_gpt_oem_a};" \
	"name=oem_b,size=32M,uuid=${uuid_gpt_oem_b};" \
	"name=cache,size=100M,uuid=${uuid_gpt_cache};" \
	"name=persist,size=4M,uuid=${uuid_gpt_persist};" \
	"name=userdata,size=-,uuid=${uuid_gpt_userdata};\0" \

#undef CONFIG_PREBOOT
#define CONFIG_PREBOOT \
	"mmc dev 0; " \
	"gpt guid mmc 0; " \
	"if test $? = 1; then " \
		"fastboot usb 0; " \
	"fi; "

#define CONFIG_SYS_BOOT_RAMDISK_HIGH
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"bootavb flow v; " \

/* Enable atags */
#define CONFIG_SYS_BOOTPARAMS_LEN	(64*1024)
#define CONFIG_INITRD_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG

#define ROCKCHIP_DEVICE_SETTINGS
#endif

#endif
