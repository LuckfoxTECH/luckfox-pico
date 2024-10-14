// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <pta_system.h>
#include <tee_internal_api.h>
#include "rk_derive_key_api.h"

TEE_Result rk_derive_ta_unique_key(uint8_t *extra, uint16_t extra_size,
				    uint8_t *key, uint16_t key_size)
{
	TEE_UUID sta_uuid = PTA_SYSTEM_UUID;
	TEE_TASessionHandle sta_session = TEE_HANDLE_NULL;
	uint32_t origin;
	TEE_Result res;
	TEE_Param taParams[4];
	uint32_t nParamTypes;

	if (!extra || extra_size == 0 || !key || (key_size != 16 && key_size != 32))
		return TEE_ERROR_BAD_PARAMETERS;

	nParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);

	res = TEE_OpenTASession(&sta_uuid, 0, nParamTypes, taParams, &sta_session, &origin);
	if (res != TEE_SUCCESS)
		return res;

	nParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
			TEE_PARAM_TYPE_MEMREF_OUTPUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);

	taParams[0].memref.buffer = extra;
	taParams[0].memref.size = extra_size;
	taParams[1].memref.buffer = key;
	taParams[1].memref.size = key_size;

	res = TEE_InvokeTACommand(sta_session, 0, PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY,
				  nParamTypes, taParams, &origin);
	if (res != TEE_SUCCESS)
		EMSG("Failure when calling PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY");

	TEE_CloseTASession(sta_session);

	return res;
}
