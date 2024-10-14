/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#ifndef RKTEST_HANDLE_H
#define RKTEST_HANDLE_H

#define ASSERT_PARAM_TYPES(pt)\
do {\
	if ((pt) != param_types)\
		return TEE_ERROR_BAD_PARAMETERS;\
} while (0)

TEE_Result handle_crypto_sha(void);
TEE_Result handle_crypto_aes(void);
TEE_Result handle_crypto_rsa(void);
TEE_Result handle_otp_read(void);
TEE_Result handle_otp_write(void);
TEE_Result handle_otp_size(void);
TEE_Result handle_property(void);
TEE_Result handle_storage(void);
TEE_Result handle_transfer_data(uint32_t param_types, TEE_Param params[4]);
TEE_Result handle_storage_speed(uint32_t param_types, TEE_Param params[4]);
TEE_Result handle_trng_read(void);

#endif /*RKTEST_HANDLE_H*/
