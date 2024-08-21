/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include "c_model.h"
#include "cmode_adapter.h"
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_otp_key.h"
#include "test_otp_key_crypto.h"
#include "test_utils.h"

uint8_t otp_key0[32] = {
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
};

uint8_t otp_key1[32] = {
	0xdf, 0x20, 0x5a, 0xb3, 0x88, 0x50, 0x9e, 0x4f,
	0x01, 0x21, 0xe7, 0xc9, 0x24, 0x00, 0x1b, 0x84,
	0x2a, 0xfb, 0x83, 0xac, 0xe4, 0x61, 0x4a, 0x94,
	0x1f, 0xf4, 0x84, 0xc3, 0x1f, 0xe5, 0x52, 0xc7,
};

uint8_t otp_key2[32] = {
	0xd5, 0x20, 0xaa, 0xb3, 0x88, 0x5f, 0x9e, 0x41,
	0x05, 0x21, 0x07, 0xc9, 0x44, 0x00, 0x1b, 0x80,
	0x23, 0xfb, 0x13, 0xac, 0xe4, 0x66, 0x4a, 0x93,
	0x13, 0xf4, 0x04, 0xc3, 0x3f, 0xe7, 0x52, 0xc0,
};

uint8_t otp_key3[32] = {
	0x10, 0x44, 0x80, 0xb3, 0x88, 0x5f, 0x02, 0x03,
	0x05, 0x21, 0x07, 0xc9, 0x44, 0x00, 0x1b, 0x80,
	0x5f, 0x9e, 0x41, 0xac, 0xe4, 0x64, 0x43, 0xa3,
	0x13, 0x06, 0x07, 0x08, 0x3f, 0xe7, 0x05, 0x06,
};

#define DATA_BUTT	0xFFFFFFFF
#define TEST_DATA_MAX	(500 * 1024)

static const uint32_t test_key_ids[] = {
	RK_OEM_OTP_KEY0,
	RK_OEM_OTP_KEY1,
	RK_OEM_OTP_KEY2,
	RK_OEM_OTP_KEY3,
};
struct test_otp_key_item {
	uint32_t algo;
	uint32_t modes[RK_CIPHER_MODE_MAX];
	uint32_t key_lens[4];
	uint32_t iv_len;
	uint32_t data_len;
	uint32_t operations[4];
};

static struct test_otp_key_item test_item_tbl[] = {
	{
		.algo    = RK_ALGO_AES,
		.modes   = {
			RK_CIPHER_MODE_ECB,
			RK_CIPHER_MODE_CBC,
			RK_CIPHER_MODE_CTS,
			RK_CIPHER_MODE_CTR,
			RK_CIPHER_MODE_CFB,
			RK_CIPHER_MODE_OFB,
			DATA_BUTT,
		},
		.key_lens   = {16, 24, 32, DATA_BUTT},
		.iv_len     = AES_BLOCK_SIZE,
		.data_len   = TEST_DATA_MAX,
		.operations = {
			RK_OP_CIPHER_ENC,
			RK_OP_CIPHER_DEC,
			DATA_BUTT,
		},
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
			DATA_BUTT,
		},
		.key_lens   = {16, DATA_BUTT},
		.iv_len     = SM4_BLOCK_SIZE,
		.data_len   = TEST_DATA_MAX,
		.operations = {
			RK_OP_CIPHER_ENC,
			RK_OP_CIPHER_DEC,
			DATA_BUTT,
		},
	},
};

void test_set_otp_tag(void)
{
	uint32_t res;

	res = rk_set_oem_hr_otp_read_lock(RK_OEM_OTP_KEY0);
	printf("trusty_set_oem_hr_otp_read_lock 0. res:%d\n", res);

	res = rk_set_oem_hr_otp_read_lock(RK_OEM_OTP_KEY1);
	printf("trusty_set_oem_hr_otp_read_lock 1. res:%d\n", res);

	res = rk_set_oem_hr_otp_read_lock(RK_OEM_OTP_KEY2);
	printf("trusty_set_oem_hr_otp_read_lock 2. res:%d\n", res);

	res = rk_set_oem_hr_otp_read_lock(RK_OEM_OTP_KEY3);
	printf("trusty_set_oem_hr_otp_read_lock 3. res:%d\n", res);

	return;
}

void test_write_otp_key(void)
{
	uint32_t res;

	res = rk_write_oem_otp_key(RK_OEM_OTP_KEY0,
				   otp_key0, sizeof(otp_key0));
	printf("write otp key 0. res:%d\n", res);

	res = rk_write_oem_otp_key(RK_OEM_OTP_KEY1,
				   otp_key1, sizeof(otp_key1));
	printf("write otp key 1. res:%d\n", res);

	res = rk_write_oem_otp_key(RK_OEM_OTP_KEY2,
				   otp_key2, sizeof(otp_key2));
	printf("write otp key 2. res:%d\n", res);

	res = rk_write_oem_otp_key(RK_OEM_OTP_KEY3,
				   otp_key3, sizeof(otp_key3));
	printf("write otp key 3. res:%d\n", res);

	return;
}

static int test_otp_key_item_virt(uint32_t key_id, const struct test_otp_key_item *item)
{
	int res = 0;
	RK_RES rk_res;
	uint32_t i, j, k;
	uint8_t *key = NULL;
	rk_cipher_config cipher_cfg;
	uint8_t *plain = NULL, *cipher_soft = NULL, *cipher_hard = NULL;
	uint32_t algo = 0, mode = 0, key_len, data_len, operation;

	plain = malloc(item->data_len);
	if (!plain) {
		printf("plain malloc %uByte error!\n", item->data_len);
		res = -1;
		goto exit;
	}

	cipher_soft = malloc(item->data_len);
	if (!cipher_soft) {
		printf("cipher_soft malloc %uByte error!\n", item->data_len);
		res = -1;
		goto exit;
	}

	cipher_hard = malloc(item->data_len);
	if (!cipher_hard) {
		printf("cipher_hard malloc %uByte error!\n", item->data_len);
		res = -1;
		goto exit;
	}

	switch (key_id) {
	case RK_OEM_OTP_KEY0:
		key = otp_key0;
		break;
	case RK_OEM_OTP_KEY1:
		key = otp_key1;
		break;
	case RK_OEM_OTP_KEY2:
		key = otp_key2;
		break;
	case RK_OEM_OTP_KEY3:
		key = otp_key3;
		break;
	default:
		return -1;
	}

	memset(&cipher_cfg, 0x00, sizeof(cipher_cfg));
	test_get_rng(cipher_cfg.iv, item->iv_len);

	test_get_rng(plain, item->data_len);

	memset(cipher_soft, 0x00, item->data_len);
	memset(cipher_hard, 0x00, item->data_len);

	for (i = 0; i < ARRAY_SIZE(item->modes); i++) {
		if (item->modes[i] == DATA_BUTT)
			break;

		algo = item->algo;
		mode = item->modes[i];

		for (j = 0; j < ARRAY_SIZE(item->key_lens); j++) {
			if (item->key_lens[j] == DATA_BUTT)
				break;

			key_len  = item->key_lens[j];
			data_len = item->data_len;
			if (is_no_multi_blocksize(mode))
				data_len -= 3;

			for (k = 0; k < ARRAY_SIZE(item->operations); k++) {
				if (item->operations[k] == DATA_BUTT)
					break;

				operation = item->operations[k];

				cipher_cfg.algo      = algo;
				cipher_cfg.mode      = mode;
				cipher_cfg.operation = operation;
				cipher_cfg.key_len   = key_len;
				cipher_cfg.reserved  = NULL;

				rk_res = rk_oem_otp_key_cipher_virt(key_id, &cipher_cfg,
								 plain, cipher_hard, data_len);
				if (rk_res == RK_CRYPTO_ERR_NOT_SUPPORTED) {
					printf("virt:\totpkey%d\t[%s-%u]\t%s\t%s\tN/A\n",
					       key_id, test_algo_name(algo), key_len * 8,
					       test_mode_name(mode), test_op_name(operation));
					res = 0;
					continue;
				} else if (res) {
					printf("rk_oem_otp_key_cipher_virt fail! 0x%08x\n", res);
					goto exit;
				}

				rk_res = soft_cipher(algo, mode, operation, key, key_len,
						  cipher_cfg.iv, plain, data_len, cipher_soft);
				if (res) {
					printf("soft_cipher fail! 0x%08x\n", res);
					res = -1;
					goto exit;
				}

				if (memcmp(cipher_soft, cipher_hard, data_len)) {
					printf("rk_oem_otp_key_cipher_virt compare failed.\n");
					res = -1;
					goto exit;
				}

				printf("virt:\totpkey%d\t[%s-%u]\t%s\t%s\tPASS\n",
				       key_id, test_algo_name(algo), key_len * 8,
				       test_mode_name(mode), test_op_name(operation));
			}
		}
	}

exit:
	if (plain)
		free(plain);

	if (cipher_soft)
		free(cipher_soft);

	if (cipher_hard)
		free(cipher_hard);

	if (res)
		printf("virt:\totpkey%d\t[%s-%u]\t%s\t%s\tFAIL\n",
		       key_id, test_algo_name(algo), key_len * 8,
		       test_mode_name(mode), test_op_name(operation));

	return res;
}

static int test_otp_key_item_fd(uint32_t key_id, const struct test_otp_key_item *item)
{
	int res = 0;
	RK_RES rk_res;
	uint32_t i, j, k;
	uint8_t *key = NULL;
	rk_cipher_config cipher_cfg;
	uint8_t *cipher_soft = NULL;
	rk_crypto_mem *plain = NULL;
	rk_crypto_mem *cipher_hard = NULL;
	uint32_t algo = 0, mode = 0, key_len, data_len, operation;

	cipher_soft = malloc(item->data_len);
	if (!cipher_soft) {
		printf("cipher_soft malloc %uByte error!\n", item->data_len);
		res = -1;
		goto exit;
	}

	plain = rk_crypto_mem_alloc(item->data_len);
	if (!plain) {
		printf("rk_crypto_mem_alloc %uByte error!\n", item->data_len);
		res = -1;
		goto exit;
	}

	cipher_hard = rk_crypto_mem_alloc(item->data_len);
	if (!cipher_hard) {
		printf("rk_crypto_mem_alloc %uByte error!\n", item->data_len);
		res = -1;
		goto exit;
	}

	switch (key_id) {
	case RK_OEM_OTP_KEY0:
		key = otp_key0;
		break;
	case RK_OEM_OTP_KEY1:
		key = otp_key1;
		break;
	case RK_OEM_OTP_KEY2:
		key = otp_key2;
		break;
	case RK_OEM_OTP_KEY3:
		key = otp_key3;
		break;
	default:
		return -1;
	}

	memset(&cipher_cfg, 0x00, sizeof(cipher_cfg));
	test_get_rng(cipher_cfg.iv, item->iv_len);

	test_get_rng(plain->vaddr, item->data_len);

	memset(cipher_soft, 0x00, item->data_len);
	memset(cipher_hard->vaddr, 0x00, item->data_len);

	for (i = 0; i < ARRAY_SIZE(item->modes); i++) {
		if (item->modes[i] == DATA_BUTT)
			break;

		algo = item->algo;
		mode = item->modes[i];

		for (j = 0; j < ARRAY_SIZE(item->key_lens); j++) {
			if (item->key_lens[j] == DATA_BUTT)
				break;

			key_len  = item->key_lens[j];
			data_len = item->data_len;
			if (is_no_multi_blocksize(mode))
				data_len -= 3;

			for (k = 0; k < ARRAY_SIZE(item->operations); k++) {
				if (item->operations[k] == DATA_BUTT)
					break;

				operation = item->operations[k];

				cipher_cfg.algo      = algo;
				cipher_cfg.mode      = mode;
				cipher_cfg.operation = operation;
				cipher_cfg.key_len   = key_len;
				cipher_cfg.reserved  = NULL;

				rk_res = rk_oem_otp_key_cipher(key_id, &cipher_cfg, plain->dma_fd,
							    cipher_hard->dma_fd, data_len);
				if (rk_res == RK_CRYPTO_ERR_NOT_SUPPORTED) {
					printf("dma_fd:\totpkey%d\t[%s-%u]\t%s\t%s\tN/A\n",
					       key_id, test_algo_name(algo), key_len * 8,
					       test_mode_name(mode), test_op_name(operation));
					res = 0;
					continue;
				} else if (res) {
					printf("rk_oem_otp_key_cipher fail! 0x%08x\n", res);
					goto exit;
				}

				rk_res = soft_cipher(algo, mode, operation, key, key_len,
						  cipher_cfg.iv, plain->vaddr, data_len,
						  cipher_soft);
				if (res) {
					printf("soft_cipher fail! 0x%08x\n", res);
					res = -1;
					goto exit;
				}

				if (memcmp(cipher_soft, cipher_hard->vaddr, data_len)) {
					printf("rk_oem_otp_key_cipher compare failed.\n");
					res = -1;
					goto exit;
				}

				printf("dma_fd:\totpkey%d\t[%s-%u]\t%s\t%s\tPASS\n",
				       key_id, test_algo_name(algo), key_len * 8,
				       test_mode_name(mode), test_op_name(operation));
			}
		}
	}

exit:
	if (cipher_soft)
		free(cipher_soft);

	if (plain)
		rk_crypto_mem_free(plain);

	if (cipher_hard)
		rk_crypto_mem_free(cipher_hard);

	if (res)
		printf("dma_fd:\totpkey%d\t[%s-%u]\t%s\t%s\tFAIL\n",
		       key_id, test_algo_name(algo), key_len * 8,
		       test_mode_name(mode), test_op_name(operation));

	return res;
}

static int test_otp_key_fd(void)
{
	int res;
	uint32_t i, j;
	uint32_t key_id;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error!\n");
		return res;
	}


	for (i = 0; i < ARRAY_SIZE(test_key_ids); i++) {
		key_id = test_key_ids[i];

		for (j = 0; j < ARRAY_SIZE(test_item_tbl); j++) {
			res = test_otp_key_item_fd(key_id, &test_item_tbl[j]);
			if (res)
				goto exit;
		}
	}

exit:
	rk_crypto_deinit();
	return res;
}

static int test_otp_key_virt(void)
{
	int res;
	uint32_t i, j;
	uint32_t key_id;

	for (i = 0; i < ARRAY_SIZE(test_key_ids); i++) {
		key_id = test_key_ids[i];

		for (j = 0; j < ARRAY_SIZE(test_item_tbl); j++) {
			res = test_otp_key_item_virt(key_id, &test_item_tbl[j]);
			if (res)
				goto exit;
		}
	}

exit:
	return res;
}

RK_RES test_otp_key(void)
{
	if (test_otp_key_virt())
		return RK_CRYPTO_ERR_GENERIC;

	if (test_otp_key_fd())
		return RK_CRYPTO_ERR_GENERIC;

	return RK_CRYPTO_SUCCESS;
}
