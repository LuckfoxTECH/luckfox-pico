/**
 * @file tuya_hal_queue.h
 * @author shiqq@tuya.com
 * @brief queue相关接口封装
 * @version 0.1
 * @date 2021-01-04
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef _TUYA_HAL_QUEUE_H
#define _TUYA_HAL_QUEUE_H

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tuya_hal_queue_create_init用于创建并初始化tuya 消息队列
 *
 * @param[in] msgsize 消息体的大小，msgcount 消息体的个数
 * @param[out] pQueue_hanle 返回queue句柄
 * @return int 0=成功，非0=失败
 */
OPERATE_RET tuya_hal_queue_create_init(QUEUE_HANDLE *queue, int msgsize, int msgcount);

/**
 * @brief tuya_hal_queue_post用于发送一个消息到指定的队列中
 *
 * @param[in] pQueue_hanle tuya queue句柄，data消息体指针，timeout 超时时间
 * @return int 0=成功，非0=失败
 */
OPERATE_RET tuya_hal_queue_post(CONST QUEUE_HANDLE pQueue_hanle, void *data, unsigned int timeout);

/**
 * @brief tuya_hal_queue_fetch用于等待一个消息队列
 *
 * @param[in] pQueue_hanle  tuya queue句柄，data消息体指针，timeout 超时时间
 * @return int 0=成功，非0=失败
 */
OPERATE_RET tuya_hal_queue_fetch(QUEUE_HANDLE pQueue_hanle, void *msg, unsigned int timeout);

/**
 * @brief tuya_hal_queue_free 释放消息队列
 *
 * @param[in] mutexHandle QUEUE_HANDLE tuya queue句柄
 * @return int 0=成功，非0=失败
 */
OPERATE_RET tuya_hal_queue_free(CONST QUEUE_HANDLE pQueue_hanle);

/**
 * @brief tuya_os_queue_intf_init queue组件接口
 *
 * @return int 0=成功，非0=失败
 */

/* add begin: by sunkz, interface regist */
VOID_T tuya_os_queue_intf_init(VOID_T);
/* add end */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
