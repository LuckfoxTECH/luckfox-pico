/*
 * Copyright (c) 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <asm/io.h>
#include <asm/arch/bootrom.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk3036.h>
#define GRF_BASE	0x20008000

#ifdef CONFIG_SPL_BUILD
int arch_cpu_init(void)
{
	return 0;
}
#endif
void board_debug_uart_init(void)
{
	struct rk3036_grf * const grf = (void *)GRF_BASE;
	/*
	 * NOTE: sd card and debug uart use same iomux in rk3036,
	 * so if you enable uart,
	 * you can not boot from sdcard
	 */
	rk_clrsetreg(&grf->gpio1c_iomux,
		     GPIO1C3_MASK << GPIO1C3_SHIFT |
		     GPIO1C2_MASK << GPIO1C2_SHIFT,
		     GPIO1C3_UART2_SOUT << GPIO1C3_SHIFT |
		     GPIO1C2_UART2_SIN << GPIO1C2_SHIFT);

}
