/*
 * Copyright (c) 2019, Rockchip Technology Co., Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * usage: decrypt cipher text with AES CTS mode,
 *        key is auto derived from hardware key in TEE.
 * params: cipher, input data
 *         cipher_len, input data length
 *         plain, output data
 *         plain_len, output data length
 * return: 0 success, others fail
 */
int rk_decrypt_data(unsigned char *cipher, unsigned int cipher_len,
				unsigned char *plain, unsigned int *plain_len);
/*
 * usage: encrypt plain text with AES CTS mode,
 *        key is auto derived from hardware key in TEE.
 * params: plain, input data
 *         plain_len, input data length
 *         cipher, output data
 *         cipher_len, output data length
 * return: 0 success, others fail
 */
int rk_encrypt_data(unsigned char *plain, unsigned int plain_len,
				unsigned char *cipher, unsigned int *cipher_len);
/*
 * usage: read secure boot enable flag.
 * params: flag, output data
 *         flag = 0 means disable, flag = 1 means enabled
 * return: 0 success, others fail
 */
int rk_read_secure_boot_enable_flag(uint8_t *flag);
/*
 * usage: read vboot key hash.
 * params: buf, output data
 *         length, output data length, it should be 32 or 64 bytes
 * return: 0 success, others fail
 */
int rk_read_vbootkey_hash(uint8_t *buf, uint32_t length);
