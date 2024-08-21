/*
 * (C) Copyright 2015 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_RK3288_COMMON_H
#define __CONFIG_RK3288_COMMON_H

#include <asm/arch/hardware.h>
#include "rockchip-common.h"

#ifndef CONFIG_SPL_BUILD
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND
#endif

#define CONFIG_SKIP_LOWLEVEL_INIT_ONLY
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		1024

#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SYS_TEXT_BASE		0x00200000
#define CONFIG_SYS_INIT_SP_ADDR		0x00400000
#define CONFIG_SYS_LOAD_ADDR		0x00800800
#define CONFIG_SPL_STACK		0x00180000
#define CONFIG_SPL_TEXT_BASE		0x00000000
#define CONFIG_SPL_MAX_SIZE		0x40000

#define CONFIG_SYS_BOOTM_LEN		(64 << 20)	/*  64M */
#define GICD_BASE			0xffc01000
#define GICC_BASE			0xffc02000

#define CONFIG_ROCKUSB_G_DNL_PID	0x320A

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

/* RAW SD card / eMMC locations. */
#define CONFIG_SYS_SPI_U_BOOT_OFFS	(128 << 10)

/* FAT sd card locations. */
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME		"u-boot.img"

#define CONFIG_SYS_SDRAM_BASE		0
#define SDRAM_BANK_SIZE			(2UL << 30)
#define SDRAM_MAX_SIZE			0xfe000000

#ifndef CONFIG_SPL_BUILD
/* usb otg */
#define CONFIG_ROCKCHIP_USB2_PHY

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_CMD_USB_MASS_STORAGE

/* usb host support */
#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x00000000\0" \
	"pxefile_addr_r=0x00100000\0" \
	"fdt_addr_r=0x08100000\0" \
	"kernel_addr_r=0x02008000\0" \
	"ramdisk_addr_r=0x0a200000\0"

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
