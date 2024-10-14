/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <stdio.h>
#include "dlmodule.h"
#include "dlmodule_test.h"

rt_event_t  dl_event;
RTM_EXPORT(dl_event);

/**
 * This is an example to run dlmodule.
 */
static void dlmodule_example_thread(void *p)
{
    struct rt_dlmodule *module;
    rt_err_t ret;
    rt_uint32_t flag;
    rt_uint32_t event;

    const char *pgname = "/dlmodule/hello.mo";
    const char *pexamp1 = "/dlmodule/example1.mo";
    const char *pexamp2 = "/dlmodule/example2.mo";

    dl_event = rt_event_create("dl_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(dl_event != RT_NULL);

    module = dlmodule_exec(pgname, pgname, rt_strlen(pgname));
    if (module == RT_NULL)
    {
        rt_kprintf("can not find file: %s\n", pgname);
    }

    flag = 0;
    module = dlmodule_exec(pexamp1, pexamp1, rt_strlen(pexamp1));
    if (module == RT_NULL)
    {
        goto exit;
    }

    while (1)
    {
        rt_thread_delay(3000);
        if (flag == 0)
        {
            //stop examp1
            rt_event_send(dl_event, EVENT_DL_EXIT);
            rt_thread_delay(10);

            //wait for examp1 stop
            ret = rt_event_recv(dl_event, EVENT_DL_EXITED,
                                RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                                RT_WAITING_FOREVER, &event);
            RT_ASSERT(ret == RT_EOK);

            //start examp2
            flag = 1;
            module = dlmodule_exec(pexamp2, pexamp2, rt_strlen(pexamp2));
            if (module == RT_NULL)
            {
                break;;
            }
        }
        else
        {
            //stop examp2
            rt_event_send(dl_event, EVENT_DL_EXIT);
            rt_thread_delay(10);

            //wait for examp2 stop
            ret = rt_event_recv(dl_event, EVENT_DL_EXITED,
                                RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                                RT_WAITING_FOREVER, &event);
            RT_ASSERT(ret == RT_EOK);

            //start examp1
            flag = 0;
            module = dlmodule_exec(pexamp1, pexamp1, rt_strlen(pexamp1));
            if (module == RT_NULL)
            {
                break;;
            }
        }
    }

exit:
    rt_event_delete(dl_event);

    rt_kprintf("dl_module test exit!!\n");
}

int dlmodule_example_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("dl_example", dlmodule_example_thread, RT_NULL, 1024, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}
INIT_APP_EXPORT(dlmodule_example_init);
