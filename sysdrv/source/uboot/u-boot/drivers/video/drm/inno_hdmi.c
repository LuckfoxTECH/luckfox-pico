// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 */
#include <common.h>
#include <clk.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/arch-rockchip/clock.h>
#include <dm/of_access.h>
#include <dm/device.h>
#include <dm/read.h>
#include <linux/hdmi.h>
#include <linux/media-bus-format.h>

#include "inno_hdmi.h"
#include "rockchip_connector.h"
#include "rockchip_crtc.h"
#include "rockchip_display.h"

struct inno_hdmi_i2c {
	u8			slave_reg;
	u8			ddc_addr;
	u8			segment_addr;
	bool			is_regaddr;
	bool			is_segment;

	unsigned int		scl_high_ns;
	unsigned int		scl_low_ns;
};

enum inno_hdmi_dev_type {
	RK3036_HDMI,
	RK3128_HDMI,
};

enum {
	CSC_ITU601_16_235_TO_RGB_0_255_8BIT,
	CSC_ITU601_0_255_TO_RGB_0_255_8BIT,
	CSC_ITU709_16_235_TO_RGB_0_255_8BIT,
	CSC_RGB_0_255_TO_ITU601_16_235_8BIT,
	CSC_RGB_0_255_TO_ITU709_16_235_8BIT,
	CSC_RGB_0_255_TO_RGB_16_235_8BIT,
};

static const char coeff_csc[][24] = {
	/*
	 * YUV2RGB:601 SD mode(Y[16:235], UV[16:240], RGB[0:255]):
	 *   R = 1.164*Y + 1.596*V - 204
	 *   G = 1.164*Y - 0.391*U - 0.813*V + 154
	 *   B = 1.164*Y + 2.018*U - 258
	 */
	{
		0x04, 0xa7, 0x00, 0x00, 0x06, 0x62, 0x02, 0xcc,
		0x04, 0xa7, 0x11, 0x90, 0x13, 0x40, 0x00, 0x9a,
		0x04, 0xa7, 0x08, 0x12, 0x00, 0x00, 0x03, 0x02
	},
	/*
	 * YUV2RGB:601 SD mode(YUV[0:255],RGB[0:255]):
	 *   R = Y + 1.402*V - 248
	 *   G = Y - 0.344*U - 0.714*V + 135
	 *   B = Y + 1.772*U - 227
	 */
	{
		0x04, 0x00, 0x00, 0x00, 0x05, 0x9b, 0x02, 0xf8,
		0x04, 0x00, 0x11, 0x60, 0x12, 0xdb, 0x00, 0x87,
		0x04, 0x00, 0x07, 0x16, 0x00, 0x00, 0x02, 0xe3
	},
	/*
	 * YUV2RGB:709 HD mode(Y[16:235],UV[16:240],RGB[0:255]):
	 *   R = 1.164*Y + 1.793*V - 248
	 *   G = 1.164*Y - 0.213*U - 0.534*V + 77
	 *   B = 1.164*Y + 2.115*U - 289
	 */
	{
		0x04, 0xa7, 0x00, 0x00, 0x07, 0x2c, 0x02, 0xf8,
		0x04, 0xa7, 0x10, 0xda, 0x12, 0x22, 0x00, 0x4d,
		0x04, 0xa7, 0x08, 0x74, 0x00, 0x00, 0x03, 0x21
	},

	/*
	 * RGB2YUV:601 SD mode:
	 *   Cb = -0.291G - 0.148R + 0.439B + 128
	 *   Y  = 0.504G  + 0.257R + 0.098B + 16
	 *   Cr = -0.368G + 0.439R - 0.071B + 128
	 */
	{
		0x11, 0x5f, 0x01, 0x82, 0x10, 0x23, 0x00, 0x80,
		0x02, 0x1c, 0x00, 0xa1, 0x00, 0x36, 0x00, 0x1e,
		0x11, 0x29, 0x10, 0x59, 0x01, 0x82, 0x00, 0x80
	},
	/*
	 * RGB2YUV:709 HD mode:
	 *   Cb = - 0.338G - 0.101R + 0.439B + 128
	 *   Y  = 0.614G   + 0.183R + 0.062B + 16
	 *   Cr = - 0.399G + 0.439R - 0.040B + 128
	 */
	{
		0x11, 0x98, 0x01, 0xc1, 0x10, 0x28, 0x00, 0x80,
		0x02, 0x74, 0x00, 0xbb, 0x00, 0x3f, 0x00, 0x10,
		0x11, 0x5a, 0x10, 0x67, 0x01, 0xc1, 0x00, 0x80
	},
	/*
	 * RGB[0:255]2RGB[16:235]:
	 *   R' = R x (235-16)/255 + 16;
	 *   G' = G x (235-16)/255 + 16;
	 *   B' = B x (235-16)/255 + 16;
	 */
	{
		0x00, 0x00, 0x03, 0x6F, 0x00, 0x00, 0x00, 0x10,
		0x03, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
		0x00, 0x00, 0x00, 0x00, 0x03, 0x6F, 0x00, 0x10
	},
};

struct hdmi_data_info {
	int vic;
	bool sink_is_hdmi;
	bool sink_has_audio;
	unsigned int enc_in_format;
	unsigned int enc_out_format;
	unsigned int colorimetry;
};

struct inno_hdmi_phy_config {
	unsigned long mpixelclock;
	u8 pre_emphasis;	/* pre-emphasis value */
	u8 vlev_ctr;		/* voltage level control */
};

struct inno_hdmi_plat_data {
	enum inno_hdmi_dev_type dev_type;
	struct inno_hdmi_phy_config *phy_config;
};

struct inno_hdmi {
	struct device *dev;
	struct drm_device *drm_dev;
	struct ddc_adapter adap;
	struct hdmi_edid_data edid_data;
	struct hdmi_data_info	hdmi_data;

	struct clk pclk;
	int vic;
	void *regs;
	void *grf;

	struct inno_hdmi_i2c *i2c;

	unsigned int tmds_rate;
	const struct inno_hdmi_plat_data *plat_data;

	unsigned int sample_rate;
	unsigned int audio_cts;
	unsigned int audio_n;
	bool audio_enable;

	struct drm_display_mode previous_mode;
};

static struct inno_hdmi_phy_config rk3036_hdmi_phy_config[] = {
	/* pixelclk pre-emp vlev */
	{ 74250000,  0x3f, 0xbb },
	{ 165000000, 0x6f, 0xbb },
	{ ~0UL,	     0x00, 0x00 }
};

static struct inno_hdmi_phy_config rk3128_hdmi_phy_config[] = {
	/* pixelclk pre-emp vlev */
	{ 74250000,  0x3f, 0xaa },
	{ 165000000, 0x5f, 0xaa },
	{ ~0UL,	     0x00, 0x00 }
};

static void hdmi_writeb(struct inno_hdmi *hdmi, u16 offset, u32 val)
{
	writel(val, hdmi->regs + (offset << 2));
}

static u32 hdmi_readb(struct inno_hdmi *hdmi, u16 offset)
{
	return readl(hdmi->regs + (offset << 2));
}

static void hdmi_modb(struct inno_hdmi *hdmi, u16 offset, u32 msk, u32 val)
{
	u32 temp = hdmi_readb(hdmi, offset) & ~msk;

	temp |= val & msk;
	hdmi_writeb(hdmi, offset, temp);
}

static void inno_hdmi_sys_power(struct inno_hdmi *hdmi, bool enable)
{
	if (enable)
		hdmi_modb(hdmi, HDMI_SYS_CTRL, m_POWER, v_PWR_ON);
	else
		hdmi_modb(hdmi, HDMI_SYS_CTRL, m_POWER, v_PWR_OFF);
}

static void inno_hdmi_set_pwr_mode(struct inno_hdmi *hdmi, int mode)
{
	const struct inno_hdmi_phy_config *phy_config =
						hdmi->plat_data->phy_config;

	switch (mode) {
	case NORMAL:
		inno_hdmi_sys_power(hdmi, false);
		for (; phy_config->mpixelclock != ~0UL; phy_config++)
			if (hdmi->tmds_rate <= phy_config->mpixelclock)
				break;
		if (!phy_config->mpixelclock)
			return;
		hdmi_writeb(hdmi, HDMI_PHY_PRE_EMPHASIS,
			    phy_config->pre_emphasis);
		hdmi_writeb(hdmi, HDMI_PHY_DRIVER, phy_config->vlev_ctr);

		hdmi_writeb(hdmi, HDMI_PHY_SYS_CTL, 0x15);
		hdmi_writeb(hdmi, HDMI_PHY_SYS_CTL, 0x14);
		hdmi_writeb(hdmi, HDMI_PHY_SYS_CTL, 0x10);

		hdmi_writeb(hdmi, HDMI_PHY_CHG_PWR, 0x0f);
		hdmi_writeb(hdmi, HDMI_PHY_SYNC, 0x00);
		hdmi_writeb(hdmi, HDMI_PHY_SYNC, 0x01);
		inno_hdmi_sys_power(hdmi, true);

		break;

	case LOWER_PWR:
		inno_hdmi_sys_power(hdmi, false);
		hdmi_writeb(hdmi, HDMI_PHY_DRIVER, 0x00);
		hdmi_writeb(hdmi, HDMI_PHY_PRE_EMPHASIS, 0x00);
		hdmi_writeb(hdmi, HDMI_PHY_CHG_PWR, 0x00);
		hdmi_writeb(hdmi, HDMI_PHY_SYS_CTL, 0x15);

		break;

	default:
		dev_err(hdmi->dev, "Unknown power mode %d\n", mode);
	}
}

static void inno_hdmi_i2c_init(struct inno_hdmi *hdmi)
{
	int ddc_bus_freq;

	ddc_bus_freq = (hdmi->tmds_rate >> 2) / HDMI_SCL_RATE;
	hdmi_writeb(hdmi, DDC_BUS_FREQ_L, ddc_bus_freq & 0xFF);
	hdmi_writeb(hdmi, DDC_BUS_FREQ_H, (ddc_bus_freq >> 8) & 0xFF);

	/* Clear the EDID interrupt flag and mute the interrupt */
	hdmi_writeb(hdmi, HDMI_INTERRUPT_MASK1, 0);
	hdmi_writeb(hdmi, HDMI_INTERRUPT_STATUS1, m_INT_EDID_READY);
}

static void inno_hdmi_reset(struct inno_hdmi *hdmi)
{
	u32 val;
	u32 msk;

	hdmi_modb(hdmi, HDMI_SYS_CTRL, m_RST_DIGITAL, v_NOT_RST_DIGITAL);
	udelay(100);

	hdmi_modb(hdmi, HDMI_SYS_CTRL, m_RST_ANALOG, v_NOT_RST_ANALOG);
	udelay(100);

	msk = m_REG_CLK_INV | m_REG_CLK_SOURCE | m_POWER | m_INT_POL;
	val = v_REG_CLK_INV | v_REG_CLK_SOURCE_SYS | v_PWR_ON | v_INT_POL_HIGH;

	hdmi_modb(hdmi, HDMI_SYS_CTRL, msk, val);

	inno_hdmi_set_pwr_mode(hdmi, NORMAL);
}

static int inno_hdmi_upload_frame(struct inno_hdmi *hdmi, int setup_rc,
				  union hdmi_infoframe *frame, u32 frame_index,
				  u32 mask, u32 disable, u32 enable)
{
	if (mask)
		hdmi_modb(hdmi, HDMI_PACKET_SEND_AUTO, mask, disable);

	hdmi_writeb(hdmi, HDMI_CONTROL_PACKET_BUF_INDEX, frame_index);

	if (setup_rc >= 0) {
		u8 packed_frame[HDMI_MAXIMUM_INFO_FRAME_SIZE];
		ssize_t rc, i;

		rc = hdmi_infoframe_pack(frame, packed_frame,
					 sizeof(packed_frame));
		if (rc < 0)
			return rc;

		for (i = 0; i < rc; i++)
			hdmi_writeb(hdmi, HDMI_CONTROL_PACKET_ADDR + i,
				    packed_frame[i]);

		if (mask)
			hdmi_modb(hdmi, HDMI_PACKET_SEND_AUTO, mask, enable);
	}

	return setup_rc;
}

static int inno_hdmi_config_video_vsi(struct inno_hdmi *hdmi,
				      struct drm_display_mode *mode)
{
	union hdmi_infoframe frame;
	int rc;

	rc = drm_hdmi_vendor_infoframe_from_display_mode(&frame.vendor.hdmi,
							 mode);

	return inno_hdmi_upload_frame(hdmi, rc, &frame, INFOFRAME_VSI,
		m_PACKET_VSI_EN, v_PACKET_VSI_EN(0), v_PACKET_VSI_EN(1));
}

static int inno_hdmi_config_video_avi(struct inno_hdmi *hdmi,
				      struct drm_display_mode *mode)
{
	union hdmi_infoframe frame;
	int rc;

	rc = drm_hdmi_avi_infoframe_from_display_mode(&frame.avi, mode, false);

	if (hdmi->hdmi_data.enc_out_format == HDMI_COLORSPACE_YUV444)
		frame.avi.colorspace = HDMI_COLORSPACE_YUV444;
	else if (hdmi->hdmi_data.enc_out_format == HDMI_COLORSPACE_YUV422)
		frame.avi.colorspace = HDMI_COLORSPACE_YUV422;
	else
		frame.avi.colorspace = HDMI_COLORSPACE_RGB;

	if (frame.avi.colorspace != HDMI_COLORSPACE_RGB)
		frame.avi.colorimetry = hdmi->hdmi_data.colorimetry;

	frame.avi.scan_mode = HDMI_SCAN_MODE_NONE;

	return inno_hdmi_upload_frame(hdmi, rc, &frame, INFOFRAME_AVI, 0, 0, 0);
}

static int inno_hdmi_config_video_csc(struct inno_hdmi *hdmi)
{
	struct hdmi_data_info *data = &hdmi->hdmi_data;
	int c0_c2_change = 0;
	int csc_enable = 0;
	int csc_mode = 0;
	int auto_csc = 0;
	int value;
	int i;

	/* Input video mode is SDR RGB24bit, data enable signal from external */
	hdmi_writeb(hdmi, HDMI_VIDEO_CONTRL1, v_DE_EXTERNAL |
		    v_VIDEO_INPUT_FORMAT(VIDEO_INPUT_SDR_RGB444));

	/* Input color hardcode to RGB, and output color hardcode to RGB888 */
	value = v_VIDEO_INPUT_BITS(VIDEO_INPUT_8BITS) |
		v_VIDEO_OUTPUT_COLOR(0) |
		v_VIDEO_INPUT_CSP(0);
	hdmi_writeb(hdmi, HDMI_VIDEO_CONTRL2, value);

	if (data->enc_in_format == data->enc_out_format) {
		if (data->enc_in_format == HDMI_COLORSPACE_RGB ||
		    data->enc_in_format >= HDMI_COLORSPACE_YUV444) {
			value = v_SOF_DISABLE | v_COLOR_DEPTH_NOT_INDICATED(1);
			hdmi_writeb(hdmi, HDMI_VIDEO_CONTRL3, value);

			hdmi_modb(hdmi, HDMI_VIDEO_CONTRL,
				  m_VIDEO_AUTO_CSC | m_VIDEO_C0_C2_SWAP,
				  v_VIDEO_AUTO_CSC(AUTO_CSC_DISABLE) |
				  v_VIDEO_C0_C2_SWAP(C0_C2_CHANGE_DISABLE));
			return 0;
		}
	}

	if (data->colorimetry == HDMI_COLORIMETRY_ITU_601) {
		if (data->enc_in_format == HDMI_COLORSPACE_RGB &&
		    data->enc_out_format == HDMI_COLORSPACE_YUV444) {
			csc_mode = CSC_RGB_0_255_TO_ITU601_16_235_8BIT;
			auto_csc = AUTO_CSC_DISABLE;
			c0_c2_change = C0_C2_CHANGE_DISABLE;
			csc_enable = v_CSC_ENABLE;
		} else if ((data->enc_in_format == HDMI_COLORSPACE_YUV444) &&
			   (data->enc_out_format == HDMI_COLORSPACE_RGB)) {
			csc_mode = CSC_ITU601_16_235_TO_RGB_0_255_8BIT;
			auto_csc = AUTO_CSC_ENABLE;
			c0_c2_change = C0_C2_CHANGE_DISABLE;
			csc_enable = v_CSC_DISABLE;
		}
	} else {
		if (data->enc_in_format == HDMI_COLORSPACE_RGB &&
		    data->enc_out_format == HDMI_COLORSPACE_YUV444) {
			csc_mode = CSC_RGB_0_255_TO_ITU709_16_235_8BIT;
			auto_csc = AUTO_CSC_DISABLE;
			c0_c2_change = C0_C2_CHANGE_DISABLE;
			csc_enable = v_CSC_ENABLE;
		} else if ((data->enc_in_format == HDMI_COLORSPACE_YUV444) &&
			   (data->enc_out_format == HDMI_COLORSPACE_RGB)) {
			csc_mode = CSC_ITU709_16_235_TO_RGB_0_255_8BIT;
			auto_csc = AUTO_CSC_ENABLE;
			c0_c2_change = C0_C2_CHANGE_DISABLE;
			csc_enable = v_CSC_DISABLE;
		}
	}

	for (i = 0; i < 24; i++)
		hdmi_writeb(hdmi, HDMI_VIDEO_CSC_COEF + i,
			    coeff_csc[csc_mode][i]);

	value = v_SOF_DISABLE | csc_enable | v_COLOR_DEPTH_NOT_INDICATED(1);
	hdmi_writeb(hdmi, HDMI_VIDEO_CONTRL3, value);
	hdmi_modb(hdmi, HDMI_VIDEO_CONTRL, m_VIDEO_AUTO_CSC |
		  m_VIDEO_C0_C2_SWAP, v_VIDEO_AUTO_CSC(auto_csc) |
		  v_VIDEO_C0_C2_SWAP(c0_c2_change));

	return 0;
}

static int inno_hdmi_config_video_timing(struct inno_hdmi *hdmi,
					 struct drm_display_mode *mode)
{
	int value;

	if (hdmi->plat_data->dev_type == RK3036_HDMI) {
		value = BIT(20) | BIT(21);
		value |= mode->flags & DRM_MODE_FLAG_PHSYNC ? BIT(4) : 0;
		value |= mode->flags & DRM_MODE_FLAG_PVSYNC ? BIT(5) : 0;
		writel(value, hdmi->grf + 0x148);
	}
	/* Set detail external video timing polarity and interlace mode */
	value = v_EXTERANL_VIDEO(1);
	value |= mode->flags & DRM_MODE_FLAG_PHSYNC ?
		 v_HSYNC_POLARITY(1) : v_HSYNC_POLARITY(0);
	value |= mode->flags & DRM_MODE_FLAG_PVSYNC ?
		 v_VSYNC_POLARITY(1) : v_VSYNC_POLARITY(0);
	value |= mode->flags & DRM_MODE_FLAG_INTERLACE ?
		 v_INETLACE(1) : v_INETLACE(0);
	hdmi_writeb(hdmi, HDMI_VIDEO_TIMING_CTL, value);

	/* Set detail external video timing */
	value = mode->htotal;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HTOTAL_L, value & 0xFF);
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HTOTAL_H, (value >> 8) & 0xFF);

	value = mode->htotal - mode->hdisplay;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HBLANK_L, value & 0xFF);
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HBLANK_H, (value >> 8) & 0xFF);

	value = mode->htotal - mode->hsync_start;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HDELAY_L, value & 0xFF);
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HDELAY_H, (value >> 8) & 0xFF);

	value = mode->hsync_end - mode->hsync_start;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HDURATION_L, value & 0xFF);
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_HDURATION_H, (value >> 8) & 0xFF);

	value = mode->vtotal;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_VTOTAL_L, value & 0xFF);
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_VTOTAL_H, (value >> 8) & 0xFF);

	value = mode->vtotal - mode->vdisplay;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_VBLANK, value & 0xFF);

	value = mode->vtotal - mode->vsync_start;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_VDELAY, value & 0xFF);

	value = mode->vsync_end - mode->vsync_start;
	hdmi_writeb(hdmi, HDMI_VIDEO_EXT_VDURATION, value & 0xFF);

	hdmi_writeb(hdmi, HDMI_PHY_PRE_DIV_RATIO, 0x1e);
	hdmi_writeb(hdmi, HDMI_PHY_FEEDBACK_DIV_RATIO_LOW, 0x2c);
	hdmi_writeb(hdmi, HDMI_PHY_FEEDBACK_DIV_RATIO_HIGH, 0x01);

	return 0;
}

static int inno_hdmi_setup(struct inno_hdmi *hdmi,
			   struct drm_display_mode *mode)
{
	hdmi->hdmi_data.vic = drm_match_cea_mode(mode);

	hdmi->hdmi_data.enc_in_format = HDMI_COLORSPACE_RGB;
	hdmi->hdmi_data.enc_out_format = HDMI_COLORSPACE_RGB;

	if (hdmi->hdmi_data.vic == 6 || hdmi->hdmi_data.vic == 7 ||
	    hdmi->hdmi_data.vic == 21 || hdmi->hdmi_data.vic == 22 ||
	    hdmi->hdmi_data.vic == 2 || hdmi->hdmi_data.vic == 3 ||
	    hdmi->hdmi_data.vic == 17 || hdmi->hdmi_data.vic == 18)
		hdmi->hdmi_data.colorimetry = HDMI_COLORIMETRY_ITU_601;
	else
		hdmi->hdmi_data.colorimetry = HDMI_COLORIMETRY_ITU_709;

	/* Mute video and audio output */
	hdmi_modb(hdmi, HDMI_AV_MUTE, m_AUDIO_MUTE | m_VIDEO_BLACK,
		  v_AUDIO_MUTE(1) | v_VIDEO_MUTE(1));

	/* Set HDMI Mode */
	hdmi_writeb(hdmi, HDMI_HDCP_CTRL,
		    v_HDMI_DVI(hdmi->hdmi_data.sink_is_hdmi));

	inno_hdmi_config_video_timing(hdmi, mode);

	inno_hdmi_config_video_csc(hdmi);

	if (hdmi->hdmi_data.sink_is_hdmi) {
		inno_hdmi_config_video_avi(hdmi, mode);
		inno_hdmi_config_video_vsi(hdmi, mode);
	}

	/*
	 * When IP controller have configured to an accurate video
	 * timing, then the TMDS clock source would be switched to
	 * DCLK_LCDC, so we need to init the TMDS rate to mode pixel
	 * clock rate, and reconfigure the DDC clock.
	 */
	hdmi->tmds_rate = mode->clock * 1000;
	inno_hdmi_i2c_init(hdmi);
	/* Unmute video and audio output */
	hdmi_modb(hdmi, HDMI_AV_MUTE, m_VIDEO_BLACK, v_VIDEO_MUTE(0));
	if (hdmi->audio_enable)
		hdmi_modb(hdmi, HDMI_AV_MUTE, m_AUDIO_MUTE, v_AUDIO_MUTE(0));

	return 0;
}

static int inno_hdmi_i2c_read(struct inno_hdmi *hdmi,
			      struct i2c_msg *msgs)
{
	struct inno_hdmi_i2c *i2c = hdmi->i2c;
	unsigned int length = msgs->len;
	unsigned char *buf = msgs->buf;
	int interrupt = 0, i = 20;

	while (i--) {
		mdelay(50);
		interrupt = 0;
		interrupt = hdmi_readb(hdmi, HDMI_INTERRUPT_STATUS1);

		if (interrupt & m_INT_EDID_READY)
			break;
	}

	if (!interrupt) {
		printf("[%s] i2c read reg[0x%02x] no interrupt\n",
		       __func__, i2c->slave_reg);
		return -EAGAIN;
	}

	/* Clear HDMI EDID interrupt flag */
	hdmi_writeb(hdmi, HDMI_INTERRUPT_STATUS1, m_INT_EDID_READY);

	while (length--)
		*buf++ = hdmi_readb(hdmi, HDMI_EDID_FIFO_ADDR);

	return 0;
}

static int inno_hdmi_i2c_write(struct inno_hdmi *hdmi,
			       struct i2c_msg *msgs)
{
	unsigned int length = msgs->len;

	hdmi->i2c->segment_addr = 0;
	hdmi->i2c->ddc_addr = 0;

	/*
	 * The DDC module only support read EDID message, so
	 * we assume that each word write to this i2c adapter
	 * should be the offset of EDID word address.
	 */
	if (length != 1 ||
	    (msgs->addr != DDC_ADDR && msgs->addr != DDC_SEGMENT_ADDR)) {
		printf("DDC word write to i2c adapter is not EDID address\n");
		return -EINVAL;
	}

	if (msgs->addr == DDC_SEGMENT_ADDR)
		hdmi->i2c->segment_addr = msgs->buf[0];
	if (msgs->addr == DDC_ADDR)
		hdmi->i2c->ddc_addr = msgs->buf[0];

	/* Set edid fifo first addr */
	hdmi_writeb(hdmi, HDMI_EDID_FIFO_OFFSET, 0x00);

	/* Set edid word address 0x00/0x80 */
	hdmi_writeb(hdmi, HDMI_EDID_WORD_ADDR, hdmi->i2c->ddc_addr);

	/* Set edid segment pointer */
	hdmi_writeb(hdmi, HDMI_EDID_SEGMENT_POINTER, hdmi->i2c->segment_addr);

	return 0;
}

static int inno_hdmi_i2c_xfer(struct ddc_adapter *adap,
			      struct i2c_msg *msgs, int num)
{
	struct inno_hdmi *hdmi = container_of(adap, struct inno_hdmi, adap);
	int i, ret = 0;

	/* Clear the EDID interrupt flag and unmute the interrupt */
	hdmi_writeb(hdmi, HDMI_INTERRUPT_MASK1, m_INT_EDID_READY);
	hdmi_writeb(hdmi, HDMI_INTERRUPT_STATUS1, m_INT_EDID_READY);

	for (i = 0; i < num; i++) {
		dev_dbg(hdmi->dev, "xfer: num: %d/%d, len: %d, flags: %#x\n",
			i + 1, num, msgs[i].len, msgs[i].flags);

		if (msgs[i].flags & I2C_M_RD)
			ret = inno_hdmi_i2c_read(hdmi, &msgs[i]);
		else
			ret = inno_hdmi_i2c_write(hdmi, &msgs[i]);

		if (ret < 0)
			break;
	}

	if (!ret)
		ret = num;

	/* Mute HDMI EDID interrupt */
	hdmi_writeb(hdmi, HDMI_INTERRUPT_MASK1, 0);

	return ret;
}

static int rockchip_inno_hdmi_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct inno_hdmi *hdmi;
	struct drm_display_mode *mode_buf;
	ofnode hdmi_node = conn->dev->node;
	int ret;

	hdmi = calloc(1, sizeof(struct inno_hdmi));
	if (!hdmi)
		return -ENOMEM;

	mode_buf = calloc(1, MODE_LEN * sizeof(struct drm_display_mode));
	if (!mode_buf)
		return -ENOMEM;

	hdmi->regs = dev_read_addr_ptr(conn->dev);

	hdmi->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (hdmi->grf <= 0) {
		printf("%s: Get syscon grf failed (ret=%p)\n",
		       __func__, hdmi->grf);
		return -ENXIO;
	}

	hdmi->i2c = malloc(sizeof(struct inno_hdmi_i2c));
	if (!hdmi->i2c)
		return -ENOMEM;

	hdmi->adap.ddc_xfer = inno_hdmi_i2c_xfer;

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

	conn_state->type = DRM_MODE_CONNECTOR_HDMIA;
	conn_state->output_mode = ROCKCHIP_OUT_MODE_AAAA;

	hdmi->plat_data = (struct inno_hdmi_plat_data *)dev_get_driver_data(conn->dev);
	hdmi->edid_data.mode_buf = mode_buf;
	hdmi->sample_rate = 48000;

	conn->data = hdmi;

	inno_hdmi_reset(hdmi);
	ret = clk_get_by_name(conn->dev, "pclk", &hdmi->pclk);
	if (ret < 0) {
		dev_err(hdmi->dev, "failed to get pclk: %d\n", ret);
		return ret;
	}
	hdmi->tmds_rate = clk_get_rate(&hdmi->pclk);
	inno_hdmi_i2c_init(hdmi);

	/* Unmute hotplug interrupt */
	hdmi_modb(hdmi, HDMI_STATUS, m_MASK_INT_HOTPLUG, v_MASK_INT_HOTPLUG(1));

	return 0;
}

static int rockchip_inno_hdmi_enable(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct inno_hdmi *hdmi = conn->data;

	if (!hdmi)
		return -EFAULT;

	/* Store the display mode for plugin/DKMS poweron events */
	memcpy(&hdmi->previous_mode, mode, sizeof(hdmi->previous_mode));

	inno_hdmi_setup(hdmi, mode);
	inno_hdmi_set_pwr_mode(hdmi, NORMAL);

	return 0;
}

static void rockchip_inno_hdmi_deinit(struct rockchip_connector *conn, struct display_state *state)
{
	struct inno_hdmi *hdmi = conn->data;

	if (hdmi->i2c)
		free(hdmi->i2c);
	if (hdmi)
		free(hdmi);
}

static int rockchip_inno_hdmi_prepare(struct rockchip_connector *conn, struct display_state *state)
{
	return 0;
}

static int rockchip_inno_hdmi_disable(struct rockchip_connector *conn, struct display_state *state)
{
	struct inno_hdmi *hdmi = conn->data;

	inno_hdmi_set_pwr_mode(hdmi, LOWER_PWR);
	return 0;
}

static int rockchip_inno_hdmi_detect(struct rockchip_connector *conn, struct display_state *state)
{
	struct inno_hdmi *hdmi = conn->data;

	return (hdmi_readb(hdmi, HDMI_STATUS) & m_HOTPLUG) ?
		connector_status_connected : connector_status_disconnected;
}

static int rockchip_inno_hdmi_get_timing(struct rockchip_connector *conn,
					 struct display_state *state)
{
	int  i, ret;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct inno_hdmi *hdmi = conn->data;
	struct edid *edid = (struct edid *)conn_state->edid;
	const u8 def_modes_vic[6] = {16, 4, 2, 17, 31, 19};

	if (!hdmi)
		return -EFAULT;

	ret = drm_do_get_edid(&hdmi->adap, conn_state->edid);
	if (!ret) {
		hdmi->hdmi_data.sink_is_hdmi =
			drm_detect_hdmi_monitor(edid);
		hdmi->hdmi_data.sink_has_audio = drm_detect_monitor_audio(edid);
		ret = drm_add_edid_modes(&hdmi->edid_data, conn_state->edid);
	}
	if (ret <= 0) {
		hdmi->hdmi_data.sink_is_hdmi = true;
		hdmi->hdmi_data.sink_has_audio = true;
		do_cea_modes(&hdmi->edid_data, def_modes_vic,
			     sizeof(def_modes_vic));
		hdmi->edid_data.preferred_mode = &hdmi->edid_data.mode_buf[0];
		printf("failed to get edid\n");
	}
	drm_rk_filter_whitelist(&hdmi->edid_data);

	if (!drm_mode_prune_invalid(&hdmi->edid_data)) {
		printf("can't find valid hdmi mode\n");
		return -EINVAL;
	}

	for (i = 0; i < hdmi->edid_data.modes; i++)
		hdmi->edid_data.mode_buf[i].vrefresh =
			drm_mode_vrefresh(&hdmi->edid_data.mode_buf[i]);

	drm_mode_sort(&hdmi->edid_data);

	*mode = *hdmi->edid_data.preferred_mode;
	hdmi->vic = drm_match_cea_mode(mode);

	printf("mode:%dx%d\n", mode->hdisplay, mode->vdisplay);

	conn_state->bus_format = MEDIA_BUS_FMT_RGB888_1X24;

	return 0;
}

const struct rockchip_connector_funcs rockchip_inno_hdmi_funcs = {
	.init = rockchip_inno_hdmi_init,
	.deinit = rockchip_inno_hdmi_deinit,
	.prepare = rockchip_inno_hdmi_prepare,
	.enable = rockchip_inno_hdmi_enable,
	.disable = rockchip_inno_hdmi_disable,
	.get_timing = rockchip_inno_hdmi_get_timing,
	.detect = rockchip_inno_hdmi_detect,
};

static int rockchip_inno_hdmi_probe(struct udevice *dev)
{
	int id;
	struct rockchip_connector *conn = dev_get_priv(dev);

	id = of_alias_get_id(ofnode_to_np(dev->node), "hdmi");
	if (id < 0)
		id = 0;

	rockchip_connector_bind(conn, dev, id, &rockchip_inno_hdmi_funcs, NULL,
				DRM_MODE_CONNECTOR_HDMIA);

	return 0;
}

static int rockchip_inno_hdmi_bind(struct udevice *dev)
{
	return 0;
}

static const struct inno_hdmi_plat_data rk3036_hdmi_drv_data = {
	.dev_type   = RK3036_HDMI,
	.phy_config = rk3036_hdmi_phy_config,
};

static const struct inno_hdmi_plat_data rk3128_hdmi_drv_data = {
	.dev_type   = RK3128_HDMI,
	.phy_config = rk3128_hdmi_phy_config,
};

static const struct udevice_id rockchip_inno_hdmi_ids[] = {
	{
	 .compatible = "rockchip,rk3036-inno-hdmi",
	 .data = (ulong)&rk3036_hdmi_drv_data,
	},
	{
	 .compatible = "rockchip,rk3128-inno-hdmi",
	 .data = (ulong)&rk3128_hdmi_drv_data,
	}, {}

};

U_BOOT_DRIVER(rockchip_inno_hdmi) = {
	.name = "rockchip_inno_hdmi",
	.id = UCLASS_DISPLAY,
	.of_match = rockchip_inno_hdmi_ids,
	.probe	= rockchip_inno_hdmi_probe,
	.bind	= rockchip_inno_hdmi_bind,
	.priv_auto_alloc_size = sizeof(struct rockchip_connector),
};
