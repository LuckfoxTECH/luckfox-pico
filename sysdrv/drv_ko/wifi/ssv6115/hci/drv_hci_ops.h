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

#ifndef _DRV_HCI_OPS_H_
#define _DRV_HCI_OPS_H_

#include "hci/ssv_hci.h"

static inline int ssv_drv_hci_start(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_start))
        return -1;

    return hci_ops->hci_start(hci_priv);
}

static inline int ssv_drv_hci_stop(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_stop))
        return -1;

    return hci_ops->hci_stop(hci_priv);
}

static inline int ssv_drv_hci_ble_start(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_ble_start))
        return -1;

    return hci_ops->hci_ble_start(hci_priv);
}

static inline int ssv_drv_hci_ble_stop(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_ble_stop))
        return -1;

    return hci_ops->hci_ble_stop(hci_priv);
}

static inline int ssv_drv_hci_read_word(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, u32 addr, u32 *regval)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_read_word))
        return -1;

    return hci_ops->hci_read_word(hci_priv, addr, regval);
}

static inline int ssv_drv_hci_write_word(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, u32 addr, u32 regval)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_write_word))
        return -1;

    return hci_ops->hci_write_word(hci_priv, addr, regval);
}

static inline void ssv_drv_hci_trigger_tx(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx))
        return;

    hci_ops->hci_trigger_tx(hci_priv);
}

static inline int ssv_drv_hci_tx(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, struct sk_buff *skb, int txqid, bool force_trigger, u32 tx_flags)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx))
        return -1;

    return hci_ops->hci_tx(hci_priv, skb, txqid, force_trigger, tx_flags);
}

/*
 * Only change txq status to pause (set paused variable to true)
 * true: keep packets(do nothing) if inactive variable is false
 */
static inline void ssv_drv_hci_tx_pause_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_pause_by_sta))
        return;

    hci_ops->hci_tx_pause_by_sta(hci_priv, txqid);
}

/*
 * Only change txq status to pause (set paused variable to false)
 * false: process packets if inactive variable is false
 */
static inline void ssv_drv_hci_tx_resume_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_resume_by_sta))
        return;

    hci_ops->hci_tx_resume_by_sta(hci_priv, txqid);
}

/*
 * Only change txq status to pause (set paused variable to true)
 * true: keep packets(do nothing) if inactive variable is false
 */
static inline void ssv_drv_hci_tx_pause_by_txqid(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_pause_by_sta))
        return;

    hci_ops->hci_tx_pause_by_txqid(hci_priv, txqid);
}

/*
 * Only change txq status to pause (set paused variable to false)
 * false: process packets if inactive variable is false
 */
static inline void ssv_drv_hci_tx_resume_by_txqid(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_resume_by_sta))
        return;

    hci_ops->hci_tx_resume_by_txqid(hci_priv, txqid);
}

/*
 * Only change txq status to active (set inactive variable to false)
 * false: Not to drop packets
 */
static inline void ssv_drv_hci_tx_active_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_active_by_sta))
        return;

    hci_ops->hci_tx_active_by_sta(hci_priv, txqid);
}

/*
 * Only change txq status to active (set inactive variable to false)
 * true: Drop all packets in the tx queue
 */
static inline void ssv_drv_hci_tx_inactive_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_inactive_by_sta))
        return;

    hci_ops->hci_tx_inactive_by_sta(hci_priv, txqid);
}

/*
 * Only change txq status to active (set inactive variable to false)
 * false: Not to drop packets
 */
static inline void ssv_drv_hci_tx_active_by_txqid(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_active_by_sta))
        return;

    hci_ops->hci_tx_active_by_txqid(hci_priv, txqid);
}

/*
 * Only change txq status to active (set inactive variable to false)
 * true: Drop all packets in the tx queue
 */
static inline void ssv_drv_hci_tx_inactive_by_txqid(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_tx_inactive_by_sta))
        return;

    hci_ops->hci_tx_inactive_by_txqid(hci_priv, txqid);
}

static inline int ssv_drv_hci_ble_txq_flush(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_ble_txq_flush))
        return -1;

    return hci_ops->hci_ble_txq_flush(hci_priv);
}

static inline int ssv_drv_hci_txq_flush_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_flush_by_sta))
        return -1;

    return hci_ops->hci_txq_flush_by_sta(hci_priv, txqid);
}

static inline void ssv_drv_hci_txq_lock_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_lock_by_sta))
        return;

    hci_ops->hci_txq_lock_by_sta(hci_priv, txqid);
}

static inline void ssv_drv_hci_txq_unlock_by_sta(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_unlock_by_sta))
        return;

    hci_ops->hci_txq_unlock_by_sta(hci_priv, txqid);
}

static inline int ssv_drv_hci_txq_len(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_len))
        return false;

    return hci_ops->hci_txq_len(hci_priv);
}

static inline bool ssv_drv_hci_txq_empty(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_empty))
        return false;

    return hci_ops->hci_txq_empty(hci_priv, txqid);
}

static inline int ssv_drv_hci_set_cap(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, enum ssv_hci_cap cap, bool enable)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_set_cap))
        return -1;

    return hci_ops->hci_set_cap(hci_priv, cap, enable);
}

static inline void ssv_drv_hci_set_trigger_conf(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, bool en, u32 qlen)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_set_cap))
        return;

    hci_ops->hci_set_trigger_conf(hci_priv, en, qlen);
}

static inline int ssv_drv_hci_rx_enqueue(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, struct sk_buff *skb, bool is_tail)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_rx_enqueue))
        return -1;

    return hci_ops->hci_rx_enqueue(hci_priv, skb, is_tail);
}

static inline void ssv_drv_hci_txq_st(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops, int txqid, bool *inactive, bool *paused, int *locked, u32 *qsize, u32 *pkt_cnt)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_st))
        return;

    return hci_ops->hci_txq_st(hci_priv, txqid, inactive, paused, locked, qsize, pkt_cnt);
}

static inline void ssv_drv_hci_fw_reset(void *hci_priv, struct ssv6xxx_hci_ops *hci_ops)
{
    if ((NULL == hci_priv) || (NULL == hci_ops->hci_txq_st))
        return;

    return hci_ops->fw_reset(hci_priv);
}

#endif /* _DRV_HCI_OPS_H_ */


