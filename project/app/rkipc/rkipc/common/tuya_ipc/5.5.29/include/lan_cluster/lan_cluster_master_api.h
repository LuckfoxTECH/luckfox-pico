#ifndef __LAN_CLUSTER_MASTER_API_H
#define __LAN_CLUSTER_MASTER_API_H

#include "lan_cluster_base.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif
//只有master能力节点才能调用的API

//集群中节点的合法性由上层业务确认后调用
OPERATE_RET lan_cluster_node_ack(IN CONST LAN_NODE_ACK_S *node_array, IN UINT_T node_num);

//注册集群中节点状态变化回调，当前是全量
OPERATE_RET lan_cluster_reg_node_cb(IN CLS_NODE_CHANGE_CALLBACK cb);

//注册集群单个节点状态变化回调
OPERATE_RET lan_cluster_reg_node_single_cb(IN CLS_NODE_CHANGE_SINGLE_CALLBACK cb);

//查询节点id是否在集群中
OPERATE_RET lan_cluster_find_node(IN CONST CHAR_T *id);

#ifdef __cplusplus
}
#endif
#endif
