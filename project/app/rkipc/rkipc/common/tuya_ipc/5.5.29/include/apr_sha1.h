/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* NIST Secure Hash Algorithm
 * 	heavily modified by Uwe Hollerbach uh@alumni.caltech edu
 * 	from Peter C. Gutmann's implementation as found in
 * 	Applied Cryptography by Bruce Schneier
 * 	This code is hereby placed in the public domain
 */

#ifndef APR_SHA1_H
#define APR_SHA1_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file apr_sha1.h
 * @brief APR-UTIL SHA1 library
 */

/** size of the SHA1 DIGEST */
#define APR_SHA1_DIGESTSIZE 20

/**
 * Define the Magic String prefix that identifies a password as being
 * hashed using our algorithm.
 */
#define APR_SHA1PW_ID "{SHA}"

/** length of the SHA Password */
#define APR_SHA1PW_IDLEN 5

/** @see apr_sha1_ctx_t */
typedef struct apr_sha1_ctx_t apr_sha1_ctx_t;

/**
 * SHA1 context structure
 */
struct apr_sha1_ctx_t {
	/** message digest */
	unsigned int digest[5];
	/** 64-bit bit counts */
	unsigned int count_lo, count_hi;
	/** SHA data buffer */
	unsigned int data[16];
	/** unprocessed amount in data */
	int local;
};

/**
 * Initialize the SHA digest
 * @param context The SHA context to initialize
 */
void apr_sha1_init(apr_sha1_ctx_t *context);

/**
 * Update the SHA digest
 * @param context The SHA1 context to update
 * @param input The buffer to add to the SHA digest
 * @param inputLen The length of the input buffer
 */
void apr_sha1_update(apr_sha1_ctx_t *context, const char *input, unsigned int inputLen);

/**
 * Update the SHA digest with binary data
 * @param context The SHA1 context to update
 * @param input The buffer to add to the SHA digest
 * @param inputLen The length of the input buffer
 */
void apr_sha1_update_binary(apr_sha1_ctx_t *context, const unsigned char *input,
                            unsigned int inputLen);

/**
 * Finish computing the SHA digest
 * @param digest the output buffer in which to store the digest
 * @param context The context to finalize
 */
void apr_sha1_final(unsigned char digest[APR_SHA1_DIGESTSIZE], apr_sha1_ctx_t *context);

// calculate HMAC_SHA1
void HMAC_SHA1(unsigned char hmac[20], const unsigned char *key, int key_len,
               const unsigned char *message, int message_len);

#ifdef __cplusplus
}
#endif

#endif /* APR_SHA1_H */
