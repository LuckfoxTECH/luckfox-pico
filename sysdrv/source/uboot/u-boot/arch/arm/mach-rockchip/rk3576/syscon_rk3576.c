/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>

static const struct udevice_id rk3576_syscon_ids[] = {
	{ .compatible = "rockchip,rk3576-sys-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ .compatible = "rockchip,rk3576-ioc-grf", .data = ROCKCHIP_SYSCON_IOC },
	{ }
};

U_BOOT_DRIVER(syscon_rk3576) = {
	.name = "rk3576_syscon",
	.id = UCLASS_SYSCON,
	.of_match = rk3576_syscon_ids,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind = dm_scan_fdt_dev,
#endif
};
