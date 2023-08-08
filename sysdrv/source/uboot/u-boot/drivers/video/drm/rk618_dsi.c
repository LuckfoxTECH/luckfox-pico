// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd
 *
 * Author: Wyon Bi <bivvy.bi@rock-chips.com>
 */

#include <common.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <drm/drm_mipi_dsi.h>
#include <video_bridge.h>
#include <asm/unaligned.h>
#include <linux/math64.h>

#include "rockchip_display.h"
#include "rockchip_bridge.h"
#include "rk618.h"

/* host registers */
#define HOSTREG(x)		((x) + 0x1000)
#define DSI_VERSION		HOSTREG(0x0000)
#define DSI_PWR_UP		HOSTREG(0x0004)
#define SHUTDOWNZ		BIT(0)
#define POWER_UP		BIT(0)
#define RESET			0
#define DSI_CLKMGR_CFG		HOSTREG(0x0008)
#define TO_CLK_DIVIDSION(x)	UPDATE(x, 15, 8)
#define TX_ESC_CLK_DIVIDSION(x)	UPDATE(x, 7, 0)
#define DSI_DPI_CFG		HOSTREG(0x000c)
#define EN18_LOOSELY		BIT(10)
#define COLORM_ACTIVE_LOW	BIT(9)
#define SHUTD_ACTIVE_LOW	BIT(8)
#define HSYNC_ACTIVE_LOW	BIT(7)
#define VSYNC_ACTIVE_LOW	BIT(6)
#define DATAEN_ACTIVE_LOW	BIT(5)
#define DPI_COLOR_CODING(x)	UPDATE(x, 4, 2)
#define DPI_VID(x)		UPDATE(x, 1, 0)
#define DSI_PCKHDL_CFG		HOSTREG(0x0018)
#define GEN_VID_RX(x)		UPDATE(x, 6, 5)
#define EN_CRC_RX		BIT(4)
#define EN_ECC_RX		BIT(3)
#define EN_BTA			BIT(2)
#define EN_EOTP_RX		BIT(1)
#define EN_EOTP_TX		BIT(0)
#define DSI_VID_MODE_CFG	HOSTREG(0x001c)
#define LPCMDEN			BIT(12)
#define FRAME_BTA_ACK		BIT(11)
#define EN_NULL_PKT		BIT(10)
#define EN_MULTI_PKT		BIT(9)
#define EN_LP_HFP		BIT(8)
#define EN_LP_HBP		BIT(7)
#define EN_LP_VACT		BIT(6)
#define EN_LP_VFP		BIT(5)
#define EN_LP_VBP		BIT(4)
#define EN_LP_VSA		BIT(3)
#define VID_MODE_TYPE(x)	UPDATE(x, 2, 1)
#define EN_VIDEO_MODE		BIT(0)
#define DSI_VID_PKT_CFG		HOSTREG(0x0020)
#define NULL_PKT_SIZE(x)	UPDATE(x, 30, 21)
#define NUM_CHUNKS(x)		UPDATE(x, 20, 11)
#define VID_PKT_SIZE(x)		UPDATE(x, 10, 0)
#define DSI_CMD_MODE_CFG	HOSTREG(0x0024)
#define TEAR_FX_EN		BIT(14)
#define ACK_RQST_EN		BIT(13)
#define DCS_LW_TX		BIT(12)
#define GEN_LW_TX		BIT(11)
#define MAX_RD_PKT_SIZE		BIT(10)
#define DCS_SR_0P_TX		BIT(9)
#define DCS_SW_1P_TX		BIT(8)
#define DCS_SW_0P_TX		BIT(7)
#define GEN_SR_2P_TX		BIT(6)
#define GEN_SR_1P_TX		BIT(5)
#define GEN_SR_0P_TX		BIT(4)
#define GEN_SW_2P_TX		BIT(3)
#define GEN_SW_1P_TX		BIT(2)
#define GEN_SW_0P_TX		BIT(1)
#define EN_CMD_MODE		BIT(0)
#define DSI_TMR_LINE_CFG	HOSTREG(0x0028)
#define HLINE_TIME(x)		UPDATE(x, 31, 18)
#define HBP_TIME(x)		UPDATE(x, 17, 9)
#define HSA_TIME(x)		UPDATE(x, 8, 0)
#define DSI_VTIMING_CFG		HOSTREG(0x002c)
#define V_ACTIVE_LINES(x)	UPDATE(x, 26, 16)
#define VFP_LINES(x)		UPDATE(x, 15, 10)
#define VBP_LINES(x)		UPDATE(x, 9, 4)
#define VSA_LINES(x)		UPDATE(x, 3, 0)
#define DSI_PHY_TMR_CFG		HOSTREG(0x0030)
#define PHY_HS2LP_TIME(x)	UPDATE(x, 31, 24)
#define PHY_LP2HS_TIME(x)	UPDATE(x, 23, 16)
#define MAX_RD_TIME(x)		UPDATE(x, 14, 0)
#define DSI_GEN_HDR		HOSTREG(0x0034)
#define DSI_GEN_PLD_DATA	HOSTREG(0x0038)
#define DSI_GEN_PKT_STATUS	HOSTREG(0x003c)
#define GEN_RD_CMD_BUSY		BIT(6)
#define GEN_PLD_R_FULL		BIT(5)
#define GEN_PLD_R_EMPTY		BIT(4)
#define GEN_PLD_W_FULL		BIT(3)
#define GEN_PLD_W_EMPTY		BIT(2)
#define GEN_CMD_FULL		BIT(1)
#define GEN_CMD_EMPTY		BIT(0)
#define DSI_TO_CNT_CFG		HOSTREG(0x0040)
#define LPRX_TO_CNT(x)		UPDATE(x, 31, 16)
#define HSTX_TO_CNT(x)		UPDATE(x, 15, 0)
#define DSI_INT_ST0		HOSTREG(0x0044)
#define DSI_INT_ST1		HOSTREG(0x0048)
#define DSI_INT_MSK0		HOSTREG(0x004c)
#define DSI_INT_MSK1		HOSTREG(0x0050)
#define DSI_PHY_RSTZ		HOSTREG(0x0054)
#define PHY_ENABLECLK		BIT(2)
#define DSI_PHY_IF_CFG		HOSTREG(0x0058)
#define PHY_STOP_WAIT_TIME(x)	UPDATE(x, 9, 2)
#define N_LANES(x)		UPDATE(x, 1, 0)
#define DSI_PHY_IF_CTRL		HOSTREG(0x005c)
#define PHY_TX_TRIGGERS(x)	UPDATE(x, 8, 5)
#define PHY_TXEXITULPSLAN	BIT(4)
#define PHY_TXREQULPSLAN	BIT(3)
#define PHY_TXEXITULPSCLK	BIT(2)
#define PHY_RXREQULPSCLK	BIT(1)
#define PHY_TXREQUESCLKHS	BIT(0)
#define DSI_PHY_STATUS		HOSTREG(0x0060)
#define ULPSACTIVENOT3LANE	BIT(12)
#define PHYSTOPSTATE3LANE	BIT(11)
#define ULPSACTIVENOT2LANE	BIT(10)
#define PHYSTOPSTATE2LANE	BIT(9)
#define ULPSACTIVENOT1LANE	BIT(8)
#define PHYSTOPSTATE1LANE	BIT(7)
#define RXULPSESC0LANE		BIT(6)
#define ULPSACTIVENOT0LANE	BIT(5)
#define PHYSTOPSTATE0LANE	BIT(4)
#define PHYULPSACTIVENOTCLK	BIT(3)
#define PHYSTOPSTATECLKLANE	BIT(2)
#define PHYSTOPSTATELANE	(PHYSTOPSTATE0LANE | PHYSTOPSTATECLKLANE)
#define PHYDIRECTION		BIT(1)
#define PHYLOCK			BIT(0)
#define DSI_LP_CMD_TIM		HOSTREG(0x0070)
#define OUTVACT_LPCMD_TIME(x)	UPDATE(x, 15, 8)
#define INVACT_LPCMD_TIME(x)	UPDATE(x, 7, 0)
#define DSI_MAX_REGISTER	DSI_LP_CMD_TIM

/* phy registers */
#define PHYREG(x)		((x) + 0x0c00)
#define MIPI_PHY_REG0		PHYREG(0x0000)
#define LANE_EN_MASK		GENMASK(6, 2)
#define LANE_EN_CK		BIT(6)
#define MIPI_PHY_REG1		PHYREG(0x0004)
#define REG_DA_PPFC		BIT(4)
#define REG_DA_SYNCRST		BIT(2)
#define REG_DA_LDOPD		BIT(1)
#define REG_DA_PLLPD		BIT(0)
#define MIPI_PHY_REG3		PHYREG(0x000c)
#define REG_FBDIV_HI_MASK	GENMASK(5, 5)
#define REG_FBDIV_HI(x)		UPDATE(x, 5, 5)
#define REG_PREDIV_MASK		GENMASK(4, 0)
#define REG_PREDIV(x)		UPDATE(x, 4, 0)
#define MIPI_PHY_REG4		PHYREG(0x0010)
#define REG_FBDIV_LO_MASK	GENMASK(7, 0)
#define REG_FBDIV_LO(x)		UPDATE(x, 7, 0)
#define MIPI_PHY_REG5		PHYREG(0x0014)
#define MIPI_PHY_REG6		PHYREG(0x0018)
#define MIPI_PHY_REG7		PHYREG(0x001c)
#define MIPI_PHY_REG9		PHYREG(0x0024)
#define MIPI_PHY_REG20		PHYREG(0x0080)
#define REG_DIG_RSTN		BIT(0)
#define MIPI_PHY_MAX_REGISTER	PHYREG(0x0348)

#define THS_SETTLE_OFFSET	0x00
#define THS_SETTLE_MASK		GENMASK(3, 0)
#define THS_SETTLE(x)		UPDATE(x, 3, 0)
#define TLPX_OFFSET		0x14
#define TLPX_MASK		GENMASK(5, 0)
#define TLPX(x)			UPDATE(x, 5, 0)
#define THS_PREPARE_OFFSET	0x18
#define THS_PREPARE_MASK	GENMASK(6, 0)
#define THS_PREPARE(x)		UPDATE(x, 6, 0)
#define THS_ZERO_OFFSET		0x1c
#define THS_ZERO_MASK		GENMASK(5, 0)
#define THS_ZERO(x)		UPDATE(x, 5, 0)
#define THS_TRAIL_OFFSET	0x20
#define THS_TRAIL_MASK		GENMASK(6, 0)
#define THS_TRAIL(x)		UPDATE(x, 6, 0)
#define THS_EXIT_OFFSET		0x24
#define THS_EXIT_MASK		GENMASK(4, 0)
#define THS_EXIT(x)		UPDATE(x, 4, 0)
#define TCLK_POST_OFFSET	0x28
#define TCLK_POST_MASK		GENMASK(3, 0)
#define TCLK_POST(x)		UPDATE(x, 3, 0)
#define TWAKUP_HI_OFFSET	0x30
#define TWAKUP_HI_MASK		GENMASK(1, 0)
#define TWAKUP_HI(x)		UPDATE(x, 1, 0)
#define TWAKUP_LO_OFFSET	0x34
#define TWAKUP_LO_MASK		GENMASK(7, 0)
#define TWAKUP_LO(x)		UPDATE(x, 7, 0)
#define TCLK_PRE_OFFSET		0x38
#define TCLK_PRE_MASK		GENMASK(3, 0)
#define TCLK_PRE(x)		UPDATE(x, 3, 0)
#define TTA_GO_OFFSET		0x40
#define TTA_GO_MASK		GENMASK(5, 0)
#define TTA_GO(x)		UPDATE(x, 5, 0)
#define TTA_SURE_OFFSET		0x44
#define TTA_SURE_MASK		GENMASK(5, 0)
#define TTA_SURE(x)		UPDATE(x, 5, 0)
#define TTA_WAIT_OFFSET		0x48
#define TTA_WAIT_MASK		GENMASK(5, 0)
#define TTA_WAIT(x)		UPDATE(x, 5, 0)

#define USEC_PER_SEC		1000000L
#define USEC_PER_MSEC		1000L
#define PSEC_PER_NSEC		1000L
#define PSEC_PER_SEC		1000000000000LL

struct mipi_dphy {
	u8 prediv;
	u16 fbdiv;
	unsigned int rate;
};

struct rk618_dsi {
	struct udevice *dev;
	struct rk618 *parent;
	struct mipi_dphy phy;
	unsigned int channel;
	unsigned int lanes;
	enum mipi_dsi_pixel_format format;
	unsigned long mode_flags;
	struct drm_display_mode mode;
};

enum {
	NON_BURST_MODE_SYNC_PULSE,
	NON_BURST_MODE_SYNC_EVENT,
	BURST_MODE,
};

enum {
	PIXEL_COLOR_CODING_16BIT_1,
	PIXEL_COLOR_CODING_16BIT_2,
	PIXEL_COLOR_CODING_16BIT_3,
	PIXEL_COLOR_CODING_18BIT_1,
	PIXEL_COLOR_CODING_18BIT_2,
	PIXEL_COLOR_CODING_24BIT,
};

#define dsi_read_poll_timeout(dsi, addr, val, cond, sleep_us, timeout_us) \
({ \
	unsigned long timeout = timer_get_us() + (timeout_us); \
	for (;;) { \
		(val) = dsi_read(dsi, addr); \
		if (cond) \
			break; \
		if ((timeout_us) && time_after(timer_get_us(), timeout)) { \
			(val) = dsi_read(dsi, addr); \
			break; \
		} \
		if (sleep_us) \
			udelay(sleep_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

static inline int dsi_write(struct rk618_dsi *dsi, u32 reg, u32 val)
{
	return rk618_i2c_write(dsi->parent, reg, val);
}

static inline u32 dsi_read(struct rk618_dsi *dsi, u32 reg)
{
	u32 val;

	rk618_i2c_read(dsi->parent, reg, &val);

	return val;
}

static inline void dsi_update_bits(struct rk618_dsi *dsi,
				   u32 reg, u32 mask, u32 val)
{
	u32 orig, tmp;

	orig = dsi_read(dsi, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	dsi_write(dsi, reg, tmp);
}

static inline bool is_clk_lane(u32 offset)
{
	if (offset == 0x100)
		return true;

	return false;
}

static void rk618_dsi_set_hs_clk(struct rk618_dsi *dsi)
{
	const struct drm_display_mode *mode = &dsi->mode;
	struct mipi_dphy *phy = &dsi->phy;
	u32 fout, fref, prediv, fbdiv;
	u32 min_delta = UINT_MAX;
	unsigned int value;

	value = dev_read_u32_default(dsi->dev, "rockchip,lane-rate", 0);
	if (value > 0) {
		fout = value * USEC_PER_SEC;
	} else {
		int bpp = mipi_dsi_pixel_format_to_bpp(dsi->format);
		unsigned int lanes = dsi->lanes;
		u64 bandwidth;

		bandwidth = (u64)mode->clock * 1000 * bpp;
		do_div(bandwidth, lanes);

		/* take 1 / 0.9, since mbps must big than bandwidth of RGB */
		bandwidth *= 10;
		do_div(bandwidth, 9);

		do_div(bandwidth, USEC_PER_SEC);
		bandwidth *= USEC_PER_SEC;
		fout = bandwidth;
	}

	if (fout > 1000000000UL)
		fout = 1000000000UL;

	fref = clk_get_rate(&dsi->parent->clkin);

	for (prediv = 1; prediv <= 12; prediv++) {
		u64 tmp;
		u32 delta;

		if (fref % prediv)
			continue;

		tmp = (u64)fout * prediv;
		do_div(tmp, fref);
		fbdiv = tmp;

		if (fbdiv < 12 || fbdiv > 511)
			continue;

		if (fbdiv == 15)
			continue;

		tmp = (u64)fbdiv * fref;
		do_div(tmp, prediv);

		delta = abs(fout - tmp);
		if (!delta) {
			phy->rate = tmp;
			phy->prediv = prediv;
			phy->fbdiv = fbdiv;
			break;
		} else if (delta < min_delta) {
			phy->rate = tmp;
			phy->prediv = prediv;
			phy->fbdiv = fbdiv;
			min_delta = delta;
		}
	}
}

static void rk618_dsi_phy_power_off(struct rk618_dsi *dsi)
{
	dsi_update_bits(dsi, MIPI_PHY_REG0, LANE_EN_MASK, 0);
	dsi_update_bits(dsi, MIPI_PHY_REG1, REG_DA_LDOPD | REG_DA_PLLPD,
			REG_DA_LDOPD | REG_DA_PLLPD);
}

static void rk618_dsi_phy_power_on(struct rk618_dsi *dsi, u32 txclkesc)
{
	struct mipi_dphy *phy = &dsi->phy;
	u32 offset, value, index;
	const struct {
		unsigned int rate;
		u8 ths_settle;
		u8 ths_zero;
		u8 ths_trail;
	} timing_table[] = {
		{ 110000000, 0x00, 0x03, 0x0c},
		{ 150000000, 0x01, 0x04, 0x0d},
		{ 200000000, 0x02, 0x04, 0x11},
		{ 250000000, 0x03, 0x05, 0x14},
		{ 300000000, 0x04, 0x06, 0x18},
		{ 400000000, 0x05, 0x07, 0x1d},
		{ 500000000, 0x06, 0x08, 0x23},
		{ 600000000, 0x07, 0x0a, 0x29},
		{ 700000000, 0x08, 0x0b, 0x31},
		{ 800000000, 0x09, 0x0c, 0x34},
		{1000000000, 0x0a, 0x0f, 0x40},
	};
	u32 Ttxbyteclkhs, UI, Ttxddrclkhs, Ttxclkesc;
	u32 Tlpx, Ths_exit, Tclk_post, Tclk_pre, Ths_prepare;
	u32 Tta_go, Tta_sure, Tta_wait;

	Ttxbyteclkhs = div_u64(PSEC_PER_SEC, phy->rate / 8);
	UI = Ttxddrclkhs = div_u64(PSEC_PER_SEC, phy->rate);
	Ttxclkesc = div_u64(PSEC_PER_SEC, txclkesc);

	dsi_update_bits(dsi, MIPI_PHY_REG3, REG_FBDIV_HI_MASK |
			REG_PREDIV_MASK, REG_FBDIV_HI(phy->fbdiv >> 8) |
			REG_PREDIV(phy->prediv));
	dsi_update_bits(dsi, MIPI_PHY_REG4,
			REG_FBDIV_LO_MASK, REG_FBDIV_LO(phy->fbdiv));
	dsi_update_bits(dsi, MIPI_PHY_REG1, REG_DA_LDOPD | REG_DA_PLLPD, 0);

	dsi_update_bits(dsi, MIPI_PHY_REG0, LANE_EN_MASK,
			LANE_EN_CK | GENMASK(dsi->lanes - 1 + 2, 2));

	dsi_update_bits(dsi, MIPI_PHY_REG1, REG_DA_SYNCRST, REG_DA_SYNCRST);
	udelay(1);
	dsi_update_bits(dsi, MIPI_PHY_REG1, REG_DA_SYNCRST, 0);

	dsi_update_bits(dsi, MIPI_PHY_REG20, REG_DIG_RSTN, 0);
	udelay(1);
	dsi_update_bits(dsi, MIPI_PHY_REG20, REG_DIG_RSTN, REG_DIG_RSTN);

	/* XXX */
	dsi_write(dsi, MIPI_PHY_REG6, 0x11);
	dsi_write(dsi, MIPI_PHY_REG7, 0x11);
	dsi_write(dsi, MIPI_PHY_REG9, 0xcc);

	if (phy->rate < 800000000)
		dsi_update_bits(dsi, MIPI_PHY_REG1, REG_DA_PPFC, REG_DA_PPFC);
	else
		dsi_write(dsi, MIPI_PHY_REG5, 0x30);

	for (index = 0; index < ARRAY_SIZE(timing_table); index++)
		if (phy->rate <= timing_table[index].rate)
			break;

	if (index == ARRAY_SIZE(timing_table))
		--index;

	for (offset = 0x100; offset <= 0x300; offset += 0x80) {
		dsi_update_bits(dsi, PHYREG(offset + THS_SETTLE_OFFSET),
				THS_SETTLE_MASK,
				THS_SETTLE(timing_table[index].ths_settle));

		/*
		 * The value of counter for HS Tlpx Time
		 * Tlpx = Tpin_txbyteclkhs * value
		 */
		Tlpx = 60 * PSEC_PER_NSEC;
		value = DIV_ROUND_UP(Tlpx, Ttxbyteclkhs);
		Tlpx = Ttxbyteclkhs * value;
		dsi_update_bits(dsi, PHYREG(offset + TLPX_OFFSET),
				TLPX_MASK, TLPX(value));

		/*
		 * The value of counter for HS Ths-prepare
		 * For clock lane, Ths-prepare(38ns~95ns)
		 * For data lane, Ths-prepare(40ns+4UI~85ns+6UI)
		 * Ths-prepare = Ttxddrclkhs * value
		 */
		if (is_clk_lane(offset))
			Ths_prepare = 65 * PSEC_PER_NSEC;
		else
			Ths_prepare = 65 * PSEC_PER_NSEC + 4 * UI;

		value = DIV_ROUND_UP(Ths_prepare, Ttxddrclkhs);
		dsi_update_bits(dsi, PHYREG(offset + THS_PREPARE_OFFSET),
				THS_PREPARE_MASK, THS_PREPARE(value));

		dsi_update_bits(dsi, PHYREG(offset + THS_ZERO_OFFSET),
				THS_ZERO_MASK,
				THS_ZERO(timing_table[index].ths_zero));

		dsi_update_bits(dsi, PHYREG(offset + THS_TRAIL_OFFSET),
				THS_TRAIL_MASK,
				THS_TRAIL(timing_table[index].ths_trail));

		/*
		 * The value of counter for HS Ths-exit
		 * Ths-exit = Tpin_txbyteclkhs * value
		 */
		Ths_exit = 120 * PSEC_PER_NSEC;
		value = DIV_ROUND_UP(Ths_exit, Ttxbyteclkhs);
		dsi_update_bits(dsi, PHYREG(offset + THS_EXIT_OFFSET),
				THS_EXIT_MASK, THS_EXIT(value));

		/*
		 * The value of counter for HS Tclk-post
		 * Tclk-post = Ttxbyteclkhs * value
		 */
		Tclk_post = 70 * PSEC_PER_NSEC + 52 * UI;
		value = DIV_ROUND_UP(Tclk_post, Ttxbyteclkhs);
		dsi_update_bits(dsi, PHYREG(offset + TCLK_POST_OFFSET),
				TCLK_POST_MASK, TCLK_POST(value));

		/*
		 * The value of counter for HS Twakup
		 * Twakup for ulpm,
		 * Twakup = Tpin_sys_clk * value
		 */
		dsi_update_bits(dsi, PHYREG(offset + TWAKUP_HI_OFFSET),
				TWAKUP_HI_MASK, TWAKUP_HI(0x3));
		dsi_update_bits(dsi, PHYREG(offset + TWAKUP_LO_OFFSET),
				TWAKUP_LO_MASK, TWAKUP_LO(0xff));

		/*
		 * The value of counter for HS Tclk-pre
		 * Tclk-pre for clock lane
		 * Tclk-pre = Tpin_txbyteclkhs * value
		 */
		Tclk_pre = 8 * UI;
		value = DIV_ROUND_UP(Tclk_pre, Ttxbyteclkhs);
		dsi_update_bits(dsi, PHYREG(offset + TCLK_PRE_OFFSET),
				TCLK_PRE_MASK, TCLK_PRE(value));

		/*
		 * The value of counter for HS Tta-go
		 * Tta-go for turnaround
		 * Tta-go = Ttxclkesc * value
		 */
		Tta_go = 4 * Tlpx;
		value = DIV_ROUND_UP(Tta_go, Ttxclkesc);
		dsi_update_bits(dsi, PHYREG(offset + TTA_GO_OFFSET),
				TTA_GO_MASK, TTA_GO(value));

		/*
		 * The value of counter for HS Tta-sure
		 * Tta-sure for turnaround
		 * Tta-sure = Ttxclkesc * value
		 */
		Tta_sure = 2 * Tlpx;
		value = DIV_ROUND_UP(Tta_sure, Ttxclkesc);
		dsi_update_bits(dsi, PHYREG(offset + TTA_SURE_OFFSET),
				TTA_SURE_MASK, TTA_SURE(value));

		/*
		 * The value of counter for HS Tta-wait
		 * Tta-wait for turnaround
		 * Interval from receiving ppi turnaround request to
		 * sending esc request.
		 * Tta-wait = Ttxclkesc * value
		 */
		Tta_wait = 5 * Tlpx;
		value = DIV_ROUND_UP(Tta_wait, Ttxclkesc);
		dsi_update_bits(dsi, PHYREG(offset + TTA_WAIT_OFFSET),
				TTA_WAIT_MASK, TTA_WAIT(value));
	}
}

static int rk618_dsi_pre_enable(struct rk618_dsi *dsi)
{
	struct drm_display_mode *mode = &dsi->mode;
	u32 esc_clk_div, txclkesc;
	u32 lanebyteclk, dpipclk;
	u32 hsw, hbp, vsw, vfp, vbp;
	u32 hsa_time, hbp_time, hline_time;
	u32 value;
	int ret;

	rk618_dsi_set_hs_clk(dsi);

	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, RESET);

	/* Configuration of the internal clock dividers */
	esc_clk_div = DIV_ROUND_UP(dsi->phy.rate >> 3, 20000000);
	txclkesc = dsi->phy.rate >> 3 / esc_clk_div;
	value = TO_CLK_DIVIDSION(10) | TX_ESC_CLK_DIVIDSION(esc_clk_div);
	dsi_write(dsi, DSI_CLKMGR_CFG, value);

	/* The DPI interface configuration */
	value = DPI_VID(dsi->channel);

	if (mode->flags & DRM_MODE_FLAG_NVSYNC)
		value |= VSYNC_ACTIVE_LOW;
	if (mode->flags & DRM_MODE_FLAG_NHSYNC)
		value |= HSYNC_ACTIVE_LOW;

	switch (dsi->format) {
	case MIPI_DSI_FMT_RGB666:
		value |= DPI_COLOR_CODING(PIXEL_COLOR_CODING_18BIT_2);
		break;
	case MIPI_DSI_FMT_RGB666_PACKED:
		value |= DPI_COLOR_CODING(PIXEL_COLOR_CODING_18BIT_1);
		value |= EN18_LOOSELY;
		break;
	case MIPI_DSI_FMT_RGB565:
		value |= DPI_COLOR_CODING(PIXEL_COLOR_CODING_16BIT_1);
		break;
	case MIPI_DSI_FMT_RGB888:
	default:
		value |= DPI_COLOR_CODING(PIXEL_COLOR_CODING_24BIT);
		break;
	}

	dsi_write(dsi, DSI_DPI_CFG, value);

	/* Packet handler configuration */
	value = GEN_VID_RX(dsi->channel) | EN_CRC_RX | EN_ECC_RX | EN_BTA;

	if (!(dsi->mode_flags & MIPI_DSI_MODE_EOT_PACKET))
		value |= EN_EOTP_TX;

	dsi_write(dsi, DSI_PCKHDL_CFG, value);

	/* Video mode configuration */
	value = EN_LP_VACT | EN_LP_VBP | EN_LP_VFP | EN_LP_VSA;

	if (!(dsi->mode_flags & MIPI_DSI_MODE_VIDEO_HFP))
		value |= EN_LP_HFP;

	if (!(dsi->mode_flags & MIPI_DSI_MODE_VIDEO_HBP))
		value |= EN_LP_HBP;

	if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO_BURST)
		value |= VID_MODE_TYPE(BURST_MODE);
	else if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO_SYNC_PULSE)
		value |= VID_MODE_TYPE(NON_BURST_MODE_SYNC_PULSE);
	else
		value |= VID_MODE_TYPE(NON_BURST_MODE_SYNC_EVENT);

	dsi_write(dsi, DSI_VID_MODE_CFG, value);

	/* Video packet configuration */
	dsi_write(dsi, DSI_VID_PKT_CFG, VID_PKT_SIZE(mode->hdisplay));

	/* Timeout timers configuration */
	dsi_write(dsi, DSI_TO_CNT_CFG, LPRX_TO_CNT(1000) | HSTX_TO_CNT(1000));

	hsw = mode->hsync_end - mode->hsync_start;
	hbp = mode->htotal - mode->hsync_end;
	vsw = mode->vsync_end - mode->vsync_start;
	vfp = mode->vsync_start - mode->vdisplay;
	vbp = mode->vtotal - mode->vsync_end;

	/* Line timing configuration */
	lanebyteclk = (dsi->phy.rate >> 3) / USEC_PER_SEC;
	dpipclk = mode->clock / USEC_PER_MSEC;
	hline_time = DIV_ROUND_UP(mode->htotal * lanebyteclk, dpipclk);
	hbp_time = DIV_ROUND_UP(hbp * lanebyteclk, dpipclk);
	hsa_time = DIV_ROUND_UP(hsw * lanebyteclk, dpipclk);
	dsi_write(dsi, DSI_TMR_LINE_CFG, HLINE_TIME(hline_time) |
		  HBP_TIME(hbp_time) | HSA_TIME(hsa_time));

	/* Vertical timing configuration */
	dsi_write(dsi, DSI_VTIMING_CFG,
		  V_ACTIVE_LINES(mode->vdisplay) | VFP_LINES(vfp) |
		  VBP_LINES(vbp) | VSA_LINES(vsw));

	/* D-PHY interface configuration */
	value = N_LANES(dsi->lanes - 1) | PHY_STOP_WAIT_TIME(0x20);
	dsi_write(dsi, DSI_PHY_IF_CFG, value);

	/* D-PHY timing configuration */
	value = PHY_HS2LP_TIME(20) | PHY_LP2HS_TIME(16) | MAX_RD_TIME(10000);
	dsi_write(dsi, DSI_PHY_TMR_CFG, value);

	/* enables the D-PHY Clock Lane Module */
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_ENABLECLK, PHY_ENABLECLK);

	dsi_update_bits(dsi, DSI_VID_MODE_CFG, EN_VIDEO_MODE, 0);
	dsi_update_bits(dsi, DSI_CMD_MODE_CFG, EN_CMD_MODE, EN_CMD_MODE);

	rk618_dsi_phy_power_on(dsi, txclkesc);

	/* wait for the PHY to acquire lock */
	ret = dsi_read_poll_timeout(dsi, DSI_PHY_STATUS,
				    value, value & PHYLOCK, 50, 1000);
	if (ret) {
		dev_err(dsi->dev, "PHY is not locked\n");
		return ret;
	}

	/* wait for the lane go to the stop state */
	ret = dsi_read_poll_timeout(dsi, DSI_PHY_STATUS,
				    value, value & PHYSTOPSTATELANE, 50, 1000);
	if (ret) {
		dev_err(dsi->dev, "lane module is not in stop state\n");
		return ret;
	}

	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, POWER_UP);

	return 0;
}

static void rk618_dsi_enable(struct rk618_dsi *dsi)
{
	/* controls the D-PHY PPI txrequestclkhs signal */
	dsi_update_bits(dsi, DSI_PHY_IF_CTRL,
			PHY_TXREQUESCLKHS, PHY_TXREQUESCLKHS);

	/* enables the DPI Video mode transmission */
	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, RESET);
	dsi_update_bits(dsi, DSI_CMD_MODE_CFG, EN_CMD_MODE, 0);
	dsi_update_bits(dsi, DSI_VID_MODE_CFG, EN_VIDEO_MODE, EN_VIDEO_MODE);
	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, POWER_UP);

	printf("final DSI-Link bandwidth: %lu x %d Mbps\n",
	       dsi->phy.rate / USEC_PER_SEC, dsi->lanes);
}

static void rk618_dsi_disable(struct rk618_dsi *dsi)
{
	/* enables the Command mode protocol for transmissions */
	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, RESET);
	dsi_update_bits(dsi, DSI_PHY_IF_CTRL, PHY_TXREQUESCLKHS, 0);
	dsi_update_bits(dsi, DSI_VID_MODE_CFG, EN_VIDEO_MODE, 0);
	dsi_update_bits(dsi, DSI_CMD_MODE_CFG, EN_CMD_MODE, EN_CMD_MODE);
	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, POWER_UP);
}

static void rk618_dsi_post_disable(struct rk618_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PWR_UP, SHUTDOWNZ, RESET);
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_ENABLECLK, 0);

	rk618_dsi_phy_power_off(dsi);
}

static void rk618_dsi_bridge_pre_enable(struct rockchip_bridge *bridge)
{
	struct rk618_dsi *dsi = dev_get_priv(bridge->dev);

	rk618_dsi_pre_enable(dsi);
}

static void rk618_dsi_bridge_enable(struct rockchip_bridge *bridge)
{
	struct rk618_dsi *dsi = dev_get_priv(bridge->dev);

	rk618_dsi_enable(dsi);
}

static void rk618_dsi_bridge_post_disable(struct rockchip_bridge *bridge)
{
	struct rk618_dsi *dsi = dev_get_priv(bridge->dev);

	rk618_dsi_post_disable(dsi);
}

static void rk618_dsi_bridge_disable(struct rockchip_bridge *bridge)
{
	struct rk618_dsi *dsi = dev_get_priv(bridge->dev);

	rk618_dsi_disable(dsi);
}

static void rk618_dsi_bridge_mode_set(struct rockchip_bridge *bridge,
				      const struct drm_display_mode *mode)
{
	struct rk618_dsi *dsi = dev_get_priv(bridge->dev);

	memcpy(&dsi->mode, mode, sizeof(*mode));
}

static const struct rockchip_bridge_funcs rk618_dsi_bridge_funcs = {
	.enable = rk618_dsi_bridge_enable,
	.disable = rk618_dsi_bridge_disable,
	.pre_enable = rk618_dsi_bridge_pre_enable,
	.post_disable = rk618_dsi_bridge_post_disable,
	.mode_set = rk618_dsi_bridge_mode_set,
};

static ssize_t rk618_dsi_transfer(struct rk618_dsi *dsi,
				  const struct mipi_dsi_msg *msg)
{
	struct mipi_dsi_packet packet;
	u32 value, mask;
	int ret;

	if (msg->flags & MIPI_DSI_MSG_USE_LPM)
		dsi_update_bits(dsi, DSI_PHY_IF_CTRL, PHY_TXREQUESCLKHS, 0);
	else
		dsi_update_bits(dsi, DSI_PHY_IF_CTRL,
				PHY_TXREQUESCLKHS, PHY_TXREQUESCLKHS);

	switch (msg->type) {
	case MIPI_DSI_DCS_SHORT_WRITE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_SW_0P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				DCS_SW_0P_TX : 0);
		break;
	case MIPI_DSI_DCS_SHORT_WRITE_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_SW_1P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				DCS_SW_1P_TX : 0);
		break;
	case MIPI_DSI_DCS_LONG_WRITE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_LW_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				DCS_LW_TX : 0);
		break;
	case MIPI_DSI_DCS_READ:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_SR_0P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				DCS_SR_0P_TX : 0);
		break;
	case MIPI_DSI_SET_MAXIMUM_RETURN_PACKET_SIZE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG,
				MAX_RD_PKT_SIZE,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				MAX_RD_PKT_SIZE : 0);
		break;
	case MIPI_DSI_GENERIC_SHORT_WRITE_0_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SW_0P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_SW_0P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_SHORT_WRITE_1_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SW_1P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_SW_1P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SW_2P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_SW_2P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_LONG_WRITE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_LW_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_LW_TX : 0);
		break;
	case MIPI_DSI_GENERIC_READ_REQUEST_0_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SR_0P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_SR_0P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_READ_REQUEST_1_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SR_1P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_SR_1P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_READ_REQUEST_2_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SR_2P_TX,
				msg->flags & MIPI_DSI_MSG_USE_LPM ?
				GEN_SR_2P_TX : 0);
		break;
	default:
		return -EINVAL;
	}

	/* create a packet to the DSI protocol */
	ret = mipi_dsi_create_packet(&packet, msg);
	if (ret) {
		dev_err(dsi->dev, "failed to create packet: %d\n", ret);
		return ret;
	}

	/* Send payload */
	while (packet.payload_length >= 4) {
		mask = GEN_PLD_W_FULL;
		ret = dsi_read_poll_timeout(dsi, DSI_GEN_PKT_STATUS,
					    value, !(value & mask), 50, 1000);
		if (ret) {
			dev_err(dsi->dev, "Write payload FIFO is full\n");
			return ret;
		}

		value = get_unaligned_le32(packet.payload);
		dsi_write(dsi, DSI_GEN_PLD_DATA, value);
		packet.payload += 4;
		packet.payload_length -= 4;
	}

	value = 0;
	switch (packet.payload_length) {
	case 3:
		value |= packet.payload[2] << 16;
		/* Fall through */
	case 2:
		value |= packet.payload[1] << 8;
		/* Fall through */
	case 1:
		value |= packet.payload[0];
		dsi_write(dsi, DSI_GEN_PLD_DATA, value);
		break;
	}

	mask = GEN_CMD_FULL;
	ret = dsi_read_poll_timeout(dsi, DSI_GEN_PKT_STATUS,
				    value, !(value & mask), 50, 1000);
	if (ret) {
		dev_err(dsi->dev, "Command FIFO is full\n");
		return ret;
	}

	/* Send packet header */
	value = get_unaligned_le32(packet.header);
	dsi_write(dsi, DSI_GEN_HDR, value);

	mask = GEN_PLD_W_EMPTY | GEN_CMD_EMPTY;
	ret = dsi_read_poll_timeout(dsi, DSI_GEN_PKT_STATUS,
				    value, (value & mask) == mask, 50, 1000);
	if (ret) {
		dev_err(dsi->dev, "Write payload FIFO is not empty\n");
		return ret;
	}

	if (msg->rx_len) {
		u8 *payload = msg->rx_buf;
		u16 length;

		mask = GEN_RD_CMD_BUSY;
		ret = dsi_read_poll_timeout(dsi, DSI_GEN_PKT_STATUS,
					    value, !(value & mask), 50, 1000);
		if (ret) {
			dev_err(dsi->dev,
				"entire response is not stored in the FIFO\n");
			return ret;
		}

		/* Receive payload */
		for (length = msg->rx_len; length; length -= 4) {
			mask = GEN_PLD_R_EMPTY;
			ret = dsi_read_poll_timeout(dsi, DSI_GEN_PKT_STATUS,
						    value, !(value & mask),
						    50, 1000);
			if (ret) {
				dev_err(dsi->dev,
					"Read payload FIFO is empty\n");
				return ret;
			}

			value = dsi_read(dsi, DSI_GEN_PLD_DATA);

			switch (length) {
			case 3:
				payload[2] = (value >> 16) & 0xff;
				/* Fall through */
			case 2:
				payload[1] = (value >> 8) & 0xff;
				/* Fall through */
			case 1:
				payload[0] = value & 0xff;
				return length;
			}

			payload[0] = (value >>  0) & 0xff;
			payload[1] = (value >>  8) & 0xff;
			payload[2] = (value >> 16) & 0xff;
			payload[3] = (value >> 24) & 0xff;
			payload += 4;
		}
	}

	return packet.payload_length;
}

static int rk618_dsi_probe(struct udevice *dev)
{
	struct rk618_dsi *dsi = dev_get_priv(dev);
	struct rockchip_bridge *bridge =
		(struct rockchip_bridge *)dev_get_driver_data(dev);
	int ret;

	dsi->dev = dev;
	dsi->parent = dev_get_priv(dev->parent);

	ret = device_probe(dev->parent);
	if (ret)
		return ret;

	bridge->dev = dev;

	/* Mask all interrupts */
	dsi_write(dsi, DSI_INT_MSK0, 0xffffffff);
	dsi_write(dsi, DSI_INT_MSK1, 0xffffffff);

	return 0;
}

static struct rockchip_bridge rk618_dsi_driver_data = {
	.funcs = &rk618_dsi_bridge_funcs,
};

static const struct udevice_id rk618_dsi_ids[] = {
	{
		.compatible = "rockchip,rk618-dsi",
		.data = (ulong)&rk618_dsi_driver_data,
	},
	{}
};

static ssize_t rk618_dsi_host_transfer(struct mipi_dsi_host *host,
				       const struct mipi_dsi_msg *msg)
{
	struct rk618_dsi *dsi = dev_get_priv(host->dev);

	return rk618_dsi_transfer(dsi, msg);
}

static int rk618_dsi_host_attach(struct mipi_dsi_host *host,
				 struct mipi_dsi_device *device)
{
	struct rk618_dsi *dsi = dev_get_priv(host->dev);

	if (device->lanes < 1 || device->lanes > 4)
		return -EINVAL;

	dsi->lanes = device->lanes;
	dsi->channel = device->channel;
	dsi->format = device->format;
	dsi->mode_flags = device->mode_flags;

	return 0;
}

static const struct mipi_dsi_host_ops rk618_dsi_host_ops = {
	.attach = rk618_dsi_host_attach,
	.transfer = rk618_dsi_host_transfer,
};

static int rk618_dsi_bind(struct udevice *dev)
{
	struct mipi_dsi_host *host = dev_get_platdata(dev);

	host->dev = dev;
	host->ops = &rk618_dsi_host_ops;

	return dm_scan_fdt_dev(dev);
}

static int rk618_dsi_child_post_bind(struct udevice *dev)
{
	struct mipi_dsi_host *host = dev_get_platdata(dev->parent);
	struct mipi_dsi_device *device = dev_get_parent_platdata(dev);
	char name[20];

	sprintf(name, "%s.%d", host->dev->name, device->channel);
	device_set_name(dev, name);

	device->dev = dev;
	device->host = host;
	device->lanes = dev_read_u32_default(dev, "dsi,lanes", 4);
	device->format = dev_read_u32_default(dev, "dsi,format",
					      MIPI_DSI_FMT_RGB888);
	device->mode_flags = dev_read_u32_default(dev, "dsi,flags",
						  MIPI_DSI_MODE_VIDEO |
						  MIPI_DSI_MODE_VIDEO_BURST |
						  MIPI_DSI_MODE_VIDEO_HBP |
						  MIPI_DSI_MODE_LPM |
						  MIPI_DSI_MODE_EOT_PACKET);
	device->channel = dev_read_u32_default(dev, "reg", 0);

	return 0;
}

static int rk618_dsi_child_pre_probe(struct udevice *dev)
{
	struct mipi_dsi_device *device = dev_get_parent_platdata(dev);
	int ret;

	ret = mipi_dsi_attach(device);
	if (ret) {
		dev_err(dev, "mipi_dsi_attach() failed: %d\n", ret);
		return ret;
	}

	return 0;
}

U_BOOT_DRIVER(rk618_dsi) = {
	.name = "rk618_dsi",
	.id = UCLASS_VIDEO_BRIDGE,
	.of_match = rk618_dsi_ids,
	.probe = rk618_dsi_probe,
	.bind = rk618_dsi_bind,
	.priv_auto_alloc_size = sizeof(struct rk618_dsi),
	.per_child_platdata_auto_alloc_size = sizeof(struct mipi_dsi_device),
	.platdata_auto_alloc_size = sizeof(struct mipi_dsi_host),
	.child_post_bind = rk618_dsi_child_post_bind,
	.child_pre_probe = rk618_dsi_child_pre_probe,
};
