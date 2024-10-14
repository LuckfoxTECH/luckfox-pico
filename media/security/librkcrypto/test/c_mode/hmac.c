#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aes_core.h"
#include "hash_md5.h"
#include "hash_sha1.h"
#include "hash_sha256.h"
#include "hash_sha512.h"
#include "hash_sm3.h"
#define PARTITION_BYTE_SIZE 64
typedef int (*hash_f)(const unsigned char *in, unsigned int in_len, 
							unsigned char *out, unsigned int *out_len);

#if 0
static int rk_hmac_64(unsigned char *key, unsigned int key_len, const unsigned char *in, 
					unsigned int in_len, unsigned char *out, unsigned int *out_len, hash_f hash)
{
	int res = -1;
	unsigned int i, hash_len, tmp_key_len;
	unsigned char *tmp = NULL;
	unsigned char tmp_key[PARTITION_BYTE_SIZE];
	unsigned char ipad[PARTITION_BYTE_SIZE], opad[PARTITION_BYTE_SIZE];
	unsigned char istr[PARTITION_BYTE_SIZE], ostr[PARTITION_BYTE_SIZE];

	if(key == NULL || in == NULL || out == NULL || out_len == NULL)
		return -1;

	memset(ipad, 0x36, sizeof(ipad));
	memset(opad, 0x5c, sizeof(opad));
	memset(istr, 0x00, sizeof(istr));
	memset(ostr, 0x00, sizeof(ostr));
	memset(tmp_key, 0x00, sizeof(tmp_key));
	
	tmp_key_len = key_len;
	if(key_len > PARTITION_BYTE_SIZE){
		res = (*hash)(key, key_len, tmp_key, &tmp_key_len);
		if(res != 0){
			goto exit;
		}
	}else{
		memcpy(tmp_key, key, key_len);
	}

	memcpy(istr, tmp_key, tmp_key_len);
	memcpy(ostr, tmp_key, tmp_key_len);

	for(i=0; i<key_len; i++)
		istr[i] ^= ipad[i];

	for(i=0; i<key_len; i++)
		ostr[i] ^= opad[i];

	tmp = (unsigned char*)malloc(PARTITION_BYTE_SIZE+in_len);
	if(tmp == NULL){
		res = -1;
		goto exit;
	}

	memcpy(tmp, istr, sizeof(istr));
	memcpy(tmp+sizeof(istr), in, in_len);
	memset(istr, 0x00, sizeof(istr));
	res = (*hash)(tmp, PARTITION_BYTE_SIZE+in_len, istr, &hash_len);
	if(res != 0){
		goto exit;
	}
	free(tmp);

	tmp = (unsigned char*)malloc(PARTITION_BYTE_SIZE+hash_len);
	if(tmp == NULL){
		res = -1;
		goto exit;
	}

	memcpy(tmp, opad, sizeof(opad));
	memcpy(tmp+sizeof(opad), istr, hash_len);
	res = (*hash)(tmp, PARTITION_BYTE_SIZE+hash_len, out, out_len);

exit:
	if(tmp)
		free(tmp);
	return res;
}
#endif
static int rk_hmac_64(const unsigned char *key, unsigned int key_length,  const unsigned char *data, unsigned int data_length,
                unsigned char *digest, unsigned int *digest_len, hash_f hash)
  
{  
	int res = -1;
    unsigned int b = 64; /* blocksize */  
    unsigned char ipad = 0x36;  
    unsigned char opad = 0x5c;   
    unsigned char k0[64];  
    unsigned char k0xorIpad[64];  
    unsigned char step7data[64];  
    unsigned char *step5data = NULL;  
    unsigned char step8data[64+64];
	unsigned int tmp_len = 0;
    unsigned int i;  
  
    for (i=0; i<64; i++)  
    {  
        k0[i] = 0x00;  
    }
  
    if (key_length != b)    /* Step 1 */  
    {  
        /* Step 2 */  
        if (key_length > b)        
        {  
            res = (*hash)(key, key_length, digest, &tmp_len);
			if(res != 0){
				goto exit;
			}
			
            for (i=0;i<tmp_len;i++)  
            {  
                k0[i]=digest[i];  
            }  
        }  
        else if (key_length < b)  /* Step 3 */  
        {  
            for (i=0; i<key_length; i++)  
            {  
                k0[i] = key[i];  
            }  
        }  
    }  
    else  
    {  
        for (i=0;i<b;i++)  
        {  
            k0[i] = key[i];  
        }  
    }  

    /* Step 4 */  
    for (i=0; i<64; i++)  
    {  
        k0xorIpad[i] = k0[i] ^ ipad;  
    }  

	step5data = malloc(64+data_length);
	if(step5data == NULL){
		res = -1;
		goto exit;
	}
    /* Step 5 */  
    for (i=0; i<64; i++)  
    {  
        step5data[i] = k0xorIpad[i];  
    }  
    for (i=0;i<data_length;i++)  
    {  
        step5data[i+64] = data[i];  
    }  
 
  
    /* Step 6 */  
    res = (*hash)(step5data, data_length+b, digest, &tmp_len);
	if(res != 0){
		goto exit;
	}
  
    /* Step 7 */  
    for (i=0; i<64; i++)  
    {  
        step7data[i] = k0[i] ^ opad;  
    }  

  
    /* Step 8 */  
    for (i=0;i<64;i++)  
    {  
        step8data[i] = step7data[i];  
    }  
    for (i=0;i<tmp_len;i++)  
    {  
        step8data[i+64] = digest[i];  
    }  
  
    /* Step 9 */  
    res = (*hash)(step8data, b+tmp_len, digest, &tmp_len);
	if(res != 0){
		goto exit;
	}
	*digest_len = tmp_len;

exit:
	if(step5data)
		free(step5data);
	return res;
}  

static int rk_hmac_128(const unsigned char *key, unsigned int key_length,  const unsigned char *data, unsigned int data_length,  
                unsigned char *digest, unsigned int *digest_len, hash_f hash)
  
{  
	int res = -1;
    unsigned int b = 128; /* blocksize */  
    unsigned char ipad = 0x36;  
    unsigned char opad = 0x5c;   
    unsigned char k0[128];  
    unsigned char k0xorIpad[128];  
    unsigned char step7data[128];  
    unsigned char *step5data = NULL;  
    unsigned char step8data[128+128];
	unsigned int tmp_len = 0;
    unsigned int i;  
  
    for (i=0; i<128; i++)  
    {  
        k0[i] = 0x00;  
    }
  
    if (key_length != b)    /* Step 1 */  
    {  
        /* Step 2 */  
        if (key_length > b)        
        {  
            res = (*hash)(key, key_length, digest, &tmp_len);
			if(res != 0){
				goto exit;
			}
			
            for (i=0;i<tmp_len;i++)  
            {  
                k0[i]=digest[i];  
            }  
        }  
        else if (key_length < b)  /* Step 3 */  
        {  
            for (i=0; i<key_length; i++)  
            {  
                k0[i] = key[i];  
            }  
        }  
    }  
    else  
    {  
        for (i=0;i<b;i++)  
        {  
            k0[i] = key[i];  
        }  
    }  

    /* Step 4 */  
    for (i=0; i<128; i++)  
    {  
        k0xorIpad[i] = k0[i] ^ ipad;  
    }  

	step5data = malloc(128+data_length);
	if(step5data == NULL){
		res = -1;
		goto exit;
	}
    /* Step 5 */  
    for (i=0; i<128; i++)  
    {  
        step5data[i] = k0xorIpad[i];  
    }  
    for (i=0;i<data_length;i++)  
    {  
        step5data[i+128] = data[i];  
    }  
 
  
    /* Step 6 */  
    res = (*hash)(step5data, data_length+b, digest, &tmp_len);
	if(res != 0){
		goto exit;
	}
  
    /* Step 7 */  
    for (i=0; i<128; i++)  
    {  
        step7data[i] = k0[i] ^ opad;  
    }  

  
    /* Step 8 */  
    for (i=0;i<128;i++)  
    {  
        step8data[i] = step7data[i];  
    }  
    for (i=0;i<tmp_len;i++)  
    {  
        step8data[i+128] = digest[i];  
    }  
  
    /* Step 9 */  
    res = (*hash)(step8data, b+tmp_len, digest, &tmp_len);
	if(res != 0){
		goto exit;
	}
	*digest_len = tmp_len;

exit:
	if(step5data)
		free(step5data);
	return res;
}  


int rk_hmac_md5(const unsigned char *key, unsigned int key_len, const unsigned char *in,
					unsigned int in_len, unsigned char *out, unsigned int *out_len)
{
	if(key_len > 64)
		return -1;

	return rk_hmac_64(key, key_len, in, in_len, out, out_len, rk_hash_md5);
}

int rk_hmac_sha1(const unsigned char *key, unsigned int key_len, const unsigned char *in,
					unsigned int in_len, unsigned char *out, unsigned int *out_len)
{
	if(key_len > 64)
		return -1;
		
	return rk_hmac_64(key, key_len, in, in_len, out, out_len, rk_hash_sha1);
}

int rk_hmac_sha256(const unsigned char *key, unsigned int key_len, const unsigned char *in,
					unsigned int in_len, unsigned char *out, unsigned int *out_len)
{
	if(key_len > 64)
		return -1;

	return rk_hmac_64(key, key_len, in, in_len, out, out_len, rk_hash_sha256);
}

int rk_hmac_sha512(const unsigned char *key, unsigned int key_len, const unsigned char *in,
					unsigned int in_len, unsigned char *out, unsigned int *out_len)
{
	if(key_len > 128)
		return -1;

	return rk_hmac_128(key, key_len, in, in_len, out, out_len, rk_hash_sha512);
}

int rk_hmac_sm3(const unsigned char *key, unsigned int key_len, const unsigned char *in,
					unsigned int in_len, unsigned char *out, unsigned int *out_len)
{
	if(key_len > 64)
		return -1;

	return rk_hmac_64(key, key_len, in, in_len, out, out_len, rk_hash_sm3);
}

