/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc layer frw task.
 * Author: dujinxin
 * Create: 2020-09-28
 */
/* 头文件包含 */
#include "hcc_task.h"
#include "securec.h"
#include "oal_mm.h"
#include "oal_channel_host_if.h"
#include "hcc_list.h"
#include <linux/delay.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 宏定义 */
#define THRESHOLD_SIZE_128_BYTES    128
#define THRESHOLD_SIZE_256_BYTES    256
#define THRESHOLD_SIZE_512_BYTES    512
#define THRESHOLD_SIZE_1024_BYTES   1024

#define oal_min(a, b)         (((a) > (b)) ? (b) : (a))
#define oal_max(a, b)         (((a) > (b)) ? (a) : (b))
/* 全局变量定义 */
hcc_handler_stru *g_hcc_host_handler = HI_NULL;
hi_u32  g_hcc_assemble_count = 5;
static hi_u32  g_lo_buf_times = 0;
#define MAX_TIMES                           1
#define MAX_TIME_VALUE                      15
#define MIN_TIME_VALUE                      15

/* 函数定义 */
hcc_handler_stru *hcc_host_get_handler(hi_void)
{
    return g_hcc_host_handler;
}

static hi_void hcc_tx_list_free(hcc_data_queue *hcc_queue)
{
    hcc_unc_struc *unc_buf = HI_NULL;
    for (;;) {
        unc_buf = hcc_list_dequeue(hcc_queue);
        if (unc_buf == HI_NULL) {
            break;
        }
        unc_buf->free(unc_buf);
    }
}

hi_void hcc_clear_queues(hcc_handler_stru *hcc, hcc_chan_type type)
{
    hi_s32 i;
    hcc_data_queue *head = HI_NULL;
    for (i = 0; i < HCC_QUEUE_COUNT; i++) {
        head = &hcc->hcc_transer_info.hcc_queues[type].queues[i].queue_info;
        if (hcc_list_len(head) > 0) {
            hcc_tx_list_free(head);
        }
    }
}

hi_void hcc_sched_transfer(hcc_handler_stru *hcc_handler)
{
    if (oal_warn_on(hcc_handler == HI_NULL)) {
        oam_error_log0("hcc_sched_transfer:: hcc_handler is null!");
        return;
    }

    hi_wait_queue_wake_up_interrupt(&hcc_handler->hcc_transer_info.hcc_tx_wq);
}

/* init hcc transfer_queue */
hi_void hcc_transfer_queues_init(hcc_handler_stru *hcc_handler)
{
    hi_s32 i, j;
    for (i = 0; i < HCC_DIR_COUNT; i++) {
        for (j = 0; j < HCC_QUEUE_COUNT; j++) {
            hcc_list_head_init(&hcc_handler->hcc_transer_info.hcc_queues[i].queues[j].queue_info);
        }
    }
}

static hi_void hcc_build_next_assem_descr(hcc_handler_stru *hcc_handler,
    hcc_queue_type_enum type,
    hcc_data_queue *head,
    hcc_data_queue *next_assembled_head,
    const hcc_unc_struc *descr,
    hi_u32 remain_len)
{
    hi_s32 i = 0;
    hi_s32 len;
    hi_u8 *buf = HI_NULL;
    hcc_unc_struc *unc_buf = HI_NULL;
    hcc_unc_struc *unc_buf_t = HI_NULL;
    hi_u32 assemble_max_count, queue_len, current_trans_len;

    buf = (hi_u8 *)oal_unc_data(descr);
    len = (hi_s32)oal_unc_len(descr);
    assemble_max_count = oal_max(1, g_hcc_assemble_count);
    queue_len = hcc_list_len(head);
    current_trans_len = oal_min(queue_len, assemble_max_count);
    current_trans_len = oal_min(current_trans_len, remain_len);
    buf[0] = 0;

    if (current_trans_len == 0) {
        return;
    }

    for (;;) {
        /* move the netbuf from head queue to prepare-send queue, head->tail */
        unc_buf = hcc_list_dequeue(head);
        if (unc_buf == HI_NULL) {
            break;
        }

        /* align the buff len to 32B */
        if (hcc_handler->hcc_bus_ops && hcc_handler->hcc_bus_ops->private_len_align) {
            unc_buf_t = hcc_handler->hcc_bus_ops->private_len_align(unc_buf, HIBUS_H2D_SCATT_BUFFLEN_ALIGN);
            if (oal_unlikely(unc_buf_t == HI_NULL)) {
                hcc_list_add_head(head, unc_buf);
                break;
            }
        }

        current_trans_len--;

        hcc_list_add_tail(next_assembled_head, unc_buf_t);
        if (oal_likely(i >= len)) {
            break;
        }

        buf[i++] = (hi_u8)(oal_unc_len(unc_buf_t) >> HIBUS_H2D_SCATT_BUFFLEN_ALIGN_BITS);
        if (current_trans_len == 0) {
            /* send empty */
            if (i != len) {
                buf[i] = 0;
            }
            break;
        }
    }

    if (oal_likely(!hcc_is_list_empty(next_assembled_head))) {
        hcc_handler->hcc_transer_info.tx_assem_info.assembled_queue_type = type;
    }
}

hcc_unc_struc *hcc_tx_assem_descr_get(hcc_handler_stru *hcc_handler)
{
    return hcc_list_dequeue(&hcc_handler->tx_descr_info.tx_assem_descr_hdr);
}

static  hi_void hcc_tx_assem_descr_put(hcc_handler_stru *hcc_handler,
    hcc_unc_struc *unc_buf)
{
    hcc_list_add_tail(&hcc_handler->tx_descr_info.tx_assem_descr_hdr, unc_buf);
}

static hi_s32 hcc_send_data_packet(hcc_handler_stru *hcc_handler,
                                   hcc_data_queue *head,
                                   hcc_queue_type_enum type,
                                   hcc_data_queue *next_assembled_head,
                                   hcc_send_mode mode,
                                   hi_u32 *remain_len)
{
    hi_s32 ret = HI_SUCCESS;
    hcc_data_queue head_send;
    hcc_unc_struc *unc_buf_t = HI_NULL;

    if (*remain_len == 0) {
        return HI_SUCCESS;
    }

    hcc_unc_struc *descr_netbuf = hcc_tx_assem_descr_get(hcc_handler);
    if (descr_netbuf == HI_NULL) {
        ret = -OAL_ENOMEM;
        goto failed_get_assem_descr;
    }

    hi_u32 *info = hcc_handler->hcc_transer_info.tx_assem_info.info;

    hcc_list_head_init(&head_send);

    if (hcc_is_list_empty(next_assembled_head)) {
        /* single send */
        hcc_unc_struc *unc_buf = hcc_list_dequeue(head);
        if (unc_buf == HI_NULL) {
            ret = HI_FAIL;
            goto failed_get_sig_buff;
        }

        if (hcc_handler->hcc_bus_ops && hcc_handler->hcc_bus_ops->private_len_align) {
            unc_buf_t = hcc_handler->hcc_bus_ops->private_len_align(unc_buf, HIBUS_H2D_SCATT_BUFFLEN_ALIGN);
            if (oal_unlikely(unc_buf_t == HI_NULL)) {
                hcc_list_add_head(head, unc_buf);
                ret = HI_FAIL;
                goto failed_align_unc_buf;
            }
        } else {
            goto failed_get_sig_buff;
        }

        hcc_list_add_tail(&head_send, unc_buf_t);
        info[0]++;
    } else {
        hi_u32 assemble_len = hcc_list_len(next_assembled_head);
        /* move the assem list to send queue */
        hcc_list_splice_init(next_assembled_head, &head_send);
        info[assemble_len]++;
    }

    hi_u32 total_send = hcc_list_len(&head_send);
    if (*remain_len >= total_send) {
        *remain_len -= total_send;
    } else {
        *remain_len = 0;
    }

    if (mode == HCC_ASSEM_SEND) {
        hcc_build_next_assem_descr(hcc_handler, type, head, next_assembled_head, descr_netbuf, *remain_len);
    } else {
        hi_u8 *buf = oal_unc_data(descr_netbuf);
        *((hi_u32 *) buf) = 0;
    }

    /* add the assem descr buf */
    hcc_list_add_head(&head_send, descr_netbuf);

    ret = oal_channel_transfer_list(hcc_handler->hi_channel, &head_send, SDIO_WRITE);

    if ((hcc_handler->hcc_bus_ops != HI_NULL) && (hcc_handler->hcc_bus_ops->wlan_pm_set_packet_cnt)) {
        hcc_handler->hcc_bus_ops->wlan_pm_set_packet_cnt(total_send);
    }
    hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[type].total_pkts += total_send;

    descr_netbuf = hcc_list_dequeue(&head_send);
    if (descr_netbuf == HI_NULL) {
        ret = HI_SUCCESS;
        goto failed_get_assem_descr;
    }
    hcc_tx_assem_descr_put(hcc_handler, descr_netbuf);
    /* free the sent netbuf,release the wakelock */
    hcc_tx_list_free(&head_send);
    return ret;
failed_align_unc_buf:
failed_get_sig_buff:
    hcc_tx_assem_descr_put(hcc_handler, descr_netbuf);
failed_get_assem_descr:
    return ret;
}

static hi_s32 hcc_send_single_descr(hcc_handler_stru *hcc_handler, hcc_unc_struc *unc_buf)
{
    hi_s32 ret;
    hcc_data_queue head_send;
    oal_reference(hcc_handler);
    hcc_list_head_init(&head_send);
    hcc_list_add_tail(&head_send, unc_buf);
    ret = oal_channel_transfer_list(hcc_handler->hi_channel, &head_send, SDIO_WRITE);
    return ret;
}

hi_s32 hcc_send_descr_control_data(hcc_handler_stru *hcc_handler, hcc_descr_type descr_type,
                                   const hi_void *data, hi_u32 ul_size)
{
    hi_s32 ret;
    hcc_unc_struc *unc_buf = HI_NULL;
    struct hcc_descr_header *dscr_hdr = HI_NULL;
    unc_buf = hcc_tx_assem_descr_get(hcc_handler);
    if (unc_buf == HI_NULL) {
        return -OAL_ENOMEM;
    }

    dscr_hdr = (struct hcc_descr_header *)oal_unc_data(unc_buf);
    dscr_hdr->descr_type = descr_type;

    if (ul_size) {
        if (oal_warn_on(data == HI_NULL)) {
            hcc_tx_assem_descr_put(hcc_handler, unc_buf);
            return -OAL_EINVAL;
        }
        if (oal_warn_on(ul_size + sizeof(struct hcc_descr_header) > oal_unc_len(unc_buf))) {
            hcc_tx_assem_descr_put(hcc_handler, unc_buf);
            return -OAL_EINVAL;
        }

        /* lint -e124 */
        if (memcpy_s((hi_void *)((hi_u8 *)oal_unc_data(unc_buf) + sizeof(struct hcc_descr_header)),
            ul_size, data, ul_size) != 0) {
            return -OAL_EINVAL;
        }
    }
    ret = hcc_send_single_descr(hcc_handler, unc_buf);

    hcc_tx_assem_descr_put(hcc_handler, unc_buf);
    return ret;
}

static hi_void  hcc_restore_assemble_netbuf_list(hcc_handler_stru *hcc_handler)
{
    hcc_queue_type_enum type;
    hcc_data_queue *assembled_head;

    type = hcc_handler->hcc_transer_info.tx_assem_info.assembled_queue_type;
    assembled_head = &hcc_handler->hcc_transer_info.tx_assem_info.assembled_head;

    if (type >= HCC_QUEUE_COUNT) {
        type = DATA_LO_QUEUE;
    }

    if (!hcc_is_list_empty(assembled_head)) {
        hcc_list_splice_sync(&hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[type].queue_info,
            assembled_head);
    }
}

static  hi_s32 hcc_send_assemble_reset(hcc_handler_stru *hcc_handler)
{
    hi_s32 ret;
    hcc_handler->hcc_transer_info.tx_flow_ctrl.flowctrl_reset_count++;
    /* 当只发送一个聚合描述符包，并且聚合个数为0描述通知Device 重置聚合信息 */
    ret = hcc_send_descr_control_data(hcc_handler, HCC_DESCR_ASSEM_RESET, HI_NULL, 0);
    hcc_restore_assemble_netbuf_list(hcc_handler);
    return ret;
}

hi_s32 hcc_tx_queue_switch(hcc_handler_stru *hcc_handler, hcc_netbuf_queue_type queue_type)
{
    return hcc_send_descr_control_data(hcc_handler, HCC_QUEUE_SWITCH, &queue_type, sizeof(queue_type));
}

hi_s32 hcc_switch_high_pri_queue(hcc_handler_stru *hcc_handler,
    hcc_netbuf_queue_type pool_type)
{
    hi_s32 ret = OAL_EFAIL;
    if (pool_type == HCC_HIGH_QUEUE) {
        /* 此处在最高位表示是否是配置事件 */
        ret = hcc_tx_queue_switch(hcc_handler, HCC_HIGH_QUEUE);
    }
    return ret;
}

hi_s32 hcc_restore_normal_pri_queue(hcc_handler_stru *hcc_handler, hcc_netbuf_queue_type pool_type)
{
    hi_s32 ret = OAL_EFAIL;
    if (pool_type == HCC_HIGH_QUEUE) {
        ret = hcc_tx_queue_switch(hcc_handler, HCC_NORMAL_QUEUE);
    }
    return ret;
}

static hi_u32 hcc_tx_flow_ctrl_handle(hcc_handler_stru *hcc_handler, hcc_queue_type_enum type)
{
    hi_u32 priority_cnt;
    hi_s32 ret = HI_SUCCESS;
    hcc_trans_queue_stru* hcc_queue;

    hcc_queue = &hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[type];

    if (hcc_handler->hi_channel && oal_get_channel_func(hcc_handler->hi_channel) != HI_NULL) {
        ret = oal_channel_get_credit(hcc_handler->hi_channel, &priority_cnt);
        if (ret < 0) {
            oam_error_log0("hcc_tx_flow_ctrl_handle fail!");
            hcc_handler->hcc_transer_info.channel_exception_flag = (1 << EXCEPTION_CMD_SEND_FAILED);
            return HI_FAIL;
        }
    }
    oal_spin_lock(&(hcc_handler->hcc_transer_info.tx_flow_ctrl.st_hipri_lock));
    hcc_handler->hcc_transer_info.tx_flow_ctrl.uc_hipriority_cnt = hcc_mgmt_pkt_get(priority_cnt);
    hcc_handler->hcc_transer_info.tx_flow_ctrl.uc_lopriority_cnt = hcc_large_pkt_get(priority_cnt);
    oal_spin_unlock(&(hcc_handler->hcc_transer_info.tx_flow_ctrl.st_hipri_lock));
    if (hcc_handler->hcc_bus_ops &&
        hcc_handler->hcc_bus_ops->tx_flow_ctrl_handle) {
        if (hcc_queue->flow_ctrl.flow_type == HCC_FLOWCTRL_CREDIT) {
            ret = hcc_handler->hcc_bus_ops->tx_flow_ctrl_handle(hcc_queue->flow_ctrl.flow_type,
                hcc_handler->hcc_transer_info.tx_flow_ctrl.uc_hipriority_cnt);
        } else if (hcc_queue->flow_ctrl.flow_type == HCC_FLOWCTRL_SDIO) {
            ret = hcc_handler->hcc_bus_ops->tx_flow_ctrl_handle(hcc_queue->flow_ctrl.flow_type,
                hcc_handler->hcc_transer_info.tx_flow_ctrl.uc_lopriority_cnt);
        }
    }
    return ret;
}

hi_void hcc_tx_sleep(hi_void)
{
    g_lo_buf_times++;
    if (g_lo_buf_times > MAX_TIMES) {
        usleep_range(MIN_TIME_VALUE, MAX_TIME_VALUE);
        g_lo_buf_times = 0;
    }
}

hi_s32 hcc_host_proc_tx_queue(hcc_handler_stru *hcc_handler, hcc_queue_type_enum type)
{
    hi_s32 count = 0;

    if (type >= HCC_QUEUE_COUNT) {
        oam_error_log1("hcc_send_tx_queue:: invalid hcc_queue type[%d]", type);
        return count;
    }

    if (hcc_handler->hcc_transer_info.tx_flow_ctrl.flowctrl_flag == D2H_MSG_FLOWCTRL_OFF) {
        hcc_tx_sleep();
        return  count;
    }

    hcc_trans_queue_stru *hcc_queue = &hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[type];
    hcc_data_queue *head = &hcc_queue->queue_info;

    hi_u32 remain_len = hcc_list_len(head);
    if (!remain_len) {
        goto hcc_tx_exit;
    }

    /* hcc流控处理 */
    hi_s32 ret = hcc_tx_flow_ctrl_handle(hcc_handler, type);
    if (ret != HI_SUCCESS) {
        hcc_tx_sleep();
        goto hcc_tx_exit;
    }

    g_lo_buf_times = 0;

    if (hcc_queue->flow_ctrl.flow_type == HCC_FLOWCTRL_CREDIT) {
        remain_len = oal_min(hcc_handler->hcc_transer_info.tx_flow_ctrl.uc_hipriority_cnt, remain_len);
    } else if (hcc_queue->flow_ctrl.flow_type == HCC_FLOWCTRL_SDIO) {
        remain_len = oal_min(hcc_handler->hcc_transer_info.tx_flow_ctrl.uc_lopriority_cnt, remain_len);
    }

    hi_u32 remain_len_t = remain_len;
    hcc_data_queue *next_assembled_head = &hcc_handler->hcc_transer_info.tx_assem_info.assembled_head;

    if (!hcc_is_list_empty(next_assembled_head)) {
        if (hcc_send_assemble_reset(hcc_handler) != HI_SUCCESS) {
            /* send one pkt */
            count = 1;
            goto hcc_tx_exit;
        }
    }

    hcc_send_mode send_mode = hcc_queue->send_mode;
    hcc_queue_type pool_type = hcc_queue->pool_type;
    while (remain_len != 0) {
        if (hcc_queue->flow_ctrl.flow_type == HCC_FLOWCTRL_CREDIT) {
            ret = hcc_switch_high_pri_queue(hcc_handler, pool_type);
            if (ret != HI_SUCCESS) {
                break;
            }
        }

        if (hcc_handler->hcc_transer_info.tx_flow_ctrl.flowctrl_flag == D2H_MSG_FLOWCTRL_OFF) {
            break;
        }

        ret = hcc_send_data_packet(hcc_handler, head, type, next_assembled_head, send_mode, &remain_len);
        if (ret != HI_SUCCESS) {
            break;
        }
        count += (hi_s32)(remain_len_t - remain_len);
    }

    if (hcc_queue->flow_ctrl.flow_type == HCC_FLOWCTRL_CREDIT) {
        hcc_restore_normal_pri_queue(hcc_handler, pool_type);
    }

hcc_tx_exit:
    return count;
}

hi_u32 hcc_host_proc_rx_queue(hcc_handler_stru *hcc_handler, hcc_queue_type_enum  type)
{
    hi_s32 count = 0;
    hi_s32 ret;
    hcc_data_queue *head = HI_NULL;
    hcc_unc_struc *unc_buf = HI_NULL;
    head = &hcc_handler->hcc_transer_info.hcc_queues[HCC_RX].queues[type].queue_info;

    for (;;) {
        unc_buf = hcc_list_dequeue(head);
        if (unc_buf == HI_NULL) {
            break;
        }
        if (hcc_handler->hcc_bus_ops != NULL && hcc_handler->hcc_bus_ops->rx_proc_queue) {
            ret = hcc_handler->hcc_bus_ops->rx_proc_queue(unc_buf);
            count++;
        } else {
            unc_buf->free(unc_buf);
        }
    }
    return count;
}

hi_void hcc_trans_flow_ctrl_info_init(hcc_handler_stru *hcc_handle)
{
    hi_s32 i;

    hcc_handle->hcc_transer_info.tx_flow_ctrl.flowctrl_flag = D2H_MSG_FLOWCTRL_ON;
    hcc_handle->hcc_transer_info.tx_flow_ctrl.flowctrl_off_count = 0;
    hcc_handle->hcc_transer_info.tx_flow_ctrl.flowctrl_on_count = 0;
    oal_spin_lock_init(&hcc_handle->hcc_transer_info.tx_flow_ctrl.lock);

    hcc_handle->hcc_transer_info.tx_flow_ctrl.uc_hipriority_cnt = HCC_FLOW_HIGH_PRI_BUFF_CNT;
    hcc_handle->hcc_transer_info.tx_flow_ctrl.uc_lopriority_cnt = HCC_FLOW_LOW_PRI_BUFF_CNT;
    oal_spin_lock_init(&hcc_handle->hcc_transer_info.tx_flow_ctrl.st_hipri_lock);

    hcc_trans_queues_stru *hcc_tx_queues = &hcc_handle->hcc_transer_info.hcc_queues[HCC_TX];
    hcc_trans_queues_stru *hcc_rx_queues = &hcc_handle->hcc_transer_info.hcc_queues[HCC_TX];

    for (i = 0; i < HCC_QUEUE_COUNT; i++) {
        /* TX queue */
        hcc_tx_queues->queues[i].flow_ctrl.enable = HI_TRUE;
        hcc_tx_queues->queues[i].flow_ctrl.flow_type = HCC_FLOWCTRL_SDIO;
        hcc_tx_queues->queues[i].flow_ctrl.is_stopped = HI_FALSE;
        hcc_tx_queues->queues[i].flow_ctrl.low_waterline = THRESHOLD_SIZE_512_BYTES;
        hcc_tx_queues->queues[i].flow_ctrl.high_waterline = THRESHOLD_SIZE_1024_BYTES;
        hcc_tx_queues->queues[i].pool_type = HCC_NORMAL_QUEUE;

        /* RX queue */
        hcc_rx_queues->queues[i].flow_ctrl.enable = HI_TRUE;
        hcc_rx_queues->queues[i].flow_ctrl.is_stopped = HI_FALSE;
        hcc_rx_queues->queues[i].flow_ctrl.low_waterline = THRESHOLD_SIZE_128_BYTES;
        hcc_rx_queues->queues[i].flow_ctrl.high_waterline = THRESHOLD_SIZE_512_BYTES;
    }

    /* Additional high priority flow_ctrl settings */
    hcc_tx_queues->queues[DATA_HI_QUEUE].flow_ctrl.flow_type = HCC_FLOWCTRL_CREDIT;
    hcc_tx_queues->queues[DATA_HI_QUEUE].flow_ctrl.enable = HI_FALSE;
    hcc_tx_queues->queues[DATA_HI_QUEUE].pool_type = HCC_HIGH_QUEUE;
}

hi_void hcc_trans_send_mode_init(hcc_handler_stru *hcc_handler)
{
    hi_s32 i;

    for (i = 0; i < HCC_QUEUE_COUNT; i++) {
        hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[i].send_mode = HCC_ASSEM_SEND;
    }
    hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[DATA_HI_QUEUE].send_mode = HCC_SINGLE_SEND;
}

hi_s32 hcc_message_register(const hcc_handler_stru *hcc_handler, hi_u8 msg, hcc_msg_cb cb, hi_void *data)
{
    return oal_channel_message_register(hcc_handler->hi_channel, msg, cb, data);
}

hi_void hcc_message_unregister(const hcc_handler_stru *hcc_handler, hi_u8 msg)
{
    oal_channel_message_unregister(hcc_handler->hi_channel, msg);
}

static hi_u32 hcc_host_check_header_vaild(const hcc_header_stru *hcc_hdr)
{
    if ((hcc_hdr->main_type >= HCC_TYPE_BUFF)) {
        oam_error_log1("hcc_host_check_header_vaild:: invalid type[%d]", hcc_hdr->main_type);
        return HI_FALSE;
    }
    return HI_TRUE;
}

static hi_void hcc_transfer_rx_handler_replace(hcc_handler_stru *hcc_handler, hcc_unc_struc *unc_buf_new)
{
    hcc_unc_struc *unc_buf_old = HI_NULL;
    hcc_trans_queue_stru *hcc_queue = &hcc_handler->hcc_transer_info.hcc_queues[HCC_RX].queues[DATA_LO_QUEUE];
    unc_buf_old = hcc_list_dequeue(&hcc_queue->queue_info);
    if (unc_buf_old != HI_NULL) {
        hcc_queue->loss_pkts++;
        unc_buf_old->free(unc_buf_old);
    }
    hcc_list_add_tail(&hcc_queue->queue_info, unc_buf_new);
}

static hi_void hcc_transfer_rx_handler(hcc_handler_stru *hcc_handler, hcc_unc_struc *unc_buf)
{
    hcc_list_add_tail(&hcc_handler->hcc_transer_info.hcc_queues[HCC_RX].queues[DATA_LO_QUEUE].queue_info, unc_buf);
}

hi_void hcc_rx_sched_transfer(hcc_handler_stru *hcc_handler)
{
    if (oal_warn_on(hcc_handler == HI_NULL)) {
        oam_error_log0("hcc_sched_transfer:: hcc_handler is null!");
        return;
    }
    hi_wait_queue_wake_up_interrupt(&hcc_handler->hcc_transer_info.hcc_rx_wq);
}

static hi_void hcc_rx_list_handler(hcc_handler_stru *hcc_handler, hcc_data_queue *head)
{
    hi_u32 scatt_count;
    hcc_unc_struc *unc_buf = HI_NULL;
    hcc_header_stru *hcc_hdr = HI_NULL;

    scatt_count = hcc_list_len(head);
    if (scatt_count > HIBUS_DEV2HOST_SCATT_MAX) {
        oam_error_log1("hcc_rx_netbuf_list_handler:: scatt buffs overflow, scatt_count[%d]", scatt_count);
        scatt_count = 0;
    }

    hcc_handler->hcc_transer_info.rx_assem_info.info[scatt_count]++;
    hcc_trans_queues_stru *hcc_rx = &hcc_handler->hcc_transer_info.hcc_queues[HCC_RX];

    for (;;) {
        unc_buf = hcc_list_dequeue(head);
        if (unc_buf == HI_NULL) {
            break;
        }
        hcc_hdr = (hcc_header_stru *)oal_unc_data(unc_buf);
        if (hcc_host_check_header_vaild(hcc_hdr) != HI_TRUE) {
            unc_buf->free(unc_buf);
            oam_error_log0("invalid hcc header: ");
            break;
        }

        if (hcc_rx->queues[DATA_LO_QUEUE].flow_ctrl.enable == HI_TRUE) {
            if (hcc_list_len(&hcc_rx->queues[DATA_LO_QUEUE].queue_info) >
                hcc_rx->queues[DATA_LO_QUEUE].flow_ctrl.high_waterline) {
                hcc_transfer_rx_handler_replace(hcc_handler, unc_buf);
            } else {
                hcc_transfer_rx_handler(hcc_handler, unc_buf);
            }
        } else {
            hcc_transfer_rx_handler(hcc_handler, unc_buf);
        }
    }

    hcc_rx_sched_transfer(hcc_handler);
}

hi_s32 hcc_transfer_rx_data_handler(hi_void *data)
{
    hi_s32 ret;
    hcc_data_queue head;
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)data;

    hcc_list_head_init(&head);

    ret = oal_channel_build_rx_list(hcc_handler->hi_channel, &head);
    if (ret != HI_SUCCESS) {
        oam_error_log1("oal_channel_build_rx_list:: oal_channel_build_rx_list failed[%d]", ret);
        return ret;
    }

    ret = oal_channel_transfer_list(hcc_handler->hi_channel, &head, SDIO_READ);
    if (ret != HI_SUCCESS) {
        hcc_list_purge(&head);
        oam_error_log1("oal_channel_transfer_list:: oal_channel_transfer_list failed[%d]", ret);
        return -OAL_EFAIL;
    }

    hcc_rx_list_handler(hcc_handler, &head);
    return HI_SUCCESS;
}

hi_void hcc_tx_assem_descr_exit(hcc_handler_stru *hcc_handler)
{
    hcc_list_purge(&hcc_handler->tx_descr_info.tx_assem_descr_hdr);
}

hi_void hcc_host_tx_assem_info_reset(hcc_handler_stru *hcc_handler)
{
    memset_s(hcc_handler->hcc_transer_info.tx_assem_info.info,
        sizeof(hcc_handler->hcc_transer_info.tx_assem_info.info),
        0,
        sizeof(hcc_handler->hcc_transer_info.tx_assem_info.info));
}

hi_void hcc_host_rx_assem_info_reset(hcc_handler_stru *hcc_handler)
{
    memset_s(hcc_handler->hcc_transer_info.rx_assem_info.info,
        sizeof(hcc_handler->hcc_transer_info.rx_assem_info.info),
        0,
        sizeof(hcc_handler->hcc_transer_info.rx_assem_info.info));
}

hi_void hcc_assem_info_init(hcc_handler_stru *hcc_handler)
{
    hcc_handler->hcc_transer_info.tx_assem_info.assemble_max_count = g_hcc_assemble_count;
    hcc_host_tx_assem_info_reset(hcc_handler);
    hcc_host_rx_assem_info_reset(hcc_handler);
    hcc_list_head_init(&hcc_handler->hcc_transer_info.tx_assem_info.assembled_head);
}

hi_s32 hcc_tx_assem_descr_init(hcc_handler_stru *hcc_handler)
{
    hi_s32 i;
    hi_s32 ret = HI_SUCCESS;
    hcc_unc_struc *accem_desm = HI_NULL;

    if (hcc_handler->hcc_bus_ops == HI_NULL ||
        hcc_handler->hcc_bus_ops->alloc_unc_buf == HI_NULL) {
        return HI_FAIL;
    }

    hcc_list_head_init(&hcc_handler->tx_descr_info.tx_assem_descr_hdr);
    hcc_handler->tx_descr_info.descr_num = MAX_ASSEM_DESC_CNT;

    for (i = 0; i < hcc_handler->tx_descr_info.descr_num; i++) {
        accem_desm = hcc_handler->hcc_bus_ops->alloc_unc_buf(HIBUS_HOST2DEV_SCATT_SIZE, NORMAL_STRU_TYPE);
        if (accem_desm == HI_NULL) {
            goto failed_netbuf_alloc;
        }

        hcc_list_add_tail(&hcc_handler->tx_descr_info.tx_assem_descr_hdr, accem_desm);
        if (!oal_is_aligned((uintptr_t)oal_unc_data(accem_desm), 4))  { /* 4 字节对齐 */
            oam_error_log0("hcc_tx_assem_descr_init:: 4 aligned failed!");
        }
    }
    return ret;
failed_netbuf_alloc:
    hcc_list_purge(&hcc_handler->tx_descr_info.tx_assem_descr_hdr);
    return -OAL_ENOMEM;
}

hi_s32 hcc_heartbeat_callback(hi_void *data)
{
    hcc_handler_stru *hcc_handle = (hcc_handler_stru *)data;
    if (oal_channel_send_msg(hcc_handle->hi_channel, H2D_MSG_HEARTBEAT_ACK) != HI_SUCCESS) {
        return HI_FAIL;
    }
    return HI_SUCCESS;
}

hi_void hcc_dev_flowctrl_off(hcc_handler_stru *hcc_handler)
{
    hcc_handler->hcc_transer_info.tx_flow_ctrl.flowctrl_flag = D2H_MSG_FLOWCTRL_OFF;
    hcc_handler->hcc_transer_info.tx_flow_ctrl.flowctrl_off_count++;
}

hi_s32 hcc_flow_off_callback(hi_void *data)
{
    hcc_dev_flowctrl_off((hcc_handler_stru *)data);
    return HI_SUCCESS;
}

#ifdef _PRE_HICHANNEL_DEBUG
hi_s32 hcc_test_write_over_callback(hi_void *data)
{
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)data;

    if (oal_warn_on(hcc_handler == HI_NULL)) {
        oam_error_log0("hcc_sched_transfer:: hcc_handler is null!");
        return HI_FAIL;
    }
    oal_complete(&hcc_handler->hcc_transer_info.hcc_test_tx);
    return HI_SUCCESS;
}
#endif

//static void hcc_trig_tx_sched(unsigned long data)
static void hcc_trig_tx_sched(oal_timer_list_stru * timer_list)
{
    hcc_handler_stru *hcc_handler = hcc_host_get_handler();
    if (hcc_handler == HI_NULL) {
        hcc_handler->hcc_transer_info.hcc_timer_status = TIMER_STOP;
        return;
    }
    hcc_handler->hcc_transer_info.hcc_timer_status = TIMER_RUNING;

    /* stop tcpip tx queue */
    hcc_trans_queue_stru *hcc_queue = &hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[DATA_LO_QUEUE];
    if ((hcc_handler->hcc_bus_ops != HI_NULL) &&
        (hcc_handler->hcc_bus_ops->awake_tcpip_tx_queue != HI_NULL)) {
        hcc_handler->hcc_bus_ops->awake_tcpip_tx_queue(hcc_queue);
    }

    hcc_sched_transfer(hcc_handler);
    //hi_u32 ret = oal_timer_start(&hcc_handler->hcc_transer_info.hcc_timer, HCC_TRIG_TX_SCHED_TIMEOUT);
    hi_u32 ret = oal_timer_start(timer_list, HCC_TRIG_TX_SCHED_TIMEOUT);
    if (ret != 0 && ret != 1) {
        oam_error_log1("{hcc_trig_tx_sched timer fail: fail ret = %d}", ret);
    }
    hcc_handler->hcc_transer_info.hcc_timer_status = TIMER_STOP;
}

static hcc_unc_struc* hcc_alloc_unc_buf(hi_s32 len, hcc_stru_type type)
{
    hcc_handler_stru *hcc_handler = hcc_host_get_handler();
    if (hcc_handler == HI_NULL) {
        return HI_NULL;
    }

    if (hcc_handler->hcc_bus_ops &&
        hcc_handler->hcc_bus_ops->alloc_unc_buf) {
        return hcc_handler->hcc_bus_ops->alloc_unc_buf(len, type);
    }

    return HI_NULL;
}

static hi_u32 hcc_wlan_pm_wakeup_dev(hi_void *data)
{
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)data;
    if (hcc_handler == HI_NULL) {
        return HI_FAIL;
    }

    if (hcc_handler->hcc_bus_ops &&
        hcc_handler->hcc_bus_ops->wlan_pm_wakeup_dev) {
        return hcc_handler->hcc_bus_ops->wlan_pm_wakeup_dev();
    }
    return HI_SUCCESS;
}

static struct hcc_bus_ops g_bus_ops = {
    .rx             = hcc_transfer_rx_data_handler,
    .alloc_unc_buf  = hcc_alloc_unc_buf,
    .pm_wakeup_dev  = hcc_wlan_pm_wakeup_dev,
};

hi_u32 hcc_host_init(struct hcc_bus_adpta_ops* hcc_bus_adpta_opt)
{
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)oal_memalloc(sizeof(hcc_handler_stru));
    if (hcc_handler == HI_NULL) {
        oam_error_log0("hcc_host_init:: malloc hcc_handler fail!");
        return HI_FAIL;
    }
    else
    {
    	g_hcc_host_handler = hcc_handler;
		oam_info_log0("hcc_host_init:: malloc hcc_handler success!");
    }
    if (memset_s(hcc_handler, sizeof(hcc_handler_stru), 0, sizeof(hcc_handler_stru)) != EOK) {
        goto sdio_init_err;
    }

    if (oal_channel_init((void*)hcc_handler, 1, &g_bus_ops) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: oal_sdio_init fail!");
        goto sdio_init_err;
    }

    hi_wait_queue_init_head(&hcc_handler->hcc_transer_info.hcc_tx_wq);
    hi_wait_queue_init_head(&hcc_handler->hcc_transer_info.hcc_rx_wq);
#ifdef _PRE_HICHANNEL_DEBUG
    oal_init_completion(&hcc_handler->hcc_transer_info.hcc_test_tx);
#endif
    hcc_transfer_queues_init(hcc_handler);
    hcc_trans_flow_ctrl_info_init(hcc_handler);
    oal_mutex_init(&hcc_handler->tx_transfer_lock);
    hcc_handler->hcc_bus_ops = hcc_bus_adpta_opt;
    hi_u32 ret = hcc_task_init(hcc_handler);
    if (ret != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: hcc_task_init failed");
        goto hcc_tast_init_err;
    }

    /* tx线程防呆 */
    oal_timer_init(&hcc_handler->hcc_transer_info.hcc_timer, HCC_TRIG_TX_SCHED_TIMEOUT,
        hcc_trig_tx_sched,
        (uintptr_t)hcc_handler);
    oal_timer_add(&hcc_handler->hcc_transer_info.hcc_timer);
    hcc_handler->hcc_transer_info.hcc_timer_status = TIMER_ADD;

    hcc_assem_info_init(hcc_handler);
    hcc_trans_send_mode_init(hcc_handler);

    if (hcc_tx_assem_descr_init(hcc_handler) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: hcc_tx_assem_descr_init failed");
        goto hcc_tast_init_err;
    }

    if (hcc_message_register(hcc_handler,
        D2H_MSG_FLOWCTRL_OFF, hcc_flow_off_callback, hcc_handler) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: hcc_message_register_flowctrl_off failed!");
        goto failed_reg_flowoff_msg;
    }

    if (hcc_message_register(hcc_handler,
        D2H_MSG_HEARTBEAT, hcc_heartbeat_callback, hcc_handler) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: heartbeat_cb register failed!");
        goto failed_reg_heartbeat_msg;
    }

    /* notify SDIO device to start heartbeat */
    if (oal_channel_send_msg(hcc_handler->hi_channel, H2D_MSG_HEART_BEAT_OPEN) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: start heartbeat fail");
        goto failed_reg_heartbeat_msg;
    }

#ifdef _PRE_HICHANNEL_HDR_OPT
    if (oal_channel_send_msg(hcc_handler->hi_channel, H2D_MSG_HICHANNEL_HDR_OPT) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: hichannel_hdr opt fail");
        goto failed_reg_heartbeat_msg;
    }
#endif

#ifdef _PRE_HICHANNEL_DEBUG
    if (hcc_message_register(hcc_handler,
        D2H_MSG_TEST_WRITE_OVER,
        hcc_test_write_over_callback, hcc_handler) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: hcc_message_register_flowctrl_off failed!");
        goto failed_reg_test_write_over;
    }
#endif

    //g_hcc_host_handler = hcc_handler;
    if (oal_channel_send_msg(hcc_handler->hi_channel, H2D_MSG_SDIO_READY) != HI_SUCCESS) {
        oam_error_log0("hcc_host_init:: send ready msg fail");
        goto failed_send_ready_msg;
    }
    oam_warning_log0("hcc_host_init SUCCESSFULLY");
    return HI_SUCCESS;
failed_send_ready_msg:
#ifdef _PRE_HICHANNEL_DEBUG
failed_reg_test_write_over:
    hcc_message_unregister(hcc_handler, D2H_MSG_HEARTBEAT);
    hcc_message_unregister(hcc_handler, D2H_MSG_FLOWCTRL_OFF);
#endif
failed_reg_heartbeat_msg:
    hcc_message_unregister(hcc_handler, D2H_MSG_FLOWCTRL_OFF);
failed_reg_flowoff_msg:
    hcc_tx_assem_descr_exit(hcc_handler);
    oal_timer_delete(&hcc_handler->hcc_transer_info.hcc_timer);
    hcc_handler->hcc_transer_info.hcc_timer_status = TIMER_DEL;
hcc_tast_init_err:
    oal_kthread_stop(hcc_handler->hcc_transer_info.hcc_transfer_thread);
    hcc_handler->hcc_transer_info.hcc_transfer_thread = HI_NULL;
    oal_mutex_destroy(&hcc_handler->tx_transfer_lock);
    oal_channel_exit(hcc_handler->hi_channel);
sdio_init_err:
    oal_free(hcc_handler);
    return HI_FAIL;
}

hi_void hcc_delete_tx_sched_timer(hcc_handler_stru *hcc)
{
    hi_u16 retry_time = 10000;
    if (hcc == HI_NULL || hcc->hcc_transer_info.hcc_timer_status == TIMER_DEL) {
        return;
    }

    while (retry_time > 0 && hcc->hcc_transer_info.hcc_timer_status == TIMER_RUNING) {
        usleep_range(1, 1);
        retry_time--;
    }
    oal_timer_delete(&hcc->hcc_transer_info.hcc_timer);
    hcc->hcc_transer_info.hcc_timer_status = TIMER_DEL;
}

hi_void hcc_host_exit(hcc_handler_stru *hcc)
{
    if (hcc == HI_NULL) {
        return;
    }

    hcc_message_unregister(hcc, D2H_MSG_HEARTBEAT);
#ifdef _PRE_HICHANNEL_DEBUG
    hcc_message_unregister(hcc, D2H_MSG_TEST_WRITE_OVER);
#endif
    hcc_tx_assem_descr_exit(hcc);
    hcc_delete_tx_sched_timer(hcc);
    oal_disable_channel_state(hcc->hi_channel, OAL_SDIO_ALL);
    hcc_exit_task_thread(hcc);
    oal_kthread_stop(hcc->hcc_transer_info.hcc_transfer_thread);
    oal_kthread_stop(hcc->hcc_transer_info.hcc_rx_thread);
    hcc_clear_queues(hcc, HCC_TX);
    hcc_clear_queues(hcc, HCC_RX);
    hcc->hcc_transer_info.hcc_transfer_thread = HI_NULL;
    oal_mutex_destroy(&hcc->tx_transfer_lock);
    oal_channel_exit(hcc->hi_channel);
    oal_free(hcc);
    g_hcc_host_handler = HI_NULL;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
