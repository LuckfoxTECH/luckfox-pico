/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <common.h>
#include <bitfield.h>
#include <clk-uclass.h>
#include <dm.h>
#include <errno.h>
#include <syscon.h>
#include <asm/arch/clock.h>
#include <asm/arch/cru_rk3328.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk3328.h>
#include <asm/io.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rk3328-cru.h>

DECLARE_GLOBAL_DATA_PTR;

#define RATE_TO_DIV(input_rate, output_rate) \
	((input_rate) / (output_rate) - 1);
#define DIV_TO_RATE(input_rate, div)    ((input_rate) / ((div) + 1))

#ifndef CONFIG_SPL_BUILD
#define RK3328_CLK_DUMP(_id, _name, _iscru)	\
{						\
	.id = _id,				\
	.name = _name,				\
	.is_cru = _iscru,			\
}
#endif

static struct rockchip_pll_rate_table rk3328_pll_rates[] = {
	/* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac */
	RK3036_PLL_RATE(1200000000, 1, 50, 1, 1, 1, 0),
#ifndef CONFIG_SPL_BUILD
	RK3036_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
	RK3036_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
#endif
	RK3036_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
	RK3036_PLL_RATE(800000000, 1, 200, 6, 1, 1, 0),
	RK3036_PLL_RATE(600000000, 1, 75, 3, 1, 1, 0),
#ifndef CONFIG_SPL_BUILD
	RK3036_PLL_RATE(594000000, 1, 99, 4, 1, 1, 0),
	RK3036_PLL_RATE(500000000, 1, 125, 6, 1, 1, 0),
#endif
	{ /* sentinel */ },
};

static struct rockchip_pll_rate_table rk3328_pll_frac_rates[] = {
	/* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac */
#ifndef CONFIG_SPL_BUILD
	RK3036_PLL_RATE(1016064000, 3, 127, 1, 1, 0, 134217),
	/* vco = 1016064000 */
	RK3036_PLL_RATE(983040000, 24, 983, 1, 1, 0, 671088),
	/* vco = 983040000 */
#endif
	RK3036_PLL_RATE(491520000, 24, 983, 2, 1, 0, 671088),
	/* vco = 983040000 */
#ifndef CONFIG_SPL_BUILD
	RK3036_PLL_RATE(61440000, 6, 215, 7, 2, 0, 671088),
	/* vco = 860156000 */
	RK3036_PLL_RATE(56448000, 12, 451, 4, 4, 0, 9797894),
	/* vco = 903168000 */
	RK3036_PLL_RATE(40960000, 12, 409, 4, 5, 0, 10066329),
	/* vco = 819200000 */
#endif
	{ /* sentinel */ },
};

#define RK3328_CPUCLK_RATE(_rate, _aclk_div, _pclk_div)		\
{								\
	.rate	= _rate##U,					\
	.aclk_div = _aclk_div,					\
	.pclk_div = _pclk_div,					\
}

static struct rockchip_cpu_rate_table rk3328_cpu_rates[] = {
	RK3328_CPUCLK_RATE(1200000000, 1, 5),
	RK3328_CPUCLK_RATE(1008000000, 1, 5),
	RK3328_CPUCLK_RATE(816000000, 1, 3),
	RK3328_CPUCLK_RATE(600000000, 1, 3),
};

#ifndef CONFIG_SPL_BUILD
static const struct rk3328_clk_info clks_dump[] = {
	RK3328_CLK_DUMP(PLL_APLL, "apll", true),
	RK3328_CLK_DUMP(PLL_DPLL, "dpll", true),
	RK3328_CLK_DUMP(PLL_CPLL, "cpll", true),
	RK3328_CLK_DUMP(PLL_GPLL, "gpll", true),
	RK3328_CLK_DUMP(PLL_NPLL, "npll", true),
	RK3328_CLK_DUMP(ARMCLK, "armclk", true),
	RK3328_CLK_DUMP(ACLK_BUS_PRE, "aclk_bus", true),
	RK3328_CLK_DUMP(HCLK_BUS_PRE, "hclk_bus", true),
	RK3328_CLK_DUMP(PCLK_BUS_PRE, "pclk_bus", true),
	RK3328_CLK_DUMP(ACLK_PERI_PRE, "aclk_peri", true),
	RK3328_CLK_DUMP(HCLK_PERI, "hclk_peri", true),
	RK3328_CLK_DUMP(PCLK_PERI, "pclk_peri", true),
};
#endif

static struct rockchip_pll_clock rk3328_pll_clks[] = {
	[APLL] = PLL(pll_rk3328, PLL_APLL, RK3328_PLL_CON(0),
		     RK3328_MODE_CON, 0, 10, 0, rk3328_pll_frac_rates),
	[DPLL] = PLL(pll_rk3328, PLL_DPLL, RK3328_PLL_CON(8),
		     RK3328_MODE_CON, 4, 10, 0, NULL),
	[CPLL] = PLL(pll_rk3328, PLL_CPLL, RK3328_PLL_CON(16),
		    RK3328_MODE_CON, 8, 10, 0, rk3328_pll_rates),
	[GPLL] = PLL(pll_rk3328, PLL_GPLL, RK3328_PLL_CON(24),
		     RK3328_MODE_CON, 12, 10, 0, rk3328_pll_frac_rates),
	[NPLL] = PLL(pll_rk3328, PLL_NPLL, RK3328_PLL_CON(40),
		     RK3328_MODE_CON, 1, 10, 0, rk3328_pll_rates),
};

static ulong rk3328_armclk_set_clk(struct rk3328_clk_priv *priv, ulong hz)
{
	struct rk3328_cru *cru = priv->cru;
	const struct rockchip_cpu_rate_table *rate;
	ulong old_rate;

	rate = rockchip_get_cpu_settings(rk3328_cpu_rates, hz);
	if (!rate) {
		printf("%s unsupported rate\n", __func__);
		return -EINVAL;
	}

	/*
	 * select apll as cpu/core clock pll source and
	 * set up dependent divisors for PERI and ACLK clocks.
	 * core hz : apll = 1:1
	 */
	old_rate = rockchip_pll_get_rate(&rk3328_pll_clks[NPLL],
					 priv->cru, NPLL);
	if (old_rate > hz) {
		if (rockchip_pll_set_rate(&rk3328_pll_clks[NPLL],
					  priv->cru, NPLL, hz))
			return -EINVAL;
		rk_clrsetreg(&cru->clksel_con[0],
			     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
			     CORE_CLK_PLL_SEL_NPLL << CORE_CLK_PLL_SEL_SHIFT |
			     0 << CORE_DIV_CON_SHIFT);
		rk_clrsetreg(&cru->clksel_con[1],
			     CORE_ACLK_DIV_MASK | CORE_DBG_DIV_MASK,
			     rate->aclk_div << CORE_ACLK_DIV_SHIFT |
			     rate->pclk_div << CORE_DBG_DIV_SHIFT);
	} else if (old_rate < hz) {
		rk_clrsetreg(&cru->clksel_con[1],
			     CORE_ACLK_DIV_MASK | CORE_DBG_DIV_MASK,
			     rate->aclk_div << CORE_ACLK_DIV_SHIFT |
			     rate->pclk_div << CORE_DBG_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[0],
			     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
			     CORE_CLK_PLL_SEL_NPLL << CORE_CLK_PLL_SEL_SHIFT |
			     0 << CORE_DIV_CON_SHIFT);
		if (rockchip_pll_set_rate(&rk3328_pll_clks[NPLL],
					  priv->cru, NPLL, hz))
			return -EINVAL;
	}

	return rockchip_pll_get_rate(&rk3328_pll_clks[NPLL], priv->cru, NPLL);
}

#ifndef CONFIG_SPL_BUILD
static ulong rk3328_i2c_get_clk(struct rk3328_clk_priv *priv, ulong clk_id)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con;

	switch (clk_id) {
	case SCLK_I2C0:
		con = readl(&cru->clksel_con[34]);
		div = con >> CLK_I2C0_DIV_CON_SHIFT & CLK_I2C_DIV_CON_MASK;
		break;
	case SCLK_I2C1:
		con = readl(&cru->clksel_con[34]);
		div = con >> CLK_I2C1_DIV_CON_SHIFT & CLK_I2C_DIV_CON_MASK;
		break;
	case SCLK_I2C2:
		con = readl(&cru->clksel_con[35]);
		div = con >> CLK_I2C2_DIV_CON_SHIFT & CLK_I2C_DIV_CON_MASK;
		break;
	case SCLK_I2C3:
		con = readl(&cru->clksel_con[35]);
		div = con >> CLK_I2C3_DIV_CON_SHIFT & CLK_I2C_DIV_CON_MASK;
		break;
	default:
		printf("do not support this i2c bus\n");
		return -EINVAL;
	}

	return DIV_TO_RATE(priv->gpll_hz, div);
}

static ulong rk3328_i2c_set_clk(struct rk3328_clk_priv *priv,
				ulong clk_id, uint hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;

	src_clk_div = priv->gpll_hz / hz;
	assert(src_clk_div - 1 < 127);

	switch (clk_id) {
	case SCLK_I2C0:
		rk_clrsetreg(&cru->clksel_con[34],
			     CLK_I2C_DIV_CON_MASK << CLK_I2C0_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_MASK << CLK_I2C0_PLL_SEL_SHIFT,
			     (src_clk_div - 1) << CLK_I2C0_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_GPLL << CLK_I2C0_PLL_SEL_SHIFT);
		break;
	case SCLK_I2C1:
		rk_clrsetreg(&cru->clksel_con[34],
			     CLK_I2C_DIV_CON_MASK << CLK_I2C1_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_MASK << CLK_I2C1_PLL_SEL_SHIFT,
			     (src_clk_div - 1) << CLK_I2C1_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_GPLL << CLK_I2C1_PLL_SEL_SHIFT);
		break;
	case SCLK_I2C2:
		rk_clrsetreg(&cru->clksel_con[35],
			     CLK_I2C_DIV_CON_MASK << CLK_I2C2_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_MASK << CLK_I2C2_PLL_SEL_SHIFT,
			     (src_clk_div - 1) << CLK_I2C2_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_GPLL << CLK_I2C2_PLL_SEL_SHIFT);
		break;
	case SCLK_I2C3:
		rk_clrsetreg(&cru->clksel_con[35],
			     CLK_I2C_DIV_CON_MASK << CLK_I2C3_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_MASK << CLK_I2C3_PLL_SEL_SHIFT,
			     (src_clk_div - 1) << CLK_I2C3_DIV_CON_SHIFT |
			     CLK_I2C_PLL_SEL_GPLL << CLK_I2C3_PLL_SEL_SHIFT);
		break;
	default:
		printf("do not support this i2c bus\n");
		return -EINVAL;
	}

	return DIV_TO_RATE(priv->gpll_hz, src_clk_div);
}

static ulong rk3328_gmac2io_set_clk(struct rk3328_clk_priv *priv, ulong rate)
{
	struct rk3328_cru *cru = priv->cru;
	struct rk3328_grf_regs *grf;
	ulong ret;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	/*
	 * The RGMII CLK can be derived either from an external "clkin"
	 * or can be generated from internally by a divider from SCLK_MAC.
	 */
	if (readl(&grf->mac_con[1]) & BIT(10) &&
	    readl(&grf->soc_con[4]) & BIT(14)) {
		/* An external clock will always generate the right rate... */
		ret = rate;
	} else {
		u32 con = readl(&cru->clksel_con[27]);
		ulong pll_rate;
		u8 div;

		if ((con >> GMAC2IO_PLL_SEL_SHIFT) & GMAC2IO_PLL_SEL_GPLL)
			pll_rate = priv->gpll_hz;
		else
			pll_rate = priv->cpll_hz;

		div = DIV_ROUND_UP(pll_rate, rate) - 1;
		if (div <= 0x1f)
			rk_clrsetreg(&cru->clksel_con[27], GMAC2IO_CLK_DIV_MASK,
				     div << GMAC2IO_CLK_DIV_SHIFT);
		else
			debug("Unsupported div for gmac:%d\n", div);

		return DIV_TO_RATE(pll_rate, div);
	}

	return ret;
}

static ulong rk3328_gmac2phy_src_set_clk(struct rk3328_cru *cru, ulong rate)
{
	u32 con = readl(&cru->clksel_con[26]);
	ulong pll_rate;
	u8 div;

	if ((con >> GMAC2PHY_PLL_SEL_SHIFT) & GMAC2PHY_PLL_SEL_GPLL)
		pll_rate = GPLL_HZ;
	else
		pll_rate = CPLL_HZ;

	div = DIV_ROUND_UP(pll_rate, rate) - 1;
	if (div <= 0x1f)
		rk_clrsetreg(&cru->clksel_con[26], GMAC2PHY_CLK_DIV_MASK,
			     div << GMAC2PHY_CLK_DIV_SHIFT);
	else
		debug("Unsupported div for gmac:%d\n", div);

	return DIV_TO_RATE(pll_rate, div);
}

static ulong rk3328_gmac2phy_set_clk(struct rk3328_cru *cru, ulong rate)
{
	struct rk3328_grf_regs *grf;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (readl(&grf->mac_con[2]) & BIT(10))
		/* An external clock will always generate the right rate... */
		return rate;
	else
		return rk3328_gmac2phy_src_set_clk(cru, rate);
}
#endif

static ulong rk3328_mmc_get_clk(struct rk3328_clk_priv *priv, uint clk_id)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con, con_id;

	switch (clk_id) {
	case HCLK_SDMMC:
	case SCLK_SDMMC:
		con_id = 30;
		break;
	case HCLK_EMMC:
	case SCLK_EMMC:
	case SCLK_EMMC_SAMPLE:
		con_id = 32;
		break;
	default:
		return -EINVAL;
	}
	con = readl(&cru->clksel_con[con_id]);
	div = (con & CLK_EMMC_DIV_CON_MASK) >> CLK_EMMC_DIV_CON_SHIFT;

	if ((con & CLK_EMMC_PLL_MASK) >> CLK_EMMC_PLL_SHIFT
	    == CLK_EMMC_PLL_SEL_24M)
		return DIV_TO_RATE(OSC_HZ, div) / 2;
	else
		return DIV_TO_RATE(priv->gpll_hz, div) / 2;
}

static ulong rk3328_mmc_set_clk(struct rk3328_clk_priv *priv,
				ulong clk_id, ulong set_rate)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;
	u32 con_id;

	switch (clk_id) {
	case HCLK_SDMMC:
	case SCLK_SDMMC:
		con_id = 30;
		break;
	case HCLK_EMMC:
	case SCLK_EMMC:
		con_id = 32;
		break;
	default:
		return -EINVAL;
	}
	/* Select clk_sdmmc/emmc source from GPLL by default */
	/* mmc clock defaulg div 2 internal, need provide double in cru */
	src_clk_div = DIV_ROUND_UP(priv->gpll_hz / 2, set_rate);

	if (src_clk_div > 127) {
		/* use 24MHz source for 400KHz clock */
		src_clk_div = DIV_ROUND_UP(OSC_HZ / 2, set_rate);
		rk_clrsetreg(&cru->clksel_con[con_id],
			     CLK_EMMC_PLL_MASK | CLK_EMMC_DIV_CON_MASK,
			     CLK_EMMC_PLL_SEL_24M << CLK_EMMC_PLL_SHIFT |
			     (src_clk_div - 1) << CLK_EMMC_DIV_CON_SHIFT);
	} else {
		rk_clrsetreg(&cru->clksel_con[con_id],
			     CLK_EMMC_PLL_MASK | CLK_EMMC_DIV_CON_MASK,
			     CLK_EMMC_PLL_SEL_GPLL << CLK_EMMC_PLL_SHIFT |
			     (src_clk_div - 1) << CLK_EMMC_DIV_CON_SHIFT);
	}

	return rk3328_mmc_get_clk(priv, clk_id);
}

static ulong rk3328_spi_get_clk(struct rk3328_clk_priv *priv)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con, mux, p_rate;

	con = readl(&cru->clksel_con[24]);
	div = (con & CLK_SPI_DIV_CON_MASK) >> CLK_SPI_DIV_CON_SHIFT;
	mux = (con & CLK_SPI_PLL_SEL_MASK) >> CLK_SPI_PLL_SEL_SHIFT;
	if (mux)
		p_rate = priv->gpll_hz;
	else
		p_rate = priv->cpll_hz;

	return DIV_TO_RATE(p_rate, div);
}

static ulong rk3328_spi_set_clk(struct rk3328_clk_priv *priv, uint hz)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div = priv->gpll_hz / hz;

	rk_clrsetreg(&cru->clksel_con[24],
		     CLK_SPI_PLL_SEL_MASK | CLK_SPI_DIV_CON_MASK,
		     CLK_SPI_PLL_SEL_GPLL << CLK_SPI_PLL_SEL_SHIFT |
		     (div - 1) << CLK_SPI_DIV_CON_SHIFT);

	return DIV_TO_RATE(priv->gpll_hz, div);
}

#ifndef CONFIG_SPL_BUILD
static ulong rk3328_pwm_get_clk(struct rk3328_clk_priv *priv)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con;

	con = readl(&cru->clksel_con[24]);
	div = (con & CLK_PWM_DIV_CON_MASK) >> CLK_PWM_DIV_CON_SHIFT;

	return DIV_TO_RATE(priv->gpll_hz, div);
}

static ulong rk3328_pwm_set_clk(struct rk3328_clk_priv *priv, uint hz)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div = priv->gpll_hz / hz;

	rk_clrsetreg(&cru->clksel_con[24],
		     CLK_PWM_PLL_SEL_MASK | CLK_PWM_DIV_CON_MASK,
		     CLK_PWM_PLL_SEL_GPLL << CLK_PWM_PLL_SEL_SHIFT |
		     (div - 1) << CLK_PWM_DIV_CON_SHIFT);

	return DIV_TO_RATE(priv->gpll_hz, div);
}

static ulong rk3328_saradc_get_clk(struct rk3328_clk_priv *priv)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, val;

	val = readl(&cru->clksel_con[23]);
	div = bitfield_extract(val, CLK_SARADC_DIV_CON_SHIFT,
			       CLK_SARADC_DIV_CON_WIDTH);

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3328_saradc_set_clk(struct rk3328_clk_priv *priv, uint hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(OSC_HZ, hz) - 1;
	assert(src_clk_div < 128);

	rk_clrsetreg(&cru->clksel_con[23],
		     CLK_SARADC_DIV_CON_MASK,
		     src_clk_div << CLK_SARADC_DIV_CON_SHIFT);

	return rk3328_saradc_get_clk(priv);
}

static ulong rk3328_tsadc_get_clk(struct rk3328_clk_priv *priv)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, val;

	val = readl(&cru->clksel_con[22]);
	div = bitfield_extract(val, CLK_SARADC_DIV_CON_SHIFT,
			       CLK_SARADC_DIV_CON_WIDTH);

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3328_tsadc_set_clk(struct rk3328_clk_priv *priv, uint hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(OSC_HZ, hz) - 1;
	assert(src_clk_div < 128);

	rk_clrsetreg(&cru->clksel_con[22],
		     CLK_SARADC_DIV_CON_MASK,
		     src_clk_div << CLK_SARADC_DIV_CON_SHIFT);

	return rk3328_tsadc_get_clk(priv);
}

static ulong rk3328_vop_get_clk(struct rk3328_clk_priv *priv, ulong clk_id)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_VOP_PRE:
	case ACLK_VOP:
		con = readl(&cru->clksel_con[39]);
		div = (con & ACLK_VOP_DIV_CON_MASK) >> ACLK_VOP_DIV_CON_SHIFT;
		parent = priv->cpll_hz;
		break;
	case ACLK_VIO_PRE:
	case ACLK_VIO:
		con = readl(&cru->clksel_con[37]);
		div = (con & ACLK_VIO_DIV_CON_MASK) >> ACLK_VIO_DIV_CON_SHIFT;
		parent = priv->cpll_hz;
		break;
	case HCLK_VIO_PRE:
	case HCLK_VIO:
		parent = rk3328_vop_get_clk(priv, ACLK_VIO_PRE);
		con = readl(&cru->clksel_con[37]);
		div = (con & HCLK_VIO_DIV_CON_MASK) >> HCLK_VIO_DIV_CON_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3328_vop_set_clk(struct rk3328_clk_priv *priv,
				ulong clk_id, uint hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;
	u32 con, parent;

	src_clk_div = DIV_ROUND_UP(priv->cpll_hz, hz);
	assert(src_clk_div - 1 < 31);

	switch (clk_id) {
	case ACLK_VOP_PRE:
	case ACLK_VOP:
		rk_clrsetreg(&cru->clksel_con[39],
			     ACLK_VOP_PLL_SEL_MASK | ACLK_VOP_DIV_CON_MASK,
			     ACLK_VOP_PLL_SEL_CPLL << ACLK_VOP_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_VOP_DIV_CON_SHIFT);
		break;
	case ACLK_VIO_PRE:
	case ACLK_VIO:
		rk_clrsetreg(&cru->clksel_con[37],
			     ACLK_VIO_PLL_SEL_MASK | ACLK_VIO_DIV_CON_MASK,
			     ACLK_VIO_PLL_SEL_CPLL << ACLK_VIO_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_VIO_DIV_CON_SHIFT);
		break;
	case HCLK_VIO_PRE:
	case HCLK_VIO:
		src_clk_div = DIV_ROUND_UP(rk3328_vop_get_clk(priv,
							      ACLK_VIO_PRE),
					   hz);
		rk_clrsetreg(&cru->clksel_con[37],
			     HCLK_VIO_DIV_CON_MASK,
			     (src_clk_div - 1) << HCLK_VIO_DIV_CON_SHIFT);
		break;
	case DCLK_LCDC:
		con = readl(&cru->clksel_con[40]);
		con = (con & DCLK_LCDC_SEL_MASK) >> DCLK_LCDC_SEL_SHIFT;
		if (con) {
			parent = readl(&cru->clksel_con[40]);
			parent = (parent & DCLK_LCDC_PLL_SEL_MASK) >>
				 DCLK_LCDC_PLL_SEL_SHIFT;
			if (parent)
				src_clk_div = DIV_ROUND_UP(priv->cpll_hz, hz);
			else
				src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);

			rk_clrsetreg(&cru->clksel_con[40],
				     DCLK_LCDC_DIV_CON_MASK,
				     (src_clk_div - 1) <<
				     DCLK_LCDC_DIV_CON_SHIFT);
		}
		break;
	default:
		printf("do not support this vop freq\n");
		return -EINVAL;
	}

	return rk3328_vop_get_clk(priv, clk_id);
}
#endif

static ulong rk3328_bus_get_clk(struct rk3328_clk_priv *priv, ulong clk_id)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_BUS_PRE:
		con = readl(&cru->clksel_con[0]);
		div = (con & ACLK_BUS_DIV_CON_MASK) >> ACLK_BUS_DIV_CON_SHIFT;
		parent = priv->cpll_hz;
		break;
	case HCLK_BUS_PRE:
		con = readl(&cru->clksel_con[1]);
		div = (con & HCLK_BUS_DIV_CON_MASK) >> HCLK_BUS_DIV_CON_SHIFT;
		parent = rk3328_bus_get_clk(priv, ACLK_BUS_PRE);
		break;
	case PCLK_BUS_PRE:
		con = readl(&cru->clksel_con[1]);
		div = (con & PCLK_BUS_DIV_CON_MASK) >> PCLK_BUS_DIV_CON_SHIFT;
		parent = rk3328_bus_get_clk(priv, ACLK_BUS_PRE);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3328_bus_set_clk(struct rk3328_clk_priv *priv,
				ulong clk_id, ulong hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	switch (clk_id) {
	case ACLK_BUS_PRE:
		src_clk_div = DIV_ROUND_UP(priv->cpll_hz, hz);
		assert(src_clk_div - 1 < 31);
		if (src_clk_div > 32)
			src_clk_div = 32;
		rk_clrsetreg(&cru->clksel_con[0],
			     CLK_BUS_PLL_SEL_MASK | ACLK_BUS_DIV_CON_MASK,
			     CLK_BUS_PLL_SEL_CPLL << CLK_BUS_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_BUS_DIV_CON_SHIFT);
		break;
	case HCLK_BUS_PRE:
		src_clk_div = DIV_ROUND_UP(rk3328_bus_get_clk(priv,
							      ACLK_BUS_PRE),
					   hz);
		assert(src_clk_div - 1 < 3);
		rk_clrsetreg(&cru->clksel_con[1],
			     HCLK_BUS_DIV_CON_MASK,
			     (src_clk_div - 1) << HCLK_BUS_DIV_CON_SHIFT);
		break;
	case PCLK_BUS_PRE:
		src_clk_div = DIV_ROUND_UP(rk3328_bus_get_clk(priv,
							      ACLK_BUS_PRE),
					   hz);
		assert(src_clk_div - 1 < 7);
		rk_clrsetreg(&cru->clksel_con[1],
			     PCLK_BUS_DIV_CON_MASK,
			     (src_clk_div - 1) << PCLK_BUS_DIV_CON_SHIFT);
		break;
	default:
		printf("do not support this bus freq\n");
		return -EINVAL;
	}
	return rk3328_bus_get_clk(priv, clk_id);
}

static ulong rk3328_peri_get_clk(struct rk3328_clk_priv *priv, ulong clk_id)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_PERI_PRE:
		con = readl(&cru->clksel_con[28]);
		div = (con & ACLK_PERI_DIV_CON_MASK) >> ACLK_PERI_DIV_CON_SHIFT;
		parent = priv->cpll_hz;
		break;
	case HCLK_PERI:
		con = readl(&cru->clksel_con[29]);
		div = (con & HCLK_PERI_DIV_CON_MASK) >> HCLK_PERI_DIV_CON_SHIFT;
		parent = rk3328_peri_get_clk(priv, ACLK_PERI_PRE);
		break;
	case PCLK_PERI:
		con = readl(&cru->clksel_con[29]);
		div = (con & PCLK_PERI_DIV_CON_MASK) >> PCLK_PERI_DIV_CON_SHIFT;
		parent = rk3328_peri_get_clk(priv, ACLK_PERI_PRE);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3328_peri_set_clk(struct rk3328_clk_priv *priv,
				 ulong clk_id, ulong hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	switch (clk_id) {
	case ACLK_PERI_PRE:
		src_clk_div = DIV_ROUND_UP(priv->cpll_hz, hz);
		assert(src_clk_div - 1 < 31);
		if (src_clk_div > 32)
			src_clk_div = 32;
		rk_clrsetreg(&cru->clksel_con[28],
			     CLK_PERI_PLL_SEL_MASK | ACLK_PERI_DIV_CON_MASK,
			     CLK_PERI_PLL_SEL_CPLL << CLK_PERI_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_PERI_DIV_CON_SHIFT);
		break;
	case HCLK_PERI:
		src_clk_div = DIV_ROUND_UP(rk3328_peri_get_clk(priv,
							       ACLK_PERI_PRE),
					   hz);
		assert(src_clk_div - 1 < 3);
		rk_clrsetreg(&cru->clksel_con[29],
			     HCLK_PERI_DIV_CON_MASK,
			     (src_clk_div - 1) << HCLK_PERI_DIV_CON_SHIFT);
		break;
	case PCLK_PERI:
		src_clk_div = DIV_ROUND_UP(rk3328_peri_get_clk(priv,
							       ACLK_PERI_PRE),
					   hz);
		assert(src_clk_div - 1 < 7);
		rk_clrsetreg(&cru->clksel_con[29],
			     PCLK_PERI_DIV_CON_MASK,
			     (src_clk_div - 1) << PCLK_PERI_DIV_CON_SHIFT);
		break;
	default:
		printf("do not support this bus freq\n");
		return -EINVAL;
	}

	return rk3328_peri_get_clk(priv, clk_id);
}

#ifndef CONFIG_SPL_BUILD
static ulong rk3328_crypto_get_clk(struct rk3328_clk_priv *priv, ulong clk_id)
{
	struct rk3328_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case SCLK_CRYPTO:
		con = readl(&cru->clksel_con[20]);
		div = (con & CRYPTO_DIV_MASK) >> CRYPTO_DIV_SHIFT;
		parent = priv->gpll_hz;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3328_crypto_set_clk(struct rk3328_clk_priv *priv, ulong clk_id,
				   ulong hz)
{
	struct rk3328_cru *cru = priv->cru;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);
	assert(src_clk_div - 1 <= 127);

	/*
	 * select gpll as crypto clock source and
	 * set up dependent divisors for crypto clocks.
	 */
	switch (clk_id) {
	case SCLK_CRYPTO:
		rk_clrsetreg(&cru->clksel_con[20],
			     CRYPTO_PLL_SEL_MASK | CRYPTO_DIV_MASK,
			     CRYPTO_PLL_SEL_GPLL << CRYPTO_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << CRYPTO_DIV_SHIFT);
		break;
	default:
		printf("do not support this peri freq\n");
		return -EINVAL;
	}

	return rk3328_crypto_get_clk(priv, clk_id);
}
#endif

static ulong rk3328_clk_get_rate(struct clk *clk)
{
	struct rk3328_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate = 0;

#ifndef CONFIG_SPL_BUILD
	if (!priv->gpll_hz) {
		priv->gpll_hz = rockchip_pll_get_rate(&rk3328_pll_clks[GPLL],
						      priv->cru, GPLL);
		debug("%s gpll=%lu\n", __func__, priv->gpll_hz);
	}
	if (!priv->cpll_hz) {
		priv->cpll_hz = rockchip_pll_get_rate(&rk3328_pll_clks[CPLL],
						      priv->cru, CPLL);
		debug("%s cpll=%lu\n", __func__, priv->cpll_hz);
	}
#endif

	switch (clk->id) {
	case PLL_APLL:
	case PLL_DPLL:
	case PLL_CPLL:
	case PLL_GPLL:
	case PLL_NPLL:
		rate = rockchip_pll_get_rate(&rk3328_pll_clks[clk->id - 1],
					     priv->cru, clk->id - 1);
		break;
	case ARMCLK:
		rate = rockchip_pll_get_rate(&rk3328_pll_clks[NPLL],
					     priv->cru, NPLL);
		break;
	case ACLK_BUS_PRE:
	case HCLK_BUS_PRE:
	case PCLK_BUS_PRE:
		rate = rk3328_bus_get_clk(priv, clk->id);
		break;
	case ACLK_PERI_PRE:
	case HCLK_PERI:
	case PCLK_PERI:
		rate = rk3328_peri_get_clk(priv, clk->id);
		break;
	case HCLK_SDMMC:
	case HCLK_EMMC:
	case SCLK_SDMMC:
	case SCLK_EMMC:
	case SCLK_EMMC_SAMPLE:
		rate = rk3328_mmc_get_clk(priv, clk->id);
		break;
	case SCLK_SPI:
		rate = rk3328_spi_get_clk(priv);
		break;
#ifndef CONFIG_SPL_BUILD
	case SCLK_I2C0:
	case SCLK_I2C1:
	case SCLK_I2C2:
	case SCLK_I2C3:
		rate = rk3328_i2c_get_clk(priv, clk->id);
		break;
	case SCLK_PWM:
		rate = rk3328_pwm_get_clk(priv);
		break;
	case SCLK_SARADC:
		rate = rk3328_saradc_get_clk(priv);
		break;
	case SCLK_TSADC:
		rate = rk3328_tsadc_get_clk(priv);
		break;
	case ACLK_VOP_PRE:
	case ACLK_VIO_PRE:
	case HCLK_VIO_PRE:
	case ACLK_VOP:
	case ACLK_VIO:
	case HCLK_VIO:
		rate = rk3328_vop_get_clk(priv, clk->id);
		break;
	case SCLK_CRYPTO:
		rate = rk3328_crypto_get_clk(priv, clk->id);
		break;
#endif
	default:
		return -ENOENT;
	}

	return rate;
}

static ulong rk3328_clk_set_rate(struct clk *clk, ulong rate)
{
	struct rk3328_clk_priv *priv = dev_get_priv(clk->dev);
	ulong ret = 0;

	switch (clk->id) {
	case PLL_APLL:
	case PLL_DPLL:
	case PLL_NPLL:
		ret = rockchip_pll_set_rate(&rk3328_pll_clks[clk->id - 1],
					    priv->cru, clk->id - 1, rate);
		break;
	case PLL_CPLL:
		ret = rockchip_pll_set_rate(&rk3328_pll_clks[CPLL],
					    priv->cru, CPLL, rate);
		priv->cpll_hz = rate;
		break;
	case PLL_GPLL:
		ret = rockchip_pll_set_rate(&rk3328_pll_clks[GPLL],
					    priv->cru, GPLL, rate);
		priv->gpll_hz = rate;
		break;
	case ARMCLK:
		if (priv->armclk_hz)
			ret = rk3328_armclk_set_clk(priv, rate);
		priv->armclk_hz = rate;
		break;
	case ACLK_BUS_PRE:
	case HCLK_BUS_PRE:
	case PCLK_BUS_PRE:
		rate = rk3328_bus_set_clk(priv, clk->id, rate);
		break;
	case ACLK_PERI_PRE:
	case HCLK_PERI:
	case PCLK_PERI:
		rate = rk3328_peri_set_clk(priv, clk->id, rate);
		break;
	case HCLK_SDMMC:
	case HCLK_EMMC:
	case SCLK_SDMMC:
	case SCLK_EMMC:
		ret = rk3328_mmc_set_clk(priv, clk->id, rate);
		break;
	case SCLK_SPI:
		ret = rk3328_spi_set_clk(priv, rate);
		break;
#ifndef CONFIG_SPL_BUILD
	case SCLK_I2C0:
	case SCLK_I2C1:
	case SCLK_I2C2:
	case SCLK_I2C3:
		ret = rk3328_i2c_set_clk(priv, clk->id, rate);
		break;
	case SCLK_MAC2IO:
		ret = rk3328_gmac2io_set_clk(priv, rate);
		break;
	case SCLK_MAC2PHY:
		ret = rk3328_gmac2phy_set_clk(priv->cru, rate);
		break;
	case SCLK_MAC2PHY_SRC:
		ret = rk3328_gmac2phy_src_set_clk(priv->cru, rate);
		break;
	case SCLK_PWM:
		ret = rk3328_pwm_set_clk(priv, rate);
		break;
	case SCLK_SARADC:
		ret = rk3328_saradc_set_clk(priv, rate);
		break;
	case SCLK_TSADC:
		ret = rk3328_tsadc_set_clk(priv, rate);
		break;
	case DCLK_LCDC:
	case ACLK_VOP_PRE:
	case ACLK_VIO_PRE:
	case HCLK_VIO_PRE:
	case ACLK_VOP:
	case ACLK_VIO:
	case HCLK_VIO:
		rate = rk3328_vop_set_clk(priv, clk->id, rate);
		break;
	case SCLK_CRYPTO:
		rate = rk3328_crypto_set_clk(priv, clk->id, rate);
		break;
#endif
	case SCLK_PDM:
	case SCLK_RTC32K:
	case SCLK_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
	case SCLK_SDIO:
	case SCLK_TSP:
	case SCLK_WIFI:
	case ACLK_RGA_PRE:
	case SCLK_RGA:
	case ACLK_RKVDEC_PRE:
	case ACLK_RKVENC:
	case ACLK_VPU_PRE:
	case SCLK_VDEC_CABAC:
	case SCLK_VDEC_CORE:
	case SCLK_VENC_CORE:
	case SCLK_VENC_DSP:
	case SCLK_EFUSE:
	case PCLK_DDR:
	case ACLK_GMAC:
	case PCLK_GMAC:
	case SCLK_USB3OTG_SUSPEND:
		return 0;
	default:
		return -ENOENT;
	}

	return ret;
}

#ifndef CONFIG_SPL_BUILD
static int rk3328_gmac2io_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk3328_grf_regs *grf;
	const char *clock_output_name;
	int ret;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	/*
	 * If the requested parent is in the same clock-controller and the id
	 * is SCLK_MAC2IO_SRC ("clk_mac2io_src"), switch to the internal clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC2IO_SRC)) {
		debug("%s: switching RGMII to SCLK_MAC2IO_SRC\n", __func__);
		rk_clrreg(&grf->mac_con[1], BIT(10));
		return 0;
	}

	/*
	 * Otherwise, we need to check the clock-output-names of the
	 * requested parent to see if the requested id is "gmac_clkin".
	 */
	ret = dev_read_string_index(parent->dev, "clock-output-names",
				    parent->id, &clock_output_name);
	if (ret < 0)
		return -ENODATA;

	/* If this is "gmac_clkin", switch to the external clock input */
	if (!strcmp(clock_output_name, "gmac_clkin")) {
		debug("%s: switching RGMII to CLKIN\n", __func__);
		rk_setreg(&grf->mac_con[1], BIT(10));
		return 0;
	}

	return -EINVAL;
}

static int rk3328_gmac2io_ext_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk3328_grf_regs *grf;
	const char *clock_output_name;
	int ret;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	/*
	 * If the requested parent is in the same clock-controller and the id
	 * is SCLK_MAC2IO ("clk_mac2io"), switch to the internal clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC2IO)) {
		debug("%s: switching RGMII to SCLK_MAC2IO\n", __func__);
		rk_clrreg(&grf->soc_con[4], BIT(14));
		return 0;
	}

	/*
	 * Otherwise, we need to check the clock-output-names of the
	 * requested parent to see if the requested id is "gmac_clkin".
	 */
	ret = dev_read_string_index(parent->dev, "clock-output-names",
				    parent->id, &clock_output_name);
	if (ret < 0)
		return -ENODATA;

	/* If this is "gmac_clkin", switch to the external clock input */
	if (!strcmp(clock_output_name, "gmac_clkin")) {
		debug("%s: switching RGMII to CLKIN\n", __func__);
		rk_setreg(&grf->soc_con[4], BIT(14));
		return 0;
	}

	return -EINVAL;
}

static int rk3328_gmac2phy_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk3328_grf_regs *grf;
	const char *clock_output_name;
	int ret;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	/*
	 * If the requested parent is in the same clock-controller and the id
	 * is SCLK_MAC2PHY_SRC ("clk_mac2phy_src"), switch to the internal clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC2PHY_SRC)) {
		debug("%s: switching MAC CLK to SCLK_MAC2IO_PHY\n", __func__);
		rk_clrreg(&grf->mac_con[2], BIT(10));
		return 0;
	}

	/*
	 * Otherwise, we need to check the clock-output-names of the
	 * requested parent to see if the requested id is "phy_50m_out".
	 */
	ret = dev_read_string_index(parent->dev, "clock-output-names",
				    parent->id, &clock_output_name);
	if (ret < 0)
		return -ENODATA;

	/* If this is "phy_50m_out", switch to the external clock input */
	if (!strcmp(clock_output_name, "phy_50m_out")) {
		debug("%s: switching MAC CLK to PHY_50M_OUT\n", __func__);
		rk_setreg(&grf->mac_con[2], BIT(10));
		return 0;
	}

	return -EINVAL;
}

static int rk3328_lcdc_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk3328_clk_priv *priv = dev_get_priv(clk->dev);

	if (parent->id == HDMIPHY)
		rk_clrsetreg(&priv->cru->clksel_con[40],
			     DCLK_LCDC_SEL_MASK,
			     DCLK_LCDC_SEL_HDMIPHY << DCLK_LCDC_SEL_SHIFT);
	else if (parent->id == PLL_CPLL)
		rk_clrsetreg(&priv->cru->clksel_con[40],
			     DCLK_LCDC_SEL_MASK | DCLK_LCDC_PLL_SEL_MASK,
			     (DCLK_LCDC_SEL_PLL << DCLK_LCDC_SEL_SHIFT) |
			     (DCLK_LCDC_PLL_SEL_CPLL <<
			     DCLK_LCDC_PLL_SEL_SHIFT));
	else
		rk_clrsetreg(&priv->cru->clksel_con[40],
			     DCLK_LCDC_SEL_MASK | DCLK_LCDC_PLL_SEL_MASK,
			     (DCLK_LCDC_SEL_PLL << DCLK_LCDC_SEL_SHIFT) |
			     (DCLK_LCDC_PLL_SEL_GPLL <<
			     DCLK_LCDC_PLL_SEL_SHIFT));

	return 0;
}
#endif

static int rk3328_clk_set_parent(struct clk *clk, struct clk *parent)
{
	switch (clk->id) {
#ifndef CONFIG_SPL_BUILD
	case SCLK_MAC2IO:
		return rk3328_gmac2io_set_parent(clk, parent);
	case SCLK_MAC2IO_EXT:
		return rk3328_gmac2io_ext_set_parent(clk, parent);
	case SCLK_MAC2PHY:
		return rk3328_gmac2phy_set_parent(clk, parent);
	case DCLK_LCDC:
		return rk3328_lcdc_set_parent(clk, parent);
#endif
	case SCLK_PDM:
	case SCLK_RTC32K:
	case SCLK_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
		return 0;
	}

	debug("%s: unsupported clk %ld\n", __func__, clk->id);
	return -ENOENT;
}

#define ROCKCHIP_MMC_DELAY_SEL		BIT(10)
#define ROCKCHIP_MMC_DEGREE_MASK	0x3
#define ROCKCHIP_MMC_DELAYNUM_OFFSET	2
#define ROCKCHIP_MMC_DELAYNUM_MASK	(0xff << ROCKCHIP_MMC_DELAYNUM_OFFSET)

#define PSECS_PER_SEC 1000000000000LL
/*
 * Each fine delay is between 44ps-77ps. Assume each fine delay is 60ps to
 * simplify calculations. So 45degs could be anywhere between 33deg and 57.8deg.
 */
#define ROCKCHIP_MMC_DELAY_ELEMENT_PSEC 60

int rk3328_mmc_get_phase(struct clk *clk)
{
	struct rk3328_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3328_cru *cru = priv->cru;
	u32 raw_value, delay_num;
	u16 degrees = 0;
	ulong rate;

	rate = rk3328_clk_get_rate(clk);

	if (rate < 0)
		return rate;

	if (clk->id == SCLK_EMMC_SAMPLE)
		raw_value = readl(&cru->emmc_con[1]);
	else if (clk->id == SCLK_SDMMC_SAMPLE)
		raw_value = readl(&cru->sdmmc_con[1]);
	else
		raw_value = readl(&cru->sdio_con[1]);

	raw_value >>= 1;
	degrees = (raw_value & ROCKCHIP_MMC_DEGREE_MASK) * 90;

	if (raw_value & ROCKCHIP_MMC_DELAY_SEL) {
		/* degrees/delaynum * 10000 */
		unsigned long factor = (ROCKCHIP_MMC_DELAY_ELEMENT_PSEC / 10) *
					36 * (rate / 1000000);

		delay_num = (raw_value & ROCKCHIP_MMC_DELAYNUM_MASK);
		delay_num >>= ROCKCHIP_MMC_DELAYNUM_OFFSET;
		degrees += DIV_ROUND_CLOSEST(delay_num * factor, 10000);
	}

	return degrees % 360;
}

int rk3328_mmc_set_phase(struct clk *clk, u32 degrees)
{
	struct rk3328_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3328_cru *cru = priv->cru;
	u8 nineties, remainder, delay_num;
	u32 raw_value, delay;
	ulong rate;

	rate = rk3328_clk_get_rate(clk);

	if (rate < 0)
		return rate;

	nineties = degrees / 90;
	remainder = (degrees % 90);

	/*
	 * Convert to delay; do a little extra work to make sure we
	 * don't overflow 32-bit / 64-bit numbers.
	 */
	delay = 10000000; /* PSECS_PER_SEC / 10000 / 10 */
	delay *= remainder;
	delay = DIV_ROUND_CLOSEST(delay, (rate / 1000) * 36 *
				(ROCKCHIP_MMC_DELAY_ELEMENT_PSEC / 10));

	delay_num = (u8)min_t(u32, delay, 255);

	raw_value = delay_num ? ROCKCHIP_MMC_DELAY_SEL : 0;
	raw_value |= delay_num << ROCKCHIP_MMC_DELAYNUM_OFFSET;
	raw_value |= nineties;

	raw_value <<= 1;
	if (clk->id == SCLK_EMMC_SAMPLE)
		writel(raw_value | 0xffff0000, &cru->emmc_con[1]);
	else if (clk->id == SCLK_SDMMC_SAMPLE)
		writel(raw_value | 0xffff0000, &cru->sdmmc_con[1]);
	else
		writel(raw_value | 0xffff0000, &cru->sdio_con[1]);

	debug("mmc set_phase(%d) delay_nums=%u reg=%#x actual_degrees=%d\n",
	      degrees, delay_num, raw_value, rk3328_mmc_get_phase(clk));

	return 0;
}

static int rk3328_clk_get_phase(struct clk *clk)
{
	int ret;

	debug("%s %ld\n", __func__, clk->id);
	switch (clk->id) {
	case SCLK_EMMC_SAMPLE:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO_SAMPLE:
		ret = rk3328_mmc_get_phase(clk);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static int rk3328_clk_set_phase(struct clk *clk, int degrees)
{
	int ret;

	debug("%s %ld\n", __func__, clk->id);
	switch (clk->id) {
	case SCLK_EMMC_SAMPLE:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO_SAMPLE:
		ret = rk3328_mmc_set_phase(clk, degrees);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static struct clk_ops rk3328_clk_ops = {
	.get_rate = rk3328_clk_get_rate,
	.set_rate = rk3328_clk_set_rate,
	.set_parent = rk3328_clk_set_parent,
	.get_phase = rk3328_clk_get_phase,
	.set_phase = rk3328_clk_set_phase,
};

static void rkclk_init(struct rk3328_clk_priv *priv)
{
	if (rockchip_pll_get_rate(&rk3328_pll_clks[NPLL],
				  priv->cru, NPLL) != APLL_HZ)
		rk3328_armclk_set_clk(priv, APLL_HZ);

	priv->gpll_hz = rockchip_pll_get_rate(&rk3328_pll_clks[GPLL],
					      priv->cru, GPLL);
	priv->cpll_hz = rockchip_pll_get_rate(&rk3328_pll_clks[CPLL],
					      priv->cru, CPLL);

	/* before set pll set child div first */
	rk_clrsetreg(&priv->cru->clksel_con[24], (0x3f << 8) | (0x3f << 0),
		     (0x17 << 8) | (0x17 << 0));
	rk_clrsetreg(&priv->cru->clksel_con[27], (0x1f << 8) | (0x1f << 0),
		     (0x17 << 8) | (0x17 << 0));
	rk_clrsetreg(&priv->cru->clksel_con[31], 0xff << 0, 0xb << 0);
	rk_clrsetreg(&priv->cru->clksel_con[43], 0xff << 0, 0xb << 0);
	rk_clrsetreg(&priv->cru->clksel_con[52], 0x1f << 8, 0x5 << 8);

	rockchip_pll_set_rate(&rk3328_pll_clks[GPLL],
			      priv->cru, GPLL, GPLL_HZ);
	priv->gpll_hz = GPLL_HZ;

	rockchip_pll_set_rate(&rk3328_pll_clks[CPLL],
			      priv->cru, CPLL, CPLL_HZ);
	priv->cpll_hz = CPLL_HZ;

	rk3328_bus_set_clk(priv, ACLK_BUS_PRE, ACLK_BUS_HZ);
	rk3328_bus_set_clk(priv, HCLK_BUS_PRE, ACLK_BUS_HZ / 2);
	rk3328_bus_set_clk(priv, PCLK_BUS_PRE, ACLK_BUS_HZ / 2);
	rk3328_peri_set_clk(priv, ACLK_PERI_PRE, ACLK_PERI_HZ);
	rk3328_peri_set_clk(priv, HCLK_PERI, ACLK_PERI_HZ / 2);
	rk3328_peri_set_clk(priv, PCLK_PERI, ACLK_PERI_HZ / 2);
	/*rk3328_mmc_set_clk(priv, SCLK_EMMC, rate);*/

	/* set usbphy and hdmiphy from phy */
	rk_clrsetreg(&priv->cru->misc, (0x1 << 13) |
		     (0x1 << 15), (0 << 15) | (0 << 13));
}

static int rk3328_clk_probe(struct udevice *dev)
{
	struct rk3328_clk_priv *priv = dev_get_priv(dev);
	int ret = 0;

	priv->sync_kernel = false;
	if (!priv->armclk_enter_hz)
		priv->armclk_enter_hz =
		rockchip_pll_get_rate(&rk3328_pll_clks[NPLL],
				      priv->cru, NPLL);
	rkclk_init(priv);
	if (!priv->armclk_init_hz)
		priv->armclk_init_hz =
		rockchip_pll_get_rate(&rk3328_pll_clks[NPLL],
				      priv->cru, NPLL);

	ret = clk_set_defaults(dev);
	if (ret)
		debug("%s clk_set_defaults failed %d\n", __func__, ret);
	else
		priv->sync_kernel = true;

	return 0;
}

static int rk3328_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct rk3328_clk_priv *priv = dev_get_priv(dev);

	priv->cru = dev_read_addr_ptr(dev);

	return 0;
}

static int rk3328_clk_bind(struct udevice *dev)
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
		priv->glb_srst_fst_value = offsetof(struct rk3328_cru,
						    glb_srst_fst_value);
		priv->glb_srst_snd_value = offsetof(struct rk3328_cru,
						    glb_srst_snd_value);
		sys_child->priv = priv;
	}

	ret = device_bind_driver_to_node(dev, "rockchip_reset", "reset",
					 dev_ofnode(dev), &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk3328_cru,
						    softrst_con[0]);
		sf_priv->sf_reset_num = 12;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk3328_clk_ids[] = {
	{ .compatible = "rockchip,rk3328-cru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3328_cru) = {
	.name		= "rockchip_rk3328_cru",
	.id		= UCLASS_CLK,
	.of_match	= rk3328_clk_ids,
	.priv_auto_alloc_size = sizeof(struct rk3328_clk_priv),
	.ofdata_to_platdata = rk3328_clk_ofdata_to_platdata,
	.ops		= &rk3328_clk_ops,
	.bind		= rk3328_clk_bind,
	.probe		= rk3328_clk_probe,
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
	struct udevice *cru_dev;
	struct rk3328_clk_priv *priv;
	const struct rk3328_clk_info *clk_dump;
	struct clk clk;
	unsigned long clk_count = ARRAY_SIZE(clks_dump);
	unsigned long rate;
	int i, ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(rockchip_rk3328_cru),
					  &cru_dev);
	if (ret) {
		printf("%s failed to get cru device\n", __func__);
		return ret;
	}

	priv = dev_get_priv(cru_dev);
	printf("CLK: (%s. arm: enter %lu KHz, init %lu KHz, kernel %lu%s)\n",
	       priv->sync_kernel ? "sync kernel" : "uboot",
	       priv->armclk_enter_hz / 1000,
	       priv->armclk_init_hz / 1000,
	       priv->set_armclk_rate ? priv->armclk_hz / 1000 : 0,
	       priv->set_armclk_rate ? " KHz" : "N/A");
	for (i = 0; i < clk_count; i++) {
		clk_dump = &clks_dump[i];
		if (clk_dump->name) {
			clk.id = clk_dump->id;
			if (clk_dump->is_cru)
				ret = clk_request(cru_dev, &clk);
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
