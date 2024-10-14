#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "aes_core.h"

static void rk_left_shift(int len, unsigned char* add, unsigned char*des)  
{  
    int i;  
    for (i = 0; i < len - 1; i++)  
    {  
        des[i] = (add[i] << 1) + (add[i + 1] >= 0x80?1:0);  
    }  
    des[len - 1] = add[len - 1] << 1;  
}  
  
static void rk_array_xor(int len, const unsigned char*a1, const unsigned char*a2, unsigned char*des)
{  
    int i;  
    for (i = 0; i < len; i++)  
    {  
        des[i] = a1[i] ^ a2[i];  
    }  
}  
  
static void rk_derive_mac_key(RK_AES_KEY *key, unsigned char *k1, unsigned char *k2)  
{
    unsigned char plain[AES_BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  
    unsigned char Rb[AES_BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87 };  
    unsigned char c0[AES_BLOCK_SIZE];  
         
    rk_aes_encrypt(plain, c0, key);
    if (c0[0]<0x80)    //generate k1  
    {  
        rk_left_shift(AES_BLOCK_SIZE, c0, k1);  
    }  
    else  
    {  
        rk_left_shift(AES_BLOCK_SIZE, c0, k1);  
        rk_array_xor(AES_BLOCK_SIZE, k1, Rb, k1);  
    }  
  
    if (k1[0] < 0x80)   //generate k2  
    {  
        rk_left_shift(AES_BLOCK_SIZE, k1, k2);  
    }  
    else  
    {  
        rk_left_shift(AES_BLOCK_SIZE, k1, k2);  
        rk_array_xor(AES_BLOCK_SIZE, k2, Rb, k2);  
    }  
}  
  
int rk_aes_genarate_cmac(const unsigned char *key, unsigned int key_len, const unsigned char *msg, unsigned int msg_len, unsigned char *macvalue)
{  
    int i,block; 
    unsigned char IVtemp[AES_BLOCK_SIZE];  
    unsigned char Blocktemp[AES_BLOCK_SIZE];
	unsigned char k1[AES_BLOCK_SIZE], k2[AES_BLOCK_SIZE];
	RK_AES_KEY aes_key;
	int result;

  	memset(IVtemp, 0x00, sizeof(IVtemp));
	memset(Blocktemp, 0x00, sizeof(Blocktemp));
	memset(k1, 0x00, sizeof(k1));
	memset(k2, 0x00, sizeof(k2));
	
	result = rk_aes_set_encrypt_key(key, key_len*8, &aes_key);
	if(result != 0)
		return result;
	
	rk_derive_mac_key(&aes_key, k1, k2);
    if (msg_len % AES_BLOCK_SIZE == 0 && msg_len!=0)  
    {  
        block = msg_len / AES_BLOCK_SIZE;  
        for (i = 0; i < block-1; i++)  
        {  
            rk_array_xor(16, &msg[i * AES_BLOCK_SIZE], IVtemp, Blocktemp); 
			rk_aes_encrypt(Blocktemp, IVtemp, &aes_key); 
        }  
        rk_array_xor(16, &msg[(block-1)*AES_BLOCK_SIZE], IVtemp, Blocktemp);  
        rk_array_xor(16, Blocktemp, k1, Blocktemp);
		rk_aes_encrypt(Blocktemp, macvalue, &aes_key);
    }  
    else  
    {  
        if (msg_len==0)  
        {  
            block = 1;  
            Blocktemp[0] = 0x80;//padding the first bit with 1  
            rk_array_xor(16, Blocktemp, k2, Blocktemp);
			rk_aes_encrypt(Blocktemp, macvalue, &aes_key);
        }  
        else  
        {  
            unsigned char remain = msg_len % AES_BLOCK_SIZE;  
            block = msg_len / AES_BLOCK_SIZE + 1;  
            for (i = 0; i < block - 1; i++)  
            {  
                rk_array_xor(AES_BLOCK_SIZE, &msg[i * AES_BLOCK_SIZE], IVtemp, Blocktemp);
				rk_aes_encrypt(Blocktemp, IVtemp, &aes_key);
            }  
            // the last block padding  
            for (i = 0; i < remain; i++)   
            {  
                Blocktemp[i] = msg[(block - 1) * AES_BLOCK_SIZE + i];  
            }  
            Blocktemp[remain] = 0x80;  
            for (i = remain + 1; i < AES_BLOCK_SIZE; i++)  
            {  
                Blocktemp[i] = 0;  
            }  
            // end of the last block padding              
              
            rk_array_xor(AES_BLOCK_SIZE, Blocktemp, k2, Blocktemp);  
            rk_array_xor(AES_BLOCK_SIZE, Blocktemp, IVtemp, Blocktemp);
			rk_aes_encrypt(Blocktemp, macvalue, &aes_key);
        }  
  
    } 
	return 0;
}  

int rk_aes_verify_cmac(const unsigned char *key, unsigned int key_len, const unsigned char *msg, unsigned int msg_len, unsigned char *macvalue)
{  
    int i, block;
	int result=-1; 
	unsigned char IVtemp[AES_BLOCK_SIZE];  
    unsigned char Blocktemp[AES_BLOCK_SIZE];
	unsigned char k1[AES_BLOCK_SIZE], k2[AES_BLOCK_SIZE];
	unsigned char tmp_macvalue[AES_BLOCK_SIZE];  
	RK_AES_KEY aes_key;

  	memset(IVtemp, 0x00, sizeof(IVtemp));
	memset(Blocktemp, 0x00, sizeof(Blocktemp));
	memset(k1, 0x00, sizeof(k1));
	memset(k2, 0x00, sizeof(k2));
     
  	result = rk_aes_set_encrypt_key(key, key_len*8, &aes_key);
	if(result != 0)
		return result;
	
	rk_derive_mac_key(&aes_key, k1, k2);
    if (msg_len % AES_BLOCK_SIZE == 0 && msg_len != 0)  
    {
        block = msg_len / AES_BLOCK_SIZE;  
        for (i = 0; i < block - 1; i++)  
        {  
            rk_array_xor(AES_BLOCK_SIZE, &msg[i * AES_BLOCK_SIZE], IVtemp, Blocktemp);
			rk_aes_encrypt(Blocktemp, IVtemp, &aes_key);
        }  
        rk_array_xor(AES_BLOCK_SIZE, &msg[(block - 1) * AES_BLOCK_SIZE], IVtemp, Blocktemp);  
        rk_array_xor(AES_BLOCK_SIZE, Blocktemp, k1, Blocktemp);
		rk_aes_encrypt(Blocktemp, tmp_macvalue, &aes_key); 
    }  
    else  
    {  
        if (msg_len == 0)  
        {  
            block = 1;  
            Blocktemp[0] = 0x80;//padding the first bit with 1  
            rk_array_xor(AES_BLOCK_SIZE, Blocktemp, k2, Blocktemp);  
            rk_aes_encrypt(Blocktemp, tmp_macvalue, &aes_key);  
        }  
        else  
        {  
            unsigned char remain = msg_len % AES_BLOCK_SIZE;  
            block = msg_len / AES_BLOCK_SIZE + 1;  
            for (i = 0; i < block - 1; i++)  
            {  
                rk_array_xor(AES_BLOCK_SIZE, &msg[i * AES_BLOCK_SIZE], IVtemp, Blocktemp);  
                rk_aes_encrypt(Blocktemp, IVtemp, &aes_key); 
            }  
            // the last block padding  
            for (i = 0; i < remain; i++)  
            {  
                Blocktemp[i] = msg[(block - 1) * AES_BLOCK_SIZE + i];  
            }  
            Blocktemp[remain] = 0x80;  
            for (i = remain + 1; i < AES_BLOCK_SIZE; i++)  
            {  
                Blocktemp[i] = 0;  
            }  
            // end of the last block padding              
  
            rk_array_xor(AES_BLOCK_SIZE, Blocktemp, k2, Blocktemp);  
            rk_array_xor(AES_BLOCK_SIZE, Blocktemp, IVtemp, Blocktemp);
			rk_aes_encrypt(Blocktemp, tmp_macvalue, &aes_key); 
        }  
  
    }  
    result = -1;  
    for (i = 0; i < AES_BLOCK_SIZE; i++)  
    {  
        if (tmp_macvalue[i] != macvalue[i])  
        {  
            return(result);  
        }  
    }  
    result = 0;  
    return(result);  
}  
