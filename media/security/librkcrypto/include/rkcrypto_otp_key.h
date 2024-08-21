/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _RKCRYPTO_OTP_KEY_H_
#define _RKCRYPTO_OTP_KEY_H_

#include "rkcrypto_common.h"

RK_RES rk_write_oem_otp_key(enum RK_OEM_OTP_KEYID key_id, uint8_t *key, uint32_t key_len);
RK_RES rk_oem_otp_key_is_written(enum RK_OEM_OTP_KEYID key_id, uint8_t *is_written);
RK_RES rk_set_oem_hr_otp_read_lock(enum RK_OEM_OTP_KEYID key_id);
RK_RES rk_oem_otp_key_cipher_virt(enum RK_OEM_OTP_KEYID key_id, rk_cipher_config *config,
				  uint8_t *src, uint8_t *dst, uint32_t len);
RK_RES rk_oem_otp_key_cipher(enum RK_OEM_OTP_KEYID key_id, rk_cipher_config *config,
			     int32_t in_fd, int32_t out_fd, uint32_t len);

#endif /* _RKCRYPTO_OTP_KEY_H_ */
