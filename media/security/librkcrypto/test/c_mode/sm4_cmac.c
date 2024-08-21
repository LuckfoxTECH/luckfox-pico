#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"

static void rk_left_shift(int len, unsigned char* add, unsigned char*des)
{
    int i;
    for (i = 0; i < len - 1; i++)
    {
        des[i] = (add[i] << 1) + (add[i + 1] >= 0x80?1:0);
    }
    des[len - 1] = add[len - 1] << 1;
}

static void rk_array_xor(int len, unsigned char*a1, unsigned char*a2, unsigned char*des)
{
    int i;
    for (i = 0; i < len; i++)
    {
        des[i] = a1[i] ^ a2[i];
    }
}

static void rk_derive_mac_key(sm4_context *key, unsigned char *k1, unsigned char *k2)
{
    unsigned char plain[SM4_BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char Rb[SM4_BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87 };
    unsigned char c0[SM4_BLOCK_SIZE];

    rk_sm4_crypt_ecb(key, plain, c0);
    if (c0[0]<0x80)    //generate k1
    {
        rk_left_shift(SM4_BLOCK_SIZE, c0, k1);
    }
    else
    {
        rk_left_shift(SM4_BLOCK_SIZE, c0, k1);
        rk_array_xor(SM4_BLOCK_SIZE, k1, Rb, k1);
    }

    if (k1[0] < 0x80)   //generate k2
    {
        rk_left_shift(SM4_BLOCK_SIZE, k1, k2);
    }
    else
    {
        rk_left_shift(SM4_BLOCK_SIZE, k1, k2);
        rk_array_xor(SM4_BLOCK_SIZE, k2, Rb, k2);
    }
}

int rk_sm4_genarate_cmac(unsigned char *key, unsigned int key_len, unsigned char *msg, unsigned int msg_len, unsigned char *macvalue)
{
    int i,block;
    unsigned char IVtemp[SM4_BLOCK_SIZE];
    unsigned char Blocktemp[SM4_BLOCK_SIZE];
	unsigned char k1[SM4_BLOCK_SIZE], k2[SM4_BLOCK_SIZE];
	sm4_context ctx;

  	memset(IVtemp, 0x00, sizeof(IVtemp));
	memset(Blocktemp, 0x00, sizeof(Blocktemp));
	memset(k1, 0x00, sizeof(k1));
	memset(k2, 0x00, sizeof(k2));

	rk_sm4_setkey_enc(&ctx, key);

	rk_derive_mac_key(&ctx, k1, k2);
    if (msg_len % SM4_BLOCK_SIZE == 0 && msg_len!=0)
    {
        block = msg_len / SM4_BLOCK_SIZE;
        for (i = 0; i < block-1; i++)
        {
            rk_array_xor(16, &msg[i * SM4_BLOCK_SIZE], IVtemp, Blocktemp);
			rk_sm4_crypt_ecb(&ctx, Blocktemp, IVtemp);
        }
        rk_array_xor(16, &msg[(block-1)*SM4_BLOCK_SIZE], IVtemp, Blocktemp);
        rk_array_xor(16, Blocktemp, k1, Blocktemp);
		rk_sm4_crypt_ecb(&ctx, Blocktemp, macvalue);
    }
    else
    {
        if (msg_len==0)
        {
            block = 1;
            Blocktemp[0] = 0x80;//padding the first bit with 1
            rk_array_xor(16, Blocktemp, k2, Blocktemp);
			rk_sm4_crypt_ecb(&ctx, Blocktemp, macvalue);
        }
        else
        {
            unsigned char remain = msg_len % SM4_BLOCK_SIZE;
            block = msg_len / SM4_BLOCK_SIZE + 1;
            for (i = 0; i < block - 1; i++)
            {
                rk_array_xor(SM4_BLOCK_SIZE, &msg[i * SM4_BLOCK_SIZE], IVtemp, Blocktemp);
				rk_sm4_crypt_ecb(&ctx, Blocktemp, IVtemp);
            }
            // the last block padding
            for (i = 0; i < remain; i++)
            {
                Blocktemp[i] = msg[(block - 1) * SM4_BLOCK_SIZE + i];
            }
            Blocktemp[remain] = 0x80;
            for (i = remain + 1; i < SM4_BLOCK_SIZE; i++)
            {
                Blocktemp[i] = 0;
            }
            // end of the last block padding

            rk_array_xor(SM4_BLOCK_SIZE, Blocktemp, k2, Blocktemp);
            rk_array_xor(SM4_BLOCK_SIZE, Blocktemp, IVtemp, Blocktemp);
			rk_sm4_crypt_ecb(&ctx, Blocktemp, macvalue);
        }

    }
	return 0;
}

int rk_sm4_verify_cmac(unsigned char *key, unsigned int key_len, unsigned char *msg, unsigned int msg_len, unsigned char *macvalue)
{
    int i, block;
	int result=-1;
	unsigned char IVtemp[SM4_BLOCK_SIZE];
    unsigned char Blocktemp[SM4_BLOCK_SIZE];
	unsigned char k1[SM4_BLOCK_SIZE], k2[SM4_BLOCK_SIZE];
	unsigned char tmp_macvalue[SM4_BLOCK_SIZE];
	sm4_context ctx;

  	memset(IVtemp, 0x00, sizeof(IVtemp));
	memset(Blocktemp, 0x00, sizeof(Blocktemp));
	memset(k1, 0x00, sizeof(k1));
	memset(k2, 0x00, sizeof(k2));

  	rk_sm4_setkey_enc(&ctx, key);

	rk_derive_mac_key(&ctx, k1, k2);
    if (msg_len % SM4_BLOCK_SIZE == 0 && msg_len != 0)
    {
        block = msg_len / SM4_BLOCK_SIZE;
        for (i = 0; i < block - 1; i++)
        {
            rk_array_xor(SM4_BLOCK_SIZE, &msg[i * SM4_BLOCK_SIZE], IVtemp, Blocktemp);
			rk_sm4_crypt_ecb(&ctx, Blocktemp, IVtemp);
        }
        rk_array_xor(SM4_BLOCK_SIZE, &msg[(block - 1) * SM4_BLOCK_SIZE], IVtemp, Blocktemp);
        rk_array_xor(SM4_BLOCK_SIZE, Blocktemp, k1, Blocktemp);
		rk_sm4_crypt_ecb(&ctx, Blocktemp, tmp_macvalue);
    }
    else
    {
        if (msg_len == 0)
        {
            block = 1;
            Blocktemp[0] = 0x80;//padding the first bit with 1
            rk_array_xor(SM4_BLOCK_SIZE, Blocktemp, k2, Blocktemp);
            rk_sm4_crypt_ecb(&ctx, Blocktemp, tmp_macvalue);
        }
        else
        {
            unsigned char remain = msg_len % SM4_BLOCK_SIZE;
            block = msg_len / SM4_BLOCK_SIZE + 1;
            for (i = 0; i < block - 1; i++)
            {
                rk_array_xor(SM4_BLOCK_SIZE, &msg[i * SM4_BLOCK_SIZE], IVtemp, Blocktemp);
                rk_sm4_crypt_ecb(&ctx, Blocktemp, IVtemp);
            }
            // the last block padding
            for (i = 0; i < remain; i++)
            {
                Blocktemp[i] = msg[(block - 1) * SM4_BLOCK_SIZE + i];
            }
            Blocktemp[remain] = 0x80;
            for (i = remain + 1; i < SM4_BLOCK_SIZE; i++)
            {
                Blocktemp[i] = 0;
            }
            // end of the last block padding

            rk_array_xor(SM4_BLOCK_SIZE, Blocktemp, k2, Blocktemp);
            rk_array_xor(SM4_BLOCK_SIZE, Blocktemp, IVtemp, Blocktemp);
			rk_sm4_crypt_ecb(&ctx, Blocktemp, tmp_macvalue);
        }

    }
    result = -1;
    for (i = 0; i < SM4_BLOCK_SIZE; i++)
    {
        if (tmp_macvalue[i] != macvalue[i])
        {
            return(result);
        }
    }
    result = 0;
    return(result);
}
