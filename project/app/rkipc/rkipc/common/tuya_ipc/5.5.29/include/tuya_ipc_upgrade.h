/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_upgrade.h
 **********************************************************************************/

#ifndef _TUYA_IPC_UPGRADE_H
#define _TUYA_IPC_UPGRADE_H

#ifdef __cplusplus
extern "C" {
#endif
typedef int (*TUYA_IPC_SDK_DEV_UPGRADE_INFORM_CB)(IN CONST FW_UG_S *fw);
typedef struct {
	TUYA_IPC_SDK_DEV_UPGRADE_INFORM_CB upgrade_cb;

} TUYA_IPC_SDK_UPGRADE_S;

typedef struct {
	GW_ATTACH_ATTR_T *sub_dev_ota_info;
	UINT_T sub_dev_cnt;
} TUYA_IPC_SUB_DEV_OTA_INFO_S;

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_sdk(     IN CONST FW_UG_S *fw,\
 *                                     IN CONST GET_FILE_DATA_CB get_file_cb,\
 *                                     IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
 *                                     IN PVOID pri_data)
 * \brief OTA, upgrade via TUYA SDK
 * \param[in] fw firmware: infomation
 * \param[in] get_file_cb: callback function during downloading fw
 * \param[in] upgrd_nofity_cb: callback function when downloading fw fininsh
 * \param[in] pri_data: data transferred between callback functions
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_upgrade_sdk(IN CONST FW_UG_S *fw, IN CONST GET_FILE_DATA_CB get_file_cb,
                                 IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb, IN PVOID_T pri_data);

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_progress_report
 * \brief send a upgrade progress to tuya cloude and app
 * \param[in] percent: upgrade progress percent , valid value [0,100]
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_upgrade_progress_report(IN UINT_T percent);

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_progress_report_by_type
 * \brief send a upgrade progress to tuya cloude and app
 * \param[in] percent: upgrade progress percent , valid value [0,100]
 * \param[in] type: upgrade dev type , valid value [DEV_TYPE_T]
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_upgrade_progress_report_by_type(IN UINT_T percent, IN BYTE_T type);

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_progress_status
 * \brief report upgrade status to app
 *
 * \return return OPRT_OK
 */
OPERATE_RET tuya_ipc_upgrade_progress_status(IN CONST CHAR_T *devId, IN CONST BYTE_T tp,
                                             IN BYTE_T status);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_UPGRADE_H*/
