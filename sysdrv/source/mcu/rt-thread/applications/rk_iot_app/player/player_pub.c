/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <rtthread.h>
#include <rtdef.h>
#include <rtservice.h>

#include "publisher.h"
#include "observer.h"
#include "event_queue.h"
#include "sys_ctrl.h"
#include "player_pub.h"
// #include "cJSON.h"
#include "globe_player.h"

#define DBG_SECTION_NAME  "PLAYERPUB"
#include "rkdebug.h"

#undef PRJCONF_SYS_CTRL_QUEUE_LEN
#define PRJCONF_SYS_CTRL_QUEUE_LEN          6
#define PRJCONF_SYS_CTRL_PRIO               (RT_THREAD_PRIORITY_MAX / 2 - 1)

static event_queue *g_player_event_queue = NULL;
static publisher_base *g_player_publisher = NULL;

static int compare(uint32_t newEvent, uint32_t obsEvent)
{
    if (!CMP_EVENT_TYPE(newEvent, obsEvent) && (EVENT_SUBTYPE(obsEvent) == ALL_SUBTYPE || newEvent == obsEvent))
        return 0;

    LOGD("cmp -1, newEvent: 0x%x, obsEvent: 0x%x", newEvent, obsEvent);
    return -1;
}

int playerpub_create(void)
{
    uint32_t queue_len = PRJCONF_SYS_CTRL_QUEUE_LEN;

    if (g_player_event_queue  == NULL)
#if SYS_CTRL_PRIO_QUEUE
        g_player_event_queue  = prio_event_queue_create(queue_len, sizeof(event_msg));
#else
        g_player_event_queue  = normal_event_queue_create(queue_len, sizeof(event_msg));
#endif
    if (g_player_event_queue  == NULL)
    {
        LOGE("create queue failed");
        return -1;
    }

    if (g_player_publisher == NULL)
        g_player_publisher = publisher_create(g_player_event_queue, compare, PRJCONF_SYS_CTRL_PRIO, 2048);
    if (g_player_publisher == NULL)
    {
        LOGE("create publisher failed");
        return -1;
    }

    return 0;
}

int playerpub_attach(observer_base *obs)
{
    if (g_player_publisher == NULL)
        return -1;

    return g_player_publisher ->attach(g_player_publisher, obs);
}

int playerpub_detach(observer_base *obs)
{
    if (g_player_publisher == NULL)
        return -1;

    return g_player_publisher->detach(g_player_publisher, obs);
}

static __inline int event_send(event_queue *queue, uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(struct event_msg *data), uint32_t wait_ms)
{
    struct event_msg msg =  {NULL, destruct, MK_EVENT(type, subtype), data};
    return queue->send(queue, &msg, wait_ms);
}

/*
static void freeMsgData(uint32_t data)
{
    free((void *)data);
}
*/

int playerpub_send(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (g_player_event_queue  == NULL)
        return -1;

    return event_send(g_player_event_queue, type, subtype, data, NULL, wait_ms);
}

/*int sys_event_send_with_free(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (g_sys_queue == NULL)
        return -1;

    return event_send(g_sys_queue, type, subtype, data, freeMsgData, wait_ms);
}*/

int playerpub_send_with_destruct(uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(struct event_msg *data), uint32_t wait_ms)
{
    if (g_player_event_queue  == NULL)
        return -1;

    return event_send(g_player_event_queue, type, subtype, data, destruct, wait_ms);
}

