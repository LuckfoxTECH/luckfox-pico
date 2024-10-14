
#include <memory.h>
#include <openssl/evp.h>

// ----- KDF FUNCTIONS START -----
//typedef void *(*KDF)(const void *in, size_t inlen, void *out, size_t *outlen);

int x9_63_kdf(const EVP_MD *md, const unsigned char *share, size_t sharelen, size_t keylen, unsigned char *outkey)
{
	int ret = 0;

	EVP_MD_CTX ctx;
	unsigned char counter[4] = {0, 0, 0, 1};
	unsigned char dgst[EVP_MAX_MD_SIZE];
	unsigned int dgstlen;
	int rlen = (int)keylen;
	unsigned char * pp;

	pp = outkey;

	if (keylen > (size_t)EVP_MD_size(md)*255)
	{
		fprintf(stderr, "%s(%d):", __FILE__, __LINE__);
		goto end;
	}

	while (rlen > 0)
	{
		EVP_MD_CTX_init(&ctx);

		if (!EVP_DigestInit(&ctx, md))
		{
			fprintf(stderr, "%s(%d):", __FILE__, __LINE__);
			goto end;
		}

		if (!EVP_DigestUpdate(&ctx, share, sharelen))
		{
			fprintf(stderr, "%s(%d):", __FILE__, __LINE__);
			goto end;
		}
		if (!EVP_DigestUpdate(&ctx, counter, 4))
		{
			fprintf(stderr, "%s(%d):", __FILE__, __LINE__);
			goto end;
		}
		if (!EVP_DigestFinal(&ctx, dgst, &dgstlen))
		{
			fprintf(stderr, "%s(%d):", __FILE__, __LINE__);
			goto end;
		}

		EVP_MD_CTX_cleanup(&ctx);

		memcpy(pp, dgst, keylen>=dgstlen ? dgstlen:keylen);

		rlen -= dgstlen;
		pp += dgstlen;
		counter[3]++;
	}

	ret = 1;

end:
	return ret;
}

// ----- KDF FUNCTIONS END -----