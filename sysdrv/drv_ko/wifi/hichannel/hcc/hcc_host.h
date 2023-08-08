/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc driver implementatioin.
 * Author: Hisilicon
 * Create: 2020-09-10
 */

#ifndef __HCC_HOST_H
#define __HCC_HOST_H

#include "oal_mutex.h"
#include "oal_wait.h"
#include "oal_timer.h"
#include "oal_workqueue.h"
#include "oal_wakelock.h"
#include "oal_thread.h"
#include "hcc_comm.h"
#include "oal_completion.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HCC_OFF                             0
#define HCC_ON                              1
#define HCC_EXCEPTION                       2
#define HIBUS_HOST2DEV_SCATT_MAX            64
#define HIBUS_HOST2DEV_SCATT_SIZE           64
#define HIBUS_DEV2HOST_SCATT_MAX            64
#define HIBUS_H2D_SCATT_BUFFLEN_ALIGN       32
#define HIBUS_H2D_SCATT_BUFFLEN_ALIGN_BITS  3

/* 宏定义 */
#define HCC_TX_ASSEM_INFO_MAX_NUM           (HIBUS_HOST2DEV_SCATT_MAX + 1)
#define HCC_RX_ASSEM_INFO_MAX_NUM           (HIBUS_HOST2DEV_SCATT_MAX + 1)
#define HCC_FLOW_HIGH_PRI_BUFF_CNT          5
#define HCC_FLOW_LOW_PRI_BUFF_CNT           5
#define HCC_TRIG_TX_SCHED_TIMEOUT           10
/* 0~7 bits occupied by hisyslink */
#define hcc_large_pkt_get(reg)              (((reg) >> 8) & 0xFF)
#define hcc_short_pkt_get(reg)              (((reg) >> 16) & 0xFF)
#define hcc_mgmt_pkt_get(reg)               (((reg) >> 24) & 0xFF)
#define MAX_ASSEM_DESC_CNT                  2

typedef hi_s32 (*hcc_msg_cb)(hi_void *data);

typedef hi_u8 hcc_queue_type;

typedef enum {
    HCC_FLOWCTRL_SDIO,
    HCC_FLOWCTRL_CREDIT,
    HCC_FLOWCTRL_BUTT
}hcc_flowctrl_type;

typedef enum {
    TIMER_ADD,
    TIMER_RUNING,
    TIMER_STOP,
    TIMER_DEL
}hcc_timer_status;

typedef enum {
    HCC_TX,
    HCC_RX,
    HCC_DIR_COUNT
} hcc_chan_type;

typedef struct {
    hi_u8                       flowctrl_flag;
    hi_u8                       enable;
    hi_u16                      flow_type;
    hi_u16                      is_stopped;
    hi_u16                      low_waterline;
    hi_u16                      high_waterline;
} hcc_flow_ctrl_stru;

typedef enum {
    HCC_SINGLE_SEND  = 0,
    HCC_ASSEM_SEND,
    HCC_MODE_COUNT
} hcc_send_mode;

typedef struct {
    /* transfer pkts limit every loop */
    hi_u32                  pool_type;
    hi_u32                  burst_limit;
    hcc_flow_ctrl_stru      flow_ctrl;
    hcc_send_mode           send_mode;
    hi_u32                  total_pkts;
    hi_u32                  loss_pkts;
    hcc_data_queue          queue_info;
} hcc_trans_queue_stru;

typedef struct {
    hcc_trans_queue_stru              queues[HCC_QUEUE_COUNT];
} hcc_trans_queues_stru;

typedef struct {
    hi_u32                      info[HCC_TX_ASSEM_INFO_MAX_NUM];
    hi_u32                      assemble_max_count;
    hcc_data_queue              assembled_head;
    hcc_queue_type_enum         assembled_queue_type;
} hcc_tx_assem_info_stru;

typedef struct {
    hi_u32                       info[HCC_RX_ASSEM_INFO_MAX_NUM];
} hcc_rx_assem_info;

typedef struct {
    hi_u32 flowctrl_flag;
    hi_u32 flowctrl_on_count;
    hi_u32 flowctrl_off_count;
    hi_u32 flowctrl_reset_count;
    hi_u32 flowctrl_hipri_update_count;
    hi_u8  uc_hipriority_cnt;
    hi_u8  uc_lopriority_cnt;
    hi_u8  des_cnt;
    hi_u8  auc_resv[2];                 /* resv 2 对齐 */
    oal_spin_lock_stru  st_hipri_lock;  /* 读写uc_hipriority_cnt时要加锁 */
    oal_timer_list_stru flow_timer;
    unsigned long       timeout;
    oal_delayed_work    worker;
    oal_spin_lock_stru  lock;
}hcc_tx_flow_ctrl_info_stru;

typedef struct {
    oal_kthread_stru            *hcc_transfer_thread;
    oal_kthread_stru            *hcc_rx_thread;
    oal_wait_queue_head_stru    hcc_tx_wq;
    oal_wait_queue_head_stru    hcc_rx_wq;
#ifdef _PRE_HICHANNEL_DEBUG
    oal_completion              hcc_test_tx;
#endif
    hi_u32                      channel_exception_flag;
    oal_timer_list_stru         hcc_timer;
    hcc_timer_status            hcc_timer_status;
    hcc_trans_queues_stru       hcc_queues[HCC_DIR_COUNT];
    hcc_tx_assem_info_stru      tx_assem_info;
    hcc_rx_assem_info           rx_assem_info;
    hcc_tx_flow_ctrl_info_stru  tx_flow_ctrl;
} hcc_transfer_handler_stru;

typedef struct {
    hi_u8                   descr_num;
    hcc_data_queue          tx_assem_descr_hdr;
} hcc_tx_assem_descr;

struct hcc_bus_adpta_ops {
    hi_u32  (*rx_proc_queue)(hcc_unc_struc *unc_buf);
    hi_u32  (*tx_discard_key_frame)(hi_u32 queue_id, hcc_trans_queue_stru *hcc_queue, hcc_unc_struc *unc_buf);
    hi_u32  (*tx_sort_key_frame)(hi_u32 queue_id, hcc_trans_queue_stru *hcc_queue, hcc_unc_struc *unc_buf);
    hi_u32  (*tx_flow_ctrl_handle)(hi_u16 flow_type, hi_u8 dev_mem_cnt);
    hi_void (*wlan_pm_set_packet_cnt)(hi_u32 cnt);
    hcc_unc_struc *(*private_len_align)(hcc_unc_struc *ufc_buf, int align_len);
    hcc_unc_struc *(*alloc_unc_buf)(hi_s32 len, hcc_stru_type type);
    hi_void (*free_unc_buf)(hi_void *data);
    hi_u32 (*wlan_pm_wakeup_dev)(hi_void);
    hi_void (*channel_rx_test)(hcc_header_stru *hcc_hdr, hi_char* buf, int len);
    hi_void (*stop_tcpip_tx_queue)(hcc_trans_queue_stru *hcc_queue);
    hi_void (*awake_tcpip_tx_queue)(hcc_trans_queue_stru *hcc_queue);
};

typedef struct {
    hi_void                     *hi_channel;
    oal_mutex_stru              tx_transfer_lock;
    hcc_transfer_handler_stru   hcc_transer_info;
    hcc_tx_assem_descr          tx_descr_info;
    struct hcc_bus_adpta_ops*   hcc_bus_ops;
} hcc_handler_stru;

/* inline 函数定义 */
static inline void hcc_tx_transfer_lock(hcc_handler_stru *hcc)
{
    if (oal_warn_on(hcc == HI_NULL)) {
        return;
    }
    oal_mutex_lock(&hcc->tx_transfer_lock);
}

static inline void hcc_tx_transfer_unlock(hcc_handler_stru *hcc)
{
    if (oal_warn_on(hcc == HI_NULL)) {
        return;
    }
    oal_mutex_unlock(&hcc->tx_transfer_lock);
}

/* 函数声明 */
hi_s32  hcc_message_register(const hcc_handler_stru *hcc_handler, hi_u8 msg, hcc_msg_cb cb, hi_void *data);
hi_void hcc_message_unregister(const hcc_handler_stru *hcc_handler, hi_u8 msg);
hi_u32  hcc_host_init(struct hcc_bus_adpta_ops* hcc_bus_opt);
hi_void hcc_host_exit(hcc_handler_stru *hcc);
hi_void hcc_sched_transfer(hcc_handler_stru *hcc_handler);
hi_s32  hcc_host_proc_tx_queue(hcc_handler_stru *hcc_handler, hcc_queue_type_enum type);
hi_u32  hcc_host_proc_rx_queue(hcc_handler_stru *hcc_handler, hcc_queue_type_enum  type);
hcc_handler_stru *hcc_host_get_handler(hi_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* __HCC_HOST_H */
