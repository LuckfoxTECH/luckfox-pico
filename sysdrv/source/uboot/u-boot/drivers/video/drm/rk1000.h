/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _RK1000_H_
#define _RK1000_H_

#include <clk.h>
#include <asm/gpio.h>
#include <dm/device.h>
#include <power/regulator.h>

struct rk1000_ctl {
	struct udevice *dev;
	struct gpio_desc reset_gpio;
	struct clk mclk;
};

int rk1000_ctl_i2c_read(struct rk1000_ctl *rk1000_ctl, u8 reg, u8 *val);
int rk1000_ctl_write_block(struct rk1000_ctl *rk1000_ctl,
			   u8 reg, const u8 *buf, u8 len);

#endif
