/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#include "c_model.h"
#include "cmode_adapter.h"

RK_RES soft_cipher(uint32_t algo, uint32_t mode, uint32_t operation,
		   uint8_t *key, uint32_t key_len, uint8_t *iv,
		   uint8_t *in, uint32_t in_len, uint8_t *out)
{
	int ret;
	int is_enc = (operation == RK_OP_CIPHER_ENC) ? 1 : 0;

	if (algo == RK_ALGO_DES || algo == RK_ALGO_TDES) {
		switch (mode) {
		case RK_CIPHER_MODE_ECB:
			ret = rk_des_ecb_encrypt(in, out, in_len, key, key_len, is_enc);
			break;
		case RK_CIPHER_MODE_CBC:
			ret = rk_des_cbc_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CFB:
			ret = rk_des_cfb_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_OFB:
			ret = rk_des_ofb_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		default:
			return RK_CRYPTO_ERR_PARAMETER;
		}
	} else if (algo == RK_ALGO_AES) {
		switch (mode) {
		case RK_CIPHER_MODE_ECB:
			ret = rk_aes_ecb_encrypt(in, out, in_len, key, key_len, is_enc);
			break;
		case RK_CIPHER_MODE_CBC:
			ret = rk_aes_cbc_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CFB:
			ret = rk_aes_cfb_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_OFB:
			ret = rk_aes_ofb_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CTS:
			ret = rk_aes_cts_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CTR:
			ret = rk_aes_ctr_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_XTS:
			ret = rk_aes_xts_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		default:
			return RK_CRYPTO_ERR_PARAMETER;
		}
	} else if (algo == RK_ALGO_SM4) {
		switch (mode) {
		case RK_CIPHER_MODE_ECB:
			ret = rk_sm4_ecb_encrypt(in, out, in_len, key, key_len, is_enc);
			break;
		case RK_CIPHER_MODE_CBC:
			ret = rk_sm4_cbc_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CFB:
			ret = rk_sm4_cfb_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_OFB:
			ret = rk_sm4_ofb_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CTS:
			ret = rk_sm4_cts_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_CTR:
			ret = rk_sm4_ctr_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		case RK_CIPHER_MODE_XTS:
			ret = rk_sm4_xts_encrypt(in, out, in_len, key, key_len, iv, is_enc);
			break;
		default:
			return RK_CRYPTO_ERR_PARAMETER;
		}

	} else {
		return RK_CRYPTO_ERR_PARAMETER;
	}

	return ret == 0 ? RK_CRYPTO_SUCCESS : RK_CRYPTO_ERR_GENERIC;
}

RK_RES soft_ae(uint32_t algo, uint32_t mode, uint32_t operation,
	       uint8_t *key, uint32_t key_len, uint8_t *iv, uint32_t iv_len,
	       uint8_t *aad, uint32_t aad_len, uint32_t tag_len,
	       uint8_t *in, uint32_t in_len, uint8_t *out, uint8_t *tag)
{
	int ret;
	int is_enc = (operation == RK_OP_CIPHER_ENC) ? 1 : 0;
	struct aes_ae_in aes_in;
	struct aes_ae_out aes_out;
	struct sm4_ae_in sm4_in;
	struct sm4_ae_out sm4_out;

	aes_in.key      = key;
	aes_in.src      = in;
	aes_in.iv       = iv;
	aes_in.aad      = aad;
	aes_in.key_len  = key_len;
	aes_in.src_len  = in_len;
	aes_in.iv_len   = iv_len;
	aes_in.aad_len  = aad_len;
	aes_in.tag_size = tag_len;

	aes_out.dest = out;
	aes_out.tag  = tag;

	memcpy(&sm4_in, &aes_in, sizeof(aes_in));
	memcpy(&sm4_out, &aes_out, sizeof(aes_out));

	if (algo == RK_ALGO_AES) {
		switch (mode) {
		case RK_CIPHER_MODE_GCM:
			ret = rk_aes_gcm_encrypt(&aes_in, &aes_out, is_enc);
			break;
		case RK_CIPHER_MODE_CCM:
			ret = rk_aes_ccm_encrypt(&aes_in, &aes_out, is_enc);
			break;
		default:
			return RK_CRYPTO_ERR_PARAMETER;
		}
	} else if (algo == RK_ALGO_SM4) {
		switch (mode) {
		case RK_CIPHER_MODE_GCM:
			ret = rk_sm4_gcm_encrypt(&sm4_in, &sm4_out, is_enc);
			break;
		case RK_CIPHER_MODE_CCM:
			ret = rk_sm4_ccm_op(&sm4_in, &sm4_out, is_enc);
			break;
		default:
			return RK_CRYPTO_ERR_PARAMETER;
		}

	} else {
		return RK_CRYPTO_ERR_PARAMETER;
	}

	return ret == 0 ? RK_CRYPTO_SUCCESS : RK_CRYPTO_ERR_GENERIC;
}

RK_RES soft_hash(uint32_t algo, const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len)
{
	int ret = -1;

	switch (algo) {
	case RK_ALGO_MD5:
		ret = rk_hash_md5(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA1:
		ret = rk_hash_sha1(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA256:
		ret = rk_hash_sha256(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA224:
		ret = rk_hash_sha224(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA384:
		ret = rk_hash_sha384(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA512:
		ret = rk_hash_sha512(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA512_224:
		ret = rk_hash_sha512_224(in, in_len, out, out_len);
		break;
	case RK_ALGO_SHA512_256:
		ret = rk_hash_sha512_256(in, in_len, out, out_len);
		break;
	case RK_ALGO_SM3:
		ret = rk_hash_sm3(in, in_len, out, out_len);
		break;
	default:
		return RK_CRYPTO_ERR_PARAMETER;
	}

	return ret == 0 ? RK_CRYPTO_SUCCESS : RK_CRYPTO_ERR_GENERIC;
}

RK_RES soft_hmac(uint32_t algo, const uint8_t *key, uint32_t key_len,
		 const uint8_t *in,  uint32_t in_len, uint8_t *out, uint32_t *out_len)
{
	int ret = -1;

	switch (algo) {
	case RK_ALGO_HMAC_MD5:
		ret = rk_hmac_md5(key, key_len, in, in_len, out, out_len);
		break;
	case RK_ALGO_HMAC_SHA1:
		ret = rk_hmac_sha1(key, key_len, in, in_len, out, out_len);
		break;
	case RK_ALGO_HMAC_SHA256:
		ret = rk_hmac_sha256(key, key_len, in, in_len, out, out_len);
		break;
	case RK_ALGO_HMAC_SHA512:
		ret = rk_hmac_sha512(key, key_len, in, in_len, out, out_len);
		break;
	case RK_ALGO_HMAC_SM3:
		ret = rk_hmac_sm3(key, key_len, in, in_len, out, out_len);
		break;
	default:
		return RK_CRYPTO_ERR_PARAMETER;
	}

	return ret == 0 ? RK_CRYPTO_SUCCESS : RK_CRYPTO_ERR_GENERIC;
}

