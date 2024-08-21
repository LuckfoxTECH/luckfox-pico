// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#include <tee_internal_api_extensions.h>
#include <tee_internal_api.h>
#include <tee_ta_api.h>
#include <string.h>
#include <trace.h>

#include "ta_aes_perf.h"
#include "ta_aes_perf_priv.h"

#define CHECK(res, name, action) do {			\
		if ((res) != TEE_SUCCESS) {		\
			DMSG(name ": 0x%08x", (res));	\
			action				\
		}					\
	} while(0)

#define TAG_LEN	128

static uint8_t iv[] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
			0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF };
static int use_iv;

static TEE_OperationHandle crypto_op = NULL;
static uint32_t algo;

static bool is_inbuf_a_secure_memref(TEE_Param *param)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	/*
	 * Check secure attribute for the referenced buffer
	 * Trust core on validity of the memref size: test only 1st byte
	 * instead of the overall buffer, and if it's not secure, assume
	 * the buffer is nonsecure.
	 */
	res = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_READ |
					 TEE_MEMORY_ACCESS_SECURE,
					 param->memref.buffer, 1);
	return (res == TEE_SUCCESS);
}

static bool is_outbuf_a_secure_memref(TEE_Param *param)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	/*
	 * Check secure attribute for the referenced buffer
	 * Trust core on validity of the memref size: test only 1st byte
	 * instead of the overall buffer, and if it's not secure, assume
	 * the buffer is nonsecure.
	 */
	res = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_WRITE |
					 TEE_MEMORY_ACCESS_SECURE,
					 param->memref.buffer, 1);
	return (res == TEE_SUCCESS);
}

#if defined(CFG_CACHE_API)
static TEE_Result flush_memref_buffer(TEE_Param *param)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	res = TEE_CacheFlush(param->memref.buffer,
			     param->memref.size);
	CHECK(res, "TEE_CacheFlush(in)", return res;);
	return res;
}
#else
static __maybe_unused TEE_Result flush_memref_buffer(TEE_Param *param __unused)
{
	return TEE_SUCCESS;
}
#endif /* CFG_CACHE_API */

TEE_Result cmd_process(uint32_t param_types,
		       TEE_Param params[TEE_NUM_PARAMS],
		       bool use_sdp)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int n = 0;
	int unit = 0;
	void *in = NULL;
	void *out = NULL;
	uint32_t insz = 0;
	uint32_t outsz = 0;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
						   TEE_PARAM_TYPE_MEMREF_INOUT,
						   TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_NONE);
	bool secure_in = false;
	bool secure_out = false;
	TEE_Result (*do_update)(TEE_OperationHandle, const void *, uint32_t,
				void *, uint32_t *) = NULL;

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	if (use_sdp) {
		/*
		 * Whatever is expected as memory reference, it is mandatory
		 * for SDP aware trusted applications of safely indentify all
		 * memory reference parameters. Hence these tests must be part
		 * of the performance test setup.
		 */
		secure_in = is_inbuf_a_secure_memref(&params[0]);
		secure_out = is_outbuf_a_secure_memref(&params[1]);

		/*
		 * We could invalidate only the caches. We prefer to flush
		 * them in case 2 sub-buffers are accessed by TAs from a single
		 * allocated SDP memory buffer, and those are not cache-aligned.
		 * Invalidating might cause data loss in cache lines. Hence
		 * rather flush them all before accessing (in read or write).
		 */
		if (secure_in) {
			res = flush_memref_buffer(&params[0]);
			CHECK(res, "pre-flush in memref param", return res;);
		}
		if (secure_out) {
			res = flush_memref_buffer(&params[1]);
			CHECK(res, "pre-flush out memref param", return res;);
		}
	}

	in = params[0].memref.buffer;
	insz = params[0].memref.size;
	out = params[1].memref.buffer;
	outsz = params[1].memref.size;
	n = params[2].value.a;
	unit = params[2].value.b;
	if (!unit)
		unit = insz;

	if (algo == TEE_ALG_AES_GCM)
		do_update = TEE_AEUpdate;
	else
		do_update = TEE_CipherUpdate;

	while (n--) {
		uint32_t i = 0;
		for (i = 0; i < insz / unit; i++) {
			res = do_update(crypto_op, in, unit, out, &outsz);
			CHECK(res, "TEE_CipherUpdate/TEE_AEUpdate", return res;);
			in  = (void *)((uintptr_t)in + unit);
			out = (void *)((uintptr_t)out + unit);
		}
		if (insz % unit) {
			res = do_update(crypto_op, in, insz % unit, out, &outsz);
			CHECK(res, "TEE_CipherUpdate/TEE_AEUpdate", return res;);
		}
	}

	if (secure_out) {
		/* intentionally flush output data from cache for SDP buffers */
		res = flush_memref_buffer(&params[1]);
		CHECK(res, "post-flush out memref param", return res;);
	}

	return TEE_SUCCESS;
}

TEE_Result cmd_prepare_key(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_ObjectHandle hkey = TEE_HANDLE_NULL;
	TEE_ObjectHandle hkey2 = TEE_HANDLE_NULL;
	TEE_Attribute attr = { };
	uint32_t mode = 0;
	uint32_t op_keysize = 0;
	uint32_t keysize = 0;
	const uint8_t *ivp = NULL;
	size_t ivlen = 0;
	static uint8_t aes_key[] = { 0x00, 0x01, 0x02, 0x03,
				     0x04, 0x05, 0x06, 0x07,
				     0x08, 0x09, 0x0A, 0x0B,
				     0x0C, 0x0D, 0x0E, 0x0F,
				     0x10, 0x11, 0x12, 0x13,
				     0x14, 0x15, 0x16, 0x17,
				     0x18, 0x19, 0x1A, 0x1B,
				     0x1C, 0x1D, 0x1E, 0x1F };
	static uint8_t aes_key2[] = { 0x20, 0x21, 0x22, 0x23,
				      0x24, 0x25, 0x26, 0x27,
				      0x28, 0x29, 0x2A, 0x2B,
				      0x2C, 0x2D, 0x2E, 0x2F,
				      0x30, 0x31, 0x32, 0x33,
				      0x34, 0x35, 0x36, 0x37,
				      0x38, 0x39, 0x3A, 0x3B,
				      0x3C, 0x3D, 0x3E, 0x3F };
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	mode = params[0].value.a ? TEE_MODE_DECRYPT : TEE_MODE_ENCRYPT;
	keysize = params[0].value.b;
	op_keysize = keysize;

	switch (params[1].value.a) {
	case TA_AES_ECB:
		algo = TEE_ALG_AES_ECB_NOPAD;
		use_iv = 0;
		break;
	case TA_AES_CBC:
		algo = TEE_ALG_AES_CBC_NOPAD;
		use_iv = 1;
		break;
	case TA_AES_CTR:
		algo = TEE_ALG_AES_CTR;
		use_iv = 1;
		break;
	case TA_AES_XTS:
		algo = TEE_ALG_AES_XTS;
		use_iv = 1;
		op_keysize *= 2;
		break;
	case TA_AES_GCM:
		algo = TEE_ALG_AES_GCM;
		use_iv = 1;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	cmd_clean_res();

	res = TEE_AllocateOperation(&crypto_op, algo, mode, op_keysize);
	CHECK(res, "TEE_AllocateOperation", return res;);

	res = TEE_AllocateTransientObject(TEE_TYPE_AES, keysize, &hkey);
	CHECK(res, "TEE_AllocateTransientObject", return res;);

	attr.attributeID = TEE_ATTR_SECRET_VALUE;
	attr.content.ref.buffer = aes_key;
	attr.content.ref.length = keysize / 8;

	res = TEE_PopulateTransientObject(hkey, &attr, 1);
	CHECK(res, "TEE_PopulateTransientObject", return res;);

	if (algo == TEE_ALG_AES_XTS) {
		res = TEE_AllocateTransientObject(TEE_TYPE_AES, keysize,
						  &hkey2);
		CHECK(res, "TEE_AllocateTransientObject", return res;);

		attr.content.ref.buffer = aes_key2;

		res = TEE_PopulateTransientObject(hkey2, &attr, 1);
		CHECK(res, "TEE_PopulateTransientObject", return res;);

		res = TEE_SetOperationKey2(crypto_op, hkey, hkey2);
		CHECK(res, "TEE_SetOperationKey2", return res;);

		TEE_FreeTransientObject(hkey2);
	} else {
		res = TEE_SetOperationKey(crypto_op, hkey);
		CHECK(res, "TEE_SetOperationKey", return res;);
	}

	TEE_FreeTransientObject(hkey);

	if (use_iv) {
		ivp = iv;
		ivlen = sizeof(iv);
	} else {
		ivp = NULL;
		ivlen = 0;
	}

	if (algo == TEE_ALG_AES_GCM) {
		return TEE_AEInit(crypto_op, ivp, ivlen, TAG_LEN, 0, 0);
	} else {
		TEE_CipherInit(crypto_op, ivp, ivlen);
		return TEE_SUCCESS;
	}
}

void cmd_clean_res(void)
{
	if (crypto_op)
		TEE_FreeOperation(crypto_op);
}
