// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2019, Linaro Limited
 */

#include <ctype.h>
#include <endian.h>
#ifdef OPENSSL_FOUND
#include <openssl/evp.h>
#endif
#include <stdint.h>
#include <string.h>
#include <tee_api_types.h>
#include <tee_client_api.h>
#include "xtest_uuid_helpers.h"

static int hex(char c)
{
	char lc = tolower(c);

	if (isdigit(lc))
		return lc - '0';
	if (isxdigit(lc))
		return lc - 'a' + 10;
	return -1;
}

static uint32_t parse_hex(const char *s, size_t nchars, uint32_t *res)
{
	uint32_t v = 0;
	size_t n = 0;
	int c = 0;

	for (n = 0; n < nchars; n++) {
		c = hex(s[n]);
		if (c == -1) {
			*res = TEE_ERROR_BAD_FORMAT;
			goto out;
		}
		v = (v << 4) + c;
	}
	*res = TEE_SUCCESS;
out:
	return v;
}

TEEC_Result xtest_uuid_from_str(TEEC_UUID *uuid, const char *s)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_UUID u = { };
	const char *p = s;
	size_t i = 0;

	if (!p || strnlen(p, 37) != 36)
		return TEEC_ERROR_BAD_FORMAT;
	if (p[8] != '-' || p[13] != '-' || p[18] != '-' || p[23] != '-')
		return TEEC_ERROR_BAD_FORMAT;

	u.timeLow = parse_hex(p, 8, &res);
	if (res)
		goto out;
	p += 9;
	u.timeMid = parse_hex(p, 4, &res);
	if (res)
		goto out;
	p += 5;
	u.timeHiAndVersion = parse_hex(p, 4, &res);
	if (res)
		goto out;
	p += 5;
	for (i = 0; i < 8; i++) {
		u.clockSeqAndNode[i] = parse_hex(p, 2, &res);
		if (res)
			goto out;
		if (i == 1)
			p += 3;
		else
			p += 2;
	}
	*uuid = u;
out:
	return res;
}

#ifdef OPENSSL_FOUND
TEEC_Result xtest_uuid_v5(TEEC_UUID *uuid, const TEEC_UUID *ns,
			  const void *name, size_t size)
{
	TEEC_Result res = TEEC_SUCCESS;
	EVP_MD_CTX *mdctx = NULL;
	const EVP_MD *md = NULL;
	unsigned char hash[EVP_MAX_MD_SIZE] = { };
	unsigned int md_len = 0;
	unsigned char nsbe[16] = { };
	uint32_t be32 = 0;
	uint16_t be16 = 0;
	int ret = 0;

	/* Convert from host to big endian */
	be32 = htobe32(ns->timeLow);
	memcpy(&nsbe[0], &be32, sizeof(be32));
	be16 = htobe16(ns->timeMid);
	memcpy(&nsbe[4], &be16, sizeof(be16));
	be16 = htobe16(ns->timeHiAndVersion);
	memcpy(&nsbe[6], &be16, sizeof(be16));
	memcpy(&nsbe[8], &ns->clockSeqAndNode, sizeof(ns->clockSeqAndNode));

	mdctx = EVP_MD_CTX_create();
	if (!mdctx)
		return TEEC_ERROR_OUT_OF_MEMORY;
	md = EVP_sha1();
	if (!md) {
		res = TEEC_ERROR_NOT_SUPPORTED;
		goto out;
	}
	ret = EVP_DigestInit_ex(mdctx, md, NULL);
	if (!ret) {
		res = TEEC_ERROR_GENERIC;
		goto out;
	}
	ret = EVP_DigestUpdate(mdctx, nsbe, sizeof(nsbe));
	if (!ret) {
		res = TEEC_ERROR_GENERIC;
		goto out;
	}
	ret = EVP_DigestUpdate(mdctx, name, size);
	if (!ret) {
		res = TEEC_ERROR_GENERIC;
		goto out;
	}
	ret = EVP_DigestFinal_ex(mdctx, hash, &md_len);
	if (!ret) {
		res = TEEC_ERROR_GENERIC;
		goto out;
	}

	/* Mark it as UUIDv5 */
	hash[6] = (hash[6] & 0x0F) | 0x50;
	hash[8] = (hash[8] & 0x3F) | 0x80;

	/* Convert from big endian to host */
	memcpy(&be32, &hash[0], sizeof(uint32_t));
	uuid->timeLow = be32toh(be32);
	memcpy(&be16, &hash[4], sizeof(uint16_t));
	uuid->timeMid = be16toh(be16);
	memcpy(&be16, &hash[6], sizeof(uint16_t));
	uuid->timeHiAndVersion = be16toh(be16);
	memcpy(uuid->clockSeqAndNode, &hash[8], sizeof(uuid->clockSeqAndNode));
out:
	EVP_MD_CTX_destroy(mdctx);
	return res;
}
#endif /*OPENSSL_FOUND*/
