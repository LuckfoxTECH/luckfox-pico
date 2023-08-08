// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <max96745.h>
#include <video_bridge.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

struct max96745_bridge_priv {
	struct gpio_desc lock_gpio;
};

static bool max96745_bridge_detect(struct rockchip_bridge *bridge)
{
	struct max96745_bridge_priv *priv = dev_get_priv(bridge->dev);

	if (!dm_gpio_get_value(&priv->lock_gpio))
		return false;

	return true;
}

static const struct rockchip_bridge_funcs max96745_bridge_funcs = {
	.detect = max96745_bridge_detect,
};

static int max96745_bridge_probe(struct udevice *dev)
{
	struct max96745_bridge_priv *priv = dev_get_priv(dev);
	struct rockchip_bridge *bridge;
	int ret;

	ret = gpio_request_by_name(dev, "lock-gpios", 0, &priv->lock_gpio,
				   GPIOD_IS_IN);
	if (ret) {
		dev_err(dev, "failed to get lock GPIO: %d\n", ret);
		return ret;
	}

	bridge = calloc(1, sizeof(*bridge));
	if (!bridge)
		return -ENOMEM;

	dev->driver_data = (ulong)bridge;
	bridge->dev = dev;
	bridge->funcs = &max96745_bridge_funcs;

	return 0;
}

static const struct udevice_id max96745_bridge_of_match[] = {
	{ .compatible = "maxim,max96745-bridge", },
	{ }
};

U_BOOT_DRIVER(max96745_bridge) = {
	.name = "max96745_bridge",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = max96745_bridge_of_match,
	.probe = max96745_bridge_probe,
	.priv_auto_alloc_size = sizeof(struct max96745_bridge_priv),
};
