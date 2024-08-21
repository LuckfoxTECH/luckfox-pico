/*
 * SPDX-License-Identifier:     GPL-2.0+
 * (C) Copyright 2008-2016 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <clk-uclass.h>
#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <misc.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <asm/unaligned.h>
#include <asm/arch/clock.h>
#include <dm/device.h>
#include <dm/lists.h>
#include <dm/read.h>
#include <asm/io.h>
#include <linux/list.h>
#include <div64.h>
#include <linux/media-bus-format.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"

#define INNO_HDMI_PHY_TIMEOUT_LOOP_COUNT	1000
#define UPDATE(x, h, l)	(((x) << (l)) & GENMASK((h), (l)))

/* REG: 0x00 */
#define PRE_PLL_REFCLK_SEL_MASK			BIT(0)
#define PRE_PLL_REFCLK_SEL_PCLK			BIT(0)
#define PRE_PLL_REFCLK_SEL_OSCCLK		0
/* REG: 0x01 */
#define BYPASS_RXSENSE_EN_MASK			BIT(2)
#define BYPASS_RXSENSE_EN			BIT(2)
#define BYPASS_PWRON_EN_MASK			BIT(1)
#define BYPASS_PWRON_EN				BIT(1)
#define BYPASS_PLLPD_EN_MASK			BIT(0)
#define BYPASS_PLLPD_EN				BIT(0)
/* REG: 0x02 */
#define BYPASS_PDATA_EN_MASK			BIT(4)
#define BYPASS_PDATA_EN				BIT(4)
#define PDATAEN_MASK				BIT(0)
#define PDATAEN_DISABLE				BIT(0)
#define PDATAEN_ENABLE				0
/* REG: 0x03 */
#define BYPASS_AUTO_TERM_RES_CAL		BIT(7)
#define AUDO_TERM_RES_CAL_SPEED_14_8(x)		UPDATE(x, 6, 0)
/* REG: 0x04 */
#define AUDO_TERM_RES_CAL_SPEED_7_0(x)		UPDATE(x, 7, 0)
/* REG: 0xaa */
#define POST_PLL_CTRL_MASK			BIT(0)
#define POST_PLL_CTRL_MANUAL			BIT(0)
/* REG: 0xe0 */
#define POST_PLL_POWER_MASK			BIT(5)
#define POST_PLL_POWER_DOWN			BIT(5)
#define POST_PLL_POWER_UP			0
#define PRE_PLL_POWER_MASK			BIT(4)
#define PRE_PLL_POWER_DOWN			BIT(4)
#define PRE_PLL_POWER_UP			0
#define RXSENSE_CLK_CH_MASK			BIT(3)
#define RXSENSE_CLK_CH_ENABLE			BIT(3)
#define RXSENSE_DATA_CH2_MASK			BIT(2)
#define RXSENSE_DATA_CH2_ENABLE			BIT(2)
#define RXSENSE_DATA_CH1_MASK			BIT(1)
#define RXSENSE_DATA_CH1_ENABLE			BIT(1)
#define RXSENSE_DATA_CH0_MASK			BIT(0)
#define RXSENSE_DATA_CH0_ENABLE			BIT(0)
/* REG: 0xe1 */
#define BANDGAP_MASK				BIT(4)
#define BANDGAP_ENABLE				BIT(4)
#define BANDGAP_DISABLE				0
#define TMDS_DRIVER_MASK			GENMASK(3, 0)
#define TMDS_DRIVER_ENABLE			UPDATE(0xf, 3, 0)
#define TMDS_DRIVER_DISABLE			0
/* REG: 0xe2 */
#define PRE_PLL_FB_DIV_8_MASK			BIT(7)
#define PRE_PLL_FB_DIV_8_SHIFT			7
#define PRE_PLL_FB_DIV_8(x)			UPDATE(x, 7, 7)
#define PCLK_VCO_DIV_5_MASK			BIT(5)
#define PCLK_VCO_DIV_5_SHIFT			5
#define PCLK_VCO_DIV_5(x)			UPDATE(x, 5, 5)
#define PRE_PLL_PRE_DIV_MASK			GENMASK(4, 0)
#define PRE_PLL_PRE_DIV(x)			UPDATE(x, 4, 0)
/* REG: 0xe3 */
#define PRE_PLL_FB_DIV_7_0(x)			UPDATE(x, 7, 0)
/* REG: 0xe4 */
#define PRE_PLL_PCLK_DIV_B_MASK			GENMASK(6, 5)
#define PRE_PLL_PCLK_DIV_B_SHIFT		5
#define PRE_PLL_PCLK_DIV_B(x)			UPDATE(x, 6, 5)
#define PRE_PLL_PCLK_DIV_A_MASK			GENMASK(4, 0)
#define PRE_PLL_PCLK_DIV_A_SHIFT		0
#define PRE_PLL_PCLK_DIV_A(x)			UPDATE(x, 4, 0)
/* REG: 0xe5 */
#define PRE_PLL_PCLK_DIV_C_MASK			GENMASK(6, 5)
#define PRE_PLL_PCLK_DIV_C_SHIFT		5
#define PRE_PLL_PCLK_DIV_C(x)			UPDATE(x, 6, 5)
#define PRE_PLL_PCLK_DIV_D_MASK			GENMASK(4, 0)
#define PRE_PLL_PCLK_DIV_D_SHIFT		0
#define PRE_PLL_PCLK_DIV_D(x)			UPDATE(x, 4, 0)
/* REG: 0xe6 */
#define PRE_PLL_TMDSCLK_DIV_C_MASK		GENMASK(5, 4)
#define PRE_PLL_TMDSCLK_DIV_C(x)		UPDATE(x, 5, 4)
#define PRE_PLL_TMDSCLK_DIV_A_MASK		GENMASK(3, 2)
#define PRE_PLL_TMDSCLK_DIV_A(x)		UPDATE(x, 3, 2)
#define PRE_PLL_TMDSCLK_DIV_B_MASK		GENMASK(1, 0)
#define PRE_PLL_TMDSCLK_DIV_B(x)		UPDATE(x, 1, 0)
/* REG: 0xe8 */
#define PRE_PLL_LOCK_STATUS			BIT(0)
/* REG: 0xe9 */
#define POST_PLL_POST_DIV_EN_MASK		GENMASK(7, 6)
#define POST_PLL_POST_DIV_ENABLE		UPDATE(3, 7, 6)
#define POST_PLL_POST_DIV_DISABLE		0
#define POST_PLL_PRE_DIV_MASK			GENMASK(4, 0)
#define POST_PLL_PRE_DIV(x)			UPDATE(x, 4, 0)
/* REG: 0xea */
#define POST_PLL_FB_DIV_7_0(x)			UPDATE(x, 7, 0)
/* REG: 0xeb */
#define POST_PLL_FB_DIV_8_MASK			BIT(7)
#define POST_PLL_FB_DIV_8(x)			UPDATE(x, 7, 7)
#define POST_PLL_POST_DIV_MASK			GENMASK(5, 4)
#define POST_PLL_POST_DIV(x)			UPDATE(x, 5, 4)
#define POST_PLL_LOCK_STATUS			BIT(0)
/* REG: 0xee */
#define TMDS_CH_TA_MASK				GENMASK(7, 4)
#define TMDS_CH_TA_ENABLE			UPDATE(0xf, 7, 4)
#define TMDS_CH_TA_DISABLE			0
/* REG: 0xef */
#define TMDS_CLK_CH_TA(x)			UPDATE(x, 7, 6)
#define TMDS_DATA_CH2_TA(x)			UPDATE(x, 5, 4)
#define TMDS_DATA_CH1_TA(x)			UPDATE(x, 3, 2)
#define TMDS_DATA_CH0_TA(x)			UPDATE(x, 1, 0)
/* REG: 0xf0 */
#define TMDS_DATA_CH2_PRE_EMPHASIS_MASK		GENMASK(5, 4)
#define TMDS_DATA_CH2_PRE_EMPHASIS(x)		UPDATE(x, 5, 4)
#define TMDS_DATA_CH1_PRE_EMPHASIS_MASK		GENMASK(3, 2)
#define TMDS_DATA_CH1_PRE_EMPHASIS(x)		UPDATE(x, 3, 2)
#define TMDS_DATA_CH0_PRE_EMPHASIS_MASK		GENMASK(1, 0)
#define TMDS_DATA_CH0_PRE_EMPHASIS(x)		UPDATE(x, 1, 0)
/* REG: 0xf1 */
#define TMDS_CLK_CH_OUTPUT_SWING(x)		UPDATE(x, 7, 4)
#define TMDS_DATA_CH2_OUTPUT_SWING(x)		UPDATE(x, 3, 0)
/* REG: 0xf2 */
#define TMDS_DATA_CH1_OUTPUT_SWING(x)		UPDATE(x, 7, 4)
#define TMDS_DATA_CH0_OUTPUT_SWING(x)		UPDATE(x, 3, 0)

enum inno_hdmi_phy_type {
	INNO_HDMI_PHY_RK3228,
	INNO_HDMI_PHY_RK3328,
	INNO_HDMI_PHY_RK3528
};

struct inno_hdmi_phy_drv_data;

struct phy_config {
	unsigned long	tmdsclock;
	u8		regs[14];
};

struct inno_hdmi_phy {
	struct udevice *dev;
	ofnode node;
	void *regs;

	/* platform data */
	const struct inno_hdmi_phy_drv_data *plat_data;
	unsigned long pixclock;
	u32 bus_width;
	struct phy_config *phy_cfg;
};

struct pre_pll_config {
	unsigned long pixclock;
	unsigned long tmdsclock;
	u8 prediv;
	u16 fbdiv;
	u8 tmds_div_a;
	u8 tmds_div_b;
	u8 tmds_div_c;
	u8 pclk_div_a;
	u8 pclk_div_b;
	u8 pclk_div_c;
	u8 pclk_div_d;
	u8 vco_div_5_en;
	u32 fracdiv;
};

struct post_pll_config {
	unsigned long tmdsclock;
	u8 prediv;
	u16 fbdiv;
	u8 postdiv;
	u8 version;
};

struct inno_hdmi_phy_ops {
	void (*init)(struct inno_hdmi_phy *inno);
	int (*power_on)(struct inno_hdmi_phy *inno,
			const struct post_pll_config *cfg,
			const struct phy_config *phy_cfg);
	void (*power_off)(struct inno_hdmi_phy *inno);
	int (*pre_pll_update)(struct inno_hdmi_phy *inno,
			      const struct pre_pll_config *cfg);
	unsigned long (*recalc_rate)(struct inno_hdmi_phy *inno,
				     unsigned long parent_rate);
};

struct inno_hdmi_phy_drv_data {
	enum inno_hdmi_phy_type		dev_type;
	const struct inno_hdmi_phy_ops	*ops;
	const struct phy_config		*phy_cfg_table;
};

struct rockchip_inno_data {
	char compatible[30];
	const void *data;
};

struct clk_inno_hdmi {
	struct udevice *dev;
	ulong rate;
};

/* global variables are used to pass reource from phy drivers to clk driver */
static struct inno_hdmi_phy *g_inno;

static const struct pre_pll_config pre_pll_cfg_table[] = {
	{ 27000000,  27000000, 1,  90, 3, 2, 2, 10, 3, 3, 4, 0, 0},
	{ 27000000,  33750000, 1,  90, 1, 3, 3, 10, 3, 3, 4, 0, 0},
	{ 40000000,  40000000, 1,  80, 2, 2, 2, 12, 2, 2, 2, 0, 0},
	{ 59341000,  59341000, 1,  98, 3, 1, 2,  1, 3, 3, 4, 0, 0xE6AE6B},
	{ 59400000,  59400000, 1,  99, 3, 1, 1,  1, 3, 3, 4, 0, 0},
	{ 59341000,  74176250, 1,  98, 0, 3, 3,  1, 3, 3, 4, 0, 0xE6AE6B},
	{ 59400000,  74250000, 1,  99, 1, 2, 2,  1, 3, 3, 4, 0, 0},
	{ 74176000,  74176000, 1,  98, 1, 2, 2,  1, 2, 3, 4, 0, 0xE6AE6B},
	{ 74250000,  74250000, 1,  99, 1, 2, 2,  1, 2, 3, 4, 0, 0},
	{ 74176000,  92720000, 4, 494, 1, 2, 2,  1, 3, 3, 4, 0, 0x816817},
	{ 74250000,  92812500, 4, 495, 1, 2, 2,  1, 3, 3, 4, 0, 0},
	{148352000, 148352000, 1,  98, 1, 1, 1,  1, 2, 2, 2, 0, 0xE6AE6B},
	{148500000, 148500000, 1,  99, 1, 1, 1,  1, 2, 2, 2, 0, 0},
	{148352000, 185440000, 4, 494, 0, 2, 2,  1, 3, 2, 2, 0, 0x816817},
	{148500000, 185625000, 4, 495, 0, 2, 2,  1, 3, 2, 2, 0, 0},
	{296703000, 296703000, 1,  98, 0, 1, 1,  1, 0, 2, 2, 0, 0xE6AE6B},
	{297000000, 297000000, 1,  99, 0, 1, 1,  1, 0, 2, 2, 0, 0},
	{296703000, 370878750, 4, 494, 1, 2, 0,  1, 3, 1, 1, 0, 0x816817},
	{297000000, 371250000, 4, 495, 1, 2, 0,  1, 3, 1, 1, 0, 0},
	{593407000, 296703500, 1,  98, 0, 1, 1,  1, 0, 2, 1, 0, 0xE6AE6B},
	{594000000, 297000000, 1,  99, 0, 1, 1,  1, 0, 2, 1, 0, 0},
	{593407000, 370879375, 4, 494, 1, 2, 0,  1, 3, 1, 1, 1, 0x816817},
	{594000000, 371250000, 4, 495, 1, 2, 0,  1, 3, 1, 1, 1, 0},
	{593407000, 593407000, 1,  98, 0, 2, 0,  1, 0, 1, 1, 0, 0xE6AE6B},
	{594000000, 594000000, 1,  99, 0, 2, 0,  1, 0, 1, 1, 0, 0},
	{     ~0UL,	    0, 0,   0, 0, 0, 0,  0, 0, 0, 0, 0, 0}
};

static const struct post_pll_config post_pll_cfg_table[] = {
	{33750000,  1, 40, 8, 1},
	{33750000,  1, 80, 8, 2},
	{33750000,  1, 10, 2, 4},
	{74250000,  1, 40, 8, 1},
	{74250000, 18, 80, 8, 2},
	{74250000,  1, 20, 4, 8},
	{148500000, 2, 40, 4, 3},
	{148500000, 1, 10, 2, 8},
	{297000000, 4, 40, 2, 3},
	{297000000, 2, 20, 2, 8},
	{594000000, 8, 40, 1, 3},
	{594000000, 4, 20, 1, 8},
	{     ~0UL, 0,  0, 0, 0}
};

static const struct phy_config rk3228_phy_cfg[] = {
	{	165000000, {
			0xaa, 0x00, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}, {
		340000000, {
			0xaa, 0x15, 0x6a, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}, {
		594000000, {
			0xaa, 0x15, 0x7a, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}, {
		~0UL, {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}
};

static const struct phy_config rk3328_phy_cfg[] = {
	{	165000000, {
			0x07, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x08,
			0x00, 0xac, 0xcc, 0xcc, 0xcc,
		},
	}, {
		340000000, {
			0x0b, 0x0d, 0x0d, 0x0d, 0x07, 0x15, 0x08, 0x08, 0x08,
			0x3f, 0xac, 0xcc, 0xcd, 0xdd,
		},
	}, {
		594000000, {
			0x10, 0x1a, 0x1a, 0x1a, 0x07, 0x15, 0x08, 0x08, 0x08,
			0x00, 0xac, 0xcc, 0xcc, 0xcc,
		},
	}, {
		~0UL, {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}
};

static const struct phy_config rk3528_phy_cfg[] = {
	/* tmdsclk bias-clk bias-data voltage-clk voltage-data pre-emphasis-data */
	{	165000000, {
			0x03, 0x04, 0x0c, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}, {
		340000000, {
			0x03, 0x04, 0x0c, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}, {
		594000000, {
			0x02, 0x08, 0x0d, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}, {
		~0UL, {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00,
		},
	}
};

static inline void inno_write(struct inno_hdmi_phy *inno, u32 reg, u8 val)
{
	writel(val, inno->regs + (reg * 4));
}

static inline u8 inno_read(struct inno_hdmi_phy *inno, u32 reg)
{
	u32 val;

	val = readl(inno->regs + (reg * 4));

	return val;
}

static inline void inno_update_bits(struct inno_hdmi_phy *inno, u8 reg,
				    u8 mask, u8 val)
{
	u32 tmp, orig;

	orig = inno_read(inno, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	inno_write(inno, reg, tmp);
}

static u32 inno_hdmi_phy_get_tmdsclk(struct inno_hdmi_phy *inno,
				     unsigned long rate)
{
	u32 tmdsclk;

	switch (inno->bus_width) {
	case 4:
		tmdsclk = (u32)rate / 2;
		break;
	case 5:
		tmdsclk = (u32)rate * 5 / 8;
		break;
	case 6:
		tmdsclk = (u32)rate * 3 / 4;
		break;
	case 10:
		tmdsclk = (u32)rate * 5 / 4;
		break;
	case 12:
		tmdsclk = (u32)rate * 3 / 2;
		break;
	case 16:
		tmdsclk = (u32)rate * 2;
		break;
	default:
		tmdsclk = rate;
	}

	return tmdsclk;
}

static u8 rk_get_cpu_version(void)
{
	u8 val = 0;
#ifdef CONFIG_ROCKCHIP_EFUSE
	struct udevice *dev;
	u32 regs[2] = {0};
	u8 fuses[1];
	ofnode node;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_MISC,
					  DM_GET_DRIVER(rockchip_efuse), &dev);
	if (ret) {
		printf("%s: no misc-device found\n", __func__);
		return -EINVAL;
	}

	node = dev_read_subnode(dev, "cpu-version");
	if (!ofnode_valid(node))
		return -EINVAL;

	ret = ofnode_read_u32_array(node, "reg", regs, 2);
	if (ret) {
		printf("Cannot get efuse reg\n");
		return -EINVAL;
	}

	ret = misc_read(dev, regs[0], &fuses, regs[1]);
	if (ret) {
		printf("%s: misc_read failed\n", __func__);
		return 0;
	}

	val = fuses[0];
	val = (val >> 3) & 0x1;
#endif
	return val;
}

static int inno_hdmi_phy_power_on(struct rockchip_phy *phy)
{
#ifdef CONFIG_SPL_BUILD
	struct inno_hdmi_phy *inno = (struct inno_hdmi_phy *)phy->data;
#else
	struct inno_hdmi_phy *inno = dev_get_priv(phy->dev);
#endif
	const struct post_pll_config *cfg = post_pll_cfg_table;
	const struct phy_config *phy_cfg = inno->plat_data->phy_cfg_table;
	u32 tmdsclock = inno_hdmi_phy_get_tmdsclk(inno, inno->pixclock);
	u32 chipversion = 1;

	printf("start Inno HDMI PHY Power On\n");

	if (inno->phy_cfg)
		phy_cfg = inno->phy_cfg;

	if (!tmdsclock) {
		printf("TMDS clock is zero!\n");
		return -EINVAL;
	}

	if (inno->plat_data->dev_type == INNO_HDMI_PHY_RK3328 &&
	    rk_get_cpu_version())
		chipversion = 2;
	else if (inno->plat_data->dev_type == INNO_HDMI_PHY_RK3228 &&
		 tmdsclock <= 33750000)
		chipversion = 4;
	else if (inno->plat_data->dev_type == INNO_HDMI_PHY_RK3528)
		chipversion = 8;

	printf("tmdsclock = %d; chipversion = %d\n", tmdsclock, chipversion);

	for (; cfg->tmdsclock != ~0UL; cfg++)
		if (tmdsclock <= cfg->tmdsclock &&
		    cfg->version & chipversion)
			break;

	for (; phy_cfg->tmdsclock != ~0UL; phy_cfg++)
		if (tmdsclock <= phy_cfg->tmdsclock)
			break;

	if (cfg->tmdsclock == ~0UL || phy_cfg->tmdsclock == ~0UL)
		return -EINVAL;

	printf("Inno HDMI PHY Power On\n");
	if (inno->plat_data->ops->power_on)
		return inno->plat_data->ops->power_on(inno, cfg, phy_cfg);
	else
		return -EINVAL;
}

static int inno_hdmi_phy_power_off(struct rockchip_phy *phy)
{
#ifdef CONFIG_SPL_BUILD
	struct inno_hdmi_phy *inno = (struct inno_hdmi_phy *)phy->data;
#else
	struct inno_hdmi_phy *inno = dev_get_priv(phy->dev);
#endif

	if (inno->plat_data->ops->power_off)
		inno->plat_data->ops->power_off(inno);
	printf("Inno HDMI PHY Power Off\n");

	return 0;
}

static int inno_hdmi_phy_clk_is_prepared(struct inno_hdmi_phy *inno)
{
	u8 status;

	if (inno->plat_data->dev_type == INNO_HDMI_PHY_RK3228)
		status = inno_read(inno, 0xe0) & PRE_PLL_POWER_MASK;
	else
		status = inno_read(inno, 0xa0) & 1;

	return status ? 0 : 1;
}

static int inno_hdmi_phy_clk_prepare(struct inno_hdmi_phy *inno)
{
	if (inno->plat_data->dev_type == INNO_HDMI_PHY_RK3228)
		inno_update_bits(inno, 0xe0, PRE_PLL_POWER_MASK,
				 PRE_PLL_POWER_UP);
	else
		inno_update_bits(inno, 0xa0, 1, 0);

	return 0;
}

static int inno_hdmi_phy_clk_set_rate(struct inno_hdmi_phy *inno,
				      unsigned long rate)
{
	const struct pre_pll_config *cfg = pre_pll_cfg_table;
	u32 tmdsclock = inno_hdmi_phy_get_tmdsclk(inno, rate);

	for (; cfg->pixclock != ~0UL; cfg++)
		if (cfg->pixclock == rate && cfg->tmdsclock == tmdsclock)
			break;

	if (cfg->pixclock == ~0UL) {
		printf("unsupported rate %lu\n", rate);
		return -EINVAL;
	}

	if (inno->plat_data->ops->pre_pll_update)
		inno->plat_data->ops->pre_pll_update(inno, cfg);

	inno->pixclock = rate;

	return 0;
}

static void inno_hdmi_phy_rk3228_init(struct inno_hdmi_phy *inno)
{
	u32 m, v;

	/*
	 * Use phy internal register control
	 * rxsense/poweron/pllpd/pdataen signal.
	 */
	m = BYPASS_RXSENSE_EN_MASK | BYPASS_PWRON_EN_MASK |
	    BYPASS_PLLPD_EN_MASK;
	v = BYPASS_RXSENSE_EN | BYPASS_PWRON_EN | BYPASS_PLLPD_EN;
	inno_update_bits(inno, 0x01, m, v);
	inno_update_bits(inno, 0x02, BYPASS_PDATA_EN_MASK, BYPASS_PDATA_EN);

	/* manual power down post-PLL */
	inno_update_bits(inno, 0xaa, POST_PLL_CTRL_MASK, POST_PLL_CTRL_MANUAL);
}

static int
inno_hdmi_phy_rk3228_power_on(struct inno_hdmi_phy *inno,
			      const struct post_pll_config *cfg,
			      const struct phy_config *phy_cfg)
{
	int pll_tries;
	u32 m, v;

	/* pdata_en disable */
	inno_update_bits(inno, 0x02, PDATAEN_MASK, PDATAEN_DISABLE);

	/* Power down Post-PLL */
	inno_update_bits(inno, 0xe0, PRE_PLL_POWER_MASK, PRE_PLL_POWER_DOWN);
	inno_update_bits(inno, 0xe0, POST_PLL_POWER_MASK, POST_PLL_POWER_DOWN);

	/* Post-PLL update */
	m = POST_PLL_PRE_DIV_MASK;
	v = POST_PLL_PRE_DIV(cfg->prediv);
	inno_update_bits(inno, 0xe9, m, v);

	m = POST_PLL_FB_DIV_8_MASK;
	v = POST_PLL_FB_DIV_8(cfg->fbdiv >> 8);
	inno_update_bits(inno, 0xeb, m, v);
	inno_write(inno, 0xea, POST_PLL_FB_DIV_7_0(cfg->fbdiv));

	if (cfg->postdiv == 1) {
		/* Disable Post-PLL post divider */
		m = POST_PLL_POST_DIV_EN_MASK;
		v = POST_PLL_POST_DIV_DISABLE;
		inno_update_bits(inno, 0xe9, m, v);
	} else {
		/* Enable Post-PLL post divider */
		m = POST_PLL_POST_DIV_EN_MASK;
		v = POST_PLL_POST_DIV_ENABLE;
		inno_update_bits(inno, 0xe9, m, v);

		m = POST_PLL_POST_DIV_MASK;
		v = POST_PLL_POST_DIV(cfg->postdiv / 2 - 1);
		inno_update_bits(inno, 0xeb, m, v);
	}

	for (v = 0; v < 4; v++)
		inno_write(inno, 0xef + v, phy_cfg->regs[v]);

	/* Power up Post-PLL */
	inno_update_bits(inno, 0xe0, POST_PLL_POWER_MASK, POST_PLL_POWER_UP);
	inno_update_bits(inno, 0xe0, PRE_PLL_POWER_MASK, PRE_PLL_POWER_UP);

	/* BandGap enable */
	inno_update_bits(inno, 0xe1, BANDGAP_MASK, BANDGAP_ENABLE);

	/* TMDS driver enable */
	inno_update_bits(inno, 0xe1, TMDS_DRIVER_MASK, TMDS_DRIVER_ENABLE);

	/* Wait for post PLL lock */
	pll_tries = 0;
	while (!(inno_read(inno, 0xeb) & POST_PLL_LOCK_STATUS)) {
		if (pll_tries == INNO_HDMI_PHY_TIMEOUT_LOOP_COUNT) {
			printf("Post-PLL unlock\n");
			return -ETIMEDOUT;
		}

		pll_tries++;
		udelay(100);
	}

	if (cfg->tmdsclock > 340000000)
		mdelay(100);

	/* pdata_en enable */
	inno_update_bits(inno, 0x02, PDATAEN_MASK, PDATAEN_ENABLE);
	return 0;
}

static void inno_hdmi_phy_rk3228_power_off(struct inno_hdmi_phy *inno)
{
	/* TMDS driver Disable */
	inno_update_bits(inno, 0xe1, TMDS_DRIVER_MASK, TMDS_DRIVER_DISABLE);

	/* BandGap Disable */
	inno_update_bits(inno, 0xe1, BANDGAP_MASK, BANDGAP_DISABLE);

	/* Post-PLL power down */
	inno_update_bits(inno, 0xe0, POST_PLL_POWER_MASK, POST_PLL_POWER_DOWN);
}

static int
inno_hdmi_phy_rk3228_pre_pll_update(struct inno_hdmi_phy *inno,
				    const struct pre_pll_config *cfg)
{
	int pll_tries;
	u32 m, v;

	/* Power down PRE-PLL */
	inno_update_bits(inno, 0xe0, PRE_PLL_POWER_MASK, PRE_PLL_POWER_DOWN);

	m = PRE_PLL_FB_DIV_8_MASK | PCLK_VCO_DIV_5_MASK | PRE_PLL_PRE_DIV_MASK;
	v = PRE_PLL_FB_DIV_8(cfg->fbdiv >> 8) |
	    PCLK_VCO_DIV_5(cfg->vco_div_5_en) | PRE_PLL_PRE_DIV(cfg->prediv);
	inno_update_bits(inno, 0xe2, m, v);

	inno_write(inno, 0xe3, PRE_PLL_FB_DIV_7_0(cfg->fbdiv));

	m = PRE_PLL_PCLK_DIV_B_MASK | PRE_PLL_PCLK_DIV_A_MASK;
	v = PRE_PLL_PCLK_DIV_B(cfg->pclk_div_b) |
	    PRE_PLL_PCLK_DIV_A(cfg->pclk_div_a);
	inno_update_bits(inno, 0xe4, m, v);

	m = PRE_PLL_PCLK_DIV_C_MASK | PRE_PLL_PCLK_DIV_D_MASK;
	v = PRE_PLL_PCLK_DIV_C(cfg->pclk_div_c) |
	    PRE_PLL_PCLK_DIV_D(cfg->pclk_div_d);
	inno_update_bits(inno, 0xe5, m, v);

	m = PRE_PLL_TMDSCLK_DIV_C_MASK | PRE_PLL_TMDSCLK_DIV_A_MASK |
	    PRE_PLL_TMDSCLK_DIV_B_MASK;
	v = PRE_PLL_TMDSCLK_DIV_C(cfg->tmds_div_c) |
	    PRE_PLL_TMDSCLK_DIV_A(cfg->tmds_div_a) |
	    PRE_PLL_TMDSCLK_DIV_B(cfg->tmds_div_b);
	inno_update_bits(inno, 0xe6, m, v);

	/* Power up PRE-PLL */
	inno_update_bits(inno, 0xe0, PRE_PLL_POWER_MASK, PRE_PLL_POWER_UP);

	/* Wait for Pre-PLL lock */
	pll_tries = 0;
	while (!(inno_read(inno, 0xe8) & PRE_PLL_LOCK_STATUS)) {
		if (pll_tries == INNO_HDMI_PHY_TIMEOUT_LOOP_COUNT) {
			printf("Pre-PLL unlock\n");
			return -ETIMEDOUT;
		}

		pll_tries++;
		udelay(100);
	}

	return 0;
}

static void inno_hdmi_phy_rk3328_init(struct inno_hdmi_phy *inno)
{
	/*
	 * Use phy internal register control
	 * rxsense/poweron/pllpd/pdataen signal.
	 */
	inno_write(inno, 0x01, 0x07);
	inno_write(inno, 0x02, 0x91);
}

static int
inno_hdmi_phy_rk3328_power_on(struct inno_hdmi_phy *inno,
			      const struct post_pll_config *cfg,
			      const struct phy_config *phy_cfg)
{
	u32 val;

	/* set pdata_en to 0 */
	inno_update_bits(inno, 0x02, 1, 0);
	/* Power off post PLL */
	inno_update_bits(inno, 0xaa, 1, 1);

	val = cfg->fbdiv & 0xff;
	inno_write(inno, 0xac, val);
	if (cfg->postdiv == 1) {
		inno_write(inno, 0xaa, 2);
		val = (cfg->fbdiv >> 8) | cfg->prediv;
		inno_write(inno, 0xab, val);
	} else {
		val = (cfg->postdiv / 2) - 1;
		inno_write(inno, 0xad, val);
		val = (cfg->fbdiv >> 8) | cfg->prediv;
		inno_write(inno, 0xab, val);
		inno_write(inno, 0xaa, 0x0e);
	}

	for (val = 0; val < 14; val++)
		inno_write(inno, 0xb5 + val, phy_cfg->regs[val]);

	/* bit[7:6] of reg c8/c9/ca/c8 is ESD detect threshold:
	 * 00 - 340mV
	 * 01 - 280mV
	 * 10 - 260mV
	 * 11 - 240mV
	 * default is 240mV, now we set it to 340mV
	 */
	inno_write(inno, 0xc8, 0);
	inno_write(inno, 0xc9, 0);
	inno_write(inno, 0xca, 0);
	inno_write(inno, 0xcb, 0);

	if (phy_cfg->tmdsclock > 340000000) {
		/* Set termination resistor to 100ohm */
		val = 75000000 / 100000;
		inno_write(inno, 0xc5, ((val >> 8) & 0xff) | 0x80);
		inno_write(inno, 0xc6, val & 0xff);
		inno_write(inno, 0xc7, 3 << 1);
		inno_write(inno, 0xc5, ((val >> 8) & 0xff));
	} else if (phy_cfg->tmdsclock > 165000000) {
		inno_write(inno, 0xc5, 0x81);
		/* clk termination resistor is 50ohm
		 * data termination resistor is 150ohm
		 */
		inno_write(inno, 0xc8, 0x30);
		inno_write(inno, 0xc9, 0x10);
		inno_write(inno, 0xca, 0x10);
		inno_write(inno, 0xcb, 0x10);
	} else {
		inno_write(inno, 0xc5, 0x81);
	}

	/* set TMDS sync detection counter length */
	val = 47520000000UL / phy_cfg->tmdsclock;
	inno_write(inno, 0xd8, (val >> 8) & 0xff);
	inno_write(inno, 0xd9, val & 0xff);

	/* Power up post PLL */
	inno_update_bits(inno, 0xaa, 1, 0);
	/* Power up tmds driver */
	inno_update_bits(inno, 0xb0, 4, 4);
	inno_write(inno, 0xb2, 0x0f);

	/* Wait for post PLL lock */
	for (val = 0; val < 5; val++) {
		if (inno_read(inno, 0xaf) & 1)
			break;
		udelay(1000);
	}
	if (!(inno_read(inno, 0xaf) & 1)) {
		printf("HDMI PHY Post PLL unlock\n");
		return -ETIMEDOUT;
	}
	if (phy_cfg->tmdsclock > 340000000)
		mdelay(100);
	/* set pdata_en to 1 */
	inno_update_bits(inno, 0x02, 1, 1);

	return 0;
}

static void inno_hdmi_phy_rk3328_power_off(struct inno_hdmi_phy *inno)
{
	/* Power off driver */
	inno_write(inno, 0xb2, 0);
	/* Power off band gap */
	inno_update_bits(inno, 0xb0, 4, 0);
	/* Power off post pll */
	inno_update_bits(inno, 0xaa, 1, 1);
}

static int
inno_hdmi_phy_rk3328_pre_pll_update(struct inno_hdmi_phy *inno,
				    const struct pre_pll_config *cfg)
{
	u32 val;

	/* Power off PLL */
	inno_update_bits(inno, 0xa0, 1, 1);
	/* Configure pre-pll */
	inno_update_bits(inno, 0xa0, 2, (cfg->vco_div_5_en & 1) << 1);
	inno_write(inno, 0xa1, cfg->prediv);
	if (cfg->fracdiv)
		val = ((cfg->fbdiv >> 8) & 0x0f) | 0xc0;
	else
		val = ((cfg->fbdiv >> 8) & 0x0f) | 0xf0;
	inno_write(inno, 0xa2, val);
	inno_write(inno, 0xa3, cfg->fbdiv & 0xff);
	val = (cfg->pclk_div_a & 0x1f) |
	      ((cfg->pclk_div_b & 3) << 5);
	inno_write(inno, 0xa5, val);
	val = (cfg->pclk_div_d & 0x1f) |
	      ((cfg->pclk_div_c & 3) << 5);
	inno_write(inno, 0xa6, val);
	val = ((cfg->tmds_div_a & 3) << 4) |
	      ((cfg->tmds_div_b & 3) << 2) |
	      (cfg->tmds_div_c & 3);
	inno_write(inno, 0xa4, val);

	if (cfg->fracdiv) {
		val = cfg->fracdiv & 0xff;
		inno_write(inno, 0xd3, val);
		val = (cfg->fracdiv >> 8) & 0xff;
		inno_write(inno, 0xd2, val);
		val = (cfg->fracdiv >> 16) & 0xff;
		inno_write(inno, 0xd1, val);
	} else {
		inno_write(inno, 0xd3, 0);
		inno_write(inno, 0xd2, 0);
		inno_write(inno, 0xd1, 0);
	}

	/* Power up PLL */
	inno_update_bits(inno, 0xa0, 1, 0);

	/* Wait for PLL lock */
	for (val = 0; val < 5; val++) {
		if (inno_read(inno, 0xa9) & 1)
			break;
		udelay(1000);
	}
	if (val == 5) {
		printf("Pre-PLL unlock\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static unsigned long
inno_hdmi_3328_phy_pll_recalc_rate(struct inno_hdmi_phy *inno,
				   unsigned long parent_rate)
{
	unsigned long rate, vco, frac;
	u8 nd, no_a, no_b, no_d;
	__maybe_unused u8 no_c;
	u16 nf;

	nd = inno_read(inno, 0xa1) & 0x3f;
	nf = ((inno_read(inno, 0xa2) & 0x0f) << 8) | inno_read(inno, 0xa3);
	vco = parent_rate * nf;
	if ((inno_read(inno, 0xa2) & 0x30) == 0) {
		frac = inno_read(inno, 0xd3) |
		       (inno_read(inno, 0xd2) << 8) |
		       (inno_read(inno, 0xd1) << 16);
		vco += DIV_ROUND_CLOSEST(parent_rate * frac, (1 << 24));
	}
	if (inno_read(inno, 0xa0) & 2) {
		rate = vco / (nd * 5);
	} else {
		no_a = inno_read(inno, 0xa5) & 0x1f;
		no_b = ((inno_read(inno, 0xa5) >> 5) & 7) + 2;
		no_c = (1 << ((inno_read(inno, 0xa6) >> 5) & 7));
		no_d = inno_read(inno, 0xa6) & 0x1f;
		if (no_a == 1)
			rate = vco / (nd * no_b * no_d * 2);
		else
			rate = vco / (nd * no_a * no_d * 2);
	}
	inno->pixclock = rate;

	return rate;
}

static int
inno_hdmi_phy_rk3528_power_on(struct inno_hdmi_phy *inno,
			      const struct post_pll_config *cfg,
			      const struct phy_config *phy_cfg)
{
	u32 val;
	u64 temp;
	u32 tmdsclock = inno_hdmi_phy_get_tmdsclk(inno, inno->pixclock);

	/* Power off post PLL */
	inno_update_bits(inno, 0xaa, 1, 0);

	val = cfg->prediv;
	inno_write(inno, 0xab, val);

	if (cfg->postdiv == 1) {
		inno_write(inno, 0xad, 0x8);
		inno_write(inno, 0xaa, 2);
	} else {
		val = (cfg->postdiv / 2) - 1;
		inno_write(inno, 0xad, val);
		inno_write(inno, 0xaa, 0x0e);
	}

	val = cfg->fbdiv & 0xff;
	inno_write(inno, 0xac, val);
	val = (cfg->fbdiv >> 8) & BIT(0);
	inno_update_bits(inno, 0xad, BIT(4), val);

	/* current bias clk/data 2 */
	val = phy_cfg->regs[0] << 4 | phy_cfg->regs[1];
	inno_write(inno, 0xbf, val);

	/* current bias data 1/0 */
	val = phy_cfg->regs[1] << 4 | phy_cfg->regs[1];
	inno_write(inno, 0xc0, val);

	/* output voltage */
	inno_write(inno, 0xb5, phy_cfg->regs[2]);
	inno_write(inno, 0xb6, phy_cfg->regs[3]);
	inno_write(inno, 0xb7, phy_cfg->regs[3]);
	inno_write(inno, 0xb8, phy_cfg->regs[3]);

	/* pre-emphasis */
	inno_write(inno, 0xbb, phy_cfg->regs[4]);
	inno_write(inno, 0xbc, phy_cfg->regs[4]);
	inno_write(inno, 0xbd, phy_cfg->regs[4]);

	/* enable LDO */
	inno_write(inno, 0xb4, 0x7);

	/* enable serializer */
	inno_write(inno, 0xbe, 0x70);

	inno_write(inno, 0xb2, 0x0f);

	for (val = 0; val < 5; val++) {
		if (inno_read(inno, 0xaf) & 1)
			break;
		udelay(1000);
	}
	if (!(inno_read(inno, 0xaf) & 1)) {
		dev_err(inno->dev, "HDMI PHY Post PLL unlock\n");
		return -ETIMEDOUT;
	}

	/* set termination resistance */
	if (phy_cfg->tmdsclock > 340000000) {
		inno_write(inno, 0xc7, 0x76);
		inno_write(inno, 0xc5, 0x83);
		inno_write(inno, 0xc8, 0x00);
		inno_write(inno, 0xc9, 0x2f);
		inno_write(inno, 0xca, 0x2f);
		inno_write(inno, 0xcb, 0x2f);
	} else {
		inno_write(inno, 0xc7, 0x76);
		inno_write(inno, 0xc5, 0x83);
		inno_write(inno, 0xc8, 0x00);
		inno_write(inno, 0xc9, 0x0f);
		inno_write(inno, 0xca, 0x0f);
		inno_write(inno, 0xcb, 0x0f);
	}


	/* set TMDS sync detection counter length */
	temp = 47520000000UL / tmdsclock;
	inno_write(inno, 0xd8, (temp >> 8) & 0xff);
	inno_write(inno, 0xd9, temp & 0xff);

	if (phy_cfg->tmdsclock > 340000000)
		mdelay(100);
	/* set pdata_en to 0/1 */
	inno_update_bits(inno, 0x02, 1, 0);
	inno_update_bits(inno, 0x02, 1, 1);

	/* Enable PHY IRQ */
	inno_write(inno, 0x05, 0x22);
	inno_write(inno, 0x07, 0x22);
	inno_write(inno, 0xcc, 0x0f);

	return 0;
}

static void inno_hdmi_phy_rk3528_power_off(struct inno_hdmi_phy *inno)
{
	/* Power off driver */
	inno_write(inno, 0xb2, 0);
	/* Power off band gap */
	inno_update_bits(inno, 0xb0, 4, 0);
	/* Power off post pll */
	inno_update_bits(inno, 0xaa, 1, 1);

	/* Disable PHY IRQ */
	inno_write(inno, 0x05, 0);
	inno_write(inno, 0x07, 0);
}

static void inno_hdmi_phy_rk3528_init(struct inno_hdmi_phy *inno)
{
	/*
	 * Use phy internal register control
	 * rxsense/poweron/pllpd/pdataen signal.
	 */
	inno_write(inno, 0x02, 0x81);
}

static int
inno_hdmi_phy_rk3528_pre_pll_update(struct inno_hdmi_phy *inno,
				    const struct pre_pll_config *cfg)
{
	u32 val;

	inno_update_bits(inno, 0xb0, 4, 4);
	inno_write(inno, 0xcc, 0x0f);

	/* Power on PLL */
	inno_update_bits(inno, 0xa0, 1, 0);
	/* Configure pre-pll */
	inno_update_bits(inno, 0xa0, 2, (cfg->vco_div_5_en & 1) << 1);
	inno_write(inno, 0xa1, cfg->prediv);
	if (cfg->fracdiv)
		val = ((cfg->fbdiv >> 8) & 0x0f) | 0xc0;
	else
		val = ((cfg->fbdiv >> 8) & 0x0f) | 0xf0;
	inno_write(inno, 0xa2, val);
	inno_write(inno, 0xa3, cfg->fbdiv & 0xff);
	val = (cfg->pclk_div_a & 0x1f) |
	      ((cfg->pclk_div_b & 3) << 5);
	inno_write(inno, 0xa5, val);
	val = (cfg->pclk_div_d & 0x1f) |
	      ((cfg->pclk_div_c & 3) << 5);
	inno_write(inno, 0xa6, val);
	val = ((cfg->tmds_div_a & 3) << 4) |
	      ((cfg->tmds_div_b & 3) << 2) |
	      (cfg->tmds_div_c & 3);
	inno_write(inno, 0xa4, val);

	if (cfg->fracdiv) {
		val = cfg->fracdiv & 0xff;
		inno_write(inno, 0xd3, val);
		val = (cfg->fracdiv >> 8) & 0xff;
		inno_write(inno, 0xd2, val);
		val = (cfg->fracdiv >> 16) & 0xff;
		inno_write(inno, 0xd1, val);
	} else {
		inno_write(inno, 0xd3, 0);
		inno_write(inno, 0xd2, 0);
		inno_write(inno, 0xd1, 0);
	}

	/* Wait for PLL lock */
	for (val = 0; val < 5; val++) {
		if (inno_read(inno, 0xa9) & 1)
			break;
		udelay(1000);
	}
	if (val == 5) {
		dev_err(inno->dev, "Pre-PLL unlock\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static unsigned long
inno_hdmi_rk3528_phy_pll_recalc_rate(struct inno_hdmi_phy *inno,
				     unsigned long parent_rate)
{
	unsigned long frac;
	u8 nd, no_a, no_b, no_d;
	u16 nf;
	u64 vco = parent_rate;

	nd = inno_read(inno, 0xa1) & 0x3f;
	nf = ((inno_read(inno, 0xa2) & 0x0f) << 8) | inno_read(inno, 0xa3);
	vco *= nf;
	if ((inno_read(inno, 0xa2) & 0x30) == 0) {
		frac = inno_read(inno, 0xd3) |
		       (inno_read(inno, 0xd2) << 8) |
		       (inno_read(inno, 0xd1) << 16);
		vco += DIV_ROUND_CLOSEST(parent_rate * frac, (1 << 24));
	}
	if (inno_read(inno, 0xa0) & 2) {
		do_div(vco, nd * 5);
	} else {
		no_a = inno_read(inno, 0xa5) & 0x1f;
		no_b = ((inno_read(inno, 0xa5) >> 5) & 7) + 2;
		no_d = inno_read(inno, 0xa6) & 0x1f;
		if (no_a == 1)
			do_div(vco, nd * no_b * no_d * 2);
		else
			do_div(vco, nd * no_a * no_d * 2);
	}

	frac = vco;
	inno->pixclock = DIV_ROUND_CLOSEST(frac, 1000) * 1000;

	dev_dbg(inno->dev, "%s rate %lu\n", __func__, inno->pixclock);

	return frac;
}

#ifndef CONFIG_SPL_BUILD
#define PHY_TAB_LEN 60

static
int inno_hdmi_update_phy_table(struct inno_hdmi_phy *inno, u32 *config,
			       struct phy_config *phy_cfg,
			       int phy_table_size)
{
	int i, j;

	for (i = 0; i < phy_table_size; i++) {
		phy_cfg[i].tmdsclock =
			(unsigned long)config[i * 15];

		debug("%ld ", phy_cfg[i].tmdsclock);
		for (j = 0; j < 14; j++) {
			phy_cfg[i].regs[j] = (u8)config[i * 15 + 1 + j];
			debug("0x%02x ", phy_cfg[i].regs[j]);
		}
		debug("\n");
	}

	/*
	 * The last set of phy cfg is used to indicate whether
	 * there is no more phy cfg data.
	 */
	phy_cfg[i].tmdsclock = ~0UL;
	for (j = 0; j < 14; j++)
		phy_cfg[i].regs[j] = 0;

	return 0;
}
#endif

static const struct inno_hdmi_phy_ops rk3228_hdmi_phy_ops = {
	.init = inno_hdmi_phy_rk3228_init,
	.power_on = inno_hdmi_phy_rk3228_power_on,
	.power_off = inno_hdmi_phy_rk3228_power_off,
	.pre_pll_update = inno_hdmi_phy_rk3228_pre_pll_update,
};

static const struct inno_hdmi_phy_ops rk3328_hdmi_phy_ops = {
	.init = inno_hdmi_phy_rk3328_init,
	.power_on = inno_hdmi_phy_rk3328_power_on,
	.power_off = inno_hdmi_phy_rk3328_power_off,
	.pre_pll_update = inno_hdmi_phy_rk3328_pre_pll_update,
	.recalc_rate = inno_hdmi_3328_phy_pll_recalc_rate,
};

static const struct inno_hdmi_phy_ops rk3528_hdmi_phy_ops = {
	.init = inno_hdmi_phy_rk3528_init,
	.power_on = inno_hdmi_phy_rk3528_power_on,
	.power_off = inno_hdmi_phy_rk3528_power_off,
	.pre_pll_update = inno_hdmi_phy_rk3528_pre_pll_update,
	.recalc_rate = inno_hdmi_rk3528_phy_pll_recalc_rate,
};

static const struct inno_hdmi_phy_drv_data rk3228_hdmi_phy_drv_data = {
	.dev_type = INNO_HDMI_PHY_RK3228,
	.ops = &rk3228_hdmi_phy_ops,
	.phy_cfg_table = rk3228_phy_cfg,
};

static const struct inno_hdmi_phy_drv_data rk3328_hdmi_phy_drv_data = {
	.dev_type = INNO_HDMI_PHY_RK3328,
	.ops = &rk3328_hdmi_phy_ops,
	.phy_cfg_table = rk3328_phy_cfg,
};

static const struct inno_hdmi_phy_drv_data rk3528_hdmi_phy_drv_data = {
	.dev_type = INNO_HDMI_PHY_RK3528,
	.ops = &rk3528_hdmi_phy_ops,
	.phy_cfg_table = rk3528_phy_cfg,
};

static const struct rockchip_inno_data inno_hdmi_phy_of_match[] = {
	{ .compatible = "rockchip,rk3228-hdmi-phy",
	  .data = &rk3228_hdmi_phy_drv_data
	},
	{ .compatible = "rockchip,rk3328-hdmi-phy",
	  .data = &rk3328_hdmi_phy_drv_data
	},
	{ .compatible = "rockchip,rk3528-hdmi-phy",
	  .data = &rk3528_hdmi_phy_drv_data
	},
	{}
};

static int inno_hdmi_phy_init(struct rockchip_phy *phy)
{
#ifdef CONFIG_SPL_BUILD
	struct inno_hdmi_phy *inno = (struct inno_hdmi_phy *)phy->data;
#else
	struct udevice *dev = phy->dev;
	struct inno_hdmi_phy *inno = dev_get_priv(phy->dev);
	int val, phy_table_size, ret;
	u32 *phy_config;
#endif
	int i;
	const char *name;

#ifdef CONFIG_SPL_BUILD
	inno->regs = (void *)RK3528_HDMIPHY_BASE;
#else
	inno->regs = dev_read_addr_ptr(dev);
	inno->node = dev->node;
#endif
	if (!inno->regs) {
		printf("%s: failed to get phy address\n", __func__);
		return -ENOMEM;
	}

#ifdef CONFIG_SPL_BUILD
	name = "rockchip,rk3528-hdmi-phy";
#else
	name = dev_read_string(dev, "compatible");
#endif
	for (i = 0; i < ARRAY_SIZE(inno_hdmi_phy_of_match); i++) {
		if (!strcmp(name, inno_hdmi_phy_of_match[i].compatible)) {
			inno->plat_data = inno_hdmi_phy_of_match[i].data;
			break;
		}
	}

#ifndef CONFIG_SPL_BUILD
	dev_read_prop(dev, "rockchip,phy-table", &val);

	if (val >= 0) {
		if (val % PHY_TAB_LEN || !val) {
			printf("Invalid phy cfg table format!\n");
			return -EINVAL;
		}

		phy_config = malloc(val);
		if (!phy_config) {
			printf("kmalloc phy table failed\n");
			return -ENOMEM;
		}

		phy_table_size = val / PHY_TAB_LEN;
		/* Effective phy cfg data and the end of phy cfg table */
		inno->phy_cfg = malloc(val + PHY_TAB_LEN);
		if (!inno->phy_cfg) {
			free(phy_config);
			return -ENOMEM;
		}

		dev_read_u32_array(dev, "rockchip,phy-table",
				   phy_config, val / sizeof(u32));
		ret = inno_hdmi_update_phy_table(inno, phy_config,
						 inno->phy_cfg,
						 phy_table_size);
		if (ret) {
			free(phy_config);
			return ret;
		}
		free(phy_config);
	} else {
		printf("use default hdmi phy table\n");
	}
#endif

	if (i >= ARRAY_SIZE(inno_hdmi_phy_of_match))
		return 0;

	if (!inno->plat_data || !inno->plat_data->ops)
		return -EINVAL;

	if (inno->plat_data->ops->init)
		inno->plat_data->ops->init(inno);

	return 0;
}

static unsigned long inno_hdmi_phy_set_pll(struct rockchip_phy *phy,
					   unsigned long rate)
{
#ifdef CONFIG_SPL_BUILD
	struct inno_hdmi_phy *inno = (struct inno_hdmi_phy *)phy->data;
#else
	struct inno_hdmi_phy *inno = dev_get_priv(phy->dev);
#endif

#ifdef CONFIG_SPL_BUILD
	if (!inno)
		inno = g_inno;
#endif
	inno_hdmi_phy_clk_prepare(inno);
	inno_hdmi_phy_clk_is_prepared(inno);
	inno_hdmi_phy_clk_set_rate(inno, rate);
	return 0;
}

static int
inno_hdmi_phy_set_bus_width(struct rockchip_phy *phy, u32 bus_width)
{
#ifdef CONFIG_SPL_BUILD
	struct inno_hdmi_phy *inno = (struct inno_hdmi_phy *)phy->data;
#else
	struct inno_hdmi_phy *inno = dev_get_priv(phy->dev);
#endif

	inno->bus_width = bus_width;

	return 0;
}

static long
inno_hdmi_phy_clk_round_rate(struct rockchip_phy *phy, unsigned long rate)
{
#ifdef CONFIG_SPL_BUILD
	struct inno_hdmi_phy *inno = (struct inno_hdmi_phy *)phy->data;
#else
	struct inno_hdmi_phy *inno = dev_get_priv(phy->dev);
#endif
	int i;
	const struct pre_pll_config *cfg = pre_pll_cfg_table;
	u32 tmdsclock = inno_hdmi_phy_get_tmdsclk(inno, rate);

	for (; cfg->pixclock != ~0UL; cfg++)
		if (cfg->pixclock == rate)
			break;

	/*
	 * XXX: Limit pixel clock under 600MHz
	 * rk3228 does not support non-zero fracdiv
	 */
	if ((inno->plat_data->dev_type == INNO_HDMI_PHY_RK3228 &&
	     cfg->fracdiv) || cfg->pixclock > 600000000)
		return -EINVAL;

	/*
	 * If there is no dts phy cfg table, use default phy cfg table.
	 * The tmds clock maximum is 594MHz. So there is no need to check
	 * whether tmds clock is out of range.
	 */
	if (!inno->phy_cfg)
		return cfg->pixclock;

	/* Check if tmds clock is out of dts phy config's range. */
	for (i = 0; inno->phy_cfg[i].tmdsclock != ~0UL; i++) {
		if (inno->phy_cfg[i].tmdsclock >= tmdsclock)
			break;
	}

	if (inno->phy_cfg[i].tmdsclock == ~0UL)
		return -EINVAL;

	return cfg->pixclock;
}

const struct rockchip_phy_funcs inno_hdmi_phy_funcs = {
	.init = inno_hdmi_phy_init,
	.power_on = inno_hdmi_phy_power_on,
	.power_off = inno_hdmi_phy_power_off,
	.set_pll = inno_hdmi_phy_set_pll,
	.set_bus_width = inno_hdmi_phy_set_bus_width,
	.round_rate = inno_hdmi_phy_clk_round_rate,
};

static struct rockchip_phy inno_hdmi_phy_driver_data = {
	 .funcs = &inno_hdmi_phy_funcs,
};

static const struct udevice_id inno_hdmi_phy_ids[] = {
	{
	 .compatible = "rockchip,rk3328-hdmi-phy",
	 .data = (ulong)&inno_hdmi_phy_driver_data,
	},
	{
	 .compatible = "rockchip,rk3228-hdmi-phy",
	 .data = (ulong)&inno_hdmi_phy_driver_data,
	},
	{
	 .compatible = "rockchip,rk3528-hdmi-phy",
	 .data = (ulong)&inno_hdmi_phy_driver_data,
	},
	{}
};

#ifdef CONFIG_SPL_BUILD
int inno_spl_hdmi_phy_probe(struct display_state *state)
{
	struct inno_hdmi_phy *inno = malloc(sizeof(struct inno_hdmi_phy));

	memset(inno, 0, sizeof(*inno));
	g_inno = inno;

	state->conn_state.connector->phy = &inno_hdmi_phy_driver_data;
	state->conn_state.connector->phy->data = (void *)inno;
	return 0;
}
#else
static int inno_hdmi_phy_probe(struct udevice *dev)
{
	struct inno_hdmi_phy *inno = dev_get_priv(dev);
	struct rockchip_phy *phy =
		(struct rockchip_phy *)dev_get_driver_data(dev);

	inno->dev = dev;
	phy->dev = dev;

	g_inno = inno;
	dev->driver_data = (ulong)&inno_hdmi_phy_driver_data;
	phy = &inno_hdmi_phy_driver_data;

	return 0;
}
#endif

static int rockchip_inno_phy_hdmi_bind(struct udevice *parent)
{
	struct udevice *child;
	ofnode subnode;
	int ret;

	subnode = ofnode_find_subnode(parent->node, "clk-port");
	if (!ofnode_valid(subnode)) {
		printf("%s: no subnode for %s\n", __func__, parent->name);
		return -ENXIO;
	}

	ret = device_bind_driver_to_node(parent, "clk_inno_hdmi", "inno_hdmi_pll_clk", subnode, &child);
	if (ret) {
		printf("%s: clk-port cannot bind its driver\n", __func__);
		return ret;
	}

	return 0;
}

U_BOOT_DRIVER(inno_hdmi_phy) = {
	.name = "inno_hdmi_phy",
	.id = UCLASS_PHY,
	.of_match = inno_hdmi_phy_ids,
#ifndef CONFIG_SPL_BUILD
	.probe = inno_hdmi_phy_probe,
#endif
	.bind = rockchip_inno_phy_hdmi_bind,
	.priv_auto_alloc_size = sizeof(struct inno_hdmi_phy),
};


static ulong inno_hdmi_clk_get_rate(struct clk *clk)
{
	struct clk_inno_hdmi *priv = dev_get_priv(clk->dev);

	return priv->rate;
}

static ulong inno_hdmi_clk_set_rate(struct clk *clk, ulong rate)
{
	struct clk_inno_hdmi *priv = dev_get_priv(clk->dev);
	int ret;

	inno_hdmi_phy_clk_prepare(g_inno);
	inno_hdmi_phy_clk_is_prepared(g_inno);
	ret = inno_hdmi_phy_clk_set_rate(g_inno, rate);
	if (ret < 0) {
		printf("inno hdmi set rate failed ret:%d\n", ret);
		return ret;
	}

	priv->rate = g_inno->pixclock;

	return priv->rate;
}

static const struct clk_ops inno_hdmi_clk_ops = {
	.get_rate	= inno_hdmi_clk_get_rate,
	.set_rate	= inno_hdmi_clk_set_rate,
};

static int inno_hdmi_clk_probe(struct udevice *dev)
{
	return 0;
}

/*
 * In order for other display interfaces to use hdmiphy as source
 * for dclk, hdmiphy must register a virtual clock driver
 */
U_BOOT_DRIVER(clk_inno_hdmi) = {
	.name		= "clk_inno_hdmi",
	.id		= UCLASS_CLK,
	.priv_auto_alloc_size = sizeof(struct clk_inno_hdmi),
	.ops		= &inno_hdmi_clk_ops,
	.probe		= inno_hdmi_clk_probe,
};
