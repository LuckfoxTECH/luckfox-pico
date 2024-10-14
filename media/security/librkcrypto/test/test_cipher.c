/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_trace.h"
#include "cmode_adapter.h"
#include "test_cipher.h"
#include "test_utils.h"

#define DATA_BUTT	0xFFFFFFFF
#define TEST_DATA_MAX	(1024 * 1024)
#define MULTI_BLOCKSIZE	(256 * 1024)

struct test_cipher_item {
	uint32_t algo;
	uint32_t modes[RK_CIPHER_MODE_MAX];
	uint32_t key_lens[4];
	uint32_t iv_len;
};

static struct test_cipher_item test_item_tbl[] = {
{
	.algo  = RK_ALGO_DES,
	.modes = {
		RK_CIPHER_MODE_ECB,
		RK_CIPHER_MODE_CBC,
		RK_CIPHER_MODE_CFB,
		RK_CIPHER_MODE_OFB,
		DATA_BUTT,
	},
	.key_lens = {8},
	.iv_len   = DES_BLOCK_SIZE,
},

{
	.algo  = RK_ALGO_TDES,
	.modes = {
		RK_CIPHER_MODE_ECB,
		RK_CIPHER_MODE_CBC,
		RK_CIPHER_MODE_CFB,
		RK_CIPHER_MODE_OFB,
		DATA_BUTT,
	},
	.key_lens = {24},
	.iv_len   = DES_BLOCK_SIZE,
},

{
	.algo  = RK_ALGO_AES,
	.modes = {
		RK_CIPHER_MODE_ECB,
		RK_CIPHER_MODE_CBC,
		RK_CIPHER_MODE_CTS,
		RK_CIPHER_MODE_CTR,
		RK_CIPHER_MODE_CFB,
		RK_CIPHER_MODE_OFB,
		RK_CIPHER_MODE_XTS,
		DATA_BUTT,
	},
	.key_lens = {16, 24, 32},
	.iv_len   = AES_BLOCK_SIZE,
},

{
	.algo  = RK_ALGO_SM4,
	.modes = {
		RK_CIPHER_MODE_ECB,
		RK_CIPHER_MODE_CBC,
		RK_CIPHER_MODE_CTS,
		RK_CIPHER_MODE_CTR,
		RK_CIPHER_MODE_CFB,
		RK_CIPHER_MODE_OFB,
		RK_CIPHER_MODE_XTS,
		DATA_BUTT,
	},
	.key_lens = {16},
	.iv_len   = SM4_BLOCK_SIZE,
},

};

static RK_RES test_cipher_item_virt(const struct test_cipher_item *item, int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t i, j, k;
	uint32_t ops[] = {RK_OP_CIPHER_ENC, RK_OP_CIPHER_DEC};
	uint32_t data_len = TEST_DATA_MAX;
	rk_handle cipher_hdl = 0;
	rk_cipher_config cipher_cfg;
	uint8_t *plain = NULL, *cipher_soft = NULL, *cipher_hard = NULL;
	uint32_t algo = 0, mode = 0, key_len, iv_len, operation;
	uint32_t loop_nbytes, tmp_len;
	uint8_t *tmp_data_in, *tmp_data_out;
	uint8_t iv_tmp[AES_BLOCK_SIZE];
	size_t page_size = getpagesize();

	if (posix_memalign((void *)&plain, page_size, data_len) || !plain) {
		E_TRACE("plain malloc %uByte error!\n", data_len);
		goto exit;
	}

	if (posix_memalign((void *)&cipher_soft, page_size, data_len) || !cipher_soft) {
		E_TRACE("cipher_soft malloc %uByte error!\n", data_len);
		goto exit;
	}

	if (posix_memalign((void *)&cipher_hard, page_size, data_len) || !cipher_hard) {
		E_TRACE("cipher_hard malloc %uByte error!\n", data_len);
		goto exit;
	}

	test_get_rng(plain, data_len);

	memset(cipher_soft, 0x00, data_len);
	memset(cipher_hard, 0x00, data_len);

	for (i = 0; i < ARRAY_SIZE(item->modes); i++) {
		algo = item->algo;
		mode = item->modes[i];

		if (mode == DATA_BUTT)
			break;

		for (j = 0; j < ARRAY_SIZE(item->key_lens); j++) {
			key_len = item->key_lens[j];
			iv_len  = item->iv_len;

			if (key_len == 0)
				break;

			if (mode == RK_CIPHER_MODE_XTS)
				key_len *= 2;

			for (k = 0; k < ARRAY_SIZE(ops); k++) {
				operation = ops[k];

				memset(&cipher_cfg, 0x00, sizeof(cipher_cfg));
				cipher_cfg.algo      = algo;
				cipher_cfg.mode      = mode;
				cipher_cfg.operation = operation;
				cipher_cfg.key_len   = key_len;

				test_get_rng(cipher_cfg.key, key_len);
				test_get_rng(cipher_cfg.iv, iv_len);
				memcpy(iv_tmp, cipher_cfg.iv, iv_len);

				res = rk_cipher_init(&cipher_cfg, &cipher_hdl);
				if (res) {
					if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
						E_TRACE("rk_cipher_init error[%x]\n", res);
						goto exit;
					}

					if (verbose)
						printf("virt:\t[%s-%u]\t%s\t%s\tN/A\n",
						       test_algo_name(algo), key_len * 8,
						       test_mode_name(mode),
						       test_op_name(operation));
					res = RK_CRYPTO_SUCCESS;
					continue;
				}

				if (is_no_multi_blocksize(mode))
					data_len = TEST_DATA_MAX - 3;
				else
					data_len = TEST_DATA_MAX;

				/* chain multi crypt */
				loop_nbytes  = data_len;
				tmp_data_in  = plain;
				tmp_data_out = cipher_hard;

				while (loop_nbytes) {

					tmp_len = loop_nbytes > MULTI_BLOCKSIZE ? MULTI_BLOCKSIZE : loop_nbytes;

					res = rk_cipher_crypt_virt(cipher_hdl, tmp_data_in,
								   tmp_data_out, tmp_len);
					if (res) {
						rk_cipher_final(cipher_hdl);
						E_TRACE("rk_cipher_crypt_virt error[%x]\n", res);
						goto exit;
					}

					tmp_data_in  += tmp_len;
					tmp_data_out += tmp_len;
					loop_nbytes  -= tmp_len;
				}

				rk_cipher_final(cipher_hdl);
				cipher_hdl = 0;

				res = soft_cipher(algo, mode, operation,
						  cipher_cfg.key, cipher_cfg.key_len, iv_tmp,
						  plain, data_len, cipher_soft);
				if (res) {
					E_TRACE("soft_cipher error[%x]\n", res);
					goto exit;
				}

				/* Verify the result */
				if (memcmp(cipher_hard, cipher_soft, data_len) != 0) {
					E_TRACE("rkcrypto_test_cipher_virt compare failed.\n");
//					test_dump_hex("cipher_hard", cipher_hard, data_len);
//					test_dump_hex("cipher_soft", cipher_soft, data_len);
					res = RK_CRYPTO_ERR_GENERIC;
					goto exit;
				}

				if (verbose)
					printf("virt:\t[%s-%u]\t%s\t%s\tPASS\n",
					       test_algo_name(algo), key_len * 8,
					       test_mode_name(mode), test_op_name(operation));
			}
		}
	}

	res = RK_CRYPTO_SUCCESS;
exit:
	if (plain)
		free(plain);

	if (cipher_soft)
		free(cipher_soft);

	if (cipher_hard)
		free(cipher_hard);

	if (res)
		printf("virt:\t[%s-%u]\t%s\t%s\tFAIL\n",
		       test_algo_name(algo), key_len * 8,
		       test_mode_name(mode), test_op_name(operation));

	return res;
}

static RK_RES test_cipher_item_fd(const struct test_cipher_item *item, int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t i, j, k;
	uint32_t ops[] = {RK_OP_CIPHER_ENC, RK_OP_CIPHER_DEC};
	uint32_t data_len = TEST_DATA_MAX;
	rk_handle cipher_hdl = 0;
	rk_cipher_config cipher_cfg;
	rk_crypto_mem *plain = NULL, *cipher_soft = NULL, *cipher_hard = NULL;
	uint32_t algo = 0, mode = 0, key_len = 0, iv_len, operation;
	uint8_t iv_tmp[AES_BLOCK_SIZE];

	plain = rk_crypto_mem_alloc(data_len);
	if (!plain) {
		E_TRACE("plain malloc %uByte error!\n", data_len);
		goto exit;
	}

	cipher_soft = rk_crypto_mem_alloc(data_len);
	if (!cipher_soft) {
		E_TRACE("cipher_soft malloc %uByte error!\n", data_len);
		goto exit;
	}

	cipher_hard = rk_crypto_mem_alloc(data_len);
	if (!cipher_hard) {
		E_TRACE("cipher_hard malloc %uByte error!\n", data_len);
		goto exit;
	}

	test_get_rng(plain->vaddr, data_len);

	for (i = 0; i < ARRAY_SIZE(item->modes); i++) {
		algo = item->algo;
		mode = item->modes[i];

		if (mode == DATA_BUTT)
			break;

		for (j = 0; j < ARRAY_SIZE(item->key_lens); j++) {
			key_len = item->key_lens[j];
			iv_len  = item->iv_len;

			if (key_len == 0)
				break;

			if (mode == RK_CIPHER_MODE_XTS)
				key_len *= 2;

			for (k = 0; k < ARRAY_SIZE(ops); k++) {
				operation = ops[k];

				memset(&cipher_cfg, 0x00, sizeof(cipher_cfg));
				cipher_cfg.algo      = algo;
				cipher_cfg.mode      = mode;
				cipher_cfg.operation = operation;
				cipher_cfg.key_len   = key_len;

				test_get_rng(cipher_cfg.key, key_len);
				test_get_rng(cipher_cfg.iv, iv_len);
				memcpy(iv_tmp, cipher_cfg.iv, iv_len);

				res = rk_cipher_init(&cipher_cfg, &cipher_hdl);
				if (res) {
					if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
						E_TRACE("rk_cipher_init error[%x]\n", res);
						goto exit;
					}

					if (verbose)
						printf("dma_fd:\t[%s-%u]\t%s\t%s\tN/A\n",
						       test_algo_name(algo), key_len * 8,
						       test_mode_name(mode), test_op_name(operation));
					res = RK_CRYPTO_SUCCESS;
					continue;
				}

				if (is_no_multi_blocksize(mode))
					data_len = TEST_DATA_MAX - 7;
				else
					data_len = TEST_DATA_MAX;

				res = rk_cipher_crypt(cipher_hdl, plain->dma_fd,
						      cipher_hard->dma_fd, data_len);
				if (res) {
					rk_cipher_final(cipher_hdl);
					E_TRACE("rk_cipher_crypt error[%x]\n", res);
					goto exit;
				}

				rk_cipher_final(cipher_hdl);
				cipher_hdl = 0;

				res = soft_cipher(algo, mode, operation,
						  cipher_cfg.key, cipher_cfg.key_len, iv_tmp,
						  plain->vaddr, data_len, cipher_soft->vaddr);
				if (res) {
					E_TRACE("soft_cipher error[%x]\n", res);
					goto exit;
				}

				/* Verify the result */
				if (memcmp(cipher_hard->vaddr, cipher_soft->vaddr, data_len) != 0) {
					E_TRACE("rkcrypto_test_cipher compare failed.\n");
//					test_dump_hex("cipher_hard", cipher_hard->vaddr, data_len);
//					test_dump_hex("cipher_soft", cipher_soft->vaddr, data_len);
					res = RK_CRYPTO_ERR_GENERIC;
					goto exit;
				}

				if (verbose)
					printf("dma_fd:\t[%s-%u]\t%s\t%s\tPASS\n",
					       test_algo_name(algo), key_len * 8,
					       test_mode_name(mode), test_op_name(operation));
			}
		}
	}

	res = RK_CRYPTO_SUCCESS;
exit:
	rk_crypto_mem_free(plain);
	rk_crypto_mem_free(cipher_soft);
	rk_crypto_mem_free(cipher_hard);

	if (res && key_len)
		printf("dma_fd:\t[%s-%u]\t%s\t%s\tFAIL\n",
		       test_algo_name(algo), key_len * 8,
		       test_mode_name(mode), test_op_name(operation));

	return res;
}

RK_RES test_cipher(int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t i;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	for (i = 0; i < ARRAY_SIZE(test_item_tbl); i++) {
		res = test_cipher_item_virt(&test_item_tbl[i], verbose);
		if (res)
			goto exit;

		res = test_cipher_item_fd(&test_item_tbl[i], verbose);
		if (res)
			goto exit;
		if (verbose)
			printf("\n");
	}
exit:
	rk_crypto_deinit();
	return res;
}

