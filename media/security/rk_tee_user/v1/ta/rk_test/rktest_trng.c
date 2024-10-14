// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include "rktest_handle.h"
#include "../rk_public_api/rk_trng_api.h"


static void my_dump_hex(const char *name, const uint8_t *array, uint32_t len)
{
	uint32_t i;
	char buffer[256];
	char *p;

	IMSG("=================== %s [%u] ================", name, len);

	p = buffer;
	for (i = 0; i < len; i++) {
		if (i % 16 == 0 && i != 0) {
			IMSG("%s", buffer);
			p = buffer;
			TEE_MemFill(buffer, 0x00, sizeof(buffer));
		}
		p += snprintf(p, 256, "%02x ", array[i]);
	}
	IMSG("%s\n", buffer);
}

TEE_Result handle_trng_read(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t read_len = 256;
	uint8_t *read_data;

	read_data = TEE_Malloc(read_len, 0);
	if (!read_data) {
		EMSG("Malloc read_len buffer FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	res = rk_get_trng(read_data, read_len);
	if (res != TEE_SUCCESS)
		EMSG("rk_get_trng failed with code 0x%x", res);
	else
		my_dump_hex("rk_get_trng success with data:", read_data, read_len);

	TEE_Free(read_data);
	return res;
}

