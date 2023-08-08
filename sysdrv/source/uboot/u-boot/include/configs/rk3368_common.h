/*
 * Copyright (c) 2016 Andreas Färber
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_RK3368_COMMON_H
#define __CONFIG_RK3368_COMMON_H

#include "rockchip-common.h"

#define CONFIG_SYS_CACHELINE_SIZE	64

#include <asm/arch/hardware.h>
#include <linux/sizes.h>

#define CONFIG_SYS_SDRAM_BASE		0
#define SDRAM_MAX_SIZE			0xfe000000
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_SYS_TEXT_BASE		0x00200000
#define CONFIG_SYS_INIT_SP_ADDR		0x00400000
#define CONFIG_SYS_LOAD_ADDR		0x00280000
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)	/* 64M */

#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE            0x00000000
#define CONFIG_SPL_MAX_SIZE             0x40000
#define CONFIG_SPL_BSS_START_ADDR       0x400000
#define CONFIG_SPL_BSS_MAX_SIZE         0x20000
#define CONFIG_SPL_STACK		0x00188000

#define GICD_BASE			0xffB71000
#define GICC_BASE			0xffB72000

#define CONFIG_ROCKUSB_G_DNL_PID	0x330A

#define CONFIG_BOUNCE_BUFFER
#define COUNTER_FREQUENCY              24000000
#define CONFIG_PREBOOT

#ifndef CONFIG_SPL_BUILD

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE

#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x00500000\0" \
	"pxefile_addr_r=0x00600000\0" \
	"fdt_addr_r=0x08300000\0" \
	"kernel_addr_r=0x00280000\0" \
	"ramdisk_addr_r=0x0a200000\0"

#include <config_distro_bootcmd.h>

#ifndef ROCKCHIP_DEVICE_SETTINGS
#define ROCKCHIP_DEVICE_SETTINGS
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS	\
	"partitions=" PARTS_DEFAULT \
	RKIMG_DET_BOOTDEV \
	ROCKCHIP_DEVICE_SETTINGS \
	BOOTENV

#endif

#endif
