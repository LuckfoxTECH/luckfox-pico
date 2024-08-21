/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef TA_CRYPT_H
#define TA_CRYPT_H

/* This UUID is generated with the ITU-T UUID generator at
   http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_CRYPT_UUID { 0xcb3e5ba0, 0xadf1, 0x11e0, \
	{ 0x99, 0x8b, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }

#define TA_CRYPT_CMD_SHA224             1
#define TA_CRYPT_CMD_SHA256             2
#define TA_CRYPT_CMD_AES256ECB_ENC      3
#define TA_CRYPT_CMD_AES256ECB_DEC      4

/*
 * TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation,
 *              uint32_t algorithm, uint32_t mode, uint32_t maxKeySize);
 * in/out   params[0].value.a = operation
 * in/out   params[0].value.b = algorithm
 * in       params[1].value.a = mode
 * in       params[2].value.b = maxKeySize
 */
#define TA_CRYPT_CMD_ALLOCATE_OPERATION 5

/*
 * void TEE_FreeOperation(TEE_OperationHandle operation);
 * in       params[0].value.a = operation
 */
#define TA_CRYPT_CMD_FREE_OPERATION     6

/*
 * void TEE_GetOperationInfo(TEE_OperationHandle operation,
 *              TEE_OperationInfo* operationInfo);
 * in       params[0].value.a = operation
 * out      params[1].memref  = operationInfo
 */
#define TA_CRYPT_CMD_GET_OPERATION_INFO 7

/*
 * void TEE_ResetOperation(TEE_OperationHandle operation);
 * in       params[0].value.a = operation
 */
#define TA_CRYPT_CMD_RESET_OPERATION    8

/*
 * TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation,
 *              TEE_ObjectHandle key);
 * in       params[0].value.a = operation
 * in       params[0].value.b = key
 */
#define TA_CRYPT_CMD_SET_OPERATION_KEY  9

/*
 * TEE_Result TEE_SetOperationKey2(TEE_OperationHandle operation,
 *              TEE_ObjectHandle key1, TEE_ObjectHandle key2);
 * in       params[0].value.a = operation
 * in       params[0].value.b = key1
 * in       params[0].value.a = key2
 */
#define TA_CRYPT_CMD_SET_OPERATION_KEY2 10

/*
 * void TEE_CopyOperation(TEE_OperationHandle dstOperation,
 *              TEE_OperationHandle srcOperation);
 * in       params[0].value.a = dstOperation
 * in       params[0].value.b = srcOperation
 */
#define TA_CRYPT_CMD_COPY_OPERATION     11

/*
 * void TEE_DigestUpdate(TEE_OperationHandle operation,
 *              void *chunk, size_t chunkSize);
 * in       params[0].value.a = operation
 * in       params[1].memref = chunk
 */
#define TA_CRYPT_CMD_DIGEST_UPDATE      12

/*
 * TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation,
 *              const void *chunk, size_t chunkLen,
 *              void *hash, size_t *hashLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = chunk
 * out      params[2].memref = hash
 */
#define TA_CRYPT_CMD_DIGEST_DO_FINAL    13

/*
 * void TEE_CipherInit(TEE_OperationHandle operation, const void *IV,
 *              size_t IVLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = IV
 */
#define TA_CRYPT_CMD_CIPHER_INIT        14

/*
 * TEE_Result TEE_CipherUpdate(TEE_OperationHandle operation,
 *              const void *srcData, size_t srcLen,
 *              void *destData, size_t *destLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = srcData
 * out      params[2].memref = dstData
 */
#define TA_CRYPT_CMD_CIPHER_UPDATE      15

/*
 * TEE_Result TEE_CipherDoFinal(TEE_OperationHandle operation,
 *              const void *srcData, size_t srcLen,
 *              void *destData, size_t *destLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = srcData
 * out      params[2].memref = destData
 */
#define TA_CRYPT_CMD_CIPHER_DO_FINAL    16

/*
 * void TEE_MACInit(TEE_OperationHandle operation,
 *              const void *IV, size_t IVLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = IV
 */
#define TA_CRYPT_CMD_MAC_INIT           17

/*
 * void TEE_MACUpdate(TEE_OperationHandle operation,
 *              const void *chunk, size_t chunkSize);
 * in       params[0].value.a = operation
 * in       params[1].memref = chunk
 */
#define TA_CRYPT_CMD_MAC_UPDATE         18

/*
 * TEE_Result TEE_MACFinalCompute(TEE_OperationHandle operation,
 *              const void *message, size_t messageLen,
 *              void *mac, size_t *macLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = message
 * out      params[2].memref = mac
 */
#define TA_CRYPT_CMD_MAC_FINAL_COMPUTE  19

/*
 * TEE_Result TEE_MACFinalCompare(TEE_OperationHandle operation,
 *              const void *message, size_t messageLen,
 *              const void *mac, size_t *macLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = message
 * in       params[2].memref = mac
 */
#define TA_CRYPT_CMD_MAC_FINAL_COMPARE  20

/*
 * TEE_Result TEE_AllocateTransientObject(TEE_ObjectType objectType,
 *              uint32_t maxObjectSize, TEE_ObjectHandle* object);
 * in       params[0].value.a = objectType
 * in       params[0].value.b = maxObjectSize
 * out      params[1].value.a = object;
 */
#define TA_CRYPT_CMD_ALLOCATE_TRANSIENT_OBJECT  21

/*
 * void TEE_FreeTransientObject(TEE_ObjectHandle object);
 * in       params[0].value.a = object
 */
#define TA_CRYPT_CMD_FREE_TRANSIENT_OBJECT      22

/*
 * void TEE_ResetTransientObject(TEE_ObjectHandle object);
 * in       params[0].value.a = object
 */
#define TA_CRYPT_CMD_RESET_TRANSIENT_OBJECT     23

/*
 * TEE_Result TEE_PopulateTransientObject(TEE_ObjectHandle object,
 *              TEE_Attribute *attrs, uint32_t attrCount);
 * in       params[0].value.a = object
 * in       params[1].memref = attrs
 */
#define TA_CRYPT_CMD_POPULATE_TRANSIENT_OBJECT  24

/*
 * void TEE_CopyObjectAttributes(TEE_ObjectHandle destObject,
 *              TEE_ObjectHandle srcObject);
 * in       params[0].value.a = destObject
 * in       params[0].value.b = srcObject
 */
#define TA_CRYPT_CMD_COPY_OBJECT_ATTRIBUTES     25

/*
 * TEE_Result TEE_GenerateKey(TEE_ObjectHandle object, uint32_t keySize,
 *              TEE_Attribute *params, uint32_t paramCount);
 * in       params[0].value.a = object
 * in       params[0].value.b = keySize
 * in       params[1].memref = params
 */
#define TA_CRYPT_CMD_GENERATE_KEY               26

/*
 * TEE_Result TEE_AsymmetricEncrypt(TEE_OperationHandle operation,
 *              const TEE_Attribute *params, uint32_t paramCount,
 *              const void *srcData, size_t srcLen, void *destData,
 *              size_t *destLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = params
 * in       params[2].memref = srcData
 * out      params[3].memref = destData
 */
#define TA_CRYPT_CMD_ASYMMETRIC_ENCRYPT         27

/*
 * TEE_Result TEE_AsymmetricDecrypt(TEE_OperationHandle operation,
 *              const TEE_Attribute *params, uint32_t paramCount,
 *              const void *srcData, size_t srcLen, void *destData,
 *              size_t *destLen)
 * in       params[0].value.a = operation
 * in       params[1].memref = params
 * in       params[2].memref = srcData
 * out      params[3].memref = destData
 */
#define TA_CRYPT_CMD_ASYMMETRIC_DECRYPT         28

/*
 * TEE_Result TEE_AsymmetricSignDigest(TEE_OperationHandle operation,
 *              const TEE_Attribute *params, uint32_t paramCount,
 *              const void *digest, size_t digestLen, void *signature,
 *              size_t *signatureLen)
 * in       params[0].value.a = operation
 * in       params[1].memref = params
 * in       params[2].memref = digest
 * out      params[3].memref = signature
 */
#define TA_CRYPT_CMD_ASYMMETRIC_SIGN_DIGEST     29

/*
 * TEE_Result TEE_AsymmetricVerifyDigest(TEE_OperationHandle operation,
 *              const TEE_Attribute *params, uint32_t paramCount,
 *              const void *digest, size_t digestLen, const void *signature,
 *              size_t signatureLen)
 * in       params[0].value.a = operation
 * in       params[1].memref = params
 * in       params[2].memref = digest
 * in       params[3].memref = signature
 */
#define TA_CRYPT_CMD_ASYMMETRIC_VERIFY_DIGEST   30

/*
 * void TEE_DeriveKey(TEE_OperationHandle operation,
 *              const TEE_Attribute *params, uint32_t paramCount,
 *              TEE_ObjectHandle derivedKey)
 * in       params[0].value.a = operation
 * in       params[1].memref = params
 * in       params[0].value.b = derivedKey
 */
#define TA_CRYPT_CMD_DERIVE_KEY                 31

/*
 * void TEE_RandomNumberGenerate(void *randomBuffer, size_t randomBufferLen);
 * out      params[0].memref = randomBuffer
 */
#define TA_CRYPT_CMD_RANDOM_NUMBER_GENERATE     32

/*
 * TEE_Result TEE_AEInit(TEE_OperationHandle operation,
 *              const void* nonce, size_t nonceLen,
 *              uint32_t tagLen, uint32_t AADLen, uint32_t payloadLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = nonce
 * in       params[0].value.b = tagLen
 * in       params[2].value.a = AADLen
 * in       params[2].value.b = payloadLen
 */
#define TA_CRYPT_CMD_AE_INIT                    33

/*
 * void TEE_AEUpdateAAD(TEE_OperationHandle operation,
 *              void* AADdata, size_t AADdataLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = AADdata
 */
#define TA_CRYPT_CMD_AE_UPDATE_AAD              34

/*
 * TEE_Result TEE_AEUpdate(TEE_OperationHandle operation,
 *              const void* srcData, size_t srcLen,
 *              void* destData, size_t *destLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = srcData
 * out      params[2].memref = destData
 */
#define TA_CRYPT_CMD_AE_UPDATE                  35

/*
 * TEE_Result TEE_AEEncryptFinal(TEE_OperationHandle operation,
 *              const void* srcData, size_t srcLen,
 *              void* destData, size_t* destLen,
 *              void* tag, size_t* tagLen);
 * in       params[0].value[0].a = operation
 * in       params[1].memref = srcData
 * out      params[2].memref = destData
 * out      params[3].memref = tag
 */
#define TA_CRYPT_CMD_AE_ENCRYPT_FINAL           36

/*
 * TEE_Result TEE_AEDecryptFinal(TEE_OperationHandle operation,
 *              const void* srcData, size_t srcLen,
 *              void* destData, size_t *destLen,
 *              const void* tag, size_t tagLen);
 * in       params[0].value.a = operation
 * in       params[1].memref = srcData
 * out      params[2].memref = destData
 * in       params[3].memref = tag
 */
#define TA_CRYPT_CMD_AE_DECRYPT_FINAL           37

/*
 * TEE_Result TEE_GetObjectBufferAttribute(TEE_ObjectHandle object,
 *              uint32_t attributeID, void* buffer, size_t* size);
 * in      params[0].value.a = object
 * in      params[0].value.b = attributeID
 * out     params[1].memrefs = buffer
 */
#define TA_CRYPT_CMD_GET_OBJECT_BUFFER_ATTRIBUTE 38

/*
 * TEE_Result TEE_GetObjectValueAttribute(TEE_ObjectHandle object,
 *              uint32_t attributeID, void* buffer, size_t* size);
 * in      params[0].value.a = object
 * in      params[0].value.b = attributeID
 * out     params[1].value.a = value a
 * out     params[1].value.b = value b
 */
#define TA_CRYPT_CMD_GET_OBJECT_VALUE_ATTRIBUTE 39

/* To set or get a global value */
#define TA_CRYPT_CMD_SETGLOBAL     40
#define TA_CRYPT_CMD_GETGLOBAL     41

/* If mbedtls is compiled with MBEDTLS_SELF_TEST, run the self tests */
#define TA_CRYPT_CMD_MBEDTLS_SELF_TESTS		42

/*
 * in	params[0].memref = cert-chain
 * in	params[1].memref = trust-anchor-cert
 */
#define TA_CRYPT_CMD_MBEDTLS_CHECK_CERT		43

/*
 * in	params[0].memref = Certificate request in PKCS#10 format
 * out	params[1].memref = Signed certificate in X.509 format
 * out	params[2].memref = Certificate chain
 */
#define TA_CRYPT_CMD_MBEDTLS_SIGN_CERT		44

/*
 * system pTA is used for adding entropy to RNG pool */
#define TA_CRYPT_CMD_SEED_RNG_POOL 45

/*
 * Testing arithmetical interface.
 *
 * Coding of signed 32-bit values:
 * a int32_t with its bit pattern stored in a 32-bit value
 */

#define TA_CRYPT_ARITH_INVALID_HANDLE	0xffffffff

/*
 * in	params[0].value.a:	Number of bits
 * out	params[1].value.b:	Handle to bignum variable
 */
#define TA_CRYPT_CMD_ARITH_NEW_VAR		46

/*
 * in	params[0].value.a:	Number of bits
 * in	params[0].value.b:	Handle to bignum variable modulus
 * out	params[1].value.a:	Handle to FMM context
 */
#define TA_CRYPT_CMD_ARITH_NEW_FMM_CTX		47

/*
 * in	params[0].value.a:	Number of bits
 * out	params[1].value.a:	Handle to FMM variable
 */
#define TA_CRYPT_CMD_ARITH_NEW_FMM_VAR		48

/*
 * in	params[0].value.a:	Handle to bignum variable, FMM context, or
 *				FMM variable
 */
#define TA_CRYPT_CMD_ARITH_FREE_HANDLE		49


/*
 * in	params[0].value.a:	Handle to bignum variable
 * in	params[0].value.b:	S32 representing the sign of the value
 * in	params[1].memref:	octet string representing the value
 */
#define TA_CRYPT_CMD_ARITH_FROM_OCTET_STRING	50

/*
 * in	params[0].value.a:	Handle to bignum variable
 * in	params[0].value.b:	S32 representing the value
 */
#define TA_CRYPT_CMD_ARITH_FROM_S32		51

/*
 * in	params[0].value.a:	Handle to bignum variable
 * out	params[1].value.a:	S32 representing the sign of the value
 * out	params[2].memref:	octet string representing the value
 */
#define TA_CRYPT_CMD_ARITH_GET_VALUE		52

/*
 * in	params[0].value.a:	Handle to bignum variable
 * out	params[1].value.a:	S32 the value
 */
#define TA_CRYPT_CMD_ARITH_GET_VALUE_S32	53

/*
 * in	params[0].value.a:	Handle to bignum variable
 * in	params[0].value.b:	Bit number
 * out	params[1].value.a:	Bit value
 */
#define TA_CRYPT_CMD_ARITH_GET_BIT		54

/*
 * in	params[0].value.a:	Handle to bignum variable
 * out	params[1].value.a:	Bit count
 */
#define TA_CRYPT_CMD_ARITH_GET_BIT_COUNT	55

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	number of bits
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_SHIFT_RIGHT		56

/*
 * in	params[0].value.a:	handle op1
 * in	params[0].value.b:	handle op2
 * out	params[1].value.a:	result
 */
#define TA_CRYPT_CMD_ARITH_CMP			57

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	S32 shortVal
 * out	params[1].value.a:	result
 */
#define TA_CRYPT_CMD_ARITH_CMP_S32		58

/*
 * in	params[0].value.a:	handle a
 * in	params[0].value.b:	handle b
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_ADD			59

/*
 * in	params[0].value.a:	handle a
 * in	params[0].value.b:	handle b
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_SUB			60

/*
 * in	params[0].value.a:	handle a
 * in	params[0].value.b:	handle b
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_MUL			61

/*
 * in	params[0].value.a:	handle a
 * in	params[0].value.b:	handle result
 */
#define TA_CRYPT_CMD_ARITH_NEG			62

/*
 * in	params[0].value.a:	handle a
 * in	params[0].value.b:	handle result
 */
#define TA_CRYPT_CMD_ARITH_SQR			63

/*
 * in	params[0].value.a:	handle op1
 * in	params[0].value.b:	handle op2
 * in	params[1].value.a:	handle result Q
 * in	params[1].value.b:	handle result R
 */
#define TA_CRYPT_CMD_ARITH_DIV			64

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	handle n
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_MOD			65

/*
 * in	params[0].value.a:	handle op1
 * in	params[0].value.b:	handle op2
 * in	params[1].value.a:	handle n
 * in	params[1].value.b:	handle result
 */
#define TA_CRYPT_CMD_ARITH_ADDMOD		66

/*
 * in	params[0].value.a:	handle op1
 * in	params[0].value.b:	handle op2
 * in	params[1].value.a:	handle n
 * in	params[1].value.b:	handle result
 */
#define TA_CRYPT_CMD_ARITH_SUBMOD		67

/*
 * in	params[0].value.a:	handle op1
 * in	params[0].value.b:	handle op2
 * in	params[1].value.a:	handle n
 * in	params[1].value.b:	handle result
 */
#define TA_CRYPT_CMD_ARITH_MULMOD		68

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	handle n
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_SQRMOD		69

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	handle n
 * in	params[1].value.a:	handle result
 */
#define TA_CRYPT_CMD_ARITH_INVMOD		70

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	handle n
 * in	params[1].value.a:	bool result
 */
#define TA_CRYPT_CMD_ARITH_IS_RELATIVE_PRIME	71

/*
 * in	params[0].value.a:	handle op1
 * in	params[0].value.b:	handle op2
 * in	params[1].value.a:	handle result u
 * in	params[1].value.b:	handle result v
 * in	params[2].value.a:	handle result gcd
 */
#define TA_CRYPT_CMD_ARITH_COMPUTE_EGCD		72

/*
 * in	params[0].value.a:	handle op
 * in	params[0].value.b:	confidence level
 * out	params[1].value.a:	S32 result
 */
#define TA_CRYPT_CMD_ARITH_IS_PRIME		73

/*
 * in	params[0].value.a:	handle src
 * in	params[0].value.b:	handle n
 * in	params[1].value.a:	handle FMM context
 * in	params[1].value.b:	handle result FMM variable
 */
#define TA_CRYPT_CMD_ARITH_TO_FMM		74

/*
 * in	params[0].value.a:	handle FMM src
 * in	params[0].value.b:	handle bigint n
 * in	params[1].value.a:	handle FMM context
 * in	params[1].value.b:	handle result bigint
 */
#define TA_CRYPT_CMD_ARITH_FROM_FMM		75

/*
 * in	params[0].value.a:	handle FMM op1
 * in	params[0].value.b:	handle FMM op2
 * in	params[1].value.a:	handle bigint n
 * in	params[1].value.b:	handle FMM context
 * in	params[2].value.a:	handle FMM result
 */
#define TA_CRYPT_CMD_ARITH_COMPUTE_FMM		76

/*
 * system PTA is used for deriving device and TA unique keys. This function in
 * the "crypt" TA is testing the key derivation.
 */
#define TA_CRYPT_CMD_DERIVE_TA_UNIQUE_KEY	77

/*
 * system PTA is used for deriving device and TA unique keys. This function in
 * the "crypt" TA is testing the key derivation. This function tries to derive
 * keys by using shared memory buffers (something that shall fail).
 *
 * in  params[0].memref.buffer     Buffer for extra data
 * in  params[0].memref.size       Size of extra data
 * out params[1].memref.buffer     Buffer for the derived key
 * out params[1].memref.size       Size of the derived key
 */
#define TA_CRYPT_CMD_DERIVE_TA_UNIQUE_KEY_SHM	78

/*
 * in	params[0].value.a:	algorithm
 * in	params[0].value.b:	element
 * out	params[1].value.a:	TEE_IsAlgorithmSupported() return status
 */
#define TA_CRYPT_CMD_IS_ALGO_SUPPORTED		79
#endif /*TA_CRYPT_H */
