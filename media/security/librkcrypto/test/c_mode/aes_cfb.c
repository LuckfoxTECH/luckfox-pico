#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes_core.h"


typedef void (*block128_f)(const unsigned char in[16],
					unsigned char out[16],
					const void *key);

static void rk_crypto_cfb128_encrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *key,
			unsigned char ivec[16], int *num,
			int enc, block128_f block)
{
    unsigned int n;
    size_t l = 0;
    n = *num;
    if (enc) {

	while (l < len) {
		if (n == 0) {
			(*block)(ivec, ivec, key);
		}
		out[l] = ivec[n] ^= in[l];
		++l;
		n = (n+1) % 16;
	}
	*num = n;
    } else {
	while (l < len) {
		unsigned char c;
		if (n == 0) {
			(*block)(ivec, ivec, key);
		}
		out[l] = ivec[n] ^ (c = in[l]); ivec[n] = c;
		++l;
		n = (n+1) % 16;
	}
	*num=n;
    }
}

static void rk_aes_cfb128_encrypt(const unsigned char *in, unsigned char *out,
		      size_t length, const RK_AES_KEY *key,
		      unsigned char *ivec, int *num, const int enc)
{
    rk_crypto_cfb128_encrypt(in,out,length,key,ivec,num,enc,(block128_f)rk_aes_encrypt);
}

int rk_aes_cfb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        unsigned char *ivec, const int enc)
{
	RK_AES_KEY ks1;
	int num = 0;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;

	if (key_len != 128 / 8 && key_len != 192 / 8 && key_len != 256 / 8)
		return -2;

	rk_aes_set_encrypt_key(key, key_len * 8, &ks1);
	rk_aes_cfb128_encrypt(in, out, length, &ks1, ivec, &num, enc);

	return 0;
}


