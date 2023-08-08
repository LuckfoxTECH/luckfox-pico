/**
 * @file uni_base64.h
 * @brief
 * @author nzy
 * @version 1.0.0
 * @date 2015-06-09
 */
#ifndef _UNI_BASE64_H
#define _UNI_BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tuya_base64_encode
 *
 * @param[in] bindata
 * @param[out] base64
 * @param[in] binlength
 *
 * @return
 */
char *tuya_base64_encode(const unsigned char *bindata, char *base64, int binlength);

/**
 * @brief tuya_base64_decode
 *
 * @param[out] base64
 * @param[in] bindata
 *
 * @return
 */
int tuya_base64_decode(const char *base64, unsigned char *bindata);

#ifdef __cplusplus
}
#endif
#endif
