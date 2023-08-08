// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <dm/pinctrl.h>
#include <dm/uclass.h>
#include <dm/uclass-id.h>

#include "rk1000.h"

#define CTRL_ADC		0x00
#define ADC_OFF			0x88
#define CTRL_CODEC		0x01
#define CODEC_OFF		0x0d
#define CTRL_I2C		0x02
#define I2C_TIMEOUT_PERIOD	0x22
#define CTRL_TVE		0x03
#define TVE_OFF			0x00

int rk1000_ctl_i2c_write(struct rk1000_ctl *rk1000_ctl, u8 reg, u8 val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk1000_ctl->dev);
	struct i2c_msg msg;
	u8 buf[2];
	int ret;

	buf[0] = reg;
	buf[1] = val;
	msg.addr = chip->chip_addr;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = buf;

	ret = dm_i2c_xfer(rk1000_ctl->dev, &msg, 1);
	if (ret) {
		dev_err(rk1000_ctl->dev, "rk1000 ctrl i2c write failed: %d\n",
			ret);
		return ret;
	}

	return 0;
}

int rk1000_ctl_i2c_read(struct rk1000_ctl *rk1000_ctl, u8 reg, u8 *val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk1000_ctl->dev);
	u8 data;
	struct i2c_msg msg[] = {
		{
			.addr = chip->chip_addr,
			.flags = 0,
			.buf = (u8 *)&reg,
			.len = 1,
		}, {
			.addr = chip->chip_addr,
			.flags = I2C_M_RD,
			.buf = (u8 *)&data,
			.len = 1,
		}
	};
	int ret;

	ret = dm_i2c_xfer(rk1000_ctl->dev, msg, 2);
	if (ret) {
		dev_err(rk1000_ctl->dev, "rk1000 ctrl i2c read failed: %d\n",
			ret);
		return ret;
	}

	*val = data;

	return 0;
}

int rk1000_ctl_write_block(struct rk1000_ctl *rk1000_ctl,
			   u8 reg, const u8 *buf, u8 len)
{
	int i, ret;

	for (i = 0; i < len; i++) {
		ret = rk1000_ctl_i2c_write(rk1000_ctl, reg + i, buf[i]);
		if (ret)
			break;
	}

	return ret;
}

static int rk1000_ctl_probe(struct udevice *dev)
{
	struct rk1000_ctl *rk1000_ctl = dev_get_priv(dev);
	int ret;

	rk1000_ctl->dev = dev;

	ret = gpio_request_by_name(dev, "reset-gpios", 0,
				   &rk1000_ctl->reset_gpio, GPIOD_IS_OUT);
	if (ret) {
		dev_err(dev, "Cannot get reset GPIO: %d\n", ret);
		return ret;
	}

	ret = clk_get_by_name(dev, "mclk", &rk1000_ctl->mclk);
	if (ret < 0) {
		dev_err(dev, "failed to get clkin: %d\n", ret);
		return ret;
	}

	clk_enable(&rk1000_ctl->mclk);

	pinctrl_select_state(rk1000_ctl->dev, "default");

	dm_gpio_set_value(&rk1000_ctl->reset_gpio, 0);
	mdelay(1);
	dm_gpio_set_value(&rk1000_ctl->reset_gpio, 1);
	mdelay(1);
	dm_gpio_set_value(&rk1000_ctl->reset_gpio, 0);

	return 0;
}

static const struct udevice_id rk1000_ctl_of_match[] = {
	{ .compatible = "rockchip,rk1000-ctl" },
	{}
};

U_BOOT_DRIVER(rk1000) = {
	.name = "rk1000_ctl",
	.id = UCLASS_I2C_GENERIC,
	.of_match = rk1000_ctl_of_match,
	.probe = rk1000_ctl_probe,
	.bind = dm_scan_fdt_dev,
	.priv_auto_alloc_size = sizeof(struct rk1000_ctl),
};
