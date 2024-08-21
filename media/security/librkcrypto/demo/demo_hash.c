/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <string.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_demo.h"

#define HASH_MAX_LEN	64
static uint8_t input[16] = {
	0xc9, 0x07, 0x21, 0x05, 0x80, 0x1b, 0x00, 0x44,
	0xac, 0x13, 0xfb, 0x23, 0x93, 0x4a, 0x66, 0xe4,
};
static uint8_t expected_hash[] = {
	0x5e, 0xd1, 0x70, 0xb2, 0x0a, 0xcd, 0xf2, 0x8e,
	0xee, 0x28, 0xd7, 0x70, 0x78, 0x79, 0x2d, 0xf5,
	0x83, 0xba, 0xf4, 0x52, 0xce, 0x3f, 0x71, 0x70,
	0x15, 0x4f, 0x2c, 0x48, 0xbc, 0x51, 0x23, 0x6f,
};
static uint8_t expected_hmac[] = {
	0x91, 0xcb, 0x13, 0x85, 0x1e, 0xa1, 0xbb, 0xc2,
	0xa4, 0x0f, 0x79, 0x9f, 0xc8, 0xb1, 0x95, 0x3b,
	0x2d, 0x3f, 0xe7, 0xe4, 0x6d, 0x95, 0x19, 0x34,
	0x97, 0x7f, 0x63, 0x46, 0xff, 0x92, 0xa3, 0x51,
};
static uint8_t key[SHA256_BLOCK_SIZE] = {
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
};
static uint32_t hash_algo = RK_ALGO_SHA256;
static uint32_t hmac_algo = RK_ALGO_HMAC_SHA256;

RK_RES demo_hash(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_hash_config hash_cfg;
	rk_handle hash_hdl = 0;
	uint32_t data_len = sizeof(input);
	rk_crypto_mem *in = NULL;
	uint8_t output[HASH_MAX_LEN];


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

	memcpy(in->vaddr, input, data_len);
	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = hash_algo;

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		printf("rk_hash_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_hash_update(hash_hdl, in->dma_fd, in->size);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		printf("rk_hash_update error = %d\n", res);
		goto exit;
	}

	rk_hash_final(hash_hdl, output);

	/* Verify the result */
	if (memcmp(output, expected_hash, sizeof(expected_hash)) != 0) {
		printf("HASH result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test HASH success!\n");

exit:
	rk_crypto_mem_free(in);

	rk_crypto_deinit();

	return res;
}

RK_RES demo_hash_virt(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_hash_config hash_cfg;
	uint8_t output[HASH_MAX_LEN];
	uint32_t data_len = sizeof(input);
	rk_handle hash_hdl = 0;
	uint32_t data_block = 128;
	uint8_t *tmp_data;
	uint32_t tmp_len;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error! res: 0x%08x\n", res);
		return res;
	}

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = hash_algo;

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		printf("rk_hash_init error! res: 0x%08x\n", res);
		goto exit;
	}

	tmp_len  = data_len;
	tmp_data = input;

	while (tmp_len) {
		if (tmp_len > data_block) {
			res = rk_hash_update_virt(hash_hdl, tmp_data, data_block);
			if (res) {
				rk_hash_final(hash_hdl, NULL);
				printf("rk_hash_update_virt error! res: 0x%08x\n", res);
				goto exit;
			}
		} else {
			data_block = tmp_len;
			res = rk_hash_update_virt(hash_hdl, tmp_data, tmp_len);
			if (res) {
				rk_hash_final(hash_hdl, NULL);
				printf("rk_hash_update_virt error! res: 0x%08x\n", res);
				goto exit;
			}
		}
		tmp_len -= data_block;
		tmp_data += data_block;
	}

	rk_hash_final(hash_hdl, output);

	if (memcmp(output, expected_hash, sizeof(expected_hash)) != 0) {
		printf("HASH result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test HASH_VIRT success!\n");

exit:
	rk_crypto_deinit();
	return res;
}

RK_RES demo_hmac(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_hash_config hash_cfg;
	rk_handle hash_hdl = 0;
	uint32_t data_len = sizeof(input);
	rk_crypto_mem *in = NULL;
	uint8_t output[HASH_MAX_LEN];

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

	memcpy(in->vaddr, input, data_len);
	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo    = hmac_algo;
	hash_cfg.key     = key;
	hash_cfg.key_len = sizeof(key);

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		printf("rk_hash_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_hash_update(hash_hdl, in->dma_fd, in->size);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		printf("rk_hash_update error = %d\n", res);
		goto exit;
	}

	rk_hash_final(hash_hdl, output);

	/* Verify the result */
	if (memcmp(output, expected_hmac, sizeof(expected_hmac)) != 0) {
		printf("HMAC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test HMAC success!\n");

exit:
	rk_crypto_mem_free(in);

	rk_crypto_deinit();

	return res;
}

RK_RES demo_hmac_virt(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_hash_config hash_cfg;
	uint8_t output[HASH_MAX_LEN];
	uint32_t data_len = sizeof(input);
	rk_handle hash_hdl = 0;
	uint32_t data_block = 128;
	uint8_t *tmp_data;
	uint32_t tmp_len;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error! res: 0x%08x\n", res);
		return res;
	}

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo    = hmac_algo;
	hash_cfg.key     = key;
	hash_cfg.key_len = sizeof(key);

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		printf("rk_hash_init error! res: 0x%08x\n", res);
		goto exit;
	}

	tmp_len  = data_len;
	tmp_data = input;

	while (tmp_len) {
		data_block = tmp_len > data_block ? data_block : tmp_len;

		res = rk_hash_update_virt(hash_hdl, tmp_data, data_block);
		if (res) {
			rk_hash_final(hash_hdl, NULL);
			printf("rk_hash_update_virt error! res: 0x%08x\n", res);
			goto exit;
		}

		tmp_len -= data_block;
		tmp_data += data_block;
	}

	rk_hash_final(hash_hdl, output);

	if (memcmp(output, expected_hmac, sizeof(expected_hmac)) != 0) {
		printf("HASH result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test HASH_VIRT success!\n");

exit:
	rk_crypto_deinit();
	return res;
}
