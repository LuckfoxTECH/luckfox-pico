/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * Copyright (c) 2020, Linaro Limited
 */

#ifndef XML_CLIENT_API_H_
#define XML_CLIENT_API_H_

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tee_api_defines.h>
#include <tee_client_api.h>
#include <unistd.h>

#include <xml_common_api.h>
#include "xtest_helpers.h"
#include "xtest_test.h"

/* ta_testingClientAPI_test.h*/
#define COMMAND_TTA_Remember_Expected_ParamTypes 0x00000002
#define COMMAND_TTA_Copy_ParamIn_to_ParamOut	0x00000001
#define COMMAND_TTA_Check_ParamTypes		0x00000003
#define COMMAND_TTA_To_Be_Cancelled		0x00000004
#define COMMAND_TTA_Success			0x00000005
#define COMMAND_TTA_Panic			0x00000006

/* ta_answerSuccessTo_OpenSession_Invoke_test.h */
#define COMMAND_TTA_Remember_Expected_ParamTypes 0x00000002
#define COMMAND_TTA_Copy_ParamIn_to_ParamOut	0x00000001
#define COMMAND_TTA_Check_ParamTypes		0x00000003

/* ta_testingClientAPI_test.h */
#define COMMAND_TTA_Remember_Expected_ParamTypes 0x00000002
#define COMMAND_TTA_Copy_ParamIn_to_ParamOut	0x00000001
#define COMMAND_TTA_Check_ParamTypes		0x00000003
#define COMMAND_TTA_To_Be_Cancelled		0x00000004
#define COMMAND_TTA_Success			0x00000005
#define COMMAND_TTA_Panic			0x00000006

#define INVALID_CONNECTION_METHODS			0x0A
#define COMMAND_TTA_Check_Expected_ParamTypes	COMMAND_TTA_Check_ParamTypes
#define COMMAND_TTA_Check_Update_Params		0xFFFF0002u
#define COMMAND_TTA_Store_Expected_Param_Info	0xFFFF0001u

#define CLIENT_APP01	NULL

#define TEEC_UNDEFINED_ERROR 0xDEADDEAD

#define TEEC_ORIGIN_ANY_NOT_TRUSTED_APP  0x00000005

#define OFFSET0 0
#define OFFSET_02 0x64

#define BIG_SIZE			1024
#define IGNORE				0xFEFEFEFE
#define VALUE01				0x01234567
#define VALUE02				0xFEDCBA98
#define SIZE_OVER_MEMORY		0xFFFFFFFE
#define SIZE_VALUE01			4
#define SIZE_LESSER_THAN_SIZE_02	0x1B58
#define SIZE_GREATER_THAN_SIZE_02	0x2328
#define ZERO				0
#define SIZE_02				0x2000
#define ALLOC_SIZE_02			0x2800
#define BYTE_01				1
#define BYTE_02				2
#define BYTE_03				3
#define BYTE_04				4

#define VALUE_A_IN_0			0x01234567
#define VALUE_A_IN_1			0xF9E8D7C6
#define VALUE_A_IN_2			0xE01C083D
#define VALUE_A_IN_3			0xDCA65016
#define VALUE_A_OUT_0			0xABCD0248
#define VALUE_A_OUT_1			0x03579EF4
#define VALUE_A_OUT_2			0x344C64BC
#define VALUE_A_OUT_3			0x3590BBD9
#define VALUE_B_IN_0			0x89ABCDEF
#define VALUE_B_IN_1			0x1248DCBA
#define VALUE_B_IN_2			0x5E816B61
#define VALUE_B_IN_3			0x4C899A96
#define VALUE_B_OUT_0			0x1A2B3C4D
#define VALUE_B_OUT_1			0x1439F7A2
#define VALUE_B_OUT_2			0x6EC61CAE
#define VALUE_B_OUT_3			0xB2639F77

#define TEEC_MEM_INOUT			(TEEC_MEM_INPUT | TEEC_MEM_OUTPUT)

static TEEC_SharedMemory share_mem[4];
static TEEC_SharedMemory *SHARE_MEM01 = share_mem;
static TEEC_SharedMemory *SHARE_MEM02 = share_mem + 1;
static TEEC_SharedMemory *SHARE_MEM03 = share_mem + 2;
static TEEC_SharedMemory *SHARE_MEM04 = share_mem + 3;
static TEEC_SharedMemory *SHARE_MEM_NULL_BUFFER;
static TEEC_Session session[2];
static TEEC_Session *SESSION01 = session;
static TEEC_Session *SESSION02 = session + 1;
static TEEC_Context context[2];
static TEEC_Context *CONTEXT01 = context;
static TEEC_Context *CONTEXT02 = context + 1;
static TEEC_Operation operation[1];
static TEEC_Operation *OPERATION01 = operation;

/* "ItIsNotTotosTEEs" */
static const char *INVALID_NOT_EXISTING_TEE = "ItIsNotTotosTEEs\0";

/** ALL_TEMPORARY_MEMORIES */
static uint8_t *TEMP_MEM01;
static uint8_t *TEMP_MEM02;
static uint8_t *TEMP_MEM03;
static uint8_t *TEMP_MEM04;
static uint8_t *TEMP_MEM_NULL_BUFFER;

static TEEC_UUID UUID_TTA_answerErrorTo_OpenSession = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x45, 0x52, 0x52, 0x54, 0x4F, 0x4F, 0x53 }
};
static TEEC_UUID UUID_TTA_answerSuccessTo_OpenSession_Invoke = {
	0x534D4152, 0x542D, 0x4353,
	{ 0x4C, 0x54, 0x2D, 0x54, 0x41, 0x2D, 0x53, 0x55 }
};
static TEEC_UUID UUID_Unknown = {
	0x534D4152, 0x542D, 0x4355,
	{ 0x4E, 0x4B, 0x2D, 0x4E, 0x4F, 0x2D, 0x57, 0x4E }
};
static TEEC_UUID UUID_TTA_testingClientAPI_Parameters_OpenSession = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x43, 0x4C, 0x49, 0x50, 0x4F, 0x50, 0x53 }
};
static TEEC_UUID UUID_TTA_testingClientAPI = {
	0x534D4152, 0x542D, 0x4353,
	{ 0x4C, 0x54, 0x2D, 0x54, 0x41, 0x2D, 0x53, 0x54 }
};
static TEEC_UUID UUID_TTA_answerErrorTo_Invoke = {
	0x534D4152, 0x542D, 0x4353,
	{ 0x4C, 0x54, 0x2D, 0x54, 0x41, 0x2D, 0x45, 0x52 }
};
/* TTA_testingClientAPI_Parameters_Invoke */
static TEEC_UUID UUID_TTA_testingClientAPI_Parameters = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x43, 0x4C, 0x49, 0x50, 0x41, 0x52, 0x41 }
};

/*Registers the TEEC_SharedMemory to the TEE*/
static TEEC_Result RegisterSharedMemory(TEEC_Context *ctx,
					TEEC_SharedMemory *shm,
					uint32_t size, uint32_t flags)
{
	shm->flags = flags;
	shm->size = size;
	shm->buffer = malloc(size);
	return TEEC_RegisterSharedMemory(ctx, shm);
}

/*Allocates temporary memory area*/
#define AllocateTempMemory(temp_mem, size) \
	temp_mem = malloc(size)

/*Releases temporary memory area*/
#define ReleaseTempMemory(temp_mem) \
	do { \
		if (temp_mem != NULL) { \
			free(temp_mem); \
			temp_mem = NULL; \
		} \
	} while (0)


/* Assigns a and b to the value parameter */
static inline void TEEC_prepare_OperationEachParameter_value(TEEC_Operation *op,
							     size_t n,
							     uint32_t a,
							     uint32_t b)
{
	if (IGNORE != a)
		op->params[n].value.a = a;

	if (IGNORE != b)
		op->params[n].value.b = b;

}

/*Define TEEC_SharedMemory memory content.*/
#define TEEC_defineMemoryContent_sharedMemory(sh_mem, val, size_val) \
	memcpy(sh_mem->buffer, &val, size_val)

/*Define temp memory content.*/
#define TEEC_defineMemoryContent_tmpMemory(buf, val, size_val) \
	memcpy(buf, &(val), size_val)

#define INVOKE_REMEMBER_EXP_PARAM_TYPES(session, cmd, p0, p1, p2, p3, exp) \
	do { \
		uint32_t ret_orig = 0; \
 \
		memset(OPERATION01, 0x00, sizeof(TEEC_Operation)); \
		OPERATION01->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, \
							   TEEC_NONE, \
							   TEEC_NONE, \
							   TEEC_NONE); \
		OPERATION01->params[0].value.a = \
			TEEC_PARAM_TYPES((p0), (p1), (p2), (p3)); \
		ADBG_EXPECT(c, exp, \
			    TEEC_InvokeCommand(session, cmd, OPERATION01, \
					       &ret_orig));  \
		ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, TEEC_ORIGIN_TRUSTED_APP, \
					      ret_orig); \
	} while (0)

static void store_param_info(TEEC_Operation *op, uint32_t param_num,
			     uint32_t pt, uint32_t size_in, uint32_t value_in,
			     uint32_t size_out, uint32_t value_out)
{
	memset(op, 0, sizeof(*op));
	op->params[0].value.a = param_num;
	op->params[0].value.b = pt;
	switch (pt) {
	case TEE_PARAM_TYPE_MEMREF_INPUT:
	case TEE_PARAM_TYPE_MEMREF_OUTPUT:
	case TEE_PARAM_TYPE_MEMREF_INOUT:
		op->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						  TEEC_VALUE_INPUT,
						  TEEC_VALUE_INPUT,
						  TEEC_NONE);
		if (size_in != IGNORE)
			op->params[1].value.a = size_in;
		if (value_in != IGNORE)
			op->params[1].value.b = value_in;
		if (size_out != IGNORE)
			op->params[2].value.a = size_out;
		if (value_out != IGNORE)
			op->params[2].value.b = value_out;
		break;
	default:
		op->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
						  TEEC_NONE, TEEC_NONE);
		break;
	}
}

#define INVOKE_STORE_EXP_PARAM_INFO(session, cmd, paramNumber, paramType, \
				    sizeIN, valueIN, sizeOUT, valueOUT, \
				    expReturnOrigin, expTEEC_Result) \
	do { \
		uint32_t ret_orig = 0; \
 \
		store_param_info(OPERATION01, paramNumber, paramType, sizeIN, \
				 valueIN, sizeOUT, valueOUT); \
		ADBG_EXPECT(c, expTEEC_Result, \
			    TEEC_InvokeCommand(session, cmd, OPERATION01, \
					       &ret_orig));  \
		ADBG_EXPECT_TEEC_ERROR_ORIGIN(c, expReturnOrigin, ret_orig); \
	} while (0)

static void init_mem(uint8_t *buf, size_t buf_size, size_t begin_size,
		     size_t middle_size, uint8_t begin_value,
		     uint8_t middle_value, uint8_t end_value)
{
	memset(buf, begin_value, begin_size);
	memset(buf + begin_size, middle_value, middle_size);
	memset(buf + begin_size + middle_size, end_value,
	       buf_size - begin_size - middle_size);
}

#define TEEC_initialize_memory(shm, tmpMem, offset, _size, value_beginning, \
			       value_middle, value_end) \
	do { \
		if ((unsigned long)shm != IGNORE) {\
			TEEC_SharedMemory *__shm = (void *)(long)shm; \
			init_mem(__shm->buffer, __shm->size, offset, _size, \
				 value_beginning, value_middle, value_end); \
			assert(tempMem == IGNORE); \
		} else if ((unsigned long)tmpMem != IGNORE) {\
			/* \
			 * We can't tell the size of tmpMem, so we assume \
			 * it's offset + size large. \
			 */ \
			init_mem((void *)(long)tmpMem, offset + _size, offset, \
				 _size, value_beginning, value_middle, \
				 value_end); \
		} else { \
			assert(0); \
		} \
	} while (0)

static void TEEC_prepare_OperationEachParameter_memref(TEEC_Operation *op,
						      size_t param_num,
						      TEEC_SharedMemory *shm,
						      size_t offs, size_t sz)
{
	op->params[param_num] = (TEEC_Parameter){
		.memref = {
			.parent = shm, .size = sz, .offset = offs,
		}
	};
}

static void TEEC_prepare_OperationEachParameter_tmpref(TEEC_Operation *op,
						      size_t param_num,
						      void *buf, size_t sz)
{
	op->params[param_num] = (TEEC_Parameter){
		.tmpref = {
			.buffer = buf, .size = sz,
		}
	};
}

/*Compares two memories and checks if their length and content is the same */
#define TEEC_checkMemoryContent_sharedMemory(op, param_num, shrm, exp_buf, \
					     exp_blen) \
	do { \
		if ((exp_buf) == IGNORE) { \
			ADBG_EXPECT((c), exp_blen, \
				    (op)->params[(param_num)].memref.size); \
		} else { \
			ADBG_EXPECT_COMPARE_POINTER((c), (shrm), ==, \
						    (op)->params[(param_num)].\
							memref.parent); \
			ADBG_EXPECT_BUFFER((c), &(exp_buf), (exp_blen), \
					   (shrm)->buffer, \
					   (op)->params[(param_num)].\
						memref.size); \
		} \
	} while (0)

/*
 * Compares the content of the memory cells in OP with the expected value
 * contained.
 */
#define TEEC_checkMemoryContent_tmpMemory(op, param_num, \
	buf, exp_buf, exp_blen) \
	do { \
		if ((exp_buf) == 0) { \
			ADBG_EXPECT((c), exp_blen, \
				    (op)->params[(param_num)].tmpref.size); \
		} else { \
			ADBG_EXPECT_COMPARE_POINTER((c), (buf), ==, \
						    (op)->params[(param_num)].\
							tmpref.buffer); \
			ADBG_EXPECT_BUFFER((c), &(exp_buf), (exp_blen), \
					   (buf), \
					   (op)->params[(param_num)].\
						memref.size); \
		} \
	} while (0)

/*
 * Compares the content of the memory cells in OP with the expected value
 * contained.
 */
#define TEEC_checkContent_Parameter_value(op, param_num, exp_a, exp_b) \
	do { \
		if (IGNORE != exp_a) \
			ADBG_EXPECT((c), exp_a, \
				    (op)->params[(param_num)].value.a); \
		if (IGNORE != exp_b) \
			ADBG_EXPECT((c), exp_b, \
				    (op)->params[(param_num)].value.b); \
	} while (0)

/*Invoke command using TEEC_InvokeCommand and check the returned value.*/
#define XML_InvokeCommand(c, session, cmd, operation, returnOrigin, expected) \
	do { \
		uint32_t ret_orig = 0; \
 \
		ADBG_EXPECT(c, expected, \
			    TEEC_InvokeCommand(session, cmd, operation, \
					       &ret_orig)); \
		if (returnOrigin) \
			ADBG_EXPECT(c, (unsigned long)returnOrigin, ret_orig); \
	} while (0)

#endif /* XML_CLIENT_API_H_ */
