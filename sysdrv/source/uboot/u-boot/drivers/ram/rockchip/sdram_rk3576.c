// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd.
 */

#include <common.h>

#ifdef CONFIG_TPL_BUILD
int sdram_init(void)
{
	return (-1);
}
#endif /* CONFIG_TPL_BUILD */
