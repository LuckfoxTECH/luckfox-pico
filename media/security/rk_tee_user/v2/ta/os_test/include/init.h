/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef INIT_H
#define INIT_H

#include <tee_api.h>

TEE_Result ta_entry_init(uint32_t param_types, TEE_Param params[4]);

#endif
