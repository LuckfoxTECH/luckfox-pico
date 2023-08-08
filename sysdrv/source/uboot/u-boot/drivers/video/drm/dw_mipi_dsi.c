/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
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
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>
#include <linux/iopoll.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_panel.h"
#include "rockchip_phy.h"

#define UPDATE(v, h, l)		(((v) << (l)) & GENMASK((h), (l)))

#define DSI_VERSION			0x00
#define DSI_PWR_UP			0x04
#define RESET				0
#define POWERUP				BIT(0)

#define DSI_CLKMGR_CFG			0x08
#define TO_CLK_DIVIDSION(div)		(((div) & 0xff) << 8)
#define TX_ESC_CLK_DIVIDSION(div)	(((div) & 0xff) << 0)

#define DSI_DPI_VCID			0x0c
#define DPI_VID(vid)			(((vid) & 0x3) << 0)

#define DSI_DPI_COLOR_CODING		0x10
#define EN18_LOOSELY			BIT(8)
#define DPI_COLOR_CODING_16BIT_1	0x0
#define DPI_COLOR_CODING_16BIT_2	0x1
#define DPI_COLOR_CODING_16BIT_3	0x2
#define DPI_COLOR_CODING_18BIT_1	0x3
#define DPI_COLOR_CODING_18BIT_2	0x4
#define DPI_COLOR_CODING_24BIT		0x5

#define DSI_DPI_CFG_POL			0x14
#define COLORM_ACTIVE_LOW		BIT(4)
#define SHUTD_ACTIVE_LOW		BIT(3)
#define HSYNC_ACTIVE_LOW		BIT(2)
#define VSYNC_ACTIVE_LOW		BIT(1)
#define DATAEN_ACTIVE_LOW		BIT(0)

#define DSI_DPI_LP_CMD_TIM		0x18
#define OUTVACT_LPCMD_TIME(p)		(((p) & 0xff) << 16)
#define INVACT_LPCMD_TIME(p)		((p) & 0xff)

#define DSI_DBI_VCID			0x1c
#define DBI_VCID(x)			UPDATE(x, 1, 0)
#define DSI_DBI_CFG			0x20
#define DSI_DBI_CMDSIZE			0x28

#define DSI_PCKHDL_CFG			0x2c
#define CRC_RX_EN			BIT(4)
#define ECC_RX_EN			BIT(3)
#define BTA_EN				BIT(2)
#define EOTP_RX_EN			BIT(1)
#define EOTP_TX_EN			BIT(0)
#define DSI_MODE_CFG			0x34
#define CMD_VIDEO_MODE			BIT(0)
#define COMMAND_MODE			BIT(0)
#define VIDEO_MODE			0
#define DSI_VID_MODE_CFG		0x38
#define VPG_EN				BIT(16)
#define LP_CMD_EN			BIT(15)
#define FRAME_BTA_ACK			BIT(14)
#define LP_HFP_EN			BIT(13)
#define LP_HBP_EN			BIT(12)
#define LP_VACT_EN			BIT(11)
#define LP_VFP_EN			BIT(10)
#define LP_VBP_EN			BIT(9)
#define LP_VSA_EN			BIT(8)
#define VID_MODE_TYPE_BURST_SYNC_PULSES	0x0
#define VID_MODE_TYPE_BURST_SYNC_EVENTS	0x1
#define VID_MODE_TYPE_BURST		0x2

#define DSI_VID_PKT_SIZE		0x3c
#define VID_PKT_SIZE(p)			(((p) & 0x3fff) << 0)
#define VID_PKT_MAX_SIZE		0x3fff

#define DSI_VID_NUM_CHUMKS		0x40
#define DSI_VID_NULL_PKT_SIZE		0x44
#define DSI_VID_HSA_TIME		0x48
#define DSI_VID_HBP_TIME		0x4c
#define DSI_VID_HLINE_TIME		0x50
#define DSI_VID_VSA_LINES		0x54
#define DSI_VID_VBP_LINES		0x58
#define DSI_VID_VFP_LINES		0x5c
#define DSI_VID_VACTIVE_LINES		0x60
#define DSI_EDPI_CMD_SIZE		0x64
#define DSI_CMD_MODE_CFG		0x68
#define MAX_RD_PKT_SIZE			BIT(24)
#define DCS_LW_TX			BIT(19)
#define DCS_SR_0P_TX			BIT(18)
#define DCS_SW_1P_TX			BIT(17)
#define DCS_SW_0P_TX			BIT(16)
#define GEN_LW_TX			BIT(14)
#define GEN_SR_2P_TX			BIT(13)
#define GEN_SR_1P_TX			BIT(12)
#define GEN_SR_0P_TX			BIT(11)
#define GEN_SW_2P_TX			BIT(10)
#define GEN_SW_1P_TX			BIT(9)
#define GEN_SW_0P_TX			BIT(8)
#define ACK_RQST_EN			BIT(1)
#define TEAR_FX_EN			BIT(0)

#define DSI_GEN_HDR			0x6c
#define GEN_HDATA(data)			(((data) & 0xffff) << 8)
#define GEN_HDATA_MASK			(0xffff << 8)
#define GEN_HTYPE(type)			(((type) & 0xff) << 0)
#define GEN_HTYPE_MASK			0xff

#define DSI_GEN_PLD_DATA		0x70

#define DSI_CMD_PKT_STATUS		0x74
#define GEN_CMD_EMPTY			BIT(0)
#define GEN_CMD_FULL			BIT(1)
#define GEN_PLD_W_EMPTY			BIT(2)
#define GEN_PLD_W_FULL			BIT(3)
#define GEN_PLD_R_EMPTY			BIT(4)
#define GEN_PLD_R_FULL			BIT(5)
#define GEN_RD_CMD_BUSY			BIT(6)

#define DSI_TO_CNT_CFG			0x78
#define HSTX_TO_CNT(p)			(((p) & 0xffff) << 16)
#define LPRX_TO_CNT(p)			((p) & 0xffff)

#define DSI_BTA_TO_CNT			0x8c
#define DSI_LPCLK_CTRL			0x94
#define AUTO_CLKLANE_CTRL		BIT(1)
#define PHY_TXREQUESTCLKHS		BIT(0)

#define DSI_PHY_TMR_LPCLK_CFG		0x98
#define PHY_CLKHS2LP_TIME(lbcc)		(((lbcc) & 0x3ff) << 16)
#define PHY_CLKLP2HS_TIME(lbcc)		((lbcc) & 0x3ff)

#define DSI_PHY_TMR_CFG			0x9c
#define PHY_HS2LP_TIME(lbcc)		(((lbcc) & 0xff) << 24)
#define PHY_LP2HS_TIME(lbcc)		(((lbcc) & 0xff) << 16)
#define MAX_RD_TIME(lbcc)		((lbcc) & 0x7fff)

#define DSI_PHY_RSTZ			0xa0
#define PHY_ENFORCEPLL			BIT(3)
#define PHY_ENABLECLK			BIT(2)
#define PHY_RSTZ			BIT(1)
#define PHY_SHUTDOWNZ			BIT(0)

#define DSI_PHY_IF_CFG			0xa4
#define N_LANES(n)			((((n) - 1) & 0x3) << 0)
#define PHY_STOP_WAIT_TIME(cycle)	(((cycle) & 0xff) << 8)

#define DSI_PHY_STATUS			0xb0
#define PHY_STOPSTATE0LANE		BIT(4)
#define PHY_STOPSTATECLKLANE		BIT(2)
#define PHY_LOCK			BIT(0)
#define PHY_STOPSTATELANE		(PHY_STOPSTATE0LANE | \
					 PHY_STOPSTATECLKLANE)

#define DSI_PHY_TST_CTRL0		0xb4
#define PHY_TESTCLK			BIT(1)
#define PHY_TESTCLR			BIT(0)

#define DSI_PHY_TST_CTRL1		0xb8
#define PHY_TESTEN			BIT(16)
#define PHY_TESTDOUT_SHIFT		8
#define PHY_TESTDIN_MASK		GENMASK(7, 0)
#define PHY_TESTDIN(x)			UPDATE(x, 7, 0)

#define DSI_INT_ST0			0xbc
#define DSI_INT_ST1			0xc0
#define DSI_INT_MSK0			0xc4
#define DSI_INT_MSK1			0xc8

#define PHY_STATUS_TIMEOUT_US		10000
#define CMD_PKT_STATUS_TIMEOUT_US	20000

/* Test Code: 0x44 (HS RX Control of Lane 0) */
#define HSFREQRANGE(x)			UPDATE(x, 6, 1)
/* Test Code: 0x17 (PLL Input Divider Ratio) */
#define INPUT_DIV(x)			UPDATE(x, 6, 0)
/* Test Code: 0x18 (PLL Loop Divider Ratio) */
#define FEEDBACK_DIV_LO(x)		UPDATE(x, 4, 0)
#define FEEDBACK_DIV_HI(x)		(BIT(7) | UPDATE(x, 3, 0))

#define GRF_REG_FIELD(reg, lsb, msb)	(((reg) << 10) | ((lsb) << 5) | (msb))

enum grf_reg_fields {
	DPIUPDATECFG,
	DPISHUTDN,
	DPICOLORM,
	VOPSEL,
	TURNREQUEST,
	TURNDISABLE,
	SKEWCALHS,
	FORCETXSTOPMODE,
	FORCERXMODE,
	ENABLE_N,
	MASTERSLAVEZ,
	ENABLECLK,
	BASEDIR,
	MAX_FIELDS,
};

struct dw_mipi_dsi_plat_data {
	const u32 *dsi0_grf_reg_fields;
	const u32 *dsi1_grf_reg_fields;
	unsigned long max_bit_rate_per_lane;
};

struct mipi_dphy {
	/* Non-SNPS PHY */
	struct rockchip_phy *phy;

	u16 input_div;
	u16 feedback_div;
};

struct dw_mipi_dsi {
	struct rockchip_connector connector;
	struct udevice *dev;
	void *base;
	void *grf;
	int id;
	struct dw_mipi_dsi *master;
	struct dw_mipi_dsi *slave;
	bool prepared;

	unsigned int lane_mbps; /* per lane */
	u32 channel;
	u32 lanes;
	u32 format;
	u32 mode_flags;
	struct mipi_dphy dphy;
	struct drm_display_mode mode;
	bool data_swap;

	const struct dw_mipi_dsi_plat_data *pdata;
};

static inline void dsi_write(struct dw_mipi_dsi *dsi, u32 reg, u32 val)
{
	writel(val, dsi->base + reg);
}

static inline u32 dsi_read(struct dw_mipi_dsi *dsi, u32 reg)
{
	return readl(dsi->base + reg);
}

static inline void dsi_update_bits(struct dw_mipi_dsi *dsi,
				   u32 reg, u32 mask, u32 val)
{
	u32 orig, tmp;

	orig = dsi_read(dsi, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	dsi_write(dsi, reg, tmp);
}

static void grf_field_write(struct dw_mipi_dsi *dsi, enum grf_reg_fields index,
			    unsigned int val)
{
	const u32 field = dsi->id ? dsi->pdata->dsi1_grf_reg_fields[index] :
			  dsi->pdata->dsi0_grf_reg_fields[index];
	u16 reg;
	u8 msb, lsb;

	if (!field)
		return;

	reg = (field >> 10) & 0x3ffff;
	lsb = (field >>  5) & 0x1f;
	msb = (field >>  0) & 0x1f;

	rk_clrsetreg(dsi->grf + reg, GENMASK(msb, lsb), val << lsb);
}

static inline void dpishutdn_assert(struct dw_mipi_dsi *dsi)
{
	grf_field_write(dsi, DPISHUTDN, 1);
}

static inline void dpishutdn_deassert(struct dw_mipi_dsi *dsi)
{
	grf_field_write(dsi, DPISHUTDN, 0);
}

static int genif_wait_w_pld_fifo_not_full(struct dw_mipi_dsi *dsi)
{
	u32 sts;
	int ret;

	ret = readl_poll_timeout(dsi->base + DSI_CMD_PKT_STATUS,
				 sts, !(sts & GEN_PLD_W_FULL),
				 CMD_PKT_STATUS_TIMEOUT_US);
	if (ret < 0) {
		printf("generic write payload fifo is full\n");
		return ret;
	}

	return 0;
}

static int genif_wait_cmd_fifo_not_full(struct dw_mipi_dsi *dsi)
{
	u32 sts;
	int ret;

	ret = readl_poll_timeout(dsi->base + DSI_CMD_PKT_STATUS,
				 sts, !(sts & GEN_CMD_FULL),
				 CMD_PKT_STATUS_TIMEOUT_US);
	if (ret < 0) {
		printf("generic write cmd fifo is full\n");
		return ret;
	}

	return 0;
}

static int genif_wait_write_fifo_empty(struct dw_mipi_dsi *dsi)
{
	u32 sts;
	u32 mask;
	int ret;

	mask = GEN_CMD_EMPTY | GEN_PLD_W_EMPTY;
	ret = readl_poll_timeout(dsi->base + DSI_CMD_PKT_STATUS,
				 sts, (sts & mask) == mask,
				 CMD_PKT_STATUS_TIMEOUT_US);
	if (ret < 0) {
		printf("generic write fifo is full\n");
		return ret;
	}

	return 0;
}

static inline void mipi_dphy_enableclk_assert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_ENABLECLK, PHY_ENABLECLK);
	udelay(1);
}

static inline void mipi_dphy_enableclk_deassert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_ENABLECLK, 0);
	udelay(1);
}

static inline void mipi_dphy_shutdownz_assert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_SHUTDOWNZ, 0);
	udelay(1);
}

static inline void mipi_dphy_shutdownz_deassert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_SHUTDOWNZ, PHY_SHUTDOWNZ);
	udelay(1);
}

static inline void mipi_dphy_rstz_assert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_RSTZ, 0);
	udelay(1);
}

static inline void mipi_dphy_rstz_deassert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_RSTZ, PHY_RSTZ, PHY_RSTZ);
	udelay(1);
}

static inline void testif_testclk_assert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL0, PHY_TESTCLK, PHY_TESTCLK);
	udelay(1);
}

static inline void testif_testclk_deassert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL0, PHY_TESTCLK, 0);
	udelay(1);
}

static inline void testif_testclr_assert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL0, PHY_TESTCLR, PHY_TESTCLR);
	udelay(1);
}

static inline void testif_testclr_deassert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL0, PHY_TESTCLR, 0);
	udelay(1);
}

static inline void testif_testen_assert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL1, PHY_TESTEN, PHY_TESTEN);
	udelay(1);
}

static inline void testif_testen_deassert(struct dw_mipi_dsi *dsi)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL1, PHY_TESTEN, 0);
	udelay(1);
}

static inline void testif_set_data(struct dw_mipi_dsi *dsi, u8 data)
{
	dsi_update_bits(dsi, DSI_PHY_TST_CTRL1,
			PHY_TESTDIN_MASK, PHY_TESTDIN(data));
	udelay(1);
}

static inline u8 testif_get_data(struct dw_mipi_dsi *dsi)
{
	return dsi_read(dsi, DSI_PHY_TST_CTRL1) >> PHY_TESTDOUT_SHIFT;
}

static void testif_test_code_write(struct dw_mipi_dsi *dsi, u8 test_code)
{
	testif_testclk_assert(dsi);
	testif_set_data(dsi, test_code);
	testif_testen_assert(dsi);
	testif_testclk_deassert(dsi);
	testif_testen_deassert(dsi);
}

static void testif_test_data_write(struct dw_mipi_dsi *dsi, u8 test_data)
{
	testif_testclk_deassert(dsi);
	testif_set_data(dsi, test_data);
	testif_testclk_assert(dsi);
}

static void testif_write(struct dw_mipi_dsi *dsi, u8 test_code, u8 test_data)
{
	testif_test_code_write(dsi, test_code);
	testif_test_data_write(dsi, test_data);

	dev_dbg(dsi->dev,
		"test_code=0x%02x, test_data=0x%02x, monitor_data=0x%02x\n",
		test_code, test_data, testif_get_data(dsi));
}

static int mipi_dphy_power_on(struct dw_mipi_dsi *dsi)
{
	u32 mask, val;
	int ret;

	mipi_dphy_shutdownz_deassert(dsi);
	mipi_dphy_rstz_deassert(dsi);
	mdelay(2);

	if (dsi->dphy.phy) {
		rockchip_phy_set_mode(dsi->dphy.phy, PHY_MODE_MIPI_DPHY);
		rockchip_phy_power_on(dsi->dphy.phy);
	}

	ret = readl_poll_timeout(dsi->base + DSI_PHY_STATUS,
				 val, val & PHY_LOCK, PHY_STATUS_TIMEOUT_US);
	if (ret < 0) {
		dev_err(dsi->dev, "PHY is not locked\n");
		return ret;
	}

	udelay(200);

	mask = PHY_STOPSTATELANE;
	ret = readl_poll_timeout(dsi->base + DSI_PHY_STATUS,
				 val, (val & mask) == mask,
				 PHY_STATUS_TIMEOUT_US);
	if (ret < 0) {
		dev_err(dsi->dev, "lane module is not in stop state\n");
		return ret;
	}

	udelay(10);

	return 0;
}

static void dw_mipi_dsi_phy_init(struct dw_mipi_dsi *dsi)
{
	/* Table 5-1 Frequency Ranges */
	const struct {
		unsigned long max_lane_mbps;
		u8 hsfreqrange;
	} hsfreqrange_table[] = {
		{  90, 0x00}, { 100, 0x10}, { 110, 0x20}, { 130, 0x01},
		{ 140, 0x11}, { 150, 0x21}, { 170, 0x02}, { 180, 0x12},
		{ 200, 0x22}, { 220, 0x03}, { 240, 0x13}, { 250, 0x23},
		{ 270, 0x04}, { 300, 0x14}, { 330, 0x05}, { 360, 0x15},
		{ 400, 0x25}, { 450, 0x06}, { 500, 0x16}, { 550, 0x07},
		{ 600, 0x17}, { 650, 0x08}, { 700, 0x18}, { 750, 0x09},
		{ 800, 0x19}, { 850, 0x29}, { 900, 0x39}, { 950, 0x0a},
		{1000, 0x1a}, {1050, 0x2a}, {1100, 0x3a}, {1150, 0x0b},
		{1200, 0x1b}, {1250, 0x2b}, {1300, 0x3b}, {1350, 0x0c},
		{1400, 0x1c}, {1450, 0x2c}, {1500, 0x3c}
	};
	u8 hsfreqrange, counter;
	unsigned int index, txbyteclkhs;
	u16 n, m;

	for (index = 0; index < ARRAY_SIZE(hsfreqrange_table); index++)
		if (dsi->lane_mbps <= hsfreqrange_table[index].max_lane_mbps)
			break;

	if (index == ARRAY_SIZE(hsfreqrange_table))
		--index;

	hsfreqrange = hsfreqrange_table[index].hsfreqrange;
	testif_write(dsi, 0x44, HSFREQRANGE(hsfreqrange));

	txbyteclkhs = dsi->lane_mbps >> 3;
	counter = txbyteclkhs * 60 / 1000;
	testif_write(dsi, 0x60, 0x80 | counter);
	testif_write(dsi, 0x70, 0x80 | counter);

	n = dsi->dphy.input_div - 1;
	m = dsi->dphy.feedback_div - 1;
	testif_write(dsi, 0x19, 0x30);
	testif_write(dsi, 0x17, INPUT_DIV(n));
	testif_write(dsi, 0x18, FEEDBACK_DIV_LO(m));
	testif_write(dsi, 0x18, FEEDBACK_DIV_HI(m >> 5));
}

static unsigned long dw_mipi_dsi_get_lane_rate(struct dw_mipi_dsi *dsi)
{
	const struct drm_display_mode *mode = &dsi->mode;
	unsigned long max_lane_rate = dsi->pdata->max_bit_rate_per_lane;
	unsigned long lane_rate;
	unsigned int value;
	int bpp, lanes;
	u64 tmp;

	/* optional override of the desired bandwidth */
	value = dev_read_u32_default(dsi->dev, "rockchip,lane-rate", 0);
	if (value > 0)
		return value * 1000 * 1000;

	bpp = mipi_dsi_pixel_format_to_bpp(dsi->format);
	if (bpp < 0)
		bpp = 24;

	lanes = dsi->slave ? dsi->lanes * 2 : dsi->lanes;
	tmp = (u64)mode->clock * 1000 * bpp;
	do_div(tmp, lanes);

	/* take 1 / 0.9, since mbps must big than bandwidth of RGB */
	tmp *= 10;
	do_div(tmp, 9);

	if (tmp > max_lane_rate)
		lane_rate = max_lane_rate;
	else
		lane_rate = tmp;

	return lane_rate;
}

static void dw_mipi_dsi_set_pll(struct dw_mipi_dsi *dsi, unsigned long rate)
{
	unsigned long fin, fout;
	unsigned long fvco_min, fvco_max, best_freq = 984000000;
	u8 min_prediv, max_prediv;
	u8 _prediv, best_prediv = 2;
	u16 _fbdiv, best_fbdiv = 82;
	u32 min_delta = ~0U;

	fin = 24000000;
	fout = rate;

	/* 5Mhz < Fref / N < 40MHz, 80MHz < Fvco < 1500Mhz */
	min_prediv = DIV_ROUND_UP(fin, 40000000);
	max_prediv = fin / 5000000;
	fvco_min = 80000000;
	fvco_max = 1500000000;

	for (_prediv = min_prediv; _prediv <= max_prediv; _prediv++) {
		u64 tmp, _fout;
		u32 delta;

		/* Fvco = Fref * M / N */
		tmp = (u64)fout * _prediv;
		do_div(tmp, fin);
		_fbdiv = tmp;

		/*
		 * Due to the use of a "by 2 pre-scaler," the range of the
		 * feedback multiplication value M is limited to even division
		 * numbers, and m must be greater than 12, less than 1000.
		 */
		if (_fbdiv <= 12 || _fbdiv >= 1000)
			continue;

		if (_fbdiv % 2)
			++_fbdiv;

		_fout = (u64)_fbdiv * fin;
		do_div(_fout, _prediv);

		if (_fout < fvco_min || _fout > fvco_max)
			continue;

		delta = abs(fout - _fout);
		if (!delta) {
			best_prediv = _prediv;
			best_fbdiv = _fbdiv;
			best_freq = _fout;
			break;
		} else if (delta < min_delta) {
			best_prediv = _prediv;
			best_fbdiv = _fbdiv;
			best_freq = _fout;
			min_delta = delta;
		}
	}

	dsi->lane_mbps = best_freq / 1000 / 1000;
	dsi->dphy.input_div = best_prediv;
	dsi->dphy.feedback_div = best_fbdiv;
	if (dsi->slave) {
		dsi->slave->lane_mbps = dsi->lane_mbps;
		dsi->slave->dphy.input_div = dsi->dphy.input_div;
		dsi->slave->dphy.feedback_div = dsi->dphy.feedback_div;
	}
	if (dsi->master) {
		dsi->master->lane_mbps = dsi->lane_mbps;
		dsi->master->dphy.input_div = dsi->dphy.input_div;
		dsi->master->dphy.feedback_div = dsi->dphy.feedback_div;
	}
}

static int dw_mipi_dsi_read_from_fifo(struct dw_mipi_dsi *dsi,
				      const struct mipi_dsi_msg *msg)
{
	u8 *payload = msg->rx_buf;
	u16 length;
	u32 val;
	int ret;

	ret = readl_poll_timeout(dsi->base + DSI_CMD_PKT_STATUS,
				 val, !(val & GEN_RD_CMD_BUSY), 5000);
	if (ret) {
		printf("entire response isn't stored in the FIFO\n");
		return ret;
	}

	/* Receive payload */
	for (length = msg->rx_len; length; length -= 4) {
		ret = readl_poll_timeout(dsi->base + DSI_CMD_PKT_STATUS,
					 val, !(val & GEN_PLD_R_EMPTY), 5000);
		if (ret) {
			printf("Read payload FIFO is empty\n");
			return ret;
		}

		val = dsi_read(dsi, DSI_GEN_PLD_DATA);

		switch (length) {
		case 3:
			payload[2] = (val >> 16) & 0xff;
			/* Fall through */
		case 2:
			payload[1] = (val >> 8) & 0xff;
			/* Fall through */
		case 1:
			payload[0] = val & 0xff;
			return 0;
		}

		payload[0] = (val >>  0) & 0xff;
		payload[1] = (val >>  8) & 0xff;
		payload[2] = (val >> 16) & 0xff;
		payload[3] = (val >> 24) & 0xff;
		payload += 4;
	}

	return 0;
}

static int dw_mipi_dsi_turn_on_peripheral(struct dw_mipi_dsi *dsi)
{
	dpishutdn_assert(dsi);
	udelay(20);
	dpishutdn_deassert(dsi);

	return 0;
}

static int dw_mipi_dsi_shutdown_peripheral(struct dw_mipi_dsi *dsi)
{
	dpishutdn_deassert(dsi);
	udelay(20);
	dpishutdn_assert(dsi);

	return 0;
}

static ssize_t dw_mipi_dsi_transfer(struct dw_mipi_dsi *dsi,
				    const struct mipi_dsi_msg *msg)
{
	struct mipi_dsi_packet packet;
	int ret;
	int val;

	if (msg->flags & MIPI_DSI_MSG_USE_LPM) {
		dsi_update_bits(dsi, DSI_VID_MODE_CFG, LP_CMD_EN, LP_CMD_EN);
		dsi_update_bits(dsi, DSI_LPCLK_CTRL, PHY_TXREQUESTCLKHS, 0);
	} else {
		dsi_update_bits(dsi, DSI_VID_MODE_CFG, LP_CMD_EN, 0);
		dsi_update_bits(dsi, DSI_LPCLK_CTRL,
				PHY_TXREQUESTCLKHS, PHY_TXREQUESTCLKHS);
	}

	switch (msg->type) {
	case MIPI_DSI_SHUTDOWN_PERIPHERAL:
		return dw_mipi_dsi_shutdown_peripheral(dsi);
	case MIPI_DSI_TURN_ON_PERIPHERAL:
		return dw_mipi_dsi_turn_on_peripheral(dsi);
	case MIPI_DSI_DCS_SHORT_WRITE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_SW_0P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				DCS_SW_0P_TX : 0);
		break;
	case MIPI_DSI_DCS_SHORT_WRITE_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_SW_1P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				DCS_SW_1P_TX : 0);
		break;
	case MIPI_DSI_DCS_LONG_WRITE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_LW_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				DCS_LW_TX : 0);
		break;
	case MIPI_DSI_DCS_READ:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_SR_0P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				DCS_SR_0P_TX : 0);
		break;
	case MIPI_DSI_SET_MAXIMUM_RETURN_PACKET_SIZE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, MAX_RD_PKT_SIZE,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				MAX_RD_PKT_SIZE : 0);
		break;
	case MIPI_DSI_GENERIC_SHORT_WRITE_0_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SW_0P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_SW_0P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_SHORT_WRITE_1_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SW_1P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_SW_1P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SW_2P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_SW_2P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_LONG_WRITE:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_LW_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_LW_TX : 0);
		break;
	case MIPI_DSI_GENERIC_READ_REQUEST_0_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SR_0P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_SR_0P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_READ_REQUEST_1_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SR_1P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_SR_1P_TX : 0);
		break;
	case MIPI_DSI_GENERIC_READ_REQUEST_2_PARAM:
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, GEN_SR_2P_TX,
				dsi->mode_flags & MIPI_DSI_MODE_LPM ?
				GEN_SR_2P_TX : 0);
		break;
	default:
		return -EINVAL;
	}

	if (msg->flags & MIPI_DSI_MSG_REQ_ACK)
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG,
				ACK_RQST_EN, ACK_RQST_EN);

	/* create a packet to the DSI protocol */
	ret = mipi_dsi_create_packet(&packet, msg);
	if (ret) {
		printf("failed to create packet: %d\n", ret);
		return ret;
	}

	/* Send payload */
	while (DIV_ROUND_UP(packet.payload_length, 4)) {
		/*
		 * Alternatively, you can always keep the FIFO
		 * nearly full by monitoring the FIFO state until
		 * it is not full, and then writea single word of data.
		 * This solution is more resource consuming
		 * but it simultaneously avoids FIFO starvation,
		 * making it possible to use FIFO sizes smaller than
		 * the amount of data of the longest packet to be written.
		 */
		ret = genif_wait_w_pld_fifo_not_full(dsi);
		if (ret)
			return ret;

		if (packet.payload_length < 4) {
			/* send residu payload */
			val = 0;
			memcpy(&val, packet.payload, packet.payload_length);
			dsi_write(dsi, DSI_GEN_PLD_DATA, val);
			packet.payload_length = 0;
		} else {
			val = get_unaligned_le32(packet.payload);
			dsi_write(dsi, DSI_GEN_PLD_DATA, val);
			packet.payload += 4;
			packet.payload_length -= 4;
		}
	}

	ret = genif_wait_cmd_fifo_not_full(dsi);
	if (ret)
		return ret;

	/* Send packet header */
	val = get_unaligned_le32(packet.header);
	dsi_write(dsi, DSI_GEN_HDR, val);

	ret = genif_wait_write_fifo_empty(dsi);
	if (ret)
		return ret;

	if (msg->rx_len) {
		ret = dw_mipi_dsi_read_from_fifo(dsi, msg);
		if (ret < 0)
			return ret;
	}

	if (dsi->slave) {
		ret = dw_mipi_dsi_transfer(dsi->slave, msg);
		if (ret < 0)
			return ret;
	}

	return msg->rx_len ? msg->rx_len : msg->tx_len;
}

static void dw_mipi_dsi_video_mode_config(struct dw_mipi_dsi *dsi)
{
	u32 val = LP_VACT_EN | LP_VFP_EN | LP_VBP_EN | LP_VSA_EN |
		  LP_HFP_EN | LP_HBP_EN;

	if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO_HFP)
		val &= ~LP_HFP_EN;

	if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO_HBP)
		val &= ~LP_HBP_EN;

	if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO_BURST)
		val |= VID_MODE_TYPE_BURST;
	else if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO_SYNC_PULSE)
		val |= VID_MODE_TYPE_BURST_SYNC_PULSES;
	else
		val |= VID_MODE_TYPE_BURST_SYNC_EVENTS;

	dsi_write(dsi, DSI_VID_MODE_CFG, val);

	if (dsi->mode_flags & MIPI_DSI_CLOCK_NON_CONTINUOUS)
		dsi_update_bits(dsi, DSI_LPCLK_CTRL,
				AUTO_CLKLANE_CTRL, AUTO_CLKLANE_CTRL);
}

static void dw_mipi_dsi_enable(struct dw_mipi_dsi *dsi)
{
	const struct drm_display_mode *mode = &dsi->mode;

	dsi_update_bits(dsi, DSI_LPCLK_CTRL,
			PHY_TXREQUESTCLKHS, PHY_TXREQUESTCLKHS);

	dsi_write(dsi, DSI_PWR_UP, RESET);

	if (dsi->mode_flags & MIPI_DSI_MODE_VIDEO) {
		dsi_update_bits(dsi, DSI_MODE_CFG, CMD_VIDEO_MODE, VIDEO_MODE);
	} else {
		dsi_write(dsi, DSI_DBI_VCID, DBI_VCID(dsi->channel));
		dsi_update_bits(dsi, DSI_CMD_MODE_CFG, DCS_LW_TX, 0);
		dsi_write(dsi, DSI_EDPI_CMD_SIZE, mode->hdisplay);
		dsi_update_bits(dsi, DSI_MODE_CFG,
				CMD_VIDEO_MODE, COMMAND_MODE);
	}

	dsi_write(dsi, DSI_PWR_UP, POWERUP);

	if (dsi->slave)
		dw_mipi_dsi_enable(dsi->slave);
}

static void dw_mipi_dsi_disable(struct dw_mipi_dsi *dsi)
{
	dsi_write(dsi, DSI_PWR_UP, RESET);
	dsi_write(dsi, DSI_LPCLK_CTRL, 0);
	dsi_write(dsi, DSI_EDPI_CMD_SIZE, 0);
	dsi_update_bits(dsi, DSI_MODE_CFG, CMD_VIDEO_MODE, COMMAND_MODE);
	dsi_write(dsi, DSI_PWR_UP, POWERUP);

	if (dsi->slave)
		dw_mipi_dsi_disable(dsi->slave);
}

static void dw_mipi_dsi_post_disable(struct dw_mipi_dsi *dsi)
{
	if (!dsi->prepared)
		return;

	if (dsi->master)
		dw_mipi_dsi_post_disable(dsi->master);

	dsi_write(dsi, DSI_PWR_UP, RESET);
	dsi_write(dsi, DSI_PHY_RSTZ, 0);

	if (dsi->dphy.phy)
		rockchip_phy_power_off(dsi->dphy.phy);

	dsi->prepared = false;

	if (dsi->slave)
		dw_mipi_dsi_post_disable(dsi->slave);
}

static void dw_mipi_dsi_init(struct dw_mipi_dsi *dsi)
{
	u32 esc_clk_div;

	dsi_write(dsi, DSI_PWR_UP, RESET);

	/* The maximum value of the escape clock frequency is 20MHz */
	esc_clk_div = DIV_ROUND_UP(dsi->lane_mbps >> 3, 20);
	dsi_write(dsi, DSI_CLKMGR_CFG, TO_CLK_DIVIDSION(10) |
		  TX_ESC_CLK_DIVIDSION(esc_clk_div));
}

static void dw_mipi_dsi_dpi_config(struct dw_mipi_dsi *dsi,
				   struct drm_display_mode *mode)
{
	u32 val = 0, color = 0;

	switch (dsi->format) {
	case MIPI_DSI_FMT_RGB888:
		color = DPI_COLOR_CODING_24BIT;
		break;
	case MIPI_DSI_FMT_RGB666:
		color = DPI_COLOR_CODING_18BIT_2 | EN18_LOOSELY;
		break;
	case MIPI_DSI_FMT_RGB666_PACKED:
		color = DPI_COLOR_CODING_18BIT_1;
		break;
	case MIPI_DSI_FMT_RGB565:
		color = DPI_COLOR_CODING_16BIT_1;
		break;
	}

	if (mode->flags & DRM_MODE_FLAG_NVSYNC)
		val |= VSYNC_ACTIVE_LOW;
	if (mode->flags & DRM_MODE_FLAG_NHSYNC)
		val |= HSYNC_ACTIVE_LOW;

	dsi_write(dsi, DSI_DPI_VCID, DPI_VID(dsi->channel));
	dsi_write(dsi, DSI_DPI_COLOR_CODING, color);
	dsi_write(dsi, DSI_DPI_CFG_POL, val);
	dsi_write(dsi, DSI_DPI_LP_CMD_TIM, OUTVACT_LPCMD_TIME(4)
		  | INVACT_LPCMD_TIME(4));
}

static void dw_mipi_dsi_packet_handler_config(struct dw_mipi_dsi *dsi)
{
	u32 val = CRC_RX_EN | ECC_RX_EN | BTA_EN | EOTP_TX_EN;

	if (dsi->mode_flags & MIPI_DSI_MODE_EOT_PACKET)
		val &= ~EOTP_TX_EN;

	dsi_write(dsi, DSI_PCKHDL_CFG, val);
}

static void dw_mipi_dsi_video_packet_config(struct dw_mipi_dsi *dsi,
					    struct drm_display_mode *mode)
{
	dsi_write(dsi, DSI_VID_PKT_SIZE, VID_PKT_SIZE(mode->hdisplay));
}

static void dw_mipi_dsi_command_mode_config(struct dw_mipi_dsi *dsi)
{
	dsi_write(dsi, DSI_TO_CNT_CFG, HSTX_TO_CNT(1000) | LPRX_TO_CNT(1000));
	dsi_write(dsi, DSI_BTA_TO_CNT, 0xd00);
}

/* Get lane byte clock cycles. */
static int dw_mipi_dsi_get_hcomponent_lbcc(struct dw_mipi_dsi *dsi,
					   u32 hcomponent)
{
	u32 lbcc;

	lbcc = hcomponent * dsi->lane_mbps * 1000 / 8;

	if (!dsi->mode.clock)
		return 0;

	return DIV_ROUND_CLOSEST(lbcc, dsi->mode.clock);
}

static void dw_mipi_dsi_line_timer_config(struct dw_mipi_dsi *dsi)
{
	int htotal, hsa, hbp, lbcc;
	struct drm_display_mode *mode = &dsi->mode;

	htotal = mode->htotal;
	hsa = mode->hsync_end - mode->hsync_start;
	hbp = mode->htotal - mode->hsync_end;

	lbcc = dw_mipi_dsi_get_hcomponent_lbcc(dsi, htotal);
	dsi_write(dsi, DSI_VID_HLINE_TIME, lbcc);

	lbcc = dw_mipi_dsi_get_hcomponent_lbcc(dsi, hsa);
	dsi_write(dsi, DSI_VID_HSA_TIME, lbcc);

	lbcc = dw_mipi_dsi_get_hcomponent_lbcc(dsi, hbp);
	dsi_write(dsi, DSI_VID_HBP_TIME, lbcc);
}

static void dw_mipi_dsi_vertical_timing_config(struct dw_mipi_dsi *dsi)
{
	u32 vactive, vsa, vfp, vbp;
	struct drm_display_mode *mode = &dsi->mode;

	vactive = mode->vdisplay;
	vsa = mode->vsync_end - mode->vsync_start;
	vfp = mode->vsync_start - mode->vdisplay;
	vbp = mode->vtotal - mode->vsync_end;

	dsi_write(dsi, DSI_VID_VACTIVE_LINES, vactive);
	dsi_write(dsi, DSI_VID_VSA_LINES, vsa);
	dsi_write(dsi, DSI_VID_VFP_LINES, vfp);
	dsi_write(dsi, DSI_VID_VBP_LINES, vbp);
}

static void dw_mipi_dsi_dphy_timing_config(struct dw_mipi_dsi *dsi)
{
	dsi_write(dsi, DSI_PHY_TMR_CFG, PHY_HS2LP_TIME(0x14)
		  | PHY_LP2HS_TIME(0x10) | MAX_RD_TIME(10000));

	dsi_write(dsi, DSI_PHY_TMR_LPCLK_CFG, PHY_CLKHS2LP_TIME(0x40)
		  | PHY_CLKLP2HS_TIME(0x40));
}

static void dw_mipi_dsi_dphy_interface_config(struct dw_mipi_dsi *dsi)
{
	dsi_write(dsi, DSI_PHY_IF_CFG, PHY_STOP_WAIT_TIME(0x20) |
		  N_LANES(dsi->lanes));
}

static void dw_mipi_dsi_clear_err(struct dw_mipi_dsi *dsi)
{
	dsi_read(dsi, DSI_INT_ST0);
	dsi_read(dsi, DSI_INT_ST1);
	dsi_write(dsi, DSI_INT_MSK0, 0);
	dsi_write(dsi, DSI_INT_MSK1, 0);
}

static int dw_mipi_dsi_connector_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct dw_mipi_dsi *dsi = dev_get_priv(conn->dev);

	conn_state->disp_info  = rockchip_get_disp_info(conn_state->type, dsi->id);
	dsi->dphy.phy = conn->phy;

	conn_state->output_mode = ROCKCHIP_OUT_MODE_P888;
	conn_state->color_space = V4L2_COLORSPACE_DEFAULT;
	conn_state->output_if |=
		dsi->id ? VOP_OUTPUT_IF_MIPI1 : VOP_OUTPUT_IF_MIPI0;

#ifndef CONFIG_ROCKCHIP_RK3568
	if (dsi->id) {
		struct udevice *dev;
		int ret;

		ret = uclass_get_device_by_name(UCLASS_DISPLAY, "dsi@ff960000",
						&dev);
		if (ret)
			return ret;

		dsi->master = dev_get_priv(dev);
		if (!dsi->master)
			return -ENODEV;

		conn_state->output_flags = ROCKCHIP_OUTPUT_DATA_SWAP;
	}
#endif

	if (dsi->lanes > 4) {
		struct udevice *dev;
		int ret;

		ret = uclass_get_device_by_name(UCLASS_DISPLAY,
#if defined(CONFIG_ROCKCHIP_RK3288)
						"dsi@ff964000",
#elif defined(CONFIG_ROCKCHIP_RK3399)
						"dsi@ff968000",
#else
						"dsi@fe070000",
#endif
						&dev);
		if (ret)
			return ret;

		dsi->slave = dev_get_priv(dev);
		if (!dsi->slave)
			return -ENODEV;

		dsi->lanes /= 2;
		dsi->slave->lanes = dsi->lanes;
		dsi->slave->format = dsi->format;
		dsi->slave->mode_flags = dsi->mode_flags;
		dsi->slave->channel = dsi->channel;
		conn_state->output_flags =
				ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE;
		if (dsi->data_swap)
			conn_state->output_flags |= ROCKCHIP_OUTPUT_DATA_SWAP;

		conn_state->output_if |= VOP_OUTPUT_IF_MIPI1;

#if defined(CONFIG_ROCKCHIP_RK3568)
		struct rockchip_phy *phy = NULL;
		struct udevice *phy_dev;

		ret = uclass_get_device_by_phandle(UCLASS_PHY, dev,
						   "phys", &phy_dev);
		if (ret)
			return -ENODEV;

		phy = (struct rockchip_phy *)dev_get_driver_data(phy_dev);
		if (!phy)
			return -ENODEV;

		dsi->slave->dphy.phy = phy;
		if (phy->funcs && phy->funcs->init)
			return phy->funcs->init(phy);
#endif

	}

	return 0;
}

static void dw_mipi_dsi_set_hs_clk(struct dw_mipi_dsi *dsi, unsigned long rate)
{
	rate = rockchip_phy_set_pll(dsi->dphy.phy, rate);
	dsi->lane_mbps = rate / 1000 / 1000;
}

static void dw_mipi_dsi_host_init(struct dw_mipi_dsi *dsi)
{
	dw_mipi_dsi_init(dsi);
	dw_mipi_dsi_dpi_config(dsi, &dsi->mode);
	dw_mipi_dsi_packet_handler_config(dsi);
	dw_mipi_dsi_video_mode_config(dsi);
	dw_mipi_dsi_video_packet_config(dsi, &dsi->mode);
	dw_mipi_dsi_command_mode_config(dsi);
	dsi_update_bits(dsi, DSI_MODE_CFG, CMD_VIDEO_MODE, COMMAND_MODE);
	dw_mipi_dsi_line_timer_config(dsi);
	dw_mipi_dsi_vertical_timing_config(dsi);
	dw_mipi_dsi_dphy_timing_config(dsi);
	dw_mipi_dsi_dphy_interface_config(dsi);
	dw_mipi_dsi_clear_err(dsi);
}

static void dw_mipi_dsi_vop_routing(struct dw_mipi_dsi *dsi, int vop_id)
{
	grf_field_write(dsi, VOPSEL, vop_id);

	if (dsi->slave)
		grf_field_write(dsi->slave, VOPSEL, vop_id);
}

static void mipi_dphy_init(struct dw_mipi_dsi *dsi)
{
	u32 map[] = {0x0, 0x1, 0x3, 0x7, 0xf};

	mipi_dphy_enableclk_deassert(dsi);
	mipi_dphy_shutdownz_assert(dsi);
	mipi_dphy_rstz_assert(dsi);
	testif_testclr_assert(dsi);

	/* Configures DPHY to work as a Master */
	grf_field_write(dsi, MASTERSLAVEZ, 1);

	/* Configures lane as TX */
	grf_field_write(dsi, BASEDIR, 0);

	/* Set all REQUEST inputs to zero */
	grf_field_write(dsi, TURNREQUEST, 0);
	grf_field_write(dsi, TURNDISABLE, 0);
	grf_field_write(dsi, FORCETXSTOPMODE, 0);
	grf_field_write(dsi, FORCERXMODE, 0);
	udelay(1);

	testif_testclr_deassert(dsi);

	if (!dsi->dphy.phy)
		dw_mipi_dsi_phy_init(dsi);

	/* Enable Data Lane Module */
	grf_field_write(dsi, ENABLE_N, map[dsi->lanes]);

	/* Enable Clock Lane Module */
	grf_field_write(dsi, ENABLECLK, 1);

	mipi_dphy_enableclk_assert(dsi);
}

static void dw_mipi_dsi_pre_enable(struct dw_mipi_dsi *dsi)
{
	if (dsi->prepared)
		return;

	if (dsi->master)
		dw_mipi_dsi_pre_enable(dsi->master);

	dw_mipi_dsi_host_init(dsi);
	mipi_dphy_init(dsi);
	mipi_dphy_power_on(dsi);
	dsi_write(dsi, DSI_PWR_UP, POWERUP);

	dsi->prepared = true;

	if (dsi->slave)
		dw_mipi_dsi_pre_enable(dsi->slave);
}

static int dw_mipi_dsi_connector_prepare(struct rockchip_connector *conn,
					 struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct crtc_state *crtc_state = &state->crtc_state;
	struct dw_mipi_dsi *dsi = dev_get_priv(conn->dev);
	unsigned long lane_rate;

	memcpy(&dsi->mode, &conn_state->mode, sizeof(struct drm_display_mode));
	if (dsi->slave) {
		dsi->mode.hdisplay /= 2;
		memcpy(&dsi->slave->mode, &dsi->mode,
		       sizeof(struct drm_display_mode));
	}

	lane_rate = dw_mipi_dsi_get_lane_rate(dsi);
	if (dsi->dphy.phy)
		dw_mipi_dsi_set_hs_clk(dsi, lane_rate);
	else
		dw_mipi_dsi_set_pll(dsi, lane_rate);

	if (dsi->slave && dsi->slave->dphy.phy)
		dw_mipi_dsi_set_hs_clk(dsi->slave, lane_rate);

	printf("final DSI-Link bandwidth: %u Mbps x %d\n",
	       dsi->lane_mbps, dsi->slave ? dsi->lanes * 2 : dsi->lanes);

	dw_mipi_dsi_vop_routing(dsi, crtc_state->crtc_id);
	dw_mipi_dsi_pre_enable(dsi);

	return 0;
}

static void dw_mipi_dsi_connector_unprepare(struct rockchip_connector *conn,
					    struct display_state *state)
{
	struct dw_mipi_dsi *dsi = dev_get_priv(conn->dev);

	dw_mipi_dsi_post_disable(dsi);
}

static int dw_mipi_dsi_connector_enable(struct rockchip_connector *conn,
					struct display_state *state)
{
	struct dw_mipi_dsi *dsi = dev_get_priv(conn->dev);

	dw_mipi_dsi_enable(dsi);

	return 0;
}

static int dw_mipi_dsi_connector_disable(struct rockchip_connector *conn,
					 struct display_state *state)
{
	struct dw_mipi_dsi *dsi = dev_get_priv(conn->dev);

	dw_mipi_dsi_disable(dsi);

	return 0;
}

static const struct rockchip_connector_funcs dw_mipi_dsi_connector_funcs = {
	.init = dw_mipi_dsi_connector_init,
	.prepare = dw_mipi_dsi_connector_prepare,
	.unprepare = dw_mipi_dsi_connector_unprepare,
	.enable = dw_mipi_dsi_connector_enable,
	.disable = dw_mipi_dsi_connector_disable,
};

static int dw_mipi_dsi_probe(struct udevice *dev)
{
	struct dw_mipi_dsi *dsi = dev_get_priv(dev);
	const struct dw_mipi_dsi_plat_data *pdata =
		(const struct dw_mipi_dsi_plat_data *)dev_get_driver_data(dev);
	int id;

	dsi->base = dev_read_addr_ptr(dev);
	dsi->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (IS_ERR(dsi->grf))
		return PTR_ERR(dsi->grf);

	id = of_alias_get_id(ofnode_to_np(dev->node), "dsi");
	if (id < 0)
		id = 0;

	dsi->dev = dev;
	dsi->pdata = pdata;
	dsi->id = id;
	dsi->data_swap = dev_read_bool(dsi->dev, "rockchip,data-swap");

	rockchip_connector_bind(&dsi->connector, dev, dsi->id, &dw_mipi_dsi_connector_funcs, NULL,
				DRM_MODE_CONNECTOR_DSI);

	return 0;
}

static const u32 px30_dsi_grf_reg_fields[MAX_FIELDS] = {
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0434,  7,  7),
	[DPICOLORM]		= GRF_REG_FIELD(0x0434,  3,  3),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0434,  2,  2),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0438,  7, 10),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0438,  5,  5),
	[VOPSEL]		= GRF_REG_FIELD(0x0438,  0,  0),
};

static const struct dw_mipi_dsi_plat_data px30_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = px30_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 1000000000UL,
};

static const u32 rk1808_dsi_grf_reg_fields[MAX_FIELDS] = {
	[MASTERSLAVEZ]          = GRF_REG_FIELD(0x0440,  8,  8),
	[DPIUPDATECFG]          = GRF_REG_FIELD(0x0440,  7,  7),
	[DPICOLORM]             = GRF_REG_FIELD(0x0440,  3,  3),
	[DPISHUTDN]             = GRF_REG_FIELD(0x0440,  2,  2),
	[FORCETXSTOPMODE]       = GRF_REG_FIELD(0x0444,  7, 10),
	[FORCERXMODE]           = GRF_REG_FIELD(0x0444,  6,  6),
	[TURNDISABLE]           = GRF_REG_FIELD(0x0444,  5,  5),
};

static const struct dw_mipi_dsi_plat_data rk1808_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk1808_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 2000000000UL,
};

static const u32 rk3128_dsi_grf_reg_fields[MAX_FIELDS] = {
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0150, 10, 13),
	[FORCERXMODE]		= GRF_REG_FIELD(0x0150,  9,  9),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0150,  8,  8),
	[DPICOLORM]		= GRF_REG_FIELD(0x0150,  5,  5),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0150,  4,  4),
};

static const struct dw_mipi_dsi_plat_data rk3128_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk3128_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 1000000000UL,
};

static const u32 rk3288_dsi0_grf_reg_fields[MAX_FIELDS] = {
	[DPICOLORM]		= GRF_REG_FIELD(0x025c,  8,  8),
	[DPISHUTDN]		= GRF_REG_FIELD(0x025c,  7,  7),
	[VOPSEL]		= GRF_REG_FIELD(0x025c,  6,  6),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0264,  8, 11),
	[FORCERXMODE]		= GRF_REG_FIELD(0x0264,  4,  7),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0264,  0,  3),
	[TURNREQUEST]		= GRF_REG_FIELD(0x03a4,  8, 10),
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x03a8,  0,  0),
};

static const u32 rk3288_dsi1_grf_reg_fields[MAX_FIELDS] = {
	[DPICOLORM]		= GRF_REG_FIELD(0x025c, 11, 11),
	[DPISHUTDN]		= GRF_REG_FIELD(0x025c, 10, 10),
	[VOPSEL]		= GRF_REG_FIELD(0x025c,  9,  9),
	[ENABLE_N]		= GRF_REG_FIELD(0x0268, 12, 15),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0268,  8, 11),
	[FORCERXMODE]		= GRF_REG_FIELD(0x0268,  4,  7),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0268,  0,  3),
	[BASEDIR]		= GRF_REG_FIELD(0x027c, 15, 15),
	[MASTERSLAVEZ]		= GRF_REG_FIELD(0x027c, 14, 14),
	[ENABLECLK]		= GRF_REG_FIELD(0x027c, 12, 12),
	[TURNREQUEST]		= GRF_REG_FIELD(0x03a4,  4,  7),
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x03a8,  1,  1),
};

static const struct dw_mipi_dsi_plat_data rk3288_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk3288_dsi0_grf_reg_fields,
	.dsi1_grf_reg_fields = rk3288_dsi1_grf_reg_fields,
	.max_bit_rate_per_lane = 1500000000UL,
};

static const u32 rk3366_dsi_grf_reg_fields[MAX_FIELDS] = {
	[VOPSEL]		= GRF_REG_FIELD(0x0400,  2,  2),
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0410,  9,  9),
	[DPICOLORM]		= GRF_REG_FIELD(0x0410,  3,  3),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0410,  2,  2),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0414,  7, 10),
	[FORCERXMODE]		= GRF_REG_FIELD(0x0414,  6,  6),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0414,  5,  5),
};

static const struct dw_mipi_dsi_plat_data rk3366_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk3366_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 1000000000UL,
};

static const u32 rk3368_dsi_grf_reg_fields[MAX_FIELDS] = {
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0418,  7,  7),
	[DPICOLORM]		= GRF_REG_FIELD(0x0418,  3,  3),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0418,  2,  2),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x041c,  7, 10),
	[FORCERXMODE]		= GRF_REG_FIELD(0x041c,  6,  6),
	[TURNDISABLE]		= GRF_REG_FIELD(0x041c,  5,  5),
};

static const struct dw_mipi_dsi_plat_data rk3368_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk3368_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 1000000000UL,
};

static const u32 rk3399_dsi0_grf_reg_fields[MAX_FIELDS] = {
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x6224, 15, 15),
	[DPISHUTDN]		= GRF_REG_FIELD(0x6224, 14, 14),
	[DPICOLORM]		= GRF_REG_FIELD(0x6224, 13, 13),
	[VOPSEL]		= GRF_REG_FIELD(0x6250,  0,  0),
	[TURNREQUEST]		= GRF_REG_FIELD(0x6258, 12, 15),
	[TURNDISABLE]		= GRF_REG_FIELD(0x6258,  8, 11),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x6258,  4,  7),
	[FORCERXMODE]		= GRF_REG_FIELD(0x6258,  0,  3),
};

static const u32 rk3399_dsi1_grf_reg_fields[MAX_FIELDS] = {
	[VOPSEL]		= GRF_REG_FIELD(0x6250,  4,  4),
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x6250,  3,  3),
	[DPISHUTDN]		= GRF_REG_FIELD(0x6250,  2,  2),
	[DPICOLORM]		= GRF_REG_FIELD(0x6250,  1,  1),
	[TURNDISABLE]		= GRF_REG_FIELD(0x625c, 12, 15),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x625c,  8, 11),
	[FORCERXMODE]           = GRF_REG_FIELD(0x625c,  4,  7),
	[ENABLE_N]		= GRF_REG_FIELD(0x625c,  0,  3),
	[MASTERSLAVEZ]		= GRF_REG_FIELD(0x6260,  7,  7),
	[ENABLECLK]		= GRF_REG_FIELD(0x6260,  6,  6),
	[BASEDIR]		= GRF_REG_FIELD(0x6260,  5,  5),
	[TURNREQUEST]		= GRF_REG_FIELD(0x6260,  0,  3),
};

static const struct dw_mipi_dsi_plat_data rk3399_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk3399_dsi0_grf_reg_fields,
	.dsi1_grf_reg_fields = rk3399_dsi1_grf_reg_fields,
	.max_bit_rate_per_lane = 1500000000UL,
};

static const u32 rk3568_dsi0_grf_reg_fields[MAX_FIELDS] = {
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0360,  2,  2),
	[DPICOLORM]		= GRF_REG_FIELD(0x0360,  1,  1),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0360,  0,  0),
	[SKEWCALHS]		= GRF_REG_FIELD(0x0368, 11, 15),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0368,  4,  7),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0368,  2,  2),
	[FORCERXMODE]		= GRF_REG_FIELD(0x0368,  0,  0),
};

static const u32 rk3568_dsi1_grf_reg_fields[MAX_FIELDS] = {
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0360, 10, 10),
	[DPICOLORM]		= GRF_REG_FIELD(0x0360,  9,  9),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0360,  8,  8),
	[SKEWCALHS]             = GRF_REG_FIELD(0x036c, 11, 15),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x036c,  4,  7),
	[TURNDISABLE]		= GRF_REG_FIELD(0x036c,  2,  2),
	[FORCERXMODE]		= GRF_REG_FIELD(0x036c,  0,  0),
};

static const struct dw_mipi_dsi_plat_data rk3568_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rk3568_dsi0_grf_reg_fields,
	.dsi1_grf_reg_fields = rk3568_dsi1_grf_reg_fields,
	.max_bit_rate_per_lane = 1200000000UL,
};

static const u32 rv1108_dsi_grf_reg_fields[MAX_FIELDS] = {
	[DPICOLORM]		= GRF_REG_FIELD(0x0410,  7,  7),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0410,  6,  6),
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0410,  8,  8),
	[FORCERXMODE]		= GRF_REG_FIELD(0x0414,  5,  5),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x0414,  6,  9),
	[TURNDISABLE]		= GRF_REG_FIELD(0x0414,  4,  4),
};

static const struct dw_mipi_dsi_plat_data rv1108_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rv1108_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 1000000000UL,
};

static const u32 rv1126_dsi_grf_reg_fields[MAX_FIELDS] = {
	[DPIUPDATECFG]		= GRF_REG_FIELD(0x0008,  5,  5),
	[DPISHUTDN]		= GRF_REG_FIELD(0x0008,  4,  4),
	[DPICOLORM]		= GRF_REG_FIELD(0x0008,  3,  3),
	[FORCETXSTOPMODE]	= GRF_REG_FIELD(0x10220,  4,  7),
	[TURNDISABLE]		= GRF_REG_FIELD(0x10220,  2,  2),
	[FORCERXMODE]		= GRF_REG_FIELD(0x10220,  0,  0),
};

static const struct dw_mipi_dsi_plat_data rv1126_mipi_dsi_plat_data = {
	.dsi0_grf_reg_fields = rv1126_dsi_grf_reg_fields,
	.max_bit_rate_per_lane = 1000000000UL,
};

static const struct udevice_id dw_mipi_dsi_ids[] = {
	{
		.compatible = "rockchip,px30-mipi-dsi",
		.data = (ulong)&px30_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk1808-mipi-dsi",
		.data = (ulong)&rk1808_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk3128-mipi-dsi",
		.data = (ulong)&rk3128_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk3288-mipi-dsi",
		.data = (ulong)&rk3288_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk3366-mipi-dsi",
		.data = (ulong)&rk3366_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk3368-mipi-dsi",
		.data = (ulong)&rk3368_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk3399-mipi-dsi",
		.data = (ulong)&rk3399_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rk3568-mipi-dsi",
		.data = (ulong)&rk3568_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rv1108-mipi-dsi",
		.data = (ulong)&rv1108_mipi_dsi_plat_data,
	},
	{
		.compatible = "rockchip,rv1126-mipi-dsi",
		.data = (ulong)&rv1126_mipi_dsi_plat_data,
	},
	{}
};

static ssize_t dw_mipi_dsi_host_transfer(struct mipi_dsi_host *host,
					 const struct mipi_dsi_msg *msg)
{
	struct dw_mipi_dsi *dsi = dev_get_priv(host->dev);

	return dw_mipi_dsi_transfer(dsi, msg);
}

static int dw_mipi_dsi_host_attach(struct mipi_dsi_host *host,
				   struct mipi_dsi_device *device)
{
	struct dw_mipi_dsi *dsi = dev_get_priv(host->dev);

	if (device->lanes < 1 || device->lanes > 8)
		return -EINVAL;

	dsi->lanes = device->lanes;
	dsi->channel = device->channel;
	dsi->format = device->format;
	dsi->mode_flags = device->mode_flags;

	return 0;
}

static const struct mipi_dsi_host_ops dw_mipi_dsi_host_ops = {
	.attach = dw_mipi_dsi_host_attach,
	.transfer = dw_mipi_dsi_host_transfer,
};

static int dw_mipi_dsi_bind(struct udevice *dev)
{
	struct mipi_dsi_host *host = dev_get_platdata(dev);

	host->dev = dev;
	host->ops = &dw_mipi_dsi_host_ops;

	return dm_scan_fdt_dev(dev);
}

static int dw_mipi_dsi_child_post_bind(struct udevice *dev)
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

static int dw_mipi_dsi_child_pre_probe(struct udevice *dev)
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

U_BOOT_DRIVER(dw_mipi_dsi) = {
	.name = "dw_mipi_dsi",
	.id = UCLASS_DISPLAY,
	.of_match = dw_mipi_dsi_ids,
	.probe = dw_mipi_dsi_probe,
	.bind = dw_mipi_dsi_bind,
	.priv_auto_alloc_size = sizeof(struct dw_mipi_dsi),
	.per_child_platdata_auto_alloc_size = sizeof(struct mipi_dsi_device),
	.platdata_auto_alloc_size = sizeof(struct mipi_dsi_host),
	.child_post_bind = dw_mipi_dsi_child_post_bind,
	.child_pre_probe = dw_mipi_dsi_child_pre_probe,
};
