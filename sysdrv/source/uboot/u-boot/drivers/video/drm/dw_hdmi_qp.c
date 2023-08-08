// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
 * Author: Algea Cao <algea.cao@rock-chips.com>
 */

#include <common.h>
#include <malloc.h>
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/arch/vendor.h>
#include <edid.h>
#include <dm/device.h>
#include <dm/of_access.h>
#include <dm/ofnode.h>
#include <dm/read.h>
#include <linux/hdmi.h>
#include <linux/media-bus-format.h>
#include <linux/dw_hdmi.h>
#include <asm/io.h>
#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "dw_hdmi_qp.h"
#include "rockchip_phy.h"

enum frl_mask {
	FRL_3GBPS_3LANE = 1,
	FRL_6GBPS_3LANE,
	FRL_6GBPS_4LANE,
	FRL_8GBPS_4LANE,
	FRL_10GBPS_4LANE,
	FRL_12GBPS_4LANE,
};

#define DDC_CI_ADDR		0x37
#define DDC_SEGMENT_ADDR	0x30

#define HDMI_EDID_LEN		512

/* DW-HDMI Controller >= 0x200a are at least compliant with SCDC version 1 */
#define SCDC_MIN_SOURCE_VERSION	0x1

#define HDMI14_MAX_TMDSCLK	340000000

struct hdmi_vmode {
	bool mdataenablepolarity;

	unsigned int mpixelclock;
	unsigned int mpixelrepetitioninput;
	unsigned int mpixelrepetitionoutput;
	unsigned int mtmdsclock;
};

struct hdmi_data_info {
	unsigned int enc_in_bus_format;
	unsigned int enc_out_bus_format;
	unsigned int enc_in_encoding;
	unsigned int enc_out_encoding;
	unsigned int quant_range;
	unsigned int pix_repet_factor;
	struct hdmi_vmode video_mode;
};

struct dw_hdmi_phy_data {
	enum dw_hdmi_phy_type type;
	const char *name;
	unsigned int gen;
	bool has_svsret;
	int (*configure)(struct dw_hdmi *hdmi,
			 const struct dw_hdmi_plat_data *pdata,
			 unsigned long mpixelclock);
};

struct dw_hdmi_i2c {
	u8			slave_reg;
	bool			is_regaddr;
	bool			is_segment;

	unsigned int		scl_high_ns;
	unsigned int		scl_low_ns;
};

struct dw_hdmi_qp {
	enum dw_hdmi_devtype dev_type;
	unsigned int version;
	struct hdmi_data_info hdmi_data;
	struct hdmi_edid_data edid_data;
	const struct dw_hdmi_plat_data *plat_data;
	struct ddc_adapter adap;

	int vic;
	int id;

	unsigned long bus_format;
	bool cable_plugin;
	bool sink_is_hdmi;
	bool sink_has_audio;
	void *regs;
	void *rk_hdmi;
	struct dw_hdmi_i2c *i2c;

	struct {
		const struct dw_hdmi_qp_phy_ops *ops;
		const char *name;
		void *data;
		bool enabled;
	} phy;

	struct drm_display_mode previous_mode;

	unsigned int sample_rate;
	unsigned int audio_cts;
	unsigned int audio_n;
	bool audio_enable;
	bool scramble_low_rates;

	void (*write)(struct dw_hdmi_qp *hdmi, u32 val, int offset);
	u8 (*read)(struct dw_hdmi_qp *hdmi, int offset);

	bool hdcp1x_enable;
	bool output_bus_format_rgb;
};

static inline void hdmi_writel(struct dw_hdmi_qp *hdmi, u32 val, int offset)
{
	writel(val, hdmi->regs + offset);
}

static inline u32 hdmi_readl(struct dw_hdmi_qp *hdmi, int offset)
{
	return readl(hdmi->regs + offset);
}

static void
hdmi_modb(struct dw_hdmi_qp *hdmi, u32 data, u32 mask, unsigned int reg)
{
	u32 val = hdmi_readl(hdmi, reg) & ~mask;

	val |= data & mask;
	hdmi_writel(hdmi, val, reg);
}

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

static bool drm_scdc_set_scrambling(struct ddc_adapter *adapter, bool enable)
{
	u8 config;
	int ret;

	ret = drm_scdc_readb(adapter, SCDC_TMDS_CONFIG, &config);
	if (ret < 0) {
		debug("Failed to read TMDS config: %d\n", ret);
		return false;
	}

	if (enable)
		config |= SCDC_SCRAMBLING_ENABLE;
	else
		config &= ~SCDC_SCRAMBLING_ENABLE;

	ret = drm_scdc_writeb(adapter, SCDC_TMDS_CONFIG, config);
	if (ret < 0) {
		debug("Failed to enable scrambling: %d\n", ret);
		return false;
	}

	return true;
}

static bool
drm_scdc_set_high_tmds_clock_ratio(struct ddc_adapter *adapter, bool set)
{
	u8 config;
	int ret;

	ret = drm_scdc_readb(adapter, SCDC_TMDS_CONFIG, &config);
	if (ret < 0) {
		debug("Failed to read TMDS config: %d\n", ret);
		return false;
	}

	if (set)
		config |= SCDC_TMDS_BIT_CLOCK_RATIO_BY_40;
	else
		config &= ~SCDC_TMDS_BIT_CLOCK_RATIO_BY_40;

	ret = drm_scdc_writeb(adapter, SCDC_TMDS_CONFIG, config);
	if (ret < 0) {
		debug("Failed to set TMDS clock ratio: %d\n", ret);
		return false;
	}

	/*
	 * The spec says that a source should wait minimum 1ms and maximum
	 * 100ms after writing the TMDS config for clock ratio. Lets allow a
	 * wait of up to 2ms here.
	 */
	udelay(2000);
	return true;
}

static void dw_hdmi_i2c_init(struct dw_hdmi_qp *hdmi)
{
	/* Software reset */
	hdmi_writel(hdmi, 0x01, I2CM_CONTROL0);

	hdmi_writel(hdmi, 0x085c085c, I2CM_FM_SCL_CONFIG0);

	hdmi_modb(hdmi, 0, I2CM_FM_EN, I2CM_INTERFACE_CONTROL0);

	/* Clear DONE and ERROR interrupts */
	hdmi_writel(hdmi, I2CM_OP_DONE_CLEAR | I2CM_NACK_RCVD_CLEAR,
		    MAINUNIT_1_INT_CLEAR);
}

static int dw_hdmi_i2c_read(struct dw_hdmi_qp *hdmi,
			    unsigned char *buf, unsigned int length)
{
	struct dw_hdmi_i2c *i2c = hdmi->i2c;
	int i = 20;
	u32 intr = 0;

	if (!i2c->is_regaddr) {
		printf("set read register address to 0\n");
		i2c->slave_reg = 0x00;
		i2c->is_regaddr = true;
	}

	while (length--) {
		hdmi_modb(hdmi, i2c->slave_reg++ << 12, I2CM_ADDR,
			  I2CM_INTERFACE_CONTROL0);

		hdmi_modb(hdmi, I2CM_FM_READ, I2CM_WR_MASK,
			  I2CM_INTERFACE_CONTROL0);

		while (i--) {
			udelay(1000);
			intr = hdmi_readl(hdmi, MAINUNIT_1_INT_STATUS) &
				(I2CM_OP_DONE_IRQ | I2CM_READ_REQUEST_IRQ |
				 I2CM_NACK_RCVD_IRQ);
			if (intr) {
				hdmi_writel(hdmi, intr, MAINUNIT_1_INT_CLEAR);
				break;
			}
		}

		if (!i) {
			printf("i2c read time out!\n");
			hdmi_writel(hdmi, 0x01, I2CM_CONTROL0);
			return -EAGAIN;
		}

		/* Check for error condition on the bus */
		if (intr & I2CM_NACK_RCVD_IRQ) {
			printf("i2c read err!\n");
			hdmi_writel(hdmi, 0x01, I2CM_CONTROL0);
			return -EIO;
		}

		*buf++ = hdmi_readl(hdmi, I2CM_INTERFACE_RDDATA_0_3) & 0xff;
		hdmi_modb(hdmi, 0, I2CM_WR_MASK, I2CM_INTERFACE_CONTROL0);
		i = 20;
	}
	i2c->is_segment = false;

	return 0;
}

static int dw_hdmi_i2c_write(struct dw_hdmi_qp *hdmi,
			     unsigned char *buf, unsigned int length)
{
	struct dw_hdmi_i2c *i2c = hdmi->i2c;
	int i = 20;
	u32 intr = 0;

	if (!i2c->is_regaddr) {
		/* Use the first write byte as register address */
		i2c->slave_reg = buf[0];
		length--;
		buf++;
		i2c->is_regaddr = true;
	}

	while (length--) {
		hdmi_writel(hdmi, *buf++, I2CM_INTERFACE_WRDATA_0_3);
		hdmi_modb(hdmi, i2c->slave_reg++ << 12, I2CM_ADDR,
			  I2CM_INTERFACE_CONTROL0);
		hdmi_modb(hdmi, I2CM_FM_WRITE, I2CM_WR_MASK,
			  I2CM_INTERFACE_CONTROL0);

		while (i--) {
			udelay(1000);
			intr = hdmi_readl(hdmi, MAINUNIT_1_INT_STATUS) &
				(I2CM_OP_DONE_IRQ | I2CM_READ_REQUEST_IRQ |
				 I2CM_NACK_RCVD_IRQ);
			if (intr) {
				hdmi_writel(hdmi, intr, MAINUNIT_1_INT_CLEAR);
				break;
			}
		}

		if (!i) {
			printf("i2c write time out!\n");
			hdmi_writel(hdmi, 0x01, I2CM_CONTROL0);
			return -EAGAIN;
		}

		/* Check for error condition on the bus */
		if (intr & I2CM_NACK_RCVD_IRQ) {
			printf("i2c write nack!\n");
			hdmi_writel(hdmi, 0x01, I2CM_CONTROL0);
			return -EIO;
		}
		hdmi_modb(hdmi, 0, I2CM_WR_MASK, I2CM_INTERFACE_CONTROL0);
		i = 20;
	}

	return 0;
}

static int dw_hdmi_i2c_xfer(struct ddc_adapter *adap,
			    struct i2c_msg *msgs, int num)
{
	struct dw_hdmi_qp *hdmi = container_of(adap, struct dw_hdmi_qp, adap);
	struct dw_hdmi_i2c *i2c = hdmi->i2c;
	u8 addr = msgs[0].addr;
	int i, ret = 0;

	debug("i2c xfer: num: %d, addr: %#x\n", num, addr);

	for (i = 0; i < num; i++) {
		if (msgs[i].len == 0) {
			printf("unsupported transfer %d/%d, no data\n",
			       i + 1, num);
			return -EOPNOTSUPP;
		}
	}

	/* Unmute DONE and ERROR interrupts */
	hdmi_modb(hdmi, I2CM_NACK_RCVD_MASK_N | I2CM_OP_DONE_MASK_N,
		  I2CM_NACK_RCVD_MASK_N | I2CM_OP_DONE_MASK_N,
		  MAINUNIT_1_INT_MASK_N);

	/* Set slave device address taken from the first I2C message */
	if (addr == DDC_SEGMENT_ADDR && msgs[0].len == 1)
		addr = DDC_ADDR;

	hdmi_modb(hdmi, addr << 5, I2CM_SLVADDR, I2CM_INTERFACE_CONTROL0);

	/* Set slave device register address on transfer */
	i2c->is_regaddr = false;

	/* Set segment pointer for I2C extended read mode operation */
	i2c->is_segment = false;

	for (i = 0; i < num; i++) {
		debug("xfer: num: %d/%d, len: %d, flags: %#x\n",
		      i + 1, num, msgs[i].len, msgs[i].flags);

		if (msgs[i].addr == DDC_SEGMENT_ADDR && msgs[i].len == 1) {
			i2c->is_segment = true;
			hdmi_modb(hdmi, DDC_SEGMENT_ADDR, I2CM_SEG_ADDR,
				  I2CM_INTERFACE_CONTROL1);
			hdmi_modb(hdmi, *msgs[i].buf, I2CM_SEG_PTR,
				  I2CM_INTERFACE_CONTROL1);
		} else {
			if (msgs[i].flags & I2C_M_RD)
				ret = dw_hdmi_i2c_read(hdmi, msgs[i].buf,
						       msgs[i].len);
			else
				ret = dw_hdmi_i2c_write(hdmi, msgs[i].buf,
							msgs[i].len);
		}
		if (ret < 0)
			break;
	}

	if (!ret)
		ret = num;

	/* Mute DONE and ERROR interrupts */
	hdmi_modb(hdmi, 0, I2CM_OP_DONE_MASK_N | I2CM_NACK_RCVD_MASK_N,
		  MAINUNIT_1_INT_MASK_N);

	return ret;
}

static int dw_hdmi_detect_phy(struct dw_hdmi_qp *hdmi)
{
	/* Vendor PHYs require support from the glue layer. */
	if (!hdmi->plat_data->qp_phy_ops || !hdmi->plat_data->phy_name) {
		dev_err(hdmi->dev,
			"Vendor HDMI PHY not supported by glue layer\n");
		return -ENODEV;
	}

	hdmi->phy.ops = hdmi->plat_data->qp_phy_ops;
	hdmi->phy.data = hdmi->plat_data->phy_data;
	hdmi->phy.name = hdmi->plat_data->phy_name;

	return 0;
}

static unsigned int
hdmi_get_tmdsclock(struct dw_hdmi_qp *hdmi, unsigned long mpixelclock)
{
	unsigned int tmdsclock = mpixelclock;
	unsigned int depth =
		hdmi_bus_fmt_color_depth(hdmi->hdmi_data.enc_out_bus_format);

	if (!hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_out_bus_format)) {
		switch (depth) {
		case 16:
			tmdsclock = mpixelclock * 2;
			break;
		case 12:
			tmdsclock = mpixelclock * 3 / 2;
			break;
		case 10:
			tmdsclock = mpixelclock * 5 / 4;
			break;
		default:
			break;
		}
	}

	return tmdsclock;
}

static void hdmi_infoframe_set_checksum(u8 *ptr, int size)
{
	u8 csum = 0;
	int i;

	ptr[3] = 0;
	/* compute checksum */
	for (i = 0; i < size; i++)
		csum += ptr[i];

	ptr[3] = 256 - csum;
}

static bool is_hdmi2_sink(struct dw_hdmi_qp *hdmi)
{
	return hdmi->edid_data.display_info.hdmi.scdc.supported ||
		hdmi->edid_data.display_info.color_formats & DRM_COLOR_FORMAT_YCRCB420;
}

static void hdmi_config_AVI(struct dw_hdmi_qp *hdmi, struct drm_display_mode *mode)
{
	struct hdmi_avi_infoframe frame;
	u32 val, i, j;
	u8 buff[17];
	bool is_hdmi2 = false;
	enum hdmi_quantization_range rgb_quant_range =
		hdmi->hdmi_data.quant_range;

	if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format) ||
	    hdmi->edid_data.display_info.hdmi.scdc.supported)
		is_hdmi2 = true;
	/* Initialise info frame from DRM mode */
	drm_hdmi_avi_infoframe_from_display_mode(&frame, mode, is_hdmi2);

	/*
	 * Ignore monitor selectable quantization, use quantization set
	 * by the user
	 */
	drm_hdmi_avi_infoframe_quant_range(&frame, mode, rgb_quant_range,
					   true);
	if (hdmi_bus_fmt_is_yuv444(hdmi->hdmi_data.enc_out_bus_format))
		frame.colorspace = HDMI_COLORSPACE_YUV444;
	else if (hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_out_bus_format))
		frame.colorspace = HDMI_COLORSPACE_YUV422;
	else if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format))
		frame.colorspace = HDMI_COLORSPACE_YUV420;
	else
		frame.colorspace = HDMI_COLORSPACE_RGB;

	/* Set up colorimetry and quant range */
	if (!hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_out_bus_format)) {
		switch (hdmi->hdmi_data.enc_out_encoding) {
		case V4L2_YCBCR_ENC_601:
			if (hdmi->hdmi_data.enc_in_encoding == V4L2_YCBCR_ENC_XV601)
				frame.colorimetry = HDMI_COLORIMETRY_EXTENDED;
			else
				frame.colorimetry = HDMI_COLORIMETRY_ITU_601;
			frame.extended_colorimetry =
					HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;
			break;
		case V4L2_YCBCR_ENC_709:
			if (hdmi->hdmi_data.enc_in_encoding == V4L2_YCBCR_ENC_XV709)
				frame.colorimetry = HDMI_COLORIMETRY_EXTENDED;
			else
				frame.colorimetry = HDMI_COLORIMETRY_ITU_709;
			frame.extended_colorimetry =
					HDMI_EXTENDED_COLORIMETRY_XV_YCC_709;
			break;
		case V4L2_YCBCR_ENC_BT2020:
			if (hdmi->hdmi_data.enc_in_encoding == V4L2_YCBCR_ENC_BT2020)
				frame.colorimetry = HDMI_COLORIMETRY_EXTENDED;
			else
				frame.colorimetry = HDMI_COLORIMETRY_ITU_709;
			frame.extended_colorimetry =
					HDMI_EXTENDED_COLORIMETRY_BT2020;
			break;
		default: /* Carries no data */
			frame.colorimetry = HDMI_COLORIMETRY_ITU_601;
			frame.extended_colorimetry =
					HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;
			break;
		}

		frame.ycc_quantization_range = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
	} else {
		if (hdmi->hdmi_data.enc_out_encoding == V4L2_YCBCR_ENC_BT2020) {
			frame.colorimetry = HDMI_COLORIMETRY_EXTENDED;
			frame.extended_colorimetry =
				HDMI_EXTENDED_COLORIMETRY_BT2020;
		} else {
			frame.colorimetry = HDMI_COLORIMETRY_NONE;
			frame.extended_colorimetry =
				HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;
		}

		if (is_hdmi2_sink(hdmi) &&
		    frame.quantization_range == HDMI_QUANTIZATION_RANGE_FULL)
			frame.ycc_quantization_range = HDMI_YCC_QUANTIZATION_RANGE_FULL;
		else
			frame.ycc_quantization_range = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
	}

	frame.scan_mode = HDMI_SCAN_MODE_NONE;

	hdmi_avi_infoframe_pack_only(&frame, buff, 17);

	/* mode which vic >= 128 must use avi version 3 */
	if (hdmi->vic >= 128) {
		frame.version = 3;
		buff[1] = frame.version;
		buff[4] &= 0x1f;
		buff[4] |= ((frame.colorspace & 0x7) << 5);
		buff[7] = hdmi->vic;
		hdmi_infoframe_set_checksum(buff, 17);
	}

	/*
	 * The Designware IP uses a different byte format from standard
	 * AVI info frames, though generally the bits are in the correct
	 * bytes.
	 */

	val = (frame.version << 8) | (frame.length << 16);
	hdmi_writel(hdmi, val, PKT_AVI_CONTENTS0);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i * 4 + j >= 14)
				break;
			if (!j)
				val = buff[i * 4 + j + 3];
			val |= buff[i * 4 + j + 3] << (8 * j);
		}

		hdmi_writel(hdmi, val, PKT_AVI_CONTENTS1 + i * 4);
	}

	hdmi_modb(hdmi, 0, PKTSCHED_AVI_FIELDRATE, PKTSCHED_PKT_CONFIG1);

	hdmi_modb(hdmi, PKTSCHED_AVI_TX_EN, PKTSCHED_AVI_TX_EN,
		  PKTSCHED_PKT_EN);
}

#define VSI_PKT_TYPE		0x81
#define VSI_PKT_VERSION		1
#define HDMI_FORUM_OUI		0xc45dd8
#define ALLM_MODE		BIT(1)
#define HDMI_FORUM_LEN		9

static void hdmi_config_vendor_specific_infoframe(struct dw_hdmi_qp *hdmi,
						  struct drm_display_mode *mode)
{
	struct hdmi_vendor_infoframe frame;
	struct dw_hdmi_link_config *link_cfg = NULL;
	u8 buffer[10];
	u32 val;
	ssize_t err;
	int i, reg;

	link_cfg = dw_hdmi_rockchip_get_link_cfg(hdmi->rk_hdmi);

	hdmi_modb(hdmi, 0, PKTSCHED_VSI_TX_EN, PKTSCHED_PKT_EN);

	for (i = 0; i <= 7; i++)
		hdmi_writel(hdmi, 0, PKT_VSI_CONTENTS0 + i * 4);

	if (link_cfg->allm_en) {
		buffer[0] = VSI_PKT_TYPE;
		buffer[1] = VSI_PKT_VERSION;
		buffer[2] = 5;
		buffer[4] = HDMI_FORUM_OUI & 0xff;
		buffer[5] = (HDMI_FORUM_OUI >> 8) & 0xff;
		buffer[6] = (HDMI_FORUM_OUI >> 16) & 0xff;
		buffer[7] = VSI_PKT_VERSION;
		buffer[8] = ALLM_MODE;

		hdmi_infoframe_set_checksum(buffer, HDMI_FORUM_LEN);

		err = 9;
	} else {
		err = drm_hdmi_vendor_infoframe_from_display_mode(&frame, mode);
		if (err < 0)
			/*
			 * Going into that statement does not means vendor infoframe
			 * fails. It just informed us that vendor infoframe is not
			 * needed for the selected mode. Only 4k or stereoscopic 3D
			 * mode requires vendor infoframe. So just simply return.
			 */
			return;

		err = hdmi_vendor_infoframe_pack(&frame, buffer, sizeof(buffer));
		if (err < 0) {
			dev_err(hdmi->dev, "Failed to pack vendor infoframe: %zd\n",
				err);
			return;
		}
	}

	/* vsi header */
	val = (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
	hdmi_writel(hdmi, val, PKT_VSI_CONTENTS0);

	reg = PKT_VSI_CONTENTS1;
	for (i = 3; i < err; i++) {
		if (i % 4 == 3)
			val = buffer[i];
		if (i % 4 == 0)
			val |= buffer[i] << 8;
		if (i % 4 == 1)
			val |= buffer[i] << 16;
		if (i % 4 == 2)
			val |= buffer[i] << 24;

		if ((i % 4 == 2) || (i == (err - 1))) {
			hdmi_writel(hdmi, val, reg);
			reg += 4;
		}
	}

	hdmi_writel(hdmi, 0, PKT_VSI_CONTENTS7);

	hdmi_modb(hdmi, 0, PKTSCHED_VSI_FIELDRATE, PKTSCHED_PKT_CONFIG1);
	hdmi_modb(hdmi, PKTSCHED_VSI_TX_EN, PKTSCHED_VSI_TX_EN,
		  PKTSCHED_PKT_EN);
}

static void hdmi_config_CVTEM(struct dw_hdmi_qp *hdmi,
			      struct dw_hdmi_link_config *link_cfg)
{
	u8 ds_type = 0;
	u8 sync = 1;
	u8 vfr = 1;
	u8 afr = 0;
	u8 new = 1;
	u8 end = 0;
	u8 data_set_length = 136;
	u8 hb1[6] = { 0x80, 0, 0, 0, 0, 0x40 };
	u8 *pps_body;
	u32 val, i, reg;
	struct drm_display_mode *mode = &hdmi->previous_mode;
	int hsync, hfront, hback;

	hdmi_modb(hdmi, 0, PKTSCHED_EMP_CVTEM_TX_EN, PKTSCHED_PKT_EN);

	if (!link_cfg->dsc_mode) {
		printf("don't use dsc mode\n");
		return;
	}

	pps_body = link_cfg->pps_payload;

	hsync = mode->hsync_end - mode->hsync_start;
	hback = mode->htotal - mode->hsync_end;
	hfront = mode->hsync_start - mode->hdisplay;

	for (i = 0; i < 6; i++) {
		val = i << 16 | hb1[i] << 8;
		hdmi_writel(hdmi, val, PKT0_EMP_CVTEM_CONTENTS0 + i * 0x20);
	}

	val = new << 7 | end << 6 | ds_type << 4 | afr << 3 |
	      vfr << 2 | sync << 1;
	hdmi_writel(hdmi, val, PKT0_EMP_CVTEM_CONTENTS1);

	val = data_set_length << 16 | pps_body[0] << 24;
	hdmi_writel(hdmi, val, PKT0_EMP_CVTEM_CONTENTS2);

	reg = PKT0_EMP_CVTEM_CONTENTS3;
	for (i = 1; i < 125; i++) {
		if (reg == PKT1_EMP_CVTEM_CONTENTS0 ||
		    reg == PKT2_EMP_CVTEM_CONTENTS0 ||
		    reg == PKT3_EMP_CVTEM_CONTENTS0 ||
		    reg == PKT4_EMP_CVTEM_CONTENTS0 ||
		    reg == PKT5_EMP_CVTEM_CONTENTS0) {
			reg += 4;
			i--;
			continue;
		}
		if (i % 4 == 1)
			val = pps_body[i];
		if (i % 4 == 2)
			val |= pps_body[i] << 8;
		if (i % 4 == 3)
			val |= pps_body[i] << 16;
		if (!(i % 4)) {
			val |= pps_body[i] << 24;
			hdmi_writel(hdmi, val, reg);
			reg += 4;
		}
	}

	val = (hfront & 0xff) << 24 | pps_body[127] << 16 |
	      pps_body[126] << 8 | pps_body[125];
	hdmi_writel(hdmi, val, PKT4_EMP_CVTEM_CONTENTS6);

	val = (hback & 0xff) << 24 | ((hsync >> 8) & 0xff) << 16 |
	      (hsync & 0xff) << 8 | ((hfront >> 8) & 0xff);
	hdmi_writel(hdmi, val, PKT4_EMP_CVTEM_CONTENTS7);

	val = link_cfg->hcactive << 8 | ((hback >> 8) & 0xff);
	hdmi_writel(hdmi, val, PKT5_EMP_CVTEM_CONTENTS1);

	for (i = PKT5_EMP_CVTEM_CONTENTS2; i <= PKT5_EMP_CVTEM_CONTENTS7; i += 4)
		hdmi_writel(hdmi, 0, i);

	hdmi_modb(hdmi, PKTSCHED_EMP_CVTEM_TX_EN, PKTSCHED_EMP_CVTEM_TX_EN,
		  PKTSCHED_PKT_EN);
}

static int hdmi_set_frl_mask(int frl_rate)
{
	switch (frl_rate) {
	case 48:
		return FRL_12GBPS_4LANE;
	case 40:
		return FRL_10GBPS_4LANE;
	case 32:
		return FRL_8GBPS_4LANE;
	case 24:
		return FRL_6GBPS_4LANE;
	case 18:
		return FRL_6GBPS_3LANE;
	case 9:
		return FRL_3GBPS_3LANE;
	}

	return 0;
}

static int hdmi_start_flt(struct dw_hdmi_qp *hdmi, u8 rate)
{
	u8 val;
	u32 value;
	u8 ffe_lv = 0;
	int i = 0;
	bool ltsp = false;

	hdmi_modb(hdmi, AVP_DATAPATH_VIDEO_SWDISABLE,
		  AVP_DATAPATH_VIDEO_SWDISABLE, GLOBAL_SWDISABLE);

	hdmi_writel(hdmi, AVP_DATAPATH_SWINIT_P, GLOBAL_SWRESET_REQUEST);

	/* clear flt flags */
	drm_scdc_writeb(&hdmi->adap, 0x10, 0xff);

	/* FLT_READY & FFE_LEVELS read */
	for (i = 0; i < 20; i++) {
		drm_scdc_readb(&hdmi->adap, SCDC_STATUS_FLAGS_0, &val);
		if (val & BIT(6))
			break;
		mdelay(20);
	}

	if (i == 20) {
		printf("sink flt isn't ready\n");
		return -EINVAL;
	}

	/* max ffe level 3 */
	val = 0 << 4 | hdmi_set_frl_mask(rate);
	drm_scdc_writeb(&hdmi->adap, 0x31, val);
	/* select FRL_RATE & FFE_LEVELS */
	hdmi_writel(hdmi, ffe_lv, FLT_CONFIG0);

	i = 500;
	while (i--) {
		mdelay(4);
		drm_scdc_readb(&hdmi->adap, 0x10, &val);

		if (!(val & 0x30))
			continue;

		if (val & BIT(5)) {
			u8 reg_val, ln0, ln1, ln2, ln3;

			drm_scdc_readb(&hdmi->adap, 0x41, &reg_val);
			ln0 = reg_val & 0xf;
			ln1 = (reg_val >> 4) & 0xf;

			drm_scdc_readb(&hdmi->adap, 0x42, &reg_val);
			ln2 = reg_val & 0xf;
			ln3 = (reg_val >> 4) & 0xf;

			if (!ln0 && !ln1 && !ln2 && !ln3) {
				printf("goto ltsp\n");
				ltsp = true;
				hdmi_writel(hdmi, 0, FLT_CONFIG1);
			} else if ((ln0 == 0xf) | (ln1 == 0xf) | (ln2 == 0xf) | (ln3 == 0xf)) {
				printf("goto lts4\n");
				break;
			} else if ((ln0 == 0xe) | (ln1 == 0xe) | (ln2 == 0xe) | (ln3 == 0xe)) {
				printf("goto ffe\n");
				break;
			} else {
				value = (ln3 << 16) | (ln2 << 12) | (ln1 << 8) | (ln0 << 4) | 0xf;
				hdmi_writel(hdmi, value, FLT_CONFIG1);
			}
		}

		drm_scdc_writeb(&hdmi->adap, 0x10, val);

		if ((val & BIT(4)) && ltsp) {
			hdmi_modb(hdmi, 0, AVP_DATAPATH_VIDEO_SWDISABLE, GLOBAL_SWDISABLE);
			printf("flt success\n");
			break;
		}
	}

	if (i < 0) {
		printf("flt time out\n");
		return -ETIMEDOUT;
	}

	return 0;
}

#define HDMI_MODE_FRL_MASK     BIT(30)

static void hdmi_set_op_mode(struct dw_hdmi_qp *hdmi,
			     struct dw_hdmi_link_config *link_cfg,
			     struct display_state *state,
			     struct rockchip_connector *conn)
{
	int frl_rate;
	int i, ret;

	if (!link_cfg->frl_mode) {
		printf("dw hdmi qp use tmds mode\n");
		hdmi_modb(hdmi, 0, OPMODE_FRL, LINK_CONFIG0);
		hdmi_modb(hdmi, 0, OPMODE_FRL_4LANES, LINK_CONFIG0);
		hdmi->phy.ops->init(conn, hdmi->rk_hdmi, state);
		hdmi->phy.enabled = true;
		return;
	}

	if (link_cfg->frl_lanes == 4)
		hdmi_modb(hdmi, OPMODE_FRL_4LANES, OPMODE_FRL_4LANES,
			  LINK_CONFIG0);
	else
		hdmi_modb(hdmi, 0, OPMODE_FRL_4LANES, LINK_CONFIG0);

	hdmi_modb(hdmi, 1, OPMODE_FRL, LINK_CONFIG0);

	frl_rate = link_cfg->frl_lanes * link_cfg->rate_per_lane;
	hdmi->phy.ops->init(conn, hdmi->rk_hdmi, state);
	hdmi->phy.enabled = true;

	mdelay(200);
	ret = hdmi_start_flt(hdmi, frl_rate);
	if (ret) {
		hdmi_writel(hdmi, 0, FLT_CONFIG0);
		drm_scdc_writeb(&hdmi->adap, 0x31, 0);
		hdmi_modb(hdmi, 0, AVP_DATAPATH_VIDEO_SWDISABLE, GLOBAL_SWDISABLE);
		return;
	}

	for (i = 0; i < 200; i++) {
		hdmi_modb(hdmi, PKTSCHED_NULL_TX_EN, PKTSCHED_NULL_TX_EN, PKTSCHED_PKT_EN);
		udelay(50);
		hdmi_modb(hdmi, 0, PKTSCHED_NULL_TX_EN, PKTSCHED_PKT_EN);
		udelay(50);
	}
}

static int dw_hdmi_setup(struct dw_hdmi_qp *hdmi,
			 struct rockchip_connector *conn,
			 struct drm_display_mode *mode,
			 struct display_state *state)
{
	int ret;
	void *data = hdmi->plat_data->phy_data;
	struct dw_hdmi_link_config *link_cfg;
	struct drm_hdmi_info *hdmi_info = &hdmi->edid_data.display_info.hdmi;
	struct hdmi_vmode *vmode = &hdmi->hdmi_data.video_mode;
	u8 bytes = 0;

	if (!hdmi->vic)
		printf("Non-CEA mode used in HDMI\n");
	else
		printf("CEA mode used vic=%d\n", hdmi->vic);

	vmode->mpixelclock = mode->clock * 1000;
	vmode->mtmdsclock = hdmi_get_tmdsclock(hdmi, vmode->mpixelclock);
	if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format))
		vmode->mtmdsclock /= 2;
	printf("mtmdsclock:%d\n", vmode->mtmdsclock);

	if (hdmi->plat_data->get_enc_out_encoding)
		hdmi->hdmi_data.enc_out_encoding =
			hdmi->plat_data->get_enc_out_encoding(data);
	else if (hdmi->vic == 6 || hdmi->vic == 7 ||
		 hdmi->vic == 21 || hdmi->vic == 22 ||
		 hdmi->vic == 2 || hdmi->vic == 3 ||
		 hdmi->vic == 17 || hdmi->vic == 18)
		hdmi->hdmi_data.enc_out_encoding = V4L2_YCBCR_ENC_601;
	else
		hdmi->hdmi_data.enc_out_encoding = V4L2_YCBCR_ENC_709;

	if (mode->flags & DRM_MODE_FLAG_DBLCLK) {
		hdmi->hdmi_data.video_mode.mpixelrepetitionoutput = 1;
		hdmi->hdmi_data.video_mode.mpixelrepetitioninput = 1;
	} else {
		hdmi->hdmi_data.video_mode.mpixelrepetitionoutput = 0;
		hdmi->hdmi_data.video_mode.mpixelrepetitioninput = 0;
	}

	/* TOFIX: Get input encoding from plat data or fallback to none */
	if (hdmi->plat_data->get_enc_in_encoding)
		hdmi->hdmi_data.enc_in_encoding =
			hdmi->plat_data->get_enc_in_encoding(data);
	else if (hdmi->plat_data->input_bus_encoding)
		hdmi->hdmi_data.enc_in_encoding =
			hdmi->plat_data->input_bus_encoding;
	else
		hdmi->hdmi_data.enc_in_encoding = V4L2_YCBCR_ENC_DEFAULT;

	if (hdmi->plat_data->get_quant_range)
		hdmi->hdmi_data.quant_range =
			hdmi->plat_data->get_quant_range(data);
	else
		hdmi->hdmi_data.quant_range = HDMI_QUANTIZATION_RANGE_DEFAULT;

	/*
	 * According to the dw-hdmi specification 6.4.2
	 * vp_pr_cd[3:0]:
	 * 0000b: No pixel repetition (pixel sent only once)
	 * 0001b: Pixel sent two times (pixel repeated once)
	 */
	hdmi->hdmi_data.pix_repet_factor =
		(mode->flags & DRM_MODE_FLAG_DBLCLK) ? 1 : 0;
	hdmi->hdmi_data.video_mode.mdataenablepolarity = true;

	/* HDMI Initialization Step B.2 */
	hdmi->phy.ops->set_pll(conn, hdmi->rk_hdmi, state);

	rk3588_set_grf_cfg(hdmi->rk_hdmi);
	link_cfg = dw_hdmi_rockchip_get_link_cfg(hdmi->rk_hdmi);

	/* not for DVI mode */
	if (hdmi->sink_is_hdmi) {
		printf("%s HDMI mode\n", __func__);
		hdmi_modb(hdmi, 0, OPMODE_DVI, LINK_CONFIG0);
		hdmi_modb(hdmi, HDCP2_BYPASS, HDCP2_BYPASS, HDCP2LOGIC_CONFIG0);
		hdmi_modb(hdmi, KEEPOUT_REKEY_ALWAYS, KEEPOUT_REKEY_CFG, FRAME_COMPOSER_CONFIG9);
		hdmi_writel(hdmi, 0, FLT_CONFIG0);
		if (hdmi_info->scdc.supported)
			drm_scdc_writeb(&hdmi->adap, 0x31, 0);
		if (!link_cfg->frl_mode) {
			if (vmode->mtmdsclock > HDMI14_MAX_TMDSCLK) {
				drm_scdc_readb(&hdmi->adap, SCDC_SINK_VERSION, &bytes);
				drm_scdc_writeb(&hdmi->adap, SCDC_SOURCE_VERSION,
						min_t(u8, bytes, SCDC_MIN_SOURCE_VERSION));
				drm_scdc_set_high_tmds_clock_ratio(&hdmi->adap, 1);
				drm_scdc_set_scrambling(&hdmi->adap, 1);
				hdmi_writel(hdmi, 1, SCRAMB_CONFIG0);
				mdelay(100);
			} else {
				if (hdmi_info->scdc.supported) {
					drm_scdc_set_high_tmds_clock_ratio(&hdmi->adap, 0);
					drm_scdc_set_scrambling(&hdmi->adap, 0);
				}
				hdmi_writel(hdmi, 0, SCRAMB_CONFIG0);
			}
		}
		/* HDMI Initialization Step F - Configure AVI InfoFrame */
		hdmi_config_AVI(hdmi, mode);
		hdmi_config_vendor_specific_infoframe(hdmi, mode);
		hdmi_config_CVTEM(hdmi, link_cfg);
		hdmi_set_op_mode(hdmi, link_cfg, state, conn);
		/* clear avmute */
		mdelay(50);
		hdmi_writel(hdmi, 2, PKTSCHED_PKT_CONTROL0);
		hdmi_modb(hdmi, PKTSCHED_GCP_TX_EN, PKTSCHED_GCP_TX_EN,
			  PKTSCHED_PKT_EN);
	} else {
		hdmi_modb(hdmi, OPMODE_DVI, OPMODE_DVI, LINK_CONFIG0);
		ret = hdmi->phy.ops->init(conn, hdmi->rk_hdmi, state);
		if (ret)
			return ret;
		hdmi->phy.enabled = true;
		printf("%s DVI mode\n", __func__);
	}

	return 0;
}

int dw_hdmi_detect_hotplug(struct dw_hdmi_qp *hdmi,
			   struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	int ret;

	ret = hdmi->phy.ops->read_hpd(hdmi->rk_hdmi);
	if (ret || state->force_output) {
		if (!hdmi->id)
			conn_state->output_if |= VOP_OUTPUT_IF_HDMI0;
		else
			conn_state->output_if |= VOP_OUTPUT_IF_HDMI1;
	}

	return ret;
}

int rockchip_dw_hdmi_qp_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	const struct dw_hdmi_plat_data *pdata =
		(const struct dw_hdmi_plat_data *)dev_get_driver_data(conn->dev);
	void *rk_hdmi = dev_get_priv(conn->dev);
	struct dw_hdmi_qp *hdmi;
	struct drm_display_mode *mode_buf;
	ofnode hdmi_node = conn->dev->node;
	struct device_node *ddc_node;

	hdmi = malloc(sizeof(struct dw_hdmi_qp));
	if (!hdmi)
		return -ENOMEM;

	memset(hdmi, 0, sizeof(struct dw_hdmi_qp));
	mode_buf = malloc(MODE_LEN * sizeof(struct drm_display_mode));
	if (!mode_buf)
		return -ENOMEM;

	hdmi->rk_hdmi = rk_hdmi;
	hdmi->id = of_alias_get_id(ofnode_to_np(hdmi_node), "hdmi");
	if (hdmi->id < 0)
		hdmi->id = 0;
	conn_state->disp_info = rockchip_get_disp_info(conn_state->type, hdmi->id);

	memset(mode_buf, 0, MODE_LEN * sizeof(struct drm_display_mode));

	hdmi->regs = dev_read_addr_ptr(conn->dev);

	ddc_node = of_parse_phandle(ofnode_to_np(hdmi_node), "ddc-i2c-bus", 0);
	if (ddc_node) {
		uclass_get_device_by_ofnode(UCLASS_I2C, np_to_ofnode(ddc_node),
					    &hdmi->adap.i2c_bus);
		if (hdmi->adap.i2c_bus)
			hdmi->adap.ops = i2c_get_ops(hdmi->adap.i2c_bus);
	}

	hdmi->i2c = malloc(sizeof(struct dw_hdmi_i2c));
	if (!hdmi->i2c)
		return -ENOMEM;
	hdmi->adap.ddc_xfer = dw_hdmi_i2c_xfer;

	/*
	 * Read high and low time from device tree. If not available use
	 * the default timing scl clock rate is about 99.6KHz.
	 */
	hdmi->i2c->scl_high_ns =
		ofnode_read_s32_default(hdmi_node,
					"ddc-i2c-scl-high-time-ns", 4708);
	hdmi->i2c->scl_low_ns =
		ofnode_read_s32_default(hdmi_node,
					"ddc-i2c-scl-low-time-ns", 4916);

	dw_hdmi_i2c_init(hdmi);
	conn_state->output_mode = ROCKCHIP_OUT_MODE_AAAA;

	hdmi->dev_type = pdata->dev_type;
	hdmi->plat_data = pdata;
	hdmi->edid_data.mode_buf = mode_buf;

	conn->data = hdmi;

	dw_hdmi_detect_phy(hdmi);
	hdmi_writel(hdmi, 0, MAINUNIT_0_INT_MASK_N);
	hdmi_writel(hdmi, 0, MAINUNIT_1_INT_MASK_N);
	hdmi_writel(hdmi, 428571429, TIMER_BASE_CONFIG0);

	dw_hdmi_qp_set_iomux(hdmi->rk_hdmi);

	return 0;
}

void rockchip_dw_hdmi_qp_deinit(struct rockchip_connector *conn, struct display_state *state)
{
	struct dw_hdmi_qp *hdmi = conn->data;

	if (hdmi->i2c)
		free(hdmi->i2c);
	if (hdmi->edid_data.mode_buf)
		free(hdmi->edid_data.mode_buf);
	if (hdmi)
		free(hdmi);
}

int rockchip_dw_hdmi_qp_prepare(struct rockchip_connector *conn, struct display_state *state)
{
	return 0;
}

static void dw_hdmi_disable(struct rockchip_connector *conn, struct dw_hdmi_qp *hdmi,
			    struct display_state *state)
{
	if (hdmi->phy.enabled) {
		hdmi->phy.ops->disable(conn, hdmi->rk_hdmi, state);
		hdmi->phy.enabled = false;
	}
}

int rockchip_dw_hdmi_qp_enable(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct dw_hdmi_qp *hdmi = conn->data;

	if (!hdmi)
		return -EFAULT;

	/* Store the display mode for plugin/DKMS poweron events */
	memcpy(&hdmi->previous_mode, mode, sizeof(hdmi->previous_mode));

	dw_hdmi_setup(hdmi, conn, mode, state);

	return 0;
}

int rockchip_dw_hdmi_qp_disable(struct rockchip_connector *conn, struct display_state *state)
{
	struct dw_hdmi_qp *hdmi = conn->data;

	dw_hdmi_disable(conn, hdmi, state);
	return 0;
}

int rockchip_dw_hdmi_qp_get_timing(struct rockchip_connector *conn, struct display_state *state)
{
	int ret, i;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct dw_hdmi_qp *hdmi = conn->data;
	struct edid *edid = (struct edid *)conn_state->edid;
	unsigned int bus_format;
	unsigned long enc_out_encoding;
	struct overscan *overscan = &conn_state->overscan;
	const u8 def_modes_vic[6] = {4, 16, 2, 17, 31, 19};

	if (!hdmi)
		return -EFAULT;

	ret = drm_do_get_edid(&hdmi->adap, conn_state->edid);
	if (!ret) {
		hdmi->sink_is_hdmi =
			drm_detect_hdmi_monitor(edid);
		hdmi->sink_has_audio = drm_detect_monitor_audio(edid);
		ret = drm_add_edid_modes(&hdmi->edid_data, conn_state->edid);
	}
	if (ret < 0) {
		hdmi->sink_is_hdmi = true;
		hdmi->sink_has_audio = true;
		do_cea_modes(&hdmi->edid_data, def_modes_vic,
			     sizeof(def_modes_vic));
		hdmi->edid_data.preferred_mode = &hdmi->edid_data.mode_buf[0];
		printf("failed to get edid\n");
	}
	drm_rk_filter_whitelist(&hdmi->edid_data);
	if (hdmi->phy.ops->mode_valid)
		hdmi->phy.ops->mode_valid(hdmi->rk_hdmi, state);
	drm_mode_max_resolution_filter(&hdmi->edid_data,
				       &state->crtc_state.max_output);
	if (!drm_mode_prune_invalid(&hdmi->edid_data)) {
		printf("can't find valid hdmi mode\n");
		return -EINVAL;
	}

	for (i = 0; i < hdmi->edid_data.modes; i++)
		hdmi->edid_data.mode_buf[i].vrefresh =
			drm_mode_vrefresh(&hdmi->edid_data.mode_buf[i]);

	drm_mode_sort(&hdmi->edid_data);
	dw_hdmi_qp_selete_output(&hdmi->edid_data, conn, &bus_format,
				 overscan, hdmi->dev_type,
				 hdmi->output_bus_format_rgb, hdmi->rk_hdmi,
				 state);

	*mode = *hdmi->edid_data.preferred_mode;
	hdmi->vic = drm_match_cea_mode(mode);

	printf("mode:%dx%d bus_format:0x%x\n", mode->hdisplay, mode->vdisplay, bus_format);
	conn_state->bus_format = bus_format;
	hdmi->hdmi_data.enc_in_bus_format = bus_format;
	hdmi->hdmi_data.enc_out_bus_format = bus_format;

	switch (bus_format) {
	case MEDIA_BUS_FMT_UYVY10_1X20:
		conn_state->bus_format = MEDIA_BUS_FMT_YUV10_1X30;
		hdmi->hdmi_data.enc_in_bus_format =
			MEDIA_BUS_FMT_YUV10_1X30;
		break;
	case MEDIA_BUS_FMT_UYVY8_1X16:
		conn_state->bus_format = MEDIA_BUS_FMT_YUV8_1X24;
		hdmi->hdmi_data.enc_in_bus_format =
			MEDIA_BUS_FMT_YUV8_1X24;
		break;
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
		conn_state->output_mode = ROCKCHIP_OUT_MODE_YUV420;
		break;
	}

	if (hdmi->vic == 6 || hdmi->vic == 7 || hdmi->vic == 21 ||
	    hdmi->vic == 22 || hdmi->vic == 2 || hdmi->vic == 3 ||
	    hdmi->vic == 17 || hdmi->vic == 18)
		enc_out_encoding = V4L2_YCBCR_ENC_601;
	else
		enc_out_encoding = V4L2_YCBCR_ENC_709;

	if (enc_out_encoding == V4L2_YCBCR_ENC_BT2020)
		conn_state->color_space = V4L2_COLORSPACE_BT2020;
	else if (bus_format == MEDIA_BUS_FMT_RGB888_1X24 ||
		 bus_format == MEDIA_BUS_FMT_RGB101010_1X30)
		conn_state->color_space = V4L2_COLORSPACE_DEFAULT;
	else if (enc_out_encoding == V4L2_YCBCR_ENC_709)
		conn_state->color_space = V4L2_COLORSPACE_REC709;
	else
		conn_state->color_space = V4L2_COLORSPACE_SMPTE170M;

	return 0;
}

int rockchip_dw_hdmi_qp_detect(struct rockchip_connector *conn, struct display_state *state)
{
	int ret;
	struct dw_hdmi_qp *hdmi = conn->data;

	if (!hdmi)
		return -EFAULT;

	ret = dw_hdmi_detect_hotplug(hdmi, state);

	return ret;
}

int rockchip_dw_hdmi_qp_get_edid(struct rockchip_connector *conn, struct display_state *state)
{
	int ret;
	struct connector_state *conn_state = &state->conn_state;
	struct dw_hdmi_qp *hdmi = conn->data;

	ret = drm_do_get_edid(&hdmi->adap, conn_state->edid);

	return ret;
}
