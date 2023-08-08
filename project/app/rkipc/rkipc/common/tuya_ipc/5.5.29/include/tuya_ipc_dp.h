/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_dp.h
 **********************************************************************************/

#ifndef _TUYA_IPC_DP_H
#define _TUYA_IPC_DP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_iot_config.h"
#include <stdio.h>
#include <time.h>
#if defined(WIFI_GW) && (WIFI_GW == 1)
#include "tuya_cloud_wifi_defs.h"
#endif
#include "cJSON.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

/**
 * \fn OOPERATE_RET tuya_ipc_dp_report(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E
 * type, IN VOID * pVal, IN CONST UINT_T cnt) \brief report dp(data point) to tuya cloud \return
 * OPERATE_RET
 */
OPERATE_RET tuya_ipc_dp_report(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E type,
                               IN VOID *pVal, IN CONST UINT_T cnt);

/**
 * \fn OPERATE_RET tuya_ipc_dp_report_sync(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id,IN DP_PROP_TP_E
 * type,IN VOID * pVal,IN CONST UINT_T cnt, IN CONST UINT_T timeout) \brief report dp to tuya cloud
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_dp_report_sync(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id, IN DP_PROP_TP_E type,
                                    IN VOID *pVal, IN CONST UINT_T cnt, IN CONST UINT_T timeout);

OPERATE_RET tuya_ipc_dp_report_sync_with_time(IN CONST CHAR_T *dev_id, IN BYTE_T dp_id,
                                              IN DP_PROP_TP_E type, IN VOID *pVal,
                                              IN CONST UINT_T cnt, IN CONST UINT_T timeout,
                                              UINT_T time);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_DP_H*/
