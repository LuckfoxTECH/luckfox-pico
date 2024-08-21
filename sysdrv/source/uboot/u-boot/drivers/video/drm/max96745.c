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
#include <linux/iopoll.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

static bool max96745_bridge_link_locked(struct udevice *dev)
{
	int ret;

	ret = dm_i2c_reg_read(dev->parent, 0x002a);
	if (ret < 0)
		return false;

	if (!FIELD_GET(LINK_LOCKED, ret))
		return false;

	return true;
}

static bool max96745_bridge_detect(struct rockchip_bridge *bridge)
{
	return max96745_bridge_link_locked(bridge->dev);
}

static void max96745_bridge_enable(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	struct drm_display_mode *mode = &bridge->state->conn_state.mode;
	u8 cxtp, tx_rate;
	int ret;

	ret = dm_i2c_reg_read(dev->parent, 0x0011);
	if (ret < 0)
		return;

	cxtp = FIELD_GET(CXTP_A, ret);

	ret = dm_i2c_reg_read(dev->parent, 0x0028);
	if (ret < 0)
		return;

	tx_rate = FIELD_GET(TX_RATE, ret);

	if (!cxtp && mode->clock > 95000 && tx_rate == 1) {
		ret = dm_i2c_reg_clrset(dev->parent, 0x0028, TX_RATE,
					FIELD_PREP(TX_RATE, 2));
		if (ret < 0)
			return;

		ret = dm_i2c_reg_clrset(dev->parent, 0x0029, RESET_ONESHOT,
					FIELD_PREP(RESET_ONESHOT, 1));
		if (ret < 0)
			return;

		if (readx_poll_timeout(max96745_bridge_link_locked, dev, ret,
				       ret, 200000))
			dev_err(dev, "%s: GMSL link not locked\n", __func__);
	}
}

static void max96745_bridge_post_disable(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	u8 cxtp, tx_rate;
	int ret;

	ret = dm_i2c_reg_read(dev->parent, 0x0011);
	if (ret < 0)
		return;

	cxtp = FIELD_GET(CXTP_A, ret);

	ret = dm_i2c_reg_read(dev->parent, 0x0028);
	if (ret < 0)
		return;

	tx_rate = FIELD_GET(TX_RATE, ret);

	if (!cxtp && tx_rate == 2) {
		ret = dm_i2c_reg_clrset(dev->parent, 0x0028, TX_RATE,
					FIELD_PREP(TX_RATE, 1));
		if (ret < 0)
			return;

		ret = dm_i2c_reg_clrset(dev->parent, 0x0029, RESET_ONESHOT,
					FIELD_PREP(RESET_ONESHOT, 1));
		if (ret < 0)
			return;

		if (readx_poll_timeout(max96745_bridge_link_locked, dev, ret,
				       ret, 200000))
			dev_err(dev, "%s: GMSL link not locked\n", __func__);
	}
}

static const struct rockchip_bridge_funcs max96745_bridge_funcs = {
	.detect		= max96745_bridge_detect,
	.enable		= max96745_bridge_enable,
	.post_disable	= max96745_bridge_post_disable,
};

static int max96745_bridge_probe(struct udevice *dev)
{
	struct rockchip_bridge *bridge;

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
};
