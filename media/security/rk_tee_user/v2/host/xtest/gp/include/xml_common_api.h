/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * Copyright (c) 2020, Linaro Limited
 */

#ifndef XML_COMMON_API_H_
#define XML_COMMON_API_H_

#include <adbg.h>
#include <tee_client_api.h>

#include "xtest_helpers.h"
#include "xtest_test.h"

#define BIT(n) (1ul << (n))
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define ALLOCATE_SHARED_MEMORY(context, sharedMemory, sharedMemorySize, \
			       memoryType, exit_label) \
	res = AllocateSharedMemory(context, sharedMemory, sharedMemorySize, \
				   memoryType); \
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res)) \
		goto exit_label; \
	memset(sharedMemory->buffer, 0, sharedMemorySize);

#define ALLOCATE_AND_FILL_SHARED_MEMORY(context, sharedMemory, \
					sharedMemorySize, \
					memoryType, copySize, data, \
					exit_label) \
	res = AllocateSharedMemory(context, sharedMemory, sharedMemorySize, \
				   memoryType); \
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res)) \
		goto exit_label; \
	memset(sharedMemory->buffer, 0, sharedMemorySize); \
	if (data != NULL) { \
		memcpy(sharedMemory->buffer, data, copySize); \
	}
#define ALLOCATE_AND_FILL_SHARED_MEMORY_6(a,b,c,d,e,f) \
		        ALLOCATE_AND_FILL_SHARED_MEMORY(a,b,c,d,c,e,f)

#define SET_SHARED_MEMORY_OPERATION_PARAMETER(parameterNumber, \
					      sharedMemoryOffset, \
					      sharedMemory, \
					      sharedMemorySize) \
	op.params[parameterNumber].memref.offset = sharedMemoryOffset; \
	op.params[parameterNumber].memref.size = sharedMemorySize; \
	op.params[parameterNumber].memref.parent = sharedMemory;

/*Open session using TEEC_OpenSession and
	check the returned value and/or returned origin.*/
#define XML_OpenSession(c, context, session, destination, connectionMethod, \
			connectionData, operation, returnOrigin, expected) \
	do { \
		uint32_t ret_orig = 0; \
 \
		XML_VERIFY(c, expected, \
			   TEEC_OpenSession(context, session, destination, \
					    connectionMethod, connectionData, \
					    operation, &ret_orig)); \
		if (!(unsigned long)(returnOrigin) || \
		    (unsigned long)(returnOrigin) == \
		    TEEC_ORIGIN_ANY_NOT_TRUSTED_APP) \
			ADBG_EXPECT_NOT(c, (unsigned long)returnOrigin, \
					ret_orig); \
		else \
			ADBG_EXPECT(c, (unsigned long)returnOrigin, ret_orig); \
	} while (0)

/* XML_VERIFY macro define.
 *
 * Use ADBG_EXPECT or ADBG_EXPECT_NOT depending on the expected return value.
 *
 * ADBG_EXPECT() -> IF(EXP == GOT) RETURN TRUE
 * ADBG_EXPECT() -> IF(EXP != GOT) RETURN TRUE
 */
#define XML_VERIFY(c, exp, got) \
	do { \
		if (exp == TEEC_UNDEFINED_ERROR) \
			ADBG_EXPECT_NOT(c, exp, got); \
		else \
			ADBG_EXPECT(c, exp, got); \
	} while (0)

/*Initialize context using TEEC_InitializeContext and
	check the returned value.*/
#define XML_InitializeContext(c, name, context, expected) \
	XML_VERIFY(c, expected, TEEC_InitializeContext(name, context))

#define OPERATION_TEEC_PARAM_TYPES(op, p0, p1, p2, p3) \
	do { \
		op->paramTypes = TEEC_PARAM_TYPES(p0, p1, p2, p3); \
	} while (0)

/*dummy functions*/
#define TEEC_SetUp_TEE()    /*do nothing for now*/
#define TEEC_TearDown_TEE(a)    /*do nothing for now*/
#define TEEC_SelectApp(a, b)    /*do nothing for now*/
#define TEEC_createThread(a, b) /*do nothing for now*/

struct attr_value {
	uint8_t buf[1024];
	size_t buf_size;
	uint32_t attr_id;
};

#define MAX_NUM_SAVED_ATTR_VALUES	8
static struct attr_value saved_attr[MAX_NUM_SAVED_ATTR_VALUES] __maybe_unused;


static TEEC_Result __maybe_unused
Invoke_Simple_Function(ADBG_Case_t *c __unused, TEEC_Session *sess,
		       uint32_t cmdId)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	return TEEC_InvokeCommand(sess, cmdId, &op, &org);
}

static TEEC_Result __maybe_unused
Invoke_Simple_Function_v1(ADBG_Case_t *c __unused, TEEC_Session *sess,
			  uint32_t cmd, uint32_t a, uint32_t b)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	op.params[0].value.a = a;
	op.params[0].value.b = b;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, cmd, &op, &org);
}

static TEEC_Result __maybe_unused
Invoke_Simple_Function_v2(ADBG_Case_t *c __unused, TEEC_Session *sess,
			  uint32_t cmd, uint32_t a0, uint32_t b0,
			  uint32_t a1, uint32_t b1)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	op.params[0].value.a = a0;
	op.params[0].value.b = b0;
	op.params[1].value.a = a1;
	op.params[1].value.b = b1;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);

	return TEEC_InvokeCommand(sess, cmd, &op, &org);
}

static TEEC_Result __maybe_unused
Invoke_Simple_Function_v3(ADBG_Case_t *c __unused, TEEC_Session *sess,
			  uint32_t cmd, uint32_t a0, uint32_t b0, uint32_t a1,
			  uint32_t b1, uint32_t a2, uint32_t b2)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	op.params[0].value.a = a0;
	op.params[0].value.b = b0;
	op.params[1].value.a = a1;
	op.params[1].value.b = b1;
	op.params[2].value.a = a2;
	op.params[2].value.b = b2;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);

	return TEEC_InvokeCommand(sess, cmd, &op, &org);
}

static TEEC_Result __maybe_unused
Invoke_Simple_Function_v4(ADBG_Case_t *c __unused, TEEC_Session *sess,
			  uint32_t cmd, uint32_t a0, uint32_t b0, uint32_t a1,
			  uint32_t b1, uint32_t a2, uint32_t b2, uint32_t a3,
			  uint32_t b3)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	op.params[0].value.a = a0;
	op.params[0].value.b = b0;
	op.params[1].value.a = a1;
	op.params[1].value.b = b1;
	op.params[2].value.a = a2;
	op.params[2].value.b = b2;
	op.params[3].value.a = a3;
	op.params[3].value.b = b3;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_VALUE_INPUT, TEEC_VALUE_INPUT);

	return TEEC_InvokeCommand(sess, cmd, &op, &org);
}

static TEEC_Result __maybe_unused AllocateSharedMemory(TEEC_Context *ctx,
						       TEEC_SharedMemory *shm,
						       uint32_t size,
						       uint32_t flags)
{
	shm->flags = flags;
	shm->size = size;
	return TEEC_AllocateSharedMemory(ctx, shm);
}

static TEEC_Result GetObjectBufferAttribute_helper(ADBG_Case_t *c,
						   TEEC_Session *sess,
						   size_t n,
						   uint32_t cmd, uint32_t obj,
						   uint32_t attr_id,
						   bool buffer_is_null,
						   uint32_t buffer_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEE_SUCCESS;
	uint32_t org = 0;
	static TEEC_SharedMemory shm = { };
	uint32_t memref_type = TEEC_MEMREF_TEMP_OUTPUT;

	if (!ADBG_EXPECT_COMPARE_SIGNED(c, n, <, MAX_NUM_SAVED_ATTR_VALUES))
		return TEEC_ERROR_BAD_PARAMETERS;
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, buffer_size, <=,
					sizeof(saved_attr[n].buf)))
		return TEEC_ERROR_BAD_PARAMETERS;

	if (!buffer_is_null) {
		shm.size = buffer_size;
		shm.flags = TEEC_MEM_OUTPUT;
		res = TEEC_AllocateSharedMemory(sess->ctx, &shm);
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
			return res;
		SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, &shm, shm.size);
		memref_type = TEEC_MEMREF_PARTIAL_OUTPUT;
	}

	op.params[0].value.a = obj;
	op.params[0].value.b = attr_id;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, memref_type,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmd, &op, &org);

	if (!buffer_is_null) {
		if (res) {
			memset(saved_attr + n, 0, sizeof(saved_attr[n]));
		} else {
			memcpy(saved_attr[n].buf, shm.buffer,
			       sizeof(saved_attr[n].buf));
			saved_attr[n].buf_size = op.params[1].memref.size;
			saved_attr[n].attr_id = attr_id;
		}
		TEEC_ReleaseSharedMemory(&shm);
	}

	return res;
}

static TEEC_Result __maybe_unused
Invoke_GetObjectBufferAttribute(ADBG_Case_t *c, TEEC_Session *sess,
				uint32_t cmd, uint32_t obj, uint32_t attr_id,
				bool buffer_is_null, uint32_t buffer_size)
{
	return GetObjectBufferAttribute_helper(c, sess, 0, cmd, obj, attr_id,
					       buffer_is_null, buffer_size);
}

static TEEC_Result __maybe_unused
Invoke_FreeTransientObject(ADBG_Case_t *c, TEEC_Session *sess, uint32_t cmd,
			   uint32_t obj_handle)
{
	return Invoke_Simple_Function_v1(c, sess, cmd, obj_handle, 0);
}
#endif /* XML_COMMON_API_H_ */
