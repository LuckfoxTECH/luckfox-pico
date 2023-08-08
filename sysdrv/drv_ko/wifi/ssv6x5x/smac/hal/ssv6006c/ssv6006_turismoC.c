/*
 * Copyright (c) 2015 iComm Semiconductor Ltd.
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
#include <linux/version.h>
#if ((defined SSV_SUPPORT_SSV6006))
#include <linux/nl80211.h>

#include <ssv_chip_id.h>
#include <ssv6200.h>
#include "ssv6006c/ssv6006C_reg.h"
#include "ssv6006c/ssv6006C_aux.h"
#include <smac/dev.h>
#include <smac/efuse.h>
#include <smac/ssv_skb.h>
#include <hal.h>
#include <ssvdevice/ssv_cmd.h>
#include "ssv6006_mac.h"
#include "ssv6006_priv.h"
#include "ssv6006_priv_normal.h"
#include "turismoC_rf_reg.c"
#include "turismoC_wifi_phy_reg.c"
#include "ssv_desc.h"
#include "turismo_common.h"
#include "turismo_common.c"

#include <ssv6xxx_common.h>
#include <linux_80211.h>

//------------mapping from value to index----------------------
#if 0
static uint8_t rate_gain_value[] = {0x72, 0x5B, 0x60, 0x66, 0x6C, 0x72, 0x79, 0x80, 0x88, 0x90, 0x98, 0xA1, 0xAB, 0xB5};
static uint8_t band_gain_value[] = {0x72, 0x40, 0x44, 0x48, 0x4C, 0x51, 0x56, 0x5B, 0x60, 0x66, 0x6C, 0x72, 0x79, 0x80, 0x88, 0x90, 0x98, 0xA1, 0xAB, 0xB5};
#endif
static uint8_t rate_gain_value[] = {0x72, 0xb5, 0xab, 0xa1, 0x98, 0x90, 0x88, 0x80, 0x79, 0x72, 0x6c, 
                                    0x66, 0x60, 0x5b, 0x56, 0x51, 0x4c, 0x48, 0x44, 0x40};
static uint8_t band_gain_value[] = {0x72, 0x48, 0x4c, 0x51, 0x56, 0x5b, 0x60, 0x66, 0x6c, 0x72, 0x79,
                                    0x80, 0x88, 0x90, 0x98, 0xa1, 0xab};
#define MAX_RATE_GAIN_NUM   (sizeof(rate_gain_value) / sizeof((rate_gain_value)[0]))
#define MAX_BAND_GAIN_NUM   (sizeof(band_gain_value) / sizeof((band_gain_value)[0]))

int rf_table_get_rate_gain_idx_by_value(u32 value)
{
    u8 rate_gain_idx = 0;

    // rate_gain_index start from 1
    // rate gain_index 0 may be correct default value
    for (rate_gain_idx = 1; rate_gain_idx < MAX_RATE_GAIN_NUM; rate_gain_idx++)
    {
        if (rate_gain_value[rate_gain_idx] == value)
            break;
    }

    if (rate_gain_idx == MAX_RATE_GAIN_NUM) {
        printk("Cannot find rate gain[%d] index, reset to default value\n", value);
        rate_gain_idx = 0;
    }

    return rate_gain_idx;
}

int rf_table_get_band_gain_idx_by_value(u32 value)
{
    u8 band_gain_idx = 0;

    // band_gain_index start from 1
    // band gain_index 0 may be correct default value
    for (band_gain_idx = 1; band_gain_idx < MAX_BAND_GAIN_NUM; band_gain_idx++)
    {
        if (band_gain_value[band_gain_idx] == value)
            break;
    }

    if (band_gain_idx == MAX_BAND_GAIN_NUM) {
        printk("Cannot find band gain[%d] index, reset to default value\n", value);
        band_gain_idx = 0;
    }

    return band_gain_idx;
}

struct st_rf_table def_rf_table = {
    /* rt_config */
    { {6, 6, 6, 6, 6, 6, 6} , 0x42, 0x42, 7, 9, 10, 7, 7, 7, 7, 7, 0},
    /* ht_config */
    { {6, 6, 6, 6, 6, 6, 6} , 0x42, 0x42, 7, 9, 10, 7, 7, 7, 7, 7, 0},
    /* lt_config */
    { {6, 6, 6, 6, 6, 6, 6} , 0x42, 0x42, 7, 9, 10, 7, 7, 7, 7, 7, 0},
    /* rf_gain */
    4,
    /* rate_gain_b */
    12,
    /* rate_config_g */
    {1, 3, 5, 7},
    /* rate_config_20n */
    {1, 3, 5, 7},
    /* rate_config_40n */
    {1, 3, 5, 7},
    /* low_boundarty */
    30,
    /* high_boundary */
    50,
    /* boot flag*/
    EN_NOT_FIRST_BOOT,
    /* work mode */
    EN_WORK_NOMAL,
    /* rt_5g_config */
    { 11, 8, 8, 7, 0x9264924a, 0x96dbb6cc },
    /* ht_5g_config */
    { 11, 8, 8, 7, 0x9264924a, 0x96dbb6cc },
    /* lt_5g_config */
    { 11, 8, 8, 7, 0x9264924a, 0x96dbb6cc },
    /* band_f0_threshold */
    0x141E,
    /* band_f1_threshold */
    0x157C,
    /* band_f2_threshold */
    0x1644,
    /* Signature */
    RF_API_SIGNATURE,
    /* Structure version */
    RF_API_TABLE_VERSION,
    /* DCDC flag */
    1,
    /* external pa settings*/
    { 0, 0, 1, 0xd, 8, 8, 0xc, 0},
};


//------------mapping from value to index----------------------

static bool ssv6006_turismoC_set_rf_enable(struct ssv_hw *sh, bool val);

static void ssv6006_turismoC_load_phy_table(ssv_cabrio_reg **phy_table)
{
    return;
}

static u32 ssv6006_turismoC_get_phy_table_size(struct ssv_hw *sh)
{
    return 0;
}

static void ssv6006_turismoC_load_rf_table(ssv_cabrio_reg **rf_table)
{
    return;
}

static u32 ssv6006_turismoC_get_rf_table_size(struct ssv_hw *sh)
{
    return 0;
}

static void ssv6006_turismoC_init_PLL(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return;
    }

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_INIT_PLL;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_INIT_PLL);
    host_cmd->len = HOST_CMD_HDR_LEN;

    HCI_SEND_CMD(sc->sh, skb);
}

static int ssv6006_turismoC_set_channel(struct ssv_softc *sc, struct ieee80211_channel *chan, 
        enum nl80211_channel_type channel_type, bool offchan)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    struct ssv_rf_chan *ptr = NULL;
    int ret = 0;
#ifdef	CONFIG_ENABLE_ACS_FUNC
	struct ssv_hw *sh = sc->sh;

	HAL_EDCA_UPDATE_SURVEY(sh);
	HAL_EDCA_ENABLE(sh, false);
#endif

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_chan));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_chan));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_CHAN;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_CHAN);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_chan);

    ptr = (struct ssv_rf_chan *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_rf_chan));
    ptr->chan = chan->hw_value; 
    ptr->chan_type = channel_type;
    ptr->off_chan = offchan;

#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)) && defined(CONFIG_HW_SCAN))
    if ((false == sc->hw_scan_done) || ((true == sc->bScanning) && (true == sc->hw_scan_start))) {
        ptr->scan = 1;
        //IEEE80211_CHAN_NO_IR: do not initiate radiation, this includes sending probe requests or beaconing.
        //IEEE80211_CHAN_RADAR: Radar detection is required on this channel.
        if (sc->scan_is_passive || (chan->flags & IEEE80211_CHAN_NO_IR)) {
            ptr->passive_chan = 1;
        }
        memcpy(&ptr->scan_param, &sc->scan_param, sizeof(struct ssv_scan_param));
        if (chan->hw_value > 14) {
            ptr->scan_param.no_cck = true;
            memcpy(ptr->scan_param.ie, sc->scan_ie_band5g, sc->scan_ie_len_band5g);
            ptr->scan_param.ie_len = sc->scan_ie_len_band5g;
        } else {
            memcpy(ptr->scan_param.ie, sc->scan_ie_band2g, sc->scan_ie_len_band2g);
            ptr->scan_param.ie_len = sc->scan_ie_len_band2g;
        }
    }
#endif
    printk("chan change ch %d, type %d, off_chan %d\n", ptr->chan, ptr->chan_type, ptr->off_chan);
    ret = HCI_SEND_CMD(sc->sh, skb);

    // update tx power
    sc->dpd.pwr_mode  = NORMAL_PWR;  
#ifdef CONFIG_ENABLE_ACS_FUNC
	sc->survey_cur_chan = chan->hw_value;
	HAL_EDCA_ENABLE(sh, true);
#endif	
    return 0;
}

static u32 ssv6006_turismoC_update_xtal(struct ssv_hw *sh)
{
    u32 xtal = 0;

    switch (sh->cfg.crystal_type) {
        case SSV6XXX_IQK_CFG_XTAL_16M:
            xtal = 16;
            break;
        case SSV6XXX_IQK_CFG_XTAL_24M:
            xtal = 24;
            break;
        case SSV6XXX_IQK_CFG_XTAL_26M:
            xtal = 26;
            break;
        case SSV6XXX_IQK_CFG_XTAL_40M:
            xtal = 40;
            break;
        case SSV6XXX_IQK_CFG_XTAL_12M:
            xtal = 12;
            break;
        case SSV6XXX_IQK_CFG_XTAL_20M:
            xtal = 20;
            break;
        case SSV6XXX_IQK_CFG_XTAL_25M:
            xtal = 25;
            break;
        case SSV6XXX_IQK_CFG_XTAL_32M:
            xtal = 32;
            break; 
        default:
            xtal = 40;
            printk("Please redefine xtal_clock(wifi.cfg)!!\n");
            WARN_ON(1);
            break;      
    }

    return xtal;
}

static int ssv6006_turismoC_set_pll_phy_rf(struct ssv_hw *sh ,ssv_cabrio_reg *rf_tbl, ssv_cabrio_reg *phy_tbl)
{	
    struct ssv_softc *sc = sh->sc; 
    int  ret = 0;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    struct ssv_rf_cali *ptr = NULL;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_INIT_PLL_PHY_RF;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_INIT_PLL_PHY_RF);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali);

    ptr = (struct ssv_rf_cali *)host_cmd->un.dat8;
    memset(ptr, 0, sizeof(struct ssv_rf_cali));
    if (sh->cfg.hw_caps & SSV6200_HW_CAP_5GHZ)
        ptr->support_5g = 1;

    if (sh->cfg.disable_fw_thermal == 1) {
        ptr->thermal = 0;
    } else {
        ptr->thermal = 1;
    }
    
    ptr->disable_greentx = (true == sh->cfg.disable_greentx) ? 1 : 0;
    ptr->disable_cci = (true == sh->cfg.disable_cci) ? 1 : 0;
    ptr->xtal = ssv6006_turismoC_update_xtal(sh); 
    ptr->bus_clk = ((sh->cfg.clk_src_80m == true) ? 80 : 40);
    // update rf table
    memcpy(&ptr->rf_table, &sh->rf_table, sizeof(struct st_rf_table)); 
    ptr->thermal_low_thershold = sh->rf_table.low_boundary;
    ptr->thermal_high_thershold = sh->rf_table.high_boundary;
    if (sh->cfg.volt_regulator == SSV6XXX_VOLT_LDO_CONVERT) {
        ptr->rf_table.dcdc_flag = 0;
    } else { 
        ptr->rf_table.dcdc_flag = 1;
    }

    ret = HCI_SEND_CMD(sc->sh, skb);

    // default txgain
    sh->default_txgain[BAND_2G] = GET_RG_TX_GAIN;
    sh->default_txgain[BAND_5100] = GET_RG_5G_TX_GAIN_F0;
    sh->default_txgain[BAND_5500] = GET_RG_5G_TX_GAIN_F1;
    sh->default_txgain[BAND_5700] = GET_RG_5G_TX_GAIN_F2;
    sh->default_txgain[BAND_5900] = GET_RG_5G_TX_GAIN_F3;
    // default edcca
    sh->sc->default_edcca = GET_RG_AGC_THRESHOLD;

    return ret;
}

static bool ssv6006_turismoC_set_rf_enable(struct ssv_hw *sh, bool val)
{
    struct ssv_softc *sc = sh->sc; 
    bool  ret = true;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return false;
    }

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    if (val) {
        host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_ENABLE;
        host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_ENABLE);
    } else {
        host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_DISABLE;
        host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_DISABLE);
    }
    host_cmd->len = HOST_CMD_HDR_LEN;
    ret = HCI_SEND_CMD(sc->sh, skb);

    return true;   
}

static bool ssv6006_turismoC_dump_phy_reg(struct ssv_hw *sh)
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_softc *sc = sh->sc; 
    int  ret = 0;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return false;
    }

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_DUMP_PHY_REG;
    host_cmd->len = HOST_CMD_HDR_LEN;
    ret = HCI_SEND_CMD(sc->sh, skb);
#endif
    return true;
}

static bool ssv6006_turismoC_dump_rf_reg(struct ssv_hw *sh)
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_softc *sc = sh->sc; 
    int  ret = 0;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return false;
    }

    skb_put(skb, HOST_CMD_HDR_LEN);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
    host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_DUMP_RF_REG;
    host_cmd->len = HOST_CMD_HDR_LEN;
    ret = HCI_SEND_CMD(sc->sh, skb);
#endif
    return true;
}

static bool ssv6006_turismoC_support_iqk_cmd(struct ssv_hw *sh)
{
    return false;
}

static void ssv6006_cmd_turismoC_cali(struct ssv_hw *sh, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_DBGMSG
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    struct ssv_rf_cali_param *cali_param;
    struct ssv_rf_chan *chan;
    int  i = 0, pa_band = 0, ret = 0;

    if(!strcmp(argv[1], "do")){
        skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali_param)));
        if (skb != NULL) {
            skb_put(skb, HOST_CMD_HDR_LEN +  sizeof(struct ssv_rf_cali_param));
            host_cmd = (struct cfg_host_cmd *)skb->data;
            memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
            host_cmd->c_type = HOST_CMD;
            host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
            host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_INIT_CALI;
            host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_INIT_CALI);
            host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali_param);
            cali_param = (struct ssv_rf_cali_param *)host_cmd->un.dat8;
            memset(cali_param, 0x0, sizeof(struct ssv_rf_cali_param));
            if (sh->cfg.hw_caps & SSV6200_HW_CAP_5GHZ)
                cali_param->support_5g = 1;
            ret = HCI_SEND_CMD(sc->sh, skb);
            snprintf_res(cmd_data,"\n   CALIRATION DONE\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

    } else if(!strcmp(argv[1], "show")) {
    #ifdef CONFIG_ENABLE_CLI_DBG_TOOL
        ssv6006c_cmd_hwinfo_phy_cali(sh);
    #endif
    } else if(!strcmp(argv[1], "restore")) {

        skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali_param));
        if (skb != NULL) {
            skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali_param));
            host_cmd = (struct cfg_host_cmd *)skb->data;
            memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
            host_cmd->c_type = HOST_CMD;
            host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
            host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RESTORE_CALI;
            host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RESTORE_CALI);
            host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_cali_param);
            cali_param = (struct ssv_rf_cali_param *)host_cmd->un.dat8;
            memset(cali_param, 0x0, sizeof(struct ssv_rf_cali_param));
            if (sh->cfg.hw_caps & SSV6200_HW_CAP_5GHZ)
                cali_param->support_5g = 1;
            ret = HCI_SEND_CMD(sc->sh, skb);
            snprintf_res(cmd_data, "\n Restore calibration result done!\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

    }else if(!strcmp(argv[1], "dpd")){

        if(!strcmp(argv[2], "show")){
            snprintf_res(cmd_data, "\n DPD result: \n");
            for (pa_band = 0; pa_band < PADPDBAND; pa_band++) {
                u32 regval;

                switch (pa_band) {
                    case 0:
                        snprintf_res(cmd_data,"\t 2G       channel <= 14 ");
                        break;
                    case 1:
                        snprintf_res(cmd_data,"\t 5G       channel <  36");
                        break;
                    case 2:
                        snprintf_res(cmd_data,"\t 5G 36 <= channel < 100");
                        break;
                    case 3:
                        snprintf_res(cmd_data,"\t 5G 100<= channel < 140");
                        break;
                    case 4:
                        snprintf_res(cmd_data,"\t 5G 140<= channel");
                        break;
                    default:
                        break;
                }

                snprintf_res(cmd_data,"\n");

                snprintf_res(cmd_data,"\n\t\tread back bank%d am_am:", pa_band); 
                for (i = 0 ; i < MAX_PADPD_TONE/2; i ++) {
                    if (i % 4 == 0)
                        snprintf_res(cmd_data,"\n\t\t");

                    regval = REG32_R(padpd_am_addr_table[pa_band][i]);
                    snprintf_res(cmd_data, "%03d %03d ", regval & 0xffff, (regval >>16) & 0xffff);
                }
                snprintf_res(cmd_data,"\n\n\n");
            } // end of pa_band

        } else if(!strcmp(argv[2], "enable")){
            skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
            if (skb != NULL) {
                skb_put(skb, HOST_CMD_HDR_LEN);
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_DPD_ENABLE;
                host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_DPD_ENABLE);
                host_cmd->len = HOST_CMD_HDR_LEN;
                ret = HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data,"enable DPD\r\n");
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

        } else if(!strcmp(argv[2], "disable")){
            skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
            if (skb != NULL) {
                skb_put(skb, HOST_CMD_HDR_LEN);
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_DPD_DISABLE;
                host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_DPD_DISABLE);
                host_cmd->len = HOST_CMD_HDR_LEN;
                ret = HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data,"disable DPD\r\n");
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

        } else if(!strcmp(argv[2], "do")) {
            skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_chan)));
            if (skb != NULL) {
                skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_chan)));
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_INIT_DPD;
                host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_INIT_DPD);
                host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_chan);
                chan = (struct ssv_rf_chan *)host_cmd->un.dat8;
                memset(chan, 0, sizeof(struct ssv_rf_chan));
                chan->chan = sc->hw_chan; 
                chan->chan_type = sc->hw_chan_type;
                printk("sc->hw_chan = %d, sc->hw_chan_type = %d\n", chan->chan, chan->chan_type);
                ret = HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data, "DPD done\r\n");
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

        } else if(!strcmp(argv[2], "restore")) {
            skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
            if (skb != NULL) {
                skb_put(skb, HOST_CMD_HDR_LEN);
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RESTORE_DPD;
                host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RESTORE_DPD);
                host_cmd->len = HOST_CMD_HDR_LEN;
                ret = HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data, "\n Restore current band dpd result done!\n");
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

        } else {
            snprintf_res(cmd_data,"\n cali [do|show|restore|dpd(do|show|enable|disable|restore)] \n");    
        }

    }else {
        snprintf_res(cmd_data,"\n cali [do|show|restore|dpd(show|enable|disable)] \n");
    }     
#endif
}

static void ssv6006_cmd_turismoC_txgen(struct ssv_hw *sh, u8 drate)
{
#ifdef CONFIG_ENABLE_DBGMSG
    struct sk_buff *skb = NULL;
    int    len = (int) sizeof(pkt1614);
    unsigned char *data = NULL;
    struct ssv6006_tx_desc *tx_desc;
    struct ssv_softc *sc = sh->sc;

    skb = ssv_skb_alloc(sc, len);
    if (!skb) {
        printk("Cannot alloc skb_buf\n");
        return;
    }

    data = skb_put(skb, len);
    memcpy(data, pkt1614, len);
    tx_desc = (struct ssv6006_tx_desc *)data;
    tx_desc->drate_idx0 = drate;

    SET_RG_TXD_SEL(0);  // set tx gen path disable
    SET_RG_TX_START(0); // disable trigger tx!!
    HCI_SEND_CMD(sc->sh, skb);
#endif

}

static void ssv6006_cmd_turismoC_rfphy_ops(struct ssv_softc *sc,  ssv_rf_phy_ops PHY_OPS,struct ssv_rf_tool_param *param)
{
    struct sk_buff *skb = NULL;
	struct cfg_host_cmd *host_rf_cmd;
	struct ssv_rf_tool_param *rf_tool_param;
	u32  ssv_skb_lenth = HOST_CMD_HDR_LEN;

	if (NULL != param) {
	    ssv_skb_lenth = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);
	}
	  
    skb = ssv_skb_alloc(sc, ssv_skb_lenth);
    if (skb != NULL) {
        skb_put(skb, ssv_skb_lenth);
	    host_rf_cmd = (struct cfg_host_cmd *)skb->data;
	    memset(host_rf_cmd, 0x0, sizeof(struct cfg_host_cmd));
	    host_rf_cmd->c_type = HOST_CMD;
	    host_rf_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
	    host_rf_cmd->sub_h_cmd = (u32)PHY_OPS;
	    host_rf_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)PHY_OPS);
	    host_rf_cmd->len = ssv_skb_lenth;
		if (NULL != param) {
			rf_tool_param = (struct ssv_rf_tool_param *)host_rf_cmd->un.dat8;
			memcpy(rf_tool_param, param, sizeof(struct ssv_rf_tool_param));
		}

		HCI_SEND_CMD(sc->sh, skb);
    }
}

static void ssv6006_cmd_turismoC_rfphy_ops_start(struct ssv_softc *sc, u32 regval)
{
	struct ssv_rf_tool_param rf_param;;
	memset(&rf_param ,0 ,sizeof(struct ssv_rf_tool_param));
	rf_param.count = 0xffffffff;
	rf_param.interval = regval;
	ssv6006_cmd_turismoC_rfphy_ops(sc, SSV6XXX_RFPHY_CMD_RF_TOOL_TX, &rf_param);
}

static void ssv6006_cmd_turismoC_rfphy_ops_stop(struct ssv_softc *sc)
{
	ssv6006_cmd_turismoC_rfphy_ops(sc, SSV6XXX_RFPHY_CMD_RF_TOOL_STOP, NULL);
}


static void ssv6006_cmd_turismoC_rf(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    u32 regval = 0;
    char *endp;
	bool is_txgen = false;
	u32 interval = 0;
    struct ssv_softc *sc = sh->sc;
    int ch = sc->hw_chan;
    int pa_band =0;
    struct ssv_rf_tool_param *rf_tool_param;
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd;
    unsigned char rate_tbl[] = {
        0x00,0x01,0x02,0x03,                        // B mode long preamble [0~3]
        0x00,0x12,0x13,                             // B mode short preamble [4~6], no 2M short preamble
        0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,    // G mode [7~14]
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,    // N mode HT20 long GI mixed format [15~22]
        0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,    // N mode HT20 short GI mixed format  [23~30]
        0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,    // N mode HT40 long GI mixed format [31~38]
        0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,    // N mode HT40 short GI mixed format  [39~46]
    };
#define SIZE_RATE_TBL   (sizeof(rate_tbl) / sizeof((rate_tbl)[0]))

    pa_band = ssv6006_get_pa_band(ch);

    if (argc < 2) 
        goto out;

    if (!strcmp(argv[1], "tx")) {

        if (argc == 3)
            regval = simple_strtoul(argv[2], &endp, 0);
        else
            regval = 1;

        skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
        if (skb != NULL) {
            skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
            host_cmd = (struct cfg_host_cmd *)skb->data;
            memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
            host_cmd->c_type = HOST_CMD;
            host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
            host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_TX;
            host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_TX);
            host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);

            rf_tool_param = (struct ssv_rf_tool_param *)host_cmd->un.dat8;
            memset(rf_tool_param, 0x0, sizeof(struct ssv_rf_tool_param));
            rf_tool_param->count = 0xffffffff;
            rf_tool_param->interval = regval;

            HCI_SEND_CMD(sc->sh, skb);
            sc->sc_flags |= SC_OP_BLOCK_CNTL;
            sc->sc_flags |= SC_OP_CHAN_FIXED; // fixed channel
            snprintf_res(cmd_data,"\n   RF TX\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");

        return;

    } else if (!strcmp(argv[1], "rx")) {
        sc->sc_flags |= SC_OP_BLOCK_CNTL;
        sc->sc_flags |= SC_OP_CHAN_FIXED; // fixed channel
        snprintf_res(cmd_data,"\n   RF RX\n");

    } else if(!strcmp(argv[1], "stop")) {

        skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN);
        if (skb != NULL) {
            skb_put(skb, HOST_CMD_HDR_LEN);
            host_cmd = (struct cfg_host_cmd *)skb->data;
            memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
            host_cmd->c_type = HOST_CMD;
            host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
            host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_STOP;
            host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_STOP);
            host_cmd->len = HOST_CMD_HDR_LEN;
            HCI_SEND_CMD(sc->sh, skb);
            sc->sc_flags &= ~SC_OP_BLOCK_CNTL;
            sc->sc_flags &= ~SC_OP_CHAN_FIXED;
            snprintf_res(cmd_data,"\n   RF STOP\n");
        } else
            snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        return;

    } else if(!strcmp(argv[1], "rate")){
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);
            if ((regval != 4) && (regval < SIZE_RATE_TBL)) {
				if((1==GET_RG_TX_START)&&(1==GET_RG_TXD_SEL)){
					is_txgen = true;
					interval = GET_RG_IFS_TIME;
				}
				
				if(is_txgen){
					ssv6006_cmd_turismoC_rfphy_ops_stop(sc);
				}
                skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
                if (skb != NULL) {
                    skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
                    host_cmd = (struct cfg_host_cmd *)skb->data;
                    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                    host_cmd->c_type = HOST_CMD;
                    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                    host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_RATE;
                    host_cmd->blocking_seq_no = 
                        (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_RATE);
                    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);

                    rf_tool_param = (struct ssv_rf_tool_param *)host_cmd->un.dat8;
                    memset(rf_tool_param, 0, sizeof(struct ssv_rf_tool_param));
                    rf_tool_param->rate = regval;

                    HCI_SEND_CMD(sc->sh, skb);
                    snprintf_res(cmd_data,"\n   rf rate index %d\n", regval);					
                } else{
                    snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
				}

				if(is_txgen){
					ssv6006_cmd_turismoC_rfphy_ops_start(sc,interval);
				}
				
            } else {
                snprintf_res(cmd_data,"Not support rf rate index %d\n", regval);
            }
            return;

        } else {
            snprintf_res(cmd_data,"\n\t Incorrect rf rate set format\n");
            return;
        }

    } else if(!strcmp(argv[1], "disablethermal")){
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);
            regval = (0 == regval) ? 0 : 1;

            skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
            if (skb != NULL) {
                skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_THERMAL;
                host_cmd->blocking_seq_no =
                    (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_THERMAL);
                host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);

                rf_tool_param = (struct ssv_rf_tool_param *)host_cmd->un.dat8;
                memset(rf_tool_param, 0, sizeof(struct ssv_rf_tool_param));
                rf_tool_param->disable_thermal = regval;

                HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data,"\n   rf thermal %d\n", regval);
            } else {
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
		    }

            return;

        } else {
            snprintf_res(cmd_data,"\n\t Incorrect rf disablethermal set format\n");
            return;
        }

    } else if(!strcmp(argv[1], "freq")){
        if (argc == 3){
            regval = simple_strtoul(argv[2], &endp, 0);

            skb = ssv_skb_alloc(sc, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
            if (skb != NULL) {
                skb_put(skb, (HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param)));
                host_cmd = (struct cfg_host_cmd *)skb->data;
                memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
                host_cmd->c_type = HOST_CMD;
                host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_RFPHY_OPS;
                host_cmd->sub_h_cmd = (u32)SSV6XXX_RFPHY_CMD_RF_TOOL_FREQ;
                host_cmd->blocking_seq_no = 
                    (((u16)SSV6XXX_HOST_CMD_RFPHY_OPS << 16) | (u16)SSV6XXX_RFPHY_CMD_RF_TOOL_FREQ);
                host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv_rf_tool_param);

                rf_tool_param = (struct ssv_rf_tool_param *)host_cmd->un.dat8;
                memset(rf_tool_param, 0, sizeof(struct ssv_rf_tool_param));
                rf_tool_param->freq = regval;

                HCI_SEND_CMD(sc->sh, skb);
                snprintf_res(cmd_data,"Set cbanki/cbanko to 0x%x\n", regval);
            } else
                snprintf_res(cmd_data,"\n Cannot alloc host command buffer\n");
        } else{
            snprintf_res(cmd_data,"./cli rf freq [value]\n");
        }
        return;

    } else if(!strcmp(argv[1], "rfreq")){
        if (argc == 2){
            snprintf_res(cmd_data,"Get freq 0x%x/0x%x\n", GET_RG_XO_CBANKI, GET_RG_XO_CBANKO);
        } else {
            snprintf_res(cmd_data,"./cli rf rfreq\n");
        }
        return;

    } else if(!strcmp(argv[1], "count")){
        if  (argc == 3) {
            int     count = 0,  err = 0, integer= 0, point = 0;
            bool valid =false;
            if (!strcmp(argv[2], "0")){
                count = GET_RO_11B_PACKET_CNT;
                err = GET_RO_11B_PACKET_ERR_CNT;
                valid = true;  
            } else if(!strcmp(argv[2], "1")){
                count = GET_RO_11GN_PACKET_CNT;
                err = GET_RO_11GN_PACKET_ERR_CNT;                
                valid = true;   
            }
            if (count != 0) {
                integer = (err * 100)/count;
                point = ((err*10000)/count)%100;
            }
            if (valid) {
                snprintf_res(cmd_data,"count = %d\n", count);
                snprintf_res(cmd_data,"err = %d\n", err);
                snprintf_res(cmd_data,"err_rate = %01d.%02d%\n", integer, point);
                return; 
            }
        }            
        snprintf_res(cmd_data,"\n\t./cli rf count 0|1\n");
        return;

    } else if(!strcmp(argv[1], "rssi")){
        snprintf_res(cmd_data,"\n ofdm RSSI -%d, B mode RSSI -%d\n", 
                GET_RO_11GN_RCPI, GET_RO_11B_RCPI);
        return;

    } else if(!strcmp(argv[1], "phy_txgen")) {

        ssv6006_cmd_turismoC_rfphy_ops_start(sc, 1);
        sc->sc_flags |= SC_OP_BLOCK_CNTL;
        sc->sc_flags |= SC_OP_CHAN_FIXED; // fixed channel
        snprintf_res(cmd_data,"\n   RF TX\n");
        return;
	}else if(!strcmp(argv[1], "block")){
		sc->sc_flags |= SC_OP_BLOCK_CNTL;
        sc->sc_flags |= SC_OP_CHAN_FIXED;
        snprintf_res(cmd_data,"\n\t block control form system\n");
        return;
    } else if(!strcmp(argv[1], "unblock")){
   		sc->sc_flags &= ~SC_OP_BLOCK_CNTL;
        sc->sc_flags &= ~SC_OP_CHAN_FIXED;
        ssv6006_cmd_turismoC_rfphy_ops_stop(sc);
        snprintf_res(cmd_data,"\n\t unblock control form system\n");
        return;
	} else if(!strcmp(argv[1], "ack")){
        if (argc == 3) {
            if (!strcmp(argv[2], "disable")){               
                snprintf_res(cmd_data,"\n set %s %s\n", argv[1], argv[2]);
            } else if (!strcmp(argv[2], "enable")){
                snprintf_res(cmd_data,"\n set %s %s\n", argv[1], argv[2]);
            }
        } else {
            snprintf_res(cmd_data,"\n\t incorrect set ack format\n");
        }
        return;
	}else {
    
        snprintf_res(cmd_data, 
                "\n\t./cli rf phy_txgen|block|unblock|count|ack|freq|rfreq|rssi|rate|ifs|disablethermal\n");
        return;
    }
out:

    snprintf_res(cmd_data,"\n\t Current RF tool settings: ch %d, pa_band %d\n", ch, pa_band);
    snprintf_res(cmd_data,"\t bbscale:\n");

    if (sc->sc_flags && SC_OP_BLOCK_CNTL) {
        snprintf_res(cmd_data,"\t system control is blocked\n");
    } else {
        snprintf_res(cmd_data,"\t WARNING system control is not blocked\n");
    }     
    snprintf_res(cmd_data,"\t\t HT40 0x%08x, HT20 0x%08x, Legacy 0x%08x, B 0x%02x\n",
            REG32(ADR_WIFI_PHY_COMMON_BB_SCALE_REG_3), REG32(ADR_WIFI_PHY_COMMON_BB_SCALE_REG_2),
            REG32(ADR_WIFI_PHY_COMMON_BB_SCALE_REG_1), GET_RG_BB_SCALE_BARKER_CCK);

    switch (pa_band){
        case BAND_2G:
            regval = GET_RG_DPD_BB_SCALE_2500;
            break;
        case BAND_5100:
            regval = GET_RG_DPD_BB_SCALE_5100;
            break;
        case BAND_5500:    
            regval = GET_RG_DPD_BB_SCALE_5500;                   
            break;
        case BAND_5700:    
            regval = GET_RG_DPD_BB_SCALE_5700;                    
            break;
        case BAND_5900:    
            regval = GET_RG_DPD_BB_SCALE_5900;                   
            break;
        default:
            break;
    }    
    snprintf_res(cmd_data,"\t current band dpd bbscale: 0x%x\n", regval);

    snprintf_res(cmd_data,"\t cbank:\n");
    snprintf_res(cmd_data,"\t\t CBANKI %d, CBANKO %d\n", GET_RG_XO_CBANKI, GET_RG_XO_CBANKO);        
}

static void ssv6006_turismoC_chg_xtal_freq_offset(struct ssv_hw *sh)
{
    if (sh->flash_config.exist) {
        // [15:8] xi, [7:0] xo
        sh->rf_table.low_boundary = sh->flash_config.xtal_offset_low_boundary;
        sh->rf_table.high_boundary = sh->flash_config.xtal_offset_high_boundary;
        sh->rf_table.rt_config.freq_xi = ((sh->flash_config.rt_config.xtal_offset & 0xff00) >> 8);
        sh->rf_table.rt_config.freq_xo = ((sh->flash_config.rt_config.xtal_offset & 0x00ff) >> 0);
        sh->rf_table.lt_config.freq_xi = ((sh->flash_config.lt_config.xtal_offset & 0xff00) >> 8);
        sh->rf_table.lt_config.freq_xo = ((sh->flash_config.lt_config.xtal_offset & 0x00ff) >> 0);
        sh->rf_table.ht_config.freq_xi = ((sh->flash_config.ht_config.xtal_offset & 0xff00) >> 8);
        sh->rf_table.ht_config.freq_xo = ((sh->flash_config.ht_config.xtal_offset & 0x00ff) >> 0);
    } else if (sh->efuse_bitmap & BIT(EFUSE_CRYSTAL_FREQUENCY_OFFSET)) {
        sh->rf_table.rt_config.freq_xi = sh->cfg.crystal_frequency_offset;
        sh->rf_table.rt_config.freq_xo = sh->cfg.crystal_frequency_offset;
        sh->rf_table.lt_config.freq_xi = sh->cfg.crystal_frequency_offset;
        sh->rf_table.lt_config.freq_xo = sh->cfg.crystal_frequency_offset;
        sh->rf_table.ht_config.freq_xi = sh->cfg.crystal_frequency_offset;
        sh->rf_table.ht_config.freq_xo = sh->cfg.crystal_frequency_offset;
    }
}

static void ssv6006_turismoC_chg_cck_bbscale(struct ssv_hw *sh, int cck)
{
    sh->rf_table.rate_gain_b = rf_table_get_rate_gain_idx_by_value(cck);
}

static void ssv6006_turismoC_chg_legacy_bbscale(struct ssv_hw *sh, int bpsk, int qpsk, int qam16, int qam64)
{
    sh->rf_table.rate_config_g.rate1 = rf_table_get_rate_gain_idx_by_value(bpsk);
    sh->rf_table.rate_config_g.rate2 = rf_table_get_rate_gain_idx_by_value(qpsk);
    sh->rf_table.rate_config_g.rate3 = rf_table_get_rate_gain_idx_by_value(qam16);
    sh->rf_table.rate_config_g.rate4 = rf_table_get_rate_gain_idx_by_value(qam64);
}

static void ssv6006_turismoC_chg_ht20_bbscale(struct ssv_hw *sh, int bpsk, int qpsk, int qam16, int qam64)
{
    sh->rf_table.rate_config_20n.rate1 = rf_table_get_rate_gain_idx_by_value(bpsk);
    sh->rf_table.rate_config_20n.rate2 = rf_table_get_rate_gain_idx_by_value(qpsk);
    sh->rf_table.rate_config_20n.rate3 = rf_table_get_rate_gain_idx_by_value(qam16);
    sh->rf_table.rate_config_20n.rate4 = rf_table_get_rate_gain_idx_by_value(qam64);
}

static void ssv6006_turismoC_chg_ht40_bbscale(struct ssv_hw *sh, int bpsk, int qpsk, int qam16, int qam64)
{
    sh->rf_table.rate_config_40n.rate1 = rf_table_get_rate_gain_idx_by_value(bpsk);
    sh->rf_table.rate_config_40n.rate2 = rf_table_get_rate_gain_idx_by_value(qpsk);
    sh->rf_table.rate_config_40n.rate3 = rf_table_get_rate_gain_idx_by_value(qam16);
    sh->rf_table.rate_config_40n.rate4 = rf_table_get_rate_gain_idx_by_value(qam64);
}

static void ssv6006_turismoC_chg_bbscale(struct ssv_hw *sh)
{
    int value = 0;
    int b_rate_gain_tbl[] = {0x98, 0x90, 0x88, 0x80, 0x79, 0x72, 0x6c, 0x66,
        0x60, 0x5b, 0x56, 0x51, 0x4c, 0x48, 0x44, 0x40};
    int ofdm_rate_gain_tbl[] = {0xb5, 0xab, 0xa1, 0x98, 0x90, 0x88, 0x80, 0x79, 0x72, 0x6c, 
        0x66, 0x60, 0x5b, 0x56, 0x51, 0x4c, 0x48, 0x44, 0x40};

#define SIZE_B_RATE_GAIN_TBL   (sizeof(b_rate_gain_tbl) / sizeof((b_rate_gain_tbl)[0]))
#define MAX_OFDM_RATE_GAIN_INDEX                 (12)
#define OFDM_RATE_GAIN_64QAM_OFFSET              (6)  
#define OFDM_RATE_GAIN_16QAM_OFFSET              (4)  
#define OFDM_RATE_GAIN_QPSK_OFFSET               (2)  
#define OFDM_RATE_GAIN_BPSK_OFFSET               (0)  

#define EFUSE_RATE_GAIN_MASK_CCK                 (0x01)  
#define EFUSE_RATE_GAIN_MASK_LEGACY              (0x02)  
#define EFUSE_RATE_GAIN_MASK_HT20                (0x04)  
#define EFUSE_RATE_GAIN_MASK_HT40                (0x08)  

#define EFUSE_RATE_TBL_MASK_B_RATE               (0xf0)
#define EFUSE_RATE_TBL_SFT_B_RATE                (4)
#define EFUSE_RATE_TBL_MASK_LEGACY_RATE          (0xf0)
#define EFUSE_RATE_TBL_SFT_LEGACY_RATE           (4)
#define EFUSE_RATE_TBL_MASK_HT20_RATE            (0x0f)
#define EFUSE_RATE_TBL_SFT_HT20_RATE             (0)
#define EFUSE_RATE_TBL_MASK_HT40_RATE            (0x0f)
#define EFUSE_RATE_TBL_SFT_HT40_RATE             (0)

    // set b rate gain
    if (sh->flash_config.exist) {
        ssv6006_turismoC_chg_cck_bbscale(sh, sh->flash_config.rate_delta[0]);
    } else {
        if ((sh->efuse_bitmap & BIT(EFUSE_RATE_TABLE_1)) &&
                (sh->cfg.efuse_rate_gain_mask & EFUSE_RATE_GAIN_MASK_CCK)) {

            value = ((sh->cfg.rate_table_1 & EFUSE_RATE_TBL_MASK_B_RATE) >> EFUSE_RATE_TBL_SFT_B_RATE);
            if (sh->cfg.rf_b_rate_offset != 0)
            {
                if (((value + sh->cfg.rf_b_rate_offset) >= 0) && ((value + sh->cfg.rf_b_rate_offset) < SIZE_B_RATE_GAIN_TBL))
                {
                    value += sh->cfg.rf_b_rate_offset;
                    printk("rate gain b gain offset %d, value %d\n", sh->cfg.rf_b_rate_offset, value);
                }
            }
            if (value < SIZE_B_RATE_GAIN_TBL) 
                ssv6006_turismoC_chg_cck_bbscale(sh, b_rate_gain_tbl[value]);
        }
        else
        {
            if (sh->cfg.rf_b_rate_offset != 0)
            {
                value = sh->rf_table.rate_gain_b;
                if (((value + sh->cfg.rf_b_rate_offset) >= 0) && ((value + sh->cfg.rf_b_rate_offset) < MAX_RATE_GAIN_NUM))
                {
                    sh->rf_table.rate_gain_b += sh->cfg.rf_b_rate_offset;
                    printk("rate gain b gain offset %d, value %d\n", sh->cfg.rf_b_rate_offset, sh->rf_table.rate_gain_b);
                }
            }
            
            printk("rate gain b use default value\n");
        }
    }

    // set legacy rate gain
    if (sh->flash_config.exist) {
        ssv6006_turismoC_chg_legacy_bbscale(sh, 
                sh->flash_config.rate_delta[1],  
                sh->flash_config.rate_delta[2],
                sh->flash_config.rate_delta[3], 
                sh->flash_config.rate_delta[4]);
    } else {
        if ((sh->efuse_bitmap & BIT(EFUSE_RATE_TABLE_2)) && 
                (sh->cfg.efuse_rate_gain_mask & EFUSE_RATE_GAIN_MASK_LEGACY)) {

            value = ((sh->cfg.rate_table_2 & EFUSE_RATE_TBL_MASK_LEGACY_RATE) >> EFUSE_RATE_TBL_SFT_LEGACY_RATE);
            if (sh->cfg.rf_g_rate_offset != 0) {

                if (((value + sh->cfg.rf_g_rate_offset) >= 0) && 
                    ((value + sh->cfg.rf_g_rate_offset) <= MAX_OFDM_RATE_GAIN_INDEX)) {

                    value += sh->cfg.rf_g_rate_offset;
                    printk("rate gain g gain offset %d, value %d\n", sh->cfg.rf_g_rate_offset, value);
                }
            }
            
            if (value <= MAX_OFDM_RATE_GAIN_INDEX) {
                ssv6006_turismoC_chg_legacy_bbscale(sh,
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_BPSK_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_QPSK_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_16QAM_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_64QAM_OFFSET]);
                printk("rate gain legacy use value %d\n", value);
            } else
                printk("incorrect rate gain legacy index value %d\n", value);
        } else {

            if (sh->cfg.rf_g_rate_offset != 0) {

                value = sh->rf_table.rate_config_g.rate1;
                if (((value + sh->cfg.rf_g_rate_offset) >= 0) && 
                    ((value + sh->cfg.rf_g_rate_offset) < (MAX_RATE_GAIN_NUM-OFDM_RATE_GAIN_64QAM_OFFSET))) {

                    value += sh->cfg.rf_g_rate_offset;
                    printk("rate gain g gain offset %d, value %d\n", sh->cfg.rf_g_rate_offset, value);
                    sh->rf_table.rate_config_g.rate1 = value+OFDM_RATE_GAIN_BPSK_OFFSET;                    
                    sh->rf_table.rate_config_g.rate2 = value+OFDM_RATE_GAIN_QPSK_OFFSET;                    
                    sh->rf_table.rate_config_g.rate3 = value+OFDM_RATE_GAIN_16QAM_OFFSET;                    
                    sh->rf_table.rate_config_g.rate4 = value+OFDM_RATE_GAIN_64QAM_OFFSET;                    
                }
            }
            printk("rate gain legacy use default value\n");
        }
    }

    // set ht20 rate gain
    if (sh->flash_config.exist) {
        ssv6006_turismoC_chg_ht20_bbscale(sh, 
                sh->flash_config.rate_delta[5],  
                sh->flash_config.rate_delta[6],
                sh->flash_config.rate_delta[7], 
                sh->flash_config.rate_delta[8]);
    } else {
        if ((sh->efuse_bitmap & BIT(EFUSE_RATE_TABLE_2)) &&
                (sh->cfg.efuse_rate_gain_mask & EFUSE_RATE_GAIN_MASK_HT20)) {

            value = ((sh->cfg.rate_table_2 & EFUSE_RATE_TBL_MASK_HT20_RATE) >> EFUSE_RATE_TBL_SFT_HT20_RATE);
            if (sh->cfg.rf_ht20_rate_offset != 0) {
                if (((value + sh->cfg.rf_ht20_rate_offset) >= 0) && 
                    ((value + sh->cfg.rf_ht20_rate_offset) <= MAX_OFDM_RATE_GAIN_INDEX)) {

                    value += sh->cfg.rf_ht20_rate_offset;
                    printk("rate gain ht20 gain offset %d\n", sh->cfg.rf_ht20_rate_offset);
                }
            }
            if (value <= MAX_OFDM_RATE_GAIN_INDEX) {
                ssv6006_turismoC_chg_ht20_bbscale(sh,
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_BPSK_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_QPSK_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_16QAM_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_64QAM_OFFSET]);
                printk("rate gain ht20 use value %d\n", value);
            } else
                printk("incorrect rate gain ht20 index value %d\n", value);
        } else {
            if (sh->cfg.rf_ht20_rate_offset != 0) {
                
                value = sh->rf_table.rate_config_20n.rate1;
                if ((value + sh->cfg.rf_ht20_rate_offset) >= 0 && 
                    ((value + sh->cfg.rf_ht20_rate_offset) < (MAX_RATE_GAIN_NUM-OFDM_RATE_GAIN_64QAM_OFFSET))) {
                    
                    value += sh->cfg.rf_ht20_rate_offset;
                    printk("rate gain ht20 gain offset %d, value %d\n", sh->cfg.rf_ht20_rate_offset, value);
                    sh->rf_table.rate_config_20n.rate1 = value+OFDM_RATE_GAIN_BPSK_OFFSET;                    
                    sh->rf_table.rate_config_20n.rate2 = value+OFDM_RATE_GAIN_QPSK_OFFSET;                    
                    sh->rf_table.rate_config_20n.rate3 = value+OFDM_RATE_GAIN_16QAM_OFFSET;                    
                    sh->rf_table.rate_config_20n.rate4 = value+OFDM_RATE_GAIN_64QAM_OFFSET;                    
                }
            }
            printk("rate gain ht20 use default value\n");
        }
    }

    // set ht40 rate gain
    if (sh->flash_config.exist) {
        ssv6006_turismoC_chg_ht40_bbscale(sh, 
                sh->flash_config.rate_delta[9],  
                sh->flash_config.rate_delta[10],
                sh->flash_config.rate_delta[11], 
                sh->flash_config.rate_delta[12]);
    } else {
        if ((sh->efuse_bitmap & BIT(EFUSE_RATE_TABLE_1)) &&
                (sh->cfg.efuse_rate_gain_mask & EFUSE_RATE_GAIN_MASK_HT40)) {

            value = ((sh->cfg.rate_table_1 & EFUSE_RATE_TBL_MASK_HT40_RATE) >> EFUSE_RATE_TBL_SFT_HT40_RATE);
            if (sh->cfg.rf_ht40_rate_offset != 0) {
                if (((value + sh->cfg.rf_ht40_rate_offset) >= 0) && 
                    ((value + sh->cfg.rf_ht40_rate_offset) <= MAX_OFDM_RATE_GAIN_INDEX ))
                {
                    value += sh->cfg.rf_ht40_rate_offset;
                    printk("rate gain ht40 gain offset %d, value %d\n", sh->cfg.rf_ht40_rate_offset, value);
                }
            }
            if (value <= MAX_OFDM_RATE_GAIN_INDEX) {
                ssv6006_turismoC_chg_ht40_bbscale(sh,
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_BPSK_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_QPSK_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_16QAM_OFFSET],
                        ofdm_rate_gain_tbl[value+OFDM_RATE_GAIN_64QAM_OFFSET]);
                printk("rate gain ht40 use value %d\n", value);
            } else
                printk("incorrect rate gain ht40 index value %d\n", value);
        } else {
            if (sh->cfg.rf_ht40_rate_offset != 0) {

                value = sh->rf_table.rate_config_40n.rate1;
                if (((value + sh->cfg.rf_ht40_rate_offset) >= 0) && 
                    ((value + sh->cfg.rf_ht40_rate_offset) < (MAX_RATE_GAIN_NUM - OFDM_RATE_GAIN_64QAM_OFFSET))) {
                    
                    value += sh->cfg.rf_ht40_rate_offset;
                    printk("rate gain ht40 gain offset %d, value %d\n", sh->cfg.rf_ht40_rate_offset, value);
                    sh->rf_table.rate_config_40n.rate1 = value+OFDM_RATE_GAIN_BPSK_OFFSET;                    
                    sh->rf_table.rate_config_40n.rate2 = value+OFDM_RATE_GAIN_QPSK_OFFSET;                    
                    sh->rf_table.rate_config_40n.rate3 = value+OFDM_RATE_GAIN_16QAM_OFFSET;                    
                    sh->rf_table.rate_config_40n.rate4 = value+OFDM_RATE_GAIN_64QAM_OFFSET;                    
                }
            }
            printk("rate gain ht40 use default value\n");
        }
    }
}

static void ssv6006_turismoC_chg_dpd_bbscale(struct ssv_hw *sh)
{
    int i = 0, value = 0;
    int band_gain_tbl[] = {0x48, 0x4c, 0x51, 0x56, 0x5b, 0x60, 0x66, 0x6c,
        0x72, 0x79, 0x80, 0x88, 0x90, 0x98, 0xa1, 0xab};

#define MAX_2G_BAND_GAIN    7
#define SIZE_BAND_GAIN_TBL   (sizeof(band_gain_tbl) / sizeof((band_gain_tbl)[0]))

    // set 2.4g band
    if (sh->flash_config.exist) {
        for (i = 0; i < MAX_2G_BAND_GAIN; i++) {
            sh->rf_table.rt_config.band_gain[i] = rf_table_get_band_gain_idx_by_value(sh->flash_config.rt_config.g_band_gain[i]);
            sh->rf_table.lt_config.band_gain[i] = rf_table_get_band_gain_idx_by_value(sh->flash_config.lt_config.g_band_gain[i]);
            sh->rf_table.ht_config.band_gain[i] = rf_table_get_band_gain_idx_by_value(sh->flash_config.ht_config.g_band_gain[i]);
        }
    } else if (sh->efuse_bitmap & BIT(EFUSE_TX_POWER_INDEX_1)) {
        value = ((sh->cfg.tx_power_index_1 & 0x0f) >> 0);
        if (sh->cfg.rf_band_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band_gain_offset) >= 0) && ((value + sh->cfg.rf_band_gain_offset) <= 15 ))
            {
                value += sh->cfg.rf_band_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band_gain_offset, value);
            }
        }
        
        printk("band gain use %d\n", value);
        for (i = 0; i < MAX_2G_BAND_GAIN; i++) {
            sh->rf_table.rt_config.band_gain[i] = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
            sh->rf_table.lt_config.band_gain[i] = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
            sh->rf_table.ht_config.band_gain[i] = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        }
    } else {
        value = sh->rf_table.rt_config.band_gain[0];
        if (sh->cfg.rf_band_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band_gain_offset) >= 0) && ((value + sh->cfg.rf_band_gain_offset) <= 15))
            {
                value += sh->cfg.rf_band_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band_gain_offset, value);
                for (i = 0; i < MAX_2G_BAND_GAIN; i++) {
                    sh->rf_table.rt_config.band_gain[i] = value;
                    sh->rf_table.lt_config.band_gain[i] = value;
                    sh->rf_table.ht_config.band_gain[i] = value;
                }
            }
        }
        printk("band gain use default value\n");
    }

    //set band 5100
    if (sh->flash_config.exist) {
        sh->rf_table.rt_5g_config.bbscale_band0 = rf_table_get_band_gain_idx_by_value(sh->flash_config.rt_config.a_band_gain[0]);
        sh->rf_table.lt_5g_config.bbscale_band0 = rf_table_get_band_gain_idx_by_value(sh->flash_config.lt_config.a_band_gain[0]);
        sh->rf_table.ht_5g_config.bbscale_band0 = rf_table_get_band_gain_idx_by_value(sh->flash_config.ht_config.a_band_gain[0]);
    }

    //set band 5500
    if (sh->flash_config.exist) {
        sh->rf_table.rt_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(sh->flash_config.rt_config.a_band_gain[1]);
        sh->rf_table.lt_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(sh->flash_config.lt_config.a_band_gain[1]);
        sh->rf_table.ht_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(sh->flash_config.ht_config.a_band_gain[1]);
    } else if (sh->efuse_bitmap & BIT(EFUSE_TX_POWER_INDEX_1)) {
        value = ((sh->cfg.tx_power_index_1 & 0xf0) >> 4);
        if (sh->cfg.rf_band5500_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band5500_gain_offset) >= 0) && ((value + sh->cfg.rf_band5500_gain_offset) <= 15 ))
            {
                value += sh->cfg.rf_band5500_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band5500_gain_offset, value);
            }
        }
        
        printk("band gain use %d\n", value);
        sh->rf_table.rt_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        sh->rf_table.lt_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        sh->rf_table.ht_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
    } else {
        value = sh->rf_table.rt_5g_config.bbscale_band1;
        if (sh->cfg.rf_band5500_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band5500_gain_offset) >= 0) && ((value + sh->cfg.rf_band5500_gain_offset) <= 15))
            {
                value += sh->cfg.rf_band5500_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band5500_gain_offset, value);
                sh->rf_table.rt_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
                sh->rf_table.lt_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
                sh->rf_table.ht_5g_config.bbscale_band1 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
            }
        }
        printk("band5500 gain use default value\n");
    }

    // set band 5700
    if (sh->flash_config.exist) {
        sh->rf_table.rt_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(sh->flash_config.rt_config.a_band_gain[2]);
        sh->rf_table.lt_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(sh->flash_config.lt_config.a_band_gain[2]);
        sh->rf_table.ht_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(sh->flash_config.ht_config.a_band_gain[2]);
    } else if (sh->efuse_bitmap & BIT(EFUSE_TX_POWER_INDEX_2)) {
        value = ((sh->cfg.tx_power_index_2 & 0x0f) >> 0);
        if (sh->cfg.rf_band5700_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band5700_gain_offset) >= 0) && ((value + sh->cfg.rf_band5700_gain_offset) <= 15 ))
            {
                value += sh->cfg.rf_band5700_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band5700_gain_offset, value);
            }
        }
        
        printk("band gain use %d\n", value);
        sh->rf_table.rt_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        sh->rf_table.lt_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        sh->rf_table.ht_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
    } else {
        value = sh->rf_table.rt_5g_config.bbscale_band2;
        if (sh->cfg.rf_band5700_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band5700_gain_offset) >= 0) && ((value + sh->cfg.rf_band5700_gain_offset) <= 15))
            {
                value += sh->cfg.rf_band5700_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band5700_gain_offset, value);
                sh->rf_table.rt_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
                sh->rf_table.lt_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
                sh->rf_table.ht_5g_config.bbscale_band2 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
            }
        }
        printk("band5700 gain use default value\n");
    }

    // set band 5900
    if (sh->flash_config.exist) {
        sh->rf_table.rt_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(sh->flash_config.rt_config.a_band_gain[3]);
        sh->rf_table.lt_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(sh->flash_config.lt_config.a_band_gain[3]);
        sh->rf_table.ht_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(sh->flash_config.ht_config.a_band_gain[3]);
    } else if (sh->efuse_bitmap & BIT(EFUSE_TX_POWER_INDEX_2)) {
        value = ((sh->cfg.tx_power_index_2 & 0xf0) >> 4);
        if (sh->cfg.rf_band5900_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band5900_gain_offset) >= 0) && ((value + sh->cfg.rf_band5900_gain_offset) <= 15 ))
            {
                value += sh->cfg.rf_band5900_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band5900_gain_offset, value);
            }
        }
        
        printk("band gain use %d\n", value);
        sh->rf_table.rt_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        sh->rf_table.lt_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
        sh->rf_table.ht_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
    } else {
        value = sh->rf_table.rt_5g_config.bbscale_band3;
        if (sh->cfg.rf_band5900_gain_offset != 0)
        {
            if (((value + sh->cfg.rf_band5900_gain_offset) >= 0) && ((value + sh->cfg.rf_band5900_gain_offset) <= 15))
            {
                value += sh->cfg.rf_band5900_gain_offset;
                printk("band gain offset %d, value %d\n", sh->cfg.rf_band5900_gain_offset, value);
                sh->rf_table.rt_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
                sh->rf_table.lt_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
                sh->rf_table.ht_5g_config.bbscale_band3 = rf_table_get_band_gain_idx_by_value(band_gain_tbl[value]);
            }
        }
        printk("band5900 gain use default value\n");
    }
}

static void ssv6006_turismoC_chg_external_pa(struct ssv_hw *sh)
{
#define EXTERNAL_PA_LNA_TRIGGER_MASK        0x000000FF
#define EXTERNAL_PA_LNA_TRIGGER_SHIFT       0
#define EXTERNAL_PA_LNA_ON_OFFSET_MASK      0x0000FF00
#define EXTERNAL_PA_LNA_ON_OFFSET_SHIFT     8
#define EXTERNAL_PA_LNA_OFF_OFFSET_MASK     0x00FF0000
#define EXTERNAL_PA_LNA_OFF_OFFSET_SHIFT    16

    sh->rf_table.extpa_tbl.extpa_en = (true == sh->cfg.external_pa) ? 1 : 0;
    // 2.4g lna
    sh->rf_table.extpa_tbl.lna_trigger_2G = 
            ((sh->cfg.external_lna_2g & EXTERNAL_PA_LNA_TRIGGER_MASK) >> EXTERNAL_PA_LNA_TRIGGER_SHIFT);
    sh->rf_table.extpa_tbl.rssi_lna_on_offset = 
            ((sh->cfg.external_lna_2g & EXTERNAL_PA_LNA_ON_OFFSET_MASK) >> EXTERNAL_PA_LNA_ON_OFFSET_SHIFT);
    sh->rf_table.extpa_tbl.rssi_lna_off_offset = 
            ((sh->cfg.external_lna_2g & EXTERNAL_PA_LNA_OFF_OFFSET_MASK) >> EXTERNAL_PA_LNA_OFF_OFFSET_SHIFT);
    // 5g lna
    sh->rf_table.extpa_tbl.lna_trigger_5G = 
            ((sh->cfg.external_lna_5g & EXTERNAL_PA_LNA_TRIGGER_MASK) >> EXTERNAL_PA_LNA_TRIGGER_SHIFT);
    sh->rf_table.extpa_tbl.rssi_5g_lna_on_offset = 
            ((sh->cfg.external_lna_5g & EXTERNAL_PA_LNA_ON_OFFSET_MASK) >> EXTERNAL_PA_LNA_ON_OFFSET_SHIFT);
    sh->rf_table.extpa_tbl.rssi_5g_lna_off_offset = 
            ((sh->cfg.external_lna_5g & EXTERNAL_PA_LNA_OFF_OFFSET_MASK) >> EXTERNAL_PA_LNA_OFF_OFFSET_SHIFT);
}

static void ssv6006_turismoC_chg_padpd(struct ssv_hw *sh)
{
    if (sh->cfg.padpd & 0x1)
        sh->rf_table.rt_config.padpd_cali = 1;
    if (sh->cfg.padpd & 0x2)
        sh->rf_table.lt_config.padpd_cali = 1;
    if (sh->cfg.padpd & 0x4)
        sh->rf_table.ht_config.padpd_cali = 1;
}

static void ssv6006_turismoC_update_rf_table(struct ssv_hw *sh)
{
    // init rf_table
    memcpy(&sh->rf_table, &def_rf_table, sizeof(struct st_rf_table));

    ssv6006_turismoC_chg_xtal_freq_offset(sh);
    ssv6006_turismoC_chg_bbscale(sh);
    ssv6006_turismoC_chg_dpd_bbscale(sh);
    ssv6006_turismoC_chg_external_pa(sh);
    ssv6006_turismoC_chg_padpd(sh);
}

void ssv6006c_update_edcca(struct ssv_hw *sh, bool val)
{
    if ((sh->sc->hw_chan_type == NL80211_CHAN_HT20)||
            (sh->sc->hw_chan_type == NL80211_CHAN_NO_HT)) {
        if (val) {
            SET_RG_AGC_THRESHOLD(EDCCA_HIGH);
        } else {
            SET_RG_AGC_THRESHOLD(EDCCA_LOW);
        }
    } else {
        if (val) {
            SET_RG_AGC_THRESHOLD(EDCCA_HIGH_HT40);
        } else {
            SET_RG_AGC_THRESHOLD(EDCCA_LOW_HT40);
        }
    }
    sh->sc->default_edcca = GET_RG_AGC_THRESHOLD;
}

#ifdef CONFIG_ENABLE_HOST_THERMAL
static int ssv6006_turismoC_get_current_temperature(struct ssv_hw *sh, int *pvalue)
{
    int ret = -1;
    int i = 0;
    int value2 = 0;
    if (sh->cfg.host_thermal_debug != 0)
    {
        printk("host_thermal_debug : [temp] = %d\n", sh->cfg.host_thermal_debug);
        return sh->cfg.host_thermal_debug;
    }
    
    SET_RG_SARADC_THERMAL(1);
    SET_RG_EN_SARADC(1);
    for (i = 0; i < 100;i++) {
        if (GET_SAR_ADC_FSM_RDY) {
            ret = 0;
            break;
        }
    }
    value2 = GET_DB_DA_SARADC_BIT;
    //PHY_LOG_D("SARDAC = %d\n", value2);
    if((GET_CHIP_ID_63_32 == 0x30202020) && (GET_CHIP_ID_95_64 == 0x30303643) ) 
    {
        //TurismoC, Temperature 1
        *pvalue = (value2 - 31)*2+(value2 - 31)/2;
    }
    else if(GET_CHIP_ID_95_64 == 0x30303644)
    {
        //TurismoD and TurismoD ECO, Temperature 2
        *pvalue = (value2-14)*3+(value2-14)/20;
    }
    else if( (GET_CHIP_ID_63_32 == 0x32202020) && (GET_CHIP_ID_95_64 == 0x30303643))
    {
        //TurismoC ECO, Temperature 3
        //Need implement when get new sar code table
    }
    else
    {
        //undefined , use default 
        *pvalue = (value2 - 31)*2+(value2 - 31)/2;
    }
    SET_RG_SARADC_THERMAL(0);
    SET_RG_EN_SARADC(0);
    return ret;
}

static int ssv6006_turismoC_get_current_tempe_state(struct ssv_hw *sh)
{
     int tempe_state = 1;
     int tempe = 0;
     int i = 0;
 
     if (ssv6006_turismoC_get_current_temperature(sh, &tempe) < 0)
     {
         return -1;
     }
 
     if ((sh->htc_config.hw_temp_boundary_levels < 3) || (sh->htc_config.hw_temp_boundary_levels > 8))
     {
         return -1;
     }
 
     for (i = 2; i <= sh->htc_config.hw_temp_boundary_levels; i++)
     {
         if(tempe >= sh->htc_config.hw_temp_items[i].temp)
         {
             tempe_state++;
         }
         else
         {
            break;
         }
    }
     
    printk("turismoC_current_tempe is : %d,[temp] = %d,[reg_count] = %d\n", tempe,
        sh->htc_config.hw_temp_items[tempe_state].temp,
        sh->htc_config.hw_temp_items[tempe_state].reg_count);
    return tempe_state;
}

static void ssv6006_turismoC_set_compensation_reg(struct ssv_hw *sh, int tempe_state)
{
    int i = 0, reg_count = 0;
    int normal_reg_count = 0;
    int value = 0;
    
    printk("ssv6006_turismoC_set_compensation_reg : tempe_state=%d\n",tempe_state);
    
    normal_reg_count = sh->htc_config.hw_temp_items[0].reg_count;

    for (i = 0;i < normal_reg_count; i++)
    {
        REG32_W(sh->htc_config.hw_tem_reg_items[0][i].reg, sh->htc_config.hw_tem_reg_items[0][i].vaule);
        printk("set_compensation_reg[normal]: %08X:%08X\n", sh->htc_config.hw_tem_reg_items[0][i].reg, sh->htc_config.hw_tem_reg_items[0][i].vaule);
    }
    
    reg_count = sh->htc_config.hw_temp_items[tempe_state].reg_count;
    for (i = 0;i < reg_count; i++)
    {
        if (ADR_PMU_REG_1 == sh->htc_config.hw_tem_reg_items[tempe_state][i].reg)
        {
            value = sh->htc_config.hw_tem_reg_items[tempe_state][i].vaule;
            if (value == 0xff)
            {
                value = sh->rf_table.ht_config.freq_xo;
            }
            else
            {
                value = sh->rf_table.ht_config.freq_xo + (value - 200);
            }
            
            SET_RG_XO_CBANKI(value);
            SET_RG_XO_CBANKO(value);
            printk("set_compensation_reg[%d]: SET_RG_XO_CBANKI:%08x:0x%x\n", tempe_state, sh->htc_config.hw_tem_reg_items[tempe_state][i].reg, value);
        }
        else
        {
            REG32_W(sh->htc_config.hw_tem_reg_items[tempe_state][i].reg, sh->htc_config.hw_tem_reg_items[tempe_state][i].vaule);
            printk("set_compensation_reg[%d]: %08X:%08X\n", tempe_state, sh->htc_config.hw_tem_reg_items[tempe_state][i].reg, sh->htc_config.hw_tem_reg_items[tempe_state][i].vaule);
        }
    }
}

static void ssv6006_turismoC_do_temperature_compensation(struct ssv_hw *sh)
{
    int now_tempe_state = 0;
    //printk("enter turismoC_do_temperature_compensation........\n");
    if(!sh->htc_config.exist){
        return ;
    }
    now_tempe_state = ssv6006_turismoC_get_current_tempe_state(sh);
    if(now_tempe_state<0||now_tempe_state>7){
        return ;
    }
    if ((sh->htc_config.hw_temp_state != now_tempe_state))
    {
        sh->htc_config.hw_temp_state = now_tempe_state;
        ssv6006_turismoC_set_compensation_reg(sh,now_tempe_state);
    }
}
#endif

void ssv_attach_ssv6006_turismoC_BBRF(struct ssv_hal_ops *hal_ops)
{
    hal_ops->load_phy_table = ssv6006_turismoC_load_phy_table;
    hal_ops->get_phy_table_size = ssv6006_turismoC_get_phy_table_size;
    hal_ops->load_rf_table = ssv6006_turismoC_load_rf_table;
    hal_ops->get_rf_table_size = ssv6006_turismoC_get_rf_table_size;
    hal_ops->init_pll = ssv6006_turismoC_init_PLL;
    hal_ops->set_channel = ssv6006_turismoC_set_channel;
    hal_ops->set_pll_phy_rf = ssv6006_turismoC_set_pll_phy_rf;
    hal_ops->set_rf_enable = ssv6006_turismoC_set_rf_enable;

    hal_ops->dump_phy_reg = ssv6006_turismoC_dump_phy_reg;
    hal_ops->dump_rf_reg = ssv6006_turismoC_dump_rf_reg;
    hal_ops->support_iqk_cmd = ssv6006_turismoC_support_iqk_cmd;
    hal_ops->cmd_cali = ssv6006_cmd_turismoC_cali;
    hal_ops->cmd_txgen = ssv6006_cmd_turismoC_txgen;
    hal_ops->cmd_rf = ssv6006_cmd_turismoC_rf;

    hal_ops->update_rf_table = ssv6006_turismoC_update_rf_table;
#ifdef CONFIG_ENABLE_HOST_THERMAL	
    hal_ops->do_temperature_compensation = ssv6006_turismoC_do_temperature_compensation;
    hal_ops->get_temp = ssv6006_turismoC_get_current_temperature;
#endif	
}
#endif
