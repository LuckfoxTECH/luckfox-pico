/**
 ******************************************************************************
 *
 * @file rwnx_tx.c
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */
#include <linux/dma-mapping.h>
#include <linux/etherdevice.h>
#include <net/sock.h>

#include "rwnx_defs.h"
#include "rwnx_tx.h"
//#include "rwnx_msg_tx.h"
//#include "rwnx_mesh.h"
#include "rwnx_events.h"
#include "rwnx_compat.h"
#include "aicwf_txrxif.h"


/******************************************************************************
 * Power Save functions
 *****************************************************************************/
/**
 * rwnx_set_traffic_status - Inform FW if traffic is available for STA in PS
 *
 * @rwnx_hw: Driver main data
 * @sta: Sta in PS mode
 * @available: whether traffic is buffered for the STA
 * @ps_id: type of PS data requested (@LEGACY_PS_ID or @UAPSD_ID)
  */
void rwnx_set_traffic_status(struct rwnx_hw *rwnx_hw,
                             struct rwnx_sta *sta,
                             bool available,
                             u8 ps_id)
{
    #if 0
    if (sta->tdls.active) {
        rwnx_send_tdls_peer_traffic_ind_req(rwnx_hw,
                                            rwnx_hw->vif_table[sta->vif_idx]);
    } else {
        bool uapsd = (ps_id != LEGACY_PS_ID);
        rwnx_send_me_traffic_ind(rwnx_hw, sta->sta_idx, uapsd, available);
        trace_ps_traffic_update(sta->sta_idx, available, uapsd);
    }
    #endif
}

#ifdef CONFIG_VNET_MODE
extern aicwf_custom_msg_vnet g_custom_msg_vnet;
netdev_tx_t rwnx_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    int ret;
    //printk("rwnx_start_xmit %d \r\n",  skb->len);
    if (g_custom_msg_vnet.wlan_status == WLAN_DISCONNECT &&
            g_custom_msg_vnet.ap_status == AIC_AP_CLOSE) {
        txrx_err("wlan is not connected=%d\n", g_custom_msg_vnet.wlan_status);
        aicwf_dev_skb_free(skb);
        return NETDEV_TX_OK;
    }
    #ifdef AICWF_USB_SUPPORT
    ret = aicwf_frame_tx((void *)(g_rwnx_plat->usbdev), skb);
    #endif

    #ifdef AICWF_SDIO_SUPPORT
    ret = aicwf_frame_tx((void *)(g_rwnx_plat->sdiodev), skb);
    #endif

    if (ret) {
        txrx_err("frame_tx ret=%d\n", ret);
        //return NETDEV_TX_BUSY;
    }

    return NETDEV_TX_OK;
}

#elif defined(CONFIG_RAWDATA_MODE)
int rwnx_tx_data(struct sk_buff *skb)
{
    int ret;
    #ifdef AICWF_USB_SUPPORT
    ret = aicwf_frame_tx((void *)(g_rwnx_plat->usbdev), skb);
    #endif

    #ifdef AICWF_SDIO_SUPPORT
    ret = aicwf_frame_tx((void *)(g_rwnx_plat->sdiodev), skb);
    #endif

    return ret;
}
#endif

#define HOST_DATA_HDR_LEN   (offsetof(host_data_t, payload))
void rwnx_tx_msg(u8 * msg,  u16 msg_len)
{
    void *p_dev = NULL;
    u8_l type = 0;
    char *buf = NULL;
    struct aicwf_bus *bus = NULL;
    host_data_t *p_host_data = NULL;

    if (!msg) {
        return;
    }

    #ifdef AICWF_SDIO_SUPPORT
    p_dev = g_rwnx_plat->sdiodev;
    bus   = g_rwnx_plat->sdiodev->bus_if;
    type  = SDIO_TYPE_CFG;
    #endif
    #ifdef AICWF_USB_SUPPORT
    p_dev = g_rwnx_plat->usbdev;
    bus   = g_rwnx_plat->usbdev->bus_if;
    type  = USB_TYPE_CFG;
    #endif

    buf = bus->cmd_buf;
    p_host_data = (host_data_t *)(buf);
    p_host_data->ptype	= type;
    p_host_data->plen	= (msg_len + 4);

    memcpy((buf + HOST_DATA_HDR_LEN), msg, msg_len);
    aicwf_msg_tx(p_dev, buf, (msg_len + 8));
}
