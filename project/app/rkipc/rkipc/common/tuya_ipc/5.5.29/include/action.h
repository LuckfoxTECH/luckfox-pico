/***********************************************************
 *  File: action.h
 *  Author: nzy
 *  Date: 20180718
 ***********************************************************/
#ifndef __ACTION_H
#define __ACTION_H

#include "mem_pool.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"
#include "ty_cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ACTION_GLOBALS
#define __ACTION_EXT
#else
#define __ACTION_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define RULE_ID_LEN 20

typedef BYTE_T ACTION_TP_T;
#define AT_DP 1          // dp issue
#define AT_RULE 2        // rule trigger
#define AT_DELAY 3       // delay event
#define AT_MULTI 4       // multi event
#define AT_TOGGLE 5      // toggle event
#define AT_SECURE_MODE 6 // secure mode
#define AT_DP_STEP 7     // dp step event
#define AT_GROUP 8       // group

#if defined(LOCAL_SCENE) && (LOCAL_SCENE == 1)
typedef struct {
	CHAR_T *gid;
	CHAR_T *sid;
} AT_DP_SID_GID_S;
#endif
// dp issue
typedef struct {
	CHAR_T *mlk_dp_cmd; // {"cid":"xxx",dps:{"1":true,"2":100}}
#if defined(LOCAL_SCENE) && (LOCAL_SCENE == 1)
	BOOL_T is_gs; // gid ,sid
	AT_DP_SID_GID_S gs_id;
#endif
} AT_DP_ISSUE_S;

// rule trigger
typedef struct {
	CHAR_T id[RULE_ID_LEN + 1]; //后续改成指针，外部复制
} AT_RULE_TRIGGER_S;

// delay event
typedef struct {
	UINT_T msecond;
} AT_DELAY_S;

typedef union {
	BOOL_T bl;
	INT_T val; // notice:bitmap dp maxlen <= 31
	CHAR_T *str;
} ACTION_VAL_U;

// multi event
typedef struct {
	CHAR_T cid[DEV_ID_LEN + 1];
	CHAR_T dp_id;
	ACTION_VAL_U dp_val;
	INT_T type;
	CHAR_T flag; // first
} AT_MULTI_S;

// toggle event
typedef struct {
	CHAR_T cid[DEV_ID_LEN + 1]; // device id
	BYTE_T dp_id;
} AT_TOGGLE_S;

typedef struct {
	// CHAR_T cid[DEV_ID_LEN+1]; // device id
	INT_T mode;
} AT_SECURE_MODE_S;

typedef enum {
	DP_STEP_TYPE_LOW,
	DP_STEP_TYPE_HIGH,
} DP_STEP_TYPE_E;

typedef struct {
	CHAR_T cid[DEV_ID_LEN + 1];
	BYTE_T dp_id;
	INT_T min;
	INT_T max;
	INT_T step;
	DP_STEP_TYPE_E type;
} AT_DPSTEP_S;

typedef struct {
	CHAR_T *mlk_dp_cmd; // {"cid":"000d6ffffe6d7101","ctype":2,"dps":{"3":false},"mbid":"10020"}
} AT_GROUP_S;

typedef union {
	AT_DP_ISSUE_S dp_isu;
	AT_RULE_TRIGGER_S rule_trig;
	AT_DELAY_S delay;
	AT_MULTI_S multi;
	AT_TOGGLE_S dp_toggle;
	AT_SECURE_MODE_S secure_mode;
	AT_GROUP_S group;
	AT_DPSTEP_S dp_step;
} ACTION_U;

typedef struct {
	ACTION_TP_T ac_tp;
	ACTION_U ac;
	BOOL_T is_check; //检查状态是否跟设置的一样，双控环路检查
#if defined(ENABLE_LAN_LINKAGE_MASTER) && (ENABLE_LAN_LINKAGE_MASTER == 1)
	CHAR_T *gw_id; //当局域网联动时不为空，Malloc出来，记录网关ID或WIFI设备的虚拟ID
	DP_PROP_TP_E dp_tp;       //多控关联时，需要记录dp点的类型
	DP_PROP_ENUM_S prop_enum; //类型是枚举时，枚举值
#endif
} ACTION_S;

// action set
typedef struct s_action {
	INT_T ruleType;
	USHORT_T num;
	ACTION_S action[0];
} ACTION_SET_S;

typedef struct {
	INT_T ruleType;
	UINT_T index;
	// ACTION_SET_S *action_set;
	CHAR_T rule_id[RULE_ID_LEN + 1];
	BYTE_T ruleTypeLocal; // LINKAGE_RULE_LOCAL or LINKAGE_RULE_LAN
	USHORT_T seq_no;      // rule seqno
} DELAY_ACTION_SET_S;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
 *  Function: actions_parse
 *  Input: root
 *  Output: cond_set
 *  Return: OPERATE_RET
 ***********************************************************/
__ACTION_EXT
OPERATE_RET actions_parse(IN ty_cJSON *root, OUT ACTION_SET_S **act_set);

/***********************************************************
 *  Function: free_action_set
 *  Input: action_set
 *  Output: action_set
 *  Return: OPERATE_RET
 ***********************************************************/
__ACTION_EXT
VOID free_action_set(INOUT ACTION_SET_S *action_set);

__ACTION_EXT
VOID actions_dump_print(ACTION_SET_S *p_act_set);

#if defined(ENABLE_LAN_LINKAGE_MASTER) && (ENABLE_LAN_LINKAGE_MASTER == 1)
typedef OPERATE_RET (*ACTION_CHECK_GW_ID_FUNC)(IN CONST CHAR_T *gw_id);

__ACTION_EXT
OPERATE_RET actions_check_gw_id(IN CONST ty_cJSON *root, IN ACTION_CHECK_GW_ID_FUNC check_func);
#endif

OPERATE_RET actions_set_gw_vid(IN CHAR_T *gw_vid);
#ifdef __cplusplus
}
#endif
#endif
