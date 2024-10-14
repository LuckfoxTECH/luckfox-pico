// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */

#include <tee_internal_api.h>
#include "rk_oem_otp_api.h"
#include "rk_os_service.h"


TEE_Result rk_otp_read(uint32_t offset, uint8_t *data, uint32_t len)
{
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	uint32_t error_origin;
	TEE_Result res;
	TEE_Param params[4];
	uint32_t paramtypes;
	TEE_UUID uuid = RK_OS_SERVICE_UUID;

	if (!data || len == 0)
		return TEE_ERROR_BAD_PARAMETERS;

	paramtypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);
	res = TEE_OpenTASession(&uuid, 0, paramtypes, params, &session,
				&error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_OpenTASession failed with code 0x%x origin 0x%x",
		     res, error_origin);
		session = TEE_HANDLE_NULL;
		return res;
	}

	paramtypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				     TEE_PARAM_TYPE_MEMREF_INOUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);
	TEE_MemFill(params, 0, sizeof(params));
	params[0].value.a = offset;
	params[1].memref.buffer = data;
	params[1].memref.size = len;
	res = TEE_InvokeTACommand(session, 0, OS_SERVICE_CMD_READ_OEM_OTP,
				  paramtypes, params, &error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_InvokeTACommand failed with code 0x%x origin 0x%x\n",
		     res, error_origin);
	}

	TEE_CloseTASession(session);
	session = TEE_HANDLE_NULL;
	return res;
}

TEE_Result rk_otp_write(uint32_t offset, uint8_t *data, uint32_t len)
{
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	uint32_t error_origin;
	TEE_Result res;
	TEE_Param params[4];
	uint32_t paramtypes;
	TEE_UUID uuid = RK_OS_SERVICE_UUID;

	if (!data || len == 0)
		return TEE_ERROR_BAD_PARAMETERS;

	paramtypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);
	res = TEE_OpenTASession(&uuid, 0, paramtypes, params, &session,
				&error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_OpenTASession failed with code 0x%x origin 0x%x",
		     res, error_origin);
		session = TEE_HANDLE_NULL;
		return res;
	}

	paramtypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				     TEE_PARAM_TYPE_MEMREF_INOUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);
	TEE_MemFill(params, 0, sizeof(params));
	params[0].value.a = offset;
	params[1].memref.buffer = data;
	params[1].memref.size = len;
	res = TEE_InvokeTACommand(session, 0, OS_SERVICE_CMD_WRITE_OEM_OTP,
				  paramtypes, params, &error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_InvokeTACommand failed with code 0x%x origin 0x%x\n",
		     res, error_origin);
	}

	TEE_CloseTASession(session);
	session = TEE_HANDLE_NULL;
	return res;
}

TEE_Result rk_otp_size(uint32_t *otp_size)
{
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	uint32_t error_origin;
	TEE_Result res;
	TEE_Param params[4];
	uint32_t paramtypes;
	TEE_UUID uuid = RK_OS_SERVICE_UUID;

	if (!otp_size)
		return TEE_ERROR_BAD_PARAMETERS;

	paramtypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);
	res = TEE_OpenTASession(&uuid, 0, paramtypes, params, &session,
				&error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_OpenTASession failed with code 0x%x origin 0x%x",
		     res, error_origin);
		session = TEE_HANDLE_NULL;
		return res;
	}

	paramtypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE);
	TEE_MemFill(params, 0, sizeof(params));
	res = TEE_InvokeTACommand(session, 0, OS_SERVICE_CMD_SIZEOF_OEM_OTP,
				  paramtypes, params, &error_origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_InvokeTACommand failed with code 0x%x origin 0x%x\n",
		     res, error_origin);
	}
	*otp_size = params[0].value.a;

	TEE_CloseTASession(session);
	session = TEE_HANDLE_NULL;
	return res;
}