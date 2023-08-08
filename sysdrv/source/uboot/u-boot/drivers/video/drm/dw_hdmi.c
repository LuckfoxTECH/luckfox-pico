/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <syscon.h>
#include <asm/gpio.h>
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
#include "dw_hdmi.h"
#include "rockchip_phy.h"

#define HDCP_PRIVATE_KEY_SIZE   280
#define HDCP_KEY_SHA_SIZE       20
#define HDMI_HDCP1X_ID		5
/*
 * Unless otherwise noted, entries in this table are 100% optimization.
 * Values can be obtained from hdmi_compute_n() but that function is
 * slow so we pre-compute values we expect to see.
 *
 * All 32k and 48k values are expected to be the same (due to the way
 * the math works) for any rate that's an exact kHz.
 */
static const struct dw_hdmi_audio_tmds_n common_tmds_n_table[] = {
	{ .tmds = 25175000, .n_32k = 4096, .n_44k1 = 12854, .n_48k = 6144, },
	{ .tmds = 25200000, .n_32k = 4096, .n_44k1 = 5656, .n_48k = 6144, },
	{ .tmds = 27000000, .n_32k = 4096, .n_44k1 = 5488, .n_48k = 6144, },
	{ .tmds = 28320000, .n_32k = 4096, .n_44k1 = 5586, .n_48k = 6144, },
	{ .tmds = 30240000, .n_32k = 4096, .n_44k1 = 5642, .n_48k = 6144, },
	{ .tmds = 31500000, .n_32k = 4096, .n_44k1 = 5600, .n_48k = 6144, },
	{ .tmds = 32000000, .n_32k = 4096, .n_44k1 = 5733, .n_48k = 6144, },
	{ .tmds = 33750000, .n_32k = 4096, .n_44k1 = 6272, .n_48k = 6144, },
	{ .tmds = 36000000, .n_32k = 4096, .n_44k1 = 5684, .n_48k = 6144, },
	{ .tmds = 40000000, .n_32k = 4096, .n_44k1 = 5733, .n_48k = 6144, },
	{ .tmds = 49500000, .n_32k = 4096, .n_44k1 = 5488, .n_48k = 6144, },
	{ .tmds = 50000000, .n_32k = 4096, .n_44k1 = 5292, .n_48k = 6144, },
	{ .tmds = 54000000, .n_32k = 4096, .n_44k1 = 5684, .n_48k = 6144, },
	{ .tmds = 65000000, .n_32k = 4096, .n_44k1 = 7056, .n_48k = 6144, },
	{ .tmds = 68250000, .n_32k = 4096, .n_44k1 = 5376, .n_48k = 6144, },
	{ .tmds = 71000000, .n_32k = 4096, .n_44k1 = 7056, .n_48k = 6144, },
	{ .tmds = 72000000, .n_32k = 4096, .n_44k1 = 5635, .n_48k = 6144, },
	{ .tmds = 73250000, .n_32k = 4096, .n_44k1 = 14112, .n_48k = 6144, },
	{ .tmds = 74250000, .n_32k = 4096, .n_44k1 = 6272, .n_48k = 6144, },
	{ .tmds = 75000000, .n_32k = 4096, .n_44k1 = 5880, .n_48k = 6144, },
	{ .tmds = 78750000, .n_32k = 4096, .n_44k1 = 5600, .n_48k = 6144, },
	{ .tmds = 78800000, .n_32k = 4096, .n_44k1 = 5292, .n_48k = 6144, },
	{ .tmds = 79500000, .n_32k = 4096, .n_44k1 = 4704, .n_48k = 6144, },
	{ .tmds = 83500000, .n_32k = 4096, .n_44k1 = 7056, .n_48k = 6144, },
	{ .tmds = 85500000, .n_32k = 4096, .n_44k1 = 5488, .n_48k = 6144, },
	{ .tmds = 88750000, .n_32k = 4096, .n_44k1 = 14112, .n_48k = 6144, },
	{ .tmds = 97750000, .n_32k = 4096, .n_44k1 = 14112, .n_48k = 6144, },
	{ .tmds = 101000000, .n_32k = 4096, .n_44k1 = 7056, .n_48k = 6144, },
	{ .tmds = 106500000, .n_32k = 4096, .n_44k1 = 4704, .n_48k = 6144, },
	{ .tmds = 108000000, .n_32k = 4096, .n_44k1 = 5684, .n_48k = 6144, },
	{ .tmds = 115500000, .n_32k = 4096, .n_44k1 = 5712, .n_48k = 6144, },
	{ .tmds = 119000000, .n_32k = 4096, .n_44k1 = 5544, .n_48k = 6144, },
	{ .tmds = 135000000, .n_32k = 4096, .n_44k1 = 5488, .n_48k = 6144, },
	{ .tmds = 146250000, .n_32k = 4096, .n_44k1 = 6272, .n_48k = 6144, },
	{ .tmds = 148500000, .n_32k = 4096, .n_44k1 = 5488, .n_48k = 6144, },
	{ .tmds = 154000000, .n_32k = 4096, .n_44k1 = 5544, .n_48k = 6144, },
	{ .tmds = 162000000, .n_32k = 4096, .n_44k1 = 5684, .n_48k = 6144, },

	/* For 297 MHz+ HDMI spec have some other rule for setting N */
	{ .tmds = 297000000, .n_32k = 3073, .n_44k1 = 4704, .n_48k = 5120, },
	{ .tmds = 594000000, .n_32k = 3073, .n_44k1 = 9408, .n_48k = 10240, },

	/* End of table */
	{ .tmds = 0,         .n_32k = 0,    .n_44k1 = 0,    .n_48k = 0, },
};

static const u16 csc_coeff_default[3][4] = {
	{ 0x2000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x2000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x2000, 0x0000 }
};

static const u16 csc_coeff_rgb_out_eitu601[3][4] = {
	{ 0x2000, 0x6926, 0x74fd, 0x010e },
	{ 0x2000, 0x2cdd, 0x0000, 0x7e9a },
	{ 0x2000, 0x0000, 0x38b4, 0x7e3b }
};

static const u16 csc_coeff_rgb_out_eitu709[3][4] = {
	{ 0x2000, 0x7106, 0x7a02, 0x00a7 },
	{ 0x2000, 0x3264, 0x0000, 0x7e6d },
	{ 0x2000, 0x0000, 0x3b61, 0x7e25 }
};

static const u16 csc_coeff_rgb_in_eitu601[3][4] = {
	{ 0x2591, 0x1322, 0x074b, 0x0000 },
	{ 0x6535, 0x2000, 0x7acc, 0x0200 },
	{ 0x6acd, 0x7534, 0x2000, 0x0200 }
};

static const u16 csc_coeff_rgb_in_eitu709[3][4] = {
	{ 0x2dc5, 0x0d9b, 0x049e, 0x0000 },
	{ 0x62f0, 0x2000, 0x7d11, 0x0200 },
	{ 0x6756, 0x78ab, 0x2000, 0x0200 }
};

static const u16 csc_coeff_full_to_limited[3][4] = {
	{ 0x36f7, 0x0000, 0x0000, 0x0040 },
	{ 0x0000, 0x36f7, 0x0000, 0x0040 },
	{ 0x0000, 0x0000, 0x36f7, 0x0040 }
};

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

struct hdcp_keys {
	u8 KSV[8];
	u8 devicekey[HDCP_PRIVATE_KEY_SIZE];
	u8 sha1[HDCP_KEY_SHA_SIZE];
	u8 seeds[2];
};

struct dw_hdmi_i2c {
	u8			slave_reg;
	bool			is_regaddr;
	bool			is_segment;

	unsigned int		scl_high_ns;
	unsigned int		scl_low_ns;
};

struct dw_hdmi {
	int id;
	enum dw_hdmi_devtype dev_type;
	unsigned int version;
	struct hdmi_data_info hdmi_data;
	struct hdmi_edid_data edid_data;
	const struct dw_hdmi_plat_data *plat_data;
	struct ddc_adapter adap;

	int vic;
	int io_width;

	unsigned long bus_format;
	bool cable_plugin;
	bool sink_is_hdmi;
	bool sink_has_audio;
	void *regs;
	void *grf;
	void *gpio_base;
	struct dw_hdmi_i2c *i2c;

	struct {
		const struct dw_hdmi_phy_ops *ops;
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

	void (*write)(struct dw_hdmi *hdmi, u8 val, int offset);
	u8 (*read)(struct dw_hdmi *hdmi, int offset);

	bool hdcp1x_enable;
	bool output_bus_format_rgb;

	struct gpio_desc hpd_gpiod;
};

static void dw_hdmi_writel(struct dw_hdmi *hdmi, u8 val, int offset)
{
	writel(val, hdmi->regs + (offset << 2));
}

static u8 dw_hdmi_readl(struct dw_hdmi *hdmi, int offset)
{
	return readl(hdmi->regs + (offset << 2));
}

static void dw_hdmi_writeb(struct dw_hdmi *hdmi, u8 val, int offset)
{
	writeb(val, hdmi->regs + offset);
}

static u8 dw_hdmi_readb(struct dw_hdmi *hdmi, int offset)
{
	return readb(hdmi->regs + offset);
}

static inline void hdmi_writeb(struct dw_hdmi *hdmi, u8 val, int offset)
{
	hdmi->write(hdmi, val, offset);
}

static inline u8 hdmi_readb(struct dw_hdmi *hdmi, int offset)
{
	return hdmi->read(hdmi, offset);
}

static void hdmi_modb(struct dw_hdmi *hdmi, u8 data, u8 mask, unsigned reg)
{
	u8 val = hdmi_readb(hdmi, reg) & ~mask;

	val |= data & mask;
	hdmi_writeb(hdmi, val, reg);
}

static void hdmi_mask_writeb(struct dw_hdmi *hdmi, u8 data, unsigned int reg,
			     u8 shift, u8 mask)
{
	hdmi_modb(hdmi, data << shift, mask, reg);
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

static int is_color_space_conversion(struct dw_hdmi *hdmi)
{
	struct drm_display_mode *mode =
		hdmi->edid_data.preferred_mode;
	bool is_cea_default;

	is_cea_default = (drm_match_cea_mode(mode) > 1) &&
			 (hdmi->hdmi_data.quant_range ==
			  HDMI_QUANTIZATION_RANGE_DEFAULT);

	/*
	 * When output is rgb limited range or default range with
	 * cea mode, csc should be enabled.
	 */
	if (hdmi->hdmi_data.enc_in_bus_format !=
	    hdmi->hdmi_data.enc_out_bus_format ||
	    ((hdmi->hdmi_data.quant_range == HDMI_QUANTIZATION_RANGE_LIMITED ||
	      is_cea_default) &&
	     hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_in_bus_format)))
		return 1;

	return 0;
}

static int is_color_space_decimation(struct dw_hdmi *hdmi)
{
	if (!hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_out_bus_format))
		return 0;

	if (hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_in_bus_format) ||
	    hdmi_bus_fmt_is_yuv444(hdmi->hdmi_data.enc_in_bus_format))
		return 1;

	return 0;
}

static inline void hdmi_phy_test_clear(struct dw_hdmi *hdmi,
				       unsigned char bit)
{
	hdmi_modb(hdmi, bit << HDMI_PHY_TST0_TSTCLR_OFFSET,
		  HDMI_PHY_TST0_TSTCLR_MASK, HDMI_PHY_TST0);
}

static inline void hdmi_phy_test_enable(struct dw_hdmi *hdmi,
					unsigned char bit)
{
	hdmi_modb(hdmi, bit << HDMI_PHY_TST0_TSTEN_OFFSET,
		  HDMI_PHY_TST0_TSTEN_MASK, HDMI_PHY_TST0);
}

static inline void hdmi_phy_test_clock(struct dw_hdmi *hdmi,
				       unsigned char bit)
{
	hdmi_modb(hdmi, bit << HDMI_PHY_TST0_TSTCLK_OFFSET,
		  HDMI_PHY_TST0_TSTCLK_MASK, HDMI_PHY_TST0);
}

static inline void hdmi_phy_test_din(struct dw_hdmi *hdmi,
				     unsigned char bit)
{
	hdmi_writeb(hdmi, bit, HDMI_PHY_TST1);
}

static inline void hdmi_phy_test_dout(struct dw_hdmi *hdmi,
				      unsigned char bit)
{
	hdmi_writeb(hdmi, bit, HDMI_PHY_TST2);
}

static int dw_hdmi_i2c_read(struct dw_hdmi *hdmi,
			    unsigned char *buf, unsigned int length)
{
	struct dw_hdmi_i2c *i2c = hdmi->i2c;
	int interrupt = 0, i = 20;

	if (!i2c->is_regaddr) {
		printf("set read register address to 0\n");
		i2c->slave_reg = 0x00;
		i2c->is_regaddr = true;
	}

	while (length--) {
		hdmi_writeb(hdmi, i2c->slave_reg++, HDMI_I2CM_ADDRESS);
		if (i2c->is_segment)
			hdmi_writeb(hdmi, HDMI_I2CM_OPERATION_READ_EXT,
				    HDMI_I2CM_OPERATION);
		else
			hdmi_writeb(hdmi, HDMI_I2CM_OPERATION_READ,
				    HDMI_I2CM_OPERATION);

		while (i--) {
			udelay(1000);
			interrupt = hdmi_readb(hdmi, HDMI_IH_I2CM_STAT0);
			if (interrupt)
				hdmi_writeb(hdmi, interrupt,
					    HDMI_IH_I2CM_STAT0);
			if (interrupt & (m_SCDC_READREQ | m_I2CM_DONE |
					 m_I2CM_ERROR))
				break;
		}

		if (!interrupt) {
			printf("[%s] i2c read reg[0x%02x] no interrupt\n",
			       __func__, i2c->slave_reg);
			return -EAGAIN;
		}

		/* Check for error condition on the bus */
		if (interrupt & HDMI_IH_I2CM_STAT0_ERROR) {
			printf("[%s] read reg[0x%02x] data error:0x%02x\n",
			       __func__, i2c->slave_reg, interrupt);
			return -EIO;
		}

		i = 20;
		*buf++ = hdmi_readb(hdmi, HDMI_I2CM_DATAI);
	}
	i2c->is_segment = false;

	return 0;
}

static int dw_hdmi_i2c_write(struct dw_hdmi *hdmi,
			     unsigned char *buf, unsigned int length)
{
	struct dw_hdmi_i2c *i2c = hdmi->i2c;
	int i = 20;
	u8 interrupt = 0;

	if (!i2c->is_regaddr) {
		/* Use the first write byte as register address */
		i2c->slave_reg = buf[0];
		length--;
		buf++;
		i2c->is_regaddr = true;
	}

	while (length--) {
		hdmi_writeb(hdmi, *buf++, HDMI_I2CM_DATAO);
		hdmi_writeb(hdmi, i2c->slave_reg++, HDMI_I2CM_ADDRESS);
		hdmi_writeb(hdmi, HDMI_I2CM_OPERATION_WRITE,
			    HDMI_I2CM_OPERATION);

		while (i--) {
			udelay(1000);
			interrupt = hdmi_readb(hdmi, HDMI_IH_I2CM_STAT0);
			if (interrupt)
				hdmi_writeb(hdmi,
					    interrupt, HDMI_IH_I2CM_STAT0);

			if (interrupt & (m_SCDC_READREQ |
					 m_I2CM_DONE | m_I2CM_ERROR))
				break;
		}

		if ((interrupt & m_I2CM_ERROR) || (i == -1)) {
			printf("[%s] write data error\n", __func__);
			return -EIO;
		} else if (interrupt & m_I2CM_DONE) {
			printf("[%s] write offset %02x success\n",
			       __func__, i2c->slave_reg);
			return -EAGAIN;
		}

		i = 20;
	}

	return 0;
}

static int dw_hdmi_i2c_xfer(struct ddc_adapter *adap,
			    struct i2c_msg *msgs, int num)
{
	struct dw_hdmi *hdmi = container_of(adap, struct dw_hdmi, adap);
	struct dw_hdmi_i2c *i2c = hdmi->i2c;
	u8 addr = msgs[0].addr;
	int i, ret = 0;

	printf("xfer: num: %d, addr: %#x\n", num, addr);
	for (i = 0; i < num; i++) {
		if (msgs[i].len == 0) {
			printf("unsupported transfer %d/%d, no data\n",
			       i + 1, num);
			return -EOPNOTSUPP;
		}
	}

	hdmi_writeb(hdmi, 0x00, HDMI_IH_MUTE_I2CM_STAT0);

	/* Set slave device address taken from the first I2C message */
	if (addr == DDC_SEGMENT_ADDR && msgs[0].len == 1)
		addr = DDC_ADDR;
	hdmi_writeb(hdmi, addr, HDMI_I2CM_SLAVE);

	/* Set slave device register address on transfer */
	i2c->is_regaddr = false;

	/* Set segment pointer for I2C extended read mode operation */
	i2c->is_segment = false;

	for (i = 0; i < num; i++) {
		debug("xfer: num: %d/%d, len: %d, flags: %#x\n",
		      i + 1, num, msgs[i].len, msgs[i].flags);
		if (msgs[i].addr == DDC_SEGMENT_ADDR && msgs[i].len == 1) {
			i2c->is_segment = true;
			hdmi_writeb(hdmi, DDC_SEGMENT_ADDR, HDMI_I2CM_SEGADDR);
			hdmi_writeb(hdmi, *msgs[i].buf, HDMI_I2CM_SEGPTR);
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
	hdmi_writeb(hdmi, HDMI_IH_I2CM_STAT0_ERROR | HDMI_IH_I2CM_STAT0_DONE,
		    HDMI_IH_MUTE_I2CM_STAT0);

	return ret;
}

static bool hdmi_phy_wait_i2c_done(struct dw_hdmi *hdmi, int msec)
{
	u32 val;

	while ((val = hdmi_readb(hdmi, HDMI_IH_I2CMPHY_STAT0) & 0x3) == 0) {
		if (msec-- == 0)
			return false;
		udelay(1000);
	}
	hdmi_writeb(hdmi, val, HDMI_IH_I2CMPHY_STAT0);

	return true;
}

static void dw_hdmi_phy_i2c_write(struct dw_hdmi *hdmi, unsigned short data,
				  unsigned char addr)
{
	hdmi_writeb(hdmi, 0xFF, HDMI_IH_I2CMPHY_STAT0);
	hdmi_writeb(hdmi, addr, HDMI_PHY_I2CM_ADDRESS_ADDR);
	hdmi_writeb(hdmi, (unsigned char)(data >> 8),
		    HDMI_PHY_I2CM_DATAO_1_ADDR);
	hdmi_writeb(hdmi, (unsigned char)(data >> 0),
		    HDMI_PHY_I2CM_DATAO_0_ADDR);
	hdmi_writeb(hdmi, HDMI_PHY_I2CM_OPERATION_ADDR_WRITE,
		    HDMI_PHY_I2CM_OPERATION_ADDR);
	hdmi_phy_wait_i2c_done(hdmi, 1000);
}

static void dw_hdmi_phy_enable_powerdown(struct dw_hdmi *hdmi, bool enable)
{
	hdmi_mask_writeb(hdmi, !enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_PDZ_OFFSET,
			 HDMI_PHY_CONF0_PDZ_MASK);
}

static void dw_hdmi_phy_enable_tmds(struct dw_hdmi *hdmi, u8 enable)
{
	hdmi_mask_writeb(hdmi, enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_ENTMDS_OFFSET,
			 HDMI_PHY_CONF0_ENTMDS_MASK);
}

static void dw_hdmi_phy_enable_svsret(struct dw_hdmi *hdmi, u8 enable)
{
	hdmi_mask_writeb(hdmi, enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_SVSRET_OFFSET,
			 HDMI_PHY_CONF0_SVSRET_MASK);
}

static void dw_hdmi_phy_gen2_pddq(struct dw_hdmi *hdmi, u8 enable)
{
	hdmi_mask_writeb(hdmi, enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_GEN2_PDDQ_OFFSET,
			 HDMI_PHY_CONF0_GEN2_PDDQ_MASK);
}

static void dw_hdmi_phy_gen2_txpwron(struct dw_hdmi *hdmi, u8 enable)
{
	hdmi_mask_writeb(hdmi, enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_GEN2_TXPWRON_OFFSET,
			 HDMI_PHY_CONF0_GEN2_TXPWRON_MASK);
}

static void dw_hdmi_phy_sel_data_en_pol(struct dw_hdmi *hdmi, u8 enable)
{
	hdmi_mask_writeb(hdmi, enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_SELDATAENPOL_OFFSET,
			 HDMI_PHY_CONF0_SELDATAENPOL_MASK);
}

static void dw_hdmi_phy_sel_interface_control(struct dw_hdmi *hdmi, u8 enable)
{
	hdmi_mask_writeb(hdmi, enable, HDMI_PHY_CONF0,
			 HDMI_PHY_CONF0_SELDIPIF_OFFSET,
			 HDMI_PHY_CONF0_SELDIPIF_MASK);
}

static void dw_hdmi_phy_power_off(struct dw_hdmi *hdmi)
{
	const struct dw_hdmi_phy_data *phy = hdmi->phy.data;
	unsigned int i;
	u16 val;

	if (phy->gen == 1) {
		dw_hdmi_phy_enable_tmds(hdmi, 0);
		dw_hdmi_phy_enable_powerdown(hdmi, true);
		return;
	}

	dw_hdmi_phy_gen2_txpwron(hdmi, 0);

	/*
	 * Wait for TX_PHY_LOCK to be deasserted to indicate that the PHY went
	 * to low power mode.
	 */
	for (i = 0; i < 5; ++i) {
		val = hdmi_readb(hdmi, HDMI_PHY_STAT0);
		if (!(val & HDMI_PHY_TX_PHY_LOCK))
			break;

		udelay(2000);
	}

	if (val & HDMI_PHY_TX_PHY_LOCK)
		printf("PHY failed to power down\n");
	else
		printf("PHY powered down in %u iterations\n", i);

	dw_hdmi_phy_gen2_pddq(hdmi, 1);
}

static int dw_hdmi_phy_power_on(struct dw_hdmi *hdmi)
{
	const struct dw_hdmi_phy_data *phy = hdmi->phy.data;
	unsigned int i;
	u8 val;

	if (phy->gen == 1) {
		dw_hdmi_phy_enable_powerdown(hdmi, false);

		/* Toggle TMDS enable. */
		dw_hdmi_phy_enable_tmds(hdmi, 0);
		dw_hdmi_phy_enable_tmds(hdmi, 1);
		return 0;
	}

	dw_hdmi_phy_gen2_txpwron(hdmi, 1);
	dw_hdmi_phy_gen2_pddq(hdmi, 0);

	/* Wait for PHY PLL lock */
	for (i = 0; i < 5; ++i) {
		val = hdmi_readb(hdmi, HDMI_PHY_STAT0) & HDMI_PHY_TX_PHY_LOCK;
		if (val)
			break;

		udelay(2000);
	}

	if (!val) {
		printf("PHY PLL failed to lock\n");
		return -ETIMEDOUT;
	}
	printf("PHY PLL locked %u iterations\n", i);

	return 0;
}

/*
 * PHY configuration function for the DWC HDMI 3D TX PHY. Based on the available
 * information the DWC MHL PHY has the same register layout and is thus also
 * supported by this function.
 */
static
int hdmi_phy_configure_dwc_hdmi_3d_tx(struct dw_hdmi *hdmi,
				      const struct dw_hdmi_plat_data *pdata,
				      unsigned long mpixelclock)
{
	const struct dw_hdmi_mpll_config *mpll_config = pdata->mpll_cfg;
	const struct dw_hdmi_curr_ctrl *curr_ctrl = pdata->cur_ctr;
	const struct dw_hdmi_phy_config *phy_config = pdata->phy_config;
	unsigned int tmdsclock = hdmi->hdmi_data.video_mode.mtmdsclock;
	unsigned int depth =
		hdmi_bus_fmt_color_depth(hdmi->hdmi_data.enc_out_bus_format);

	if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format) &&
	    pdata->mpll_cfg_420)
		mpll_config = pdata->mpll_cfg_420;

	/* PLL/MPLL Cfg - always match on final entry */
	for (; mpll_config->mpixelclock != ~0UL; mpll_config++)
		if (mpixelclock <= mpll_config->mpixelclock)
			break;

	for (; curr_ctrl->mpixelclock != ~0UL; curr_ctrl++)
		if (tmdsclock <= curr_ctrl->mpixelclock)
			break;

	for (; phy_config->mpixelclock != ~0UL; phy_config++)
		if (tmdsclock <= phy_config->mpixelclock)
			break;

	if (mpll_config->mpixelclock == ~0UL ||
	    curr_ctrl->mpixelclock == ~0UL ||
	    phy_config->mpixelclock == ~0UL)
		return -EINVAL;

	if (!hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_out_bus_format))
		depth = fls(depth - 8);
	else
		depth = 0;
	if (depth)
		depth--;

	dw_hdmi_phy_i2c_write(hdmi, mpll_config->res[depth].cpce,
			      HDMI_3D_TX_PHY_CPCE_CTRL);

	dw_hdmi_phy_i2c_write(hdmi, mpll_config->res[depth].gmp,
			      HDMI_3D_TX_PHY_GMPCTRL);
	dw_hdmi_phy_i2c_write(hdmi, curr_ctrl->curr[depth],
			      HDMI_3D_TX_PHY_CURRCTRL);

	dw_hdmi_phy_i2c_write(hdmi, 0, HDMI_3D_TX_PHY_PLLPHBYCTRL);
	dw_hdmi_phy_i2c_write(hdmi, HDMI_3D_TX_PHY_MSM_CTRL_CKO_SEL_FB_CLK,
			      HDMI_3D_TX_PHY_MSM_CTRL);

	dw_hdmi_phy_i2c_write(hdmi, phy_config->term, HDMI_3D_TX_PHY_TXTERM);
	dw_hdmi_phy_i2c_write(hdmi, phy_config->sym_ctr,
			      HDMI_3D_TX_PHY_CKSYMTXCTRL);
	dw_hdmi_phy_i2c_write(hdmi, phy_config->vlev_ctr,
			      HDMI_3D_TX_PHY_VLEVCTRL);

	return 0;
}

static const struct dw_hdmi_phy_data dw_hdmi_phys[] = {
	{
		.type = DW_HDMI_PHY_DWC_HDMI_TX_PHY,
		.name = "DWC HDMI TX PHY",
		.gen = 1,
	}, {
		.type = DW_HDMI_PHY_DWC_MHL_PHY_HEAC,
		.name = "DWC MHL PHY + HEAC PHY",
		.gen = 2,
		.has_svsret = true,
		.configure = hdmi_phy_configure_dwc_hdmi_3d_tx,
	}, {
		.type = DW_HDMI_PHY_DWC_MHL_PHY,
		.name = "DWC MHL PHY",
		.gen = 2,
		.has_svsret = true,
		.configure = hdmi_phy_configure_dwc_hdmi_3d_tx,
	}, {
		.type = DW_HDMI_PHY_DWC_HDMI_3D_TX_PHY_HEAC,
		.name = "DWC HDMI 3D TX PHY + HEAC PHY",
		.gen = 2,
		.configure = hdmi_phy_configure_dwc_hdmi_3d_tx,
	}, {
		.type = DW_HDMI_PHY_DWC_HDMI_3D_TX_PHY,
		.name = "DWC HDMI 3D TX PHY",
		.gen = 2,
		.configure = hdmi_phy_configure_dwc_hdmi_3d_tx,
	}, {
		.type = DW_HDMI_PHY_DWC_HDMI20_TX_PHY,
		.name = "DWC HDMI 2.0 TX PHY",
		.gen = 2,
		.has_svsret = true,
		.configure = hdmi_phy_configure_dwc_hdmi_3d_tx,
	}, {
		.type = DW_HDMI_PHY_VENDOR_PHY,
		.name = "Vendor PHY",
	}
};

static int rockchip_dw_hdmi_scrambling_enable(struct dw_hdmi *hdmi,
					      int enable)
{
	u8 stat;

	drm_scdc_readb(&hdmi->adap, SCDC_TMDS_CONFIG, &stat);

	if (stat < 0) {
		debug("Failed to read tmds config\n");
		return false;
	}

	if (enable == 1) {
		/* Write on Rx the bit Scrambling_Enable, register 0x20 */
		stat |= SCDC_SCRAMBLING_ENABLE;
		drm_scdc_writeb(&hdmi->adap, SCDC_TMDS_CONFIG, stat);
		/* TMDS software reset request */
		hdmi_writeb(hdmi, (u8)~HDMI_MC_SWRSTZ_TMDSSWRST_REQ,
			    HDMI_MC_SWRSTZ);
		/* Enable/Disable Scrambling */
		hdmi_writeb(hdmi, 1, HDMI_FC_SCRAMBLER_CTRL);
	} else {
		/* Enable/Disable Scrambling */
		hdmi_writeb(hdmi, 0, HDMI_FC_SCRAMBLER_CTRL);
		/* TMDS software reset request */
		hdmi_writeb(hdmi, (u8)~HDMI_MC_SWRSTZ_TMDSSWRST_REQ,
			    HDMI_MC_SWRSTZ);
		/* Write on Rx the bit Scrambling_Enable, register 0x20 */
		stat &= ~SCDC_SCRAMBLING_ENABLE;
		drm_scdc_writeb(&hdmi->adap, SCDC_TMDS_CONFIG, stat);
	}

	return 0;
}

static void rockchip_dw_hdmi_scdc_set_tmds_rate(struct dw_hdmi *hdmi)
{
	u8 stat;

	drm_scdc_readb(&hdmi->adap, SCDC_TMDS_CONFIG, &stat);
	if (hdmi->hdmi_data.video_mode.mtmdsclock > 340000000)
		stat |= SCDC_TMDS_BIT_CLOCK_RATIO_BY_40;
	else
		stat &= ~SCDC_TMDS_BIT_CLOCK_RATIO_BY_40;
	drm_scdc_writeb(&hdmi->adap, SCDC_TMDS_CONFIG, stat);
}

static int hdmi_phy_configure(struct dw_hdmi *hdmi)
{
	const struct dw_hdmi_phy_data *phy = hdmi->phy.data;
	const struct dw_hdmi_plat_data *pdata = hdmi->plat_data;
	unsigned long mpixelclock = hdmi->hdmi_data.video_mode.mpixelclock;
	unsigned long mtmdsclock = hdmi->hdmi_data.video_mode.mtmdsclock;
	int ret;

	dw_hdmi_phy_power_off(hdmi);

	/* Control for TMDS Bit Period/TMDS Clock-Period Ratio */
	if (hdmi->edid_data.display_info.hdmi.scdc.supported)
		rockchip_dw_hdmi_scdc_set_tmds_rate(hdmi);

	/* Leave low power consumption mode by asserting SVSRET. */
	if (phy->has_svsret)
		dw_hdmi_phy_enable_svsret(hdmi, 1);

	/* PHY reset. The reset signal is active high on Gen2 PHYs. */
	hdmi_writeb(hdmi, HDMI_MC_PHYRSTZ_PHYRSTZ, HDMI_MC_PHYRSTZ);
	hdmi_writeb(hdmi, 0, HDMI_MC_PHYRSTZ);

	hdmi_writeb(hdmi, HDMI_MC_HEACPHY_RST_ASSERT, HDMI_MC_HEACPHY_RST);

	hdmi_phy_test_clear(hdmi, 1);
	hdmi_writeb(hdmi, HDMI_PHY_I2CM_SLAVE_ADDR_PHY_GEN2,
		    HDMI_PHY_I2CM_SLAVE_ADDR);
	hdmi_phy_test_clear(hdmi, 0);

	/* Write to the PHY as configured by the platform */
	if (pdata->configure_phy)
		ret = pdata->configure_phy(hdmi, pdata, mpixelclock);
	else
		ret = phy->configure(hdmi, pdata, mpixelclock);
	if (ret) {
		printf("PHY configuration failed (clock %lu)\n",
		       mpixelclock);
		return ret;
	}

	/* Wait for resuming transmission of TMDS clock and data */
	if (mtmdsclock > 340000000)
		mdelay(100);

	return dw_hdmi_phy_power_on(hdmi);
}

static int dw_hdmi_phy_init(struct rockchip_connector *conn, struct dw_hdmi *hdmi,
			    void *data)
{
	int i, ret;

	/* HDMI Phy spec says to do the phy initialization sequence twice */
	for (i = 0; i < 2; i++) {
		dw_hdmi_phy_sel_data_en_pol(hdmi, 1);
		dw_hdmi_phy_sel_interface_control(hdmi, 0);
		ret = hdmi_phy_configure(hdmi);
		if (ret)
			return ret;
	}

	return 0;
}

static void dw_hdmi_phy_disable(struct rockchip_connector *conn, struct dw_hdmi *hdmi,
				void *data)
{
	dw_hdmi_phy_power_off(hdmi);
}

static enum drm_connector_status
dw_hdmi_phy_read_hpd(struct dw_hdmi *hdmi, void *data)
{
	return hdmi_readb(hdmi, HDMI_PHY_STAT0) & HDMI_PHY_HPD ?
		connector_status_connected : connector_status_disconnected;
}

static const struct dw_hdmi_phy_ops dw_hdmi_synopsys_phy_ops = {
	.init = dw_hdmi_phy_init,
	.disable = dw_hdmi_phy_disable,
	.read_hpd = dw_hdmi_phy_read_hpd,
};

static int dw_hdmi_detect_phy(struct dw_hdmi *hdmi)
{
	unsigned int i;
	u8 phy_type;

	phy_type = hdmi_readb(hdmi, HDMI_CONFIG2_ID);

	/*
	 * RK3228 and RK3328 phy_type is DW_HDMI_PHY_DWC_HDMI20_TX_PHY,
	 * but it has a vedor phy.
	 */
	if (phy_type == DW_HDMI_PHY_VENDOR_PHY ||
	    hdmi->dev_type == RK3528_HDMI ||
	    hdmi->dev_type == RK3328_HDMI ||
	    hdmi->dev_type == RK3228_HDMI) {
		/* Vendor PHYs require support from the glue layer. */
		if (!hdmi->plat_data->phy_ops || !hdmi->plat_data->phy_name) {
			printf(
				"Vendor HDMI PHY not supported by glue layer\n");
			return -ENODEV;
		}

		hdmi->phy.ops = hdmi->plat_data->phy_ops;
		hdmi->phy.data = hdmi->plat_data->phy_data;
		hdmi->phy.name = hdmi->plat_data->phy_name;
		return 0;
	}

	/* Synopsys PHYs are handled internally. */
	for (i = 0; i < ARRAY_SIZE(dw_hdmi_phys); ++i) {
		if (dw_hdmi_phys[i].type == phy_type) {
			hdmi->phy.ops = &dw_hdmi_synopsys_phy_ops;
			hdmi->phy.name = dw_hdmi_phys[i].name;
			hdmi->phy.data = (void *)&dw_hdmi_phys[i];

			if (!dw_hdmi_phys[i].configure &&
			    !hdmi->plat_data->configure_phy) {
				printf("%s requires platform support\n",
				       hdmi->phy.name);
				return -ENODEV;
			}

			return 0;
		}
	}

	printf("Unsupported HDMI PHY type (%02x)\n", phy_type);
	return -ENODEV;
}

static unsigned int
hdmi_get_tmdsclock(struct dw_hdmi *hdmi, unsigned long mpixelclock)
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

static void hdmi_av_composer(struct dw_hdmi *hdmi,
			     const struct drm_display_mode *mode)
{
	u8 bytes = 0, inv_val = 0;
	struct hdmi_vmode *vmode = &hdmi->hdmi_data.video_mode;
	struct drm_hdmi_info *hdmi_info = &hdmi->edid_data.display_info.hdmi;
	int hblank, vblank, h_de_hs, v_de_vs, hsync_len, vsync_len;
	unsigned int hdisplay, vdisplay;

	vmode->mpixelclock = mode->crtc_clock * 1000;
	if ((mode->flags & DRM_MODE_FLAG_3D_MASK) ==
		DRM_MODE_FLAG_3D_FRAME_PACKING)
		vmode->mpixelclock *= 2;
	vmode->mtmdsclock = hdmi_get_tmdsclock(hdmi, vmode->mpixelclock);
	if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format))
		vmode->mtmdsclock /= 2;
	printf("final pixclk = %d tmdsclk = %d\n",
	       vmode->mpixelclock, vmode->mtmdsclock);

	/* Set up HDMI_FC_INVIDCONF
	 * fc_invidconf.HDCP_keepout must be set (1'b1)
	 * when activate the scrambler feature.
	 */
	inv_val = (vmode->mtmdsclock > 340000000 ||
		   (hdmi_info->scdc.scrambling.low_rates &&
		   hdmi->scramble_low_rates) ?
		   HDMI_FC_INVIDCONF_HDCP_KEEPOUT_ACTIVE :
		   HDMI_FC_INVIDCONF_HDCP_KEEPOUT_INACTIVE);

	inv_val |= mode->flags & DRM_MODE_FLAG_PVSYNC ?
		HDMI_FC_INVIDCONF_VSYNC_IN_POLARITY_ACTIVE_HIGH :
		HDMI_FC_INVIDCONF_VSYNC_IN_POLARITY_ACTIVE_LOW;

	inv_val |= mode->flags & DRM_MODE_FLAG_PHSYNC ?
		HDMI_FC_INVIDCONF_HSYNC_IN_POLARITY_ACTIVE_HIGH :
		HDMI_FC_INVIDCONF_HSYNC_IN_POLARITY_ACTIVE_LOW;

	inv_val |= (vmode->mdataenablepolarity ?
		HDMI_FC_INVIDCONF_DE_IN_POLARITY_ACTIVE_HIGH :
		HDMI_FC_INVIDCONF_DE_IN_POLARITY_ACTIVE_LOW);

	if (hdmi->vic == 39)
		inv_val |= HDMI_FC_INVIDCONF_R_V_BLANK_IN_OSC_ACTIVE_HIGH;
	else
		inv_val |= mode->flags & DRM_MODE_FLAG_INTERLACE ?
			HDMI_FC_INVIDCONF_R_V_BLANK_IN_OSC_ACTIVE_HIGH :
			HDMI_FC_INVIDCONF_R_V_BLANK_IN_OSC_ACTIVE_LOW;

	inv_val |= mode->flags & DRM_MODE_FLAG_INTERLACE ?
		HDMI_FC_INVIDCONF_IN_I_P_INTERLACED :
		HDMI_FC_INVIDCONF_IN_I_P_PROGRESSIVE;

	inv_val |= hdmi->sink_is_hdmi ?
		HDMI_FC_INVIDCONF_DVI_MODEZ_HDMI_MODE :
		HDMI_FC_INVIDCONF_DVI_MODEZ_DVI_MODE;

	hdmi_writeb(hdmi, inv_val, HDMI_FC_INVIDCONF);

	hdisplay = mode->hdisplay;
	hblank = mode->htotal - mode->hdisplay;
	h_de_hs = mode->hsync_start - mode->hdisplay;
	hsync_len = mode->hsync_end - mode->hsync_start;

	/*
	 * When we're setting a YCbCr420 mode, we need
	 * to adjust the horizontal timing to suit.
	 */
	if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format)) {
		hdisplay /= 2;
		hblank /= 2;
		h_de_hs /= 2;
		hsync_len /= 2;
	}

	vdisplay = mode->vdisplay;
	vblank = mode->vtotal - mode->vdisplay;
	v_de_vs = mode->vsync_start - mode->vdisplay;
	vsync_len = mode->vsync_end - mode->vsync_start;

	/*
	 * When we're setting an interlaced mode, we need
	 * to adjust the vertical timing to suit.
	 */
	if (mode->flags & DRM_MODE_FLAG_INTERLACE) {
		vdisplay /= 2;
		vblank /= 2;
		v_de_vs /= 2;
		vsync_len /= 2;
	} else if ((mode->flags & DRM_MODE_FLAG_3D_MASK) ==
		DRM_MODE_FLAG_3D_FRAME_PACKING) {
		vdisplay += mode->vtotal;
	}

	/* Scrambling Control */
	if (hdmi_info->scdc.supported) {
		if (vmode->mtmdsclock > 340000000 ||
		    (hdmi_info->scdc.scrambling.low_rates &&
		     hdmi->scramble_low_rates)) {
			drm_scdc_readb(&hdmi->adap, SCDC_SINK_VERSION, &bytes);
			drm_scdc_writeb(&hdmi->adap, SCDC_SOURCE_VERSION,
					bytes);
			rockchip_dw_hdmi_scrambling_enable(hdmi, 1);
		} else {
			rockchip_dw_hdmi_scrambling_enable(hdmi, 0);
		}
	}

	/* Set up horizontal active pixel width */
	hdmi_writeb(hdmi, hdisplay >> 8, HDMI_FC_INHACTV1);
	hdmi_writeb(hdmi, hdisplay, HDMI_FC_INHACTV0);

	/* Set up vertical active lines */
	hdmi_writeb(hdmi, vdisplay >> 8, HDMI_FC_INVACTV1);
	hdmi_writeb(hdmi, vdisplay, HDMI_FC_INVACTV0);

	/* Set up horizontal blanking pixel region width */
	hdmi_writeb(hdmi, hblank >> 8, HDMI_FC_INHBLANK1);
	hdmi_writeb(hdmi, hblank, HDMI_FC_INHBLANK0);

	/* Set up vertical blanking pixel region width */
	hdmi_writeb(hdmi, vblank, HDMI_FC_INVBLANK);

	/* Set up HSYNC active edge delay width (in pixel clks) */
	hdmi_writeb(hdmi, h_de_hs >> 8, HDMI_FC_HSYNCINDELAY1);
	hdmi_writeb(hdmi, h_de_hs, HDMI_FC_HSYNCINDELAY0);

	/* Set up VSYNC active edge delay (in lines) */
	hdmi_writeb(hdmi, v_de_vs, HDMI_FC_VSYNCINDELAY);

	/* Set up HSYNC active pulse width (in pixel clks) */
	hdmi_writeb(hdmi, hsync_len >> 8, HDMI_FC_HSYNCINWIDTH1);
	hdmi_writeb(hdmi, hsync_len, HDMI_FC_HSYNCINWIDTH0);

	/* Set up VSYNC active edge delay (in lines) */
	hdmi_writeb(hdmi, vsync_len, HDMI_FC_VSYNCINWIDTH);
}

static void dw_hdmi_update_csc_coeffs(struct dw_hdmi *hdmi)
{
	const u16 (*csc_coeff)[3][4] = &csc_coeff_default;
	unsigned i;
	u32 csc_scale = 1;
	int enc_out_rgb, enc_in_rgb;

	enc_out_rgb = hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_out_bus_format);
	enc_in_rgb = hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_in_bus_format);

	if (is_color_space_conversion(hdmi)) {
		if (enc_out_rgb && enc_in_rgb) {
			csc_coeff = &csc_coeff_full_to_limited;
			csc_scale = 0;
		} else if (enc_out_rgb) {
			if (hdmi->hdmi_data.enc_out_encoding ==
						V4L2_YCBCR_ENC_601)
				csc_coeff = &csc_coeff_rgb_out_eitu601;
			else
				csc_coeff = &csc_coeff_rgb_out_eitu709;
		} else if (enc_in_rgb) {
			if (hdmi->hdmi_data.enc_out_encoding ==
						V4L2_YCBCR_ENC_601)
				csc_coeff = &csc_coeff_rgb_in_eitu601;
			else
				csc_coeff = &csc_coeff_rgb_in_eitu709;
			csc_scale = 0;
		}
	}

	/* The CSC registers are sequential, alternating MSB then LSB */
	for (i = 0; i < ARRAY_SIZE(csc_coeff_default[0]); i++) {
		u16 coeff_a = (*csc_coeff)[0][i];
		u16 coeff_b = (*csc_coeff)[1][i];
		u16 coeff_c = (*csc_coeff)[2][i];

		hdmi_writeb(hdmi, coeff_a & 0xff, HDMI_CSC_COEF_A1_LSB + i * 2);
		hdmi_writeb(hdmi, coeff_a >> 8, HDMI_CSC_COEF_A1_MSB + i * 2);
		hdmi_writeb(hdmi, coeff_b & 0xff, HDMI_CSC_COEF_B1_LSB + i * 2);
		hdmi_writeb(hdmi, coeff_b >> 8, HDMI_CSC_COEF_B1_MSB + i * 2);
		hdmi_writeb(hdmi, coeff_c & 0xff, HDMI_CSC_COEF_C1_LSB + i * 2);
		hdmi_writeb(hdmi, coeff_c >> 8, HDMI_CSC_COEF_C1_MSB + i * 2);
	}

	hdmi_modb(hdmi, csc_scale, HDMI_CSC_SCALE_CSCSCALE_MASK,
		  HDMI_CSC_SCALE);
}

static int is_color_space_interpolation(struct dw_hdmi *hdmi)
{
	if (!hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_in_bus_format))
		return 0;

	if (hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_out_bus_format) ||
	    hdmi_bus_fmt_is_yuv444(hdmi->hdmi_data.enc_out_bus_format))
		return 1;

	return 0;
}

static void hdmi_video_csc(struct dw_hdmi *hdmi)
{
	int color_depth = 0;
	int interpolation = HDMI_CSC_CFG_INTMODE_DISABLE;
	int decimation = 0;

	/* YCC422 interpolation to 444 mode */
	if (is_color_space_interpolation(hdmi))
		interpolation = HDMI_CSC_CFG_INTMODE_CHROMA_INT_FORMULA1;
	else if (is_color_space_decimation(hdmi))
		decimation = HDMI_CSC_CFG_DECMODE_CHROMA_INT_FORMULA3;

	switch (hdmi_bus_fmt_color_depth(hdmi->hdmi_data.enc_out_bus_format)) {
	case 8:
		color_depth = HDMI_CSC_SCALE_CSC_COLORDE_PTH_24BPP;
		break;
	case 10:
		color_depth = HDMI_CSC_SCALE_CSC_COLORDE_PTH_30BPP;
		break;
	case 12:
		color_depth = HDMI_CSC_SCALE_CSC_COLORDE_PTH_36BPP;
		break;
	case 16:
		color_depth = HDMI_CSC_SCALE_CSC_COLORDE_PTH_48BPP;
		break;

	default:
		return;
	}

	/* Configure the CSC registers */
	hdmi_writeb(hdmi, interpolation | decimation, HDMI_CSC_CFG);
	hdmi_modb(hdmi, color_depth, HDMI_CSC_SCALE_CSC_COLORDE_PTH_MASK,
		  HDMI_CSC_SCALE);

	dw_hdmi_update_csc_coeffs(hdmi);
}

static void dw_hdmi_enable_video_path(struct dw_hdmi *hdmi)
{
	u8 clkdis;

	/* control period minimum duration */
	hdmi_writeb(hdmi, 12, HDMI_FC_CTRLDUR);
	hdmi_writeb(hdmi, 32, HDMI_FC_EXCTRLDUR);
	hdmi_writeb(hdmi, 1, HDMI_FC_EXCTRLSPAC);

	/* Set to fill TMDS data channels */
	hdmi_writeb(hdmi, 0x0B, HDMI_FC_CH0PREAM);
	hdmi_writeb(hdmi, 0x16, HDMI_FC_CH1PREAM);
	hdmi_writeb(hdmi, 0x21, HDMI_FC_CH2PREAM);

	/* Enable pixel clock and tmds data path */
	clkdis = 0x7F;
	clkdis &= ~HDMI_MC_CLKDIS_PIXELCLK_DISABLE;
	hdmi_writeb(hdmi, clkdis, HDMI_MC_CLKDIS);

	clkdis &= ~HDMI_MC_CLKDIS_TMDSCLK_DISABLE;
	hdmi_writeb(hdmi, clkdis, HDMI_MC_CLKDIS);

	/* Enable csc path */
	if (is_color_space_conversion(hdmi)) {
		clkdis &= ~HDMI_MC_CLKDIS_CSCCLK_DISABLE;
		hdmi_writeb(hdmi, clkdis, HDMI_MC_CLKDIS);
	}

	/* Enable pixel repetition path */
	if (hdmi->hdmi_data.video_mode.mpixelrepetitioninput) {
		clkdis &= ~HDMI_MC_CLKDIS_PREPCLK_DISABLE;
		hdmi_writeb(hdmi, clkdis, HDMI_MC_CLKDIS);
	}

	/* Enable color space conversion if needed */
	if (is_color_space_conversion(hdmi))
		hdmi_writeb(hdmi, HDMI_MC_FLOWCTRL_FEED_THROUGH_OFF_CSC_IN_PATH,
			    HDMI_MC_FLOWCTRL);
	else
		hdmi_writeb(hdmi, HDMI_MC_FLOWCTRL_FEED_THROUGH_OFF_CSC_BYPASS,
			    HDMI_MC_FLOWCTRL);
}

static void dw_hdmi_clear_overflow(struct dw_hdmi *hdmi)
{
	unsigned int count;
	unsigned int i;
	u8 val;

	/*
	 * Under some circumstances the Frame Composer arithmetic unit can miss
	 * an FC register write due to being busy processing the previous one.
	 * The issue can be worked around by issuing a TMDS software reset and
	 * then write one of the FC registers several times.
	 *
	 * The number of iterations matters and depends on the HDMI TX revision
	 * (and possibly on the platform). So far only i.MX6Q (v1.30a) and
	 * i.MX6DL (v1.31a) have been identified as needing the workaround, with
	 * 4 and 1 iterations respectively.
	 */

	switch (hdmi->version) {
	case 0x130a:
		count = 4;
		break;
	case 0x131a:
	case 0x200a:
	case 0x201a:
	case 0x211a:
		count = 1;
		break;
	default:
		return;
	}

	/* TMDS software reset */
	hdmi_writeb(hdmi, (u8)~HDMI_MC_SWRSTZ_TMDSSWRST_REQ, HDMI_MC_SWRSTZ);

	val = hdmi_readb(hdmi, HDMI_FC_INVIDCONF);
	for (i = 0; i < count; i++)
		hdmi_writeb(hdmi, val, HDMI_FC_INVIDCONF);
}

static void hdmi_disable_overflow_interrupts(struct dw_hdmi *hdmi)
{
	hdmi_writeb(hdmi, HDMI_IH_MUTE_FC_STAT2_OVERFLOW_MASK,
		    HDMI_IH_MUTE_FC_STAT2);
}

static void hdmi_video_packetize(struct dw_hdmi *hdmi)
{
	unsigned int color_depth = 0;
	unsigned int remap_size = HDMI_VP_REMAP_YCC422_16bit;
	unsigned int output_select = HDMI_VP_CONF_OUTPUT_SELECTOR_PP;
	struct hdmi_data_info *hdmi_data = &hdmi->hdmi_data;
	u8 val, vp_conf;

	if (hdmi_bus_fmt_is_rgb(hdmi->hdmi_data.enc_out_bus_format) ||
	    hdmi_bus_fmt_is_yuv444(hdmi->hdmi_data.enc_out_bus_format) ||
	    hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format)) {
		switch (hdmi_bus_fmt_color_depth(
					hdmi->hdmi_data.enc_out_bus_format)) {
		case 8:
			color_depth = 0;
			output_select = HDMI_VP_CONF_OUTPUT_SELECTOR_BYPASS;
			break;
		case 10:
			color_depth = 5;
			break;
		case 12:
			color_depth = 6;
			break;
		case 16:
			color_depth = 7;
			break;
		default:
			output_select = HDMI_VP_CONF_OUTPUT_SELECTOR_BYPASS;
		}
	} else if (hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_out_bus_format)) {
		switch (hdmi_bus_fmt_color_depth(
					hdmi->hdmi_data.enc_out_bus_format)) {
		case 0:
		case 8:
			remap_size = HDMI_VP_REMAP_YCC422_16bit;
			break;
		case 10:
			remap_size = HDMI_VP_REMAP_YCC422_20bit;
			break;
		case 12:
			remap_size = HDMI_VP_REMAP_YCC422_24bit;
			break;

		default:
			return;
		}
		output_select = HDMI_VP_CONF_OUTPUT_SELECTOR_YCC422;
	} else {
		return;
	}

	/* set the packetizer registers */
	val = (color_depth << HDMI_VP_PR_CD_COLOR_DEPTH_OFFSET) &
	      HDMI_VP_PR_CD_COLOR_DEPTH_MASK;
	hdmi_writeb(hdmi, val, HDMI_VP_PR_CD);

	hdmi_modb(hdmi, HDMI_VP_STUFF_PR_STUFFING_STUFFING_MODE,
		  HDMI_VP_STUFF_PR_STUFFING_MASK, HDMI_VP_STUFF);

	/* Data from pixel repeater block */
	if (hdmi_data->pix_repet_factor > 0) {
		vp_conf = HDMI_VP_CONF_PR_EN_ENABLE |
			  HDMI_VP_CONF_BYPASS_SELECT_PIX_REPEATER;
	} else { /* data from packetizer block */
		vp_conf = HDMI_VP_CONF_PR_EN_DISABLE |
			  HDMI_VP_CONF_BYPASS_SELECT_VID_PACKETIZER;
	}

	hdmi_modb(hdmi, vp_conf,
		  HDMI_VP_CONF_PR_EN_MASK |
		  HDMI_VP_CONF_BYPASS_SELECT_MASK, HDMI_VP_CONF);

	if ((color_depth == 5 && hdmi->previous_mode.htotal % 4) ||
	    (color_depth == 6 && hdmi->previous_mode.htotal % 2))
		hdmi_modb(hdmi, 0, HDMI_VP_STUFF_IDEFAULT_PHASE_MASK,
			  HDMI_VP_STUFF);
	else
		hdmi_modb(hdmi, 1 << HDMI_VP_STUFF_IDEFAULT_PHASE_OFFSET,
			  HDMI_VP_STUFF_IDEFAULT_PHASE_MASK, HDMI_VP_STUFF);

	hdmi_writeb(hdmi, remap_size, HDMI_VP_REMAP);

	if (output_select == HDMI_VP_CONF_OUTPUT_SELECTOR_PP) {
		vp_conf = HDMI_VP_CONF_BYPASS_EN_DISABLE |
			  HDMI_VP_CONF_PP_EN_ENABLE |
			  HDMI_VP_CONF_YCC422_EN_DISABLE;
	} else if (output_select == HDMI_VP_CONF_OUTPUT_SELECTOR_YCC422) {
		vp_conf = HDMI_VP_CONF_BYPASS_EN_DISABLE |
			  HDMI_VP_CONF_PP_EN_DISABLE |
			  HDMI_VP_CONF_YCC422_EN_ENABLE;
	} else if (output_select == HDMI_VP_CONF_OUTPUT_SELECTOR_BYPASS) {
		vp_conf = HDMI_VP_CONF_BYPASS_EN_ENABLE |
			  HDMI_VP_CONF_PP_EN_DISABLE |
			  HDMI_VP_CONF_YCC422_EN_DISABLE;
	} else {
		return;
	}

	hdmi_modb(hdmi, vp_conf,
		  HDMI_VP_CONF_BYPASS_EN_MASK | HDMI_VP_CONF_PP_EN_ENMASK |
		  HDMI_VP_CONF_YCC422_EN_MASK, HDMI_VP_CONF);

	hdmi_modb(hdmi, HDMI_VP_STUFF_PP_STUFFING_STUFFING_MODE |
			HDMI_VP_STUFF_YCC422_STUFFING_STUFFING_MODE,
		  HDMI_VP_STUFF_PP_STUFFING_MASK |
		  HDMI_VP_STUFF_YCC422_STUFFING_MASK, HDMI_VP_STUFF);

	hdmi_modb(hdmi, output_select, HDMI_VP_CONF_OUTPUT_SELECTOR_MASK,
		  HDMI_VP_CONF);
}

static void hdmi_video_sample(struct dw_hdmi *hdmi)
{
	int color_format = 0;
	u8 val;

	switch (hdmi->hdmi_data.enc_in_bus_format) {
	case MEDIA_BUS_FMT_RGB888_1X24:
		color_format = 0x01;
		break;
	case MEDIA_BUS_FMT_RGB101010_1X30:
		color_format = 0x03;
		break;
	case MEDIA_BUS_FMT_RGB121212_1X36:
		color_format = 0x05;
		break;
	case MEDIA_BUS_FMT_RGB161616_1X48:
		color_format = 0x07;
		break;

	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
		color_format = 0x09;
		break;
	case MEDIA_BUS_FMT_YUV10_1X30:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
		color_format = 0x0B;
		break;
	case MEDIA_BUS_FMT_YUV12_1X36:
	case MEDIA_BUS_FMT_UYYVYY12_0_5X36:
		color_format = 0x0D;
		break;
	case MEDIA_BUS_FMT_YUV16_1X48:
	case MEDIA_BUS_FMT_UYYVYY16_0_5X48:
		color_format = 0x0F;
		break;

	case MEDIA_BUS_FMT_UYVY8_1X16:
		color_format = 0x16;
		break;
	case MEDIA_BUS_FMT_UYVY10_1X20:
		color_format = 0x14;
		break;
	case MEDIA_BUS_FMT_UYVY12_1X24:
		color_format = 0x12;
		break;

	default:
		return;
	}

	val = HDMI_TX_INVID0_INTERNAL_DE_GENERATOR_DISABLE |
		((color_format << HDMI_TX_INVID0_VIDEO_MAPPING_OFFSET) &
		HDMI_TX_INVID0_VIDEO_MAPPING_MASK);
	hdmi_writeb(hdmi, val, HDMI_TX_INVID0);

	/* Enable TX stuffing: When DE is inactive, fix the output data to 0 */
	val = HDMI_TX_INSTUFFING_BDBDATA_STUFFING_ENABLE |
		HDMI_TX_INSTUFFING_RCRDATA_STUFFING_ENABLE |
		HDMI_TX_INSTUFFING_GYDATA_STUFFING_ENABLE;
	hdmi_writeb(hdmi, val, HDMI_TX_INSTUFFING);
	hdmi_writeb(hdmi, 0x0, HDMI_TX_GYDATA0);
	hdmi_writeb(hdmi, 0x0, HDMI_TX_GYDATA1);
	hdmi_writeb(hdmi, 0x0, HDMI_TX_RCRDATA0);
	hdmi_writeb(hdmi, 0x0, HDMI_TX_RCRDATA1);
	hdmi_writeb(hdmi, 0x0, HDMI_TX_BCBDATA0);
	hdmi_writeb(hdmi, 0x0, HDMI_TX_BCBDATA1);
}

static void dw_hdmi_disable(struct rockchip_connector *conn, struct dw_hdmi *hdmi,
			    struct display_state *state)
{
	if (hdmi->phy.enabled) {
		hdmi->phy.ops->disable(conn, hdmi, state);
		hdmi->phy.enabled = false;
	}
}

static void hdmi_config_AVI(struct dw_hdmi *hdmi, struct drm_display_mode *mode)
{
	struct hdmi_avi_infoframe frame;
	u8 val;
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

	/* Set up colorimetry */
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
	default: /* Carries no data */
		frame.colorimetry = HDMI_COLORIMETRY_ITU_601;
		frame.extended_colorimetry =
				HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;
		break;
	}

	frame.scan_mode = HDMI_SCAN_MODE_NONE;

	/*
	 * The Designware IP uses a different byte format from standard
	 * AVI info frames, though generally the bits are in the correct
	 * bytes.
	 */

	/*
	 * AVI data byte 1 differences: Colorspace in bits 0,1,7 rather than
	 * 5,6,7, active aspect present in bit 6 rather than 4.
	 */
	val = (frame.scan_mode & 3) << 4 | (frame.colorspace & 0x3);
	if (frame.active_aspect & 15)
		val |= HDMI_FC_AVICONF0_ACTIVE_FMT_INFO_PRESENT;
	if (frame.top_bar || frame.bottom_bar)
		val |= HDMI_FC_AVICONF0_BAR_DATA_HORIZ_BAR;
	if (frame.left_bar || frame.right_bar)
		val |= HDMI_FC_AVICONF0_BAR_DATA_VERT_BAR;
	hdmi_writeb(hdmi, val, HDMI_FC_AVICONF0);

	/* AVI data byte 2 differences: none */
	val = ((frame.colorimetry & 0x3) << 6) |
	      ((frame.picture_aspect & 0x3) << 4) |
	      (frame.active_aspect & 0xf);
	hdmi_writeb(hdmi, val, HDMI_FC_AVICONF1);

	/* AVI data byte 3 differences: none */
	val = ((frame.extended_colorimetry & 0x7) << 4) |
	      ((frame.quantization_range & 0x3) << 2) |
	      (frame.nups & 0x3);
	if (frame.itc)
		val |= HDMI_FC_AVICONF2_IT_CONTENT_VALID;
	hdmi_writeb(hdmi, val, HDMI_FC_AVICONF2);

	/* AVI data byte 4 differences: none */
	val = frame.video_code & 0x7f;
	hdmi_writeb(hdmi, val, HDMI_FC_AVIVID);

	/* AVI Data Byte 5- set up input and output pixel repetition */
	val = (((hdmi->hdmi_data.video_mode.mpixelrepetitioninput + 1) <<
		HDMI_FC_PRCONF_INCOMING_PR_FACTOR_OFFSET) &
		HDMI_FC_PRCONF_INCOMING_PR_FACTOR_MASK) |
		((hdmi->hdmi_data.video_mode.mpixelrepetitionoutput <<
		HDMI_FC_PRCONF_OUTPUT_PR_FACTOR_OFFSET) &
		HDMI_FC_PRCONF_OUTPUT_PR_FACTOR_MASK);
	hdmi_writeb(hdmi, val, HDMI_FC_PRCONF);

	/*
	 * AVI data byte 5 differences: content type in 0,1 rather than 4,5,
	 * ycc range in bits 2,3 rather than 6,7
	 */
	val = ((frame.ycc_quantization_range & 0x3) << 2) |
	      (frame.content_type & 0x3);
	hdmi_writeb(hdmi, val, HDMI_FC_AVICONF3);

	/* AVI Data Bytes 6-13 */
	hdmi_writeb(hdmi, frame.top_bar & 0xff, HDMI_FC_AVIETB0);
	hdmi_writeb(hdmi, (frame.top_bar >> 8) & 0xff, HDMI_FC_AVIETB1);
	hdmi_writeb(hdmi, frame.bottom_bar & 0xff, HDMI_FC_AVISBB0);
	hdmi_writeb(hdmi, (frame.bottom_bar >> 8) & 0xff, HDMI_FC_AVISBB1);
	hdmi_writeb(hdmi, frame.left_bar & 0xff, HDMI_FC_AVIELB0);
	hdmi_writeb(hdmi, (frame.left_bar >> 8) & 0xff, HDMI_FC_AVIELB1);
	hdmi_writeb(hdmi, frame.right_bar & 0xff, HDMI_FC_AVISRB0);
	hdmi_writeb(hdmi, (frame.right_bar >> 8) & 0xff, HDMI_FC_AVISRB1);
}

static void hdmi_config_vendor_specific_infoframe(struct dw_hdmi *hdmi,
						  struct drm_display_mode *mode)
{
	struct hdmi_vendor_infoframe frame;
	u8 buffer[10];
	ssize_t err;

	/* Disable HDMI vendor specific infoframe send */
	hdmi_mask_writeb(hdmi, 0, HDMI_FC_DATAUTO0, HDMI_FC_DATAUTO0_VSD_OFFSET,
			 HDMI_FC_DATAUTO0_VSD_MASK);

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
		printf("Failed to pack vendor infoframe: %zd\n", err);
		return;
	}

	/* Set the length of HDMI vendor specific InfoFrame payload */
	hdmi_writeb(hdmi, buffer[2], HDMI_FC_VSDSIZE);

	/* Set 24bit IEEE Registration Identifier */
	hdmi_writeb(hdmi, buffer[4], HDMI_FC_VSDIEEEID0);
	hdmi_writeb(hdmi, buffer[5], HDMI_FC_VSDIEEEID1);
	hdmi_writeb(hdmi, buffer[6], HDMI_FC_VSDIEEEID2);

	/* Set HDMI_Video_Format and HDMI_VIC/3D_Structure */
	hdmi_writeb(hdmi, buffer[7], HDMI_FC_VSDPAYLOAD0);
	hdmi_writeb(hdmi, buffer[8], HDMI_FC_VSDPAYLOAD1);

	if (frame.s3d_struct >= HDMI_3D_STRUCTURE_SIDE_BY_SIDE_HALF)
		hdmi_writeb(hdmi, buffer[9], HDMI_FC_VSDPAYLOAD2);

	/* Packet frame interpolation */
	hdmi_writeb(hdmi, 1, HDMI_FC_DATAUTO1);

	/* Auto packets per frame and line spacing */
	hdmi_writeb(hdmi, 0x11, HDMI_FC_DATAUTO2);

	/* Configures the Frame Composer On RDRB mode */
	hdmi_mask_writeb(hdmi, 1, HDMI_FC_DATAUTO0, HDMI_FC_DATAUTO0_VSD_OFFSET,
			 HDMI_FC_DATAUTO0_VSD_MASK);
}

static void hdmi_set_cts_n(struct dw_hdmi *hdmi, unsigned int cts,
			   unsigned int n)
{
	/* Must be set/cleared first */
	hdmi_modb(hdmi, 0, HDMI_AUD_CTS3_CTS_MANUAL, HDMI_AUD_CTS3);

	/* nshift factor = 0 */
	hdmi_modb(hdmi, 0, HDMI_AUD_CTS3_N_SHIFT_MASK, HDMI_AUD_CTS3);

	hdmi_writeb(hdmi, ((cts >> 16) & HDMI_AUD_CTS3_AUDCTS19_16_MASK) |
		    HDMI_AUD_CTS3_CTS_MANUAL, HDMI_AUD_CTS3);
	hdmi_writeb(hdmi, (cts >> 8) & 0xff, HDMI_AUD_CTS2);
	hdmi_writeb(hdmi, cts & 0xff, HDMI_AUD_CTS1);

	hdmi_writeb(hdmi, (n >> 16) & 0x0f, HDMI_AUD_N3);
	hdmi_writeb(hdmi, (n >> 8) & 0xff, HDMI_AUD_N2);
	hdmi_writeb(hdmi, n & 0xff, HDMI_AUD_N1);
}

static int hdmi_match_tmds_n_table(struct dw_hdmi *hdmi,
				   unsigned long pixel_clk,
				   unsigned long freq)
{
	const struct dw_hdmi_plat_data *plat_data = hdmi->plat_data;
	const struct dw_hdmi_audio_tmds_n *tmds_n = NULL;
	int i;

	if (plat_data->tmds_n_table) {
		for (i = 0; plat_data->tmds_n_table[i].tmds != 0; i++) {
			if (pixel_clk == plat_data->tmds_n_table[i].tmds) {
				tmds_n = &plat_data->tmds_n_table[i];
				break;
			}
		}
	}

	if (!tmds_n) {
		for (i = 0; common_tmds_n_table[i].tmds != 0; i++) {
			if (pixel_clk == common_tmds_n_table[i].tmds) {
				tmds_n = &common_tmds_n_table[i];
				break;
			}
		}
	}

	if (!tmds_n)
		return -ENOENT;

	switch (freq) {
	case 32000:
		return tmds_n->n_32k;
	case 44100:
	case 88200:
	case 176400:
		return (freq / 44100) * tmds_n->n_44k1;
	case 48000:
	case 96000:
	case 192000:
		return (freq / 48000) * tmds_n->n_48k;
	default:
		return -ENOENT;
	}
}

static u64 hdmi_audio_math_diff(unsigned int freq, unsigned int n,
				unsigned int pixel_clk)
{
	u64 final, diff;
	u64 cts;

	final = (u64)pixel_clk * n;

	cts = final;
	do_div(cts, 128 * freq);

	diff = final - (u64)cts * (128 * freq);

	return diff;
}

static unsigned int hdmi_compute_n(struct dw_hdmi *hdmi,
				   unsigned long pixel_clk,
				   unsigned long freq)
{
	unsigned int min_n = DIV_ROUND_UP((128 * freq), 1500);
	unsigned int max_n = (128 * freq) / 300;
	unsigned int ideal_n = (128 * freq) / 1000;
	unsigned int best_n_distance = ideal_n;
	unsigned int best_n = 0;
	u64 best_diff = U64_MAX;
	int n;

	/* If the ideal N could satisfy the audio math, then just take it */
	if (hdmi_audio_math_diff(freq, ideal_n, pixel_clk) == 0)
		return ideal_n;

	for (n = min_n; n <= max_n; n++) {
		u64 diff = hdmi_audio_math_diff(freq, n, pixel_clk);

		if (diff < best_diff || (diff == best_diff &&
					 abs(n - ideal_n) < best_n_distance)) {
			best_n = n;
			best_diff = diff;
			best_n_distance = abs(best_n - ideal_n);
		}

		/*
		 * The best N already satisfy the audio math, and also be
		 * the closest value to ideal N, so just cut the loop.
		 */
		if ((best_diff == 0) && (abs(n - ideal_n) > best_n_distance))
			break;
	}

	return best_n;
}

static unsigned int hdmi_find_n(struct dw_hdmi *hdmi, unsigned long pixel_clk,
				unsigned long sample_rate)
{
	int n;

	n = hdmi_match_tmds_n_table(hdmi, pixel_clk, sample_rate);
	if (n > 0)
		return n;

	printf("Rate %lu missing; compute N dynamically\n",
	       pixel_clk);

	return hdmi_compute_n(hdmi, pixel_clk, sample_rate);
}

static
void hdmi_set_clk_regenerator(struct dw_hdmi *hdmi, unsigned long pixel_clk,
			      unsigned int sample_rate)
{
	unsigned long ftdms = pixel_clk;
	unsigned int n, cts;
	u64 tmp;

	n = hdmi_find_n(hdmi, pixel_clk, sample_rate);

	/*
	 * Compute the CTS value from the N value.  Note that CTS and N
	 * can be up to 20 bits in total, so we need 64-bit math.  Also
	 * note that our TDMS clock is not fully accurate; it is accurate
	 * to kHz.  This can introduce an unnecessary remainder in the
	 * calculation below, so we don't try to warn about that.
	 */
	tmp = (u64)ftdms * n;
	do_div(tmp, 128 * sample_rate);
	cts = tmp;

	printf("%s: fs=%uHz ftdms=%lu.%03luMHz N=%d cts=%d\n", __func__,
	       sample_rate, ftdms / 1000000, (ftdms / 1000) % 1000, n, cts);

	hdmi->audio_n = n;
	hdmi->audio_cts = cts;
	hdmi_set_cts_n(hdmi, cts, hdmi->audio_enable ? n : 0);
}

static void hdmi_clk_regenerator_update_pixel_clock(struct dw_hdmi *hdmi)
{
	hdmi_set_clk_regenerator(hdmi, hdmi->hdmi_data.video_mode.mtmdsclock,
				 hdmi->sample_rate);
}

static void hdmi_enable_audio_clk(struct dw_hdmi *hdmi)
{
	hdmi_modb(hdmi, 0, HDMI_MC_CLKDIS_AUDCLK_DISABLE, HDMI_MC_CLKDIS);
}

void dw_hdmi_set_sample_rate(struct dw_hdmi *hdmi, unsigned int rate)
{
	hdmi->sample_rate = rate;
	hdmi_set_clk_regenerator(hdmi, hdmi->hdmi_data.video_mode.mtmdsclock,
				 hdmi->sample_rate);
}

static int dw_hdmi_hdcp_load_key(struct dw_hdmi *hdmi)
{
	int i, j, ret, val;
	struct hdcp_keys *hdcp_keys;

	val = sizeof(*hdcp_keys);
	hdcp_keys = malloc(val);
	if (!hdcp_keys)
		return -ENOMEM;

	memset(hdcp_keys, 0, val);

	ret = vendor_storage_read(HDMI_HDCP1X_ID, hdcp_keys, val);
	if (ret < val) {
		printf("HDCP: read size %d\n", ret);
		free(hdcp_keys);
		return -EINVAL;
	}

	if (hdcp_keys->KSV[0] == 0x00 &&
	    hdcp_keys->KSV[1] == 0x00 &&
	    hdcp_keys->KSV[2] == 0x00 &&
	    hdcp_keys->KSV[3] == 0x00 &&
	    hdcp_keys->KSV[4] == 0x00) {
		printf("HDCP: Invalid hdcp key\n");
		free(hdcp_keys);
		return -EINVAL;
	}

	/* Disable decryption logic */
	hdmi_writeb(hdmi, 0, HDMI_HDCPREG_RMCTL);
	/* Poll untile DPK write is allowed */
	do {
		val = hdmi_readb(hdmi, HDMI_HDCPREG_RMSTS);
	} while ((val & DPK_WR_OK_STS) == 0);

	hdmi_writeb(hdmi, 0, HDMI_HDCPREG_DPK6);
	hdmi_writeb(hdmi, 0, HDMI_HDCPREG_DPK5);

	/* The useful data in ksv should be 5 byte */
	for (i = 4; i >= 0; i--)
		hdmi_writeb(hdmi, hdcp_keys->KSV[i], HDMI_HDCPREG_DPK0 + i);
	/* Poll untile DPK write is allowed */
	do {
		val = hdmi_readb(hdmi, HDMI_HDCPREG_RMSTS);
	} while ((val & DPK_WR_OK_STS) == 0);

	/* Enable decryption logic */
	hdmi_writeb(hdmi, 1, HDMI_HDCPREG_RMCTL);
	hdmi_writeb(hdmi, hdcp_keys->seeds[0], HDMI_HDCPREG_SEED1);
	hdmi_writeb(hdmi, hdcp_keys->seeds[1], HDMI_HDCPREG_SEED0);

	/* Write encrypt device private key */
	for (i = 0; i < DW_HDMI_HDCP_DPK_LEN - 6; i += 7) {
		for (j = 6; j >= 0; j--)
			hdmi_writeb(hdmi, hdcp_keys->devicekey[i + j],
				    HDMI_HDCPREG_DPK0 + j);
		do {
			val = hdmi_readb(hdmi, HDMI_HDCPREG_RMSTS);
		} while ((val & DPK_WR_OK_STS) == 0);
	}

	free(hdcp_keys);
	return 0;
}

static void hdmi_tx_hdcp_config(struct dw_hdmi *hdmi,
				const struct drm_display_mode *mode)
{
	u8 vsync_pol, hsync_pol, data_pol, hdmi_dvi;

	if (!hdmi->hdcp1x_enable)
		return;

	/* Configure the video polarity */
	vsync_pol = mode->flags & DRM_MODE_FLAG_PVSYNC ?
		    HDMI_A_VIDPOLCFG_VSYNCPOL_ACTIVE_HIGH :
		    HDMI_A_VIDPOLCFG_VSYNCPOL_ACTIVE_LOW;
	hsync_pol = mode->flags & DRM_MODE_FLAG_PHSYNC ?
		    HDMI_A_VIDPOLCFG_HSYNCPOL_ACTIVE_HIGH :
		    HDMI_A_VIDPOLCFG_HSYNCPOL_ACTIVE_LOW;
	data_pol = HDMI_A_VIDPOLCFG_DATAENPOL_ACTIVE_HIGH;
	hdmi_modb(hdmi, vsync_pol | hsync_pol | data_pol,
		  HDMI_A_VIDPOLCFG_VSYNCPOL_MASK |
		  HDMI_A_VIDPOLCFG_HSYNCPOL_MASK |
		  HDMI_A_VIDPOLCFG_DATAENPOL_MASK,
		  HDMI_A_VIDPOLCFG);

	/* Config the display mode */
	hdmi_dvi = hdmi->sink_is_hdmi ? HDMI_A_HDCPCFG0_HDMIDVI_HDMI :
		   HDMI_A_HDCPCFG0_HDMIDVI_DVI;
	hdmi_modb(hdmi, hdmi_dvi, HDMI_A_HDCPCFG0_HDMIDVI_MASK,
		  HDMI_A_HDCPCFG0);

	if (!(hdmi_readb(hdmi, HDMI_HDCPREG_RMSTS) & 0x3f))
		dw_hdmi_hdcp_load_key(hdmi);

	hdmi_modb(hdmi, HDMI_FC_INVIDCONF_HDCP_KEEPOUT_ACTIVE,
		  HDMI_FC_INVIDCONF_HDCP_KEEPOUT_MASK,
		  HDMI_FC_INVIDCONF);

	if (hdmi_readb(hdmi, HDMI_CONFIG1_ID) & HDMI_A_HDCP22_MASK) {
		hdmi_modb(hdmi, HDMI_HDCP2_OVR_ENABLE |
			  HDMI_HDCP2_FORCE_DISABLE,
			  HDMI_HDCP2_OVR_EN_MASK |
			  HDMI_HDCP2_FORCE_MASK,
			  HDMI_HDCP2REG_CTRL);
		hdmi_writeb(hdmi, 0xff, HDMI_HDCP2REG_MASK);
		hdmi_writeb(hdmi, 0xff, HDMI_HDCP2REG_MUTE);
	}

	hdmi_writeb(hdmi, 0x40, HDMI_A_OESSWCFG);
		    hdmi_modb(hdmi, HDMI_A_HDCPCFG0_BYPENCRYPTION_DISABLE |
		    HDMI_A_HDCPCFG0_EN11FEATURE_DISABLE |
		    HDMI_A_HDCPCFG0_SYNCRICHECK_ENABLE,
		    HDMI_A_HDCPCFG0_BYPENCRYPTION_MASK |
		    HDMI_A_HDCPCFG0_EN11FEATURE_MASK |
		    HDMI_A_HDCPCFG0_SYNCRICHECK_MASK, HDMI_A_HDCPCFG0);

	hdmi_modb(hdmi, HDMI_A_HDCPCFG1_ENCRYPTIONDISABLE_ENABLE |
		  HDMI_A_HDCPCFG1_PH2UPSHFTENC_ENABLE,
		  HDMI_A_HDCPCFG1_ENCRYPTIONDISABLE_MASK |
		  HDMI_A_HDCPCFG1_PH2UPSHFTENC_MASK, HDMI_A_HDCPCFG1);

	/* Reset HDCP Engine */
	if (hdmi_readb(hdmi, HDMI_MC_CLKDIS) & HDMI_MC_CLKDIS_HDCPCLK_MASK) {
		hdmi_modb(hdmi, HDMI_A_HDCPCFG1_SWRESET_ASSERT,
			  HDMI_A_HDCPCFG1_SWRESET_MASK, HDMI_A_HDCPCFG1);
	}

	hdmi_writeb(hdmi, 0x00, HDMI_A_APIINTMSK);
	hdmi_modb(hdmi, HDMI_A_HDCPCFG0_RXDETECT_ENABLE,
		  HDMI_A_HDCPCFG0_RXDETECT_MASK, HDMI_A_HDCPCFG0);

	hdmi_modb(hdmi, HDMI_MC_CLKDIS_HDCPCLK_ENABLE,
		  HDMI_MC_CLKDIS_HDCPCLK_MASK, HDMI_MC_CLKDIS);

	printf("%s success\n", __func__);
}

static int dw_hdmi_setup(struct dw_hdmi *hdmi,
			 struct rockchip_connector *conn,
			 struct drm_display_mode *mode,
			 struct display_state *state)
{
	int ret;
	void *data = hdmi->plat_data->phy_data;

	hdmi_disable_overflow_interrupts(hdmi);
	if (!hdmi->vic)
		printf("Non-CEA mode used in HDMI\n");
	else
		printf("CEA mode used vic=%d\n", hdmi->vic);

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

	/* HDMI Initialization Step B.1 */
	hdmi_av_composer(hdmi, mode);

	/* HDMI Initialization Step B.2 */
	ret = hdmi->phy.ops->init(conn, hdmi, state);
	if (ret)
		return ret;
	hdmi->phy.enabled = true;

	/* HDMI Initializateion Step B.3 */
	dw_hdmi_enable_video_path(hdmi);

	/* HDMI Initialization Step E - Configure audio */
	if (hdmi->sink_has_audio) {
		printf("sink has audio support\n");
		hdmi_clk_regenerator_update_pixel_clock(hdmi);
		hdmi_enable_audio_clk(hdmi);
	}

	/* not for DVI mode */
	if (hdmi->sink_is_hdmi) {
		/* HDMI Initialization Step F - Configure AVI InfoFrame */
		hdmi_config_AVI(hdmi, mode);
		hdmi_config_vendor_specific_infoframe(hdmi, mode);
		hdmi_modb(hdmi, HDMI_A_HDCPCFG0_HDMIDVI_HDMI,
			  HDMI_A_HDCPCFG0_HDMIDVI_MASK,
			  HDMI_A_HDCPCFG0);
	} else {
		hdmi_modb(hdmi, HDMI_A_HDCPCFG0_HDMIDVI_DVI,
			  HDMI_A_HDCPCFG0_HDMIDVI_MASK,
			  HDMI_A_HDCPCFG0);
		printf("%s DVI mode\n", __func__);
	}

	hdmi_video_packetize(hdmi);
	hdmi_video_csc(hdmi);
	hdmi_video_sample(hdmi);
	hdmi_tx_hdcp_config(hdmi, mode);
	dw_hdmi_clear_overflow(hdmi);

	return 0;
}

int dw_hdmi_detect_hotplug(struct dw_hdmi *hdmi,
			   struct display_state *state)
{
	return hdmi->phy.ops->read_hpd(hdmi, state);
}

static int dw_hdmi_set_reg_wr(struct dw_hdmi *hdmi)
{
	switch (hdmi->io_width) {
	case 4:
		hdmi->write = dw_hdmi_writel;
		hdmi->read = dw_hdmi_readl;
		break;
	case 1:
		hdmi->write = dw_hdmi_writeb;
		hdmi->read = dw_hdmi_readb;
		break;
	default:
		printf("reg-io-width must be 1 or 4\n");
		return -EINVAL;
	}

	return 0;
}

static void initialize_hdmi_mutes(struct dw_hdmi *hdmi)
{
	/*mute unnecessary interrupt, only enable hpd */
	hdmi_writeb(hdmi, 0xff, HDMI_FC_MASK0);
	hdmi_writeb(hdmi, 0xff, HDMI_FC_MASK1);
	hdmi_writeb(hdmi, 0xff, HDMI_FC_MASK2);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_FC_STAT0);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_FC_STAT1);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_FC_STAT2);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_AS_STAT0);
	hdmi_writeb(hdmi, 0xfe, HDMI_IH_MUTE_PHY_STAT0);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_I2CM_STAT0);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_CEC_STAT0);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_VP_STAT0);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_I2CMPHY_STAT0);
	hdmi_writeb(hdmi, 0xff, HDMI_IH_MUTE_AHBDMAAUD_STAT0);
	hdmi_writeb(hdmi, 0xf1, HDMI_PHY_MASK0);

	/*Force output black*/
	dw_hdmi_writel(hdmi, 0x00, HDMI_FC_DBGTMDS2);
	dw_hdmi_writel(hdmi, 0x00, HDMI_FC_DBGTMDS1);
	dw_hdmi_writel(hdmi, 0x00, HDMI_FC_DBGTMDS0);
}

static void dw_hdmi_dev_init(struct dw_hdmi *hdmi)
{
	hdmi->version = (hdmi_readb(hdmi, HDMI_DESIGN_ID) << 8)
		      | (hdmi_readb(hdmi, HDMI_REVISION_ID) << 0);

	initialize_hdmi_mutes(hdmi);
}

static void dw_hdmi_i2c_set_divs(struct dw_hdmi *hdmi)
{
	unsigned long low_ns, high_ns;
	unsigned long div_low, div_high;

	/* Standard-mode */
	if (hdmi->i2c->scl_high_ns < 4000)
		high_ns = 4708;
	else
		high_ns = hdmi->i2c->scl_high_ns;

	if (hdmi->i2c->scl_low_ns < 4700)
		low_ns = 4916;
	else
		low_ns = hdmi->i2c->scl_low_ns;

	div_low = (24000 * low_ns) / 1000000;
	if ((24000 * low_ns) % 1000000)
		div_low++;

	div_high = (24000 * high_ns) / 1000000;
	if ((24000 * high_ns) % 1000000)
		div_high++;

	/* Maximum divider supported by hw is 0xffff */
	if (div_low > 0xffff)
		div_low = 0xffff;

	if (div_high > 0xffff)
		div_high = 0xffff;

	hdmi_writeb(hdmi, div_high & 0xff, HDMI_I2CM_SS_SCL_HCNT_0_ADDR);
	hdmi_writeb(hdmi, (div_high >> 8) & 0xff,
		    HDMI_I2CM_SS_SCL_HCNT_1_ADDR);
	hdmi_writeb(hdmi, div_low & 0xff, HDMI_I2CM_SS_SCL_LCNT_0_ADDR);
	hdmi_writeb(hdmi, (div_low >> 8) & 0xff,
		    HDMI_I2CM_SS_SCL_LCNT_1_ADDR);
}

static void dw_hdmi_i2c_init(struct dw_hdmi *hdmi)
{
	/* Software reset */
	hdmi_writeb(hdmi, 0x00, HDMI_I2CM_SOFTRSTZ);

	/* Set Standard Mode speed */
	hdmi_modb(hdmi, HDMI_I2CM_DIV_STD_MODE,
		  HDMI_I2CM_DIV_FAST_STD_MODE, HDMI_I2CM_DIV);

	/* Set done, not acknowledged and arbitration interrupt polarities */
	hdmi_writeb(hdmi, HDMI_I2CM_INT_DONE_POL, HDMI_I2CM_INT);
	hdmi_writeb(hdmi, HDMI_I2CM_CTLINT_NAC_POL | HDMI_I2CM_CTLINT_ARB_POL,
		    HDMI_I2CM_CTLINT);

	/* Clear DONE and ERROR interrupts */
	hdmi_writeb(hdmi, HDMI_IH_I2CM_STAT0_ERROR | HDMI_IH_I2CM_STAT0_DONE,
		    HDMI_IH_I2CM_STAT0);

	/* Mute DONE and ERROR interrupts */
	hdmi_writeb(hdmi, HDMI_IH_I2CM_STAT0_ERROR | HDMI_IH_I2CM_STAT0_DONE,
		    HDMI_IH_MUTE_I2CM_STAT0);

	/* set SDA high level holding time */
	hdmi_writeb(hdmi, 0x48, HDMI_I2CM_SDA_HOLD);

	dw_hdmi_i2c_set_divs(hdmi);
}

void dw_hdmi_audio_enable(struct dw_hdmi *hdmi)
{
	hdmi->audio_enable = true;
	hdmi_set_cts_n(hdmi, hdmi->audio_cts, hdmi->audio_n);
}

void dw_hdmi_audio_disable(struct dw_hdmi *hdmi)
{
	hdmi->audio_enable = false;
	hdmi_set_cts_n(hdmi, hdmi->audio_cts, 0);
}

int rockchip_dw_hdmi_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	const struct dw_hdmi_plat_data *pdata =
		(const struct dw_hdmi_plat_data *)dev_get_driver_data(conn->dev);
	struct crtc_state *crtc_state = &state->crtc_state;
	struct dw_hdmi *hdmi;
	struct drm_display_mode *mode_buf;
	ofnode hdmi_node = conn->dev->node;
	u32 val;
	struct device_node *ddc_node;
	int ret;

	hdmi = malloc(sizeof(struct dw_hdmi));
	if (!hdmi)
		return -ENOMEM;

	memset(hdmi, 0, sizeof(struct dw_hdmi));
	mode_buf = malloc(MODE_LEN * sizeof(struct drm_display_mode));
	if (!mode_buf)
		return -ENOMEM;
	hdmi->id = of_alias_get_id(ofnode_to_np(hdmi_node), "hdmi");
	if (hdmi->id < 0)
		hdmi->id = 0;
	conn_state->disp_info  = rockchip_get_disp_info(conn_state->type, hdmi->id);

	memset(mode_buf, 0, MODE_LEN * sizeof(struct drm_display_mode));

	hdmi->regs = dev_read_addr_ptr(conn->dev);
	hdmi->io_width = ofnode_read_s32_default(hdmi_node, "reg-io-width", -1);

	if (ofnode_read_bool(hdmi_node, "scramble-low-rates"))
		hdmi->scramble_low_rates = true;

	if (ofnode_read_bool(hdmi_node, "hdcp1x-enable"))
		hdmi->hdcp1x_enable = true;
	else
		hdmi->hdcp1x_enable = false;

	if (ofnode_read_bool(hdmi_node, "force_output_bus_format_RGB") ||
	    ofnode_read_bool(hdmi_node, "unsupported-yuv-input"))
		hdmi->output_bus_format_rgb = true;
	else
		hdmi->output_bus_format_rgb = false;

	ddc_node = of_parse_phandle(ofnode_to_np(hdmi_node), "ddc-i2c-bus", 0);
	if (ddc_node) {
		uclass_get_device_by_ofnode(UCLASS_I2C, np_to_ofnode(ddc_node),
					    &hdmi->adap.i2c_bus);
		if (hdmi->adap.i2c_bus)
			hdmi->adap.ops = i2c_get_ops(hdmi->adap.i2c_bus);
	}

	hdmi->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (hdmi->grf <= 0) {
		printf("%s: Get syscon grf failed (ret=%p)\n",
		       __func__, hdmi->grf);
		return -ENXIO;
	}

	hdmi->gpio_base = (void *)dev_read_addr_index(conn->dev, 1);
	if (!hdmi->gpio_base)
		return -ENODEV;

	ret = gpio_request_by_name(conn->dev, "hpd-gpios", 0,
				   &hdmi->hpd_gpiod, GPIOD_IS_IN);
        if (ret && ret != -ENOENT) {
                printf("%s: Cannot get HPD GPIO: %d\n", __func__, ret);
                return ret;
        }

	dw_hdmi_set_reg_wr(hdmi);

	if (pdata->grf_vop_sel_reg) {
		if (crtc_state->crtc_id)
			val = ((1 << pdata->vop_sel_bit) |
			       (1 << (16 + pdata->vop_sel_bit)));
		else
			val = ((0 << pdata->vop_sel_bit) |
			       (1 << (16 + pdata->vop_sel_bit)));
		writel(val, hdmi->grf + pdata->grf_vop_sel_reg);
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
	conn_state->output_if |= VOP_OUTPUT_IF_HDMI0;
	conn_state->output_mode = ROCKCHIP_OUT_MODE_AAAA;

	hdmi->dev_type = pdata->dev_type;
	hdmi->plat_data = pdata;
	hdmi->edid_data.mode_buf = mode_buf;
	hdmi->sample_rate = 48000;

	conn->data = hdmi;
	dw_hdmi_set_iomux(hdmi->grf, hdmi->gpio_base,
			  &hdmi->hpd_gpiod, hdmi->dev_type);
	dw_hdmi_detect_phy(hdmi);
	dw_hdmi_dev_init(hdmi);

	return 0;
}

void rockchip_dw_hdmi_deinit(struct rockchip_connector *conn, struct display_state *state)
{
	struct dw_hdmi *hdmi = conn->data;

	if (hdmi->i2c)
		free(hdmi->i2c);
	if (hdmi->edid_data.mode_buf)
		free(hdmi->edid_data.mode_buf);
	if (hdmi)
		free(hdmi);
}

int rockchip_dw_hdmi_prepare(struct rockchip_connector *conn, struct display_state *state)
{
	return 0;
}

int rockchip_dw_hdmi_enable(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct dw_hdmi *hdmi = conn->data;

	if (!hdmi)
		return -EFAULT;

	/* Store the display mode for plugin/DKMS poweron events */
	memcpy(&hdmi->previous_mode, mode, sizeof(hdmi->previous_mode));

	dw_hdmi_setup(hdmi, conn, mode, state);

	return 0;
}

int rockchip_dw_hdmi_disable(struct rockchip_connector *conn, struct display_state *state)
{
	struct dw_hdmi *hdmi = conn->data;

	dw_hdmi_disable(conn, hdmi, state);
	return 0;
}

int rockchip_dw_hdmi_get_timing(struct rockchip_connector *conn, struct display_state *state)
{
	int ret, i;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct dw_hdmi *hdmi = conn->data;
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
		hdmi->edid_data.mode_buf[0].type |= DRM_MODE_TYPE_PREFERRED;
		hdmi->edid_data.preferred_mode = &hdmi->edid_data.mode_buf[0];
		printf("failed to get edid\n");
	}
	drm_rk_filter_whitelist(&hdmi->edid_data);
	if (hdmi->phy.ops->mode_valid)
		hdmi->phy.ops->mode_valid(conn, hdmi, state);
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
	drm_rk_selete_output(&hdmi->edid_data, conn_state, &bus_format,
			     overscan, hdmi->dev_type, hdmi->output_bus_format_rgb);

	*mode = *hdmi->edid_data.preferred_mode;
	hdmi->vic = drm_match_cea_mode(mode);

	printf("mode:%dx%d\n", mode->hdisplay, mode->vdisplay);
	if (state->force_output)
		bus_format = state->force_bus_format;
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

int rockchip_dw_hdmi_detect(struct rockchip_connector *conn, struct display_state *state)
{
	int ret;
	struct dw_hdmi *hdmi = conn->data;

	if (!hdmi)
		return -EFAULT;

	ret = dw_hdmi_detect_hotplug(hdmi, state);

	return ret;
}

int rockchip_dw_hdmi_get_edid(struct rockchip_connector *conn, struct display_state *state)
{
	int ret;
	struct connector_state *conn_state = &state->conn_state;
	struct dw_hdmi *hdmi = conn->data;

	ret = drm_do_get_edid(&hdmi->adap, conn_state->edid);

	return ret;
}

int inno_dw_hdmi_phy_init(struct rockchip_connector *conn, struct dw_hdmi *hdmi, void *data)
{
	struct display_state *state = (struct display_state *)data;
	struct connector_state *conn_state = &state->conn_state;
	u32 color_depth, bus_width;

	color_depth =
		hdmi_bus_fmt_color_depth(hdmi->hdmi_data.enc_out_bus_format);

	if (hdmi_bus_fmt_is_yuv420(hdmi->hdmi_data.enc_out_bus_format))
		bus_width = color_depth / 2;
	else if (!hdmi_bus_fmt_is_yuv422(hdmi->hdmi_data.enc_out_bus_format))
		bus_width = color_depth;
	else
		bus_width = 8;
	rockchip_phy_set_bus_width(conn->phy, bus_width);
	rockchip_phy_set_pll(conn->phy,
			     conn_state->mode.crtc_clock * 1000);
	if (hdmi->edid_data.display_info.hdmi.scdc.supported)
		rockchip_dw_hdmi_scdc_set_tmds_rate(hdmi);
	rockchip_phy_power_on(conn->phy);

	return 0;
}

void inno_dw_hdmi_phy_disable(struct rockchip_connector *conn, struct dw_hdmi *hdmi, void *data)
{
}

enum drm_connector_status
inno_dw_hdmi_phy_read_hpd(struct dw_hdmi *hdmi, void *data)
{
	enum drm_connector_status status;
	struct display_state *state = (struct display_state *)data;

	status = dw_hdmi_phy_read_hpd(hdmi, state);

	if (hdmi->dev_type == RK3328_HDMI) {
		if (status == connector_status_connected)
			inno_dw_hdmi_set_domain(hdmi->grf, 1);
		else
			inno_dw_hdmi_set_domain(hdmi->grf, 0);
	}

	return status;
}

void inno_dw_hdmi_mode_valid(struct rockchip_connector *conn, struct dw_hdmi *hdmi, void *data)
{
	struct hdmi_edid_data *edid_data = &hdmi->edid_data;
	unsigned long rate;
	int i, ret;
	struct drm_display_mode *mode_buf = edid_data->mode_buf;

	for (i = 0; i < edid_data->modes; i++) {
		if (edid_data->mode_buf[i].invalid)
			continue;
		if (edid_data->mode_buf[i].flags & DRM_MODE_FLAG_DBLCLK)
			rate = mode_buf[i].clock * 1000 * 2;
		else
			rate = mode_buf[i].clock * 1000;

		/* Check whether mode is out of phy cfg range. */
		ret = rockchip_phy_round_rate(conn->phy, rate);

		if (ret < 0)
			edid_data->mode_buf[i].invalid = true;
	}
}
