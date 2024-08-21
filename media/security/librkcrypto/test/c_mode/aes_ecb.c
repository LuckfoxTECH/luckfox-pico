#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aes_core.h"



static int rk_crypto_ecb128_encrypt(const unsigned char *in, unsigned long length, unsigned char *out,
		const RK_AES_KEY *ks, const int enc)
{
	unsigned long i = 0;
	
	for(i=0; i<length/16; i++){
		if (enc)
			rk_aes_encrypt(in+i*16, out+i*16, ks);
		else
			rk_aes_decrypt(in+i*16, out+i*16, ks);

	};
	return 0;
}

int rk_aes_ecb_encrypt(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        const int enc)
{
    RK_AES_KEY ks;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;
	
	if (key_len != 128/8 && key_len != 192/8 && key_len != 256/8)
		return -2;
	
	if(length % 16 != 0)
		return -3;

    if (enc) {
            rk_aes_set_encrypt_key(key, key_len * 8, &ks);
    } else {
            rk_aes_set_decrypt_key(key, key_len * 8, &ks);
    }

	 rk_crypto_ecb128_encrypt(in, length, out, &ks, enc);
	 return 0;
}


