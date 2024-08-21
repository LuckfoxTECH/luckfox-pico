//SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Open Mobile Platform LLC
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ta_supp_plugin.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <test_supp_plugin.h>
#include <trace.h>

static const TEE_UUID uuid = TEST_PLUGIN_UUID;

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes __unused,
				    TEE_Param pParams[4] __unused,
				    void **ppSessionContext __unused)
{
	/* check the plugin was loaded */
	return tee_invoke_supp_plugin(&uuid, TEST_PLUGIN_CMD_PING, 0, NULL, 0,
				      NULL);
}

void TA_CloseSessionEntryPoint(void *pSessionContext __unused)
{
}

static TEE_Result pass_values(uint32_t param_types,
			     TEE_Param params[TEE_NUM_PARAMS])
{
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
					  TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE);
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t outlen = 0;
	struct add_op {
		uint32_t a;
		uint32_t b;
	} values = { 0 };

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	values.a = params[0].value.a;
	values.b = params[0].value.b;

	res = tee_invoke_supp_plugin(&uuid, TEST_PLUGIN_CMD_PASS_VALUES,
				     params[1].value.a, (void *)&values,
				     sizeof(struct add_op), &outlen);
	params[0].value.a = values.a;

	return res;
}

static TEE_Result write_array(uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS])
{
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE);
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t outlen = 0;
	uint8_t *d = params[0].memref.buffer;

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	res = tee_invoke_supp_plugin(&uuid, TEST_PLUGIN_CMD_WRITE_ARR, 0,
				     params[0].memref.buffer,
				     params[0].memref.size, &outlen);
	params[1].value.a = d[0];

	return res;
}

static TEE_Result get_array(uint32_t param_types,
			    TEE_Param params[TEE_NUM_PARAMS])
{
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE);
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t outlen = 0;

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	res = tee_invoke_supp_plugin(&uuid, TEST_PLUGIN_CMD_GET_ARR, 0,
				     params[0].memref.buffer,
				     params[0].memref.size,
				     &outlen);
	params[1].value.a = (uint32_t)outlen;

	return res;
}

static TEE_Result bad_input_uuid(uint32_t param_types __unused,
				 TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	char data[32] = {  };
	size_t inlen = sizeof(data);

	return tee_invoke_supp_plugin(NULL, 0, 0, data, inlen, NULL);
}

static TEE_Result bad_input_data(uint32_t param_types __unused,
				 TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	return tee_invoke_supp_plugin(&uuid, 0, 0, NULL, 10, NULL);
}

static TEE_Result bad_input_inlen(uint32_t param_types __unused,
				  TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	char data[32] = {  };

	return tee_invoke_supp_plugin(&uuid, 0, 0, data, 0, NULL);
}

static TEE_Result call_unknown_plugin(uint32_t param_types __unused,
				      TEE_Param params[TEE_NUM_PARAMS]
				      __unused)
{
	const TEE_UUID nulluuid = {  };
	char data[32] = {  };
	size_t inlen = sizeof(data);

	return tee_invoke_supp_plugin(&nulluuid, 0, 0, data, inlen, NULL);
}

TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext __unused,
				      uint32_t nCommandID,
				      uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
	switch (nCommandID) {
	case TA_SUPP_PLUGIN_CMD_PASS_VALUES:
		return pass_values(nParamTypes, pParams);
	case TA_SUPP_PLUGIN_CMD_WRITE_ARR:
		return write_array(nParamTypes, pParams);
	case TA_SUPP_PLUGIN_CMD_GET_ARR:
		return get_array(nParamTypes, pParams);
	case TA_SUPP_PLUGIN_CMD_BAD_UUID:
		return bad_input_uuid(nParamTypes, pParams);
	case TA_SUPP_PLUGIN_CMD_BAD_IN_DATA:
		return bad_input_data(nParamTypes, pParams);
	case TA_SUPP_PLUGIN_CMD_BAD_IN_LEN:
		return bad_input_inlen(nParamTypes, pParams);
	case TA_SUPP_PLUGIN_CMD_UNKNOWN_UUID:
		return call_unknown_plugin(nParamTypes, pParams);
	default:
		return TEE_ERROR_NOT_SUPPORTED;
	}
}
