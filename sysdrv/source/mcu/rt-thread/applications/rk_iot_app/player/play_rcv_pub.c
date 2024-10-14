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

#include "publisher.h"
#include "observer.h"
#include "event_queue.h"
#include "sys_ctrl.h"
#include "play_rcv_pub.h"

#define RCVPUB_DEBUG(fmt, arg...)       rt_kprintf("[RCVPUB debug] <%s, %s() : %d> " fmt "\n",__FILE__,  __func__, __LINE__, ##arg)
#define RCVPUB_ALERT(fmt, arg...)       rt_kprintf("[RCVPUB alert] <%s, %s() : %d> " fmt "\n",__FILE__,  __func__, __LINE__, ##arg)
#define RCVPUB_ERROR(fmt, arg...)       rt_kprintf("[RCVPUB error] <%s, %s() : %d> " fmt "\n", __FILE__, __func__, __LINE__, ##arg)
#define RCVPUB_NOWAY()                  rt_kprintf("[RCVPUB should not be here] <%s, %s() : %d> \n",__FILE__,  __func__, __LINE__)
#define RCVPUB_NOTSUPPORT()             RCVPUB_ALERT("not support command")

static event_queue *player_rcv_queue = NULL;
static publisher_base *player_rcv_publisher = NULL;

static int player_compare(uint32_t newEvent, uint32_t obsEvent)
{
    if (!CMP_EVENT_TYPE(newEvent, obsEvent) && (EVENT_SUBTYPE(obsEvent) == ALL_SUBTYPE || newEvent == obsEvent))
        return 0;

    RCVPUB_DEBUG("cmp -1, newEvent: 0x%x, obsEvent: 0x%x", newEvent, obsEvent);
    return -1;
}

static int event_send(event_queue *queue, uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(struct event_msg *data), uint32_t wait_ms)
{
    struct event_msg msg =  {NULL, destruct, MK_EVENT(type, subtype), data};
    return (queue->send)(queue, &msg, wait_ms);
    //return 0;
}

int player_rcvpub_create(void)
{
    uint32_t queue_len = PRJCONF_SYS_CTRL_QUEUE_LEN;

    if (player_rcv_queue == NULL)
#if SYS_CTRL_PRIO_QUEUE
        player_rcv_queue = prio_event_queue_create(queue_len, sizeof(event_msg));

#else
        player_rcv_queue = normal_event_queue_create(queue_len, sizeof(event_msg));
#endif

    if (player_rcv_queue == NULL)
    {
        RCVPUB_ERROR("create queue failed");
        return -1;
    }

    if (player_rcv_publisher == NULL)
        player_rcv_publisher = publisher_create(player_rcv_queue, player_compare, RT_THREAD_PRIORITY_MAX / 2 - 1, 2048);

    if (player_rcv_publisher == NULL)
    {
        RCVPUB_ERROR("create publisher failed");
        return -1;
    }

    return 0;
}

int player_rcvpub_attach(observer_base *obs)
{
    if (player_rcv_publisher == NULL)
        return -1;

    return player_rcv_publisher ->attach(player_rcv_publisher, obs);
}

int player_rcvpub_detach(observer_base *obs)
{
    if (player_rcv_publisher == NULL)
        return -1;

    return player_rcv_publisher->detach(player_rcv_publisher, obs);
}

/*
static void freeMsgData(uint32_t data)
{
    free((void *)data);
}
*/

int player_rcvpub_send(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (player_rcv_queue == NULL)
        return -1;

    return event_send(player_rcv_queue, type, subtype, data, NULL, wait_ms);
}

/*int sys_event_send_with_free(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (g_sys_queue == NULL)
        return -1;

    return event_send(g_sys_queue, type, subtype, data, freeMsgData, wait_ms);
}*/

int player_rcvpub_send_with_destruct(uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(struct event_msg *data), uint32_t wait_ms)
{
    if (player_rcv_queue == NULL)
        return -1;

    return event_send(player_rcv_queue, type, subtype, data, destruct, wait_ms);
}


