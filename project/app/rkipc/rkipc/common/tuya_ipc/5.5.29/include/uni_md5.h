/**
 * @file uni_md5.h
 * @brief
 * @author www.tuya.com
 * @version 1.0.0
 * @date 2021-01-11
 */

#ifndef __UNI_MD5_H
#define __UNI_MD5_H

#define MD5_DECRYPT_LEN 16

typedef struct {
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];
} UNI_MD5_CTX_S;

/**
 * @brief uni_md5_init
 *
 * @param context
 */
void uni_md5_init(UNI_MD5_CTX_S *context);

/**
 * @brief uni_md5_update
 *
 * @param context
 * @param[in] input
 * @param[in] inputlen
 */
void uni_md5_update(UNI_MD5_CTX_S *context, const unsigned char *input,
                    const unsigned int inputlen);

/**
 * @brief uni_md5_final
 *
 * @param context
 * @param digest[16]
 */
void uni_md5_final(UNI_MD5_CTX_S *context, unsigned char digest[16]);

#endif
