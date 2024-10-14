/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    adapter.h
  * @version V0.0.1
  * @brief   the module is to bridge the interface for different os.
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */
#ifndef __ADAPTER_H__
#define __ADAPTER_H__

#if defined(__RT_THREAD__)

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include "adapter_type.h"

#ifdef RT_USING_VICAP
#define VICAP_MODULE_COMPILED
#else
#undef VICAP_MODULE_COMPILED
#endif

#ifdef RT_USING_ISP3
#define ISP_MODULE_COMPILED
#else
#undef ISP_MODULE_COMPILED
#endif

#if defined(VICAP_MODULE_COMPILED)

#if defined(RT_USING_VICAP_WORKMODE_FRAME_ONEFRAME)
#define VICAP_MODULE_WORKMODE_FRAME_ONEFRAME
#elif defined(RT_USING_VICAP_WORKMODE_FRAME_PINGPONG)
#define VICAP_MODULE_WORKMODE_FRAME_PINGONG
#elif defined(RT_USING_VICAP_WORKMODE_BLOCK_PINGPONG)
#define VICAP_MODULE_WORKMODE_BLOCK_PINGPONG
#endif

#include "dma.h"
#include "iomux.h"
#include "drv_heap.h"
#include "drv_clock.h"

#endif

#if defined(ISP_MODULE_COMPILED)
#include "dma.h"
#include "drv_heap.h"
#include "drv_clock.h"
#endif

#if defined(VICAP_MODULE_COMPILED) || defined(ISP_MODULE_COMPILED)
#define ADAPTER_MODULE_COMPILED
#else
#undef  ADAPTER_MODULE_COMPILED
#endif

#elif defined(__RK_OS__)

#include "sys/defs.h"
#include "sys/queue.h"
#include "sdkconfig.h"
#include "I2cDevice.h"
#include "rk_cache.h"
#include "interrupt.h"
#include "subsys/global.h"
#include "BspConfig.h"
#include "iomux.h"

#include "kernel/RKOS.h"
#include "kernel/device.h"
#include "kernel/rkutil.h"
#include "kernel/TaskPlugin.h"
#include "driver/cru.h"
#include "adapter_type.h"

#ifdef CONFIG_DRIVER_VICAP
#define VICAP_MODULE_COMPILED
#else
#undef VICAP_MODULE_COMPILED
#endif

#ifdef CONFIG_DRIVER_ISP3
#define ISP_MODULE_COMPILED
#else
#undef ISP_MODULE_COMPILED
#endif

#if defined(VICAP_MODULE_COMPILED)

#include "hal_vicap.h"

#if defined(CONFIG_VICAP_WORKMODE_FRAME_ONEFRAME)
#define VICAP_MODULE_WORKMODE_FRAME_ONEFRAME
#elif defined(CONFIG_VICAP_WORKMODE_FRAME_PINGPONG)
#define VICAP_MODULE_WORKMODE_FRAME_PINGONG
#elif defined(CONFIG_VICAP_WORKMODE_BLOCK_PINGPONG)
#define VICAP_MODULE_WORKMODE_BLOCK_PINGPONG
#endif

#endif

#if defined(VICAP_MODULE_COMPILED) || defined(ISP_MODULE_COMPILED)
#define ADAPTER_MODULE_COMPILED
#else
#undef  ADAPTER_MODULE_COMPILED
#endif

#endif

#if defined(__RT_THREAD__)

/* Compiler Related Definitions */
#define rk_inline                               rt_inline

/* redefine macro function */
#define RK_ALIGN(size, align)                   RT_ALIGN(size, align)
#define rk_container_of(ptr, type, member)      rt_container_of(ptr, type, member)

/* redefine system variables */
typedef rt_mutex_t                              rk_mutex_t;
typedef rt_sem_t                                rk_semaphore_t;
typedef struct rt_device                        rk_device;
typedef struct rt_i2c_bus_device                rk_i2c_bus_device;
typedef struct clk_gate                         rk_clk_gate;

/* define private system variables */
typedef rt_list_t                               rk_list_node;
typedef rt_list_t                               rk_queue_list;

#define rk_list_first_entry(ptr, type, member)              rt_list_first_entry(ptr, type, member)
#define rk_list_for_each_entry(pos, head, member)           rt_list_for_each_entry(pos, head, member)
#define rk_kprintf                                          rt_kprintf

#elif defined(__RK_OS__)

/* Compiler Related Definitions */
#define rk_inline                               static __inline

/* redefine macro function */
#define RK_ALIGN(size, align)                   (((size) + (align) - 1) & ~((align) - 1))
#define rk_container_of(ptr, type, field)       container_of(ptr, type, field)

/* redefine system variables */
typedef pSemaphore                              rk_mutex_t;
typedef pSemaphore                              rk_semaphore_t;
typedef struct _DEVICE_CLASS                    rk_device;
typedef struct _I2C_DEVICE_CLASS                rk_i2c_bus_device;
typedef struct _CLK_GATE                        rk_clk_gate;

/* define private system variables */
typedef struct _list_node
{
    TAILQ_ENTRY(_list_node) node_entry;
} rk_list_node;

typedef TAILQ_HEAD(queue_list, _list_node)       rk_queue_list;

#define rk_list_first_entry(head, type, member)            \
({\
    rk_list_node *node = (rk_list_node *)TAILQ_FIRST(head);\
    rk_container_of(node, type, member);\
})

#define node_list_for_each_entry(var, head, field)          TAILQ_FOREACH(var, head, field)
/* Be carefully! vicap_video_buf is prohibited to be modified,
 * if it musts be modified, please modify the name of struct vicap_video_buf
 * in drv_vicap.h at the same time
 */
#define rk_list_for_each_entry(pos, head, member)           \
    rk_list_node *list_node;\
    for ((pos) = rk_list_first_entry(head, struct vicap_video_buf, member),\
          list_node = TAILQ_FIRST(head);\
          (pos);\
          list_node = TAILQ_NEXT(list_node, node_entry),\
          list_node ? ((pos) = rk_container_of(list_node, struct vicap_video_buf, member)) :\
          ((pos) = RK_NULL))

#define rk_kprintf                                rk_printf

#endif

/* system interfaces */
#if defined(__RT_THREAD__)
#define rk_interrupt_enter()                      rt_interrupt_enter()
#define rk_interrupt_leave()                      rt_interrupt_leave()
#elif defined(__RK_OS__)
#define rk_interrupt_enter()                      rkos_critical_enter()
#define rk_interrupt_leave()                      rkos_critical_exit()
#endif

dt_base_t rk_hw_interrupt_disable(void);
void rk_hw_interrupt_enable(dt_base_t level);

rk_tick_t rk_tick_get(void);

rk_mutex_t rk_mutex_create(const char *name, uint32_t flag);
ret_err_t rk_mutex_take(rk_mutex_t mutex, uint32_t time);
ret_err_t rk_mutex_release(rk_mutex_t mutex);
ret_err_t rk_mutex_delete(rk_mutex_t mutex);

rk_semaphore_t rk_semaphore_create(const char *name, uint32_t value,
                                   uint32_t flag);
ret_err_t rk_semaphore_take(rk_semaphore_t sem, uint32_t time);
ret_err_t rk_semaphore_release(rk_semaphore_t sem);
ret_err_t rk_semaphore_delete(rk_semaphore_t sem);

void rk_free(void *mem);
void *rk_malloc(uint32_t size);
void rk_mem_free(void *mem);
void *rk_mem_malloc(uint32_t size);
void rk_dma_free_large(void *mem);
void *rk_dma_malloc_large(dt_ubase_t size);
void *rk_memset(void *s, int val, dt_ubase_t count);
void *rk_memcpy(void *dst, const void *src, dt_ubase_t count);
void rk_hw_cpu_dcache_ops(int ops, void *addr, int size);

ret_size_t rk_strlen(const char *s);
char *rk_strstr(const char *s1, const char *s2);
char *rk_strncpy(char *dst, const char *src, dt_ubase_t len);
int32_t rk_strcmp(const char *cs, const char *ct);
int32_t rk_strncmp(const char *cs, const char *ct, dt_ubase_t len);

ret_err_t rk_device_init(rk_device *dev);
rk_device *rk_find_device(const char *dev_name);
ret_err_t rk_device_close(rk_device *dev);
ret_err_t rk_device_open(rk_device *dev, uint16_t flag);
ret_err_t rk_device_control(rk_device *dev, int cmd, void *arg);

ret_err_t rk_clk_enable(rk_clk_gate *gate);
ret_err_t rk_clk_disable(rk_clk_gate *gate);
ret_err_t rk_clk_get_rate(eCLOCK_Name clk_id);
ret_err_t rk_clk_set_rate(eCLOCK_Name clk_id, uint32_t rate);
rk_clk_gate *rk_get_clk_gate_from_id(int clk_id);

rk_inline int rk_queue_list_isemty(rk_queue_list *head)
{
    MACRO_ASSERT(head != RK_NULL);

#if defined(__RT_THREAD__)

    return rt_list_isempty(head);

#elif defined(__RK_OS__)

    return TAILQ_EMPTY(head);

#endif
}

rk_inline void rk_list_node_init(rk_list_node *node)
{
    MACRO_ASSERT(node != RK_NULL);

#if defined(__RT_THREAD__)

    rt_list_init(node);

#elif defined(__RK_OS__)

    node->node_entry.tqe_next = RK_NULL;
    node->node_entry.tqe_prev = &node->node_entry.tqe_next;

#endif

}

rk_inline void rk_queue_list_init(rk_queue_list *head)
{
    MACRO_ASSERT(head != RK_NULL);

#if defined(__RT_THREAD__)

    rt_list_init(head);

#elif defined(__RK_OS__)

    TAILQ_INIT(head);

#endif
}

rk_inline void rk_list_insert_before(rk_queue_list *head,
                                     rk_list_node *node)
{
    MACRO_ASSERT(head != NULL);
    MACRO_ASSERT(node != NULL);

#if defined(__RT_THREAD__)

    rt_list_insert_before(head, node);

#elif defined(__RK_OS__)

    TAILQ_INSERT_TAIL(head, node, node_entry);

#endif
}

rk_inline void rk_list_remove(rk_queue_list *head,
                              rk_list_node *node)
{
    MACRO_ASSERT(head != NULL);
    MACRO_ASSERT(node != NULL);

#if defined(__RT_THREAD__)

    rt_list_remove(node);

#elif defined(__RK_OS__)

    TAILQ_REMOVE(head, node, node_entry);

#endif
}

#endif

