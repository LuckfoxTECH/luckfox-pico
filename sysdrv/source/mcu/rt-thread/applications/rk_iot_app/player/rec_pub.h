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

#ifndef RECPUB_H_
#define RECPUB_H_

#include <stdlib.h>
// #include "common/framework/sys_ctrl/publisher.h"
// #include "common/framework/sys_ctrl/observer.h"
// #include "common/framework/sys_ctrl/event_queue.h"
#include "publisher.h"
#include "observer.h"
#include "event_queue.h"

#define RECPUB_PRIO_QUEUE (0)

#define ALL_SUBTYPE (0xFFFF)

typedef enum recpub_msg_type
{
    RECPUB_MSG_TYPE_RECORD,
} rec_msg_type;

typedef enum recpub_msg_subtype
{
    RECPUB_MSG_SUB_TYPE_ALL = ALL_SUBTYPE,
} rec_msg_subtype;

/** @brief Get type from event */
#define EVENT_TYPE(event)  ((uint16_t)((event) >> 16))

/** @brief Get subtype from event */
#define EVENT_SUBTYPE(event)  ((uint16_t)((event) & 0xFFFF))

/** @brief Make event from type and subtype */
#define MK_EVENT(type, subtype) \
    (((uint32_t)(type) << 16) | ((uint32_t)(subtype) & 0xFFFF))

#define CMP_EVENT_TYPE(event1, event2)  ((event1 ^ event2) & 0xFFFF0000)


static __inline observer_base *recpub_callback_observer_create(uint16_t type,
        uint16_t subtype,
        void (*cb)(uint32_t event, uint32_t data, void *arg),
        void *arg)
{
    return callback_observer_create(MK_EVENT(type, subtype), cb, arg);
}

int recpub_create(void);

int recpub_attach(observer_base *obs);

int recpub_detach(observer_base *obs);

int recpub_send(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms);

int recpub_send_with_destruct(uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(struct event_msg data), uint32_t wait_ms);

#define recpub_send_with_free(type, subtype, data, wait_ms) \
    recpub_send_with_destruct(type, subtype, data, (void (*)(struct event_msg))free, wait_ms)


#endif /* RECPUB_H_ */

