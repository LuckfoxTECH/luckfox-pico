#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"

void rk_crypto_cfb128_encrypt(void *ctx, const unsigned char *in, unsigned char *out,
			size_t len, unsigned char ivec[SM4_BLOCK_SIZE], int *num,
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
		n = (n+1) % SM4_BLOCK_SIZE;
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
		n = (n+1) % SM4_BLOCK_SIZE;
	}
	*num=n;
    }
}

int rk_sm4_cfb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len,
        unsigned char *ivec, const int enc)
{
	sm4_context  ctx;
	int num = 0;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;

	if (key_len != 16)
		return -2;

	if(length == 0)
		return -3;

	rk_sm4_setkey_enc(&ctx, key);

	rk_crypto_cfb128_encrypt((void *)&ctx, in, out,length, ivec,
				&num,enc, rk_rk_sm4_crypt_ecb);
	return 0;
}


