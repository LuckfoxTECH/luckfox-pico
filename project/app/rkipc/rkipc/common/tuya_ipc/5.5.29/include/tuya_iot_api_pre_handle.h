/**
 * @file tuya_iot_api_pre_handle.h
 * @brief
 * @author tuya
 * @version 1.0.0
 * @date 2021-01-11
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
 */

#ifndef __TUYA_IOT_API_PRE_HANDLE_H
#define __TUYA_IOT_API_PRE_HANDLE_H

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef OPERATE_RET (*tuya_iot_upgrade_gw_notify_cb)(IN CONST FW_UG_S *fw,
                                                     IN CONST GET_FILE_DATA_CB get_file_cb,
                                                     IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,
                                                     IN CONST PVOID_T pri_data, BOOL_T notify,
                                                     UINT_T download_buf_size,
                                                     OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_refuse_upgrade_cb)(IN CONST FW_UG_S *fw, IN CONST CHAR_T *dev_id,
                                                  OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_reset_upgrade_detect_timer_cb)(IN CONST INT_T time_ms,
                                                              OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_gw_version_update_cb)(IN GW_PERMIT_DEV_TP_T type,
                                                     IN CONST CHAR_T *ver, OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_dev_upgd_progress_rept_cb)(IN CONST UINT_T percent,
                                                          IN CONST CHAR_T *devid,
                                                          IN CONST DEV_TYPE_T tp,
                                                          OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_dev_upgd_result_report_cb)(IN CONST CHAR_T *dev_id,
                                                          IN CONST DEV_TYPE_T type,
                                                          IN CONST INT_T result,
                                                          OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_get_wakeup_data_cb)(INOUT BYTE_T *wakeup_data_arr,
                                                   INOUT UINT_T *p_len, OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_get_heartbeat_data_cb)(INOUT BYTE_T *heartbeat_data_arr,
                                                      INOUT UINT_T *p_len, OUT BOOL_T *b_finish);

typedef INT_T (*tuya_iot_get_heartbeat_interval_cb)(OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_book_wakeup_topic_cb)(OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_book_additional_topic_cb)(IN CHAR_T *topic, OUT BOOL_T *b_finish);

typedef INT_T (*tuya_iot_get_mqtt_socket_fd_cb)(OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_send_custom_mqtt_msg_cb)(IN CONST UINT_T protocol,
                                                        IN CONST BYTE_T *p_data,
                                                        OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_send_mqtt_msg_with_topic_cb)(IN CONST CHAR_T *topic,
                                                            IN CONST UINT_T protocol,
                                                            IN CONST BYTE_T *p_data,
                                                            OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_upload_incre_data_cb)(IN CONST USHORT_T map_id, CONST UINT_T offset,
                                                     IN CONST BYTE_T *pbuffer,
                                                     IN CONST USHORT_T buf_len,
                                                     OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_media_data_report_cb)(IN CONST FLOW_BODY_ST *dt_body,
                                                     IN CONST UINT_T timeout, OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_media_data_report_v2_cb)(IN CONST FLOW_BODY_V2_ST *dt_body,
                                                        IN CONST UINT_T timeout,
                                                        OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_get_custom_cfg_cb)(IN CONST TY_CUSTOM_CFG_E type,
                                                  OUT CHAR_T **pp_cfg_str, OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_get_location_info_cb)(INOUT TY_LOCATION_INFO_S *p_location,
                                                     OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_custom_data_report_sync_cb)(
    IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *topic, IN CONST BYTE_T qos,
    IN CONST UINT_T timeout, OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_custom_data_report_async_cb)(
    IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *topic, IN CONST BYTE_T qos,
    IN CONST UINT_T timeout, OUT BOOL_T *b_finish);

typedef OPERATE_RET (*tuya_iot_dp_low_power_query_cb)(IN CONST UCHAR_T *dps, IN CONST UINT_T cnt,
                                                      OUT TY_RECV_OBJ_DP_S **obj_dps,
                                                      OUT TY_RECV_RAW_DP_S **raw_dps,
                                                      OUT BOOL_T *b_finish);

typedef struct {
	tuya_iot_upgrade_gw_notify_cb upgrade_gw_notify_cb;
	tuya_iot_refuse_upgrade_cb refuse_upgrade_cb;
	tuya_iot_reset_upgrade_detect_timer_cb reset_upgrade_detect_timer_cb;
	tuya_iot_gw_version_update_cb gw_version_update_cb;
	tuya_iot_dev_upgd_progress_rept_cb dev_upgd_progress_rept_cb;
	tuya_iot_dev_upgd_result_report_cb dev_upgd_result_report_cb;
	tuya_iot_get_wakeup_data_cb get_wakeup_data_cb;
	tuya_iot_get_heartbeat_data_cb get_heartbeat_data_cb;
	tuya_iot_get_heartbeat_interval_cb get_heartbeat_interval_cb;
	tuya_iot_book_wakeup_topic_cb book_wakeup_topic_cb;
	tuya_iot_book_additional_topic_cb book_additional_topic_cb;
	tuya_iot_get_mqtt_socket_fd_cb get_mqtt_socket_fd_cb;
	tuya_iot_send_custom_mqtt_msg_cb send_custom_mqtt_msg_cb;
	tuya_iot_send_mqtt_msg_with_topic_cb send_mqtt_msg_with_topic_cb;
	tuya_iot_upload_incre_data_cb upload_incre_data_cb;
	tuya_iot_media_data_report_cb media_data_report_cb;
	tuya_iot_media_data_report_v2_cb media_data_report_v2_cb;
	tuya_iot_get_custom_cfg_cb get_custom_cfg_cb;
	tuya_iot_get_location_info_cb get_location_info_cb;
	tuya_iot_custom_data_report_sync_cb custom_data_report_sync_cb;
	tuya_iot_custom_data_report_async_cb custom_data_report_async_cb;
	tuya_iot_dp_low_power_query_cb dp_low_power_query_cb;
} TUYA_IOT_API_PRE_HANDLE_CBS_S;

/**
 * @brief tuya_iot_api_pre_handle_register_cb
 *
 * @param[in] cbs
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_api_pre_handle_register_cb(IN TUYA_IOT_API_PRE_HANDLE_CBS_S *cbs);

/**
 * @brief tuya_iot_api_pre_handle_cb_get
 *
 * @return handler of callbacks
 */
TUYA_IOT_API_PRE_HANDLE_CBS_S *tuya_iot_api_pre_handle_cb_get();

#ifdef __cplusplus
}
#endif

#endif
