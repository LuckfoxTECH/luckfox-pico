// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#include <tee_api.h>
#include <storage_benchmark.h>
#include <ta_storage_benchmark.h>
#include <tee_internal_api_extensions.h>

#define DEFAULT_CHUNK_SIZE (1 << 10)
#define DEFAULT_DATA_SIZE (1024)

#define SCRAMBLE(x) ((x & 0xff) ^ 0xaa)

#define ASSERT_PARAM_TYPE(pt_in, pt_expect) \
do { \
	if ((pt_in) != (pt_expect)) \
		return TEE_ERROR_BAD_PARAMETERS; \
} while (0)

static uint8_t filename[] = "BenchmarkTestFile";

static void fill_buffer(uint8_t *buf, size_t size)
{
	size_t i = 0;

	if (!buf)
		return;

	for (i = 0; i < size; i++)
		buf[i] = SCRAMBLE(i);
}

static TEE_Result verify_buffer(uint8_t *buf, size_t size)
{
	size_t i = 0;

	if (!buf)
		return TEE_ERROR_BAD_PARAMETERS;

	for (i = 0; i < size; i++) {
		uint8_t expect_data = SCRAMBLE(i);

		if (expect_data != buf[i]) {
			return TEE_ERROR_CORRUPT_OBJECT;
		}
	}

	return TEE_SUCCESS;
}

static inline uint32_t tee_time_to_ms(TEE_Time t)
{
	return t.seconds * 1000 + t.millis;
}

static inline uint32_t get_delta_time_in_ms(TEE_Time start, TEE_Time stop)
{
	return tee_time_to_ms(stop) - tee_time_to_ms(start);
}

static TEE_Result prepare_test_file(size_t data_size, uint8_t *chunk_buf,
				size_t chunk_size)
{
	size_t remain_bytes = data_size;
	TEE_Result res = TEE_SUCCESS;
	TEE_ObjectHandle object = TEE_HANDLE_NULL;

	res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE,
			filename, sizeof(filename),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE |
			TEE_DATA_FLAG_ACCESS_WRITE_META |
			TEE_DATA_FLAG_OVERWRITE,
			NULL, NULL, 0, &object);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to create persistent object, res=0x%08x",
				res);
		goto exit;
	}

	while (remain_bytes) {
		size_t write_size = 0;

		if (remain_bytes < chunk_size)
			write_size = remain_bytes;
		else
			write_size = chunk_size;
		res = TEE_WriteObjectData(object, chunk_buf, write_size);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to write data, res=0x%08x", res);
			goto exit_close_object;
		}
		remain_bytes -= write_size;
	}
exit_close_object:
	TEE_CloseObject(object);
exit:
	return res;
}

static TEE_Result test_write(TEE_ObjectHandle object, size_t data_size,
		uint8_t *chunk_buf, size_t chunk_size,
		uint32_t *spent_time_in_ms)
{
	TEE_Time start_time = { };
	TEE_Time stop_time = { };
	size_t remain_bytes = data_size;
	TEE_Result res = TEE_SUCCESS;

	TEE_GetSystemTime(&start_time);

	while (remain_bytes) {
		size_t write_size = 0;

		DMSG("Write data, remain bytes: %zu", remain_bytes);
		if (chunk_size > remain_bytes)
			write_size = remain_bytes;
		else
			write_size = chunk_size;
		res = TEE_WriteObjectData(object, chunk_buf, write_size);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to write data, res=0x%08x", res);
			goto exit;
		}
		remain_bytes -= write_size;
	}

	TEE_GetSystemTime(&stop_time);

	*spent_time_in_ms = get_delta_time_in_ms(start_time, stop_time);

	IMSG("start: %u.%u(s), stop: %u.%u(s), delta: %u(ms)",
			start_time.seconds, start_time.millis,
			stop_time.seconds, stop_time.millis,
			*spent_time_in_ms);

exit:
	return res;
}

static TEE_Result test_read(TEE_ObjectHandle object, size_t data_size,
		uint8_t *chunk_buf, size_t chunk_size,
		uint32_t *spent_time_in_ms)
{
	TEE_Time start_time = { };
	TEE_Time stop_time = { };
	size_t remain_bytes = data_size;
	TEE_Result res = TEE_SUCCESS;
	uint32_t read_bytes = 0;

	TEE_GetSystemTime(&start_time);

	while (remain_bytes) {
		size_t read_size = 0;

		DMSG("Read data, remain bytes: %zu", remain_bytes);
		if (remain_bytes < chunk_size)
			read_size = remain_bytes;
		else
			read_size = chunk_size;
		res = TEE_ReadObjectData(object, chunk_buf, read_size,
				&read_bytes);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to read data, res=0x%08x", res);
			goto exit;
		}

		remain_bytes -= read_size;
	}

	TEE_GetSystemTime(&stop_time);

	*spent_time_in_ms = get_delta_time_in_ms(start_time, stop_time);

	IMSG("start: %u.%u(s), stop: %u.%u(s), delta: %u(ms)",
			start_time.seconds, start_time.millis,
			stop_time.seconds, stop_time.millis,
			*spent_time_in_ms);

exit:
	return res;
}

static TEE_Result test_rewrite(TEE_ObjectHandle object, size_t data_size,
		uint8_t *chunk_buf, size_t chunk_size,
		uint32_t *spent_time_in_ms)
{
	TEE_Time start_time = { };
	TEE_Time stop_time = { };
	size_t remain_bytes = data_size;
	TEE_Result res = TEE_SUCCESS;
	uint32_t read_bytes = 0;

	TEE_GetSystemTime(&start_time);

	while (remain_bytes) {
		size_t write_size = 0;
		int32_t negative_chunk_size = 0;

		if (remain_bytes < chunk_size)
			write_size = remain_bytes;
		else
			write_size = chunk_size;
		negative_chunk_size = -(int32_t)write_size;

		/* Read a chunk */
		res = TEE_ReadObjectData(object, chunk_buf, write_size,
				&read_bytes);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to read data, res=0x%08x", res);
			goto exit;
		}

		if (read_bytes != write_size) {
			EMSG("Partial data read, bytes=%u", read_bytes);
			res = TEE_ERROR_CORRUPT_OBJECT;
			goto exit;
		}

		/* Seek to the position before read */
		res = TEE_SeekObjectData(object, negative_chunk_size,
				TEE_DATA_SEEK_CUR);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to seek to previous offset");
			goto exit;
		}

		/* Write a chunk*/
		res = TEE_WriteObjectData(object, chunk_buf, write_size);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to write data, res=0x%08x", res);
			goto exit;
		}

		remain_bytes -= write_size;
	}

	TEE_GetSystemTime(&stop_time);

	*spent_time_in_ms = get_delta_time_in_ms(start_time, stop_time);

	IMSG("start: %u.%u(s), stop: %u.%u(s), delta: %u(ms)",
			start_time.seconds, start_time.millis,
			stop_time.seconds, stop_time.millis,
			*spent_time_in_ms);

exit:
	return res;
}

static TEE_Result verify_file_data(TEE_ObjectHandle object, size_t data_size,
		uint8_t *chunk_buf, size_t chunk_size)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t tmp_data_size = data_size;

	res = TEE_SeekObjectData(object, 0, TEE_DATA_SEEK_SET);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to seek to offset 0");
		goto exit;
	}

	TEE_MemFill(chunk_buf, 0, chunk_size);

	tmp_data_size = data_size;
	while (tmp_data_size > 0) {
		uint32_t read_bytes = 0;

		res = TEE_ReadObjectData(object, chunk_buf, chunk_size,
				&read_bytes);
		if (res != TEE_SUCCESS) {
			EMSG("Failed to read data, res=0x%08x", res);
			goto exit;
		}

		if (read_bytes != chunk_size) {
			EMSG("Data size not match");
			res = TEE_ERROR_CORRUPT_OBJECT;
			goto exit;
		}

		res = verify_buffer(chunk_buf, chunk_size);
		if (res != TEE_SUCCESS) {
			EMSG("Verify data failed, res=0x%08x", res);
			goto exit;
		}

		tmp_data_size -= chunk_size;
	}

exit:
	return res;
}

static TEE_Result ta_stroage_benchmark_chunk_access_test(uint32_t nCommandID,
		uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t data_size = 0;
	size_t chunk_size = 0;
	TEE_ObjectHandle object = TEE_HANDLE_NULL;
	uint8_t *chunk_buf = NULL;
	uint32_t *spent_time_in_ms = &params[2].value.a;
	bool do_verify = false;

	ASSERT_PARAM_TYPE(param_types, TEE_PARAM_TYPES(
					TEE_PARAM_TYPE_VALUE_INPUT,
					TEE_PARAM_TYPE_VALUE_INPUT,
					TEE_PARAM_TYPE_VALUE_OUTPUT,
					TEE_PARAM_TYPE_NONE));

	data_size = params[0].value.a;
	chunk_size = params[0].value.b;
	do_verify = params[1].value.a;

	if (data_size == 0)
		data_size = DEFAULT_DATA_SIZE;

	if (chunk_size == 0)
		chunk_size = DEFAULT_CHUNK_SIZE;

	IMSG("command id: %u, test data size: %zd, chunk size: %zd\n",
			nCommandID, data_size, chunk_size);

	chunk_buf = TEE_Malloc(chunk_size, TEE_MALLOC_FILL_ZERO);
	if (!chunk_buf) {
		EMSG("Failed to allocate memory");
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}

	fill_buffer(chunk_buf, chunk_size);
	res = prepare_test_file(data_size, chunk_buf, chunk_size);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to create test file, res=0x%08x",
				res);
		goto exit_free_chunk_buf;
	}

	res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE,
			filename, sizeof(filename),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE |
			TEE_DATA_FLAG_ACCESS_WRITE_META |
			TEE_DATA_FLAG_OVERWRITE,
			&object);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to open persistent object, res=0x%08x",
				res);
		goto exit_remove_object;
	}

	switch (nCommandID) {
	case TA_STORAGE_BENCHMARK_CMD_TEST_READ:
		res = test_read(object, data_size, chunk_buf,
				chunk_size, spent_time_in_ms);
		break;

	case TA_STORAGE_BENCHMARK_CMD_TEST_WRITE:
		res = test_write(object, data_size, chunk_buf,
				chunk_size, spent_time_in_ms);
		break;

	case TA_STORAGE_BENCHMARK_CMD_TEST_REWRITE:
		res = test_rewrite(object, data_size, chunk_buf,
				chunk_size, spent_time_in_ms);
		break;

	default:
		res = TEE_ERROR_BAD_PARAMETERS;
	}

	if (res != TEE_SUCCESS)
		goto exit_remove_object;

	if (do_verify)
		res = verify_file_data(object, data_size,
				chunk_buf, chunk_size);


exit_remove_object:
	TEE_CloseAndDeletePersistentObject1(object);
exit_free_chunk_buf:
	TEE_Free(chunk_buf);
exit:

	return res;
}

TEE_Result ta_storage_benchmark_cmd_handler(uint32_t nCommandID,
		uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;

	switch (nCommandID) {
	case TA_STORAGE_BENCHMARK_CMD_TEST_READ:
	case TA_STORAGE_BENCHMARK_CMD_TEST_WRITE:
	case TA_STORAGE_BENCHMARK_CMD_TEST_REWRITE:
		res = ta_stroage_benchmark_chunk_access_test(nCommandID,
				param_types, params);
		break;

	default:
		res = TEE_ERROR_BAD_PARAMETERS;
	}

	return res;
}

