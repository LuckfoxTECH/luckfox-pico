// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2008-2018 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <errno.h>
#include <i2c.h>
#include <video_bridge.h>
#include <linux/media-bus-format.h>

#include "rockchip_display.h"
#include "rockchip_bridge.h"
#include "rockchip_panel.h"
#include "rockchip_connector.h"

#include "rk618.h"

enum {
	LVDS_8BIT_MODE_FORMAT_1,
	LVDS_8BIT_MODE_FORMAT_2,
	LVDS_8BIT_MODE_FORMAT_3,
	LVDS_6BIT_MODE,
};

struct rk618_lvds_priv {
	struct udevice *dev;
	struct rk618 *parent;
	bool dual_channel;
};

static int lvds_write(struct rk618_lvds_priv *priv, u16 reg, u32 val)
{
	return rk618_i2c_write(priv->parent, reg, val);
}

static void rk618_lvds_bridge_enable(struct rockchip_bridge *bridge)
{
	struct rk618_lvds_priv *priv = dev_get_priv(bridge->dev);
	struct rockchip_panel *panel = bridge->conn->panel;
	u32 value, format;

	rk618_frc_dclk_invert(priv->parent);

	switch (panel->bus_format) {
	case MEDIA_BUS_FMT_RGB666_1X7X3_JEIDA:	/* jeida-18 */
		format = LVDS_6BIT_MODE;
		break;
	case MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA:	/* jeida-24 */
		format = LVDS_8BIT_MODE_FORMAT_2;
		break;
	case MEDIA_BUS_FMT_RGB666_1X7X3_SPWG:	/* vesa-18 */
		format = LVDS_8BIT_MODE_FORMAT_3;
		break;
	case MEDIA_BUS_FMT_RGB888_1X7X4_SPWG:	/* vesa-24 */
	default:
		format = LVDS_8BIT_MODE_FORMAT_1;
		break;
	}

	value = LVDS_CON_CHA0TTL_DISABLE | LVDS_CON_CHA1TTL_DISABLE |
		LVDS_CON_CHA0_POWER_UP | LVDS_CON_CBG_POWER_UP |
		LVDS_CON_PLL_POWER_UP | LVDS_CON_SELECT(format);

	if (priv->dual_channel)
		value |= LVDS_CON_CHA1_POWER_UP | LVDS_DCLK_INV |
			 LVDS_CON_CHASEL_DOUBLE_CHANNEL;
	else
		value |= LVDS_CON_CHA1_POWER_DOWN |
			 LVDS_CON_CHASEL_SINGLE_CHANNEL;

	lvds_write(priv, RK618_LVDS_CON, value);
}

static void rk618_lvds_bridge_disable(struct rockchip_bridge *bridge)
{
	struct rk618_lvds_priv *priv = dev_get_priv(bridge->dev);

	lvds_write(priv, RK618_LVDS_CON,
		   LVDS_CON_CHA0_POWER_DOWN | LVDS_CON_CHA1_POWER_DOWN |
		   LVDS_CON_CBG_POWER_DOWN | LVDS_CON_PLL_POWER_DOWN);
}

static const struct rockchip_bridge_funcs rk618_lvds_bridge_funcs = {
	.enable = rk618_lvds_bridge_enable,
	.disable = rk618_lvds_bridge_disable,
};

static int rk618_lvds_probe(struct udevice *dev)
{
	struct rk618_lvds_priv *priv = dev_get_priv(dev);
	struct rockchip_bridge *bridge =
		(struct rockchip_bridge *)dev_get_driver_data(dev);
	int ret;

	priv->dev = dev;
	priv->parent = dev_get_priv(dev->parent);
	priv->dual_channel = dev_read_bool(dev, "dual-channel");

	ret = device_probe(dev->parent);
	if (ret)
		return ret;

	bridge->dev = dev;

	return 0;
}

static struct rockchip_bridge rk618_lvds_driver_data = {
	.funcs = &rk618_lvds_bridge_funcs,
};

static const struct udevice_id rk618_lvds_ids[] = {
	{
		.compatible = "rockchip,rk618-lvds",
		.data = (ulong)&rk618_lvds_driver_data,
	},
	{ }
};

U_BOOT_DRIVER(rk618_lvds) = {
	.name = "rk618_lvds",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = rk618_lvds_ids,
	.probe = rk618_lvds_probe,
	.priv_auto_alloc_size = sizeof(struct rk618_lvds_priv),
};
