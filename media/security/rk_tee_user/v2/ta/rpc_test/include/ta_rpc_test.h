/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef TA_RPC_TEST_H
#define TA_RPC_TEST_H

/* This UUID is generated with the ITU-T UUID generator at
   http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_RPC_TEST_UUID { 0xd17f73a0, 0x36ef, 0x11e1,          \
	{ 0x98, 0x4a, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }

#define TA_RPC_CMD_CRYPT_SHA224			1
#define TA_RPC_CMD_CRYPT_SHA256			2
#define TA_RPC_CMD_CRYPT_AES256ECB_ENC		3
#define TA_RPC_CMD_CRYPT_AES256ECB_DEC		4
#define TA_RPC_CMD_OPEN				5
#define TA_RPC_CMD_CRYPT_PRIVMEM_SHA224		6
#define TA_RPC_CMD_CRYPT_PRIVMEM_SHA256		7
#define TA_RPC_CMD_CRYPT_PRIVMEM_AES256ECB_ENC	8
#define TA_RPC_CMD_CRYPT_PRIVMEM_AES256ECB_DEC	9

#endif
