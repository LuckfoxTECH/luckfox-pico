// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018, Linaro Limited
 */

#include <tee_internal_api.h>

#include "seed_rng_taf.h"

TEE_Result seed_rng_pool(uint32_t param_types, TEE_Param params[4])
{
	static const TEE_UUID system_uuid = PTA_SYSTEM_UUID;
	TEE_TASessionHandle sess = TEE_HANDLE_NULL;
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t ret_orig = 0;

	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
			    TEE_PARAM_TYPE_NONE,
			    TEE_PARAM_TYPE_NONE,
			    TEE_PARAM_TYPE_NONE)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!params[0].memref.size)
		return TEE_ERROR_BAD_PARAMETERS;

	res = TEE_OpenTASession(&system_uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
				&sess, &ret_orig);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_OpenTASession failed");
		goto cleanup_return;
	}

	res = TEE_InvokeTACommand(sess, TEE_TIMEOUT_INFINITE,
				  PTA_SYSTEM_ADD_RNG_ENTROPY,
				  param_types, params, &ret_orig);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_InvokeTACommand failed");
		goto cleanup_return;
	}

cleanup_return:
	TEE_CloseTASession(sess);
	return res;
}
