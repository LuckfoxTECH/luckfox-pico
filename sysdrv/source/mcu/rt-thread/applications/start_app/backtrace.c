/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    backtrace.c
  * @version V0.1
  * @brief   backtrace interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-04-01     Cliff.Chen      first implementation
  *
  ******************************************************************************
  */

#include <rtthread.h>
#include <cmsis_gcc.h>
#include <backtrace.h>

#ifdef RT_USING_CMBACKTRACE
#include "cm_backtrace.h"

void dump_call_stack(void)
{
    uint32_t call_stack[16] = {0};
    size_t i, depth = 0;

    rt_kprintf("=============call stack list===============\n");
    depth = cm_backtrace_call_stack(call_stack, sizeof(call_stack), cmb_get_sp(), 0, 0);

    rt_kprintf("Show more call stack info by run: addr2line -e rtthread.elf -a -f ");
    for (i = 0; i < depth; i++)
    {
        rt_kprintf("%08x ", call_stack[i]);
    }
}
#endif
