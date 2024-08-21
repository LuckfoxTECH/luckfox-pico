// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#define STR_TRACE_USER_TA "RKTEST"

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "rktest_ta.h"
#include "rktest_handle.h"

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
				    TEE_Param  params[4], void **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The IMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("Hello!\n");
	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the parameters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
				      uint32_t param_types, TEE_Param params[4])
{
	/* Unused parameters */
	(void)&sess_ctx;

	switch (cmd_id) {
	case RKTEST_TA_CMD_TRANSFER_DATA:
		return handle_transfer_data(param_types, params);
	case RKTEST_TA_CMD_STORAGE:
		return handle_storage();
	case RKTEST_TA_CMD_PROPERTY:
		return handle_property();
	case RKTEST_TA_CMD_CRYPTO_SHA:
		return handle_crypto_sha();
	case RKTEST_TA_CMD_CRYPTO_AES:
		return handle_crypto_aes();
	case RKTEST_TA_CMD_CRYPTO_RSA:
		return handle_crypto_rsa();
	case RKTEST_TA_CMD_OEM_OTP_READ:
		return handle_otp_read();
	case RKTEST_TA_CMD_OEM_OTP_WRITE:
		return handle_otp_write();
	case RKTEST_TA_CMD_OEM_OTP_SIZE:
		return handle_otp_size();
	case RKTEST_TA_CMD_STORAGE_SPEED:
		return handle_storage_speed(param_types, params);
	case RKTEST_TA_CMD_TRNG_READ:
		return handle_trng_read();
	default:
		EMSG("InvokeCommandEntry: BAD PARAMETERS!");
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

