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
#include <asm/gpio.h>
#include <backlight.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"
#include "rockchip_panel.h"

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

#define RK3562_GRF_IOC_VO_IO_CON	0x10500
#define RK3562_RGB_DATA_BYPASS(v)	HIWORD_UPDATE(v, 6, 6)

#define RK3568_GRF_VO_CON1		0X0364
#define RK3568_RGB_DATA_BYPASS(v)	HIWORD_UPDATE(v, 6, 6)

#define RK3576_VCCIO_IOC_MISC_CON8	0x6420
#define RK3576_VOP_MCU_SEL(v)		HIWORD_UPDATE(v, 10, 10)

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

struct mcu_cmd_header {
	u8 data_type;
	u8 delay;
	u8 payload_length;
} __packed;

struct mcu_cmd_desc {
	struct mcu_cmd_header header;
	const u8 *payload;
};

struct mcu_cmd_seq {
	struct mcu_cmd_desc *cmds;
	unsigned int cmd_cnt;
};

struct rockchip_mcu_panel_desc {
	struct mcu_cmd_seq *init_seq;
	struct mcu_cmd_seq *exit_seq;

	struct {
		unsigned int width;
		unsigned int height;
	} size;

	struct {
		unsigned int prepare;
		unsigned int enable;
		unsigned int disable;
		unsigned int unprepare;
		unsigned int reset;
		unsigned int init;
	} delay;

	unsigned int bpc;
	u32 bus_format;
	u32 bus_flags;
	bool power_invert;
};

struct rockchip_mcu_panel {
	struct rockchip_panel base;
	struct rockchip_mcu_panel_desc *desc;
	struct udevice *power_supply;
	struct udevice *backlight;

	struct gpio_desc enable_gpio;
	struct gpio_desc reset_gpio;

	bool prepared;
	bool enabled;
};

static inline struct rockchip_mcu_panel *to_rockchip_mcu_panel(struct rockchip_panel *panel)
{
	return container_of(panel, struct rockchip_mcu_panel, base);
}

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

	conn_state->color_range = DRM_COLOR_YCBCR_FULL_RANGE;
	conn_state->color_encoding = DRM_COLOR_YCBCR_BT709;
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
	case MEDIA_BUS_FMT_RGB565_2X8_LE:
	case MEDIA_BUS_FMT_BGR565_2X8_LE:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S565;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_RGB666_3X6:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S666;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_RGB888_3X8:
	case MEDIA_BUS_FMT_BGR888_3X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S888;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_RGB888_DUMMY_4X8:
	case MEDIA_BUS_FMT_BGR888_DUMMY_4X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S888_DUMMY;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		break;
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_BT656;
		conn_state->output_if = VOP_OUTPUT_IF_BT656;
		conn_state->color_range = DRM_COLOR_YCBCR_LIMITED_RANGE;
		conn_state->color_encoding = DRM_COLOR_YCBCR_BT601;
		break;
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_BT1120;
		conn_state->output_if = VOP_OUTPUT_IF_BT1120;
		conn_state->color_range = DRM_COLOR_YCBCR_LIMITED_RANGE;
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

static int rockchip_mcu_panel_send_cmds(struct display_state *state,
					struct mcu_cmd_seq *cmds)
{
	int i;

	if (!cmds)
		return -EINVAL;

	display_send_mcu_cmd(state, MCU_SETBYPASS, 1);
	for (i = 0; i < cmds->cmd_cnt; i++) {
		struct mcu_cmd_desc *desc = &cmds->cmds[i];
		int value = 0;

		value = desc->payload[0];
		display_send_mcu_cmd(state, desc->header.data_type, value);

		if (desc->header.delay)
			mdelay(desc->header.delay);
	}
	display_send_mcu_cmd(state, MCU_SETBYPASS, 0);

	return 0;
}

static void rockchip_mcu_panel_prepare(struct rockchip_panel *panel)
{
	struct rockchip_mcu_panel *mcu_panel = to_rockchip_mcu_panel(panel);
	int ret;

	if (mcu_panel->prepared)
		return;

	if (dm_gpio_is_valid(&mcu_panel->enable_gpio))
		dm_gpio_set_value(&mcu_panel->enable_gpio, 1);

	if (mcu_panel->desc->delay.prepare)
		mdelay(mcu_panel->desc->delay.prepare);

	if (dm_gpio_is_valid(&mcu_panel->reset_gpio))
		dm_gpio_set_value(&mcu_panel->reset_gpio, 1);

	if (mcu_panel->desc->delay.reset)
		mdelay(mcu_panel->desc->delay.reset);

	if (dm_gpio_is_valid(&mcu_panel->reset_gpio))
		dm_gpio_set_value(&mcu_panel->reset_gpio, 0);

	if (mcu_panel->desc->delay.init)
		mdelay(mcu_panel->desc->delay.init);

	if (mcu_panel->desc->init_seq) {
		ret = rockchip_mcu_panel_send_cmds(panel->state, mcu_panel->desc->init_seq);
		if (ret)
			printf("failed to send mcu panel init cmds: %d\n", ret);
	}

	mcu_panel->prepared = true;
}

static void rockchip_mcu_panel_unprepare(struct rockchip_panel *panel)
{
	struct rockchip_mcu_panel *mcu_panel = to_rockchip_mcu_panel(panel);
	int ret;

	if (!mcu_panel->prepared)
		return;

	if (mcu_panel->desc->exit_seq) {
		ret = rockchip_mcu_panel_send_cmds(panel->state, mcu_panel->desc->exit_seq);
		if (ret)
			printf("failed to send mcu panel exit cmds: %d\n", ret);
	}

	if (dm_gpio_is_valid(&mcu_panel->reset_gpio))
		dm_gpio_set_value(&mcu_panel->reset_gpio, 1);

	if (dm_gpio_is_valid(&mcu_panel->enable_gpio))
		dm_gpio_set_value(&mcu_panel->enable_gpio, 0);

	if (mcu_panel->desc->delay.unprepare)
		mdelay(mcu_panel->desc->delay.unprepare);

	mcu_panel->prepared = false;
}

static void rockchip_mcu_panel_enable(struct rockchip_panel *panel)
{
	struct rockchip_mcu_panel *mcu_panel = to_rockchip_mcu_panel(panel);

	if (mcu_panel->enabled)
		return;

	if (mcu_panel->desc->delay.enable)
		mdelay(mcu_panel->desc->delay.enable);

	if (mcu_panel->backlight)
		backlight_enable(mcu_panel->backlight);

	mcu_panel->enabled = true;
}

static void rockchip_mcu_panel_disable(struct rockchip_panel *panel)
{
	struct rockchip_mcu_panel *mcu_panel = to_rockchip_mcu_panel(panel);

	if (!mcu_panel->enabled)
		return;

	if (mcu_panel->backlight)
		backlight_disable(mcu_panel->backlight);

	if (mcu_panel->desc->delay.disable)
		mdelay(mcu_panel->desc->delay.disable);

	mcu_panel->enabled = false;
}

static const struct rockchip_panel_funcs rockchip_mcu_panel_funcs = {
	.prepare = rockchip_mcu_panel_prepare,
	.unprepare = rockchip_mcu_panel_unprepare,
	.enable = rockchip_mcu_panel_enable,
	.disable = rockchip_mcu_panel_disable,
};

static int rockchip_mcu_panel_parse_cmds(const u8 *data, int length,
					 struct mcu_cmd_seq *pcmds)
{
	int len;
	const u8 *buf;
	const struct mcu_cmd_header *header;
	int i, cnt = 0;

	/* scan commands */
	cnt = 0;
	buf = data;
	len = length;
	while (len > sizeof(*header)) {
		header = (const struct mcu_cmd_header *)buf;
		buf += sizeof(*header) + header->payload_length;
		len -= sizeof(*header) + header->payload_length;
		cnt++;
	}

	pcmds->cmds = calloc(cnt, sizeof(struct mcu_cmd_desc));
	if (!pcmds->cmds)
		return -ENOMEM;

	pcmds->cmd_cnt = cnt;

	buf = data;
	len = length;
	for (i = 0; i < cnt; i++) {
		struct mcu_cmd_desc *desc = &pcmds->cmds[i];

		header = (const struct mcu_cmd_header *)buf;
		length -= sizeof(*header);
		buf += sizeof(*header);
		desc->header.data_type = header->data_type;
		desc->header.delay = header->delay;
		desc->header.payload_length = header->payload_length;
		desc->payload = buf;
		buf += header->payload_length;
		length -= header->payload_length;
	}

	return 0;
}

static int rockchip_mcu_panel_init(struct rockchip_mcu_panel *mcu_panel, ofnode mcu_panel_node)
{
	const void *data;
	int len;
	int ret;

	ret = gpio_request_by_name_nodev(mcu_panel_node, "enable-gpios", 0,
					 &mcu_panel->enable_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		printf("%s: Cannot get mcu panel enable GPIO: %d\n", __func__, ret);
		return ret;
	}

	ret = gpio_request_by_name_nodev(mcu_panel_node, "reset-gpios", 0,
					 &mcu_panel->reset_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		printf("%s: Cannot get mcu panel reset GPIO: %d\n", __func__, ret);
		return ret;
	}

	mcu_panel->desc = malloc(sizeof(struct rockchip_mcu_panel_desc));
	if (!mcu_panel->desc)
		return -ENOMEM;

	mcu_panel->desc->power_invert = ofnode_read_bool(mcu_panel_node, "power-invert");

	mcu_panel->desc->delay.prepare = ofnode_read_u32_default(mcu_panel_node, "prepare-delay-ms", 0);
	mcu_panel->desc->delay.unprepare = ofnode_read_u32_default(mcu_panel_node, "unprepare-delay-ms", 0);
	mcu_panel->desc->delay.enable = ofnode_read_u32_default(mcu_panel_node, "enable-delay-ms", 0);
	mcu_panel->desc->delay.disable = ofnode_read_u32_default(mcu_panel_node, "disable-delay-ms", 0);
	mcu_panel->desc->delay.init = ofnode_read_u32_default(mcu_panel_node, "init-delay-ms", 0);
	mcu_panel->desc->delay.reset = ofnode_read_u32_default(mcu_panel_node, "reset-delay-ms", 0);

	mcu_panel->desc->bus_format = ofnode_read_u32_default(mcu_panel_node, "bus-format",
							      MEDIA_BUS_FMT_RBG888_1X24);
	mcu_panel->desc->bpc = ofnode_read_u32_default(mcu_panel_node, "bpc", 8);

	data = ofnode_get_property(mcu_panel_node, "panel-init-sequence", &len);
	if (data) {
		mcu_panel->desc->init_seq = calloc(1, sizeof(*mcu_panel->desc->init_seq));
		if (!mcu_panel->desc->init_seq)
			return -ENOMEM;

		ret = rockchip_mcu_panel_parse_cmds(data, len, mcu_panel->desc->init_seq);
		if (ret) {
			printf("failed to parse panel init sequence\n");
			goto free_on_cmds;
		}
	}

	data = ofnode_get_property(mcu_panel_node, "panel-exit-sequence", &len);
	if (data) {
		mcu_panel->desc->exit_seq = calloc(1, sizeof(*mcu_panel->desc->exit_seq));
		if (!mcu_panel->desc->exit_seq) {
			ret = -ENOMEM;
			goto free_on_cmds;
		}

		ret = rockchip_mcu_panel_parse_cmds(data, len, mcu_panel->desc->exit_seq);
		if (ret) {
			printf("failed to parse panel exit sequence\n");
			goto free_cmds;
		}
	}

	return 0;

free_cmds:
	free(mcu_panel->desc->exit_seq);
free_on_cmds:
	free(mcu_panel->desc->init_seq);
	return ret;
}

static int rockchip_rgb_probe(struct udevice *dev)
{
	struct rockchip_rgb *rgb = dev_get_priv(dev);
	ofnode mcu_panel_node;
	int phandle;
	int ret;

	rgb->dev = dev;
	rgb->funcs = (const struct rockchip_rgb_funcs *)dev_get_driver_data(dev);
	rgb->grf = syscon_get_regmap(dev_get_parent(dev));
	rgb->data_sync_bypass = dev_read_bool(dev, "rockchip,data-sync-bypass");
	rgb->id = of_alias_get_id(ofnode_to_np(dev->node), "rgb");
	if (rgb->id < 0)
		rgb->id = 0;

	mcu_panel_node = dev_read_subnode(dev, "mcu-panel");
	if (ofnode_valid(mcu_panel_node) && ofnode_is_available(mcu_panel_node)) {
		struct rockchip_mcu_panel *mcu_panel;

		mcu_panel = malloc(sizeof(struct rockchip_mcu_panel));
		if (!mcu_panel) {
			printf("failed to alloc mcu_panel data\n");
			return -ENOMEM;
		}

		ret = rockchip_mcu_panel_init(mcu_panel, mcu_panel_node);
		if (ret < 0) {
			printf("failed to init mcu_panel: %d\n", ret);
			return ret;
		}

		phandle = ofnode_read_u32_default(mcu_panel_node, "backlight", -1);
		if (phandle < 0) {
			printf("failed to find backlight phandle\n");
			return -EINVAL;
		}

		ret = uclass_get_device_by_phandle_id(UCLASS_PANEL_BACKLIGHT, phandle,
						      &mcu_panel->backlight);
		if (ret && ret != -ENOENT) {
			printf("%s: failed to get backlight device: %d\n", __func__, ret);
			return ret;
		}

		mcu_panel->base.dev = dev;
		mcu_panel->base.bus_format = mcu_panel->desc->bus_format;
		mcu_panel->base.bpc = mcu_panel->desc->bpc;
		mcu_panel->base.funcs = &rockchip_mcu_panel_funcs;
		mcu_panel->enabled = false;
		mcu_panel->prepared = false;

		rgb->connector.panel = &mcu_panel->base;
	}

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

static void rk3562_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK3562_GRF_IOC_VO_IO_CON,
		     RK3562_RGB_DATA_BYPASS(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs rk3562_rgb_funcs = {
	.prepare = rk3562_rgb_prepare,
};

static void rk3568_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK3568_GRF_VO_CON1, RK3568_RGB_DATA_BYPASS(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs rk3568_rgb_funcs = {
	.prepare = rk3568_rgb_prepare,
};

static void rk3576_rgb_prepare(struct rockchip_rgb *rgb, int pipe)
{
	regmap_write(rgb->grf, RK3576_VCCIO_IOC_MISC_CON8,
		     RK3576_VOP_MCU_SEL(rgb->data_sync_bypass));
}

static const struct rockchip_rgb_funcs rk3576_rgb_funcs = {
	.prepare = rk3576_rgb_prepare,
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
		.compatible = "rockchip,rk3562-rgb",
		.data = (ulong)&rk3562_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk3568-rgb",
		.data = (ulong)&rk3568_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk3576-rgb",
		.data = (ulong)&rk3576_rgb_funcs,
	},
	{
		.compatible = "rockchip,rk3588-rgb",
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
