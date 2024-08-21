/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef TA_STORAGE_BENCHMARK_H
#define TA_STORAGE_BENCHMARK_H

#define TA_STORAGE_BENCHMARK_UUID { 0xf157cda0, 0x550c, 0x11e5,\
	{ 0xa6, 0xfa, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }

enum storage_benchmark_cmd {
	TA_STORAGE_BENCHMARK_CMD_TEST_READ,
	TA_STORAGE_BENCHMARK_CMD_TEST_WRITE,
	TA_STORAGE_BENCHMARK_CMD_TEST_REWRITE,
};

#endif /* TA_STORAGE_BENCHMARK_H */
