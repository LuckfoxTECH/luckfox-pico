/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include "ta_TCF_test.h"

#define TA_FLAGS		(TA_FLAG_USER_MODE \
				| TA_FLAG_EXEC_DDR \
				| TA_FLAG_SINGLE_INSTANCE \
				)
#define TA_STACK_SIZE		(2*1024)
#define TA_DATA_SIZE		(32*1024)

#define TA_CURRENT_TA_EXT_PROPERTIES \
	{ "gp.test.description", USER_TA_PROP_TYPE_STRING, \
	"TTA_TCF v1.9" }, \
	{ "smc.ta.teststring", USER_TA_PROP_TYPE_STRING, \
	"this is a test string" }, \
	{ "smc.ta.testbooltrue", USER_TA_PROP_TYPE_BOOL, \
	&(const bool){ true } }, \
	{ "smc.ta.testboolfalse", USER_TA_PROP_TYPE_BOOL, \
	&(const bool){ false } }, \
	{ "smc.ta.testu32", USER_TA_PROP_TYPE_U32, \
	&(const uint32_t){ 48059 } }, \
	{ "smc.ta.testbinaryblock", USER_TA_PROP_TYPE_BINARY_BLOCK, \
	"VGhpcyBpcyBhIHRleHQgYmluYXJ5IGJsb2Nr" }, \
	{ "smc.ta.testuuid", USER_TA_PROP_TYPE_UUID, \
	&(const TEE_UUID) { 0x534D4152, 0x542D, 0x4353, \
		{ 0x4C, 0x54, 0x2D, 0x54, 0x41, 0x2D, 0x30, 0x31 } } }, \
	{ "smc.ta.testidentity", USER_TA_PROP_TYPE_IDENTITY, \
	&(const TEE_Identity) { 0xF0000000, \
		{ 0x534D4152, 0x542D, 0x4353, \
		{ 0x4C, 0x54, 0x2D, 0x54, 0x41, 0x2D, 0x30, 0x31 } } } }

#endif /* USER_TA_HEADER_DEFINES_H */
