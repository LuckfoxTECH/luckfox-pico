#if defined(MBEDTLS_AES_ALT)

#ifndef MBEDTLS_AES_INF_H
#define MBEDTLS_AES_INF_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t aes_key[16];
} mbedtls_aes_context;

#ifdef __cplusplus
}
#endif

#endif /* aes.h */
#endif