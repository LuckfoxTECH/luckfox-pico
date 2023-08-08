#ifndef _IPC_HTTPC_H
#define _IPC_HTTPC_H

#include "cJSON.h"
#include "gw_intf.h"
#include "tuya_cloud_types.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTPC_NULL_CHECK(pointer)                                                                  \
	if (pointer == NULL) {                                                                         \
		PR_ERR("%s is null", #pointer);                                                            \
		return OPRT_INVALID_PARM;                                                                  \
	}

OPERATE_RET ipc_printf_log_up(IN CONST PCHAR_T pFile, IN CONST INT_T line, IN CONST PCHAR_T pFunc,
                              IN CONST unsigned char log_type, IN CONST PCHAR_T pFmt, ...);

#define tuya_ipc_log 1
#define tuya_p2p_log 2

#ifdef __LITEOS__
#define IPC_PRINT_LOG_UP(logo_type, fmt, ...)
#else

#define IPC_PRINT_LOG_UP(logo_type, fmt, ...)                                                      \
	do {                                                                                           \
		ipc_printf_log_up(__FILE__, __LINE__, (PCHAR_T) __func__, (unsigned char)logo_type, fmt,   \
		                  ##__VA_ARGS__);                                                          \
	} while (0)
#endif

#define IPC_PR_UPLOAD(fmt, ...) IPC_PRINT_LOG_UP(tuya_ipc_log, fmt, ##__VA_ARGS__)

/*
NULL

{\"withVideoDataStorage\":\"true\"}
*/
OPERATE_RET httpc_ipc_oss_storage_cfg_get_v10(IN CONST CHAR_T *gw_id, IN CONST CHAR_T *param,
                                              OUT cJSON **result);
OPERATE_RET httpc_order_request_errcode_transfer(char *ErroValu);

/*
0:start
1:stop
2:event
*/
OPERATE_RET httpc_ipc_cloud_storage_event_notify_v10(IN CONST CHAR_T *gw_id,
                                                     IN CONST INT_T cloud_type,
                                                     IN CONST CHAR_T *cloud_msg,
                                                     OUT cJSON **result);

/*
NULL

{\"withVideoDataStorage\":\"true\"}
*/
OPERATE_RET httpc_ipc_oss_storage_cfg_get(IN CONST CHAR_T *param, OUT cJSON **result);

/* 0:start 1:stop 2:event */
OPERATE_RET httpc_ipc_cloud_storage_event_notify(IN CONST INT_T cloud_type,
                                                 IN CONST CHAR_T *cloud_msg, OUT cJSON **result);

/**** 自定义的HTTP(S)交互请求
api_name: 与服务端约定的接口名
api_version: 接口使用的版本，如 1.0 2.0
message: 接口中需要设备上报给服务端的数据内容，一版为JSON格式字符串
result: 服务端返回的消息，会在该函数内部动态申请空间，需要在外部处理完result之后调用Free接口释放内存
****/
OPERATE_RET http_gw_ipc_custom_msg(IN CONST CHAR_T *api_name, IN CONST CHAR_T *api_version,
                                   IN CONST CHAR_T *message, OUT cJSON **result);

VOID tuya_ipc_log_upload_init(void);

#if defined(ENABLE_CLOUD_OPERATION) && (ENABLE_CLOUD_OPERATION == 1)
/*direct call*/
OPERATE_RET httpc_domain_certs_get(OUT ty_cJSON **result, IN CONST CHAR_T *url_msg);
/*direct call*/
OPERATE_RET httpc_cloud_storage_cfg_get(OUT ty_cJSON **result, IN CONST BOOL_T withvideostorage);
/*direct call*/
OPERATE_RET httpc_cloud_storage_common_cfg_get(OUT ty_cJSON **result);
/*direct call*/
OPERATE_RET httpc_cloud_upload_complete(IN CONST UINT_T time, IN CONST CHAR_T *p_filetype,
                                        IN CONST CHAR_T *p_extend, IN CONST CHAR_T *p_bucket,
                                        IN CONST CHAR_T *p_file);
#endif

OPERATE_RET httpc_cloud_event_sepc_get(OUT cJSON **result);

OPERATE_RET httpc_restriction_cfg_get(OUT cJSON **result);

OPERATE_RET http_device_update_skill(IN CONST CHAR_T *dev_id, IN CONST CHAR_T *skill);

#ifdef __cplusplus
}
#endif
#endif
