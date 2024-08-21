// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#include <assert.h>
#include <err.h>
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <ta_crypt.h>
#include <ta_os_test.h>
#include <utee_defines.h>

#include "xtest_helpers.h"
#include "xtest_test.h"

/* Round up the even multiple of size, size has to be a multiple of 2 */
#define ROUNDUP(v, size) (((v) + (size - 1)) & ~(size - 1))

TEEC_Context xtest_teec_ctx;

TEEC_Result xtest_teec_ctx_init(void)
{
	return TEEC_InitializeContext(xtest_tee_name, &xtest_teec_ctx);
}

TEEC_Result xtest_teec_open_session(TEEC_Session *session,
				    const TEEC_UUID *uuid, TEEC_Operation *op,
				    uint32_t *ret_orig)
{
	return TEEC_OpenSession(&xtest_teec_ctx, session, uuid,
				TEEC_LOGIN_PUBLIC, NULL, op, ret_orig);
}

void xtest_teec_ctx_deinit(void)
{
	TEEC_FinalizeContext(&xtest_teec_ctx);
}

TEEC_Result ta_crypt_cmd_allocate_operation(ADBG_Case_t *c, TEEC_Session *s,
					    TEE_OperationHandle *oph,
					    uint32_t algo, uint32_t mode,
					    uint32_t max_key_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = 0;
	op.params[0].value.b = algo;
	op.params[1].value.a = mode;
	op.params[1].value.b = max_key_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ALLOCATE_OPERATION, &op,
				 &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	if (res == TEEC_SUCCESS)
		*oph = (TEE_OperationHandle)(uintptr_t)op.params[0].value.a;

	return res;
}

TEEC_Result ta_crypt_cmd_allocate_transient_object(ADBG_Case_t *c,
						   TEEC_Session *s,
						   TEE_ObjectType obj_type,
						   uint32_t max_obj_size,
						   TEE_ObjectHandle *o)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = obj_type;
	op.params[0].value.b = max_obj_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ALLOCATE_TRANSIENT_OBJECT, &op,
				 &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
			    ret_orig);
	}

	if (res == TEEC_SUCCESS)
		*o = (TEE_ObjectHandle)(uintptr_t)op.params[1].value.a;

	return res;
}

void xtest_add_attr(size_t *attr_count, TEE_Attribute *attrs, uint32_t attr_id,
		    const void *buf, size_t len)
{
	attrs[*attr_count].attributeID = attr_id;
	attrs[*attr_count].content.ref.buffer = (void *)buf;
	attrs[*attr_count].content.ref.length = len;
	(*attr_count)++;
}

void xtest_add_attr_value(size_t *attr_count, TEE_Attribute *attrs,
			  uint32_t attr_id, uint32_t value_a, uint32_t value_b)
{
	attrs[*attr_count].attributeID = attr_id;
	attrs[*attr_count].content.value.a = value_a;
	attrs[*attr_count].content.value.b = value_b;
	(*attr_count)++;
}

struct tee_attr_packed {
	uint32_t attr_id;
	uint32_t a;
	uint32_t b;
};

TEE_Result pack_attrs(const TEE_Attribute *attrs, uint32_t attr_count,
		      uint8_t **buf, size_t *blen)
{
	struct tee_attr_packed *a = NULL;
	uint8_t *b = NULL;
	size_t bl = 0;
	size_t n = 0;

	*buf = NULL;
	*blen = 0;
	if (attr_count == 0)
		return TEE_SUCCESS;

	bl = sizeof(uint32_t) + sizeof(struct tee_attr_packed) * attr_count;
	for (n = 0; n < attr_count; n++) {
		if ((attrs[n].attributeID & TEE_ATTR_BIT_VALUE) != 0)
			continue; /* Only memrefs need to be updated */

		if (!attrs[n].content.ref.buffer)
			continue;

		/* Make room for padding */
		bl += ROUNDUP(attrs[n].content.ref.length, 4);
	}

	b = calloc(1, bl);
	if (!b)
		return TEE_ERROR_OUT_OF_MEMORY;

	*buf = b;
	*blen = bl;

	*(uint32_t *)(void *)b = attr_count;
	b += sizeof(uint32_t);
	a = (struct tee_attr_packed *)(void *)b;
	b += sizeof(struct tee_attr_packed) * attr_count;

	for (n = 0; n < attr_count; n++) {
		a[n].attr_id = attrs[n].attributeID;
		if (attrs[n].attributeID & TEE_ATTR_BIT_VALUE) {
			a[n].a = attrs[n].content.value.a;
			a[n].b = attrs[n].content.value.b;
			continue;
		}

		a[n].b = attrs[n].content.ref.length;

		if (!attrs[n].content.ref.buffer) {
			a[n].a = 0;
			continue;
		}

		memcpy(b, attrs[n].content.ref.buffer,
		       attrs[n].content.ref.length);

		/* Make buffer pointer relative to *buf */
		a[n].a = (uint32_t)(uintptr_t)(b - *buf);

		/* Round up to good alignment */
		b += ROUNDUP(attrs[n].content.ref.length, 4);
	}

	return TEE_SUCCESS;
}

TEEC_Result ta_crypt_cmd_populate_transient_object(ADBG_Case_t *c,
						   TEEC_Session *s,
						   TEE_ObjectHandle o,
						   const TEE_Attribute *attrs,
						   uint32_t attr_count)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	uint8_t *buf = NULL;
	size_t blen = 0;

	res = pack_attrs(attrs, attr_count, &buf, &blen);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	assert((uintptr_t)o <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)o;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_POPULATE_TRANSIENT_OBJECT, &op,
				 &ret_orig);

	if (res != TEEC_SUCCESS && res != TEEC_ERROR_TARGET_DEAD) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	free(buf);
	return res;
}

TEE_Result ta_crypt_cmd_set_operation_key(ADBG_Case_t *c, TEEC_Session *s,
					  TEE_OperationHandle oph,
					  TEE_ObjectHandle key)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	assert((uintptr_t)key <= UINT32_MAX);
	op.params[0].value.b = (uint32_t)(uintptr_t)key;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_SET_OPERATION_KEY, &op,
				 &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	return res;
}

TEEC_Result ta_crypt_cmd_free_transient_object(ADBG_Case_t *c, TEEC_Session *s,
					       TEE_ObjectHandle o)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)o <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)o;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_FREE_TRANSIENT_OBJECT, &op,
				 &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	return res;
}

TEEC_Result ta_crypt_cmd_derive_key(ADBG_Case_t *c, TEEC_Session *s,
				    TEE_OperationHandle oph, TEE_ObjectHandle o,
				    const TEE_Attribute *params,
				    uint32_t paramCount)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	uint8_t *buf = NULL;
	size_t blen = 0;

	res = pack_attrs(params, paramCount, &buf, &blen);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	assert((uintptr_t)oph <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	assert((uintptr_t)o <= UINT32_MAX);
	op.params[0].value.b = (uint32_t)(uintptr_t)o;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_DERIVE_KEY, &op, &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	free(buf);
	return res;
}

TEEC_Result ta_crypt_cmd_get_object_buffer_attribute(ADBG_Case_t *c,
						     TEEC_Session *s,
						     TEE_ObjectHandle o,
						     uint32_t attr_id,
						     void *buf, size_t *blen)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	assert((uintptr_t)o <= UINT32_MAX);
	op.params[0].value.a = (uint32_t)(uintptr_t)o;
	op.params[0].value.b = attr_id;

	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = *blen;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_GET_OBJECT_BUFFER_ATTRIBUTE,
				 &op, &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	if (res == TEEC_SUCCESS)
		*blen = op.params[1].tmpref.size;

	return res;
}

TEEC_Result ta_crypt_cmd_free_operation(ADBG_Case_t *c, TEEC_Session *s,
					TEE_OperationHandle oph)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = (uint32_t)(uintptr_t)oph;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_FREE_OPERATION, &op,
				 &ret_orig);

	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
	}

	return res;
}

bool ta_crypt_cmd_is_algo_supported(ADBG_Case_t *c, TEEC_Session *s,
				    uint32_t algo, uint32_t element)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;
	TEEC_Result st = TEEC_ERROR_GENERIC;

	op.params[0].value.a = algo;
	op.params[0].value.b = element;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_IS_ALGO_SUPPORTED, &op,
				 &ret_orig);
	if (res != TEEC_SUCCESS) {
		(void)ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP,
						    ret_orig);
		return res;
	}

	st = op.params[1].value.a;
	ADBG_EXPECT_TRUE(c, st == TEEC_SUCCESS ||
			 st == TEEC_ERROR_NOT_SUPPORTED);
	if (st == TEE_SUCCESS)
		return true;
	return false;
}

TEEC_Result ta_os_test_cmd_client_identity(TEEC_Session *session,
					   uint32_t *login,
					   TEEC_UUID *client_uuid)
{
	TEEC_Operation operation = { };
	TEEC_Result result = TEEC_ERROR_GENERIC;

	operation.params[1].tmpref.buffer = client_uuid;
	operation.params[1].tmpref.size = sizeof(*client_uuid);

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
						TEEC_MEMREF_TEMP_OUTPUT,
						TEEC_NONE, TEEC_NONE);

	result = TEEC_InvokeCommand(session, TA_OS_TEST_CMD_CLIENT_IDENTITY,
				    &operation, NULL);

	if (result != TEEC_SUCCESS)
		return result;

	*login = operation.params[0].value.a;

	return TEEC_SUCCESS;
}

void xtest_mutex_init(pthread_mutex_t *mutex)
{
	int e = pthread_mutex_init(mutex, NULL);

	if (e)
		errx(1, "pthread_mutex_init: %s", strerror(e));
}

void xtest_mutex_destroy(pthread_mutex_t *mutex)
{
	int e = pthread_mutex_destroy(mutex);

	if (e)
		errx(1, "pthread_mutex_destroy: %s", strerror(e));
}

void xtest_mutex_lock(pthread_mutex_t *mutex)
{
	int e = pthread_mutex_lock(mutex);

	if (e)
		errx(1, "pthread_mutex_lock: %s", strerror(e));
}

void xtest_mutex_unlock(pthread_mutex_t *mutex)
{
	int e = pthread_mutex_unlock(mutex);

	if (e)
		errx(1, "pthread_mutex_unlock: %s", strerror(e));
}

void xtest_barrier_init(pthread_barrier_t *barrier, unsigned count)
{
	int e = pthread_barrier_init(barrier, NULL, count);

	if (e)
		errx(1, "pthread_barrier_init: %s", strerror(e));
}

void xtest_barrier_destroy(pthread_barrier_t *barrier)
{
	int e = pthread_barrier_destroy(barrier);

	if (e)
		errx(1, "pthread_barrier_destroy: %s", strerror(e));
}

int xtest_barrier_wait(pthread_barrier_t *barrier)
{
	int e = pthread_barrier_wait(barrier);

	if (e && e != PTHREAD_BARRIER_SERIAL_THREAD)
		errx(1, "pthread _barrier_wait: %s", strerror(e));
	return e;
}
