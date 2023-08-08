/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: HMAC module initialization and uninstallation.
 * Author: Hisilicon
 * Create: 2020-09-11
 */

#ifndef __HCC_ADAPT_H__
#define __HCC_ADAPT_H__

/* 头文件包含 */
#include "hi_types.h"
#include "hcc_comm.h"
#include "oal_netbuf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_PRI_MEM_LOW_LEVEL       2
#define LOW_PRI_MEM_LOW_LEVEL      5

#define MAX_CNT_IN_QUEUE           1000
#define AWAKE_CNT_IN_QUEUE         200  /* 唤醒tcp/ip协议栈发包队列门限值 */

#define MIN_SLEEP_TIME             1000
#define MAX_SLEEP_TIME             1000

typedef enum {
    HCC_SUB_TYPE_IP_DATA,
    HCC_SUB_TYPE_USER_MSG,
    HCC_SUB_TYPE_BUTT,
}hcc_sub_type_enum;

/* 函数声明 */
hi_u32  hcc_tx_data_adapt(oal_netbuf_stru *netbuf, hcc_type_enum type, hi_u32 sub_type);
hi_u32  hcc_adapt_init(hi_void);
hi_void hcc_adapt_exit(hi_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
