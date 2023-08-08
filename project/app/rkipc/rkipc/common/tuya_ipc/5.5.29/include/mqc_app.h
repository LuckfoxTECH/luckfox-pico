/**
 * @file mqtt_client.h
 * @brief TUYA MQTT service
 * @version 0.1
 * @date 2017-04-11
 *
 * @copyright Copyright 2017-2021 Tuya Inc. All Rights Reserved.
 */
#ifndef _MQC_APP_H
#define _MQC_APP_H

#include "gw_intf.h"
#include "mqtt_client.h"
#include "tuya_cloud_types.h"
#include "uni_rfc.h"

#ifdef __cplusplus
extern "C" {
#endif

// mqtt protocol
#define PRO_DATA_PUSH 4                 /* dev -> cloud push dp data */
#define PRO_CMD 5                       /* cloud -> dev send dp data */
#define PRO_DEV_UNBIND 8                /* cloud -> dev */
#define PRO_GW_RESET 11                 /* cloud -> dev reset dev */
#define PRO_TIMER_UG_INF 13             /* cloud -> dev update timer */
#define PRO_UPGD_REQ 15                 /* cloud -> dev update dev/gw */
#define PRO_UPGE_PUSH 16                /* dev -> cloud update upgrade percent */
#define PRO_IOT_DA_REQ 22               /* cloud -> dev send data req */
#define PRO_IOT_DA_RESP 23              /* dev -> cloud send data resp */
#define PRO_DEV_LINE_STAT_UPDATE 25     /* dev -> sub device online status update */
#define PRO_CMD_ACK 26                  /* dev -> cloud  dev send ackId to cloud */
#define PRO_MQ_EXT_CFG_INF 27           /* cloud -> dev runtime cfg update */
#define PRO_MQ_BLE_RAW_CMD 28           /* cloud -> ble dev RAW data */
#define PRO_MQ_QUERY_DP 31              /* cloud -> dev query dp stat */
#define PRO_GW_SIGMESH_TOPO_UPDATE 33   /* cloud -> dev sigmesh topo update */
#define PRO_UG_SUMMER_TABLE 41          // ug sumer timer table
#define PRO_GW_UPLOAD_LOG 45            /* dev -> cloud, upload log*/
#define PRO_GW_LINKAGE_UPDATE 49        /* cloud -> dev 场景更新推送 */
#define PRO_MQ_THINGCONFIG 51           /* 设备免密配网 */
#define PRO_MQ_PEGASUS_CONFIG 63        /* 闪电配网 */
#define PRO_MQ_APP_PROTOCOL_RX 64       /* 应用用户自定义协议 cloud->dev */
#define PRO_MQ_APP_PROTOCOL_TX 65       /* 应用用户自定义协议 dev->cloud */
#define PRO_MQ_NG_DEV_EVENT 55          /* NG下发的设备事件 */
#define PRO_DEV_WAKEUP 102              /* cloud -> dev, 更新子设备待唤醒标志 */
#define PRO_MQ_COMMON 100               /* cloud -> dev common protocol*/
#define PRO_MQ_EN_GW_ADD_DEV_REQ 200    // gw enable add sub device request
#define PRO_MQ_EN_GW_ADD_DEV_RESP 201   // gw enable add sub device respond
#define PRO_DEV_LC_GROUP_OPER 202       /* cloud -> dev */
#define PRO_DEV_LC_GROUP_OPER_RESP 203  /* dev -> cloud */
#define PRO_DEV_LC_SENCE_OPER 204       /* cloud -> dev */
#define PRO_DEV_LC_SENCE_OPER_RESP 205  /* dev -> cloud */
#define PRO_DEV_LC_SENCE_EXEC 206       /* cloud -> dev */
#define PRO_CLOUD_STORAGE_ORDER_REQ 300 /* 云存储订单 */
#define PRO_3RD_PARTY_STREAMING_REQ 301 /* echo show/chromecast request */
#define PRO_RTC_REQ 302                 /* cloud -> dev */
#define PRO_AI_DETECT_DATA_SYNC_REQ                                                                \
	304 /* 本地AI数据更新，当前用于人脸检测样本数据更新(新增/删除/变更) */
#define PRO_FACE_DETECT_DATA_SYNC 306   /* 人脸识别数据同步通知,门禁设备使用 */
#define PRO_CLOUD_STORAGE_EVENT_REQ 307 /* 联动触发云存储 */
#define PRO_DOORBELL_STATUS_REQ 308     /* 门铃请求被用户处理，接听或者拒绝 */
#define PRO_MQ_CLOUD_STREAM_GATEWAY 312
#define PRO_GW_COM_SENCE_EXE 403 /* cloud -> dev 原云端场景放到本地执行 */
#define PRO_DEV_ALARM_DOWN 701   /* cloud -> dev */
#define PRO_DEV_ALARM_UP 702     /* dev -> cloud */

/**
 * @brief  MQTT service initialization, will start after call to mqc_app_start
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_init(VOID);

/**
 * @brief  Start MQTT service
 *
 * @param[in] enable_time_check Enable replay check or not
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_start(BOOL_T enable_time_check);

/**
 * @brief  Disconnect MQTT client from broker, will re-connect later
 */
VOID mqc_app_disconnect(VOID);

/**
 * @brief  Disconnect MQTT client from broker, delete MQTT client thread
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_quit(VOID_T);

/**
 * @brief Close MQTT client socket fd, will re-connect later
 */
VOID mqc_app_close(VOID);

/**
 * @brief Get MQTT broker connection status
 *
 * @return TRUE on connected, FALSE on disconnected
 */
BOOL_T get_mqc_conn_stat(VOID);

/**
 * @brief MQTT data replay check
 *
 * @param[in] cmd_from Cmd origin
 * @param[in] seq Data sequence
 *
 * @return OPRT_OK on success. OPRT_SVC_MQTT_CMD_NOT_EXEC on error
 */
OPERATE_RET mqc_very_pcmcd_mcd(IN CONST UINT_T cmd_from, IN CONST UINT_T seq);

/**
 * @brief Publish dp report data to MQTT broker
 *
 * @param[in] rfc_da The data
 * @param[in] to_lmt Timeout to get ACK or else report error (default: 3s)
 * @param[in] cb Callback when ACK got or timeout
 * @param[in] prv_data Private data to callback
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_dp_report_async(RFC_DATA_S *rfc_da, IN CONST UINT_T to_lmt,
                                IN CONST MQ_PUB_ASYNC_IFM_CB cb, IN VOID *prv_data);

/**
 * @brief Publish dp query data to MQTT broker
 *
 * @param[in] rfc_da The data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_obj_dp_query(RFC_DATA_S *rfc_da);

/**
 * @brief Upload upgrade progress to broker
 *
 * @param[in] percent Protocol ID, see PRO_XX defined above
 * @param[in] devid Device ID that is in upgrade
 * @param[in] tp OTA channel, see DEV_TYPE_T
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_upgd_progress_rept_v41(IN CONST UINT_T percent, IN CONST CHAR_T *devid,
                                       IN CONST DEV_TYPE_T tp);

/**
 * @brief Publish data to MQTT broker asynchronously
 *
 * @param[in] pro Protocol ID, see PRO_XX defined above
 * @param[in] data The data
 * @param[in] qos Quality of Service (values: 0, 1)
 * @param[in] to_lmt Timeout to get ACK or else report error (default: 3s)
 * @param[in] cb Callback when ACK got or timeout
 * @param[in] prv_data Private data to callback
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_prot_data_rept_seq(IN CONST UINT_T pro, IN CONST CHAR_T *data, IN CONST BYTE_T qos,
                                   IN CONST UINT_T to_lmt, IN CONST MQ_PUB_ASYNC_IFM_CB cb,
                                   IN VOID *prv_data);

/**
 * @brief Get default wakeup data
 *
 * @param[in,out] wakeup_data_arr Buffer to wakeup data
 * @param[in,out] p_len Buffer length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_get_wakeup_data(INOUT BYTE_T *wakeup_data_arr, INOUT UINT_T *p_len);

/**
 * @brief Get default heartbeat data
 *
 * @param[in,out] heartbeat_data_arr Buffer to heartbeat data
 * @param[in,out] p_len Buffer length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_get_heartbeat_data(INOUT BYTE_T *heartbeat_data_arr, INOUT UINT_T *p_len);

/**
 * @brief Subcribe a wakeup topic for lowpower device
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_book_wakeup_topic(VOID);

/**
 * @brief Subcribe an addition topic
 *
 * @param[in] topic Topic to subscribe
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_book_additional_topic(IN CHAR_T *topic);

/**
 * @brief Subcribe multiple topics within one MQTT request
 *
 * @param[in] topics Array of topics to subscribe
 * @param[in] cnt Count of topics to subscribe
 * @param[in] msg_cb Callback when MQTT msgs recv. If msg_cb is NULL,
 * msgs will transfer to default recv_cb of MQTT client
 * @param[in] auto_resub Re-subscribe when mqtt re-connect
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
#define mqc_subscribe_multi_topics(topics, cnt, msg_cb)                                            \
	mqc_subscribe_multi_topics_ext(topics, cnt, msg_cb, TRUE)
OPERATE_RET mqc_subscribe_multi_topics_ext(IN CONST CHAR_T *topics[], IN CONST BYTE_T cnt,
                                           MQ_MSG_RECV_CB msg_cb, BOOL_T auto_resub);

/**
 * @brief Unsubcribe multiple topics within one MQTT request
 *
 * @param[in] topics Array of topics to unsubscribe
 * @param[in] cnt Count of topics to unsubscribe
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_unsubscribe_multi_topics(IN CONST CHAR_T *topics[], IN CONST BYTE_T cnt);

/**
 * @brief Get MQTT client hearbeat interval
 *
 * @param[in] hand Handler of MQTT client
 *
 * @return Heartbeat interval (unit sec), -1 on error
 */
INT_T mqc_get_alive_time_s(VOID);

/**
 * @brief Get MQTT client socket fd
 *
 * @return MQTT client socket fd on success, -1 on error.
 */
INT_T mqc_get_mqtt_socket_fd(VOID);

/**
 * @brief Restart MQTT client
 */
VOID mqc_restart(VOID);

#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1)
/**
 * @brief Get MQTT client's IP
 *
 * @return UNW_IP_ADDR_T format
 */
UINT_T mqc_get_mqtt_socket_ip(VOID);
#endif

/**
 * @brief Publish protocol data to the default topic
 *
 * @param[in] protocol Protocol ID, see PRO_XX defined above
 * @param[in] p_data The data to be published
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST BYTE_T *p_data);

/**
 * @brief Publish protocol data to the specific topic
 *
 * @param[in] topic Destination topic name
 * @param[in] protocol Protocol ID, see PRO_XX defined above
 * @param[in] p_data The data to be published
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_send_mqtt_msg_with_topic(IN CONST CHAR_T *topic, IN CONST UINT_T protocol,
                                         IN CONST BYTE_T *p_data);

/**
 * @brief Callback to handle protocol data
 *
 * @param[in] root_json Json encoded protocol data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*mqc_protocol_handler_cb)(IN ty_cJSON *root_json);

/**
 * @brief Register handler for specific protocol
 *
 * @param[in] mq_pro Protocol ID, see PRO_XX defined above
 * @param[in] handler Callback when protocol data got
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_register_cb(UINT_T mq_pro, mqc_protocol_handler_cb handler);

/**
 * @brief Publish map data to MQTT broker asynchronously
 *
 * @param[in] dt_body Map data defined by FLOW_BODY_ST
 * @param[in] to_lmt Timeout to get ACK or else report error (default: 3s)
 * @param[in] cb Callback when ACK got or timeout
 * @param[in] prv_data Private data to callback
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_media_pub_async(IN CONST FLOW_BODY_ST *dt_body, IN CONST UINT_T to_lmt,
                                IN CONST MQ_PUB_ASYNC_IFM_CB cb, IN VOID *prv_data);

/**
 * @brief Publish map data to MQTT broker synchronously
 *
 * @param[in] dt_body Map data defined by FLOW_BODY_ST
 * @param[in] timeout Timeout to get ACK or else report error
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_media_pub_sync(IN CONST FLOW_BODY_ST *dt_body, IN CONST UINT_T timeout);

/**
 * @brief Publish map data to MQTT broker synchronously
 *
 * @param[in] dt_body Map data defined by FLOW_BODY_V2_ST
 * @param[in] timeout Timeout to get ACK or else report error
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_media_pub_sync_v2(IN CONST FLOW_BODY_V2_ST *dt_body, IN CONST UINT_T timeout);

/**
 * @brief Publish map data to MQTT broker synchronously
 *
 * @param[in] map_id Map ID
 * @param[in] offset Offset
 * @param[in] pbuffer Map data
 * @param[in] buf_len Data length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_media_pub_data_sync(IN CONST USHORT_T map_id, IN CONST UINT_T offset,
                                    IN CONST BYTE_T *pbuffer, IN CONST USHORT_T buf_len);

/**
 * @brief Publish customized data to MQTT broker asynchronously
 *
 * @param[in] data Customized data
 * @param[in] len Data length
 * @param[in] topic Destination topic name
 * @param[in] qos Quality of Service (values: 0, 1)
 * @param[in] timeout Timeout to get ACK or else report error
 * @param[in] cb Callback when ACK got or timeout
 * @param[in] prv_data Private data to callback
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_custom_pub_async(IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *topic,
                                 IN CONST BYTE_T qos, IN CONST UINT_T timeout,
                                 IN CONST MQ_PUB_ASYNC_IFM_CB cb, IN VOID *prv_data);

/**
 * @brief Publish customized data to MQTT broker synchronously
 *
 * @param[in] data Customized data
 * @param[in] len Data length
 * @param[in] topic Destination topic name
 * @param[in] qos Quality of Service (values: 0, 1)
 * @param[in] timeout Timeout to get ACK or else report error
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_custom_pub_sync(IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *topic,
                                IN CONST BYTE_T qos, IN CONST UINT_T timeout);

/**
 * @brief Register APP extension protocol
 *
 * @param[in] sub_mq_pro APP extension protocol
 * @param[in] handler Hander when extension protocol data got
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_reg_ext_proto(CONST CHAR_T *sub_mq_pro, mqc_protocol_handler_cb handler);

/**
 * @brief Unregister APP extension protocol
 *
 * @param[in] sub_mq_pro APP extension protocol
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_unreg_ext_proto(CONST CHAR_T *sub_mq_pro);

/**
 * @brief Publish APP extension protocol data to MQTT broker
 *
 * @param[in] data APP extension protocol data
 * @param[in] qos Quality of Service (values: 0, 1)
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_app_ext_proto_data_rept(CONST CHAR_T *data, CONST BYTE_T qos);

/**
 * @brief Register SDK extension protocol
 *
 * @param[in] sub_mq_pro SDK extension protocol
 * @param[in] handler Hander when extension protocol data got
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_sdk_reg_ext_proto(CONST CHAR_T *sub_mq_pro, mqc_protocol_handler_cb handler);

/**
 * @brief Unregister SDK extension protocol
 *
 * @param[in] sub_mq_pro SDK extension protocol
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_sdk_unreg_ext_proto(CONST CHAR_T *sub_mq_pro);

/**
 * @brief Publish SDK extension protocol data to MQTT broker
 *
 * @param[in] data SDK extension protocol data
 * @param[in] qos QOS level(only 0/1 )
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_sdk_ext_proto_data_rept(CONST CHAR_T *data, CONST BYTE_T qos);

/**
 *  @brief Register NG extension protocol
 *
 * @param[in] ext_mq_pro NG extension protocol
 * @param[in] handler Hander when extension protocol data got
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_ng_reg_ext_proto(CONST CHAR_T *ext_mq_pro, mqc_protocol_handler_cb handler);

/**
 * @brief Unregister SDK extension protocol
 *
 * @param[in] ext_mq_pro SDK extension protocol
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_ng_unreg_ext_proto(IN CONST CHAR_T *ext_mq_pro);

/**
 * @brief Publish NG extension protocol data to MQTT broker
 * @param[in] data NG extension protocol data
 * @param[in] qos QOS level(only 0/1 )
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_ng_ext_proto_data_rept(CONST CHAR_T *data, CONST BYTE_T qos);

/**
 * @brief MQTT client is permited or not to connect to broker
 *
 * @return TRUE on permited, FALSE on denied
 */
typedef BOOL_T (*mqc_con_permit_cb)(VOID);

/**
 * @brief Set additional hook to check permission for MQTT broker
 *
 * @param[in] cb_permit Callback when permission is checking
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqc_set_connection_permit_cb(mqc_con_permit_cb cb_permit);

#ifdef __cplusplus
}
#endif

#endif
