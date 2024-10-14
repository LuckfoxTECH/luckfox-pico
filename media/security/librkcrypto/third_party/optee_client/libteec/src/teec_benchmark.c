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
#include <err.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <tee_bench.h>
#include <tee_client_api.h>
#include <unistd.h>

#include "teec_benchmark.h"

struct tee_ts_global *bench_ts_global;
static const TEEC_UUID pta_benchmark_uuid = PTA_BENCHMARK_UUID;

static TEEC_Context bench_ctx;
static TEEC_Session bench_sess;

static pthread_mutex_t teec_bench_mu = PTHREAD_MUTEX_INITIALIZER;

/* Cycle counter */
static inline uint64_t read_ccounter(void)
{
	uint64_t ccounter = 0;
#ifdef __aarch64__
	asm volatile("mrs %0, PMCCNTR_EL0" : "=r"(ccounter));
#else
	asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(ccounter));
#endif
	return ccounter * TEE_BENCH_DIVIDER;
}

static TEEC_Result benchmark_pta_open(void)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t ret_orig = 0;

	res = TEEC_InitializeContext(NULL, &bench_ctx);
	if (res != TEEC_SUCCESS)
		return res;

	res = TEEC_OpenSession(&bench_ctx, &bench_sess,
			&pta_benchmark_uuid,
			TEEC_LOGIN_PUBLIC, NULL, NULL, &ret_orig);
	if (res != TEEC_SUCCESS) {
		TEEC_FinalizeContext(&bench_ctx);
		return res;
	}

	return res;
}

static void benchmark_pta_close(void)
{
	TEEC_CloseSession(&bench_sess);
	TEEC_FinalizeContext(&bench_ctx);
}

static TEEC_Result benchmark_get_bench_buf_paddr(uint64_t *paddr_ts_buf,
				uint64_t *size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t ret_orig = 0;
	TEEC_Operation op;

	memset(&op, 0, sizeof(op));

	res = benchmark_pta_open();
	if (res != TEEC_SUCCESS)
		return res;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE,
					TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(&bench_sess, BENCHMARK_CMD_GET_MEMREF,
					&op, &ret_orig);
	if (res != TEEC_SUCCESS)
		return res;

	*paddr_ts_buf = op.params[0].value.a;
	*size = op.params[0].value.b;

	benchmark_pta_close();

	return res;
}

static void *mmap_paddr(intptr_t paddr, uint64_t size)
{
	int devmem = 0;
	off_t offset = 0;
	off_t page_addr = 0;
	intptr_t *hw_addr = NULL;

	devmem = open("/dev/mem", O_RDWR);
	if (!devmem)
		return NULL;

	offset = (off_t)paddr % getpagesize();
	page_addr = (off_t)(paddr - offset);

	hw_addr = (intptr_t *)mmap(0, size, PROT_READ|PROT_WRITE,
					MAP_SHARED, devmem, page_addr);
	if (hw_addr == MAP_FAILED) {
		close(devmem);
		return NULL;
	}

	close(devmem);
	return (hw_addr + offset);
}

/* check if we are in benchmark mode */
static bool benchmark_check_mode(void)
{
	uint64_t ts_buf_raw = 0;
	uint64_t ts_buf_size = 0;
	bool res = true;

	if (!bench_ts_global) {
		/* receive buffer from Benchmark PTA and register it */
		benchmark_get_bench_buf_paddr(&ts_buf_raw, &ts_buf_size);
		if (ts_buf_raw && ts_buf_size) {
			bench_ts_global = mmap_paddr(ts_buf_raw, ts_buf_size);
			res = (bench_ts_global) ? true : false;
		} else {
			res = false;
		}
	}

	return res;
}

/* Adding timestamp to buffer */
void bm_timestamp(void)
{
	struct tee_ts_cpu_buf *cpu_buf = NULL;
	uint64_t ts_i = 0;
	void *ret_addr = NULL;
	uint32_t cur_cpu = 0;
	int ret = 0;
	cpu_set_t cpu_set_old;
	cpu_set_t cpu_set_tmp;
	struct tee_time_st ts_data;

	memset(&cpu_set_old, 0, sizeof(cpu_set_old));
	memset(&cpu_set_tmp, 0, sizeof(cpu_set_tmp));
	memset(&ts_data, 0, sizeof(ts_data));

	if (pthread_mutex_trylock(&teec_bench_mu))
		return;

	if (!benchmark_check_mode())
		goto error;

	CPU_ZERO(&cpu_set_old);
	ret = sched_getaffinity(0, sizeof(cpu_set_old), &cpu_set_old);
	if (ret)
		goto error;

	/* stick to the same core while putting timestamp */
	cur_cpu = sched_getcpu();
	CPU_ZERO(&cpu_set_tmp);
	CPU_SET(cur_cpu, &cpu_set_tmp);
	ret = sched_setaffinity(0, sizeof(cpu_set_tmp), &cpu_set_tmp);
	if (ret)
		goto error;

	/* fill timestamp data */
	if (cur_cpu >= bench_ts_global->cores) {
		ret = sched_setaffinity(0, sizeof(cpu_set_old), &cpu_set_old);
		goto error;
	}

	ret_addr = __builtin_return_address(0);

	cpu_buf = &bench_ts_global->cpu_buf[cur_cpu];
	ts_i = __sync_fetch_and_add(&cpu_buf->head, 1);
	ts_data.cnt = read_ccounter();
	ts_data.addr = (uintptr_t)ret_addr;
	ts_data.src = TEE_BENCH_CLIENT;
	cpu_buf->stamps[ts_i & TEE_BENCH_MAX_MASK] = ts_data;

	/* set back affinity mask */
	sched_setaffinity(0, sizeof(cpu_set_old), &cpu_set_old);

error:
	pthread_mutex_unlock(&teec_bench_mu);
}

