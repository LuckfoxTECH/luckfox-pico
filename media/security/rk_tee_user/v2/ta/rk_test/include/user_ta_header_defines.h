/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <rktest_ta.h> /* To get the DEMO_TA_UUID define */

#define TA_UUID RKTEST_TA_UUID

#define TA_FLAGS	(TA_FLAG_USER_MODE | TA_FLAG_EXEC_DDR | TA_FLAG_SINGLE_INSTANCE)
#define TA_STACK_SIZE               (2 * 1024)
#define TA_DATA_SIZE                (256 * 1024)

#endif /*USER_TA_HEADER_DEFINES_H*/
