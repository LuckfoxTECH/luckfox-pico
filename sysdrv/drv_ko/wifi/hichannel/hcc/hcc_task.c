/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc layer frw task.
 * Author: Hisilicon
 * Create: 2020-09-28
 */
#include "hcc_task.h"
#include "hcc_host.h"
#include "oam_ext_if.h"
#include "securec.h"
#include "hcc_list.h"
#include "sdio_host.h"
#include <linux/delay.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HCC_TASK_PRIO       3
static volatile hi_u8   g_thread_exit_flag = HI_FALSE;
static hi_u8   g_thread_working = HI_FALSE;
hcc_task_stru g_hcc_task;

#define HCC_TASK_NAME          "hisi_hcc0"
#define HCC_TASK_TX_NAME       "hisi_hcc_tx"
#define HCC_TASK_RX_NAME       "hisi_hcc_rx"
#define TX_WRITE               0
#define RX_READ                1
#define DELAY_10_US         10
#define HCC_TASK_SIZE       0x4000

hi_s32 queues_len_check(hcc_handler_stru *hcc_handler, hcc_chan_type dir)
{
    hi_s32 i;
    hcc_trans_queue_stru *p_queue;

    p_queue = hcc_handler->hcc_transer_info.hcc_queues[dir].queues;

    for (i = 0; i < HCC_QUEUE_COUNT; i++, p_queue++) {
        if (hcc_list_len(&p_queue->queue_info)) {
            return HI_TRUE;
        }
    }
    return HI_FALSE;
}

static  hi_s32  hcc_thread_tx_wait_event_cond_check(hcc_handler_stru *hcc_handler)
{
    hi_s32 ret;
    /*
     * please first check the condition
     * which may be ok likely to reduce the cpu mips
     */
    ret = (queues_len_check(hcc_handler, HCC_TX) &&
           (hcc_handler->hcc_transer_info.channel_exception_flag == HI_FALSE) &&
           hcc_handler->hcc_transer_info.tx_flow_ctrl.flowctrl_flag == D2H_MSG_FLOWCTRL_ON);
#ifdef _PRE_CONFIG_WLAN_THRANS_THREAD_DEBUG
    if (ret == HI_TRUE)
        hcc_handler->hcc_transer_info.thread_stat.wait_event_run_count++;
    if (ret == HI_FALSE)
        hcc_handler->hcc_transer_info.thread_stat.wait_event_block_count++;
#endif
    return ret;
}

static  hi_s32  hcc_thread_rx_wait_event_cond_check(hcc_handler_stru *hcc_handler)
{
    hi_s32 ret;

    /*
     * please first check the condition
     * which may be ok likely to reduce the cpu mips
     */
    ret = queues_len_check(hcc_handler, HCC_RX);
#ifdef _PRE_CONFIG_WLAN_THRANS_THREAD_DEBUG
    if (ret == HI_TRUE)
        hcc_handler->hcc_transer_info.thread_stat.wait_event_run_count++;
    if (ret == HI_FALSE)
        hcc_handler->hcc_transer_info.thread_stat.wait_event_block_count++;
#endif
    return ret;
}

hi_s32 hcc_thread_process(hcc_handler_stru *hcc_handler, hi_u8 wr)
{
    hi_s32 ret = 0;
    if (wr == TX_WRITE) {
        ret += hcc_host_proc_tx_queue(hcc_handler, DATA_HI_QUEUE);
        ret += hcc_host_proc_tx_queue(hcc_handler, DATA_LO_QUEUE);
    } else {
        ret += hcc_host_proc_rx_queue(hcc_handler, DATA_HI_QUEUE);
        ret += hcc_host_proc_rx_queue(hcc_handler, DATA_LO_QUEUE);
    }
    return ret;
}

hi_u8 hcc_get_thread_exit_flag(hi_void)
{
    return g_thread_exit_flag;
}

hi_void hcc_exit_task_thread(hcc_handler_stru* hcc)
{
    hi_u16 retry_time = 10000;
    g_thread_exit_flag = HI_TRUE;
    hcc_sched_transfer(hcc);
    while (g_thread_working && retry_time > 0) {
        usleep_range(DELAY_10_US, DELAY_10_US);
        retry_time--;
    }
}

static hi_s32 hcc_task_rx_thread(hi_void *data)
{
    hi_s32 hcc_ret;
#ifdef _PRE_WLAN_TCP_OPT
    static hi_u8 ack_loop_count;
#endif
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)data;

    allow_signal(SIGTERM);
    oam_info_log0("hcc_task_rx_thread:: hcc_task_rx_thread enter");
    g_thread_exit_flag = HI_FALSE;
    for (;;) {
        if (g_thread_exit_flag == HI_TRUE || oal_kthread_should_stop()) {
            oam_warning_log0("hcc_task_thread:: hcc_task leave");
            break;
        }
        hcc_ret = hi_wait_event_interruptible(hcc_handler->hcc_transer_info.hcc_rx_wq,
            (hcc_thread_rx_wait_event_cond_check(hcc_handler) == HI_TRUE));
        if (hcc_ret == -ERESTARTSYS || g_thread_exit_flag == HI_TRUE) {
            oam_info_log0("hcc_task_rx_thread:: hcc_task was interupterd by a singnal");
            break;
        }

        hcc_thread_process(hcc_handler, RX_READ);
    }
    return 0;
}

/*****************************************************************************S
 hcc task 线程主程序
*****************************************************************************/
static hi_s32 hcc_task_tx_thread(hi_void *data)
{
    hi_s32 hcc_ret = 0;
    hcc_handler_stru *hcc_handler = (hcc_handler_stru *)data;

    allow_signal(SIGTERM);
    oam_info_log0("hcc_task_tx_thread:: hcc_task_tx_thread enter");
    g_thread_exit_flag = HI_FALSE;
    g_thread_working = HI_TRUE;
    for (;;) {
        if (g_thread_exit_flag == HI_TRUE || oal_kthread_should_stop()) {
            oam_warning_log0("hcc_task_tx_thread:: hcc_task leave");
            break;
        }
        hcc_ret = hi_wait_event_interruptible(hcc_handler->hcc_transer_info.hcc_tx_wq,
            (hcc_thread_tx_wait_event_cond_check(hcc_handler) == HI_TRUE));
        if (hcc_ret == -ERESTARTSYS || g_thread_exit_flag == HI_TRUE) {
            oam_info_log0("hcc_task_tx_thread:: hcc_task was interupterd by a singnal");
            break;
        }

        hcc_thread_process(hcc_handler, TX_WRITE);
    }
    g_thread_working = HI_FALSE;
    return hcc_ret;
}

hi_u32 hcc_task_init(hcc_handler_stru *hcc_handler)
{
    oal_kthread_param_stru thread_param = {0};

    memset_s(&g_hcc_task, sizeof(hcc_task_stru), 0, sizeof(hcc_task_stru));

    hi_wait_queue_init_head(&g_hcc_task.hcc_wq);

    memset_s(&thread_param, sizeof(oal_kthread_param_stru), 0, sizeof(oal_kthread_param_stru));
    thread_param.l_cpuid = 0;
    thread_param.l_policy = OAL_SCHED_FIFO;
    thread_param.l_prio = HCC_TASK_PRIO;
    thread_param.ul_stacksize = HCC_TASK_SIZE;

    hcc_handler->hcc_transer_info.hcc_transfer_thread = oal_kthread_create(HCC_TASK_TX_NAME,
        hcc_task_tx_thread, hcc_handler, &thread_param);
    hcc_handler->hcc_transer_info.hcc_rx_thread = oal_kthread_create(HCC_TASK_RX_NAME,
        hcc_task_rx_thread, hcc_handler, &thread_param);
    if (IS_ERR_OR_NULL(hcc_handler->hcc_transer_info.hcc_transfer_thread) ||
        IS_ERR_OR_NULL(hcc_handler->hcc_transer_info.hcc_rx_thread)) {
        return HI_FAIL;
    }
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

