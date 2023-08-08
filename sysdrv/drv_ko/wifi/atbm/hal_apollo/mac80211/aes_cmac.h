/*
 * Copyright 2008, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef AES_CMAC_H
#define AES_CMAC_H
#ifdef CONFIG_ATBM_USE_SW_ENC

#include <linux/crypto.h>
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
#include <crypto/algapi.h>
#include <crypto/internal/cipher.h>
#include <crypto/internal/skcipher.h>
#endif
struct crypto_cipher * ieee80211_aes_cmac_key_setup(const u8 key[]);
void ieee80211_aes_cmac(struct crypto_cipher *tfm, const u8 *aad,
			const u8 *data, size_t data_len, u8 *mic);
void ieee80211_aes_cmac_key_free(struct crypto_cipher *tfm);
#endif
#endif /* AES_CMAC_H */
