// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#include <adbg.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <tee_client_api.h>
#include <time.h>
#include <unistd.h>

#include "crypto_common.h"
#include "xtest_helpers.h"

/*
 * TEE client stuff
 */

static TEEC_Context ctx;
static TEEC_Session sess;
static TEEC_SharedMemory in_shm = {
	.flags = TEEC_MEM_INPUT
};
static TEEC_SharedMemory out_shm = {
	.flags = TEEC_MEM_OUTPUT
};

static void errx(const char *msg, TEEC_Result res, uint32_t *orig)
{
	fprintf(stderr, "%s: 0x%08x", msg, res);
	if (orig)
		fprintf(stderr, " (orig=%d)", (int)*orig);
	fprintf(stderr, "\n");
	exit (1);
}

static void check_res(TEEC_Result res, const char *errmsg, uint32_t *orig)
{
	if (res != TEEC_SUCCESS)
		errx(errmsg, res, orig);
}

static void open_ta(void)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_UUID uuid = TA_SHA_PERF_UUID;
	uint32_t err_origin = 0;

	res = TEEC_InitializeContext(NULL, &ctx);
	check_res(res,"TEEC_InitializeContext", NULL);

	res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL,
			       NULL, &err_origin);
	check_res(res,"TEEC_OpenSession", &err_origin);
}

/*
 * Statistics
 *
 * We want to compute min, max, mean and standard deviation of processing time
 */

struct statistics {
	int n;
	double m;
	double M2;
	double min;
	double max;
	int initialized;
};

/* Take new sample into account (Knuth/Welford algorithm) */
static void update_stats(struct statistics *s, uint64_t t)
{
	double x = (double)t;
	double delta = x - s->m;

	s->n++;
	s->m += delta/s->n;
	s->M2 += delta*(x - s->m);
	if (!s->initialized) {
		s->min = s->max = x;
		s->initialized = 1;
	} else {
		if (s->min > x)
			s->min = x;
		if (s->max < x)
			s->max = x;
	}
}

static double stddev(struct statistics *s)
{
	if (s->n < 2)
		return NAN;
	return sqrt(s->M2/s->n);
}

static const char *algo_str(uint32_t algo)
{
	switch (algo) {
	case TA_SHA_SHA1:
		return "SHA1";
	case TA_SHA_SHA224:
		return "SHA224";
	case TA_SHA_SHA256:
		return "SHA256";
	case TA_SHA_SHA384:
		return "SHA384";
	case TA_SHA_SHA512:
		return "SHA512";
	default:
		return "???";
	}
}

static int hash_size(uint32_t algo)
{
	switch (algo) {
	case TA_SHA_SHA1:
		return 20;
	case TA_SHA_SHA224:
		return 28;
	case TA_SHA_SHA256:
		return 32;
	case TA_SHA_SHA384:
		return 48;
	case TA_SHA_SHA512:
		return 64;
	default:
		return 0;
	}
}

#define _TO_STR(x) #x
#define TO_STR(x) _TO_STR(x)


static void alloc_shm(size_t sz, uint32_t algo, int offset)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;

	in_shm.buffer = NULL;
	in_shm.size = sz + offset;
	res = TEEC_AllocateSharedMemory(&ctx, &in_shm);
	check_res(res, "TEEC_AllocateSharedMemory", NULL);

	out_shm.buffer = NULL;
	out_shm.size = hash_size(algo);
	res = TEEC_AllocateSharedMemory(&ctx, &out_shm);
	check_res(res, "TEEC_AllocateSharedMemory", NULL);
}

static void free_shm(void)
{
	TEEC_ReleaseSharedMemory(&in_shm);
	TEEC_ReleaseSharedMemory(&out_shm);
}

static ssize_t read_random(void *in, size_t rsize)
{
	static int rnd;
	ssize_t s = 0;

	if (!rnd) {
		rnd = open("/dev/urandom", O_RDONLY);
		if (rnd < 0) {
			perror("open");
			return 1;
		}
	}
	s = read(rnd, in, rsize);
	if (s < 0) {
		perror("read");
		return 1;
	}
	if ((size_t)s != rsize) {
		printf("read: requested %zu bytes, got %zd\n",
		       rsize, s);
	}
	return 0;
}

static long get_current_time(struct timespec *ts)
{
	if (clock_gettime(CLOCK_MONOTONIC, ts) < 0) {
		perror("clock_gettime");
		exit(1);
	}
	return 0;
}

static uint64_t timespec_diff_ns(struct timespec *start, struct timespec *end)
{
	uint64_t ns = 0;

	if (end->tv_nsec < start->tv_nsec) {
		ns += 1000000000 * (end->tv_sec - start->tv_sec - 1);
		ns += 1000000000 - start->tv_nsec + end->tv_nsec;
	} else {
		ns += 1000000000 * (end->tv_sec - start->tv_sec);
		ns += end->tv_nsec - start->tv_nsec;
	}
	return ns;
}

static uint64_t run_test_once(void *in, size_t size,  int random_in,
			      TEEC_Operation *op)
{
	struct timespec t0 = { };
	struct timespec t1 = { };
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t ret_origin = 0;

	if (random_in == CRYPTO_USE_RANDOM)
		read_random(in, size);

	get_current_time(&t0);
	res = TEEC_InvokeCommand(&sess, TA_SHA_PERF_CMD_PROCESS, op,
				 &ret_origin);
	check_res(res, "TEEC_InvokeCommand", &ret_origin);
	get_current_time(&t1);

	return timespec_diff_ns(&t0, &t1);
}

static void prepare_op(int algo)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t ret_origin = 0;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].value.a = algo;
	res = TEEC_InvokeCommand(&sess, TA_SHA_PERF_CMD_PREPARE_OP, &op,
				 &ret_origin);
	check_res(res, "TEEC_InvokeCommand", &ret_origin);
}

static void do_warmup(int warmup)
{
	struct timespec t0 = { };
	struct timespec t = { };
	int i = 0;

	get_current_time(&t0);
	do {
		for (i = 0; i < 100000; i++)
			;
		get_current_time(&t);
	} while (timespec_diff_ns(&t0, &t) < (uint64_t)warmup * 1000000000);
}

static const char *yesno(int v)
{
	return (v ? "yes" : "no");
}

static double mb_per_sec(size_t size, double usec)
{
	return (1000000000/usec)*((double)size/(1024*1024));
}

/* Hash test: buffer of size byte. Run test n times.
 * Entry point for running SHA benchmark
 * Params:
 * algo - Algorithm
 * size - Buffer size
 * n - Number of measurements
 * l - Amount of inner loops
 * random_in - Get input from /dev/urandom
 * offset - Buffer offset wrt. alloc-ed address
 * warmup - Start with a-second busy loop
 * verbosity - Verbosity level
 * */
extern void sha_perf_run_test(int algo, size_t size, unsigned int n,
				unsigned int l, int random_in, int offset,
				int warmup, int verbosity)
{
	uint64_t t = 0;
	struct statistics stats = { };
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	int n0 = n;
	struct timespec ts = { };
	double sd = 0;

	vverbose("sha-perf\n");
	if (clock_getres(CLOCK_MONOTONIC, &ts) < 0) {
		perror("clock_getres");
		return;
	}
	vverbose("Clock resolution is %jd ns\n",
		 (intmax_t)ts.tv_sec * 1000000000 + ts.tv_nsec);

	open_ta();
	prepare_op(algo);

	alloc_shm(size, algo, offset);

	if (random_in == CRYPTO_USE_ZEROS)
		memset((uint8_t *)in_shm.buffer + offset, 0, size);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);
	op.params[0].memref.parent = &in_shm;
	op.params[0].memref.offset = 0;
	op.params[0].memref.size = size + offset;
	op.params[1].memref.parent = &out_shm;
	op.params[1].memref.offset = 0;
	op.params[1].memref.size = hash_size(algo);
	op.params[2].value.a = l;
	op.params[2].value.b = offset;

	verbose("Starting test: %s, size=%zu bytes, ",
		algo_str(algo), size);
	verbose("random=%s, ", yesno(random_in == CRYPTO_USE_RANDOM));
	verbose("unaligned=%s, ", yesno(offset));
	verbose("inner loops=%u, loops=%u, warm-up=%u s\n", l, n, warmup);

	if (warmup)
		do_warmup(warmup);

	while (n-- > 0) {
		t = run_test_once((uint8_t *)in_shm.buffer + offset, size,
				  random_in, &op);
		update_stats(&stats, t);
		if (n % (n0 / 10) == 0)
			vverbose("#");
	}
	vverbose("\n");
	sd = stddev(&stats);
	printf("min=%gus max=%gus mean=%gus stddev=%gus (cv %g%%) (%gMiB/s)\n",
	       stats.min / 1000, stats.max / 1000, stats.m / 1000,
	       sd / 1000, 100 * sd / stats.m, mb_per_sec(size, stats.m));
	verbose("2-sigma interval: %g..%gus (%g..%gMiB/s)\n",
		(stats.m - 2 * sd) / 1000, (stats.m + 2 * sd) / 1000,
		mb_per_sec(size, stats.m + 2 * sd),
		mb_per_sec(size, stats.m - 2 * sd));
	free_shm();
}

static void usage(const char *progname,
				/* Default params */
				int algo, size_t size, int warmup, int l, int n)
{
	fprintf(stderr, "Usage: %s [-h]\n", progname);
	fprintf(stderr, "Usage: %s [-a ALGO] [-l LOOP] [-n LOOP] [-r] [-s SIZE]", progname);
	fprintf(stderr, " [-v [-v]] [-w SEC]\n");
	fprintf(stderr, "SHA performance testing tool for OP-TEE\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -a ALGO          Algorithm (SHA1, SHA224, SHA256, SHA384, SHA512) [%s]\n", algo_str(algo));
	fprintf(stderr, "  -h|--help Print this help and exit\n");
	fprintf(stderr, "  -l LOOP          Inner loop iterations (TA calls TEE_DigestDoFinal() <x> times) [%u]\n", l);
	fprintf(stderr, "  -n LOOP          Outer test loop iterations [%u]\n", n);
	fprintf(stderr, "  -r|--random      Get input data from /dev/urandom (default:  all-zeros)\n");
	fprintf(stderr, "  -s SIZE          Test buffer size in bytes [%zu]\n", size);
	fprintf(stderr, "  -u|--unalign     Use unaligned buffer (odd address)\n");
	fprintf(stderr, "  -v               Be verbose (use twice for greater effect)\n");
	fprintf(stderr, "  -w|--warmup SEC  Warm-up time in seconds: execute a busy loop before\n");
	fprintf(stderr, "                   the test to mitigate the effects of cpufreq etc. [%u]\n", warmup);
}

#define NEXT_ARG(i) \
	do { \
		if (++i == argc) { \
			fprintf(stderr, "%s: %s: missing argument\n", \
				argv[0], argv[i - 1]); \
			return 1; \
		} \
	} while (0);



extern int sha_perf_runner_cmd_parser(int argc, char *argv[])
{
	int i = 0;
	/* Command line params */
	size_t size = 1024;	/* Buffer size (-s) */
	unsigned int n = CRYPTO_DEF_COUNT;/* Number of measurements (-n)*/
	unsigned int l = CRYPTO_DEF_LOOPS;	/* Inner loops (-l) */
	int verbosity = CRYPTO_DEF_VERBOSITY;	/* Verbosity (-v) */
	int algo = TA_SHA_SHA1;	/* Algorithm (-a) */
	/* Get input data from /dev/urandom (-r) */
	int random_in = CRYPTO_USE_ZEROS;
	/* Start with a 2-second busy loop (-w) */
	int warmup = CRYPTO_DEF_WARMUP;
	int offset = 0; /* Buffer offset wrt. alloc'ed address (-u) */

	/* Parse command line */
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage(argv[0], algo, size, warmup, l, n);
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-l")) {
			NEXT_ARG(i);
			l = atoi(argv[i]);
		} else if (!strcmp(argv[i], "-a")) {
			NEXT_ARG(i);
			if (!strcasecmp(argv[i], "SHA1"))
				algo = TA_SHA_SHA1;
			else if (!strcasecmp(argv[i], "SHA224"))
				algo = TA_SHA_SHA224;
			else if (!strcasecmp(argv[i], "SHA256"))
				algo = TA_SHA_SHA256;
			else if (!strcasecmp(argv[i], "SHA384"))
				algo = TA_SHA_SHA384;
			else if (!strcasecmp(argv[i], "SHA512"))
				algo = TA_SHA_SHA512;
			else {
				fprintf(stderr, "%s, invalid algorithm\n",
					argv[0]);
				usage(argv[0], algo, size, warmup, l, n);
				return 1;
			}
		} else if (!strcmp(argv[i], "-n")) {
			NEXT_ARG(i);
			n = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--random") ||
			   !strcmp(argv[i], "-r")) {
			random_in = CRYPTO_USE_RANDOM;
		} else if (!strcmp(argv[i], "-s")) {
			NEXT_ARG(i);
			size = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--unalign") ||
			   !strcmp(argv[i], "-u")) {
			offset = 1;
		} else if (!strcmp(argv[i], "-v")) {
			verbosity++;
		} else if (!strcmp(argv[i], "--warmup") ||
			   !strcmp(argv[i], "-w")) {
			NEXT_ARG(i);
			warmup = atoi(argv[i]);
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			usage(argv[0], algo, size, warmup, l, n);
			return 1;
		}
	}

	sha_perf_run_test(algo, size, n, l, random_in, offset, warmup, verbosity);

	return 0;
}
