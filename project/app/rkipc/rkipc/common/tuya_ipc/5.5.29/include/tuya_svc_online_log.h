/**
 * @file tuya_svc_online_log.h
 * @author 愚者(maht@tuya.com)
 * @brief log服务应用接口描述文件
 * @version 0.1
 * @date 2019-08-29
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef __TUYA_SVC_ONLINE_LOG_H__
#define __TUYA_SVC_ONLINE_LOG_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief online log module initialization
 *
 * @param[in] p_env: the online log enviroment, used to report to cloud
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_online_log_init(const char *p_env);

/**
 * @brief online log module activate
 *
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_online_log_active(VOID);

/**
 * @brief upload user runtime status log
 *
 * @param[in] p_log: the log string
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_online_log_upload_runstat(const char *p_log);

/**
 * @brief upload user realtime log
 *
 * @param[in] p_log: the log string
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_online_log_upload_realtime(const char *p_log);

/**
 * @brief upload user sequnence log
 *
 * @param[in] p_log: custom sequnence log string
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_online_log_upload_custom_seq_log(const char *p_log);

#ifdef __cplusplus
}
#endif
#endif
