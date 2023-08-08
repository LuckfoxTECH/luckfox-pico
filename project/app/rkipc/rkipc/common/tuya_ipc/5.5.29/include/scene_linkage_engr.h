/***********************************************************
 *  File: scene_linkage_engr.h
 *  Author: nzy
 *  Date: 20180723
 ***********************************************************/
#ifndef __SCENE_LINKAGE_ENGR_H
#define __SCENE_LINKAGE_ENGR_H

#include "scene_linkage.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SCENE_LINKAGE_ENGR_GLOBALS
#define __SCENE_LINKAGE_ENGR_EXT
#else
#define __SCENE_LINKAGE_ENGR_EXT extern
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
/***********************************************************
 *  Function: engr_scene_linkage_module_init
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 *  Note:
 ***********************************************************/
__SCENE_LINKAGE_ENGR_EXT
OPERATE_RET engr_scene_linkage_module_init(VOID);

/***********************************************************
 *  Function: scene_linkage_module_uninit
 *  Input: none
 *  Output: none
 *  Return: none
 *  Note:
 ***********************************************************/
__SCENE_LINKAGE_ENGR_EXT
VOID engr_scene_linkage_module_uninit(VOID);

/***********************************************************
 *  Function: engr_dp_condition_detect
 *  Input: id
 *  Input: dp_cmd
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
// Note:
// id:means gwid,if(cid == NULL) then gwid is actual cid
// dp_cmd:{"cid":"xxxxxx",dps:{"1":2,"2":"111"}} or {"devid":"xxxxxx",dps:{"1":2,"2":"111"}}
__SCENE_LINKAGE_ENGR_EXT
OPERATE_RET engr_dp_condition_detect(IN CONST CHAR_T *id, IN CONST CHAR_T *dp_cmd,
                                     IN CONST CHAR_T *gw_id, IN LINKAGE_RULE_TYPE_T rule_type);

__SCENE_LINKAGE_ENGR_EXT
OPERATE_RET engr_scene_linkage_module_reinit(VOID);

__SCENE_LINKAGE_ENGR_EXT
VOID engr_reset_scene_info_to_disk(VOID);

__SCENE_LINKAGE_ENGR_EXT
BOOL_T engr_check_modify_time_changed(INT_T input_last_mod_time);

__SCENE_LINKAGE_ENGR_EXT
VOID engr_load_scene_info_from_disk(VOID);

__SCENE_LINKAGE_ENGR_EXT
OPERATE_RET engr_save_scene_link(INT_T total_cnt, INT_T sl_last_mod_time, ty_cJSON *rules);

#ifdef __cplusplus
}
#endif
#endif
