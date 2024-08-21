// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include "rktest_handle.h"
#include "../rk_public_api/rk_crypto_api.h"

typedef struct {
    uint32_t	algo;
    uint32_t	mode;
    uint32_t	operation;
    uint8_t	key[64];
    uint32_t	key_len;
    uint8_t	iv[16];
    void	*reserved;
} rk_cipher_config;

/* Crypto algorithm */
enum RK_CRYPTO_ALGO {
    RK_ALGO_AES = 1,
    RK_ALGO_DES,
    RK_ALGO_TDES,
    RK_ALGO_SM4,
    RK_ALGO_ALGO_MAX
    };

/* Crypto mode */
enum RK_CIPIHER_MODE {
    RK_CIPHER_MODE_ECB = 0,
    RK_CIPHER_MODE_CBC = 1,
    RK_CIPHER_MODE_CTS = 2,
    RK_CIPHER_MODE_CTR = 3,
    RK_CIPHER_MODE_CFB = 4,
    RK_CIPHER_MODE_OFB = 5,
    RK_CIPHER_MODE_XTS = 6,
    RK_CIPHER_MODE_CCM = 7,
    RK_CIPHER_MODE_GCM = 8,
    RK_CIPHER_MODE_CMAC = 9,
    RK_CIPHER_MODE_CBC_MAC = 10,
    RK_CIPHER_MODE_MAX
    };

/* Algorithm operation */
#define RK_MODE_ENCRYPT			1
#define RK_MODE_DECRYPT			0

#define get_ms_diff(old, now) ((int)(((now).seconds - (old).seconds) * 1000 + ((now).millis - (old).millis)))

static TEE_Result rk_user_ta_cipher(rk_cipher_config *config, uint8_t *src, uint32_t len)
{
    TEE_UUID sta_uuid = { 0x0cacdb5d, 0x4fea, 0x466c,
                         { 0x97, 0x16, 0x3d, 0x54, 0x16, 0x52, 0x83, 0x0f } };
    TEE_TASessionHandle sta_session = TEE_HANDLE_NULL;
    uint32_t origin;
    TEE_Result res;
    TEE_Param taParams[4];
    uint32_t nParamTypes;
    TEE_Time t1;
    TEE_Time t2;

    //IMSG("--------------TEE_OpenTASession------------\n");
    nParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE);

    TEE_GetSystemTime(&t1);
    res = TEE_OpenTASession(&sta_uuid, 0, nParamTypes, taParams, &sta_session, &origin);
    if (res != TEE_SUCCESS) {
        EMSG("TEE_OpenTASession failed\n");
        return res;
    }
    TEE_GetSystemTime(&t2);
    IMSG("TEE_OpenTASession use time: %d", get_ms_diff(t1, t2));

    nParamTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                                  TEE_PARAM_TYPE_MEMREF_INOUT,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE);

    taParams[0].memref.buffer = config;
    taParams[0].memref.size = sizeof(*config);
    taParams[1].memref.buffer = src;
    taParams[1].memref.size = len;

    TEE_GetSystemTime(&t1);
    res = TEE_InvokeTACommand(sta_session, 0, 3, nParamTypes, taParams, &origin);
    if (res != TEE_SUCCESS) {
        EMSG("TEE_InvokeTACommand returned 0x%x\n", res);
    }
    TEE_GetSystemTime(&t2);
    IMSG("TEE_InvokeTACommand use time: %d", get_ms_diff(t1, t2));

    TEE_GetSystemTime(&t1);
    TEE_CloseTASession(sta_session);
    TEE_GetSystemTime(&t2);
    IMSG("TEE_CloseTASession use time: %d", get_ms_diff(t1, t2));

    sta_session = TEE_HANDLE_NULL;

    return res;
}

TEE_Result handle_crypto_hw(uint32_t param_types, TEE_Param params[4])
{
    uint32_t res;
    TEE_Time t1;
    TEE_Time t2;
    rk_cipher_config config;
    uint32_t algo = RK_ALGO_AES;
    uint32_t mode = RK_CIPHER_MODE_CBC;
    uint8_t key[16] = {
        0x1a, 0x0c, 0x67, 0xa4, 0xff, 0x33, 0x07, 0x36,
        0xf6, 0x48, 0x08, 0xc8, 0x22, 0x00, 0xb2, 0xc0,
    };
    uint32_t key_len = sizeof(key);
    uint8_t iv[16] = {
        0x10, 0x44, 0x80, 0xb3, 0x88, 0x5f, 0x02, 0x03,
        0x05, 0x21, 0x07, 0xc9, 0x44, 0x00, 0x1b, 0x80,
    };

    ASSERT_PARAM_TYPES(TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
		       TEE_PARAM_TYPE_NONE,
		       TEE_PARAM_TYPE_NONE,
		       TEE_PARAM_TYPE_NONE));

    config.algo = algo;
    config.mode = mode;
    config.operation = RK_MODE_ENCRYPT;
    config.key_len = key_len;
    config.reserved = NULL;
    TEE_MemMove(config.iv, iv, sizeof(iv));
    TEE_MemMove(config.key, key, sizeof(key));

    TEE_GetSystemTime(&t1);
    res = rk_user_ta_cipher(&config, params[0].memref.buffer, params[0].memref.size);
    if (res)
        EMSG("test rk_user_ta_cipher fail! 0x%08x", res);
    TEE_GetSystemTime(&t2);
    IMSG("rk_user_ta_cipher encrypt use time: %d", get_ms_diff(t1, t2));

    config.algo = algo;
    config.mode = mode;
    config.operation = RK_MODE_DECRYPT;
    config.key_len = key_len;
    config.reserved = NULL;
    TEE_MemMove(config.iv, iv, sizeof(iv));
    TEE_MemMove(config.key, key, sizeof(key));

    TEE_GetSystemTime(&t1);
    res = rk_user_ta_cipher(&config, params[0].memref.buffer, params[0].memref.size);
    if (res)
        EMSG("test rk_user_ta_cipher fail! 0x%08x", res);
    TEE_GetSystemTime(&t2);
    IMSG("rk_user_ta_cipher decrypt use time: %d", get_ms_diff(t1, t2));

    return res;
}

