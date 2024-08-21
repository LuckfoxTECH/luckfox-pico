// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include "sha2_taf.h"
#include "sha2_impl.h"

TEE_Result ta_entry_sha224(uint32_t param_types, TEE_Param params[4])
{
	/*
	 * It is expected that memRef[0] is input buffer and memRef[1] is
	 * output buffer.
	 */
	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
			    TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			    TEE_PARAM_TYPE_NONE)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[1].memref.size < SHA224_DIGEST_SIZE)
		return TEE_ERROR_BAD_PARAMETERS;

	sha224((unsigned char *)params[0].memref.buffer,
	       (unsigned int)params[0].memref.size,
	       (unsigned char *)params[1].memref.buffer);

	return TEE_SUCCESS;
}

TEE_Result ta_entry_sha256(uint32_t param_types, TEE_Param params[4])
{
	/*
	 * It is expected that memRef[0] is input buffer and memRef[1] is
	 * output buffer.
	 */
	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
			    TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			    TEE_PARAM_TYPE_NONE)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (params[1].memref.size < SHA256_DIGEST_SIZE)
		return TEE_ERROR_BAD_PARAMETERS;

	sha256((unsigned char *)params[0].memref.buffer,
	       (unsigned int)params[0].memref.size,
	       (unsigned char *)params[1].memref.buffer);

	return TEE_SUCCESS;
}
