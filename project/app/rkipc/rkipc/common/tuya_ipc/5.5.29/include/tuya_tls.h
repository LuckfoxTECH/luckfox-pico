/**
 * @file tuya_tls.h
 * @brief Common process - tls include
 * @version 0.1
 * @date 2019-08-20
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef TUYA_TLS_H
#define TUYA_TLS_H

// mbedtls only used to encryption the seesion,not used to create the seesion
#include "ssl.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	CONST UINT8_T *pub_perm;
	CONST UINT8_T *pri_perm;
	UINT16_T pub_perm_len;
	UINT16_T pri_perm_len;
} tuya_tls_cert_perm_t;

typedef PVOID_T tuya_tls_hander;
typedef INT_T (*tuya_tls_send_cb)(VOID *p_custom_net_ctx, CONST BYTE_T *buf, SIZE_T len);
typedef INT_T (*tuya_tls_recv_cb)(VOID *p_custom_net_ctx, BYTE_T *buf, SIZE_T len);

VOID tuya_tls_load_der_info(VOID);

/**
 * @brief tls register
 *
 * @param[in] p_uuid uudi
 * @param[in] p_authkey auth key
 * @param[in] p_psk_key psk key
 *
 */
VOID tuya_tls_register_constant(IN CHAR_T *p_uuid, IN CHAR_T *p_authkey, IN CHAR_T *p_psk_key);

/**
 * @brief set tls verity
 *
 * @param[in] verify_mode VERIFY_NONE:0,VERIFY_OPTIONAL:1,VERIFY_REQUIRED:2
 */
VOID tuya_tls_set_ssl_verify(IN CONST INT_T verify_mode);

/**
 * @brief tls register x509 ca
 *
 * @param[in] p_ctx ca content
 * @param[in] p_der ca
 * @param[in] der_len ca len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_tls_register_x509_crt_der(VOID *p_ctx, UCHAR_T *p_der, UINT_T der_len);

/**
 * @brief load ca flag
 *
 * @param[in] load flag
 *
 */
VOID tuya_tls_set_load_cert(BOOL_T load);

/**
 * @brief set ca
 *
 * @param[in] cert ca
 * @param[in] len ca length
 *
 */
void tuya_tls_set_coustom_cert(CHAR_T *cert, INT_T len);

/**
 * @brief set pkey
 *
 * @param[in] cert ca
 * @param[in] len ca length
 * @param[in] client_pkey key
 * @param[in] pkey_len key length
 *
 */
void tuya_tls_set_client_cert_pkey(CHAR_T *cert, INT_T len, CHAR_T *client_pkey, INT_T pkey_len);

typedef enum {
	TY_TLS_CERT_EXPIRED,
} TY_TLS_EVENT_E;
typedef VOID (*tuya_tls_event_cb)(TY_TLS_EVENT_E event, VOID *p_args);

/**
 * @brief tls init
 *
 * @param[in] event_cb refer to tuya_tls_event_cb
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_tls_init(IN tuya_tls_event_cb event_cb);

/**
 * @brief tls connect
 *
 * @param[in] p_tls_handler refer to tuya_tls_hander
 * @param[in] hostname url
 * @param[in] port_num port
 * @param[in] sethostname need set hostname or not
 * @param[in] p_custom_net_ctx net content
 * @param[in] send_cb callback of send
 * @param[in] recv_cb callback of received
 * @param[in] socket_fd fd
 * @param[in] overtime_s connect timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_tls_connect(OUT tuya_tls_hander *p_tls_handler, IN CHAR_T *hostname,
                             IN INT_T port_num, IN INT_T sethostname, IN VOID *p_custom_net_ctx,
                             IN tuya_tls_send_cb send_cb, IN tuya_tls_recv_cb recv_cb,
                             IN INT_T socket_fd, IN INT_T overtime_s);

/**
 * @brief tls write
 *
 * @param[in] tls_handler refer to tuya_tls_hander
 * @param[in] buf write data
 * @param[in] len write length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_tls_write(IN tuya_tls_hander tls_handler, BYTE_T *buf, UINT_T len);

/**
 * @brief tls read
 *
 * @param[in] tls_handler refer to tuya_tls_hander
 * @param[out] buf read data
 * @param[in] len read length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_tls_read(IN tuya_tls_hander tls_handler, BYTE_T *buf, UINT_T len);

/**
 * @brief generated random
 *
 * @param[in] tls_handler refer to tuya_tls_hander
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_tls_disconnect(IN tuya_tls_hander tls_handler);

/**
 * @brief generated random
 *
 * @param[in] p_rng no use
 * @param[out] output random data
 * @param[in] output_len length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int __tuya_tls_random(void *p_rng, unsigned char *output, size_t output_len);

/**
 * @brief set tls calloc and free function
 *
 */
void tuya_tls_set_calloc_and_free(void);

#ifdef __cplusplus
}

#endif
#endif
