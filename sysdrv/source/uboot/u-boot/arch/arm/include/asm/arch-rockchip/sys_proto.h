/*
 * (C) Copyright 2016 Rockchip Electronics Co.,Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ASM_ARCH_SYS_PROTO_H
#define _ASM_ARCH_SYS_PROTO_H

#if !defined(CONFIG_SPL_FRAMEWORK) || CONFIG_IS_ENABLED(TINY_FRAMEWORK)
int sdram_init(void);
#endif

#endif /* _ASM_ARCH_SYS_PROTO_H */
