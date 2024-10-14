// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Linaro Limited */

#include "xtest_test.h"
#include "xtest_helpers.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ta_crypt.h>
#include <tee_api_types.h>
#include <adbg.h>

#ifdef OPENSSL_FOUND
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#endif

#include "regression_8100_ca_crt.h"
#include "regression_8100_mid_crt.h"
#include "regression_8100_my_crt.h"
#include "regression_8100_my_csr.h"

#ifdef CFG_TA_MBEDTLS

static void test_8101(ADBG_Case_t *c __maybe_unused)
{
#ifdef CFG_TA_MBEDTLS_SELF_TEST
	TEEC_Session session = { };
	uint32_t ret_orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
					      &session, &crypt_user_ta_uuid,
					      NULL, &ret_orig)))
		return;
	ADBG_EXPECT_TEEC_SUCCESS(c,
		TEEC_InvokeCommand(&session, TA_CRYPT_CMD_MBEDTLS_SELF_TESTS,
				   NULL, &ret_orig));
	TEEC_CloseSession(&session);
#else
	Do_ADBG_Log("CFG_TA_MBEDTLS_SELF_TEST not set, test skipped");
#endif
}
ADBG_CASE_DEFINE(regression, 8101, test_8101, "TA mbedTLS self tests");

static int __printf(2, 3) myasprintf(char **strp, const char *fmt, ...)
{
	char *str = NULL;
	int rc = 0;
	va_list ap;

	va_start(ap, fmt);
	rc = vsnprintf(str, rc, fmt, ap);
	if (rc <= 0)
		goto out;

	str = malloc(rc);
	if (!str) {
		rc = -1;
		goto out;
	}

	rc = vsnprintf(str, rc, fmt, ap);
	if (rc <= 0)
		free(str);
	else
		*strp = str;

out:
	va_end(ap);
	return rc;
}

static void test_8102(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	char *chain = NULL;
	int clen = 0;
	char *trust = NULL;
	int tlen = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
					      &session, &crypt_user_ta_uuid,
					      NULL, &ret_orig)))
		return;

	clen = myasprintf(&chain, "%*s\n%*s",
			  (int)regression_8100_my_crt_size,
			  regression_8100_my_crt,
			  (int)regression_8100_mid_crt_size,
			   regression_8100_mid_crt);
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, clen, !=, -1))
		goto out;
	tlen = myasprintf(&trust, "%*s", (int)regression_8100_ca_crt_size,
			  regression_8100_ca_crt);
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, tlen, !=, -1))
		goto out;

	op.params[0].tmpref.buffer = chain;
	op.params[0].tmpref.size = clen;
	op.params[1].tmpref.buffer = trust;
	op.params[1].tmpref.size = tlen;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_NONE, TEEC_NONE);

	ADBG_EXPECT_TEEC_SUCCESS(c,
		TEEC_InvokeCommand(&session, TA_CRYPT_CMD_MBEDTLS_CHECK_CERT,
				   &op, &ret_orig));
out:
	free(chain);
	free(trust);
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 8102, test_8102, "TA mbedTLS test cert chain");

#ifdef OPENSSL_FOUND
static void osslerr(void)
{
	while (true) {
		unsigned long e = 0;
		char b[256] = { };
		const char *f = NULL;
		int l = 0;

		e = ERR_get_error_line(&f, &l);
		if (!e)
			return;
		ERR_error_string_n(e, b, sizeof(b));
		Do_ADBG_Log("%s:%d \"%s\"", f, l, b);
	}
}

static bool get_cert(ADBG_Case_t *c, const char *crt_str, X509 **crt)
{
	bool ret = false;
	size_t slen = strlen(crt_str) + 1;
	BIO *buf = BIO_new(BIO_s_mem());
	size_t b = 0;

	if (!ADBG_EXPECT_NOT_NULL(c, buf))
		goto out;

	b = BIO_write(buf, crt_str, slen);
	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c, b, ==, slen))
		goto out;

	if (!PEM_read_bio_X509(buf, crt, 0, NULL))
		goto out;

	ret = true;
out:
	if (!ret)
		osslerr();
	BIO_free(buf);
	return ret;
}

static bool push_cert(ADBG_Case_t *c, const char *crt_str, STACK_OF(X509) *cs)
{
	X509 *crt = NULL;
	int rc = 0;

	if (!get_cert(c, crt_str, &crt))
		return false;
	rc = sk_X509_push(cs, crt);
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, rc, >, 0)) {
		osslerr();
		X509_free(crt);
		return false;
	}

	return true;
}

static bool check(ADBG_Case_t *c, STACK_OF(X509) *trusted,
		  STACK_OF(X509) *untrusted, X509 *crt)
{
	bool ret = false;
	X509_STORE *store = NULL;
	X509_STORE_CTX *csc = NULL;
	X509_VERIFY_PARAM *pm = NULL;
	int i = 0;
	time_t vfy_time = 0;

	pm = X509_VERIFY_PARAM_new();
	vfy_time = 1526898005; /* Mon, 21 May 2018 10:20:05 +0000 */
	X509_VERIFY_PARAM_set_time(pm, vfy_time);

	store = X509_STORE_new();
	if (!ADBG_EXPECT_NOT_NULL(c, store))
		goto out;
	X509_STORE_set_flags(store, 0);
	if (!ADBG_EXPECT_TRUE(c, X509_STORE_set1_param(store, pm)))
		goto out;

	csc = X509_STORE_CTX_new();
	if (!ADBG_EXPECT_NOT_NULL(c, csc))
		goto out;

	if (!ADBG_EXPECT_TRUE(c, X509_STORE_CTX_init(csc, store, crt,
						     untrusted)))
		goto out;
	X509_STORE_CTX_trusted_stack(csc, trusted);

	i = X509_verify_cert(csc);
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, i, >, 0))
		goto out;
	i = X509_STORE_CTX_get_error(csc);
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, i, ==, X509_V_OK))
		goto out;
	ret = true;
out:
	if (!ret)
		osslerr();
	X509_VERIFY_PARAM_free(pm);
	X509_STORE_free(store);
	X509_STORE_CTX_free(csc);
	return ret;
}

static bool verify_cert(ADBG_Case_t *c, const char *ca, const char *mid,
			const char *cert)
{
	bool ret = false;
	STACK_OF(X509) *trusted = NULL;
	STACK_OF(X509) *untrusted = NULL;
	X509 *crt = NULL;

	trusted = sk_X509_new_null();
	if (!ADBG_EXPECT_NOT_NULL(c, trusted))
		goto out;
	untrusted = sk_X509_new_null();
	if (!ADBG_EXPECT_NOT_NULL(c, untrusted))
		goto out;

	if (!ADBG_EXPECT_TRUE(c, get_cert(c, cert, &crt)))
		goto out;
	if (!ADBG_EXPECT_TRUE(c, push_cert(c, mid, untrusted)))
		goto out;
	if (!ADBG_EXPECT_TRUE(c, push_cert(c, ca, trusted)))
		goto out;

	ret = ADBG_EXPECT_TRUE(c, check(c, trusted, untrusted, crt));
out:
	if (!ret)
		osslerr();
	X509_free(crt);
	sk_X509_pop_free(untrusted, X509_free);
	sk_X509_pop_free(trusted, X509_free);
	return ret;
}
#else /*!OPENSSL_FOUND*/
static bool verify_cert(ADBG_Case_t *c, const char *ca,
			const char *mid, const char *cert)
{
	UNUSED(c);
	UNUSED(ca);
	UNUSED(mid);
	UNUSED(cert);
	Do_ADBG_Log("OpenSSL not available, skipping certificate verification");
	return true;
}
#endif

static void test_8103(ADBG_Case_t *c)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Session session = { };
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	char *csr = NULL;
	int clen = 0;
	char cert[2048];
	char chain[4096];
	char *ca = NULL;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, xtest_teec_open_session(
					      &session, &crypt_user_ta_uuid,
					      NULL, &ret_orig)))
		return;

	clen = myasprintf(&csr, "%*s", (int)regression_8100_my_csr_size,
			  regression_8100_my_csr);
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, clen, >=, 0))
		goto out;
	op.params[0].tmpref.buffer = csr;
	op.params[0].tmpref.size = clen;
	op.params[1].tmpref.buffer = cert;
	op.params[1].tmpref.size = sizeof(cert);
	op.params[2].tmpref.buffer = chain;
	op.params[2].tmpref.size = sizeof(chain);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
	res = TEEC_InvokeCommand(&session, TA_CRYPT_CMD_MBEDTLS_SIGN_CERT, &op,
				 &ret_orig);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;

	myasprintf(&ca, "%*s", (int)regression_8100_ca_crt_size,
		   regression_8100_ca_crt);
	if (!ADBG_EXPECT_NOT_NULL(c, ca))
		goto out;
	verify_cert(c, ca, op.params[2].tmpref.buffer,
		    op.params[1].tmpref.buffer);
out:
	free(ca);
	free(csr);
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 8103, test_8103,
		 "TA mbedTLS process certificate request");
#endif /*CFG_TA_MBEDTLS*/
