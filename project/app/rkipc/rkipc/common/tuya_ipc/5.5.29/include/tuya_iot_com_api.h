/**
 * @file tuya_iot_com_api.h
 * @brief
 * @author tuya
 * @version 1.0.0
 * @date 2021-01-11
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
 */
#ifndef __TUYA_IOT_COM_API_H
#define __TUYA_IOT_COM_API_H

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tuya_iot_get_sdk_info
 *
 * @return sdk information
 */
CHAR_T *tuya_iot_get_sdk_info(VOID);

/**
 * @brief tuya_iot_get_os_adapt_info
 *
 * @return os adapter information
 */
CHAR_T *tuya_iot_get_os_adapt_info(VOID_T);

/**
 * @brief tuya_iot_init_params
 * @desc init tuya_iot_sdk
 *
 * @param[in] fs_storge_path: filesystem read write storge path
 *            (if os have no fs,then fs_storge_path is invalid)
 * @param[in] p_param: custom init params
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_init_params(IN CONST CHAR_T *fs_storge_path,
                                 IN CONST TY_INIT_PARAMS_S *p_param);
#define tuya_iot_init(fs_storge_path) tuya_iot_init_params(fs_storge_path, NULL)

/**
 * @brief tuya_iot_kv_flash_init_param
 * @desc init kv flash
 *
 * @param[in] fs_storge_path: filesystem read write storge path
 *            (if os have no fs,then fs_storge_path is invalid)
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_kv_flash_init_param(IN CONST CHAR_T *fs_storge_path);
#define tuya_iot_kv_flash_init tuya_iot_kv_flash_init_param

/**
 * @brief tuya_iot_oem_set
 * @desc set oem mode
 *
 * @param[in] oem
 *
 * @return none
 */
VOID tuya_iot_oem_set(IN CONST BOOL_T oem);

/**
 * @brief tuya_iot_set_udf_parameter
 * @desc set user defined funtions
 *
 * @param[in] udf: user defined funtions(json), max length: 255
 *
 * @return OPRT_OK: success  Other: fail
 *
 * @note call before init
 */
OPERATE_RET tuya_iot_set_udf_parameter(IN CONST CHAR_T *udf);

/**
 * @brief tuya_iot_upload_rst_log_set
 * @desc set rst log upload mode
 *
 * @param[in] upload
 *
 * @return none
 *
 * @note must call first
 */
VOID tuya_iot_upload_rst_log_set(IN CONST BOOL_T upload);

/**
 * @brief tuya_iot_ignore_upgrade_set
 * @desc set rst log upload mode
 *
 * @param[in] ignore
 *
 * @return none
 *
 * @note must call first
 */
VOID tuya_iot_ignore_upgrade_set(IN CONST BOOL_T ignore);

/**
 * @brief tuya_iot_mqtt_disconnect
 * @desc force disconnect MQTT
 *
 * @return none
 *
 * @note must call first
 */
VOID tuya_iot_mqtt_disconnect(VOID);

/**
 * @brief tuya_iot_mqtt_restart
 * @desc restart MQTT
 *
 * @return
 *
 * @note must call first
 */
VOID tuya_iot_mqtt_restart(VOID);

/**
 * @brief tuya_iot_upgrade_gw_notify
 * @desc upgrage gateway firmware
 *
 * @param[in] fw: firmware info
 * @param[in] get_file_cb: download content storage callback
 * @param[in] upgrd_nofity_cb: firmware download finish callback
 * @param[in] pri_data: private param of get_file_cb && upgrd_nofity_cb
 * @param[in] notify: notify app the upgrade status
 * @param[in] download_buf_size: max download buffer size when downloading
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_upgrade_gw_notify(IN CONST FW_UG_S *fw, IN CONST GET_FILE_DATA_CB get_file_cb,
                                       IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,
                                       IN CONST PVOID_T pri_data, BOOL_T notify,
                                       UINT_T download_buf_size);
#define tuya_iot_upgrade_gw(fw, get_file_cb, upgrd_nofity_cb, pri_data)                            \
	tuya_iot_upgrade_gw_notify(fw, get_file_cb, upgrd_nofity_cb, pri_data, TRUE, 0)

/**
 * @brief tuya_iot_refuse_upgrade
 *
 * @param[in] fw
 * @param[in] dev_id
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_refuse_upgrade(IN CONST FW_UG_S *fw, IN CONST CHAR_T *dev_id);

/**
 * @brief tuya_iot_reset_upgrade_detect_timer
 * @desc reset the upgrade detect time
 *
 * @param[in] time_ms: the time you want start the detect
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_reset_upgrade_detect_timer(IN CONST INT_T time_ms);

/**
 * @brief tuya_iot_upgrade_dev_notify
 * @desc upgrage sub-device/soc/mcu firmware
 *
 * @param[in] devid: if upgrade sub-device, then devid = sub-device_id
 *                   if upgrade soc/mcu, then devid = NULL
 * @param[in] fw: firmware info
 * @param[in] get_file_cb: download content storage callback
 * @param[in] upgrd_nofity_cb: firmware download finish callback
 * @param[in] pri_data: private param of get_file_cb && upgrd_nofity_cb
 * @param[in] notify
 * @param[in] download_buf_size
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_upgrade_dev_notify(IN CONST CHAR_T *devid, IN CONST FW_UG_S *fw,
                                        IN CONST GET_FILE_DATA_CB get_file_cb,
                                        IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,
                                        IN CONST PVOID_T pri_data, BOOL_T notify,
                                        UINT_T download_buf_size);
#define tuya_iot_upgrade_dev(devid, fw, get_file_cb, upgrd_nofity_cb, pri_data)                    \
	tuya_iot_upgrade_dev_notify(devid, fw, get_file_cb, upgrd_nofity_cb, pri_data, TRUE, 0)

/**
 * @brief tuya_iot_get_dp_desc
 * @desc get dp info of a sub-device and a dp id
 *
 * @param[in] dev_id: sub-device id
 * @param[in] dpid: dp id
 *
 * @return (DP_DESC_IF_S *): success  NULL: fail
 */
DP_DESC_IF_S *tuya_iot_get_dp_desc(IN CONST CHAR_T *dev_id, IN CONST BYTE_T dpid);

/**
 * @brief tuya_iot_get_dp_prop_value
 * @desc get dp value of a sub-device and a dp id
 *
 * @param[in] dev_id: sub-device id
 * @param[in] dpid: dp id
 *
 * @return (DP_PROP_VALUE_U *): success  NULL: fail
 */
DP_PROP_VALUE_U *tuya_iot_get_dp_prop_value(IN CONST CHAR_T *dev_id, IN CONST BYTE_T dpid);

/**
 * @brief tuya_iot_dev_upgd_progress_rept
 *
 * @param[in] percent
 * @param[in] devid
 * @param[in] tp
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_dev_upgd_progress_rept(IN CONST UINT_T percent, IN CONST CHAR_T *devid,
                                            IN CONST DEV_TYPE_T tp);

/**
 * @brief tuya_iot_dev_upgd_result_report
 *
 * @param[in] dev_id
 * @param[in] type
 * @param[in] result
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_dev_upgd_result_report(IN CONST CHAR_T *dev_id, IN CONST DEV_TYPE_T type,
                                            IN CONST INT_T result);

/**
 * @brief tuya_iot_regist_dp_rept_cb
 *
 * @param[in] dp_rept_cbs
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_regist_dp_rept_cb(IN DP_REPT_HADLE_CB_S *dp_rept_cbs);

/**
 * @brief dev_report_dp_json_async
 * @desc report dp info a-synced.
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] dp_data: dp array header
 * @param[in] cnt: dp array count
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id, IN CONST TY_OBJ_DP_S *dp_data,
                                     IN CONST UINT_T cnt);

/**
 * @brief dev_report_dp_json_async_force
 * @desc report dp info a-synced.
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] dp_data: dp array header
 * @param[in] cnt: dp array count
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_report_dp_json_async_force(IN CONST CHAR_T *dev_id, IN CONST TY_OBJ_DP_S *dp_data,
                                           IN CONST UINT_T cnt);

/**
 * @brief dev_query_dp_json_async
 * @desc report dp info a-synced.
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] dp_data: dp array header
 * @param[in] cnt: dp array count
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_query_dp_json_async(IN CONST CHAR_T *dev_id, IN CONST TY_OBJ_DP_S *dp_data,
                                    IN CONST UINT_T cnt);

/**
 * @brief dev_query_dp_json_async_force
 * @desc report dp info a-synced.
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] dp_data: dp array header
 * @param[in] cnt: dp array count
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_query_dp_json_async_force(IN CONST CHAR_T *dev_id, IN CONST TY_OBJ_DP_S *dp_data,
                                          IN CONST UINT_T cnt);

/**
 * @brief dev_report_dp_raw_sync_extend
 * @desc report dp raw info synced.
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] dpid: raw dp id
 * @param[in] data: raw data
 * @param[in] len: len of raw data
 * @param[in] timeout: function blocks until timeout seconds
 * @param[in] enable_auto_retrans
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_report_dp_raw_sync_extend(IN CONST CHAR_T *dev_id, IN CONST BYTE_T dpid,
                                          IN CONST BYTE_T *data, IN CONST UINT_T len,
                                          IN CONST UINT_T timeout,
                                          IN CONST BOOL_T enable_auto_retrans);
#define dev_report_dp_raw_sync(dev_id, dpid, data, len, timeout)                                   \
	dev_report_dp_raw_sync_extend(dev_id, dpid, data, len, timeout, TRUE)

/**
 * @brief dev_report_dp_raw_sync_extend_with_time
 * @desc report dp raw info synced.
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] dpid: raw dp id
 * @param[in] data: raw data
 * @param[in] len: len of raw data
 * @param[in] timeout: function blocks until timeout seconds
 * @param[in] enable_auto_retrans
 * @param[in] time_str
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_report_dp_raw_sync_extend_with_time(IN CONST CHAR_T *dev_id, IN CONST BYTE_T dpid,
                                                    IN CONST BYTE_T *data, IN CONST UINT_T len,
                                                    IN CONST UINT_T timeout,
                                                    IN CONST BOOL_T enable_auto_retrans,
                                                    IN CONST CHAR_T *time_str);
#define dev_report_dp_raw_sync_with_time(dev_id, dpid, data, len, timeout, time_str)               \
	dev_report_dp_raw_sync_extend_with_time(dev_id, dpid, data, len, timeout, TRUE, time_str)

/**
 * @brief dev_report_dp_stat_sync_extend
 * @desc: report dp status info synced.
 *        if time_stamp==0, time_stamp = time of msg arrival of the server
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                    if gateway/soc/mcu, then devid = NULL
 * @param[in] dp_data: dp status array header
 * @param[in] cnt: dp status array count
 * @param[in] timeout: function blocks until timeout seconds
 * @param[in] enable_auto_retrans
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET dev_report_dp_stat_sync_extend(IN CONST CHAR_T *dev_id, IN CONST TY_OBJ_DP_S *dp_data,
                                           IN CONST UINT_T cnt, IN CONST UINT_T timeout,
                                           IN CONST BOOL_T enable_auto_retrans);
#define dev_report_dp_stat_sync(dev_id, dp_data, cnt, timeout)                                     \
	dev_report_dp_stat_sync_extend(dev_id, dp_data, cnt, timeout, TRUE)

/**
 * @brief tuya_iot_get_wakeup_data
 * @desc get mqtt wakeup string pattern
 *
 * @param[in/out] wakeup_data_arr: wakeup string arr
 * @param[in/out] p_len: wakeup string len
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_get_wakeup_data(INOUT BYTE_T *wakeup_data_arr, INOUT UINT_T *p_len);

/**
 * @brief tuya_iot_get_heartbeat_data
 * @desc get mqtt heartbeat string pattern
 *
 * @param[in/out] heartbeat_data_arr: heartbeat string arr
 * @param[in/out] p_len: heartbeat string len
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_get_heartbeat_data(INOUT BYTE_T *heartbeat_data_arr, INOUT UINT_T *p_len);

/**
 * @brief tuya_iot_get_heartbeat_interval
 * @desc get mqtt heartbeat interval
 *
 * @return interval value
 */
INT_T tuya_iot_get_heartbeat_interval(VOID);

/**
 * @brief tuya_iot_book_wakeup_topic
 * @desc unscribe mqtt msg topic and subscribe wakeup topic
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_book_wakeup_topic(VOID);

/**
 * @brief tuya_iot_get_mqtt_socket_fd
 * @desc get curr mqtt socket fd
 *
 * @return mqtt socket fd
 */
INT_T tuya_iot_get_mqtt_socket_fd(VOID);

/**
 * @brief tuya_iot_send_custom_mqtt_msg
 * @desc send a custom protocol mqtt msg
 *
 * @param protocol: mqtt protocol
 * @param p_data: mqtt msg
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET tuya_iot_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST BYTE_T *p_data);

///***********************************************************
//*  Function: tuya_iot_media_init
//*  Input: none
//*  Output: none
//*  Return: OPERATE_RET
//***********************************************************/
// OPERATE_RET tuya_iot_media_init(VOID);

/**
 * @brief tuya_iot_upload_incre_data
 *
 * @param[in] map_id: Each map has an Id
 * @param[in] offset: Map data offset
 * @param[in] pbuffer: Map data cache pointer
 * @param[in] buf_len: Map data cache length
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_upload_incre_data(IN CONST USHORT_T map_id, IN CONST UINT_T offset,
                                       IN CONST BYTE_T *pbuffer, IN CONST USHORT_T buf_len);

/**
 * @brief tuya_iot_media_data_report
 *
 * @param[in] dt_body: media data
 * @param[in] timeout: need report time
 *
 * @return OPERATE_RET
 *
 * @warning please dont use tuya_iot_media_data_report and tuya_iot_media_data_report_v2 in one
 * application
 */
OPERATE_RET tuya_iot_media_data_report(IN CONST FLOW_BODY_ST *dt_body, IN CONST UINT_T timeout);

/**
 * @brief tuya_iot_media_data_report_v2
 *
 * @param[in] dt_body: media data version 2
 * @param[in] timeout: need report time
 *
 * @return OPERATE_RET
 *
 * @waring please dont use tuya_iot_media_data_report and tuya_iot_media_data_report_v2 in one
 * application
 */
OPERATE_RET tuya_iot_media_data_report_v2(IN CONST FLOW_BODY_V2_ST *dt_body,
                                          IN CONST UINT_T timeout);

#define tuya_iot_upload_layout_buffer(map_id, buffer, len)                                         \
	tuya_iot_map_cleaner_upload_buffer(map_id, buffer, len, "layout/lay.bin", UP_CLEANER_MAP)
#define tuya_iot_upload_route_buffer(map_id, buffer, len)                                          \
	tuya_iot_map_cleaner_upload_buffer(map_id, buffer, len, "route/rou.bin", UP_CLEANER_PATH)

#define tuya_iot_upload_layout_file(map_id, local_file_name)                                       \
	tuya_iot_map_cleaner_upload_file(map_id, local_file_name, "layout/lay.bin", UP_CLEANER_MAP)
#define tuya_iot_upload_route_file(map_id, local_file_name)                                        \
	tuya_iot_map_cleaner_upload_file(map_id, local_file_name, "route/rou.bin", UP_CLEANER_PATH)

/**
 * @brief tuya_iot_map_cleaner_upload_buffer
 * @desc sweeper function. upload cleaner map info
 *
 * @param[in] map_id
 * @param[in] buffer
 * @param[in] len
 * @param[in] cloud_file_name
 * @param[in] map_type
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_map_cleaner_upload_buffer(IN CONST INT_T map_id, IN CONST BYTE_T *buffer,
                                               IN CONST UINT_T len,
                                               IN CONST CHAR_T *cloud_file_name,
                                               IN CONST UP_MAP_TYPE_E map_type);

/**
 * @brief tuya_iot_map_cleaner_upload_file
 * @desc sweeper function. upload cleaner map info
 *
 * @param[in] map_id
 * @param[in] local_file_name
 * @param[in] cloud_file_name
 * @param[in] map_type
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_map_cleaner_upload_file(IN CONST INT_T map_id,
                                             IN CONST CHAR_T *local_file_name,
                                             IN CONST CHAR_T *cloud_file_name,
                                             IN CONST UP_MAP_TYPE_E map_type);

/**
 * @brief tuya_iot_map_record_upload_buffer
 * @desc sweeper function. upload record map info
 *
 * @param[in] map_id
 * @param[in] buffer
 * @param[in] len
 * @param[in] descript
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_map_record_upload_buffer(IN CONST INT_T map_id, IN CONST BYTE_T *buffer,
                                              IN CONST UINT_T len, IN CONST CHAR_T *descript);

/**
 * @brief tuya_iot_map_record_upload_buffer_with_filename
 * @desc sweeper function. upload record map info
 *
 * @param[in] map_id
 * @param[in] buffer
 * @param[in] len
 * @param[in] cloud_file_name
 * @param[in] descript
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_map_record_upload_buffer_with_filename(IN CONST INT_T map_id,
                                                            IN CONST BYTE_T *buffer,
                                                            IN CONST UINT_T len,
                                                            IN CONST CHAR_T *cloud_file_name,
                                                            IN CONST CHAR_T *descript);

/**
 * @brief tuya_iot_get_gw_id
 *
 * @return device id
 */
CHAR_T *tuya_iot_get_gw_id(VOID);

/**
 * @brief tuya_iot_get_region_info
 *
 * @param[in/out] p_region_info
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_get_region_info(INOUT TY_IOT_REGION_INFO_S *p_region_info);

/**
 * @brief tuya_iot_get_custom_cfg
 *
 * @param[in] type
 * @param[out] pp_cfg_str
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_get_custom_cfg(IN CONST TY_CUSTOM_CFG_E type, OUT CHAR_T **pp_cfg_str);

/**
 * @brief tuya_iot_release_custom_cfg
 *
 * @param[in] p_cfg_str
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_release_custom_cfg(IN CHAR_T *p_cfg_str);

/**
 * @brief tuya_iot_get_location_info
 *
 * @param[in] p_location
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_get_location_info(INOUT TY_LOCATION_INFO_S *p_location);

/**
 * @brief tuya_iot_set_log_attr
 *
 * @param[in] log_level
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_set_log_attr(IN CONST INT_T log_level);

/**
 * @brief tuya_iot_set_log_bool_time
 *
 * @param[in] if_ms_level
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_set_log_bool_time(BOOL_T if_ms_level);

/**
 * @brief tuya_iot_custom_data_report_sync
 *
 * @param[in] data
 * @param[in] len
 * @param[in] topic
 * @param[in] qos
 * @param[in] timeout
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_custom_data_report_sync(IN CONST BYTE_T *data, IN CONST INT_T len,
                                             IN CONST CHAR_T *topic, IN CONST BYTE_T qos,
                                             IN CONST UINT_T timeout);

/**
 * @brief tuya_iot_custom_data_report_async
 *
 * @param[in] data
 * @param[in] len
 * @param[in] topic
 * @param[in] qos
 * @param[in] timeout
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_iot_custom_data_report_async(IN CONST BYTE_T *data, IN CONST INT_T len,
                                              IN CONST CHAR_T *topic, IN CONST BYTE_T qos,
                                              IN CONST UINT_T timeout);

/**
 * @brief tuya_iot_get_active_stat
 *
 * @return
 */
BYTE_T tuya_iot_get_active_stat(VOID);

/**
 * @brief tuya_iot_app_cbs_init
 *
 * @param[in] app_cbs
 *
 * @return
 */
VOID tuya_iot_app_cbs_init(IN CONST TY_IOT_APP_CBS_S *app_cbs);

/**
 * @brief tuya_iot_dp_low_power_query
 *
 * @param[in] dps: data of DP
 * @param[in] cnt: count of DP
 * @param[out] obj_dps: get object type DP
 * @param[out] raw_dps: get raw type DP
 *
 * @return
 */
OPERATE_RET tuya_iot_dp_low_power_query(IN CONST UCHAR_T *dps, IN CONST UINT_T cnt,
                                        OUT TY_RECV_OBJ_DP_S **obj_dps,
                                        OUT TY_RECV_RAW_DP_S **raw_dps);

/**
 * @brief according url get third cloud ca
 *
 * @param[in] url Third cloud url
 *
 * @return
 */
void tuya_iot_get_third_cloud_ca(CHAR_T *p_url);

#ifdef __cplusplus
}
#endif

#endif
