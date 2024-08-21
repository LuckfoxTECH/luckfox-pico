/*
 * Copyright (c) 2021 iComm-semi Ltd.
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

/**
 * @file fmac_msg_tx.c
 * @brief TX function definitions
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <linux/types.h>

#include "fmac/lmac_types.h"
#include "fmac/fmac.h"
#include "hci/drv_hci_ops.h"
#include "ipc_msg.h"
#include "ssv_efuse.h"
#include "ssv_phy_rf.h"
#include "ssv_rftool_msg.h"
#include "ssv_debug.h"

/*******************************************************************************
 *         Local Defines
 ******************************************************************************/


/*******************************************************************************
 *         Local Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Local Structures
 ******************************************************************************/


/*******************************************************************************
 *         Global Variables
 ******************************************************************************/

/*******************************************************************************
 *         Local Variables
 ******************************************************************************/

/*******************************************************************************
 *         Local Functions
 ******************************************************************************/

static int ssv_rftool_drv_hci_tx(struct ssv_rftool_softc *srcc, struct sk_buff *skb, int txqid, bool force_trigger, u32 tx_flags)
{
    if ((NULL == srcc->hci_priv) || (NULL == srcc->hci_ops->hci_tx))
        return -1;

    return srcc->hci_ops->hci_tx(srcc->hci_priv, skb, txqid, force_trigger, tx_flags);
}

int ssv_rftool_private_msg_to_hci(struct ssv_rftool_softc *srfc, u8 *msg_buffer, u32 msg_len)
{
    struct sk_buff *skb = NULL;
    struct tx_bmu_desc *tx_bmu_hdr = NULL;
    struct sdio_hdr *sdio_hdr = NULL;
    struct txdesc_api *txdec_hdr = NULL;
    u16 headroom =  SSV_TX_HDR_SIZE;
    u16 frame_len = headroom + msg_len;
    u32 frame_oft = 0;

    if(1)
    {
        skb = __dev_alloc_skb(frame_len, GFP_KERNEL); 
        if (NULL == skb)
        {
            SSV_LOG_DBG("%s(): Can't alloc skb.\n", __FUNCTION__);
            return -1;
        }
        skb_put(skb, frame_len);
        memset((void *)skb->data, 0, skb->len);

        /* |                                                                | 
        * |<----------         send to HCI               ----------------->|
        * |                                                                | 
        * +-------------+----------+------------+--------------------------+ 
        * | tx_bmu_desc | sdio_hdr | txdesc_api |     MSDU Frame           |
        * +-------------+----------+------------+--------------------------+ 
        *  
        * |<--- skb->data
        * |<------------------------  skb->len --------------------------->| 
        * | 
        *
        */

        /* add tx header and payload */
        //build tx_bmu_hdr
        tx_bmu_hdr = (struct tx_bmu_desc *)(skb->data);
        ssv_build_tx_bmu_header(tx_bmu_hdr, frame_len);
        frame_oft += sizeof(struct tx_bmu_desc);

        //build sdio_hdr
        sdio_hdr =(struct sdio_hdr*)(skb->data+frame_oft);
        sdio_hdr->type = E_IPC_TYPE_PRIV_MSG;
        sdio_hdr->len = frame_len;
        sdio_hdr->queue_idx = 0;
        sdio_hdr->reserved = 0;
        frame_oft += sizeof(struct sdio_hdr);

        //build "empty" txdec_api_hdr
        txdec_hdr = (struct txdesc_api*)(skb->data+frame_oft);
        frame_oft += sizeof(struct txdesc_api);

        //copy msg payload
        memcpy((void *)(skb->data+frame_oft), msg_buffer, msg_len);

    }
#ifdef CONFIG_HWIF_AND_HCI
    return ssv_rftool_drv_hci_tx(srfc, skb, SSV_SW_TXQ_ID_WIFI_CMD, true, 0);
#else
    dev_kfree_skb_any(skb);
    return 0;
#endif
}
#if 1
int ssv_rftool_event(void *app_param, struct sk_buff *skb)
{
    struct ssv_rftool_softc *srfc = (struct ssv_rftool_softc *)app_param;
    struct rx_info *rx_info = NULL;

    u32 *rx_desc = (u32 *)&skb->data[0];
    u8 pkt_type = (u8)((*rx_desc) & 0xFF);
    //u16 real_pkt_len = (u16)((*rx_desc) >> 8);

    skb_pull(skb, 4); //Remove HWIF header(4-bytes).

    rx_info = (struct rx_info *)skb->data;
    skb_pull(skb, sizeof(struct rx_info)); //Remove HW RX information(84-bytes).
    skb_pull(skb, RX_BUF_HEADROOM_SIZE); //Remove SW RX headroom(24-bytes).


    if (E_IPC_TYPE_PRIV_MSG == pkt_type) {
        ST_IPC_PRIV_MSG *msg = (ST_IPC_PRIV_MSG *)skb->data;
        //SSV_LOG_DBG("pkt type = %d, msg id = %d\n", pkt_type, msg->msgid);
        switch(msg->msgid)
        {
            case E_IPC_PRIV_MSG_TYPE_RSP_EFUSE_IND:
            {
                struct ssv_efuse_tool_param *param = (struct ssv_efuse_tool_param *)msg->data;
                ssv_res_update_to_cfg(param);
                complete(&srfc->ssv_rftool_cmd_done);
                dev_kfree_skb_any(skb);

                return 0;
            }
            
            case E_IPC_PRIV_MSG_TYPE_RSP_RF_IND:
            {
                struct ssv_rf_tool_param *param = (struct ssv_rf_tool_param *)msg->data;
                ssv_res_rf_update_to_cfg((void*)param);
                complete(&srfc->ssv_rftool_cmd_done);
                dev_kfree_skb_any(skb);
                
                return 0;
            }
        
            default:
            {
                goto unknown_msg;
            }
        }
    }

unknown_msg:
    skb_push(skb, RX_BUF_HEADROOM_SIZE); //recovery SW RX headroom(24-bytes).
    skb_push(skb, sizeof(struct rx_info)); //recovery HW RX information(84-bytes).
    skb_push(skb, 4); //recovery HWIF header(4-bytes).
    
    return -1;

}
#endif
