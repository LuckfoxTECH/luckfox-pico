/*
 * Copyright (c) 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/periph.h>
#include <asm/arch/grf_rk3066.h>

#define GRF_BASE	0x20008000

void board_debug_uart_init(void)
{
	struct rk3066_grf * const grf = (void *)GRF_BASE;

	/* Enable early UART on the RK3066 */
	rk_clrsetreg(&grf->gpio1b_iomux,
		     GPIO1B1_MASK | GPIO1B0_MASK,
		     GPIO1B1_UART2_SOUT << GPIO1B1_SHIFT |
		     GPIO1B0_UART2_SIN << GPIO1B0_SHIFT);
}

