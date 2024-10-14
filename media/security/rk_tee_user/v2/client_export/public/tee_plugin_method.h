/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2020, Open Mobile Platform LLC
 */

#ifndef TEE_PLUGIN_METHOD_H
#define TEE_PLUGIN_METHOD_H

#include <stddef.h>
#include <tee_client_api.h>

struct plugin_method {
	const char *name; /* short friendly name of the plugin */
	TEEC_UUID uuid;
	TEEC_Result (*init)(void);
	TEEC_Result (*invoke)(unsigned int cmd, unsigned int sub_cmd,
			      void *data, size_t in_len, size_t *out_len);
};

#endif /* TEE_PLUGIN_METHOD_H */
