/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 */

#ifndef TA_SDP_BASIC_H
#define TA_SDP_BASIC_H

#define TA_SDP_BASIC_UUID { 0x12345678, 0x5b69, 0x11e4, \
	{ 0x9d, 0xbb, 0x10, 0x1f, 0x74, 0xf0, 0x00, 0x99 } }

#define TA_SDP_BASIC_CMD_INJECT			0
#define TA_SDP_BASIC_CMD_TRANSFORM		1
#define TA_SDP_BASIC_CMD_DUMP			2

#define TA_SDP_BASIC_CMD_INVOKE_INJECT		3
#define TA_SDP_BASIC_CMD_INVOKE_TRANSFORM	4
#define TA_SDP_BASIC_CMD_INVOKE_DUMP		5

#define TA_SDP_BASIC_CMD_PTA_INJECT		6
#define TA_SDP_BASIC_CMD_PTA_TRANSFORM		7
#define TA_SDP_BASIC_CMD_PTA_DUMP		8

#endif /* TA_SDP_BASIC_H */
