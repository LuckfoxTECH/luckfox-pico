/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef STORAGE_BENCHMARK_H
#define STORAGE_BENCHMARK_H

#include <tee_api.h>

TEE_Result ta_storage_benchmark_cmd_handler(uint32_t nCommandID,
		uint32_t param_types, TEE_Param params[4]);

#endif /*STORAGE_BENCHMARK_H */
