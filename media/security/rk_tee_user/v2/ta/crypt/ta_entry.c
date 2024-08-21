// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include <aes_taf.h>
#include <arith_taf.h>
#include <cryp_taf.h>
#include <derive_key_taf.h>
#include <mbedtls_taf.h>
#include <seed_rng_taf.h>
#include <sha2_taf.h>
#include <ta_crypt.h>
#include <tee_ta_api.h>
#include <trace.h>

static TEE_Result set_global(uint32_t param_types, TEE_Param params[4]);
static TEE_Result get_global(uint32_t param_types, TEE_Param params[4]);
static int _globalvalue;

/*
 * Trusted Application Entry Points
 */

/* Called each time a new instance is created */
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

/* Called each time an instance is destroyed */
void TA_DestroyEntryPoint(void)
{
}

/* Called each time a session is opened */
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes,
				    TEE_Param pParams[4],
				    void **ppSessionContext)
{
	(void)nParamTypes;
	(void)pParams;
	(void)ppSessionContext;
	return TEE_SUCCESS;
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext)
{
	(void)pSessionContext;
}

/*
 * To provoke the linker to produce R_ARM_ABS32 relocations we need to
 * pre-initilize a pointer to the function and then also call the function
 * directly.
 */
static TEE_Result (*ta_cmd_entries[])(uint32_t, TEE_Param *) = {
	[TA_CRYPT_CMD_SHA224] = ta_entry_sha224,
	[TA_CRYPT_CMD_SHA256] = ta_entry_sha256,
};

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
	static bool use_fptr = false;

	(void)pSessionContext;

	switch (nCommandID) {
	case TA_CRYPT_CMD_SHA224:
		use_fptr = !use_fptr;
		if (use_fptr)
			return ta_cmd_entries[nCommandID](nParamTypes, pParams);
		else
			return ta_entry_sha224(nParamTypes, pParams);

	case TA_CRYPT_CMD_SHA256:
		use_fptr = !use_fptr;
		if (use_fptr)
			return ta_cmd_entries[nCommandID](nParamTypes, pParams);
		else
			return ta_entry_sha256(nParamTypes, pParams);

	case TA_CRYPT_CMD_AES256ECB_ENC:
		return ta_entry_aes256ecb_encrypt(nParamTypes, pParams);

	case TA_CRYPT_CMD_AES256ECB_DEC:
		return ta_entry_aes256ecb_decrypt(nParamTypes, pParams);

	case TA_CRYPT_CMD_ALLOCATE_OPERATION:
		return ta_entry_allocate_operation(nParamTypes, pParams);

	case TA_CRYPT_CMD_FREE_OPERATION:
		return ta_entry_free_operation(nParamTypes, pParams);

	case TA_CRYPT_CMD_GET_OPERATION_INFO:
		return ta_entry_get_operation_info(nParamTypes, pParams);

	case TA_CRYPT_CMD_RESET_OPERATION:
		return ta_entry_reset_operation(nParamTypes, pParams);

	case TA_CRYPT_CMD_SET_OPERATION_KEY:
		return ta_entry_set_operation_key(nParamTypes, pParams);

	case TA_CRYPT_CMD_SET_OPERATION_KEY2:
		return ta_entry_set_operation_key2(nParamTypes, pParams);

	case TA_CRYPT_CMD_COPY_OPERATION:
		return ta_entry_copy_operation(nParamTypes, pParams);

	case TA_CRYPT_CMD_DIGEST_UPDATE:
		return ta_entry_digest_update(nParamTypes, pParams);

	case TA_CRYPT_CMD_DIGEST_DO_FINAL:
		return ta_entry_digest_do_final(nParamTypes, pParams);

	case TA_CRYPT_CMD_CIPHER_INIT:
		return ta_entry_cipher_init(nParamTypes, pParams);

	case TA_CRYPT_CMD_CIPHER_UPDATE:
		return ta_entry_cipher_update(nParamTypes, pParams);

	case TA_CRYPT_CMD_CIPHER_DO_FINAL:
		return ta_entry_cipher_do_final(nParamTypes, pParams);

	case TA_CRYPT_CMD_MAC_INIT:
		return ta_entry_mac_init(nParamTypes, pParams);

	case TA_CRYPT_CMD_MAC_UPDATE:
		return ta_entry_mac_update(nParamTypes, pParams);

	case TA_CRYPT_CMD_MAC_FINAL_COMPUTE:
		return ta_entry_mac_final_compute(nParamTypes, pParams);

	case TA_CRYPT_CMD_MAC_FINAL_COMPARE:
		return ta_entry_mac_final_compare(nParamTypes, pParams);

	case TA_CRYPT_CMD_ALLOCATE_TRANSIENT_OBJECT:
		return ta_entry_allocate_transient_object(nParamTypes, pParams);

	case TA_CRYPT_CMD_FREE_TRANSIENT_OBJECT:
		return ta_entry_free_transient_object(nParamTypes, pParams);

	case TA_CRYPT_CMD_RESET_TRANSIENT_OBJECT:
		return ta_entry_reset_transient_object(nParamTypes, pParams);

	case TA_CRYPT_CMD_POPULATE_TRANSIENT_OBJECT:
		return ta_entry_populate_transient_object(nParamTypes, pParams);

	case TA_CRYPT_CMD_COPY_OBJECT_ATTRIBUTES:
		return ta_entry_copy_object_attributes(nParamTypes, pParams);

	case TA_CRYPT_CMD_GENERATE_KEY:
		return ta_entry_generate_key(nParamTypes, pParams);

	case TA_CRYPT_CMD_ASYMMETRIC_ENCRYPT:
		return ta_entry_asymmetric_encrypt(nParamTypes, pParams);

	case TA_CRYPT_CMD_ASYMMETRIC_DECRYPT:
		return ta_entry_asymmetric_decrypt(nParamTypes, pParams);

	case TA_CRYPT_CMD_ASYMMETRIC_SIGN_DIGEST:
		return ta_entry_asymmetric_sign_digest(nParamTypes, pParams);

	case TA_CRYPT_CMD_ASYMMETRIC_VERIFY_DIGEST:
		return ta_entry_asymmetric_verify_digest(nParamTypes, pParams);

	case TA_CRYPT_CMD_DERIVE_KEY:
		return ta_entry_derive_key(nParamTypes, pParams);

	case TA_CRYPT_CMD_RANDOM_NUMBER_GENERATE:
		return ta_entry_random_number_generate(nParamTypes, pParams);

	case TA_CRYPT_CMD_AE_INIT:
		return ta_entry_ae_init(nParamTypes, pParams);

	case TA_CRYPT_CMD_AE_UPDATE_AAD:
		return ta_entry_ae_update_aad(nParamTypes, pParams);

	case TA_CRYPT_CMD_AE_UPDATE:
		return ta_entry_ae_update(nParamTypes, pParams);

	case TA_CRYPT_CMD_AE_ENCRYPT_FINAL:
		return ta_entry_ae_encrypt_final(nParamTypes, pParams);

	case TA_CRYPT_CMD_AE_DECRYPT_FINAL:
		return ta_entry_ae_decrypt_final(nParamTypes, pParams);

	case TA_CRYPT_CMD_GET_OBJECT_BUFFER_ATTRIBUTE:
		return ta_entry_get_object_buffer_attribute(nParamTypes,
							    pParams);
	case TA_CRYPT_CMD_GET_OBJECT_VALUE_ATTRIBUTE:
		return ta_entry_get_object_value_attribute(nParamTypes,
							   pParams);
	case TA_CRYPT_CMD_SETGLOBAL:
		return set_global(nParamTypes, pParams);

	case TA_CRYPT_CMD_GETGLOBAL:
		return get_global(nParamTypes, pParams);

#ifdef CFG_TA_MBEDTLS
	case TA_CRYPT_CMD_MBEDTLS_SELF_TESTS:
		return ta_entry_mbedtls_self_tests(nParamTypes, pParams);
	case TA_CRYPT_CMD_MBEDTLS_CHECK_CERT:
		return ta_entry_mbedtls_check_cert(nParamTypes, pParams);
	case TA_CRYPT_CMD_MBEDTLS_SIGN_CERT:
		return ta_entry_mbedtls_sign_cert(nParamTypes, pParams);
#endif
#ifdef CFG_SYSTEM_PTA
	case TA_CRYPT_CMD_SEED_RNG_POOL:
		return seed_rng_pool(nParamTypes, pParams);
	case TA_CRYPT_CMD_DERIVE_TA_UNIQUE_KEY:
		return derive_ta_unique_key_test(nParamTypes, pParams);
	case TA_CRYPT_CMD_DERIVE_TA_UNIQUE_KEY_SHM:
		return derive_ta_unique_key_test_shm(nParamTypes, pParams);
#endif
	case TA_CRYPT_CMD_ARITH_NEW_VAR:
		return ta_entry_arith_new_var(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_NEW_FMM_CTX:
		return ta_entry_arith_new_fmm_ctx(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_NEW_FMM_VAR:
		return ta_entry_arith_new_fmm_var(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_FREE_HANDLE:
		return ta_entry_arith_free_handle(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_FROM_OCTET_STRING:
		return ta_entry_arith_from_octet_string(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_FROM_S32:
		return ta_entry_arith_from_s32(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_GET_VALUE:
		return ta_entry_arith_get_value(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_GET_VALUE_S32:
		return ta_entry_arith_get_value_s32(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_GET_BIT:
		return ta_entry_arith_get_bit(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_GET_BIT_COUNT:
		return ta_entry_arith_get_bit_count(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_SHIFT_RIGHT:
		return ta_entry_arith_shift_right(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_CMP:
		return ta_entry_arith_cmp(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_CMP_S32:
		return ta_entry_arith_cmp_s32(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_ADD:
		return ta_entry_arith_add(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_SUB:
		return ta_entry_arith_sub(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_MUL:
		return ta_entry_arith_mul(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_NEG:
		return ta_entry_arith_neg(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_SQR:
		return ta_entry_arith_sqr(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_DIV:
		return ta_entry_arith_div(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_MOD:
		return ta_entry_arith_mod(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_ADDMOD:
		return ta_entry_arith_addmod(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_SUBMOD:
		return ta_entry_arith_submod(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_MULMOD:
		return ta_entry_arith_mulmod(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_SQRMOD:
		return ta_entry_arith_sqrmod(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_INVMOD:
		return ta_entry_arith_invmod(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_IS_RELATIVE_PRIME:
		return ta_entry_arith_is_rel_prime(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_COMPUTE_EGCD:
		return ta_entry_arith_compute_egcd(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_IS_PRIME:
		return ta_entry_arith_is_prime(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_TO_FMM:
		return ta_entry_arith_to_fmm(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_FROM_FMM:
		return ta_entry_arith_from_fmm(nParamTypes, pParams);
	case TA_CRYPT_CMD_ARITH_COMPUTE_FMM:
		return ta_entry_arith_compute_fmm(nParamTypes, pParams);
	case TA_CRYPT_CMD_IS_ALGO_SUPPORTED:
		return ta_entry_is_algo_supported(nParamTypes, pParams);

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

static TEE_Result set_global(uint32_t param_types, TEE_Param params[4])
{
	int i = 0;

	/* Param 0 is a memref, input/output */
	if (TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(param_types, 0))
		return TEE_ERROR_BAD_PARAMETERS;

	/* Other parameters must be of type TEE_PARAM_TYPE_NONE */
	for (i = 1; i < 4; i++) {
		if (TEE_PARAM_TYPE_NONE != TEE_PARAM_TYPE_GET(param_types, i))
			return TEE_ERROR_BAD_PARAMETERS;
	}

	_globalvalue = params[0].value.a;
	return TEE_SUCCESS;
}

static TEE_Result get_global(uint32_t param_types, TEE_Param params[4])
{
	int i = 0;

	/* Param 0 is a memref, input/output */
	if (TEE_PARAM_TYPE_VALUE_OUTPUT != TEE_PARAM_TYPE_GET(param_types, 0))
		return TEE_ERROR_BAD_PARAMETERS;

	/* Other parameters must be of type TEE_PARAM_TYPE_NONE */
	for (i = 1; i < 4; i++) {
		if (TEE_PARAM_TYPE_NONE != TEE_PARAM_TYPE_GET(param_types, i))
			return TEE_ERROR_BAD_PARAMETERS;
	}

	params[0].value.a = _globalvalue;
	return TEE_SUCCESS;
}
