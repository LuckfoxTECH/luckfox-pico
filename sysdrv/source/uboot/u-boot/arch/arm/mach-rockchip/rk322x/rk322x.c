/*
 * Copyright (c) 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <asm/io.h>
#include <asm/arch/bootrom.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk322x.h>

#define GRF_BASE	0x11000000
#define CRU_MISC_CON	0x110e0134
#define SGRF_DDR_CON0	0x10150000

const char * const boot_devices[BROM_LAST_BOOTSOURCE + 1] = {
	[BROM_BOOTSOURCE_EMMC] = "/dwmmc@30020000",
	[BROM_BOOTSOURCE_SD] = "/dwmmc@30000000",
};

#ifndef CONFIG_TPL_BUILD
int arch_cpu_init(void)
{
	static struct rk322x_grf * const grf = (void *)GRF_BASE;
	/* We do some SoC one time setting here. */

#ifdef CONFIG_SPL_BUILD
	/* Disable the ddr secure region setting to make it non-secure */
	rk_clrreg(SGRF_DDR_CON0, 0x4000);
#endif

	/* PWMs select rkpwm clock source */
	rk_setreg(&grf->soc_con[2], 1 << 0);

	/* PWM0~3 io select */
	rk_setreg(&grf->con_iomux, 0xf << 0);

	/* UART1~2 io select */
	rk_setreg(&grf->con_iomux, (1 << 11) | (1 << 8));

	/* HDMI phy clock source select HDMIPHY clock out */
	rk_clrreg(CRU_MISC_CON, 1 << 13);

	/*
	* The integrated macphy is enabled by default, disable it
	* for saving power consuming.
	*/
	rk_clrsetreg(&grf->macphy_con[0], MACPHY_CFG_ENABLE_MASK,
		     0 << MACPHY_CFG_ENABLE_SHIFT);
	/* TODO: ECO version */

	return 0;
}
#endif

void board_debug_uart_init(void)
{
	static struct rk322x_grf * const grf = (void *)GRF_BASE;
	enum {
		GPIO1B2_SHIFT		= 4,
		GPIO1B2_MASK		= 3 << GPIO1B2_SHIFT,
		GPIO1B2_GPIO		= 0,
		GPIO1B2_UART21_SIN	= 2,

		GPIO1B1_SHIFT		= 2,
		GPIO1B1_MASK		= 3 << GPIO1B1_SHIFT,
		GPIO1B1_GPIO            = 0,
		GPIO1B1_UART1_SOUT,
		GPIO1B1_UART21_SOUT,
	};
	enum {
		CON_IOMUX_UART2SEL_SHIFT= 8,
		CON_IOMUX_UART2SEL_MASK	= 1 << CON_IOMUX_UART2SEL_SHIFT,
		CON_IOMUX_UART2SEL_2	= 0,
		CON_IOMUX_UART2SEL_21,
	};

	/* Enable early UART2 channel 1 on the RK322x */
	rk_clrsetreg(&grf->gpio1b_iomux,
		     GPIO1B1_MASK | GPIO1B2_MASK,
		     GPIO1B2_UART21_SIN << GPIO1B2_SHIFT |
		     GPIO1B1_UART21_SOUT << GPIO1B1_SHIFT);
	/* Set channel C as UART2 input */
	rk_clrsetreg(&grf->con_iomux,
		     CON_IOMUX_UART2SEL_MASK,
		     CON_IOMUX_UART2SEL_21 << CON_IOMUX_UART2SEL_SHIFT);
}
