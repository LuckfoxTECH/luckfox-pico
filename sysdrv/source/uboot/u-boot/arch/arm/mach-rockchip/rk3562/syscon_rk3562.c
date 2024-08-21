/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>

static const struct udevice_id rk3562_syscon_ids[] = {
	{ .compatible = "rockchip,rk3562-sys-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ .compatible = "rockchip,rk3562-pmu-grf", .data = ROCKCHIP_SYSCON_PMUGRF },
	{ .compatible = "rockchip,rk3562-ioc-grf", .data = ROCKCHIP_SYSCON_IOC },
	{ }
};

U_BOOT_DRIVER(syscon_rk3562) = {
	.name = "rk3562_syscon",
	.id = UCLASS_SYSCON,
	.of_match = rk3562_syscon_ids,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind = dm_scan_fdt_dev,
#endif
};
