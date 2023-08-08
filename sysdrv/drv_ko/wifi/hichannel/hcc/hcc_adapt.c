/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc layer frw task.
 * Author: dujinxin
 * Create: 2020-09-28
 */
/* 头文件包含 */
#include "oal_netbuf.h"
#include "hcc_host.h"
#include "oal_mm.h"
#include "wal_net.h"
#include "wal_netlink.h"
#include "hcc_adapt.h"
#include "hcc_list.h"
#include "hi_types_base.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SLEEP_MIN_TIME 100
#define SLEEP_MAX_TIME 100
#define COMPLETE_TIMEOUT 1000
#define  MAX_TEST_PACKAGE_LEN         1500
#define oal_round_up(_old_len, _align)   ((((_old_len) + ((_align) - 1)) / (_align)) * (_align))
#ifdef _PRE_HICHANNEL_DEBUG
static hi_char g_test_buf[MAX_TEST_PACKAGE_LEN] = {0};
#endif
static atomic_t g_tx_num = {0};
static hi_void hcc_adapt_mem_free(hi_void *data);
static hi_void hcc_rx_test_speed(hcc_header_stru *hcc_hdr, hi_char* buf, int len);

hcc_unc_struc* hcc_structure_conversion(const hi_void* priv, hcc_stru_type type)
{
    oal_netbuf_stru *netbuf = HI_NULL;
    hcc_normal_struc* p_normal = HI_NULL;
    hcc_unc_struc *unc_buf = oal_memalloc(sizeof(hcc_unc_struc));
    if (unc_buf == HI_NULL) {
        return HI_NULL;
    }
    memset_s(unc_buf, sizeof(hcc_unc_struc), 0, sizeof(hcc_unc_struc));

    switch (type) {
        case NETBUF_STRU_TYPE:
            netbuf = (oal_netbuf_stru *)priv;
            unc_buf->buf = oal_netbuf_data(netbuf);
            unc_buf->length = oal_netbuf_len(netbuf);
            unc_buf->priv = (hi_void *)priv;
            unc_buf->priv_type = NETBUF_STRU_TYPE;
            unc_buf->msg_num = atomic_add_return(1, &g_tx_num);
            unc_buf->free = hcc_adapt_mem_free;
            break;
        case NORMAL_STRU_TYPE:
            p_normal = (hcc_normal_struc *)priv;
            unc_buf->buf = p_normal->buf;
            unc_buf->length = p_normal->length;
            unc_buf->priv = NULL;
            unc_buf->priv_type = NORMAL_STRU_TYPE;
            unc_buf->msg_num = atomic_add_return(1, &g_tx_num);
            unc_buf->free = hcc_adapt_mem_free;
            break;
        default:
            oal_free(unc_buf);
            unc_buf = HI_NULL;
            break;
    }
    return unc_buf;
}

static hi_void hcc_adapt_netbuf_rx_data_process(oal_netbuf_stru *netbuf, hcc_header_stru *hcc_hdr)
{
    oal_netbuf_next(netbuf) = HI_NULL;
    oal_netbuf_prev(netbuf) = HI_NULL;
    oal_netbuf_pull(netbuf, HCC_HDR_TOTAL_LEN + hcc_hdr->pad_align);
    oal_netbuf_len(netbuf) = hcc_hdr->pay_len;

    switch (hcc_hdr->main_type) {
        case HCC_TYPE_DATA:
            if (wal_rx_data_proc(netbuf) != HI_SUCCESS) {
                oam_info_log0("hcc_rx_data_process:: wal_rx_data_proc failed");
            }
            break;
        case HCC_TYPE_TEST_XFER:
            hcc_rx_test_speed(hcc_hdr, oal_netbuf_data(netbuf), hcc_hdr->pay_len);
            oal_netbuf_free(netbuf);
            break;
        case HCC_TYPE_MSG:
            if (oal_send_user_msg(oal_netbuf_data(netbuf), oal_netbuf_len(netbuf)) != HI_SUCCESS) {
                oam_error_log0("hcc_rx_data_process:: oal_user_msg_event failed");
            }
            oal_netbuf_free(netbuf);
            break;
        default:
            oam_error_log0("hcc_rx_data_process:: unknown main type.");
            oal_netbuf_free(netbuf);
            break;
    }
}

static hi_u32 hcc_host_check_hdr(hcc_header_stru *hcc_hdr)
{
    if (hcc_hdr->main_type >= HCC_TYPE_BUFF) {
        return HI_FALSE;
    }
    return HI_TRUE;
}

/*****************************************************************************
 功能描述  : 处理从device接收的消息
 输入参数  : unc_buf 通用buf,存放各种私有数据结构，根据不同的数据结构调用不
             同的处理函数
 注意事项  : 以后要扩展其他类型，只需case分支中新增处理函数
 返 回 值  : HI_SUCCESS 消息处理成功， HI_FAIL 消息处理失败，消息头异常
*****************************************************************************/
static hi_u32 hcc_adapt_rx_data_process(hcc_unc_struc *unc_buf)
{
    oal_netbuf_stru *netbuf = HI_NULL;
    hcc_header_stru *hcc_hdr = (hcc_header_stru *)oal_unc_data(unc_buf);
    if (hcc_host_check_hdr(hcc_hdr) != HI_TRUE) {
        unc_buf->free(unc_buf);
        return HI_FAIL;
    }

    switch (oal_unc_priv_type(unc_buf)) {
        case NETBUF_STRU_TYPE:
            netbuf = (oal_netbuf_stru *)oal_unc_priv(unc_buf);
            hcc_adapt_netbuf_rx_data_process(netbuf, hcc_hdr);
            oal_free(unc_buf);
            break;
        default:
            unc_buf->free(unc_buf);
            break;
    }
    return HI_SUCCESS;
}

hi_u32 hcc_tx_hcc_hdr_init(hcc_unc_struc *unc_buf, const hcc_transfer_param *param)
{
    hcc_header_stru *hcc_hdr = HI_NULL;
    hi_u32 payload_len;

    /* calculate the pad lengh to ensure the hcc_total_len is 64Bytes */
    payload_len = oal_unc_len(unc_buf) - HCC_HDR_TOTAL_LEN;

    hcc_hdr = (hcc_header_stru *)oal_unc_data(unc_buf);
    if (hcc_hdr == HI_NULL) {
        return HI_FAIL;
    }

    hcc_hdr->main_type = param->main_type;
    hcc_hdr->sub_type = param->sub_type;
    hcc_hdr->pay_len = payload_len;
    hcc_hdr->pad_hdr = HCC_HDR_RESERVED_MAX_LEN - param->extend_len;
    hcc_hdr->pad_align = 0;   /* Device alloc netbuf's payload all 4B aligned! */

    return HI_SUCCESS;
}

static hi_u32 hcc_host_tx(hcc_handler_stru *hcc_handler, oal_netbuf_stru *netbuf, const hcc_transfer_param *param)
{
    hi_u32 ret;
    hcc_trans_queue_stru *hcc_queue = HI_NULL;
    hcc_unc_struc *unc_buf = HI_NULL;
    hcc_queue = &hcc_handler->hcc_transer_info.hcc_queues[HCC_TX].queues[param->queue_id];

    unc_buf = hcc_structure_conversion(netbuf, NETBUF_STRU_TYPE);
    if (unc_buf == HI_NULL) {
        return HI_FAIL;
    }

    /* 1. build hcc header */
    ret = hcc_tx_hcc_hdr_init(unc_buf, param);
    if (ret != HI_SUCCESS) {
        oal_free(unc_buf);
        return HI_FAIL;
    }

    /* stop tcpip tx queue */
    if ((hcc_handler->hcc_bus_ops != HI_NULL) &&
        (hcc_handler->hcc_bus_ops->stop_tcpip_tx_queue != HI_NULL)) {
        hcc_handler->hcc_bus_ops->stop_tcpip_tx_queue(hcc_queue);
    }

    /* 注册回调接口，丢包操作 */
    if (hcc_handler->hcc_bus_ops != HI_NULL &&
        hcc_handler->hcc_bus_ops->tx_discard_key_frame) {
        ret = hcc_handler->hcc_bus_ops->tx_discard_key_frame(param->queue_id, hcc_queue, unc_buf);
        if (ret == HI_SUCCESS) {
            oal_netbuf_free((oal_netbuf_stru*)unc_buf->priv);
            oal_free(unc_buf);
            return HI_SUCCESS;
        }
    }

    /* 注册回调接口，处理关键帧排序,可以将关键帧插入队列前面 */
    if (hcc_handler->hcc_bus_ops != HI_NULL &&
        hcc_handler->hcc_bus_ops->tx_sort_key_frame) {
        ret = hcc_handler->hcc_bus_ops->tx_sort_key_frame(param->queue_id, hcc_queue, unc_buf);
        if (ret != HI_SUCCESS) {
            hcc_list_add_tail(&hcc_queue->queue_info, unc_buf);
        }
    } else {
        hcc_list_add_tail(&hcc_queue->queue_info, unc_buf);
    }

    /* 4. sched hcc tx */
    hcc_sched_transfer(hcc_handler);
    return HI_SUCCESS;
}

/* SDIO 传输网络数据适配预处理 */
hi_u32 hcc_tx_data_adapt(oal_netbuf_stru *netbuf, hcc_type_enum type, hi_u32 sub_type)
{
    hi_u32 ret;
    hcc_transfer_param param;
    hcc_queue_type_enum queue_id = (type == HCC_TYPE_MSG) ? DATA_HI_QUEUE : DATA_LO_QUEUE;

    if (oal_netbuf_headroom(netbuf) < HCC_HDR_TOTAL_LEN) {
        oam_error_log1("hcc_tx_data_adapt:: headroom is not enough, headroom[%d]", oal_netbuf_headroom(netbuf));
        return HI_FAIL;
    }

    oal_netbuf_push(netbuf, HCC_HDR_TOTAL_LEN);

    hcc_hdr_param_init(&param, type, sub_type, 0, HCC_FC_NONE, queue_id);

    ret = hcc_host_tx(hcc_host_get_handler(), netbuf, &param);
    if (ret != HI_SUCCESS) {
        oam_error_log1("hcc_tx_data_adapt:: hcc_host_tx failed[%d]", ret);
        return HI_FAIL;
    }
    return HI_SUCCESS;
}

static hcc_unc_struc *hcc_adapt_netbuf_len_align(hcc_unc_struc *unc_buf, oal_netbuf_stru *netbuf, int align_len)
{
    hi_s32 ret;
    hi_u32 len_algin, tail_room_len;
    hi_u32 len = oal_netbuf_len(netbuf);
    if (oal_is_aligned(len, align_len)) {
        return unc_buf;
    }
    /* align the netbuf */
    len_algin = oal_round_up(len, align_len);
    if (len_algin < len) {
        oam_error_log2("hcc_netbuf_len_align::len_aglin[%d],len[%d]", len_algin, len);
        oal_free(unc_buf);
        return HI_NULL;
    }
    tail_room_len = len_algin - len;
    if (oal_unlikely(tail_room_len > oal_netbuf_tailroom(netbuf))) {
        /* tailroom not enough */
        ret = oal_netbuf_expand_head(netbuf, 0, (hi_s32)tail_room_len, GFP_KERNEL);
        if (oal_warn_on(ret != HI_SUCCESS)) {
            oal_free(unc_buf);
            return HI_NULL;
        }
    }
    oal_netbuf_put(netbuf, tail_room_len);
    /* 重新设置unc配置 */
    unc_buf->buf = oal_netbuf_data(netbuf);
    unc_buf->length = oal_netbuf_len(netbuf);
    unc_buf->priv = (hi_void *)netbuf;
    unc_buf->priv_type = NETBUF_STRU_TYPE;
    unc_buf->free = hcc_adapt_mem_free;
    return unc_buf;
}

static hcc_unc_struc* hcc_adapt_netbuf_alloc(hi_s32 len)
{
    oal_netbuf_stru *netbuf;
    netbuf = __netdev_alloc_skb(HI_NULL, len, GFP_KERNEL);
    if (netbuf == HI_NULL) {
        oam_error_log1("{[WIFI][E]rx no mem:%u}", len);
        return NULL;
    }

    oal_netbuf_put(netbuf, len);
    /* 重新设置unc配置 */
    return hcc_structure_conversion(netbuf, NETBUF_STRU_TYPE);
}

static hcc_unc_struc* hcc_adapt_nomal_alloc(hi_s32 len)
{
    hcc_normal_struc p_normal;

    p_normal.buf = oal_memalloc(len);
    if (p_normal.buf  == HI_NULL) {
        return HI_NULL;
    }
    p_normal.length = len;
    /* 重新设置unc配置 */
    return hcc_structure_conversion(&p_normal, NORMAL_STRU_TYPE);
}

/*****************************************************************************
 功能描述  : 检查是否需要丢帧，如果插入TX队列的数据太多，可丢掉一些非关键数据
 输入参数  : queue_id 队列id
             hcc_queue队列头指针
             unc_buf 通用数据结构指针
 返 回 值  : HI_SUCCESS 可以丢掉此帧，HI_FAIL此帧不能丢掉
*****************************************************************************/
static hi_u32 hcc_adapt_check_discard_frame(hi_u32 queue_id,
    hcc_trans_queue_stru *hcc_queue, hcc_unc_struc *unc_buf)
{
    hcc_header_stru *hcc_hdr = HI_NULL;
    if (queue_id != DATA_LO_QUEUE) {
        return HI_FAIL;
    }

    hcc_hdr = (hcc_header_stru *)oal_unc_data(unc_buf);
    if (hcc_hdr->main_type == HCC_TYPE_TEST_XFER &&
        hcc_queue->queue_info.qlen > 0x1000) {
        usleep_range(SLEEP_MIN_TIME, SLEEP_MAX_TIME);
    }
    return HI_FAIL;
}

/*****************************************************************************
 功能描述  : 释放私有数据结构类型
 输入参数  : data 通用数据结构指针
 返 回 值  : 无
*****************************************************************************/
static hi_void hcc_adapt_mem_free(hi_void *data)
{
    oal_netbuf_stru *netbuf = HI_NULL;
    hcc_unc_struc* unc_buf = (hcc_unc_struc*) data;
    if (unc_buf == HI_NULL) {
        return;
    }

    switch (unc_buf->priv_type) {
        case NETBUF_STRU_TYPE:
            netbuf = (oal_netbuf_stru *)unc_buf->priv;
            if (netbuf) {
                oal_netbuf_free(netbuf);
            }
            break;
        case NORMAL_STRU_TYPE:
            if (unc_buf->buf) {
                oal_free(unc_buf->buf);
            }
            break;
        default:
            oam_error_log1("unknown structure type:%d", unc_buf->priv_type);
            break;
    }
    oal_free(unc_buf);
}

/*****************************************************************************
 功能描述  : stop tcpip tx_queue
*****************************************************************************/
hi_void hcc_adapt_stop_tcpip_tx_queue(hcc_trans_queue_stru *hcc_queue)
{
    if ((hcc_list_len(&hcc_queue->queue_info) > MAX_CNT_IN_QUEUE) &&
        (hcc_queue->queue_info.flow_flag == HI_FALSE)) {
        oal_net_device_stru *netdev = oal_get_netdev_by_name("wlan0");
        if (netdev == HI_NULL) {
            oam_error_log0("hcc_adapt_stop_tcpip_tx_queue:: netdev is NULL");
            return;
        }
        oal_dev_put(netdev);
        oal_netif_stop_queue(netdev);
        hcc_queue->queue_info.flow_flag = HI_TRUE;
        oal_netif_stop_queue(netdev);
    }
}

/*****************************************************************************
 功能描述  : awake tcpip tx_queue
*****************************************************************************/
hi_void hcc_adapt_awake_tcpip_tx_queue(hcc_trans_queue_stru *hcc_queue)
{
    if ((hcc_list_len(&hcc_queue->queue_info) < AWAKE_CNT_IN_QUEUE) && (hcc_queue->queue_info.flow_flag == HI_TRUE)) {
        oal_net_device_stru *netdev = oal_get_netdev_by_name("wlan0");
        if (netdev == HI_NULL) {
            oam_error_log0("hcc_adapt_awake_tcpip_tx_queue:: netdev is NULL");
            return;
        }
        oal_dev_put(netdev);
        oal_netif_wake_queue(netdev);
        hcc_queue->queue_info.flow_flag = HI_FALSE;
    }
}

/*****************************************************************************
 功能描述  : awake tcp/ip tx_queue
*****************************************************************************/

/*****************************************************************************
 功能描述  : 针对不通结构类型的结构体做对齐操作
 输入参数  : unc_buf 通用数据结构指针
             align_len 对齐长度
 注意事项  ：如果有些数据结构已经有对齐，可以直接返回
 返 回 值  : 返回通用数据结构指针
*****************************************************************************/
static hcc_unc_struc *hcc_adapt_len_align(hcc_unc_struc *unc_buf, int align_len)
{
    oal_netbuf_stru *netbuf = HI_NULL;
    hcc_unc_struc *unc_buf_t = unc_buf;
    switch (oal_unc_priv_type(unc_buf)) {
        case NETBUF_STRU_TYPE:
            netbuf = (oal_netbuf_stru *)oal_unc_priv(unc_buf);
            unc_buf_t = hcc_adapt_netbuf_len_align(unc_buf, netbuf, align_len);
            break;
        default:
            break;
    }
    return unc_buf_t;
}

/*****************************************************************************
 功能描述  : 申请私有数据结构内存，并转换为通用数据结构
 输入参数  : len 申请长度，单位：Byte
             type 申请的私有数据结构类型
 注意事项  ：后续要扩展，可在case分支新增其他数据结构类型
 返 回 值  : 返回通用数据结构指针
*****************************************************************************/
static hcc_unc_struc* hcc_adapt_alloc_unc_buf(hi_s32 len, hcc_stru_type type)
{
    switch (type) {
        case NETBUF_STRU_TYPE:
            return hcc_adapt_netbuf_alloc(len);
        case NORMAL_STRU_TYPE:
            return hcc_adapt_nomal_alloc(len);
        default:
            break;
    }
    return HI_NULL;
}

/*****************************************************************************
 功能描述  : 流控处理函数
 输入参数  : flow_type 数据类型，HCC_FLOWCTRL_CREDIT为管理帧，HCC_FLOWCTRL_SDIO
             为数据帧
             dev_mem_cnt device内存资源数
 返 回 值  : HI_SUCCESS 内存充足，继续发送数据， HI_FAIL device内存不足，停止发
             送数据
*****************************************************************************/
static hi_u32 hcc_adapt_tx_flow_ctrl_handle(hi_u16 flow_type, hi_u8 dev_mem_cnt)
{
    if (flow_type == HCC_FLOWCTRL_CREDIT) {
        if (dev_mem_cnt <= HI_PRI_MEM_LOW_LEVEL) {
            return HI_FAIL;
        }

    } else if (flow_type == HCC_FLOWCTRL_SDIO) {
        if (dev_mem_cnt <= LOW_PRI_MEM_LOW_LEVEL) {
            return HI_FAIL;
        }
    }
    return HI_SUCCESS;
}

/*****************************************************************************
 功能描述  : 测试hcc通道读速率，统计从device收到包的个数。
 输入参数  : hcc_hdr hcc消息头
             buf 存放数据缓冲区
             len 缓冲区存放数据大小
 返 回 值  : 无
*****************************************************************************/
static hi_void hcc_rx_test_speed(hcc_header_stru *hcc_hdr, hi_char* buf, int len)
{
    if (len < sizeof(hi_s32)) {
        return;
    }

    if (hcc_hdr->sub_type == HCC_TEST_READ_START) {
        oam_error_log1("Read start.package:%d", *((hi_u32*)buf));
    } else if (hcc_hdr->sub_type == HCC_TEST_READ_TRANFER) {
        oam_error_log1("Package Num:%d", *((hi_u32*)buf));
    } else if (hcc_hdr->sub_type == HCC_TEST_READ_OVER) {
        oam_error_log1("Read over.package:%d", *((hi_u32*)buf));
    }
}

#ifdef _PRE_HICHANNEL_DEBUG
static void hcc_test_write(char* buf, int len, hcc_type_enum type, hi_u32 sub_type)
{
    oal_netbuf_stru *netbuf = HI_NULL;

    netbuf = oal_netbuf_alloc(len + HCC_HDR_TOTAL_LEN + HIBUS_H2D_SCATT_BUFFLEN_ALIGN, 0, 4); /* 4: 4字节对齐 */
    if (netbuf == HI_NULL) {
        oam_error_log0("hcc_test_write:: netbuf_alloc failed!");
        return;
    }

    oal_netbuf_put(netbuf, len);
    if (memcpy_s(oal_netbuf_data(netbuf), len, buf, len) != EOK) {
        oam_error_log0("hcc_test_write:: memcpy_s failed!");
        oal_netbuf_free(netbuf);
        return;
    }

    hi_u32 ret = hcc_tx_data_adapt(netbuf, type, sub_type);
    if (ret != HI_SUCCESS) {
        oam_error_log0("hcc_test_write:: hcc_tx_data_adapt failed");
        oal_netbuf_free(netbuf);
        return;
    }
}

/*****************************************************************************
 功能描述  : 测试hcc通道写速率
 输入参数  : 无
 返 回 值  : 无
*****************************************************************************/
hi_void hcc_test_channel_speed(hi_void)
{
    int package_cnt = 100000;
    int ret;
    struct timeval start_tv, end_tv;
    hi_u32 len;
    hi_u32 mlen;
    int speed;

    do_gettimeofday(&start_tv);
    printk("test write start.\n");
    memset_s(g_test_buf, sizeof(g_test_buf), 0x55, sizeof(g_test_buf));
    hcc_test_write((char*)&package_cnt, sizeof(package_cnt), HCC_TYPE_TEST_XFER, HCC_TEST_WRITE_START);
    for (int i = 0; i < package_cnt; i++) {
        hcc_test_write(g_test_buf, sizeof(g_test_buf), HCC_TYPE_TEST_XFER, HCC_TEST_WRITE_TRANFER);
    }
    hcc_test_write((char*)&package_cnt, sizeof(package_cnt), HCC_TYPE_TEST_XFER, HCC_TEST_WRITE_OVER);

    ret = oal_wait_for_completion_timeout(&hcc_host_get_handler()->hcc_transer_info.hcc_test_tx, COMPLETE_TIMEOUT * HZ);
    if (ret == 0) {
        printk("hcc_task_tx_thread:: hcc_task was interupterd by a singnal\n");
    }
    do_gettimeofday(&end_tv);
    len = (package_cnt * MAX_TEST_PACKAGE_LEN);
    mlen = 1024 * 1024;  /* 1024: bit, Kb, Mb 之间转换单元 */
    speed = len / mlen;
    printk("test write over. speed:%ld Mbps\n", (speed / (end_tv.tv_sec - start_tv.tv_sec)) * 8); /* 8: Byte,bit转换 */
}
#endif

static struct hcc_bus_adpta_ops g_hcc_bus_opt = {
    .rx_proc_queue          = hcc_adapt_rx_data_process,
    .tx_discard_key_frame   = hcc_adapt_check_discard_frame,
    .tx_sort_key_frame      = HI_NULL,
    .tx_flow_ctrl_handle    = hcc_adapt_tx_flow_ctrl_handle,
    .wlan_pm_set_packet_cnt = HI_NULL,
    .private_len_align      = hcc_adapt_len_align,
    .alloc_unc_buf          = hcc_adapt_alloc_unc_buf,
    .free_unc_buf           = hcc_adapt_mem_free,
    .wlan_pm_wakeup_dev     = HI_NULL,
#ifdef _PRE_HICHANNEL_DEBUG
    .channel_rx_test        = hcc_rx_test_speed,
#endif
    .stop_tcpip_tx_queue    = hcc_adapt_stop_tcpip_tx_queue,
    .awake_tcpip_tx_queue   = hcc_adapt_awake_tcpip_tx_queue
};

hi_u32 hcc_adapt_init(hi_void)
{
    return hcc_host_init(&g_hcc_bus_opt);
}

hi_void hcc_adapt_exit(hi_void)
{
    hcc_host_exit(hcc_host_get_handler());
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

