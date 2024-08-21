// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "xtest_test.h"
#include "xtest_helpers.h"
#include "tee_api_defines.h"
#include "tee_client_api.h"

#define OFFSET0 0

#define PARAM_0 0
#define PARAM_1 1
#define PARAM_2 2
#define PARAM_3 3

struct xtest_session {
	ADBG_Case_t *c;
	TEEC_Session session;
	TEEC_Context context;
};

/* Compares two memories and checks if their length and content is the same */
#define EXPECT_SHARED_MEM_BUFFER(c, exp_buf, exp_blen, op, param_num, shrm) \
	do { \
		if ((exp_buf) == NULL) { \
			ADBG_EXPECT((c), exp_blen, \
				    (op)->params[(param_num)].memref.size); \
		} else { \
			ADBG_EXPECT_COMPARE_POINTER((c), (shrm), ==, \
			    (op)->params[(param_num)].memref.parent); \
			ADBG_EXPECT_BUFFER((c), (exp_buf), (exp_blen), \
			   (shrm)->buffer, \
			   (op)->params[(param_num)].memref.size); \
		} \
	} while (0)

/*
 * Compares the content of the memory cells in OP with the expected value
 * contained.
 */
#define EXPECT_OP_TMP_MEM_BUFFER(c, exp_buf, exp_blen, op, param_num, buf) \
	do { \
		if ((exp_buf) == NULL) { \
			ADBG_EXPECT((c), exp_blen, \
			    (op)->params[(param_num)].tmpref.size); \
		} else { \
			ADBG_EXPECT_COMPARE_POINTER((c), (buf), ==, \
			    (op)->params[(param_num)].tmpref.buffer); \
			ADBG_EXPECT_BUFFER((c), (exp_buf), (exp_blen), \
			   (buf), \
			   (op)->params[(param_num)].memref.size); \
		} \
	} while (0)

/* Registers the TEEC_SharedMemory to the TEE. */
static TEEC_Result RegisterSharedMemory(TEEC_Context *ctx,
					TEEC_SharedMemory *shm, size_t size,
					uint32_t flags)
{
	shm->flags = flags;
	shm->size = size;
	return TEEC_RegisterSharedMemory(ctx, shm);
}

/* Allocates shared memory inside of the TEE. */
static TEEC_Result AllocateSharedMemory(TEEC_Context *ctx,
					TEEC_SharedMemory *shm, size_t size,
					uint32_t flags)
{
	shm->flags = flags;
	shm->size = size;
	return TEEC_AllocateSharedMemory(ctx, shm);
}

static void CloseSession_null(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "CloseSession_null");
	{
		/* In reality doesn't test anything. */
		TEEC_CloseSession(NULL);
	}
	Do_ADBG_EndSubCase(cs->c, "CloseSession_null");
}

static void Allocate_In(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Allocate_In");
	{
		TEEC_SharedMemory shm = { };
		size_t size = 1024;

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm, size,
					     TEEC_MEM_INPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Allocate_In");
}

static void Allocate_out_of_memory(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Allocate_out_of_memory");
	{
		TEEC_SharedMemory shm = { };
		size_t SIZE_OVER_MEMORY_CAPACITY = SIZE_MAX;

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		ADBG_EXPECT_TEEC_RESULT(cs->c, TEEC_ERROR_OUT_OF_MEMORY,
			AllocateSharedMemory(&cs->context, &shm,
					     SIZE_OVER_MEMORY_CAPACITY,
					     TEEC_MEM_INPUT));
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Allocate_out_of_memory");
}

static void OpenSession_error_notExistingTA(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "OpenSession_error_notExistingTA");
	{
		TEEC_UUID NONEXISTING_TA_UUID = { 0x534D1192, 0x6143, 0x234C,
						  { 0x47, 0x55, 0x53, 0x52,
						    0x54, 0x4F, 0x4F, 0x59 } };
		uint32_t ret_orig = 0;

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		ADBG_EXPECT_COMPARE_UNSIGNED(cs->c, TEEC_SUCCESS, !=,
			TEEC_OpenSession(&cs->context, &cs->session,
					 &NONEXISTING_TA_UUID,
					 TEEC_LOGIN_PUBLIC, NULL, NULL,
					 &ret_orig));
		ADBG_EXPECT_COMPARE_UNSIGNED(cs->c, TEEC_ORIGIN_TRUSTED_APP, !=,
					     ret_orig);
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "OpenSession_error_notExistingTA");
}

static void Allocate_InOut(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Allocate_InOut");
	{
		TEEC_SharedMemory shm = { };
		uint8_t val[] = { 54, 76, 98, 32 };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm, sizeof(val),
					     TEEC_MEM_INPUT | TEEC_MEM_OUTPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Allocate_InOut");
}

static void Register_In(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Register_In");
	{
		TEEC_SharedMemory shm = { };
		uint8_t val[] = { 32, 65, 43, 21, 98 };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		shm.buffer = val;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, sizeof(val),
					     TEEC_MEM_INPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Register_In");
}

static void Register_notZeroLength_Out(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Register_notZeroLength_Out");
	{
		TEEC_SharedMemory shm = { };
		uint8_t val[] = { 56, 67, 78, 99 };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		shm.buffer = val;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, sizeof(val),
					     TEEC_MEM_OUTPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Register_notZeroLength_Out");
}

static void Register_InOut(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Register_InOut");
	{
		TEEC_SharedMemory shm = { };
		uint8_t val[] = { 54, 76, 23, 98, 255, 23, 86 };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		shm.buffer = val;
		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, sizeof(val),
					     TEEC_MEM_INPUT | TEEC_MEM_OUTPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Register_InOut");
}

static void Register_zeroLength_Out(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Register_zeroLength_Out");
	{
		uint8_t val[] = { 65, 76, 98, 32 };
		TEEC_SharedMemory shm = { };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		shm.buffer = val;
		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, 0,
					     TEEC_MEM_OUTPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Register_zeroLength_Out");
}

static void Allocate_Out(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Allocate_Out");
	{
		TEEC_SharedMemory shm = { };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm, 0,
					     TEEC_MEM_OUTPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Allocate_Out");
}

static void FinalizeContext_null(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "FinalizeContext_null");
	{
		TEEC_FinalizeContext(NULL);
	}
	Do_ADBG_EndSubCase(cs->c, "FinalizeContext_null");
}

static void InitializeContext_NotExistingTEE(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "InitializeContext_NotExistingTEE");
	{
		if (!ADBG_EXPECT_COMPARE_UNSIGNED(cs->c, TEEC_SUCCESS, !=,
			TEEC_InitializeContext("Invalid TEE name",
					       &cs->context)))
			TEEC_FinalizeContext(&cs->context);
	}
	Do_ADBG_EndSubCase(cs->c, "InitializeContext_NotExistingTEE");
}

static void AllocateThenRegister_SameMemory(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "AllocateThenRegister_SameMemory");
	{
		TEEC_SharedMemory shm = { };
		size_t size_allocation = 32;

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm,
					     size_allocation, TEEC_MEM_INPUT)))
			goto out_final;

		ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm,
					     size_allocation, TEEC_MEM_INPUT));

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "AllocateThenRegister_SameMemory");
}

static void AllocateSameMemory_twice(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "AllocateSameMemory_twice");
	{
		TEEC_SharedMemory shm = { };
		size_t size_allocation = 32;

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm,
					     size_allocation, TEEC_MEM_INPUT)))
			goto out_final;

		ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm,
					     size_allocation, TEEC_MEM_INPUT));

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "AllocateSameMemory_twice");
}

static void RegisterSameMemory_twice(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "RegisterSameMemory_twice");
	{
		uint8_t val[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
		TEEC_SharedMemory shm = { };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		shm.buffer = val;
		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, sizeof(val),
					     TEEC_MEM_INPUT)))
			goto out_final;

		ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, sizeof(val),
					     TEEC_MEM_INPUT));

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "RegisterSameMemory_twice");
}

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

static void Allocate_sharedMemory_32k(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Allocate_sharedMemory_32k");
	{
		size_t size = MIN(32 * 1024,
				  TEEC_CONFIG_SHAREDMEM_MAX_SIZE);
		TEEC_SharedMemory shm = { };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			AllocateSharedMemory(&cs->context, &shm, size,
					     TEEC_MEM_INPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Allocate_sharedMemory_32k");
}

#define SHM_32K_SIZE	MIN(32 * 1024, TEEC_CONFIG_SHAREDMEM_MAX_SIZE)

static void Register_sharedMemory_32k(struct xtest_session *cs)
{
	Do_ADBG_BeginSubCase(cs->c, "Register_sharedMemory_32k");
	{
		uint8_t val[SHM_32K_SIZE] = { };
		TEEC_SharedMemory shm = { };

		if (!ADBG_EXPECT(cs->c, TEEC_SUCCESS,
			TEEC_InitializeContext(xtest_tee_name, &cs->context)))
			goto out;

		shm.buffer = val;
		if (!ADBG_EXPECT_TEEC_SUCCESS(cs->c,
			RegisterSharedMemory(&cs->context, &shm, SHM_32K_SIZE,
					     TEEC_MEM_INPUT)))
			goto out_final;

		TEEC_ReleaseSharedMemory(&shm);
out_final:
		TEEC_FinalizeContext(&cs->context);
	}
out:
	Do_ADBG_EndSubCase(cs->c, "Register_sharedMemory_32k");
}

static void xtest_teec_TEE(ADBG_Case_t *c)
{
	struct xtest_session connection = { c };

	CloseSession_null(&connection);

	Allocate_In(&connection);

	Allocate_out_of_memory(&connection);

	OpenSession_error_notExistingTA(&connection);

	Allocate_InOut(&connection);

	Register_In(&connection);

	Register_notZeroLength_Out(&connection);

	Register_InOut(&connection);

	Register_zeroLength_Out(&connection);

	Allocate_Out(&connection);

	FinalizeContext_null(&connection);

	InitializeContext_NotExistingTEE(&connection);

	AllocateThenRegister_SameMemory(&connection);

	AllocateSameMemory_twice(&connection);

	RegisterSameMemory_twice(&connection);

	Allocate_sharedMemory_32k(&connection);

	Register_sharedMemory_32k(&connection);
}

ADBG_CASE_DEFINE(regression, 5006, xtest_teec_TEE,
		"Tests for Global platform TEEC");
