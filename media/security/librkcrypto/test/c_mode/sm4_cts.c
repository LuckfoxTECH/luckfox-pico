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
		(*block)((const unsigned char*)out, out, ctx);
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
		in  += SM4_BLOCK_SIZE;
		out += SM4_BLOCK_SIZE;
	}
}


static int rk_crypto_cts_encrypt(void *ctx, const unsigned char *in, unsigned char *out,
		unsigned int length, unsigned char *ivec, const int enc, block128_f block)
{
	int k = 0, r = 0;

	r = length % SM4_BLOCK_SIZE;
	if (r) {
		k = length - r - SM4_BLOCK_SIZE;
	} else {
		k = length;
	}

	if (enc){
		unsigned char  peniv[SM4_BLOCK_SIZE] = {0};
		memset(peniv, 0x00, sizeof(peniv));

		rk_crypto_cbc128_encrypt(ctx, in, out, k, ivec,block);

		if (r) {
			memcpy(peniv, in + k + SM4_BLOCK_SIZE, r);

			rk_crypto_cbc128_encrypt(ctx, in + k, out + k, SM4_BLOCK_SIZE, ivec, block);
			memcpy(out + length - r, out + k, r);
			rk_crypto_cbc128_encrypt(ctx, peniv, out + k, SM4_BLOCK_SIZE, ivec, block);
		}else{
			//swap last two block
			memcpy(peniv, out + length - SM4_BLOCK_SIZE, SM4_BLOCK_SIZE);
			memcpy(out + length - SM4_BLOCK_SIZE, out + length - 2*SM4_BLOCK_SIZE, SM4_BLOCK_SIZE);
			memcpy(out + length - 2*SM4_BLOCK_SIZE, peniv, SM4_BLOCK_SIZE);
		}
	}else{
		unsigned int i;
		unsigned char *pout_tmp = NULL;
		unsigned char  tmp1[SM4_BLOCK_SIZE], tmp2[SM4_BLOCK_SIZE];

		memset(tmp1, 0x00, sizeof(tmp1));
		memset(tmp2, 0x00, sizeof(tmp2));

		if(r == 0){
			rk_crypto_cbc128_decrypt(ctx, in, out, k-2*SM4_BLOCK_SIZE, ivec, block);

			//swap last two block
			rk_crypto_cbc128_decrypt(ctx, in + length - SM4_BLOCK_SIZE, out + length - 2*SM4_BLOCK_SIZE,
										SM4_BLOCK_SIZE, ivec, block);
			rk_crypto_cbc128_decrypt(ctx, in + length - 2*SM4_BLOCK_SIZE, out + length - SM4_BLOCK_SIZE,
										SM4_BLOCK_SIZE, ivec, block);
		}else{
			rk_crypto_cbc128_decrypt(ctx, in, out, k, ivec, block);
			(*block)(in + k, tmp1, ctx);

			memcpy(tmp2, in + k + SM4_BLOCK_SIZE, r);
			memcpy(tmp2+r, tmp1+r, SM4_BLOCK_SIZE-r);

			//get last one plain text
			pout_tmp = out + k + SM4_BLOCK_SIZE;
			for(i=0; i<SM4_BLOCK_SIZE; i++,pout_tmp++)
		 		*pout_tmp = tmp1[i] ^ tmp2[i];

			(*block)(tmp2, tmp2, ctx);

			//get sencond to last plain text
			pout_tmp = out + k;
			for(i=0; i<SM4_BLOCK_SIZE; i++,pout_tmp++)
		 		*pout_tmp = tmp2[i] ^ ivec[i];

		}
	}

	return 0;
}


int rk_sm4_cts_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len,
        unsigned char *ivec, const int enc)
{
	sm4_context  ctx;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;

	if (key_len != 16)
		return -2;

	if(length <= SM4_BLOCK_SIZE)
		return -3;

	if (enc) {
		rk_sm4_setkey_enc(&ctx, key);
	} else {
		rk_sm4_setkey_dec(&ctx, key);
	}

	rk_crypto_cts_encrypt((void*)(&ctx), in, out, length, ivec, enc, rk_rk_sm4_crypt_ecb);

	return 0;
}


