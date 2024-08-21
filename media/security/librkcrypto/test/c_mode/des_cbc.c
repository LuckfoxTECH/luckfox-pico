#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "des_core.h"

static void rk_crypto_cbc128_encrypt(void *ctx, const unsigned char *in, unsigned char *out,
			int len, unsigned char *ivec, block128_f block)
{
	int n;
	const unsigned char *iv = ivec;

	while (len) {
		for(n=0; n<DES_BLOCK_SIZE && n<len; ++n)
			out[n] = in[n] ^ iv[n];
		for(; n<DES_BLOCK_SIZE; ++n)
			out[n] = iv[n];
		(*block)(out, out, ctx);
		iv = out;
		if (len<=DES_BLOCK_SIZE) break;
		len -= DES_BLOCK_SIZE;
		in  += DES_BLOCK_SIZE;
		out += DES_BLOCK_SIZE;
	}
	memcpy(ivec,iv,DES_BLOCK_SIZE);
}

static void rk_crypto_cbc128_decrypt(void *ctx, const unsigned char *in, unsigned char *out,
			int len, unsigned char *ivec, block128_f block)
{
	int n;
	unsigned char c;
	unsigned char tmp_buf[DES_BLOCK_SIZE];
	
	memset(tmp_buf, 0x00, sizeof(tmp_buf));

	while (len) {
		(*block)(in, tmp_buf,ctx);
		for(n=0; n<DES_BLOCK_SIZE && n<len; ++n) {
			c = in[n];
			out[n] = tmp_buf[n] ^ ivec[n];
			ivec[n] = c;
		}
		if (len<=DES_BLOCK_SIZE) {
			for (; n<DES_BLOCK_SIZE; ++n)
				ivec[n] = in[n];
			break;
		}
		len -= DES_BLOCK_SIZE;
		in	+= DES_BLOCK_SIZE;
		out += DES_BLOCK_SIZE;
	}

}
  
int rk_des_cbc_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        unsigned char *ivec, const int enc)
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
					rk_crypto_cbc128_encrypt((void*)&ctx, in, out, length, ivec, rk_des_crypt_ecb);
					break;
				case 16:
					rk_des3_set2key_enc(&ctx3, key);
					rk_crypto_cbc128_encrypt((void*)&ctx3, in, out, length, ivec, rk_des3_crypt_ecb);
					break;
				case 24:
					rk_des3_set3key_enc(&ctx3, key);
					rk_crypto_cbc128_encrypt((void*)&ctx3, in, out, length, ivec, rk_des3_crypt_ecb);
					break;
				default:
					return -1;
			}

        } else {
			switch(key_len){
				case 8:
					rk_des_setkey_dec(&ctx, key);
					rk_crypto_cbc128_decrypt((void*)&ctx, in, out, length, ivec, rk_des_crypt_ecb);
					break;
				case 16:
					rk_des3_set2key_dec(&ctx3, key);
					rk_crypto_cbc128_decrypt((void*)&ctx3, in, out, length, ivec, rk_des3_crypt_ecb);
					break;
				case 24:
					rk_des3_set3key_dec(&ctx3, key);
					rk_crypto_cbc128_decrypt((void*)&ctx3, in, out, length, ivec, rk_des3_crypt_ecb);
					break;
				default:
					return -1;
			}
        }
		return 0;
}


