// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <rktest.h>

/*
 * You can enable or disable the OTP test functions here.
 * BE CAUTION:
 * OTP means One Time Programmable Memory.
 * You can read OTP memory many times, but you can only write it once.
 * The "otp_write" program will try to write one byte to OTP memory (offset 0).
 */
#ifndef OTP_TEST
#define OTP_TEST DISABLE
#endif


static const struct{
	const char *word;
	enum_func main_cmd;
} keyword[] = {
	{"transfer_data",		TRANSFER_DATA},
	{"storage",			STORAGE},
	{"storage_speed",		STORAGE_SPEED},
	{"property",			PROPERTY},
	{"crypto_sha",			CRYPTO_SHA},
	{"crypto_aes",			CRYPTO_AES},
	{"crypto_rsa",			CRYPTO_RSA},
#if (OTP_TEST == ENABLE)
	{"otp_read",			OTP_READ},
	{"otp_write",			OTP_WRITE},
	{"otp_size",			OTP_SIZE},
#endif
	{"trng",			TRNG_READ},
	{NULL,				TEST_NULL},
};


static void printf_main_cmd(void)
{
	printf("Please entry one correct parameter when excuting the app!\n");
	printf("The correct parameters list:\n");
	for (int i = 0; keyword[i].word; i++)
		printf("	%s\n", keyword[i].word);
}

static enum_func config_main_cmd(const char *cp)
{
	for (int i = 0; keyword[i].word; i++)
		if (strcasecmp(cp, keyword[i].word) == 0)
			return keyword[i].main_cmd;

	printf_main_cmd();
	return TEST_NULL;
}

int main(int argc, char *argv[])
{
	uint32_t invokeCommand = TEST_NULL;

	if (argc != 2) {
		printf_main_cmd();
		return 0;
	}

	invokeCommand = config_main_cmd(argv[1]);
	return rk_test(invokeCommand);
}

