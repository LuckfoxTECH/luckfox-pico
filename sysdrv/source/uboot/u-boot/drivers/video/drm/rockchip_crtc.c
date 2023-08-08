/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <clk.h>
#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <asm/unaligned.h>
#include <linux/list.h>
#include <dm/device.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <dm/lists.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"

static const struct udevice_id rockchip_vp_ids[] = {
	{ .compatible = "rockchip-vp" },
	{ }
};

U_BOOT_DRIVER(rockchip_vp) = {
	.name		= "rockchip-vp",
	.id		= UCLASS_VIDEO_CRTC,
	.of_match	= rockchip_vp_ids,
};

static const struct rockchip_crtc rk3036_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3036_vop,
};

static const struct rockchip_crtc rv1106_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rv1106_vop,
};

static const struct rockchip_crtc rv1108_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rv1108_vop,
};

static const struct rockchip_crtc rv1126_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rv1126_vop,
};

static const struct rockchip_crtc px30_vop_lit_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &px30_vop_lit,
};

static const struct rockchip_crtc px30_vop_big_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &px30_vop_big,
};

static const struct rockchip_crtc rk3308_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3308_vop,
};

static const struct rockchip_crtc rk1808_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk1808_vop,
};

static const struct rockchip_crtc rk3288_vop_big_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3288_vop_big,
};

static const struct rockchip_crtc rk3288_vop_lit_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3288_vop_lit,
};

static const struct rockchip_crtc rk3368_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3368_vop,
};

static const struct rockchip_crtc rk3366_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3366_vop,
};

static const struct rockchip_crtc rk3399_vop_big_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3399_vop_big,
};

static const struct rockchip_crtc rk3399_vop_lit_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3399_vop_lit,
};

static const struct rockchip_crtc rk322x_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk322x_vop,
};

static const struct rockchip_crtc rk3328_vop_data = {
	.funcs = &rockchip_vop_funcs,
	.data = &rk3328_vop,
};

static const struct rockchip_crtc rk3528_vop_data = {
	.funcs = &rockchip_vop2_funcs,
	.data = &rk3528_vop,
};

static const struct rockchip_crtc rk3568_vop_data = {
	.funcs = &rockchip_vop2_funcs,
	.data = &rk3568_vop,
};

static const struct rockchip_crtc rk3588_vop_data = {
	.funcs = &rockchip_vop2_funcs,
	.data = &rk3588_vop,
};

static const struct udevice_id rockchip_vop_ids[] = {
	{
		.compatible = "rockchip,rk3036-vop",
		.data = (ulong)&rk3036_vop_data,
	}, {
		.compatible = "rockchip,rv1108-vop",
		.data = (ulong)&rv1108_vop_data,
	}, {
		.compatible = "rockchip,rv1106-vop",
		.data = (ulong)&rv1106_vop_data,
	}, {
		.compatible = "rockchip,rv1126-vop",
		.data = (ulong)&rv1126_vop_data,
	}, {
		.compatible = "rockchip,rk3126-vop",
		.data = (ulong)&rk3036_vop_data,
	}, {
		.compatible = "rockchip,px30-vop-lit",
		.data = (ulong)&px30_vop_lit_data,
	}, {
		.compatible = "rockchip,px30-vop-big",
		.data = (ulong)&px30_vop_big_data,
	}, {
		.compatible = "rockchip,rk3308-vop",
		.data = (ulong)&rk3308_vop_data,
	}, {
		.compatible = "rockchip,rk1808-vop-lit",
		.data = (ulong)&rk1808_vop_data,
	}, {
		.compatible = "rockchip,rk3288-vop-big",
		.data = (ulong)&rk3288_vop_big_data,
	}, {
		.compatible = "rockchip,rk3288-vop-lit",
		.data = (ulong)&rk3288_vop_lit_data,
	}, {
		.compatible = "rockchip,rk3368-vop",
		.data = (ulong)&rk3368_vop_data,
	}, {
		.compatible = "rockchip,rk3366-vop",
		.data = (ulong)&rk3366_vop_data,
	}, {
		.compatible = "rockchip,rk3399-vop-big",
		.data = (ulong)&rk3399_vop_big_data,
	}, {
		.compatible = "rockchip,rk3399-vop-lit",
		.data = (ulong)&rk3399_vop_lit_data,
	}, {
		.compatible = "rockchip,rk322x-vop",
		.data = (ulong)&rk322x_vop_data,
	}, {
		.compatible = "rockchip,rk3328-vop",
		.data = (ulong)&rk3328_vop_data,
	}, {
		.compatible = "rockchip,rk3528-vop",
		.data = (ulong)&rk3528_vop_data,
	}, {
		.compatible = "rockchip,rk3568-vop",
		.data = (ulong)&rk3568_vop_data,
	}, {
		.compatible = "rockchip,rk3588-vop",
		.data = (ulong)&rk3588_vop_data,
	}, { }
};

static int rockchip_vop_probe(struct udevice *dev)
{
	struct udevice *child;
	int ret;

	for (device_find_first_child(dev, &child);
	     child;
	     device_find_next_child(&child)) {
		ret = device_probe(child);
		if (ret)
			return ret;

		ret = clk_set_defaults(child);
		if (ret)
			return ret;
	}

	return 0;
}

static int rockchip_vop_bind(struct udevice *dev)
{
	ofnode ports, node;
	int ret;

	ports = dev_read_subnode(dev, "ports");
	if (!ofnode_valid(ports))
		return 0;

	ofnode_for_each_subnode(node, ports) {
		const char *name = ofnode_get_name(node);

		ret = device_bind_driver_to_node(dev, "rockchip-vp", name,
						 node, NULL);
		if (ret) {
			dev_err(dev, "unable to bind vp device node: %d\n", ret);
			return ret;
		}
	}

	return 0;
}

U_BOOT_DRIVER(rockchip_vop) = {
	.name	= "rockchip-vop",
	.id	= UCLASS_VIDEO_CRTC,
	.of_match = rockchip_vop_ids,
	.bind	= rockchip_vop_bind,
	.probe	= rockchip_vop_probe,
};

UCLASS_DRIVER(rockchip_crtc) = {
	.id		= UCLASS_VIDEO_CRTC,
	.name		= "CRTC",
};
