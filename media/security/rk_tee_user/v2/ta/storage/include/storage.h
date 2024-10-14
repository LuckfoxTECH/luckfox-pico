/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <tee_api.h>

TEE_Result ta_storage_cmd_open(uint32_t command, uint32_t param_types,
						 TEE_Param params[4]);
TEE_Result ta_storage_cmd_create(uint32_t command, uint32_t param_types,
						   TEE_Param params[4]);
TEE_Result ta_storage_cmd_create_overwrite(uint32_t command,
					   uint32_t param_types,
					   TEE_Param params[4]);
TEE_Result ta_storage_cmd_close(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_read(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_write(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_seek(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_unlink(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_rename(uint32_t command, uint32_t param_types,
						   TEE_Param params[4]);
TEE_Result ta_storage_cmd_trunc(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_alloc_enum(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_free_enum(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_reset_enum(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_start_enum(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_next_enum(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_key_in_persistent(uint32_t param_types,
					    TEE_Param params[4]);
TEE_Result ta_storage_cmd_loop(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_restrict_usage(uint32_t param_types,
					 TEE_Param params[4]);
TEE_Result ta_storage_cmd_alloc_obj(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_free_obj(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_reset_obj(uint32_t param_types, TEE_Param params[4]);
TEE_Result ta_storage_cmd_get_obj_info(uint32_t param_types,
					    TEE_Param params[4]);

#endif /*STORAGE_H */
