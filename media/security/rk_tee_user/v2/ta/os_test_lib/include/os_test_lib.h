/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018, Linaro Limited
 */

#ifndef _OS_TEST_LIB_H_
#define _OS_TEST_LIB_H_

#include <tee_api_types.h>

int os_test_shlib_add(int a, int b);
void os_test_shlib_panic(void);

#if defined(WITH_TLS_TESTS)
extern __thread int os_test_shlib_tls_a;
extern __thread int os_test_shlib_tls_b;
#endif

TEE_Result os_test_shlib_cxx_ctor(void);

#endif /* _OS_TEST_LIB_H_ */
