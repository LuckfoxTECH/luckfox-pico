#ifndef __LAN_MSG_RELIABLE_TRANS_H
#define __LAN_MSG_RELIABLE_TRANS_H

#include "lan_cluster_api.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

#define MAX_MERGE_NUM_IN_MSG 3 //一个RPC消息中包含最大的命令个数

#define PEER_NODE_ALLOW_CB_MAX 4

typedef struct {
	CHAR_T *send_buf[MAX_MERGE_NUM_IN_MSG]; // 数量的
	BYTE_T merge_cnt;                       // send_buf的实际个数
	CHAR_T *send_buf_out;                   // 合并后的send_buf
} LAN_MSG_MERGE_INFO;

typedef OPERATE_RET (*LAN_SEND_MSG2JSON)(IN CONST CHAR_T *send_buf, IN INT_T seq, IN VOID *param,
                                         OUT JRPC_MSG_S *requset);

typedef VOID (*LAN_SEND_MSG_MERGE)(IN OUT LAN_MSG_MERGE_INFO *merge_info, IN VOID *param);

//注意：如果允许合并信息，param字段会是合并前的第一个消息的参数
typedef VOID (*LAN_SEND_MSG_RET)(IN OPERATE_RET op_ret, IN CONST CHAR_T *id,
                                 IN CONST CHAR_T *send_buf, IN VOID *param);

typedef VOID (*LAN_SEND_BUF_FREE)(IN CONST CHAR_T *send_buf, IN VOID *cb_param);

typedef VOID (*LAN_PEER_NODE_ALLOW)(IN CONST CHAR_T *id, IN BOOL_T allow);

typedef struct {
	USHORT_T init_peer_cnt; //初始化对端节点的个数
	USHORT_T max_retry_cnt; //每个消息的最大的重试次数，如果超过，就开始丢弃该消息
	USHORT_T max_total_retry_cnt; //最大的连续重试次数，如果超过，就开始丢消息drop_msg_cnt个
	USHORT_T drop_msg_cnt; //一次丢消息的个数
	USHORT_T
	cache_msg_max_cnt_per_peer; //每个对端存储的最大的消息个数，一旦超过。视为消息堆积，快速丢弃一半的新报文
} LAN_MSG_MOD_INIT_PARAM;

//为不同类型的消息，提供差异的部分
typedef struct {
	LAN_SEND_MSG2JSON func_msg2Json; //消息转为json-RPC接口，必须
	VOID *func_msg2Json_param;

	LAN_SEND_MSG_MERGE func_msgMerge; //多条消息合并接口，非必须
	VOID *func_msgMerge_param;

	LAN_SEND_MSG_RET func_msgRet; //消息发送结果回调，非必须,
	                              // TODO,是否所有场景都回调？当前只保证正常发送后回调，异常时不一定
	// VOID *func_msgRet_param;			//发送结果回调的参数在调用发送接口处传入

	LAN_SEND_BUF_FREE func_buf_free; //内存释放接口，非必须，空时内部使用Free释放.
	                                 //如果有merge函数，发生merge时内部使用Malloc Free

} LAN_MSG_RELI_TYPE_PARAM;

typedef VOID *LAN_MSG_RELI_TYPE;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/

OPERATE_RET lan_msg_reli_trans_init(LAN_MSG_MOD_INIT_PARAM *init_param);

OPERATE_RET lan_msg_get_recv_seq(IN CONST CHAR_T *id, OUT INT_T *recv_seq);

OPERATE_RET lan_msg_set_recv_seq(IN CONST CHAR_T *id, IN INT_T recv_seq);

// send_buf在函数内部释放
OPERATE_RET lan_msg_reli_send(LAN_MSG_RELI_TYPE type, IN CONST CHAR_T *id,
                              IN CONST CHAR_T *send_buf, VOID *cb_param);

OPERATE_RET lan_msg_add_peer_node(IN CONST CHAR_T *id);

OPERATE_RET lan_msg_stop_peer_node(IN CONST CHAR_T *id);

OPERATE_RET lan_msg_del_peer_node(IN CONST CHAR_T *id);

OPERATE_RET lan_msg_del_all_peer_node();

OPERATE_RET lan_msg_peer_node_clear_com_stat(IN CONST CHAR_T *id);

#if 0
OPERATE_RET lan_msg_set_peer_node_com_allow(IN CONST CHAR_T *id, BOOL_T allow);
OPERATE_RET lan_msg_get_peer_node_com_allow(IN CONST CHAR_T *id, BOOL_T *allow);
OPERATE_RET lan_msg_attach_on_peer_node_allow(LAN_PEER_NODE_ALLOW func_cb);
#endif

#ifdef __cplusplus
}
#endif
#endif
