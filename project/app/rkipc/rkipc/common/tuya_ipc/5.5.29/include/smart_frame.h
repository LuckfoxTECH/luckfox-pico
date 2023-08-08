/**
 * @file smart_frame.h
 * @author nzy@tuya.com
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _SMART_FRAME_H
#define _SMART_FRAME_H

#include "gw_intf.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief info of dp command
 */
typedef struct {
	DP_CMD_TYPE_E tp; // command source
	ty_cJSON *cmd_js; // command content
} SF_GW_DEV_CMD_S;

/**
 * @brief info of dp message data
 */
typedef struct msg_data_s {
	UINT_T serno;   // serial number
	UINT_T len;     // message data lenth
	BYTE_T data[0]; // message data
} MSG_DATA_S;

/**
 * @brief Smart frame init
 *
 * @param VOID
 *
 * @note This API is used for initializing the resource of smart frame
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET smart_frame_init(VOID);

/**
 * @brief Send dp command
 *
 * @param[in] gd_cmd: dp command information
 *
 * @note This API is used for sending dp command from mqtt/lan/bt
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_send_gw_dev_cmd(IN SF_GW_DEV_CMD_S *gd_cmd);

/**
 * @brief Start to synchronize dp data
 *
 * @param VOID
 *
 * @note This API is used to start a timer task to synchronize dp data to the cloud/app
 *
 * @return VOID
 */
VOID sf_start_sync_obj_dp(VOID);

/**
 * @brief Packed the local dp data as string in json format
 *
 * @param[in] dev_cntl: device control handle
 * @param[in] max_len: max lenth of the dp string
 * @param[in] addDevId: if add devid or not
 * @param[out] p_all_data_packed: if all dp is packed or not
 * @param[in] reset_flow_ctl: if need reset flow control or not
 * @param[in] is_lan: if be lan or not
 *
 * @note This API is used for packing the local dp data as string in json format
 *
 * @return the dp string in json format
 */
CHAR_T *sf_pack_local_obj_dp_data(IN DEV_CNTL_N_S *dev_cntl, IN CONST UINT_T max_len,
                                  IN CONST BOOL_T addDevId, OUT BOOL_T *p_all_data_packed,
                                  BOOL_T reset_flow_ctl, IN CONST BOOL_T is_lan);

/**
 * @brief Get serial number for dp report
 *
 * @param VOID
 *
 * @note This API is used for getting serial number for dp report
 *
 * @return the serial number
 */
UINT_T sf_get_serial_no(VOID);

/**
 * @brief Response for dp query
 *
 * @param[in] pCidArr: cid in json format
 * @param[in] pDpIdArr: dp id in json format
 *
 * @note This API is used for response for dp query
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_respone_obj_dp_query(IN CONST ty_cJSON *pCidArr, IN CONST ty_cJSON *pDpIdArr);

/**
 * @brief Get dp control information
 *
 * @param[in] id: device id
 * @param[in] dp_id: dp id
 *
 * @note This API is used for getting dp control information by device id and dp id.
 *
 * @return the dp control information
 */
DP_CNTL_S *sf_dp_data_get(IN CHAR_T *id, IN CONST CHAR_T dp_id);

/**
 * @brief Check the dp data is same
 *
 * @param[in] dp_cmd: dp information in josn format
 *
 * @note This API is used for checking the dp data is same.
 *
 * @return the result whether the dp data is same.
 */
BOOL_T sf_dp_data_is_equl(IN ty_cJSON *dp_cmd);

// special dp callback function
typedef OPERATE_RET (*sf_special_dp_cb)(IN CONST UINT16_T dpid, IN CONST ty_cJSON *dp_obj);

/**
 * @brief Register special dp callback function
 *
 * @param[in] handler: special dp callback function
 *
 * @note This API is used to register special dp callback function.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_special_dp_register_cb(IN sf_special_dp_cb handler);

/**
 * @brief Query dp for low power
 *
 * @param[in] dps: dp string
 * @param[in] cnt: dp count
 * @param[out] obj_dps: dp in object format
 * @param[out] raw_dps: dp in raw format
 *
 * @note This API is used for quering dp for low power
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_dp_low_power_query(IN CONST UCHAR_T *dps, IN CONST UINT_T cnt,
                                  OUT TY_RECV_OBJ_DP_S **obj_dps, OUT TY_RECV_RAW_DP_S **raw_dps);

// callback function by different kind of transport type
typedef OPERATE_RET (*sf_trans_type_cb)(IN CONST UINT16_T dpid, IN CONST ty_cJSON *data_js);

/**
 * @brief Register callback function for different kind of transport type
 *
 * @param[in] trans: transport type
 * @param[in] handler: callback function
 *
 * @note This API is used for registering callback function for different kind of transport type
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_trans_type_register_cb(IN CONST DP_TRANS_TYPE_T trans, IN sf_trans_type_cb handler);

/**
 * @brief Set enable switch for deleting null dp
 *
 * @param[in] enable: enable switch
 *
 * @note This API is used for setting enable switch for deleting null dp.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_dp_set_delete_null_dp(BOOL_T enable);

/**
 * @brief Set default dp report timeout
 *
 * @param[in] timeout_s: in second
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_dp_set_report_timeout(UINT_T timeout_s);

// callback function for subdevice
typedef OPERATE_RET (*subdev_dev_cntl_cb)(IN DEV_CNTL_N_S *subdev_cntl, VOID *param);
// callback function for traversal subdevice
typedef OPERATE_RET (*sf_subdev_dev_cntl_traversal_cb)(subdev_dev_cntl_cb, VOID *param);

// callback function for checking whether the subdevice needs to execute the reliable trasmission
typedef BOOL_T (*sf_subdev_reliable_transfer_check_cb)(IN CHAR_T *id);

// callback function for getting the time of subdevice's reliable trasmission
typedef UINT_T (*sf_subdev_reliable_transfer_time_get_cb)(IN CHAR_T *id);

// callback function for checking whether the subdevice nedds report
typedef BOOL_T (*sf_subdev_report_check_cb)(IN CHAR_T *id);

/**
 * @brief callback function of subdevice handle
 */
typedef struct {
	/** callback function for traversal subdevice */
	sf_subdev_dev_cntl_traversal_cb traversal_cb;
	/** callback function for checking whether the subdevice needs to execute the reliable
	 * trasmission */
	sf_subdev_reliable_transfer_check_cb rt_check_cb;
	/** callback function for getting the time of subdevice's reliable trasmission */
	sf_subdev_reliable_transfer_time_get_cb rt_time_get_cb;
	/** callback function for checking whether the subdevice nedds report */
	sf_subdev_report_check_cb rpt_check_cb;
} SF_SUBDEV_HANDLE_CB_S;

/**
 * @brief Register callback function for subdevice
 *
 * @param[in] cbs: callback functions
 *
 * @note This API is used for registering  callback function for subdevice
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_subdev_register_cb(IN SF_SUBDEV_HANDLE_CB_S *cbs);

/**
 * @brief dp data information
 */
typedef struct {
	CHAR_T *dev_id;    // device id
	TY_OBJ_DP_S *data; // dp data
	UINT_T cnt;        // dp count
} TY_OBJ_DP_DATA_S;

/**
 * @brief dp data report information for object type
 */
typedef struct {
	TY_OBJ_DP_DATA_S obj_dp; // dp data information
	BOOL_T force_send;       // if report dp forcely or not
} TY_OBJ_DP_REPT_S;

/**
 * @brief dp data report information for statistic type
 */
typedef struct {
	TY_OBJ_DP_DATA_S obj_dp; // dp data information
	UINT_T timeout;          // report timeout
	BOOL_T auto_retrans;     // if re-report or not
} TY_STAT_DP_REPT_S;

/**
 * @brief dp data report information for raw type
 */
typedef struct {
	CHAR_T *dev_id;      // device id
	BYTE_T dpid;         // dp id
	BYTE_T *data;        // dp data
	UINT_T len;          // dp data lenth
	CHAR_T *time_str;    // dp report time
	UINT_T timeout;      // report timeout
	BOOL_T auto_retrans; // if re-report or not
} TY_RAW_DP_REPT_S;

/**
 * @brief Print the key time for dp report
 *
 * @param VOID
 *
 * @note This API is used to print the key time for dp report
 *
 * @return VOID
 */
VOID_T ty_print_dp_time(VOID_T);

/**
 * @brief Register callback function for dp report
 *
 * @param[in] dp_rept_cbs: callback functions
 *
 * @note This API is used for registering callback function for dp report.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET sf_regist_dp_rept_cb(IN DP_REPT_HADLE_CB_S *dp_rept_cbs);

/**
 * @brief Report dp data
 *
 * @param[in] dp_rept_type: dp report type
 * @param[in] dp_rept: dp report data
 * @param[in] is_query: if query interface or not
 *
 * @note This API is used for reporting dp data.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_dp_report(IN CONST DP_REPT_TYPE_E dp_rept_type, IN VOID_T *dp_rept,
                         IN CONST BOOL_T is_query);

/**
 * @brief ty_dp_json_to_obj
 * @desc transfer json format dp to object
 *
 * @param[in] dev_id: if sub-device, then devid = sub-device_id
 *                if gateway/soc/mcu, then devid = NULL
 * @param[in] json_str: string in json format
 * @param[OUT] dp_data: dp array header
 * @param[OUT] cnt: dp array count
 *
 * @note This API is used to transfer json format dp to object.
 * Memory of dp_data need to be freed.
 *
 * @return OPRT_OK: success  Other: fail
 */
OPERATE_RET ty_get_obj_dp_from_json(IN CHAR_T *dev_id, IN CONST CHAR_T *json_str,
                                    OUT TY_OBJ_DP_S **dp_data, OUT UINT_T *cnt);

/**
 * @brief ty_free_obj_dp_data
 * @desc free memory of object dp
 *
 * @param[in] dp_data: dp data
 * @param[in] cnt: dp count
 *
 * @note This API is used to free memory of object dp.
 *
 * @return VOID
 */
VOID_T ty_free_obj_dp_data(TY_OBJ_DP_S *dp_data, UINT_T cnt);

#ifdef __cplusplus
}
#endif
#endif
