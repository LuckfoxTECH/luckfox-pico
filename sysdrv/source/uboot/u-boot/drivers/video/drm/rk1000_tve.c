// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <boot_rkimg.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <video_bridge.h>
#include <asm/io.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <linux/media-bus-format.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

#include "rk1000.h"

#define TVE_POWCR	0x03
#define TVE_OFF		0X07
#define TVE_ON		0x03

struct rk1000_tve {
	struct udevice *dev;
	struct rk1000_ctl rk1000_ctl;
};

enum {
	CVBS_NTSC = 0,
	CVBS_PAL,
};

int rk1000_tve_i2c_write(struct rk1000_tve *rk1000_tve, u8 reg, u8 val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk1000_tve->dev);
	struct i2c_msg msg;
	u8 buf[2];
	int ret;

	buf[0] = reg;
	buf[1] = val;
	msg.addr = chip->chip_addr;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = buf;

	ret = dm_i2c_xfer(rk1000_tve->dev, &msg, 1);
	if (ret) {
		dev_err(rk1000_tve->dev,
			"rk1000 tve i2c write failed: %d\n", ret);
		return ret;
	}

	return 0;
}

int rk1000_tve_i2c_read(struct rk1000_tve *rk1000_tve, u8 reg, u8 *val)
{
	struct dm_i2c_chip *chip = dev_get_parent_platdata(rk1000_tve->dev);
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

	ret = dm_i2c_xfer(rk1000_tve->dev, msg, 2);
	if (ret) {
		dev_err(rk1000_tve->dev,
			"rk1000 tve i2c read failed: %d\n", ret);
		return ret;
	}

	*val = data;

	return 0;
}

static int rk1000_tv_write_block(struct rk1000_tve *rk1000_tve,
				 u8 reg, const u8 *buf, u8 len)
{
	int i, ret;

	for (i = 0; i < len; i++) {
		ret = rk1000_tve_i2c_write(rk1000_tve, reg + i, buf[i]);
		if (ret)
			break;
	}

	return ret;
}

static int rk1000_tve_probe(struct udevice *dev)
{
	struct rk1000_tve *rk1000_tve = dev_get_priv(dev);
	struct rockchip_bridge *bridge =
		(struct rockchip_bridge *)dev_get_driver_data(dev);
	int ret;

	rk1000_tve->dev = dev;

	ret = uclass_get_device_by_name(UCLASS_I2C_GENERIC,
					"rk1000-ctl@40",
					&rk1000_tve->rk1000_ctl.dev);
	if (ret)
		return ret;

	bridge->dev = dev;

	return 0;
}

static void rk1000_tve_bridge_enable(struct rockchip_bridge *bridge)
{
	u8 tv_encoder_regs_pal[] = {0x06, 0x00, 0x00, 0x03, 0x00, 0x00};
	u8 tv_encoder_control_regs_pal[] = {0x41, 0x01};
	u8 tv_encoder_regs_ntsc[] = {0x00, 0x00, 0x00, 0x03, 0x00, 0x00};
	u8 tv_encoder_control_regs_ntsc[] = {0x43, 0x01};
	char data[4] = {0x88, 0x00, 0x22, 0x00};
	struct rk1000_tve *rk1000_tve = dev_get_priv(bridge->dev);
	struct connector_state *conn_state = &bridge->state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct rk1000_ctl *rk1000_ctl = &rk1000_tve->rk1000_ctl;

	rk1000_ctl_write_block(rk1000_ctl, 0, (u8 *)data, 4);

	/* rk1000 power down output dac */
	data[0] = 0x07;
	rk1000_tv_write_block(rk1000_tve, 0x03, (u8 *)data, 1);

	if (mode->vdisplay == 576) {
		rk1000_tv_write_block(rk1000_tve, 0, tv_encoder_regs_pal,
				      sizeof(tv_encoder_regs_pal));
		rk1000_ctl_write_block(rk1000_ctl, 3,
				       tv_encoder_control_regs_pal,
				       sizeof(tv_encoder_control_regs_pal));
	} else {
		rk1000_tv_write_block(rk1000_tve, 0, tv_encoder_regs_ntsc,
				      sizeof(tv_encoder_regs_ntsc));
		rk1000_ctl_write_block(rk1000_ctl, 3,
				       tv_encoder_control_regs_ntsc,
				       sizeof(tv_encoder_control_regs_ntsc));
	}
}

static void rk1000_tve_bridge_disable(struct rockchip_bridge *bridge)
{
	struct rk1000_tve *rk1000_tve = dev_get_priv(bridge->dev);
	char data[1] = {0x07};

	/* rk1000 power down output dac */
	rk1000_tv_write_block(rk1000_tve, 0x03, (u8 *)data, 1);
}

static void drm_rk1000_select_output(struct overscan *overscan,
				     struct drm_display_mode *mode)
{
	char baseparameter_buf[8 * RK_BLK_SIZE] __aligned(ARCH_DMA_MINALIGN);
	struct base_screen_info *screen_info = NULL;
	struct base_disp_info base_parameter;
	struct blk_desc *dev_desc;
	const struct base_overscan *scan;
	disk_partition_t part_info;
	int ret, i, screen_size;
	int max_scan = 100;
	int min_scan = 51;

	overscan->left_margin = max_scan;
	overscan->right_margin = max_scan;
	overscan->top_margin = max_scan;
	overscan->bottom_margin = max_scan;

	mode->hdisplay = 720;
	mode->hsync_start = 732;
	mode->hsync_end = 738;
	mode->htotal = 864;
	mode->vdisplay = 576;
	mode->vsync_start = 582;
	mode->vsync_end = 588;
	mode->vtotal = 625;
	mode->clock = 27000;
	mode->flags = DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC;

	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: Could not find device\n", __func__);
		return;
	}

	if (part_get_info_by_name(dev_desc, "baseparameter", &part_info) < 0) {
		printf("Could not find baseparameter partition\n");
		return;
	}

	ret = blk_dread(dev_desc, part_info.start, 1,
			(void *)baseparameter_buf);
	if (ret < 0) {
		printf("read baseparameter failed\n");
		return;
	}

	memcpy(&base_parameter, baseparameter_buf, sizeof(base_parameter));
	scan = &base_parameter.scan;

	screen_size = sizeof(base_parameter.screen_list) /
		sizeof(base_parameter.screen_list[0]);

	for (i = 0; i < screen_size; i++) {
		if (base_parameter.screen_list[i].type ==
		    DRM_MODE_CONNECTOR_TV) {
			screen_info = &base_parameter.screen_list[i];
			break;
		}
	}

	if (scan->leftscale < min_scan && scan->leftscale > 0)
		overscan->left_margin = min_scan;
	else if (scan->leftscale < max_scan)
		overscan->left_margin = scan->leftscale;

	if (scan->rightscale < min_scan && scan->rightscale > 0)
		overscan->right_margin = min_scan;
	else if (scan->rightscale < max_scan)
		overscan->right_margin = scan->rightscale;

	if (scan->topscale < min_scan && scan->topscale > 0)
		overscan->top_margin = min_scan;
	else if (scan->topscale < max_scan)
		overscan->top_margin = scan->topscale;

	if (scan->bottomscale < min_scan && scan->bottomscale > 0)
		overscan->bottom_margin = min_scan;
	else if (scan->bottomscale < max_scan)
		overscan->bottom_margin = scan->bottomscale;

	if (screen_info &&
	    (screen_info->mode.hdisplay == 720 &&
	     screen_info->mode.hsync_end == 742 &&
	     screen_info->mode.vdisplay == 480)) {
		mode->hdisplay = 720;
		mode->hsync_start = 736;
		mode->hsync_end = 742;
		mode->htotal = 858;
		mode->vdisplay = 480;
		mode->vsync_start = 494;
		mode->vsync_end = 500;
		mode->vtotal = 525;
		mode->clock = 27000;
	} else {
		mode->hdisplay = 720;
		mode->hsync_start = 732;
		mode->hsync_end = 738;
		mode->htotal = 864;
		mode->vdisplay = 576;
		mode->vsync_start = 582;
		mode->vsync_end = 588;
		mode->vtotal = 625;
		mode->clock = 27000;
	}
}

static int rk1000_tve_get_timing(struct udevice *dev)
{
	struct rockchip_bridge *bridge =
		(struct rockchip_bridge *)dev_get_driver_data(dev);
	struct connector_state *conn_state = &bridge->state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct overscan *overscan = &conn_state->overscan;

	drm_rk1000_select_output(overscan, mode);

	return 0;
}

static const struct rockchip_bridge_funcs rk1000_tve_bridge_funcs = {
	.enable = rk1000_tve_bridge_enable,
	.disable = rk1000_tve_bridge_disable,
};

static struct rockchip_bridge rk1000_tve_driver_data = {
	.funcs = &rk1000_tve_bridge_funcs,
};

struct video_bridge_ops rk1000_tve_ops = {
	.get_timing = rk1000_tve_get_timing,
};

static const struct udevice_id rk1000_tve_ids[] = {
	{
		.compatible = "rockchip,rk1000-tve",
		.data = (ulong)&rk1000_tve_driver_data,
	},
	{ }
};

U_BOOT_DRIVER(rk1000_tve) = {
	.name = "rk1000_tve",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = rk1000_tve_ids,
	.ops = &rk1000_tve_ops,
	.probe = rk1000_tve_probe,
	.priv_auto_alloc_size = sizeof(struct rk1000_tve),
};
