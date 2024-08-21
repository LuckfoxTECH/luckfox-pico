/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2019, Linaro Limited
 */

#ifndef _OS_TEST_LIB_DL_H_
#define _OS_TEST_LIB_DL_H_

#include <tee_api_types.h>

int os_test_shlib_dl_add(int a, int b);
void os_test_shlib_dl_panic(void);
TEE_Result os_test_shlib_dl_cxx_ctor(void);

#endif /* _OS_TEST_LIB_DL_H_ */
