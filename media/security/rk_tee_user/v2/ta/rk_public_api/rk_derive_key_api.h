/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */

#ifndef RK_DERIVE_KEY_H
#define RK_DERIVE_KEY_H

#include <stdint.h>

TEE_Result rk_derive_ta_unique_key(uint8_t *extra, uint16_t extra_size,
				    uint8_t *key, uint16_t key_size);

#endif /* RK_DERIVE_KEY_H */
