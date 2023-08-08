// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Rockchip USBDP Combo PHY with Samsung IP block driver
 *
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd
 */

#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <asm/unaligned.h>
#include <asm/io.h>
#include <clk.h>
#include <dm/device.h>
#include <dm/of_access.h>
#include <dm/read.h>
#include <generic-phy.h>
#include <linux/bitfield.h>
#include <linux/hdmi.h>
#include <linux/media-bus-format.h>
#include <linux/list.h>
#include <asm/gpio.h>
#include <generic-phy.h>
#include <regmap.h>
#include <reset.h>
#include <drm/drm_dp_helper.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"

#define DPTX_VERSION_NUMBER			0x0000
#define DPTX_VERSION_TYPE			0x0004
#define DPTX_ID					0x0008

#define DPTX_CONFIG_REG1			0x0100
#define DPTX_CONFIG_REG2			0x0104
#define DPTX_CONFIG_REG3			0x0108

#define DPTX_CCTL				0x0200
#define FORCE_HPD				BIT(4)
#define DEFAULT_FAST_LINK_TRAIN_EN		BIT(2)
#define ENHANCE_FRAMING_EN			BIT(1)
#define SCRAMBLE_DIS				BIT(0)
#define DPTX_SOFT_RESET_CTRL			0x0204
#define VIDEO_RESET				BIT(5)
#define AUX_RESET				BIT(4)
#define AUDIO_SAMPLER_RESET			BIT(3)
#define PHY_SOFT_RESET				BIT(1)
#define CONTROLLER_RESET			BIT(0)

#define DPTX_VSAMPLE_CTRL			0x0300
#define PIXEL_MODE_SELECT			GENMASK(22, 21)
#define VIDEO_MAPPING				GENMASK(20, 16)
#define VIDEO_STREAM_ENABLE			BIT(5)
#define DPTX_VSAMPLE_STUFF_CTRL1		0x0304
#define DPTX_VSAMPLE_STUFF_CTRL2		0x0308
#define DPTX_VINPUT_POLARITY_CTRL		0x030c
#define DE_IN_POLARITY				BIT(2)
#define HSYNC_IN_POLARITY			BIT(1)
#define VSYNC_IN_POLARITY			BIT(0)
#define DPTX_VIDEO_CONFIG1			0x0310
#define HACTIVE					GENMASK(31, 16)
#define HBLANK					GENMASK(15, 2)
#define I_P					BIT(1)
#define R_V_BLANK_IN_OSC			BIT(0)
#define DPTX_VIDEO_CONFIG2			0x0314
#define VBLANK					GENMASK(31, 16)
#define VACTIVE					GENMASK(15, 0)
#define DPTX_VIDEO_CONFIG3			0x0318
#define H_SYNC_WIDTH				GENMASK(31, 16)
#define H_FRONT_PORCH				GENMASK(15, 0)
#define DPTX_VIDEO_CONFIG4			0x031c
#define V_SYNC_WIDTH				GENMASK(31, 16)
#define V_FRONT_PORCH				GENMASK(15, 0)
#define DPTX_VIDEO_CONFIG5			0x0320
#define INIT_THRESHOLD_HI			GENMASK(22, 21)
#define AVERAGE_BYTES_PER_TU_FRAC		GENMASK(19, 16)
#define INIT_THRESHOLD				GENMASK(13, 7)
#define AVERAGE_BYTES_PER_TU			GENMASK(6, 0)
#define DPTX_VIDEO_MSA1				0x0324
#define VSTART					GENMASK(31, 16)
#define HSTART					GENMASK(15, 0)
#define DPTX_VIDEO_MSA2				0x0328
#define MISC0					GENMASK(31, 24)
#define DPTX_VIDEO_MSA3				0x032c
#define MISC1					GENMASK(31, 24)
#define DPTX_VIDEO_HBLANK_INTERVAL		0x0330
#define HBLANK_INTERVAL_EN			BIT(16)
#define HBLANK_INTERVAL				GENMASK(15, 0)

#define DPTX_AUD_CONFIG1			0x0400
#define AUDIO_TIMESTAMP_VERSION_NUM		GENMASK(29, 24)
#define AUDIO_PACKET_ID				GENMASK(23, 16)
#define AUDIO_MUTE				BIT(15)
#define NUM_CHANNELS				GENMASK(14, 12)
#define HBR_MODE_ENABLE				BIT(10)
#define AUDIO_DATA_WIDTH			GENMASK(9, 5)
#define AUDIO_DATA_IN_EN			GENMASK(4, 1)
#define AUDIO_INF_SELECT			BIT(0)

#define DPTX_SDP_VERTICAL_CTRL			0x0500
#define EN_VERTICAL_SDP				BIT(2)
#define EN_AUDIO_STREAM_SDP			BIT(1)
#define EN_AUDIO_TIMESTAMP_SDP			BIT(0)
#define DPTX_SDP_HORIZONTAL_CTRL		0x0504
#define EN_HORIZONTAL_SDP			BIT(2)
#define DPTX_SDP_STATUS_REGISTER		0x0508
#define DPTX_SDP_MANUAL_CTRL			0x050c
#define DPTX_SDP_STATUS_EN			0x0510

#define DPTX_SDP_REGISTER_BANK			0x0600
#define SDP_REGS				GENMASK(31, 0)

#define DPTX_PHYIF_CTRL				0x0a00
#define PHY_WIDTH				BIT(25)
#define PHY_POWERDOWN				GENMASK(20, 17)
#define PHY_BUSY				GENMASK(15, 12)
#define SSC_DIS					BIT(16)
#define XMIT_ENABLE				GENMASK(11, 8)
#define PHY_LANES				GENMASK(7, 6)
#define PHY_RATE				GENMASK(5, 4)
#define TPS_SEL					GENMASK(3, 0)
#define DPTX_PHY_TX_EQ				0x0a04
#define DPTX_CUSTOMPAT0				0x0a08
#define DPTX_CUSTOMPAT1				0x0a0c
#define DPTX_CUSTOMPAT2				0x0a10
#define DPTX_HBR2_COMPLIANCE_SCRAMBLER_RESET	0x0a14
#define DPTX_PHYIF_PWRDOWN_CTRL			0x0a18

#define DPTX_AUX_CMD				0x0b00
#define AUX_CMD_TYPE				GENMASK(31, 28)
#define AUX_ADDR				GENMASK(27, 8)
#define I2C_ADDR_ONLY				BIT(4)
#define AUX_LEN_REQ				GENMASK(3, 0)
#define DPTX_AUX_STATUS				0x0b04
#define AUX_TIMEOUT				BIT(17)
#define AUX_BYTES_READ				GENMASK(23, 19)
#define AUX_STATUS				GENMASK(7, 4)
#define DPTX_AUX_DATA0				0x0b08
#define DPTX_AUX_DATA1				0x0b0c
#define DPTX_AUX_DATA2				0x0b10
#define DPTX_AUX_DATA3				0x0b14

#define DPTX_GENERAL_INTERRUPT			0x0d00
#define VIDEO_FIFO_OVERFLOW_STREAM0		BIT(6)
#define AUDIO_FIFO_OVERFLOW_STREAM0		BIT(5)
#define SDP_EVENT_STREAM0			BIT(4)
#define AUX_CMD_INVALID				BIT(3)
#define AUX_REPLY_EVENT				BIT(1)
#define HPD_EVENT				BIT(0)
#define DPTX_GENERAL_INTERRUPT_ENABLE		0x0d04
#define AUX_REPLY_EVENT_EN			BIT(1)
#define HPD_EVENT_EN				BIT(0)
#define DPTX_HPD_STATUS				0x0d08
#define HPD_STATE				GENMASK(11, 9)
#define HPD_STATUS				BIT(8)
#define HPD_HOT_UNPLUG				BIT(2)
#define HPD_HOT_PLUG				BIT(1)
#define HPD_IRQ					BIT(0)
#define DPTX_HPD_INTERRUPT_ENABLE		0x0d0c
#define HPD_UNPLUG_ERR_EN			BIT(3)
#define HPD_UNPLUG_EN				BIT(2)
#define HPD_PLUG_EN				BIT(1)
#define HPD_IRQ_EN				BIT(0)

#define DPTX_MAX_REGISTER			DPTX_HPD_INTERRUPT_ENABLE

#define SDP_REG_BANK_SIZE			16

struct drm_dp_link_caps {
	bool enhanced_framing;
	bool tps3_supported;
	bool tps4_supported;
	bool channel_coding;
	bool ssc;
};

struct drm_dp_link_train_set {
	unsigned int voltage_swing[4];
	unsigned int pre_emphasis[4];
};

struct drm_dp_link_train {
	struct drm_dp_link_train_set request;
	struct drm_dp_link_train_set adjust;
	bool clock_recovered;
	bool channel_equalized;
};

struct dw_dp_link {
	u8 dpcd[DP_RECEIVER_CAP_SIZE];
	unsigned char revision;
	unsigned int rate;
	unsigned int lanes;
	struct drm_dp_link_caps caps;
	struct drm_dp_link_train train;
	u8 sink_count;
	u8 vsc_sdp_extension_for_colorimetry_supported;
};

struct dw_dp_video {
	struct drm_display_mode mode;
	u32 bus_format;
	u8 video_mapping;
	u8 pixel_mode;
	u8 color_format;
	u8 bpc;
	u8 bpp;
};

struct dw_dp_sdp {
	struct dp_sdp_header header;
	u8 db[32];
	unsigned long flags;
};

struct dw_dp {
	struct rockchip_connector connector;
	struct udevice *dev;
	struct regmap *regmap;
	struct phy phy;
	struct reset_ctl reset;
	int id;

	struct gpio_desc hpd_gpio;
	struct drm_dp_aux aux;
	struct dw_dp_link link;
	struct dw_dp_video video;

	bool force_hpd;
	bool force_output;
};

enum {
	SOURCE_STATE_IDLE,
	SOURCE_STATE_UNPLUG,
	SOURCE_STATE_HPD_TIMEOUT = 4,
	SOURCE_STATE_PLUG = 7
};

enum {
	DPTX_VM_RGB_6BIT,
	DPTX_VM_RGB_8BIT,
	DPTX_VM_RGB_10BIT,
	DPTX_VM_RGB_12BIT,
	DPTX_VM_RGB_16BIT,
	DPTX_VM_YCBCR444_8BIT,
	DPTX_VM_YCBCR444_10BIT,
	DPTX_VM_YCBCR444_12BIT,
	DPTX_VM_YCBCR444_16BIT,
	DPTX_VM_YCBCR422_8BIT,
	DPTX_VM_YCBCR422_10BIT,
	DPTX_VM_YCBCR422_12BIT,
	DPTX_VM_YCBCR422_16BIT,
	DPTX_VM_YCBCR420_8BIT,
	DPTX_VM_YCBCR420_10BIT,
	DPTX_VM_YCBCR420_12BIT,
	DPTX_VM_YCBCR420_16BIT,
};

enum {
	DPTX_MP_SINGLE_PIXEL,
	DPTX_MP_DUAL_PIXEL,
	DPTX_MP_QUAD_PIXEL,
};

enum {
	DPTX_SDP_VERTICAL_INTERVAL = BIT(0),
	DPTX_SDP_HORIZONTAL_INTERVAL = BIT(1),
};

enum {
	DPTX_PHY_PATTERN_NONE,
	DPTX_PHY_PATTERN_TPS_1,
	DPTX_PHY_PATTERN_TPS_2,
	DPTX_PHY_PATTERN_TPS_3,
	DPTX_PHY_PATTERN_TPS_4,
	DPTX_PHY_PATTERN_SERM,
	DPTX_PHY_PATTERN_PBRS7,
	DPTX_PHY_PATTERN_CUSTOM_80BIT,
	DPTX_PHY_PATTERN_CP2520_1,
	DPTX_PHY_PATTERN_CP2520_2,
};

enum {
	DPTX_PHYRATE_RBR,
	DPTX_PHYRATE_HBR,
	DPTX_PHYRATE_HBR2,
	DPTX_PHYRATE_HBR3,
};

struct dw_dp_output_format {
	u32 bus_format;
	u32 color_format;
	u8 video_mapping;
	u8 bpc;
	u8 bpp;
};

static const struct dw_dp_output_format possible_output_fmts[] = {
	{ MEDIA_BUS_FMT_RGB101010_1X30, DRM_COLOR_FORMAT_RGB444,
	  DPTX_VM_RGB_10BIT, 10, 30 },
	{ MEDIA_BUS_FMT_RGB888_1X24, DRM_COLOR_FORMAT_RGB444,
	  DPTX_VM_RGB_8BIT, 8, 24 },
	{ MEDIA_BUS_FMT_YUV10_1X30, DRM_COLOR_FORMAT_YCRCB444,
	  DPTX_VM_YCBCR444_10BIT, 10, 30 },
	{ MEDIA_BUS_FMT_YUV8_1X24, DRM_COLOR_FORMAT_YCRCB444,
	  DPTX_VM_YCBCR444_8BIT, 8, 24},
	{ MEDIA_BUS_FMT_YUYV10_1X20, DRM_COLOR_FORMAT_YCRCB422,
	  DPTX_VM_YCBCR422_10BIT, 10, 20 },
	{ MEDIA_BUS_FMT_YUYV8_1X16, DRM_COLOR_FORMAT_YCRCB422,
	  DPTX_VM_YCBCR422_8BIT, 8, 16 },
	{ MEDIA_BUS_FMT_UYYVYY10_0_5X30, DRM_COLOR_FORMAT_YCRCB420,
	  DPTX_VM_YCBCR420_10BIT, 10, 15 },
	{ MEDIA_BUS_FMT_UYYVYY8_0_5X24, DRM_COLOR_FORMAT_YCRCB420,
	  DPTX_VM_YCBCR420_8BIT, 8, 12 },
	{ MEDIA_BUS_FMT_RGB666_1X24_CPADHI, DRM_COLOR_FORMAT_RGB444,
	  DPTX_VM_RGB_6BIT, 6, 18 },
};

static int dw_dp_aux_write_data(struct dw_dp *dp, const u8 *buffer, size_t size)
{
	size_t i, j;

	for (i = 0; i < DIV_ROUND_UP(size, 4); i++) {
		size_t num = min_t(size_t, size - i * 4, 4);
		u32 value = 0;

		for (j = 0; j < num; j++)
			value |= buffer[i * 4 + j] << (j * 8);

		regmap_write(dp->regmap, DPTX_AUX_DATA0 + i * 4, value);
	}

	return size;
}

static int dw_dp_aux_read_data(struct dw_dp *dp, u8 *buffer, size_t size)
{
	size_t i, j;

	for (i = 0; i < DIV_ROUND_UP(size, 4); i++) {
		size_t num = min_t(size_t, size - i * 4, 4);
		u32 value;

		regmap_read(dp->regmap, DPTX_AUX_DATA0 + i * 4, &value);

		for (j = 0; j < num; j++)
			buffer[i * 4 + j] = value >> (j * 8);
	}

	return size;
}

static ssize_t dw_dp_aux_transfer(struct drm_dp_aux *aux,
				  struct drm_dp_aux_msg *msg)
{
	u32 status, value;
	ssize_t ret = 0;
	int timeout = 0;
	struct dw_dp *dp = dev_get_priv(aux->dev);

	if (WARN_ON(msg->size > 16))
		return -E2BIG;

	switch (msg->request & ~DP_AUX_I2C_MOT) {
	case DP_AUX_NATIVE_WRITE:
	case DP_AUX_I2C_WRITE:
	case DP_AUX_I2C_WRITE_STATUS_UPDATE:
		ret = dw_dp_aux_write_data(dp, msg->buffer, msg->size);
		if (ret < 0)
			return ret;
		break;
	case DP_AUX_NATIVE_READ:
	case DP_AUX_I2C_READ:
		break;
	default:
		return -EINVAL;
	}

	if (msg->size > 0)
		value = FIELD_PREP(AUX_LEN_REQ, msg->size - 1);
	else
		value = FIELD_PREP(I2C_ADDR_ONLY, 1);

	value |= FIELD_PREP(AUX_CMD_TYPE, msg->request);
	value |= FIELD_PREP(AUX_ADDR, msg->address);
	regmap_write(dp->regmap, DPTX_AUX_CMD, value);

	timeout = regmap_read_poll_timeout(dp->regmap, DPTX_GENERAL_INTERRUPT,
					   status, status & AUX_REPLY_EVENT,
					   200, 10);

	if (timeout) {
		printf("timeout waiting for AUX reply\n");
		return -ETIMEDOUT;
	}
	regmap_write(dp->regmap, DPTX_GENERAL_INTERRUPT, AUX_REPLY_EVENT);

	regmap_read(dp->regmap, DPTX_AUX_STATUS, &value);
	if (value & AUX_TIMEOUT) {
		printf("aux timeout\n");
		return -ETIMEDOUT;
	}

	msg->reply = FIELD_GET(AUX_STATUS, value);

	if (msg->size > 0 && msg->reply == DP_AUX_NATIVE_REPLY_ACK) {
		if (msg->request & DP_AUX_I2C_READ) {
			size_t count = FIELD_GET(AUX_BYTES_READ, value) - 1;

			if (count != msg->size) {
				printf("aux fail to read %lu bytes\n", count);
				return -EBUSY;
			}

			ret = dw_dp_aux_read_data(dp, msg->buffer, count);
			if (ret < 0)
				return ret;
		}
	}

	return ret;
}

static bool dw_dp_bandwidth_ok(struct dw_dp *dp,
			       const struct drm_display_mode *mode, u32 bpp,
			       unsigned int lanes, unsigned int rate)
{
	u32 max_bw, req_bw;

	req_bw = mode->clock * bpp / 8;
	max_bw = lanes * rate;
	if (req_bw > max_bw)
		return false;

	return true;
}

static void dw_dp_hpd_init(struct dw_dp *dp)
{
	if (dm_gpio_is_valid(&dp->hpd_gpio) || dp->force_hpd) {
		regmap_update_bits(dp->regmap, DPTX_CCTL, FORCE_HPD,
				   FIELD_PREP(FORCE_HPD, 1));
		return;
	}

	/* Enable all HPD interrupts */
	regmap_update_bits(dp->regmap, DPTX_HPD_INTERRUPT_ENABLE,
			   HPD_UNPLUG_EN | HPD_PLUG_EN | HPD_IRQ_EN,
			   FIELD_PREP(HPD_UNPLUG_EN, 1) |
			   FIELD_PREP(HPD_PLUG_EN, 1) |
			   FIELD_PREP(HPD_IRQ_EN, 1));

	/* Enable all top-level interrupts */
	regmap_update_bits(dp->regmap, DPTX_GENERAL_INTERRUPT_ENABLE,
			   HPD_EVENT_EN, FIELD_PREP(HPD_EVENT_EN, 1));
}

static void dw_dp_aux_init(struct dw_dp *dp)
{
	regmap_update_bits(dp->regmap, DPTX_SOFT_RESET_CTRL, AUX_RESET,
			   FIELD_PREP(AUX_RESET, 1));
	udelay(10);
	regmap_update_bits(dp->regmap, DPTX_SOFT_RESET_CTRL, AUX_RESET,
			   FIELD_PREP(AUX_RESET, 0));

	regmap_update_bits(dp->regmap, DPTX_GENERAL_INTERRUPT_ENABLE,
			   AUX_REPLY_EVENT_EN,
			   FIELD_PREP(AUX_REPLY_EVENT_EN, 1));
}

static void dw_dp_init(struct dw_dp *dp)
{
	regmap_update_bits(dp->regmap, DPTX_SOFT_RESET_CTRL, CONTROLLER_RESET,
			   FIELD_PREP(CONTROLLER_RESET, 1));
	udelay(10);
	regmap_update_bits(dp->regmap, DPTX_SOFT_RESET_CTRL, CONTROLLER_RESET,
			   FIELD_PREP(CONTROLLER_RESET, 0));

	regmap_update_bits(dp->regmap, DPTX_SOFT_RESET_CTRL, PHY_SOFT_RESET,
			   FIELD_PREP(PHY_SOFT_RESET, 1));
	udelay(10);
	regmap_update_bits(dp->regmap, DPTX_SOFT_RESET_CTRL, PHY_SOFT_RESET,
			   FIELD_PREP(PHY_SOFT_RESET, 0));

	regmap_update_bits(dp->regmap, DPTX_CCTL, DEFAULT_FAST_LINK_TRAIN_EN,
			   FIELD_PREP(DEFAULT_FAST_LINK_TRAIN_EN, 0));

	dw_dp_hpd_init(dp);
	dw_dp_aux_init(dp);
}

static void dw_dp_phy_set_pattern(struct dw_dp *dp, u32 pattern)
{
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, TPS_SEL,
			   FIELD_PREP(TPS_SEL, pattern));
}

static void dw_dp_phy_xmit_enable(struct dw_dp *dp, u32 lanes)
{
	u32 xmit_enable;

	switch (lanes) {
	case 4:
	case 2:
	case 1:
		xmit_enable = GENMASK(lanes - 1, 0);
		break;
	case 0:
	default:
		xmit_enable = 0;
		break;
	}

	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, XMIT_ENABLE,
			   FIELD_PREP(XMIT_ENABLE, xmit_enable));
}

static int dw_dp_link_power_up(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	u8 value;
	int ret;

	if (link->revision < 0x11)
		return 0;

	ret = drm_dp_dpcd_readb(&dp->aux, DP_SET_POWER, &value);
	if (ret < 0)
		return ret;

	value &= ~DP_SET_POWER_MASK;
	value |= DP_SET_POWER_D0;

	ret = drm_dp_dpcd_writeb(&dp->aux, DP_SET_POWER, value);
	if (ret < 0)
		return ret;

	udelay(1000);
	return 0;
}

static int dw_dp_link_probe(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	u8 dpcd;
	int ret;

	ret = drm_dp_read_dpcd_caps(&dp->aux, link->dpcd);
	if (ret < 0)
		return ret;

	ret = drm_dp_dpcd_readb(&dp->aux, DP_DPRX_FEATURE_ENUMERATION_LIST,
				&dpcd);
	if (ret < 0)
		return ret;

	link->vsc_sdp_extension_for_colorimetry_supported =
		!!(dpcd & DP_VSC_SDP_EXT_FOR_COLORIMETRY_SUPPORTED);

	link->revision = link->dpcd[DP_DPCD_REV];
	link->rate = min_t(u32, dp->phy.attrs.max_link_rate * 100,
			   drm_dp_max_link_rate(link->dpcd));
	link->lanes = min_t(u8, dp->phy.attrs.bus_width,
			    drm_dp_max_lane_count(link->dpcd));

	link->caps.enhanced_framing = drm_dp_enhanced_frame_cap(link->dpcd);
	link->caps.tps3_supported = drm_dp_tps3_supported(link->dpcd);
	link->caps.tps4_supported = drm_dp_tps4_supported(link->dpcd);
	link->caps.channel_coding = drm_dp_channel_coding_supported(link->dpcd);
	link->caps.ssc = !!(link->dpcd[DP_MAX_DOWNSPREAD] &
			    DP_MAX_DOWNSPREAD_0_5);

	return 0;
}

static int dw_dp_link_train_update_vs_emph(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	struct drm_dp_link_train_set *request = &link->train.request;
	union phy_configure_opts phy_cfg;
	unsigned int lanes = link->lanes, *vs, *pe;
	u8 buf[4];
	int i, ret;

	vs = request->voltage_swing;
	pe = request->pre_emphasis;

	for (i = 0; i < lanes; i++) {
		phy_cfg.dp.voltage[i] = vs[i];
		phy_cfg.dp.pre[i] = pe[i];
	}
	phy_cfg.dp.lanes = lanes;
	phy_cfg.dp.link_rate = link->rate / 100;
	phy_cfg.dp.set_lanes = false;
	phy_cfg.dp.set_rate = false;
	phy_cfg.dp.set_voltages = true;
	ret = generic_phy_configure(&dp->phy, &phy_cfg);
	if (ret)
		return ret;

	for (i = 0; i < lanes; i++)
		buf[i] = (vs[i] << DP_TRAIN_VOLTAGE_SWING_SHIFT) |
			 (pe[i] << DP_TRAIN_PRE_EMPHASIS_SHIFT);
	ret = drm_dp_dpcd_write(&dp->aux, DP_TRAINING_LANE0_SET, buf, lanes);
	if (ret < 0)
		return ret;

	return 0;
}

static int dw_dp_link_configure(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	union phy_configure_opts phy_cfg;
	u8 buf[2];
	int ret, phy_rate;

	/* Move PHY to P3 */
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_POWERDOWN,
			   FIELD_PREP(PHY_POWERDOWN, 0x3));

	phy_cfg.dp.lanes = link->lanes;
	phy_cfg.dp.link_rate = link->rate / 100;
	phy_cfg.dp.ssc = link->caps.ssc;
	phy_cfg.dp.set_lanes = true;
	phy_cfg.dp.set_rate = true;
	phy_cfg.dp.set_voltages = false;
	ret = generic_phy_configure(&dp->phy, &phy_cfg);
	if (ret)
		return ret;

	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_LANES,
			   FIELD_PREP(PHY_LANES, link->lanes / 2));

	switch (link->rate) {
	case 810000:
		phy_rate = DPTX_PHYRATE_HBR3;
		break;
	case 540000:
		phy_rate = DPTX_PHYRATE_HBR2;
		break;
	case 270000:
		phy_rate = DPTX_PHYRATE_HBR;
		break;
	case 162000:
	default:
		phy_rate = DPTX_PHYRATE_RBR;
		break;
	}
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_RATE,
			   FIELD_PREP(PHY_RATE, phy_rate));

	/* Move PHY to P0 */
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_POWERDOWN,
			   FIELD_PREP(PHY_POWERDOWN, 0x0));

	dw_dp_phy_xmit_enable(dp, link->lanes);

	buf[0] = drm_dp_link_rate_to_bw_code(link->rate);
	buf[1] = link->lanes;

	if (link->caps.enhanced_framing) {
		buf[1] |= DP_LANE_COUNT_ENHANCED_FRAME_EN;
		regmap_update_bits(dp->regmap, DPTX_CCTL, ENHANCE_FRAMING_EN,
				   FIELD_PREP(ENHANCE_FRAMING_EN, 1));
	} else {
		regmap_update_bits(dp->regmap, DPTX_CCTL, ENHANCE_FRAMING_EN,
				   FIELD_PREP(ENHANCE_FRAMING_EN, 0));
	}

	ret = drm_dp_dpcd_write(&dp->aux, DP_LINK_BW_SET, buf, sizeof(buf));
	if (ret < 0)
		return ret;

	buf[0] = link->caps.ssc ? DP_SPREAD_AMP_0_5 : 0;
	buf[1] = link->caps.channel_coding ? DP_SET_ANSI_8B10B : 0;

	ret = drm_dp_dpcd_write(&dp->aux, DP_DOWNSPREAD_CTRL, buf,
				sizeof(buf));
	if (ret < 0)
		return ret;

	return 0;
}

static void dw_dp_link_train_init(struct drm_dp_link_train *train)
{
	struct drm_dp_link_train_set *request = &train->request;
	struct drm_dp_link_train_set *adjust = &train->adjust;
	unsigned int i;

	for (i = 0; i < 4; i++) {
		request->voltage_swing[i] = 0;
		adjust->voltage_swing[i] = 0;

		request->pre_emphasis[i] = 0;
		adjust->pre_emphasis[i] = 0;
	}

	train->clock_recovered = false;
	train->channel_equalized = false;
}

static int dw_dp_link_train_set_pattern(struct dw_dp *dp, u32 pattern)
{
	u8 buf = 0;
	int ret;

	if (pattern && pattern != DP_TRAINING_PATTERN_4) {
		buf |= DP_LINK_SCRAMBLING_DISABLE;

		regmap_update_bits(dp->regmap, DPTX_CCTL, SCRAMBLE_DIS,
				   FIELD_PREP(SCRAMBLE_DIS, 1));
	} else {
		regmap_update_bits(dp->regmap, DPTX_CCTL, SCRAMBLE_DIS,
				   FIELD_PREP(SCRAMBLE_DIS, 0));
	}

	switch (pattern) {
	case DP_TRAINING_PATTERN_DISABLE:
		dw_dp_phy_set_pattern(dp, DPTX_PHY_PATTERN_NONE);
		break;
	case DP_TRAINING_PATTERN_1:
		dw_dp_phy_set_pattern(dp, DPTX_PHY_PATTERN_TPS_1);
		break;
	case DP_TRAINING_PATTERN_2:
		dw_dp_phy_set_pattern(dp, DPTX_PHY_PATTERN_TPS_2);
		break;
	case DP_TRAINING_PATTERN_3:
		dw_dp_phy_set_pattern(dp, DPTX_PHY_PATTERN_TPS_3);
		break;
	case DP_TRAINING_PATTERN_4:
		dw_dp_phy_set_pattern(dp, DPTX_PHY_PATTERN_TPS_4);
		break;
	default:
		return -EINVAL;
	}

	ret = drm_dp_dpcd_writeb(&dp->aux, DP_TRAINING_PATTERN_SET,
				 buf | pattern);
	if (ret < 0)
		return ret;

	return 0;
}

static void dw_dp_link_get_adjustments(struct dw_dp_link *link,
				       u8 status[DP_LINK_STATUS_SIZE])
{
	struct drm_dp_link_train_set *adjust = &link->train.adjust;
	unsigned int i;

	for (i = 0; i < link->lanes; i++) {
		adjust->voltage_swing[i] =
			drm_dp_get_adjust_request_voltage(status, i) >>
				DP_TRAIN_VOLTAGE_SWING_SHIFT;

		adjust->pre_emphasis[i] =
			drm_dp_get_adjust_request_pre_emphasis(status, i) >>
				DP_TRAIN_PRE_EMPHASIS_SHIFT;
	}
}

static void dw_dp_link_train_adjust(struct drm_dp_link_train *train)
{
	struct drm_dp_link_train_set *request = &train->request;
	struct drm_dp_link_train_set *adjust = &train->adjust;
	unsigned int i;

	for (i = 0; i < 4; i++)
		if (request->voltage_swing[i] != adjust->voltage_swing[i])
			request->voltage_swing[i] = adjust->voltage_swing[i];

	for (i = 0; i < 4; i++)
		if (request->pre_emphasis[i] != adjust->pre_emphasis[i])
			request->pre_emphasis[i] = adjust->pre_emphasis[i];
}

static int dw_dp_link_clock_recovery(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	u8 status[DP_LINK_STATUS_SIZE];
	unsigned int tries = 0;
	int ret;

	ret = dw_dp_link_train_set_pattern(dp, DP_TRAINING_PATTERN_1);
	if (ret)
		return ret;

	for (;;) {
		ret = dw_dp_link_train_update_vs_emph(dp);
		if (ret)
			return ret;

		drm_dp_link_train_clock_recovery_delay(link->dpcd);

		ret = drm_dp_dpcd_read_link_status(&dp->aux, status);
		if (ret < 0) {
			dev_err(dp->dev, "failed to read link status: %d\n",
				ret);
			return ret;
		}

		if (drm_dp_clock_recovery_ok(status, link->lanes)) {
			link->train.clock_recovered = true;
			break;
		}

		dw_dp_link_get_adjustments(link, status);

		if (link->train.request.voltage_swing[0] ==
		    link->train.adjust.voltage_swing[0])
			tries++;
		else
			tries = 0;

		if (tries == 5)
			break;

		dw_dp_link_train_adjust(&link->train);
	}

	return 0;
}

static int dw_dp_link_channel_equalization(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	u8 status[DP_LINK_STATUS_SIZE], pattern;
	unsigned int tries;
	int ret;

	if (link->caps.tps4_supported)
		pattern = DP_TRAINING_PATTERN_4;
	else if (link->caps.tps3_supported)
		pattern = DP_TRAINING_PATTERN_3;
	else
		pattern = DP_TRAINING_PATTERN_2;
	ret = dw_dp_link_train_set_pattern(dp, pattern);
	if (ret)
		return ret;

	for (tries = 1; tries < 5; tries++) {
		ret = dw_dp_link_train_update_vs_emph(dp);
		if (ret)
			return ret;

		drm_dp_link_train_channel_eq_delay(link->dpcd);

		ret = drm_dp_dpcd_read_link_status(&dp->aux, status);
		if (ret < 0)
			return ret;

		if (!drm_dp_clock_recovery_ok(status, link->lanes)) {
			dev_err(dp->dev,
				"clock recovery lost while eq\n");
			link->train.clock_recovered = false;
			break;
		}

		if (drm_dp_channel_eq_ok(status, link->lanes)) {
			link->train.channel_equalized = true;
			break;
		}

		dw_dp_link_get_adjustments(link, status);
		dw_dp_link_train_adjust(&link->train);
	}

	return 0;
}

static int dw_dp_link_downgrade(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	struct dw_dp_video *video = &dp->video;

	switch (link->rate) {
	case 162000:
		return -EINVAL;
	case 270000:
		link->rate = 162000;
		break;
	case 540000:
		link->rate = 270000;
		break;
	case 810000:
		link->rate = 540000;
		break;
	}

	if (!dw_dp_bandwidth_ok(dp, &video->mode, video->bpp, link->lanes,
				link->rate))
		return -E2BIG;

	return 0;
}

static int dw_dp_link_train(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	int ret;

retry:
	dw_dp_link_train_init(&link->train);

	printf("training link: %u lane%s at %u MHz\n",
	       link->lanes, (link->lanes > 1) ? "s" : "", link->rate / 100);

	ret = dw_dp_link_configure(dp);
	if (ret < 0) {
		dev_err(dp->dev, "failed to configure DP link: %d\n", ret);
		return ret;
	}

	ret = dw_dp_link_clock_recovery(dp);
	if (ret < 0) {
		dev_err(dp->dev, "clock recovery failed: %d\n", ret);
		goto out;
	}

	if (!link->train.clock_recovered) {
		dev_err(dp->dev, "clock recovery failed, downgrading link\n");

		ret = dw_dp_link_downgrade(dp);
		if (ret < 0)
			goto out;
		else
			goto retry;
	}

	printf("clock recovery succeeded\n");

	ret = dw_dp_link_channel_equalization(dp);
	if (ret < 0) {
		dev_err(dp->dev, "channel equalization failed: %d\n", ret);
		goto out;
	}

	if (!link->train.channel_equalized) {
		dev_err(dp->dev,
			"channel equalization failed, downgrading link\n");

		ret = dw_dp_link_downgrade(dp);
		if (ret < 0)
			goto out;
		else
			goto retry;
	}

	printf("channel equalization succeeded\n");

out:
	dw_dp_link_train_set_pattern(dp, DP_TRAINING_PATTERN_DISABLE);
	return ret;
}

static int dw_dp_link_enable(struct dw_dp *dp)
{
	int ret;

	ret = dw_dp_link_power_up(dp);
	if (ret < 0)
		return ret;

	ret = dw_dp_link_train(dp);
	if (ret < 0) {
		dev_err(dp->dev, "link training failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_dp_set_phy_default_config(struct dw_dp *dp)
{
	struct dw_dp_link *link = &dp->link;
	union phy_configure_opts phy_cfg;
	int ret, i, phy_rate;

	link->vsc_sdp_extension_for_colorimetry_supported = false;
	link->rate = 270000;
	link->lanes = dp->phy.attrs.bus_width;

	link->caps.enhanced_framing = true;
	link->caps.channel_coding = true;
	link->caps.ssc = true;

	/* Move PHY to P3 */
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_POWERDOWN,
			   FIELD_PREP(PHY_POWERDOWN, 0x3));

	for (i = 0; i < link->lanes; i++) {
		phy_cfg.dp.voltage[i] = 3;
		phy_cfg.dp.pre[i] = 0;
	}
	phy_cfg.dp.lanes = link->lanes;
	phy_cfg.dp.link_rate = link->rate / 100;
	phy_cfg.dp.ssc = link->caps.ssc;
	phy_cfg.dp.set_lanes = true;
	phy_cfg.dp.set_rate = true;
	phy_cfg.dp.set_voltages = true;
	ret = generic_phy_configure(&dp->phy, &phy_cfg);
	if (ret)
		return ret;

	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_LANES,
			   FIELD_PREP(PHY_LANES, link->lanes / 2));

	switch (link->rate) {
	case 810000:
		phy_rate = DPTX_PHYRATE_HBR3;
		break;
	case 540000:
		phy_rate = DPTX_PHYRATE_HBR2;
		break;
	case 270000:
		phy_rate = DPTX_PHYRATE_HBR;
		break;
	case 162000:
	default:
		phy_rate = DPTX_PHYRATE_RBR;
		break;
	}
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_RATE,
			   FIELD_PREP(PHY_RATE, phy_rate));

	/* Move PHY to P0 */
	regmap_update_bits(dp->regmap, DPTX_PHYIF_CTRL, PHY_POWERDOWN,
			   FIELD_PREP(PHY_POWERDOWN, 0x0));

	dw_dp_phy_xmit_enable(dp, link->lanes);

	regmap_update_bits(dp->regmap, DPTX_CCTL, ENHANCE_FRAMING_EN,
			   FIELD_PREP(ENHANCE_FRAMING_EN, 1));

	dw_dp_phy_set_pattern(dp, DPTX_PHY_PATTERN_NONE);
	return 0;
}

static int dw_dp_send_sdp(struct dw_dp *dp, struct dw_dp_sdp *sdp)
{
	const u8 *payload = sdp->db;
	u32 reg;
	int i, nr = 0;

	reg = DPTX_SDP_REGISTER_BANK + nr * 9 * 4;

	/* SDP header */
	regmap_write(dp->regmap, reg, get_unaligned_le32(&sdp->header));

	/* SDP data payload */
	for (i = 1; i < 9; i++, payload += 4)
		regmap_write(dp->regmap, reg + i * 4,
			     FIELD_PREP(SDP_REGS, get_unaligned_le32(payload)));

	if (sdp->flags & DPTX_SDP_VERTICAL_INTERVAL)
		regmap_update_bits(dp->regmap, DPTX_SDP_VERTICAL_CTRL,
				   EN_VERTICAL_SDP << nr,
				   EN_VERTICAL_SDP << nr);

	if (sdp->flags & DPTX_SDP_HORIZONTAL_INTERVAL)
		regmap_update_bits(dp->regmap, DPTX_SDP_HORIZONTAL_CTRL,
				   EN_HORIZONTAL_SDP << nr,
				   EN_HORIZONTAL_SDP << nr);

	return 0;
}

static void dw_dp_vsc_sdp_pack(const struct drm_dp_vsc_sdp *vsc,
			       struct dw_dp_sdp *sdp)
{
	sdp->header.HB0 = 0;
	sdp->header.HB1 = DP_SDP_VSC;
	sdp->header.HB2 = vsc->revision;
	sdp->header.HB3 = vsc->length;

	sdp->db[16] = (vsc->pixelformat & 0xf) << 4;
	sdp->db[16] |= vsc->colorimetry & 0xf;

	switch (vsc->bpc) {
	case 8:
		sdp->db[17] = 0x1;
		break;
	case 10:
		sdp->db[17] = 0x2;
		break;
	case 12:
		sdp->db[17] = 0x3;
		break;
	case 16:
		sdp->db[17] = 0x4;
		break;
	case 6:
	default:
		break;
	}

	if (vsc->dynamic_range == DP_DYNAMIC_RANGE_CTA)
		sdp->db[17] |= 0x80;

	sdp->db[18] = vsc->content_type & 0x7;

	sdp->flags |= DPTX_SDP_VERTICAL_INTERVAL;
}

static int dw_dp_send_vsc_sdp(struct dw_dp *dp)
{
	struct dw_dp_video *video = &dp->video;
	struct drm_dp_vsc_sdp vsc = {};
	struct dw_dp_sdp sdp = {};

	vsc.revision = 0x5;
	vsc.length = 0x13;

	switch (video->color_format) {
	case DRM_COLOR_FORMAT_YCRCB444:
		vsc.pixelformat = DP_PIXELFORMAT_YUV444;
		break;
	case DRM_COLOR_FORMAT_YCRCB420:
		vsc.pixelformat = DP_PIXELFORMAT_YUV420;
		break;
	case DRM_COLOR_FORMAT_YCRCB422:
		vsc.pixelformat = DP_PIXELFORMAT_YUV422;
		break;
	case DRM_COLOR_FORMAT_RGB444:
	default:
		vsc.pixelformat = DP_PIXELFORMAT_RGB;
		break;
	}

	if (video->color_format == DRM_COLOR_FORMAT_RGB444)
		vsc.colorimetry = DP_COLORIMETRY_DEFAULT;
	else
		vsc.colorimetry = DP_COLORIMETRY_BT709_YCC;

	vsc.bpc = video->bpc;
	vsc.dynamic_range = DP_DYNAMIC_RANGE_CTA;
	vsc.content_type = DP_CONTENT_TYPE_NOT_DEFINED;

	dw_dp_vsc_sdp_pack(&vsc, &sdp);

	return dw_dp_send_sdp(dp, &sdp);
}

static int dw_dp_video_set_pixel_mode(struct dw_dp *dp, u8 pixel_mode)
{
	switch (pixel_mode) {
	case DPTX_MP_SINGLE_PIXEL:
	case DPTX_MP_DUAL_PIXEL:
	case DPTX_MP_QUAD_PIXEL:
		break;
	default:
		return -EINVAL;
	}

	regmap_update_bits(dp->regmap, DPTX_VSAMPLE_CTRL, PIXEL_MODE_SELECT,
			   FIELD_PREP(PIXEL_MODE_SELECT, pixel_mode));

	return 0;
}

static int dw_dp_video_set_msa(struct dw_dp *dp, u8 color_format, u8 bpc,
			       u16 vstart, u16 hstart)
{
	struct dw_dp_link *link = &dp->link;
	u16 misc = 0;

	if (link->vsc_sdp_extension_for_colorimetry_supported)
		misc |= DP_MSA_MISC_COLOR_VSC_SDP;

	switch (color_format) {
	case DRM_COLOR_FORMAT_RGB444:
		misc |= DP_MSA_MISC_COLOR_RGB;
		break;
	case DRM_COLOR_FORMAT_YCRCB444:
		misc |= DP_MSA_MISC_COLOR_YCBCR_444_BT709;
		break;
	case DRM_COLOR_FORMAT_YCRCB422:
		misc |= DP_MSA_MISC_COLOR_YCBCR_422_BT709;
		break;
	case DRM_COLOR_FORMAT_YCRCB420:
		break;
	default:
		return -EINVAL;
	}

	switch (bpc) {
	case 6:
		misc |= DP_MSA_MISC_6_BPC;
		break;
	case 8:
		misc |= DP_MSA_MISC_8_BPC;
		break;
	case 10:
		misc |= DP_MSA_MISC_10_BPC;
		break;
	case 12:
		misc |= DP_MSA_MISC_12_BPC;
		break;
	case 16:
		misc |= DP_MSA_MISC_16_BPC;
		break;
	default:
		return -EINVAL;
	}

	regmap_write(dp->regmap, DPTX_VIDEO_MSA1,
		     FIELD_PREP(VSTART, vstart) | FIELD_PREP(HSTART, hstart));
	regmap_write(dp->regmap, DPTX_VIDEO_MSA2, FIELD_PREP(MISC0, misc));
	regmap_write(dp->regmap, DPTX_VIDEO_MSA3, FIELD_PREP(MISC1, misc >> 8));

	return 0;
}

static int dw_dp_video_enable(struct dw_dp *dp)
{
	struct dw_dp_video *video = &dp->video;
	struct dw_dp_link *link = &dp->link;
	struct drm_display_mode *mode = &video->mode;
	u8 color_format = video->color_format;
	u8 bpc = video->bpc;
	u8 pixel_mode = video->pixel_mode;
	u8 bpp = video->bpp, init_threshold, vic;
	u32 hactive, hblank, h_sync_width, h_front_porch;
	u32 vactive, vblank, v_sync_width, v_front_porch;
	u32 vstart = mode->vtotal - mode->vsync_start;
	u32 hstart = mode->htotal - mode->hsync_start;
	u32 peak_stream_bandwidth, link_bandwidth;
	u32 average_bytes_per_tu, average_bytes_per_tu_frac;
	u32 ts, hblank_interval;
	u32 value;
	int ret;

	ret = dw_dp_video_set_pixel_mode(dp, pixel_mode);
	if (ret)
		return ret;

	ret = dw_dp_video_set_msa(dp, color_format, bpc, vstart, hstart);
	if (ret)
		return ret;

	regmap_update_bits(dp->regmap, DPTX_VSAMPLE_CTRL, VIDEO_MAPPING,
			   FIELD_PREP(VIDEO_MAPPING, video->video_mapping));

	/* Configure DPTX_VINPUT_POLARITY_CTRL register */
	value = 0;
	if (mode->flags & DRM_MODE_FLAG_PHSYNC)
		value |= FIELD_PREP(HSYNC_IN_POLARITY, 1);
	if (mode->flags & DRM_MODE_FLAG_PVSYNC)
		value |= FIELD_PREP(VSYNC_IN_POLARITY, 1);
	regmap_write(dp->regmap, DPTX_VINPUT_POLARITY_CTRL, value);

	/* Configure DPTX_VIDEO_CONFIG1 register */
	hactive = mode->hdisplay;
	hblank = mode->htotal - mode->hdisplay;
	value = FIELD_PREP(HACTIVE, hactive) | FIELD_PREP(HBLANK, hblank);
	if (mode->flags & DRM_MODE_FLAG_INTERLACE)
		value |= FIELD_PREP(I_P, 1);
	vic = drm_match_cea_mode(mode);
	if (vic == 5 || vic == 6 || vic == 7 ||
	    vic == 10 || vic == 11 || vic == 20 ||
	    vic == 21 || vic == 22 || vic == 39 ||
	    vic == 25 || vic == 26 || vic == 40 ||
	    vic == 44 || vic == 45 || vic == 46 ||
	    vic == 50 || vic == 51 || vic == 54 ||
	    vic == 55 || vic == 58 || vic  == 59)
		value |= R_V_BLANK_IN_OSC;
	regmap_write(dp->regmap, DPTX_VIDEO_CONFIG1, value);

	/* Configure DPTX_VIDEO_CONFIG2 register */
	vblank = mode->vtotal - mode->vdisplay;
	vactive = mode->vdisplay;
	regmap_write(dp->regmap, DPTX_VIDEO_CONFIG2,
		     FIELD_PREP(VBLANK, vblank) | FIELD_PREP(VACTIVE, vactive));

	/* Configure DPTX_VIDEO_CONFIG3 register */
	h_sync_width = mode->hsync_end - mode->hsync_start;
	h_front_porch = mode->hsync_start - mode->hdisplay;
	regmap_write(dp->regmap, DPTX_VIDEO_CONFIG3,
		     FIELD_PREP(H_SYNC_WIDTH, h_sync_width) |
		     FIELD_PREP(H_FRONT_PORCH, h_front_porch));

	/* Configure DPTX_VIDEO_CONFIG4 register */
	v_sync_width = mode->vsync_end - mode->vsync_start;
	v_front_porch = mode->vsync_start - mode->vdisplay;
	regmap_write(dp->regmap, DPTX_VIDEO_CONFIG4,
		     FIELD_PREP(V_SYNC_WIDTH, v_sync_width) |
		     FIELD_PREP(V_FRONT_PORCH, v_front_porch));

	/* Configure DPTX_VIDEO_CONFIG5 register */
	peak_stream_bandwidth = mode->clock * bpp / 8;
	link_bandwidth = (link->rate / 1000) * link->lanes;
	ts = peak_stream_bandwidth * 64 / link_bandwidth;
	average_bytes_per_tu = ts / 1000;
	average_bytes_per_tu_frac = ts / 100 - average_bytes_per_tu * 10;
	if (pixel_mode == DPTX_MP_SINGLE_PIXEL) {
		if (average_bytes_per_tu < 6)
			init_threshold = 32;
		else if (hblank <= 80 &&
			 color_format != DRM_COLOR_FORMAT_YCRCB420)
			init_threshold = 12;
		else if (hblank <= 40 &&
			 color_format == DRM_COLOR_FORMAT_YCRCB420)
			init_threshold = 3;
		else
			init_threshold = 16;
	} else {
		u32 t1 = 0, t2 = 0, t3 = 0;

		switch (bpc) {
		case 6:
			t1 = (4 * 1000 / 9) * link->lanes;
			break;
		case 8:
			if (color_format == DRM_COLOR_FORMAT_YCRCB422) {
				t1 = (1000 / 2) * link->lanes;
			} else {
				if (pixel_mode == DPTX_MP_DUAL_PIXEL)
					t1 = (1000 / 3) * link->lanes;
				else
					t1 = (3000 / 16) * link->lanes;
			}
			break;
		case 10:
			if (color_format == DRM_COLOR_FORMAT_YCRCB422)
				t1 = (2000 / 5) * link->lanes;
			else
				t1 = (4000 / 15) * link->lanes;
			break;
		case 12:
			if (color_format == DRM_COLOR_FORMAT_YCRCB422) {
				if (pixel_mode == DPTX_MP_DUAL_PIXEL)
					t1 = (1000 / 6) * link->lanes;
				else
					t1 = (1000 / 3) * link->lanes;
			} else {
				t1 = (2000 / 9) * link->lanes;
			}
			break;
		case 16:
			if (color_format != DRM_COLOR_FORMAT_YCRCB422 &&
			    pixel_mode == DPTX_MP_DUAL_PIXEL)
				t1 = (1000 / 6) * link->lanes;
			else
				t1 = (1000 / 4) * link->lanes;
			break;
		default:
			return -EINVAL;
		}

		if (color_format == DRM_COLOR_FORMAT_YCRCB420)
			t2 = (link->rate / 4) * 1000 / (mode->clock / 2);
		else
			t2 = (link->rate / 4) * 1000 / mode->clock;

		if (average_bytes_per_tu_frac)
			t3 = average_bytes_per_tu + 1;
		else
			t3 = average_bytes_per_tu;
		init_threshold = t1 * t2 * t3 / (1000 * 1000);
		if (init_threshold <= 16 || average_bytes_per_tu < 10)
			init_threshold = 40;
	}

	regmap_write(dp->regmap, DPTX_VIDEO_CONFIG5,
		     FIELD_PREP(INIT_THRESHOLD_HI, init_threshold >> 6) |
		     FIELD_PREP(AVERAGE_BYTES_PER_TU_FRAC,
				average_bytes_per_tu_frac) |
		     FIELD_PREP(INIT_THRESHOLD, init_threshold) |
		     FIELD_PREP(AVERAGE_BYTES_PER_TU, average_bytes_per_tu));

	/* Configure DPTX_VIDEO_HBLANK_INTERVAL register */
	hblank_interval = hblank * (link->rate / 4) / mode->clock;
	regmap_write(dp->regmap, DPTX_VIDEO_HBLANK_INTERVAL,
		     FIELD_PREP(HBLANK_INTERVAL_EN, 1) |
		     FIELD_PREP(HBLANK_INTERVAL, hblank_interval));

	/* Video stream enable */
	regmap_update_bits(dp->regmap, DPTX_VSAMPLE_CTRL, VIDEO_STREAM_ENABLE,
			   FIELD_PREP(VIDEO_STREAM_ENABLE, 1));

	if (link->vsc_sdp_extension_for_colorimetry_supported)
		dw_dp_send_vsc_sdp(dp);

	return 0;
}

static bool dw_dp_detect(struct dw_dp *dp)
{
	u32 value;

	if (dm_gpio_is_valid(&dp->hpd_gpio))
		return dm_gpio_get_value(&dp->hpd_gpio);

	regmap_read(dp->regmap, DPTX_HPD_STATUS, &value);
	if (FIELD_GET(HPD_STATE, value) == SOURCE_STATE_PLUG) {
		regmap_write(dp->regmap, DPTX_HPD_STATUS, HPD_HOT_PLUG);
		return true;
	}

	return false;
}

static int dw_dp_connector_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct dw_dp *dp = dev_get_priv(conn->dev);
	int ret;

	conn_state->output_if |= dp->id ? VOP_OUTPUT_IF_DP1 : VOP_OUTPUT_IF_DP0;
	conn_state->output_mode = ROCKCHIP_OUT_MODE_AAAA;
	conn_state->color_space = V4L2_COLORSPACE_DEFAULT;

	clk_set_defaults(dp->dev);

	reset_assert(&dp->reset);
	udelay(20);
	reset_deassert(&dp->reset);

	conn_state->disp_info  = rockchip_get_disp_info(conn_state->type,
							dp->id);
	dw_dp_init(dp);
	ret = generic_phy_power_on(&dp->phy);

	return ret;
}

static int dw_dp_connector_get_edid(struct rockchip_connector *conn, struct display_state *state)
{
	int ret;
	struct connector_state *conn_state = &state->conn_state;
	struct dw_dp *dp = dev_get_priv(conn->dev);

	ret = drm_do_get_edid(&dp->aux.ddc, conn_state->edid);

	return ret;
}

static int dw_dp_get_output_fmts_index(u32 bus_format)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(possible_output_fmts); i++) {
		const struct dw_dp_output_format *fmt = &possible_output_fmts[i];

		if (fmt->bus_format == bus_format)
			break;
	}

	if (i == ARRAY_SIZE(possible_output_fmts))
		return 1;

	return i;
}

static int dw_dp_connector_prepare(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct dw_dp *dp = dev_get_priv(conn->dev);
	struct dw_dp_video *video = &dp->video;
	int bus_fmt;

	bus_fmt = dw_dp_get_output_fmts_index(conn_state->bus_format);
	video->video_mapping = possible_output_fmts[bus_fmt].video_mapping;
	video->color_format = possible_output_fmts[bus_fmt].color_format;
	video->bus_format = possible_output_fmts[bus_fmt].bus_format;
	video->bpc = possible_output_fmts[bus_fmt].bpc;
	video->bpp = possible_output_fmts[bus_fmt].bpp;

	return 0;
}

static int dw_dp_connector_enable(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct dw_dp *dp = dev_get_priv(conn->dev);
	struct dw_dp_video *video = &dp->video;
	int ret;

	memcpy(&video->mode, mode, sizeof(video->mode));
	video->pixel_mode = DPTX_MP_QUAD_PIXEL;

	if (dp->force_output) {
		ret = dw_dp_set_phy_default_config(dp);
		if (ret < 0)
			printf("failed to set phy_default config: %d\n", ret);
	} else {
		ret = dw_dp_link_enable(dp);
		if (ret < 0) {
			printf("failed to enable link: %d\n", ret);
			return ret;
		}
	}

	ret = dw_dp_video_enable(dp);
	if (ret < 0) {
		printf("failed to enable video: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_dp_connector_disable(struct rockchip_connector *conn, struct display_state *state)
{
	/* TODO */

	return 0;
}

static int dw_dp_connector_detect(struct rockchip_connector *conn, struct display_state *state)
{
	struct dw_dp *dp = dev_get_priv(conn->dev);
	int status, tries, ret;

	for (tries = 0; tries < 200; tries++) {
		status = dw_dp_detect(dp);
		if (status)
			break;
		mdelay(2);
	}

	if (state->force_output && !status)
		dp->force_output = true;

	if (!status && !dp->force_output)
		generic_phy_power_off(&dp->phy);

	if (status && !dp->force_output) {
		ret = dw_dp_link_probe(dp);
		if (ret)
			printf("failed to probe DP link: %d\n", ret);
	}

	return status;
}

static int dw_dp_mode_valid(struct dw_dp *dp, struct hdmi_edid_data *edid_data)
{
	struct dw_dp_link *link = &dp->link;
	struct drm_display_info *di = &edid_data->display_info;
	u32 min_bpp;
	int i;

	if (di->color_formats & DRM_COLOR_FORMAT_YCRCB420 &&
	    link->vsc_sdp_extension_for_colorimetry_supported)
		min_bpp = 12;
	else if (di->color_formats & DRM_COLOR_FORMAT_YCRCB422)
		min_bpp = 16;
	else if (di->color_formats & DRM_COLOR_FORMAT_RGB444)
		min_bpp = 18;
	else
		min_bpp = 24;

	for (i = 0; i < edid_data->modes; i++) {
		if (!dw_dp_bandwidth_ok(dp, &edid_data->mode_buf[i], min_bpp, link->lanes,
					link->rate))
			edid_data->mode_buf[i].invalid = true;
	}

	return 0;
}

static u32 dw_dp_get_output_bus_fmts(struct dw_dp *dp, struct hdmi_edid_data *edid_data)
{
	struct dw_dp_link *link = &dp->link;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(possible_output_fmts); i++) {
		const struct dw_dp_output_format *fmt = &possible_output_fmts[i];

		if (fmt->bpc > edid_data->display_info.bpc)
			continue;

		if (!(edid_data->display_info.color_formats & fmt->color_format))
			continue;

		if (fmt->color_format == DRM_COLOR_FORMAT_YCRCB420 &&
		    !link->vsc_sdp_extension_for_colorimetry_supported)
			continue;

		if (drm_mode_is_420(&edid_data->display_info, edid_data->preferred_mode) &&
		    fmt->color_format != DRM_COLOR_FORMAT_YCRCB420)
			continue;

		if (!dw_dp_bandwidth_ok(dp, edid_data->preferred_mode, fmt->bpp, link->lanes,
					link->rate))
			continue;

		break;
	}

	if (i == ARRAY_SIZE(possible_output_fmts))
		return 1;

	return i;
}

static int dw_dp_connector_get_timing(struct rockchip_connector *conn, struct display_state *state)
{
	int ret, i;
	struct connector_state *conn_state = &state->conn_state;
	struct dw_dp *dp = dev_get_priv(conn->dev);
	struct drm_display_mode *mode = &conn_state->mode;
	struct hdmi_edid_data edid_data;
	struct drm_display_mode *mode_buf;
	struct vop_rect rect;
	u32 bus_fmt;

	mode_buf = malloc(MODE_LEN * sizeof(struct drm_display_mode));
	if (!mode_buf)
		return -ENOMEM;

	memset(mode_buf, 0, MODE_LEN * sizeof(struct drm_display_mode));
	memset(&edid_data, 0, sizeof(struct hdmi_edid_data));
	edid_data.mode_buf = mode_buf;

	if (!dp->force_output) {
		ret = drm_do_get_edid(&dp->aux.ddc, conn_state->edid);
		if (!ret)
			ret = drm_add_edid_modes(&edid_data, conn_state->edid);

		if (ret < 0) {
			printf("failed to get edid\n");
			goto err;
		}

		drm_rk_filter_whitelist(&edid_data);
		if (state->conn_state.secondary) {
			rect.width = state->crtc_state.max_output.width / 2;
			rect.height = state->crtc_state.max_output.height / 2;
		} else {
			rect.width = state->crtc_state.max_output.width;
			rect.height = state->crtc_state.max_output.height;
		}

		drm_mode_max_resolution_filter(&edid_data, &rect);
		dw_dp_mode_valid(dp, &edid_data);

		if (!drm_mode_prune_invalid(&edid_data)) {
			printf("can't find valid hdmi mode\n");
			ret = -EINVAL;
			goto err;
		}

		for (i = 0; i < edid_data.modes; i++)
			edid_data.mode_buf[i].vrefresh =
				drm_mode_vrefresh(&edid_data.mode_buf[i]);

		drm_mode_sort(&edid_data);
		memcpy(mode, edid_data.preferred_mode, sizeof(struct drm_display_mode));
	}

	if (state->force_output)
		bus_fmt = dw_dp_get_output_fmts_index(state->force_bus_format);
	else
		bus_fmt = dw_dp_get_output_bus_fmts(dp, &edid_data);

	conn_state->bus_format = possible_output_fmts[bus_fmt].bus_format;

	switch (possible_output_fmts[bus_fmt].color_format) {
	case DRM_COLOR_FORMAT_YCRCB420:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_YUV420;
		break;
	case DRM_COLOR_FORMAT_YCRCB422:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_S888_DUMMY;
		break;
	case DRM_COLOR_FORMAT_RGB444:
	case DRM_COLOR_FORMAT_YCRCB444:
	default:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_AAAA;
		break;
	}

err:
	free(mode_buf);

	return 0;
}

static const struct rockchip_connector_funcs dw_dp_connector_funcs = {
	.init = dw_dp_connector_init,
	.get_edid = dw_dp_connector_get_edid,
	.prepare = dw_dp_connector_prepare,
	.enable = dw_dp_connector_enable,
	.disable = dw_dp_connector_disable,
	.detect = dw_dp_connector_detect,
	.get_timing = dw_dp_connector_get_timing,
};

static int dw_dp_ddc_init(struct dw_dp *dp)
{
	dp->aux.name = "dw-dp";
	dp->aux.dev = dp->dev;
	dp->aux.transfer = dw_dp_aux_transfer;
	dp->aux.ddc.ddc_xfer = drm_dp_i2c_xfer;

	return 0;
}

static int dw_dp_probe(struct udevice *dev)
{
	struct dw_dp *dp = dev_get_priv(dev);
	int ret;

	ret = regmap_init_mem(dev, &dp->regmap);
	if (ret)
		return ret;

	dp->id = of_alias_get_id(ofnode_to_np(dev->node), "dp");
	if (dp->id < 0)
		dp->id = 0;

	ret = reset_get_by_index(dev, 0, &dp->reset);
	if (ret) {
		dev_err(dev, "failed to get reset control: %d\n", ret);
		return ret;
	}

	dp->force_hpd = dev_read_bool(dev, "force-hpd");

	ret = gpio_request_by_name(dev, "hpd-gpios", 0, &dp->hpd_gpio,
				   GPIOD_IS_IN);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "failed to get hpd GPIO: %d\n", ret);
		return ret;
	}

	generic_phy_get_by_index(dev, 0, &dp->phy);

	dp->dev = dev;

	dw_dp_ddc_init(dp);

	rockchip_connector_bind(&dp->connector, dev, dp->id, &dw_dp_connector_funcs, NULL,
				DRM_MODE_CONNECTOR_DisplayPort);

	return 0;
}

static const struct udevice_id dw_dp_ids[] = {
	{
		.compatible = "rockchip,rk3588-dp",
	},
	{}
};

U_BOOT_DRIVER(dw_dp) = {
	.name = "dw_dp",
	.id = UCLASS_DISPLAY,
	.of_match = dw_dp_ids,
	.probe = dw_dp_probe,
	.priv_auto_alloc_size = sizeof(struct dw_dp),
};

