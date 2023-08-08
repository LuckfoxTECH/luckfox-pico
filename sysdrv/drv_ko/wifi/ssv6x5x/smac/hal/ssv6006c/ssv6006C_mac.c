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

// Include defines from config.mak to feed eclipse defines from ccflags-y
#ifdef ECLIPSE
#include <ssv_mod_conf.h>
#endif // ECLIPSE

#include <linux/version.h>

#if ((defined SSV_SUPPORT_SSV6006))
#include <linux/etherdevice.h>
#include <linux/string.h>
#include <linux/random.h>
#include <linux/platform_device.h>

#include <ssv_chip_id.h>
#include "ssv6006_cfg.h"
#include "ssv6006_mac.h"
#include "ssv6006c/ssv6006C_reg.h"
#include "ssv6006c/ssv6006C_aux.h"
#include <smac/dev.h>
#include <smac/wow.h>
#include <smac/efuse.h>

#include <hal.h>
#include "ssv6006_priv.h"
#include "ssv6006_priv_normal.h"
#include <smac/ssv_skb.h>
#include <ssvdevice/ssv_cmd.h>
#include <hci/hctrl.h>
#include <hwif/usb/usb.h>
#include <linux_80211.h>


static u32 ssv6006c_alloc_pbuf(struct ssv_softc *sc, int size, int type);
static void ssv6006c_write_key_to_hw(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv, 
        void *key, int wsid, int key_idx, enum SSV6XXX_WSID_SEC key_type);
static int ssv6006c_set_macaddr(struct ssv_hw *sh, int vif_idx);
static int ssv6006c_set_bssid(struct ssv_hw *sh, u8 *bssid, int vif_idx);
static void ssv6006c_write_hw_group_keyidx(struct ssv_hw *sh, struct ssv_vif_priv_data *vif_priv, int key_idx);
static int ssv6006c_reset_cpu(struct ssv_hw *sh);

static const u32 reg_wsid[] = {ADR_WSID0, ADR_WSID1, ADR_WSID2, ADR_WSID3, 
    ADR_WSID4, ADR_WSID5, ADR_WSID6, ADR_WSID7};

static const ssv_cabrio_reg ssv6006c_mac_ini_table[]=
{   
    //-----------------------------------------------------------------------------------------------------------------------------------------
    /* Set wmm parameter to EDCA Q4
       (use to send mgmt frame/null data frame in STA mode and broadcast frame in AP mode) */
    //{ADR_TXQ4_MTX_Q_AIFSN, 	           0xffff2101},   
    //---
    //HCI module config
    //keep it in host
    //HCI-enable 4 bits
    //RX to Host(bit2)
    //AUTO_SEQNO enable(bit3)
    //Fill tx queue info in rx pacet(Bit 25)	
    //tx rx packet debug counter enable(bit28) 
    //TX on_demand interrupt control between allocate size and transmit data mismatch
    {ADR_CONTROL,                      0x12000006},
    //---
    //RX module config
    //(bit 0)                       Enable hardware timestamp for TSF 
    //(bit8~bit15) value(28)  Time stamp write location
    {ADR_RX_TIME_STAMP_CFG,            ((28 << MRX_STP_OFST_SFT) | 0x01)},
    //-----------------------------------------------------------------------------------------------------------------------------------------
    //MAC config
    {ADR_GLBLE_SET,                    (0 << OP_MODE_SFT)  |                          /* STA mode by default */
        (0 << SNIFFER_MODE_SFT) |                      /* disable sniffer mode */
            (1 << DUP_FLT_SFT) |                           /* Enable duplicate detection */
            (SSV6006_TX_PKT_RSVD_SETTING << TX_PKT_RSVD_SFT) |
            ((u32)(RXPB_OFFSET) << PB_OFFSET_SFT) },      /* set rx packet buffer offset */

    /**
     * Disable tx/rx ether trap table.
     */
    {ADR_TX_ETHER_TYPE_0,              0x00000000},
    {ADR_TX_ETHER_TYPE_1,              0x00000000},
    {ADR_RX_ETHER_TYPE_0,              0x00000000},
    {ADR_RX_ETHER_TYPE_1,              0x00000000}, 
    /**                                               
     * Set reason trap to discard frames.                
     */                                              
    {ADR_REASON_TRAP0,                 0x7FBC7F87},
    {ADR_REASON_TRAP1,                 0x0000033F},

    // trap HW_ID not match to CPU for smartlink
    {ADR_TRAP_HW_ID,                   M_ENG_CPU},   /* Trap to CPU */


    /**                                         
     * Reset all wsid table entry to invalid.
     */                                      
    {ADR_WSID0,                        0x00000000},  
    {ADR_WSID1,                        0x00000000},
    {ADR_WSID2,                        0x00000000},  
    {ADR_WSID3,                        0x00000000},
    {ADR_WSID4,                        0x00000000},  
    {ADR_WSID5,                        0x00000000},
    {ADR_WSID6,                        0x00000000},  
    {ADR_WSID7,                        0x00000000},            
    //-----------------------------------------------------------------------------------------------------------------------------------------
    //SDIO interrupt
    /*
     * Set tx interrupt threshold for EACA0 ~ EACA3 queue & low threshold
     */
    {ADR_MASK_TYPHOST_INT_MAP,         0xffff7fff}, //bit 15 for int 15 map group
    {ADR_MASK_TYPHOST_INT_MAP_15,      0xff0fffff}, //bit 20,21,22,23
    /* update rate for ack/cts response */
    /* set B mode ack/cts rate 1M */
    {ADR_MTX_RESPFRM_RATE_TABLE_01, 0x0000},
    {ADR_MTX_RESPFRM_RATE_TABLE_02, 0x0000},
    {ADR_MTX_RESPFRM_RATE_TABLE_03, 0x0002},
    {ADR_MTX_RESPFRM_RATE_TABLE_11, 0x0000},
    {ADR_MTX_RESPFRM_RATE_TABLE_12, 0x0000},
    {ADR_MTX_RESPFRM_RATE_TABLE_13, 0x0012},
    /* set G mode ack/cts rate */
    //G_12M response 6M control rate
    {ADR_MTX_RESPFRM_RATE_TABLE_92_B2, 0x9090},
    //G_24M response 12M control rate 
    {ADR_MTX_RESPFRM_RATE_TABLE_94_B4, 0x9292},
    /* set N mode ack/cts rate */
    //MCS1(LGI) response 6M control rate
    {ADR_MTX_RESPFRM_RATE_TABLE_C1_E1, 0x9090},
    //MCS3(LGI) response 12M control rate
    {ADR_MTX_RESPFRM_RATE_TABLE_C3_E3, 0x9292},
    //MCS1(SGI) response 6M control rate
    {ADR_MTX_RESPFRM_RATE_TABLE_D1_F1, 0x9090},
    //MCS3(SGI) response 12M control rate
    {ADR_MTX_RESPFRM_RATE_TABLE_D3_F3, 0x9292},
    //SET default BA control setting
    {ADR_BA_CTRL, 0x9},

};

static void ssv6006c_sec_lut_setting(struct ssv_hw *sh)
{
    u8 lut_sel = 1;

#if 0 //Ian.Wu. It doesn't need on AMPDU 1.3
    sh->sc->ccmp_h_sel = 1;

    if (sh->cfg.hw_caps & SSV6200_HW_CAP_AMPDU_TX) {
        printk("Support AMPDU TX mode, ccmp header source must from SW\n");
        sh->sc->ccmp_h_sel = 1;
    }

    printk("CCMP header source from %s, Security LUT version V%d\n", (sh->sc->ccmp_h_sel == 1) ?  "SW" : "LUT", lut_sel+1);
    //CCMP header source select. 
    //bit22 set 0: CCMP header source from LUT (Frame to Security Engine: 80211_H + Payload)
    //          1: CCMP header source from SW  (Frame to Security Engine: 80211_H + CCMP_H + Payload + MIC)
    SMAC_REG_SET_BITS(sh, ADR_GLBLE_SET, (sh->sc->ccmp_h_sel << 22), 0x400000);
#endif    
    //LUT version selset.
    //bit23 set 0: V1 (Cabrio design)
    //          1: V2 (Turismo design)
    SMAC_REG_SET_BITS(sh, ADR_GLBLE_SET, (lut_sel << 23), 0x800000);
}

static void ssv6006c_set_page_id(struct ssv_hw *sh)
{
    u32 dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);
    int tx_page = sh->tx_info.tx_page_threshold;
    
    SMAC_REG_SET_BITS(sh, ADR_TRX_ID_THRESHOLD,
            ((sh->tx_info.tx_id_threshold << TX_ID_THOLD_SFT)|
             (sh->rx_info.rx_id_threshold << RX_ID_THOLD_SFT)),
            (TX_ID_THOLD_MSK | RX_ID_THOLD_MSK));
    
    if (dev_type == SSV_HWIF_INTERFACE_USB) 
        tx_page = tx_page + SSV6006_USB_FIFO;

    SMAC_REG_SET_BITS(sh, ADR_ID_LEN_THREADSHOLD1,
            ((tx_page << ID_TX_LEN_THOLD_SFT)|
             (sh->rx_info.rx_page_threshold << ID_RX_LEN_THOLD_SFT)),
            (ID_TX_LEN_THOLD_MSK | ID_RX_LEN_THOLD_MSK));
}

static void ssv6006c_update_page_id(struct ssv_hw *sh)
{
    HAL_INIT_TX_CFG(sh);
    HAL_INIT_RX_CFG(sh);
    ssv6006c_set_page_id(sh);   
}

static int ssv6006c_init_mac(struct ssv_hw *sh)
{
    struct ssv_softc *sc=sh->sc;
    int ret = 0, i = 0;
    u32 regval;
    u8 null_address[6]={0};

    // load mac init_table
    ret = HAL_WRITE_MAC_INI(sh);   

    if (ret)
        goto exit;

    //Enable TSF to be a hw jiffies(add one by us)

    SMAC_REG_SET_BITS(sh, ADR_MTX_BCN_EN_MISC, 
            1 << MTX_TSF_TIMER_EN_SFT, MTX_TSF_TIMER_EN_MSK);

    //RX module config
    SMAC_REG_WRITE(sh, ADR_HCI_TX_RX_INFO_SIZE,
            ((u32)(TXPB_OFFSET) << TX_PBOFFSET_SFT) |    /* packet buffer offset for tx */
            ((u32)(sh->tx_desc_len)  << TX_INFO_SIZE_SFT) |   /* tx_info_size send (bytes) (need word boundry, times of 4bytes) */
            ((u32)(sh->rx_desc_len)  << RX_INFO_SIZE_SFT) |   /* rx_info_size send (bytes) (need word boundry, times of 4bytes) */
            ((u32)(sh->rx_pinfo_pad) << RX_LAST_PHY_SIZE_SFT )    /* rx_last_phy_size send (bytes) */
            );

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //MMU[decide packet buffer no.]
    /* Setting MMU to 256 pages */
    // removed, turismo page size is fixed. remove this registers.
    //SMAC_REG_SET_BITS(sh, ADR_MMU_CTRL, (255<<MMU_SHARE_MCU_SFT), MMU_SHARE_MCU_MSK); // not exist for new mac

    //-----------------------------------------------------------------------------------------------------------------------------------------
    //Dual interface

    // Freddie ToDo: 
    //   1. Check against HW capability. Only SSV6200 ECO version support RX MAC address filter mask.
    //   2. Enable filter only for the second MA address.
    // @C600011C
    // bit0: default 1. 0 to enable don't care bit1 of RX RA, i.e. bit 1 of first byte.
    // bit1: default 1. 0 to enable don't care bit40 of RX RA, i.e. bit0 of last byte.
    // bit2: default 1. 0 to enable don't care bit41 of RX RA. i.e. bit1 of last byte.
    // bit3: default 1. 0 to accept ToDS in non-AP mode and non-ToDS in AP mode.
    /* Setting RX mask to allow muti-MAC*/
    SMAC_REG_READ(sh,ADR_MRX_WATCH_DOG, &regval);
    regval &= 0xfffffff0;
    SMAC_REG_WRITE(sh,ADR_MRX_WATCH_DOG, regval);


    //-----------------------------------------------------------------------------------------------------------------------------------------
    //Setting Tx resource low
    SET_TX_PAGE_LIMIT(sh->tx_info.tx_lowthreshold_page_trigger);
    SET_TX_COUNT_LIMIT(sh->tx_info.tx_lowthreshold_id_trigger);
    SET_TX_LIMIT_INT_EN(1);

    /**
     * Set ssv6200 mac address and set default BSSID. In hardware reset,
     * we the BSSID to 00:00:00:00:00:00.
     */
    // Freddie ToDo: Set MAC addresss when primary interface is being added.    
    ssv6006c_set_macaddr(sh, 0);
    for (i=0; i<SSV6006_NUM_HW_BSSID; i++)
        ssv6006c_set_bssid(sh, null_address, i);

    /**
     * Set Tx/Rx processing flows.
     */
    if (USE_MAC80211_RX(sh)){
        HAL_SET_RX_FLOW(sh, RX_DATA_FLOW, RX_CPU_HCI); 
    } else {
        HAL_SET_RX_FLOW(sh, RX_DATA_FLOW, RX_CIPHER_MIC_CPU_HCI);
    }
    HAL_SET_RX_FLOW(sh, RX_MGMT_FLOW, RX_CPU_HCI);
    HAL_SET_RX_FLOW(sh, RX_CTRL_FLOW, RX_CPU_HCI);

    HAL_SET_REPLAY_IGNORE(sh, 1); //?? move to init_mac

    /**
     * Set ssv6200 mac decision table for hardware. The table 
     * selection is according to the type of wireless interface: 
     * AP & STA mode.
     */
    HAL_UPDATE_DECISION_TABLE(sc);

    SMAC_REG_SET_BITS(sc->sh, ADR_GLBLE_SET, SSV6XXX_OPMODE_STA, OP_MODE_MSK);
    //Enale security LUT V2
    ssv6006c_sec_lut_setting(sh);

    /* Set Rate Report HWID */
    SMAC_REG_SET_BITS(sc->sh, ADR_MTX_RATERPT, M_ENG_CPU, MTX_RATERPT_HWID_MSK);
    /* Set rate report length */
    SET_PEERPS_REJECT_ENABLE(1); 

    /* set ba window size according to max rx aggr size*/
    SMAC_REG_WRITE(sh, ADR_AMPDU_SCOREBOAD_SIZE, sh->cfg.max_rx_aggr_size);
exit:
    return ret;

}

static void ssv6006c_reset_sysplf(struct ssv_hw *sh)
{
    SMAC_SYSPLF_RESET(sh, ADR_BRG_SW_RST, (1 << PLF_SW_RST_SFT));
}


static int ssv6006c_init_hw_sec_phy_table(struct ssv_softc *sc)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
    if (skb == NULL)
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_INIT;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_INIT);
    host_cmd->len = HOST_CMD_HDR_LEN;

    retval = HCI_SEND_CMD(sc->sh, skb);
    return retval;
}


static int ssv6006c_write_mac_ini(struct ssv_hw *sh){

    return SSV6XXX_SET_HW_TABLE(sh, ssv6006c_mac_ini_table);
}


static int ssv6006c_set_rx_flow(struct ssv_hw *sh, enum ssv_rx_flow type, u32 rxflow)
{

    switch (type){
        case RX_DATA_FLOW:
            return SMAC_REG_WRITE(sh, ADR_RX_FLOW_DATA, rxflow);

        case RX_MGMT_FLOW:
            return SMAC_REG_WRITE(sh, ADR_RX_FLOW_MNG, rxflow);

        case RX_CTRL_FLOW:
            return SMAC_REG_WRITE(sh, ADR_RX_FLOW_CTRL, rxflow);
        default:
            return 1;
    }
}

static int ssv6006c_set_rx_ctrl_flow(struct ssv_hw *sh)
{
    return ssv6006c_set_rx_flow(sh, ADR_RX_FLOW_CTRL, RX_CPU_HCI);
}

static int ssv6006c_set_macaddr(struct ssv_hw *sh, int vif_idx)
{
    int  ret = 0;

    switch (vif_idx) {
        case 0: //interface 0
            ret = SMAC_REG_WRITE(sh, ADR_STA_MAC_0, *((u32 *)&sh->cfg.maddr[0][0]));
            if (!ret)
                ret = SMAC_REG_WRITE(sh, ADR_STA_MAC_1, *((u32 *)&sh->cfg.maddr[0][4]));
            break;
        case 1: //interface 1
            ret = SMAC_REG_WRITE(sh, ADR_STA_MAC1_0, *((u32 *)&sh->cfg.maddr[1][0]));
            if (!ret)
                ret = SMAC_REG_WRITE(sh, ADR_STA_MAC1_1, *((u32 *)&sh->cfg.maddr[1][4]));
            break;
        default:
            printk("Does not support set MAC address to HW for VIF %d\n", vif_idx);
            ret = -1;
            break;
    }
    return ret;
}

static int ssv6006c_set_macaddr_2(struct ssv_hw *sh, int vif_idx, u8 *macaddr)
{
    int  ret = 0;

    switch (vif_idx) {
        case 0: //interface 0
            ret = SMAC_REG_WRITE(sh, ADR_STA_MAC_0, *((u32 *)macaddr));
            if (!ret)
                ret = SMAC_REG_WRITE(sh, ADR_STA_MAC_1, *((u32 *)(macaddr + 4)));
            break;
        case 1: //interface 1
            ret = SMAC_REG_WRITE(sh, ADR_STA_MAC1_0, *((u32 *)macaddr));
            if (!ret)
                ret = SMAC_REG_WRITE(sh, ADR_STA_MAC1_1, *((u32 *)(macaddr + 4)));
            break;
        default:
            printk("Does not support set MAC address to HW for VIF %d\n", vif_idx);
            ret = -1;
            break;
    }
    return ret;
}

static int ssv6006c_set_bssid(struct ssv_hw *sh, u8 *bssid, int vif_idx)
{
    int  ret = 0;
    struct ssv_softc *sc = sh->sc;   

    switch (vif_idx) {
        case 0: //interface 0
            memcpy(sc->bssid[vif_idx], bssid, 6);            
            ret = SMAC_REG_WRITE(sh, ADR_BSSID_0, *((u32 *)&sc->bssid[0][0]));
            if (!ret)
                ret = SMAC_REG_WRITE(sh, ADR_BSSID_1, *((u32 *)&sc->bssid[0][4]));
            break;
        case 1: //interface 1
            memcpy(sc->bssid[vif_idx], bssid, 6);
            ret = SMAC_REG_WRITE(sh, ADR_BSSID1_0, *((u32 *)&sc->bssid[1][0]));
            if (!ret)
                ret = SMAC_REG_WRITE(sh, ADR_BSSID1_1, *((u32 *)&sc->bssid[1][4]));
            break;
        default:
            printk("Does not support set BSSID to HW for VIF %d\n", vif_idx);
            ret = -1;
            break;
    }
    return ret;
}

static u64 ssv6006c_get_ic_time_tag(struct ssv_hw *sh)
{
    u32 regval;

    SMAC_REG_READ(sh, ADR_CHIP_DATE_YYYYMMDD, &regval);
    sh->chip_tag = ((u64)regval<<32);
    SMAC_REG_READ(sh, ADR_CHIP_DATE_00HHMMSS, &regval);
    sh->chip_tag |= (regval);

    return sh->chip_tag;
}

static void ssv6006c_get_chip_id(struct ssv_hw *sh)
{
    char *chip_id = sh->chip_id;
    u32 regval;

    //CHIP ID
    SMAC_REG_READ(sh, ADR_CHIP_ID_3, &regval);
    *((u32 *)&chip_id[0]) = __be32_to_cpu(regval);
    SMAC_REG_READ(sh, ADR_CHIP_ID_2, &regval);
    *((u32 *)&chip_id[4]) = __be32_to_cpu(regval);
    SMAC_REG_READ(sh, ADR_CHIP_ID_1, &regval);
    *((u32 *)&chip_id[8]) = __be32_to_cpu(regval);
    SMAC_REG_READ(sh, ADR_CHIP_ID_0, &regval);
    *((u32 *)&chip_id[12]) = __be32_to_cpu(regval);
    chip_id[12+sizeof(u32)] = 0;;
}

static void ssv6006c_set_mrx_mode(struct ssv_hw *sh, u32 val)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    int retval = 0;

    skb = ssv_skb_alloc(sh->sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return;
    }  

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->sub_h_cmd = ((val == MRX_MODE_NORMAL) ? SSV6XXX_MRX_NORMAL : SSV6XXX_MRX_PROMISCUOUS);
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_MRX_MODE;
    host_cmd->len = skb->len;
    retval = HCI_SEND_CMD(sh, skb);
    if (retval)
        printk("%s(): Fail to send mrx mode\n", __FUNCTION__);

    SMAC_REG_WRITE(sh, ADR_MRX_FLT_TB13, val);
}

static void ssv6006c_get_mrx_mode(struct ssv_hw *sh, u32 *val)
{
    SMAC_REG_READ(sh, ADR_MRX_FLT_TB13, val);
}

static u32 _ssv6006c_get_sec_key_tbl_addr(struct ssv_softc *sc)
{
    struct ssv_hw *sh = sc->sh;
    u32     word_data = 0;
    u32     sec_key_tbl = 0;

    /* Get the packet ID of secure table. */
    SMAC_REG_READ(sh, ADR_SCRT_SET, &word_data);
    sec_key_tbl = PBUF_MapIDtoPkt((word_data&SCRT_PKT_ID_MSK)>>SCRT_PKT_ID_SFT);

    return sec_key_tbl;
}

static void ssv6006c_save_hw_status(struct ssv_softc *sc)
{
    struct ssv_hw *sh = sc->sh;
    int     i = 0;
    int     address = 0;
    u32     word_data = 0;
    u32     sec_key_tbl = 0;

    /* Get the packet ID of secure table. */
    SMAC_REG_READ(sh, ADR_SCRT_SET, &word_data);
    sec_key_tbl = _ssv6006c_get_sec_key_tbl_addr(sc);

    /* security table */
    for (i = 0; i < sizeof(struct ssv6006_hw_sec); i += 4) {
        address = sec_key_tbl + i;
        SMAC_REG_READ(sh, address, &word_data);
        *(u32 *)(&sh->hw_sec_key_data[i]) = word_data;
    }
}

static void ssv6006c_restore_hw_config(struct ssv_softc *sc)
{
    struct ssv_hw *sh = sc->sh;
    int     i = 0;
    int     address = 0;
    u32     word_data = 0;
    u32     sec_key_tbl = 0;

    /* Get the packet ID of secure table. */
    SMAC_REG_READ(sh, ADR_SCRT_SET, &word_data);
    sec_key_tbl = PBUF_BASE_ADDR|(((word_data&SCRT_PKT_ID_MSK)>>SCRT_PKT_ID_SFT)<<16);;

    /* security table */
    sec_key_tbl = _ssv6006c_get_sec_key_tbl_addr(sc);
    for (i = 0; i < sizeof(struct ssv6006_hw_sec); i += 4) {
        address = sec_key_tbl + i;
        word_data = *(u32 *)(&sh->hw_sec_key_data[i]);
        SMAC_REG_WRITE(sh, address, word_data);
    }
    memset((void *)&sh->hw_sec_key_data[0], 0, sizeof(sh->hw_sec_key_data));
}

static void ssv6006c_set_hw_wsid(struct ssv_softc *sc, struct ieee80211_vif *vif,
        struct ieee80211_sta *sta, int wsid)
{
    struct ssv_sta_priv_data *sta_priv_dat=(struct ssv_sta_priv_data *)sta->drv_priv;
    struct ssv_sta_info *sta_info;/* sta_info is already protected by ssv6200_sta_add(). */

    sta_info = &sc->sta_info[wsid];/* sta_info is already protected by ssv6200_sta_add(). */
    /**
     * Allocate a free hardware WSID for the added STA. If no more
     * hardware entry present, set hw_wsid=-1 for
     * struct ssv_sta_info.
     */    
    if (sta_priv_dat->sta_idx < SSV6006_NUM_HW_STA) 
    {
        u32 reg_peer_mac0[] = {ADR_PEER_MAC0_0, ADR_PEER_MAC1_0, ADR_PEER_MAC2_0, ADR_PEER_MAC3_0, 
            ADR_PEER_MAC4_0, ADR_PEER_MAC5_0, ADR_PEER_MAC6_0, ADR_PEER_MAC7_0};        
        u32 reg_peer_mac1[] = {ADR_PEER_MAC0_1, ADR_PEER_MAC1_1, ADR_PEER_MAC2_1, ADR_PEER_MAC3_1, 
            ADR_PEER_MAC4_1, ADR_PEER_MAC5_1, ADR_PEER_MAC6_1, ADR_PEER_MAC7_1};

        /* Add STA into hardware for hardware offload */
        SMAC_REG_WRITE(sc->sh, reg_peer_mac0[wsid], *((u32 *)&sta->addr[0]));
        SMAC_REG_WRITE(sc->sh, reg_peer_mac1[wsid], *((u32 *)&sta->addr[4]));

        /* Valid this wsid entity */
        SMAC_REG_WRITE(sc->sh, reg_wsid[wsid], 1);

        sta_info->hw_wsid = sta_priv_dat->sta_idx;/* sta_info is already protected by ssv6200_sta_add(). */

    } 
}

static void ssv6006c_del_hw_wsid(struct ssv_softc *sc, int hw_wsid)
{
    if ((hw_wsid != -1) && (hw_wsid < SSV6006_NUM_HW_STA)) {
        u32 reg_peer_mac0[] = {ADR_PEER_MAC0_0, ADR_PEER_MAC1_0, ADR_PEER_MAC2_0, ADR_PEER_MAC3_0,
            ADR_PEER_MAC4_0, ADR_PEER_MAC5_0, ADR_PEER_MAC6_0, ADR_PEER_MAC7_0};
        u32 reg_peer_mac1[] = {ADR_PEER_MAC0_1, ADR_PEER_MAC1_1, ADR_PEER_MAC2_1, ADR_PEER_MAC3_1,
            ADR_PEER_MAC4_1, ADR_PEER_MAC5_1, ADR_PEER_MAC6_1, ADR_PEER_MAC7_1};
        SMAC_REG_WRITE(sc->sh, reg_wsid[hw_wsid], 0x00);
        /* Delete STA in hardware */
        SMAC_REG_WRITE(sc->sh, reg_peer_mac0[hw_wsid], 0x0);
        SMAC_REG_WRITE(sc->sh, reg_peer_mac1[hw_wsid], 0x0);
    }
}

static int _ssv6006c_write_pairwise_keyidx_to_hw(struct ssv_hw *sh, int key_idx, int wsid)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_sec_param *ptr = NULL;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    if (skb == NULL)
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_PAIRWIRE_KEY_IDX;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_PAIRWIRE_KEY_IDX);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param);

    ptr = (struct ssv_sec_param *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_sec_param));
    ptr->wsid_idx = wsid;
    ptr->key_idx = key_idx;

    printk("%s, wsid_idx %d, key_idx %d\n", __FUNCTION__, ptr->wsid_idx, ptr->key_idx);
    retval = HCI_SEND_CMD(sc->sh, skb);
    return retval;
}

static int _ssv6006c_write_group_keyidx_to_hw(struct ssv_hw *sh, int key_idx, int vif_idx)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_sec_param *ptr = NULL;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    if (skb == NULL) 
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_GROUP_KEY_IDX;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_GROUP_KEY_IDX);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param);

    ptr = (struct ssv_sec_param *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_sec_param));
    ptr->vif_idx = vif_idx;
    ptr->key_idx = key_idx;

    printk("%s, vif_idx %d, ptr->key_idx %d\n", __FUNCTION__, vif_idx, key_idx);
    retval = HCI_SEND_CMD(sc->sh, skb);
    return retval;
}

static int _ssv6006c_set_pairwise_cipher_type(struct ssv_hw *sh, u8 cipher, u8 wsid)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_sec_param *ptr = NULL;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    if (skb == NULL) 
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_PAIRWIRE_CIPHER_TYPE;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_PAIRWIRE_CIPHER_TYPE);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param);

    ptr = (struct ssv_sec_param *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_sec_param));
    ptr->wsid_idx = wsid;
    ptr->cipher = cipher;

    printk("%s, wsid_idx %d, cipher %d\n", __FUNCTION__, ptr->wsid_idx, ptr->cipher);
    retval = HCI_SEND_CMD(sc->sh, skb);
    return retval;
}

static int _ssv6006c_set_group_cipher_type(struct ssv_hw *sh, u8 cipher, u8 vif_idx)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_sec_param *ptr = NULL;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    if (skb == NULL) 
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_GROUP_CIPHER_TYPE;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_GROUP_CIPHER_TYPE);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param);

    ptr = (struct ssv_sec_param *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_sec_param));
    ptr->vif_idx = vif_idx;
    ptr->cipher = cipher;

    printk("%s, vif_idx %d, cipher %d\n", __FUNCTION__, vif_idx, cipher);
    retval = HCI_SEND_CMD(sc->sh, skb);
    return retval;
}

static int _ssv6006c_write_pairwise_key_to_hw(struct ssv_hw *sh,
        u8 wsid, u8 algorithm, u8 key_idx, const u8 *key, int key_len)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_sec_param *ptr = NULL;
    struct ssv6006_hw_sec *p_hw_sec = (struct ssv6006_hw_sec *)&sh->hw_sec_key_data[0];
    u32    pn_offset  = 0;
    u32    word_data = 0;
    u32    sec_key_tbl = 0;
    int    address = 0;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    if (skb == NULL) 
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_PAIRWIRE_KEY;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_PAIRWIRE_KEY);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param);

    ptr = (struct ssv_sec_param *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_sec_param));
    ptr->wsid_idx = wsid;
    ptr->key_idx = key_idx;
    ptr->key_len = key_len;
    ptr->alg = algorithm;
    memcpy((u8 *)ptr->key, (u8 *)key, key_len);

    printk("%s, wsid_idx %d, key_idx %d, key_len %d, alg %d\n", 
            __FUNCTION__, ptr->wsid_idx, ptr->key_idx, ptr->key_len, ptr->alg);

    retval = HCI_SEND_CMD(sc->sh, skb);
    //Restore PN value if need.
    if((p_hw_sec != NULL) && (p_hw_sec->sta_key[wsid].pair_cipher_type != SECURITY_NONE))
    {
        sec_key_tbl = _ssv6006c_get_sec_key_tbl_addr(sc);
        pn_offset = offsetof(struct ssv6006_hw_sec, sta_key)+sizeof(struct ssv6006_hw_sta_key)*wsid+offsetof(struct ssv6006_hw_sta_key, tx_pn_l);
        address = sec_key_tbl + pn_offset;
        word_data = p_hw_sec->sta_key[wsid].tx_pn_l;
        SMAC_REG_WRITE(sh, address, word_data);
        address += 4; //pointer to tx_pn_h
        word_data = p_hw_sec->sta_key[wsid].tx_pn_h;
        SMAC_REG_WRITE(sh, address, word_data);
        //Clear the stored key setting after restore.
        memset((void *)&(p_hw_sec->sta_key[wsid]), 0, sizeof(struct ssv6006_hw_sta_key));
    }
    return retval;
}

static int _ssv6006c_write_group_key_to_hw(struct ssv_hw *sh,
        u8 vif_idx, u8 algorithm, u8 key_idx, const u8 *key, int key_len)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_sec_param *ptr = NULL;
    struct ssv6006_hw_sec *p_hw_sec = (struct ssv6006_hw_sec *)&sh->hw_sec_key_data[0];
    u32    pn_offset  = 0;
    u32    word_data = 0;
    u32    sec_key_tbl = 0;
    int    address = 0;
    int retval = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    if (skb == NULL) 
        return -ENOMEM;

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SECURITY;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_SECURITY_CMD_GROUP_KEY;
    host_cmd->blocking_seq_no = 
        (((u16)SSV6XXX_HOST_CMD_SECURITY << 16) | (u16)SSV6XXX_SECURITY_CMD_GROUP_KEY);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_sec_param);

    ptr = (struct ssv_sec_param *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_sec_param));
    ptr->vif_idx = vif_idx;
    ptr->key_idx = key_idx;
    ptr->key_len = key_len;
    ptr->alg = algorithm;
    memcpy((u8 *)ptr->key, (u8 *)key, key_len);

    printk("%s, vif_idx %d, key_idx %d, key_len %d, alg %d\n", 
            __FUNCTION__, ptr->vif_idx, ptr->key_idx, ptr->key_len, ptr->alg);

    retval = HCI_SEND_CMD(sc->sh, skb);
    //Restore PN value if need.
    if((p_hw_sec != NULL) && (p_hw_sec->bss_group[vif_idx].group_cipher_type != SECURITY_NONE))
    {
        sec_key_tbl = _ssv6006c_get_sec_key_tbl_addr(sc);
        pn_offset = offsetof(struct ssv6006_hw_sec, bss_group)*vif_idx+offsetof(struct ssv6006_bss, tx_pn_l);
        address = sec_key_tbl + pn_offset;
        word_data = p_hw_sec->bss_group[vif_idx].tx_pn_l;
        SMAC_REG_WRITE(sh, address, word_data);
        address += 4; //pointer to tx_pn_h
        word_data = p_hw_sec->bss_group[vif_idx].tx_pn_h;
        SMAC_REG_WRITE(sh, address, word_data);
        //Clear the stored key setting after restore.
        memset((void *)&(p_hw_sec->bss_group[vif_idx]), 0, sizeof(struct ssv6006_bss));
    }
    return retval;
}


static void ssv6006c_set_aes_tkip_hw_crypto_group_key(struct ssv_softc *sc,
        struct ssv_vif_info *vif_info,
        struct ssv_sta_info *sta_info,
        void *param)
{
    int wsid = sta_info->hw_wsid;/* sta_info is protected by ssv6200_set_key(). */
    int key_idx = *(u8 *)param;

    if (wsid == (-1))
        return;   

    BUG_ON(key_idx == 0);

    printk("Set CCMP/TKIP group key index %d to WSID %d.\n", key_idx, wsid);

    if (vif_info->vif_priv == NULL) {
        dev_err(sc->dev, "NULL VIF.\n");
        return;
    }

    dev_info(sc->dev, "Write group key index %d to VIF %d \n", key_idx, vif_info->vif_priv->vif_idx);
    _ssv6006c_write_group_keyidx_to_hw(sc->sh, key_idx, vif_info->vif_priv->vif_idx);
}

static void ssv6006c_write_pairwise_keyidx_to_hw(struct ssv_hw *sh, int key_idx, int wsid)
{
    _ssv6006c_write_pairwise_keyidx_to_hw(sh, key_idx, wsid);
}

static void ssv6006c_write_hw_group_keyidx(struct ssv_hw *sh, struct ssv_vif_priv_data *vif_priv, int key_idx)
{
    _ssv6006c_write_group_keyidx_to_hw(sh, key_idx, vif_priv->vif_idx);
}

static int ssv6006c_write_pairwise_key_to_hw(struct ssv_softc *sc,
        int index, u8 algorithm, const u8 *key, int key_len,
        struct ieee80211_key_conf *keyconf,
        struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv)
{
    int wsid = (-1);

    if (sta_priv == NULL)
    {
        dev_err(sc->dev, "Set pair-wise key with NULL STA.\n");
        return -EOPNOTSUPP;
    }

    wsid = sta_priv->sta_info->hw_wsid;/* sta_info is already protected by ssv6200_set_key(). */

    if ((wsid < 0) || (wsid >= SSV_NUM_STA))
    {
        dev_err(sc->dev, "Set pair-wise key to invalid WSID %d.\n", wsid);
        return -EOPNOTSUPP;
    }

    dev_info(sc->dev, "Set STA %d's pair-wise key of %d bytes.\n", wsid, key_len);

    if(index >= 0)
        ssv6006c_write_pairwise_keyidx_to_hw(sc->sh, index, wsid);

    return _ssv6006c_write_pairwise_key_to_hw(sc->sh, wsid, algorithm, index, key, key_len);
}

static int ssv6006c_write_group_key_to_hw(struct ssv_softc *sc,
        int index, u8 algorithm, const u8 *key, int key_len,
        struct ieee80211_key_conf *keyconf,
        struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv)
{

    int wsid = sta_priv ? sta_priv->sta_info->hw_wsid : (-1);/* sta_info is already protected by ssv6200_set_key(). */

    if (vif_priv == NULL) {
        dev_err(sc->dev, "Setting group key to NULL VIF\n");
        return -EOPNOTSUPP;
    }

    dev_info(sc->dev, "Setting VIF %d group key %d of length %d to WSID %d.\n",
            vif_priv->vif_idx, index, key_len, wsid);

    /*save group key index */
    vif_priv->group_key_idx = index;

    if (sta_priv)
        sta_priv->group_key_idx = index;

    /* write group key index to all sta entity*/
    WARN_ON(sc->vif_info[vif_priv->vif_idx].vif_priv == NULL);
    ssv6xxx_foreach_vif_sta(sc, &sc->vif_info[vif_priv->vif_idx],
            ssv6006c_set_aes_tkip_hw_crypto_group_key, &index);
    return _ssv6006c_write_group_key_to_hw(sc->sh, vif_priv->vif_idx, algorithm, index, key, key_len);
}

static void ssv6006c_write_key_to_hw(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv, 
        void *key, int wsid, int key_idx, enum SSV6XXX_WSID_SEC key_type)
{  
    return;
}

static void ssv6006c_set_pairwise_cipher_type(struct ssv_hw *sh, u8 cipher, u8 wsid)
{
    printk(KERN_INFO "Set parewise key type %d\n", cipher);
    _ssv6006c_set_pairwise_cipher_type(sh, cipher, wsid);
}

static void ssv6006c_set_group_cipher_type(struct ssv_hw *sh, struct ssv_vif_priv_data *vif_priv, u8 cipher)
{
    printk(KERN_INFO "Set group key type %d\n", cipher);
    _ssv6006c_set_group_cipher_type(sh, cipher, vif_priv->vif_idx);
}

#ifdef CONFIG_PM
static void ssv6006c_save_clear_trap_reason(struct ssv_softc *sc)
{
    u32 trap0, trap1;
    SMAC_REG_READ(sc->sh, ADR_REASON_TRAP0, &trap0);
    SMAC_REG_READ(sc->sh, ADR_REASON_TRAP1, &trap1);
    SMAC_REG_WRITE(sc->sh, ADR_REASON_TRAP0, 0x00000000);
    SMAC_REG_WRITE(sc->sh, ADR_REASON_TRAP1, 0x00000000);
    printk("trap0 %08x, trap1 %08x\n", trap0, trap1);
}

static void ssv6006c_restore_trap_reason(struct ssv_softc *sc)
{
    SMAC_REG_WRITE(sc->sh, ADR_REASON_TRAP0, 0x7FBC7F87);
    SMAC_REG_WRITE(sc->sh, ADR_REASON_TRAP1, 0x0000033F);
}

static void ssv6006c_ps_save_reset_rx_flow(struct ssv_softc *sc)
{
#if 0
    struct ssv_hw *sh = sc->sh;

    SET_MTX_RATERPT_HWID(M_ENG_CPU);
    // don't change rx ctrl flow to trash can
    // otherwise, fw may be busy loop state to wait ba response.
    //ssv6006c_set_rx_flow(sc->sh, RX_CTRL_FLOW, RX_TRASH);
    ssv6006c_set_rx_flow(sc->sh, RX_MGMT_FLOW, RX_CPU_TRASH);
    if (USE_MAC80211_RX(sc->sh)) {
        HAL_SET_RX_FLOW(sc->sh, RX_DATA_FLOW, RX_CPU_TRASH); 
    } else {
        HAL_SET_RX_FLOW(sc->sh, RX_DATA_FLOW, RX_CIPHER_MIC_CPU_TRASH);
    }
#endif
}

static void ssv6006c_ps_restore_rx_flow(struct ssv_softc *sc)
{
#if 0
    struct ssv_hw *sh = sc->sh;

    ssv6006c_set_rx_flow(sc->sh, RX_CTRL_FLOW, RX_CPU_HCI);
    ssv6006c_set_rx_flow(sc->sh, RX_MGMT_FLOW, RX_CPU_HCI);
    if (USE_MAC80211_RX(sc->sh)){
        HAL_SET_RX_FLOW(sc->sh, RX_DATA_FLOW, RX_CPU_HCI); 
    } else {
        HAL_SET_RX_FLOW(sc->sh, RX_DATA_FLOW, RX_CIPHER_MIC_CPU_HCI); 
    }
    SET_MTX_RATERPT_HWID(M_ENG_CPU);
#endif
}

static void ssv6006c_pmu_awake(struct ssv_softc *sc)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv6xxx_ps_params *ptr;
    int dev_type = HCI_DEVICE_TYPE(sc->sh->hci.hci_ctrl);

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_ps_params));
    if (skb == NULL) {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return;
    }  

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_ps_params));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_PS;
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_ps_params);
    ptr = (struct ssv6xxx_ps_params *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv6xxx_ps_params));
    ptr->if_type = ((dev_type == SSV_HWIF_INTERFACE_USB) ? 0 : 1);
    ptr->aid = sc->ps_aid;
    ptr->ops = SSV6XXX_PS_WAKEUP_FINISH;
    ptr->seqno = (u8)sc->ps_event_cnt;
    printk("%s() ps cmd: %d\n", __FUNCTION__, ptr->seqno);
    sc->ps_event_cnt++;
    if (128 == sc->ps_event_cnt)
        sc->ps_event_cnt = 0;

    HCI_SEND_CMD(sc->sh, skb);
}

static void ssv6006c_ps_hold_on3(struct ssv_softc *sc, int value)
{
    //SMAC WoW phase 2 - tell F/W to hold ON3, or host cmd can't work.
    SMAC_REG_WRITE(sc->sh, ADR_PMU_RAM_05, value);
}
#endif

static void ssv6006c_set_wep_hw_crypto_setting (struct ssv_softc *sc,
        struct ssv_vif_info *vif_info, struct ssv_sta_info *sta_info,
        void *param)
{
    int                       wsid = sta_info->hw_wsid;/* sta_info is protected by ssv6200_set_key(). */
    struct ssv_sta_priv_data *sta_priv = (struct ssv_sta_priv_data *)sta_info->sta->drv_priv;/* sta_info is protected by ssv6200_set_key(). */
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif_info->vif->drv_priv;

    if (wsid == (-1))
        return;

    sta_priv->has_hw_encrypt = vif_priv->has_hw_encrypt;
    sta_priv->has_hw_decrypt = vif_priv->has_hw_decrypt;
    sta_priv->need_sw_encrypt = vif_priv->need_sw_encrypt;
    sta_priv->need_sw_decrypt = vif_priv->need_sw_decrypt;
    
    if (false == sta_priv->wep_key_update) {
        _ssv6006c_set_pairwise_cipher_type(sc->sh, vif_priv->wep_cipher, wsid);
        _ssv6006c_write_pairwise_keyidx_to_hw(sc->sh, vif_priv->wep_idx, wsid);
        sta_priv->wep_key_update = true;
    }
}

static void ssv6006c_set_wep_key(struct ssv_softc *sc, struct ssv_vif_priv_data *vif_priv,
        struct ssv_sta_priv_data *sta_priv, enum sec_type_en cipher, struct ieee80211_key_conf *key)
{
    if ((vif_priv->has_hw_decrypt == true) && (vif_priv->has_hw_encrypt == true)) {
        //store all wep key in group_key[] of BSSIDX
        printk("Store WEP key index %d to HW group_key[%d] of VIF %d\n", key->keyidx, key->keyidx,vif_priv->vif_idx);
        ssv6xxx_foreach_vif_sta(sc, &sc->vif_info[vif_priv->vif_idx], ssv6006c_set_wep_hw_crypto_setting, key);

        _ssv6006c_set_group_cipher_type(sc->sh, cipher, vif_priv->vif_idx);
        _ssv6006c_write_group_keyidx_to_hw(sc->sh, key->keyidx, vif_priv->vif_idx);
        _ssv6006c_write_group_key_to_hw(sc->sh, vif_priv->vif_idx, key->cipher, key->keyidx, key->key, key->keylen);
    }
    else
        printk("Not support HW security\n");
}

static void ssv6006c_set_wep_hw_crypto_key(struct ssv_softc *sc,
        struct ssv_sta_priv_data *sta_priv, struct ssv_vif_priv_data *vif_priv)
{
    if (sta_priv->sta_idx == (-1))
        return;
    
    if (false == sta_priv->wep_key_update) {
        _ssv6006c_set_pairwise_cipher_type(sc->sh, vif_priv->wep_cipher, sta_priv->sta_idx);
        _ssv6006c_write_pairwise_keyidx_to_hw(sc->sh, vif_priv->wep_idx, sta_priv->sta_idx);
        sta_priv->wep_key_update = true;
    }
}

static void ssv6006c_set_replay_ignore(struct ssv_hw *sh,u8 ignore)
{
    u32 temp;
    SMAC_REG_READ(sh,ADR_SCRT_SET,&temp);
    temp = temp & SCRT_RPLY_IGNORE_I_MSK;
    temp |= (ignore << SCRT_RPLY_IGNORE_SFT);
    SMAC_REG_WRITE(sh,ADR_SCRT_SET, temp);
}

static void ssv6006c_update_decision_table_6(struct ssv_hw *sh, u32 value)
{
    SMAC_REG_WRITE(sh, ADR_MRX_FLT_TB6, value);
}

static int ssv6006c_update_decision_table(struct ssv_softc *sc)
{
    unsigned int i;
#ifndef USE_CONCURRENT_DECI_TBL
    for(i=0; i<MAC_DECITBL1_SIZE; i++) {
        SMAC_REG_WRITE(sc->sh, ADR_MRX_FLT_TB0+i*4, 
                sc->mac_deci_tbl[i]);
        SMAC_REG_CONFIRM(sc->sh, ADR_MRX_FLT_TB0+i*4,
                sc->mac_deci_tbl[i]);  
    }
    for(i=0; i<MAC_DECITBL2_SIZE; i++) {
        SMAC_REG_WRITE(sc->sh, ADR_MRX_FLT_EN0+i*4,
                sc->mac_deci_tbl[i+MAC_DECITBL1_SIZE]); 
        SMAC_REG_CONFIRM(sc->sh, ADR_MRX_FLT_EN0+i*4,
                sc->mac_deci_tbl[i+MAC_DECITBL1_SIZE]);   
    }
#else
    extern u16 concurrent_deci_tbl[];
    for(i=0; i<MAC_DECITBL1_SIZE; i++) {
        SMAC_REG_WRITE(sc->sh, ADR_MRX_FLT_TB0+i*4, 
                concurrent_deci_tbl[i]);
        SMAC_REG_CONFIRM(sc->sh, ADR_MRX_FLT_TB0+i*4,
                concurrent_deci_tbl[i]);  
    }
    for(i=0; i<MAC_DECITBL2_SIZE; i++) {
        SMAC_REG_WRITE(sc->sh, ADR_MRX_FLT_EN0+i*4,
                concurrent_deci_tbl[i+MAC_DECITBL1_SIZE]); 
        SMAC_REG_CONFIRM(sc->sh, ADR_MRX_FLT_EN0+i*4,
                concurrent_deci_tbl[i+MAC_DECITBL1_SIZE]);   
    }
    SMAC_REG_WRITE(sc->sh, ADR_MRX_FLT_EN9,
            concurrent_deci_tbl[8]); 
    SMAC_REG_CONFIRM(sc->sh, ADR_MRX_FLT_EN10,
            concurrent_deci_tbl[9]);
    SMAC_REG_CONFIRM(sc->sh, ADR_DUAL_IDX_EXTEND, 1); 
#endif
    return 0;
}

static void ssv6006c_get_fw_version(struct ssv_hw *sh, u32 *regval)
{
    SMAC_REG_READ(sh, ADR_TX_SEG, regval);
}

static void ssv6006c_set_op_mode(struct ssv_hw *sh, u32 op_mode, int vif_idx)
{
    switch (vif_idx) {
        case 0:
            SMAC_REG_SET_BITS(sh, ADR_GLBLE_SET, op_mode, OP_MODE_MSK);
            break;
        case 1:
            SMAC_REG_SET_BITS(sh, ADR_OP_MODE1, op_mode, OP_MODE1_MSK);
            break;
        default:
            printk("Does not support set OP mode to HW for VIF %d\n", vif_idx);
            break;
    }
}

static void ssv6006c_set_dur_burst_sifs_g(struct ssv_hw *sh, u32 val)
{   
    // function removed at ssv6006 MP chio
}

static void ssv6006c_set_dur_slot(struct ssv_hw *sh, u32 val)
{
    SET_SLOTTIME(val);
} 

static void ssv6006c_set_sifs(struct ssv_hw *sh, int band)
{
    if (band == INDEX_80211_BAND_2GHZ){
        SET_SIFS(10);
        SET_SIGEXT(6);
    } else {
        SET_SIFS(16);
        SET_SIGEXT(0);
    }
} 

static void ssv6006c_set_qos_enable(struct ssv_hw *sh, bool val)
{
    //set QoS status
    SMAC_REG_SET_BITS(sh, ADR_GLBLE_SET, 
            (val<<QOS_EN_SFT), QOS_EN_MSK);

}

static void ssv6006c_set_wmm_param(struct ssv_softc *sc, 
        const struct ieee80211_tx_queue_params *params, u16 queue)
{
    struct ssv_hw *sh = sc->sh;
    u32 i = 0;
    u32 regval = 0;
    u32 cw;
    u8 hw_txqid = sc->tx.hw_txqid[queue];

    if (sc->sh->cfg.backoff_enable) {
        int backoff = (sc->sh->cfg.backoff_value & 0x0000FFFF);
        
        printk("set fix backoff value %d\n", backoff);
        SET_TXQ0_MTX_Q_RND_MODE(0x4); 
        SET_TXQ0_MTX_Q_BKF_CNT_FIX(backoff);
        
        SET_TXQ1_MTX_Q_RND_MODE(0x4); 
        SET_TXQ1_MTX_Q_BKF_CNT_FIX(backoff);
        
        SET_TXQ2_MTX_Q_RND_MODE(0x4); 
        SET_TXQ2_MTX_Q_BKF_CNT_FIX(backoff);
        
        SET_TXQ3_MTX_Q_RND_MODE(0x4); 
        SET_TXQ3_MTX_Q_BKF_CNT_FIX(backoff);
        return;
    }
    
    cw = params->aifs&0xf;
    cw|= ((ilog2(params->cw_min+1))&0xf)<<TXQ1_MTX_Q_ECWMIN_SFT;//8;
    cw|= ((ilog2(params->cw_max+1))&0xf)<<TXQ1_MTX_Q_ECWMAX_SFT;//12;
    cw|= ((params->txop)&0xff)<<TXQ1_MTX_Q_TXOP_LIMIT_SFT;//16;

    //Check whether all queues to follow the VO setting.
    if(sc->sh->cfg.wmm_follow_vo)
    {
        if(WMM_AC_VO == queue)
        {
            for(i = 0; i < WMM_NUM_AC; i++)
            {
                SMAC_REG_WRITE(sc->sh, ADR_TXQ0_MTX_Q_AIFSN+0x100*hw_txqid, cw);
            }
        }
        else
        {
            SMAC_REG_READ(sc->sh, ADR_TXQ0_MTX_Q_AIFSN+0x100*(sc->tx.hw_txqid[WMM_AC_VO]), &regval);
            if(0 != regval)
            {
                SMAC_REG_WRITE(sc->sh, ADR_TXQ0_MTX_Q_AIFSN+0x100*hw_txqid, regval);
            }
            else
            {
                SMAC_REG_WRITE(sc->sh, ADR_TXQ0_MTX_Q_AIFSN+0x100*hw_txqid, cw);
            }
        }
    }
    else
    {
        SMAC_REG_WRITE(sc->sh, ADR_TXQ0_MTX_Q_AIFSN+0x100*hw_txqid, cw);
    }
}

// allocate pbuf    
static u32 ssv6006c_alloc_pbuf(struct ssv_softc *sc, int size, int type)
{
    u32 regval, pad;
    int cnt = MAX_RETRY_COUNT;
    int page_cnt = (size + ((1 << HW_MMU_PAGE_SHIFT) - 1)) >> HW_MMU_PAGE_SHIFT;

    regval = 0;

    mutex_lock(&sc->mem_mutex);

    //brust could be dividen by 4
    pad = size%4;
    size += pad;

    do{
        //printk("[A] ssv6xxx_pbuf_alloc\n");

        SMAC_REG_WRITE(sc->sh, ADR_WR_ALC, (size | (type << 16)));
        SMAC_REG_READ(sc->sh, ADR_WR_ALC, &regval);

        if (regval == 0) {
            cnt--;
            msleep(1);
        }
        else
            break;

    } while (cnt);

    // If TX buffer is allocated, AMPDU maximum size m
    if (type == TX_BUF) {
        sc->sh->tx_page_available -= page_cnt;
        sc->sh->page_count[PACKET_ADDR_2_ID(regval)] = page_cnt;
    }

    mutex_unlock(&sc->mem_mutex);

    if (regval == 0)
        dev_err(sc->dev, "Failed to allocate packet buffer of %d bytes in %d type.",
                size, type);
    else {
        dev_info(sc->dev, "Allocated %d type packet buffer of size %d (%d) at address %x.\n",
                type, size, page_cnt, regval);
    }

    return regval;
}

static inline bool ssv6006c_mcu_input_full(struct ssv_softc *sc)
{
    u32 regval=0;

    SMAC_REG_READ(sc->sh, ADR_MCU_STATUS, &regval);
    return (CH0_FULL_MSK & regval);
}

// free pbuf
static bool ssv6006c_free_pbuf(struct ssv_softc *sc, u32 pbuf_addr)
{
    u32  regval=0;
    u16  failCount=0;
    u8  *p_tx_page_cnt = &sc->sh->page_count[PACKET_ADDR_2_ID(pbuf_addr)];

    while (ssv6006c_mcu_input_full(sc))
    {
        if (failCount++ < 1000) continue;
        printk("=============>ERROR!!MAILBOX Block[%d]\n", failCount);
        return false;
    } //Wait until input queue of cho is not full.

    mutex_lock(&sc->mem_mutex);

    // {HWID[3:0], PKTID[6:0]}
    regval = ((M_ENG_TRASH_CAN << HW_ID_OFFSET) |(pbuf_addr >> ADDRESS_OFFSET));

    printk("[A] ssv6xxx_pbuf_free addr[%08x][%x]\n", pbuf_addr, regval);
    SMAC_REG_WRITE(sc->sh, ADR_CH0_TRIG_1, regval);

    if (*p_tx_page_cnt)
    {
        sc->sh->tx_page_available += *p_tx_page_cnt;
        *p_tx_page_cnt = 0;
    }

    mutex_unlock(&sc->mem_mutex);

    return true;
}

static void ssv6006c_ampdu_auto_crc_en(struct ssv_hw *sh)
{
    // Enable HW_AUTO_CRC_32 ======================================
    SMAC_REG_SET_BITS(sh, ADR_MTX_MISC_EN, (0x1 << MTX_AMPDU_CRC8_AUTO_SFT),
            MTX_AMPDU_CRC8_AUTO_MSK);
}

static void ssv6006c_set_rx_ba(struct ssv_hw *sh, bool on, u8 *ta,
        u16 tid, u16 ssn, u8 buf_size)
{
#if 0   /* move to init_mac*/
    if (on) {
        //turn on ba session
        SMAC_REG_WRITE(sh, ADR_BA_CTRL, 
                (1 << BA_AGRE_EN_SFT)| (0x01 << BA_CTRL_SFT));
    } else {
        //turn off ba session
        if (sh->sc->rx_ba_session_count == 0)
            SMAC_REG_WRITE(sh, ADR_BA_CTRL, 0x0);
    }
#endif
    // set a wrong TID first, then HW will flush TID while receive new onw
    // to avoid some problem
    SET_BA_TID (0xf);   

}

static u8 ssv6006c_read_efuse(struct ssv_hw *sh, u8 *pbuf)
{

    u32 val,i,j;

    if (GET_EFS_RD_FLAG != 1) {
        //e-fuse data in buffer are not latest, read e-fuse again
        SET_EFS_RD_KICK(1);
        i = 0;
        while (!GET_EFS_PROGRESS_DONE) {
            i++;
            udelay(100);
            if ( i > 10000){
                printk("EFUSE read error!!\n");
                break;
            }
        }
    }

    //buffer data 0(bit0~31) is reserved for chip identity
    sh->cfg.chip_identity = REG32(ADR_EFUSE_WDATA_0_0);
    sh->cfg.chip_identity &= 0xff000000; //just read 8bit

    //store buffer data 1~ to e-fuse mapping table
    for (i = 0; i < (EFUSE_MAX_SECTION_MAP); i++) {
        val =  REG32(ADR_EFUSE_WDATA_0_1+i*4);
        for ( j = 0; j < 4; j++)
            *pbuf++ = ((val >> j*8) & 0xff);
    }
    return 1;
}

static void ssv6006c_write_efuse(struct ssv_hw *sh, u8 *data, u8 data_length)
{
#ifdef CONFIG_ENABLE_DBGMSG
    int i = 0x0;
    u8 loop = 0x0;
    u32 temp_value = 0x0;
    u32 align_4_byte = 0x0;
    u32 efuse_start_addr = 0x0;

    efuse_start_addr = ADR_EFUSE_WDATA_0_1;
    loop = data_length / 4;
    align_4_byte = (data_length % 4)?(data_length % 4):0;

    // clean efuse data buffer
    for(i = 0;i < 8; i++) {
        REG32_W(efuse_start_addr + (i * 4), 0x0);
    }

    for(i = 0;i < loop; i++) { 
        temp_value = (data[(i * 4)+3] << 24) + (data[(i * 4)+2] << 16) + (data[(i * 4)+1] << 8) + (data[(i * 4)] << 0);
        REG32_W(efuse_start_addr + (i * 4), temp_value);
    }

    temp_value = 0;
    for (i = 0; i < align_4_byte; i++) {
        temp_value += data[loop * 4 + i] << (i * 8);
        REG32_W(efuse_start_addr + (loop * 4), temp_value);
    }

    SET_RG_EN_LDO_EFUSE(0x1);
    SET_EFS_VDDQ_EN(0x1); 
    SET_EFS_WR_KICK(0x1);

    do {  
        temp_value = GET_EFS_PROGRESS_DONE;
    } while (0 == temp_value);

    SET_RG_EN_LDO_EFUSE(0x0);
    SET_EFS_VDDQ_EN(0x0); 
#endif
}

#define CLK_SRC_SYNTH_40M  4
#define CLK_80M_PHY        8
static int ssv6006c_chg_clk_src(struct ssv_hw *sh)
{
    // move these code to init pll.
#if 0
    int ret = 0;

    if (sh->cfg.clk_src_80m)
        ret = SMAC_REG_WRITE(sh, ADR_CLOCK_SELECTION, CLK_80M_PHY);
    else
        ret = SMAC_REG_WRITE(sh, ADR_CLOCK_SELECTION, CLK_SRC_SYNTH_40M);

    msleep(1);
#endif
    return 0;
}

static void ssv6006c_beacon_loss_enable(struct ssv_hw *sh)
{
    SET_RG_RX_MONITOR_ON(1);
}

static void ssv6006c_beacon_loss_disable(struct ssv_hw *sh)
{
    SET_RG_RX_MONITOR_ON(0);
}

static void ssv6006c_beacon_loss_config(struct ssv_hw *sh, u16 beacon_int, const u8 *bssid)
{
    struct ssv_softc *sc = sh->sc;  
    u32   mac_31_0;
    u16   mac_47_32;

    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_BEACON, 
            "%s(): beacon_int %x, bssid %02x:%02x:%02x:%02x:%02x:%02x\n", 
            __FUNCTION__, beacon_int, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

    memcpy(&mac_31_0, bssid, 4);
    memcpy(&mac_47_32, bssid+4, 2);

    ssv6006c_beacon_loss_disable(sh);
    SET_RG_RX_BEACON_INTERVAL(beacon_int);
    SET_RG_RX_BEACON_LOSS_CNT_LMT(10);

    SET_RG_RX_PKT_FC(0x0080);
    SET_RG_RX_PKT_ADDR1_ON(0);
    SET_RG_RX_PKT_ADDR2_ON(0);
    SET_RG_RX_PKT_ADDR3_ON(1);
    SET_RG_RX_PKT_ADDR3_31_0((u32)mac_31_0);
    SET_RG_RX_PKT_ADDR3_47_32((u16)mac_47_32);

    ssv6006c_beacon_loss_enable(sh);
    return;
}

static void ssv6006c_update_txq_mask(struct ssv_hw *sh, u32 txq_mask)
{
    SMAC_REG_SET_BITS(sh, ADR_MTX_MISC_EN,
            (txq_mask << MTX_HALT_Q_MB_SFT), MTX_HALT_Q_MB_MSK);
}

static void ssv6006c_readrg_hci_inq_info(struct ssv_hw *sh, int *hci_used_id, int *tx_use_page)
{
    int ret = 0;
    u32 regval = 0;

    *hci_used_id = 0;

    ret = SMAC_REG_READ(sh, ADR_TX_ID_ALL_INFO, &regval);
    if (ret == 0) 
        *tx_use_page = ((regval & TX_PAGE_USE_7_0_MSK) >> TX_PAGE_USE_7_0_SFT);    
}

#define MAX_HW_TXQ_INFO_LEN     1
static bool ssv6006c_readrg_txq_info(struct ssv_hw *sh, u32 *txq_info, int *hci_used_id)
{
    int ret = 0;
    struct ssv6xxx_hci_txq_info2 value;
    u32 dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);

    ret = SMAC_REG_READ(sh, ADR_TX_ID_ALL_INFO2, (u32 *)&value);
    if (ret == 0) {
        if (dev_type == SSV_HWIF_INTERFACE_USB) {
            if (value.tx_use_page > sh->tx_info.tx_page_threshold) {
                value.tx_use_page = sh->tx_info.tx_page_threshold;
            }
        }
        memcpy(txq_info, &value, sizeof(struct ssv6xxx_hci_txq_info));
    }
    *hci_used_id = 0;
    
    return ret;
}

static bool ssv6006c_dump_wsid(struct ssv_hw *sh)
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    const u8 *op_mode_str[]={"STA", "AP", "AD-HOC", "WDS"};
    const u8 *ht_mode_str[]={"Non-HT", "HT-MF", "HT-GF", "RSVD"}; 
    u32 regval;
    int s;
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;    

    // ToDO Liam
    // s < 2 should changed to SSV6006_NUM_HW_STA when above register define extended.
    for (s = 0; s < SSV6006_NUM_HW_STA; s++) {

        SMAC_REG_READ(sh, reg_wsid[s], &regval);
        snprintf_res(cmd_data, "==>WSID[%d]\n\tvalid[%d] qos[%d] op_mode[%s] ht_mode[%s]\n",
                s, regval&0x1, (regval>>1)&0x1, op_mode_str[((regval>>2)&3)], ht_mode_str[((regval>>4)&3)]);

        SMAC_REG_READ(sh, reg_wsid[s]+4, &regval);
        snprintf_res(cmd_data, "\tMAC[%02x:%02x:%02x:%02x:",
                (regval&0xff), ((regval>>8)&0xff), ((regval>>16)&0xff), ((regval>>24)&0xff));

        SMAC_REG_READ(sh, reg_wsid[s]+8, &regval);
        snprintf_res(cmd_data, "%02x:%02x]\n",
                (regval&0xff), ((regval>>8)&0xff));                                     
    }
#endif
    return 0;
}

static bool ssv6006c_dump_decision(struct ssv_hw *sh)
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32 addr, regval;
    int s;
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data; 

    snprintf_res(cmd_data, ">> Decision Table:\n");
    for(s = 0, addr = ADR_MRX_FLT_TB0; s < 16; s++, addr+=4) {
        SMAC_REG_READ(sh, addr, &regval);
        snprintf_res(cmd_data, "   [%d]: ADDR[0x%08x] = 0x%08x\n",
                s, addr, regval);
    }
    snprintf_res(cmd_data, "\n\n>> Decision Mask:\n");
    for (s = 0, addr = ADR_MRX_FLT_EN0; s < 9; s++, addr+=4) {
        SMAC_REG_READ(sh, addr, &regval);
        snprintf_res(cmd_data, "   [%d]: ADDR[0x%08x] = 0x%08x\n",
                s, addr, regval);
    }
    snprintf_res(cmd_data, "\n\n");
#endif
    return 0;
}

static u32 ssv6006c_get_ffout_cnt(u32 value, int tag)
{
    switch (tag) {
        case M_ENG_CPU: 	
            return ((value & FFO0_CNT_MSK) >> FFO0_CNT_SFT);
        case M_ENG_HWHCI: 	
            return ((value & FFO1_CNT_MSK) >> FFO1_CNT_SFT);
        case M_ENG_ENCRYPT:	
            return ((value & FFO3_CNT_MSK) >> FFO3_CNT_SFT); 	
        case M_ENG_MACRX:  	
            return ((value & FFO4_CNT_MSK) >> FFO4_CNT_SFT);
        case M_ENG_MIC: 	
            return ((value & FFO5_CNT_MSK) >> FFO5_CNT_SFT);
        case M_ENG_TX_EDCA0: 	
            return ((value & FFO6_CNT_MSK) >> FFO6_CNT_SFT);
        case M_ENG_TX_EDCA1:	
            return ((value & FFO7_CNT_MSK) >> FFO7_CNT_SFT);
        case M_ENG_TX_EDCA2:	
            return ((value & FFO8_CNT_MSK) >> FFO8_CNT_SFT);
        case M_ENG_TX_EDCA3:	
            return ((value & FFO9_CNT_MSK) >> FFO9_CNT_SFT);
        case M_ENG_TX_MNG:	
            return ((value & FFO10_CNT_MSK) >> FFO10_CNT_SFT);
        case M_ENG_ENCRYPT_SEC:	
            return ((value & FFO11_CNT_MSK) >> FFO11_CNT_SFT);
        case M_ENG_MIC_SEC:	
            return ((value & FFO12_CNT_MSK) >> FFO12_CNT_SFT);
        case M_ENG_TRASH_CAN:	
            return ((value & FFO15_CNT_MSK) >> FFO15_CNT_SFT);
        default:
            return 0;
    }
}

static u32 ssv6006c_get_in_ffcnt(u32 value, int tag)
{
    switch (tag) {
        case M_ENG_CPU: 	
            return ((value & FF0_CNT_MSK) >> FF0_CNT_SFT);
        case M_ENG_HWHCI: 	
            return ((value & FF1_CNT_MSK) >> FF1_CNT_SFT);
        case M_ENG_ENCRYPT:	
            return ((value & FF3_CNT_MSK) >> FF3_CNT_SFT); 	
        case M_ENG_MACRX:  	
            return ((value & FF4_CNT_MSK) >> FF4_CNT_SFT);
        case M_ENG_MIC: 	
            return ((value & FF5_CNT_MSK) >> FF5_CNT_SFT);
        case M_ENG_TX_EDCA0: 	
            return ((value & FF6_CNT_MSK) >> FF6_CNT_SFT);
        case M_ENG_TX_EDCA1:	
            return ((value & FF7_CNT_MSK) >> FF7_CNT_SFT);
        case M_ENG_TX_EDCA2:	
            return ((value & FF8_CNT_MSK) >> FF8_CNT_SFT);
        case M_ENG_TX_EDCA3:	
            return ((value & FF9_CNT_MSK) >> FF9_CNT_SFT);
        case M_ENG_TX_MNG:	
            return ((value & FF10_CNT_MSK) >> FF10_CNT_SFT);
        case M_ENG_ENCRYPT_SEC:	
            return ((value & FF11_CNT_MSK) >> FF11_CNT_SFT);
        case M_ENG_MIC_SEC:	
            return ((value & FF12_CNT_MSK) >> FF12_CNT_SFT);
        case M_ENG_TRASH_CAN:	
            return ((value & FF15_CNT_MSK) >> FF15_CNT_SFT);
        default:
            return 0;
    }
}

static void ssv6006c_read_ffout_cnt(struct ssv_hw *sh, 
        u32 *value, u32 *value1, u32 *value2)
{
    SMAC_REG_READ(sh, ADR_RD_FFOUT_CNT1, value);
    SMAC_REG_READ(sh, ADR_RD_FFOUT_CNT2, value1);
    SMAC_REG_READ(sh, ADR_RD_FFOUT_CNT3, value2);    
}

static void ssv6006c_read_in_ffcnt(struct ssv_hw *sh, 
        u32 *value, u32 *value1)
{
    SMAC_REG_READ(sh, ADR_RD_IN_FFCNT1, value);
    SMAC_REG_READ(sh, ADR_RD_IN_FFCNT2, value1);   
}

static void ssv6006c_read_id_len_threshold(struct ssv_hw *sh, 
        u32 *tx_len, u32 *rx_len)
{
    u32 regval = 0;

    if(SMAC_REG_READ(sh, ADR_ID_LEN_THREADSHOLD2, &regval));
    *tx_len = ((regval & TX_ID_ALC_LEN_MSK) >> TX_ID_ALC_LEN_SFT);
    *rx_len = ((regval & RX_ID_ALC_LEN_MSK) >> RX_ID_ALC_LEN_SFT);
}

static void ssv6006c_read_allid_map(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3)
{
    SMAC_REG_READ(sh, ADR_RD_ID0, id0);
    SMAC_REG_READ(sh, ADR_RD_ID1, id1);
    SMAC_REG_READ(sh, ADR_RD_ID2, id2);
    SMAC_REG_READ(sh, ADR_RD_ID3, id3);
}

static void ssv6006c_read_txid_map(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3)
{
    SMAC_REG_READ(sh, ADR_TX_ID0, id0);
    SMAC_REG_READ(sh, ADR_TX_ID1, id1);
    SMAC_REG_READ(sh, ADR_TX_ID2, id2);
    SMAC_REG_READ(sh, ADR_TX_ID3, id3);
}

static void ssv6006c_read_rxid_map(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3)
{
    SMAC_REG_READ(sh, ADR_RX_ID0, id0);
    SMAC_REG_READ(sh, ADR_RX_ID1, id1);
    SMAC_REG_READ(sh, ADR_RX_ID2, id2);
    SMAC_REG_READ(sh, ADR_RX_ID3, id3);
}

static void ssv6006c_read_tag_status(struct ssv_hw *sh, 
        u32 *ava_status)
{
    u32 regval = 0;

    if(SMAC_REG_READ(sh, ADR_TAG_STATUS, &regval));
    *ava_status = ((regval & AVA_TAG_MSK) >> AVA_TAG_SFT);
}

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
void ssv6006c_reset_mib_mac(struct ssv_hw *sh)
{
    SMAC_REG_WRITE(sh, ADR_MIB_EN, 0);
    msleep(1);
    SMAC_REG_WRITE(sh, ADR_MIB_EN, 0xffffffff);
}

static void ssv6006c_reset_mib(struct ssv_hw *sh)
{
    ssv6006c_reset_mib_mac(sh);

    HAL_RESET_MIB_PHY(sh);
}
#endif

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static void ssv6006c_list_mib(struct ssv_hw *sh)
{
    u32 addr, value;
    int i;
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;  

    addr = MIB_REG_BASE;

    for (i = 0; i < 120; i++, addr+=4) {
        SMAC_REG_READ(sh, addr, &value); 
        snprintf_res(cmd_data, "%08x ", value);

        if (((i+1) & 0x07) == 0)
            snprintf_res(cmd_data, "\n");
    }
    snprintf_res(cmd_data, "\n");  
}
#endif

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static void ssv6006c_dump_mib_rx(struct ssv_hw *sh)
{
    u32  value;
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;  

    snprintf_res(cmd_data, "HWHCI status:\n");
    snprintf_res(cmd_data, "%-12s\t%-12s\t%-12s\t%-12s\n",
            "HCI_RX_PKT_CNT", "HCI_RX_DROP_CNT", "HCI_RX_TRAP_CNT", "HCI_RX_FAIL_CNT");

    snprintf_res(cmd_data, "[%08x]\t", GET_RX_PKT_COUNTER);
    snprintf_res(cmd_data, "[%08x]\t", GET_RX_PKT_DROP_COUNTER);
    snprintf_res(cmd_data, "[%08x]\t", GET_RX_PKT_TRAP_COUNTER);
    snprintf_res(cmd_data, "[%08x]\n\n", GET_HOST_RX_FAIL_COUNTER);

    snprintf_res(cmd_data, "MAC RX status:\n");
    snprintf_res(cmd_data, "%-12s\t%-12s\t%-12s\t%-12s\n",
            "MRX_FCS_SUCC", "MRX_FCS_ERR", "MRX_ALC_FAIL", "MRX_MISS");

    SMAC_REG_READ(sh, ADR_MRX_FCS_SUCC, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_MRX_FCS_ERR, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_MRX_ALC_FAIL, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_MRX_MISS, &value); 
    snprintf_res(cmd_data, "[%08x]\n", value);

    snprintf_res(cmd_data, "%-12s\t%-12s\t%-12s\t%-12s\n",
            "MRX_MB_MISS", "MRX_NIDLE_MISS", "LEN_ALC_FAIL", "LEN_CRC_FAIL");

    SMAC_REG_READ(sh, ADR_MRX_MB_MISS, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_MRX_NIDLE_MISS, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_DBG_LEN_ALC_FAIL, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_DBG_LEN_CRC_FAIL, &value); 
    snprintf_res(cmd_data, "[%08x]\n", value);

    snprintf_res(cmd_data, "%-12s\t%-12s\t%-12s\t%-12s\n",
            "DBG_AMPDU_PASS", "DBG_AMPDU_FAIL", "ID_ALC_FAIL1", "ID_ALC_FAIL2");

    SMAC_REG_READ(sh, ADR_DBG_AMPDU_PASS, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_DBG_AMPDU_FAIL, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_ID_ALC_FAIL1, &value); 
    snprintf_res(cmd_data, "[%08x]\t", value);

    SMAC_REG_READ(sh, ADR_ID_ALC_FAIL2, &value); 
    snprintf_res(cmd_data, "[%08x]\n\n", value);
    HAL_DUMP_MIB_RX_PHY(sh);
}
#endif

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static void ssv6006c_dump_mib_tx(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "HWHCI status:\n");
    snprintf_res(cmd_data, "  %-16s  :%08x\t%-16s  :%08x\n", 
            "HCI_TX_ALLOC_CNT", GET_HCI_TX_ALLOC_CNT, "HCI_TX_PKT_CNT", GET_TX_PKT_COUNTER);
    snprintf_res(cmd_data, "  %-16s  :%08x\t%-16s  :%08x\n", 
            "HCI_TX_DROP_CNT", GET_TX_PKT_DROP_COUNTER, "HCI_TX_TRAP_CNT", GET_TX_PKT_TRAP_COUNTER);
    snprintf_res(cmd_data, "  %-16s  :%08x\n\n", "HCI_TX_FAIL_CNT", GET_HOST_TX_FAIL_COUNTER);

    snprintf_res(cmd_data, "MAC TX status:\n");
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx Group"
            , GET_MTX_GRP,"Tx Fail", GET_MTX_FAIL);
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx Retry"
            , GET_MTX_RETRY,"Tx Multi Retry", GET_MTX_MULTI_RETRY);
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx RTS success"
            , GET_MTX_RTS_SUCC,"Tx RTS Fail", GET_MTX_RTS_FAIL);
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx ACK Fail"
            , GET_MTX_ACK_FAIL,"Tx total count", GET_MTX_FRM);
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx ack count"
            , GET_MTX_ACK_TX,"Tx WSID0 success", GET_MTX_WSID0_SUCC);
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx WSID0 frame"
            , GET_MTX_WSID0_FRM,"Tx WSID0 retry", GET_MTX_WSID0_RETRY);
    snprintf_res(cmd_data, "  %-16s  :%08d\t%-16s  :%08d\n", "Tx WSID0 Total "
            , GET_MTX_WSID0_TOTAL, "CTS_TX", GET_MTX_CTS_TX);
}
#endif

static void ssv6006c_cmd_mib(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    int i, primary = 0, secondary = 0;
    struct ssv_hw *sh = sc->sh;
    /**
     *  mib [reset|rx|tx|edca]
     * (1) mib reset
     * (2) mib rx
     * (3) mib tx
     * (4) mib edca
     */
    if ((argc == 2) && (!strcmp(argv[1], "reset"))) {
        ssv6006c_reset_mib(sc->sh);
        snprintf_res(cmd_data, " => MIB reseted\n");

    } else if ((argc == 2) && (!strcmp(argv[1], "list"))) {
        ssv6006c_list_mib(sc->sh);      
    } else if ((argc == 2) && (strcmp(argv[1], "rx") == 0)) {
        ssv6006c_dump_mib_rx(sc->sh);
    } else if ((argc == 2) && (strcmp(argv[1], "tx") == 0)) {
        ssv6006c_dump_mib_tx(sc->sh);
    } else if ((argc == 2) && (strcmp(argv[1], "edcca") == 0)) {
        for (i = 0; i < 5; i++) { // show last five edca stat
            primary = GET_PRIMARY_EDCA(sc);
            secondary = GET_SECONDARY_EDCA(sc);        
            snprintf_res(cmd_data, "Primary EDCCA   :channel use percentage %d\n", primary);
            snprintf_res(cmd_data, "Secondary EDCCA :channel use percentage %d\n", secondary);
            snprintf_res(cmd_data, "Current settings: 0x%x\n\n\n", GET_RG_AGC_THRESHOLD);
            msleep(100); // edca update 100ms 
        }
    } else {
        snprintf_res(cmd_data, "mib [reset|list|rx|tx|edcca]\n\n");
    }   
#endif
}

static void ssv6006c_cmd_power_saving(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    char *endp;
    u8 triggers = 0;
    /**
     *  ps [aid]
     * aid: association id for station
     * aid 0 for disconnection
     */
    if (argc == 3) {
        sc->ps_aid = simple_strtoul(argv[1], &endp, 10);              
        triggers = simple_strtoul(argv[2], &endp, 10);              
#ifdef CONFIG_PM
        ssv6xxx_trigger_pmu(sc, triggers, 0);
#endif
    } else {
        snprintf_res(cmd_data, "ps [aid_value]\n\n");
    }   
#endif
}

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static void ssv6006c_cmd_hwinfo_mac_peer_stat(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 value, value1, peer_mode, mib;

    // enable peer mib
    SET_MTX_MIB_EN0(1);
    SET_MTX_MIB_EN1(1);
    SET_MTX_MIB_EN2(1);
    SET_MTX_MIB_EN3(1);
    SET_MTX_MIB_EN4(1);
    SET_MTX_MIB_EN5(1);
    SET_MTX_MIB_EN6(1);
    SET_MTX_MIB_EN7(1);

    MDELAY(500);

    value1 = GET_BSSID_47_32;
    value = GET_BSSID_31_0;
    snprintf_res(cmd_data, "BSSID_0    : %02x:%02x:%02x:%02x:%02x:%02x\n",
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff));

    value1 = GET_STA_MAC_47_32;
    value = GET_STA_MAC_31_0;
    snprintf_res(cmd_data, "SELF_MAC_0 : %02x:%02x:%02x:%02x:%02x:%02x\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff));

    value1 = GET_BSSID1_47_32;
    value = GET_BSSID1_31_0;
    snprintf_res(cmd_data, "BSSID_1    : %02x:%02x:%02x:%02x:%02x:%02x\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff));

    value1 = GET_STA_MAC1_47_32;
    value = GET_STA_MAC1_31_0;
    snprintf_res(cmd_data, "SELF_MAC_1 : %02x:%02x:%02x:%02x:%02x:%02x\n\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff));

    value1 = GET_PEER_MAC0_47_32;
    value = GET_PEER_MAC0_31_0;
    peer_mode = GET_PEER_OP_MODE0;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID0, &mib);
    snprintf_res(cmd_data, "PEER_MAC_0 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT0_SUCC_MSK) >> MTX_MIB_CNT0_SUCC_SFT),
            ((mib & MTX_MIB_CNT0_ATTEMPT_MSK) >> MTX_MIB_CNT0_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT0_FRAME_MSK) >> MTX_MIB_CNT0_FRAME_SFT));

    value1 = GET_PEER_MAC1_47_32;
    value = GET_PEER_MAC1_31_0;
    peer_mode = GET_PEER_OP_MODE1;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID1, &mib);
    snprintf_res(cmd_data, "PEER_MAC_1 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT1_SUCC_MSK) >> MTX_MIB_CNT1_SUCC_SFT),
            ((mib & MTX_MIB_CNT1_ATTEMPT_MSK) >> MTX_MIB_CNT1_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT1_FRAME_MSK) >> MTX_MIB_CNT1_FRAME_SFT));

    value1 = GET_PEER_MAC2_47_32;
    value = GET_PEER_MAC2_31_0;
    peer_mode = GET_PEER_OP_MODE2;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID2, &mib);
    snprintf_res(cmd_data, "PEER_MAC_2 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT2_SUCC_MSK) >> MTX_MIB_CNT2_SUCC_SFT),
            ((mib & MTX_MIB_CNT2_ATTEMPT_MSK) >> MTX_MIB_CNT2_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT2_FRAME_MSK) >> MTX_MIB_CNT2_FRAME_SFT));

    value1 = GET_PEER_MAC3_47_32;
    value = GET_PEER_MAC3_31_0;
    peer_mode = GET_PEER_OP_MODE3;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID3, &mib);
    snprintf_res(cmd_data, "PEER_MAC_3 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT3_SUCC_MSK) >> MTX_MIB_CNT3_SUCC_SFT),
            ((mib & MTX_MIB_CNT3_ATTEMPT_MSK) >> MTX_MIB_CNT3_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT3_FRAME_MSK) >> MTX_MIB_CNT3_FRAME_SFT));

    value1 = GET_PEER_MAC4_47_32;
    value = GET_PEER_MAC4_31_0;
    peer_mode = GET_PEER_OP_MODE4;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID4, &mib);
    snprintf_res(cmd_data, "PEER_MAC_4 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT4_SUCC_MSK) >> MTX_MIB_CNT4_SUCC_SFT),
            ((mib & MTX_MIB_CNT4_ATTEMPT_MSK) >> MTX_MIB_CNT4_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT4_FRAME_MSK) >> MTX_MIB_CNT4_FRAME_SFT));

    value1 = GET_PEER_MAC5_47_32;
    value = GET_PEER_MAC5_31_0;
    peer_mode = GET_PEER_OP_MODE5;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID5, &mib);
    snprintf_res(cmd_data, "PEER_MAC_5 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT5_SUCC_MSK) >> MTX_MIB_CNT5_SUCC_SFT),
            ((mib & MTX_MIB_CNT5_ATTEMPT_MSK) >> MTX_MIB_CNT5_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT5_FRAME_MSK) >> MTX_MIB_CNT5_FRAME_SFT));

    value1 = GET_PEER_MAC6_47_32;
    value = GET_PEER_MAC6_31_0;
    peer_mode = GET_PEER_OP_MODE6;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID6, &mib);
    snprintf_res(cmd_data, "PEER_MAC_6 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT6_SUCC_MSK) >> MTX_MIB_CNT6_SUCC_SFT),
            ((mib & MTX_MIB_CNT6_ATTEMPT_MSK) >> MTX_MIB_CNT6_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT6_FRAME_MSK) >> MTX_MIB_CNT6_FRAME_SFT));

    value1 = GET_PEER_MAC7_47_32;
    value = GET_PEER_MAC7_31_0;
    peer_mode = GET_PEER_OP_MODE7;
    SMAC_REG_READ(sh, ADR_MTX_MIB_WSID7, &mib);
    snprintf_res(cmd_data, "PEER_MAC_7 : %02x:%02x:%02x:%02x:%02x:%02x, mode 0x%x, succ %d, attempt %d, frame %d\n", 
            (((value >> 0) & 0xff)), ((value >> 8) & 0xff), ((value >> 16) & 0xff),
            ((value >> 24) & 0xff), ((value1 >> 0) & 0xff), ((value1 >> 8) & 0xff), peer_mode,
            ((mib & MTX_MIB_CNT7_SUCC_MSK) >> MTX_MIB_CNT7_SUCC_SFT),
            ((mib & MTX_MIB_CNT7_ATTEMPT_MSK) >> MTX_MIB_CNT7_ATTEMPT_SFT),
            ((mib & MTX_MIB_CNT7_FRAME_MSK) >> MTX_MIB_CNT7_FRAME_SFT));
}

static void ssv6006c_cmd_hwinfo_mac_stat(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "RG_MAC_LPBK                     : 0x%x\n", GET_RG_MAC_LPBK); 
    snprintf_res(cmd_data, "RG_MAC_M2M          (obsolete)  : 0x%x\n", GET_RG_MAC_M2M); 
    snprintf_res(cmd_data, "RG_PHY_LPBK                     : 0x%x\n", GET_RG_PHY_LPBK); 
    snprintf_res(cmd_data, "RG_LPBK_RX_EN                   : 0x%x\n", GET_RG_LPBK_RX_EN); 
    snprintf_res(cmd_data, "EXT_MAC_MODE        (obsolete)  : 0x%x\n", GET_EXT_MAC_MODE); 
    snprintf_res(cmd_data, "EXT_PHY_MODE        (obsolete)  : 0x%x\n", GET_EXT_PHY_MODE); 
    snprintf_res(cmd_data, "SNIFFER_MODE        rx_sniffer  : 0x%x\n", GET_SNIFFER_MODE); 
    snprintf_res(cmd_data, "AMPDU_SNIFFER       rx_sniffer  : 0x%x\n", GET_AMPDU_SNIFFER); 
    snprintf_res(cmd_data, "MTX_MTX2PHY_SLOW    tx_for_lpbk : 0x%x\n", GET_MTX_MTX2PHY_SLOW); 
    snprintf_res(cmd_data, "MTX_M2M_SLOW_PRD    tx_for_lpbk : 0x%d\n", GET_MTX_M2M_SLOW_PRD); 
}

static void ssv6006c_cmd_hwinfo_mac_scrt_stat(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "TX_PKT_RSVD        HCI          : %d\n", GET_TX_PKT_RSVD); 
    snprintf_res(cmd_data, "REASON_TRAP0       SECURITY/HCI : 0x%08x\n", GET_REASON_TRAP0); 
    snprintf_res(cmd_data, "REASON_TRAP1       SECURITY/HCI : 0x%08x\n", GET_REASON_TRAP1); 
    snprintf_res(cmd_data, "LUT_SEL_V2         SECURITY     : 0x%x\n", GET_CCMP_H_SEL); 
    snprintf_res(cmd_data, "CCMP_H_SEL         SECURITY     : 0x%x\n", GET_LUT_SEL_V2); 
    snprintf_res(cmd_data, "PAIR_SCRT          SECURITY     : 0x%x\n", GET_PAIR_SCRT); 
    snprintf_res(cmd_data, "GRP_SCRT           SECURITY     : 0x%x\n", GET_GRP_SCRT); 
    snprintf_res(cmd_data, "SCRT_PKT_ID        SECURITY     : 0x%d\n", GET_SCRT_PKT_ID); 
    snprintf_res(cmd_data, "SCRT_RPLY_IGNORE   SECURITY     : 0x%d\n", GET_SCRT_RPLY_IGNORE); 
}

static void ssv6006c_cmd_hwinfo_mac_beacon(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 value, value1;

    snprintf_res(cmd_data, "HWBCN_PKTID\n"); 
    snprintf_res(cmd_data, "\tMTX_BCN_CFG_VLD      : %d\n", GET_MTX_BCN_CFG_VLD); 
    snprintf_res(cmd_data, "\tMTX_AUTO_BCN_ONGOING : 0x%x\n", GET_MTX_AUTO_BCN_ONGOING); 
    snprintf_res(cmd_data, "\tMTX_BCN_PKT_ID0      : %d\n", GET_MTX_BCN_PKT_ID0); 
    snprintf_res(cmd_data, "\tMTX_BCN_PKT_ID1      : %d\n", GET_MTX_BCN_PKT_ID1); 
    snprintf_res(cmd_data, "\tMTX_BCN_PKTID_CH_LOCK (deprecated): 0x%x\n\n", GET_MTX_BCN_PKTID_CH_LOCK); 

    value = GET_MTX_BCN_TIMER;
    value1 = GET_MTX_BCN_PERIOD;
    snprintf_res(cmd_data, "HWBCN_TIMER\n"); 
    snprintf_res(cmd_data, "\tMTX_BCN_TIMER_EN                         : 0x%x\n", GET_MTX_BCN_TIMER_EN);
    snprintf_res(cmd_data, "\tMTX_BCN_TIMER(active interval count down): 0x%x=%d\n", value, value); 
    snprintf_res(cmd_data, "\tMTX_BCN_PERIOD(fixed period in TBTT)     : 0x%x=%d\n\n", value1, value1); 

    snprintf_res(cmd_data, "HWBCN_TSF\n"); 
    snprintf_res(cmd_data, "\tMTX_TSF_TIMER_EN         : 0x%x\n", GET_MTX_TSF_TIMER_EN); 
    snprintf_res(cmd_data, "\tMTX_BCN_TSF              : 0x%x/0x%x(U/L)\n", GET_MTX_BCN_TSF_U, GET_MTX_BCN_TSF_L); 
    snprintf_res(cmd_data, "\tMTX_TIME_STAMP_AUTO_FILL : 0x%x\n\n", GET_MTX_TIME_STAMP_AUTO_FILL); 

    snprintf_res(cmd_data, "HWBCN_DTIM\n"); 
    snprintf_res(cmd_data, "\tMTX_DTIM_CNT_AUTO_FILL  : 0x%x\n", GET_MTX_DTIM_CNT_AUTO_FILL); 
    snprintf_res(cmd_data, "\tMTX_DTIM_NUM            : %d\n", GET_MTX_DTIM_NUM); 
    snprintf_res(cmd_data, "\tMTX_DTIM_OFST0          : %d\n", GET_MTX_DTIM_OFST0); 
    snprintf_res(cmd_data, "\tMTX_DTIM_OFST1          : %d\n\n", GET_MTX_DTIM_OFST1); 

    snprintf_res(cmd_data, "HWBCN_MISC_OPTION\n"); 
    snprintf_res(cmd_data, "\tMTX_BCN_AUTO_SEQ_NO        : 0x%x\n", GET_MTX_BCN_AUTO_SEQ_NO); 
    snprintf_res(cmd_data, "\tTXQ5_DTIM_BEACON_BURST_MNG : 0x%x\n\n", GET_TXQ5_DTIM_BEACON_BURST_MNG); 

    snprintf_res(cmd_data, "HWBCN_INT_DEPRECATED\n"); 
    snprintf_res(cmd_data, "\tMTX_INT_DTIM_NUM (deprecated) : %d\n", GET_MTX_INT_DTIM_NUM); 
    snprintf_res(cmd_data, "\tMTX_INT_DTIM     (deprecated) : 0x%x\n", GET_MTX_INT_DTIM); 
    snprintf_res(cmd_data, "\tMTX_INT_BCN      (deprecated) : 0x%x\n", GET_MTX_INT_BCN); 
    snprintf_res(cmd_data, "\tMTX_EN_INT_BCN   (deprecated) : 0x%x\n", GET_MTX_EN_INT_BCN); 
    snprintf_res(cmd_data, "\tMTX_EN_INT_DTIM  (deprecated) : 0x%x\n\n", GET_MTX_EN_INT_DTIM); 
}

static void ssv6006c_cmd_hwinfo_mac_txq(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "BLOCK_TXQ : 0x%x\n", GET_BLOCK_TXQ); 
    snprintf_res(cmd_data, "TXQ_NULLDATAFRAME_GEN_EN: txq0=%x, txq1=%x, txq2=%x, txq3=%x, txq4=%x, txq5=%x\n", 
            GET_TXQ0_Q_NULLDATAFRAME_GEN_EN, GET_TXQ1_Q_NULLDATAFRAME_GEN_EN,
            GET_TXQ2_Q_NULLDATAFRAME_GEN_EN, GET_TXQ3_Q_NULLDATAFRAME_GEN_EN,
            GET_TXQ4_Q_NULLDATAFRAME_GEN_EN, GET_TXQ5_Q_NULLDATAFRAME_GEN_EN); 
    snprintf_res(cmd_data, "TXQ_MTX_Q_RND_MODE      : txq0=%d, txq1=%d, txq2=%d, txq3=%d, txq4=%d, txq5=%d\n", 
            GET_TXQ0_MTX_Q_RND_MODE, GET_TXQ1_MTX_Q_RND_MODE,
            GET_TXQ2_MTX_Q_RND_MODE, GET_TXQ3_MTX_Q_RND_MODE, 
            GET_TXQ4_MTX_Q_RND_MODE, GET_TXQ5_MTX_Q_RND_MODE); 
    snprintf_res(cmd_data, "TXQ_MTX_Q_MB_NO_RLS     : txq0=%x, txq1=%x, txq2=%x, txq3=%x, txq4=%x, txq5=%x\n", 
            GET_TXQ0_MTX_Q_MB_NO_RLS, GET_TXQ1_MTX_Q_MB_NO_RLS,
            GET_TXQ2_MTX_Q_MB_NO_RLS, GET_TXQ3_MTX_Q_MB_NO_RLS, 
            GET_TXQ4_MTX_Q_MB_NO_RLS, GET_TXQ5_MTX_Q_MB_NO_RLS); 
    snprintf_res(cmd_data, "TXQ_MTX_Q_BKF_CNT_FIX   : txq0=0x%x, txq1=0x%x, txq2=0x%x, txq3=0x%x, txq4=0x%x, txq5=0x%x\n", 
            GET_TXQ0_MTX_Q_BKF_CNT_FIX, GET_TXQ1_MTX_Q_BKF_CNT_FIX,
            GET_TXQ2_MTX_Q_BKF_CNT_FIX, GET_TXQ3_MTX_Q_BKF_CNT_FIX,
            GET_TXQ4_MTX_Q_BKF_CNT_FIX, GET_TXQ5_MTX_Q_BKF_CNT_FIX); 
}

static void ssv6006c_cmd_hwinfo_mac_edca(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "EDCA_Q0: TXOP[%d], ECWMIN[%d], ECWMAX[%d], AIFSN[%d]\n", 
            GET_TXQ0_MTX_Q_TXOP_LIMIT, GET_TXQ0_MTX_Q_ECWMIN, GET_TXQ0_MTX_Q_ECWMAX, GET_TXQ0_MTX_Q_AIFSN); 
    snprintf_res(cmd_data, "EDCA_Q1: TXOP[%d], ECWMIN[%d], ECWMAX[%d], AIFSN[%d]\n", 
            GET_TXQ1_MTX_Q_TXOP_LIMIT, GET_TXQ1_MTX_Q_ECWMIN, GET_TXQ1_MTX_Q_ECWMAX, GET_TXQ1_MTX_Q_AIFSN); 
    snprintf_res(cmd_data, "EDCA_Q2: TXOP[%d], ECWMIN[%d], ECWMAX[%d], AIFSN[%d]\n", 
            GET_TXQ2_MTX_Q_TXOP_LIMIT, GET_TXQ2_MTX_Q_ECWMIN, GET_TXQ2_MTX_Q_ECWMAX, GET_TXQ2_MTX_Q_AIFSN); 
    snprintf_res(cmd_data, "EDCA_Q3: TXOP[%d], ECWMIN[%d], ECWMAX[%d], AIFSN[%d]\n", 
            GET_TXQ3_MTX_Q_TXOP_LIMIT, GET_TXQ3_MTX_Q_ECWMIN, GET_TXQ3_MTX_Q_ECWMAX, GET_TXQ3_MTX_Q_AIFSN); 
    snprintf_res(cmd_data, "EDCA_Q4: TXOP[%d], ECWMIN[%d], ECWMAX[%d], AIFSN[%d]\n", 
            GET_TXQ4_MTX_Q_TXOP_LIMIT, GET_TXQ4_MTX_Q_ECWMIN, GET_TXQ4_MTX_Q_ECWMAX, GET_TXQ4_MTX_Q_AIFSN); 
    snprintf_res(cmd_data, "EDCA_Q5: TXOP[%d], ECWMIN[%d], ECWMAX[%d], AIFSN[%d]\n", 
            GET_TXQ5_MTX_Q_TXOP_LIMIT, GET_TXQ5_MTX_Q_ECWMIN, GET_TXQ5_MTX_Q_ECWMAX, GET_TXQ5_MTX_Q_AIFSN); 
}

static void ssv6006c_cmd_hwinfo_mac_txstat(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 regval;

    snprintf_res(cmd_data, "MTX_STAT\n");
    snprintf_res(cmd_data, "\tSTAT_ENABLE     : 0x%x\n", GET_STAT_ENABLE); 
    snprintf_res(cmd_data, "\tSTAT_FSM        : %d\n", GET_STAT_FSM); 
    snprintf_res(cmd_data, "\tSTAT_PKT_ID     : %d\n", GET_STAT_PKT_ID);
    SMAC_REG_READ(sh, ADR_STAT_CONF0, &regval);
    snprintf_res(cmd_data, "\tWD STAT_CONF0   : 0x%x\n", regval); 
    SMAC_REG_READ(sh, ADR_STAT_CONF1, &regval);
    snprintf_res(cmd_data, "\tWD STAT_CONF1   : 0x%x\n\n", regval); 

    snprintf_res(cmd_data, "MTX_SETTING\n");
    snprintf_res(cmd_data, "\tSIFS         : %d\n", GET_SIFS);
    snprintf_res(cmd_data, "\tSLOTTIME     : %d\n", GET_SLOTTIME);
    snprintf_res(cmd_data, "\tMAC_CLK_80M  : 0x%x\n", GET_MAC_CLK_80M);
    snprintf_res(cmd_data, "\tSIGEXT       : %d\n", GET_SIGEXT);
    snprintf_res(cmd_data, "\tTOUT_B       : %d\n", GET_TOUT_B);
    snprintf_res(cmd_data, "\tTOUT_AGN     : %d\n", GET_TOUT_AGN);
    snprintf_res(cmd_data, "\tEIFS_IN_SLOT : %d\n\n", GET_EIFS_IN_SLOT);

    snprintf_res(cmd_data, "FINETUNE\n");
    snprintf_res(cmd_data, "\tTXSIFS_SUB_MIN                    : %d\n", GET_TXSIFS_SUB_MIN);
    snprintf_res(cmd_data, "\tTXSIFS_SUB_MAX                    : %d\n", GET_TXSIFS_SUB_MAX);
    snprintf_res(cmd_data, "\tNAVCS_PHYCS_FALL_OFFSET_STEP      : %d\n", GET_NAVCS_PHYCS_FALL_OFFSET_STEP);
    snprintf_res(cmd_data, "\tTX_IP_FALL_OFFSET_STEP            : %d\n", GET_TX_IP_FALL_OFFSET_STEP);
    snprintf_res(cmd_data, "\tPHYTXSTART_NCYCLE                 : %d\n", GET_PHYTXSTART_NCYCLE);
    snprintf_res(cmd_data, "\tMTX_DBG_PHYRX_IFS_DELTATIME       : %d\n", GET_MTX_DBG_PHYRX_IFS_DELTATIME);
    snprintf_res(cmd_data, "\tMTX_NAV                           : %d\n\n", GET_MTX_NAV);

    snprintf_res(cmd_data, "MTX_STATUS\n");
    SMAC_REG_READ(sh, ADR_MTX_STATUS, &regval);
    snprintf_res(cmd_data, "\tMTX_TX_EN                   : 0x%x\n", 
            ((regval & RO_MTX_TX_EN_MSK) >> RO_MTX_TX_EN_SFT));
    snprintf_res(cmd_data, "\tMAC_TX_FIFO_WINC            : 0x%x\n", 
            ((regval & RO_MAC_TX_FIFO_WINC_MSK) >> RO_MAC_TX_FIFO_WINC_SFT));
    snprintf_res(cmd_data, "\tMAC_TX_FIFO_WFULL_MX        : 0x%x\n", 
            ((regval & RO_MAC_TX_FIFO_WFULL_MX_MSK)>>RO_MAC_TX_FIFO_WFULL_MX_SFT));
    snprintf_res(cmd_data, "\tMAC_TX_FIFO_WEMPTY          : 0x%x\n", 
            ((regval & RO_MAC_TX_FIFO_WEMPTY_MSK)>>RO_MAC_TX_FIFO_WEMPTY_SFT));
    snprintf_res(cmd_data, "\tTOMAC_TX_IP                 : 0x%x\n", 
            ((regval & TOMAC_TX_IP_MSK)>>TOMAC_TX_IP_SFT));
    snprintf_res(cmd_data, "\tTOMAC_ED_CCA_PRIMARY_MX     : 0x%x\n", 
            ((regval & TOMAC_ED_CCA_PRIMARY_MX_MSK) >> TOMAC_ED_CCA_PRIMARY_MX_SFT));
    snprintf_res(cmd_data, "\tTOMAC_ED_CCA_SECONDARY_MX   : 0x%x\n", 
            ((regval & TOMAC_ED_CCA_SECONDARY_MX_MSK)>>TOMAC_ED_CCA_SECONDARY_MX_SFT));
    snprintf_res(cmd_data, "\tTOMAC_CS_CCA_MX             : 0x%x\n", 
            ((regval & TOMAC_CS_CCA_MX_MSK)>>TOMAC_CS_CCA_MX_SFT));
    snprintf_res(cmd_data, "\tBT_BUSY                     : 0x%x\n\n", 
            ((regval & BT_BUSY_MSK)>>BT_BUSY_SFT));

    snprintf_res(cmd_data, "MTX_PEER_PS\n");
    snprintf_res(cmd_data, "\tMAC_TX_PEER_PS_LOCK_EN            : 0x%x\n", GET_MAC_TX_PEER_PS_LOCK_EN);
    snprintf_res(cmd_data, "\tMAC_TX_PEER_PS_LOCK_AUTOLOCK_EN   : 0x%x\n", GET_MAC_TX_PEER_PS_LOCK_AUTOLOCK_EN);
    snprintf_res(cmd_data, "\tMAC_TX_PS_LOCK_STATUS             : 0x%x\n\n", GET_MAC_TX_PS_LOCK_STATUS);
}

static void ssv6006c_cmd_hwinfo_mac_tx2phy(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "BLOCKTX_IGNORE_TOMAC_TX_IP            : 0x%x\n", GET_MTX_BLOCKTX_IGNORE_TOMAC_TX_IP);
    snprintf_res(cmd_data, "BLOCKTX_IGNORE_TOMAC_RX_EN            : 0x%x\n", GET_MTX_BLOCKTX_IGNORE_TOMAC_RX_EN);
    snprintf_res(cmd_data, "BLOCKTX_IGNORE_TOMAC_CCA_CS           : 0x%x\n", GET_MTX_BLOCKTX_IGNORE_TOMAC_CCA_CS);
    snprintf_res(cmd_data, "BLOCKTX_IGNORE_TOMAC_CCA_ED_SECONDARY : 0x%x\n", GET_MTX_BLOCKTX_IGNORE_TOMAC_CCA_ED_SECONDARY);
    snprintf_res(cmd_data, "BLOCKTX_IGNORE_TOMAC_CCA_ED_PRIMARY   : 0x%x\n", GET_MTX_BLOCKTX_IGNORE_TOMAC_CCA_ED_PRIMARY);
    snprintf_res(cmd_data, "IGNORE_PHYRX_IFS_DELTATIME            : 0x%x\n", GET_MTX_IGNORE_PHYRX_IFS_DELTATIME);
}

static void ssv6006c_cmd_hwinfo_mac_tx2ptc(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "MTX_RATERPT_HWID                : %d\n", GET_MTX_RATERPT_HWID);
    snprintf_res(cmd_data, "CTYPE_RATE_RPT                  : %d\n", GET_CTYPE_RATE_RPT);
    snprintf_res(cmd_data, "NO_PKT_BUF_REDUCTION            : 0x%x\n", GET_NO_PKT_BUF_REDUCTION);
    snprintf_res(cmd_data, "NO_REDUCE_TXALLFAIL_PKT         : 0x%x\n", GET_NO_REDUCE_TXALLFAIL_PKT);
    snprintf_res(cmd_data, "NO_REDUCE_PKT_PEERPS_MPDU       : 0x%x\n", GET_NO_REDUCE_PKT_PEERPS_MPDU);
    snprintf_res(cmd_data, "NO_REDUCE_PKT_PEERPS_AMPDUV1P2  : 0x%x\n", GET_NO_REDUCE_PKT_PEERPS_AMPDUV1P2);
    snprintf_res(cmd_data, "NO_REDUCE_PKT_PEERPS_AMPDUV1P3  : 0x%x\n", GET_NO_REDUCE_PKT_PEERPS_AMPDUV1P3);
}

static void ssv6006c_cmd_hwinfo_mac_fixrate(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "MTX_DBGOPT_FORCE_TXMAJOR_RATE       : 0x%x\n", GET_MTX_DBGOPT_FORCE_TXMAJOR_RATE);
    snprintf_res(cmd_data, "MTX_DBGOPT_FORCE_TXCTRL_RATE        : 0x%x\n", GET_MTX_DBGOPT_FORCE_TXCTRL_RATE);
    snprintf_res(cmd_data, "MTX_DBGOPT_FORCE_DO_RTS_CTS_MODE    : 0x%x\n", GET_MTX_DBGOPT_FORCE_DO_RTS_CTS_MODE);
    snprintf_res(cmd_data, "MTX_DBGOPT_FORCE_TXMAJOR_RATE_EN    : 0x%x\n", GET_MTX_DBGOPT_FORCE_TXMAJOR_RATE_EN);
    snprintf_res(cmd_data, "MTX_DBGOPT_FORCE_TXCTRL_RATE_EN     : 0x%x\n", GET_MTX_DBGOPT_FORCE_TXCTRL_RATE_EN);
    snprintf_res(cmd_data, "MTX_DBGOPT_FORCE_DO_RTS_CTS_MODE_EN : 0x%x\n", GET_MTX_DBGOPT_FORCE_DO_RTS_CTS_MODE_EN);
}

static void ssv6006c_cmd_hwinfo_lookup_fsm_mtxdma(u32 value, char *mtxstr)
{
    int len = 0;

    len = sprintf(mtxstr, "ro_fsm_mtxdma[%d]        : ", value);

    switch (value) {
        case 0:
            len = sprintf(mtxstr+len, "%s", "IDLE");
            break;
        case 1:
            len = sprintf(mtxstr+len, "%s", "RD_DES");
            break;
        case 2:
            len = sprintf(mtxstr+len, "%s", "RD_HDR");
            break;
        case 3:
            len = sprintf(mtxstr+len, "%s", "GEN_PHYTXDESC");
            break;
        case 4:
            len = sprintf(mtxstr+len, "%s", "TX_FRM");
            break;
        default:
            len = sprintf(mtxstr+len, "%s", "INVALID");
            break;
    }
}

static void ssv6006c_cmd_hwinfo_lookup_mac_tx_wait_response_phase(u32 value, char *mtxstr)
{
    int len = 0;

    len = sprintf(mtxstr, "ro_wait_response_phase[%d]   : ", value);

    switch (value) {
        case 0: 
            len = sprintf(mtxstr+len, "%s", "IDLE");
            break;
        case 1:
            len = sprintf(mtxstr+len, "%s", "WAIT_RXCCA");
            break;
        case 2:
            len = sprintf(mtxstr+len, "%s", "WAIT_YETTOUT");
            break;
        case 3:
            len = sprintf(mtxstr+len, "%s", "WAIT_RXPROC");
            break;
        default:
            len = sprintf(mtxstr+len, "%s", "INVALID");
            break;
    }
}

static void ssv6006c_cmd_hwinfo_lookup_mac_tx_mtxptc(u32 value, char *mtxstr)
{
    int len = 0;

    len = sprintf(mtxstr, "ro_fsm_mtxptc[%d]        : ", value);

    switch (value) {
        case 0: 
            len = sprintf(mtxstr+len, "%s", "IDLE");
            break;
        case 1: 
            len = sprintf(mtxstr+len, "%s", "WRDES_RATERPT");
            break;
        case 2:
            len = sprintf(mtxstr+len, "%s", "WRDES_CTYPE");
            break;
        case 3: 
            len = sprintf(mtxstr+len, "%s", "SIGNALRESULT");
            break;
        case 4: 
            len = sprintf(mtxstr+len, "%s", "WAIT_REDUCE");
            break;
        case 5: 
            len = sprintf(mtxstr+len, "%s", "LOCKQ");
            break;
        case 6: 
            len = sprintf(mtxstr+len, "%s", "EVTX");
            break;
        default:
            len = sprintf(mtxstr+len, "%s", "INVALID");
            break;
    }
}

static void ssv6006c_cmd_hwinfo_lookup_mac_tx_ptc_schedule(u32 value, char *mtxstr)
{
    int len = 0;

    len = sprintf(mtxstr, "ro_ptc_schedule[%d]      : ", value);

    switch (value) {
        case 0:
            len = sprintf(mtxstr+len, "%s", "IDLE");
            break;
        case 1: 
            len = sprintf(mtxstr+len, "%s", "WAIT_RSP_ACK");
            break;
        case 2: 
            len = sprintf(mtxstr+len, "%s", "WAIT_RSP_BA");
            break;
        case 3:
            len = sprintf(mtxstr+len, "%s", "WAIT_RSP_CTS");
            break;
        case 9:
            len = sprintf(mtxstr+len, "%s", "WAIT_SIFS_TX_ACK");
            break;
        case 10:
            len = sprintf(mtxstr+len, "%s", "WAIT_SIFS_TX_BA");
            break;
        case 11:
            len = sprintf(mtxstr+len, "%s", "WAIT_SIFS_TX_CTS");
            break;
        case 12:
            len = sprintf(mtxstr+len, "%s", "WAIT_SIFS_TX_MAJOR");
            break;
        case 13:
            len = sprintf(mtxstr+len, "%s", "WAIT_SIFS_TX_TXOP");
            break;
        case 14:
            len = sprintf(mtxstr+len, "%s", "WAIT_FRAMESEQ_TX");
            break;
        case 15:
            len = sprintf(mtxstr+len, "%s", "WAIT_FRAMESEQ_TXOP");
            break;
        default:
            len = sprintf(mtxstr+len, "%s", "INVALID");
            break;
    }
}

static void ssv6006c_cmd_hwinfo_lookup_mtxdma_cmd(u32 value, char *mtxstr)
{
    int len = 0;

    len = sprintf(mtxstr, "ro_mtxdma_cmd        : ack_0x%x, ba_0x%x, cts_0x%x, nulldata_0x%x, postctrlqtx_0x%x, qtx_0x%x", 
            ((value & 0x20) >> 5), ((value & 0x10) >> 4), ((value & 0x8) >> 3),
            ((value & 0x4) >> 2), ((value & 0x2) >> 1), ((value & 0x1) >> 0));
}

static void ssv6006c_cmd_hwinfo_mac_txfsm(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    char mtxstr[256];
    u32 regval;

    memset(mtxstr, 0, sizeof(mtxstr));
    regval = GET_RO_PTC_SCHEDULE;
    ssv6006c_cmd_hwinfo_lookup_mac_tx_ptc_schedule(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);

    memset(mtxstr, 0, sizeof(mtxstr));
    regval = GET_RO_FSM_MTXPTC;
    ssv6006c_cmd_hwinfo_lookup_mac_tx_mtxptc(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);

    snprintf_res(cmd_data, "RO_ACT_MASK              : 0x%x\n", GET_RO_ACT_MASK);
    snprintf_res(cmd_data, "RO_CAND_MASK             : 0x%x\n", GET_RO_CAND_MASK);

    memset(mtxstr, 0, sizeof(mtxstr));
    regval = GET_RO_WAIT_RESPONSE_PHASE;
    ssv6006c_cmd_hwinfo_lookup_mac_tx_wait_response_phase(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);

    snprintf_res(cmd_data, "RO_FSM_MTXHALT           : 0x%x\n", GET_RO_FSM_MTXHALT);

    memset(mtxstr, 0, sizeof(mtxstr));
    regval = GET_RO_FSM_MTXDMA;
    ssv6006c_cmd_hwinfo_lookup_fsm_mtxdma(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);

    snprintf_res(cmd_data, "RO_FSM_MTXPHYTX          : 0x%x\n", GET_RO_FSM_MTXPHYTX);

    memset(mtxstr, 0, sizeof(mtxstr));
    regval = GET_RO_MTXDMA_CMD;
    ssv6006c_cmd_hwinfo_lookup_mtxdma_cmd(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);

    snprintf_res(cmd_data, "RO_TXOP_INTERVAL         : %d\n", GET_RO_TXOP_INTERVAL);

}

static void ssv6006c_cmd_hwinfo_mac_txifs(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 ifs_st0 = GET_RO_IFSST0;
    u32 ifs_st1 = GET_RO_IFSST1;
    u32 ifs_st2 = GET_RO_IFSST2;
    u32 ifs_st3 = GET_RO_IFSST3;

    /* ST0 */
    snprintf_res(cmd_data, "r_nav_cnt                               : %d\n", ((ifs_st0 & 0x7fff0000) >> 16));
    snprintf_res(cmd_data, "r_nav_cycle_count_us                    : %d\n", ((ifs_st0 & 0x00007f00) >> 8));
    snprintf_res(cmd_data, "r_response_timeout_timer                : %d\n", ((ifs_st0 & 0x000000ff) >> 0));

    /* ST1 */
    snprintf_res(cmd_data, "r_ptc_cycle_count_us                    : %d\n", ((ifs_st1 & 0x7f000000) >> 24));
    snprintf_res(cmd_data, "r_ptc_delta_us                          : %d\n", ((ifs_st1 & 0x001f0000) >> 16));
    snprintf_res(cmd_data, "r_rx_ifs_calc                           : %d\n", ((ifs_st1 & 0x00004000) >> 14));
    snprintf_res(cmd_data, "r_sigext_sifs_medium_must_idle          : %d\n", ((ifs_st1 & 0x00002000) >> 13));
    snprintf_res(cmd_data, "r_sigext_sifs_period                    : %d\n", ((ifs_st1 & 0x00001000) >> 12));
    snprintf_res(cmd_data, "f_navcs_phycs_deassert                  : %d\n", ((ifs_st1 & 0x00000800) >> 11));
    snprintf_res(cmd_data, "r_ed_cs_phycs                           : %d\n", ((ifs_st1 & 0x00000400) >> 10));
    snprintf_res(cmd_data, "r_navcs_phycs_proc                      : %d\n", ((ifs_st1 & 0x00000200) >> 9));
    snprintf_res(cmd_data, "r_lastproc_need_sigext                  : %d\n", ((ifs_st1 & 0x00000100) >> 8));
    snprintf_res(cmd_data, "r_trig_sifs_or_slot_past                : %d\n", ((ifs_st1 & 0x00000080) >> 7));
    snprintf_res(cmd_data, "r_mac_rx_proc                           : %d\n", ((ifs_st1 & 0x00000040) >> 6));
    snprintf_res(cmd_data, "r_rx_proc                               : %d\n", ((ifs_st1 & 0x00000020) >> 5));
    snprintf_res(cmd_data, "r_tx_proc                               : %d\n", ((ifs_st1 & 0x00000010) >> 4));
    snprintf_res(cmd_data, "c_navcs_phycs_deassert_alone_set_ifs    : %d\n", ((ifs_st1 & 0x00000008) >> 3));
    snprintf_res(cmd_data, "g_sigext_proc_start                     : %d\n", ((ifs_st1 & 0x00000004) >> 2));
    snprintf_res(cmd_data, "g_tx_us_tick_sel                        : %d\n", ((ifs_st1 & 0x00000002) >> 1));
    snprintf_res(cmd_data, "g_rx_us_tick_sel                        : %d\n", ((ifs_st1 & 0x00000001) >> 0));

    /* ST2 */
    snprintf_res(cmd_data, "r_rx_ifs_deltatime                      : %d\n", ((ifs_st2 & 0x7fffff00) >> 8));
    snprintf_res(cmd_data, "r_rx_ifs_delta_us                       : %d\n", ((ifs_st2 & 0x0000001f) >> 0));

    /* ST3 */
    snprintf_res(cmd_data, "r_eifs_cnt                              : %d\n", ((ifs_st3 & 0x0000003f) >> 0));

}

static void ssv6006c_cmd_hwinfo_mac_txbase(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    char mtxstr[256];
    u32 regval;
    u32 mtx_base1 = GET_RO_MTX_BASE1;
    u32 mtx_base2 = GET_RO_MTX_BASE2;
    u32 mtx_base3 = GET_RO_MTX_BASE3;


    /* MTX_BASE1 */
    snprintf_res(cmd_data, "f_ptc_sifs                            : 0x%x\n", ((mtx_base1 & 0x04000000) >> 26));
    snprintf_res(cmd_data, "f_ptc_txsifs                          : 0x%x\n", ((mtx_base1 & 0x20000000) >> 29));
    snprintf_res(cmd_data, "f_ptc_txsifs_end                      : 0x%x\n", ((mtx_base1 & 0x10000000) >> 28));
    snprintf_res(cmd_data, "f_ptc_slot                            : 0x%x\n", ((mtx_base1 & 0x01000000) >> 24));
    snprintf_res(cmd_data, "f_ptc_txslot                          : 0x%x\n", ((mtx_base1 & 0x02000000) >> 25));
    snprintf_res(cmd_data, "f_ptc_phytxstart                      : 0x%x\n", ((mtx_base1 & 0x08000000) >> 27));
    snprintf_res(cmd_data, "mac_tx_fifo_wempty                    : 0x%x\n", ((mtx_base1 & 0x00080000) >> 19));
    snprintf_res(cmd_data, "mac_tx_fifo_wfull_mx                  : 0x%x\n", ((mtx_base1 & 0x00040000) >> 18));
    snprintf_res(cmd_data, "wait_rx_response                      : 0x%x\n", ((mtx_base1 & 0x00020000) >> 17));
    snprintf_res(cmd_data, "wait_tx_response                      : 0x%x\n", ((mtx_base1 & 0x00010000) >> 16));
    snprintf_res(cmd_data, "{cand_beacon_q_mask,cand_edca_q_mask} : 0x%x\n", ((mtx_base1 & 0x00007f00) >> 8));
    snprintf_res(cmd_data, "{act_pkt_beacon_q_mask,act_q_mask}    : 0x%x\n", ((mtx_base1 & 0x0000007f) >> 0));
    snprintf_res(cmd_data, "act_nulldatagen                       : 0x%x\n", ((mtx_base1 & 0x00000080) >> 7));

    /* MTX_BASE2 */
    snprintf_res(cmd_data, "ro_txop_interval[15:0]: %d\n", ((mtx_base2 & 0xffff0000) >> 16));
    memset(mtxstr, 0, sizeof(mtxstr));
    regval = ((mtx_base2 & 0x00003f00) >> 8);
    ssv6006c_cmd_hwinfo_lookup_mtxdma_cmd(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);
    memset(mtxstr, 0, sizeof(mtxstr));
    regval = ((mtx_base2 & 0x00000030) >> 4);
    ssv6006c_cmd_hwinfo_lookup_mac_tx_wait_response_phase(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);
    memset(mtxstr, 0, sizeof(mtxstr));
    regval = ((mtx_base2 & 0x0000000f) >> 0);
    ssv6006c_cmd_hwinfo_lookup_mac_tx_ptc_schedule(regval, mtxstr);
    snprintf_res(cmd_data, "%s\n", mtxstr);

    /* MTX_BASE3 */
    snprintf_res(cmd_data, "csr_prescaler_us : %d\n", ((mtx_base3 & 0x000000ff) >> 0));
    snprintf_res(cmd_data, "act_retry_flag   : %d\n", ((mtx_base3 & 0x80000000) >> 31));
    snprintf_res(cmd_data, "act_pktid        : %d\n", ((mtx_base3 & 0x7f000000) >> 24));
    snprintf_res(cmd_data, "act_ratesetidx   : %d\n", ((mtx_base3 & 0x00c00000) >> 22));
    snprintf_res(cmd_data, "act_trycnt       : %d\n", ((mtx_base3 & 0x000f0000) >> 16));
}

static void ssv6006c_cmd_hwinfo_mac_hci_stat(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "HCI_MONITOR_REG0 : 0x%08x\n", GET_HCI_MONITOR_REG0);
    snprintf_res(cmd_data, "HCI_MONITOR_REG2 : 0x%08x\n", GET_HCI_MONITOR_REG2);
    snprintf_res(cmd_data, "HCI_MONITOR_REG3 : 0x%08x\n", GET_HCI_MONITOR_REG3);
    snprintf_res(cmd_data, "HCI_MONITOR_REG4 : 0x%08x\n", GET_HCI_MONITOR_REG4);
    snprintf_res(cmd_data, "HCI_MONITOR_REG5 : 0x%08x\n", GET_HCI_MONITOR_REG5);
}

static void ssv6006c_cmd_hwinfo_page_id(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "PAGE ID info:\n");
    snprintf_res(cmd_data, "TX_ID_COUNT : 0x%08x\n", GET_TX_ID_COUNT);
    snprintf_res(cmd_data, "RX_ID_COUNT : 0x%08x\n", GET_RX_ID_COUNT);
    snprintf_res(cmd_data, "TX_ID_THOLD : 0x%08x\n", GET_TX_ID_THOLD);
    snprintf_res(cmd_data, "RX_ID_THOLD : 0x%08x\n", GET_RX_ID_THOLD);
    snprintf_res(cmd_data, "TX_PAGE_USE : 0x%08x\n", GET_TX_PAGE_USE_7_0);
    snprintf_res(cmd_data, "TX_PAGE_THOLD:0x%08x\n", GET_ID_TX_LEN_THOLD);
    snprintf_res(cmd_data, "RX_PAGE_THOLD:0x%08x\n", GET_ID_RX_LEN_THOLD);
}

static void ssv6006c_cmd_hwinfo_phy_pmu(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 regval;

    regval = GET_RO_AD_VBAT_OK;
    snprintf_res(cmd_data, "RO_AD_VBAT_OK[%d]       : %s\n", regval, ((1 == regval) ? "pass" : "fail"));

    regval = GET_RO_PMU_STATE;
    snprintf_res(cmd_data, "GET_RO_PMU_STATE[%d]    : %s\n", regval, ((3 == regval) ? "pass" : "fail"));

    SMAC_REG_READ(sh, ADR_PMU_REG_3, &regval);
    snprintf_res(cmd_data, "DCDC/LDO settings ADR_PMU_REG_3 is 0x%08x\n", regval);
}

static void ssv6006c_cmd_hwinfo_phy_xtal(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    int xtal_freq = 0;
    char *clock_setting[] = {"16M", "24M", "26M", "40M", "12M", "20M", 
        "25M", "32M", "19.2M", "38.4M", "52M", "Wrong settings"};

    xtal_freq = GET_RG_DP_XTAL_FREQ;
    xtal_freq = (xtal_freq < 11) ? xtal_freq : 11;
    snprintf_res(cmd_data, "DPLL reference clock setting is   : %s\n", clock_setting[xtal_freq]);

    xtal_freq = GET_RG_SX_XTAL_FREQ;
    xtal_freq = (xtal_freq < 11) ? xtal_freq : 11;
    snprintf_res(cmd_data, "SX reference clock setting is     : %s\n", clock_setting[xtal_freq]); 
}

static void ssv6006c_cmd_hwinfo_phy_sx(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "RG_PHY_MD_EN is           : %d\n", GET_RG_PHY_MD_EN);
    snprintf_res(cmd_data, "DB_DA_SX_SUB_SEL is       : %d\n", GET_DB_DA_SX_SUB_SEL);
    snprintf_res(cmd_data, "DB_DA_SX5GB_SUB_SEL is    : %d\n", GET_DB_DA_SX5GB_SUB_SEL);
    snprintf_res(cmd_data, "DB_SX_SBCAL_NTARGET is    : %d\n", GET_DB_SX_SBCAL_NTARGET);
    snprintf_res(cmd_data, "DB_SX_SBCAL_NCOUNT is     : %d\n", GET_DB_SX_SBCAL_NCOUNT);
    snprintf_res(cmd_data, "DB_SX5GB_SBCAL_NTARGET is : %d\n", GET_DB_SX5GB_SBCAL_NTARGET);
    snprintf_res(cmd_data, "DB_SX5GB_SBCAL_NCOUNT is  : %d\n", GET_DB_SX5GB_SBCAL_NCOUNT);
    snprintf_res(cmd_data, "RO_RF_CH_FREQ is          : %d MHz\n", GET_RO_RF_CH_FREQ);
}

void ssv6006c_cmd_hwinfo_phy_cali(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    int i;
    u32 regval, wifi_dc_addr, value;

    snprintf_res(cmd_data, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "2G Rx DC calibration results\n");
    snprintf_res(cmd_data, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (i = 0; i < 21; i++) {
        wifi_dc_addr = ADR_WF_DCOC_IDAC_REGISTER1 + (i<<2);
        SMAC_REG_READ(sh, wifi_dc_addr, &regval);
        snprintf_res(cmd_data, "Turismo register 0x%08x : 0x%08x\n", wifi_dc_addr, regval);
    }

    snprintf_res(cmd_data, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "5G Rx DC calibration results\n");
    snprintf_res(cmd_data, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (i = 0; i < 21; i++) {
        wifi_dc_addr = ADR_5G_DCOC_IDAC_REGISTER1 + (i<<2);
        SMAC_REG_READ(sh, wifi_dc_addr, &regval);
        snprintf_res(cmd_data, "Turismo register 0x%08x : 0x%08x\n", wifi_dc_addr, regval);
    }

    snprintf_res(cmd_data, "\nBW20 RG_WF_RX_ABBCTUNE is     : %d\n", GET_RG_WF_RX_ABBCTUNE);
    snprintf_res(cmd_data, "BW40 RG_WF_N_RX_ABBCTUNE is   : %d\n", GET_RG_WF_N_RX_ABBCTUNE);

    // TX DC
    snprintf_res(cmd_data, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "Tx DC status\n");
    snprintf_res(cmd_data, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "RG_WF_TX_DAC_IOFFSET is : %d\n", GET_RG_WF_TX_DAC_IOFFSET);
    snprintf_res(cmd_data, "RG_WF_TX_DAC_QOFFSET is : %d\n", GET_RG_WF_TX_DAC_QOFFSET);
    snprintf_res(cmd_data, "RG_BT_TX_DAC_IOFFSET is : %d\n", GET_RG_BT_TX_DAC_IOFFSET);
    snprintf_res(cmd_data, "RG_BT_TX_DAC_QOFFSET is : %d\n", GET_RG_BT_TX_DAC_QOFFSET);
    snprintf_res(cmd_data, "RG_5G_TX_DAC_IOFFSET is : %d\n", GET_RG_5G_TX_DAC_IOFFSET);
    snprintf_res(cmd_data, "RG_5G_TX_DAC_QOFFSET is : %d\n", GET_RG_5G_TX_DAC_QOFFSET);

    // TX IQ
    snprintf_res(cmd_data, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "Tx IQ status\n");
    snprintf_res(cmd_data, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "tx 2500 alpha is : %d, tx 2500 theta is : %d\n", 
            GET_RG_TX_IQ_2500_ALPHA, GET_RG_TX_IQ_2500_THETA);
    snprintf_res(cmd_data, "tx 5100 alpha is : %d, tx 5100 theta is : %d\n",
            GET_RG_TX_IQ_5100_ALPHA, GET_RG_TX_IQ_5100_THETA);
    snprintf_res(cmd_data, "tx 5500 alpha is : %d, tx 5500 theta is : %d\n",
            GET_RG_TX_IQ_5500_ALPHA, GET_RG_TX_IQ_5500_THETA);
    snprintf_res(cmd_data, "tx 5700 alpha is : %d, tx 5700 theta is : %d\n",
            GET_RG_TX_IQ_5700_ALPHA, GET_RG_TX_IQ_5700_THETA);
    snprintf_res(cmd_data, "tx 5900 alpha is : %d, tx 5900 theta is : %d\n",
            GET_RG_TX_IQ_5900_ALPHA, GET_RG_TX_IQ_5900_THETA);
    // RX IQ
    snprintf_res(cmd_data, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "Rx IQ status\n");
    snprintf_res(cmd_data, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "rx 2500 alpha is : %d, rx 2500 theta is : %d\n",
            GET_RG_RX_IQ_2500_ALPHA, GET_RG_RX_IQ_2500_THETA);
    snprintf_res(cmd_data, "rx 5100 alpha is : %d, rx 5100 theta is : %d\n",
            GET_RG_RX_IQ_5100_ALPHA, GET_RG_RX_IQ_5100_THETA);
    snprintf_res(cmd_data, "rx 5500 alpha is : %d, rx 5500 theta is : %d\n",
            GET_RG_RX_IQ_5500_ALPHA, GET_RG_RX_IQ_5500_THETA);
    snprintf_res(cmd_data, "rx 5700 alpha is : %d, rx 5700 theta is : %d\n",
            GET_RG_RX_IQ_5700_ALPHA, GET_RG_RX_IQ_5700_THETA);
    snprintf_res(cmd_data, "rx 5900 alpha is : %d, rx 5900 theta is : %d\n",
            GET_RG_RX_IQ_5900_ALPHA, GET_RG_RX_IQ_5900_THETA);

    // phy status
    snprintf_res(cmd_data, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    snprintf_res(cmd_data, "PHY status\n");
    snprintf_res(cmd_data, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_SYS_REG, &regval);
    value = ((regval & RG_BB_CLK_SEL_MSK) >> RG_BB_CLK_SEL_SFT);
    snprintf_res(cmd_data, "RF/PHY reference clock is from %s\n", (value ? "DPLL(80MHz)" : "Xtal(26MHz)"));
    snprintf_res(cmd_data, "SW channel bandwidth = %sM, channel type=%d\n", 
            (((sh->sc->hw_chan_type == NL80211_CHAN_HT40MINUS)||(sh->sc->hw_chan_type == NL80211_CHAN_HT40PLUS)) ? "40" : "20"),
            sh->sc->hw_chan_type);
    if (((regval & RG_SYSTEM_BW_MSK)>> RG_SYSTEM_BW_SFT)) {
        snprintf_res(cmd_data, "PHY is set to 40MHz bandwidth\n");
        if (((regval & RG_PRIMARY_CH_SIDE_MSK)>>RG_PRIMARY_CH_SIDE_SFT))
            snprintf_res(cmd_data, "PHY primary channel is high at 40M\n");
        else
            snprintf_res(cmd_data, "PHY secondary channel is low at 40M\n");
    } else
        snprintf_res(cmd_data, "PHY is set to 20MHz bandwidth\n");


    snprintf_res(cmd_data, "RO_MRX_EN_CNT 1st read is : %d\n", GET_RO_MRX_EN_CNT);
    MDELAY(100);
    snprintf_res(cmd_data, "RO_MRX_EN_CNT 2nd read is : %d\n", GET_RO_MRX_EN_CNT); 
    snprintf_res(cmd_data, "RO_TX_FIFO_EMPTY_CNT is   : %d\n", GET_RO_TX_FIFO_EMPTY_CNT); 
    snprintf_res(cmd_data, "RO_RX_FIFO_FULL_CNT is    : %d\n", GET_RO_RX_FIFO_FULL_CNT); 
    snprintf_res(cmd_data, "RO_MAC_PHY_TRX_EN_SYNC is : %d\n", GET_RO_MAC_PHY_TRX_EN_SYNC); 
    snprintf_res(cmd_data, "RO_CSTATE_TX is           : %d\n", GET_RO_CSTATE_TX); 
    snprintf_res(cmd_data, "RO_TX_IP is               : %d\n", GET_RO_TX_IP); 
    snprintf_res(cmd_data, "RO_CSTATE_RX is           : %d\n", GET_RO_CSTATE_RX); 
    snprintf_res(cmd_data, "RO_AGC_START_80M is       : %d\n", GET_RO_AGC_START_80M); 
    snprintf_res(cmd_data, "RO_CSTATE_AGC is          : %d\n", GET_RO_CSTATE_AGC); 
    snprintf_res(cmd_data, "RO_MRX_RX_EN is           : %d\n", GET_RO_MRX_RX_EN); 
    snprintf_res(cmd_data, "RO_CSTATE_PKT is          : %d\n", GET_RO_CSTATE_PKT); 
}

static void ssv6006c_cmd_hwinfo_phy_txband(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;

    snprintf_res(cmd_data, "RG_5G_TX_BAND_F0 is   : %d\n", GET_RG_5G_TX_BAND_F0);
    snprintf_res(cmd_data, "RG_5G_TX_BAND_F1 is   : %d\n", GET_RG_5G_TX_BAND_F1);
    snprintf_res(cmd_data, "RG_5G_TX_BAND_F2 is   : %d\n", GET_RG_5G_TX_BAND_F2);
}

static void ssv6006c_cmd_hwinfo_phy_txgain(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 regval;

    regval = GET_RG_TXGAIN_PHYCTRL;
    snprintf_res(cmd_data, "TX_GAIN is controlled by[%d]        : %s\n", regval, ((regval==1) ? "PHY" : "RF"));
    snprintf_res(cmd_data, "2G RG_TX_GAIN is                    : 0x%08x\n\n", GET_RG_TX_GAIN);

    SMAC_REG_READ(sh, ADR_5G_TX_GAIN_PAFB_CONTROL, &regval);
    snprintf_res(cmd_data, "5G RG_5G_TX_GAIN_F0 (<5100)is       : 0x%x\n", 
            ((regval & RG_5G_TX_GAIN_F0_MSK) >> RG_5G_TX_GAIN_F0_SFT));
    snprintf_res(cmd_data, "5G RG_5G_TX_PAFB_EN_F0 is           : %d\n", 
            ((regval & RG_5G_TX_PAFB_EN_F0_MSK) >> RG_5G_TX_PAFB_EN_F0_SFT));

    snprintf_res(cmd_data, "5G RG_5G_TX_GAIN_F1 (5100~5495) is  : 0x%x\n",
            ((regval & RG_5G_TX_GAIN_F1_MSK) >> RG_5G_TX_GAIN_F1_SFT));
    snprintf_res(cmd_data, "5G RG_5G_TX_PAFB_EN_F1 is           : %d\n",
            ((regval & RG_5G_TX_PAFB_EN_F1_MSK) >> RG_5G_TX_PAFB_EN_F1_SFT));

    snprintf_res(cmd_data, "5G RG_5G_TX_GAIN_F2 (5500~5695) is  : 0x%x\n",
            ((regval & RG_5G_TX_GAIN_F2_MSK) >> RG_5G_TX_GAIN_F2_SFT));
    snprintf_res(cmd_data, "5G RG_5G_TX_PAFB_EN_F2 is           : %d\n",
            ((regval & RG_5G_TX_PAFB_EN_F2_MSK) >> RG_5G_TX_PAFB_EN_F2_SFT));

    snprintf_res(cmd_data, "5G RG_5G_TX_GAIN_F3 (>=5700) is     : 0x%x\n",
            ((regval & RG_5G_TX_GAIN_F3_MSK) >> RG_5G_TX_GAIN_F3_SFT));
    snprintf_res(cmd_data, "5G RG_5G_TX_PAFB_EN_F3 is           : %d\n\n",
            ((regval & RG_5G_TX_PAFB_EN_F3_MSK) >> RG_5G_TX_PAFB_EN_F3_SFT));

    SMAC_REG_READ(sh, ADR_5G_TX_PGA_CAPSW_CONTROL_I, &regval); 
    snprintf_res(cmd_data, "Tx PA bias ADR_5G_TX_PGA_CAPSW_CONTROL_I settings is  : 0x%x\n", regval);
    SMAC_REG_READ(sh, ADR_5G_TX_PGA_CAPSW_CONTROL_II, &regval);
    snprintf_res(cmd_data, "Tx PA bias ADR_5G_TX_PGA_CAPSW_Control_II settings is : 0x%x\n\n", regval);

    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_BB_SCALE_REG_0, &regval);
    snprintf_res(cmd_data, "RG_BB_SCALE_MAN_EN is           : %d\n",
            ((regval & RG_BB_SCALE_MAN_EN_MSK) >> RG_BB_SCALE_MAN_EN_SFT));
    snprintf_res(cmd_data, "RG_BB_SCALE_BARKER_CCK is       : 0x%x\n", 
            ((regval & RG_BB_SCALE_BARKER_CCK_MSK) >> RG_BB_SCALE_BARKER_CCK_SFT));
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_BB_SCALE_REG_1, &regval);
    snprintf_res(cmd_data, "RG_BB_SCALE_LEGACY is           : 0x%x\n", regval);
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_BB_SCALE_REG_2, &regval);
    snprintf_res(cmd_data, "RG_BB_SCALE_HT20 is             : 0x%x\n", regval);
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_BB_SCALE_REG_3, &regval);
    snprintf_res(cmd_data, "RG_BB_SCALE_HT40 is             : 0x%x\n\n", regval);

    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_RF_PWR_REG_0, &regval);
    snprintf_res(cmd_data, "RG_RF_PWR_MAN_EN is             : %d\n",
            ((regval & RG_RF_PWR_MAN_EN_MSK) >> RG_RF_PWR_MAN_EN_SFT));
    snprintf_res(cmd_data, "RG_RF_PWR_BARKER_CCK is         : 0x%x\n", 
            ((regval & RG_RF_PWR_BARKER_CCK_MSK) >> RG_RF_PWR_BARKER_CCK_SFT));
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_RF_PWR_REG_1, &regval);
    snprintf_res(cmd_data, "RG_RF_PWR_LEGACY is             : 0x%x\n", regval);
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_RF_PWR_REG_2, &regval);
    snprintf_res(cmd_data, "RG_RF_PWR_HT20 is               : 0x%x\n", regval);
    SMAC_REG_READ(sh, ADR_WIFI_PHY_COMMON_RF_PWR_REG_3, &regval);
    snprintf_res(cmd_data, "RG_RF_PWR_HT40 is               : 0x%x\n\n", regval);

    SMAC_REG_READ(sh, ADR_WIFI_PADPD_5G_BB_GAIN_REG, &regval);
    snprintf_res(cmd_data, "RG_DPD_BB_SCALE_5100 is : 0x%x\n", 
            ((regval & RG_DPD_BB_SCALE_5100_MSK) >> RG_DPD_BB_SCALE_5100_SFT));
    snprintf_res(cmd_data, "RG_DPD_BB_SCALE_5500 is : 0x%x\n",
            ((regval & RG_DPD_BB_SCALE_5500_MSK) >> RG_DPD_BB_SCALE_5500_SFT));
    snprintf_res(cmd_data, "RG_DPD_BB_SCALE_5700 is : 0x%x\n",
            ((regval & RG_DPD_BB_SCALE_5700_MSK) >> RG_DPD_BB_SCALE_5700_SFT));
    snprintf_res(cmd_data, "RG_DPD_BB_SCALE_5900 is : 0x%x\n",
            ((regval & RG_DPD_BB_SCALE_5900_MSK) >> RG_DPD_BB_SCALE_5900_SFT));
    SMAC_REG_READ(sh, ADR_WIFI_PADPD_2G_BB_GAIN_REG, &regval);
    snprintf_res(cmd_data, "RG_DPD_BB_SCALE_2500 is : 0x%x\n\n",
            ((regval & RG_DPD_BB_SCALE_2500_MSK) >> RG_DPD_BB_SCALE_2500_SFT));

    regval = GET_RG_DPD_AM_EN;
    snprintf_res(cmd_data, "Tx PADPD AM is turned[%d] %s\n", regval, ((regval) ? "ON" : "OFF"));
    regval = GET_RG_DPD_PM_EN;
    snprintf_res(cmd_data, "Tx PADPD PM is turned[%d] %s\n", regval, ((regval) ? "ON" : "OFF"));
    regval = GET_RG_DPD_PM_AMSEL;
    snprintf_res(cmd_data, "Tx PADPD PM table is controlled by %s[%d] signal\n", ((regval) ? "DPD" : "orignal"), regval);
}

static void ssv6006c_cmd_fw_left_letter(struct ssv_hw *sh)
{
#define PMU_RAM_09 ADR_PMU_RAM_09
#define PMU_RAM_10 ADR_PMU_RAM_10

    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 log_buf=0, log_size=0;
    u32 s=0, value=0;
    u8 temp[5]={0};
    u32 dbg_msg = 0, fw_task_tracker = 0, msg_evt_tracker = 0;

    // reserved 32byte to fw 
    // byte 0: mac task track
    // byte 1: mac task wait event count
    // byte 2: cmd task track
    // byte 3: cmd task wait event count
    // byte 4 ~ 7: msg event queue address
    SMAC_REG_READ(sh, PMU_RAM_09, &dbg_msg);

    SMAC_REG_READ(sh, dbg_msg, &fw_task_tracker);
    snprintf_res(cmd_data, "mac track:0x%x, mac wait event:%d\n", 
            (fw_task_tracker & 0x000000ff), ((fw_task_tracker & 0x0000ff00) >> 8));
    snprintf_res(cmd_data, "cmd track:0x%x, cmd wait event:%d\n", 
            ((fw_task_tracker & 0x00ff0000) >> 16), ((fw_task_tracker & 0xff000000) >> 24));
    
    SMAC_REG_READ(sh, dbg_msg+4, &msg_evt_tracker);
    snprintf_res(cmd_data, "msg evt address 0x%08x\n", msg_evt_tracker); 


    log_buf = dbg_msg+32;
    SMAC_REG_READ(sh, PMU_RAM_10, &log_size);

    snprintf_res(cmd_data, "log_buf:0x%x,log_size:%d\n", log_buf, log_size);

    for(s=0;s<log_size;s+=4,log_buf+=sizeof(u32))
    {
        memset(temp,0,sizeof(temp));
        SMAC_REG_READ(sh, log_buf, &value);
        memcpy(temp,&value,4);
        snprintf_res(cmd_data, "%s",(char *)temp);
    }
    return ;
}

static void ssv6006c_cmd_fw_pc(struct ssv_hw *sh)
{

    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 value=0;
    SMAC_REG_READ(sh, ADR_N10_DBG1, &value);

    snprintf_res(cmd_data, "fw pc:0x%x\n", value);

    return ;
}
#endif

static void ssv6006c_cmd_hwinfo(struct ssv_hw *sh, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    /**
     *  hwmsg [mac|phy]
     */
    if (argc < 2) {
        snprintf_res(cmd_data, "hwmsg [mac|phy|fw]\n\n");
        return;
    }

    if (!strcmp(argv[1], "mac")) {
        if ((argc == 3) && (!strcmp(argv[2], "peer"))) {
            ssv6006c_cmd_hwinfo_mac_peer_stat(sh); 
        } else if ((argc == 3) && (!strcmp(argv[2], "stat"))) {
            ssv6006c_cmd_hwinfo_mac_stat(sh); 
        } else if ((argc == 3) && (!strcmp(argv[2], "scrt"))) {
            ssv6006c_cmd_hwinfo_mac_scrt_stat(sh); 
        } else if ((argc == 3) && (!strcmp(argv[2], "beacon"))) {
            ssv6006c_cmd_hwinfo_mac_beacon(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "txq"))) {
            ssv6006c_cmd_hwinfo_mac_txq(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "edca"))) {
            ssv6006c_cmd_hwinfo_mac_edca(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "tx"))) {
            ssv6006c_cmd_hwinfo_mac_txstat(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "tx2phy"))) {
            ssv6006c_cmd_hwinfo_mac_tx2phy(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "tx2ptc"))) {
            ssv6006c_cmd_hwinfo_mac_tx2ptc(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "fixrate"))) {
            ssv6006c_cmd_hwinfo_mac_fixrate(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "txfsm"))) {
            ssv6006c_cmd_hwinfo_mac_txfsm(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "txifs"))) {
            ssv6006c_cmd_hwinfo_mac_txifs(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "txbase"))) {
            ssv6006c_cmd_hwinfo_mac_txbase(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "hci"))) {
            ssv6006c_cmd_hwinfo_mac_hci_stat(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "pgid"))) {
            ssv6006c_cmd_hwinfo_page_id(sh);
        }
        else {
            snprintf_res(cmd_data, "hwmsg mac [peer|stat|scrt|beacon|txq|edca|tx|tx2phy|tx2ptc|fixrate|txfsm|txifs|txbase|hci|pgid]\n\n");
        }
    } else if (!strcmp(argv[1], "phy")) {
        if ((argc == 3) && (!strcmp(argv[2], "pmu"))) {
            ssv6006c_cmd_hwinfo_phy_pmu(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "xtal"))) {
            ssv6006c_cmd_hwinfo_phy_xtal(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "sx"))) {
            ssv6006c_cmd_hwinfo_phy_sx(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "cali"))) {
            ssv6006c_cmd_hwinfo_phy_cali(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "txband"))) {
            ssv6006c_cmd_hwinfo_phy_txband(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "txgain"))) {
            ssv6006c_cmd_hwinfo_phy_txgain(sh);
        } else {
            snprintf_res(cmd_data, "hwmsg phy [pmu|xtal|sx|cali|txband|txgain]\n\n");
        }
    }else if (!strcmp(argv[1], "fw")){
        if ((argc == 3) && (!strcmp(argv[2], "pc"))) {
            ssv6006c_cmd_fw_pc(sh);
        } else if ((argc == 3) && (!strcmp(argv[2], "left_letter"))) {
            ssv6006c_cmd_fw_left_letter(sh);
    } else {
            snprintf_res(cmd_data, "hwmsg fw [pc|left_letter]\n\n");
        }

    }else {
        snprintf_res(cmd_data, "hwmsg [mac|phy][fw]\n\n");
    } 
#endif
    return;
}

static void ssv6006c_get_rd_id_adr(u32 *id_base_address)
{
    id_base_address[0] = ADR_RD_ID0;
    id_base_address[1] = ADR_RD_ID1;
    id_base_address[2] = ADR_RD_ID2;
    id_base_address[3] = ADR_RD_ID3;
}

static int ssv6006c_burst_read_reg(struct ssv_hw *sh, u32 *addr, u32 *buf, u8 reg_amount)
{
    int ret = (-1), i;
    u32 dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);
    if (dev_type == SSV_HWIF_INTERFACE_SDIO) {
        ret = SMAC_BURST_REG_READ(sh, addr, buf, reg_amount);
    } else {
        for (i = 0 ; i < reg_amount ; i++) {
            ret = SMAC_REG_READ(sh, addr[i], &buf[i]);
            if (ret != 0) {
                printk("%s(): read 0x%08x failed.\n", __func__, addr[i]);
                break;
            }
        }
    }

    return ret;
}

static int ssv6006c_burst_write_reg(struct ssv_hw *sh, u32 *addr, u32 *buf, u8 reg_amount)
{
    int ret = (-1), i;
    u32 dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);
    if (dev_type == SSV_HWIF_INTERFACE_SDIO) {
        ret = SMAC_BURST_REG_WRITE(sh, addr, buf, reg_amount);
    } else {
        for (i = 0 ; i < reg_amount ; i++) {
            ret = SMAC_REG_WRITE(sh, addr[i], buf[i]);
            if (ret != 0) {
                printk("%s(): write 0x%08x failed.\n", __func__, addr[i]);
                break;
            }
        }
    }

    return ret;
}

static int ssv6006c_auto_gen_nullpkt(struct ssv_hw *sh, int hwq)
{
    switch (hwq) {
        case 0:
            SET_TXQ0_Q_NULLDATAFRAME_GEN_EN(1);
            MDELAY(100);
            SET_TXQ0_Q_NULLDATAFRAME_GEN_EN(0);
            break;
        case 1:
            SET_TXQ1_Q_NULLDATAFRAME_GEN_EN(1);
            MDELAY(100);
            SET_TXQ1_Q_NULLDATAFRAME_GEN_EN(0);
            break;
        case 2:
            SET_TXQ2_Q_NULLDATAFRAME_GEN_EN(1);
            MDELAY(100);
            SET_TXQ2_Q_NULLDATAFRAME_GEN_EN(0);
            break;
        case 3:
            SET_TXQ3_Q_NULLDATAFRAME_GEN_EN(1);
            MDELAY(100);
            SET_TXQ3_Q_NULLDATAFRAME_GEN_EN(0);
            break;
        case 4:
            SET_TXQ4_Q_NULLDATAFRAME_GEN_EN(1);
            MDELAY(100);
            SET_TXQ4_Q_NULLDATAFRAME_GEN_EN(0);
            break;
        case 5:
            SET_TXQ5_Q_NULLDATAFRAME_GEN_EN(1);
            MDELAY(100);
            SET_TXQ5_Q_NULLDATAFRAME_GEN_EN(0);
            break;
        default:
            return -EOPNOTSUPP;
    }

    return 0;
}

static void ssv6006c_load_fw_enable_mcu(struct ssv_hw *sh)
{
    // After FW loaded, set IVB to 0, boot from SRAM, enable N10 clock, and release N10
    SET_N10CFG_DEFAULT_IVB(0);

    SET_CLK_EN_CPUN10(1);
    SET_RESET_N_CPUN10(1); // N10 might be disabled by default. Enable it. 
}

static int ssv6006c_load_fw_disable_mcu(struct ssv_hw *sh)
{
    int ret = 0;
    // Before loading FW, reset N10
    if (ssv6006c_reset_cpu(sh) != 0)
        return -1;

    SET_CLK_EN_CPUN10(0);

    SET_MCU_ENABLE(0);
    SET_RG_REBOOT(1);

    return ret;
}

static int ssv6006c_load_fw_set_status(struct ssv_hw *sh, u32 status)
{
    return SMAC_REG_WRITE(sh, ADR_TX_SEG, status);
}

static int ssv6006c_load_fw_get_status(struct ssv_hw *sh, u32 *status)
{
    return SMAC_REG_READ(sh, ADR_TX_SEG, status);
}

static void ssv6006c_load_fw_pre_config_device(struct ssv_hw *sh)
{
    HCI_LOAD_FW_PRE_CONFIG_DEVICE(sh->hci.hci_ctrl);
}

static void ssv6006c_load_fw_post_config_device(struct ssv_hw *sh)
{
    HCI_LOAD_FW_POST_CONFIG_DEVICE(sh->hci.hci_ctrl);
}

// Reset CPU (after reset, CPU is stopped)
static int ssv6006c_reset_cpu(struct ssv_hw *sh)
{    
    // Keep original interrupt mask
    u32 org_int_mask = GET_MASK_TYPMCU_INT_MAP;
    u32 cnt = 0;


    /* Safly reset CPU: (Precondition: CPU must be alive)
     * Through sysctrl to make CPU enter standby first, then do CPU reset. 
     */
    if (GET_RESET_N_CPUN10) {
        // Mask all interrupt for CPU, except SYSCTRL interrupt
        SET_MASK_TYPMCU_INT_MAP(0xffffdfff);        
        // Request CPU enter standby through SYSCTRL COMMAND
        SET_SYSCTRL_CMD(0x0000000e);

        // Confirm if N10 enter standby
        while(!GET_N10_STANDBY) {
            cnt++;
            // 1 ms checking time limit
            if (cnt > 10) {
                printk("Reset CPU failed! CPU can't enter standby\n");
                return -1;
            }
        }
    }

    // Reset CPU
    SET_RESET_N_CPUN10(0);
    // Set original interrupt mask back
    SET_MASK_TYPMCU_INT_MAP(org_int_mask);

    return 0;
}

/* Set SRAM mapping mode to:
 * 0: ILM  64KB, DLM 128KB 
 * 1: ILM 160KB, DLM 32KB
 */
static void ssv6006c_set_sram_mode(struct ssv_hw *sh, enum SSV_SRAM_MODE mode)
{
    //TODO: wait csr to replace raw value of register 
    switch (mode) {
        case SRAM_MODE_ILM_64K_DLM_128K:
            SMAC_REG_SET_BITS(sh, ADR_SRAMCFG_SETTING, (0<<1), 0x2);            
            break;
        case SRAM_MODE_ILM_160K_DLM_32K:
            SMAC_REG_SET_BITS(sh, ADR_SRAMCFG_SETTING, (1<<1), 0x2);
            break;
    }
}

static int ssv6006c_get_sram_mode(struct ssv_hw *sh)
{
    return GET_ILM160KB_EN;
}

static void ssv6006c_enable_usb_acc(struct ssv_softc *sc, u8 epnum)
{
    switch (epnum) {
        case SSV_EP_CMD:
            dev_info(sc->dev, "Enable Command(ep%d) acc\n", SSV_EP_CMD);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (1<<0), 0x1);
            break;
        case SSV_EP_RSP:
            dev_info(sc->dev, "Enable Command Rsp(ep%d) acc\n", SSV_EP_RSP);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (1<<1), 0x2);
            break;
        case SSV_EP_TX:
            dev_info(sc->dev, "Enable TX(ep%d) acc\n", SSV_EP_TX);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (1<<2), 0x4);
            break;
        case SSV_EP_RX:
            dev_info(sc->dev, "Enable RX(ep%d) acc\n", SSV_EP_RX);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (1<<3), 0x8);
            break;
    }
}

static void ssv6006c_disable_usb_acc(struct ssv_softc *sc, u8 epnum)
{   
    switch (epnum) {
        case SSV_EP_CMD:
            dev_info(sc->dev, "Disable Command(ep%d) acc\n", SSV_EP_CMD);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (0<<0), 0x1);
            break;
        case SSV_EP_RSP:
            dev_info(sc->dev, "Disable Command Rsp(ep%d) acc\n", SSV_EP_RSP);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (0<<1), 0x2);
            break;
        case SSV_EP_TX:
            dev_info(sc->dev, "Disable TX(ep%d) acc\n", SSV_EP_TX);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (0<<2), 0x4);
            break;
        case SSV_EP_RX:
            dev_info(sc->dev, "Disable RX(ep%d) acc\n", SSV_EP_RX);
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_ACC_CTRL_REG_0, (0<<3), 0x8);
            break;
    }
}

// Set Hardware support USB Link Power Management (LPM)
static void ssv6006c_set_usb_lpm(struct ssv_softc *sc, u8 enable)
{
    int dev_type = HCI_DEVICE_TYPE(sc->sh->hci.hci_ctrl);
    int i = 0;

    if (dev_type != SSV_HWIF_INTERFACE_USB) {
        printk("Not support set USB LPM for this model!!\n");
        return;
    }

    for(i=0 ; i<SSV6200_MAX_VIF ;i++) {
        if (sc->vif_info[i].vif == NULL)
            continue;

        if (sc->vif_info[i].vif->type == NL80211_IFTYPE_AP) {
            printk("Force to disable USB LPM function due to exist AP interface\n");
            //TODO: don't use magic number, wait USB register address define to replace it
            SMAC_REG_SET_BITS(sc->sh, ADR_USB_AND_DEVICE_CONTROL_REG, (0 << 11), 0x800);
            return;
        }
    }        

    printk("Set USB LPM support to %d\n", enable);
    //TODO: don't use magic number, wait USB register address define to replace it
    SMAC_REG_SET_BITS(sc->sh, ADR_USB_AND_DEVICE_CONTROL_REG, (enable << 11), 0x800);
}

static int ssv6006c_jump_to_rom(struct ssv_softc *sc)
{
    struct ssv_hw *sh = sc->sh;
    int ret = 0;

    dev_info(sc->dev, "Jump to ROM\n");
    if (ssv6006c_reset_cpu(sh) != 0)
        return -1;

    SET_N10CFG_DEFAULT_IVB(0x4); //set IVB to ROM code address: 0x40000
    SET_RESET_N_CPUN10(1); //release reset to let CPU run

    msleep(500); //Delay to avoid ROM code is not ready.

    return ret;
}

static void ssv6006c_init_gpio_cfg(struct ssv_hw *sh)
{
    u32 regval;
    /* GPIO 15 will cause the RF interfence. */
    regval = GET_MANUAL_IO;
    regval |= 0x8000;
    SET_MANUAL_IO(regval);
}

static void ssv6006c_cmd_efuse(struct ssv_hw *sh, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_DBGMSG
    struct ssv_softc *sc = sh->sc;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    u8 efuse_mapping_table[EFUSE_HWSET_MAX_SIZE/8];
    u8 mac[ETH_ALEN], oldmac[ETH_ALEN], newmac[ETH_ALEN];
    int i = 0;
    int j = 0;
    char *endp;
    u8 value8, value;
    u32 value32 = 0, category_idx = 0;
    int shift = 0; 
    u8 efuse_real_content_len = 0;
    char *efuse_map_name[] = {"none", "cali", "sar", "oldmac", "freq", "txpower1", "txpower2",
        "chipid", "none1", "vid", "pid", "mac", "ratetbl1", "ratetbl2"};
    struct efuse_map ssv_efuse_item_table[] = {
        {4, 0, 0}, // none use 
        {4, 8, 0}, // cali
        {4, 8, 0}, // sar
        {4, 48, 0},// Mac address old
        {4, 8, 0}, // xtal
        {4, 8, 0}, // txpower1
        {4, 8, 0}, // txpower2
        {4, 4, 0}, // chip id
        {4, 0, 0}, // none use
        {4, 16, 0},// vid
        {4, 16, 0},// pid
        {4, 48, 0},// mac address new
        {4,  8, 0},// rate table1
        {4,  8, 0},// rate table2
    };

    /* efuse [r|w] [category] [value] */
    if ((argc == 4) && (strcmp(argv[1], "w") == 0)) {
        memset(mac, 0x00, ETH_ALEN);
        memset(oldmac, 0x00, ETH_ALEN);
        memset(newmac, 0x00, ETH_ALEN);
        memset(efuse_mapping_table, 0x00, EFUSE_HWSET_MAX_SIZE/8);

        // parse input value 
        for (i = EFUSE_R_CALIBRATION_RESULT; i <= EFUSE_RATE_TABLE_2; i++) {
            if (!strcmp(argv[2], efuse_map_name[i])) {
                if ((i == NO_USE) || (i == EFUSE_MAC))
                    break;

                if (i == EFUSE_MAC_NEW) {
                    memset(mac, 0x00, ETH_ALEN);
                    if (6 == sscanf(argv[3], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
                                &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
                        if (!is_valid_ether_addr(mac)) {
                            snprintf_res(cmd_data, "Invalid mac address\n");
                            return;
                        }
                    } else {
                        snprintf_res(cmd_data, "Invalid mac address\n");
                        return;
                    }
                    break; 
                } else {
                    value32 = simple_strtoul(argv[3], &endp, 16);
                    if (ssv_efuse_item_table[i].byte_cnts == 4)
                        value32 = (value32 & 0xf);
                    else if (ssv_efuse_item_table[i].byte_cnts == 8)
                        value32 = (value32 & 0xff);
                    else if (ssv_efuse_item_table[i].byte_cnts == 16)
                        value32 = (value32 & 0xffff);
                    else {
                        snprintf_res(cmd_data, "Invalid value\n");
                        return;
                    }
                    break;
                }
            }
        }

        if ((i == EFUSE_R_CALIBRATION_RESULT) || (i == EFUSE_SAR_RESULT) || 
                (i == EFUSE_MAC) || (i == EFUSE_CHIP_ID) ||
                (i == NO_USE) || (i == EFUSE_VID) ||
                (i == EFUSE_PID) || (i > EFUSE_RATE_TABLE_2)) {
            snprintf_res(cmd_data, "efuse w [freq|txpower1|txpower2|mac|ratetbl1|ratetbl2] value\n");
            return;
        }

        // save index
        category_idx = i;
        // parse efuse value
        SSV_READ_EFUSE(sh, efuse_mapping_table);
        efuse_real_content_len = parser_efuse(sh, efuse_mapping_table, oldmac, newmac, ssv_efuse_item_table);
        // update efuse mapping table value
        ssv_efuse_item_table[category_idx].value = value32;

        // add index
        shift = efuse_real_content_len % 8;
        if (shift) {
            value8 = efuse_mapping_table[efuse_real_content_len>>3];
            value8 &= 0x0f;
            efuse_mapping_table[efuse_real_content_len>>3] = ((category_idx << 4) | value8);
            efuse_real_content_len += ssv_efuse_item_table[category_idx].offset;
        } else {
            efuse_mapping_table[efuse_real_content_len>>3] = (category_idx & 0x0f);
            efuse_real_content_len += ssv_efuse_item_table[category_idx].offset;
        }

        // add value
        for (j = 0; j < ssv_efuse_item_table[category_idx].byte_cnts; j=j+4) {
            if (category_idx == EFUSE_MAC_NEW) {
                if (0 == (j % 8))
                    value = mac[j>>3] & 0xff;
                else
                    value = (mac[j>>3] & 0xf0) >> 4;

            } else
                value = ((ssv_efuse_item_table[category_idx].value >> j) & 0xff);

            shift = efuse_real_content_len % 8;
            if (shift) {
                value8 = efuse_mapping_table[efuse_real_content_len>>3];
                value8 &= 0x0f;
                efuse_mapping_table[efuse_real_content_len>>3] = ((value << 4) | value8);
                efuse_real_content_len += 4;
            } else {
                efuse_mapping_table[efuse_real_content_len>>3] = (value & 0x0f);
                efuse_real_content_len += 4;
            }
        }

        if (efuse_real_content_len > EFUSE_HWSET_MAX_SIZE) {
            snprintf_res(cmd_data, "No enough efuse space\n"); 
            return; 
        }

        // write value to efuse
        SSV_WRITE_EFUSE(sh, efuse_mapping_table, efuse_real_content_len); 

        // read value from efuse
        memset(oldmac, 0x00, ETH_ALEN);
        memset(newmac, 0x00, ETH_ALEN);
        memset(efuse_mapping_table, 0x00, EFUSE_HWSET_MAX_SIZE/8);

        SSV_READ_EFUSE(sh, efuse_mapping_table);
        //Parser data. 
        parser_efuse(sh, efuse_mapping_table, oldmac, newmac, ssv_efuse_item_table);
        if (category_idx == EFUSE_MAC_NEW) {
            snprintf_res(cmd_data, "Efuse mac[%d] = %02x:%02x:%02x:%02x:%02x:%02x\n", 
                    EFUSE_MAC_NEW, newmac[0], newmac[1], newmac[2], newmac[3], newmac[4], newmac[5]);
        } else {
            snprintf_res(cmd_data, "Efuse %s[%d] = 0x%02x\n", 
                    efuse_map_name[category_idx], category_idx, ssv_efuse_item_table[category_idx].value);
        }

        return;

    } else if ((argc == 3) && (strcmp(argv[1], "r") == 0)) {
        memset(oldmac, 0x00, ETH_ALEN);
        memset(newmac, 0x00, ETH_ALEN);
        memset(efuse_mapping_table, 0x00, EFUSE_HWSET_MAX_SIZE/8);

        SSV_READ_EFUSE(sh, efuse_mapping_table);
        //Parser data. 
        parser_efuse(sh, efuse_mapping_table, oldmac, newmac, ssv_efuse_item_table);

        for (i = EFUSE_R_CALIBRATION_RESULT; i <= EFUSE_RATE_TABLE_2; i++) {
            if (!strcmp(argv[2], efuse_map_name[i])) {
                if ((i == NO_USE) || (i == EFUSE_MAC))
                    break;

                if (i == EFUSE_MAC_NEW) {
                    if (!is_valid_ether_addr(newmac)) 
                        snprintf_res(cmd_data, "Efuse cannot read %s value\n", argv[2]);
                    else {
                        snprintf_res(cmd_data, "Efuse mac[%d] = %02x:%02x:%02x:%02x:%02x:%02x\n", 
                                EFUSE_MAC_NEW, newmac[0], newmac[1], newmac[2], newmac[3], newmac[4], newmac[5]);
                    }
                    break;
                } else {
                    if (!(sh->efuse_bitmap & BIT(i)))
                        snprintf_res(cmd_data, "Efuse cannot read %s value\n", argv[2]); 
                    else {
                        snprintf_res(cmd_data, "Efuse %s[%d] = 0x%02x\n", 
                                argv[2], i, ssv_efuse_item_table[i].value);
                    }
                    break;
                }
            }
        }

        if ((i == EFUSE_R_CALIBRATION_RESULT) || (i == EFUSE_SAR_RESULT) || 
                (i == EFUSE_MAC) || (i == EFUSE_CHIP_ID) ||
                (i == NO_USE) || (i == EFUSE_VID) ||
                (i == EFUSE_PID) || (i > EFUSE_RATE_TABLE_2)) {
            snprintf_res(cmd_data, "efuse r [freq|txpower1|txpower2|mac|ratetbl1|ratetbl2] value\n");
        }

        return;

    } else if ((argc == 2) && (strcmp(argv[1], "checkfree") == 0)) {
        memset(oldmac, 0x00, ETH_ALEN);
        memset(newmac, 0x00, ETH_ALEN);
        memset(efuse_mapping_table, 0x00, EFUSE_HWSET_MAX_SIZE/8);

        SSV_READ_EFUSE(sh, efuse_mapping_table);
        //Parser data. 
        efuse_real_content_len = parser_efuse(sh, efuse_mapping_table, oldmac, newmac, ssv_efuse_item_table);
        // show result
        // snprintf_res(cmd_data,"max size = %d, real content len = %d\n", EFUSE_HWSET_MAX_SIZE, efuse_real_content_len);
        snprintf_res(cmd_data,"free bits = %d\n", (EFUSE_HWSET_MAX_SIZE-efuse_real_content_len));
        return;
    }
    else {
        snprintf_res(cmd_data, "efuse [r|w|checkfree] [category] [value]\n\n");
    }
#endif
}

void ssv6006c_set_on3_enable(struct ssv_hw *sh, bool val)
{
    u32 regval=0;
    //0x303:
    //bit[0]: oscillator ready
    //bit[1]: DPLL ready
    //bit[8~9]: PMU in chip on status
    do{
        if(-1 == SMAC_REG_READ(sh, ADR_SYSCTRL_STATUS, &regval))
        {
            goto EXIT;
        }
    }while(regval!=0x303);

    if(-1 == SMAC_REG_WRITE(sh, ADR_POWER_ON_OFF_CTRL, 0x7334))
    {
        goto EXIT;
    }
    if (val)
    {
        if(-1 == SMAC_REG_WRITE(sh, ADR_SYSCTRL_COMMAND, 0x80c))
        {
            goto EXIT;
        }
    }
    else
    {
        if(-1 ==SMAC_REG_WRITE(sh, ADR_SYSCTRL_COMMAND, 0x40c))
        {
            goto EXIT;
        }
    }

    do{
        if(-1 == SMAC_REG_READ(sh, ADR_SYSCTRL_STATUS, &regval))
        {
            goto EXIT;
        }
    }while(regval!=0x303);

EXIT:
    return;
}

static int ssv6006c_init_hci_rx_aggr(struct ssv_hw *sh)
{
    u32 regval = 0;
    int i = 0;

    if (sh->cfg.hw_caps & SSV6200_HW_CAP_HCI_TX_AGGR) {
        printk(KERN_ERR "Enable HCI TX aggregation\n");
        //SMAC_REG_WRITE(sh,0xc1000004, 0x40000003);
        SMAC_REG_SET_BITS(sh, ADR_HCI_TRX_MODE, (1<<HCI_TX_AGG_EN_SFT), HCI_TX_AGG_EN_MSK);
    }

    /* 
     * HCI RX Aggregation
     * FLOW CONTROL
     * 1. STOP HCI RX
     * 2. CONFIRM RX STATUS
     * 3. ENABLE HCI RX AGGREGATION
     * 4. SET AGGREGATION LENGTH[LENGTH=AGGREGATION+MPDU]
     * 5. START HCI RX
     */
    if (sh->cfg.hw_caps & SSV6200_HW_CAP_HCI_RX_AGGR) {
        SMAC_REG_SET_BITS(sh, ADR_HCI_TRX_MODE, (0<<HCI_RX_EN_SFT), HCI_RX_EN_MSK);
        do {
            SMAC_REG_READ(sh, ADR_RX_PACKET_LENGTH_STATUS, &regval);
            regval &= HCI_RX_LEN_I_MSK;
            i++;
            if (i > 10000) {
                printk("CANNOT ENABLE HCI RX AGGREGATION!!!\n");
                WARN_ON(1);
                return -1;
            }
        } while (regval != 0);


        regval = 0;
        SMAC_REG_SET_BITS(sh, ADR_HCI_TRX_MODE, (1<<HCI_RX_FORM_1_SFT), HCI_RX_FORM_1_MSK);
        regval = (sh->cfg.hw_rx_agg_cnt << RX_AGG_CNT_SFT) |
            (sh->cfg.hw_rx_agg_method_3 << RX_AGG_METHOD_3_SFT) |
            (sh->cfg.hw_rx_agg_timer_reload << RX_AGG_TIMER_RELOAD_VALUE_SFT);
        SMAC_REG_WRITE(sh, ADR_FORCE_RX_AGGREGATION_MODE, regval);
        SMAC_REG_SET_BITS(sh, ADR_HCI_FORCE_PRE_BULK_IN, HCI_RX_AGGR_SIZE, HCI_BULK_IN_HOST_SIZE_MSK);
        SMAC_REG_SET_BITS(sh, ADR_HCI_TRX_MODE, (1<<HCI_RX_EN_SFT), HCI_RX_EN_MSK);

        sh->rx_mode = (sh->cfg.hw_rx_agg_method_3) ? RX_HW_AGG_MODE_METH3 : RX_HW_AGG_MODE;
    } 

    return 0;
}

static void ssv6006c_init_PLL(struct ssv_hw *sh)
{
    u32 regval , count = 0;

    /* for turismo, it just needs to set register once , pll is initialized by hw auto */
    SET_RG_LOAD_RFTABLE_RDY(0x1);
    do {
        msleep(1);
        regval = REG32_R(ADR_PMU_STATE_REG);
        count++;
        if (regval == 0x13)
            break;
        if (count > 100) {
            printk(" PLL initial fails \r\n");
            WARN_ON(1);
            break;
        }
    } while (1);

    msleep(1);
    /* enable PHY clock */
    REG32_W(ADR_WIFI_PHY_COMMON_SYS_REG, 0x80010000);
    /* do clock switch */
    REG32_W(ADR_CLOCK_SELECTION, 0x00000008);
    msleep(1); /* wait for clock settled */
}

static void ssv6006c_set_crystal_clk(struct ssv_hw *sh)
{
    SET_RG_EN_IOTADC_160M(0);

    switch (sh->cfg.crystal_type) {
        case SSV6XXX_IQK_CFG_XTAL_16M:
            SET_RG_DP_XTAL_FREQ(0);
            SET_RG_SX_XTAL_FREQ(0);
            break;
        case SSV6XXX_IQK_CFG_XTAL_24M:
            SET_RG_DP_XTAL_FREQ(1);
            SET_RG_SX_XTAL_FREQ(1);
            break;
        case SSV6XXX_IQK_CFG_XTAL_26M:
            SET_RG_DP_XTAL_FREQ(2);
            SET_RG_SX_XTAL_FREQ(2);
            break;
        case SSV6XXX_IQK_CFG_XTAL_40M:
            SET_RG_DP_XTAL_FREQ(3);
            SET_RG_SX_XTAL_FREQ(3);
            break;
        case SSV6XXX_IQK_CFG_XTAL_12M:
            SET_RG_DP_XTAL_FREQ(4);
            SET_RG_SX_XTAL_FREQ(4);
            break;
        case SSV6XXX_IQK_CFG_XTAL_20M:
            SET_RG_DP_XTAL_FREQ(5);
            SET_RG_SX_XTAL_FREQ(5);
            break;
        case SSV6XXX_IQK_CFG_XTAL_25M:
            SET_RG_DP_XTAL_FREQ(6);
            SET_RG_SX_XTAL_FREQ(6);
            break;
        case SSV6XXX_IQK_CFG_XTAL_32M:
            SET_RG_DP_XTAL_FREQ(7);
            SET_RG_SX_XTAL_FREQ(7);
            break;
        default:
            printk("Please redefine xtal_clock(wifi.cfg)!!\n");
            WARN_ON(1);
            break;
    }

    ssv6006c_init_PLL(sh);
    if (sh->cfg.clk_src_80m)
        SET_CLK_DIGI_SEL(0x8);
    else
        SET_CLK_DIGI_SEL(0x4);
    udelay(1);  /*  wait for clock set */
}

static int ssv6006c_reset_hw_mac(struct ssv_hw *sh)
{
    int i = 0;
    u32 regval = 0;

    SMAC_REG_WRITE(sh, ADR_BRG_SW_RST, 1 << MAC_SW_RST_SFT);
    do
    {
        SMAC_REG_READ(sh, ADR_BRG_SW_RST, &regval);
        i ++;
        if (i >10000) {
            printk("MAC reset fail !!!!\n");
            WARN_ON(1);
            return -1;    
        }
    } while (regval != 0);

    // set mac clk frequency
    regval = GET_CLK_DIGI_SEL; 
    switch (regval) {
        case 8: // 80M
            SET_MAC_CLK_80M(1);
            SET_PHYTXSTART_NCYCLE(26);
            SET_PRESCALER_US(80);
            break;
        case 4: // 40M
            SET_MAC_CLK_80M(0);
            SET_PHYTXSTART_NCYCLE(13);
            SET_PRESCALER_US(40);
            break;
        default:
            printk("digi clk is invalid for mac!!!!\n");
            WARN_ON(1);
            return -1;
            break;
    }

    return 0;
}

static void ssv6006c_wait_usb_rom_ready(struct ssv_hw *sh)
{
    int i = 0;
    u32 dev_type = HCI_DEVICE_TYPE(sh->hci.hci_ctrl); 

    if (dev_type == SSV_HWIF_INTERFACE_USB) {
        // USB ROM cod wil connect hci to usb after rom ready.
        // wait usb20_host_selrw =1
        while (GET_USB20_HOST_SELRW == 0) {
            i ++;
            if (i > 100) break;
            mdelay(1);
        }
        printk("wait %d ms for usb rom code ready\n", i);
    }
}

static void ssv6006c_detach_usb_hci(struct ssv_hw *sh)
{
    // detach hci from usb when removing driver
    SET_USB20_HOST_SELRW(0);
}

static void ssv6006c_pll_chk(struct ssv_hw *sh)
{
    u32 regval;

    // check clock selection 80M 
    regval = GET_CLK_DIGI_SEL; 
    if (regval != 8) {
        HAL_INIT_PLL(sh); // reset pll 
        SET_MAC_CLK_80M(1);
        SET_PHYTXSTART_NCYCLE(26);
        SET_PRESCALER_US(80);
    }
}

#ifdef CONFIG_STA_BCN_FILTER
void ssv6006c_set_mrx_filter(struct ssv_hw *sh, u8 ID, bool enable, u16 rule_bitmap)
{
    u32 addr = ADR_MRX_FLT_EN0 + (ID * 4);
    u32 val = 0;

    SMAC_REG_READ(sh, addr, &val);
    if(enable) {
        val = (val | rule_bitmap);
    } else {
        val = (val & (~rule_bitmap));
    }

    SMAC_REG_WRITE(sh, addr, val);
}
#endif

void ssv_attach_ssv6006c_mac(struct ssv_hal_ops *hal_ops)
{
    hal_ops->init_mac = ssv6006c_init_mac;
    hal_ops->reset_sysplf = ssv6006c_reset_sysplf;

    hal_ops->init_hw_sec_phy_table = ssv6006c_init_hw_sec_phy_table;
    hal_ops->write_mac_ini = ssv6006c_write_mac_ini;

    hal_ops->set_rx_flow = ssv6006c_set_rx_flow;
    hal_ops->set_rx_ctrl_flow = ssv6006c_set_rx_ctrl_flow;
    hal_ops->set_macaddr = ssv6006c_set_macaddr;
    hal_ops->set_macaddr_2 = ssv6006c_set_macaddr_2;
    hal_ops->set_bssid = ssv6006c_set_bssid;
    hal_ops->get_ic_time_tag = ssv6006c_get_ic_time_tag;
    hal_ops->get_chip_id = ssv6006c_get_chip_id;
    hal_ops->set_mrx_mode = ssv6006c_set_mrx_mode;
    hal_ops->get_mrx_mode = ssv6006c_get_mrx_mode;

    hal_ops->save_hw_status = ssv6006c_save_hw_status;
    hal_ops->restore_hw_config = ssv6006c_restore_hw_config;
    hal_ops->set_hw_wsid = ssv6006c_set_hw_wsid;
    hal_ops->del_hw_wsid = ssv6006c_del_hw_wsid;

    hal_ops->set_aes_tkip_hw_crypto_group_key = ssv6006c_set_aes_tkip_hw_crypto_group_key;
    hal_ops->write_pairwise_keyidx_to_hw = ssv6006c_write_pairwise_keyidx_to_hw;
    hal_ops->write_group_keyidx_to_hw = ssv6006c_write_hw_group_keyidx;
    hal_ops->write_pairwise_key_to_hw = ssv6006c_write_pairwise_key_to_hw;
    hal_ops->write_group_key_to_hw = ssv6006c_write_group_key_to_hw;
    hal_ops->write_key_to_hw = ssv6006c_write_key_to_hw;
    hal_ops->set_pairwise_cipher_type = ssv6006c_set_pairwise_cipher_type; 
    hal_ops->set_group_cipher_type = ssv6006c_set_group_cipher_type; 
    hal_ops->set_wep_key = ssv6006c_set_wep_key;
	hal_ops->set_wep_hw_crypto_key = ssv6006c_set_wep_hw_crypto_key;

#ifdef CONFIG_PM
    hal_ops->save_clear_trap_reason = ssv6006c_save_clear_trap_reason;
    hal_ops->restore_trap_reason = ssv6006c_restore_trap_reason;
    hal_ops->ps_save_reset_rx_flow = ssv6006c_ps_save_reset_rx_flow;
    hal_ops->ps_restore_rx_flow = ssv6006c_ps_restore_rx_flow;
    hal_ops->pmu_awake = ssv6006c_pmu_awake;
    hal_ops->ps_hold_on3 = ssv6006c_ps_hold_on3;
#endif

    hal_ops->set_replay_ignore = ssv6006c_set_replay_ignore;
    hal_ops->update_decision_table_6 = ssv6006c_update_decision_table_6;
    hal_ops->update_decision_table = ssv6006c_update_decision_table;
    hal_ops->get_fw_version = ssv6006c_get_fw_version;
    hal_ops->set_op_mode = ssv6006c_set_op_mode;
    hal_ops->set_dur_burst_sifs_g = ssv6006c_set_dur_burst_sifs_g;
    hal_ops->set_dur_slot = ssv6006c_set_dur_slot;
    hal_ops->set_sifs = ssv6006c_set_sifs;
    hal_ops->set_qos_enable = ssv6006c_set_qos_enable;
    hal_ops->set_wmm_param = ssv6006c_set_wmm_param;
    hal_ops->update_page_id = ssv6006c_update_page_id;

    hal_ops->alloc_pbuf = ssv6006c_alloc_pbuf;
    hal_ops->free_pbuf = ssv6006c_free_pbuf;
    hal_ops->ampdu_auto_crc_en = ssv6006c_ampdu_auto_crc_en;
    hal_ops->set_rx_ba = ssv6006c_set_rx_ba;
    hal_ops->read_efuse = ssv6006c_read_efuse;
    hal_ops->write_efuse = ssv6006c_write_efuse;
    hal_ops->chg_clk_src = ssv6006c_chg_clk_src;

    hal_ops->beacon_loss_enable = ssv6006c_beacon_loss_enable;
    hal_ops->beacon_loss_disable = ssv6006c_beacon_loss_disable;
    hal_ops->beacon_loss_config = ssv6006c_beacon_loss_config;

    hal_ops->update_txq_mask = ssv6006c_update_txq_mask;
    hal_ops->readrg_hci_inq_info = ssv6006c_readrg_hci_inq_info;
    hal_ops->readrg_txq_info = ssv6006c_readrg_txq_info;

    hal_ops->dump_wsid = ssv6006c_dump_wsid;
    hal_ops->dump_decision = ssv6006c_dump_decision;
    hal_ops->get_ffout_cnt = ssv6006c_get_ffout_cnt;
    hal_ops->get_in_ffcnt = ssv6006c_get_in_ffcnt;
    hal_ops->read_ffout_cnt = ssv6006c_read_ffout_cnt;
    hal_ops->read_in_ffcnt = ssv6006c_read_in_ffcnt;
    hal_ops->read_id_len_threshold = ssv6006c_read_id_len_threshold;
    hal_ops->read_allid_map = ssv6006c_read_allid_map;
    hal_ops->read_txid_map = ssv6006c_read_txid_map;
    hal_ops->read_rxid_map = ssv6006c_read_rxid_map;
    hal_ops->read_tag_status = ssv6006c_read_tag_status;
    hal_ops->cmd_mib = ssv6006c_cmd_mib;
    hal_ops->cmd_power_saving = ssv6006c_cmd_power_saving;
    hal_ops->cmd_hwinfo = ssv6006c_cmd_hwinfo;
    hal_ops->get_rd_id_adr = ssv6006c_get_rd_id_adr;
    hal_ops->burst_read_reg = ssv6006c_burst_read_reg;
    hal_ops->burst_write_reg = ssv6006c_burst_write_reg;
    hal_ops->auto_gen_nullpkt = ssv6006c_auto_gen_nullpkt;

    hal_ops->load_fw_enable_mcu = ssv6006c_load_fw_enable_mcu;
    hal_ops->load_fw_disable_mcu = ssv6006c_load_fw_disable_mcu;
    hal_ops->load_fw_set_status = ssv6006c_load_fw_set_status;
    hal_ops->load_fw_get_status = ssv6006c_load_fw_get_status;
    hal_ops->load_fw_pre_config_device = ssv6006c_load_fw_pre_config_device;
    hal_ops->load_fw_post_config_device = ssv6006c_load_fw_post_config_device;
    hal_ops->reset_cpu = ssv6006c_reset_cpu;    
    hal_ops->set_sram_mode = ssv6006c_set_sram_mode;    
    hal_ops->get_sram_mode = ssv6006c_get_sram_mode;    
    hal_ops->enable_usb_acc = ssv6006c_enable_usb_acc;
    hal_ops->disable_usb_acc = ssv6006c_disable_usb_acc;
    hal_ops->set_usb_lpm = ssv6006c_set_usb_lpm;    
    hal_ops->jump_to_rom = ssv6006c_jump_to_rom;
    hal_ops->init_gpio_cfg = ssv6006c_init_gpio_cfg;
    hal_ops->cmd_efuse = ssv6006c_cmd_efuse;
    hal_ops->set_on3_enable = ssv6006c_set_on3_enable;
    hal_ops->init_hci_rx_aggr = ssv6006c_init_hci_rx_aggr;
    hal_ops->reset_hw_mac = ssv6006c_reset_hw_mac;
    hal_ops->set_crystal_clk = ssv6006c_set_crystal_clk;
    hal_ops->wait_usb_rom_ready = ssv6006c_wait_usb_rom_ready;
    hal_ops->detach_usb_hci = ssv6006c_detach_usb_hci; 
    hal_ops->pll_chk = ssv6006c_pll_chk; 
#ifdef CONFIG_STA_BCN_FILTER
    hal_ops->set_mrx_filter = ssv6006c_set_mrx_filter;
#endif

}
/* Used before ssv_hw is initialized */
static int ssv_hwif_read_reg (struct ssv_softc *sc, u32 addr, u32 *val)
{
    struct ssv6xxx_platform_data *priv = sc->dev->platform_data;
    return priv->ops->readreg(sc->dev, addr, val);
}

static u64 _ssv6006_get_ic_time_tag(struct ssv_softc *sc)
{
    u32 regval;
    u64 ic_time_tag;

    ssv_hwif_read_reg(sc, ADR_CHIP_DATE_YYYYMMDD, &regval);
    ic_time_tag = ((u64)regval<<32);
    ssv_hwif_read_reg(sc, ADR_CHIP_DATE_00HHMMSS, &regval);
    ic_time_tag |= (regval);

    return ic_time_tag;
}

void ssv_attach_ssv6006(struct ssv_softc *sc, struct ssv_hal_ops *hal_ops)
{
    u32  regval, chip_type;
    char fpga_tag[5];
    u64  ic_time_tag;
    struct ssv6xxx_platform_data *priv = sc->dev->platform_data;

    ssv_hwif_read_reg(sc, ADR_CHIP_INFO_FPGATAG, &regval);
    // get FPGA tag to identify phy v5    
    *((u32 *)&fpga_tag[0])= __be32_to_cpu(regval);
    fpga_tag[4] = 0x0;

    // get ic time tag to identify phy v4.
    ic_time_tag = _ssv6006_get_ic_time_tag(sc);

    printk(KERN_INFO"Load SSV6006 common code\n");
    ssv_attach_ssv6006_common(hal_ops);

    if (strstr(priv->chip_id, SSV6006C)
            || strstr(priv->chip_id, SSV6006D)){
        ssv_attach_ssv6006c_mac(hal_ops);
        printk(KERN_INFO"Load SSV6006C/D HAL MAC function \n");    
    }

    ssv_hwif_read_reg(sc, ADR_CHIP_TYPE_VER, &regval);
    chip_type = regval >>24;

    printk(KERN_INFO"Chip type %x\n", chip_type);

    if (chip_type == CHIP_TYPE_CHIP){

        printk(KERN_INFO"Load SSV6006 HAL common PHY function \n");
        ssv_attach_ssv6006_phy(hal_ops);
        if (strstr(priv->chip_id, SSV6006C)
                || strstr(priv->chip_id, SSV6006D)){     
            printk(KERN_INFO"Load SSV6006C/D HAL BB-RF function \n");
            ssv_attach_ssv6006_turismoC_BBRF(hal_ops); // for mp chip;
        }

    } 
}

#endif
