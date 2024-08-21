#ifndef __DES_CORE_H__
#define __DES_CORE_H__

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define ENCRYPT	1
#define DECRYPT	0

#define DES_BLOCK_SIZE 8

#ifdef OPENSSL_FIPS
#define FIPS_AES_SIZE_T	int
#endif

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
	DES_MODE_ECB = 0,
	DES_MODE_CBC,
	DES_MODE_OFB,
	DES_MODE_CFB
};

/**
 * \brief          DES context structure
 */
typedef struct
{
    unsigned int sk[32];            /*!<  DES subkeys       */
}
rk_des_context;

/**
 * \brief          Triple-DES context structure
 */
typedef struct
{
    unsigned int  sk[96];            /*!<  3DES subkeys      */
}
rk_des3_context;

int rk_des_setkey_enc( rk_des_context *ctx, const unsigned char key[DES_BLOCK_SIZE] );
int rk_des_setkey_dec( rk_des_context *ctx, const unsigned char key[DES_BLOCK_SIZE] );
int rk_des3_set2key_enc( rk_des3_context *ctx,
						  const unsigned char key[DES_BLOCK_SIZE * 2] );
int rk_des3_set2key_dec( rk_des3_context *ctx,
						  const unsigned char key[DES_BLOCK_SIZE * 2] );
int rk_des3_set3key_enc( rk_des3_context *ctx,
						  const unsigned char key[DES_BLOCK_SIZE * 3] );
int rk_des3_set3key_dec( rk_des3_context *ctx,
						  const unsigned char key[DES_BLOCK_SIZE * 3] );
int rk_des_crypt_ecb(const unsigned char input[8],
						unsigned char output[8], void *ctx );

int rk_des3_crypt_ecb(const unsigned char input[8],
                     unsigned char output[8],  void *ctx);

typedef int (*block128_f)(const unsigned char input[8],
						unsigned char output[8], void *ctx);


#ifdef  __cplusplus
}
#endif

#endif
