/*
 * Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEE_BENCH_H
#define TEE_BENCH_H

#include <inttypes.h>

#define PTA_BENCHMARK_UUID \
		{ 0x0b9a63b0, 0xb4c6, 0x4c85, \
			{ 0xa2, 0x84, 0xa2, 0x28, 0xef, 0x54, 0x7b, 0x4e } }

#define BENCHMARK_CMD(id)	(0xFA190000 | ((id) & 0xFFFF))
#define BENCHMARK_CMD_REGISTER_MEMREF	BENCHMARK_CMD(1)
#define BENCHMARK_CMD_GET_MEMREF		BENCHMARK_CMD(2)
#define BENCHMARK_CMD_UNREGISTER		BENCHMARK_CMD(3)

/*
 * Cycle count divider is enabled (in PMCR),
 * CCNT value is incremented every 64th clock cycle
 */
#define TEE_BENCH_DIVIDER		64
/* max amount of timestamps per buffer */
#define TEE_BENCH_MAX_STAMPS	32
#define TEE_BENCH_MAX_MASK		(TEE_BENCH_MAX_STAMPS - 1)

/* OP-TEE susbsystems ids */
#define TEE_BENCH_CLIENT	0x10000000
#define TEE_BENCH_KMOD		0x20000000
#define TEE_BENCH_CORE		0x30000000
#define TEE_BENCH_UTEE		0x40000000
#define TEE_BENCH_DUMB_TA	0xF0000001

/* storing timestamp */
struct tee_time_st {
	uint64_t cnt;	/* stores value from CNTPCT register */
	uint64_t addr;	/* stores value from program counter register */
	uint64_t src;	/* OP-TEE subsystem id */
};

/* per-cpu circular buffer for timestamps */
struct tee_ts_cpu_buf {
	uint64_t head;
	uint64_t tail;
	struct tee_time_st stamps[TEE_BENCH_MAX_STAMPS];
};

/* memory layout for shared memory, where timestamps will be stored */
struct tee_ts_global {
	uint64_t cores;
	struct tee_ts_cpu_buf cpu_buf[];
};
#endif /* TEE_BENCH_H */
