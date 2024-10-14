// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include "rktest_handle.h"
#include "../rk_public_api/rk_crypto_api.h"


TEE_Result handle_crypto_sha(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint8_t in_data[] = {'C', 'h', 'e', 'c', 'k', ' ', 't', 'h', 'e', ' ',
			     's', 'h', 'a', '2', '5', '6', ' ', 't', 'e', 's',
			     't', ' ', 'r', 'e', 's', 'u', 'l', 't', '.'
			    };
	uint8_t out_data[128] = {'\0'};
	uint32_t out_len = sizeof(out_data);
	uint8_t sha_refer[] = {0x7a, 0x3e, 0x37, 0xc2, 0xdc, 0xe7, 0xd3, 0xba,
			       0x41, 0xcd, 0x59, 0xff, 0xea, 0x8d, 0x25, 0xe0,
			       0xdf, 0x91, 0xdd, 0xb1, 0xd1, 0x87, 0xa1, 0x02,
			       0xc5, 0x48, 0x0e, 0x25, 0xbe, 0x7f, 0x6d, 0xd4
			      };
	crypto_ctx_t *ctx = NULL;
	uint32_t hash_algo = TEE_ALG_SHA256;

	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	res = rk_hash_begin(ctx, hash_algo);
	if (res) {
		EMSG("rk_hash_begin FAILED! return value: 0x%x", res);
		goto out;
	}

	res = rk_hash_update(ctx, in_data, sizeof(in_data));
	if (res) {
		EMSG("rk_hash_update FAILED! return value: 0x%x", res);
		goto out;
	}

	res = rk_hash_finish(ctx, NULL, out_data, 0, &out_len);
	if (res) {
		EMSG("rk_hash_finish FAILED! return value: 0x%x", res);
		goto out;
	}
	IMSG("Crypto SHA256 done.");

	//Compare with referance data
	if (TEE_MemCompare(out_data, sha_refer, out_len))
		EMSG("SHA256 compare ERR!");
	else
		IMSG("SHA256 compare OK.");

out:
	if (ctx)
		rk_crypto_free_ctx(&ctx);
	return res;
}

TEE_Result handle_crypto_aes(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint8_t aes_128key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
				0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
			       };
	uint8_t aes_128iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			      };
	uint8_t two_aes_128key[] = {
		//key_1
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		//key_2
		0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
		0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01
	};
	uint8_t in_data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
			    };
	uint8_t enc_out_data[32] = {0};
	uint8_t dec_out_data[32] = {0};
	uint32_t alg;
	uint32_t mod;
	uint32_t temp_out_len = 0;
	uint32_t enc_out_len = 0;
	uint32_t dec_out_len = 0;
	crypto_ctx_t *ctx = NULL;

	//==========================Test AES CBC=============================
	//Take the rk_cipher_crypto api as example.
	alg = TEE_ALG_AES_CBC_NOPAD;
	mod = TEE_MODE_ENCRYPT;
	res = rk_cipher_crypto(in_data, enc_out_data, sizeof(in_data),
			       aes_128key, sizeof(aes_128key), aes_128iv,
			       alg, mod);
	if (res) {
		EMSG("rk_cipher_crypto FAILED! return value: 0x%x", res);
		return res;
	}
	IMSG("AES CBC ENC done.");

	mod = TEE_MODE_DECRYPT;
	res = rk_cipher_crypto(enc_out_data, dec_out_data, sizeof(in_data),
			       aes_128key, sizeof(aes_128key), aes_128iv,
			       alg, mod);
	if (res != TEE_SUCCESS) {
		EMSG("rk_cipher_crypto FAILED! return value: 0x%x", res);
		return res;
	}
	IMSG("AES CBC DEC done.");

	//Compare data
	if (TEE_MemCompare(dec_out_data, in_data, sizeof(in_data)))
		EMSG("AES CBC test ERR!");
	else
		IMSG("AES CBC test OK.");

	//==========================Test AES XTS=============================
	//Take the rk_cipher_begin/updeta/finish apis as example.
	TEE_MemFill(enc_out_data, 0, sizeof(enc_out_data));
	TEE_MemFill(dec_out_data, 0, sizeof(dec_out_data));
	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	alg = TEE_ALG_AES_XTS;
	mod = TEE_MODE_ENCRYPT;
	res = rk_cipher_begin(ctx, two_aes_128key, sizeof(two_aes_128key),
			      aes_128iv, alg, mod);
	if (res) {
		EMSG("rk_cipher_begin FAILED! return value: 0x%x", res);
		goto exit;
	}
	temp_out_len = sizeof(enc_out_data); //Remaining length of the the output buffer.
	res = rk_cipher_update(ctx, in_data, enc_out_data, sizeof(in_data), &temp_out_len);
	if (res) {
		EMSG("rk_cipher_update FAILED! return value: 0x%x", res);
		goto exit;
	}
	enc_out_len += temp_out_len;
	temp_out_len = sizeof(enc_out_data) - enc_out_len; //Remaining length of the the output buffer.
	res = rk_cipher_finish(ctx, enc_out_data + enc_out_len, &temp_out_len);
	if (res) {
		EMSG("rk_cipher_finish FAILED! return value: 0x%x", res);
		goto exit;
	}
	enc_out_len += temp_out_len;
	IMSG("AES XTS ENC done.");

	mod = TEE_MODE_DECRYPT;
	res = rk_cipher_begin(ctx, two_aes_128key, sizeof(two_aes_128key),
			      aes_128iv, alg, mod);
	if (res) {
		EMSG("rk_cipher_begin FAILED! return value: 0x%x", res);
		goto exit;
	}
	temp_out_len = sizeof(dec_out_data);
	res = rk_cipher_update(ctx, enc_out_data, dec_out_data, enc_out_len, &temp_out_len);
	if (res) {
		EMSG("rk_cipher_update FAILED! return value: 0x%x", res);
		goto exit;
	}
	dec_out_len += temp_out_len;
	temp_out_len = sizeof(dec_out_data) - dec_out_len;
	res = rk_cipher_finish(ctx, dec_out_data + dec_out_len, &temp_out_len);
	if (res) {
		EMSG("rk_cipher_finish FAILED! return value: 0x%x", res);
		goto exit;
	}
	dec_out_len += temp_out_len;
	IMSG("AES XTS DEC done.");

	//Compare data
	if (TEE_MemCompare(dec_out_data, in_data, sizeof(in_data)))
		EMSG("AES XTS test ERR!");
	else
		IMSG("AES XTS test OK.");

exit:
	if (ctx)
		rk_crypto_free_ctx(&ctx);

	return res;
}

TEE_Result handle_crypto_rsa(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint8_t in_data[] = "The data for testing RSA enc, dec, sign and verify.";
	uint8_t out_data_1[256] = {0};
	uint8_t out_data_2[256] = {0};
	uint32_t out_len_1 = sizeof(out_data_1);
	uint32_t out_len_2 = sizeof(out_data_2);
	uint8_t hash[32] = {0}; //sha256
	uint32_t hash_len = sizeof(hash);
	uint32_t key_len = 2048 / 8;
	uint32_t algo;
	TEE_OperationMode mode;
	crypto_ctx_t *ctx = NULL;
	rsa_key_t *key = NULL;

	key = TEE_Malloc(sizeof(rsa_key_t), TEE_MALLOC_FILL_ZERO);
	if (!key) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		EMSG("Malloc key memory FAILED!");
		goto out;
	}

	res = rk_gen_rsa_key(key, key_len, 65537);
	if (res) {
		EMSG("rk_gen_rsa_key FAILED! return value: 0x%x", res);
		goto out;
	}
	IMSG("RSA gen key done.");

	//test encrypt
	algo = TEE_ALG_RSAES_PKCS1_V1_5;
	mode = TEE_MODE_ENCRYPT;
	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		res =  TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}
	res = rk_rsa_begin(ctx, key, algo, mode);
	if (res) {
		EMSG("rk_rsa_begin FAILED! return value: 0x%x", res);
		goto out;
	}
	res = rk_rsa_finish(ctx, in_data, out_data_1, sizeof(in_data), &out_len_1, 0);
	if (res) {
		EMSG("rk_rsa_finish FAILED! return value: 0x%x", res);
		goto out;
	}
	rk_crypto_free_ctx(&ctx);

	IMSG("RSA ENC done.");

	//test decrypt
	algo = TEE_ALG_RSAES_PKCS1_V1_5;
	mode = TEE_MODE_DECRYPT;
	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		res =  TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}
	res = rk_rsa_begin(ctx, key, algo, mode);
	if (res) {
		EMSG("rk_rsa_begin FAILED! return value: 0x%x", res);
		goto out;
	}
	res = rk_rsa_finish(ctx, out_data_1, out_data_2, out_len_1, &out_len_2, 0);
	if (res) {
		EMSG("rk_rsa_finish FAILED! return value: 0x%x", res);
		goto out;
	}
	rk_crypto_free_ctx(&ctx);

	IMSG("RSA DEC done.");

	//Compare the encrypt and decrypt data
	if (TEE_MemCompare(in_data, out_data_2, sizeof(in_data)))
		EMSG("RSA ENC and DEC compare ERR!");
	IMSG("RSA ENC and DEC compare OK.");

	//test sign
	//Calculate the hash 256 value of input data.
	res = rk_hash_crypto(in_data, hash, sizeof(in_data), hash_len, TEE_ALG_SHA256);
	if (res != TEE_SUCCESS) {
		EMSG("CryptoSHA ERR: 0x%x.", res);
		goto out;
	}

	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		res =  TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}
	rk_set_sign_mode(ctx, SIGN_DIGEST);
	algo = TEE_ALG_RSASSA_PKCS1_V1_5_SHA256;
	mode = TEE_MODE_SIGN;
	res = rk_rsa_begin(ctx, key, algo, mode);
	if (res) {
		EMSG("rk_rsa_begin FAILED! return value: 0x%x", res);
		goto out;
	}
	res = rk_rsa_finish(ctx, hash, out_data_1, hash_len, &out_len_1, 0);
	if (res) {
		EMSG("rk_rsa_finish FAILED! return value: 0x%x", res);
		goto out;
	}
	rk_crypto_free_ctx(&ctx);

	IMSG("RSA Sign done.");

	//test verify
	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		res =  TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}
	rk_set_sign_mode(ctx, SIGN_DIGEST);
	algo = TEE_ALG_RSASSA_PKCS1_V1_5_SHA256;
	mode = TEE_MODE_VERIFY;
	res = rk_rsa_begin(ctx, key, algo, mode);
	if (res) {
		EMSG("rk_rsa_begin FAILED! return value: 0x%x", res);
		goto out;
	}
	res = rk_rsa_finish(ctx, hash, out_data_1, hash_len, &out_len_1, 0);
	if (res) {
		EMSG("rk_rsa_finish FAILED! return value: 0x%x", res);
		goto out;
	}
	rk_crypto_free_ctx(&ctx);

	IMSG("RSA Verify OK.");

out:
	if (key) {
		TEE_Free(key);
		key = NULL;
	}
	if (ctx)
		rk_crypto_free_ctx(&ctx);

	return res;
}

