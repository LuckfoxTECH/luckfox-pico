// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2024 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <video_bridge.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/media-bus-format.h>
#include <linux/hdmi.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_panel.h"

#define SII902X_TPI_VIDEO_DATA			0x0

#define SII902X_TPI_PIXEL_REPETITION		0x8
#define SII902X_TPI_AVI_PIXEL_REP_BUS_24BIT     BIT(5)
#define SII902X_TPI_AVI_PIXEL_REP_RISING_EDGE   BIT(4)
#define SII902X_TPI_AVI_PIXEL_REP_4X		3
#define SII902X_TPI_AVI_PIXEL_REP_2X		1
#define SII902X_TPI_AVI_PIXEL_REP_NONE		0
#define SII902X_TPI_CLK_RATIO_HALF		(0 << 6)
#define SII902X_TPI_CLK_RATIO_1X		(1 << 6)
#define SII902X_TPI_CLK_RATIO_2X		(2 << 6)
#define SII902X_TPI_CLK_RATIO_4X		(3 << 6)

#define SII902X_TPI_AVI_IN_FORMAT		0x9
#define SII902X_TPI_AVI_INPUT_BITMODE_12BIT	BIT(7)
#define SII902X_TPI_AVI_INPUT_DITHER		BIT(6)
#define SII902X_TPI_AVI_INPUT_RANGE_LIMITED	(2 << 2)
#define SII902X_TPI_AVI_INPUT_RANGE_FULL	(1 << 2)
#define SII902X_TPI_AVI_INPUT_RANGE_AUTO	(0 << 2)
#define SII902X_TPI_AVI_INPUT_COLORSPACE_BLACK	(3 << 0)
#define SII902X_TPI_AVI_INPUT_COLORSPACE_YUV422	(2 << 0)
#define SII902X_TPI_AVI_INPUT_COLORSPACE_YUV444	(1 << 0)
#define SII902X_TPI_AVI_INPUT_COLORSPACE_RGB	(0 << 0)

#define SII902X_TPI_AVI_INFOFRAME		0x0c

#define SII902X_SYS_CTRL_DATA			0x1a
#define SII902X_SYS_CTRL_PWR_DWN		BIT(4)
#define SII902X_SYS_CTRL_AV_MUTE		BIT(3)
#define SII902X_SYS_CTRL_DDC_BUS_REQ		BIT(2)
#define SII902X_SYS_CTRL_DDC_BUS_GRTD		BIT(1)
#define SII902X_SYS_CTRL_OUTPUT_MODE		BIT(0)
#define SII902X_SYS_CTRL_OUTPUT_HDMI		1
#define SII902X_SYS_CTRL_OUTPUT_DVI		0

#define SII902X_REG_CHIPID(n)			(0x1b + (n))

#define SII902X_PWR_STATE_CTRL			0x1e
#define SII902X_AVI_POWER_STATE_MSK		GENMASK(1, 0)
#define SII902X_AVI_POWER_STATE_D(l)		((l) & SII902X_AVI_POWER_STATE_MSK)

/* Audio  */
#define SII902X_TPI_I2S_ENABLE_MAPPING_REG	0x1f
#define SII902X_TPI_I2S_CONFIG_FIFO0		(0 << 0)
#define SII902X_TPI_I2S_CONFIG_FIFO1		(1 << 0)
#define SII902X_TPI_I2S_CONFIG_FIFO2		(2 << 0)
#define SII902X_TPI_I2S_CONFIG_FIFO3		(3 << 0)
#define SII902X_TPI_I2S_LEFT_RIGHT_SWAP		(1 << 2)
#define SII902X_TPI_I2S_AUTO_DOWNSAMPLE		(1 << 3)
#define SII902X_TPI_I2S_SELECT_SD0		(0 << 4)
#define SII902X_TPI_I2S_SELECT_SD1		(1 << 4)
#define SII902X_TPI_I2S_SELECT_SD2		(2 << 4)
#define SII902X_TPI_I2S_SELECT_SD3		(3 << 4)
#define SII902X_TPI_I2S_FIFO_ENABLE		(1 << 7)

#define SII902X_TPI_I2S_INPUT_CONFIG_REG	0x20
#define SII902X_TPI_I2S_FIRST_BIT_SHIFT_YES	(0 << 0)
#define SII902X_TPI_I2S_FIRST_BIT_SHIFT_NO	(1 << 0)
#define SII902X_TPI_I2S_SD_DIRECTION_MSB_FIRST	(0 << 1)
#define SII902X_TPI_I2S_SD_DIRECTION_LSB_FIRST	(1 << 1)
#define SII902X_TPI_I2S_SD_JUSTIFY_LEFT		(0 << 2)
#define SII902X_TPI_I2S_SD_JUSTIFY_RIGHT	(1 << 2)
#define SII902X_TPI_I2S_WS_POLARITY_LOW		(0 << 3)
#define SII902X_TPI_I2S_WS_POLARITY_HIGH	(1 << 3)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_128	(0 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_256	(1 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_384	(2 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_512	(3 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_768	(4 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_1024	(5 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_1152	(6 << 4)
#define SII902X_TPI_I2S_MCLK_MULTIPLIER_192	(7 << 4)
#define SII902X_TPI_I2S_SCK_EDGE_FALLING	(0 << 7)
#define SII902X_TPI_I2S_SCK_EDGE_RISING		(1 << 7)

#define SII902X_TPI_I2S_STRM_HDR_BASE		0x21
#define SII902X_TPI_I2S_STRM_HDR_SIZE		5

#define SII902X_TPI_AUDIO_CONFIG_BYTE2_REG	0x26
#define SII902X_TPI_AUDIO_CODING_STREAM_HEADER	(0 << 0)
#define SII902X_TPI_AUDIO_CODING_PCM		(1 << 0)
#define SII902X_TPI_AUDIO_CODING_AC3		(2 << 0)
#define SII902X_TPI_AUDIO_CODING_MPEG1		(3 << 0)
#define SII902X_TPI_AUDIO_CODING_MP3		(4 << 0)
#define SII902X_TPI_AUDIO_CODING_MPEG2		(5 << 0)
#define SII902X_TPI_AUDIO_CODING_AAC		(6 << 0)
#define SII902X_TPI_AUDIO_CODING_DTS		(7 << 0)
#define SII902X_TPI_AUDIO_CODING_ATRAC		(8 << 0)
#define SII902X_TPI_AUDIO_MUTE_DISABLE		(0 << 4)
#define SII902X_TPI_AUDIO_MUTE_ENABLE		(1 << 4)
#define SII902X_TPI_AUDIO_LAYOUT_2_CHANNELS	(0 << 5)
#define SII902X_TPI_AUDIO_LAYOUT_8_CHANNELS	(1 << 5)
#define SII902X_TPI_AUDIO_INTERFACE_DISABLE	(0 << 6)
#define SII902X_TPI_AUDIO_INTERFACE_SPDIF	(1 << 6)
#define SII902X_TPI_AUDIO_INTERFACE_I2S		(2 << 6)

#define SII902X_TPI_AUDIO_CONFIG_BYTE3_REG	0x27
#define SII902X_TPI_AUDIO_FREQ_STREAM		(0 << 3)
#define SII902X_TPI_AUDIO_FREQ_32KHZ		(1 << 3)
#define SII902X_TPI_AUDIO_FREQ_44KHZ		(2 << 3)
#define SII902X_TPI_AUDIO_FREQ_48KHZ		(3 << 3)
#define SII902X_TPI_AUDIO_FREQ_88KHZ		(4 << 3)
#define SII902X_TPI_AUDIO_FREQ_96KHZ		(5 << 3)
#define SII902X_TPI_AUDIO_FREQ_176KHZ		(6 << 3)
#define SII902X_TPI_AUDIO_FREQ_192KHZ		(7 << 3)
#define SII902X_TPI_AUDIO_SAMPLE_SIZE_STREAM	(0 << 6)
#define SII902X_TPI_AUDIO_SAMPLE_SIZE_16	(1 << 6)
#define SII902X_TPI_AUDIO_SAMPLE_SIZE_20	(2 << 6)
#define SII902X_TPI_AUDIO_SAMPLE_SIZE_24	(3 << 6)

#define SII902X_TPI_AUDIO_CONFIG_BYTE4_REG	0x28

#define SII902X_INT_ENABLE			0x3c
#define SII902X_INT_STATUS			0x3d
#define SII902X_HOTPLUG_EVENT			BIT(0)
#define SII902X_PLUGGED_STATUS			BIT(2)

#define SII902X_TPI_SYNC_GEN_CTRL		0x60
#define SII902X_TPI_SYNC_POLAR_DETECT		0x61
#define SII902X_TPI_HBIT_TO_HSYNC		0x62
#define SII902X_EMBEDDED_SYNC_EXTRACTION_REG	0x63
#define SII902X_EMBEDDED_SYNC_EXTRACTION	BIT(6)

#define SII902X_REG_TPI_RQB			0xc7

/* Indirect internal register access */
#define SII902X_IND_SET_PAGE			0xbc
#define SII902X_IND_OFFSET			0xbd
#define SII902X_IND_VALUE			0xbe

#define SII902X_TPI_MISC_INFOFRAME_BASE		0xbf
#define SII902X_TPI_MISC_INFOFRAME_END		0xde
#define SII902X_TPI_MISC_INFOFRAME_SIZE	\
	(SII902X_TPI_MISC_INFOFRAME_END - SII902X_TPI_MISC_INFOFRAME_BASE)

#define SII902X_I2C_BUS_ACQUISITION_TIMEOUT_MS	500

#define SII902X_AUDIO_PORT_INDEX		3

struct sii902x {
	struct udevice *dev;
	struct udevice *power_supply;
	struct gpio_desc enable_gpio;
	struct gpio_desc reset_gpio;
	struct rockchip_bridge *bridge;
	struct drm_display_mode mode;
	struct ddc_adapter adap;
	struct hdmi_edid_data edid_data;
	int bus_format;
};

enum sii902x_bus_format {
	FORMAT_RGB_INPUT,
	FORMAT_YCBCR422_INPUT,
	FORMAT_YCBCR444_INPUT,
};

static inline struct sii902x *bridge_to_sii902x(struct rockchip_bridge *bridge)
{
	return container_of(&bridge, struct sii902x, bridge);
}

static int sii902x_reg_update_bits(struct sii902x *sii902x, u8 reg, u8 mask, u8 val)
{
	u8 status;

	status = dm_i2c_reg_read(sii902x->dev, reg);
	status &= ~mask;
	status |= val & mask;

	return dm_i2c_reg_write(sii902x->dev, reg, status);
}

static bool sii902x_bridge_detect(struct rockchip_bridge *bridge)
{
	struct udevice *dev = bridge->dev;
	u8 status;

	status = dm_i2c_reg_read(dev, SII902X_INT_STATUS);

	return (status & SII902X_PLUGGED_STATUS) ? true : false;
}

static bool sii902x_check_embedded_format(uint32_t bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
		return true;
	default:
		return false;
	}
}

static void sii902x_set_embedded_sync(struct sii902x *sii902x)
{
	struct udevice *dev = sii902x->dev;
	unsigned char data[8];
	struct videomode vm;

	if (!sii902x_check_embedded_format(sii902x->bus_format))
		return;

	switch (sii902x->bus_format) {
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		sii902x_reg_update_bits(sii902x, SII902X_TPI_SYNC_GEN_CTRL, 0x20, 0x20);
		break;
	default:
		break;
	}

	sii902x_reg_update_bits(sii902x,  SII902X_TPI_SYNC_GEN_CTRL, 0x80, 0x00);
	dm_i2c_reg_write(dev, SII902X_EMBEDDED_SYNC_EXTRACTION_REG, 0x00);
	sii902x_reg_update_bits(sii902x, SII902X_TPI_SYNC_GEN_CTRL, 0x80, 0x80);

	drm_display_mode_to_videomode(&sii902x->mode, &vm);
	data[0] = vm.hfront_porch & 0xff;
	data[1] = (vm.hfront_porch >> 8) & 0x03;
	if (sii902x->mode.flags & DRM_MODE_FLAG_INTERLACE) {
		data[2] = (sii902x->mode.vtotal >> 1) & 0xff;
		data[3] = ((sii902x->mode.vtotal >> 1) >> 8) & 0x1f;
	} else {
		data[2] = 0;
		data[3] = 0;
	}
	data[4] = vm.hsync_len & 0xff;
	data[5] = (vm.hsync_len >> 8) & 0x03;
	data[6] = vm.vfront_porch;
	data[7] = vm.vsync_len;
	dm_i2c_write(dev, SII902X_TPI_HBIT_TO_HSYNC, data, 8);

	sii902x_reg_update_bits(sii902x, SII902X_TPI_SYNC_GEN_CTRL, 0x80, 0x80);
	sii902x_reg_update_bits(sii902x, SII902X_EMBEDDED_SYNC_EXTRACTION_REG, 0x40, 0x40);

	sii902x_reg_update_bits(sii902x, SII902X_EMBEDDED_SYNC_EXTRACTION_REG,
				SII902X_EMBEDDED_SYNC_EXTRACTION,
				SII902X_EMBEDDED_SYNC_EXTRACTION);
}

static void sii902x_set_format(struct sii902x *sii902x)
{
	struct udevice *dev = sii902x->dev;
	u8 val;

	switch (sii902x->bus_format) {
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		val = SII902X_TPI_AVI_INPUT_COLORSPACE_YUV422;
		break;
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_VUY8_1X24:
		val = SII902X_TPI_AVI_INPUT_COLORSPACE_YUV444;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	default:
		val = SII902X_TPI_AVI_INPUT_COLORSPACE_RGB;
		break;
	}

	val |= SII902X_TPI_AVI_INPUT_RANGE_AUTO;
	val &= ~(SII902X_TPI_AVI_INPUT_DITHER |
		 SII902X_TPI_AVI_INPUT_BITMODE_12BIT);
	dm_i2c_reg_write(dev, SII902X_TPI_AVI_IN_FORMAT, val);

	sii902x_set_embedded_sync(sii902x);
}

static void sii902x_set_connector_display_info(struct sii902x *sii902x,
					       struct connector_state *conn_state)
{
	conn_state->bus_format = sii902x->bus_format;
	switch (conn_state->bus_format) {
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_BT656;
		conn_state->output_if = VOP_OUTPUT_IF_BT656;
		conn_state->color_range = DRM_COLOR_YCBCR_LIMITED_RANGE;
		conn_state->color_encoding = DRM_COLOR_YCBCR_BT601;
		break;
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_BT1120;
		conn_state->output_if = VOP_OUTPUT_IF_BT1120;
		conn_state->color_range = DRM_COLOR_YCBCR_LIMITED_RANGE;
		conn_state->color_encoding = DRM_COLOR_YCBCR_BT709;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	default:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_P888;
		conn_state->output_if = VOP_OUTPUT_IF_RGB;
		conn_state->color_range = DRM_COLOR_YCBCR_FULL_RANGE;
		conn_state->color_encoding = DRM_COLOR_YCBCR_BT709;
		break;
	}
}

static void sii902x_bridge_mode_set(struct rockchip_bridge *bridge,
				    const struct drm_display_mode *mode)
{
	struct sii902x *sii902x = dev_get_priv(bridge->dev);
	struct connector_state *conn_state = &bridge->state->conn_state;
	struct edid *edid = (struct edid *)conn_state->edid;
	struct udevice *dev = sii902x->dev;
	struct hdmi_avi_infoframe frame;
	u8 buf[HDMI_INFOFRAME_SIZE(AVI)];
	u8 output_mode;
	u16 pixel_clock_10kHz = mode->clock / 10;
	int ret, vrefresh;

	if (drm_detect_hdmi_monitor(edid))
		output_mode = SII902X_SYS_CTRL_OUTPUT_HDMI;
	else
		output_mode = SII902X_SYS_CTRL_OUTPUT_DVI;

	sii902x_reg_update_bits(sii902x, SII902X_SYS_CTRL_DATA,
				SII902X_SYS_CTRL_OUTPUT_MODE,
				output_mode);

	drm_mode_copy(&sii902x->mode, mode);
	vrefresh = drm_mode_vrefresh(mode) * 100;
	buf[0] = pixel_clock_10kHz & 0xff;
	buf[1] = pixel_clock_10kHz >> 8;
	buf[2] = vrefresh & 0xff;
	buf[3] = vrefresh >> 8;
	buf[4] = mode->crtc_htotal;
	buf[5] = mode->crtc_htotal >> 8;
	buf[6] = mode->crtc_vtotal;
	buf[7] = mode->crtc_vtotal >> 8;
	buf[8] = SII902X_TPI_CLK_RATIO_1X | SII902X_TPI_AVI_PIXEL_REP_NONE |
		 SII902X_TPI_AVI_PIXEL_REP_BUS_24BIT;
	switch (sii902x->bus_format) {
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		buf[8] |= SII902X_TPI_AVI_PIXEL_REP_RISING_EDGE;
		break;
	default:
		break;
	}

	buf[9] = SII902X_TPI_AVI_INPUT_RANGE_AUTO;
	switch (sii902x->bus_format) {
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
		buf[9] |= SII902X_TPI_AVI_INPUT_COLORSPACE_YUV422;
		break;
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_VUY8_1X24:
		buf[9] |= SII902X_TPI_AVI_INPUT_COLORSPACE_YUV444;
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	default:
		buf[9] |= SII902X_TPI_AVI_INPUT_COLORSPACE_RGB;
		break;
	}

	ret = dm_i2c_write(dev, SII902X_TPI_VIDEO_DATA, buf, 10);
	if (ret) {
		dev_err(dev, "failed to set video data\n");
		return;
	}

	ret = drm_hdmi_avi_infoframe_from_display_mode(&frame, &sii902x->mode, false);
	if (ret < 0) {
		dev_err(dev, "couldn't fill AVI infoframe\n");
		return;
	}

	ret = hdmi_avi_infoframe_pack(&frame, buf, sizeof(buf));
	if (ret < 0) {
		dev_err(dev, "failed to pack AVI infoframe: %d\n", ret);
		return;
	}

	/* Do not send the infoframe header, but keep the CRC field. */
	dm_i2c_write(dev, SII902X_TPI_AVI_INFOFRAME,
		     buf + HDMI_INFOFRAME_HEADER_SIZE - 1,
		     HDMI_AVI_INFOFRAME_SIZE + 1);
	sii902x_set_format(sii902x);
	sii902x_set_connector_display_info(sii902x, conn_state);
}

static void sii902x_bridge_enable(struct rockchip_bridge *bridge)
{
	struct sii902x *sii902x = dev_get_priv(bridge->dev);

	sii902x_reg_update_bits(sii902x, SII902X_PWR_STATE_CTRL,
				SII902X_AVI_POWER_STATE_MSK,
				SII902X_AVI_POWER_STATE_D(0));
	sii902x_reg_update_bits(sii902x, SII902X_SYS_CTRL_DATA, SII902X_SYS_CTRL_PWR_DWN, 0);
}

static void sii902x_bridge_disable(struct rockchip_bridge *bridge)
{
	struct sii902x *sii902x = dev_get_priv(bridge->dev);

	sii902x_reg_update_bits(sii902x, SII902X_SYS_CTRL_DATA,
				SII902X_SYS_CTRL_PWR_DWN,
				SII902X_SYS_CTRL_PWR_DWN);
}

static const struct rockchip_bridge_funcs sii902x_bridge_funcs = {
	.detect		= sii902x_bridge_detect,
	.mode_set	= sii902x_bridge_mode_set,
	.enable		= sii902x_bridge_enable,
	.post_disable	= sii902x_bridge_disable,
};

static void sii902x_reset(struct sii902x *sii902x)
{
	if (!dm_gpio_is_valid(&sii902x->reset_gpio))
		return;

	dm_gpio_set_value(&sii902x->reset_gpio, 1);

	/* The datasheet says treset-min = 100us. Make it 150us to be sure. */
	udelay(500);

	dm_gpio_set_value(&sii902x->reset_gpio, 0);
}

#define SII902X_READ_REG(v)	dm_i2c_reg_read(sii902x->dev, v)

/*
 * The purpose of sii902x_i2c_bypass_select is to enable the pass through
 * mode of the HDMI transmitter. Do not use regmap from within this function,
 * only use sii902x_*_unlocked functions to read/modify/write registers.
 * We are holding the parent adapter lock here, keep this in mind before
 * adding more i2c transactions.
 *
 * Also, since SII902X_SYS_CTRL_DATA is used with regmap_update_bits elsewhere
 * in this driver, we need to make sure that we only touch 0x1A[2:1] from
 * within sii902x_i2c_bypass_select and sii902x_i2c_bypass_deselect, and that
 * we leave the remaining bits as we have found them.
 */
static int sii902x_i2c_bypass_select(struct sii902x *sii902x)
{
	struct udevice *dev = sii902x->dev;
	u8 status;
	int ret;

	sii902x_reg_update_bits(sii902x, SII902X_SYS_CTRL_DATA,
				SII902X_SYS_CTRL_DDC_BUS_REQ,
				SII902X_SYS_CTRL_DDC_BUS_REQ);

	ret = readx_poll_timeout(SII902X_READ_REG, SII902X_SYS_CTRL_DATA, status,
				 status & SII902X_SYS_CTRL_DDC_BUS_GRTD,
				 SII902X_I2C_BUS_ACQUISITION_TIMEOUT_MS * 1000);
	if (ret) {
		dev_err(dev, "Failed to acquire the i2c bus\n");
		return -ETIMEDOUT;
	}

	dm_i2c_reg_write(dev, SII902X_SYS_CTRL_DATA, status);

	return 0;
}

/*
 * The purpose of sii902x_i2c_bypass_deselect is to disable the pass through
 * mode of the HDMI transmitter. Do not use regmap from within this function,
 * only use sii902x_*_unlocked functions to read/modify/write registers.
 * We are holding the parent adapter lock here, keep this in mind before
 * adding more i2c transactions.
 *
 * Also, since SII902X_SYS_CTRL_DATA is used with regmap_update_bits elsewhere
 * in this driver, we need to make sure that we only touch 0x1A[2:1] from
 * within sii902x_i2c_bypass_select and sii902x_i2c_bypass_deselect, and that
 * we leave the remaining bits as we have found them.
 */
static int sii902x_i2c_bypass_deselect(struct sii902x *sii902x)
{
	struct udevice *dev = sii902x->dev;
	unsigned int retries;
	u8 status;
	int ret;

	/*
	 * When the HDMI transmitter is in pass through mode, we need an
	 * (undocumented) additional delay between STOP and START conditions
	 * to guarantee the bus won't get stuck.
	 */
	udelay(30);

	/*
	 * Sometimes the I2C bus can stall after failure to use the
	 * EDID channel. Retry a few times to see if things clear
	 * up, else continue anyway.
	 */
	retries = 5;
	do {
		status = dm_i2c_reg_read(dev, SII902X_SYS_CTRL_DATA);
		retries--;
	} while (status < 0 && retries);
	if (status < 0) {
		dev_err(dev, "failed to read status (%d)\n", status);
		return status;
	}

	ret = sii902x_reg_update_bits(sii902x, SII902X_SYS_CTRL_DATA,
				      SII902X_SYS_CTRL_DDC_BUS_REQ |
				      SII902X_SYS_CTRL_DDC_BUS_GRTD, 0);
	if (ret)
		return ret;

	ret = readx_poll_timeout(SII902X_READ_REG, SII902X_SYS_CTRL_DATA, status,
				 !(status & (SII902X_SYS_CTRL_DDC_BUS_REQ |
					     SII902X_SYS_CTRL_DDC_BUS_GRTD)),
				 SII902X_I2C_BUS_ACQUISITION_TIMEOUT_MS * 1000);
	if (ret) {
		dev_err(dev, "failed to release the i2c bus\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static int sii902x_i2c_xfer(struct ddc_adapter *adap, struct i2c_msg *msgs, int num)
{
	struct sii902x *sii902x = container_of(adap, struct sii902x, adap);
	struct udevice *dev = sii902x->dev;
	u8 addr = msgs[0].addr;
	int i;
	int ret;

	ret = sii902x_i2c_bypass_select(sii902x);
	if (ret)
		return -EINVAL;

	debug("i2c xfer: num: %d, addr: %#x\n", num, addr);

	for (i = 0; i < num; i++) {
		debug("xfer: num: %d/%d, len: %d, flags: %#x\n",
		      i + 1, num, msgs[i].len, msgs[i].flags);

		ret = dm_i2c_xfer(dev, msgs, num);
		if (ret < 0)
			break;
	}

	ret = sii902x_i2c_bypass_deselect(sii902x);
	if (ret)
		return -EINVAL;

	if (!ret)
		ret = num;

	return ret;
}

static int sii902x_init(struct sii902x *sii902x)
{
	struct udevice *dev = sii902x->dev;
	u8 chipid[4];
	u8 status;
	int ret;

	sii902x_reset(sii902x);

	ret = dm_i2c_reg_write(dev, SII902X_REG_TPI_RQB, 0x0);
	if (ret) {
		dev_err(dev, "enable TPI mode failed %d\n", ret);
		return ret;
	}

	ret = dm_i2c_read(dev, SII902X_REG_CHIPID(0), chipid, 4);
	if (ret) {
		dev_err(dev, "regmap_read failed %d\n", ret);
		return ret;
	}

	if (chipid[0] != 0xb0) {
		dev_err(dev, "Invalid chipid: %02x (expecting 0xb0)\n",
			chipid[0]);
		return -EINVAL;
	}

	/* Clear all pending interrupts */
	status = dm_i2c_reg_read(dev, SII902X_INT_STATUS);
	dm_i2c_reg_write(dev, SII902X_INT_STATUS, status);

	sii902x->adap.i2c_bus = dev->parent;
	sii902x->adap.ddc_xfer = sii902x_i2c_xfer;

	sii902x->edid_data.mode_buf = malloc(MODE_LEN * sizeof(struct drm_display_mode));
	if (!sii902x->edid_data.mode_buf) {
		dev_err(dev, "failed to malloc mode buffer for edid\n");
		return -ENOMEM;
	}

	return 0;
}

static int sii902x_probe(struct udevice *dev)
{
	struct sii902x *sii902x = dev_get_priv(dev);
	struct rockchip_bridge *bridge = (struct rockchip_bridge *)dev_get_driver_data(dev);
	int val;
	int ret;

	sii902x->dev = dev;
	sii902x->bridge = bridge;
	bridge->dev = dev;

	ret = gpio_request_by_name(dev, "reset-gpio", 0,
				   &sii902x->reset_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "Cannot get reset GPIO: %d\n", ret);
		return ret;
	}

	ret = gpio_request_by_name(dev, "enable-gpio", 0,
				   &sii902x->enable_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT) {
		dev_err(dev, "Cannot get enable GPIO: %d\n", ret);
		return ret;
	} else {
		dm_gpio_set_value(&sii902x->enable_gpio, 1);
		udelay(2000);
	}

	val = ofnode_read_u32_default(dev->node, "bus-format", -1);
	if (val < 0) {
		sii902x->bus_format = MEDIA_BUS_FMT_RGB888_1X24;
	} else {
		switch (val) {
		case FORMAT_RGB_INPUT:
			sii902x->bus_format = MEDIA_BUS_FMT_RGB888_1X24;
			break;
		case FORMAT_YCBCR422_INPUT:
			sii902x->bus_format = MEDIA_BUS_FMT_YUYV8_1X16;
			break;
		case FORMAT_YCBCR444_INPUT:
			sii902x->bus_format = MEDIA_BUS_FMT_YUV8_1X24;
			break;
		default:
			sii902x->bus_format = val;
			break;
		}
	}

	ret = sii902x_init(sii902x);
	if (ret < 0) {
		dev_err(dev, "Failed to init sii902x %d\n", ret);
		return ret;
	}

	printf("sii902x init successfully\n");

	return 0;
}

static struct rockchip_bridge sii902x_driver_data = {
	.funcs = &sii902x_bridge_funcs,
};

static const struct udevice_id sii902x_of_match[] = {
	{
		.compatible = "sil,sii9022",
		.data = (ulong)&sii902x_driver_data,
	},
	{}
};

static int sii902x_get_timing(struct udevice *dev)
{
	struct sii902x *sii902x = dev_get_priv(dev);
	struct rockchip_bridge *bridge = sii902x->bridge;
	struct display_state *state = bridge->state;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	int ret;

	ret = drm_do_get_edid(&sii902x->adap, conn_state->edid);
	if (!ret) {
		ret = drm_add_edid_modes(&sii902x->edid_data, conn_state->edid);
	}
	if (ret < 0) {
		dev_err(dev, "Failed to get edid %d\n", ret);
		return ret;
	}

	drm_mode_max_resolution_filter(&sii902x->edid_data, &state->crtc_state.max_output);
	if (!drm_mode_prune_invalid(&sii902x->edid_data)) {
		dev_err(dev, "Failed to find valid display mode\n");
		return -EINVAL;
	}

	drm_mode_sort(&sii902x->edid_data);
	*mode = *sii902x->edid_data.preferred_mode;

	return 0;
}

struct video_bridge_ops sii902x_ops = {
	.get_timing = sii902x_get_timing,
};

U_BOOT_DRIVER(sii902x) = {
	.name = "sii902x",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = sii902x_of_match,
	.probe = sii902x_probe,
	.priv_auto_alloc_size = sizeof(struct sii902x),
	.ops = &sii902x_ops,
};
