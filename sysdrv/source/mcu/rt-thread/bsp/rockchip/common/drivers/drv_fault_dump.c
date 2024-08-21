/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_fault_dump.c
  * @version V0.1
  * @brief   fault dump hook
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-10-09     Cliff.Chen      first implementation
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_USNING_FAULT_DUMP_HOOK
#include <stdio.h>
#include <cm_backtrace.h>
#include "drv_fault_dump.h"

#ifndef RT_FAULT_DUMP_BUFFER_SIZE
#define RT_FAULT_DUMP_BUFFER_SIZE                  (4 * 1024)
#endif

struct fault_dump_buffer
{
    char buff[RT_FAULT_DUMP_BUFFER_SIZE];
    unsigned int index;
};

static struct fault_dump_buffer fault_dump;

static void fault_println_hook(const char *str)
{
    int n, left;

    n = rt_strlen(str);
    left = RT_FAULT_DUMP_BUFFER_SIZE - fault_dump.index;
    if (n > 0 && fault_dump.buff && left > n)
    {
        rt_memcpy(&fault_dump.buff[fault_dump.index], str, n);
        fault_dump.index += n;
    }
}

static void dump_end_hook(void)
{
    // send maibox to ap
}

void fault_dump_hook_init(void)
{
    rt_kprintf("fault dump buffer: %p\n", fault_dump.buff);
    fault_dump.index = 0;

    cmb_set_hook(fault_println_hook, dump_end_hook);
}
#endif
