/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <ta_sims.h>

#define TA_UUID TA_SIMS_TEST_UUID

#define TA_FLAGS		(TA_FLAG_USER_MODE | TA_FLAG_EXEC_DDR | \
				TA_FLAG_SINGLE_INSTANCE | \
				TA_FLAG_MULTI_SESSION)
#define TA_STACK_SIZE		(4 * 1024)
#define TA_DATA_SIZE		(64 * 1024)

#endif /* USER_TA_HEADER_DEFINES_H */
