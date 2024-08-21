/* SPDX-License-Identifier: BSD-2-Clause */
/* Copyright (c) 2018, Linaro Limited */

#ifndef __ARITH_TAF_H
#define __ARITH_TAF_H

#include <tee_internal_api.h>

TEE_Result ta_entry_arith_new_var(uint32_t param_type,
				  TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_new_fmm_ctx(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_new_fmm_var(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_free_handle(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_from_octet_string(uint32_t param_type,
					    TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_from_s32(uint32_t param_type,
				   TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_get_value(uint32_t param_type,
				    TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_get_value_s32(uint32_t param_type,
					TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_get_bit(uint32_t param_type,
				  TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_get_bit_count(uint32_t param_type,
					TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_shift_right(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_cmp(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_cmp_s32(uint32_t param_type,
				  TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_add(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_sub(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_mul(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_neg(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_sqr(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_div(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_mod(uint32_t param_type,
			      TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_addmod(uint32_t param_type,
				 TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_submod(uint32_t param_type,
				 TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_mulmod(uint32_t param_type,
				 TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_sqrmod(uint32_t param_type,
				 TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_invmod(uint32_t param_type,
				 TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_is_rel_prime(uint32_t param_type,
				       TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_compute_egcd(uint32_t param_type,
				       TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_is_prime(uint32_t param_type,
				   TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_to_fmm(uint32_t param_type,
				 TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_from_fmm(uint32_t param_type,
				   TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_arith_compute_fmm(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS]);

#endif /*__ARITH_TAF_H*/
