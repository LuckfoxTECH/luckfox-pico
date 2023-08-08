#ifndef __LAN_DEV_BASE_H
#define __LAN_DEV_BASE_H

#include "lan_cluster_api.h"
#include "tuya_cloud_com_defs.h"

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

OPERATE_RET lan_dev_base_init(IN CONST CHAR_T *dev_id);

BOOL_T lan_dev_master_cap();

BOOL_T lan_scene_linkage_master_is_alive();

#ifdef __cplusplus
}
#endif
#endif
