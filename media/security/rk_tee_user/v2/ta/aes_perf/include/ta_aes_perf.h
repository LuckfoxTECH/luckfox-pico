/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef TA_AES_PERF_H
#define TA_AES_PERF_H

#define TA_AES_PERF_UUID { 0xe626662e, 0xc0e2, 0x485c, \
	{ 0xb8, 0xc8, 0x09, 0xfb, 0xce, 0x6e, 0xdf, 0x3d } }

/*
 * Commands implemented by the TA
 */

#define TA_AES_PERF_CMD_PREPARE_KEY	0
#define TA_AES_PERF_CMD_PROCESS		1
#define TA_AES_PERF_CMD_PROCESS_SDP	2

/*
 * Supported AES modes of operation
 */

#define TA_AES_ECB	0
#define TA_AES_CBC	1
#define TA_AES_CTR	2
#define TA_AES_XTS	3
#define TA_AES_GCM	4

/*
 * AES key sizes
 */
#define AES_128	128
#define AES_192	192
#define AES_256	256

#endif /* TA_AES_PERF_H */
