/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>

static const struct udevice_id px30_syscon_ids[] = {
	{ .compatible = "rockchip,px30-pmu", .data = ROCKCHIP_SYSCON_PMU },
	{ .compatible = "rockchip,px30-pmugrf", .data = ROCKCHIP_SYSCON_PMUGRF },
	{ .compatible = "rockchip,px30-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ }
};

U_BOOT_DRIVER(syscon_px30) = {
	.name = "px30_syscon",
	.id = UCLASS_SYSCON,
	.of_match = px30_syscon_ids,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind = dm_scan_fdt_dev,
#endif
};
