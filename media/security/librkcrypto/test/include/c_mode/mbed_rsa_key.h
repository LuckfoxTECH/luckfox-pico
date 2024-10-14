#ifndef __MBED_RSA_KEY_H__
#define __MBED_RSA_KEY_H__

#include <stdio.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define RSA_TEST_KEY_NUM 2000
typedef struct {
	uint32_t 	n_len;
	uint8_t		n[512];
	uint32_t 	e_len;
	uint8_t		e[4];
	uint32_t 	d_len;
	uint8_t		d[512];
}mbed_rsa_key_t;

extern 
const mbed_rsa_key_t rsa_key_512[RSA_TEST_KEY_NUM];
extern 
const mbed_rsa_key_t rsa_key_1024[RSA_TEST_KEY_NUM];
extern 
const mbed_rsa_key_t rsa_key_2048[RSA_TEST_KEY_NUM];
extern 
const mbed_rsa_key_t rsa_key_3072[RSA_TEST_KEY_NUM];
extern 
const mbed_rsa_key_t rsa_key_4096[RSA_TEST_KEY_NUM];


#ifdef __cplusplus
}
#endif

#endif

