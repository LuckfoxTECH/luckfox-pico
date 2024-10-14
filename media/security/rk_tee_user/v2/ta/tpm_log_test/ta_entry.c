// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2020, ARM Limited. All rights reserved.
 */

#include <inttypes.h>
#include <pta_system.h>
#include <string.h>
#include <ta_tpm_log.h>
#include <tee_internal_api.h>

#define LOG_SIZE		1024

static TEE_Result invoke_system_pta(uint32_t cmd_id, uint32_t param_types,
				    TEE_Param params[TEE_NUM_PARAMS])
{
	const TEE_UUID system_uuid = PTA_SYSTEM_UUID;
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t ret_origin = 0;

	res = TEE_OpenTASession(&system_uuid, TEE_TIMEOUT_INFINITE,
				0, NULL, &session, &ret_origin);
	if (res != TEE_SUCCESS)
		return res;

	res = TEE_InvokeTACommand(session, TEE_TIMEOUT_INFINITE,
				  cmd_id, param_types, params, &ret_origin);

	TEE_CloseTASession(session);

	return res;
}

/*
 * Test case for the use of right parameters.
 */
static TEE_Result test_with_right_parameters(void)
{
	unsigned char buf[LOG_SIZE] = { 0 };
	const uint32_t param_types = TEE_PARAM_TYPES(
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
	TEE_Param params[TEE_NUM_PARAMS] = { 0 };
	uint32_t field = 0;
	unsigned char *digestptr = NULL;
	unsigned int i = 0;

	params[0].memref.buffer = (void *)buf;
	params[0].memref.size = LOG_SIZE;

	if (invoke_system_pta(PTA_SYSTEM_GET_TPM_EVENT_LOG,
			      param_types, params) == TEE_SUCCESS) {
		DMSG("Received %i bytes of event log", params[0].memref.size);
		DMSG("Parsing the event log header:");

		memcpy(&field, &buf[0], sizeof(uint32_t));
		DMSG("\tPCRIndex = 0x%" PRIx32, field);

		/*
		 * PCR Index must be 0 on the header.
		 * Ref. Section 5.3 of TCG EFI Protocol Specification.
		 * Family 2.0 Level 00 Revision 00.13. March 30, 2016
		 */
		if (field != 0) {
			EMSG("PCRIndex must be 0");
			return TEE_ERROR_GENERIC;
		}

		memcpy(&field, &buf[4], sizeof(uint32_t));
		DMSG("\tEventType = 0x%" PRIx32, field);

		/*
		 * Event type must be EV_NO_ACTION on the header.
		 * Ref. Section 5.3 of TCG EFI Protocol Specification.
		 * Family 2.0 Level 00 Revision 00.13. March 30, 2016
		 */
		if (field != 3) {
			EMSG("EventType must be 3 (EV_NO_ACTION)");
			return TEE_ERROR_GENERIC;
		}

		/*
		 * Header digest must be formed of 20 bytes set to 0.
		 * Ref. Section 5.3 of TCG EFI Protocol Specification.
		 * Family 2.0 Level 00 Revision 00.12. March 30, 2016
		 */
		digestptr = &buf[8];
		for (i = 0; i < 20; i++) {
			if (*(digestptr + i) != 0) {
				EMSG("Digest byte %i must be 0", i);
				return TEE_ERROR_GENERIC;
			}
		}

		DMSG("--> Digest value passed");
		return TEE_SUCCESS;
	}
	return TEE_ERROR_ACCESS_DENIED;
}

/*
 * Test case for short buffer.
 */
static TEE_Result test_short_buffer(void)
{
	unsigned char buf[LOG_SIZE] = { 0 };
	const uint32_t param_types = TEE_PARAM_TYPES(
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
	TEE_Param params[TEE_NUM_PARAMS] = { 0 };
	unsigned int i = 0;

	params[0].memref.buffer = (void *)buf;
	params[0].memref.size = 1;

	if (invoke_system_pta(PTA_SYSTEM_GET_TPM_EVENT_LOG, param_types,
			      params) != TEE_ERROR_SHORT_BUFFER) {
		return TEE_ERROR_GENERIC;
	}

	for (i = 0; i < LOG_SIZE; i++) {
		if (buf[i] != 0) {
			EMSG("Buf is expected to be all zeroed");
			return TEE_ERROR_GENERIC;
		}
	}

	return TEE_SUCCESS;
}

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
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes __unused,
				    TEE_Param pParams[4] __unused,
				    void **ppSessionContext __unused)
{
	return TEE_SUCCESS;
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext __unused)
{
}

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext __unused,
				      uint32_t nCommandID,
				      uint32_t nParamTypes __unused,
				      TEE_Param pParams[4] __unused)
{
	switch (nCommandID) {
	case TA_TPM_TEST_GET_LOG:
		return test_with_right_parameters();

	case TA_TPM_TEST_SHORT_BUF:
		return test_short_buffer();

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
