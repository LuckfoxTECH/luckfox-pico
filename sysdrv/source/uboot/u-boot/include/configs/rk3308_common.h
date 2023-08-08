/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_RK3308_COMMON_H
#define __CONFIG_RK3308_COMMON_H

#include "rockchip-common.h"

#define CONFIG_SYS_MALLOC_LEN		(10 << 20)
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_SIZE		(256 * 1024 * 1024)
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE		0x00000000
#define CONFIG_SPL_MAX_SIZE		0x40000
#define CONFIG_SPL_BSS_START_ADDR	0x00400000
#define CONFIG_SPL_BSS_MAX_SIZE		0x2000
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x8000

#define CONFIG_SYS_NS16550_MEM32

#define CONFIG_SYS_TEXT_BASE		0x00600000
#define CONFIG_SYS_INIT_SP_ADDR		0x00800000
#define CONFIG_SYS_LOAD_ADDR		0x00C00800
#define CONFIG_SPL_STACK		0x00400000
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)	/* 64M */

#define COUNTER_FREQUENCY		24000000

#define GICD_BASE			0xff581000
#define GICC_BASE			0xff582000

#define OTP_SECURE_BOOT_ENABLE_ADDR	0x0
#define OTP_SECURE_BOOT_ENABLE_SIZE	1
#define OTP_RSA_HASH_ADDR		0x10
#define OTP_RSA_HASH_SIZE		32

#define CONFIG_SYS_BOOTM_LEN	(64 << 20)	/* 64M */

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

#define CONFIG_SYS_SDRAM_BASE		0
#define SDRAM_MAX_SIZE			0xff000000
#define SDRAM_BANK_SIZE			(2UL << 30)
#ifdef CONFIG_DM_DVFS
#define CONFIG_PREBOOT			"dvfs repeat"
#else
#define CONFIG_PREBOOT
#endif

#ifndef CONFIG_SPL_BUILD

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_ROCKUSB_G_DNL_PID        0x330d

#ifdef CONFIG_ARM64
#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x00500000\0" \
	"pxefile_addr_r=0x00600000\0" \
	"fdt_addr_r=0x01f00000\0" \
	"kernel_addr_no_low_bl32_r=0x00280000\0" \
	"kernel_addr_r=0x00680000\0" \
	"kernel_addr_c=0x02480000\0" \
	"ramdisk_addr_r=0x04000000\0"
#else
#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x00500000\0" \
	"pxefile_addr_r=0x00600000\0" \
	"fdt_addr_r=0x02800000\0" \
	"kernel_addr_r=0x00058000\0" \
	"kernel_addr_c=0x2008000\0" \
	"ramdisk_addr_r=0x02900000\0"
#endif

#include <config_distro_bootcmd.h>
#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS \
	"partitions=" PARTS_DEFAULT \
	ROCKCHIP_DEVICE_SETTINGS \
	RKIMG_DET_BOOTDEV \
	BOOTENV_SHARED_RKNAND \
	BOOTENV

#endif

#endif
