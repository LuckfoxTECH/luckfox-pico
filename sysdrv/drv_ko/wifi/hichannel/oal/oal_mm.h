/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oal_mm.h 的头文件
 * Author: Hisilicon
 * Create: 2020-09-03
 */
#ifndef __OAL_MM_H__
#define __OAL_MM_H__

/* 头文件包含 */
#include <linux/slab.h>
#include <linux/hardirq.h>
#include <linux/vmalloc.h>

#include "hi_types_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 宏定义 */
#define OAL_GFP_KERNEL                          GFP_KERNEL
#define OAL_GFP_ATOMIC                          GFP_ATOMIC

/* inline 函数声明 */
/*****************************************************************************
 功能描述  : 申请核心态的内存空间，并填充0。对于Linux操作系统而言，
             需要考虑中断上下文和内核上下文的不同情况(GFP_KERNEL和GFP_ATOMIC)。
 输入参数  : ul_size: alloc mem size
 返 回 值  : alloc mem addr
*****************************************************************************/
static inline hi_void* oal_memalloc(hi_u32 ul_size)
{
    hi_s32   l_flags = GFP_KERNEL;
    hi_void   *puc_mem_space = HI_NULL;

    /* 不睡眠或在中断程序中标志置为GFP_ATOMIC */
    if (in_interrupt() || irqs_disabled()) {
        l_flags = GFP_ATOMIC;
    }

    puc_mem_space = kmalloc(ul_size, l_flags);
    if (puc_mem_space == HI_NULL) {
        return HI_NULL;
    }

    return puc_mem_space;
}

static inline hi_void* oal_kzalloc(hi_u32 ul_size, hi_s32 l_flags)
{
    return kzalloc(ul_size, l_flags);
}

static inline hi_void*  oal_vmalloc(hi_u32 ul_size)
{
    return vmalloc(ul_size);
}

/*****************************************************************************
 功能描述  : 释放核心态的内存空间。
*****************************************************************************/
static inline hi_void  oal_free(const hi_void *p_buf)
{
    kfree(p_buf);
}

static inline hi_void  oal_vfree(hi_void *p_buf)
{
    vfree(p_buf);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_mm.h */

