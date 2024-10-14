/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include "rkcrypto_demo.h"

static void guide(void)
{
	printf("\n######## Entry one correct parameter. Example: librkcrypto_demo -a ########\n");
	printf("	[-a]: OTPKEY_CIPHER\n");
	printf("	[-b]: OTPKEY_CIPHER_VIRT\n");
	printf("	[-c]: CIPHER\n");
	printf("	[-d]: CIPHER_VIRT\n");
	printf("	[-e]: HASH\n");
	printf("	[-f]: HASH_VIRT\n");
	printf("	[-g]: HMAC\n");
	printf("	[-h]: HMAC_VIRT\n");
	printf("	[-i]: RSA\n");
	printf("	[-j]: AE\n");
	printf("	[-k]: AE_VIRT\n");
}

int main(int argc, char *argv[])
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	int opt = 0;
	const char *opt_string = "abcdefghijk";

	if (argc < 2)
		guide();

	while ((opt = getopt(argc, argv, opt_string)) != -1) {
		switch (opt) {
		case 'a':
			printf("TEST OTPKEY_CIPHER:\n");
			res = demo_otpkey();
			break;
		case 'b':
			printf("TEST OTPKEY_CIPHER_VIRT:\n");
			res = demo_otpkey_virt();
			break;
		case 'c':
			printf("TEST CIPHER:\n");
			res = demo_cipher();
			break;
		case 'd':
			printf("TEST CIPHER_VIRT:\n");
			res = demo_cipher_virt();
			break;
		case 'e':
			printf("TEST HASH:\n");
			res = demo_hash();
			break;
		case 'f':
			printf("TEST HASH_VIRT:\n");
			res = demo_hash_virt();
			break;
		case 'g':
			printf("TEST HMAC:\n");
			res = demo_hmac();
			break;
		case 'h':
			printf("TEST HMAC_VIRT:\n");
			res = demo_hmac_virt();
			break;
		case 'i':
			printf("TEST RSA:\n");
			res = demo_rsa();
			break;
		case 'j':
			printf("TEST AE:\n");
			res = demo_ae();
			break;
		case 'k':
			printf("TEST AE_VIRT:\n");
			res = demo_ae_virt();
			break;
		case '?':
			printf("error optopt: %c\n", optopt);
			break;
		default:
			break;
		}
	}

	return res;
}

