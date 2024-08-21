// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <rktest_ta.h>
#include <rktest.h>


const TEEC_UUID UUID = RKTEST_TA_UUID;

static TEEC_Result invoke_transfer_data(TEEC_Context *context, TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_SharedMemory sm;
	uint8_t temref_input[50];
	const uint8_t transfer_inout[] = "Transfer data test.";

	memcpy(temref_input, transfer_inout, sizeof(transfer_inout));
	//Initialize the Shared Memory buffers
	sm.size = sizeof(temref_input);
	sm.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	res = TEEC_AllocateSharedMemory(context, &sm);
	if (res != TEEC_SUCCESS) {
		printf("AllocateSharedMemory ERR! res= 0x%x\n", res);
		return res;
	}

	memset(operation, 0, sizeof(TEEC_Operation));
	//Note: these parameters must correspond to operation.params[],
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE);
	operation->params[0].value.a = 66;
	//This buffer will be temporarily shared.
	operation->params[1].tmpref.size = sizeof(temref_input);
	operation->params[1].tmpref.buffer = (void *)temref_input;
	operation->params[2].memref.parent = &sm;
	operation->params[2].memref.offset = 0;
	operation->params[2].memref.size = sm.size;

	//Invoke to TA
	res = TEEC_InvokeCommand(session, RKTEST_TA_CMD_TRANSFER_DATA,
				operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		goto out;
	}

	//Check the result
	if (operation->params[0].value.a == 66+1 &&
		operation->params[0].value.b == operation->params[0].value.a)
		printf("test value : Pass!\n");
	else
		printf("test value : Fail! (mismatch values)\n");

	//Check if the sm->buffer (params[2]) = TRANSFER_INOUT (params[1])
	if (memcmp(sm.buffer, transfer_inout, sizeof(transfer_inout)) == 0)
		printf("test buffer : Pass!\n");
	else
		printf("test buffer : Fail! (mismatch buffer)\n");

out:
	TEEC_ReleaseSharedMemory(&sm);
	return res;
};

static TEEC_Result invoke_storage(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_STORAGE,
				operation, error_origin);
};

static TEEC_Result test_storage_speed(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin,
			uint32_t storage_type, uint32_t *storage_size, uint32_t times)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint8_t *temref_input = NULL;

	for (uint32_t i = 0; i < times; i++) {
		temref_input = malloc(storage_size[i]);
		memset(temref_input, 0xab, storage_size[i]);
		memset(operation, 0, sizeof(TEEC_Operation));

		//Note: these parameters must correspond to operation.params[],
		operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_VALUE_INPUT,
			TEEC_NONE, TEEC_NONE);
		operation->params[0].tmpref.size = storage_size[i];
		operation->params[0].tmpref.buffer = (void *)temref_input;
		operation->params[1].value.a = storage_type;

		//Invoke to TA
		res = TEEC_InvokeCommand(session, RKTEST_TA_CMD_STORAGE_SPEED,
					operation, error_origin);

		if (temref_input)
			free(temref_input);

		if (res != TEEC_SUCCESS) {
			printf("InvokeCommand ERR! res= 0x%x\n", res);
			break;
		}
	}

	return res;
};

static TEEC_Result invoke_storage_speed(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t rpmb_storage_size[] = {32, 1024, 4096, 30000};
	uint32_t ree_storage_size[] = {32, 1024, 4096, 30000, 60000};

	res = test_storage_speed(session, operation, error_origin, TEE_STORAGE_PRIVATE_RPMB,
		rpmb_storage_size, sizeof(rpmb_storage_size) / sizeof(rpmb_storage_size[0]));
	if (res != TEEC_SUCCESS)
		return res;

	res = test_storage_speed(session, operation, error_origin, TEE_STORAGE_PRIVATE_REE,
		ree_storage_size, sizeof(ree_storage_size) / sizeof(ree_storage_size[0]));
	return res;
}

static TEEC_Result invoke_property(TEEC_Session *session,
		TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_PROPERTY,
				operation, error_origin);
};

static TEEC_Result invoke_crypto_sha(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_SHA,
				operation, error_origin);
};

static TEEC_Result invoke_crypto_aes(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_AES,
				operation, error_origin);
};

static TEEC_Result invoke_crypto_rsa(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_RSA,
				operation, error_origin);
};

static TEEC_Result invoke_otp_read(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_OEM_OTP_READ,
				operation, error_origin);
};

static TEEC_Result invoke_otp_write(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_OEM_OTP_WRITE,
				operation, error_origin);
};

static TEEC_Result invoke_otp_size(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_OEM_OTP_SIZE,
				  operation, error_origin);
};

static TEEC_Result invoke_trng_read(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_TRNG_READ,
				  operation, error_origin);
}

TEEC_Result rk_test(uint32_t invoke_command)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t error_origin = 0;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;

	//[1] Connect to TEE
	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code 0x%x\n", res);
		return res;
	}

	//[2] Open session with TEE application
	res = TEEC_OpenSession(&contex, &session, &UUID,
		TEEC_LOGIN_PUBLIC, NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code 0x%x origin 0x%x\n",
			res, error_origin);
		goto out;
	}

	//[3] Perform operation initialization
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
		TEEC_NONE, TEEC_NONE);

	//[4] Start invoke command to the TEE application.
	switch (invoke_command) {
	case  TRANSFER_DATA:
		res = invoke_transfer_data(&contex, &session, &operation, &error_origin);
		break;
	case  STORAGE:
		res = invoke_storage(&session, &operation, &error_origin);
		break;
	case  STORAGE_SPEED:
		res = invoke_storage_speed(&session, &operation, &error_origin);
		break;
	case  PROPERTY:
		res = invoke_property(&session, &operation, &error_origin);
		break;
	case  CRYPTO_SHA:
		res = invoke_crypto_sha(&session, &operation, &error_origin);
		break;
	case  CRYPTO_AES:
		res = invoke_crypto_aes(&session, &operation, &error_origin);
		break;
	case  CRYPTO_RSA:
		res = invoke_crypto_rsa(&session, &operation, &error_origin);
		break;
	case  OTP_READ:
		res = invoke_otp_read(&session, &operation, &error_origin);
		break;
	case  OTP_WRITE:
		res = invoke_otp_write(&session, &operation, &error_origin);
		break;
	case  OTP_SIZE:
		res = invoke_otp_size(&session, &operation, &error_origin);
		break;
	case  TRNG_READ:
		res = invoke_trng_read(&session, &operation, &error_origin);
		break;
	default:
		printf("Doing nothing.\n");
		break;
	}
	if (res != TEEC_SUCCESS) {
		printf("Test ERR. res 0x%x origin 0x%x\n", res, error_origin);
		goto out1;
	}
	printf("Test OK.\n");
	//[5] Tidyup resources
out1:
	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return res;
}

