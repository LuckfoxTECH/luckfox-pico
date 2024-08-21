/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <string.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_otp_key.h"
#include "rkcrypto_demo.h"

static uint8_t otp_key_0[32] = {
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
};
static uint8_t iv[16] = {
	0x10, 0x44, 0x80, 0xb3, 0x88, 0x5f, 0x02, 0x03,
	0x05, 0x21, 0x07, 0xc9, 0x44, 0x00, 0x1b, 0x80,
};
static uint8_t input[16] = {
	0xc9, 0x07, 0x21, 0x05, 0x80, 0x1b, 0x00, 0x44,
	0xac, 0x13, 0xfb, 0x23, 0x93, 0x4a, 0x66, 0xe4,
};
static uint8_t expected_enc[16] = {
	0xeb, 0xe7, 0xde, 0x12, 0x8d, 0x77, 0xf4, 0xe8,
	0x83, 0x4a, 0x63, 0x1d, 0x0e, 0xcc, 0xdb, 0x1c,
};
static uint32_t algo = RK_ALGO_AES;
static uint32_t mode = RK_CIPHER_MODE_CBC;

RK_RES demo_otpkey(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_cipher_config config;
	uint32_t key_id = RK_OEM_OTP_KEY0;
	uint32_t key_len = 16;
	uint32_t len = sizeof(input);
	rk_crypto_mem *in = NULL;
	rk_crypto_mem *out = NULL;

	/* Write keys. If written before, it will returns failure. */
	if (rk_write_oem_otp_key(RK_OEM_OTP_KEY0, otp_key_0, sizeof(otp_key_0)))
		printf("Check if otp key 0 is already written!\n");
	else
		printf("Write otp key 0, success!\n");

	/* Do cipher. */
	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error! res: 0x%08x\n", res);
		return res;
	}

	in = rk_crypto_mem_alloc(len);
	if (!in) {
		printf("malloc %uByte error!\n", len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	out = rk_crypto_mem_alloc(len);
	if (!out) {
		printf("malloc %uByte error!\n", len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	config.algo      = algo;
	config.mode      = mode;
	config.operation = RK_OP_CIPHER_ENC;
	config.key_len   = key_len;
	config.reserved  = NULL;
	memcpy(config.iv, iv, sizeof(iv));
	memcpy(in->vaddr, input, len);

	res = rk_oem_otp_key_cipher(key_id, &config, in->dma_fd, out->dma_fd, len);
	if (res) {
		printf("test rk_oem_otp_key_cipher fail! 0x%08x\n", res);
		goto exit;
	}

	if (memcmp(out->vaddr, expected_enc, len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test OTPKEY_CIPHER ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;

	res = rk_oem_otp_key_cipher(key_id, &config, out->dma_fd, out->dma_fd, len);
	if (res) {
		printf("test rk_oem_otp_key_cipher fail! 0x%08x\n", res);
		goto exit;
	}

	if (memcmp(out->vaddr, input, len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test OTPKEY_CIPHER DEC success!\n");

exit:
	rk_crypto_mem_free(in);
	rk_crypto_mem_free(out);

	rk_crypto_deinit();

	return res;
}

RK_RES demo_otpkey_virt(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_cipher_config config;
	uint32_t key_id = RK_OEM_OTP_KEY0;
	uint32_t key_len = 16;
	uint8_t output[16];
	uint32_t data_len = sizeof(input);

	/* Write keys. If written before, it will returns failure. */
	if (rk_write_oem_otp_key(RK_OEM_OTP_KEY0, otp_key_0, sizeof(otp_key_0)))
		printf("Check if otp key 0 is already written!\n");
	else
		printf("Write otp key 0, success!\n");

	/* Do cipher. */
	memset(output, 0, sizeof(output));

	config.algo      = algo;
	config.mode      = mode;
	config.operation = RK_OP_CIPHER_ENC;
	config.key_len   = key_len;
	config.reserved  = NULL;
	memcpy(config.iv, iv, sizeof(iv));

	res = rk_oem_otp_key_cipher_virt(key_id, &config, input, output, data_len);
	if (res) {
		printf("Do rk_oem_otp_key_cipher_virt error!\n");
		goto exit;
	}

	if (memcmp(output, expected_enc, data_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test OTPKEY_CIPHER_VIRT ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;

	res = rk_oem_otp_key_cipher_virt(key_id, &config, output, output, data_len);
	if (res) {
		printf("Do rk_oem_otp_key_cipher_virt error!\n");
		goto exit;
	}

	if (memcmp(output, input, data_len)) {
		printf("DEC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test OTPKEY_CIPHER_VIRT DEC success!\n");

exit:
	return res;
}
