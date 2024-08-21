/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#if defined(RT_USING_RPMSG_CMD)

#include "rpmsg_cmd.h"

static void *rpmsg_cmd_table_callback_find(uint32_t cmd, struct rpmsg_cmd_table_t *table, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < size; i++)
    {
        if (table[i].cmd == cmd)
        {
            return (void *)(table[i].callback);
        }
    }

    return RT_NULL;
}

rt_err_t rpmsg_cmd_send(struct rpmsg_cmd_data_t *p_rpmsg_data, uint32_t timeout)
{
    int32_t  ret;

    RT_ASSERT(p_rpmsg_data);
    RT_ASSERT(p_rpmsg_data->handle);

    struct rpmsg_cmd_head_t *head = &p_rpmsg_data->head;
    struct rpmsg_ept_handle_t *handle = p_rpmsg_data->handle;

#ifdef PRIMARY_CPU
    ret = rpmsg_lite_send(handle->instance, handle->ept,
                          EPT_M2R_ADDR(handle->endpoint),
                          head, sizeof(struct rpmsg_cmd_head_t),
                          timeout);
#else
    ret = rpmsg_lite_send(handle->instance, handle->ept,
                          EPT_R2M_ADDR(handle->endpoint),
                          head, sizeof(struct rpmsg_cmd_head_t),
                          timeout);
#endif

    return ret;
}

static int32_t rpmsg_ept_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    struct rpmsg_cmd_data_t rpmsg_data;
    struct rpmsg_cmd_head_t *head = &rpmsg_data.head;
    struct rpmsg_ept_handle_t *handle = (struct rpmsg_ept_handle_t *)priv;

    RT_ASSERT(payload_len == sizeof(struct rpmsg_cmd_head_t));

    /* Get payload data */
    rt_memcpy(head, payload, sizeof(struct rpmsg_cmd_data_t));

    /* Copy priv & src to rpmsg_data */
    rpmsg_data.handle = handle;
    rpmsg_data.endpoint = src;

    if (head->type == RPMSG_TYPE_DIRECT)
    {
        void (*callback)(void *param);
        callback = rpmsg_cmd_table_callback_find(head->cmd, handle->cmd_table, handle->cmd_counter);
        if (callback) {
            callback(&rpmsg_data);
        }
    }
    else if (head->type == RPMSG_TYPE_URGENT)
    {
        rt_mq_urgent(handle->mq, &rpmsg_data, sizeof(struct rpmsg_cmd_data_t));
    }
    else
    {
        rt_mq_send(handle->mq, &rpmsg_data, sizeof(struct rpmsg_cmd_data_t));
    }

    return RL_RELEASE;
}

static void rpmsg_cmd_ept_thread(void *arg)
{
    struct rpmsg_ept_handle_t *handle = (struct rpmsg_ept_handle_t *)arg;
    struct rpmsg_cmd_data_t rpmsg_data;

    while (1)
    {
        if (rt_mq_recv(handle->mq, &rpmsg_data, sizeof(struct rpmsg_cmd_data_t), RT_WAITING_FOREVER) == RT_EOK)
        {
            struct rpmsg_cmd_head_t *head = &rpmsg_data.head;
            void (*callback)(void *param);
            callback = rpmsg_cmd_table_callback_find(head->cmd, handle->cmd_table, handle->cmd_counter);
            if (callback) {
                callback(&rpmsg_data);
            }
        }
    }
}

void rpmsg_cmd_ept_init(struct rpmsg_ept_handle_t *handle,
                        uint32_t master_id, uint32_t remote_id, uint32_t endpoint,
                        struct rpmsg_cmd_table_t *cmd_table, uint32_t cmd_counter,
                        uint32_t stack_size, uint32_t priority)
{
    char name[32];
    RT_ASSERT(handle);

    handle->master_id   = master_id;
    handle->remote_id   = remote_id;
    handle->endpoint    = endpoint;
    handle->cmd_table   = cmd_table;
    handle->cmd_counter = cmd_counter;

    snprintf(name, sizeof(name), "rpmsg_cmd_%1d_%1d_%3d", handle->master_id, handle->remote_id, handle->endpoint);

#ifdef PRIMARY_CPU
    handle->instance = rpmsg_master_get_instance(handle->master_id, handle->remote_id);
#else
    handle->instance = rpmsg_remote_get_instance(handle->master_id, handle->remote_id);
#endif
    RT_ASSERT(handle->instance);

    handle->ept = rpmsg_lite_create_ept(handle->instance, handle->endpoint, rpmsg_ept_cb, handle);
    RT_ASSERT(handle->ept);

    handle->mq  = rt_mq_create(name, sizeof(struct rpmsg_cmd_data_t), RPMSG_MQ_MAX, RT_IPC_FLAG_FIFO);
    RT_ASSERT(handle->mq);

    handle->thread = rt_thread_create(name, rpmsg_cmd_ept_thread, handle,
                                          stack_size, priority, 10);
    RT_ASSERT(handle->thread);
    rt_thread_startup(handle->thread);
}

void rpmsg_cmd_ept_thread_init(struct rpmsg_ept_handle_t *handle,
                                uint32_t stack_size, uint32_t priority)
{
    char name[32];
    RT_ASSERT(handle);
    RT_ASSERT(handle->master_id == MASTER_ID);
    RT_ASSERT(handle->remote_id != MASTER_ID);
    RT_ASSERT(handle->remote_id <= REMOTE_ID_3);

    snprintf(name, sizeof(name), "rpmsg_cmd_%1d_%1d_%3d", handle->master_id, handle->remote_id, handle->endpoint);

#ifdef PRIMARY_CPU
    handle->instance = rpmsg_master_get_instance(handle->master_id, handle->remote_id);
#else
    handle->instance = rpmsg_remote_get_instance(handle->master_id, handle->remote_id);
#endif
    RT_ASSERT(handle->instance);

    handle->ept = rpmsg_lite_create_ept(handle->instance, handle->endpoint, rpmsg_ept_cb, handle);
    RT_ASSERT(handle->ept);

    handle->mq  = rt_mq_create(name, sizeof(struct rpmsg_cmd_data_t), RPMSG_MQ_MAX, RT_IPC_FLAG_FIFO);
    RT_ASSERT(handle->mq);

    handle->thread = rt_thread_create(name, rpmsg_cmd_ept_thread, handle,
                                          stack_size, priority, 10);
    RT_ASSERT(handle->thread);
    rt_thread_startup(handle->thread);
}
#endif  // RT_USING_RPMSG_LITE
