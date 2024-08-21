/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-20     mark.huang  the first version
 */
#ifndef _BT_BT_CONTROL_H_
#define _BT_BT_CONTROL_H_

#include "btHwControl.h"
#include "osi/types.h"
#include "osi/semaphore.h"
#include "osi/allocator.h"
#include "osi/task.h"
#include "rk_err.h"
#include "rk_log.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack/bt_types.h"


//extern bt_hw_control_t bt_hw_control;

typedef struct _UART_DEV_ARG
{
    uint32 dwBitRate;
    uint32 dwBitWidth;
    uint32 Channel;
    uint32 stopbit;
    uint32 parity;

} UART_DEV_ARG;

#define CMD_HEAD_LEN 4

typedef struct
{
    int(*power_event_notify)(POWER_NOTIFICATION_t event);
    osi_sem_t cmd_complete_sem;
    uint8 cmd_complete_flag;
    uint8 BTSleepConfigStatus;
    uint8 PowerStatus;
    uint8 needChangeMac;
    struct bd_addr bdaddr;

} _btPcb_;

#endif


