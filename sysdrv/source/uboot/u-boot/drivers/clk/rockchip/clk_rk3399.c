/*
 * (C) Copyright 2015 Google, Inc
 * (C) 2017 Theobroma Systems Design und Consulting GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <dt-structs.h>
#include <errno.h>
#include <mapmem.h>
#include <syscon.h>
#include <bitfield.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/cru_rk3399.h>
#include <asm/arch/hardware.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rk3399-cru.h>

DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_IS_ENABLED(OF_PLATDATA)
struct rk3399_clk_plat {
	struct dtd_rockchip_rk3399_cru dtd;
};

struct rk3399_pmuclk_plat {
	struct dtd_rockchip_rk3399_pmucru dtd;
};
#endif

struct pll_div {
	u32 refdiv;
	u32 fbdiv;
	u32 postdiv1;
	u32 postdiv2;
	u32 frac;
	u32 freq;
};

#define RATE_TO_DIV(input_rate, output_rate) \
	((input_rate) / (output_rate) - 1);
#define DIV_TO_RATE(input_rate, div)    ((input_rate) / ((div) + 1))

#define PLL_DIVISORS(hz, _refdiv, _postdiv1, _postdiv2) {\
	.refdiv = _refdiv,\
	.fbdiv = (u32)((u64)hz * _refdiv * _postdiv1 * _postdiv2 / OSC_HZ),\
	.postdiv1 = _postdiv1, .postdiv2 = _postdiv2, .freq = hz};

#if !defined(CONFIG_SPL_BUILD)
static const struct pll_div ppll_init_cfg = PLL_DIVISORS(PPLL_HZ, 2, 2, 1);
#endif
static const struct pll_div gpll_init_cfg = PLL_DIVISORS(GPLL_HZ, 1, 3, 1);
static const struct pll_div npll_init_cfg = PLL_DIVISORS(NPLL_HZ, 1, 3, 1);
static const struct pll_div apll_1600_cfg = PLL_DIVISORS(1600*MHz, 3, 1, 1);
static const struct pll_div apll_816_cfg = PLL_DIVISORS(816 * MHz, 1, 2, 1);
static const struct pll_div apll_600_cfg = PLL_DIVISORS(600*MHz, 1, 2, 1);

static const struct pll_div *apll_cfgs[] = {
	[APLL_1600_MHZ] = &apll_1600_cfg,
	[APLL_816_MHZ] = &apll_816_cfg,
	[APLL_600_MHZ] = &apll_600_cfg,
};

#ifndef CONFIG_SPL_BUILD
#define RK3399_CLK_DUMP(_id, _name, _iscru)    \
{                                              \
	.id = _id,                              \
	.name = _name,                          \
	.is_cru = _iscru,                       \
}

static const struct rk3399_clk_info clks_dump[] = {
	RK3399_CLK_DUMP(PLL_APLLL, "aplll", true),
	RK3399_CLK_DUMP(PLL_APLLB, "apllb", true),
	RK3399_CLK_DUMP(PLL_DPLL, "dpll", true),
	RK3399_CLK_DUMP(PLL_CPLL, "cpll", true),
	RK3399_CLK_DUMP(PLL_GPLL, "gpll", true),
	RK3399_CLK_DUMP(PLL_NPLL, "npll", true),
	RK3399_CLK_DUMP(PLL_VPLL, "vpll", true),
	RK3399_CLK_DUMP(ACLK_PERIHP, "aclk_perihp", true),
	RK3399_CLK_DUMP(HCLK_PERIHP, "hclk_perihp", true),
	RK3399_CLK_DUMP(PCLK_PERIHP, "pclk_perihp", true),
	RK3399_CLK_DUMP(ACLK_PERILP0, "aclk_perilp0", true),
	RK3399_CLK_DUMP(HCLK_PERILP0, "hclk_perilp0", true),
	RK3399_CLK_DUMP(PCLK_PERILP0, "pclk_perilp0", true),
	RK3399_CLK_DUMP(HCLK_PERILP1, "hclk_perilp1", true),
	RK3399_CLK_DUMP(PCLK_PERILP1, "pclk_perilp1", true),
};
#endif

enum {
	/* PLL_CON0 */
	PLL_FBDIV_MASK			= 0xfff,
	PLL_FBDIV_SHIFT			= 0,

	/* PLL_CON1 */
	PLL_POSTDIV2_SHIFT		= 12,
	PLL_POSTDIV2_MASK		= 0x7 << PLL_POSTDIV2_SHIFT,
	PLL_POSTDIV1_SHIFT		= 8,
	PLL_POSTDIV1_MASK		= 0x7 << PLL_POSTDIV1_SHIFT,
	PLL_REFDIV_MASK			= 0x3f,
	PLL_REFDIV_SHIFT		= 0,

	/* PLL_CON2 */
	PLL_LOCK_STATUS_SHIFT		= 31,
	PLL_LOCK_STATUS_MASK		= 1 << PLL_LOCK_STATUS_SHIFT,
	PLL_FRACDIV_MASK		= 0xffffff,
	PLL_FRACDIV_SHIFT		= 0,

	/* PLL_CON3 */
	PLL_MODE_SHIFT			= 8,
	PLL_MODE_MASK			= 3 << PLL_MODE_SHIFT,
	PLL_MODE_SLOW			= 0,
	PLL_MODE_NORM,
	PLL_MODE_DEEP,
	PLL_DSMPD_SHIFT			= 3,
	PLL_DSMPD_MASK			= 1 << PLL_DSMPD_SHIFT,
	PLL_INTEGER_MODE		= 1,

	/* PMUCRU_CLKSEL_CON0 */
	PMU_PCLK_DIV_CON_MASK		= 0x1f,
	PMU_PCLK_DIV_CON_SHIFT		= 0,

	/* PMUCRU_CLKSEL_CON1 */
	SPI3_PLL_SEL_SHIFT		= 7,
	SPI3_PLL_SEL_MASK		= 1 << SPI3_PLL_SEL_SHIFT,
	SPI3_PLL_SEL_24M		= 0,
	SPI3_PLL_SEL_PPLL		= 1,
	SPI3_DIV_CON_SHIFT		= 0x0,
	SPI3_DIV_CON_MASK		= 0x7f,

	/* PMUCRU_CLKSEL_CON2 */
	I2C_DIV_CON_MASK		= 0x7f,
	CLK_I2C8_DIV_CON_SHIFT		= 8,
	CLK_I2C0_DIV_CON_SHIFT		= 0,

	/* PMUCRU_CLKSEL_CON3 */
	CLK_I2C4_DIV_CON_SHIFT		= 0,

	/* CLKSEL_CON0 / CLKSEL_CON2 */
	ACLKM_CORE_DIV_CON_MASK	= 0x1f,
	ACLKM_CORE_DIV_CON_SHIFT	= 8,
	CLK_CORE_PLL_SEL_MASK		= 3,
	CLK_CORE_PLL_SEL_SHIFT		= 6,
	CLK_CORE_PLL_SEL_ALPLL		= 0x0,
	CLK_CORE_PLL_SEL_ABPLL		= 0x1,
	CLK_CORE_PLL_SEL_DPLL		= 0x10,
	CLK_CORE_PLL_SEL_GPLL		= 0x11,
	CLK_CORE_DIV_MASK		= 0x1f,
	CLK_CORE_DIV_SHIFT		= 0,

	/* CLKSEL_CON1 / CLKSEL_CON3 */
	PCLK_DBG_DIV_MASK		= 0x1f,
	PCLK_DBG_DIV_SHIFT		= 0x8,
	ATCLK_CORE_DIV_MASK		= 0x1f,
	ATCLK_CORE_DIV_SHIFT		= 0,

	/* CLKSEL_CON14 */
	PCLK_PERIHP_DIV_CON_SHIFT	= 12,
	PCLK_PERIHP_DIV_CON_MASK	= 0x7 << PCLK_PERIHP_DIV_CON_SHIFT,
	HCLK_PERIHP_DIV_CON_SHIFT	= 8,
	HCLK_PERIHP_DIV_CON_MASK	= 3 << HCLK_PERIHP_DIV_CON_SHIFT,
	ACLK_PERIHP_PLL_SEL_SHIFT	= 7,
	ACLK_PERIHP_PLL_SEL_MASK	= 1 << ACLK_PERIHP_PLL_SEL_SHIFT,
	ACLK_PERIHP_PLL_SEL_CPLL	= 0,
	ACLK_PERIHP_PLL_SEL_GPLL	= 1,
	ACLK_PERIHP_DIV_CON_SHIFT	= 0,
	ACLK_PERIHP_DIV_CON_MASK	= 0x1f,

	/* CLKSEL_CON21 */
	ACLK_EMMC_PLL_SEL_SHIFT         = 7,
	ACLK_EMMC_PLL_SEL_MASK          = 0x1 << ACLK_EMMC_PLL_SEL_SHIFT,
	ACLK_EMMC_PLL_SEL_GPLL          = 0x1,
	ACLK_EMMC_DIV_CON_SHIFT         = 0,
	ACLK_EMMC_DIV_CON_MASK          = 0x1f,

	/* CLKSEL_CON22 */
	CLK_EMMC_PLL_SHIFT              = 8,
	CLK_EMMC_PLL_MASK               = 0x7 << CLK_EMMC_PLL_SHIFT,
	CLK_EMMC_PLL_SEL_GPLL           = 0x1,
	CLK_EMMC_PLL_SEL_24M            = 0x5,
	CLK_EMMC_DIV_CON_SHIFT          = 0,
	CLK_EMMC_DIV_CON_MASK           = 0x7f << CLK_EMMC_DIV_CON_SHIFT,

	/* CLKSEL_CON23 */
	PCLK_PERILP0_DIV_CON_SHIFT	= 12,
	PCLK_PERILP0_DIV_CON_MASK	= 0x7 << PCLK_PERILP0_DIV_CON_SHIFT,
	HCLK_PERILP0_DIV_CON_SHIFT	= 8,
	HCLK_PERILP0_DIV_CON_MASK	= 3 << HCLK_PERILP0_DIV_CON_SHIFT,
	ACLK_PERILP0_PLL_SEL_SHIFT	= 7,
	ACLK_PERILP0_PLL_SEL_MASK	= 1 << ACLK_PERILP0_PLL_SEL_SHIFT,
	ACLK_PERILP0_PLL_SEL_CPLL	= 0,
	ACLK_PERILP0_PLL_SEL_GPLL	= 1,
	ACLK_PERILP0_DIV_CON_SHIFT	= 0,
	ACLK_PERILP0_DIV_CON_MASK	= 0x1f,

	/* CRU_CLK_SEL24_CON */
	CRYPTO0_PLL_SEL_SHIFT		= 6,
	CRYPTO0_PLL_SEL_MASK		= 3 << CRYPTO0_PLL_SEL_SHIFT,
	CRYPTO_PLL_SEL_CPLL		= 0,
	CRYPTO_PLL_SEL_GPLL,
	CRYPTO_PLL_SEL_PPLL		= 0,
	CRYPTO0_DIV_SHIFT		= 0,
	CRYPTO0_DIV_MASK		= 0x1f << CRYPTO0_DIV_SHIFT,

	/* CLKSEL_CON25 */
	PCLK_PERILP1_DIV_CON_SHIFT	= 8,
	PCLK_PERILP1_DIV_CON_MASK	= 0x7 << PCLK_PERILP1_DIV_CON_SHIFT,
	HCLK_PERILP1_PLL_SEL_SHIFT	= 7,
	HCLK_PERILP1_PLL_SEL_MASK	= 1 << HCLK_PERILP1_PLL_SEL_SHIFT,
	HCLK_PERILP1_PLL_SEL_CPLL	= 0,
	HCLK_PERILP1_PLL_SEL_GPLL	= 1,
	HCLK_PERILP1_DIV_CON_SHIFT	= 0,
	HCLK_PERILP1_DIV_CON_MASK	= 0x1f,

	/* CLKSEL_CON26 */
	CLK_SARADC_DIV_CON_SHIFT	= 8,
	CLK_SARADC_DIV_CON_MASK		= GENMASK(15, 8),
	CLK_SARADC_DIV_CON_WIDTH	= 8,
	CRYPTO1_PLL_SEL_SHIFT		= 6,
	CRYPTO1_PLL_SEL_MASK		= 3 << CRYPTO1_PLL_SEL_SHIFT,
	CRYPTO1_DIV_SHIFT		= 0,
	CRYPTO1_DIV_MASK		= 0x1f << CRYPTO1_DIV_SHIFT,

	/* CLKSEL_CON27 */
	CLK_TSADC_SEL_X24M		= 0x0,
	CLK_TSADC_SEL_SHIFT		= 15,
	CLK_TSADC_SEL_MASK		= 1 << CLK_TSADC_SEL_SHIFT,
	CLK_TSADC_DIV_CON_SHIFT		= 0,
	CLK_TSADC_DIV_CON_MASK		= 0x3ff,

	/* CLKSEL_CON47 & CLKSEL_CON48 */
	ACLK_VOP_PLL_SEL_SHIFT		= 6,
	ACLK_VOP_PLL_SEL_MASK		= 0x3 << ACLK_VOP_PLL_SEL_SHIFT,
	ACLK_VOP_PLL_SEL_CPLL		= 0x1,
	ACLK_VOP_PLL_SEL_GPLL		= 0x2,
	ACLK_VOP_DIV_CON_SHIFT		= 0,
	ACLK_VOP_DIV_CON_MASK		= 0x1f << ACLK_VOP_DIV_CON_SHIFT,

	/* CLKSEL_CON49 & CLKSEL_CON50 */
	DCLK_VOP_DCLK_SEL_SHIFT         = 11,
	DCLK_VOP_DCLK_SEL_MASK          = 1 << DCLK_VOP_DCLK_SEL_SHIFT,
	DCLK_VOP_DCLK_SEL_DIVOUT        = 0,
	DCLK_VOP_PLL_SEL_SHIFT          = 8,
	DCLK_VOP_PLL_SEL_MASK           = 3 << DCLK_VOP_PLL_SEL_SHIFT,
	DCLK_VOP_PLL_SEL_VPLL           = 0,
	DCLK_VOP_PLL_SEL_CPLL           = 1,
	DCLK_VOP_DIV_CON_MASK           = 0xff,
	DCLK_VOP_DIV_CON_SHIFT          = 0,

	/* CLKSEL_CON57 */
	PCLK_ALIVE_DIV_CON_SHIFT	= 0,
	PCLK_ALIVE_DIV_CON_MASK		= 0x1f << PCLK_ALIVE_DIV_CON_SHIFT,

	/* CLKSEL_CON58 */
	CLK_SPI_PLL_SEL_WIDTH = 1,
	CLK_SPI_PLL_SEL_MASK = ((1 < CLK_SPI_PLL_SEL_WIDTH) - 1),
	CLK_SPI_PLL_SEL_CPLL = 0,
	CLK_SPI_PLL_SEL_GPLL = 1,
	CLK_SPI_PLL_DIV_CON_WIDTH = 7,
	CLK_SPI_PLL_DIV_CON_MASK = ((1 << CLK_SPI_PLL_DIV_CON_WIDTH) - 1),

	CLK_SPI5_PLL_DIV_CON_SHIFT      = 8,
	CLK_SPI5_PLL_SEL_SHIFT	        = 15,

	/* CLKSEL_CON59 */
	CLK_SPI1_PLL_SEL_SHIFT		= 15,
	CLK_SPI1_PLL_DIV_CON_SHIFT	= 8,
	CLK_SPI0_PLL_SEL_SHIFT		= 7,
	CLK_SPI0_PLL_DIV_CON_SHIFT	= 0,

	/* CLKSEL_CON60 */
	CLK_SPI4_PLL_SEL_SHIFT		= 15,
	CLK_SPI4_PLL_DIV_CON_SHIFT	= 8,
	CLK_SPI2_PLL_SEL_SHIFT		= 7,
	CLK_SPI2_PLL_DIV_CON_SHIFT	= 0,

	/* CLKSEL_CON61 */
	CLK_I2C_PLL_SEL_MASK		= 1,
	CLK_I2C_PLL_SEL_CPLL		= 0,
	CLK_I2C_PLL_SEL_GPLL		= 1,
	CLK_I2C5_PLL_SEL_SHIFT		= 15,
	CLK_I2C5_DIV_CON_SHIFT		= 8,
	CLK_I2C1_PLL_SEL_SHIFT		= 7,
	CLK_I2C1_DIV_CON_SHIFT		= 0,

	/* CLKSEL_CON62 */
	CLK_I2C6_PLL_SEL_SHIFT		= 15,
	CLK_I2C6_DIV_CON_SHIFT		= 8,
	CLK_I2C2_PLL_SEL_SHIFT		= 7,
	CLK_I2C2_DIV_CON_SHIFT		= 0,

	/* CLKSEL_CON63 */
	CLK_I2C7_PLL_SEL_SHIFT		= 15,
	CLK_I2C7_DIV_CON_SHIFT		= 8,
	CLK_I2C3_PLL_SEL_SHIFT		= 7,
	CLK_I2C3_DIV_CON_SHIFT		= 0,

	/* CRU_SOFTRST_CON4 */
	RESETN_DDR0_REQ_SHIFT		= 8,
	RESETN_DDR0_REQ_MASK		= 1 << RESETN_DDR0_REQ_SHIFT,
	RESETN_DDRPHY0_REQ_SHIFT	= 9,
	RESETN_DDRPHY0_REQ_MASK		= 1 << RESETN_DDRPHY0_REQ_SHIFT,
	RESETN_DDR1_REQ_SHIFT		= 12,
	RESETN_DDR1_REQ_MASK		= 1 << RESETN_DDR1_REQ_SHIFT,
	RESETN_DDRPHY1_REQ_SHIFT	= 13,
	RESETN_DDRPHY1_REQ_MASK		= 1 << RESETN_DDRPHY1_REQ_SHIFT,
};

#define VCO_MAX_KHZ	(3200 * (MHz / KHz))
#define VCO_MIN_KHZ	(800 * (MHz / KHz))
#define OUTPUT_MAX_KHZ	(3200 * (MHz / KHz))
#define OUTPUT_MIN_KHZ	(16 * (MHz / KHz))

/*
 *  the div restructions of pll in integer mode, these are defined in
 *  * CRU_*PLL_CON0 or PMUCRU_*PLL_CON0
 */
#define PLL_DIV_MIN	16
#define PLL_DIV_MAX	3200

/*
 * How to calculate the PLL(from TRM V0.3 Part 1 Page 63):
 * Formulas also embedded within the Fractional PLL Verilog model:
 * If DSMPD = 1 (DSM is disabled, "integer mode")
 * FOUTVCO = FREF / REFDIV * FBDIV
 * FOUTPOSTDIV = FOUTVCO / POSTDIV1 / POSTDIV2
 * Where:
 * FOUTVCO = Fractional PLL non-divided output frequency
 * FOUTPOSTDIV = Fractional PLL divided output frequency
 *               (output of second post divider)
 * FREF = Fractional PLL input reference frequency, (the OSC_HZ 24MHz input)
 * REFDIV = Fractional PLL input reference clock divider
 * FBDIV = Integer value programmed into feedback divide
 *
 */

static uint32_t rkclk_pll_get_rate(u32 *pll_con)
{
	u32 refdiv, fbdiv, postdiv1, postdiv2;
	u32 con;

	con = readl(&pll_con[3]);
	switch ((con & PLL_MODE_MASK) >> PLL_MODE_SHIFT) {
	case PLL_MODE_SLOW:
		return OSC_HZ;
	case PLL_MODE_NORM:
		/* normal mode */
		con = readl(&pll_con[0]);
		fbdiv = (con & PLL_FBDIV_MASK) >> PLL_FBDIV_SHIFT;
		con = readl(&pll_con[1]);
		postdiv1 = (con & PLL_POSTDIV1_MASK) >> PLL_POSTDIV1_SHIFT;
		postdiv2 = (con & PLL_POSTDIV2_MASK) >> PLL_POSTDIV2_SHIFT;
		refdiv = (con & PLL_REFDIV_MASK) >> PLL_REFDIV_SHIFT;
		return (24 * fbdiv / (refdiv * postdiv1 * postdiv2)) * 1000000;
	case PLL_MODE_DEEP:
	default:
		return 32768;
	}
}

static void rkclk_set_pll(u32 *pll_con, const struct pll_div *div)
{
	/* All 8 PLLs have same VCO and output frequency range restrictions. */
	u32 vco_khz = OSC_HZ / 1000 * div->fbdiv / div->refdiv;
	u32 output_khz = vco_khz / div->postdiv1 / div->postdiv2;

	debug("PLL at %p: fbdiv=%d, refdiv=%d, postdiv1=%d, "
			   "postdiv2=%d, vco=%u khz, output=%u khz\n",
			   pll_con, div->fbdiv, div->refdiv, div->postdiv1,
			   div->postdiv2, vco_khz, output_khz);
	assert(vco_khz >= VCO_MIN_KHZ && vco_khz <= VCO_MAX_KHZ &&
	       output_khz >= OUTPUT_MIN_KHZ && output_khz <= OUTPUT_MAX_KHZ &&
	       div->fbdiv >= PLL_DIV_MIN && div->fbdiv <= PLL_DIV_MAX);

	/*
	 * When power on or changing PLL setting,
	 * we must force PLL into slow mode to ensure output stable clock.
	 */
	rk_clrsetreg(&pll_con[3], PLL_MODE_MASK,
		     PLL_MODE_SLOW << PLL_MODE_SHIFT);

	/* use integer mode */
	rk_clrsetreg(&pll_con[3], PLL_DSMPD_MASK,
		     PLL_INTEGER_MODE << PLL_DSMPD_SHIFT);

	rk_clrsetreg(&pll_con[0], PLL_FBDIV_MASK,
		     div->fbdiv << PLL_FBDIV_SHIFT);
	rk_clrsetreg(&pll_con[1],
		     PLL_POSTDIV2_MASK | PLL_POSTDIV1_MASK |
		     PLL_REFDIV_MASK | PLL_REFDIV_SHIFT,
		     (div->postdiv2 << PLL_POSTDIV2_SHIFT) |
		     (div->postdiv1 << PLL_POSTDIV1_SHIFT) |
		     (div->refdiv << PLL_REFDIV_SHIFT));

	/* waiting for pll lock */
	while (!(readl(&pll_con[2]) & (1 << PLL_LOCK_STATUS_SHIFT)))
		udelay(1);

	/* pll enter normal mode */
	rk_clrsetreg(&pll_con[3], PLL_MODE_MASK,
		     PLL_MODE_NORM << PLL_MODE_SHIFT);
}

static ulong rk3399_pll_get_rate(struct rk3399_clk_priv *priv,
				 enum rk3399_pll_id pll_id)
{
	struct rk3399_cru *cru = priv->cru;
	u32 *pll_con;

	switch (pll_id) {
	case PLL_APLLL:
		pll_con = &cru->apll_l_con[0];
		break;
	case PLL_APLLB:
		pll_con = &cru->apll_b_con[0];
		break;
	case PLL_DPLL:
		pll_con = &cru->dpll_con[0];
		break;
	case PLL_CPLL:
		pll_con = &cru->cpll_con[0];
		break;
	case PLL_GPLL:
		pll_con = &cru->gpll_con[0];
		break;
	case PLL_NPLL:
		pll_con = &cru->npll_con[0];
		break;
	case PLL_VPLL:
		pll_con = &cru->vpll_con[0];
		break;
	default:
		pll_con = &cru->vpll_con[0];
		break;
	}

	return rkclk_pll_get_rate(pll_con);
}

static int pll_para_config(u32 freq_hz, struct pll_div *div)
{
	u32 ref_khz = OSC_HZ / KHz, refdiv, fbdiv = 0;
	u32 postdiv1, postdiv2 = 1;
	u32 fref_khz;
	u32 diff_khz, best_diff_khz;
	const u32 max_refdiv = 63, max_fbdiv = 3200, min_fbdiv = 16;
	const u32 max_postdiv1 = 7, max_postdiv2 = 7;
	u32 vco_khz;
	u32 freq_khz = freq_hz / KHz;

	if (!freq_hz) {
		printf("%s: the frequency can't be 0 Hz\n", __func__);
		return -1;
	}

	postdiv1 = DIV_ROUND_UP(VCO_MIN_KHZ, freq_khz);
	if (postdiv1 > max_postdiv1) {
		postdiv2 = DIV_ROUND_UP(postdiv1, max_postdiv1);
		postdiv1 = DIV_ROUND_UP(postdiv1, postdiv2);
	}

	vco_khz = freq_khz * postdiv1 * postdiv2;

	if (vco_khz < VCO_MIN_KHZ || vco_khz > VCO_MAX_KHZ ||
	    postdiv2 > max_postdiv2) {
		printf("%s: Cannot find out a supported VCO"
		       " for Frequency (%uHz).\n", __func__, freq_hz);
		return -1;
	}

	div->postdiv1 = postdiv1;
	div->postdiv2 = postdiv2;

	best_diff_khz = vco_khz;
	for (refdiv = 1; refdiv < max_refdiv && best_diff_khz; refdiv++) {
		fref_khz = ref_khz / refdiv;

		fbdiv = vco_khz / fref_khz;
		if ((fbdiv >= max_fbdiv) || (fbdiv <= min_fbdiv))
			continue;
		diff_khz = vco_khz - fbdiv * fref_khz;
		if (fbdiv + 1 < max_fbdiv && diff_khz > fref_khz / 2) {
			fbdiv++;
			diff_khz = fref_khz - diff_khz;
		}

		if (diff_khz >= best_diff_khz)
			continue;

		best_diff_khz = diff_khz;
		div->refdiv = refdiv;
		div->fbdiv = fbdiv;
	}

	if (best_diff_khz > 4 * (MHz/KHz)) {
		printf("%s: Failed to match output frequency %u, "
		       "difference is %u Hz,exceed 4MHZ\n", __func__, freq_hz,
		       best_diff_khz * KHz);
		return -1;
	}
	return 0;
}

void rk3399_configure_cpu(struct rk3399_cru *cru,
			  enum apll_frequencies freq,
			  enum cpu_cluster cluster)
{
	u32 aclkm_div;
	u32 pclk_dbg_div;
	u32 atclk_div, apll_hz;
	int con_base, parent;
	u32 *pll_con;

	switch (cluster) {
	case CPU_CLUSTER_LITTLE:
		con_base = 0;
		parent = CLK_CORE_PLL_SEL_ALPLL;
		pll_con = &cru->apll_l_con[0];
		break;
	case CPU_CLUSTER_BIG:
	default:
		con_base = 2;
		parent = CLK_CORE_PLL_SEL_ABPLL;
		pll_con = &cru->apll_b_con[0];
		break;
	}

	apll_hz = apll_cfgs[freq]->freq;
	rkclk_set_pll(pll_con, apll_cfgs[freq]);

	aclkm_div = apll_hz / ACLKM_CORE_HZ - 1;
	assert((aclkm_div + 1) * ACLKM_CORE_HZ <= apll_hz &&
	       aclkm_div < 0x1f);

	pclk_dbg_div = apll_hz / PCLK_DBG_HZ - 1;
	assert((pclk_dbg_div + 1) * PCLK_DBG_HZ <= apll_hz &&
	       pclk_dbg_div < 0x1f);

	atclk_div = apll_hz / ATCLK_CORE_HZ - 1;
	assert((atclk_div + 1) * ATCLK_CORE_HZ <= apll_hz &&
	       atclk_div < 0x1f);

	rk_clrsetreg(&cru->clksel_con[con_base],
		     ACLKM_CORE_DIV_CON_MASK | CLK_CORE_PLL_SEL_MASK |
		     CLK_CORE_DIV_MASK,
		     aclkm_div << ACLKM_CORE_DIV_CON_SHIFT |
		     parent << CLK_CORE_PLL_SEL_SHIFT |
		     0 << CLK_CORE_DIV_SHIFT);

	rk_clrsetreg(&cru->clksel_con[con_base + 1],
		     PCLK_DBG_DIV_MASK | ATCLK_CORE_DIV_MASK,
		     pclk_dbg_div << PCLK_DBG_DIV_SHIFT |
		     atclk_div << ATCLK_CORE_DIV_SHIFT);
}
#define I2C_CLK_REG_MASK(bus) \
			(I2C_DIV_CON_MASK << \
			CLK_I2C ##bus## _DIV_CON_SHIFT | \
			CLK_I2C_PLL_SEL_MASK << \
			CLK_I2C ##bus## _PLL_SEL_SHIFT)

#define I2C_CLK_REG_VALUE(bus, clk_div) \
			      ((clk_div - 1) << \
					CLK_I2C ##bus## _DIV_CON_SHIFT | \
			      CLK_I2C_PLL_SEL_GPLL << \
					CLK_I2C ##bus## _PLL_SEL_SHIFT)

#define I2C_CLK_DIV_VALUE(con, bus) \
			(con >> CLK_I2C ##bus## _DIV_CON_SHIFT) & \
				I2C_DIV_CON_MASK;

#define I2C_PMUCLK_REG_MASK(bus) \
			(I2C_DIV_CON_MASK << \
			 CLK_I2C ##bus## _DIV_CON_SHIFT)

#define I2C_PMUCLK_REG_VALUE(bus, clk_div) \
				((clk_div - 1) << \
				CLK_I2C ##bus## _DIV_CON_SHIFT)

static ulong rk3399_i2c_get_clk(struct rk3399_cru *cru, ulong clk_id)
{
	u32 div, con;

	switch (clk_id) {
	case SCLK_I2C1:
		con = readl(&cru->clksel_con[61]);
		div = I2C_CLK_DIV_VALUE(con, 1);
		break;
	case SCLK_I2C2:
		con = readl(&cru->clksel_con[62]);
		div = I2C_CLK_DIV_VALUE(con, 2);
		break;
	case SCLK_I2C3:
		con = readl(&cru->clksel_con[63]);
		div = I2C_CLK_DIV_VALUE(con, 3);
		break;
	case SCLK_I2C5:
		con = readl(&cru->clksel_con[61]);
		div = I2C_CLK_DIV_VALUE(con, 5);
		break;
	case SCLK_I2C6:
		con = readl(&cru->clksel_con[62]);
		div = I2C_CLK_DIV_VALUE(con, 6);
		break;
	case SCLK_I2C7:
		con = readl(&cru->clksel_con[63]);
		div = I2C_CLK_DIV_VALUE(con, 7);
		break;
	default:
		printf("do not support this i2c bus\n");
		return -EINVAL;
	}

	return DIV_TO_RATE(GPLL_HZ, div);
}

static ulong rk3399_i2c_set_clk(struct rk3399_cru *cru, ulong clk_id, uint hz)
{
	int src_clk_div;

	/* i2c0,4,8 src clock from ppll, i2c1,2,3,5,6,7 src clock from gpll*/
	src_clk_div = GPLL_HZ / hz;
	assert(src_clk_div - 1 <= 127);

	switch (clk_id) {
	case SCLK_I2C1:
		rk_clrsetreg(&cru->clksel_con[61], I2C_CLK_REG_MASK(1),
			     I2C_CLK_REG_VALUE(1, src_clk_div));
		break;
	case SCLK_I2C2:
		rk_clrsetreg(&cru->clksel_con[62], I2C_CLK_REG_MASK(2),
			     I2C_CLK_REG_VALUE(2, src_clk_div));
		break;
	case SCLK_I2C3:
		rk_clrsetreg(&cru->clksel_con[63], I2C_CLK_REG_MASK(3),
			     I2C_CLK_REG_VALUE(3, src_clk_div));
		break;
	case SCLK_I2C5:
		rk_clrsetreg(&cru->clksel_con[61], I2C_CLK_REG_MASK(5),
			     I2C_CLK_REG_VALUE(5, src_clk_div));
		break;
	case SCLK_I2C6:
		rk_clrsetreg(&cru->clksel_con[62], I2C_CLK_REG_MASK(6),
			     I2C_CLK_REG_VALUE(6, src_clk_div));
		break;
	case SCLK_I2C7:
		rk_clrsetreg(&cru->clksel_con[63], I2C_CLK_REG_MASK(7),
			     I2C_CLK_REG_VALUE(7, src_clk_div));
		break;
	default:
		printf("do not support this i2c bus\n");
		return -EINVAL;
	}

	return rk3399_i2c_get_clk(cru, clk_id);
}

/*
 * RK3399 SPI clocks have a common divider-width (7 bits) and a single bit
 * to select either CPLL or GPLL as the clock-parent. The location within
 * the enclosing CLKSEL_CON (i.e. div_shift and sel_shift) are variable.
 */

struct spi_clkreg {
	uint8_t reg;  /* CLKSEL_CON[reg] register in CRU */
	uint8_t div_shift;
	uint8_t sel_shift;
};

/*
 * The entries are numbered relative to their offset from SCLK_SPI0.
 *
 * Note that SCLK_SPI3 (which is configured via PMUCRU and requires different
 * logic is not supported).
 */
static const struct spi_clkreg spi_clkregs[] = {
	[0] = { .reg = 59,
		.div_shift = CLK_SPI0_PLL_DIV_CON_SHIFT,
		.sel_shift = CLK_SPI0_PLL_SEL_SHIFT, },
	[1] = { .reg = 59,
		.div_shift = CLK_SPI1_PLL_DIV_CON_SHIFT,
		.sel_shift = CLK_SPI1_PLL_SEL_SHIFT, },
	[2] = { .reg = 60,
		.div_shift = CLK_SPI2_PLL_DIV_CON_SHIFT,
		.sel_shift = CLK_SPI2_PLL_SEL_SHIFT, },
	[3] = { .reg = 60,
		.div_shift = CLK_SPI4_PLL_DIV_CON_SHIFT,
		.sel_shift = CLK_SPI4_PLL_SEL_SHIFT, },
	[4] = { .reg = 58,
		.div_shift = CLK_SPI5_PLL_DIV_CON_SHIFT,
		.sel_shift = CLK_SPI5_PLL_SEL_SHIFT, },
};

static ulong rk3399_spi_get_clk(struct rk3399_cru *cru, ulong clk_id)
{
	const struct spi_clkreg *spiclk = NULL;
	u32 div, val;

	switch (clk_id) {
	case SCLK_SPI0 ... SCLK_SPI5:
		spiclk = &spi_clkregs[clk_id - SCLK_SPI0];
		break;

	default:
		pr_err("%s: SPI clk-id %ld not supported\n", __func__, clk_id);
		return -EINVAL;
	}

	val = readl(&cru->clksel_con[spiclk->reg]);
	div = bitfield_extract(val, spiclk->div_shift,
			       CLK_SPI_PLL_DIV_CON_WIDTH);

	return DIV_TO_RATE(GPLL_HZ, div);
}

static ulong rk3399_spi_set_clk(struct rk3399_cru *cru, ulong clk_id, uint hz)
{
	const struct spi_clkreg *spiclk = NULL;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(GPLL_HZ, hz) - 1;
	assert(src_clk_div < 128);

	switch (clk_id) {
	case SCLK_SPI1 ... SCLK_SPI5:
		spiclk = &spi_clkregs[clk_id - SCLK_SPI0];
		break;

	default:
		pr_err("%s: SPI clk-id %ld not supported\n", __func__, clk_id);
		return -EINVAL;
	}

	rk_clrsetreg(&cru->clksel_con[spiclk->reg],
		     ((CLK_SPI_PLL_DIV_CON_MASK << spiclk->div_shift) |
		       (CLK_SPI_PLL_SEL_GPLL << spiclk->sel_shift)),
		     ((src_clk_div << spiclk->div_shift) |
		      (CLK_SPI_PLL_SEL_GPLL << spiclk->sel_shift)));

	return rk3399_spi_get_clk(cru, clk_id);
}

#define RK3399_LIMIT_PLL_ACLK_VOP	(400 * 1000000)

static ulong rk3399_vop_set_clk(struct rk3399_cru *cru, ulong clk_id, u32 hz)
{
	struct pll_div vpll_config = {0}, cpll_config = {0};
	int aclk_vop = RK3399_LIMIT_PLL_ACLK_VOP;
	void *aclkreg_addr, *dclkreg_addr;
	u32 div = 1;

	switch (clk_id) {
	case DCLK_VOP0:
		aclkreg_addr = &cru->clksel_con[47];
		dclkreg_addr = &cru->clksel_con[49];
		break;
	case DCLK_VOP1:
		aclkreg_addr = &cru->clksel_con[48];
		dclkreg_addr = &cru->clksel_con[50];
		break;
	default:
		return -EINVAL;
	}
	/* vop aclk source clk: cpll */
	div = GPLL_HZ / aclk_vop;
	assert(div - 1 <= 31);

	rk_clrsetreg(aclkreg_addr,
		     ACLK_VOP_PLL_SEL_MASK | ACLK_VOP_DIV_CON_MASK,
		     ACLK_VOP_PLL_SEL_GPLL << ACLK_VOP_PLL_SEL_SHIFT |
		     (div - 1) << ACLK_VOP_DIV_CON_SHIFT);

	if (readl(dclkreg_addr) & DCLK_VOP_PLL_SEL_MASK) {
		if (pll_para_config(hz, &cpll_config))
			return -1;
		rkclk_set_pll(&cru->cpll_con[0], &cpll_config);
	} else {
		if (pll_para_config(hz, &vpll_config))
			return -1;
		rkclk_set_pll(&cru->vpll_con[0], &vpll_config);
	}

	rk_clrsetreg(dclkreg_addr,
		     DCLK_VOP_DCLK_SEL_MASK | DCLK_VOP_DIV_CON_MASK,
		     DCLK_VOP_DCLK_SEL_DIVOUT << DCLK_VOP_DCLK_SEL_SHIFT |
		     (1 - 1) << DCLK_VOP_DIV_CON_SHIFT);

	return hz;
}

static ulong rk3399_mmc_get_clk(struct rk3399_cru *cru, uint clk_id)
{
	u32 div, con;

	switch (clk_id) {
	case HCLK_SDMMC:
	case SCLK_SDMMC:
		con = readl(&cru->clksel_con[16]);
		/* dwmmc controller have internal div 2 */
		div = 2;
		break;
	case SCLK_EMMC:
		con = readl(&cru->clksel_con[22]);
		div = 1;
		break;
	default:
		return -EINVAL;
	}

	div *= (con & CLK_EMMC_DIV_CON_MASK) >> CLK_EMMC_DIV_CON_SHIFT;
	if ((con & CLK_EMMC_PLL_MASK) >> CLK_EMMC_PLL_SHIFT
			== CLK_EMMC_PLL_SEL_24M)
		return DIV_TO_RATE(OSC_HZ, div);
	else
		return DIV_TO_RATE(GPLL_HZ, div);
}

static ulong rk3399_mmc_set_clk(struct rk3399_cru *cru,
				ulong clk_id, ulong set_rate)
{
	int src_clk_div;
	int aclk_emmc = 198*MHz;

	switch (clk_id) {
	case HCLK_SDMMC:
	case SCLK_SDMMC:
		/* Select clk_sdmmc source from GPLL by default */
		/* mmc clock defaulg div 2 internal, provide double in cru */
		src_clk_div = DIV_ROUND_UP(GPLL_HZ / 2, set_rate);

		if (src_clk_div > 128) {
			/* use 24MHz source for 400KHz clock */
			src_clk_div = DIV_ROUND_UP(OSC_HZ / 2, set_rate);
			assert(src_clk_div - 1 < 128);
			rk_clrsetreg(&cru->clksel_con[16],
				     CLK_EMMC_PLL_MASK | CLK_EMMC_DIV_CON_MASK,
				     CLK_EMMC_PLL_SEL_24M << CLK_EMMC_PLL_SHIFT |
				     (src_clk_div - 1) << CLK_EMMC_DIV_CON_SHIFT);
		} else {
			rk_clrsetreg(&cru->clksel_con[16],
				     CLK_EMMC_PLL_MASK | CLK_EMMC_DIV_CON_MASK,
				     CLK_EMMC_PLL_SEL_GPLL << CLK_EMMC_PLL_SHIFT |
				     (src_clk_div - 1) << CLK_EMMC_DIV_CON_SHIFT);
		}
		break;
	case SCLK_EMMC:
		/* Select aclk_emmc source from GPLL */
		src_clk_div = DIV_ROUND_UP(GPLL_HZ , aclk_emmc);
		assert(src_clk_div - 1 < 32);

		rk_clrsetreg(&cru->clksel_con[21],
			     ACLK_EMMC_PLL_SEL_MASK | ACLK_EMMC_DIV_CON_MASK,
			     ACLK_EMMC_PLL_SEL_GPLL << ACLK_EMMC_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_EMMC_DIV_CON_SHIFT);

		/* Select clk_emmc source from GPLL too */
		src_clk_div = DIV_ROUND_UP(GPLL_HZ, set_rate);
		if (src_clk_div > 128) {
			/* use 24MHz source for 400KHz clock */
			src_clk_div = DIV_ROUND_UP(OSC_HZ, set_rate);
			assert(src_clk_div - 1 < 128);
			rk_clrsetreg(&cru->clksel_con[22],
				     CLK_EMMC_PLL_MASK | CLK_EMMC_DIV_CON_MASK,
				     CLK_EMMC_PLL_SEL_24M << CLK_EMMC_PLL_SHIFT |
				     (src_clk_div - 1) << CLK_EMMC_DIV_CON_SHIFT);
		} else {
			rk_clrsetreg(&cru->clksel_con[22],
				     CLK_EMMC_PLL_MASK | CLK_EMMC_DIV_CON_MASK,
				     CLK_EMMC_PLL_SEL_GPLL << CLK_EMMC_PLL_SHIFT |
				     (src_clk_div - 1) << CLK_EMMC_DIV_CON_SHIFT);
		}
		break;
	default:
		return -EINVAL;
	}
	return rk3399_mmc_get_clk(cru, clk_id);
}

static ulong rk3399_gmac_set_clk(struct rk3399_cru *cru, ulong rate)
{
	ulong ret;

	/*
	 * The RGMII CLK can be derived either from an external "clkin"
	 * or can be generated from internally by a divider from SCLK_MAC.
	 */
	if (readl(&cru->clksel_con[19]) & BIT(4)) {
		/* An external clock will always generate the right rate... */
		ret = rate;
	} else {
		/*
		 * No platform uses an internal clock to date.
		 * Implement this once it becomes necessary and print an error
		 * if someone tries to use it (while it remains unimplemented).
		 */
		pr_err("%s: internal clock is UNIMPLEMENTED\n", __func__);
		ret = 0;
	}

	return ret;
}

#define PMUSGRF_DDR_RGN_CON16 0xff330040
static ulong rk3399_ddr_set_clk(struct rk3399_cru *cru,
				ulong set_rate)
{
	struct pll_div dpll_cfg;

	/*  IC ECO bug, need to set this register */
	writel(0xc000c000, PMUSGRF_DDR_RGN_CON16);

	/*  clk_ddrc == DPLL = 24MHz / refdiv * fbdiv / postdiv1 / postdiv2 */
	switch (set_rate) {
	case 50 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 1, .fbdiv = 12, .postdiv1 = 3, .postdiv2 = 2};
		break;
	case 200 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 1, .fbdiv = 50, .postdiv1 = 6, .postdiv2 = 1};
		break;
	case 300 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 2, .fbdiv = 100, .postdiv1 = 4, .postdiv2 = 1};
		break;
	case 400 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 1, .fbdiv = 50, .postdiv1 = 3, .postdiv2 = 1};
		break;
	case 666 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 2, .fbdiv = 111, .postdiv1 = 2, .postdiv2 = 1};
		break;
	case 800 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 1, .fbdiv = 100, .postdiv1 = 3, .postdiv2 = 1};
		break;
	case 933 * MHz:
		dpll_cfg = (struct pll_div)
		{.refdiv = 1, .fbdiv = 116, .postdiv1 = 3, .postdiv2 = 1};
		break;
	default:
		pr_err("Unsupported SDRAM frequency!,%ld\n", set_rate);
	}
	rkclk_set_pll(&cru->dpll_con[0], &dpll_cfg);

	return set_rate;
}

static ulong rk3399_saradc_get_clk(struct rk3399_cru *cru)
{
	u32 div, val;

	val = readl(&cru->clksel_con[26]);
	div = bitfield_extract(val, CLK_SARADC_DIV_CON_SHIFT,
			       CLK_SARADC_DIV_CON_WIDTH);

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3399_saradc_set_clk(struct rk3399_cru *cru, uint hz)
{
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(OSC_HZ, hz) - 1;
	assert(src_clk_div <= 255);

	rk_clrsetreg(&cru->clksel_con[26],
		     CLK_SARADC_DIV_CON_MASK,
		     src_clk_div << CLK_SARADC_DIV_CON_SHIFT);

	return rk3399_saradc_get_clk(cru);
}

static ulong rk3399_tsadc_get_clk(struct rk3399_cru *cru)
{
	u32 div, val;

	val = readl(&cru->clksel_con[27]);
	div = bitfield_extract(val, CLK_TSADC_SEL_SHIFT,
			       10);

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3399_tsadc_set_clk(struct rk3399_cru *cru, uint hz)
{
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(OSC_HZ, hz) - 1;
	assert(src_clk_div <= 255);

	rk_clrsetreg(&cru->clksel_con[27],
		     CLK_TSADC_DIV_CON_MASK | CLK_TSADC_SEL_MASK,
		     (CLK_TSADC_SEL_X24M << CLK_TSADC_SEL_SHIFT) |
		     (src_clk_div << CLK_TSADC_DIV_CON_SHIFT));

	return rk3399_tsadc_get_clk(cru);
}

static ulong rk3399_crypto_get_clk(struct rk3399_clk_priv *priv, ulong clk_id)
{
	struct rk3399_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case SCLK_CRYPTO0:
		con = readl(&cru->clksel_con[24]);
		div = (con & CRYPTO0_DIV_MASK) >> CRYPTO0_DIV_SHIFT;
		parent = GPLL_HZ;
		break;
	case SCLK_CRYPTO1:
		con = readl(&cru->clksel_con[26]);
		div = (con & CRYPTO1_DIV_MASK) >> CRYPTO1_DIV_SHIFT;
		parent = GPLL_HZ;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3399_crypto_set_clk(struct rk3399_clk_priv *priv, ulong clk_id,
				   ulong hz)
{
	struct rk3399_cru *cru = priv->cru;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(GPLL_HZ, hz);
	assert(src_clk_div - 1 <= 31);

	/*
	 * select gpll as crypto clock source and
	 * set up dependent divisors for crypto clocks.
	 */
	switch (clk_id) {
	case SCLK_CRYPTO0:
		rk_clrsetreg(&cru->clksel_con[24],
			     CRYPTO0_PLL_SEL_MASK | CRYPTO0_DIV_MASK,
			     CRYPTO_PLL_SEL_GPLL << CRYPTO0_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << CRYPTO0_DIV_SHIFT);
		break;
	case SCLK_CRYPTO1:
		rk_clrsetreg(&cru->clksel_con[26],
			     CRYPTO1_PLL_SEL_MASK | CRYPTO1_DIV_MASK,
			     CRYPTO_PLL_SEL_GPLL << CRYPTO1_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << CRYPTO1_DIV_SHIFT);
		break;
	default:
		printf("do not support this peri freq\n");
		return -EINVAL;
	}

	return rk3399_crypto_get_clk(priv, clk_id);
}

#ifndef CONFIG_SPL_BUILD
static ulong rk3399_peri_get_clk(struct rk3399_clk_priv *priv, ulong clk_id)
{
	struct rk3399_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_PERIHP:
		con = readl(&cru->clksel_con[14]);
		div = (con & ACLK_PERIHP_DIV_CON_MASK) >>
		      ACLK_PERIHP_DIV_CON_SHIFT;
		parent = GPLL_HZ;
		break;
	case PCLK_PERIHP:
		con = readl(&cru->clksel_con[14]);
		div = (con & PCLK_PERIHP_DIV_CON_MASK) >>
		      PCLK_PERIHP_DIV_CON_SHIFT;
		parent = rk3399_peri_get_clk(priv, ACLK_PERIHP);
		break;
	case HCLK_PERIHP:
		con = readl(&cru->clksel_con[14]);
		div = (con & HCLK_PERIHP_DIV_CON_MASK) >>
		      HCLK_PERIHP_DIV_CON_SHIFT;
		parent = rk3399_peri_get_clk(priv, ACLK_PERIHP);
		break;
	case ACLK_PERILP0:
		con = readl(&cru->clksel_con[23]);
		div = (con & ACLK_PERILP0_DIV_CON_MASK) >>
		      ACLK_PERILP0_DIV_CON_SHIFT;
		parent = GPLL_HZ;
		break;
	case HCLK_PERILP0:
		con = readl(&cru->clksel_con[23]);
		div = (con & HCLK_PERILP0_DIV_CON_MASK) >>
		      HCLK_PERILP0_DIV_CON_SHIFT;
		parent = rk3399_peri_get_clk(priv, ACLK_PERILP0);
		break;
	case PCLK_PERILP0:
		con = readl(&cru->clksel_con[23]);
		div = (con & PCLK_PERILP0_DIV_CON_MASK) >>
		      PCLK_PERILP0_DIV_CON_SHIFT;
		parent = rk3399_peri_get_clk(priv, ACLK_PERILP0);
		break;
	case HCLK_PERILP1:
		con = readl(&cru->clksel_con[25]);
		div = (con & HCLK_PERILP1_DIV_CON_MASK) >>
		      HCLK_PERILP1_DIV_CON_SHIFT;
		parent = GPLL_HZ;
		break;
	case PCLK_PERILP1:
		con = readl(&cru->clksel_con[25]);
		div = (con & PCLK_PERILP1_DIV_CON_MASK) >>
		      PCLK_PERILP1_DIV_CON_SHIFT;
		parent = rk3399_peri_get_clk(priv, HCLK_PERILP1);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3399_alive_get_clk(struct rk3399_clk_priv *priv)
{
	struct rk3399_cru *cru = priv->cru;
	u32 div, con, parent;

	con = readl(&cru->clksel_con[57]);
	div = (con & PCLK_ALIVE_DIV_CON_MASK) >>
	      PCLK_ALIVE_DIV_CON_SHIFT;
	parent = GPLL_HZ;
	return DIV_TO_RATE(parent, div);
}
#endif

static ulong rk3399_clk_get_rate(struct clk *clk)
{
	struct rk3399_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate = 0;

	switch (clk->id) {
	case PLL_APLLL:
	case PLL_APLLB:
	case PLL_DPLL:
	case PLL_CPLL:
	case PLL_GPLL:
	case PLL_NPLL:
	case PLL_VPLL:
		rate = rk3399_pll_get_rate(priv, clk->id);
		break;
	case HCLK_SDMMC:
	case SCLK_SDMMC:
	case SCLK_EMMC:
		rate = rk3399_mmc_get_clk(priv->cru, clk->id);
		break;
	case SCLK_I2C1:
	case SCLK_I2C2:
	case SCLK_I2C3:
	case SCLK_I2C5:
	case SCLK_I2C6:
	case SCLK_I2C7:
		rate = rk3399_i2c_get_clk(priv->cru, clk->id);
		break;
	case SCLK_SPI0...SCLK_SPI5:
		rate = rk3399_spi_get_clk(priv->cru, clk->id);
		break;
	case SCLK_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
	case SCLK_UART3:
		return 24000000;
		break;
	case PCLK_HDMI_CTRL:
		break;
	case DCLK_VOP0:
	case DCLK_VOP1:
		break;
	case PCLK_EFUSE1024NS:
		break;
	case SCLK_SARADC:
		rate = rk3399_saradc_get_clk(priv->cru);
		break;
	case SCLK_TSADC:
		rate = rk3399_tsadc_get_clk(priv->cru);
		break;
	case SCLK_CRYPTO0:
	case SCLK_CRYPTO1:
		rate = rk3399_crypto_get_clk(priv, clk->id);
		break;
#ifndef CONFIG_SPL_BUILD
	case ACLK_PERIHP:
	case HCLK_PERIHP:
	case PCLK_PERIHP:
	case ACLK_PERILP0:
	case HCLK_PERILP0:
	case PCLK_PERILP0:
	case HCLK_PERILP1:
	case PCLK_PERILP1:
		rate = rk3399_peri_get_clk(priv, clk->id);
		break;
	case PCLK_ALIVE:
	case PCLK_WDT:
		rate = rk3399_alive_get_clk(priv);
		break;
#endif
	default:
		return -ENOENT;
	}

	return rate;
}

static ulong rk3399_clk_set_rate(struct clk *clk, ulong rate)
{
	struct rk3399_clk_priv *priv = dev_get_priv(clk->dev);
	ulong ret = 0;

	switch (clk->id) {
	case 0 ... 63:
		return 0;

	case ACLK_PERIHP:
	case HCLK_PERIHP:
	case PCLK_PERIHP:
		return 0;

	case ACLK_PERILP0:
	case HCLK_PERILP0:
	case PCLK_PERILP0:
		return 0;

	case ACLK_CCI:
		return 0;

	case HCLK_PERILP1:
	case PCLK_PERILP1:
		return 0;

	case HCLK_SDMMC:
	case SCLK_SDMMC:
	case SCLK_EMMC:
		ret = rk3399_mmc_set_clk(priv->cru, clk->id, rate);
		break;
	case SCLK_MAC:
		ret = rk3399_gmac_set_clk(priv->cru, rate);
		break;
	case SCLK_I2C1:
	case SCLK_I2C2:
	case SCLK_I2C3:
	case SCLK_I2C5:
	case SCLK_I2C6:
	case SCLK_I2C7:
		ret = rk3399_i2c_set_clk(priv->cru, clk->id, rate);
		break;
	case SCLK_SPI0...SCLK_SPI5:
		ret = rk3399_spi_set_clk(priv->cru, clk->id, rate);
		break;
	case PCLK_HDMI_CTRL:
	case PCLK_VIO_GRF:
		/* the PCLK gates for video are enabled by default */
		break;
	case DCLK_VOP0:
	case DCLK_VOP1:
		ret = rk3399_vop_set_clk(priv->cru, clk->id, rate);
		break;
	case SCLK_DDRCLK:
		ret = rk3399_ddr_set_clk(priv->cru, rate);
		break;
	case PCLK_EFUSE1024NS:
		break;
	case SCLK_SARADC:
		ret = rk3399_saradc_set_clk(priv->cru, rate);
		break;
	case SCLK_TSADC:
		ret = rk3399_tsadc_set_clk(priv->cru, rate);
		break;
	case SCLK_CRYPTO0:
	case SCLK_CRYPTO1:
		ret = rk3399_crypto_set_clk(priv, clk->id, rate);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static int __maybe_unused rk3399_gmac_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk3399_clk_priv *priv = dev_get_priv(clk->dev);
	const char *clock_output_name;
	int ret;

	/*
	 * If the requested parent is in the same clock-controller and
	 * the id is SCLK_MAC ("clk_gmac"), switch to the internal clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC)) {
		debug("%s: switching RGMII to SCLK_MAC\n", __func__);
		rk_clrreg(&priv->cru->clksel_con[19], BIT(4));
		return 0;
	}

	/*
	 * Otherwise, we need to check the clock-output-names of the
	 * requested parent to see if the requested id is "clkin_gmac".
	 */
	ret = dev_read_string_index(parent->dev, "clock-output-names",
				    parent->id, &clock_output_name);
	if (ret < 0)
		return -ENODATA;

	/* If this is "clkin_gmac", switch to the external clock input */
	if (!strcmp(clock_output_name, "clkin_gmac")) {
		debug("%s: switching RGMII to CLKIN\n", __func__);
		rk_setreg(&priv->cru->clksel_con[19], BIT(4));
		return 0;
	}

	return -EINVAL;
}

static int __maybe_unused rk3399_dclk_vop_set_parent(struct clk *clk,
						     struct clk *parent)
{
	struct rk3399_clk_priv *priv = dev_get_priv(clk->dev);
	void *dclkreg_addr;

	switch (clk->id) {
	case DCLK_VOP0_DIV:
		dclkreg_addr = &priv->cru->clksel_con[49];
		break;
	case DCLK_VOP1_DIV:
		dclkreg_addr = &priv->cru->clksel_con[50];
		break;
	default:
		return -EINVAL;
	}
	if (parent->id == PLL_CPLL) {
		rk_clrsetreg(dclkreg_addr, DCLK_VOP_PLL_SEL_MASK,
			     DCLK_VOP_PLL_SEL_CPLL << DCLK_VOP_PLL_SEL_SHIFT);
	} else {
		rk_clrsetreg(dclkreg_addr, DCLK_VOP_PLL_SEL_MASK,
			     DCLK_VOP_PLL_SEL_VPLL << DCLK_VOP_PLL_SEL_SHIFT);
	}

	return 0;
}

static int __maybe_unused rk3399_clk_set_parent(struct clk *clk, struct clk *parent)
{
	switch (clk->id) {
	case SCLK_RMII_SRC:
		return rk3399_gmac_set_parent(clk, parent);
	case DCLK_VOP0_DIV:
	case DCLK_VOP1_DIV:
		return rk3399_dclk_vop_set_parent(clk, parent);
	}

	debug("%s: unsupported clk %ld\n", __func__, clk->id);
	return -ENOENT;
}

static int rk3399_clk_enable(struct clk *clk)
{
	switch (clk->id) {
	case HCLK_HOST0:
	case HCLK_HOST0_ARB:
	case SCLK_USBPHY0_480M_SRC:
	case HCLK_HOST1:
	case HCLK_HOST1_ARB:
	case SCLK_USBPHY1_480M_SRC:
	case ACLK_USB3OTG1:
	case ACLK_USB3_GRF:
	case SCLK_USB3OTG1_REF:
	case SCLK_USB3OTG1_SUSPEND:
		return 0;
	}

	debug("%s: unsupported clk %ld\n", __func__, clk->id);
	return -ENOENT;
}

static struct clk_ops rk3399_clk_ops = {
	.get_rate = rk3399_clk_get_rate,
	.set_rate = rk3399_clk_set_rate,
#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
	.set_parent = rk3399_clk_set_parent,
#endif
	.enable = rk3399_clk_enable,
};

static void rkclk_init(struct rk3399_cru *cru)
{
	u32 aclk_div;
	u32 hclk_div;
	u32 pclk_div;

	rk3399_configure_cpu(cru, APLL_816_MHZ, CPU_CLUSTER_LITTLE);
	rk3399_configure_cpu(cru, APLL_816_MHZ, CPU_CLUSTER_BIG);

	/*
	 * some cru registers changed by bootrom, we'd better reset them to
	 * reset/default values described in TRM to avoid confusion in kernel.
	 * Please consider these three lines as a fix of bootrom bug.
	 */
	if (rkclk_pll_get_rate(&cru->npll_con[0]) != NPLL_HZ)
		rkclk_set_pll(&cru->npll_con[0], &npll_init_cfg);

	if (rkclk_pll_get_rate(&cru->gpll_con[0]) == GPLL_HZ)
		return;

	rk_clrsetreg(&cru->clksel_con[12], 0xffff, 0x4101);
	rk_clrsetreg(&cru->clksel_con[19], 0xffff, 0x033f);
	rk_clrsetreg(&cru->clksel_con[56], 0x0003, 0x0003);

	/* configure perihp aclk, hclk, pclk */
	aclk_div = DIV_ROUND_UP(GPLL_HZ, PERIHP_ACLK_HZ) - 1;

	hclk_div = PERIHP_ACLK_HZ / PERIHP_HCLK_HZ - 1;
	assert((hclk_div + 1) * PERIHP_HCLK_HZ <=
	       PERIHP_ACLK_HZ && (hclk_div <= 0x3));

	pclk_div = PERIHP_ACLK_HZ / PERIHP_PCLK_HZ - 1;
	assert((pclk_div + 1) * PERIHP_PCLK_HZ <=
	       PERIHP_ACLK_HZ && (pclk_div <= 0x7));

	rk_clrsetreg(&cru->clksel_con[14],
		     PCLK_PERIHP_DIV_CON_MASK | HCLK_PERIHP_DIV_CON_MASK |
		     ACLK_PERIHP_PLL_SEL_MASK | ACLK_PERIHP_DIV_CON_MASK,
		     pclk_div << PCLK_PERIHP_DIV_CON_SHIFT |
		     hclk_div << HCLK_PERIHP_DIV_CON_SHIFT |
		     ACLK_PERIHP_PLL_SEL_GPLL << ACLK_PERIHP_PLL_SEL_SHIFT |
		     aclk_div << ACLK_PERIHP_DIV_CON_SHIFT);

	/* configure perilp0 aclk, hclk, pclk */
	aclk_div = DIV_ROUND_UP(GPLL_HZ, PERILP0_ACLK_HZ) - 1;

	hclk_div = PERILP0_ACLK_HZ / PERILP0_HCLK_HZ - 1;
	assert((hclk_div + 1) * PERILP0_HCLK_HZ <=
	       PERILP0_ACLK_HZ && (hclk_div <= 0x3));

	pclk_div = PERILP0_ACLK_HZ / PERILP0_PCLK_HZ - 1;
	assert((pclk_div + 1) * PERILP0_PCLK_HZ <=
	       PERILP0_ACLK_HZ && (pclk_div <= 0x7));

	rk_clrsetreg(&cru->clksel_con[23],
		     PCLK_PERILP0_DIV_CON_MASK | HCLK_PERILP0_DIV_CON_MASK |
		     ACLK_PERILP0_PLL_SEL_MASK | ACLK_PERILP0_DIV_CON_MASK,
		     pclk_div << PCLK_PERILP0_DIV_CON_SHIFT |
		     hclk_div << HCLK_PERILP0_DIV_CON_SHIFT |
		     ACLK_PERILP0_PLL_SEL_GPLL << ACLK_PERILP0_PLL_SEL_SHIFT |
		     aclk_div << ACLK_PERILP0_DIV_CON_SHIFT);

	/* perilp1 hclk select gpll as source */
	hclk_div = DIV_ROUND_UP(GPLL_HZ, PERILP1_HCLK_HZ) - 1;
	assert((hclk_div + 1) * PERILP1_HCLK_HZ <=
	       GPLL_HZ && (hclk_div <= 0x1f));

	pclk_div = PERILP1_HCLK_HZ / PERILP1_PCLK_HZ - 1;
	assert((pclk_div + 1) * PERILP1_PCLK_HZ <=
	       PERILP1_HCLK_HZ && (pclk_div <= 0x7));

	rk_clrsetreg(&cru->clksel_con[25],
		     PCLK_PERILP1_DIV_CON_MASK | HCLK_PERILP1_DIV_CON_MASK |
		     HCLK_PERILP1_PLL_SEL_MASK,
		     pclk_div << PCLK_PERILP1_DIV_CON_SHIFT |
		     hclk_div << HCLK_PERILP1_DIV_CON_SHIFT |
		     HCLK_PERILP1_PLL_SEL_GPLL << HCLK_PERILP1_PLL_SEL_SHIFT);

	rk_clrsetreg(&cru->clksel_con[21],
		     ACLK_EMMC_PLL_SEL_MASK | ACLK_EMMC_DIV_CON_MASK,
		     ACLK_EMMC_PLL_SEL_GPLL << ACLK_EMMC_PLL_SEL_SHIFT |
		     (4 - 1) << ACLK_EMMC_DIV_CON_SHIFT);
	rk_clrsetreg(&cru->clksel_con[22], 0x3f << 0, 7 << 0);

	/*
	 * I2c MUx is in cpll by default, but cpll is for dclk_vop exclusive.
	 * If dclk_vop set rate after i2c init, the CPLL changed,
	 * but the i2c not perception, it will resulting the wrong
	 * frequency of the i2c.
	 * So set the i2c frequency according to the kernel configuration,
	 * and Hang I2C on the GPLL.
	 */
	rk_clrsetreg(&cru->clksel_con[61], I2C_CLK_REG_MASK(1),
		     I2C_CLK_REG_VALUE(1, 4));
	rk_clrsetreg(&cru->clksel_con[62], I2C_CLK_REG_MASK(2),
		     I2C_CLK_REG_VALUE(2, 4));
	rk_clrsetreg(&cru->clksel_con[63], I2C_CLK_REG_MASK(3),
		     I2C_CLK_REG_VALUE(3, 4));
	rk_clrsetreg(&cru->clksel_con[61], I2C_CLK_REG_MASK(5),
		     I2C_CLK_REG_VALUE(5, 4));
	rk_clrsetreg(&cru->clksel_con[62], I2C_CLK_REG_MASK(6),
		     I2C_CLK_REG_VALUE(6, 4));
	rk_clrsetreg(&cru->clksel_con[63], I2C_CLK_REG_MASK(7),
		     I2C_CLK_REG_VALUE(7, 4));

	rkclk_set_pll(&cru->gpll_con[0], &gpll_init_cfg);
}

static int rk3399_clk_probe(struct udevice *dev)
{
	struct rk3399_clk_priv *priv = dev_get_priv(dev);

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct rk3399_clk_plat *plat = dev_get_platdata(dev);

	priv->cru = map_sysmem(plat->dtd.reg[0], plat->dtd.reg[1]);
#endif

	priv->sync_kernel = false;
	if (!priv->armlclk_enter_hz)
		priv->armlclk_enter_hz =
		rkclk_pll_get_rate(&priv->cru->apll_l_con[0]);
	if (!priv->armbclk_enter_hz)
		priv->armbclk_enter_hz =
		rkclk_pll_get_rate(&priv->cru->apll_b_con[0]);
	rkclk_init(priv->cru);
	if (!priv->armlclk_init_hz)
		priv->armlclk_init_hz =
		rkclk_pll_get_rate(&priv->cru->apll_l_con[0]);
	if (!priv->armbclk_init_hz)
		priv->armbclk_init_hz =
		rkclk_pll_get_rate(&priv->cru->apll_b_con[0]);

	return 0;
}

static int rk3399_clk_ofdata_to_platdata(struct udevice *dev)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	struct rk3399_clk_priv *priv = dev_get_priv(dev);

	priv->cru = dev_read_addr_ptr(dev);
#endif
	return 0;
}

static int rk3399_clk_bind(struct udevice *dev)
{
	int ret;
	struct udevice *sys_child, *sf_child;
	struct sysreset_reg *priv;
	struct softreset_reg *sf_priv;

	/* The reset driver does not have a device node, so bind it here */
	ret = device_bind_driver(dev, "rockchip_sysreset", "sysreset",
				 &sys_child);
	if (ret) {
		debug("Warning: No sysreset driver: ret=%d\n", ret);
	} else {
		priv = malloc(sizeof(struct sysreset_reg));
		priv->glb_srst_fst_value = offsetof(struct rk3399_cru,
						    glb_srst_fst_value);
		priv->glb_srst_snd_value = offsetof(struct rk3399_cru,
						    glb_srst_snd_value);
		sys_child->priv = priv;
	}

	ret = device_bind_driver_to_node(dev, "rockchip_reset", "reset",
					 dev_ofnode(dev), &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk3399_cru,
						    softrst_con[0]);
		sf_priv->sf_reset_num = 21;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk3399_clk_ids[] = {
	{ .compatible = "rockchip,rk3399-cru" },
	{ }
};

U_BOOT_DRIVER(clk_rk3399) = {
	.name		= "rockchip_rk3399_cru",
	.id		= UCLASS_CLK,
	.of_match	= rk3399_clk_ids,
	.priv_auto_alloc_size = sizeof(struct rk3399_clk_priv),
	.ofdata_to_platdata = rk3399_clk_ofdata_to_platdata,
	.ops		= &rk3399_clk_ops,
	.bind		= rk3399_clk_bind,
	.probe		= rk3399_clk_probe,
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	.platdata_auto_alloc_size = sizeof(struct rk3399_clk_plat),
#endif
};

static ulong rk3399_i2c_get_pmuclk(struct rk3399_pmucru *pmucru, ulong clk_id)
{
	u32 div, con;

	switch (clk_id) {
	case SCLK_I2C0_PMU:
		con = readl(&pmucru->pmucru_clksel[2]);
		div = I2C_CLK_DIV_VALUE(con, 0);
		break;
	case SCLK_I2C4_PMU:
		con = readl(&pmucru->pmucru_clksel[3]);
		div = I2C_CLK_DIV_VALUE(con, 4);
		break;
	case SCLK_I2C8_PMU:
		con = readl(&pmucru->pmucru_clksel[2]);
		div = I2C_CLK_DIV_VALUE(con, 8);
		break;
	default:
		printf("do not support this i2c bus\n");
		return -EINVAL;
	}

	return DIV_TO_RATE(PPLL_HZ, div);
}

static ulong rk3399_i2c_set_pmuclk(struct rk3399_pmucru *pmucru, ulong clk_id,
				   uint hz)
{
	int src_clk_div;

	src_clk_div = PPLL_HZ / hz;
	assert(src_clk_div - 1 < 127);

	switch (clk_id) {
	case SCLK_I2C0_PMU:
		rk_clrsetreg(&pmucru->pmucru_clksel[2], I2C_PMUCLK_REG_MASK(0),
			     I2C_PMUCLK_REG_VALUE(0, src_clk_div));
		break;
	case SCLK_I2C4_PMU:
		rk_clrsetreg(&pmucru->pmucru_clksel[3], I2C_PMUCLK_REG_MASK(4),
			     I2C_PMUCLK_REG_VALUE(4, src_clk_div));
		break;
	case SCLK_I2C8_PMU:
		rk_clrsetreg(&pmucru->pmucru_clksel[2], I2C_PMUCLK_REG_MASK(8),
			     I2C_PMUCLK_REG_VALUE(8, src_clk_div));
		break;
	default:
		printf("do not support this i2c bus\n");
		return -EINVAL;
	}

	return DIV_TO_RATE(PPLL_HZ, src_clk_div);
}

static ulong rk3399_pwm_get_clk(struct rk3399_pmucru *pmucru)
{
	u32 div, con;

	/* PWM closk rate is same as pclk_pmu */
	con = readl(&pmucru->pmucru_clksel[0]);
	div = con & PMU_PCLK_DIV_CON_MASK;

	return DIV_TO_RATE(PPLL_HZ, div);
}

static ulong rk3399_pmuclk_get_rate(struct clk *clk)
{
	struct rk3399_pmuclk_priv *priv = dev_get_priv(clk->dev);
	ulong rate = 0;

	switch (clk->id) {
	case PCLK_RKPWM_PMU:
	case PCLK_WDT_M0_PMU:
		rate = rk3399_pwm_get_clk(priv->pmucru);
		break;
	case SCLK_I2C0_PMU:
	case SCLK_I2C4_PMU:
	case SCLK_I2C8_PMU:
		rate = rk3399_i2c_get_pmuclk(priv->pmucru, clk->id);
		break;
	case SCLK_UART4_PMU:
		rate = 24000000;
		break;
	default:
		return -ENOENT;
	}

	return rate;
}

static ulong rk3399_pmuclk_set_rate(struct clk *clk, ulong rate)
{
	struct rk3399_pmuclk_priv *priv = dev_get_priv(clk->dev);
	ulong ret = 0;

	switch (clk->id) {
	case SCLK_I2C0_PMU:
	case SCLK_I2C4_PMU:
	case SCLK_I2C8_PMU:
		ret = rk3399_i2c_set_pmuclk(priv->pmucru, clk->id, rate);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static struct clk_ops rk3399_pmuclk_ops = {
	.get_rate = rk3399_pmuclk_get_rate,
	.set_rate = rk3399_pmuclk_set_rate,
};

#ifndef CONFIG_SPL_BUILD
static void pmuclk_init(struct rk3399_pmucru *pmucru)
{
	u32 pclk_div;

	/*  configure pmu pll(ppll) */
	rkclk_set_pll(&pmucru->ppll_con[0], &ppll_init_cfg);

	/*  configure pmu pclk */
	pclk_div = PPLL_HZ / PMU_PCLK_HZ - 1;
	rk_clrsetreg(&pmucru->pmucru_clksel[0],
		     PMU_PCLK_DIV_CON_MASK,
		     pclk_div << PMU_PCLK_DIV_CON_SHIFT);
}
#endif

static int rk3399_pmuclk_probe(struct udevice *dev)
{
#if CONFIG_IS_ENABLED(OF_PLATDATA) || !defined(CONFIG_SPL_BUILD)
	struct rk3399_pmuclk_priv *priv = dev_get_priv(dev);
#endif

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct rk3399_pmuclk_plat *plat = dev_get_platdata(dev);

	priv->pmucru = map_sysmem(plat->dtd.reg[0], plat->dtd.reg[1]);
#endif

#ifndef CONFIG_SPL_BUILD
	pmuclk_init(priv->pmucru);
#endif
	return 0;
}

static int rk3399_pmuclk_ofdata_to_platdata(struct udevice *dev)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	struct rk3399_pmuclk_priv *priv = dev_get_priv(dev);

	priv->pmucru = dev_read_addr_ptr(dev);
#endif
	return 0;
}

static int rk3399_pmuclk_bind(struct udevice *dev)
{
	int ret = 0;
	struct udevice *sf_child;
	struct softreset_reg *sf_priv;

	ret = device_bind_driver_to_node(dev, "rockchip_reset",
					 "reset", dev_ofnode(dev),
					 &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk3399_pmucru,
						    pmucru_softrst_con[0]);
		sf_priv->sf_reset_num = 2;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk3399_pmuclk_ids[] = {
	{ .compatible = "rockchip,rk3399-pmucru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3399_pmuclk) = {
	.name		= "rockchip_rk3399_pmucru",
	.id		= UCLASS_CLK,
	.of_match	= rk3399_pmuclk_ids,
	.priv_auto_alloc_size = sizeof(struct rk3399_pmuclk_priv),
	.ofdata_to_platdata = rk3399_pmuclk_ofdata_to_platdata,
	.ops		= &rk3399_pmuclk_ops,
	.probe		= rk3399_pmuclk_probe,
	.bind		= rk3399_pmuclk_bind,
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	.platdata_auto_alloc_size = sizeof(struct rk3399_pmuclk_plat),
#endif
};

#ifndef CONFIG_SPL_BUILD
/**
 * soc_clk_dump() - Print clock frequencies
 * Returns zero on success
 *
 * Implementation for the clk dump command.
 */
int soc_clk_dump(void)
{
	struct udevice *cru_dev, *pmucru_dev;
	struct rk3399_clk_priv *priv;
	const struct rk3399_clk_info *clk_dump;
	struct clk clk;
	unsigned long clk_count = ARRAY_SIZE(clks_dump);
	unsigned long rate;
	int i, ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(clk_rk3399),
					  &cru_dev);
	if (ret) {
		printf("%s failed to get cru device\n", __func__);
		return ret;
	}

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(rockchip_rk3399_pmuclk),
					  &pmucru_dev);
	if (ret) {
		printf("%s failed to get pmucru device\n", __func__);
		return ret;
	}

	priv = dev_get_priv(cru_dev);
	printf("CLK: (%s. arml: enter %lu KHz, init %lu KHz, kernel %lu%s)\n",
	       priv->sync_kernel ? "sync kernel" : "uboot",
	       priv->armlclk_enter_hz / 1000,
	       priv->armlclk_init_hz / 1000,
	       priv->set_armclk_rate ? priv->armlclk_hz / 1000 : 0,
	       priv->set_armclk_rate ? " KHz" : "N/A");
	printf("CLK: (%s. armb: enter %lu KHz, init %lu KHz, kernel %lu%s)\n",
	       priv->sync_kernel ? "sync kernel" : "uboot",
	       priv->armbclk_enter_hz / 1000,
	       priv->armbclk_init_hz / 1000,
	       priv->set_armclk_rate ? priv->armbclk_hz / 1000 : 0,
	       priv->set_armclk_rate ? " KHz" : "N/A");
	for (i = 0; i < clk_count; i++) {
		clk_dump = &clks_dump[i];
		if (clk_dump->name) {
			clk.id = clk_dump->id;
			if (clk_dump->is_cru)
				ret = clk_request(cru_dev, &clk);
			else
				ret = clk_request(pmucru_dev, &clk);
			if (ret < 0)
				return ret;

			rate = clk_get_rate(&clk);
			clk_free(&clk);
			if (i == 0) {
				if (rate < 0)
					printf("  %s %s\n", clk_dump->name,
					       "unknown");
				else
					printf("  %s %lu KHz\n", clk_dump->name,
					       rate / 1000);
			} else {
				if (rate < 0)
					printf("  %s %s\n", clk_dump->name,
					       "unknown");
				else
					printf("  %s %lu KHz\n", clk_dump->name,
					       rate / 1000);
			}
		}
	}

	return 0;
}
#endif
