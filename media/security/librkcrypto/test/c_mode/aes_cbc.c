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
		for(n=0; n<16 && n<len; ++n)
			out[n] = in[n] ^ iv[n];
		for(; n<16; ++n)
			out[n] = iv[n];
		rk_aes_encrypt(out, out,key);
		iv = out;
		if (len<=16) break;
		len -= 16;
		in  += 16;
		out += 16;
	}
	memcpy(ivec,iv,16);
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
		for(n=0; n<16 && n<len; ++n) {
			c = in[n];
			out[n] = tmp_buf[n] ^ ivec[n];
			ivec[n] = c;
		}
		if (len<=16) {
			for (; n<16; ++n)
				ivec[n] = in[n];
			break;
		}
		len -= 16;
		in	+= 16;
		out += 16;
	}

}
  
int rk_aes_cbc_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        unsigned char *ivec, const int enc)
{
		RK_AES_KEY ks;

		if(key_len != 16 && key_len != 24 && key_len != 32)
			return -1;

		if(length%AES_BLOCK_SIZE)
			return -1;

        if (enc) {
			rk_aes_set_encrypt_key(key, key_len * 8, &ks);
			rk_crypto_cbc128_encrypt(in, out, length, &ks, ivec);
        } else {
			rk_aes_set_decrypt_key(key, key_len * 8, &ks);
			rk_crypto_cbc128_decrypt(in, out, length, &ks, ivec);
        }
		return 0;
}


