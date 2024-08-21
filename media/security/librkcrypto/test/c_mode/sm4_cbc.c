#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"

static void rk_crypto_cbc128_encrypt(void *ctx, const unsigned char *in, unsigned char *out,
			unsigned int len, unsigned char *ivec, block128_f block)
{
	unsigned int n;
	const unsigned char *iv = ivec;

	while (len) {
		for(n=0; n<SM4_BLOCK_SIZE && n<len; ++n)
			out[n] = in[n] ^ iv[n];
		for(; n<SM4_BLOCK_SIZE; ++n)
			out[n] = iv[n];
		(*block)(out, out, ctx);
		iv = out;
		if (len<=SM4_BLOCK_SIZE) break;
		len -= SM4_BLOCK_SIZE;
		in  += SM4_BLOCK_SIZE;
		out += SM4_BLOCK_SIZE;
	}
	memcpy(ivec,iv,SM4_BLOCK_SIZE);
}

static void rk_crypto_cbc128_decrypt(void *ctx, const unsigned char *in, unsigned char *out,
			unsigned int len, unsigned char *ivec, block128_f block)
{
	unsigned int n;
	unsigned char c;
	unsigned char tmp_buf[SM4_BLOCK_SIZE];

	memset(tmp_buf, 0x00, sizeof(tmp_buf));

	while (len) {
		(*block)(in, tmp_buf, ctx);
		for(n=0; n<SM4_BLOCK_SIZE && n<len; ++n) {
			c = in[n];
			out[n] = tmp_buf[n] ^ ivec[n];
			ivec[n] = c;
		}
		if (len<=SM4_BLOCK_SIZE) {
			for (; n<SM4_BLOCK_SIZE; ++n)
				ivec[n] = in[n];
			break;
		}
		len -= SM4_BLOCK_SIZE;
		in	+= SM4_BLOCK_SIZE;
		out += SM4_BLOCK_SIZE;
	}

}

int rk_sm4_cbc_encrypt(const unsigned char *in, unsigned char *out,
        unsigned int length, const unsigned char *key, const int key_len,
        unsigned char *ivec, const int enc)
{
		sm4_context  ctx;

		if(key_len != 16)
			return -1;

		if(length%SM4_BLOCK_SIZE || length == 0)
			return -1;

        if (enc) {
			rk_sm4_setkey_enc(&ctx, key);
			rk_crypto_cbc128_encrypt((void*)&ctx, in, out, length, ivec, rk_rk_sm4_crypt_ecb);
        } else {
			rk_sm4_setkey_dec(&ctx, key);
			rk_crypto_cbc128_decrypt((void*)&ctx, in, out, length, ivec, rk_rk_sm4_crypt_ecb);
        }


		return 0;
}


