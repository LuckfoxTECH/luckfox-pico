#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"

static int xor(const unsigned char *a, const unsigned char *b,unsigned char *c)
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

static int rk_sm4_cbc_mac_encrypt(void *ctx, const unsigned char *in, unsigned int length, unsigned char *out,
		const int enc)
{
	unsigned int i = 0;
	int ret = 0;
	unsigned char tmp[16];
	memset(out, 0x00, 16);
	memset(tmp, 0x00, 16);

	for(i=0; i<(length/16); i++){
		xor(in + 16 * i, out, out);
		rk_sm4_crypt_ecb(ctx, out, out);
	};

	if(length%16){
		memcpy(tmp, in + 16 * i, length%16);
		xor(tmp, out, out);
		rk_sm4_crypt_ecb(ctx, out, out);
	}
	return ret;
}

static int compare_string(char *a, char *b, unsigned int len)
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
int rk_sm4_cbc_mac_op(const unsigned char *in, unsigned char *out,
        unsigned int length, const unsigned char *key, const unsigned int key_len,
        const int enc)
{
    sm4_context ctx;
	int ret = 0;
	unsigned char tmp_out[16]={0};
	unsigned int out_len = 16;

	if (in == NULL || out ==NULL || key == NULL)
		return -1;

	if (key_len != 16)
		return -2;

	if(length == 0)
		return -3;

    rk_sm4_setkey_enc(&ctx, key);
	ret = rk_sm4_cbc_mac_encrypt(&ctx, in, length, tmp_out, enc);

	if(enc)
		memcpy(out, tmp_out, out_len);
	else
		ret = compare_string((char*)out, (char*)tmp_out,out_len);

	return ret;
}




