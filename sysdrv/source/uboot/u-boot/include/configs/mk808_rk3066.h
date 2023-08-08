/*
 * Copyright (c) 2017 Paweł Jarosz <paweljarosz3691@gmail.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H
/*
 * Using bootrom layout for rockchip-idb* and spl*. Size is nand erase size.
 */
#define MTDPARTS_DEFAULT \
		"mtdparts=rockchip-nand.0:" \
			"4M(idb)," \
			"4M(idb.backup)," \
			"4M(spl)," \
			"4M(spl.backup1)," \
			"4M(spl.backup2)," \
			"4M(spl.backup3)," \
			"4M(spl.backup4)," \
			"4M(u-boot)," \
			"4M(u-boot.backup)," \
			"4M(u-boot-env)," \
			"4M(u-boot-env.backup)," \
			"16M(kernel)," \
			"32M(initrd)," \
			"-(rootfs)"

#define DFU_ALT_INFO_NAND \
		"idb raw 0x0 0x400000;" \
		"idb.backup raw 0x400000 0x400000;" \
		"spl raw 0x800000 0x400000;" \
		"spl.backup1 raw 0xC00000 0x400000;" \
		"spl.backup2 raw 0x1000000 0x400000;" \
		"spl.backup3 raw 0x1400000 0x400000;" \
		"spl.backup4 raw 0x1800000 0x400000;" \
		"u-boot raw 0x1C00000 0x400000;" \
		"u-boot.backup raw 0x2000000 0x400000;" \
		"u-boot-env raw 0x2400000 0x400000;" \
		"u-boot-env.backup raw 0x2800000 0x400000;" \
		"kernel raw 0x2C00000 0x1000000;" \
		"initrd raw 0x3C00000 0x2000000"

#define DFU_ALT_INFO_RAM \
	"tpl.bin ram 0x10080C00 0x10000;" \
	"spl.bin ram 0x60000000 0x40000;" \
	"u-boot.bin ram 0x60408000 0x80000"

#define ROCKCHIP_DEVICE_SETTINGS \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"dfu_alt_info=" DFU_ALT_INFO_NAND "\0" \
	"dfu_alt_info_ram=" DFU_ALT_INFO_RAM "\0"

#define CONFIG_SYS_DFU_DATA_BUF_SIZE		SZ_4M
#define CONFIG_SYS_NAND_U_BOOT_OFFS		0x01c00000
#define CONFIG_SYS_NAND_U_BOOT_OFFS_REDUND	0x02000000

#include <configs/rk3066_common.h>

#endif

