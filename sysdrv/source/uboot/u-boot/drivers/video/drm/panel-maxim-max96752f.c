// SPDX-License-Identifier: GPL-2.0+
/*
 * Maxim MAX96752F GMSL2 Deserializer
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

struct max96752f;

struct panel_desc {
	const char *name;
	int (*prepare)(struct max96752f *max96752f);
	int (*unprepare)(struct max96752f *max96752f);
	int (*enable)(struct max96752f *max96752f);
	int (*disable)(struct max96752f *max96752f);
	int (*backlight_enable)(struct max96752f *max96752f);
	int (*backlight_disable)(struct max96752f *max96752f);
};

struct max96752f {
	struct udevice *dev;
	struct udevice *serializer;
	struct udevice *backlight;

	const struct panel_desc *desc;
};

static void max96752f_panel_prepare(struct rockchip_panel *panel)
{
	struct max96752f *max96752f = dev_get_priv(panel->dev);
	const struct panel_desc *desc = max96752f->desc;

	if (desc->prepare)
		desc->prepare(max96752f);
}

static void max96752f_panel_unprepare(struct rockchip_panel *panel)
{
	struct max96752f *max96752f = dev_get_priv(panel->dev);
	const struct panel_desc *desc = max96752f->desc;

	if (desc->unprepare)
		desc->unprepare(max96752f);
}

static void max96752f_panel_enable(struct rockchip_panel *panel)
{
	struct max96752f *max96752f = dev_get_priv(panel->dev);
	const struct panel_desc *desc = max96752f->desc;

	if (desc->enable)
		desc->enable(max96752f);

	if (max96752f->backlight)
		backlight_enable(max96752f->backlight);

	if (desc->backlight_enable)
		desc->backlight_enable(max96752f);
}

static void max96752f_panel_disable(struct rockchip_panel *panel)
{
	struct max96752f *max96752f = dev_get_priv(panel->dev);
	const struct panel_desc *desc = max96752f->desc;

	if (desc->backlight_disable)
		desc->backlight_disable(max96752f);

	if (max96752f->backlight)
		backlight_disable(max96752f->backlight);

	if (desc->disable)
		desc->disable(max96752f);
}

static const struct rockchip_panel_funcs max96752f_panel_funcs = {
	.prepare = max96752f_panel_prepare,
	.unprepare = max96752f_panel_unprepare,
	.enable = max96752f_panel_enable,
	.disable = max96752f_panel_disable,
};

static int max96752f_probe(struct udevice *dev)
{
	struct max96752f *max96752f = dev_get_priv(dev);
	struct rockchip_panel *panel;
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	max96752f->dev = dev;
	max96752f->serializer = dev->parent->parent;
	max96752f->desc = (const struct panel_desc *)dev_get_driver_data(dev);

	ret = uclass_get_device_by_phandle(UCLASS_PANEL_BACKLIGHT, dev,
					   "backlight", &max96752f->backlight);
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
	panel->funcs = &max96752f_panel_funcs;

	return 0;
}

#define maxim_serializer_write(max96752f, reg, val) do {	\
		int ret;					\
		ret = dm_i2c_reg_write(max96752f->serializer,	\
				       reg, val);		\
		if (ret)					\
			return ret;				\
	} while (0)

#define maxim_deserializer_write(max96752f, reg, val) do {	\
		int ret;					\
		ret = dm_i2c_reg_write(max96752f->dev,		\
				       reg, val);		\
		if (ret)					\
			return ret;				\
	} while (0)

static int boe_av156fht_l83_panel_prepare(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0002, 0x43);
	maxim_deserializer_write(max96752f, 0x0140, 0x20);

	maxim_deserializer_write(max96752f, 0x01ce, 0x5e);	/* oldi */
	maxim_deserializer_write(max96752f, 0x020c, 0x84);	/* bl_pwm */
	maxim_deserializer_write(max96752f, 0x0206, 0x83);	/* tp_int */

	maxim_deserializer_write(max96752f, 0x0215, 0x90);	/* lcd_en */
	mdelay(20);

	return 0;
}

static int boe_av156fht_l83_panel_unprepare(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0215, 0x80);	/* lcd_en */

	return 0;
}

static int boe_av156fht_l83_panel_enable(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0227, 0x90);	/* lcd_rst */
	mdelay(20);
	maxim_deserializer_write(max96752f, 0x020f, 0x90);	/* tp_rst */
	mdelay(100);
	maxim_deserializer_write(max96752f, 0x0221, 0x90);	/* lcd_stb */
	mdelay(60);
	maxim_deserializer_write(max96752f, 0x0212, 0x90);	/* bl_current_ctl */
	maxim_deserializer_write(max96752f, 0x0209, 0x90);	/* bl_en */

	return 0;
}

static int boe_av156fht_l83_panel_disable(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0209, 0x80);	/* bl_en */
	maxim_deserializer_write(max96752f, 0x0212, 0x80);	/* bl_current_ctl */
	maxim_deserializer_write(max96752f, 0x0221, 0x80);	/* lcd_stb */
	maxim_deserializer_write(max96752f, 0x020f, 0x80);	/* tp_rst */
	maxim_deserializer_write(max96752f, 0x0227, 0x80);	/* lcd_rst */

	return 0;
}

static int boe_av156fht_l83_panel_backlight_enable(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0212, 0x90);	/* bl_current_ctl */
	maxim_deserializer_write(max96752f, 0x0209, 0x90);	/* bl_en */

	return 0;
}

static int boe_av156fht_l83_panel_backlight_disable(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0209, 0x80);	/* bl_en */
	maxim_deserializer_write(max96752f, 0x0212, 0x80);	/* bl_current_ctl */

	return 0;
}

static const struct panel_desc boe_av156fht_l83 = {
	.name			= "boe-av156fht-l83",
	.prepare		= boe_av156fht_l83_panel_prepare,
	.unprepare		= boe_av156fht_l83_panel_unprepare,
	.enable			= boe_av156fht_l83_panel_enable,
	.disable		= boe_av156fht_l83_panel_disable,
	.backlight_enable	= boe_av156fht_l83_panel_backlight_enable,
	.backlight_disable	= boe_av156fht_l83_panel_backlight_disable,
};

static int hannstar_hsd123jpw3_a15_prepare(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0002, 0x43);
	maxim_deserializer_write(max96752f, 0x0140, 0x20);
	maxim_deserializer_write(max96752f, 0x01ce, 0x5e);

	maxim_deserializer_write(max96752f, 0x0203, 0x83);	/* GPIO1  <- TP_INT */
	maxim_deserializer_write(max96752f, 0x0206, 0x84);      /* GPIO2  -> TP_RST */
	maxim_deserializer_write(max96752f, 0x0224, 0x84);	/* GPIO12 -> LCD_BL_PWM */

	return 0;
}

static int hannstar_hsd123jpw3_a15_unprepare(struct max96752f *max96752f)
{
	return 0;
}

static int hannstar_hsd123jpw3_a15_enable(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0221, 0x10);	/* GPIO11 -> LCD_RESET */
	mdelay(20);

	return 0;
}

static int hannstar_hsd123jpw3_a15_disable(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0221, 0x00);	/* GPIO11 -> LCD_RESET */
	mdelay(20);

	return 0;
}

static const struct panel_desc hannstar_hsd123jpw3_a15 = {
	.name			= "hannstar,hsd123jpw3-a15",
	.prepare		= hannstar_hsd123jpw3_a15_prepare,
	.unprepare		= hannstar_hsd123jpw3_a15_unprepare,
	.enable			= hannstar_hsd123jpw3_a15_enable,
	.disable		= hannstar_hsd123jpw3_a15_disable,
};

static int ogm_101fhbllm01_prepare(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x01ce, 0x5e);

	maxim_deserializer_write(max96752f, 0x0203, 0x84);	/* GPIO1 -> BL_PWM */
	maxim_deserializer_write(max96752f, 0x0206, 0x84);	/* GPIO2 -> TP_RST */
	maxim_deserializer_write(max96752f, 0x0209, 0x83);	/* GPIO3 <- TP_INT */

	maxim_deserializer_write(max96752f, 0x0001, 0x02);

	return 0;
}

static int ogm_101fhbllm01_unprepare(struct max96752f *max96752f)
{
	maxim_deserializer_write(max96752f, 0x0001, 0x01);

	return 0;
}

static const struct panel_desc ogm_101fhbllm01 = {
	.name			= "ogm,101fhbllm01",
	.prepare		= ogm_101fhbllm01_prepare,
	.unprepare		= ogm_101fhbllm01_unprepare,
};

static const struct udevice_id max96752f_of_match[] = {
	{ .compatible = "boe,av156fht-l83", .data = (ulong)&boe_av156fht_l83 },
	{ .compatible = "hannstar,hsd123jpw3-a15", .data = (ulong)&hannstar_hsd123jpw3_a15 },
	{ .compatible = "ogm,101fhbllm01", .data = (ulong)&ogm_101fhbllm01 },
	{}
};

U_BOOT_DRIVER(max96752f) = {
	.name = "max96752f",
	.id = UCLASS_PANEL,
	.of_match = max96752f_of_match,
	.probe = max96752f_probe,
	.priv_auto_alloc_size = sizeof(struct max96752f),
};
