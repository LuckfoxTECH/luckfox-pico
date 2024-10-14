// SPDX-License-Identifier: GPL-2.0
/*
 * sc830ai driver
 *
 * Copyright (C) 2023 Rockchip Electronics Co., Ltd.
 *
 * V0.0X01.0X01 first version
 */

//#define DEBUG
#include <linux/clk.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/consumer.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/rk-camera-module.h>
#include <media/media-entity.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>
#include <linux/pinctrl/consumer.h>
#include <linux/rk-preisp.h>
#include "../platform/rockchip/isp/rkisp_tb_helper.h"

#define DRIVER_VERSION			KERNEL_VERSION(0, 0x01, 0x01)

#ifndef V4L2_CID_DIGITAL_GAIN
#define V4L2_CID_DIGITAL_GAIN		V4L2_CID_GAIN
#endif

#define MIPI_FREQ_405M			405000000	// 910Mbps

#define SC830AI_MAX_PIXEL_RATE		(MIPI_FREQ_405M / 10 * 2 * SC830AI_2LANES)
#define OF_CAMERA_HDR_MODE		"rockchip,camera-hdr-mode"

#define SC830AI_XVCLK_FREQ		24000000

/* TODO: Get the real chip id from reg */
#define CHIP_ID				0xC143
#define SC830AI_REG_CHIP_ID		0x3107

#define SC830AI_REG_CTRL_MODE		0x0100
#define SC830AI_MODE_SW_STANDBY		0x0
#define SC830AI_MODE_STREAMING		BIT(0)

/*expo*/
#define	SC830AI_EXPOSURE_MIN		2    /*okay*/
#define	SC830AI_EXPOSURE_STEP		1    /*okay*/
#define SC830AI_VTS_MAX			0xffff   /*okay*/

//long exposure
#define SC830AI_REG_EXP_LONG_H		0x3e00    //[3:0]
#define SC830AI_REG_EXP_LONG_M		0x3e01    //[7:0]
#define SC830AI_REG_EXP_LONG_L		0x3e02    //[7:4]

//short exposure  //for hdr
#define SC830AI_REG_EXP_SF_H		0x3e22
#define SC830AI_REG_EXP_SF_M		0x3e04    //[7:0]
#define SC830AI_REG_EXP_SF_L		0x3e05    //[7:4]

#define SC830AI_FETCH_EXP_H(VAL)	(((VAL) >> 12) & 0xF)
#define SC830AI_FETCH_EXP_M(VAL)	(((VAL) >> 4) & 0xFF)
#define SC830AI_FETCH_EXP_L(VAL)	(((VAL) & 0xF) << 4)

/*gain*/
//long frame and normal gain reg
#define SC830AI_REG_DGAIN		0x3e06
#define SC830AI_REG_DGAIN_FINE		0x3e07
#define SC830AI_REG_AGAIN		0x3e09

//short frame gain reg
#define SC830AI_SF_REG_DGAIN		0x3e10
#define SC830AI_SF_REG_DGAIN_FINE	0x3e11
#define SC830AI_SF_REG_AGAIN		0x3e13

#define SC830AI_GAIN_MIN		0x20	//1.000 = 32 * 1/32
#define SC830AI_GAIN_MAX		(4032)   /* need_view   3.938 * 32 * 32 = 4032.512 */
#define SC830AI_GAIN_STEP		1
#define SC830AI_GAIN_DEFAULT		0x20

#define SC830AI_REG_VTS			0x320e

//group hold
#define SC830AI_GROUP_UPDATE_ADDRESS	0x3812
#define SC830AI_GROUP_UPDATE_START_DATA	0x00
#define SC830AI_GROUP_UPDATE_LAUNCH	0x30
#define SC830AI_GROUP_DEALY_CTRL	0x3802

#define SC830AI_SOFTWARE_RESET_REG	0x0103
#define SC830AI_REG_TEST_PATTERN	0x4501
#define SC830AI_TEST_PATTERN_ENABLE	0x08

#define SC830AI_FLIP_REG		0x3221
#define SC830AI_FLIP_MASK		0x60
#define SC830AI_MIRROR_MASK		0x06

#define REG_NULL			0xFFFF

#define SC830AI_REG_VALUE_08BIT		1
#define SC830AI_REG_VALUE_16BIT		2
#define SC830AI_REG_VALUE_24BIT		3

#define SC830AI_2LANES			2
#define SC830AI_4LANES			4

#define OF_CAMERA_PINCTRL_STATE_DEFAULT	"rockchip,camera_default"
#define OF_CAMERA_PINCTRL_STATE_SLEEP	"rockchip,camera_sleep"

#define SC830AI_NAME				"sc830ai"

static const char * const sc830ai_supply_names[] = {
	"dvdd",		// Digital core power
	"dovdd",	// Digital I/O power
	"avdd",		// Analog power
};
#define SC830AI_NUM_SUPPLIES ARRAY_SIZE(sc830ai_supply_names)

struct regval {
	u16 addr;
	u8 val;
};

struct sc830ai_mode {
	u32 bus_fmt;
	u32 width;
	u32 height;
	struct v4l2_fract max_fps;
	u32 hts_def;
	u32 vts_def;
	u32 exp_def;
	u32 mipi_freq_idx;
	u32 bpp;
	const struct regval *reg_list;
	u32 hdr_mode;
	u32 vc[PAD_MAX];
};

struct sc830ai {
	struct i2c_client	*client;
	struct clk		*xvclk;
	struct gpio_desc	*reset_gpio;
	struct gpio_desc	*pwdn_gpio;
	struct regulator_bulk_data supplies[SC830AI_NUM_SUPPLIES];

	struct pinctrl		*pinctrl;
	struct pinctrl_state	*pins_default;
	struct pinctrl_state	*pins_sleep;

	struct v4l2_subdev	subdev;
	struct media_pad	pad;
	struct v4l2_ctrl_handler ctrl_handler;
	struct v4l2_ctrl	*exposure;
	struct v4l2_ctrl	*anal_a_gain;
	struct v4l2_ctrl	*digi_gain;
	struct v4l2_ctrl	*hblank;
	struct v4l2_ctrl	*vblank;
	struct v4l2_ctrl	*test_pattern;
	struct v4l2_ctrl	*pixel_rate;
	struct v4l2_ctrl	*link_freq;
	struct mutex		mutex;
	struct v4l2_fract	cur_fps;
	bool			streaming;
	bool			power_on;
	bool			is_first_streamoff;
	const struct sc830ai_mode *cur_mode;
	u32			module_index;
	u32			cfg_num;
	const char		*module_facing;
	const char		*module_name;
	const char		*len_name;
	u32			cur_vts;
	bool			has_init_exp;
	struct preisp_hdrae_exp_s init_hdrae_exp;
};


#define to_sc830ai(sd) container_of(sd, struct sc830ai, subdev)

//cleaned_0x09_SC830AI_MIPI_24Minput_4lane_360Mbps_10bit_3840x2160_15fps.ini
static __maybe_unused const struct regval sc830ai_4lane_linear10bit_3840x2160_regs[] = {
	{0x0103, 0x01},
	{0x0100, 0x00},
	{0x36e9, 0x80},
	{0x37f9, 0x80},
	{0x301f, 0x09},
	{0x320c, 0x08},
	{0x320d, 0x34},
	{0x320e, 0x08},
	{0x320f, 0xec},
	{0x3281, 0x80},
	{0x3301, 0x0e},
	{0x3303, 0x18},
	{0x3306, 0x50},
	{0x3308, 0x20},
	{0x330a, 0x00},
	{0x330b, 0xd8},
	{0x330c, 0x20},
	{0x330e, 0x40},
	{0x330f, 0x08},
	{0x3314, 0x16},
	{0x3317, 0x07},
	{0x3319, 0x0c},
	{0x3321, 0x0c},
	{0x3324, 0x09},
	{0x3325, 0x09},
	{0x3327, 0x16},
	{0x3328, 0x10},
	{0x3329, 0x1c},
	{0x332b, 0x0d},
	{0x3333, 0x10},
	{0x333e, 0x0e},
	{0x3352, 0x0c},
	{0x3353, 0x0c},
	{0x335e, 0x06},
	{0x335f, 0x08},
	{0x3364, 0x5e},
	{0x3366, 0x01},
	{0x337c, 0x02},
	{0x337d, 0x0a},
	{0x3390, 0x01},
	{0x3391, 0x0b},
	{0x3392, 0x1f},
	{0x3393, 0x0e},
	{0x3394, 0x30},
	{0x3395, 0x30},
	{0x3396, 0x01},
	{0x3397, 0x0b},
	{0x3398, 0x1f},
	{0x3399, 0x0e},
	{0x339a, 0x0e},
	{0x339b, 0x30},
	{0x339c, 0x30},
	{0x339f, 0x0e},
	{0x33a2, 0x04},
	{0x33ad, 0x3c},
	{0x33af, 0x68},
	{0x33b1, 0x80},
	{0x33b2, 0x58},
	{0x33b3, 0x40},
	{0x33ba, 0x0c},
	{0x33f9, 0x80},
	{0x33fb, 0xa0},
	{0x33fc, 0x4b},
	{0x33fd, 0x5f},
	{0x349f, 0x03},
	{0x34a0, 0x0e},
	{0x34a6, 0x4b},
	{0x34a7, 0x5f},
	{0x34a8, 0x20},
	{0x34a9, 0x10},
	{0x34aa, 0x01},
	{0x34ab, 0x10},
	{0x34ac, 0x01},
	{0x34ad, 0x28},
	{0x34f8, 0x5f},
	{0x34f9, 0x10},
	{0x3630, 0xc8},
	{0x3632, 0x46},
	{0x3633, 0x33},
	{0x3637, 0x24},
	{0x3638, 0xc3},
	{0x363c, 0x40},
	{0x363d, 0x40},
	{0x363e, 0x70},
	{0x3670, 0x01},
	{0x3674, 0xc6},
	{0x3675, 0x8c},
	{0x3676, 0x8c},
	{0x367c, 0x4b},
	{0x367d, 0x5f},
	{0x3698, 0x82},
	{0x3699, 0x8d},
	{0x369a, 0x9c},
	{0x369b, 0xba},
	{0x369e, 0xba},
	{0x369f, 0xba},
	{0x36a2, 0x49},
	{0x36a3, 0x4b},
	{0x36a4, 0x4f},
	{0x36a5, 0x5f},
	{0x36a6, 0x5f},
	{0x36d0, 0x01},
	{0x36ea, 0x08},
	{0x36eb, 0x14},
	{0x36ec, 0x03},
	{0x36ed, 0x22},
	{0x370f, 0x01},
	{0x3721, 0x9c},
	{0x3722, 0x03},
	{0x3724, 0x31},
	{0x37b0, 0x03},
	{0x37b1, 0x03},
	{0x37b2, 0x03},
	{0x37b3, 0x4b},
	{0x37b4, 0x4f},
	{0x37fa, 0x08},
	{0x37fb, 0x31},
	{0x37fc, 0x10},
	{0x37fd, 0x04},
	{0x3903, 0x40},
	{0x3905, 0x4c},
	{0x391e, 0x09},
	{0x3929, 0x18},
	{0x3933, 0x80},
	{0x3934, 0x03},
	{0x3935, 0x00},
	{0x3936, 0x34},
	{0x3937, 0x6a},
	{0x3938, 0x69},
	{0x3e00, 0x01},
	{0x3e01, 0x1c},
	{0x3e02, 0x60},
	{0x3e09, 0x40},
	{0x440e, 0x02},
	{0x4837, 0x2c},
	{0x5010, 0x01},
	{0x5799, 0x77},
	{0x57aa, 0xeb},
	{0x57d9, 0x00},
	{0x5ae0, 0xfe},
	{0x5ae1, 0x40},
	{0x5ae2, 0x38},
	{0x5ae3, 0x30},
	{0x5ae4, 0x28},
	{0x5ae5, 0x38},
	{0x5ae6, 0x30},
	{0x5ae7, 0x28},
	{0x5ae8, 0x3f},
	{0x5ae9, 0x34},
	{0x5aea, 0x2c},
	{0x5aeb, 0x3f},
	{0x5aec, 0x34},
	{0x5aed, 0x2c},
	{0x5aee, 0xfe},
	{0x5aef, 0x40},
	{0x5af4, 0x38},
	{0x5af5, 0x30},
	{0x5af6, 0x28},
	{0x5af7, 0x38},
	{0x5af8, 0x30},
	{0x5af9, 0x28},
	{0x5afa, 0x3f},
	{0x5afb, 0x34},
	{0x5afc, 0x2c},
	{0x5afd, 0x3f},
	{0x5afe, 0x34},
	{0x5aff, 0x2c},
	{0x5f00, 0x05},
	{0x36e9, 0x53},
	{0x37f9, 0x27},
	// {0x0100, 0x01},
	{REG_NULL, 0x00},
};

//cleaned_0x09_SC830AI_MIPI_24Minput_2lane_360Mbps_10bit_3840x2160_15fps.ini
static __maybe_unused const struct regval sc830ai_2lane_linear10bit_3840x2160_regs[] = {
	{0x0103, 0x01},
	{0x0100, 0x00},
	{0x36e9, 0x80},
	{0x37f9, 0x80},
	{0x3018, 0x32},
	{0x3019, 0x0c},
	{0x301f, 0x07},
	{0x320c, 0x09},
	{0x320d, 0x60},
	{0x3281, 0x80},
	{0x3301, 0x0e},
	{0x3303, 0x18},
	{0x3306, 0x50},
	{0x3308, 0x20},
	{0x330a, 0x00},
	{0x330b, 0xd8},
	{0x330c, 0x20},
	{0x330e, 0x40},
	{0x330f, 0x08},
	{0x3314, 0x16},
	{0x3317, 0x07},
	{0x3319, 0x0c},
	{0x3321, 0x0c},
	{0x3324, 0x09},
	{0x3325, 0x09},
	{0x3327, 0x16},
	{0x3328, 0x10},
	{0x3329, 0x1c},
	{0x332b, 0x0d},
	{0x3333, 0x10},
	{0x333e, 0x0e},
	{0x3352, 0x0c},
	{0x3353, 0x0c},
	{0x335e, 0x06},
	{0x335f, 0x08},
	{0x3364, 0x5e},
	{0x3366, 0x01},
	{0x337c, 0x02},
	{0x337d, 0x0a},
	{0x3390, 0x01},
	{0x3391, 0x0b},
	{0x3392, 0x1f},
	{0x3393, 0x0e},
	{0x3394, 0x30},
	{0x3395, 0x30},
	{0x3396, 0x01},
	{0x3397, 0x0b},
	{0x3398, 0x1f},
	{0x3399, 0x0e},
	{0x339a, 0x0e},
	{0x339b, 0x30},
	{0x339c, 0x30},
	{0x339f, 0x0e},
	{0x33a2, 0x04},
	{0x33ad, 0x3c},
	{0x33af, 0x68},
	{0x33b1, 0x80},
	{0x33b2, 0x58},
	{0x33b3, 0x40},
	{0x33ba, 0x0c},
	{0x33f9, 0x80},
	{0x33fb, 0xa0},
	{0x33fc, 0x4b},
	{0x33fd, 0x5f},
	{0x349f, 0x03},
	{0x34a0, 0x0e},
	{0x34a6, 0x4b},
	{0x34a7, 0x5f},
	{0x34a8, 0x20},
	{0x34a9, 0x10},
	{0x34aa, 0x01},
	{0x34ab, 0x10},
	{0x34ac, 0x01},
	{0x34ad, 0x28},
	{0x34f8, 0x5f},
	{0x34f9, 0x10},
	{0x3630, 0xc8},
	{0x3632, 0x46},
	{0x3633, 0x33},
	{0x3637, 0x24},
	{0x3638, 0xc3},
	{0x363c, 0x40},
	{0x363d, 0x40},
	{0x363e, 0x70},
	{0x3670, 0x01},
	{0x3674, 0xc6},
	{0x3675, 0x8c},
	{0x3676, 0x8c},
	{0x367c, 0x4b},
	{0x367d, 0x5f},
	{0x3698, 0x82},
	{0x3699, 0x8d},
	{0x369a, 0x9c},
	{0x369b, 0xba},
	{0x369e, 0xba},
	{0x369f, 0xba},
	{0x36a2, 0x49},
	{0x36a3, 0x4b},
	{0x36a4, 0x4f},
	{0x36a5, 0x5f},
	{0x36a6, 0x5f},
	{0x36d0, 0x01},
	{0x36ea, 0x09},
	{0x36eb, 0x04},
	{0x36ec, 0x03},
	{0x36ed, 0x22},
	{0x370f, 0x01},
	{0x3721, 0x9c},
	{0x3722, 0x03},
	{0x3724, 0x31},
	{0x37b0, 0x03},
	{0x37b1, 0x03},
	{0x37b2, 0x03},
	{0x37b3, 0x4b},
	{0x37b4, 0x4f},
	{0x37fa, 0x09},
	{0x37fb, 0x31},
	{0x37fc, 0x10},
	{0x37fd, 0x14},
	{0x3903, 0x40},
	{0x3905, 0x4c},
	{0x391e, 0x09},
	{0x3929, 0x18},
	{0x3933, 0x80},
	{0x3934, 0x03},
	{0x3935, 0x00},
	{0x3936, 0x34},
	{0x3937, 0x6a},
	{0x3938, 0x69},
	{0x3e00, 0x01},
	{0x3e01, 0x18},
	{0x3e09, 0x40},
	{0x440e, 0x02},
	{0x4837, 0x14},
	{0x5010, 0x01},
	{0x5799, 0x77},
	{0x57aa, 0xeb},
	{0x57d9, 0x00},
	{0x5ae0, 0xfe},
	{0x5ae1, 0x40},
	{0x5ae2, 0x38},
	{0x5ae3, 0x30},
	{0x5ae4, 0x28},
	{0x5ae5, 0x38},
	{0x5ae6, 0x30},
	{0x5ae7, 0x28},
	{0x5ae8, 0x3f},
	{0x5ae9, 0x34},
	{0x5aea, 0x2c},
	{0x5aeb, 0x3f},
	{0x5aec, 0x34},
	{0x5aed, 0x2c},
	{0x5aee, 0xfe},
	{0x5aef, 0x40},
	{0x5af4, 0x38},
	{0x5af5, 0x30},
	{0x5af6, 0x28},
	{0x5af7, 0x38},
	{0x5af8, 0x30},
	{0x5af9, 0x28},
	{0x5afa, 0x3f},
	{0x5afb, 0x34},
	{0x5afc, 0x2c},
	{0x5afd, 0x3f},
	{0x5afe, 0x34},
	{0x5aff, 0x2c},
	{0x5f00, 0x05},
	{0x36e9, 0x53},
	{0x37f9, 0x53},
	// {0x0100, 0x01},
	{REG_NULL, 0x00},
};

/*
 * The width and height must be configured to be
 * the same as the current output resolution of the sensor.
 * The input width of the isp needs to be 16 aligned.
 * The input height of the isp needs to be 8 aligned.
 * If the width or height does not meet the alignment rules,
 * you can configure the cropping parameters with the following function to
 * crop out the appropriate resolution.
 * struct v4l2_subdev_pad_ops {
 *	.get_selection
 * }
 */
static const struct sc830ai_mode supported_modes[] = {
	{
		.bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
		.width = 3840,
		.height = 2160,
		.max_fps = {
			.numerator = 10000,
			.denominator = 150000,
		},
		.exp_def = 0x08c0,
		.hts_def = 0x0226*5-0x180,
		.vts_def = 0x08ca,
		.bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
		.reg_list = sc830ai_2lane_linear10bit_3840x2160_regs,
		.hdr_mode = NO_HDR,
		.mipi_freq_idx = 0,
		.bpp = 10,
		.vc[PAD0] = V4L2_MBUS_CSI2_CHANNEL_0,
	},
};

static const char * const sc830ai_test_pattern_menu[] = {
	"Disabled",
	"Vertical Color Bar Type 1",
	"Vertical Color Bar Type 2",
	"Vertical Color Bar Type 3",
	"Vertical Color Bar Type 4"
};

static const s64 link_freq_items[] = {
	MIPI_FREQ_405M,
};

/* Write registers up to 4 at a time */
static int sc830ai_write_reg(struct i2c_client *client, u16 reg,
			    u32 len, u32 val)
{
	u32 buf_i, val_i;
	u8 buf[6];
	u8 *val_p;
	__be32 val_be;

	if (len > 4)
		return -EINVAL;

	buf[0] = reg >> 8;
	buf[1] = reg & 0xff;

	val_be = cpu_to_be32(val);
	val_p = (u8 *)&val_be;
	buf_i = 2;
	val_i = 4 - len;

	while (val_i < 4)
		buf[buf_i++] = val_p[val_i++];

	if (i2c_master_send(client, buf, len + 2) != len + 2)
		return -EIO;

	return 0;
}

static int sc830ai_write_array(struct i2c_client *client,
			      const struct regval *regs)
{
	u32 i;
	int ret = 0;

	for (i = 0; ret == 0 && regs[i].addr != REG_NULL; i++) {
		ret = sc830ai_write_reg(client, regs[i].addr,
				       SC830AI_REG_VALUE_08BIT, regs[i].val);
	}
	return ret;
}

/* Read registers up to 4 at a time */
static int sc830ai_read_reg(struct i2c_client *client, u16 reg, unsigned int len,
			   u32 *val)
{
	struct i2c_msg msgs[2];
	u8 *data_be_p;
	__be32 data_be = 0;
	__be16 reg_addr_be = cpu_to_be16(reg);
	int ret;

	if (len > 4 || !len)
		return -EINVAL;

	data_be_p = (u8 *)&data_be;
	/* Write register address */
	msgs[0].addr = client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 2;
	msgs[0].buf = (u8 *)&reg_addr_be;

	/* Read data from register */
	msgs[1].addr = client->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = len;
	msgs[1].buf = &data_be_p[4 - len];

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret != ARRAY_SIZE(msgs))
		return -EIO;

	*val = be32_to_cpu(data_be);

	return 0;
}

static int sc830ai_get_reso_dist(const struct sc830ai_mode *mode,
				struct v4l2_mbus_framefmt *framefmt)
{
	return abs(mode->width - framefmt->width) +
	       abs(mode->height - framefmt->height);
}

static const struct sc830ai_mode *
sc830ai_find_best_fit(struct sc830ai *sc830ai, struct v4l2_subdev_format *fmt)
{
	struct v4l2_mbus_framefmt *framefmt = &fmt->format;
	int dist;
	int cur_best_fit = 0;
	int cur_best_fit_dist = -1;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(supported_modes); i++) {
		dist = sc830ai_get_reso_dist(&supported_modes[i], framefmt);
		if ((cur_best_fit_dist == -1 || dist < cur_best_fit_dist) &&
			supported_modes[i].bus_fmt == framefmt->code) {
			cur_best_fit_dist = dist;
			cur_best_fit = i;
		}
	}
	dev_info(&sc830ai->client->dev, "%s: cur_best_fit(%d)",
		 __func__, cur_best_fit);

	return &supported_modes[cur_best_fit];
}

static void sc830ai_change_mode(struct sc830ai *sc830ai, const struct sc830ai_mode *mode)
{
	sc830ai->cur_mode = mode;
	sc830ai->cur_vts = sc830ai->cur_mode->vts_def;
	dev_info(&sc830ai->client->dev, "set fmt: cur_mode: %dx%d, hdr: %d\n",
		mode->width, mode->height, mode->hdr_mode);
}

static int sc830ai_set_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *fmt)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	const struct sc830ai_mode *mode;
	s64 h_blank, vblank_def;
	u64 pixel_rate = 0;

	mutex_lock(&sc830ai->mutex);

	mode = sc830ai_find_best_fit(sc830ai, fmt);
	fmt->format.code = mode->bus_fmt;
	fmt->format.width = mode->width;
	fmt->format.height = mode->height;
	fmt->format.field = V4L2_FIELD_NONE;
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
		*v4l2_subdev_get_try_format(sd, cfg, fmt->pad) = fmt->format;
#else
		mutex_unlock(&sc830ai->mutex);
		return -ENOTTY;
#endif
	} else {
		sc830ai_change_mode(sc830ai, mode);
		h_blank = mode->hts_def - mode->width;
		__v4l2_ctrl_modify_range(sc830ai->hblank, h_blank,
					 h_blank, 1, h_blank);
		vblank_def = mode->vts_def - mode->height;
		__v4l2_ctrl_modify_range(sc830ai->vblank, vblank_def,
					 SC830AI_VTS_MAX - mode->height,
					 1, vblank_def);
		__v4l2_ctrl_s_ctrl(sc830ai->link_freq, mode->mipi_freq_idx);
		pixel_rate = (u32)link_freq_items[mode->mipi_freq_idx] /
			mode->bpp * 2 * SC830AI_2LANES;
		__v4l2_ctrl_s_ctrl_int64(sc830ai->pixel_rate, pixel_rate);
		sc830ai->cur_fps = mode->max_fps;
		sc830ai->cur_vts = mode->vts_def;
	}

	mutex_unlock(&sc830ai->mutex);

	return 0;
}

static int sc830ai_get_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *fmt)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	const struct sc830ai_mode *mode = sc830ai->cur_mode;

	mutex_lock(&sc830ai->mutex);
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
		fmt->format = *v4l2_subdev_get_try_format(sd, cfg, fmt->pad);
#else
		mutex_unlock(&sc830ai->mutex);
		return -ENOTTY;
#endif
	} else {
		fmt->format.width = mode->width;
		fmt->format.height = mode->height;
		fmt->format.code = mode->bus_fmt;
		fmt->format.field = V4L2_FIELD_NONE;
		if (fmt->pad < PAD_MAX && mode->hdr_mode != NO_HDR)
			fmt->reserved[0] = mode->vc[fmt->pad];
		else
			fmt->reserved[0] = mode->vc[PAD0];
	}
	mutex_unlock(&sc830ai->mutex);

	return 0;
}

static int sc830ai_enum_mbus_code(struct v4l2_subdev *sd,
				 struct v4l2_subdev_pad_config *cfg,
				 struct v4l2_subdev_mbus_code_enum *code)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);

	if (code->index != 0)
		return -EINVAL;
	code->code = sc830ai->cur_mode->bus_fmt;

	return 0;
}

static int sc830ai_enum_frame_sizes(struct v4l2_subdev *sd,
				   struct v4l2_subdev_pad_config *cfg,
				   struct v4l2_subdev_frame_size_enum *fse)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);

	if (fse->index >= sc830ai->cfg_num)
		return -EINVAL;

	if (fse->code != supported_modes[fse->index].bus_fmt)
		return -EINVAL;

	fse->min_width  = supported_modes[fse->index].width;
	fse->max_width  = supported_modes[fse->index].width;
	fse->max_height = supported_modes[fse->index].height;
	fse->min_height = supported_modes[fse->index].height;

	return 0;
}

static int sc830ai_enable_test_pattern(struct sc830ai *sc830ai, u32 pattern)
{
	u32 val = 0;
	int ret = 0;

	ret = sc830ai_read_reg(sc830ai->client, SC830AI_REG_TEST_PATTERN,
			      SC830AI_REG_VALUE_08BIT, &val);
	if (pattern)
		val |= SC830AI_TEST_PATTERN_ENABLE;
	else
		val &= ~SC830AI_TEST_PATTERN_ENABLE;
	ret |= sc830ai_write_reg(sc830ai->client, SC830AI_REG_TEST_PATTERN,
				SC830AI_REG_VALUE_08BIT, val);
	return ret;
}

static int sc830ai_g_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *fi)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	const struct sc830ai_mode *mode = sc830ai->cur_mode;

	if (sc830ai->streaming)
		fi->interval = sc830ai->cur_fps;
	else
		fi->interval = mode->max_fps;

	return 0;
}

static int sc830ai_g_mbus_config(struct v4l2_subdev *sd,
				 unsigned int pad_id,
				 struct v4l2_mbus_config *config)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	const struct sc830ai_mode *mode = sc830ai->cur_mode;
	u32 val = 0;

	if (mode->hdr_mode == NO_HDR)
		val = 1 << (SC830AI_2LANES - 1) |
		V4L2_MBUS_CSI2_CHANNEL_0 |
		V4L2_MBUS_CSI2_CONTINUOUS_CLOCK;
	if (mode->hdr_mode == HDR_X2)
		val = 1 << (SC830AI_2LANES - 1) |
		V4L2_MBUS_CSI2_CHANNEL_0 |
		V4L2_MBUS_CSI2_CONTINUOUS_CLOCK |
		V4L2_MBUS_CSI2_CHANNEL_1;

	config->type = V4L2_MBUS_CSI2_DPHY;
	config->flags = val;

	return 0;
}

static void sc830ai_get_module_inf(struct sc830ai *sc830ai,
				  struct rkmodule_inf *inf)
{
	memset(inf, 0, sizeof(*inf));
	strscpy(inf->base.sensor, SC830AI_NAME, sizeof(inf->base.sensor));
	strscpy(inf->base.module, sc830ai->module_name,
		sizeof(inf->base.module));
	strscpy(inf->base.lens, sc830ai->len_name, sizeof(inf->base.lens));
}

static void sc830ai_get_gain_reg(u32 val, u32 *again_reg, u32 *dgain_reg,
				 u32 *dgain_fine_reg)
{
	u32 total_gain;

	total_gain = val;
	if (total_gain < 0x20)
		total_gain = 0x20;
	else if (total_gain > SC830AI_GAIN_MAX)
		total_gain = SC830AI_GAIN_MAX;

	if (total_gain < 0x40) { /* 1 - 2x gain */
		*again_reg = 0x40;
		*dgain_reg = 0x00;
		*dgain_fine_reg = total_gain << 2;
	} else if (total_gain < 0x80) { /* 2x - 4x gain */
		*again_reg = 0x48;
		*dgain_reg = 0x00;
		*dgain_fine_reg = (total_gain << 2) >> 1;
	} else if (total_gain < 0x100) { /* 4x - 8x gain */
		*again_reg = 0x49;
		*dgain_reg = 0x00;
		*dgain_fine_reg = (total_gain << 2) >> 2;
	} else if (total_gain < 0x200) { /* 8x - 16x gain */
		*again_reg = 0x4B;
		*dgain_reg = 0x00;
		*dgain_fine_reg = (total_gain << 2) >> 3;
	} else if (total_gain < 0x400) { /* 16x - 32x gain */
		*again_reg = 0x4F;
		*dgain_reg = 0x00;
		*dgain_fine_reg = (total_gain << 2) >> 4;
	} else if (total_gain < 0x800) { /* 32x - 64x gain */
		*again_reg = 0x5F;
		*dgain_reg = 0x00;
		*dgain_fine_reg = (total_gain << 2) >> 5;
	} else  { // (total_gain < 0x1000) /* 64x gain */
		*again_reg = 0x5F;
		*dgain_reg = 0x01;
		*dgain_fine_reg = (total_gain << 2) >> 6;
	}

	*dgain_fine_reg = *dgain_fine_reg / 4 * 4;
}

static int sc830ai_get_channel_info(struct sc830ai *sc830ai, struct rkmodule_channel_info *ch_info)
{
	if (ch_info->index < PAD0 || ch_info->index >= PAD_MAX)
		return -EINVAL;
	ch_info->vc = sc830ai->cur_mode->vc[ch_info->index];
	ch_info->width = sc830ai->cur_mode->width;
	ch_info->height = sc830ai->cur_mode->height;
	ch_info->bus_fmt = sc830ai->cur_mode->bus_fmt;
	return 0;
}

static long sc830ai_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	struct rkmodule_hdr_cfg *hdr_cfg;
	const struct sc830ai_mode *mode;
	struct rkmodule_channel_info *ch_info;
	long ret = 0;
	u64 pixel_rate = 0;
	u32 i, h, w, stream;

	switch (cmd) {
	case PREISP_CMD_SET_HDRAE_EXP:
		/*
		 * ret = sc830ai_set_hdrae(sc830ai, arg);
		 */
		break;

	case RKMODULE_SET_HDR_CFG:
		hdr_cfg = (struct rkmodule_hdr_cfg *)arg;
		if (sc830ai->streaming) {
			ret = sc830ai_write_array(sc830ai->client, sc830ai->cur_mode->reg_list);
			if (ret)
				return ret;
		}
		w = sc830ai->cur_mode->width;
		h = sc830ai->cur_mode->height;
		for (i = 0; i < sc830ai->cfg_num; i++) {
			if (w == supported_modes[i].width &&
			h == supported_modes[i].height &&
			supported_modes[i].hdr_mode == hdr_cfg->hdr_mode) {
				sc830ai_change_mode(sc830ai, &supported_modes[i]);
				break;
			}
		}
		if (i == sc830ai->cfg_num) {
			dev_err(&sc830ai->client->dev,
				"not find hdr mode:%d %dx%d config\n",
				hdr_cfg->hdr_mode, w, h);
			ret = -EINVAL;
		} else {
			mode = sc830ai->cur_mode;
			w = mode->hts_def - mode->width;
			h = mode->vts_def - mode->height;
			__v4l2_ctrl_modify_range(sc830ai->hblank, w, w, 1, w);
			__v4l2_ctrl_modify_range(sc830ai->vblank, h,
				SC830AI_VTS_MAX - mode->height,
				1, h);
			__v4l2_ctrl_s_ctrl(sc830ai->link_freq, mode->mipi_freq_idx);
			pixel_rate = (u32)link_freq_items[mode->mipi_freq_idx] /
				mode->bpp * 2 * SC830AI_2LANES;
			__v4l2_ctrl_s_ctrl_int64(sc830ai->pixel_rate,
						 pixel_rate);
			sc830ai->cur_fps = mode->max_fps;
			sc830ai->cur_vts = mode->vts_def;
			dev_info(&sc830ai->client->dev,
				"sensor mode: %d\n", mode->hdr_mode);
		}
		break;
	case RKMODULE_GET_MODULE_INFO:
		sc830ai_get_module_inf(sc830ai, (struct rkmodule_inf *)arg);
		break;

	case RKMODULE_GET_HDR_CFG:
		hdr_cfg = (struct rkmodule_hdr_cfg *)arg;
		hdr_cfg->esp.mode = HDR_NORMAL_VC;
		hdr_cfg->hdr_mode = sc830ai->cur_mode->hdr_mode;
		break;

	case RKMODULE_SET_QUICK_STREAM:
		stream = *((u32 *)arg);
		if (stream)
			ret = sc830ai_write_reg(sc830ai->client, SC830AI_REG_CTRL_MODE,
				SC830AI_REG_VALUE_08BIT, SC830AI_MODE_STREAMING);
		else
			ret = sc830ai_write_reg(sc830ai->client, SC830AI_REG_CTRL_MODE,
				SC830AI_REG_VALUE_08BIT, SC830AI_MODE_SW_STANDBY);
		break;

	case RKMODULE_GET_CHANNEL_INFO:
		ch_info = (struct rkmodule_channel_info *)arg;
		ret = sc830ai_get_channel_info(sc830ai, ch_info);
		break;

	default:
		ret = -ENOIOCTLCMD;
		break;
	}

	return ret;
}

#ifdef CONFIG_COMPAT
static long sc830ai_compat_ioctl32(struct v4l2_subdev *sd,
				  unsigned int cmd, unsigned long arg)
{
	void __user *up = compat_ptr(arg);
	struct rkmodule_inf *inf;
	struct rkmodule_awb_cfg *cfg;
	struct rkmodule_hdr_cfg *hdr;
	struct preisp_hdrae_exp_s *hdrae;
	struct rkmodule_channel_info *ch_info;
	long ret;
	u32  stream;
	u32 brl = 0;
	struct rkmodule_csi_dphy_param *dphy_param;

	switch (cmd) {
	case RKMODULE_GET_MODULE_INFO:
		inf = kzalloc(sizeof(*inf), GFP_KERNEL);
		if (!inf) {
			ret = -ENOMEM;
			return ret;
		}

		ret = sc830ai_ioctl(sd, cmd, inf);
		if (!ret) {
			if (copy_to_user(up, inf, sizeof(*inf))) {
				kfree(inf);
				return -EFAULT;
			}
		}
		kfree(inf);
		break;
	case RKMODULE_AWB_CFG:
		cfg = kzalloc(sizeof(*cfg), GFP_KERNEL);
		if (!cfg) {
			ret = -ENOMEM;
			return ret;
		}

		if (copy_from_user(cfg, up, sizeof(*cfg))) {
			kfree(cfg);
			return -EFAULT;
		}
		ret = sc830ai_ioctl(sd, cmd, cfg);
		kfree(cfg);
		break;
	case RKMODULE_GET_HDR_CFG:
		hdr = kzalloc(sizeof(*hdr), GFP_KERNEL);
		if (!hdr) {
			ret = -ENOMEM;
			return ret;
		}

		ret = sc830ai_ioctl(sd, cmd, hdr);
		if (!ret) {
			if (copy_to_user(up, hdr, sizeof(*hdr))) {
				kfree(hdr);
				return -EFAULT;
			}
		}
		kfree(hdr);
		break;
	case RKMODULE_SET_HDR_CFG:
		hdr = kzalloc(sizeof(*hdr), GFP_KERNEL);
		if (!hdr) {
			ret = -ENOMEM;
			return ret;
		}

		if (copy_from_user(hdr, up, sizeof(*hdr))) {
			kfree(hdr);
			return -EFAULT;
		}
		ret = sc830ai_ioctl(sd, cmd, hdr);
		kfree(hdr);
		break;
	case PREISP_CMD_SET_HDRAE_EXP:
		hdrae = kzalloc(sizeof(*hdrae), GFP_KERNEL);
		if (!hdrae) {
			ret = -ENOMEM;
			return ret;
		}

		if (copy_from_user(hdrae, up, sizeof(*hdrae))) {
			kfree(hdrae);
			return -EFAULT;
		}
		ret = sc830ai_ioctl(sd, cmd, hdrae);
		kfree(hdrae);
		break;
	case RKMODULE_SET_QUICK_STREAM:
		if (copy_from_user(&stream, up, sizeof(u32)))
			return -EFAULT;
		ret = sc830ai_ioctl(sd, cmd, &stream);
		break;
	case RKMODULE_GET_SONY_BRL:
		ret = sc830ai_ioctl(sd, cmd, &brl);
		if (!ret) {
			if (copy_to_user(up, &brl, sizeof(u32)))
				return -EFAULT;
		}
		break;
	case RKMODULE_GET_CHANNEL_INFO:
		ch_info = kzalloc(sizeof(*ch_info), GFP_KERNEL);
		if (!ch_info) {
			ret = -ENOMEM;
			return ret;
		}

		ret = sc830ai_ioctl(sd, cmd, ch_info);
		if (!ret) {
			ret = copy_to_user(up, ch_info, sizeof(*ch_info));
			if (ret)
				ret = -EFAULT;
		}
		kfree(ch_info);
		break;
	case RKMODULE_GET_CSI_DPHY_PARAM:
		dphy_param = kzalloc(sizeof(*dphy_param), GFP_KERNEL);
		if (!dphy_param) {
			ret = -ENOMEM;
			return ret;
		}

		ret = sc830ai_ioctl(sd, cmd, dphy_param);
		if (!ret) {
			ret = copy_to_user(up, dphy_param, sizeof(*dphy_param));
			if (ret)
				ret = -EFAULT;
		}
		kfree(dphy_param);
		break;

	default:
		ret = -ENOIOCTLCMD;
		break;
	}

	return ret;
}
#endif


static int __sc830ai_start_stream(struct sc830ai *sc830ai)
{
	int ret;

	ret = sc830ai_write_array(sc830ai->client, sc830ai->cur_mode->reg_list);
	if (ret)
		return ret;

	ret = __v4l2_ctrl_handler_setup(&sc830ai->ctrl_handler);
	if (ret)
		return ret;
	/* In case these controls are set before streaming */
	if (sc830ai->has_init_exp && sc830ai->cur_mode->hdr_mode != NO_HDR) {
		ret = sc830ai_ioctl(&sc830ai->subdev, PREISP_CMD_SET_HDRAE_EXP,
			&sc830ai->init_hdrae_exp);
		if (ret) {
			dev_err(&sc830ai->client->dev,
				"init exp fail in hdr mode\n");
			return ret;
		}
	}
	return sc830ai_write_reg(sc830ai->client, SC830AI_REG_CTRL_MODE,
				SC830AI_REG_VALUE_08BIT, SC830AI_MODE_STREAMING);
}

static int __sc830ai_stop_stream(struct sc830ai *sc830ai)
{
	sc830ai->has_init_exp = false;
	return sc830ai_write_reg(sc830ai->client, SC830AI_REG_CTRL_MODE,
		SC830AI_REG_VALUE_08BIT, SC830AI_MODE_SW_STANDBY);
}

static int sc830ai_s_stream(struct v4l2_subdev *sd, int on)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	struct i2c_client *client = sc830ai->client;
	int ret = 0;

	dev_info(&sc830ai->client->dev, "s_stream: %d. %dx%d, hdr: %d, bpp: %d\n",
	       on, sc830ai->cur_mode->width, sc830ai->cur_mode->height,
	       sc830ai->cur_mode->hdr_mode, sc830ai->cur_mode->bpp);

	mutex_lock(&sc830ai->mutex);
	on = !!on;
	if (on == sc830ai->streaming)
		goto unlock_and_return;

	if (on) {
		ret = pm_runtime_get_sync(&client->dev);
		if (ret < 0) {
			pm_runtime_put_noidle(&client->dev);
			goto unlock_and_return;
		}
		ret = __sc830ai_start_stream(sc830ai);
		if (ret) {
			v4l2_err(sd, "start stream failed while write regs\n");
			pm_runtime_put(&client->dev);
			goto unlock_and_return;
		}
	} else {
		__sc830ai_stop_stream(sc830ai);
		pm_runtime_put(&client->dev);
	}

	sc830ai->streaming = on;

unlock_and_return:
	mutex_unlock(&sc830ai->mutex);
	return ret;
}

static int sc830ai_s_power(struct v4l2_subdev *sd, int on)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	struct i2c_client *client = sc830ai->client;
	int ret = 0;

	mutex_lock(&sc830ai->mutex);

	/* If the power state is not modified - no work to do. */
	if (sc830ai->power_on == !!on)
		goto unlock_and_return;

	if (on) {
		ret = pm_runtime_get_sync(&client->dev);
		if (ret < 0) {
			pm_runtime_put_noidle(&client->dev);
			goto unlock_and_return;
		}

		ret |= sc830ai_write_reg(sc830ai->client,
			SC830AI_SOFTWARE_RESET_REG,
			SC830AI_REG_VALUE_08BIT,
			0x01);
		/*
		 * usleep_range(100, 200);
		 * ret |= sc830ai_write_reg(sc2310->client,
		 *	0x303f,
		 *	SC830AI_REG_VALUE_08BIT,
		 *	0x01);
		 */
		sc830ai->power_on = true;
	} else {
		pm_runtime_put(&client->dev);
		sc830ai->power_on = false;
	}

unlock_and_return:
	mutex_unlock(&sc830ai->mutex);

	return ret;
}

/* Calculate the delay in us by clock rate and clock cycles */
static inline u32 sc830ai_cal_delay(u32 cycles)
{
	return DIV_ROUND_UP(cycles, SC830AI_XVCLK_FREQ / 1000 / 1000);
}

static int __sc830ai_power_on(struct sc830ai *sc830ai)
{
	int ret;
	u32 delay_us;
	struct device *dev = &sc830ai->client->dev;

	if (!IS_ERR_OR_NULL(sc830ai->pins_default)) {
		ret = pinctrl_select_state(sc830ai->pinctrl, sc830ai->pins_default);
		if (ret < 0)
			dev_err(dev, "could not set pins\n");
	}

	ret = clk_set_rate(sc830ai->xvclk, SC830AI_XVCLK_FREQ);
	if (ret < 0)
		dev_warn(dev, "Failed to set xvclk rate 24MHz\n");
	if (clk_get_rate(sc830ai->xvclk) != SC830AI_XVCLK_FREQ)
		dev_warn(dev, "xvclk mismatched\n");
	ret = clk_prepare_enable(sc830ai->xvclk);
	if (ret < 0) {
		dev_err(dev, "Failed to enable xvclk\n");
		goto err_clk;
	}
	if (!IS_ERR(sc830ai->reset_gpio))
		gpiod_direction_output(sc830ai->reset_gpio, 0);

	ret = regulator_bulk_enable(SC830AI_NUM_SUPPLIES, sc830ai->supplies);
	if (ret < 0) {
		dev_err(dev, "Failed to enable regulators\n");
		goto disable_clk;
	}

	if (!IS_ERR(sc830ai->reset_gpio))
		gpiod_direction_output(sc830ai->reset_gpio, 1);

	usleep_range(5000, 10000);
	if (!IS_ERR(sc830ai->pwdn_gpio))
		gpiod_direction_output(sc830ai->pwdn_gpio, 1);

	if (!IS_ERR(sc830ai->reset_gpio))
		usleep_range(6000, 8000);
	else
		usleep_range(12000, 16000);

	/* 8192 cycles prior to first SCCB transaction */
	delay_us = sc830ai_cal_delay(8192);
	usleep_range(delay_us, delay_us * 2);
	return 0;
err_clk:
	if (!IS_ERR(sc830ai->reset_gpio))
		gpiod_direction_output(sc830ai->reset_gpio, 0);
disable_clk:
	clk_disable_unprepare(sc830ai->xvclk);

	return ret;
}

static void __sc830ai_power_off(struct sc830ai *sc830ai)
{
	int ret;
	struct device *dev = &sc830ai->client->dev;

	if (!IS_ERR(sc830ai->pwdn_gpio))
		gpiod_direction_output(sc830ai->pwdn_gpio, 0);

	clk_disable_unprepare(sc830ai->xvclk);

	if (!IS_ERR(sc830ai->reset_gpio))
		gpiod_direction_output(sc830ai->reset_gpio, 0);

	if (!IS_ERR_OR_NULL(sc830ai->pins_sleep)) {
		ret = pinctrl_select_state(sc830ai->pinctrl,
					   sc830ai->pins_sleep);
		if (ret < 0)
			dev_dbg(dev, "could not set pins\n");
	}
	regulator_bulk_disable(SC830AI_NUM_SUPPLIES, sc830ai->supplies);
}

static int sc830ai_runtime_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct sc830ai *sc830ai = to_sc830ai(sd);

	return __sc830ai_power_on(sc830ai);
}

static int sc830ai_runtime_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct sc830ai *sc830ai = to_sc830ai(sd);

	__sc830ai_power_off(sc830ai);

	return 0;
}

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
static int sc830ai_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);
	struct v4l2_mbus_framefmt *try_fmt =
				v4l2_subdev_get_try_format(sd, fh->pad, 0);
	const struct sc830ai_mode *def_mode = &supported_modes[0];

	mutex_lock(&sc830ai->mutex);
	/* Initialize try_fmt */
	try_fmt->width = def_mode->width;
	try_fmt->height = def_mode->height;
	try_fmt->code = def_mode->bus_fmt;
	try_fmt->field = V4L2_FIELD_NONE;

	mutex_unlock(&sc830ai->mutex);
	/* No crop or compose */

	return 0;
}
#endif

static int sc830ai_enum_frame_interval(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_interval_enum *fie)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);

	if (fie->index >= sc830ai->cfg_num)
		return -EINVAL;

	fie->code = supported_modes[fie->index].bus_fmt;
	fie->width = supported_modes[fie->index].width;
	fie->height = supported_modes[fie->index].height;
	fie->interval = supported_modes[fie->index].max_fps;
	fie->reserved[0] = supported_modes[fie->index].hdr_mode;
	return 0;
}

#define CROP_START(SRC, DST) (((SRC) - (DST)) / 2 / 4 * 4)
#define DST_WIDTH_3840 3840
#define DST_HEIGHT_2160 2160
#define DST_WIDTH_1920 1920
#define DST_HEIGHT_1080 1080

static int sc830ai_get_selection(struct v4l2_subdev *sd,
				 struct v4l2_subdev_pad_config *cfg,
				 struct v4l2_subdev_selection *sel)
{
	struct sc830ai *sc830ai = to_sc830ai(sd);

	if (sel->target == V4L2_SEL_TGT_CROP_BOUNDS) {
		if (sc830ai->cur_mode->width == 3856) {
			sel->r.left = CROP_START(sc830ai->cur_mode->width, DST_WIDTH_3840);
			sel->r.width = DST_WIDTH_3840;
			sel->r.top = CROP_START(sc830ai->cur_mode->height, DST_HEIGHT_2160);
			sel->r.height = DST_HEIGHT_2160;
		} else if (sc830ai->cur_mode->width == 1944) {
			sel->r.left = CROP_START(sc830ai->cur_mode->width, DST_WIDTH_1920);
			sel->r.width = DST_WIDTH_1920;
			sel->r.top = CROP_START(sc830ai->cur_mode->height, DST_HEIGHT_1080);
			sel->r.height = DST_HEIGHT_1080;
		} else {
			sel->r.left = CROP_START(sc830ai->cur_mode->width,
						 sc830ai->cur_mode->width);
			sel->r.width = sc830ai->cur_mode->width;
			sel->r.top = CROP_START(sc830ai->cur_mode->height,
						sc830ai->cur_mode->height);
			sel->r.height = sc830ai->cur_mode->height;
		}
		return 0;
	}
	return -EINVAL;
}

static const struct dev_pm_ops sc830ai_pm_ops = {
	SET_RUNTIME_PM_OPS(sc830ai_runtime_suspend,
			   sc830ai_runtime_resume, NULL)
};

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
static const struct v4l2_subdev_internal_ops sc830ai_internal_ops = {
	.open = sc830ai_open,
};
#endif

static const struct v4l2_subdev_core_ops sc830ai_core_ops = {
	.s_power = sc830ai_s_power,
	.ioctl = sc830ai_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = sc830ai_compat_ioctl32,
#endif
};

static const struct v4l2_subdev_video_ops sc830ai_video_ops = {
	.s_stream = sc830ai_s_stream,
	.g_frame_interval = sc830ai_g_frame_interval,
};

static const struct v4l2_subdev_pad_ops sc830ai_pad_ops = {
	.enum_mbus_code = sc830ai_enum_mbus_code,
	.enum_frame_size = sc830ai_enum_frame_sizes,
	.enum_frame_interval = sc830ai_enum_frame_interval,
	.get_fmt = sc830ai_get_fmt,
	.set_fmt = sc830ai_set_fmt,
	.get_selection = sc830ai_get_selection,
	.get_mbus_config = sc830ai_g_mbus_config,
};

static const struct v4l2_subdev_ops sc830ai_subdev_ops = {
	.core	= &sc830ai_core_ops,
	.video	= &sc830ai_video_ops,
	.pad	= &sc830ai_pad_ops,
};

static void sc830ai_modify_fps_info(struct sc830ai *sc830ai)
{
	const struct sc830ai_mode *mode = sc830ai->cur_mode;

	sc830ai->cur_fps.denominator = mode->max_fps.denominator * sc830ai->cur_vts /
				       mode->vts_def;
}

static int sc830ai_set_ctrl(struct v4l2_ctrl *ctrl)
{
	struct sc830ai *sc830ai = container_of(ctrl->handler,
					     struct sc830ai, ctrl_handler);
	struct i2c_client *client = sc830ai->client;
	s64 max;
	u32 again, dgain, dgain_fine;
	int ret = 0;
	u32 val;

	/* Propagate change of current control to all related controls */
	switch (ctrl->id) {
	case V4L2_CID_VBLANK:
		/* Update max exposure while meeting expected vblanking */
		max = sc830ai->cur_mode->height + ctrl->val - 8;
		__v4l2_ctrl_modify_range(sc830ai->exposure,
					 sc830ai->exposure->minimum, max,
					 sc830ai->exposure->step,
					 sc830ai->exposure->default_value);
		break;
	}

	if (!pm_runtime_get_if_in_use(&client->dev))
		return 0;

	switch (ctrl->id) {
	case V4L2_CID_EXPOSURE:
		if (sc830ai->cur_mode->hdr_mode != NO_HDR)
			goto out_ctrl;
		ret = sc830ai_write_reg(sc830ai->client,
					SC830AI_REG_EXP_LONG_H,
					SC830AI_REG_VALUE_08BIT,
					SC830AI_FETCH_EXP_H(ctrl->val));
		ret |= sc830ai_write_reg(sc830ai->client,
					SC830AI_REG_EXP_LONG_M,
					SC830AI_REG_VALUE_08BIT,
					SC830AI_FETCH_EXP_M(ctrl->val));
		ret |= sc830ai_write_reg(sc830ai->client,
					 SC830AI_REG_EXP_LONG_L,
					 SC830AI_REG_VALUE_08BIT,
					 SC830AI_FETCH_EXP_L(ctrl->val));

		dev_dbg(&client->dev, "set exposure 0x%x\n",
			ctrl->val);
		break;
	case V4L2_CID_ANALOGUE_GAIN:
		if (sc830ai->cur_mode->hdr_mode != NO_HDR)
			goto out_ctrl;
		sc830ai_get_gain_reg(ctrl->val, &again, &dgain, &dgain_fine);
		dev_dbg(&client->dev, "recv_gain:%d set again 0x%x, set dgain 0x%x, dgain_fine 0x%x\n",
			ctrl->val, again, dgain, dgain_fine);

		ret |= sc830ai_write_reg(sc830ai->client,
					SC830AI_REG_AGAIN,
					SC830AI_REG_VALUE_08BIT,
					again);
		ret |= sc830ai_write_reg(sc830ai->client,
					SC830AI_REG_DGAIN,
					SC830AI_REG_VALUE_08BIT,
					dgain);
		ret |= sc830ai_write_reg(sc830ai->client,
					SC830AI_REG_DGAIN_FINE,
					SC830AI_REG_VALUE_08BIT,
					dgain_fine);
		break;
	case V4L2_CID_VBLANK:
		ret = sc830ai_write_reg(sc830ai->client, SC830AI_REG_VTS,
					SC830AI_REG_VALUE_16BIT,
					ctrl->val + sc830ai->cur_mode->height);
		if (!ret)
			sc830ai->cur_vts = ctrl->val + sc830ai->cur_mode->height;
		sc830ai_modify_fps_info(sc830ai);
		dev_dbg(&client->dev, "set vblank 0x%x\n",
			ctrl->val);
		break;
	case V4L2_CID_TEST_PATTERN:
		ret = sc830ai_enable_test_pattern(sc830ai, ctrl->val);
		break;
	case V4L2_CID_HFLIP:
		ret = sc830ai_read_reg(sc830ai->client, SC830AI_FLIP_REG,
				      SC830AI_REG_VALUE_08BIT, &val);
		if (ret)
			break;
		if (ctrl->val)
			val |= SC830AI_MIRROR_MASK;
		else
			val &= ~SC830AI_MIRROR_MASK;
		ret |= sc830ai_write_reg(sc830ai->client, SC830AI_FLIP_REG,
					SC830AI_REG_VALUE_08BIT, val);
		break;
	case V4L2_CID_VFLIP:
		ret = sc830ai_read_reg(sc830ai->client, SC830AI_FLIP_REG,
				      SC830AI_REG_VALUE_08BIT, &val);
		if (ret)
			break;
		if (ctrl->val)
			val |= SC830AI_FLIP_MASK;
		else
			val &= ~SC830AI_FLIP_MASK;
		ret |= sc830ai_write_reg(sc830ai->client, SC830AI_FLIP_REG,
					SC830AI_REG_VALUE_08BIT, val);
		break;
	default:
		dev_warn(&client->dev, "%s Unhandled id:0x%x, val:0x%x\n",
			 __func__, ctrl->id, ctrl->val);
		break;
	}

out_ctrl:
	pm_runtime_put(&client->dev);

	return ret;
}

static const struct v4l2_ctrl_ops sc830ai_ctrl_ops = {
	.s_ctrl = sc830ai_set_ctrl,
};

static int sc830ai_initialize_controls(struct sc830ai *sc830ai)
{
	const struct sc830ai_mode *mode;
	struct v4l2_ctrl_handler *handler;
	s64 exposure_max, vblank_def;
	u64 pixel_rate = 0;
	u32 h_blank;
	int ret;

	handler = &sc830ai->ctrl_handler;
	mode = sc830ai->cur_mode;
	ret = v4l2_ctrl_handler_init(handler, 9);
	if (ret)
		return ret;
	handler->lock = &sc830ai->mutex;

	sc830ai->link_freq = v4l2_ctrl_new_int_menu(handler, NULL,
				V4L2_CID_LINK_FREQ, 0, 0, link_freq_items);
	v4l2_ctrl_s_ctrl(sc830ai->link_freq, mode->mipi_freq_idx);

	/* pixel rate = link frequency * 2 * lanes / BITS_PER_SAMPLE */
	pixel_rate = (u32)link_freq_items[mode->mipi_freq_idx] / mode->bpp * 2 * SC830AI_2LANES;
	sc830ai->pixel_rate = v4l2_ctrl_new_std(handler, NULL,
		V4L2_CID_PIXEL_RATE, 0, SC830AI_MAX_PIXEL_RATE,
		1, pixel_rate);

	h_blank = mode->hts_def - mode->width;
	sc830ai->hblank = v4l2_ctrl_new_std(handler, NULL, V4L2_CID_HBLANK,
				h_blank, h_blank, 1, h_blank);
	if (sc830ai->hblank)
		sc830ai->hblank->flags |= V4L2_CTRL_FLAG_READ_ONLY;

	vblank_def = mode->vts_def - mode->height;
	sc830ai->vblank = v4l2_ctrl_new_std(handler, &sc830ai_ctrl_ops,
				V4L2_CID_VBLANK, vblank_def,
				SC830AI_VTS_MAX - mode->height,
				1, vblank_def);

	 exposure_max = mode->vts_def - 4;	/*vts_def  0x08ca=2250*/
	 sc830ai->exposure = v4l2_ctrl_new_std(handler, &sc830ai_ctrl_ops,
				V4L2_CID_EXPOSURE, SC830AI_EXPOSURE_MIN,
				exposure_max, SC830AI_EXPOSURE_STEP,
				mode->exp_def);	/*exp_def 0x08c0=2240*/

	sc830ai->anal_a_gain = v4l2_ctrl_new_std(handler, &sc830ai_ctrl_ops,
				V4L2_CID_ANALOGUE_GAIN, SC830AI_GAIN_MIN,
				SC830AI_GAIN_MAX, SC830AI_GAIN_STEP,
				SC830AI_GAIN_DEFAULT);

	sc830ai->test_pattern = v4l2_ctrl_new_std_menu_items(handler,
				&sc830ai_ctrl_ops, V4L2_CID_TEST_PATTERN,
				ARRAY_SIZE(sc830ai_test_pattern_menu) - 1,
				0, 0, sc830ai_test_pattern_menu);

	v4l2_ctrl_new_std(handler, &sc830ai_ctrl_ops, V4L2_CID_HFLIP, 0, 1, 1, 0);
	v4l2_ctrl_new_std(handler, &sc830ai_ctrl_ops, V4L2_CID_VFLIP, 0, 1, 1, 0);

	if (handler->error) {
		ret = handler->error;
		dev_err(&sc830ai->client->dev,
		"Failed to init controls(%d)\n", ret);
		goto err_free_handler;
	}

	sc830ai->subdev.ctrl_handler = handler;
	sc830ai->has_init_exp = false;
	sc830ai->cur_fps = mode->max_fps;
	sc830ai->cur_vts = mode->vts_def;

	return 0;

err_free_handler:
	v4l2_ctrl_handler_free(handler);

	return ret;
}

static int sc830ai_check_sensor_id(struct sc830ai *sc830ai,
				  struct i2c_client *client)
{
	struct device *dev = &sc830ai->client->dev;
	u32 id = 0;
	int ret;

	ret = sc830ai_read_reg(client, SC830AI_REG_CHIP_ID,
		SC830AI_REG_VALUE_16BIT, &id);
	if (id != CHIP_ID) {
		dev_err(dev, "Unexpected sensor id(%06x), ret(%d)\n", id, ret);
		return -ENODEV;
	}

	dev_info(dev, "Detected sc830ai id %06x\n", CHIP_ID);

	return 0;
}

static int sc830ai_configure_regulators(struct sc830ai *sc830ai)
{
	unsigned int i;

	for (i = 0; i < SC830AI_NUM_SUPPLIES; i++)
		sc830ai->supplies[i].supply = sc830ai_supply_names[i];

	return devm_regulator_bulk_get(&sc830ai->client->dev,
				       SC830AI_NUM_SUPPLIES,
				       sc830ai->supplies);
}

static int sc830ai_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct device_node *node = dev->of_node;
	struct sc830ai *sc830ai;
	struct v4l2_subdev *sd;
	char facing[2];
	int ret;
	u32 i, hdr_mode = 0;

	dev_info(dev, "driver version: %02x.%02x.%02x",
		DRIVER_VERSION >> 16,
		(DRIVER_VERSION & 0xff00) >> 8,
		DRIVER_VERSION & 0x00ff);

	sc830ai = devm_kzalloc(dev, sizeof(*sc830ai), GFP_KERNEL);
	if (!sc830ai)
		return -ENOMEM;

	ret = of_property_read_u32(node, RKMODULE_CAMERA_MODULE_INDEX,
				   &sc830ai->module_index);
	ret |= of_property_read_string(node, RKMODULE_CAMERA_MODULE_FACING,
				       &sc830ai->module_facing);
	ret |= of_property_read_string(node, RKMODULE_CAMERA_MODULE_NAME,
				       &sc830ai->module_name);
	ret |= of_property_read_string(node, RKMODULE_CAMERA_LENS_NAME,
				       &sc830ai->len_name);
	if (ret) {
		dev_err(dev, "could not get module information!\n");
		return -EINVAL;
	}

	ret = of_property_read_u32(node, OF_CAMERA_HDR_MODE, &hdr_mode);
	if (ret) {
		hdr_mode = NO_HDR;
		dev_warn(dev, " Get hdr mode failed! no hdr default\n");
	}

	sc830ai->client = client;
	sc830ai->cfg_num = ARRAY_SIZE(supported_modes);
	for (i = 0; i < sc830ai->cfg_num; i++) {
		if (hdr_mode == supported_modes[i].hdr_mode) {
			sc830ai->cur_mode = &supported_modes[i];
			break;
		}
	}
	if (i == ARRAY_SIZE(supported_modes))
		sc830ai->cur_mode = &supported_modes[0];

	sc830ai->xvclk = devm_clk_get(dev, "xvclk");
	if (IS_ERR(sc830ai->xvclk)) {
		dev_err(dev, "Failed to get xvclk\n");
		return -EINVAL;
	}

	sc830ai->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_ASIS);
	if (IS_ERR(sc830ai->reset_gpio))
		dev_warn(dev, "Failed to get reset-gpios\n");
	sc830ai->pwdn_gpio = devm_gpiod_get(dev, "pwdn", GPIOD_ASIS);
	if (IS_ERR(sc830ai->pwdn_gpio))
		dev_warn(dev, "Failed to get pwdn_gpios\n");

	sc830ai->pinctrl = devm_pinctrl_get(dev);
	if (!IS_ERR(sc830ai->pinctrl)) {
		sc830ai->pins_default =
			pinctrl_lookup_state(sc830ai->pinctrl,
					     OF_CAMERA_PINCTRL_STATE_DEFAULT);
		if (IS_ERR(sc830ai->pins_default))
			dev_info(dev, "could not get default pinstate\n");

		sc830ai->pins_sleep =
			pinctrl_lookup_state(sc830ai->pinctrl,
					     OF_CAMERA_PINCTRL_STATE_SLEEP);
		if (IS_ERR(sc830ai->pins_sleep))
			dev_info(dev, "could not get sleep pinstate\n");
	} else {
		dev_info(dev, "no pinctrl\n");
	}

	ret = sc830ai_configure_regulators(sc830ai);
	if (ret) {
		dev_err(dev, "Failed to get power regulators\n");
		return ret;
	}

	mutex_init(&sc830ai->mutex);

	sd = &sc830ai->subdev;
	v4l2_i2c_subdev_init(sd, client, &sc830ai_subdev_ops);
	ret = sc830ai_initialize_controls(sc830ai);
	if (ret)
		goto err_destroy_mutex;

	ret = __sc830ai_power_on(sc830ai);
	if (ret)
		goto err_free_handler;

	ret = sc830ai_check_sensor_id(sc830ai, client);
	if (ret)
		goto err_power_off;
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
	sd->internal_ops = &sc830ai_internal_ops;
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE |
		     V4L2_SUBDEV_FL_HAS_EVENTS;
#endif
#if defined(CONFIG_MEDIA_CONTROLLER)
	sc830ai->pad.flags = MEDIA_PAD_FL_SOURCE;
	sd->entity.function = MEDIA_ENT_F_CAM_SENSOR;
	ret = media_entity_pads_init(&sd->entity, 1, &sc830ai->pad);
	if (ret < 0)
		goto err_power_off;
#endif

	memset(facing, 0, sizeof(facing));
	if (strcmp(sc830ai->module_facing, "back") == 0)
		facing[0] = 'b';
	else
		facing[0] = 'f';
	snprintf(sd->name, sizeof(sd->name), "m%02d_%s_%s %s",
		 sc830ai->module_index, facing,
		 SC830AI_NAME, dev_name(sd->dev));

	ret = v4l2_async_register_subdev_sensor_common(sd);
	if (ret) {
		dev_err(dev, "v4l2 async register subdev failed\n");
		goto err_clean_entity;
	}

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_idle(dev);

	return 0;

err_clean_entity:
#if defined(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&sd->entity);
#endif
err_power_off:
	__sc830ai_power_off(sc830ai);
err_free_handler:
	v4l2_ctrl_handler_free(&sc830ai->ctrl_handler);
err_destroy_mutex:
	mutex_destroy(&sc830ai->mutex);

	return ret;
}

static int sc830ai_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct sc830ai *sc830ai = to_sc830ai(sd);

	v4l2_async_unregister_subdev(sd);
#if defined(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&sd->entity);
#endif
	v4l2_ctrl_handler_free(&sc830ai->ctrl_handler);
	mutex_destroy(&sc830ai->mutex);

	pm_runtime_disable(&client->dev);
	if (!pm_runtime_status_suspended(&client->dev))
		__sc830ai_power_off(sc830ai);
	pm_runtime_set_suspended(&client->dev);

	return 0;
}

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id sc830ai_of_match[] = {
	{ .compatible = "smartsens,sc830ai" },
	{},
};
MODULE_DEVICE_TABLE(of, sc830ai_of_match);
#endif

static const struct i2c_device_id sc830ai_match_id[] = {
	{ "smartsens,sc830ai", 0 },
	{ },
};

static struct i2c_driver sc830ai_i2c_driver = {
	.driver = {
		.name = SC830AI_NAME,
		.pm = &sc830ai_pm_ops,
		.of_match_table = of_match_ptr(sc830ai_of_match),
	},
	.probe		= &sc830ai_probe,
	.remove		= &sc830ai_remove,
	.id_table	= sc830ai_match_id,
};

static int __init sensor_mod_init(void)
{
	return i2c_add_driver(&sc830ai_i2c_driver);
}

static void __exit sensor_mod_exit(void)
{
	i2c_del_driver(&sc830ai_i2c_driver);
}

device_initcall_sync(sensor_mod_init);
module_exit(sensor_mod_exit);

MODULE_DESCRIPTION("smartsens,sc830ai sensor driver");
MODULE_LICENSE("GPL");
