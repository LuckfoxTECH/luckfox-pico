/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>

#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_RPMSG_CMD

#include "rpmsg_cmd.h"

//#define RPMSG_CMD_DEBUG

#if defined(PRIMARY_CPU) /*CPU1*/

void rpmsg_cmd_cpuusage_callback(void *param)
{
    struct rpmsg_cmd_data_t *p_rpmsg_data = (struct rpmsg_cmd_data_t *)param;
    struct rpmsg_ept_handle_t *handle = p_rpmsg_data->handle;
    struct rpmsg_cmd_head_t *head = &p_rpmsg_data->head;
    uint32_t usage;

    usage = *(uint32_t *)head->addr;
#ifdef RPMSG_CMD_DEBUG
    rt_kprintf("cpu%d: cpu usage is %d%\n", handle->remote_id, usage);
#endif

    rt_free_shmem(head->addr);
}

void rpmsg_cmd_cpuusage(void *param)
{
    uint32_t id = *(uint32_t *)param;
    uint32_t usage;

    if (id == HAL_CPU_TOPOLOGY_GetCurrentCpuId())
    {
        usage = rt_cpu_usage_get();
#ifdef RPMSG_CMD_DEBUG
        rt_kprintf("cpu%d: cpu usage is %d%\n", id, usage);
#endif
    }
    else
    {
        rt_err_t ret;
        struct rpmsg_cmd_data_t rpmsg_data;
        struct rpmsg_cmd_head_t *head = &rpmsg_data.head;

        rpmsg_data.handle = rpmsg_cmd_master_get_handle(id);

        head->type = RPMSG_TYPE_DIRECT;
        head->cmd  = RPMSG_CMD_GET_CPU_USAGE;
        head->addr = rt_malloc_shmem(sizeof(uint32_t));
        RT_ASSERT(head->addr != RT_NULL);

        ret = rpmsg_cmd_send(&rpmsg_data, RL_BLOCK);
        RT_ASSERT(ret == RL_SUCCESS);
    }
}

#else

void rpmsg_cmd_cpuusage_callback(void *param)
{
    struct rpmsg_cmd_data_t *p_rpmsg_data = (struct rpmsg_cmd_data_t *)param;
    struct rpmsg_cmd_head_t *head = &p_rpmsg_data->head;

    *(uint32_t *)head->addr = rt_cpu_usage_get();

    head->cmd = RPMSG_CMD2ACK(head->cmd);
    rpmsg_cmd_send(p_rpmsg_data, RL_BLOCK);
}

#endif

#endif
