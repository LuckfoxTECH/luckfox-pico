#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"

static int rk_crypto_ecb128_encrypt(void *ctx, const unsigned char *in, unsigned int length, unsigned char *out,block128_f block)
{
	unsigned int i = 0;

	for(i=0; i<length/SM4_BLOCK_SIZE; i++){
		(*block)(in+i*SM4_BLOCK_SIZE, out+i*SM4_BLOCK_SIZE, ctx);
	};
	return 0;
}

int rk_sm4_ecb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned int length, const unsigned char *key, const int key_len,
        const int enc)
{
	sm4_context  ctx;

	if(key_len != 16)
		return -1;

	if(length%SM4_BLOCK_SIZE || length == 0)
		return -1;

    if (enc) {
		rk_sm4_setkey_enc(&ctx, key);
    } else {
		rk_sm4_setkey_dec(&ctx, key);
    }

	rk_crypto_ecb128_encrypt((void*)&ctx, in, length, out, rk_rk_sm4_crypt_ecb);
	return 0;
}


