/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-12     Steven Liu   the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "hal_base.h"
#include "task_ipc.h"

#if defined(RT_USING_COMMON_TEST_IPC) || defined(RT_USING_COMMON_TEST_AMPMSG)

/************************************************/
/*                                              */
/*                AMPMSG_TEST                   */
/*                                              */
/************************************************/

#define AMPMSG_REQ   ((uint32_t)0x55)
#define AMPMSG_ACK   ((uint32_t)0xaa)
#define AMPMSG_DONE  ((uint32_t)0x5a)
#define AMPMSG_ERROR ((uint32_t)-1)

static IPC_DATA_T *p_gshare = &share_t;
static uint32_t master_isr_flag = 0;
static uint32_t remote_isr_flag = 0;

static HAL_Status ampmsg_master_cb(void)
{
    master_isr_flag = 1;
    return HAL_OK;
}

static void ampmsg_master_test(void)
{
    uint32_t src_cpu, dst_cpu = 0;
    IPC_MSG_T pmsg;
    master_isr_flag = 0;
    amp_msg_init(p_gshare, ampmsg_master_cb);
    // only example: init test data
    pmsg.cmd = AMPMSG_REQ;
    pmsg.data[0] = 0x00000055;
    rt_kprintf("master: send req and data 0x%08x to remote!\n", pmsg.data[0]);
    amp_msg_send(p_gshare, &pmsg, dst_cpu);
    while (1)
    {
        if (master_isr_flag)
        {
            master_isr_flag = 0;
            HAL_SPINLOCK_Lock(p_gshare->spinlock_id);
            src_cpu = p_gshare->src_cpu;
            memcpy(&pmsg, &p_gshare->msg, sizeof(IPC_MSG_T));
            HAL_SPINLOCK_Unlock(p_gshare->spinlock_id);
            if (src_cpu == 0)
            {
                // only example to process data
                if (pmsg.cmd == AMPMSG_ACK)
                {
                    if (pmsg.data[0] == 0x0000AA55)
                    {
                        rt_kprintf("master: recv ack and data 0x%08x from remote!\n", pmsg.data[0]);
                        pmsg.cmd = AMPMSG_DONE;
                    }
                    else
                    {
                        rt_kprintf("master: recv error from remote!\n");
                        pmsg.cmd = AMPMSG_ERROR;
                    }
                    amp_msg_send(p_gshare, &pmsg, src_cpu);
                }
            }
            else if (src_cpu == 1)
            {
            }
            else if (src_cpu == 2)
            {
            }
            else     /*if (src_cpu == 3)*/
            {
            }
        }
        rt_thread_delay(100);
        //;
        //asm volatile ("wfi");
        //;
    }
}

static HAL_Status ampmsg_remote_cb(void)
{
    remote_isr_flag = 1;
    return HAL_OK;
}

static void ampmsg_remote_test(void)
{
    uint32_t src_cpu;
    IPC_MSG_T pmsg;
    remote_isr_flag = 0;
    amp_msg_init(p_gshare, ampmsg_remote_cb);
    while (1)
    {
        if (remote_isr_flag)
        {
            remote_isr_flag = 0;
            HAL_SPINLOCK_Lock(p_gshare->spinlock_id);
            src_cpu = p_gshare->src_cpu;
            memcpy(&pmsg, &p_gshare->msg, sizeof(IPC_MSG_T));
            HAL_SPINLOCK_Unlock(p_gshare->spinlock_id);
            if (src_cpu == 1)
            {
                if (pmsg.cmd == AMPMSG_REQ)
                {
                    // only example to process data
                    rt_kprintf("remote: recv req and data 0x%08x from master!\n", pmsg.data[0]);
                    pmsg.cmd = AMPMSG_ACK;
                    pmsg.data[0] |= 0x0000AA00;
                    rt_kprintf("remote: send ack and data 0x%08x to master!\n", pmsg.data[0]);
                    rt_kprintf("src_cpu = %d\n", src_cpu);
                    amp_msg_send(p_gshare, &pmsg, src_cpu);
                }
                else if (pmsg.cmd == AMPMSG_DONE)
                {
                    rt_kprintf("AMPMsg test OK!\n");
                }
                else if (pmsg.cmd == AMPMSG_ERROR)
                {
                    rt_kprintf("AMPMsg test Failed!\n");
                }
                else
                {
                    rt_kprintf("pmsg.cmd error: 0x%8x\n", pmsg.cmd);
                }
            }
            else if (src_cpu == 2)
            {
            }
            else if (src_cpu == 3)
            {
            }
            else     /*if (src_cpu == 0)*/
            {
            }
        }
        rt_thread_delay(100);
        //;
        //asm volatile ("wfi");
        //;
    }
}

void multi_cpu_cowork_test(void)
{
    uint32_t curr_cpu_id;

    while (1)
    {
        HAL_SPINLOCK_Lock(p_gshare->spinlock_id);        // Use spinlock to protect share mem

        curr_cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

        // an example for access share memory
        p_gshare->msg.cmd = curr_cpu_id;     // current cpu id as a flag for example
        p_gshare->msg.data[0]++;              // muti-cpu can access data buffer

        printf("CPU(%d) lockID = %d, flag = %d, data = %d\n", curr_cpu_id, p_gshare->spinlock_id, p_gshare->msg.cmd, p_gshare->msg.data[0]);

        if (curr_cpu_id == 0)
        {
            printf("\n");
        }

        HAL_SPINLOCK_Unlock(p_gshare->spinlock_id);      // Use spinlock to protect share mem
        HAL_DelayMs(500);
    }
}

/********************* Public Function Definition ****************************/
static void ipc_test_thread(void *p)
{
    amp_sync_poweron();

#if defined(RT_USING_COMMON_TEST_AMPMSG)

#if defined(PRIMARY_CPU)
    ampmsg_master_test();
#elif defined(CPU0)
    ampmsg_remote_test();
#endif

#elif defined(RT_USING_COMMON_TEST_IPC)

    multi_cpu_cowork_test();

#endif
}

int ipc_test_thread_init(void)
{
    rt_thread_t thread;
    thread = rt_thread_create("ipc_test", ipc_test_thread, RT_NULL, 2048, 10, 10);
    RT_ASSERT(thread != RT_NULL);
    rt_thread_startup(thread);
    return RT_EOK;
}
INIT_APP_EXPORT(ipc_test_thread_init);

#endif
