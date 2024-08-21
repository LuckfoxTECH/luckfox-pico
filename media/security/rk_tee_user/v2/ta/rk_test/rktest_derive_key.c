// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <string.h>
#include <tee_internal_api.h>
#include "rktest_handle.h"
#include "../rk_public_api/rk_derive_key_api.h"

TEE_Result handle_derive_key(void)
{
	TEE_Result res = TEE_SUCCESS;
	const char *data="data string for deriver ta unique key";
	uint8_t ta_key[32];

	memset(ta_key, 0, sizeof(ta_key));
	res = rk_derive_ta_unique_key((uint8_t *)data, strlen(data),
				ta_key, sizeof(ta_key));
	if (res) {
		EMSG("rk_derive_ta_unique_key fail!");
	} else {
		EMSG("rk_derive_ta_unique_key success!");
		ta_dump_hex("ta unique key", ta_key, sizeof(ta_key));
	}

	return res;
}
