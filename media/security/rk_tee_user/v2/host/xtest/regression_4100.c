// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Linaro Limited */

#include <adbg.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ta_crypt.h>
#include <tee_api_types.h>
#include <util.h>

#include "xtest_test.h"
#include "xtest_helpers.h"

static TEEC_Result cmd_new_var(ADBG_Case_t *c, TEEC_Session *s,
			       uint32_t num_bits, uint32_t *handle)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = num_bits;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_NEW_VAR, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*handle = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_new_fmm_var(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t num_bits, uint32_t *handle)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = num_bits;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_NEW_FMM_VAR, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*handle = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_new_fmm_ctx(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t num_bits, uint32_t hmodulus,
				   uint32_t *handle)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = num_bits;
	op.params[0].value.b = hmodulus;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_NEW_FMM_CTX, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*handle = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_free_handle(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t handle)
{
	if (handle == TA_CRYPT_ARITH_INVALID_HANDLE)
		return TEEC_SUCCESS;

	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = handle;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_FREE_HANDLE, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_from_octet_string(ADBG_Case_t *c, TEEC_Session *s,
					 uint8_t *buf, uint32_t buf_len,
					 int32_t sign, uint32_t h)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = h;
	op.params[0].value.b = sign;
	op.params[1].tmpref.buffer = buf;
	op.params[1].tmpref.size = buf_len;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_TEMP_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_FROM_OCTET_STRING, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_from_s32(ADBG_Case_t *c, TEEC_Session *s,
				uint32_t handle, int32_t v)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = handle;
	op.params[0].value.b = v;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_FROM_S32, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_get_bit(ADBG_Case_t *c, TEEC_Session *s,
				uint32_t handle, uint32_t bit_num, uint32_t *v)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = handle;
	op.params[0].value.b = bit_num;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_GET_BIT, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*v = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_get_bit_count(ADBG_Case_t *c, TEEC_Session *s,
				uint32_t handle, uint32_t *v)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = handle;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_GET_BIT_COUNT, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*v = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_binary_cmd(ADBG_Case_t *c, TEEC_Session *s, uint32_t cmd,
				  uint32_t hop1, uint32_t hop2, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop1;
	op.params[0].value.b = hop2;
	op.params[1].value.a = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_unary_cmd(ADBG_Case_t *c, TEEC_Session *s, uint32_t cmd,
				  uint32_t hop, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop;
	op.params[0].value.b = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_ternary_cmd(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t cmd, uint32_t hop1, uint32_t hop2,
				   uint32_t hn, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop1;
	op.params[0].value.b = hop2;
	op.params[1].value.a = hn;
	op.params[1].value.b = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_neg(ADBG_Case_t *c, TEEC_Session *s, uint32_t hop,
			   uint32_t hres)
{
	return cmd_unary_cmd(c, s, TA_CRYPT_CMD_ARITH_NEG, hop, hres);
}

static TEEC_Result cmd_add(ADBG_Case_t *c, TEEC_Session *s,
			   uint32_t hop1, uint32_t hop2, uint32_t hres)
{
	return cmd_binary_cmd(c, s, TA_CRYPT_CMD_ARITH_ADD, hop1, hop2, hres);
}

static TEEC_Result cmd_sub(ADBG_Case_t *c, TEEC_Session *s,
			   uint32_t hop1, uint32_t hop2, uint32_t hres)
{
	return cmd_binary_cmd(c, s, TA_CRYPT_CMD_ARITH_SUB, hop1, hop2, hres);
}

static TEEC_Result cmd_mul(ADBG_Case_t *c, TEEC_Session *s,
			   uint32_t hop1, uint32_t hop2, uint32_t hres)
{
	return cmd_binary_cmd(c, s, TA_CRYPT_CMD_ARITH_MUL, hop1, hop2, hres);
}

static TEEC_Result cmd_mod(ADBG_Case_t *c, TEEC_Session *s,
			   uint32_t hop1, uint32_t hop2, uint32_t hres)
{
	return cmd_binary_cmd(c, s, TA_CRYPT_CMD_ARITH_MOD, hop1, hop2, hres);
}

static TEEC_Result cmd_invmod(ADBG_Case_t *c, TEEC_Session *s,
			      uint32_t hop1, uint32_t hop2, uint32_t hres)
{
	return cmd_binary_cmd(c, s, TA_CRYPT_CMD_ARITH_INVMOD, hop1, hop2,
			      hres);
}

static TEEC_Result cmd_addmod(ADBG_Case_t *c, TEEC_Session *s,
			      uint32_t hop1, uint32_t hop2, uint32_t hn,
			      uint32_t hres)
{
	return cmd_ternary_cmd(c, s, TA_CRYPT_CMD_ARITH_ADDMOD, hop1, hop2,
			       hn, hres);
}

static TEEC_Result cmd_submod(ADBG_Case_t *c, TEEC_Session *s,
			      uint32_t hop1, uint32_t hop2, uint32_t hn,
			      uint32_t hres)
{
	return cmd_ternary_cmd(c, s, TA_CRYPT_CMD_ARITH_SUBMOD, hop1, hop2,
			       hn, hres);
}

static TEEC_Result cmd_mulmod(ADBG_Case_t *c, TEEC_Session *s,
			      uint32_t hop1, uint32_t hop2, uint32_t hn,
			      uint32_t hres)
{
	return cmd_ternary_cmd(c, s, TA_CRYPT_CMD_ARITH_MULMOD, hop1, hop2,
			       hn, hres);
}

static TEEC_Result cmd_is_prime(ADBG_Case_t *c, TEEC_Session *s, uint32_t hop,
				uint32_t conf_level, int32_t *pres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop;
	op.params[0].value.b = conf_level;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_IS_PRIME, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*pres = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_shift_right(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t hop, uint32_t bits, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop;
	op.params[0].value.b = bits;
	op.params[1].value.a = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_SHIFT_RIGHT, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_to_fmm(ADBG_Case_t *c, TEEC_Session *s, uint32_t hsrc,
			      uint32_t hn, uint32_t hctx, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hsrc;
	op.params[0].value.b = hn;
	op.params[1].value.a = hctx;
	op.params[1].value.b = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_TO_FMM, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_from_fmm(ADBG_Case_t *c, TEEC_Session *s, uint32_t hsrc,
				uint32_t hn, uint32_t hctx, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hsrc;
	op.params[0].value.b = hn;
	op.params[1].value.a = hctx;
	op.params[1].value.b = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_FROM_FMM, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_compute_fmm(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t hop1, uint32_t hop2, uint32_t hn,
				   uint32_t hctx, uint32_t hres)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop1;
	op.params[0].value.b = hop2;
	op.params[1].value.a = hn;
	op.params[1].value.b = hctx;
	op.params[2].value.a = hres;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_COMPUTE_FMM, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_compute_egcd(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t hop1, uint32_t hop2, uint32_t hu,
				   uint32_t hv, uint32_t hgcd)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop1;
	op.params[0].value.b = hop2;
	op.params[1].value.a = hu;
	op.params[1].value.b = hv;
	op.params[2].value.a = hgcd;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_COMPUTE_EGCD, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static TEEC_Result cmd_compute_gcd(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t hop1, uint32_t hop2, uint32_t hgcd)
{
	return cmd_compute_egcd(c, s, hop1, hop2, TA_CRYPT_ARITH_INVALID_HANDLE,
				TA_CRYPT_ARITH_INVALID_HANDLE, hgcd);
}

static int digit_value(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - '0';

	if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;

	if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;

	return -1;
}

static TEEC_Result convert_from_string(ADBG_Case_t *c, TEEC_Session *s,
				       const char *str, uint32_t h)
{
	TEEC_Result res = TEEC_ERROR_BAD_FORMAT;
	size_t spos = strlen(str);
	int32_t sign = 1;
	size_t os_len = (spos + 1) / 2;
	uint8_t *os = calloc(1, os_len);
	int ospos = os_len - 1;
	int nibble = 0;

	if (!os)
		return TEEC_ERROR_OUT_OF_MEMORY;

	while (spos) {
		spos--;
		nibble = digit_value(str[spos]);
		if (nibble == -1)
			break;
		os[ospos] = nibble;

		if (!spos)
			break;
		spos--;
		nibble = digit_value(str[spos]);
		if (nibble == -1)
			break;

		os[ospos] |= nibble << 4;
		ospos--;
	}

	if (spos)
		goto out;

	if (str[spos] == '-')
		sign = -1;

	res = cmd_from_octet_string(c, s, os, os_len, sign, h);
out:
	free(os);
	return res;
}

static TEEC_Result cmd_get_value(ADBG_Case_t *c, TEEC_Session *s, uint8_t *buf,
				 uint32_t *buf_len, int32_t *sign, uint32_t h)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = h;
	op.params[2].tmpref.buffer = buf;
	op.params[2].tmpref.size = *buf_len;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_GET_VALUE, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*sign = op.params[1].value.a;
	*buf_len = op.params[2].tmpref.size;

	return res;
}

static TEEC_Result __maybe_unused print_handle(ADBG_Case_t *c, TEEC_Session *s,
					       const char *name, uint32_t h)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint8_t *os = NULL;
	uint32_t os_len = 0;
	int32_t sign = 0;

	res = cmd_get_value(c, s, os, &os_len, &sign, h);
	if (res) {
		if (!ADBG_EXPECT_TEEC_RESULT(c, TEEC_ERROR_SHORT_BUFFER, res))
			return res;

		os = malloc(os_len);
		if (!ADBG_EXPECT_NOT_NULL(c, os))
			return TEEC_ERROR_OUT_OF_MEMORY;

		res = cmd_get_value(c, s, os, &os_len, &sign, h);
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
			goto out;
	}

	Do_ADBG_Log("%s sign %" PRId32 " (length %" PRIu32 ")",
		    name, sign, os_len);
	Do_ADBG_HexLog(os, os_len, 16);
out:
	free(os);
	return res;
}

static TEEC_Result cmd_get_value_s32(ADBG_Case_t *c, TEEC_Session *s,
				     uint32_t h, int32_t *val)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = h;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_GET_VALUE_S32, &op,
				 &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);
	if (!res)
		*val = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_cmp(ADBG_Case_t *c, TEEC_Session *s, uint32_t hop1,
			   uint32_t hop2, int32_t *cmp_res)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop1;
	op.params[0].value.b = hop2;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_CMP, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	if (!res)
		*cmp_res = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_cmp_s32(ADBG_Case_t *c, TEEC_Session *s, uint32_t hop,
			       int32_t s32, int32_t *cmp_res)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop;
	op.params[0].value.b = s32;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_CMP_S32, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	if (!res)
		*cmp_res = op.params[1].value.a;

	return res;
}

static TEEC_Result cmd_div(ADBG_Case_t *c, TEEC_Session *s, uint32_t hop1,
			   uint32_t hop2, uint32_t hq, uint32_t hr)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = hop1;
	op.params[0].value.b = hop2;
	op.params[1].value.a = hq;
	op.params[1].value.b = hr;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, TA_CRYPT_CMD_ARITH_DIV, &op, &ret_orig);
	ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, ret_orig);

	return res;
}

static void test_4101(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	uint32_t handle = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	Do_ADBG_BeginSubCase(c, "Normal allocation and initialization");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_new_var(c, &session, 512, &handle)))
		goto out;
	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
			handle, !=, TA_CRYPT_ARITH_INVALID_HANDLE))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, &session, handle)))
		goto out;

	Do_ADBG_EndSubCase(c, "Normal allocation and initialization");

	Do_ADBG_BeginSubCase(c, "Zero allocation");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_new_var(c, &session, 0, &handle)))
		goto out;
	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
			handle, !=, TA_CRYPT_ARITH_INVALID_HANDLE))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, &session, handle)))
		goto out;

	Do_ADBG_EndSubCase(c, "Zero allocation");

	Do_ADBG_BeginSubCase(c, "Too large");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_new_var(c, &session, 4096, &handle)))
		goto out;
	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
			handle, !=, TA_CRYPT_ARITH_INVALID_HANDLE))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, &session, handle)))
		goto out;

	Do_ADBG_EndSubCase(c, "Too large");

	Do_ADBG_BeginSubCase(c, "Boundaries");

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_new_var(c, &session, 2048, &handle)))
		goto out;
	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
			handle, !=, TA_CRYPT_ARITH_INVALID_HANDLE))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, &session, handle)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_new_var(c, &session, 2049, &handle)))
		goto out;
	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c,
			handle, !=, TA_CRYPT_ARITH_INVALID_HANDLE))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, &session, handle)))
		goto out;

	Do_ADBG_EndSubCase(c, "Boundaries");
out:
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4101, test_4101,
		"Test TEE Internal API Arithmetical API - Bigint init");

static void test_4102(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	uint32_t ha = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hb = TA_CRYPT_ARITH_INVALID_HANDLE;
	size_t n = 0;
	const struct {
		const uint8_t *os;
		uint32_t os_len;
		const char *str;
		int32_t sign;
	} data[] = {
		{ .os = (const uint8_t []){ 1, 2, 3, 4 },
		  .os_len = 4, .sign = 1, .str = "01020304", },
		{ .os = (const uint8_t []){ 1, 2, 3, 4, 5 },
		  .os_len = 5, .sign = 1, .str = "0102030405", },
		{ .os = (const uint8_t []){
			0x11, 0x22, 0x44, 0x55, 0x66, 0x77, 0x88 },
		  .os_len = 7, .sign = 1, .str = "11224455667788", },
		{ .os = (const uint8_t []){ 0, 1, 2, 3, 4 },
		  .os_len = 5, .sign = 1, .str = "01020304", },
	};

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, &session, 512, &ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, &session, 512, &hb)))
		goto out;

	for (n = 0; n < ARRAY_SIZE(data); n++) {
		int32_t sign = 0;
		int32_t cmp_res = 0;
		size_t len_diff = 0;
		uint8_t os_res[10];
		uint32_t os_len;

		Do_ADBG_BeginSubCase(c, "\"%s\"", data[n].str);

		if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c,
				&session, data[n].str, ha)))
			goto out;

		/* Check that it's read back correctly */
		os_len = data[n].os_len;
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_get_value(c, &session,
				os_res, &os_len, &sign, ha)))
			goto out;
		if (!ADBG_EXPECT_COMPARE_UNSIGNED(c, data[n].os_len,
						  >=, os_len))
			goto out;

		len_diff = data[n].os_len - os_len;
		if (!ADBG_EXPECT_BUFFER(c, data[n].os + len_diff,
					data[n].os_len - len_diff,
					os_res, os_len))
			goto out;
		if (!ADBG_EXPECT_COMPARE_SIGNED(c, sign, ==, data[n].sign))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_from_octet_string(c,
				&session, os_res, os_len, sign, hb)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_cmp(c, &session,
				ha, hb, &cmp_res)))
			goto out;
		if (!ADBG_EXPECT_COMPARE_SIGNED(c, cmp_res, ==, 0))
			goto out;

		Do_ADBG_EndSubCase(c, NULL);
	}
out:
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4102, test_4102,
		"Test TEE Internal API Arithmetical API - Octet string");

static void test_4103(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;
	uint32_t h = TA_CRYPT_ARITH_INVALID_HANDLE;
	int32_t v = 0;
	static const int32_t data[] = {
		1, -1, 123, -123, 0x7FFFFFFF, (int32_t)0x80000000,
		(int32_t)0xFFFFFFFF, 0,
	};
	static const char *data_str[] = { "1FFFFFFFFF", "-1FFFFFFFFF" };

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, &session, 512, &h)))
		goto out;

	for (n = 0; n < ARRAY_SIZE(data); n++) {
		int32_t cmp_res = 0;

		Do_ADBG_BeginSubCase(c, "%" PRId32 " (0x%" PRIx32 ")",
					data[n], (uint32_t)data[n]);

		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				cmd_from_s32(c, &session, h, data[n])))
			goto out;
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				cmd_cmp_s32(c, &session, h, data[n], &cmp_res)))
			goto out;
		if (!ADBG_EXPECT_COMPARE_SIGNED(c, cmp_res, ==, 0))
			goto out;
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				cmd_get_value_s32(c, &session, h, &v))) {
			Do_ADBG_Log("n %zu", n);
			goto out;
		}
		if (!ADBG_EXPECT_COMPARE_SIGNED(c, data[n], ==, v))
			goto out;

		Do_ADBG_EndSubCase(c, NULL);
	}

	for (n = 0; n < ARRAY_SIZE(data_str); n++) {
		Do_ADBG_BeginSubCase(c, "\"%s\" (overflow)", data_str[n]);
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c,
				&session, data_str[n], h)))
			goto out;
		if (!ADBG_EXPECT_TEEC_RESULT(c, TEE_ERROR_OVERFLOW,
				cmd_get_value_s32(c, &session, h, &v)))
			goto out;
		Do_ADBG_EndSubCase(c, NULL);
	}

out:
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4103, test_4103,
		"Test TEE Internal API Arithmetical API - S32");

static TEEC_Result compare_str(ADBG_Case_t *c, TEEC_Session *s,
			       const char str1[], const char str2[],
			       int32_t cmp_res)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	int32_t cres = 0;
	uint32_t h1 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t h2 = TA_CRYPT_ARITH_INVALID_HANDLE;

	res = cmd_new_var(c, s, 512, &h1);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;
	res = cmd_new_var(c, s, 512, &h2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;

	res = convert_from_string(c, s, str1, h1);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;
	res = convert_from_string(c, s, str2, h2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;

	res = cmd_cmp(c, s, h1, h2, &cres);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;

	ADBG_EXPECT_COMPARE_SIGNED(c, cres, ==, cmp_res);
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, h1));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, h2));
	return res;
}

static TEEC_Result compare_s32(ADBG_Case_t *c, TEEC_Session *s,
			       const char str[], int32_t val,
			       int32_t cmp_res)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	int32_t cres = 0;
	uint32_t h = TA_CRYPT_ARITH_INVALID_HANDLE;

	res = cmd_new_var(c, s, 512, &h);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	res = convert_from_string(c, s, str, h);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;

	res = cmd_cmp_s32(c, s, h, val, &cres);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out;

	ADBG_EXPECT_COMPARE_SIGNED(c, cres, ==, cmp_res);
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, h));

	return res;
}

static void test_4104(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;
	const struct {
		const char *str1;
		const char *str2;
		int32_t cmp_res;
	} data[] = {
		{ "0", "1", -1 },
		{ "1", "0", 1 },
		{ "1", "2", -1 },
		{ "2", "1", 1 },
		{ "-1", "FFFFFFFFFF", -1 },
		{ "FFFFFFFFFF", "-1", 1 },
		{ "1", "-FFFFFFFFFF", 1 },
		{ "-FFFFFFFFFF", "1", -1 },
		{ "1", "100000000", -1 },
		{ "0", "0", 0 },
		{ "1", "1", 0 },
		{ "-1", "-1", 0}
	};

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	Do_ADBG_BeginSubCase(c, "Compare bigints");
	for (n = 0; n < ARRAY_SIZE(data); n++) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_str(c, &session,
				data[n].str1, data[n].str2, data[n].cmp_res)))
			goto out;
	}
	Do_ADBG_EndSubCase(c, "Compare bigints");

	const struct {
		const char *str;
		int32_t val;
		int32_t cmp_res;
	} data_s32[] = {
		{"0", 0, 0 },
		{"0", 1, -1 },
		{"1", 0, 1 },
		{"0", -1, 1 },
		{"-1", 0, -1 },
		{"1", 1, 0 },
		{"-1", -1, 0 },
		{"-1", 1, -1 },
		{"1", -1, 1 },
		{"123", 0x123, 0 },
		{"-123", -0x123, 0 },

		/* corner case */
		{"7FFFFFFF", INT32_MAX, 0 },
		{"-7FFFFFFF", INT32_MIN, 1 },
		{"7FFFFFFF", 0, 1 },
		{"-7FFFFFFF", 0, -1 },
		{"-80000000", INT32_MIN, 0 },
		{"80000000", INT32_MAX, 1 },
		{"-80000001", INT32_MIN, -1 },
		{"-7FFFFFFF", INT32_MIN, 1 },

		/* large BigInt */
		{"1FFFFFFFF", 1, 1 },
		{"-1FFFFFFFF", 1, -1 },
	};

	Do_ADBG_BeginSubCase(c, "Compare S32");
	for (n = 0; n < ARRAY_SIZE(data_s32); n++) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_s32(c, &session,
				data_s32[n].str, data_s32[n].val,
				data_s32[n].cmp_res)))
			goto out;
	}
	Do_ADBG_EndSubCase(c, "Compare S32");

out:
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4104, test_4104,
		"Test TEE Internal API Arithmetical API - Compare");

static TEEC_Result compare_handle(ADBG_Case_t *c, TEEC_Session *s,
				  uint32_t hop1, uint32_t hop2, int32_t cmp_res)
{
	int32_t cres = 0;
	TEEC_Result res = cmd_cmp(c, s, hop1, hop2, &cres);

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, cres, ==, cmp_res))
		return TEEC_ERROR_GENERIC;

	return TEEC_SUCCESS;
}

static bool do_addsub(ADBG_Case_t *c, TEEC_Session *s, const char *str_s,
		      const char *str_t, const char *str_r)
{
	bool res = false;
	uint32_t ha = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hb = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hr = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 1024, &ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 1024, &hb)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 1024, &hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 1024, &hr)))
		goto out;

	/* First do the ADD dominated phase */

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_r, hr)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_s, ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_t, hb)))
		goto out;

	/* hc = ha + hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_add(c, s, ha, hb, hc)))
		goto out;
	/* check hc == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hr, 0)))
		goto out;
	/* ha = ha + hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_add(c, s, ha, hb, ha)))
		goto out;
	/* check ha == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, ha, hr, 0)))
		goto out;
	/* ha = ha - hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_sub(c, s, ha, hb, ha)))
		goto out;
	/* hb = ha + hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_add(c, s, ha, hb, hb)))
		goto out;
	/* check hb == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hb, hr, 0)))
		goto out;

	/* Do the SUB dominated phase */

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_s, hr)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_r, ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_t, hb)))
		goto out;

	/* hc = ha - hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_sub(c, s, ha, hb, hc)))
		goto out;
	/* check hc == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hr, 0)))
		goto out;
	/* ha = ha - hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_sub(c, s, ha, hb, ha)))
		goto out;
	/* check ha == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, ha, hr, 0)))
		goto out;
	/* ha = ha + hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_add(c, s, ha, hb, ha)))
		goto out;
	/* hb = ha - hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_sub(c, s, ha, hb, hb)))
		goto out;
	/* check hb == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hb, hr, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, ha));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hb));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hr));

	return res;
}

static void test_4105(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;
	static const struct {
		const char *s;
		const char *t;
		const char *r;
	} data[] = {
		{ "1", "1", "2" },
		{ "-1", "1", "0" },
		{ "0", "0", "0" },
		{ "0", "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
		  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" },
		/* single word, no carry */
		{ "FF", "FF", "1FE" },
		/* single word, with carry */
		{ "FFFFFFFF", "FFFFFFFF", "1FFFFFFFE" },
		/* mult word with partial carry */
		{ "FFFFFFFF", "100000000FFFFFFFFFFFFFFFF",
		  "10000000100000000FFFFFFFE" },
		/* mult word with carry all the way */
		{ "FFFFFFFFFFFFFFFF", "FFFFFFFFFFFFFFFF",
		  "1FFFFFFFFFFFFFFFE" },
		/* single word, no carry */
		{ "-FF", "-FF", "-1FE" },
		/* single word, with carry */
		{ "-FFFFFFFF", "-FFFFFFFF", "-1FFFFFFFE" },
		/* mult word with partial carry */
		{ "-FFFFFFFF", "-100000000FFFFFFFFFFFFFFFF",
		  "-10000000100000000FFFFFFFE" },
		/* mult word with carry */
		{ "-FFFFFFFFFFFFFFFF", "-FFFFFFFFFFFFFFFF",
		  "-1FFFFFFFFFFFFFFFE" },
		/* single word, no carry */
		{ "FFFF", "-FF", "FF00" },
		/* single word, with carry */
		{ "F00000000", "-00000FFFF", "EFFFF0001" },
		/* multi words with carry */
		{ "FFFFFFFF00000000", "-FFFFFFFF", "FFFFFFFE00000001" },
		{ "10000000FFFFFFFF00000000", "-FFFFFFFF",
		  "10000000FFFFFFFE00000001" },
		{ "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
		  "-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE", "1" },
		{ "FF", "-FFFF", "-FF00" },
		{ "FFFFFFFF", "-1FFFFFFFF", "-100000000" },
		/* single word, no carry */
		{ "-FFFF", "FF", "-FF00" },
		/* single word, with carry */
		{ "-F00000000", "00000FFFF", "-EFFFF0001" },
		/* multi words with carry */
		{ "-FFFFFFFF00000000", "FFFFFFFF", "-FFFFFFFE00000001" },
		{ "-10000000FFFFFFFF00000000", "FFFFFFFF",
		  "-10000000FFFFFFFE00000001" },
		{ "-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
		  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE", "-1" },
		{ "-FF", "FFFF", "FF00" },
		{ "-FFFFFFFF", "1FFFFFFFF", "100000000" },
	};

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(data); n++) {
		if (!ADBG_EXPECT_TRUE(c, do_addsub(c, &session, data[n].s,
				data[n].t, data[n].r))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4105, test_4105,
		"Test TEE Internal API Arithmetical API - Add, Sub");

static void test_4106(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;
	uint32_t h1 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t h2 = TA_CRYPT_ARITH_INVALID_HANDLE;
	static const struct {
		const char *val;
		const char *neg_val;
	} data[] = {
		{ "0", "0" },
		{ "1", "-1" },
		{ "123", "-123" },
		{ "123456789123456789", "-123456789123456789" },
	};

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, &session, 1024, &h1)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, &session, 1024, &h2)))
		goto out;

	for (n = 0; n < ARRAY_SIZE(data); n++) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c,
				&session, data[n].val, h1)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c,
				&session, data[n].neg_val, h2)))
			goto out;

		if (n && !ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c,
				&session, h1, h2, 1)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_neg(c, &session, h1, h1)))
			goto out;
		if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, &session,
				h1, h2, 0)))
			goto out;
	}
out:
	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4106, test_4106,
		"Test TEE Internal API Arithmetical API - Neg");

static bool do_mul(ADBG_Case_t *c, TEEC_Session *s, const char *str_s,
		      const char *str_t, const char *str_r)
{
	bool res = false;
	uint32_t ha = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hb = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hr = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 1024, &ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 1024, &hb)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hr)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_r, hr)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_s, ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_t, hb)))
		goto out;

	/* hc = ha * hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mul(c, s, ha, hb, hc)))
		goto out;
	/* check hc == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hr, 0)))
		goto out;
	/* ha = ha * hb */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mul(c, s, ha, hb, ha)))
		goto out;
	/* check ha == hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, ha, hr, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, ha));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hb));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hr));

	return res;
}

static void test_4107(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;
	static const struct {
		const char *s;
		const char *t;
		const char *r;
	} data[] = {
		{ "0", "0", "0" },
		{ "0", "FFFFFFFF", "0" },
		{ "1", "1", "1" },
		{ "-1", "1", "-1" },
		{ "-1", "-1", "1" },
		{ "FF", "1", "FF" },
		{ "2", "2", "4" },
		{ "3", "3", "9" },
		{ "100", "100", "10000" },
		{ "FFFFFFFF", "FFFFFFFF", "FFFFFFFE00000001" },
		{ "4F239BBAE89A447149CDB0B50A103C69591DD9E0C91A57955A6C266"
		  "C7ED42A5ED5F4",
		  "44FF5A67036657E041D55AE42AE25517A1",
		  "155465C8221717FFC135C87ABF6D34184DF5E6906D2EBA7C364879AA"
		  "0BE840FD06F1E0A7036BC3B7B1844FF95F07A39CE17A74" },
		{ "4F239BBAE89A447149CDB0B50A103C69591DD9E0C91A57955A6C266C"
		  "7ED42A5ED5F4",
		  "-44FF5A67036657E041D55AE42AE25517A1",
		  "-155465C8221717FFC135C87ABF6D34184DF5E6906D2EBA7C364879A"
		  "A0BE840FD06F1E0A7036BC3B7B1844FF95F07A39CE17A74" },
		{ "-4F239BBAE89A447149CDB0B50A103C69591DD9E0C91A57955A6C266"
		  "C7ED42A5ED5F4",
		  "-44FF5A67036657E041D55AE42AE25517A1",
		  "155465C8221717FFC135C87ABF6D34184DF5E6906D2EBA7C364879AA"
		  "0BE840FD06F1E0A7036BC3B7B1844FF95F07A39CE17A74" },
	};

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(data); n++) {
		if (!ADBG_EXPECT_TRUE(c, do_mul(c, &session, data[n].s,
				data[n].t, data[n].r))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4107, test_4107,
		"Test TEE Internal API Arithmetical API - Mul");

static bool do_div(ADBG_Case_t *c, TEEC_Session *s, const char *str_s,
		   const char *str_t, const char *str_out, int32_t qsign,
		   int32_t rsign)
{
	bool res = false;
	int32_t cres = 0;
	uint32_t hn = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hd = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hq = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hr = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hv = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hout = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hd)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hq)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hr)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hv)))
		goto out;

	if (str_out) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				cmd_new_var(c, s, 2048, &hout)))
			goto out;
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				convert_from_string(c, s, str_out, hout)))
			goto out;
	}

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_s, hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_t, hd)))
		goto out;

	/* (hq, hr) = n / d */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_div(c, s, hn, hd, hq, hr)))
		goto out;
	if (str_out) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				compare_handle(c, s, hq, hout, 0)))
			goto out;
	}

	/* hv = hq * hd */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mul(c, s, hq, hd, hv)))
		goto out;
	/* hv = hv + hr */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_add(c, s, hv, hr, hv)))
		goto out;
	/* check hn == hv */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hn, hv, 0)))
		goto out;

	if (qsign) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				cmd_cmp_s32(c, s, hq, 0, &cres)))
			goto out;
		if (!ADBG_EXPECT_COMPARE_SIGNED(c, cres, ==, qsign))
			goto out;
	}

	if (rsign) {
		if (!ADBG_EXPECT_TEEC_SUCCESS(c,
				cmd_cmp_s32(c, s, hr, 0, &cres)))
			goto out;
		if (!ADBG_EXPECT_COMPARE_SIGNED(c, cres, ==, rsign))
			goto out;
	}

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hn));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hd));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hq));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hr));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hv));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hout));

	return res;
}

static void test_4108(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;
	static const struct {
		const char *s;
		const char *t;
		const char *out;
		int32_t qsign;
		int32_t rsign;
	} data[] = {
		{ "811111100000000112345678", "8000000012345678" },
		{ "0", "1", "0" },
		{ "1", "1", "1" },
		{ "b7fb", "5", "24CB" },
		{ "124378912734891273894712890347102358129034789120374",
		  "1984086C15FA011154C86FA68", "B73D14EC7205D3311F6E78411D" },
		{ "124378912734891273894712890347102358129034789120374",
		  "1984086C15FA011154C86FA68" },
		{ "-124378912734891273894712890347102358129034789120374",
		  "1984086C15FA011154C86FA68" },
		{ "124378912734891273894712890347102358129034789120374",
		  "-1984086C15FA011154C86FA68" },
		{ "-124378912734891273894712890347102358129034789120374",
		  "-1984086C15FA011154C86FA68" },
		{ "12345678", "10" },
		{ "-12345678", "10" },
		{ "12345678", "-10" },
		{ "-12345678", "-10" },
		{ "12345678901234567890123456789012345678901", "10" },
		{ "1234567890123456789012345678901234567890", "10" },
		{ "123456789012345678901234567890123456789", "10" },
		{ "53", "7", NULL, 1, 1 },
		{ "-53", "7", NULL, -1, -1 },
		{ "53", "-7", NULL, -1, 1 },
		{ "-53", "-7", NULL, 1, -1 },
		{ "123456789abcdef123456789abcdef", "fedcba98765432100",
		  NULL, 1, 1 },
		{ "-123456789abcdef123456789abcdef", "fedcba98765432100",
		  NULL, -1, -1 },
		{ "123456789abcdef123456789abcdef", "-fedcba98765432100",
		  NULL, -1, 1 },
		{ "-123456789abcdef123456789abcdef", "-fedcba98765432100",
		  NULL, 1, -1 },
	};

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(data); n++) {
		if (!ADBG_EXPECT_TRUE(c, do_div(c, &session, data[n].s,
				data[n].t, data[n].out, data[n].qsign,
				data[n].rsign))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4108, test_4108,
		"Test TEE Internal API Arithmetical API - Div");

static const struct {
	const char *op;
	const char *n;
	const char *res;
} test_4109_data[] = {
	{ "10", "A", "6" },
	{ "10", "20", "10" },
	{ "-1", "123", "122" },
	{ "-122", "123", "1" },
	{ "-1", "100000000000000000000", "FFFFFFFFFFFFFFFFFFFF" },

	{	"42176E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A",
		"A9A9C49F0015DBEF9EB0D927F379821AE2C59",
		"3460EB99F07BC4617A7586E6522B98F66E5B2" },

	{	"42176E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A",
		"1234", "1082" },

	{	"FFFF6E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A",
		"2", "0" },

	{	"FFFF6E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A",
		"FFFF6E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A",
		"0" },

	{	"FFFF6E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A",
		"FFFF6E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828B",
		"FFFF6E7CD0386C94194C3C3DACFA7237"
		"CCF4E0829E2EBEAB59DE12CAE3305C9E"
		"576F837A42D9C13BB1D83E4DFD330B65"
		"76FD516B2AE9FBDD152F0A820C5DD18E"
		"D048D476EC791DC989D6D8C30B2B30FF"
		"C2A4ABAC082C672B38E138610B74CE0C"
		"30D1B378BE6FE0C44E09EC11586245CD"
		"8F1216619392E6B2C857C71EA78DD4D8"
		"61A482D9D1251C341D1D6205AFB2BFB5"
		"3DD1F272B5624FA61B020936745387CD"
		"F1F3EBC4435CBE6C6B323497098FDE9D"
		"61AA0CC87416235E706A71F73EC3C291"
		"D9ACA2EECD712DA40966DD251310AE94"
		"9713F69B2A32E42732A7F6669547D88A"
		"415B25C582FDBBF06C5EC5F5750053D4"
		"9F7ED68DED168C92F9C531828A" },
};

static bool test_4109_mod(ADBG_Case_t *c, TEEC_Session *s, const char *str_op,
			  const char *str_n, const char *str_res)
{
	bool res = false;
	uint32_t hop = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hn = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hres = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hop)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hres)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_res, hres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op, hop)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_n, hn)))
		goto out;

	/* hc = hop mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mod(c, s, hop, hn, hc)))
		goto out;
	/* check hc == hres */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hres, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hn));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hres));

	return res;
}

static void test_4109(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(test_4109_data); n++) {
		if (!ADBG_EXPECT_TRUE(c, test_4109_mod(c, &session,
				test_4109_data[n].op, test_4109_data[n].n,
				test_4109_data[n].res))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4109, test_4109,
		"Test TEE Internal API Arithmetical API - Mod");

static const char n2048str[] = "F9000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "00000000000000000000000000000000"
			       "000000000000000000000000000005C5";

static const struct {
	const char *op1;
	const char *op2;
	const char *n;
	const char *addres;
	const char *mulres;
} test_4110_data[] = {
	{	"0", "0", "2", "0", "0" },
	{	"1", "1", "2", "0", "1" },
	{	"2", "2", "9", "4", "4" },
	{	"123", "123", "200", "46", "C9" },
	{	"0", "123", "200", "123", "0" },
	{	"123", "0", "200", "123", "0" },

	/* Testing some 2048 bit numbers */
	{	"F9000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		,
		"F9000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000001",
		n2048str,
		"F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFA3C",
		"2143D4" },

	{	"F9000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000"
		"00000000000000000000000000000000",
		"5C5", n2048str, "0",
		"F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
		"FFFFFFFFFFFFFFFFFFFFFFFFFFDEBC2C" },
};

static bool test_4110_mod(ADBG_Case_t *c, TEEC_Session *s,
			  const char *str_op1, const char *str_op2,
			  const char *str_n, const char *str_add_res,
			  const char *str_mul_res)
{
	bool res = false;
	uint32_t hop1 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hop2 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hn = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t haddres = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hmulres = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hop1)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hop2)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &haddres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hmulres)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_add_res, haddres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_mul_res, hmulres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op1, hop1)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op2, hop2)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_n, hn)))
		goto out;

	/* hc = (hop1 + hop2) mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_addmod(c, s, hop1, hop2, hn, hc)))
		goto out;
	/* check hc == haddres */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, haddres, 0)))
		goto out;
	/* hc = (haddres - hop2) mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_submod(c, s, haddres, hop2, hn,
						    hc)))
		goto out;
	/* check hc == hop1 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hop1, 0)))
		goto out;

	/* hc = (hop1 * hop2) mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mulmod(c, s, hop1, hop2, hn, hc)))
		goto out;
	/* check hc == hmulres */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hmulres, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop1));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop2));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hn));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, haddres));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hmulres));

	return res;
}

static bool test_4110_fmm(ADBG_Case_t *c, TEEC_Session *s,
			  const char *str_op1, const char *str_op2,
			  const char *str_n, const char *str_res)
{
	bool res = false;
	uint32_t hop1 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hop2 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hn = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hres = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hfmmres = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hctx = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t nbitsize = 0;
	uint32_t n_is_odd = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_fmm_var(c, s, 2048, &hop1)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_fmm_var(c, s, 2048, &hop2)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_fmm_var(c, s, 2048, &hfmmres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hc)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, str_n, hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_res, hres)))
		goto out;
	/* Check if FMM is applicable on this modulus */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_get_bit(c, s, hn, 0, &n_is_odd)))
		goto out;
	if (!n_is_odd) {
		res = true;
		goto out;
	}

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_get_bit_count(c, s, hn, &nbitsize)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_new_fmm_ctx(c, s, nbitsize, hn, &hctx)))
		goto out;

	/* hop1 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op1, hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_to_fmm(c, s, hc, hn, hctx, hop1)))
		goto out;

	/* hop2 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op2, hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_to_fmm(c, s, hc, hn, hctx, hop2)))
		goto out;

	/* hfmmres = hop1 * hop2 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_compute_fmm(c, s, hop1, hop2, hn, hctx, hfmmres)))
		goto out;
	/* hc = hfmmres mod hn*/
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_from_fmm(c, s, hfmmres, hn, hctx, hc)))
		goto out;
	/* check hc == hres */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hres, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop1));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop2));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hn));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hres));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hfmmres));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hctx));
	return res;
}

static void test_4110(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(test_4110_data); n++) {
		if (!ADBG_EXPECT_TRUE(c, test_4110_mod(c, &session,
				test_4110_data[n].op1, test_4110_data[n].op2,
				test_4110_data[n].n,
				test_4110_data[n].addres,
				test_4110_data[n].mulres))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
		if (!ADBG_EXPECT_TRUE(c, test_4110_fmm(c, &session,
				test_4110_data[n].op1, test_4110_data[n].op2,
				test_4110_data[n].n,
				test_4110_data[n].mulres))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4110, test_4110,
		"Test TEE Internal API Arithmetical API - Mod arith");

static const struct {
	const char *op;
	const char *n;
	const char *res;
} test_4111_data[] = {
	{	"2", "3", "2" },
	{	"3", "7", "5" },
	{	"5", "7", "3" },
	{	"7B", "FA1", "8E5" },
	{	"462D578CF0F88", "AB54A98CEB1F0AD3", "5ABBF22B54DBD000" },
	{	"7", n2048str,
		"B1DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6"
		"DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB"
		"6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6D"
		"B6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6"
		"DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB"
		"6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6D"
		"B6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6"
		"DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB"
		"6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6D"
		"B6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6"
		"DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB"
		"6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6D"
		"B6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6"
		"DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB"
		"6DB6DB6DB6DB6DB6DB6DB6DB6DB6DB6D"
		"B6DB6DB6DB6DB6DB6DB6DB6DB6DB71D6" },
};

static bool test_4111_invmod(ADBG_Case_t *c, TEEC_Session *s,
			     const char *str_op, const char *str_n,
			     const char *str_res)
{
	bool res = false;
	uint32_t hop = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hn = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hres = TA_CRYPT_ARITH_INVALID_HANDLE;
	int32_t cres = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hop)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hn)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hres)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_res, hres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op, hop)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_n, hn)))
		goto out;

	/* hc = hop^-1 mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_invmod(c, s, hop, hn, hc)))
		goto out;
	/* check hc == hres */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hres, 0)))
		goto out;

	/* hc = (hres * hop) mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mulmod(c, s, hres, hop, hn, hc)))
		goto out;
	/* check hc == 1 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_cmp_s32(c, s, hc, 1, &cres)))
		goto out;
	if (!ADBG_EXPECT_COMPARE_SIGNED(c, cres, ==, 0))
		goto out;

	/* hc = hres^-1 mod hn */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_invmod(c, s, hres, hn, hc)))
		goto out;
	/* check hc == hop */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hop, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hn));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hres));

	return res;
}

static void test_4111(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(test_4111_data); n++) {
		if (!ADBG_EXPECT_TRUE(c, test_4111_invmod(c, &session,
				test_4111_data[n].op, test_4111_data[n].n,
				test_4111_data[n].res))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4111, test_4111,
		"Test TEE Internal API Arithmetical API - Invmod");

static const struct {
	uint32_t level;
	int32_t res1;
	int32_t res2;
	const char *op;
} test_4112_data[] = {
	/* Simple cases */
	{	80, 0, 0, "0" },
	{	80, 0, 0, "1" },
	{	80, 1, -1, "2" },
	{	80, 1, -1, "3" },
	{	80, 1, -1, "17" },
	{	80, 1, -1, "FA1" },
	{	80, 0, 0, "FA0" },
	/*
	 * Testing large composites and the only allowed classification is
	 * "composite", aka "0".
	 */
	{	80, 0, 0,  "136233FDE5569" },
	{	80, 0, 0,  "177B96388FD94D5D7EC9AE513" },
	{	80, 0, 0,  "22770A7DC599BC90B2FF981CCB5CF057"
			   "03344C8F3504189ED" },
	{	80, 0, 0,  "0002ECD53B2150542D1494A424D50322"
			   "EBABBBD5DB89392A368D7759" },
	{	80, 0, 0,  "00000004C8971C2B6F7A8503AB80749E"
			   "2BBA5BB6B6031BD43564DCA8003F0E3E"
			   "54E677A0A4B1B6153662B3130C11C273"
			   "C0F9408445B2ABB47D64153BEB48C154"
			   "659FDFDEDC496C8EA81AA7B7CE2B2CB1"
			   "AA7C8B407DE8C86F122607B2A226385D"
			   "9432F6CB30B5E4DE033E3363D48A1865"
			   "D9BC95DC7883D9DC9331691A17964979" },
	{	80, 0, 0,  "00038AFFFFFFFFFFFFFFFFFFFFFFFFFF"
			   "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
			   "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
			   "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
			   "FFFFFFFFFFFFFFEA5C6D11EDD2905192"
			   "C5429E6A6D796E89250BF563DA53D393"
			   "25129B70435894B56E337D5866F0BEF6"
			   "63D6C26DA889B69D4B1508ED63A17AFD"
			   "8BC0C5BE09AC18B64B2B7D87279D32EB"
			   "4F3375C10D58DF1F7CA2147A61CFA557"
			   "235244D4A4700AB5953C4C6ED428EB5F"
			   "09F07DAED46A995B8FFE48EFB93390C9"
			   "60685A31987879F818CCE9B9816E13DE"
			   "E4BEA5CC38C56A5D1983425DDD909C88"
			   "463B447F" },
	/* Large primes */
	{	80, 1, -1, "000136A352B2C7A5" },
	{	80, 1, -1, "000000022770A7DC599BC90B2FF981CC"
			   "B5CF05703344C8F350418AAD" },

	{	80, 1, -1, "000067E04F440C5FECCA2694B1A4AB4F"
			   "CEC2D2B2D286FC93DD259D2B36A3A00D"
			   "F3726A4D4DFA99195998D92F43DD7370"
			   "B8C079E23072036F0C13F3F6ABA1D6DF" },

	{	80, 1, -1, "000BC9F7E42CE74132EC6933DE1B8357"
			   "CD2AEB9E4B8A5F26BD233D1E6F295555"
			   "4D2CFA9760F3F69CCC52E8E428F68C37"
			   "AD5BC70CDB12F4DCEF23CB101C36A2A7" },

	{	80, 1, -1, "00000005233FE157B5B44102598513AE"
			   "BB8876B0D1791769C8A1B0F36875D06F"
			   "E26951C7372192C7B21CAB67ADF0D6FE"
			   "1A718FB3FC4A51454DE87CD3F852005B"
			   "AAEC35D5" },

	{	80, 1, -1, "0000001A655954FBC4408E09FE97EB82"
			   "5E0F096426B42317A36E53FF9608DD3F"
			   "A661040245906334496987EFB14BEA5F"
			   "7C050444C9B22A09EC76AF4EBDC609D8"
			   "A90C05E9E5AF06829C542DFCF7E6B9DC"
			   "0B065659C1816309CD6012C37F8CE649"
			   "8DECF850F33943F0F83081406846D306"
			   "22E0BCE697E612FC1F7A5D902280D6E0"
			   "3009E585" },
};

static bool test_4112_is_prime(ADBG_Case_t *c, TEEC_Session *s,
			       uint32_t conf_level, int32_t res1, int32_t res2,
			       const char *str_val)
{
	bool res = false;
	uint32_t hop = TA_CRYPT_ARITH_INVALID_HANDLE;
	int32_t pres = 11;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hop)))
		return res;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_val, hop)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_is_prime(c, s, hop, conf_level,
						      &pres)))
		goto out;
	if (!ADBG_EXPECT_TRUE(c, pres == res1 || pres == res2)) {
		Do_ADBG_Log("Unexpected prime clasification %" PRId32 ", expected %" PRId32 " or %" PRId32,
			    pres, res1, res2);
		goto out;
	}

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop));

	return res;
}

static void test_4112(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(test_4112_data); n++) {
		if (!ADBG_EXPECT_TRUE(c, test_4112_is_prime(c, &session,
				test_4112_data[n].level,
				test_4112_data[n].res1,
				test_4112_data[n].res2,
				test_4112_data[n].op))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4112, test_4112,
		"Test TEE Internal API Arithmetical API - Is prime");

static const struct {
	const char *res;
	size_t bits;
	const char *op;
} test_4113_data[] = {
	{ "0", 31, "7FFFFFFF" },
	{ "0", 65, "7FFFFFFF" },
	{ "0", 1, "0" },
	{ "-1",1, "-2" },
	{ "1", 0, "1" },
	{ "1", 1, "2" },
	{ "1", 2, "4" },
	{ "1", 3, "8" },
	{ "1", 4, "10" },
	{ "1", 5, "20" },
	{ "1", 6, "40" },
	{ "1", 7, "80" },
	{ "1", 8, "100" },
	{ "1", 9, "200" },
	{ "1", 10, "400" },
	{ "1", 11, "800" },
	{ "1", 12, "1000" },
	{ "1", 13, "2000" },
	{ "1", 14, "4000" },
	{ "1", 15, "8000" },
	{ "1", 16, "10000" },
	{ "1", 17, "20000" },
	{ "1", 18, "40000" },
	{ "1", 19, "80000" },
	{ "1", 20, "100000" },
	{ "1", 21, "200000" },
	{ "1", 22, "400000" },
	{ "1", 23, "800000" },
	{ "1", 24, "1000000" },
	{ "1", 25, "2000000" },
	{ "1", 26, "4000000" },
	{ "1", 27, "8000000" },
	{ "1", 28, "10000000" },
	{ "1", 29, "20000000" },
	{ "1", 30, "40000000" },
	{ "1", 31, "80000000" },
	{ "1", 32, "100000000" },
	{ "1", 64, "10000000000000000" },
	{ "FFFFFFFF", 64, "FFFFFFFF0000000000000000" },
	{ "1111FFFF0000", 64," 1111FFFF00000000000000000000" },
	{ "1", 33, "200000000" },
	{ "1", 65, "20000000000000000" },
	{ "FFFFFFFF", 65, "1FFFFFFFE0000000000000000" },
	{ "24B0D975EC8FCC8E1D54CA4BF7ACFC4534F04", 81,
	  "4961B2EBD91F991C3AA99497EF59F88A69E08AD5C340167793C3CC32F" },
};

static bool test_4113_shift_right(ADBG_Case_t *c, TEEC_Session *s,
				  const char *str_op, size_t bits,
				  const char *str_res)
{
	bool res = false;
	uint32_t hop = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hc = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hres = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hop)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hc)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hres)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_res, hres)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			convert_from_string(c, s, str_op, hop)))
		goto out;

	/* hc = hop shift_right bits */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_shift_right(c, s, hop, bits, hc)))
		goto out;
	/* check hc == hres */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hc, hres, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hop));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hc));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hres));

	return res;
}

static void test_4113(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(test_4113_data); n++) {
		if (!ADBG_EXPECT_TRUE(c, test_4113_shift_right(c, &session,
				test_4113_data[n].op, test_4113_data[n].bits,
				test_4113_data[n].res))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4113, test_4113,
		"Test TEE Internal API Arithmetical API - shift right");

static const struct {
	const char *strx;
	const char *stry;
} test_4114_data[] = {
	{ "0", "0" },
	{ "0", "123" },
	{ "1", "1" },
	{ "1", "-1" },
	{ "2", "2" },
	{ "-2", "2" },
	{ "2", "4" },
	{ "-2", "-4" },
	{ "400", "800" },
	{ "261", "2B5" },
	{ "F", "A" },
	{ "C", "13" },
	{ "165D662", "1664FEA" },
	{ "AB59CDFD83CE2B24",
	  "4961BF04008953A7F9567AAFBA94D4AF55F473F14FD68AA022982F0FE" },
	/* two large primes */
	{ "5D0A380DC40EDE5E036FA051FC6D7F93",
	  "3277FD425328576569AFB2EAC6B1430578099CA8ADA4BC73" },
	/* two large powers of 2 */
	{ "20000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "0",
	  "40000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "00000000000000000000000000000000"
	  "000000000000000000000" },
};

static bool test_4114_gcd(ADBG_Case_t *c, TEEC_Session *s, const char *strx,
			  const char *stry)
{
	bool res = false;
	uint32_t hx = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hy = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t ha = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hb = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t hgcd = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t ht1 = TA_CRYPT_ARITH_INVALID_HANDLE;
	uint32_t ht2 = TA_CRYPT_ARITH_INVALID_HANDLE;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hx)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hy)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &ha)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hb)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &hgcd)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &ht1)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_new_var(c, s, 2048, &ht2)))
		goto out;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, strx, hx)))
		goto out;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, convert_from_string(c, s, stry, hy)))
		goto out;

	/* (hgcd, a, b) = ExtendedGCD(x, y) */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			cmd_compute_egcd(c, s, hx, hy, ha, hb, hgcd)))
		goto out;
	/* ht1 = GCD(x, y) */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_compute_gcd(c, s, hx, hy, ht1)))
		goto out;
	/* check hgcd == ht1 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hgcd, ht1, 0)))
		goto out;

	/* ht1 = a * x */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mul(c, s, ha, hx, ht1)))
		goto out;
	/* ht2 = b * y */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_mul(c, s, hb, hy, ht2)))
		goto out;
	/* ht1 = ht1 + ht2 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, cmd_add(c, s, ht1, ht2, ht1)))
		goto out;
	/* check hgcd == ht1 */
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, compare_handle(c, s, hgcd, ht1, 0)))
		goto out;

	res = true;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hx));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hy));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, ha));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hb));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, hgcd));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, ht1));
	ADBG_EXPECT_TEEC_SUCCESS(c, cmd_free_handle(c, s, ht2));

	return res;
}

static void test_4114(ADBG_Case_t *c)
{
	TEEC_Session session = { };
	uint32_t ret_orig = 0;
	size_t n = 0;

	if (!ADBG_EXPECT_TEEC_SUCCESS(c,
			xtest_teec_open_session(&session, &crypt_user_ta_uuid,
						NULL, &ret_orig)))
		return;

	for (n = 0; n < ARRAY_SIZE(test_4114_data); n++) {
		if (!ADBG_EXPECT_TRUE(c, test_4114_gcd(c, &session,
				test_4114_data[n].strx,
				test_4114_data[n].stry))) {
			Do_ADBG_Log("n %zu", n);
			break;
		}
	}

	TEEC_CloseSession(&session);
}
ADBG_CASE_DEFINE(regression, 4114, test_4114,
		"Test TEE Internal API Arithmetical API - GCD");
