// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <asm-generic/gpio.h>
#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <max96752f.h>

static int max96752f_gpio_direction_output(struct udevice *dev, unsigned offset,
					   int value)
{
	int ret;

	ret = dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(offset),
				GPIO_OUT_DIS | GPIO_OUT,
				FIELD_PREP(GPIO_OUT_DIS, 0) |
				FIELD_PREP(GPIO_OUT, value));
	if (ret < 0)
		return ret;

	return 0;
}

static int max96752f_gpio_get_value(struct udevice *dev, unsigned offset)
{
	int ret;

	ret = dm_i2c_reg_read(dev->parent, GPIO_A_REG(offset));
	if (ret < 0)
		return ret;

	return FIELD_GET(GPIO_OUT, ret);
}

static int max96752f_gpio_set_value(struct udevice *dev, unsigned offset,
				    int value)
{
	int ret;

	ret = dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(offset), GPIO_OUT,
				FIELD_PREP(GPIO_OUT, value));
	if (ret < 0)
		return ret;

	return 0;
}

static int max96752f_gpio_get_function(struct udevice *dev, unsigned offset)
{
	int ret;

	ret = dm_i2c_reg_read(dev->parent, GPIO_A_REG(offset));
	if (ret < 0)
		return ret;

	return FIELD_GET(GPIO_OUT_DIS, ret) ? GPIOF_INPUT : GPIOF_OUTPUT;
}

static const struct dm_gpio_ops max96752f_gpio_ops = {
	.direction_output	= max96752f_gpio_direction_output,
	.get_value		= max96752f_gpio_get_value,
	.set_value		= max96752f_gpio_set_value,
	.get_function		= max96752f_gpio_get_function,
};

static int max96752f_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);

	uc_priv->gpio_count = 16;
	uc_priv->bank_name = of_node_full_name(dev_np(dev));

	return 0;
}

static const struct udevice_id max96752f_gpio_match[] = {
	{ .compatible = "maxim,max96752f-gpio" },
	{ }
};

U_BOOT_DRIVER(max96752f_gpio) = {
	.name = "max96752f_gpio",
	.id = UCLASS_GPIO,
	.of_match = max96752f_gpio_match,
	.probe = max96752f_gpio_probe,
	.ops = &max96752f_gpio_ops,
};
