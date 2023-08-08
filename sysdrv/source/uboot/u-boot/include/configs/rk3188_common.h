/*
 * (C) Copyright 2015 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_RK3188_COMMON_H
#define __CONFIG_RK3188_COMMON_H

#define CONFIG_SYS_CACHELINE_SIZE	64

#include <asm/arch/hardware.h>
#include "rockchip-common.h"

#define CONFIG_SKIP_LOWLEVEL_INIT_ONLY
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		1024

#define CONFIG_SYS_TIMER_COUNTS_DOWN
/* RK3188 do not have a ARMv7 ARCH timer */
#undef CONFIG_SYS_ARCH_TIMER

#ifdef CONFIG_SPL_ROCKCHIP_BACK_TO_BROM
/* Bootrom will load u-boot binary to 0x60000000 once return from SPL */
#define CONFIG_SYS_TEXT_BASE		0x60000000
#else
#define CONFIG_SYS_TEXT_BASE		0x60200000
#endif
#define CONFIG_SYS_INIT_SP_ADDR		0x60400000
#define CONFIG_SYS_LOAD_ADDR		0x60800800

#define CONFIG_ROCKCHIP_MAX_INIT_SIZE	(0x8000 - 0x800)
#define CONFIG_ROCKCHIP_CHIP_TAG	"RK31"
#define CONFIG_ROCKUSB_G_DNL_PID	0x310B

#define CONFIG_SPL_TEXT_BASE		0x10080800
/* spl size 32kb sram - 2kb bootrom */
#define CONFIG_SPL_MAX_SIZE		(0x8000 - 0x800)
#define CONFIG_SPL_FRAMEWORK		1
#define CONFIG_ROCKCHIP_SERIAL		1

#define CONFIG_SPL_STACK		0x10087fff

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

#define CONFIG_SYS_SDRAM_BASE		0x60000000
#define SDRAM_BANK_SIZE			(2UL << 30)
#define SDRAM_MAX_SIZE			0x80000000

#ifndef CONFIG_SPL_BUILD

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE

/* usb otg */
#define CONFIG_ROCKCHIP_USB2_PHY

/* usb host support */
#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x60000000\0" \
	"pxefile_addr_r=0x60100000\0" \
	"fdt_addr_r=0x68300000\0" \
	"kernel_addr_r=0x62000000\0" \
	"ramdisk_addr_r=0x6a200000\0"

#include <config_distro_bootcmd.h>

/* Linux fails to load the fdt if it's loaded above 256M on a Rock board,
 * so limit the fdt reallocation to that */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdt_high=0x6fffffff\0" \
	"initrd_high=0x6fffffff\0" \
	"partitions=" PARTS_DEFAULT \
	ENV_MEM_LAYOUT_SETTINGS \
	ROCKCHIP_DEVICE_SETTINGS \
	BOOTENV

#endif /* CONFIG_SPL_BUILD */

#define CONFIG_PREBOOT

#endif
