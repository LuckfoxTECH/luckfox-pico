/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef OS_TEST_H
#define OS_TEST_H

#include <tee_api.h>

TEE_Result ta_entry_basic(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_client_with_timeout(uint32_t param_types,
					TEE_Param params[4]);
TEE_Result ta_entry_panic(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_client(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_params_access_rights(uint32_t p_types, TEE_Param params[4]);
TEE_Result ta_entry_wait(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_bad_mem_access(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_ta2ta_memref(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_ta2ta_memref_mix(uint32_t param_types,
				     TEE_Param params[4]);
TEE_Result ta_entry_params(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_null_memref(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_call_lib(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_call_lib_panic(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_call_lib_dl(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_call_lib_dl_panic(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_get_global_var(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_client_identity(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_entry_tls_test_main(void);
TEE_Result ta_entry_tls_test_shlib(void);
TEE_Result ta_entry_dl_phdr(void);
TEE_Result ta_entry_dl_phdr_dl(void);
TEE_Result ta_entry_cxx_ctor_main(void);
TEE_Result ta_entry_cxx_ctor_shlib(void);
TEE_Result ta_entry_cxx_ctor_shlib_dl(void);
TEE_Result ta_entry_cxx_exc_main(void);
TEE_Result ta_entry_cxx_exc_mixed(void);

#endif /*OS_TEST_H */
