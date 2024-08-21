/******************************************************************************
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author              Notes
 * 2019-05-10     mark.huang && xyp   first implementation
 *
 ******************************************************************************/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "rk_log.h"
#include "bt_app_core.h"
#include "osi/queue.h"
#include "osi/task.h"
#include "osi/thread.h"

static void bt_app_task_handler(void *arg);
static bool bt_app_send_msg(bt_app_msg_t *msg);
static void bt_app_work_dispatched(bt_app_msg_t *msg);

static osi_mq_t s_bt_app_task_queue = NULL;
static osi_task_t s_bt_app_task_handle = NULL;

bool bt_app_work_dispatch(bt_app_cb_t p_cback, uint16_t event, void *p_params, int param_len, bt_app_copy_cb_t p_copy_cback)
{
    RK_LOGD(BT_APP_CORE_TAG, "%s event 0x%x, param len %d", __func__, event, param_len);

    bt_app_msg_t msg;
    memset(&msg, 0, sizeof(bt_app_msg_t));

    msg.sig = BT_APP_SIG_WORK_DISPATCH;
    msg.event = event;
    msg.cb = p_cback;

    if (param_len == 0)
    {
        return bt_app_send_msg(&msg);
    }
    else if (p_params && param_len > 0)
    {
        if ((msg.param = malloc(param_len)) != NULL)
        {
            memcpy(msg.param, p_params, param_len);
            /* check if caller has provided a copy callback to do the deep copy */
            if (p_copy_cback)
            {
                p_copy_cback(&msg, msg.param, p_params);
            }
            return bt_app_send_msg(&msg);
        }
    }

    return false;
}

static bool bt_app_send_msg(bt_app_msg_t *msg)
{
    if (msg == NULL)
    {
        return false;
    }

    if (osi_mq_send(s_bt_app_task_queue, msg, sizeof(bt_app_msg_t), 10) != OSI_EOK)
    {
        RK_LOGE(BT_APP_CORE_TAG, "%s xQueue send failed", __func__);
        return false;
    }
    return true;
}

static void bt_app_work_dispatched(bt_app_msg_t *msg)
{
    if (msg->cb)
    {
        msg->cb(msg->event, msg->param);
    }
}

static void bt_app_task_handler(void *arg)
{
    bt_app_msg_t msg;
    for (;;)
    {
        if (OSI_EOK == osi_mq_recv(s_bt_app_task_queue, &msg, sizeof(bt_app_msg_t), OSI_MAX_TIMEOUT))
        {
            RK_LOGD(BT_APP_CORE_TAG, "%s, sig 0x%x, 0x%x", __func__, msg.sig, msg.event);
            switch (msg.sig)
            {
            case BT_APP_SIG_WORK_DISPATCH:
                bt_app_work_dispatched(&msg);
                break;
            default:
                RK_LOGW(BT_APP_CORE_TAG, "%s, unhandled sig: %d", __func__, msg.sig);
                break;
            } // switch (msg.sig)

            if (msg.param)
            {
                free(msg.param);
            }
        }
    }
}

void bt_app_task_start_up(void)
{
    s_bt_app_task_queue = osi_mq_create("bt_app", sizeof(bt_app_msg_t), 10);
    osi_task_create(bt_app_task_handler, "BtAppT", 3072, NULL, RK_TASK_PRIO_MAX - 3, &s_bt_app_task_handle, 10);
    return;
}

void bt_app_task_shut_down(void)
{
    if (s_bt_app_task_handle)
    {
        osi_task_delete(&s_bt_app_task_handle);
        s_bt_app_task_handle = NULL;
    }
    if (s_bt_app_task_queue)
    {
        osi_mq_delete(&s_bt_app_task_queue);
        s_bt_app_task_queue = NULL;
    }
}
