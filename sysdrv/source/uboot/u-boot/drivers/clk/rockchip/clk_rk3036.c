/*
 * (C) Copyright 2015 Google, Inc
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
#include <asm/arch/cru_rk3036.h>
#include <asm/arch/hardware.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rk3036-cru.h>
#include <linux/log2.h>

DECLARE_GLOBAL_DATA_PTR;

enum {
	VCO_MAX_HZ	= 2400U * 1000000,
	VCO_MIN_HZ	= 600 * 1000000,
	OUTPUT_MAX_HZ	= 2400U * 1000000,
	OUTPUT_MIN_HZ	= 24 * 1000000,
};

#ifndef CONFIG_SPL_BUILD
#define RK3036_CLK_DUMP(_id, _name, _iscru)	\
{						\
	.id = _id,				\
	.name = _name,				\
	.is_cru = _iscru,			\
}

static const struct rk3036_clk_info clks_dump[] = {
	RK3036_CLK_DUMP(PLL_APLL, "apll", true),
	RK3036_CLK_DUMP(PLL_DPLL, "dpll", true),
	RK3036_CLK_DUMP(PLL_GPLL, "gpll", true),
};
#endif

#define RATE_TO_DIV(input_rate, output_rate) \
	((input_rate) / (output_rate) - 1);

#define DIV_TO_RATE(input_rate, div)	((input_rate) / ((div) + 1))

#define PLL_DIVISORS(hz, _refdiv, _postdiv1, _postdiv2) {\
	.refdiv = _refdiv,\
	.fbdiv = (u32)((u64)hz * _refdiv * _postdiv1 * _postdiv2 / OSC_HZ),\
	.postdiv1 = _postdiv1, .postdiv2 = _postdiv2};\
	_Static_assert(((u64)hz * _refdiv * _postdiv1 * _postdiv2 / OSC_HZ) *\
			 OSC_HZ / (_refdiv * _postdiv1 * _postdiv2) == hz,\
			 #hz "Hz cannot be hit with PLL "\
			 "divisors on line " __stringify(__LINE__));

/* use integer mode*/
static const struct pll_div apll_init_cfg = PLL_DIVISORS(APLL_HZ, 1, 3, 1);
static const struct pll_div gpll_init_cfg = PLL_DIVISORS(GPLL_HZ, 2, 2, 1);

static int rkclk_set_pll(struct rk3036_cru *cru, enum rk_clk_id clk_id,
			 const struct pll_div *div)
{
	int pll_id = rk_pll_id(clk_id);
	struct rk3036_pll *pll = &cru->pll[pll_id];

	/* All PLLs have same VCO and output frequency range restrictions. */
	uint vco_hz = OSC_HZ / 1000 * div->fbdiv / div->refdiv * 1000;
	uint output_hz = vco_hz / div->postdiv1 / div->postdiv2;

	debug("PLL at %p: fbdiv=%d, refdiv=%d, postdiv1=%d, postdiv2=%d,\
		 vco=%u Hz, output=%u Hz\n",
			pll, div->fbdiv, div->refdiv, div->postdiv1,
			div->postdiv2, vco_hz, output_hz);
	assert(vco_hz >= VCO_MIN_HZ && vco_hz <= VCO_MAX_HZ &&
	       output_hz >= OUTPUT_MIN_HZ && output_hz <= OUTPUT_MAX_HZ);

	/* use integer mode */
	rk_setreg(&pll->con1, 1 << PLL_DSMPD_SHIFT);

	/* Power down */
	rk_setreg(&pll->con1, 1 << PLL_PD_SHIFT);

	rk_clrsetreg(&pll->con0,
		     PLL_POSTDIV1_MASK | PLL_FBDIV_MASK,
		     (div->postdiv1 << PLL_POSTDIV1_SHIFT) | div->fbdiv);
	rk_clrsetreg(&pll->con1, PLL_POSTDIV2_MASK | PLL_REFDIV_MASK,
		     (div->postdiv2 << PLL_POSTDIV2_SHIFT |
		     div->refdiv << PLL_REFDIV_SHIFT));

	/* Power Up */
	rk_clrreg(&pll->con1, 1 << PLL_PD_SHIFT);

	/* waiting for pll lock */
	while (readl(&pll->con1) & (1 << PLL_LOCK_STATUS_SHIFT))
		udelay(1);

	return 0;
}

static void rkclk_init(struct rk3036_cru *cru)
{
	u32 aclk_div;
	u32 hclk_div;
	u32 pclk_div;
	u32 nandc_div;

	/* pll enter slow-mode */
	rk_clrsetreg(&cru->cru_mode_con,
		     GPLL_MODE_MASK | APLL_MODE_MASK,
		     GPLL_MODE_SLOW << GPLL_MODE_SHIFT |
		     APLL_MODE_SLOW << APLL_MODE_SHIFT);

	/* init pll */
	rkclk_set_pll(cru, CLK_ARM, &apll_init_cfg);
	rkclk_set_pll(cru, CLK_GENERAL, &gpll_init_cfg);

	/*
	 * select apll as cpu/core clock pll source and
	 * set up dependent divisors for PERI and ACLK clocks.
	 * core hz : apll = 1:1
	 */
	aclk_div = APLL_HZ / CORE_ACLK_HZ - 1;
	assert((aclk_div + 1) * CORE_ACLK_HZ <= APLL_HZ && aclk_div < 0x7);

	pclk_div = APLL_HZ / CORE_PERI_HZ - 1;
	assert((pclk_div + 1) * CORE_PERI_HZ <= APLL_HZ && pclk_div < 0xf);

	rk_clrsetreg(&cru->cru_clksel_con[0],
		     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
		     CORE_CLK_PLL_SEL_APLL << CORE_CLK_PLL_SEL_SHIFT |
		     0 << CORE_DIV_CON_SHIFT);

	rk_clrsetreg(&cru->cru_clksel_con[1],
		     CORE_ACLK_DIV_MASK | CORE_PERI_DIV_MASK,
		     aclk_div << CORE_ACLK_DIV_SHIFT |
		     pclk_div << CORE_PERI_DIV_SHIFT);

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	aclk_div = GPLL_HZ / BUS_ACLK_HZ - 1;
	assert((aclk_div + 1) * BUS_ACLK_HZ <= GPLL_HZ && aclk_div <= 0x1f);

	pclk_div = BUS_ACLK_HZ / BUS_PCLK_HZ - 1;
	assert((pclk_div + 1) * BUS_PCLK_HZ <= BUS_ACLK_HZ && pclk_div <= 0x7);

	hclk_div = BUS_ACLK_HZ / BUS_HCLK_HZ - 1;
	assert((hclk_div + 1) * BUS_HCLK_HZ <= BUS_ACLK_HZ && hclk_div <= 0x3);

	rk_clrsetreg(&cru->cru_clksel_con[0],
		     BUS_ACLK_PLL_SEL_MASK | BUS_ACLK_DIV_MASK,
		     BUS_ACLK_PLL_SEL_GPLL << BUS_ACLK_PLL_SEL_SHIFT |
		     aclk_div << BUS_ACLK_DIV_SHIFT);

	rk_clrsetreg(&cru->cru_clksel_con[1],
		     BUS_PCLK_DIV_MASK | BUS_HCLK_DIV_MASK,
		     pclk_div << BUS_PCLK_DIV_SHIFT |
		     hclk_div << BUS_HCLK_DIV_SHIFT);

	/*
	 * select gpll as pd_peri bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	aclk_div = GPLL_HZ / PERI_ACLK_HZ - 1;
	assert((aclk_div + 1) * PERI_ACLK_HZ <= GPLL_HZ && aclk_div < 0x1f);

	hclk_div = ilog2(PERI_ACLK_HZ / PERI_HCLK_HZ);
	assert((1 << hclk_div) * PERI_HCLK_HZ <=
		PERI_ACLK_HZ && (hclk_div < 0x4));

	pclk_div = ilog2(PERI_ACLK_HZ / PERI_PCLK_HZ);
	assert((1 << pclk_div) * PERI_PCLK_HZ <=
		PERI_ACLK_HZ && pclk_div < 0x8);

	rk_clrsetreg(&cru->cru_clksel_con[10],
		     PERI_PLL_SEL_MASK | PERI_PCLK_DIV_MASK |
		     PERI_HCLK_DIV_MASK | PERI_ACLK_DIV_MASK,
		     PERI_PLL_GPLL << PERI_PLL_SEL_SHIFT |
		     pclk_div << PERI_PCLK_DIV_SHIFT |
		     hclk_div << PERI_HCLK_DIV_SHIFT |
		     aclk_div << PERI_ACLK_DIV_SHIFT);

	nandc_div = DIV_ROUND_UP(GPLL_HZ, 150 * 1000000);

	rk_clrsetreg(&cru->cru_clksel_con[16],
		     NANDC_PLL_MASK | NANDC_DIV_MASK,
		     NANDC_SEL_GPLL << NANDC_PLL_SHIFT |
		     nandc_div << NANDC_DIV_SHIFT);

	/* PLL enter normal-mode */
	rk_clrsetreg(&cru->cru_mode_con,
		     GPLL_MODE_MASK | APLL_MODE_MASK,
		     GPLL_MODE_NORM << GPLL_MODE_SHIFT |
		     APLL_MODE_NORM << APLL_MODE_SHIFT);
}

/* Get pll rate by id */
static uint32_t rkclk_pll_get_rate(struct rk3036_cru *cru,
				   enum rk_clk_id clk_id)
{
	uint32_t refdiv, fbdiv, postdiv1, postdiv2;
	uint32_t con;
	int pll_id = rk_pll_id(clk_id);
	struct rk3036_pll *pll = &cru->pll[pll_id];
	static u8 clk_shift[CLK_COUNT] = {
		0xff, APLL_MODE_SHIFT, DPLL_MODE_SHIFT, 0xff,
		GPLL_MODE_SHIFT, 0xff
	};
	static u32 clk_mask[CLK_COUNT] = {
		0xffffffff, APLL_MODE_MASK, DPLL_MODE_MASK, 0xffffffff,
		GPLL_MODE_MASK, 0xffffffff
	};
	uint shift;
	uint mask;

	con = readl(&cru->cru_mode_con);
	shift = clk_shift[clk_id];
	mask = clk_mask[clk_id];

	switch ((con & mask) >> shift) {
	case GPLL_MODE_SLOW:
		return OSC_HZ;
	case GPLL_MODE_NORM:

		/* normal mode */
		con = readl(&pll->con0);
		postdiv1 = (con & PLL_POSTDIV1_MASK) >> PLL_POSTDIV1_SHIFT;
		fbdiv = (con & PLL_FBDIV_MASK) >> PLL_FBDIV_SHIFT;
		con = readl(&pll->con1);
		postdiv2 = (con & PLL_POSTDIV2_MASK) >> PLL_POSTDIV2_SHIFT;
		refdiv = (con & PLL_REFDIV_MASK) >> PLL_REFDIV_SHIFT;
		return (24 * fbdiv / (refdiv * postdiv1 * postdiv2)) * 1000000;
	case GPLL_MODE_DEEP:
	default:
		return 32768;
	}
}

static ulong rockchip_mmc_get_clk(struct rk3036_cru *cru, uint clk_general_rate,
				  int periph)
{
	uint src_rate;
	uint div, mux;
	u32 con;

	switch (periph) {
	case HCLK_EMMC:
	case SCLK_EMMC:
		con = readl(&cru->cru_clksel_con[12]);
		mux = (con & EMMC_PLL_MASK) >> EMMC_PLL_SHIFT;
		div = (con & EMMC_DIV_MASK) >> EMMC_DIV_SHIFT;
		break;
	case HCLK_SDIO:
	case SCLK_SDIO:
		con = readl(&cru->cru_clksel_con[12]);
		mux = (con & MMC0_PLL_MASK) >> MMC0_PLL_SHIFT;
		div = (con & MMC0_DIV_MASK) >> MMC0_DIV_SHIFT;
		break;
	default:
		return -EINVAL;
	}

	src_rate = mux == EMMC_SEL_24M ? OSC_HZ : clk_general_rate;
	return DIV_TO_RATE(src_rate, div) / 2;
}

static ulong rockchip_mmc_set_clk(struct rk3036_cru *cru, uint clk_general_rate,
				  int periph, uint freq)
{
	int src_clk_div;
	int mux;

	debug("%s: clk_general_rate=%u\n", __func__, clk_general_rate);

	/* mmc clock auto divide 2 in internal */
	src_clk_div = DIV_ROUND_UP(clk_general_rate / 2, freq);

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
		rk_clrsetreg(&cru->cru_clksel_con[12],
			     EMMC_PLL_MASK | EMMC_DIV_MASK,
			     mux << EMMC_PLL_SHIFT |
			     (src_clk_div - 1) << EMMC_DIV_SHIFT);
		break;
	case HCLK_SDIO:
	case SCLK_SDIO:
		rk_clrsetreg(&cru->cru_clksel_con[11],
			     MMC0_PLL_MASK | MMC0_DIV_MASK,
			     mux << MMC0_PLL_SHIFT |
			     (src_clk_div - 1) << MMC0_DIV_SHIFT);
		break;
	default:
		return -EINVAL;
	}

	return rockchip_mmc_get_clk(cru, clk_general_rate, periph);
}

static ulong rk3036_spi_get_clk(struct rk3036_cru *cru, uint clk_general_rate)
{
	u32 div, con;

	con = readl(&cru->cru_clksel_con[25]);
	div = (con & SPI_DIV_MASK) >> SPI_DIV_SHIFT;

	return DIV_TO_RATE(clk_general_rate, div);
}

static ulong  rk3036_spi_set_clk(struct rk3036_cru *cru,
				 uint clk_general_rate,
				 ulong hz)
{
	int div;

	div = DIV_ROUND_UP(clk_general_rate, hz);
	assert(div - 1 < 128);
	rk_clrsetreg(&cru->cru_clksel_con[25],
		     SPI_PLL_SEL_MASK | SPI_DIV_MASK,
		     SPI_PLL_SEL_GPLL << SPI_PLL_SEL_SHIFT |
		     (div - 1) << SPI_DIV_SHIFT);
	return  rk3036_spi_get_clk(cru, clk_general_rate);
}

static ulong rockchip_dclk_lcdc_get_clk(struct rk3036_cru *cru,
					uint clk_general_rate)
{
	u32 con, div, sel, parent;

	con = readl(&cru->cru_clksel_con[28]);
	div = (con & LCDC_DCLK_DIV_MASK) >> LCDC_DCLK_DIV_SHIFT;
	sel = (con & LCDC_DCLK_SEL_MASK) >> LCDC_DCLK_SEL_SHIFT;
	if (sel == LCDC_DCLK_SEL_GPLL)
		parent = clk_general_rate;
	else
		return -ENOENT;

	return DIV_TO_RATE(parent, div);
}

static ulong rockchip_dclk_lcdc_set_clk(struct rk3036_cru *cru,
					uint clk_general_rate, uint freq)
{
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(clk_general_rate, freq);
	assert(src_clk_div - 1 <= 255);

	rk_clrsetreg(&cru->cru_clksel_con[28],
		     LCDC_DCLK_SEL_MASK | LCDC_DCLK_DIV_MASK,
		     LCDC_DCLK_SEL_GPLL << LCDC_DCLK_SEL_SHIFT |
		     (src_clk_div - 1) << LCDC_DCLK_DIV_SHIFT);

	return rockchip_dclk_lcdc_get_clk(cru, clk_general_rate);
}

static ulong rockchip_aclk_lcdc_get_clk(struct rk3036_cru *cru,
					uint clk_general_rate)
{
	u32 con, div, sel, parent;

	con = readl(&cru->cru_clksel_con[31]);
	div = (con & LCDC_ACLK_DIV_MASK) >> LCDC_ACLK_DIV_SHIFT;
	sel = (con & LCDC_ACLK_SEL_MASK) >> LCDC_ACLK_SEL_SHIFT;
	if (sel == LCDC_ACLK_SEL_GPLL)
		parent = clk_general_rate;
	else
		return -ENOENT;

	return DIV_TO_RATE(parent, div);
}

static ulong rockchip_aclk_lcdc_set_clk(struct rk3036_cru *cru,
					uint clk_general_rate, uint freq)
{
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(clk_general_rate, freq);
	assert(src_clk_div - 1 <= 31);

	rk_clrsetreg(&cru->cru_clksel_con[31],
		     LCDC_ACLK_SEL_MASK | LCDC_ACLK_DIV_MASK,
		     LCDC_ACLK_SEL_GPLL << LCDC_ACLK_SEL_SHIFT |
		     (src_clk_div - 1) << LCDC_ACLK_DIV_SHIFT);

	return rockchip_aclk_lcdc_get_clk(cru, clk_general_rate);
}

static ulong rk3036_peri_get_clk(struct rk3036_clk_priv *priv, ulong clk_id,
				 uint clk_general_rate)
{
	struct rk3036_cru *cru = priv->cru;
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_PERI:
		con = readl(&cru->cru_clksel_con[10]);
		div = (con & PERI_ACLK_DIV_MASK) >> PERI_ACLK_DIV_SHIFT;
		parent = clk_general_rate;
		break;
	case PCLK_PWM:
		con = readl(&cru->cru_clksel_con[10]);
		div = (con & PERI_PCLK_DIV_MASK) >> PERI_PCLK_DIV_SHIFT;
		parent = rk3036_peri_get_clk(priv, ACLK_PERI, clk_general_rate);
		break;
	default:
		printf("do not support this peripheral bus\n");
		return -EINVAL;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3036_peri_set_clk(struct rk3036_clk_priv *priv,
				 ulong clk_id, uint clk_general_rate,
				 uint hz)
{
	struct rk3036_cru *cru = priv->cru;
	int src_clk_div;

	switch (clk_id) {
	case ACLK_PERI:
		src_clk_div = DIV_ROUND_UP(clk_general_rate, hz);
		assert(src_clk_div - 1 < 32);
		rk_clrsetreg(&cru->cru_clksel_con[10],
			     PERI_PLL_SEL_MASK | PERI_ACLK_DIV_MASK,
			     PERI_PLL_GPLL << PERI_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << PERI_ACLK_DIV_SHIFT);
		break;
	case PCLK_PWM:
		src_clk_div = DIV_ROUND_UP(rk3036_peri_get_clk(priv,
							       ACLK_PERI,
							       clk_general_rate),
							       hz);
		assert(src_clk_div - 1 < 8);
		rk_clrsetreg(&cru->cru_clksel_con[10],
			     PERI_PCLK_DIV_MASK,
			     (src_clk_div - 1) << PERI_PCLK_DIV_SHIFT);
		break;
	default:
		printf("do not support this peripheral bus\n");
		return -EINVAL;
	}

	return rk3036_peri_get_clk(priv, clk_id, clk_general_rate);
}

static ulong rk3036_clk_get_rate(struct clk *clk)
{
	struct rk3036_clk_priv *priv = dev_get_priv(clk->dev);
	ulong gclk_rate = rkclk_pll_get_rate(priv->cru, CLK_GENERAL);

	switch (clk->id) {
	case 0 ... 63:
		return rkclk_pll_get_rate(priv->cru, clk->id);
	case SCLK_LCDC:
		return rockchip_dclk_lcdc_get_clk(priv->cru, gclk_rate);
	case ACLK_LCDC:
		return rockchip_aclk_lcdc_get_clk(priv->cru, gclk_rate);
	case SCLK_SPI:
		return rk3036_spi_get_clk(priv->cru, gclk_rate);
	case PCLK_PWM:
		return rk3036_peri_get_clk(priv, clk->id, gclk_rate);
	default:
		return -ENOENT;
	}
}

static ulong rk3036_clk_set_rate(struct clk *clk, ulong rate)
{
	struct rk3036_clk_priv *priv = dev_get_priv(clk->dev);
	ulong new_rate, gclk_rate;

	gclk_rate = rkclk_pll_get_rate(priv->cru, CLK_GENERAL);
	switch (clk->id) {
	case 0 ... 63:
		return 0;
	case HCLK_EMMC:
	case SCLK_EMMC:
		new_rate = rockchip_mmc_set_clk(priv->cru, gclk_rate,
						clk->id, rate);
		break;
	case SCLK_LCDC:
		new_rate = rockchip_dclk_lcdc_set_clk(priv->cru, gclk_rate,
						      rate);
		break;
	case ACLK_LCDC:
		new_rate = rockchip_aclk_lcdc_set_clk(priv->cru, gclk_rate,
						      rate);
		break;
	case SCLK_SPI:
		new_rate = rk3036_spi_set_clk(priv->cru, gclk_rate,
					      rate);
		break;
	case PCLK_PWM:
		new_rate = rk3036_peri_set_clk(priv, clk->id, gclk_rate,
					       rate);
		break;
	default:
		return -ENOENT;
	}

	return new_rate;
}

static struct clk_ops rk3036_clk_ops = {
	.get_rate	= rk3036_clk_get_rate,
	.set_rate	= rk3036_clk_set_rate,
};

static int rk3036_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct rk3036_clk_priv *priv = dev_get_priv(dev);

	priv->cru = dev_read_addr_ptr(dev);

	return 0;
}

static int rk3036_clk_probe(struct udevice *dev)
{
	struct rk3036_clk_priv *priv = dev_get_priv(dev);

	priv->sync_kernel = false;
	if (!priv->armclk_enter_hz)
		priv->armclk_enter_hz = rkclk_pll_get_rate(priv->cru,
							   CLK_ARM);
	rkclk_init(priv->cru);
	if (!priv->armclk_init_hz)
		priv->armclk_init_hz = rkclk_pll_get_rate(priv->cru,
							  CLK_ARM);

	return 0;
}

static int rk3036_clk_bind(struct udevice *dev)
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
		priv->glb_srst_fst_value = offsetof(struct rk3036_cru,
						    cru_glb_srst_fst_value);
		priv->glb_srst_snd_value = offsetof(struct rk3036_cru,
						    cru_glb_srst_snd_value);
		sys_child->priv = priv;
	}

	ret = device_bind_driver_to_node(dev, "rockchip_reset", "reset",
					 dev_ofnode(dev), &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk3036_cru,
						    cru_softrst_con[0]);
		sf_priv->sf_reset_num = 9;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk3036_clk_ids[] = {
	{ .compatible = "rockchip,rk3036-cru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3036_cru) = {
	.name		= "clk_rk3036",
	.id		= UCLASS_CLK,
	.of_match	= rk3036_clk_ids,
	.priv_auto_alloc_size = sizeof(struct rk3036_clk_priv),
	.ofdata_to_platdata = rk3036_clk_ofdata_to_platdata,
	.ops		= &rk3036_clk_ops,
	.bind		= rk3036_clk_bind,
	.probe		= rk3036_clk_probe,
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
	struct rk3036_clk_priv *priv;
	const struct rk3036_clk_info *clk_dump;
	struct clk clk;
	unsigned long clk_count = ARRAY_SIZE(clks_dump);
	unsigned long rate;
	int i, ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(rockchip_rk3036_cru),
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

