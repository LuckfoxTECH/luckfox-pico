/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_CRU_RK322X_H
#define _ASM_ARCH_CRU_RK322X_H

#include <common.h>

#define MHz				1000 * 1000
#define OSC_HZ				(24 * MHz)
#define APLL_HZ				(600 * MHz)
#define GPLL_HZ				(1200 * MHz)
#define CPLL_HZ				(500 * MHz)
#define ACLK_BUS_HZ			(150 * MHz)
#define ACLK_PERI_HZ			(150 * MHz)

/* Private data for the clock driver - used by rockchip_get_cru() */
struct rk322x_clk_priv {
	struct rk322x_cru *cru;
	ulong gpll_hz;
	ulong cpll_hz;
	ulong armclk_hz;
	ulong armclk_enter_hz;
	ulong armclk_init_hz;
	bool sync_kernel;
	bool set_armclk_rate;
};

struct rk322x_cru {
	struct rk322x_pll {
		unsigned int con0;
		unsigned int con1;
		unsigned int con2;
	} pll[4];
	unsigned int reserved0[4];
	unsigned int cru_mode_con;
	unsigned int cru_clksel_con[35];
	unsigned int cru_clkgate_con[16];
	unsigned int cru_softrst_con[9];
	unsigned int cru_misc_con;
	unsigned int reserved1[2];
	unsigned int cru_glb_cnt_th;
	unsigned int reserved2[3];
	unsigned int cru_glb_rst_st;
	unsigned int reserved3[(0x1c0 - 0x150) / 4 - 1];
	unsigned int cru_sdmmc_con[2];
	unsigned int cru_sdio_con[2];
	unsigned int reserved4[2];
	unsigned int cru_emmc_con[2];
	unsigned int reserved5[4];
	unsigned int cru_glb_srst_fst_value;
	unsigned int cru_glb_srst_snd_value;
	unsigned int cru_pll_mask_con;
};
check_member(rk322x_cru, cru_pll_mask_con, 0x01f8);

enum rk322x_pll_id {
	APLL,
	DPLL,
	CPLL,
	GPLL,
	NPLL,
	PLL_COUNT,
};

struct rk322x_clk_info {
	unsigned long id;
	char *name;
	bool is_cru;
};

#define RK2928_PLL_CON(x)		((x) * 0x4)
#define RK2928_MODE_CON		0x40

enum {
	/* CRU_CLK_SEL0_CON */
	BUS_ACLK_PLL_SEL_SHIFT	= 13,
	BUS_ACLK_PLL_SEL_MASK	= 3 << BUS_ACLK_PLL_SEL_SHIFT,
	BUS_ACLK_PLL_SEL_CPLL	= 0,
	BUS_ACLK_PLL_SEL_GPLL,
	BUS_ACLK_PLL_SEL_HDMIPLL,
	BUS_ACLK_DIV_SHIFT	= 8,
	BUS_ACLK_DIV_MASK	= 0x1f << BUS_ACLK_DIV_SHIFT,
	CORE_CLK_PLL_SEL_SHIFT	= 6,
	CORE_CLK_PLL_SEL_MASK	= 3 << CORE_CLK_PLL_SEL_SHIFT,
	CORE_CLK_PLL_SEL_APLL	= 0,
	CORE_CLK_PLL_SEL_GPLL,
	CORE_CLK_PLL_SEL_DPLL,
	CORE_DIV_CON_SHIFT	= 0,
	CORE_DIV_CON_MASK	= 0x1f << CORE_DIV_CON_SHIFT,

	/* CRU_CLK_SEL1_CON */
	BUS_PCLK_DIV_SHIFT	= 12,
	BUS_PCLK_DIV_MASK	= 7 << BUS_PCLK_DIV_SHIFT,
	BUS_HCLK_DIV_SHIFT	= 8,
	BUS_HCLK_DIV_MASK	= 3 << BUS_HCLK_DIV_SHIFT,
	CORE_ACLK_DIV_SHIFT	= 4,
	CORE_ACLK_DIV_MASK	= 7 << CORE_ACLK_DIV_SHIFT,
	CORE_PERI_DIV_SHIFT	= 0,
	CORE_PERI_DIV_MASK	= 0xf << CORE_PERI_DIV_SHIFT,

	/* CRU_CLKSEL5_CON */
	GMAC_OUT_PLL_SHIFT	= 15,
	GMAC_OUT_PLL_MASK	= 1 << GMAC_OUT_PLL_SHIFT,
	GMAC_OUT_DIV_SHIFT	= 8,
	GMAC_OUT_DIV_MASK	= 0x1f << GMAC_OUT_DIV_SHIFT,
	MAC_PLL_SEL_SHIFT	= 7,
	MAC_PLL_SEL_MASK	= 1 << MAC_PLL_SEL_SHIFT,
	RMII_EXTCLK_SLE_SHIFT	= 5,
	RMII_EXTCLK_SEL_MASK	= 1 << RMII_EXTCLK_SLE_SHIFT,
	RMII_EXTCLK_SEL_INT		= 0,
	RMII_EXTCLK_SEL_EXT,
	CLK_MAC_DIV_SHIFT	= 0,
	CLK_MAC_DIV_MASK	= 0x1f << CLK_MAC_DIV_SHIFT,

	/* CRU_CLKSEL10_CON */
	PERI_PCLK_DIV_SHIFT	= 12,
	PERI_PCLK_DIV_MASK	= 7 << PERI_PCLK_DIV_SHIFT,
	PERI_PLL_SEL_SHIFT	= 10,
	PERI_PLL_SEL_MASK	= 3 << PERI_PLL_SEL_SHIFT,
	PERI_PLL_CPLL		= 0,
	PERI_PLL_GPLL,
	PERI_PLL_HDMIPLL,
	PERI_HCLK_DIV_SHIFT	= 8,
	PERI_HCLK_DIV_MASK	= 3 << PERI_HCLK_DIV_SHIFT,
	PERI_ACLK_DIV_SHIFT	= 0,
	PERI_ACLK_DIV_MASK	= 0x1f << PERI_ACLK_DIV_SHIFT,

	/* CRU_CLKSEL11_CON */
	EMMC_PLL_SHIFT		= 12,
	EMMC_PLL_MASK		= 3 << EMMC_PLL_SHIFT,
	EMMC_SEL_CPLL		= 0,
	EMMC_SEL_GPLL,
	EMMC_SEL_24M,
	SDIO_PLL_SHIFT		= 10,
	SDIO_PLL_MASK		= 3 << SDIO_PLL_SHIFT,
	SDIO_SEL_CPLL		= 0,
	SDIO_SEL_GPLL,
	SDIO_SEL_24M,
	MMC0_PLL_SHIFT		= 8,
	MMC0_PLL_MASK		= 3 << MMC0_PLL_SHIFT,
	MMC0_SEL_CPLL		= 0,
	MMC0_SEL_GPLL,
	MMC0_SEL_24M,
	MMC0_DIV_SHIFT		= 0,
	MMC0_DIV_MASK		= 0xff << MMC0_DIV_SHIFT,

	/* CRU_CLKSEL12_CON */
	EMMC_DIV_SHIFT		= 8,
	EMMC_DIV_MASK		= 0xff << EMMC_DIV_SHIFT,
	SDIO_DIV_SHIFT		= 0,
	SDIO_DIV_MASK		= 0xff << SDIO_DIV_SHIFT,

	/* CLKSEL_CON24 */
	CRYPTO_PLL_SEL_SHIFT	= 5,
	CRYPTO_PLL_SEL_MASK	= 0x1 << CRYPTO_PLL_SEL_SHIFT,
	CRYPTO_PLL_SEL_CPLL	= 0,
	CRYPTO_PLL_SEL_GPLL,
	CRYPTO_DIV_SHIFT	= 0,
	CRYPTO_DIV_MASK		= 0x1f << CRYPTO_DIV_SHIFT,

	/* CLKSEL_CON25 */
	SPI_PLL_SEL_SHIFT	= 8,
	SPI_PLL_SEL_MASK	= 0x1 << SPI_PLL_SEL_SHIFT,
	SPI_PLL_SEL_CPLL	= 0,
	SPI_PLL_SEL_GPLL,
	SPI_DIV_SHIFT		= 0,
	SPI_DIV_MASK		= 0x7f << SPI_DIV_SHIFT,

	/* CRU_CLKSEL26_CON */
	DDR_CLK_PLL_SEL_SHIFT	= 8,
	DDR_CLK_PLL_SEL_MASK	= 3 << DDR_CLK_PLL_SEL_SHIFT,
	DDR_CLK_SEL_DPLL	= 0,
	DDR_CLK_SEL_GPLL,
	DDR_CLK_SEL_APLL,
	DDR_DIV_SEL_SHIFT	= 0,
	DDR_DIV_SEL_MASK	= 3 << DDR_DIV_SEL_SHIFT,

	/* CRU_CLKSEL27_CON */
	DCLK_LCDC_PLL_SEL_GPLL		= 0,
	DCLK_LCDC_PLL_SEL_CPLL		= 1,
	DCLK_LCDC_PLL_SEL_SHIFT		= 0,
	DCLK_LCDC_PLL_SEL_MASK		= 1 << DCLK_LCDC_PLL_SEL_SHIFT,
	DCLK_LCDC_SEL_HDMIPHY		= 0,
	DCLK_LCDC_SEL_PLL		= 1,
	DCLK_LCDC_SEL_SHIFT		= 1,
	DCLK_LCDC_SEL_MASK		= 1 << DCLK_LCDC_SEL_SHIFT,
	DCLK_LCDC_DIV_CON_SHIFT		= 8,
	DCLK_LCDC_DIV_CON_MASK		= 0xFf << DCLK_LCDC_DIV_CON_SHIFT,

	/* CRU_CLKSEL29_CON */
	GMAC_CLK_SRC_SHIFT	= 12,
	GMAC_CLK_SRC_MASK	= 1 << GMAC_CLK_SRC_SHIFT,

	/* CRU_CLKSEL33_CON */
	ACLK_VOP_PLL_SEL_SHIFT		= 5,
	ACLK_VOP_PLL_SEL_MASK		= 0x3 << ACLK_VOP_PLL_SEL_SHIFT,
	ACLK_VOP_PLL_SEL_CPLL		= 0,
	ACLK_VOP_PLL_SEL_GPLL		= 1,
	ACLK_VOP_PLL_SEL_HDMIPHY		= 2,
	ACLK_VOP_DIV_CON_SHIFT		= 0,
	ACLK_VOP_DIV_CON_MASK		= 0x1f << ACLK_VOP_DIV_CON_SHIFT,

	/* CRU_SOFTRST5_CON */
	DDRCTRL_PSRST_SHIFT	= 11,
	DDRCTRL_SRST_SHIFT	= 10,
	DDRPHY_PSRST_SHIFT	= 9,
	DDRPHY_SRST_SHIFT	= 8,
};
#endif
