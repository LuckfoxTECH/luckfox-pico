/*
lan_cluster_api.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef __LAN_CLUSTER_API_H
#define __LAN_CLUSTER_API_H

#include "lan_cluster_base.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

//集群功能初始化
OPERATE_RET lan_cluster_init(IN CONST LAN_NODE_S *node);

//获取我自己的节点信息
OPERATE_RET lan_cluster_get_my_node(OUT LAN_NODE_S *my_node);

//注册集群中自身状态变化回调
OPERATE_RET lan_cluster_reg_state_cb(IN STATE_CHANGE_CALLBACK cb);

OPERATE_RET lan_cluster_get_state(OUT NODE_CLS_ST_T *state, OUT LAN_NODE_S *master_node);

//查询master最近一次保活的时间
VOID lan_cluster_get_master_keepalive_time(OUT TIME_S *ka_time);

#ifdef __cplusplus
}
#endif
#endif
