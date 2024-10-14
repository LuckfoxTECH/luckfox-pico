// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <tee_client_api.h>
#include <tee_api_defines_extensions.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "anti_copy.h"
#include "rk_anti_copy_ta.h"

#define STORAGE_CMD_WRITE_OEM_ENCRYPT_DATA	24

static TEEC_Result save_otp_data(uint8_t *data, uint32_t len)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t error_origin = 0;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	const TEEC_UUID otp_uuid = OTP_UUID;
	const TEEC_UUID *uuid;

	uuid = &otp_uuid;

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code 0x%x\n", res);
		return res;
	}

	res = TEEC_OpenSession(&contex, &session, uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code 0x%x origin 0x%x\n",
		       res, error_origin);
		goto out;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
						TEEC_NONE, TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.size = len;
	operation.params[0].tmpref.buffer = data;

	res = TEEC_InvokeCommand(&session, STORAGE_CMD_WRITE_OEM_ENCRYPT_DATA,
				 &operation, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		goto out1;
	}
	printf("save data into OTP success!\n");
out1:
	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return res;
}

static TEEC_Result call_anti_copy_ta(void)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t error_origin = 0;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	const TEEC_UUID otp_uuid = RK_ANTI_COPY_TA_UUID;
	const TEEC_UUID *uuid;

	uuid = &otp_uuid;

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code 0x%x\n", res);
		return res;
	}

	res = TEEC_OpenSession(&contex, &session, uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code 0x%x origin 0x%x\n",
		       res, error_origin);
		goto out;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(&session, RK_TA_CMD_MID_VERIFY,
				 &operation, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		goto out1;
	}

out1:
	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return res;
}

int anti_copy_mid_gen(void)
{
	uint8_t key[32] = {0xd3, 0x0b, 0x8f, 0x1c, 0xf4, 0xd9, 0x6c, 0x53,
			   0x7a, 0xf1, 0xa7, 0x47, 0x08, 0x16, 0x9b, 0xe9,
			   0x4b, 0x49, 0x93, 0x75, 0x90, 0xde, 0x9e, 0x41,
			   0x09, 0x40, 0xed, 0x4a, 0x10, 0xde, 0x51, 0x3c};
	uint8_t otp_data[16] = {0xd3, 0x0b, 0x8f, 0x1c, 0xf4, 0xd9, 0x6c, 0x53,
				0x7a, 0xf1, 0xa7, 0x47, 0x08, 0x16, 0x9b, 0xe9};
	uint8_t hmac[32];
	uint32_t hmac_len;

	HMAC(EVP_sha256(), key, sizeof(key), otp_data, sizeof(otp_data), hmac, &hmac_len);
	dump_hex("otp_data", otp_data, sizeof(otp_data));
	dump_hex("hmac", hmac, hmac_len);

	if (save_otp_data(otp_data, sizeof(otp_data))) {
		printf("save data into OTP fail!\n");
		return -1;
	}
	printf("generate hmac and save data into otp success!\n");
	return 0;
}

int anti_copy_mid_verify(void)
{
	return call_anti_copy_ta();
}
