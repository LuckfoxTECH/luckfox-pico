/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#ifndef _RKTEST_H_
#define _RKTEST_H_

#include <tee_client_api.h>
#include <tee_api_defines_extensions.h>

#define DISABLE		0
#define ENABLE		1

typedef enum {
	TRANSFER_DATA = 0,
	STORAGE,
	STORAGE_SPEED,
	PROPERTY,
	CRYPTO_SHA,
	CRYPTO_AES,
	CRYPTO_RSA,
	SECSTOR_TA,
	OTP_READ,
	OTP_WRITE,
	OTP_SIZE,
	OTP_NS_READ,
	OTP_NS_WRITE,
	TRNG_READ,
	SOCKET,
	TEST_NULL,
	CRYPTO_HW,
	DERIVE_KEY,
} enum_func;

TEEC_Result rk_test(uint32_t invoke_command);

#endif /*_RKTEST_H_*/
