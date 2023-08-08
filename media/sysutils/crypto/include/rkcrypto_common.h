/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _RKCRYPTO_COMMON_H_
#define _RKCRYPTO_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t RK_RES;
typedef uint32_t rk_handle;

#define STR_HELPER(x)	#x
#define STR(x)		STR_HELPER(x)

/* RK_CRYPTO api verison */
#define RK_CRYPTO_API_MAJOR_VERSION	1
#define RK_CRYPTO_API_MINOR_VERSION	0
#define RK_CRYPTO_API_REVISION_VERSION	0
#define RK_CRYPTO_API_BUILD_VERSION	0

#define RK_CRYPTO_API_VERSION STR(RK_CRYPTO_API_MAJOR_VERSION) "." \
			      STR(RK_CRYPTO_API_MINOR_VERSION) "." \
			      STR(RK_CRYPTO_API_REVISION_VERSION) "_[" \
			      STR(RK_CRYPTO_API_BUILD_VERSION) "]"

#define RK_CRYPTO_API_FULL_VERSION "rkcrypto api version " RK_CRYPTO_API_VERSION

/* API return codes */
#define RK_CRYPTO_SUCCESS		0x00000000
#define RK_CRYPTO_ERR_GENERIC		0xF0000000
#define RK_CRYPTO_ERR_PARAMETER		0xF0000001
#define RK_CRYPTO_ERR_STATE		0xF0000002
#define RK_CRYPTO_ERR_NOT_SUPPORTED	0xF0000003
#define RK_CRYPTO_ERR_OUT_OF_MEMORY	0xF0000004
#define RK_CRYPTO_ERR_ACCESS_DENIED	0xF0000005

/* Algorithm operation */
#define RK_MODE_ENCRYPT			1
#define RK_MODE_DECRYPT			0
#define RK_OP_CIPHER_ENC		RK_MODE_ENCRYPT
#define RK_OP_CIPHER_DEC		RK_MODE_DECRYPT

/* Algorithm block length */
#define DES_BLOCK_SIZE			8
#define AES_BLOCK_SIZE			16
#define SM4_BLOCK_SIZE			16
#define SHA1_HASH_SIZE			20
#define SHA224_HASH_SIZE		28
#define SHA256_HASH_SIZE		32
#define SHA384_HASH_SIZE		48
#define SHA512_HASH_SIZE		64
#define SHA512_224_HASH_SIZE		28
#define SHA512_256_HASH_SIZE		32
#define MD5_HASH_SIZE			16
#define SM3_HASH_SIZE			32

#define SM3_BLOCK_SIZE			64
#define SHA1_BLOCK_SIZE			64
#define MD5_BLOCK_SIZE			64
#define SHA224_BLOCK_SIZE		64
#define SHA256_BLOCK_SIZE		64
#define SHA384_BLOCK_SIZE		128
#define SHA512_BLOCK_SIZE		128
#define SHA512_224_BLOCK_SIZE		128
#define SHA512_256_BLOCK_SIZE		128

#define AES_AE_DATA_BLOCK		128
#define MAX_HASH_BLOCK_SIZE		128
#define MAX_TDES_KEY_SIZE		24
#define MAX_AES_KEY_SIZE		32

#define RK_CRYPTO_MAX_DATA_LEN		(1 * 1024 * 1024)

/* Crypto algorithm */
enum RK_CRYPTO_ALGO {
	RK_ALGO_CIPHER_TOP = 0x00,
	RK_ALGO_AES,
	RK_ALGO_DES,
	RK_ALGO_TDES,
	RK_ALGO_SM4,
	RK_ALGO_CIPHER_BUTT,

	RK_ALGO_HASH_TOP = 0x10,
	RK_ALGO_MD5,
	RK_ALGO_SHA1,
	RK_ALGO_SHA256,
	RK_ALGO_SHA224,
	RK_ALGO_SHA512,
	RK_ALGO_SHA384,
	RK_ALGO_SHA512_224,
	RK_ALGO_SHA512_256,
	RK_ALGO_SM3,
	RK_ALGO_HASH_BUTT,

	RK_ALGO_HMAC_TOP = 0x20,
	RK_ALGO_HMAC_MD5,
	RK_ALGO_HMAC_SHA1,
	RK_ALGO_HMAC_SHA256,
	RK_ALGO_HMAC_SHA512,
	RK_ALGO_HMAC_SM3,
	RK_ALGO_HMAC_BUTT,
};

/* Crypto mode */
enum RK_CIPIHER_MODE {
	RK_CIPHER_MODE_ECB = 0,
	RK_CIPHER_MODE_CBC,
	RK_CIPHER_MODE_CTS,
	RK_CIPHER_MODE_CTR,
	RK_CIPHER_MODE_CFB,
	RK_CIPHER_MODE_OFB,
	RK_CIPHER_MODE_XTS,
	RK_CIPHER_MODE_CCM,
	RK_CIPHER_MODE_GCM,
	RK_CIPHER_MODE_CMAC,
	RK_CIPHER_MODE_CBC_MAC,
	RK_CIPHER_MODE_MAX
};

enum RK_OEM_OTP_KEYID {
	RK_OEM_OTP_KEY0 = 0,
	RK_OEM_OTP_KEY1 = 1,
	RK_OEM_OTP_KEY2 = 2,
	RK_OEM_OTP_KEY3 = 3,
	RK_OEM_OTP_KEY_FW = 10,	//keyid of fw_encryption_key
	RK_OEM_OTP_KEYMAX
};

typedef struct {
	uint32_t	algo;
	uint32_t	mode;
	uint32_t	operation;
	uint8_t		key[64];
	uint32_t	key_len;
	uint8_t		iv[16];
	void		*reserved;
} rk_cipher_config;

typedef struct {
	uint32_t	algo;
	uint32_t	mode;
	uint32_t	operation;
	uint8_t		key[32];
	uint32_t	key_len;
	uint8_t		iv[16];
	uint32_t	iv_len;
	uint32_t	tag_len;
	uint32_t	aad_len;
	uint32_t	payload_len;
	void		*reserved;
} rk_ae_config;

typedef struct {
	uint32_t	algo;
	uint8_t		*key;
	uint32_t	key_len;
} rk_hash_config;

#endif /* _RKCRYPTO_COMMON_H_ */
