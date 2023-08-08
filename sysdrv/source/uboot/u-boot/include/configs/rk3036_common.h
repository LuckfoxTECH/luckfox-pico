/*
 * (C) Copyright 2015 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __CONFIG_RK3036_COMMON_H
#define __CONFIG_RK3036_COMMON_H

#include <asm/arch/hardware.h>
#include "rockchip-common.h"

#define CONFIG_SYS_MALLOC_LEN		(20 << 20)
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)	/*  64M */

#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_MEM32
#define CONFIG_SYS_TEXT_BASE		0x60200000
#define CONFIG_SYS_INIT_SP_ADDR		0x60400000
#define CONFIG_SYS_LOAD_ADDR		0x61800800
#define CONFIG_SPL_TEXT_BASE		0x60000000

#define CONFIG_TPL_STACK		0x10081fff
#define CONFIG_TPL_TEXT_BASE		0x10081000

#define CONFIG_ROCKCHIP_MAX_INIT_SIZE	(4 << 10)
#define CONFIG_ROCKCHIP_CHIP_TAG	"RK30"
#define CONFIG_ROCKUSB_G_DNL_PID	0x301A

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

#define CONFIG_SYS_SDRAM_BASE		0x60000000
#define SDRAM_BANK_SIZE			(512UL << 20UL)
#define SDRAM_MAX_SIZE			0x80000000

#define CONFIG_SPI_FLASH_GIGADEVICE

#ifndef CONFIG_SPL_BUILD
/* usb otg */

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_CMD_USB_MASS_STORAGE

/* memory <= 128M board */
#define ENV_MEM_LAYOUT_SETTINGS1 \
	"scriptaddr1=0x60000000\0" \
	"pxefile_addr1_r=0x60100000\0" \
	"fdt_addr1_r=0x61f00000\0" \
	"kernel_addr1_r=0x62000000\0" \
	"ramdisk_addr1_r=0x64000000\0"

#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x60000000\0" \
	"pxefile_addr_r=0x60100000\0" \
	"fdt_addr_r=0x68300000\0" \
	"kernel_addr_r=0x62000000\0" \
	"ramdisk_addr_r=0x6a200000\0"

#include <config_distro_bootcmd.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	"partitions=" PARTS_DEFAULT \
	ENV_MEM_LAYOUT_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS1 \
	RKIMG_DET_BOOTDEV \
	BOOTENV
#endif

#define CONFIG_PREBOOT

#endif
