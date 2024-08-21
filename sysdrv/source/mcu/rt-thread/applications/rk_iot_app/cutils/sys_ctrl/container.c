/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <rtthread.h>
#include <rtdef.h>
#include <rtservice.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "kernel/os/os.h"
// #include "sys/list.h"
// #include "sys/param.h"
#include "defs.h"
#include "container.h"


#define CONTAINER_DEBUG(fmt, arg...)    rt_kprintf("[Container debug] <%s : %d> " fmt "\n", __func__, __LINE__, ##arg)
#define CONTAINER_ALERT(fmt, arg...)    rt_kprintf("[Container alert] <%s : %d> " fmt "\n", __func__, __LINE__, ##arg)
#define CONTAINER_ERROR(fmt, arg...)    rt_kprintf("[Container error] <%s : %d> " fmt "\n", __func__, __LINE__, ##arg)
#define CONTAINER_NOWAY()               rt_kprintf("[Container should not be here] <%s : %d> \n", __func__, __LINE__)
#define CONTAINER_NOTSUPPORT()          CONTAINER_ALERT("not support command")


typedef struct list_node
{
    rt_list_t node;
    uint32_t arg;
} list_node;

typedef struct sorted_list
{
    container_base base;
    rt_sem_t sem;
    rt_mutex_t lock;
    rt_list_t head;
    rt_list_t pool;
    list_node *poolBuf;
    int (*compare)(uint32_t newArg, uint32_t oldArg);
} sorted_list;

typedef struct sorted_list_config
{
    uint32_t size;
    int (*compare)(uint32_t oldArg, uint32_t newArg);
} sorted_list_config;

static int sorted_list_init(struct container_base *base, uint32_t config)
{
//  rt_err_t ret1 = -RT_ERROR;
//  rt_err_t ret2 = -RT_ERROR;
    list_node *vec = NULL;
    sorted_list_config *cfg = (sorted_list_config *)config;
    sorted_list *impl = __containerof(base, sorted_list, base);

    rt_list_init(&impl->head);
    rt_list_init(&impl->pool);
    impl->base.size = cfg->size;
    impl->compare = cfg->compare;

    impl->sem = rt_sem_create("list_sem", 0, RT_IPC_FLAG_FIFO);
    if (impl->sem == RT_NULL)
        goto failed;

    impl->lock = rt_mutex_create("list_mutex", RT_IPC_FLAG_FIFO);
    if (impl->lock != RT_NULL)
        goto failed;

    vec = rt_malloc(sizeof(list_node) * impl->base.size);
    if (vec == RT_NULL)
        goto failed;
    rt_memset(vec, 0, sizeof(list_node) * impl->base.size);
    impl->poolBuf = vec;

    for (int i = impl->base.size; i-- > 0;)
    {
        rt_list_init(&vec[i].node);
        //list_add_tail(&vec[i].node, &impl->pool);
        rt_list_insert_before(&impl->pool, &vec[i].node);
    }

    return 0;

failed:
    if (vec != NULL)
        rt_free(vec);

    if (impl->sem)
        rt_sem_delete(impl->sem);
    if (impl->lock)
        rt_mutex_delete(impl->lock);
    if (impl != NULL)
        rt_free(impl);

    return -1;
}

static int sorted_list_deinit(struct container_base *base)
{
    sorted_list *impl = __containerof(base, sorted_list, base);

    /* TODO: flush the node left */

    rt_sem_delete(impl->sem);
    rt_mutex_delete(impl->lock);
    if (impl->poolBuf != NULL)
        rt_free(impl->poolBuf);
    if (impl != NULL)
        rt_free(impl);

    return 0;
}

static int sorted_list_control(struct container_base *base, uint32_t cmd, uint32_t arg)
{
    sorted_list *impl = __containerof(base, sorted_list, base);
    /* TODO: tbc... */
    (void)impl;
    return -1;
}

static int sorted_list_push(struct container_base *base, uint32_t arg, uint32_t timeout)
{
#define BLOCK_WAIT_EACHTIME (3)
    sorted_list *impl = __containerof(base, sorted_list, base);
    list_node *node = NULL;
    list_node *tmp = NULL;

    rt_mutex_take(impl->lock, RT_WAITING_FOREVER);
    //OS_MutexLock(&impl->lock, -1);    /* TODO: OS should move to event_queue */

    /* 1. get 1 node from pool */


    while ((node = rt_list_first_entry(&impl->pool, list_node, node)) == NULL)
    {
        rt_mutex_release(impl->lock);
        rt_thread_delay(BLOCK_WAIT_EACHTIME);
        if (timeout <= BLOCK_WAIT_EACHTIME)
        {
            CONTAINER_ALERT("list full and timeout");

            return -1;
        }
        rt_mutex_take(impl->lock, RT_WAITING_FOREVER);
        timeout -= BLOCK_WAIT_EACHTIME;
    }
    node->arg = arg;

    CONTAINER_DEBUG("get a node from pool");

    /* 2. find a proper place and insert it */
    rt_list_for_each_entry(tmp, &impl->head, node)
    {
        if (impl->compare(arg, tmp->arg) >= 0)
        {
            tmp = rt_list_prev_entry(tmp, node);
            break;
        }
    }
    rt_list_insert_after(&tmp->node, &node->node);

    CONTAINER_DEBUG("insert node to list");

    /* 3. release sem to pop */
    rt_sem_release(impl->sem);

    rt_mutex_release(impl->lock);
    return 0;
}

static int sorted_list_pop(struct container_base *base, uint32_t *arg, uint32_t timeout)
{
    sorted_list *impl = __containerof(base, sorted_list, base);
    list_node *node = NULL;
    rt_err_t ret;

    ret = rt_sem_take(impl->sem, timeout);
    if (ret != RT_EOK)
        return -1;

    rt_mutex_take(impl->lock, RT_WAITING_FOREVER);

    /* 1. get node from list */
    node = rt_list_first_entry(&impl->head, list_node, node);
    if (node == NULL)
    {
        CONTAINER_ERROR("list empty but sem released!");
        rt_mutex_release(impl->lock);
        return -2;
    }
    *arg = node->arg;

    CONTAINER_DEBUG("get a node from list");

    /* 2. remove the node from this and push it to pool */
    rt_list_remove(&node->node);
    rt_list_insert_before(&impl->head, &node->node);

    CONTAINER_DEBUG("remove a node from list and push it to pool");

    rt_mutex_release(impl->lock);
    return 0;
}

container_base *sorted_list_create(uint32_t size, int (*compare)(uint32_t newArg, uint32_t oldArg))
{
    sorted_list *impl = rt_malloc(sizeof(*impl));
    if (impl == NULL)
        return NULL;
    rt_memset(impl, 0, sizeof(*impl));

    sorted_list_config cfg;
    cfg.compare = compare;
    cfg.size = size;

    impl->base.control = sorted_list_control;
    impl->base.deinit = sorted_list_deinit;
    impl->base.pop = sorted_list_pop;
    impl->base.push = sorted_list_push;

    if (sorted_list_init(&impl->base, (uint32_t)&cfg) != 0)
    {
        CONTAINER_ERROR("init failed");
        rt_free(impl);
        return NULL;
    }

    return &impl->base;
}
