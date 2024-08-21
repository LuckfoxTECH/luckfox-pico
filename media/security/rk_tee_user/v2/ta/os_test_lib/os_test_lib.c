// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018, Linaro Limited
 */

#include "os_test_lib.h"
#include <tee_internal_api.h>
#include <trace.h>

extern int os_test_global;

static void __attribute__((constructor)) os_test_shlib_init(void)
{
	os_test_global *= 10;
	os_test_global += 2;
	DMSG("os_test_global=%d", os_test_global);
}

#if defined(WITH_TLS_TESTS)
__thread int os_test_shlib_tls_a;
__thread int os_test_shlib_tls_b = 123;
#endif

int os_test_shlib_add(int a, int b)
{
	return a + b;
}

void os_test_shlib_panic(void)
{
	TEE_Panic(0);
}
