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

#ifndef _SSV_HCI_H_
#define _SSV_HCI_H_


// #define STA_TXQ_NUM          (9)
#define STA_TXQ_NUM                 (NX_REMOTE_STA_MAX)

/**
* The number of software TX queue. The
* higher queue value has the higher priority. 
* (STA0....STAn) (WIFI_MNG) (BLE_PDU) (WIFI_CMD)
*/
#define SSV_SW_STA_TXQ_NUM          (STA_TXQ_NUM)
#define SSV_SW_WIFI_MNG_TXQ_NUM     (2)
#define SSV_SW_WIFI_CMD_TXQ_NUM     (1)
#define SSV_SW_BLE_PDU_TXQ_NUM      (1)
#define SSV_SW_TXQ_NUM              (SSV_SW_STA_TXQ_NUM+SSV_SW_BLE_PDU_TXQ_NUM+SSV_SW_WIFI_MNG_TXQ_NUM+SSV_SW_WIFI_CMD_TXQ_NUM)
#define SSV_SW_TXQ_ID_MNG0          (SSV_SW_TXQ_NUM-4)  // for host drv_vif_index = 0
#define SSV_SW_TXQ_ID_MNG1          (SSV_SW_TXQ_NUM-3)  // for host drv_vif_index = 1
#define SSV_SW_TXQ_ID_BLE_PDU       (SSV_SW_TXQ_NUM-2)
#define SSV_SW_TXQ_ID_WIFI_CMD      (SSV_SW_TXQ_NUM-1)
#define SSV_SW_TXQ_ID_STAMAX        (SSV_SW_STA_TXQ_NUM-1)
#define SSV_SW_TXQ_ID_MNGMAX        SSV_SW_TXQ_ID_MNG1

#define MAX_RX_CB_NUM 5
typedef int (*proc_rx_cb)(void *app_param, struct sk_buff *skb);

enum ssv_hci_cap {
    HCI_CAP_TX_AGGR = 0,
    HCI_CAP_RX_AGGR = 1,
    HCI_CAP_MAX_NUM = 32, // limited by u32 hci_cap in struct ssv6xxx_hci_ctrl
};

/* struct hci_rx_aggr_info - HCI RX Aggregation Format description */
struct hci_rx_aggr_info {
    u32             jmp_mpdu_len : 16,
                    jmp_mpdu_len1: 16;
    u32             extra_info;
};

/**
* struct ssv_hci_ops - the interface between ssv hci and upper driver.
*
*/
struct ssv6xxx_hci_ops {
    /* public operation */
    int (*hci_start)(void* hci_priv);
    int (*hci_stop)(void* hci_priv);
    int (*hci_ble_start)(void* hci_priv);
    int (*hci_ble_stop)(void* hci_priv);
    int (*hci_read_word)(void* hci_priv, u32 addr, u32 *regval);
    int (*hci_write_word)(void* hci_priv, u32 addr, u32 regval);
    void (*hci_trigger_tx)(void* hci_priv);
    int (*hci_tx)(void* hci_priv, struct sk_buff *, int, bool, u32);
    void (*hci_tx_pause_by_sta)(void* hci_priv, int txqid);
    void (*hci_tx_resume_by_sta)(void* hci_priv, int txqid);
    void (*hci_tx_pause_by_txqid)(void* hci_priv, int txqid);
    void (*hci_tx_resume_by_txqid)(void* hci_priv, int txqid);
    void (*hci_tx_active_by_sta)(void* hci_priv, int txqid);
    void (*hci_tx_inactive_by_sta)(void* hci_priv, int txqid);
    void (*hci_tx_active_by_txqid)(void* hci_priv, int txqid);
    void (*hci_tx_inactive_by_txqid)(void* hci_priv, int txqid);
    int (*hci_ble_txq_flush)(void* hci_priv);
    int (*hci_txq_flush_by_sta)(void* hci_priv, int aid);
    void (*hci_txq_lock_by_sta)(void* hci_priv, int txqid);
    void (*hci_txq_unlock_by_sta)(void* hci_priv, int txqid);
    /**
        * Function provided for query of queue status by upper layer. The
        * parameter maybe one of
        * @ NULL :    indicate all queues
        * @ non-NULL: indicate the specify queue
        */
    int (*hci_txq_len)(void* hci_priv);
    bool (*hci_txq_empty)(void* hci_priv, int txqid);
    int (*hci_set_cap)(void* hci_priv, enum ssv_hci_cap cap, bool enable);
    void (*hci_set_trigger_conf)(void* hci_priv, bool en, u32 qlen);
    void (*hci_set_tx_timestamp)(void* hci_priv, u32 timestamp);
    void (*hci_get_tx_timestamp)(void* hci_priv, u32 *timestamp);
    int (*hci_rx_enqueue)(void* hci_priv, struct sk_buff *skb, bool is_tail);
    /* for debug operation */
    void (*hci_txq_st)(void* hci_priv, int txqid, bool *inactive, bool *paused, int *locked, u32 *qsize, u32 *pkt_cnt);
    void (*hci_rxq_st)(void* hci_priv, u32 *qsize, u32 *pkt_cnt, u32 *post_rx_cnt);

    /* for fw reset operation */
    void (*fw_reset)(void* hci_priv);
};

extern struct ssv6xxx_hci_ops ssv_hci_ops;

int ssv6xxx_hci_deinit(void *hci_priv);
int ssv6xxx_hci_init(void **hci_priv,void* hwif_priv,struct device *dev);

int ssv6xxx_hci_post_tx_register(void* hci_priv, void (*hci_post_tx_cb)(struct sk_buff *, void *), void *args);
int ssv6xxx_check_resource_register(void* hci_priv,
    int (*check_resource_cb)(void *app_param, struct sk_buff_head *qhead, int reserved_page ,int *p_max_count), 
    void *args);
int ssv6xxx_hci_proc_rx_register(void* hci_priv,proc_rx_cb rx_cb, void *args);
int ssv6xxx_hci_proc_rx_unregister(void* hci_priv,proc_rx_cb rx_cb, void *args);


#endif /* _SSV_HCI_H_ */


