/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * Copyright (c) 2020, Linaro Limited
 */

#ifndef XML_INTERNAL_API_H_
#define XML_INTERNAL_API_H_

#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tee_api_defines.h>
#include <tee_client_api.h>
#include <unistd.h>

#include "xml_common_api.h"
#include "xtest_helpers.h"
#include "xtest_test.h"

#define Invoke_MaskUnmaskCancellations Invoke_Simple_Function
#define Invoke_ProcessTAInvoke_Payload_Value Invoke_Simple_Function
#define Invoke_ProcessTAInvoke_Payload_Value_In_Out Invoke_Simple_Function
#define Invoke_ProcessTAInvoke_Payload_Memref Invoke_Simple_Function

#define Invoke_GetPropertyAsBool_withoutEnum Invoke_GetPropertyAsXXX_withoutEnum
#define Invoke_GetPropertyAsU32_withoutEnum Invoke_GetPropertyAsXXX_withoutEnum
#define Invoke_GetPropertyAsUUID_withoutEnum Invoke_GetPropertyAsXXX_withoutEnum
#define Invoke_GetPropertyAsIdentity_withoutEnum \
	Invoke_GetPropertyAsXXX_withoutEnum
#define Invoke_GetPropertyAsBinaryBlock_withoutEnum \
	Invoke_GetPropertyAsXXX_withoutEnum
#define Invoke_GetPropertyAsString_withoutEnum \
	Invoke_GetPropertyAsXXX_withoutEnum

#define Invoke_GetPropertyAsXXXX_fromEnum Invoke_StartPropertyEnumerator

#define Invoke_FreePropertyEnumerator Invoke_ResetPropertyEnumerator
#define Invoke_GetNextProperty_enumNotStarted Invoke_ResetPropertyEnumerator

#define Invoke_ProcessTAInvoke_DeadErrorSuccess \
	Invoke_ProcessInvokeTAOpenSession

#define CLIENT_APP01                    NULL

#define TEEC_UNDEFINED_ERROR 0xDEADDEAD

#define TEEC_ORIGIN_ANY_NOT_TRUSTED_APP  0x00000005
/* same as TEE_ORIGIN_NOT_TRUSTED_APP */

#define SIZE_ZERO 0

#define TEE_ERROR_TOO_SHORT_BUFFER TEE_ERROR_SHORT_BUFFER

#define CMD_TEE_GetPropertyAsString_withoutEnum             0x00000010
#define CMD_TEE_GetPropertyAsBool_withoutEnum               0x00000015
#define CMD_TEE_GetPropertyAsInt_withoutEnum                0x00000020
#define CMD_TEE_GetPropertyAsBinaryBlock_withoutEnum        0x00000025
#define CMD_TEE_GetPropertyAsUUID_withoutEnum               0x00000030
#define CMD_TEE_GetPropertyAsIdentity_withoutEnum           0x00000035
#define CMD_TEE_GetPropertyAsXXXX_fromEnum                  0x00000045
#define CMD_TEE_AllocatePropertyEnumerator                  0x00000060
#define CMD_TEE_StartPropertyEnumerator                     0x00000065
#define CMD_TEE_ResetPropertyEnumerator                     0x00000070
#define CMD_TEE_FreePropertyEnumerator                      0x00000075
#define CMD_TEE_GetPropertyName                             0x00000080
#define CMD_TEE_Malloc                                      0x00000100
#define CMD_TEE_Realloc                                     0x00000110
#define CMD_TEE_MemMove                                     0x00000120
#define CMD_TEE_MemCompare                                  0x00000130
#define CMD_TEE_MemFill                                     0x00000140
#define CMD_TEE_Panic                                       0x00000104
#define CMD_TEE_CheckMemoryAccessRight                      0x00000103
#define CMD_TEE_GetCancellationFlag_RequestedCancel         0x00000105
#define CMD_TEE_MaskUnmaskCancellations                     0x00000106
#define CMD_TEE_Free                                        0x00000107
#define CMD_ProcessInvokeTAOpenSession                      0x00000200
#define CMD_ProcessTAInvokeTA_simple                        0x00000201
#define CMD_ProcessTAInvokeTA_PayloadValue                  0x00000202
#define CMD_TEE_GetNextPropertyEnumerator_notStarted        0x00000203
#define CMD_ProcessTAInvokeTA_PayloadMemref                 0x00000204
#define CMD_ProcessTAInvokeTA_PayloadValue_In_Out           0x00000205
#define CMD_TEE_OpenTASession                               0x00000300
#define CMD_TEE_InvokeTACommand                             0x00000301
#define CMD_TEE_CloseTASession                              0x00000302

#define CMD_SUCCESS					    1
#define CMD_SET_PANIC_ON_DESTROY			    1

static TEEC_SharedMemory share_mem[2];
static TEEC_SharedMemory *SHARE_MEM01 = share_mem;
static TEEC_SharedMemory *SHARE_MEM02 = share_mem + 1;
static TEEC_Session session[2];
static TEEC_Session *SESSION01 = session;
static TEEC_Session *SESSION02 = session + 1;
static TEEC_Context context[2];
static TEEC_Context *CONTEXT01 = context;
static TEEC_Context *CONTEXT02 = context + 1;

#define TA_SESSION01 0
#define TA_SESSION02 1
#define TA_SESSION_NULL 0xffffffff

static uint32_t BIG_SIZE = 1024;

static char *NO_DATA;
static uint32_t enumerator1;
static uint32_t *ENUMERATOR1 = &enumerator1;

#define ANY_OWNER_NOT_SET		0
#define ANY_OWNER_SET			TEE_MEMORY_ACCESS_ANY_OWNER
#define ANY_OWNER_SET_ACCESS_READ	(TEE_MEMORY_ACCESS_ANY_OWNER | \
					 TEE_MEMORY_ACCESS_READ)
#define ANY_OWNER_SET_ACCESS_WRITE	(TEE_MEMORY_ACCESS_ANY_OWNER | \
					 TEE_MEMORY_ACCESS_WRITE)
#define ANY_OWNER_SET_ACCESS_READ_WRITE	(TEE_MEMORY_ACCESS_ANY_OWNER | \
					 TEE_MEMORY_ACCESS_READ | \
					 TEE_MEMORY_ACCESS_WRITE)

#define SMALL_SIZE 0xA

#define CMD_TEE_GetInstanceData 0x00000101
#define CMD_TEE_SetInstanceData 0x00000102
#define CMD_TEE_GetPropertyAsU32_withoutEnum 0x00000020

#define NORMAL_SIZE_BUFFER 1
#define TOO_SHORT_BUFFER 0
#define CASE_NOT_NULL 1
#define CASE_NULL 0
#define CASE_BUFFER1_DIFFERS_FIRST 1
#define CASE_BUFFER2_DIFFERS_FIRST 2
#define CASE_EQUAL 0
#define CASE_ERROR_ICA2 3
#define CASE_PAYLOAD_VALUE 4
#define CASE_SUCCESS_ICA2 2
#define CASE_TARGET_DEAD_ICA2 1
#define CASE_CANCEL_TIMEOUT 2
#define CASE_ITEM_NOT_FOUND 3
#define CASE_SUCCESS 0
#define CASE_TARGET_BUSY 4
#define CASE_TARGET_DEAD 1
#define RESULT_EQUAL 0
#define RESULT_INTEGER_GREATER_THAN_ZERO 1
#define RESULT_INTEGER_LOWER_THAN_ZERO 2

#define HINT_ZERO 0
#define SIZE_OVER_MEMORY 0xFFFFFFFE

#define TEE_ORIGIN_NOT_TRUSTED_APP 5
/* same as TEEC_ORIGIN_ANY_NOT_TRUSTED_APP */
#define TEE_PROPSET_IMPLEMENTATION TEE_PROPSET_TEE_IMPLEMENTATION

static char VALUE_PREDEFINED_STRING[] = "this is a test string";
static char VALUE_PREDEFINED_U32[] = "48059";
static char VALUE_PREDEFINED_UUID[] = "534D4152-542D-4353-4C54-2D54412D3031";
static char VALUE_PREDEFINED_IDENTITY[] =
	"F0000000:534D4152-542D-4353-4C54-2D54412D3031";

static char *VALUE_NONE;
static char VALUE_PREDEFINED_BINARY_BLOCK[] =
	"VGhpcyBpcyBhIHRleHQgYmluYXJ5IGJsb2Nr";
static char VALUE_PREDEFINED_BOOLEAN[] = "true";

static uint8_t CHAR1[] = { 0x10 };

static char GPD_CLIENT_identity[] = "gpd.client.identity\0";
static char GPD_TA_appID[] = "gpd.ta.appID\0";
static char GPD_TA_dataSize[] = "gpd.ta.dataSize\0";
static char GPD_TA_instanceKeepAlive[] = "gpd.ta.instanceKeepAlive\0";
static char GPD_TA_multiSession[] = "gpd.ta.multiSession\0";
static char GPD_TA_singleInstance[] = "gpd.ta.singleInstance\0";
static char GPD_TA_stackSize[] = "gpd.ta.stackSize\0";
static char GPD_TEE_ARITH_maxBigIntSize[] = "gpd.tee.arith.maxBigIntSize\0";
static char GPD_TEE_SYSTEM_TIME_protectionLevel[] =
	"gpd.tee.systemTime.protectionLevel\0";
static char GPD_TEE_TA_PERSISTENT_TIME_protectionLevel[] =
	"gpd.tee.TAPersistentTime.protectionLevel\0";
static char GPD_TEE_apiversion[] = "gpd.tee.apiversion\0";
static char GPD_TEE_description[] = "gpd.tee.description\0";
static char GPD_TEE_deviceID[] = "gpd.tee.deviceID\0";
static char PROPERTY_NAME_NOT_VALID_ENCODING[] = "gpd.\t\n\r\0";
static char PROPERTY_NAME_UNKNOWN[] = "unknown\0";
static char SMC_TA_testuuid[] = "smc.ta.testuuid\0";
static char SMC_TA_testbinaryblock[] = "smc.ta.testbinaryblock\0";
static char SMC_TA_testbooltrue[] = "smc.ta.testbooltrue\0";
static char SMC_TA_testidentity[] = "smc.ta.testidentity\0";
static char SMC_TA_teststring[] = "smc.ta.teststring\0";
static char SMC_TA_testu32[] = "smc.ta.testu32\0";
static char STRING_SAMPLE_SIZE_4_CHAR[] = "TEE\0";


/** ALL_TEEC_UUID
 *
 * These constants are the UUID of existing
 * Trusted Applications
 */
static TEEC_UUID UUID_TTA_testingInternalAPI_TrustedCoreFramework = {
	0x534D4152, 0x542D, 0x4353,
	{ 0x4C, 0x54, 0x2D, 0x54, 0x41, 0x2D, 0x30, 0x31 }
};
static TEEC_UUID UUID_TTA_testingInternalAPI_TrustedCoreFramework_ICA = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x41, 0x54, 0x43, 0x46, 0x49, 0x43, 0x41 }
};
static TEEC_UUID UUID_TTA_testingInternalAPI_TrustedCoreFramework_ICA2 = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x54, 0x43, 0x46, 0x49, 0x43, 0x41, 0x32 }
};
static TEEC_UUID
	UUID_TTA_testingInternalAPI_TrustedCoreFramework_MultipleInstanceTA = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x4D, 0x4C, 0x54, 0x49, 0x4E, 0x53, 0x54, 0x43 }
};
static TEEC_UUID
	UUID_TTA_testingInternalAPI_TrustedCoreFramework_SingleInstanceTA = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x53, 0x47, 0x4C, 0x49, 0x4E, 0x53, 0x54, 0x43 }
};
static TEEC_UUID UUID_Unknown = {
	0x534D4152, 0x542D, 0x4355,
	{ 0x4E, 0x4B, 0x2D, 0x4E, 0x4F, 0x2D, 0x57, 0x4E }
};

static TEEC_UUID
UUID_TTA_testingInternalAPI_TrustedCoreFramework_PanicAtCloseSession = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x43, 0x4C, 0x4F, 0x50, 0x41, 0x4E, 0x49, 0x43 }
};

static TEEC_UUID
UUID_TTA_testingInternalAPI_TrustedCoreFramework_PanicAtCreation = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x43, 0x52, 0x50, 0x41, 0x4E, 0x49, 0x43 }
};

static void __maybe_unused *cancellation_thread(void *arg)
{
	TEEC_RequestCancellation((TEEC_Operation *)arg);
	return NULL;
}

#define RequestCancellation(op) \
	(void)ADBG_EXPECT(c, 0, \
			  pthread_create(&THREAD02, NULL, cancellation_thread, \
					 (void *)op)); \
	(void)ADBG_EXPECT(c, 0, pthread_join(THREAD02, NULL));

static TEEC_Result Invoke_MemFill(ADBG_Case_t *c, TEEC_Session *sess,
				  uint32_t cmdId, uint32_t memoryFillSize,
				  uint8_t *charFill)
{
	return Invoke_Simple_Function_v2(c, sess, cmdId, memoryFillSize, 0,
					 *charFill, 0);
}

static TEEC_Result Invoke_GetPropertyAsXXX_withoutEnum(
	ADBG_Case_t *c, TEEC_Session *sess, uint32_t cmdId,
	TEE_PropSetHandle propSet, char *name, uint32_t kindBuffer,
	char *expectedValue)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;
	uint32_t nameLen = 0;
	uint32_t expectedValueLen = 0;

	nameLen = strlen(name) + 1;
	ALLOCATE_AND_FILL_SHARED_MEMORY(sess->ctx, SHARE_MEM01, BIG_SIZE,
					TEEC_MEM_INPUT, nameLen, name, mem01_exit)

	if (kindBuffer == TOO_SHORT_BUFFER) {
		ALLOCATE_SHARED_MEMORY(sess->ctx, SHARE_MEM02, 1,
				       TEEC_MEM_OUTPUT, mem02_exit)
	} else {
		ALLOCATE_SHARED_MEMORY(sess->ctx, SHARE_MEM02, BIG_SIZE,
				       TEEC_MEM_OUTPUT, mem02_exit)
	}

	op.params[0].value.a = (unsigned long)propSet;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01, nameLen)
	SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, SHARE_MEM02,
					      SHARE_MEM02->size)

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT, TEEC_MEMREF_PARTIAL_INPUT,
		TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (res)
		goto exit;

	if (expectedValue != VALUE_NONE) {
		expectedValueLen = strlen(expectedValue);
		if (cmdId != CMD_TEE_GetPropertyAsBool_withoutEnum &&
		    cmdId != CMD_TEE_GetPropertyAsIdentity_withoutEnum &&
		    cmdId != CMD_TEE_GetPropertyAsUUID_withoutEnum &&
		    cmdId != CMD_TEE_GetPropertyAsU32_withoutEnum)
			expectedValueLen++;
		if (!ADBG_EXPECT_BUFFER(c, expectedValue, expectedValueLen,
					op.params[2].memref.parent->buffer,
					op.params[2].memref.size))
			res = TEEC_ERROR_GENERIC;
	}

exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_MemCompare(
	ADBG_Case_t *c, TEEC_Session *sess, uint32_t cmdId,
	uint32_t memorySize, uint32_t Case, uint32_t compareResult)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;
	uint32_t outValue = 0;

	op.params[0].value.a = memorySize;
	op.params[1].value.a = Case;
	op.params[2].value.a = outValue;

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
		TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (res != TEEC_SUCCESS)
		goto exit;

	if (compareResult == RESULT_EQUAL) {
		(void)ADBG_EXPECT_COMPARE_SIGNED(c, op.params[2].value.a, ==,
						 0);
	} else if (compareResult == RESULT_INTEGER_GREATER_THAN_ZERO) {
		(void)ADBG_EXPECT_COMPARE_SIGNED(c,
						 (int32_t)op.params[2].value.a,
						 >, 0);
	} else if (compareResult == RESULT_INTEGER_LOWER_THAN_ZERO) {
		(void)ADBG_EXPECT_COMPARE_SIGNED(c,
						 (int32_t)op.params[2].value.a,
						 <, 0);
	}

exit:
	return res;
}

static TEEC_Result Invoke_SetInstanceData(ADBG_Case_t *c,
					  TEEC_Session *sess, uint32_t cmdId,
					  char *data)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	size_t data_size = strlen(data) + 1;
	uint32_t org;

	ALLOCATE_AND_FILL_SHARED_MEMORY(sess->ctx, SHARE_MEM01, BIG_SIZE,
					TEEC_MEM_INPUT,
					data_size, data, mem01_exit)

	SET_SHARED_MEMORY_OPERATION_PARAMETER(0, 0, SHARE_MEM01, data_size)

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_GetInstanceData(
	ADBG_Case_t *c, TEEC_Session *sess, uint32_t cmdId, char *expectedData,
	uint32_t expectedDataSize)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	ALLOCATE_SHARED_MEMORY(sess->ctx, SHARE_MEM01, BIG_SIZE,
			       TEEC_MEM_OUTPUT, mem01_exit)

	SET_SHARED_MEMORY_OPERATION_PARAMETER(0, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (res != TEEC_SUCCESS)
		goto exit;

	if (res != TEE_ERROR_GENERIC) {
		if (!ADBG_EXPECT_BUFFER(c, expectedData, expectedDataSize,
					SHARE_MEM01->buffer,
					op.params[0].memref.size))
			res = TEE_ERROR_GENERIC;
	}

exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_ProcessInvokeTAOpenSession(
	ADBG_Case_t *c, TEEC_Session *sess, uint32_t cmdId,
	uint32_t TACmd, TEEC_UUID *UUID, uint32_t returnOrigin)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_UUID swapped_uuid = *UUID;
	uint32_t org;

	swapped_uuid.timeLow = htonl(swapped_uuid.timeLow);
	swapped_uuid.timeMid = htons(swapped_uuid.timeMid);
	swapped_uuid.timeHiAndVersion = htons(swapped_uuid.timeHiAndVersion);

	ALLOCATE_AND_FILL_SHARED_MEMORY(sess->ctx, SHARE_MEM01, BIG_SIZE,
					TEEC_MEM_INPUT, 16, &swapped_uuid,
					mem01_exit)

	op.params[0].value.a = TACmd;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01, 16)
	op.params[2].value.a = returnOrigin;

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT, TEEC_MEMREF_PARTIAL_INPUT, TEEC_VALUE_OUTPUT,
		TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (TEE_ORIGIN_NOT_TRUSTED_APP == returnOrigin) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, op.params[2].value.a, !=,
					     TEE_ORIGIN_TRUSTED_APP);
	} else {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, op.params[2].value.a, ==,
					     returnOrigin);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_CheckMemoryAccessRight(ADBG_Case_t *c,
						 TEEC_Session *sess,
						 uint32_t cmdId,
						 uint32_t memoryParamType,
						 uint32_t memoryAccessFlags)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;
	uint32_t memory_flag;

	switch (memoryParamType) {
	case TEEC_MEMREF_TEMP_INPUT:
	case TEEC_MEMREF_PARTIAL_INPUT:
		memory_flag = TEEC_MEM_INPUT;
		break;
	case TEEC_MEMREF_TEMP_OUTPUT:
	case TEEC_MEMREF_PARTIAL_OUTPUT:
		memory_flag = TEEC_MEM_OUTPUT;
		break;
	case TEEC_MEMREF_TEMP_INOUT:
	case TEEC_MEMREF_PARTIAL_INOUT:
	case TEEC_MEMREF_WHOLE:
		memory_flag = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
		break;
	default:
		memory_flag = 0;
		break;
	}

	ALLOCATE_SHARED_MEMORY(sess->ctx, SHARE_MEM01, BIG_SIZE,
			       memory_flag, mem01_exit)

	op.params[0].value.a = memoryAccessFlags;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT, memoryParamType, TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_MemMove(ADBG_Case_t *c, TEEC_Session *sess,
				  uint32_t cmdId, uint32_t memorySize)
{
	return Invoke_Simple_Function_v1(c, sess, cmdId, memorySize, 0);
}

static TEEC_Result Invoke_AllocatePropertyEnumerator(ADBG_Case_t *c,
						     TEEC_Session *sess,
						     uint32_t cmdId,
						     uint32_t *enumerator)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (res != TEEC_SUCCESS)
		goto exit;

	*enumerator = op.params[0].value.a;
exit:
	return res;
}

static TEEC_Result Invoke_StartPropertyEnumerator(ADBG_Case_t *c,
						  TEEC_Session *sess,
						  uint32_t cmdId,
						  uint32_t *enumerator,
						  TEE_PropSetHandle propSet)
{
	return Invoke_Simple_Function_v2(c, sess, cmdId, *enumerator, 0,
					 (unsigned long)propSet, 0);
}

static TEEC_Result Invoke_ResetPropertyEnumerator(ADBG_Case_t *c,
						  TEEC_Session *sess,
						  uint32_t cmdId,
						  uint32_t *enumerator)
{
	return Invoke_Simple_Function_v1(c, sess, cmdId, *enumerator, 0);
}

static TEEC_Result Invoke_GetPropertyName(ADBG_Case_t *c, TEEC_Session *sess,
					  uint32_t cmdId, uint32_t *enumerator,
					  char *propertyName,
					  uint32_t kindBuffer)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;
	uint32_t strLen = 0;

	if (kindBuffer == TOO_SHORT_BUFFER) {
		ALLOCATE_SHARED_MEMORY(sess->ctx, SHARE_MEM01, 1,
				       TEEC_MEM_OUTPUT, mem01_exit)
	} else {
		ALLOCATE_SHARED_MEMORY(sess->ctx, SHARE_MEM01, BIG_SIZE,
				       TEEC_MEM_OUTPUT, mem01_exit)
	}

	op.params[0].value.a = *enumerator;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT, TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE,
		TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (res != TEEC_SUCCESS)
		goto exit;

	strLen = strlen(propertyName) + 1;

	(void)ADBG_EXPECT_COMPARE_SIGNED(c, op.params[1].memref.size, ==,
					 strLen);

	(void)ADBG_EXPECT_COMPARE_SIGNED(c,
					 0, ==,
					 memcmp(SHARE_MEM01->buffer,
						propertyName, strLen));

exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Malloc(ADBG_Case_t *c, TEEC_Session *sess,
				 uint32_t cmdId, uint32_t memorySize,
				 uint32_t hint)
{
	return Invoke_Simple_Function_v2(c, sess, cmdId, memorySize, 0, hint,
					 0);
}

static TEEC_Result Invoke_Panic(ADBG_Case_t *c, TEEC_Session *sess,
				uint32_t cmdId)
{
	TEEC_Result res = TEE_ERROR_NOT_SUPPORTED;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org;

	op.paramTypes = TEEC_PARAM_TYPES(
		TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmdId, &op, &org);

	if (res != TEEC_SUCCESS)
		goto exit;

	ADBG_EXPECT_COMPARE_SIGNED(c, org, ==, TEE_ORIGIN_TEE);

exit:
	return res;
}

static TEEC_Result Invoke_Realloc(ADBG_Case_t *c, TEEC_Session *sess,
				  uint32_t cmdId, uint32_t oldMemorySize,
				  uint32_t newMemorySize)
{
	return Invoke_Simple_Function_v2(c, sess, cmdId, oldMemorySize, 0,
					 newMemorySize, 0);
}

static TEEC_Result Invoke_Free(ADBG_Case_t *c, TEEC_Session *sess,
			       uint32_t cmdId, uint32_t Case)
{
	return Invoke_Simple_Function_v1(c, sess, cmdId, Case, 0);
}

static TEEC_Result Invoke_OpenTASession(ADBG_Case_t *c, TEEC_Session *sess,
					uint32_t cmd_id, uint32_t ta_session,
					TEEC_UUID *uuid, uint32_t exp_eo)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t eo = 0;
	TEEC_UUID swapped_uuid = *uuid;
	TEEC_SharedMemory shm = {
		.flags = TEEC_MEM_INPUT,
		.buffer = &swapped_uuid,
		.size = sizeof(*uuid),
	};

	swapped_uuid.timeLow = htonl(swapped_uuid.timeLow);
	swapped_uuid.timeMid = htons(swapped_uuid.timeMid);
	swapped_uuid.timeHiAndVersion = htons(swapped_uuid.timeHiAndVersion);

	res = TEEC_RegisterSharedMemory(sess->ctx, &shm);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_VALUE_OUTPUT, TEEC_NONE);
	op.params[0].value.a = ta_session;

	op.params[1].memref.offset = 0;
	op.params[1].memref.size = sizeof(*uuid);
	op.params[1].memref.parent = &shm;

	res = TEEC_InvokeCommand(sess, cmd_id, &op, &eo);
	TEEC_ReleaseSharedMemory(&shm);

	if (!ADBG_EXPECT(c, TEE_ORIGIN_TRUSTED_APP, eo))
		return TEEC_ERROR_GENERIC;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;
	ADBG_EXPECT(c, exp_eo, op.params[2].value.b);
	return op.params[2].value.a;
}

static TEEC_Result Invoke_InvokeTACommand(ADBG_Case_t *c, TEEC_Session *sess,
					  uint32_t cmd_id, uint32_t ta_session,
					  uint32_t ta_cmd_id, uint32_t exp_eo)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t eo = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].value.a = ta_session;
	op.params[0].value.b = ta_cmd_id;

	res = TEEC_InvokeCommand(sess, cmd_id, &op, &eo);

	if (!ADBG_EXPECT(c, TEE_ORIGIN_TRUSTED_APP, eo))
		return TEEC_ERROR_GENERIC;
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;
	ADBG_EXPECT(c, exp_eo, op.params[1].value.b);
	return op.params[1].value.a;
}

static TEEC_Result Invoke_CloseTASession(ADBG_Case_t *c, TEEC_Session *sess,
					 uint32_t cmd_id, uint32_t ta_session)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t eo = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].value.a = ta_session;

	res = TEEC_InvokeCommand(sess, cmd_id, &op, &eo);

	ADBG_EXPECT(c, TEE_ORIGIN_TRUSTED_APP, eo);
	return res;
}
#endif /* XML_INTERNAL_API_H_ */
