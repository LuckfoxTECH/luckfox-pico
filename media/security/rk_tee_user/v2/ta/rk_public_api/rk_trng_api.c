// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */

#include <tee_internal_api.h>
#include "rk_trng_api.h"
#include "rk_os_service.h"

/*
 * RK356x, RK3399 platform supported.
 * Not all platform supports this API. When not supports, it will returns
 * TEE_ERROR_NOT_SUPPORTED.
 */
TEE_Result rk_get_trng(uint8_t *buffer, uint32_t size)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t param_type;
	TEE_Param params[4];
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	uint32_t error_origin = 0;
	TEE_UUID uuid = RK_OS_SERVICE_UUID;

	if (!buffer || size == 0)
		return TEE_ERROR_BAD_PARAMETERS;

	param_type = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

	res = TEE_OpenTASession(&uuid, 0, param_type, params,
				&session, &error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_OpenTASession failed with code 0x%x origin 0x%x",
		     res, error_origin);
		session = TEE_HANDLE_NULL;
		return res;
	}

	param_type = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);

	TEE_MemFill(params, 0, sizeof(params));
	params[0].memref.buffer = buffer;
	params[0].memref.size   = size;

	res = TEE_InvokeTACommand(session, 0, OS_SERVICE_CMD_GET_TRNG,
				  param_type, params, &error_origin);
	if (res != TEE_SUCCESS)
		EMSG("TEE_InvokeTACommand failed with code 0x%x origin 0x%x\n",
		     res, error_origin);

	TEE_CloseTASession(session);
	session = TEE_HANDLE_NULL;
	return res;
}
