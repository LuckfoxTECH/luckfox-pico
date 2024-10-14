#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aes_core.h"

static void rk_crypto_cbc128_encrypt(const unsigned char *in, unsigned char *out,
		 int len, const void *key, unsigned char *ivec)
{
	int n;
	const unsigned char *iv = ivec;

	while (len) {
		for(n=0; n<AES_BLOCK_SIZE && n<len; ++n)
		 	out[n] = in[n] ^ iv[n];
		for(; n<AES_BLOCK_SIZE; ++n)
		 	out[n] = iv[n];
		rk_aes_encrypt(out, out,key);
		iv = out;
		if (len<=AES_BLOCK_SIZE) break;
		len -= AES_BLOCK_SIZE;
		in  += AES_BLOCK_SIZE;
		out += AES_BLOCK_SIZE;
	}
	memcpy(ivec,iv,AES_BLOCK_SIZE);
}

static void rk_crypto_cbc128_decrypt(const unsigned char *in, unsigned char *out,
		 int len, const void *key, unsigned char *ivec)
{
	int n;
	unsigned char c;
	unsigned char tmp_buf[AES_BLOCK_SIZE];

	memset(tmp_buf, 0x00, sizeof(tmp_buf));

	while (len) {
		rk_aes_decrypt(in, tmp_buf, key);
		for(n=0; n<AES_BLOCK_SIZE && n<len; ++n) {
			 c = in[n];
			 out[n] = tmp_buf[n] ^ ivec[n];
			 ivec[n] = c;
		}
		if (len<=AES_BLOCK_SIZE) {
			 for (; n<AES_BLOCK_SIZE; ++n)
				 ivec[n] = in[n];
			 break;
		}
		len -= AES_BLOCK_SIZE;
		in  += AES_BLOCK_SIZE;
		out += AES_BLOCK_SIZE;
	}
}
   

static int rk_crypto_cts_encrypt(const unsigned char *in, unsigned char *out,
		unsigned long length, const RK_AES_KEY *ks1, unsigned char *ivec, const int enc)
{
	int k = 0, r = 0;

	r = length % AES_BLOCK_SIZE;
	if (r) {
		k = length - r - AES_BLOCK_SIZE;
	} else {
		k = length;
	}

	if (enc){
		unsigned char  peniv[AES_BLOCK_SIZE] = {0};
		memset(peniv, 0x00, sizeof(peniv));

		rk_crypto_cbc128_encrypt(in, out, k, ks1, ivec);

		if (r) {
			memcpy(peniv, in + k + AES_BLOCK_SIZE, r);

			rk_crypto_cbc128_encrypt(in + k, out + k, AES_BLOCK_SIZE, ks1, ivec);
			memcpy(out + length - r, out + k, r);
			rk_crypto_cbc128_encrypt(peniv, out + k, AES_BLOCK_SIZE, ks1, ivec);
		}else{
			//swap last two block
			memcpy(peniv, out + length - AES_BLOCK_SIZE, AES_BLOCK_SIZE);
			memcpy(out + length - AES_BLOCK_SIZE, out + length - 2*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
			memcpy(out + length - 2*AES_BLOCK_SIZE, peniv, AES_BLOCK_SIZE);
		}
	}else{
		int i;
		unsigned char *pout_tmp = NULL;
		unsigned char  tmp1[AES_BLOCK_SIZE], tmp2[AES_BLOCK_SIZE];

		memset(tmp1, 0x00, sizeof(tmp1));
		memset(tmp2, 0x00, sizeof(tmp2));

		if(r == 0){
			rk_crypto_cbc128_decrypt(in, out, k-2*AES_BLOCK_SIZE, ks1, ivec);
			//swap last two block
			rk_crypto_cbc128_decrypt(in + length - AES_BLOCK_SIZE, out + length - 2*AES_BLOCK_SIZE, 
										AES_BLOCK_SIZE, ks1, ivec);
			rk_crypto_cbc128_decrypt(in + length - 2*AES_BLOCK_SIZE, out + length - AES_BLOCK_SIZE, 
										AES_BLOCK_SIZE, ks1, ivec);
		}else{
			rk_crypto_cbc128_decrypt(in, out, k, ks1, ivec);
			rk_aes_decrypt(in + k, tmp1, ks1);

			memcpy(tmp2, in + k + AES_BLOCK_SIZE, r);
			memcpy(tmp2+r, tmp1+r, AES_BLOCK_SIZE-r);

			//get last one plain text
			pout_tmp = out + k + AES_BLOCK_SIZE;
			for(i=0; i<AES_BLOCK_SIZE; i++,pout_tmp++)
		 		*pout_tmp = tmp1[i] ^ tmp2[i];

			rk_aes_decrypt(tmp2, tmp2, ks1);
	
			//get sencond to last plain text
			pout_tmp = out + k;
			for(i=0; i<AES_BLOCK_SIZE; i++,pout_tmp++)
		 		*pout_tmp = tmp2[i] ^ ivec[i];
		}
	}

	return 0;
}

   
int rk_aes_cts_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        unsigned char *ivec, const int enc)
{
	RK_AES_KEY ks1;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;

	if (key_len != 128/8 && key_len != 192/8 && key_len != 256/8)
		return -2;

	if(length <= AES_BLOCK_SIZE)
		return -3;

	if (enc) {
		rk_aes_set_encrypt_key(key, key_len * 8, &ks1);
	} else {
		rk_aes_set_decrypt_key(key, key_len * 8, &ks1);
	}

	rk_crypto_cts_encrypt(in, out, length, &ks1, ivec, enc);

	return 0;
}


