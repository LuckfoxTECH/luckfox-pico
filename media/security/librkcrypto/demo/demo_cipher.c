/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <string.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_demo.h"

static uint8_t key_0[16] = {
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
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

RK_RES demo_cipher(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_cipher_config config;
	uint32_t key_len = sizeof(key_0);
	uint32_t data_len = sizeof(input);
	rk_handle handle = 0;
	rk_crypto_mem *in = NULL;
	rk_crypto_mem *out = NULL;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error! res: 0x%08x\n", res);
		return res;
	}

	in = rk_crypto_mem_alloc(data_len);
	if (!in) {
		printf("malloc %uByte error!\n", data_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	out = rk_crypto_mem_alloc(data_len);
	if (!out) {
		printf("malloc %uByte error!\n", data_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	config.algo      = algo;
	config.mode      = mode;
	config.operation = RK_OP_CIPHER_ENC;
	config.key_len   = key_len;
	config.reserved  = NULL;
	memcpy(config.key, key_0, key_len);
	memcpy(config.iv, iv, sizeof(iv));
	memcpy(in->vaddr, input, data_len);

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_cipher_crypt(handle, in->dma_fd, out->dma_fd, data_len);
	if (res) {
		rk_cipher_final(handle);
		printf("rk_cipher_crypt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);

	if (memcmp(out->vaddr, expected_enc, data_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;
	memcpy(config.iv, iv, sizeof(iv));

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_cipher_crypt(handle, out->dma_fd, out->dma_fd, data_len);
	if (res) {
		rk_cipher_final(handle);
		printf("rk_cipher_crypt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);

	if (memcmp(out->vaddr, input, data_len)) {
		printf("DEC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER DEC success!\n");

exit:
	rk_crypto_mem_free(in);
	rk_crypto_mem_free(out);

	rk_crypto_deinit();

	return res;
}

RK_RES demo_cipher_virt(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_cipher_config config;
	uint32_t key_len = sizeof(key_0);
	uint8_t output[16];
	uint32_t data_len = sizeof(input);
	rk_handle handle = 0;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error! res: 0x%08x\n", res);
		return res;
	}

	config.algo      = algo;
	config.mode      = mode;
	config.operation = RK_OP_CIPHER_ENC;
	config.key_len   = key_len;
	config.reserved  = NULL;
	memcpy(config.key, key_0, key_len);
	memcpy(config.iv, iv, sizeof(iv));
	memset(output, 0, sizeof(output));

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_cipher_crypt_virt(handle, input, output, data_len);
	if (res) {
		rk_cipher_final(handle);
		printf("rk_cipher_crypt_virt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);

	if (memcmp(output, expected_enc, data_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER_VIRT ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;
	memcpy(config.iv, iv, sizeof(iv));

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_cipher_crypt_virt(handle, output, output, data_len);
	if (res) {
		rk_cipher_final(handle);
		printf("rk_cipher_crypt_virt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);

	if (memcmp(output, input, data_len)) {
		printf("DEC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER_VIRT DEC success!\n");

exit:
	rk_crypto_deinit();
	return res;
}
