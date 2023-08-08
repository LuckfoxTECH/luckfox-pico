/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>

static const struct udevice_id rv1106_syscon_ids[] = {
	{ .compatible = "rockchip,rv1106-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ .compatible = "rockchip,rv1106-pmugrf", .data = ROCKCHIP_SYSCON_PMUGRF },
	{ }
};

U_BOOT_DRIVER(syscon_rv1106) = {
	.name = "rv1106_syscon",
	.id = UCLASS_SYSCON,
	.of_match = rv1106_syscon_ids,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind = dm_scan_fdt_dev,
#endif
};
