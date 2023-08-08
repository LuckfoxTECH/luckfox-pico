#ifndef __LAN_PUB_SUB_MQ_API_H
#define __LAN_PUB_SUB_MQ_API_H

#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************/
/*
使用说明
1 初始化
    调用 lan_pub_sub_mq_svc_add 新增服务
2 发布
    当某个节点需要发布时，调用 lan_pub_sub_mq_svc_publish
    内部会先调用 LAN_PUB_SUB_MQ_SVC_MSG_SERIAL 完成消息的序列化，发送给订阅者
3 订阅者处理
    内部先会调用 LAN_PUB_SUB_MQ_SVC_MSG_UNSERIAL 完成反序列化，
    然后回调LAN_PUB_SUB_MQ_SVC_FROM_REMOTE_CB通知

注意：发布的内容必须是json格式，即消息序列化函数的出参内容必须是json
*/

/***********************************************************
*************************micro define***********************
***********************************************************/
#define LAN_TOPIC_NAME_LEN_MAX 32

typedef VOID *LAN_PUB_SUB_MQ_SVC_HANDLE;

//收到其他设备的发布消息，回调
typedef OPERATE_RET (*LAN_PUB_SUB_MQ_SVC_FROM_REMOTE_CB)(IN CONST VOID *pub_content,
                                                         VOID *cb_param);

//消息序列化, msg_json在消息发送后，由模块内部Free释放
typedef OPERATE_RET (*LAN_PUB_SUB_MQ_SVC_MSG_SERIAL)(IN CONST VOID *pub_content,
                                                     OUT CHAR_T **pub_msg);

//消息反序列化, pub_content在回调后，由模块内部调用Free释放
typedef OPERATE_RET (*LAN_PUB_SUB_MQ_SVC_MSG_UNSERIAL)(IN CONST CHAR_T *pub_msg,
                                                       OUT VOID **pub_content);

typedef struct {
	CHAR_T topic_name[LAN_TOPIC_NAME_LEN_MAX + 1];
	LAN_PUB_SUB_MQ_SVC_FROM_REMOTE_CB func_from_remote; //收到推送后的回调，必须字段
	VOID *cb_param;                                     //回调的参数
	BOOL_T omit_self;                                   //自己发布的消息，是否回调
	LAN_PUB_SUB_MQ_SVC_MSG_SERIAL func_serail;          //必须字段
	LAN_PUB_SUB_MQ_SVC_MSG_UNSERIAL func_unserail;      //必须字段
} LAN_PUB_SUB_MQ_SVC_PARAM_S;

OPERATE_RET lan_pub_sub_mq_svc_add(IN CONST LAN_PUB_SUB_MQ_SVC_PARAM_S *param,
                                   OUT LAN_PUB_SUB_MQ_SVC_HANDLE *hdl);

OPERATE_RET lan_pub_sub_mq_svc_publish(IN LAN_PUB_SUB_MQ_SVC_HANDLE hdl,
                                       IN CONST VOID *pub_content);

#ifdef __cplusplus
}
#endif
#endif
