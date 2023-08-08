// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <max96755f.h>
#include <dm/of_access.h>
#include <dm/ofnode.h>

static int max96755f_select(struct udevice *mux, struct udevice *bus,
			    uint channel)
{
	struct max96755f_priv *priv = dev_get_priv(mux);
	int link_cfg;

	dm_i2c_reg_clrset(priv->dev, 0x0001, DIS_REM_CC,
			  FIELD_PREP(DIS_REM_CC, 0));

	if (!priv->split_mode)
		return 0;

	link_cfg = dm_i2c_reg_read(priv->dev, 0x0010);
	if ((link_cfg & LINK_CFG) == SPLITTER_MODE)
		return 0;

	if (channel == 0 && (link_cfg & LINK_CFG) != LINKA) {
		dm_i2c_reg_clrset(priv->dev, 0x0010,
				  RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				  FIELD_PREP(RESET_ONESHOT, 1) |
				  FIELD_PREP(AUTO_LINK, 0) |
				  FIELD_PREP(LINK_CFG, LINKA));
		mdelay(50);
	} else if (channel == 1 && (link_cfg & LINK_CFG) != LINKB) {
		dm_i2c_reg_clrset(priv->dev, 0x0010,
				  RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				  FIELD_PREP(RESET_ONESHOT, 1) |
				  FIELD_PREP(AUTO_LINK, 0) |
				  FIELD_PREP(LINK_CFG, LINKB));
		mdelay(50);
	}

	return 0;
}

static int max96755f_deselect(struct udevice *mux, struct udevice *bus,
			      uint channel)
{
	struct max96755f_priv *priv = dev_get_priv(mux);

	dm_i2c_reg_clrset(priv->dev, 0x0001, DIS_REM_CC,
			  FIELD_PREP(DIS_REM_CC, 1));
	return 0;
}

static const struct i2c_mux_ops max96755f_ops = {
	.select = max96755f_select,
	.deselect = max96755f_deselect,
};

static int max96755f_power_on(struct max96755f_priv *priv)
{
	int ret;

	if (dm_gpio_is_valid(&priv->enable_gpio)) {
		dm_gpio_set_value(&priv->enable_gpio, 1);
		mdelay(100);
	}

	ret = dm_i2c_reg_clrset(priv->dev, 0x0010, RESET_ALL,
				FIELD_PREP(RESET_ALL, 1));
	if (ret < 0)
		return ret;

	mdelay(100);

	dm_i2c_reg_clrset(priv->dev, 0x0001, DIS_REM_CC,
			  FIELD_PREP(DIS_REM_CC, 1));
	return 0;
}

static int max96755f_probe(struct udevice *dev)
{
	struct max96755f_priv *priv = dev_get_priv(dev);
	ofnode child;
	u8 nr = 0;
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	priv->dev = dev;

	ret = gpio_request_by_name(dev, "enable-gpios", 0,
				   &priv->enable_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "%s: failed to get enable GPIO: %d\n", __func__, ret);
		return ret;
	}

	ret = max96755f_power_on(priv);
	if (ret) {
		dev_err(dev, "%s: failed to power on: %d\n", __func__, ret);
		return ret;
	}

	ofnode_for_each_subnode(child, dev_ofnode(dev)) {
		if (!ofnode_is_available(child) ||
		    !of_find_property(ofnode_to_np(child), "reg", NULL))
			continue;

		nr++;
	}

	if (nr == 2)
		priv->split_mode = true;

	return 0;
}

static const struct udevice_id max96755f_of_match[] = {
	{ .compatible = "maxim,max96755f" },
	{}
};

U_BOOT_DRIVER(max96755f) = {
	.name = "max96755f",
	.id = UCLASS_I2C_MUX,
	.of_match = max96755f_of_match,
	.bind = dm_scan_fdt_dev,
	.probe = max96755f_probe,
	.ops = &max96755f_ops,
	.priv_auto_alloc_size = sizeof(struct max96755f_priv),
};

