#ifndef __AES_CORE_H__
#define __AES_CORE_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define AES_ENCRYPT	1
#define AES_DECRYPT	0

/* Because array size can't be a const in C, the following two are macros.
   Both sizes are in bytes. */
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16

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

/* This should be a hidden type, but EVP requires that the size be known */
struct rk_aes_key_st {
    uint32_t rd_key[4 *(AES_MAXNR + 1)];

    int rounds;
};

typedef struct rk_aes_key_st RK_AES_KEY;

enum{
	AES_MODE_XTS = 0,
	AES_MODE_ECB,
	AES_MODE_CBC,
	AES_MODE_CTS,
	AES_MODE_CTR,
	AES_MODE_OFB,
	AES_MODE_CFB
};

enum{
	HASH_MODE_MD5 = 0,
	HASH_MODE_SHA1,
	HASH_MODE_SHA256,
	HASH_MODE_SHA224,
	HASH_MODE_SHA512,
	HASH_MODE_SHA384
};

enum{
	HMAC_MODE_MD5 = 0,
	HMAC_MODE_SHA1,
	HMAC_MODE_SHA256,
	HMAC_MODE_SHA512
};


struct ctr_state {
	unsigned char ivec[AES_BLOCK_SIZE]; /* ivec[0..7] as IV, ivec[8..15] as counter */
	unsigned char ecount[AES_BLOCK_SIZE];
	unsigned int num;
};

struct aes_ae_in {
	const void   *key;
	const void   *src;
	const void   *iv;
	const void   *aad;
	int    key_len;
	int    src_len;
	int    iv_len;
	int    aad_len;
	int    tag_size;
};

struct aes_ae_out {
	void   *dest;
	void   *tag;
	int    dest_len;
};

int rk_aes_set_encrypt_key(const unsigned char *userKey, const int bits,
	RK_AES_KEY *key);
int rk_aes_set_decrypt_key(const unsigned char *userKey, const int bits,
	RK_AES_KEY *key);

void rk_aes_encrypt(const unsigned char *in, unsigned char *out,
	const RK_AES_KEY *key);
void rk_aes_decrypt(const unsigned char *in, unsigned char *out,
	const RK_AES_KEY *key);



#ifdef  __cplusplus
}
#endif

#endif
