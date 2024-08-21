// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2019, Linaro Limited
 */

#include <pta_system.h>
#include <string.h>
#include <tee_internal_api.h>

#include "derive_key_taf.h"

#define TA_DERIVED_KEY_MIN_SIZE		16
#define TA_DERIVED_KEY_MAX_SIZE		32
#define TA_DERIVED_EXTRA_DATA_MAX_SIZE	1024

static const TEE_UUID system_uuid = PTA_SYSTEM_UUID;

/*
 * Helper function that just derives a key.
 */
static TEE_Result derive_unique_key(TEE_TASessionHandle session,
				    uint8_t *key, uint16_t key_size,
				    uint8_t *extra, uint16_t extra_size)
{
	TEE_Param params[TEE_NUM_PARAMS] = { 0 };
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t ret_origin = 0;
	uint32_t param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
					       TEE_PARAM_TYPE_MEMREF_OUTPUT,
					       TEE_PARAM_TYPE_NONE,
					       TEE_PARAM_TYPE_NONE);
	if (extra && extra_size > 0) {
		params[0].memref.buffer = extra;
		params[0].memref.size = extra_size;
	}

	params[1].memref.buffer = key;
	params[1].memref.size = key_size;

	res = TEE_InvokeTACommand(session, TEE_TIMEOUT_INFINITE,
				  PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY,
				  param_types, params, &ret_origin);
	if (res != TEE_SUCCESS)
		EMSG("Failure when calling PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY");

	return res;
}

TEE_Result derive_ta_unique_key_test(uint32_t param_types,
				     TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	size_t i = 0;
	TEE_Result res_final = TEE_SUCCESS;
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	uint8_t big_key[64] = { };
	uint8_t extra_key_data[] = { "My dummy data" };
	uint8_t key1[32] = { };
	uint8_t key2[32] = { };
	uint32_t ret_origin = 0;

	if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE))
		return TEE_ERROR_BAD_PARAMETERS;

	res = TEE_OpenTASession(&system_uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
				&session, &ret_origin);

	if (res != TEE_SUCCESS)
		return res;

	/*
	 * Testing for successful calls to the PTA and that two calls with same
	 * input data generates the same output data (keys).
	 */
	res = derive_unique_key(session, key1, sizeof(key1), NULL, 0);
	if (res != TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	res = derive_unique_key(session, key2, sizeof(key2), NULL, 0);
	if (res != TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	if (TEE_MemCompare(key1, key2, sizeof(key1)) != 0)
		res_final = TEE_ERROR_GENERIC;

	TEE_MemFill(key1, 0, sizeof(key1));
	TEE_MemFill(key2, 0, sizeof(key2));

	/*
	 * Testing for successful calls to the PTA and that two calls with same
	 * input data generates the same output data (keys). Here we are using
	 * extra data also.
	 */
	res = derive_unique_key(session, key1, sizeof(key1), extra_key_data,
				sizeof(extra_key_data));
	if (res != TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	res = derive_unique_key(session, key2, sizeof(key2), extra_key_data,
				sizeof(extra_key_data));
	if (res != TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	if (TEE_MemCompare(key1, key2, sizeof(key1)) != 0)
		res_final = TEE_ERROR_GENERIC;

	TEE_MemFill(key1, 0, sizeof(key1));
	TEE_MemFill(key2, 0, sizeof(key2));

	/*
	 * Testing for successful calls to the PTA and that two calls with
	 * different input data do not generate the same output data (keys). We
	 * do that by using one key with and one key without extra data.
	 */
	res = derive_unique_key(session, key1, sizeof(key1), extra_key_data,
				sizeof(extra_key_data));
	if (res != TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	res = derive_unique_key(session, key2, sizeof(key2), NULL, 0);
	if (res != TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	/* They should not be equal */
	if (TEE_MemCompare(key1, key2, sizeof(key1)) == 0)
		res_final = TEE_ERROR_GENERIC;

	TEE_MemFill(key1, 0, sizeof(key1));
	TEE_MemFill(key2, 0, sizeof(key2));

	/*
	 * Testing limits for extra data size (if this would success, then we
	 * would overwrite the buffer extra_key_data also).
	 */
	res = derive_unique_key(session, key1, sizeof(key1), extra_key_data,
				TA_DERIVED_EXTRA_DATA_MAX_SIZE + 1);
	/* This shall fail */
	if (res == TEE_SUCCESS)
		res_final = TEE_ERROR_GENERIC;

	TEE_MemFill(key1, 0, sizeof(key1));

	/* Testing limits. */
	for (i = 0; i < sizeof(big_key); i++) {
		uint8_t *extra = NULL;
		uint8_t extra_size = 0;

		TEE_MemFill(big_key, 0, sizeof(big_key));

		/* Alternate between using and not using extra data. */
		if (i % 2) {
			extra = extra_key_data;
			extra_size = sizeof(extra_key_data);
		}

		res = derive_unique_key(session, big_key, i, extra, extra_size);
		if (i < TA_DERIVED_KEY_MIN_SIZE) {
			if (res != TEE_SUCCESS)
				continue;

			EMSG("Small key test iteration %zu failed", i);
			res_final = TEE_ERROR_GENERIC;
			break;
		}

		if (i > TA_DERIVED_KEY_MAX_SIZE) {
			if (res != TEE_SUCCESS)
				continue;

			EMSG("Big key test iteration %zu failed", i);
			res_final = TEE_ERROR_GENERIC;
			break;
		}

		if (res != TEE_SUCCESS) {
			res_final = TEE_ERROR_GENERIC;
			break;
		}
	}

	TEE_CloseTASession(session);

	return res_final;
}

TEE_Result derive_ta_unique_key_test_shm(uint32_t param_types,
					 TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_TASessionHandle session = TEE_HANDLE_NULL;
	uint32_t ret_origin = 0;

	if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
					   TEE_PARAM_TYPE_MEMREF_OUTPUT,
					   TEE_PARAM_TYPE_NONE,
					   TEE_PARAM_TYPE_NONE))
		return TEE_ERROR_BAD_PARAMETERS;

	res = TEE_OpenTASession(&system_uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
				&session, &ret_origin);

	if (res != TEE_SUCCESS)
		return res;

	/*
	 * Testing for unsuccessful calls to the PTA. They should be
	 * unsuccessful since we are using an out buffer coming from normal
	 * world.
	 */
	res = TEE_InvokeTACommand(session, TEE_TIMEOUT_INFINITE,
				  PTA_SYSTEM_DERIVE_TA_UNIQUE_KEY,
				  param_types, params, &ret_origin);
	TEE_CloseTASession(session);

	return res;
}
