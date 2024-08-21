// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2015, Linaro Limited
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xtest_test.h"
#include "xtest_helpers.h"

#include <crypto_common.h>
#include <util.h>

/* SHA bechmarks */
static void xtest_tee_benchmark_2001(ADBG_Case_t *Case_p);
static void xtest_tee_benchmark_2002(ADBG_Case_t *Case_p);

/* AES benchmarks */
static void xtest_tee_benchmark_2011(ADBG_Case_t *Case_p);
static void xtest_tee_benchmark_2012(ADBG_Case_t *Case_p);

/* ----------------------------------------------------------------------- */
/* -------------------------- SHA Benchmarks ----------------------------- */
/* ----------------------------------------------------------------------- */

static void xtest_tee_benchmark_2001(ADBG_Case_t *c)
{
	UNUSED(c);

	int algo = TA_SHA_SHA1;	/* Algorithm */
	size_t size = 1024;	/* Buffer size */
	int offset = 0;          /* Buffer offset wrt. alloc'ed address */

	sha_perf_run_test(algo, size, CRYPTO_DEF_COUNT,
				CRYPTO_DEF_LOOPS, CRYPTO_USE_RANDOM, offset,
				CRYPTO_DEF_WARMUP, CRYPTO_DEF_VERBOSITY);

}

static void xtest_tee_benchmark_2002(ADBG_Case_t *c)
{
	UNUSED(c);

	int algo = TA_SHA_SHA256;	/* Algorithm */
	size_t size = 4096;	/* Buffer size */
	int offset = 0;          /* Buffer offset wrt. alloc'ed address */

	sha_perf_run_test(algo, size, CRYPTO_DEF_COUNT,
				CRYPTO_DEF_LOOPS, CRYPTO_USE_RANDOM, offset,
				CRYPTO_DEF_WARMUP, CRYPTO_DEF_VERBOSITY);

}

ADBG_CASE_DEFINE(benchmark, 2001, xtest_tee_benchmark_2001,
		"TEE SHA Performance test (TA_SHA_SHA1)");
ADBG_CASE_DEFINE(benchmark, 2002, xtest_tee_benchmark_2002,
		"TEE SHA Performance test (TA_SHA_SHA226)");


/* ----------------------------------------------------------------------- */
/* -------------------------- AES Benchmarks ----------------------------- */
/* ----------------------------------------------------------------------- */

static void xtest_tee_benchmark_2011(ADBG_Case_t *c)
{
	UNUSED(c);

	int mode = TA_AES_ECB;	/* AES mode */
	int decrypt = 0; /* Encrypt */
	int keysize = AES_128;
	size_t size = 1024;	/* Buffer size */

	aes_perf_run_test(mode, keysize, decrypt, size, CRYPTO_DEF_UNIT_SIZE,
		CRYPTO_DEF_COUNT, CRYPTO_DEF_LOOPS, CRYPTO_USE_RANDOM,
		AES_PERF_INPLACE, CRYPTO_DEF_WARMUP, CRYPTO_DEF_VERBOSITY);
}

static void xtest_tee_benchmark_2012(ADBG_Case_t *c)
{
	UNUSED(c);

	int mode = TA_AES_CBC;	/* AES mode */
	int decrypt = 0; /* Encrypt */
	int keysize = AES_256;
	size_t size = 1024;	/* Buffer size */

	aes_perf_run_test(mode, keysize, decrypt, size, CRYPTO_DEF_UNIT_SIZE,
		CRYPTO_DEF_COUNT, CRYPTO_DEF_LOOPS, CRYPTO_USE_RANDOM,
		AES_PERF_INPLACE, CRYPTO_DEF_WARMUP, CRYPTO_DEF_VERBOSITY);
}

ADBG_CASE_DEFINE(benchmark, 2011, xtest_tee_benchmark_2011,
		"TEE AES Performance test (TA_AES_ECB)");
ADBG_CASE_DEFINE(benchmark, 2012, xtest_tee_benchmark_2012,
		"TEE AES Performance test (TA_AES_CBC)");
