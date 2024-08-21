// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2015, Linaro Limited
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xtest_test.h"
#include "xtest_helpers.h"

#include <ta_storage_benchmark.h>
#include <util.h>

#define DO_VERIFY 0
#define DEFAULT_DATA_SIZE (2 * 1024 * 1024) /* 2MB */
#define DEFAULT_CHUNK_SIZE (1 * 1024) /* 1KB */
#define DEFAULT_COUNT (10)

size_t data_size_table[] = {
	256,
	512,
	1024,
	2 * 1024,
	4 * 1024,
	16 * 1024,
	512 * 1024,
	1024 * 1024,
	0
};

static void xtest_tee_benchmark_1001(ADBG_Case_t *Case_p);
static void xtest_tee_benchmark_1002(ADBG_Case_t *Case_p);
static void xtest_tee_benchmark_1003(ADBG_Case_t *Case_p);

static TEEC_Result run_test_with_args(enum storage_benchmark_cmd cmd,
		uint32_t arg0, uint32_t arg1, uint32_t arg2,
		uint32_t arg3, uint32_t *out0, uint32_t *out1)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Session sess = { };
	uint32_t orig = 0;

	res = xtest_teec_open_session(&sess, &storage_benchmark_ta_uuid, NULL, &orig);
	if (res != TEEC_SUCCESS)
		return res;

	op.params[0].value.a = arg0;
	op.params[0].value.b = arg1;
	op.params[1].value.a = arg2;
	op.params[1].value.b = arg3;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
			TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(&sess, cmd, &op, &orig);

	if (out0)
		*out0 = op.params[2].value.a;
	if (out1)
		*out1 = op.params[2].value.b;

	TEEC_CloseSession(&sess);

	return res;
}

struct test_record {
	size_t data_size;
	float spent_time;
	float speed_in_kb;
};

static TEEC_Result run_chunk_access_test(enum storage_benchmark_cmd cmd,
		uint32_t data_size, uint32_t chunk_size, struct test_record *rec)
{
	TEE_Result res = TEEC_ERROR_GENERIC;
	uint32_t spent_time = 0;

	res = run_test_with_args(cmd, data_size, chunk_size, DO_VERIFY, 0,
				&spent_time, NULL);

	rec->data_size = data_size;
	rec->spent_time = (float)spent_time / 1000.0;
	rec->speed_in_kb = ((float)data_size / 1024.0) / rec->spent_time;

	return res;
}

static void show_test_result(struct test_record records[], size_t size)
{
	size_t i = 0;

	printf("-----------------+---------------+----------------\n");
	printf(" Data Size (B) \t | Time (s)\t | Speed (kB/s)\t \n");
	printf("-----------------+---------------+----------------\n");

	for (i = 0; i < size; i++) {
		printf(" %8zd \t | %8.3f \t | %8.3f\n",
			records[i].data_size, records[i].spent_time,
			records[i].speed_in_kb);
	}

	printf("-----------------+---------------+----------------\n");

}

static void chunk_test(ADBG_Case_t *c, enum storage_benchmark_cmd cmd)
{
	uint32_t chunk_size = DEFAULT_CHUNK_SIZE;
	struct test_record records[ARRAY_SIZE(data_size_table) - 1];
	size_t i = 0;

	for (i = 0; data_size_table[i]; i++) {
		ADBG_EXPECT_TEEC_SUCCESS(c,
			run_chunk_access_test(cmd, data_size_table[i],
				chunk_size, &records[i]));
	}

	show_test_result(records, ARRAY_SIZE(records));
}

static void xtest_tee_benchmark_1001(ADBG_Case_t *c)
{
	chunk_test(c, TA_STORAGE_BENCHMARK_CMD_TEST_WRITE);
}

static void xtest_tee_benchmark_1002(ADBG_Case_t *c)
{
	chunk_test(c, TA_STORAGE_BENCHMARK_CMD_TEST_READ);
}

static void xtest_tee_benchmark_1003(ADBG_Case_t *c)
{
	chunk_test(c, TA_STORAGE_BENCHMARK_CMD_TEST_REWRITE);
}

ADBG_CASE_DEFINE(benchmark, 1001, xtest_tee_benchmark_1001,
		"TEE Trusted Storage Performance Test (WRITE)");
ADBG_CASE_DEFINE(benchmark, 1002, xtest_tee_benchmark_1002,
		"TEE Trusted Storage Performance Test (READ)");
ADBG_CASE_DEFINE(benchmark, 1003, xtest_tee_benchmark_1003,
		"TEE Trusted Storage Performance Test (REWRITE)");
