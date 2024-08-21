/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef TA_CONCURRENT_H
#define TA_CONCURRENT_H

/* This UUID is generated with the ITU-T UUID generator at
   http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_CONCURRENT_UUID { 0xe13010e0, 0x2ae1, 0x11e5, \
	{ 0x89, 0x6a, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }

struct ta_concurrent_shm {
	uint32_t active_count;
};

/*
 * Busy loops and updates max concurrency.  params[0].memref should contain
 * a struct ta_concurent_shm which can be used to tell how many instances
 * of this function is running in parallel.
 *
 * in/out	params[0].memref
 * in/out	params[1].value.a	(input) number times to calcule the hash
 * in/out	params[1].value.b	(output) max concurency
 */

#define TA_CONCURRENT_CMD_BUSY_LOOP	0

/*
 * Calculates a sha-256 hash over param[2].memref and stores the result in
 * params[3].memref. params[0].memref should contain a struct
 * ta_concurent_shm which can be used to tell how many instances of this
 * function is running in parallel.
 *
 * in/out	params[0].memref
 * in/out	params[1].value.a	(input) number times to calcule the hash
 * in/out	params[1].value.b	(output) max concurency
 * in		params[2].memref
 * out		params[3].memref
 */
#define TA_CONCURRENT_CMD_SHA256	1

#endif /*TA_OS_TEST_H */
