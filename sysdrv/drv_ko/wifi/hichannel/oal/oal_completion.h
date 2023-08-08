/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oal_completion.h
 * Author: Hisilicon
 * Create: 2020-09-02
 */

#ifndef __OAL_COMPLETION_H__
#define __OAL_COMPLETION_H__

/* 头文件 */
#include <linux/completion.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 宏定义 */
typedef struct completion oal_completion;

#define oal_init_completion(_my_completion)     init_completion(_my_completion)
#define oal_complete(_my_completion)            complete(_my_completion)
#define oal_wait_for_completion(_my_completion) wait_for_completion(_my_completion)
#define oal_complete_all(_my_completion)        complete_all(_my_completion)

/* inline 函数定义 */
/*****************************************************************************
 功能描述  : 同步：等待超时检查
 返 回 值  : Return: 0 if timed out, and positive (at least 1, or number of jiffies left till timeout) if completed.
*****************************************************************************/
static inline hi_u32 oal_wait_for_completion_timeout(oal_completion *pst_completion, hi_u32 ul_timeout)
{
    return wait_for_completion_timeout(pst_completion, ul_timeout);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

