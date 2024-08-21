// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */
#include <stdint.h>
#include <init.h>
#include <os_test.h>
#include <ta_os_test.h>
#include <tee_internal_api_extensions.h>
#include <tee_ta_api.h>

/*
 * Trusted Application Entry Points
 */

/* Called each time a new instance is created */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("TA_CreateEntryPoint");
	return TEE_SUCCESS;
}

/* Called each time an instance is destroyed */
void TA_DestroyEntryPoint(void)
{
	DMSG("TA_DestroyEntryPoint");
}

/* Called each time a session is opened */
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes,
				    TEE_Param pParams[4],
				    void **ppSessionContext)
{
	(void)nParamTypes;
	(void)pParams;
	(void)ppSessionContext;
	DMSG("TA_OpenSessionEntryPoint");
	TEE_UnmaskCancellation();
	return TEE_SUCCESS;
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext)
{
	(void)pSessionContext;
	DMSG("TA_CloseSessionEntryPoint");
}

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
	(void)pSessionContext;

	switch (nCommandID) {
	case TA_OS_TEST_CMD_INIT:
		return ta_entry_init(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CLIENT_WITH_TIMEOUT:
		return ta_entry_client_with_timeout(nParamTypes, pParams);

	case TA_OS_TEST_CMD_BASIC:
		return ta_entry_basic(nParamTypes, pParams);

	case TA_OS_TEST_CMD_PANIC:
		return ta_entry_panic(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CLIENT:
		return ta_entry_client(nParamTypes, pParams);

	case TA_OS_TEST_CMD_PARAMS_ACCESS:
		return ta_entry_params_access_rights(nParamTypes, pParams);

	case TA_OS_TEST_CMD_WAIT:
		return ta_entry_wait(nParamTypes, pParams);

	case TA_OS_TEST_CMD_BAD_MEM_ACCESS:
		return ta_entry_bad_mem_access(nParamTypes, pParams);

	case TA_OS_TEST_CMD_TA2TA_MEMREF:
		return ta_entry_ta2ta_memref(nParamTypes, pParams);

	case TA_OS_TEST_CMD_TA2TA_MEMREF_MIX:
		return ta_entry_ta2ta_memref_mix(nParamTypes, pParams);

	case TA_OS_TEST_CMD_PARAMS:
		return ta_entry_params(nParamTypes, pParams);

	case TA_OS_TEST_CMD_NULL_MEMREF_PARAMS:
		return ta_entry_null_memref(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CALL_LIB:
		return ta_entry_call_lib(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CALL_LIB_PANIC:
		return ta_entry_call_lib_panic(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CALL_LIB_DL:
		return ta_entry_call_lib_dl(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CALL_LIB_DL_PANIC:
		return ta_entry_call_lib_dl_panic(nParamTypes, pParams);

	case TA_OS_TEST_CMD_GET_GLOBAL_VAR:
		return ta_entry_get_global_var(nParamTypes, pParams);

	case TA_OS_TEST_CMD_CLIENT_IDENTITY:
		return ta_entry_client_identity(nParamTypes, pParams);

	case TA_OS_TEST_CMD_TLS_TEST_MAIN:
		return ta_entry_tls_test_main();

	case TA_OS_TEST_CMD_TLS_TEST_SHLIB:
		return ta_entry_tls_test_shlib();

	case TA_OS_TEST_CMD_DL_PHDR:
		return ta_entry_dl_phdr();

	case TA_OS_TEST_CMD_DL_PHDR_DL:
		return ta_entry_dl_phdr_dl();

#if defined(WITH_CXX_TESTS)
	case TA_OS_TEST_CMD_CXX_CTOR_MAIN:
		return ta_entry_cxx_ctor_main();

	case TA_OS_TEST_CMD_CXX_CTOR_SHLIB:
		return ta_entry_cxx_ctor_shlib();

	case TA_OS_TEST_CMD_CXX_CTOR_SHLIB_DL:
		return ta_entry_cxx_ctor_shlib_dl();

	case TA_OS_TEST_CMD_CXX_EXC_MAIN:
		return ta_entry_cxx_exc_main();

	case TA_OS_TEST_CMD_CXX_EXC_MIXED:
		return ta_entry_cxx_exc_mixed();
#else
	case TA_OS_TEST_CMD_CXX_CTOR_MAIN:
	case TA_OS_TEST_CMD_CXX_CTOR_SHLIB:
	case TA_OS_TEST_CMD_CXX_CTOR_SHLIB_DL:
	case TA_OS_TEST_CMD_CXX_EXC_MAIN:
	case TA_OS_TEST_CMD_CXX_EXC_MIXED:
		return TEE_ERROR_NOT_SUPPORTED;
#endif

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
