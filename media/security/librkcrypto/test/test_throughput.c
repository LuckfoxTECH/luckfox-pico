/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_otp_key.h"
#include "rkcrypto_random.h"
#include "test_utils.h"
#include "rsa_key_data.h"
#include "test_throughput.h"

#define TEST_BLOCK_SIZE		1024 * 1024	/* 1MB */
#define TEST_OTP_BLOCK_SIZE	500 * 1024
#define DURATION		1		/* 1s */
#define DATA_BUTT		0xFFFFFFFF

static int test_otp_key_item_tp(bool is_virt, uint32_t key_id, uint32_t key_len,
				uint32_t algo, uint32_t mode, uint32_t operation,
				rk_crypto_mem *fd, uint32_t data_len)
{
	uint32_t res = 0;
	rk_cipher_config config;
	uint8_t iv[16];
	uint8_t in_out[RK_CRYPTO_MAX_DATA_LEN];
	struct timespec start, end;
	uint64_t total_nsec, nsec;
	uint32_t rounds;

	nsec = DURATION * 1000000000;

	memset(iv, 0x00, sizeof(iv));
	test_get_rng(iv, sizeof(iv));

	if (is_virt) {
		memset(in_out, 0x00, sizeof(in_out));
		test_get_rng(in_out, data_len);
	}

	memcpy(config.iv, iv, sizeof(iv));

	config.algo      = algo;
	config.mode      = mode;
	config.key_len   = key_len;
	config.reserved  = NULL;
	config.operation = operation;
	total_nsec = 0;
	rounds = 0;

	while (total_nsec < nsec) {
		clock_gettime(CLOCK_REALTIME, &start);

		if (is_virt)
			res = rk_oem_otp_key_cipher_virt(key_id, &config, in_out, in_out, data_len);
		else
			res = rk_oem_otp_key_cipher(key_id, &config, fd->dma_fd, fd->dma_fd, data_len);

		if (res == RK_CRYPTO_ERR_NOT_SUPPORTED) {
			if (is_virt)
				printf("virt:\totpkey\t[%s-%u]\t%s\t%s\tN/A.\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode),
				       test_op_name(operation));
			else
				printf("dma_fd:\totpkey\t[%s-%u]\t%s\t%s\tN/A.\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode),
				       test_op_name(operation));

			return RK_CRYPTO_SUCCESS;
		} else if (res) {
			printf("test rk_oem_otp_key_cipher failed! 0x%08x\n", res);
			return res;
		}

		clock_gettime(CLOCK_REALTIME, &end);
		total_nsec += (end.tv_sec - start.tv_sec) * 1000000000 +
			      (end.tv_nsec - start.tv_nsec);
		rounds ++;
	}

	if (is_virt)
		printf("virt:\totpkey\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(operation), (data_len * rounds / (1024 * 1024)));
	else
		printf("dma_fd:\totpkey\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(operation), (data_len * rounds / (1024 * 1024)));

	return res;
}

static int test_otp_key_virt_tp(void)
{
	uint32_t h, j, k;
	uint32_t algo, key_id, mode, operation, len, key_len;

	const uint32_t algo_tab[] = {
		RK_ALGO_AES,
		RK_ALGO_SM4,
	};
	const uint32_t mode_tab[] = {
		RK_CIPHER_MODE_ECB,
		RK_CIPHER_MODE_CBC,
		RK_CIPHER_MODE_CTR,
	};
	const uint32_t op_tab[] = {
		RK_OP_CIPHER_ENC,
		RK_OP_CIPHER_DEC,
	};

	for (h = 0; h < ARRAY_SIZE(algo_tab); h++) {
		for (j = 0; j < ARRAY_SIZE(mode_tab); j++) {
			for (k = 0; k < ARRAY_SIZE(op_tab); k++) {
				algo      = algo_tab[h];
				mode      = mode_tab[j];
				operation = op_tab[k];
				key_id    = RK_OEM_OTP_KEY3;
				len       = TEST_OTP_BLOCK_SIZE;

				if (algo == RK_ALGO_AES) {
					key_len = 32;
				} else {
					key_len = 16;
				}

				if (test_otp_key_item_tp(true, key_id, key_len, algo,
							 mode, operation, NULL, len))
					goto error;
			}
		}
	}

	printf("virt:\ttest otp_key throughput SUCCESS.\n\n");
	return 0;

error:
	printf("virt:\ttest otp_key throughput FAILED!!!\n\n");
	return -1;
}

static int test_otp_key_fd_tp(void)
{
	int res = 0;
	uint32_t h, j, k;
	uint32_t algo, key_id, mode, operation, len, key_len;
	rk_crypto_mem *in_out = NULL;

	const uint32_t algo_tab[] = {
		RK_ALGO_AES,
		RK_ALGO_SM4,
	};
	const uint32_t mode_tab[] = {
		RK_CIPHER_MODE_ECB,
		RK_CIPHER_MODE_CBC,
		RK_CIPHER_MODE_CTR,
	};
	const uint32_t op_tab[] = {
		RK_OP_CIPHER_ENC,
		RK_OP_CIPHER_DEC,
	};

	if (rk_crypto_init()) {
		printf("rk_crypto_init error!\n");
		return -1;
	}

	in_out = rk_crypto_mem_alloc(TEST_OTP_BLOCK_SIZE);
	if (!in_out) {
		printf("rk_crypto_mem_alloc %uByte error!\n", TEST_OTP_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	for (h = 0; h < ARRAY_SIZE(algo_tab); h++) {
		for (j = 0; j < ARRAY_SIZE(mode_tab); j++) {
			for (k = 0; k < ARRAY_SIZE(op_tab); k++) {
				algo      = algo_tab[h];
				mode      = mode_tab[j];
				operation = op_tab[k];
				key_id    = RK_OEM_OTP_KEY3;
				len       = TEST_OTP_BLOCK_SIZE;

				if (algo == RK_ALGO_AES) {
					key_len = 32;
				} else {
					key_len = 16;
				}

				if (test_otp_key_item_tp(false, key_id, key_len, algo,
							 mode, operation, in_out, len)) {
					printf("dma_fd:\ttest otp_key throughput FAILED!!!\n");
					res = -1;
					goto out;
				}
			}
		}
	}

	printf("dma_fd:\ttest otp_key throughput SUCCESS.\n\n");

out:
	if (!in_out)
		rk_crypto_mem_free(in_out);

	rk_crypto_deinit();
	return res;
}

static int test_otp_key_tp(void)
{
	if (test_otp_key_fd_tp())
		return -1;

	if (test_otp_key_virt_tp())
		return -1;

	return 0;
}

static int test_cipher_item_tp(bool is_virt, uint32_t key_len, uint32_t algo,
			       uint32_t mode, uint32_t operation,
			       void *in_out, uint32_t data_len)
{
	uint32_t res = 0;
	rk_handle handle = 0;
	rk_cipher_config config;
	struct timespec start, end;
	uint64_t total_nsec, nsec;
	uint32_t rounds;

	nsec = DURATION * 1000000000;

	test_get_rng(config.iv, sizeof(config.iv));
	test_get_rng(config.key, key_len);

	if (is_virt)
		test_get_rng(in_out, data_len);

	config.algo      = algo;
	config.mode      = mode;
	config.key_len   = key_len;
	config.reserved  = NULL;
	config.operation = operation;
	total_nsec = 0;
	rounds = 0;

	while (total_nsec < nsec) {
		clock_gettime(CLOCK_REALTIME, &start);

		res = rk_cipher_init(&config, &handle);
		if (res) {
			if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
				printf("test rk_cipher_init failed! 0x%08x\n", res);
				goto error;
			}

			if (is_virt)
				printf("virt:\t[%s-%u]\t%s\tN/A\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode));
			else
				printf("dma_fd:\t[%s-%u]\t%s\tN/A\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode));
			return 0;
		}

		if (is_virt)
			res = rk_cipher_crypt_virt(handle, in_out, in_out, data_len);
		else
			res = rk_cipher_crypt(handle,
					      ((rk_crypto_mem *)in_out)->dma_fd,
					      ((rk_crypto_mem *)in_out)->dma_fd,
					      data_len);

		if (res) {
			rk_cipher_final(handle);
			printf("test rk_cipher_crypt failed! 0x%08x\n", res);
			goto error;
		}

		rk_cipher_final(handle);

		clock_gettime(CLOCK_REALTIME, &end);
		total_nsec += (end.tv_sec - start.tv_sec) * 1000000000 +
			      (end.tv_nsec - start.tv_nsec);
		rounds ++;
	}

	if (is_virt)
		printf("virt:\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(operation), (data_len * rounds / (1024 * 1024)));
	else
		printf("dma_fd:\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(operation), (data_len * rounds / (1024 * 1024)));

	return res;
error:
	if (is_virt)
		printf("virt:\t[%s-%u]\t%s\tFailed.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode));
	else
		printf("dma_fd:\t[%s-%u]\t%s\tFailed.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode));
	return res;
}

static int test_cipher_tp(int throughput_mode)
{
	int res = 0;
	uint32_t h, j, k;
	uint32_t algo, mode, operation, len, key_len;
	rk_crypto_mem *in_out_fd = NULL;
	uint8_t *in_out_virt = NULL;
	size_t page_size = getpagesize();

	struct test_cipher_item_tp {
		uint32_t algo;
		uint32_t modes[RK_CIPHER_MODE_MAX];
		uint32_t key_len;
		uint32_t op[2];
	};

	static struct test_cipher_item_tp test_item_tbl[] = {
		{
			.algo  = RK_ALGO_DES,
			.modes = {
				RK_CIPHER_MODE_ECB,
				RK_CIPHER_MODE_CBC,
				DATA_BUTT,
			},
			.key_len = 8,
			.op = {RK_OP_CIPHER_ENC, RK_OP_CIPHER_DEC},
		},

		{
			.algo  = RK_ALGO_TDES,
			.modes = {
				RK_CIPHER_MODE_ECB,
				RK_CIPHER_MODE_CBC,
				DATA_BUTT,
			},
			.key_len = 24,
			.op = {RK_OP_CIPHER_ENC, RK_OP_CIPHER_DEC},
		},

		{
			.algo  = RK_ALGO_AES,
			.modes = {
				RK_CIPHER_MODE_ECB,
				RK_CIPHER_MODE_CBC,
				RK_CIPHER_MODE_CTS,
				RK_CIPHER_MODE_CTR,
				DATA_BUTT,
			},
			.key_len = 32,
			.op = {RK_OP_CIPHER_ENC, RK_OP_CIPHER_DEC},
		},

		{
			.algo  = RK_ALGO_SM4,
			.modes = {
				RK_CIPHER_MODE_ECB,
				RK_CIPHER_MODE_CBC,
				RK_CIPHER_MODE_CTS,
				RK_CIPHER_MODE_CTR,
				DATA_BUTT,
			},
			.key_len = 16,
			.op = {RK_OP_CIPHER_ENC, RK_OP_CIPHER_DEC},
		},

	};

	if (rk_crypto_init()) {
		printf("rk_crypto_init error!\n");
		return -1;
	}

	in_out_fd = rk_crypto_mem_alloc(TEST_BLOCK_SIZE);
	if (!in_out_fd) {
		printf("rk_crypto_mem_alloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	if (posix_memalign((void *)&in_out_virt, page_size, TEST_BLOCK_SIZE) || !in_out_virt) {
		printf("malloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	/* Test dma_fd cipher */
	for (h = 0; h < ARRAY_SIZE(test_item_tbl); h++) {
		if (throughput_mode != T_DMA_FD && throughput_mode != T_ALL)
			break;

		for (j = 0; j < ARRAY_SIZE(test_item_tbl[h].modes); j++) {
			if (test_item_tbl[h].modes[j] == DATA_BUTT)
				break;

			for (k = 0; k < ARRAY_SIZE(test_item_tbl[h].op); k++) {
				algo      = test_item_tbl[h].algo;
				key_len   = test_item_tbl[h].key_len;
				mode      = test_item_tbl[h].modes[j];
				operation = test_item_tbl[h].op[k];
				len       = TEST_BLOCK_SIZE;

				if (test_cipher_item_tp(false, key_len, algo, mode,
							operation, in_out_fd, len)) {
					printf("dma_fd:\ttest cipher throughput FAILED!!!\n");
					res = -1;
					goto out;
				}
			}
		}
	}

	if (throughput_mode == T_DMA_FD || throughput_mode == T_ALL)
		printf("dma_fd:\ttest cipher throughput SUCCESS.\n\n");

	/* Test virt cipher */
	for (h = 0; h < ARRAY_SIZE(test_item_tbl); h++) {
		if (throughput_mode != T_VIRT && throughput_mode != T_ALL)
			break;

		for (j = 0; j < ARRAY_SIZE(test_item_tbl[h].modes); j++) {
			if (test_item_tbl[h].modes[j] == DATA_BUTT)
				break;

			for (k = 0; k < ARRAY_SIZE(test_item_tbl[h].op); k++) {
				algo      = test_item_tbl[h].algo;
				key_len   = test_item_tbl[h].key_len;
				mode      = test_item_tbl[h].modes[j];
				operation = test_item_tbl[h].op[k];
				len       = TEST_BLOCK_SIZE;

				if (test_cipher_item_tp(true, key_len, algo, mode,
							operation, in_out_virt, len)) {
					printf("virt:\ttest cipher throughput FAILED!!!\n");
					res = -1;
					goto out;
				}
			}
		}
	}

	if (throughput_mode == T_VIRT || throughput_mode == T_ALL)
		printf("virt:\ttest cipher throughput SUCCESS.\n\n");

out:
	if (in_out_fd)
		rk_crypto_mem_free(in_out_fd);

	if (in_out_virt)
		free(in_out_virt);

	rk_crypto_deinit();
	return res;
}

static int test_ae_item_tp(bool is_virt, uint32_t key_len, uint32_t algo,
			   uint32_t mode, void *in, void *out, uint32_t data_len,
			   void *aad, uint32_t aad_len, void *tag)
{
	uint32_t res = 0;
	rk_handle handle = 0;
	rk_ae_config config;
	struct timespec start, end;
	uint64_t total_nsec, nsec;
	uint32_t rounds;
	uint8_t iv_tmp[32];

	/*
	  The test case of aead will fall back to the software algorithm when it is calculated,
	  and the test is meaningless.
	 */
	if (is_virt) {
		printf("virt:\t[%s-%u]\t%s\tN/A\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode));
		return 0;
	}

	nsec = DURATION * 1000000000;

	test_get_rng(config.iv, sizeof(config.iv));
	test_get_rng(config.key, key_len);

	memcpy(iv_tmp, config.iv, sizeof(config.iv));

	if (is_virt) {
		test_get_rng(in, data_len);
		test_get_rng(aad, aad_len);
	} else {
		test_get_rng(((rk_crypto_mem *)in)->vaddr, data_len);
		test_get_rng(((rk_crypto_mem *)aad)->vaddr, aad_len);
	}

	config.algo      = algo;
	config.mode      = mode;
	config.key_len   = key_len;
	config.reserved  = NULL;
	config.aad_len   = aad_len;
	config.iv_len    = 12;
	config.tag_len   = 16;

	/* ENC */
	config.operation = RK_OP_CIPHER_ENC;
	total_nsec = 0;
	rounds = 0;

	while (total_nsec < nsec) {
		clock_gettime(CLOCK_REALTIME, &start);

		res = rk_ae_init(&config, &handle);
		if (res) {
			if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
				printf("test rk_ae_init failed! 0x%08x\n", res);
				goto error;
			}

			if (is_virt)
				printf("virt:\t[%s-%u]\t%s\tN/A\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode));
			else
				printf("dma_fd:\t[%s-%u]\t%s\tN/A\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode));
			return 0;
		}

		if (is_virt) {
			res = rk_ae_set_aad_virt(handle, aad);
			if (res) {
				rk_ae_final(handle);
				goto error;
			}
			res = rk_ae_crypt_virt(handle, in, out, data_len, tag);
		} else {
			res = rk_ae_set_aad(handle, ((rk_crypto_mem *)aad)->dma_fd);
			if (res) {
				rk_ae_final(handle);
				goto error;
			}

			res = rk_ae_crypt(handle,
					  ((rk_crypto_mem *)in)->dma_fd,
					  ((rk_crypto_mem *)out)->dma_fd,
					  data_len,
					  tag);
		}

		if (res) {
			rk_ae_final(handle);
			printf("test rk_ad_crypt failed! 0x%08x\n", res);
			goto error;
		}

		rk_ae_final(handle);

		clock_gettime(CLOCK_REALTIME, &end);
		total_nsec += (end.tv_sec - start.tv_sec) * 1000000000 +
			      (end.tv_nsec - start.tv_nsec);
		rounds ++;
	}

	if (is_virt)
		printf("virt:\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(config.operation), (data_len * rounds / (1024 * 1024)));
	else
		printf("dma_fd:\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(config.operation), (data_len * rounds / (1024 * 1024)));

	/* DEC */
	config.operation = RK_OP_CIPHER_DEC;
	memcpy(config.iv, iv_tmp, config.iv_len);
	total_nsec = 0;
	rounds = 0;
	while (total_nsec < nsec) {
		clock_gettime(CLOCK_REALTIME, &start);

		res = rk_ae_init(&config, &handle);
		if (res) {
			if (res != RK_CRYPTO_ERR_NOT_SUPPORTED) {
				printf("test rk_ae_init failed! 0x%08x\n", res);
				goto error;
			}

			if (is_virt)
				printf("virt:\t[%s-%u]\t%s\tN/A\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode));
			else
				printf("dma_fd:\t[%s-%u]\t%s\tN/A\n",
				       test_algo_name(algo), key_len * 8, test_mode_name(mode));
			return 0;
		}

		if (is_virt) {
			res = rk_ae_set_aad_virt(handle, aad);
			if (res) {
				rk_ae_final(handle);
				goto error;
			}
			res = rk_ae_crypt_virt(handle, out, in, data_len, tag);
		} else {
			res = rk_ae_set_aad(handle, ((rk_crypto_mem *)aad)->dma_fd);
			if (res) {
				rk_ae_final(handle);
				goto error;
			}

			res = rk_ae_crypt(handle,
					  ((rk_crypto_mem *)out)->dma_fd,
					  ((rk_crypto_mem *)in)->dma_fd,
					  data_len,
					  tag);
		}

		if (res) {
			rk_ae_final(handle);
			printf("test rk_ad_crypt failed! 0x%08x\n", res);
			goto error;
		}

		rk_ae_final(handle);

		clock_gettime(CLOCK_REALTIME, &end);
		total_nsec += (end.tv_sec - start.tv_sec) * 1000000000 +
			      (end.tv_nsec - start.tv_nsec);
		rounds ++;
	}

	if (is_virt)
		printf("virt:\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(config.operation), (data_len * rounds / (1024 * 1024)));
	else
		printf("dma_fd:\t[%s-%u]\t%s\t%s\t%dMB/s.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode),
		       test_op_name(config.operation), (data_len * rounds / (1024 * 1024)));

	return res;
error:
	if (is_virt)
		printf("virt:\t[%s-%u]\t%s\tFailed.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode));
	else
		printf("dma_fd:\t[%s-%u]\t%s\tFailed.\n",
		       test_algo_name(algo), key_len * 8, test_mode_name(mode));
	return res;
}

static int test_ae_tp(int throughput_mode)
{
	int res = 0;
	uint32_t h, j;
	uint32_t algo, mode, len, key_len, aad_len;
	rk_crypto_mem *in_fd = NULL, *out_fd = NULL, *aad_fd = NULL;
	uint8_t *in_virt = NULL, *out_virt = NULL, *aad_virt = NULL;
	size_t page_size = getpagesize();
	uint32_t aad_buff_len = 1024;
	uint8_t tag[16];

	struct test_aead_item_tp {
		uint32_t algo;
		uint32_t modes[RK_CIPHER_MODE_MAX];
		uint32_t key_len;
		uint32_t aad_len;
		uint32_t len;
	};

	static struct test_aead_item_tp test_item_tbl[] = {
		{
			.algo  = RK_ALGO_AES,
			.modes = {
				RK_CIPHER_MODE_GCM,
				DATA_BUTT,
			},
			.key_len = 32,
			.aad_len = 1024,
			.len     = TEST_BLOCK_SIZE,
		},

		{
			.algo  = RK_ALGO_SM4,
			.modes = {
				RK_CIPHER_MODE_GCM,
				DATA_BUTT,
			},
			.key_len = 16,
			.aad_len = 1024,
			.len     = TEST_BLOCK_SIZE,
		},
	};

	if (rk_crypto_init()) {
		printf("rk_crypto_init error!\n");
		return -1;
	}

	in_fd = rk_crypto_mem_alloc(TEST_BLOCK_SIZE);
	if (!in_fd) {
		printf("rk_crypto_mem_alloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	out_fd = rk_crypto_mem_alloc(TEST_BLOCK_SIZE);
	if (!out_fd) {
		printf("rk_crypto_mem_alloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	aad_fd = rk_crypto_mem_alloc(aad_buff_len);
	if (!aad_fd) {
		printf("rk_crypto_mem_alloc %uByte error!\n", aad_buff_len);
		res = -1;
		goto out;
	}

	if (posix_memalign((void *)&in_virt, page_size, TEST_BLOCK_SIZE) || !in_virt) {
		printf("malloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	if (posix_memalign((void *)&out_virt, page_size, TEST_BLOCK_SIZE) || !out_virt) {
		printf("malloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	if (posix_memalign((void *)&aad_virt, page_size, aad_buff_len) || !aad_virt) {
		printf("malloc %uByte error!\n", aad_buff_len);
		res = -1;
		goto out;
	}

	/* Test dma_fd cipher */
	for (h = 0; h < ARRAY_SIZE(test_item_tbl); h++) {
		if (throughput_mode != T_DMA_FD && throughput_mode != T_ALL)
			break;

		for (j = 0; j < ARRAY_SIZE(test_item_tbl[h].modes); j++) {
			if (test_item_tbl[h].modes[j] == DATA_BUTT)
				break;

			algo      = test_item_tbl[h].algo;
			key_len   = test_item_tbl[h].key_len;
			mode      = test_item_tbl[h].modes[j];
			aad_len   = test_item_tbl[h].aad_len;
			len       = test_item_tbl[h].len;

			if (test_ae_item_tp(false, key_len, algo, mode,
					    in_fd, out_fd, len,
					    aad_fd, aad_len, tag)) {
				printf("dma_fd:\ttest aead throughput FAILED!!!\n");
				res = -1;
				goto out;
			}
		}
	}
	if (throughput_mode == T_DMA_FD || throughput_mode == T_ALL)
		printf("dma_fd:\ttest aead throughput SUCCESS.\n\n");

	/* Test virt cipher */
	for (h = 0; h < ARRAY_SIZE(test_item_tbl); h++) {
		if (throughput_mode != T_VIRT && throughput_mode != T_ALL)
			break;

		for (j = 0; j < ARRAY_SIZE(test_item_tbl[h].modes); j++) {
			if (test_item_tbl[h].modes[j] == DATA_BUTT)
				break;

			algo      = test_item_tbl[h].algo;
			key_len   = test_item_tbl[h].key_len;
			mode      = test_item_tbl[h].modes[j];
			aad_len   = test_item_tbl[h].aad_len;
			len       = test_item_tbl[h].len;

			if (test_ae_item_tp(true, key_len, algo, mode,
					    in_virt, out_virt, len,
					    aad_virt, aad_len, tag)) {
				printf("virt:\ttest aead throughput FAILED!!!\n");
				res = -1;
				goto out;
			}
		}
	}

	if (throughput_mode == T_VIRT || throughput_mode == T_ALL)
		printf("virt:\ttest aead throughput SUCCESS.\n\n");

out:
	if (in_fd)
		rk_crypto_mem_free(in_fd);

	if (out_fd)
		rk_crypto_mem_free(out_fd);

	if (aad_fd)
		rk_crypto_mem_free(aad_fd);

	if (in_virt)
		free(in_virt);

	if (out_virt)
		free(out_virt);

	if (aad_virt)
		free(aad_virt);

	rk_crypto_deinit();
	return res;
}

static int test_hash_item_tp(bool is_virt, bool is_hmac, uint32_t algo,
			     uint32_t blocksize, void *input, uint32_t data_len)
{
	int res = 0;
	uint32_t data_block = data_len;
	uint32_t tmp_len;
	uint8_t hash[64];
	uint8_t key[MAX_HASH_BLOCK_SIZE];
	uint8_t *tmp_data;
	rk_handle hash_hdl = 0;
	rk_hash_config hash_cfg;
	uint32_t key_len;
	struct timespec start, end;
	uint64_t total_nsec, nsec;
	uint32_t rounds;

	nsec = DURATION * 1000000000;

	if (is_virt)
		test_get_rng(input, data_len);

	memset(hash, 0x00, sizeof(hash));

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = algo;

	if (is_hmac) {
		key_len = blocksize;
		test_get_rng(key, key_len);
		hash_cfg.key     = key;
		hash_cfg.key_len = key_len;
	}

	total_nsec = 0;
	rounds = 0;

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res) {
		if (is_virt)
			printf("virt:\t[%12s]\tN/A\n", test_algo_name(algo));
		else
			printf("dma_fd:\t[%12s]\tN/A\n", test_algo_name(algo));
		return 0;
	}

	while (total_nsec < nsec) {
		clock_gettime(CLOCK_REALTIME, &start);

		data_block = data_len;

		if (is_virt) {
			tmp_len    = data_len;
			tmp_data   = input;

			while (tmp_len) {
				data_block = tmp_len > data_block ? data_block : tmp_len;

				res = rk_hash_update_virt(hash_hdl, tmp_data, data_block);
				if (res) {
					rk_hash_final(hash_hdl, NULL);
					printf("rk_hash_update_virt[%lu/%u] error = %d\n",
					       (unsigned long)(tmp_data - (uint8_t *)input), tmp_len, res);
					goto error;
				}

				tmp_len -= data_block;
				tmp_data += data_block;
			}
		} else {
			res = rk_hash_update(hash_hdl, ((rk_crypto_mem *)input)->dma_fd, data_block);
			if (res) {
				rk_hash_final(hash_hdl, NULL);
				printf("rk_hash_update error = %d\n", res);
				goto error;
			}
		}

		clock_gettime(CLOCK_REALTIME, &end);
		total_nsec += (end.tv_sec - start.tv_sec) * 1000000000 +
			      (end.tv_nsec - start.tv_nsec);
		rounds ++;
	}

	res = rk_hash_final(hash_hdl, hash);
	if (res) {
		printf("rk_hash_final error = %d\n", res);
		return -1;
	}

	if (is_virt)
		printf("virt:\t[%12s]\t%dMB/s.\n",
		       test_algo_name(algo), (data_len * rounds / (1024 * 1024)));
	else
		printf("dma_fd:\t[%12s]\t%dMB/s.\n",
		       test_algo_name(algo), (data_len * rounds / (1024 * 1024)));

	return res;
error:
	return res;
}

static int test_hash_tp(int throughput_mode)
{
	int res;
	uint32_t buffer_len = TEST_BLOCK_SIZE;;
	rk_crypto_mem *input_fd = NULL;
	uint8_t *input_virt = NULL;
	uint32_t i;
	size_t page_size = getpagesize();

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

	if (rk_crypto_init()) {
		printf("rk_crypto_init error!\n");
		return -1;
	}

	input_fd = rk_crypto_mem_alloc(buffer_len);
	if (!input_fd) {
		printf("rk_crypto_mem_alloc %uByte error!\n", buffer_len);
		res = -1;
		goto out;
	}

	if (posix_memalign((void *)&input_virt, page_size, TEST_BLOCK_SIZE) || !input_virt) {
		printf("malloc %uByte error!\n", TEST_BLOCK_SIZE);
		res = -1;
		goto out;
	}

	/* Test virt hash */
	for (i = 0; i < ARRAY_SIZE(test_hash_tbl); i++) {
		if (throughput_mode != T_VIRT && throughput_mode != T_ALL)
			break;

		res = test_hash_item_tp(true, false, test_hash_tbl[i].algo,
					test_hash_tbl[i].blocksize, input_virt, buffer_len);
		if (res) {
			printf("virt:\ttest hash throughput FAILED!!!\n");
			goto out;
		}
	}

	if (throughput_mode == T_VIRT || throughput_mode == T_ALL)
		printf("virt:\ttest hash throughput SUCCESS.\n\n");

	/* Test dma_fd hash */
	for (i = 0; i < ARRAY_SIZE(test_hash_tbl); i++) {
		if (throughput_mode != T_DMA_FD && throughput_mode != T_ALL)
			break;

		res = test_hash_item_tp(false, false, test_hash_tbl[i].algo,
					test_hash_tbl[i].blocksize, input_fd, buffer_len);
		if (res) {
			printf("dma_fd:\ttest hash throughput FAILED!!!\n");
			goto out;
		}
	}

	if (throughput_mode == T_DMA_FD || throughput_mode == T_ALL)
		printf("dma_fd:\ttest hash throughput SUCCESS.\n\n");

	/* Test virt hmac */
	for (i = 0; i < ARRAY_SIZE(test_hmac_tbl); i++) {
		if (throughput_mode != T_VIRT && throughput_mode != T_ALL)
			break;

		res = test_hash_item_tp(true, true, test_hmac_tbl[i].algo,
					test_hmac_tbl[i].blocksize, input_virt, buffer_len);
		if (res) {
			printf("virt:\ttest hmac throughput FAILED!!!\n");
			goto out;
		}
	}

	if (throughput_mode == T_VIRT || throughput_mode == T_ALL)
		printf("virt:\ttest hmac throughput SUCCESS.\n\n");

	/* Test dma_fd hmac */
	for (i = 0; i < ARRAY_SIZE(test_hmac_tbl); i++) {
		if (throughput_mode != T_DMA_FD && throughput_mode != T_ALL)
			break;

		res = test_hash_item_tp(false, true, test_hmac_tbl[i].algo,
					test_hmac_tbl[i].blocksize, input_fd, buffer_len);
		if (res) {
			printf("dma_fd:\ttest hmac throughput FAILED!!!\n");
			goto out;
		}
	}

	if (throughput_mode == T_DMA_FD || throughput_mode == T_ALL)
		printf("dma_fd:\ttest hmac throughput SUCCESS.\n\n");

out:
	if (input_fd)
		rk_crypto_mem_free(input_fd);

	if (input_virt)
		free(input_virt);

	rk_crypto_deinit();

	return 0;
}

static int test_rsa_item_tp(uint32_t nbits)
{
	uint8_t *data_plain = NULL, *data_enc = NULL, *data_dec = NULL;
	uint32_t nbytes = nbits / 8;
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;
	struct timespec t1, t2, t3;
	uint64_t priv_cost, pub_cost;
	uint32_t out_len = 0;
	RK_RES res;

	data_plain = malloc(nbytes);
	if (!data_plain) {
		printf("malloc data_plain %uByte error!\n", nbytes);
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	data_enc = malloc(nbytes);
	if (!data_enc) {
		printf("malloc data_enc %uByte error!\n", nbytes);
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	data_dec = malloc(nbytes);
	if (!data_dec) {
		printf("malloc data_dec %uByte error!\n", nbytes);
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	res = rk_get_random(data_plain, nbytes);
	if (res) {
		printf("failed to get random data.");
		goto exit;
	}

	/* make sure data_plain is less than n */
	data_plain[0] = 0x00;

	memset(data_enc, 0x00, nbytes);

	test_init_pubkey(&pub_key, nbits);
	test_init_privkey(&priv_key, nbits);

	clock_gettime(CLOCK_REALTIME, &t1);

	res = rk_rsa_priv_encrypt(&priv_key, RK_RSA_CRYPT_PADDING_NONE,
				  data_plain, nbytes, data_enc, &out_len);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED)
			printf("rk_rsa_priv_encrypt failed %x\n", res);
		goto exit;
	}

	clock_gettime(CLOCK_REALTIME, &t2);

	res = rk_rsa_pub_decrypt(&pub_key, RK_RSA_CRYPT_PADDING_NONE,
				 data_enc, out_len, data_dec, &out_len);
	if (res) {
		printf("rk_rsa_pub_decrypt failed %x\n", res);
		goto exit;
	}

	clock_gettime(CLOCK_REALTIME, &t3);

	if (nbytes != out_len || memcmp(data_dec, data_plain, nbytes)) {
		printf("rk_rsa_pub_decrypt compare failed\n");
		test_dump_hex("result", data_dec, out_len);
		test_dump_hex("expect", data_plain, nbytes);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	priv_cost = (t2.tv_sec - t1.tv_sec) * 1000000000 +
		    (t2.tv_nsec - t1.tv_nsec);

	pub_cost = (t3.tv_sec - t2.tv_sec) * 1000000000 +
		   (t3.tv_nsec - t2.tv_nsec);


	printf("virt:\t[RSA-%u]\tPRIV\tENCRYPT\t%lums.\n",
	       nbits, (unsigned long)(priv_cost / 1000000));

	printf("virt:\t[RSA-%u]\tPUB\tDECRYPT\t%lums.\n",
	       nbits, (unsigned long)(pub_cost / 1000000));

exit:
	if (data_plain)
		free(data_plain);

	if (data_enc)
		free(data_enc);

	if (data_dec)
		free(data_dec);

	return (res == RK_CRYPTO_SUCCESS) ? 0 : -1;
}

static int test_rsa_tp(void)
{
	int res;
	uint32_t i;
	uint32_t rsa_bits_tbl[] = {
		RSA_BITS_1024,
		RSA_BITS_2048,
		RSA_BITS_3072,
		RSA_BITS_4096,
	};

	if (rk_crypto_init()) {
		printf("rk_crypto_init error!\n");
		return -1;
	}

	for (i = 0; i < ARRAY_SIZE(rsa_bits_tbl); i++) {
		res = test_rsa_item_tp(rsa_bits_tbl[i]);
		if (res) {
			printf("test rsa-%u throughput FAIL.\n\n", rsa_bits_tbl[i]);
			goto out;
		}
	}

	printf("test rsa throughput SUCCESS.\n\n");

out:
	rk_crypto_deinit();

	return 0;
}

RK_RES test_throughput(int throughput_mode)
{
	if (throughput_mode == T_OTP || throughput_mode == T_ALL) {
		if (test_otp_key_tp())
			printf("Test otp key throughput FAILED.\n\n");
	}

	if (test_cipher_tp(throughput_mode))
		printf("Test cipher throughput FAILED.\n\n");

	if (test_ae_tp(throughput_mode))
		printf("Test ae throughput FAILED.\n\n");

	if (test_hash_tp(throughput_mode))
		printf("Test hash throughput FAILED.\n\n");

	if (test_rsa_tp())
		printf("Test rsa throughput FAILED.\n\n");

	printf("Test throughput SUCCESS.\n");

	return RK_CRYPTO_SUCCESS;
}
