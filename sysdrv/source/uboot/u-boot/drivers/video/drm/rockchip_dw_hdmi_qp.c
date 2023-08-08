// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
 * Author: Algea Cao <algea.cao@rock-chips.com>
 */

#include <common.h>
#include <boot_rkimg.h>
#include <clk.h>
#include <fdtdec.h>
#include <regmap.h>
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/io.h>
#include <asm-generic/gpio.h>
#include <dm/of_access.h>
#include <dm/device.h>
#include <linux/dw_hdmi.h>
#include <linux/hdmi.h>
#include <linux/media-bus-format.h>
#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"
#include "dw_hdmi_qp.h"
#include "rockchip_dw_hdmi_qp.h"

#define HIWORD_UPDATE(val, mask)	((val) | (mask) << 16)

#define RK3588_GRF_SOC_CON2		0x0308
#define RK3588_HDMI1_HPD_INT_MSK	BIT(15)
#define RK3588_HDMI1_HPD_INT_CLR	BIT(14)
#define RK3588_HDMI0_HPD_INT_MSK	BIT(13)
#define RK3588_HDMI0_HPD_INT_CLR	BIT(12)
#define RK3588_GRF_SOC_CON7		0x031c
#define RK3588_SET_HPD_PATH_MASK	(0x3 << 12)
#define RK3588_GRF_SOC_STATUS1		0x0384
#define RK3588_HDMI0_LOW_MORETHAN100MS	BIT(20)
#define RK3588_HDMI0_HPD_PORT_LEVEL	BIT(19)
#define RK3588_HDMI0_IHPD_PORT		BIT(18)
#define RK3588_HDMI0_OHPD_INT		BIT(17)
#define RK3588_HDMI0_LEVEL_INT		BIT(16)
#define RK3588_HDMI0_INTR_CHANGE_CNT	(0x7 << 13)
#define RK3588_HDMI1_LOW_MORETHAN100MS	BIT(28)
#define RK3588_HDMI1_HPD_PORT_LEVEL	BIT(27)
#define RK3588_HDMI1_IHPD_PORT		BIT(26)
#define RK3588_HDMI1_OHPD_INT		BIT(25)
#define RK3588_HDMI1_LEVEL_INT		BIT(24)
#define RK3588_HDMI1_INTR_CHANGE_CNT	(0x7 << 21)

#define RK3588_GRF_VO1_CON3		0x000c
#define RK3588_COLOR_FORMAT_MASK	0xf
#define RK3588_YUV444			0x2
#define RK3588_YUV420			0x3
#define RK3588_COMPRESSED_DATA		0xb
#define RK3588_COLOR_DEPTH_MASK		(0xf << 4)
#define RK3588_8BPC			0
#define RK3588_10BPC			(0x6 << 4)
#define RK3588_CECIN_MASK		BIT(8)
#define RK3588_SCLIN_MASK		BIT(9)
#define RK3588_SDAIN_MASK		BIT(10)
#define RK3588_MODE_MASK		BIT(11)
#define RK3588_COMPRESS_MODE_MASK	BIT(12)
#define RK3588_I2S_SEL_MASK		BIT(13)
#define RK3588_SPDIF_SEL_MASK		BIT(14)
#define RK3588_GRF_VO1_CON4		0x0010
#define RK3588_HDMI21_MASK		BIT(0)
#define RK3588_GRF_VO1_CON9		0x0024
#define RK3588_HDMI0_GRANT_SEL		BIT(10)
#define RK3588_HDMI0_GRANT_SW		BIT(11)
#define RK3588_HDMI1_GRANT_SEL		BIT(12)
#define RK3588_HDMI1_GRANT_SW		BIT(13)
#define RK3588_GRF_VO1_CON6		0x0018
#define RK3588_GRF_VO1_CON7		0x001c

#define PPS_TABLE_LEN 8

#define COLOR_DEPTH_10BIT		BIT(31)
#define HDMI_FRL_MODE			BIT(30)
#define HDMI_EARC_MODE			BIT(29)
#define DATA_RATE_MASK			0xFFFFFFF

#define HDMI20_MAX_RATE			600000
#define HDMI_8K60_RATE			2376000

enum device_type {
	RK3588_HDMI
};

struct pps_data {
	u32 pic_width;
	u32 pic_height;
	u32 slice_width;
	u32 slice_height;
	bool convert_rgb;
	u8 bpc;
	u8 bpp;
	u8 raw_pps[128];
};

enum hdmi_frl_rate_per_lane {
	FRL_12G_PER_LANE = 12,
	FRL_10G_PER_LANE = 10,
	FRL_8G_PER_LANE = 8,
	FRL_6G_PER_LANE = 6,
	FRL_3G_PER_LANE = 3,
};

struct rockchip_hdmi {
	struct rockchip_connector connector;
	int id;
	struct udevice *dev;
	struct regmap *regmap;
	struct regmap *vo1_regmap;
	void *grf;
	void *vo1_grf;

	unsigned long bus_format;
	unsigned long output_bus_format;
	unsigned long enc_out_encoding;

	u8 max_frl_rate_per_lane;
	u8 max_lanes;
	bool allm_en;
	u32 bus_width;
	struct drm_hdmi_dsc_cap dsc_cap;
	struct dw_hdmi_link_config link_cfg;

	struct clk link_clk;

	struct gpio_desc enable_gpio;
};

/*
 * Selected Rate Control Related Parameter Recommended Values
 * from DSC_v1.11 spec & C Model release: DSC_model_20161212
 */
static struct pps_data pps_datas[PPS_TABLE_LEN] = {
	{
		/* 7680x4320/960X96 rgb 8bpc 12bpp */
		7680, 4320, 960, 96, 1, 8, 192,
		{
			0x12, 0x00, 0x00, 0x8d, 0x30, 0xc0, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x05, 0xa0,
			0x01, 0x55, 0x03, 0x90, 0x00, 0x0a, 0x05, 0xc9,
			0x00, 0xa0, 0x00, 0x0f, 0x01, 0x44, 0x01, 0xaa,
			0x08, 0x00, 0x10, 0xf4, 0x03, 0x0c, 0x20, 0x00,
			0x06, 0x0b, 0x0b, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x00, 0x82, 0x00, 0xc0, 0x09, 0x00,
			0x09, 0x7e, 0x19, 0xbc, 0x19, 0xba, 0x19, 0xf8,
			0x1a, 0x38, 0x1a, 0x38, 0x1a, 0x76, 0x2a, 0x76,
			0x2a, 0x76, 0x2a, 0x74, 0x3a, 0xb4, 0x52, 0xf4,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 8bpc 11bpp */
		7680, 4320, 960, 96, 1, 8, 176,
		{
			0x12, 0x00, 0x00, 0x8d, 0x30, 0xb0, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x05, 0x28,
			0x01, 0x74, 0x03, 0x40, 0x00, 0x0f, 0x06, 0xe0,
			0x00, 0x2d, 0x00, 0x0f, 0x01, 0x44, 0x01, 0x33,
			0x0f, 0x00, 0x10, 0xf4, 0x03, 0x0c, 0x20, 0x00,
			0x06, 0x0b, 0x0b, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x00, 0x82, 0x01, 0x00, 0x09, 0x40,
			0x09, 0xbe, 0x19, 0xfc, 0x19, 0xfa, 0x19, 0xf8,
			0x1a, 0x38, 0x1a, 0x38, 0x1a, 0x76, 0x2a, 0x76,
			0x2a, 0x76, 0x2a, 0xb4, 0x3a, 0xb4, 0x52, 0xf4,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 8bpc 10bpp */
		7680, 4320, 960, 96, 1, 8, 160,
		{
			0x12, 0x00, 0x00, 0x8d, 0x30, 0xa0, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x04, 0xb0,
			0x01, 0x9a, 0x02, 0xe0, 0x00, 0x19, 0x09, 0xb0,
			0x00, 0x12, 0x00, 0x0f, 0x01, 0x44, 0x00, 0xbb,
			0x16, 0x00, 0x10, 0xec, 0x03, 0x0c, 0x20, 0x00,
			0x06, 0x0b, 0x0b, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x00, 0xc2, 0x01, 0x00, 0x09, 0x40,
			0x09, 0xbe, 0x19, 0xfc, 0x19, 0xfa, 0x19, 0xf8,
			0x1a, 0x38, 0x1a, 0x78, 0x1a, 0x76, 0x2a, 0xb6,
			0x2a, 0xb6, 0x2a, 0xf4, 0x3a, 0xf4, 0x5b, 0x34,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 8bpc 9bpp */
		7680, 4320, 960, 96, 1, 8, 144,
		{
			0x12, 0x00, 0x00, 0x8d, 0x30, 0x90, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x04, 0x38,
			0x01, 0xc7, 0x03, 0x16, 0x00, 0x1c, 0x08, 0xc7,
			0x00, 0x10, 0x00, 0x0f, 0x01, 0x44, 0x00, 0xaa,
			0x17, 0x00, 0x10, 0xf1, 0x03, 0x0c, 0x20, 0x00,
			0x06, 0x0b, 0x0b, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x00, 0xc2, 0x01, 0x00, 0x09, 0x40,
			0x09, 0xbe, 0x19, 0xfc, 0x19, 0xfa, 0x19, 0xf8,
			0x1a, 0x38, 0x1a, 0x78, 0x1a, 0x76, 0x2a, 0xb6,
			0x2a, 0xb6, 0x2a, 0xf4, 0x3a, 0xf4, 0x63, 0x74,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 10bpc 12bpp */
		7680, 4320, 960, 96, 1, 10, 192,
		{
			0x12, 0x00, 0x00, 0xad, 0x30, 0xc0, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x05, 0xa0,
			0x01, 0x55, 0x03, 0x90, 0x00, 0x0a, 0x05, 0xc9,
			0x00, 0xa0, 0x00, 0x0f, 0x01, 0x44, 0x01, 0xaa,
			0x08, 0x00, 0x10, 0xf4, 0x07, 0x10, 0x20, 0x00,
			0x06, 0x0f, 0x0f, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x01, 0x02, 0x11, 0x80, 0x22, 0x00,
			0x22, 0x7e, 0x32, 0xbc, 0x32, 0xba, 0x3a, 0xf8,
			0x3b, 0x38, 0x3b, 0x38, 0x3b, 0x76, 0x4b, 0x76,
			0x4b, 0x76, 0x4b, 0x74, 0x5b, 0xb4, 0x73, 0xf4,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 10bpc 11bpp */
		7680, 4320, 960, 96, 1, 10, 176,
		{
			0x12, 0x00, 0x00, 0xad, 0x30, 0xb0, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x05, 0x28,
			0x01, 0x74, 0x03, 0x40, 0x00, 0x0f, 0x06, 0xe0,
			0x00, 0x2d, 0x00, 0x0f, 0x01, 0x44, 0x01, 0x33,
			0x0f, 0x00, 0x10, 0xf4, 0x07, 0x10, 0x20, 0x00,
			0x06, 0x0f, 0x0f, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x01, 0x42, 0x19, 0xc0, 0x2a, 0x40,
			0x2a, 0xbe, 0x3a, 0xfc, 0x3a, 0xfa, 0x3a, 0xf8,
			0x3b, 0x38, 0x3b, 0x38, 0x3b, 0x76, 0x4b, 0x76,
			0x4b, 0x76, 0x4b, 0xb4, 0x5b, 0xb4, 0x73, 0xf4,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 10bpc 10bpp */
		7680, 4320, 960, 96, 1, 10, 160,
		{
			0x12, 0x00, 0x00, 0xad, 0x30, 0xa0, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x04, 0xb0,
			0x01, 0x9a, 0x02, 0xe0, 0x00, 0x19, 0x09, 0xb0,
			0x00, 0x12, 0x00, 0x0f, 0x01, 0x44, 0x00, 0xbb,
			0x16, 0x00, 0x10, 0xec, 0x07, 0x10, 0x20, 0x00,
			0x06, 0x0f, 0x0f, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x01, 0xc2, 0x22, 0x00, 0x2a, 0x40,
			0x2a, 0xbe, 0x3a, 0xfc, 0x3a, 0xfa, 0x3a, 0xf8,
			0x3b, 0x38, 0x3b, 0x78, 0x3b, 0x76, 0x4b, 0xb6,
			0x4b, 0xb6, 0x4b, 0xf4, 0x63, 0xf4, 0x7c, 0x34,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
	{
		/* 7680x4320/960X96 rgb 10bpc 9bpp */
		7680, 4320, 960, 96, 1, 10, 144,
		{
			0x12, 0x00, 0x00, 0xad, 0x30, 0x90, 0x10, 0xe0,
			0x1e, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x04, 0x38,
			0x01, 0xc7, 0x03, 0x16, 0x00, 0x1c, 0x08, 0xc7,
			0x00, 0x10, 0x00, 0x0f, 0x01, 0x44, 0x00, 0xaa,
			0x17, 0x00, 0x10, 0xf1, 0x07, 0x10, 0x20, 0x00,
			0x06, 0x0f, 0x0f, 0x33, 0x0e, 0x1c, 0x2a, 0x38,
			0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7b,
			0x7d, 0x7e, 0x01, 0xc2, 0x22, 0x00, 0x2a, 0x40,
			0x2a, 0xbe, 0x3a, 0xfc, 0x3a, 0xfa, 0x3a, 0xf8,
			0x3b, 0x38, 0x3b, 0x78, 0x3b, 0x76, 0x4b, 0xb6,
			0x4b, 0xb6, 0x4b, 0xf4, 0x63, 0xf4, 0x84, 0x74,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
	},
};

static bool hdmi_bus_fmt_is_rgb(unsigned int bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_RGB101010_1X30:
	case MEDIA_BUS_FMT_RGB121212_1X36:
	case MEDIA_BUS_FMT_RGB161616_1X48:
		return true;

	default:
		return false;
	}
}

static bool hdmi_bus_fmt_is_yuv444(unsigned int bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_YUV10_1X30:
	case MEDIA_BUS_FMT_YUV12_1X36:
	case MEDIA_BUS_FMT_YUV16_1X48:
		return true;

	default:
		return false;
	}
}

static bool hdmi_bus_fmt_is_yuv422(unsigned int bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_UYVY10_1X20:
	case MEDIA_BUS_FMT_UYVY12_1X24:
		return true;

	default:
		return false;
	}
}

static bool hdmi_bus_fmt_is_yuv420(unsigned int bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
	case MEDIA_BUS_FMT_UYYVYY12_0_5X36:
	case MEDIA_BUS_FMT_UYYVYY16_0_5X48:
		return true;

	default:
	return false;
	}
}

static int hdmi_bus_fmt_color_depth(unsigned int bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
		return 8;

	case MEDIA_BUS_FMT_RGB101010_1X30:
	case MEDIA_BUS_FMT_YUV10_1X30:
	case MEDIA_BUS_FMT_UYVY10_1X20:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
		return 10;

	case MEDIA_BUS_FMT_RGB121212_1X36:
	case MEDIA_BUS_FMT_YUV12_1X36:
	case MEDIA_BUS_FMT_UYVY12_1X24:
	case MEDIA_BUS_FMT_UYYVYY12_0_5X36:
		return 12;

	case MEDIA_BUS_FMT_RGB161616_1X48:
	case MEDIA_BUS_FMT_YUV16_1X48:
	case MEDIA_BUS_FMT_UYYVYY16_0_5X48:
		return 16;

	default:
		return 0;
	}
}

static unsigned int
hdmi_get_tmdsclock(struct rockchip_hdmi *hdmi, unsigned long pixelclock)
{
	unsigned int tmdsclock = pixelclock;
	unsigned int depth =
		hdmi_bus_fmt_color_depth(hdmi->output_bus_format);

	if (!hdmi_bus_fmt_is_yuv422(hdmi->output_bus_format)) {
		switch (depth) {
		case 16:
			tmdsclock = pixelclock * 2;
			break;
		case 12:
			tmdsclock = pixelclock * 3 / 2;
			break;
		case 10:
			tmdsclock = pixelclock * 5 / 4;
			break;
		default:
			break;
		}
	}

	return tmdsclock;
}

static void hdmi_select_link_config(struct rockchip_hdmi *hdmi,
				    struct drm_display_mode *mode,
				    unsigned int tmdsclk)
{
	int max_lanes, max_rate_per_lane;
	int max_dsc_lanes, max_dsc_rate_per_lane;
	unsigned long max_frl_rate;

	max_lanes = hdmi->max_lanes;
	max_rate_per_lane = hdmi->max_frl_rate_per_lane;
	max_frl_rate = max_lanes * max_rate_per_lane * 1000000;

	hdmi->link_cfg.dsc_mode = false;
	hdmi->link_cfg.frl_lanes = max_lanes;
	hdmi->link_cfg.rate_per_lane = max_rate_per_lane;
	hdmi->link_cfg.allm_en = hdmi->allm_en;

	if (!max_frl_rate ||
	    (tmdsclk < HDMI20_MAX_RATE && mode->clock < HDMI20_MAX_RATE)) {
		printf("%s use tmds mode\n", __func__);
		hdmi->link_cfg.frl_mode = false;
		return;
	}

	hdmi->link_cfg.frl_mode = true;

	if (!hdmi->dsc_cap.v_1p2)
		return;

	max_dsc_lanes = hdmi->dsc_cap.max_lanes;
	max_dsc_rate_per_lane =
		hdmi->dsc_cap.max_frl_rate_per_lane;

	if (mode->clock >= HDMI_8K60_RATE &&
	    !hdmi_bus_fmt_is_yuv420(hdmi->bus_format) &&
	    !hdmi_bus_fmt_is_yuv422(hdmi->bus_format)) {
		hdmi->link_cfg.dsc_mode = true;
		hdmi->link_cfg.frl_lanes = max_dsc_lanes;
		hdmi->link_cfg.rate_per_lane = max_dsc_rate_per_lane;
	} else {
		hdmi->link_cfg.dsc_mode = false;
		hdmi->link_cfg.frl_lanes = max_lanes;
		hdmi->link_cfg.rate_per_lane = max_rate_per_lane;
	}
}

/////////////////////////////////////////////////////////////////////////////////////

static int hdmi_dsc_get_slice_height(int vactive)
{
	int slice_height;

	/*
	 * Slice Height determination : HDMI2.1 Section 7.7.5.2
	 * Select smallest slice height >=96, that results in a valid PPS and
	 * requires minimum padding lines required for final slice.
	 *
	 * Assumption : Vactive is even.
	 */
	for (slice_height = 96; slice_height <= vactive; slice_height += 2)
		if (vactive % slice_height == 0)
			return slice_height;

	return 0;
}

static int hdmi_dsc_get_num_slices(struct rockchip_hdmi *hdmi,
				   struct drm_display_mode *mode,
				   int src_max_slices, int src_max_slice_width,
				   int hdmi_max_slices, int hdmi_throughput)
{
/* Pixel rates in KPixels/sec */
#define HDMI_DSC_PEAK_PIXEL_RATE		2720000
/*
 * Rates at which the source and sink are required to process pixels in each
 * slice, can be two levels: either at least 340000KHz or at least 40000KHz.
 */
#define HDMI_DSC_MAX_ENC_THROUGHPUT_0		340000
#define HDMI_DSC_MAX_ENC_THROUGHPUT_1		400000

/* Spec limits the slice width to 2720 pixels */
#define MAX_HDMI_SLICE_WIDTH			2720
	int kslice_adjust;
	int adjusted_clk_khz;
	int min_slices;
	int target_slices;
	int max_throughput; /* max clock freq. in khz per slice */
	int max_slice_width;
	int slice_width;
	int pixel_clock = mode->clock;

	if (!hdmi_throughput)
		return 0;

	/*
	 * Slice Width determination : HDMI2.1 Section 7.7.5.1
	 * kslice_adjust factor for 4:2:0, and 4:2:2 formats is 0.5, where as
	 * for 4:4:4 is 1.0. Multiplying these factors by 10 and later
	 * dividing adjusted clock value by 10.
	 */
	if (hdmi_bus_fmt_is_yuv444(hdmi->output_bus_format) ||
	    hdmi_bus_fmt_is_rgb(hdmi->output_bus_format))
		kslice_adjust = 10;
	else
		kslice_adjust = 5;

	/*
	 * As per spec, the rate at which the source and the sink process
	 * the pixels per slice are at two levels: at least 340Mhz or 400Mhz.
	 * This depends upon the pixel clock rate and output formats
	 * (kslice adjust).
	 * If pixel clock * kslice adjust >= 2720MHz slices can be processed
	 * at max 340MHz, otherwise they can be processed at max 400MHz.
	 */

	adjusted_clk_khz = DIV_ROUND_UP(kslice_adjust * pixel_clock, 10);

	if (adjusted_clk_khz <= HDMI_DSC_PEAK_PIXEL_RATE)
		max_throughput = HDMI_DSC_MAX_ENC_THROUGHPUT_0;
	else
		max_throughput = HDMI_DSC_MAX_ENC_THROUGHPUT_1;

	/*
	 * Taking into account the sink's capability for maximum
	 * clock per slice (in MHz) as read from HF-VSDB.
	 */
	max_throughput = min(max_throughput, hdmi_throughput * 1000);

	min_slices = DIV_ROUND_UP(adjusted_clk_khz, max_throughput);
	max_slice_width = min(MAX_HDMI_SLICE_WIDTH, src_max_slice_width);

	/*
	 * Keep on increasing the num of slices/line, starting from min_slices
	 * per line till we get such a number, for which the slice_width is
	 * just less than max_slice_width. The slices/line selected should be
	 * less than or equal to the max horizontal slices that the combination
	 * of PCON encoder and HDMI decoder can support.
	 */
	do {
		if (min_slices <= 1 && src_max_slices >= 1 && hdmi_max_slices >= 1)
			target_slices = 1;
		else if (min_slices <= 2 && src_max_slices >= 2 && hdmi_max_slices >= 2)
			target_slices = 2;
		else if (min_slices <= 4 && src_max_slices >= 4 && hdmi_max_slices >= 4)
			target_slices = 4;
		else if (min_slices <= 8 && src_max_slices >= 8 && hdmi_max_slices >= 8)
			target_slices = 8;
		else if (min_slices <= 12 && src_max_slices >= 12 && hdmi_max_slices >= 12)
			target_slices = 12;
		else if (min_slices <= 16 && src_max_slices >= 16 && hdmi_max_slices >= 16)
			target_slices = 16;
		else
			return 0;

		slice_width = DIV_ROUND_UP(mode->hdisplay, target_slices);
		if (slice_width > max_slice_width)
			min_slices = target_slices + 1;
	} while (slice_width > max_slice_width);

	return target_slices;
}

static int hdmi_dsc_slices(struct rockchip_hdmi *hdmi,
			   struct drm_display_mode *mode)
{
	int hdmi_throughput = hdmi->dsc_cap.clk_per_slice;
	int hdmi_max_slices = hdmi->dsc_cap.max_slices;
	int rk_max_slices = 8;
	int rk_max_slice_width = 2048;

	return hdmi_dsc_get_num_slices(hdmi, mode, rk_max_slices,
				       rk_max_slice_width,
				       hdmi_max_slices, hdmi_throughput);
}

static int
hdmi_dsc_get_bpp(struct rockchip_hdmi *hdmi, int src_fractional_bpp,
		 int slice_width, int num_slices, bool hdmi_all_bpp,
		 int hdmi_max_chunk_bytes)
{
	int max_dsc_bpp, min_dsc_bpp;
	int target_bytes;
	bool bpp_found = false;
	int bpp_decrement_x16;
	int bpp_target;
	int bpp_target_x16;

	/*
	 * Get min bpp and max bpp as per Table 7.23, in HDMI2.1 spec
	 * Start with the max bpp and keep on decrementing with
	 * fractional bpp, if supported by PCON DSC encoder
	 *
	 * for each bpp we check if no of bytes can be supported by HDMI sink
	 */

	/* only 9\10\12 bpp was tested */
	min_dsc_bpp = 9;
	max_dsc_bpp = 12;

	/*
	 * Taking into account if all dsc_all_bpp supported by HDMI2.1 sink
	 * Section 7.7.34 : Source shall not enable compressed Video
	 * Transport with bpp_target settings above 12 bpp unless
	 * DSC_all_bpp is set to 1.
	 */
	if (!hdmi_all_bpp)
		max_dsc_bpp = min(max_dsc_bpp, 12);

	/*
	 * The Sink has a limit of compressed data in bytes for a scanline,
	 * as described in max_chunk_bytes field in HFVSDB block of edid.
	 * The no. of bytes depend on the target bits per pixel that the
	 * source configures. So we start with the max_bpp and calculate
	 * the target_chunk_bytes. We keep on decrementing the target_bpp,
	 * till we get the target_chunk_bytes just less than what the sink's
	 * max_chunk_bytes, or else till we reach the min_dsc_bpp.
	 *
	 * The decrement is according to the fractional support from PCON DSC
	 * encoder. For fractional BPP we use bpp_target as a multiple of 16.
	 *
	 * bpp_target_x16 = bpp_target * 16
	 * So we need to decrement by {1, 2, 4, 8, 16} for fractional bpps
	 * {1/16, 1/8, 1/4, 1/2, 1} respectively.
	 */

	bpp_target = max_dsc_bpp;

	/* src does not support fractional bpp implies decrement by 16 for bppx16 */
	if (!src_fractional_bpp)
		src_fractional_bpp = 1;
	bpp_decrement_x16 = DIV_ROUND_UP(16, src_fractional_bpp);
	bpp_target_x16 = bpp_target * 16;

	while (bpp_target_x16 > (min_dsc_bpp * 16)) {
		int bpp;

		bpp = DIV_ROUND_UP(bpp_target_x16, 16);
		target_bytes = DIV_ROUND_UP((num_slices * slice_width * bpp), 8);
		if (target_bytes <= hdmi_max_chunk_bytes) {
			bpp_found = true;
			break;
		}
		bpp_target_x16 -= bpp_decrement_x16;
	}
	if (bpp_found)
		return bpp_target_x16;

	return 0;
}

static int
dw_hdmi_dsc_bpp(struct rockchip_hdmi *hdmi,
		int num_slices, int slice_width)
{
	bool hdmi_all_bpp = hdmi->dsc_cap.all_bpp;
	int fractional_bpp = 0;
	int hdmi_max_chunk_bytes = hdmi->dsc_cap.total_chunk_kbytes * 1024;

	return hdmi_dsc_get_bpp(hdmi, fractional_bpp, slice_width,
				num_slices, hdmi_all_bpp,
				hdmi_max_chunk_bytes);
}

static int dw_hdmi_qp_set_link_cfg(struct rockchip_hdmi *hdmi,
				   u16 pic_width, u16 pic_height,
				   u16 slice_width, u16 slice_height,
				   u16 bits_per_pixel, u8 bits_per_component)
{
	int i;

	for (i = 0; i < PPS_TABLE_LEN; i++)
		if (pic_width == pps_datas[i].pic_width &&
		    pic_height == pps_datas[i].pic_height &&
		    slice_width == pps_datas[i].slice_width &&
		    slice_height == pps_datas[i].slice_height &&
		    bits_per_component == pps_datas[i].bpc &&
		    bits_per_pixel == pps_datas[i].bpp &&
		    hdmi_bus_fmt_is_rgb(hdmi->output_bus_format) == pps_datas[i].convert_rgb)
			break;

	if (i == PPS_TABLE_LEN) {
		dev_err(hdmi->dev, "can't find pps cfg!\n");
		return -EINVAL;
	}

	memcpy(hdmi->link_cfg.pps_payload, pps_datas[i].raw_pps, 128);
	hdmi->link_cfg.hcactive = DIV_ROUND_UP(slice_width * (bits_per_pixel / 16), 8) *
		(pic_width / slice_width);

	return 0;
}

static void dw_hdmi_qp_dsc_configure(struct rockchip_hdmi *hdmi,
				     struct drm_display_mode *mode)
{
	int ret;
	int slice_height;
	int slice_width;
	int bits_per_pixel;
	int slice_count;
	bool hdmi_is_dsc_1_2;
	unsigned int depth = hdmi_bus_fmt_color_depth(hdmi->output_bus_format);

	hdmi_is_dsc_1_2 = hdmi->dsc_cap.v_1p2;

	if (!hdmi_is_dsc_1_2)
		return;

	slice_height = hdmi_dsc_get_slice_height(mode->vdisplay);
	if (!slice_height)
		return;

	slice_count = hdmi_dsc_slices(hdmi, mode);
	if (!slice_count)
		return;

	slice_width = DIV_ROUND_UP(mode->hdisplay, slice_count);

	bits_per_pixel = dw_hdmi_dsc_bpp(hdmi, slice_count, slice_width);
	if (!bits_per_pixel)
		return;

	ret = dw_hdmi_qp_set_link_cfg(hdmi, mode->hdisplay,
				      mode->vdisplay, slice_width,
				      slice_height, bits_per_pixel, depth);

	if (ret) {
		dev_err(hdmi->dev, "set vdsc cfg failed\n");
		return;
	}
	printf("dsc_enable\n");
}

static unsigned int drm_rk_select_color(struct hdmi_edid_data *edid_data,
					struct base_screen_info *screen_info,
					enum dw_hdmi_devtype dev_type,
					bool output_bus_format_rgb)
{
	struct drm_display_info *info = &edid_data->display_info;
	struct drm_display_mode *mode = edid_data->preferred_mode;
	struct drm_hdmi_info *hdmi_info = &edid_data->display_info.hdmi;
	int max_tmds_clock = info->max_tmds_clock;
	bool support_dc = false;
	bool mode_420 = drm_mode_is_420(info, mode);
	unsigned int color_depth = 8;
	unsigned int base_color = DRM_HDMI_OUTPUT_YCBCR444;
	unsigned int color_format = DRM_HDMI_OUTPUT_DEFAULT_RGB;
	unsigned long tmdsclock, pixclock = mode->clock;

	if (screen_info)
		base_color = screen_info->format;

	switch (base_color) {
	case DRM_HDMI_OUTPUT_YCBCR_HQ:
		if (info->color_formats & DRM_COLOR_FORMAT_YCRCB444)
			color_format = DRM_HDMI_OUTPUT_YCBCR444;
		else if (info->color_formats & DRM_COLOR_FORMAT_YCRCB422)
			color_format = DRM_HDMI_OUTPUT_YCBCR422;
		else if (mode_420 && pixclock >= 340000)
			color_format = DRM_HDMI_OUTPUT_YCBCR420;
		break;
	case DRM_HDMI_OUTPUT_YCBCR_LQ:
		if (mode_420 && pixclock >= 340000)
			color_format = DRM_HDMI_OUTPUT_YCBCR420;
		else if (info->color_formats & DRM_COLOR_FORMAT_YCRCB422)
			color_format = DRM_HDMI_OUTPUT_YCBCR422;
		else if (info->color_formats & DRM_COLOR_FORMAT_YCRCB444)
			color_format = DRM_HDMI_OUTPUT_YCBCR444;
		break;
	case DRM_HDMI_OUTPUT_YCBCR420:
		if (mode_420 && pixclock >= 340000)
			color_format = DRM_HDMI_OUTPUT_YCBCR420;
		break;
	case DRM_HDMI_OUTPUT_YCBCR422:
		if (info->color_formats & DRM_COLOR_FORMAT_YCRCB422)
			color_format = DRM_HDMI_OUTPUT_YCBCR422;
		break;
	case DRM_HDMI_OUTPUT_YCBCR444:
		if (info->color_formats & DRM_COLOR_FORMAT_YCRCB444)
			color_format = DRM_HDMI_OUTPUT_YCBCR444;
		break;
	case DRM_HDMI_OUTPUT_DEFAULT_RGB:
	default:
		break;
	}

	if (output_bus_format_rgb)
		color_format = DRM_HDMI_OUTPUT_DEFAULT_RGB;

	if (color_format == DRM_HDMI_OUTPUT_DEFAULT_RGB &&
	    info->edid_hdmi_dc_modes & DRM_EDID_HDMI_DC_30)
		support_dc = true;
	if (color_format == DRM_HDMI_OUTPUT_YCBCR444 &&
	    (info->edid_hdmi_dc_modes &
	     (DRM_EDID_HDMI_DC_Y444 | DRM_EDID_HDMI_DC_30)))
		support_dc = true;
	if (color_format == DRM_HDMI_OUTPUT_YCBCR422)
		support_dc = true;
	if (color_format == DRM_HDMI_OUTPUT_YCBCR420 &&
	    info->hdmi.y420_dc_modes & DRM_EDID_YCBCR420_DC_30)
		support_dc = true;

	if (mode->flags & DRM_MODE_FLAG_DBLCLK)
		pixclock *= 2;

	if (screen_info && screen_info->depth == 10)
		color_depth = screen_info->depth;

	if (mode->clock >= 600000) {
		color_format = DRM_HDMI_OUTPUT_YCBCR420;
	} else if (mode->clock >= 340000) {
		if (drm_mode_is_420(info, mode))
			color_format = DRM_HDMI_OUTPUT_YCBCR420;
	}

	if (color_format == DRM_HDMI_OUTPUT_YCBCR422 || color_depth == 8)
		tmdsclock = pixclock;
	else
		tmdsclock = pixclock * color_depth / 8;

	if (color_format == DRM_HDMI_OUTPUT_YCBCR420)
		tmdsclock /= 2;

	if (!max_tmds_clock)
		max_tmds_clock = 340000;

	if (hdmi_info->max_frl_rate_per_lane && mode->clock > 600000)
		max_tmds_clock =
			hdmi_info->max_lanes * hdmi_info->max_frl_rate_per_lane *
				1000000;

	if (tmdsclock > max_tmds_clock) {
		if (max_tmds_clock >= 594000) {
			color_depth = 8;
		} else if (max_tmds_clock > 340000) {
			if (drm_mode_is_420(info, mode) || tmdsclock >= 594000)
				color_format = DRM_HDMI_OUTPUT_YCBCR420;
		} else {
			color_depth = 8;
			if (drm_mode_is_420(info, mode) || tmdsclock >= 594000)
				color_format = DRM_HDMI_OUTPUT_YCBCR420;
		}
	}

	printf("color_format:%x\n", color_format);
	if (color_depth > 8 && support_dc) {
		switch (color_format) {
		case DRM_HDMI_OUTPUT_YCBCR444:
			return MEDIA_BUS_FMT_YUV10_1X30;
		case DRM_HDMI_OUTPUT_YCBCR422:
			return MEDIA_BUS_FMT_UYVY10_1X20;
		case DRM_HDMI_OUTPUT_YCBCR420:
			return MEDIA_BUS_FMT_UYYVYY10_0_5X30;
		default:
			return MEDIA_BUS_FMT_RGB101010_1X30;
		}
	} else {
		switch (color_format) {
		case DRM_HDMI_OUTPUT_YCBCR444:
			return MEDIA_BUS_FMT_YUV8_1X24;
		case DRM_HDMI_OUTPUT_YCBCR422:
			return MEDIA_BUS_FMT_UYVY8_1X16;
		case DRM_HDMI_OUTPUT_YCBCR420:
			return MEDIA_BUS_FMT_UYYVYY8_0_5X24;
		default:
			return MEDIA_BUS_FMT_RGB888_1X24;
		}
	}
}

#define SUPPORT_HDMI_ALLM	BIT(1)

void dw_hdmi_qp_selete_output(struct hdmi_edid_data *edid_data,
			      struct rockchip_connector *conn,
			      unsigned int *bus_format,
			      struct overscan *overscan,
			      enum dw_hdmi_devtype dev_type,
			      bool output_bus_format_rgb,
			      void *data, struct display_state *state)
{
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;
	struct drm_hdmi_info *hdmi_info = &edid_data->display_info.hdmi;
	int ret, i, screen_size;
	struct base_disp_info base_parameter;
	struct base2_disp_info *base2_parameter = state->conn_state.disp_info;
	const struct base_overscan *scan;
	struct base_screen_info *screen_info = NULL;
	struct base2_screen_info *screen_info2 = NULL;
	int max_scan = 100;
	int min_scan = 51;
	int offset = 0;
	int color_depth;
	unsigned int tmdsclk;
	unsigned int pixel_clk;
	bool found = false;
	struct blk_desc *dev_desc;
	disk_partition_t part_info;
	char baseparameter_buf[8 * RK_BLK_SIZE] __aligned(ARCH_DMA_MINALIGN);

	overscan->left_margin = max_scan;
	overscan->right_margin = max_scan;
	overscan->top_margin = max_scan;
	overscan->bottom_margin = max_scan;

	hdmi->max_frl_rate_per_lane = hdmi_info->max_frl_rate_per_lane;
	hdmi->max_lanes = hdmi_info->max_lanes;
	memcpy(&hdmi->dsc_cap, &hdmi_info->dsc_cap, sizeof(struct drm_hdmi_dsc_cap));

	if (output_bus_format_rgb)
		*bus_format = MEDIA_BUS_FMT_RGB888_1X24;
	else
		*bus_format = MEDIA_BUS_FMT_YUV8_1X24;

	if (!base2_parameter) {
		dev_desc = rockchip_get_bootdev();
		if (!dev_desc) {
			printf("%s: Could not find device\n", __func__);
			goto null_basep;
		}

		ret = part_get_info_by_name(dev_desc, "baseparameter",
					    &part_info);
		if (ret < 0) {
			printf("Could not find baseparameter partition\n");
			goto null_basep;
		}

read_aux:
		ret = blk_dread(dev_desc, part_info.start + offset, 1,
				(void *)baseparameter_buf);
		if (ret < 0) {
			printf("read baseparameter failed\n");
			goto null_basep;
		}

		memcpy(&base_parameter, baseparameter_buf,
		       sizeof(base_parameter));
		scan = &base_parameter.scan;

		screen_size = sizeof(base_parameter.screen_list) /
			sizeof(base_parameter.screen_list[0]);

		for (i = 0; i < screen_size; i++) {
			if (base_parameter.screen_list[i].type ==
			    DRM_MODE_CONNECTOR_HDMIA) {
				found = true;
				screen_info = &base_parameter.screen_list[i];
				break;
			}
		}

		if (!found && !offset) {
			printf("hdmi info isn't saved in main block\n");
			offset += 16;
			goto read_aux;
		}
	} else {
		bool allm_en = false;

		scan = &base2_parameter->overscan_info;
		screen_size = sizeof(base2_parameter->screen_info) /
			sizeof(base2_parameter->screen_info[0]);

		for (i = 0; i < screen_size; i++) {
			if (base2_parameter->screen_info[i].type ==
			    DRM_MODE_CONNECTOR_HDMIA) {
				screen_info2 =
					&base2_parameter->screen_info[i];
				break;
			}
		}
		screen_info = malloc(sizeof(*screen_info));

		screen_info->type = screen_info2->type;
		screen_info->mode = screen_info2->resolution;
		screen_info->format = screen_info2->format;
		screen_info->depth = screen_info2->depthc;
		screen_info->feature = screen_info2->feature;

		/* check if allm is enabled */
		allm_en = base2_parameter->reserved[0] & BIT(0);
		if (allm_en && (hdmi_info->add_func & SUPPORT_HDMI_ALLM))
			hdmi->allm_en = true;
		else
			hdmi->allm_en = false;
	}

	if (scan->leftscale < min_scan && scan->leftscale > 0)
		overscan->left_margin = min_scan;
	else if (scan->leftscale < max_scan && scan->leftscale > 0)
		overscan->left_margin = scan->leftscale;

	if (scan->rightscale < min_scan && scan->rightscale > 0)
		overscan->right_margin = min_scan;
	else if (scan->rightscale < max_scan && scan->rightscale > 0)
		overscan->right_margin = scan->rightscale;

	if (scan->topscale < min_scan && scan->topscale > 0)
		overscan->top_margin = min_scan;
	else if (scan->topscale < max_scan && scan->topscale > 0)
		overscan->top_margin = scan->topscale;

	if (scan->bottomscale < min_scan && scan->bottomscale > 0)
		overscan->bottom_margin = min_scan;
	else if (scan->bottomscale < max_scan && scan->bottomscale > 0)
		overscan->bottom_margin = scan->bottomscale;

null_basep:

	if (screen_info)
		printf("base_parameter.mode:%dx%d\n",
		       screen_info->mode.hdisplay,
		       screen_info->mode.vdisplay);
	drm_rk_select_mode(edid_data, screen_info);

	*bus_format = drm_rk_select_color(edid_data, screen_info,
					  dev_type, output_bus_format_rgb);

	if (state->force_output) {
		memcpy(edid_data->preferred_mode, &state->force_mode,
		       sizeof(struct drm_display_mode));
		if (state->force_bus_format)
			*bus_format = state->force_bus_format;
	}

	hdmi->bus_format = *bus_format;
	color_depth = hdmi_bus_fmt_color_depth(*bus_format);
	pixel_clk = edid_data->preferred_mode->clock;
	tmdsclk = hdmi_get_tmdsclock(hdmi, pixel_clk);
	if (hdmi_bus_fmt_is_yuv420(hdmi->output_bus_format))
		tmdsclk /= 2;
	hdmi_select_link_config(hdmi, edid_data->preferred_mode, tmdsclk);
	dw_hdmi_qp_dsc_configure(hdmi, edid_data->preferred_mode);
	if (hdmi->link_cfg.frl_mode) {
		dm_gpio_set_value(&hdmi->enable_gpio, 0);
		/* in the current version, support max 40G frl */
		if (hdmi->link_cfg.rate_per_lane >= 10) {
			hdmi->link_cfg.frl_lanes = 4;
			hdmi->link_cfg.rate_per_lane = 10;
		}
		hdmi->bus_width = hdmi->link_cfg.frl_lanes *
			hdmi->link_cfg.rate_per_lane * 1000000;
		/* 10 bit color depth and frl mode */
		if (color_depth == 10)
			hdmi->bus_width |=
				COLOR_DEPTH_10BIT | HDMI_FRL_MODE;
		else
			hdmi->bus_width |= HDMI_FRL_MODE;
	} else {
		dm_gpio_set_value(&hdmi->enable_gpio, 1);
		hdmi->bus_width =
			hdmi_get_tmdsclock(hdmi, pixel_clk * 10);
		if (hdmi_bus_fmt_is_yuv420(*bus_format))
			hdmi->bus_width /= 2;

		if (color_depth == 10)
			hdmi->bus_width |= COLOR_DEPTH_10BIT;
	}

	rockchip_phy_set_bus_width(conn->phy, hdmi->bus_width);
}

static void rk3588_set_link_mode(struct rockchip_hdmi *hdmi)
{
	int val;
	bool is_hdmi0;

	if (!hdmi->id)
		is_hdmi0 = true;
	else
		is_hdmi0 = false;

	if (!hdmi->link_cfg.frl_mode) {
		val = HIWORD_UPDATE(0, RK3588_HDMI21_MASK);
		if (is_hdmi0)
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON4);
		else
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON7);

		val = HIWORD_UPDATE(0, RK3588_COMPRESS_MODE_MASK | RK3588_COLOR_FORMAT_MASK);
		if (is_hdmi0)
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON3);
		else
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON6);

		return;
	}

	val = HIWORD_UPDATE(RK3588_HDMI21_MASK, RK3588_HDMI21_MASK);
	if (is_hdmi0)
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON4);
	else
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON7);

	if (hdmi->link_cfg.dsc_mode) {
		val = HIWORD_UPDATE(RK3588_COMPRESS_MODE_MASK | RK3588_COMPRESSED_DATA,
				    RK3588_COMPRESS_MODE_MASK | RK3588_COLOR_FORMAT_MASK);
		if (is_hdmi0)
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON3);
		else
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON6);
	} else {
		val = HIWORD_UPDATE(0, RK3588_COMPRESS_MODE_MASK | RK3588_COLOR_FORMAT_MASK);
		if (is_hdmi0)
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON3);
		else
			writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON6);
	}
}

static void rk3588_set_color_format(struct rockchip_hdmi *hdmi, u64 bus_format,
				    u32 depth)
{
	u32 val = 0;

	switch (bus_format) {
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_RGB101010_1X30:
		val = HIWORD_UPDATE(0, RK3588_COLOR_FORMAT_MASK);
		break;
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
		val = HIWORD_UPDATE(RK3588_YUV420, RK3588_COLOR_FORMAT_MASK);
		break;
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_YUV10_1X30:
		val = HIWORD_UPDATE(RK3588_YUV444, RK3588_COLOR_FORMAT_MASK);
		break;
	default:
		dev_err(hdmi->dev, "can't set correct color format\n");
		return;
	}

	if (hdmi->link_cfg.dsc_mode)
		val = HIWORD_UPDATE(RK3588_COMPRESSED_DATA, RK3588_COLOR_FORMAT_MASK);

	if (depth == 8)
		val |= HIWORD_UPDATE(RK3588_8BPC, RK3588_COLOR_DEPTH_MASK);
	else
		val |= HIWORD_UPDATE(RK3588_10BPC, RK3588_COLOR_DEPTH_MASK);

	if (!hdmi->id)
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON3);
	else
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON6);
}

void rk3588_set_grf_cfg(void *data)
{
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;
	int color_depth;

	rk3588_set_link_mode(hdmi);
	color_depth = hdmi_bus_fmt_color_depth(hdmi->bus_format);
	rk3588_set_color_format(hdmi, hdmi->bus_format, color_depth);
}

void dw_hdmi_qp_set_iomux(void *data)
{
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;
	u32 val;

	if (!hdmi->id) {
		val = HIWORD_UPDATE(RK3588_SCLIN_MASK, RK3588_SCLIN_MASK) |
		      HIWORD_UPDATE(RK3588_SDAIN_MASK, RK3588_SDAIN_MASK) |
		      HIWORD_UPDATE(RK3588_MODE_MASK, RK3588_MODE_MASK) |
		      HIWORD_UPDATE(RK3588_I2S_SEL_MASK, RK3588_I2S_SEL_MASK);
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON3);

		val = HIWORD_UPDATE(RK3588_SET_HPD_PATH_MASK,
				    RK3588_SET_HPD_PATH_MASK);
		writel(val, hdmi->grf + RK3588_GRF_SOC_CON7);

		val = HIWORD_UPDATE(RK3588_HDMI0_GRANT_SEL,
				    RK3588_HDMI0_GRANT_SEL);
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON9);
	} else {
		val = HIWORD_UPDATE(RK3588_SCLIN_MASK, RK3588_SCLIN_MASK) |
		      HIWORD_UPDATE(RK3588_SDAIN_MASK, RK3588_SDAIN_MASK) |
		      HIWORD_UPDATE(RK3588_MODE_MASK, RK3588_MODE_MASK) |
		      HIWORD_UPDATE(RK3588_I2S_SEL_MASK, RK3588_I2S_SEL_MASK);
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON6);

		val = HIWORD_UPDATE(RK3588_SET_HPD_PATH_MASK,
				    RK3588_SET_HPD_PATH_MASK);
		writel(val, hdmi->grf + RK3588_GRF_SOC_CON7);

		val = HIWORD_UPDATE(RK3588_HDMI1_GRANT_SEL,
				    RK3588_HDMI1_GRANT_SEL);
		writel(val, hdmi->vo1_grf + RK3588_GRF_VO1_CON9);
	}
}

struct dw_hdmi_link_config *dw_hdmi_rockchip_get_link_cfg(void *data)
{
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;

	return &hdmi->link_cfg;
}

static void dw_hdmi_qp_rockchip_phy_disable(struct rockchip_connector *conn, void *data,
					    void *state)
{
	rockchip_phy_power_off(conn->phy);
}

static int dw_hdmi_qp_rockchip_genphy_init(struct rockchip_connector *conn, void *data, void *state)
{
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;

	dw_hdmi_qp_rockchip_phy_disable(conn, data, state);

	rockchip_phy_set_bus_width(conn->phy, hdmi->bus_width);

	return rockchip_phy_power_on(conn->phy);
}

static enum drm_connector_status dw_hdmi_rk3588_read_hpd(void *data)
{
	u32 val;
	int ret;
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;

	val = readl(hdmi->grf + RK3588_GRF_SOC_STATUS1);

	if (!hdmi->id) {
		if (val & RK3588_HDMI0_LEVEL_INT)
			ret = connector_status_connected;
		else
			ret = connector_status_disconnected;
	} else {
		if (val & RK3588_HDMI1_LEVEL_INT)
			ret = connector_status_connected;
		else
			ret = connector_status_disconnected;
	}

	return ret;
}

static void dw_hdmi_rk3588_set_pll(struct rockchip_connector *conn, void *data, void *state)
{
	struct rockchip_hdmi *hdmi = (struct rockchip_hdmi *)data;
	u32 rate = (hdmi->bus_width & DATA_RATE_MASK) * 100;

	clk_set_rate(&hdmi->link_clk, rate);
}
static const struct dw_hdmi_qp_phy_ops rk3588_hdmi_phy_ops = {
	.init = dw_hdmi_qp_rockchip_genphy_init,
	.disable = dw_hdmi_qp_rockchip_phy_disable,
	.read_hpd = dw_hdmi_rk3588_read_hpd,
	.set_pll = dw_hdmi_rk3588_set_pll,
};

static const struct rockchip_connector_funcs rockchip_dw_hdmi_qp_funcs = {
	.init = rockchip_dw_hdmi_qp_init,
	.deinit = rockchip_dw_hdmi_qp_deinit,
	.prepare = rockchip_dw_hdmi_qp_prepare,
	.enable = rockchip_dw_hdmi_qp_enable,
	.disable = rockchip_dw_hdmi_qp_disable,
	.get_timing = rockchip_dw_hdmi_qp_get_timing,
	.detect = rockchip_dw_hdmi_qp_detect,
	.get_edid = rockchip_dw_hdmi_qp_get_edid,
};

const struct dw_hdmi_plat_data rk3588_hdmi_drv_data = {
	.qp_phy_ops    = &rk3588_hdmi_phy_ops,
	.phy_name   = "samsung_hdptx_phy",
	.dev_type   = RK3588_HDMI,
};

static int rockchip_dw_hdmi_qp_probe(struct udevice *dev)
{
	int ret;
	struct regmap *map;
	struct rockchip_hdmi *hdmi = dev_get_priv(dev);

	hdmi->dev = dev;

	hdmi->id = of_alias_get_id(ofnode_to_np(dev->node), "hdmi");
	if (hdmi->id < 0)
		hdmi->id = 0;

	hdmi->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (hdmi->grf <= 0) {
		dev_err(dev, "%s: Get syscon grf failed (ret=%p)\n",
			__func__, hdmi->grf);
		return -ENXIO;
	}

	map = syscon_regmap_lookup_by_phandle(dev, "rockchip,vo1_grf");
	hdmi->vo1_grf = regmap_get_range(map, 0);

	if (hdmi->vo1_grf <= 0) {
		dev_err(dev, "%s: Get syscon vo1 grf failed (ret=%p)\n",
			__func__, hdmi->vo1_grf);
		return -ENXIO;
	}

	ret = gpio_request_by_name(dev, "enable-gpios", 0,
				   &hdmi->enable_gpio, GPIOD_IS_OUT);
	if (ret) {
		dev_err(dev, "Cannot get enable GPIO: %d\n", ret);
		return ret;
	}

	ret = clk_get_by_name(dev, "link_clk", &hdmi->link_clk);
	if (ret) {
		printf("%s: can't get link_clk\n", __func__);
		return ret;
	}

	rockchip_connector_bind(&hdmi->connector, dev, hdmi->id, &rockchip_dw_hdmi_qp_funcs,
				NULL, DRM_MODE_CONNECTOR_HDMIA);

	return 0;
}

static const struct udevice_id rockchip_dw_hdmi_qp_ids[] = {
	{
	 .compatible = "rockchip,rk3588-dw-hdmi",
	 .data = (ulong)&rk3588_hdmi_drv_data,
	}, {}
};

U_BOOT_DRIVER(rockchip_dw_hdmi_qp) = {
	.name = "rockchip_dw_hdmi_qp",
	.id = UCLASS_DISPLAY,
	.of_match = rockchip_dw_hdmi_qp_ids,
	.probe	= rockchip_dw_hdmi_qp_probe,
	.priv_auto_alloc_size = sizeof(struct rockchip_hdmi),
};
