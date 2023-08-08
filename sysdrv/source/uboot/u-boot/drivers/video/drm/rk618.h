/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2008-2018 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _RK618_H_
#define _RK618_H_

#include <clk.h>
#include <dm/device.h>
#include <power/regulator.h>
#include <asm/gpio.h>

#define UPDATE(v, h, l)	(((v) << (l)) & GENMASK((h), (l)))
#define HIWORD_UPDATE(v, h, l)	(((v) << (l)) | (GENMASK(h, l) << 16))

#define RK618_FRC_REG			0x0054
#define FRC_DEN_INV			HIWORD_UPDATE(1, 6, 6)
#define FRC_SYNC_INV			HIWORD_UPDATE(1, 5, 5)
#define FRC_DCLK_INV			HIWORD_UPDATE(1, 4, 4)
#define FRC_OUT_ZERO			HIWORD_UPDATE(1, 3, 3)
#define FRC_OUT_MODE_RGB666		HIWORD_UPDATE(1, 2, 2)
#define FRC_OUT_MODE_RGB888		HIWORD_UPDATE(0, 2, 2)
#define FRC_DITHER_MODE_HI_FRC		HIWORD_UPDATE(1, 1, 1)
#define FRC_DITHER_MODE_FRC		HIWORD_UPDATE(0, 1, 1)
#define FRC_DITHER_ENABLE		HIWORD_UPDATE(1, 0, 0)
#define FRC_DITHER_DISABLE		HIWORD_UPDATE(0, 0, 0)
#define RK618_LVDS_CON			0x0084
#define LVDS_CON_START_PHASE(x)		HIWORD_UPDATE(x, 14, 14)
#define LVDS_DCLK_INV			HIWORD_UPDATE(1, 13, 13)
#define LVDS_CON_CHADS_10PF		HIWORD_UPDATE(3, 12, 11)
#define LVDS_CON_CHADS_5PF		HIWORD_UPDATE(2, 12, 11)
#define LVDS_CON_CHADS_7PF		HIWORD_UPDATE(1, 12, 11)
#define LVDS_CON_CHADS_3PF		HIWORD_UPDATE(0, 12, 11)
#define LVDS_CON_CHA1TTL_ENABLE		HIWORD_UPDATE(1, 10, 10)
#define LVDS_CON_CHA1TTL_DISABLE	HIWORD_UPDATE(0, 10, 10)
#define LVDS_CON_CHA0TTL_ENABLE		HIWORD_UPDATE(1, 9, 9)
#define LVDS_CON_CHA0TTL_DISABLE	HIWORD_UPDATE(0, 9, 9)
#define LVDS_CON_CHA1_POWER_UP		HIWORD_UPDATE(1, 8, 8)
#define LVDS_CON_CHA1_POWER_DOWN	HIWORD_UPDATE(0, 8, 8)
#define LVDS_CON_CHA0_POWER_UP		HIWORD_UPDATE(1, 7, 7)
#define LVDS_CON_CHA0_POWER_DOWN	HIWORD_UPDATE(0, 7, 7)
#define LVDS_CON_CBG_POWER_UP		HIWORD_UPDATE(1, 6, 6)
#define LVDS_CON_CBG_POWER_DOWN		HIWORD_UPDATE(0, 6, 6)
#define LVDS_CON_PLL_POWER_DOWN		HIWORD_UPDATE(1, 5, 5)
#define LVDS_CON_PLL_POWER_UP		HIWORD_UPDATE(0, 5, 5)
#define LVDS_CON_START_SEL_EVEN_PIXEL	HIWORD_UPDATE(1, 4, 4)
#define LVDS_CON_START_SEL_ODD_PIXEL	HIWORD_UPDATE(0, 4, 4)
#define LVDS_CON_CHASEL_DOUBLE_CHANNEL	HIWORD_UPDATE(1, 3, 3)
#define LVDS_CON_CHASEL_SINGLE_CHANNEL	HIWORD_UPDATE(0, 3, 3)
#define LVDS_CON_MSBSEL_D7		HIWORD_UPDATE(1, 2, 2)
#define LVDS_CON_MSBSEL_D0		HIWORD_UPDATE(0, 2, 2)
#define LVDS_CON_SELECT(x)		HIWORD_UPDATE(x, 1, 0)
#define LVDS_CON_SELECT_6BIT_MODE	HIWORD_UPDATE(3, 1, 0)
#define LVDS_CON_SELECT_8BIT_MODE_3	HIWORD_UPDATE(2, 1, 0)
#define LVDS_CON_SELECT_8BIT_MODE_2	HIWORD_UPDATE(1, 1, 0)
#define LVDS_CON_SELECT_8BIT_MODE_1	HIWORD_UPDATE(0, 1, 0)

struct rk618 {
	struct udevice *dev;
	struct udevice *power_supply;
	struct gpio_desc enable_gpio;
	struct gpio_desc reset_gpio;
	struct clk clkin;
};

int rk618_i2c_write(struct rk618 *rk618, u16 reg, u32 val);
int rk618_i2c_read(struct rk618 *rk618, u16 reg, u32 *val);
void rk618_frc_dither_disable(struct rk618 *rk618);
void rk618_frc_dither_enable(struct rk618 *rk618);
void rk618_frc_dclk_invert(struct rk618 *rk618);

#endif
