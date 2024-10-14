/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "cmode_adapter.h"
#include "test_ae.h"
#include "test_utils.h"

#define DATA_BUTT	0xFFFFFFFF
#define TEST_DATA_MAX	(1024 * 1024)
#define TEST_AAD_MAX	(1 * 1024)

struct test_ae_item {
	uint32_t algo[2];
	uint32_t modes[RK_CIPHER_MODE_MAX];
	uint32_t key_lens[3];
	uint32_t data_lens[3];
	uint32_t aad_lens[3];
};

static struct test_ae_item test_item_tbl[] = {
	{
		.algo  = {RK_ALGO_AES, RK_ALGO_SM4},
		.modes = {
			RK_CIPHER_MODE_GCM,
			DATA_BUTT,
		},
		.key_lens = {16, 24, 32},
		.data_lens  = {256, 32 * 1024, TEST_DATA_MAX},
		.aad_lens = {20, 256, TEST_AAD_MAX},
	},
};

static RK_RES test_ae_item_virt(const struct test_ae_item *item, int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t h, i, k, l, n;
	rk_handle handle = 0;
	rk_ae_config ae_cfg;
	uint8_t *plain = NULL, *ae_soft = NULL, *ae_hard = NULL, *aad = NULL;
	uint8_t iv_tmp[32];
	uint32_t algo = 0, mode = 0, key_len, iv_len, data_len, aad_len, operation;
	uint32_t tag_len = 16;
	uint8_t tag_hard[16], tag_soft[16];
	size_t page_size = getpagesize();

	if (posix_memalign((void *)&plain, page_size, TEST_DATA_MAX) || !plain) {
		printf("plain malloc %dByte error!\n", TEST_DATA_MAX);
		goto exit;
	}

	if (posix_memalign((void *)&ae_soft, page_size, TEST_DATA_MAX) || !ae_soft) {
		printf("ae_soft malloc %dByte error!\n", TEST_DATA_MAX);
		goto exit;
	}

	if (posix_memalign((void *)&ae_hard, page_size, TEST_DATA_MAX) || !ae_hard) {
		printf("ae_hard malloc %dByte error!\n", TEST_DATA_MAX);
		goto exit;
	}

	if (posix_memalign((void *)&aad, page_size, TEST_AAD_MAX) || !aad) {
		printf("aad malloc %dByte error!\n", TEST_AAD_MAX);
		goto exit;
	}

	iv_len  = 12;

	for (h = 0; h < ARRAY_SIZE(item->algo); h++) {
		algo = item->algo[h];

		for (i = 0; i < ARRAY_SIZE(item->modes); i++) {
			mode = item->modes[i];

			if (mode == DATA_BUTT)
				break;

			for (k = 0; k < ARRAY_SIZE(item->key_lens); k++) {
				key_len = item->key_lens[k];

				if (algo == RK_ALGO_SM4 && key_len !=16)
					continue;

				for (l = 0; l < ARRAY_SIZE(item->data_lens); l++) {
					data_len  = item->data_lens[l];
					test_get_rng(plain, data_len);

					for (n = 0; n < ARRAY_SIZE(item->aad_lens); n++) {
						aad_len = item->aad_lens[n];

						memset(ae_soft, 0x00, data_len);
						memset(ae_hard, 0x00, data_len);
						memset(tag_hard, 0x00, sizeof(tag_hard));
						memset(tag_soft, 0x00, sizeof(tag_soft));

						/* encryption */
						operation = RK_OP_CIPHER_ENC;

						memset(&ae_cfg, 0x00, sizeof(ae_cfg));
						ae_cfg.algo         = algo;
						ae_cfg.mode         = mode;
						ae_cfg.operation    = operation;
						ae_cfg.key_len      = key_len;
						ae_cfg.iv_len       = iv_len;
						ae_cfg.aad_len      = aad_len;
						ae_cfg.tag_len      = tag_len;

						test_get_rng(ae_cfg.key, key_len);
						test_get_rng(ae_cfg.iv, iv_len);
						test_get_rng(aad, aad_len);
						memcpy(iv_tmp, ae_cfg.iv, iv_len);

						res = rk_ae_init(&ae_cfg, &handle);
						if (res) {
							if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
								printf("rk_ae_init error[%x]\n", res);
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
							data_len = data_len - 3;

						res = rk_ae_set_aad_virt(handle, aad);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_set_aad_virt error[%x]\n", res);
							goto exit;
						}

						res = rk_ae_crypt_virt(handle, plain, ae_hard, data_len, tag_hard);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_crypt_virt error[%x]\n", res);
							goto exit;
						}

						rk_ae_final(handle);

						res = soft_ae(algo, mode, operation,
								ae_cfg.key, ae_cfg.key_len, iv_tmp, iv_len,
								aad, ae_cfg.aad_len, ae_cfg.tag_len,
								plain, data_len, ae_soft, tag_soft);
						if (res) {
							printf("soft_ae error[%x]\n", res);
							goto exit;
						}

						/* Verify the result */
						if (memcmp(ae_hard, ae_soft, data_len) != 0) {
							printf("rkcrypto_test_ae_virt compare data failed.\n");
							res = RK_CRYPTO_ERR_GENERIC;
							goto exit;
						}

						if (memcmp(tag_hard, tag_soft, ae_cfg.tag_len) != 0) {
							printf("rkcrypto_test_ae_virt compare tag failed.\n");
							res = RK_CRYPTO_ERR_GENERIC;
							goto exit;
						}

						if (verbose)
							printf("virt:\t[%s-%u]\t%s\t%s\tPASS\n",
								test_algo_name(algo), key_len * 8,
								test_mode_name(mode), test_op_name(operation));

						/* decryption */
						operation = RK_OP_CIPHER_DEC;
						ae_cfg.operation = operation;
						memcpy(ae_cfg.iv, iv_tmp, iv_len);

						res = rk_ae_init(&ae_cfg, &handle);
						if (res) {
							if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
								printf("rk_ae_init error[%x]\n", res);
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

						res = rk_ae_set_aad_virt(handle, aad);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_set_aad_virt error[%x]\n", res);
							goto exit;
						}

						res = rk_ae_crypt_virt(handle, ae_hard, ae_hard, data_len, tag_hard);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_crypt_virt error[%x]\n", res);
							goto exit;
						}

						rk_ae_final(handle);

						/* Verify the result */
						if (memcmp(ae_hard, plain, data_len) != 0) {
							printf("rkcrypto_test_ae_virt compare data failed.\n");
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
		}
	}

	res = RK_CRYPTO_SUCCESS;
exit:
	if (plain)
		free(plain);

	if (ae_soft)
		free(ae_soft);

	if (ae_hard)
		free(ae_hard);

	if (aad)
		free(aad);

	if (res)
		printf("virt:\t[%s-%u]\t%s\t%s\tFAIL\n",
		       test_algo_name(algo), key_len * 8,
		       test_mode_name(mode), test_op_name(operation));

	return res;
}

static RK_RES test_ae_item_fd(const struct test_ae_item *item, int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t h, i, k, l, n;
	rk_handle handle = 0;
	rk_ae_config ae_cfg;
	rk_crypto_mem *plain = NULL, *ae_soft = NULL, *ae_hard = NULL, *aad = NULL;
	uint8_t iv_tmp[32];
	uint32_t algo = 0, mode = 0, key_len, iv_len, data_len, aad_len, operation;
	uint32_t tag_len = 16;
	uint8_t tag_hard[16], tag_soft[16];

	plain = rk_crypto_mem_alloc(TEST_DATA_MAX);
	if (!plain) {
		printf("plain malloc %dByte error!\n", TEST_DATA_MAX);
		goto exit;
	}

	ae_soft = rk_crypto_mem_alloc(TEST_DATA_MAX);
	if (!ae_soft) {
		printf("ae_soft malloc %dByte error!\n", TEST_DATA_MAX);
		goto exit;
	}

	ae_hard = rk_crypto_mem_alloc(TEST_DATA_MAX);
	if (!ae_hard) {
		printf("ae_hard malloc %dByte error!\n", TEST_DATA_MAX);
		goto exit;
	}

	aad = rk_crypto_mem_alloc(TEST_AAD_MAX);
	if (!aad) {
		printf("aad malloc %dByte error!\n", TEST_AAD_MAX);
		goto exit;
	}

	memset(tag_hard, 0x00, sizeof(tag_hard));
	memset(tag_soft, 0x00, sizeof(tag_soft));

	iv_len  = 12;
	for (h = 0; h < ARRAY_SIZE(item->algo); h++) {
		algo = item->algo[h];

		for (i = 0; i < ARRAY_SIZE(item->modes); i++) {
			mode = item->modes[i];

			if (mode == DATA_BUTT)
				break;

			for (k = 0; k < ARRAY_SIZE(item->key_lens); k++) {
				key_len = item->key_lens[k];

				if (algo == RK_ALGO_SM4 && key_len !=16)
						continue;

				for (l = 0; l < ARRAY_SIZE(item->data_lens); l++) {
					data_len  = item->data_lens[l];
					test_get_rng(plain->vaddr, data_len);

					for (n = 0; n < ARRAY_SIZE(item->aad_lens); n++) {
						aad_len = item->aad_lens[n];

						/* encryption */
						operation = RK_OP_CIPHER_ENC;

						memset(&ae_cfg, 0x00, sizeof(ae_cfg));
						ae_cfg.algo       = algo;
						ae_cfg.mode       = mode;
						ae_cfg.operation  = operation;
						ae_cfg.key_len    = key_len;
						ae_cfg.iv_len     = iv_len;
						ae_cfg.aad_len    = aad_len;
						ae_cfg.tag_len    = tag_len;

						test_get_rng(ae_cfg.key, key_len);
						test_get_rng(ae_cfg.iv, iv_len);
						test_get_rng(aad->vaddr, aad_len);
						memcpy(iv_tmp, ae_cfg.iv, iv_len);

						res = rk_ae_init(&ae_cfg, &handle);
						if (res) {
							if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
								printf("rk_ae_init error[%x]\n", res);
								goto exit;
							}

							if (verbose)
								printf("dma_fd:\t[%s-%u]\t%s\t%s\tN/A\n",
									test_algo_name(algo), key_len * 8,
									test_mode_name(mode),
									test_op_name(operation));
							res = RK_CRYPTO_SUCCESS;
							continue;
						}

						if (is_no_multi_blocksize(mode))
							data_len = data_len - 3;

						res = rk_ae_set_aad(handle, aad->dma_fd);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_set_aad_virt error[%x]\n", res);
							goto exit;
						}

						res = rk_ae_crypt(handle, plain->dma_fd, ae_hard->dma_fd, data_len, tag_hard);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_crypt error[%x]\n", res);
							goto exit;
						}

						rk_ae_final(handle);

						res = soft_ae(algo, mode, operation,
								ae_cfg.key, ae_cfg.key_len, iv_tmp, iv_len,
								aad->vaddr, aad_len, ae_cfg.tag_len,
								plain->vaddr, data_len, ae_soft->vaddr, tag_soft);
						if (res) {
							printf("soft_ae error[%x]\n", res);
							goto exit;
						}

						/* Verify the result */
						if (memcmp(ae_hard->vaddr, ae_soft->vaddr, data_len) != 0) {
							printf("rkcrypto_test_ae compare data failed.\n");
							res = RK_CRYPTO_ERR_GENERIC;
							goto exit;
						}

						if (memcmp(tag_hard, tag_soft, ae_cfg.tag_len) != 0) {
							printf("rkcrypto_test_ae compare tag failed.\n");
							res = RK_CRYPTO_ERR_GENERIC;
							goto exit;
						}

						if (verbose)
							printf("dma_fd:\t[%s-%u]\t%s\t%s\tPASS\n",
								test_algo_name(algo), key_len * 8,
								test_mode_name(mode), test_op_name(operation));

						/* decryption */
						operation = RK_OP_CIPHER_DEC;
						ae_cfg.operation = operation;
						memcpy(ae_cfg.iv, iv_tmp, iv_len);

						res = rk_ae_init(&ae_cfg, &handle);
						if (res) {
							if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
								printf("rk_ae_init error[%x]\n", res);
								goto exit;
							}

							if (verbose)
								printf("dma_fd:\t[%s-%u]\t%s\t%s\tN/A\n",
									test_algo_name(algo), key_len * 8,
									test_mode_name(mode),
									test_op_name(operation));
							res = RK_CRYPTO_SUCCESS;
							continue;
						}

						res = rk_ae_set_aad(handle, aad->dma_fd);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_set_aad_virt error[%x]\n", res);
							goto exit;
						}

						res = rk_ae_crypt(handle, ae_hard->dma_fd, ae_hard->dma_fd, data_len, tag_hard);
						if (res) {
							rk_ae_final(handle);
							printf("rk_ae_crypt error[%x]\n", res);
							goto exit;
						}

						rk_ae_final(handle);

						/* Verify the result */
						if (memcmp(ae_hard->vaddr, plain->vaddr, data_len) != 0) {
							printf("rkcrypto_test_ae compare data failed.\n");
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
		}
	}

	res = RK_CRYPTO_SUCCESS;
exit:
	if (plain)
		rk_crypto_mem_free(plain);

	if (ae_soft)
		rk_crypto_mem_free(ae_soft);

	if (ae_hard)
		rk_crypto_mem_free(ae_hard);

	if (aad)
		rk_crypto_mem_free(aad);

	if (res)
		printf("dma_fd:\t[%s-%u]\t%s\t%s\tFAIL\n",
		       test_algo_name(algo), key_len * 8,
		       test_mode_name(mode), test_op_name(operation));

	return res;
}

RK_RES test_ae(int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t i;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	for (i = 0; i < ARRAY_SIZE(test_item_tbl); i++) {
		res = test_ae_item_virt(&test_item_tbl[i], verbose);
		if (res)
			goto exit;

		res = test_ae_item_fd(&test_item_tbl[i], verbose);
		if (res)
			goto exit;

		if (verbose)
			printf("\n");
	}

exit:
	rk_crypto_deinit();
	return res;
}

