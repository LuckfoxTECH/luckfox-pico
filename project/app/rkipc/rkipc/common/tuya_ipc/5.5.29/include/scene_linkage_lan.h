#ifndef __SCENE_LINKAGE_LAN_H
#define __SCENE_LINKAGE_LAN_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/

//局域网联动模块初始化
OPERATE_RET lan_scene_linkage_init(IN CONST CHAR_T *dev_id);

// dp点变化时，检测局域网联动
OPERATE_RET lan_dp_condition_detect(IN CONST CHAR_T *id, IN CONST CHAR_T *dp_cmd);

// mq上下线
VOID lan_scene_linkage_on_mqtt_online(IN BOOL_T online);

#ifdef __cplusplus
}
#endif
#endif
