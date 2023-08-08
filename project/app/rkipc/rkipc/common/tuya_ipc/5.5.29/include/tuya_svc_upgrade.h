/**
 * @file tuya_svc_upgrade.h
 * @brief TUYA OTA service
 * @version 0.1
 * @date 2019-08-28
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_SVC_UPGRADE_H__
#define __TUYA_SVC_UPGRADE_H__

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef BYTE_T TI_UPGRD_STAT_S;
#define TUS_RD 1         // ready
#define TUS_UPGRDING 2   // upgrading
#define TUS_UPGRD_FINI 3 // finish
#define TUS_UPGRD_EXEC 4 // error

#define TUS_DOWNLOAD_START 10    // download start
#define TUS_DOWNLOAD_COMPLETE 11 // download complete
#define TUS_UPGRADE_START 12     // upgrade start
#define TUS_UPGRADE_SUCCESS 3    // upgrade success

#define TUS_DOWNLOAD_ERROR_UNKONW 40             // unknown error
#define TUS_DOWNLOAD_ERROR_LOW_BATTERY 41        // low battery
#define TUS_DOWNLOAD_ERROR_STORAGE_NOT_ENOUGH 42 // storage not enough
#define TUS_DOWNLOAD_ERROR_MALLOC_FAIL 43        // malloc fail
#define TUS_DOWNLOAD_ERROR_TIMEOUT 44            // download timeout
#define TUS_DOWNLOAD_ERROR_HMAC 45               // hmac not match
#define TUS_UPGRADE_ERROR_LOW_BATTERY 46         // low battery
#define TUS_UPGRADE_ERROR_MALLOC_FAIL 47         // malloc fail
#define TUS_UPGRADE_ERROR_VERSION 48             // version invalid
#define TUS_UPGRADE_ERROR_HMAC 49                // hmac not match

/**
 * @brief Definition of firmware upgrade workload
 */
typedef struct {
	/*
	 * @param[out] dev_id device id for gw or nodeId for subdev
	 * @return OPERATE_RET OPRT_OK indicates a valid device need detect
	 */
	OPERATE_RET (*upgrade_get_dev)(OUT CHAR_T dev_id[DEV_ID_LEN + 1]);
	/*
	* @param[in] is_err TRUE indicates error happen, detect process is over
	* @param[in] is_exe valid when !is_err
	                TRUE indicates upgrade is got and notified
	                FALSE indicates upgrade is got but execute time not arrival
	* @param[in] detect_interval valid when !is_err && !is_exe
	                         upgrade should be detected again after detect_interval(s)
	*/
	VOID (*upgrade_detect_result)(IN BOOL_T is_err, IN BOOL_T is_exe, IN UINT_T detect_interval);
} tuya_upgrade_detect_t;

/**
 * @brief Handler of GW upgrade inform
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef int (*dev_upgrade_inform_cb)(const FW_UG_S *fw);

/**
 * @brief Handler of pre-process inform
 */
typedef VOID (*dev_upgrade_pre_inform_cb)(BOOL_T *handled, const FW_UG_S *fw);

/**
 * @brief Handler of sub-device upgrade inform
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef int (*subdev_upgrade_inform_cb)(const char *dev_id, const FW_UG_S *fw);

/**
 * @brief Handler of process inform for some upgrade type
 *
 * @return OPRT_OK on had handled. Others should process by svc_upgrade
 */
typedef int (*dev_upgrade_mq15_inform_cb)(ty_cJSON *root_json);

/**
 * @brief Definition of firmware upgrade handler
 */
typedef struct {
	/** Handler of gw upgrade inform */
	dev_upgrade_inform_cb dev_upgrade_cb;
	/** Handler of sub-device upgrade inform */
	subdev_upgrade_inform_cb subdev_upgrade_cb;
	/** Handler of gw upgrade pre-precess */
	dev_upgrade_inform_cb pre_dev_upgrade_cb;
	/** Handler of sub-device pre-precess */
	subdev_upgrade_inform_cb pre_subdev_upgrade_cb;
} tuya_upgrade_param_t, *tuya_upgrade_param_p;

/**
 * @brief Initilize firmware upgrade framework
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_svc_upgrade_init(tuya_upgrade_param_t *ota_param);

/**
 * @brief Run firmware upgrade framework, workloads registered will be executed
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_svc_upgrade_active(void);

/**
 * @brief Register a new firmware upgrade workload
 *
 * @param[in] detect The workload, see tuya_upgrade_detect_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_upgrade_detect_register(tuya_upgrade_detect_t *detect);

/**
 * @brief Register pre-precess handler to replace the default one
 *
 * @param[in] pre_ug_cb The new handler
 */
void tuya_svc_upgrade_register_pre_cb(dev_upgrade_pre_inform_cb pre_ug_cb);

/**
 * @brief Register process handler for some upgrade type
 *
 * @param[in] mq15_cb The process handler
 */
void tuya_svc_upgrade_register_mq15_cb(dev_upgrade_mq15_inform_cb mq15_cb);

/**
 * @brief Reset firmware upgrade check interval
 *
 * @param[in] upgrade_interval Next interval to check(unit sec)
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_svc_upgrade_detect_reset(const int upgrade_interval);

/**
 * @brief Refuse to download the specific firmware
 *
 * @param[in] dev_id ID of sub-device, or NULL for GW
 * @param[in] fw Firmware info to download
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_svc_upgrade_refuse(const FW_UG_S *fw, const char *dev_id);

/**
 * @brief Sync dowload result to cloud
 *
 * @param[in] dev_id ID of sub-device, or NULL for GW
 * @param[in] tp OTA channel, see DEV_TYPE_T
 * @param[in] result See TI_UPGRD_STAT_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_svc_upgrade_result_report(const char *dev_id, const DEV_TYPE_T type, const int result);

/**
 * @brief Start to download the specific firmware
 *
 * @param[in] dev_id ID of sub-device, or NULL for GW
 * @param[in] fw Firmware info to download
 * @param[in] get_file_cb Callback to transfer data while downloading
 * @param[in] pri_data Private data
 * @param[in] upgrd_nofity_cb Callback when upgrade complete or error
 * @param[in] upload_upgrade_percent Upload upgrade percent or not
 * @param[in] download_buf_size Buffer size allocated for OTA
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_svc_upgrade_start(const CHAR_T *dev_id, const FW_UG_S *fw,
                           const GET_FILE_DATA_CB get_file_cb, VOID *pri_data,
                           const UPGRADE_NOTIFY_CB upgrd_nofity_cb,
                           const BOOL_T upload_upgrade_percent, const UINT_T download_buf_size);

/**
 * @brief Notify to upgrade immediately like mqtt15
 *
 * @param[in] dev_id ID of sub-device, or NULL for GW
 * @param[in] dev type to upgrade
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_upgrade_mqtt_notify(CHAR_T *dev_id, DEV_TYPE_T tp);

OPERATE_RET tuya_svc_upgrade_ug_info_parse(ty_cJSON *result, UPGRADE_TYPE_T type, FW_UG_S *ug_info);

#ifdef __cplusplus
}
#endif

#endif
