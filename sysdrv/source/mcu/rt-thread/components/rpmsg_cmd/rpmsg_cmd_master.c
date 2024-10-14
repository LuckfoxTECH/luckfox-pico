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

#if defined(PRIMARY_CPU)

static struct rpmsg_ept_handle_t rpmsg_cmd_handles[3];

static struct rpmsg_cmd_table_t rpmsg_cmd_table[] =
{
    { RPMSG_ACK_GET_CPU_USAGE, rpmsg_cmd_cpuusage_callback},
};

/*
 * Get ept handle by remote cpu id
 */
struct rpmsg_ept_handle_t *rpmsg_cmd_master_get_handle(uint32_t remote_id)
{
    if (remote_id == REMOTE_ID_0) {
        return &rpmsg_cmd_handles[0];
    }
    else if (remote_id == REMOTE_ID_2) {
        return &rpmsg_cmd_handles[1];
    }
    else if (remote_id == REMOTE_ID_3) {
        return &rpmsg_cmd_handles[2];
    }

    return RT_NULL;
}

/*
 * Initial epts:
 */
int rpmsg_cmd_master_init(void)
{
    struct rpmsg_ept_handle_t *handle;

    /* Init ept0: master(1) --> remote(0) */
    handle = &rpmsg_cmd_handles[0];
    handle->master_id = MASTER_ID;
    handle->remote_id = REMOTE_ID_0;
    handle->endpoint  = EPT_M1R0_SYSCMD;
    handle->cmd_counter = sizeof(rpmsg_cmd_table) / sizeof(struct rpmsg_cmd_table_t);
    handle->cmd_table = rpmsg_cmd_table;
    rpmsg_cmd_ept_thread_init(handle, 2048, RT_THREAD_PRIORITY_MAX / 2);

    /* Init ept1: master(1) --> remote(2) */
    handle = &rpmsg_cmd_handles[1];
    handle->master_id = MASTER_ID;
    handle->remote_id = REMOTE_ID_2;
    handle->endpoint  = EPT_M1R2_SYSCMD;
    handle->cmd_counter = sizeof(rpmsg_cmd_table) / sizeof(struct rpmsg_cmd_table_t);
    handle->cmd_table = rpmsg_cmd_table;
    rpmsg_cmd_ept_thread_init(handle, 2048, RT_THREAD_PRIORITY_MAX / 2);

    /* Init ept2: master(1) --> remote(3) */
    handle = &rpmsg_cmd_handles[2];
    handle->master_id = MASTER_ID;
    handle->remote_id = REMOTE_ID_3;
    handle->endpoint  = EPT_M1R3_SYSCMD;
    handle->cmd_counter = sizeof(rpmsg_cmd_table) / sizeof(struct rpmsg_cmd_table_t);
    handle->cmd_table = rpmsg_cmd_table;
    rpmsg_cmd_ept_thread_init(handle, 2048, RT_THREAD_PRIORITY_MAX / 2);
}
INIT_APP_EXPORT(rpmsg_cmd_master_init);

static void rpmsg_cmd_usage(void)
{
    rt_kprintf("\nusage: rpmsg_cmd <cmd> [0|1|2|3] ...\n");
    rt_kprintf("\n");
    rt_kprintf("   <cmd>      <comments>       \n");
    rt_kprintf("  cpuusage -- get cpu usage n% \n");
    rt_kprintf("\n");
}

static void rpmsg_cmd(int argc, char *argv[])
{
    char *cmd;

    if (argc > 1)
    {
        cmd = argv[1];
        if (!rt_strcmp(cmd, "cpuusage"))
        {
            if (argc > 2)
            {
                cmd = argv[2];
                uint32_t id = atoi(cmd);
                if (id < PLATFORM_CORE_COUNT)
                {
                    rpmsg_cmd_cpuusage(&id);
                }
                else
                {
                    rt_kprintf("\n");
                    rt_kprintf("example: rpmsg_cmd cpuusage [0|1|2|3]\n");
                    rt_kprintf("\n");
                }
            }
            else
            {
                for (uint32_t id = 0; id < PLATFORM_CORE_COUNT; id++)
                {
                    rpmsg_cmd_cpuusage(&id);
                }
            }
        }
        else
        {
            rpmsg_cmd_usage();
        }
    }
    else
    {
        rpmsg_cmd_usage();
    }

    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(rpmsg_cmd, rpmsg_cmd);
#endif

#endif

#endif  // RT_USING_RPMSG_LITE
