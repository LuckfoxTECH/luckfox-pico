// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2016, Linaro Limited
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#include <err.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef OPENSSL_FOUND
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#endif

#include <adbg.h>
#include "xtest_test.h"
#include "xtest_helpers.h"

/* include here shandalone tests */
#include "crypto_common.h"
#include "install_ta.h"
#include "stats.h"


ADBG_SUITE_DEFINE(benchmark);
#ifdef WITH_GP_TESTS
ADBG_SUITE_DEFINE(gp);
#endif
#ifdef CFG_PKCS11_TA
ADBG_SUITE_DEFINE(pkcs11);
#endif
ADBG_SUITE_DEFINE(regression);

char *xtest_tee_name = NULL;
unsigned int level = 0;
static const char glevel[] = "0";

#ifdef WITH_GP_TESTS
#define GP_SUITE	"+gp"
#else
#define GP_SUITE	""
#endif

#ifdef CFG_PKCS11_TA
#define PKCS11_SUITE	"+pkcs11"
#else
#define PKCS11_SUITE	""
#endif

static char gsuitename[] = "regression" GP_SUITE PKCS11_SUITE;

void usage(char *program);

void usage(char *program)
{
	printf("Usage: %s <options> [[-x] <test-id>]...]\n", program);
	printf("\n");
	printf("options:\n");
	printf("\t-d <TEE-identifer> TEE identifier. Use default TEE if not set\n");
	printf("\t-l <level>         Test level [0-15].  Values higher than 0 enable\n");
	printf("\t                   optional tests. Default: 0. All tests: 15.\n");
	printf("\t-t <test_suite>    Available test suites: regression benchmark");
#ifdef WITH_GP_TESTS
	printf(" gp");
#endif
#ifdef CFG_PKCS11_TA
	printf(" pkcs11");
#endif
	printf("\n");
	printf("\t                   To run several suites, use multiple names\n");
	printf("\t                   separated by a '+')\n");
	printf("\t                   Default value: '%s'\n", gsuitename);
	printf("\t-h                 Show usage\n");
	printf("\t<test-id>          Add <test-id> to the list of tests to be run.\n");
	printf("\t                   A substring match is performed. May be specified\n");
	printf("\t                   several times. If no tests are given, all the\n");
	printf("\t                   tests are added.\n");
	printf("\t-x <test-id>       Exclude <test-id> from the list of tests to be\n");
	printf("\t                   run. A substring match is performed. May be\n");
	printf("\t                   specified several times.\n");
	printf("applets:\n");
	printf("\t--sha-perf [opts]  SHA performance testing tool (-h for usage)\n");
	printf("\t--aes-perf [opts]  AES performance testing tool (-h for usage)\n");
#ifdef CFG_SECSTOR_TA_MGMT_PTA
	printf("\t--install-ta [directory or list of TAs]\n");
	printf("\t                   Install TAs\n");
#endif
#ifdef CFG_SECURE_DATA_PATH
	printf("\t--sdp-basic [opts] Basic Secure Data Path test setup ('-h' for usage)\n");
#endif
	printf("\t--stats [opts]     Various statistics ('-h' for usage)\n");
	printf("\n");
	printf("Examples:\n");
	printf("\txtest -t regression 4001 4003\n");
	printf("\t                   run regression tests 4001 and 4003\n");
	printf("\txtest -t regression -x 1027 -x 1028\n");
	printf("\t                   run all regression tests but 1027 and 1028\n");
	printf("\n");
}

static void init_ossl(void)
{
#ifdef OPENSSL_FOUND
	OPENSSL_init();
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
#endif
}

int main(int argc, char *argv[])
{
	int opt = 0;
	int index = 0;
	TEEC_Result tee_res = TEEC_ERROR_GENERIC;
	int ret = 0;
	char *p = (char *)glevel;
	char *test_suite = (char *)gsuitename;
	char *token = NULL;
	ADBG_Suite_Definition_t all = {
		.SuiteID_p = NULL,
		.cases = TAILQ_HEAD_INITIALIZER(all.cases),
	};
	bool exclusion = false;
	size_t last_gen_option = 1;

	opterr = 0;

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		warn("signal(SIGPIPE, SIG_IGN)");

	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
		warn("signal(SIGPIPE, SIG_IGN)");

	init_ossl();

	if (argc > 1 && !strcmp(argv[1], "--sha-perf"))
		return sha_perf_runner_cmd_parser(argc-1, &argv[1]);
	else if (argc > 1 && !strcmp(argv[1], "--aes-perf"))
		return aes_perf_runner_cmd_parser(argc-1, &argv[1]);
#ifdef CFG_SECSTOR_TA_MGMT_PTA
	else if (argc > 1 && !strcmp(argv[1], "--install-ta"))
		return install_ta_runner_cmd_parser(argc - 1, argv + 1);
#endif
#ifdef CFG_SECURE_DATA_PATH
	else if (argc > 1 && !strcmp(argv[1], "--sdp-basic"))
		return sdp_basic_runner_cmd_parser(argc-1, &argv[1]);
#endif
	else if (argc > 1 && !strcmp(argv[1], "--stats"))
		return stats_runner_cmd_parser(argc - 1, &argv[1]);

	while ((opt = getopt(argc, argv, "d:l:t:h")) != -1) {
		switch (opt) {
		case 'd':
			xtest_tee_name = optarg;
			last_gen_option = optind;
			break;
		case 'l':
			p = optarg;
			last_gen_option = optind;
			break;
		case 't':
			test_suite = optarg;
			last_gen_option = optind;
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		case '?':
			if (optopt == 'x') {
				/*
				 * The -x option is not processed here,
				 * it is part of the test IDs.
				 */
				goto next;
			}
			/* option not recognized */
			usage(argv[0]);
			return -1;
		default:
			usage(argv[0]);
			return -1;
		}
	}
next:

	for (index = last_gen_option; index < argc; index++) {
		if (!strcmp(argv[index], "-x")) {
			exclusion = true;
			continue;
		}
		printf("Test ID: %s%s\n", exclusion ? "-x " : "", argv[index]);
		exclusion = false;
	}

	if (p)
		level = atoi(p);
	else
		level = 0;
	printf("Run test suite with level=%d\n", level);

	printf("\nTEE test application started over %s TEE instance\n",
	       xtest_tee_name ? xtest_tee_name : "default");

	tee_res = xtest_teec_ctx_init();
	if (tee_res != TEEC_SUCCESS) {
		fprintf(stderr, "Failed to open TEE context: 0x%" PRIx32 "\n",
								tee_res);
		return -1;
	}

	/* Concatenate all the selected suites into 'all' */
	for (token = test_suite; ; token = NULL) {

		token = strtok(token, "+");
		if (!token)
			break;

		if (!strcmp(token, "regression"))
			ret = Do_ADBG_AppendToSuite(&all, &ADBG_Suite_regression);
		else if (!strcmp(token, "benchmark"))
			ret = Do_ADBG_AppendToSuite(&all, &ADBG_Suite_benchmark);
#ifdef WITH_GP_TESTS
		else if (!strcmp(token, "gp"))
			ret = Do_ADBG_AppendToSuite(&all, &ADBG_Suite_gp);
#endif
#ifdef CFG_PKCS11_TA
		else if (!strcmp(token, "pkcs11"))
			ret = Do_ADBG_AppendToSuite(&all, &ADBG_Suite_pkcs11);
#endif
		else {
			fprintf(stderr, "Unkown test suite: %s\n", token);
			ret = -1;
		}
		if (ret < 0)
			goto err;
	}

	/* Run the tests */
	ret = Do_ADBG_RunSuite(&all, argc - last_gen_option, argv + last_gen_option);

err:
	free((void *)all.SuiteID_p);
	xtest_teec_ctx_deinit();

	printf("TEE test application done!\n");
	return ret;
}
