#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "des_core.h"

static int rk_crypto_ecb128_encrypt(void *ctx, const unsigned char *in, unsigned long length, unsigned char *out,block128_f block)
{
	unsigned long i = 0;
	
	for (i = 0; i < length / DES_BLOCK_SIZE; i++){
		(*block)(in + i * DES_BLOCK_SIZE, out + i * DES_BLOCK_SIZE, ctx);
	};
	return 0;
}

int rk_des_ecb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        const int enc)
{
	rk_des_context  ctx;
	rk_des3_context ctx3;
	
	if(key_len != 8 && key_len != 16 && key_len != 24)
		return -1;

	if(length%DES_BLOCK_SIZE)
		return -1;

    if (enc) {
		switch(key_len){
			case 8:
				rk_des_setkey_enc(&ctx, key);
				rk_crypto_ecb128_encrypt((void*)&ctx, in, length, out, rk_des_crypt_ecb);
				break;
			case 16:
				rk_des3_set2key_enc(&ctx3, key);
				rk_crypto_ecb128_encrypt((void*)&ctx3, in, length, out, rk_des3_crypt_ecb);
				break;
			case 24:
				rk_des3_set3key_enc(&ctx3, key);
				rk_crypto_ecb128_encrypt((void*)&ctx3, in, length, out, rk_des3_crypt_ecb);
				break;
			default:
				return -1;
		}

    } else {
		switch(key_len){
			case 8:
				rk_des_setkey_dec(&ctx, key);
				rk_crypto_ecb128_encrypt((void*)&ctx, in, length, out, rk_des_crypt_ecb);
				break;
			case 16:
				rk_des3_set2key_dec(&ctx3, key);
				rk_crypto_ecb128_encrypt((void*)&ctx3, in, length, out, rk_des3_crypt_ecb);
				break;
			case 24:
				rk_des3_set3key_dec(&ctx3, key);
				rk_crypto_ecb128_encrypt((void*)&ctx3, in, length, out, rk_des3_crypt_ecb);
				break;
			default:
				return -1;
		}
    }
	return 0;
}


