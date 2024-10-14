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
RK_RES rk_cipher_crypt(rk_handle handle, int in_fd, int out_fd, uint32_t len);
RK_RES rk_cipher_crypt_virt(rk_handle handle, const uint8_t *in, uint8_t *out, uint32_t len);
RK_RES rk_cipher_final(rk_handle handle);

RK_RES rk_ae_init(const rk_ae_config *config, rk_handle *handle);
RK_RES rk_ae_set_aad(rk_handle handle, int aad_fd);
RK_RES rk_ae_set_aad_virt(rk_handle handle, uint8_t *aad_virt);
RK_RES rk_ae_crypt(rk_handle handle, int in_fd, int out_fd, uint32_t len, uint8_t *tag);
RK_RES rk_ae_crypt_virt(rk_handle handle, const uint8_t *in, uint8_t *out, uint32_t len,
			uint8_t *tag);
RK_RES rk_ae_final(rk_handle handle);

RK_RES rk_hash_init(const rk_hash_config *config, rk_handle *handle);
RK_RES rk_hash_update(rk_handle handle, int data_fd, uint32_t data_len);
RK_RES rk_hash_update_virt(rk_handle handle, const uint8_t *data, uint32_t data_len);
RK_RES rk_hash_final(rk_handle handle, uint8_t *hash);

RK_RES rk_rsa_pub_encrypt(const rk_rsa_pub_key_pack *pub, enum RK_RSA_CRYPT_PADDING padding,
			  const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len);
RK_RES rk_rsa_priv_decrypt(const rk_rsa_priv_key_pack *priv, enum RK_RSA_CRYPT_PADDING padding,
			   const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len);
RK_RES rk_rsa_priv_encrypt(const rk_rsa_priv_key_pack *priv, enum RK_RSA_CRYPT_PADDING padding,
			   const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len);
RK_RES rk_rsa_pub_decrypt(const rk_rsa_pub_key_pack *pub, enum RK_RSA_CRYPT_PADDING padding,
			  const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len);
RK_RES rk_rsa_sign(const rk_rsa_priv_key_pack *priv, enum RK_RSA_SIGN_PADDING padding,
		   const uint8_t *in, uint32_t in_len, const uint8_t *hash,
		   uint8_t *out, uint32_t *out_len);
RK_RES rk_rsa_verify(const rk_rsa_pub_key_pack *pub, enum RK_RSA_SIGN_PADDING padding,
		     const uint8_t *in, uint32_t in_len, const uint8_t *hash,
		     uint8_t *sign, uint32_t sign_len);

#endif
