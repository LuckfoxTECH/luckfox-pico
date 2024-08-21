/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef TA_RPC_H
#define TA_RPC_H

#include <ta_rpc_test.h>
#include <tee_api.h>

TEE_Result rpc_sha224(bool sec_mem, uint32_t nParamTypes, TEE_Param pParams[4]);
TEE_Result rpc_sha256(bool sec_mem, uint32_t nParamTypes, TEE_Param pParams[4]);
TEE_Result rpc_aes256ecb_encrypt(bool sec_mem, uint32_t nParamTypes,
				 TEE_Param pParams[4]);
TEE_Result rpc_aes256ecb_decrypt(bool sec_mem, uint32_t nParamTypes,
				 TEE_Param pParams[4]);
TEE_Result rpc_open(void *session_context, uint32_t param_types,
		    TEE_Param params[4]);

#endif
