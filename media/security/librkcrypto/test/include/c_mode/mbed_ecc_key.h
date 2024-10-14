#ifndef __MBED_ECC_KEY_H__
#define __MBED_ECC_KEY_H__

#include <stdio.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "ecp.h"
#define ECC_TEST_KEY_NUM 2000


extern const mbed_ecc_key_t ecc_key_192k1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_192r1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_224k1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_224r1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_256k1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_256r1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_384r1[ECC_TEST_KEY_NUM];
extern const mbed_ecc_key_t ecc_key_521r1[ECC_TEST_KEY_NUM];

#ifdef __cplusplus
}
#endif

#endif

