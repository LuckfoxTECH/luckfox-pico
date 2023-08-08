/*
 * (C) Copyright 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __CONFIG_RV1108_COMMON_H
#define __CONFIG_RV1108_COMMON_H

#include <asm/arch/hardware.h>
#include "rockchip-common.h"

#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_SYS_SDRAM_BASE		0x60000000
#define SDRAM_MAX_SIZE			0x80000000
#define CONFIG_SYS_TEXT_BASE		0x60000000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_TEXT_BASE + 0x200000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x2000000)

/* SPL support */
#define CONFIG_SPL_STACK		0x10080700
#define CONFIG_SPL_TEXT_BASE		0x10080800
#define CONFIG_SPL_MAX_SIZE		0x4000

/* BSS setup */
#define CONFIG_SPL_BSS_MAX_SIZE		0x100

#define CONFIG_ROCKUSB_G_DNL_PID	0x110A

#define CONFIG_BOUNCE_BUFFER

/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE

/* rockchip ohci host driver */
#define CONFIG_USB_OHCI_NEW
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	1

#ifndef CONFIG_SPL_BUILD

#define ENV_MEM_LAYOUT_SETTINGS \
	"scriptaddr=0x60000000\0" \
	"fdt_addr_r=0x61f00000\0" \
	"kernel_addr_r=0x62000000\0" \
	"ramdisk_addr_r=0x64000000\0"

#include <config_distro_bootcmd.h>
#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS \
	"partitions=" PARTS_DEFAULT \
	RKIMG_DET_BOOTDEV \
	BOOTENV
#endif

#endif
