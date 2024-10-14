/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _RSA_KEY_DATA_H_
#define _RSA_KEY_DATA_H_

#include "rkcrypto_common.h"

RK_RES test_init_pubkey(rk_rsa_pub_key_pack *pub, uint32_t nbits);
RK_RES test_init_privkey(rk_rsa_priv_key_pack *piv, uint32_t nbits);

#endif /*_TEST_RSA_H_*/

