/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rkcrypto_random.h"

#define RAND_MAX_LEN	1024 * 1024

static unsigned int hamming_weight8(uint8_t w)
{
	unsigned int res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	return (res + (res >> 4)) & 0x0F;
}

RK_RES test_random(void)
{
	uint32_t len, hw, i;
	uint8_t data[RAND_MAX_LEN];

	for (len = 8; len <= RAND_MAX_LEN; len *= 2) {
		if (rk_get_random(data, len))
			goto error;

		hw = 0;
		for (i = 0; i < len; i++) {
			hw += hamming_weight8(data[i]);
		}

		/*
		 * With sufficient length (>= 8 bytes), the hamming weight of
		 * every 8 bits should stay between 1 and 7 on average.
		 */
		if (hw < len || hw > (len * 8 - len)) {
			printf("Get random %d bits, hamming_weight is %d.\n", len * 8, hw);
			goto error;
		}
	}

	printf("TEST get random function SUCCESS.\n");
	printf("NOTE: Testing for calling rk_get_random API. No testing for randomness/unpredictability.\n");
	return RK_CRYPTO_SUCCESS;
error:
	printf("TEST get random function FAILED!!!\n");
	return RK_CRYPTO_ERR_GENERIC;
}
