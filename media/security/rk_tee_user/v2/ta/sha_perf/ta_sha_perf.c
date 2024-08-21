// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#include <tee_internal_api.h>
#include <tee_ta_api.h>
#include <string.h>
#include <trace.h>

#include "ta_sha_perf.h"
#include "ta_sha_perf_priv.h"

#define CHECK(res, name, action) do {			\
		if ((res) != TEE_SUCCESS) {		\
			DMSG(name ": 0x%08x", (res));	\
			action				\
		}					\
	} while(0)

static TEE_OperationHandle digest_op = NULL;

TEE_Result cmd_process(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int n = 0;
	void *in = NULL;
	void *out = NULL;
	uint32_t insz = 0;
	uint32_t outsz = 0;
	uint32_t offset = 0;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_MEMREF_OUTPUT,
						   TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	offset = params[2].value.b;
	in = (uint8_t *)params[0].memref.buffer + offset;
	insz = params[0].memref.size - offset;
	out = params[1].memref.buffer;
	outsz = params[1].memref.size;
	n = params[2].value.a;

	while (n--) {
		res = TEE_DigestDoFinal(digest_op, in, insz, out, &outsz);
		CHECK(res, "TEE_DigestDoFinal", return res;);
	}
	return TEE_SUCCESS;
}

TEE_Result cmd_prepare_op(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t algo = 0;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (params[0].value.a) {
	case TA_SHA_SHA1:
		algo = TEE_ALG_SHA1;
		break;
	case TA_SHA_SHA224:
		algo = TEE_ALG_SHA224;
		break;
	case TA_SHA_SHA256:
		algo = TEE_ALG_SHA256;
		break;
	case TA_SHA_SHA384:
		algo = TEE_ALG_SHA384;
		break;
	case TA_SHA_SHA512:
		algo = TEE_ALG_SHA512;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (digest_op)
		TEE_FreeOperation(digest_op);

	res = TEE_AllocateOperation(&digest_op, algo, TEE_MODE_DIGEST, 0);
	CHECK(res, "TEE_AllocateOperation", return res;);

	return TEE_SUCCESS;
}


void cmd_clean_res(void)
{
	if (digest_op)
		TEE_FreeOperation(digest_op);	
}
