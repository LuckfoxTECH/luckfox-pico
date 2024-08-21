/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef AES_IMPL_H
#define AES_IMPL_H

int rijndaelSetupEncrypt(unsigned long *rk, const unsigned char *key,
			 int keybits);

int rijndaelSetupDecrypt(unsigned long *rk, const unsigned char *key,
			 int keybits);

void rijndaelEncrypt(const unsigned long *rk, int nrounds,
		     const unsigned char plaintext[16],
		     unsigned char ciphertext[16]);

void rijndaelDecrypt(const unsigned long *rk, int nrounds,
		     const unsigned char ciphertext[16],
		     unsigned char plaintext[16]);

#define AES_BLOCK_SIZE		128

#define AES_128			128
#define AES_192			192
#define AES_256			256

#define KEYLENGTH(keybits)	((keybits)/8)
#define RKLENGTH(keybits)	((keybits)/8+28)
#define NROUNDS(keybits)	((keybits)/32+6)

#endif
