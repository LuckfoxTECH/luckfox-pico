/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef TA_SIMS_TEST_H
#define TA_SIMS_TEST_H

/* This UUID is generated with uuidgen */
#define TA_SIMS_TEST_UUID { 0xe6a33ed4, 0x562b, 0x463a, \
	{ 0xbb, 0x7e, 0xff, 0x5e, 0x15, 0xa4, 0x93, 0xc8 } }

#define TA_SIMS_CMD_READ                 1
#define TA_SIMS_CMD_WRITE                2
#define TA_SIMS_CMD_GET_COUNTER          3
#define TA_SIMS_CMD_PANIC                4
#define TA_SIMS_OPEN_TA_SESSION          5

#endif
