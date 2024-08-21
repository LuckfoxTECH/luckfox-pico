// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018, Linaro Limited
 */

extern "C" {
#include "os_test_lib_dl.h"
#include <tee_api_types.h>
}

class OsTestLibDlCtorTest {
public:
	OsTestLibDlCtorTest() : val(2) {}

	int val;
};

static OsTestLibDlCtorTest os_test_lib_dl_ctor_test;

TEE_Result os_test_shlib_dl_cxx_ctor(void)
{
	if (os_test_lib_dl_ctor_test.val != 2)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}
