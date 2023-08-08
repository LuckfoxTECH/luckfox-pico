/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Ë«ÏòÁ´±í
 * Author: Hisilicon
 * Create: 2020-09-19
 */
#ifndef __HCC_LIST_H__
#define __HCC_LIST_H__

#include <hi_types_base.h>
#include "osal_list.h"

#define oal_unc_data(_pst_buf)                   ((_pst_buf)->buf)
#define oal_unc_priv(_pst_buf)                   ((_pst_buf)->priv)
#define oal_unc_priv_type(_pst_buf)              ((_pst_buf)->priv_type)
#define oal_unc_len(_pst_buf)                    ((_pst_buf)->length)
#define oal_unc_free(_pst_buf)                    ((_pst_buf)->free)

static inline hi_bool hcc_is_list_empty(const hcc_data_queue *hcc_queue)
{
    return osal_list_empty(&hcc_queue->data_queue);
}

static inline hi_u32 hcc_list_len(const hcc_data_queue *hcc_queue)
{
    return hcc_queue->qlen;
}

static inline hi_void hcc_list_add_tail(hcc_data_queue *hcc_queue,
    hcc_unc_struc *unc_buf)
{
    unsigned long flags;
    oal_spin_lock_irq_save(&hcc_queue->data_queue_lock, &flags);
    osal_list_add_tail(&unc_buf->list, &hcc_queue->data_queue);
    hcc_queue->qlen++;
    oal_spin_unlock_irq_restore(&hcc_queue->data_queue_lock, &flags);
}

static inline hi_void hcc_list_head_init(hcc_data_queue *hcc_queue)
{
    oal_spin_lock_init(&hcc_queue->data_queue_lock);
    osal_init_list_head(&hcc_queue->data_queue);
    hcc_queue->qlen = 0;
    hcc_queue->flow_flag = HI_FALSE;
}

static inline hi_void _hcc_list_unlink(struct osal_list_head *node, hcc_data_queue *hcc_queue)
{
    struct osal_list_head *next = NULL;
    struct osal_list_head *prev = NULL;

    hcc_queue->qlen--;
    next       = node->next;
    prev       = node->prev;
    node->next  = node->prev = NULL;
    next->prev = prev;
    prev->next = next;
}

static inline struct osal_list_head *hcc_list_peek(hcc_data_queue *hcc_queue)
{
    struct osal_list_head *list = hcc_queue->data_queue.next;
    if (list == &hcc_queue->data_queue) {
        list = NULL;
    }
    return list;
}

static inline struct osal_list_head *hcc_list_peek_tail(const hcc_data_queue* hcc_queue)
{
    struct osal_list_head *node = hcc_queue->data_queue.prev;
    if (node == (struct osal_list_head *)&hcc_queue->data_queue) {
        node = NULL;
    }
    return node;
}

static inline struct osal_list_head *_hcc_list_dequeue(hcc_data_queue *hcc_queue)
{
    struct osal_list_head *node = hcc_list_peek(hcc_queue);
    if (node) {
        _hcc_list_unlink(node, hcc_queue);
    }
    return node;
}

static inline hcc_unc_struc *hcc_list_dequeue(hcc_data_queue *hcc_queue)
{
    unsigned long flags;
    hcc_unc_struc *unc_buf = HI_NULL;
    struct osal_list_head *node;
    oal_spin_lock_irq_save(&hcc_queue->data_queue_lock, &flags);
    node = _hcc_list_dequeue(hcc_queue);
    if (node != HI_NULL) {
        unc_buf = osal_hlist_entry(node, hcc_unc_struc, list);
    }
    oal_spin_unlock_irq_restore(&hcc_queue->data_queue_lock, &flags);
    return unc_buf;
}

static inline hcc_unc_struc *hcc_list_delist_tail(hcc_data_queue* hcc_queue)
{
    hcc_unc_struc *unc_buf = HI_NULL;
    unsigned long flags;
    oal_spin_lock_irq_save(&hcc_queue->data_queue_lock, &flags);
    struct osal_list_head *node = hcc_list_peek_tail(hcc_queue);
    if (node) {
        _hcc_list_unlink(node, hcc_queue);
        unc_buf = osal_hlist_entry(node, hcc_unc_struc, list);
    }
    oal_spin_unlock_irq_restore(&hcc_queue->data_queue_lock, &flags);
    return unc_buf;
}

static inline void _hcc_list_add_head(struct osal_list_head *n, struct osal_list_head *h)
{
    n->next = h->next;
    n->prev = h;
    h->next->prev = n;
    h->next = n;
}

static inline hi_void hcc_list_add_head(hcc_data_queue* hcc_queue, hcc_unc_struc *unc_buf)
{
    unsigned long flags;
    oal_spin_lock_irq_save(&hcc_queue->data_queue_lock, &flags);
    _hcc_list_add_head(&unc_buf->list, &hcc_queue->data_queue);
    hcc_queue->qlen++;
    oal_spin_unlock_irq_restore(&hcc_queue->data_queue_lock, &flags);
}

static inline hi_void hcc_list_splice_sync(hcc_data_queue* hcc_queue, hcc_data_queue* head)
{
    hcc_unc_struc *unc_buf = HI_NULL;
    for (;;) {
        unc_buf = hcc_list_delist_tail(head);
        if (unc_buf == NULL) {
            break;
        }
        hcc_list_add_head(hcc_queue, unc_buf);
    }
}

static inline hi_void _hcc_list_splice(const struct osal_list_head *list,
    struct osal_list_head *prev,
    struct osal_list_head *next)
{
    struct osal_list_head *first = list->next;
    struct osal_list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

static inline hi_void hcc_list_splice_init(hcc_data_queue *hcc_queue, hcc_data_queue *head)
{
    if (!hcc_is_list_empty(hcc_queue)) {
        _hcc_list_splice(&hcc_queue->data_queue, &head->data_queue, head->data_queue.next);
        head->qlen += hcc_queue->qlen;
        hcc_list_head_init(hcc_queue);
    }
}

static inline void hcc_list_purge(hcc_data_queue *hcc_queue)
{
    hcc_unc_struc *unc_buf = HI_NULL;
    while ((unc_buf = hcc_list_dequeue(hcc_queue)) != NULL) {
        if (unc_buf->free) {
            unc_buf->free((hi_void*)unc_buf);
        }
    }
}

#endif  /* __HI_STDLIB_H__ */
