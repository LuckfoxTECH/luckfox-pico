/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 *
 */

#ifndef __PSTORE_H_
#define __PSTORE_H_

void putc_to_ram(const char c);
void puts_to_ram(const char *str);

#endif
