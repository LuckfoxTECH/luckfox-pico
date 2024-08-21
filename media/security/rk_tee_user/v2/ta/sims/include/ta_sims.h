/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef TA_SIMS_H
#define TA_SIMS_H

#include <ta_sims_test.h>
#include <tee_api.h>

TEE_Result sims_open_session(void **ctx);
TEE_Result sims_open_ta_session(void *ctx, uint32_t nParamTypes,
				TEE_Param pParams[4]);
void sims_close_session(void *ctx);
TEE_Result sims_read(uint32_t nParamTypes, TEE_Param pParams[4]);
TEE_Result sims_write(uint32_t nParamTypes, TEE_Param pParams[4]);
TEE_Result sims_get_counter(void *session_context, uint32_t param_types,
			    TEE_Param params[4]);
TEE_Result sims_entry_panic(void *session_context, uint32_t nParamTypes,
			    TEE_Param pParams[4]);

#endif
