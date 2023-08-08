/**
 * @file mqtt_client.h
 * @brief Wrapper of MQTT request and response
 * @version 0.1
 * @date 2015-05-26
 *
 * @copyright Copyright 2015-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

typedef PVOID_T MQ_HANDLE; // Definition of MQTT client handler

/**
 * @brief  MQTT msg is recved from broker
 *
 * @param[in] data Buffer of msg
 * @param[in] len Length os msg
 */
typedef VOID (*MQ_DATA_RECV_CB)(IN BYTE_T *data, IN UINT_T len);

/**
 * @brief Definition of MQTT client's credentials
 */
typedef struct {
	/** default topic subscribed from MQTT broker */
	CHAR_T *subcribe_topic;
	/** client id */
	CHAR_T *client_id;
	/** user name */
	CHAR_T *user_name;
	/** password */
	CHAR_T *passwd;
} MQ_CLIENT_IF_S;

/**
 * @brief  Create MQTT client instance
 *
 * @param[in] domain_tbl Domains of MQTT broker
 * @param[in] domain_num Count of domains
 * @param[in] serv_port Port of MQTT broker
 * @param[in] enable_tls TLS is enabled or not
 * @param[in] mqc_if Credentials of MQTT client
 * @param[in] s_alive MQTT hearbeat interval(unit sec), 0 will use the default timeout(60)
 * @param[in] recv_cb Callback when msg recved from broker
 * @param[in] wakeup_domain_tbl Domains of MQTT lowpower broker
 * @param[in] wakeup_domain_num Count of lowpower domains
 * @param[in] wakeup_serv_port Port of lowpower broker
 * @param[out] hand Handler of MQTT client
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET create_mqtt_hand(IN CONST CHAR_T **domain_tbl, IN CONST BYTE_T domain_num,
                             IN CONST USHORT_T serv_port, IN CONST BOOL_T enable_tls,
                             IN CONST MQ_CLIENT_IF_S *mqc_if, IN CONST USHORT_T s_alive,
                             IN CONST MQ_DATA_RECV_CB recv_cb, IN CONST CHAR_T **wakeup_domain_tbl,
                             IN CONST BYTE_T wakeup_domain_num, IN CONST USHORT_T wakeup_serv_port,
                             OUT MQ_HANDLE *hand);

/**
 * @brief  Update crenditials of MQTT client
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] p_username Username to connect to broker
 * @param[in] p_passwd Password to connect to broker
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_update_auth(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *p_username,
                             IN CONST CHAR_T *p_passwd);

/**
 * @brief MQTT client is connected to broker
 */
typedef VOID (*MQ_CONNED_CB)(VOID);
/**
 * @brief MQTT client is disconnected to broker
 */
typedef VOID (*MQ_DISCONN_CB)(VOID);
/**
 * @brief MQTT client is not permited to connect to broker
 *
 * @param[in] deny_times How many times this MQTT client is denied by broker
 */
typedef VOID (*MQ_CONN_DENY_CB)(IN BYTE_T deny_times);
/**
 * @brief Update authentication of MQTT client
 */
typedef VOID (*MQ_UPDATE_AUTH_CB)(VOID);

/**
 * @brief Check permission when connect to broker
 *
 * @return 1: permit to connect mqtt
 * 0: not permit to connect mqtt, and reconnect after a random delay
 * -1: not permit to connect mqtt, and reconnect after a short delay
 */
typedef INT_T (*MQ_PERMIT_CONN_CB)(VOID);

/**
 * @brief  Register event handers of MQTT client
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] conn_cb Hander when MQTT client is connected to broker
 * @param[in] dis_conn_cb Hander when MQTT client is disconnected to broker
 * @param[in] conn_deny_cb Hander when MQTT client is not permited to connect to broker
 * @param[in] permit_conn_cb Hander to check permission when connect to broker
 * @param[in] update_auth_cb Hander to update authentication
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_register_cb(IN CONST MQ_HANDLE hand, IN CONST MQ_CONNED_CB conn_cb,
                             IN CONST MQ_DISCONN_CB dis_conn_cb,
                             IN CONST MQ_CONN_DENY_CB conn_deny_cb,
                             IN CONST MQ_PERMIT_CONN_CB permit_conn_cb,
                             IN CONST MQ_UPDATE_AUTH_CB update_auth_cb);

/**
 * @brief  Connect to broker, MQTT client thread is created
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] name Name of MQTT thread
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_client_start(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *name);

/**
 * @brief  Restart MQTT client
 *
 * @param[in] hand Handler of MQTT client
 */
VOID mqtt_restart(IN CONST MQ_HANDLE hand);

/**
 * @brief  Disconnect MQTT client from broker, delete MQTT client thread
 *
 * @param[in] hand Handler of MQTT client
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_client_quit(IN CONST MQ_HANDLE hand);

/**
 * @brief  Whether MQTT client is connected to broker or not
 *
 * @param[in] hand Handler of MQTT client
 *
 * @return TRUE indicates connected to MQTT broker,
 * FALSE indicates disconnected to MQTT broker
 */
BOOL_T get_mqtt_conn_stat(IN CONST MQ_HANDLE hand);

/**
 * @brief  Disconnect MQTT client from broker, will re-connect later
 *
 * @param[in] hand Handler of MQTT client
 */
VOID mq_disconnect(IN CONST MQ_HANDLE hand);

/**
 * @brief  Release MQTT client
 *
 * @param[in] hand Handler of MQTT client
 *
 * @note MQTT client shall be closed before this API invoked
 */
VOID release_mqtt_hand(IN CONST MQ_HANDLE hand);

/**
 * @brief  Close MQTT client socket fd, will re-connect later
 *
 * @param[in] hand Handler of MQTT client
 */
VOID mqtt_close(IN CONST MQ_HANDLE hand);

/**
 * @brief  Async callback that indicates mqtt_publish_async success or not.
 *
 * @param[in] op_ret OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 * @param[in] prv_data Private data
 */
typedef VOID (*MQ_PUB_ASYNC_IFM_CB)(IN CONST OPERATE_RET op_ret, IN CONST VOID *prv_data);

/**
 * @brief Publish a msg to the specific MQTT topic with QOS level and timeout handler
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] topic Topic that the msg will be published
 * @param[in] qos QOS level
 * @param[in] data Buffer of msg content
 * @param[in] len Length of msg content
 * @param[in] to_lmt Timeout(unit sec) of QOS-1 ACK, 0 for the default value(3)
 * @param[in] cb Callback when the msg is published and ACK is recved or timeout
 * @param[in] prv_data Private data
 *
 * @note if(0 == qos) then to_lmt cb prv_data useless,
 * if(1 == qos) then need wait respond, other qos not support.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_publish_async(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *topic, IN CONST BYTE_T qos,
                               IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST UINT_T to_lmt,
                               IN CONST MQ_PUB_ASYNC_IFM_CB cb, IN VOID *prv_data);

/**
 * @brief Retrieve MQTT client socket fd
 *
 * @param[in] hand Handler of MQTT client
 *
 * @return MQTT client socket fd on success, -1 on error.
 */
INT_T mqtt_get_socket_fd(IN CONST MQ_HANDLE hand);

/**
 * @brief Retrieve MQTT PING interval
 *
 * @param[in] hand Handler of MQTT client
 *
 * @return MQTT PING interval(unit sec) on success, -1 on error.
 */
INT_T mqtt_get_alive_time_s(IN CONST MQ_HANDLE hand);

/**
 * @brief Subcribe a wakeup topic for lowpower device
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] wakeup_topic Topic to subscribe
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_book_wakeup_topic(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *wakeup_topic);

/**
 * @brief Subcribe an addition topic
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] topic Topic to subscribe
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_book_additional_topic(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *topic);

/**
 * @brief Send MQTT PING msg immediately
 *
 * @param[in] hand Handler of MQTT client
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_keep_alive(IN CONST MQ_HANDLE hand);

/**
 * @brief Callback when specific MQTT msg recv
 *
 * @param[in] topic Topic that msg is recved from
 * @param[in] data Buffer of msg content
 * @param[in] len Length of msg content
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef VOID (*MQ_MSG_RECV_CB)(IN CONST CHAR_T *topic, IN BYTE_T *data, IN UINT_T len);

/**
 * @brief Subcribe multiple topics within one MQTT request
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] topics Array of topics to subscribe
 * @param[in] cnt Count of topics to subscribe
 * @param[in] msg_cb Callback when MQTT msgs recv. If msg_cb is NULL,
 * msgs will transfer to default recv_cb of MQTT client
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_subscribe_multi_topics(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *topics[],
                                        IN CONST BYTE_T cnt, MQ_MSG_RECV_CB msg_cb,
                                        BOOL_T auto_resub);

/**
 * @brief Unsubcribe multiple topics within one MQTT request
 *
 * @param[in] hand Handler of MQTT client
 * @param[in] topics Array of topics to unsubscribe
 * @param[in] cnt Count of topics to unsubscribe
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mqtt_unsubscribe_multi_topics(IN CONST MQ_HANDLE hand, IN CONST CHAR_T *topics[],
                                          IN CONST BYTE_T cnt);

#ifdef __cplusplus
}
#endif

#endif
