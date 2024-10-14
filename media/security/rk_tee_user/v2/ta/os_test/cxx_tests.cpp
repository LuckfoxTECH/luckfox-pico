// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020 Huawei Technologies Co., Ltd
 */

extern "C" {

#include <dlfcn.h>
#include <os_test_lib.h>
#include <stdint.h>
#include <tee_ta_api.h>

#include "cxx_tests.h"
#include "os_test.h"

};

class CtorTest {
public:
	CtorTest() : val(1) {}

	int val;
};

static CtorTest ctor_test;

TEE_Result ta_entry_cxx_ctor_main(void)
{
	if (ctor_test.val != 1)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

TEE_Result ta_entry_cxx_ctor_shlib(void)
{
	return os_test_shlib_cxx_ctor();
}

TEE_Result ta_entry_cxx_ctor_shlib_dl(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Result (*ctor_test_fn)(void);
	void *handle = NULL;

	handle = dlopen("b3091a65-9751-4784-abf7-0298a7cc35ba",
			RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
	if (!handle)
		return TEE_ERROR_GENERIC;

	ctor_test_fn = (TEE_Result (*)(void))dlsym(handle,
						   "os_test_shlib_dl_cxx_ctor");
	if (ctor_test_fn)
		res = ctor_test_fn();

	dlclose(handle);
	return res;
}

class MyException {
};

TEE_Result ta_entry_cxx_exc_main(void)
{
	try {
		throw MyException();
	} catch (MyException &e) {
		return TEE_SUCCESS;
	}

	return TEE_ERROR_GENERIC;
}

class MixedFrameException {
};

void throw_mfe(void)
{
	throw MixedFrameException();
}

class MixedFrameExceptionTest {
public:
	MixedFrameExceptionTest() {}

	bool test();
};

bool MixedFrameExceptionTest::test()
{
	try {
		throwing_c_func();
	} catch (MixedFrameException e) {
		return true;
	}
	return false;
}

TEE_Result ta_entry_cxx_exc_mixed(void)
{
	MixedFrameExceptionTest test;

	if (test.test())
		return TEE_SUCCESS;

	return TEE_ERROR_GENERIC;
}
