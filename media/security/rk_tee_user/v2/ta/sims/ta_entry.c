// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include <tee_ta_api.h>
#include <user_ta_header_defines.h>
#include <ta_sims.h>

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
	return sims_open_session(ppSessionContext);
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext)
{
	sims_close_session(pSessionContext);
}

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
	switch (nCommandID) {
	case TA_SIMS_OPEN_TA_SESSION:
		return sims_open_ta_session(pSessionContext,
					    nParamTypes, pParams);

	case TA_SIMS_CMD_READ:
		return sims_read(nParamTypes, pParams);

	case TA_SIMS_CMD_WRITE:
		return sims_write(nParamTypes, pParams);

	case TA_SIMS_CMD_GET_COUNTER:
		return sims_get_counter(pSessionContext, nParamTypes, pParams);

	case TA_SIMS_CMD_PANIC:
		return sims_entry_panic(pSessionContext, nParamTypes, pParams);

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
