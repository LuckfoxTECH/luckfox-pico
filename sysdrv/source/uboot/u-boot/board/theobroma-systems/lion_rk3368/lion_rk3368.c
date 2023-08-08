/*
 * Copyright (c) 2017 Theobroma Systems Design und Consulting GmbH
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <ram.h>
#include <asm/io.h>
#include <syscon.h>

DECLARE_GLOBAL_DATA_PTR;

int mach_cpu_init(void)
{
	return 0;
}
