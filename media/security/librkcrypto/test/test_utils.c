#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rkcrypto_common.h"
#include "test_utils.h"

bool is_no_multi_blocksize(uint32_t mode)
{
	if (mode == RK_CIPHER_MODE_CTS ||
	    mode == RK_CIPHER_MODE_CTR ||
	    mode == RK_CIPHER_MODE_CFB ||
	    mode == RK_CIPHER_MODE_OFB)
		return true;
	else
		return false;
}

void test_get_rng(uint8_t *trn, uint32_t len)
{
	static int init_flag;

	if (!init_flag) {
		srand(time(NULL));
		init_flag = 1;
	}

	for (uint32_t i = 0; i < len; i++)
		trn[i] = rand() & 0xff;
}

void test_dump_hex(char *var_name, const uint8_t *data, uint32_t len)
{
	uint32_t i;
	char buffer[256];
	char *p;

	printf("=================== %s [%u] ================\n", var_name, len);

	p = buffer;
	for (i = 0; i < len; i++) {
		if (i % 16 == 0 && i != 0) {
			printf("%s\n", buffer);
			p = buffer;
			memset(buffer, 0x00, sizeof(buffer));
		}
		p += snprintf(p, 256, "%02x ", data[i]);
	}
	printf("%s\n", buffer);
}

struct test_name_map {
	uint32_t	id;
	const char	*name;
};

static const struct test_name_map algo_map_tbl[] = {
	{RK_ALGO_AES,         "AES"},
	{RK_ALGO_DES,         "DES"},
	{RK_ALGO_TDES,        "TDES"},
	{RK_ALGO_SM4,         "SM4"},

	{RK_ALGO_MD5,         "MD5"},
	{RK_ALGO_SHA1,        "SHA1"},
	{RK_ALGO_SHA256,      "SHA256"},
	{RK_ALGO_SHA224,      "SHA224"},
	{RK_ALGO_SHA512,      "SHA512"},
	{RK_ALGO_SHA384,      "SHA384"},
	{RK_ALGO_SHA512_224,  "SHA512_224"},
	{RK_ALGO_SHA512_256,  "SHA512_256"},
	{RK_ALGO_SM3,         "SM3"},

	{RK_ALGO_HMAC_MD5,    "HMAC_MD5"},
	{RK_ALGO_HMAC_SHA1,   "HMAC_SHA1"},
	{RK_ALGO_HMAC_SHA256, "HMAC_SHA256"},
	{RK_ALGO_HMAC_SHA512, "HMAC_SHA512"},
	{RK_ALGO_HMAC_SM3,    "HMAC_SM3"},
	{RK_ALGO_CMAC_SM4,    "CMAC_SM4"},
	{RK_ALGO_CBCMAC_SM4,  "CBCMAC_SM4"},
	{RK_ALGO_CMAC_AES,    "CMAC_AES"},
	{RK_ALGO_CBCMAC_AES,  "CBCMAC_AES"},
};

static const struct test_name_map mode_map_tbl[] = {
	{RK_CIPHER_MODE_ECB,     "ECB"},
	{RK_CIPHER_MODE_CBC,     "CBC"},
	{RK_CIPHER_MODE_CTS,     "CTS"},
	{RK_CIPHER_MODE_CTR,     "CTR"},
	{RK_CIPHER_MODE_CFB,     "CFB"},
	{RK_CIPHER_MODE_OFB,     "OFB"},
	{RK_CIPHER_MODE_XTS,     "XTS"},
	{RK_CIPHER_MODE_CCM,     "CCM"},
	{RK_CIPHER_MODE_GCM,     "GCM"},
};

static const struct test_name_map ops_map_tbl[] = {
	{RK_OP_CIPHER_ENC,       "ENCRYPT"},
	{RK_OP_CIPHER_DEC,       "DECRYPT"},
};

static const char *get_name_from_map(uint32_t id, const struct test_name_map *map, uint32_t map_cnt)
{
	uint32_t i;

	for (i = 0; i < map_cnt; i++, map++) {
		if (map->id == id)
			return map->name;
	}

	return "Unknown";
}

const char *test_algo_name(uint32_t algo)
{
	return get_name_from_map(algo, &algo_map_tbl[0], ARRAY_SIZE(algo_map_tbl));
}

const char *test_mode_name(uint32_t mode)
{
	return get_name_from_map(mode, &mode_map_tbl[0], ARRAY_SIZE(mode_map_tbl));
}

const char *test_op_name(uint32_t operation)
{
	return get_name_from_map(operation, &ops_map_tbl[0], ARRAY_SIZE(ops_map_tbl));
}

