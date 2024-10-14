/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#ifndef __RKCRYPTO_RSA_HELPER_H__
#define __RKCRYPTO_RSA_HELPER_H__

#include <stdint.h>
#include "rkcrypto_common.h"

/*
 * The rk_rsa_pub_key will be encoded in BER format as follows
 *
 * SEQUENCE {
 *        n INTEGER ({ rsa_get_n }),
 *        e INTEGER ({ rsa_get_e })
 * }
 */

RK_RES rk_rsa_pubkey_encode(rk_rsa_pub_key_pack *pub,
			    uint8_t *asn1_key, uint16_t *asn1_key_len, uint16_t *key_bits);

/*
 * The rk_rsa_priv_key will be encoded in BER format as follows
 *
 * SEQUENCE {
 *	version		INTEGER,
 *	n		INTEGER ({ rsa_get_n }),
 *	e		INTEGER ({ rsa_get_e }),
 *	d		INTEGER ({ rsa_get_d }),
 *	prime1		INTEGER,
 *	prime2		INTEGER,
 *	exponent1	INTEGER,
 *	exponent2	INTEGER,
 *	coefficient	INTEGER
 * }
 */
RK_RES rk_rsa_privkey_encode(rk_rsa_priv_key_pack *priv,
			     uint8_t *asn1_key, uint16_t *asn1_key_len, uint16_t *key_bits);

RK_RES rk_rsa_crypt_do_padding(enum RK_RSA_CRYPT_PADDING padding,
			       uint16_t key_len, bool is_priv_key,
			       const uint8_t *data, uint32_t data_len,
			       uint8_t *pad, uint32_t *pad_len);

RK_RES rk_rsa_crypt_undo_padding(enum RK_RSA_CRYPT_PADDING padding,
				 uint16_t key_len, bool is_priv_key,
				 const uint8_t *pad, uint32_t pad_len,
				 uint8_t *data, uint32_t *data_len);

RK_RES rk_rsa_sign_do_padding(enum RK_RSA_SIGN_PADDING padding, uint16_t key_len, uint16_t n_bits,
			      const uint8_t *data, uint32_t data_len, const uint8_t *hash,
			      uint8_t *pad, uint32_t *pad_len);

RK_RES rk_rsa_sign_undo_padding(enum RK_RSA_SIGN_PADDING padding, uint16_t key_len, uint16_t n_bits,
				const uint8_t *data, uint32_t data_len,
				const uint8_t *hash, const uint8_t *dec);
#endif

