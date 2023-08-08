#ifndef __EVENT_CLOUD_H__
#define __EVENT_CLOUD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

#include "cloud_operation.h"
#include "ipc_httpc.h"
#include "tuya_ipc_api.h"
#include "ty_cJSON.h"
#include "uni_log.h"

OPERATE_RET cloud_get_unify_storage_config(INOUT CLOUD_CONFIG_S *config);
OPERATE_RET httpc_cloud_event_sepc_get(OUT cJSON **result);

#ifdef __cplusplus
}
#endif

#endif
