/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_trace.h"
#include "test_multi.h"
#include "test_cipher.h"
#include "test_hash.h"

static RK_RES test_support_multi(void)
{
	printf("=============== Multithreaded testing start =============\n");

	///TODO: test multi thread

	printf("=============== Multithreaded testing pass ==============\n");

	return RK_CRYPTO_SUCCESS;
}

static RK_RES test_nosupport_multi(void)
{
	RK_RES res;

	printf("=============== Non-multithreaded testing start =============\n");

	/* Test normal calculation */
	res = test_cipher(1);
	if (res) {
		E_TRACE("test_cipher error[%x]\n", res);
		goto error;
	}

	res = test_hash(1);
	if (res) {
		E_TRACE("test_hash error[%x]\n", res);
		goto error;
	}

	res = test_hmac(1);
	if (res) {
		E_TRACE("test_hash error[%x]\n", res);
		goto error;
	}

	/* Test abnormal operation */
	rk_cipher_config cipher_cfg;
	rk_hash_config hash_cfg;
	rk_handle hdl_cipher1, hdl_cipher2, hdl_hash1, hdl_hash2;

	memset(&cipher_cfg, 0x00, sizeof(cipher_cfg));

	cipher_cfg.algo      = RK_ALGO_AES;
	cipher_cfg.mode      = RK_CIPHER_MODE_ECB;
	cipher_cfg.operation = RK_OP_CIPHER_ENC;
	cipher_cfg.key_len   = 16;

	res = rk_crypto_init();
	if (res) {
		E_TRACE("rk_crypto_init error[%x]\n", res);
		goto error;
	}

	res = rk_cipher_init(&cipher_cfg, &hdl_cipher1);
	if (res) {
		E_TRACE("rk_cipher_init error[%x]\n", res);
		goto error;
	}

	res = rk_cipher_init(&cipher_cfg, &hdl_cipher2);
	if (res != RK_CRYPTO_ERR_BUSY) {
		E_TRACE("rk_cipher_init should be busy[%x]\n", res);
		goto error;
	}

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));

	hash_cfg.algo = RK_ALGO_HMAC_SHA256;

	res = rk_hash_init(&hash_cfg, &hdl_hash1);
	if (res != RK_CRYPTO_ERR_BUSY) {
		E_TRACE("rk_hash_init should be busy[%x]\n", res);
		goto error;
	}

	rk_cipher_final(hdl_cipher1);

	res = rk_hash_init(&hash_cfg, &hdl_hash2);
	if (res) {
		E_TRACE("rk_cipher_init error[%x]\n", res);
		goto error;
	}

	res = rk_cipher_init(&cipher_cfg, &hdl_cipher2);
	if (res != RK_CRYPTO_ERR_BUSY) {
		E_TRACE("rk_cipher_init should be busy[%x]\n", res);
		goto error;
	}

	rk_hash_final(hdl_hash2, NULL);

	rk_crypto_deinit();

	/* Test normal calculation again */
	res = test_cipher(1);
	if (res) {
		E_TRACE("test_cipher error[%x]\n", res);
		goto error;
	}

	res = test_hash(1);
	if (res) {
		E_TRACE("test_hash error[%x]\n", res);
		goto error;
	}

	res = test_hmac(1);
	if (res) {
		E_TRACE("test_hash error[%x]\n", res);
		goto error;
	}

	printf("=============== Non-multithreaded testing pass =============\n");

	return RK_CRYPTO_SUCCESS;

error:
	printf("=============== Non-multithreaded testing fail =============\n");
	return res;
}

RK_RES test_multi(void)
{
	int cfd;
	RK_RES res = RK_CRYPTO_ERR_GENERIC;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	/* Open the crypto device */
	cfd = open("/dev/crypto", O_RDWR, 0);
	if (cfd < 0) {
		if (errno != EBUSY) {
			printf("An unexpected return value of init[%x]\n", res);
			res = RK_CRYPTO_ERR_GENERIC;
			goto exit;
		}

		res = test_nosupport_multi();
	} else {
		close(cfd);
		res = test_support_multi();
	}

exit:
	return res;
}

