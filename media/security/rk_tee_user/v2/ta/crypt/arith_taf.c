// SPDX-License-Identifier: BSD-2-Clause
/* Copyright (c) 2018, Linaro Limited */

#include <arith_taf.h>
#include <inttypes.h>
#include <ta_crypt.h>
#include <tee_internal_api.h>
#include <util.h>

#include "handle.h"

#define HT_BIGINT	BIT(31)
#define HT_FMMCTX	BIT(30)
#define HT_FMMVAR	BIT(29)
#define HT_MASK		(HT_BIGINT | HT_FMMCTX | HT_FMMVAR)

#define CHECK_PT(t0, t1, t2, t3) do { \
		const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_##t0, \
							TEE_PARAM_TYPE_##t1, \
							TEE_PARAM_TYPE_##t2, \
							TEE_PARAM_TYPE_##t3); \
		\
		if ((param_types) != exp_pt) { \
			EMSG("Unpexteded param_types 0x%" PRIx32 ", exptected 0x%" PRIx32, (param_types), exp_pt); \
			return TEE_ERROR_BAD_PARAMETERS; \
		} \
	} while (0)

static struct handle_db hdb = HANDLE_DB_INITIALIZER;

static void *lookup_handle(uint32_t type, uint32_t handle)
{
	void *ptr = NULL;

	if ((handle & HT_MASK) == type)
		ptr = handle_lookup(&hdb, handle & ~HT_MASK);

	if (!ptr)
		EMSG("Invalid handle 0x%" PRIx32, handle);

	return ptr;
}

static bool get_handle(uint32_t type, void *ptr, uint32_t *handle)
{
	switch (type) {
	case HT_BIGINT:
	case HT_FMMCTX:
	case HT_FMMVAR:
		break;
	default:
		EMSG("Invalid type 0x%" PRIx32, type);
		return false;
	}

	int h = handle_get(&hdb, ptr);

	if (h < 0) {
		EMSG("Failed to allocate handle");
		return false;
	}

	*handle = (uint32_t)h | type;
	return true;
}

static void *put_handle(uint32_t handle)
{
	void *ptr = handle_put(&hdb, handle & ~HT_MASK);

	if (!ptr)
		EMSG("Invalid handle 0x%" PRIx32, handle);

	return ptr;
}

TEE_Result ta_entry_arith_new_var(uint32_t param_types,
				  TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	size_t len = TEE_BigIntSizeInU32(params[0].value.a);
	TEE_BigInt *big_int = TEE_Malloc(len * sizeof(TEE_BigInt), 0);

	if (!big_int)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!get_handle(HT_BIGINT, big_int, &params[1].value.a)) {
		TEE_Free(big_int);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	TEE_BigIntInit(big_int, len);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_new_fmm_ctx(uint32_t param_types,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_BigInt *modulus = NULL;
	uint32_t len = 0;
	TEE_BigIntFMMContext *ctx = NULL;

	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	modulus = lookup_handle(HT_BIGINT, params[0].value.b);
	if (!modulus)
		return TEE_ERROR_BAD_PARAMETERS;

	len = TEE_BigIntFMMContextSizeInU32(params[0].value.a);
	ctx = TEE_Malloc(len * sizeof(*ctx), 0);

	if (!get_handle(HT_FMMCTX, ctx, &params[1].value.a)) {
		TEE_Free(ctx);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	TEE_BigIntInitFMMContext(ctx, len, modulus);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_new_fmm_var(uint32_t param_types,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	size_t len = TEE_BigIntFMMSizeInU32(params[0].value.a);
	TEE_BigIntFMM *fmm = TEE_Malloc(len * sizeof(*fmm), 0);

	if (!fmm)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!get_handle(HT_FMMVAR, fmm, &params[1].value.a)) {
		TEE_Free(fmm);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	TEE_BigIntInitFMM(fmm, len);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_free_handle(uint32_t param_types,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, NONE, NONE, NONE);

	void *ptr = put_handle(params[0].value.a & ~HT_MASK);

	if (!ptr)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_Free(ptr);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_from_octet_string(uint32_t param_types,
					    TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, MEMREF_INPUT, NONE, NONE);

	TEE_BigInt *big_int = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!big_int)
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_BigIntConvertFromOctetString(big_int,
						params[1].memref.buffer,
						params[1].memref.size,
						params[0].value.b);
}

TEE_Result ta_entry_arith_from_s32(uint32_t param_types,
				   TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, NONE, NONE, NONE);

	TEE_BigInt *big_int = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!big_int)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_BigIntConvertFromS32(big_int, params[0].value.b);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_get_value(uint32_t param_types,
				    TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, MEMREF_OUTPUT, NONE);

	TEE_BigInt *big_int = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!big_int)
		return TEE_ERROR_BAD_PARAMETERS;

	if (TEE_BigIntCmpS32(big_int, 0) < 0)
		params[1].value.a = -1;
	else
		params[1].value.a = 1;

	return TEE_BigIntConvertToOctetString(params[2].memref.buffer,
					      &params[2].memref.size, big_int);
}

TEE_Result ta_entry_arith_get_value_s32(uint32_t param_types,
					TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *big_int = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!big_int)
		return TEE_ERROR_BAD_PARAMETERS;

	int32_t v = 0;
	TEE_Result res = TEE_BigIntConvertToS32(&v, big_int);

	if (!res)
		params[1].value.a = v;

	return res;
}

TEE_Result ta_entry_arith_get_bit(uint32_t param_types,
				  TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *big_int = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!big_int)
		return TEE_ERROR_BAD_PARAMETERS;

	params[1].value.a = TEE_BigIntGetBit(big_int, params[0].value.b);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_get_bit_count(uint32_t param_types,
					TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *big_int = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!big_int)
		return TEE_ERROR_BAD_PARAMETERS;

	params[1].value.a = TEE_BigIntGetBitCount(big_int);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_shift_right(uint32_t param_types,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, NONE, NONE);

	TEE_BigInt *op = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *dest = lookup_handle(HT_BIGINT, params[1].value.a);

	if (!op || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_BigIntShiftRight(dest, op, params[0].value.b);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_cmp(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *op1 = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *op2 = lookup_handle(HT_BIGINT, params[0].value.b);

	if (!op1 || !op2)
		return TEE_ERROR_BAD_PARAMETERS;

	params[1].value.a = TEE_BigIntCmp(op1, op2);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_cmp_s32(uint32_t param_types,
				  TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *op = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!op)
		return TEE_ERROR_BAD_PARAMETERS;

	params[1].value.a = TEE_BigIntCmpS32(op, params[0].value.b);

	return TEE_SUCCESS;
}

static TEE_Result ternary_func(uint32_t param_types,
			       TEE_Param params[TEE_NUM_PARAMS],
			       void (*func)(TEE_BigInt *dest,
					    const TEE_BigInt *op1,
					    const TEE_BigInt *op2,
					    const TEE_BigInt *n))
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, NONE, NONE);

	TEE_BigInt *op1 = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *op2 = lookup_handle(HT_BIGINT, params[0].value.b);
	TEE_BigInt *n = lookup_handle(HT_BIGINT, params[1].value.a);
	TEE_BigInt *dest = lookup_handle(HT_BIGINT, params[1].value.b);

	if (!op1 || !op2 || !n || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	func(dest, op1, op2, n);

	return TEE_SUCCESS;
}

static TEE_Result binary_func(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS],
			      void (*func)(TEE_BigInt *dest,
					   const TEE_BigInt *op1,
					   const TEE_BigInt *op2))
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, NONE, NONE);

	TEE_BigInt *op1 = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *op2 = lookup_handle(HT_BIGINT, params[0].value.b);
	TEE_BigInt *dest = lookup_handle(HT_BIGINT, params[1].value.a);

	if (!op1 || !op2 || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	func(dest, op1, op2);

	return TEE_SUCCESS;
}

static TEE_Result unary_func(uint32_t param_types,
			     TEE_Param params[TEE_NUM_PARAMS],
			     void (*func)(TEE_BigInt *dest,
					  const TEE_BigInt *op))
{
	CHECK_PT(VALUE_INPUT, NONE, NONE, NONE);

	TEE_BigInt *op = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *dest = lookup_handle(HT_BIGINT, params[0].value.b);

	if (!op || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	func(dest, op);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_add(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	return binary_func(param_types, params,  TEE_BigIntAdd);
}

TEE_Result ta_entry_arith_sub(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	return binary_func(param_types, params,  TEE_BigIntSub);
}

TEE_Result ta_entry_arith_mul(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	return binary_func(param_types, params,  TEE_BigIntMul);
}

TEE_Result ta_entry_arith_neg(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	return unary_func(param_types, params, TEE_BigIntNeg);
}

TEE_Result ta_entry_arith_sqr(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	return unary_func(param_types, params, TEE_BigIntSquare);
}

TEE_Result ta_entry_arith_div(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, NONE, NONE);

	TEE_BigInt *op1 = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *op2 = lookup_handle(HT_BIGINT, params[0].value.b);
	TEE_BigInt *dest_q = lookup_handle(HT_BIGINT, params[1].value.a);
	TEE_BigInt *dest_r = lookup_handle(HT_BIGINT, params[1].value.b);

	if (!op1 || !op2 || !dest_q || !dest_r)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_BigIntDiv(dest_q, dest_r, op1, op2);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_mod(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	return binary_func(param_types, params, TEE_BigIntMod);
}

TEE_Result ta_entry_arith_addmod(uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	return ternary_func(param_types, params, TEE_BigIntAddMod);
}

TEE_Result ta_entry_arith_submod(uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	return ternary_func(param_types, params, TEE_BigIntSubMod);
}

TEE_Result ta_entry_arith_mulmod(uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	return ternary_func(param_types, params, TEE_BigIntMulMod);
}

TEE_Result ta_entry_arith_sqrmod(uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	return binary_func(param_types, params, TEE_BigIntSquareMod);
}

TEE_Result ta_entry_arith_invmod(uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	return binary_func(param_types, params, TEE_BigIntInvMod);
}

TEE_Result ta_entry_arith_is_rel_prime(uint32_t param_types,
				       TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *op1 = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *op2 = lookup_handle(HT_BIGINT, params[0].value.b);

	if (!op1 || !op2)
		return TEE_ERROR_BAD_PARAMETERS;

	params[1].value.a = TEE_BigIntRelativePrime(op1, op2);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_compute_egcd(uint32_t param_types,
				       TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, VALUE_INPUT, NONE);

	TEE_BigInt *op1 = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *op2 = lookup_handle(HT_BIGINT, params[0].value.b);
	TEE_BigInt *dest_gcd = lookup_handle(HT_BIGINT, params[2].value.a);
	TEE_BigInt *dest_u = NULL;
	TEE_BigInt *dest_v = NULL;

	if (!op1 || !op2 || !dest_gcd)
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[1].value.a != TA_CRYPT_ARITH_INVALID_HANDLE) {
		dest_u = lookup_handle(HT_BIGINT, params[1].value.a);
		if (!dest_u)
			return TEE_ERROR_BAD_PARAMETERS;
	}
	if (params[1].value.b != TA_CRYPT_ARITH_INVALID_HANDLE) {
		dest_v = lookup_handle(HT_BIGINT, params[1].value.b);
		if (!dest_v)
			return TEE_ERROR_BAD_PARAMETERS;
	}

	TEE_BigIntComputeExtendedGcd(dest_gcd, dest_u, dest_v, op1, op2);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_is_prime(uint32_t param_types,
				   TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_OUTPUT, NONE, NONE);

	TEE_BigInt *op = lookup_handle(HT_BIGINT, params[0].value.a);

	if (!op)
		return TEE_ERROR_BAD_PARAMETERS;

	params[1].value.a = TEE_BigIntIsProbablePrime(op, params[0].value.b);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_to_fmm(uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, NONE, NONE);

	TEE_BigInt *src = lookup_handle(HT_BIGINT, params[0].value.a);
	TEE_BigInt *n = lookup_handle(HT_BIGINT, params[0].value.b);
	TEE_BigIntFMMContext *ctx = lookup_handle(HT_FMMCTX, params[1].value.a);
	TEE_BigIntFMM *dest = lookup_handle(HT_FMMVAR, params[1].value.b);

	if (!src || !n | !ctx || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_BigIntConvertToFMM(dest, src, n, ctx);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_from_fmm(uint32_t param_types,
				   TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, NONE, NONE);

	TEE_BigIntFMM *src = lookup_handle(HT_FMMVAR, params[0].value.a);
	TEE_BigInt *n = lookup_handle(HT_BIGINT, params[0].value.b);
	TEE_BigIntFMMContext *ctx = lookup_handle(HT_FMMCTX, params[1].value.a);
	TEE_BigInt *dest = lookup_handle(HT_BIGINT, params[1].value.b);

	if (!src || !n | !ctx || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_BigIntConvertFromFMM(dest, src, n, ctx);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_arith_compute_fmm(uint32_t param_types,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	CHECK_PT(VALUE_INPUT, VALUE_INPUT, VALUE_INPUT, NONE);

	TEE_BigIntFMM *op1 = lookup_handle(HT_FMMVAR, params[0].value.a);
	TEE_BigIntFMM *op2 = lookup_handle(HT_FMMVAR, params[0].value.b);
	TEE_BigInt *n = lookup_handle(HT_BIGINT, params[1].value.a);
	TEE_BigIntFMMContext *ctx = lookup_handle(HT_FMMCTX, params[1].value.b);
	TEE_BigIntFMM *dest = lookup_handle(HT_FMMVAR, params[2].value.a);

	if (!op1 || !op2 || !n | !ctx || !dest)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_BigIntComputeFMM(dest, op1, op2, n, ctx);

	return TEE_SUCCESS;
}
