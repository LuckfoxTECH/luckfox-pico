#ifndef __SM4_CORE_H__
#define __SM4_CORE_H__

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define ENCRYPT	1
#define DECRYPT	0

#define SM4_BLOCK_SIZE 16

#ifdef OPENSSL_FIPS
#define FIPS_AES_SIZE_T	int
#endif

/* zhangzj: make sure u64 is 8 bytes */
/* zhangzj: make sure u64 is 8 bytes */
#ifndef u32

#if defined(__arch64__)
#define U64(C) C##UL
#else
#define U64(C) C##ULL
#endif

typedef int64_t i64;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8; 
#endif

enum{
	SM4_MODE_ECB = 0,
	SM4_MODE_CBC,
	SM4_MODE_OFB,
	SM4_MODE_CFB,
	SM4_MODE_CTS,
	SM4_MODE_CTR,
	SM4_MODE_XTS,
	SM4_MODE_CCM,
	SM4_MODE_GCM,
	SM4_MODE_CMAC,
	SM4_MODE_CBC_MAC
};

struct sm4_ae_in {
	void   *key;
	void   *src;
	void   *iv;
	void   *aad;
	int    key_len;
	unsigned int    src_len;
	unsigned int    iv_len;
	unsigned int    aad_len;
	unsigned int    tag_size;
};

struct sm4_ae_out {
	void   *dest;
	void   *tag;
	unsigned int    dest_len;
};

/**
 * \brief          SM4 context structure
 */
typedef struct
{
    int mode;                   /*!<  encrypt/decrypt   */
    unsigned long sk[32];       /*!<  SM4 subkeys       */
}sm4_context;

/*
 * SM4 key schedule (128-bit, encryption)
 */
void rk_sm4_setkey_enc( sm4_context *ctx, const unsigned char key[16] );

/*
 * SM4 key schedule (128-bit, decryption)
 */
void rk_sm4_setkey_dec( sm4_context *ctx, const unsigned char key[16] );

/*
 * SM4-ECB block encryption/decryption
 */
int rk_sm4_crypt_ecb( void *ctx, const unsigned char *input, unsigned char *output);

int rk_rk_sm4_crypt_ecb(const unsigned char *input,
                   unsigned char *output, void *ctx);

#ifndef block128_f
typedef int (*block128_f)( const unsigned char *input, \
                   unsigned char *output, void *ctx);
#endif
#ifdef  __cplusplus
}
#endif

#endif
