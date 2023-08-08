/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 * Author: Andy Yan <andy.yan@rock-chips.com>
 * (C) Copyright 2017 Theobroma Systems Design und Consulting GmbH
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
#include <asm/arch/clock.h>
#include <asm/arch/cru_rk3368.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rk3368-cru.h>

DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_IS_ENABLED(OF_PLATDATA)
struct rk3368_clk_plat {
	struct dtd_rockchip_rk3368_cru dtd;
};
#endif

struct pll_div {
	ulong rate;
	u32 nr;
	u32 nf;
	u32 no;
	u32 nb;
};

#define RK3368_PLL_RATE(_rate, _nr, _nf, _no, _nb)	\
{							\
	.rate	= _rate##U,				\
	.nr = _nr,					\
	.nf = _nf,					\
	.no = _no,					\
	.nb = _nb,					\
}

static struct pll_div rk3368_pll_rates[] = {
	/* _mhz,  _nr, _nf, _no, _nb */
	RK3368_PLL_RATE(594000000, 1, 99, 4, 16),
	RK3368_PLL_RATE(424200000, 5, 707, 8, 0),
	RK3368_PLL_RATE(410000000, 3, 205, 4, 16),
};

#define OSC_HZ		(24 * 1000 * 1000)
#define APLL_L_HZ	(800 * 1000 * 1000)
#define APLL_B_HZ	(816 * 1000 * 1000)
#define GPLL_HZ		(576 * 1000 * 1000)
#define CPLL_HZ		(400 * 1000 * 1000)
#define NPLL_HZ		(594 * 1000 * 1000)

#define DIV_TO_RATE(input_rate, div)    ((input_rate) / ((div) + 1))

#if !defined(CONFIG_SPL_BUILD)
#define RK3368_CLK_DUMP(_id, _name, _iscru)	\
{						\
	.id = _id,				\
	.name = _name,				\
	.is_cru = _iscru,			\
}

static const struct rk3368_clk_info clks_dump[] = {
	RK3368_CLK_DUMP(PLL_APLLB, "apllb", true),
	RK3368_CLK_DUMP(PLL_APLLL, "aplll", true),
	RK3368_CLK_DUMP(PLL_DPLL, "dpll", true),
	RK3368_CLK_DUMP(PLL_CPLL, "cpll", true),
	RK3368_CLK_DUMP(PLL_GPLL, "gpll", true),
	RK3368_CLK_DUMP(PLL_NPLL, "npll", true),
	RK3368_CLK_DUMP(ARMCLKB, "armclkb", true),
	RK3368_CLK_DUMP(ARMCLKL, "armclkl", true),
	RK3368_CLK_DUMP(ACLK_BUS, "aclk_bus", true),
	RK3368_CLK_DUMP(HCLK_BUS, "hclk_bus", true),
	RK3368_CLK_DUMP(PCLK_BUS, "pclk_Bus", true),
	RK3368_CLK_DUMP(ACLK_PERI, "aclk_peri", true),
	RK3368_CLK_DUMP(HCLK_PERI, "hclk_peri", true),
	RK3368_CLK_DUMP(PCLK_PERI, "pclk_peri", true),
};
#endif

#define RK3368_CPUCLK_RATE(_rate, _aclk_div, _pclk_div)		\
{								\
	.rate	= _rate##U,					\
	.aclk_div = _aclk_div,					\
	.pclk_div = _pclk_div,					\
}

static struct rockchip_cpu_rate_table rk3368_cpu_rates[] = {
#if !defined(CONFIG_SPL_BUILD)
	RK3368_CPUCLK_RATE(1200000000, 1, 5),
	RK3368_CPUCLK_RATE(1008000000, 1, 5),
#endif
	RK3368_CPUCLK_RATE(816000000, 1, 3),
	RK3368_CPUCLK_RATE(600000000, 1, 3),
};

#define PLL_DIVISORS(hz, _nr, _no) { \
	.nr = _nr, .nf = (u32)((u64)hz * _nr * _no / OSC_HZ), .no = _no}; \
	_Static_assert(((u64)hz * _nr * _no / OSC_HZ) * OSC_HZ /\
		       (_nr * _no) == hz, #hz "Hz cannot be hit with PLL " \
		       "divisors on line " __stringify(__LINE__));

#if IS_ENABLED(CONFIG_SPL_BUILD) || IS_ENABLED(CONFIG_TPL_BUILD)
static const struct pll_div apll_l_init_cfg = PLL_DIVISORS(APLL_L_HZ, 12, 2);
static const struct pll_div apll_b_init_cfg = PLL_DIVISORS(APLL_B_HZ, 1, 2);
#if !defined(CONFIG_TPL_BUILD)
static const struct pll_div gpll_init_cfg = PLL_DIVISORS(GPLL_HZ, 1, 2);
static const struct pll_div cpll_init_cfg = PLL_DIVISORS(CPLL_HZ, 1, 6);
#endif
#endif

static ulong rk3368_clk_get_rate(struct clk *clk);

#define VCO_MAX_KHZ	2200000
#define VCO_MIN_KHZ	440000
#define FREF_MAX_KHZ	2200000
#define FREF_MIN_KHZ	269
#define PLL_LIMIT_FREQ	400000000

struct pll_div *rkclk_get_pll_config(ulong freq_hz)
{
	unsigned int rate_count = ARRAY_SIZE(rk3368_pll_rates);
	int i;

	for (i = 0; i < rate_count; i++) {
		if (freq_hz == rk3368_pll_rates[i].rate)
			return &rk3368_pll_rates[i];
	}
	return NULL;
}

static int pll_para_config(ulong freq_hz, struct pll_div *div, uint *ext_div)
{
	struct pll_div *best_div = NULL;
	uint ref_khz = OSC_HZ / 1000, nr, nf = 0;
	uint fref_khz;
	uint diff_khz, best_diff_khz;
	const uint max_nr = 1 << 6, max_nf = 1 << 12, max_no = 1 << 4;
	uint vco_khz;
	uint no = 1;
	uint freq_khz = freq_hz / 1000;

	if (!freq_hz) {
		printf("%s: the frequency can not be 0 Hz\n", __func__);
		return -EINVAL;
	}

	no = DIV_ROUND_UP(VCO_MIN_KHZ, freq_khz);
	if (ext_div) {
		*ext_div = DIV_ROUND_UP(PLL_LIMIT_FREQ, freq_hz);
		no = DIV_ROUND_UP(no, *ext_div);
	}

	best_div = rkclk_get_pll_config(freq_hz * (*ext_div));
	if (best_div) {
		div->nr = best_div->nr;
		div->nf = best_div->nf;
		div->no = best_div->no;
		div->nb = best_div->nb;
		return 0;
	}

	/* only even divisors (and 1) are supported */
	if (no > 1)
		no = DIV_ROUND_UP(no, 2) * 2;

	vco_khz = freq_khz * no;
	if (ext_div)
		vco_khz *= *ext_div;

	if (vco_khz < VCO_MIN_KHZ || vco_khz > VCO_MAX_KHZ || no > max_no) {
		printf("%s: Cannot find out VCO for Frequency (%luHz).\n",
		       __func__, freq_hz);
		return -1;
	}

	div->no = no;

	best_diff_khz = vco_khz;
	for (nr = 1; nr < max_nr && best_diff_khz; nr++) {
		fref_khz = ref_khz / nr;
		if (fref_khz < FREF_MIN_KHZ)
			break;
		if (fref_khz > FREF_MAX_KHZ)
			continue;

		nf = vco_khz / fref_khz;
		if (nf >= max_nf)
			continue;
		diff_khz = vco_khz - nf * fref_khz;
		if (nf + 1 < max_nf && diff_khz > fref_khz / 2) {
			nf++;
			diff_khz = fref_khz - diff_khz;
		}

		if (diff_khz >= best_diff_khz)
			continue;

		best_diff_khz = diff_khz;
		div->nr = nr;
		div->nf = nf;
	}

	if (best_diff_khz > 4 * 1000) {
		printf("%s:Fail to match output freq %lu,best_is %u Hz\n",
		       __func__, freq_hz, best_diff_khz * 1000);
		return -EINVAL;
	}

	return 0;
}

/* Get pll rate by id */
static uint32_t rkclk_pll_get_rate(struct rk3368_cru *cru,
				   enum rk3368_pll_id pll_id)
{
	uint32_t nr, no, nf;
	uint32_t con;
	struct rk3368_pll *pll = &cru->pll[pll_id];

	con = readl(&pll->con3);

	switch ((con & PLL_MODE_MASK) >> PLL_MODE_SHIFT) {
	case PLL_MODE_SLOW:
		return OSC_HZ;
	case PLL_MODE_NORMAL:
		con = readl(&pll->con0);
		no = ((con & PLL_OD_MASK) >> PLL_OD_SHIFT) + 1;
		nr = ((con & PLL_NR_MASK) >> PLL_NR_SHIFT) + 1;
		con = readl(&pll->con1);
		nf = ((con & PLL_NF_MASK) >> PLL_NF_SHIFT) + 1;

		return (24 * nf / (nr * no)) * 1000000;
	case PLL_MODE_DEEP_SLOW:
	default:
		return 32768;
	}
}

static int rkclk_set_pll(struct rk3368_cru *cru, enum rk3368_pll_id pll_id,
			 const struct pll_div *div)
{
	struct rk3368_pll *pll = &cru->pll[pll_id];
	/* All PLLs have same VCO and output frequency range restrictions*/
	uint vco_hz = OSC_HZ / 1000 * div->nf / div->nr * 1000;
	uint output_hz = vco_hz / div->no;

	debug("PLL at %p: nf=%d, nr=%d, no=%d, vco=%u Hz, output=%u Hz\n",
	      pll, div->nf, div->nr, div->no, vco_hz, output_hz);

	/* enter slow mode and reset pll */
	rk_clrsetreg(&pll->con3, PLL_MODE_MASK | PLL_RESET_MASK,
		     PLL_RESET << PLL_RESET_SHIFT);

	rk_clrsetreg(&pll->con0, PLL_NR_MASK | PLL_OD_MASK,
		     ((div->nr - 1) << PLL_NR_SHIFT) |
		     ((div->no - 1) << PLL_OD_SHIFT));
	writel((div->nf - 1) << PLL_NF_SHIFT, &pll->con1);
	/*
	 * BWADJ should be set to NF / 2 to ensure the nominal bandwidth.
	 * Compare the RK3368 TRM, section "3.6.4 PLL Bandwidth Adjustment".
	 */
	if (div->nb)
		clrsetbits_le32(&pll->con2, PLL_BWADJ_MASK, div->nb - 1);
	else
		clrsetbits_le32(&pll->con2, PLL_BWADJ_MASK, (div->nf >> 1) - 1);

	udelay(10);

	/* return from reset */
	rk_clrreg(&pll->con3, PLL_RESET_MASK);

	/* waiting for pll lock */
	while (!(readl(&pll->con1) & PLL_LOCK_STA))
		udelay(1);

	rk_clrsetreg(&pll->con3, PLL_MODE_MASK,
		     PLL_MODE_NORMAL << PLL_MODE_SHIFT);

	return 0;
}

#if !IS_ENABLED(CONFIG_SPL_BUILD) || CONFIG_IS_ENABLED(MMC_SUPPORT)
static ulong rk3368_mmc_get_clk(struct rk3368_cru *cru, uint clk_id)
{
	u32 div, con, con_id, rate;
	u32 pll_rate;

	switch (clk_id) {
	case HCLK_SDMMC:
		con_id = 50;
		break;
	case HCLK_EMMC:
		con_id = 51;
		break;
	case SCLK_SDIO0:
		con_id = 48;
		break;
	default:
		return -EINVAL;
	}

	con = readl(&cru->clksel_con[con_id]);
	switch (con & MMC_PLL_SEL_MASK) {
	case MMC_PLL_SEL_GPLL:
		pll_rate = rkclk_pll_get_rate(cru, GPLL);
		break;
	case MMC_PLL_SEL_24M:
		pll_rate = OSC_HZ;
		break;
	case MMC_PLL_SEL_CPLL:
		pll_rate = rkclk_pll_get_rate(cru, CPLL);
		break;
	case MMC_PLL_SEL_USBPHY_480M:
	default:
		return -EINVAL;
	}
	div = (con & MMC_CLK_DIV_MASK) >> MMC_CLK_DIV_SHIFT;
	rate = DIV_TO_RATE(pll_rate, div);

	debug("%s: raw rate %d (post-divide by 2)\n", __func__, rate);
	return rate >> 1;
}

static ulong rk3368_mmc_find_best_rate_and_parent(struct clk *clk,
						  ulong rate,
						  u32 *best_mux,
						  u32 *best_div)
{
	int i;
	ulong best_rate = 0;
	const ulong MHz = 1000000;
	const struct {
		u32 mux;
		ulong rate;
	} parents[] = {
		{ .mux = MMC_PLL_SEL_CPLL, .rate = CPLL_HZ },
		{ .mux = MMC_PLL_SEL_GPLL, .rate = GPLL_HZ },
		{ .mux = MMC_PLL_SEL_24M,  .rate = 24 * MHz }
	};

	debug("%s: target rate %ld\n", __func__, rate);
	for (i = 0; i < ARRAY_SIZE(parents); ++i) {
		/*
		 * Find the largest rate no larger than the target-rate for
		 * the current parent.
		 */
		ulong parent_rate = parents[i].rate;
		u32 div = DIV_ROUND_UP(parent_rate, rate);
		u32 adj_div = div;
		ulong new_rate = parent_rate / adj_div;

		debug("%s: rate %ld, parent-mux %d, parent-rate %ld, div %d\n",
		      __func__, rate, parents[i].mux, parents[i].rate, div);

		/* Skip, if not representable */
		if ((div - 1) > MMC_CLK_DIV_MASK)
			continue;

		/* Skip, if we already have a better (or equal) solution */
		if (new_rate <= best_rate)
			continue;

		/* This is our new best rate. */
		best_rate = new_rate;
		*best_mux = parents[i].mux;
		*best_div = div - 1;
	}

	debug("%s: best_mux = %x, best_div = %d, best_rate = %ld\n",
	      __func__, *best_mux, *best_div, best_rate);

	return best_rate;
}

static ulong rk3368_mmc_set_clk(struct clk *clk, ulong rate)
{
	struct rk3368_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3368_cru *cru = priv->cru;
	ulong clk_id = clk->id;
	u32 con_id, mux = 0, div = 0;

	/* Find the best parent and rate */
	rk3368_mmc_find_best_rate_and_parent(clk, rate << 1, &mux, &div);

	switch (clk_id) {
	case HCLK_SDMMC:
		con_id = 50;
		break;
	case HCLK_EMMC:
		con_id = 51;
		break;
	case SCLK_SDIO0:
		con_id = 48;
		break;
	default:
		return -EINVAL;
	}

	rk_clrsetreg(&cru->clksel_con[con_id],
		     MMC_PLL_SEL_MASK | MMC_CLK_DIV_MASK,
		     mux | div);

	return rk3368_mmc_get_clk(cru, clk_id);
}
#endif

#if IS_ENABLED(CONFIG_TPL_BUILD)
static ulong rk3368_ddr_set_clk(struct rk3368_cru *cru, ulong set_rate)
{
	const struct pll_div *dpll_cfg = NULL;
	const ulong MHz = 1000000;

	/* Fout = ((Fin /NR) * NF )/ NO */
	static const struct pll_div dpll_1200 = PLL_DIVISORS(1200 * MHz, 1, 1);
	static const struct pll_div dpll_1332 =	PLL_DIVISORS(1332 * MHz, 2, 1);
	static const struct pll_div dpll_1600 =	PLL_DIVISORS(1600 * MHz, 3, 2);

	switch (set_rate) {
	case 1200*MHz:
		dpll_cfg = &dpll_1200;
		break;
	case 1332*MHz:
		dpll_cfg = &dpll_1332;
		break;
	case 1600*MHz:
		dpll_cfg = &dpll_1600;
		break;
	default:
		pr_err("Unsupported SDRAM frequency!,%ld\n", set_rate);
	}
	rkclk_set_pll(cru, DPLL, dpll_cfg);

	return set_rate;
}
#endif

#if CONFIG_IS_ENABLED(GMAC_ROCKCHIP)
static ulong rk3368_gmac_set_clk(struct rk3368_cru *cru, ulong set_rate)
{
	ulong ret;

	/*
	 * The gmac clock can be derived either from an external clock
	 * or can be generated from internally by a divider from SCLK_MAC.
	 */
	if (readl(&cru->clksel_con[43]) & GMAC_MUX_SEL_EXTCLK) {
		/* An external clock will always generate the right rate... */
		ret = set_rate;
	} else {
		u32 con = readl(&cru->clksel_con[43]);
		ulong pll_rate;
		u8 div;

		if (((con >> GMAC_PLL_SHIFT) & GMAC_PLL_MASK) ==
		    GMAC_PLL_SELECT_GENERAL)
			pll_rate = GPLL_HZ;
		else if (((con >> GMAC_PLL_SHIFT) & GMAC_PLL_MASK) ==
			 GMAC_PLL_SELECT_CODEC)
			pll_rate = CPLL_HZ;
		else
			/* CPLL is not set */
			return -EPERM;

		div = DIV_ROUND_UP(pll_rate, set_rate) - 1;
		if (div <= 0x1f)
			rk_clrsetreg(&cru->clksel_con[43], GMAC_DIV_CON_MASK,
				     div << GMAC_DIV_CON_SHIFT);
		else
			debug("Unsupported div for gmac:%d\n", div);

		return DIV_TO_RATE(pll_rate, div);
	}

	return ret;
}
#endif

/*
 * RK3368 SPI clocks have a common divider-width (7 bits) and a single bit
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
 */
static const struct spi_clkreg spi_clkregs[] = {
	[0] = { .reg = 45, .div_shift = 0, .sel_shift = 7, },
	[1] = { .reg = 45, .div_shift = 8, .sel_shift = 15, },
	[2] = { .reg = 46, .div_shift = 8, .sel_shift = 15, },
};

static inline u32 extract_bits(u32 val, unsigned width, unsigned shift)
{
	return (val >> shift) & ((1 << width) - 1);
}

static ulong rk3368_spi_get_clk(struct rk3368_cru *cru, ulong clk_id)
{
	const struct spi_clkreg *spiclk = NULL;
	u32 div, val;

	switch (clk_id) {
	case SCLK_SPI0 ... SCLK_SPI2:
		spiclk = &spi_clkregs[clk_id - SCLK_SPI0];
		break;

	default:
		pr_err("%s: SPI clk-id %ld not supported\n", __func__, clk_id);
		return -EINVAL;
	}

	val = readl(&cru->clksel_con[spiclk->reg]);
	div = extract_bits(val, 7, spiclk->div_shift);

	debug("%s: div 0x%x\n", __func__, div);
	return DIV_TO_RATE(GPLL_HZ, div);
}

static ulong rk3368_spi_set_clk(struct rk3368_cru *cru, ulong clk_id, uint hz)
{
	const struct spi_clkreg *spiclk = NULL;
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(GPLL_HZ, hz);
	assert(src_clk_div < 127);

	switch (clk_id) {
	case SCLK_SPI0 ... SCLK_SPI2:
		spiclk = &spi_clkregs[clk_id - SCLK_SPI0];
		break;

	default:
		pr_err("%s: SPI clk-id %ld not supported\n", __func__, clk_id);
		return -EINVAL;
	}

	rk_clrsetreg(&cru->clksel_con[spiclk->reg],
		     ((0x7f << spiclk->div_shift) |
		      (0x1 << spiclk->sel_shift)),
		     ((src_clk_div << spiclk->div_shift) |
		      (1 << spiclk->sel_shift)));

	return rk3368_spi_get_clk(cru, clk_id);
}

static ulong rk3368_saradc_get_clk(struct rk3368_cru *cru)
{
	u32 div, val;

	val = readl(&cru->clksel_con[25]);
	div = bitfield_extract(val, CLK_SARADC_DIV_CON_SHIFT,
			       CLK_SARADC_DIV_CON_WIDTH);

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3368_saradc_set_clk(struct rk3368_cru *cru, uint hz)
{
	int src_clk_div;

	src_clk_div = DIV_ROUND_UP(OSC_HZ, hz) - 1;
	assert(src_clk_div < 128);

	rk_clrsetreg(&cru->clksel_con[25],
		     CLK_SARADC_DIV_CON_MASK,
		     src_clk_div << CLK_SARADC_DIV_CON_SHIFT);

	return rk3368_saradc_get_clk(cru);
}

static ulong rk3368_bus_get_clk(struct rk3368_cru *cru, ulong clk_id)
{
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_BUS:
		con = readl(&cru->clksel_con[8]);
		div = (con & ACLK_BUS_DIV_CON_MASK) >> ACLK_BUS_DIV_CON_SHIFT;
		parent = rkclk_pll_get_rate(cru, GPLL);
		break;
	case HCLK_BUS:
		con = readl(&cru->clksel_con[8]);
		div = (con & HCLK_BUS_DIV_CON_MASK) >> HCLK_BUS_DIV_CON_SHIFT;
		parent = rk3368_bus_get_clk(cru, ACLK_BUS);
		break;
	case PCLK_BUS:
	case PCLK_PWM0:
	case PCLK_PWM1:
	case PCLK_I2C0:
	case PCLK_I2C1:
		con = readl(&cru->clksel_con[8]);
		div = (con & PCLK_BUS_DIV_CON_MASK) >> PCLK_BUS_DIV_CON_SHIFT;
		parent = rk3368_bus_get_clk(cru, ACLK_BUS);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3368_bus_set_clk(struct rk3368_cru *cru,
				ulong clk_id, ulong hz)
{
	int src_clk_div;

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	switch (clk_id) {
	case ACLK_BUS:
		src_clk_div = DIV_ROUND_UP(rkclk_pll_get_rate(cru, GPLL), hz);
		assert(src_clk_div - 1 < 31);
		rk_clrsetreg(&cru->clksel_con[8],
			     CLK_BUS_PLL_SEL_MASK | ACLK_BUS_DIV_CON_MASK,
			     CLK_BUS_PLL_SEL_GPLL << CLK_BUS_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_BUS_DIV_CON_SHIFT);
		break;
	case HCLK_BUS:
		src_clk_div = DIV_ROUND_UP(rk3368_bus_get_clk(cru,
							      ACLK_BUS),
					   hz);
		assert(src_clk_div - 1 < 3);
		rk_clrsetreg(&cru->clksel_con[8],
			     HCLK_BUS_DIV_CON_MASK,
			     (src_clk_div - 1) << HCLK_BUS_DIV_CON_SHIFT);
		break;
	case PCLK_BUS:
		src_clk_div = DIV_ROUND_UP(rk3368_bus_get_clk(cru,
							      ACLK_BUS),
					   hz);
		assert(src_clk_div - 1 < 3);
		rk_clrsetreg(&cru->clksel_con[8],
			     PCLK_BUS_DIV_CON_MASK,
			     (src_clk_div - 1) << PCLK_BUS_DIV_CON_SHIFT);
		break;
	default:
		printf("do not support this bus freq\n");
		return -EINVAL;
	}
	return rk3368_bus_get_clk(cru, clk_id);
}

static ulong rk3368_peri_get_clk(struct rk3368_cru *cru, ulong clk_id)
{
	u32 div, con, parent;

	switch (clk_id) {
	case ACLK_PERI:
		con = readl(&cru->clksel_con[9]);
		div = (con & ACLK_PERI_DIV_CON_MASK) >> ACLK_PERI_DIV_CON_SHIFT;
		parent = rkclk_pll_get_rate(cru, GPLL);
		break;
	case HCLK_PERI:
		con = readl(&cru->clksel_con[9]);
		div = (con & HCLK_PERI_DIV_CON_MASK) >> HCLK_PERI_DIV_CON_SHIFT;
		parent = rk3368_peri_get_clk(cru, ACLK_PERI);
		break;
	case PCLK_PERI:
	case PCLK_I2C2:
	case PCLK_I2C3:
	case PCLK_I2C4:
	case PCLK_I2C5:
		con = readl(&cru->clksel_con[9]);
		div = (con & PCLK_PERI_DIV_CON_MASK) >> PCLK_PERI_DIV_CON_SHIFT;
		parent = rk3368_peri_get_clk(cru, ACLK_PERI);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3368_peri_set_clk(struct rk3368_cru *cru,
				 ulong clk_id, ulong hz)
{
	int src_clk_div;

	/*
	 * select gpll as pd_bus bus clock source and
	 * set up dependent divisors for PCLK/HCLK and ACLK clocks.
	 */
	switch (clk_id) {
	case ACLK_PERI:
		src_clk_div = DIV_ROUND_UP(rkclk_pll_get_rate(cru, GPLL), hz);
		assert(src_clk_div - 1 < 31);
		rk_clrsetreg(&cru->clksel_con[9],
			     CLK_PERI_PLL_SEL_MASK | ACLK_PERI_DIV_CON_MASK,
			     CLK_PERI_PLL_SEL_GPLL << CLK_PERI_PLL_SEL_SHIFT |
			     (src_clk_div - 1) << ACLK_PERI_DIV_CON_SHIFT);
		break;
	case HCLK_PERI:
		src_clk_div = DIV_ROUND_UP(rk3368_peri_get_clk(cru,
							       ACLK_PERI),
					   hz);
		assert(src_clk_div - 1 < 3);
		rk_clrsetreg(&cru->clksel_con[9],
			     HCLK_PERI_DIV_CON_MASK,
			     (src_clk_div - 1) << HCLK_PERI_DIV_CON_SHIFT);
		break;
	case PCLK_PERI:
		src_clk_div = DIV_ROUND_UP(rk3368_peri_get_clk(cru,
							       ACLK_PERI),
					   hz);
		assert(src_clk_div - 1 < 3);
		rk_clrsetreg(&cru->clksel_con[9],
			     PCLK_PERI_DIV_CON_MASK,
			     (src_clk_div - 1) << PCLK_PERI_DIV_CON_SHIFT);
		break;
	default:
		printf("do not support this bus freq\n");
		return -EINVAL;
	}

	return rk3368_peri_get_clk(cru, clk_id);
}

#if !defined(CONFIG_SPL_BUILD)
static ulong rk3368_vop_get_clk(struct rk3368_cru *cru,  int clk_id)
{
	u32 div, con, parent, sel;

	switch (clk_id) {
	case DCLK_VOP:
		con = readl(&cru->clksel_con[20]);
		div = con & DCLK_VOP_DIV_MASK;
		parent = rkclk_pll_get_rate(cru, NPLL);
		break;
	case ACLK_VOP:
		con = readl(&cru->clksel_con[19]);
		div = con & ACLK_VOP_DIV_MASK;
		sel =  (con & (ACLK_VOP_PLL_SEL_MASK <<
			ACLK_VOP_PLL_SEL_SHIFT)) >>
			ACLK_VOP_PLL_SEL_SHIFT;
		if (sel == ACLK_VOP_PLL_SEL_CPLL)
			parent = rkclk_pll_get_rate(cru, CPLL);
		else if (ACLK_VOP_PLL_SEL_GPLL)
			parent = rkclk_pll_get_rate(cru, GPLL);
		else
			parent = 480000000;
		break;
	default:
		return -EINVAL;
	}

	return DIV_TO_RATE(parent, div);
}

static ulong rk3368_vop_set_clk(struct rk3368_cru *cru, int clk_id, uint hz)
{
	struct pll_div npll_config = {0};
	u32 lcdc_div;
	int ret;

	switch (clk_id) {
	case DCLK_VOP:
		if (!(NPLL_HZ % hz)) {
			rkclk_set_pll(cru, NPLL, rkclk_get_pll_config(NPLL_HZ));
			lcdc_div = NPLL_HZ / hz;
		} else {
			ret = pll_para_config(hz, &npll_config, &lcdc_div);
			if (ret)
				return ret;

			rkclk_set_pll(cru, NPLL, &npll_config);
		}
		/* vop dclk source clk: npll,dclk_div: 1 */
		rk_clrsetreg(&cru->clksel_con[20],
			     (DCLK_VOP_PLL_SEL_MASK << DCLK_VOP_PLL_SEL_SHIFT) |
			     (DCLK_VOP_DIV_MASK << DCLK_VOP_DIV_SHIFT),
			     (DCLK_VOP_PLL_SEL_NPLL << DCLK_VOP_PLL_SEL_SHIFT) |
			     (lcdc_div - 1) << DCLK_VOP_DIV_SHIFT);
		break;
	case ACLK_VOP:
		if ((rkclk_pll_get_rate(cru, CPLL) % hz) == 0) {
			lcdc_div = rkclk_pll_get_rate(cru, CPLL) / hz;
			rk_clrsetreg(&cru->clksel_con[19],
				     (ACLK_VOP_PLL_SEL_MASK <<
				     ACLK_VOP_PLL_SEL_SHIFT) |
				     (ACLK_VOP_DIV_MASK <<
				     ACLK_VOP_DIV_SHIFT),
				     (ACLK_VOP_PLL_SEL_CPLL <<
				     ACLK_VOP_PLL_SEL_SHIFT) |
				     (lcdc_div - 1) <<
				     ACLK_VOP_DIV_SHIFT);
		} else {
			lcdc_div = rkclk_pll_get_rate(cru, GPLL) / hz;
			rk_clrsetreg(&cru->clksel_con[19],
				     (ACLK_VOP_PLL_SEL_MASK <<
				     ACLK_VOP_PLL_SEL_SHIFT) |
				     (ACLK_VOP_DIV_MASK <<
				     ACLK_VOP_DIV_SHIFT),
				     (ACLK_VOP_PLL_SEL_GPLL <<
				     ACLK_VOP_PLL_SEL_SHIFT) |
				     (lcdc_div - 1) <<
				     ACLK_VOP_DIV_SHIFT);
		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static ulong rk3368_alive_get_clk(struct rk3368_clk_priv *priv)
{
	struct rk3368_cru *cru = priv->cru;
	u32 div, con, parent;

	con = readl(&cru->clksel_con[10]);
	div = (con & PCLK_ALIVE_DIV_CON_MASK) >>
	      PCLK_ALIVE_DIV_CON_SHIFT;
	parent = GPLL_HZ;
	return DIV_TO_RATE(parent, div);
}

static ulong rk3368_crypto_get_rate(struct rk3368_clk_priv *priv)
{
	struct rk3368_cru *cru = priv->cru;
	u32 div, val;

	val = readl(&cru->clksel_con[10]);
	div = (val & CLK_CRYPTO_DIV_CON_MASK) >> CLK_CRYPTO_DIV_CON_SHIFT;

	return DIV_TO_RATE(rk3368_bus_get_clk(priv->cru, ACLK_BUS), div);
}

static ulong rk3368_crypto_set_rate(struct rk3368_clk_priv *priv,
				    uint hz)
{
	struct rk3368_cru *cru = priv->cru;
	int src_clk_div;
	uint p_rate;

	p_rate = rk3368_bus_get_clk(priv->cru, ACLK_BUS);
	src_clk_div = DIV_ROUND_UP(p_rate, hz) - 1;
	assert(src_clk_div < 3);

	rk_clrsetreg(&cru->clksel_con[10],
		     CLK_CRYPTO_DIV_CON_MASK,
		     src_clk_div << CLK_CRYPTO_DIV_CON_SHIFT);

	return rk3368_crypto_get_rate(priv);
}
#endif

static ulong rk3368_armclk_set_clk(struct rk3368_clk_priv *priv,
				   int clk_id, ulong hz)
{
	struct rk3368_cru *cru = priv->cru;
	const struct rockchip_cpu_rate_table *rate;
	struct pll_div pll_config = {0};
	ulong old_rate;
	u32 pll_div, pll_id, con_id;
	int ret;

	rate = rockchip_get_cpu_settings(rk3368_cpu_rates, hz);
	if (!rate) {
		printf("%s unsupported rate\n", __func__);
		return -EINVAL;
	}

	/*
	 * select apll as cpu/core clock pll source and
	 * set up dependent divisors for PERI and ACLK clocks.
	 * core hz : apll = 1:1
	 */

	ret = pll_para_config(hz, &pll_config, &pll_div);
	if (ret)
		return ret;

	if (clk_id == ARMCLKB) {
		old_rate = rkclk_pll_get_rate(priv->cru, APLLB);
		pll_id = APLLB;
		con_id = 0;
	} else {
		old_rate = rkclk_pll_get_rate(priv->cru, APLLL);
		pll_id = APLLL;
		con_id = 2;
	}

	if (old_rate > hz) {
		ret = rkclk_set_pll(priv->cru, pll_id, &pll_config);
		rk_clrsetreg(&cru->clksel_con[con_id],
			     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
			     CORE_CLK_PLL_SEL_APLL << CORE_CLK_PLL_SEL_SHIFT |
			     0 << CORE_DIV_CON_SHIFT);
		rk_clrsetreg(&cru->clksel_con[con_id + 1],
			     CORE_ACLK_DIV_MASK | CORE_DBG_DIV_MASK,
			     rate->aclk_div << CORE_ACLK_DIV_SHIFT |
			     rate->pclk_div << CORE_DBG_DIV_SHIFT);
	} else if (old_rate < hz) {
		rk_clrsetreg(&cru->clksel_con[con_id],
			     CORE_CLK_PLL_SEL_MASK | CORE_DIV_CON_MASK,
			     CORE_CLK_PLL_SEL_APLL << CORE_CLK_PLL_SEL_SHIFT |
			     0 << CORE_DIV_CON_SHIFT);
		rk_clrsetreg(&cru->clksel_con[con_id + 1],
			     CORE_ACLK_DIV_MASK | CORE_DBG_DIV_MASK,
			     rate->aclk_div << CORE_ACLK_DIV_SHIFT |
			     rate->pclk_div << CORE_DBG_DIV_SHIFT);
		ret = rkclk_set_pll(priv->cru, pll_id, &pll_config);
	}

	return rkclk_pll_get_rate(priv->cru, pll_id);
}

static ulong rk3368_clk_get_rate(struct clk *clk)
{
	struct rk3368_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate = 0;

	debug("%s: id %ld\n", __func__, clk->id);
	switch (clk->id) {
	case PLL_APLLB:
	case PLL_APLLL:
	case PLL_DPLL:
	case PLL_CPLL:
	case PLL_GPLL:
	case PLL_NPLL:
		rate = rkclk_pll_get_rate(priv->cru, clk->id - 1);
		break;
	case ARMCLKB:
		rate = rkclk_pll_get_rate(priv->cru, APLLB);
		break;
	case ARMCLKL:
		rate = rkclk_pll_get_rate(priv->cru, APLLL);
		break;
	case SCLK_SPI0 ... SCLK_SPI2:
		rate = rk3368_spi_get_clk(priv->cru, clk->id);
		break;
	case ACLK_BUS:
	case HCLK_BUS:
	case PCLK_BUS:
	case PCLK_PWM0:
	case PCLK_PWM1:
	case PCLK_I2C0:
	case PCLK_I2C1:
		rate = rk3368_bus_get_clk(priv->cru, clk->id);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
	case PCLK_I2C2:
	case PCLK_I2C3:
	case PCLK_I2C4:
	case PCLK_I2C5:
		rate = rk3368_peri_get_clk(priv->cru, clk->id);
		break;
#if !IS_ENABLED(CONFIG_SPL_BUILD) || CONFIG_IS_ENABLED(MMC_SUPPORT)
	case HCLK_SDMMC:
	case HCLK_EMMC:
		rate = rk3368_mmc_get_clk(priv->cru, clk->id);
		break;
#endif
	case SCLK_SARADC:
		rate = rk3368_saradc_get_clk(priv->cru);
		break;
#if !defined(CONFIG_SPL_BUILD)
	case ACLK_VOP:
	case DCLK_VOP:
		rate =  rk3368_vop_get_clk(priv->cru, clk->id);
		break;
	case PCLK_WDT:
		rate = rk3368_alive_get_clk(priv);
		break;
	case SCLK_CRYPTO:
		rate = rk3368_crypto_get_rate(priv);
		break;
#endif
	default:
		return -ENOENT;
	}
	return rate;
}

static ulong rk3368_clk_set_rate(struct clk *clk, ulong rate)
{
	__maybe_unused struct rk3368_clk_priv *priv = dev_get_priv(clk->dev);
	struct pll_div pll_config = {0};
	u32 pll_div;
	ulong ret = 0;

	switch (clk->id) {
	case PLL_APLLB:
	case PLL_APLLL:
	case PLL_CPLL:
	case PLL_GPLL:
	case PLL_NPLL:
		ret = pll_para_config(rate, &pll_config, &pll_div);
		if (ret)
			return ret;

		ret = rkclk_set_pll(priv->cru, clk->id - 1, &pll_config);
		break;
	case ARMCLKB:
		if (priv->armbclk_hz)
			ret = rk3368_armclk_set_clk(priv, clk->id, rate);
		priv->armbclk_hz = rate;
		break;
	case ARMCLKL:
		if (priv->armlclk_hz)
			ret = rk3368_armclk_set_clk(priv, clk->id, rate);
		priv->armlclk_hz = rate;
		break;
	case SCLK_SPI0 ... SCLK_SPI2:
		ret = rk3368_spi_set_clk(priv->cru, clk->id, rate);
		break;
#if IS_ENABLED(CONFIG_TPL_BUILD)
	case SCLK_DDRCLK:
		ret = rk3368_ddr_set_clk(priv->cru, rate);
		break;
#endif
	case ACLK_BUS:
	case HCLK_BUS:
	case PCLK_BUS:
		rate = rk3368_bus_set_clk(priv->cru, clk->id, rate);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		rate = rk3368_peri_set_clk(priv->cru, clk->id, rate);
		break;
#if !IS_ENABLED(CONFIG_SPL_BUILD) || CONFIG_IS_ENABLED(MMC_SUPPORT)
	case HCLK_SDMMC:
	case HCLK_EMMC:
		ret = rk3368_mmc_set_clk(clk, rate);
		break;
#endif
#if CONFIG_IS_ENABLED(GMAC_ROCKCHIP)
	case SCLK_MAC:
		/* select the external clock */
		ret = rk3368_gmac_set_clk(priv->cru, rate);
		break;
#endif
	case SCLK_SARADC:
		ret =  rk3368_saradc_set_clk(priv->cru, rate);
		break;
#if !defined(CONFIG_SPL_BUILD)
	case ACLK_VOP:
	case DCLK_VOP:
		ret =  rk3368_vop_set_clk(priv->cru, clk->id, rate);
		break;
	case ACLK_CCI_PRE:
		ret =  0;
		break;
	case SCLK_CRYPTO:
		ret = rk3368_crypto_set_rate(priv, rate);
		break;
#endif
	default:
		return -ENOENT;
	}

	return ret;
}

static int __maybe_unused rk3368_gmac_set_parent(struct clk *clk, struct clk *parent)
{
	struct rk3368_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3368_cru *cru = priv->cru;
	const char *clock_output_name;
	int ret;

	/*
	 * If the requested parent is in the same clock-controller and
	 * the id is SCLK_MAC ("sclk_mac"), switch to the internal
	 * clock.
	 */
	if ((parent->dev == clk->dev) && (parent->id == SCLK_MAC)) {
		debug("%s: switching GAMC to SCLK_MAC\n", __func__);
		rk_clrreg(&cru->clksel_con[43], GMAC_MUX_SEL_EXTCLK);
		return 0;
	}

	/*
	 * Otherwise, we need to check the clock-output-names of the
	 * requested parent to see if the requested id is "ext_gmac".
	 */
	ret = dev_read_string_index(parent->dev, "clock-output-names",
				    parent->id, &clock_output_name);
	if (ret < 0)
		return -ENODATA;

	/* If this is "ext_gmac", switch to the external clock input */
	if (!strcmp(clock_output_name, "ext_gmac")) {
		debug("%s: switching GMAC to external clock\n", __func__);
		rk_setreg(&cru->clksel_con[43], GMAC_MUX_SEL_EXTCLK);
		return 0;
	}

	return -EINVAL;
}

static int __maybe_unused rk3368_clk_set_parent(struct clk *clk, struct clk *parent)
{
	switch (clk->id) {
	case SCLK_MAC:
		return rk3368_gmac_set_parent(clk, parent);
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

int rk3368_mmc_get_phase(struct clk *clk)
{
	struct rk3368_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3368_cru *cru = priv->cru;
	u32 raw_value, delay_num;
	u16 degrees = 0;
	ulong rate;

	rate = rk3368_clk_get_rate(clk);

	if (rate < 0)
		return rate;

	if (clk->id == SCLK_EMMC_SAMPLE)
		raw_value = readl(&cru->emmc_con[1]);
	else if (clk->id == SCLK_SDMMC_SAMPLE)
		raw_value = readl(&cru->sdmmc_con[1]);
	else
		raw_value = readl(&cru->sdio0_con[1]);

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

int rk3368_mmc_set_phase(struct clk *clk, u32 degrees)
{
	struct rk3368_clk_priv *priv = dev_get_priv(clk->dev);
	struct rk3368_cru *cru = priv->cru;
	u8 nineties, remainder, delay_num;
	u32 raw_value, delay;
	ulong rate;

	rate = rk3368_clk_get_rate(clk);

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
		writel(raw_value | 0xffff0000, &cru->sdio0_con[1]);

	debug("mmc set_phase(%d) delay_nums=%u reg=%#x actual_degrees=%d\n",
	      degrees, delay_num, raw_value, rk3368_mmc_get_phase(clk));

	return 0;
}

static int rk3368_clk_get_phase(struct clk *clk)
{
	int ret;

	debug("%s %ld\n", __func__, clk->id);
	switch (clk->id) {
	case SCLK_EMMC_SAMPLE:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO0_SAMPLE:
		ret = rk3368_mmc_get_phase(clk);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static int rk3368_clk_set_phase(struct clk *clk, int degrees)
{
	int ret;

	debug("%s %ld\n", __func__, clk->id);
	switch (clk->id) {
	case SCLK_EMMC_SAMPLE:
	case SCLK_SDMMC_SAMPLE:
	case SCLK_SDIO0_SAMPLE:
		ret = rk3368_mmc_set_phase(clk, degrees);
		break;
	default:
		return -ENOENT;
	}

	return ret;
}

static struct clk_ops rk3368_clk_ops = {
	.get_rate = rk3368_clk_get_rate,
	.set_rate = rk3368_clk_set_rate,
	.get_phase = rk3368_clk_get_phase,
	.set_phase = rk3368_clk_set_phase,
#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
	.set_parent = rk3368_clk_set_parent,
#endif
};

#if IS_ENABLED(CONFIG_SPL_BUILD) || IS_ENABLED(CONFIG_TPL_BUILD)
static void rkclk_init(struct rk3368_cru *cru)
{
	u32 apllb, aplll, dpll, cpll, gpll;

	rkclk_set_pll(cru, APLLB, &apll_b_init_cfg);
	rkclk_set_pll(cru, APLLL, &apll_l_init_cfg);
#if !defined(CONFIG_TPL_BUILD)
	/*
	 * If we plan to return to the boot ROM, we can't increase the
	 * GPLL rate from the SPL stage.
	 */
	rkclk_set_pll(cru, GPLL, &gpll_init_cfg);
	rkclk_set_pll(cru, CPLL, &cpll_init_cfg);
#endif
rk_clrsetreg(&cru->clksel_con[37],  (1 << 8), 1 << 8);
	apllb = rkclk_pll_get_rate(cru, APLLB);
	aplll = rkclk_pll_get_rate(cru, APLLL);
	dpll = rkclk_pll_get_rate(cru, DPLL);
	cpll = rkclk_pll_get_rate(cru, CPLL);
	gpll = rkclk_pll_get_rate(cru, GPLL);

	debug("%s apllb(%d) apll(%d) dpll(%d) cpll(%d) gpll(%d)\n",
	      __func__, apllb, aplll, dpll, cpll, gpll);
}
#endif

static int rk3368_clk_probe(struct udevice *dev)
{
	struct rk3368_clk_priv __maybe_unused *priv = dev_get_priv(dev);
	int ret;
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct rk3368_clk_plat *plat = dev_get_platdata(dev);

	priv->cru = map_sysmem(plat->dtd.reg[0], plat->dtd.reg[1]);
#endif
	priv->sync_kernel = false;
	if (!priv->armlclk_enter_hz)
		priv->armlclk_enter_hz = rkclk_pll_get_rate(priv->cru, APLLL);
	if (!priv->armbclk_enter_hz)
		priv->armbclk_enter_hz = rkclk_pll_get_rate(priv->cru, APLLB);
#if IS_ENABLED(CONFIG_SPL_BUILD) || IS_ENABLED(CONFIG_TPL_BUILD)
	rkclk_init(priv->cru);
#endif
	rkclk_set_pll(priv->cru, NPLL, rkclk_get_pll_config(NPLL_HZ));
	if (!priv->armlclk_init_hz)
		priv->armlclk_init_hz = rkclk_pll_get_rate(priv->cru, APLLL);
	if (!priv->armbclk_init_hz)
		priv->armbclk_init_hz = rkclk_pll_get_rate(priv->cru, APLLB);
	/* Process 'assigned-{clocks/clock-parents/clock-rates}' properties */
	ret = clk_set_defaults(dev);
	if (ret)
		debug("%s clk_set_defaults failed %d\n", __func__, ret);
	else
		priv->sync_kernel = true;
	return 0;
}

static int rk3368_clk_ofdata_to_platdata(struct udevice *dev)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	struct rk3368_clk_priv *priv = dev_get_priv(dev);

	priv->cru = dev_read_addr_ptr(dev);
#endif

	return 0;
}

static int rk3368_clk_bind(struct udevice *dev)
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
		priv->glb_srst_fst_value = offsetof(struct rk3368_cru,
						    glb_srst_fst_val);
		priv->glb_srst_snd_value = offsetof(struct rk3368_cru,
						    glb_srst_snd_val);
		sys_child->priv = priv;
	}

	ret = device_bind_driver_to_node(dev, "rockchip_reset", "reset",
					 dev_ofnode(dev), &sf_child);
	if (ret) {
		debug("Warning: No rockchip reset driver: ret=%d\n", ret);
	} else {
		sf_priv = malloc(sizeof(struct softreset_reg));
		sf_priv->sf_reset_offset = offsetof(struct rk3368_cru,
						    softrst_con[0]);
		sf_priv->sf_reset_num = 15;
		sf_child->priv = sf_priv;
	}

	return 0;
}

static const struct udevice_id rk3368_clk_ids[] = {
	{ .compatible = "rockchip,rk3368-cru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3368_cru) = {
	.name		= "rockchip_rk3368_cru",
	.id		= UCLASS_CLK,
	.of_match	= rk3368_clk_ids,
	.priv_auto_alloc_size = sizeof(struct rk3368_clk_priv),
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	.platdata_auto_alloc_size = sizeof(struct rk3368_clk_plat),
#endif
	.ofdata_to_platdata = rk3368_clk_ofdata_to_platdata,
	.ops		= &rk3368_clk_ops,
	.bind		= rk3368_clk_bind,
	.probe		= rk3368_clk_probe,
};

#if !defined(CONFIG_SPL_BUILD)
/**
 * soc_clk_dump() - Print clock frequencies
 * Returns zero on success
 *
 * Implementation for the clk dump command.
 */
int soc_clk_dump(void)
{
	struct udevice *cru_dev;
	struct rk3368_clk_priv *priv;
	const struct rk3368_clk_info *clk_dump;
	struct clk clk;
	unsigned long clk_count = ARRAY_SIZE(clks_dump);
	unsigned long rate;
	int i, ret;

	ret = uclass_get_device_by_driver(UCLASS_CLK,
					  DM_GET_DRIVER(rockchip_rk3368_cru),
					  &cru_dev);
	if (ret) {
		printf("%s failed to get cru device\n", __func__);
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
	       priv->set_armclk_rate ? priv->armlclk_hz / 1000 : 0,
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
