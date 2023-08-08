/***********************************************************
 *  File: scene_linkage.h
 *  Author: nzy
 *  Date: 20180723
 ***********************************************************/
#ifndef __SCENE_LINKAGE_H
#define __SCENE_LINKAGE_H

#include "action.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SCENE_LINKAGE_GLOBALS
#define __SCENE_LINKAGE_EXT
#else
#define __SCENE_LINKAGE_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef BYTE_T LINKAGE_RULE_TYPE_T;

//��https://wiki.tuya-inc.com:7799/pages/viewpage.action?pageId=22086410
#define LINKAGE_RULE_LOCAL 0
#define LINKAGE_RULE_LAN 1

#define RULE_TYPE_LOCAL_2_PROTOCOL(local) ((local) + 1)
#define RULE_TYPE_PROTOCOL_2_LOCAL(pro) ((pro)-1)

#ifdef ENABLE_LAN_LINKAGE_MASTER
#define LINKAGE_RULE_NUM 2
#else
#define LINKAGE_RULE_NUM 1
#endif

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
 *  Function: scene_linkage_module_init
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 *  Note:
 ***********************************************************/
__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_module_init(VOID);

/***********************************************************
 *  Function: scene_linkage_module_uninit
 *  Input: none
 *  Output: none
 *  Return: none
 *  Note:
 ***********************************************************/
__SCENE_LINKAGE_EXT
VOID scene_linkage_module_uninit(VOID);

__SCENE_LINKAGE_EXT
VOID scene_linkage_update_from_server(LINKAGE_RULE_TYPE_T linkage_type, INT_T delaymS);

#if defined(LOCAL_SCENE) && (LOCAL_SCENE == 1)
__SCENE_LINKAGE_EXT
VOID scene_linkage_scene_exe(CHAR_T *sceneId);

//如果场景在本地，就执行且返回OPRT_OK。
//否则返回不存在 OPRT_NOT_FOUND
__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_scene_exe_by_name(const CHAR_T *scene_name);
#endif
/***********************************************************
 *  Function: dp_condition_detect
 *  Input: id
 *  Input: dp_cmd
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
// Note:
// id:means gwid,if(cid == NULL) then gwid is actual cid
// dp_cmd:{"cid":"xxxxxx",dps:{"1":2,"2":"111"}} or {"devid":"xxxxxx",dps:{"1":2,"2":"111"}}
__SCENE_LINKAGE_EXT
OPERATE_RET dp_condition_detect(IN CONST CHAR_T *id, IN CONST CHAR_T *dp_cmd,
                                IN CONST CHAR_T *gw_id, IN LINKAGE_RULE_TYPE_T rule_type);

#if defined(ENABLE_LAN_LINKAGE_MASTER) && (ENABLE_LAN_LINKAGE_MASTER == 1)

typedef OPERATE_RET (*LAN_ACTION_SET_EXECUTE)(IN CONST CHAR_T *gw_id, IN CONST CHAR_T *dp_cmd,
                                              IN BOOL_T check, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_TOGGLE_ACTION_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_SCENE_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_LOCAL_SCENE_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_DPSTEP_ACTION_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_GROUP_ACTION_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);

__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_reg_lan_action(IN LAN_ACTION_SET_EXECUTE func);

__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_reg_lan_toggle_action(IN LAN_TOGGLE_ACTION_EXECUTE func);

__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_reg_lan_scene_action(LAN_SCENE_EXECUTE func);

__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_reg_lan_local_scene_action(LAN_LOCAL_SCENE_EXECUTE func);

__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_reg_lan_dpstep_action(IN LAN_DPSTEP_ACTION_EXECUTE func);

__SCENE_LINKAGE_EXT
OPERATE_RET scene_linkage_reg_lan_group_action(IN LAN_GROUP_ACTION_EXECUTE func);
#endif

#if defined(ENABLE_ALARM) && (ENABLE_ALARM == 1)
OPERATE_RET secne_linkage_parse_alarm(IN CHAR_T *alarmed_mode, OUT CHAR_T **id_list);
#endif

//如果条件或动作是网关本身的，cid为网关的虚拟ID，否则为子设备node_id。调用者需要拷贝内容
typedef struct {
	CHAR_T *cid;
	BYTE_T dp_id;
} SCENE_LINKAGE_ON_EXE_COND_S;

typedef struct {
	CHAR_T *cid;
	BYTE_T dp_id;
} SCENE_LINKAGE_ON_EXE_ACT_DP_ISSUE_S;

//当联动执行时，且动作类型是普通的dp点时，会调用回调,
// cond为最后一个触发的条件
//动作如果有多个，会回调多次
typedef VOID (*SCENE_LINKAGE_ON_EXE_DP_ISSUE)(IN CONST SCENE_LINKAGE_ON_EXE_COND_S *cond,
                                              IN CONST SCENE_LINKAGE_ON_EXE_ACT_DP_ISSUE_S *act_dp);

OPERATE_RET scene_linkage_reg_on_exe_dp_issue(IN SCENE_LINKAGE_ON_EXE_DP_ISSUE on_exe_func);

#ifdef __cplusplus
}
#endif
#endif
