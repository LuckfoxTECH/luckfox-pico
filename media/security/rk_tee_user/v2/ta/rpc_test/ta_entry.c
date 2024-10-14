// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include <tee_ta_api.h>
#include <user_ta_header_defines.h>
#include <ta_rpc.h>

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
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes, TEE_Param pParams[4],
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

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
	(void)pSessionContext;

	switch (nCommandID) {
	case TA_RPC_CMD_CRYPT_SHA224:
		return rpc_sha224(false, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_SHA256:
		return rpc_sha256(false, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_AES256ECB_ENC:
		return rpc_aes256ecb_encrypt(false, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_AES256ECB_DEC:
		return rpc_aes256ecb_decrypt(false, nParamTypes, pParams);

	case TA_RPC_CMD_OPEN:
		return rpc_open(pSessionContext, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_PRIVMEM_SHA224:
		return rpc_sha224(true, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_PRIVMEM_SHA256:
		return rpc_sha256(true, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_PRIVMEM_AES256ECB_ENC:
		return rpc_aes256ecb_encrypt(true, nParamTypes, pParams);

	case TA_RPC_CMD_CRYPT_PRIVMEM_AES256ECB_DEC:
		return rpc_aes256ecb_decrypt(true, nParamTypes, pParams);

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
