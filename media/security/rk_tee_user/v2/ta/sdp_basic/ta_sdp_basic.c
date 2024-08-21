// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 */

#include <pta_invoke_tests.h>
#include <string.h>
#include <ta_sdp_basic.h>
#include <tee_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_internal_api.h>
#include <tee_ta_api.h>
#include <trace.h>


/*
 * Basic Secure Data Path access test commands:
 * - command INJECT: copy from non secure input into secure output.
 * - command TRANSFROM: read, transform and write from/to secure in/out.
 * - command DUMP: copy from secure input into non secure output.
 */

static TEE_Result cmd_inject(uint32_t types,
			     TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result rc = TEE_ERROR_GENERIC;
	const int sec_idx = 1;		/* highlight secure buffer index */
	const int ns_idx = 0;		/* highlight nonsecure buffer index */

	if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				     TEE_PARAM_TYPE_MEMREF_OUTPUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE)) {
		EMSG("bad parameters %x", (unsigned)types);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[sec_idx].memref.size < params[ns_idx].memref.size)
		return TEE_ERROR_SHORT_BUFFER;

	/*
	 * We could rely on the TEE to provide consistent buffer/size values
	 * to reference a buffer with a unique and consistent secure attribute
	 * value. Hence it is safe enough (and more optimal) to test only the
	 * secure attribute of a single byte of it. Yet, since the current
	 * test does not deal with performance, let check the secure attribute
	 * of each byte of the buffer.
	 */
	rc = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_READ |
					 TEE_MEMORY_ACCESS_NONSECURE,
					 params[ns_idx].memref.buffer,
					 params[ns_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CheckMemoryAccessRights(nsec) failed %x", rc);
		return rc;
	}

	rc = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_WRITE |
					 TEE_MEMORY_ACCESS_SECURE,
					 params[sec_idx].memref.buffer,
					 params[sec_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CheckMemoryAccessRights(secure) failed %x", rc);
		return rc;
	}


#ifdef CFG_CACHE_API
	/*
	 * we should invalidate cache (here we assume buffer were not
	 * filled through cpu core caches. We flush buffers so that
	 * cache is not corrupted in cache target buffer not aligned
	 * on cache line size.
	 */
	rc = TEE_CacheFlush(params[sec_idx].memref.buffer,
			    params[sec_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CacheFlush(%p, %x) failed: 0x%x",
					params[sec_idx].memref.buffer,
					params[sec_idx].memref.size, rc);
		return rc;
	}
#endif /* CFG_CACHE_API */

	/* inject data */
	TEE_MemMove(params[sec_idx].memref.buffer,
		    params[ns_idx].memref.buffer,
		    params[sec_idx].memref.size);

#ifdef CFG_CACHE_API
	rc = TEE_CacheFlush(params[sec_idx].memref.buffer,
			    params[sec_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CacheFlush(%p, %x) failed: 0x%x",
					params[sec_idx].memref.buffer,
					params[sec_idx].memref.size, rc);
		return rc;
	}
#endif /* CFG_CACHE_API */

	return rc;
}

static TEE_Result cmd_transform(uint32_t types,
				TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result rc = TEE_ERROR_GENERIC;
	unsigned char *p = NULL;
	size_t sz = 0;

	if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE))
		return TEE_ERROR_BAD_PARAMETERS;

	/*
	 * We could rely on the TEE to provide consistent buffer/size values
	 * to reference a buffer with a unique and consistent secure attribute
	 * value. Hence it is safe enough (and more optimal) to test only the
	 * secure attribute of a single byte of it. Yet, since the current
	 * test does not deal with performance, let check the secure attribute
	 * of each byte of the buffer.
	 */
	rc = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_READ |
					 TEE_MEMORY_ACCESS_WRITE |
					 TEE_MEMORY_ACCESS_SECURE,
					 params[0].memref.buffer,
					 params[0].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CheckMemoryAccessRights(secure) failed %x", rc);
		return rc;
	}


#ifdef CFG_CACHE_API
	/*
	 * we should invalidate cache (here we assume buffer were not
	 * filled through cpu core caches. We flush buffers so that
	 * cache is not corrupted in cache target buffer not aligned
	 * on cache line size.
	 */
	rc = TEE_CacheFlush(params[0].memref.buffer,
			    params[0].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CacheFlush(%p, %x) failed: 0x%x",
					params[0].memref.buffer,
					params[0].memref.size, rc);
		return rc;
	}
#endif /* CFG_CACHE_API */

	/* transform the data */
	p = (unsigned char *)params[0].memref.buffer;
	sz = params[0].memref.size;
	for (; sz; sz--, p++)
			*p = ~(*p) + 1;

#ifdef CFG_CACHE_API
	rc = TEE_CacheFlush(params[0].memref.buffer,
			    params[0].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CacheFlush(%p, %x) failed: 0x%x",
					params[0].memref.buffer,
					params[0].memref.size, rc);
		return rc;
	}
#endif /* CFG_CACHE_API */

	return rc;
}

static TEE_Result cmd_dump(uint32_t types,
			   TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result rc = TEE_ERROR_GENERIC;
	const int sec_idx = 0;		/* highlight secure buffer index */
	const int ns_idx = 1;		/* highlight nonsecure buffer index */

	if (types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
				     TEE_PARAM_TYPE_MEMREF_OUTPUT,
				     TEE_PARAM_TYPE_NONE,
				     TEE_PARAM_TYPE_NONE))
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[ns_idx].memref.size < params[sec_idx].memref.size)
		return TEE_ERROR_SHORT_BUFFER;

	/*
	 * We could rely on the TEE to provide consistent buffer/size values
	 * to reference a buffer with a unique and consistent secure attribute
	 * value. Hence it is safe enough (and more optimal) to test only the
	 * secure attribute of a single byte of it. Yet, since the current
	 * test does not deal with performance, let check the secure attribute
	 * of each byte of the buffer.
	 */
	rc = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_WRITE |
					 TEE_MEMORY_ACCESS_NONSECURE,
					 params[ns_idx].memref.buffer,
					 params[ns_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CheckMemoryAccessRights(nsec) failed %x", rc);
		return rc;
	}

	rc = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_ANY_OWNER |
					 TEE_MEMORY_ACCESS_READ |
					 TEE_MEMORY_ACCESS_SECURE,
					 params[sec_idx].memref.buffer,
					 params[sec_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CheckMemoryAccessRights(secure) failed %x", rc);
		return rc;
	}

#ifdef CFG_CACHE_API
	/*
	 * we should invalidate cache (here we assume buffer were not
	 * filled through cpu core caches. We flush buffers so that
	 * cache is not corrupted in cache target buffer not aligned
	 * on cache line size.
	 */
	rc = TEE_CacheFlush(params[sec_idx].memref.buffer,
			    params[sec_idx].memref.size);
	if (rc != TEE_SUCCESS) {
		EMSG("TEE_CacheFlush(%p, %x) failed: 0x%x",
					params[sec_idx].memref.buffer,
					params[sec_idx].memref.size, rc);
		return rc;
	}
#endif /* CFG_CACHE_API */

	/* dump the data */
	TEE_MemMove(params[ns_idx].memref.buffer,
		    params[sec_idx].memref.buffer,
		    params[sec_idx].memref.size);

	return rc;
}

static TEE_Result cmd_invoke(uint32_t nParamTypes,
			     TEE_Param pParams[TEE_NUM_PARAMS],
			     uint32_t nCommandID)
{
        const TEE_UUID uuid = TA_SDP_BASIC_UUID;
        static TEE_TASessionHandle sess = TEE_HANDLE_NULL;
        uint32_t ret_orig = 0;
        TEE_Result res = TEE_ERROR_GENERIC;

	if (sess == TEE_HANDLE_NULL) {
	        res = TEE_OpenTASession(&uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
					&sess, &ret_orig);
		if (res != TEE_SUCCESS) {
	                EMSG("SDP basic test TA: TEE_OpenTASession() FAILED \n");
	                goto cleanup_return;
		}

        }

        res = TEE_InvokeTACommand(sess, TEE_TIMEOUT_INFINITE,
				  nCommandID, nParamTypes, pParams, &ret_orig);
        if (res != TEE_SUCCESS) {
                EMSG("SDP basic test TA: TEE_OpenTASession() FAILED %x/%d\n",
								res, ret_orig);
        }

cleanup_return:
	if (res != TEE_SUCCESS) {
		TEE_CloseTASession(sess);
		sess = TEE_HANDLE_NULL;
	}
        return res;
}

static TEE_Result cmd_invoke_pta(uint32_t nParamTypes,
			     TEE_Param pParams[TEE_NUM_PARAMS],
			     uint32_t nCommandID)
{
        const TEE_UUID uuid = PTA_INVOKE_TESTS_UUID;
        static TEE_TASessionHandle sess = TEE_HANDLE_NULL;
        uint32_t ret_orig = 0;
        TEE_Result res = TEE_ERROR_GENERIC;

	if (sess == TEE_HANDLE_NULL) {
	        res = TEE_OpenTASession(&uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
					&sess, &ret_orig);
		if (res != TEE_SUCCESS) {
	                EMSG("SDP basic test TA: TEE_OpenTASession() FAILED \n");
	                goto cleanup_return;
		}

        }

        res = TEE_InvokeTACommand(sess, TEE_TIMEOUT_INFINITE,
				  nCommandID, nParamTypes, pParams, &ret_orig);
        if (res != TEE_SUCCESS) {
                EMSG("SDP basic test TA: TEE_OpenTASession() FAILED %x/%d\n",
								res, ret_orig);
        }

cleanup_return:
	if (res != TEE_SUCCESS) {
		TEE_CloseTASession(sess);
		sess = TEE_HANDLE_NULL;
	}
        return res;
}

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes,
				    TEE_Param pParams[TEE_NUM_PARAMS],
				    void **ppSessionContext)
{
	(void)nParamTypes;
	(void)pParams;
	(void)ppSessionContext;
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *pSessionContext)
{
	(void)pSessionContext;
}

TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[TEE_NUM_PARAMS])
{
	(void)pSessionContext;

	switch (nCommandID) {
	case TA_SDP_BASIC_CMD_INJECT:
		return cmd_inject(nParamTypes, pParams);
	case TA_SDP_BASIC_CMD_TRANSFORM:
		return cmd_transform(nParamTypes, pParams);
	case TA_SDP_BASIC_CMD_DUMP:
		return cmd_dump(nParamTypes, pParams);

	case TA_SDP_BASIC_CMD_INVOKE_INJECT:
		return cmd_invoke(nParamTypes, pParams, TA_SDP_BASIC_CMD_INJECT);
	case TA_SDP_BASIC_CMD_INVOKE_TRANSFORM:
		return cmd_invoke(nParamTypes, pParams, TA_SDP_BASIC_CMD_TRANSFORM);
	case TA_SDP_BASIC_CMD_INVOKE_DUMP:
		return cmd_invoke(nParamTypes, pParams, TA_SDP_BASIC_CMD_DUMP);

	case TA_SDP_BASIC_CMD_PTA_INJECT:
		return cmd_invoke_pta(nParamTypes, pParams, PTA_INVOKE_TESTS_CMD_COPY_NSEC_TO_SEC);
	case TA_SDP_BASIC_CMD_PTA_TRANSFORM:
		return cmd_invoke_pta(nParamTypes, pParams, PTA_INVOKE_TESTS_CMD_READ_MODIFY_SEC);
	case TA_SDP_BASIC_CMD_PTA_DUMP:
		return cmd_invoke_pta(nParamTypes, pParams, PTA_INVOKE_TESTS_CMD_COPY_SEC_TO_NSEC);

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
