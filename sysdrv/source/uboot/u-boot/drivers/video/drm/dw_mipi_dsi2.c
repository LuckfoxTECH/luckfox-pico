/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Author: Guochun Huang <hero.huang@rock-chips.com>
 */

#include <drm/drm_mipi_dsi.h>

#include <config.h>
#include <common.h>
#include <errno.h>
#include <asm/unaligned.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <dm/device.h>
#include <dm/read.h>
#include <dm/of_access.h>
#include <regmap.h>
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>
#include <linux/iopoll.h>

#include "rockchip_bridge.h"
#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_panel.h"
#include "rockchip_phy.h"

#define UPDATE(v, h, l)		(((v) << (l)) & GENMASK((h), (l)))

#define DSI2_PWR_UP			0x000c
#define RESET				0
#define POWER_UP			BIT(0)
#define CMD_TX_MODE(x)			UPDATE(x,  24,  24)
#define DSI2_SOFT_RESET			0x0010
#define SYS_RSTN			BIT(2)
#define PHY_RSTN			BIT(1)
#define IPI_RSTN			BIT(0)
#define INT_ST_MAIN			0x0014
#define DSI2_MODE_CTRL			0x0018
#define DSI2_MODE_STATUS		0x001c
#define DSI2_CORE_STATUS		0x0020
#define PRI_RD_DATA_AVAIL		BIT(26)
#define PRI_FIFOS_NOT_EMPTY		BIT(25)
#define PRI_BUSY			BIT(24)
#define CRI_RD_DATA_AVAIL		BIT(18)
#define CRT_FIFOS_NOT_EMPTY		BIT(17)
#define CRI_BUSY			BIT(16)
#define IPI_FIFOS_NOT_EMPTY		BIT(9)
#define IPI_BUSY			BIT(8)
#define CORE_FIFOS_NOT_EMPTY		BIT(1)
#define CORE_BUSY			BIT(0)
#define MANUAL_MODE_CFG			0x0024
#define MANUAL_MODE_EN			BIT(0)
#define DSI2_TIMEOUT_HSTX_CFG		0x0048
#define TO_HSTX(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_HSTXRDY_CFG	0x004c
#define TO_HSTXRDY(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPRX_CFG		0x0050
#define TO_LPRXRDY(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPTXRDY_CFG	0x0054
#define TO_LPTXRDY(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPTXTRIG_CFG	0x0058
#define TO_LPTXTRIG(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_LPTXULPS_CFG	0x005c
#define TO_LPTXULPS(x)			UPDATE(x, 15, 0)
#define DSI2_TIMEOUT_BTA_CFG		0x60
#define TO_BTA(x)			UPDATE(x, 15, 0)

#define DSI2_PHY_MODE_CFG		0x0100
#define PPI_WIDTH(x)			UPDATE(x, 9, 8)
#define PHY_LANES(x)			UPDATE(x - 1, 5, 4)
#define PHY_TYPE(x)			UPDATE(x, 0, 0)
#define DSI2_PHY_CLK_CFG		0X0104
#define PHY_LPTX_CLK_DIV(x)		UPDATE(x, 12, 8)
#define NON_CONTINUOUS_CLK		BIT(0)
#define DSI2_PHY_LP2HS_MAN_CFG		0x010c
#define PHY_LP2HS_TIME(x)		UPDATE(x, 28, 0)
#define DSI2_PHY_HS2LP_MAN_CFG		0x0114
#define PHY_HS2LP_TIME(x)		UPDATE(x, 28, 0)
#define DSI2_PHY_MAX_RD_T_MAN_CFG	0x011c
#define PHY_MAX_RD_TIME(x)		UPDATE(x, 26, 0)
#define DSI2_PHY_ESC_CMD_T_MAN_CFG	0x0124
#define PHY_ESC_CMD_TIME(x)		UPDATE(x, 28, 0)
#define DSI2_PHY_ESC_BYTE_T_MAN_CFG	0x012c
#define PHY_ESC_BYTE_TIME(x)		UPDATE(x, 28, 0)

#define DSI2_PHY_IPI_RATIO_MAN_CFG	0x0134
#define PHY_IPI_RATIO(x)		UPDATE(x, 21, 0)
#define DSI2_PHY_SYS_RATIO_MAN_CFG	0x013C
#define PHY_SYS_RATIO(x)		UPDATE(x, 16, 0)

#define DSI2_DSI_GENERAL_CFG		0x0200
#define BTA_EN				BIT(1)
#define EOTP_TX_EN			BIT(0)
#define DSI2_DSI_VCID_CFG		0x0204
#define TX_VCID(x)			UPDATE(x, 1, 0)
#define DSI2_DSI_SCRAMBLING_CFG		0x0208
#define SCRAMBLING_SEED(x)		UPDATE(x, 31, 16)
#define SCRAMBLING_EN			BIT(0)
#define DSI2_DSI_VID_TX_CFG		0x020c
#define LPDT_DISPLAY_CMD_EN		BIT(20)
#define BLK_VFP_HS_EN			BIT(14)
#define BLK_VBP_HS_EN			BIT(13)
#define BLK_VSA_HS_EN			BIT(12)
#define BLK_HFP_HS_EN			BIT(6)
#define BLK_HBP_HS_EN			BIT(5)
#define BLK_HSA_HS_EN			BIT(4)
#define VID_MODE_TYPE(x)		UPDATE(x, 1, 0)
#define DSI2_CRI_TX_HDR			0x02c0
#define CMD_TX_MODE(x)			UPDATE(x, 24, 24)
#define DSI2_CRI_TX_PLD			0x02c4
#define DSI2_CRI_RX_HDR			0x02c8
#define DSI2_CRI_RX_PLD			0x02cc

#define DSI2_IPI_COLOR_MAN_CFG		0x0300
#define IPI_DEPTH(x)			UPDATE(x, 7, 4)
#define IPI_DEPTH_5_6_5_BITS		0x02
#define IPI_DEPTH_6_BITS		0x03
#define IPI_DEPTH_8_BITS		0x05
#define IPI_DEPTH_10_BITS		0x06
#define IPI_FORMAT(x)			UPDATE(x, 3, 0)
#define IPI_FORMAT_RGB			0x0
#define IPI_FORMAT_DSC			0x0b
#define DSI2_IPI_VID_HSA_MAN_CFG	0x0304
#define VID_HSA_TIME(x)			UPDATE(x, 29, 0)
#define DSI2_IPI_VID_HBP_MAN_CFG	0x030c
#define VID_HBP_TIME(x)			UPDATE(x, 29, 0)
#define DSI2_IPI_VID_HACT_MAN_CFG	0x0314
#define VID_HACT_TIME(x)		UPDATE(x, 29, 0)
#define DSI2_IPI_VID_HLINE_MAN_CFG	0x031c
#define VID_HLINE_TIME(x)		UPDATE(x, 29, 0)
#define DSI2_IPI_VID_VSA_MAN_CFG	0x0324
#define VID_VSA_LINES(x)		UPDATE(x, 9, 0)
#define DSI2_IPI_VID_VBP_MAN_CFG	0X032C
#define VID_VBP_LINES(x)		UPDATE(x, 9, 0)
#define DSI2_IPI_VID_VACT_MAN_CFG	0X0334
#define VID_VACT_LINES(x)		UPDATE(x, 13, 0)
#define DSI2_IPI_VID_VFP_MAN_CFG	0X033C
#define VID_VFP_LINES(x)		UPDATE(x, 9, 0)
#define DSI2_IPI_PIX_PKT_CFG		0x0344
#define MAX_PIX_PKT(x)			UPDATE(x, 15, 0)

#define DSI2_INT_ST_PHY			0x0400
#define DSI2_INT_MASK_PHY		0x0404
#define DSI2_INT_ST_TO			0x0410
#define DSI2_INT_MASK_TO		0x0414
#define DSI2_INT_ST_ACK			0x0420
#define DSI2_INT_MASK_ACK		0x0424
#define DSI2_INT_ST_IPI			0x0430
#define DSI2_INT_MASK_IPI		0x0434
#define DSI2_INT_ST_FIFO		0x0440
#define DSI2_INT_MASK_FIFO		0x0444
#define DSI2_INT_ST_PRI			0x0450
#define DSI2_INT_MASK_PRI		0x0454
#define DSI2_INT_ST_CRI			0x0460
#define DSI2_INT_MASK_CRI		0x0464
#define DSI2_INT_FORCE_CRI		0x0468
#define DSI2_MAX_REGISGER		DSI2_INT_FORCE_CRI

#define CMD_PKT_STATUS_TIMEOUT_US	1000
#define MODE_STATUS_TIMEOUT_US		20000
#define SYS_CLK				351000000LL
#define PSEC_PER_SEC			1000000000000LL
#define USEC_PER_SEC			1000000L
#define MSEC_PER_SEC			1000L

#define GRF_REG_FIELD(reg, lsb, msb)	(((reg) << 16) | ((lsb) << 8) | (msb))

enum vid_mode_type {
	VID_MODE_TYPE_NON_BURST_SYNC_PULSES,
	VID_MODE_TYPE_NON_BURST_SYNC_EVENTS,
	VID_MODE_TYPE_BURST,
};

enum mode_ctrl {
	IDLE_MODE,
	AUTOCALC_MODE,
	COMMAND_MODE,
	VIDEO_MODE,
	DATA_STREAM_MODE,
	VIDE_TEST_MODE,
	DATA_STREAM_TEST_MODE,
};

enum grf_reg_fields {
	TXREQCLKHS_EN,
	GATING_EN,
	IPI_SHUTDN,
	IPI_COLORM,
	IPI_COLOR_DEPTH,
	IPI_FORMAT,
	MAX_FIELDS,
};

enum phy_type {
	DPHY,
	CPHY,
};

enum ppi_width {
	PPI_WIDTH_8_BITS,
	PPI_WIDTH_16_BITS,
	PPI_WIDTH_32_BITS,
};

struct rockchip_cmd_header {
	u8 data_type;
	u8 delay_ms;
	u8 payload_length;
};

struct dw_mipi_dsi2_plat_data {
	const u32 *dsi0_grf_reg_fields;
	const u32 *dsi1_grf_reg_fields;
	unsigned long long dphy_max_bit_rate_per_lane;
	unsigned long long cphy_max_symbol_rate_per_lane;
};

struct mipi_dcphy {
	/* Non-SNPS PHY */
	struct rockchip_phy *phy;

	u16 input_div;
	u16 feedback_div;
};

/**
 * struct mipi_dphy_configure - MIPI D-PHY configuration set
 *
 * This structure is used to represent the configuration state of a
 * MIPI D-PHY phy.
 */
struct mipi_dphy_configure {
	unsigned int		clk_miss;
	unsigned int		clk_post;
	unsigned int		clk_pre;
	unsigned int		clk_prepare;
	unsigned int		clk_settle;
	unsigned int		clk_term_en;
	unsigned int		clk_trail;
	unsigned int		clk_zero;
	unsigned int		d_term_en;
	unsigned int		eot;
	unsigned int		hs_exit;
	unsigned int		hs_prepare;
	unsigned int		hs_settle;
	unsigned int		hs_skip;
	unsigned int		hs_trail;
	unsigned int		hs_zero;
	unsigned int		init;
	unsigned int		lpx;
	unsigned int		ta_get;
	unsigned int		ta_go;
	unsigned int		ta_sure;
	unsigned int		wakeup;
	unsigned long		hs_clk_rate;
	unsigned long		lp_clk_rate;
	unsigned char		lanes;
};

struct dw_mipi_dsi2 {
	struct rockchip_connector connector;
	struct udevice *dev;
	void *base;
	void *grf;
	int id;
	struct dw_mipi_dsi2 *master;
	struct dw_mipi_dsi2 *slave;
	bool prepared;

	bool c_option;
	bool dsc_enable;
	bool scrambling_en;
	unsigned int slice_width;
	unsigned int slice_height;
	u32 version_major;
	u32 version_minor;

	unsigned int lane_hs_rate; /* Kbps/Ksps per lane */
	u32 channel;
	u32 lanes;
	u32 format;
	u32 mode_flags;
	struct mipi_dcphy dcphy;
	struct drm_display_mode mode;
	bool data_swap;

	struct mipi_dsi_device *device;
	struct mipi_dphy_configure mipi_dphy_cfg;
	const struct dw_mipi_dsi2_plat_data *pdata;
	struct drm_dsc_picture_parameter_set *pps;
};

static inline void dsi_write(struct dw_mipi_dsi2 *dsi2, u32 reg, u32 val)
{
	writel(val, dsi2->base + reg);
}

static inline u32 dsi_read(struct dw_mipi_dsi2 *dsi2, u32 reg)
{
	return readl(dsi2->base + reg);
}

static inline void dsi_update_bits(struct dw_mipi_dsi2 *dsi2,
				   u32 reg, u32 mask, u32 val)
{
	u32 orig, tmp;

	orig = dsi_read(dsi2, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	dsi_write(dsi2, reg, tmp);
}

static void grf_field_write(struct dw_mipi_dsi2 *dsi2, enum grf_reg_fields index,
			    unsigned int val)
{
	const u32 field = dsi2->id ? dsi2->pdata->dsi1_grf_reg_fields[index] :
			  dsi2->pdata->dsi0_grf_reg_fields[index];
	u16 reg;
	u8 msb, lsb;

	if (!field)
		return;

	reg = (field >> 16) & 0xffff;
	lsb = (field >>  8) & 0xff;
	msb = (field >>  0) & 0xff;

	regmap_write(dsi2->grf, reg, GENMASK(msb, lsb) << 16 | val << lsb);
}

static unsigned long dw_mipi_dsi2_get_lane_rate(struct dw_mipi_dsi2 *dsi2)
{
	const struct drm_display_mode *mode = &dsi2->mode;
	u64 max_lane_rate, lane_rate;
	unsigned int value;
	int bpp, lanes;
	u64 tmp;

	max_lane_rate = (dsi2->c_option) ?
			dsi2->pdata->cphy_max_symbol_rate_per_lane :
			dsi2->pdata->dphy_max_bit_rate_per_lane;

	/*
	 * optional override of the desired bandwidth
	 * High-Speed mode: Differential and terminated: 80Mbps ~ 4500 Mbps
	 */
	value = dev_read_u32_default(dsi2->dev, "rockchip,lane-rate", 0);
	if (value >= 80000 && value <= 4500000)
		return value * MSEC_PER_SEC;
	else if (value >= 80 && value <= 4500)
		return value * USEC_PER_SEC;

	bpp = mipi_dsi_pixel_format_to_bpp(dsi2->format);
	if (bpp < 0)
		bpp = 24;

	lanes = dsi2->slave ? dsi2->lanes * 2 : dsi2->lanes;
	tmp = (u64)mode->crtc_clock * 1000 * bpp;
	do_div(tmp, lanes);

	if (dsi2->c_option)
		tmp = DIV_ROUND_CLOSEST(tmp * 100, 228);

	/* set BW a little larger only in video burst mode in
	 * consideration of the protocol overhead and HS mode
	 * switching to BLLP mode, take 1 / 0.9, since Mbps must
	 * big than bandwidth of RGB
	 */
	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_BURST) {
		tmp *= 10;
		do_div(tmp, 9);
	}

	if (tmp > max_lane_rate)
		lane_rate = max_lane_rate;
	else
		lane_rate = tmp;

	return lane_rate;
}

static int cri_fifos_wait_avail(struct dw_mipi_dsi2 *dsi2)
{
	u32 sts, mask;
	int ret;

	mask = CRI_BUSY | CRT_FIFOS_NOT_EMPTY;
	ret = readl_poll_timeout(dsi2->base + DSI2_CORE_STATUS,
				 sts, !(sts & mask),
				 CMD_PKT_STATUS_TIMEOUT_US);
	if (ret < 0) {
		printf("command interface is busy: 0x%x\n", sts);
		return ret;
	}

	return 0;
}

static int dw_mipi_dsi2_read_from_fifo(struct dw_mipi_dsi2 *dsi2,
				      const struct mipi_dsi_msg *msg)
{
	u8 *payload = msg->rx_buf;
	u8 data_type;
	u16 wc;
	int i, j, ret, len = msg->rx_len;
	unsigned int vrefresh = drm_mode_vrefresh(&dsi2->mode);
	u32 val;

	ret = readl_poll_timeout(dsi2->base + DSI2_CORE_STATUS,
				 val, val & CRI_RD_DATA_AVAIL,
				 DIV_ROUND_UP(1000000, vrefresh));
	if (ret) {
		printf("CRI has no available read data\n");
		return ret;
	}

	val = dsi_read(dsi2, DSI2_CRI_RX_HDR);
	data_type = val & 0x3f;

	if (mipi_dsi_packet_format_is_short(data_type)) {
		for (i = 0; i < len && i < 2; i++)
			payload[i] = (val >> (8 * (i + 1))) & 0xff;

		return 0;
	}

	wc = (val >> 8) & 0xffff;
	/* Receive payload */
	for (i = 0; i < len && i < wc; i += 4) {
		val = dsi_read(dsi2, DSI2_CRI_RX_PLD);
		for (j = 0; j < 4 && j + i < len && j + i < wc; j++)
			payload[i + j] = val >> (8 * j);
	}

	return 0;
}

static ssize_t dw_mipi_dsi2_transfer(struct dw_mipi_dsi2 *dsi2,
				    const struct mipi_dsi_msg *msg)
{
	struct mipi_dsi_packet packet;
	int ret;
	int val;
	u32 mode;

	dsi_update_bits(dsi2, DSI2_DSI_VID_TX_CFG, LPDT_DISPLAY_CMD_EN,
			msg->flags & MIPI_DSI_MSG_USE_LPM ?
			LPDT_DISPLAY_CMD_EN : 0);

	/* create a packet to the DSI protocol */
	ret = mipi_dsi_create_packet(&packet, msg);
	if (ret) {
		printf("failed to create packet: %d\n", ret);
		return ret;
	}

	/* check cri interface is not busy */
	ret = cri_fifos_wait_avail(dsi2);
	if (ret)
		return ret;

	/* Send payload */
	while (DIV_ROUND_UP(packet.payload_length, 4)) {
		if (packet.payload_length < 4) {
			/* send residu payload */
			val = 0;
			memcpy(&val, packet.payload, packet.payload_length);
			dsi_write(dsi2, DSI2_CRI_TX_PLD, val);
			packet.payload_length = 0;
		} else {
			val = get_unaligned_le32(packet.payload);
			dsi_write(dsi2, DSI2_CRI_TX_PLD, val);
			packet.payload += 4;
			packet.payload_length -= 4;
		}
	}

	/* Send packet header */
	mode = CMD_TX_MODE(msg->flags & MIPI_DSI_MSG_USE_LPM ? 1 : 0);
	val = get_unaligned_le32(packet.header);
	dsi_write(dsi2, DSI2_CRI_TX_HDR, mode | val);

	ret = cri_fifos_wait_avail(dsi2);
	if (ret)
		return ret;

	if (msg->rx_len) {
		ret = dw_mipi_dsi2_read_from_fifo(dsi2, msg);
		if (ret < 0)
			return ret;
	}

	if (dsi2->slave) {
		ret = dw_mipi_dsi2_transfer(dsi2->slave, msg);
		if (ret < 0)
			return ret;
	}

	return msg->rx_len ? msg->rx_len : msg->tx_len;
}

static void dw_mipi_dsi2_ipi_color_coding_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u32 val, color_depth;

	switch (dsi2->format) {
	case MIPI_DSI_FMT_RGB666:
	case MIPI_DSI_FMT_RGB666_PACKED:
		color_depth = IPI_DEPTH_6_BITS;
		break;
	case MIPI_DSI_FMT_RGB565:
		color_depth = IPI_DEPTH_5_6_5_BITS;
		break;
	case MIPI_DSI_FMT_RGB888:
	default:
		color_depth = IPI_DEPTH_8_BITS;
		break;
	}

	val = IPI_DEPTH(color_depth) |
	      IPI_FORMAT(dsi2->dsc_enable ? IPI_FORMAT_DSC : IPI_FORMAT_RGB);
	dsi_write(dsi2, DSI2_IPI_COLOR_MAN_CFG, val);
	grf_field_write(dsi2, IPI_COLOR_DEPTH, color_depth);

	if (dsi2->dsc_enable)
		grf_field_write(dsi2, IPI_FORMAT, IPI_FORMAT_DSC);
}

static void dw_mipi_dsi2_ipi_set(struct dw_mipi_dsi2 *dsi2)
{
	struct drm_display_mode *mode = &dsi2->mode;
	u32 hline, hsa, hbp, hact;
	u64 hline_time, hsa_time, hbp_time, hact_time, tmp;
	u64 pixel_clk, phy_hs_clk;
	u32 vact, vsa, vfp, vbp;
	u16 val;

	if (dsi2->slave || dsi2->master)
		val = mode->hdisplay / 2;
	else
		val = mode->hdisplay;

	dsi_write(dsi2, DSI2_IPI_PIX_PKT_CFG, MAX_PIX_PKT(val));

	dw_mipi_dsi2_ipi_color_coding_cfg(dsi2);

	/*
	 * if the controller is intended to operate in data stream mode,
	 * no more steps are required.
	 */
	if (!(dsi2->mode_flags & MIPI_DSI_MODE_VIDEO))
		return;

	vact = mode->vdisplay;
	vsa = mode->vsync_end - mode->vsync_start;
	vfp = mode->vsync_start - mode->vdisplay;
	vbp = mode->vtotal - mode->vsync_end;
	hact = mode->hdisplay;
	hsa = mode->hsync_end - mode->hsync_start;
	hbp = mode->htotal - mode->hsync_end;
	hline = mode->htotal;

	pixel_clk = mode->crtc_clock * MSEC_PER_SEC;

	if (dsi2->c_option)
		phy_hs_clk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 7);
	else
		phy_hs_clk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 16);

	tmp = hsa * phy_hs_clk;
	hsa_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HSA_MAN_CFG, VID_HSA_TIME(hsa_time));

	tmp = hbp * phy_hs_clk;
	hbp_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HBP_MAN_CFG, VID_HBP_TIME(hbp_time));

	tmp = hact * phy_hs_clk;
	hact_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HACT_MAN_CFG, VID_HACT_TIME(hact_time));

	tmp = hline * phy_hs_clk;
	hline_time = DIV_ROUND_CLOSEST(tmp << 16, pixel_clk);
	dsi_write(dsi2, DSI2_IPI_VID_HLINE_MAN_CFG, VID_HLINE_TIME(hline_time));

	dsi_write(dsi2, DSI2_IPI_VID_VSA_MAN_CFG, VID_VSA_LINES(vsa));
	dsi_write(dsi2, DSI2_IPI_VID_VBP_MAN_CFG, VID_VBP_LINES(vbp));
	dsi_write(dsi2, DSI2_IPI_VID_VACT_MAN_CFG, VID_VACT_LINES(vact));
	dsi_write(dsi2, DSI2_IPI_VID_VFP_MAN_CFG, VID_VFP_LINES(vfp));
}

static void dw_mipi_dsi2_set_vid_mode(struct dw_mipi_dsi2 *dsi2)
{
	u32 val = 0, mode;
	int ret;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_HFP)
		val |= BLK_HFP_HS_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_HBP)
		val |= BLK_HBP_HS_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_HSA)
		val |= BLK_HSA_HS_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_BURST)
		val |= VID_MODE_TYPE_BURST;
	else if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO_SYNC_PULSE)
		val |= VID_MODE_TYPE_NON_BURST_SYNC_PULSES;
	else
		val |= VID_MODE_TYPE_NON_BURST_SYNC_EVENTS;

	dsi_write(dsi2, DSI2_DSI_VID_TX_CFG, val);

	dsi_write(dsi2, DSI2_MODE_CTRL, VIDEO_MODE);
	ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
				 mode, mode & VIDEO_MODE,
				 MODE_STATUS_TIMEOUT_US);
	if (ret < 0)
		printf("failed to enter video mode\n");
}

static void dw_mipi_dsi2_set_data_stream_mode(struct dw_mipi_dsi2 *dsi2)
{
	u32 mode;
	int ret;

	dsi_write(dsi2, DSI2_MODE_CTRL, DATA_STREAM_MODE);
	ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
				 mode, mode & DATA_STREAM_MODE,
				 MODE_STATUS_TIMEOUT_US);
	if (ret < 0)
		printf("failed to enter data stream mode\n");
}

static void dw_mipi_dsi2_set_cmd_mode(struct dw_mipi_dsi2 *dsi2)
{
	u32 mode;
	int ret;

	dsi_write(dsi2, DSI2_MODE_CTRL, COMMAND_MODE);
	ret = readl_poll_timeout(dsi2->base + DSI2_MODE_STATUS,
				 mode, mode & COMMAND_MODE,
				 MODE_STATUS_TIMEOUT_US);
	if (ret < 0)
		printf("failed to enter cmd mode\n");
}

static int dw_mipi_dsi2_get_dsc_params_from_sink(struct dw_mipi_dsi2 *dsi2)
{
	struct udevice *dev = dsi2->device->dev;
	struct rockchip_cmd_header *header;
	struct drm_dsc_picture_parameter_set *pps = NULL;
	u8 *dsc_packed_pps;
	const void *data;
	int len;

	dsi2->c_option = dev_read_bool(dev, "phy-c-option");
	dsi2->scrambling_en = dev_read_bool(dev, "scrambling-enable");
	dsi2->dsc_enable = dev_read_bool(dev, "compressed-data");

	if (dsi2->slave) {
		dsi2->slave->c_option = dsi2->c_option;
		dsi2->slave->scrambling_en = dsi2->scrambling_en;
		dsi2->slave->dsc_enable = dsi2->dsc_enable;
	}

	dsi2->slice_width = dev_read_u32_default(dev, "slice-width", 0);
	dsi2->slice_height = dev_read_u32_default(dev, "slice-height", 0);
	dsi2->version_major = dev_read_u32_default(dev, "version-major", 0);
	dsi2->version_minor = dev_read_u32_default(dev, "version-minor", 0);

	data = dev_read_prop(dev, "panel-init-sequence", &len);
	if (!data)
		return -EINVAL;

	while (len > sizeof(*header)) {
		header = (struct rockchip_cmd_header *)data;
		data += sizeof(*header);
		len -= sizeof(*header);

		if (header->payload_length > len)
			return -EINVAL;

		if (header->data_type == MIPI_DSI_PICTURE_PARAMETER_SET) {
			dsc_packed_pps = calloc(1, header->payload_length);
			if (!dsc_packed_pps)
				return -ENOMEM;

			memcpy(dsc_packed_pps, data, header->payload_length);
			pps = (struct drm_dsc_picture_parameter_set *)dsc_packed_pps;
			break;
		}

		data += header->payload_length;
		len -= header->payload_length;
	}

	dsi2->pps = pps;

	return 0;
}

static void dw_mipi_dsi2_enable(struct dw_mipi_dsi2 *dsi2)
{
	dw_mipi_dsi2_ipi_set(dsi2);

	if (dsi2->mode_flags & MIPI_DSI_MODE_VIDEO)
		dw_mipi_dsi2_set_vid_mode(dsi2);
	else
		dw_mipi_dsi2_set_data_stream_mode(dsi2);

	if (dsi2->slave)
		dw_mipi_dsi2_enable(dsi2->slave);
}

static void dw_mipi_dsi2_disable(struct dw_mipi_dsi2 *dsi2)
{
	dsi_write(dsi2, DSI2_IPI_PIX_PKT_CFG, 0);
	dw_mipi_dsi2_set_cmd_mode(dsi2);

	if (dsi2->slave)
		dw_mipi_dsi2_disable(dsi2->slave);
}

static void dw_mipi_dsi2_post_disable(struct dw_mipi_dsi2 *dsi2)
{
	if (!dsi2->prepared)
		return;

	dsi_write(dsi2, DSI2_PWR_UP, RESET);

	if (dsi2->dcphy.phy)
		rockchip_phy_power_off(dsi2->dcphy.phy);

	dsi2->prepared = false;

	if (dsi2->slave)
		dw_mipi_dsi2_post_disable(dsi2->slave);
}

static int dw_mipi_dsi2_connector_pre_init(struct rockchip_connector *conn,
					   struct display_state *state)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(conn->dev);
	struct mipi_dsi_host *host = dev_get_platdata(dsi2->dev);
	struct mipi_dsi_device *device;
	char name[20];
	struct udevice *dev;

	device = calloc(1, sizeof(struct dw_mipi_dsi2));
	if (!device)
		return -ENOMEM;

	if (conn->bridge)
		dev = conn->bridge->dev;
	else if (conn->panel)
		dev = conn->panel->dev;
	else
		return -ENODEV;

	device->dev = dev;
	device->host = host;
	device->lanes = dev_read_u32_default(dev, "dsi,lanes", 4);
	device->channel = dev_read_u32_default(dev, "reg", 0);
	device->format = dev_read_u32_default(dev, "dsi,format",
					      MIPI_DSI_FMT_RGB888);
	device->mode_flags = dev_read_u32_default(dev, "dsi,flags",
						  MIPI_DSI_MODE_VIDEO |
						  MIPI_DSI_MODE_VIDEO_BURST |
						  MIPI_DSI_MODE_VIDEO_HBP |
						  MIPI_DSI_MODE_LPM |
						  MIPI_DSI_MODE_EOT_PACKET);

	sprintf(name, "%s.%d", host->dev->name, device->channel);
	device_set_name(dev, name);
	dsi2->device = device;
	dev->parent_platdata = device;

	mipi_dsi_attach(dsi2->device);

	return 0;
}

static int dw_mipi_dsi2_connector_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct crtc_state *cstate = &state->crtc_state;
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(conn->dev);
	struct rockchip_phy *phy = NULL;
	struct udevice *phy_dev;
	struct udevice *dev;
	int ret;

	conn_state->disp_info  = rockchip_get_disp_info(conn_state->type, dsi2->id);
	dsi2->dcphy.phy = conn->phy;

	conn_state->output_mode = ROCKCHIP_OUT_MODE_P888;
	conn_state->color_space = V4L2_COLORSPACE_DEFAULT;
	conn_state->output_if |=
		dsi2->id ? VOP_OUTPUT_IF_MIPI1 : VOP_OUTPUT_IF_MIPI0;

	if (!(dsi2->mode_flags & MIPI_DSI_MODE_VIDEO)) {
		conn_state->output_flags |= ROCKCHIP_OUTPUT_MIPI_DS_MODE;
		conn_state->hold_mode = true;
	}

	if (dsi2->lanes > 4) {
		ret = uclass_get_device_by_name(UCLASS_DISPLAY,
						"dsi@fde30000",
						&dev);
		if (ret)
			return ret;

		dsi2->slave = dev_get_priv(dev);
		if (!dsi2->slave)
			return -ENODEV;

		dsi2->slave->master = dsi2;
		dsi2->lanes /= 2;
		dsi2->slave->lanes = dsi2->lanes;
		dsi2->slave->format = dsi2->format;
		dsi2->slave->mode_flags = dsi2->mode_flags;
		dsi2->slave->channel = dsi2->channel;
		conn_state->output_flags |=
				ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE;
		if (dsi2->data_swap)
			conn_state->output_flags |= ROCKCHIP_OUTPUT_DATA_SWAP;

		conn_state->output_if |= VOP_OUTPUT_IF_MIPI1;

		ret = uclass_get_device_by_phandle(UCLASS_PHY, dev,
						   "phys", &phy_dev);
		if (ret)
			return -ENODEV;

		phy = (struct rockchip_phy *)dev_get_driver_data(phy_dev);
		if (!phy)
			return -ENODEV;

		dsi2->slave->dcphy.phy = phy;
		if (phy->funcs && phy->funcs->init)
			return phy->funcs->init(phy);
	}

	dw_mipi_dsi2_get_dsc_params_from_sink(dsi2);

	if (dsi2->dsc_enable) {
		cstate->dsc_enable = 1;
		cstate->dsc_sink_cap.version_major = dsi2->version_major;
		cstate->dsc_sink_cap.version_minor = dsi2->version_minor;
		cstate->dsc_sink_cap.slice_width = dsi2->slice_width;
		cstate->dsc_sink_cap.slice_height = dsi2->slice_height;
		/* only can support rgb888 panel now */
		cstate->dsc_sink_cap.target_bits_per_pixel_x16 = 8 << 4;
		cstate->dsc_sink_cap.native_420 = 0;
		memcpy(&cstate->pps, dsi2->pps, sizeof(struct drm_dsc_picture_parameter_set));
	}

	return 0;
}

/*
 * Minimum D-PHY timings based on MIPI D-PHY specification. Derived
 * from the valid ranges specified in Section 6.9, Table 14, Page 41
 * of the D-PHY specification (v2.1).
 */
int mipi_dphy_get_default_config(unsigned long long hs_clk_rate,
				 struct mipi_dphy_configure *cfg)
{
	unsigned long long ui;

	if (!cfg)
		return -EINVAL;

	ui = ALIGN(PSEC_PER_SEC, hs_clk_rate);
	do_div(ui, hs_clk_rate);

	cfg->clk_miss = 0;
	cfg->clk_post = 60000 + 52 * ui;
	cfg->clk_pre = 8000;
	cfg->clk_prepare = 38000;
	cfg->clk_settle = 95000;
	cfg->clk_term_en = 0;
	cfg->clk_trail = 60000;
	cfg->clk_zero = 262000;
	cfg->d_term_en = 0;
	cfg->eot = 0;
	cfg->hs_exit = 100000;
	cfg->hs_prepare = 40000 + 4 * ui;
	cfg->hs_zero = 105000 + 6 * ui;
	cfg->hs_settle = 85000 + 6 * ui;
	cfg->hs_skip = 40000;

	/*
	 * The MIPI D-PHY specification (Section 6.9, v1.2, Table 14, Page 40)
	 * contains this formula as:
	 *
	 *     T_HS-TRAIL = max(n * 8 * ui, 60 + n * 4 * ui)
	 *
	 * where n = 1 for forward-direction HS mode and n = 4 for reverse-
	 * direction HS mode. There's only one setting and this function does
	 * not parameterize on anything other that ui, so this code will
	 * assumes that reverse-direction HS mode is supported and uses n = 4.
	 */
	cfg->hs_trail = max(4 * 8 * ui, 60000 + 4 * 4 * ui);

	cfg->init = 100;
	cfg->lpx = 60000;
	cfg->ta_get = 5 * cfg->lpx;
	cfg->ta_go = 4 * cfg->lpx;
	cfg->ta_sure = 2 * cfg->lpx;
	cfg->wakeup = 1000;

	return 0;
}

static void dw_mipi_dsi2_set_hs_clk(struct dw_mipi_dsi2 *dsi2, unsigned long rate)
{
	mipi_dphy_get_default_config(rate, &dsi2->mipi_dphy_cfg);

	if (!dsi2->c_option)
		rockchip_phy_set_mode(dsi2->dcphy.phy, PHY_MODE_MIPI_DPHY);

	rate = rockchip_phy_set_pll(dsi2->dcphy.phy, rate);
	dsi2->lane_hs_rate = DIV_ROUND_CLOSEST(rate, MSEC_PER_SEC);
}

static void dw_mipi_dsi2_host_softrst(struct dw_mipi_dsi2 *dsi2)
{
	dsi_write(dsi2, DSI2_SOFT_RESET, 0X0);
	udelay(100);
	dsi_write(dsi2, DSI2_SOFT_RESET, SYS_RSTN | PHY_RSTN | IPI_RSTN);
}

static void
dw_mipi_dsi2_work_mode(struct dw_mipi_dsi2 *dsi2, u32 mode)
{
	/*
	 * select controller work in Manual mode
	 * Manual: MANUAL_MODE_EN
	 * Automatic: 0
	 */
	dsi_write(dsi2, MANUAL_MODE_CFG, mode);
}

static void dw_mipi_dsi2_phy_mode_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u32 val = 0;

	/* PPI width is fixed to 16 bits in DCPHY */
	val |= PPI_WIDTH(PPI_WIDTH_16_BITS) | PHY_LANES(dsi2->lanes);
	val |= PHY_TYPE(dsi2->c_option ? CPHY : DPHY);
	dsi_write(dsi2, DSI2_PHY_MODE_CFG, val);
}

static void dw_mipi_dsi2_phy_clk_mode_cfg(struct dw_mipi_dsi2 *dsi2)
{
	u32 sys_clk = SYS_CLK / USEC_PER_SEC;
	u32 esc_clk_div;
	u32 val = 0;

	if (dsi2->mode_flags & MIPI_DSI_CLOCK_NON_CONTINUOUS)
		val |= NON_CONTINUOUS_CLK;

	/* The Escape clock ranges from 1MHz to 20MHz. */
	esc_clk_div = DIV_ROUND_UP(sys_clk, 10 * 2);
	val |= PHY_LPTX_CLK_DIV(esc_clk_div);

	dsi_write(dsi2, DSI2_PHY_CLK_CFG, val);
}

static void dw_mipi_dsi2_phy_ratio_cfg(struct dw_mipi_dsi2 *dsi2)
{
	struct drm_display_mode *mode = &dsi2->mode;
	u64 pixel_clk, ipi_clk, phy_hsclk, tmp;

	/*
	 * in DPHY mode, the phy_hstx_clk is exactly 1/16 the Lane high-speed
	 * data rate; In CPHY mode, the phy_hstx_clk is exactly 1/7 the trio
	 * high speed symbol rate.
	 */
	if (dsi2->c_option)
		phy_hsclk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 7);

	else
		phy_hsclk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 16);

	/* IPI_RATIO_MAN_CFG = PHY_HSTX_CLK / IPI_CLK */
	pixel_clk = mode->crtc_clock * MSEC_PER_SEC;
	ipi_clk = pixel_clk / 4;

	tmp = DIV_ROUND_CLOSEST(phy_hsclk << 16, ipi_clk);
	dsi_write(dsi2, DSI2_PHY_IPI_RATIO_MAN_CFG, PHY_IPI_RATIO(tmp));

	/* SYS_RATIO_MAN_CFG = MIPI_DCPHY_HSCLK_Freq / SYS_CLK */
	tmp = DIV_ROUND_CLOSEST(phy_hsclk << 16, SYS_CLK);
	dsi_write(dsi2, DSI2_PHY_SYS_RATIO_MAN_CFG, PHY_SYS_RATIO(tmp));
}

static void dw_mipi_dsi2_lp2hs_or_hs2lp_cfg(struct dw_mipi_dsi2 *dsi2)
{
	struct mipi_dphy_configure *cfg = &dsi2->mipi_dphy_cfg;
	unsigned long long tmp, ui;
	unsigned long long hstx_clk;

	hstx_clk = DIV_ROUND_CLOSEST(dsi2->lane_hs_rate * MSEC_PER_SEC, 16);

	ui = ALIGN(PSEC_PER_SEC, hstx_clk);
	do_div(ui, hstx_clk);

	/* PHY_LP2HS_TIME = (TLPX + THS-PREPARE + THS-ZERO) / Tphy_hstx_clk */
	tmp = cfg->lpx + cfg->hs_prepare + cfg->hs_zero;
	tmp = DIV_ROUND_CLOSEST(tmp << 16, ui);
	dsi_write(dsi2, DSI2_PHY_LP2HS_MAN_CFG, PHY_LP2HS_TIME(tmp));

	/* PHY_HS2LP_TIME = (THS-TRAIL + THS-EXIT) / Tphy_hstx_clk */
	tmp = cfg->hs_trail + cfg->hs_exit;
	tmp = DIV_ROUND_CLOSEST(tmp << 16, ui);
	dsi_write(dsi2, DSI2_PHY_HS2LP_MAN_CFG, PHY_HS2LP_TIME(tmp));
}

static void dw_mipi_dsi2_phy_init(struct dw_mipi_dsi2 *dsi2)
{
	dw_mipi_dsi2_phy_mode_cfg(dsi2);
	dw_mipi_dsi2_phy_clk_mode_cfg(dsi2);
	dw_mipi_dsi2_phy_ratio_cfg(dsi2);
	dw_mipi_dsi2_lp2hs_or_hs2lp_cfg(dsi2);

	/* phy configuration 8 - 10 */
}

static void dw_mipi_dsi2_tx_option_set(struct dw_mipi_dsi2 *dsi2)
{
	u32 val;

	val = BTA_EN | EOTP_TX_EN;

	if (dsi2->mode_flags & MIPI_DSI_MODE_EOT_PACKET)
		val &= ~EOTP_TX_EN;

	dsi_write(dsi2, DSI2_DSI_GENERAL_CFG, val);
	dsi_write(dsi2, DSI2_DSI_VCID_CFG, TX_VCID(dsi2->channel));

	if (dsi2->scrambling_en)
		dsi_write(dsi2, DSI2_DSI_SCRAMBLING_CFG, SCRAMBLING_EN);
}

static void dw_mipi_dsi2_irq_enable(struct dw_mipi_dsi2 *dsi2, bool enable)
{
	if (enable) {
		dsi_write(dsi2, DSI2_INT_MASK_PHY, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_TO, 0xf);
		dsi_write(dsi2, DSI2_INT_MASK_ACK, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_IPI, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_FIFO, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_PRI, 0x1);
		dsi_write(dsi2, DSI2_INT_MASK_CRI, 0x1);
	} else {
		dsi_write(dsi2, DSI2_INT_MASK_PHY, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_TO, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_ACK, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_IPI, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_FIFO, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_PRI, 0x0);
		dsi_write(dsi2, DSI2_INT_MASK_CRI, 0x0);
	};
}

static void mipi_dcphy_power_on(struct dw_mipi_dsi2 *dsi2)
{
	if (!dsi2->dcphy.phy)
		return;

	rockchip_phy_power_on(dsi2->dcphy.phy);
}

static void dw_mipi_dsi2_pre_enable(struct dw_mipi_dsi2 *dsi2)
{
	if (dsi2->prepared)
		return;

	dw_mipi_dsi2_host_softrst(dsi2);
	dsi_write(dsi2, DSI2_PWR_UP, RESET);

	dw_mipi_dsi2_work_mode(dsi2, MANUAL_MODE_EN);
	dw_mipi_dsi2_phy_init(dsi2);
	dw_mipi_dsi2_tx_option_set(dsi2);
	dw_mipi_dsi2_irq_enable(dsi2, 0);
	mipi_dcphy_power_on(dsi2);
	dsi_write(dsi2, DSI2_PWR_UP, POWER_UP);
	dw_mipi_dsi2_set_cmd_mode(dsi2);

	dsi2->prepared = true;

	if (dsi2->slave)
		dw_mipi_dsi2_pre_enable(dsi2->slave);
}

static int dw_mipi_dsi2_connector_prepare(struct rockchip_connector *conn,
					  struct display_state *state)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(conn->dev);
	struct connector_state *conn_state = &state->conn_state;
	unsigned long lane_rate;

	memcpy(&dsi2->mode, &conn_state->mode, sizeof(struct drm_display_mode));
	if (dsi2->slave)
		memcpy(&dsi2->slave->mode, &dsi2->mode,
		       sizeof(struct drm_display_mode));

	lane_rate = dw_mipi_dsi2_get_lane_rate(dsi2);
	if (dsi2->dcphy.phy)
		dw_mipi_dsi2_set_hs_clk(dsi2, lane_rate);

	if (dsi2->slave && dsi2->slave->dcphy.phy)
		dw_mipi_dsi2_set_hs_clk(dsi2->slave, lane_rate);

	printf("final DSI-Link bandwidth: %u %s x %d\n",
	       dsi2->lane_hs_rate, dsi2->c_option ? "Ksps" : "Kbps",
	       dsi2->slave ? dsi2->lanes * 2 : dsi2->lanes);

	dw_mipi_dsi2_pre_enable(dsi2);

	return 0;
}

static void dw_mipi_dsi2_connector_unprepare(struct rockchip_connector *conn,
					     struct display_state *state)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(conn->dev);

	dw_mipi_dsi2_post_disable(dsi2);
}

static int dw_mipi_dsi2_connector_enable(struct rockchip_connector *conn,
					 struct display_state *state)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(conn->dev);

	dw_mipi_dsi2_enable(dsi2);

	return 0;
}

static int dw_mipi_dsi2_connector_disable(struct rockchip_connector *conn,
					  struct display_state *state)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(conn->dev);

	dw_mipi_dsi2_disable(dsi2);

	return 0;
}

static const struct rockchip_connector_funcs dw_mipi_dsi2_connector_funcs = {
	.pre_init = dw_mipi_dsi2_connector_pre_init,
	.init = dw_mipi_dsi2_connector_init,
	.prepare = dw_mipi_dsi2_connector_prepare,
	.unprepare = dw_mipi_dsi2_connector_unprepare,
	.enable = dw_mipi_dsi2_connector_enable,
	.disable = dw_mipi_dsi2_connector_disable,
};

static int dw_mipi_dsi2_probe(struct udevice *dev)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(dev);
	const struct dw_mipi_dsi2_plat_data *pdata =
		(const struct dw_mipi_dsi2_plat_data *)dev_get_driver_data(dev);
	struct udevice *syscon;
	int id, ret;

	dsi2->base = dev_read_addr_ptr(dev);

	ret = uclass_get_device_by_phandle(UCLASS_SYSCON, dev, "rockchip,grf",
					   &syscon);
	if (!ret) {
		dsi2->grf = syscon_get_regmap(syscon);
		if (!dsi2->grf)
			return -ENODEV;
	}

	id = of_alias_get_id(ofnode_to_np(dev->node), "dsi");
	if (id < 0)
		id = 0;

	dsi2->dev = dev;
	dsi2->pdata = pdata;
	dsi2->id = id;
	dsi2->data_swap = dev_read_bool(dsi2->dev, "rockchip,data-swap");

	rockchip_connector_bind(&dsi2->connector, dev, id, &dw_mipi_dsi2_connector_funcs, NULL,
				DRM_MODE_CONNECTOR_DSI);

	return 0;
}

static const u32 rk3588_dsi0_grf_reg_fields[MAX_FIELDS] = {
	[TXREQCLKHS_EN]		= GRF_REG_FIELD(0x0000, 11, 11),
	[GATING_EN]		= GRF_REG_FIELD(0x0000, 10, 10),
	[IPI_SHUTDN]		= GRF_REG_FIELD(0x0000,  9,  9),
	[IPI_COLORM]		= GRF_REG_FIELD(0x0000,  8,  8),
	[IPI_COLOR_DEPTH]	= GRF_REG_FIELD(0x0000,  4,  7),
	[IPI_FORMAT]		= GRF_REG_FIELD(0x0000,  0,  3),
};

static const u32 rk3588_dsi1_grf_reg_fields[MAX_FIELDS] = {
	[TXREQCLKHS_EN]		= GRF_REG_FIELD(0x0004, 11, 11),
	[GATING_EN]		= GRF_REG_FIELD(0x0004, 10, 10),
	[IPI_SHUTDN]		= GRF_REG_FIELD(0x0004,  9,  9),
	[IPI_COLORM]		= GRF_REG_FIELD(0x0004,  8,  8),
	[IPI_COLOR_DEPTH]	= GRF_REG_FIELD(0x0004,  4,  7),
	[IPI_FORMAT]		= GRF_REG_FIELD(0x0004,  0,  3),
};

static const struct dw_mipi_dsi2_plat_data rk3588_mipi_dsi2_plat_data = {
	.dsi0_grf_reg_fields = rk3588_dsi0_grf_reg_fields,
	.dsi1_grf_reg_fields = rk3588_dsi1_grf_reg_fields,
	.dphy_max_bit_rate_per_lane = 4500000000ULL,
	.cphy_max_symbol_rate_per_lane = 2000000000ULL,
};

static const struct udevice_id dw_mipi_dsi2_ids[] = {
	{
		.compatible = "rockchip,rk3588-mipi-dsi2",
		.data = (ulong)&rk3588_mipi_dsi2_plat_data,
	},
	{}
};

static ssize_t dw_mipi_dsi2_host_transfer(struct mipi_dsi_host *host,
					 const struct mipi_dsi_msg *msg)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(host->dev);

	return dw_mipi_dsi2_transfer(dsi2, msg);
}

static int dw_mipi_dsi2_host_attach(struct mipi_dsi_host *host,
				   struct mipi_dsi_device *device)
{
	struct dw_mipi_dsi2 *dsi2 = dev_get_priv(host->dev);

	if (device->lanes < 1 || device->lanes > 8)
		return -EINVAL;

	dsi2->lanes = device->lanes;
	dsi2->channel = device->channel;
	dsi2->format = device->format;
	dsi2->mode_flags = device->mode_flags;

	return 0;
}

static const struct mipi_dsi_host_ops dw_mipi_dsi2_host_ops = {
	.attach = dw_mipi_dsi2_host_attach,
	.transfer = dw_mipi_dsi2_host_transfer,
};

static int dw_mipi_dsi2_bind(struct udevice *dev)
{
	struct mipi_dsi_host *host = dev_get_platdata(dev);

	host->dev = dev;
	host->ops = &dw_mipi_dsi2_host_ops;

	return dm_scan_fdt_dev(dev);
}

U_BOOT_DRIVER(dw_mipi_dsi2) = {
	.name = "dw_mipi_dsi2",
	.id = UCLASS_DISPLAY,
	.of_match = dw_mipi_dsi2_ids,
	.probe = dw_mipi_dsi2_probe,
	.bind = dw_mipi_dsi2_bind,
	.priv_auto_alloc_size = sizeof(struct dw_mipi_dsi2),
	.platdata_auto_alloc_size = sizeof(struct mipi_dsi_host),
};
