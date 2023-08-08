// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <linux/media-bus-format.h>
#include <max96752f.h>
#include <video_bridge.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

static void max96752f_bridge_pre_enable(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	struct connector_state *conn_state = &bridge->state->conn_state;
	bool oldi_format, oldi_4th_lane;

	max96752f_init(dev->parent);

	switch (conn_state->bus_format) {
	case MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA:
		oldi_4th_lane = false;
		oldi_format = 0x0;
		break;
	case MEDIA_BUS_FMT_RGB666_1X7X3_SPWG:
		oldi_4th_lane = true;
		oldi_format = 0x0;
		break;
	case MEDIA_BUS_FMT_RGB888_1X7X4_SPWG:
		oldi_4th_lane = false;
		oldi_format = 0x1;
		break;
	default:
		oldi_4th_lane = false;
		oldi_format = 0x1;
		break;
	}

	dm_i2c_reg_clrset(dev->parent, 0x01ce, OLDI_FORMAT | OLDI_4TH_LANE,
			  FIELD_PREP(OLDI_4TH_LANE, oldi_4th_lane) |
			  FIELD_PREP(OLDI_FORMAT, oldi_format));
}

static const struct rockchip_bridge_funcs max96752f_bridge_funcs = {
	.pre_enable = max96752f_bridge_pre_enable,
};

static int max96752f_bridge_probe(struct udevice *dev)
{
	struct rockchip_bridge *bridge;

	bridge = calloc(1, sizeof(*bridge));
	if (!bridge)
		return -ENOMEM;

	dev->driver_data = (ulong)bridge;
	bridge->dev = dev;
	bridge->funcs = &max96752f_bridge_funcs;

	return 0;
}

static const struct udevice_id max96752f_bridge_of_match[] = {
	{ .compatible = "maxim,max96752f-bridge", },
	{ }
};

U_BOOT_DRIVER(max96752f_bridge) = {
	.name = "max96752f_bridge",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = max96752f_bridge_of_match,
	.probe = max96752f_bridge_probe,
};
