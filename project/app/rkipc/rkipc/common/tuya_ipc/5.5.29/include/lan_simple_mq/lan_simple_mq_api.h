#ifndef __LAN_SIMPLE_MQ_API_H
#define __LAN_SIMPLE_MQ_API_H

#include "tuya_cloud_com_defs.h"

#include "lan_msg_reliable_trans.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

//消息方向，确定源和目的
typedef BYTE_T LAN_SMP_MQ_MSG_DRCT;
#define LAN_SMP_MQ_MSG_DRCT_NORMAL_TO_MASTER 0x01 // 消息从普通节点发给master
#define LAN_SMP_MQ_MSG_DRCT_MASTER_TO_NORMAL 0x02 // 消息从master发给  普通节点

#define LAN_SVC_NAME_MAX 32

typedef VOID *LAN_SIMPLE_MQ_SEND_HANDLE;

typedef OPERATE_RET (*LAN_SIMPLE_MQ_RECV_CB)(IN CONST CHAR_T *peer_id, IN CONST CHAR_T *content,
                                             VOID *recv_cb_param);

typedef struct {
	CHAR_T svc_name[LAN_SVC_NAME_MAX + 1];
	LAN_SEND_MSG_MERGE func_msgMerge; //多条消息合并接口，非必须
	VOID *func_msgMerge_param;

	LAN_SEND_MSG_RET func_msgRet; //消息发送结果回调，非必须,
	                              // TODO,是否所有场景都回调？当前只保证正常发送后回调，异常时不一定
	// VOID *func_msgRet_param;

	LAN_SEND_BUF_FREE func_buf_free; //内存释放接口，非必须，空时内部使用Free释放.
	LAN_SMP_MQ_MSG_DRCT drct;        //必须字段
} LAN_SMP_MQ_SEND_SVC_PARAM_S;

typedef struct {
	CHAR_T svc_name[LAN_SVC_NAME_MAX]; //必须字段
	LAN_SMP_MQ_MSG_DRCT drct;          //必须字段
	LAN_SIMPLE_MQ_RECV_CB recv_cb;     //必须字段
	VOID *recv_cb_param;
} LAN_SMP_MQ_RECV_SVC_PARAM_S;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/

OPERATE_RET lan_simple_mq_add_send_service(LAN_SMP_MQ_SEND_SVC_PARAM_S *param,
                                           LAN_SIMPLE_MQ_SEND_HANDLE *handle);

// send_buf 必须得是json格式
//默认情况下   在函数内部使用Free释放
//如果需要特殊处理，需要在创建 send_service中的参数param 中的func_buf_free指定
OPERATE_RET lan_simple_mq_send(LAN_SIMPLE_MQ_SEND_HANDLE handle, IN CONST CHAR_T *peer_id,
                               IN CONST CHAR_T *send_buf, VOID *cb_param);

OPERATE_RET lan_simple_mq_add_recv_service(LAN_SMP_MQ_RECV_SVC_PARAM_S *param);

#ifdef __cplusplus
}
#endif
#endif
