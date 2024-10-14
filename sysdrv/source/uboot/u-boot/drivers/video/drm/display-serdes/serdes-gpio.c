// SPDX-License-Identifier: GPL-2.0+
/*
 * gpiolib support for different serdes chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 *
 */

#include "core.h"

static int serdes_gpio_get(struct udevice *dev, unsigned int offset)
{
	struct serdes_pinctrl *serdes_pinctrl = dev_get_priv(dev->parent);
	struct serdes *serdes = serdes_pinctrl->parent;
	int ret = 0;

	if (serdes->chip_data->gpio_ops->get_level)
		ret = serdes->chip_data->gpio_ops->get_level(serdes, offset);

	SERDES_DBG_MFD("%s: %s %s gpio=%d\n", __func__,
		       serdes->dev->name,
		       serdes->chip_data->name, offset);
	return ret;
}

static int serdes_gpio_set(struct udevice *dev,
			   unsigned int offset, int value)
{
	struct serdes_pinctrl *serdes_pinctrl = dev_get_priv(dev->parent);
	struct serdes *serdes = serdes_pinctrl->parent;
	int ret = 0;

	if (serdes->chip_data->gpio_ops->set_level)
		ret = serdes->chip_data->gpio_ops->set_level(serdes, offset, value);

	SERDES_DBG_MFD("%s: %s %s gpio=%d,val=%d\n", __func__,
		       serdes->dev->name,
		       serdes->chip_data->name, offset, value);
	return ret;
}

static int serdes_gpio_direction_input(struct udevice *dev,
				       unsigned int offset)
{
	struct serdes_pinctrl *serdes_pinctrl = dev_get_priv(dev->parent);
	struct serdes *serdes = serdes_pinctrl->parent;
	int ret = 0;

	if (serdes->chip_data->gpio_ops->direction_input)
		ret = serdes->chip_data->gpio_ops->direction_input(serdes, offset);

	SERDES_DBG_MFD("%s: %s %s gpio=%d\n", __func__,
		       serdes->dev->name,
		       serdes->chip_data->name, offset);
	return ret;
}

static int serdes_gpio_direction_output(struct udevice *dev,
					unsigned int offset, int value)
{
	struct serdes_pinctrl *serdes_pinctrl = dev_get_priv(dev->parent);
	struct serdes *serdes = serdes_pinctrl->parent;
	int ret = 0;

	if (serdes->chip_data->gpio_ops->direction_output)
		ret = serdes->chip_data->gpio_ops->direction_output(serdes, offset, value);

	SERDES_DBG_MFD("%s: %s %s gpio=%d,val=%d\n", __func__,
		       serdes->dev->name,
		       serdes->chip_data->name, offset, value);
	return ret;
}

static int serdes_gpio_probe(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev->parent->parent);
	struct serdes_pinctrl *serdes_pinctrl = serdes->serdes_pinctrl;
	struct serdes_gpio *serdes_gpio;

	serdes_gpio = calloc(1, sizeof(*serdes_gpio));
	if (!serdes_gpio)
		return -ENOMEM;

	serdes_pinctrl->serdes_gpio = serdes_gpio;

	serdes_gpio->parent = serdes_pinctrl;

	SERDES_DBG_MFD("%s: %s %s\n", __func__,
		       dev->name, serdes->chip_data->name);
	return 0;
}

static struct dm_gpio_ops serdes_gpio_ops = {
	.set_value = serdes_gpio_set,
	.get_value = serdes_gpio_get,
	.direction_input = serdes_gpio_direction_input,
	.direction_output = serdes_gpio_direction_output,
};

U_BOOT_DRIVER(serdes_gpio) = {
	.name = "serdes-gpio",
	.id = UCLASS_GPIO,
	.probe = serdes_gpio_probe,
	.ops = &serdes_gpio_ops,
	.priv_auto_alloc_size = sizeof(struct serdes_gpio),
};

