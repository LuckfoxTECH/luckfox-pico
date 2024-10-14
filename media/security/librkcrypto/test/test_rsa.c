/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rkcrypto_core.h"
#include "rkcrypto_trace.h"
#include "cmode_adapter.h"
#include "test_rsa.h"
#include "test_utils.h"
#include "rsa_key_data.h"

#define TEST_END_PASS(nbits, padding_name)  do { \
		if (verbose) \
			printf("******** %-20s %-u\t%-16s test PASS !!! ********\n", \
			       __func__, nbits, padding_name); \
	} while (0)

#define TEST_END_FAIL(nbits, padding_name)  do { \
		if (verbose) \
			printf("******** %-20s %-u\t%-16s test FAIL !!! ********\n", \
			       __func__, nbits, padding_name); \
	} while (0)

#define TEST_END_NA(nbits, padding_name)  do { \
		if (verbose) \
			printf("******** %-20s %-u\t%-16s test N/A  !!! ********\n", \
			       __func__, nbits, padding_name); \
	} while (0)

typedef RK_RES (*test_rsa_one)(uint32_t padding, const char *padding_name,
			       uint32_t nbits, uint32_t data_len, int verbose);

struct test_rsa_item {
	test_rsa_one	do_test;
	uint32_t	padding;
	const char	*padding_name;
	uint32_t	data_size;
};

static RK_RES test_rsa_pub_enc(uint32_t padding, const char *padding_name,
			       uint32_t nbits, uint32_t data_len, int verbose);
static RK_RES test_rsa_priv_enc(uint32_t padding, const char *padding_name,
				uint32_t nbits, uint32_t data_len, int verbose);
static RK_RES test_rsa_sign(uint32_t padding, const char *padding_name,
			    uint32_t nbits, uint32_t data_len, int verbose);

#define TEST_RSA_CRYPT(test, name, size)	{test, RK_RSA_CRYPT_PADDING_##name, #name, size}
#define TEST_RSA_SIGN(test, name, size)		{test, RK_RSA_SIGN_PADDING_##name, #name, size}
#define TEST_RSA_EMPTY()			{NULL, 0, NULL, 0}

static const struct test_rsa_item test_rsa_tbl[] = {
	TEST_RSA_CRYPT(test_rsa_pub_enc,  NONE,             0),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  BLOCK_TYPE_0,     SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  BLOCK_TYPE_1,     SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  BLOCK_TYPE_2,     SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  PKCS1_V1_5,       SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  OAEP_SHA1,        SHA1_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  OAEP_SHA256,      SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  OAEP_SHA384,      SHA384_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_pub_enc,  OAEP_SHA512,      SHA512_HASH_SIZE),
	TEST_RSA_EMPTY(),

	TEST_RSA_CRYPT(test_rsa_priv_enc, NONE,             0),
	TEST_RSA_CRYPT(test_rsa_priv_enc, BLOCK_TYPE_0,     SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, BLOCK_TYPE_1,     SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, BLOCK_TYPE_2,     SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, PKCS1_V1_5,	    SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, OAEP_SHA1,        SHA1_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, OAEP_SHA224,      SHA224_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, OAEP_SHA256,      SHA256_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, OAEP_SHA384,      SHA384_HASH_SIZE),
	TEST_RSA_CRYPT(test_rsa_priv_enc, OAEP_SHA512,      SHA512_HASH_SIZE),
	TEST_RSA_EMPTY(),

	TEST_RSA_SIGN(test_rsa_sign,      PKCS1_V15_SHA1,   512),
	TEST_RSA_SIGN(test_rsa_sign,      PKCS1_V15_SHA224, 512),
	TEST_RSA_SIGN(test_rsa_sign,      PKCS1_V15_SHA256, 512),
	TEST_RSA_SIGN(test_rsa_sign,      PKCS1_V15_SHA384, 512),
	TEST_RSA_SIGN(test_rsa_sign,      PKCS1_V15_SHA512, 512),
	TEST_RSA_SIGN(test_rsa_sign,	  PKCS1_PSS_SHA1,   512),
	TEST_RSA_SIGN(test_rsa_sign,	  PKCS1_PSS_SHA224, 512),
	TEST_RSA_SIGN(test_rsa_sign,	  PKCS1_PSS_SHA256, 512),
	TEST_RSA_SIGN(test_rsa_sign,	  PKCS1_PSS_SHA384, 512),
	TEST_RSA_SIGN(test_rsa_sign,	  PKCS1_PSS_SHA512, 512),
};

#ifdef RSA_OPENSSL_COMPRAE
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/ec_key.h>
#include <openssl/base.h>
#include <openssl/sha.h>
#include <openssl/err.h>

static RK_RES rk2ssl_padding(uint32_t rk_padding, int *ssl_padding, const EVP_MD **digest_md)
{
	switch (rk_padding) {
	/* crypt padding */
	case RK_RSA_CRYPT_PADDING_OAEP_SHA1:
		*ssl_padding = RSA_PKCS1_OAEP_PADDING;
		*digest_md   = EVP_sha1();
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA224:
		*ssl_padding = RSA_PKCS1_OAEP_PADDING;
		*digest_md   = EVP_sha224();
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA256:
		*ssl_padding = RSA_PKCS1_OAEP_PADDING;
		*digest_md   = EVP_sha256();
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA384:
		*ssl_padding = RSA_PKCS1_OAEP_PADDING;
		*digest_md   = EVP_sha384();
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA512:
		*ssl_padding = RSA_PKCS1_OAEP_PADDING;
		*digest_md   = EVP_sha512();
		break;
	case RK_RSA_CRYPT_PADDING_PKCS1_V1_5:
		*ssl_padding = RSA_PKCS1_PADDING;
		*digest_md   = NULL;
		break;

	/* sign padding */
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA1:
		*ssl_padding = RSA_PKCS1_PADDING;
		*digest_md   = EVP_sha1();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA224:
		*ssl_padding = RSA_PKCS1_PADDING;
		*digest_md   = EVP_sha224();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256:
		*ssl_padding = RSA_PKCS1_PADDING;
		*digest_md   = EVP_sha256();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA384:
		*ssl_padding = RSA_PKCS1_PADDING;
		*digest_md   = EVP_sha384();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA512:
		*ssl_padding = RSA_PKCS1_PADDING;
		*digest_md   = EVP_sha512();
		break;

	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA1:
		*ssl_padding = RSA_PKCS1_PSS_PADDING;
		*digest_md   = EVP_sha1();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA224:
		*ssl_padding = RSA_PKCS1_PSS_PADDING;
		*digest_md   = EVP_sha224();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA256:
		*ssl_padding = RSA_PKCS1_PSS_PADDING;
		*digest_md   = EVP_sha256();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA384:
		*ssl_padding = RSA_PKCS1_PSS_PADDING;
		*digest_md   = EVP_sha384();
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA512:
		*ssl_padding = RSA_PKCS1_PSS_PADDING;
		*digest_md   = EVP_sha512();
		break;

	default:
		return RK_CRYPTO_ERR_PARAMETER;
	}

	return RK_CRYPTO_SUCCESS;
}

static void openssl_dump_last_error(void)
{
	ERR_load_ERR_strings();
	ERR_load_crypto_strings();
	unsigned long ulErr = ERR_get_error();
	char szErrMsg[512] = {0};
	char *pTmp = NULL;

	pTmp = ERR_error_string(ulErr, szErrMsg);
	printf("%s\n", pTmp);
}

static EVP_PKEY *openssl_alloc_evpkey(rk_rsa_priv_key_pack *priv)
{
	BIGNUM *be = NULL;
	BIGNUM *bn = NULL;
	BIGNUM *bd = NULL;
	RSA *rsa_key = NULL;
	EVP_PKEY *evp_key = NULL;

	be = BN_new();
	if (!be)
		goto error;

	bd = BN_new();
	if (!bd)
		goto error;

	bn = BN_new();
	if (!bn)
		goto error;

	rsa_key = RSA_new();
	if (!rsa_key)
		goto error;

	BN_bin2bn(priv->key.n, priv->key.n_len, bn);
	BN_bin2bn(priv->key.e, priv->key.e_len, be);
	BN_bin2bn(priv->key.d, priv->key.d_len, bd);

	rsa_key->e = be;
	rsa_key->d = bd;
	rsa_key->n = bn;

	evp_key = EVP_PKEY_new();
	if (!evp_key) {
		printf("EVP_PKEY_new failed!\n");
		goto error;
	}

	if (EVP_PKEY_set1_RSA(evp_key, rsa_key) != 1) {
		printf("EVP_PKEY_set1_RSA err\n");
		goto error;
	}

	return evp_key;
error:
	if (be)
		BN_free(be);
	if (bn)
		BN_free(bn);
	if (bd)
		BN_free(bd);

	rsa_key->e = NULL;
	rsa_key->d = NULL;
	rsa_key->n = NULL;

	if (evp_key)
		EVP_PKEY_free(evp_key);

	return NULL;
}

static void openssl_free_evpkey(EVP_PKEY *evp_key)
{
	if (evp_key)
		EVP_PKEY_free(evp_key);
}

static RK_RES openssl_encrypt(const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len,
			      int padding, const EVP_MD *digest_algorithm,
			      rk_rsa_priv_key_pack *priv)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	size_t tmp_len;
	EVP_PKEY *evp_key = NULL;
	EVP_PKEY_CTX *pkey_ctx = NULL;

	evp_key = openssl_alloc_evpkey(priv);
	if (!evp_key)
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;

	pkey_ctx = EVP_PKEY_CTX_new(evp_key, NULL /* engine */);
	if (!pkey_ctx) {
		printf("EVP_PKEY_CTX_new err\n");
		return RK_CRYPTO_ERR_GENERIC;
	}

	if (EVP_PKEY_encrypt_init(pkey_ctx) <= 0) {
		printf("EVP_PKEY_encrypt_init err\n");
		goto exit;
	}

	if (EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, padding) <= 0) {
		printf("EVP_PKEY_CTX_set_rsa_padding err\n");
		goto exit;
	}

	if (padding == RSA_PKCS1_OAEP_PADDING) {
		if (!EVP_PKEY_CTX_set_rsa_oaep_md(pkey_ctx, digest_algorithm)) {
			printf("EVP_PKEY_CTX_set_rsa_oaep_md err\n");
			goto exit;
		}

		if (!EVP_PKEY_CTX_set_rsa_mgf1_md(pkey_ctx, digest_algorithm)) {
			printf("EVP_PKEY_CTX_set_rsa_mgf1_md err\n");
			goto exit;
		}
	}

	if (EVP_PKEY_encrypt(pkey_ctx, NULL /* out */, &tmp_len, in, in_len) <= 0) {
		printf("EVP_PKEY_encrypt err\n");
		goto exit;
	}
	if (EVP_PKEY_encrypt(pkey_ctx, out, &tmp_len, in, in_len) <= 0) {
		printf("EVP_PKEY_encrypt err\n");
		goto exit;
	}

	*out_len = (uint32_t)tmp_len;

	res = RK_CRYPTO_SUCCESS;
exit:
	if (res)
		openssl_dump_last_error();

	EVP_PKEY_CTX_free(pkey_ctx);
	openssl_free_evpkey(evp_key);

	return res;
}

static RK_RES openssl_decrypt(const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len,
			      int padding, const EVP_MD *digest_algorithm,
			      rk_rsa_priv_key_pack *priv)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	size_t tmp_len;
	EVP_PKEY *evp_key = NULL;
	EVP_PKEY_CTX *pkey_ctx = NULL;

	evp_key = openssl_alloc_evpkey(priv);
	if (!evp_key) {
		printf("openssl_alloc_evpkey err\n");
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;
	}

	pkey_ctx = EVP_PKEY_CTX_new(evp_key, NULL /* engine */);
	if (!pkey_ctx) {
		printf("EVP_PKEY_CTX_new err\n");
		return RK_CRYPTO_ERR_GENERIC;
	}

	if (EVP_PKEY_decrypt_init(pkey_ctx) <= 0) {
		printf("EVP_PKEY_encrypt_init err\n");
		goto exit;
	}

	if (EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, padding) <= 0) {
		printf("EVP_PKEY_CTX_set_rsa_padding err\n");
		goto exit;
	}

	if (padding == RSA_PKCS1_OAEP_PADDING) {
		if (!EVP_PKEY_CTX_set_rsa_oaep_md(pkey_ctx, digest_algorithm)) {
			printf("EVP_PKEY_CTX_set_rsa_oaep_md err\n");
			goto exit;
		}

		if (!EVP_PKEY_CTX_set_rsa_mgf1_md(pkey_ctx, digest_algorithm)) {
			printf("EVP_PKEY_CTX_set_rsa_mgf1_md err\n");
			goto exit;
		}
	}

	if (EVP_PKEY_decrypt(pkey_ctx, NULL /* out */, &tmp_len, in, in_len) <= 0) {
		printf("EVP_PKEY_decrypt err\n");
		goto exit;
	}
	if (EVP_PKEY_decrypt(pkey_ctx, out, &tmp_len, in, in_len) <= 0) {
		printf("EVP_PKEY_decrypt err\n");
		goto exit;
	}

	*out_len = (uint32_t)tmp_len;

	res = RK_CRYPTO_SUCCESS;
exit:
	if (res)
		openssl_dump_last_error();

	EVP_PKEY_CTX_free(pkey_ctx);
	openssl_free_evpkey(evp_key);

	return res;
}

static RK_RES openssl_sign(const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len,
			   int padding, const EVP_MD *digest_algorithm, rk_rsa_priv_key_pack *priv)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	size_t tmp_len = *out_len;
	EVP_PKEY_CTX *pkey_ctx;
	EVP_PKEY *evp_key = NULL;
	EVP_MD_CTX *digest_ctx = NULL;

	digest_ctx = EVP_MD_CTX_new();
	if (!digest_ctx) {
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	evp_key = openssl_alloc_evpkey(priv);
	if (!evp_key) {
		printf("openssl_alloc_evpkey err\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	if (EVP_DigestSignInit(digest_ctx, &pkey_ctx, digest_algorithm,
			       NULL /* engine */, evp_key) != 1) {
		printf("EVP_DigestSignInit err\n");
		goto exit;
	}

	if (EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, padding) <= 0) {
		printf("EVP_PKEY_CTX_set_rsa_padding err\n");
		goto exit;
	}

	if (padding == RSA_PKCS1_PSS_PADDING) {
		uint32_t saltlen = EVP_MD_size(digest_algorithm);

		if (EVP_PKEY_CTX_set_rsa_pss_saltlen(pkey_ctx, saltlen) <= 0) {
			printf("EVP_PKEY_CTX_set_rsa_pss_saltlen err\n");
			goto exit;
		}
	}

	if (EVP_DigestSignUpdate(digest_ctx, in, in_len) != 1) {
		printf("EVP_DigestSignUpdate err\n");
		goto exit;
	}
	if (EVP_DigestSignFinal(digest_ctx, NULL, &tmp_len) != 1) {
		printf("EVP_DigestSignFinal err\n");
		goto exit;
	}
	if (EVP_DigestSignFinal(digest_ctx, out, &tmp_len) <= 0) {
		printf("EVP_DigestSignFinal err\n");
		goto exit;
	}

	*out_len = (uint32_t)tmp_len;
	res = RK_CRYPTO_SUCCESS;
exit:
	if (res)
		openssl_dump_last_error();

	if (digest_ctx)
		EVP_MD_CTX_free(digest_ctx);

	openssl_free_evpkey(evp_key);

	return res;

}

static RK_RES openssl_verify(const uint8_t *in, uint32_t in_len, uint8_t *sign, uint32_t sign_len,
			     int padding, const EVP_MD *digest_algorithm, rk_rsa_priv_key_pack *priv)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	EVP_PKEY_CTX *pkey_ctx;
	EVP_PKEY *evp_key = NULL;
	EVP_MD_CTX *digest_ctx = NULL;

	digest_ctx = EVP_MD_CTX_new();
	if (!digest_ctx) {
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	evp_key = openssl_alloc_evpkey(priv);
	if (!evp_key) {
		printf("openssl_alloc_evpkey err\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	if (EVP_DigestVerifyInit(digest_ctx, &pkey_ctx, digest_algorithm,
			       NULL /* engine */, evp_key) != 1) {
		printf("EVP_DigestVerifyInit err\n");
		goto exit;
	}

	if (EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, padding) <= 0) {
		printf("EVP_PKEY_CTX_set_rsa_padding err\n");
		goto exit;
	}

	if (padding == RSA_PKCS1_PSS_PADDING) {
		uint32_t saltlen = EVP_MD_size(digest_algorithm);

		if (EVP_PKEY_CTX_set_rsa_pss_saltlen(pkey_ctx, saltlen) <= 0) {
			printf("EVP_PKEY_CTX_set_rsa_pss_saltlen err\n");
			goto exit;
		}
	}

	if (EVP_DigestVerifyUpdate(digest_ctx, in, in_len) != 1) {
		printf("EVP_DigestVerifyUpdate err\n");
		goto exit;
	}

	if (!EVP_DigestVerifyFinal(digest_ctx, sign, sign_len)) {
		printf("EVP_DigestVerifyFinal err\n");
		goto exit;
	}

	res = RK_CRYPTO_SUCCESS;
exit:
	if (res)
		openssl_dump_last_error();

	if (digest_ctx)
		EVP_MD_CTX_free(digest_ctx);

	openssl_free_evpkey(evp_key);

	return res;
}

#endif

static RK_RES get_hash_algo_from_padding(uint32_t padding, uint32_t *hlen, uint32_t *hash_algo)
{
	uint32_t shaalgo = RK_ALGO_SHA1;

	switch (padding) {
	case RK_RSA_CRYPT_PADDING_OAEP_SHA1:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA1:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA1:
		*hlen = SHA1_HASH_SIZE;
		shaalgo = RK_ALGO_SHA1;
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA224:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA224:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA224:
		*hlen = SHA224_HASH_SIZE;
		shaalgo = RK_ALGO_SHA224;
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA256:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA256:
		*hlen = SHA256_HASH_SIZE;
		shaalgo = RK_ALGO_SHA256;
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA384:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA384:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA384:
		*hlen = SHA384_HASH_SIZE;
		shaalgo = RK_ALGO_SHA384;
		break;
	case RK_RSA_CRYPT_PADDING_OAEP_SHA512:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA512:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA512:
		*hlen = SHA512_HASH_SIZE;
		shaalgo = RK_ALGO_SHA512;
		break;
	default:
		D_TRACE("unknown padding %x", padding);
		*hlen = 0;
		shaalgo = 0;
		return RK_CRYPTO_ERR_PADDING;
	}

	*hash_algo = shaalgo;

	return RK_CRYPTO_SUCCESS;
}

static RK_RES calc_padding_digest(uint32_t padding, const uint8_t *data, uint32_t data_len,
				  uint8_t *digest)
{
	RK_RES res;
	uint32_t hash_algo, hash_len;
	rk_hash_config hash_cfg;
	rk_handle hash_hdl = 0;

	res = get_hash_algo_from_padding(padding, &hash_len, &hash_algo);
	if (res)
		goto exit;

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));

	hash_cfg.algo = hash_algo;

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res)
		goto exit;

	if (data && data_len != 0) {
		res = rk_hash_update_virt(hash_hdl, data, data_len);
		if (res) {
			rk_hash_final(hash_hdl, NULL);
			goto exit;
		}
	}

	res = rk_hash_final(hash_hdl, digest);
exit:
	if (res)
		D_TRACE("digest error.");

	return res;
}

static RK_RES test_rsa_pub_enc(uint32_t padding, const char *padding_name,
			       uint32_t nbits, uint32_t data_len, int verbose)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t  *data = NULL;
	uint8_t  *enc_buf = NULL;
	uint8_t  *dec_buf = NULL;
	uint32_t out_len, nbytes = nbits / 8;
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	data = (uint8_t *)malloc(data_len);
	if (!data) {
		printf("malloc for data failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memset(data, 0xab, data_len);
	/* make sure data < n */
	data[0] = 0x11;

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

	test_init_pubkey(&pub_key, nbits);
	test_init_privkey(&priv_key, nbits);

	res = rk_rsa_pub_encrypt(&pub_key, padding, data, data_len, enc_buf, &out_len);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED &&
		    res != RK_CRYPTO_ERR_PADDING_OVERFLOW)
			printf("rk_rsa_pub_encrypt failed %x\n", res);
		goto exit;
	}

#ifdef RSA_OPENSSL_COMPRAE
	int ssl_padding;
	const EVP_MD *digest_md;

	if (rk2ssl_padding(padding, &ssl_padding, &digest_md) == RK_CRYPTO_SUCCESS) {
		res = openssl_decrypt(enc_buf, out_len, dec_buf, &out_len,
				      ssl_padding, digest_md, &priv_key);
		if (res) {
			printf("openssl_decrypt error!\n");
			goto exit;
		}

		if (out_len != data_len || memcmp(data, dec_buf, data_len)) {
			printf("rk_rsa_pub_encrypt not match openssl_decrypt\n");
			test_dump_hex("result", dec_buf, out_len);
			test_dump_hex("expect", data, data_len);
			res = RK_CRYPTO_ERR_GENERIC;
			goto exit;
		}

		res = openssl_encrypt(data, data_len, enc_buf, &out_len,
				      ssl_padding, digest_md, &priv_key);
		if (res) {
			printf("openssl_encrypt error!\n");
			goto exit;
		}
	}
#endif
	res = rk_rsa_priv_decrypt(&priv_key, padding, enc_buf, out_len, dec_buf, &out_len);
	if (res) {
		printf("rk_rsa_priv_decrypt failed %x\n", res);
		goto exit;
	}

	if (data_len != out_len || memcmp(dec_buf, data, data_len)) {
		printf("rk_rsa_priv_decrypt compare failed\n");
		test_dump_hex("result", enc_buf, out_len);
		test_dump_hex("expect", data, data_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

exit:
	if (data)
		free(data);

	if (enc_buf)
		free(enc_buf);

	if (dec_buf)
		free(dec_buf);

	if (res == RK_CRYPTO_ERR_NOT_SUPPORTED ||
	    res == RK_CRYPTO_ERR_PADDING_OVERFLOW) {
		TEST_END_NA(nbits, padding_name);
		res = RK_CRYPTO_SUCCESS;
	} else if (res) {
		TEST_END_FAIL(nbits, padding_name);
	} else {
		TEST_END_PASS(nbits, padding_name);
	}

	return res;
}

static RK_RES test_rsa_priv_enc(uint32_t padding, const char *padding_name,
				uint32_t nbits, uint32_t data_len, int verbose)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t  *data = NULL;
	uint8_t  *enc_buf = NULL;
	uint8_t  *dec_buf = NULL;
	uint32_t out_len, nbytes = nbits / 8;
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	data = (uint8_t *)malloc(data_len);
	if (!data) {
		printf("malloc for data failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memset(data, 0xab, data_len);
	/* make sure data < n */
	data[0] = 0x11;

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

	test_init_pubkey(&pub_key, nbits);
	test_init_privkey(&priv_key, nbits);

	res = rk_rsa_priv_encrypt(&priv_key, padding, data, data_len, enc_buf, &out_len);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED &&
		    res != RK_CRYPTO_ERR_PADDING_OVERFLOW)
			printf("rk_rsa_priv_encrypt failed %x\n", res);
		goto exit;
	}

	res = rk_rsa_pub_decrypt(&pub_key, padding, enc_buf, out_len, dec_buf, &out_len);
	if (res) {
		printf("rk_rsa_pub_decrypt failed %x\n", res);
		goto exit;
	}

	if (data_len != out_len || memcmp(dec_buf, data, data_len)) {
		printf("rk_rsa_pub_decrypt compare failed\n");
		test_dump_hex("result", enc_buf, out_len);
		test_dump_hex("expect", data, data_len);
		res = RK_CRYPTO_ERR_GENERIC;
		goto exit;
	}

exit:
	if (data)
		free(data);

	if (enc_buf)
		free(enc_buf);

	if (dec_buf)
		free(dec_buf);

	if (res == RK_CRYPTO_ERR_NOT_SUPPORTED ||
	    res == RK_CRYPTO_ERR_PADDING_OVERFLOW) {
		TEST_END_NA(nbits, padding_name);
		res = RK_CRYPTO_SUCCESS;
	} else if (res) {
		TEST_END_FAIL(nbits, padding_name);
	} else {
		TEST_END_PASS(nbits, padding_name);
	}

	return res;
}

static RK_RES test_rsa_sign_common(uint32_t padding, const char *padding_name,
				   uint32_t nbits, const uint8_t *in, uint32_t in_len,
				   const uint8_t *hash, int verbose)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint8_t  *sign = NULL;
	uint32_t sign_len, nbytes = nbits / 8;
	const char *test_name = hash ? "test_rsa_sign_digest" : "test_rsa_sign_data";
	rk_rsa_pub_key_pack pub_key;
	rk_rsa_priv_key_pack priv_key;

	sign = (uint8_t *)malloc(nbytes);
	if (!sign) {
		printf("malloc for sign failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	test_init_pubkey(&pub_key, nbits);
	test_init_privkey(&priv_key, nbits);

	res = rk_rsa_sign(&priv_key, padding, in, in_len, hash, sign, &sign_len);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED &&
		    res != RK_CRYPTO_ERR_PADDING_OVERFLOW)
			printf("rk_rsa_sign failed %x\n", res);
		else {
			res = RK_CRYPTO_SUCCESS;

			if (verbose) {
				printf("******** %-20s %u\t%-16s test N/A  !!! ********\n",
				       "test_rsa_sign_data", nbits, padding_name);
				printf("******** %-20s %u\t%-16s test N/A  !!! ********\n",
				       "test_rsa_sign_digest", nbits, padding_name);
			}
		}
		goto exit;
	}

#ifdef RSA_OPENSSL_COMPRAE
	int ssl_padding;
	const EVP_MD *digest_md;

	if (rk2ssl_padding(padding, &ssl_padding, &digest_md) == RK_CRYPTO_SUCCESS) {
		res = openssl_verify(in, in_len, sign, sign_len, ssl_padding, digest_md, &priv_key);
		if (res) {
			printf("openssl_verify error!\n");
			goto exit;
		}

		res = openssl_sign(in, in_len, sign, &sign_len,
			ssl_padding, digest_md, &priv_key);
		if (res) {
			printf("openssl_sign error!\n");
			goto exit;
		}
	}
#endif

	res = rk_rsa_verify(&pub_key, padding, in, in_len, hash, sign, sign_len);
	if (res) {
		printf("rk_rsa_verify failed %x\n", res);
		goto exit;
	}

	/* modify sign data to make it wrong */
	*sign += 1;
	res = rk_rsa_verify(&pub_key, padding, in, in_len, hash, sign, sign_len);
	if (res != RK_CRYPTO_ERR_VERIFY) {
		printf("rk_rsa_verify should be RK_CRYPTO_ERR_VERIFY but %x\n", res);
		goto exit;
	}

	res = RK_CRYPTO_SUCCESS;

	if (verbose)
		printf("******** %-20s %u\t%-16s test PASS !!! ********\n",
		       test_name, nbits, padding_name);

exit:
	if (sign)
		free(sign);

	if (res && verbose)
		printf("******** %-20s %u\t%-16s test FAIL !!! ********\n",
		       test_name, nbits, padding_name);

	return res;
}

static RK_RES test_rsa_sign(uint32_t padding, const char *padding_name,
			    uint32_t nbits, uint32_t in_len, int verbose)
{
	RK_RES res;
	uint8_t  *data = NULL;
	uint8_t digest[SHA512_HASH_SIZE];

	data = (uint8_t *)malloc(in_len);
	if (!data) {
		printf("malloc for data failed\n");
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memset(data, 0xab, in_len);
	memset(digest, 0x00, sizeof(digest));

	res = calc_padding_digest(padding, data, in_len, digest);
	if (res) {
		if (res == RK_CRYPTO_ERR_NOT_SUPPORTED) {
			if (verbose) {
				printf("******** %-20s %u\t%-16s test N/A  !!! ********\n",
				       "test_rsa_sign_data", nbits, padding_name);
				printf("******** %-20s %u\t%-16s test N/A  !!! ********\n",
				       "test_rsa_sign_digest", nbits, padding_name);
			}
			res = RK_CRYPTO_SUCCESS;
			goto exit;
		}

		printf("calc_padding_digest %x\n", res);
		goto exit;
	}

	res = test_rsa_sign_common(padding, padding_name, nbits, data, in_len,
				   NULL, verbose);

	if (res) {
		printf("test_rsa_sign data failed %x\n", res);
		goto exit;
	}

	res = test_rsa_sign_common(padding, padding_name, nbits, data, in_len,
				   digest, verbose);

	if (res) {
		printf("test_rsa_sign digest failed %x\n", res);
		goto exit;
	}

exit:
	if (data)
		free(data);

	return res;
}

RK_RES test_rsa(int verbose)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint32_t i, j, data_size;
	uint32_t rsa_key_nbits[] = {
		RSA_BITS_1024,
		RSA_BITS_2048,
		RSA_BITS_3072,
		RSA_BITS_4096,
	};

	res = rk_crypto_init();
	if (res) {
		printf("rk_crypto_init error %08x\n", res);
		return res;
	}

	for (i = 0; i < ARRAY_SIZE(rsa_key_nbits); i++) {
		/* verbose == 0 means alled by the stress test, only test RSA 2048 */
		if (!verbose && rsa_key_nbits[i] != RSA_BITS_2048)
			continue;

		for (j = 0; j < ARRAY_SIZE(test_rsa_tbl); j++) {
			if (!test_rsa_tbl[j].do_test) {
				if (verbose)
					printf("\n");

				continue;
			}

			data_size =  test_rsa_tbl[j].data_size ? : (rsa_key_nbits[i] / 8);
			res = test_rsa_tbl[j].do_test(test_rsa_tbl[j].padding,
						      test_rsa_tbl[j].padding_name,
						      rsa_key_nbits[i],
						      data_size,
						      verbose);
			if (res)
				goto exit;
		}

		if (verbose)
			printf("\n");
	}
exit:
	rk_crypto_deinit();
	return res;
}
