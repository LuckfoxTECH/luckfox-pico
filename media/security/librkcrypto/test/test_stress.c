/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rkcrypto_random.h"
#include "test_cipher.h"
#include "test_hash.h"
#include "test_rsa.h"
#include "test_stress.h"
#include "test_utils.h"

typedef RK_RES (*test_func)(int verbose);

struct test_stress_item {
	test_func	func;
	const char	*name;
};

#define DEFINE_STRESS_ITEM(_func) {\
	.func	= _func, \
	.name	= #_func, \
}

struct test_stress_item test_stress_tbl[] = {
	DEFINE_STRESS_ITEM(test_cipher),
	DEFINE_STRESS_ITEM(test_hash),
	DEFINE_STRESS_ITEM(test_hmac),
	DEFINE_STRESS_ITEM(test_rsa),
};

void stress_test(int test_cnt)
{
	int i;
	uint32_t j;
	int verbose = 0;
	RK_RES res;
	uint32_t tbl_len = ARRAY_SIZE(test_stress_tbl);
	uint8_t rand_buf[ARRAY_SIZE(test_stress_tbl)];

	printf("===================== stress test begin =====================\n");
	for (i = 0; i < test_cnt; i++) {
		printf("stress test %d/%d...\n", i + 1, test_cnt);

		res = rk_get_random(rand_buf, tbl_len);
		if (res) {
			printf("rk_get_random failed %x\n",res);
			goto exit;
		}

		for (j = 0; j < tbl_len; j++) {
			uint32_t index = rand_buf[j] % tbl_len;

			res = test_stress_tbl[index].func(verbose);
			if (res) {
				printf("%s error[%x]\n", test_stress_tbl[index].name, res);
				goto exit;
			}
		}
	}

	printf("===================== stress test finish =====================\n");
exit:
	return;
}

