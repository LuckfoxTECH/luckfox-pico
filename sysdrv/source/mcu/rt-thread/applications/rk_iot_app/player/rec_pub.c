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


#define DBG_SECTION_NAME        "REC_PUB"
#include "rkdebug.h"

static event_queue *g_rec_queue = NULL;
static publisher_base *g_rec_publisher = NULL;

static int compare(uint32_t newEvent, uint32_t obsEvent)
{
    if (!CMP_EVENT_TYPE(newEvent, obsEvent) && (EVENT_SUBTYPE(obsEvent) == ALL_SUBTYPE || newEvent == obsEvent))
        return 0;

    //RECPUB_DEBUG("cmp -1, newEvent: 0x%x, obsEvent: 0x%x", newEvent, obsEvent);
    //rt_kprintf("[RECPUB debug] cmp -1, newEvent: 0x%x, obsEvent: 0x%x", newEvent, obsEvent);

    return -1;
}

#define PRJCONF_SYS_CTRL_QUEUE_LEN          6
#define PRJCONF_SYS_CTRL_PRIO               (RT_THREAD_PRIORITY_MAX / 2 - 1)
#define PRJCONF_SYS_CTRL_STACK_SIZE         (2 * 1024)

int recpub_create(void)
{
    uint32_t queue_len = PRJCONF_SYS_CTRL_QUEUE_LEN;

    LOGD("recpub_create Enter ...");
    if (g_rec_queue == NULL)
#if SYS_CTRL_PRIO_QUEUE
        g_rec_queue = prio_event_queue_create(queue_len, sizeof(event_msg));
#else
        g_rec_queue = normal_event_queue_create(queue_len, sizeof(event_msg));
#endif
    if (g_rec_queue == NULL)
    {
        LOGE("create queue failed");
        return -1;
    }

    if (g_rec_publisher == NULL)
        g_rec_publisher = publisher_create(g_rec_queue, compare, PRJCONF_SYS_CTRL_PRIO, PRJCONF_SYS_CTRL_STACK_SIZE);
    if (g_rec_publisher == NULL)
    {
        LOGE("create publisher failed");
        return -1;
    }

    return 0;
}

int recpub_attach(observer_base *obs)
{
    if (g_rec_publisher == NULL)
        return -1;

    return g_rec_publisher ->attach(g_rec_publisher, obs);
}

int recpub_detach(observer_base *obs)
{
    if (g_rec_publisher == NULL)
        return -1;

    return g_rec_publisher->detach(g_rec_publisher, obs);
}
// struct event_msg {
//  void (*execute)(event_msg *msg);
//  void (*destruct)(event_msg *msg);
//  uint32_t    event   /* subtype = 0 means all subtype */;
//  uint32_t    data;
//  uint32_t    extra[0];
// };
static __inline int event_send(event_queue *queue, uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(event_msg *), uint32_t wait_ms)
{
    struct event_msg msg = {NULL, destruct, MK_EVENT(type, subtype), data};
    return queue->send(queue, &msg, wait_ms);
}

/*
static void freeMsgData(uint32_t data)
{
    free((void *)data);
}
*/

int recpub_send(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (g_rec_queue == NULL)
        return -1;

    return event_send(g_rec_queue, type, subtype, data, NULL, wait_ms);
}

/*int sys_event_send_with_free(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (g_sys_queue == NULL)
        return -1;

    return event_send(g_sys_queue, type, subtype, data, freeMsgData, wait_ms);
}*/

int recpub_send_with_destruct(uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(struct event_msg *data), uint32_t wait_ms)
{
    if (g_rec_queue == NULL)
        return -1;

    return event_send(g_rec_queue, type, subtype, data, destruct, wait_ms);
}

