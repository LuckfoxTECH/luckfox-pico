// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include <tee_internal_api.h>
#include "cryp_taf.h"

#define ASSERT_PARAM_TYPE(pt)                    \
do {                                             \
	if ((pt) != param_type)                  \
		return TEE_ERROR_BAD_PARAMETERS; \
} while (0)

/* pointers in 32-bit fields passed as an offset to ta_head */
extern const void *ta_head;
#define VAL2HANDLE(v) (void *)(uintptr_t)(v == TEE_HANDLE_NULL ? v : v + (uintptr_t)&ta_head)
#define HANDLE2VAL(h) (uint32_t)(h == TEE_HANDLE_NULL ? (uintptr_t)h : (uintptr_t)((uintptr_t)h - (uintptr_t)&ta_head))

TEE_Result ta_entry_allocate_operation(uint32_t param_type, TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_OperationHandle op = TEE_HANDLE_NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INOUT,
			   TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	res = TEE_AllocateOperation(&op,
				    params[0].value.b, params[1].value.a,
				    params[1].value.b);
	params[0].value.a = HANDLE2VAL(op);
	return res;
}

TEE_Result ta_entry_free_operation(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_FreeOperation(op);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_get_operation_info(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	if (params[1].memref.size < sizeof(TEE_OperationInfo))
		return TEE_ERROR_SHORT_BUFFER;
	params[1].memref.size = sizeof(TEE_OperationInfo);

	TEE_GetOperationInfo(op, params[1].memref.buffer);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_reset_operation(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_ResetOperation(op);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_set_operation_key(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_ObjectHandle key = VAL2HANDLE(params[0].value.b);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	return TEE_SetOperationKey(op, key);
}

TEE_Result ta_entry_set_operation_key2(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_ObjectHandle key1 = VAL2HANDLE(params[0].value.b);
	TEE_ObjectHandle key2 = VAL2HANDLE(params[1].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	return TEE_SetOperationKey2(op, key1, key2);
}

TEE_Result ta_entry_copy_operation(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle dst = VAL2HANDLE(params[0].value.a);
	TEE_OperationHandle src = VAL2HANDLE(params[0].value.b);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));


	TEE_CopyOperation(dst, src);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_digest_update(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	TEE_DigestUpdate(op, params[1].memref.buffer, params[1].memref.size);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_digest_do_final(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	return TEE_DigestDoFinal(op,
			params[1].memref.buffer, params[1].memref.size,
			params[2].memref.buffer, &params[2].memref.size);
}

TEE_Result ta_entry_cipher_init(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	void *buffer = NULL;
	size_t size = 0;

	if (param_type == TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE)) {
		buffer = NULL;
		size = 0;
	} else if (param_type == TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						 TEE_PARAM_TYPE_MEMREF_INPUT,
						 TEE_PARAM_TYPE_NONE,
						 TEE_PARAM_TYPE_NONE)) {
		buffer = params[1].memref.buffer;
		size = params[1].memref.size;
	} else
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_CipherInit(op, buffer, size);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_cipher_update(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	return TEE_CipherUpdate(op,
			params[1].memref.buffer, params[1].memref.size,
			params[2].memref.buffer, &params[2].memref.size);
}

TEE_Result ta_entry_cipher_do_final(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	return TEE_CipherDoFinal(op,
			 params[1].memref.buffer, params[1].memref.size,
			 params[2].memref.buffer, &params[2].memref.size);
}

TEE_Result ta_entry_mac_init(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	void *buffer = NULL;
	size_t size = 0;

	if (param_type == TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE)) {
		buffer = NULL;
		size = 0;
	} else if (param_type == TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						 TEE_PARAM_TYPE_MEMREF_INPUT,
						 TEE_PARAM_TYPE_NONE,
						 TEE_PARAM_TYPE_NONE)) {
		buffer = params[1].memref.buffer;
		size = params[1].memref.size;
	} else
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_MACInit(op, buffer, size);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_mac_update(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	TEE_MACUpdate(op, params[1].memref.buffer, params[1].memref.size);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_mac_final_compute(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	return TEE_MACComputeFinal(op,
			   params[1].memref.buffer, params[1].memref.size,
			   params[2].memref.buffer, &params[2].memref.size);
}

TEE_Result ta_entry_mac_final_compare(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE));

	return TEE_MACCompareFinal(op,
			   params[1].memref.buffer, params[1].memref.size,
			   params[2].memref.buffer, params[2].memref.size);
}

TEE_Result ta_entry_allocate_transient_object(uint32_t param_type,
					      TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_ObjectHandle o = TEE_HANDLE_NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	res = TEE_AllocateTransientObject(params[0].value.a, params[0].value.b,
					  &o);
	if (res == TEE_SUCCESS)
		params[1].value.a = HANDLE2VAL(o);
	return res;
}

TEE_Result ta_entry_free_transient_object(uint32_t param_type,
					  TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_FreeTransientObject(o);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_reset_transient_object(uint32_t param_type,
					   TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	TEE_ResetTransientObject(o);
	return TEE_SUCCESS;
}

struct attr_packed {
	uint32_t id;
	uint32_t a;
	uint32_t b;
};

static TEE_Result unpack_attrs(const uint8_t *buf, size_t blen,
			       TEE_Attribute **attrs, uint32_t *attr_count)
{
	TEE_Result res = TEE_SUCCESS;
	TEE_Attribute *a = NULL;
	const struct attr_packed *ap = NULL;
	size_t num_attrs = 0;
	const size_t num_attrs_size = sizeof(uint32_t);

	if (blen == 0)
		goto out;

	if (((uintptr_t)buf & 0x3) != 0 || blen < num_attrs_size)
		return TEE_ERROR_BAD_PARAMETERS;
	num_attrs = *(uint32_t *) (void *)buf;
	if ((blen - num_attrs_size) < (num_attrs * sizeof(*ap)))
		return TEE_ERROR_BAD_PARAMETERS;
	ap = (const struct attr_packed *)(const void *)(buf + num_attrs_size);

	if (num_attrs > 0) {
		size_t n;

		a = TEE_Malloc(num_attrs * sizeof(TEE_Attribute), 0);
		if (!a)
			return TEE_ERROR_OUT_OF_MEMORY;
		for (n = 0; n < num_attrs; n++) {
			uintptr_t p;

			a[n].attributeID = ap[n].id;
			if (ap[n].id & TEE_ATTR_FLAG_VALUE) {
				a[n].content.value.a = ap[n].a;
				a[n].content.value.b = ap[n].b;
				continue;
			}

			a[n].content.ref.length = ap[n].b;
			p = (uintptr_t)ap[n].a;
			if (p) {
				if ((p + a[n].content.ref.length) > blen) {
					res = TEE_ERROR_BAD_PARAMETERS;
					goto out;
				}
				p += (uintptr_t)buf;
			}
			a[n].content.ref.buffer = (void *)p;
		}
	}

	res = TEE_SUCCESS;
out:
	if (res == TEE_SUCCESS) {
		*attrs = a;
		*attr_count = num_attrs;
	} else {
		TEE_Free(a);
	}
	return res;
}

TEE_Result ta_entry_populate_transient_object(uint32_t param_type,
					      TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_PopulateTransientObject(o, attrs, attr_count);
	TEE_Free(attrs);
	return res;
}

TEE_Result ta_entry_copy_object_attributes(uint32_t param_type,
					   TEE_Param params[4])
{
	TEE_ObjectHandle dst = VAL2HANDLE(params[0].value.a);
	TEE_ObjectHandle src = VAL2HANDLE(params[0].value.b);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	return TEE_CopyObjectAttributes1(dst, src);
}

TEE_Result ta_entry_generate_key(uint32_t param_type, TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_GenerateKey(o, params[0].value.b, attrs, attr_count);
	TEE_Free(attrs);
	return res;
}

TEE_Result ta_entry_asymmetric_encrypt(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_AsymmetricEncrypt(op, attrs, attr_count,
			params[2].memref.buffer, params[2].memref.size,
			params[3].memref.buffer, &params[3].memref.size);
	TEE_Free(attrs);
	return res;
}

TEE_Result ta_entry_asymmetric_decrypt(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_AsymmetricDecrypt(op, attrs, attr_count,
			params[2].memref.buffer, params[2].memref.size,
			params[3].memref.buffer, &params[3].memref.size);
	TEE_Free(attrs);
	return res;
}

TEE_Result ta_entry_asymmetric_sign_digest(uint32_t param_type,
					   TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_AsymmetricSignDigest(op, attrs, attr_count,
			params[2].memref.buffer, params[2].memref.size,
			params[3].memref.buffer, &params[3].memref.size);
	TEE_Free(attrs);
	return res;
}

TEE_Result ta_entry_asymmetric_verify_digest(uint32_t param_type,
					     TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_AsymmetricVerifyDigest(op, attrs, attr_count,
			params[2].memref.buffer, params[2].memref.size,
			params[3].memref.buffer, params[3].memref.size);
	TEE_Free(attrs);
	return res;
}

TEE_Result ta_entry_derive_key(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_ObjectHandle key = VAL2HANDLE(params[0].value.b);
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Attribute *attrs = NULL;
	uint32_t attr_count = 0;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	res = unpack_attrs(params[1].memref.buffer, params[1].memref.size,
			   &attrs, &attr_count);
	if (res != TEE_SUCCESS)
		return res;

	TEE_DeriveKey(op, attrs, attr_count, key);
	TEE_Free(attrs);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_random_number_generate(uint32_t param_type,
					   TEE_Param params[4])
{
	void *buf = NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	buf = TEE_Malloc(params[0].memref.size, 0);
	if (!buf)
		return TEE_ERROR_OUT_OF_MEMORY;

	TEE_GenerateRandom(buf, params[0].memref.size);
	TEE_MemMove(params[0].memref.buffer, buf, params[0].memref.size);
	TEE_Free(buf);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_ae_init(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE));
	return TEE_AEInit(op, params[1].memref.buffer, params[1].memref.size,
			  params[0].value.b * 8, /* tag_len in bits */
			  params[2].value.a, params[2].value.b);
}

TEE_Result ta_entry_ae_update_aad(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	TEE_AEUpdateAAD(op, params[1].memref.buffer, params[1].memref.size);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_ae_update(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE));

	TEE_AEUpdate(op, params[1].memref.buffer, params[1].memref.size,
		     params[2].memref.buffer, &params[2].memref.size);
	return TEE_SUCCESS;
}

TEE_Result ta_entry_ae_encrypt_final(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_OUT_OF_MEMORY;
	void *b2 = NULL;
	void *b3 = NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT));

	b2 = TEE_Malloc(params[2].memref.size, 0);
	b3 = TEE_Malloc(params[3].memref.size, 0);
	if (!b2 || !b3)
		goto out;

	res = TEE_AEEncryptFinal(op, params[1].memref.buffer,
				 params[1].memref.size, b2,
				 &params[2].memref.size, b3,
				 &params[3].memref.size);
	if (!res) {
		TEE_MemMove(params[2].memref.buffer, b2, params[2].memref.size);
		TEE_MemMove(params[3].memref.buffer, b3, params[3].memref.size);
	}
out:
	TEE_Free(b2);
	TEE_Free(b3);
	return res;
}

TEE_Result ta_entry_ae_decrypt_final(uint32_t param_type, TEE_Param params[4])
{
	TEE_OperationHandle op = VAL2HANDLE(params[0].value.a);
	TEE_Result res = TEE_ERROR_OUT_OF_MEMORY;
	void *b2 = NULL;
	void *b3 = NULL;

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT,
			   TEE_PARAM_TYPE_MEMREF_INPUT));

	b2 = TEE_Malloc(params[2].memref.size, 0);
	b3 = TEE_Malloc(params[3].memref.size, 0);
	if (!b2 || !b3)
		goto out;

	TEE_MemMove(b3, params[3].memref.buffer, params[3].memref.size);
	res = TEE_AEDecryptFinal(op, params[1].memref.buffer,
				 params[1].memref.size, b2,
				 &params[2].memref.size, b3,
				 params[3].memref.size);
	if (!res)
		TEE_MemMove(params[2].memref.buffer, b2, params[2].memref.size);
out:
	TEE_Free(b2);
	TEE_Free(b3);

	return res;
}

TEE_Result ta_entry_get_object_buffer_attribute(uint32_t param_type,
						TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	return TEE_GetObjectBufferAttribute(o, params[0].value.b,
			params[1].memref.buffer, &params[1].memref.size);
}

TEE_Result ta_entry_get_object_value_attribute(uint32_t param_type,
					       TEE_Param params[4])
{
	TEE_ObjectHandle o = VAL2HANDLE(params[0].value.a);

	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	return TEE_GetObjectValueAttribute(o, params[0].value.b,
				   &params[1].value.a, &params[1].value.b);
}

TEE_Result ta_entry_is_algo_supported(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	ASSERT_PARAM_TYPE(TEE_PARAM_TYPES
			  (TEE_PARAM_TYPE_VALUE_INPUT,
			   TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			   TEE_PARAM_TYPE_NONE));

	params[1].value.a = TEE_IsAlgorithmSupported(params[0].value.a,
						     params[0].value.b);

	return TEE_SUCCESS;
}
