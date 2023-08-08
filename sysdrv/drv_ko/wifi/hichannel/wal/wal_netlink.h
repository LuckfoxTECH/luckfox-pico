/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc driver implementatioin.
 * Author: Hisilicon
 * Create: 2020-09-28
 */
#ifndef _WAL_NETLINK_H
#define _WAL_NETLINK_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_util.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#ifdef _PRE_HICHANNEL_HDR_OPT
#define MAX_USER_DATA_LEN               412     /* 小于这个值的数据报文通过高优先级通道传输 */
#else
#define MAX_USER_DATA_LEN               384
#endif
#define MAX_USER_LONG_DATA_LEN          1500    /* 消息最大长度 */
/*****************************************************************************
  3 函数声明
*****************************************************************************/
hi_s32 oal_netlink_init(hi_void);

hi_s32 oal_netlink_deinit(hi_void);

hi_s32 oal_send_user_msg(hi_u8 *buf, hi_u32 len);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif
