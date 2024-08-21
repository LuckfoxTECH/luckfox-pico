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

#ifndef PRIMARY_CPU

#if defined(CPU0)
#define REMOTE_ID   REMOTE_ID_0
#define MASTER_EPT  EPT_M1R0_SYSCMD
#elif defined(CPU2)
#define REMOTE_ID   REMOTE_ID_2
#define MASTER_EPT  EPT_M1R2_SYSCMD
#elif defined(CPU3)
#define REMOTE_ID   REMOTE_ID_3
#define MASTER_EPT  EPT_M1R3_SYSCMD
#else
#error "error: Undefined CPU id!"
#endif
#define REMOTE_EPT EPT_M2R_ADDR(MASTER_EPT)

static struct rpmsg_ept_handle_t rpmsg_cmd_handle;

static struct rpmsg_cmd_table_t rpmsg_cmd_table[] =
{
    { RPMSG_CMD_GET_CPU_USAGE, rpmsg_cmd_cpuusage_callback},
};

struct rpmsg_ept_handle_t *rpmsg_cmd_remote_get_handle(void)
{
    return &rpmsg_cmd_handle;
}

/*
 * Initial epts:
 */
int rpmsg_cmd_remote_init(void)
{
    /* Use rpmsg_cmd_ept_init() initial ept & creat thread */
    rpmsg_cmd_ept_init(&rpmsg_cmd_handle,
                        MASTER_ID, REMOTE_ID, REMOTE_EPT,
                        rpmsg_cmd_table,
                        sizeof(rpmsg_cmd_table) / sizeof(struct rpmsg_cmd_table_t),
                        2048, RT_THREAD_PRIORITY_MAX / 2);
}
INIT_APP_EXPORT(rpmsg_cmd_remote_init);

#endif

#endif  // RT_USING_RPMSG_LITE
