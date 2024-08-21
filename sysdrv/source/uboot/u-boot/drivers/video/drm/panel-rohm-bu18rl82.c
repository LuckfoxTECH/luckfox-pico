// SPDX-License-Identifier: GPL-2.0+
/*
 * Rohm BU18RL82-based panel driver
 *
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <config.h>
#include <common.h>
#include <backlight.h>
#include <errno.h>
#include <malloc.h>
#include <video.h>

#include <asm/gpio.h>
#include <dm/device.h>
#include <dm/read.h>
#include <dm/pinctrl.h>
#include <dm/uclass-id.h>
#include <linux/media-bus-format.h>

#include "rockchip_display.h"
#include "rockchip_panel.h"

struct bu18rl82;

struct panel_desc {
	const char *name;
	int (*prepare)(struct bu18rl82 *rl82);
	int (*unprepare)(struct bu18rl82 *rl82);
	int (*enable)(struct bu18rl82 *rl82);
	int (*disable)(struct bu18rl82 *rl82);
	int (*backlight_enable)(struct bu18rl82 *rl82);
	int (*backlight_disable)(struct bu18rl82 *rl82);
};

struct bu18rl82 {
	struct udevice *dev;
	struct udevice *backlight;
	const struct panel_desc *desc;
};

static void bu18rl82_panel_prepare(struct rockchip_panel *panel)
{
	struct bu18rl82 *rl82 = dev_get_priv(panel->dev);
	const struct panel_desc *desc = rl82->desc;

	if (desc->prepare)
		desc->prepare(rl82);
}

static void bu18rl82_panel_unprepare(struct rockchip_panel *panel)
{
	struct bu18rl82 *rl82 = dev_get_priv(panel->dev);
	const struct panel_desc *desc = rl82->desc;

	if (desc->unprepare)
		desc->unprepare(rl82);
}

static void bu18rl82_panel_enable(struct rockchip_panel *panel)
{
	struct bu18rl82 *rl82 = dev_get_priv(panel->dev);
	const struct panel_desc *desc = rl82->desc;

	if (desc->enable)
		desc->enable(rl82);

	if (rl82->backlight)
		backlight_enable(rl82->backlight);

	if (desc->backlight_enable)
		desc->backlight_enable(rl82);
}

static void bu18rl82_panel_disable(struct rockchip_panel *panel)
{
	struct bu18rl82 *rl82 = dev_get_priv(panel->dev);
	const struct panel_desc *desc = rl82->desc;

	if (desc->backlight_disable)
		desc->backlight_disable(rl82);

	if (rl82->backlight)
		backlight_disable(rl82->backlight);

	if (desc->disable)
		desc->disable(rl82);
}

static const struct rockchip_panel_funcs bu18rl82_panel_funcs = {
	.prepare = bu18rl82_panel_prepare,
	.unprepare = bu18rl82_panel_unprepare,
	.enable = bu18rl82_panel_enable,
	.disable = bu18rl82_panel_disable,
};

static int bu18rl82_probe(struct udevice *dev)
{
	struct bu18rl82 *rl82 = dev_get_priv(dev);
	struct rockchip_panel *panel;
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	rl82->dev = dev;
	rl82->desc = (const struct panel_desc *)dev_get_driver_data(dev);

	ret = uclass_get_device_by_phandle(UCLASS_PANEL_BACKLIGHT, dev,
					   "backlight", &rl82->backlight);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "%s: Cannot get backlight: %d\n", __func__, ret);
		return ret;
	}

	panel = calloc(1, sizeof(*panel));
	if (!panel)
		return -ENOMEM;

	dev->driver_data = (ulong)panel;
	panel->dev = dev;
	panel->bus_format = MEDIA_BUS_FMT_RGB888_1X24;
	panel->funcs = &bu18rl82_panel_funcs;

	return 0;
}

#define rohm_deserializer_write(rl82, reg, val) do {			\
		int ret;						\
		ret = dm_i2c_reg_write(rl82->dev, reg, val);		\
		if (ret) {						\
			dev_err(rl82->dev,				\
				"failed to set register 0x%04x\n",	\
				reg);					\
			return ret;					\
		}							\
	} while (0)

static int csot_mg1561b01_prepare(struct bu18rl82 *rl82)
{
	const struct reg_sequence {
		u16 reg;
		u8 def;
	} regs[] = {
		{ 0x0011, 0x03 }, { 0x0012, 0x03 },
		{ 0x001f, 0x02 }, { 0x0020, 0x02 },
		{ 0x0031, 0x41 }, { 0x0032, 0x41 },
		{ 0x0073, 0x80 }, { 0x0074, 0x07 },
		{ 0x007b, 0x38 }, { 0x007c, 0x04 },
		{ 0x0079, 0x0a },
		{ 0x0429, 0x0a }, { 0x045d, 0x01 },
		{ 0x0529, 0x0a }, { 0x055d, 0x01 },
		{ 0x060a, 0xb0 }, { 0x060b, 0xff }, { 0x060c, 0xff },
		{ 0x0644, 0x18 }, { 0x0645, 0x01 }, { 0x0646, 0x2d },
	};
	int i;

	for (i = 0; i < ARRAY_SIZE(regs); i++)
		rohm_deserializer_write(rl82, regs[i].reg, regs[i].def);

	/* BL_PWM - GPIO0 */
	rohm_deserializer_write(rl82, 0x0057, 0x00);
	rohm_deserializer_write(rl82, 0x0058, 0x02);

	/* TP_INT - GPIO3 */
	rohm_deserializer_write(rl82, 0x0060, 0x08);
	rohm_deserializer_write(rl82, 0x042e, 0x05);

	/* TP_RST - GPIO4 */
	rohm_deserializer_write(rl82, 0x0063, 0x00);
	rohm_deserializer_write(rl82, 0x0064, 0x01);

	return 0;
}

static int csot_mg1561b01_unprepare(struct bu18rl82 *rl82)
{
	/* TP_RST - GPIO4 */
	rohm_deserializer_write(rl82, 0x0064, 0x00);

	return 0;
}

static int csot_mg1561b01_enable(struct bu18rl82 *rl82)
{
	rohm_deserializer_write(rl82, 0x0091, 0x03);
	rohm_deserializer_write(rl82, 0x0090, 0x01);

	return 0;
}

static int csot_mg1561b01_disable(struct bu18rl82 *rl82)
{
	rohm_deserializer_write(rl82, 0x0090, 0x00);
	rohm_deserializer_write(rl82, 0x0091, 0x00);

	return 0;
}

static int csot_mg1561b01_backlight_enable(struct bu18rl82 *rl82)
{
	/* BL_EN - GPIO1 */
	rohm_deserializer_write(rl82, 0x005a, 0x00);
	rohm_deserializer_write(rl82, 0x005b, 0x01);

	return 0;
}

static int csot_mg1561b01_backlight_disable(struct bu18rl82 *rl82)
{
	/* BL_EN - GPIO1 */
	rohm_deserializer_write(rl82, 0x005b, 0x00);

	return 0;
}

static const struct panel_desc csot_mg1561b01 = {
	.name			= "csot,mg1561b01",
	.prepare		= csot_mg1561b01_prepare,
	.unprepare		= csot_mg1561b01_unprepare,
	.enable			= csot_mg1561b01_enable,
	.disable		= csot_mg1561b01_disable,
	.backlight_enable	= csot_mg1561b01_backlight_enable,
	.backlight_disable	= csot_mg1561b01_backlight_disable,
};

static int touch_china_v123awf3_r1_prepare(struct bu18rl82 *rl82)
{
	const struct reg_sequence {
		u16 reg;
		u8 def;
	} regs[] = {
		{ 0x0011, 0x03 }, { 0x0012, 0x03 },
		{ 0x001f, 0x02 }, { 0x0020, 0x02 },
		{ 0x0031, 0x48 }, { 0x0032, 0x48 },
		{ 0x0073, 0x80 }, { 0x0074, 0x07 },
		{ 0x007b, 0xd0 }, { 0x007c, 0x02 },
		{ 0x0079, 0x0a },
		{ 0x0429, 0x0a }, { 0x045d, 0x01 },
		{ 0x0529, 0x0a }, { 0x055d, 0x01 },
		{ 0x060a, 0xb0 }, { 0x060b, 0xff }, { 0x060c, 0xff },
		{ 0x0644, 0x90 }, { 0x0646, 0xd2 },
	};
	int i;

	for (i = 0; i < ARRAY_SIZE(regs); i++)
		rohm_deserializer_write(rl82, regs[i].reg, regs[i].def);

	/* TP_INT - GPIO4 */
	rohm_deserializer_write(rl82, 0x0063, 0x08);
	rohm_deserializer_write(rl82, 0x042f, 0x06);

	/* TP_RST - GPIO3 */
	rohm_deserializer_write(rl82, 0x0060, 0x00);
	rohm_deserializer_write(rl82, 0x0061, 0x00);

	/* LCD_BIAS_EN - GPIO2 */
	rohm_deserializer_write(rl82, 0x005d, 0x00);
	rohm_deserializer_write(rl82, 0x005e, 0x01);
	mdelay(6);

	return 0;
}

static int touch_china_v123awf3_r1_unprepare(struct bu18rl82 *rl82)
{
	/* LCD_BIAS_EN - GPIO2 */
	rohm_deserializer_write(rl82, 0x005e, 0x00);

	return 0;
}

static int touch_china_v123awf3_r1_enable(struct bu18rl82 *rl82)
{
	rohm_deserializer_write(rl82, 0x0091, 0x03);
	rohm_deserializer_write(rl82, 0x0090, 0x01);

	/* RSEX - GPIO5 */
	rohm_deserializer_write(rl82, 0x0066, 0x00);
	rohm_deserializer_write(rl82, 0x0067, 0x01);
	/* TP_RST - GPIO3 */
	rohm_deserializer_write(rl82, 0x0060, 0x00);
	rohm_deserializer_write(rl82, 0x0061, 0x01);
	mdelay(20);
	/* LCD_PON - GPIO1 */
	rohm_deserializer_write(rl82, 0x005a, 0x00);
	rohm_deserializer_write(rl82, 0x005b, 0x01);

	return 0;
}

static int touch_china_v123awf3_r1_disable(struct bu18rl82 *rl82)
{
	rohm_deserializer_write(rl82, 0x0090, 0x00);
	rohm_deserializer_write(rl82, 0x0091, 0x00);

	/* LCD_PON - GPIO1 */
	rohm_deserializer_write(rl82, 0x005b, 0x00);
	mdelay(100);
	/* TP_RST - GPIO3 */
	rohm_deserializer_write(rl82, 0x0061, 0x00);
	/* RSEX - GPIO5 */
	rohm_deserializer_write(rl82, 0x0067, 0x00);

	return 0;
}

static int touch_china_v123awf3_r1_backlight_enable(struct bu18rl82 *rl82)
{
	/* BL_PWM - GPIO0 */
	rohm_deserializer_write(rl82, 0x0057, 0x00);
	rohm_deserializer_write(rl82, 0x0058, 0x02);

	return 0;
}

static int touch_china_v123awf3_r1_backlight_disable(struct bu18rl82 *rl82)
{
	/* BL_PWM - GPIO0 */
	rohm_deserializer_write(rl82, 0x0058, 0x00);

	return 0;
}

static const struct panel_desc touch_china_v123awf3_r1 = {
	.name			= "touch-china,v123awf3-r1",
	.prepare		= touch_china_v123awf3_r1_prepare,
	.unprepare		= touch_china_v123awf3_r1_unprepare,
	.enable			= touch_china_v123awf3_r1_enable,
	.disable		= touch_china_v123awf3_r1_disable,
	.backlight_enable	= touch_china_v123awf3_r1_backlight_enable,
	.backlight_disable	= touch_china_v123awf3_r1_backlight_disable,
};

static const struct udevice_id bu18rl82_of_match[] = {
	{ .compatible = "csot,mg1561b01", .data = (ulong)&csot_mg1561b01 },
	{ .compatible = "touch-china,v123awf3-r1", .data = (ulong)&touch_china_v123awf3_r1 },
	{}
};

U_BOOT_DRIVER(panel_rohm_bu18rl82) = {
	.name = "panel-rohm-bu18rl82",
	.id = UCLASS_PANEL,
	.of_match = bu18rl82_of_match,
	.probe = bu18rl82_probe,
	.priv_auto_alloc_size = sizeof(struct bu18rl82),
};
