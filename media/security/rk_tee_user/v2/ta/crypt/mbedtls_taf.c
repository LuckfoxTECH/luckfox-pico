// SPDX-License-Identifier: BSD-2-Clause
/* Copyright (c) 2018, Linaro Limited */

#include <mbedtls_taf.h>
#include <mbedtls/aes.h>
#include <mbedtls/base64.h>
#include <mbedtls/bignum.h>
#include <mbedtls/des.h>
#include <mbedtls/md5.h>
#include <mbedtls/rsa.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/x509_csr.h>
#include <mbedtls/x509.h>
#include <stdio.h>
#include <string.h>
#include <tee_internal_api.h>

/* From ca_crt.c */
extern const uint8_t ca_crt[];
extern const size_t ca_crt_size;
/* From mid_crt.c */
extern const uint8_t mid_crt[];
extern const size_t mid_crt_size;
/* From mid_key.c */
extern const uint8_t mid_key[];
extern const size_t mid_key_size;

TEE_Result
ta_entry_mbedtls_self_tests(uint32_t param_type,
			    TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	if (param_type != exp_pt)
		return TEE_ERROR_BAD_PARAMETERS;

#ifdef CFG_TA_MBEDTLS_SELF_TEST
#define DO_MBEDTLS_SELF_TEST(x) do { \
		if (mbedtls_##x##_self_test(1)) { \
			EMSG("mbedtls_%s_self_test: failed", #x); \
			return TEE_ERROR_GENERIC; \
		} \
	} while (0)

	DO_MBEDTLS_SELF_TEST(aes);
	DO_MBEDTLS_SELF_TEST(des);
	DO_MBEDTLS_SELF_TEST(md5);
	DO_MBEDTLS_SELF_TEST(sha1);
	DO_MBEDTLS_SELF_TEST(sha256);
	DO_MBEDTLS_SELF_TEST(base64);
	DO_MBEDTLS_SELF_TEST(mpi);
	DO_MBEDTLS_SELF_TEST(rsa);
	DO_MBEDTLS_SELF_TEST(x509);

	return TEE_SUCCESS;
#else
	return TEE_ERROR_NOT_IMPLEMENTED;
#endif
}

TEE_Result ta_entry_mbedtls_check_cert(uint32_t param_type,
				    TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res = TEE_SUCCESS;
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
	int ret = 0;
	uint32_t flags = 0;
	mbedtls_x509_crt crt = { };
	mbedtls_x509_crt trust_crt = { };

	if (param_type != exp_pt)
		return TEE_ERROR_BAD_PARAMETERS;

	mbedtls_x509_crt_init(&crt);
	mbedtls_x509_crt_init(&trust_crt);

	ret = mbedtls_x509_crt_parse(&crt, params[0].memref.buffer,
				     params[0].memref.size);
	if (ret) {
		EMSG("mbedtls_x509_crt_parse: failed: %#x", ret);
		return TEE_ERROR_BAD_FORMAT;
	}

	ret = mbedtls_x509_crt_parse(&trust_crt, params[1].memref.buffer,
				     params[1].memref.size);
	if (ret) {
		EMSG("mbedtls_x509_crt_parse: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	ret = mbedtls_x509_crt_verify(&crt, &trust_crt, NULL, NULL, &flags,
				      NULL, NULL);
	if (ret) {
		EMSG("mbedtls_x509_crt_verify: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;

	}

out:
	mbedtls_x509_crt_free(&trust_crt);
	mbedtls_x509_crt_free(&crt);

	return res;
}

static int f_rng(void *rng __unused, unsigned char *output, size_t output_len)
{
	TEE_GenerateRandom(output, output_len);
	return 0;
}

static TEE_Result write_cert(mbedtls_x509write_cert *crt, void *buf,
			     size_t *blen)
{
	int ret = 0;
	void *b = NULL;
	size_t bl = 1024;

	ret = mbedtls_x509write_crt_pem(crt, buf, *blen, f_rng, NULL);
	if (!ret)
		return TEE_SUCCESS;

	if (ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
		EMSG("mbedtls_x509write_crt_pem: failed: %#x", ret);
		return TEE_ERROR_GENERIC;
	}

	/*
	 * We were called with a too small buffer, let's find out how
	 * large it has to be.
	 */
	while (true) {
		b = TEE_Malloc(bl, TEE_MALLOC_FILL_ZERO);
		if (!b)
			return TEE_ERROR_OUT_OF_MEMORY;
		ret = mbedtls_x509write_crt_pem(crt, b, bl, f_rng, NULL);
		if (!ret) {
			*blen = strlen(b) + 1;
			TEE_Free(b);
			return TEE_ERROR_SHORT_BUFFER;
		}
		TEE_Free(b);
		if (ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
			EMSG("mbedtls_x509write_crt_pem: failed: %#x", ret);
			return TEE_ERROR_GENERIC;
		}
		bl *= 2;
	}
}

static TEE_Result parse_issuer_cert(mbedtls_x509_crt *crt)
{
	int ret = 0;
	unsigned char *buf = NULL;

	buf = TEE_Malloc(mid_crt_size + 1, TEE_MALLOC_FILL_ZERO);
	if (!buf)
		return TEE_ERROR_OUT_OF_MEMORY;

	memcpy(buf, mid_crt, mid_crt_size);
	ret = mbedtls_x509_crt_parse(crt, buf, mid_crt_size + 1);
	TEE_Free(buf);
	if (ret) {
		EMSG("mbedtls_x509_crt_parse: failed: %#x", ret);
		return TEE_ERROR_BAD_FORMAT;
	}

	return TEE_SUCCESS;
}

static TEE_Result parse_issuer_key(mbedtls_pk_context *pk)
{
	int ret = 0;
	unsigned char *buf = NULL;

	buf = TEE_Malloc(mid_key_size + 1, TEE_MALLOC_FILL_ZERO);
	if (!buf)
		return TEE_ERROR_OUT_OF_MEMORY;

	memcpy(buf, mid_key, mid_key_size);
	ret = mbedtls_pk_parse_key(pk, buf, mid_key_size + 1, NULL, 0);
	TEE_Free(buf);
	if (ret) {
		EMSG("mbedtls_pk_parse_key: failed: %#x", ret);
		return TEE_ERROR_BAD_FORMAT;
	}

	return TEE_SUCCESS;
}

TEE_Result ta_entry_mbedtls_sign_cert(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res = TEE_SUCCESS;
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE);
	char name[256] = { 0 };
	mbedtls_mpi serial = { };
	mbedtls_x509_crt issuer_crt = { };
	mbedtls_pk_context issuer_key = { };
	mbedtls_x509write_cert crt = { };
	mbedtls_x509_csr csr = { };
	int ret = 0;
	size_t sz = 0;

	if (param_type != exp_pt)
		return TEE_ERROR_BAD_PARAMETERS;

	mbedtls_mpi_init(&serial);
	mbedtls_x509_crt_init(&issuer_crt);
	mbedtls_pk_init(&issuer_key);
	mbedtls_x509write_crt_init(&crt);
	mbedtls_x509_csr_init(&csr);

	ret = parse_issuer_cert(&issuer_crt);
	if (ret)
		goto out;
	ret = parse_issuer_key(&issuer_key);
	if (ret)
		goto out;

	ret = mbedtls_mpi_lset(&serial, 1);
	if (ret) {
		EMSG("mbedtls_mpi_read_string: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	ret = mbedtls_x509_csr_parse(&csr, params[0].memref.buffer,
				     params[0].memref.size);
	if (ret) {
		EMSG("mbedtls_x509_csr_parse: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	/* Extract and set subject name */
	ret = mbedtls_x509_dn_gets(name, sizeof(name), &csr.subject);
	if (ret < 0) {
		EMSG("mbedtls_x509_dn_gets: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}
	ret = mbedtls_x509write_crt_set_subject_name(&crt, name);
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_subject_name: failed: %#x",
		     ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	/* Extract and set issuer name */
	ret = mbedtls_x509_dn_gets(name, sizeof(name), &issuer_crt.subject);
	if (ret < 0) {
		EMSG("mbedtls_x509_dn_gets: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}
	ret = mbedtls_x509write_crt_set_issuer_name(&crt, name);
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_issuer_name: failed: %#x",
		     ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	mbedtls_x509write_crt_set_md_alg(&crt, csr.sig_md);
	mbedtls_x509write_crt_set_subject_key(&crt, &csr.pk);
	mbedtls_x509write_crt_set_issuer_key(&crt, &issuer_key);

	ret = mbedtls_x509write_crt_set_serial(&crt, &serial);
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_serial: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	ret = mbedtls_x509write_crt_set_validity(&crt, "19700101000000",
						 "20301231235959");
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_validity: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	ret = mbedtls_x509write_crt_set_basic_constraints(&crt, 0, 0);
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_validity: failed: %#x", ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	ret = mbedtls_x509write_crt_set_subject_key_identifier(&crt);
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_subject_key_identifier: failed: %#x",
		     ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	ret = mbedtls_x509write_crt_set_authority_key_identifier(&crt);
	if (ret) {
		EMSG("mbedtls_x509write_crt_set_authority_key_identifier: failed: %#x",
		     ret);
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	sz = params[1].memref.size;
	res = write_cert(&crt, params[1].memref.buffer, &sz);
	if (res && res != TEE_ERROR_SHORT_BUFFER)
		goto out;
	params[1].memref.size = sz;

	ret = snprintf(params[2].memref.buffer, params[2].memref.size,
		       "%*s\n%*s", (int)mid_crt_size, (char *)mid_crt,
		       (int)ca_crt_size, (char *)ca_crt);
	if (ret < 0) {
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}
	if ((size_t)ret >= params[2].memref.size)
		res = TEE_ERROR_SHORT_BUFFER;
	params[2].memref.size = ret + 1;

out:
	mbedtls_mpi_free(&serial);
	mbedtls_x509_crt_free(&issuer_crt);
	mbedtls_pk_free(&issuer_key);
	mbedtls_x509write_crt_free(&crt);
	mbedtls_x509_csr_free(&csr);
	return res;
}
