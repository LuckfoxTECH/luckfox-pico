/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rkcrypto_common.h"
#include "rkcrypto_core.h"
#include "rkcrypto_mem.h"

#define BLOCK_SIZE	512 * 1024
#define MAX_COUNT	1024

int test_crypto_mem(void)
{
	int32_t count;
	rk_crypto_mem *mem[MAX_COUNT];

	if (rk_crypto_init()) {
		printf("rk_crypto_init error!\n");
		return -1;
	}

	for (count = 0; count < MAX_COUNT; count ++) {
		mem[count] = rk_crypto_mem_alloc(BLOCK_SIZE);
		if (!mem[count]) {
			usleep(300); // wait for kernel log print to avoid confusion
			printf("Crypto mem alloc: maximum size is [%d KB * %d].\n",
			       (BLOCK_SIZE / 1024), count);
			break;
		}
	}

	if (count == MAX_COUNT) {
		printf("Crypto mem alloc: successfully alloc [%d KB * %d] size.\n",
		       (BLOCK_SIZE / 1024), count);
		printf("You and can alloc for more. Change '#define MAX_COUNT' to test larger size.\n");
	}

	for (count = count - 1; count >= 0; count --) {
		rk_crypto_mem_free(mem[count]);
	}

	rk_crypto_deinit();

	return 0;
}
