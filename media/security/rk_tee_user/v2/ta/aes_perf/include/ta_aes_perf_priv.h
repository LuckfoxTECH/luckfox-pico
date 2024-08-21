/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef TA_EAS_PERF_PRIV_H
#define TA_EAS_PERF_PRIV_H

#include <tee_api.h>

TEE_Result cmd_prepare_key(uint32_t param_types, TEE_Param params[4]);
TEE_Result cmd_process(uint32_t param_types, TEE_Param params[4], bool sdp);
void cmd_clean_res(void);

#endif /* TA_EAS_PERF_PRIV_H */
