#ifndef STORAGE_SERVICE_H
#define STORAGE_SERVICE_H

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_api.h"

OPERATE_RET cloud_storage_upload(IN CHAR *name, IN CHAR *content_type, IN CHAR *data,
                                 IN INT data_len);
OPERATE_RET cloud_storage_m3u8_update(IN CHAR *start_time, IN CHAR *last_ts_name);
OPERATE_RET cloud_storage_upload_append(IN CHAR *name, IN CHAR *content_type, IN CHAR *data,
                                        IN INT data_len, IN INT position);
OPERATE_RET cloud_storage_check_config(VOID);

#endif // STORAGE_SERVICE_H
