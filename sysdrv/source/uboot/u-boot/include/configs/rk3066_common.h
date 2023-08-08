/*
 * Copyright (c) 2017 Paweł Jarosz <paweljarosz3691@gmail.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_RK3066_COMMON_H
#define __CONFIG_RK3066_COMMON_H

#include <asm/arch/hardware.h>
#include "rockchip-common.h"

#define CONFIG_SKIP_LOWLEVEL_INIT_ONLY
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		256

#define CONFIG_SYS_SDRAM_BASE		0x60000000
#define SDRAM_BANK_SIZE			(1024UL << 20UL)
#define SDRAM_MAX_SIZE			0x80000000

#define CONFIG_SYS_TIMER_COUNTS_DOWN

#define CONFIG_SYS_TEXT_BASE		0x60408000
#define CONFIG_SYS_INIT_SP_ADDR		0x78000000
#define CONFIG_SYS_LOAD_ADDR		0x70800800

#define CONFIG_BOUNCE_BUFFER
#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SYS_MAX_NAND_DEVICE	8

#define CONFIG_ROCKUSB_G_DNL_PID	0x300A

#ifdef CONFIG_TPL_BUILD
#define CONFIG_SPL_TEXT_BASE		0x10080C04
#define CONFIG_SPL_STACK		0x1008FFFF
/* tpl size max 32kb - 4byte RK30 header */
#define CONFIG_SPL_MAX_SIZE		(0x8000 - 0x4)
#elif defined(CONFIG_SPL_BUILD)
/* spl size max 200k */
#define CONFIG_SPL_MAX_SIZE		0x32000
#define CONFIG_SPL_TEXT_BASE		0x60000000
#define CONFIG_SPL_STACK		0x1008FFFF
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_LOAD
#define CONFIG_SPL_NAND_ECC
#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_INIT
#define CONFIG_SPL_NAND_BBT
#define CONFIG_SPL_NAND_IDS
#define CONFIG_SPL_NAND_UTIL
#define CONFIG_SPL_NAND_RAW_ONLY
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_SIZE	0x80000
#define CONFIG_EXTRA_ENV_SETTINGS ROCKCHIP_DEVICE_SETTINGS
#endif

#include <config_distro_defaults.h>

#ifndef CONFIG_SPL_BUILD

#define CONFIG_USB_FUNCTION_MASS_STORAGE

#define MTDIDS_DEFAULT			"nand0=rockchip-nand.0"

#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x60000000\0" \
	"pxefile_addr_r=0x60100000\0" \
	"fdt_addr_r=0x68300000\0" \
	"kernel_addr_r=0x62000000\0" \
	"ramdisk_addr_r=0x6a200000\0"

#include <config_distro_bootcmd.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdt_high=0x6fffffff\0" \
	"initrd_high=0x6fffffff\0" \
	"partitions=" PARTS_DEFAULT \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	ENV_MEM_LAYOUT_SETTINGS \
	ROCKCHIP_DEVICE_SETTINGS \
	BOOTENV

#endif

#define CONFIG_PREBOOT

#endif
