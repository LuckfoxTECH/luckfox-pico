/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "task_ipc.h"

/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/********************* Public Function Definition ****************************/

IPC_DATA_MEM IPC_DATA_T share_t = { RK_SHMEM_SPINLOCK_ID, 0, 0, 0 };

static uint32_t amp_irq[4] = { AMP0_IRQn, AMP1_IRQn, AMP2_IRQn, AMP3_IRQn };

/* sync cpy power on, wait for cpu0~3 power on */
void amp_sync_poweron(void)
{
    IPC_DATA_T *p_sync = &share_t;
    uint32_t cpu_id, lock_id = p_sync->spinlock_id;
    uint32_t cpu_sta = 0, time_out = 500;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    HAL_SPINLOCK_Lock(lock_id);
    p_sync->cpu_sta |= (0x00000001 << cpu_id) << CPU_POWER_UP_SHIFT;
    HAL_SPINLOCK_Unlock(lock_id);

    do{
        HAL_DelayMs(1);
        if (--time_out == 0) {
            printf("cpu power on fail: cpu_sta = 0x%8x\n", cpu_sta);
            while (1) {
                ;
            }
        }

        HAL_SPINLOCK_Lock(lock_id);
        cpu_sta = p_sync->cpu_sta;
        HAL_SPINLOCK_Unlock(lock_id);
    } while ((cpu_sta & (0x000000f << CPU_POWER_UP_SHIFT)) != (0x000000f << CPU_POWER_UP_SHIFT));
}

static void amp_msg_isr(int vector, void *param)
{
    uint32_t cpu_id;
    uint32_t retval;
    IPC_DATA_T *ipc_dat = (IPC_DATA_T *)param;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    //rk_printf("cpu_id=%ld: master_isr, vector = %d\n", cpu_id, vector);

    if (ipc_dat->rx_cb[cpu_id]) {
        retval = ipc_dat->rx_cb[cpu_id]();
        if (retval != HAL_OK) {
            printf("amp_msg_isr[%d]: callback error!\n", cpu_id);
        }
    }

    HAL_GIC_EndOfInterrupt(vector);
}

HAL_Status amp_msg_init(IPC_DATA_T *ipc_dat, ipc_rx_cb_t callback)
{
    uint32_t cpu_id;
    HAL_Status ret = HAL_OK;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    HAL_IRQ_HANDLER_SetIRQHandler(amp_irq[cpu_id], amp_msg_isr, ipc_dat);
    HAL_GIC_Enable(amp_irq[cpu_id]);

    HAL_SPINLOCK_Lock(ipc_dat->spinlock_id);
    ipc_dat->rx_cb[cpu_id] = callback;
    ipc_dat->cpu_sta |= (0x00000001 << cpu_id) << CPU_INITIALED_SHIFT;
    HAL_SPINLOCK_Unlock(ipc_dat->spinlock_id);

    return ret;
}

HAL_Status amp_msg_send(IPC_DATA_T *ipc_dat, IPC_MSG_T *ipc_msg, uint32_t dst_cpu)
{
    uint32_t cpu_id;
    uint32_t cpu_sta = 0, time_out = 500;
    HAL_Status ret = HAL_ERROR;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    while (1) {
        HAL_SPINLOCK_Lock(ipc_dat->spinlock_id);
        cpu_sta = ipc_dat->cpu_sta;
        if (cpu_sta & ((0x00000001 << dst_cpu) << CPU_INITIALED_SHIFT)) {
            ipc_dat->src_cpu = cpu_id;
            memcpy(&ipc_dat->msg, ipc_msg, sizeof(IPC_MSG_T));
            ret = HAL_OK;
        }
        HAL_SPINLOCK_Unlock(ipc_dat->spinlock_id);

        if (ret == HAL_OK) {
            HAL_GIC_SetPending(amp_irq[dst_cpu]);
            break;
        }

        HAL_DelayMs(1);
        if (--time_out == 0) {
            printf("cpu power on fail: cpu_sta = 0x%8x\n", cpu_sta);
        }
    }

    return ret;
}
