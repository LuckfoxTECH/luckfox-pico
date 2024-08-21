/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2019, Linaro Limited
 */

#ifndef DERIVE_KEY_TAF_H
#define DERIVE_KEY_TAF_H

#include <tee_internal_api.h>

TEE_Result derive_ta_unique_key_test(uint32_t param_types,
				     TEE_Param params[4]);
TEE_Result derive_ta_unique_key_test_shm(uint32_t param_types,
					 TEE_Param params[4]);
#endif
