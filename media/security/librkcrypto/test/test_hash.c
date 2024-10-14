/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_trace.h"
#include "cmode_adapter.h"
#include "test_hash.h"
#include "test_utils.h"

#define HASH_MAX_LEN	64
#define TEST_DATA_MAX	(1024 * 1024 - 31)

struct test_hash_item {
	uint32_t algo;
	uint32_t blocksize;
};

static struct test_hash_item test_hash_tbl[] = {
	{RK_ALGO_MD5,        MD5_BLOCK_SIZE},
	{RK_ALGO_SHA1,       SHA1_BLOCK_SIZE},
	{RK_ALGO_SHA256,     SHA256_BLOCK_SIZE},
	{RK_ALGO_SHA224,     SHA224_BLOCK_SIZE},
	{RK_ALGO_SHA512,     SHA512_BLOCK_SIZE},
	{RK_ALGO_SHA384,     SHA384_BLOCK_SIZE},
	{RK_ALGO_SHA512_224, SHA512_224_BLOCK_SIZE},
	{RK_ALGO_SHA512_256, SHA512_256_BLOCK_SIZE},
	{RK_ALGO_SM3,        SM3_BLOCK_SIZE},
};

static struct test_hash_item test_hmac_tbl[] = {
	{RK_ALGO_HMAC_MD5,    MD5_BLOCK_SIZE},
	{RK_ALGO_HMAC_SHA1,   SHA1_BLOCK_SIZE},
	{RK_ALGO_HMAC_SHA256, SHA256_BLOCK_SIZE},
	{RK_ALGO_HMAC_SHA512, SHA512_BLOCK_SIZE},
	{RK_ALGO_HMAC_SM3,    SM3_BLOCK_SIZE},
};

static RK_RES test_hash_item_virt(const struct test_hash_item *item,
				  uint8_t *buffer, uint32_t buffer_len, bool is_hmac, int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t data_block = 32 * 1024;
	uint32_t out_len, tmp_len;
	uint8_t hash_soft[HASH_MAX_LEN], hash_hard[HASH_MAX_LEN];
	uint8_t key[MAX_HASH_BLOCK_SIZE];
	uint8_t *tmp_data;
	const char *test_name = "hash";
	rk_handle hash_hdl = 0;
	rk_hash_config hash_cfg;
	uint32_t algo, key_len;

	test_get_rng(buffer, buffer_len);

	memset(hash_soft, 0x00, sizeof(hash_soft));
	memset(hash_hard, 0x00, sizeof(hash_hard));

	algo    = item->algo;
	key_len = item->blocksize;

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = algo;

	if (is_hmac) {
		test_get_rng(key, key_len);
		hash_cfg.key     = key;
		hash_cfg.key_len = key_len;
		test_name = "hmac";
	}

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
			E_TRACE("rk_hash_init error[%x]\n", res);
			goto exit;
		}

		if (verbose)
			printf("virt:\t[%12s]\tN/A\n", test_algo_name(algo));
		return RK_CRYPTO_SUCCESS;
	}

	tmp_len  = buffer_len;
	tmp_data = buffer;

	while (tmp_len) {
		data_block = tmp_len > data_block ? data_block : tmp_len;

		res = rk_hash_update_virt(hash_hdl, tmp_data, data_block);
		if (res) {
			rk_hash_final(hash_hdl, NULL);
			E_TRACE("rk_hash_update_virt[%lu/%u] error = %d\n",
				(unsigned long)(tmp_data - buffer), tmp_len, res);
			goto exit;
		}

		tmp_len -= data_block;
		tmp_data += data_block;
	}

	rk_hash_final(hash_hdl, hash_hard);

	if (is_hmac)
		res = soft_hmac(algo, key, key_len, buffer, buffer_len, hash_soft, &out_len);
	else
		res = soft_hash(algo, buffer, buffer_len, hash_soft, &out_len);
	if (res) {
		E_TRACE("soft_%s error[%x]\n", test_name, res);
		goto exit;
	}

	/* Verify the result */
	if (memcmp(hash_hard, hash_soft, out_len) != 0) {
		E_TRACE("test_%s_item_virt compare failed.\n", test_name);
		test_dump_hex("hash_hard", hash_hard, out_len);
		test_dump_hex("hash_soft", hash_soft, out_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	hash_hdl = 0;
	if (verbose)
		printf("virt:\t[%12s]\tPASS\n", test_algo_name(algo));

	res = RK_CRYPTO_SUCCESS;
exit:
	if (res)
		printf("virt:\t[%12s]\tFAIL\n", test_algo_name(algo));

	return res;
}

static RK_RES test_hash_item_fd(const struct test_hash_item *item,
				rk_crypto_mem *buffer, bool is_hmac, int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t out_len;
	uint8_t hash_soft[HASH_MAX_LEN], hash_hard[HASH_MAX_LEN];
	uint8_t key[MAX_HASH_BLOCK_SIZE];
	const char *test_name = "hash";
	rk_handle hash_hdl = 0;
	rk_hash_config hash_cfg;
	uint32_t algo, key_len;

	test_get_rng(buffer->vaddr, buffer->size);

	memset(hash_soft, 0x00, sizeof(hash_soft));
	memset(hash_hard, 0x00, sizeof(hash_hard));

	algo    = item->algo;
	key_len = item->blocksize;

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = algo;

	if (is_hmac) {
		test_get_rng(key, key_len);
		hash_cfg.key     = key;
		hash_cfg.key_len = key_len;
		test_name = "hmac";
	}

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
			E_TRACE("rk_hash_init error[%x]\n", res);
			goto exit;
		}

		if (verbose)
			printf("dma_fd:\t[%12s]\tN/A\n", test_algo_name(algo));

		return RK_CRYPTO_SUCCESS;
	}

	res = rk_hash_update(hash_hdl, buffer->dma_fd, buffer->size);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		E_TRACE("rk_hash_update error = %d\n", res);
		goto exit;
	}

	rk_hash_final(hash_hdl, hash_hard);

	if (is_hmac)
		res = soft_hmac(algo, key, key_len, buffer->vaddr, buffer->size,
				hash_soft, &out_len);
	else
		res = soft_hash(algo, buffer->vaddr, buffer->size, hash_soft, &out_len);
	if (res) {
		E_TRACE("soft_%s error[%x]\n", test_name, res);
		goto exit;
	}

	/* Verify the result */
	if (memcmp(hash_hard, hash_soft, out_len) != 0) {
		E_TRACE("test_%s_item_fd compare failed.\n", test_name);
		test_dump_hex("buffer", buffer->vaddr, buffer->size);
		test_dump_hex("hash_hard", hash_hard, out_len);
		test_dump_hex("hash_soft", hash_soft, out_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}


	hash_hdl = 0;
	if (verbose)
		printf("dma_fd:\t[%12s]\tPASS\n", test_algo_name(algo));

	res = RK_CRYPTO_SUCCESS;
exit:
	if (res)
		printf("dma_fd:\t[%12s]\tFAIL\n", test_algo_name(algo));

	return res;
}

RK_RES test_hash(int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint8_t *buffer = NULL;
	uint32_t buffer_len = TEST_DATA_MAX;
	rk_crypto_mem *mem_buf = NULL;
	uint32_t i;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	buffer = malloc(buffer_len);
	if (!buffer) {
		E_TRACE("test hash malloc buffer %uByte error!\n", buffer_len);
		goto exit;
	}

	mem_buf = rk_crypto_mem_alloc(buffer_len);
	if (!mem_buf) {
		E_TRACE("test hash rk_crypto_mem_alloc %uByte error!\n", buffer_len);
		goto exit;
	}

	for (i = 0; i < ARRAY_SIZE(test_hash_tbl); i++) {
		res = test_hash_item_virt(&test_hash_tbl[i], buffer, buffer_len, false, verbose);
		if (res)
			goto exit;

		res = test_hash_item_fd(&test_hash_tbl[i], mem_buf, false, verbose);
		if (res)
			goto exit;
	}
exit:
	rk_crypto_mem_free(mem_buf);
	rk_crypto_deinit();
	if (buffer)
		free(buffer);
	return res;
}

RK_RES test_hmac(int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint8_t *buffer = NULL;
	uint32_t buffer_len = TEST_DATA_MAX;
	rk_crypto_mem *mem_buf = NULL;
	uint32_t i;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	buffer = malloc(buffer_len);
	if (!buffer) {
		E_TRACE("test hmac malloc buffer %uByte error!\n", buffer_len);
		goto exit;
	}

	mem_buf = rk_crypto_mem_alloc(buffer_len);
	if (!mem_buf) {
		E_TRACE("test hmac rk_crypto_mem_alloc %uByte error!\n", buffer_len);
		goto exit;
	}

	for (i = 0; i < ARRAY_SIZE(test_hmac_tbl); i++) {
		res = test_hash_item_virt(&test_hmac_tbl[i], buffer, buffer_len, true, verbose);
		if (res)
			goto exit;

		res = test_hash_item_fd(&test_hmac_tbl[i], mem_buf, true, verbose);
		if (res)
			goto exit;
	}
exit:
	rk_crypto_mem_free(mem_buf);
	rk_crypto_deinit();
	if (buffer)
		free(buffer);
	return res;
}
