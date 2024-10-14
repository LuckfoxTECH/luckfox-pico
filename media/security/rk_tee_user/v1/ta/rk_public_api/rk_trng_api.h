/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */

#ifndef RK_TRNG_API_H
#define RK_TRNG_API_H

#include <stdint.h>

TEE_Result rk_get_trng(uint8_t *buffer, uint32_t size);

#endif /* RK_TRNG_API_H */
