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

#include <linux/kthread.h>

#include "ssv_cfg.h"
#include <hwif/hwif.h>
#include <hci/ssv_hci.h>
#include <hci/hctrl.h>
#include "dev.h"
#include "utils/debugfs.h"

#include "fmac/fmac_msg_tx.h"
#include "fmac/fmac.h"
#include "fmac/fmac_rx.h"
#include "fmac/fmac_tx.h"
#include "fmac/fmac_strs.h"
#include "fmac/fmac_utils.h"
#include "ipc_msg.h"
#include "rftool/ssv_phy_rf.h"
#include "rftool/ssv_efuse.h"
#include "ssv_debug.h"

extern struct ssv6xxx_cfg ssv_cfg;

extern void ssv_update_mgmt_txdesc(struct ssv_vif *vif, struct ssv_sta *sta, 
                                        struct sk_buff *skb, bool robust, bool no_cck);

static int ssv_cmd_help(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    extern struct ssv_cmd_table cmd_table[];
    struct ssv_cmd_table *sc_tbl;
    int total_cmd = 0;

    ssv_snprintf_res(cmd_data, "Usage:\n");
    //Skip 3 help command.
    for (sc_tbl = &cmd_table[3]; sc_tbl->cmd; sc_tbl++) {
        ssv_snprintf_res(cmd_data, "%-20s\t\t%s\n", (char*)sc_tbl->cmd, sc_tbl->usage);
        total_cmd++;
    }
    ssv_snprintf_res(cmd_data, "Total CMDs: %x\n\nType cli help [CMD] for more detail command.\n\n", total_cmd);
   
    return 0;
}

static int ssv_cmd_reg(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    u32 addr = 0, value = 0, count = 0;
    char *endp;
    int s;

    /* reg [r|w] [address] [value|word-count] */

    if ((argc == 4) && (strcmp(argv[1], "w") == 0)) {
        /* reg w <address> <word_value> */
        addr = simple_strtoul(argv[2], &endp, 16);
        value = simple_strtoul(argv[3], &endp, 16);
    #ifdef CONFIG_HWIF_AND_HCI
        if (ssv_hw->hci_ops->hci_write_word)
            ssv_hw->hci_ops->hci_write_word(ssv_hw->hci_priv, addr, value);
    #else
        if (ssv_hw->hwif_ops->writereg)
            ssv_hw->hwif_ops->writereg(ssv_hw->dev, addr, value);
    #endif

        value = 0;
    #ifdef CONFIG_HWIF_AND_HCI
        if (ssv_hw->hci_ops->hci_read_word)
            ssv_hw->hci_ops->hci_read_word(ssv_hw->hci_priv, addr, &value);
    #else    
        if (ssv_hw->hwif_ops->readreg) 
            ssv_hw->hwif_ops->readreg(ssv_hw->dev, addr, &value);
    #endif

        ssv_snprintf_res(cmd_data, " => write [0x%08x]: 0x%08x\n", addr, value);
        return 0;
    } else if (((argc == 4) || (argc == 3)) && (strcmp(argv[1], "r") == 0)) {
        /* reg r <address> <word_count> */
        count = (argc ==3 )? 1: simple_strtoul(argv[3], &endp, 10);
        addr = simple_strtoul(argv[2], &endp, 16);
        ssv_snprintf_res(cmd_data, "ADDRESS: 0x%08x\n", addr);
        
        for(s = 0; s < count; s++, addr += 4) {
        #ifdef CONFIG_HWIF_AND_HCI
            if (ssv_hw->hci_ops->hci_read_word)
                ssv_hw->hci_ops->hci_read_word(ssv_hw->hci_priv, addr, &value);
        #else    
            if (ssv_hw->hwif_ops->readreg) 
                ssv_hw->hwif_ops->readreg(ssv_hw->dev, addr, &value);
        #endif
            
            ssv_snprintf_res(cmd_data, "%08x ", value);
 
            if (((s+1) & 0x07) == 0){
                ssv_snprintf_res(cmd_data, "\n");
            }
        }
        ssv_snprintf_res(cmd_data, "\n");
        return 0;
	} else {
        ssv_snprintf_res(cmd_data, "reg [r|w] [address] [value|word-count]\n\n");
        return 0;

    }
    return -1;
}

static int ssv_cmd_hci(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    int s = 0, locked = 0;
    bool inactive = false;
    bool paused = false;
    u32 qsize = 0, pkt_cnt = 0, post_rx_pkt = 0; 
    
    if ((argc == 3) && (!strcmp(argv[1], "txq")) && (!strcmp(argv[2], "show"))) {
        for (s = 0; s < SSV_SW_TXQ_NUM ; s++) {  // 3 is WIFI_MNG/BLE/WIFI_CMD
           
            paused = false;
            locked = 0;
            qsize = 0;
            pkt_cnt = 0;

            if (ssv_hw->hci_ops->hci_txq_st)
                ssv_hw->hci_ops->hci_txq_st(ssv_hw->hci_priv, s, &inactive, &paused, &locked, &qsize, &pkt_cnt);
            
            ssv_snprintf_res(cmd_data, ">> txq[%d] inactive %d pause %d lock %d cur_qsize %d\n", s, inactive, paused, locked, qsize);
            ssv_snprintf_res(cmd_data, "    Total %d frame sent\n", pkt_cnt);
        }
    
    } else if ((argc == 3) && (!strcmp(argv[1], "rxq")) && (!strcmp(argv[2], "show"))) {
        qsize = 0;
        pkt_cnt = 0;
        post_rx_pkt = 0;

        if (ssv_hw->hci_ops->hci_rxq_st)
            ssv_hw->hci_ops->hci_rxq_st(ssv_hw->hci_priv, &qsize, &pkt_cnt, &post_rx_pkt);
        
        ssv_snprintf_res(cmd_data, ">> rxq cur_qsize %d, rx packet %d, post rx packet %d\n\n", qsize, pkt_cnt, post_rx_pkt);

    } else {
        ssv_snprintf_res(cmd_data, "hci [txq|rxq] [show]\n\n");
    }

    return 0;
}

static char raw_data_scanu_start_req[] =
{
    0xb8, 0x01, 0xb8, 0x01, 0xff, 0x98, 0xff, 0xff, 0xb0, 0x01, 0x00, 0x00, 0xde, 0xfa, 0xfe, 0xca,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12, 0xb0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*                                                                    , MSG , */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00,
    0x0d, 0x00, 0x54, 0x01, 0x6c, 0x09, 0x00, 0x00, 0x14, 0x00, 0x71, 0x09, 0x00, 0x00, 0x14, 0x00,
    0x76, 0x09, 0x00, 0x00, 0x14, 0x00, 0x7b, 0x09, 0x00, 0x00, 0x14, 0x00, 0x80, 0x09, 0x00, 0x00,
    0x14, 0x00, 0x85, 0x09, 0x00, 0x00, 0x14, 0x00, 0x8a, 0x09, 0x00, 0x00, 0x14, 0x00, 0x8f, 0x09,
    0x00, 0x00, 0x14, 0x00, 0x94, 0x09, 0x00, 0x00, 0x14, 0x00, 0x99, 0x09, 0x00, 0x00, 0x14, 0x00,
    0x9e, 0x09, 0x00, 0x00, 0x14, 0x00, 0xa3, 0x09, 0x00, 0x01, 0x14, 0x00, 0xa8, 0x09, 0x00, 0x01,
    0x14, 0x00, 0xb4, 0x09, 0x00, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static int ssv_cmd_tx(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct sk_buff *skb = NULL;
    int len = 1000;
    int raw_data_len = sizeof(raw_data_scanu_start_req);

    skb = __dev_alloc_skb(len, GFP_KERNEL); 
    if (!skb) {
        ssv_snprintf_res(cmd_data, "Cannot not alloc skb\n\n");
        return 0;
    }
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    skb_put(skb, raw_data_len);
    memcpy((void *)skb->data, (const void *)&raw_data_scanu_start_req[0], raw_data_len);
    SSV_LOG_DBG("[%s][%d] skb->len = %d\n", __FUNCTION__, __LINE__, skb->len);

    // static int ssv6xxx_hci_enqueue(void* hci_priv, struct sk_buff *skb, int txqid,
    //         bool force_trigger, u32 tx_flags);
    #ifdef CONFIG_HWIF_AND_HCI
        /* use wifi command queue to send tx packet 
         * wifi command queue is not paused mode 
         * */
        if (ssv_hw->hci_ops->hci_tx)
            ssv_hw->hci_ops->hci_tx(ssv_hw->hci_priv, skb, SSV_SW_TXQ_ID_WIFI_CMD, false, 0); 
    #else
        if (ssv_hw->hwif_ops->write) {
            ssv_hw->hwif_ops->write(ssv_hw->dev, (void *)skb, skb->len, 0);
        }
        dev_kfree_skb_any(skb);
    #endif
    
    ssv_snprintf_res(cmd_data, "Done to send tx packet\n\n");
    return 0;
}

#if (HWIF_SUPPORT == 2)
static int ssv_cmd_sdio(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    u32 addr, value;
    char *endp;
    int ret=0;

    /* 
    *    (1) sdio reg r [sdio_addr]
    *    (2) sdio reg w [sdio_addr] [value]
    */

    if ((argc == 4) && (!strcmp(argv[1], "reg")) && (!strcmp(argv[2], "r"))) 
    {
        addr = simple_strtoul(argv[3], &endp, 16); 

        if (!ssv_hw->hwif_ops->cmd52_read)
        {
            ssv_snprintf_res(cmd_data, "The interface doesn't provide cmd52 read\n");
            return 0;
        }    

        ret = ssv_hw->hwif_ops->cmd52_read(ssv_hw->dev, addr, &value);

        if (ret >= 0) 
        {
            ssv_snprintf_res(cmd_data, "  ==> %x\n", value);

            return 0;
        }
    }
    else if ((argc ==5) && (!strcmp(argv[1], "reg")) && (!strcmp(argv[2], "w")))
    {
        addr = simple_strtoul(argv[3], &endp, 16);
        value = simple_strtoul(argv[4], &endp, 16);

        if (!ssv_hw->hwif_ops->cmd52_write)
        {
    		ssv_snprintf_res(cmd_data, "The interface doesn't provide cmd52 write\n");

		return 0;
        }
        ret = ssv_hw->hwif_ops->cmd52_write(ssv_hw->dev, addr, value);

        if (ret >= 0) 
        {
            ssv_snprintf_res(cmd_data, "  ==> write done.\n");

            return 0;
        }
    }

    ssv_snprintf_res(cmd_data, "sdio cmd52 fail: %d\n", ret);

    return 0;
 
}
#endif

static int ssv_cmd_ampdu(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *ssv_sc = ssv_hw->sc;
    u32 sta_idx = 0, value = 0;
    char *endp;
    int ret=-1;

    /* 
    *    (1) sdio reg r [sdio_addr]
    *    (2) sdio reg w [sdio_addr] [value]
    */

    if ((argc == 3) && (!strcmp(argv[1], "show"))) 
    {
        sta_idx = simple_strtoul(argv[2], &endp, 16);
        ssv_rxreord_all_dump(ssv_sc, sta_idx);
        ssv_snprintf_res(cmd_data, "ampdu show %d\n", sta_idx);
        return 0;
    }
    else if ((argc ==4) && (!strcmp(argv[1], "level")))
    {
        sta_idx = simple_strtoul(argv[2], &endp, 16);
        value = simple_strtoul(argv[3], &endp, 16);
        ssv_sc->sta_table[sta_idx].reord_info.level = value;
        ssv_snprintf_res(cmd_data, "ampdu level sta_idx: %d value: %d\n", sta_idx, value);
        return 0;
    }

    ssv_snprintf_res(cmd_data, "ampdu cmd fail: %d\n", ret);

    return 0;
 
}

static int ssv_cmd_rc(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *ssv_sc = ssv_hw->sc;
    
    int i;
    u8 sta_id = 0;
    int val;
    char *endp;
    int ret=0;

    bool bConnected = false;
    //should check sta is connected first 
    for (i = 0; i < NX_REMOTE_STA_MAX; i++) 
    {
        struct ssv_sta *sta = &ssv_sc->sta_table[i];
        if(sta->valid == 1)
        {
            sta_id = sta->sta_idx;
            bConnected = true;
        }
        // SSV_LOG_DBG("0th %d, %d %d %d\n", sta->valid, sta->aid, sta->sta_idx, sta->vif_idx);
    }   
    if (bConnected == false)
    {
        // rc setting will be activated after connected 
        sta_id = 0;
    }
    
    if (argc < 2) 
    {
        ssv_snprintf_res(cmd_data, "    Fixe Rate: rc set hex [value]\n");
        ssv_snprintf_res(cmd_data, "  LDPC ON/OFF: rc ldpc [value]\n");
        ssv_snprintf_res(cmd_data, "       LTF GI: rc ltfgi [value]\n");
        ssv_snprintf_res(cmd_data, "Reset Default: rc sta_reset_default\n");
        return 0;
    }
    
    if (!strcmp(argv[1], "set"))
    {
        if( argc == 4 )
        {
            val = simple_strtoul(argv[3], &endp, 0);
            if(!strcmp(argv[2], "hex"))
            {
                ssv_snprintf_res(cmd_data, "call cmd rc set hex %s to sta %d\n", argv[3], sta_id);
                ssv_sc->rate_config = val;

                ret = ssv_send_me_rc_set_rate(ssv_sc, sta_id, ssv_sc->rate_config);
                    
                return ret;
            }
            else
            {
                ssv_snprintf_res(cmd_data, "rc set hex [value]\n");
                return 0;
            }
        }
        else
        {
            ssv_snprintf_res(cmd_data, "rc set hex [value]\n");
        }
    }
    else if (!strcmp(argv[1], "ldpc")) 
    {
        if(argc == 3)
        {
            val = simple_strtoul(argv[2], &endp, 0);
            ssv_snprintf_res(cmd_data, "call cmd rc ldpc %d to sta %d\n", val, sta_id);

            ret = ssv_send_me_rc_set_ldpc(ssv_sc, sta_id, val);
                
            return ret;
        }
        else
        {
            ssv_snprintf_res(cmd_data, "rc ldpc [value]\n");
        }
    } 
    else if (!strcmp(argv[1], "ltfgi")) 
    {
        if(argc == 3)
        {
            val = simple_strtoul(argv[2], &endp, 0);
            ssv_snprintf_res(cmd_data, "call cmd rc ltfgi %d to sta %d\n", val, sta_id);
            ssv_snprintf_res(cmd_data, "ltfgi mode :\n");
            ssv_snprintf_res(cmd_data, "\t 1 - LTF 6.4us + GI 0.8us\n");
            ssv_snprintf_res(cmd_data, "\t 2 - LTF 6.4us + GI 1.6us\n");
            ssv_snprintf_res(cmd_data, "\t 3 - LTF 12.8us + GI 3.2us\n");

            ret = ssv_send_me_rc_set_ltf_gi(ssv_sc, sta_id, val);

            return ret;
        }
        else
        {
            ssv_snprintf_res(cmd_data, "rc ltfgi [value]\n");
            ssv_snprintf_res(cmd_data, "value :\n");
            ssv_snprintf_res(cmd_data, "\t 1 - LTF 6.4us + GI 0.8us\n");
            ssv_snprintf_res(cmd_data, "\t 2 - LTF 6.4us + GI 1.6us\n");
            ssv_snprintf_res(cmd_data, "\t 3 - LTF 12.8us + GI 3.2us\n");
        }
    }
    else if (!strcmp(argv[1], "sta_reset_default")) 
    {
        ret = ssv_send_me_rc_sta_reset_default(ssv_sc, sta_id);

        return ret;
    }

    return 0;
 
}

int ssv_private_msg_to_hci(struct ssv_softc *sc, u8 *msg_buffer, u32 msg_len);
static int ssv_txtput_thread_m2(void *data)
{
#define Q_DELAY_MS 20
	int qlen = 0, max_qlen, q_delay_urange[2];
	struct ssv_softc *sc = data;
    u32 msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sc->ssv_txtput.size_per_frame);
    ST_IPC_PRIV_MSG *txtput_msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if(!txtput_msg)
    {
        SSV_LOG_DBG("txtput, kzalloc() for msg failed!!\n");
        goto end;
    }
    txtput_msg->msgid = E_IPC_PRIV_MSG_TYPE_TXTPUT_REQ;
    txtput_msg->msglen = sc->ssv_txtput.size_per_frame;

	/* Suppose the max sdio tput is 200Mbps
	 * max_qlen = 200Mbps / 8 / size_per_frame_in_byte * delay_in_sec
	 */
	max_qlen = (200 * 1000 / 8 * Q_DELAY_MS) / sc->ssv_txtput.size_per_frame;
	q_delay_urange[0] = Q_DELAY_MS * 1000;
	q_delay_urange[1] = q_delay_urange[0] + 1000;
	 SSV_LOG_DBG("max_qlen: %d\n", max_qlen);
	while (!kthread_should_stop() && sc->ssv_txtput.loop_times > 0) {
		sc->ssv_txtput.loop_times--;
        qlen = ssv_private_msg_to_hci(sc, (u8*)txtput_msg, msg_total_len);
		if (qlen >= max_qlen) {
			// SSV_LOG_DBG("%s: qlen=%d\n", __func__, qlen);
			usleep_range(q_delay_urange[0], q_delay_urange[1]);
		}
	}

    if(txtput_msg)
    {
        kfree(txtput_msg);
    }

end:
	sc->ssv_txtput.txtput_tsk = NULL;
	return 0;
}

int ssv_txtput_generate_m2(struct ssv_softc *sc, u32 size_per_frame, u32 loop_times)
{
	sc->ssv_txtput.size_per_frame = size_per_frame;
	sc->ssv_txtput.loop_times = loop_times;
	sc->ssv_txtput.txtput_tsk = kthread_run(ssv_txtput_thread_m2, sc, "ssv_txtput_thread_m2");
	return 0;
}

static int txtput_tsk_cleanup(struct ssv_softc *sc)
{
	int ret = 0;
	if (sc->ssv_txtput.txtput_tsk) {
		 SSV_LOG_DBG("Stopping txtput task...\n");
		ret = kthread_stop(sc->ssv_txtput.txtput_tsk);
		while (sc->ssv_txtput.txtput_tsk != NULL) {
			msleep(1);
		}
        	 SSV_LOG_DBG("txtput task is stopped.\n");
	}
	return ret;
}

static int ssv_cmd_txtput(void *cmd_priv, int argc, char *argv[])
{
	struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
	char *endp;
	u32 size_per_frame, loop_times;

	if ( (argc == 2) && (!strcmp(argv[1], "stop"))) {
		txtput_tsk_cleanup(sc);
		return 0;
	}

	if (argc != 3) {
		ssv_snprintf_res(cmd_data, "* txtput stop\n");
		ssv_snprintf_res(cmd_data, "* txtput [size] [frames]\n");
		ssv_snprintf_res(cmd_data, " EX: txtput 14000 9999 \n");

		return 0;
	}

	size_per_frame = simple_strtoul(argv[1], &endp, 10);
	loop_times	   = simple_strtoul(argv[2], &endp, 10);
	ssv_snprintf_res(cmd_data, "size & frames: %d & %d\n", size_per_frame, loop_times);

	//already in progress
	if (sc->ssv_txtput.txtput_tsk) {
		ssv_snprintf_res(cmd_data, "txtput already in progress\n");
		return 0;
	}
	ssv_txtput_generate_m2(sc, size_per_frame, loop_times);

	return 0;
}

// #define MAX_FRM_SIZE 2304
#define MAX_FRM_SIZE (ALIGN(FHOST_RX_BUF_SIZE, 4)-sizeof(ST_IPC_PRIV_MSG))
static int ssv_cmd_rxtput(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    char *endp = NULL;
    ST_IPC_PRIV_MSG *rxtput_msg = NULL;
    ST_IPC_TPUT_PARAM rxtput = {0};
    u32 msg_total_len = 0;

    if (argc != 3) {
        ssv_snprintf_res(cmd_data, "rxtput [size] [frames]\n");
		return 0;
    }

	rxtput.size = (u32)simple_strtoul(argv[1], &endp, 10);
	rxtput.looptimes = (u32)simple_strtoul(argv[2], &endp, 10);

	if (rxtput.size > MAX_FRM_SIZE) {
	    ssv_snprintf_res(cmd_data, "Frame size(%u) > max(%u)\n", rxtput.size, MAX_FRM_SIZE);
	    return 0 ;
	}
    ssv_snprintf_res(cmd_data, "size & frames: %d & %d\n",
        rxtput.size, rxtput.looptimes);

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(ST_IPC_TPUT_PARAM));
    rxtput_msg = kzalloc(msg_total_len+1, GFP_KERNEL);

    if(!rxtput_msg) {
        SSV_LOG_DBG("rxtput, kzalloc() for msg failed!!\n");
        return 0;
    }
    rxtput_msg->msgid = E_IPC_PRIV_MSG_TYPE_RXTPUT_REQ;
    rxtput_msg->msglen = sizeof(ST_IPC_TPUT_PARAM);
    memcpy((void *)&rxtput_msg->data[0], (const void *)&rxtput, sizeof(ST_IPC_TPUT_PARAM));

    if (0 > ssv_private_msg_to_hci(sc, (u8*)rxtput_msg, msg_total_len)) {
        SSV_LOG_DBG("Fail to send private command\n");
    }

    if(rxtput_msg) {
        kfree(rxtput_msg);
        rxtput_msg = NULL;
    }

	return 0;
}

static int ssv_cmd_mac(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    int i = 0;
    u8 sta_idx = 0xFF;
    /**
     *  mac [rxq|txq|reset] [show|bysta|sta] [show|sta_idx]
     * (1) mac rxq show
     * (2) mac txq show
     * (3) mac rxq bysta show
     * (4) mac txq bysta show
     * (5) mac reset
     * (6) mac reset sta [0-10]
     */

    if ((argc == 3) && (!strcmp(argv[1], "rxq")) && (!strcmp(argv[2], "show"))) {
        ssv_snprintf_res(cmd_data, "===== MAC RXQ SHOW ALL =====\n");
        ssv_snprintf_res(cmd_data, "  rx frame %d\n", sc->rx.rx_count);
        ssv_snprintf_res(cmd_data, "  rx data frame %d\n", sc->rx.rx_data_count);
        ssv_snprintf_res(cmd_data, "  rx mgmt frame %d\n", sc->rx.rx_mgmt_count);
        ssv_snprintf_res(cmd_data, "  rx bcn frame %d\n", sc->rx.rx_bcn_count);
        ssv_snprintf_res(cmd_data, "  rx probereq frame %d\n", sc->rx.rx_probereq_count);
        ssv_snprintf_res(cmd_data, "  rx proberesp frame %d\n", sc->rx.rx_proberesp_count);
        ssv_snprintf_res(cmd_data, "  rx assoc_req frame %d\n", sc->rx.rx_assoc_req_count);
        ssv_snprintf_res(cmd_data, "  rx assoc_resp frame %d\n", sc->rx.rx_assoc_resp_count);
        ssv_snprintf_res(cmd_data, "  rx auth frame %d\n", sc->rx.rx_auth_count);
        ssv_snprintf_res(cmd_data, "  rx disassoc frame %d\n", sc->rx.rx_disassoc_count);
        ssv_snprintf_res(cmd_data, "  rx deauth frame %d\n", sc->rx.rx_deauth_count);
        ssv_snprintf_res(cmd_data, "  rx reordering frame %d\n", sc->rx.rx_reord_count);
        ssv_snprintf_res(cmd_data, "  rx arp req frame %d\n", sc->rx.rx_arp_req_count);
        ssv_snprintf_res(cmd_data, "  rx arp reply frame %d\n", sc->rx.rx_arp_reply_count);
        ssv_snprintf_res(cmd_data, "  rx icmp echo frame %d\n", sc->rx.rx_icmp_echo);
        ssv_snprintf_res(cmd_data, "  rx icmp echo reply frame %d\n", sc->rx.rx_icmp_echoreply);
        ssv_snprintf_res(cmd_data, "  rx dhcp discv frame %d\n", sc->rx.rx_dhcp_discv);
        ssv_snprintf_res(cmd_data, "  rx dhcp offer frame %d\n", sc->rx.rx_dhcp_offer);
        ssv_snprintf_res(cmd_data, "  rx dhcp req frame %d\n", sc->rx.rx_dhcp_req);
        ssv_snprintf_res(cmd_data, "  rx dhcp ack reply frame %d\n", sc->rx.rx_dhcp_ack);
        ssv_snprintf_res(cmd_data, "  rx eapol reply frame %d\n", sc->rx.rx_eapol);
            return 0;

    } else if ((argc == 3) && (!strcmp(argv[1], "txq")) && (!strcmp(argv[2], "show"))) {
        ssv_snprintf_res(cmd_data, "===== MAC TXQ SHOW ALL =====\n");
        ssv_snprintf_res(cmd_data, "  tx frame %d\n", sc->tx.tx_count);
        ssv_snprintf_res(cmd_data, "  tx data frame %d\n", sc->tx.tx_data_count);
        ssv_snprintf_res(cmd_data, "  tx mgmt frame %d\n", sc->tx.tx_mgmt_count);
        ssv_snprintf_res(cmd_data, "  tx auth frame %d\n", sc->tx.tx_auth_count);
        ssv_snprintf_res(cmd_data, "  tx deauth frame %d\n", sc->tx.tx_deauth_count);
        ssv_snprintf_res(cmd_data, "  tx assocreq frame %d\n", sc->tx.tx_assoc_req_count);
        ssv_snprintf_res(cmd_data, "  tx assocresp frame %d\n", sc->tx.tx_assoc_resp_count);
        ssv_snprintf_res(cmd_data, "  tx probereq frame %d\n", sc->tx.tx_probe_req_count);
        ssv_snprintf_res(cmd_data, "  tx proberesp frame %d\n", sc->tx.tx_probe_resp_count);
        ssv_snprintf_res(cmd_data, "  tx arp req frame %d\n", sc->tx.tx_arp_req_count);
        ssv_snprintf_res(cmd_data, "  tx arp reply frame %d\n", sc->tx.tx_arp_reply_count);
        ssv_snprintf_res(cmd_data, "  tx icmp echo frame %d\n", sc->tx.tx_icmp_echo);
        ssv_snprintf_res(cmd_data, "  tx icmp echo reply frame %d\n", sc->tx.tx_icmp_echoreply);
        ssv_snprintf_res(cmd_data, "  tx dhcp discv frame %d\n", sc->tx.tx_dhcp_discv);
        ssv_snprintf_res(cmd_data, "  tx dhcp offer frame %d\n", sc->tx.tx_dhcp_offer);
        ssv_snprintf_res(cmd_data, "  tx dhcp req frame %d\n", sc->tx.tx_dhcp_req);
        ssv_snprintf_res(cmd_data, "  tx dhcp ack reply frame %d\n", sc->tx.tx_dhcp_ack);
        ssv_snprintf_res(cmd_data, "  tx eapol reply frame %d\n", sc->tx.tx_eapol);
      ssv_snprintf_res(cmd_data, "  tx flowctl stop count %d\n", sc->tx.flowctl_stop_count);
      ssv_snprintf_res(cmd_data, "  tx flowctl wake count %d\n", sc->tx.flowctl_wake_count);
        return 0;

    } else if((argc == 4) && (!strcmp(argv[1], "rxq")) && (!strcmp(argv[2], "bysta")) && (!strcmp(argv[3], "show"))){
    
    ssv_snprintf_res(cmd_data, "===== MAC RXQ SHOW BY STA =====\n");
    for(i = 0; i < NX_REMOTE_STA_MAX; i++)
    {
        struct ssv_sta *sta = &sc->sta_table[i];
        if(sta->valid) 
        {
            sta_idx = sta->sta_idx;
            ssv_snprintf_res(cmd_data, "sta_idx %d:\n", i);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx frame", sc->rx_bysta[sta_idx].rx_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx data frame", sc->rx_bysta[sta_idx].rx_data_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx mgmt frame", sc->rx_bysta[sta_idx].rx_mgmt_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx bcn frame", sc->rx_bysta[sta_idx].rx_bcn_count);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx probereq frame", sc->rx_bysta[sta_idx].rx_probereq_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx proberesp frame", sc->rx_bysta[sta_idx].rx_proberesp_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx assoc_req frame", sc->rx_bysta[sta_idx].rx_assoc_req_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx assoc_resp frame", sc->rx_bysta[sta_idx].rx_assoc_resp_count);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx auth frame", sc->rx_bysta[sta_idx].rx_auth_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx disassoc frame", sc->rx_bysta[sta_idx].rx_disassoc_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx deauth frame", sc->rx_bysta[sta_idx].rx_deauth_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx reordering frame", sc->rx_bysta[sta_idx].rx_reord_count);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx arp req frame", sc->rx_bysta[sta_idx].rx_arp_req_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx arp reply frame", sc->rx_bysta[sta_idx].rx_arp_reply_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx icmp echo frame", sc->rx_bysta[sta_idx].rx_icmp_echo);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx icmp echoreply", sc->rx_bysta[sta_idx].rx_icmp_echoreply);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx dhcp discv frame", sc->rx_bysta[sta_idx].rx_dhcp_discv);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx dhcp offer frame", sc->rx_bysta[sta_idx].rx_dhcp_offer);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx dhcp req frame", sc->rx_bysta[sta_idx].rx_dhcp_req);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx dhcp ack frame", sc->rx_bysta[sta_idx].rx_dhcp_ack);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "rx eapol frame", sc->rx_bysta[sta_idx].rx_eapol);
            ssv_snprintf_res(cmd_data, "\n");
            
        }
    } 
        return 0;
    }else if((argc == 4) && (!strcmp(argv[1], "txq")) && (!strcmp(argv[2], "bysta")) && (!strcmp(argv[3], "show"))){
    ssv_snprintf_res(cmd_data, "===== MAC TXQ SHOW BY STA =====\n");
    for(i = 0; i < NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX; i++)
    {
        struct ssv_sta *sta = &sc->sta_table[i];
        if(sta->valid) 
        {
            sta_idx = sta->sta_idx;
            ssv_snprintf_res(cmd_data, "sta_idx %d:\n", i);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx frame", sc->tx_bysta[sta_idx].tx_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx data frame", sc->tx_bysta[sta_idx].tx_data_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx mgmt frame", sc->tx_bysta[sta_idx].tx_mgmt_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx auth frame", sc->tx_bysta[sta_idx].tx_auth_count);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx deauth frame", sc->tx_bysta[sta_idx].tx_deauth_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx assoc req frame", sc->tx_bysta[sta_idx].tx_assoc_req_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx assoc resp frame", sc->tx_bysta[sta_idx].tx_assoc_resp_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx probe req frame", sc->tx_bysta[sta_idx].tx_probe_req_count);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx probe resp frame", sc->tx_bysta[sta_idx].tx_probe_resp_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx arp req frame", sc->tx_bysta[sta_idx].tx_arp_req_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx arp reply frame", sc->tx_bysta[sta_idx].tx_arp_reply_count);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx icmp echo frame", sc->tx_bysta[sta_idx].tx_icmp_echo);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx icmp echoreply", sc->tx_bysta[sta_idx].tx_icmp_echoreply);            
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx dhcp discv frame", sc->tx_bysta[sta_idx].tx_dhcp_discv);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx dhcp offer frame", sc->tx_bysta[sta_idx].tx_dhcp_offer);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx dhcp req frame", sc->tx_bysta[sta_idx].tx_dhcp_req);
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx dhcp ack frame", sc->tx_bysta[sta_idx].tx_dhcp_ack);
            ssv_snprintf_res(cmd_data, "  %-20s: %8d,", "tx eapol frame", sc->tx_bysta[sta_idx].tx_eapol);
            ssv_snprintf_res(cmd_data, "\n");            
        }
    } 
        return 0;    
    }else if(!strcmp(argv[1], "reset")){
        
        if(argc == 2){
            for(i = 0; i < NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX; i++)
            {
                struct ssv_sta *sta = &sc->sta_table[i];
                memset(&sc->rx, 0, sizeof(struct ssv_rx));
                memset(&sc->tx, 0, sizeof(struct ssv_tx));
                memset(&sc->rx_bysta[sta->sta_idx], 0, sizeof(struct ssv_rx));
                memset(&sc->tx_bysta[sta->sta_idx], 0, sizeof(struct ssv_tx));                
            }
            msleep(1);
            ssv_snprintf_res(cmd_data, "mac TRX queue reset!\n"); 
            return 0;
        }else if((argc == 4 && (!strcmp(argv[2], "sta")))) {
            
            int sta_idx;
            sscanf(argv[3], "%d", &sta_idx); 
            if (sta_idx > NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX) {
                ssv_snprintf_res(cmd_data, "sta_idx range is [0-11].\n");
                return 0;
            }else{                          
                memset(&sc->rx_bysta[sta_idx], 0 , sizeof(struct ssv_rx));
                memset(&sc->tx_bysta[sta_idx], 0 , sizeof(struct ssv_tx));
                ssv_snprintf_res(cmd_data, "mac sta %d reset!\n", sta_idx);
                return 0;
            }
                        
        }else ssv_snprintf_res(cmd_data,"Incorrect mac reset command format!\n");

    }else {
        ssv_snprintf_res(cmd_data, "    ./cli mac [rxq|txq] show\n");
        ssv_snprintf_res(cmd_data, "    ./cli mac [rxq|txq] bysta show\n");
        ssv_snprintf_res(cmd_data, "    ./cli mac reset\n");
        ssv_snprintf_res(cmd_data, "    ./cli mac reset sta 0~11\n");
    }
    return 0;
}

#if 1 //MIB functions
static u32 _ssv_read_reg(struct ssv_hw *ssv_hw, u32 addr)
{
    u32 value = 0;
#ifdef CONFIG_HWIF_AND_HCI
    if (ssv_hw->hci_ops->hci_read_word)
        ssv_hw->hci_ops->hci_read_word(ssv_hw->hci_priv, addr, &value);
#else
    if (ssv_hw->hwif_ops->readreg)
        ssv_hw->hwif_ops->readreg(ssv_hw->dev, addr, &value);
#endif
    return value;
}

static void _ssv_write_reg(struct ssv_hw *ssv_hw, u32 addr, u32 value)
{
#ifdef CONFIG_HWIF_AND_HCI
    if (ssv_hw->hci_ops->hci_write_word)
        ssv_hw->hci_ops->hci_write_word(ssv_hw->hci_priv, addr, value);
#else
    if (ssv_hw->hwif_ops->writereg)
        ssv_hw->hwif_ops->writereg(ssv_hw->dev, addr, value);
#endif
}

#define REG_MAC_CORE_BASE_ADDR                  (0x08000000)
/// Offset of the MAC_CNTRL_1 register from the base address
#define NXMAC_MAC_CNTRL_1_OFFSET                (0x0000004C)
/// Address of the MAC_CNTRL_1 register
#define NXMAC_MAC_CNTRL_1_ADDR                  (REG_MAC_CORE_BASE_ADDR + NXMAC_MAC_CNTRL_1_OFFSET)
/// MIB_TABLE_RESET field bit
#define NXMAC_MIB_TABLE_RESET_BIT               ((uint32_t)0x00001000)
/// MIB_TABLE_RESET field position
#define NXMAC_MIB_TABLE_RESET_POS               (12)

/// Offset of the MIB_TABLE register from the base address
#define NXMAC_MIB_TABLE_OFFSET                  (0x00000800)
/// Address of the MIB_TABLE register
#define NXMAC_MIB_TBL_ADDR                      (REG_MAC_CORE_BASE_ADDR + NXMAC_MIB_TABLE_OFFSET)
#define NXMAC_MIB_TABLE_LEN                     (255)

#define NXMAC_MIB_TBL_TX_UNICAST_MPDU_CNT_IDX   (12)
#define NXMAC_MIB_TBL_TX_UNICAST_MPDU_CNT_ADDR  (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_UNICAST_MPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_TX_GROUP_MPDU_CNT_IDX     (20)
#define NXMAC_MIB_TBL_TX_GROUP_MPDU_CNT_ADDR    (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_GROUP_MPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_TX_FAILED_CNT_IDX         (28)
#define NXMAC_MIB_TBL_TX_FAILED_CNT_ADDR        (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_FAILED_CNT_IDX<<2))
#define NXMAC_MIB_TBL_TX_RETRY_CNT_IDX          (36)
#define NXMAC_MIB_TBL_TX_RETRY_CNT_ADDR         (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_RETRY_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RTS_SUCCESS_CNT_IDX       (44)
#define NXMAC_MIB_TBL_RTS_SUCCESS_CNT_ADDR      (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RTS_SUCCESS_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RTS_FAILURE_CNT_IDX       (52)
#define NXMAC_MIB_TBL_RTS_FAILURE_CNT_ADDR      (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RTS_FAILURE_CNT_IDX<<2))
#define NXMAC_MIB_TBL_ACK_FAILURE_CNT_IDX       (60)
#define NXMAC_MIB_TBL_ACK_FAILURE_CNT_ADDR      (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_ACK_FAILURE_CNT_IDX<<2))
#define NXMAC_MIB_TBL_TX_AMPDU_CNT_IDX          (204)
#define NXMAC_MIB_TBL_TX_AMPDU_CNT_ADDR         (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_TX_MPDU_IN_AMPDU_CNT_IDX  (205)
#define NXMAC_MIB_TBL_TX_MPDU_IN_AMPDU_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_MPDU_IN_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_TX_BYTE_IN_AMPDU_CNT_IDX  (206)
#define NXMAC_MIB_TBL_TX_BYTE_IN_AMPDU_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_TX_BYTE_IN_AMPDU_CNT_IDX<<2))

#define NXMAC_MIB_TBL_RX_FCS_ERR_CNT_IDX        (1)
#define NXMAC_MIB_TBL_RX_FCS_ERR_CNT_ADDR       (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_FCS_ERR_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_PHY_ERR_CNT_IDX        (2)
#define NXMAC_MIB_TBL_RX_PHY_ERR_CNT_ADDR       (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_PHY_ERR_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_FIFO_OVERFLOW_IDX      (3)
#define NXMAC_MIB_TBL_RX_FIFO_OVERFLOW_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_FIFO_OVERFLOW_IDX<<2))

#define NXMAC_MIB_TBL_RX_UNICAST_MPDU_CNT_IDX   (68)
#define NXMAC_MIB_TBL_RX_UNICAST_MPDU_CNT_ADDR  (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_UNICAST_MPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_GROUP_MPDU_CNT_IDX     (76)
#define NXMAC_MIB_TBL_RX_GROUP_MPDU_CNT_ADDR    (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_GROUP_MPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_OTHER_MPDU_CNT_IDX     (84)
#define NXMAC_MIB_TBL_RX_OTHER_MPDU_CNT_ADDR    (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_OTHER_MPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_RETRY_MPDU_CNT_IDX     (92)
#define NXMAC_MIB_TBL_RX_RETRY_MPDU_CNT_ADDR    (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_RETRY_MPDU_CNT_IDX<<2))

#define NXMAC_MIB_TBL_RX_UNICAST_AMPDU_CNT_IDX  (207)
#define NXMAC_MIB_TBL_RX_UNICAST_AMPDU_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_UNICAST_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_GROUP_AMPDU_CNT_IDX    (208)
#define NXMAC_MIB_TBL_RX_GROUP_AMPDU_CNT_ADDR   (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_GROUP_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_OTHER_AMPDU_CNT_IDX    (209)
#define NXMAC_MIB_TBL_RX_OTHER_AMPDU_CNT_ADDR   (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_OTHER_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_MPDU_IN_AMPDU_CNT_IDX  (210)
#define NXMAC_MIB_TBL_RX_MPDU_IN_AMPDU_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_MPDU_IN_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_BYTE_IN_AMPDU_CNT_IDX  (211)
#define NXMAC_MIB_TBL_RX_BYTE_IN_AMPDU_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_BYTE_IN_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_RX_CRC_ERR_AMPDU_CNT_IDX  (212)
#define NXMAC_MIB_TBL_RX_CRC_ERR_AMPDU_CNT_ADDR (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_RX_CRC_ERR_AMPDU_CNT_IDX<<2))
#define NXMAC_MIB_TBL_IMP_BAR_FAILURE_CNT_IDX   (213)
#define NXMAC_MIB_TBL_IMP_BAR_FAILURE_CNT_ADDR  (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_IMP_BAR_FAILURE_CNT_IDX<<2))
#define NXMAC_MIB_TBL_EXP_BAR_FAILURE_CNT_IDX   (214)
#define NXMAC_MIB_TBL_EXP_BAR_FAILURE_CNT_ADDR  (NXMAC_MIB_TBL_ADDR + (NXMAC_MIB_TBL_EXP_BAR_FAILURE_CNT_IDX<<2))

static void _ssv_reset_mib(struct ssv_hw *ssv_hw)
{
    u32 reg_val = 0;

    reg_val = _ssv_read_reg(ssv_hw, NXMAC_MAC_CNTRL_1_ADDR);
    reg_val |= NXMAC_MIB_TABLE_RESET_BIT;
    _ssv_write_reg(ssv_hw, NXMAC_MAC_CNTRL_1_ADDR, reg_val);
    msleep(1);
}

static void _ssv_list_mib(struct ssv_hw *ssv_hw)
{
    u32 addr, value;
    int i;
    struct ssv_cmd_data *cmd_data = &ssv_hw->cmd_data;

    addr = NXMAC_MIB_TBL_ADDR;

    ssv_snprintf_res(cmd_data, "%08x: ", addr);
    for (i = 0; i < NXMAC_MIB_TABLE_LEN; i++, addr+=4) {
        value = _ssv_read_reg(ssv_hw, addr);
        ssv_snprintf_res(cmd_data, "%08x ", value);

        if (((i+1) & 0x07) == 0) {
            ssv_snprintf_res(cmd_data, "\n");
            ssv_snprintf_res(cmd_data, "%08x: ", addr+4);
        }
    }
    ssv_snprintf_res(cmd_data, "\n");
}

static void _ssv_dump_mib_rx(struct ssv_hw *ssv_hw)
{
    struct ssv_cmd_data *cmd_data = &ssv_hw->cmd_data;
    int i = 0;

    ssv_snprintf_res(cmd_data, "===== MAC RX status =====\n");

    ssv_snprintf_res(cmd_data, "- Basic set\n");
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "FCS error", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_FCS_ERR_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "PHY error", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_PHY_ERR_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "FIFO overflow", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_FIFO_OVERFLOW_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "\n");

    ssv_snprintf_res(cmd_data, "\n");
    ssv_snprintf_res(cmd_data, "- EDCA set\n");
    for(i = 0; i < 8; i++)
    {
        ssv_snprintf_res(cmd_data, " tid %d:\n", i);
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Unicast MPDU", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_UNICAST_MPDU_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Group MPDU", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_GROUP_MPDU_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "\n");
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Other MPDU", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_OTHER_MPDU_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Retry MPDU", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_RETRY_MPDU_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "\n");
    }

    ssv_snprintf_res(cmd_data, "\n");
    ssv_snprintf_res(cmd_data, "- A-MPDU set\n");
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "Unicast", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_UNICAST_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "Group", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_GROUP_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "Other", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_OTHER_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "\n");
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "MPDUs", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_MPDU_IN_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "Bytes", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_BYTE_IN_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "\n");
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "CRC error", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RX_CRC_ERR_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "I. BAR FAIL", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_IMP_BAR_FAILURE_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "E. BAR FAIL", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_EXP_BAR_FAILURE_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "\n");

    ssv_snprintf_res(cmd_data, "\n");
}

static void _ssv_dump_mib_tx(struct ssv_hw *ssv_hw)
{
    struct ssv_cmd_data *cmd_data = &ssv_hw->cmd_data;
    int i = 0;

    ssv_snprintf_res(cmd_data, "===== MAC TX status =====\n");

    ssv_snprintf_res(cmd_data, "- EDCA set\n");
    for(i = 0; i < 8; i++)
    {
        ssv_snprintf_res(cmd_data, " tid %d:\n", i);
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Unicast MPDU", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_UNICAST_MPDU_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Group MPDU", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_GROUP_MPDU_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "\n");
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Failed", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_FAILED_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "Retry", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_RETRY_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "ACK failure", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_ACK_FAILURE_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "\n");
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "RTS success", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RTS_SUCCESS_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
            , "RTS failure", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_RTS_FAILURE_CNT_ADDR+(i<<2)));
        ssv_snprintf_res(cmd_data, "\n");
    }

    ssv_snprintf_res(cmd_data, "\n");
    ssv_snprintf_res(cmd_data, "- A-MPDU set\n");
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "A-MPDUs", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "MPDUs",  _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_MPDU_IN_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "  %-12s: %8d,"
        , "Bytes", _ssv_read_reg(ssv_hw, NXMAC_MIB_TBL_TX_BYTE_IN_AMPDU_CNT_ADDR));
    ssv_snprintf_res(cmd_data, "\n");

    ssv_snprintf_res(cmd_data, "\n");
}

static void _ssv_dump_mib_hwif(struct ssv_hw *ssv_hw)
{
    struct ssv_cmd_data *cmd_data = &ssv_hw->cmd_data;
    u32 tx_pkt = 0;
    u32 rx_pkt = 0;

    ssv_snprintf_res(cmd_data, "===== HWIF status =====\n");

    ssv_snprintf_res(cmd_data, "- TX\n");
    ssv_hw->hwif_ops->tx_st(ssv_hw->dev, &tx_pkt);
    ssv_snprintf_res(cmd_data, "  Total %d frame sent\n", tx_pkt);

    ssv_snprintf_res(cmd_data, "- RX\n");
    ssv_hw->hwif_ops->rx_st(ssv_hw->dev, &rx_pkt);
    ssv_snprintf_res(cmd_data, "  Total %d frame received\n", rx_pkt);

    ssv_snprintf_res(cmd_data, "\n");
}
#endif //end of MIB functions

static int ssv_cmd_mib(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    // struct ssv_softc *sc = ssv_hw->sc;

    /**
     *  mib [reset|rx|tx]
     * (1) mib reset
     * (2) mib rx
     * (3) mib tx
     * (4) mib hwif
     */
    if ((argc == 2) && (!strcmp(argv[1], "reset"))) {
        _ssv_reset_mib(ssv_hw);
        ssv_snprintf_res(cmd_data, " => MIB reseted\n");
    } else if ((argc == 2) && (!strcmp(argv[1], "list"))) {
        _ssv_list_mib(ssv_hw);
    } else if ((argc == 2) && (strcmp(argv[1], "rx") == 0)) {
        _ssv_dump_mib_rx(ssv_hw);
    } else if ((argc == 2) && (strcmp(argv[1], "tx") == 0)) {
        _ssv_dump_mib_tx(ssv_hw);
    } else if ((argc == 2) && (strcmp(argv[1], "hwif") == 0)) {
        _ssv_dump_mib_hwif(ssv_hw);
    } else {
        ssv_snprintf_res(cmd_data, "mib [reset|list|rx|tx|hwif]\n\n");
    }
    return 0;
}

int rx_pkt_type_and_record(struct sk_buff *rx_skb)
{
    int i = 0;
    u8 *base = (u8 *)&rx_skb->data[0];
    u8 pkt_type = (u8)((*base) & 0xFF);
    //u16 real_pkt_len = (u16)((*base) >> 8);
    char *raw_data = (char *)(base + 4 + sizeof(struct rx_info) + RX_BUF_HEADROOM_SIZE);
    char *data = NULL;
    
    if(pkt_type == E_IPC_TYPE_MSG) {
        struct ipc_e2a_msg *msg_rx = (struct ipc_e2a_msg *)raw_data;
        SSV_LOG_DBG("====================\n");
        SSV_LOG_DBG("id: %s dest: %s src: %s\n", SSV_ID2STR(msg_rx->id), SSV_TASKID2STR(msg_rx->dummy_dest_id), SSV_TASKID2STR(msg_rx->dummy_src_id));
        data =(char *) msg_rx->param;
        for(i = 1; i <= msg_rx->param_len; i++) {
            SSV_LOG_DBG_ONE_LINE("%02x ", data[i-1]&0xff);
            if(i % 16 == 0)
                SSV_LOG_DBG("\n");
        }
        SSV_LOG_DBG("\n");
        SSV_LOG_DBG("====================\n");
    } else if(pkt_type == E_IPC_TYPE_DATA){
        SSV_LOG_DBG("********************\n");
        data = (char *)raw_data;
        for(i = 1; i <= 32; i++) {
            SSV_LOG_DBG_ONE_LINE("%02x ", data[i-1]&0xff);
            if(i % 16 == 0)
                SSV_LOG_DBG("\n");
        }
        SSV_LOG_DBG("\n");
        SSV_LOG_DBG("********************\n");
    } else {
        SSV_LOG_DBG("??? rx type: %d ???\n",pkt_type);
    }
    return 0;
}

int tx_pkt_type_and_record(struct sk_buff *tx_skb)
{
    int i = 0;
    u8 *base = (char *)&tx_skb->data[0];
    struct sdio_hdr *sdio_hdr = (struct sdio_hdr *)(base + sizeof(struct tx_bmu_desc));
    u16 headroom =  SSV_TX_HDR_SIZE;
    u16 frame_len = sdio_hdr->len - headroom;
    char *raw_data = base + headroom;
    struct lmac_msg *tx_msg = (struct lmac_msg *)raw_data;
    if(sdio_hdr->type == E_IPC_TYPE_MSG) { 
        SSV_LOG_DBG("####################\n");
        SSV_LOG_DBG("id: %s dest: %s src: %s\n",SSV_ID2STR(tx_msg->id), SSV_TASKID2STR(tx_msg->dest_id), SSV_TASKID2STR(tx_msg->src_id));
        raw_data =(char *) tx_msg->param;
        frame_len = tx_msg->param_len;
        for(i = 1; i <= frame_len; i++) {
            SSV_LOG_DBG_ONE_LINE("%02x ", raw_data[i-1]&0xff);
            if(i % 16 == 0)
                SSV_LOG_DBG("\n");
        }
        SSV_LOG_DBG("\n");
        SSV_LOG_DBG("####################\n");
    } else if (sdio_hdr->type == E_IPC_TYPE_DATA) {
        SSV_LOG_DBG("!!!!!!!!!!!!!!!!!!!!\n");
	if(frame_len > 32)
            frame_len = 32;
        for(i = 1; i <= frame_len; i++) {
            SSV_LOG_DBG_ONE_LINE("%02x ", raw_data[i-1]&0xff);
            if(i % 16 == 0)
                SSV_LOG_DBG("\n");
        }
        SSV_LOG_DBG("\n");
        SSV_LOG_DBG("!!!!!!!!!!!!!!!!!!!!\n");
    } else {
        SSV_LOG_DBG("??? tx type: %d ???\n", sdio_hdr->type);
    }
    return 0;
}

static inline int _ssv_pktrec_cb(struct sk_buff *rx_skb, struct sk_buff *tx_skb)
{
    int status  = 0;
    if(rx_skb !=NULL) {
        status = rx_pkt_type_and_record(rx_skb);
    } else if(tx_skb !=NULL) {
        status = tx_pkt_type_and_record(tx_skb);
    } else {
        SSV_LOG_DBG("rx is null tx is null\n\n");
        status = -1;
    }
    return  status;
}

static int ssv_cmd_pktrec(void *cmd_priv, int argc, char *argv[])
{
    char *endp;
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    
    if ((argc == 3) && (!strcmp(argv[1], "enable"))) {
        /**
        * bit0:rx msg enable bit
        * bit1:tx msg enable bit
        * bit2:rx data enable bit
        * bit3:tx data enable bit
        */
        sc->dump_level = (u32)simple_strtoul(argv[2], &endp, 10);
        sc->pktrec = _ssv_pktrec_cb;
    } else if ((argc == 2) && (strcmp(argv[1], "disable") == 0)) {
        sc->pktrec = NULL;
    } else {
        ssv_snprintf_res(cmd_data, "msg [enable level|disable]\n\n");
    }
    return 0;
}

static int ssv_cmd_flowctl(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    int hci_txq_cnt = 0;

    if ((argc == 2) && (!strcmp(argv[1], "show"))) {
        if (ssv_hw->hci_ops->hci_txq_len)
        {
            hci_txq_cnt = ssv_hw->hci_ops->hci_txq_len(ssv_hw->hci_priv);
        }
        ssv_snprintf_res(cmd_data, ">> cfg flowctl = %d\n", ssv_cfg.flowctl);
        ssv_snprintf_res(cmd_data, ">> cfg low threshold  = %d\n", ssv_cfg.flowctl_low_threshold);
        ssv_snprintf_res(cmd_data, ">> cfg high threshold = %d\n", ssv_cfg.flowctl_high_threshold);
        ssv_snprintf_res(cmd_data, ">> txq current count = %d\n", hci_txq_cnt);
        ssv_snprintf_res(cmd_data, ">> txq current status %s\n", ((sc->flowctl_txq_stop) ? "stop" : "start"));
    } else {
        ssv_snprintf_res(cmd_data, ">> flowctl [show]\n");
    }
    return 0;
}

static int ssv_cmd_rf(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);

    ssv_6030_rf(ssv_hw, argc, argv);

    return 0;
}

static int ssv_cmd_rfble(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);

    ssv_6030_rfble(ssv_hw, argc, argv);

    return 0;
}

static int ssv_cmd_efuse(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);

    ssv_6030_efuse(ssv_hw, argc, argv);

    return 0;
}

static int ssv_cmd_fwreset(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;

    queue_work(sc->fw_reset_wq, &sc->fw_reset_work);

    return 0;
}

static int ssv_cmd_skipke(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;

    if ((argc == 2) && (!strcmp(argv[1], "enable"))) {
        sc->skip_ke = 1;
        ssv_snprintf_res(cmd_data, "enable skip ke\n");
    } else if ((argc == 2) && (!strcmp(argv[1], "disable"))) {
        sc->skip_ke = 0;
        ssv_snprintf_res(cmd_data, "disable skip ke\n");
    } else {
        ssv_snprintf_res(cmd_data, ">> skipke [enable/disable]\n");
    }

    return 0;
}

#ifdef SSV_MODULE_TEST
static void _ssv_cmd_modtest_usage(struct ssv_cmd_data *cmd_data)
{
    ssv_snprintf_res(cmd_data, ">> modtest [enable|disable] hci [tx|rx]\n");
    ssv_snprintf_res(cmd_data, ">> modtest set delay [fixed|random] [min ms] [max ms]\n");
    ssv_snprintf_res(cmd_data, ">> modtest [show|clear]\n");
}

static void _ssv_cmd_modtest_show(struct ssv_cmd_data *cmd_data)
{
    ssv_snprintf_res(cmd_data, ">> cfg mask = %d\n", ssv_cfg.mod_test_mask);
    ssv_snprintf_res(cmd_data, ">> cfg delay mode  = %d\n", ssv_cfg.mod_test_delay_mode);
    ssv_snprintf_res(cmd_data, ">> cfg delay = %d ~ %d (ms)\n", ssv_cfg.mod_test_delay_min, ssv_cfg.mod_test_delay_max);
}

static int ssv_cmd_modtest(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    // struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    // struct ssv_softc *sc = ssv_hw->sc;
    char *endp = NULL;
    u32 value1 = 0;
    u32 value2 = 0;
    bool enable = false;

    if (argc > 6) {
        goto USAGE;
    }

    if ((argc == 2) && (!strcmp(argv[1], "show"))) {
        //do nothing
    } else if ((argc == 2) && (!strcmp(argv[1], "clear"))) {
        ssv_cfg.mod_test_mask = 0;
        ssv_cfg.mod_test_delay_mode = 0;
        ssv_cfg.mod_test_delay_min = 1;
        ssv_cfg.mod_test_delay_max = 1;
    } else if (argc == 4) {
        if (!strcmp(argv[1], "enable")) {
            enable = true;
            ssv_cfg.mod_test_delay_min = (0 != ssv_cfg.mod_test_delay_min)?ssv_cfg.mod_test_delay_min:1;
            ssv_cfg.mod_test_delay_max = (0 != ssv_cfg.mod_test_delay_max)?ssv_cfg.mod_test_delay_max:1;
        } else if (!strcmp(argv[1], "disable")) {
            enable = false;
        }
        else {
            goto USAGE;
        }
        //hci module start
        if (!strcmp(argv[2], "hci")) {
            if (!strcmp(argv[3], "tx")) {
                if(enable == true) {
                    ssv_cfg.mod_test_mask |= SSV_MOD_TEST_HCI_TX;
                }
                else {
                    ssv_cfg.mod_test_mask &= (~SSV_MOD_TEST_HCI_TX);
                }
            } else if (!strcmp(argv[3], "rx")) {
                if(enable == true) {
                    ssv_cfg.mod_test_mask |= SSV_MOD_TEST_HCI_RX;
                }
                else {
                    ssv_cfg.mod_test_mask &= (~SSV_MOD_TEST_HCI_RX);
                }
            }
            else {
                goto USAGE;
            }
        }
        //hci module stop
        else {
            goto USAGE;
        }
    } else if ((argc == 6) && (!strcmp(argv[1], "set"))) {
        //delay time
        if (!strcmp(argv[2], "delay")) {
            if (!strcmp(argv[3], "fixed")) {
                ssv_cfg.mod_test_delay_mode = SSV_MOD_TEST_DELAY_FIXED;
            } else if (!strcmp(argv[3], "random")) {
                ssv_cfg.mod_test_delay_mode = SSV_MOD_TEST_DELAY_RANDOM;
            }
            else {
                goto USAGE;
            }
            value1 = simple_strtoul(argv[4], &endp, 10);
            value2 = simple_strtoul(argv[5], &endp, 10);
            if(value1 > value2)
            {
                goto USAGE;
            }
            ssv_cfg.mod_test_delay_min = (0 != value1)?value1:1;
            ssv_cfg.mod_test_delay_max = (0 != value2)?value2:1;
        } else {
            goto USAGE;
        }
    } else {
        goto USAGE;
    }
    _ssv_cmd_modtest_show(cmd_data);
    return 0;
USAGE:
    _ssv_cmd_modtest_usage(cmd_data);
    return 0;
}
#endif

#ifdef SSV_PERFORMANCE_WATCH
static int watch_tsk_cleanup(struct ssv_softc *sc)
{
    int ret = 0;
    if (sc->watch_tsk) {
    	 SSV_LOG_DBG("Stopping txtput task...\n");
              sc->watch_stop = 1;
    	while (sc->watch_tsk!= NULL) {
    		msleep(1);
    	}
        	 SSV_LOG_DBG("watch task is stopped.\n");
    }
    return ret;
}

static int ssv_watch_thread(void *data)
{
#define WATCH_INTERVAL_MS 4
    struct ssv_hw *ssv_hw = data;
    struct ssv_softc *sc = ssv_hw->sc;
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)sc->hci_priv;
    struct ssv6xxx_hwif_info info;
    struct sysinfo stSysinfo;
    unsigned long throughput = 0;
    int q_num;
    SSV_LOG_DBG("ssv_watch_thread started.\n");
    sc->watch_stop = 0;
    while(sc->watch_time > 0) {
        if(sc->watch_stop == 1) {
            break;
        }
        if (ssv_hw->hwif_ops->get_info)
            ssv_hw->hwif_ops->get_info(ssv_hw->dev, &info);
        if(hci_ctrl->tx_run_time == 0) {
            throughput = 0;
        } else {
            SSV_LOG_DBG("hwif with hci throughput percentatge %ld \n", info.tx_run_data_time*100/hci_ctrl->tx_run_time);
            throughput = hci_ctrl->tx_run_size*1000*8/hci_ctrl->tx_run_time /1000000;
        }
        SSV_LOG_DBG("hci resource: %ld %ld %ld %ld\n", hci_ctrl->no_tx_resource_cnt, hci_ctrl->zero_resource_cnt, hci_ctrl->less_resource_10percent_cnt, hci_ctrl->over_resource_10percent_cnt);
        SSV_LOG_DBG("hci tx: %ld Mbits %ld count/s\n", throughput, hci_ctrl->tx_run_cnt);

        if(sc->tx_run_time == 0) {
            throughput = 0;
        } else {
            throughput = sc->tx_run_size*1000*8/sc->tx_run_time /1000000;
        }
        SSV_LOG_DBG("fmac tx: %ld Mbits %ld count/s\n", throughput, sc->tx_run_cnt);
        if((sc->watch_type & 0x1) == 0x1) {
            for (q_num = (SSV_SW_TXQ_NUM - 1); q_num >= 0; q_num--)
            {
                SSV_LOG_DBG("q_num[%d] 0[%d] 0-32[%d] 32-64[%d] 64-128[%d]128-256[%d] over256[%d]\n", q_num,
                    sc->sw_txq_res[q_num][0], sc->sw_txq_res[q_num][1], sc->sw_txq_res[q_num][2], sc->sw_txq_res[q_num][3], sc->sw_txq_res[q_num][4], sc->sw_txq_res[q_num][5]);
            }
        }
        if((sc->watch_type & 0x2) == 0x2) {
            memset(&stSysinfo, 0, sizeof(struct sysinfo));
            si_meminfo(&stSysinfo);
            // SSV_LOG_DBG("stSysinfo.totalram: %lu \n", stSysinfo.totalram << (PAGE_SHIFT - 10));
            SSV_LOG_DBG("freeram: %lu \n", stSysinfo.freeram << (PAGE_SHIFT - 10));
        }
        msleep(WATCH_INTERVAL_MS);
        sc->watch_time -= WATCH_INTERVAL_MS;
    }
    sc->watch_tsk= NULL;
    return 0;
}

static int ssv_cmd_watch(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    struct ssv6xxx_hci_ctrl *hci_ctrl = (struct ssv6xxx_hci_ctrl *)sc->hci_priv;
    char *endp;
    u32 value;

    if ( (argc == 2) && (!strcmp(argv[1], "stop"))) {
    	watch_tsk_cleanup(sc);
    	return 0;
    } else if ((argc == 4) && (!strcmp(argv[1], "tx"))) {
        value = (u32)simple_strtoul(argv[3], &endp, 10);
        if(!strcmp(argv[2], "set")) {
            if((value & 0x1) == 0x1) {
                sc->skip_fmac_to_hci = 1;
            } else {
                sc->skip_fmac_to_hci = 0;
            }

            if((value & 0x2) == 0x2) {
                hci_ctrl->skip_fmac_to_hci = 1;
            } else {
                hci_ctrl->skip_fmac_to_hci = 0;
            }
            if((value & 0x4) == 0x4) {
                hci_ctrl->skip_hci_to_hwif = 1;
            } else {
                hci_ctrl->skip_hci_to_hwif = 0;
            }
        } else if(!strcmp(argv[2], "show")) {
            value = (u32)simple_strtoul(argv[3], &endp, 10);
             sc->watch_type = value;
        } else if(!strcmp(argv[2], "watch")) {
             sc->watch_time = value;
	sc->watch_tsk= kthread_run(ssv_watch_thread, ssv_hw, "ssv_watch_thread");
        }
    } else {
        ssv_snprintf_res(cmd_data, ">> performance [tx/rx] [set/watch]\n");
    }

    return 0;
}
#endif

struct st_reg_cmd_param
{
    u32 rw_method;
    u32 addr;
    u32 value;
};

struct st_reg_cmd_param register_data;
int ssv_resp_reg_rw_handler(struct ssv_softc *sc, u8* data)
{
    struct st_reg_cmd_param *param = (struct st_reg_cmd_param*)data;
    // SSV_LOG_DBG("method:%d, addr =0x%x, value = 0x%x\n", param->rw_method, param->addr, param->value);
    register_data.rw_method = param->rw_method;
    register_data.addr = param->addr;
    register_data.value = param->value;
    return 0;
}
static int ssv_cmd_regr(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    char *endp;
    
    u32 reg_addr;
    u32 msg_total_len = 0;
    struct st_reg_cmd_param *param;
    ST_IPC_PRIV_MSG *regw_msg;

    if (argc != 2) 
    {
        ssv_snprintf_res(cmd_data, "* regr [address]\n");
        ssv_snprintf_res(cmd_data, " EX: regr 0x0b102200\n");

        return 0;
    }

    reg_addr = simple_strtoul(argv[1], &endp, 16);
 
    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(struct st_reg_cmd_param));
    regw_msg = kzalloc(msg_total_len+1, GFP_KERNEL);

     if(!regw_msg)
    {
        SSV_LOG_DBG("regw, kzalloc() for msg failed!!\n");
        goto end;
    }
        
    regw_msg->msgid = E_IPC_PRIV_MSG_TYPE_CMD_REG_RW;
    regw_msg->msglen = msg_total_len;
    param= (struct st_reg_cmd_param*)regw_msg->data;
    param->rw_method = 0;
    param->addr = reg_addr;
	
    ssv_private_msg_to_hci(sc, (u8*)regw_msg, msg_total_len);

    msleep(1);
    ssv_snprintf_res(cmd_data, "ADDRESS: 0x%x\n", register_data.addr);
    ssv_snprintf_res(cmd_data, "%08x\n", register_data.value);
    if(regw_msg)
    {
        kfree(regw_msg);
    }

end:
	
    return 0;
}
static int ssv_cmd_regw(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    char *endp;
    
    u32 reg_addr, reg_value;
    u32 msg_total_len = 0;
    struct st_reg_cmd_param *param;
    ST_IPC_PRIV_MSG *regw_msg;

    if (argc != 3) 
    {
        ssv_snprintf_res(cmd_data, "* regw [address] [value]\n");
        ssv_snprintf_res(cmd_data, " EX: regw 0x0b102200 0x11\n");

        return 0;
    }

    reg_addr = simple_strtoul(argv[1], &endp, 16);
    reg_value = simple_strtoul(argv[2], &endp, 16);
    ssv_snprintf_res(cmd_data, "=> write [0x%x]: 0x%08x\n", reg_addr, reg_value);
 
    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(struct st_reg_cmd_param));
    regw_msg = kzalloc(msg_total_len+1, GFP_KERNEL);

     if(!regw_msg)
    {
        SSV_LOG_DBG("regw, kzalloc() for msg failed!!\n");
        goto end;
    }
        
    regw_msg->msgid = E_IPC_PRIV_MSG_TYPE_CMD_REG_RW;
    regw_msg->msglen = msg_total_len;
    param= (struct st_reg_cmd_param*)regw_msg->data;
    param->rw_method = 1;
    param->addr = reg_addr;
    param->value = reg_value;
	
    ssv_private_msg_to_hci(sc, (u8*)regw_msg, msg_total_len);

    if(regw_msg)
    {
        kfree(regw_msg);
    }

end:
	
    return 0;
}

static int ssv_cmd_version(void *cmd_priv, int argc, char *argv[])
{
    extern const char *sw_driver_version;
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *ssv_sc = ssv_hw->sc;
   
    ssv_snprintf_res(cmd_data, "SSV host driver version: %s Firmware version: %d\n", sw_driver_version, ssv_sc->fw_version);

    return 0;
 
}

static void _ping_mac(struct ssv_softc *sc, struct ssv_vif *my_vif)
{
    struct sk_buff *skb, *ack_skb;
    struct net_device *dev = my_vif->ndev;
    struct ssv_sta *peer = NULL;
    struct txdesc_api *desc;

    if (NULL == my_vif) {
        return;
    }

    if (NL80211_IFTYPE_AP == SSV_VIF_TYPE(my_vif)) {
        list_for_each_entry(peer, &my_vif->ap.sta_list, list)
        {
            if (peer->valid) {
                break;
            }
        }
    } else if (NL80211_IFTYPE_STATION == SSV_VIF_TYPE(my_vif)) {
        peer = my_vif->sta.ap;
    } else {
        //TODO
        return;
    }

    if (NULL == peer || peer->valid == false) {
        SSV_LOG_DBG("%s: no peer can be pinged!\n", dev->name);
        return;
    }

#if 0
    {
        int i;

        SSV_LOG_DBG("peer->mac_addr:");
        for (i = 0 ; i < ETH_ALEN ; i++) {
            SSV_LOG_DBG("%02X", peer->mac_addr[i]);
        }
        SSV_LOG_DBG("\n");
        SSV_LOG_DBG("dev->dev_addr:");
        for (i = 0 ; i < ETH_ALEN ; i++) {
            SSV_LOG_DBG("%02X", dev->dev_addr[i]);
        }
        SSV_LOG_DBG("\n");
    }
#endif

    if (NL80211_IFTYPE_AP == SSV_VIF_TYPE(my_vif)) {
        skb = ssv_build_nulldata_frame(peer->mac_addr, dev->dev_addr, dev->dev_addr, peer->qos, true);
    } else {
        skb = ssv_build_nulldata_frame(peer->mac_addr, dev->dev_addr, peer->mac_addr, peer->qos, false);
    }

    if (NULL == skb) {
        SSV_LOG_DBG("%s: fail to build faked nulldata\n", dev->name);
        return;
    }

    ssv_update_mgmt_txdesc(my_vif, peer, skb, false, false);

    desc = (struct txdesc_api *)((u8 *)skb->data + sizeof(struct sdio_hdr) + sizeof(struct tx_bmu_desc));

    desc->host.sw_ack = 0x1;
    desc->host.sw_seq = sc->now_ack_seq++;

    ack_skb = skb_clone(skb, GFP_KERNEL);

    if(!ack_skb)
    {
        SSV_LOG_DBG("%s: create TX ack skb failed!\n", dev->name);
        dev_kfree_skb_any(skb);
        return;
    }
    else
    {
        /* 
         * Use skb->cb to make tag 
         * If sw ack, it drop the ping tx with the tag directly.
         */
        ack_skb->cb[0] = 'p';
        ack_skb->cb[1] = 'i';
        ack_skb->cb[2] = 'n';
        ack_skb->cb[3] = 'g';
        ack_skb->cb[4] = 'm';
        ack_skb->cb[5] = 'a';
        ack_skb->cb[6] = 'c';
        skb_queue_tail(&sc->wait_sw_ack_q, ack_skb);
    }

    sc->ssv_ping.result = false;

    SSV_LOG_DBG(KERN_INFO "Send ping request to MAC layer via %s: seq=%u\n", dev->name, desc->host.sw_seq);

    sc->ssv_ping.start = jiffies;
    if(-1 == ssv_fmac_hci_tx(sc, skb, SSV_SW_TXQ_ID_MNG1, false, 0))
    {
        SSV_LOG_DBG("%s: send ping via HCI failed!\n", dev->name);
        dev_kfree_skb_any(skb);
        dev_kfree_skb_any(ack_skb);
        return;
    }

    while (sc->ssv_ping.result == false) {
        if (time_after(jiffies, sc->ssv_ping.start+1000)) {
            SSV_LOG_DBG(KERN_INFO "%s: No any ping response from MAC layer\n", dev->name);
            break;
        }
    }

    return;
}

static void ping_mac(struct ssv_softc *sc)
{
    struct ssv_vif *vif = NULL;
    bool found = false;

    list_for_each_entry(vif, &sc->vifs, list) {
        if (vif->up) {
            switch (SSV_VIF_TYPE(vif)) {
                case NL80211_IFTYPE_STATION:
                case NL80211_IFTYPE_AP:
                {
                    found = true;
                    _ping_mac(sc, vif);
                    break;
                }
		case NL80211_IFTYPE_ADHOC:
		case NL80211_IFTYPE_AP_VLAN:
		case NL80211_IFTYPE_WDS:
		case NL80211_IFTYPE_MESH_POINT:
		case NL80211_IFTYPE_P2P_CLIENT:
		case NL80211_IFTYPE_P2P_GO:
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
		case NL80211_IFTYPE_P2P_DEVICE:
#endif
                {
                    //TODO
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    if (!found) {
        SSV_LOG_DBG("get my vif failed!\n");
        return;
    }
}

static void ping_ipc(struct ssv_softc *sc, unsigned long size)
{
    u32 msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(unsigned int)+size);
    ST_IPC_PRIV_MSG *ping_msg = kzalloc(msg_total_len, GFP_KERNEL);

    if(!ping_msg)
    {
        SSV_LOG_DBG("ping, kzalloc() for msg failed!!\n");
        return;
    }
    ping_msg->msgid = E_IPC_PRIV_MSG_TYPE_PING_IPC_REQ;
    ping_msg->msglen = sizeof(unsigned int)+size;
    sc->ssv_ping.result = false;
    {
        int i;
        for (i = sizeof(unsigned int) ; i < ping_msg->msglen ; i++) {
            ping_msg->data[i] = i%0xFF;
        }
    }
    *(unsigned int *)ping_msg->data = ++sc->ssv_ping.seq;

    SSV_LOG_DBG(KERN_INFO "Send ping request to IPC layer: seq=%u\n", sc->ssv_ping.seq);

    sc->ssv_ping.start = jiffies;
    ssv_private_msg_to_hci(sc, (u8*)ping_msg, msg_total_len);

    if(ping_msg)
    {
        kfree(ping_msg);
    }

    while (sc->ssv_ping.result == false) {
        if (time_after(jiffies, sc->ssv_ping.start+1000)) {
            SSV_LOG_DBG(KERN_INFO "No any ping response from IPC layer\n");
            break;
        }
    }

    return;
}

#if 0
static int ssv_ping_mac_thread(void *data)
{
    struct ssv_softc *sc = data;

    while (!kthread_should_stop() && sc->ssv_ping.ping_count > 0) {
        sc->ssv_ping.ping_count--;
        ping_mac(sc);
        msleep(1000);
    }
    sc->ssv_ping.ping_tsk = NULL;
    return 0;
}

static int ssv_ping_ipc_thread(void *data)
{
    struct ssv_softc *sc = data;

    while (!kthread_should_stop() && sc->ssv_ping.ping_count > 0) {
        sc->ssv_ping.ping_count--;
        ping_ipc(sc);
        msleep(1000);
    }
    sc->ssv_ping.ping_tsk = NULL;
    return 0;
}
#endif

static int ssv_cmd_ping(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    char *endp;

    /* ping [ipc|mac] */

    if ((argc == 2) && (strcmp(argv[1], "ipc") == 0)) {
        /* ping ipc */
        ping_ipc(sc, 0);

        return 0;
    } else if ((argc == 3) && (strcmp(argv[1], "ipc") == 0)) {
        /* ping ipc [size] */
        unsigned long size = simple_strtoul(argv[2], &endp, 10);
        ping_ipc(sc, size);

        return 0;
    } else if ((argc == 2) && (strcmp(argv[1], "mac") == 0)) {
        /* ping mac */
        ping_mac(sc);

        return 0;
    } else {
        ssv_snprintf_res(cmd_data, "ping [ipc|umac]\n\n");
        return 0;
    }
    return -1;
}

static int ssv_cmd_adapt(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    u8 bAdaptEnable = 0;
    
    u32 msg_total_len = 0;
    ST_IPC_PRIV_MSG *adapt_msg;

    /**
        *  AT+ADAPT [0|1]
        */
        
    if ( (argc == 2) && (!strcmp(argv[1], "1")) ) 
    {
        ssv_snprintf_res(cmd_data, " call cmd AT+ADAPT 1\n");
        bAdaptEnable =1;
    }
    else if( (argc == 2) && (!strcmp(argv[1], "0")) )
    {
        ssv_snprintf_res(cmd_data, " call cmd AT+ADAPT 0\n");
        bAdaptEnable = 0;
    }
    else
    {
        ssv_snprintf_res(cmd_data, " AT+ADAPT [0|1]\n\n");
        ssv_snprintf_res(cmd_data, " - 0: Disable adaptive test\n\n");
        ssv_snprintf_res(cmd_data, " - 1: Enable adaptive test\n\n");
        return 0;
    }

    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG));
    adapt_msg = kzalloc(msg_total_len+1, GFP_KERNEL);

     if(!adapt_msg)
    {
        SSV_LOG_DBG("adapt, kzalloc() for msg failed!!\n");
        goto end;
    }

    if(bAdaptEnable == 1)
        adapt_msg->msgid = E_IPC_PRIV_MSG_TYPE_ADAPTIVE_OPS_ENABLE;
    else
        adapt_msg->msgid = E_IPC_PRIV_MSG_TYPE_ADAPTIVE_OPS_DISABLE;
    adapt_msg->msglen = msg_total_len;
	
    ssv_private_msg_to_hci(sc, (u8*)adapt_msg, msg_total_len);

    if(adapt_msg)
    {
        kfree(adapt_msg);
    }

end:
	
    return 0;
}
static int ssv_cmd_set_adapt(void *cmd_priv, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = (struct ssv_cmd_data *)cmd_priv;
    struct ssv_hw *ssv_hw = container_of(cmd_data, struct ssv_hw, cmd_data);
    struct ssv_softc *sc = ssv_hw->sc;
    char *endp;
    
    u32 msg_total_len = 0;
    struct st_ipc_adaptive_param *param;
    ST_IPC_PRIV_MSG *adapt_set_msg;

    /**
        *  AT+SET_ADAP <interval> <hit count> <total count> <threshold>
        */
       
    if (argc != 5) 
    {
        ssv_snprintf_res(cmd_data, " AT+SET_ADAPT <interval> <hit count> <total count> <threshold>\n\n");
        goto end;
    }
    else
    {
        ssv_snprintf_res(cmd_data, " call cmd AT+SET_ADAPT %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4]);
    }
 
    msg_total_len = (u32)(sizeof(ST_IPC_PRIV_MSG)+sizeof(struct st_ipc_adaptive_param));
    adapt_set_msg = kzalloc(msg_total_len+1, GFP_KERNEL);

     if(!adapt_set_msg)
    {
        SSV_LOG_DBG("adapt set, kzalloc() for msg failed!!\n");
        goto end;
    }
        
    adapt_set_msg->msgid = E_IPC_PRIV_MSG_TYPE_ADAPTIVE_OPS_SET;
    adapt_set_msg->msglen = msg_total_len;
    param= (struct st_ipc_adaptive_param*)adapt_set_msg->data;
    param->interval = (u32)simple_strtoul(argv[1], &endp, 0);
    param->hit_count = (u32)simple_strtoul(argv[2], &endp, 0);
    param->total_count = (u32)simple_strtoul(argv[3], &endp, 0);
    param->threshold = (u32)simple_strtoul(argv[4], &endp, 0);

    ssv_private_msg_to_hci(sc, (u8*)adapt_set_msg, msg_total_len);

    if(adapt_set_msg)
    {
        kfree(adapt_set_msg);
    }

end:
	
    return 0;
}

struct ssv_cmd_table cmd_table[] = {
    { "help",       ssv_cmd_help,       "ssv6200 command usage."            , 2048},
    { "-h",         ssv_cmd_help,       "ssv6200 command usage."            , 2048},
    { "--help",     ssv_cmd_help,       "ssv6200 command usage."            , 2048},
    { "reg",        ssv_cmd_reg,        "ssv6200 register read/write."      , 4096},
    { "hci",        ssv_cmd_hci,        "hci txq/rxq status"                , 4096},
    { "tx",         ssv_cmd_tx,         "send tx command"                   ,  256},
#if (HWIF_SUPPORT == 2)
    { "sdio",       ssv_cmd_sdio,       "SDIO command"                      ,  256},
#endif
    { "ampdu",      ssv_cmd_ampdu,      "AMPDU command"                     ,  256},
    { "rc",         ssv_cmd_rc,         "fix rate set"                      ,  256},
    { "rxtput",     ssv_cmd_rxtput,     "test rx sdio throughput"           ,  128},
    { "txtput",     ssv_cmd_txtput,     "test tx io throughput"             ,  256},
    { "mac",        ssv_cmd_mac,        "SW FMAC"                           , 4096},
    { "mib",        ssv_cmd_mib,        "mib counter related"               , 4096},
    { "pktrec",     ssv_cmd_pktrec,     "packet record"                     , 4096},
    { "flowctl",    ssv_cmd_flowctl,    "flow control"                      ,  512},
    { "rf",         ssv_cmd_rf,         "change parameters for rf tool"     ,  1024},
    { "rfble",         ssv_cmd_rfble,         "change parameters for rfble tool"     ,  1024},
    { "efuse",      ssv_cmd_efuse,      "efuse tool"                        ,  512},
#ifdef SSV_MODULE_TEST
    { "modtest",    ssv_cmd_modtest,    "module test"                       ,  512},
#endif
    { "fwreset",    ssv_cmd_fwreset,    "ssv_cmd_fwreset"                        ,  512},
    { "skipke",     ssv_cmd_skipke,     "ssv_cmd_skipke"                        ,  512},
#ifdef SSV_PERFORMANCE_WATCH
    { "performance",     ssv_cmd_watch,     "ssv_cmd_watch"                        ,  512},
#endif
    { "regr",     ssv_cmd_regr,     "for read ble register"                        ,  512},
    { "regw",     ssv_cmd_regw,     "for wirte ble register"                        ,  512},
    { "version",  ssv_cmd_version,  "get version"                            ,  512},
    { "ping",     ssv_cmd_ping,     "ping tool for datapath"                 ,  128},

    { "AT+ADAPT",       ssv_cmd_adapt,      "Enable/Disable adaptive"   , 512},
    { "AT+SET_ADAPT",   ssv_cmd_set_adapt,  "Set adaptive"              , 512},
    
    { NULL, NULL, NULL, 0},
};

