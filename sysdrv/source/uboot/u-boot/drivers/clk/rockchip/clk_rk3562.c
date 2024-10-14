// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd
 * Author: Finley Xiao <finley.xiao@rock-chips.com>
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/clock.h>
#include <asm/arch/cru_rk3562.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rk3562-cru.h>

DECLARE_GLOBAL_DATA_PTR;

#define DIV_TO_RATE(input_rate, div)    ((input_rate) / ((div) + 1))

static struct rockchip_pll_rate_table rk3562_pll_rates[] = {
	/* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac */
	RK3036_PLL_RATE(1608000000, 1, 67, 1, 1, 1, 0),
	RK3036_PLL_RATE(1416000000, 1, 118, 2, 1, 1, 0),
	RK3036_PLL_RATE(1296000000, 1, 108, 2, 1, 1, 0),
	RK3036_PLL_RATE(1200000000, 1, 100, 2, 1, 1, 0),
	RK3036_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
	RK3036_PLL_RATE(1104000000, 1, 92, 2, 1, 1, 0),
	RK3036_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
	RK3036_PLL_RATE(1000000000, 3, 250, 2, 1, 1, 0),
	RK3036_PLL_RATE(912000000, 1, 76, 2, 1, 1, 0),
	RK3036_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
	RK3036_PLL_RATE(705600000, 2, 235, 4, 1, 0, 3355443),
	RK3036_PLL_RATE(611520000, 4, 611, 6, 1, 0, 8724152),
	RK3036_PLL_RATE(600000000, 1, 100, 4, 1, 1, 0),
	RK3036_PLL_RATE(594000000, 1, 99, 4, 1, 1, 0),
	RK3036_PLL_RATE(500000000, 1, 125, 6, 1, 1, 0),
	RK3036_PLL_RATE(408000000, 1, 68, 2, 2, 1, 0),
	RK3036_PLL_RATE(312000000, 1, 78, 6, 1, 1, 0),
	RK3036_PLL_RATE(216000000, 1, 72, 4, 2, 1, 0),
	RK3036_PLL_RATE(96000000, 1, 96, 6, 4, 1, 0),
	{ /* sentinel */ },
};

static struct rockchip_pll_clock rk3562_pll_clks[] = {
	[APLL] = PLL(pll_rk3328, PLL_APLL, RK3562_PLL_CON(0),
		     RK3562_MODE_CON, 0, 10, 0, rk3562_pll_rates),
	[GPLL] = PLL(pll_rk3328, PLL_GPLL, RK3562_PLL_CON(24),
		     RK3562_MODE_CON, 2, 10, 0, rk3562_pll_rates),
	[VPLL] = PLL(pll_rk3328, PLL_VPLL, RK3562_PLL_CON(32),
		     RK3562_MODE_CON, 6, 10, 0, rk3562_pll_rates),
	[HPLL] = PLL(pll_rk3328, PLL_HPLL, RK3562_PLL_CON(40),
		     RK3562_MODE_CON, 8, 10, 0, rk3562_pll_rates),
	[CPLL] = PLL(pll_rk3328, PLL_CPLL, RK3562_PMU1_PLL_CON(0),
		     RK3562_PMU1_MODE_CON, 0, 10, 0, rk3562_pll_rates),
	[DPLL] = PLL(pll_rk3328, PLL_DPLL, RK3562_SUBDDR_PLL_CON(0),
		     RK3562_SUBDDR_MODE_CON, 0, 10, 0, NULL),
};

#define RK3562_CPUCLK_RATE(_rate, _aclk_m_core, _pclk_dbg)	\
{								\
	.rate = _rate##U,					\
	.aclk_div = _aclk_m_core,				\
	.pclk_div = _pclk_dbg,					\
}

static struct rockchip_cpu_rate_table rk3562_cpu_rates[] = {
	RK3562_CPUCLK_RATE(1416000000, 1, 8),
	RK3562_CPUCLK_RATE(1296000000, 1, 8),
	RK3562_CPUCLK_RATE(1200000000, 1, 8),
	RK3562_CPUCLK_RATE(1104000000, 1, 8),
	RK3562_CPUCLK_RATE(1008000000, 1, 8),
	RK3562_CPUCLK_RATE(912000000, 1, 6),
	RK3562_CPUCLK_RATE(816000000, 1, 6),
	RK3562_CPUCLK_RATE(600000000, 1, 6),
	RK3562_CPUCLK_RATE(408000000, 1, 4),
	{ /* sentinel */ },
};

#ifndef CONFIG_SPL_BUILD
#define RK3562_CLK_DUMP(_id, _name)		\
{						\
	.id = _id,				\
	.name = _name,				\
}

static const struct rk3562_clk_info clks_dump[] = {
	RK3562_CLK_DUMP(PLL_APLL, "apll"),
	RK3562_CLK_DUMP(PLL_GPLL, "gpll"),
	RK3562_CLK_DUMP(PLL_VPLL, "vpll"),
	RK3562_CLK_DUMP(PLL_HPLL, "hpll"),
	RK3562_CLK_DUMP(PLL_CPLL, "cpll"),
	RK3562_CLK_DUMP(PLL_DPLL, "dpll"),
	RK3562_CLK_DUMP(ACLK_BUS, "aclk_bus"),
	RK3562_CLK_DUMP(HCLK_BUS, "hclk_bus"),
	RK3562_CLK_DUMP(PCLK_BUS, "pclk_bus"),
	RK3562_CLK_DUMP(ACLK_PERI, "aclk_peri"),
	RK3562_CLK_DUMP(HCLK_PERI, "hclk_peri"),
	RK3562_CLK_DUMP(PCLK_PERI, "pclk_peri"),
};
#endif

/*
 *
 * rational_best_approximation(31415, 10000,
 *		(1 << 8) - 1, (1 << 5) - 1, &n, &d);
 *
 * you may look at given_numerator as a fixed point number,
 * with the fractional part size described in given_denominator.
 *
 * for theoretical background, see:
 * http://en.wikipedia.org/wiki/Continued_fraction
 */
static void rational_best_approximation(unsigned long given_numerator,
					unsigned long given_denominator,
					unsigned long max_numerator,
					unsigned long max_denominator,
					unsigned long *best_numerator,
					unsigned long *best_denominator)
{
	unsigned long n, d, n0, d0, n1, d1;

	n = given_numerator;
	d = given_denominator;
	n0 = 0;
	d1 = 0;
	n1 = 1;
	d0 = 1;
	for (;;) {
		unsigned long t, a;

		if (n1 > max_numerator || d1 > max_denominator) {
			n1 = n0;
			d1 = d0;
			break;
		}
		if (d == 0)
			break;
		t = d;
		a = n / d;
		d = n % d;
		n = t;
		t = n0 + a * n1;
		n0 = n1;
		n1 = t;
		t = d0 + a * d1;
		d0 = d1;
		d1 = t;
	}
	*best_numerator = n1;
	*best_denominator = d1;
}

static int rk3562_armclk_set_rate(struct rk3562_clk_priv *priv, ulong new_rate)
{
	const struct rockchip_cpu_rate_table *rate;
	struct rk3562_cru *cru = priv->cru;
	ulong old_rate;

	rate = rockchip_get_cpu_settings(rk3562_cpu_rates, new_rate);
	if (!rate) {
		printf("%s unsupported rate\n", __func__);
		return -EINVAL;
	}

	/*
	 * set up dependent divisors for DBG and ACLK clocks.
	 */
	old_rate = rockchip_pll_get_rate(&rk3562_pll_clks[APLL], priv->cru,
					 APLL);
	if (old_rate == new_rate) {
		rk_clrsetreg(&cru->clksel_con[11], ACLK_CORE_PRE_DIV_MASK,
			     rate->aclk_div << ACLK_CORE_PRE_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[12], PCLK_DBG_PRE_DIV_MASK,
			     rate->pclk_div << PCLK_DBG_PRE_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[10], CLK_CORE_PRE_DIV_MASK, 0);
	} else if (old_rate > new_rate) {
		if (rockchip_pll_set_rate(&rk3562_pll_clks[APLL],
					  priv->cru, APLL, new_rate))
			return -EINVAL;
		rk_clrsetreg(&cru->clksel_con[11], ACLK_CORE_PRE_DIV_MASK,
			     rate->aclk_div << ACLK_CORE_PRE_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[12], PCLK_DBG_PRE_DIV_MASK,
			     rate->pclk_div << PCLK_DBG_PRE_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[10], CLK_CORE_PRE_DIV_MASK, 0);
	} else if (old_rate < new_rate) {
		rk_clrsetreg(&cru->clksel_con[11], ACLK_CORE_PRE_DIV_MASK,
			     rate->aclk_div << ACLK_CORE_PRE_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[12], PCLK_DBG_PRE_DIV_MASK,
			     rate->pclk_div << PCLK_DBG_PRE_DIV_SHIFT);
		rk_clrsetreg(&cru->clksel_con[10], CLK_CORE_PRE_DIV_MASK, 0);

		if (rockchip_pll_set_rate(&rk3562_pll_clks[APLL],
					  priv->cru, APLL, new_rate))
			return -EINVAL;
	}

	return 0;
}

static ulong rk3562_bus_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, con, div;
	ulong rate;

	switch (clk_id) {
	case ACLK_BUS:
		con = readl(&cru->clksel_con[40]);
		sel = (con & ACLK_BUS_SEL_MASK) >> ACLK_BUS_SEL_SHIFT;
		div = (con & ACLK_BUS_DIV_MASK) >> ACLK_BUS_DIV_SHIFT;
		break;
	case HCLK_BUS:
		con = readl(&cru->clksel_con[40]);
		sel = (con & HCLK_BUS_SEL_MASK) >> HCLK_BUS_SEL_SHIFT;
		div = (con & HCLK_BUS_DIV_MASK) >> HCLK_BUS_DIV_SHIFT;
		break;
	case PCLK_BUS:
		con = readl(&cru->clksel_con[41]);
		sel = (con & PCLK_BUS_SEL_MASK) >> PCLK_BUS_SEL_SHIFT;
		div = (con & PCLK_BUS_DIV_MASK) >> PCLK_BUS_DIV_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	if (sel == ACLK_BUS_SEL_CPLL)
		rate = priv->cpll_hz;
	else
		rate = priv->gpll_hz;

	return DIV_TO_RATE(rate, div);
}

static ulong rk3562_bus_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, div;

	if (priv->cpll_hz % rate == 0) {
		sel = ACLK_BUS_SEL_CPLL;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else {
		sel= ACLK_BUS_SEL_GPLL;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	}

	switch (clk_id) {
	case ACLK_BUS:
		rk_clrsetreg(&cru->clksel_con[40],
			     ACLK_BUS_SEL_MASK | ACLK_BUS_DIV_MASK,
			     (sel << ACLK_BUS_SEL_SHIFT) |
			     ((div - 1) << ACLK_BUS_DIV_SHIFT));
		break;
	case HCLK_BUS:
		rk_clrsetreg(&cru->clksel_con[40],
			     HCLK_BUS_SEL_MASK | HCLK_BUS_DIV_MASK,
			     (sel << HCLK_BUS_SEL_SHIFT) |
			     ((div - 1) << HCLK_BUS_DIV_SHIFT));
		break;
	case PCLK_BUS:
		rk_clrsetreg(&cru->clksel_con[41],
			     PCLK_BUS_SEL_MASK | PCLK_BUS_DIV_MASK,
			     (sel << PCLK_BUS_SEL_SHIFT) |
			     ((div - 1) << PCLK_BUS_DIV_SHIFT));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_bus_get_rate(priv, clk_id);
}

static ulong rk3562_peri_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, con, div;
	ulong rate;

	switch (clk_id) {
	case ACLK_PERI:
		con = readl(&cru->periclksel_con[0]);
		sel = (con & ACLK_PERI_SEL_MASK) >> ACLK_PERI_SEL_SHIFT;
		div = (con & ACLK_PERI_DIV_MASK) >> ACLK_PERI_DIV_SHIFT;
		break;
	case HCLK_PERI:
		con = readl(&cru->periclksel_con[0]);
		sel = (con & HCLK_PERI_SEL_MASK) >> HCLK_PERI_SEL_SHIFT;
		div = (con & HCLK_PERI_DIV_MASK) >> HCLK_PERI_DIV_SHIFT;
		break;
	case PCLK_PERI:
		con = readl(&cru->periclksel_con[1]);
		sel = (con & PCLK_PERI_SEL_MASK) >> PCLK_PERI_SEL_SHIFT;
		div = (con & PCLK_PERI_DIV_MASK) >> PCLK_PERI_DIV_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	if (sel == ACLK_PERI_SEL_CPLL)
		rate = priv->cpll_hz;
	else
		rate = priv->gpll_hz;

	return DIV_TO_RATE(rate, div);
}

static ulong rk3562_peri_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, div;

	if (priv->cpll_hz % rate == 0) {
		sel = ACLK_PERI_SEL_CPLL;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else {
		sel= ACLK_PERI_SEL_GPLL;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	}

	switch (clk_id) {
	case ACLK_PERI:
		rk_clrsetreg(&cru->periclksel_con[0],
			     ACLK_PERI_SEL_MASK | ACLK_PERI_DIV_MASK,
			     (sel << ACLK_PERI_SEL_SHIFT) |
			     ((div - 1) << ACLK_PERI_DIV_SHIFT));
		break;
	case HCLK_PERI:
		rk_clrsetreg(&cru->periclksel_con[0],
			     HCLK_PERI_SEL_MASK | HCLK_PERI_DIV_MASK,
			     (sel << HCLK_PERI_SEL_SHIFT) |
			     ((div - 1) << HCLK_PERI_DIV_SHIFT));
		break;
	case PCLK_PERI:
		rk_clrsetreg(&cru->periclksel_con[1],
			     PCLK_PERI_SEL_MASK | PCLK_PERI_DIV_MASK,
			     (sel << PCLK_PERI_SEL_SHIFT) |
			     ((div - 1) << PCLK_PERI_DIV_SHIFT));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_peri_get_rate(priv, clk_id);
}

static ulong rk3562_i2c_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, con, div;
	ulong rate;

	switch (clk_id) {
	case CLK_PMU0_I2C0:
		con = readl(&cru->pmu0clksel_con[3]);
		sel = (con & CLK_PMU0_I2C0_SEL_MASK) >> CLK_PMU0_I2C0_SEL_SHIFT;
		if (sel == CLK_PMU0_I2C0_SEL_200M)
			rate = 200 * MHz;
		else if (sel == CLK_PMU0_I2C0_SEL_24M)
			rate = OSC_HZ;
		else
			rate = 32768;
		div = (con & CLK_PMU0_I2C0_DIV_MASK) >> CLK_PMU0_I2C0_DIV_SHIFT;

		return DIV_TO_RATE(rate, div);
	case CLK_I2C:
	case CLK_I2C1:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
		con = readl(&cru->clksel_con[41]);
		sel = (con & CLK_I2C_SEL_MASK) >> CLK_I2C_SEL_SHIFT;
		if (sel == CLK_I2C_SEL_200M)
			rate = 200 * MHz;
		else if (sel == CLK_I2C_SEL_100M)
			rate = 100 * MHz;
		else if (sel == CLK_I2C_SEL_50M)
			rate = 50 * MHz;
		else
			rate = OSC_HZ;
		break;
	default:
		return -ENOENT;
	}

	return rate;
}

static ulong rk3562_i2c_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, div;

	switch (clk_id) {
	case CLK_PMU0_I2C0:
		if (rate == 200 * MHz) {
			sel = CLK_PMU0_I2C0_SEL_200M;
			div = 1;
		} else if (rate == OSC_HZ) {
			sel = CLK_PMU0_I2C0_SEL_24M;
			div = 1;
		} else if (rate == 32768) {
			sel = CLK_PMU0_I2C0_SEL_32K;
			div = 1;
		} else {
			sel = CLK_PMU0_I2C0_SEL_200M;
			div = DIV_ROUND_UP(200 * MHz, rate);
			assert(div - 1 <= 31);
		}
		rk_clrsetreg(&cru->pmu0clksel_con[3], CLK_PMU0_I2C0_DIV_MASK,
			     (div - 1) << CLK_PMU0_I2C0_DIV_SHIFT);
		rk_clrsetreg(&cru->pmu0clksel_con[3], CLK_PMU0_I2C0_SEL_MASK,
			     sel << CLK_PMU0_I2C0_SEL_SHIFT);
		break;
	case CLK_I2C:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
		if (rate == 200 * MHz)
			sel = CLK_I2C_SEL_200M;
		else if (rate == 100 * MHz)
			sel = CLK_I2C_SEL_100M;
		else if (rate == 50 * MHz)
			sel = CLK_I2C_SEL_50M;
		else
			sel = CLK_I2C_SEL_24M;
		rk_clrsetreg(&cru->clksel_con[41], CLK_I2C_SEL_MASK,
			     sel << CLK_I2C_SEL_SHIFT);
		break;
	default:
		return -ENOENT;
	}


	return rk3562_i2c_get_rate(priv, clk_id);
}

static ulong rk3562_uart_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 reg, con, fracdiv, div, src, p_src, p_rate;
	unsigned long m, n;

	switch (clk_id) {
	case SCLK_PMU1_UART0:
		con = readl(&cru->pmu1clksel_con[2]);
		src = (con & CLK_PMU1_UART0_SEL_MASK) >>
		       CLK_PMU1_UART0_SEL_SHIFT;
		div = (con & CLK_PMU1_UART0_SRC_DIV_MASK) >>
		       CLK_PMU1_UART0_SRC_DIV_SHIFT;
		if (src == CLK_UART_SEL_SRC) {
			return DIV_TO_RATE(priv->cpll_hz, div);
		} else if (src == CLK_UART_SEL_FRAC) {
			fracdiv = readl(&cru->pmu1clksel_con[3]);
			n = fracdiv & CLK_UART_FRAC_NUMERATOR_MASK;
			n >>= CLK_UART_FRAC_NUMERATOR_SHIFT;
			m = fracdiv & CLK_UART_FRAC_DENOMINATOR_MASK;
			m >>= CLK_UART_FRAC_DENOMINATOR_SHIFT;
			return DIV_TO_RATE(priv->cpll_hz, div) * n / m;
		} else {
			return OSC_HZ;
		}
	case SCLK_UART1:
		reg = 21;
		break;
	case SCLK_UART2:
		reg = 23;
		break;
	case SCLK_UART3:
		reg = 25;
		break;
	case SCLK_UART4:
		reg = 27;
		break;
	case SCLK_UART5:
		reg = 29;
		break;
	case SCLK_UART6:
		reg = 31;
		break;
	case SCLK_UART7:
		reg = 33;
		break;
	case SCLK_UART8:
		reg = 35;
		break;
	case SCLK_UART9:
		reg = 37;
		break;
	default:
		return -ENOENT;
	}
	con = readl(&cru->periclksel_con[reg]);
	src = (con & CLK_UART_SEL_MASK) >> CLK_UART_SEL_SHIFT;
	div = (con & CLK_UART_SRC_DIV_MASK) >> CLK_UART_SRC_DIV_SHIFT;
	p_src = (con & CLK_UART_SRC_SEL_MASK) >> CLK_UART_SRC_SEL_SHIFT;
	if (p_src == CLK_UART_SRC_SEL_GPLL)
		p_rate = priv->gpll_hz;
	else
		p_rate = priv->cpll_hz;
	if (src == CLK_UART_SEL_SRC) {
		return DIV_TO_RATE(p_rate, div);
	} else if (src == CLK_UART_SEL_FRAC) {
		fracdiv = readl(&cru->periclksel_con[reg + 1]);
		n = fracdiv & CLK_UART_FRAC_NUMERATOR_MASK;
		n >>= CLK_UART_FRAC_NUMERATOR_SHIFT;
		m = fracdiv & CLK_UART_FRAC_DENOMINATOR_MASK;
		m >>= CLK_UART_FRAC_DENOMINATOR_SHIFT;
		return DIV_TO_RATE(p_rate, div) * n / m;
	} else {
		return OSC_HZ;
	}
}

static ulong rk3562_uart_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 reg, clk_src, uart_src, div;
	unsigned long m = 0, n = 0, val;

	switch (clk_id) {
	case SCLK_PMU1_UART0:
		if (priv->cpll_hz % rate == 0) {
			uart_src = CLK_UART_SEL_SRC;
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
		} else if (rate == OSC_HZ) {
			uart_src = CLK_UART_SEL_XIN24M;
			div = 2;
		} else {
			uart_src = CLK_UART_SEL_FRAC;
			div = 2;
			rational_best_approximation(rate, priv->cpll_hz / div,
						    GENMASK(16 - 1, 0),
						    GENMASK(16 - 1, 0),
						    &n, &m);
		}

		rk_clrsetreg(&cru->pmu1clksel_con[2],
			     CLK_PMU1_UART0_SEL_MASK |
			     CLK_PMU1_UART0_SRC_DIV_MASK,
			     (uart_src << CLK_PMU1_UART0_SEL_SHIFT) |
			     ((div - 1) << CLK_PMU1_UART0_SRC_DIV_SHIFT));
		if (m && n) {
			val = n << CLK_UART_FRAC_NUMERATOR_SHIFT | m;
			writel(val, &cru->pmu1clksel_con[3]);
		}

		return rk3562_uart_get_rate(priv, clk_id);
	case SCLK_UART1:
		reg = 21;
		break;
	case SCLK_UART2:
		reg = 23;
		break;
	case SCLK_UART3:
		reg = 25;
		break;
	case SCLK_UART4:
		reg = 27;
		break;
	case SCLK_UART5:
		reg = 29;
		break;
	case SCLK_UART6:
		reg = 31;
		break;
	case SCLK_UART7:
		reg = 33;
		break;
	case SCLK_UART8:
		reg = 35;
		break;
	case SCLK_UART9:
		reg = 37;
		break;
	default:
		return -ENOENT;
	}

	if (priv->gpll_hz % rate == 0) {
		clk_src = CLK_UART_SRC_SEL_GPLL;
		uart_src = CLK_UART_SEL_SRC;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	} else if (priv->cpll_hz % rate == 0) {
		clk_src = CLK_UART_SRC_SEL_CPLL;
		uart_src = CLK_UART_SEL_SRC;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else if (rate == OSC_HZ) {
		clk_src = CLK_UART_SRC_SEL_GPLL;
		uart_src = CLK_UART_SEL_XIN24M;
		div = 2;
	} else {
		clk_src = CLK_UART_SRC_SEL_GPLL;
		uart_src = CLK_UART_SEL_FRAC;
		div = 2;
		rational_best_approximation(rate, priv->gpll_hz / div,
					    GENMASK(16 - 1, 0),
					    GENMASK(16 - 1, 0),
					    &n, &m);
	}

	rk_clrsetreg(&cru->periclksel_con[reg],
		     CLK_UART_SEL_MASK | CLK_UART_SRC_SEL_MASK |
		     CLK_UART_SRC_DIV_MASK,
		     (clk_src << CLK_UART_SRC_SEL_SHIFT) |
		     (uart_src << CLK_UART_SEL_SHIFT) |
		     ((div - 1) << CLK_UART_SRC_DIV_SHIFT));
	if (m && n) {
		val = n << CLK_UART_FRAC_NUMERATOR_SHIFT | m;
		writel(val, &cru->periclksel_con[reg + 1]);
	}

	return rk3562_uart_get_rate(priv, clk_id);
}

static ulong rk3562_pwm_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, con, div, mask, shift;
	ulong rate;

	switch (clk_id) {
	case CLK_PMU1_PWM0:
		con = readl(&cru->pmu1clksel_con[4]);
		sel = (con & CLK_PMU1_PWM0_SEL_MASK) >> CLK_PMU1_PWM0_SEL_SHIFT;
		if (sel == CLK_PMU1_PWM0_SEL_200M)
			rate = 200 * MHz;
		else if (sel == CLK_PMU1_PWM0_SEL_24M)
			rate = OSC_HZ;
		else
			rate = 32768;
		div = (con & CLK_PMU1_PWM0_DIV_MASK) >> CLK_PMU1_PWM0_DIV_SHIFT;

		return DIV_TO_RATE(rate, div);
	case CLK_PWM1_PERI:
		mask = CLK_PWM1_PERI_SEL_MASK;
		shift = CLK_PWM1_PERI_SEL_SHIFT;
		break;
	case CLK_PWM2_PERI:
		mask = CLK_PWM2_PERI_SEL_MASK;
		shift = CLK_PWM2_PERI_SEL_SHIFT;
		break;
	case CLK_PWM3_PERI:
		mask = CLK_PWM3_PERI_SEL_MASK;
		shift = CLK_PWM3_PERI_SEL_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	con = readl(&cru->periclksel_con[40]);
	sel = (con & mask) >> shift;
	if (sel == CLK_PWM_SEL_100M)
		rate = 100 * MHz;
	else if (sel == CLK_PWM_SEL_50M)
		rate = 50 * MHz;
	else
		rate = OSC_HZ;

	return rate;
}

static ulong rk3562_pwm_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, div, mask, shift;

	switch (clk_id) {
	case CLK_PMU1_PWM0:
		if (rate == 200 * MHz) {
			sel = CLK_PMU1_PWM0_SEL_200M;
			div = 1;
		} else if (rate == OSC_HZ) {
			sel = CLK_PMU1_PWM0_SEL_24M;
			div = 1;
		} else if (rate == 32768) {
			sel = CLK_PMU1_PWM0_SEL_32K;
			div = 1;
		} else {
			sel = CLK_PMU1_PWM0_SEL_200M;
			div = DIV_ROUND_UP(200 * MHz, rate);
			assert(div - 1 <= 3);
		}
		rk_clrsetreg(&cru->pmu1clksel_con[4], CLK_PMU1_PWM0_DIV_MASK,
			     (div - 1) << CLK_PMU1_PWM0_DIV_SHIFT);
		rk_clrsetreg(&cru->pmu1clksel_con[4], CLK_PMU1_PWM0_SEL_MASK,
			     sel << CLK_PMU1_PWM0_SEL_SHIFT);

		return rk3562_pwm_get_rate(priv, clk_id);
	case CLK_PWM1_PERI:
		mask = CLK_PWM1_PERI_SEL_MASK;
		shift = CLK_PWM1_PERI_SEL_SHIFT;
		break;
	case CLK_PWM2_PERI:
		mask = CLK_PWM2_PERI_SEL_MASK;
		shift = CLK_PWM2_PERI_SEL_SHIFT;
		break;
	case CLK_PWM3_PERI:
		mask = CLK_PWM3_PERI_SEL_MASK;
		shift = CLK_PWM3_PERI_SEL_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	if (rate == 100 * MHz)
		sel = CLK_PWM_SEL_100M;
	else if (rate == 50 * MHz)
		sel = CLK_PWM_SEL_50M;
	else
		sel = CLK_PWM_SEL_24M;
	rk_clrsetreg(&cru->periclksel_con[40], mask, sel << shift);

	return rk3562_pwm_get_rate(priv, clk_id);
}

static ulong rk3562_spi_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, con, div, mask, shift;
	ulong rate;

	switch (clk_id) {
	case CLK_PMU1_SPI0:
		con = readl(&cru->pmu1clksel_con[4]);
		sel = (con & CLK_PMU1_SPI0_SEL_MASK) >> CLK_PMU1_SPI0_SEL_SHIFT;
		if (sel == CLK_PMU1_SPI0_SEL_200M)
			rate = 200 * MHz;
		else if (sel == CLK_PMU1_SPI0_SEL_24M)
			rate = OSC_HZ;
		else
			rate = 32768;
		div = (con & CLK_PMU1_SPI0_DIV_MASK) >> CLK_PMU1_SPI0_DIV_SHIFT;

		return DIV_TO_RATE(rate, div);
	case CLK_SPI1:
		mask = CLK_SPI1_SEL_MASK;
		shift = CLK_SPI1_SEL_SHIFT;
		break;
	case CLK_SPI2:
		mask = CLK_SPI2_SEL_MASK;
		shift = CLK_SPI2_SEL_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	con = readl(&cru->periclksel_con[20]);
	sel = (con & mask) >> shift;
	if (sel == CLK_SPI_SEL_200M)
		rate = 200 * MHz;
	else if (sel == CLK_SPI_SEL_100M)
		rate = 100 * MHz;
	else if (sel == CLK_SPI_SEL_50M)
		rate = 50 * MHz;
	else
		rate = OSC_HZ;

	return rate;
}

static ulong rk3562_spi_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, div, mask, shift;

	switch (clk_id) {
	case CLK_PMU1_SPI0:
		if (rate == 200 * MHz) {
			sel = CLK_PMU1_SPI0_SEL_200M;
			div = 1;
		} else if (rate == OSC_HZ) {
			sel = CLK_PMU1_SPI0_SEL_24M;
			div = 1;
		} else if (rate == 32768) {
			sel = CLK_PMU1_SPI0_SEL_32K;
			div = 1;
		} else {
			sel = CLK_PMU1_SPI0_SEL_200M;
			div = DIV_ROUND_UP(200 * MHz, rate);
			assert(div - 1 <= 3);
		}
		rk_clrsetreg(&cru->pmu1clksel_con[4], CLK_PMU1_SPI0_DIV_MASK,
			     (div - 1) << CLK_PMU1_SPI0_DIV_SHIFT);
		rk_clrsetreg(&cru->pmu1clksel_con[4], CLK_PMU1_SPI0_SEL_MASK,
			     sel << CLK_PMU1_SPI0_SEL_SHIFT);

		return rk3562_spi_get_rate(priv, clk_id);
	case CLK_SPI1:
		mask = CLK_SPI1_SEL_MASK;
		shift = CLK_SPI1_SEL_SHIFT;
		break;
	case CLK_SPI2:
		mask = CLK_SPI2_SEL_MASK;
		shift = CLK_SPI2_SEL_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	if (rate == 200 * MHz)
		sel = CLK_SPI_SEL_200M;
	else if (rate == 100 * MHz)
		sel = CLK_SPI_SEL_100M;
	else if (rate == 50 * MHz)
		sel = CLK_SPI_SEL_50M;
	else
		sel = CLK_SPI_SEL_24M;
	rk_clrsetreg(&cru->periclksel_con[20], mask, sel << shift);

	return rk3562_spi_get_rate(priv, clk_id);
}

static ulong rk3562_tsadc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, con;

	con = readl(&cru->clksel_con[43]);
	switch (clk_id) {
	case CLK_TSADC_TSEN:
		div = (con & CLK_TSADC_TSEN_DIV_MASK) >>
		       CLK_TSADC_TSEN_DIV_SHIFT;
		break;
	case CLK_TSADC:
		div = (con & CLK_TSADC_DIV_MASK) >> CLK_TSADC_DIV_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3562_tsadc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				   ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, mask, shift;

	switch (clk_id) {
	case CLK_TSADC_TSEN:
		mask = CLK_TSADC_TSEN_DIV_MASK;
		shift =	CLK_TSADC_TSEN_DIV_SHIFT;
		break;
	case CLK_TSADC:
		mask = CLK_TSADC_DIV_MASK;
		shift =	CLK_TSADC_DIV_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	div = DIV_ROUND_UP(OSC_HZ, rate);
	rk_clrsetreg(&cru->clksel_con[43], mask, (div - 1) << shift);

	return rk3562_tsadc_get_rate(priv, clk_id);
}

static ulong rk3562_saradc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, con;

	switch (clk_id) {
	case CLK_SARADC_VCCIO156:
		con = readl(&cru->clksel_con[44]);
		div = (con & CLK_SARADC_VCCIO156_DIV_MASK) >>
		       CLK_SARADC_VCCIO156_DIV_SHIFT;
		break;
	case CLK_SARADC:
		con = readl(&cru->periclksel_con[46]);
		div = (con & CLK_SARADC_DIV_MASK) >> CLK_SARADC_DIV_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3562_saradc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				    ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div;

	switch (clk_id) {
	case CLK_SARADC_VCCIO156:
		div = DIV_ROUND_UP(OSC_HZ, rate);
		rk_clrsetreg(&cru->clksel_con[44], CLK_SARADC_VCCIO156_DIV_MASK,
			     (div - 1) << CLK_SARADC_VCCIO156_DIV_SHIFT);
		break;
	case CLK_SARADC:
		div = DIV_ROUND_UP(OSC_HZ, rate);
		rk_clrsetreg(&cru->periclksel_con[46], CLK_SARADC_DIV_MASK,
			     (div - 1) << CLK_SARADC_DIV_SHIFT);
		break;
	default:
		return -ENOENT;
	}

	return rk3562_saradc_get_rate(priv, clk_id);
}

static ulong rk3562_sfc_get_rate(struct rk3562_clk_priv *priv)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, sel, con, parent;

	con = readl(&cru->periclksel_con[20]);
	div = (con & SCLK_SFC_DIV_MASK) >> SCLK_SFC_DIV_SHIFT;
	sel = (con & SCLK_SFC_SEL_MASK) >> SCLK_SFC_SEL_SHIFT;
	if (sel == SCLK_SFC_SRC_SEL_GPLL)
		parent = priv->gpll_hz;
	else if (sel == SCLK_SFC_SRC_SEL_CPLL)
		parent = priv->cpll_hz;
	else
		parent = OSC_HZ;

	return DIV_TO_RATE(parent, div);
}

static ulong rk3562_sfc_set_rate(struct rk3562_clk_priv *priv, ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	int div, sel;

	if (OSC_HZ % rate == 0) {
		div = DIV_ROUND_UP(OSC_HZ, rate);
		sel = SCLK_SFC_SRC_SEL_24M;
	} else if ((priv->cpll_hz % rate) == 0) {
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
		sel = SCLK_SFC_SRC_SEL_CPLL;
	} else {
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
		sel = SCLK_SFC_SRC_SEL_GPLL;
	}

	assert(div - 1 <= 255);
	rk_clrsetreg(&cru->periclksel_con[20],
		     SCLK_SFC_SEL_MASK | SCLK_SFC_DIV_MASK,
		     sel << SCLK_SFC_SEL_SHIFT |
		     (div - 1) << SCLK_SFC_DIV_SHIFT);

	return rk3562_sfc_get_rate(priv);
}

static ulong rk3562_emmc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, sel, con, parent;

	switch (clk_id) {
	case CCLK_EMMC:
		con = readl(&cru->periclksel_con[18]);
		div = (con & CCLK_EMMC_DIV_MASK) >> CCLK_EMMC_DIV_SHIFT;
		sel = (con & CCLK_EMMC_SEL_MASK) >> CCLK_EMMC_SEL_SHIFT;
		if (sel == CCLK_EMMC_SEL_GPLL)
			parent = priv->gpll_hz;
		else if (sel == CCLK_EMMC_SEL_CPLL)
			parent = priv->cpll_hz;
		else if (sel == CCLK_EMMC_SEL_HPLL)
			parent = priv->hpll_hz;
		else
			parent = OSC_HZ;
		break;
	case BCLK_EMMC:
		con = readl(&cru->periclksel_con[19]);
		div = (con & BCLK_EMMC_DIV_MASK) >> BCLK_EMMC_DIV_SHIFT;
		sel = (con & BCLK_EMMC_SEL_MASK) >> BCLK_EMMC_SEL_SHIFT;
		if (sel == BCLK_EMMC_SEL_GPLL)
			parent = priv->gpll_hz;
		else
			parent = priv->cpll_hz;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3562_emmc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	int div, sel;

	switch (clk_id) {
	case CCLK_EMMC:
		if (OSC_HZ % rate == 0) {
			div = DIV_ROUND_UP(OSC_HZ, rate);
			sel = CCLK_EMMC_SEL_24M;
		} else if ((priv->cpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
			sel = CCLK_EMMC_SEL_CPLL;
		} else if ((priv->hpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->hpll_hz, rate);
			sel = CCLK_EMMC_SEL_HPLL;
		} else {
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
			sel = CCLK_EMMC_SEL_GPLL;
		}
		rk_clrsetreg(&cru->periclksel_con[18],
			     CCLK_EMMC_SEL_MASK | CCLK_EMMC_DIV_MASK,
			     sel << CCLK_EMMC_SEL_SHIFT |
			     (div - 1) << CCLK_EMMC_DIV_SHIFT);
		break;
	case BCLK_EMMC:
		if ((priv->cpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
			sel = BCLK_EMMC_SEL_CPLL;
		} else {
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
			sel = BCLK_EMMC_SEL_GPLL;
		}
		rk_clrsetreg(&cru->periclksel_con[19],
			     BCLK_EMMC_SEL_MASK | BCLK_EMMC_DIV_MASK,
			     sel << BCLK_EMMC_SEL_SHIFT |
			     (div - 1) << BCLK_EMMC_DIV_SHIFT);
		break;
	default:
		return -ENOENT;
	}

	return rk3562_emmc_get_rate(priv, clk_id);
}

static ulong rk3562_sdmmc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, sel, con;
	ulong prate;

	switch (clk_id) {
	case HCLK_SDMMC0:
	case CCLK_SDMMC0:
	case SCLK_SDMMC0_SAMPLE:
		con = readl(&cru->periclksel_con[16]);
		div = (con & CCLK_SDMMC0_DIV_MASK) >> CCLK_SDMMC0_DIV_SHIFT;
		sel = (con & CCLK_SDMMC0_SEL_MASK) >> CCLK_SDMMC0_SEL_SHIFT;
		break;
	case HCLK_SDMMC1:
	case CCLK_SDMMC1:
	case SCLK_SDMMC1_SAMPLE:
		con = readl(&cru->periclksel_con[17]);
		div = (con & CCLK_SDMMC1_DIV_MASK) >> CCLK_SDMMC1_DIV_SHIFT;
		sel = (con & CCLK_SDMMC1_SEL_MASK) >> CCLK_SDMMC1_SEL_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	if (sel == CCLK_SDMMC_SEL_GPLL)
		prate = priv->gpll_hz;
	else if (sel == CCLK_SDMMC_SEL_CPLL)
		prate = priv->cpll_hz;
	else if (sel == CCLK_SDMMC_SEL_HPLL)
		prate = priv->hpll_hz;
	else
		prate = OSC_HZ;

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_sdmmc_set_rate(struct rk3562_clk_priv *priv,
				   ulong clk_id, ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 div, sel;

	if (OSC_HZ % rate == 0) {
		div = DIV_ROUND_UP(OSC_HZ, rate);
		sel = CCLK_SDMMC_SEL_24M;
	} else if ((priv->cpll_hz % rate) == 0) {
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
		sel = CCLK_SDMMC_SEL_CPLL;
	} else if ((priv->hpll_hz % rate) == 0) {
		div = DIV_ROUND_UP(priv->hpll_hz, rate);
		sel = CCLK_SDMMC_SEL_HPLL;
	} else {
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
		sel = CCLK_SDMMC_SEL_CPLL;
	}

	switch (clk_id) {
	case HCLK_SDMMC0:
	case CCLK_SDMMC0:
		rk_clrsetreg(&cru->periclksel_con[16],
			     CCLK_SDMMC0_SEL_MASK | CCLK_SDMMC0_DIV_MASK,
			     sel << CCLK_SDMMC0_SEL_SHIFT |
			     (div - 1) << CCLK_SDMMC0_DIV_SHIFT);
		break;
	case HCLK_SDMMC1:
	case CCLK_SDMMC1:
		rk_clrsetreg(&cru->periclksel_con[17],
			     CCLK_SDMMC1_SEL_MASK | CCLK_SDMMC1_DIV_MASK,
			     sel << CCLK_SDMMC1_SEL_SHIFT |
			     (div - 1) << CCLK_SDMMC1_DIV_SHIFT);
		break;
	default:
		return -ENOENT;
	}

	return rk3562_sdmmc_get_rate(priv, clk_id);
}

static ulong rk3562_vop_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 con, sel, div;
	ulong prate;

	switch (clk_id) {
	case ACLK_VOP:
		con = readl(&cru->clksel_con[28]);
		div = (con & ACLK_VOP_DIV_MASK) >> ACLK_VOP_DIV_SHIFT;
		sel = (con & ACLK_VOP_SEL_MASK) >> ACLK_VOP_SEL_SHIFT;
		if (sel == ACLK_VOP_SEL_GPLL)
			prate = priv->gpll_hz;
		else if (sel == ACLK_VOP_SEL_CPLL)
			prate = priv->cpll_hz;
		else if (sel == ACLK_VOP_SEL_HPLL)
			prate = priv->hpll_hz;
		else if (sel == ACLK_VOP_SEL_VPLL)
			prate = priv->vpll_hz;
		else
			return -ENOENT;

		return DIV_TO_RATE(prate, div);
	case DCLK_VOP:
		con = readl(&cru->clksel_con[30]);
		div = (con & DCLK_VOP_DIV_MASK) >> DCLK_VOP_DIV_SHIFT;
		sel = (con & DCLK_VOP_SEL_MASK) >> DCLK_VOP_SEL_SHIFT;
		if (sel == DCLK_VOP_SEL_VPLL)
			priv->vpll_hz =
				rockchip_pll_get_rate(&rk3562_pll_clks[VPLL],
						      priv->cru, VPLL);
		break;
	case DCLK_VOP1:
		con = readl(&cru->clksel_con[31]);
		div = (con & DCLK_VOP1_DIV_MASK) >> DCLK_VOP1_DIV_SHIFT;
		sel = (con & DCLK_VOP1_SEL_MASK) >> DCLK_VOP1_SEL_SHIFT;
		break;
	default:
		return -ENOENT;
	}

	if (sel == DCLK_VOP_SEL_GPLL)
		prate = priv->gpll_hz;
	else if (sel == DCLK_VOP_SEL_HPLL)
		prate = priv->hpll_hz;
	else if (sel == DCLK_VOP_SEL_VPLL)
		prate = priv->vpll_hz;
	else
		return -ENOENT;

	return DIV_TO_RATE(prate, div);
}

#define RK3562_VOP_PLL_LIMIT_FREQ 594000000

static ulong rk3562_vop_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 i, div, sel, best_div = 0, best_sel = 0;
	ulong pll_rate, now, best_rate = 0;

	switch (clk_id) {
	case ACLK_VOP:
		if ((priv->cpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
			sel = ACLK_VOP_SEL_CPLL;
		} else if ((priv->hpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->hpll_hz, rate);
			sel = ACLK_VOP_SEL_HPLL;
		} else if ((priv->vpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->vpll_hz, rate);
			sel = ACLK_VOP_SEL_VPLL;
		} else {
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
			sel = ACLK_VOP_SEL_GPLL;
		}
		rk_clrsetreg(&cru->clksel_con[28],
			     ACLK_VOP_SEL_MASK | ACLK_VOP_DIV_MASK,
			     sel << ACLK_VOP_SEL_SHIFT |
			     ((div - 1) << ACLK_VOP_DIV_SHIFT));

		return rk3562_vop_get_rate(priv, clk_id);
	case DCLK_VOP:
		div = DIV_ROUND_UP(RK3562_VOP_PLL_LIMIT_FREQ, rate);
		if (div % 2)
			div = div + 1;
		rk_clrsetreg(&cru->clksel_con[30],
			     DCLK_VOP_SEL_MASK | DCLK_VOP_DIV_MASK,
			     DCLK_VOP_SEL_VPLL << DCLK_VOP_SEL_SHIFT |
			     ((div - 1) << DCLK_VOP_DIV_SHIFT));
		rockchip_pll_set_rate(&rk3562_pll_clks[VPLL], priv->cru,
				      VPLL, div * rate);
		break;
	case DCLK_VOP1:
		for (i = 0; i <= DCLK_VOP_SEL_APLL; i++) {
			switch (i) {
			case DCLK_VOP_SEL_GPLL:
				pll_rate = priv->gpll_hz;
				break;
			case DCLK_VOP_SEL_HPLL:
				pll_rate = priv->hpll_hz;
				break;
			case DCLK_VOP_SEL_VPLL:
			case DCLK_VOP_SEL_APLL:
				continue;
			default:
				printf("do not support this vop pll sel\n");
				return -EINVAL;
			}

			div = DIV_ROUND_UP(pll_rate, rate);
			if (div > 255)
				continue;
			now = pll_rate / div;
			if (abs(rate - now) < abs(rate - best_rate)) {
				best_rate = now;
				best_div = div;
				best_sel = i;
			}
			debug("p_rate=%lu, best_rate=%lu, div=%u, sel=%u\n",
			      pll_rate, best_rate, best_div, best_sel);
		}
		if (best_rate) {
			rk_clrsetreg(&cru->clksel_con[31],
				     DCLK_VOP1_SEL_MASK | DCLK_VOP1_DIV_MASK,
				     best_sel << DCLK_VOP1_SEL_SHIFT |
				     (best_div - 1) << DCLK_VOP1_DIV_SHIFT);
		} else {
			printf("do not support this vop freq %lu\n", rate);
			return -EINVAL;
		}
		break;
	default:
		return -ENOENT;
	}

	return rk3562_vop_get_rate(priv, clk_id);
}

static ulong rk3562_gmac_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 con, sel, div;
	ulong prate;

	switch (clk_id) {
	case CLK_GMAC_125M_CRU_I:
		con = readl(&cru->clksel_con[45]);
		sel = (con & CLK_GMAC_125M_SEL_MASK) >> CLK_GMAC_125M_SEL_SHIFT;
		if (sel == CLK_GMAC_125M)
			return 125000000;
		else
			return OSC_HZ;
	case CLK_GMAC_50M_CRU_I:
		con = readl(&cru->clksel_con[45]);
		sel = (con & CLK_GMAC_50M_SEL_MASK) >> CLK_GMAC_50M_SEL_SHIFT;
		if (sel == CLK_GMAC_50M)
			return 50000000;
		else
			return OSC_HZ;
	case CLK_MAC100_50M_MATRIX:
		con = readl(&cru->clksel_con[47]);
		sel = (con & CLK_GMAC_50M_SEL_MASK) >> CLK_GMAC_50M_SEL_SHIFT;
		if (sel == CLK_GMAC_50M)
			return 50000000;
		else
			return OSC_HZ;
	case CLK_GMAC_ETH_OUT2IO:
		con = readl(&cru->clksel_con[46]);
		sel = (con & CLK_GMAC_ETH_OUT2IO_SEL_MASK) >> CLK_GMAC_ETH_OUT2IO_SEL_SHIFT;
		div = (con & CLK_GMAC_ETH_OUT2IO_DIV_MASK) >> CLK_GMAC_ETH_OUT2IO_DIV_SHIFT;
		if (sel == CLK_GMAC_ETH_OUT2IO_GPLL)
			prate = priv->gpll_hz;
		else
			prate = priv->cpll_hz;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_gmac_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, div;

	switch (clk_id) {
	case CLK_GMAC_125M_CRU_I:
		if (rate == 125000000)
			sel = CLK_GMAC_125M;
		else
			sel = CLK_GMAC_24M;
		rk_clrsetreg(&cru->clksel_con[45], CLK_GMAC_125M_SEL_MASK,
			     sel << CLK_GMAC_125M_SEL_SHIFT);
		break;
	case CLK_GMAC_50M_CRU_I:
		if (rate == 50000000)
			sel = CLK_GMAC_50M;
		else
			sel = CLK_GMAC_24M;
		rk_clrsetreg(&cru->clksel_con[45], CLK_GMAC_50M_SEL_MASK,
			     sel << CLK_GMAC_50M_SEL_SHIFT);
		break;
	case CLK_MAC100_50M_MATRIX:
		if (rate == 50000000)
			sel = CLK_GMAC_50M;
		else
			sel = CLK_GMAC_24M;
		rk_clrsetreg(&cru->clksel_con[47], CLK_GMAC_50M_SEL_MASK,
			     sel << CLK_GMAC_50M_SEL_SHIFT);
		break;
	case CLK_GMAC_ETH_OUT2IO:
		if ((priv->cpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
			sel = CLK_GMAC_ETH_OUT2IO_CPLL;
		} else {
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
			sel = CLK_GMAC_ETH_OUT2IO_GPLL;
		}
		rk_clrsetreg(&cru->clksel_con[46],
			     CLK_GMAC_ETH_OUT2IO_SEL_MASK | CLK_GMAC_ETH_OUT2IO_DIV_MASK,
			     sel << CLK_GMAC_ETH_OUT2IO_SEL_SHIFT |
			     (div - 1) << CLK_GMAC_ETH_OUT2IO_DIV_SHIFT);
		break;
	default:
		return -ENOENT;
	}

	return rk3562_gmac_get_rate(priv, clk_id);
}

static ulong rk3562_clk_get_rate(struct clk *clk)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate = 0;

	if (!priv->gpll_hz || !priv->cpll_hz || !priv->hpll_hz) {
		printf("%s: gpll=%lu, cpll=%lu, hpll=%lu\n",
		       __func__, priv->gpll_hz, priv->cpll_hz, priv->hpll_hz);
		return -ENOENT;
	}

	switch (clk->id) {
	case PLL_APLL:
	case ARMCLK:
		rate = rockchip_pll_get_rate(&rk3562_pll_clks[APLL], priv->cru,
					     APLL);
		break;
	case PLL_GPLL:
		rate = rockchip_pll_get_rate(&rk3562_pll_clks[GPLL], priv->cru,
					     GPLL);
		break;

	case PLL_VPLL:
		rate = rockchip_pll_get_rate(&rk3562_pll_clks[VPLL], priv->cru,
					     VPLL);
		break;
	case PLL_HPLL:
		rate = rockchip_pll_get_rate(&rk3562_pll_clks[HPLL], priv->cru,
					     HPLL);
		break;
	case PLL_CPLL:
		rate = rockchip_pll_get_rate(&rk3562_pll_clks[CPLL], priv->cru,
					     CPLL);
		break;
	case PLL_DPLL:
		rate = rockchip_pll_get_rate(&rk3562_pll_clks[DPLL], priv->cru,
					     DPLL);
		break;
	case ACLK_BUS:
	case HCLK_BUS:
	case PCLK_BUS:
		rate = rk3562_bus_get_rate(priv, clk->id);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		rate = rk3562_peri_get_rate(priv, clk->id);
		break;
	case CLK_PMU0_I2C0:
	case CLK_I2C:
	case CLK_I2C1:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
		rate = rk3562_i2c_get_rate(priv, clk->id);
		break;
	case SCLK_PMU1_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
	case SCLK_UART3:
	case SCLK_UART4:
	case SCLK_UART5:
	case SCLK_UART6:
	case SCLK_UART7:
	case SCLK_UART8:
	case SCLK_UART9:
		rate = rk3562_uart_get_rate(priv, clk->id);
		break;
	case CLK_PMU1_PWM0:
	case CLK_PWM1_PERI:
	case CLK_PWM2_PERI:
	case CLK_PWM3_PERI:
		rate = rk3562_pwm_get_rate(priv, clk->id);
		break;
	case CLK_PMU1_SPI0:
	case CLK_SPI1:
	case CLK_SPI2:
		rate = rk3562_spi_get_rate(priv, clk->id);
		break;
	case CLK_TSADC:
	case CLK_TSADC_TSEN:
		rate = rk3562_tsadc_get_rate(priv, clk->id);
		break;
	case CLK_SARADC:
	case CLK_SARADC_VCCIO156:
		rate = rk3562_saradc_get_rate(priv, clk->id);
		break;
	case SCLK_SFC:
		rate = rk3562_sfc_get_rate(priv);
		break;
	case CCLK_EMMC:
	case BCLK_EMMC:
		rate = rk3562_emmc_get_rate(priv, clk->id);
		break;
	case HCLK_SDMMC0:
	case HCLK_SDMMC1:
	case CCLK_SDMMC0:
	case CCLK_SDMMC1:
	case SCLK_SDMMC0_SAMPLE:
	case SCLK_SDMMC1_SAMPLE:
		rate = rk3562_sdmmc_get_rate(priv, clk->id);
		break;
	case ACLK_VOP:
	case DCLK_VOP:
	case DCLK_VOP1:
		rate = rk3562_vop_get_rate(priv, clk->id);
		break;
	case CLK_GMAC_125M_CRU_I:
	case CLK_GMAC_50M_CRU_I:
	case CLK_GMAC_ETH_OUT2IO:
	case CLK_MAC100_50M_MATRIX:
		rate = rk3562_gmac_get_rate(priv, clk->id);
		break;
	case CLK_WDTNS:
		rate = OSC_HZ;
		break;
	default:
		return -ENOENT;
	}

	return rate;
};

static ulong rk3562_clk_set_rate(struct clk *clk, ulong rate)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);
	ulong ret = 0;

	if (!priv->gpll_hz || !priv->cpll_hz || !priv->hpll_hz) {
		printf("%s: gpll=%lu, cpll=%lu, hpll=%lu\n",
		       __func__, priv->gpll_hz, priv->cpll_hz, priv->hpll_hz);
		return -ENOENT;
	}

	debug("%s: id=%ld, rate=%ld\n", __func__, clk->id, rate);

	switch (clk->id) {
	case PLL_APLL:
	case ARMCLK:
		if (priv->armclk_hz)
			rk3562_armclk_set_rate(priv, rate);
		priv->armclk_hz = rate;
		break;
	case PLL_GPLL:
		ret = rockchip_pll_set_rate(&rk3562_pll_clks[GPLL], priv->cru,
					    GPLL, rate);
		priv->gpll_hz = rockchip_pll_get_rate(&rk3562_pll_clks[GPLL],
						      priv->cru, GPLL);
		break;
	case PLL_VPLL:
		ret = rockchip_pll_set_rate(&rk3562_pll_clks[VPLL], priv->cru,
					    VPLL, rate);
		priv->vpll_hz = rockchip_pll_get_rate(&rk3562_pll_clks[VPLL],
						      priv->cru, VPLL);
		break;
	case PLL_HPLL:
		ret = rockchip_pll_set_rate(&rk3562_pll_clks[HPLL], priv->cru,
					    HPLL, rate);
		priv->hpll_hz = rockchip_pll_get_rate(&rk3562_pll_clks[HPLL],
						      priv->cru, HPLL);
		break;
	case ACLK_BUS:
	case HCLK_BUS:
	case PCLK_BUS:
		ret = rk3562_bus_set_rate(priv, clk->id, rate);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		ret = rk3562_peri_set_rate(priv, clk->id, rate);
		break;
	case CLK_PMU0_I2C0:
	case CLK_I2C:
	case CLK_I2C1:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
		ret = rk3562_i2c_set_rate(priv, clk->id, rate);
		break;
	case SCLK_PMU1_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
	case SCLK_UART3:
	case SCLK_UART4:
	case SCLK_UART5:
	case SCLK_UART6:
	case SCLK_UART7:
	case SCLK_UART8:
	case SCLK_UART9:
		ret = rk3562_uart_set_rate(priv, clk->id, rate);
		break;
	case CLK_PMU1_PWM0:
	case CLK_PWM1_PERI:
	case CLK_PWM2_PERI:
	case CLK_PWM3_PERI:
		ret = rk3562_pwm_set_rate(priv, clk->id, rate);
		break;
	case CLK_PMU1_SPI0:
	case CLK_SPI1:
	case CLK_SPI2:
		ret = rk3562_spi_set_rate(priv, clk->id, rate);
		break;
	case CLK_TSADC:
	case CLK_TSADC_TSEN:
		ret = rk3562_tsadc_set_rate(priv, clk->id, rate);
		break;
	case CLK_SARADC:
	case CLK_SARADC_VCCIO156:
		ret = rk3562_saradc_set_rate(priv, clk->id, rate);
		break;
	case SCLK_SFC:
		ret = rk3562_sfc_set_rate(priv, rate);
		break;
	case CCLK_EMMC:
	case BCLK_EMMC:
		ret = rk3562_emmc_set_rate(priv, clk->id, rate);
		break;
	case HCLK_SDMMC0:
	case HCLK_SDMMC1:
	case CCLK_SDMMC0:
	case CCLK_SDMMC1:
		ret = rk3562_sdmmc_set_rate(priv, clk->id, rate);
		break;
	case ACLK_VOP:
	case DCLK_VOP:
	case DCLK_VOP1:
		ret = rk3562_vop_set_rate(priv, clk->id, rate);
		break;
	case CLK_GMAC_125M_CRU_I:
	case CLK_GMAC_50M_CRU_I:
	case CLK_GMAC_ETH_OUT2IO:
	case CLK_MAC100_50M_MATRIX:
		ret = rk3562_gmac_set_rate(priv, clk->id, rate);
		break;
	default:
		return -ENOENT;
	}

	return ret;
};

#define ROCKCHIP_MMC_DELAY_SEL		BIT(11)
#define ROCKCHIP_MMC_DEGREE_SHIFT	1
#define ROCKCHIP_MMC_DEGREE_MASK	(0x3 << ROCKCHIP_MMC_DEGREE_SHIFT)
#define ROCKCHIP_MMC_DELAYNUM_SHIFT	3
#define ROCKCHIP_MMC_DELAYNUM_MASK	(0xff << ROCKCHIP_MMC_DELAYNUM_SHIFT)
#define PSECS_PER_SEC 1000000000000LL

/*
 * Each fine delay is between 44ps-77ps. Assume each fine delay is 60ps to
 * simplify calculations. So 45degs could be anywhere between 33deg and 57.8deg.
 */
#define ROCKCHIP_MMC_DELAY_ELEMENT_PSEC 60

int rk3562_mmc_get_phase(struct clk *clk)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3562_cru *cru = priv->cru;
	u32 raw_value, delay_num;
	u16 degrees = 0;
	ulong rate;

	rate = rk3562_clk_get_rate(clk);
	if (rate < 0)
		return rate;

	if (clk->id == SCLK_SDMMC0_SAMPLE)
		raw_value = readl(&cru->sdmmc0_con[1]);
	else if (clk->id == SCLK_SDMMC0_SAMPLE)
		raw_value = readl(&cru->sdmmc1_con[1]);
	else
		return -ENONET;

	raw_value &= ROCKCHIP_MMC_DEGREE_MASK;
	degrees = (raw_value >>  ROCKCHIP_MMC_DEGREE_SHIFT) * 90;

	if (raw_value & ROCKCHIP_MMC_DELAY_SEL) {
		/* degrees/delaynum * 10000 */
		unsigned long factor = (ROCKCHIP_MMC_DELAY_ELEMENT_PSEC / 10) *
					36 * (rate / 1000000);

		delay_num = (raw_value & ROCKCHIP_MMC_DELAYNUM_MASK);
		delay_num >>= ROCKCHIP_MMC_DELAYNUM_SHIFT;
		degrees += DIV_ROUND_CLOSEST(delay_num * factor, 10000);
	}

	return degrees % 360;
}

int rk3562_mmc_set_phase(struct clk *clk, u32 degrees)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3562_cru *cru = priv->cru;
	u8 nineties, remainder, delay_num;
	u32 raw_value, delay;
	ulong rate;

	rate = rk3562_clk_get_rate(clk);
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
	raw_value |= delay_num << ROCKCHIP_MMC_DELAYNUM_SHIFT;
	raw_value |= nineties << ROCKCHIP_MMC_DEGREE_SHIFT;

	if (clk->id == SCLK_SDMMC0_SAMPLE)
		writel(raw_value | 0xffff0000, &cru->sdmmc0_con[1]);
	else
		writel(raw_value | 0xffff0000, &cru->sdmmc1_con[1]);

	debug("mmc set_phase(%d) delay_nums=%u reg=%#x actual_degrees=%d\n",
	      degrees, delay_num, raw_value, rk3562_mmc_get_phase(clk));

	return 0;
}

static int rk3562_clk_get_phase(struct clk *clk)
{
	int ret;

	switch (clk->id) {
	case SCLK_SDMMC0_SAMPLE:
	case SCLK_SDMMC1_SAMPLE:
		ret = rk3562_mmc_get_phase(clk);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static int rk3562_clk_set_phase(struct clk *clk, int degrees)
{
	int ret;

	switch (clk->id) {
	case SCLK_SDMMC0_SAMPLE:
	case SCLK_SDMMC1_SAMPLE:
		ret = rk3562_mmc_set_phase(clk, degrees);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static struct clk_ops rk3562_clk_ops = {
	.get_rate = rk3562_clk_get_rate,
	.set_rate = rk3562_clk_set_rate,
	.get_phase = rk3562_clk_get_phase,
	.set_phase = rk3562_clk_set_phase,
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
	const struct rk3562_clk_info *clk_dump;
	struct rk3562_clk_priv *priv;
	struct udevice *cru_dev;
	struct clk clk;
	ulong clk_count = ARRAY_SIZE(clks_dump);
	ulong rate;
	int i, ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(rockchip_rk3562_cru),
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

static void rk3562_clk_init(struct rk3562_clk_priv *priv)
{
	int ret;

	priv->sync_kernel = false;
	if (!priv->armclk_enter_hz)
		priv->armclk_enter_hz =
			rockchip_pll_get_rate(&rk3562_pll_clks[APLL],
					      priv->cru, APLL);

	if (!priv->armclk_init_hz) {
#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_SUPPORT_USBPLUG)
		ret = rk3562_armclk_set_rate(priv, APLL_HZ);
		if (!ret)
			priv->armclk_init_hz = APLL_HZ;

#else
		struct clk clk;

		ret = rockchip_get_scmi_clk(&clk.dev);
		if (ret) {
			printf("Failed to get scmi clk dev\n");
			return;
		}

		clk.id = ARMCLK;
		ret = clk_set_rate(&clk, CPU_PVTPLL_HZ);
		if (ret < 0) {
			printf("Failed to set scmi cpu %dhz\n", CPU_PVTPLL_HZ);
			return;
		} else {
			priv->armclk_init_hz = CPU_PVTPLL_HZ;
		}
#endif
	}
	if (priv->cpll_hz != CPLL_HZ) {
		ret = rockchip_pll_set_rate(&rk3562_pll_clks[CPLL], priv->cru,
					    CPLL, CPLL_HZ);
		if (!ret)
			priv->cpll_hz = CPLL_HZ;
	}

	if (priv->gpll_hz != GPLL_HZ) {
		ret = rockchip_pll_set_rate(&rk3562_pll_clks[GPLL], priv->cru,
					    GPLL, GPLL_HZ);
		if (!ret)
			priv->gpll_hz = GPLL_HZ;
	}

	if (priv->hpll_hz != HPLL_HZ) {
		ret = rockchip_pll_set_rate(&rk3562_pll_clks[HPLL], priv->cru,
					    HPLL, HPLL_HZ);
		if (!ret)
			priv->hpll_hz = HPLL_HZ;
	}
}

static int rk3562_clk_probe(struct udevice *dev)
{
	struct rk3562_clk_priv *priv = dev_get_priv(dev);
	int ret;

	rk3562_clk_init(priv);

	/* Process 'assigned-{clocks/clock-parents/clock-rates}' properties */
	ret = clk_set_defaults(dev);
	if (ret)
		debug("%s clk_set_defaults failed %d\n", __func__, ret);
	else
		priv->sync_kernel = true;

	return 0;
}

static int rk3562_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct rk3562_clk_priv *priv = dev_get_priv(dev);

	priv->cru = dev_read_addr_ptr(dev);

	return 0;
}

static int rk3562_clk_bind(struct udevice *dev)
{
	struct udevice *sys_child, *sf_child;
	struct softreset_reg *sf_priv;
	struct sysreset_reg *priv;
	int ret;

	/* The reset driver does not have a device node, so bind it here */
	ret = device_bind_driver(dev, "rockchip_sysreset", "sysreset",
				 &sys_child);
	if (ret) {
		debug("Warning: No sysreset driver: ret=%d\n", ret);
	} else {
		priv = malloc(sizeof(struct sysreset_reg));
		priv->glb_srst_fst_value = offsetof(struct rk3562_cru,
						    glb_srst_fst);
		priv->glb_srst_snd_value = offsetof(struct rk3562_cru,
						    glb_srst_snd);
		sys_child->priv = priv;
	}

	ret = device_bind_driver_to_node(dev, "rockchip_reset", "reset",
					 dev_ofnode(dev), &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk3562_cru,
						    softrst_con[0]);
		/* (0x30444 - 0x400) / 4 + 1 = 49170 */
		sf_priv->sf_reset_num = 49170;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk3562_clk_ids[] = {
	{ .compatible = "rockchip,rk3562-cru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3562_cru) = {
	.name		= "rockchip_rk3562_cru",
	.id		= UCLASS_CLK,
	.of_match	= rk3562_clk_ids,
	.priv_auto_alloc_size = sizeof(struct rk3562_clk_priv),
	.ofdata_to_platdata = rk3562_clk_ofdata_to_platdata,
	.ops		= &rk3562_clk_ops,
	.bind		= rk3562_clk_bind,
	.probe		= rk3562_clk_probe,
};

/* spl scmi clk */
#ifdef CONFIG_SPL_BUILD

static ulong rk3562_crypto_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	struct rk3562_cru *cru = priv->cru;
	u32 sel, con;
	ulong rate;

	con = readl(&cru->periclksel_con[43]);
	switch (clk_id) {
	case CLK_CORE_CRYPTO:
		sel = (con & CLK_CORE_CRYPTO_SEL_MASK) >>
		       CLK_CORE_CRYPTO_SEL_SHIFT;
		if (sel == CLK_CORE_CRYPTO_SEL_200M)
			rate = 200 * MHz;
		else if (sel == CLK_CORE_CRYPTO_SEL_100M)
			rate = 100 * MHz;
		else
			rate = OSC_HZ;
		break;
	case CLK_PKA_CRYPTO:
		sel = (con & CLK_PKA_CRYPTO_SEL_MASK) >>
		       CLK_PKA_CRYPTO_SEL_SHIFT;
		if (sel == CLK_PKA_CRYPTO_SEL_300M)
			rate = 300 * MHz;
		else if (sel == CLK_PKA_CRYPTO_SEL_200M)
			rate = 200 * MHz;
		else if (sel == CLK_PKA_CRYPTO_SEL_100M)
			rate = 100 * MHz;
		else
			rate = OSC_HZ;
		break;
	default:
		return -ENOENT;
	}

	return rate;
}

static ulong rk3562_crypto_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				    ulong rate)
{
	struct rk3562_cru *cru = priv->cru;
	u32 mask, shift, sel;

	switch (clk_id) {
	case CLK_CORE_CRYPTO:
		mask = CLK_CORE_CRYPTO_SEL_MASK;
		shift =	CLK_CORE_CRYPTO_SEL_SHIFT;
		if (rate == 200 * MHz)
			sel = CLK_CORE_CRYPTO_SEL_200M;
		else if (rate == 100 * MHz)
			sel = CLK_CORE_CRYPTO_SEL_100M;
		else
			sel = CLK_CORE_CRYPTO_SEL_24M;
		break;
	case CLK_PKA_CRYPTO:
		mask = CLK_PKA_CRYPTO_SEL_MASK;
		shift =	CLK_PKA_CRYPTO_SEL_SHIFT;
		if (rate == 300 * MHz)
			sel = CLK_PKA_CRYPTO_SEL_300M;
		else if (rate == 200 * MHz)
			sel = CLK_PKA_CRYPTO_SEL_200M;
		else if (rate == 100 * MHz)
			sel = CLK_PKA_CRYPTO_SEL_100M;
		else
			sel = CLK_PKA_CRYPTO_SEL_24M;
		break;
	default:
		return -ENOENT;
	}
	rk_clrsetreg(&cru->periclksel_con[43], mask, sel << shift);

	return rk3562_crypto_get_rate(priv, clk_id);
}

static ulong rk3562_clk_scmi_get_rate(struct clk *clk)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);

	switch (clk->id) {
	case CLK_CORE_CRYPTO:
	case CLK_PKA_CRYPTO:
		return rk3562_crypto_get_rate(priv, clk->id);
	default:
		return -ENOENT;
	}
};

static ulong rk3562_clk_scmi_set_rate(struct clk *clk, ulong rate)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);

	switch (clk->id) {
	case CLK_CORE_CRYPTO:
	case CLK_PKA_CRYPTO:
		return rk3562_crypto_set_rate(priv, clk->id, rate);
	default:
		return -ENOENT;
	}
	return 0;
};

static int rk3562_scmi_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct rk3562_clk_priv *priv = dev_get_priv(dev);

	priv->cru = (struct rk3562_cru *)0xff100000;

	return 0;
}

/* A fake scmi driver for SPL/TPL where smccc agent is not available. */
static const struct clk_ops scmi_clk_ops = {
	.get_rate = rk3562_clk_scmi_get_rate,
	.set_rate = rk3562_clk_scmi_set_rate,
};

U_BOOT_DRIVER(scmi_clock) = {
	.name = "scmi_clk",
	.id = UCLASS_CLK,
	.ops = &scmi_clk_ops,
	.priv_auto_alloc_size = sizeof(struct rk3562_clk_priv),
	.ofdata_to_platdata = rk3562_scmi_clk_ofdata_to_platdata,
};
#endif
