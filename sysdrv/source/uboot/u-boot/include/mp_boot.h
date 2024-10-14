/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _MP_BOOT_H_
#define _MP_BOOT_H_

#include <spl.h>

struct task_data {
	struct spl_load_info info;

	/* to be filled by smp task */
	void *boot_addr;
	ulong boot_size;
};

void mpb_init_1(struct spl_load_info info);
void mpb_init_x(int evt);
ulong mpb_post(int evt);

#endif

