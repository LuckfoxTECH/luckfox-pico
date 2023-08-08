/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include "aes_cbc.h"
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#define TEST_DATA_MAX	(1024 * 1024)

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

static void guide(void)
{
	printf("\n######## Entry one correct parameter. Example: librkcrypto_demo -a ########\n");
	printf("	[-a]: CIPHER_FD\n");
	printf("	[-b]: CIPHER_VIRT\n");
}

void test_get_rng(uint8_t *trn, uint32_t len)
{
	static int init_flag;

	if (!init_flag) {
		srand(time(NULL));
		init_flag = 1;
	}

	for (uint32_t i = 0; i < len; i++)
		trn[i] = rand() & 0xff;
}

static long getCurrentTimeMsec() {
    long msec = 0;
    char str[20] = {0};
    struct timeval stuCurrentTime;

    gettimeofday(&stuCurrentTime, NULL);
    sprintf(str, "%ld%06ld", stuCurrentTime.tv_sec,
        (stuCurrentTime.tv_usec));
    for (size_t i = 0; i < strlen(str); i++) {
        msec = msec * 10 + (str[i] - '0');
    }

    return msec;
}

RK_RES demo_cipher_fd(void)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	rk_cipher_config config;
	uint32_t key_len = sizeof(key_0);
	uint32_t data_len = sizeof(input);
	uint32_t out_len;
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

	res = rk_cipher_crypt(handle, in->dma_fd, data_len, out->dma_fd, &out_len);
	if (res) {
		printf("rk_cipher_crypt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);

	if (memcmp(out->vaddr, expected_enc, out_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_cipher_crypt(handle, out->dma_fd, out_len, out->dma_fd, &out_len);
	if (res) {
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
	uint8_t *input_value = NULL, *soft_output = NULL, *hard_output = NULL;;
	uint32_t data_len = TEST_DATA_MAX;
	uint32_t out_len;
	rk_handle handle = 0;
    long hard_before_time, soft_before_time;
    long hard_after_time, soft_after_time;

    input_value = malloc(data_len);
	if (!input_value) {
		printf("input_value malloc %uByte error!\n", data_len);
		goto exit;
	}

    soft_output = malloc(data_len);
	if (!soft_output) {
		printf("soft_output malloc %uByte error!\n", data_len);
		goto exit;
	}

	hard_output = malloc(data_len);
	if (!hard_output) {
		printf("hard_output malloc %uByte error!\n", data_len);
		goto exit;
	}

    test_get_rng(input_value, data_len);
    memset(soft_output, 0x00, data_len);
	memset(hard_output, 0x00, data_len);

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
	memset(hard_output, 0, sizeof(hard_output));

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

    hard_before_time = getCurrentTimeMsec();
	res = rk_cipher_crypt_virt(handle, input_value, data_len, hard_output, &out_len);
    hard_after_time = getCurrentTimeMsec();
    printf("Hard time-consuming is %ld\n", (hard_after_time - hard_before_time));
	if (res) {
		printf("rk_cipher_crypt_virt error[%x]\n", res);
		goto exit;
	}


    int is_enc = (config.operation == RK_OP_CIPHER_ENC) ? 1 : 0;
    soft_before_time = getCurrentTimeMsec();
    res = rk_aes_cbc_encrypt(input_value, soft_output, data_len, key_0, key_len, iv, is_enc);
    soft_after_time = getCurrentTimeMsec();
    printf("Soft time-consuming is %ld\n", (soft_after_time - soft_before_time));
    if (res) {
		printf("rk_aes_cbc_encrypt_virt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);


    if (memcmp(soft_output, hard_output, out_len)) {
		printf("ENC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER_VIRT ENC success!\n");

	config.operation = RK_OP_CIPHER_DEC;

	res = rk_cipher_init(&config, &handle);
	if (res) {
		printf("rk_cipher_init error! res: 0x%08x\n", res);
		goto exit;
	}

	res = rk_cipher_crypt_virt(handle, hard_output, out_len, hard_output, &out_len);
	if (res) {
		printf("rk_cipher_crypt_virt error[%x]\n", res);
		goto exit;
	}

	rk_cipher_final(handle);

	if (memcmp(hard_output, input_value, data_len)) {
		printf("Hard DEC result not equal to expected value, error!\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

	printf("Test CIPHER_VIRT DEC success!\n");

exit:
    if (input_value)
        free(input_value);

    if (soft_output)
		free(soft_output);

	if (hard_output)
		free(hard_output);

	rk_crypto_deinit();
	return res;
}

int main(int argc, char *argv[])
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t opt = 0;
	const char *opt_string = "ab";

	if (argc < 2)
		guide();

	while ((opt = getopt(argc, argv, opt_string)) != -1) {
		switch (opt) {
		case 'a':
			printf("TEST CIPHER:\n");
			res = demo_cipher_fd();
			break;
		case 'b':
			printf("TEST CIPHER_VIRT:\n");
			res = demo_cipher_virt();
			break;
		case '?':
			printf("error optopt: %c\n", optopt);
			break;
		default:
			break;
		}
	}

	return res;
}


