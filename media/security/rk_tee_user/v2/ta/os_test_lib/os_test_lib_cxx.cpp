// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020 Huawei Technologies Co., Ltd
 */

extern "C" {
#include "os_test_lib.h"
#include <tee_api_types.h>
}

class OsTestLibCtorTest {
public:
	OsTestLibCtorTest() : val(2) {}

	int val;
};

static OsTestLibCtorTest os_test_lib_ctor_test;

TEE_Result os_test_shlib_cxx_ctor(void)
{
	if (os_test_lib_ctor_test.val != 2)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}
