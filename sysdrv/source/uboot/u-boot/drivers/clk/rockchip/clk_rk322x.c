/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <errno.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/cru_rk322x.h>
#include <asm/arch/hardware.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rk3228-cru.h>
#include <linux/log2.h>

DECLARE_GLOBAL_DATA_PTR;

#define DIV_TO_RATE(input_rate, div)	((input_rate) / ((div) + 1))

#ifndef CONFIG_SPL_BUILD
#define RK322x_CLK_DUMP(_id, _name, _iscru)	\
{						\
	.id = _id,				\
	.name = _name,				\
	.is_cru = _iscru,			\
}
#endif

static struct rockchip_pll_rate_table rk322x_pll_rates[] = {
	/* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac */
	RK3036_PLL_RATE(1200000000, 1, 50, 1, 1, 1, 0),
#ifndef CONFIG_SPL_BUILD
	RK3036_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
	RK3036_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
#endif
	RK3036_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
	RK3036_PLL_RATE(800000000, 1, 100, 3, 1, 1, 0),
	RK3036_PLL_RATE(600000000, 1, 75, 3, 1, 1, 0),
#ifndef CONFIG_SPL_BUILD
	RK3036_PLL_RATE(594000000, 2, 99, 2, 1, 1, 0),
	RK3036_PLL_RATE(500000000, 1, 125, 6, 1, 1, 0),
	RK3036_PLL_RATE(400000000, 1, 50, 3, 1, 1, 0),
#endif
	{ /* sentinel */ },
};

#define RK322x_CPUCLK_RATE(_rate, _aclk_div, _pclk_div)		\
{								\
	.rate	= _rate##U,					\
	.aclk_div = _aclk_div,					\
	.pclk_div = _pclk_div,					\
}

static struct rockchip_cpu_rate_table rk322x_cpu_rates[] = {
	RK322x_CPUCLK_RATE(1200000000, 1, 5),
	RK322x_CPUCLK_RATE(1008000000, 1, 5),
	RK322x_CPUCLK_RATE(816000000, 1, 3),
	RK322x_CPUCLK_RATE(600000000, 1, 3),
};

#ifndef CONFIG_SPL_BUILD
static const struct rk322x_clk_info clks_dump[] = {
	RK322x_CLK_DUMP(PLL_APLL, "apll", true),
	RK322x_CLK_DUMP(PLL_DPLL, "dpll", true),
	RK322x_CLK_DUMP(PLL_CPLL, "cpll", true),
	RK322x_CLK_DUMP(PLL_GPLL, "gpll", true),
	RK322x_CLK_DUMP(ARMCLK, "armclk", true),
	RK322x_CLK_DUMP(ACLK_CPU, "aclk_bus", true),
	RK322x_CLK_DUMP(HCLK_CPU, "hclk_bus", true),
	RK322x_CLK_DUMP(PCLK_CPU, "pclk_bus", true),
	RK322x_CLK_DUMP(ACLK_PERI, "aclk_peri", true),
	RK322x_CLK_DUMP(HCLK_PERI, "hclk_peri", true),
	RK322x_CLK_DUMP(PCLK_PERI, "pclk_peri", true),
};
#endif

static struct rockchip_pll_clock rk322x_pll_clks[] = {
	[APLL] = PLL(pll_rk3036, PLL_APLL, RK2928_PLL_CON(0),
		     RK2928_MODE_CON, 0, 10, 0, rk322x_pll_rates),
	[DPLL] = PLL(pll_rk3036, PLL_DPLL, RK2928_PLL_CON(3),
		     RK2928_MODE_CON, 4, 10, 0, rk322x_pll_rates),
	[CPLL] = PLL(pll_rk3036, PLL_CPLL, RK2928_PLL_CON(6),
		    RK2928_MODE_CON, 8, 10, 0, rk322x_pll_rates),
	[GPLL] = PLL(pll_rk3036, PLL_GPLL, RK2928_PLL_CON(9),
		     RK2928_MODE_CON, 12, 10, 0, rk322x_pll_rates),
};

static ulong rk322x_armclk_set_clk(struct rk322x_clk_priv *priv, ulong hz)
{
	struct rk322x_cru *cru = priv->cru;
	const struct rockchip_cpu_rate_table *rate;
	ulong old_rate;

	rate = rockchip_get_cpu_settings(rk322x_cpu_rates, hz);
	if (!rate) {
		printf("%s unsupported rate\n", __func__);
		return -EINVAL;
	}

	/*
	 * select apll as cpu/core clock pll source and
	 * set up dependent divisors for PERI and ACLK clocks.
	 * core hz : apll = 1:1
	 */
	old_rate = rockchip_pll_get_rate(&rk322x_pll_clks[APLL],
					 priv->cru, APLL);
	if (old_rate > hz) {
		if (rockchip_pll_set_rate(&rk322x_pll_clks[APLL],
					  priv->cru, APLL, hz))
			return -EINVAL;
		rk_clrsetreg(&cru->cru_clksel_con[0],
			     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
			     CORE_CLK_PLL_SEL_APLL << CORE_CLK_PLL_SEL_SHIFT |
			     0 << CORE_DIV_CON_SHIFT);
		rk_clrsetreg(&cru->cru_clksel_con[1],
			     CORE_ACLK_DIV_MASK | CORE_PERI_DIV_MASK,
			     rate->aclk_div << CORE_ACLK_DIV_SHIFT |
			     rate->pclk_div << CORE_PERI_DIV_SHIFT);
	} else if (old_rate < hz) {
		rk_clrsetreg(&cru->cru_clksel_con[1],
			     CORE_ACLK_DIV_MASK | CORE_PERI_DIV_MASK,
			     rate->aclk_div << CORE_ACLK_DIV_SHIFT |
			     rate->pclk_div << CORE_PERI_DIV_SHIFT);
		rk_clrsetreg(&cru->cru_clksel_con[0],
			     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
			     CORE_CLK_PLL_SEL_APLL << CORE_CLK_PLL_SEL_SHIFT |
			     0 << CORE_DIV_CON_SHIFT);
		if (rockchip_pll_set_rate(&rk322x_pll_clks[APLL],
					  priv->cru, APLL, hz))
			return -EINVAL;
	}

	return rockchip_pll_get_rate(&rk322x_pll_clks[APLL], priv->cru, APLL);
}

static ulong rk322x_mmc_get_clk(struct rk322x_clk_priv *priv,
				int periph)
{
	struct rk322x_cru *cru = priv->cru;
	uint src_rate;
	uint div, mux;
	u32 con;

	switch (periph) {
	case HCLK_EMMC:
	case SCLK_EMMC:
	case SCLK_EMMC_SAMPLE:
		con = readl(&cru->cru_clksel_con[11]);
		mux = (con & EMMC_PLL_MASK) >> EMMC_PLL_SHIFT;
		con = readl(&cru->cru_clksel_con[12]);
		div = (con & EMMC_DIV_MASK) >> EMMC_DIV_SHIFT;
		break;
	case HCLK_SDMMC:
	case SCLK_SDMMC:
	case SCLK_SDMMC_SAMPLE:
		con = readl(&cru->cru_clksel_con[11]);
		mux = (con & MMC0_PLL_MASK) >> MMC0_PLL_SHIFT;
		div = (con & MMC0_DIV_MASK) >> MMC0_DIV_SHIFT;
		break;
	case SCLK_SDIO:
	case SCLK_SDIO_SAMPLE:
		con = readl(&cru->cru_clksel_con[11]);
		mux = (con & SDIO_PLL_MASK) >> SDIO_PLL_SHIFT;
		con = readl(&cru->cru_clksel_con[12]);
		div = (con & SDIO_DIV_MASK) >> SDIO_DIV_SHIFT;
		break;
	default:
		return -EINVAL;
	}

	src_rate = mux == EMMC_SEL_24M ? OSC_HZ : priv->gpll_hz;
	return DIV_TO_RATE(src_rate, div) / 2;
}

#ifndef CONFIG_SPL_BUILD
static ulong rk322x_mac_set_clk(struct rk322x_clk_priv *priv, uint freq)
{
	struct rk322x_cru *cru = priv->cru;
	ulong ret;

	/*
	 * The gmac clock can be derived either from an external clock
	 * or can be generated from internally by a divider from SCLK_MAC.
	 */
	if (readl(&cru->cru_clksel_con[5]) & BIT(5)) {
		/* An external clock will always generate the right rate... */
		ret = freq;
	} else {
		u32 con = readl(&cru->cru_clksel_con[5]);
		ulong pll_rate;
		u8 div;

		if (con & MAC_PLL_SEL_MASK)
			pll_rate = priv->gpll_hz;
		else
			/* CPLL is not set */
			return -EPERM;

		div = DIV_ROUND_UP(pll_rate, freq) - 1;
		if (div <= 0x1f)
			rk_clrsetreg(&cru->cru_clksel_con[5], CLK_MAC_DIV_MASK,
				     div << CLK_MAC_DIV_SHIFT);
		else
			debug("Unsupported div for gmac:%d\n", div);

		return DIV_TO_RATE(pll_rate, div);
	}

	return ret;
}
#endif

static ulong rk322x_mmc_set_clk(struct rk322x_clk_priv *priv,
				int periph, uint freq)
{
	struct rk322x_cru *cru = priv->cru;
	int src_clk_div;
	int mux;

	/* mmc clock defaulg div 2 internal, need provide double in cru */
	src_clk_div = DIV_ROUND_UP(priv->gpll_hz / 2, freq);

	if (src_clk_div > 128) {
		src_clk_div = DIV_ROUND_UP(OSC_HZ / 2, freq);
		assert(src_clk_div - 1 < 128);
		mux = EMMC_SEL_24M;
	} else {
		mux = EMMC_SEL_GPLL;
	}

	switch (periph) {
	case HCLK_EMMC:
	case SCLK_EMMC:
	case SCLK_EMMC_SAMPLE:
		rk_clrsetreg(&cru->cru_clksel_con[11],
			     EMMC_PLL_MASK,
			     mux << EMMC_PLL_SHIFT);
		rk_clrsetreg(&cru->cru_clksel_con[12],
			     EMMC_DIV_MASK,
			     (src_clk_div - 1) << EMMC_DIV_SHIFT);
		break;
	case HCLK_SDMMC:
	case SCLK_SDMMC:
	case SCLK_SDMMC_SAMPLE:
		rk_clrsetreg(&cru->cru_clksel_con[11],
			     MMC0_PLL_MASK | MMC0_DIV_MASK,
			     mux << MMC0_PLL_SHIFT |
			     (src_clk_div - 1) << MMC0_DIV_SHIFT);
		break;
	case SCLK_SDIO:
	case SCLK_SDIO_SAMPLE:
		rk_clrsetreg(&cru->cru_clksel_con[11],
			     SDIO_PLL_MASK,
			     mux << SDIO_PLL_SHIFT);
		rk_clrsetreg(&cru->cru_clksel_con[12],
			     SDIO_DIV_MASK,
			     (src_clk_div - 1) << SDIO_DIV_SHIFT);
		break;
	default:
		return -EINVAL;
	}

	return rk322x_mmc_get_clk(priv, periph);
}

static ulong rk322x_bus_get_clk(struct rk322x_clk_priv *priv, ulong clk_id)
{
	struct rk322x_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_CPU:
		con = readl(&cru->cru_clksel_con[0]);
		div = (con & BUS_ACLK_DIV_MASK) >> BUS_ACLK_DIV_SHIFT;
		parent = priv->gpll_hz;
		break;
	case HCLK_CPU:
		con = readl(&cru->cru_clksel_con[1]);
		div = (con & BUS_HCLK_DIV_MASK) >> BUS_HCLK_DIV_SHIFT;
		parent = rk322x_bus_get_clk(priv, ACLK_CPU);
		break;
	case PCLK_CPU:
	case PCLK_I2C0:
	case PCLK_I2C1:
	case PCLK_I2C2:
	case PCLK_I2C3:
	case PCLK_PWM:
		con = readl(&cru->cru_clksel_con[1]);
		div = (con & BUS_PCLK_DIV_MASK) >> BUS_PCLK_DIV_SHIFT;
		parent = rk322x_bus_get_clk(priv, ACLK_CPU);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk322x_bus_set_clk(struct rk322x_clk_priv *priv,
				ulong clk_id, ulong hz)
{
	struct rk322x_cru *cru = priv->cru;
	int src_clk_div;

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	switch (clk_id) {
	case ACLK_CPU:
		src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);
		assert(src_clk_div - 1 < 32);
		rk_clrsetreg(&cru->cru_clksel_con[0],
			     BUS_ACLK_PLL_SEL_MASK | BUS_ACLK_DIV_MASK,
			     BUS_ACLK_PLL_SEL_GPLL << BUS_ACLK_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << BUS_ACLK_DIV_SHIFT);
		break;
	case HCLK_CPU:
		src_clk_div = DIV_ROUND_UP(rk322x_bus_get_clk(priv,
							      ACLK_CPU),
					   hz);
		assert(src_clk_div - 1 < 4);
		rk_clrsetreg(&cru->cru_clksel_con[1],
			     BUS_HCLK_DIV_MASK,
			     (src_clk_div - 1) << BUS_HCLK_DIV_SHIFT);
		break;
	case PCLK_CPU:
		src_clk_div = DIV_ROUND_UP(rk322x_bus_get_clk(priv,
							      ACLK_CPU),
					   hz);
		assert(src_clk_div - 1 < 8);
		rk_clrsetreg(&cru->cru_clksel_con[1],
			     BUS_PCLK_DIV_MASK,
			     (src_clk_div - 1) << BUS_PCLK_DIV_SHIFT);
		break;
	default:
		printf("do not support this bus freq\n");
		return -EINVAL;
	}

	return rk322x_bus_get_clk(priv, clk_id);
}

static ulong rk322x_peri_get_clk(struct rk322x_clk_priv *priv, ulong clk_id)
{
	struct rk322x_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_PERI:
		con = readl(&cru->cru_clksel_con[10]);
		div = (con & PERI_ACLK_DIV_MASK) >> PERI_ACLK_DIV_SHIFT;
		parent = priv->gpll_hz;
		break;
	case HCLK_PERI:
		con = readl(&cru->cru_clksel_con[10]);
		div = (con & PERI_HCLK_DIV_MASK) >> PERI_HCLK_DIV_SHIFT;
		parent = rk322x_peri_get_clk(priv, ACLK_PERI);
		break;
	case PCLK_PERI:
		con = readl(&cru->cru_clksel_con[10]);
		div = (con & PERI_PCLK_DIV_MASK) >> PERI_PCLK_DIV_SHIFT;
		parent = rk322x_peri_get_clk(priv, ACLK_PERI);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk322x_peri_set_clk(struct rk322x_clk_priv *priv,
				 ulong clk_id, ulong hz)
{
	struct rk322x_cru *cru = priv->cru;
	int src_clk_div;

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	switch (clk_id) {
	case ACLK_PERI:
		src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);
		assert(src_clk_div - 1 < 32);
		rk_clrsetreg(&cru->cru_clksel_con[10],
			     PERI_PLL_SEL_MASK | PERI_ACLK_DIV_MASK,
			     PERI_PLL_GPLL << PERI_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << PERI_ACLK_DIV_SHIFT);
		break;
	case HCLK_PERI:
		src_clk_div = DIV_ROUND_UP(rk322x_peri_get_clk(priv,
							       ACLK_PERI),
					   hz);
		assert(src_clk_div - 1 < 4);
		rk_clrsetreg(&cru->cru_clksel_con[10],
			     PERI_HCLK_DIV_MASK,
			     (src_clk_div - 1) << PERI_HCLK_DIV_SHIFT);
		break;
	case PCLK_PERI:
		src_clk_div = DIV_ROUND_UP(rk322x_peri_get_clk(priv,
							       ACLK_PERI),
					   hz);
		assert(src_clk_div - 1 < 8);
		rk_clrsetreg(&cru->cru_clksel_con[10],
			     PERI_PCLK_DIV_MASK,
			     (src_clk_div - 1) << PERI_PCLK_DIV_SHIFT);
		break;
	default:
		printf("do not support this bus freq\n");
		return -EINVAL;
	}

	return rk322x_peri_get_clk(priv, clk_id);
}

static ulong rk322x_spi_get_clk(struct rk322x_clk_priv *priv)
{
	struct rk322x_cru *cru = priv->cru;
	u32 div, con, parent;

	con = readl(&cru->cru_clksel_con[25]);
	div = (con & SPI_DIV_MASK) >> SPI_DIV_SHIFT;
	parent = priv->gpll_hz;

	return DIV_TO_RATE(parent, div);
}

static ulong rk322x_spi_set_clk(struct rk322x_clk_priv *priv, ulong hz)
{
	struct rk322x_cru *cru = priv->cru;
	int div;

	div = DIV_ROUND_UP(priv->gpll_hz, hz);
	assert(div - 1 < 128);
	rk_clrsetreg(&cru->cru_clksel_con[25],
		     SPI_PLL_SEL_MASK | SPI_DIV_MASK,
		     SPI_PLL_SEL_GPLL << SPI_PLL_SEL_SHIFT |
		     (div - 1) << SPI_DIV_SHIFT);
	return rk322x_spi_get_clk(priv);
}

#ifndef CONFIG_SPL_BUILD
static ulong rk322x_vop_get_clk(struct rk322x_clk_priv *priv, ulong clk_id)
{
	struct rk322x_cru *cru = priv->cru;
	u32 div, con, sel, parent;

	switch (clk_id) {
	case ACLK_VOP:
		con = readl(&cru->cru_clksel_con[33]);
		div = (con & ACLK_VOP_DIV_CON_MASK) >> ACLK_VOP_DIV_CON_SHIFT;
		parent = priv->gpll_hz;
		break;
	case DCLK_VOP:
		con = readl(&cru->cru_clksel_con[27]);
		con = (con & DCLK_LCDC_SEL_MASK) >> DCLK_LCDC_SEL_SHIFT;
		if (con) {
			sel = readl(&cru->cru_clksel_con[27]);
			sel = (sel & DCLK_LCDC_PLL_SEL_MASK) >>
				 DCLK_LCDC_PLL_SEL_SHIFT;
			if (sel)
				parent = priv->cpll_hz;
			else
				parent = priv->gpll_hz;

			con = readl(&cru->cru_clksel_con[27]);
			div = (con & DCLK_LCDC_DIV_CON_MASK) >>
			      DCLK_LCDC_DIV_CON_SHIFT;
		} else {
			parent = priv->cpll_hz;
			div = 1;
		}
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk322x_vop_set_clk(struct rk322x_clk_priv *priv,
				ulong clk_id, uint hz)
{
	struct rk322x_cru *cru = priv->cru;
	int src_clk_div;
	u32 con, parent;

	switch (clk_id) {
	case ACLK_VOP:
		src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);
		assert(src_clk_div - 1 < 32);
		rk_clrsetreg(&cru->cru_clksel_con[33],
			     ACLK_VOP_PLL_SEL_MASK | ACLK_VOP_DIV_CON_MASK,
			     ACLK_VOP_PLL_SEL_GPLL << ACLK_VOP_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_VOP_DIV_CON_SHIFT);
		break;
	case DCLK_VOP:
		con = readl(&cru->cru_clksel_con[27]);
		con = (con & DCLK_LCDC_SEL_MASK) >> DCLK_LCDC_SEL_SHIFT;
		if (con) {
			parent = readl(&cru->cru_clksel_con[27]);
			parent = (parent & DCLK_LCDC_PLL_SEL_MASK) >>
				 DCLK_LCDC_PLL_SEL_SHIFT;
			if (parent)
				src_clk_div = DIV_ROUND_UP(priv->cpll_hz, hz);
			else
				src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);

			assert(src_clk_div - 1 < 256);
			rk_clrsetreg(&cru->cru_clksel_con[27],
				     DCLK_LCDC_DIV_CON_MASK,
				     (src_clk_div - 1) <<
				     DCLK_LCDC_DIV_CON_SHIFT);
		}
		break;
	default:
		printf("do not support this vop freq\n");
		return -EINVAL;
	}

	return rk322x_vop_get_clk(priv, clk_id);
}

static ulong rk322x_crypto_get_clk(struct rk322x_clk_priv *priv, ulong clk_id)
{
	struct rk322x_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case SCLK_CRYPTO:
		con = readl(&cru->cru_clksel_con[24]);
		div = (con & CRYPTO_DIV_MASK) >> CRYPTO_DIV_SHIFT;
		parent = priv->gpll_hz;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk322x_crypto_set_clk(struct rk322x_clk_priv *priv, ulong clk_id,
				   ulong hz)
{
	struct rk322x_cru *cru = priv->cru;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(priv->gpll_hz, hz);
	assert(src_clk_div - 1 <= 31);

	/*
	 * select gpll as crypto clock source and
	 * set up dependent divisors for crypto clocks.
	 */
	switch (clk_id) {
	case SCLK_CRYPTO:
		rk_clrsetreg(&cru->cru_clksel_con[24],
			     CRYPTO_PLL_SEL_MASK | CRYPTO_DIV_MASK,
			     CRYPTO_PLL_SEL_GPLL << CRYPTO_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << CRYPTO_DIV_SHIFT);
		break;
	default:
		printf("do not support this peri freq\n");
		return -EINVAL;
	}

	return rk322x_crypto_get_clk(priv, clk_id);
}
#endif

static ulong rk322x_clk_get_rate(struct clk *clk)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate;

	switch (clk->id) {
	case PLL_APLL:
	case PLL_DPLL:
	case PLL_CPLL:
	case PLL_GPLL:
		rate = rockchip_pll_get_rate(&rk322x_pll_clks[clk->id - 1],
					     priv->cru, clk->id - 1);
		break;
	case ARMCLK:
		rate = rockchip_pll_get_rate(&rk322x_pll_clks[APLL],
					     priv->cru, APLL);
		break;
	case HCLK_EMMC:
	case SCLK_EMMC:
	case SCLK_EMMC_SAMPLE:
	case HCLK_SDMMC:
	case SCLK_SDMMC:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO:
	case SCLK_SDIO_SAMPLE:
		rate = rk322x_mmc_get_clk(priv, clk->id);
		break;
	case SCLK_SPI0:
		rate = rk322x_spi_get_clk(priv);
		break;
	case ACLK_CPU:
	case HCLK_CPU:
	case PCLK_CPU:
	case PCLK_I2C0:
	case PCLK_I2C1:
	case PCLK_I2C2:
	case PCLK_I2C3:
	case PCLK_PWM:
		rate = rk322x_bus_get_clk(priv, clk->id);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		rate = rk322x_peri_get_clk(priv, clk->id);
		break;
#ifndef CONFIG_SPL_BUILD
	case DCLK_VOP:
	case ACLK_VOP:
		rate = rk322x_vop_get_clk(priv, clk->id);
		break;
	case SCLK_CRYPTO:
		rate = rk322x_crypto_get_clk(priv, clk->id);
		break;
#endif
	default:
		return -ENOENT;
	}

	return rate;
}

static ulong rk322x_clk_set_rate(struct clk *clk, ulong rate)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);
	ulong ret = 0;

	switch (clk->id) {
	case PLL_APLL:
	case PLL_DPLL:
		ret = rockchip_pll_set_rate(&rk322x_pll_clks[clk->id - 1],
					    priv->cru, clk->id - 1, rate);
		break;
	case PLL_CPLL:
		ret = rockchip_pll_set_rate(&rk322x_pll_clks[CPLL],
					    priv->cru, CPLL, rate);
		priv->cpll_hz = rate;
		break;
	case PLL_GPLL:
		ret = rockchip_pll_set_rate(&rk322x_pll_clks[GPLL],
					    priv->cru, GPLL, rate);
		priv->gpll_hz = rate;
		break;
	case ARMCLK:
		if (priv->armclk_hz)
			ret = rk322x_armclk_set_clk(priv, rate);
		priv->armclk_hz = rate;
		break;
	case HCLK_EMMC:
	case SCLK_EMMC:
	case SCLK_EMMC_SAMPLE:
	case HCLK_SDMMC:
	case SCLK_SDMMC:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO:
	case SCLK_SDIO_SAMPLE:
		ret = rk322x_mmc_set_clk(priv, clk->id, rate);
		break;
	case SCLK_DDRC:
		ret = rockchip_pll_set_rate(&rk322x_pll_clks[DPLL],
					    priv->cru, DPLL, rate);
		break;
	case SCLK_SPI0:
		rate = rk322x_spi_set_clk(priv, rate);
		break;
	case ACLK_CPU:
	case HCLK_CPU:
	case PCLK_CPU:
		ret = rk322x_bus_set_clk(priv, clk->id, rate);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		ret = rk322x_peri_set_clk(priv, clk->id, rate);
		break;
#ifndef CONFIG_SPL_BUILD
	case SCLK_MAC_SRC:
	case SCLK_MAC:
		ret = rk322x_mac_set_clk(priv, rate);
		break;
	case DCLK_VOP:
	case ACLK_VOP:
		ret = rk322x_vop_set_clk(priv, clk->id, rate);
		break;
	case SCLK_CRYPTO:
		ret = rk322x_crypto_set_clk(priv, clk->id, rate);
		break;
#endif
	default:
		return -ENOENT;
	}

	return ret;
}

#ifndef CONFIG_SPL_BUILD
static int rk322x_gmac_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk322x_cru *cru = priv->cru;

	/*
	 * If the requested parent is in the same clock-controller and the id
	 * is SCLK_MAC_SRC ("sclk_gmac_src"), switch to the internal clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC_SRC)) {
		debug("%s: switching RGMII to SCLK_MAC_SRC\n", __func__);
		rk_clrsetreg(&cru->cru_clksel_con[5], BIT(5), 0);
		return 0;
	}

	/*
	 * If the requested parent is in the same clock-controller and the id
	 * is SCLK_MAC_EXTCLK (sclk_mac_extclk), switch to the external clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC_EXTCLK)) {
		debug("%s: switching RGMII to SCLK_MAC_EXTCLK\n", __func__);
		rk_clrsetreg(&cru->cru_clksel_con[5], BIT(5), BIT(5));
		return 0;
	}

	return -EINVAL;
}

static int rk322x_gmac_extclk_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);
	const char *clock_output_name;
	struct rk322x_cru *cru = priv->cru;
	int ret;

	ret = dev_read_string_index(parent->dev, "clock-output-names",
				    parent->id, &clock_output_name);
	if (ret < 0)
		return -ENODATA;

	if (!strcmp(clock_output_name, "ext_gmac")) {
		debug("%s: switching gmac extclk to ext_gmac\n", __func__);
		rk_clrsetreg(&cru->cru_clksel_con[29], BIT(10), 0);
		return 0;
	} else if (!strcmp(clock_output_name, "phy_50m_out")) {
		debug("%s: switching gmac extclk to phy_50m_out\n", __func__);
		rk_clrsetreg(&cru->cru_clksel_con[29], BIT(10), BIT(10));
		return 0;
	}

	return -EINVAL;
}

static int rk322x_lcdc_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);

	if (parent->id == HDMIPHY)
		rk_clrsetreg(&priv->cru->cru_clksel_con[27],
			     DCLK_LCDC_SEL_MASK,
			     DCLK_LCDC_SEL_HDMIPHY << DCLK_LCDC_SEL_SHIFT);
	else if (parent->id == PLL_CPLL)
		rk_clrsetreg(&priv->cru->cru_clksel_con[27],
			     DCLK_LCDC_SEL_MASK | DCLK_LCDC_PLL_SEL_MASK,
			     (DCLK_LCDC_SEL_PLL << DCLK_LCDC_SEL_SHIFT) |
			     (DCLK_LCDC_PLL_SEL_CPLL <<
			     DCLK_LCDC_PLL_SEL_SHIFT));
	else
		rk_clrsetreg(&priv->cru->cru_clksel_con[27],
			     DCLK_LCDC_SEL_MASK | DCLK_LCDC_PLL_SEL_MASK,
			     (DCLK_LCDC_SEL_PLL << DCLK_LCDC_SEL_SHIFT) |
			     (DCLK_LCDC_PLL_SEL_GPLL <<
			     DCLK_LCDC_PLL_SEL_SHIFT));

	return 0;
}
#endif

static int rk322x_clk_set_parent(struct clk *clk, struct clk *parent)
{
	switch (clk->id) {
#ifndef CONFIG_SPL_BUILD
	case SCLK_MAC:
		return rk322x_gmac_set_parent(clk, parent);
	case SCLK_MAC_EXTCLK:
		return rk322x_gmac_extclk_set_parent(clk, parent);
	case DCLK_VOP:
		return rk322x_lcdc_set_parent(clk, parent);
#endif
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

int rk322x_mmc_get_phase(struct clk *clk)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk322x_cru *cru = priv->cru;
	u32 raw_value, delay_num;
	u16 degrees = 0;
	ulong rate;

	rate = rk322x_clk_get_rate(clk);

	if (rate < 0)
		return rate;

	if (clk->id == SCLK_EMMC_SAMPLE)
		raw_value = readl(&cru->cru_emmc_con[1]);
	else if (clk->id == SCLK_SDMMC_SAMPLE)
		raw_value = readl(&cru->cru_sdmmc_con[1]);
	else
		raw_value = readl(&cru->cru_sdio_con[1]);

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

int rk322x_mmc_set_phase(struct clk *clk, u32 degrees)
{
	struct rk322x_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk322x_cru *cru = priv->cru;
	u8 nineties, remainder, delay_num;
	u32 raw_value, delay;
	ulong rate;

	rate = rk322x_clk_get_rate(clk);

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
		writel(raw_value | 0xffff0000, &cru->cru_emmc_con[1]);
	else if (clk->id == SCLK_SDMMC_SAMPLE)
		writel(raw_value | 0xffff0000, &cru->cru_sdmmc_con[1]);
	else
		writel(raw_value | 0xffff0000, &cru->cru_sdio_con[1]);

	debug("mmc set_phase(%d) delay_nums=%u reg=%#x actual_degrees=%d\n",
	      degrees, delay_num, raw_value, rk322x_mmc_get_phase(clk));

	return 0;
}

static int rk322x_clk_get_phase(struct clk *clk)
{
	int ret;

	debug("%s %ld\n", __func__, clk->id);
	switch (clk->id) {
	case SCLK_EMMC_SAMPLE:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO_SAMPLE:
		ret = rk322x_mmc_get_phase(clk);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static int rk322x_clk_set_phase(struct clk *clk, int degrees)
{
	int ret;

	debug("%s %ld\n", __func__, clk->id);
	switch (clk->id) {
	case SCLK_EMMC_SAMPLE:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO_SAMPLE:
		ret = rk322x_mmc_set_phase(clk, degrees);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static struct clk_ops rk322x_clk_ops = {
	.get_rate	= rk322x_clk_get_rate,
	.set_rate	= rk322x_clk_set_rate,
	.set_parent	= rk322x_clk_set_parent,
	.get_phase	= rk322x_clk_get_phase,
	.set_phase	= rk322x_clk_set_phase,
};

static int rk322x_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct rk322x_clk_priv *priv = dev_get_priv(dev);

	priv->cru = dev_read_addr_ptr(dev);

	return 0;
}

#ifndef CONFIG_TPL_BUILD
static void rkclk_init(struct rk322x_clk_priv *priv)
{
	struct rk322x_cru *cru = priv->cru;

	if (rockchip_pll_get_rate(&rk322x_pll_clks[APLL],
				  priv->cru, APLL) != APLL_HZ)
		rk322x_armclk_set_clk(priv, APLL_HZ);

	priv->gpll_hz = rockchip_pll_get_rate(&rk322x_pll_clks[GPLL],
					      priv->cru, GPLL);
	priv->cpll_hz = rockchip_pll_get_rate(&rk322x_pll_clks[CPLL],
					      priv->cru, CPLL);

	/* before set pll set child div first */
	rk322x_bus_set_clk(priv, ACLK_CPU, ACLK_BUS_HZ / 4);
	rk322x_peri_set_clk(priv, ACLK_PERI, ACLK_PERI_HZ / 4);
	rk322x_mmc_set_clk(priv, SCLK_EMMC, 50000000);
	rk322x_mmc_set_clk(priv, SCLK_SDMMC, 50000000);
	rk322x_mmc_set_clk(priv, SCLK_SDIO, 50000000);
	rk_clrsetreg(&cru->cru_clksel_con[2], (0x1 << 14) |
		     (0x1f << 8), (1 << 14) | (0xb << 8));
	rk_clrsetreg(&cru->cru_clksel_con[23], (0x1f << 0) | (0x1f << 8),
		     (0x1f << 0) | (5 << 8));
	rk_clrsetreg(&cru->cru_clksel_con[33],
		     ACLK_VOP_PLL_SEL_MASK | ACLK_VOP_DIV_CON_MASK,
		     ACLK_VOP_PLL_SEL_GPLL << ACLK_VOP_PLL_SEL_SHIFT |
		     3 << ACLK_VOP_DIV_CON_SHIFT);
	rk_clrsetreg(&cru->cru_clksel_con[22], 0x1f << 0, 5 << 0);
	rk_clrsetreg(&cru->cru_clksel_con[24], 0x1f << 0, 0xb << 0);
	rk_clrsetreg(&cru->cru_clksel_con[28], (0x1f << 8) | (0x1f << 0),
		     (5 << 8) | (5 << 0));
	rk_clrsetreg(&cru->cru_clksel_con[31], (0x1f << 8) | (0x1f << 0),
		     (5 << 8) | (5 << 0));
	rk_clrsetreg(&cru->cru_clksel_con[32], 0x1f << 0, 5 << 0);
	rk_clrsetreg(&cru->cru_clksel_con[33], (0x1f << 8) | (0x1f << 0),
		     (5 << 8) | (5 << 0));
	rk_clrsetreg(&cru->cru_clksel_con[34], (0x1f << 8) | (0x1f << 0),
		     (5 << 8) | (3 << 0));

	rockchip_pll_set_rate(&rk322x_pll_clks[GPLL],
			      priv->cru, GPLL, GPLL_HZ);
	priv->gpll_hz = GPLL_HZ;

	rockchip_pll_set_rate(&rk322x_pll_clks[CPLL],
			      priv->cru, CPLL, CPLL_HZ);
	priv->cpll_hz = CPLL_HZ;

	rk322x_bus_set_clk(priv, ACLK_CPU, ACLK_BUS_HZ);
	rk322x_bus_set_clk(priv, HCLK_CPU, ACLK_BUS_HZ / 2);
	rk322x_bus_set_clk(priv, PCLK_CPU, ACLK_BUS_HZ / 2);
	rk322x_peri_set_clk(priv, ACLK_PERI, ACLK_PERI_HZ);
	rk322x_peri_set_clk(priv, HCLK_PERI, ACLK_PERI_HZ / 2);
	rk322x_peri_set_clk(priv, PCLK_PERI, ACLK_PERI_HZ / 2);
	/*rk322x_mmc_set_clk(priv, SCLK_EMMC, rate);*/

	/* set usbphy and hdmiphy from phy */
	rk_clrsetreg(&cru->cru_misc_con, (0x1 << 13) |
		     (0x1 << 15), (0 << 15) | (0 << 13));
}
#endif

static int rk322x_clk_probe(struct udevice *dev)
{
#ifndef CONFIG_TPL_BUILD
	struct rk322x_clk_priv *priv = dev_get_priv(dev);
	int ret = 0;

	priv->sync_kernel = false;
	if (!priv->armclk_enter_hz)
		priv->armclk_enter_hz =
		rockchip_pll_get_rate(&rk322x_pll_clks[APLL],
				      priv->cru, APLL);
	rkclk_init(priv);
	if (!priv->armclk_init_hz)
		priv->armclk_init_hz =
		rockchip_pll_get_rate(&rk322x_pll_clks[APLL],
				      priv->cru, APLL);
	ret = clk_set_defaults(dev);
	if (ret)
		debug("%s clk_set_defaults failed %d\n", __func__, ret);
	else
		priv->sync_kernel = true;
#endif
	return 0;
}

static int rk322x_clk_bind(struct udevice *dev)
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
		priv->glb_srst_fst_value = offsetof(struct rk322x_cru,
						    cru_glb_srst_fst_value);
		priv->glb_srst_snd_value = offsetof(struct rk322x_cru,
						    cru_glb_srst_snd_value);
		sys_child->priv = priv;
	}

	ret = device_bind_driver_to_node(dev, "rockchip_reset", "reset",
					 dev_ofnode(dev), &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk322x_cru,
						    cru_softrst_con[0]);
		sf_priv->sf_reset_num = 9;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk322x_clk_ids[] = {
	{ .compatible = "rockchip,rk3228-cru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk322x_cru) = {
	.name		= "clk_rk322x",
	.id		= UCLASS_CLK,
	.of_match	= rk322x_clk_ids,
	.priv_auto_alloc_size = sizeof(struct rk322x_clk_priv),
	.ofdata_to_platdata = rk322x_clk_ofdata_to_platdata,
	.ops		= &rk322x_clk_ops,
	.bind		= rk322x_clk_bind,
	.probe		= rk322x_clk_probe,
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
	struct rk322x_clk_priv *priv;
	const struct rk322x_clk_info *clk_dump;
	struct clk clk;
	unsigned long clk_count = ARRAY_SIZE(clks_dump);
	unsigned long rate;
	int i, ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(rockchip_rk322x_cru),
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

