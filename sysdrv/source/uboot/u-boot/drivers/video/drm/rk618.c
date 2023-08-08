// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2008-2018 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <i2c.h>
#include <errno.h>
#include <dm.h>
#include <dm/uclass.h>
#include <dm/uclass-id.h>

#include "rk618.h"

int rk618_i2c_write(struct rk618 *rk618, u16 reg, u32 val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk618->dev);
	struct i2c_msg msg;
	u8 buf[] = {
		(reg >> 0) & 0xff, (reg >> 8) & 0xff,
		(val >> 0) & 0xff, (val >> 8) & 0xff,
		(val >> 16) & 0xff, (val >> 24) & 0xff
	};
	int ret;

	msg.addr = chip->chip_addr;
	msg.flags = 0;
	msg.len = sizeof(buf);
	msg.buf = buf;

	ret = dm_i2c_xfer(rk618->dev, &msg, 1);
	if (ret) {
		dev_err(rk618->dev, "Could not execute transfer: %d\n", ret);
		return ret;
	}

	return 0;
}

int rk618_i2c_read(struct rk618 *rk618, u16 reg, u32 *val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk618->dev);
	u32 data;
	struct i2c_msg msg[] = {
		{
			.addr = chip->chip_addr,
			.flags = 0,
			.buf = (u8 *)&reg,
			.len = 2,
		}, {
			.addr = chip->chip_addr,
			.flags = I2C_M_RD,
			.buf = (u8 *)&data,
			.len = 4,
		}
	};
	int ret;

	ret = dm_i2c_xfer(rk618->dev, msg, 2);
	if (ret) {
		dev_err(rk618->dev, "Could not execute transfer: %d\n", ret);
		return ret;
	}

	*val = data;

	return 0;
}

void rk618_frc_dither_disable(struct rk618 *rk618)
{
	rk618_i2c_write(rk618, RK618_FRC_REG, FRC_DITHER_DISABLE);
}

void rk618_frc_dither_enable(struct rk618 *rk618)
{
	rk618_i2c_write(rk618, RK618_FRC_REG, FRC_DITHER_ENABLE);
}

void rk618_frc_dclk_invert(struct rk618 *rk618)
{
	rk618_i2c_write(rk618, RK618_FRC_REG, FRC_DCLK_INV);
}

static int rk618_power_on(struct rk618 *rk618)
{
	if (rk618->power_supply)
		regulator_set_enable(rk618->power_supply, 1);

	if (dm_gpio_is_valid(&rk618->enable_gpio))
		dm_gpio_set_value(&rk618->enable_gpio, 1);

	mdelay(2);
	dm_gpio_set_value(&rk618->reset_gpio, 0);
	mdelay(4);
	dm_gpio_set_value(&rk618->reset_gpio, 1);
	mdelay(50);
	dm_gpio_set_value(&rk618->reset_gpio, 0);

	return 0;
}

static void rk618_cru_init(struct rk618 *rk618)
{
	rk618_i2c_write(rk618, 0x0058, 0xffff0000);
	rk618_i2c_write(rk618, 0x005c, 0xffff191e);
	rk618_i2c_write(rk618, 0x0060, 0x00000000);
	rk618_i2c_write(rk618, 0x0064, 0xffff2186);
	rk618_i2c_write(rk618, 0x0068, 0xffff1028);
	rk618_i2c_write(rk618, 0x006c, 0xffff0641);
	rk618_i2c_write(rk618, 0x0070, 0x00800000);
	rk618_i2c_write(rk618, 0x0074, 0xffff1028);
	rk618_i2c_write(rk618, 0x0078, 0xffff0641);
	rk618_i2c_write(rk618, 0x007c, 0x00800000);
}

static int rk618_probe(struct udevice *dev)
{
	struct rk618 *rk618 = dev_get_priv(dev);
	int ret;

	rk618->dev = dev;

	ret = uclass_get_device_by_phandle(UCLASS_REGULATOR, dev,
					   "power-supply",
					   &rk618->power_supply);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "Cannot get power supply: %d\n", ret);
		return ret;
	}

	ret = gpio_request_by_name(dev, "enable-gpios", 0,
				   &rk618->enable_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "Cannot get enable GPIO: %d\n", ret);
		return ret;
	}

	ret = gpio_request_by_name(dev, "reset-gpios", 0,
				   &rk618->reset_gpio, GPIOD_IS_OUT);
	if (ret) {
		dev_err(dev, "Cannot get reset GPIO: %d\n", ret);
		return ret;
	}

	ret = clk_get_by_name(dev, "clkin", &rk618->clkin);
	if (ret < 0) {
		dev_err(dev, "failed to get clkin: %d\n", ret);
		return ret;
	}

	ret = clk_set_rate(&rk618->clkin, 11289600);
	if (ret < 0) {
		dev_err(dev, "failed to set rate: %d\n", ret);
		return ret;
	}

	clk_enable(&rk618->clkin);

	ret = rk618_power_on(rk618);
	if (ret) {
		dev_err(dev, "failed to power on: %d\n", ret);
		return ret;
	}

	rk618_cru_init(rk618);

	return 0;
}

static const struct udevice_id rk618_of_match[] = {
	{ .compatible = "rockchip,rk618" },
	{}
};

U_BOOT_DRIVER(rk618) = {
	.name = "rk618",
	.id = UCLASS_I2C_GENERIC,
	.of_match = rk618_of_match,
	.probe = rk618_probe,
	.bind = dm_scan_fdt_dev,
	.priv_auto_alloc_size = sizeof(struct rk618),
};
