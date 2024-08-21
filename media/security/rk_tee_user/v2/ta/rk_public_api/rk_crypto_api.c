// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include <tee_api_defines_extensions.h>
#include <utee_defines.h>
#include <util.h>
#include "rk_crypto_api.h"

#define CRYPTO_DEBUG		0


static uint32_t long2byte(uint64_t value, uint8_t ch[8])
{
	int i;
	uint32_t len = 0;

	for (i = 7; i >= 0; i--) {
		ch[i] = (value & 0xff00000000000000) >> 56;
		value = value << 8;
		if (ch[i] != 0 && len == 0)
			len = i + 1;
	}

	return len;
}

TEE_Result rk_gen_rsa_key(rsa_key_t *rsa_key, uint32_t key_len,
			  uint64_t public_exponent)
{
	TEE_Result res;
	TEE_ObjectHandle obj;
	TEE_Attribute attr;
	uint8_t e[8] = {0};
	uint32_t e_len = 0;
	uint32_t out_len = 0;

	if (rsa_key == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (key_len != 32 && key_len != 64 && key_len != 96 &&
	    key_len != 128 && key_len != 192 && key_len != 256 &&
	    key_len != 384 && key_len != 512)
		return TEE_ERROR_BAD_PARAMETERS;

	/* support 3 and 65537 for now */
	if (public_exponent != 3 && public_exponent != 65537)
		return TEE_ERROR_BAD_PARAMETERS;

	e_len = long2byte(public_exponent, e);

	TEE_InitRefAttribute(&attr, TEE_ATTR_RSA_PUBLIC_EXPONENT, e, e_len);

	res = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, key_len * 8, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		return res;
	}

	res = TEE_GenerateKey(obj, key_len * 8, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GenerateKey failed with code 0x%x", res);
		goto exit;
	}

	out_len = sizeof(rsa_key->n);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_MODULUS, rsa_key->n,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_n failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->key_len = out_len;

	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_PUBLIC_EXPONENT,
					   rsa_key->e, &e_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_e failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->e_len = e_len;

	out_len = sizeof(rsa_key->d);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_PRIVATE_EXPONENT,
					   rsa_key->d, &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_d failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->d_len = out_len;

	out_len = sizeof(rsa_key->p);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_PRIME1, rsa_key->p,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_p failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->p_len = out_len;

	out_len = sizeof(rsa_key->q);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_PRIME2, rsa_key->q,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_q failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->q_len = out_len;

	out_len = sizeof(rsa_key->dp);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_EXPONENT1, rsa_key->dp,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_dp failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->dp_len = out_len;

	out_len = sizeof(rsa_key->dq);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_EXPONENT2, rsa_key->dq,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_dq failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->dq_len = out_len;

	out_len = sizeof(rsa_key->iq);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_COEFFICIENT, rsa_key->iq,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_iq failed with code 0x%x", res);
		goto exit;
	}
	rsa_key->iq_len = out_len;

exit:
	TEE_FreeTransientObject(obj);

	return res;
}

TEE_Result rk_gen_ec_key(ec_key_t *ec_key, uint32_t key_len /* bit */,
			 uint32_t curve)
{
	TEE_Result res;
	TEE_ObjectHandle obj;
	TEE_Attribute attr;
	uint32_t out_len = 0;
	uint32_t padding_len = 0;
	uint32_t key_size_bytes = 0;
	uint8_t tmp_buf[66] = {0};

#if CRYPTO_DEBUG
	IMSG("key_len = %d; curve = %d", key_len, curve);
#endif

	if (ec_key == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (key_len != 192 && key_len != 224 && key_len != 256 &&
	    key_len != 384 && key_len != 521)
		return TEE_ERROR_BAD_PARAMETERS;

	if (curve != TEE_ECC_CURVE_NIST_P192 &&
	    curve != TEE_ECC_CURVE_NIST_P224 &&
	    curve != TEE_ECC_CURVE_NIST_P256 &&
	    curve != TEE_ECC_CURVE_NIST_P384 &&
	    curve != TEE_ECC_CURVE_NIST_P521)
		return TEE_ERROR_BAD_PARAMETERS;

	ec_key->curve = curve;
	ec_key->key_len = key_len;

	key_size_bytes = (key_len + 7) / 8;

	TEE_InitValueAttribute(&attr, TEE_ATTR_ECC_CURVE, curve, 0);

	res = TEE_AllocateTransientObject(TEE_TYPE_ECDSA_KEYPAIR, key_len, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		return res;
	}

	res = TEE_GenerateKey(obj, key_len, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GenerateKey failed with code 0x%x", res);
		goto exit;
	}

	out_len = sizeof(tmp_buf);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_ECC_PRIVATE_VALUE, tmp_buf,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for ECC_d failed with code 0x%x", res);
		goto exit;
	}

	padding_len = key_size_bytes - out_len;
	TEE_MemFill(ec_key->d, 0, padding_len);
	TEE_MemMove(ec_key->d + padding_len, tmp_buf, out_len);
	ec_key->d_len = key_size_bytes;

	out_len = sizeof(tmp_buf);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_ECC_PUBLIC_VALUE_X, tmp_buf,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for ECC_x failed with code 0x%x", res);
		goto exit;
	}

	padding_len = key_size_bytes - out_len;
	TEE_MemFill(ec_key->x, 0, padding_len);
	TEE_MemMove(ec_key->x + padding_len, tmp_buf, out_len);
	ec_key->x_len = key_size_bytes;

	out_len = sizeof(tmp_buf);
	res = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_ECC_PUBLIC_VALUE_Y, tmp_buf,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for ECC_y failed with code 0x%x", res);
		goto exit;
	}

	padding_len = key_size_bytes - out_len;
	TEE_MemFill(ec_key->y, 0, padding_len);
	TEE_MemMove(ec_key->y + padding_len, tmp_buf, out_len);
	ec_key->y_len = key_size_bytes;

exit:
	TEE_FreeTransientObject(obj);

	return res;
}

TEE_Result rk_cipher_crypto(uint8_t *in, uint8_t *out, uint32_t len,
			    uint8_t *key, uint32_t key_len, uint8_t *iv,
			    uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle crypto_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_ObjectHandle obj_2 = NULL;
	TEE_Attribute attr;
	uint32_t obj_type;
	uint32_t iv_len = 0;
	uint32_t out_size = 0;
	uint32_t obj_key_size = 0;
	uint32_t op_key_size = 0;

	if (in == NULL || out == NULL || key == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_ENCRYPT && mode != TEE_MODE_DECRYPT)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("in is 0x%x; out is 0x%x; key is 0x%x; ", *in, *out, *key);
	IMSG("in_len is %d; key_len is %d; algo is %08x", len, key_len, algo);
	if (iv)
		IMSG("iv is 0x%x", *iv);
#endif
	out_size = len;

	switch (algo) {
	case TEE_ALG_AES_ECB_NOPAD:
	case TEE_ALG_AES_CBC_NOPAD:
	case TEE_ALG_AES_CTR:
	case TEE_ALG_AES_CTS:
		if (key_len != 16 && key_len != 24 && key_len != 32)
			return TEE_ERROR_BAD_PARAMETERS;
		/* CTS is only for data which is not multiple of AES_BLOCK_SIZE
		 * Others except CTR, are only for data which is multiple of AES_BLOCK_SIZE */
		if (((algo == TEE_ALG_AES_CTS) && (len % AES_BLOCK_SIZE == 0)) ||
		    ((algo != TEE_ALG_AES_CTS) &&
		     (algo != TEE_ALG_AES_CTR) && (len % AES_BLOCK_SIZE != 0)))
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_AES;
		iv_len = AES_BLOCK_SIZE;
		obj_key_size = key_len * 8;
		op_key_size = obj_key_size;
		break;
	case TEE_ALG_AES_XTS:
		if (key_len != 32 && key_len != 64)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_AES;
		iv_len = AES_BLOCK_SIZE;
		key_len = key_len / 2;	//Divide the key into two XTS keys.
		obj_key_size = key_len * 8;
		op_key_size = obj_key_size * 2; //Should specifies both two keys.
		break;
	case TEE_ALG_SM4_ECB_NOPAD:
	case TEE_ALG_SM4_CBC_NOPAD:
	case TEE_ALG_SM4_CTR:
		if (key_len != 16)
			return TEE_ERROR_BAD_PARAMETERS;
		/* CTS is only for data which is not multiple of AES_BLOCK_SIZE
		 * Others is only for data which is multiple of AES_BLOCK_SIZE */
		if (len % SM4_BLOCK_SIZE != 0)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_SM4;
		iv_len = SM4_BLOCK_SIZE;
		obj_key_size = key_len * 8;
		op_key_size = obj_key_size;
		break;
	case TEE_ALG_DES_ECB_NOPAD:
	case TEE_ALG_DES_CBC_NOPAD:
		if ((key_len != 8) || (len % DES_BLOCK_SIZE != 0))
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_DES;
		iv_len = DES_BLOCK_SIZE;
		obj_key_size = key_len * 7;
		op_key_size = obj_key_size;
		break;
	case TEE_ALG_DES3_ECB_NOPAD:
	case TEE_ALG_DES3_CBC_NOPAD:
		if ((key_len != 16 && key_len != 24) || (len % DES_BLOCK_SIZE != 0))
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_DES3;
		iv_len = DES_BLOCK_SIZE;
		obj_key_size = key_len * 7;
		op_key_size = obj_key_size;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!iv)
		iv_len = 0;

	res = TEE_AllocateOperation(&crypto_op, algo, mode, op_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	/* Set object of key 1 */
	TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key, key_len);
	res = TEE_AllocateTransientObject(obj_type, obj_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}
	res = TEE_PopulateTransientObject(obj, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	if (algo == TEE_ALG_AES_XTS) {
		/* Set object of key 2 */
		TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key + key_len,
				     key_len);
		res = TEE_AllocateTransientObject(obj_type, obj_key_size, &obj_2);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
			goto exit;
		}
		res = TEE_PopulateTransientObject(obj_2, &attr, 1);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
			goto exit;
		}

		/* Set operation of two keys */
		res = TEE_SetOperationKey2(crypto_op, obj, obj_2);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_SetOperationKey2 failed with code 0x%x", res);
			goto exit;
		}
	} else {
		/* Set operation of one key */
		res = TEE_SetOperationKey(crypto_op, obj);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_SetOperationKey failed with code 0x%x", res);
			goto exit;
		}
	}

	TEE_CipherInit(crypto_op, iv, iv_len);
	res = TEE_CipherDoFinal(crypto_op, in, len, out, &out_size);

	if (res != TEE_SUCCESS) {
		EMSG("TEE_CipherDoFinal failed with code 0x%x", res);
		goto exit;
	}

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (obj_2)
		TEE_FreeTransientObject(obj_2);
	if (crypto_op)
		TEE_FreeOperation(crypto_op);
	return res;
}

TEE_Result rk_set_padding(crypto_ctx_t *ctx, int padding)
{
	if (!ctx)
		return TEE_ERROR_BAD_PARAMETERS;
	if (padding)
		ctx->padding = PKCS7_PADDING;
	else
		ctx->padding = NO_PADDING;

	return TEE_SUCCESS;
}

TEE_Result rk_set_sign_mode(crypto_ctx_t *ctx, unsigned int mode)
{
	if (ctx == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != SIGN_DIGEST && mode != SIGN_DATA)
		return TEE_ERROR_BAD_PARAMETERS;

	ctx->sign_mode = mode;

	return TEE_SUCCESS;
}

static TEE_Result rk_aes_padding_pkcs7(uint8_t *in, uint8_t *out, uint32_t len)
{
	int pad_len;

	if ((!in) || (!out))
		return TEE_ERROR_BAD_PARAMETERS;

	pad_len = AES_BLOCK_SIZE - len % AES_BLOCK_SIZE;

	TEE_MemMove(out, in, len);
	TEE_MemFill(&out[len], pad_len, pad_len);

	return TEE_SUCCESS;
}

static int rk_cut_pkcs7_padding(uint8_t *out, uint32_t *out_len)
{
	int i, n;
	int len = 0;

	if (out == NULL || out_len == NULL)
		return TEE_ERROR_BAD_PARAMETERS;
	if (*out_len == 0)
		return TEE_ERROR_BAD_PARAMETERS;

	len = *out_len;

	n = out[len - 1];

	for (i = 0; i < n; i ++) {
		if (out[len - 1 - i] != n) {
			return TEE_ERROR_BAD_FORMAT;
		}
	}

	*out_len -= n;

	return TEE_SUCCESS;
}

crypto_ctx_t *rk_crypto_malloc_ctx(void)
{
	return TEE_Malloc(sizeof(crypto_ctx_t), TEE_MALLOC_FILL_ZERO);
}

void rk_crypto_free_ctx(crypto_ctx_t **ctx)
{
	if (!*ctx)
		return;

	if ((*ctx)->obj)
		TEE_FreeTransientObject((*ctx)->obj);
	if ((*ctx)->obj_2)
		TEE_FreeTransientObject((*ctx)->obj_2);
	if ((*ctx)->op)
		TEE_FreeOperation((*ctx)->op);

	TEE_MemFill(*ctx, 0, sizeof(crypto_ctx_t));
	TEE_Free(*ctx);
	*ctx = NULL;
}

TEE_Result rk_cipher_begin(crypto_ctx_t *ctx, uint8_t *key, uint32_t key_len,
			   uint8_t *iv, uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle crypto_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_ObjectHandle obj_2 = NULL;
	TEE_Attribute attr;
	uint32_t obj_type;
	uint32_t iv_len = 0;
	uint32_t obj_key_size = 0;
	uint32_t op_key_size = 0;

	if (ctx == NULL || key == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_ENCRYPT && mode != TEE_MODE_DECRYPT)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_CLASS(algo) != TEE_OPERATION_CIPHER)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("key is 0x%x; key_len is %d; algo is %08x", *key, key_len, algo);
	if (iv)
		IMSG("iv is 0x%x", *iv);
#endif

	switch (algo) {
	case TEE_ALG_AES_ECB_NOPAD:
	case TEE_ALG_AES_CBC_NOPAD:
	case TEE_ALG_AES_CTR:
	case TEE_ALG_AES_CTS:
		if (key_len != 16 && key_len != 24 && key_len != 32)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_AES;
		iv_len = AES_BLOCK_SIZE;
		obj_key_size = key_len * 8;
		op_key_size = obj_key_size;
		break;
	case TEE_ALG_AES_XTS:
		if (key_len != 32 && key_len != 64)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_AES;
		iv_len = AES_BLOCK_SIZE;
		key_len = key_len / 2;	//Divide the key into two XTS keys.
		obj_key_size = key_len * 8;
		op_key_size = obj_key_size * 2; //Should specifies both two keys.
		break;
	case TEE_ALG_SM4_ECB_NOPAD:
	case TEE_ALG_SM4_CBC_NOPAD:
	case TEE_ALG_SM4_CTR:
		if (key_len != 16)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_SM4;
		iv_len = SM4_BLOCK_SIZE;
		obj_key_size = key_len * 8;
		op_key_size = obj_key_size;
		break;
	case TEE_ALG_DES_ECB_NOPAD:
	case TEE_ALG_DES_CBC_NOPAD:
		if (key_len != 8)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_DES;
		iv_len = DES_BLOCK_SIZE;
		obj_key_size = key_len * 7;
		op_key_size = obj_key_size;
		break;
	case TEE_ALG_DES3_ECB_NOPAD:
	case TEE_ALG_DES3_CBC_NOPAD:
		if (key_len != 16 && key_len != 24)
			return TEE_ERROR_BAD_PARAMETERS;
		obj_type = TEE_TYPE_DES3;
		iv_len = DES_BLOCK_SIZE;
		obj_key_size = key_len * 7;
		op_key_size = obj_key_size;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!iv)
		iv_len = 0;

	res = TEE_AllocateOperation(&crypto_op, algo, mode, op_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	/* Set object of key 1 */
	TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key, key_len);
	res = TEE_AllocateTransientObject(obj_type, obj_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}
	res = TEE_PopulateTransientObject(obj, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	if (algo == TEE_ALG_AES_XTS) {
		/* Set object of key 2 */
		TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key + key_len,
				     key_len);
		res = TEE_AllocateTransientObject(obj_type, obj_key_size, &obj_2);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
			goto exit;
		}
		res = TEE_PopulateTransientObject(obj_2, &attr, 1);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
			goto exit;
		}

		/* Set operation of two keys */
		res = TEE_SetOperationKey2(crypto_op, obj, obj_2);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_SetOperationKey2 failed with code 0x%x", res);
			goto exit;
		}
	} else {
		/* Set operation of one key */
		res = TEE_SetOperationKey(crypto_op, obj);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_SetOperationKey failed with code 0x%x", res);
			goto exit;
		}
	}

	TEE_CipherInit(crypto_op, iv, iv_len);

	ctx->obj = obj;
	ctx->obj_2 = obj_2;
	ctx->op = crypto_op;
	ctx->algo = algo;
	ctx->mode = mode;
	ctx->buffer_offs = 0;

	return TEE_SUCCESS;

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (obj_2)
		TEE_FreeTransientObject(obj_2);
	if (crypto_op)
		TEE_FreeOperation(crypto_op);

	return res;
}

static TEE_Result rk_cipher_buffer_update(crypto_ctx_t *ctx, uint8_t *in,
					  uint8_t *out,
					  uint32_t in_len, uint32_t *out_len)
{
	TEE_Result res = 0;
	uint32_t real_in_len = 0;
	uint32_t tmp_out_len = 0;
	uint32_t out_size = 0;
	uint32_t buf_offs = 0;

#if CRYPTO_DEBUG
	IMSG("ctx->buffer_offs: %d; ctx->algo: 0x%x",
	     ctx->buffer_offs, ctx->algo);
	if (in && out && out_len)
		IMSG("in is 0x%x; out is 0x%x; in_len is %d; out_len is %d",
		     *in, *out, in_len, *out_len);
#endif

	buf_offs = ctx->buffer_offs;

	if ((ctx->buffer_offs + in_len) <= AES_BLOCK_SIZE) {
		TEE_MemMove(ctx->buffer + ctx->buffer_offs, in, in_len);
		ctx->buffer_offs += in_len;
		out_size = 0;
		if ((ctx->buffer_offs == AES_BLOCK_SIZE) &&
		    ((ctx->padding == NO_PADDING) ||
		     ((ctx->padding == PKCS7_PADDING) && ctx->mode == TEE_MODE_ENCRYPT))) {
			tmp_out_len = AES_BLOCK_SIZE;
			if (*out_len < tmp_out_len)
				return TEE_ERROR_SHORT_BUFFER;
			res = TEE_CipherUpdate(ctx->op, ctx->buffer, ctx->buffer_offs, out,
					       &tmp_out_len);
			if (res != TEE_SUCCESS) {
				EMSG("TEE_CipherUpdate failed with code 0x%x\n", res);
				return res;
			}
			ctx->buffer_offs = 0;
			out_size += tmp_out_len;
		}
	} else {
		if (ctx->padding == NO_PADDING) {
			real_in_len = ((ctx->buffer_offs + in_len) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
		} else if (ctx->padding == PKCS7_PADDING) {
			if (ctx->mode == TEE_MODE_DECRYPT)
				real_in_len = ROUNDUP(ctx->buffer_offs + in_len,
						      AES_BLOCK_SIZE) - AES_BLOCK_SIZE;
			else
				real_in_len = ROUNDDOWN(ctx->buffer_offs + in_len, AES_BLOCK_SIZE);
		}

		if (*out_len < real_in_len)
			return TEE_ERROR_SHORT_BUFFER;

		/* update one block size data */
		TEE_MemMove(ctx->buffer + ctx->buffer_offs, in,
			    AES_BLOCK_SIZE - ctx->buffer_offs);
		tmp_out_len = AES_BLOCK_SIZE;
		res = TEE_CipherUpdate(ctx->op, ctx->buffer, AES_BLOCK_SIZE, out, &tmp_out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_CipherUpdate failed with code 0x%x\n", res);
			return res;
		}
		out_size += tmp_out_len;

		tmp_out_len = real_in_len - AES_BLOCK_SIZE;
		res = TEE_CipherUpdate(ctx->op, in + AES_BLOCK_SIZE - ctx->buffer_offs,
				       tmp_out_len, out + AES_BLOCK_SIZE, &tmp_out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_CipherUpdate failed with code 0x%x\n", res);
			return res;
		}
		out_size += tmp_out_len;

		tmp_out_len = (ctx->buffer_offs + in_len) % AES_BLOCK_SIZE;
		if (ctx->padding == NO_PADDING) {
			ctx->buffer_offs = tmp_out_len;
		} else if (ctx->padding == PKCS7_PADDING) {
			if (ctx->mode == TEE_MODE_DECRYPT)
				ctx->buffer_offs = tmp_out_len == 0 ? 16 : tmp_out_len;
			else
				ctx->buffer_offs = tmp_out_len;
		}

		TEE_MemMove(ctx->buffer, in + real_in_len - buf_offs, ctx->buffer_offs);
	}

	*out_len = out_size;

#if CRYPTO_DEBUG
	IMSG("out[0] is 0x%x; out_size is %d", *out, out_size);
#endif

	return TEE_SUCCESS;
}

TEE_Result rk_cipher_update(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			    uint32_t in_len, uint32_t *out_len)
{
	TEE_Result res = 0;

	if (ctx == NULL ||
	    (in == NULL && in_len != 0) ||
	    out_len == NULL ||
	    (out == NULL && *out_len != 0))
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo);
	IMSG("in is 0x%x; out is 0x%x; in_len is %d; out_len is %d",
	     *in, *out, in_len, *out_len);
#endif

	if (ctx->algo == TEE_ALG_AES_ECB_NOPAD ||
	    ctx->algo == TEE_ALG_AES_CBC_NOPAD ||
	    ctx->algo == TEE_ALG_SM4_ECB_NOPAD ||
	    ctx->algo == TEE_ALG_SM4_CBC_NOPAD) {
		res = rk_cipher_buffer_update(ctx, in, out, in_len, out_len);
		if (res != TEE_SUCCESS) {
			EMSG("rk_cipher_buffer_update failed with code 0x%x\n", res);
			return res;
		}
	} else {
		res = TEE_CipherUpdate(ctx->op, in, in_len, out, out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_CipherUpdate failed with code 0x%x\n", res);
			return res;
		}
	}

	return TEE_SUCCESS;
}

TEE_Result rk_cipher_finish(crypto_ctx_t *ctx, uint8_t *out, uint32_t *out_len)
{
	TEE_Result res = 0;
	uint8_t feed_in[32];
	int fd_len = 0;

	if (ctx == NULL ||
	    out_len == NULL ||
	    (out == NULL && *out_len != 0))
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x; ctx->padding: %d",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo, ctx->padding);
#endif

	if ((ctx->algo == TEE_ALG_AES_ECB_NOPAD) ||
	    (ctx->algo == TEE_ALG_AES_CBC_NOPAD) ||
	    (ctx->algo == TEE_ALG_SM4_ECB_NOPAD) ||
	    (ctx->algo == TEE_ALG_SM4_CBC_NOPAD)) {
		if (ctx->padding == NO_PADDING) {
			if (ctx->buffer_offs % AES_BLOCK_SIZE) {
				EMSG("The length of all data is NOT multiple of block size");
				return TEE_ERROR_BAD_PARAMETERS;
			}
			res = TEE_CipherDoFinal(ctx->op, NULL, 0, out, out_len);
			if (res != TEE_SUCCESS) {
				EMSG("TEE_CipherDoFinal failed with code 0x%x", res);
				return res;
			}
		} else if (ctx->padding == PKCS7_PADDING) {
			if (ctx->mode == TEE_MODE_ENCRYPT) {
				res = rk_aes_padding_pkcs7(ctx->buffer, feed_in, ctx->buffer_offs);
				if (res != TEE_SUCCESS) {
					EMSG("rk_aes_padding_pkcs7 failed with 0x%x", res);
					return res;
				}

				fd_len = (ctx->buffer_offs / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;

				if ((int)*out_len < fd_len)
					return TEE_ERROR_SHORT_BUFFER;

				res = TEE_CipherDoFinal(ctx->op, feed_in, fd_len, out, out_len);
				if (res != TEE_SUCCESS) {
					EMSG("TEE_CipherDoFinal failed with code 0x%x", res);
					return res;
				}
			} else {
				if (*out_len < ctx->buffer_offs) {
					return TEE_ERROR_SHORT_BUFFER;
				}
				if (ctx->buffer_offs % AES_BLOCK_SIZE) {
					EMSG("The length of all data is NOT multiple of block size");
					return TEE_ERROR_BAD_PARAMETERS;
				}
				res = TEE_CipherDoFinal(ctx->op, ctx->buffer, ctx->buffer_offs, out, out_len);
				if (res != TEE_SUCCESS) {
					EMSG("TEE_CipherDoFinal failed with code 0x%x", res);
					return res;
				}

				res = rk_cut_pkcs7_padding(out, out_len);
				if (res != TEE_SUCCESS) {
					EMSG("rk_cut_pkcs7_padding failed with code 0x%x", res);
					return res;
				}

			}
		}
	} else {
		res = TEE_CipherDoFinal(ctx->op, NULL, 0, out, out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_CipherDoFinal failed with code 0x%x", res);
			return res;
		}
	}

	return TEE_SUCCESS;
}

TEE_Result rk_ae_begin(crypto_ctx_t *ctx, uint8_t *key, uint32_t key_len,
		       uint8_t *iv, uint32_t iv_len, uint32_t aad_len, uint32_t tag_len,
		       uint32_t payload_len, uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle crypto_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_Attribute attr;
	uint32_t obj_type;
	uint32_t max_key_size = 0;
	uint32_t tag_bit_len = tag_len * 8;

	if (ctx == NULL || key == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_ENCRYPT && mode != TEE_MODE_DECRYPT)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("key is 0x%x; key_len is %d; algo is %08x",
	     *key, key_len, algo);
	IMSG("aad_len is %d; tag_len is %d; payload_len is %d",
	     aad_len, tag_len, payload_len);
	if (iv)
		IMSG("iv is 0x%x", *iv);
	IMSG("iv_len is %d", iv_len);
#endif

	if (key_len != 16 && key_len != 24 && key_len != 32)
		return TEE_ERROR_BAD_PARAMETERS;
	obj_type = TEE_TYPE_AES;
	max_key_size = key_len * 8;

	if (algo == TEE_ALG_AES_GCM) {
		if (iv_len > AES_BLOCK_SIZE)
			return TEE_ERROR_BAD_PARAMETERS;
		if (tag_bit_len != 128 && tag_bit_len != 120 && tag_bit_len != 112 &&
		    tag_bit_len != 104 && tag_bit_len != 96)
			return TEE_ERROR_BAD_PARAMETERS;
	} else if (algo == TEE_ALG_AES_CCM) {
		if (iv_len > 15 || iv_len < 7 || iv == NULL)
			return TEE_ERROR_BAD_PARAMETERS;
		if (tag_bit_len != 128 && tag_bit_len != 112 && tag_bit_len != 96 &&
		    tag_bit_len != 80 && tag_bit_len != 64 && tag_bit_len != 48 &&
		    tag_bit_len != 32)
			return TEE_ERROR_BAD_PARAMETERS;
	} else {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key, key_len);

	res = TEE_AllocateTransientObject(obj_type, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(obj, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&crypto_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(crypto_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AEInit(crypto_op, iv, iv_len, tag_bit_len, aad_len, payload_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AEInit failed with code 0x%x", res);
		goto exit;
	}

	ctx->obj = obj;
	ctx->op = crypto_op;
	ctx->algo = algo;
	ctx->mode = mode;

	return TEE_SUCCESS;

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (crypto_op)
		TEE_FreeOperation(crypto_op);

	return res;
}


TEE_Result rk_ae_update(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			uint32_t in_len, uint32_t *out_len, rk_ae_update_type_t is_aad)
{
	TEE_Result res = 0;

	if (ctx == NULL || (in == NULL && in_len != 0))
		return TEE_ERROR_BAD_PARAMETERS;

	if (!is_aad && (out_len == NULL || (out == NULL && *out_len != 0)))
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (is_aad) {
		TEE_AEUpdateAAD(ctx->op, in, in_len);
	} else {
		res = TEE_AEUpdate(ctx->op, in, in_len, out, out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AEUpdate failed with code 0x%x", res);
			return res;
		}
	}

	return TEE_SUCCESS;
}

TEE_Result rk_ae_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			uint8_t *tag,
			uint32_t in_len, uint32_t *out_len, uint32_t *tag_len)
{
	TEE_Result res = 0;
	uint32_t tmp_out_len;
	uint32_t tmp_tag_len;

	if (ctx == NULL ||
	    (in == NULL && in_len != 0) ||
	    out_len == NULL ||
	    (out == NULL && *out_len != 0) ||
	    tag == NULL || tag_len == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	tmp_tag_len = *tag_len;
	tmp_out_len = *out_len;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x;ctx->mode: 0x%x",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo, ctx->mode);
	IMSG("in_len = %d, out_len = %d, tag_len = %d",
	     in_len, tmp_out_len, tmp_tag_len);
#endif

	if (ctx->mode == TEE_MODE_ENCRYPT) {
		res = TEE_AEEncryptFinal(ctx->op, in, in_len, out, &tmp_out_len,
					 tag, &tmp_tag_len);
	} else {
		uint8_t tmp_tag[16];

		if (tmp_tag_len > sizeof(tmp_tag))
			return TEE_ERROR_BAD_PARAMETERS;

		TEE_MemMove(tmp_tag, tag, tmp_tag_len);
		res = TEE_AEDecryptFinal(ctx->op, in, in_len, out, &tmp_out_len,
					 tmp_tag, tmp_tag_len);
	}

	*tag_len = tmp_tag_len;
	*out_len = tmp_out_len;

	if (res != TEE_SUCCESS) {
		EMSG("TEE_AEFinal failed with code 0x%x", res);
		return res;
	}

	return TEE_SUCCESS;
}

TEE_Result rk_hash_begin(crypto_ctx_t *ctx, uint32_t algo)
{
	TEE_Result res = 0;
	TEE_OperationHandle hash_op = NULL;

	if (!ctx)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_CLASS(algo) != TEE_OPERATION_DIGEST)
		return TEE_ERROR_BAD_PARAMETERS;

	res = TEE_AllocateOperation(&hash_op, algo, TEE_MODE_DIGEST, 0);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		return res;
	}

	ctx->op = hash_op;

	return TEE_SUCCESS;
}

TEE_Result rk_hash_update(crypto_ctx_t *ctx, uint8_t *in, uint32_t in_len)
{
	if (ctx == NULL || (in == NULL && in_len != 0)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (ctx->op == NULL) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	TEE_DigestUpdate(ctx->op, in, in_len);

	return TEE_SUCCESS;
}

TEE_Result rk_hash_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			  uint32_t in_len, uint32_t *out_len)
{
	TEE_Result res = 0;

	if (ctx == NULL ||
	    (in == NULL && in_len != 0) ||
	    out_len == NULL ||
	    (out == NULL && *out_len != 0)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (ctx->op == NULL) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	res = TEE_DigestDoFinal(ctx->op, in, in_len, out, out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_DigestDoFinal failed with code 0x%x", res);
		return res;
	}

	return TEE_SUCCESS;
}

TEE_Result rk_hash_crypto(uint8_t *in, uint8_t *out, uint32_t in_len,
			  uint32_t out_len, uint32_t algo)
{
	TEE_Result res = TEE_SUCCESS;
	crypto_ctx_t *ctx = NULL;
	uint32_t out_size = out_len;

	ctx = rk_crypto_malloc_ctx();
	if (!ctx) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	res = rk_hash_begin(ctx, algo);
	if (res) {
		EMSG("---rk_hash_begin---FAILED! return value: 0x%x", res);
		goto exit;
	}

	res = rk_hash_update(ctx, in, in_len);
	if (res) {
		EMSG("---rk_hash_update---FAILED! return value: 0x%x", res);
		goto exit;
	}

	res = rk_hash_finish(ctx, NULL, out, 0, &out_size);
	if (res) {
		EMSG("---rk_hash_finish---FAILED! return value: 0x%x", res);
		goto exit;
	}

	if (out_size != out_len) {
		EMSG("out_size:%d != out_len:%d", out_size, out_len);
		res = TEE_ERROR_GENERIC;
		goto exit;
	}

exit:
	if (ctx)
		rk_crypto_free_ctx(&ctx);

	return res;
}

TEE_Result rk_mac_crypto(uint8_t *in, uint8_t *out, uint32_t in_len,
			 uint32_t *out_len, uint8_t *key, uint32_t key_len,
			 uint8_t *iv, uint32_t algo)
{
	TEE_OperationHandle mac_op;
	TEE_ObjectHandle obj;
	TEE_Attribute attr;
	TEE_Result res = 0;
	uint32_t obj_type = 0;
	uint32_t iv_len = 0;
	uint32_t out_size = 0;
	uint32_t max_key_size = 0;

	if (!in || !out || !key || !in_len || !key_len)
		return TEE_ERROR_BAD_PARAMETERS;

	/* AES_CMAC is only for data which is larger than AES_BLOCK_SIZE */
	if (algo == TEE_ALG_AES_CMAC && in_len < AES_BLOCK_SIZE)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("in is 0x%x; out is 0x%x; key is 0x%x", *in, *out, *key);
	IMSG("in_len is %d; out_len is %d; key_len is %d; iv_len is %d; algo is %08x",
	     in_len, *out_len, key_len, iv_len, algo);
	if (iv)
		IMSG("iv is 0x%x", *iv);
#endif

	switch (algo) {
	case TEE_ALG_AES_CMAC:
		obj_type = TEE_TYPE_AES;
		iv_len = AES_BLOCK_SIZE;
		max_key_size = key_len * 8;
		out_size = AES_BLOCK_SIZE;
		break;
	case TEE_ALG_HMAC_SHA256:
		obj_type = TEE_TYPE_HMAC_SHA256;
		iv_len = AES_BLOCK_SIZE;
		max_key_size = key_len * 8;
		out_size = SHA256_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SHA1:
		obj_type = TEE_TYPE_HMAC_SHA1;
		iv_len = AES_BLOCK_SIZE;
		max_key_size = key_len * 8;
		out_size = SHA1_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_MD5:
		obj_type = TEE_TYPE_HMAC_MD5;
		iv_len = AES_BLOCK_SIZE;
		max_key_size = key_len * 8;
		out_size = MD5_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SM3:
		obj_type = TEE_TYPE_HMAC_SM3;
		iv_len = AES_BLOCK_SIZE;
		max_key_size = key_len * 8;
		out_size = SM3_HASH_SIZE;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!iv)
		iv_len = 0;

	TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key, key_len);

	res = TEE_AllocateTransientObject(obj_type, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(obj, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&mac_op, algo, TEE_MODE_MAC, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(mac_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	TEE_MACInit(mac_op, iv, iv_len);
	res = TEE_MACComputeFinal(mac_op, in, in_len, out, &out_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_MACComputeFinal failed with code 0x%x", res);
		goto exit;
	}

	*out_len = out_size;
exit:
	TEE_FreeTransientObject(obj);
	TEE_FreeOperation(mac_op);

	return res;
}

TEE_Result rk_mac_begin(crypto_ctx_t *ctx, uint8_t *key, uint32_t key_len,
			uint8_t *iv, uint32_t algo)
{
	TEE_OperationHandle mac_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_Attribute attr;
	TEE_Result res = 0;
	uint32_t obj_type = 0;
	uint32_t iv_len = 0;
	uint32_t max_key_size = 0;
	uint32_t key_block[MAX_HASH_BLOCK_SIZE];
	uint32_t key_block_len = 0;
	uint32_t block_size = 0;
	uint32_t hash_algo;

	if (ctx == NULL || key == NULL || key_len == 0)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("key is 0x%x; key_len is %d; iv_len is %d; algo is %08x",
	     *key, key_len, iv_len, algo);
	if (iv)
		IMSG("iv is 0x%x", *iv);
#endif

	TEE_MemFill(key_block, 0, sizeof(key_block));

	switch (algo) {
	case TEE_ALG_AES_CMAC:
		obj_type = TEE_TYPE_AES;
		iv_len = AES_BLOCK_SIZE;
		max_key_size = key_len * 8;
		if (max_key_size != 128 &&
		    max_key_size != 192 &&
		    max_key_size != 256)
			return TEE_ERROR_BAD_PARAMETERS;
		TEE_MemMove(key_block, key, key_len);
		key_block_len = key_len;
		break;
	case TEE_ALG_HMAC_SHA1:
		obj_type = TEE_TYPE_HMAC_SHA1;
		block_size = 64;
		hash_algo = TEE_ALG_SHA1;
		key_block_len = SHA1_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SHA224:
		obj_type = TEE_TYPE_HMAC_SHA224;
		block_size = 64;
		key_block_len = SHA224_HASH_SIZE;
		hash_algo = TEE_ALG_SHA224;
		break;
	case TEE_ALG_HMAC_SHA256:
		obj_type = TEE_TYPE_HMAC_SHA256;
		block_size = 64;
		key_block_len = SHA256_HASH_SIZE;
		hash_algo = TEE_ALG_SHA256;
		break;
	case TEE_ALG_HMAC_SHA384:
		obj_type = TEE_TYPE_HMAC_SHA384;
		block_size = 128;
		key_block_len = SHA384_HASH_SIZE;
		hash_algo = TEE_ALG_SHA384;
		break;
	case TEE_ALG_HMAC_SHA512:
		obj_type = TEE_TYPE_HMAC_SHA512;
		block_size = 128;
		key_block_len = SHA512_HASH_SIZE;
		hash_algo = TEE_ALG_SHA512;
		break;
	case TEE_ALG_HMAC_MD5:
		obj_type = TEE_TYPE_HMAC_MD5;
		block_size = 64;
		key_block_len = MD5_HASH_SIZE;
		hash_algo = TEE_ALG_MD5;
		break;
	case TEE_ALG_HMAC_SM3:
		obj_type = TEE_TYPE_HMAC_SM3;
		block_size = 64;
		hash_algo = TEE_ALG_SM3;
		key_block_len = SM3_HASH_SIZE;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!iv)
		iv_len = 0;

	/* handle the key size for HMAC */
	if (algo != TEE_ALG_AES_CMAC) {
		iv_len = 0;
		/* Long keys are hashed. */
		if (key_len > block_size) {
			res = rk_hash_crypto(key, (uint8_t *)key_block, key_len, key_block_len,
					     hash_algo);
			if (res) {
				EMSG("Hash long keys failed.");
				return res;
			}
			/* Min key size is set as 64 bits in OS. */
		} else if (key_len < 8) {
			TEE_MemMove(key_block, key, key_len);
			key_block_len = 8;
		} else {
			if (key_len <= sizeof(key_block)) {
				TEE_MemMove(key_block, key, key_len);
				key_block_len = key_len;
			} else {
				EMSG("Check key len failed.");
				res = TEE_ERROR_GENERIC;
				return res;
			}
		}
		max_key_size = key_block_len * 8;
	}

	TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key_block, key_block_len);

	res = TEE_AllocateTransientObject(obj_type, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(obj, &attr, 1);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&mac_op, algo, TEE_MODE_MAC, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(mac_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	TEE_MACInit(mac_op, iv, iv_len);

	ctx->obj = obj;
	ctx->op = mac_op;
	ctx->algo = algo;

	return TEE_SUCCESS;

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (mac_op)
		TEE_FreeOperation(mac_op);

	return res;
}

TEE_Result rk_mac_update(crypto_ctx_t *ctx, uint8_t *in, uint32_t in_len)
{
	if (ctx == NULL || (in == NULL && in_len != 0))
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo);
	IMSG("in is 0x%x; in_len is %d", *in, in_len);
#endif

	TEE_MACUpdate(ctx->op, in, in_len);

	return TEE_SUCCESS;
}

TEE_Result rk_mac_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *mac,
			 uint32_t in_len, uint32_t *mac_len, rk_mac_mode_t mode)
{
	TEE_Result res = 0;
	uint32_t out_size = 0;

	if (ctx == NULL ||
	    (in == NULL && in_len != 0) ||
	    mac_len == NULL ||
	    mac == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo);
	IMSG("mac is 0x%x; in_len is %d; mac_len is %d",
	     *mac, in_len, *mac_len);
	if (in)
		IMSG("in is 0x%x", *in);
#endif

	switch (ctx->algo) {
	case TEE_ALG_AES_CMAC:
		out_size = AES_BLOCK_SIZE;
		break;
	case TEE_ALG_HMAC_SHA1:
		out_size = SHA1_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SHA224:
		out_size = SHA224_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SHA256:
		out_size = SHA256_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SHA384:
		out_size = SHA384_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SHA512:
		out_size = SHA512_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_MD5:
		out_size = MD5_HASH_SIZE;
		break;
	case TEE_ALG_HMAC_SM3:
		out_size = SM3_HASH_SIZE;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (mode == RK_MAC_SIGN) {
		res = TEE_MACComputeFinal(ctx->op, in, in_len, mac, &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_MACComputeFinal failed with code 0x%x", res);
			return res;
		}
		*mac_len = out_size;
	} else if (mode == RK_MAC_VERIFY) {
		res = TEE_MACCompareFinal(ctx->op, in, in_len, mac, *mac_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_MACCompareFinal failed with code 0x%x", res);
			return res;
		}
	} else {
		EMSG("Unsupport mode: 0x%x", mode);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

TEE_Result rk_rsa_begin(crypto_ctx_t *ctx, rsa_key_t *key, uint32_t algo,
			TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle rsa_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_Attribute attr[3];
	uint32_t max_key_size = 0;

	if (!key || !ctx)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_ENCRYPT && mode != TEE_MODE_DECRYPT &&
	    mode != TEE_MODE_SIGN && mode != TEE_MODE_VERIFY)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_RSA)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("key->n is 0x%x; key->e is 0x%x; key->d is 0x%x",
	     *(key->n), *(key->e), *(key->d));
	IMSG("key->key_len is %d; algo is 0x%x; mode is 0x%x",
	     key->key_len, algo, mode);
#endif

	max_key_size = key->key_len * 8;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_RSA_MODULUS, key->n, key->key_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_RSA_PUBLIC_EXPONENT, key->e,
			     key->e_len);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_RSA_PRIVATE_EXPONENT, key->d,
			     key->d_len);

	res = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		return res;
	}

	res = TEE_PopulateTransientObject(obj, attr, 3);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&rsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(rsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	ctx->obj = obj;
	ctx->op = rsa_op;
	ctx->algo = algo;
	ctx->mode = mode;

	return TEE_SUCCESS;

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (rsa_op)
		TEE_FreeOperation(rsa_op);

	return res;
}

TEE_Result rk_rsa_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			 uint32_t in_len,
			 uint32_t *out_len, uint32_t salt_len)
{
	TEE_Result res = 0;
	TEE_Attribute attr;

	if (ctx == NULL ||
	    (in == NULL && in_len != 0) ||
	    out_len == NULL ||
	    (out == NULL && *out_len != 0))
		return TEE_ERROR_BAD_PARAMETERS;
	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo);
	IMSG("salt_len is %d", salt_len);
	if (in != NULL && in_len != 0)
		IMSG("in is 0x%x; in_len is %d; ", *in, in_len);
	if (out != NULL && *out_len != 0)
		IMSG("out is 0x%x; out_len is %d;", *out, *out_len);
#endif

	TEE_InitValueAttribute(&attr, TEE_ATTR_RSA_PSS_SALT_LENGTH, salt_len, 0);

	switch (ctx->mode) {
	case TEE_MODE_ENCRYPT:
		res = TEE_AsymmetricEncrypt(ctx->op, NULL, 0, in, in_len, out, out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricEncrypt failed with code 0x%x", res);
			return res;
		}
		break;
	case TEE_MODE_DECRYPT:
		res = TEE_AsymmetricDecrypt(ctx->op, NULL, 0, in, in_len, out, out_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricDecrypt failed with code 0x%x", res);
			return res;
		}
		break;
	case TEE_MODE_SIGN:
		if (ctx->sign_mode == SIGN_DIGEST) {
			res = TEE_AsymmetricSignDigest(ctx->op, &attr, 1, in, in_len, out, out_len);
		} else if (ctx->sign_mode == SIGN_DATA) {
			res = TEE_AsymmetricPrivateEncrypt(ctx->op, NULL, 0, in, in_len, out, out_len);
		} else {
			EMSG("Error sign mode!");
			res = TEE_ERROR_BAD_PARAMETERS;
		}

		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricSignDigest failed with code 0x%x", res);
			return res;
		}
		break;
	case TEE_MODE_VERIFY:
		if (ctx->sign_mode == SIGN_DIGEST) {
			res = TEE_AsymmetricVerifyDigest(ctx->op, &attr, 1, in, in_len, out, *out_len);
		} else if (ctx->sign_mode == SIGN_DATA) {
			res = TEE_AsymmetricPublicDecrypt(ctx->op, NULL, 0, in, in_len, out, out_len);
		} else {
			EMSG("Error sign mode!");
			res = TEE_ERROR_BAD_PARAMETERS;
		}

		if (res != TEE_SUCCESS) {
			if (res == TEE_ERROR_SIGNATURE_INVALID) {
				EMSG("Verify failed!!!The signature is invalid!!!");
			} else {
				EMSG("TEE_AsymmetricVerifyDigest failed with code 0x%x", res);
			}
			return res;
		}
		break;
	default:
		EMSG("Unknown mode!!!");
		return TEE_ERROR_BAD_PARAMETERS;
	}

#if CRYPTO_DEBUG
	IMSG("out[0] is 0x%x; out_len is %d; in_len is %d",
	     *out, *out_len, in_len);
#endif

	return TEE_SUCCESS;
}

TEE_Result rk_rsa_crypto(uint8_t *in, uint8_t *out, uint32_t len,
			 rsa_key_t *key, uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle rsa_op;
	TEE_ObjectHandle obj;
	TEE_Attribute attr[3];
	uint32_t out_size = 0;
	uint32_t max_key_size = 0;

	if (!in || !out || !key || !len)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_ENCRYPT && mode != TEE_MODE_DECRYPT)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_RSA)
		return TEE_ERROR_BAD_PARAMETERS;
#if CRYPTO_DEBUG
	IMSG("in is 0x%x; out is 0x%x; len is %d; key_len is 0x%x",
	     *in, *out, len, key->key_len);
#endif

	out_size = key->key_len;

	max_key_size = key->key_len * 8;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_RSA_MODULUS, key->n, key->key_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_RSA_PUBLIC_EXPONENT, key->e, 3);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_RSA_PRIVATE_EXPONENT, key->d,
			     key->d_len);

	res = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(obj, attr, 3);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&rsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(rsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	if (mode == TEE_MODE_ENCRYPT) {
		res = TEE_AsymmetricEncrypt(rsa_op, NULL, 0, in, len, out, &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricEncrypt failed with code 0x%x", res);
			goto exit;
		}
	} else {
		res = TEE_AsymmetricDecrypt(rsa_op, NULL, 0, in, len, out, &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricDecrypt failed with code 0x%x", res);
			goto exit;
		}
	}

#if CRYPTO_DEBUG
	IMSG("out[0] is 0x%x; out_size is %d; len is %d",
	     *out, out_size, len);
#endif

exit:
	TEE_FreeTransientObject(obj);
	TEE_FreeOperation(rsa_op);

	return res;
}

TEE_Result rk_rsa_sign(uint8_t *digest, uint8_t *signature, uint32_t digest_len,
		       uint32_t *signature_len, rsa_key_t *key, uint32_t salt_len,
		       uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle rsa_op;
	TEE_ObjectHandle obj;
	TEE_Attribute attr[4];
	uint32_t out_size = 0;
	uint32_t max_key_size = 0;

	if (!digest || !signature || !signature_len || !key)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_SIGN && mode != TEE_MODE_VERIFY)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_RSA)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("digest is 0x%x; signature is 0x%x; digest_len is %d; salt_len is %d",
	     *digest, *signature, digest_len, salt_len);
	IMSG("key->n is 0x%x; key->e is 0x%x; key->d is 0x%x",
	     *key->n, *key->e, *key->d);
#endif
	out_size = *signature_len;
	max_key_size = key->key_len * 8;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_RSA_MODULUS, key->n, key->key_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_RSA_PUBLIC_EXPONENT, key->e, 3);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_RSA_PRIVATE_EXPONENT, key->d,
			     key->d_len);
	TEE_InitValueAttribute(&attr[3], TEE_ATTR_RSA_PSS_SALT_LENGTH, salt_len, 0);

	res = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(obj, attr, 3);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&rsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(rsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	if (mode == TEE_MODE_SIGN) {
		res = TEE_AsymmetricSignDigest(rsa_op, &attr[3], 1, digest, digest_len,
					       signature, &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricSignDigest failed with code 0x%x", res);
			goto exit;
		}
		*signature_len = out_size;
	} else {
		res = TEE_AsymmetricVerifyDigest(rsa_op, &attr[3], 1, digest, digest_len,
						 signature, out_size);
		if (res != TEE_SUCCESS) {
			if (res == TEE_ERROR_SIGNATURE_INVALID) {
				EMSG("Verify failed!!!The signature is invalid!!!");
			} else {
				EMSG("TEE_AsymmetricVerifyDigest failed with code 0x%x", res);
			}
		}
	}

exit:
	TEE_FreeTransientObject(obj);
	TEE_FreeOperation(rsa_op);

	return res;
}

TEE_Result rk_ecdsa_sign(uint8_t *digest, uint8_t *signature,
			 uint32_t digest_len, uint32_t *signature_len,
			 ec_key_t *key, uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = 0;
	TEE_OperationHandle ecdsa_op;
	TEE_ObjectHandle obj;
	TEE_Attribute attr[4];
	uint32_t out_size = 0;
	uint32_t max_key_size = 0;

	if (!digest || !signature || !signature_len || !key)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_SIGN && mode != TEE_MODE_VERIFY)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_ECDSA)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("digest is 0x%x; signature is 0x%x; digest_len is %d; signature_len is %d",
	     *digest, *signature, digest_len, *signature_len);
	IMSG("key->d is 0x%x; key->x is 0x%x; key->y is 0x%x",
	     *key->d, *key->x, *key->y);
	IMSG("key->d_len is %d; key->x_len is %d; key->y_len is %d",
	     key->d_len, key->x_len, key->y_len);
	IMSG("key->curve is 0x%x; key->key_len is %d; algo is 0x%x; mode is 0x%x",
	     key->curve, key->key_len, algo, mode);
#endif
	out_size = *signature_len;
	max_key_size = key->key_len;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_ECC_PUBLIC_VALUE_X, key->x, key->x_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_ECC_PUBLIC_VALUE_Y, key->y, key->y_len);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_ECC_PRIVATE_VALUE, key->d, key->d_len);
	TEE_InitValueAttribute(&attr[3], TEE_ATTR_ECC_CURVE, key->curve, 0);

	res = TEE_AllocateTransientObject(TEE_TYPE_ECDSA_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		return res;
	}

	res = TEE_PopulateTransientObject(obj, attr, 4);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&ecdsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(ecdsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	if (mode == TEE_MODE_SIGN) {
		res = TEE_AsymmetricSignDigest(ecdsa_op, NULL, 0, digest, digest_len, signature,
					       &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricSignDigest failed with code 0x%x", res);
			goto exit;
		}
		*signature_len = out_size;
	} else {
		res = TEE_AsymmetricVerifyDigest(ecdsa_op, NULL, 0, digest, digest_len,
						 signature, out_size);
		if (res != TEE_SUCCESS) {
			if (res == TEE_ERROR_SIGNATURE_INVALID) {
				EMSG("Verify failed!!!The signature is invalid!!!");
			} else {
				EMSG("TEE_AsymmetricVerifyDigest failed with code 0x%x", res);
			}
		}
	}

exit:
	TEE_FreeTransientObject(obj);
	TEE_FreeOperation(ecdsa_op);

	return res;
}

TEE_Result rk_sm2_pke(uint8_t *in, uint32_t in_len, uint8_t *out,
		      uint32_t *out_len,
		      ec_key_t *key, uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_OperationHandle ecdsa_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_Attribute attr[4];
	uint32_t out_size = 0;
	uint32_t max_key_size = 0;

	if (!in || !out || !out_len || !key)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("in is 0x%x; out is 0x%x; in_len is %d; out_len is %d",
	     *in, *out, in_len, *out_len);
	IMSG("key->d is 0x%x; key->x is 0x%x; key->y is 0x%x",
	     *key->d, *key->x, *key->y);
	IMSG("key->d_len is %d; key->x_len is %d; key->y_len is %d",
	     key->d_len, key->x_len, key->y_len);
	IMSG("key->curve is 0x%x; key->key_len is %d; algo is 0x%x; mode is 0x%x",
	     key->curve, key->key_len, algo, mode);
#endif

	if (key->curve != TEE_ECC_CURVE_SM2) {
		EMSG("curve = %08x != TEE_ECC_CURVE_SM2", key->curve);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (algo != TEE_ALG_SM2_PKE) {
		EMSG("algo = %08x != TEE_ALG_SM2_PKE", algo);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (mode != TEE_MODE_ENCRYPT && mode != TEE_MODE_DECRYPT)
		return TEE_ERROR_BAD_PARAMETERS;

	out_size = *out_len;
	max_key_size = key->key_len;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_ECC_PUBLIC_VALUE_X, key->x, key->x_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_ECC_PUBLIC_VALUE_Y, key->y, key->y_len);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_ECC_PRIVATE_VALUE, key->d, key->d_len);
	TEE_InitValueAttribute(&attr[3], TEE_ATTR_ECC_CURVE, key->curve, 0);

	res = TEE_AllocateTransientObject(TEE_TYPE_SM2_PKE_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		return res;
	}

	res = TEE_PopulateTransientObject(obj, attr, 4);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&ecdsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(ecdsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	if (mode == TEE_MODE_ENCRYPT) {
		res = TEE_AsymmetricEncrypt(ecdsa_op, NULL, 0, in, in_len, out, &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricEncrypt failed with code 0x%x", res);
			goto exit;
		}
	} else {
		res = TEE_AsymmetricDecrypt(ecdsa_op, NULL, 0, in, in_len, out, &out_size);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricDecrypt failed with code 0x%x", res);
			goto exit;
		}
	}

	*out_len = out_size;

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (ecdsa_op)
		TEE_FreeOperation(ecdsa_op);

	return res;
}

TEE_Result rk_sm2_dsa_sm3(uint8_t *digest, uint32_t digest_len,
			  uint8_t *signature, uint32_t *signature_len,
			  ec_key_t *key, uint32_t algo, TEE_OperationMode mode)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_OperationHandle ecdsa_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_Attribute attr[4];
	uint32_t sign_len = 0;
	uint32_t max_key_size = 0;

	if (!digest || digest_len != SM3_HASH_SIZE || !signature || !signature_len
	    || !key)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("digest is 0x%x; signature is 0x%x; digest_len is %d; signature_len is %d",
	     *digest, *signature, digest_len, *signature_len);
	IMSG("key->d is 0x%x; key->x is 0x%x; key->y is 0x%x",
	     *key->d, *key->x, *key->y);
	IMSG("key->d_len is %d; key->x_len is %d; key->y_len is %d",
	     key->d_len, key->x_len, key->y_len);
	IMSG("key->curve is 0x%x; key->key_len is %d; algo is 0x%x; mode is 0x%x",
	     key->curve, key->key_len, algo, mode);
#endif

	if (key->curve != TEE_ECC_CURVE_SM2) {
		EMSG("curve = %08x != TEE_ECC_CURVE_SM2", key->curve);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (algo != TEE_ALG_SM2_DSA_SM3) {
		EMSG("algo = %08x != TEE_ALG_SM2_DSA_SM3", algo);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (mode != TEE_MODE_SIGN && mode != TEE_MODE_VERIFY)
		return TEE_ERROR_BAD_PARAMETERS;

	sign_len = *signature_len;
	max_key_size = key->key_len;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_ECC_PUBLIC_VALUE_X, key->x, key->x_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_ECC_PUBLIC_VALUE_Y, key->y, key->y_len);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_ECC_PRIVATE_VALUE, key->d, key->d_len);
	TEE_InitValueAttribute(&attr[3], TEE_ATTR_ECC_CURVE, key->curve, 0);

	res = TEE_AllocateTransientObject(TEE_TYPE_SM2_DSA_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		return res;
	}

	res = TEE_PopulateTransientObject(obj, attr, 4);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&ecdsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(ecdsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	if (mode == TEE_MODE_SIGN) {
		res = TEE_AsymmetricSignDigest(ecdsa_op, NULL, 0, digest, digest_len, signature,
					       &sign_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricSignDigest failed with code 0x%x", res);
			goto exit;
		}
		*signature_len = sign_len;
	} else {
		res = TEE_AsymmetricVerifyDigest(ecdsa_op, NULL, 0, digest, digest_len,
						 signature, sign_len);
		if (res != TEE_SUCCESS) {
			if (res == TEE_ERROR_SIGNATURE_INVALID) {
				EMSG("Verify failed!!!The signature is invalid!!!");
			} else {
				EMSG("TEE_AsymmetricVerifyDigest failed with code 0x%x", res);
			}
		}
	}

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (ecdsa_op)
		TEE_FreeOperation(ecdsa_op);

	return res;
}

/* eph means ephemeral public key */
TEE_Result rk_sm2_kep_genkey(rk_sm2_kep_parms *kep_parms, uint8_t *share_key,
			     uint32_t share_key_len, uint8_t *conf_out)
{
	TEE_Result res = TEE_SUCCESS;
	TEE_OperationHandle derive_op = NULL;
	TEE_ObjectHandle keyA = NULL, eph_keyA = NULL;
	TEE_ObjectHandle sv_handle = NULL;
	TEE_Attribute params[9];
	ec_key_t *my_key, *my_eph_key;
	ec_pub_key_t *pub_B, *eph_pub_B;
	uint32_t param_count = 0;
	uint32_t algo = TEE_ALG_SM2_KEP;
	uint32_t curve = TEE_ECC_CURVE_SM2;
	uint32_t keysize = 256;

	if (!kep_parms || !share_key)
		return TEE_ERROR_BAD_PARAMETERS;

	my_key = &kep_parms->my_key;
	my_eph_key = &kep_parms->my_eph_key;
	pub_B = &kep_parms->pub_B;
	eph_pub_B = &kep_parms->eph_pub_B;

	res = TEE_AllocateOperation(&derive_op, algo, TEE_MODE_DERIVE, keysize * 2);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	/* set keyA */
	res = TEE_AllocateTransientObject(TEE_TYPE_SM2_KEP_KEYPAIR, keysize, &keyA);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject keyA failed with code 0x%x", res);
		goto exit;
	}

	param_count = 0;

	TEE_InitValueAttribute(&params[param_count], TEE_ATTR_ECC_CURVE, curve, 0);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PRIVATE_VALUE,
			     my_key->d, my_key->d_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_X,
			     my_key->x, my_key->x_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_Y,
			     my_key->y, my_key->y_len);
	param_count++;

	res = TEE_PopulateTransientObject(keyA, params, param_count);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject keyA failed with code 0x%x", res);
		goto exit;
	}

	/* set eph keyA */
	res = TEE_AllocateTransientObject(TEE_TYPE_SM2_KEP_KEYPAIR, keysize, &eph_keyA);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject eph_keyA failed with code 0x%x", res);
		goto exit;
	}

	param_count = 0;

	TEE_InitValueAttribute(&params[param_count], TEE_ATTR_ECC_CURVE, curve, 0);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PRIVATE_VALUE,
			     my_eph_key->d, my_eph_key->d_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_X,
			     my_eph_key->x, my_eph_key->x_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_Y,
			     my_eph_key->y, my_eph_key->y_len);
	param_count++;

	res = TEE_PopulateTransientObject(eph_keyA, params, param_count);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject eph_keyA failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey2(derive_op, keyA, eph_keyA);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	TEE_FreeTransientObject(keyA);
	keyA = NULL;
	TEE_FreeTransientObject(eph_keyA);
	eph_keyA = NULL;

	res = TEE_AllocateTransientObject(TEE_TYPE_GENERIC_SECRET, share_key_len,
					  &sv_handle);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}
	params[0].attributeID = TEE_ATTR_SM2_KEP_USER;
	params[0].content.value.a = !kep_parms->is_initiator;
	params[0].content.value.b = 0; 		/* Not used */
	param_count = 1;

	TEE_InitRefAttribute(&params[param_count],
			     TEE_ATTR_ECC_PUBLIC_VALUE_X,
			     pub_B->x, pub_B->x_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count],
			     TEE_ATTR_ECC_PUBLIC_VALUE_Y,
			     pub_B->y, pub_B->y_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count],
			     TEE_ATTR_ECC_EPHEMERAL_PUBLIC_VALUE_X,
			     eph_pub_B->x, eph_pub_B->x_len);
	param_count++;

	TEE_InitRefAttribute(&params[param_count],
			     TEE_ATTR_ECC_EPHEMERAL_PUBLIC_VALUE_Y,
			     eph_pub_B->y, eph_pub_B->y_len);
	param_count++;

	if (kep_parms->id_a_len) {
		TEE_InitRefAttribute(&params[param_count],
				     TEE_ATTR_SM2_ID_INITIATOR,
				     kep_parms->id_a, kep_parms->id_a_len);
		param_count++;
	}

	if (kep_parms->id_b_len) {
		TEE_InitRefAttribute(&params[param_count],
				     TEE_ATTR_SM2_ID_RESPONDER,
				     kep_parms->id_b, kep_parms->id_b_len);
		param_count++;
	}

	if (kep_parms->conf_in_len) {
		TEE_InitRefAttribute(&params[param_count],
				     TEE_ATTR_SM2_KEP_CONFIRMATION_IN,
				     kep_parms->conf_in, kep_parms->conf_in_len);
		param_count++;
	}

	if (conf_out) {
		TEE_InitRefAttribute(&params[param_count],
				     TEE_ATTR_SM2_KEP_CONFIRMATION_OUT,
				     conf_out, SM3_HASH_SIZE);
		param_count++;
	}

	TEE_DeriveKey(derive_op, params, param_count, sv_handle);

	res = TEE_GetObjectBufferAttribute(sv_handle, TEE_ATTR_SECRET_VALUE, share_key,
					   &share_key_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute failed with code 0x%x", res);
		goto exit;
	}

exit:
	if (keyA)
		TEE_FreeTransientObject(keyA);
	if (eph_keyA)
		TEE_FreeTransientObject(eph_keyA);
	if (sv_handle)
		TEE_FreeTransientObject(sv_handle);
	if (derive_op)
		TEE_FreeOperation(derive_op);

	return res;
}

TEE_Result rk_ecdsa_begin(crypto_ctx_t *ctx, ec_key_t *key, uint32_t algo,
			  TEE_OperationMode mode)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_OperationHandle ecdsa_op = NULL;
	TEE_ObjectHandle obj = NULL;
	TEE_Attribute attr[4];
	uint32_t max_key_size = 0;

	if (!key || !ctx)
		return TEE_ERROR_BAD_PARAMETERS;

	if (mode != TEE_MODE_SIGN && mode != TEE_MODE_VERIFY)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_ECDSA)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("key->d is 0x%x; key->x is 0x%x; key->y is 0x%x",
	     *key->d, *key->x, *key->y);
	IMSG("key->d_len is %d; key->x_len is %d; key->y_len is %d",
	     key->d_len, key->x_len, key->y_len);
	IMSG("key->curve is 0x%x; key->key_len is %d; algo is 0x%x; mode is 0x%x",
	     key->curve, key->key_len, algo, mode);
#endif

	max_key_size = key->key_len;

	TEE_InitRefAttribute(&attr[0], TEE_ATTR_ECC_PUBLIC_VALUE_X, key->x, key->x_len);
	TEE_InitRefAttribute(&attr[1], TEE_ATTR_ECC_PUBLIC_VALUE_Y, key->y, key->y_len);
	TEE_InitRefAttribute(&attr[2], TEE_ATTR_ECC_PRIVATE_VALUE, key->d, key->d_len);
	TEE_InitValueAttribute(&attr[3], TEE_ATTR_ECC_CURVE, key->curve, 0);

	res = TEE_AllocateTransientObject(TEE_TYPE_ECDSA_KEYPAIR, max_key_size, &obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(obj, attr, 4);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&ecdsa_op, algo, mode, max_key_size);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(ecdsa_op, obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	ctx->obj = obj;
	ctx->op = ecdsa_op;
	ctx->algo = algo;
	ctx->mode = mode;

	return TEE_SUCCESS;

exit:
	if (obj)
		TEE_FreeTransientObject(obj);
	if (ecdsa_op)
		TEE_FreeOperation(ecdsa_op);

	return res;
}

TEE_Result rk_ecdsa_finish(crypto_ctx_t *ctx, uint8_t *digest, uint8_t *sign,
			   uint32_t digest_len, uint32_t *sign_len)
{
	TEE_Result res = 0;

	if (ctx == NULL ||
	    (digest == NULL && digest_len != 0) ||
	    sign_len == NULL ||
	    (sign == NULL && *sign_len != 0))
		return TEE_ERROR_BAD_PARAMETERS;

	if (ctx->obj == NULL || ctx->op == NULL)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ctx->obj: 0x%x;ctx->op: 0x%x;ctx->algo: 0x%x",
	     (uint32_t)ctx->obj, (uint32_t)ctx->op, ctx->algo);
#endif

	switch (ctx->mode) {
	case TEE_MODE_SIGN:
		res = TEE_AsymmetricSignDigest(ctx->op, NULL, 0, digest, digest_len, sign,
					       sign_len);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_AsymmetricSignDigest failed with code 0x%x", res);
			return res;
		}
		break;
	case TEE_MODE_VERIFY:
		res = TEE_AsymmetricVerifyDigest(ctx->op, NULL, 0, digest, digest_len, sign,
						 *sign_len);
		if (res != TEE_SUCCESS) {
			if (res == TEE_ERROR_SIGNATURE_INVALID) {
				EMSG("Verify failed!!!The asn1_sign is invalid!!!");
			} else {
				EMSG("TEE_AsymmetricVerifyDigest failed with code 0x%x", res);
			}
			return res;
		}
		break;
	default:
		EMSG("Unknown mode: 0x%x", ctx->mode);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

TEE_Result rk_pkcs5_pbkdf2_hmac(uint8_t *password, uint32_t password_len,
				uint8_t *salt, uint32_t salt_len, uint32_t iterations,
				uint32_t algo, uint32_t key_len, uint8_t *out_key)
{
	TEE_Result res = 0;
	TEE_OperationHandle derive_op = NULL;
	TEE_ObjectHandle password_obj = NULL;
	TEE_ObjectHandle key_obj = NULL;
	TEE_Attribute params[4];
	uint32_t param_count = 0;
	uint32_t out_len = 0;

	if (out_key == NULL && key_len != 0)
		return TEE_ERROR_BAD_PARAMETERS;
	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_PBKDF2)
		return TEE_ERROR_BAD_PARAMETERS;
	if (password_len > 512)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("password_len is %d; salt_len is %d; iterations is %d; key_len is %d",
	     password_len, salt_len, iterations, key_len);
#endif

	TEE_InitRefAttribute(params, TEE_ATTR_PBKDF2_PASSWORD, password, password_len);
	param_count = 1;

	res = TEE_AllocateTransientObject(TEE_TYPE_PBKDF2_PASSWORD, password_len * 8,
					  &password_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(password_obj, params, param_count);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&derive_op, algo, TEE_MODE_DERIVE,
				    password_len * 8);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(derive_op, password_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateTransientObject(TEE_TYPE_GENERIC_SECRET, key_len * 8,
					  &key_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}
	param_count = 0;

	if (salt) {
		TEE_InitRefAttribute(&params[param_count], TEE_ATTR_PBKDF2_SALT, salt,
				     salt_len);
		param_count++;
	}
	TEE_InitValueAttribute(&params[param_count], TEE_ATTR_PBKDF2_DKM_LENGTH,
			       key_len, 0);
	param_count++;
	TEE_InitValueAttribute(&params[param_count], TEE_ATTR_PBKDF2_ITERATION_COUNT,
			       iterations, 0);
	param_count++;

	TEE_DeriveKey(derive_op, params, param_count, key_obj);

	out_len = key_len;
	res = TEE_GetObjectBufferAttribute(key_obj, TEE_ATTR_SECRET_VALUE, out_key,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_n failed with code 0x%x", res);
		goto exit;
	}

exit:
	if (password_obj)
		TEE_FreeTransientObject(password_obj);
	if (key_obj)
		TEE_FreeTransientObject(key_obj);
	if (derive_op)
		TEE_FreeOperation(derive_op);

	return res;
}

TEE_Result rk_hkdf_genkey(uint8_t *ikm, uint32_t ikm_len,
			  uint8_t *salt, uint32_t salt_len, uint32_t *info, uint32_t info_len,
			  uint32_t algo, uint32_t okm_len, uint8_t *okm)
{
	TEE_Result res = 0;
	TEE_OperationHandle derive_op = NULL;
	TEE_ObjectHandle password_obj = NULL;
	TEE_ObjectHandle key_obj = NULL;
	TEE_Attribute params[4];
	uint32_t param_count = 0;
	uint32_t out_len = 0;

	if (okm == NULL && okm_len != 0)
		return TEE_ERROR_BAD_PARAMETERS;
	if (ikm == NULL && ikm_len != 0)
		return TEE_ERROR_BAD_PARAMETERS;
	if (salt == NULL && salt_len != 0)
		return TEE_ERROR_BAD_PARAMETERS;
	if (info == NULL && info_len != 0)
		return TEE_ERROR_BAD_PARAMETERS;
	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_HKDF)
		return TEE_ERROR_BAD_PARAMETERS;
	if (ikm_len > 512)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("ikm_len is %d; salt_len is %d; okm_len is %d;  info_len is %d; algo is 0x%x",
	     ikm_len, salt_len, okm_len, info_len, algo);
#endif

	TEE_InitRefAttribute(params, TEE_ATTR_HKDF_IKM, ikm, ikm_len);
	param_count = 1;

	res = TEE_AllocateTransientObject(TEE_TYPE_HKDF_IKM, ikm_len * 8,
					  &password_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_PopulateTransientObject(password_obj, params, param_count);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateOperation(&derive_op, algo, TEE_MODE_DERIVE, ikm_len * 8);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(derive_op, password_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateTransientObject(TEE_TYPE_GENERIC_SECRET, okm_len * 8,
					  &key_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}
	param_count = 0;

	if (salt) {
		TEE_InitRefAttribute(&params[param_count], TEE_ATTR_HKDF_SALT, salt, salt_len);
		param_count++;
	}

	TEE_InitValueAttribute(&params[param_count], TEE_ATTR_HKDF_OKM_LENGTH, okm_len,
			       0);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_HKDF_INFO, info, info_len);
	param_count++;

	TEE_DeriveKey(derive_op, params, param_count, key_obj);

	out_len = okm_len;
	res = TEE_GetObjectBufferAttribute(key_obj, TEE_ATTR_SECRET_VALUE, okm,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute for RSA_n failed with code 0x%x", res);
		goto exit;
	}

exit:
	if (password_obj)
		TEE_FreeTransientObject(password_obj);
	if (key_obj)
		TEE_FreeTransientObject(key_obj);
	if (derive_op)
		TEE_FreeOperation(derive_op);

	return res;
}

TEE_Result rk_ecdh_genkey(uint8_t *private, uint8_t *publicx, uint32_t *publicy,
			  uint32_t algo,  uint32_t curve, uint32_t keysize, uint8_t *out)
{
	TEE_Result res = 0;
	TEE_OperationHandle derive_op = NULL;
	TEE_ObjectHandle password_obj = NULL;
	TEE_ObjectHandle key_obj = NULL;
	TEE_Attribute params[4];
	uint32_t param_count = 0;
	uint32_t out_len = 0;
	uint32_t size_byte = (keysize + 7) / 8;

	if (private == NULL || publicx == NULL || publicy == NULL)
		return TEE_ERROR_BAD_PARAMETERS;
	if (TEE_ALG_GET_MAIN_ALG(algo) != TEE_MAIN_ALGO_ECDH)
		return TEE_ERROR_BAD_PARAMETERS;
	if (keysize != 192 && keysize != 224 && keysize != 256 &&
	    keysize != 384 && keysize != 521)
		return TEE_ERROR_BAD_PARAMETERS;

#if CRYPTO_DEBUG
	IMSG("algo is %d; curve is %d; keysize is %d", algo, curve, keysize);
#endif

	res = TEE_AllocateOperation(&derive_op, algo, TEE_MODE_DERIVE, keysize);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateOperation failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateTransientObject(TEE_TYPE_ECDH_KEYPAIR, keysize,
					  &password_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	TEE_InitValueAttribute(&params[param_count], TEE_ATTR_ECC_CURVE, curve, 0);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PRIVATE_VALUE, private,
			     size_byte);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_X, private,
			     size_byte);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_Y, private,
			     size_byte);
	param_count++;

	res = TEE_PopulateTransientObject(password_obj, params, param_count);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_PopulateTransientObject failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_SetOperationKey(derive_op, password_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_SetOperationKey failed with code 0x%x", res);
		goto exit;
	}

	res = TEE_AllocateTransientObject(TEE_TYPE_GENERIC_SECRET, size_byte * 8,
					  &key_obj);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocateTransientObject failed with code 0x%x", res);
		goto exit;
	}
	param_count = 0;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_X, publicx,
			     size_byte);
	param_count++;

	TEE_InitRefAttribute(&params[param_count], TEE_ATTR_ECC_PUBLIC_VALUE_Y, publicy,
			     size_byte);
	param_count++;

	TEE_DeriveKey(derive_op, params, param_count, key_obj);

	out_len = size_byte;
	res = TEE_GetObjectBufferAttribute(key_obj, TEE_ATTR_SECRET_VALUE, out,
					   &out_len);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetObjectBufferAttribute failed with code 0x%x", res);
		goto exit;
	}

exit:
	if (password_obj)
		TEE_FreeTransientObject(password_obj);
	if (key_obj)
		TEE_FreeTransientObject(key_obj);
	if (derive_op)
		TEE_FreeOperation(derive_op);

	return res;
}

