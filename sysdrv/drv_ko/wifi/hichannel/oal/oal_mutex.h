/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oal_mutex.h 的头文件
 * Author: Hisilicon
 * Create: 2020-08-04
 */
#ifndef __OAL_MUTEX_H__
#define __OAL_MUTEX_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include <linux/mutex.h>
#include "hi_types_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 STRUCT定义
*****************************************************************************/
typedef struct mutex          oal_mutex_stru;

#define    oal_mutex_init(mutex)        mutex_init(mutex)
#define    oal_mutex_destroy(mutex)     mutex_destroy(mutex)

/*****************************************************************************
  10 函数声明
*****************************************************************************/
static inline hi_void oal_mutex_lock(oal_mutex_stru *lock)
{
    mutex_lock(lock);
}

static inline hi_s32 oal_mutex_trylock(oal_mutex_stru *lock)
{
    return mutex_trylock(lock);
}

static inline hi_void oal_mutex_unlock(oal_mutex_stru *lock)
{
    mutex_unlock(lock);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_mutex.h */

