#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm4_core.h"


#define CCM_DEBUG 0
#if 1

typedef void (*ccm128_f)(const unsigned char *in, unsigned char *out,
			size_t blocks, const void *key,
			const unsigned char ivec[16],unsigned char cmac[16]);


struct ccm128_context {
	union { u64 u[2]; u8 c[16]; } nonce, cmac;
	u64 blocks;
	block128_f block;
	void *key;
};

//#define U64(C) C##UL

typedef struct ccm128_context CCM128_CONTEXT;


/* First you setup M and L parameters and pass the key schedule.
 * This is called once per session setup... */
static void rk_crypto_ccm128_init(CCM128_CONTEXT *ctx,
	unsigned int M,unsigned int L,void *key,block128_f block)
{
//	printf("m = %d,L = %d\n",M,L);
	memset(ctx->nonce.c,0,sizeof(ctx->nonce.c));
	ctx->nonce.c[0] = ((u8)(L-1)&7) | (u8)(((M-2)/2)&7)<<3;
	ctx->blocks = 0;
	ctx->block = block;
	ctx->key = key;
}

/* !!! Following interfaces are to be called *once* per packet !!! */

/* Then you setup per-message nonce and pass the length of the message */
static int  rk_crypto_ccm128_setiv(CCM128_CONTEXT *ctx,
	const unsigned char *nonce,size_t nlen,size_t mlen)
{
	unsigned int L = ctx->nonce.c[0]&7;	/* the L parameter */

	if (nlen<(14-L)) return -1;		/* nonce is too short */

	if (sizeof(mlen)==8 && L>=3) {
		ctx->nonce.c[8]  = (u8)(mlen>>(56%(sizeof(mlen)*8)));
		ctx->nonce.c[9]  = (u8)(mlen>>(48%(sizeof(mlen)*8)));
		ctx->nonce.c[10] = (u8)(mlen>>(40%(sizeof(mlen)*8)));
		ctx->nonce.c[11] = (u8)(mlen>>(32%(sizeof(mlen)*8)));
	}
	else
		ctx->nonce.u[1] = 0;

	ctx->nonce.c[12] = (u8)(mlen>>24);
	ctx->nonce.c[13] = (u8)(mlen>>16);
	ctx->nonce.c[14] = (u8)(mlen>>8);
	ctx->nonce.c[15] = (u8)mlen;

	ctx->nonce.c[0] &= ~0x40;	/* clear Adata flag */
	memcpy(&ctx->nonce.c[1],nonce,14-L);

	return 0;
}

/* Then you pass additional authentication data, this is optional */
static void  rk_crypto_ccm128_aad(CCM128_CONTEXT *ctx,
	const unsigned char *aad,size_t alen)
{	unsigned int i;
	block128_f block = ctx->block;

	if (alen==0) return;

	ctx->nonce.c[0] |= 0x40;	/* set Adata flag */
	(*block)(ctx->nonce.c,ctx->cmac.c,ctx->key),
	ctx->blocks++;

	if (alen<(0x10000-0x100)) {
		ctx->cmac.c[0] ^= (u8)(alen>>8);
		ctx->cmac.c[1] ^= (u8)alen;
		i=2;
	}
	else if (sizeof(alen)==8 && alen>=(size_t)1<<(32%(sizeof(alen)*8))) {
		ctx->cmac.c[0] ^= 0xFF;
		ctx->cmac.c[1] ^= 0xFF;
		ctx->cmac.c[2] ^= (u8)(alen>>(56%(sizeof(alen)*8)));
		ctx->cmac.c[3] ^= (u8)(alen>>(48%(sizeof(alen)*8)));
		ctx->cmac.c[4] ^= (u8)(alen>>(40%(sizeof(alen)*8)));
		ctx->cmac.c[5] ^= (u8)(alen>>(32%(sizeof(alen)*8)));
		ctx->cmac.c[6] ^= (u8)(alen>>24);
		ctx->cmac.c[7] ^= (u8)(alen>>16);
		ctx->cmac.c[8] ^= (u8)(alen>>8);
		ctx->cmac.c[9] ^= (u8)alen;
		i=10;
	}
	else {
		ctx->cmac.c[0] ^= 0xFF;
		ctx->cmac.c[1] ^= 0xFE;
		ctx->cmac.c[2] ^= (u8)(alen>>24);
		ctx->cmac.c[3] ^= (u8)(alen>>16);
		ctx->cmac.c[4] ^= (u8)(alen>>8);
		ctx->cmac.c[5] ^= (u8)alen;
		i=6;
	}

	do {
		for(;i<16 && alen;++i,++aad,--alen)
			ctx->cmac.c[i] ^= *aad;
		(*block)(ctx->cmac.c,ctx->cmac.c,ctx->key),
		ctx->blocks++;
		i=0;
	} while (alen);
}

/* Finally you encrypt or decrypt the message */

/* counter part of nonce may not be larger than L*8 bits,
 * L is not larger than 8, therefore 64-bit counter... */
static void  rk_ctr64_inc(unsigned char *counter) {
	unsigned int n=8;
	u8  c;

	counter += 8;
	do {
		--n;
		c = counter[n];
		++c;
		counter[n] = c;
		if (c) return;
	} while (n);
}

int  rk_crypto_ccm128_encrypt(CCM128_CONTEXT *ctx,
	const unsigned char *inp, unsigned char *out,
	size_t len)
{
	size_t		n;
	unsigned int	i,L;
	unsigned char	flags0	= ctx->nonce.c[0];
	block128_f	block	= ctx->block;
	void *		key	= ctx->key;
	union { u64 u[2]; u8 c[16]; } scratch;

	if (!(flags0&0x40))
		(*block)(ctx->nonce.c,ctx->cmac.c,key),
		ctx->blocks++;

	ctx->nonce.c[0] = L = flags0&7;
	for (n=0,i=15-L;i<15;++i) {
		n |= ctx->nonce.c[i];
		ctx->nonce.c[i]=0;
		n <<= 8;
	}
	n |= ctx->nonce.c[15];	/* reconstructed length */
	ctx->nonce.c[15]=1;

//	printf("n = %d,len = %d\n",n,len);

	if (n!=len) return -1;	/* length mismatch */

	ctx->blocks += ((len+15)>>3)|1;
	if (ctx->blocks > (U64(1)<<61))	return -2; /* too much data */

	while (len>=16) {
#if defined(STRICT_ALIGNMENT)
		union { u64 u[2]; u8 c[16]; } temp;

		memcpy (temp.c,inp,16);
		ctx->cmac.u[0] ^= temp.u[0];
		ctx->cmac.u[1] ^= temp.u[1];
#else
		ctx->cmac.u[0] ^= ((u64*)inp)[0];
		ctx->cmac.u[1] ^= ((u64*)inp)[1];
#endif
		(*block)(ctx->cmac.c,ctx->cmac.c,key);
		(*block)(ctx->nonce.c,scratch.c,key);
		rk_ctr64_inc(ctx->nonce.c);
#if defined(STRICT_ALIGNMENT)
		temp.u[0] ^= scratch.u[0];
		temp.u[1] ^= scratch.u[1];
		memcpy(out,temp.c,16);
#else
		((u64*)out)[0] = scratch.u[0]^((u64*)inp)[0];
		((u64*)out)[1] = scratch.u[1]^((u64*)inp)[1];
#endif
		inp += 16;
		out += 16;
		len -= 16;
	}

	if (len) {
		for (i=0; i<len; ++i) ctx->cmac.c[i] ^= inp[i];
		(*block)(ctx->cmac.c,ctx->cmac.c,key);
		(*block)(ctx->nonce.c,scratch.c,key);
		for (i=0; i<len; ++i) out[i] = scratch.c[i]^inp[i];
	}

	for (i=15-L;i<16;++i)
		ctx->nonce.c[i]=0;

	(*block)(ctx->nonce.c,scratch.c,key);
	ctx->cmac.u[0] ^= scratch.u[0];
	ctx->cmac.u[1] ^= scratch.u[1];

	ctx->nonce.c[0] = flags0;

	return 0;
}

static int  rk_crypto_ccm128_decrypt(CCM128_CONTEXT *ctx,
	const unsigned char *inp, unsigned char *out,
	size_t len)
{
	size_t		n;
	unsigned int	i,L;
	unsigned char	flags0	= ctx->nonce.c[0];
	block128_f	block	= ctx->block;
	void *		key	= ctx->key;
	union { u64 u[2]; u8 c[16]; } scratch;

	if (!(flags0&0x40))
		(*block)(ctx->nonce.c,ctx->cmac.c,key);

	ctx->nonce.c[0] = L = flags0&7;
	for (n=0,i=15-L;i<15;++i) {
		n |= ctx->nonce.c[i];
		ctx->nonce.c[i]=0;
		n <<= 8;
	}
	n |= ctx->nonce.c[15];	/* reconstructed length */
	ctx->nonce.c[15]=1;

//	printf("n = %d,len = %d\n",n,len);

	if (n!=len) return -1;

	while (len>=16) {
#if defined(STRICT_ALIGNMENT)
		union { u64 u[2]; u8 c[16]; } temp;
#endif
		(*block)(ctx->nonce.c,scratch.c,key);
		rk_ctr64_inc(ctx->nonce.c);
#if defined(STRICT_ALIGNMENT)
		memcpy (temp.c,inp,16);
		ctx->cmac.u[0] ^= (scratch.u[0] ^= temp.u[0]);
		ctx->cmac.u[1] ^= (scratch.u[1] ^= temp.u[1]);
		memcpy (out,scratch.c,16);
#else
		ctx->cmac.u[0] ^= (((u64*)out)[0] = scratch.u[0]^((u64*)inp)[0]);
		ctx->cmac.u[1] ^= (((u64*)out)[1] = scratch.u[1]^((u64*)inp)[1]);
#endif
		(*block)(ctx->cmac.c,ctx->cmac.c,key);

		inp += 16;
		out += 16;
		len -= 16;
	}

	if (len) {
		(*block)(ctx->nonce.c,scratch.c,key);
		for (i=0; i<len; ++i)
			ctx->cmac.c[i] ^= (out[i] = scratch.c[i]^inp[i]);
		(*block)(ctx->cmac.c,ctx->cmac.c,key);
	}

	for (i=15-L;i<16;++i)
		ctx->nonce.c[i]=0;

	(*block)(ctx->nonce.c,scratch.c,key);
	ctx->cmac.u[0] ^= scratch.u[0];
	ctx->cmac.u[1] ^= scratch.u[1];

	ctx->nonce.c[0] = flags0;

	return 0;
}

static void  rk_ctr64_add (unsigned char *counter,size_t inc)
{	size_t n=8, val=0;

	counter += 8;
	do {
		--n;
		val += counter[n] + (inc&0xff);
		counter[n] = (unsigned char)val;
		val >>= 8;	/* carry bit */
		inc >>= 8;
	} while(n && (inc || val));
}

static int  rk_crypto_ccm128_encrypt_ccm64(CCM128_CONTEXT *ctx,
	const unsigned char *inp, unsigned char *out,
	size_t len,ccm128_f stream)
{
	size_t		n;
	unsigned int	i,L;
	unsigned char	flags0	= ctx->nonce.c[0];
	block128_f	block	= ctx->block;
	void *		key	= ctx->key;
	union { u64 u[2]; u8 c[16]; } scratch;

	if (!(flags0&0x40))
		(*block)(ctx->nonce.c,ctx->cmac.c,key),
		ctx->blocks++;

	ctx->nonce.c[0] = L = flags0&7;
	for (n=0,i=15-L;i<15;++i) {
		n |= ctx->nonce.c[i];
		ctx->nonce.c[i]=0;
		n <<= 8;
	}
	n |= ctx->nonce.c[15];	/* reconstructed length */
	ctx->nonce.c[15]=1;

	if (n!=len) return -1;	/* length mismatch */

	ctx->blocks += ((len+15)>>3)|1;
	if (ctx->blocks > (U64(1)<<61))	return -2; /* too much data */

	n=len/16;
	if (n) {
		(*stream)(inp,out,n,key,ctx->nonce.c,ctx->cmac.c);
		n   *= 16;
		inp += n;
		out += n;
		len -= n;
		if (len) rk_ctr64_add(ctx->nonce.c,n/16);
	}

	if (len) {
		for (i=0; i<len; ++i) ctx->cmac.c[i] ^= inp[i];
		(*block)(ctx->cmac.c,ctx->cmac.c,key);
		(*block)(ctx->nonce.c,scratch.c,key);
		for (i=0; i<len; ++i) out[i] = scratch.c[i]^inp[i];
	}

	for (i=15-L;i<16;++i)
		ctx->nonce.c[i]=0;

	(*block)(ctx->nonce.c,scratch.c,key);
	ctx->cmac.u[0] ^= scratch.u[0];
	ctx->cmac.u[1] ^= scratch.u[1];

	ctx->nonce.c[0] = flags0;

	return 0;
}

static int  rk_crypto_ccm128_decrypt_ccm64(CCM128_CONTEXT *ctx,
	const unsigned char *inp, unsigned char *out,
	size_t len,ccm128_f stream)
{
	size_t		n;
	unsigned int	i,L;
	unsigned char	flags0	= ctx->nonce.c[0];
	block128_f	block	= ctx->block;
	void *		key	= ctx->key;
	union { u64 u[2]; u8 c[16]; } scratch;

	if (!(flags0&0x40))
		(*block)(ctx->nonce.c,ctx->cmac.c,key);

	ctx->nonce.c[0] = L = flags0&7;
	for (n=0,i=15-L;i<15;++i) {
		n |= ctx->nonce.c[i];
		ctx->nonce.c[i]=0;
		n <<= 8;
	}
	n |= ctx->nonce.c[15];	/* reconstructed length */
	ctx->nonce.c[15]=1;

	if (n!=len) return -1;

	n=len/16;
	if (n) {
		(*stream)(inp,out,n,key,ctx->nonce.c,ctx->cmac.c);
		n   *= 16;
		inp += n;
		out += n;
		len -= n;
		if (len) rk_ctr64_add(ctx->nonce.c,n/16);
	}

	if (len) {
		(*block)(ctx->nonce.c,scratch.c,key);
		for (i=0; i<len; ++i)
			ctx->cmac.c[i] ^= (out[i] = scratch.c[i]^inp[i]);
		(*block)(ctx->cmac.c,ctx->cmac.c,key);
	}

	for (i=15-L;i<16;++i)
		ctx->nonce.c[i]=0;

	(*block)(ctx->nonce.c,scratch.c,key);
	ctx->cmac.u[0] ^= scratch.u[0];
	ctx->cmac.u[1] ^= scratch.u[1];

	ctx->nonce.c[0] = flags0;

	return 0;
}

static size_t  rk_crypto_ccm128_tag(CCM128_CONTEXT *ctx,unsigned char *tag,size_t len)
{	unsigned int M = (ctx->nonce.c[0]>>3)&7;	/* the M parameter */

	M *= 2; M += 2;
	if (len<M)	return 0;
	memcpy(tag,ctx->cmac.c,M);
	return M;
}

#endif




/*m is the lengh of tag*/

#if 0
int rk_aes_ccm_op(struct sm4_ae_in *in, struct sm4_ae_out *out, const int enc)
{
	int time = 0;
	int i = 0;
    RK_AES_KEY ks1, ks2;
	CCM128_CONTEXT ctx;
	int ret = 0;
	//unsigned int m = 12;
	unsigned int l = 0;

	if (in->key == NULL || in->iv == NULL || in->src == NULL || in->aad == NULL)
		return -1;

	if (in->key_len!= 128/8 && in->key_len != 192/8 && in->key_len != 256/8)
		return -2;

	if(in->src_len % 16 != 0)
		return -3;

	if(out->dest == NULL || out->tag == NULL)
		return -4;
	printf("-----param sucess-----\n");


	l = out->dest_len;/* dest_len = inlength */

	ret = rk_aes_set_encrypt_key(in->key, in->key_len * 8, &ks1);
	if(ret != 0)
		printf("-----set_encrypt_key fail-----\n");

	rk_crypto_ccm128_init(&ctx, in->tag_size, l, &ks1, (block128_f)rk_aes_encrypt);

	ret = rk_crypto_ccm128_setiv(&ctx, in->iv, in->iv_len, l);/*l ?*/
	if (ret != 0)
		printf("========rk_crypto_ccm128_setiv ret = %d================\n",ret);

	rk_crypto_ccm128_aad(&ctx, in->aad, in->aad_len);

	if(enc){
		if((ret = rk_crypto_ccm128_encrypt(&ctx, in->src , out->dest,in->src_len)) != 0)
			printf("=========rk_crypto_ccm128_encrypt ret = %d===\n",ret);
		rk_crypto_ccm128_tag(&ctx, out->tag, 12); /*tag is length*/
		}
	else{if((ret = rk_crypto_ccm128_decrypt(&ctx, out->dest, in->src, out->dest_len)) != 0)
			printf("=========rk_crypto_ccm128_decrypt ret = %d===\n",ret);

		}

	printf("----op done------------");

	 return 0;
}
#endif

static int compare_string(char *a, char *b, unsigned int len)
{
	unsigned int i;

	if((len <= 0) || (a == NULL) || (b == NULL))
		return -1;

	for (i = 0; i < len; i++){
		if(*a != *b)
			return -1;
		a++;
		b++;
	}
	return 0;
}


static void dump_hex(char *var_name, unsigned char *data, unsigned int len)
{
	unsigned int i;
	printf("LINE:%d  %s:", __LINE__, var_name);
	for (i = 0; i < len; i++) {
		if(i % 16 == 0)
			printf("\n");
		printf("%02x  ", data[i]);
	}
	printf("\n");
}

int rk_sm4_ccm_op(struct sm4_ae_in *in, struct sm4_ae_out *out, const int enc)
{
    sm4_context sm4_ctx;
	CCM128_CONTEXT ctx;
	int ret = 0;

	unsigned int m = 0;
	unsigned int l = 0;

	unsigned char tag_tmp[16]= {0};

	if (in->key == NULL || in->iv == NULL || in->src == NULL || in->aad == NULL)
		return -1;

	if (in->key_len!= 16)
		return -2;

	if(in->src_len % 16 != 0)
		return -3;

	if(out->dest == NULL || out->tag == NULL)
		return -4;

	m = in->tag_size;
//	tag_tmp = malloc(m);

	l = 15 - in->iv_len;  /* l + iv_len = 15 */
	rk_sm4_setkey_enc(&sm4_ctx, in->key);
	if(ret != 0)
		printf("-----set_encrypt_key fail-----\n");


	/* M  :tag size ,L  = 8? src_Len*/
	rk_crypto_ccm128_init(&ctx, in->tag_size, l, &sm4_ctx, rk_rk_sm4_crypt_ecb);



//	ret = rk_crypto_ccm128_setiv(&ctx, in->iv, in->iv_len, l);/*l ?*/
	ret = rk_crypto_ccm128_setiv(&ctx, in->iv, in->iv_len, in->src_len);


	if (ret != 0)
		printf("========rk_crypto_ccm128_setiv ret = %d================\n",ret);

	rk_crypto_ccm128_aad(&ctx, in->aad, in->aad_len);

	/* verify tag */
	if (enc == 2){
		if((ret = rk_crypto_ccm128_encrypt(&ctx, in->src , out->dest,in->src_len)) != 0)
			printf("=========rk_crypto_ccm128_encrypt ret = %d===\n",ret);

		rk_crypto_ccm128_tag(&ctx, tag_tmp, m); /*tag is length*/
		ret = compare_string((char*)tag_tmp,out->tag,m);
		if (ret!=0){
			dump_hex("verify tag_tmp:",tag_tmp,m);
			dump_hex("verify out->tag:",out->tag,m);
			printf("=======ccm verify failed========\n");
			return ret;
		}
		return ret;
	}
	if(enc == 1){
		if((ret = rk_crypto_ccm128_encrypt(&ctx, in->src , out->dest,in->src_len)) != 0)
			printf("=========rk_crypto_ccm128_encrypt ret = %d===\n",ret);


		rk_crypto_ccm128_tag(&ctx, out->tag, m); /*tag is length*/
#if CCM_DEBUG
		dump_hex("rk: out->dest:",out->dest,in->src_len);
		dump_hex("cmm --out->tag:",out->tag,m);
#endif
	}
	if(enc == 0){
		if((ret = rk_crypto_ccm128_decrypt(&ctx, out->dest, in->src, out->dest_len)) != 0)
			printf("=========rk_crypto_ccm128_decrypt ret = %d===\n",ret);

	}

	return ret;
}



