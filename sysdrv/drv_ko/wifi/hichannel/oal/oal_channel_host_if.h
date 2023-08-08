/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hcc layer frw task.
 * Author: dujinxin
 * Create: 2020-09-28
 */
#ifndef __OAL_CHANNEL_HOST_IF_H__
#define __OAL_CHANNEL_HOST_IF_H__

/*****************************************************************************
  1 Include other Head file
*****************************************************************************/
#include "sdio_host.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define oal_get_channel_func(pst_channel)                           ((oal_channel_stru *)pst_channel)->func
#define oal_get_channel_default_handler()                           oal_get_sdio_default_handler()
#define oal_channel_build_rx_list(pst_channel, head) oal_sdio_build_rx_list((oal_channel_stru *)pst_channel, head)
#define oal_channel_transfer_list(pst_channel, head, rw)            \
    oal_sdio_transfer_list((oal_channel_stru *)pst_channel, head, rw)
#define oal_channel_message_register(pst_channel, msg, cb, data)    \
    oal_sdio_message_register((oal_channel_stru *)pst_channel, msg, cb, data)
#define oal_channel_message_unregister(pst_channel, msg)            \
    oal_sdio_message_unregister((oal_channel_stru *)pst_channel, msg)
#define oal_channel_claim_host(pst_channel)                         oal_sdio_claim_host((oal_channel_stru *)pst_channel)
#define oal_channel_release_host(pst_channel)              oal_sdio_release_host((oal_channel_stru *)pst_channel)
#define oal_channel_send_msg(pst_channel, val)             oal_sdio_send_msg((oal_channel_stru *)pst_channel, val)
#define oal_disable_channel_state(pst_channel, mask)       oal_disable_sdio_state((oal_channel_stru *)pst_channel, mask)
#define oal_channel_init(pdata, card, bus_ops)             oal_sdio_init(pdata, card, bus_ops)
#define oal_channel_exit(pst_channel)                      oal_sdio_exit((oal_channel_stru *)pst_channel)
#define oal_channel_get_credit(pst_channel, mem_pool_cnt)  \
    oal_sdio_get_credit((oal_channel_stru *)pst_channel, mem_pool_cnt)
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
