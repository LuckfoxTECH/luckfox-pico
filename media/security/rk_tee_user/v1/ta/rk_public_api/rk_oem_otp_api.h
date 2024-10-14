/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */

#ifndef RK_OEM_OTP_H
#define RK_OEM_OTP_H

#include <stdint.h>

TEE_Result rk_otp_read(uint32_t offset, uint8_t *data, uint32_t len);
TEE_Result rk_otp_write(uint32_t offset, uint8_t *data, uint32_t len);
TEE_Result rk_otp_size(uint32_t *otp_size);

#endif /* RK_OEM_OTP_H */
