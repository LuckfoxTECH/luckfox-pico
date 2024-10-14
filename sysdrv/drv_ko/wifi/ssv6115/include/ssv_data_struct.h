/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _SSV_DATA_STRUCT_H_
#define _SSV_DATA_STRUCT_H_

#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/types.h>

struct ssv6xxx_queue {
    struct list_head queue;
    spinlock_t lock;
};

struct ssv6xxx_list_node {
    struct list_head list;
};

static inline void ssv6xxx_init_queue(struct ssv6xxx_queue *ssv_queue) {
    INIT_LIST_HEAD(&ssv_queue->queue);
    spin_lock_init(&ssv_queue->lock);
}

static inline void ssv6xxx_init_list_node(struct ssv6xxx_list_node *node) {
    INIT_LIST_HEAD(&node->list);
}

static inline void ssv6xxx_enqueue_list_node(struct ssv6xxx_list_node *node, struct ssv6xxx_queue *ssv_queue) {
    unsigned long flags;

    spin_lock_irqsave(&ssv_queue->lock, flags);

    list_del_init(&node->list);
    list_add_tail(&node->list, &ssv_queue->queue);

    spin_unlock_irqrestore(&ssv_queue->lock, flags);
}

static inline struct ssv6xxx_list_node *ssv6xxx_dequeue_list_node(struct ssv6xxx_queue *ssv_queue) {
    unsigned long flags;
    struct ssv6xxx_list_node *ssv_list_node;

    spin_lock_irqsave(&ssv_queue->lock, flags);

    if (list_empty(&ssv_queue->queue)) {
        ssv_list_node = NULL;
    } else {
        ssv_list_node = container_of((&ssv_queue->queue)->next, struct ssv6xxx_list_node, list);
        list_del_init(&ssv_list_node->list);
    }

    spin_unlock_irqrestore(&ssv_queue->lock, flags);

    return ssv_list_node;
}

#endif /* _SSV_DATA_STRUCT_H_ */

