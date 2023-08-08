// SPDX-License-Identifier: GPL-2.0+
/*
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

struct maxim_deserializer_panel;

struct maxim_deserializer_panel_desc {
	const char *name;
	u16 id;
	struct drm_display_mode mode;
	u32 width_mm;
	u32 height_mm;

	struct {
		const char *name;
		u8 addr;
		u8 dev_id;
	} deserializer;

	void (*prepare)(struct maxim_deserializer_panel *p);
	void (*unprepare)(struct maxim_deserializer_panel *p);
	void (*enable)(struct maxim_deserializer_panel *p);
	void (*disable)(struct maxim_deserializer_panel *p);
};

struct maxim_deserializer_panel {
	struct udevice *dev;
	struct udevice *backlight;
	struct gpio_desc enable_gpio;

	/* the panel desc as detected */
	const struct maxim_deserializer_panel_desc *desc;
};

static void maxim_max96752f_panel_prepare(struct maxim_deserializer_panel *p)
{
	pinctrl_select_state(p->dev, p->desc->name ?
			     p->desc->name : p->desc->deserializer.name);

	dm_i2c_reg_write(p->dev, 0x0002, 0x43);
	dm_i2c_reg_write(p->dev, 0x0140, 0x20);

	dm_i2c_reg_write(p->dev, 0x01ce, 0x5e);	/* oldi */
	dm_i2c_reg_write(p->dev, 0x020c, 0x84);	/* bl_pwm */
	dm_i2c_reg_write(p->dev, 0x0206, 0x83);	/* tp_int */

	dm_i2c_reg_write(p->dev, 0x0215, 0x90);	/* lcd_en */
	mdelay(20);
}

static void maxim_max96752f_panel_unprepare(struct maxim_deserializer_panel *p)
{
	dm_i2c_reg_write(p->dev, 0x0215, 0x80);	/* lcd_en */
}

static void maxim_max96752f_panel_enable(struct maxim_deserializer_panel *p)
{
	dm_i2c_reg_write(p->dev, 0x0227, 0x90);	/* lcd_rst */
	mdelay(20);
	dm_i2c_reg_write(p->dev, 0x020f, 0x90);	/* tp_rst */
	mdelay(100);
	dm_i2c_reg_write(p->dev, 0x0221, 0x90);	/* lcd_stb */
	mdelay(60);
	dm_i2c_reg_write(p->dev, 0x0212, 0x90);	/* bl_current_ctl */
	dm_i2c_reg_write(p->dev, 0x0209, 0x90);	/* bl_en */
}

static void maxim_max96752f_panel_disable(struct maxim_deserializer_panel *p)
{
	dm_i2c_reg_write(p->dev, 0x0209, 0x80);	/* bl_en */
	dm_i2c_reg_write(p->dev, 0x0212, 0x80); /* bl_current_ctl */
	dm_i2c_reg_write(p->dev, 0x0221, 0x80);	/* lcd_stb */
	dm_i2c_reg_write(p->dev, 0x020f, 0x80);	/* tp_rst */
	dm_i2c_reg_write(p->dev, 0x0227, 0x80);	/* lcd_rst */
}

static const struct maxim_deserializer_panel_desc maxim_deserializer_default_panels[] = {
	{
		.deserializer = {
			.name = "max96752f",
			.addr = 0x48,
			.dev_id = 0x82,
		},

		.mode = {
			.clock = 148500,
			.hdisplay = 1920,
			.hsync_start = 1920 + 20,
			.hsync_end = 1920 + 20 + 20,
			.htotal = 1920 + 20 + 20 + 20,
			.vdisplay = 1080,
			.vsync_start = 1080 + 250,
			.vsync_end = 1080 + 250 + 2,
			.vtotal = 1080 + 250 + 2 + 8,
			.flags = DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
		},

		.prepare = maxim_max96752f_panel_prepare,
		.enable = maxim_max96752f_panel_enable,
		.disable = maxim_max96752f_panel_disable,
		.unprepare = maxim_max96752f_panel_unprepare,
	},
};

static void maxim_deserializer_panel_prepare(struct rockchip_panel *panel)
{
	struct maxim_deserializer_panel *p = dev_get_priv(panel->dev);

	if (!p->desc)
		return;

	if (p->desc->prepare)
		p->desc->prepare(p);
}

static void maxim_deserializer_panel_unprepare(struct rockchip_panel *panel)
{
	struct maxim_deserializer_panel *p = dev_get_priv(panel->dev);

	if (!p->desc)
		return;

	if (p->desc->unprepare)
		p->desc->unprepare(p);
}

static void maxim_deserializer_panel_enable(struct rockchip_panel *panel)
{
	struct maxim_deserializer_panel *p = dev_get_priv(panel->dev);

	if (!p->desc)
		return;

	if (p->desc->enable)
		p->desc->enable(p);

	if (p->backlight)
		backlight_enable(p->backlight);
}

static void maxim_deserializer_panel_disable(struct rockchip_panel *panel)
{
	struct maxim_deserializer_panel *p = dev_get_priv(panel->dev);

	if (!p->desc)
		return;

	if (p->backlight)
		backlight_disable(p->backlight);

	if (p->desc->disable)
		p->desc->disable(p);
}

static u16 maxim_deserializer_panel_get_id(struct maxim_deserializer_panel *p)
{
	/* TODO */
	return 0;
}

static int maxim_deserializer_panel_detect(struct maxim_deserializer_panel *p)
{
	const struct maxim_deserializer_panel_desc *desc = NULL;
	struct udevice *dev = p->dev;
	struct dm_i2c_chip *chip = dev_get_parent_platdata(dev);
	u16 id = maxim_deserializer_panel_get_id(p);
	unsigned int i, ret;

	if (id) {
		/* TODO */
	} else {
		u8 dev_id;

		for (i = 0; i < ARRAY_SIZE(maxim_deserializer_default_panels); i++) {
			chip->chip_addr = maxim_deserializer_default_panels[i].deserializer.addr;

			ret = dm_i2c_reg_read(dev, 0x000d);
			if (ret < 0)
				continue;

			dev_id = ret;

			if (maxim_deserializer_default_panels[i].deserializer.dev_id == dev_id) {
				desc = &maxim_deserializer_default_panels[i];
				break;
			}
		}
	}

	if (!desc)
		return -ENODEV;

	p->desc = desc;
	chip->chip_addr = desc->deserializer.addr;

	return 0;
}

static int maxim_deserializer_panel_get_mode(struct rockchip_panel *panel,
					     struct drm_display_mode *mode)
{
	struct maxim_deserializer_panel *p = dev_get_priv(panel->dev);
	int ret;

	ret = maxim_deserializer_panel_detect(p);
	if (ret)
		return ret;

	memcpy(mode, &p->desc->mode, sizeof(struct drm_display_mode));
	mode->vrefresh = drm_mode_vrefresh(mode);

	return 0;
}

static const struct rockchip_panel_funcs maxim_deserializer_panel_funcs = {
	.prepare = maxim_deserializer_panel_prepare,
	.unprepare = maxim_deserializer_panel_unprepare,
	.enable = maxim_deserializer_panel_enable,
	.disable = maxim_deserializer_panel_disable,
	.get_mode = maxim_deserializer_panel_get_mode,
};

static void maxim_deserializer_panel_power_on(struct maxim_deserializer_panel *p)
{
	if (dm_gpio_is_valid(&p->enable_gpio)) {
		dm_gpio_set_value(&p->enable_gpio, 1);
		mdelay(500);
	}
}

static int maxim_deserializer_panel_probe(struct udevice *dev)
{
	struct maxim_deserializer_panel *p = dev_get_priv(dev);
	struct rockchip_panel *panel;
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	p->dev = dev;

	ret = uclass_get_device_by_phandle(UCLASS_PANEL_BACKLIGHT, dev,
					   "backlight", &p->backlight);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "%s: Cannot get backlight: %d\n", __func__, ret);
		return ret;
	}

	ret = gpio_request_by_name(dev, "enable-gpios", 0,
				   &p->enable_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "%s: Cannot get enable GPIO: %d\n", __func__, ret);
		return ret;
	}

	maxim_deserializer_panel_power_on(p);

	panel = calloc(1, sizeof(*panel));
	if (!panel)
		return -ENOMEM;

	dev->driver_data = (ulong)panel;
	panel->dev = dev;
	panel->bus_format = MEDIA_BUS_FMT_RGB888_1X24;
	panel->funcs = &maxim_deserializer_panel_funcs;

	return 0;
}

static const struct udevice_id maxim_deserializer_panel_of_match[] = {
	{ .compatible = "maxim,deserializer-panel", },
	{}
};

U_BOOT_DRIVER(maxim_deserializer_panel) = {
	.name = "maxim_deserializer_panel",
	.id = UCLASS_PANEL,
	.of_match = maxim_deserializer_panel_of_match,
	.probe = maxim_deserializer_panel_probe,
	.priv_auto_alloc_size = sizeof(struct maxim_deserializer_panel),
};
