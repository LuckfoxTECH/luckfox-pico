/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */

#ifndef RK_CRYPTO_API_H
#define RK_CRYPTO_API_H

#include <stdint.h>

#define DES_BLOCK_SIZE		8
#define AES_BLOCK_SIZE		16
#define SHA1_HASH_SIZE		20
#define SHA224_HASH_SIZE	28
#define SHA256_HASH_SIZE	32
#define SHA384_HASH_SIZE	48
#define SHA512_HASH_SIZE	64
#define MD5_HASH_SIZE		16
#define AES_AE_DATA_BLOCK	128
#define MAX_HASH_BLOCK_SIZE	128 /* SHA-512 is the longest so far */

typedef enum {
	PKCS7_PADDING = 0,
	NO_PADDING  = 1,
} rk_padding_t;

typedef enum {
	SIGN_DIGEST = 0,
	SIGN_DATA  = 1,
} rk_sign_type_t;

typedef enum {
	RK_MAC_SIGN = 3,
	RK_MAC_VERIFY = 13,
} rk_mac_mode_t;

typedef enum {
	RK_AE_UPDATE_DATA = 0,
	RK_AE_UPDATE_AAD = 1,
} rk_ae_update_type_t;

typedef struct crypto_context {
	TEE_OperationHandle op;
	TEE_ObjectHandle obj;
	TEE_ObjectHandle obj_2; //for those one operation with two keys, such as aes-xts
	TEE_OperationMode mode;
	uint32_t algo;
	uint32_t padding;
	uint32_t sign_mode;
	uint8_t buffer[16];
	uint32_t buffer_offs;
} crypto_ctx_t;

/*
 * ecc key max size is 521 bits, so 66 bytes is enough.
 * set buffer to 68 bytes for 4 byte alignment.
 */
typedef struct EC_KEY {
	uint32_t d_len;
	uint32_t x_len;
	uint32_t y_len;
	uint32_t curve;
	uint32_t key_len;
	uint8_t d[68];
	uint8_t x[68];
	uint8_t y[68];
} ec_key_t;

typedef struct EC_PUB_KEY {
	uint32_t x_len;
	uint32_t y_len;
	uint32_t curve;
	uint32_t key_len;
	uint8_t x[68];
	uint8_t y[68];
} ec_pub_key_t;

typedef struct RSA_KEY {
	uint16_t key_len;
	uint16_t d_len;
	uint16_t e_len;
	uint16_t p_len;
	uint16_t q_len;
	uint16_t dp_len;
	uint16_t dq_len;
	uint16_t iq_len;
	uint8_t n[512];
	uint8_t e[8];
	uint8_t d[512];
	uint8_t p[256];
	uint8_t q[256];
	uint8_t dp[256];
	uint8_t dq[256];
	uint8_t iq[256];
} rsa_key_t;


crypto_ctx_t *rk_crypto_malloc_ctx(void);
void rk_crypto_free_ctx(crypto_ctx_t **ctx);

TEE_Result rk_hash_crypto(uint8_t *in, uint8_t *out, uint32_t in_len,
			  uint32_t out_len, uint32_t algo);
TEE_Result rk_hash_begin(crypto_ctx_t *ctx, uint32_t algo);
TEE_Result rk_hash_update(crypto_ctx_t *ctx, uint8_t *in, uint32_t in_len);
TEE_Result rk_hash_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			  uint32_t in_len, uint32_t *out_len);

TEE_Result rk_cipher_crypto(uint8_t *in, uint8_t *out, uint32_t len,
			    uint8_t *key, uint32_t key_len, uint8_t *iv,
			    uint32_t algo, TEE_OperationMode mode);
TEE_Result rk_set_padding(crypto_ctx_t *ctx, int padding);
TEE_Result rk_cipher_begin(crypto_ctx_t *ctx, uint8_t *key, uint32_t key_len,
			   uint8_t *iv, uint32_t algo, TEE_OperationMode mode);
TEE_Result rk_cipher_update(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			    uint32_t in_len, uint32_t *out_len);
TEE_Result rk_cipher_finish(crypto_ctx_t *ctx, uint8_t *out, uint32_t *out_len);

TEE_Result rk_ae_begin(crypto_ctx_t *ctx, uint8_t *key, uint32_t key_len,
		       uint8_t *iv, uint32_t iv_len, uint32_t add_len, uint32_t tag_len,
		       uint32_t payload_len, uint32_t algo, TEE_OperationMode mode);
TEE_Result rk_ae_update(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			uint32_t in_len, uint32_t *out_len, rk_ae_update_type_t is_add);
TEE_Result rk_ae_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			uint8_t *tag, uint32_t in_len, uint32_t *out_len, uint32_t *tag_len);

TEE_Result rk_gen_rsa_key(rsa_key_t *rsa_key, uint32_t key_len,
			  uint64_t public_exponent);
TEE_Result rk_rsa_crypto(uint8_t *in, uint8_t *out, uint32_t len,
			 rsa_key_t *key, uint32_t algo, TEE_OperationMode mode);
TEE_Result rk_rsa_sign(uint8_t *digest, uint8_t *signature, uint32_t digest_len,
		       uint32_t *signature_len, rsa_key_t *key, uint32_t salt_len,
		       uint32_t algo, TEE_OperationMode mode);
TEE_Result rk_set_sign_mode(crypto_ctx_t *ctx, unsigned int mode);
TEE_Result rk_rsa_begin(crypto_ctx_t *ctx, rsa_key_t *key, uint32_t algo,
			TEE_OperationMode mode);
TEE_Result rk_rsa_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			 uint32_t in_len,
			 uint32_t *out_len, uint32_t salt_len);

TEE_Result rk_gen_ec_key(ec_key_t *ec_key, uint32_t key_len, uint32_t curve);
TEE_Result rk_ecdh_genkey(uint8_t *private, uint8_t *publicx, uint32_t *publicy,
			  uint32_t algo,  uint32_t curve, uint32_t keysize, uint8_t *out);
TEE_Result rk_ecdsa_sign(uint8_t *digest, uint8_t *signature,
			 uint32_t digest_len, uint32_t *signature_len,
			 ec_key_t *key, uint32_t algo, TEE_OperationMode mode);
TEE_Result rk_ecdsa_begin(crypto_ctx_t *ctx, ec_key_t *key, uint32_t algo,
			  TEE_OperationMode mode);
TEE_Result rk_ecdsa_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *out,
			   uint32_t in_len, uint32_t *out_len);

TEE_Result rk_mac_crypto(uint8_t *in, uint8_t *out, uint32_t in_len,
			 uint32_t *out_len, uint8_t *key, uint32_t key_len,
			 uint8_t *iv, uint32_t algo);
TEE_Result rk_mac_begin(crypto_ctx_t *ctx, uint8_t *key, uint32_t key_len,
			uint8_t *iv, uint32_t algo);
TEE_Result rk_mac_update(crypto_ctx_t *ctx, uint8_t *in, uint32_t in_len);
TEE_Result rk_mac_finish(crypto_ctx_t *ctx, uint8_t *in, uint8_t *mac,
			 uint32_t in_len, uint32_t *mac_len, rk_mac_mode_t mode);

TEE_Result rk_hkdf_genkey(uint8_t *ikm, uint32_t ikm_len,
			  uint8_t *salt, uint32_t salt_len, uint32_t *info, uint32_t info_len,
			  uint32_t algo, uint32_t okm_len, uint8_t *okm);
TEE_Result rk_pkcs5_pbkdf2_hmac(uint8_t *password, uint32_t password_len,
				uint8_t *salt, uint32_t salt_len, uint32_t iterations,
				uint32_t algo, uint32_t key_len, uint8_t *out_key);

#endif /* RK_CRYPTO_API_H */
