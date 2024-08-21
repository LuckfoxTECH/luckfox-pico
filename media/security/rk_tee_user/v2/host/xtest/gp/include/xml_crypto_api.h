/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * Copyright (c) 2020, Linaro Limited
 */

#ifndef XML_CRYPTO_API_H_
#define XML_CRYPTO_API_H_

#include <assert.h>
#include <err.h>
#include <openssl/bn.h>
#include <openssl/dsa.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#include <openssl/rsa.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tee_api_defines.h>
#include <tee_client_api.h>
#include <unistd.h>

#define CMD_DS_AllocateTransientObject			0x10000001
#define CMD_DS_StoreBuffer				0x1F0000CB
#define CMD_DS_InitRefAttribute				0x10000006
#define CMD_DS_GetObjectBufferAttribute			0x10000008
#define CMD_DS_FreeTransientObject			0x10000004
#define CMD_DS_RestrictObjectUsage1			0x10000003

#include "xml_common_api.h"
#include "xtest_helpers.h"
#include "xtest_test.h"

#define declare_local_vars \
	uint32_t iHandleFlags1 __maybe_unused = 0; \
	uint32_t iHandleFlags2 __maybe_unused = 0; \
	uint32_t iAttributeList1 __maybe_unused = 0; \
	uint32_t iObjectUsage1 __maybe_unused = 0; \
	const uint32_t iAttributeListEmpty __maybe_unused = 0; \
	const uint32_t iHandleFlagsNone __maybe_unused = 0;

#define set_flag(vector, flag)  do { (vector) |= (flag); } while (0)
#define clear_flag(vector, flag)  do { (vector) &= ~(flag); } while (0)

#define set_handle_flag(vector, flag)	set_flag((vector), (flag))
#define clear_handle_flag(vector, flag)	clear_flag((vector), (flag))
#define add_attribute(vector, id)	set_flag((vector), 1ul << (id))
#define set_usage_flag(vector, flag)	set_flag((vector), (flag))

#define BIT_CHANGE(a, b) ((a) ^= BIT((b)))

#define CRYPTO_INIT(b) \
	b.buffer = NULL; \
	b.size = 0;

#define CRYPTO_MALLOC(b, size) \
	b.size = size; \
	b.buffer = malloc(size);

#define CRYPTO_FREE(b) { \
		if (b.buffer != NULL) { \
			b.size = 0; \
			free(b.buffer); \
			b.buffer = NULL; \
		} }

#define CMD_Crypto_InitObjectWithKeys               0x00010001
#define CMD_Crypto_FreeAllKeysAndOperations         0x00010002
#define CMD_Crypto_AllocateOperation                0x00010003
#define CMD_Crypto_FreeOperation                    0x00010004
#define CMD_Crypto_ResetOperation                   0x00010005
#define CMD_Crypto_SetOperationKey                  0x00010006
#define CMD_Crypto_SetOperationKey2                 0x00010007
#define CMD_Crypto_CopyOperation                    0x00010008
#define CMD_Crypto_GetOperationInfo                 0x00010009
#define CMD_Crypto_DigestUpdate                     0x0001000A
#define CMD_Crypto_DigestDoFinal                    0x0001000B
#define CMD_Crypto_CipherInit                       0x0001000C
#define CMD_Crypto_CipherUpdate                     0x0001000D
#define CMD_Crypto_CipherDoFinal                    0x0001000E
#define CMD_Crypto_MACInit                          0x0001000F
#define CMD_Crypto_MACUpdate                        0x00010010
#define CMD_Crypto_MACComputeFinal                  0x00010011
#define CMD_Crypto_MACCompareFinal                  0x00010012
#define CMD_Crypto_AsymmetricEncrypt                0x00010013
#define CMD_Crypto_AsymmetricDecrypt                0x00010014
#define CMD_Crypto_AsymmetricSignDigest             0x00010015
#define CMD_Crypto_AsymmetricVerifyDigest           0x00010016
#define CMD_Crypto_AEInit                           0x00010017
#define CMD_Crypto_AEUpdate                         0x00010018
#define CMD_Crypto_AEUpdateAAD                      0x00010019
#define CMD_Crypto_AEEncryptFinal                   0x0001001A
#define CMD_Crypto_AEDecryptFinal                   0x0001001B
#define CMD_Crypto_DeriveKey                        0x0001001D
#define CMD_Crypto_GenerateRandom                   0x0001001E
#define CMD_Crypto_GetOperationInfoMultiple         0x0001001F
#define CMD_Crypto_TTAEnsureIntermediateBufferSize  0x00010020
#define CMD_Crypto_InitObjectWithKeysExt            0x00010021

/*Missing TEE Error codes*/
#define TEE_ERROR_TOO_SHORT_BUFFER  TEE_ERROR_SHORT_BUFFER

/*Other defines*/
#define TEE_USAGE_NONE      0

#define ATTRIBUTE_01	0
#define ATTRIBUTE_02	1
#define ATTRIBUTE_03	2
#define ATTRIBUTE_04	3
#define ATTRIBUTE_05	4
#define ATTRIBUTE_06	5
#define ATTRIBUTE_NONE	0xFFFFFFFF

#define BUFFER_01	0
#define BUFFER_02	1
#define BUFFER_03	2
#define BUFFER_04	3
#define BUFFER_05	4
#define BUFFER_06	5

#define BIG_SIZE		1024
#define DS_BIG_SIZE		16384

/*ALL_OBJECT_SIZES*/
#define KEY_SIZE_TOO_LARGE		4096
#define SIZE_AES_192			192
#define SIZE_AES_256			256
#define SIZE_DES3_128			128
#define SIZE_DES3_192			192
#define SIZE_DES_64			64
#define SIZE_DH_KEYPAIR_1024		1024
#define SIZE_DIGEST_MD5_16		16
#define SIZE_DIGEST_SHA1_20		20
#define SIZE_DIGEST_SHA224_28		28
#define SIZE_DIGEST_SHA256_32		32
#define SIZE_DIGEST_SHA384_48		48
#define SIZE_DIGEST_SHA512_64		64
#define SIZE_DSA_SHA1_KEYPAIR_768	768
#define SIZE_DSA_SHA1_PUBLIC_KEY_768	768
#define SIZE_DSA_SHA224_KEYPAIR_2048	2048
#define SIZE_DSA_SHA224_PUBLIC_KEY_2048	2048
#define SIZE_DSA_SHA256_KEYPAIR_2048	2048
#define SIZE_DSA_SHA256_KEYPAIR_3072	3072
#define SIZE_DSA_SHA256_PUBLIC_KEY_2048	2048
#define SIZE_DSA_SHA256_PUBLIC_KEY_3072	3072
#define SIZE_ECDH_P192			192
#define SIZE_ECDH_P224			224
#define SIZE_ECDH_P256			256
#define SIZE_ECDH_P384			384
#define SIZE_ECDH_P521			521
#define SIZE_ECDSA_P192			192
#define SIZE_ECDSA_P224			224
#define SIZE_ECDSA_P256			256
#define SIZE_ECDSA_P384			384
#define SIZE_ECDSA_P521			521
#define SIZE_GENERIC_SECRET_2048	2048
#define SIZE_HMAC_MD5_256		256
#define SIZE_HMAC_SHA1_256		256
#define SIZE_HMAC_SHA224_256		256
#define SIZE_HMAC_SHA256_512		512
#define SIZE_HMAC_SHA384_512		512
#define SIZE_HMAC_SHA512_512		512
#define SIZE_RSA_KEYPAIR_1024		1024
#define SIZE_RSA_KEYPAIR_2048		2048
#define SIZE_RSA_PUBLIC_KEY_2048	2048
#define SIZE_ZERO			0
#define WRONG_ECC_SIZE			10
#define WRONG_SIZE			5

/*ALL_TEE_TAG_LENGTH_FOR_AES*/
#define AES_104_bits                104
#define AES_112_bits                112
#define AES_120_bits                120
#define AES_128_bits                128
#define AES_32_bits                 32
#define AES_48_bits                 48
#define AES_64_bits                 64
#define AES_80_bits                 80
#define AES_96_bits                 96

/*attribute not defined*/
#define TEE_ATTR_NONE               0

#define AE_TAG_BUFFER_SIZE_32		32
#define BIG_ATTRIBUTE_BUFFER_SIZE	512
#define BUFFER_BIG_SIZE			BIG_SIZE

#define OBJECT_HANDLE_NULL	0
#define OBJECT_HANDLE_01	1
#define OBJECT_HANDLE_02	2
#define OBJECT_HANDLE_03	3
#define OBJECT_HANDLE_INVALID	4

#define OPERATION_HANDLE_NULL		0
#define OPERATION_HANDLE_01		1
#define OPERATION_HANDLE_02		2
#define OPERATION_HANDLE_INVALID	4

static TEEC_SharedMemory share_mem[6];
static TEEC_SharedMemory *SHARE_MEM01 = share_mem;
static TEEC_SharedMemory *SHARE_MEM02 = share_mem + 1;
static TEEC_SharedMemory *SHARE_MEM03 = share_mem + 2;
static TEEC_SharedMemory *SHARE_MEM04 = share_mem + 3;
static TEEC_SharedMemory *SHARE_MEM05 = share_mem + 4;
static TEEC_SharedMemory *SHARE_MEM06 = share_mem + 5;
static TEEC_Session session[2];
static TEEC_Session *SESSION01 = session;
static TEEC_Session *SESSION02 = session + 1;
static TEEC_Context context[2];
static TEEC_Context *CONTEXT01 = context;
static TEEC_Context *CONTEXT02 = context + 1;

/*ALL_ATTRIBUTE_VALUES*/
static const uint8_t TEE_ATTR_AES_256_VALUE01[] = {
	0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0,
	0x85, 0x7d, 0x77, 0x81,
	0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3,
	0x09, 0x14, 0xdf, 0xf4
};
static const uint8_t TEE_ATTR_DH_PRIME_VALUE01[] = {
	0xe0, 0x01, 0xe8, 0x96, 0x7d, 0xb4, 0x93, 0x53, 0xe1, 0x6f, 0x8e, 0x89,
	0x22, 0x0c, 0xce, 0xfc,
	0x5c, 0x5f, 0x12, 0xe3, 0xdf, 0xf8, 0xf1, 0xd1, 0x49, 0x90, 0x12, 0xe6,
	0xef, 0x53, 0xe3, 0x1f,
	0x02, 0xea, 0xcc, 0x5a, 0xdd, 0xf3, 0x37, 0x89, 0x35, 0xc9, 0x5b, 0x21,
	0xea, 0x3d, 0x6f, 0x1c,
	0xd7, 0xce, 0x63, 0x75, 0x52, 0xec, 0x38, 0x6c, 0x0e, 0x34, 0xf7, 0x36,
	0xad, 0x95, 0x17, 0xef,
	0xfe, 0x5e, 0x4d, 0xa7, 0xa8, 0x6a, 0xf9, 0x0e, 0x2c, 0x22, 0x8f, 0xe4,
	0xb9, 0xe6, 0xd8, 0xf8,
	0xf0, 0x2d, 0x20, 0xaf, 0x78, 0xab, 0xb6, 0x92, 0xac, 0xbc, 0x4b, 0x23,
	0xfa, 0xf2, 0xc5, 0xcc,
	0xd4, 0x9a, 0x0c, 0x9a, 0x8b, 0xcd, 0x91, 0xac, 0x0c, 0x55, 0x92, 0x01,
	0xe6, 0xc2, 0xfd, 0x1f,
	0x47, 0xc2, 0xcb, 0x2a, 0x88, 0xa8, 0x3c, 0x21, 0x0f, 0xc0, 0x54, 0xdb,
	0x29, 0x2d, 0xbc, 0x45
};
static const uint8_t TEE_ATTR_DH_BASE_VALUE01[] = {
	0x1c, 0xe0, 0xf6, 0x69, 0x26, 0x46, 0x11, 0x97, 0xef, 0x45, 0xc4, 0x65,
	0x8b, 0x83, 0xb8, 0xab,
	0x04, 0xa9, 0x22, 0x42, 0x68, 0x50, 0x4d, 0x05, 0xb8, 0x19, 0x83, 0x99,
	0xdd, 0x71, 0x37, 0x18,
	0xcc, 0x1f, 0x24, 0x5d, 0x47, 0x6c, 0xcf, 0x61, 0xa2, 0xf9, 0x34, 0x93,
	0xf4, 0x1f, 0x55, 0x52,
	0x48, 0x65, 0x57, 0xe6, 0xd4, 0xca, 0xa8, 0x00, 0xd6, 0xd0, 0xdb, 0x3c,
	0xbf, 0x5a, 0x95, 0x4b,
	0x20, 0x8a, 0x4e, 0xba, 0xf7, 0xe6, 0x49, 0xfb, 0x61, 0x24, 0xd8, 0xa2,
	0x1e, 0xf2, 0xf2, 0x2b,
	0xaa, 0xae, 0x29, 0x21, 0x10, 0x19, 0x10, 0x51, 0x46, 0x47, 0x31, 0xb6,
	0xcc, 0x3c, 0x93, 0xdc,
	0x6e, 0x80, 0xba, 0x16, 0x0b, 0x66, 0x64, 0xa5, 0x6c, 0xfa, 0x96, 0xea,
	0xf1, 0xb2, 0x83, 0x39,
	0x8e, 0xb4, 0x61, 0x64, 0xe5, 0xe9, 0x43, 0x84, 0xee, 0x02, 0x24, 0xe7,
	0x1f, 0x03, 0x7c, 0x23
};
static const uint8_t TEE_ATTR_HMAC_SHA256_512_VALUE01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f
};
static const uint8_t TEE_ATTR_DH_PUBLIC_VALUE_VALUE01[] = {
	0xbb, 0xe9, 0x18, 0xdd, 0x4b, 0x2b, 0x94, 0x1b, 0x10, 0x0e, 0x88, 0x35,
	0x28, 0x68, 0xfc, 0x62,
	0x04, 0x38, 0xa6, 0xdb, 0x32, 0xa6, 0x9e, 0xee, 0x6c, 0x6f, 0x45, 0x1c,
	0xa3, 0xa6, 0xd5, 0x37,
	0x77, 0x75, 0x5b, 0xc1, 0x37, 0x0a, 0xce, 0xfe, 0x2b, 0x8f, 0x13, 0xa9,
	0x14, 0x2c, 0x5b, 0x44,
	0x15, 0x78, 0x86, 0x30, 0xd6, 0x95, 0xb1, 0x92, 0x20, 0x63, 0xa3, 0xcf,
	0x9d, 0xef, 0x65, 0x61,
	0x27, 0x4d, 0x24, 0x01, 0xe7, 0xa1, 0x45, 0xf2, 0xd8, 0xb9, 0x3a, 0x45,
	0x17, 0xf4, 0x19, 0xd0,
	0x5e, 0xf8, 0xcb, 0x35, 0x59, 0x37, 0x9d, 0x04, 0x20, 0xa3, 0xbf, 0x02,
	0xad, 0xfe, 0xa8, 0x60,
	0xb2, 0xc3, 0xee, 0x85, 0x58, 0x90, 0xf3, 0xb5, 0x57, 0x2b, 0xb4, 0xef,
	0xd7, 0x8f, 0x37, 0x68,
	0x78, 0x7c, 0x71, 0x52, 0x9d, 0x5e, 0x0a, 0x61, 0x4f, 0x09, 0x89, 0x92,
	0x39, 0xf7, 0x4b, 0x01
};
static const uint8_t TEE_ATTR_DH_PRIVATE_VALUE_VALUE01[] = {
	0x53, 0x8d, 0x3d, 0x64, 0x27, 0x4a, 0x40, 0x05, 0x9b, 0x9c, 0x26, 0xe9,
	0x13, 0xe6, 0x91, 0x53,
	0x23, 0x7b, 0x55, 0x83
};
static const uint8_t TEE_ATTR_RSA_MODULUS_VALUE01[] = {
	0xf0, 0x1a, 0x95, 0xcd, 0x5f, 0x9f, 0x1c, 0xbc, 0x5c, 0x2e, 0xc8, 0x00,
	0x3b, 0xfa, 0xe0, 0xd5,
	0x72, 0xea, 0xfc, 0x9e, 0x74, 0xe1, 0x02, 0x66, 0xa8, 0x13, 0x3f, 0x0c,
	0xe6, 0x24, 0xcb, 0x1c,
	0xa5, 0xdf, 0x64, 0xfb, 0x06, 0xd7, 0x13, 0xce, 0xaa, 0x6c, 0xee, 0x16,
	0x7b, 0xf8, 0x92, 0xaf,
	0xc4, 0x5b, 0x46, 0x18, 0xc6, 0x30, 0xb6, 0x04, 0x1c, 0x3a, 0x2e, 0xd7,
	0xca, 0xb8, 0xb5, 0x00,
	0x78, 0x89, 0xa0, 0x69, 0x37, 0x84, 0x59, 0x99, 0x0c, 0x2f, 0x00, 0xe5,
	0x3b, 0xe1, 0x18, 0xe0,
	0xb9, 0x2e, 0x77, 0x1d, 0x32, 0x7e, 0x5f, 0xf4, 0x18, 0xf3, 0x9f, 0x58,
	0xc6, 0x83, 0xe2, 0x7a,
	0xcb, 0x89, 0x18, 0xc2, 0x09, 0x84, 0x7e, 0x9d, 0x96, 0xe0, 0xb9, 0x49,
	0x75, 0xef, 0xcf, 0xff,
	0xf0, 0xb6, 0x18, 0xd3, 0x7a, 0xc1, 0x6f, 0x0c, 0x55, 0x33, 0xbe, 0x9d,
	0x63, 0x06, 0xd6, 0x9f,
	0xc1, 0xa5, 0xe9, 0xbd, 0xb1, 0xb2, 0x5d, 0x5c, 0xf9, 0xab, 0xa9, 0xb5,
	0x6a, 0x4e, 0xa4, 0xfa,
	0x44, 0x32, 0xd6, 0x71, 0x2e, 0x5f, 0xa6, 0x25, 0xf8, 0x40, 0x24, 0xc4,
	0x5b, 0x61, 0x55, 0x1b,
	0xac, 0xa3, 0x0a, 0x11, 0x8e, 0x65, 0x20, 0xda, 0x2c, 0x0d, 0xdf, 0xdb,
	0x47, 0x6b, 0x61, 0x18,
	0x4d, 0xfe, 0xfd, 0x2a, 0x7e, 0x77, 0x40, 0x44, 0x43, 0xc6, 0x33, 0x6c,
	0xe5, 0x1b, 0x8d, 0x80,
	0xf9, 0x97, 0xa2, 0xe4, 0xb9, 0x34, 0x3e, 0x28, 0x94, 0x9f, 0xbd, 0xa8,
	0x2b, 0x0a, 0x4d, 0x1a,
	0xa8, 0x06, 0xe5, 0x99, 0x4e, 0xb9, 0x13, 0x45, 0xc8, 0xf6, 0x0f, 0xd0,
	0x4d, 0xbf, 0xe7, 0x8f,
	0xed, 0xca, 0x8e, 0xf8, 0x8d, 0x87, 0x5f, 0xd4, 0xb4, 0x1a, 0x2c, 0xc9,
	0xa7, 0x67, 0x7e, 0xb2,
	0x1b, 0xc1, 0xce, 0xb6, 0x83, 0x7c, 0xce, 0xb4, 0x3d, 0x85, 0xc7, 0x53,
	0x30, 0x7c, 0xfe, 0x85
};
static const uint8_t TEE_ATTR_RSA_PUBLIC_EXPONENT_VALUE01[] = {
	0x01, 0x00, 0x01
};
static const uint8_t TEE_ATTR_DSA_PRIME_768_VALUE01[] = {
	0xf6, 0xad, 0x20, 0x71, 0xe1, 0x5a, 0x4b, 0x9c, 0x2b, 0x7e, 0x53, 0x26,
	0xda, 0x43, 0x9d, 0xc1,
	0x47, 0x4c, 0x1a, 0xd1, 0x6f, 0x2f, 0x85, 0xe9, 0x2c, 0xea, 0x89, 0xfc,
	0xdc, 0x74, 0x66, 0x11,
	0xcf, 0x30, 0xdd, 0xc8, 0x5e, 0x33, 0xf5, 0x83, 0xc1, 0x9d, 0x10, 0xbc,
	0x1a, 0xc3, 0x93, 0x22,
	0x26, 0x24, 0x6f, 0xa7, 0xb9, 0xe0, 0xdd, 0x25, 0x77, 0xb5, 0xf4, 0x27,
	0x59, 0x4c, 0x39, 0xfa,
	0xeb, 0xfc, 0x59, 0x8a, 0x32, 0xe1, 0x74, 0xcb, 0x8a, 0x68, 0x03, 0x57,
	0xf8, 0x62, 0xf2, 0x0b,
	0x6e, 0x84, 0x32, 0xa5, 0x30, 0x65, 0x2f, 0x1c, 0x21, 0x39, 0xae, 0x1f,
	0xaf, 0x76, 0x8b, 0x83
};
static const uint8_t TEE_ATTR_DSA_SUBPRIME_160_VALUE01[] = {
	0x87, 0x44, 0xe4, 0xdd, 0xc6, 0xd0, 0x19, 0xa5, 0xea, 0xc2, 0xb1, 0x5a,
	0x15, 0xd7, 0xe1, 0xc7,
	0xf6, 0x63, 0x35, 0xf7
};
static const uint8_t TEE_ATTR_DSA_BASE_768_VALUE01[] = {
	0x9a, 0x09, 0x32, 0xb3, 0x8c, 0xb2, 0x10, 0x5b, 0x93, 0x00, 0xdc, 0xb8,
	0x66, 0xc0, 0x66, 0xd9,
	0xce, 0xc6, 0x43, 0x19, 0x2f, 0xcb, 0x28, 0x34, 0xa1, 0x23, 0x9d, 0xba,
	0x28, 0xbd, 0x09, 0xfe,
	0x01, 0x00, 0x1e, 0x04, 0x51, 0xf9, 0xd6, 0x35, 0x1f, 0x6e, 0x56, 0x4a,
	0xfb, 0xc8, 0xf8, 0xc3,
	0x9b, 0x10, 0x59, 0x86, 0x3e, 0xbd, 0x09, 0x85, 0x09, 0x0b, 0xd5, 0x5c,
	0x82, 0x8e, 0x9f, 0xc1,
	0x57, 0xac, 0x7d, 0xa3, 0xcf, 0xc2, 0x89, 0x2a, 0x0e, 0xd9, 0xb9, 0x32,
	0x39, 0x05, 0x82, 0xf2,
	0x97, 0x1e, 0x4a, 0x0c, 0x48, 0x3e, 0x06, 0x22, 0xd7, 0x31, 0x66, 0xbf,
	0x62, 0xa5, 0x9f, 0x26
};
static const uint8_t TEE_ATTR_DSA_PRIVATE_VALUE_160_VALUE01[] = {
	0x70, 0x4a, 0x46, 0xc6, 0x25, 0x2a, 0x95, 0xa3, 0x9b, 0x40, 0xe0, 0x43,
	0x5a, 0x69, 0x1b, 0xad,
	0xae, 0x52, 0xa5, 0xc0
};
static const uint8_t TEE_ATTR_DSA_PUBLIC_VALUE_768_VALUE01[] = {
	0x52, 0x9d, 0xed, 0x98, 0xa2, 0x32, 0x09, 0x85, 0xfc, 0x84, 0xb6, 0x5a,
	0x9d, 0xc8, 0xd4, 0xfe,
	0x41, 0xad, 0xa6, 0xe3, 0x59, 0x3d, 0x70, 0x4f, 0x08, 0x98, 0xc1, 0x4e,
	0xc2, 0x46, 0x34, 0xdd,
	0xf5, 0xf1, 0xdb, 0x47, 0xcc, 0x49, 0x15, 0xfc, 0xe1, 0xe2, 0x67, 0x4d,
	0x2e, 0xcd, 0x98, 0xd5,
	0x8b, 0x59, 0x8e, 0x8d, 0xdf, 0xaf, 0xf3, 0x0e, 0x88, 0x26, 0xf5, 0x0a,
	0xab, 0x40, 0x27, 0xb5,
	0xaa, 0xb8, 0x87, 0xc1, 0x9a, 0xd9, 0x6d, 0x7e, 0x57, 0xde, 0x53, 0x90,
	0xad, 0x8e, 0x55, 0x57,
	0xb4, 0x1a, 0x80, 0x19, 0xc9, 0x0d, 0x80, 0x60, 0x71, 0x79, 0xb5, 0x4e,
	0xb0, 0xad, 0x4d, 0x23
};
static const uint8_t TEE_ATTR_DSA_PRIME_2048_VALUE01[] = {
	0xaa, 0x81, 0x5c, 0x9d, 0xb1, 0xc4, 0xd3, 0xd2, 0x77, 0x3c, 0x7d, 0x0d, 0x4d, 0x1d, 0xa7, 0x5e,
	0xcf, 0xc4, 0xa3, 0x9e, 0x97, 0xd5, 0xfa, 0x19, 0x1f, 0xfe, 0xc8, 0xb1, 0x49, 0x0a, 0x29, 0x0c,
	0xe3, 0x35, 0xe5, 0xce, 0x87, 0xea, 0x62, 0x0a, 0x8a, 0x17, 0xde, 0x0b, 0xb6, 0x47, 0x14, 0xe2,
	0xec, 0x84, 0x0b, 0xf0, 0x0e, 0x6e, 0xbd, 0xb4, 0xff, 0xb4, 0xe3, 0x24, 0xca, 0x07, 0xc3, 0xc8,
	0x71, 0x73, 0x09, 0xaf, 0x14, 0x10, 0x36, 0x2a, 0x77, 0x2c, 0x9a, 0xdd, 0x83, 0x8b, 0x2b, 0x0c,
	0xae, 0x1e, 0x90, 0xab, 0x44, 0x8a, 0xda, 0xbd, 0xac, 0xd2, 0xe5, 0xdf, 0x59, 0xc4, 0x18, 0x7a,
	0x32, 0xa2, 0x37, 0x19, 0xd6, 0xc5, 0x7e, 0x94, 0x00, 0x88, 0x53, 0x83, 0xbf, 0x8f, 0x06, 0x6f,
	0x23, 0xb9, 0x41, 0x92, 0x0d, 0x54, 0xc3, 0x5b, 0x4f, 0x7c, 0xc5, 0x04, 0x4f, 0x3b, 0x40, 0xf1,
	0x70, 0x46, 0x95, 0x63, 0x07, 0xb7, 0x48, 0xe8, 0x40, 0x73, 0x28, 0x44, 0xd0, 0x0a, 0x9c, 0xe6,
	0xec, 0x57, 0x14, 0x29, 0x3b, 0x62, 0x65, 0x14, 0x7f, 0x15, 0xc6, 0x7f, 0x4b, 0xe3, 0x8b, 0x08,
	0x2b, 0x55, 0xfd, 0xea, 0xdb, 0x61, 0x24, 0x68, 0x9f, 0xb7, 0x6f, 0x9d, 0x25, 0xcc, 0x28, 0xb8,
	0xea, 0xa9, 0x8b, 0x56, 0x2d, 0x5c, 0x10, 0x11, 0xe0, 0xdc, 0xf9, 0xb3, 0x99, 0x23, 0x24, 0x0d,
	0x33, 0x2d, 0x89, 0xdc, 0x96, 0x03, 0xb7, 0xbd, 0xdd, 0x0c, 0x70, 0xb8, 0x3c, 0xaa, 0x29, 0x05,
	0x63, 0x1b, 0x1c, 0x83, 0xca, 0xbb, 0xae, 0x6c, 0x0c, 0x0c, 0x2e, 0xfe, 0x8f, 0x58, 0x13, 0x1e,
	0xd8, 0x35, 0x1b, 0xf9, 0x3e, 0x87, 0x5f, 0x6a, 0x73, 0xa9, 0x3c, 0xba, 0xd4, 0x70, 0x14, 0x1a,
	0x26, 0x87, 0xfb, 0xac, 0xf2, 0xd7, 0x1c, 0x8d, 0xde, 0xe9, 0x71, 0xad, 0x66, 0x07, 0x29, 0xad,
};
static const uint8_t TEE_ATTR_DSA_SUBPRIME_224_VALUE01[] = {
	0xea, 0x34, 0x7e, 0x90, 0xbe, 0x7c, 0x28, 0x75, 0xd1, 0xfe, 0x1d, 0xb6, 0x22, 0xb4,
	0x76, 0x38, 0x37, 0xc5, 0xe2, 0x7a, 0x60, 0x37, 0x31, 0x03, 0x48, 0xc1, 0xaa, 0x11,
};
static const uint8_t TEE_ATTR_DSA_BASE_2048_VALUE01[] = {
	0x20, 0x42, 0x09, 0x4c, 0xcb, 0xc8, 0xb8, 0x72, 0x3f, 0xc9, 0x28, 0xc1, 0x2f, 0xda, 0x67, 0x1b,
	0x83, 0x29, 0x5e, 0x99, 0xc7, 0x43, 0x57, 0x6f, 0x44, 0x50, 0x4b, 0xe1, 0x18, 0x63, 0x23, 0x31,
	0x9b, 0x50, 0x02, 0xd2, 0x4f, 0x17, 0x3d, 0xf9, 0x09, 0xea, 0x24, 0x1d, 0x6e, 0xa5, 0x28, 0x99,
	0x04, 0xee, 0x46, 0x36, 0x20, 0x4b, 0x2f, 0xbe, 0x94, 0xb0, 0x68, 0xfe, 0x09, 0x3f, 0x79, 0x62,
	0x57, 0x95, 0x49, 0x55, 0x1d, 0x3a, 0xf2, 0x19, 0xad, 0x8e, 0xd1, 0x99, 0x39, 0xef, 0xf8, 0x6b,
	0xce, 0xc8, 0x34, 0xde, 0x2f, 0x2f, 0x78, 0x59, 0x6e, 0x89, 0xe7, 0xcb, 0x52, 0xc5, 0x24, 0xe1,
	0x77, 0x09, 0x8a, 0x56, 0xc2, 0x32, 0xeb, 0x1f, 0x56, 0x3a, 0xa8, 0x4b, 0xc6, 0xb0, 0x26, 0xde,
	0xee, 0x6f, 0xf5, 0x1c, 0xb4, 0x41, 0xe0, 0x80, 0xf2, 0xda, 0xfa, 0xea, 0x1c, 0xed, 0x86, 0x42,
	0x7d, 0x1c, 0x34, 0x6b, 0xe5, 0x5c, 0x66, 0x80, 0x3d, 0x4b, 0x76, 0xd1, 0x33, 0xcd, 0x44, 0x5b,
	0x4c, 0x34, 0x82, 0xfa, 0x41, 0x50, 0x23, 0x46, 0x3c, 0x9b, 0xf3, 0x0f, 0x2f, 0x78, 0x42, 0x23,
	0xe2, 0x60, 0x57, 0xd3, 0xaa, 0x0d, 0x7f, 0xbb, 0x66, 0x06, 0x30, 0xc5, 0x2e, 0x49, 0xd4, 0xa0,
	0x32, 0x5c, 0x73, 0x89, 0xe0, 0x72, 0xaa, 0x34, 0x9f, 0x13, 0xc9, 0x66, 0xe1, 0x59, 0x75, 0x2f,
	0xbb, 0x71, 0xe9, 0x33, 0x68, 0x90, 0xf9, 0x32, 0x43, 0xfa, 0x6e, 0x72, 0xd2, 0x99, 0x36, 0x5e,
	0xe5, 0xb3, 0xfe, 0x26, 0x6e, 0xbf, 0x11, 0x10, 0x56, 0x8f, 0xee, 0x44, 0x25, 0xc8, 0x47, 0xb5,
	0x02, 0x10, 0xbd, 0x48, 0x4b, 0x97, 0x43, 0x1a, 0x42, 0x85, 0x6a, 0xdc, 0xa3, 0xe7, 0xd1, 0xa9,
	0xc9, 0xc6, 0x75, 0xc7, 0xe2, 0x66, 0x91, 0x83, 0x20, 0xdd, 0x5a, 0x78, 0xa4, 0x8c, 0x48, 0xa9,
};
static const uint8_t TEE_ATTR_DSA_PRIVATE_VALUE_224_VALUE01[] = {
	0x7b, 0x48, 0x90, 0x21, 0x57, 0x8e, 0x79, 0xe7, 0xbd, 0x3e, 0xe7, 0xab, 0x45, 0x6f,
	0x65, 0x9f, 0x3d, 0xc0, 0x7c, 0x88, 0xf5, 0xc9, 0xa3, 0x9e, 0x4f, 0x8c, 0xee, 0x81,
};
static const uint8_t TEE_ATTR_DSA_PUBLIC_VALUE_2048_VALUE01[] = {
	0x1a, 0xe1, 0x0c, 0x78, 0x6a, 0xd0, 0x90, 0x2c, 0x5c, 0x68, 0x5d, 0xae, 0x5c, 0x71, 0x21, 0x41,
	0x8a, 0x37, 0x7b, 0x88, 0x8b, 0x5f, 0x2f, 0x2b, 0xc7, 0x66, 0x23, 0x57, 0x0f, 0xd6, 0x2b, 0xcb,
	0x19, 0x0b, 0x47, 0x1a, 0xd5, 0x35, 0x9c, 0x5f, 0x06, 0x2f, 0x88, 0x19, 0x28, 0x9e, 0x95, 0x6d,
	0x8a, 0xa6, 0xf9, 0x0d, 0x1f, 0x8c, 0xf1, 0xee, 0x72, 0xd3, 0xa1, 0xbd, 0xfd, 0x56, 0xc4, 0x78,
	0xdc, 0x29, 0xa1, 0x9c, 0x45, 0x69, 0xb5, 0xa6, 0x0e, 0x3a, 0x8f, 0x34, 0xf6, 0x06, 0x56, 0xea,
	0xc5, 0xb2, 0x5d, 0xde, 0x55, 0x14, 0xa5, 0xc6, 0x7b, 0x67, 0x54, 0x23, 0x20, 0x4f, 0x6c, 0xca,
	0xf0, 0x99, 0x06, 0x17, 0xcc, 0x73, 0x55, 0xb9, 0xd3, 0xed, 0x86, 0x89, 0x78, 0xa2, 0x52, 0x02,
	0x0a, 0x76, 0x9e, 0xd5, 0x9a, 0x6e, 0xda, 0xa6, 0xef, 0xe3, 0x37, 0x7e, 0xef, 0x45, 0xf3, 0xf6,
	0xf3, 0xe6, 0x41, 0x79, 0xcc, 0x7d, 0xb8, 0xb1, 0x43, 0xfb, 0x83, 0x5c, 0x5d, 0x71, 0xbf, 0xcf,
	0xa1, 0xe2, 0xa9, 0x04, 0x9b, 0xcc, 0xf7, 0xfe, 0x9a, 0xb5, 0x75, 0x46, 0x22, 0x0f, 0xe3, 0xf4,
	0xb7, 0x52, 0x1c, 0x86, 0x17, 0x39, 0xd1, 0x38, 0x50, 0x7e, 0x81, 0xa4, 0x6a, 0x69, 0x93, 0x60,
	0x54, 0x41, 0xdc, 0xb9, 0x0d, 0x6e, 0xe4, 0xaf, 0xbc, 0x42, 0xca, 0xbe, 0x90, 0xa2, 0x54, 0x44,
	0x49, 0x68, 0x10, 0x9d, 0x7e, 0xdd, 0x96, 0x94, 0xa0, 0x23, 0x23, 0x9f, 0x1d, 0x56, 0x17, 0x5d,
	0xd1, 0xfa, 0xc1, 0x15, 0x91, 0x5e, 0x24, 0xfa, 0xb5, 0x63, 0xf4, 0xfc, 0x3f, 0x26, 0x9b, 0xed,
	0x2f, 0x30, 0x08, 0x32, 0xd1, 0x12, 0x59, 0x64, 0x85, 0xa7, 0x11, 0x41, 0x7a, 0xa7, 0x3b, 0xb4,
	0xac, 0x72, 0xa6, 0x51, 0xa1, 0xfa, 0x5b, 0xae, 0xd3, 0x63, 0x6c, 0x72, 0x0d, 0x39, 0x70, 0x08,
};

static const uint8_t TEE_ATTR_DSA_PRIME_3072_VALUE01[] = {
	0xc7, 0xb8, 0x6d, 0x70, 0x44, 0x21, 0x8e, 0x36, 0x74, 0x53, 0xd2, 0x10, 0xe7, 0x64, 0x33, 0xe4, 0xe2, 0x7a, 0x98, 0x3d, 0xb1, 0xc5, 0x60, 0xbb,
	0x97, 0x55, 0xa8, 0xfb, 0x7d, 0x81, 0x99, 0x12, 0xc5, 0x6c, 0xfe, 0x00, 0x2a, 0xb1, 0xff, 0x3f, 0x72, 0x16, 0x5b, 0x94, 0x3c, 0x0b, 0x28, 0xed,
	0x46, 0x03, 0x9a, 0x07, 0xde, 0x50, 0x7d, 0x7a, 0x29, 0xf7, 0x38, 0x60, 0x3d, 0xec, 0xd1, 0x27, 0x03, 0x80, 0xa4, 0x1f, 0x97, 0x1f, 0x25, 0x92,
	0x66, 0x1a, 0x64, 0xba, 0x2f, 0x35, 0x1d, 0x9a, 0x69, 0xe5, 0x1a, 0x88, 0x8a, 0x05, 0x15, 0x6b, 0x7f, 0xe1, 0x56, 0x3c, 0x4b, 0x77, 0xee, 0x93,
	0xa4, 0x49, 0x49, 0x13, 0x84, 0x38, 0xa2, 0xab, 0x8b, 0xdc, 0xfc, 0x49, 0xb4, 0xe7, 0x8d, 0x1c, 0xde, 0x76, 0x6e, 0x54, 0x98, 0x47, 0x60, 0x05,
	0x7d, 0x76, 0xcd, 0x74, 0x0c, 0x94, 0xa4, 0xdd, 0x25, 0xa4, 0x6a, 0xa7, 0x7b, 0x18, 0xe9, 0xd7, 0x07, 0xd6, 0x73, 0x84, 0x97, 0xd4, 0xea, 0xc3,
	0x64, 0xf4, 0x79, 0x2d, 0x97, 0x66, 0xa1, 0x6a, 0x0e, 0x23, 0x48, 0x07, 0xe9, 0x6b, 0x8c, 0x64, 0xd4, 0x04, 0xbb, 0xdb, 0x87, 0x6e, 0x39, 0xb5,
	0x79, 0x9e, 0xf5, 0x3f, 0xe6, 0xcb, 0x9b, 0xab, 0x62, 0xef, 0x19, 0xfd, 0xcc, 0x2b, 0xdd, 0x90, 0x5b, 0xed, 0xa1, 0x3b, 0x9e, 0xf7, 0xac, 0x35,
	0xf1, 0xf5, 0x57, 0xcb, 0x0d, 0xc4, 0x58, 0xc0, 0x19, 0xe2, 0xbc, 0x19, 0xa9, 0xf5, 0xdf, 0xc1, 0xe4, 0xec, 0xa9, 0xe6, 0xd4, 0x66, 0x56, 0x41,
	0x24, 0x30, 0x4a, 0x31, 0xf0, 0x38, 0x60, 0x5a, 0x3e, 0x34, 0x2d, 0xa0, 0x1b, 0xe1, 0xc2, 0xb5, 0x45, 0x61, 0x0e, 0xdd, 0x2c, 0x13, 0x97, 0xa3,
	0xc8, 0x39, 0x65, 0x88, 0xc6, 0x32, 0x9e, 0xfe, 0xb4, 0xe1, 0x65, 0xaf, 0x5b, 0x36, 0x8a, 0x39, 0xa8, 0x8e, 0x48, 0x88, 0xe3, 0x9f, 0x40, 0xbb,
	0x3d, 0xe4, 0xeb, 0x14, 0x16, 0x67, 0x2f, 0x99, 0x9f, 0xea, 0xd3, 0x7a, 0xef, 0x1c, 0xa9, 0x64, 0x3f, 0xf3, 0x2c, 0xdb, 0xc0, 0xfc, 0xeb, 0xe6,
	0x28, 0xd7, 0xe4, 0x6d, 0x28, 0x1a, 0x98, 0x9d, 0x43, 0xdd, 0x21, 0x43, 0x21, 0x51, 0xaf, 0x68, 0xbe, 0x3f, 0x6d, 0x56, 0xac, 0xfb, 0xdb, 0x6c,
	0x97, 0xd8, 0x7f, 0xcb, 0x5e, 0x62, 0x91, 0xbf, 0x8b, 0x4e, 0xe1, 0x27, 0x5a, 0xe0, 0xeb, 0x43, 0x83, 0xcc, 0x75, 0x39, 0x03, 0xc8, 0xd2, 0x9f,
	0x4a, 0xdb, 0x6a, 0x54, 0x7e, 0x40, 0x5d, 0xec, 0xdf, 0xf2, 0x88, 0xc5, 0xf6, 0xc7, 0xaa, 0x30, 0xdc, 0xb1, 0x2f, 0x84, 0xd3, 0x92, 0x49, 0x3a,
	0x70, 0x93, 0x33, 0x17, 0xc0, 0xf5, 0xe6, 0x55, 0x26, 0x01, 0xfa, 0xe1, 0x8f, 0x17, 0xe6, 0xe5, 0xbb, 0x6b, 0xf3, 0x96, 0xd3, 0x2d, 0x8a, 0xb9,
};
static const uint8_t TEE_ATTR_DSA_SUBPRIME_256_VALUE01[] = {
	0x87, 0x6f, 0xa0, 0x9e, 0x1d, 0xc6, 0x2b, 0x23, 0x6c, 0xe1, 0xc3, 0x15, 0x5b, 0xa4, 0x8b, 0x0c,
	0xcf, 0xda, 0x29, 0xf3, 0xac, 0x5a, 0x97, 0xf7, 0xff, 0xa1, 0xbd, 0x87, 0xb6, 0x8d, 0x2a, 0x4b,
};
static const uint8_t TEE_ATTR_DSA_BASE_3072_VALUE01[] = {
	0x11, 0x0a, 0xfe, 0xbb, 0x12, 0xc7, 0xf8, 0x62, 0xb6, 0xde, 0x03, 0xd4, 0x7f, 0xdb, 0xc3, 0x32, 0x6e, 0x0d, 0x4d, 0x31, 0xb1, 0x2a, 0x8c, 0xa9,
	0x5b, 0x2d, 0xee, 0x21, 0x23, 0xbc, 0xc6, 0x67, 0xd4, 0xf7, 0x2c, 0x1e, 0x72, 0x09, 0x76, 0x7d, 0x27, 0x21, 0xf9, 0x5f, 0xbd, 0x9a, 0x4d, 0x03,
	0x23, 0x6d, 0x54, 0x17, 0x4f, 0xbf, 0xaf, 0xf2, 0xc4, 0xff, 0x7d, 0xea, 0xe4, 0x73, 0x8b, 0x20, 0xd9, 0xf3, 0x7b, 0xf0, 0xa1, 0x13, 0x4c, 0x28,
	0x8b, 0x42, 0x0a, 0xf0, 0xb5, 0x79, 0x2e, 0x47, 0xa9, 0x25, 0x13, 0xc0, 0x41, 0x3f, 0x34, 0x6a, 0x4e, 0xdb, 0xab, 0x2c, 0x45, 0xbd, 0xca, 0x13,
	0xf5, 0x34, 0x1c, 0x2b, 0x55, 0xb8, 0xba, 0x54, 0x93, 0x2b, 0x92, 0x17, 0xb5, 0xa8, 0x59, 0xe5, 0x53, 0xf1, 0x4b, 0xb8, 0xc1, 0x20, 0xfb, 0xb9,
	0xd9, 0x99, 0x09, 0xdf, 0xf5, 0xea, 0x68, 0xe1, 0x4b, 0x37, 0x99, 0x64, 0xfd, 0x3f, 0x38, 0x61, 0xe5, 0xba, 0x5c, 0xc9, 0x70, 0xc4, 0xa1, 0x80,
	0xee, 0xf5, 0x44, 0x28, 0x70, 0x39, 0x61, 0x02, 0x1e, 0x7b, 0xd6, 0x8c, 0xb6, 0x37, 0x92, 0x7b, 0x8c, 0xbe, 0xe6, 0x80, 0x5f, 0xa2, 0x72, 0x85,
	0xbf, 0xee, 0x4d, 0x1e, 0xf7, 0x0e, 0x02, 0xc1, 0xa1, 0x8a, 0x7c, 0xd7, 0x8b, 0xef, 0x1d, 0xd9, 0xcd, 0xad, 0x45, 0xdd, 0xe9, 0xcd, 0x69, 0x07,
	0x55, 0x05, 0x0f, 0xc4, 0x66, 0x29, 0x37, 0xee, 0x1d, 0x6f, 0x4d, 0xb1, 0x28, 0x07, 0xcc, 0xc9, 0x5b, 0xc4, 0x35, 0xf1, 0x1b, 0x71, 0xe7, 0x08,
	0x60, 0x48, 0xb1, 0xda, 0xb5, 0x91, 0x3c, 0x60, 0x55, 0x01, 0x2d, 0xe8, 0x2e, 0x43, 0xa4, 0xe5, 0x0c, 0xf9, 0x3f, 0xef, 0xf5, 0xdc, 0xab, 0x81,
	0x4a, 0xbc, 0x22, 0x4c, 0x5e, 0x00, 0x25, 0xbd, 0x86, 0x8c, 0x3f, 0xc5, 0x92, 0x04, 0x1b, 0xba, 0x04, 0x74, 0x7c, 0x10, 0xaf, 0x51, 0x3f, 0xc3,
	0x6e, 0x4d, 0x91, 0xc6, 0x3e, 0xe5, 0x25, 0x34, 0x22, 0xcf, 0x40, 0x63, 0x39, 0x8d, 0x77, 0xc5, 0x2f, 0xcb, 0x01, 0x14, 0x27, 0xcb, 0xfc, 0xfa,
	0x67, 0xb1, 0xb2, 0xc2, 0xd1, 0xaa, 0x4a, 0x3d, 0xa7, 0x26, 0x45, 0xcb, 0x1c, 0x76, 0x70, 0x36, 0x05, 0x4e, 0x2f, 0x31, 0xf8, 0x86, 0x65, 0xa5,
	0x44, 0x61, 0xc8, 0x85, 0xfb, 0x32, 0x19, 0xd5, 0xad, 0x87, 0x48, 0xa0, 0x11, 0x58, 0xf6, 0xc7, 0xc0, 0xdf, 0x5a, 0x8c, 0x90, 0x8b, 0xa8, 0xc3,
	0xe5, 0x36, 0x82, 0x24, 0x28, 0x88, 0x6c, 0x7b, 0x50, 0x0b, 0xbc, 0x15, 0xb4, 0x9d, 0xf7, 0x46, 0xb9, 0xde, 0x5a, 0x78, 0xfe, 0x3b, 0x4f, 0x69,
	0x91, 0xd0, 0x11, 0x0c, 0x3c, 0xbf, 0xf4, 0x58, 0x03, 0x9d, 0xc3, 0x62, 0x61, 0xcf, 0x46, 0xaf, 0x4b, 0xc2, 0x51, 0x53, 0x68, 0xf4, 0xab, 0xb7,
};
static const uint8_t TEE_ATTR_DSA_PRIVATE_VALUE_256_VALUE01[] = {
	0x34, 0x70, 0x83, 0x20, 0x55, 0xda, 0xde, 0x94, 0xe1, 0x4c, 0xd8, 0x77, 0x71, 0x71, 0xd1, 0x8e,
	0x5d, 0x06, 0xf6, 0x6a, 0xef, 0xf4, 0xc6, 0x14, 0x71, 0xe4, 0xeb, 0xa7, 0x4e, 0xe5, 0x61, 0x64,
};
static const uint8_t TEE_ATTR_DSA_PUBLIC_VALUE_3072_VALUE01[] = {
	0x45, 0x6a, 0x10, 0x5c, 0x71, 0x35, 0x66, 0x23, 0x48, 0x38, 0xbc, 0x07, 0x0b, 0x8a, 0x75, 0x1a, 0x0b, 0x57, 0x76, 0x7c, 0xb7, 0x5e, 0x99, 0x11,
	0x4a, 0x1a, 0x46, 0x64, 0x1e, 0x11, 0xda, 0x1f, 0xa9, 0xf2, 0x29, 0x14, 0xd8, 0x08, 0xad, 0x71, 0x48, 0x61, 0x2c, 0x1e, 0xa5, 0x5d, 0x25, 0x30,
	0x17, 0x81, 0xe9, 0xae, 0x0c, 0x9a, 0xe3, 0x6a, 0x69, 0xd8, 0x7b, 0xa0, 0x39, 0xec, 0x7c, 0xd8, 0x64, 0xc3, 0xad, 0x09, 0x48, 0x73, 0xe6, 0xe5,
	0x67, 0x09, 0xfd, 0x10, 0xd9, 0x66, 0x85, 0x3d, 0x61, 0x1b, 0x1c, 0xff, 0x15, 0xd3, 0x7f, 0xde, 0xe4, 0x24, 0x50, 0x6c, 0x18, 0x4d, 0x62, 0xc7,
	0x03, 0x33, 0x58, 0xbe, 0x78, 0xc2, 0x25, 0x09, 0x43, 0xb6, 0xf6, 0xd0, 0x43, 0xd6, 0x3b, 0x31, 0x7d, 0xe5, 0x6e, 0x5a, 0xd8, 0xd1, 0xfd, 0x97,
	0xdd, 0x35, 0x5a, 0xbe, 0x96, 0x45, 0x2f, 0x8e, 0x43, 0x54, 0x85, 0xfb, 0x3b, 0x90, 0x7b, 0x51, 0x90, 0x0a, 0xa3, 0xf2, 0x44, 0x18, 0xdf, 0x50,
	0xb4, 0xfc, 0xda, 0xfb, 0xf6, 0x13, 0x75, 0x48, 0xc3, 0x93, 0x73, 0xb8, 0xbc, 0x4b, 0xa3, 0xda, 0xbb, 0x47, 0x46, 0xeb, 0xd1, 0x7b, 0x87, 0xfc,
	0xd6, 0xa2, 0xf1, 0x97, 0xc1, 0x07, 0xb1, 0x8e, 0xc5, 0xb4, 0x65, 0xe6, 0xe4, 0xcb, 0x43, 0x0d, 0x9c, 0x0c, 0xe7, 0x8d, 0xa5, 0x98, 0x84, 0x41,
	0x05, 0x4a, 0x37, 0x07, 0x92, 0xb7, 0x30, 0xda, 0x9a, 0xba, 0x41, 0xa3, 0x16, 0x9a, 0xf2, 0x61, 0x76, 0xf7, 0x4e, 0x6f, 0x7c, 0x0c, 0x9c, 0x9b,
	0x55, 0xb6, 0x2b, 0xbe, 0x7c, 0xe3, 0x8d, 0x46, 0x95, 0xd4, 0x81, 0x57, 0xe6, 0x60, 0xc2, 0xac, 0xb6, 0x3f, 0x48, 0x2f, 0x55, 0x41, 0x81, 0x50,
	0xe5, 0xfe, 0xe4, 0x3a, 0xce, 0x84, 0xc5, 0x40, 0xc3, 0xba, 0x76, 0x62, 0xae, 0x80, 0x83, 0x5c, 0x1a, 0x2d, 0x51, 0x89, 0x0e, 0xa9, 0x6b, 0xa2,
	0x06, 0x42, 0x7c, 0x41, 0xef, 0x8c, 0x38, 0xaa, 0x07, 0xd2, 0xa3, 0x65, 0xe7, 0xe5, 0x83, 0x80, 0xd8, 0xf4, 0x78, 0x2e, 0x22, 0xac, 0x21, 0x01,
	0xaf, 0x73, 0x2e, 0xe2, 0x27, 0x58, 0x33, 0x7b, 0x25, 0x36, 0x37, 0x83, 0x8e, 0x16, 0xf5, 0x0f, 0x56, 0xd3, 0x13, 0xd0, 0x79, 0x81, 0x88, 0x0d,
	0x68, 0x55, 0x57, 0xf7, 0xd7, 0x9a, 0x6d, 0xb8, 0x23, 0xc6, 0x1f, 0x1b, 0xb3, 0xdb, 0xc5, 0xd5, 0x04, 0x21, 0xa4, 0x84, 0x3a, 0x6f, 0x29, 0x69,
	0x0e, 0x78, 0xaa, 0x0f, 0x0c, 0xff, 0x30, 0x42, 0x31, 0x81, 0x8b, 0x81, 0xfc, 0x4a, 0x24, 0x3f, 0xc0, 0x0f, 0x09, 0xa5, 0x4c, 0x46, 0x6d, 0x6a,
	0x8c, 0x73, 0xd3, 0x2a, 0x55, 0xe1, 0xab, 0xd5, 0xec, 0x8b, 0x4e, 0x1a, 0xfa, 0x32, 0xa7, 0x9b, 0x01, 0xdf, 0x85, 0xa8, 0x1f, 0x3f, 0x5c, 0xfe,
};

static const uint8_t TEE_ATTR_DES3_192_VALUE01[] = {
	0xCD, 0xFE, 0x57, 0xB6, 0xB6, 0x2F, 0xAE, 0x6B, 0x04, 0x73, 0x40, 0xF1,
	0x02, 0xD6, 0xA4, 0x8C,
	0x89, 0x5D, 0xAD, 0xF2, 0x9D, 0x62, 0xEF, 0x25
};
static const uint8_t TEE_ATTR_RSA_PRIVATE_EXPONENT_VALUE01[] = {
	0xa5, 0x0d, 0xe1, 0x84, 0xf9, 0x02, 0xec, 0x42, 0x20, 0x2c, 0x98, 0x98,
	0x70, 0xa3, 0x1a, 0x04,
	0x21, 0xa7, 0xa0, 0x59, 0x5d, 0x87, 0x80, 0x9b, 0x09, 0x57, 0x91, 0xb4,
	0x50, 0x51, 0x62, 0xbf,
	0x22, 0xd7, 0xdb, 0x17, 0x25, 0xb0, 0x9c, 0x91, 0x29, 0x5f, 0x10, 0x9c,
	0xac, 0x44, 0x48, 0xb2,
	0x43, 0x8d, 0x6b, 0x36, 0x84, 0xa7, 0xdf, 0xb8, 0x1b, 0x9f, 0x73, 0xac,
	0x2c, 0x53, 0xa5, 0x39,
	0xd9, 0xa2, 0xe2, 0x7e, 0xf2, 0x07, 0x2d, 0x80, 0xa4, 0x7b, 0x7b, 0x66,
	0x1a, 0x2f, 0xb7, 0x66,
	0x64, 0x66, 0xa8, 0xc3, 0x8d, 0x7e, 0x8a, 0x7f, 0xc6, 0xd7, 0x52, 0xe7,
	0x38, 0x30, 0x59, 0x74,
	0x88, 0x8e, 0x8a, 0x52, 0x79, 0x30, 0x77, 0xc9, 0xe5, 0x7a, 0x3e, 0x65,
	0x5d, 0x89, 0xa9, 0xb7,
	0x0b, 0xc6, 0x62, 0x72, 0x9e, 0xa4, 0x72, 0xae, 0x4b, 0xb3, 0xf2, 0x89,
	0x47, 0x15, 0xe0, 0x5b,
	0x45, 0x4d, 0x99, 0x5b, 0x13, 0x6c, 0x90, 0xbe, 0xe5, 0xb5, 0x98, 0xad,
	0x87, 0x99, 0x1a, 0x57,
	0xd4, 0x1f, 0xf1, 0x52, 0x71, 0x5b, 0x51, 0x40, 0xdc, 0x51, 0x35, 0xf6,
	0x6c, 0xae, 0xa3, 0xf9,
	0x0f, 0x3a, 0xed, 0x28, 0xfc, 0xa5, 0x60, 0x2f, 0x4b, 0x4f, 0x31, 0xac,
	0x48, 0x3e, 0x5b, 0xba,
	0xe4, 0x2b, 0x58, 0x79, 0xe6, 0xb4, 0x6b, 0x5e, 0x56, 0x0a, 0xb2, 0xdb,
	0x68, 0xed, 0x24, 0xd8,
	0x5e, 0x6f, 0x30, 0x59, 0x8d, 0x8c, 0xa3, 0x00, 0x68, 0xf5, 0x42, 0x95,
	0x1a, 0x0b, 0xa8, 0x1c,
	0xfb, 0xdf, 0x29, 0x81, 0x10, 0x32, 0x02, 0xcc, 0x51, 0xa4, 0x17, 0x14,
	0x3e, 0xef, 0x89, 0x41,
	0xde, 0xf8, 0x2d, 0x64, 0x69, 0x30, 0xe8, 0x8a, 0xad, 0x96, 0xf6, 0xf4,
	0x82, 0x83, 0x9a, 0x77,
	0xe7, 0xde, 0x12, 0x31, 0xf7, 0x15, 0xec, 0xce, 0xed, 0x83, 0x68, 0x88,
	0x84, 0xe5, 0x64, 0x81
};
static const uint8_t TEE_ATTR_DES_64_VALUE01[] = {
	0xCD, 0xFE, 0x57, 0xB6, 0xB6, 0x2F, 0xAE, 0x6B
};
static const uint8_t TEE_ATTR_HMAC_SHA1_256_VALUE01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f
};
static const uint8_t TEE_ATTR_HMAC_MD5_256_VALUE01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f
};
static const uint8_t TEE_ATTR_HMAC_SHA224_256_VALUE01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f
};
static const uint8_t TEE_ATTR_HMAC_SHA384_512_VALUE01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f
};
static const uint8_t TEE_ATTR_HMAC_SHA512_512_VALUE01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f
};

#undef TEE_ECC_CURVE_NIST_P192
static const uint8_t TEE_ECC_CURVE_NIST_P192[] = { 0x00, 0x00, 0x00, 0x01 };
#undef TEE_ECC_CURVE_NIST_P224
static const uint8_t TEE_ECC_CURVE_NIST_P224[] = { 0x00, 0x00, 0x00, 0x02 };
#undef TEE_ECC_CURVE_NIST_P256
static const uint8_t TEE_ECC_CURVE_NIST_P256[] = { 0x00, 0x00, 0x00, 0x03 };
#undef TEE_ECC_CURVE_NIST_P384
static const uint8_t TEE_ECC_CURVE_NIST_P384[] = { 0x00, 0x00, 0x00, 0x04 };
#undef TEE_ECC_CURVE_NIST_P521
static const uint8_t TEE_ECC_CURVE_NIST_P521[] = { 0x00, 0x00, 0x00, 0x05 };

static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P192_VALUE01[] = {
	0xe1, 0x4f, 0x37, 0xb3, 0xd1, 0x37, 0x4f, 0xf8, 0xb0, 0x3f, 0x41, 0xb9, 0xb3, 0xfd, 0xd2, 0xf0, 0xeb, 0xcc, 0xf2, 0x75, 0xd6, 0x60, 0xd7, 0xf3,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P192_VALUE01[] = {
	0x07, 0x00, 0x8e, 0xa4, 0x0b, 0x08, 0xdb, 0xe7, 0x64, 0x32, 0x09, 0x6e, 0x80, 0xa2, 0x49, 0x4c, 0x94, 0x98, 0x2d, 0x2d, 0x5b, 0xcf, 0x98, 0xe6,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P192_VALUE01[] = {
	0x76, 0xfa, 0xb6, 0x81, 0xd0, 0x0b, 0x41, 0x4e, 0xa6, 0x36, 0xba, 0x21, 0x5d, 0xe2, 0x6d, 0x98, 0xc4, 0x1b, 0xd7, 0xf2, 0xe4, 0xd6, 0x54, 0x77,
};

static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P224_VALUE01[] = {
	0x97, 0xc4, 0xb7, 0x96, 0xe1, 0x63, 0x9d, 0xd1, 0x03, 0x5b, 0x70, 0x8f, 0xc0, 0x0d, 0xc7, 0xba, 0x16, 0x82, 0xce, 0xc4, 0x4a, 0x10, 0x02, 0xa1, 0xa8, 0x20, 0x61, 0x9f,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P224_VALUE01[] = {
	0x8a, 0x4d, 0xca, 0x35, 0x13, 0x6c, 0x4b, 0x70, 0xe5, 0x88, 0xe2, 0x35, 0x54, 0x63, 0x7a, 0xe2, 0x51, 0x07, 0x7d, 0x13, 0x65, 0xa6, 0xba, 0x5d, 0xb9, 0x58, 0x5d, 0xe7,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P224_VALUE01[] = {
	0xad, 0x3d, 0xee, 0x06, 0xde, 0x0b, 0xe8, 0x27, 0x9d, 0x4a, 0xf4, 0x35, 0xd7, 0x24, 0x5f, 0x14, 0xf3, 0xb4, 0xf8, 0x2e, 0xb5, 0x78, 0xe5, 0x19, 0xee, 0x00, 0x57, 0xb1,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P256_VALUE01[] = {
	0xbe, 0x34, 0xba, 0xa8, 0xd0, 0x40, 0xa3, 0xb9, 0x91, 0xf9, 0x07, 0x5b, 0x56, 0xba, 0x29, 0x2f,
	0x75, 0x5b, 0x90, 0xe4, 0xb6, 0xdc, 0x10, 0xda, 0xd3, 0x67, 0x15, 0xc3, 0x3c, 0xfd, 0xac, 0x25,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P256_VALUE01[] = {
	0xfa, 0x27, 0x37, 0xfb, 0x93, 0x48, 0x8d, 0x19, 0xca, 0xef, 0x11, 0xae, 0x7f, 0xaf, 0x6b, 0x7f,
	0x4b, 0xcd, 0x67, 0xb2, 0x86, 0xe3, 0xfc, 0x54, 0xe8, 0xa6, 0x5c, 0x2b, 0x74, 0xae, 0xcc, 0xb0,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P256_VALUE01[] = {
	0xd4, 0xcc, 0xd6, 0xda, 0xe6, 0x98, 0x20, 0x8a, 0xa8, 0xc3, 0xa6, 0xf3, 0x9e, 0x45, 0x51, 0x0d,
	0x03, 0xbe, 0x09, 0xb2, 0xf1, 0x24, 0xbf, 0xc0, 0x67, 0x85, 0x6c, 0x32, 0x4f, 0x9b, 0x4d, 0x09,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P384_VALUE01[] = {
	0xa4, 0x92, 0xce, 0x8f, 0xa9, 0x00, 0x84, 0xc2, 0x27, 0xe1, 0xa3, 0x2f, 0x79, 0x74, 0xd3, 0x9e, 0x9f, 0xf6, 0x7a, 0x7e, 0x87, 0x05, 0xec, 0x34,
	0x19, 0xb3, 0x5f, 0xb6, 0x07, 0x58, 0x2b, 0xeb, 0xd4, 0x61, 0xe0, 0xb1, 0x52, 0x0a, 0xc7, 0x6e, 0xc2, 0xdd, 0x4e, 0x9b, 0x63, 0xeb, 0xae, 0x71,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P384_VALUE01[] = {
	0xe5, 0x5f, 0xee, 0x6c, 0x49, 0xd8, 0xd5, 0x23, 0xf5, 0xce, 0x7b, 0xf9, 0xc0, 0x42, 0x5c, 0xe4, 0xff, 0x65, 0x07, 0x08, 0xb7, 0xde, 0x5c, 0xfb,
	0x09, 0x59, 0x01, 0x52, 0x39, 0x79, 0xa7, 0xf0, 0x42, 0x60, 0x2d, 0xb3, 0x08, 0x54, 0x73, 0x53, 0x69, 0x81, 0x3b, 0x5c, 0x3f, 0x5e, 0xf8, 0x68,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P384_VALUE01[] = {
	0x28, 0xf5, 0x9c, 0xc5, 0xdc, 0x50, 0x98, 0x92, 0xa9, 0x88, 0xd3, 0x8a, 0x8e, 0x25, 0x19, 0xde, 0x3d, 0x0c, 0x4f, 0xd0, 0xfb, 0xdb, 0x09, 0x93,
	0xe3, 0x8f, 0x18, 0x50, 0x6c, 0x17, 0x60, 0x6c, 0x5e, 0x24, 0x24, 0x92, 0x46, 0xf1, 0xce, 0x94, 0x98, 0x3a, 0x53, 0x61, 0xc5, 0xbe, 0x98, 0x3e,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P521_VALUE01[] = {
	0x01, 0xbd, 0x56, 0xbd, 0x10, 0x61, 0x18, 0xed, 0xa2, 0x46, 0x15, 0x5b, 0xd4, 0x3b, 0x42, 0xb8, 0xe1,
	0x3f, 0x0a, 0x6e, 0x25, 0xdd, 0x3b, 0xb3, 0x76, 0x02, 0x6f, 0xab, 0x4d, 0xc9, 0x2b, 0x61, 0x57, 0xbc,
	0x6d, 0xfe, 0xc2, 0xd1, 0x5d, 0xd3, 0xd0, 0xcf, 0x2a, 0x39, 0xaa, 0x68, 0x49, 0x40, 0x42, 0xaf, 0x48,
	0xba, 0x96, 0x01, 0x11, 0x8d, 0xa8, 0x2c, 0x6f, 0x21, 0x08, 0xa3, 0xa2, 0x03, 0xad, 0x74,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P521_VALUE01[] = {
	0x01, 0x2f, 0xbc, 0xae, 0xff, 0xa6, 0xa5, 0x1f, 0x3e, 0xe4, 0xd3, 0xd2, 0xb5, 0x1c, 0x5d, 0xec, 0x6d,
	0x7c, 0x72, 0x6c, 0xa3, 0x53, 0xfc, 0x01, 0x4e, 0xa2, 0xbf, 0x7c, 0xfb, 0xb9, 0xb9, 0x10, 0xd3, 0x2c,
	0xbf, 0xa6, 0xa0, 0x0f, 0xe3, 0x9b, 0x6c, 0xdb, 0x89, 0x46, 0xf2, 0x27, 0x75, 0x39, 0x8b, 0x2e, 0x23,
	0x3c, 0x0c, 0xf1, 0x44, 0xd7, 0x8c, 0x8a, 0x77, 0x42, 0xb5, 0xc7, 0xa3, 0xbb, 0x5d, 0x23,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P521_VALUE01[] = {
	0x00, 0x9c, 0xde, 0xf8, 0x23, 0xdd, 0x7b, 0xf9, 0xa7, 0x9e, 0x8c, 0xce, 0xac, 0xd2, 0xe4, 0x52, 0x7c,
	0x23, 0x1d, 0x0a, 0xe5, 0x96, 0x7a, 0xf0, 0x95, 0x8e, 0x93, 0x1d, 0x7d, 0xdc, 0xcf, 0x28, 0x05, 0xa3,
	0xe6, 0x18, 0xdc, 0x30, 0x39, 0xfe, 0xc9, 0xfe, 0xbb, 0xd3, 0x30, 0x52, 0xfe, 0x4c, 0x0f, 0xee, 0x98,
	0xf0, 0x33, 0x10, 0x60, 0x64, 0x98, 0x2d, 0x88, 0xf4, 0xe0, 0x35, 0x49, 0xd4, 0xa6, 0x4d,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDH_P192_VALUE01[] = {
	0xf1, 0x7d, 0x3f, 0xea, 0x36, 0x7b, 0x74, 0xd3, 0x40, 0x85, 0x1c, 0xa4, 0x27, 0x0d, 0xcb, 0x24, 0xc2, 0x71, 0xf4, 0x45, 0xbe, 0xd9, 0xd5, 0x27,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P192_VALUE01[] = {
	0xb1, 0x50, 0x53, 0x40, 0x1f, 0x57, 0x28, 0x56, 0x37, 0xec, 0x32, 0x4c, 0x1c, 0xd2, 0x13, 0x9e, 0x3a, 0x67, 0xde, 0x37, 0x39, 0x23, 0x4b, 0x37,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P192_VALUE01[] = {
	0xf2, 0x69, 0xc1, 0x58, 0x63, 0x74, 0x82, 0xaa, 0xd6, 0x44, 0xcd, 0x69, 0x2d, 0xd1, 0xd3, 0xef, 0x2c, 0x8a, 0x7c, 0x49, 0xe3, 0x89, 0xf7, 0xf6,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P192_VALUE02[] = {
	0xdf, 0xbe, 0x5d, 0x7c, 0x61, 0xfa, 0xc8, 0x8b, 0x11, 0x81, 0x1b, 0xde, 0x32, 0x8e, 0x8a, 0x0d, 0x12, 0xbf, 0x01, 0xa9, 0xd2, 0x04, 0xb5, 0x23,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P192_VALUE02[] = {
	0x42, 0xea, 0x6d, 0xd9, 0x96, 0x9d, 0xd2, 0xa6, 0x1f, 0xea, 0x1a, 0xac, 0x7f, 0x8e, 0x98, 0xed, 0xcc, 0x89, 0x6c, 0x6e, 0x55, 0x85, 0x7c, 0xc0,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDH_P224_VALUE01[] = {
	0x83, 0x46, 0xa6, 0x0f, 0xc6, 0xf2, 0x93, 0xca, 0x5a, 0x0d, 0x2a, 0xf6, 0x8b, 0xa7, 0x1d, 0x1d, 0xd3, 0x89, 0xe5, 0xe4, 0x08, 0x37, 0x94, 0x2d, 0xf3, 0xe4, 0x3c, 0xbd,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P224_VALUE01[] = {
	0x8d, 0xe2, 0xe2, 0x6a, 0xdf, 0x72, 0xc5, 0x82, 0xd6, 0x56, 0x8e, 0xf6, 0x38, 0xc4, 0xfd, 0x59, 0xb1, 0x8d, 0xa1, 0x71, 0xbd, 0xf5, 0x01, 0xf1, 0xd9, 0x29, 0xe0, 0x48,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P224_VALUE01[] = {
	0x4a, 0x68, 0xa1, 0xc2, 0xb0, 0xfb, 0x22, 0x93, 0x0d, 0x12, 0x05, 0x55, 0xc1, 0xec, 0xe5, 0x0e, 0xa9, 0x8d, 0xea, 0x84, 0x07, 0xf7, 0x1b, 0xe3, 0x6e, 0xfa, 0xc0, 0xde,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P224_VALUE02[] = {
	0x88, 0x20, 0x92, 0xcc, 0xbb, 0xa7, 0x93, 0x0f, 0x41, 0x9a, 0x8a, 0x4f, 0x9b, 0xb1, 0x69, 0x78, 0xbb, 0xc3, 0x83, 0x87, 0x29, 0x99, 0x25, 0x59, 0xa6, 0xf2, 0xe2, 0xd7,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P224_VALUE02[] = {
	0xaf, 0x33, 0xcd, 0x06, 0x29, 0xbc, 0x7e, 0x99, 0x63, 0x20, 0xa3, 0xf4, 0x03, 0x68, 0xf7, 0x4d, 0xe8, 0x70, 0x4f, 0xa3, 0x7b, 0x8f, 0xab, 0x69, 0xab, 0xaa, 0xe2, 0x80,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDH_P256_VALUE01[] = {
	0x7d, 0x7d, 0xc5, 0xf7, 0x1e, 0xb2, 0x9d, 0xda, 0xf8, 0x0d, 0x62, 0x14, 0x63, 0x2e, 0xea, 0xe0,
	0x3d, 0x90, 0x58, 0xaf, 0x1f, 0xb6, 0xd2, 0x2e, 0xd8, 0x0b, 0xad, 0xb6, 0x2b, 0xc1, 0xa5, 0x34,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P256_VALUE01[] = {
	0xea, 0xd2, 0x18, 0x59, 0x01, 0x19, 0xe8, 0x87, 0x6b, 0x29, 0x14, 0x6f, 0xf8, 0x9c, 0xa6, 0x17,
	0x70, 0xc4, 0xed, 0xbb, 0xf9, 0x7d, 0x38, 0xce, 0x38, 0x5e, 0xd2, 0x81, 0xd8, 0xa6, 0xb2, 0x30,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P256_VALUE01[] = {
	0x28, 0xaf, 0x61, 0x28, 0x1f, 0xd3, 0x5e, 0x2f, 0xa7, 0x00, 0x25, 0x23, 0xac, 0xc8, 0x5a, 0x42,
	0x9c, 0xb0, 0x6e, 0xe6, 0x64, 0x83, 0x25, 0x38, 0x9f, 0x59, 0xed, 0xfc, 0xe1, 0x40, 0x51, 0x41,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P256_VALUE02[] = {
	0xdb, 0x71, 0xe5, 0x09, 0xe3, 0xfd, 0x9b, 0x06, 0x0d, 0xdb, 0x20, 0xba, 0x5c, 0x51, 0xdc, 0xc5,
	0x94, 0x8d, 0x46, 0xfb, 0xf6, 0x40, 0xdf, 0xe0, 0x44, 0x17, 0x82, 0xca, 0xb8, 0x5f, 0xa4, 0xac,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P256_VALUE02[] = {
	0x70, 0x0c, 0x48, 0xf7, 0x7f, 0x56, 0x58, 0x4c, 0x5c, 0xc6, 0x32, 0xca, 0x65, 0x64, 0x0d, 0xb9,
	0x1b, 0x6b, 0xac, 0xce, 0x3a, 0x4d, 0xf6, 0xb4, 0x2c, 0xe7, 0xcc, 0x83, 0x88, 0x33, 0xd2, 0x87,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDH_P384_VALUE01[] = {
	0x3c, 0xc3, 0x12, 0x2a, 0x68, 0xf0, 0xd9, 0x50, 0x27, 0xad, 0x38, 0xc0, 0x67, 0x91, 0x6b, 0xa0, 0xeb, 0x8c, 0x38, 0x89, 0x4d, 0x22, 0xe1, 0xb1,
	0x56, 0x18, 0xb6, 0x81, 0x8a, 0x66, 0x17, 0x74, 0xad, 0x46, 0x3b, 0x20, 0x5d, 0xa8, 0x8c, 0xf6, 0x99, 0xab, 0x4d, 0x43, 0xc9, 0xcf, 0x98, 0xa1,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P384_VALUE01[] = {
	0x98, 0x03, 0x80, 0x7f, 0x2f, 0x6d, 0x2f, 0xd9, 0x66, 0xcd, 0xd0, 0x29, 0x0b, 0xd4, 0x10, 0xc0, 0x19, 0x03, 0x52, 0xfb, 0xec, 0x7f, 0xf6, 0x24,
	0x7d, 0xe1, 0x30, 0x2d, 0xf8, 0x6f, 0x25, 0xd3, 0x4f, 0xe4, 0xa9, 0x7b, 0xef, 0x60, 0xcf, 0xf5, 0x48, 0x35, 0x5c, 0x01, 0x5d, 0xbb, 0x3e, 0x5f,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P384_VALUE01[] = {
	0xba, 0x26, 0xca, 0x69, 0xec, 0x2f, 0x5b, 0x5d, 0x9d, 0xad, 0x20, 0xcc, 0x9d, 0xa7, 0x11, 0x38, 0x3a, 0x9d, 0xbe, 0x34, 0xea, 0x3f, 0xa5, 0xa2,
	0xaf, 0x75, 0xb4, 0x65, 0x02, 0x62, 0x9a, 0xd5, 0x4d, 0xd8, 0xb7, 0xd7, 0x3a, 0x8a, 0xbb, 0x06, 0xa3, 0xa3, 0xbe, 0x47, 0xd6, 0x50, 0xcc, 0x99,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P384_VALUE02[] = {
	0xac, 0x68, 0xf1, 0x9f, 0x2e, 0x1c, 0xb8, 0x79, 0xae, 0xd4, 0x3a, 0x99, 0x69, 0xb9, 0x1a, 0x08,
	0x39, 0xc4, 0xc3, 0x8a, 0x49, 0x74, 0x9b, 0x66, 0x1e, 0xfe, 0xdf, 0x24, 0x34, 0x51, 0x91, 0x5e,
	0xd0, 0x90, 0x5a, 0x32, 0xb0, 0x60, 0x99, 0x2b, 0x46, 0x8c, 0x64, 0x76, 0x6f, 0xc8, 0x43, 0x7a,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P384_VALUE02[] = {
	0xa7, 0xc7, 0x6b, 0x97, 0x0c, 0x3b, 0x5f, 0xe8, 0xb0, 0x5d, 0x28, 0x38, 0xae, 0x04, 0xab, 0x47,
	0x69, 0x7b, 0x9e, 0xaf, 0x52, 0xe7, 0x64, 0x59, 0x2e, 0xfd, 0xa2, 0x7f, 0xe7, 0x51, 0x32, 0x72,
	0x73, 0x44, 0x66, 0xb4, 0x00, 0x09, 0x1a, 0xdb, 0xf2, 0xd6, 0x8c, 0x58, 0xe0, 0xc5, 0x00, 0x66,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P521_VALUE01[] = {
	0x00, 0x00, 0x01, 0xb7, 0xc8, 0x5f, 0x26, 0xf5, 0xd4, 0xb2, 0xd7, 0x35, 0x5c, 0xf6, 0xb0, 0x21, 0x17,
	0x65, 0x99, 0x43, 0x76, 0x2b, 0x6d, 0x1d, 0xb5, 0xab, 0x4f, 0x1d, 0xbc, 0x44, 0xce, 0x7b, 0x29, 0x46,
	0xeb, 0x6c, 0x7d, 0xe3, 0x42, 0x96, 0x28, 0x93, 0xfd, 0x38, 0x7d, 0x1b, 0x73, 0xd7, 0xa8, 0x67, 0x2d,
	0x1f, 0x23, 0x69, 0x61, 0x17, 0x0b, 0x7e, 0xb3, 0x57, 0x99, 0x53, 0xee, 0x5c, 0xdc, 0x88, 0xcd, 0x2d,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDH_P521_VALUE02[] = {
	0x00, 0x00, 0x01, 0xba, 0x52, 0xc5, 0x6f, 0xc8, 0x77, 0x6d, 0x9e, 0x8f, 0x5d, 0xb4, 0xf0, 0xcc,
	0x27, 0x63, 0x6d, 0x0b, 0x74, 0x1b, 0xbe, 0x05, 0x40, 0x06, 0x97, 0x94, 0x2e, 0x80, 0xb7, 0x39,
	0x88, 0x4a, 0x83, 0xbd, 0xe9, 0x9e, 0x0f, 0x67, 0x16, 0x93, 0x9e, 0x63, 0x2b, 0xc8, 0x98, 0x6f,
	0xa1, 0x8d, 0xcc, 0xd4, 0x43, 0xa3, 0x48, 0xb6, 0xc3, 0xe5, 0x22, 0x49, 0x79, 0x55, 0xa4, 0xf3,
	0xc3, 0x02, 0xf6, 0x76,
};
static const uint8_t TEE_ATTR_ECC_PRIVATE_VALUE_ECDH_P521_VALUE01[] = {
	0x00, 0x00, 0x01, 0x7e, 0xec, 0xc0, 0x7a, 0xb4, 0xb3, 0x29, 0x06, 0x8f, 0xba, 0x65, 0xe5, 0x6a, 0x1f,
	0x88, 0x90, 0xaa, 0x93, 0x5e, 0x57, 0x13, 0x4a, 0xe0, 0xff, 0xcc, 0xe8, 0x02, 0x73, 0x51, 0x51, 0xf4,
	0xea, 0xc6, 0x56, 0x4f, 0x6e, 0xe9, 0x97, 0x4c, 0x5e, 0x68, 0x87, 0xa1, 0xfe, 0xfe, 0xe5, 0x74, 0x3a,
	0xe2, 0x24, 0x1b, 0xfe, 0xb9, 0x5d, 0x5c, 0xe3, 0x1d, 0xdc, 0xb6, 0xf9, 0xed, 0xb4, 0xd6, 0xfc, 0x47,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P521_VALUE01[] = {
	0x00, 0x00, 0x00, 0x60, 0x2f, 0x9d, 0x0c, 0xf9, 0xe5, 0x26, 0xb2, 0x9e, 0x22, 0x38, 0x1c, 0x20, 0x3c,
	0x48, 0xa8, 0x86, 0xc2, 0xb0, 0x67, 0x30, 0x33, 0x36, 0x63, 0x14, 0xf1, 0xff, 0xbc, 0xba, 0x24, 0x0b,
	0xa4, 0x2f, 0x4e, 0xf3, 0x8a, 0x76, 0x17, 0x46, 0x35, 0xf9, 0x1e, 0x6b, 0x4e, 0xd3, 0x42, 0x75, 0xeb,
	0x01, 0xc8, 0x46, 0x7d, 0x05, 0xca, 0x80, 0x31, 0x5b, 0xf1, 0xa7, 0xbb, 0xd9, 0x45, 0xf5, 0x50, 0xa5,
};
static const uint8_t TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDH_P521_VALUE02[] = {
	0x00, 0x00, 0x00, 0x68, 0x5a, 0x48, 0xe8, 0x6c, 0x79, 0xf0, 0xf0, 0x87, 0x5f, 0x7b, 0xc1, 0x8d,
	0x25, 0xeb, 0x5f, 0xc8, 0xc0, 0xb0, 0x7e, 0x5d, 0xa4, 0xf4, 0x37, 0x0f, 0x3a, 0x94, 0x90, 0x34,
	0x08, 0x54, 0x33, 0x4b, 0x1e, 0x1b, 0x87, 0xfa, 0x39, 0x54, 0x64, 0xc6, 0x06, 0x26, 0x12, 0x4a,
	0x4e, 0x70, 0xd0, 0xf7, 0x85, 0x60, 0x1d, 0x37, 0xc0, 0x98, 0x70, 0xeb, 0xf1, 0x76, 0x66, 0x68,
	0x77, 0xa2, 0x04, 0x6d,
};

static const uint8_t TEE_ATTR_DH_PUBLIC_VALUE_VALUE02[] = {
	0xa3, 0xf5, 0x7d, 0xbe, 0x9e, 0x2f, 0x0a, 0xda, 0xa9, 0x4e, 0x4e, 0x6a, 0xf0, 0xe0, 0x71, 0x47,
	0x0e, 0x2e, 0x41, 0x2e, 0xde, 0x73, 0x2a, 0x62, 0x14, 0xc3, 0x7c, 0x26, 0xd4, 0xe9, 0x9a, 0x54,
	0xba, 0x3d, 0xe7, 0x49, 0x85, 0x95, 0x0e, 0xe9, 0x14, 0xb2, 0x90, 0x22, 0x91, 0xdc, 0xff, 0x61,
	0xb2, 0xfc, 0xd1, 0xd0, 0x1b, 0x11, 0x14, 0xb6, 0x02, 0x64, 0x2b, 0x26, 0x5d, 0x88, 0xea, 0x8d,
	0xbb, 0xe2, 0x07, 0x0b, 0x48, 0xfb, 0x01, 0x53, 0x55, 0x1e, 0x59, 0x51, 0x36, 0xf2, 0xf9, 0xd1,
	0x97, 0xfb, 0x66, 0x12, 0x84, 0x5d, 0xed, 0xb8, 0x9b, 0x2d, 0x3e, 0x2b, 0x8c, 0xeb, 0x2a, 0x72,
	0x40, 0x9d, 0x55, 0x4c, 0xed, 0xeb, 0x55, 0x02, 0xff, 0x8c, 0xb0, 0x2e, 0x03, 0x65, 0x3f, 0x41,
	0xb1, 0xac, 0xa3, 0x30, 0x6b, 0xff, 0x6d, 0xf4, 0x6d, 0xe6, 0xe1, 0x0f, 0x86, 0x7c, 0x43, 0x64,
};


/*ALL_CRYPTO_AAD_VALUES*/
static const uint8_t AAD1_VALUE[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
};

/*ALL_CRYPTO_AAD_LENGTHS*/
#define AAD1_LENGTH     8
#define NULL_LENGTH     0

/*ALL_TEE_CRYPTO_INITIALISATION_VECTOR_VALUES*/
static const uint8_t NONCE2_VALUE_AES_GCM[] = {
	0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88
};
static const uint8_t NONCE1_VALUE_AES_CCM[] = {
	0x00, 0x8D, 0x49, 0x3B, 0x30, 0xAE, 0x8B, 0x3C, 0x96, 0x96, 0x76, 0x6C,
	0xFA
};

/*ALL_TEE_CRYPTO_INITIALISATION_VECTOR_LENGTHS*/
#define NONCE2_LENGTH_AES_GCM       12
#define NONCE1_LENGTH_AES_CCM       13

/*ALL_CRYPTO_DATA_VALUE*/
static const uint8_t DATA_ALL_ZEROES[4096];
static const uint8_t DATA_FOR_CRYPTO1[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
	0x0C, 0x0D, 0x0E, 0x0F,
	0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09,
	0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
	0x03, 0x02, 0x01, 0x00,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
	0x0C, 0x0D, 0x0E, 0x0F,
	0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09,
	0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
	0x03, 0x02, 0x01, 0x00
};
static const uint8_t DATA_FOR_CRYPTO1_PART1[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
	0x0C, 0x0D, 0x0E, 0x0F,
	0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09
};
static const uint8_t DATA_FOR_CRYPTO1_PART2[] = {
	0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
	0x03, 0x02, 0x01, 0x00,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
	0x0C, 0x0D, 0x0E, 0x0F
};
static const uint8_t DATA_FOR_CRYPTO1_PART3[] = {
	0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09,
	0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
	0x03, 0x02, 0x01, 0x00
};
static const uint8_t DATA_FOR_CRYPTO2_LENGTH_NOT_MULTIPLE_OF_128_BITS[] = {
	0x00, 0x01, 0x02, 0x03,
};
static const uint8_t DATA_FOR_CRYPTO3_LENGTH_NOT_MULTIPLE_OF_64_BITS[] = {
	0x00, 0x01, 0x02, 0x03,
};

/*ALL_CRYPTO_DATA_LENGTH*/
#define LENGTH_DATA_FOR_CRYPTO1         96
#define LENGTH_DATA_FOR_CRYPTO1_PART1   32
#define LENGTH_DATA_FOR_CRYPTO1_PART2   32
#define LENGTH_DATA_FOR_CRYPTO1_PART3   32
#define LENGTH_DATA_FOR_CRYPTO2         4
#define LENGTH_DATA_FOR_CRYPTO3         4
#define RSAES_PKCS1_OAEP_MGF1_SHA1_ENC_INVALID_LENGTH_MOD_2048		215
#define RSAES_PKCS1_OAEP_MGF1_SHA224_ENC_INVALID_LENGTH_MOD_2048	199
#define RSAES_PKCS1_OAEP_MGF1_SHA256_ENC_INVALID_LENGTH_MOD_2048	191
#define RSAES_PKCS1_OAEP_MGF1_SHA384_ENC_INVALID_LENGTH_MOD_2048	159
#define RSAES_PKCS1_OAEP_MGF1_SHA512_ENC_INVALID_LENGTH_MOD_2048	127
#define RSAES_PKCS1_V1_5_ENC_INVALID_LENGTH_MOD_2048			246

/*ALL_TEE_CRYPTO_INITIALISATION_VECTOR_VALUES*/
static const uint8_t IV1_VALUE_64bits_DES_DES3[] = {
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef
};
static const uint8_t IV2_VALUE_128bits_AES[] = {
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78,
	0x90, 0xab, 0xcd, 0xef
};
static const uint8_t IV_INVALID_LENGTH_VALUE[] = {
	0x01, 0x02, 0x03, 0x04, 0x05
};

/*ALL_TEE_CRYPTO_INITIALISATION_VECTOR_LENGTHS*/
#define IV_LENGTH_NULL                  0
#define IV_INVALID_LENGTH               5
#define IV1_LENGTH_64bits_DES_DES3      8
#define IV2_LENGTH_128bits_AES          16

static const uint8_t *TEE_ATTR_VALUE_NONE;

/*ALL_TEE_BUFFER_CASES*/
#define INPUT_BUFFER_NORMAL	5
#define INPUT_BUFFER_TOO_SHORT	4
#define OUTPUT_BUFFER_NORMAL	1
#define OUTPUT_BUFFER_TOO_SHORT	2
#define TAG_BUFFER_TOO_SHORT	3

enum signature_validity {
	INVALID_SIGNATURE = 0,
	VALID_SIGNATURE
};

enum mac_validity {
	INVALID_MAC = 0,
	VALID_MAC
};

struct crypto_buffer {
	uint8_t *buffer;
	uint32_t size;
};

/*Saved in Invoke_Crypto_AllocateOperation*/
struct crypto_op {
	uint32_t algo;
	uint32_t mode;
	uint32_t obj_size;
};
struct crypto_op saved_alloc;

/*Saved in Invoke_Crypto_InitObjectWithKeys*/
struct key_val {
	uint32_t obh;
	uint32_t obj_type;
	uint32_t obj_size;
	struct crypto_buffer key;
};
struct key_val saved_key_vals;

/*Saved in Invoke_Crypto_SetOperationKey*/
struct obh_val {
	uint32_t oph;
	uint32_t obh;
};
struct obh_val saved_obh1;

/*Saved in Invoke_Crypto_SetOperationKey2*/
struct obh_val2 {
	uint32_t oph;
	uint32_t obh1;
	uint32_t obh2;
};
struct obh_val2 saved_obh;

/*saved by Invoke_Crypto_AEUpdateAAD*/
struct crypto_buffer saved_aad_value;

/*Saved in Invoke_Crypto_AEEncryptFinal*/
struct crypto_buffer ae_encrypt_tag;

/*Saved in Invoke_Crypto_AEUpdate_for_encryption*/
struct crypto_buffer buffer_encrypted_chunks[4];

/*Saved in Invoke_Crypto_AEUpdate_for_Decryption*/
struct crypto_buffer buffer_decrypted_chunks[4];

/*filled with data in Invoke_Crypto_AsymmetricEncrypt*/
struct crypto_buffer buffer_asym_encrypted;

/*saved by Invoke_Crypto_AEInit*/
struct crypto_buffer saved_in_nonce;

/*Saved in Invoke_Crypto_DeriveKey*/
struct obh_val saved_derive;

/*Saved in Invoke_Crypto_GenerateRandom*/
struct crypto_buffer saved_random;

/*Saved in Invoke_Crypto_DigestDoFinal*/
struct crypto_buffer saved_digest;

/*Saved in Invoke_Crypto_MACInit*/
struct crypto_buffer saved_mac_iv;

/*Saved in Invoke_Crypto_CipherInit*/
struct crypto_buffer saved_cipher_iv;

/*Saved in Invoke_Crypto_CipherUpdate*/
struct crypto_buffer saved_cipher_update;

static const void *saved_key_pub_attr[3];

static TEE_OperationInfo saved_op_info;
static uint32_t s_saved_op_mult_info[64 / sizeof(uint32_t)];
static TEE_OperationInfoMultiple *saved_op_mult_info =
	(TEE_OperationInfoMultiple *)s_saved_op_mult_info;


TEEC_UUID UUID_TTA_testingInternalAPI_crypto = {
	0x534D4152, 0x5443, 0x534C,
	{ 0x54, 0x43, 0x52, 0x59, 0x50, 0x54, 0x4F, 0x31 }
};

static void crypto_reset(void)
{
	saved_obh.oph = 0;
	saved_obh.obh1 = 0;
	saved_obh.obh2 = 0;
	saved_alloc.algo = 0;
	saved_alloc.mode = 0;
	saved_alloc.obj_size = 0;
	saved_key_vals.obh = 0;
	saved_key_vals.obj_size = 0;
	saved_key_vals.obj_type = 0;
	CRYPTO_FREE(saved_key_vals.key);

	CRYPTO_FREE(saved_aad_value);
	CRYPTO_FREE(ae_encrypt_tag);
	/*4 chunks*/
	CRYPTO_FREE(buffer_encrypted_chunks[0]);
	CRYPTO_FREE(buffer_encrypted_chunks[1]);
	CRYPTO_FREE(buffer_encrypted_chunks[2]);
	CRYPTO_FREE(buffer_encrypted_chunks[3]);
	/*4 chunks*/
	CRYPTO_FREE(buffer_decrypted_chunks[0]);
	CRYPTO_FREE(buffer_decrypted_chunks[1]);
	CRYPTO_FREE(buffer_decrypted_chunks[2]);
	CRYPTO_FREE(buffer_decrypted_chunks[3]);
	CRYPTO_FREE(buffer_asym_encrypted);
	CRYPTO_FREE(saved_in_nonce);
	CRYPTO_FREE(saved_random);
	CRYPTO_FREE(saved_digest);
	CRYPTO_FREE(saved_cipher_iv);
	CRYPTO_FREE(saved_cipher_update);

	memset(&saved_op_info, 0, sizeof(saved_op_info));
	memset(s_saved_op_mult_info, 0, sizeof(s_saved_op_mult_info));
}

static TEEC_Result calculate_digest(ADBG_Case_t *c, const void *data,
				    size_t data_length,
				    struct crypto_buffer *digest);

static TEEC_Result sign_digest(ADBG_Case_t *c,
			       const struct crypto_buffer *in_dgst,
			       struct crypto_buffer *out_dgst);

static bool verify_digest(ADBG_Case_t *c, const struct crypto_buffer *in_sdgst);

static TEEC_Result mac_compute_final(ADBG_Case_t *c, TEEC_Session *s,
				     const void *full_data,
				     size_t fdata_length,
				     struct crypto_buffer *mac);

static TEEC_Result cipher_do_final(ADBG_Case_t *c, TEEC_Session *s,
				   const void *full_data,
				   size_t fdata_length,
				   struct crypto_buffer *cipher);

static void collapse_crypto_buffers(struct crypto_buffer *in_buffer,
				    struct crypto_buffer *out_buffer)
{
	int id;
	uint8_t *tmp;

	out_buffer->size = 0;

	for (id = 0; id < 4; id++)
		out_buffer->size += in_buffer[id].size;

	out_buffer->buffer = malloc(out_buffer->size);
	tmp = out_buffer->buffer;

	for (id = 0; id < 4; id++) {
		if (in_buffer[id].buffer) {
			memcpy(tmp, in_buffer[id].buffer, in_buffer[id].size);
			tmp += in_buffer[id].size;
		}
	}
}

static TEEC_Result Invoke_Crypto_AllocateOperation(ADBG_Case_t *c,
						   TEEC_Session *s,
						   uint32_t cmd, uint32_t algo,
						   uint32_t mode,
						   size_t obj_size1,
						   size_t obj_size2,
						   uint32_t op_handle)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	op.params[0].value.a = algo;
	op.params[0].value.b = mode;
	if (obj_size1 > obj_size2)
		op.params[1].value.a = obj_size1;
	else
		op.params[1].value.a = obj_size2;
	op.params[3].value.a = op_handle;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT,
					 TEEC_NONE, TEEC_VALUE_INPUT);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (!res) {
		saved_alloc.algo = algo;
		saved_alloc.mode = mode;
		saved_alloc.obj_size = op.params[1].value.a;
	}

	return res;
}

static TEEC_Result Invoke_Crypto_GetOperationInfo(ADBG_Case_t *c ,
						  TEEC_Session *s,
						  uint32_t cmd,
						  uint32_t op_handle)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig = 0;

	op.params[0].value.a = op_handle;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_VALUE_OUTPUT,
					 TEEC_VALUE_OUTPUT, TEEC_VALUE_OUTPUT);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (!res)
		saved_op_info = (TEE_OperationInfo){
			.algorithm = op.params[0].value.a,
			.operationClass = op.params[0].value.b,
			.mode = op.params[1].value.a,
			.digestLength = op.params[1].value.b,
			.maxKeySize = op.params[2].value.a,
			.keySize = op.params[2].value.b,
			.requiredKeyUsage = op.params[3].value.a,
			.handleState = op.params[3].value.b,
		};

	return res;
}

static uint32_t get_be32(const uint8_t *b)
{
	return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) |
	       ((uint32_t)b[2] << 8) | b[3];
}

static TEEC_Result Invoke_Crypto_GetOperationInfoMultiple(ADBG_Case_t *c,
							  TEEC_Session *s,
							  uint32_t cmd,
							  uint32_t op_handle,
							  uint32_t max_num_keys)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;
	TEEC_SharedMemory shm = {
		.size = 64,
		.flags = TEEC_MEM_OUTPUT,
	};

	if (!ADBG_EXPECT_COMPARE_UNSIGNED(c, max_num_keys, <=, 2))
		return TEEC_ERROR_BAD_PARAMETERS;

	res = TEEC_AllocateSharedMemory(s->ctx, &shm);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	op.params[0].value.a = op_handle;
	op.params[0].value.b = max_num_keys;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, &shm, shm.size);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	res = TEEC_InvokeCommand(s, cmd, &op, &org);

	if (!res) {
		uint8_t *b = shm.buffer;
		TEE_OperationInfoMultiple *d = saved_op_mult_info;
		size_t n = 0;

		memset(s_saved_op_mult_info, 0, sizeof(s_saved_op_mult_info));
		d->algorithm = get_be32(b);
		d->operationClass = get_be32(b + 4);
		d->mode = get_be32(b + 8);
		d->digestLength = get_be32(b + 12);
		d->maxKeySize = get_be32(b + 16);
		d->handleState = get_be32(b + 20);
		d->operationState = get_be32(b + 24);
		d->numberOfKeys = get_be32(b + 28);

		for (n = 0; n < d->numberOfKeys; n++) {
			d->keyInformation[n].keySize = get_be32(b + 32 + n * 8);
			d->keyInformation[n].requiredKeyUsage =
				get_be32(b + 36 + n * 8);
		}
	}
	TEEC_ReleaseSharedMemory(&shm);

	return res;
}

#define Check_0_OperationInfoMultiple(c, _algorithm, _operationClass, _mode, \
				      _digestLength, _maxKeySize_object1, \
				      _maxKeySize_object2, _handleState, \
				      _operationState, _numberOfKeys) \
	do { \
		ADBG_EXPECT(c, _algorithm, saved_op_mult_info->algorithm); \
		ADBG_EXPECT(c, _operationClass, \
			    saved_op_mult_info->operationClass); \
		ADBG_EXPECT(c, _mode, saved_op_mult_info->mode); \
		ADBG_EXPECT(c, _digestLength, \
			    saved_op_mult_info->digestLength); \
		ADBG_EXPECT(c, MAX(_maxKeySize_object1, _maxKeySize_object2), \
			    saved_op_mult_info->maxKeySize); \
		ADBG_EXPECT(c, _handleState, saved_op_mult_info->handleState); \
		ADBG_EXPECT(c, _operationState, \
			    saved_op_mult_info->operationState); \
		ADBG_EXPECT(c, _numberOfKeys, \
			    saved_op_mult_info->numberOfKeys); \
	} while (0)

#define Check_1_OperationInfoKey(c, _keyIndex, _keySize, _requiredKeyUsage) \
	do { \
		if (ADBG_EXPECT_COMPARE_UNSIGNED(c, _keyIndex, <, saved_op_mult_info->numberOfKeys)) { \
			ADBG_EXPECT(c, _keySize, saved_op_mult_info->keyInformation[_keyIndex].keySize); \
			ADBG_EXPECT(c, _requiredKeyUsage, saved_op_mult_info->keyInformation[_keyIndex].requiredKeyUsage); \
		} \
	} while (0)

#define Check_OperationInfo(c, _algorithm, _operationClass, _mode, \
			    _digestLength, _maxKeySize_object1, \
			    _maxKeySize_object2, _keySize, _requiredKeyUsage, \
			    _handleFlags) \
	do { \
		ADBG_EXPECT(c, _algorithm, saved_op_info.algorithm); \
		ADBG_EXPECT(c, _operationClass, saved_op_info.operationClass); \
		ADBG_EXPECT(c, _mode, saved_op_info.mode); \
		ADBG_EXPECT(c, _digestLength, saved_op_info.digestLength); \
		ADBG_EXPECT(c, MAX(_maxKeySize_object1, _maxKeySize_object2), \
			    saved_op_info.maxKeySize); \
		ADBG_EXPECT(c, _keySize, saved_op_info.keySize); \
		ADBG_EXPECT(c, _requiredKeyUsage, \
			    saved_op_info.requiredKeyUsage); \
		ADBG_EXPECT(c, _handleFlags, saved_op_info.handleState); \
		ADBG_EXPECT(c, _algorithm, saved_op_info.algorithm); \
	} while (0)

static TEEC_Result Invoke_Crypto_ResetOperation(ADBG_Case_t *c, TEEC_Session *s,
						uint32_t cmd,
						uint32_t op_handle)
{
	return Invoke_Simple_Function_v1(c, s, cmd, op_handle, 0);
}

static TEEC_Result Invoke_Crypto_FreeAllKeysAndOperations(ADBG_Case_t *c,
							  TEEC_Session *s,
							  uint32_t cmd,
							  uint32_t op_handle)
{
	return Invoke_Simple_Function_v1(c, s, cmd, op_handle, 0);
}

static TEEC_Result add_be32(uint32_t v, TEEC_SharedMemory *shm, size_t *offs)
{
	uint8_t *d = (uint8_t *)shm->buffer + *offs;
	size_t n = sizeof(uint32_t);

	if (*offs + n > shm->size)
		return TEEC_ERROR_EXCESS_DATA;

	(*offs) += n;
	while (n) {
		n--;
		d[n] = v;
		v >>= 8;
	}

	return TEEC_SUCCESS;
}

static TEEC_Result add_attr(uint32_t id, const void *v, uint32_t sz,
			    TEEC_SharedMemory *shm1, size_t *offs_1,
			    TEEC_SharedMemory *shm2, size_t *offs_2)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t vid = 0;
	uint32_t voffs = 0;
	uint32_t vsz = 0;

	if (id == TEE_ATTR_NONE)
		goto out;
	vid = id;
	voffs = *offs_2;
	vsz = sz;

	if (*offs_2 + sz > shm2->size)
		return TEEC_ERROR_EXCESS_DATA;
	memcpy((uint8_t *)shm2->buffer + *offs_2, v, sz);
	(*offs_2) += sz;

out:
	res = add_be32(vid, shm1, offs_1);
	if (res)
		return res;
	res = add_be32(voffs, shm1, offs_1);
	if (res)
		return res;
	return add_be32(vsz, shm1, offs_1);
}

static TEEC_Result
Invoke_Crypto_InitObjectWithKeys(ADBG_Case_t *c, TEEC_Session *s, uint32_t cmd,
				 uint32_t obj_type, uint32_t obj_size,
				 uint32_t id1, const void *v1, uint32_t sz1,
				 uint32_t id2, const void *v2, uint32_t sz2,
				 uint32_t id3, const void *v3, uint32_t sz3,
				 uint32_t id4, const void *v4, uint32_t sz4,
				 uint32_t id5, const void *v5, uint32_t sz5,
				 uint32_t obj_handle)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t org = 0;
	size_t offs1 = 0;
	size_t offs2 = 0;
	TEEC_SharedMemory shm1 = {
		.size = BIG_SIZE,
		.flags = TEEC_MEM_INPUT,
	};
	TEEC_SharedMemory shm2 = {
		.size = DS_BIG_SIZE,
		.flags = TEEC_MEM_INPUT,
	};

	res = TEEC_AllocateSharedMemory(s->ctx, &shm1);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	res = TEEC_AllocateSharedMemory(s->ctx, &shm2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out_free_shm1;

	res = add_attr(id1, v1, sz1, &shm1, &offs1, &shm2, &offs2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out_free_shm2;
	res = add_attr(id2, v2, sz2, &shm1, &offs1, &shm2, &offs2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out_free_shm2;
	res = add_attr(id3, v3, sz3, &shm1, &offs1, &shm2, &offs2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out_free_shm2;
	res = add_attr(id4, v4, sz4, &shm1, &offs1, &shm2, &offs2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out_free_shm2;
	res = add_attr(id5, v5, sz5, &shm1, &offs1, &shm2, &offs2);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		goto out_free_shm2;

	op.params[0].value.a = obj_type;
	op.params[0].value.b = obj_size;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, &shm1, offs1);
	SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, &shm2, offs2);
	op.params[3].value.a = obj_handle;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_VALUE_INPUT);

	res = TEEC_InvokeCommand(s, cmd, &op, &org);

	if (!res)
		saved_key_pub_attr[obj_handle] = v1;

out_free_shm2:
	TEEC_ReleaseSharedMemory(&shm2);
out_free_shm1:
	TEEC_ReleaseSharedMemory(&shm1);

	return res;
}

static TEEC_Result
Invoke_Crypto_InitObjectWithKeysExt(ADBG_Case_t *c, TEEC_Session *s,
				    uint32_t cmd, uint32_t obj_type,
				    uint32_t obj_size,
				    uint32_t id1, const void *v1, uint32_t sz1,
				    uint32_t id2, const void *v2, uint32_t sz2,
				    uint32_t id3, const void *v3, uint32_t sz3,
				    uint32_t id4, const void *v4, uint32_t sz4,
				    uint32_t id5, const void *v5, uint32_t sz5,
				    uint32_t obj_handle)
{
	return Invoke_Crypto_InitObjectWithKeys(c, s, cmd, obj_type, obj_size,
						id1, v1, sz1, id2, v2, sz2,
						id3, v3, sz3, id4, v4, sz4,
						id5, v5, sz5, obj_handle);
}



static TEEC_Result Invoke_Crypto_SetOperationKey(ADBG_Case_t *c,
						 TEEC_Session *s, uint32_t cmd,
						 uint32_t op_handle,
						 uint32_t obj_handle)
{
	TEEC_Result res = Invoke_Simple_Function_v1(c, s, cmd, op_handle,
						    obj_handle);

	if (!res) {
		saved_obh.oph = op_handle;
		saved_obh.obh1 = obj_handle;
		saved_obh.obh2 = TEE_HANDLE_NULL;
	}

	return res;
}

static TEEC_Result Invoke_Crypto_SetOperationKey2(ADBG_Case_t *c,
						  TEEC_Session *s,
						  uint32_t cmd,
						  uint32_t op_handle,
						  uint32_t obj_handle1,
						  uint32_t obj_handle2)
{
	TEEC_Result res =  Invoke_Simple_Function_v2(c, s, cmd, op_handle,
						     obj_handle1, obj_handle2,
						     0);
	if (!res) {
		saved_obh.oph = op_handle;
		saved_obh.obh1 = obj_handle1;
		saved_obh.obh2 = obj_handle2;
	}

	return res;
}

static TEEC_Result Invoke_Crypto_DeriveKey(ADBG_Case_t *c, TEEC_Session *s,
					   uint32_t cmd, uint32_t op_handle,
					   uint32_t attribute_list,
					   uint32_t obj_handle)
{
	uint32_t attr1 = ATTRIBUTE_NONE;
	uint32_t attr2 = ATTRIBUTE_NONE;
	uint32_t n = 0;

	for (n = 0; n <= ATTRIBUTE_06 && attr2 == ATTRIBUTE_NONE; n++) {
		if (BIT(n) & attribute_list) {
			if (attr1 == ATTRIBUTE_NONE)
				attr1 = n;
			else
				attr2 = n;
		}
	}

	return Invoke_Simple_Function_v2(c, s, cmd, op_handle, obj_handle,
					 attr1, attr2);
}

static TEEC_Result Invoke_Crypto_AEInit(ADBG_Case_t *c, TEEC_Session *s,
					uint32_t cmd, uint32_t op_handle,
					const void *nonce_val,
					size_t nonce_length, uint32_t tag_len,
					uint32_t aad_len, uint32_t payload_len)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;
	TEEC_SharedMemory shm = {
		.size = nonce_length,
		.flags = TEEC_MEM_INPUT,
	};


	res = TEEC_AllocateSharedMemory(s->ctx, &shm);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;
	memcpy(shm.buffer, nonce_val, nonce_length);

	op.params[0].value.a = op_handle;
	op.params[0].value.b = tag_len;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, &shm, nonce_length);
	op.params[2].value.a = aad_len;
	op.params[2].value.b = payload_len;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_VALUE_INPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &org);
	TEEC_ReleaseSharedMemory(&shm);
	return res;
}

static TEEC_Result Invoke_Crypto_AEUpdate_for_encryption(ADBG_Case_t *c,
							 TEEC_Session *s,
							 uint32_t cmd,
							 uint32_t op_handle,
							 const void *part_data,
							 uint32_t partd_length,
							 uint32_t case_buf,
							 uint32_t chunk_id)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t org = 0;
	size_t initial_size;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, DS_BIG_SIZE,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	/*if $IN_caseBuffer$ = OUTPUT_BUFFER_TOO_SHORT(2)
		then Param[3].memref.size=1  */
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &org);

	/* Store the buffer from "shm2" in
	 * "buffer_encrypted_chunks[$IN_chunkNumber$]"
	 * which will be reused for the
	 * Invoke_Crypto_TEE_AEUpdate_for_decryption function
	 */
	if (res == TEEC_SUCCESS) {
		buffer_encrypted_chunks[chunk_id].size =
			op.params[3].memref.size;
		buffer_encrypted_chunks[chunk_id].buffer = malloc(
			buffer_encrypted_chunks[chunk_id].size);
		memcpy(buffer_encrypted_chunks[chunk_id].buffer,
		       SHARE_MEM02->buffer,
		       buffer_encrypted_chunks[chunk_id].size);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_AEUpdate_for_decryption(ADBG_Case_t *c,
							 TEEC_Session *s,
							 uint32_t cmd,
							 uint32_t op_handle,
							 const void *part_data,
							 size_t partd_length,
							 uint32_t case_buf,
							 uint32_t chunk_id)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01,
					buffer_encrypted_chunks[chunk_id].size,
					TEEC_MEM_INPUT,
					buffer_encrypted_chunks[chunk_id].size,
					buffer_encrypted_chunks[chunk_id].
						buffer, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, partd_length,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01, partd_length)
	/*if $IN_caseBuffer$ = OUTPUT_BUFFER_TOO_SHORT(2)
		then Param[3].memref.size=1*/
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	/* Save the buffer from "SharedMem2" into
	 * "buffer_decrypted_chunks[$IN_chunkNumber$]"
	 * in order to collapse all buffers returned for
	 * AEUpdate_for_decryption,
	 * which will be used in AEDecryptFinal
	 */
	if (res == TEEC_SUCCESS) {
		if (!ADBG_EXPECT_BUFFER(c, part_data, partd_length,
					SHARE_MEM02->buffer,
					op.params[3].memref.size))
			res = TEEC_ERROR_GENERIC;

		buffer_decrypted_chunks[chunk_id].size =
			op.params[3].memref.size;
		buffer_decrypted_chunks[chunk_id].buffer = malloc(
			buffer_decrypted_chunks[chunk_id].size);
		memcpy(buffer_decrypted_chunks[chunk_id].buffer,
		       SHARE_MEM02->buffer,
		       buffer_decrypted_chunks[chunk_id].size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_AEUpdateAAD(ADBG_Case_t *c, TEEC_Session *s,
					     uint32_t cmd,
					     uint32_t op_handle,
					     const void *aad_data,
					     size_t aad_length)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, aad_length,
					TEEC_MEM_INPUT, aad_length,
					aad_data, mem01_exit)

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	/* Save the $IN_AAD_Value$ for AAD for later cryptographic computation
	 */
	if (res == TEEC_SUCCESS) {
		saved_aad_value.buffer = malloc(aad_length);
		saved_aad_value.size = aad_length;
		memcpy(saved_aad_value.buffer, aad_data, aad_length);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_AEEncryptFinal(ADBG_Case_t *c, TEEC_Session *s,
						uint32_t cmd,
						uint32_t op_handle,
						const void *part_data,
						size_t partd_length,
						const void *full_data,
						size_t fdata_length,
						uint32_t case_buf,
						uint32_t chunk_id)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_partd_size, initial_fdata_size;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, partd_length,
			       TEEC_MEM_OUTPUT, mem02_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM03, partd_length,
			       TEEC_MEM_OUTPUT, mem03_exit)

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	switch (case_buf) {
	case OUTPUT_BUFFER_TOO_SHORT:
		/*if $IN_caseBuffer$ =
			OUTPUT_BUFFER_TOO_SHORT(2) then Param[3].memref.size=1*/
		SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, SHARE_MEM03,
						      SHARE_MEM03->size)
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
		break;
	case TAG_BUFFER_TOO_SHORT:
		SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, SHARE_MEM03, 1)
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
		break;
	case OUTPUT_BUFFER_NORMAL:
	default:
		SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, SHARE_MEM03,
						      SHARE_MEM03->size)
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
		break;
	}
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_partd_size = op.params[2].memref.size;
	initial_fdata_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Store the buffer from "shm2" in
		 * "buffer_encrypted_chunks[$IN_chunkNumber$]"
		 * which will be reused for
		 * the Invoke_Crypto_TEE_AEDecryptFinal function
		 */
		buffer_encrypted_chunks[chunk_id].size =
			op.params[3].memref.size;
		buffer_encrypted_chunks[chunk_id].buffer = malloc(
			buffer_encrypted_chunks[chunk_id].size);
		memcpy(buffer_encrypted_chunks[chunk_id].buffer,
		       SHARE_MEM02->buffer,
		       buffer_encrypted_chunks[chunk_id].size);

		/* Store the tag from "SharedMem3" which will be reused for the
		 * Invoke_Crypto_TEE_AEDecryptFinal function
		 */
		ae_encrypt_tag.size = op.params[2].memref.size;
		ae_encrypt_tag.buffer = malloc(ae_encrypt_tag.size);
		memcpy(ae_encrypt_tag.buffer, SHARE_MEM03->buffer,
		       ae_encrypt_tag.size);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		if (initial_partd_size == op.params[2].memref.size)
			ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_fdata_size, <,
						     op.params[3].memref.size);
		else
			ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_partd_size, <,
						     op.params[2].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM03);
mem03_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_AEDecryptFinal(ADBG_Case_t *c, TEEC_Session *s,
						uint32_t cmd,
						uint32_t op_handle,
						const void *part_data,
						size_t partd_length,
						const void *full_data,
						size_t fdata_length,
						uint32_t case_buf,
						enum mac_validity mac_case,
						uint32_t chunk_id)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01,
					buffer_encrypted_chunks[chunk_id].size,
					TEEC_MEM_INPUT,
					buffer_encrypted_chunks[chunk_id].size,
					buffer_encrypted_chunks[chunk_id].
						buffer, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, partd_length,
			       TEEC_MEM_OUTPUT, mem02_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM03, ae_encrypt_tag.size,
			       TEEC_MEM_INPUT, mem03_exit)
	/* Fill "SharedMem3" with the tag previously
	 * saved in Invoke_Crypto_AEEncryptFinal
	 * (with an error (one bit changed) if $IN_caseMac$ = INVALID_MAC)
	 */
	if (ae_encrypt_tag.buffer != NULL) {
		memcpy(SHARE_MEM03->buffer, ae_encrypt_tag.buffer,
		       ae_encrypt_tag.size);

		if (mac_case == INVALID_MAC)
			BIT_CHANGE(*(uint32_t *)SHARE_MEM03->buffer, 4);
	}

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, SHARE_MEM03,
					      SHARE_MEM03->size)
	/*if $IN_caseBuffer$ = OUTPUT_BUFFER_TOO_SHORT(2)
		then Param[3].memref.size=1*/
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Save the buffer from "SharedMem2" to
		 * "buffer_decrypted_chunks[$IN_chunkNumber$]"
		 */
		buffer_decrypted_chunks[chunk_id].size =
			op.params[3].memref.size;
		buffer_decrypted_chunks[chunk_id].buffer = malloc(
			buffer_decrypted_chunks[chunk_id].size);
		memcpy(buffer_decrypted_chunks[chunk_id].buffer,
		       SHARE_MEM02->buffer,
		       buffer_decrypted_chunks[chunk_id].size);

		/* Compare the data in clear $IN_fullDataValue$ and with
		 * collapsed buffers from table
		 * "buffer_decrypted_chunks" and check they are equals
		 */
		struct crypto_buffer collapsed;
		CRYPTO_INIT(collapsed);
		collapse_crypto_buffers(buffer_decrypted_chunks, &collapsed);
		ADBG_EXPECT_BUFFER(c, full_data, fdata_length, collapsed.buffer,
				   collapsed.size);
		CRYPTO_FREE(collapsed);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM03);
mem03_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_GenerateRandom(ADBG_Case_t *c, TEEC_Session *s,
						uint32_t cmd)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM01, BIG_SIZE,
			       TEEC_MEM_OUTPUT, mem01_exit)

	SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE,
					 TEEC_MEMREF_PARTIAL_OUTPUT);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	/* Check that the buffer shm1 is not empty
	 * + Check that this random value is
	 * different of a previous call of this command
	 */
	if (res == TEEC_SUCCESS) {
		if (ADBG_EXPECT_COMPARE_SIGNED(c, 0, !=,
					       op.params[3].memref.size)) {
			if (saved_random.buffer != NULL) {
				(void)ADBG_EXPECT_COMPARE_SIGNED(c, 0, !=,
								memcmp(
								SHARE_MEM01
								->
								buffer,
								saved_random
								.
								buffer,
								op.
								params[3].
								memref.
								size));
				free(saved_random.buffer);
				saved_random.size = 0;
			}

			saved_random.size = op.params[3].memref.size;
			saved_random.buffer = malloc(saved_random.size);
			memcpy(saved_random.buffer, SHARE_MEM01->buffer,
			       saved_random.size);
		}
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_DigestUpdate(ADBG_Case_t *c, TEEC_Session *s,
					      uint32_t cmd, uint32_t op_handle,
					      const void *part_data,
					      size_t partd_length)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_DigestDoFinal(ADBG_Case_t *c, TEEC_Session *s,
					       uint32_t cmd, uint32_t op_handle,
					       const void *part_data,
					       size_t partd_length,
					       const void *full_data,
					       size_t fdata_length,
					       uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, fdata_length,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = op_handle;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE,
					 TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Compute the hash of $IN_fullDataValue$
		 * and compare it to "shm2"
		 */
		struct crypto_buffer tmp_dgst;
		CRYPTO_INIT(tmp_dgst);
		ADBG_EXPECT_TEEC_SUCCESS(c, res = calculate_digest(c, full_data,
								   fdata_length,
								   &tmp_dgst));
		ADBG_EXPECT_BUFFER(c, tmp_dgst.buffer, tmp_dgst.size,
				   SHARE_MEM02->buffer, tmp_dgst.size);

		/* Store the Digest value which can be reused for a next call to
		 * TEE_AsymmetricSignDigest or TEE_AsymmetricVerifyDigest
		 */
		CRYPTO_FREE(saved_digest);
		saved_digest.size = op.params[3].memref.size;
		saved_digest.buffer = malloc(saved_digest.size);
		memcpy(saved_digest.buffer, SHARE_MEM02->buffer,
		       saved_digest.size);
		CRYPTO_FREE(tmp_dgst);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result
Invoke_Crypto_AsymmetricSignDigest(ADBG_Case_t *c, TEEC_Session *s,
				   uint32_t cmd, uint32_t oph,
				   const void *full_data,
				   size_t fdata_length,
				   uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;
	size_t sz;

	/* Fill SharedMem1 with the previously stored Digest
		value after TEE_DigestDoFinal */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01,
					saved_digest.size,
					TEEC_MEM_INPUT, saved_digest.size,
					saved_digest.buffer, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, 512,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = oph;

	sz = SHARE_MEM01->size;
	if (case_buf == INPUT_BUFFER_TOO_SHORT)
		sz--;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01, sz)

	if (case_buf == OUTPUT_BUFFER_TOO_SHORT)
		sz = 1;
	else
		sz = SHARE_MEM02->size;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, sz)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Compute a Verify_Signature of the signature
		 * store under "SharedMem2"
		 */
		struct crypto_buffer s_dgst;
		CRYPTO_INIT(s_dgst);
		s_dgst.size = op.params[3].memref.size;
		s_dgst.buffer = malloc(s_dgst.size);
		memcpy(s_dgst.buffer, SHARE_MEM02->buffer, s_dgst.size);
		ADBG_EXPECT(c, true, verify_digest(c, &s_dgst));
		CRYPTO_FREE(s_dgst);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result
Invoke_Crypto_AsymmetricVerifyDigest(ADBG_Case_t *c, TEEC_Session *s,
				     uint32_t cmd, uint32_t oph,
				     const void *full_data,
				     size_t fdata_length, uint32_t buffer_case,
				     uint32_t valid_sig)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	uint32_t dec_input = 0;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, fdata_length,
					TEEC_MEM_INPUT,
					saved_digest.size, saved_digest.buffer, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, 512,
			       TEEC_MEM_INPUT, mem02_exit)

	struct crypto_buffer signed_dgst;
	CRYPTO_INIT(signed_dgst);
	res = sign_digest(c, &saved_digest, &signed_dgst);

	/* Fill "SharedMem2" with the valid computed signature based on
	 * the previously stored Digest value after TEE_DigestDoFinal
	 */
	if (signed_dgst.buffer != NULL) {
		memcpy(SHARE_MEM02->buffer, signed_dgst.buffer,
		       signed_dgst.size);

		if (valid_sig != VALID_SIGNATURE) {
			/*make it invalid*/
			BIT_CHANGE(*(uint32_t *)SHARE_MEM02->buffer, 4);
		}
	}

	if (buffer_case == INPUT_BUFFER_TOO_SHORT)
		dec_input = 1;

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      saved_digest.size - dec_input)
	SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
					      signed_dgst.size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE,
					 TEEC_MEMREF_PARTIAL_INPUT);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	CRYPTO_FREE(signed_dgst);

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_AsymmetricEncrypt(ADBG_Case_t *c,
						   TEEC_Session *s,
						   uint32_t cmd,
						   uint32_t oph,
						   const void *full_data,
						   size_t fdata_length,
						   uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;

	/* Fill SharedMem1 with full_data */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, fdata_length,
					TEEC_MEM_INPUT, fdata_length,
					full_data, mem01_exit)


	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, 512,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	/*if $IN_caseBuffer$ = OUTPUT_BUFFER_TOO_SHORT(2)
		then Param[3].memref.size=1*/
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Check that "SharedMem2" is not empty
		 * Store the value from "SharedMem2" to a
		 * "buffer_asym_encrypted",
		 * which will be reused in Invoke_Crypto_AsymmetricDecrypt
		 */
		if (ADBG_EXPECT_COMPARE_SIGNED(c, 0, !=,
					       op.params[3].memref.size)) {
			buffer_asym_encrypted.size = op.params[3].memref.size;
			buffer_asym_encrypted.buffer = malloc(
				buffer_asym_encrypted.size);
			memcpy(buffer_asym_encrypted.buffer,
			       SHARE_MEM02->buffer, buffer_asym_encrypted.size);
		}
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_AsymmetricDecrypt(ADBG_Case_t *c,
						   TEEC_Session *s,
						   uint32_t cmd,
						   uint32_t op_handle,
						   const void *full_data,
						   size_t fdata_length,
						   uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;
	const char *expected_res;
	size_t expected_size;
	size_t sz;

	/* Fill SharedMem1 with buffer_asym_encrypted */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01,
					buffer_asym_encrypted.size,
					TEEC_MEM_INPUT,
					buffer_asym_encrypted.size,
					buffer_asym_encrypted.buffer, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, 512,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = op_handle;

	sz = buffer_asym_encrypted.size;
	if (case_buf == INPUT_BUFFER_TOO_SHORT)
		sz--;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01, sz);

	if (case_buf == OUTPUT_BUFFER_TOO_SHORT)
		sz = 1;
	else
		sz = SHARE_MEM02->size;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, sz);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	expected_res = full_data;
	expected_size =  fdata_length;
	if (saved_alloc.algo == TEE_ALG_RSA_NOPAD) {
		/*
		 * According to GP 1.1, no pad encrypting TEE_ALG_RSA_NOPAD
		 * follows "PKCS #1 (RSA primitive)", as stated in
		 * ftp://ftp.rsasecurity.com/pub/pkcs/pkcs-1/pkcs-1v2-1.pdf
		 * Page 10, it is stated that RSA primitives RSAEP and RSADP
		 * outputs "an integer between 0 and n-1". Hence the
		 * leading 0s must not be taken into account when checking
		 * the reference
		 */
		while (expected_size && expected_res[0] == 0) {
			expected_size--;
			expected_res++;
		}
	}

	if (res == TEEC_SUCCESS) {
		/* Compare the clear data in
		 * $IN_fullDataValue$ with "SharedMem2"
		 * and check they are equal
		 */
		ADBG_EXPECT_BUFFER(c, expected_res, expected_size,
				   SHARE_MEM02->buffer,
				   op.params[3].memref.size);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_CopyOperation(ADBG_Case_t *c, TEEC_Session *s,
					       uint32_t cmd, uint32_t dst_oph,
					       uint32_t src_oph)
{
	return Invoke_Simple_Function_v1(c, s, cmd, dst_oph, src_oph);
}

static TEEC_Result Invoke_Crypto_MACInit(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *iv, size_t iv_len)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM06, iv_len,
					TEEC_MEM_INPUT, iv_len, iv, mem06_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM06, iv_len)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	/* save the $IN_InitialisationVector$ for
	 * later computation of encryptedData
	 */
	if (iv_len != 0) {
		CRYPTO_FREE(saved_mac_iv);
		saved_mac_iv.size = iv_len;
		saved_mac_iv.buffer = malloc(iv_len);
		memcpy(saved_mac_iv.buffer, iv, iv_len);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM06);
mem06_exit:
	return res;
}

/*CMD_MACUpdate*/
static TEEC_Result Invoke_Crypto_MACUpdate(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *part_data, size_t partd_length)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

/*CMD_MACCompareFinal*/
static TEEC_Result Invoke_Crypto_MACCompareFinal(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *part_data, size_t partd_length,
	const void *full_data, size_t fdata_length,
	enum mac_validity mac_case)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	/* Fill SharedMem1 with part_data */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, fdata_length,
			       TEEC_MEM_INPUT, mem02_exit)

	/* Fill SharedMem2 with valid computed MAC of full_data */
	struct crypto_buffer mac;
	CRYPTO_INIT(mac);
	res = mac_compute_final(c, s, full_data, fdata_length, &mac);

	if (mac.buffer != NULL) {
		memcpy(SHARE_MEM02->buffer, mac.buffer, mac.size);

		if (mac_case != VALID_MAC) {
			/* change one bit from the valid
				MAC to make it invalid. */
			BIT_CHANGE(*(uint32_t *)SHARE_MEM02->buffer, 4);
		}
	}

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	SET_SHARED_MEMORY_OPERATION_PARAMETER(2, 0, SHARE_MEM02, mac.size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	CRYPTO_FREE(mac);

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

/*CMD_MACComputeFinal*/
static TEEC_Result Invoke_Crypto_MACComputeFinal(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *part_data, size_t partd_length,
	const void *full_data, size_t fdata_length, uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;

	/* Fill SharedMem1 with part_data */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, fdata_length,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	/* Compute the MAC of $IN_fullDataValue$ and
		compare it to "SharedMem2" */
	if (res == TEEC_SUCCESS) {
		struct crypto_buffer tmp_mac;
		CRYPTO_INIT(tmp_mac);
		ADBG_EXPECT_TEEC_SUCCESS(c, res =
						 mac_compute_final(c, s,
								   full_data,
								   fdata_length,
								   &tmp_mac));

		if (res != TEEC_SUCCESS)
			goto exit;

		ADBG_EXPECT_COMPARE_SIGNED(c, 0, ==,
					   memcmp(SHARE_MEM02->buffer,
						  tmp_mac.buffer,
						  op.params[3].memref.size));
		CRYPTO_FREE(tmp_mac);
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

/*CMD_CipherInit*/
static TEEC_Result Invoke_Crypto_CipherInit(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *iv, size_t iv_len)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, iv_len,
					TEEC_MEM_INPUT, iv_len, iv, mem01_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Save the $IN_InitialisationVector$ for later
		 * computation of encryptedData
		 */
		if (iv != NULL) {
			CRYPTO_FREE(saved_cipher_iv);
			saved_cipher_iv.size = iv_len;
			saved_cipher_iv.buffer = malloc(iv_len);
			memcpy(saved_cipher_iv.buffer, iv, iv_len);
		}
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	CRYPTO_FREE(saved_cipher_update);
	return res;
}
/*CMD_CipherUpdate*/
static TEEC_Result Invoke_Crypto_CipherUpdate(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *part_data, uint32_t partd_length,
	uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, partd_length,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	/*if $IN_caseBuffer$ = OUTPUT_BUFFER_TOO_SHORT(2)
		then Param[3].memref.size=1*/
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Save the buffer returned in "SharedMem2" in order
		 * to collapse all buffers returned for CipherUpdate,
		 * which will be used in CipherDoFinal
		 */
		if (op.params[3].memref.size != 0) {
			void *tmp = realloc(saved_cipher_update.buffer,
					    saved_cipher_update.size +
					    op.params[3].memref.size);
			saved_cipher_update.buffer = tmp;
			memcpy(
				saved_cipher_update.buffer +
				saved_cipher_update.size, SHARE_MEM02->buffer,
				op.params[3].memref.size);
			saved_cipher_update.size += op.params[3].memref.size;
		}
	} else if (res == TEEC_ERROR_SHORT_BUFFER) {
		ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
					     op.params[3].memref.size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

/*CMD_CipherDoFinal*/
static TEEC_Result Invoke_Crypto_CipherDoFinal(
	ADBG_Case_t *c, TEEC_Session *s,
	uint32_t cmd, uint32_t oph,
	const void *part_data, size_t partd_length,
	const void *full_data, size_t fulld_length, uint32_t case_buf)
{
	TEEC_Result res;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t ret_orig;
	size_t initial_size;

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM01, partd_length,
					TEEC_MEM_INPUT, partd_length,
					part_data, mem01_exit)
	/* used fulld_length instead of partd_length as
		described in the Adaptation layer specification.*/
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM02, fulld_length,
			       TEEC_MEM_OUTPUT, mem02_exit)

	op.params[0].value.a = oph;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM01,
					      SHARE_MEM01->size)
	/*if $IN_caseBuffer$ = OUTPUT_BUFFER_TOO_SHORT(2)
		then Param[3].memref.size=1*/
	if (case_buf == OUTPUT_BUFFER_TOO_SHORT) {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02, 1)
	} else {
		SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM02,
						      SHARE_MEM02->size)
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);
	initial_size = op.params[3].memref.size;

	res = TEEC_InvokeCommand(s, cmd, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		/* Append the buffer returned in "SharedMem2"
		 * to the previously buffers
		 * returned for CipherUpdate => "collapsed_buffers"
		 */
		if (op.params[3].memref.size != 0) {
			void *tmp = realloc(saved_cipher_update.buffer,
					    saved_cipher_update.size +
					    op.params[3].memref.size);
			saved_cipher_update.buffer = tmp;
			memcpy(
				saved_cipher_update.buffer +
				saved_cipher_update.size, SHARE_MEM02->buffer,
				op.params[3].memref.size);
			saved_cipher_update.size += op.params[3].memref.size;
		}

		/* Compute the ciphered data of
		 * $IN_fullDataValue$ and compare it
		 * to "collapsed_buffers"
		 */
		struct crypto_buffer full_ciphered_data;
		CRYPTO_INIT(full_ciphered_data);
		ADBG_EXPECT_TEEC_SUCCESS(c, res =
						 cipher_do_final(c, s,
							full_data,
							fulld_length,
							&
							full_ciphered_data));

		if (res == TEEC_SUCCESS) {
			ADBG_EXPECT_BUFFER(c, full_ciphered_data.buffer,
					   full_ciphered_data.size,
					   saved_cipher_update.buffer,
					   saved_cipher_update.size);
		} else if (res == TEEC_ERROR_SHORT_BUFFER) {
			ADBG_EXPECT_COMPARE_UNSIGNED(c, initial_size, <,
						     op.params[3].memref.size);
		}

		CRYPTO_FREE(full_ciphered_data);
		CRYPTO_FREE(saved_cipher_update);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM02);
mem02_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM01);
mem01_exit:
	return res;
}

static TEEC_Result Invoke_Crypto_FreeOperation(ADBG_Case_t *c, TEEC_Session *s,
					       uint32_t cmd, uint32_t oph)
{
	return Invoke_Simple_Function_v1(c, s, cmd, oph, 0);
}


static const EVP_MD *hash_algo_to_md(uint32_t algo)
{
	switch (algo) {
	case TEE_ALG_MD5:
		return EVP_md5();
	case TEE_ALG_SHA1:
		return EVP_sha1();
	case TEE_ALG_SHA224:
		return EVP_sha224();
	case TEE_ALG_SHA256:
		return EVP_sha256();
	case TEE_ALG_SHA384:
		return EVP_sha384();
	case TEE_ALG_SHA512:
		return EVP_sha512();
	default:
		return NULL;
	}
}


static TEEC_Result calculate_digest(ADBG_Case_t *c, const void *data,
				    size_t data_length,
				    struct crypto_buffer *digest)
{
	const EVP_MD *md = hash_algo_to_md(saved_alloc.algo);
	void *buf = NULL;
	unsigned int sz = 0;

	if (!ADBG_EXPECT_NOT_NULL(c, md))
		return TEEC_ERROR_NOT_SUPPORTED;
	sz = EVP_MD_size(md);
	buf = calloc(1, sz);
	if (!buf)
		return TEEC_ERROR_OUT_OF_MEMORY;

	if (EVP_Digest(data, data_length, buf, &sz, md, NULL)) {
		digest->size = sz;
		digest->buffer = buf;
		return TEEC_SUCCESS;
	} else {
		free(buf);
		return TEEC_ERROR_GENERIC;
	}
}

struct key_attr {
	const void *val;
	size_t len;
};

#define KEY_ATTR(val_array) { .val = val_array, .len = sizeof(val_array) }

struct dsa_key_vals {
	struct key_attr priv;
	struct key_attr pub;
	struct key_attr prime;
	struct key_attr subprime;
	struct key_attr base;
};

static const struct dsa_key_vals dsa_key_vals[] = { {
		.prime = KEY_ATTR(TEE_ATTR_DSA_PRIME_768_VALUE01),
		.subprime = KEY_ATTR(TEE_ATTR_DSA_SUBPRIME_160_VALUE01),
		.base = KEY_ATTR(TEE_ATTR_DSA_BASE_768_VALUE01),
		.priv = KEY_ATTR(TEE_ATTR_DSA_PRIVATE_VALUE_160_VALUE01),
		.pub = KEY_ATTR(TEE_ATTR_DSA_PUBLIC_VALUE_768_VALUE01),
	}, {
		.prime = KEY_ATTR(TEE_ATTR_DSA_PRIME_2048_VALUE01),
		.subprime = KEY_ATTR(TEE_ATTR_DSA_SUBPRIME_224_VALUE01),
		.base = KEY_ATTR(TEE_ATTR_DSA_BASE_2048_VALUE01),
		.priv = KEY_ATTR(TEE_ATTR_DSA_PRIVATE_VALUE_224_VALUE01),
		.pub = KEY_ATTR(TEE_ATTR_DSA_PUBLIC_VALUE_2048_VALUE01),
	}, {
		.prime = KEY_ATTR(TEE_ATTR_DSA_PRIME_3072_VALUE01),
		.subprime = KEY_ATTR(TEE_ATTR_DSA_SUBPRIME_256_VALUE01),
		.base = KEY_ATTR(TEE_ATTR_DSA_BASE_3072_VALUE01),
		.priv = KEY_ATTR(TEE_ATTR_DSA_PRIVATE_VALUE_256_VALUE01),
		.pub = KEY_ATTR(TEE_ATTR_DSA_PUBLIC_VALUE_3072_VALUE01),
	},
};


struct ecdsa_key_vals {
	struct key_attr priv;
	struct key_attr x;
	struct key_attr y;
	uint32_t curve;
	uint32_t key_size_bytes;
};
static const struct ecdsa_key_vals ecdsa_key_vals[] = { {
		.priv = KEY_ATTR(TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P192_VALUE01),
		.x = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P192_VALUE01),
		.y = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P192_VALUE01),
		.curve = 1, /* TEE_ECC_CURVE_NIST_P192 */
		.key_size_bytes = 24,
	}, {
		.priv = KEY_ATTR(TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P224_VALUE01),
		.x = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P224_VALUE01),
		.y = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P224_VALUE01),
		.curve = 2 /* TEE_ECC_CURVE_NIST_P224 */,
		.key_size_bytes = 28,
	}, {
		.priv = KEY_ATTR(TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P256_VALUE01),
		.x = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P256_VALUE01),
		.y = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P256_VALUE01),
		.curve = 3 /* TEE_ECC_CURVE_NIST_P256 */,
		.key_size_bytes = 32,
	}, {
		.priv = KEY_ATTR(TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P384_VALUE01),
		.x = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P384_VALUE01),
		.y = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P384_VALUE01),
		.curve = 4 /* TEE_ECC_CURVE_NIST_P384 */,
		.key_size_bytes = 48,
	}, {
		.priv = KEY_ATTR(TEE_ATTR_ECC_PRIVATE_VALUE_ECDSA_P521_VALUE01),
		.x = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_X_ECDSA_P521_VALUE01),
		.y = KEY_ATTR(TEE_ATTR_ECC_PUBLIC_VALUE_Y_ECDSA_P521_VALUE01),
		.curve = 5 /* TEE_ECC_CURVE_NIST_P521 */,
		.key_size_bytes = 66,
	},
};

struct rsa_key_vals {
	struct key_attr mod;
	struct key_attr pub;
	struct key_attr priv;
};

static const struct rsa_key_vals rsa_key_vals[] = { {
		.mod = KEY_ATTR(TEE_ATTR_RSA_MODULUS_VALUE01),
		.pub = KEY_ATTR(TEE_ATTR_RSA_PUBLIC_EXPONENT_VALUE01),
		.priv = KEY_ATTR(TEE_ATTR_RSA_PRIVATE_EXPONENT_VALUE01),
	},
};

static const void *get_dsa_key_vals(const void *attr)
{
	const size_t nelem = sizeof(dsa_key_vals) / sizeof(dsa_key_vals[0]);
	size_t n;

	for (n = 0; n < nelem; n++) {
		if (attr == dsa_key_vals[n].priv.val ||
		    attr == dsa_key_vals[n].pub.val ||
		    attr == dsa_key_vals[n].prime.val ||
		    attr == dsa_key_vals[n].subprime.val ||
		    attr == dsa_key_vals[n].base.val) {
			return dsa_key_vals + n;
		}
	}

	return NULL;
}

static const void *get_ecdsa_key_vals(const void *attr)
{
	const size_t nelem = sizeof(ecdsa_key_vals) / sizeof(ecdsa_key_vals[0]);
	size_t n;

	for (n = 0; n < nelem; n++) {
		if (attr == ecdsa_key_vals[n].priv.val ||
		    attr == ecdsa_key_vals[n].x.val ||
		    attr == ecdsa_key_vals[n].y.val)
			return ecdsa_key_vals + n;
	}

	return NULL;
}

static const void *get_rsa_key_vals(const void *attr)
{
	const size_t nelem = sizeof(rsa_key_vals) / sizeof(rsa_key_vals[0]);
	size_t n;

	for (n = 0; n < nelem; n++) {
		if (attr == rsa_key_vals[n].priv.val ||
		    attr == rsa_key_vals[n].mod.val ||
		    attr == rsa_key_vals[n].pub.val)
			return rsa_key_vals + n;
	}

	return NULL;
}

static BIGNUM *bn_from_key_attr(const struct key_attr *ka)
{
	return BN_bin2bn(ka->val, ka->len, NULL);
}

static DSA *dsa_from_key_vals(const struct dsa_key_vals *dkv)
{
	BIGNUM *priv_key = bn_from_key_attr(&dkv->priv);
	BIGNUM *pub_key = bn_from_key_attr(&dkv->pub);
	BIGNUM *q = bn_from_key_attr(&dkv->subprime);
	BIGNUM *p = bn_from_key_attr(&dkv->prime);
	BIGNUM *g = bn_from_key_attr(&dkv->base);
	DSA *dsa = DSA_new();

	if (!dsa || !priv_key || !pub_key || !p || !q || !g)
		goto err_free_all;

	if (!DSA_set0_pqg(dsa, p, q, g))
		goto err_free_all;

	if (!DSA_set0_key(dsa, pub_key, priv_key))
		goto err_free_priv_pub;

	return dsa;

err_free_all:
	BN_free(p);
	BN_free(q);
	BN_free(g);
err_free_priv_pub:
	BN_free(priv_key);
	BN_free(pub_key);
	DSA_free(dsa);

	return NULL;
}

static TEEC_Result dsa_sign_digest(ADBG_Case_t *c,
				   const struct dsa_key_vals *dkv,
				   const struct crypto_buffer *in_dgst,
				   struct crypto_buffer *out_dgst)
{
	TEEC_Result res = TEEC_SUCCESS;
	DSA *dsa = dsa_from_key_vals(dkv);
	const BIGNUM *r = NULL;
	const BIGNUM *s = NULL;
	DSA_SIG *sig = NULL;

	if (!dsa) {
		res = TEEC_ERROR_OUT_OF_MEMORY;
		ADBG_EXPECT_TEEC_SUCCESS(c, res);
		return res;
	}

	sig = DSA_do_sign(in_dgst->buffer, in_dgst->size, dsa);
	if (!sig) {
		res = TEEC_ERROR_OUT_OF_MEMORY;
		ADBG_EXPECT_TEEC_SUCCESS(c, res);
		goto out;
	}

	out_dgst->size = 2 * dkv->subprime.len; /* q */
	out_dgst->buffer = calloc(1, out_dgst->size);
	if (!out_dgst->buffer) {
		res = TEEC_ERROR_OUT_OF_MEMORY;
		ADBG_EXPECT_TEEC_SUCCESS(c, res);
		goto out;
	}

	DSA_SIG_get0(sig, &r, &s);
	BN_bn2bin(r, out_dgst->buffer + out_dgst->size / 2 - BN_num_bytes(r));
	BN_bn2bin(s, out_dgst->buffer + out_dgst->size - BN_num_bytes(s));
	res = TEEC_SUCCESS;

out:
	DSA_free(dsa);
	DSA_SIG_free(sig);

	return res;
}

static bool dsa_verify_digest(ADBG_Case_t *c, const struct dsa_key_vals *dkv,
			      const void *digest, size_t digest_len,
			      const struct crypto_buffer *in_sdgst)
{
	DSA *dsa = dsa_from_key_vals(dkv);
	DSA_SIG *sig = NULL;
	BIGNUM *r = NULL;
	BIGNUM *s = NULL;
	bool res = false;

	if (!ADBG_EXPECT_NOT_NULL(c, dsa))
		return false;

	sig = DSA_SIG_new();
	if (!ADBG_EXPECT_NOT_NULL(c, sig))
		goto out;

	r = BN_bin2bn(in_sdgst->buffer, in_sdgst->size / 2, NULL);
	if (!ADBG_EXPECT_NOT_NULL(c, r))
		goto out;
	s = BN_bin2bn(in_sdgst->buffer + in_sdgst->size / 2,
		      in_sdgst->size / 2, NULL);
	if (!ADBG_EXPECT_NOT_NULL(c, s)) {
		BN_free(r);
		goto out;
	}
	DSA_SIG_set0(sig, r, s);

	res = DSA_do_verify(digest, digest_len, sig, dsa);
out:
	DSA_free(dsa);
	DSA_SIG_free(sig);

	return res;
}

static int curve_to_nid(uint32_t curve)
{
	switch (curve) {
	case 1 /* TEE_ECC_CURVE_NIST_P192 */:
		return NID_X9_62_prime192v1;
	case 2 /* TEE_ECC_CURVE_NIST_P224 */:
		return NID_secp224r1;
	case 3 /* TEE_ECC_CURVE_NIST_P256 */:
		return NID_X9_62_prime256v1;
	case 4 /* TEE_ECC_CURVE_NIST_P384 */:
		return NID_secp384r1;
	case 5 /* TEE_ECC_CURVE_NIST_P521 */:
		return NID_secp521r1;
	default:
		return -1;
	}
}

static EC_KEY *ecdsa_from_key_vals(const struct ecdsa_key_vals *kv)
{
	EC_KEY *key = EC_KEY_new_by_curve_name(curve_to_nid(kv->curve));
	BIGNUM *priv_key = bn_from_key_attr(&kv->priv);
	BIGNUM *x = bn_from_key_attr(&kv->x);
	BIGNUM *y = bn_from_key_attr(&kv->y);

	if (!key || !priv_key || !x || !y)
		goto err_free_all;

	if (!EC_KEY_set_private_key(key, priv_key))
		goto err_free_all;

	if (!EC_KEY_set_public_key_affine_coordinates(key, x, y))
		goto err_free_xy;

	return key;

err_free_all:
	BN_free(priv_key);
err_free_xy:
	BN_free(x);
	BN_free(y);
	EC_KEY_free(key);

	return NULL;
}


static TEEC_Result ecdsa_sign_digest(ADBG_Case_t *c,
				     const struct ecdsa_key_vals *kv,
				     const struct crypto_buffer *in_dgst,
				     struct crypto_buffer *out_dgst)
{
	TEEC_Result res = TEEC_ERROR_OUT_OF_MEMORY;
	EC_KEY *key = ecdsa_from_key_vals(kv);
	const BIGNUM *r = NULL;
	const BIGNUM *s = NULL;
	ECDSA_SIG *sig = NULL;

	if (!ADBG_EXPECT_NOT_NULL(c, key))
		return TEEC_ERROR_OUT_OF_MEMORY;

	sig = ECDSA_do_sign(in_dgst->buffer, in_dgst->size, key);
	if (!ADBG_EXPECT_NOT_NULL(c, sig))
		goto out;

	out_dgst->size = 2 * kv->key_size_bytes;
	out_dgst->buffer = calloc(1, out_dgst->size);
	if (!ADBG_EXPECT_NOT_NULL(c, out_dgst->buffer))
		goto out;

	ECDSA_SIG_get0(sig, &r, &s);
	BN_bn2bin(r, out_dgst->buffer + out_dgst->size / 2 - BN_num_bytes(r));
	BN_bn2bin(s, out_dgst->buffer + out_dgst->size - BN_num_bytes(s));
	res = TEEC_SUCCESS;
out:
	EC_KEY_free(key);
	ECDSA_SIG_free(sig);

	return res;
}

static bool ecdsa_verify_digest(ADBG_Case_t *c,
				const struct ecdsa_key_vals *kv,
				const void *digest, size_t digest_len,
				const struct crypto_buffer *in_sdgst)
{
	EC_KEY *key = ecdsa_from_key_vals(kv);
	ECDSA_SIG *sig = NULL;
	BIGNUM *r = NULL;
	BIGNUM *s = NULL;
	bool res = false;

	if (!ADBG_EXPECT_NOT_NULL(c, key))
		return false;

	sig = ECDSA_SIG_new();
	if (!ADBG_EXPECT_NOT_NULL(c, sig))
		goto out;

	r = BN_bin2bn(in_sdgst->buffer, in_sdgst->size / 2, NULL);
	if (!ADBG_EXPECT_NOT_NULL(c, r))
		goto out;
	s = BN_bin2bn(in_sdgst->buffer + in_sdgst->size / 2,
		      in_sdgst->size / 2, NULL);
	if (!ADBG_EXPECT_NOT_NULL(c, s)) {
		BN_free(r);
		goto out;
	}
	ECDSA_SIG_set0(sig, r, s);

	res = ECDSA_do_verify(digest, digest_len, sig, key);
out:
	EC_KEY_free(key);
	ECDSA_SIG_free(sig);

	return res;
}

static RSA *rsa_from_key_vals(const struct rsa_key_vals *kv)
{
	BIGNUM *priv = bn_from_key_attr(&kv->priv);
	BIGNUM *pub = bn_from_key_attr(&kv->pub);
	BIGNUM *mod = bn_from_key_attr(&kv->mod);
	RSA *key = RSA_new();

	if (!key || !priv || !pub || !mod)
		goto err_free_all;

	if (!RSA_set0_key(key, mod, pub, priv))
		goto err_free_all;

	return key;

err_free_all:
	BN_free(priv);
	BN_free(pub);
	BN_free(mod);
	RSA_free(key);

	return NULL;
}

static const EVP_MD *algo_to_md(uint32_t algo)
{
	switch (algo) {
	case TEE_ALG_RSASSA_PKCS1_V1_5_MD5:
		return EVP_md5();
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA1:
	case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA1:
		return EVP_sha1();
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA224:
	case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA224:
		return EVP_sha224();
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA256:
	case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256:
		return EVP_sha256();
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA384:
	case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA384:
		return EVP_sha384();
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA512:
	case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA512:
		return EVP_sha512();
	default:
		return NULL;
	}
}

static bool padding_is_pkcs1_v1_5(uint32_t algo)
{
	switch (algo) {
	case TEE_ALG_RSASSA_PKCS1_V1_5_MD5:
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA1:
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA224:
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA256:
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA384:
	case TEE_ALG_RSASSA_PKCS1_V1_5_SHA512:
		return true;
	default:
		return false;
	}
}

static int ossl_err_cb(const char *str, size_t len, void *u)
{
	warnx("%*s", (int)len, str);
	return 0;
}

static void print_ossl_errors(const char *func)
{
	warnx("%s failed", func);
	ERR_print_errors_cb(ossl_err_cb, NULL);
}


static TEEC_Result rsa_sign_digest(ADBG_Case_t *c,
				   const struct rsa_key_vals *kv,
				   uint32_t algo,
				   const struct crypto_buffer *in_dgst,
				   struct crypto_buffer *out_dgst)
{
	TEEC_Result res = TEEC_SUCCESS;
	const EVP_MD *md = algo_to_md(algo);
	RSA *key = NULL;
	unsigned int sz = 0;
	void *buf = NULL;

	if (!ADBG_EXPECT_NOT_NULL(c, md))
		return TEEC_ERROR_GENERIC;

	key = rsa_from_key_vals(kv);
	if (!ADBG_EXPECT_NOT_NULL(c, key))
		return TEEC_ERROR_OUT_OF_MEMORY;

	sz = RSA_size(key);
	buf = calloc(1, sz);
	if (!ADBG_EXPECT_NOT_NULL(c, buf)) {
		res = TEEC_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	if (padding_is_pkcs1_v1_5(algo)) {
		if (!RSA_sign(EVP_MD_nid(md), in_dgst->buffer, in_dgst->size,
			      buf, &sz, key)) {
			print_ossl_errors("RSA_sign");
			res = TEEC_ERROR_GENERIC;
		}
	} else {
		/*
		 * Important to use RSA_PSS_SALTLEN_DIGEST since the
		 * implementation may expect the size of the hash since the
		 * salt length isn't supplied as a paramter to
		 * TEE_AsymmetricVerifyDigest().
		 */
		if (!RSA_padding_add_PKCS1_PSS_mgf1(key, buf, in_dgst->buffer,
						    md, md,
						    RSA_PSS_SALTLEN_DIGEST)) {
			print_ossl_errors("RSA_padding_add_PKCS1_PSS_mgf1");
			res = TEEC_ERROR_GENERIC;
		} else if (!RSA_private_encrypt(sz, buf, buf, key,
						RSA_NO_PADDING)) {
			print_ossl_errors("RSA_padding_add_PKCS1_PSS_mgf1");
			res = TEEC_ERROR_GENERIC;
		}
	}

	if (!res) {
		out_dgst->buffer = buf;
		out_dgst->size = sz;
	} else {
		free(buf);
	}

out:
	RSA_free(key);
	return res;

}

static bool rsa_verify_pss_digest(ADBG_Case_t *c, const EVP_MD *md, RSA *key,
				  const void *digest,
				  const struct crypto_buffer *in_sdgst)
{
	bool res = false;
	void *buf;
	int mlen;

	buf = calloc(1, RSA_size(key));
	if (!ADBG_EXPECT_NOT_NULL(c, buf))
		return false;

	mlen = RSA_public_decrypt(in_sdgst->size, in_sdgst->buffer,
				  buf, key, RSA_NO_PADDING);
	if (mlen < 0) {
		print_ossl_errors("RSA_public_decrypt");
		goto out;
	}
	res = RSA_verify_PKCS1_PSS_mgf1(key, digest, md, md, buf, -2);
	if (!res)
		print_ossl_errors("RSA_verify_PKCS1_PSS_mgf1");

out:
	free(buf);
	return res;

}

static bool rsa_verify_digest(ADBG_Case_t *c, const struct rsa_key_vals *kv,
			      uint32_t algo,
			      const void *digest, size_t digest_len,
			      const struct crypto_buffer *in_sdgst)
{
	const EVP_MD *md = algo_to_md(algo);
	RSA *key = NULL;
	bool res;

	if (!ADBG_EXPECT_NOT_NULL(c, md))
		return false;

	key = rsa_from_key_vals(kv);
	if (!ADBG_EXPECT_NOT_NULL(c, key))
		return false;

	if (padding_is_pkcs1_v1_5(algo)) {
		res = RSA_verify(EVP_MD_nid(md), digest, digest_len,
				 in_sdgst->buffer, in_sdgst->size, key);
		if (!res)
			print_ossl_errors("RSA_verify");
	} else {
		res = rsa_verify_pss_digest(c, md, key, digest, in_sdgst);
	}

	RSA_free(key);
	return res;
}

static TEEC_Result sign_digest(ADBG_Case_t *c,
			       const struct crypto_buffer *in_dgst,
			       struct crypto_buffer *out_dgst)
{
	const void *kv = NULL;
	TEEC_Result res = TEEC_SUCCESS;

	if (!ADBG_EXPECT(c, TEE_HANDLE_NULL, saved_obh.obh2))
		return TEEC_ERROR_GENERIC;

	if (!ADBG_EXPECT_NOT(c, TEE_HANDLE_NULL, saved_obh.obh1))
		return TEEC_ERROR_GENERIC;

	kv = get_dsa_key_vals(saved_key_pub_attr[saved_obh.obh1]);
	if (kv) {
		res = dsa_sign_digest(c, kv, in_dgst, out_dgst);
		goto out;
	}

	kv = get_ecdsa_key_vals(saved_key_pub_attr[saved_obh.obh1]);
	if (kv) {
		res = ecdsa_sign_digest(c, kv, in_dgst, out_dgst);
		goto out;
	}

	kv = get_rsa_key_vals(saved_key_pub_attr[saved_obh.obh1]);
	if (kv) {
		res = rsa_sign_digest(c, kv, saved_alloc.algo,
				      in_dgst, out_dgst);
		goto out;
	}

	res = TEEC_ERROR_GENERIC;
out:
	ADBG_EXPECT_TEEC_SUCCESS(c, res);
	return res;
}

static bool verify_digest(ADBG_Case_t *c, const struct crypto_buffer *in_sdgst)
{
	const void *kv = NULL;

	if (!ADBG_EXPECT(c, TEE_HANDLE_NULL, saved_obh.obh2))
		return false;

	if (!ADBG_EXPECT_NOT(c, TEE_HANDLE_NULL, saved_obh.obh1))
		return false;

	kv = get_dsa_key_vals(saved_key_pub_attr[saved_obh.obh1]);
	if (kv)
		return dsa_verify_digest(c, kv, saved_digest.buffer,
					 saved_digest.size, in_sdgst);
	kv = get_ecdsa_key_vals(saved_key_pub_attr[saved_obh.obh1]);
	if (kv)
		return ecdsa_verify_digest(c, kv, saved_digest.buffer,
					   saved_digest.size, in_sdgst);
	kv = get_rsa_key_vals(saved_key_pub_attr[saved_obh.obh1]);
	if (kv)
		return rsa_verify_digest(c, kv, saved_alloc.algo,
					 saved_digest.buffer,
					 saved_digest.size, in_sdgst);

	return ADBG_EXPECT_TRUE(c, false);
}

static TEEC_Result mac_compute_final(
	ADBG_Case_t *c, TEEC_Session *s,
	const void *full_data, size_t fdata_length,
	struct crypto_buffer *mac)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t op1 = 3;
	uint32_t ret_orig;

	res = Invoke_Crypto_AllocateOperation(c, s, CMD_Crypto_AllocateOperation,
					      saved_alloc.algo, TEE_MODE_MAC,
					      saved_alloc.obj_size, 0, op1);

	if (res != TEEC_SUCCESS)
		goto exit;

	if (saved_obh.obh2 != TEE_HANDLE_NULL) {
		res = Invoke_Crypto_SetOperationKey2(c, s, CMD_Crypto_SetOperationKey2,
						     op1, saved_obh.obh1,
						     saved_obh.obh2);

		if (res != TEEC_SUCCESS)
			goto exit;

	} else if (saved_obh.obh1 != TEE_HANDLE_NULL) {
		res = Invoke_Crypto_SetOperationKey(c, s, CMD_Crypto_SetOperationKey,
						    op1, saved_obh.obh1);

		if (res != TEEC_SUCCESS)
			goto exit;
	}

	res = Invoke_Crypto_MACInit(c, s, CMD_Crypto_MACInit, op1,
				    saved_mac_iv.buffer, saved_mac_iv.size);

	if (res != TEEC_SUCCESS)
		goto exit;

	/* CMD_MACComputeFinal */
	/* Fill SharedMem1 with full_data */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM04, fdata_length,
					TEEC_MEM_INPUT, fdata_length,
					full_data, mem04_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM05, fdata_length,
			       TEEC_MEM_OUTPUT, mem05_exit)

	op.params[0].value.a = (uint32_t)op1;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM04,
					      SHARE_MEM04->size)
	SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM05,
					      SHARE_MEM05->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);

	res = TEEC_InvokeCommand(s, CMD_Crypto_MACComputeFinal, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		mac->size = op.params[3].memref.size;
		mac->buffer = malloc(mac->size);
		memcpy(mac->buffer, SHARE_MEM05->buffer, mac->size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM05);
mem05_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM04);
mem04_exit:
	Invoke_Crypto_FreeOperation(c, s, CMD_Crypto_FreeOperation, op1);
exit:
	return res;
}

static TEEC_Result cipher_do_final(
	ADBG_Case_t *c, TEEC_Session *s,
	const void *full_data, size_t fdata_length,
	struct crypto_buffer *cipher)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t op1 = 3;
	uint32_t ret_orig;

	res = Invoke_Crypto_AllocateOperation(c, s, CMD_Crypto_AllocateOperation,
					      saved_alloc.algo,
					      TEE_MODE_ENCRYPT,
					      saved_alloc.obj_size, 0, op1);

	if (res != TEEC_SUCCESS)
		goto crypto_alloc;

	if (saved_obh.obh2 != TEE_HANDLE_NULL) {
		res = Invoke_Crypto_SetOperationKey2(c, s, CMD_Crypto_SetOperationKey2,
						     op1, saved_obh.obh1,
						     saved_obh.obh2);

		if (res != TEEC_SUCCESS)
			goto exit;

	} else if (saved_obh.obh1 != TEE_HANDLE_NULL) {
		res = Invoke_Crypto_SetOperationKey(c, s, CMD_Crypto_SetOperationKey,
						    op1, saved_obh.obh1);

		if (res != TEEC_SUCCESS)
			goto exit;

	}

	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM04, fdata_length,
					TEEC_MEM_INPUT,
					saved_cipher_iv.size,
					saved_cipher_iv.buffer,
					mem04_exit)

	op.params[0].value.a = (uint32_t)op1;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM04,
					      saved_cipher_iv.size)

	op.params[1].memref.offset = 0;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE,
					 TEEC_NONE);

	res = TEEC_InvokeCommand(s, CMD_Crypto_CipherInit, &op, &ret_orig);

	if (res != TEEC_SUCCESS)
		goto mem05_exit;

	TEEC_ReleaseSharedMemory(SHARE_MEM04);

	/* CMD_CipherDoFinal */
	/* Fill SharedMem1 with full_data */
	ALLOCATE_AND_FILL_SHARED_MEMORY(s->ctx, SHARE_MEM04, fdata_length,
					TEEC_MEM_INPUT, fdata_length,
					full_data, mem04_exit)
	ALLOCATE_SHARED_MEMORY(s->ctx, SHARE_MEM05, fdata_length,
			       TEEC_MEM_OUTPUT, mem05_exit)

	op.params[0].value.a = (uint32_t)op1;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, SHARE_MEM04,
					      SHARE_MEM04->size)
	SET_SHARED_MEMORY_OPERATION_PARAMETER(3, 0, SHARE_MEM05,
					      SHARE_MEM05->size)

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_MEMREF_PARTIAL_OUTPUT);

	res = TEEC_InvokeCommand(s, CMD_Crypto_CipherDoFinal, &op, &ret_orig);

	if (res == TEEC_SUCCESS) {
		cipher->size = op.params[3].memref.size;
		cipher->buffer = malloc(cipher->size);
		memcpy(cipher->buffer, SHARE_MEM05->buffer, cipher->size);
	}

	TEEC_ReleaseSharedMemory(SHARE_MEM05);
mem05_exit:
	TEEC_ReleaseSharedMemory(SHARE_MEM04);
mem04_exit:
exit:
	Invoke_Crypto_FreeOperation(c, s, CMD_Crypto_FreeOperation, op1);
crypto_alloc:
	return res;
}

static TEEC_Result Invoke_Crypto_EnsureIntermediateBufferSize(ADBG_Case_t *c,
							      TEEC_Session *s,
							      uint32_t cmd,
							      uint32_t oph)
{
	return Invoke_Simple_Function_v1(c, s, cmd, oph, 0);
}

static TEEC_Result Invoke_AllocateTransientObject(ADBG_Case_t *c,
						  TEEC_Session *s, uint32_t cmd,
						  uint32_t obj_type,
						  uint32_t max_key_size,
						  uint32_t oph)
{
	return Invoke_Simple_Function_v2(c, s, cmd, oph, 0, obj_type,
					 max_key_size);
}

static TEEC_Result Invoke_RestrictObjectUsage1(ADBG_Case_t *c, TEEC_Session *s,
					       uint32_t cmd,
					       uint32_t obj_handle,
					       uint32_t obj_usage)
{
	return Invoke_Simple_Function_v1(c, s, cmd, obj_handle, obj_usage);
}

static TEEC_Result Invoke_StoreAttributeBuffer(ADBG_Case_t *c,
					       TEEC_Session *sess,
					       uint32_t cmd, uint32_t buffer,
					       const void *value_bufptr,
					       size_t value_size)
{
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	TEEC_Result res = TEE_SUCCESS;
	uint32_t org = 0;
	static TEEC_SharedMemory shm = { };

	shm.size = value_size;
	shm.flags = TEEC_MEM_INPUT;
	res = TEEC_AllocateSharedMemory(sess->ctx, &shm);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;
	memcpy(shm.buffer, value_bufptr, shm.size);

	op.params[0].value.a = buffer;
	SET_SHARED_MEMORY_OPERATION_PARAMETER(1, 0, &shm, shm.size);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					 TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_NONE, TEEC_NONE);

	res = TEEC_InvokeCommand(sess, cmd, &op, &org);

	TEEC_ReleaseSharedMemory(&shm);

	return res;
}

static TEEC_Result Invoke_InitRefAttribute(ADBG_Case_t *c, TEEC_Session *sess,
					   uint32_t cmd, uint32_t attr,
					   uint32_t attr_id, uint32_t buffer)
{
	return Invoke_Simple_Function_v2(c, sess, cmd, attr, attr_id,
					 buffer, 0);
}

static TEEC_Result
Macro_StoreRefAttribute(ADBG_Case_t *c, TEEC_Session *sess, uint32_t attr,
			uint32_t buffer, uint32_t attr_id,
			const void *value_bufptr, size_t value_size)
{
	TEEC_Result res = TEE_SUCCESS;

	res = Invoke_StoreAttributeBuffer(c, sess, CMD_DS_StoreBuffer,
					  buffer, value_bufptr, value_size);
	if (!ADBG_EXPECT_TEEC_SUCCESS(c, res))
		return res;

	res = Invoke_InitRefAttribute(c, sess, CMD_DS_InitRefAttribute,
				      attr, attr_id, buffer);
	ADBG_EXPECT_TEEC_SUCCESS(c, res);
	return res;
}

#endif /* XML_CRYPTO_API_H_ */
