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

#ifndef _SYS_QUEUE_H_
#define _SYS_QUEUE_H_

#include <rtthread.h>
#include <rtdef.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct _rtos_queue_def
{
    uint8_t flag ;      //  flag of none empty 0 -- empty       ！0 -- none empty
    uint32_t data ;     // to hold addr of data or data
    struct _rtos_queue_def *next ;
} rtos_queue_def ;

typedef struct _rtos_queue_head_def
{
    uint8_t node ;      //  the number of node
    uint8_t non_empty ; //  te number of none empey node
    rtos_queue_def *head ;
    rtos_queue_def *end ;
    rtos_queue_def *current ;
} rtos_queue_head_def ;


void rtos_queue_deinit(rtos_queue_head_def *data) ;
rtos_queue_head_def *rtos_queue_init(uint8_t num)  ;
int rtos_queue_add(rtos_queue_head_def *head,  uint32_t data) ;
uint32_t rtos_queue_get(rtos_queue_head_def *data) ;
int rtos_queue_del(rtos_queue_head_def *data) ;


int rtos_msg_init(rt_mq_t *queue, const char *name, uint32_t queue_len, uint32_t item_sieze);
int rtos_msg_deinit(rt_mq_t queue);
int rtos_msg_send(rt_mq_t queue, const void *item, uint32_t item_size, uint32_t wait_ms);
int rtos_msg_recv(rt_mq_t queue, void *item, uint32_t wait_ms);

#endif  //_SYS_QUEUE_H_