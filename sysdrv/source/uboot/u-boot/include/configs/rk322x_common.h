/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __CONFIG_RK322X_COMMON_H
#define __CONFIG_RK322X_COMMON_H

#include <asm/arch/hardware.h>
#include "rockchip-common.h"

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BOOTM_LEN	(64 << 20)	/*  64M */


#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SYS_TEXT_BASE		0x60200000
#define CONFIG_SYS_INIT_SP_ADDR		0x60400000
#define CONFIG_SYS_LOAD_ADDR		0x61800800
#define CONFIG_SPL_TEXT_BASE		0x60000000

#define GICD_BASE                 	0x32011000
#define GICC_BASE                  	0x32012000

#define CONFIG_ROCKCHIP_MAX_INIT_SIZE	(28 << 10)
#define CONFIG_ROCKCHIP_CHIP_TAG	"RK32"
#define CONFIG_ROCKUSB_G_DNL_PID	0x320B

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

#define CONFIG_SYS_SDRAM_BASE		0x60000000
#define SDRAM_BANK_SIZE			(512UL << 20UL)
#define SDRAM_MAX_SIZE			0x80000000

#ifndef CONFIG_SPL_BUILD

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE

/* usb host */
#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x60000000\0" \
	"pxefile_addr_r=0x60100000\0" \
	"fdt_addr_r=0x68300000\0" \
	"kernel_addr_r=0x62008000\0" \
	"ramdisk_addr_r=0x6a200000\0"

#include <config_distro_bootcmd.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	"partitions=" PARTS_DEFAULT \
	ENV_MEM_LAYOUT_SETTINGS \
	ROCKCHIP_DEVICE_SETTINGS \
	RKIMG_DET_BOOTDEV \
	BOOTENV
#endif

#define CONFIG_PREBOOT

#endif
