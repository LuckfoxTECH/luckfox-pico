#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "des_core.h"

static void rk_crypto_cfb128_encrypt(void *ctx, const unsigned char *in, unsigned char *out,
			size_t len, unsigned char ivec[DES_BLOCK_SIZE], int *num,
			int enc, block128_f block)
{
    unsigned int n;
    size_t l = 0;

    n = *num;
    if (enc) {

	while (l<len) {
		if (n == 0) {
			(*block)(ivec, ivec, ctx);
		}
		out[l] = ivec[n] ^= in[l];
		++l;
		n = (n+1) % DES_BLOCK_SIZE;
	}
	*num = n;
    } else {
	while (l<len) {
		unsigned char c;
		if (n == 0) {
			(*block)(ivec, ivec, ctx);
		}
		out[l] = ivec[n] ^ (c = in[l]); ivec[n] = c;
		++l;
		n = (n+1) % DES_BLOCK_SIZE;
	}
	*num=n;
    }
}

int rk_des_cfb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        unsigned char *ivec, const int enc)
{
	rk_des_context  ctx;
	rk_des3_context ctx3;
	int num = 0;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;

	if (key_len != 8 && key_len != 16 && key_len != 24)
		return -2;

	if(length == 0)
		return -3;

	switch(key_len){
	case 8:
		rk_des_setkey_enc(&ctx, key);
		rk_crypto_cfb128_encrypt((void *)&ctx, in, out,length, ivec,
					 &num,enc, rk_des_crypt_ecb);
		break;
	case 16:
		rk_des3_set2key_enc(&ctx3, key);
		rk_crypto_cfb128_encrypt((void *)&ctx3, in, out,length, ivec,
					 &num,enc, rk_des3_crypt_ecb);
		break;
	case 24:
		rk_des3_set3key_enc(&ctx3, key);
		rk_crypto_cfb128_encrypt((void *)&ctx3, in, out,length, ivec,
					 &num,enc, rk_des3_crypt_ecb);
		break;
	default:
		return -1;
	}

	 return 0;
}

