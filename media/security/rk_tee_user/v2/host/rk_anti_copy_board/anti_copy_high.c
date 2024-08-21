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
#include <sys/types.h>
#include <sys/stat.h>

#include "anti_copy.h"
#include "rk_anti_copy_ta.h"

enum RK_OEM_OTP_KEYID {
	RK_OEM_OTP_KEY0 = 0,
	RK_OEM_OTP_KEY1 = 1,
	RK_OEM_OTP_KEY2 = 2,
	RK_OEM_OTP_KEY3 = 3,
	RK_OEM_OTP_KEY_FW = 10,
	RK_OEM_OTP_KEYMAX
};

/* Crypto mode */
enum RK_CIPIHER_MODE {
	RK_CIPHER_MODE_ECB = 0,
	RK_CIPHER_MODE_CBC = 1,
	RK_CIPHER_MODE_CTS = 2,
	RK_CIPHER_MODE_CTR = 3,
	RK_CIPHER_MODE_CFB = 4,
	RK_CIPHER_MODE_OFB = 5,
	RK_CIPHER_MODE_XTS = 6,
	RK_CIPHER_MODE_CCM = 7,
	RK_CIPHER_MODE_GCM = 8,
	RK_CIPHER_MODE_CMAC = 9,
	RK_CIPHER_MODE_CBC_MAC = 10,
	RK_CIPHER_MODE_MAX
};

/* Crypto algorithm */
enum RK_CRYPTO_ALGO {
	RK_ALGO_AES = 1,
	RK_ALGO_DES,
	RK_ALGO_TDES,
	RK_ALGO_SM4,
	RK_ALGO_ALGO_MAX
};

/* Crypto operation */
#define RK_MODE_ENCRYPT			1
#define RK_MODE_DECRYPT			0

#define AES_BLOCK_SIZE			16

typedef struct {
	uint32_t	algo;
	uint32_t	mode;
	uint32_t	operation;
	uint8_t		key[64];
	uint32_t	key_len;
	uint8_t		iv[16];
	void		*reserved;
} rk_cipher_config;

#define STORAGE_CMD_WRITE_OEM_OTP_KEY			14
#define STORAGE_CMD_OEM_OTP_KEY_IS_WRITTEN		16
#define CRYPTO_SERVICE_CMD_OEM_OTP_KEY_CIPHER		0x00000001

static TEEC_Result rk_write_oem_otp_key(enum RK_OEM_OTP_KEYID key_id,
					uint8_t *key, uint32_t key_len)
{
	TEEC_Result res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = OTP_UUID;
	uint32_t error_origin = 0;

	if (key_id != RK_OEM_OTP_KEY0 &&
	    key_id != RK_OEM_OTP_KEY1 &&
	    key_id != RK_OEM_OTP_KEY2 &&
	    key_id != RK_OEM_OTP_KEY3 &&
	    key_id != RK_OEM_OTP_KEY_FW) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	if (!key) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	if (key_len != 16 &&
	    key_len != 24 &&
	    key_len != 32) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code TEEC res= 0x%x\n", res);
		return res;
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x\n",
			res, error_origin);
		goto out;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.params[0].value.a       = key_id;
	operation.params[1].tmpref.buffer = key;
	operation.params[1].tmpref.size   = key_len;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_NONE,
						TEEC_NONE);

	res = TEEC_InvokeCommand(&session, STORAGE_CMD_WRITE_OEM_OTP_KEY,
				 &operation, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x\n",
			res, error_origin);
	}

	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return res;
}

static TEEC_Result rk_oem_otp_key_is_written(enum RK_OEM_OTP_KEYID key_id,
					     uint8_t *is_written)
{
	TEEC_Result res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = OTP_UUID;
	uint32_t error_origin = 0;

	if (key_id != RK_OEM_OTP_KEY0 &&
	    key_id != RK_OEM_OTP_KEY1 &&
	    key_id != RK_OEM_OTP_KEY2 &&
	    key_id != RK_OEM_OTP_KEY3 &&
	    key_id != RK_OEM_OTP_KEY_FW) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	if (!is_written) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code TEEC res= 0x%x\n", res);
		return res;
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x\n",
			res, error_origin);
		goto out;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.params[0].value.a = key_id;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
						TEEC_NONE,
						TEEC_NONE,
						TEEC_NONE);

	res = TEEC_InvokeCommand(&session, STORAGE_CMD_OEM_OTP_KEY_IS_WRITTEN,
				 &operation, &error_origin);
	if (res == TEEC_ERROR_ACCESS_DENIED) {
		printf("Check if it has been set oem_hr_otp_read_lock!\n");
	} else if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x\n",
			res, error_origin);
	} else {
		*is_written = operation.params[0].value.b;
	}

	TEEC_CloseSession(&session);

out:
	TEEC_FinalizeContext(&contex);
	return res;
}

static TEEC_Result rk_oem_otp_key_cipher_virt(enum RK_OEM_OTP_KEYID key_id, rk_cipher_config *config,
					      uint8_t *src, uint8_t *dst, uint32_t len)
{
	TEEC_Result res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = RK_CRYPTO_SERVICE_UUID;
	uint32_t error_origin = 0, len_cipher;
	TEEC_SharedMemory sm;

	if (key_id != RK_OEM_OTP_KEY0 &&
	    key_id != RK_OEM_OTP_KEY1 &&
	    key_id != RK_OEM_OTP_KEY2 &&
	    key_id != RK_OEM_OTP_KEY3 &&
	    key_id != RK_OEM_OTP_KEY_FW) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	if (!config  || !src || !dst) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (len % AES_BLOCK_SIZE || len == 0)
		return TEEC_ERROR_BAD_PARAMETERS;

	len_cipher = len;

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code TEEC res= 0x%x\n", res);
		return res;
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x\n",
			res, error_origin);
		goto out;
	}

	sm.size = len_cipher;
	sm.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	res = TEEC_AllocateSharedMemory(&contex, &sm);
	if (res != TEEC_SUCCESS) {
		printf("AllocateSharedMemory ERR! TEEC res= 0x%x\n", res);
		goto out1;
	}

	memset(sm.buffer, 0, sm.size);
	memcpy(sm.buffer, src, len);

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.params[0].value.a       = key_id;
	operation.params[1].tmpref.buffer = config;
	operation.params[1].tmpref.size   = sizeof(rk_cipher_config);
	operation.params[2].memref.parent = &sm;
	operation.params[2].memref.offset = 0;
	operation.params[2].memref.size   = sm.size;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_PARTIAL_INOUT,
						TEEC_NONE);

	res = TEEC_InvokeCommand(&session, CRYPTO_SERVICE_CMD_OEM_OTP_KEY_CIPHER,
				 &operation, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x\n",
			res, error_origin);
	} else {
		memcpy(dst, sm.buffer, sm.size);
	}

	TEEC_ReleaseSharedMemory(&sm);

out1:
	TEEC_CloseSession(&session);

out:
	TEEC_FinalizeContext(&contex);
	return res;
}

int anti_copy_high_gen(void)
{
	uint8_t key[16] = {
		0x53, 0x46, 0x1f, 0x93, 0x4b, 0x16, 0x00, 0x28,
		0xcc, 0x34, 0xb1, 0x37, 0x30, 0xa4, 0x72, 0x66,
	};
	uint8_t is_written;

	if (rk_oem_otp_key_is_written(RK_OEM_OTP_KEY0, &is_written)) {
		return -1;
	}
	if (is_written != 0) {
		printf("oem otp key is written!\n");
		return 0;
	}
	if (rk_write_oem_otp_key(RK_OEM_OTP_KEY0, key, sizeof(key))) {
		printf("write oem otp key fail!");
		return -1;
	}
	printf("write oem otp key success!");
	return 0;
}

int anti_copy_high_verify(void)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	rk_cipher_config config;
	uint32_t key_id = RK_OEM_OTP_KEY0;
	uint32_t key_len = 16;
	uint8_t input[16] = {
		0xc9, 0x07, 0x21, 0x05, 0x80, 0x1b, 0x00, 0x44,
		0xac, 0x13, 0xfb, 0x23, 0x93, 0x4a, 0x66, 0xe4,
	};
	uint8_t expected_enc[16] = {
		0x69, 0xD9, 0x7F, 0x58, 0x80, 0xF2, 0x17, 0xE4,
		0x27, 0x85, 0xC4, 0xF8, 0x25, 0xB5, 0x76, 0x4B,
	};
	uint8_t output[16];
	uint32_t data_len = sizeof(input);

	memset(output, 0, sizeof(output));

	config.algo      = RK_ALGO_AES;
	config.mode      = RK_CIPHER_MODE_ECB;
	config.operation = RK_MODE_ENCRYPT;
	config.key_len   = key_len;
	config.reserved  = NULL;

	res = rk_oem_otp_key_cipher_virt(key_id, &config, input, output, data_len);
	if (res) {
		printf("Do rk_oem_otp_key_cipher_virt error!\n");
		goto exit;
	}

	dump_hex("output", output, data_len);
	dump_hex("expected_enc", expected_enc, data_len);

	if (memcmp(output, expected_enc, data_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = TEEC_ERROR_GENERIC;
		goto exit;
	}

	printf("Test OTPKEY_CIPHER_VIRT ENC success!\n");

exit:
	return res;
}
