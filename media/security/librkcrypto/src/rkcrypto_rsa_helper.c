/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#include <stdlib.h>
#include <string.h>

#include "rkcrypto_core.h"
#include "rkcrypto_trace.h"
#include "rkcrypto_random.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#define RSA_PKCS1_TYPE_MIN_PAD_LEN	(11)

/**< Identifier for RSA signature operations. */
#define MBEDTLS_RSA_SIGN		1

/**< Identifier for RSA encryption and decryption operations. */
#define MBEDTLS_RSA_CRYPT		2

#define ASN1_INTEGER			((uint8_t)0x02)
#define ASN1_BIT_STRING			((uint8_t)0x03)
#define ASN1_OCT_STRING			((uint8_t)0x04)
#define ASN1_NULL			((uint8_t)0x05)
#define ASN1_OBJ_IDENTIFIER		((uint8_t)0x06)
#define ASN1_SEQUENCE			((uint8_t)0x30)
#define ASN1_CONTEXT0			((uint8_t)0xA0)
#define ASN1_CONTEXT1			((uint8_t)0xA1)

typedef struct {
	const uint8_t	*data;		//the buffer of data
	uint16_t	*data_len;	//valid length of data
	uint8_t		tag;		//ASN1 data type
	uint8_t		need_plus;	//to identify weather the data is a positive number
} asn1_object_t;

struct hash_oid_item {
	uint32_t	hash_algo;
	const uint8_t	*oid;
	uint32_t	oid_size;
};

static const uint8_t sha1_oid[]   = {0x2b, 0x0e, 0x03, 0x02, 0x1a};
static const uint8_t sha224_oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04};
static const uint8_t sha256_oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01};
static const uint8_t sha384_oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02};
static const uint8_t sha512_oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03};

struct hash_oid_item hash_oid_tbl[] = {
	{RK_ALGO_SHA1,   sha1_oid,   sizeof(sha1_oid)},
	{RK_ALGO_SHA224, sha224_oid, sizeof(sha224_oid)},
	{RK_ALGO_SHA256, sha256_oid, sizeof(sha256_oid)},
	{RK_ALGO_SHA384, sha384_oid, sizeof(sha384_oid)},
	{RK_ALGO_SHA512, sha512_oid, sizeof(sha512_oid)},
};

static RK_RES get_oid_by_md(uint32_t hash_algo, const uint8_t **oid, uint32_t *old_len)
{
	uint32_t i;

	for (i = 0; i < ARRAY_SIZE(hash_oid_tbl); i++) {
		if (hash_oid_tbl[i].hash_algo == hash_algo) {
			*oid     = hash_oid_tbl[i].oid;
			*old_len = hash_oid_tbl[i].oid_size;
			return RK_CRYPTO_SUCCESS;
		}
	}

	return RK_CRYPTO_ERR_PARAMETER;
}

static RK_RES asn1_compose_len(uint32_t len, uint8_t *field, uint32_t *field_len)
{
	uint8_t tmp_field[4];

	if (field == NULL || field_len == NULL)
		return RK_CRYPTO_ERR_PARAMETER;

	if (len < 0x80) {
		*field     = len;
		*field_len = 1;
	} else {
		uint32_t i, j;

		tmp_field[0] = (len >> 24) & 0xff;
		tmp_field[1] = (len >> 16) & 0xff;
		tmp_field[2] = (len >> 8) & 0xff;
		tmp_field[3] = len & 0xff;

		for (i = 0; i < sizeof(tmp_field); i++) {
			if (tmp_field[i] == 0x00)
				continue;

			for (j = 0; j < sizeof(tmp_field) - i; j++)
				field[j + 1] = tmp_field[j + i];

			break;
		}
		field[0]   = 0X80 + sizeof(tmp_field) - i;
		*field_len = sizeof(tmp_field) - i + 1;
	}

	return RK_CRYPTO_SUCCESS;
}

static RK_RES asn1_set_object(const uint8_t *in, uint32_t in_len, uint8_t tag, uint8_t need_plus,
			      uint8_t *out, uint32_t out_max, uint32_t *out_len)
{
	RK_RES res;
	uint8_t *pout = out;
	uint32_t field_len;
	uint8_t tmp_field[5];

	if (in == NULL || out == NULL || out_len == NULL)
		return RK_CRYPTO_ERR_PARAMETER;

	*out_len = 0;

	//padding tag field
	if (out_max < 1) {
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	*pout = tag;
	pout++;
	out_max--;

	//padding length field
	if (need_plus && *in >= 0x80)
		res = asn1_compose_len(in_len + 1, tmp_field, &field_len);
	else
		res = asn1_compose_len(in_len, tmp_field, &field_len);

	if (res != RK_CRYPTO_SUCCESS) {
		D_TRACE("asn1_compose_len error");
		return res;
	}

	if (out_max < field_len) {
		D_TRACE("out_max = %d, field_len = %d", out_max, field_len);
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memmove(pout, tmp_field, field_len);
	pout    += field_len;
	out_max -= field_len;

	//padding value field
	if (need_plus && *in >= 0x80) {
		if (out_max < 1) {
			res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
			goto exit;
		}

		*pout = 0x00;
		pout++;
		out_max--;
	}

	if (out_max < in_len) {
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memmove(pout, in, in_len);
	pout += in_len;

	*out_len = pout-out;

exit:
	return res;
}

/* PKCS #1: block type 0,1,2 message padding */
/*************************************************
 *EB = 00 || BT || PS || 00 || D
 *
 *PS_LEN >= 8, mlen < key_len - 11
 ************************************************/
static RK_RES rsa_padding_add_pkcs1_type(uint16_t key_len, uint8_t bt,
					 const uint8_t *in, uint8_t in_len, uint8_t *out)
{
	uint32_t plen = 0;
	uint8_t *peb = NULL;

	if (in_len > key_len - RSA_PKCS1_TYPE_MIN_PAD_LEN) {
		E_TRACE("key_len is invalid.\n");
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;
	}

	/* block type 0 first input data can't be zero */
	if (bt == 0x00 && *in == 0)
		return RK_CRYPTO_ERR_PADDING;

	peb = out;

	/* first byte is 0x00 */
	*(peb++) = 0;

	/* Private Key BT (Block Type) */
	*(peb++) = bt;

	/* The padding string PS shall consist of k-3-||D|| octets */
	plen = key_len - 3 - in_len;
	switch (bt) {
	case 0x00: {
		/* For block type 00, the octets shall have	value 00 */
		memset(peb, 0x00, plen);
		break;
	}
	case 0x01: {
		/* for block type 01, they shall have value	FF */
		memset(peb, 0xFF, plen);
		break;
	}
	case 0x02: {
		RK_RES res;
		uint32_t i = 0;

		/* for block type 02, they shall be pseudorandomly generated and nonzero. */
		res = rk_get_random(peb, plen);
		if (res)
			return res;

		/* make sure nonzero */
		for (i = 0; i < plen; i++) {
			if (peb[i] == 0x00)
				peb[i] = 0x01;
		}
		break;
	}
	default: {
		E_TRACE("BT(0x%x) is invalid.\n", plen);
		return RK_CRYPTO_ERR_PADDING;
	}
	}

	/* skip the padding string */
	peb += plen;

	/* set 0x00 follow PS */
	*(peb++) = 0x00;

	/* input data */
	memcpy(peb, in, in_len);

	return RK_CRYPTO_SUCCESS;
}

/* PKCS #1: block type 0,1,2 message padding */
/*************************************************
 * EB = 00 || BT || PS || 00 || D
 *
 *PS_LEN >= 8, mlen < key_len - 11
 *************************************************/
static RK_RES rsa_padding_check_pkcs1_type(uint32_t key_len, uint8_t bt,
					   const uint8_t *in, uint8_t *out, uint32_t *outlen)
{
	const uint8_t *peb = NULL;
	uint32_t inlen = key_len;

	*outlen = 0x00;
	peb     = in;

	/* first byte must be 0x00 */
	if (*peb != 0x00) {
		E_TRACE("EB[0] != 0x00.\n");
		goto error;
	}
	peb++;

	/* Private Key BT (Block Type) */
	if (*peb != bt) {
		E_TRACE("EB[1] != BT(0x%x).\n",	bt);
		goto error;
	}
	peb++;

	switch (bt) {
	case 0x00:
		/* For block type 00, the octets shall have value 00 */
		for (; peb < in + inlen - 1; peb++) {
			if ((*peb == 0x00) && (*(peb + 1) != 0))
				break;
		}
		break;
	case 0x01:
		/* For block type 0x01 the octets shall have value 0xFF */
		for (; peb < in + inlen - 1; peb++) {
			if (*peb == 0xFF)
				continue;

			if (*peb == 0x00)
				break;

			peb = in + inlen - 1;
			break;
		}
		break;
	case 0x02:
		/* for block type 02, they shall be pseudorandomly generated and nonzero. */
		for (; peb < in + inlen - 1; peb++) {
			if (*peb == 0x00)
				break;
		}
		break;
	default:
		E_TRACE("BT(0x%x) is invalid.\n", bt);
		goto error;
	}

	if (peb >= (in + inlen - 1)) {
		E_TRACE("PS Error.\n");
		goto error;
	}

	/* skip 0x00 after PS */
	peb++;

	/* get payload data */
	*outlen = in + key_len - peb;
	memcpy(out, peb, *outlen);

	return RK_CRYPTO_SUCCESS;
error:
	return RK_CRYPTO_ERR_PADDING;
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-PKCS1-V1_5-ENCRYPT function
 * 00 (01/02) [a bunch of non-zero random bytes] 00 [the message]
 */
RK_RES rsa_padding_add_pkcs15_type(uint16_t key_len, bool is_priv_key,
				  const uint8_t *in, uint32_t in_len, uint8_t *out)
{
	uint32_t nb_pad, olen;
	RK_RES res;
	uint8_t *p = out;

	// We don't check p_rng because it won't be dereferenced here
	if (in == NULL || out == NULL)
		return RK_CRYPTO_ERR_PADDING;

	olen = key_len;

	/* first comparison checks for overflow */
	if (in_len + 11 < in_len || olen < in_len + 11)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;

	nb_pad = olen - 3 - in_len;

	*p++ = 0;
	if (!is_priv_key) {
		*p++ = MBEDTLS_RSA_CRYPT;

		while (nb_pad-- > 0) {
			int rng_dl = 100;

			do {
				res = rk_get_random(p, 1);
			} while (*p == 0 && --rng_dl && res == 0);

			/* Check if RNG failed to generate data */
			if (rng_dl == 0 || res != 0)
				return RK_CRYPTO_ERR_PADDING;

			p++;
		}
	} else {
		*p++ = MBEDTLS_RSA_SIGN;

		while (nb_pad-- > 0)
			*p++ = 0xFF;
	}

	*p++ = 0;
	memcpy(p, in, in_len);

	return RK_CRYPTO_SUCCESS;
}
/*
 * Implementation of the PKCS#1 v2.1 RSAES-PKCS1-V1_5-DECRYPT function
 */
static RK_RES rsa_padding_check_pkcs15_type(uint16_t key_len, bool is_priv_key,
					   const uint8_t *in, uint8_t *out, uint32_t *out_len)
{
	uint32_t ilen, pad_count = 0, i;
	const uint8_t *p;
	uint8_t bad, pad_done = 0;

	ilen = key_len;
	p    = in;
	bad  = 0;

	/*
	 * Check and get padding len in "constant-time"
	 */
	bad |= *p++; /* First byte must be 0 */

	/* This test does not depend on secret data */
	if (is_priv_key) {
		bad |= *p++ ^ MBEDTLS_RSA_CRYPT;

		/* Get padding len, but always read till end of buffer
		 * (minus one, for the 00 byte)
		 */
		for (i = 0; i < ilen - 3; i++) {
			pad_done  |= ((p[i] | (uint8_t)-p[i]) >> 7) ^ 1;
			pad_count += ((pad_done | (uint8_t)-pad_done) >> 7) ^ 1;
		}

		p += pad_count;
		bad |= *p++; /* Must be zero */
	} else {
		bad |= *p++ ^ MBEDTLS_RSA_SIGN;

		/* Get padding len, but always read till end of buffer
		 * (minus one, for the 00 byte)
		 */
		for (i = 0; i < ilen - 3; i++) {
			pad_done |= (p[i] != 0xFF);
			pad_count += (pad_done == 0);
		}

		p   += pad_count;
		bad |= *p++; /* Must be zero */
	}

	bad |= (pad_count < 8);

	if (bad)
		return RK_CRYPTO_ERR_PADDING;

	*out_len = ilen - (p - in);
	memcpy(out, p, *out_len);

	return RK_CRYPTO_SUCCESS;
}

static RK_RES get_hash_algo_from_padding(uint32_t padding,
				       uint32_t *hlen, uint32_t *hash_algo)
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

static RK_RES calc_padding_digest(uint32_t algo, const uint8_t *data, uint32_t data_len,
				  uint8_t *digest)
{
	RK_RES res;
	rk_hash_config hash_cfg;
	rk_handle hash_hdl = 0;

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));

	hash_cfg.algo = algo;

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

/**
 * Generate and apply the MGF1 operation (from PKCS#1 v2.1) to a buffer.
 *
 * \param dst       buffer to mask
 * \param dlen      length of destination buffer
 * \param src       source of the mask generation
 * \param slen      length of the source buffer
 * \param md_ctx    message digest context to use
 */
static RK_RES mgf_mask(uint8_t *dst, uint32_t dlen, uint8_t *src, uint32_t slen,
		       uint32_t hash_algo, uint32_t hash_len)
{
	uint8_t mask[SHA512_HASH_SIZE];
	uint8_t counter[4];
	uint8_t *p;
	uint32_t i, use_len;
	RK_RES res = 0;
	rk_handle hash_hdl;
	rk_hash_config hash_cfg;


	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = hash_algo;

	memset(mask, 0, sizeof(mask));
	memset(counter, 0, sizeof(counter));

	/* Generate and apply dbMask */
	p = dst;

	while (dlen > 0) {
		use_len = hash_len;
		if (dlen < hash_len)
			use_len = dlen;

		res = rk_hash_init(&hash_cfg, &hash_hdl);
		if (res)
			goto cleanup;

		res = rk_hash_update_virt(hash_hdl, src, slen);
		if (res) {
			rk_hash_final(hash_hdl, NULL);
			goto cleanup;
		}

		res = rk_hash_update_virt(hash_hdl, counter, 4);
		if (res) {
			rk_hash_final(hash_hdl, NULL);
			goto cleanup;
		}

		res = rk_hash_final(hash_hdl, mask);
		if (res)
			goto cleanup;

		for (i = 0; i < use_len; ++i)
			*p++ ^= mask[i];

		counter[3]++;

		dlen -= use_len;
	}

cleanup:
	memset(mask, 0x00, sizeof(mask));

	return res;
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-OAEP-ENCRYPT function
 */
static RK_RES rsa_padding_add_oaep_type(enum RK_RSA_CRYPT_PADDING padding, uint16_t key_len,
					const uint8_t *label, uint32_t label_len,
					const uint8_t *in, uint32_t in_len, uint8_t *out)
{
	uint32_t olen;
	RK_RES res;
	uint8_t *p = out;
	uint32_t hlen;
	uint32_t hash_algo;

	RK_CRYPTO_CHECK_PARAM(!out);
	RK_CRYPTO_CHECK_PARAM(!in);
	RK_CRYPTO_CHECK_PARAM(label_len != 0 || label);

	olen = key_len;
	res = get_hash_algo_from_padding(padding, &hlen, &hash_algo);
	if (res)
		goto error;

	/* first comparison checks for overflow */
	if (in_len + 2 * hlen + 2 < in_len || olen < in_len + 2 * hlen + 2) {
		res = RK_CRYPTO_ERR_PADDING_OVERFLOW;
		goto error;
	}

	memset(out, 0, olen);

	*p++ = 0;

	/* Generate a random octet string seed */
	res = rk_get_random(p, hlen);
	if (res)
		goto error;

	p += hlen;

	/* Construct DB */
	res = calc_padding_digest(hash_algo, label, label_len, p);
	if (res)
		goto error;

	p += hlen;
	p += olen - 2 * hlen - 2 - in_len;
	*p++ = 1;
	memcpy(p, in, in_len);

	/* maskedDB: Apply dbMask to DB */
	res = mgf_mask(out + hlen + 1, olen - hlen - 1, out + 1, hlen, hash_algo, hlen);
	if (res)
		goto error;

	/* maskedSeed: Apply seedMask to seed */
	res = mgf_mask(out + 1, hlen, out + hlen + 1, olen - hlen - 1, hash_algo, hlen);
	if (res)
		goto error;

	return RK_CRYPTO_SUCCESS;

error:
	return res;
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-OAEP-DECRYPT function
 */
RK_RES rsa_padding_check_oaep_type(enum RK_RSA_CRYPT_PADDING padding, uint16_t key_len,
				   const uint8_t *label, size_t label_len,
				   const uint8_t *in, uint8_t *out, uint32_t *out_len)
{
	RK_RES res = RK_CRYPTO_ERR_PADDING;
	uint32_t ilen, i, pad_len;
	uint8_t *p, bad, pad_done;
	uint8_t *buf = NULL;
	uint8_t lhash[SHA512_HASH_SIZE];
	uint32_t hlen;
	uint32_t hash_algo;

	ilen = key_len;
	res = get_hash_algo_from_padding(padding, &hlen, &hash_algo);
	if (res)
		return res;

	buf = malloc(ilen);
	if (!buf)
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;

	memcpy(buf, in, ilen);

	// checking for integer underflow
	if (2 * hlen + 2 > ilen) {
		res = RK_CRYPTO_ERR_PADDING;
		goto exit;
	}

	/*
	 * Unmask data and generate lHash
	 */
	res = calc_padding_digest(hash_algo, label, label_len, lhash);
	if (res)
		goto exit;

	/* seed: Apply seedMask to maskedSeed */
	res = mgf_mask(buf + 1, hlen, buf + hlen + 1, ilen - hlen - 1, hash_algo, hlen);
	if (res)
		goto exit;

	/* DB: Apply dbMask to maskedDB */
	res = mgf_mask(buf + hlen + 1, ilen - hlen - 1, buf + 1, hlen, hash_algo, hlen);
	if (res)
		goto exit;

	/*
	 * Check contents, in "constant-time"
	 */
	p = buf;
	bad = 0;

	bad |= *p++; /* First byte must be 0 */

	p += hlen; /* Skip seed */

	/* Check lHash */
	for (i = 0; i < hlen; i++)
		bad |= lhash[i] ^ *p++;

	/* Get zero-padding len, but always read till end of buffer
	 * (minus one, for the 01 byte)
	 */
	pad_len = 0;
	pad_done = 0;
	for (i = 0; i < ilen - 2 * hlen - 2; i++) {
		pad_done |= p[i];
		pad_len += ((pad_done | (uint8_t)-pad_done) >> 7) ^ 1;
	}

	p += pad_len;
	bad |= *p++ ^ 0x01;

	/*
	 * The only information "leaked" is whether the padding was correct or not
	 * (eg, no data is copied if it was not correct). This meets the
	 * recommendations in PKCS#1 v2.2: an opponent cannot distinguish between
	 * the different error conditions.
	 */
	if (bad != 0) {
		res = RK_CRYPTO_ERR_PADDING;
		goto exit;
	}

	if (ilen - (p - buf) > key_len) {
		res = RK_CRYPTO_ERR_PADDING;
		goto exit;
	}

	*out_len = ilen - (p - buf);
	memcpy(out, p, *out_len);

	res = RK_CRYPTO_SUCCESS;
exit:
	if (buf)
		free(buf);

	return res;
}

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PKCS1-V1_5-SIGN function
 * Construct a PKCS v1.5 encoding of a hashed message
 */
static int rsa_padding_add_pkcs15_sign_type(uint32_t hash_algo, uint16_t key_len,
					    const uint8_t *hash, uint8_t hash_len, uint8_t *out)
{
	RK_RES res;
	uint32_t oid_size  = 0;
	uint32_t nb_pad    = key_len;
	uint8_t *p = out;
	const uint8_t *oid  = NULL;

	/* Gather length of hash to sign */
	res = get_oid_by_md(hash_algo, &oid, &oid_size);
	if (res)
		return RK_CRYPTO_ERR_PADDING;

	/* Double-check that 8 + hash_len + oid_size can be used as a
	 * 1-byte ASN.1 length encoding and that there's no overflow.
	 */
	if (8 + hash_len + oid_size >= 0x80 ||
	    10 + hash_len < hash_len ||
	    10 + hash_len + oid_size < 10 + hash_len)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;

	/*
	 * Static bounds check:
	 * - Need 10 bytes for five tag-length pairs.
	 *   (Insist on 1-byte length encodings to protect against variants of
	 *    Bleichenbacher's forgery attack against lax PKCS#1v1.5 verification)
	 * - Need hash_len bytes for hash
	 * - Need oid_size bytes for hash alg OID.
	 */
	if (nb_pad < 10 + hash_len + oid_size)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;

	nb_pad -= 10 + hash_len + oid_size;

	/* Need space for signature header and padding delimiter (3 bytes),
	 * and 8 bytes for the minimal padding
	 */
	if (nb_pad < 3 + 8)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;

	nb_pad -= 3;

	/* Now nb_pad is the amount of memory to be filled
	 * with padding, and at least 8 bytes long.
	 */

	/* Write signature header and padding */
	*p++ = 0;
	*p++ = MBEDTLS_RSA_SIGN;
	memset(p, 0xFF, nb_pad);
	p += nb_pad;
	*p++ = 0;

	/* Signing hashed data, add corresponding ASN.1 structure
	 *
	 * DigestInfo ::= SEQUENCE {
	 *	 digestAlgorithm DigestAlgorithmIdentifier,
	 *	 digest Digest }
	 * DigestAlgorithmIdentifier ::= AlgorithmIdentifier
	 * Digest ::= OCTET STRING
	 *
	 * Schematic:
	 * TAG-SEQ + LEN [ TAG-SEQ + LEN [ TAG-OID + LEN [ OID  ]
	 *                                 TAG-NULL + LEN [ NULL ] ]
	 *                 TAG-OCTET + LEN [ HASH ] ]
	 */
	*p++ = ASN1_SEQUENCE;
	*p++ = (uint8_t)(0x08 + oid_size + hash_len);
	*p++ = ASN1_SEQUENCE;
	*p++ = (uint8_t)(0x04 + oid_size);
	*p++ = ASN1_OBJ_IDENTIFIER;
	*p++ = (uint8_t)oid_size;
	memcpy(p, oid, oid_size);
	p += oid_size;
	*p++ = ASN1_NULL;
	*p++ = 0x00;
	*p++ = ASN1_OCT_STRING;
	*p++ = (uint8_t)hash_len;
	memcpy(p, hash, hash_len);
	p += hash_len;

	return RK_CRYPTO_SUCCESS;
}

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PSS-SIGN function
 */
RK_RES rsa_padding_add_pss_type(uint16_t key_len, uint16_t n_bits,
				uint32_t hash_algo, const uint8_t *hash,
				uint32_t hash_len, uint8_t *out)
{
	uint32_t olen;
	uint8_t *p = out;
	uint8_t salt[SHA512_HASH_SIZE];
	uint32_t slen, hlen, min_slen, offset = 0;
	RK_RES res;
	uint32_t msb, ps_len;
	rk_hash_config hash_cfg;
	rk_handle hash_hdl;

	olen = key_len;
	hlen = hash_len;
	slen = hlen;

	/* Calculate the largest possible salt length. Normally this is the hash
	 * length, which is the maximum length the salt can have. If there is not
	 * enough room, use the maximum salt length that fits. The constraint is
	 * that the hash length plus the salt length plus 2 bytes must be at most
	 * the key length. This complies with FIPS 186-4.5.5 (e) and RFC 8017
	 * (PKCS#1 v2.2) 9.1.1 step 3.
	 */
	min_slen = hlen - 2;
	if (olen < hlen + min_slen + 2)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;
	else if (olen >= hlen + hlen + 2)
		slen = hlen;
	else
		slen = olen - hlen - 2;

	if (olen < hlen * 2 + 2)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;

	ps_len = olen - hlen * 2 - 2;

	if (olen < ps_len + 1 + slen + hlen)
		return RK_CRYPTO_ERR_PADDING_OVERFLOW;

	memset(out, 0, olen);

	/* Generate salt of length slen */
	res = rk_get_random(salt, slen);
	if (res)
		return res;

	/* Note: EMSA-PSS encoding is over the length of N - 1 bits */
	msb = n_bits - 1;
	p += ps_len;
	*p++ = 0x01;
	memcpy(p, salt, slen);
	p += slen;

	/* Generate H = Hash( M' ) */
	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = hash_algo;

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res)
		goto exit;

	res = rk_hash_update_virt(hash_hdl, p, 8);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		goto exit;
	}

	res = rk_hash_update_virt(hash_hdl, hash, hash_len);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		goto exit;
	}

	res = rk_hash_update_virt(hash_hdl, salt, slen);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		goto exit;
	}

	res = rk_hash_final(hash_hdl, p);
	if (res)
		goto exit;

	/* Compensate for boundary condition when applying mask */
	if (msb % 8 == 0)
		offset = 1;

	/* maskedDB: Apply dbMask to DB */
	res = mgf_mask(out + offset, olen - hlen - 1 - offset, p, hlen, hash_algo, hash_len);
	if (res)
		goto exit;

	msb = n_bits - 1;
	out[0] &= 0xFF >> (olen * 8 - msb);

	p += hlen;
	*p++ = 0xBC;

exit:
	return res;
}

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PSS-VERIFY function
 */
RK_RES rk_rsa_padding_check_pss_type(uint16_t key_len, uint16_t n_bits,
				     uint32_t hash_algo, uint32_t hash_len,
				     const uint8_t *hash, const uint8_t *dec)
{
	RK_RES res = RK_CRYPTO_ERR_PADDING;
	uint32_t siglen;
	uint8_t *p;
	uint8_t *buf = NULL;
	uint8_t result[SHA512_HASH_SIZE];
	uint8_t zeros[8];
	uint32_t hlen;
	uint32_t slen, msb;
	rk_hash_config hash_cfg;
	rk_handle hash_hdl;

	buf = malloc(key_len);
	if (!buf)
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;

	memcpy(buf, dec, key_len);

	siglen = key_len;

	if (siglen < 16) {
		res = RK_CRYPTO_ERR_PADDING;
		goto error;
	}

	p = buf;

	if (buf[siglen - 1] != 0xBC) {
		res = RK_CRYPTO_ERR_PADDING;
		goto error;
	}

	hlen = hash_len;
	slen = siglen - hlen - 1; /* Currently length of salt + padding */

	memset(zeros, 0, 8);

	/*
	 * Note: EMSA-PSS verification is over the length of N - 1 bits
	 */
	msb = n_bits - 1;

	/* Compensate for boundary condition when applying mask */
	if (msb % 8 == 0) {
		p++;
		siglen -= 1;
	}
	if (buf[0] >> (8 - siglen * 8 + msb)) {
		res = RK_CRYPTO_ERR_PADDING;
		goto error;
	}

	res = mgf_mask(p, siglen - hlen - 1, p + siglen - hlen - 1, hlen, hash_algo, hash_len);
	if (res)
		goto error;

	buf[0] &= 0xFF >> (siglen * 8 - msb);

	while (p < buf + siglen && *p == 0)
		p++;

	if (p == buf + siglen || *p++ != 0x01) {
		res = RK_CRYPTO_ERR_PADDING;
		goto error;
	}

	/* Actual salt len */
	slen -= p - buf;

	/*
	 * Generate H = Hash( M' )
	 */

	memset(&hash_cfg, 0x00, sizeof(hash_cfg));
	hash_cfg.algo = hash_algo;

	res = rk_hash_init(&hash_cfg, &hash_hdl);
	if (res)
		goto error;

	res = rk_hash_update_virt(hash_hdl, zeros, 8);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		goto error;
	}

	res = rk_hash_update_virt(hash_hdl, hash, hash_len);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		goto error;
	}

	res = rk_hash_update_virt(hash_hdl, p, slen);
	if (res) {
		rk_hash_final(hash_hdl, NULL);
		goto error;
	}

	res = rk_hash_final(hash_hdl, result);
	if (res)
		goto error;

	free(buf);

	if (memcmp(p + slen, result, hlen) == 0)
		return RK_CRYPTO_SUCCESS;
	else
		return RK_CRYPTO_ERR_VERIFY;

error:
	if (buf)
		free(buf);

	return RK_CRYPTO_ERR_PADDING;
}

RK_RES rk_rsa_pubkey_encode(rk_rsa_pub_key_pack *pub,
			    uint8_t *asn1_key, uint16_t *asn1_key_len, uint16_t *key_bits)
{
	RK_RES res;
	rk_rsa_pub_key *rsa_key = &pub->key;
	uint8_t tmp_field[8];
	uint32_t total_len = 0, tmp_len = 0, out_max;

	RK_CRYPTO_CHECK_PARAM(!asn1_key || !asn1_key_len || !pub || !key_bits);
	RK_CRYPTO_CHECK_PARAM(!rsa_key->n || rsa_key->n_len == 0);
	RK_CRYPTO_CHECK_PARAM(!rsa_key->e || rsa_key->e_len == 0);
	RK_CRYPTO_CHECK_PARAM(rsa_key->n_len != 1024 / 8 &&
			      rsa_key->n_len != 2048 / 8 &&
			      rsa_key->n_len != 3072 / 8 &&
			      rsa_key->n_len != 4096 / 8);
	RK_CRYPTO_CHECK_PARAM(pub->key_type != RK_RSA_KEY_TYPE_PLAIN);

	out_max = *asn1_key_len;

	//padding n
	res = asn1_set_object(rsa_key->n, rsa_key->n_len, ASN1_INTEGER, 1,
			      asn1_key + total_len, out_max, &tmp_len);
	if (res != RK_CRYPTO_SUCCESS) {
		D_TRACE("set rsa_key->n object error!");
		goto exit;
	}

	total_len += tmp_len;
	out_max -= tmp_len;

	//padding e
	res = asn1_set_object(rsa_key->e, rsa_key->e_len, ASN1_INTEGER, 0,
			      asn1_key + total_len, out_max, &tmp_len);
	if (res != RK_CRYPTO_SUCCESS) {
		D_TRACE("set rsa_key->e object error!");
		goto exit;
	}
	total_len += tmp_len;
	out_max   -= tmp_len;

	//add SEQUENCE info in head
	res = asn1_compose_len(total_len, tmp_field, &tmp_len);
	if (res != RK_CRYPTO_SUCCESS) {
		D_TRACE("set asn1_compose_len error!");
		goto exit;
	}

	if (out_max < tmp_len + 1) {
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memmove(asn1_key + tmp_len + 1, asn1_key, total_len);
	*asn1_key = ASN1_SEQUENCE;
	total_len++;
	memmove(asn1_key + 1, tmp_field, tmp_len);
	total_len += tmp_len;

	*asn1_key_len = total_len;
	*key_bits = rsa_key->n_len * 8;
exit:
	return res;
}

RK_RES rk_rsa_privkey_encode(rk_rsa_priv_key_pack *priv,
			     uint8_t *asn1_key, uint16_t *asn1_key_len, uint16_t *key_bits)
{
	RK_RES res;
	uint8_t *empty_data = NULL;
	uint8_t tmp_field[5];
	uint32_t total_len = 0, tmp_len = 0;
	uint8_t version[1] = {0};
	uint16_t ver_len = 1;
	rk_rsa_priv_key *rsa_key = &priv->key;
	uint32_t i, out_max, empty_data_len;
	asn1_object_t object_list[] = {
		{version,     &ver_len,          ASN1_INTEGER, 0},
		{rsa_key->n,  &rsa_key->n_len,   ASN1_INTEGER, 1},
		{rsa_key->e,  &rsa_key->e_len,   ASN1_INTEGER, 0},
		{rsa_key->d,  &rsa_key->d_len,   ASN1_INTEGER, 1},
		{rsa_key->p,  &rsa_key->p_len,   ASN1_INTEGER, 1},
		{rsa_key->q,  &rsa_key->q_len,   ASN1_INTEGER, 1},
		{rsa_key->dp, &rsa_key->dp_len,  ASN1_INTEGER, 1},
		{rsa_key->dq, &rsa_key->dq_len,  ASN1_INTEGER, 1},
		{rsa_key->qp, &rsa_key->qp_len,  ASN1_INTEGER, 1},
	};

	RK_CRYPTO_CHECK_PARAM(!asn1_key || !asn1_key_len || !priv || !key_bits);
	RK_CRYPTO_CHECK_PARAM(!rsa_key->n || rsa_key->n_len == 0);
	RK_CRYPTO_CHECK_PARAM(!rsa_key->e || rsa_key->e_len == 0);
	RK_CRYPTO_CHECK_PARAM(rsa_key->n_len != 1024 / 8 &&
			      rsa_key->n_len != 2048 / 8 &&
			      rsa_key->n_len != 3072 / 8 &&
			      rsa_key->n_len != 4096 / 8);
	RK_CRYPTO_CHECK_PARAM(!rsa_key->d || rsa_key->d_len == 0);
	RK_CRYPTO_CHECK_PARAM(rsa_key->n_len != rsa_key->d_len);

	RK_CRYPTO_CHECK_PARAM(priv->key_type != RK_RSA_KEY_TYPE_PLAIN);

	out_max = *asn1_key_len;

	empty_data_len = rsa_key->n_len / 2;
	empty_data = malloc(empty_data_len);
	if (!empty_data)
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;

	memset(empty_data, 0xff, empty_data_len);

	for (i = 0; i < ARRAY_SIZE(object_list); i++) {
		const uint8_t *data = object_list[i].data;
		uint32_t data_len = *(object_list[i].data_len);

		data     = data ? data : empty_data;
		data_len = data ? data_len : empty_data_len;

		res = asn1_set_object(data, data_len,
				      object_list[i].tag,
				      object_list[i].need_plus,
				      asn1_key + total_len, out_max, &tmp_len);
		if (res != RK_CRYPTO_SUCCESS) {
			D_TRACE("set %d object error!", i);
			goto exit;
		}

		total_len += tmp_len;
		out_max   -= tmp_len;
	}

	res = asn1_compose_len(total_len, tmp_field, &tmp_len);
	if (res != RK_CRYPTO_SUCCESS) {
		D_TRACE("set asn1_compose_len error!");
		goto exit;
	}

	if (out_max < tmp_len + 1) {
		res = RK_CRYPTO_ERR_OUT_OF_MEMORY;
		goto exit;
	}

	memmove(asn1_key + tmp_len + 1, asn1_key, total_len);
	*asn1_key = ASN1_SEQUENCE;
	total_len++;
	memmove(asn1_key + 1, tmp_field, tmp_len);
	total_len += tmp_len;

	*asn1_key_len = total_len;
	*key_bits = rsa_key->n_len * 8;
exit:
	if (empty_data)
		free(empty_data);

	return res;
}

RK_RES rk_rsa_crypt_do_padding(enum RK_RSA_CRYPT_PADDING padding,
			       uint16_t key_len, bool is_priv_key,
			       const uint8_t *data, uint32_t data_len,
			       uint8_t *pad, uint32_t *pad_len)
{
	RK_RES res = RK_CRYPTO_SUCCESS;

	RK_CRYPTO_CHECK_PARAM(key_len * 8 != RSA_BITS_1024 &&
			      key_len * 8 != RSA_BITS_2048 &&
			      key_len * 8 != RSA_BITS_3072 &&
			      key_len * 8 != RSA_BITS_4096);

	switch (padding) {
	case RK_RSA_CRYPT_PADDING_NONE:
		if (data_len != key_len) {
			D_TRACE("length not match %u != %u", data_len, key_len);
			return RK_CRYPTO_ERR_PARAMETER;
		}

		memcpy(pad, data, data_len);
		break;
	case RK_RSA_CRYPT_PADDING_BLOCK_TYPE_0:
	case RK_RSA_CRYPT_PADDING_BLOCK_TYPE_1:
	case RK_RSA_CRYPT_PADDING_BLOCK_TYPE_2: {
		uint8_t bt = (uint8_t)(padding - RK_RSA_CRYPT_PADDING_BLOCK_TYPE_0);

		res = rsa_padding_add_pkcs1_type(key_len, bt, data, data_len, pad);
		break;
	}
	case RK_RSA_CRYPT_PADDING_OAEP_SHA1:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA224:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA256:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA384:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA512:
		res = rsa_padding_add_oaep_type(padding, key_len, NULL, 0, data, data_len, pad);
		break;
	case RK_RSA_CRYPT_PADDING_PKCS1_V1_5:
		res = rsa_padding_add_pkcs15_type(key_len, is_priv_key, data, data_len, pad);
		break;
	default:
		D_TRACE("unknown padding %d", padding);
		res = RK_CRYPTO_ERR_PARAMETER;
		break;
	}

	*pad_len = key_len;

	return res;
}

RK_RES rk_rsa_crypt_undo_padding(enum RK_RSA_CRYPT_PADDING padding,
				 uint16_t key_len, bool is_priv_key,
				 const uint8_t *pad, uint32_t pad_len,
				 uint8_t *data, uint32_t *data_len)
{
	RK_RES res = RK_CRYPTO_SUCCESS;

	RK_CRYPTO_CHECK_PARAM(key_len * 8 != RSA_BITS_1024 &&
			      key_len * 8 != RSA_BITS_2048 &&
			      key_len * 8 != RSA_BITS_3072 &&
			      key_len * 8 != RSA_BITS_4096);
	RK_CRYPTO_CHECK_PARAM(key_len != pad_len);

	switch (padding) {
	case RK_RSA_CRYPT_PADDING_NONE:
		if (pad_len != key_len) {
			D_TRACE("length not match %u != %u", pad_len, key_len);
			return RK_CRYPTO_ERR_PARAMETER;
		}

		memcpy(data, pad, pad_len);
		*data_len = key_len;
		break;
	case RK_RSA_CRYPT_PADDING_BLOCK_TYPE_0:
	case RK_RSA_CRYPT_PADDING_BLOCK_TYPE_1:
	case RK_RSA_CRYPT_PADDING_BLOCK_TYPE_2: {
		uint8_t bt = (uint8_t)(padding - RK_RSA_CRYPT_PADDING_BLOCK_TYPE_0);

		res = rsa_padding_check_pkcs1_type(key_len, bt, pad, data, data_len);
		break;
	}
	case RK_RSA_CRYPT_PADDING_OAEP_SHA1:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA224:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA256:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA384:
	case RK_RSA_CRYPT_PADDING_OAEP_SHA512:
		res = rsa_padding_check_oaep_type(padding,  key_len, NULL, 0, pad, data, data_len);
		break;
	case RK_RSA_CRYPT_PADDING_PKCS1_V1_5:
		res = rsa_padding_check_pkcs15_type(key_len, is_priv_key, pad, data, data_len);
		break;
	default:
		D_TRACE("unknown padding %d", padding);
		res = RK_CRYPTO_ERR_PARAMETER;
		break;
	}

	return res;
}

RK_RES rk_rsa_sign_do_padding(enum RK_RSA_SIGN_PADDING padding, uint16_t key_len, uint16_t n_bits,
			      const uint8_t *data, uint32_t data_len, const uint8_t *hash,
			      uint8_t *pad, uint32_t *pad_len)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint32_t hash_len, hash_algo;
	uint8_t tmp_hash[SHA512_HASH_SIZE];

	res = get_hash_algo_from_padding(padding, &hash_len, &hash_algo);
	if (res)
		return res;

	memset(tmp_hash, 0x00, sizeof(tmp_hash));

	if (hash) {
		memcpy(tmp_hash, hash, hash_len);
	} else {
		res = calc_padding_digest(hash_algo, data, data_len, tmp_hash);
		if (res)
			return res;
	}

	switch (padding) {
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA1:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA224:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA384:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA512:
		res = rsa_padding_add_pkcs15_sign_type(hash_algo, key_len, tmp_hash, hash_len, pad);
		break;
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA1:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA224:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA256:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA384:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA512:
		res = rsa_padding_add_pss_type(key_len, n_bits, hash_algo, tmp_hash, hash_len, pad);
		break;
	default:
		D_TRACE("unknown padding %d", padding);
		res = RK_CRYPTO_ERR_PARAMETER;
		break;
	}

	*pad_len = key_len;

	return res;
}

RK_RES rk_rsa_sign_undo_padding(enum RK_RSA_SIGN_PADDING padding, uint16_t key_len, uint16_t n_bits,
				const uint8_t *data, uint32_t data_len,
				const uint8_t *hash, const uint8_t *dec)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	uint32_t hash_len, hash_algo;
	uint8_t tmp_hash[SHA512_HASH_SIZE];
	uint8_t *pad = NULL;

	res = get_hash_algo_from_padding(padding, &hash_len, &hash_algo);
	if (res)
		return res;

	memset(tmp_hash, 0x00, sizeof(tmp_hash));

	if (hash) {
		memcpy(tmp_hash, hash, hash_len);
	} else {
		res = calc_padding_digest(hash_algo, data, data_len, tmp_hash);
		if (res)
			goto exit;
	}

	pad = malloc(key_len);
	if (!pad)
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;

	switch (padding) {
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA1:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA224:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA384:
	case RK_RSA_SIGN_PADDING_PKCS1_V15_SHA512:
		res = rsa_padding_add_pkcs15_sign_type(hash_algo, key_len, tmp_hash, hash_len, pad);
		if (res) {
			D_TRACE("check pkcs V1.5 error.");
			goto exit;
		}

		if (memcmp(pad, dec, key_len))
			res = RK_CRYPTO_ERR_VERIFY;

		break;
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA1:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA224:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA256:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA384:
	case RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA512:
		res = rk_rsa_padding_check_pss_type(key_len, n_bits, hash_algo,
						    hash_len, tmp_hash, dec);
		break;
	default:
		D_TRACE("unknown padding %d", padding);
		res = RK_CRYPTO_ERR_PARAMETER;
		break;
	}

exit:
	if (pad)
		free(pad);

	return res;
}

