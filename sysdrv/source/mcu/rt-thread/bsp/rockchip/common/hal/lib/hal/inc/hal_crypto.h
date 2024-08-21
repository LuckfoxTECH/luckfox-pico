/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_CRYPTO_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRYPTO
 *  @{
 */

#ifndef _HAL_CRYPTO_H_
#define _HAL_CRYPTO_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup CRYPTO_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define RK_DES_KEYSIZE      8
#define RK_TDES_EDE_KEYSIZE 24
#define RK_AES_KEYSIZE_128  16
#define RK_AES_KEYSIZE_192  24
#define RK_AES_KEYSIZE_256  32
#define RK_HMAC_KEYSIZE_MAX 64
#define RK_DES_BLOCK_SIZE   8
#define RK_AES_BLOCK_SIZE   16

#define RK_HASH_MD5_SIZE        16
#define RK_HASH_SHA1_SIZE       20
#define RK_HASH_SHA224_SIZE     28
#define RK_HASH_SHA256_SIZE     32
#define RK_HASH_SHA384_SIZE     48
#define RK_HASH_SHA512_SIZE     64
#define RK_HASH_SHA512_224_SIZE 28
#define RK_HASH_SHA512_256_SIZE 32

#define RK_TRNG_MAX_SIZE 32
/***************************** Structure Definition **************************/

/**
 * struct CRYPTO_DEV - crypto dev struct.
 */
struct CRYPTO_DEV {
    void *privData; /**< crypto dev privData */
    uint32_t privDataSize;
    uint32_t privAlign;
    uint32_t dataAlign;
};

/**
 * struct CRYPTO_BIGNUM - crypto bignum struct.
 */
struct CRYPTO_BIGNUM {
    uint32_t nWords;
    uint32_t *data;
};

struct CRYPTO_INFO_DES {
    uint8_t key[RK_TDES_EDE_KEYSIZE];
    uint32_t keyLen;
    uint8_t iv[RK_DES_BLOCK_SIZE];
    uint32_t ivLen;
    uint32_t isDecrypt;
};

struct CRYPTO_INFO_AES {
    uint8_t key1[RK_AES_KEYSIZE_256];
    uint8_t key2[RK_AES_KEYSIZE_256];
    uint32_t keyLen;
    uint8_t iv[RK_AES_BLOCK_SIZE];
    uint32_t ivLen;
    uint32_t isDecrypt;
};

struct CRYPTO_INFO_HASH {
    uint32_t reserved;
};

struct CRYPTO_INFO_HMAC {
    uint8_t key[RK_HMAC_KEYSIZE_MAX];
    uint32_t keyLen;
};

struct CRYPTO_INFO_AES_HASH {
    uint8_t key[RK_AES_KEYSIZE_256];
    uint32_t keyLen;
    uint8_t iv[RK_AES_BLOCK_SIZE];
    uint32_t ivLen;
    uint32_t isDecrypt;
    uint32_t RxTx;
};

struct CRYPTO_ALGO_CONFIG {
    uint32_t chn;
    uint32_t algo;
    uint32_t mode;

    union {
        struct CRYPTO_INFO_DES des;
        struct CRYPTO_INFO_AES aes;
        struct CRYPTO_INFO_HASH hash;
        struct CRYPTO_INFO_HMAC hmac;
        struct CRYPTO_INFO_AES_HASH AesHash;
    } info;
};

struct CRYPTO_DMA_CONFIG {
    uint32_t srcAddr;
    uint32_t srcLen;
    uint32_t dstAddr;
    uint32_t dstLen;
    uint32_t isLast;
};

typedef enum {
    CRYPTO_MODE_CIPHER_ECB = 0,
    CRYPTO_MODE_CIPHER_CBC,
    CRYPTO_MODE_CIPHER_CFB,
    CRYPTO_MODE_CIPHER_OFB,
    CRYPTO_MODE_CIPHER_CTS,
    CRYPTO_MODE_CIPHER_CTR,
    CRYPTO_MODE_CIPHER_XTS,
    CRYPTO_MODE_CIPHER_CCM,
    CRYPTO_MODE_CIPHER_GCM,
    CRYPTO_MODE_CIPHER_CMAC,
    CRYPTO_MODE_CIPHER_CBC_MAC,
    CRYPTO_MODE_CIPHER_MAX
} eCRYPTO_cipherMode;

typedef enum {
    CRYPTO_MODE_HASH_MD5 = 0,
    CRYPTO_MODE_HASH_SHA1,
    CRYPTO_MODE_HASH_SHA224,
    CRYPTO_MODE_HASH_SHA256,
    CRYPTO_MODE_HASH_SHA384,
    CRYPTO_MODE_HASH_SHA512,
    CRYPTO_MODE_HASH_SHA512_224,
    CRYPTO_MODE_HASH_SHA512_256,
    CRYPTO_MODE_HASH_MAX
} eCRYPTO_hashMode;

typedef enum {
    CRYPTO_MODE_HMAC_MD5 = 0,
    CRYPTO_MODE_HMAC_SHA1,
    CRYPTO_MODE_HMAC_SHA256,
    CRYPTO_MODE_HMAC_SHA512,
    CRYPTO_MODE_HMAC_MAX
} eCRYPTO_hmacMode;

typedef enum {
    CRYPTO_ALGO_AES = 0,
    CRYPTO_ALGO_DES,
    CRYPTO_ALGO_TDES,
    CRYPTO_ALGO_HASH,
    CRYPTO_ALGO_HMAC,
    CRYPTO_ALGO_AES_HASH,
    CRYPTO_ALGO_MAX
} eCRYPTO_algo;

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup CRYPTO_Public_Function_Declare Public Function Declare
 *  @{
 */

/**
 * @brief  Init crypto and deassert reset.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_Init(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  DeInit crypto.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_DeInit(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  init crypto algo config.
 * @param  pCrypto: the handle of crypto.
 * @param  pConfig: algo config info.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_AlgoInit(struct CRYPTO_DEV *pCrypto,
                               struct CRYPTO_ALGO_CONFIG *pConfig);

/**
 * @brief  deinit crypto algo config.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_AlgoDeInit(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  set crypto dma config.
 * @param  pCrypto: the handle of crypto.
 * @param  pConfig: dma config info.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_DMAConfig(struct CRYPTO_DEV *pCrypto,
                                struct CRYPTO_DMA_CONFIG *pConfig);

/**
 * @brief  start crypto calculate.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_DMAStart(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  check interrupt status.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Check
 */
HAL_Check HAL_CRYPTO_CheckIntStatus(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  check whether hash flag is valid.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Check
 */
HAL_Check HAL_CRYPTO_CheckHashValid(struct CRYPTO_DEV *pCrypto);

/**
* @brief  check whether tag flag is valid.
* @param  pCrypto: the handle of crypto.
* @return HAL_Check
*/
HAL_Check HAL_CRYPTO_CheckTagValid(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  read hash value from regs.
 * @param  pCrypto: the handle of crypto.
 * @param  out: hash value buffer.
 * @param  outLen: hash value length.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_ReadHashReg(struct CRYPTO_DEV *pCrypto,
                                  uint8_t *out, uint32_t *outLen);

/**
 * @brief  read tag value from regs.
 * @param  pCrypto: the handle of crypto.
 * @param  out: tag value buffer.
 * @param  outLen: tag value length.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_ReadTagReg(struct CRYPTO_DEV *pCrypto,
                                 uint8_t *out, uint32_t *outLen);

/**
 * @brief  clear interrupt status.
 * @param  pCrypto: the handle of crypto.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_ClearISR(struct CRYPTO_DEV *pCrypto);

/**
 * @brief  calculate exp mod. pOut = pIn ^ pE mod pN
 * @param  pCrypto: the handle of crypto.
 * @param  pIn: the point of input data bignum.
 * @param  pE: the point of exponent bignum.
 * @param  pN: the point of modulus bignum.
 * @param  pOut: the point of outputs bignum.
 * @param  pTmp: the point of tmpdata bignum.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_ExptMod(struct CRYPTO_DEV *pCrypto,
                              struct CRYPTO_BIGNUM *pIn,
                              struct CRYPTO_BIGNUM *pE,
                              struct CRYPTO_BIGNUM *pN,
                              struct CRYPTO_BIGNUM *pOut,
                              struct CRYPTO_BIGNUM *pTmp);

/**
 * @brief  get TRNG data
 * @param  pCrypto: the handle of crypto.
 * @param  pTrng: trng buffer.
 * @param  len: trng buffer length.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_Trng(struct CRYPTO_DEV *pCrypto, uint8_t *pTrng, uint32_t len);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_CRYPTO_MODULE_ENABLED */
