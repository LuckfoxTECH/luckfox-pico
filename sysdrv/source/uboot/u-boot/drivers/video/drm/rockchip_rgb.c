/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm/of_access.h>
#include <errno.h>
#include <syscon.h>
#include <regmap.h>
#include <dm/device.h>
#include <dm/read.h>
#include <dm/pinctrl.h>
#include <linux/media-bus-format.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"

#define HIWORD_UPDATE(v, h, l)		(((v) << (l)) | (GENMASK(h, l) << 16))

#define PX30_GRF_PD_VO_CON1		0x0438
#define PX30_RGB_DATA_SYNC_BYPASS(v)	HIWORD_UPDATE(v, 3, 3)
#define PX30_RGB_VOP_SEL(v)		HIWORD_UPDATE(v, 2, 2)

#define RK1808_GRF_PD_VO_CON1		0x0444
#define RK1808_RGB_DATA_SYNC_BYPASS(v)	HIWORD_UPDATE(v, 3, 3)

#define RV1106_VENC_GRF_VOP_IO_WRAPPER	0x1000c
#define RV1106_IO_BYPASS_SEL(v)		HIWORD_UPDATE(v, 0, 1)
#define RV1106_VOGRF_VOP_PIPE_BYPASS	0x60034
#define RV1106_VOP_PIPE_BYPASS(v)	HIWORD_UPDATE(v, 0, 1)

#define RV1126_GRF_IOFUNC_CON3          0x1026c
#define RV1126_LCDC_IO_BYPASS(v)        HIWORD_UPDATE(v, 0, 0)

#define RK3288_GRF_SOC_CON6		0x025c
#define RK3288_LVDS_LCDC_SEL(v)		HIWORD_UPDATE(v,  3,  3)
#define RK3288_GRF_SOC_CON7		0x0260
#define RK3288_LVDS_PWRDWN(v)		HIWORD_UPDATE(v, 15, 15)
#define RK3288_LVDS_CON_ENABLE_2(v)	HIWORD_UPDATE(v, 12, 12)
#define RK3288_LVDS_CON_ENABLE_1(v)	HIWORD_UPDATE(v, 11, 11)
#define RK3288_LVDS_CON_CLKINV(v)	HIWORD_UPDATE(v,  8,  8)
#define RK3288_LVDS_CON_TTL_EN(v)	HIWORD_UPDATE(v,  6,  6)

#define RK3368_GRF_SOC_CON15		0x043c
#define RK3368_FORCE_JETAG(v)		HIWORD_UPDATE(v,  13,  13)

#define RK3568_GRF_VO_CON1		0X0364
#define RK3568_RGB_DATA_BYPASS(v)	HIWORD_UPDATE(v, 6, 6)

struct rockchip_rgb;

struct rockchip_rgb_funcs {
	void (*prepare)(struct rockchip_rgb *rgb, int pipe);
	void (*unprepare)(struct rockchip_rgb *rgb);
};

struct rockchip_rgb {
	struct rockchip_connector connector;
	int id;
	struct udevice *dev;
	struct regmap *grf;
	bool data_sync_bypass;
	struct rockchip_phy *phy;
	const struct rockchip_rgb_funcs *funcs;
};

static int rockchip_rgb_connector_prepare(struct rockchip_connector *conn,
					  struct display_state *state)
{
	struct rockchip_rgb *rgb = dev_get_priv(conn->dev);
	struct crtc_state *crtc_state = &state->crtc_state;
	int pipe = crtc_state->crtc_id;
	int ret;

	pinctrl_select_state(rgb->dev, "default");

	if (rgb->funcs && rgb->funcs->prepare)
		rgb->funcs->prepare(rgb, pipe);

	if (rgb->phy) {
		ret = rockchip_phy_set_mode(rgb->phy, PHY_MODE_VIDEO_TTL);
		if (ret) {
			dev_err(rgb->dev, "failed to set phy mode: %d\n", ret);
			return ret;
		}

		rockchip_phy_power_on(rgb->phy);
	}

	return 0;
}

static void rockchip_rgb_connector_unprepare(struct rockchip_connector *conn,
					     struct display_state *state)
{
	struct rockchip_rgb *rgb = dev_get_priv(conn->dev);

	if (rgb->phy)
		rockchip_phy_power_off(rgb->phy);

	if (rgb->funcs && rgb->funcs->unprepare)
		rgb->funcs->unprepare(rgb);

	pinctrl_select_state(rgb->dev, "sleep");
}

static int rockchip_rgb_connector_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct rockchip_rgb *rgb = dev_get_priv(conn->dev);
	struct connector_state *conn_state = &state->conn_state;

	rgb->phy = conn->phy;

	conn_state->color_space = V4L2_COLORSPACE_DEFAULT;
	conn_state->disp_info  = rockchip_get_disp_info(conn_state->type, rgb->id);

	switch (conn_state->bus_format) {
	case MEDIA_BUS_FMT_RGB666_1X18:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_P666;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_RGB565_1X16:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_P565;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_SRGB888_3X8:
	case MEDIA_BUS_FMT_SBGR888_3X8:
	case MEDIA_BUS_FMT_SRBG888_3X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S888;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_SRGB888_DUMMY_4X8:
	case MEDIA_BUS_FMT_SBGR888_DUMMY_4X8:
	case MEDIA_BUS_FMT_SRBG888_DUMMY_4X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S888_DUMMY;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_BT656;
		conn_state->output_if = VOP_OUTPUT_IF_BT656;
		break;
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_BT1120;
		conn_state->output_if = VOP_OUTPUT_IF_BT1120;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_RGB666_1X24_CPADHI:
	default:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_P888;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	}

	return 0;
}

static const struct rockchip_connector_funcs rockchip_rgb_connector_funcs = {
	.init = rockchip_rgb_connector_init,
	.prepare = rockchip_rgb_connector_prepare,
	.unprepare = rockchip_rgb_connector_unprepare,
};

static int rockchip_rgb_probe(struct udevice *dev)
{
	struct rockchip_rgb *rgb = dev_get_priv(dev);

	rgb->dev = dev;
	rgb->funcs = (const struct rockchip_rgb_funcs *)dev_get_driver_data(dev);
	rgb->grf = syscon_get_regmap(dev_get_parent(dev));
	rgb->data_sync_bypass = dev_read_bool(dev, "rockchip,data-sync-bypass");
	rgb->id = of_alias_get_id(ofnode_to_np(dev->node), "rgb");
	if (rgb->id < 0)
		rgb->id = 0;

	rockchip_connector_bind(&rgb->connector, dev, rgb->id, &rockchip_rgb_connector_funcs,
				NULL, DRM_MODE_CONNECTOR_LVDS);

	return 0;
}

static void rv1106_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RV1106_VENC_GRF_VOP_IO_WRAPPER,
		     RV1106_IO_BYPASS_SEL(rgb->data_sync_bypass ? 0x3 : 0x0));
	regmap_write(rgb->grf, RV1106_VOGRF_VOP_PIPE_BYPASS,
		     RV1106_VOP_PIPE_BYPASS(rgb->data_sync_bypass ? 0x3 : 0x0));
}

static const struct rockchip_rgb_funcs rv1106_rgb_funcs = {
	.prepare = rv1106_rgb_prepare,
};

static void rv1126_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RV1126_GRF_IOFUNC_CON3,
		     RV1126_LCDC_IO_BYPASS(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs rv1126_rgb_funcs = {
	.prepare = rv1126_rgb_prepare,
};

static void px30_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, PX30_GRF_PD_VO_CON1, PX30_RGB_VOP_SEL(pipe) |
		     PX30_RGB_DATA_SYNC_BYPASS(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs px30_rgb_funcs = {
	.prepare = px30_rgb_prepare,
};

static void rk1808_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK1808_GRF_PD_VO_CON1,
		     RK1808_RGB_DATA_SYNC_BYPASS(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs rk1808_rgb_funcs = {
	.prepare = rk1808_rgb_prepare,
};

static void rk3288_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK3288_GRF_SOC_CON6, RK3288_LVDS_LCDC_SEL(pipe));
	regmap_write(rgb->grf, RK3288_GRF_SOC_CON7,
		     RK3288_LVDS_PWRDWN(0) | RK3288_LVDS_CON_ENABLE_2(1) |
		     RK3288_LVDS_CON_ENABLE_1(1) | RK3288_LVDS_CON_CLKINV(0) |
		     RK3288_LVDS_CON_TTL_EN(1));
}

static void rk3288_rgb_unprepare(struct rockchip_rgb *rgb)
{
	regmap_write(rgb->grf, RK3288_GRF_SOC_CON7,
		     RK3288_LVDS_PWRDWN(1) | RK3288_LVDS_CON_ENABLE_2(0) |
		     RK3288_LVDS_CON_ENABLE_1(0) | RK3288_LVDS_CON_TTL_EN(0));
}

static const struct rockchip_rgb_funcs rk3288_rgb_funcs = {
	.prepare = rk3288_rgb_prepare,
	.unprepare = rk3288_rgb_unprepare,
};

static void rk3368_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK3368_GRF_SOC_CON15, RK3368_FORCE_JETAG(0));
}

static const struct rockchip_rgb_funcs rk3368_rgb_funcs = {
	.prepare = rk3368_rgb_prepare,
};

static void rk3568_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK3568_GRF_VO_CON1, RK3568_RGB_DATA_BYPASS(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs rk3568_rgb_funcs = {
	.prepare = rk3568_rgb_prepare,
};

static const struct udevice_id rockchip_rgb_ids[] = {
	{
		.compatible = "rockchip,px30-rgb",
		.data = (ulong)&px30_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk1808-rgb",
		.data = (ulong)&rk1808_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk3066-rgb",
	},
	{
		.compatible = "rockchip,rk3128-rgb",
	},
	{
		.compatible = "rockchip,rk3288-rgb",
		.data = (ulong)&rk3288_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk3308-rgb",
	},
	{
		.compatible = "rockchip,rk3368-rgb",
		.data = (ulong)&rk3368_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk3568-rgb",
		.data = (ulong)&rk3568_rgb_funcs,
	},
	{
		.compatible = "rockchip,rv1106-rgb",
		.data = (ulong)&rv1106_rgb_funcs,
	},
	{
		.compatible = "rockchip,rv1108-rgb",
	},
	{
		.compatible = "rockchip,rv1126-rgb",
		.data = (ulong)&rv1126_rgb_funcs,
	},
	{}
};

U_BOOT_DRIVER(rockchip_rgb) = {
	.name = "rockchip_rgb",
	.id = UCLASS_DISPLAY,
	.of_match = rockchip_rgb_ids,
	.probe = rockchip_rgb_probe,
	.priv_auto_alloc_size = sizeof(struct rockchip_rgb),
};
