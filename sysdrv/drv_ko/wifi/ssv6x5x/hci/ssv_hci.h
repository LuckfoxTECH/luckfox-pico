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

#define SSV_SC(_ctrl_hci)      (_ctrl_hci->shi->sc)
#define SSV_SH(_ctrl_hci)      (_ctrl_hci->shi->sh)

#define TX_PAGE_NOT_LIMITED 255

/**
* The number of SSV6200 software TX queue. The
* higher queue value has the higher priority. 
* (STA0....STAn) (WIFI_MNG) (BLE_PDU) (WIFI_CMD)
*/
#define SSV_SW_STA_TXQ_NUM          (9)
#define SSV_SW_WIFI_MNG_TXQ_NUM     (1)
#define SSV_SW_WIFI_CMD_TXQ_NUM     (1)
#define SSV_SW_BLE_PDU_TXQ_NUM      (1)
#define SSV_SW_TXQ_NUM              (SSV_SW_STA_TXQ_NUM+SSV_SW_BLE_PDU_TXQ_NUM+SSV_SW_WIFI_MNG_TXQ_NUM+SSV_SW_WIFI_CMD_TXQ_NUM)
#define SSV_SW_TXQ_ID_WIFI_CMD      (SSV_SW_TXQ_NUM-1)
#define SSV_SW_TXQ_ID_BLE_PDU       (SSV_SW_TXQ_NUM-2)
#define SSV_SW_TXQ_ID_MNG           (SSV_SW_TXQ_NUM-3)
#define SSV_SW_TXQ_ID_STAMAX        (SSV_SW_STA_TXQ_NUM-1)

/**
* The number of SSV6200 hardware TX queue. The
* higher queue value has the higher priority. 
* (BK) (BE) (VI) (VO) (MNG)
*/
#define SSV_HW_TXQ_NUM              5

/* 
 * hardware tx resource configuration
 */
#define SSV6XXX_ID_TX_THRESHOLD(_hctl)				((_hctl)->tx_info.tx_id_threshold)
#define SSV6XXX_PAGE_TX_THRESHOLD(_hctl)			((_hctl)->tx_info.tx_page_threshold)
#define SSV6XXX_TX_LOWTHRESHOLD_ID_TRIGGER(_hctl)	((_hctl)->tx_info.tx_lowthreshold_id_trigger)
#define SSV6XXX_TX_LOWTHRESHOLD_PAGE_TRIGGER(_hctl)	((_hctl)->tx_info.tx_lowthreshold_page_trigger)
#define SSV6XXX_ID_AC_BK_OUT_QUEUE(_hctl)			((_hctl)->tx_info.bk_txq_size)
#define SSV6XXX_ID_AC_BE_OUT_QUEUE(_hctl)			((_hctl)->tx_info.be_txq_size)
#define SSV6XXX_ID_AC_VI_OUT_QUEUE(_hctl)			((_hctl)->tx_info.vi_txq_size)
#define SSV6XXX_ID_AC_VO_OUT_QUEUE(_hctl)			((_hctl)->tx_info.vo_txq_size)
#define SSV6XXX_ID_MANAGER_QUEUE(_hctl)				((_hctl)->tx_info.manage_txq_size)
#define SSV6XXX_ID_USB_AC_BK_OUT_QUEUE(_hctl)			((_hctl)->shi->sh->cfg.bk_txq_size)
#define SSV6XXX_ID_USB_AC_BE_OUT_QUEUE(_hctl)			((_hctl)->shi->sh->cfg.be_txq_size)
#define SSV6XXX_ID_USB_AC_VI_OUT_QUEUE(_hctl)			((_hctl)->shi->sh->cfg.vi_txq_size)
#define SSV6XXX_ID_USB_AC_VO_OUT_QUEUE(_hctl)			((_hctl)->shi->sh->cfg.vo_txq_size)
#define SSV6XXX_ID_USB_MANAGER_QUEUE(_hctl)			((_hctl)->shi->sh->cfg.manage_txq_size)
#define SSV6XXX_ID_HCI_INPUT_QUEUE  				8


/**
* Define flags for enqueue API 
*/
#define HCI_FLAGS_ENQUEUE_HEAD	   0x00000001
#define HCI_FLAGS_NO_FLOWCTRL	   0x00000002


#define HCI_DBG_PRINT(_hci_ctrl, fmt, ...) \
    do { \
        (_hci_ctrl)->shi->dbgprint((_hci_ctrl)->shi->sc, LOG_HCI, fmt, ##__VA_ARGS__); \
    } while (0)

enum ssv_hci_cap {
    HCI_CAP_TX_AGGR = 0,
    HCI_CAP_MAX_NUM = 32, // limited by u32 hci_cap in struct ssv6xxx_hci_ctrl
};


/**
* struct ssv_sw_txq - ssv6200 software tx queue.
*/
struct ssv_sw_txq {
    u32 txq_no;
    struct mutex txq_lock;
    struct sk_buff_head qhead;
    bool paused;

    /* statistic counters: */
    u32 tx_pkt;
    u32 tx_flags;
};


struct ssv6xxx_hci_ctrl;
/**
* struct ssv_hci_ops - the interface between ssv hci and upper driver.
*
*/
struct ssv6xxx_hci_ops {

//    int (*hci_irq_enable)(void);
//    int (*hci_irq_disable)(void);
    int (*hci_start)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_stop)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_hcmd_start)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_ble_start)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_ble_stop)(struct ssv6xxx_hci_ctrl *hctrl);
#ifdef CONFIG_PM
    int (*hci_suspend)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_resume)(struct ssv6xxx_hci_ctrl *hctrl);
#endif
    int (*hci_read_word)(struct ssv6xxx_hci_ctrl *hctrl, u32 addr, u32 *regval);
    int (*hci_write_word)(struct ssv6xxx_hci_ctrl *hctrl, u32 addr, u32 regval);
#ifdef CONFIG_USB_EP0_RW_REGISTER
    int (*hci_mcu_read_word)(struct ssv6xxx_hci_ctrl *hctrl, u32 addr, u32 *regval);
    int (*hci_mcu_write_word)(struct ssv6xxx_hci_ctrl *hctrl, u32 addr, u32 regval);
#endif
    int (*hci_burst_read_word)(struct ssv6xxx_hci_ctrl *hctrl, u32 *addr, u32 *regval, u8 reg_amount);
    int (*hci_burst_write_word)(struct ssv6xxx_hci_ctrl *hctrl, u32 *addr, u32 *regval, u8 reg_amount);
    //int (*hci_load_fw)(struct ssv6xxx_hci_ctrl *hctrl, u8 *firmware_name, u8 openfile);
    int (*hci_load_fw)(struct ssv6xxx_hci_ctrl *hci_ctrl, u32 start_addr, u8 *data, int data_length);

    /**
        * This function is assigned by HCI driver at initial time and is called 
        * from the drivers above the HCI layer if upper layer has tx frames
        * to send. The return value of this function maybe one of:
        * @ len: after accepting the current frame, return the queue len
        * @ -1: failed
        */
    int (*hci_tx)(struct ssv6xxx_hci_ctrl *hctrl, struct sk_buff *, int, bool, u32);

#if 0
    /**
        * This function is assigned by the drivers above the HCI layer and 
        * is called from HCI driver once it receives frames from interface
        * (SDIO).
        */
    int (*hci_rx)(struct ssv6xxx_hci_ctrl *hctrl, struct sk_buff *);
#endif

    int (*hci_tx_pause)(struct ssv6xxx_hci_ctrl *hctrl, u32 txq_mask);

    /**
        * If HCI queue is full, HCI will prevent upper layer from transmitting 
        * frames. This function is used by HCI to signal upper layer to resume
        * frame transmission.
        */
    int (*hci_tx_resume)(struct ssv6xxx_hci_ctrl *hctrl, u32 txq_mask);

    /**
        * This function is used by upper layer to start/stop the specified sw-txq 
        * frames.
        */
    void (*hci_tx_pause_by_sta)(struct ssv6xxx_hci_ctrl *hctrl, int txqid);
    void (*hci_tx_resume_by_sta)(struct ssv6xxx_hci_ctrl *hctrl, int txqid);
    
    /**
        * This function is used by upper layer to discard the specified txq 
        * frames. If the parameter is NULL, all txq in HCI will be discarded.
        */
    int (*hci_txq_flush)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_ble_txq_flush)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_hcmd_txq_flush)(struct ssv6xxx_hci_ctrl *hctrl);


    /**
        * Called from upper layer to flush tx frames which are dedicated to
        * a explicitly specify station AID. This function is normally used on
        * AP mode.
        */
    int (*hci_txq_flush_by_sta)(struct ssv6xxx_hci_ctrl *hctrl, int aid);

    void (*hci_txq_lock_by_sta)(struct ssv6xxx_hci_ctrl *hctrl, int txqid);
    void (*hci_txq_unlock_by_sta)(struct ssv6xxx_hci_ctrl *hctrl, int txqid);
    /**
        * Function provided for query of queue status by upper layer. The
        * parameter maybe one of
        * @ NULL :        indicate all queues
        * @ non-NULL: indicate the specify queue
        */
    int (*hci_txq_len)(struct ssv6xxx_hci_ctrl *hctrl);
    
    bool (*hci_txq_empty)(struct ssv6xxx_hci_ctrl *hctrl, int txqid);
 
    int (*hci_pmu_wakeup)(struct ssv6xxx_hci_ctrl *hctrl);
 
    void (*hci_cmd_done)(struct ssv6xxx_hci_ctrl *hctrl, u8 *data, int len);
    int (*hci_send_cmd)(struct ssv6xxx_hci_ctrl *hctrl, struct sk_buff *);
    void (*hci_ignore_cmd)(struct ssv6xxx_hci_ctrl *hctrl, bool val);

    int (*hci_write_sram)(struct ssv6xxx_hci_ctrl *hctrl, u32 addr, u8* data, u32 size);
    int (*hci_interface_reset)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_sysplf_reset)(struct ssv6xxx_hci_ctrl *hctrl, u32 addr, u32 value);
#ifdef CONFIG_PM
    int (*hci_hwif_suspend)(struct ssv6xxx_hci_ctrl *hctrl);
    int (*hci_hwif_resume)(struct ssv6xxx_hci_ctrl *hctrl);
#endif
    int (*hci_set_cap)(struct ssv6xxx_hci_ctrl *hctrl, enum ssv_hci_cap cap, bool enable);
    int (*hci_set_trigger_conf)(struct ssv6xxx_hci_ctrl *hctrl, bool en, u32 qlen, u32 pkt_size, u32 timeout);
};



/**
* struct ssv6xxx_hci_info - ssv6xxx hci registration interface.
*
* This structure shall be allocated from registrar and register to
* ssv6xxx hci.
* @ dev
* @ if_ops : sdio/spi operation
* @ hci_ops : hci operation
* @ hci_rx_cb
*/
struct ssv6xxx_hci_info {

    struct device *dev;
    struct ssv6xxx_hwif_ops *if_ops;
    struct ssv6xxx_hci_ops *hci_ops;
    struct ssv6xxx_hci_ctrl *hci_ctrl;
    struct ssv_softc *sc;
    struct ssv_hw    *sh;

    /* Rx callback function */
    int (*hci_rx_cb)(struct sk_buff *, void *);
    int (*hci_is_rx_q_full)(void *);

    /* update flowctl stats */
    void (*hci_update_flowctl_cb)(void *);
    
	/* Post Tx callback function */
    void (*hci_post_tx_cb)(struct sk_buff_head *, void *);

    /* Tx buffer function */
    void (*hci_tx_buf_free_cb)(struct sk_buff *, void *);

	/* HCI rx mode */
    int (*hci_rx_mode_cb)(void *);
    
    /* DBG print */
	void (*dbgprint)(void *, u32 log_id, const char *fmt,...);

    struct sk_buff *(*skb_alloc) (void *app_param, s32 len);
    void (*skb_free) (void *app_param, struct sk_buff *skb);    
};


int ssv6xxx_hci_deregister(struct ssv6xxx_hci_info *);
int ssv6xxx_hci_register(struct ssv6xxx_hci_info *, bool hci_tx_aggr);

int ssv6xxx_hci_init(void);
void ssv6xxx_hci_exit(void);

static inline struct sk_buff *ssv6xxx_hci_skb_peek_next(struct sk_buff *skb,
		const struct sk_buff_head *list_)
{
	struct sk_buff *next = skb->next;

	if (next == (struct sk_buff *)list_)
		next = NULL;
	return next;
}

static inline bool ssv6xxx_hci_is_sw_sta_txq(int txqid)
{
    return (((txqid <= SSV_SW_TXQ_ID_STAMAX) && (txqid >= 0)) ? true : false);
}

static inline bool ssv6xxx_hci_is_sw_mng_txq(int txqid)
{
    return ((SSV_SW_TXQ_ID_MNG == txqid) ? true : false);
}

#define SSV_READRG_HCI_INQ_INFO(_hci_ctrl, _used_id, _tx_use_page)  \
                                                      HAL_READRG_HCI_INQ_INFO((_hci_ctrl)->shi->sh, _used_id, _tx_use_page)
#define SSV_LOAD_FW_ENABLE_MCU(_hci_ctrl)             HAL_LOAD_FW_ENABLE_MCU((_hci_ctrl)->shi->sh)
#define SSV_LOAD_FW_DISABLE_MCU(_hci_ctrl)            HAL_LOAD_FW_DISABLE_MCU((_hci_ctrl)->shi->sh)
#define SSV_LOAD_FW_SET_STATUS(_hci_ctrl, _status)    HAL_LOAD_FW_SET_STATUS((_hci_ctrl)->shi->sh, (_status))
#define SSV_LOAD_FW_GET_STATUS(_hci_ctrl, _status)    HAL_LOAD_FW_GET_STATUS((_hci_ctrl)->shi->sh, (_status))
#define SSV_RESET_CPU(_hci_ctrl)                      HAL_RESET_CPU((_hci_ctrl)->shi->sh)
#define SSV_SET_SRAM_MODE(_hci_ctrl, _mode)           HAL_SET_SRAM_MODE((_hci_ctrl)->shi->sh, _mode)
#define SSV_LOAD_FW_PRE_CONFIG_DEVICE(_hci_ctrl)      HAL_LOAD_FW_PRE_CONFIG_DEVICE((_hci_ctrl)->shi->sh)
#define SSV_LOAD_FW_POST_CONFIG_DEVICE(_hci_ctrl)     HAL_LOAD_FW_POST_CONFIG_DEVICE((_hci_ctrl)->shi->sh)

#endif /* _SSV_HCI_H_ */


