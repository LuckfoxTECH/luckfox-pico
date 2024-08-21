/*
 * Copyright (c) 2019, Mentor Graphics Corporation
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <ta_sims_keepalive_test.h>

#define TA_NAME		"SIMS_KEEPALIVE"
#define TA_UUID		TA_SIMS_KEEP_ALIVE_TEST_UUID

#define TA_FLAGS		(TA_FLAG_SINGLE_INSTANCE | \
				TA_FLAG_MULTI_SESSION | \
				TA_FLAG_INSTANCE_KEEP_ALIVE)
#define TA_STACK_SIZE		(4 * 1024)
#define TA_DATA_SIZE		(64 * 1024)

#endif /* USER_TA_HEADER_DEFINES_H */
