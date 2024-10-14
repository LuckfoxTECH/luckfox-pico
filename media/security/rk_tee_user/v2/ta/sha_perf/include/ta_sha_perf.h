/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef TA_SHA_PERF_H
#define TA_SHA_PERF_H

#define TA_SHA_PERF_UUID { 0x614789f2, 0x39c0, 0x4ebf, \
	{ 0xb2, 0x35, 0x92, 0xb3, 0x2a, 0xc1, 0x07, 0xed } }

/*
 * Commands implemented by the TA
 */

#define TA_SHA_PERF_CMD_PREPARE_OP	0
#define TA_SHA_PERF_CMD_PROCESS		1

/*
 * Supported algorithms
 */

#define TA_SHA_SHA1	0
#define TA_SHA_SHA224	1
#define TA_SHA_SHA256	2
#define TA_SHA_SHA384	3
#define TA_SHA_SHA512	4

#endif /* TA_SHA_PERF_H */
