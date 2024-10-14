/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#ifndef XTEST_UUID_HELPERS_H
#define XTEST_UUID_HELPERS_H

#include <tee_api_types.h>

/*
 * Convert a UUID string @s into a TEEC_UUID @uuid
 * Expected format for @s is: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * 'x' being any hexadecimal digit (0-9a-fA-F)
 */
TEEC_Result xtest_uuid_from_str(TEEC_UUID *uuid, const char *s);

#ifdef OPENSSL_FOUND
/*
 * Form UUIDv5 from given name space and name.
 */
TEEC_Result xtest_uuid_v5(TEEC_UUID *uuid, const TEEC_UUID *ns,
			  const void *name, size_t size);
#endif /*OPENSSL_FOUND*/
#endif
