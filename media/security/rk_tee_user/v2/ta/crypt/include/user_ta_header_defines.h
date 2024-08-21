/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <ta_crypt.h>

#define TA_UUID TA_CRYPT_UUID

#define TA_FLAGS		(TA_FLAG_USER_MODE | TA_FLAG_EXEC_DDR)
#define TA_STACK_SIZE		(32 * 1024)
#define TA_DATA_SIZE		(32 * 1024)

#endif /* USER_TA_HEADER_DEFINES_H */
