// SPDX-License-Identifier: GPL-2.0-only
/*
 *  NCA9539 I2C Port Expander I/O
 *
 *  Copyright (C) 2023 Cody Xie <cody.xie@rock-chips.com>
 *
 */
#include <common.h>
#include <errno.h>
#include <dm.h>
#include <fdtdec.h>
#include <i2c.h>
#include <malloc.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <dt-bindings/gpio/gpio.h>
#include <linux/bitops.h>

#define NCA9539_REG_INPUT_PORT_BASE 0x00
#define NCA9539_REG_INPUT_PORT0 (NCA9539_REG_INPUT_PORT_BASE + 0x0)
#define NCA9539_REG_INPUT_PORT1 (NCA9539_REG_INPUT_PORT_BASE + 0x1)
#define NCA9539_REG_OUTPUT_PORT_BASE 0x02
#define NCA9539_REG_OUTPUT_PORT0 (NCA9539_REG_OUTPUT_PORT_BASE + 0x0)
#define NCA9539_REG_OUTPUT_PORT1 (NCA9539_REG_OUTPUT_PORT_BASE + 0x1)
#define NCA9539_REG_POLARITY_BASE 0x04
#define NCA9539_REG_POLARITY_PORT0 (NCA9539_REG_POLARITY_BASE + 0x0)
#define NCA9539_REG_POLARITY_PORT1 (NCA9539_REG_POLARITY_BASE + 0x1)
#define NCA9539_REG_CONFIG_BASE 0x06
#define NCA9539_REG_CONFIG_PORT0 (NCA9539_REG_CONFIG_BASE + 0x0)
#define NCA9539_REG_CONFIG_PORT1 (NCA9539_REG_CONFIG_BASE + 0x1)

#define NCA9539_BANK_SZ 8
#define NCA9539_MAX_BANK 2

#define NCA9539_CHIP_ADDR 0x74

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

struct nca9539_info {
	struct udevice *dev;
	int addr;
	unsigned int ngpio;
};

static int nca9539_write_reg(struct udevice *dev, int reg, u8 val)
{
	int ret = 0;

	ret = dm_i2c_write(dev, reg, &val, 1);
	if (ret) {
		dev_err(dev, "%s error\n", __func__);
		return ret;
	}

	return 0;
}

static int nca9539_read_reg(struct udevice *dev, int reg, u8 *val)
{
	int ret;
	u8 byte;

	ret = dm_i2c_read(dev, reg, &byte, 1);
	if (ret) {
		dev_err(dev, "%s error\n", __func__);
		return ret;
	}

	*val = byte;

	return 0;
}

static int nca9539_gpio_get_direction(struct udevice *dev, unsigned int offset)
{
	unsigned int port = offset / NCA9539_BANK_SZ;
	unsigned int pin = offset % NCA9539_BANK_SZ;
	u8 value;
	int ret;

	dev_dbg(dev, "%s offset(%d)\n", __func__, offset);
	ret = nca9539_read_reg(dev, NCA9539_REG_CONFIG_BASE + port, &value);
	if (ret < 0) {
		dev_err(dev, "%s offset(%d) read config failed\n", __func__,
			offset);
		return ret;
	}

	if (value & BIT(pin))
		return GPIOF_INPUT;

	return GPIOF_OUTPUT;
}

static int nca9539_gpio_direction_input(struct udevice *dev,
					unsigned int offset)
{
	unsigned int port = offset / NCA9539_BANK_SZ;
	unsigned int pin = offset % NCA9539_BANK_SZ;
	u8 val;
	int ret = 0;

	dev_dbg(dev, "%s offset(%d)\n", __func__, offset);

	ret = nca9539_read_reg(dev, NCA9539_REG_CONFIG_BASE + port, &val);
	if (!ret) {
		val &= ~BIT(pin);
		val |= BIT(pin);
		ret = nca9539_write_reg(dev, NCA9539_REG_CONFIG_BASE + port,
					val);
	}

	if (ret < 0) {
		dev_err(dev, "%s offset(%d) read config failed\n", __func__,
			offset);
	}

	return ret;
}

static int nca9539_gpio_direction_output(struct udevice *dev,
					 unsigned int offset, int val)
{
	unsigned int port = offset / NCA9539_BANK_SZ;
	unsigned int pin = offset % NCA9539_BANK_SZ;
	u8 value;
	int ret;

	dev_dbg(dev, "%s offset(%d) val(%d)\n", __func__, offset, val);

	ret = nca9539_read_reg(dev, NCA9539_REG_CONFIG_BASE + port, &value);
	if (!ret) {
		value &= ~BIT(pin);
		ret = nca9539_write_reg(dev, NCA9539_REG_CONFIG_BASE + port,
					value);
	}
	if (ret < 0) {
		dev_warn(dev, "%s offset(%d) read config failed\n", __func__,
			offset);
	}

	ret = nca9539_read_reg(dev, NCA9539_REG_OUTPUT_PORT_BASE + port,
			       &value);
	if (!ret) {
		value &= ~BIT(pin);
		value |= val ? BIT(pin) : 0;
		ret = nca9539_write_reg(
			dev, NCA9539_REG_OUTPUT_PORT_BASE + port, value);
	}
	if (ret < 0) {
		dev_err(dev, "%s offset(%d) val(%d) update output failed\n",
			__func__, offset, val);
	}

	return ret;
}

static int nca9539_gpio_get(struct udevice *dev, unsigned int offset)
{
	unsigned int port = offset / NCA9539_BANK_SZ;
	unsigned int pin = offset % NCA9539_BANK_SZ;
	int reg;
	u8 value;
	int ret;

	dev_dbg(dev, "%s offset(%d)\n", __func__, offset);
	ret = nca9539_read_reg(dev, NCA9539_REG_CONFIG_BASE + port, &value);
	if (ret < 0) {
		dev_err(dev, "%s offset(%d) check config failed\n", __func__,
			offset);
		return ret;
	}
	if (!(BIT(pin) & value))
		reg = NCA9539_REG_OUTPUT_PORT_BASE + port;
	else
		reg = NCA9539_REG_INPUT_PORT_BASE + port;
	ret = nca9539_read_reg(dev, reg, &value);
	if (ret < 0) {
		dev_err(dev, "%s offset(%d) read value failed\n", __func__,
			offset);
		return -EIO;
	}

	return !!(BIT(pin) & value);
}

static int nca9539_gpio_set(struct udevice *dev, unsigned int offset, int val)
{
	unsigned int port = offset / NCA9539_BANK_SZ;
	unsigned int pin = offset % NCA9539_BANK_SZ;
	u8 value;
	int ret;

	dev_dbg(dev, "%s offset(%d) val(%d)\n", __func__, offset, val);
	ret = nca9539_read_reg(dev, NCA9539_REG_CONFIG_BASE + port, &value);
	if (ret < 0 || !!(BIT(pin) & value)) {
		dev_warn(dev, "%s offset(%d) val(%d) check config failed\n",
			__func__, offset, val);
	}

	ret = nca9539_read_reg(dev, NCA9539_REG_OUTPUT_PORT_BASE + port,
			       &value);
	if (!ret) {
		value &= ~BIT(pin);
		value |= val ? BIT(pin) : 0;
		ret = nca9539_write_reg(
			dev, NCA9539_REG_OUTPUT_PORT_BASE + port, value);
	}
	if (ret < 0) {
		dev_err(dev, "%s offset(%d) val(%d) read input failed\n",
			__func__, offset, val);
	}

	return ret;
}

static int nca9539_get_function(struct udevice *dev, unsigned offset)
{
	return nca9539_gpio_get_direction(dev, offset);
}

static int nca9539_xlate(struct udevice *dev, struct gpio_desc *desc,
			 struct ofnode_phandle_args *args)
{
	desc->offset = args->args[0];
	desc->flags = args->args[1] & GPIO_ACTIVE_LOW ? GPIOD_ACTIVE_LOW : 0;

	return 0;
}

static const struct dm_gpio_ops nca9539_ops = {
	.direction_input = nca9539_gpio_direction_input,
	.direction_output = nca9539_gpio_direction_output,
	.get_value = nca9539_gpio_get,
	.set_value = nca9539_gpio_set,
	.get_function = nca9539_get_function,
	.xlate = nca9539_xlate,
};

static int nca9539_probe(struct udevice *dev)
{
	struct nca9539_info *info = dev_get_platdata(dev);
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct dm_i2c_chip *chip = dev_get_parent_platdata(dev);
	char name[32], *str;
	ulong driver_data;

	if (!info) {
		dev_err(dev, "platdata not ready\n");
		return -ENOMEM;
	}

	if (!chip) {
		dev_err(dev, "i2c not ready\n");
		return -ENODEV;
	}

#if CONFIG_IS_ENABLED(OF_CONTROL)
	info->addr = chip->chip_addr;
#else
	info->addr = NCA9539_CHIP_ADDR;
#endif

	driver_data = dev_get_driver_data(dev);
	info->ngpio = driver_data;
	if (info->ngpio > NCA9539_MAX_BANK * NCA9539_BANK_SZ) {
		dev_err(dev, "Max support %d pins now\n",
			NCA9539_MAX_BANK * NCA9539_BANK_SZ);
		return -EINVAL;
	}

	snprintf(name, sizeof(name), "gpio@%x_", info->addr);
	str = strdup(name);
	if (!str)
		return -ENOMEM;
	uc_priv->bank_name = str;
	uc_priv->gpio_count = info->ngpio;

	dev_dbg(dev, "%s is ready\n", str);

	return 0;
}

static const struct udevice_id nca9539_ids[] = {
	{
		.compatible = "novo,nca9539-gpio",
		.data = (ulong)16,
	},
	{ /* sentinel */ },
};

U_BOOT_DRIVER(nca9539) = {
	.name = "nca9539",
	.id = UCLASS_GPIO,
	.ops = &nca9539_ops,
	.probe = nca9539_probe,
	.platdata_auto_alloc_size = sizeof(struct nca9539_info),
	.of_match = nca9539_ids,
};
