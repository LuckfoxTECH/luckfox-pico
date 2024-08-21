/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#ifndef _RK_ANTI_COPY_H_
#define _RK_ANTI_COPY_H_

#include <tee_api_defines_extensions.h>
#include <tee_client_api.h>

#define OTP_UUID { 0x2d26d8a8, 0x5134, 0x4dd8, \
		{ 0xb3, 0x2f, 0xb3, 0x4b, 0xce, 0xeb, 0xc4, 0x71 } }

#define RK_CRYPTO_SERVICE_UUID	{ 0x0cacdb5d, 0x4fea, 0x466c, \
		{ 0x97, 0x16, 0x3d, 0x54, 0x16, 0x52, 0x83, 0x0f } }

typedef enum {
	LOW_GEN = 0,
	LOW_VERIFY,
	MID_GEN,
	MID_VERIFY,
	HIGH_GEN,
	HIGH_VERIFY,
	TEST_NULL,
} enum_func;

void dump_hex(const char *var_name, const uint8_t *data, uint32_t len);
int anti_copy(uint32_t invoke_command);
int anti_copy_low_gen(void);
int anti_copy_low_verify(void);
int anti_copy_mid_gen(void);
int anti_copy_mid_verify(void);
int anti_copy_high_gen(void);
int anti_copy_high_verify(void);
#endif /*_RK_ANTI_COPY_H_*/
