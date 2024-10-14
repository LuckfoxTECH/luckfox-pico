// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * serdes-bridge.c  --  display bridge for different serdes chips
 *
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "core.h"

static struct serdes *g_serdes_ser_split[MAX_NUM_SERDES_SPLIT];

static void serdes_i2c_init(struct serdes *serdes)
{
	if (serdes->vpower_supply)
		regulator_set_enable(serdes->vpower_supply, true);

	if (dm_gpio_is_valid(&serdes->enable_gpio))
		dm_gpio_set_value(&serdes->enable_gpio, 1);

	//mdelay(5);

	//video_bridge_set_active(serdes->dev, true);

	if (serdes->chip_data->bridge_ops->init)
		serdes->chip_data->bridge_ops->init(serdes);

	serdes_i2c_set_sequence(serdes);

	SERDES_DBG_MFD("%s: %s %s\n", __func__,
		       serdes->dev->name,
		       serdes->chip_data->name);
}

static int serdes_set_i2c_address(struct serdes *serdes,
				  u32 reg_hw, u32 reg_use, int link)
{
	int ret = 0;
	struct dm_i2c_chip *chip_split;
	struct serdes *serdes_split = serdes->g_serdes_bridge_split;

	if (!serdes_split) {
		pr_info("%s: serdes_split is null\n", __func__);
		return -1;
	}

	chip_split = dev_get_parent_platdata(serdes->dev);
	SERDES_DBG_MFD("%s: %s addr=0x%x reg_hw=0x%x, reg_use=0x%x split=0x%p\n",
		       __func__, serdes_split->dev->name,
		       chip_split->chip_addr, serdes->reg_hw,
		       serdes->reg_use, serdes_split);

	chip_split->chip_addr = serdes->reg_hw;

	if (serdes_split && serdes_split->chip_data->split_ops &&
	    serdes_split->chip_data->split_ops->select)
		ret = serdes_split->chip_data->split_ops->select(serdes_split, link);

	if (serdes->chip_data->split_ops &&
	    serdes->chip_data->split_ops->set_i2c_addr)
		serdes->chip_data->split_ops->set_i2c_addr(serdes,
							   reg_use, link);

	if (serdes_split && serdes_split->chip_data->split_ops &&
	    serdes_split->chip_data->split_ops->select)
		ret = serdes_split->chip_data->split_ops->select(serdes_split,
								 SER_SPLITTER_MODE);

	chip_split->chip_addr = serdes->reg_use;

	serdes_i2c_set_sequence(serdes);

	return ret;
}

static int serdes_i2c_probe(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev);
	struct serdes_bridge *serdes_bridge = NULL;
	struct serdes_bridge_split *serdes_bridge_split = NULL;
	struct serdes_pinctrl *serdes_pinctrl = NULL;
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	serdes->dev = dev;
	serdes->chip_data = (struct serdes_chip_data *)dev_get_driver_data(dev);
	serdes->type = serdes->chip_data->serdes_type;

	SERDES_DBG_MFD("serdes %s %s probe start serdes=%p\n",
		       serdes->dev->name, serdes->chip_data->name, serdes);

	ret = uclass_get_device_by_phandle(UCLASS_REGULATOR, dev,
					   "vpower-supply",
					   &serdes->vpower_supply);
	if (ret && ret != -ENOENT)
		SERDES_DBG_MFD("%s: Cannot get power supply: %d\n",
			       __func__, ret);

	ret = gpio_request_by_name(dev, "enable-gpios", 0,
				   &serdes->enable_gpio, GPIOD_IS_OUT);
	if (ret)
		SERDES_DBG_MFD("%s: failed to get enable gpio: %d\n",
			       __func__, ret);

	ret = gpio_request_by_name(dev, "lock-gpios", 0, &serdes->lock_gpio,
				   GPIOD_IS_IN);
	if (ret)
		SERDES_DBG_MFD("%s: failed to get lock gpio: %d\n",
			       __func__, ret);

	ret = gpio_request_by_name(dev, "err-gpios", 0, &serdes->err_gpio,
				   GPIOD_IS_IN);
	if (ret)
		SERDES_DBG_MFD("%s: failed to err gpio: %d\n",
			       __func__, ret);

	if (serdes->chip_data->serdes_type == TYPE_OTHER) {
		SERDES_DBG_MFD("TYPE_OTHER just need only init i2c\n");
		serdes_i2c_init(serdes);
		return 0;
	}

	ret = serdes_get_init_seq(serdes);
	if (ret)
		return ret;

	if (serdes->chip_data->serdes_type == TYPE_SER)
		serdes_i2c_init(serdes);

	if (serdes->chip_data->bridge_ops) {
		serdes_bridge = calloc(1, sizeof(*serdes_bridge));
		if (!serdes_bridge)
			return -ENOMEM;
		serdes->serdes_bridge = serdes_bridge;

		serdes_bridge_split = calloc(1, sizeof(*serdes_bridge_split));
		if (!serdes_bridge_split)
			return -ENOMEM;
		serdes->serdes_bridge_split = serdes_bridge_split;
	}

	serdes_pinctrl = calloc(1, sizeof(*serdes_pinctrl));
	if (!serdes_pinctrl)
		return -ENOMEM;

	serdes->serdes_pinctrl = serdes_pinctrl;
	ret = serdes_pinctrl_register(dev, serdes);
	if (ret)
		return ret;

	serdes->id_serdes_bridge_split = dev_read_u32_default(dev, "id-serdes-bridge-split", 0);
	if ((serdes->id_serdes_bridge_split < MAX_NUM_SERDES_SPLIT) && (serdes->type == TYPE_SER)) {
		g_serdes_ser_split[serdes->id_serdes_bridge_split] = serdes;
		SERDES_DBG_MFD("%s: %s-%s g_serdes_split[%d]=0x%p\n", __func__,
			       serdes->dev->name, serdes->chip_data->name,
			       serdes->id_serdes_bridge_split, serdes);
	}

	serdes->reg_hw = dev_read_u32_default(dev, "reg-hw", 0);
	serdes->reg_use = dev_read_u32_default(dev, "reg", 0);
	serdes->link_use = dev_read_u32_default(dev, "link", 0);
	serdes->id_serdes_panel_split = dev_read_u32_default(dev, "id-serdes-panel-split", 0);

	if ((serdes->id_serdes_panel_split) && (serdes->type == TYPE_DES)) {
		serdes->g_serdes_bridge_split = g_serdes_ser_split[serdes->id_serdes_panel_split];
		SERDES_DBG_MFD("%s: id=%d p=0x%p\n", __func__,
			       serdes->id_serdes_panel_split,
			       serdes->g_serdes_bridge_split);
	}

	if (serdes->reg_hw) {
		SERDES_DBG_MFD("%s: %s change i2c addr from 0x%x to 0x%x\n",
			       __func__, dev->name,
			       serdes->reg_hw, serdes->reg_use);
		serdes_set_i2c_address(serdes, serdes->reg_hw,
				       serdes->reg_use, serdes->link_use);
	}

	printf("%s %s %s successful\n",
	       __func__,
	       serdes->dev->name,
	       serdes->chip_data->name);

	return 0;
}

static const struct udevice_id serdes_of_match[] = {
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROHM_BU18TL82)
	{ .compatible = "rohm,bu18tl82",
		.data = (ulong)&serdes_bu18tl82_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROHM_BU18RL82)
	{ .compatible = "rohm,bu18rl82",
		.data = (ulong)&serdes_bu18rl82_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96745)
	{ .compatible = "maxim,max96745",
		.data = (ulong)&serdes_max96745_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96755)
	{ .compatible = "maxim,max96755",
		.data = (ulong)&serdes_max96755_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96789)
	{ .compatible = "maxim,max96789",
		.data = (ulong)&serdes_max96789_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROCKCHIP_RKX111)
	{ .compatible = "rockchip,rkx111",
		.data = (ulong)&serdes_rkx111_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96752)
	{ .compatible = "maxim,max96752",
		.data = (ulong)&serdes_max96752_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96772)
	{ .compatible = "maxim,max96772",
		.data = (ulong)&serdes_max96772_data },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROCKCHIP_RKX121)
	{ .compatible = "rockchip,rkx121",
		.data = (ulong)&serdes_rkx121_data },
#endif
	{ }
};

U_BOOT_DRIVER(serdes_i2c) = {
	.name = "serdes-i2c",
	.id = UCLASS_MISC,
	.of_match = serdes_of_match,
	.probe = serdes_i2c_probe,
	.bind = dm_scan_fdt_dev,
	.priv_auto_alloc_size = sizeof(struct serdes),
};
