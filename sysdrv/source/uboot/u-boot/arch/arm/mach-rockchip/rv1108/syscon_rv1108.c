/*
 * (C) Copyright 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>

static const struct udevice_id rv1108_syscon_ids[] = {
	{ .compatible = "rockchip,rv1108-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ .compatible = "rockchip,rv1108-usbgrf", .data = ROCKCHIP_SYSCON_USBGRF },
	{ }
};

U_BOOT_DRIVER(syscon_rv1108) = {
	.name = "rv1108_syscon",
	.id = UCLASS_SYSCON,
	.of_match = rv1108_syscon_ids,
};
