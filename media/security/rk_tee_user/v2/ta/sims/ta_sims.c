// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include <ta_sims.h>
#include <tee_api.h>

#define TA_SIMS_MAX_STORAGE 4

struct sims_bucket {
	uint32_t size;
	void *data;
};

struct sims_session {
	TEE_TASessionHandle sess;
	uint32_t counter;
	uint32_t array[2048];
};

static struct sims_bucket storage[TA_SIMS_MAX_STORAGE] = { {0} };

static uint32_t counter;

TEE_Result sims_open_ta_session(void *session_context, uint32_t param_types,
				TEE_Param params[4])
{
	TEE_UUID *uuid = NULL;
	TEE_Result res = TEE_SUCCESS;
	TEE_TASessionHandle sess = TEE_HANDLE_NULL;
	uint32_t ret_orig = 0;
	struct sims_session *ctx = (struct sims_session *)session_context;

	if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE))
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[0].memref.size != sizeof(*uuid))
		return TEE_ERROR_BAD_PARAMETERS;

	uuid = TEE_Malloc(sizeof(*uuid), TEE_MALLOC_FILL_ZERO);
	if (!uuid)
		return TEE_ERROR_OUT_OF_MEMORY;

	TEE_MemMove(uuid, params[0].memref.buffer, params[0].memref.size);

	res = TEE_OpenTASession(uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
				&sess, &ret_orig);
	TEE_Free(uuid);
	if (res != TEE_SUCCESS)
		return res;

	ctx->sess = sess;
	ctx->counter++;

	return TEE_SUCCESS;
}

TEE_Result sims_open_session(void **ctx)
{
	struct sims_session *context =
	    TEE_Malloc(sizeof(struct sims_session), TEE_MALLOC_FILL_ZERO);

	if (context == NULL)
		return TEE_ERROR_OUT_OF_MEMORY;

	context->sess = TEE_HANDLE_NULL;
	context->counter = counter;
	*ctx = context;

	counter++;

	return TEE_SUCCESS;
}

void sims_close_session(void *ctx)
{
	TEE_TASessionHandle sess = ((struct sims_session *)ctx)->sess;

	if (sess != TEE_HANDLE_NULL)
		TEE_CloseTASession(sess);

	TEE_Free(ctx);
}

TEE_Result sims_read(uint32_t param_types, TEE_Param params[4])
{
	uint32_t index = 0;
	void *p = NULL;

	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
			    TEE_PARAM_TYPE_MEMREF_OUTPUT, 0, 0))
		return TEE_ERROR_BAD_PARAMETERS;

	index = params[0].value.a;
	if (index >= TA_SIMS_MAX_STORAGE)
		return TEE_ERROR_BAD_PARAMETERS;

	if (storage[index].size > params[1].memref.size)
		return TEE_ERROR_OVERFLOW;

	p = TEE_Malloc(16000, 0);
	if (p == NULL)
		return TEE_ERROR_OUT_OF_MEMORY;

	TEE_MemMove(params[1].memref.buffer, storage[index].data,
		    params[1].memref.size);

	TEE_Free(p);

	return TEE_SUCCESS;
}

TEE_Result sims_write(uint32_t param_types, TEE_Param params[4])
{
	uint32_t index = 0;

	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
			    TEE_PARAM_TYPE_MEMREF_INPUT, 0, 0))
		return TEE_ERROR_BAD_PARAMETERS;

	index = params[0].value.a;
	if (index >= TA_SIMS_MAX_STORAGE)
		return TEE_ERROR_BAD_PARAMETERS;

	if (storage[index].data != NULL)
		TEE_Free(storage[index].data);

	storage[index].data = TEE_Malloc(params[1].memref.size, 0);
	if (storage[index].data == NULL) {
		storage[index].size = 0;
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	storage[index].size = params[1].memref.size;

	TEE_MemMove(storage[index].data, params[1].memref.buffer,
		    params[1].memref.size);

	return TEE_SUCCESS;
}

TEE_Result sims_get_counter(void *session_context, uint32_t param_types,
			    TEE_Param params[4])
{
	struct sims_session *ctx = (struct sims_session *)session_context;

	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT, 0, 0, 0))
		return TEE_ERROR_BAD_PARAMETERS;

	params[0].value.a = ctx->counter;

	return TEE_SUCCESS;
}

TEE_Result sims_entry_panic(void *session_context, uint32_t param_types,
			    TEE_Param params[4])
{
	uint32_t ret_orig = 0;
	TEE_Result res = TEE_SUCCESS;
	struct sims_session *ctx = (struct sims_session *)session_context;

	if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE))
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[0].memref.buffer && params[0].memref.size) {
		res = sims_open_ta_session(session_context,
					   param_types, params);
		if (res != TEE_SUCCESS)
			return res;

		params[0].memref.buffer = NULL;
		params[0].memref.size = 0;

		/* Trigger panic to remote TA */
		(void)TEE_InvokeTACommand(ctx->sess, TEE_TIMEOUT_INFINITE,
					  TA_SIMS_CMD_PANIC,
					  param_types, params,
					  &ret_orig);
	} else {
		TEE_Panic(0xbeef); /* Trigger panic to current TA */
	}

	return TEE_SUCCESS;
}
