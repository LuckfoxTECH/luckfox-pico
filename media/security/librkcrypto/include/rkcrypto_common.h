/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _RKCRYPTO_COMMON_H_
#define _RKCRYPTO_COMMON_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t RK_RES;
typedef uint32_t rk_handle;

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* RK_CRYPTO api verison */
#define RK_CRYPTO_API_MAJOR_VERSION 1
#define RK_CRYPTO_API_MINOR_VERSION 2
#define RK_CRYPTO_API_REVISION_VERSION 0

#define RK_CRYPTO_API_VERSION                                                  \
  STR(RK_CRYPTO_API_MAJOR_VERSION)                                             \
  "." STR(RK_CRYPTO_API_MINOR_VERSION) "." STR(RK_CRYPTO_API_REVISION_VERSION)

#define RK_CRYPTO_API_FULL_VERSION "rkcrypto api version " RK_CRYPTO_API_VERSION

/* API return codes */
#define RK_CRYPTO_SUCCESS 0x00000000
#define RK_CRYPTO_ERR_GENERIC 0xF0000000
#define RK_CRYPTO_ERR_PARAMETER 0xF0000001
#define RK_CRYPTO_ERR_STATE 0xF0000002
#define RK_CRYPTO_ERR_NOT_SUPPORTED 0xF0000003
#define RK_CRYPTO_ERR_OUT_OF_MEMORY 0xF0000004
#define RK_CRYPTO_ERR_ACCESS_DENIED 0xF0000005
#define RK_CRYPTO_ERR_BUSY 0xF0000006
#define RK_CRYPTO_ERR_TIMEOUT 0xF0000007
#define RK_CRYPTO_ERR_UNINITED 0xF0000008
#define RK_CRYPTO_ERR_KEY 0xF0000009
#define RK_CRYPTO_ERR_VERIFY 0xF000000A
#define RK_CRYPTO_ERR_PADDING 0xF000000B
#define RK_CRYPTO_ERR_PADDING_OVERFLOW 0xF000000C
#define RK_CRYPTO_ERR_MAC_INVALID 0xF000000D

/* Algorithm operation */
#define RK_OP_CIPHER_ENC 1
#define RK_OP_CIPHER_DEC 0

/* Algorithm block length */
#define DES_BLOCK_SIZE 8
#define AES_BLOCK_SIZE 16
#define SM4_BLOCK_SIZE 16
#define SHA1_HASH_SIZE 20
#define SHA224_HASH_SIZE 28
#define SHA256_HASH_SIZE 32
#define SHA384_HASH_SIZE 48
#define SHA512_HASH_SIZE 64
#define SHA512_224_HASH_SIZE 28
#define SHA512_256_HASH_SIZE 32
#define MD5_HASH_SIZE 16
#define SM3_HASH_SIZE 32

#define SM3_BLOCK_SIZE 64
#define SHA1_BLOCK_SIZE 64
#define MD5_BLOCK_SIZE 64
#define SHA224_BLOCK_SIZE 64
#define SHA256_BLOCK_SIZE 64
#define SHA384_BLOCK_SIZE 128
#define SHA512_BLOCK_SIZE 128
#define SHA512_224_BLOCK_SIZE 128
#define SHA512_256_BLOCK_SIZE 128

#define AES_AE_DATA_BLOCK 128
#define MAX_HASH_BLOCK_SIZE 128
#define MAX_TDES_KEY_SIZE 24
#define MAX_AES_KEY_SIZE 32
#define MAX_AE_TAG_SIZE 16

#define RSA_BITS_1024 1024
#define RSA_BITS_2048 2048
#define RSA_BITS_3072 3072
#define RSA_BITS_4096 4096
#define MAX_RSA_KEY_BITS RSA_BITS_4096

#define RK_CRYPTO_MAX_DATA_LEN (1 * 1024 * 1024)

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
  RK_ALGO_CMAC_AES,
  RK_ALGO_CBCMAC_AES,
  RK_ALGO_CMAC_SM4,
  RK_ALGO_CBCMAC_SM4,
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
  RK_CIPHER_MODE_MAX
};

enum RK_OEM_OTP_KEYID {
  RK_OEM_OTP_KEY0 = 0,
  RK_OEM_OTP_KEY1 = 1,
  RK_OEM_OTP_KEY2 = 2,
  RK_OEM_OTP_KEY3 = 3,
  RK_OEM_OTP_KEY_FW = 10, // keyid of fw_encryption_key
  RK_OEM_OTP_KEYMAX
};

enum RK_RSA_KEY_TYPE {
  RK_RSA_KEY_TYPE_PLAIN = 0,
  RK_RSA_KEY_TYPE_KEY0_ENC = RK_OEM_OTP_KEY0 + 1,
  RK_RSA_KEY_TYPE_KEY1_ENC,
  RK_RSA_KEY_TYPE_KEY2_ENC,
  RK_RSA_KEY_TYPE_KEY3_ENC,
  RK_RSA_KEY_TYPE_MAX,
};

enum RK_RSA_CRYPT_PADDING {
  RK_RSA_CRYPT_PADDING_NONE = 0x00,  /* without padding */
  RK_RSA_CRYPT_PADDING_BLOCK_TYPE_0, /* PKCS#1 block type 0 padding*/
  RK_RSA_CRYPT_PADDING_BLOCK_TYPE_1, /* PKCS#1 block type 1padding*/
  RK_RSA_CRYPT_PADDING_BLOCK_TYPE_2, /* PKCS#1 block type 2 padding*/
  RK_RSA_CRYPT_PADDING_OAEP_SHA1,    /* PKCS#1 RSAES-OAEP-SHA1 padding*/
  RK_RSA_CRYPT_PADDING_OAEP_SHA224,  /* PKCS#1 RSAES-OAEP-SHA224 padding*/
  RK_RSA_CRYPT_PADDING_OAEP_SHA256,  /* PKCS#1 RSAES-OAEP-SHA256   padding*/
  RK_RSA_CRYPT_PADDING_OAEP_SHA384,  /* PKCS#1 RSAES-OAEP-SHA384   padding*/
  RK_RSA_CRYPT_PADDING_OAEP_SHA512,  /* PKCS#1 RSAES-OAEP-SHA512   padding*/
  RK_RSA_CRYPT_PADDING_PKCS1_V1_5,   /* PKCS#1 RSAES-PKCS1_V1_5    padding*/
};

enum RK_RSA_SIGN_PADDING {
  RK_RSA_SIGN_PADDING_PKCS1_V15_SHA1 =
      0x100, /* PKCS#1 RSASSA_PKCS1_V15_SHA1 signature*/
  RK_RSA_SIGN_PADDING_PKCS1_V15_SHA224, /* PKCS#1 RSASSA_PKCS1_V15_SHA224
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_V15_SHA256, /* PKCS#1 RSASSA_PKCS1_V15_SHA256
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_V15_SHA384, /* PKCS#1 RSASSA_PKCS1_V15_SHA384
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_V15_SHA512, /* PKCS#1 RSASSA_PKCS1_V15_SHA512
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA1,   /* PKCS#1 RSASSA_PKCS1_PSS_SHA1
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA224, /* PKCS#1 RSASSA_PKCS1_PSS_SHA224
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA256, /* PKCS#1 RSASSA_PKCS1_PSS_SHA256
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA384, /* PKCS#1 RSASSA_PKCS1_PSS_SHA1
                                           signature*/
  RK_RSA_SIGN_PADDING_PKCS1_PSS_SHA512, /* PKCS#1 RSASSA_PKCS1_PSS_SHA256
                                           signature*/
};

typedef struct {
  uint32_t algo;
  uint32_t mode;
  uint32_t operation;
  uint8_t key[64];
  uint32_t key_len;
  uint8_t iv[16];
  void *reserved;
} rk_cipher_config;

typedef struct {
  uint32_t algo;
  uint32_t mode;
  uint32_t operation;
  uint8_t key[32];
  uint32_t key_len;
  uint8_t iv[16];
  uint32_t iv_len;
  uint32_t tag_len;
  uint32_t aad_len;
  uint32_t payload_len;
  void *reserved;
} rk_ae_config;

typedef struct {
  uint32_t algo;
  uint8_t *key;
  uint32_t key_len;
} rk_hash_config;

typedef struct {
  const uint8_t *n;
  const uint8_t *e;

  uint16_t n_len;
  uint16_t e_len;
} rk_rsa_pub_key;

typedef struct {
  enum RK_RSA_KEY_TYPE key_type;
  rk_rsa_pub_key key;
} rk_rsa_pub_key_pack;

typedef struct {
  const uint8_t *n;
  const uint8_t *e;
  const uint8_t *d;
  const uint8_t *p;
  const uint8_t *q;
  const uint8_t *dp;
  const uint8_t *dq;
  const uint8_t *qp;

  uint16_t n_len;
  uint16_t e_len;
  uint16_t d_len;
  uint16_t p_len;
  uint16_t q_len;
  uint16_t dp_len;
  uint16_t dq_len;
  uint16_t qp_len;
} rk_rsa_priv_key;

typedef struct {
  enum RK_RSA_KEY_TYPE key_type;
  rk_rsa_priv_key key;
} rk_rsa_priv_key_pack;

#endif /* _RKCRYPTO_COMMON_H_ */
