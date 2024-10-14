/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "test_ae.h"
#include "test_otp_key_crypto.h"
#include "test_throughput.h"
#include "test_cipher.h"
#include "test_crypto_mem.h"
#include "test_hash.h"
#include "test_random.h"
#include "test_stress.h"
#include "test_multi.h"
#include "test_rsa.h"

enum {
	OPTION_TOP = 0,
	ALL,
	CIPHER,
	AEAD,
	HASH,
	HMAC,
	SETKEY,
	OTPKEY,
	MEM,
	RANDOM,
	THROUGHPUT,
	STRESS,
	MULTI,
	RSA,
	OPTION_BUTT,
};

static void guide(void)
{
	printf("\n######## rkcrypto api test ########\n");
	printf("%s\n", RK_CRYPTO_API_FULL_VERSION);

	printf("Entry one parameter as follow:\n");
	printf("\t-all           Function of all ciphers\n");
	printf("\t-cipher        Function of cipher\n");
	printf("\t-aead          Function of aead\n");
	printf("\t-hash          Function of hash\n");
	printf("\t-hmac          Function of hmac\n");
	printf("\t-rsa           Function of rsa\n");
	printf("\t-setkey        Function of setkey. NOTE: it will write key to OTP area.\n");
	printf("\t-otpkey        Function of otpkey\n");
	printf("\t-mem           Maximum buffer size requested by crypto mem alloc, test until alloc failed\n");
	printf("\t-random        Function of get random\n");
	printf("\t-throughput=mode  Throughput of all ciphers, MB/s\n");
	printf("\t               0 for dma_fd (default)\n");
	printf("\t               1 for virt\n");
	printf("\t               2 for otp\n");
	printf("\t               3 for all \n");
	printf("\t-stress [cnt]  stress cnt times of all cipher/hash/hmac\n");
}

int main(int argc, char *argv[])
{
	int opt;
	int option_index = 0;
	int stress_cnt = 1;
	int throughput_mode = 0;
	int verbose = 1;
	static struct option long_options[] = {
		{"all",		0,	NULL,	ALL},
		{"cipher",	0,	NULL,	CIPHER},
		{"aead",	0,	NULL,	AEAD},
		{"hash",	0,	NULL,	HASH},
		{"hmac",	0,	NULL,	HMAC},
		{"setkey",	0,	NULL,	SETKEY},
		{"otpkey",	0,	NULL,	OTPKEY},
		{"mem",		0,	NULL,	MEM},
		{"random",	0,	NULL,	RANDOM},
		{"throughput",	optional_argument,	NULL,	THROUGHPUT},
		{"stress",	1,	NULL,	STRESS},
		{"multi",	0,	NULL,	MULTI},
		{"rsa",		0,	NULL,	RSA},
		{NULL,		0,	NULL,	0},
	};

	if (argc < 2)
		guide();

	while ((opt = getopt_long_only(argc, argv, "", long_options, &option_index)) != -1) {
		switch (opt) {
		case ALL:
			test_cipher(verbose);
			test_ae(verbose);
			test_hash(verbose);
			test_hmac(verbose);
			test_rsa(verbose);
			test_write_otp_key();
			test_otp_key();
			test_crypto_mem();
			break;
		case CIPHER:
			test_cipher(verbose);
			break;
		case AEAD:
			test_ae(verbose);
			break;
		case HASH:
			test_hash(verbose);
			break;
		case HMAC:
			test_hmac(verbose);
			break;
		case RSA:
			test_rsa(verbose);
			break;
		case SETKEY:
			test_write_otp_key();
			break;
		case OTPKEY:
			test_otp_key();
			break;
		case MEM:
			test_crypto_mem();
			break;
		case RANDOM:
			test_random();
			break;
		case THROUGHPUT:
			if (optarg != NULL)
				throughput_mode = atoi(optarg);

			printf("throughput_mode = %d\n", throughput_mode);

			if (throughput_mode >= T_MAX) {
				printf("throughput_mode = %d is not support\n", throughput_mode);
				guide();
				return -1;
			}

			test_throughput(throughput_mode);
			break;
		case STRESS:
			stress_cnt = atoi(optarg);
			stress_test(stress_cnt);
			break;
		case MULTI:
			test_multi();
			break;
		case '?':
			guide();
			break;
		default:
			break;
		}

	}

	printf("\n######## Test done ########\n");

	return 0;
}
