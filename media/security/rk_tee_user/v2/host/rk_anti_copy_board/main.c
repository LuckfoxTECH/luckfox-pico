// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "anti_copy.h"

static const struct {
	const char *word;
	enum_func main_cmd;
} keyword[] = {
	{"low_gen",			LOW_GEN},
	{"low_verify",			LOW_VERIFY},
	{"mid_gen",			MID_GEN},
	{"mid_verify",			MID_VERIFY},
	{"high_gen",			HIGH_GEN},
	{"high_verify",			HIGH_VERIFY},
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
	return anti_copy(invokeCommand);
}

