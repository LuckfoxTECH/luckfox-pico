// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "rktest_handle.h"

TEE_Result handle_transfer_data(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_SUCCESS;

	ASSERT_PARAM_TYPES(TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
		TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_MEMREF_OUTPUT,
		TEE_PARAM_TYPE_NONE));

	//Get and update the params[0].
	IMSG("Get and update the params[0].");
	params[0].value.a++;
	params[0].value.b = params[0].value.a;

	//Get the params[1] and update the params[2]
	IMSG("Get the params[1]: %s", (char *)params[1].memref.buffer);
	res = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_WRITE | TEE_MEMORY_ACCESS_ANY_OWNER,
		params[2].memref.buffer, params[2].memref.size);
	if (res != TEE_SUCCESS) {
		EMSG("CheckMemoryAccessRights ERR: 0x%x.", res);
		return res;
	}
	IMSG("Copy params[1] to params[2].\nDone.");
	TEE_MemMove(params[2].memref.buffer, params[1].memref.buffer, params[2].memref.size);
	return res;
}

