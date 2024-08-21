/******************************************************************************
 *
 *  Copyright (C) 2015 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "osi/types.h"

#define OSI_MQ_MAX_TIMEOUT OSI_MAX_TIMEOUT

typedef rt_mq_t osi_mq_t;

#define osi_mq_valid( x ) ( ( ( *x ) == NULL) ? RT_FALSE : RT_EOK )
#define osi_mq_set_invalid( x ) ( ( *x ) = NULL )

osi_mq_t osi_mq_create(const char *name, uint32_t msg_size, uint32_t max_msgs);

osi_mq_t osi_mq_static_create(osi_mq_t *queue, const char *name, void *msgpool, uint32_t msg_size, uint32_t pool_size);

osi_err_t osi_mq_send(osi_mq_t queue, void *buffer, uint32_t size, uint32_t timeout);

int osi_mq_recv(osi_mq_t queue, void *buffer, uint32_t size, uint32_t timeout);

void osi_mq_delete(osi_mq_t *queue);

void osi_mq_detach(osi_mq_t *queue);

uint16_t osi_mq_entry_get(osi_mq_t queue);

uint16_t osi_mq_max_msgs_get(osi_mq_t queue);

#endif /* __QUEUE_H__ */
