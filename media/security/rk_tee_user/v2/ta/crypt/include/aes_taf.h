/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef AES_TAF_H
#define AES_TAF_H

#include <tee_api.h>

/* params[0] is input buffer and params[1] is output buffer */
TEE_Result ta_entry_aes256ecb_encrypt(uint32_t param_types,
				      TEE_Param params[4]);

/* params[0] is input buffer and params[1] is output buffer */
TEE_Result ta_entry_aes256ecb_decrypt(uint32_t param_types,
				      TEE_Param params[4]);

#endif
