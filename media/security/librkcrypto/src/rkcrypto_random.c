/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "rkcrypto_random.h"
#include "rkcrypto_trace.h"

#ifdef ANDROID
#define HWRNG_NODE	"/dev/hw_random"
#else
#define HWRNG_NODE	"/dev/hwrng"
#endif

RK_RES rk_get_random(uint8_t *data, uint32_t len)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	int hwrng_fd = -1;
	int read_len = 0;

	hwrng_fd = open(HWRNG_NODE, O_RDONLY, 0);
	if (hwrng_fd < 0) {
		E_TRACE("open %s error!", HWRNG_NODE);
		return RK_CRYPTO_ERR_GENERIC;
	}

	read_len = read(hwrng_fd, data, len);
	if (read_len != len) {
		E_TRACE("read %s error!", HWRNG_NODE);
		res = RK_CRYPTO_ERR_GENERIC;
	}

	close(hwrng_fd);

	return res;
}
