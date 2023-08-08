/*
 * Copyright (c) 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/armv8/mmu.h>
#include <asm/arch/bootrom.h>
#include <asm/arch/grf_rk3399.h>
#include <asm/arch/cru_rk3399.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <syscon.h>

DECLARE_GLOBAL_DATA_PTR;

#define GRF_EMMCCORE_CON11 0xff77f02c
#define PMU_GRF_SOC_CON0   0xff320180

static struct mm_region rk3399_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xf8000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xf8000000UL,
		.phys = 0xf8000000UL,
		.size = 0x08000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3399_mem_map;

const char * const boot_devices[BROM_LAST_BOOTSOURCE + 1] = {
	[BROM_BOOTSOURCE_EMMC] = "/sdhci@fe330000",
	[BROM_BOOTSOURCE_SPINOR] = "/spi@ff1d0000",
	[BROM_BOOTSOURCE_SD] = "/dwmmc@fe320000",
};

#ifdef CONFIG_SPL_BUILD

#define TIMER_CHN10_BASE	0xff8680a0
#define TIMER_END_COUNT_L	0x00
#define TIMER_END_COUNT_H	0x04
#define TIMER_INIT_COUNT_L	0x10
#define TIMER_INIT_COUNT_H	0x14
#define TIMER_CONTROL_REG	0x1c

#define TIMER_EN	0x1
#define	TIMER_FMODE	(0 << 1)
#define	TIMER_RMODE	(1 << 1)

void rockchip_stimer_init(void)
{
	writel(0xffffffff, TIMER_CHN10_BASE + TIMER_END_COUNT_L);
	writel(0xffffffff, TIMER_CHN10_BASE + TIMER_END_COUNT_H);
	writel(0, TIMER_CHN10_BASE + TIMER_INIT_COUNT_L);
	writel(0, TIMER_CHN10_BASE + TIMER_INIT_COUNT_H);
	writel(TIMER_EN | TIMER_FMODE, TIMER_CHN10_BASE + TIMER_CONTROL_REG);
}
#endif

#define GRF_BASE	0xff770000
#define PMUGRF_BASE	0xff320000
#define PMUSGRF_BASE	0xff330000
#define PMUCRU_BASE	0xff750000
#define NIU_PERILP_NSP_ADDR	0xffad8188
#define QOS_PRIORITY_LEVEL(h, l)	((((h) & 3) << 8) | ((l) & 3))

#ifndef CONFIG_TPL_BUILD
int arch_cpu_init(void)
{
	struct rk3399_pmugrf_regs *pmugrf = (void *)PMUGRF_BASE;
	struct rk3399_grf_regs * const grf = (void *)GRF_BASE;

	/* We do some SoC one time setting here. */

#ifdef CONFIG_SPL_BUILD
	struct rk3399_pmusgrf_regs *sgrf = (void *)PMUSGRF_BASE;

	/*
	 * Disable DDR and SRAM security regions.
	 *
	 * As we are entered from the BootROM, the region from
	 * 0x0 through 0xfffff (i.e. the first MB of memory) will
	 * be protected. This will cause issues with the DW_MMC
	 * driver, which tries to DMA from/to the stack (likely)
	 * located in this range.
	 */
	rk_clrsetreg(&sgrf->ddr_rgn_con[16], 0x1ff, 0);
	rk_clrreg(&sgrf->slv_secure_con4, 0x2000);
#endif

	/* eMMC clock generator: disable the clock multipilier */
	rk_clrreg(&grf->emmccore_con[11], 0x0ff);

	/* PWM3 select pwm3a io */
	rk_clrreg(&pmugrf->soc_con0, 1 << 5);

#if defined(CONFIG_ROCKCHIP_RK3399PRO)
	struct rk3399_pmucru *pmucru = (void *)PMUCRU_BASE;

	/* set wifi_26M to 24M and disabled by default */
	writel(0x7f002000, &pmucru->pmucru_clksel[1]);
	writel(0x01000100, &pmucru->pmucru_clkgate_con[0]);
#endif

	/* Set perilp_nsp QOS priority to 3 for USB 3.0 */
	writel(QOS_PRIORITY_LEVEL(3, 3), NIU_PERILP_NSP_ADDR);

	return 0;
}
#endif

void board_debug_uart_init(void)
{
#define GRF_BASE	0xff770000
	struct rk3399_grf_regs * const grf = (void *)GRF_BASE;

#if defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff180000)
	/* Enable early UART0 on the RK3399 */
	rk_clrsetreg(&grf->gpio2c_iomux,
		     GRF_GPIO2C0_SEL_MASK,
		     GRF_UART0BT_SIN << GRF_GPIO2C0_SEL_SHIFT);
	rk_clrsetreg(&grf->gpio2c_iomux,
		     GRF_GPIO2C1_SEL_MASK,
		     GRF_UART0BT_SOUT << GRF_GPIO2C1_SEL_SHIFT);
#else
	/* Enable early UART2 channel on the RK3399/RK3399PRO */
	rk_clrsetreg(&grf->gpio4c_iomux,
		     GRF_GPIO4C3_SEL_MASK,
		     GRF_UART2DGBC_SIN << GRF_GPIO4C3_SEL_SHIFT);
	rk_clrsetreg(&grf->gpio4c_iomux,
		     GRF_GPIO4C4_SEL_MASK,
		     GRF_UART2DBGC_SOUT << GRF_GPIO4C4_SEL_SHIFT);
#if defined(CONFIG_ROCKCHIP_RK3399PRO)
	/* Set channel A as UART2 input */
	rk_clrsetreg(&grf->soc_con7,
		     GRF_UART_DBG_SEL_MASK,
		     GRF_UART_DBG_SEL_A << GRF_UART_DBG_SEL_SHIFT);
#else
	/* Set channel C as UART2 input */
	rk_clrsetreg(&grf->soc_con7,
		     GRF_UART_DBG_SEL_MASK,
		     GRF_UART_DBG_SEL_C << GRF_UART_DBG_SEL_SHIFT);
#endif
#endif
}
