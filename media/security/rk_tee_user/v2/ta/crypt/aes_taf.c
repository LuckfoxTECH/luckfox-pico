// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#include "aes_taf.h"
#include "aes_impl.h"

/* Encryption/decryption key */
const unsigned char key[KEYLENGTH(AES_256)] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

/* Encryption/decryption buffer */
unsigned long rk[RKLENGTH(AES_256)];

TEE_Result ta_entry_aes256ecb_encrypt(uint32_t param_types, TEE_Param params[4])
{
	size_t n_input_blocks = 0;
	size_t i = 0;

	/*
	 * It is expected that memRef[0] is input buffer and memRef[1] is
	 * output buffer.
	 */
	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
			    TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			    TEE_PARAM_TYPE_NONE)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* Check that input buffer is whole mult. of block size, in bits */
	if ((params[0].memref.size << 8) % AES_BLOCK_SIZE != 0)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Check that output buffer is whole mult. of block size, in bits */
	if ((params[1].memref.size << 8) % AES_BLOCK_SIZE != 0)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Set up for encryption */
	(void)rijndaelSetupEncrypt(rk, key, AES_256);

	n_input_blocks = params[0].memref.size / (AES_BLOCK_SIZE / 8);

	for (i = 0; i < n_input_blocks; i++) {
		const unsigned char *ciphertext = params[0].memref.buffer;
		unsigned char *plaintext = params[1].memref.buffer;

		rijndaelEncrypt(rk, NROUNDS(AES_256),
				&ciphertext[i * (AES_BLOCK_SIZE / 8)],
				&plaintext[i * (AES_BLOCK_SIZE / 8)]);
	}

	return TEE_SUCCESS;
}

TEE_Result ta_entry_aes256ecb_decrypt(uint32_t param_types, TEE_Param params[4])
{
	size_t n_input_blocks = 0;
	size_t i = 0;

	/*
	 * It is expected that memRef[0] is input buffer and memRef[1] is
	 * output buffer.
	 */
	if (param_types !=
	    TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
			    TEE_PARAM_TYPE_MEMREF_OUTPUT, TEE_PARAM_TYPE_NONE,
			    TEE_PARAM_TYPE_NONE)) {
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* Check that input buffer is whole mult. of block size, in bits */
	if ((params[0].memref.size << 8) % AES_BLOCK_SIZE != 0)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Check that output buffer is whole mult. of block size, in bits */
	if ((params[1].memref.size << 8) % AES_BLOCK_SIZE != 0)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Set up for decryption */
	(void)rijndaelSetupDecrypt(rk, key, AES_256);

	n_input_blocks = params[0].memref.size / (AES_BLOCK_SIZE / 8);

	for (i = 0; i < n_input_blocks; i++) {
		const unsigned char *ciphertext = params[0].memref.buffer;
		unsigned char *plaintext = params[1].memref.buffer;

		rijndaelDecrypt(rk, NROUNDS(AES_256),
				&ciphertext[i * (AES_BLOCK_SIZE / 8)],
				&plaintext[i * (AES_BLOCK_SIZE / 8)]);
	}

	return TEE_SUCCESS;
}
