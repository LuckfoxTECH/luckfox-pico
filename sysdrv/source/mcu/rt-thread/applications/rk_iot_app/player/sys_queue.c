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

#include "sys_queue.h"

#define DBG_SECTION_NAME        "SYS_QUE"
#include "rkdebug.h"

#define CTRL_MSG_VALIDITY_CHECK 1

int rtos_queue_del(rtos_queue_head_def *data)
{
    if (data == NULL)
    {
        LOGE("%s queue del err(head-NULL)", __func__) ;
        return -1 ;
    }
    if (data->non_empty == 0)
    {
        LOGE("%s queue del err(head-emtpy)", __func__) ;
        return -1 ;
    }
    rtos_queue_def *tmp = data->head ;
    if (data->non_empty == 1)
    {
        data->head = data->head->next ;
        // data->current = data->head->next ;
    }
    else
    {
        data->head = data->head->next ;
    }
    rt_memset(tmp, 0, sizeof(rtos_queue_def)) ;
    tmp->next = NULL ;
    data->end->next = tmp ;
    data->end = data->end->next ;
    data->non_empty-- ;
    return 0 ;;
}

/**
 * Get first node data (FIFO)
 * @param  data msg queue list
 * @return      data want to get
 */
uint32_t rtos_queue_get(rtos_queue_head_def *data)
{
    if (data == NULL)
    {
        LOGD("%s queue get err(head-NULL)", __func__) ;
        return 0 ;
    }
    if (data->non_empty == 0)
    {
        LOGD("%s queue get err(head-emtpy)", __func__) ;
        return 0 ;
    }

    //LOGD("%s(): data = %d\n",__func__,data->head->data);
    return data->head->data ;
}

/**
 * Add data to msg queue
 * @param  head
 * @param  data the data want to add
 * @return      return current node total count  -1 -- add fail
 */
int rtos_queue_add(rtos_queue_head_def *head,  uint32_t data)
{
    if (head == NULL)
    {
        LOGE("%s queue add err(head-NULL)", __func__) ;
        return -1;
    }

    if (head->non_empty == head->node)
    {
        LOGE("%s queue add err(head-FULL)", __func__) ;
        return head->non_empty;
    }

    head->current->flag = 1 ;
    head->current->data = data ;
    head->current = head->current->next ;
    head->non_empty++ ;
    LOGD("head->non_empty:%d....", head->non_empty) ;
    return  head->non_empty ;
}
// typedef struct _rtos_queue_def{
//  uint32_t data ;
//  struct _rtos_queue_def* next ;
// }rtos_queue_def ;

// typedef struct _rtos_queue_head_def{
//  uint8_t node ;
//  uint8_t non_empty ;
//  rtos_queue_def* head ;
//  rtos_queue_def* end ;
// }rtos_queue_head_def ;


/**
 * init the msg queue
 * @param  num the max num of msg queue node
 * @return     the msq queue data
 */
rtos_queue_head_def *rtos_queue_init(uint8_t num)
{
    rtos_queue_head_def *tmp = rt_malloc(sizeof(rtos_queue_head_def)) ;
    if (tmp == NULL)
    {
        LOGE(" %s rt_malloc error", __func__) ;
        return NULL ;
    }
    rt_memset(tmp, 0, sizeof(rtos_queue_head_def)) ;
    tmp->node = num ;
    rtos_queue_def *queue_tmp = NULL ;
    for (uint8_t i = 0; i < num; i++)
    {
        queue_tmp = rt_malloc(sizeof(rtos_queue_def)) ;
        rt_memset(queue_tmp, 0, sizeof(rtos_queue_def)) ;
        queue_tmp->next = NULL ;
        if (tmp->head == NULL)
        {
            tmp->head = queue_tmp ;
            tmp->current = queue_tmp ;
            tmp->end = queue_tmp ;
        }
        else
        {
            tmp->end->next = queue_tmp ;
            tmp->end = queue_tmp ;
        }
        queue_tmp = NULL ;

    }

    return tmp ;
}

/**
 * delete msq queue
 * @param data [description]
 */
void rtos_queue_deinit(rtos_queue_head_def *data)
{
    return ;
}

int rtos_msg_init(rt_mq_t *queue, const char *name, uint32_t queue_len, uint32_t item_sieze)
{
    LOGD("%s() Enter ...", __func__);
#if CTRL_MSG_VALIDITY_CHECK
    if (*queue)
    {
        LOGD("%p message queue already inited", queue);
        return -1;
    }
#endif

    *queue = rt_mq_create(name, item_sieze, queue_len, RT_IPC_FLAG_FIFO);
    if (*queue == RT_NULL)
    {
        LOGD("%s() failed!", __func__);
        return -1;
    }

    LOGD("%s() Out, queue_addr:0x%08x", __func__, *queue);
    return 0;
}

int rtos_msg_deinit(rt_mq_t queue)
{
    rt_err_t ret;

#if CTRL_MSG_VALIDITY_CHECK
    if (queue == RT_NULL)
    {
        LOGE("%s(), invalid queue %p", __func__, queue);
        return 0;
    }
#endif

    ret = rt_mq_delete(queue);
    if (ret != RT_EOK)
    {
        LOGE("%s() failed, err 0x%x", __func__, ret);
        return -1;
    }

    LOGD("%s()\n", __func__);
    return 0;
}

int rtos_msg_send(rt_mq_t queue, const void *item, uint32_t item_size, uint32_t wait_ms)
{
    rt_err_t ret;

#if CTRL_MSG_VALIDITY_CHECK
    if (queue == RT_NULL)
    {
        LOGD("%s(), invalid queue %p", __func__, queue);
        return -1;
    }
#endif

    ret = rt_mq_send(queue, (void *)item, item_size);
    if (ret != RT_EOK)
    {
        LOGE("%s() failed, err 0x%x", __func__, ret);
        return -1;
    }
    return 0;
}

int rtos_msg_recv(rt_mq_t queue, void *item, uint32_t wait_ms)
{
    rt_err_t ret;

#if CTRL_MSG_VALIDITY_CHECK
    if (queue == RT_NULL)
    {
        LOGE("%s(): invalid queue %p", __func__, queue);
        return -1;
    }
#endif

    ret = rt_mq_recv(queue, item, sizeof(item), wait_ms);
    if (ret != RT_EOK)
    {
        return -1;
    }
    return 0;
}

