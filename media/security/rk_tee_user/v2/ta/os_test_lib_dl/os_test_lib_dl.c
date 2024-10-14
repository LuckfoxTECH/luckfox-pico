// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018-2019, Linaro Limited
 */

#include <os_test_lib_dl.h>
#include <tee_internal_api.h>
#include <trace.h>

extern int os_test_global;

static void __attribute__((constructor)) os_test_shlib_dl_init(void)
{
	os_test_global *= 10;
	os_test_global += 3;
	DMSG("os_test_global=%d", os_test_global);
}

int os_test_shlib_dl_add(int a, int b)
{
	return a + b;
}

void os_test_shlib_dl_panic(void)
{
	TEE_Panic(0);
}
