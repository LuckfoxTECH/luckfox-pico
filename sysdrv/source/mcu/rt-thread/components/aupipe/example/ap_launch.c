/**
  * Copyright (c) 2023 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    ap_launch.c
  * @author  Jun Zeng
  * @version V0.1
  * @date    17-02-2023
  * @brief   vehicle audio
  *
  ******************************************************************************
  */

#include "aupipe.h"

static Aupipe *aupipe = NULL;

void ap_launch_aupipe(void *arg)
{
    int ret;
    ApEventMsg *msg;

    aupipe_list_object_template();

    aupipe = aupipe_new();
    if (!aupipe)
        goto null_aupipe;

    aupipe_create_link(aupipe, arg);

    aupipe_set_state(aupipe, AUPIPE_PLAYING);

    msg = aupipe_wait_event(aupipe);
    if (msg->need_free)
        rt_free(msg);

    aupipe_set_state(aupipe, AUPIPE_NULL);

   aupipe_delete_link(aupipe);
null_aupipe:
    aupipe = NULL;
    rt_free(arg);

    return;
}

int ap_launch(int argc, char *argv[])
{
    rt_thread_t tid;

    if (!aupipe)
    {
        tid = rt_thread_create("ap_launch", ap_launch_aupipe, strdup(argv[1]),
                               4096, RT_THREAD_PRIORITY_MAX / 2, 10);
        if (tid)
            rt_thread_startup(tid);
    }

    return RT_EOK;
}

int ap_launch_set(int argc, char *argv[])
{
    int status;
    ApList *list;
    ApObject *obj;

    if (argc != 4)
    {
        rt_kprintf("Usage: ap_launch_set | object name | property name | string\n");
        return -RT_ERROR;
    }

    list = &aupipe->objs;

    while(1)
    {
        list = list->last;

        if (list == &aupipe->objs)
        {
            LOG_E("%s object is not found", argv[1]);
            return -RT_ERROR;
        }

        obj = (ApObject *)list->data;

        if (!strcmp(obj->name, argv[1]))
        {
            status = obj->set_property(obj, argv[2], argv[3]);

            if (status == RT_EOK)
                LOG_I("%s set property success", obj->name);
            else
                LOG_E("%s set property fail", obj->name);
            break;
        }
    }

    return RT_EOK;
}

int ap_launch_quit(int argc, char *argv[])
{
    ApEventMsg *msg;

    if (aupipe)
    {
        msg = rt_malloc(sizeof(ApEventMsg));
        if (!msg)
            return -RT_ERROR;
        msg->type = EVENT_TYPE_STOP;
        msg->need_free = 1;
        if (aupipe_new_event(aupipe, msg))
        {
            LOG_E("Stop failed\n");
            free(msg);
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(ap_launch, ap_launch);
MSH_CMD_EXPORT(ap_launch_set, ap_launch_set);
MSH_CMD_EXPORT(ap_launch_quit, quit ap_launch);
#endif

