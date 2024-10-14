/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#ifndef RKTEST_TA_H
#define RKTEST_TA_H

/* This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */
#define RKTEST_TA_UUID { 0x1db57234, 0xdacd, 0x462d, \
		{ 0x9b, 0xb1, 0xae, 0x79, 0xde, 0x44, 0xe2, 0xa5} }


/* The TAFs ID implemented in this TA */
#define RKTEST_TA_CMD_TRANSFER_DATA			102
#define RKTEST_TA_CMD_STORAGE				103
#define RKTEST_TA_CMD_PROPERTY				104
#define RKTEST_TA_CMD_CRYPTO_SHA			105
#define RKTEST_TA_CMD_CRYPTO_AES			106
#define RKTEST_TA_CMD_CRYPTO_RSA			107
#define RKTEST_TA_CMD_OEM_OTP_READ			108
#define RKTEST_TA_CMD_OEM_OTP_WRITE			109
#define RKTEST_TA_CMD_STORAGE_SPEED			110
#define RKTEST_TA_CMD_OEM_OTP_SIZE			111
#define RKTEST_TA_CMD_TRNG_READ				112

#endif /*RKTEST_TA_H*/
