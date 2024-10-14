// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <string.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "rk_anti_copy_ta.h"
#include "../rk_public_api/rk_crypto_api.h"

#define STORAGE_CMD_READ_OEM_ENCRYPT_DATA	26

static void ta_dump_hex(const char *name, const uint8_t *array, uint32_t len)
{
	uint32_t i;
	char buffer[256];
	char *p;

	IMSG("=================== %s [%u] ================", name, len);

	p = buffer;
	for (i = 0; i < len; i++) {
		if (i % 16 == 0 && i != 0) {
			IMSG("%s", buffer);
			p = buffer;
			TEE_MemFill(buffer, 0x00, sizeof(buffer));
		}
		p += snprintf(p, 256, "%02x ", array[i]);
	}
	IMSG("%s\n", buffer);
}

static TEE_Result read_otp_data(uint8_t *read_data, uint32_t read_data_size)
{
	TEE_UUID sta_uuid = { 0x2d26d8a8, 0x5134, 0x4dd8,
			{ 0xb3, 0x2f, 0xb3, 0x4b, 0xce, 0xeb, 0xc4, 0x71 } };
	TEE_TASessionHandle sta_session = TEE_HANDLE_NULL;
	uint32_t origin;
	TEE_Result res;
	TEE_Param taParams[4];
	uint32_t nParamTypes;

	nParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);

	res = TEE_OpenTASession(&sta_uuid, 0, nParamTypes, taParams, &sta_session, &origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_OpenTASession failed\n");
		return res;
	}

	nParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE);

	taParams[0].memref.buffer = read_data;
	taParams[0].memref.size = read_data_size;

	res = TEE_InvokeTACommand(sta_session, 0,
				  STORAGE_CMD_READ_OEM_ENCRYPT_DATA,
				  nParamTypes, taParams, &origin);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_InvokeTACommand returned 0x%x\n", res);
	}

	TEE_CloseTASession(sta_session);
	sta_session = TEE_HANDLE_NULL;

	return res;
}

static TEE_Result handle_mid_verify(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint8_t expect_hmac[32] = {0x5a, 0xfd, 0x92, 0x0d, 0xe4, 0x4c, 0xc9, 0xf4,
				   0x26, 0x04, 0x16, 0x7d, 0x0e, 0x63, 0x49, 0x16,
				   0x2b, 0xb2, 0xac, 0x58, 0xd1, 0x8e, 0x16, 0x5f,
				   0x1f, 0x91, 0x95, 0x47, 0x45, 0x2b, 0x9f, 0xd4};
	uint8_t key[32] = {0xd3, 0x0b, 0x8f, 0x1c, 0xf4, 0xd9, 0x6c, 0x53,
			   0x7a, 0xf1, 0xa7, 0x47, 0x08, 0x16, 0x9b, 0xe9,
			   0x4b, 0x49, 0x93, 0x75, 0x90, 0xde, 0x9e, 0x41,
			   0x09, 0x40, 0xed, 0x4a, 0x10, 0xde, 0x51, 0x3c};
	uint8_t hmac[32];
	uint32_t hmac_len;
	uint8_t otp_data[16];

	res = read_otp_data(otp_data, sizeof(otp_data));
	if (res) {
		EMSG("read data from otp fail!");
		return res;
	}
	ta_dump_hex("otp_data", otp_data, sizeof(otp_data));

	res = rk_mac_crypto(otp_data, hmac, sizeof(otp_data), &hmac_len,
			    key, sizeof(key), NULL, TEE_ALG_HMAC_SHA256);
	if (res) {
		EMSG("rk_mac_crypto fail! res = 0x%x", res);
		return res;
	}
	ta_dump_hex("hmac", hmac, hmac_len);

	if (memcmp(expect_hmac, hmac, hmac_len) != 0) {
		EMSG("verify fail!");
		return TEE_ERROR_MAC_INVALID;
	}
	IMSG("verify success!");
	return TEE_SUCCESS;
}

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

	IMSG("Hello!\n");

	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	/* Unused parameter */
	(void)&sess_ctx;

	IMSG("Goodbye!\n");
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the parameters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
				      uint32_t param_types __unused,
				      TEE_Param params[4] __unused)
{
	/* Unused parameters */
	(void)&sess_ctx;

	switch (cmd_id) {
	case RK_TA_CMD_MID_VERIFY:
		return handle_mid_verify();
	default:
		EMSG("InvokeCommandEntry: BAD PARAMETERS!");
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

