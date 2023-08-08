/**
 * @file tuya_iot_internal_api.h
 * @brief
 * @author tuya
 * @version 1.0.0
 * @date 2021-01-12
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
 */

#ifndef _TUYA_IOT_INTERNAL_API_H
#define _TUYA_IOT_INTERNAL_API_H

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"
#include "ty_cJSON.h"

#if defined(WIFI_GW) && (WIFI_GW == 1)
#include "thing_config.h"
#include "tuya_cloud_wifi_defs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* cloud-storage api  */
// TODO

/**
 * @brief iot_httpc_common_post_no_remalloc
 * @desc http api
 *
 * @param[in] api_name
 * @param[in] api_ver
 * @param[in] uuid
 * @param[in] devid
 * @param[in] post_data
 * @param[in] data_total_len
 * @param[in] p_head_other
 * @param[out] pp_result
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_common_post_no_remalloc(IN CONST CHAR_T *api_name, IN CONST CHAR_T *api_ver,
                                              IN CONST CHAR_T *uuid, IN CONST CHAR_T *devid,
                                              IN CHAR_T *post_data, IN UINT_T data_total_len,
                                              IN CONST CHAR_T *p_head_other,
                                              OUT ty_cJSON **pp_result);

/**
 * @brief iot_httpc_common_post
 *
 * @param[in] api_name
 * @param[in] api_ver
 * @param[in] uuid
 * @param[in] devid
 * @param[in] post_data
 * @param[in] p_head_other
 * @param[out] pp_result
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_common_post(IN CONST CHAR_T *api_name, IN CONST CHAR_T *api_ver,
                                  IN CONST CHAR_T *uuid, IN CONST CHAR_T *devid,
                                  IN CHAR_T *post_data, IN CONST CHAR_T *p_head_other,
                                  OUT ty_cJSON **pp_result);

/**
 * @brief iot_httpc_common_post_simple
 *
 * @param[in] api_name
 * @param[in] api_ver
 * @param[in] post_data
 * @param[in] p_head_other
 * @param[out] pp_result
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_common_post_simple(IN CONST CHAR_T *api_name, IN CONST CHAR_T *api_ver,
                                         IN CHAR_T *post_data, IN CONST CHAR_T *p_head_other,
                                         OUT ty_cJSON **pp_result);

typedef OPERATE_RET (*IOT_HTTP_GET_FILE_DATA_CB)(IN PVOID_T priv_data, IN CONST UINT_T total_len,
                                                 IN CONST UINT_T offset, IN CONST BYTE_T *data,
                                                 IN CONST UINT_T len, OUT UINT_T *remain_len);

/**
 * @brief iot_httpc_download_file
 *
 * @param[in] url
 * @param[in] mlk_buf_len
 * @param[in] gfd_cb
 * @param[in] priv_data
 * @param[in] total_len
 * @param[in/out] file_hmac[32]
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_download_file(IN CONST CHAR_T *url, IN CONST UINT_T mlk_buf_len,
                                    IN CONST IOT_HTTP_GET_FILE_DATA_CB gfd_cb, IN PVOID_T priv_data,
                                    IN CONST UINT_T total_len, INOUT BYTE_T file_hmac[32]);

/* mqtt api */
typedef OPERATE_RET (*iot_mqc_protocol_handler_cb)(IN ty_cJSON *root_json);

/**
 * @brief iot_mqc_app_register_cb
 * @desc 注册mqtt事件回调函数
 *
 * @param[in] mq_pro
 * @param[in] handler
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_mqc_app_register_cb(UINT_T mq_pro, iot_mqc_protocol_handler_cb handler);

typedef VOID (*iot_mqc_publish_result_cb)(IN CONST OPERATE_RET op_ret, IN CONST VOID *prv_data);
/**
 * @brief iot_mqc_send_custom_msg
 *
 * @param[in] protocol
 * @param[in] data
 * @param[in] qos
 * @param[in] to_lmt
 * @param[in] cb
 * @param[in] prv_data
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_mqc_send_custom_msg(IN CONST UINT_T protocol, IN CONST CHAR_T *data,
                                    IN CONST BYTE_T qos, IN CONST UINT_T to_lmt,
                                    IN CONST iot_mqc_publish_result_cb cb, IN VOID *prv_data);

/**
 * @brief iot_wd_common_write
 * @desc wd write
 *
 * @param[in] key
 * @param[in] value
 * @param[in] len
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_common_write(IN CONST CHAR_T *key, IN CONST BYTE_T *value, IN CONST UINT_T len);

/**
 * @brief iot_wd_common_read
 * @desc wd read
 *
 * @param[in] key
 * @param[out] value
 * @param[out] p_len
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_common_read(IN CONST CHAR_T *key, OUT BYTE_T **value, OUT UINT_T *p_len);

/**
 * @brief iot_wd_common_free_data
 * @desc wd free
 *
 * @param[in] data
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_common_free_data(IN BYTE_T *data);

/**
 * @brief iot_wd_common_delete
 * @desc wd delete
 *
 * @param[in] key
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_common_delete(IN CONST CHAR_T *key);

/**
 * @brief iot_wd_gw_desc_if_read
 * @desc wd read gateway information
 *
 * @param[out] di
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_gw_desc_if_read(OUT VOID *di);

#if defined(GW_SUPPORT_COUNTRY_CODE) && (GW_SUPPORT_COUNTRY_CODE == 1)
/**
 * @brief iot_wd_gw_write_country_code
 * @desc wd write country code
 *
 * @param[in] country_code
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_gw_write_country_code(CHAR_T *country_code);
#endif

typedef BYTE_T IOT_VAR_TP_T;
#define VT_CHAR 0
#define VT_BYTE 1
#define VT_SHORT 2
#define VT_USHORT 3
#define VT_INT 4
#define VT_BOOL 5
#define VT_STRING 6
#define VT_RAW 7

typedef struct {
	CHAR_T *key;
	IOT_VAR_TP_T vt;
	VOID *val;
	USHORT_T len;
} IOT_TY_DB_RW_S;

/**
 * @brief iot_wd_utils_serialize
 *
 * @param[in] rw
 * @param[in] rw_cnt
 * @param[out] out
 * @param[out] out_len
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_utils_serialize(IN CONST IOT_TY_DB_RW_S *rw, IN CONST UINT_T rw_cnt,
                                   OUT CHAR_T **out, OUT UINT_T *out_len);

/**
 * @brief iot_wd_utils_free_outbuf
 *
 * @param[out] out_buf
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_utils_free_outbuf(IN CHAR_T *out_buf);

/**
 * @brief iot_wd_utils_deserialize
 *
 * @param[in] in
 * @param[in] rw
 * @param[in] rw_cnt
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wd_utils_deserialize(IN CONST CHAR_T *in, INOUT IOT_TY_DB_RW_S *rw,
                                     IN CONST UINT_T rw_cnt);

/**
 * @brief iot_gw_user_token_bind
 *
 * @param[in] token
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_gw_user_token_bind(IN CONST CHAR_T *token);

/**
 * @brief iot_put_cloud_config
 * @desc the project switches to the release, reporting configuration
 *
 * @param[in] data
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_put_cloud_config(IN CONST CHAR_T *data);

#if defined(WIFI_GW) && (WIFI_GW == 1)
/**
 * @brief iot_wf_gw_unactive_custom_mode
 *
 * @param[in] wifi_mode
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_wf_gw_unactive_custom_mode(GW_WF_START_MODE wifi_mode);
#endif

/***********************************************************
网关升级时增加的前置过滤处理函数
如果*handled返回为TRUE,那么就不在通过正常的用户回调通知用户升级
如果*handled返回为FALSE,那么仍然通过正常的用户回调通知用户升级
***********************************************************/
typedef VOID (*PRE_GW_UG_INFORM_CB)(INOUT BOOL_T *handled, IN CONST FW_UG_S *fw);

/**
 * @brief iot_register_pre_gw_ug_cb
 *
 * @param pre_ug_cb
 *
 * @return none
 */
VOID iot_register_pre_gw_ug_cb(IN PRE_GW_UG_INFORM_CB pre_ug_cb);

/*
#define MBEDTLS_SSL_VERIFY_NONE                 0
#define MBEDTLS_SSL_VERIFY_OPTIONAL             1
#define MBEDTLS_SSL_VERIFY_REQUIRED             2
*/

/**
 * @brief iot_tls_set_ssl_verify
 *
 * @param[in] verify_mode
 *
 * @return none
 */
VOID iot_tls_set_ssl_verify(IN CONST INT_T verify_mode);

/**
 * @brief iot_tls_register_x509_crt_der
 * @desc Parse one or more PEM certificates from a buffer and add them to the chained list
 *
 * @param[in] p_ctx
 * @param[in] p_der
 * @param[in] der_len
 *
 * @return result
 */
INT_T iot_tls_register_x509_crt_der(VOID *p_ctx, UCHAR_T *p_der, UINT_T der_len);

/**
 * @brief iot_gw_update_versions
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_gw_update_versions(VOID);

/* RAW HTTP API */

typedef PVOID_T IOT_RAW_HTTP_S;

/**
 * @brief iot_httpc_raw_post
 *
 * @param[in] p_url
 * @param[in] data_total_len
 * @param[in/out] pp_raw_http
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_raw_post(IN CONST CHAR_T *p_url, IN CONST UINT_T data_total_len,
                               INOUT IOT_RAW_HTTP_S *pp_raw_http);

/**
 * @brief iot_httpc_raw_get
 *
 * @param[in] p_url
 * @param[out] pp_raw_http
 * @param[in] offset
 * @param[in] total_len
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_raw_get(IN CONST CHAR_T *p_url, INOUT IOT_RAW_HTTP_S *pp_raw_http,
                              IN UINT_T offset, IN UINT_T total_len);

/**
 * @brief iot_httpc_raw_put
 *
 * @param[in] p_url
 * @param[in] data_total_len
 * @param[out] pp_raw_http
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_raw_put(IN CONST CHAR_T *p_url, IN CONST UINT_T data_total_len,
                              INOUT IOT_RAW_HTTP_S *pp_raw_http);

/**
 * @brief iot_httpc_raw_write_content
 *
 * @param[in] p_raw_http
 * @param[in] buf
 * @param[in] buf_len
 * @param[in] chunked
 *
 * @return result
 */
INT_T iot_httpc_raw_write_content(IN IOT_RAW_HTTP_S p_raw_http, IN CONST BYTE_T *buf,
                                  IN CONST UINT_T buf_len, BOOL_T chunked);

/**
 * @brief iot_httpc_raw_read_resp_header
 *
 * @param[in/out] p_raw_http
 * @param[out] p_status_code
 * @param[out] p_chunked
 * @param[out] p_content_len
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_raw_read_resp_header(INOUT IOT_RAW_HTTP_S p_raw_http,
                                           OUT INT_T *p_status_code, OUT BOOL_T *p_chunked,
                                           OUT UINT_T *p_content_len);

/**
 * @brief iot_httpc_raw_read_content
 *
 * @param[in] p_raw_http
 * @param[in] buf
 * @param[in] buf_len
 *
 * @return result
 */
INT_T iot_httpc_raw_read_content(IN IOT_RAW_HTTP_S p_raw_http, IN BYTE_T *buf,
                                 IN CONST UINT_T buf_len);

/**
 * @brief iot_httpc_raw_close
 *
 * @param[in] p_raw_http
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_httpc_raw_close(IN IOT_RAW_HTTP_S p_raw_http);

/**
 * @brief tuya_iot_put_cloud_config
 *
 * @param[in] data
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_put_cloud_config(IN CONST CHAR_T *data);

#if defined(WIFI_GW) && (WIFI_GW == 1)
/**
 * @brief tuya_iot_thing_config_init
 *
 * @param[in] socket_cb
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_thing_config_init(IN THING_CONFIG_SOCKET_CB socket_cb);

/**
 * @brief tuya_iot_thing_config_start
 *
 * @param[in] ssid
 * @param[in] passwd
 * @param[in] token
 * @param[in] timeout
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_thing_config_start(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                                        IN CONST CHAR_T *token, IN CONST UINT_T timeout);

/**
 * @brief tuya_iot_thing_config_stop
 *
 * @return none
 */
VOID tuya_iot_thing_config_stop(VOID);
#endif

/**
 * @brief tuya_iot_compare_two_string
 *
 * @param cString1
 * @param cString2
 *
 * @return
 */
BOOL_T tuya_iot_compare_two_string(IN CONST CHAR_T *cString1, IN CONST CHAR_T *cString2);

typedef OPERATE_RET (*iot_special_dp_cb)(IN CONST UINT16_T dpid, IN CONST ty_cJSON *dp_obj);

/**
 * @brief iot_special_dp_register_cb
 *
 * @param[in] handler
 *
 * @return OPERATE_RET
 */
OPERATE_RET iot_special_dp_register_cb(IN iot_special_dp_cb handler);

/**
 * @brief tuya_iot_compare_two_string
 *
 * @param[in] cString1
 * @param[in] cString2
 *
 * @return
 */
BOOL_T tuya_iot_compare_two_string(IN CONST CHAR_T *cString1, IN CONST CHAR_T *cString2);

/**
 * @brief tuya_iot_get_json_safety_string
 * @desc will change the (") to the (\") in json file
 *
 * @param[in] json
 *
 * @return
 *
 * @note user shoule free memory
 */
CHAR_T *tuya_iot_get_json_safety_string(IN CONST CHAR_T *json);

#ifdef __cplusplus
}
#endif
#endif /*_TUYA_IOT_INTERNAL_API_H*/
