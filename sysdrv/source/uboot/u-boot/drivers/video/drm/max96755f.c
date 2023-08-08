// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <max96755f.h>
#include <video_bridge.h>
#include <dm/of_access.h>
#include <linux/media-bus-format.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

static void max96755f_mipi_dsi_rx_config(struct max96755f_priv *priv)
{
	struct drm_display_mode *mode = &priv->mode;
	u32 hfp, hsa, hbp, hact;
	u32 vact, vsa, vfp, vbp;

	dm_i2c_reg_clrset(priv->dev, 0x0331, NUM_LANES,
			  FIELD_PREP(NUM_LANES, priv->num_lanes - 1));
	if (!priv->dpi_deskew_en)
		return;

	vact = mode->vdisplay;
	vsa = mode->vsync_end - mode->vsync_start;
	vfp = mode->vsync_start - mode->vdisplay;
	vbp = mode->vtotal - mode->vsync_end;
	hact = mode->hdisplay;
	hsa = mode->hsync_end - mode->hsync_start;
	hfp = mode->hsync_start - mode->hdisplay;
	hbp = mode->htotal - mode->hsync_end;
	dm_i2c_reg_write(priv->dev, 0x03A4, 0xc1);
	dm_i2c_reg_write(priv->dev, 0x0385, FIELD_PREP(DPI_HSYNC_WIDTH_L, hsa));
	dm_i2c_reg_write(priv->dev, 0x0386, FIELD_PREP(DPI_VYSNC_WIDTH_L, vsa));
	dm_i2c_reg_write(priv->dev, 0x0387,
			 FIELD_PREP(DPI_VSYNC_WIDTH_H, (vsa >> 8)) |
			 FIELD_PREP(DPI_HSYNC_WIDTH_H, (hsa >> 8)));
	dm_i2c_reg_write(priv->dev, 0x03a5, FIELD_PREP(DPI_VFP_L, vfp));
	dm_i2c_reg_write(priv->dev, 0x03a6,
			 FIELD_PREP(DPI_VBP_L, vbp) |
			 FIELD_PREP(DPI_VFP_H, (vfp >> 8)));
	dm_i2c_reg_write(priv->dev, 0x03a7, FIELD_PREP(DPI_VBP_H, (vbp >> 4)));
	dm_i2c_reg_write(priv->dev, 0x03a8, FIELD_PREP(DPI_VACT_L, vact));
	dm_i2c_reg_write(priv->dev, 0x03a9, FIELD_PREP(DPI_VACT_H, (vact >> 8)));
	dm_i2c_reg_write(priv->dev, 0x03aa, FIELD_PREP(DPI_HFP_L, hfp));
	dm_i2c_reg_write(priv->dev, 0x03ab,
			 FIELD_PREP(DPI_HBP_L, hbp) |
			 FIELD_PREP(DPI_HFP_H, (hfp >> 7)));
	dm_i2c_reg_write(priv->dev, 0x03ac, FIELD_PREP(DPI_HBP_H, (hbp >> 4)));
	dm_i2c_reg_write(priv->dev, 0x03ad, FIELD_PREP(DPI_HACT_L, hact));
	dm_i2c_reg_write(priv->dev, 0x03ae, FIELD_PREP(DPI_HACT_H, (hact >> 8)));
}

static void max96755f_bridge_enable(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	struct max96755f_priv *priv = dev_get_priv(dev->parent);

	max96755f_mipi_dsi_rx_config(priv);
	if (priv->split_mode) {
		dm_i2c_reg_clrset(dev->parent, 0x0010,
				  RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				  FIELD_PREP(RESET_ONESHOT, 1) |
				  FIELD_PREP(AUTO_LINK, 0) |
				  FIELD_PREP(LINK_CFG, SPLITTER_MODE));
		mdelay(50);
		dm_i2c_reg_clrset(dev->parent, 0x0053,
				  TX_SPLIT_MASK_B | TX_SPLIT_MASK_A | TX_STR_SEL,
				  FIELD_PREP(TX_SPLIT_MASK_B, 0) |
				  FIELD_PREP(TX_SPLIT_MASK_A, 1) |
				  FIELD_PREP(TX_STR_SEL, 0));
		dm_i2c_reg_clrset(dev->parent, 0x0057,
				  TX_SPLIT_MASK_B | TX_SPLIT_MASK_A | TX_STR_SEL,
				  FIELD_PREP(TX_SPLIT_MASK_B, 1) |
				  FIELD_PREP(TX_SPLIT_MASK_A, 0) |
				  FIELD_PREP(TX_STR_SEL, 1));
		dm_i2c_reg_clrset(dev->parent, 0x032a,
				  DV_SWP_AB | DV_CONV | DV_SPL | DV_EN,
				  FIELD_PREP(DV_SWP_AB, priv->dv_swp_ab) |
				  FIELD_PREP(DV_CONV, 1) |
				  FIELD_PREP(DV_SPL, 1) |
				  FIELD_PREP(DV_EN, 1));
		dm_i2c_reg_clrset(dev->parent, 0x0311,
				  START_PORTAX | START_PORTAY,
				  FIELD_PREP(START_PORTAX, 1) |
				  FIELD_PREP(START_PORTAY, 1));
		dm_i2c_reg_clrset(dev->parent, 0x0002,
				  VID_TX_EN_X | VID_TX_EN_Y,
				  FIELD_PREP(VID_TX_EN_X, 1) |
				  FIELD_PREP(VID_TX_EN_Y, 1));

	} else {
		dm_i2c_reg_clrset(dev->parent, 0x0002, VID_TX_EN_X,
				  FIELD_PREP(VID_TX_EN_X, 1));
		dm_i2c_reg_clrset(dev->parent, 0x0311, START_PORTAX,
				  FIELD_PREP(START_PORTAX, 1));
	}

	dm_i2c_reg_clrset(dev->parent, 0x0010, RESET_ONESHOT,
			  FIELD_PREP(RESET_ONESHOT, 1));
	mdelay(100);
}

static void max96755f_bridge_disable(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	struct max96755f_priv *priv = dev_get_priv(dev->parent);

	dm_i2c_reg_clrset(dev->parent, 0x0002, VID_TX_EN_X,
			  FIELD_PREP(VID_TX_EN_X, 0));

	if (priv->split_mode)
		dm_i2c_reg_clrset(dev->parent, 0x0010,
				  AUTO_LINK | LINK_CFG,
				  FIELD_PREP(AUTO_LINK, 0) |
				  FIELD_PREP(LINK_CFG, LINKA));
}

static void max96755f_bridge_mode_set(struct rockchip_bridge *bridge,
				      const struct drm_display_mode *mode)
{
	struct udevice *dev = bridge->dev;
	struct max96755f_priv *priv = dev_get_priv(dev->parent);

	memcpy(&priv->mode, mode, sizeof(struct drm_display_mode));
}

static bool max96755f_bridge_detect(struct rockchip_bridge *bridge)
{
	struct max96755f_priv *priv = dev_get_priv(bridge->dev->parent);

	if (!dm_gpio_get_value(&priv->lock_gpio))
		return false;

	return true;
}

static const struct rockchip_bridge_funcs max96755f_bridge_funcs = {
	.enable = max96755f_bridge_enable,
	.disable = max96755f_bridge_disable,
	.mode_set = max96755f_bridge_mode_set,
	.detect = max96755f_bridge_detect,
};

static int max96755f_bridge_probe(struct udevice *dev)
{
	struct rockchip_bridge *bridge;
	struct max96755f_priv *priv = dev_get_priv(dev->parent);
	int ret;

	bridge = calloc(1, sizeof(*bridge));
	if (!bridge)
		return -ENOMEM;

	dev->driver_data = (ulong)bridge;
	bridge->dev = dev;
	bridge->funcs = &max96755f_bridge_funcs;

	priv->num_lanes = dev_read_u32_default(dev, "dsi,lanes", 4);
	priv->dv_swp_ab = dev_read_bool(dev, "vd-swap-ab");
	priv->dpi_deskew_en = dev_read_bool(dev, "dpi-deskew-en");

	ret = gpio_request_by_name(dev, "lock-gpios", 0, &priv->lock_gpio,
				   GPIOD_IS_IN);
	if (ret) {
		dev_err(dev, "failed to get lock GPIO: %d\n", ret);
		return ret;
	}

	return 0;
}

static const struct udevice_id max96755f_bridge_of_match[] = {
	{ .compatible = "maxim,max96755f-bridge", },
	{ }
};

U_BOOT_DRIVER(max96755f_bridge) = {
	.name = "max96755f_bridge",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = max96755f_bridge_of_match,
	.probe = max96755f_bridge_probe,
};
