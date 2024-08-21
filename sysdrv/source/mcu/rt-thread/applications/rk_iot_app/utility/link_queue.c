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

LinkQueue *create_empty_linkqueue()
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
        rt_kprintf("Fail to create empty linkqueue!\n");
        return NULL;
    }
    q->front = head;
    q->rear  = head;

    return q;
}

int is_empty_linkqueue(LinkQueue *q)
{
    return q->front == q->rear ? 1 : 0;
}

int push_linkqueue(LinkQueue *q, datatype data)
{
    LinkNode *tmp;
    tmp = (LinkNode *)rt_malloc(sizeof(LinkNode));
    if (tmp == NULL)
    {
        rt_kprintf("Fail to enter linkqueue!\n");
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

    if (is_empty_linkqueue(q))
    {
        //rt_kprintf("The linkqueue is empty!\n");
        return (datatype) - 1;
    }

    tmp = q->front;
    q->front = tmp->next; ///< pop the head of the list.
    rt_free(tmp);

    return q->front->data;
}

