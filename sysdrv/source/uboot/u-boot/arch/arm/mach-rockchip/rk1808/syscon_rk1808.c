// SPDX-License-Identifier:     GPL-2.0+
/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 *
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>

static const struct udevice_id rk1808_syscon_ids[] = {
	{ .compatible = "rockchip,rk1808-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ .compatible = "rockchip,rk1808-pmugrf", .data = ROCKCHIP_SYSCON_PMUGRF },
	{ }
};

U_BOOT_DRIVER(syscon_rk1808) = {
	.name = "rk1808_syscon",
	.id = UCLASS_SYSCON,
#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind = dm_scan_fdt_dev,
#endif
	.of_match = rk1808_syscon_ids,
};
