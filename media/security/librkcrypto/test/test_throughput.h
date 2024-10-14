/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _TEST_THROUGHPUT_H_
#define _TEST_THROUGHPUT_H_

#include <stdint.h>
#include "rkcrypto_common.h"

enum {
	T_DMA_FD = 0,
	T_VIRT,
	T_OTP,
	T_ALL,
	T_MAX,
};

RK_RES test_throughput(int throughput_mode);

#endif /*_TEST_THROUGHPUT_H_*/
