/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <string.h>
#include "rkcrypto_core_int.h"
#include "rkcrypto_otp_key.h"
#include "rkcrypto_trace.h"
#include "tee_client_api.h"

#define STORAGE_UUID { 0x2d26d8a8, 0x5134, 0x4dd8, \
		{ 0xb3, 0x2f, 0xb3, 0x4b, 0xce, 0xeb, 0xc4, 0x71 } }
#define RK_CRYPTO_SERVICE_UUID	{ 0x0cacdb5d, 0x4fea, 0x466c, \
		{ 0x97, 0x16, 0x3d, 0x54, 0x16, 0x52, 0x83, 0x0f } }

#define STORAGE_CMD_WRITE_OEM_OTP_KEY			14
#define STORAGE_CMD_SET_OEM_HR_OTP_READ_LOCK		15
#define STORAGE_CMD_OEM_OTP_KEY_IS_WRITTEN		16
#define CRYPTO_SERVICE_CMD_OEM_OTP_KEY_CIPHER		0x00000001
#define CRYPTO_SERVICE_CMD_OEM_OTP_KEY_PHYS_CIPHER	0x00000002

static const struct {
	const uint32_t tee_code;
	const uint32_t rk_crypto_code;
} tee_crypto_code[] = {
	{TEEC_SUCCESS,			RK_CRYPTO_SUCCESS},
	{TEEC_ERROR_GENERIC,		RK_CRYPTO_ERR_GENERIC},
	{TEEC_ERROR_BAD_PARAMETERS,	RK_CRYPTO_ERR_PARAMETER},
	{TEEC_ERROR_BAD_STATE,		RK_CRYPTO_ERR_STATE},
	{TEEC_ERROR_NOT_SUPPORTED,	RK_CRYPTO_ERR_NOT_SUPPORTED},
	{TEEC_ERROR_OUT_OF_MEMORY,	RK_CRYPTO_ERR_OUT_OF_MEMORY},
	{TEEC_ERROR_ACCESS_DENIED,	RK_CRYPTO_ERR_ACCESS_DENIED},
};

static RK_RES tee_to_crypto_code(uint32_t tee_code)
{
	uint32_t i;
	uint32_t array_size = sizeof(tee_crypto_code) / sizeof((tee_crypto_code)[0]);

	for (i = 0; i < array_size; i++) {
		if (tee_code == tee_crypto_code[i].tee_code)
			return tee_crypto_code[i].rk_crypto_code;
	}

	/* Others convert to RK_CRYPTO_ERR_GENERIC. */
	return RK_CRYPTO_ERR_GENERIC;
}

static uint32_t cipher_get_blocksize(uint32_t algo)
{
	switch (algo) {
	case RK_ALGO_DES:
	case RK_ALGO_TDES:
		return DES_BLOCK_SIZE;
	case RK_ALGO_AES:
		return AES_BLOCK_SIZE;
	case RK_ALGO_SM4:
		return SM4_BLOCK_SIZE;
	default:
		return 1;
	}
}

RK_RES rk_write_oem_otp_key(enum RK_OEM_OTP_KEYID key_id, uint8_t *key,
			    uint32_t key_len)
{
	RK_RES res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = STORAGE_UUID;
	uint32_t error_origin = 0;

	RK_CRYPTO_CHECK_PARAM(key_id != RK_OEM_OTP_KEY0 &&
			      key_id != RK_OEM_OTP_KEY1 &&
			      key_id != RK_OEM_OTP_KEY2 &&
			      key_id != RK_OEM_OTP_KEY3 &&
			      key_id != RK_OEM_OTP_KEY_FW);
	RK_CRYPTO_CHECK_PARAM(!key);
	RK_CRYPTO_CHECK_PARAM(key_len != 16 &&
			      key_len != 24 &&
			      key_len != 32);
	RK_CRYPTO_CHECK_PARAM(key_id == RK_OEM_OTP_KEY_FW &&
			      key_len != 16);

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_InitializeContext failed with code TEEC res= 0x%x", res);
		return tee_to_crypto_code(res);
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x",
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
		E_TRACE("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x",
			res, error_origin);
	}

	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return tee_to_crypto_code(res);
}

RK_RES rk_oem_otp_key_is_written(enum RK_OEM_OTP_KEYID key_id, uint8_t *is_written)
{
	RK_RES res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = STORAGE_UUID;
	uint32_t error_origin = 0;

	RK_CRYPTO_CHECK_PARAM(key_id != RK_OEM_OTP_KEY0 &&
			      key_id != RK_OEM_OTP_KEY1 &&
			      key_id != RK_OEM_OTP_KEY2 &&
			      key_id != RK_OEM_OTP_KEY3 &&
			      key_id != RK_OEM_OTP_KEY_FW);
	RK_CRYPTO_CHECK_PARAM(!is_written);

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_InitializeContext failed with code TEEC res= 0x%x", res);
		return tee_to_crypto_code(res);
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x",
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
		E_TRACE("Check if it has been set oem_hr_otp_read_lock!");
	} else if (res != TEEC_SUCCESS) {
		E_TRACE("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x",
			res, error_origin);
	} else {
		*is_written = operation.params[0].value.b;
	}

	TEEC_CloseSession(&session);

out:
	TEEC_FinalizeContext(&contex);
	return tee_to_crypto_code(res);
}

RK_RES rk_set_oem_hr_otp_read_lock(enum RK_OEM_OTP_KEYID key_id)
{
	RK_RES res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = STORAGE_UUID;
	uint32_t error_origin = 0;

	RK_CRYPTO_CHECK_PARAM(key_id != RK_OEM_OTP_KEY0 &&
			      key_id != RK_OEM_OTP_KEY1 &&
			      key_id != RK_OEM_OTP_KEY2 &&
			      key_id != RK_OEM_OTP_KEY3);

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_InitializeContext failed with code TEEC res= 0x%x", res);
		return tee_to_crypto_code(res);
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x",
			res, error_origin);
		goto out;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.params[0].value.a = key_id;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						TEEC_NONE,
						TEEC_NONE,
						TEEC_NONE);

	res = TEEC_InvokeCommand(&session, STORAGE_CMD_SET_OEM_HR_OTP_READ_LOCK,
				 &operation, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x",
			res, error_origin);
	}

	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return tee_to_crypto_code(res);
}

RK_RES rk_oem_otp_key_cipher_virt(enum RK_OEM_OTP_KEYID key_id, rk_cipher_config *config,
				  uint8_t *src, uint8_t *dst, uint32_t len)
{
	RK_RES res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = RK_CRYPTO_SERVICE_UUID;
	uint32_t error_origin = 0, len_aligned, len_cipher, block_size;
	TEEC_SharedMemory sm;

	RK_CRYPTO_CHECK_PARAM(key_id != RK_OEM_OTP_KEY0 &&
			      key_id != RK_OEM_OTP_KEY1 &&
			      key_id != RK_OEM_OTP_KEY2 &&
			      key_id != RK_OEM_OTP_KEY3 &&
			      key_id != RK_OEM_OTP_KEY_FW);
	RK_CRYPTO_CHECK_PARAM(!config  || !src || !dst);
	RK_CRYPTO_CHECK_PARAM(config->algo != RK_ALGO_AES &&
			      config->algo != RK_ALGO_SM4);
	RK_CRYPTO_CHECK_PARAM(config->mode >= RK_CIPHER_MODE_XTS);
	RK_CRYPTO_CHECK_PARAM(config->operation != RK_OP_CIPHER_ENC &&
			      config->operation != RK_OP_CIPHER_DEC);
	RK_CRYPTO_CHECK_PARAM(config->key_len != 16 &&
			      config->key_len != 24 &&
			      config->key_len != 32);
	RK_CRYPTO_CHECK_PARAM(key_id == RK_OEM_OTP_KEY_FW &&
			      config->key_len != 16);
	RK_CRYPTO_CHECK_PARAM(len > RK_CRYPTO_MAX_DATA_LEN ||
			      len == 0);
	RK_CRYPTO_CHECK_PARAM(config->mode == RK_CIPHER_MODE_CTS &&
			      len <= 16);

	len_cipher = len;
	block_size  = cipher_get_blocksize(config->algo);

	if (config->mode != RK_CIPHER_MODE_CTS && (len % block_size)) {
		RK_CRYPTO_CHECK_PARAM(config->mode != RK_CIPHER_MODE_CTR &&
				      config->mode != RK_CIPHER_MODE_CFB &&
				      config->mode != RK_CIPHER_MODE_OFB);

		len_aligned = ROUNDUP(len, block_size);
		len_cipher  = len_aligned;
	}

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_InitializeContext failed with code TEEC res= 0x%x", res);
		return tee_to_crypto_code(res);
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x",
			res, error_origin);
		goto out;
	}

	sm.size = len_cipher;
	sm.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	res = TEEC_AllocateSharedMemory(&contex, &sm);
	if (res != TEEC_SUCCESS) {
		E_TRACE("AllocateSharedMemory ERR! TEEC res= 0x%x", res);
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
		E_TRACE("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x",
			res, error_origin);
	} else {
		memcpy(dst, sm.buffer, sm.size);
	}

	TEEC_ReleaseSharedMemory(&sm);

out1:
	TEEC_CloseSession(&session);

out:
	TEEC_FinalizeContext(&contex);
	return tee_to_crypto_code(res);
}

RK_RES rk_oem_otp_key_cipher(enum RK_OEM_OTP_KEYID key_id, rk_cipher_config *config,
			     int32_t in_fd, int32_t out_fd, uint32_t len)
{
	RK_RES res;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid = RK_CRYPTO_SERVICE_UUID;
	uint32_t error_origin = 0, len_aligned, len_cipher, block_size;
	struct crypt_fd_map_op src_mop;
	struct crypt_fd_map_op dst_mop;

	RK_CRYPTO_CHECK_PARAM(in_fd < 0);
	RK_CRYPTO_CHECK_PARAM(out_fd < 0);
	RK_CRYPTO_CHECK_PARAM(key_id != RK_OEM_OTP_KEY0 &&
			      key_id != RK_OEM_OTP_KEY1 &&
			      key_id != RK_OEM_OTP_KEY2 &&
			      key_id != RK_OEM_OTP_KEY3 &&
			      key_id != RK_OEM_OTP_KEY_FW);
	RK_CRYPTO_CHECK_PARAM(!config);
	RK_CRYPTO_CHECK_PARAM(config->algo != RK_ALGO_AES &&
			      config->algo != RK_ALGO_SM4);
	RK_CRYPTO_CHECK_PARAM(config->mode >= RK_CIPHER_MODE_XTS);
	RK_CRYPTO_CHECK_PARAM(config->operation != RK_OP_CIPHER_ENC &&
			      config->operation != RK_OP_CIPHER_DEC);
	RK_CRYPTO_CHECK_PARAM(config->key_len != 16 &&
			      config->key_len != 24 &&
			      config->key_len != 32);
	RK_CRYPTO_CHECK_PARAM(key_id == RK_OEM_OTP_KEY_FW &&
			      config->key_len != 16);
	RK_CRYPTO_CHECK_PARAM(len == 0);
	RK_CRYPTO_CHECK_PARAM(config->mode == RK_CIPHER_MODE_CTS &&
			      len <= 16);

	len_cipher = len;
	block_size  = cipher_get_blocksize(config->algo);

	if (config->mode != RK_CIPHER_MODE_CTS && (len % block_size)) {
		RK_CRYPTO_CHECK_PARAM(config->mode != RK_CIPHER_MODE_CTR &&
				      config->mode != RK_CIPHER_MODE_CFB &&
				      config->mode != RK_CIPHER_MODE_OFB);

		len_aligned = ROUNDUP(len, block_size);
		len_cipher  = len_aligned;
	}

	memset(&src_mop, 0, sizeof(src_mop));
	memset(&dst_mop, 0, sizeof(dst_mop));
	src_mop.dma_fd = in_fd;
	dst_mop.dma_fd = out_fd;

	res = rk_crypto_fd_ioctl(RIOCCRYPT_FD_MAP, &src_mop);
	if (res != RK_CRYPTO_SUCCESS) {
		E_TRACE("RIOCCRYPT_FD_MAP failed, res= 0x%x", res);
		return res;
	}

	if (out_fd != in_fd) {
		res = rk_crypto_fd_ioctl(RIOCCRYPT_FD_MAP, &dst_mop);
		if (res != RK_CRYPTO_SUCCESS) {
			E_TRACE("RIOCCRYPT_FD_MAP failed, res= 0x%x", res);
			goto out;
		}
	} else {
		dst_mop.phys_addr = src_mop.phys_addr;
	}

	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_InitializeContext failed with code TEEC res= 0x%x", res);
		goto out1;
	}

	res = TEEC_OpenSession(&contex, &session, &uuid, TEEC_LOGIN_PUBLIC,
			       NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("TEEC_Opensession failed with code TEEC res= 0x%x origin 0x%x",
			res, error_origin);
		goto out2;
	}

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.params[0].value.a       = key_id;
	operation.params[1].tmpref.buffer = config;
	operation.params[1].tmpref.size   = sizeof(rk_cipher_config);
	operation.params[2].value.a       = src_mop.phys_addr;
	operation.params[2].value.b       = len_cipher;
	operation.params[3].value.a       = dst_mop.phys_addr;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_INPUT);

	res = TEEC_InvokeCommand(&session, CRYPTO_SERVICE_CMD_OEM_OTP_KEY_PHYS_CIPHER,
				 &operation, &error_origin);
	if (res != TEEC_SUCCESS) {
		E_TRACE("InvokeCommand ERR! TEEC res= 0x%x, error_origin= 0x%x",
			res, error_origin);
	}

	TEEC_CloseSession(&session);

out2:
	TEEC_FinalizeContext(&contex);

out1:
	res = tee_to_crypto_code(res);

	if (out_fd != in_fd)
		rk_crypto_fd_ioctl(RIOCCRYPT_FD_UNMAP, &dst_mop);

out:
	rk_crypto_fd_ioctl(RIOCCRYPT_FD_UNMAP, &src_mop);

	return res;
}
