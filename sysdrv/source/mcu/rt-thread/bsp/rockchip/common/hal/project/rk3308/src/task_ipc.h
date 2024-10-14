/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#ifndef _TASK_IPC_H_
#define _TASK_IPC_H_

//#define IPC_ENABLE

/* spinlock ID define */
#define RK_PRINTF_SPINLOCK_ID 0U
#define RK_SHMEM_SPINLOCK_ID  1U

/* IPC memory section define */
#define IPC_DATA_MEM __attribute__((section(".share_data")))

#define CPU_POWER_UP_SHIFT  0U
#define CPU_INITIALED_SHIFT 4U

typedef HAL_Status (*ipc_rx_cb_t)(void);

/* IPC data struct define */
struct IPC_MSG {
    uint32_t cmd;           // ipc cmd
    uint32_t data[1];       // ipc data
};
typedef struct IPC_MSG IPC_MSG_T;

/* IPC data struct define */
struct IPC_DATA {
    uint32_t spinlock_id;   // for spinlock ID
    uint32_t cpu_sta;       // cpu power up flag
    uint32_t src_cpu;       // source cpu which send msg
    ipc_rx_cb_t rx_cb[4];   // cpu 0~3 rx isr callback
    struct IPC_MSG msg;
};
typedef struct IPC_DATA IPC_DATA_T;

/* Public function declaration */
extern IPC_DATA_MEM IPC_DATA_T share_t;

void amp_sync_poweron(void);
HAL_Status amp_msg_init(IPC_DATA_T *ipc_dat, ipc_rx_cb_t callback);
HAL_Status amp_msg_send(IPC_DATA_T *ipc_dat, IPC_MSG_T *ipc_msg, uint32_t dst_cpu);

#endif
