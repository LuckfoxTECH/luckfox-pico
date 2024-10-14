#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"

struct ctr_state {
	unsigned char ivec[SM4_BLOCK_SIZE];
	unsigned char ecount[SM4_BLOCK_SIZE];
	unsigned int num;
};

static void rk_init_ctr(struct ctr_state * state, const unsigned char *iv)
{
	state->num = 0;
	memset(state->ecount, 0, SM4_BLOCK_SIZE);
	memcpy(state->ivec, iv, SM4_BLOCK_SIZE);
}

/* increment counter (128-bit int) by 1 */
static void rk_ctr128_inc(unsigned char *counter) {
	unsigned int n=SM4_BLOCK_SIZE;
	unsigned char  c;

	do {
		--n;
		c = counter[n];
		++c;
		counter[n] = c;
		if (c) return;
	} while (n);
}

static void rk_crypto_ctr128_encrypt(void *ctx, const unsigned char *in, unsigned char *out,
			unsigned int len, unsigned char *ivec, unsigned char *ecount_buf,
			unsigned int *num, block128_f block)
{
	unsigned int n, l=0;

	n = *num;
	while (l<len) {
		if (n==0) {
			(*block)(ivec, ecount_buf, ctx);
 			rk_ctr128_inc(ivec);
		}
		out[l] = in[l] ^ ecount_buf[n];
		++l;
		n = (n+1) % SM4_BLOCK_SIZE;
	}

	*num = n;
}

int rk_sm4_ctr_encrypt(const unsigned char *in, unsigned char *out,
        unsigned int length, const unsigned char *key, const int key_len,
        unsigned char *ivec, const int enc)
{
	sm4_context  ctx;
	struct ctr_state state;

	if(key_len != 16)
		return -1;

	if(length == 0)
		return -1;

	rk_init_ctr(&state, ivec);
	rk_sm4_setkey_enc(&ctx, key);
	rk_crypto_ctr128_encrypt((void*)(&ctx),in, out, length,
				state.ivec, state.ecount,
				&state.num, rk_rk_sm4_crypt_ecb);

	return 0;
}


