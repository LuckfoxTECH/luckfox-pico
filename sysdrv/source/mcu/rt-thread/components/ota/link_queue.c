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

#include <rtthread.h>
#include <rtdef.h>
#include <stdio.h>
#include <stdlib.h>

#include "link_queue.h"

#define QUEUE_DBG(fmt, arg...)  rt_kprintf(fmt"\n",##arg)

LinkQueue *create_empty_linkqueue(void)
{
    LinkNode *head;
    LinkQueue *q;

    head = (LinkNode *)rt_malloc(sizeof(LinkNode));
    if (head == NULL)
    {
        rt_kprintf("Fail to create empty linkqueue!\n");
        return NULL;
    }
    head->next = NULL;
    q = (LinkQueue *)rt_malloc(sizeof(LinkQueue));
    if (q == NULL)
    {
        QUEUE_DBG("Fail create LinkQueue\n");
        free(head);
        return NULL;
    }
    q->front = head;
    q->rear  = head;

    return q;
}

void delete_empty_linkqueue(LinkQueue *q)
{
    LinkNode *tmp = NULL;

    if (q == NULL)
    {
        QUEUE_DBG("Invalid queue %d\n", __LINE__);
        return;
    }
    if (q->rear == NULL)
    {
        QUEUE_DBG("Invalid queue->rear %d\n", __LINE__);
        return;
    }
    if (q->front == NULL)
    {
        QUEUE_DBG("Invalid queue->front %d\n", __LINE__);
        return;
    }
    if (!is_empty_linkqueue(q))
    {
        QUEUE_DBG("link equeue is not empty");
        tmp = q->front;
        q->front = tmp->next;
        free(tmp);
    }
    rt_free(q->front);
    rt_free(q);
}

int is_empty_linkqueue(LinkQueue *q)
{
    return q->front == q->rear ? 1 : 0;
}

int push_linkqueue(LinkQueue *q, datatype data)
{
    LinkNode *tmp;

    if (q == NULL)
    {
        QUEUE_DBG("Invalid queue %d\n", __LINE__);
        return -2;
    }
    if (q->rear == NULL)
    {
        QUEUE_DBG("Invalid queue->rear %d\n", __LINE__);
        return -3;
    }
    tmp = (LinkNode *)rt_malloc(sizeof(LinkNode));
    if (tmp == NULL)
    {
        QUEUE_DBG("Fail create LinkNode %d\n", __LINE__);
        return -1;
    }
    tmp->data = data;
    q->rear->next = tmp; ///< insert to the end of the list
    q->rear = tmp;

    return 0;
}

datatype pop_linkqueue(LinkQueue *q)
{
    LinkNode *tmp;

    if (q == NULL)
    {
        QUEUE_DBG("Invalid queue %d\n", __LINE__);
        return (datatype) - 2;
    }
    if (q->rear == NULL)
    {
        QUEUE_DBG("Invalid queue->rear %d\n", __LINE__);
        return (datatype) - 3;
    }
    if (q->front == NULL)
    {
        QUEUE_DBG("Invalid queue->front %d\n", __LINE__);
        return (datatype) - 4;
    }
    if (is_empty_linkqueue(q))
    {
        //QUEUE_DBG("linkqueue empty!");
        return (datatype)(-1);
    }

    tmp = q->front;
    q->front = tmp->next; ///< pop the head of the list.
    rt_free(tmp);

    return q->front->data;
}

