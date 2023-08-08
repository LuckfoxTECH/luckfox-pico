/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 * Author: Andy Yan <andy.yan@rock-chips.com>
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_CRU_RK3368_H
#define _ASM_ARCH_CRU_RK3368_H

#include <common.h>


/* RK3368 clock numbers */
enum rk3368_pll_id {
	APLLB,
	APLLL,
	DPLL,
	CPLL,
	GPLL,
	NPLL,
	PLL_COUNT,
};

struct rk3368_clk_info {
	unsigned long id;
	char *name;
	bool is_cru;
};

struct rk3368_cru {
	struct rk3368_pll {
		unsigned int con0;
		unsigned int con1;
		unsigned int con2;
		unsigned int con3;
	} pll[6];
	unsigned int reserved[0x28];
	unsigned int clksel_con[56];
	unsigned int reserved1[8];
	unsigned int clkgate_con[25];
	unsigned int reserved2[7];
	unsigned int glb_srst_fst_val;
	unsigned int glb_srst_snd_val;
	unsigned int reserved3[0x1e];
	unsigned int softrst_con[15];
	unsigned int reserved4[0x11];
	unsigned int misc_con;
	unsigned int glb_cnt_th;
	unsigned int glb_rst_con;
	unsigned int glb_rst_st;
	unsigned int reserved5[0x1c];
	unsigned int sdmmc_con[2];
	unsigned int sdio0_con[2];
	unsigned int sdio1_con[2];
	unsigned int emmc_con[2];
};
check_member(rk3368_cru, emmc_con[1], 0x41c);

struct rk3368_clk_priv {
	struct rk3368_cru *cru;
	ulong armlclk_hz;
	ulong armlclk_enter_hz;
	ulong armlclk_init_hz;
	ulong armbclk_hz;
	ulong armbclk_enter_hz;
	ulong armbclk_init_hz;
	bool sync_kernel;
	bool set_armclk_rate;
};

enum {
	/* PLL CON0 */
	PLL_NR_SHIFT			= 8,
	PLL_NR_MASK			= GENMASK(13, 8),
	PLL_OD_SHIFT			= 0,
	PLL_OD_MASK			= GENMASK(3, 0),

	/* PLL CON1 */
	PLL_LOCK_STA			= BIT(31),
	PLL_NF_SHIFT			= 0,
	PLL_NF_MASK			= GENMASK(12, 0),

	/* PLL CON2 */
	PLL_BWADJ_SHIFT			= 0,
	PLL_BWADJ_MASK			= GENMASK(11, 0),

	/* PLL CON3 */
	PLL_MODE_SHIFT			= 8,
	PLL_MODE_MASK			= GENMASK(9, 8),
	PLL_MODE_SLOW			= 0,
	PLL_MODE_NORMAL			= 1,
	PLL_MODE_DEEP_SLOW		= 3,
	PLL_RESET_SHIFT			= 5,
	PLL_RESET			= 1,
	PLL_RESET_MASK			= GENMASK(5, 5),

	/* CLKSEL1CON */
	CORE_ACLK_DIV_SHIFT		= 0,
	CORE_ACLK_DIV_MASK		= 0x1f << CORE_ACLK_DIV_SHIFT,
	CORE_DBG_DIV_SHIFT		= 8,
	CORE_DBG_DIV_MASK		= 0x1f << CORE_DBG_DIV_SHIFT,

	CORE_CLK_PLL_SEL_SHIFT		= 7,
	CORE_CLK_PLL_SEL_MASK		= 1 << CORE_CLK_PLL_SEL_SHIFT,
	CORE_CLK_PLL_SEL_APLL		= 0,
	CORE_CLK_PLL_SEL_GPLL,
	CORE_DIV_CON_SHIFT		= 0,
	CORE_DIV_CON_MASK		= 0x1f << CORE_DIV_CON_SHIFT,

	/* CLKSEL8CON */
	PCLK_BUS_DIV_CON_SHIFT		= 12,
	PCLK_BUS_DIV_CON_MASK		= 0x7 << PCLK_BUS_DIV_CON_SHIFT,
	HCLK_BUS_DIV_CON_SHIFT		= 8,
	HCLK_BUS_DIV_CON_MASK		= 0x3 << HCLK_BUS_DIV_CON_SHIFT,
	CLK_BUS_PLL_SEL_CPLL		= 0,
	CLK_BUS_PLL_SEL_GPLL		= 1,
	CLK_BUS_PLL_SEL_SHIFT		= 7,
	CLK_BUS_PLL_SEL_MASK		= 1 << CLK_BUS_PLL_SEL_SHIFT,
	ACLK_BUS_DIV_CON_SHIFT		= 0,
	ACLK_BUS_DIV_CON_MASK		= 0x1f << ACLK_BUS_DIV_CON_SHIFT,

	/* CLKSEL9CON */
	PCLK_PERI_DIV_CON_SHIFT		= 12,
	PCLK_PERI_DIV_CON_MASK		= 0x3 << PCLK_PERI_DIV_CON_SHIFT,
	HCLK_PERI_DIV_CON_SHIFT		= 8,
	HCLK_PERI_DIV_CON_MASK		= 3 << HCLK_PERI_DIV_CON_SHIFT,
	CLK_PERI_PLL_SEL_CPLL		= 0,
	CLK_PERI_PLL_SEL_GPLL,
	CLK_PERI_PLL_SEL_SHIFT		= 7,
	CLK_PERI_PLL_SEL_MASK		= 1 << CLK_PERI_PLL_SEL_SHIFT,
	ACLK_PERI_DIV_CON_SHIFT		= 0,
	ACLK_PERI_DIV_CON_MASK		= 0x1f,

	/* CLKSEL10CON */
	CLK_CRYPTO_DIV_CON_SHIFT	= 14,
	CLK_CRYPTO_DIV_CON_MASK		= 0x3 << CLK_CRYPTO_DIV_CON_SHIFT,
	PCLK_ALIVE_DIV_CON_SHIFT	= 8,
	PCLK_ALIVE_DIV_CON_MASK		= 0x1f << PCLK_ALIVE_DIV_CON_SHIFT,

	/* CLKSEL12_CON */
	MCU_STCLK_DIV_SHIFT		= 8,
	MCU_STCLK_DIV_MASK		= GENMASK(10, 8),
	MCU_PLL_SEL_SHIFT		= 7,
	MCU_PLL_SEL_MASK		= BIT(7),
	MCU_PLL_SEL_CPLL		= 0,
	MCU_PLL_SEL_GPLL		= 1,
	MCU_CLK_DIV_SHIFT		= 0,
	MCU_CLK_DIV_MASK		= GENMASK(4, 0),

	/* CLKSEL19_CON */
	ACLK_VOP_PLL_SEL_SHIFT		= 6,
	ACLK_VOP_PLL_SEL_MASK		= GENMASK(7, 6),
	ACLK_VOP_PLL_SEL_CPLL		= 0,
	ACLK_VOP_PLL_SEL_GPLL		= 1,
	ACLK_VOP_DIV_SHIFT		= 0,
	ACLK_VOP_DIV_MASK		= GENMASK(4, 0),

	/* CLKSEL20_CON */
	DCLK_VOP_PLL_SEL_SHIFT		= 8,
	DCLK_VOP_PLL_SEL_MASK		= GENMASK(9, 8),
	DCLK_VOP_PLL_SEL_CPLL		= 0,
	DCLK_VOP_PLL_SEL_GPLL		= 1,
	DCLK_VOP_PLL_SEL_NPLL		= 2,
	DCLK_VOP_DIV_SHIFT		= 0,
	DCLK_VOP_DIV_MASK		= GENMASK(7, 0),

	/* CLKSEL_CON25 */
	CLK_SARADC_DIV_CON_SHIFT	= 8,
	CLK_SARADC_DIV_CON_MASK		= GENMASK(15, 8),
	CLK_SARADC_DIV_CON_WIDTH	= 8,

	/* CLKSEL43_CON */
	GMAC_DIV_CON_SHIFT		= 0x0,
	GMAC_DIV_CON_MASK		= GENMASK(4, 0),
	GMAC_PLL_SHIFT			= 6,
	GMAC_PLL_MASK			= GENMASK(7, 6),
	GMAC_PLL_SELECT_NEW		= (0x0 << GMAC_PLL_SHIFT),
	GMAC_PLL_SELECT_CODEC		= (0x1 << GMAC_PLL_SHIFT),
	GMAC_PLL_SELECT_GENERAL		= (0x2 << GMAC_PLL_SHIFT),
	GMAC_MUX_SEL_EXTCLK             = BIT(8),

	/* CLKSEL51_CON */
	MMC_PLL_SEL_SHIFT		= 8,
	MMC_PLL_SEL_MASK		= GENMASK(9, 8),
	MMC_PLL_SEL_CPLL		= (0 << MMC_PLL_SEL_SHIFT),
	MMC_PLL_SEL_GPLL                = (1 << MMC_PLL_SEL_SHIFT),
	MMC_PLL_SEL_USBPHY_480M         = (2 << MMC_PLL_SEL_SHIFT),
	MMC_PLL_SEL_24M                 = (3 << MMC_PLL_SEL_SHIFT),
	MMC_CLK_DIV_SHIFT		= 0,
	MMC_CLK_DIV_MASK		= GENMASK(6, 0),

	/* SOFTRST1_CON */
	MCU_PO_SRST_MASK		= BIT(13),
	MCU_SYS_SRST_MASK		= BIT(12),
	DMA1_SRST_REQ                   = BIT(2),

	/* SOFTRST4_CON */
	DMA2_SRST_REQ                   = BIT(0),

	/* GLB_RST_CON */
	PMU_GLB_SRST_CTRL_SHIFT		= 2,
	PMU_GLB_SRST_CTRL_MASK		= GENMASK(3, 2),
	PMU_RST_BY_FST_GLB_SRST 	= 0,
	PMU_RST_BY_SND_GLB_SRST 	= 1,
	PMU_RST_DISABLE			= 2,
	WDT_GLB_SRST_CTRL_SHIFT		= 1,
	WDT_GLB_SRST_CTRL_MASK		= BIT(1),
	WDT_TRIGGER_SND_GLB_SRST 	= 0,
	WDT_TRIGGER_FST_GLB_SRST 	= 1,
	TSADC_GLB_SRST_CTRL_SHIFT 	= 0,
	TSADC_GLB_SRST_CTRL_MASK  	= BIT(0),
	TSADC_TRIGGER_SND_GLB_SRST 	= 0,
	TSADC_TRIGGER_FST_GLB_SRST 	= 1,

};
#endif
