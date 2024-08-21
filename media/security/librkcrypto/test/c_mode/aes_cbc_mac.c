#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aes_core.h"

static int xor(unsigned char *a, unsigned char *b,unsigned char *c)
{
	int i = 0;

	if(a == NULL || b == NULL || c ==NULL)
		return -1;
	
	for(i = 0; i < 16; i++){
		*c = *a ^ *b;
		c++;
		b++;
		a++;
	}

	return 0;
}

static int rk_aes_cbc_mac_encrypt(const unsigned char *in, unsigned long length,unsigned char *out,
		const RK_AES_KEY *ks1, const int enc)
{
	unsigned long i = 0;
	int ret = 0;
	unsigned char tmp[16];
	memset(out, 0x00, 16);
	memset(tmp, 0x00, 16);
	
	for(i=0; i<(length/16); i++){
		xor((unsigned char *)in + 16 * i, out, out);	
		rk_aes_encrypt(out, out, ks1);
	};

	if(length%16){
		memcpy(tmp, in + 16 * i, length%16);
		xor(tmp, out, out);	
		rk_aes_encrypt(out, out, ks1);
	}
	return ret;
}

static int compare_string(unsigned char *a, unsigned char *b, unsigned int len)
{
	unsigned int i;
	
	if((a == NULL) || (b == NULL))
		return -1;

	for (i = 0; i < len; i++){
		if(*a != *b)
			return -1;
		a++;
		b++;
	}
	return 0;
}
int rk_aes_cbc_mac_op(const unsigned char *in, unsigned char *out,
        unsigned long length, const unsigned char *key, const int key_len, 
        const int enc)
{
    RK_AES_KEY ks1;
	int ret = 0;
	unsigned char tmp_out[16]={0};
	unsigned int out_len = 16;
	
	if (in == NULL || out ==NULL || key == NULL)
		return -1;
	
	if (key_len != 128/8 && key_len != 192/8 && key_len != 256/8)
		return -2;
	
	if(length == 0)
		return -3;
	
    ret = rk_aes_set_encrypt_key(key, key_len * 8, &ks1);
	if (ret != 0)
		return ret;

	ret = rk_aes_cbc_mac_encrypt(in, length, tmp_out, &ks1, enc);
	if (ret != 0)
		return ret;
	
	if(enc)
		memcpy(out, tmp_out, out_len);
	else
		ret = compare_string(out,tmp_out,out_len);

	return ret;
}




