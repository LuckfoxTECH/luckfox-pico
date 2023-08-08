#ifndef __THING_CONFIG_H__
#define __THING_CONFIG_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef OPERATE_RET (*THING_CONFIG_SOCKET_CB)(IN CONST INT_T socket);

OPERATE_RET thing_config_init(IN THING_CONFIG_SOCKET_CB socket_cb);
OPERATE_RET thing_config_start(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                               IN CONST CHAR_T *token, IN CONST UINT_T timeout);
VOID thing_config_stop(VOID);
OPERATE_RET thing_config_mqc_handle(IN ty_cJSON *root_json);

#ifdef __cplusplus
}
#endif

#endif
