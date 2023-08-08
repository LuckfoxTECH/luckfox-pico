
#ifndef __LAN_CLUSTER_BASE_H
#define __LAN_CLUSTER_BASE_H

#include "tuya_cloud_com_defs.h"

#include "lan_rpc.h"

#ifdef __cplusplus
extern "C" {
#endif

//集群对外感知的状态
typedef BYTE_T NODE_CLS_ST_T;
#define CLS_ST_INVALID 0     // 没有加到集群
#define CLS_ST_MASTER 1      // 我是master
#define CLS_ST_NORMAL_NODE 2 // 我是普通节点

//对外的能力判断
#define CLS_CAP_MASTER_MIN 1000 //能变成master节点的最低能力值
#define CLS_CAP_NODE_MIN 100    //能变成普通节点的最低能力值

//集群内部处理感知的状态，每个节点自己感知到自己的状态
typedef BYTE_T NODE_ST_T;
#define ST_MASTER_WAIT 0 // master等待状态
#define ST_MASTER_OK 1   // master
#define ST_NORMAL_WAIT 2 // 普通节点等待
#define ST_NORMAL 3      // 普通节点
#define ST_MAX_NUM 4

#define MASTER_CONFIRM_MSG_TIME 5     // master广播周期，秒为单位
#define MASTER_CONFIRM_MSG_WAIT_NUM 4 // master连续广播之后能成为master的广播次数
#define MASTER_KEEPALIVE_CHECK_NUM 3  //普通节点判断master离线的 周期个数
#define MASTER_KEEPALIVE_CHECK_TIME MASTER_CONFIRM_MSG_TIME //普通节点保活检测定时器周期

#define MST_CFM_MSG_MAX_LEN 128

// master感知到的单个节点的状态变化
typedef BYTE_T NODE_CHANGE_ST_T;
#define NODE_ST_JOINING 0     // 节点发现
#define NODE_ST_JOINED 1      // 节点加入集群
#define NODE_ST_LEAVE 2       // 节点离开
#define NODE_ST_FAST_REJOIN 3 // 离开后快速加入
#define NODE_ST_FORBIDDEN 4   // 发现非法节点

//节点基本信息
typedef struct {
	CHAR_T id[GW_ID_LEN + 1]; // virtual id
	UINT_T ip;
	INT_T seqno;
	USHORT_T cap;
} LAN_NODE_S;

//用于节点上报的信息
typedef struct {
	CHAR_T id[GW_ID_LEN + 1]; // virtual id
} LAN_NODE_REPORT_S;

//用于反馈节点上报结果的信息
typedef struct {
	CHAR_T id[GW_ID_LEN + 1];            // virtual id
	CHAR_T local_key[LOCAL_KEY_LEN + 1]; // local key
	BOOL_T allow;
} LAN_NODE_ACK_S;

//自己的集群状态切换回调
typedef VOID (*STATE_CHANGE_CALLBACK)(NODE_CLS_ST_T new_state, IN CONST LAN_NODE_S *master_node);

//集群中节点变化回调,全量上报
typedef VOID (*CLS_NODE_CHANGE_CALLBACK)(IN CONST LAN_NODE_REPORT_S *node, UINT_T num);

//集群中节点变化回调,单个节点上报
typedef VOID (*CLS_NODE_CHANGE_SINGLE_CALLBACK)(IN CONST LAN_NODE_REPORT_S *node,
                                                NODE_CHANGE_ST_T st);

//不同集群角色，初始化函数类型
typedef OPERATE_RET (*LAN_CLUSTER_INIT)(IN CONST LAN_NODE_S *node);

//不同集群角色，注册自己集群状态切换回调的函数类型
typedef OPERATE_RET (*STATE_CHANGE_CALLBACK_REG)(STATE_CHANGE_CALLBACK cb);

//不同集群角色，注册集群节点变化回调的函数类型
typedef OPERATE_RET (*NODE_CHANGE_CALLBACK_REG)(CLS_NODE_CHANGE_CALLBACK cb);

//不同集群角色，注册反馈节点上报结果的函数类型
typedef OPERATE_RET (*NODE_ACK)(IN CONST LAN_NODE_ACK_S *node_array, UINT_T node_num);

//记录当前master状态的模块初始化
OPERATE_RET lan_cluster_init_node_master_status();

// master广播消息结构体和json的互转
VOID lan_cluster_master_confirm_msg_st2json(CONST LAN_NODE_S *msg_node,
                                            CHAR_T msg_json[MST_CFM_MSG_MAX_LEN]);
OPERATE_RET lan_cluster_master_confirm_msg_json2st(CONST CHAR_T *msg_json, LAN_NODE_S *msg_node);

//判断cap的值是否有master能力
BOOL_T lan_cluster_has_master_cap(USHORT_T cap);

//判断cap的值是否有集群节点能力
BOOL_T lan_cluster_has_node_cap(USHORT_T cap);

OPERATE_RET lan_cluster_init_node_state(IN STATE_CHANGE_CALLBACK_REG cb_reg);

//实现查询当前master节点的回调，lanRPC中使用
OPERATE_RET lan_cluster_lan_rpc_master_get_cb(LAN_RPC_MASTER_S *master);

//实现集群内部维护状态到外部使用者感知状态的转换
VOID lan_cluster_status_change_inner2out(NODE_ST_T new_status_in, const LAN_NODE_S *master_node_in,
                                         NODE_CLS_ST_T *new_status_out,
                                         LAN_NODE_S *master_node_out);

OPERATE_RET lan_setup_udp_server_socket(IN INT_T port, OUT INT_T *fd_out);

#ifdef __cplusplus
}
#endif
#endif
