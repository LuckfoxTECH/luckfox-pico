/* SPDX-License-Identifier: BSD-2-Clause */
/* Copyright (c) 2018, Linaro Limited */

#ifndef __MBEDTLS_TAF_H
#define __MBEDTLS_TAF_H

#include <tee_internal_api.h>

TEE_Result ta_entry_mbedtls_self_tests(uint32_t param_type,
				       TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_mbedtls_check_cert(uint32_t param_type,
				       TEE_Param params[TEE_NUM_PARAMS]);
TEE_Result ta_entry_mbedtls_sign_cert(uint32_t param_type,
				      TEE_Param params[TEE_NUM_PARAMS]);

#endif /*__MBEDTLS_TAF_H*/
