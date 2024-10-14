/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef TA_OS_TEST_H
#define TA_OS_TEST_H

/* This UUID is generated with the ITU-T UUID generator at
   http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_OS_TEST_UUID { 0x5b9e0e40, 0x2636, 0x11e1, \
	{ 0xad, 0x9e, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }

#define TA_OS_TEST_CMD_INIT                 0
#define TA_OS_TEST_CMD_CLIENT_WITH_TIMEOUT  1
#define TA_OS_TEST_CMD_BASIC                5
#define TA_OS_TEST_CMD_PANIC                6
#define TA_OS_TEST_CMD_CLIENT               7
#define TA_OS_TEST_CMD_PARAMS_ACCESS        8
#define TA_OS_TEST_CMD_WAIT                 9
#define TA_OS_TEST_CMD_BAD_MEM_ACCESS       10
#define TA_OS_TEST_CMD_TA2TA_MEMREF         11
#define TA_OS_TEST_CMD_TA2TA_MEMREF_MIX     12
#define TA_OS_TEST_CMD_PARAMS               13
#define TA_OS_TEST_CMD_CALL_LIB             14
#define TA_OS_TEST_CMD_CALL_LIB_PANIC       15
#define TA_OS_TEST_CMD_CALL_LIB_DL          16
#define TA_OS_TEST_CMD_CALL_LIB_DL_PANIC    17
#define TA_OS_TEST_CMD_GET_GLOBAL_VAR       18
#define TA_OS_TEST_CMD_NULL_MEMREF_PARAMS   19
#define TA_OS_TEST_CMD_CLIENT_IDENTITY      20
#define TA_OS_TEST_CMD_TLS_TEST_MAIN        21
#define TA_OS_TEST_CMD_TLS_TEST_SHLIB       22
#define TA_OS_TEST_CMD_DL_PHDR              23
#define TA_OS_TEST_CMD_DL_PHDR_DL           24
#define TA_OS_TEST_CMD_CXX_CTOR_MAIN        25
#define TA_OS_TEST_CMD_CXX_CTOR_SHLIB       26
#define TA_OS_TEST_CMD_CXX_CTOR_SHLIB_DL    27
#define TA_OS_TEST_CMD_CXX_EXC_MAIN         28
#define TA_OS_TEST_CMD_CXX_EXC_MIXED        29

#endif /*TA_OS_TEST_H */
