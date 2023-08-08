/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oal_workqueue.h 的头文件
 * Author: Hisilicon
 * Create: 2020-08-04
 */

#ifndef __OAL_LINUX_WORKQUEUE_H__
#define __OAL_LINUX_WORKQUEUE_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include "hi_types.h"
#include "oal_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 STRUCT定义
*****************************************************************************/
typedef struct workqueue_struct          oal_workqueue_stru;
typedef struct work_struct               oal_work_stru;
typedef struct delayed_work              oal_delayed_work;

/*****************************************************************************
  7 宏定义
*****************************************************************************/
#define oal_init_work(_p_work, _p_func)            INIT_WORK(_p_work, _p_func)
#define oal_init_delayed_work(_work, _func)         INIT_DELAYED_WORK(_work, _func)
#define oal_create_singlethread_workqueue_macro(_name)   create_singlethread_workqueue(_name)
#define oal_create_workqueue(name)                 create_workqueue(name)

/* inline 函数定义 */
/*****************************************************************************
 功能描述  : 创建一个单线程的工作队列
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
static inline oal_workqueue_stru* oal_create_singlethread_workqueue(hi_char *pc_workqueue_name)
{
    return create_singlethread_workqueue(pc_workqueue_name);
}

/*****************************************************************************
 功能描述  : 销毁工作队列
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
static inline hi_void oal_destroy_workqueue(oal_workqueue_stru *pst_workqueue)
{
    destroy_workqueue(pst_workqueue);
}

/*****************************************************************************
 功能描述  : 添加一个任务到工作队列
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
static inline hi_u32  oal_queue_work(oal_workqueue_stru *pst_workqueue, oal_work_stru *pst_work)
{
    return queue_work(pst_workqueue, pst_work);
}

/**
 * queue_delayed_work - queue work on a workqueue after delay
 * @wq: workqueue to use
 * @dwork: delayable work to queue
 * @delay: number of jiffies to wait before queueing
 *
 * Equivalent to queue_delayed_work_on() but tries to use the local CPU.
 */
static inline hi_u32  oal_queue_delayed_work(oal_workqueue_stru *pst_workqueue,
                                             oal_delayed_work *pst_work, unsigned long delay)
{
    return queue_delayed_work(pst_workqueue, pst_work, delay);
}

/**
 * queue_delayed_work_on - queue work on specific CPU after delay
 * @cpu: CPU number to execute work on
 * @wq: workqueue to use
 * @dwork: work to queue
 * @delay: number of jiffies to wait before queueing
 *
 * Returns %false if @work was already on a queue, %true otherwise.  If
 * @delay is zero and @dwork is idle, it will be scheduled for immediate
 * */
static inline hi_u32  oal_queue_delayed_work_on(hi_u32 cpu, oal_workqueue_stru *pst_workqueue,
                                                oal_delayed_work *pst_work, unsigned long delay)
{
    return queue_delayed_work_on(cpu, pst_workqueue, pst_work, delay);
}

/*****************************************************************************
 功能描述  : queue work on system wq after delay
 输入参数  :  @dwork: delayable work to queue
              @delay: number of jiffies to wait before queueing
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
static inline hi_u32  oal_queue_delayed_system_work(oal_delayed_work *pst_work, unsigned long delay)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 35))
    return queue_delayed_work(system_wq, pst_work, delay);
#else
    return 1;
#endif
}

#define oal_work_is_busy(work)              work_busy(work)
#define oal_cancel_delayed_work_sync(dwork) cancel_delayed_work_sync(dwork)
#define oal_cancel_work_sync(work)          cancel_work_sync(work)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_workqueue.h */
