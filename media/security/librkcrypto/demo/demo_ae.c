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
static uint8_t iv[12] = {
	0x10, 0x44, 0x80, 0xb3, 0x88, 0x5f, 0x02, 0x03,
	0x05, 0x21, 0x07, 0xc9,
};
static uint8_t aad_in[32] = {
	0x10, 0x44, 0x80, 0xb3, 0x88, 0x5f, 0x02, 0x03,
	0xac, 0x13, 0xfb, 0x23, 0x93, 0x4a, 0x66, 0xe4,
	0x05, 0x21, 0x07, 0xc9, 0x44, 0x00, 0x1b, 0x80,
	0xeb, 0xe7, 0xde, 0x12, 0x8d, 0x77, 0xf4, 0xe8,
};
static uint8_t input[32] = {
	0xc9, 0x07, 0x21, 0x05, 0x80, 0x1b, 0x00, 0x44,
	0xac, 0x13, 0xfb, 0x23, 0x93, 0x4a, 0x66, 0xe4,
	0xc9, 0x07, 0x21, 0x05, 0x80, 0x1b, 0x00, 0x44,
	0xac, 0x13, 0xfb, 0x23, 0x93, 0x4a, 0x66, 0xe4,
};
static uint8_t expected_enc[32] = {
	0x92, 0x8e, 0xc6, 0x87, 0x7f, 0xca, 0xc7, 0x96,
	0xbf, 0xa7, 0x86, 0xb4, 0xfe, 0xda, 0xf3, 0xec,
	0xdc, 0x24, 0x4d, 0x45, 0x78, 0x3a, 0xe3, 0x3b,
	0x55, 0xbf, 0x9b, 0x9f, 0xbd, 0x4c, 0xf7, 0xe7,
};
static uint8_t expected_tag[16] = {
	0x07, 0xf2, 0x31, 0x06, 0xc6, 0xc1, 0x7d, 0x1e,
	0xe7, 0x79, 0xe0, 0x2d, 0x27, 0x5b, 0x5d, 0x12,
};
static uint32_t algo = RK_ALGO_AES;
static uint32_t mode = RK_CIPHER_MODE_GCM;

static RK_RES set_ae_config(rk_ae_config *config, uint32_t algo, uint32_t mode, uint32_t operation,
			    uint8_t *key, uint32_t key_len, uint8_t *iv, uint32_t iv_len,
			    uint32_t tag_len, uint32_t aad_len)
{
	if (!config || !key || !iv)
		return RK_CRYPTO_ERR_GENERIC;

	config->algo      = algo;
	config->mode      = mode;
	config->operation = operation;
	config->key_len   = key_len;
	config->iv_len    = iv_len;
	config->tag_len   = tag_len;
	config->aad_len   = aad_len;
	config->reserved  = NULL;
	memcpy(config->key, key, key_len);
	memcpy(config->iv, iv, iv_len);

	return RK_CRYPTO_SUCCESS;
}

RK_RES demo_ae(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_ae_config config;
	uint32_t data_len = sizeof(input);
	uint32_t aad_len = sizeof(aad_in);
	rk_handle handle = 0;
	rk_crypto_mem *in = NULL;
	rk_crypto_mem *out = NULL;
	rk_crypto_mem *aad = NULL;
	uint8_t tag[16];

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

	aad = rk_crypto_mem_alloc(aad_len);
	if (!aad) {
		printf("malloc %uByte error!\n", aad_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	res = set_ae_config(&config, algo, mode, RK_OP_CIPHER_ENC, key_0, sizeof(key_0), iv,
			    sizeof(iv), sizeof(expected_tag), aad_len);
	if (res) {
		printf("set_ae_config error! res: 0x%08x\n", res);
		goto exit;
	}

	memcpy(in->vaddr, input, data_len);
	memcpy(aad->vaddr, aad_in, aad_len);
	memset(tag, 0, sizeof(tag));

	res = rk_ae_init(&config, &handle);
	if (res) {
		printf("rk_ae_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_ae_set_aad(handle, aad->dma_fd);
	if (res) {
		printf("rk_ae_set_aad error! res: 0x%08x\n", res);
		rk_ae_final(handle);
		goto exit;
	}

	res = rk_ae_crypt(handle, in->dma_fd, out->dma_fd, data_len, tag);
	if (res) {
		printf("rk_ae_crypt error! res: 0x%08x\n", res);
		rk_ae_final(handle);
		goto exit;
	}

	rk_ae_final(handle);

	if (memcmp(out->vaddr, expected_enc, data_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	if (memcmp(tag, expected_tag, sizeof(expected_tag))) {
		printf("ENC tag not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test AE ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;
	memcpy(config.iv, iv, sizeof(iv));

	res = rk_ae_init(&config, &handle);
	if (res) {
		printf("rk_ae_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_ae_set_aad(handle, aad->dma_fd);
	if (res) {
		printf("rk_ae_set_aad error! res: 0x%08x\n", res);
		rk_ae_final(handle);
		goto exit;
	}

	/* Ensure the the tag is correct, it will be checked when decrypting */
	res = rk_ae_crypt(handle, out->dma_fd, out->dma_fd, data_len, tag);
	if (res) {
		printf("rk_ae_crypt error! res: 0x%08x\n", res);
		rk_ae_final(handle);
		goto exit;
	}

	rk_ae_final(handle);

	if (memcmp(out->vaddr, input, data_len)) {
		printf("DEC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test AE DEC success!\n");

exit:
	rk_crypto_mem_free(in);
	rk_crypto_mem_free(out);
	rk_crypto_mem_free(aad);

	rk_crypto_deinit();

	return res;
}

RK_RES demo_ae_virt(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_ae_config config;
	uint32_t data_len = sizeof(input);
	uint8_t output[32];
	uint8_t tag[16];
	rk_handle handle = 0;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error! res: 0x%08x\n", res);
		return res;
	}

	res = set_ae_config(&config, algo, mode, RK_OP_CIPHER_ENC, key_0, sizeof(key_0), iv,
			    sizeof(iv), sizeof(expected_tag), sizeof(aad_in));
	if (res) {
		printf("set_ae_config error! res: 0x%08x\n", res);
		goto exit;
	}

	memset(tag, 0, sizeof(tag));

	res = rk_ae_init(&config, &handle);
	if (res) {
		printf("rk_ae_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_ae_set_aad_virt(handle, aad_in);
	if (res) {
		printf("rk_ae_set_aad error! res: 0x%08x\n", res);
		rk_ae_final(handle);
		goto exit;
	}

	res = rk_ae_crypt_virt(handle, input, output, data_len, tag);
	if (res) {
		rk_ae_final(handle);
		printf("rk_ae_crypt_virt error[%x]\n", res);
		goto exit;
	}

	rk_ae_final(handle);

	if (memcmp(output, expected_enc, data_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	if (memcmp(tag, expected_tag, sizeof(expected_tag))) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test AE_VIRT ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;
	memcpy(config.iv, iv, sizeof(iv));

	res = rk_ae_init(&config, &handle);
	if (res) {
		printf("rk_ae_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_ae_set_aad_virt(handle, aad_in);
	if (res) {
		printf("rk_ae_set_aad error! res: 0x%08x\n", res);
		rk_ae_final(handle);
		goto exit;
	}

	/* Ensure the the tag is correct, it will be checked when decrypting */
	res = rk_ae_crypt_virt(handle, output, output, data_len, tag);
	if (res) {
		rk_ae_final(handle);
		printf("rk_ae_crypt_virt error[%x]\n", res);
		goto exit;
	}

	rk_ae_final(handle);

	if (memcmp(output, input, data_len)) {
		printf("DEC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test AE_VIRT DEC success!\n");

exit:
	rk_crypto_deinit();
	return res;
}
