/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#ifndef __RKCRYPTO_CORE_H__
#define __RKCRYPTO_CORE_H__

#include <stdint.h>
#include "rkcrypto_common.h"

RK_RES rk_crypto_init(void);
void rk_crypto_deinit(void);
RK_RES rk_cipher_init(const rk_cipher_config *config, rk_handle *handle);
RK_RES rk_cipher_crypt(rk_handle handle, int in_fd, uint32_t in_len,
		       int out_fd, uint32_t *out_len);
RK_RES rk_cipher_crypt_virt(rk_handle handle, const uint8_t *in, uint32_t in_len,
			    uint8_t *out, uint32_t *out_len);
RK_RES rk_cipher_final(rk_handle handle);

RK_RES rk_hash_init(const rk_hash_config *config, rk_handle *handle);
RK_RES rk_hash_update(rk_handle handle, int data_fd, uint32_t data_len, bool is_last);
RK_RES rk_hash_update_virt(rk_handle handle, const uint8_t *data, uint32_t data_len, bool is_last);
RK_RES rk_hash_final(rk_handle handle, uint8_t *hash, uint32_t *hash_len);

#endif
