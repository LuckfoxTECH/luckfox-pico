#ifndef UPLOAD_NOTIFICATION_H
#define UPLOAD_NOTIFICATION_H

#include "cloud_operation.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

typedef PVOID_T Notify_Handler;

typedef enum {
	UPLOAD_NOTIFICATION, // for ipc log
	UPLOAD_COMMON,       // for sweep-rebot
	UPLOAD_EVENT_UNIFY,
	UPLOAD_INVALID
} UPLOAD_CFG_E;

VOID notification_message_free(IN Notify_Handler message);

OPERATE_RET upload_notification_init(VOID);

OPERATE_RET notification_message_malloc(IN INT_T count, OUT Notify_Handler *message,
                                        uint32_t *size);

OPERATE_RET notification_content_upload_from_buffer(UPLOAD_CFG_E cfg_type, IN TIME_T current_time,
                                                    IN CHAR_T *suffix, IN CHAR_T *data,
                                                    IN INT_T data_len, IN BOOL_T encryptNeeded,
                                                    OUT VOID *message);

OPERATE_RET notification_content_upload_from_custom_ctx(UPLOAD_CFG_E cfg_type,
                                                        IN CHAR_T *content_type,
                                                        IN CHAR_T *cloud_name,
                                                        IN CLOUD_UPLOAD_PATH_E path,
                                                        IN CLOUD_CUSTOM_CONTENT_CTX_S *p_ctx,
                                                        INOUT Notify_Handler message);

OPERATE_RET notification_content_upload_from_file(UPLOAD_CFG_E cfg_type, IN CHAR_T *local_name,
                                                  IN CHAR_T *content_type, IN CHAR_T *cloud_name,
                                                  IN CLOUD_UPLOAD_PATH_E path,
                                                  INOUT Notify_Handler message);

OPERATE_RET notification_message_upload(IN CONST BYTE_T mqtt_number, INOUT Notify_Handler message,
                                        IN UINT_T time_out);

#if 0
OPERATE_RET upload_recordFile(IN INT_T map_id, IN CHAR_T *layout_name, IN CHAR_T *route_name);
#endif

OPERATE_RET upload_map_custom_buffer(IN CONST INT_T map_id, IN CONST BYTE_T *buf,
                                     IN CONST UINT_T len, IN CONST CHAR_T *cloud_file_name,
                                     IN CONST INT_T map_type, IN CONST BOOL_T send_mqtt,
                                     IN CONST CHAR_T *extend_msg, IN CONST BOOL_T http_update);

OPERATE_RET upload_map_custom_file(IN CONST INT_T map_id, IN CONST CHAR_T *local_name,
                                   IN CONST CHAR_T *cloud_file_name, IN CONST INT_T map_type,
                                   IN CONST BOOL_T send_mqtt, IN CONST CHAR_T *extend_msg,
                                   IN CONST BOOL_T http_update);

//废弃功能
OPERATE_RET notification_snapshot_message_upload(IN CONST BYTE_T snapshot,
                                                 INOUT Notify_Handler message, IN UINT_T time_out);

INT_T notification_check_init(UPLOAD_CFG_E cfg_type);

OPERATE_RET notification_content_upload_from_buffer_unify(IN UPLOAD_CFG_E cfg_type,
                                                          IN TIME_T current_time, IN CHAR_T *suffix,
                                                          IN CHAR_T *data, IN INT_T data_len,
                                                          IN CHAR_T *secret_key, OUT VOID *message);

OPERATE_RET notification_message_upload_for_event_unify(
    IN CONST BYTE_T notification, IN VOID *message, IN UINT_T time_out, IN CHAR_T *notify_cmd,
    IN CHAR_T *notify_with, IN CHAR_T *notify_type, IN CHAR_T *extra_data, IN BOOL_T is_alarm,
    IN TIME_T start_time);

OPERATE_RET notification_content_upload_from_buffer_generic(IN TIME_T current_time,
                                                            IN CHAR_T *suffix, IN CHAR_T *data,
                                                            IN INT_T data_len,
                                                            IN CHAR_T *secret_key,
                                                            OUT VOID *message);

OPERATE_RET notification_message_upload_generic(IN CONST BYTE_T notification, IN VOID *message,
                                                IN UINT_T time_out, IN CHAR_T *notify_cmd,
                                                IN CHAR_T *notify_with, IN CHAR_T *notify_type,
                                                IN CHAR_T *extra_data);

#endif // UPLOAD_NOTIFICATION_H
