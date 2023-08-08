// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd.
 */

#include <common.h>

#ifdef CONFIG_TPL_BUILD
#ifndef CONFIG_TPL_TINY_FRAMEWORK
#error please defined CONFIG_TPL_TINY_FRAMEWORK for RK3528 !!!
#endif
#endif

#ifdef CONFIG_TPL_BUILD

/* return: 0 = success, other = fail */
int sdram_init(void)
{
	return (-1);
}
#endif /* CONFIG_TPL_BUILD */
