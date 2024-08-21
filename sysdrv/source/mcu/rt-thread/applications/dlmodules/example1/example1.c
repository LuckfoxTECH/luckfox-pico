/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <stdio.h>
#include "dlmodule_test.h"

SECTION(".param") rt_uint16_t dlmodule_thread_priority = 5;
SECTION(".param") rt_uint32_t dlmodule_thread_stacksize = 1024;

int main(int argc, char *argv[])
{
    rt_err_t ret;
    rt_uint32_t event;

    rt_kprintf("------dlmodule: example1 start------\n\n");

    while (1)
    {
        rt_kprintf("dlmodule: example1 run.\n\n");

        ret = rt_event_recv(dl_event, EVENT_DL_EXIT,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            500, &event);
        if (ret == RT_EOK)
        {
            if (event & EVENT_DL_EXIT)
            {
                break;
            }
        }
    }

    rt_event_send(dl_event, EVENT_DL_EXITED);

    return RT_EOK;
}