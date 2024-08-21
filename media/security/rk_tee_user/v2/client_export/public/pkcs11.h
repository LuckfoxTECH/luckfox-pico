/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018-2020, Linaro Limited
 */

#ifndef PKCS11_H
#define PKCS11_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * PKCS#11 Cryptoki API v2.40-errata01, See specification from:
 * http://docs.oasis-open.org/pkcs11/pkcs11-base/v2.40/errata01/os/pkcs11-base-v2.40-errata01-os-complete.html
 */
#define CK_PKCS11_VERSION_MAJOR		2
#define CK_PKCS11_VERSION_MINOR		40
#define CK_PKCS11_VERSION_PATCH		1

typedef unsigned char CK_BYTE;
typedef unsigned long CK_ULONG;
typedef long CK_LONG;

typedef CK_BYTE CK_CHAR;
typedef CK_BYTE CK_UTF8CHAR;

typedef CK_BYTE *CK_BYTE_PTR;

typedef CK_ULONG *CK_ULONG_PTR;

typedef CK_CHAR *CK_CHAR_PTR;
typedef CK_UTF8CHAR *CK_UTF8CHAR_PTR;

typedef void *CK_VOID_PTR;
typedef CK_VOID_PTR *CK_VOID_PTR_PTR;

typedef CK_BYTE CK_BBOOL;

#define CK_TRUE			1
#define CK_FALSE		0

typedef CK_ULONG CK_FLAGS;

#define CK_UNAVAILABLE_INFORMATION	(~0UL)
#define CK_EFFECTIVELY_INFINITE		0UL

typedef CK_ULONG CK_SESSION_HANDLE;
typedef CK_SESSION_HANDLE *CK_SESSION_HANDLE_PTR;

typedef CK_ULONG CK_OBJECT_HANDLE;
typedef CK_OBJECT_HANDLE *CK_OBJECT_HANDLE_PTR;

#define CK_INVALID_HANDLE	0

typedef CK_ULONG CK_SLOT_ID;
typedef CK_SLOT_ID *CK_SLOT_ID_PTR;

typedef struct CK_VERSION CK_VERSION;
typedef struct CK_VERSION *CK_VERSION_PTR;

struct CK_VERSION {
	CK_BYTE		major;
	CK_BYTE		minor;
};

typedef struct CK_DATE CK_DATE;
typedef struct CK_DATE *CK_DATE_PTR;

struct CK_DATE {
	CK_CHAR		year[4];
	CK_CHAR		month[2];
	CK_CHAR		day[2];
};

/*
 * PKCS#11 Objects attributes
 */

typedef CK_ULONG CK_ATTRIBUTE_TYPE;

typedef struct CK_ATTRIBUTE CK_ATTRIBUTE;
typedef struct CK_ATTRIBUTE *CK_ATTRIBUTE_PTR;

struct CK_ATTRIBUTE {
	CK_ATTRIBUTE_TYPE	type;
	CK_VOID_PTR		pValue;
	CK_ULONG		ulValueLen;
};

/*
 * Values for CK_ATTRIBUTE_TYPE
 *
 * This does not cover the full PKCS#11 IDs.
 */
#define CKF_ARRAY_ATTRIBUTE		(1U << 30)
#define CKA_VENDOR_DEFINED		(1U << 31)
#define CKA_CLASS			0x0000
#define CKA_TOKEN			0x0001
#define CKA_PRIVATE			0x0002
#define CKA_LABEL			0x0003
#define CKA_APPLICATION			0x0010
#define CKA_VALUE			0x0011
#define CKA_OBJECT_ID			0x0012
#define CKA_CERTIFICATE_TYPE		0x0080
#define CKA_ISSUER			0x0081
#define CKA_SERIAL_NUMBER		0x0082
#define CKA_AC_ISSUER			0x0083
#define CKA_OWNER			0x0084
#define CKA_ATTR_TYPES			0x0085
#define CKA_TRUSTED			0x0086
#define CKA_CERTIFICATE_CATEGORY	0x0087
#define CKA_JAVA_MIDP_SECURITY_DOMAIN	0x0088
#define CKA_URL				0x0089
#define CKA_HASH_OF_SUBJECT_PUBLIC_KEY	0x008a
#define CKA_HASH_OF_ISSUER_PUBLIC_KEY	0x008b
#define CKA_NAME_HASH_ALGORITHM		0x008c
#define CKA_CHECK_VALUE			0x0090
#define CKA_KEY_TYPE			0x0100
#define CKA_SUBJECT			0x0101
#define CKA_ID				0x0102
#define CKA_SENSITIVE			0x0103
#define CKA_ENCRYPT			0x0104
#define CKA_DECRYPT			0x0105
#define CKA_WRAP			0x0106
#define CKA_UNWRAP			0x0107
#define CKA_SIGN			0x0108
#define CKA_SIGN_RECOVER		0x0109
#define CKA_VERIFY			0x010a
#define CKA_VERIFY_RECOVER		0x010b
#define CKA_DERIVE			0x010c
#define CKA_START_DATE			0x0110
#define CKA_END_DATE			0x0111
#define CKA_MODULUS			0x0120
#define CKA_MODULUS_BITS		0x0121
#define CKA_PUBLIC_EXPONENT		0x0122
#define CKA_PRIVATE_EXPONENT		0x0123
#define CKA_PRIME_1			0x0124
#define CKA_PRIME_2			0x0125
#define CKA_EXPONENT_1			0x0126
#define CKA_EXPONENT_2			0x0127
#define CKA_COEFFICIENT			0x0128
#define CKA_PUBLIC_KEY_INFO		0x0129
#define CKA_PRIME			0x0130
#define CKA_SUBPRIME			0x0131
#define CKA_BASE			0x0132
#define CKA_PRIME_BITS			0x0133
#define CKA_SUBPRIME_BITS		0x0134
#define CKA_VALUE_BITS			0x0160
#define CKA_VALUE_LEN			0x0161
#define CKA_EXTRACTABLE			0x0162
#define CKA_LOCAL			0x0163
#define CKA_NEVER_EXTRACTABLE		0x0164
#define CKA_ALWAYS_SENSITIVE		0x0165
#define CKA_KEY_GEN_MECHANISM		0x0166
#define CKA_MODIFIABLE			0x0170
#define CKA_COPYABLE			0x0171
#define CKA_DESTROYABLE			0x0172
#define CKA_EC_PARAMS			0x0180
#define CKA_EC_POINT			0x0181
#define CKA_ALWAYS_AUTHENTICATE		0x0202
#define CKA_WRAP_WITH_TRUSTED		0x0210
#define CKA_WRAP_TEMPLATE		(0x0211 | CKF_ARRAY_ATTRIBUTE)
#define CKA_UNWRAP_TEMPLATE		(0x0212 | CKF_ARRAY_ATTRIBUTE)
#define CKA_DERIVE_TEMPLATE		(0x0213 | CKF_ARRAY_ATTRIBUTE)
#define CKA_OTP_FORMAT			0x0220
#define CKA_OTP_LENGTH			0x0221
#define CKA_OTP_TIME_INTERVAL		0x0222
#define CKA_OTP_USER_FRIENDLY_MODE	0x0223
#define CKA_OTP_CHALLENGE_REQUIREMENT	0x0224
#define CKA_OTP_TIME_REQUIREMENT	0x0225
#define CKA_OTP_COUNTER_REQUIREMENT	0x0226
#define CKA_OTP_PIN_REQUIREMENT		0x0227
#define CKA_OTP_COUNTER			0x022e
#define CKA_OTP_TIME			0x022f
#define CKA_OTP_USER_IDENTIFIER		0x022a
#define CKA_OTP_SERVICE_IDENTIFIER	0x022b
#define CKA_OTP_SERVICE_LOGO		0x022c
#define CKA_OTP_SERVICE_LOGO_TYPE	0x022d
#define CKA_GOSTR3410_PARAMS		0x0250
#define CKA_GOSTR3411_PARAMS		0x0251
#define CKA_GOST28147_PARAMS		0x0252
#define CKA_HW_FEATURE_TYPE		0x0300
#define CKA_RESET_ON_INIT		0x0301
#define CKA_HAS_RESET			0x0302
#define CKA_PIXEL_X			0x0400
#define CKA_PIXEL_Y			0x0401
#define CKA_RESOLUTION			0x0402
#define CKA_CHAR_ROWS			0x0403
#define CKA_CHAR_COLUMNS		0x0404
#define CKA_COLOR			0x0405
#define CKA_BITS_PER_PIXEL		0x0406
#define CKA_CHAR_SETS			0x0480
#define CKA_ENCODING_METHODS		0x0481
#define CKA_MIME_TYPES			0x0482
#define CKA_MECHANISM_TYPE		0x0500
#define CKA_REQUIRED_CMS_ATTRIBUTES	0x0501
#define CKA_DEFAULT_CMS_ATTRIBUTES	0x0502
#define CKA_SUPPORTED_CMS_ATTRIBUTES	0x0503
#define CKA_ALLOWED_MECHANISMS		(0x0600 | CKF_ARRAY_ATTRIBUTE)

/* Attribute CKA_CLASS refers to a CK_OBJECT_CLASS typed value */
typedef CK_ULONG CK_OBJECT_CLASS;
typedef CK_OBJECT_CLASS *CK_OBJECT_CLASS_PTR;

/* Values for type CK_OBJECT_CLASS */
#define CKO_VENDOR_DEFINED		(1U << 31)
#define CKO_DATA			0x0
#define CKO_CERTIFICATE			0x1
#define CKO_PUBLIC_KEY			0x2
#define CKO_PRIVATE_KEY			0x3
#define CKO_SECRET_KEY			0x4
#define CKO_HW_FEATURE			0x5
#define CKO_DOMAIN_PARAMETERS		0x6
#define CKO_MECHANISM			0x7
#define CKO_OTP_KEY			0x8

/* Attribute CKA_KEY_TYPE refers to a CK_KEY_TYPE typed value */
typedef CK_ULONG CK_KEY_TYPE;
typedef CK_KEY_TYPE *CK_KEY_TYPE_PTR;

/*
 * Values for type CK_KEY_TYPE
 *
 * This does not cover the full PKCS#11 IDs.
 */
#define CKK_VENDOR_DEFINED		(1U << 31)
#define CKK_RSA				0x000
#define CKK_DSA				0x001
#define CKK_DH				0x002
#define CKK_ECDSA			0x003
#define CKK_EC				0x003
#define CKK_GENERIC_SECRET		0x010
#define CKK_DES3			0x015
#define CKK_AES				0x01f
#define CKK_HOTP			0x023
#define CKK_MD5_HMAC			0x027
#define CKK_SHA_1_HMAC			0x028
#define CKK_SHA256_HMAC			0x02b
#define CKK_SHA384_HMAC			0x02c
#define CKK_SHA512_HMAC			0x02d
#define CKK_SHA224_HMAC			0x02e

/*
 * Mechanisms
 *
 * Note: a mechanism can be referenced as object reference in some PKCS#11 API
 * functions. In such case, the object hold attribute CKA_MECHANISM_TYPE which
 * refers to a CK_MECHANISM_TYPE typed value that defines the target mechanism.
 */

typedef CK_ULONG CK_MECHANISM_TYPE;
typedef CK_MECHANISM_TYPE *CK_MECHANISM_TYPE_PTR;

/*
 * Values for type CK_MECHANISM_TYPE
 *
 * This does not cover the full PKCS#11 IDs.
 */
#define CKM_VENDOR_DEFINED		(1U << 31)
#define CKM_RSA_PKCS_KEY_PAIR_GEN	0x00000
#define CKM_RSA_PKCS			0x00001
#define CKM_RSA_9796			0x00002
#define CKM_RSA_X_509			0x00003
#define CKM_RSA_PKCS_OAEP		0x00009
#define CKM_SHA256_RSA_PKCS		0x00040
#define CKM_SHA384_RSA_PKCS		0x00041
#define CKM_SHA512_RSA_PKCS		0x00042
#define CKM_SHA256_RSA_PKCS_PSS		0x00043
#define CKM_SHA384_RSA_PKCS_PSS		0x00044
#define CKM_SHA512_RSA_PKCS_PSS		0x00045
#define CKM_SHA224_RSA_PKCS		0x00046
#define CKM_SHA224_RSA_PKCS_PSS		0x00047
#define CKM_SHA512_224			0x00048
#define CKM_SHA512_224_HMAC		0x00049
#define CKM_SHA512_224_HMAC_GENERAL	0x0004a
#define CKM_SHA512_224_KEY_DERIVATION	0x0004b
#define CKM_SHA512_256			0x0004c
#define CKM_SHA512_256_HMAC		0x0004d
#define CKM_SHA512_256_HMAC_GENERAL	0x0004e
#define CKM_SHA512_256_KEY_DERIVATION	0x0004f
#define CKM_DES3_ECB			0x00132
#define CKM_DES3_CBC			0x00133
#define CKM_DES3_MAC			0x00134
#define CKM_DES3_MAC_GENERAL		0x00135
#define CKM_DES3_CBC_PAD		0x00136
#define CKM_DES3_CMAC_GENERAL		0x00137
#define CKM_DES3_CMAC			0x00138
#define CKM_MD5				0x00210
#define CKM_MD5_HMAC			0x00211
#define CKM_MD5_HMAC_GENERAL		0x00212
#define CKM_SHA_1			0x00220
#define CKM_SHA_1_HMAC			0x00221
#define CKM_SHA_1_HMAC_GENERAL		0x00222
#define CKM_SHA256			0x00250
#define CKM_SHA256_HMAC			0x00251
#define CKM_SHA256_HMAC_GENERAL		0x00252
#define CKM_SHA224			0x00255
#define CKM_SHA224_HMAC			0x00256
#define CKM_SHA224_HMAC_GENERAL		0x00257
#define CKM_SHA384			0x00260
#define CKM_SHA384_HMAC			0x00261
#define CKM_SHA384_HMAC_GENERAL		0x00262
#define CKM_SHA512			0x00270
#define CKM_SHA512_HMAC			0x00271
#define CKM_SHA512_HMAC_GENERAL		0x00272
#define CKM_HOTP_KEY_GEN		0x00290
#define CKM_HOTP			0x00291
#define CKM_GENERIC_SECRET_KEY_GEN	0x00350
#define CKM_MD5_KEY_DERIVATION		0x00390
#define CKM_MD2_KEY_DERIVATION		0x00391
#define CKM_SHA1_KEY_DERIVATION		0x00392
#define CKM_SHA256_KEY_DERIVATION	0x00393
#define CKM_SHA384_KEY_DERIVATION	0x00394
#define CKM_SHA512_KEY_DERIVATION	0x00395
#define CKM_SHA224_KEY_DERIVATION	0x00396
#define CKM_EC_KEY_PAIR_GEN		0x01040
#define CKM_ECDSA			0x01041
#define CKM_ECDSA_SHA1			0x01042
#define CKM_ECDSA_SHA224		0x01043
#define CKM_ECDSA_SHA256		0x01044
#define CKM_ECDSA_SHA384		0x01045
#define CKM_ECDSA_SHA512		0x01046
#define CKM_ECDH1_DERIVE		0x01050
#define CKM_ECDH1_COFACTOR_DERIVE	0x01051
#define CKM_ECMQV_DERIVE		0x01052
#define CKM_ECDH_AES_KEY_WRAP		0x01053
#define CKM_RSA_AES_KEY_WRAP		0x01054
#define CKM_AES_KEY_GEN			0x01080
#define CKM_AES_ECB			0x01081
#define CKM_AES_CBC			0x01082
#define CKM_AES_MAC			0x01083
#define CKM_AES_MAC_GENERAL		0x01084
#define CKM_AES_CBC_PAD			0x01085
#define CKM_AES_CTR			0x01086
#define CKM_AES_GCM			0x01087
#define CKM_AES_CCM			0x01088
#define CKM_AES_CTS			0x01089
#define CKM_AES_CMAC			0x0108a
#define CKM_AES_CMAC_GENERAL		0x0108b
#define CKM_AES_XCBC_MAC		0x0108c
#define CKM_AES_XCBC_MAC_96		0x0108d
#define CKM_AES_GMAC			0x0108e
#define CKM_DES3_ECB_ENCRYPT_DATA	0x01102
#define CKM_DES3_CBC_ENCRYPT_DATA	0x01103
#define CKM_AES_ECB_ENCRYPT_DATA	0x01104
#define CKM_AES_CBC_ENCRYPT_DATA	0x01105
#define CKM_AES_KEY_WRAP		0x02109
#define CKM_AES_KEY_WRAP_PAD		0x0210a

typedef struct CK_MECHANISM_INFO CK_MECHANISM_INFO;
typedef struct CK_MECHANISM_INFO *CK_MECHANISM_INFO_PTR;

struct CK_MECHANISM_INFO {
	CK_ULONG		ulMinKeySize;
	CK_ULONG		ulMaxKeySize;
	CK_FLAGS		flags;
};

/* Flags for field flags of struct ck_mechanism_info */
#define CKF_HW				(1U << 0)
#define CKF_ENCRYPT			(1U << 8)
#define CKF_DECRYPT			(1U << 9)
#define CKF_DIGEST			(1U << 10)
#define CKF_SIGN			(1U << 11)
#define CKF_SIGN_RECOVER		(1U << 12)
#define CKF_VERIFY			(1U << 13)
#define CKF_VERIFY_RECOVER		(1U << 14)
#define CKF_GENERATE			(1U << 15)
#define CKF_GENERATE_KEY_PAIR		(1U << 16)
#define CKF_WRAP			(1U << 17)
#define CKF_UNWRAP			(1U << 18)
#define CKF_DERIVE			(1U << 19)
#define CKF_EC_F_P			(1U << 20)
#define CKF_EC_F_2M			(1U << 21)
#define CKF_EC_ECPARAMETERS		(1U << 22)
#define CKF_EC_NAMEDCURVE		(1U << 23)
#define CKF_EC_UNCOMPRESS		(1U << 24)
#define CKF_EC_COMPRESS			(1U << 25)
#define CKF_EXTENSION			(1U << 31)

/*
 * Mechanism parameter structures
 *
 * This does not cover the whole mechanism parameter structures defined by
 * the PKCS#11. To be updated when needed.
 */

typedef struct CK_MECHANISM CK_MECHANISM;
typedef struct CK_MECHANISM *CK_MECHANISM_PTR;

struct CK_MECHANISM {
	CK_MECHANISM_TYPE	mechanism;
	CK_VOID_PTR		pParameter;
	CK_ULONG		ulParameterLen;
};

/* MAC General parameters */
typedef CK_ULONG CK_MAC_GENERAL_PARAMS;
typedef CK_MAC_GENERAL_PARAMS *CK_MAC_GENERAL_PARAMS_PTR;

/* AES CBC encryption parameters */
typedef struct CK_AES_CBC_ENCRYPT_DATA_PARAMS CK_AES_CBC_ENCRYPT_DATA_PARAMS;
typedef struct CK_AES_CBC_ENCRYPT_DATA_PARAMS
					*CK_AES_CBC_ENCRYPT_DATA_PARAMS_PTR;

struct CK_AES_CBC_ENCRYPT_DATA_PARAMS {
	CK_BYTE			iv[16];
	CK_BYTE_PTR		pData;
	CK_ULONG		length;
};

/* AES CTR parameters */
typedef struct CK_AES_CTR_PARAMS CK_AES_CTR_PARAMS;
typedef struct CK_AES_CTR_PARAMS *CK_AES_CTR_PARAMS_PTR;

struct CK_AES_CTR_PARAMS {
	CK_ULONG		ulCounterBits;
	CK_BYTE			cb[16];
};

/* AES GCM parameters */
typedef struct CK_GCM_PARAMS CK_GCM_PARAMS;
typedef struct CK_GCM_PARAMS *CK_GCM_PARAMS_PTR;

struct CK_GCM_PARAMS {
	CK_BYTE_PTR		pIv;
	CK_ULONG		ulIvLen;
	CK_ULONG		ulIvBits;
	CK_BYTE_PTR		pAAD;
	CK_ULONG		ulAADLen;
	CK_ULONG		ulTagBits;
};

/* AES CCM parameters */
typedef struct CK_CCM_PARAMS CK_CCM_PARAMS;
typedef struct CK_CCM_PARAMS *CK_CCM_PARAMS_PTR;

struct CK_CCM_PARAMS {
	CK_ULONG		ulDataLen;
	CK_BYTE_PTR		pNonce;
	CK_ULONG		ulNonceLen;
	CK_BYTE_PTR		pAAD;
	CK_ULONG		ulAADLen;
	CK_ULONG		ulMACLen;
};

typedef struct CK_KEY_DERIVATION_STRING_DATA CK_KEY_DERIVATION_STRING_DATA;
typedef struct CK_KEY_DERIVATION_STRING_DATA
					*CK_KEY_DERIVATION_STRING_DATA_PTR;

struct CK_KEY_DERIVATION_STRING_DATA {
	CK_BYTE_PTR pData;
	CK_ULONG    ulLen;
};

/*
 * PKCS#11 return values
 */
typedef CK_ULONG CK_RV;

/* Values for type CK_RV */
#define CKR_VENDOR_DEFINED			(1U << 31)
#define CKR_OK					0x0000
#define CKR_CANCEL				0x0001
#define CKR_HOST_MEMORY				0x0002
#define CKR_SLOT_ID_INVALID			0x0003
#define CKR_GENERAL_ERROR			0x0005
#define CKR_FUNCTION_FAILED			0x0006
#define CKR_ARGUMENTS_BAD			0x0007
#define CKR_NO_EVENT				0x0008
#define CKR_NEED_TO_CREATE_THREADS		0x0009
#define CKR_CANT_LOCK				0x000a
#define CKR_ATTRIBUTE_READ_ONLY			0x0010
#define CKR_ATTRIBUTE_SENSITIVE			0x0011
#define CKR_ATTRIBUTE_TYPE_INVALID		0x0012
#define CKR_ATTRIBUTE_VALUE_INVALID		0x0013
#define CKR_ACTION_PROHIBITED			0x001b
#define CKR_DATA_INVALID			0x0020
#define CKR_DATA_LEN_RANGE			0x0021
#define CKR_DEVICE_ERROR			0x0030
#define CKR_DEVICE_MEMORY			0x0031
#define CKR_DEVICE_REMOVED			0x0032
#define CKR_ENCRYPTED_DATA_INVALID		0x0040
#define CKR_ENCRYPTED_DATA_LEN_RANGE		0x0041
#define CKR_FUNCTION_CANCELED			0x0050
#define CKR_FUNCTION_NOT_PARALLEL		0x0051
#define CKR_FUNCTION_NOT_SUPPORTED		0x0054
#define CKR_KEY_HANDLE_INVALID			0x0060
#define CKR_KEY_SIZE_RANGE			0x0062
#define CKR_KEY_TYPE_INCONSISTENT		0x0063
#define CKR_KEY_NOT_NEEDED			0x0064
#define CKR_KEY_CHANGED				0x0065
#define CKR_KEY_NEEDED				0x0066
#define CKR_KEY_INDIGESTIBLE			0x0067
#define CKR_KEY_FUNCTION_NOT_PERMITTED		0x0068
#define CKR_KEY_NOT_WRAPPABLE			0x0069
#define CKR_KEY_UNEXTRACTABLE			0x006a
#define CKR_MECHANISM_INVALID			0x0070
#define CKR_MECHANISM_PARAM_INVALID		0x0071
#define CKR_OBJECT_HANDLE_INVALID		0x0082
#define CKR_OPERATION_ACTIVE			0x0090
#define CKR_OPERATION_NOT_INITIALIZED		0x0091
#define CKR_PIN_INCORRECT			0x00a0
#define CKR_PIN_INVALID				0x00a1
#define CKR_PIN_LEN_RANGE			0x00a2
#define CKR_PIN_EXPIRED				0x00a3
#define CKR_PIN_LOCKED				0x00a4
#define CKR_SESSION_CLOSED			0x00b0
#define CKR_SESSION_COUNT			0x00b1
#define CKR_SESSION_HANDLE_INVALID		0x00b3
#define CKR_SESSION_PARALLEL_NOT_SUPPORTED	0x00b4
#define CKR_SESSION_READ_ONLY			0x00b5
#define CKR_SESSION_EXISTS			0x00b6
#define CKR_SESSION_READ_ONLY_EXISTS		0x00b7
#define CKR_SESSION_READ_WRITE_SO_EXISTS	0x00b8
#define CKR_SIGNATURE_INVALID			0x00c0
#define CKR_SIGNATURE_LEN_RANGE			0x00c1
#define CKR_TEMPLATE_INCOMPLETE			0x00d0
#define CKR_TEMPLATE_INCONSISTENT		0x00d1
#define CKR_TOKEN_NOT_PRESENT			0x00e0
#define CKR_TOKEN_NOT_RECOGNIZED		0x00e1
#define CKR_TOKEN_WRITE_PROTECTED		0x00e2
#define CKR_UNWRAPPING_KEY_HANDLE_INVALID	0x00f0
#define CKR_UNWRAPPING_KEY_SIZE_RANGE		0x00f1
#define CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT	0x00f2
#define CKR_USER_ALREADY_LOGGED_IN		0x0100
#define CKR_USER_NOT_LOGGED_IN			0x0101
#define CKR_USER_PIN_NOT_INITIALIZED		0x0102
#define CKR_USER_TYPE_INVALID			0x0103
#define CKR_USER_ANOTHER_ALREADY_LOGGED_IN	0x0104
#define CKR_USER_TOO_MANY_TYPES			0x0105
#define CKR_WRAPPED_KEY_INVALID			0x0110
#define CKR_WRAPPED_KEY_LEN_RANGE		0x0112
#define CKR_WRAPPING_KEY_HANDLE_INVALID		0x0113
#define CKR_WRAPPING_KEY_SIZE_RANGE		0x0114
#define CKR_WRAPPING_KEY_TYPE_INCONSISTENT	0x0115
#define CKR_RANDOM_SEED_NOT_SUPPORTED		0x0120
#define CKR_RANDOM_NO_RNG			0x0121
#define CKR_DOMAIN_PARAMS_INVALID		0x0130
#define CKR_CURVE_NOT_SUPPORTED			0x0140
#define CKR_BUFFER_TOO_SMALL			0x0150
#define CKR_SAVED_STATE_INVALID			0x0160
#define CKR_INFORMATION_SENSITIVE		0x0170
#define CKR_STATE_UNSAVEABLE			0x0180
#define CKR_CRYPTOKI_NOT_INITIALIZED		0x0190
#define CKR_CRYPTOKI_ALREADY_INITIALIZED	0x0191
#define CKR_MUTEX_BAD				0x01a0
#define CKR_MUTEX_NOT_LOCKED			0x01a1
#define CKR_NEW_PIN_MODE			0x01b0
#define CKR_NEXT_OTP				0x01b1
#define CKR_EXCEEDED_MAX_ITERATIONS		0x01b5
#define CKR_FIPS_SELF_TEST_FAILED		0x01b6
#define CKR_LIBRARY_LOAD_FAILED			0x01b7
#define CKR_PIN_TOO_WEAK			0x01b8
#define CKR_PUBLIC_KEY_INVALID			0x01b9
#define CKR_FUNCTION_REJECTED			0x0200

/*
 * PKCS#11 API functions
 */

/* Argument for C_GetInfo */
typedef struct CK_INFO CK_INFO;
typedef struct CK_INFO *CK_INFO_PTR;

struct CK_INFO {
	CK_VERSION	cryptokiVersion;
	CK_UTF8CHAR	manufacturerID[32];
	CK_FLAGS	flags;
	CK_UTF8CHAR	libraryDescription[32];
	CK_VERSION	libraryVersion;
};

/* Argument for C_GetSlotInfo */
typedef struct CK_SLOT_INFO CK_SLOT_INFO;
typedef struct CK_SLOT_INFO *CK_SLOT_INFO_PTR;

struct CK_SLOT_INFO {
	CK_UTF8CHAR	slotDescription[64];
	CK_UTF8CHAR	manufacturerID[32];
	CK_FLAGS	flags;
	CK_VERSION	hardwareVersion;
	CK_VERSION	firmwareVersion;
};

/* Values for field flags of struct ck_slot_info */
#define CKF_TOKEN_PRESENT	(1U << 0)
#define CKF_REMOVABLE_DEVICE	(1U << 1)
#define CKF_HW_SLOT		(1U << 2)

/* Argument for C_GetTokenInfo */
typedef struct CK_TOKEN_INFO CK_TOKEN_INFO;
typedef struct CK_TOKEN_INFO *CK_TOKEN_INFO_PTR;

struct CK_TOKEN_INFO {
	CK_UTF8CHAR	label[32];
	CK_UTF8CHAR	manufacturerID[32];
	CK_UTF8CHAR	model[16];
	CK_CHAR		serialNumber[16];
	CK_FLAGS	flags;
	CK_ULONG	ulMaxSessionCount;
	CK_ULONG	ulSessionCount;
	CK_ULONG	ulMaxRwSessionCount;
	CK_ULONG	ulRwSessionCount;
	CK_ULONG	ulMaxPinLen;
	CK_ULONG	ulMinPinLen;
	CK_ULONG	ulTotalPublicMemory;
	CK_ULONG	ulFreePublicMemory;
	CK_ULONG	ulTotalPrivateMemory;
	CK_ULONG	ulFreePrivateMemory;
	CK_VERSION	hardwareVersion;
	CK_VERSION	firmwareVersion;
	CK_CHAR		utcTime[16];
};

/* Values for field flags of struct ck_token_info */
#define CKF_RNG					(1U << 0)
#define CKF_WRITE_PROTECTED			(1U << 1)
#define CKF_LOGIN_REQUIRED			(1U << 2)
#define CKF_USER_PIN_INITIALIZED		(1U << 3)
#define CKF_RESTORE_KEY_NOT_NEEDED		(1U << 5)
#define CKF_CLOCK_ON_TOKEN			(1U << 6)
#define CKF_PROTECTED_AUTHENTICATION_PATH	(1U << 8)
#define CKF_DUAL_CRYPTO_OPERATIONS		(1U << 9)
#define CKF_TOKEN_INITIALIZED			(1U << 10)
#define CKF_SECONDARY_AUTHENTICATION		(1U << 11)
#define CKF_USER_PIN_COUNT_LOW			(1U << 16)
#define CKF_USER_PIN_FINAL_TRY			(1U << 17)
#define CKF_USER_PIN_LOCKED			(1U << 18)
#define CKF_USER_PIN_TO_BE_CHANGED		(1U << 19)
#define CKF_SO_PIN_COUNT_LOW			(1U << 20)
#define CKF_SO_PIN_FINAL_TRY			(1U << 21)
#define CKF_SO_PIN_LOCKED			(1U << 22)
#define CKF_SO_PIN_TO_BE_CHANGED		(1U << 23)
#define CKF_ERROR_STATE				(1U << 24)

/* Argument for C_GetSessionInfo */
typedef struct CK_SESSION_INFO CK_SESSION_INFO;
typedef struct CK_SESSION_INFO *CK_SESSION_INFO_PTR;

typedef CK_ULONG CK_STATE;

/* Values for CK_STATE */
#define CKS_RO_PUBLIC_SESSION		0
#define CKS_RO_USER_FUNCTIONS		1
#define CKS_RW_PUBLIC_SESSION		2
#define CKS_RW_USER_FUNCTIONS		3
#define CKS_RW_SO_FUNCTIONS		4

struct CK_SESSION_INFO {
	CK_SLOT_ID	slotID;
	CK_STATE	state;
	CK_FLAGS	flags;
	CK_ULONG	ulDeviceError;
};

/* Values for field flags of struct ck_session_info */
#define CKF_RW_SESSION			(1U << 1)
#define CKF_SERIAL_SESSION		(1U << 2)

/* Argument for C_Login */
typedef CK_ULONG CK_USER_TYPE;

/* Values for CK_USER_TYPE */
#define CKU_SO				0
#define CKU_USER			1
#define CKU_CONTEXT_SPECIFIC		2

/* Values for argument flags of C_WaitForSlotEvent */
#define CKF_DONT_BLOCK			1

/* Argument for CK_NOTIFY typed callback function */
typedef CK_ULONG CK_NOTIFICATION;

/* Values for CK_NOTIFICATION */
#define CKN_SURRENDER			0
#define CKN_OTP_CHANGED			1

/* Callback handler types */
typedef CK_RV (*CK_NOTIFY) (CK_SESSION_HANDLE hSession, CK_NOTIFICATION event,
			    CK_VOID_PTR pApplication);
typedef CK_RV (*CK_CREATEMUTEX) (CK_VOID_PTR_PTR ppMutex);
typedef CK_RV (*CK_DESTROYMUTEX) (CK_VOID_PTR pMutex);
typedef CK_RV (*CK_LOCKMUTEX) (CK_VOID_PTR pMutex);
typedef CK_RV (*CK_UNLOCKMUTEX) (CK_VOID_PTR pMutex);

/* Argument for C_GetFunctionList */
typedef struct CK_FUNCTION_LIST CK_FUNCTION_LIST;
typedef struct CK_FUNCTION_LIST *CK_FUNCTION_LIST_PTR;
typedef struct CK_FUNCTION_LIST **CK_FUNCTION_LIST_PTR_PTR;

struct CK_FUNCTION_LIST {
	CK_VERSION version;
	CK_RV (*C_Initialize)(CK_VOID_PTR pInitArgs);
	CK_RV (*C_Finalize)(CK_VOID_PTR pReserved);
	CK_RV (*C_GetInfo)(CK_INFO_PTR pInfo);
	CK_RV (*C_GetFunctionList)(CK_FUNCTION_LIST_PTR_PTR ppFunctionList);
	CK_RV (*C_GetSlotList)(CK_BBOOL tokenPresent,
			       CK_SLOT_ID_PTR pSlotList, CK_ULONG_PTR pulCount);
	CK_RV (*C_GetSlotInfo)(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo);
	CK_RV (*C_GetTokenInfo)(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo);
	CK_RV (*C_GetMechanismList)(CK_SLOT_ID slotID,
				    CK_MECHANISM_TYPE_PTR pMechanismList,
				    CK_ULONG_PTR pulCount);
	CK_RV (*C_GetMechanismInfo)(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type,
				    CK_MECHANISM_INFO_PTR pInfo);
	CK_RV (*C_InitToken)(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin,
			     CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel);
	CK_RV (*C_InitPIN)(CK_SESSION_HANDLE hSession,
			   CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen);
	CK_RV (*C_SetPIN)(CK_SESSION_HANDLE hSession,
			  CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldLen,
			  CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen);
	CK_RV (*C_OpenSession)(CK_SLOT_ID slotID, CK_FLAGS flags,
			       CK_VOID_PTR pApplication, CK_NOTIFY Notify,
			       CK_SESSION_HANDLE_PTR phSession);
	CK_RV (*C_CloseSession)(CK_SESSION_HANDLE hSession);
	CK_RV (*C_CloseAllSessions)(CK_SLOT_ID slotID);
	CK_RV (*C_GetSessionInfo)(CK_SESSION_HANDLE hSession,
				  CK_SESSION_INFO_PTR pInfo);
	CK_RV (*C_GetOperationState)(CK_SESSION_HANDLE hSession,
				     CK_BYTE_PTR pOperationState,
				     CK_ULONG_PTR pulOperationStateLen);
	CK_RV (*C_SetOperationState)(CK_SESSION_HANDLE hSession,
				     CK_BYTE_PTR pOperationState,
				     CK_ULONG ulOperationStateLen,
				     CK_OBJECT_HANDLE hEncryptionKey,
				     CK_OBJECT_HANDLE hAuthenticationKey);
	CK_RV (*C_Login)(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType,
			 CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen);
	CK_RV (*C_Logout)(CK_SESSION_HANDLE hSession);
	CK_RV (*C_CreateObject)(CK_SESSION_HANDLE hSession,
				CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
				CK_OBJECT_HANDLE_PTR phObject);
	CK_RV (*C_CopyObject)(CK_SESSION_HANDLE hSession,
			      CK_OBJECT_HANDLE hObject,
			      CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
			      CK_OBJECT_HANDLE_PTR phNewObject);
	CK_RV (*C_DestroyObject)(CK_SESSION_HANDLE hSession,
				 CK_OBJECT_HANDLE hObject);
	CK_RV (*C_GetObjectSize)(CK_SESSION_HANDLE hSession,
				 CK_OBJECT_HANDLE hObject,
				 CK_ULONG_PTR pulSize);
	CK_RV (*C_GetAttributeValue)(CK_SESSION_HANDLE hSession,
				     CK_OBJECT_HANDLE hObject,
				     CK_ATTRIBUTE_PTR pTemplate,
				     CK_ULONG ulCount);
	CK_RV (*C_SetAttributeValue)(CK_SESSION_HANDLE hSession,
				     CK_OBJECT_HANDLE hObject,
				     CK_ATTRIBUTE_PTR pTemplate,
				     CK_ULONG ulCount);
	CK_RV (*C_FindObjectsInit)(CK_SESSION_HANDLE hSession,
				   CK_ATTRIBUTE_PTR pTemplate,
				   CK_ULONG ulCount);
	CK_RV (*C_FindObjects)(CK_SESSION_HANDLE hSession,
			       CK_OBJECT_HANDLE_PTR phObject,
			       CK_ULONG ulMaxObjectCount,
			       CK_ULONG_PTR pulObjectCount);
	CK_RV (*C_FindObjectsFinal)(CK_SESSION_HANDLE hSession);
	CK_RV (*C_EncryptInit)(CK_SESSION_HANDLE hSession,
			       CK_MECHANISM_PTR pMechanism,
			       CK_OBJECT_HANDLE hKey);
	CK_RV (*C_Encrypt)(CK_SESSION_HANDLE hSession,
			   CK_BYTE_PTR pData, CK_ULONG ulDataLen,
			   CK_BYTE_PTR pEncryptedData,
			   CK_ULONG_PTR pulEncryptedDataLen);
	CK_RV (*C_EncryptUpdate)(CK_SESSION_HANDLE hSession,
				 CK_BYTE_PTR pPart, CK_ULONG ulPartLen,
				 CK_BYTE_PTR pEncryptedData,
				 CK_ULONG_PTR pulEncryptedDataLen);
	CK_RV (*C_EncryptFinal)(CK_SESSION_HANDLE hSession,
				CK_BYTE_PTR pLastEncryptedPart,
				CK_ULONG_PTR pulLastEncryptedPartLen);
	CK_RV (*C_DecryptInit)(CK_SESSION_HANDLE hSession,
			       CK_MECHANISM_PTR pMechanism,
			       CK_OBJECT_HANDLE hKey);
	CK_RV (*C_Decrypt)(CK_SESSION_HANDLE hSession,
			   CK_BYTE_PTR pEncryptedData,
			   CK_ULONG ulEncryptedDataLen,
			   CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);
	CK_RV (*C_DecryptUpdate)(CK_SESSION_HANDLE hSession,
				 CK_BYTE_PTR pEncryptedPart,
				 CK_ULONG ulEncryptedPartLen,
				 CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);
	CK_RV (*C_DecryptFinal)(CK_SESSION_HANDLE hSession,
				CK_BYTE_PTR pLastPart,
				CK_ULONG_PTR pulLastPartLen);
	CK_RV (*C_DigestInit)(CK_SESSION_HANDLE hSession,
			      CK_MECHANISM_PTR pMechanism);
	CK_RV (*C_Digest)(CK_SESSION_HANDLE hSession,
			  CK_BYTE_PTR pData, CK_ULONG ulDataLen,
			  CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);
	CK_RV (*C_DigestUpdate)(CK_SESSION_HANDLE hSession,
				CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
	CK_RV (*C_DigestKey)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey);
	CK_RV (*C_DigestFinal)(CK_SESSION_HANDLE hSession,
			       CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);
	CK_RV (*C_SignInit)(CK_SESSION_HANDLE hSession,
			    CK_MECHANISM_PTR pMechanism,
			    CK_OBJECT_HANDLE hKey);
	CK_RV (*C_Sign)(CK_SESSION_HANDLE hSession,
			CK_BYTE_PTR pData, CK_ULONG ulDataLen,
			CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);
	CK_RV (*C_SignUpdate)(CK_SESSION_HANDLE hSession,
			      CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
	CK_RV (*C_SignFinal)(CK_SESSION_HANDLE hSession,
			     CK_BYTE_PTR pSignature,
			     CK_ULONG_PTR pulSignatureLen);
	CK_RV (*C_SignRecoverInit)(CK_SESSION_HANDLE hSession,
				   CK_MECHANISM_PTR pMechanism,
				   CK_OBJECT_HANDLE hKey);
	CK_RV (*C_SignRecover)(CK_SESSION_HANDLE hSession,
			       CK_BYTE_PTR pData, CK_ULONG ulDataLen,
			       CK_BYTE_PTR pSignature,
			       CK_ULONG_PTR pulSignatureLen);
	CK_RV (*C_VerifyInit)(CK_SESSION_HANDLE hSession,
			      CK_MECHANISM_PTR pMechanism,
			      CK_OBJECT_HANDLE hKey);
	CK_RV (*C_Verify)(CK_SESSION_HANDLE hSession,
			  CK_BYTE_PTR pData, CK_ULONG ulDataLen,
			  CK_BYTE_PTR pSignature,
			  CK_ULONG ulSignatureLen);
	CK_RV (*C_VerifyUpdate)(CK_SESSION_HANDLE hSession,
				CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
	CK_RV (*C_VerifyFinal)(CK_SESSION_HANDLE hSession,
			       CK_BYTE_PTR pSignature,
			       CK_ULONG ulSignatureLen);
	CK_RV (*C_VerifyRecoverInit)(CK_SESSION_HANDLE hSession,
				     CK_MECHANISM_PTR pMechanism,
				     CK_OBJECT_HANDLE hKey);
	CK_RV (*C_VerifyRecover)(CK_SESSION_HANDLE hSession,
				 CK_BYTE_PTR pSignature,
				 CK_ULONG ulSignatureLen,
				 CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);
	CK_RV (*C_DigestEncryptUpdate)(CK_SESSION_HANDLE hSession,
				       CK_BYTE_PTR pPart, CK_ULONG ulPartLen,
				       CK_BYTE_PTR pEncryptedPart,
				       CK_ULONG_PTR pulEncryptedPartLen);
	CK_RV (*C_DecryptDigestUpdate)(CK_SESSION_HANDLE hSession,
				       CK_BYTE_PTR pEncryptedPart,
				       CK_ULONG ulEncryptedPartLen,
				       CK_BYTE_PTR pPart,
				       CK_ULONG_PTR pulPartLen);
	CK_RV (*C_SignEncryptUpdate)(CK_SESSION_HANDLE hSession,
				     CK_BYTE_PTR pPart, CK_ULONG ulPartLen,
				     CK_BYTE_PTR pEncryptedPart,
				     CK_ULONG_PTR pulEncryptedPartLen);
	CK_RV (*C_DecryptVerifyUpdate)(CK_SESSION_HANDLE hSession,
				       CK_BYTE_PTR pEncryptedPart,
				       CK_ULONG ulEncryptedPartLen,
				       CK_BYTE_PTR pPart,
				       CK_ULONG_PTR pulPartLen);
	CK_RV (*C_GenerateKey)(CK_SESSION_HANDLE hSession,
			       CK_MECHANISM_PTR pMechanism,
			       CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
			       CK_OBJECT_HANDLE_PTR phKey);
	CK_RV (*C_GenerateKeyPair)(CK_SESSION_HANDLE hSession,
				   CK_MECHANISM_PTR pMechanism,
				   CK_ATTRIBUTE_PTR pPublicKeyTemplate,
				   CK_ULONG ulPublicKeyAttributeCount,
				   CK_ATTRIBUTE_PTR pPrivateKeyTemplate,
				   CK_ULONG ulPrivateKeyAttributeCount,
				   CK_OBJECT_HANDLE_PTR phPublicKey,
				   CK_OBJECT_HANDLE_PTR phPrivateKey);
	CK_RV (*C_WrapKey)(CK_SESSION_HANDLE hSession,
			   CK_MECHANISM_PTR pMechanism,
			   CK_OBJECT_HANDLE hWrappingKey,
			   CK_OBJECT_HANDLE hKey,
			   CK_BYTE_PTR pWrappedKey,
			   CK_ULONG_PTR pulWrappedKeyLen);
	CK_RV (*C_UnwrapKey)(CK_SESSION_HANDLE hSession,
			     CK_MECHANISM_PTR pMechanism,
			     CK_OBJECT_HANDLE hUnwrappingKey,
			     CK_BYTE_PTR pWrappedKey,
			     CK_ULONG ulWrappedKeyLen,
			     CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
			     CK_OBJECT_HANDLE_PTR phKey);
	CK_RV (*C_DeriveKey)(CK_SESSION_HANDLE hSession,
			     CK_MECHANISM_PTR pMechanism,
			     CK_OBJECT_HANDLE hBaseKey,
			     CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
			     CK_OBJECT_HANDLE_PTR phKey);
	CK_RV (*C_SeedRandom)(CK_SESSION_HANDLE hSession,
			      CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen);
	CK_RV (*C_GenerateRandom)(CK_SESSION_HANDLE hSession,
				  CK_BYTE_PTR pRandomData,
				  CK_ULONG ulRandomLen);
	CK_RV (*C_GetFunctionStatus)(CK_SESSION_HANDLE hSession);
	CK_RV (*C_CancelFunction)(CK_SESSION_HANDLE hSession);
	CK_RV (*C_WaitForSlotEvent)(CK_FLAGS flags, CK_SLOT_ID_PTR slotID,
				    CK_VOID_PTR pReserved);
};

/* Optional init_args structure for C_Initialize */
typedef struct CK_C_INITIALIZE_ARGS CK_C_INITIALIZE_ARGS;
typedef struct CK_C_INITIALIZE_ARGS *CK_C_INITIALIZE_ARGS_PTR;

struct CK_C_INITIALIZE_ARGS {
	CK_CREATEMUTEX		CreateMutex;
	CK_DESTROYMUTEX		DestroyMutex;
	CK_LOCKMUTEX		LockMutex;
	CK_UNLOCKMUTEX		UnlockMutex;
	CK_FLAGS		flags;
	CK_VOID_PTR		reserved;
};

/* Flags for field flags of struct ck_c_initialize_args */
#define CKF_LIBRARY_CANT_CREATE_OS_THREADS	(1U << 0)
#define CKF_OS_LOCKING_OK			(1U << 1)

CK_RV C_Initialize(CK_VOID_PTR pInitArgs);

CK_RV C_Finalize(CK_VOID_PTR pReserved);

CK_RV C_GetInfo(CK_INFO_PTR pInfo);

CK_RV C_GetFunctionList(CK_FUNCTION_LIST_PTR_PTR ppFunctionList);

CK_RV C_GetSlotList(CK_BBOOL tokenPresent,
		    CK_SLOT_ID_PTR pSlotList, CK_ULONG_PTR pulCount);

CK_RV C_GetSlotInfo(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo);

CK_RV C_GetTokenInfo(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo);

CK_RV C_GetMechanismList(CK_SLOT_ID slotID,
			 CK_MECHANISM_TYPE_PTR pMechanismList,
			 CK_ULONG_PTR pulCount);

CK_RV C_GetMechanismInfo(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type,
			 CK_MECHANISM_INFO_PTR pInfo);

CK_RV C_InitToken(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin,
		  CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel);

CK_RV C_InitPIN(CK_SESSION_HANDLE hSession,
		CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen);

CK_RV C_SetPIN(CK_SESSION_HANDLE hSession,
	       CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldLen,
	       CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen);

CK_RV C_OpenSession(CK_SLOT_ID slotID, CK_FLAGS flags,
		    CK_VOID_PTR pApplication, CK_NOTIFY Notify,
		    CK_SESSION_HANDLE_PTR phSession);

CK_RV C_CloseSession(CK_SESSION_HANDLE hSession);

CK_RV C_CloseAllSessions(CK_SLOT_ID slotID);

CK_RV C_GetSessionInfo(CK_SESSION_HANDLE hSession, CK_SESSION_INFO_PTR pInfo);

CK_RV C_GetOperationState(CK_SESSION_HANDLE hSession,
			  CK_BYTE_PTR pOperationState,
			  CK_ULONG_PTR pulOperationStateLen);

CK_RV C_SetOperationState(CK_SESSION_HANDLE hSession,
			  CK_BYTE_PTR pOperationState,
			  CK_ULONG ulOperationStateLen,
			  CK_OBJECT_HANDLE hEncryptionKey,
			  CK_OBJECT_HANDLE hAuthenticationKey);

CK_RV C_Login(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType,
	      CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen);

CK_RV C_Logout(CK_SESSION_HANDLE hSession);

CK_RV C_CreateObject(CK_SESSION_HANDLE hSession,
		     CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
		     CK_OBJECT_HANDLE_PTR phObject);

CK_RV C_CopyObject(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject,
		   CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
		   CK_OBJECT_HANDLE_PTR phNewObject);

CK_RV C_DestroyObject(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject);

CK_RV C_GetObjectSize(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject,
		      CK_ULONG_PTR pulSize);

CK_RV C_GetAttributeValue(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject,
			  CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);

CK_RV C_SetAttributeValue(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject,
			  CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);

CK_RV C_FindObjectsInit(CK_SESSION_HANDLE hSession,
			CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);

CK_RV C_FindObjects(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE_PTR phObject,
		    CK_ULONG ulMaxObjectCount, CK_ULONG_PTR pulObjectCount);

CK_RV C_FindObjectsFinal(CK_SESSION_HANDLE hSession);

CK_RV C_EncryptInit(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		    CK_OBJECT_HANDLE hKey);

CK_RV C_Encrypt(CK_SESSION_HANDLE hSession,
		CK_BYTE_PTR pData, CK_ULONG ulDataLen,
		CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen);

CK_RV C_EncryptUpdate(CK_SESSION_HANDLE hSession,
		      CK_BYTE_PTR pPart, CK_ULONG ulPartLen,
		      CK_BYTE_PTR pEncryptedData,
		      CK_ULONG_PTR pulEncryptedDataLen);

CK_RV C_EncryptFinal(CK_SESSION_HANDLE hSession,
		     CK_BYTE_PTR pLastEncryptedPart,
		     CK_ULONG_PTR pulLastEncryptedPartLen);

CK_RV C_DecryptInit(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		    CK_OBJECT_HANDLE hKey);

CK_RV C_Decrypt(CK_SESSION_HANDLE hSession,
		CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen,
		CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);

CK_RV C_DecryptUpdate(CK_SESSION_HANDLE hSession,
		      CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen,
		      CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);

CK_RV C_DecryptFinal(CK_SESSION_HANDLE hSession,
		     CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen);

CK_RV C_DigestInit(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism);

CK_RV C_Digest(CK_SESSION_HANDLE hSession,
	       CK_BYTE_PTR pData, CK_ULONG ulDataLen,
	       CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);

CK_RV C_DigestUpdate(CK_SESSION_HANDLE hSession,
		     CK_BYTE_PTR pPart, CK_ULONG ulPartLen);

CK_RV C_DigestKey(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey);

CK_RV C_DigestFinal(CK_SESSION_HANDLE hSession,
		    CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);

CK_RV C_SignInit(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		 CK_OBJECT_HANDLE hKey);

CK_RV C_Sign(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen,
	     CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);

CK_RV C_SignUpdate(CK_SESSION_HANDLE hSession,
		   CK_BYTE_PTR pPart, CK_ULONG ulPartLen);

CK_RV C_SignFinal(CK_SESSION_HANDLE hSession,
		  CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);

CK_RV C_SignRecoverInit(CK_SESSION_HANDLE hSession,
			CK_MECHANISM_PTR pMechanism,
			CK_OBJECT_HANDLE hKey);

CK_RV C_SignRecover(CK_SESSION_HANDLE hSession,
		    CK_BYTE_PTR pData, CK_ULONG ulDataLen,
		    CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);

CK_RV C_VerifyInit(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		   CK_OBJECT_HANDLE hKey);

CK_RV C_Verify(CK_SESSION_HANDLE hSession,
	       CK_BYTE_PTR pData, CK_ULONG ulDataLen,
	       CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen);

CK_RV C_VerifyUpdate(CK_SESSION_HANDLE hSession,
		     CK_BYTE_PTR pPart, CK_ULONG ulPartLen);

CK_RV C_VerifyFinal(CK_SESSION_HANDLE hSession,
		    CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen);

CK_RV C_VerifyRecoverInit(CK_SESSION_HANDLE hSession,
			  CK_MECHANISM_PTR pMechanism,
			  CK_OBJECT_HANDLE hKey);

CK_RV C_VerifyRecover(CK_SESSION_HANDLE hSession,
		      CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen,
		      CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);

CK_RV C_DigestEncryptUpdate(CK_SESSION_HANDLE hSession,
			    CK_BYTE_PTR pPart, CK_ULONG ulPartLen,
			    CK_BYTE_PTR pEncryptedPart,
			    CK_ULONG_PTR pulEncryptedPartLen);

CK_RV C_DecryptDigestUpdate(CK_SESSION_HANDLE hSession,
			    CK_BYTE_PTR pEncryptedPart,
			    CK_ULONG ulEncryptedPartLen,
			    CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);

CK_RV C_SignEncryptUpdate(CK_SESSION_HANDLE hSession,
			  CK_BYTE_PTR pPart,
			  CK_ULONG ulPartLen,
			  CK_BYTE_PTR pEncryptedPart,
			  CK_ULONG_PTR pulEncryptedPartLen);

CK_RV C_DecryptVerifyUpdate(CK_SESSION_HANDLE hSession,
			    CK_BYTE_PTR pEncryptedPart,
			    CK_ULONG ulEncryptedPartLen,
			    CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);

CK_RV C_GenerateKey(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
		    CK_OBJECT_HANDLE_PTR phKey);

CK_RV C_GenerateKeyPair(CK_SESSION_HANDLE hSession,
			CK_MECHANISM_PTR pMechanism,
			CK_ATTRIBUTE_PTR pPublicKeyTemplate,
			CK_ULONG ulPublicKeyAttributeCount,
			CK_ATTRIBUTE_PTR pPrivateKeyTemplate,
			CK_ULONG ulPrivateKeyAttributeCount,
			CK_OBJECT_HANDLE_PTR phPublicKey,
			CK_OBJECT_HANDLE_PTR phPrivateKey);

CK_RV C_WrapKey(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		CK_OBJECT_HANDLE hWrappingKey, CK_OBJECT_HANDLE hKey,
		CK_BYTE_PTR pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen);

CK_RV C_UnwrapKey(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		  CK_OBJECT_HANDLE hUnwrappingKey,
		  CK_BYTE_PTR pWrappedKey, CK_ULONG ulWrappedKeyLen,
		  CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
		  CK_OBJECT_HANDLE_PTR phKey);

CK_RV C_DeriveKey(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism,
		  CK_OBJECT_HANDLE hBaseKey,
		  CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount,
		  CK_OBJECT_HANDLE_PTR phKey);

CK_RV C_SeedRandom(CK_SESSION_HANDLE hSession,
		   CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen);

CK_RV C_GenerateRandom(CK_SESSION_HANDLE hSession,
		       CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen);

CK_RV C_GetFunctionStatus(CK_SESSION_HANDLE hSession);

CK_RV C_CancelFunction(CK_SESSION_HANDLE hSession);

CK_RV C_WaitForSlotEvent(CK_FLAGS flags, CK_SLOT_ID_PTR slotID,
			 CK_VOID_PTR pReserved);

#ifdef __cplusplus
}
#endif

#endif /*PKCS11_H*/
