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
 * @file fmac_cmds.c
 * @brief Handles queueing (push to IPC, ack/cfm from IPC) of commands issued to LMAC FW
 */


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include <linux/list.h>
#include <linux/slab.h>

//#include "hwif/hwif.h"
//#include "hci/ssv_hci.h"
//#include "hci/hctrl.h"
#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "fmac_cmds.h"
#include "fmac_strs.h"
#include "fmac_tx.h"
#include "fmac_utils.h"
#include "ipc_host.h"
#include "ipc_msg.h"
#include "ssv_debug.h"


/*******************************************************************************
 *         Local Defines
 ******************************************************************************/
static void _ssv_cmd_complete(struct ssv_cmd_mgr *cmd_mgr, struct ssv_cmd *cmd);
static void _ssv_cmd_mgr_print(struct ssv_cmd_mgr *cmd_mgr);
static int _ssv_cmd_mgr_queue(struct ssv_cmd_mgr *cmd_mgr, struct ssv_cmd *cmd);
static int _ssv_cmd_mgr_llind(struct ssv_cmd_mgr *cmd_mgr, struct ssv_cmd *cmd);
static int _ssv_cmd_mgr_msgind(struct ssv_cmd_mgr *cmd_mgr, struct ipc_e2a_msg *msg, msg_cb_fct cb);
static void _ssv_cmd_mgr_drain(struct ssv_cmd_mgr *cmd_mgr);


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
static void _ssv_cmd_complete(struct ssv_cmd_mgr *cmd_mgr, struct ssv_cmd *cmd)
{
    lockdep_assert_held(&cmd_mgr->lock);

    list_del(&cmd->list);
    cmd_mgr->queue_sz--;

    cmd->flags |= SSV_CMD_FLAG_DONE;
    if (cmd->flags & SSV_CMD_FLAG_NONBLOCK) {
        kfree(cmd);
    } else {
        if (SSV_CMD_WAIT_COMPLETE(cmd->flags)) {
            cmd->result = 0;
            complete(&cmd->complete);
        }
    }
}



int ssv_private_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len)
{
    struct sk_buff *skb = NULL;
    struct tx_bmu_desc *tx_bmu_hdr = NULL;
    struct sdio_hdr *sdio_hdr = NULL;
    struct txdesc_api *txdec_hdr = NULL;
    u16 headroom =  SSV_TX_HDR_SIZE;
    u16 frame_len = headroom + msg_len;
    u32 frame_oft = 0;

    if(sc->fw_reset_run == true) {
        return -1;
    }
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
    if(sc->recovery_flag == true) {
        struct fw_reset_cmd *reset_cmd;
        reset_cmd = kzalloc(msg_len + sizeof(struct fw_reset_cmd) +1, GFP_KERNEL);
        reset_cmd->msg_type = E_IPC_TYPE_PRIV_MSG;
        reset_cmd->msg_len = msg_len;
        memcpy(reset_cmd->data, msg_buffer, msg_len);
        list_add_tail(&reset_cmd->list, &sc->reset_cmd.list);
    }
    return ssv_fmac_hci_tx(sc, skb, SSV_SW_TXQ_ID_WIFI_CMD, true, 0);
#else
    dev_kfree_skb_any(skb);
    return 0;
#endif
}

static void _ssv_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len)
{
    struct sk_buff *skb = NULL;
    struct tx_bmu_desc *tx_bmu_hdr = NULL;
    struct sdio_hdr *sdio_hdr = NULL;
    struct txdesc_api *txdec_hdr = NULL;
    u16 headroom =  SSV_TX_HDR_SIZE;
    u16 frame_len = headroom + msg_len;
    u32 frame_oft = 0;
    gfp_t flags;

    if(sc->fw_reset_run == true) {
        return ;
    }

    if(1)
    {
        if (in_softirq())
            flags = GFP_ATOMIC;
        else
            flags = GFP_KERNEL;
        
        skb = __dev_alloc_skb(frame_len, flags); 
        if (NULL == skb)
        {
            SSV_LOG_DBG("%s(): Can't alloc skb.\n", __FUNCTION__);
            return;
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
        sdio_hdr->type = E_IPC_TYPE_MSG;
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
    if(sc->recovery_flag == true) {
        struct fw_reset_cmd *reset_cmd;
        reset_cmd = kzalloc(msg_len + sizeof(struct fw_reset_cmd) +1, GFP_KERNEL);
        reset_cmd->msg_type = E_IPC_TYPE_MSG;
        reset_cmd->msg_len = msg_len;
        memcpy(reset_cmd->data, msg_buffer, msg_len);
        list_add_tail(&reset_cmd->list, &sc->reset_cmd.list);
    }
    ssv_fmac_hci_tx(sc, skb, SSV_SW_TXQ_ID_WIFI_CMD, true, 0);
#else
    dev_kfree_skb_any(skb);
#endif

    return;
}

void _ssv_fwreset_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len, u32 msg_type)
{
    struct sk_buff *skb = NULL;
    struct tx_bmu_desc *tx_bmu_hdr = NULL;
    struct sdio_hdr *sdio_hdr = NULL;
    struct txdesc_api *txdec_hdr = NULL;
    u16 headroom =  SSV_TX_HDR_SIZE;
    u16 frame_len = headroom + msg_len;
    u32 frame_oft = 0;
    gfp_t flags;

    SSV_LOG_DBG("_ssv_fwreset_msg_to_hci, msg_type = %d\n",msg_type);
    if(1)
    {
        if (in_softirq())
            flags = GFP_ATOMIC;
        else
            flags = GFP_KERNEL;
        
        skb = __dev_alloc_skb(frame_len, flags); 
        if (NULL == skb)
        {
            SSV_LOG_DBG("%s(): Can't alloc skb.\n", __FUNCTION__);
            return;
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
        sdio_hdr->type = msg_type;
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
    if(sc->recovery_flag == true) {
        struct fw_reset_cmd *reset_cmd;
        reset_cmd = kzalloc(msg_len + sizeof(struct fw_reset_cmd) +1, GFP_KERNEL);
        reset_cmd->msg_type = 1;
        reset_cmd->msg_len = msg_len;
        memcpy(reset_cmd->data, msg_buffer, msg_len);
        list_add_tail(&reset_cmd->list, &sc->reset_cmd.list);
    }
    ssv_fmac_hci_tx(sc, skb, SSV_SW_TXQ_ID_WIFI_CMD, true, 0);
#else
    dev_kfree_skb_any(skb);
#endif

    return;
}

static void ssv_msg_process_all(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len)
{
    _ssv_msg_to_hci(sc, msg_buffer, msg_len);
    return;
}

void ssv_msg_to_hci(struct ssv_softc *sc, void *msg_params)
{
    struct lmac_msg *msg;
     
    msg = container_of((void *)msg_params, struct lmac_msg, param);
    /* The message don't use cmd_queue path.
     * The message send to fw by hci tx task directly, and it don't wait cfm message.
     */
    _ssv_msg_to_hci(sc, (u8 *)msg, sizeof(struct lmac_msg) + msg->param_len);
    kfree(msg);
}

static int ssv_pending_msg_hdl(struct ssv_softc *sc)
{    
	struct ssv_cmd *cur = NULL;
	struct ssv_cmd *hostid = (struct ssv_cmd *)(sc->ipc_env->msga2e_hostid);

	list_for_each_entry(cur, &sc->cmd_mgr.cmds, list) {
			ssv_cmd_dump(cur);
			// SSV_LOG_DBG("cur->tkn=%d, hostid->tkn=%d, queue_sz=%d, max_queue_sz=%d\n", cur->tkn, hostid->tkn, sc->cmd_mgr.queue_sz, sc->cmd_mgr.max_queue_sz);
			if(cur->tkn == hostid->tkn) {
                ssv_msg_process_all(sc, (u8 *)(cur->a2e_msg), sizeof(struct lmac_msg) + cur->a2e_msg->param_len);
				kfree(cur->a2e_msg);
                //ACK by driver's self.
				{
					void *hostid = sc->ipc_env->msga2e_hostid;
					sc->ipc_env->msga2e_hostid = NULL;
					sc->ipc_env->cb.recv_msgack_ind(sc, hostid);
				}
				break;
			}
	}

	mutex_lock(&sc->cmd_lock);
	sc->cmd_sent = false;
	mutex_unlock(&sc->cmd_lock);

	return 0;

}

static int _ssv_cmd_mgr_queue(struct ssv_cmd_mgr *cmd_mgr, struct ssv_cmd *cmd)
{
    struct ssv_softc *sc= container_of(cmd_mgr, struct ssv_softc, cmd_mgr);
    unsigned long tout;
    bool defer_push = false;

    // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    mutex_lock(&cmd_mgr->lock);

    if (cmd_mgr->state == SSV_CMD_MGR_STATE_CRASHED) {
        SSV_LOG_ERR("cmd queue crashed\n");
        cmd->result = -EPIPE;
        mutex_unlock(&cmd_mgr->lock);
        return -EPIPE;
    }

    if (!list_empty(&cmd_mgr->cmds)) {
        if (cmd_mgr->queue_sz == cmd_mgr->max_queue_sz) {
            SSV_LOG_ERR("Too many cmds (%d) already queued\n",
                   cmd_mgr->max_queue_sz);
            cmd->result = -ENOMEM;
            mutex_unlock(&cmd_mgr->lock);
            return -ENOMEM;
        }
    }

    cmd->flags |= SSV_CMD_FLAG_WAIT_ACK;
    if (cmd->flags & SSV_CMD_FLAG_REQ_CFM)
        cmd->flags |= SSV_CMD_FLAG_WAIT_CFM;

    cmd->tkn    = cmd_mgr->next_tkn++;
    cmd->result = -EINTR;

    if (!(cmd->flags & SSV_CMD_FLAG_NONBLOCK))
        init_completion(&cmd->complete);

    list_add_tail(&cmd->list, &cmd_mgr->cmds);
    cmd_mgr->queue_sz++;
    tout = msecs_to_jiffies(SSV_80211_CMD_TIMEOUT_MS * cmd_mgr->queue_sz);

    if (!defer_push) {
		ASSERT_ERR(!(sc->ipc_env->msga2e_hostid));
		sc->ipc_env->msga2e_hostid = (void *)cmd;
		mutex_lock(&sc->cmd_lock);
		sc->cmd_sent = true;
		mutex_unlock(&sc->cmd_lock);
        ssv_pending_msg_hdl(sc);
    }
    mutex_unlock(&cmd_mgr->lock);

    // SSV_LOG_DBG("send: cmd:%4d-%-24s\n", cmd->id, SSV_ID2STR(cmd->id));

    if (!(cmd->flags & SSV_CMD_FLAG_NONBLOCK)) {
        if (!wait_for_completion_timeout(&cmd->complete, tout)) {
			SSV_LOG_DBG("wait for cmd cfm timeout!\n");
            mutex_lock(&cmd_mgr->lock);
            // cmd_mgr->state = SSV_CMD_MGR_STATE_CRASHED;
            if (!(cmd->flags & SSV_CMD_FLAG_DONE)) {
                cmd->result = -ETIMEDOUT;
                _ssv_cmd_complete(cmd_mgr, cmd);
            }
            mutex_unlock(&cmd_mgr->lock);
            ssv_cmd_dump(cmd);
        }
    } else {
        cmd->result = 0;
    }

    return (int)cmd->result;
}

static int _ssv_cmd_mgr_llind(struct ssv_cmd_mgr *cmd_mgr, struct ssv_cmd *cmd)
{
    struct ssv_cmd *cur, *acked = NULL;
	
    // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    /* 
     * _ssv_cmd_mgr_queue() includes "send command" and "ack" action.
     * _ssv_cmd_mgr_queue() uses cmd_mgr->lock to prevent the race condition happened.
     * "ack" action must remove cmd_mgr->lock to avoid deadlock.
     * */
    list_for_each_entry(cur, &cmd_mgr->cmds, list) {
        if (!acked) {
            if (cur->tkn == cmd->tkn) {
                if (WARN_ON_ONCE(cur != cmd)) {
					//_ssv_cmd_mgr_print(cmd_mgr);
                    ssv_cmd_dump(cmd);
                    ssv_cmd_dump(cur);
                }
                acked = cur;
                //continue;
                break;
            }
        }
    }
    
    if (!acked) {
        SSV_LOG_ERR("Error: acked cmd not found\n");
    } else {
        cmd->flags &= ~SSV_CMD_FLAG_WAIT_ACK;
        if (SSV_CMD_WAIT_COMPLETE(cmd->flags)) {
            _ssv_cmd_complete(cmd_mgr, cmd);
		}
    }
    
    return 0;
}

void _ssv_cmd_mgr_fwreset_chechk(struct ssv_softc *sc , u16_l id)
{
    SSV_LOG_DBG("fw reset recv msg id: %d\n",id);
}

static int _ssv_cmd_mgr_msgind(struct ssv_cmd_mgr *cmd_mgr, struct ipc_e2a_msg *msg, msg_cb_fct cb)
{
    struct ssv_softc *sc = container_of(cmd_mgr, struct ssv_softc, cmd_mgr);
    struct ssv_cmd *cmd;
    bool found = false;

    // SSV_LOG_DBG("[%s][%d] msg->id = %u\n", __FUNCTION__, __LINE__, msg->id);

    mutex_lock(&cmd_mgr->lock);
    list_for_each_entry(cmd, &cmd_mgr->cmds, list) {
        if (cmd->reqid == msg->id &&
            (cmd->flags & SSV_CMD_FLAG_WAIT_CFM)) {
            if(sc->fw_reset_run == false) {
            if (!cb || (cb && !cb(sc, cmd, msg))) {
                found = true;
                }
            } else {
                _ssv_cmd_mgr_fwreset_chechk(sc, msg->id);
                found = true;
            }
            if(found == true) {
                cmd->flags &= ~SSV_CMD_FLAG_WAIT_CFM;

                if (cmd->e2a_msg && msg->param_len)
                    memcpy(cmd->e2a_msg, &msg->param, msg->param_len);

                if (SSV_CMD_WAIT_COMPLETE(cmd->flags)){
                    _ssv_cmd_complete(cmd_mgr, cmd);
				}

                break;
            }
        }
    }
    mutex_unlock(&cmd_mgr->lock);

    if (!found && cb)
	{
        mutex_lock(&sc->cb_lock);
        cb(sc, NULL, msg);
        mutex_unlock(&sc->cb_lock);
        return 0;
	}
    else
    {
        return -1;
    }

}

static void _ssv_cmd_mgr_print(struct ssv_cmd_mgr *cmd_mgr)
{
    struct ssv_cmd *cur;

    mutex_lock(&cmd_mgr->lock);
    SSV_LOG_DBG("q_sz/max: %2d / %2d - next tkn: %d\n",
             cmd_mgr->queue_sz, cmd_mgr->max_queue_sz,
             cmd_mgr->next_tkn);
    list_for_each_entry(cur, &cmd_mgr->cmds, list) {
        ssv_cmd_dump(cur);
    }
    mutex_unlock(&cmd_mgr->lock);
}

static void _ssv_cmd_mgr_drain(struct ssv_cmd_mgr *cmd_mgr)
{
    struct ssv_cmd *cur, *nxt;

    // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    mutex_lock(&cmd_mgr->lock);
    list_for_each_entry_safe(cur, nxt, &cmd_mgr->cmds, list) {
        list_del(&cur->list);
        cmd_mgr->queue_sz--;
        if (!(cur->flags & SSV_CMD_FLAG_NONBLOCK))
            complete(&cur->complete);
    }
    mutex_unlock(&cmd_mgr->lock);
}


/*******************************************************************************
 *         Global Functions
 ******************************************************************************/
void ssv_cmd_dump(const struct ssv_cmd *cmd)
{
    SSV_LOG_DBG("cmd %p: tkn[%d]  flags:%04x  result:%3d  cmd:%4d-%-24s - reqcfm(%4d-%-s)\n",
           cmd, cmd->tkn, cmd->flags, cmd->result, cmd->id, SSV_ID2STR(cmd->id),
           cmd->reqid, cmd->reqid != (lmac_msg_id_t)-1 ? SSV_ID2STR(cmd->reqid) : "none");
}

void ssv_cmd_mgr_init(struct ssv_cmd_mgr *cmd_mgr)
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    INIT_LIST_HEAD(&cmd_mgr->cmds);
    mutex_init(&cmd_mgr->lock);
    cmd_mgr->max_queue_sz = SSV_CMD_MAX_QUEUED;
    cmd_mgr->queue  = &_ssv_cmd_mgr_queue;
    cmd_mgr->print  = &_ssv_cmd_mgr_print;
    cmd_mgr->drain  = &_ssv_cmd_mgr_drain;
    cmd_mgr->llind  = &_ssv_cmd_mgr_llind;
    cmd_mgr->msgind = &_ssv_cmd_mgr_msgind;

    cmd_mgr->state = SSV_CMD_MGR_STATE_INITED;
}

void ssv_cmd_mgr_deinit(struct ssv_cmd_mgr *cmd_mgr)
{
    cmd_mgr->print(cmd_mgr);
    cmd_mgr->drain(cmd_mgr);
    cmd_mgr->print(cmd_mgr);
    memset(cmd_mgr, 0, sizeof(*cmd_mgr));
}
