#ifndef __TUYA_FFS_ADAPTER_H__
#define __TUYA_FFS_ADAPTER_H__

#include "mbedtls/pk.h"

#ifdef __cplusplus
extern "C" {
#endif

OPERATE_RET tuya_ffs_adapter_random_bytes(CHAR_T *random, INT_T len);

OPERATE_RET tuya_ffs_adapter_sha256(const tuya_ffs_stream_t *data_stream,
                                    tuya_ffs_stream_t *sha256_stream);

OPERATE_RET tuya_ffs_adapter_hmac_sha256(const tuya_ffs_stream_t *secret_key_stream,
                                         const tuya_ffs_stream_t *data_stream,
                                         tuya_ffs_stream_t *hmac_stream);

OPERATE_RET tuya_ffs_adapter_compute_ecdh_key(mbedtls_pk_context *private_key_stream,
                                              mbedtls_pk_context *public_key_stream,
                                              UINT8_T *secret_key_stream);

OPERATE_RET tuya_ffs_adapter_get_perm2der(const tuya_ffs_stream_t *perm, tuya_ffs_stream_t *der);

OPERATE_RET tuya_ffs_adapter_encode_base64(const tuya_ffs_stream_t *data_stream,
                                           tuya_ffs_stream_t *base64_stream);

OPERATE_RET tuya_ffs_adapter_encode_base85(const tuya_ffs_stream_t *data_stream,
                                           tuya_ffs_stream_t *base85_stream);

OPERATE_RET tuya_ffs_adapter_connect_ap(tuya_ffs_info_t *ffs_info, const CHAR_T *ssid,
                                        const CHAR_T *passwd, UINT_T timeout_sec);

OPERATE_RET tuya_ffs_adapter_scan_ssid(const CHAR_T *ssid);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __TUYA_FFS_ADAPTER_H__
