/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_demo.h"

#define TEST_END_PASS(padding_name) \
	printf("****************** %-20s %-16s test PASS !!! ******************\n", \
	       __func__, padding_name)

#define TEST_END_FAIL(padding_name) \
	printf("****************** %-20s %-16s test FAIL !!! ******************\n", \
	       __func__, padding_name)

#define TEST_END_NA(padding_name) \
	printf("****************** %-20s %-16s test N/A !!! ******************\n", \
	       __func__, padding_name)

static const uint8_t rsa_2048_n[] = {
	0xe9, 0xd7, 0x82, 0x24, 0xf8, 0x69, 0x5e, 0x85,
	0xea, 0x79, 0x84, 0x5d, 0x3d, 0x85, 0xf3, 0x7e,
	0x28, 0x6e, 0xab, 0x2d, 0x76, 0x4f, 0xd4, 0x72,
	0x63, 0xa1, 0x6a, 0x17, 0x60, 0xbd, 0xfc, 0x29,
	0xb3, 0x6d, 0x1b, 0xaf, 0x5c, 0xb2, 0x58, 0x62,
	0x08, 0xb1, 0xed, 0x04, 0xf3, 0x2e, 0x7d, 0x9d,
	0xa6, 0x3b, 0xbd, 0x26, 0x64, 0x62, 0x49, 0x3e,
	0xf9, 0x92, 0x79, 0x25, 0x97, 0xd6, 0x8c, 0xff,
	0x5c, 0x0f, 0xbb, 0x75, 0x22, 0xea, 0xe5, 0x6b,
	0xcb, 0x49, 0x46, 0xcf, 0xe3, 0x8d, 0x6b, 0x69,
	0xd1, 0x32, 0xd0, 0x9d, 0xf4, 0xc9, 0x45, 0x77,
	0xed, 0x61, 0x5a, 0xea, 0x32, 0x2f, 0x8c, 0xf7,
	0x7e, 0xaf, 0xc6, 0xf7, 0x1f, 0xe0, 0x92, 0xdf,
	0x6f, 0x8c, 0xba, 0x82, 0xfc, 0x17, 0x61, 0xec,
	0x1d, 0x36, 0x26, 0x93, 0xa2, 0x8a, 0xe4, 0x45,
	0xd2, 0x84, 0x1f, 0x3e, 0xfe, 0xb4, 0x05, 0xf8,
	0xac, 0x7a, 0x6b, 0x6a, 0x7b, 0x61, 0x9d, 0x30,
	0x72, 0xd5, 0xfe, 0x3b, 0xee, 0x30, 0x65, 0x74,
	0x98, 0xcc, 0x84, 0xb2, 0xaa, 0x42, 0xe0, 0xba,
	0xb7, 0x78, 0x7e, 0x62, 0x9a, 0x84, 0x00, 0x34,
	0x5b, 0x28, 0xf5, 0xe6, 0x6a, 0x23, 0xfe, 0x83,
	0x78, 0xef, 0x84, 0x71, 0x2b, 0x99, 0xa2, 0xd4,
	0xb8, 0x4a, 0x65, 0xd1, 0xa7, 0xf2, 0xfd, 0xdb,
	0x70, 0xbb, 0xe7, 0x5d, 0x70, 0x3c, 0xd4, 0xf6,
	0x68, 0x4b, 0xf1, 0x8f, 0xd4, 0x18, 0xaa, 0x76,
	0x1e, 0x40, 0x4d, 0x6f, 0x11, 0x09, 0x6f, 0x2e,
	0x3e, 0xa1, 0x0b, 0xc8, 0xb6, 0x16, 0x1a, 0xa3,
	0x4b, 0x81, 0x77, 0x68, 0xee, 0x70, 0x26, 0xf9,
	0xfd, 0x0f, 0x1c, 0x39, 0xa8, 0x07, 0xf3, 0xe3,
	0xd7, 0x6b, 0x20, 0x2e, 0x24, 0xa5, 0xba, 0x67,
	0xc0, 0x3f, 0x9a, 0x73, 0x74, 0x2c, 0x92, 0x39,
	0xfe, 0x75, 0xce, 0x3d, 0x39, 0x4c, 0x7f, 0x11,
};

static const uint8_t rsa_2048_e[] = {
	0x01, 0x00, 0x01,
};

static const uint8_t rsa_2048_d[] = {
	0x8e, 0xef, 0x1e, 0xea, 0xf0, 0x38, 0xa0, 0x01,
	0x65, 0x38, 0x4f, 0xd3, 0x1a, 0x77, 0x15, 0x45,
	0xc8, 0xfb, 0x14, 0x80, 0xde, 0x6f, 0x9f, 0x61,
	0x28, 0x4b, 0x12, 0xe9, 0x25, 0x46, 0x1b, 0xab,
	0x75, 0xb4, 0x3c, 0x96, 0xed, 0x27, 0x4a, 0x64,
	0xea, 0x01, 0xaf, 0xea, 0x35, 0xa6, 0xf6, 0x3b,
	0x75, 0xcf, 0xd0, 0x27, 0x76, 0x76, 0xa9, 0x64,
	0xe7, 0xf2, 0x43, 0x40, 0xda, 0x55, 0xd7, 0xab,
	0x83, 0x1f, 0x4e, 0xab, 0x23, 0xe3, 0xc6, 0xc6,
	0x23, 0xe5, 0x2f, 0x76, 0x85, 0x15, 0xfd, 0x51,
	0x1a, 0x01, 0x60, 0x89, 0x1c, 0x86, 0x26, 0x83,
	0x4e, 0xc3, 0x12, 0x6a, 0x52, 0xb0, 0xc4, 0x0e,
	0xc8, 0x0e, 0x33, 0xa4, 0xfc, 0x84, 0x2d, 0xe2,
	0x8b, 0x71, 0xce, 0xd3, 0x9f, 0xb9, 0xbf, 0x2e,
	0x2f, 0x10, 0xd9, 0x75, 0xa6, 0x86, 0x15, 0x50,
	0x24, 0x84, 0x84, 0x00, 0xe2, 0x79, 0x31, 0xa8,
	0x0a, 0x90, 0xc1, 0x1e, 0x1c, 0x54, 0x4a, 0x51,
	0x74, 0x54, 0x33, 0x91, 0x99, 0x4e, 0x3b, 0xcc,
	0x6f, 0xdd, 0xd6, 0x73, 0x0c, 0x2a, 0x6c, 0xa5,
	0xcd, 0x89, 0xa7, 0x69, 0xde, 0x66, 0xb9, 0x60,
	0xe7, 0x49, 0x17, 0x34, 0x1d, 0xbc, 0x32, 0x92,
	0xfb, 0x6f, 0x89, 0x45, 0xee, 0x12, 0x7c, 0x54,
	0x0b, 0xdf, 0x0e, 0xb4, 0xdb, 0x55, 0xbf, 0x3f,
	0x11, 0x11, 0xf4, 0xd1, 0x3e, 0xcf, 0xfa, 0xf5,
	0xe0, 0x2f, 0x72, 0xe4, 0x73, 0x62, 0xe8, 0x19,
	0xbe, 0xeb, 0x8d, 0xe0, 0x77, 0x07, 0x54, 0x91,
	0x64, 0x63, 0x1a, 0x46, 0x47, 0x28, 0x10, 0xda,
	0x4c, 0xd3, 0xe7, 0x4f, 0x0c, 0xd5, 0x4b, 0xeb,
	0x7f, 0x27, 0x0c, 0xf6, 0x25, 0x09, 0x6d, 0xbc,
	0x34, 0xfb, 0x91, 0xab, 0x59, 0x49, 0xb4, 0x1c,
	0x1a, 0xbb, 0x85, 0xae, 0x13, 0x61, 0x5c, 0xa2,
	0xc5, 0x53, 0xaf, 0x29, 0x15, 0xf3, 0x35, 0x51,
};

static void test_init_pubkey(rk_rsa_pub_key_pack *pub)
{
	memset(pub, 0x00, sizeof(*pub));

	pub->key_type  = RK_RSA_KEY_TYPE_PLAIN;

	pub->key.n     = rsa_2048_n;
	pub->key.n_len = sizeof(rsa_2048_n);
	pub->key.e     = rsa_2048_e;
	pub->key.e_len = sizeof(rsa_2048_e);
}

static void test_init_privkey(rk_rsa_priv_key_pack *priv)
{
	memset(priv, 0x00, sizeof(*priv));

	priv->key_type  = RK_RSA_KEY_TYPE_PLAIN;

	priv->key.n     = rsa_2048_n;
	priv->key.n_len = sizeof(rsa_2048_n);
	priv->key.e     = rsa_2048_e;
	priv->key.e_len = sizeof(rsa_2048_e);
	priv->key.d     = rsa_2048_d;
	priv->key.d_len = sizeof(rsa_2048_d);
}

static RK_RES demo_rsa_pub_enc(void)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t plain[SHA256_HASH_SIZE];
	uint8_t  *enc_buf = NULL;
	uint8_t  *dec_buf = NULL;
	uint32_t nbytes = RSA_BITS_2048 / 8;
	uint32_t out_len, padding = RK_RSA_CRYPT_PADDING_PKCS1_V1_5;
	const char *padding_name  = "PKCS1_V1_5";
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	enc_buf = (uint8_t *)malloc(nbytes);
	if (!enc_buf) {
		printf("malloc for enc_buf failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	dec_buf = (uint8_t *)malloc(nbytes);
	if (!dec_buf) {
		printf("malloc for dec_buf failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memset(plain, 0xab, sizeof(plain));

	test_init_pubkey(&pub_key);
	test_init_privkey(&priv_key);

	res = rk_rsa_pub_encrypt(&pub_key, padding, plain, sizeof(plain), enc_buf, &out_len);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED)
			printf("rk_rsa_pub_encrypt failed %x\n", res);
		goto exit;
	}

	res = rk_rsa_priv_decrypt(&priv_key, padding, enc_buf, out_len, dec_buf, &out_len);
	if (res) {
		printf("rk_rsa_priv_decrypt failed %x\n", res);
		goto exit;
	}

	if (sizeof(plain) != out_len || memcmp(dec_buf, plain, out_len)) {
		printf("rk_rsa_priv_decrypt compare failed\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

exit:
	if (enc_buf)
		free(enc_buf);

	if (dec_buf)
		free(dec_buf);

	if (res == RK_CRYPTO_ERR_NOT_SUPPORTED)
		TEST_END_NA(padding_name);
	else if (res)
		TEST_END_FAIL(padding_name);
	else
		TEST_END_PASS(padding_name);

	rk_crypto_deinit();

	return res == RK_CRYPTO_ERR_NOT_SUPPORTED ? RK_CRYPTO_SUCCESS : res;
}


RK_RES demo_rsa_priv_enc(void)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t *enc_buf = NULL;
	uint8_t *dec_buf = NULL;
	uint8_t plain[SHA256_HASH_SIZE];
	uint32_t out_len, padding = RK_RSA_CRYPT_PADDING_PKCS1_V1_5;
	const char *padding_name  = "PKCS1_V1_5";
	uint32_t nbytes = RSA_BITS_2048 / 8;
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	enc_buf = (uint8_t *)malloc(nbytes);
	if (!enc_buf) {
		printf("malloc for enc_buf failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	dec_buf = (uint8_t *)malloc(nbytes);
	if (!dec_buf) {
		printf("malloc for dec_buf failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memset(plain, 0xab, sizeof(plain));

	test_init_pubkey(&pub_key);
	test_init_privkey(&priv_key);

	res = rk_rsa_priv_encrypt(&priv_key, padding, plain, sizeof(plain), enc_buf, &out_len);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED)
			printf("rk_rsa_priv_encrypt failed %x\n", res);
		goto exit;
	}

	res = rk_rsa_pub_decrypt(&pub_key, padding, enc_buf, out_len, dec_buf, &out_len);
	if (res) {
		printf("rk_rsa_pub_decrypt failed %x\n", res);
		goto exit;
	}

	if (sizeof(plain) != out_len || memcmp(dec_buf, plain, out_len)) {
		printf("rk_rsa_pub_decrypt compare failed\n");
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

exit:
	if (enc_buf)
		free(enc_buf);

	if (dec_buf)
		free(dec_buf);

	if (res == RK_CRYPTO_ERR_NOT_SUPPORTED)
		TEST_END_NA(padding_name);
	else if (res)
		TEST_END_FAIL(padding_name);
	else
		TEST_END_PASS(padding_name);

	rk_crypto_deinit();

	return res == RK_CRYPTO_ERR_NOT_SUPPORTED ? RK_CRYPTO_SUCCESS : res;
}

static RK_RES demo_rsa_sign_data(void)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t *sign = NULL;
	uint8_t data[512];
	uint32_t sign_len;
	uint32_t nbytes = RSA_BITS_2048 / 8;
	uint32_t padding = RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256;
	const char *padding_name  = "PKCS1_V1_5_SHA256";
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	sign = (uint8_t *)malloc(nbytes);
	if (!sign) {
		printf("malloc for sign failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	test_init_pubkey(&pub_key);
	test_init_privkey(&priv_key);

	memset(data, 0xab, sizeof(data));

	res = rk_rsa_sign(&priv_key, padding, data, sizeof(data), NULL, sign, &sign_len);
	if (res) {
		printf("rk_rsa_sign data failed %x\n", res);
		goto exit;
	}

	res = rk_rsa_verify(&pub_key, padding, data, sizeof(data), NULL, sign, sign_len);
	if (res) {
		printf("rk_rsa_verify data failed %x\n", res);
		goto exit;
	}

	printf("****************** %-20s %-16s test PASS !!! ******************\n",
	       __func__, padding_name);

exit:
	if (sign)
		free(sign);

	if (res)
		printf("****************** %-20s %-16s test FAIL !!! ******************\n",
		       __func__, padding_name);

	rk_crypto_deinit();

	return res;
}

static RK_RES demo_rsa_sign_digest(void)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t *sign = NULL;
	uint8_t hash[SHA256_HASH_SIZE];
	uint32_t sign_len;
	uint32_t nbytes = RSA_BITS_2048 / 8;
	uint32_t padding = RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256;
	const char *padding_name  = "PKCS1_V1_5_SHA256";
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	sign = (uint8_t *)malloc(nbytes);
	if (!sign) {
		printf("malloc for sign failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	test_init_pubkey(&pub_key);
	test_init_privkey(&priv_key);

	memset(hash, 0xab, sizeof(hash));

	res = rk_rsa_sign(&priv_key, padding, NULL, 0, hash, sign, &sign_len);
	if (res) {
		printf("rk_rsa_sign digest failed %x\n", res);
		goto exit;
	}

	res = rk_rsa_verify(&pub_key, padding, NULL, 0, hash, sign, sign_len);
	if (res) {
		printf("rk_rsa_verify digest failed %x\n", res);
		goto exit;
	}

	printf("****************** %-20s %-16s test PASS !!! ******************\n",
	       __func__, padding_name);

exit:
	if (sign)
		free(sign);

	if (res)
		printf("****************** %-20s %-16s test FAIL !!! ******************\n",
		       __func__, padding_name);

	rk_crypto_deinit();

	return res;
}

RK_RES demo_rsa(void)
{
	RK_RES res;

	res = demo_rsa_pub_enc();
	if (res) {
		printf("demo_rsa_pub_enc demo failed.\n");
		return res;
	}

	res = demo_rsa_priv_enc();
	if (res) {
		printf("demo_rsa_priv_enc demo failed.\n");
		return res;
	}

	res = demo_rsa_sign_data();
	if (res) {
		printf("demo_rsa_sign_data demo failed.\n");
		return res;
	}

	res = demo_rsa_sign_digest();
	if (res) {
		printf("demo_rsa_sign_data demo failed.\n");
		return res;
	}

	return RK_CRYPTO_SUCCESS;
}

