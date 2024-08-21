// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "rktest_handle.h"


TEE_Result handle_property(void)
{
	TEE_Result res = TEE_SUCCESS;
	TEE_PropSetHandle pr_handle;
	uint8_t name[64] = {0};
	uint8_t value[64] = {0};
	uint32_t name_len = 0;
	uint32_t value_len = 0;
	int count = 16;

	res = TEE_AllocatePropertyEnumerator(&pr_handle);
	if (res != TEE_SUCCESS) {
		EMSG("AllocatePropertyEnumerator ERR: 0x%x.", res);
		return res;
	}

	TEE_StartPropertyEnumerator(pr_handle, TEE_PROPSET_TEE_IMPLEMENTATION);

	while (count) {
		name_len = sizeof(name);
		value_len = sizeof(value);

		res = TEE_GetPropertyName(pr_handle, name, &name_len);
		if (res != TEE_SUCCESS) {
			EMSG("GetPropertyName ERR: 0x%x.", res);
			goto out;
		}

		res = TEE_GetPropertyAsString(pr_handle, (char *)name,
			(char *)value, &value_len);
		if (res != TEE_SUCCESS) {
			EMSG("GetPropertyAsString ERR: 0x%x.", res);
			goto out;
		}

		IMSG("GetProperty: %s -	%s\n", name, value);
		res = TEE_GetNextProperty(pr_handle);
		if (res == TEE_ERROR_ITEM_NOT_FOUND) {
			IMSG("The enumerator has reached the end of the property set");
			IMSG("Or if it has not started.");
			res = TEE_SUCCESS;
			goto out;
		} else if (res != TEE_SUCCESS) {
			EMSG("GetNextProperty ERR: 0x%x.", res);
			goto out;
		}

		count -= 1;
	}

out:
	TEE_FreePropertyEnumerator(pr_handle);
	return res;
}

