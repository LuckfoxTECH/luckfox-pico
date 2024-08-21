// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <adbg.h>
#include <xtest_test.h>
#include <xtest_helpers.h>

#include <tee_client_api.h>
#include <ta_storage.h>
#include <tee_api_defines.h>
#include <tee_api_defines_extensions.h>
#include <tee_api_types.h>
#include <util.h>

#define DEFINE_TEST_MULTIPLE_STORAGE_IDS(test_name)			     \
static void test_name(ADBG_Case_t *c)					     \
{									     \
	size_t i;							     \
									     \
	for (i = 0; i < ARRAY_SIZE(storage_ids); i++) {			     \
		Do_ADBG_BeginSubCase(c, "Storage id: %08x", storage_ids[i]); \
		test_name##_single(c, storage_ids[i]);			     \
		Do_ADBG_EndSubCase(c, "Storage id: %08x", storage_ids[i]);   \
	}								     \
}

static uint32_t storage_ids[] = {
	TEE_STORAGE_PRIVATE,
#ifdef CFG_REE_FS
	TEE_STORAGE_PRIVATE_REE,
#endif
#ifdef CFG_RPMB_FS
	TEE_STORAGE_PRIVATE_RPMB,
#endif
};

static uint8_t file_00[] = {
	0x00, 0x6E, 0x04, 0x57, 0x08, 0xFB, 0x71, 0x96,
	0xF0, 0x2E, 0x55, 0x3D, 0x02, 0xC3, 0xA6, 0x92,
	0xE9, 0xC3, 0xEF, 0x8A, 0xB2, 0x34, 0x53, 0xE6,
	0xF0, 0x74, 0x9C, 0xD6, 0x36, 0xE7, 0xA8, 0x8E
};

static uint8_t file_01[] = {
	0x01, 0x00
};

static uint8_t file_02[] = {
	0x02, 0x11, 0x02
};

static uint8_t file_03[] = {
	0x03, 0x13, 0x03
};

static uint8_t file_04[] = {
	0x00, 0x01, 0x02
};

static uint8_t data_00[] = {
	0x00, 0x6E, 0x04, 0x57, 0x08, 0xFB, 0x71, 0x96,
	0x00, 0x2E, 0x55, 0x3D, 0x02, 0xC3, 0xA6, 0x92,
	0x00, 0xC3, 0xEF, 0x8A, 0xB2, 0x34, 0x53, 0xE6,
	0x00, 0x74, 0x9C, 0xD6, 0x36, 0xE7, 0xA8, 0x00
};

static uint8_t data_01[] = {
	0x01, 0x6E, 0x04, 0x57, 0x08, 0xFB, 0x71, 0x96,
	0x01, 0x2E, 0x55, 0x3D, 0x02, 0xC3, 0xA6, 0x92,
	0x01, 0xC3, 0xEF, 0x8A, 0xB2, 0x34, 0x53, 0xE6,
	0x01, 0x74, 0x9C, 0xD6, 0x36, 0xE7, 0xA8, 0x01
};

static uint32_t fs_id_for_tee_storage_private(void)
{
#if defined(CFG_REE_FS)
	return TEE_STORAGE_PRIVATE_REE;
#elif defined(CFG_RPMB_FS)
	return TEE_STORAGE_PRIVATE_RPMB;
#endif
}

static uint32_t real_id_for(uint32_t id)
{
	if (id == TEE_STORAGE_PRIVATE)
		return fs_id_for_tee_storage_private();
	return id;
}

static bool storage_is(uint32_t id1, uint32_t id2)
{
	return (real_id_for(id1) == real_id_for(id2));
}

static TEEC_Result fs_open(TEEC_Session *sess, void *id, uint32_t id_size,
			   uint32_t flags, uint32_t *obj, uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_size;
	op.params[1].value.a = flags;
	op.params[1].value.b = 0;
	op.params[2].value.a = storage_id;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_OPEN, &op, &org);

	if (res == TEEC_SUCCESS)
		*obj = op.params[1].value.b;

	return res;
}

static TEEC_Result fs_create(TEEC_Session *sess, void *id, uint32_t id_size,
			     uint32_t flags, uint32_t attr, void *data,
			     uint32_t data_size, uint32_t *obj,
			     uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_size;
	op.params[1].value.a = flags;
	op.params[1].value.b = 0;
	op.params[2].value.a = attr;
	op.params[2].value.b = storage_id;
	op.params[3].tmpref.buffer = data;
	op.params[3].tmpref.size = data_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT);

	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_CREATE, &op, &org);

	if (res == TEEC_SUCCESS)
		*obj = op.params[1].value.b;

	return res;
}

static TEEC_Result fs_create_overwrite(TEEC_Session *sess, void *id,
				       uint32_t id_size, uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = id;
	op.params[0].tmpref.size = id_size;
	op.params[1].value.a = storage_id;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_CREATE_OVERWRITE, &op, &org);

	return res;
}

static TEEC_Result fs_close(TEEC_Session *sess, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_CLOSE, &op, &org);
}

static TEEC_Result fs_read(TEEC_Session *sess, uint32_t obj, void *data,
			   uint32_t data_size, uint32_t *count)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = data;
	op.params[0].tmpref.size = data_size;
	op.params[1].value.a = obj;
	op.params[1].value.b = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_READ, &op, &org);

	if (res == TEEC_SUCCESS)
		*count = op.params[1].value.b;

	return res;
}

static TEEC_Result fs_write(TEEC_Session *sess, uint32_t obj, void *data,
			    uint32_t data_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].tmpref.buffer = data;
	op.params[0].tmpref.size = data_size;
	op.params[1].value.a = obj;
	op.params[1].value.b = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_WRITE, &op, &org);
}

static TEEC_Result fs_seek(TEEC_Session *sess, uint32_t obj, int32_t offset,
			   int32_t whence)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[0].value.b = *(uint32_t *)&offset;
	op.params[1].value.a = whence;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INOUT,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_SEEK, &op, &org);
}

static TEEC_Result fs_unlink(TEEC_Session *sess, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_UNLINK, &op, &org);
}

static TEEC_Result fs_trunc(TEEC_Session *sess, uint32_t obj, uint32_t len)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[0].value.b = len;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_TRUNC, &op, &org);
}

static TEEC_Result fs_rename(TEEC_Session *sess, uint32_t obj, void *id,
			     uint32_t id_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[1].tmpref.buffer = id;
	op.params[1].tmpref.size = id_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_RENAME, &op, &org);
}

static TEEC_Result fs_alloc_enum(TEEC_Session *sess, uint32_t *e)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_ALLOC_ENUM, &op, &org);

	if (res == TEEC_SUCCESS)
		*e = op.params[0].value.a;

	return res;
}

static TEEC_Result fs_reset_enum(TEEC_Session *sess, uint32_t e)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = e;
	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_RESET_ENUM, &op, &org);

	return res;
}

static TEEC_Result fs_free_enum(TEEC_Session *sess, uint32_t e)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	op.params[0].value.a = e;

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_FREE_ENUM, &op, &org);
}

static TEEC_Result fs_start_enum(TEEC_Session *sess, uint32_t e,
				 uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = e;
	op.params[0].value.b = storage_id;

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_START_ENUM, &op, &org);
}

static TEEC_Result fs_next_enum(TEEC_Session *sess, uint32_t e, void *obj_info,
				size_t info_size, void *id, uint32_t id_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
	if (obj_info && info_size)
		op.paramTypes |= (TEEC_MEMREF_TEMP_OUTPUT << 4);

	op.params[0].value.a = e;
	op.params[1].tmpref.buffer = obj_info;
	op.params[1].tmpref.size = info_size;
	op.params[2].tmpref.buffer = id;
	op.params[2].tmpref.size = id_size;

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_NEXT_ENUM, &op, &org);
}

static TEEC_Result fs_restrict_usage(TEEC_Session *sess, uint32_t obj,
				     uint32_t obj_usage)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;
	op.params[0].value.b = obj_usage;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_RESTRICT_USAGE,
				  &op, &org);
}

static TEEC_Result fs_alloc_obj(TEEC_Session *sess, uint32_t obj_type,
				     uint32_t max_key_size, uint32_t *obj)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj_type;
	op.params[0].value.b = max_key_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, TA_STORAGE_CMD_ALLOC_OBJ, &op, &org);
	*obj = op.params[1].value.a;
	return res;
}

static TEEC_Result fs_free_obj(TEEC_Session *sess, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_FREE_OBJ, &op, &org);
}

static TEEC_Result fs_reset_obj(TEEC_Session *sess, uint32_t obj)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.params[0].value.a = obj;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_RESET_OBJ, &op, &org);
}

static TEEC_Result fs_get_obj_info(TEEC_Session *sess, uint32_t obj,
				void *obj_info, size_t info_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					TEEC_MEMREF_TEMP_OUTPUT,
					TEEC_NONE, TEEC_NONE);

	op.params[0].value.a = obj;
	op.params[1].tmpref.buffer = obj_info;
	op.params[1].tmpref.size = info_size;

	return TEEC_InvokeCommand(sess, TA_STORAGE_CMD_GET_OBJ_INFO, &op, &org);
}

/* trunc */
static void test_truncate_file_length(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_READ |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* trunc */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_trunc(&sess, obj, 10)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 5, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	(void)ADBG_EXPECT_BUFFER(c, &data_00[5], 5, out, count);

	/* close */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	/* open */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE |
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 5, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	(void)ADBG_EXPECT_BUFFER(c, &data_00[5], 5, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}

/* extend */
static void test_extend_file_length(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint8_t expect[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_READ |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* extend */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_trunc(&sess, obj, 40)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 30, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	expect[0] = data_00[30];
	expect[1] = data_00[31];
	(void)ADBG_EXPECT_BUFFER(c, &expect[0], 10, out, count);

	/* close */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	/* open */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE |
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 30, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	expect[0] = data_00[30];
	expect[1] = data_00[31];
	(void)ADBG_EXPECT_BUFFER(c, &expect[0], 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}

/* file hole */
static void test_file_hole(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint8_t expect[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_READ |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 80, TEE_DATA_SEEK_SET)))
		goto exit;

	/* write */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_write(&sess, obj, data_00,
			sizeof(data_00))))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 74, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	expect[6] = data_00[0];
	expect[7] = data_00[1];
	expect[8] = data_00[2];
	expect[9] = data_00[3];
	(void)ADBG_EXPECT_BUFFER(c, &expect[0], 10, out, count);

	/* close */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	/* open */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE |
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(
		    c, fs_seek(&sess, obj, 74, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	expect[6] = data_00[0];
	expect[7] = data_00[1];
	expect[8] = data_00[2];
	expect[9] = data_00[3];
	(void)ADBG_EXPECT_BUFFER(c, &expect[0], 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}

/* create */
static void xtest_tee_test_6001_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6001)
ADBG_CASE_DEFINE(regression, 6001, xtest_tee_test_6001,
		 "Test TEE_CreatePersistentObject");

/* open */
static void xtest_tee_test_6002_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6002)
ADBG_CASE_DEFINE(regression, 6002, xtest_tee_test_6002,
		 "Test TEE_OpenPersistentObject");

/* read */
static void xtest_tee_test_6003_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_02, sizeof(file_02),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_02, sizeof(file_02),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	(void)ADBG_EXPECT_BUFFER(c, data_01, 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6003)
ADBG_CASE_DEFINE(regression, 6003, xtest_tee_test_6003,
		 "Test TEE_ReadObjectData");

/* write */
static void xtest_tee_test_6004_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_02, sizeof(file_02),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	/* write new data */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_02, sizeof(file_02),
			TEE_DATA_FLAG_ACCESS_WRITE, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_write(&sess, obj, data_00, sizeof(data_00))))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_02, sizeof(file_02),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	(void)ADBG_EXPECT_BUFFER(c, data_00, 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6004)
ADBG_CASE_DEFINE(regression, 6004, xtest_tee_test_6004,
		 "Test TEE_WriteObjectData");

/* seek */
static void xtest_tee_test_6005_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_READ |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* seek */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_seek(&sess, obj, 10, TEE_DATA_SEEK_SET)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	(void)ADBG_EXPECT_BUFFER(c, &data_00[10], 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6005)
ADBG_CASE_DEFINE(regression, 6005, xtest_tee_test_6005,
		 "Test TEE_SeekObjectData");

/* unlink */
static void xtest_tee_test_6006_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* del & close */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

	/* check result */
	if (!ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_ITEM_NOT_FOUND,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_READ, &obj, storage_id)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6006)
ADBG_CASE_DEFINE(regression, 6006, xtest_tee_test_6006,
		 "Test TEE_CloseAndDeletePersistentObject");

static void xtest_tee_test_6007_single(ADBG_Case_t *c, uint32_t storage_id)
{
	Do_ADBG_BeginSubCase(c, "Test truncate file length");
	test_truncate_file_length(c, storage_id);
	Do_ADBG_EndSubCase(c, "Test truncate file length");

	Do_ADBG_BeginSubCase(c, "Test extend file length");
	test_extend_file_length(c, storage_id);
	Do_ADBG_EndSubCase(c, "Test extend file length");

	Do_ADBG_BeginSubCase(c, "Test file hole");
	test_file_hole(c, storage_id);
	Do_ADBG_EndSubCase(c, "Test file hole");
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6007)
ADBG_CASE_DEFINE(regression, 6007, xtest_tee_test_6007,
		 "Test TEE_TruncateObjectData");

static void xtest_tee_test_6008_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_02, sizeof(file_02),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_02, sizeof(file_02),
			TEE_DATA_FLAG_ACCESS_WRITE |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	/* write new data */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_write(&sess, obj, data_00, sizeof(data_00))))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_rename(&sess, obj, file_03, sizeof(file_03))))
		goto exit;

	/* close */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_03, sizeof(file_03),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* check buffer */
	(void)ADBG_EXPECT_BUFFER(c, data_00, 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6008)
ADBG_CASE_DEFINE(regression, 6008, xtest_tee_test_6008,
		 "Test TEE_RenamePersistentObject");

static void xtest_tee_test_6009_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t obj0 = 0;
	uint32_t obj1 = 0;
	uint32_t obj2 = 0;
	uint32_t e = 0;
	uint8_t info[200] = { };
	uint8_t id[200] = { };
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	/* create file 00 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj0, storage_id)))
		goto exit;

	/* create file 01 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj1, storage_id)))
		goto exit;

	/* create file 02 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_02, sizeof(file_02),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj2, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj0)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj1)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj2)))
		goto exit;

	/* iterate */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_alloc_enum(&sess, &e)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_start_enum(&sess, e, storage_id)))
		goto exit;

	/* get 00 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_next_enum(&sess, e, info, sizeof(info), id, sizeof(id))))
		goto exit;

	/* get 01 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_next_enum(&sess, e, NULL, 0, id, sizeof(id))))
		goto exit;

	/* get 02 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_next_enum(&sess, e, info, sizeof(info), id, sizeof(id))))
		goto exit;

	/* we should not have more files */
	if (!ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_ITEM_NOT_FOUND,
		fs_next_enum(&sess, e, info, sizeof(info), id, sizeof(id))))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_free_enum(&sess, e)))
		goto exit;

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_00, sizeof(file_00),
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj0, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj0)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj1, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj1)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_02, sizeof(file_02),
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj2, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj2)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6009)
ADBG_CASE_DEFINE(regression, 6009, xtest_tee_test_6009,
	"Test TEE Internal API Persistent Object Enumeration Functions");

static void xtest_tee_test_6010_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t orig = 0;
	uint32_t o1 = 0;
	uint32_t o2 = 0;
	uint32_t e = 0;
	uint32_t f = 0;
	uint8_t data[1024] = { };
	uint8_t out[1024] = { };
	uint32_t n = 0;

	for (n = 0; n < ARRAY_SIZE(data); n++)
		data[n] = n;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	Do_ADBG_BeginSubCase(c, "CreatePersistentObject AccessConflict");

	o1 = TEE_HANDLE_NULL;
	o2 = TEE_HANDLE_NULL;
	f = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE |
	    TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_SHARE_READ |
	    TEE_DATA_FLAG_SHARE_WRITE | TEE_DATA_FLAG_OVERWRITE;

	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o1, storage_id));

	f = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE;
	ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_ACCESS_CONFLICT,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o2, storage_id));

	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o1));
	if (o2)
		ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o2));

	Do_ADBG_EndSubCase(c, "CreatePersistentObject AccessConflict");



	Do_ADBG_BeginSubCase(c, "RestrictObjectUsage Panic");
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD,
		fs_restrict_usage(&sess, 0xffffbad0, 0xffffffff));
	Do_ADBG_EndSubCase(c, "RestrictObjectUsage Panic");

	TEEC_CloseSession(&sess);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	Do_ADBG_BeginSubCase(c, "SeekObjectData BadHandle");
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD,
		fs_seek(&sess, 0xffffbad0, 5, TEE_DATA_SEEK_SET));
	Do_ADBG_EndSubCase(c, "SeekObjectData BadHandle");

	TEEC_CloseSession(&sess);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	Do_ADBG_BeginSubCase(c, "SeekObjectData NotPersist");
	o1 = 0;
	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_alloc_obj(&sess, TEE_TYPE_AES, 256, &o1));
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD,
		fs_seek(&sess, o1, 5, TEE_DATA_SEEK_SET));
	Do_ADBG_EndSubCase(c, "SeekObjectData NotPersist");

	TEEC_CloseSession(&sess);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	Do_ADBG_BeginSubCase(c, "SeekWriteRead");
	o1 = 0;
	f = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE |
	    TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_SHARE_READ |
	    TEE_DATA_FLAG_SHARE_WRITE | TEE_DATA_FLAG_OVERWRITE;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o1, storage_id)))
		goto seek_write_read_out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_seek(&sess, o1, 2, TEE_DATA_SEEK_SET)))
		goto seek_write_read_out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_seek(&sess, o1, 0, TEE_DATA_SEEK_END)))
		goto seek_write_read_out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_write(&sess, o1, data, sizeof(data))))
		goto seek_write_read_out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_seek(&sess, o1, sizeof(data), TEE_DATA_SEEK_SET)))
		goto seek_write_read_out;

	memset(out, 0xab, sizeof(out));
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_read(&sess, o1, out, sizeof(out), &n)))
		goto seek_write_read_out;

	ADBG_EXPECT_BUFFER(c, data, sizeof(data), out, n);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_seek(&sess, o1, 10, TEE_DATA_SEEK_END)))
		goto seek_write_read_out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_read(&sess, o1, out, sizeof(out), &n)))
		goto seek_write_read_out;
	ADBG_EXPECT_COMPARE_UNSIGNED(c, n, ==, 0);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_seek(&sess, o1, -(int32_t)sizeof(data) / 2,
			TEE_DATA_SEEK_END)))
		goto seek_write_read_out;

	memset(out, 0xab, sizeof(out) / 2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_read(&sess, o1, out, sizeof(out) / 2, &n)))
		goto seek_write_read_out;

	ADBG_EXPECT_BUFFER(c,
		data + sizeof(data) / 2, sizeof(data) / 2,
		out, n);

seek_write_read_out:
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o1));
	Do_ADBG_EndSubCase(c, "SeekWriteRead");

	Do_ADBG_BeginSubCase(c, "Rename Access Conflict");

	o1 = TEE_HANDLE_NULL;
	o2 = TEE_HANDLE_NULL;
	f = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE |
	    TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_SHARE_READ |
	    TEE_DATA_FLAG_SHARE_WRITE | TEE_DATA_FLAG_OVERWRITE;
	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o1, storage_id));
	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01), f, 0, data,
			  sizeof(data) / 2, &o2, storage_id));

	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_ACCESS_CONFLICT,
		fs_rename(&sess, o2, file_00, sizeof(file_00)));

	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o1));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o2));

	Do_ADBG_EndSubCase(c, "Rename Access Conflict");

	Do_ADBG_BeginSubCase(c, "Rename Access Conflict 2");

	o1 = TEE_HANDLE_NULL;
	o2 = TEE_HANDLE_NULL;
	f = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE |
	    TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_SHARE_READ |
	    TEE_DATA_FLAG_SHARE_WRITE | TEE_DATA_FLAG_OVERWRITE;
	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o1, storage_id));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, o1));

	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01), f, 0, data,
			  sizeof(data) / 2, &o2, storage_id));
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_ACCESS_CONFLICT,
		fs_rename(&sess, o2, file_00, sizeof(file_00)));

	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o1, storage_id));

	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o1));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o2));

	Do_ADBG_EndSubCase(c, "Rename Access Conflict 2");

	Do_ADBG_BeginSubCase(c, "AllocPersistentObjectEnumerator + "
				"ResetPersistentObjectEnumerator");
	e = TEE_HANDLE_NULL;
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_alloc_enum(&sess, &e));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_reset_enum(&sess, e));
	Do_ADBG_EndSubCase(c, "AllocPersistentObjectEnumerator + "
			      "ResetPersistentObjectEnumerator");

	Do_ADBG_BeginSubCase(c, "StartPersistentObjectEnumerator ItemNotFound");
	e = TEE_HANDLE_NULL;
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_alloc_enum(&sess, &e));
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_ITEM_NOT_FOUND,
		fs_next_enum(&sess, e, NULL, 0, out, sizeof(out)));
	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_ITEM_NOT_FOUND,
		fs_start_enum(&sess, e, storage_id));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_free_enum(&sess, e));
	Do_ADBG_EndSubCase(c, "StartPersistentObjectEnumerator ItemNotFound");

	Do_ADBG_BeginSubCase(c, "RenamePersistent ReadWrite");
	o1 = TEE_HANDLE_NULL;
	f = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE |
	    TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_SHARE_READ |
	    TEE_DATA_FLAG_SHARE_WRITE | TEE_DATA_FLAG_OVERWRITE;
	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_00, sizeof(file_00), f, 0, data,
			  sizeof(data), &o1, storage_id));
	ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_rename(&sess, o1, file_01, sizeof(file_01)));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, o1));
	Do_ADBG_EndSubCase(c, "RenamePersistent ReadWrite");

	Do_ADBG_BeginSubCase(c, "Close Free Reset Null");
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, TEE_HANDLE_NULL));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_free_obj(&sess, TEE_HANDLE_NULL));
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_reset_obj(&sess, TEE_HANDLE_NULL));
	Do_ADBG_EndSubCase(c, "Close Free Reset Null");

	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6010)
ADBG_CASE_DEFINE(regression, 6010, xtest_tee_test_6010, "Test Storage");

static void xtest_tee_test_6012_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t orig = 0;
	uint32_t obj = 0;

	/*
	 * create the object a first time (forced through with overwrite attribute)
	 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create_overwrite(&sess, file_04, sizeof(file_04), storage_id)))
		goto bail1;

	TEEC_CloseSession(&sess);

	/*
	 * re-create the object two times with overwrite attribute
	 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create_overwrite(&sess, file_04, sizeof(file_04),
				    storage_id)))
		goto bail1;

	/* re-create it with an object */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_04, sizeof(file_04),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_WRITE_META |
			  TEE_DATA_FLAG_OVERWRITE, 0, NULL, 0, &obj,
			  storage_id)))
		goto bail2;

	TEEC_CloseSession(&sess);

	/*
	 * re-create it again without overwrite flag: should fail and
	 * existing object should not be altered.
	 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_ACCESS_CONFLICT,
		fs_create(&sess, file_04, sizeof(file_04),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, NULL, 0, &obj,
			  storage_id)))
		goto bail2;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_04, sizeof(file_04),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj,
			storage_id)))
		goto bail1;

bail2:
	/* remove the object so that xtest 600x can be replayed */
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj));
bail1:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6012)
ADBG_CASE_DEFINE(regression, 6012, xtest_tee_test_6012,
		 "Test TEE GP TTA DS init objects");

static void xtest_tee_test_6013_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t orig = 0;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	op.params[0].value.a = storage_id;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	ADBG_EXPECT_TEEC_SUCCESS(c,
		TEEC_InvokeCommand(&sess, TA_STORAGE_CMD_KEY_IN_PERSISTENT,
				   &op, &orig));

	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6013)
ADBG_CASE_DEFINE(regression, 6013, xtest_tee_test_6013,
		 "Key usage in Persistent objects");

static void xtest_tee_test_6014_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	uint32_t orig = 0;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	op.params[0].value.a = storage_id;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	ADBG_EXPECT_TEEC_SUCCESS(c,
		TEEC_InvokeCommand(&sess, TA_STORAGE_CMD_LOOP, &op, &orig));

	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6014)
ADBG_CASE_DEFINE(regression, 6014, xtest_tee_test_6014,
		 "Loop on Persistent objects");

static void xtest_tee_test_6015_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	TEEC_Session sess2 = { };
	uint32_t orig = 0;
	uint32_t obj = 0;
	uint32_t obj2 = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess2, &storage2_ta_uuid, NULL,
					&orig)))
		goto exit2;

	/* TA #1 creates a persistent object  */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_READ |
			  TEE_DATA_FLAG_ACCESS_WRITE_META, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit;

	/* TA #2 tries to open the object created by TA #1 */
	if (!ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_ITEM_NOT_FOUND,
		fs_open(&sess2, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_READ, &obj2, storage_id)))
		goto clean;

clean:
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj));
exit:
	TEEC_CloseSession(&sess2);
exit2:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6015)
ADBG_CASE_DEFINE(regression, 6015, xtest_tee_test_6015, "Storage isolation");

struct test_6016_thread_arg {
	ADBG_Case_t *case_t;
	uint32_t storage_id;
	char file_name[8];
	TEEC_Session session;
};

static void *test_6016_thread(void *arg)
{
	struct test_6016_thread_arg *a = arg;
	TEEC_Session sess = a->session;
	uint32_t obj = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;

	/* create */
	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t,
		fs_create(&sess, a->file_name, sizeof(a->file_name),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, data_01,
			  sizeof(data_01), &obj, a->storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t, fs_close(&sess, obj)))
		goto exit;

	/* write new data */
	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t,
		fs_open(&sess, a->file_name, sizeof(a->file_name),
			TEE_DATA_FLAG_ACCESS_WRITE, &obj, a->storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t,
		fs_write(&sess, obj, data_00, sizeof(data_00))))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t, fs_close(&sess, obj)))
		goto exit;

	/* verify */
	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t,
		fs_open(&sess, a->file_name, sizeof(a->file_name),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, a->storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t,
			fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	(void)ADBG_EXPECT_BUFFER(a->case_t, data_00, 10, out, count);

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(a->case_t, fs_unlink(&sess, obj)))
		goto exit;

exit:
	return NULL;
}


#define NUM_THREADS 4
static void xtest_tee_test_6016_loop(ADBG_Case_t *c, uint32_t storage_id)
{
	struct test_6016_thread_arg arg[NUM_THREADS] = { };
	uint32_t orig = 0;
	size_t i = 0;
	size_t n = 0;
	size_t m = 0;
	pthread_t thr[NUM_THREADS] = { };

	for (m = 0; m < NUM_THREADS; m++)
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&arg[m].session,
				&storage_ta_uuid, NULL, &orig)))
			goto out;

	for (n = 0; n < NUM_THREADS; n++) {
		arg[n].case_t = c;
		arg[n].storage_id = storage_id;
		snprintf(arg[n].file_name, sizeof(arg[n].file_name),
			"file_%zu", n);
		if (!ADBG_EXPECT(c, 0, pthread_create(thr + n, NULL,
						test_6016_thread, arg + n)))
			goto out;
	}

out:
	for (i = 0; i < n; i++)
		ADBG_EXPECT(c, 0, pthread_join(thr[i], NULL));
	for (i = 0; i < m; i++)
		TEEC_CloseSession(&arg[i].session);
}

/* concurency */
static void xtest_tee_test_6016_single(ADBG_Case_t *c, uint32_t storage_id)
{
	int i = 0;
	int loops = 8;

	Do_ADBG_Log("    threads: %d, loops: %d", NUM_THREADS, loops);
	for (i = 0; i < loops; i++)
		xtest_tee_test_6016_loop(c, storage_id);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6016)
ADBG_CASE_DEFINE(regression, 6016, xtest_tee_test_6016, "Storage concurency");

static void xtest_tee_test_6017_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	TEE_ObjectInfo obj_info1 = { };
	TEE_ObjectInfo obj_info2 = { };
	uint32_t obj = 0;
	uint32_t orig = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, NULL,
			  0, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_write(&sess, obj, data_00, sizeof(data_00))))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_get_obj_info(&sess, obj, &obj_info1,
				sizeof(TEE_ObjectInfo))))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_get_obj_info(&sess, obj, &obj_info2,
				sizeof(TEE_ObjectInfo))))
		goto exit;

	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
		obj_info1.dataSize, ==, obj_info2.dataSize))
		goto exit;

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6017)
ADBG_CASE_DEFINE(regression, 6017, xtest_tee_test_6017,
		 "Test Persistent objects info");

static void xtest_tee_test_6018_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	TEE_ObjectInfo obj_info1 = { };
	TEE_ObjectInfo obj_info2 = { };
	uint32_t obj = 0;
	uint32_t orig = 0;
	uint8_t block[32 * 1024] = { };
	size_t num_blocks = 0;
	size_t block_size = 0;
	size_t n = 0;

	if (storage_is(storage_id, TEE_STORAGE_PRIVATE_RPMB)) {
		/* RPMB FS is a bit resource constrained */
		num_blocks = 10;
		block_size = 1024;
	} else {
		num_blocks = 20;
		block_size = sizeof(block);
	}

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE, 0, NULL,
			  0, &obj, storage_id)))
		goto exit;

	for (n = 0; n < num_blocks; n++) {
		memset(block, n, block_size);

		Do_ADBG_Log("writing %zu", n);
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			fs_write(&sess, obj, block, block_size)))
			goto exit;
	}

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_get_obj_info(&sess, obj, &obj_info1,
				sizeof(TEE_ObjectInfo))))
		goto exit;

	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
		obj_info1.dataSize, ==, block_size * num_blocks))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_get_obj_info(&sess, obj, &obj_info2,
				sizeof(TEE_ObjectInfo))))
		goto exit;

	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
		obj_info1.dataSize, ==, obj_info2.dataSize))
		goto exit;

	for (n = 0; n < num_blocks; n++) {
		uint8_t br[block_size];
		uint32_t count = 0;

		memset(br, 0, sizeof(br));
		memset(block, n, block_size);

		Do_ADBG_Log("reading %zu", n);
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			fs_read(&sess, obj, br, sizeof(br), &count)))
			goto exit;
		if (!ADBG_EXPECT_BUFFER(c, block, block_size, br, count))
			goto exit;
	}

	/* clean */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj)))
		goto exit;

exit:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6018)
ADBG_CASE_DEFINE(regression, 6018, xtest_tee_test_6018, "Large object");

static void xtest_tee_test_6019_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Session sess = { };
	TEEC_Session sess2 = { };
	uint32_t orig = 0;
	uint32_t obj = 0;
	uint32_t obj2 = 0;
	uint8_t out[10] = { };
	uint32_t count = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		xtest_teec_open_session(&sess2, &storage2_ta_uuid, NULL,
					&orig)))
		goto exit3;

	/* TA #1 creates a persistent object */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_WRITE_META |
			  TEE_DATA_FLAG_OVERWRITE, 0, data_00,
			  sizeof(data_00), &obj, storage_id)))
		goto exit2;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit1;

	/* TA #2 creates a persistent object */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_create(&sess2, file_01, sizeof(file_01),
			  TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_WRITE_META |
			  TEE_DATA_FLAG_OVERWRITE, 0, data_01,
			  sizeof(data_01), &obj2, storage_id)))
		goto exit1;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess2, obj2)))
		goto exit;

	/* TA #1 open and read */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess, obj, out, 10, &count)))
		goto exit;

	/* verify */
	(void)ADBG_EXPECT_BUFFER(c, data_00, 10, out, count);

	/* TA #2 open and read */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
		fs_open(&sess2, file_01, sizeof(file_01),
			TEE_DATA_FLAG_ACCESS_READ |
			TEE_DATA_FLAG_ACCESS_WRITE_META, &obj2, storage_id)))
		goto exit;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_read(&sess2, obj2, out, 10, &count)))
		goto exit;

	/* verify */
	(void)ADBG_EXPECT_BUFFER(c, data_01, 10, out, count);

exit:
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess2, obj2));
exit1:
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj));
exit2:
	TEEC_CloseSession(&sess2);
exit3:
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6019)
ADBG_CASE_DEFINE(regression, 6019, xtest_tee_test_6019, "Storage independence");

/*
 * According to the GP spec V1.1, the object_id in create/open/rename
 * functions is not allowed to reside in the shared memory.
 *
 * The function below replicates fs_open/fs_create/fs_rename but using
 * specific commands to ask the TA to use the client object ID buffer
 * from the shared memory when accessing the object through target APIs.
 * The TA is not expected to use such references and gets killed by the TEE.
 */
static TEEC_Result fs_access_with_bad_object_id_ref(TEEC_Session *sess,
						    uint32_t command,
						    void *id, uint32_t id_size,
						    uint32_t flags,
						    uint32_t attr,
						    void *data, uint32_t data_size,
						    uint32_t *obj,
						    uint32_t storage_id)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t org = 0;

	switch (command) {
	case TA_STORAGE_CMD_OPEN_ID_IN_SHM:
		op.params[0].tmpref.buffer = id;
		op.params[0].tmpref.size = id_size;
		op.params[1].value.a = flags;
		op.params[1].value.b = 0;
		op.params[2].value.a = storage_id;

		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
						 TEEC_VALUE_INOUT,
						 TEEC_VALUE_INPUT,
						 TEEC_NONE);
		break;
	case TA_STORAGE_CMD_CREATE_ID_IN_SHM:
		op.params[0].tmpref.buffer = id;
		op.params[0].tmpref.size = id_size;
		op.params[1].value.a = flags;
		op.params[1].value.b = 0;
		op.params[2].value.a = attr;
		op.params[2].value.b = storage_id;
		op.params[3].tmpref.buffer = data;
		op.params[3].tmpref.size = data_size;

		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
						 TEEC_VALUE_INOUT,
						 TEEC_VALUE_INPUT,
						 TEEC_MEMREF_TEMP_INPUT);
		break;
	case TA_STORAGE_CMD_CREATEOVER_ID_IN_SHM:
		op.params[0].tmpref.buffer = id;
		op.params[0].tmpref.size = id_size;
		op.params[1].value.a = storage_id;

		op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
						 TEEC_VALUE_INPUT,
						 TEEC_NONE, TEEC_NONE);
		break;
	case TA_STORAGE_CMD_RENAME_ID_IN_SHM:
		op.params[0].value.a = *obj;
		op.params[1].tmpref.buffer = id;
		op.params[1].tmpref.size = id_size;

		op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						 TEEC_MEMREF_TEMP_INPUT,
						 TEEC_NONE, TEEC_NONE);
		break;
	default:
		return TEE_ERROR_GENERIC;
	}

	res = TEEC_InvokeCommand(sess, command, &op, &org);

	switch (command) {
	case TA_STORAGE_CMD_OPEN_ID_IN_SHM:
	case TA_STORAGE_CMD_CREATE_ID_IN_SHM:
		if (res == TEEC_SUCCESS)
			*obj = op.params[1].value.b;
		break;
	default:
		break;
	}

	return res;
}

static void xtest_tee_test_6020_single(ADBG_Case_t *c, uint32_t storage_id)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Session sess = { };
	uint32_t orig = 0;
	uint32_t obj = 0;

	/*
	 * Invalid open request from the TA (object ID reference in SHM)
	 */
	res = xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return;

	res = fs_create(&sess, file_01, sizeof(file_01),
				TEE_DATA_FLAG_ACCESS_WRITE |
				TEE_DATA_FLAG_ACCESS_WRITE_META |
				TEE_DATA_FLAG_OVERWRITE,
				0,
				data_00, sizeof(data_00),
				&obj,
				storage_id);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto exit1;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, fs_close(&sess, obj)))
		goto exit1;

	res = fs_access_with_bad_object_id_ref(&sess,
				TA_STORAGE_CMD_OPEN_ID_IN_SHM,
				file_01, sizeof(file_01),
				TEE_DATA_FLAG_ACCESS_WRITE |
				TEE_DATA_FLAG_ACCESS_WRITE_META,
				0,
				NULL, 0,
				&obj,
				storage_id);

	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD, res);

	/*
	 * Invalid create-overwrite request from the TA (object ID reference in SHM)
	 */
	TEEC_CloseSession(&sess);
	res = xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return;

	res = fs_access_with_bad_object_id_ref(&sess,
				TA_STORAGE_CMD_CREATEOVER_ID_IN_SHM,
				file_01, sizeof(file_01),
				0,
				0,
				NULL, 0,
				NULL,
				storage_id);

	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD, res);

	/*
	 * Invalid rename request from the TA (object ID reference in SHM)
	 */
	TEEC_CloseSession(&sess);
	res = xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return;

	res = fs_open(&sess, file_01, sizeof(file_01),
				TEE_DATA_FLAG_ACCESS_WRITE |
				TEE_DATA_FLAG_ACCESS_WRITE_META,
				&obj,
				storage_id);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto exit1;

	res = fs_access_with_bad_object_id_ref(&sess,
				TA_STORAGE_CMD_RENAME_ID_IN_SHM,
				file_01, sizeof(file_01) - 1,
				0,
				0,
				NULL, 0,
				&obj,
				0);

	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD, res);

	/*
	 * Invalid creation request from the TA (object ID reference in SHM)
	 */
	TEEC_CloseSession(&sess);
	res = xtest_teec_open_session(&sess, &storage_ta_uuid, NULL, &orig);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return;

	res = fs_open(&sess, file_01, sizeof(file_01),
				TEE_DATA_FLAG_ACCESS_WRITE |
				TEE_DATA_FLAG_ACCESS_WRITE_META,
				&obj,
				storage_id);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto exit1;

	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj));

	res = fs_access_with_bad_object_id_ref(&sess,
				TA_STORAGE_CMD_CREATE_ID_IN_SHM,
				file_01, sizeof(file_01),
				TEE_DATA_FLAG_ACCESS_WRITE |
				TEE_DATA_FLAG_ACCESS_WRITE_META |
				TEE_DATA_FLAG_OVERWRITE,
				0,
				data_00, sizeof(data_00),
				&obj,
				storage_id);

	ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_TARGET_DEAD, res);
	TEEC_CloseSession(&sess);
	return;
exit1:
	ADBG_EXPECT_TEEC_SUCCESS(c, fs_unlink(&sess, obj));
	TEEC_CloseSession(&sess);
}
DEFINE_TEST_MULTIPLE_STORAGE_IDS(xtest_tee_test_6020)
ADBG_CASE_DEFINE(regression, 6020, xtest_tee_test_6020,
		 "Object IDs in SHM (negative)");
