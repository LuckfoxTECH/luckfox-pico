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

#include <linux/kernel.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
#include <linux/export.h>
#else
#include <linux/module.h>
#endif

#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/sched.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,15,0)
#include <linux/sched/prio.h>
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0)
#include <linux/sched/rt.h>
#endif

#include <ssv_conf_parser.h>

#include <ssv6200.h>
#include <hci/hctrl.h>
#include <smac/dev.h>
#include <smac/ssv_skb.h>
#include <hal.h>

#include "ssv_cmd.h"
#include <ssv_version.h>
#include <linux_80211.h>
#define SSV_CMD_PRINTF()


EXPORT_SYMBOL(snprintf_res);

static int ssv_cmd_help(struct ssv_softc *sc, int argc, char *argv[])
{
    extern struct ssv_cmd_table cmd_table[];
    struct ssv_cmd_table *sc_tbl;
    int total_cmd = 0;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    snprintf_res(cmd_data, "Usage:\n");
    //Skip 3 help command.
    for (sc_tbl = &cmd_table[3]; sc_tbl->cmd; sc_tbl++) {
        snprintf_res(cmd_data, "%-20s\t\t%s\n", (char*)sc_tbl->cmd, sc_tbl->usage);
        total_cmd++;
    }
    snprintf_res(cmd_data, "Total CMDs: %x\n\nType cli help [CMD] for more detail command.\n\n", total_cmd);
   
    return 0;
}

static int ssv_cmd_reg(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32 addr, value, count;
    char *endp;
    int s;
    int ret = 0;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    /* reg [r|w] [address] [value|word-count] */

    if ((argc == 4) && (strcmp(argv[1], "w") == 0)) {
        /* reg w <address> <word_value> */
        addr = simple_strtoul(argv[2], &endp, 16);
        value = simple_strtoul(argv[3], &endp, 16);
        if (SMAC_REG_WRITE(sc->sh, addr, value)) ;
        
        snprintf_res(cmd_data, " => write [0x%08x]: 0x%08x\n", addr, value);
        return 0;
#ifdef CONFIG_USB_EP0_RW_REGISTER
    } else if ((argc == 4) && (strcmp(argv[1], "w2") == 0)) {
        /* reg w <address> <word_value> */
        addr = simple_strtoul(argv[2], &endp, 16);
        value = simple_strtoul(argv[3], &endp, 16);
        if (SMAC_REG_MCU_WRITE(sc->sh, addr, value)) ;
        
        snprintf_res(cmd_data, " => safe write [0x%08x]: 0x%08x\n", addr, value);
        return 0;
#endif
    } else if (((argc == 4) || (argc == 3)) && (strcmp(argv[1], "r") == 0)) {
        /* reg r <address> <word_count> */
        count = (argc ==3 )? 1: simple_strtoul(argv[3], &endp, 10);
        addr = simple_strtoul(argv[2], &endp, 16);
        snprintf_res(cmd_data, "ADDRESS: 0x%08x\n", addr);
        
        for(s = 0; s < count; s++, addr += 4) {
            if (SMAC_REG_READ(sc->sh, addr, &value));
            
            snprintf_res(cmd_data, "%08x ", value);
 
            if (((s+1) & 0x07) == 0){
                snprintf_res(cmd_data, "\n");
            }
        }
        snprintf_res(cmd_data, "\n");
        return 0;
#ifdef CONFIG_USB_EP0_RW_REGISTER
    } else if (((argc == 4) || (argc == 3)) && (strcmp(argv[1], "r2") == 0)) {
        /* reg r <address> <word_count> */
        count = (argc ==3 )? 1: simple_strtoul(argv[3], &endp, 10);
        addr = simple_strtoul(argv[2], &endp, 16);
        snprintf_res(cmd_data, "ADDRESS: 0x%08x\n", addr);
        
        for(s = 0; s < count; s++, addr += 4) {
            if (SMAC_REG_MCU_READ(sc->sh, addr, &value));
            
            snprintf_res(cmd_data, "%08x ", value);
 
            if (((s+1) & 0x07) == 0){
                snprintf_res(cmd_data, "\n");
            }
        }
        snprintf_res(cmd_data, "\n");
        return 0;
#endif
    } else if (argc == 5 && strcmp(argv[1], "bw")==0) {
        u32 addr_list[8],value_list[8];
        /* reg bw <address_list> <word_value_list> <word_count> */
        addr = simple_strtoul(argv[2], &endp, 16);
        value = simple_strtoul(argv[3], &endp, 16);
        count = simple_strtoul(argv[4], &endp, 16); //amount of access register
        
        //write same value to sequential registers
        for (s=0; s<count; s++) {
            addr_list[s] = addr+4*s;
            value_list[s] = value;
        }		

        ret = HAL_BURST_WRITE_REG(sc->sh, addr_list, value_list, count);
        if (ret >= 0) {
            snprintf_res(cmd_data, "  ==> write done.\n");
            return 0;
        } else if (ret == -EOPNOTSUPP) {
            snprintf_res(cmd_data, "Does not support this command!\n");
            return 0;
        }
      
    } else if (argc == 4 && strcmp(argv[1], "br")==0) {
        /* reg br <address_list> <word_count> */        
        u32 addr_list[8],value_list[8];
        addr = simple_strtoul(argv[2], &endp, 16);
        count = simple_strtoul(argv[3], &endp, 16); 
        
        //read from seqential registers
        for (s=0; s<count; s++)
            addr_list[s] = addr+4*s;

        ret = HAL_BURST_READ_REG(sc->sh, addr_list, value_list, count);
        if (ret >= 0) {
            snprintf_res(cmd_data, "ADDRESS:   0x%x\n", addr);
            snprintf_res(cmd_data, "REG-COUNT: %d\n", count);
            for (s=0; s<count; s++)            
                snprintf_res(cmd_data, "addr %x ==> %x\n", addr_list[s], value_list[s]);
            return 0;
        } else if (ret == -EOPNOTSUPP) {
            snprintf_res(cmd_data, "Does not support this command!\n");
            return 0;
        }
        
    } 
	else {
        snprintf_res(cmd_data, "reg [r|w] [address] [value|word-count]\n\n");
        snprintf_res(cmd_data, "reg [br] [address] [word-count]\n\n");
        snprintf_res(cmd_data, "reg [bw] [address] [value] [word-count]\n\n");
        return 0;

    }
    return -1;
#else
    return 0;
#endif
}

struct ssv6xxx_cfg ssv_cfg;
EXPORT_SYMBOL(ssv_cfg);

#if 0
static int __string2s32(u8 *val_str, void *val)
{
    char *endp;
    int base=10;
    if (val_str[0]=='0' && ((val_str[1]=='x')||(val_str[1]=='X')))
        base = 16;
    *(int *)val = simple_strtoul(val_str, &endp, base);
    return 0;
}
#endif

static int __string2bool(u8 *u8str, void *val, u32 arg)
{
    char *endp;
	*(u8 *)val = !!simple_strtoul(u8str, &endp, 10);
    return 0;
}

static int __string2u32(u8 *u8str, void *val, u32 arg)
{
    char *endp;
    int base=10;
    if (u8str[0]=='0' && ((u8str[1]=='x')||(u8str[1]=='X')))
        base = 16;
    *(u32 *)val = simple_strtoul(u8str, &endp, base);    
    return 0;
}

static int __string2s32(u8 *u8str, void *val, u32 arg)
{
    char *endp;
    int base=10;
    *(s32 *)val = simple_strtol(u8str, &endp, base);
    return 0;
}

static int __string2flag32(u8 *flag_str, void *flag, u32 arg)
{
    u32 *val=(u32 *)flag;

    if (arg >= (sizeof(u32)<<3))
        return -1;
    if (strcmp(flag_str, "on")==0) {
        *val |= (1<<arg);
        
        return 0;
    }
    if (strcmp(flag_str, "off")==0) {
        *val &= ~(1<<arg); 
        return 0;
    }
    return -1;
}

static int __string2mac(u8 *mac_str, void *val, u32 arg)
{
    int s, macaddr[6];
    u8 *mac=(u8 *)val;

    s = sscanf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", 
        &macaddr[0], &macaddr[1], &macaddr[2],
        &macaddr[3], &macaddr[4], &macaddr[5]);
    if (s != 6)
        return -1;
    mac[0] = (u8)macaddr[0], mac[1] = (u8)macaddr[1];
    mac[2] = (u8)macaddr[2], mac[3] = (u8)macaddr[3];
    mac[4] = (u8)macaddr[4], mac[5] = (u8)macaddr[5];
    return 0;
}

static int __string2str(u8 *path, void *val, u32 arg)
{
    u8 *temp=(u8 *)val;

    sprintf(temp,"%s",path);

    return 0;
}

static int __string2configuration(u8 *mac_str, void *val, u32 arg)
{
    unsigned int address,value;
    int i;

    i = sscanf(mac_str, "%08x:%08x", &address, &value);
    if (i != 2)
        return -1;

    for(i=0; i<EXTERNEL_CONFIG_SUPPORT; i++)
    {
        if(ssv_cfg.configuration[i][0] == 0x0)
        {
            ssv_cfg.configuration[i][0] = address;
            ssv_cfg.configuration[i][1] = value;
            return 0;
        }
    }

    return 0;
}

// Note: if there is no default value, set default to NULL, and it will be initialized as zero
struct ssv6xxx_cfg_cmd_table cfg_cmds[] = {
    { "hw_mac",               (void *)&ssv_cfg.maddr[0][0],            0,  __string2mac     , NULL},
    { "hw_mac_2",             (void *)&ssv_cfg.maddr[1][0],            0,  __string2mac     , NULL},
    { "def_chan",             (void *)&ssv_cfg.def_chan,               0,  __string2u32     , "6"},
    { "hw_cap_ht",            (void *)&ssv_cfg.hw_caps,                0,  __string2flag32  , "on"},
    { "hw_cap_gf",            (void *)&ssv_cfg.hw_caps,                1,  __string2flag32  , "off"},
    { "hw_cap_2ghz",          (void *)&ssv_cfg.hw_caps,                2,  __string2flag32  , "on"},
    { "hw_cap_5ghz",          (void *)&ssv_cfg.hw_caps,                3,  __string2flag32  , "off"},
    { "hw_cap_security",      (void *)&ssv_cfg.hw_caps,                4,  __string2flag32  , "on"},
    { "hw_cap_sgi",           (void *)&ssv_cfg.hw_caps,                5,  __string2flag32  , "on"},
    { "hw_cap_ht40",          (void *)&ssv_cfg.hw_caps,                6,  __string2flag32  , "on"},
    { "hw_cap_ap",            (void *)&ssv_cfg.hw_caps,                7,  __string2flag32  , "on"},
    { "hw_cap_p2p",           (void *)&ssv_cfg.hw_caps,                8,  __string2flag32  , "on"},
    { "hw_cap_ampdu_rx",      (void *)&ssv_cfg.hw_caps,                9,  __string2flag32  , "on"},
    { "hw_cap_ampdu_tx",      (void *)&ssv_cfg.hw_caps,               10,  __string2flag32  , "on"},
    { "hw_cap_tdls",          (void *)&ssv_cfg.hw_caps,               11,  __string2flag32  , "off"},
    { "hw_cap_stbc",          (void *)&ssv_cfg.hw_caps,               12,  __string2flag32  , "on"},
    { "hw_cap_hci_rx_aggr",   (void *)&ssv_cfg.hw_caps,               13,  __string2flag32  , "on"},
    { "hw_cap_beacon",        (void *)&ssv_cfg.hw_caps,               14,  __string2flag32  , "off"},
	{ "hw_cap_krack",         (void *)&ssv_cfg.hw_caps,               15,  __string2flag32  , "off"},       
	{ "hw_cap_wow",           (void *)&ssv_cfg.hw_caps,               16,  __string2flag32  , "on"},
	{ "hw_cap_bq4",           (void *)&ssv_cfg.hw_caps,               17,  __string2flag32  , "off"},
	{ "hw_cap_hci_tx_aggr",   (void *)&ssv_cfg.hw_caps,               18,  __string2flag32  , "on"},
    { "hw_cap_report_tx_ack", (void *)&ssv_cfg.hw_caps,               19,  __string2flag32  , "off"},
    { "use_wpa2_only",        (void *)&ssv_cfg.use_wpa2_only,          0,  __string2u32     , NULL},
    { "wifi_tx_gain_level_gn",(void *)&ssv_cfg.wifi_tx_gain_level_gn,  0,  __string2u32     , NULL},
    { "wifi_tx_gain_level_b", (void *)&ssv_cfg.wifi_tx_gain_level_b,   0,  __string2u32     , NULL},
    { "xtal_clock",           (void *)&ssv_cfg.crystal_type,           0,  __string2u32     , "24"},
    { "volt_regulator",       (void *)&ssv_cfg.volt_regulator,         0,  __string2u32     , "1"},
    { "firmware_path",        (void *)&ssv_cfg.firmware_path[0],       0,  __string2str     , NULL},
    { "flash_bin_path",       (void *)&ssv_cfg.flash_bin_path[0],      0,  __string2str     , NULL},
    { "mac_address_path",     (void *)&ssv_cfg.mac_address_path[0],    0,  __string2str     , NULL},
    { "mac_output_path",      (void *)&ssv_cfg.mac_output_path[0],     0,  __string2str     , NULL},
    { "ignore_efuse_mac",     (void *)&ssv_cfg.ignore_efuse_mac,       0,  __string2u32     , NULL},
    { "efuse_rate_gain_mask", (void *)&ssv_cfg.efuse_rate_gain_mask,   0,  __string2u32     , "0x1"},
    { "mac_address_mode",     (void *)&ssv_cfg.mac_address_mode,       0,  __string2u32     , NULL},
    { "register",             NULL,                                    0,  __string2configuration,  NULL},
    { "beacon_rssi_minimal",  (void *)&ssv_cfg.beacon_rssi_minimal,    0,  __string2u32     , NULL},

    { "force_chip_identity",  (void *)&ssv_cfg.force_chip_identity,    0,  __string2u32     , NULL},
    { "external_firmware_name", (void *)&ssv_cfg.external_firmware_name[0], 0,  __string2str, NULL},

    { "ignore_firmware_version",  (void *)&ssv_cfg.ignore_firmware_version, 0,  __string2u32, NULL},
    { "use_sw_cipher",        (void *)&ssv_cfg.use_sw_cipher,          0,  __string2u32,      NULL},

    { "auto_rate_enable",     (void *)&ssv_cfg.auto_rate_enable,       0,  __string2u32     , "1"},
    // rc_rate_idx_set combine 4 rate idx into u32
    // , byte 0 should be for series0, byte 1 for series 1 ....
    //  default set all 4 series to mcs7
    { "rc_rate_idx_set",      (void *)&ssv_cfg.rc_rate_idx_set,        0,  __string2u32     , "0x7777"},
    // rc_rate_idx_set combine 4 retry set into u32
    // , byte 0 should be for series0, byte 1 for series 1 ....
    // default only set serires retry to and disable other series.    
    { "rc_retry_set",         (void *)&ssv_cfg.rc_retry_set,           0,  __string2u32     , "0x4444"},
    { "rc_mf",                (void *)&ssv_cfg.rc_mf,                  0,  __string2u32     , NULL},
    { "rc_long_short",        (void *)&ssv_cfg.rc_long_short,          0,  __string2u32     , NULL},
    { "rc_ht40",              (void *)&ssv_cfg.rc_ht40,                0,  __string2u32     , NULL},
    { "rc_phy_mode" ,         (void *)&ssv_cfg.rc_phy_mode,            0,  __string2u32     , "3"},
    // tx page
	{ "tx_page_threshold" ,   (void *)&ssv_cfg.tx_page_threshold,      0,  __string2u32     , "160"},
	{ "max_rx_aggr_size",     (void *)&ssv_cfg.max_rx_aggr_size,       0,  __string2u32     , "64"},
	{ "ampdu_pkt_max_size",   (void *)&ssv_cfg.ampdu_pkt_max_size,     0,  __string2u32     , "23040"},
	{ "online_reset",         (void *)&ssv_cfg.online_reset,           0,  __string2u32     , "0x00f"},
    { "hw_rx_agg_cnt",        (void *)&ssv_cfg.hw_rx_agg_cnt,          0,  __string2u32     , "3"},
	{ "hw_rx_agg_method_3",   (void *)&ssv_cfg.hw_rx_agg_method_3,     0,  __string2bool    , "0"},
	{ "hw_rx_agg_timer_reload", (void *)&ssv_cfg.hw_rx_agg_timer_reload,    0, __string2u32 , "20"},
	{ "rx_max_recv_cnt",      (void *)&ssv_cfg.rx_max_recv_cnt,        0,  __string2u32     , "2"},
    { "usb_hw_resource",      (void *)&ssv_cfg.usb_hw_resource,        0,  __string2u32     , "0"},
	{ "tx_stuck_detect",      (void *)&ssv_cfg.tx_stuck_detect,        0,  __string2bool    , "0"},
	{ "clk_src_80m",          (void *)&ssv_cfg.clk_src_80m,            0,  __string2bool    , "1"},
	{ "rts_thres_len",        (void *)&ssv_cfg.rts_thres_len,          0,  __string2u32     , "0"},
    { "bk_txq_size",          (void *)&ssv_cfg.bk_txq_size,            0,  __string2u32     , "6"},
    { "be_txq_size",          (void *)&ssv_cfg.be_txq_size,            0,  __string2u32     , "10"},
    { "vi_txq_size",          (void *)&ssv_cfg.vi_txq_size,            0,  __string2u32     , "10"},
    { "vo_txq_size",          (void *)&ssv_cfg.vo_txq_size,            0,  __string2u32     , "8"},
    { "manage_txq_size",      (void *)&ssv_cfg.manage_txq_size,        0,  __string2u32     , "8"},
    { "autosgi",              (void *)&ssv_cfg.auto_sgi,               0,  __string2u32     , "0x1"}, 
    { "rx_threshold",         (void *)&ssv_cfg.rx_threshold,           0,  __string2u32     , "256"},
    { "flowctl",              (void *)&ssv_cfg.flowctl,                0,  __string2bool    , "1"},
    { "flowctl_low_threshold",(void *)&ssv_cfg.flowctl_low_threshold,  0,  __string2u32     , "96"},
    { "flowctl_high_threshold",(void *)&ssv_cfg.flowctl_high_threshold,0,  __string2u32     , "128"},
    { "ctry",                 (void *)&ssv_cfg.ctry,                   0,  __string2str     , NULL},
    { "external_pa",          (void *)&ssv_cfg.external_pa,            0,  __string2bool    , "0"},
    { "external_lna_2g",      (void *)&ssv_cfg.external_lna_2g,        0,  __string2u32     , "0x00080d00"},
    { "external_lna_5g",      (void *)&ssv_cfg.external_lna_5g,        0,  __string2u32     , "0x000c0801"},    
	{ "auto_edcca",           (void *)&ssv_cfg.auto_edcca,             0,  __string2bool    , "1"},                
	{ "rx_ip_align",          (void *)&ssv_cfg.rx_ip_align,            0,  __string2bool    , "1"},
	{ "rx_ip_csum_off",       (void *)&ssv_cfg.rx_ip_csum_off,         0,  __string2bool    , "1"},
    { "rx_non_preempt_num",   (void *)&ssv_cfg.rx_non_preempt_num,     0,  __string2u32     , "32"},
    { "directly_ack_ctrl",    (void *)&ssv_cfg.directly_ack_ctrl,      0,  __string2bool    , "1"},
    { "powermode",            (void *)&ssv_cfg.powermode,              0,  __string2u32     , "4"},
    { "lp_keepalive",         (void *)&ssv_cfg.lp_keepalive,           0,  __string2u32     , "30000"},
    { "lp_userdtim",          (void *)&ssv_cfg.lp_userdtim,            0,  __string2u32     , "0"},
    { "lp_wakeuppin",         (void *)&ssv_cfg.lp_wakeuppin,           0,  __string2u32     , "1"},
    { "lp_ignoredisconnection",(void *)&ssv_cfg.lp_ignoredisconnection,0,  __string2u32     , "0"},
    { "lp_dataretx_guard",    (void *)&ssv_cfg.lp_dataretx_guard,      0,  __string2u32     , "0"},
    /* lp_beaconloss_guard must be more than lp_beaconloss_maxcnt 
     * lp_beaconloss_guard fw default value 20
     * lp_beaconloss_maxcnt fw default value 10
     */
    { "lp_beaconloss_guard",  (void *)&ssv_cfg.lp_beaconloss_guard,    0,  __string2u32     , "0"},
    { "lp_beaconloss_maxcnt", (void *)&ssv_cfg.lp_beaconloss_maxcnt,    0,  __string2u32     , "0"},
    { "wmm_follow_vo",        (void *)&ssv_cfg.wmm_follow_vo,          0,  __string2bool    , "1"},
    { "backoff_enable",       (void *)&ssv_cfg.backoff_enable,         0,  __string2bool    , "0"},
    { "backoff_value",        (void *)&ssv_cfg.backoff_value,          0,  __string2u32    , "0"},
    { "rc_mask",              (void *)&ssv_cfg.rc_mask      ,          0,  __string2u32     , "0xFED"},
    { "scan_hc_period",       (void *)&ssv_cfg.scan_hc_period,         0,  __string2u32     , "100"},
    { "scan_oc_period",       (void *)&ssv_cfg.scan_oc_period,         0,  __string2u32     , "50"},
    { "fw_reset",             (void *)&ssv_cfg.fw_reset,               0,  __string2bool    , "1"},
    { "fw_status_idle_cnt",   (void *)&ssv_cfg.fw_status_idle_cnt,     0,  __string2u32     , "20"},
    { "fw_tx_waitnum",        (void *)&ssv_cfg.fw_tx_waitnum,          0,  __string2u32     , "0"},
    { "fw_tx_chkhwqnum",      (void *)&ssv_cfg.fw_tx_chkhwqnum,        0,  __string2u32     , "0"},
    { "fw_tx_duration",       (void *)&ssv_cfg.fw_tx_duration,         0,  __string2u32     , "0"},
    { "fw_tx_duration_period",(void *)&ssv_cfg.fw_tx_duration_period,  0,  __string2u32     , "2"},
    { "ampdu_rx_size_cap",    (void *)&ssv_cfg.ampdu_rx_size_cap,      0,  __string2u32     , "1"},
    { "sleep_gpio_ctrl",      (void *)&ssv_cfg.sleep_gpio_ctrl,        0,  __string2u32     , "0x00028018"},
    { "dormant_gpio_ctrl",    (void *)&ssv_cfg.dormant_gpio_ctrl,      0,  __string2u32     , "0x0002b718"},
    { "hci_trigger_en",       (void *)&ssv_cfg.hci_trigger_en,         0,  __string2u32     , "0"},
    { "hci_trigger_qlen",     (void *)&ssv_cfg.hci_trigger_qlen,       0,  __string2u32     , "10"},
    { "hci_trigger_pkt_size", (void *)&ssv_cfg.hci_trigger_pkt_size,   0,  __string2u32     , "80"},
    { "hci_task_timeout",     (void *)&ssv_cfg.hci_task_timeout,       0,  __string2u32     , "3"},
    { "disable_fw_thermal",   (void *)&ssv_cfg.disable_fw_thermal,     0,  __string2u32     , "0"},
    { "rf_band_gain_offset",  (void *)&ssv_cfg.rf_band_gain_offset,    0,  __string2s32     , "0"},
    { "rf_band5100_gain_offset",  (void *)&ssv_cfg.rf_band5100_gain_offset,    0,  __string2s32     , "0"},
    { "rf_band5500_gain_offset",  (void *)&ssv_cfg.rf_band5500_gain_offset,    0,  __string2s32     , "0"},
    { "rf_band5700_gain_offset",  (void *)&ssv_cfg.rf_band5700_gain_offset,    0,  __string2s32     , "0"},
    { "rf_band5900_gain_offset",  (void *)&ssv_cfg.rf_band5900_gain_offset,    0,  __string2s32     , "0"},
    { "rf_b_rate_offset",     (void *)&ssv_cfg.rf_b_rate_offset,       0,  __string2s32     , "0"},
    { "rf_g_rate_offset",     (void *)&ssv_cfg.rf_g_rate_offset,       0,  __string2s32     , "0"},
    { "rf_ht20_rate_offset",  (void *)&ssv_cfg.rf_ht20_rate_offset,    0,  __string2s32     , "0"},
    { "rf_ht40_rate_offset",  (void *)&ssv_cfg.rf_ht40_rate_offset,    0,  __string2s32     , "0"},
    { "padpd",                (void *)&ssv_cfg.padpd,                  0,  __string2u32     , "7"},
    { "disable_greentx",      (void *)&ssv_cfg.disable_greentx,        0,  __string2bool    , "0"},
    { "disable_cci",          (void *)&ssv_cfg.disable_cci,            0,  __string2bool    , "0"},
#ifdef CONFIG_ENABLE_HOST_THERMAL
    { "disable_host_thermal",   (void *)&ssv_cfg.disable_host_thermal,     0,  __string2u32   , "0"},
    { "temp_config_table", (void *)&ssv_cfg.temp_compensation_table[0], 0,  __string2str, NULL},
    { "host_thermal_debug",   (void *)&ssv_cfg.host_thermal_debug,     0,  __string2u32   , "0"},
#endif
    { "ap_force_tim_always_high", (void *)&ssv_cfg.ap_force_tim_always_high,     0,  __string2u32     , "0"},
    { NULL, NULL, 0, NULL, NULL},
};

EXPORT_SYMBOL(cfg_cmds);


static int ssv_cmd_cfg(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    int s;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    if ((argc == 2) && (strcmp(argv[1], "reset") == 0)) {
        memset(&ssv_cfg, 0, sizeof(ssv_cfg));
        return 0;
    } else if ((argc == 2) && (strcmp(argv[1], "show") == 0)) {
        snprintf_res(cmd_data, ">> ssv6xxx config:\n");
        snprintf_res(cmd_data, "    hw_caps = 0x%08x\n", ssv_cfg.hw_caps);

        snprintf_res(cmd_data, "    def_chan = %d\n", ssv_cfg.def_chan);

        snprintf_res(cmd_data, "    wifi_tx_gain_level_gn = %d\n", ssv_cfg.wifi_tx_gain_level_gn);

        snprintf_res(cmd_data, "    wifi_tx_gain_level_b = %d\n", ssv_cfg.wifi_tx_gain_level_b);

        snprintf_res(cmd_data, "    sta-mac = %02x:%02x:%02x:%02x:%02x:%02x",
            ssv_cfg.maddr[0][0], ssv_cfg.maddr[0][1], ssv_cfg.maddr[0][2],
            ssv_cfg.maddr[0][3], ssv_cfg.maddr[0][4], ssv_cfg.maddr[0][5]);

        snprintf_res(cmd_data, "\n");
        return 0;
    }
    
    if (argc != 4)
        return -1;
    
    for(s = 0; cfg_cmds[s].cfg_cmd != NULL; s++) {        
        if (strcmp(cfg_cmds[s].cfg_cmd, argv[1]) == 0) {
            cfg_cmds[s].translate_func(argv[3], 
                cfg_cmds[s].var, cfg_cmds[s].arg);
            snprintf_res(cmd_data, "");
            return 0;
        }
    }
    return -1;
#else
    return 0;
#endif
}

// snprintf to result buffer
void snprintf_res(struct ssv_cmd_data *cmd_data, const char *fmt, ... )
{
    char *buf_head;
    int buf_left;
    va_list args;
    char *ssv6xxx_result_buf = cmd_data->ssv6xxx_result_buf;


    ssv6xxx_result_buf = cmd_data->ssv6xxx_result_buf;  
    
    // buf end reached
    if (cmd_data->rsbuf_len >= (cmd_data->rsbuf_size -1))
        return;
    buf_head = ssv6xxx_result_buf + cmd_data->rsbuf_len;
    buf_left = cmd_data->rsbuf_size - cmd_data->rsbuf_len;
   
    va_start(args, fmt); 
    cmd_data->rsbuf_len += vsnprintf(buf_head, buf_left, fmt, args);
    va_end(args);
    // No need for protection here. If vsnprint use more 
    // than buf_left. It will put 0x0 at last byte automatically
    // when overflow and stop copy,

}

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static void _dump_sta_info (struct ssv_softc *sc,
                            struct ssv_vif_info *vif_info,
                            struct ssv_sta_info *sta_info,
                            int    sta_idx)
{
    struct ssv_sta_priv_data *priv_sta = (struct ssv_sta_priv_data *)sta_info->sta->drv_priv;/* sta_info is already protected by ssv_cmd_submit(). */
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    if ((sta_info->s_flags & STA_FLAG_VALID) == 0) {/* sta_info is already protected by ssv_cmd_submit(). */
        snprintf_res(cmd_data,
                "        Station %d: %d is not valid\n",
                sta_idx, priv_sta->sta_idx);
    } else {
        snprintf_res(cmd_data,
                "        Station %d: %d\n"
                "             Address: %02X:%02X:%02X:%02X:%02X:%02X\n"
                "             WISD: %d\n"
                "             AID: %d\n",
                sta_idx, priv_sta->sta_idx,
                sta_info->sta->addr[0], sta_info->sta->addr[1], sta_info->sta->addr[2],
                sta_info->sta->addr[3], sta_info->sta->addr[4], sta_info->sta->addr[5],
                sta_info->hw_wsid, sta_info->aid);/* sta_info is already protected by ssv_cmd_submit(). */
        snprintf_res(cmd_data,
                "             SIGNAL: %d\n",
                -(sta_info->signal));
    }
}

void ssv6xxx_dump_sta_info (struct ssv_softc *sc)
{
    int j, sta_idx = 0;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    snprintf_res(cmd_data, "  >>>> bcast queue len[%d]\n", sc->bcast_txq.cur_qsize);

    for (j = 0; j < SSV6200_MAX_VIF; j++) {
        struct ieee80211_vif *vif = sc->vif_info[j].vif;
        struct ssv_vif_priv_data *priv_vif;
        struct ssv_sta_priv_data *sta_priv_iter;

        if (vif == NULL){
            snprintf_res(cmd_data, "    VIF: %d is not used.\n", j);

            continue; 
        }

         snprintf_res(cmd_data,
                 "    VIF: %d - [%02X:%02X:%02X:%02X:%02X:%02X] type[%d] p2p[%d] p2p_status[%d] channel[%d]\n", j,
                 vif->addr[0], vif->addr[1], vif->addr[2],
                 vif->addr[3], vif->addr[4], vif->addr[5], vif->type, vif->p2p, sc->p2p_status, sc->hw_chan);
        
        priv_vif = (struct ssv_vif_priv_data *)(vif->drv_priv);

         snprintf_res(cmd_data,
                 "           - sta asleep mask[%08X]\n", priv_vif->sta_asleep_mask);
        
        list_for_each_entry(sta_priv_iter, &priv_vif->sta_list, list){
            
            if ((sc->sta_info[sta_priv_iter->sta_idx].s_flags & STA_FLAG_VALID) == 0) {/* sta_info is already protected by ssv_cmd_submit(). */
                snprintf_res(cmd_data, "    STA: %d  is not valid.\n", sta_idx);

                continue;
            }
            _dump_sta_info(sc, &sc->vif_info[priv_vif->vif_idx],
                           &sc->sta_info[sta_priv_iter->sta_idx], sta_idx);
            sta_idx++;
        }
    }

} // ssv6xxx_dump_sta_info
#endif

static int ssv_cmd_sta(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    if ((argc >= 2) && (strcmp(argv[1], "show") == 0))
        ssv6xxx_dump_sta_info(sc);
    else
        snprintf_res(&sc->cmd_data, "sta show\n\n");
#endif
    return 0;
}

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static void ssv_cmd_get_chip_id(struct ssv_softc *sc, char *chip_id)
{
    HAL_GET_CHIP_ID(sc->sh);
    strcpy(chip_id, sc->sh->chip_id);
}
#endif

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static bool ssv6xxx_dump_cfg(struct ssv_hw *sh)
{
    struct ssv_cmd_data *cmd_data = &sh->sc->cmd_data;
    
    snprintf_res(cmd_data, "\n>> Current Configuration:\n\n");
    snprintf_res(cmd_data, "  hw_mac:                   %pM\n", sh->cfg.maddr[0]);
    snprintf_res(cmd_data, "  hw_mac2:                  %pM\n", sh->cfg.maddr[1]);
    snprintf_res(cmd_data, "  def_chan:                 %d\n", sh->cfg.def_chan);
    snprintf_res(cmd_data, "  hw_caps:                  0x%x\n", sh->cfg.hw_caps);
    snprintf_res(cmd_data, "  use_wpa2_only:            %d\n", sh->cfg.use_wpa2_only);
    snprintf_res(cmd_data, "  wifi_tx_gain_level_gn:    %d\n", sh->cfg.wifi_tx_gain_level_gn);
    snprintf_res(cmd_data, "  wifi_tx_gain_level_b:     %d\n", sh->cfg.wifi_tx_gain_level_b);
    snprintf_res(cmd_data, "  xtal_clock:               %d\n", sh->cfg.crystal_type);
    snprintf_res(cmd_data, "  volt_regulator:           %d\n", sh->cfg.volt_regulator);
    snprintf_res(cmd_data, "  firmware_path:            %s\n", sh->cfg.firmware_path);
    snprintf_res(cmd_data, "  mac_address_path:         %s\n", sh->cfg.mac_address_path);
    snprintf_res(cmd_data, "  mac_output_path:          %s\n", sh->cfg.mac_output_path);
    snprintf_res(cmd_data, "  ignore_efuse_mac:         %d\n", sh->cfg.ignore_efuse_mac);
    snprintf_res(cmd_data, "  mac_address_mode:         %d\n", sh->cfg.mac_address_mode);
    snprintf_res(cmd_data, "  beacon_rssi_minimal:      %d\n", sh->cfg.beacon_rssi_minimal);
    snprintf_res(cmd_data, "  force_chip_identity:      0x%x\n", sh->cfg.force_chip_identity);
    snprintf_res(cmd_data, "  external_firmware_name:   %s\n", sh->cfg.external_firmware_name);
    snprintf_res(cmd_data, "  ignore_firmware_version:  %d\n", sh->cfg.ignore_firmware_version);
    snprintf_res(cmd_data, "  auto_rate_enable:         %d\n", sh->cfg.auto_rate_enable);
    snprintf_res(cmd_data, "  rc_rate_idx_set:          0x%x\n", sh->cfg.rc_rate_idx_set);
    snprintf_res(cmd_data, "  rc_retry_set:             0x%x\n", sh->cfg.rc_retry_set);
    snprintf_res(cmd_data, "  rc_mf:                    %d\n", sh->cfg.rc_mf);
    snprintf_res(cmd_data, "  rc_long_short:            %d\n", sh->cfg.rc_long_short);
    snprintf_res(cmd_data, "  rc_ht40:                  %d\n", sh->cfg.rc_ht40);        
    snprintf_res(cmd_data, "  rc_phy_mode:              %d\n", sh->cfg.rc_phy_mode);    
    snprintf_res(cmd_data, "  hwqlimit(BK queue):        %d\n", sh->cfg.bk_txq_size);
    snprintf_res(cmd_data, "  hwqlimit(BE queue):        %d\n", sh->cfg.be_txq_size);
    snprintf_res(cmd_data, "  hwqlimit(VI queue):        %d\n", sh->cfg.vi_txq_size);
    snprintf_res(cmd_data, "  hwqlimit(VO queue):        %d\n", sh->cfg.vo_txq_size);
    snprintf_res(cmd_data, "  hwqlimit(MNG queue):       %d\n", sh->cfg.manage_txq_size);
    snprintf_res(cmd_data, "\n\n");
    return 0;
}
#endif

//dump [wsid|decision|phy-info|phy-reg|rf-reg|cfg]
static int ssv_cmd_dump(struct ssv_softc *sc, int argc, char *argv[])
{    
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    if (argc != 2) {
        snprintf_res(cmd_data, "dump [wsid|decision|phy-reg|rf-reg|cfg]\n");
        return 0;
    }

    if (strcmp(argv[1], "wsid") == 0) {
       return SSV_DUMP_WSID(sc->sh); 
    }
    
    if (strcmp(argv[1], "decision") == 0 ) {
        return SSV_DUMP_DECISION(sc->sh);
    }

    if (strcmp(argv[1], "phy-reg") == 0) {
        return HAL_DUMP_PHY_REG(sc->sh);
    }
    if (strcmp(argv[1], "rf-reg") == 0) {
        return HAL_DUMP_RF_REG(sc->sh);
    }

    if (strcmp(argv[1], "cfg") == 0) {
        return ssv6xxx_dump_cfg(sc->sh);
    }
    
    snprintf_res(cmd_data, "dump [wsid|decision|phy-reg|rf-reg|cfg]\n");
#endif
    return 0;
}

static int ssv_cmd_irq(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    char *endp;
    u32 irq_sts;
    struct ssv6xxx_hci_info *hci = &sc->sh->hci;
    struct ssv6xxx_hwif_ops *ifops = hci->if_ops;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    /* irq [set|get] [mask|enable|disable|status] */
    if ((argc >= 3) && (strcmp(argv[1], "set") == 0)) {
        if ((strcmp(argv[2], "mask") == 0) && (argc == 4)) {
            irq_sts = simple_strtoul(argv[3], &endp, 16);
	    	if (!sc->sh->hci.if_ops->irq_setmask) {
            	snprintf_res(cmd_data, "The interface doesn't provide irq_setmask operation.\n");
            	return 0;
	    	}
            ifops->irq_setmask(hci->dev, irq_sts);
            snprintf_res(cmd_data, "set sdio irq mask to 0x%08x\n", irq_sts);
            return 0;
        }
        if (strcmp(argv[2], "enable") == 0) {
	        if (!ifops->irq_enable) {
            	snprintf_res(cmd_data, "The interface doesn't provide irq_enable operation.\n");
            	return 0;
	    	}
            ifops->irq_enable(hci->dev);
            snprintf_res(cmd_data, "enable sdio irq.\n");
            return 0;
        }
        if (strcmp(argv[2], "disable") == 0) {
	    	if (!ifops->irq_disable) {
            	snprintf_res(cmd_data, "The interface doesn't provide irq_disable operation.\n");
            	return 0;
	    	}
            ifops->irq_disable(hci->dev, false);
            snprintf_res(cmd_data, "disable sdio irq.\n");
            return 0;
        }
        return -1;
    } else if ( (argc == 3) && (strcmp(argv[1], "get") == 0)) {
        if (strcmp(argv[2], "mask") == 0) {
	    	if (!ifops->irq_getmask) {
                snprintf_res(cmd_data, "The interface doesn't provide irq_getmask operation.\n");
            	return 0;
	    	}
            ifops->irq_getmask(hci->dev, &irq_sts);
            snprintf_res(cmd_data, "sdio irq mask: 0x%08x, int_mask=0x%08x\n", irq_sts,
                         hci->hci_ctrl->int_mask);
            return 0;
        }  
        if (strcmp(argv[2], "status") == 0) {
	    	if (!ifops->irq_getstatus) {
            	snprintf_res(cmd_data, "The interface doesn't provide irq_getstatus operation.\n");
            	return 0;
	    	}
            ifops->irq_getstatus(hci->dev, &irq_sts);
            snprintf_res(cmd_data, "sdio irq status: 0x%08x\n", irq_sts);
            return 0;
        }        
        return -1;
    } else {
        snprintf_res(cmd_data, "irq [set|get] [mask|enable|disable|status]\n");
    }
#endif
    return 0;
}

//mac [security|wsid|rxq]  [show]
//mac [set|get] [rate] [auto|idx]

//mac [rx|tx] [eable|disable]

static int ssv_cmd_mac(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    char *endp;
    int i;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

#if 0
    if ((argc == 3) && (!strcmp(argv[1], "wsid")) && (!strcmp(argv[2], "show"))) {
        u32 s;

        }
        return 0;
    } else
#endif
 
    if ((argc == 3) && (!strcmp(argv[1], "rx"))){
        if (!strcmp(argv[2], "enable")){
            sc->dbg_rx_frame = 1;
        } else {
            sc->dbg_rx_frame = 0;
        }

        snprintf_res(cmd_data, "  dbg_rx_frame %d\n", sc->dbg_rx_frame);
            
        return 0;
    } else if ((argc == 3) && (!strcmp(argv[1], "tx"))){

        if(!strcmp(argv[2], "enable")){
            sc->dbg_tx_frame = 1;
        } else {
            sc->dbg_tx_frame = 0;
        }

        snprintf_res(cmd_data, "  dbg_tx_frame %d\n", sc->dbg_tx_frame);
       
        return 0;
    } else if ((argc == 3) && (!strcmp(argv[1], "rxq")) && (!strcmp(argv[2], "show"))) {
        snprintf_res(cmd_data, "  rx frame %d\n", sc->rx.rx_count);
        snprintf_res(cmd_data, "  rx data frame %d\n", sc->rx.rx_data_count);
        snprintf_res(cmd_data, "  rx mgmt frame %d\n", sc->rx.rx_mgmt_count);
        snprintf_res(cmd_data, "  rx bcn frame %d\n", sc->rx.rx_bcn_count);
        snprintf_res(cmd_data, "  rx probereq frame %d\n", sc->rx.rx_probereq_count);
        snprintf_res(cmd_data, "  rx proberesp frame %d\n", sc->rx.rx_proberesp_count);
        snprintf_res(cmd_data, "  rx assoc_req frame %d\n", sc->rx.rx_assoc_req_count);
        snprintf_res(cmd_data, "  rx assoc_resp frame %d\n", sc->rx.rx_assoc_resp_count);
        snprintf_res(cmd_data, "  rx auth frame %d\n", sc->rx.rx_auth_count);
        snprintf_res(cmd_data, "  rx disassoc frame %d\n", sc->rx.rx_disassoc_count);
        snprintf_res(cmd_data, "  rx deauth frame %d\n", sc->rx.rx_deauth_count);
        return 0;
    
    } else if ((argc == 3) && (!strcmp(argv[1], "txq")) && (!strcmp(argv[2], "show"))) {
        snprintf_res(cmd_data, "  tx frame %d\n", sc->tx.tx_count);
        snprintf_res(cmd_data, "  tx data frame %d\n", sc->tx.data_count);
        snprintf_res(cmd_data, "  tx mgmt frame %d\n", sc->tx.mgmt_count);
        snprintf_res(cmd_data, "  tx auth frame %d\n", sc->tx.auth_count);
        snprintf_res(cmd_data, "  tx deauth frame %d\n", sc->tx.deauth_count);
        snprintf_res(cmd_data, "  tx assocreq frame %d\n", sc->tx.assoc_req_count);
        snprintf_res(cmd_data, "  tx assocresp frame %d\n", sc->tx.assoc_resp_count);
        snprintf_res(cmd_data, "  tx probereq frame %d\n", sc->tx.probe_req_count);
        snprintf_res(cmd_data, "  tx proberesp frame %d\n", sc->tx.probe_resp_count);
        return 0;
    }

    else if ((argc == 4) && (!strcmp(argv[1], "set")) && (!strcmp(argv[2], "rate"))) {
        if (strcmp(argv[3], "auto") == 0) {
            sc->sc_flags &= ~SC_OP_FIXED_RATE;
            return 0;
        }
        i = simple_strtoul(argv[3], &endp, 10); 
        if ( i < 0 || i > 38) {
            snprintf_res(cmd_data, " Invalid rat index !!\n");
            return -1;
        }
        sc->max_rate_idx = i;
        sc->sc_flags |= SC_OP_FIXED_RATE;
        snprintf_res(cmd_data, " Set rate to index %d\n", i);

        return 0;
    } else if ((argc == 3) && (!strcmp(argv[1], "get")) && (!strcmp(argv[2], "rate"))) {
        if (sc->sc_flags & SC_OP_FIXED_RATE)
            snprintf_res(cmd_data, " Current Rate Index: %d\n", sc->max_rate_idx);
        else 
            snprintf_res(cmd_data, "  Current Rate Index: auto\n");

        return 0;
    } else if ((argc == 2) && (!strcmp(argv[1], "setting"))){
        
        snprintf_res(cmd_data, "tx_cfg threshold:\n");
        snprintf_res(cmd_data, "\t tx_id_threshold %d tx_cfg tx_lowthreshold_id_trigger %d tx_page_threshold %d\n",
            sc->sh->tx_info.tx_id_threshold, sc->sh->tx_info.tx_lowthreshold_id_trigger, 
            sc->sh->tx_info.tx_page_threshold);
        
        snprintf_res(cmd_data, "rx_cfg threshold:\n");
        snprintf_res(cmd_data, "\t rx_id_threshold %d  rx_page_threshold %d\n",
            sc->sh->rx_info.rx_id_threshold, sc->sh->rx_info.rx_page_threshold);  
        
        snprintf_res(cmd_data, "tx page available %d\n" , sc->sh->tx_page_available);
        snprintf_res(cmd_data, "total tx+rx page %d\n" , sc->sh->total_tx_rx_page);
            
        //snprintf_res(cmd_data, "max aggr size %d page %d\n" , SSV_GET_MAX_AMPDU_SIZE(sc->sh),
        //    SSV_GET_MAX_AMPDU_SIZE(sc->sh) >> HW_MMU_PAGE_SHIFT);
      
    } else {
        snprintf_res(cmd_data, "mac [rxq] [show]\n");

        snprintf_res(cmd_data, "mac [set|get] [rate] [auto|idx]\n");

        snprintf_res(cmd_data, "mac [rx|tx] [eable|disable]\n");
        
        snprintf_res(cmd_data, "mac [setting]\n");
    }
#endif
    return 0;
}

//hci [txq|rxq] [show]
static int ssv_cmd_hci(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv6xxx_hci_ctrl *hci_ctrl = sc->sh->hci.hci_ctrl;
    struct ssv_sw_txq       *txq;
    struct ssv_cmd_data     *cmd_data = &sc->cmd_data;
    int                      s = 0;

    if ((argc == 3) && (!strcmp(argv[1], "txq")) && (!strcmp(argv[2], "show"))) {          
        for(s = 0; s < SSV_SW_TXQ_NUM; s++) {
            txq = &hci_ctrl->sw_txq[s];
            
            snprintf_res(cmd_data, ">> txq[%d] pause %d ", txq->txq_no, txq->paused);
            snprintf_res(cmd_data, "cur_qsize=%d\n", skb_queue_len(&txq->qhead));
            snprintf_res(cmd_data, "            Total %d frame sent\n", txq->tx_pkt);
        }
        return 0;
    
    } else if ((argc == 3) && (!strcmp(argv[1], "hwq")) && (!strcmp(argv[2], "show"))){
        snprintf_res(cmd_data, ">> HW Queue mask: 0x%02x\n", hci_ctrl->hw_txq_mask);
        snprintf_res(cmd_data, ">>    EDCA0: %s\n", ((hci_ctrl->hw_txq_mask & TXQ_EDCA_0) ? "stop" : "start"));
        snprintf_res(cmd_data, ">>    EDCA1: %s\n", ((hci_ctrl->hw_txq_mask & TXQ_EDCA_1) ? "stop" : "start"));
        snprintf_res(cmd_data, ">>    EDCA2: %s\n", ((hci_ctrl->hw_txq_mask & TXQ_EDCA_2) ? "stop" : "start"));
        snprintf_res(cmd_data, ">>    EDCA3: %s\n", ((hci_ctrl->hw_txq_mask & TXQ_EDCA_3) ? "stop" : "start"));
        snprintf_res(cmd_data, ">>    MGMTQ: %s\n", ((hci_ctrl->hw_txq_mask & TXQ_MGMT) ? "stop" : "start"));
        return 0;
    
    } else if ((argc == 3) && (!strcmp(argv[1], "rxq")) && (!strcmp(argv[2], "show"))){
        snprintf_res(cmd_data, ">> rx_packet = %d, rx_isr_count = %d\n",
                  hci_ctrl->rx_pkt, hci_ctrl->rx_isr_cnt);
        return 0;
    }    
    else
    {
        snprintf_res(cmd_data, "hci [txq|rxq] [show]\n\n");
        return 0;
    }
#endif
    return 0;
}

static int ssv_cmd_hwq(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
     struct ssv_cmd_data *cmd_data = &sc->cmd_data;

/*
MBOX HWID	OWNER	in queue	Out queue
0	MCU	                    16	16
1	HCI	                    8	16
2	Reserved	             X	X
3	Security TX	      4	16
4	MRX	                    16	X
5	MIC TX	              4	4
6	MTX EDCA0	       4	8
7	MTX EDCA1	       4	16
8	MTX EDCA2	       4	16
9	MTX EDCA3	       4	16
10	MTX MNG	              4	  8
11	Security RX	       4	16
12	MIC RX	              4	4
13	Reserved	            X	X
14	Reserved	            X	X
15	Trash can	     X	32



#define GET_FF0_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x0000001f ) >> 0) 
#define GET_FF1_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x000001e0 ) >> 5) 
#define GET_FF3_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x00003800 ) >> 11)
#define GET_FF5_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x000e0000 ) >> 17)
#define GET_FF6_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x00700000 ) >> 20)
#define GET_FF7_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x03800000 ) >> 23)
#define GET_FF8_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0x1c000000 ) >> 26)
#define GET_FF9_CNT                          (((REG32(ADR_RD_IN_FFCNT1))                       & 0xe0000000 ) >> 29)
#define GET_FF10_CNT                         (((REG32(ADR_RD_IN_FFCNT2))                       & 0x00000007 ) >> 0) 
#define GET_FF11_CNT                         (((REG32(ADR_RD_IN_FFCNT2))                       & 0x00000038 ) >> 3) 
#define GET_FF12_CNT                         (((REG32(ADR_RD_IN_FFCNT2))                       & 0x000001c0 ) >> 6) 
#define GET_FF13_CNT                         (((REG32(ADR_RD_IN_FFCNT2))                       & 0x00000600 ) >> 9) 
#define GET_FF14_CNT                         (((REG32(ADR_RD_IN_FFCNT2))                       & 0x00001800 ) >> 11)
#define GET_FF15_CNT                         (((REG32(ADR_RD_IN_FFCNT2))                       & 0x00006000 ) >> 13)
#define GET_FF4_CNT                          (((REG32(ADR_RD_IN_FFCNT2))                       & 0x000f8000 ) >> 15)
#define GET_FF2_CNT                          (((REG32(ADR_RD_IN_FFCNT2))                       & 0x00700000 ) >> 20)
//==============================================================================================

printf("\n[TAG]  MCU - HCI - SEC -  RX - MIC - TX0 - TX1 - TX2 - TX3 - TX4 - SEC - MIC - TSH\n");
printf("OUTPUT %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d\n",
			GET_FFO0_CNT, GET_FFO1_CNT, GET_FFO3_CNT, GET_FFO4_CNT, GET_FFO5_CNT, GET_FFO6_CNT,
			GET_FFO7_CNT, GET_FFO8_CNT, GET_FFO9_CNT, GET_FFO10_CNT, GET_FFO11_CNT, GET_FFO12_CNT, GET_FFO15_CNT);
printf("INPUT  %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d\n",
			GET_FF0_CNT, GET_FF1_CNT, GET_FF3_CNT, GET_FF4_CNT, GET_FF5_CNT, GET_FF6_CNT,
			GET_FF7_CNT, GET_FF8_CNT, GET_FF9_CNT, GET_FF10_CNT, GET_FF11_CNT, GET_FF12_CNT, GET_FF15_CNT);
printf("TX[%d]RX[%d]AVA[%d]\n",GET_TX_ID_ALC_LEN,GET_RX_ID_ALC_LEN,GET_AVA_TAG);
    */
    u32  value, value1, value2;
	u32 tx_len = 0, rx_len = 0, ava_status = 0;
    u32 id0 = 0, id1 = 0, id2 = 0, id3 = 0; 
//-----------------------------

    SSV_READ_FFOUT_CNT(sc->sh, &value, &value1, &value2);
    
    snprintf_res(cmd_data, "\n[TAG]  MCU - HCI - SEC -  RX - MIC - TX0 - TX1 - TX2 - TX3 - TX4 - SEC - MIC - TSH\n");
    
    snprintf_res(cmd_data, "OUTPUT %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d\n",
			SSV_GET_FFOUT_CNT(sc->sh, value, M_ENG_CPU), 
			SSV_GET_FFOUT_CNT(sc->sh, value, M_ENG_HWHCI), 
			SSV_GET_FFOUT_CNT(sc->sh, value, M_ENG_ENCRYPT), 
			SSV_GET_FFOUT_CNT(sc->sh, value, M_ENG_MACRX), 
			SSV_GET_FFOUT_CNT(sc->sh, value, M_ENG_MIC), 
			SSV_GET_FFOUT_CNT(sc->sh, value1, M_ENG_TX_EDCA0), 
			SSV_GET_FFOUT_CNT(sc->sh, value1, M_ENG_TX_EDCA1), 
			SSV_GET_FFOUT_CNT(sc->sh, value1, M_ENG_TX_EDCA2), 
			SSV_GET_FFOUT_CNT(sc->sh, value1, M_ENG_TX_EDCA3), 
			SSV_GET_FFOUT_CNT(sc->sh, value1, M_ENG_TX_MNG), 
			SSV_GET_FFOUT_CNT(sc->sh, value1, M_ENG_ENCRYPT_SEC), 
			SSV_GET_FFOUT_CNT(sc->sh, value2, M_ENG_MIC_SEC), 
			SSV_GET_FFOUT_CNT(sc->sh, value2, M_ENG_TRASH_CAN)); 

//-----------------------------
    SSV_READ_IN_FFCNT(sc->sh, &value, &value1);
    snprintf_res(cmd_data, "INPUT  %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d - %3d\n",
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_CPU), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_HWHCI), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_ENCRYPT), 
			SSV_GET_IN_FFCNT(sc->sh, value1, M_ENG_MACRX), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_MIC), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_TX_EDCA0), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_TX_EDCA1), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_TX_EDCA2), 
			SSV_GET_IN_FFCNT(sc->sh, value, M_ENG_TX_EDCA3), 
			SSV_GET_IN_FFCNT(sc->sh, value1, M_ENG_TX_MNG), 
			SSV_GET_IN_FFCNT(sc->sh, value1, M_ENG_ENCRYPT_SEC), 
			SSV_GET_IN_FFCNT(sc->sh, value1, M_ENG_MIC_SEC), 
			SSV_GET_IN_FFCNT(sc->sh, value1, M_ENG_TRASH_CAN)); 
//-----------------------------
    SSV_READ_ID_LEN_THRESHOLD(sc->sh, &tx_len, &rx_len);
    SSV_READ_TAG_STATUS(sc->sh, &ava_status);
    snprintf_res(cmd_data, "TX[%d]RX[%d]AVA[%d]\n", tx_len, rx_len, ava_status);

    SSV_READ_ALLID_MAP(sc->sh, &id0, &id1, &id2, &id3);
    snprintf_res(cmd_data, "ALLID_MAP [%08x %08x %08x %08x]\n", id3, id2, id1, id0);
    SSV_READ_TXID_MAP(sc->sh, &id0, &id1, &id2, &id3);
    snprintf_res(cmd_data, "TXID_MAP [%08x %08x %08x %08x]\n", id3, id2, id1, id0);
    SSV_READ_RXID_MAP(sc->sh, &id0, &id1, &id2, &id3);
    snprintf_res(cmd_data, "RXID_MAP [%08x %08x %08x %08x]\n", id3, id2, id1, id0);
#endif
    return 0; 
}

static int ssv_cmd_mib(struct ssv_softc *sc, int argc, char *argv[])
{
    SSV_CMD_MIB(sc, argc, argv);
    return 0;  
}

static int ssv_cmd_power_saving(struct ssv_softc *sc, int argc, char *argv[])
{
    SSV_CMD_POWER_SAVING(sc, argc, argv);
    return 0;  
}

static int ssv_cmd_sdio(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32   addr, value;
    char *endp;
    int   ret=0;
    struct ssv6xxx_hci_info *hci = &sc->sh->hci;
    struct ssv6xxx_hwif_ops *ifops = hci->if_ops;
    struct ssv_cmd_data     *cmd_data = &sc->cmd_data;

    /**
        * (1) sdio reg r [sdio_addr]
        * (2) sdio reg w [sdio_addr] [value]
        */
    if ((argc == 4) && (!strcmp(argv[1], "reg")) && (!strcmp(argv[2], "r"))) {
        addr = simple_strtoul(argv[3], &endp, 16);
		 if (!ifops->cmd52_read){
    		snprintf_res(cmd_data, "The interface doesn't provide cmd52 read\n");

			return 0;
		}        
		ret = ifops->cmd52_read(hci->dev, addr, &value);
        if (ret >= 0) {
            snprintf_res(cmd_data, "  ==> %x\n", value);

            return 0;
        }
    } else if ((argc ==5) && (!strcmp(argv[1], "reg")) && (!strcmp(argv[2], "w"))){
        addr = simple_strtoul(argv[3], &endp, 16);
        value = simple_strtoul(argv[4], &endp, 16);
        if (!ifops->cmd52_write){
    		snprintf_res(cmd_data, "The interface doesn't provide cmd52 write\n");

			return 0;
		}
        ret = ifops->cmd52_write(hci->dev, addr, value);
        if (ret >= 0) {
            snprintf_res(cmd_data, "  ==> write done.\n");

            return 0;
        }
    }

    snprintf_res(cmd_data, "sdio cmd52 fail: %d\n", ret);
#endif
    return 0;
}

#define CLI_VERSION     "0.09"

static int ssv_cmd_version (struct ssv_softc *sc, int argc, char *argv[]) 
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32                  regval;
    char                 chip_id[24] = "";
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    //CLI version
    snprintf_res(cmd_data, "CLI version: %s \n", CLI_VERSION);
    
    //CHIP TAG

    snprintf_res(cmd_data, "CHIP TAG: %llx \n", sc->sh->chip_tag);

    //CHIP ID
    ssv_cmd_get_chip_id(sc, chip_id);
    snprintf_res(cmd_data, "CHIP ID: %s \n", chip_id);

    snprintf_res(cmd_data, "# current Software mac version: %d\n", ssv_root_version);

    snprintf_res(cmd_data, "COMPILER DATE %s \n", COMPILERDATE);

    SSV_GET_FW_VERSION(sc->sh, &regval);
    snprintf_res(cmd_data, "Firmware image version: %d\n", regval);
#else
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    snprintf_res(cmd_data, "# current Software mac version: %d\n", ssv_root_version);
#endif
    return 0;
}

static int ssv_cmd_tool(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32   addr, value, count, len;
    char *endp;
    int   s, retval;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    /* tool [r|w] [address] [value|word-count] */

    if ((argc == 4) && (strcmp(argv[1], "w") == 0)) {
        /* reg w <address> <word_value> */
        addr = simple_strtoul(argv[2], &endp, 16);
        value = simple_strtoul(argv[3], &endp, 16);
        if (SMAC_REG_WRITE(sc->sh, addr, value)) ;
        snprintf_res(cmd_data, "ok");
        return 0;
    }

    if (( (argc == 4) || (argc == 3)) && (strcmp(argv[1], "r") == 0)) {
        /* reg r <address> <word_count> */
        count = (argc==3)? 1: simple_strtoul(argv[3], &endp, 10);
        addr = simple_strtoul(argv[2], &endp, 16);
        //snprintf_res("ADDRESS: 0x%08x\n", addr);
        for(s=0; s<count; s++, addr+=4) {
            if (SMAC_REG_READ(sc->sh, addr, &value)) ;
            snprintf_res(cmd_data, "%08x\n", value);

            //if (((s+1)&0x07) == 0)
            //    snprintf_res("\n");
        }
        //snprintf_res("\n");
        return 0;
    }
    
    if ((argc == 3) && (strcmp(argv[1], "auto_gen_nullpkt") == 0)) {
        /* tool auto_gen_nullpkt <txq> */
        value = simple_strtoul(argv[2], &endp, 10);
        retval = SSV_AUTO_GEN_NULLPKT(sc->sh, value);
        if (!retval)
            snprintf_res(cmd_data, "done to auto generate null frame\n");
        else
            snprintf_res(cmd_data, "Not suppout the tool\n");
        return 0;
    }
    
    if ((argc == 3) && (strcmp(argv[1], "dump_pbuf") == 0)) {
        /* tool dump_pbuf <id> */
        value = simple_strtoul(argv[2], &endp, 10);
        /* step 1, pbuf len*/
        addr = 0x80000000 | (value << 16); 
        SMAC_REG_READ(sc->sh, addr, &len);
        len &= 0xffff;
        snprintf_res(cmd_data, "\n pbuf id=%d, len=%d\n\n", value, len);
        /* step 2, dump pbuf context */
        if (len > 1024)
            len = 1024;
        
        for(s=0; s<len; s+=4, addr+=4) {
            if (SMAC_REG_READ(sc->sh, addr, &value));
            if((s+4)%32 == 0)
                snprintf_res(cmd_data, " %08x\n", value);
            else
                snprintf_res(cmd_data, " %08x", value);
        }
        
        return 0;
    }
#endif   
    return 0;
}

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static int txtput_thread_m2(void *data)
{
#define Q_DELAY_MS 20
	struct sk_buff *skb = NULL;
	int qlen = 0, max_qlen, q_delay_urange[2];
	struct ssv_softc *sc = data;
    struct SKB_info_st *skb_info = NULL;
    
	/* Suppose the max sdio tput is 200Mbps
	 * max_qlen = 200Mbps / 8 / size_per_frame_in_byte * delay_in_sec
	 */
	max_qlen = (200 * 1000 / 8 * Q_DELAY_MS) / sc->ssv_txtput.size_per_frame;
	q_delay_urange[0] = Q_DELAY_MS * 1000;
	q_delay_urange[1] = q_delay_urange[0] + 1000;
	printk("max_qlen: %d\n", max_qlen);
	while (!kthread_should_stop() && sc->ssv_txtput.loop_times > 0) {     
		sc->ssv_txtput.loop_times--;
		skb = ssv_skb_alloc(sc, sc->ssv_txtput.size_per_frame);
		if (skb == NULL) {
			printk("ssv command txtput_generate_m2 "
			"ssv_skb_alloc fail!!!\n");
			goto end; 
		}
        skb_reserve(skb, sizeof(struct SKB_info_st));
        skb_info = (struct SKB_info_st *)skb->head;
        skb_info->txtput_frame = true;
        skb_put(skb, sc->ssv_txtput.size_per_frame);

        SSV_TXTPUT_SET_DESC(sc->sh, skb);
		qlen = sc->sh->hci.hci_ops->hci_tx(sc->sh->hci.hci_ctrl, skb, 0, false, 0);
		if (qlen >= max_qlen) {
			//printk("%s: qlen=%d\n", __func__, qlen);
			usleep_range(q_delay_urange[0], q_delay_urange[1]);
		}
	}
	//ssv_skb_free(skb);
end:
	sc->ssv_txtput.txtput_tsk = NULL;
	return 0;
}

int txtput_generate_m2(struct ssv_softc *sc, u32 size_per_frame, u32 loop_times)
{
	sc->ssv_txtput.size_per_frame = size_per_frame;
	sc->ssv_txtput.loop_times = loop_times;
	sc->ssv_txtput.txtput_tsk = kthread_run(txtput_thread_m2, sc, "txtput_thread_m2");
	
	return 0;
}

static int txtput_tsk_cleanup(struct ssv_softc *sc)
{
	int ret = 0;
	if (sc->ssv_txtput.txtput_tsk) {
		printk("Stopping txtput task...\n");
		ret = kthread_stop(sc->ssv_txtput.txtput_tsk);
		while (sc->ssv_txtput.txtput_tsk != NULL) {
			msleep(1);
		}
        	printk("txtput task is stopped.\n");        
	}
	return ret;
}
#endif

static int ssv_cmd_txtput(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
	char *endp;
	u32 size_per_frame, loop_times;
	struct ssv_cmd_data *cmd_data = &sc->cmd_data;

	if ( (argc == 2) && (!strcmp(argv[1], "stop"))) {
		txtput_tsk_cleanup(sc);
		return 0;
	}

	if (argc != 3) {
		snprintf_res(cmd_data, "* txtput stop\n");
		snprintf_res(cmd_data, "* txtput [size] [frames]\n");
		snprintf_res(cmd_data, " EX: txtput 14000 9999 \n");

		return 0;
	}
	
	size_per_frame = simple_strtoul(argv[1], &endp, 10);
	loop_times	   = simple_strtoul(argv[2], &endp, 10);
	snprintf_res(cmd_data, "size & frames: %d & %d\n", size_per_frame, loop_times);

	//already in progress
	if (sc->ssv_txtput.txtput_tsk) {
		snprintf_res(cmd_data, "txtput already in progress\n");
		return 0;
	}
	txtput_generate_m2(sc, size_per_frame + TXPB_OFFSET, loop_times);
#endif
	return 0;
}

#define MAX_FRM_SIZE 2304
static int ssv_cmd_rxtput(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct sk_buff          *skb;
    struct cfg_host_cmd     *host_cmd;
	struct sdio_rxtput_cfg   cmd_rxtput_cfg;
    char                    *endp;
    struct ssv_cmd_data     *cmd_data = &sc->cmd_data;

    if (argc != 3) {
        snprintf_res(cmd_data, "rxtput [size] [frames]\n");
		return 0;
    }

	cmd_rxtput_cfg.size_per_frame = simple_strtoul(argv[1], &endp, 10);
	cmd_rxtput_cfg.total_frames   = simple_strtoul(argv[2], &endp, 10);
	
	if (cmd_rxtput_cfg.size_per_frame > MAX_FRM_SIZE) {
	    snprintf_res(cmd_data, "Frame size too large!!\n");
	    return 0 ;     
	}
    snprintf_res(cmd_data, "size & frames: %d& %d\n",
        cmd_rxtput_cfg.size_per_frame, cmd_rxtput_cfg.total_frames);
    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct sdio_rxtput_cfg));
	
    if(skb == NULL) {
        printk("ssv command ssv_skb_alloc fail!!!\n");
        return 0;
    }

    skb->data_len = HOST_CMD_HDR_LEN + sizeof(struct sdio_rxtput_cfg);
    skb->len      = skb->data_len;

    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd  = (u8)SSV6XXX_HOST_CMD_RX_TPUT;
    host_cmd->len    = skb->data_len;

    memcpy(host_cmd->un.dat32, &cmd_rxtput_cfg, sizeof(struct sdio_rxtput_cfg));

    if (HCI_SEND_CMD(sc->sh, skb) == 0) {
        snprintf_res(cmd_data, "## hci cmd was sent successfully\n");
    } else {
        snprintf_res(cmd_data, "## hci cmd was sent failed\n");
    }

#endif
	return 0;
}

static int ssv_cmd_check(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32 size,i,j,x,y,id,value,address,id_value;
    char *endp;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    u32 id_base_address[4];

    SSV_GET_RD_ID_ADR(sc->sh, &id_base_address[0]);

    if (argc != 2) {
        snprintf_res(cmd_data, "check [packet size]\n");
        return 0;
    }
    
        
    snprintf_res(cmd_data, " id address %x %x %x %x \n",  id_base_address[0],
                 id_base_address[1], id_base_address[2], id_base_address[3]);
 
    size = simple_strtoul(argv[1], &endp, 10);
    size = size >> 2;

    for (x = 0; x < 4; x++) {
        if (SMAC_REG_READ(sc->sh, id_base_address[x], &id_value));
        for (y = 0; y < 32 && id_value; y++, id_value>>=1) {
            if (id_value & 0x1) {
                id = 32*x + y;

                address = 0x80000000 + (id << 16);
                {
                    printk("        ");
                    for (i = 0; i < size; i += 8){
                        if(SMAC_REG_READ(sc->sh, address, &value));
                        printk("\n%08X:%08X", address,value);
                        address += 4;
                        for (j = 1; j < 8; j++){
                            if(SMAC_REG_READ(sc->sh, address, &value));
                            printk(" %08X", value);
                            address += 4;
                        }
                    }
                    printk("\n");
                }
            }
        }
    }
#endif
    return 0;
}

static int ssv_cmd_directack(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data; 
        
    if ((argc == 2) && (!strcmp(argv[1], "show"))) {
        snprintf_res(cmd_data, ">> act status = %s\n", 
            ((sc->sc_flags & SC_OP_DIRECTLY_ACK) ? "direct complete" : "delay complete"));
        snprintf_res(cmd_data, ">> tx_frame in q = %d\n", atomic_read(&sc->tx_frame));
        return 0;
    
    } else {
        snprintf_res(cmd_data, "\n directack [show]\n");
        return 0;
    }
#endif    
    return 0;
}

static int ssv_cmd_flowctl(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    if ((argc == 2) && (!strcmp(argv[1], "show"))) {
        snprintf_res(cmd_data, ">> flowctl = %d\n", sc->sh->cfg.flowctl);
        snprintf_res(cmd_data, ">> flowctl_low_threshold  = %d\n", sc->sh->cfg.flowctl_low_threshold);
        snprintf_res(cmd_data, ">> flowctl_high_threshold = %d\n", sc->sh->cfg.flowctl_high_threshold);
        snprintf_res(cmd_data, ">> flowctl_hci_cnt = %d\n", sc->flowctl_hci_cnt);
        snprintf_res(cmd_data, ">> flowctl_frame_cnt = %d\n", sc->flowctl_frame_cnt);
        snprintf_res(cmd_data, ">> tx_ack_ctl_q = %u\n", skb_queue_len(&sc->tx_ack_ctl_q));
        snprintf_res(cmd_data, ">> ieee80211_txq_status %s\n", ((sc->sc_flags & SC_OP_FLOWCTL) ? "stop" : "start"));
    
    } else {
        snprintf_res(cmd_data, ">> flowctl [show]\n");
    }
#endif
    return 0;
}

static int ssv_cmd_txrx_skb_q(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data; 
    struct ssv6xxx_hci_ctrl *hci_ctrl = sc->sh->hci.hci_ctrl;
    int txqid;
    char *endp;
        
    if ((argc == 2) && (!strcmp(argv[1], "show"))) {
        snprintf_res(cmd_data, ">> rx_skb_q = %u\n", skb_queue_len(&sc->rx_skb_q));
        for(txqid=0; txqid<SSV_SW_TXQ_NUM; txqid++) {
            snprintf_res(cmd_data, ">> sw_txq[%d] = %u\n", txqid, skb_queue_len(&hci_ctrl->sw_txq[txqid].qhead));
        }
        snprintf_res(cmd_data, ">> rx_threshold = %d\n", sc->sh->cfg.rx_threshold);
        return 0;
    
    } else if ((argc == 3) && (!strcmp(argv[1], "rx_threshold"))) {
        sc->sh->cfg.rx_threshold = simple_strtoul(argv[2], &endp, 10); 
        snprintf_res(cmd_data, ">> Set rx_threshold = %d\n", sc->sh->cfg.rx_threshold);
        return 0;
    } else {
        snprintf_res(cmd_data, "\n txrx_skb_q [show|rx_threshold] [value] \n");
        return 0;
    }
#endif    
    return 0;
}

static int ssv_cmd_log(struct ssv_softc *sc, int argc, char *argv[])
{   
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    u32 log_hex = 0;
	int log_size = 0, total_log_size;

    if ((argc == 3) && (!strcmp(argv[1], "tx_desc"))) {
        
		if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_TX_DESC;
        } else {
            sc->log_ctrl &= ~(LOG_TX_DESC);
        }
        snprintf_res(cmd_data, "  log tx_desc %s\n", ((sc->log_ctrl & LOG_TX_DESC) ? "enable": "disable"));
       
    } else if ((argc == 3) && (!strcmp(argv[1], "rx_desc"))){
        
		if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_RX_DESC;
        } else {
            sc->log_ctrl &= ~(LOG_RX_DESC);
        }
        snprintf_res(cmd_data, "  log rx_desc %s\n", ((sc->log_ctrl & LOG_RX_DESC) ? "enable": "disable"));
       
    } else if ((argc == 3) && (!strcmp(argv[1], "tx_frame"))){
        
		if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_TX_FRAME;
        } else {
            sc->log_ctrl &= ~(LOG_TX_FRAME);
        }
        snprintf_res(cmd_data, "  log tx_frame %s\n", ((sc->log_ctrl & LOG_TX_FRAME) ? "enable": "disable"));
       
    } else if((argc == 4) && (!strcmp(argv[1], "ampdu"))){
        
		if (!strcmp(argv[2], "ssn")){
            if(!strcmp(argv[3], "enable")){
                sc->log_ctrl  |= LOG_AMPDU_SSN;
            } else {
                sc->log_ctrl &= ~(LOG_AMPDU_SSN);
            }
        	snprintf_res(cmd_data, "  log ampdu ssn %s\n", ((sc->log_ctrl & LOG_AMPDU_SSN) ? "enable": "disable"));
   
        } else if (!strcmp(argv[2], "dbg")){
            if(!strcmp(argv[3], "enable")){
                sc->log_ctrl  |= LOG_AMPDU_DBG;
            } else {
                sc->log_ctrl &= ~(LOG_AMPDU_DBG);
            }
        	snprintf_res(cmd_data, "  log ampdu dbg %s\n", ((sc->log_ctrl & LOG_AMPDU_DBG) ? "enable": "disable"));
   
        }else if (!strcmp(argv[2], "err")){
            if(!strcmp(argv[3], "enable")){
                sc->log_ctrl |= LOG_AMPDU_ERR;
            } else {
                sc->log_ctrl &= ~(LOG_AMPDU_ERR);
            }
        	snprintf_res(cmd_data, "  log ampdu err %s\n", ((sc->log_ctrl & LOG_AMPDU_ERR) ? "enable": "disable"));
   
        }else{
            snprintf_res(cmd_data, " Invalid command!!\n");
            return 0;
        }

    }else if ((argc == 3) && (!strcmp(argv[1], "beacon"))) {
        
		if (!strcmp(argv[2], "enable")) {
            sc->log_ctrl  |= LOG_BEACON;
        } else {
            sc->log_ctrl &= ~(LOG_BEACON);
        }
        snprintf_res(cmd_data, "  log beacon%s\n", ((sc->log_ctrl & LOG_BEACON) ? "enable": "disable"));
		
    } else if ((argc == 3) && (!strcmp(argv[1], "rate_control"))) {
   
   	if (!strcmp(argv[2], "enable")) {
            sc->log_ctrl  |= LOG_RATE_CONTROL;
        } else {
            sc->log_ctrl &= ~(LOG_RATE_CONTROL);
        }
        snprintf_res(cmd_data, "  log rate control %s\n", ((sc->log_ctrl & LOG_RATE_CONTROL) ? "enable": "disable"));
    
    } else if ((argc == 3) && (!strcmp(argv[1], "rate_report"))) {
   
   		if (!strcmp(argv[2], "enable")) {
            sc->log_ctrl  |= LOG_RATE_REPORT;
        } else {
            sc->log_ctrl &= ~(LOG_RATE_REPORT);
        }
        snprintf_res(cmd_data, "  log rate report %s\n", ((sc->log_ctrl & LOG_RATE_REPORT) ? "enable": "disable"));
    
	} else if ((argc == 3) && (!strcmp(argv[1], "hci"))){

        if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_HCI;
        } else {
            sc->log_ctrl &= ~(LOG_HCI);
        }
        snprintf_res(cmd_data, "  log hci %s\n", ((sc->log_ctrl & LOG_HCI) ? "enable": "disable"));
	
	} else if ((argc == 3) && (!strcmp(argv[1], "hwif"))){

        if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_HWIF;
			sc->sh->priv->dbg_control = true;	
        } else {
            sc->log_ctrl &= ~(LOG_HWIF);
			sc->sh->priv->dbg_control = false;	
        }
        snprintf_res(cmd_data, "  log hwif %s\n", ((sc->log_ctrl & LOG_HWIF) ? "enable": "disable"));
    
	} else if ((argc == 3) && (!strcmp(argv[1], "flash_bin"))){

        if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_FLASH_BIN;
        } else {
            sc->log_ctrl &= ~(LOG_FLASH_BIN);
        }
        snprintf_res(cmd_data, "  log flash_bin %s\n", ((sc->log_ctrl & LOG_FLASH_BIN) ? "enable": "disable"));
	
	} else if ((argc == 3) && (!strcmp(argv[1], "flowctl"))){

        if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_FLOWCTL;
        } else {
            sc->log_ctrl &= ~(LOG_FLOWCTL);
        }
        snprintf_res(cmd_data, "  log flowctl %s\n", ((sc->log_ctrl & LOG_FLOWCTL) ? "enable": "disable"));
	
	} else if ((argc == 3) && (!strcmp(argv[1], "hex"))) {
    	
		if (1 != sscanf(argv[2], "%x", &log_hex)) {
            snprintf_res(cmd_data, " log hex hexstring\n");
		} else {
			sc->log_ctrl = log_hex;	
		}
        snprintf_res(cmd_data, "  log ctrl %d\n", sc->log_ctrl);
	
    } else if ((argc == 3) && (!strcmp(argv[1], "log_to_ram"))) {
        
		if (!strcmp(argv[2], "enable")) {
            sc->cmd_data.log_to_ram = true;
        } else {
            sc->cmd_data.log_to_ram = false;
        }
        snprintf_res(cmd_data, " log_to_ram %s\n", (sc->cmd_data.log_to_ram ? "enable" : "disable"));
		
		return 0;
    } else if ((argc == 3) && (!strcmp(argv[1], "ram_size"))) {
    	if (1 == sscanf(argv[2], "%d", &log_size)) {
			if (sc->cmd_data.dbg_log.data) {
				kfree(sc->cmd_data.dbg_log.data);
				memset(&sc->cmd_data.dbg_log, 0, sizeof(struct ssv_dbg_log));
			}
			
			if (log_size != 0) {
				if (sc->cmd_data.dbg_log.data)
					kfree(sc->cmd_data.dbg_log.data);
			
				total_log_size = log_size * 1024;
				sc->cmd_data.dbg_log.data = (char *)kzalloc(total_log_size, GFP_KERNEL);
				if (sc->cmd_data.dbg_log.data == NULL) {
        			snprintf_res(cmd_data, " Fail to alloc dbg_log_size %d Kbytes\n", log_size);
					return 0;
				}
				sc->cmd_data.dbg_log.size = 0;
				sc->cmd_data.dbg_log.totalsize = total_log_size;
				sc->cmd_data.dbg_log.top = sc->cmd_data.dbg_log.data;
				sc->cmd_data.dbg_log.tail = sc->cmd_data.dbg_log.data;
				sc->cmd_data.dbg_log.end = &(sc->cmd_data.dbg_log.data[total_log_size]);
			}
        	snprintf_res(cmd_data, " alloc dbg_log_size %d Kbytes\n", log_size);
		} else {
        	snprintf_res(cmd_data, " log ram_size [size] Kbytes\n");
		}
		return 0;
    } else if ((argc == 3) && (!strcmp(argv[1], "regw"))){
        
		if(!strcmp(argv[2], "enable")){
            sc->log_ctrl  |= LOG_REGW;
        } else {
            sc->log_ctrl &= ~(LOG_REGW);
        }
        snprintf_res(cmd_data, "  log regw %s\n", ((sc->log_ctrl & LOG_REGW) ? "enable": "disable"));
    
    } else if ((argc == 3) && (!strcmp(argv[1], "krack"))){
        
		if(!strcmp(argv[2], "enable")){
            sc->log_ctrl |= LOG_KRACK;
        } else {
            sc->log_ctrl &= ~(LOG_KRACK);
        }
        snprintf_res(cmd_data, "  log krack %s\n", ((sc->log_ctrl & LOG_KRACK) ? "enable": "disable"));       			
	} else {
        snprintf_res(cmd_data, " log log_to_ram [enable | disable]\n");
        snprintf_res(cmd_data, " log ram_size [size] kb\n");
        snprintf_res(cmd_data, " log [category] [param] [enablel | disable]\n\n");
		snprintf_res(cmd_data, " category: tx_desc, tx_frame, rx_desc, ampdu, beacon\n");
		snprintf_res(cmd_data, "           rate_control, rate_report, hci, hwif, regw, krack\n\n");
        snprintf_res(cmd_data, " ampdu param: ssn, dbg, err\n");
        return 0;
    }
    snprintf_res(cmd_data, "  log_ctrl 0x%x\n", sc->log_ctrl);
#endif
    return 0;
}

static int ssv_cmd_chan(struct ssv_softc *sc, int argc, char *argv[])
{
    char *endp;
    u32  ch;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    struct ieee80211_supported_band *sband;
    struct ieee80211_channel *channel;
    enum nl80211_channel_type type = NL80211_CHAN_HT20;
    bool support_chan = false;
    int i, band;
    
    if ((argc == 2) || (argc == 3)) {
        
        // fixed channel for debug mode
        if (argc == 2) {
            if (!strcmp(argv[1], "fixed") || !strcmp(argv[1], "auto")) {
                if (!strcmp(argv[1], "fixed"))
                    sc->sc_flags |= SC_OP_CHAN_FIXED; 
                if (!strcmp(argv[1], "auto"))
                    sc->sc_flags &= ~SC_OP_CHAN_FIXED; 
                
                snprintf_res(cmd_data, "\n %s channel fixed\n", ((sc->sc_flags & SC_OP_CHAN_FIXED) ? "Force" : "Clear"));
                return 0;
            }
        }
        
        ch = simple_strtoul(argv[1], &endp, 0);
       
        if (ch < 1) {
            snprintf_res(cmd_data, "\n  Channel syntax error.\n");
            return 0;
        }

        // chanel bandwidth
        if (argc == 3) {
            if (!strcmp(argv[2], "bw40")) {
                if ((ch == 3) || (ch == 4) || (ch == 5) || (ch == 6) ||
                    (ch == 7) || (ch == 8) || (ch == 9) || (ch == 10) ||
                    (ch == 11) || (ch == 38) || (ch == 42) || (ch == 46) ||
                    (ch == 50) || (ch == 54) || (ch == 58) || (ch == 62) ||
                    (ch == 102) || (ch == 106) || (ch == 110) || (ch == 114) ||
                    (ch == 118) || (ch == 122) || (ch == 126) || (ch == 130) ||
                    (ch == 134) || (ch == 138) || (ch == 142) || (ch == 151) ||
                    (ch == 155) || (ch == 159)) {
                        
                        type = NL80211_CHAN_HT40PLUS;
                        ch = ch - 2; // find center chan
                } else {
                    snprintf_res(cmd_data, "\n  Channel syntax error.\n");
                    return 0;
                }

            } else if (!strcmp(argv[2], "+")) {
                if ((ch >= 8) && (ch <= 13)) {
                    
                    snprintf_res(cmd_data, "\n  Channel syntax error.\n");
                    return 0;
                }
                type = NL80211_CHAN_HT40PLUS;

            } else if (!strcmp(argv[2], "-")) {
                if ((ch >= 1) && (ch <= 4)) {
                    
                    snprintf_res(cmd_data, "\n  Channel syntax error.\n");
                    return 0;
                }
                type = NL80211_CHAN_HT40MINUS;

            } else {
                snprintf_res(cmd_data, "\n  Channel syntax error.\n");
                return 0;
            }
        }
        
        if (argc == 2) 
            type = NL80211_CHAN_HT20; 

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0)
        for (band = 0; band < NUM_NL80211_BANDS; band++) {
#else
        for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
#endif
            if ((band == INDEX_80211_BAND_5GHZ) && !(sc->sh->cfg.hw_caps & SSV6200_HW_CAP_5GHZ))
                continue;
            
            sband = &sc->sbands[band];
            for (i = 0; i < sband->n_channels; i++) {
                channel = &sband->channels[i];
                if (ch == channel->hw_value)
                    support_chan = true;
            }
        }

        if (support_chan){
            struct ieee80211_channel chan, *pchan;

            memset(&chan, 0 , sizeof( struct ieee80211_channel));
             
            chan.hw_value = ch;
            pchan = &chan;
              
            snprintf_res(cmd_data, "\n  switch to ch %d by command...\n", ch);
            HAL_SET_CHANNEL(sc, pchan, type, false);
            snprintf_res(cmd_data, "\n  DONE!!\n");
        }
        else
            snprintf_res(cmd_data, "\n  invalid ch %d\n", ch);
       
    } else {
        snprintf_res(cmd_data, "\n ch [chan_number] [chan_type]\n");
    }
    return 0;
}

static int ssv_cmd_cali(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    if((argc == 2) || (argc == 3)) {
        HAL_CMD_CALI(sc->sh, argc, argv);
    } else {
        snprintf_res(&sc->cmd_data,"\n cali [do|show|dpd(show |enable|disable)] \n");
    }
#endif
    return 0;
}

static int ssv_cmd_init(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    if (!strcmp(argv[1], "mac")) {
        /* force RX reset to avoid pbuf allocating out-of-order address */
        SSV_PHY_ENABLE(sc->sh, false);
    
        // init mac hw
        HAL_INIT_MAC(sc->sh);

        SSV_PHY_ENABLE(sc->sh, true);
        snprintf_res(&sc->cmd_data, "\n   reload mac DONE\n");
    
    }else {
        snprintf_res(&sc->cmd_data, "\n init [mac]\n");
    }
#endif
    return 0;
}

static int ssv_cmd_rc(struct ssv_softc *sc, int argc, char *argv[])
{
    HAL_CMD_RC(sc->sh, argc, argv);
    return 0;
}

static int ssv_cmd_hwinfo(struct ssv_softc *sc, int argc, char *argv[])
{
    HAL_CMD_HWINFO(sc->sh, argc, argv);
    return 0;
}

static int ssv_cmd_txgen(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data; 
    u32 count = 0, rate = 0;
    int i;

    if (argc != 3) {
        snprintf_res(cmd_data, "\n ./cli tx_gen [count] [rate]\n"); 
        return 0;   
    }
        
    __string2u32(argv[1], &count, 0);
    __string2u32(argv[2], &rate, 0);
 
    for (i = 0; i < count; i++) { 
        HAL_CMD_TXGEN(sc->sh, (u8)(rate & 0x000000ff));
        mdelay(1);
    }
    
    snprintf_res(cmd_data, "\n tx_gen %d times done \n", count);
#endif
    return 0;
}

static int ssv_cmd_rf(struct ssv_softc *sc, int argc, char *argv[])
{
    HAL_CMD_RF(sc->sh, argc, argv);
    return 0;
}

static int ssv_cmd_hwq_limit(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    if ((argc == 3)&&(!strcmp(argv[1], "bk")||!strcmp(argv[1], "be")||!strcmp(argv[1], "vi")||!strcmp(argv[1], "vo")||!strcmp(argv[1], "mng"))) {
        HAL_CMD_HWQ_LIMIT(sc->sh, argc, argv);
    } else {
        snprintf_res(cmd_data, "%s [bk|be|vi|vo|mng] [queue limit]\n\n", argv[0]);
    }
#endif
    return 0;
}

static int ssv_cmd_efuse(struct ssv_softc *sc, int argc, char *argv[])
{
    HAL_CMD_EFUSE(sc->sh, argc, argv);
    return 0;
}

#ifdef CONFIG_ENABLE_HOST_THERMAL
static int ssv_cmd_get_temp(struct ssv_softc *sc, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    int val = 0;
    HAL_GET_TEMP(sc->sh, &val);
    snprintf_res(cmd_data, "ssv_cmd_get_temp val = %d\n\n", val);
    return 0;
}

static int ssv_cmd_temp_calibration(struct ssv_softc *sc, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    int val = 0;
    HAL_GET_TEMP(sc->sh, &val);

    SSV_DO_TEMPERATURE_COMPENSATION(sc->sh);
    snprintf_res(cmd_data, "sstar_cmd_get_temp val = %d\n\n", val);
    return  0;
}
#endif

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static int _ssv6xxx_tx_opertaion(struct ssv_softc *sc, ssv6xxx_tx_ops ops, u32 val)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    int ret = 0;

    skb = dev_alloc_skb(HOST_CMD_HDR_LEN + sizeof(u32));
    if (skb == NULL) {
        printk("%s(): Fail to alloc cmd buffer.\n", __FUNCTION__);
        return -1;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(u32));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd)+sizeof(u32));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_TX_OPS;
    host_cmd->sub_h_cmd = (u32)ops;
    host_cmd->un.dat32[0] = val;
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_TX_OPS << 16)|(u16)ops);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(u32);
    ret = HCI_SEND_CMD(sc->sh, skb);
    return 0;
}
#endif

static int ssv_cmd_fwtxops(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    char *endp;
    /**
        *  fwtxops [wait_num|wait_time|chk_point] <val>
        * (1) fwtxops wait_num <num>
        * (2) fwtxops wait_time <time_in_us>
        * (3) fwtxops wait_time_ht40 <time_in_us>
        * (4) fwtxops chk_pooint <val>
        */
    if ((argc == 3) && (!strcmp(argv[1], "wait_num")))
    {
        snprintf_res(cmd_data, " call cmd fwtxops wait_num %s\n", argv[2]);
        _ssv6xxx_tx_opertaion(sc, SSV6XXX_TX_CMD_WAIT_NUM, (u32)simple_strtoul(argv[2], &endp, 0));
    } else if ((argc == 3) && (!strcmp(argv[1], "wait_time"))) {
        snprintf_res(cmd_data, " call cmd fwtxops wait_time %s\n", argv[2]);
        _ssv6xxx_tx_opertaion(sc, SSV6XXX_TX_CMD_WAIT_TIME, (u32)simple_strtoul(argv[2], &endp, 0));
    } else if ((argc == 3) && (!strcmp(argv[1], "chk_point"))) {
        snprintf_res(cmd_data, " call cmd fwtxops chk_point %s\n", argv[2]);
        _ssv6xxx_tx_opertaion(sc, SSV6xxx_TX_CMD_CHECK_POINT, (u32)simple_strtoul(argv[2], &endp, 0));
    } else if ((argc == 3) && (!strcmp(argv[1], "wait_time_ht40"))) {
        snprintf_res(cmd_data, " call cmd fwtxops wait_time_ht40 %s\n", argv[2]);
        _ssv6xxx_tx_opertaion(sc, SSV6XXX_TX_CMD_WAIT_TIME_HT40, (u32)simple_strtoul(argv[2], &endp, 0));
    } else if ((argc == 3) && (!strcmp(argv[1], "chk_hwq_num"))) {
        snprintf_res(cmd_data, " call cmd fwtxops check hw queue number %s\n", argv[2]);
        _ssv6xxx_tx_opertaion(sc, SSV6XXX_TX_CMD_CHECK_HWQ_NUM, (u32)simple_strtoul(argv[2], &endp, 0));
    } else {
        snprintf_res(cmd_data, " fwtxops wait_num <num>\n\n");
        snprintf_res(cmd_data, " fwtxops wait_time <time_in_us>\n\n");
        snprintf_res(cmd_data, " fwtxops wait_time_ht40 <time_in_us>\n\n");
        snprintf_res(cmd_data, " fwtxops chk_point <val>\n\n");
    }
#endif
    return 0;
}

static int ssv_cmd_restart(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    static u32 u32Cnt = 0;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    //char *endp;
    /**
        *  re_test
        */
    snprintf_res(cmd_data, " re_test, %u times\n\n", ++u32Cnt);

    queue_work(sc->config_wq, &sc->hw_restart_work);
#endif
    return 0;
}

#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
static int _ssv6xxx_adapt_opertaion(struct ssv_softc *sc, ssv6xxx_adaptive_ops ops, struct ssv6xxx_adaptive_param *param)
{
    struct sk_buff *skb = NULL;
    struct cfg_host_cmd *host_cmd = NULL;
    int ret = 0;

    skb = ssv_skb_alloc(sc, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_adaptive_param));
    if(skb == NULL)
    {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return -1;
    }

    skb->data_len = HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_adaptive_param);
    skb->len = skb->data_len;
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd)+sizeof(struct ssv6xxx_adaptive_param));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_ADAPTIVE;
    host_cmd->sub_h_cmd = (u32)ops;
    if(NULL != param)
    {
        memcpy(host_cmd->un.dat32, param, sizeof(struct ssv6xxx_adaptive_param));
    }
    host_cmd->blocking_seq_no = (((u16)SSV6XXX_HOST_CMD_ADAPTIVE << 16)|(u16)ops);
    host_cmd->len = HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_adaptive_param);
    ret = HCI_SEND_CMD(sc->sh, skb);

    return 0;
}
#endif

static int ssv_cmd_adapt(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;

    /**
        *  AT+ADAPT [0|1]
        */
    if ((argc == 2) && (!strcmp(argv[1], "1"))) {
        snprintf_res(cmd_data, " call cmd AT+ADAPT 1\n");
        sc->bAdapt = true;
        cancel_work_sync(&sc->mib_edca_work);
        _ssv6xxx_adapt_opertaion(sc, SSV6XXX_ADAPTIVE_OPS_ENABLE, NULL);
    } else if ((argc == 2) && (!strcmp(argv[1], "0"))) {
        snprintf_res(cmd_data, " call cmd AT+ADAPT 0\n");
        _ssv6xxx_adapt_opertaion(sc, SSV6XXX_ADAPTIVE_OPS_DISABLE, NULL);
        sc->bAdapt = false;
    } else {
        snprintf_res(cmd_data, " AT+ADAPT [0|1]\n\n");
        snprintf_res(cmd_data, " - 0: Disable adaptive test\n\n");
        snprintf_res(cmd_data, " - 1: Enable adaptive test\n\n");
    }
#endif
    return 0;
}

static int ssv_cmd_set_adapt(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    struct ssv6xxx_adaptive_param param = {0};
    char *endp;

    /**
        *  AT+SET_ADAP <interval> <hit count> <total count> <threshold>
        */
    if (argc == 5)
    {
        snprintf_res(cmd_data, " call cmd AT+SET_ADAPT %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4]);
        param.interval = (u32)simple_strtoul(argv[1], &endp, 0);
        param.hit_count = (u32)simple_strtoul(argv[2], &endp, 0);
        param.total_count = (u32)simple_strtoul(argv[3], &endp, 0);
        param.threshold = (u32)simple_strtoul(argv[4], &endp, 0);
        _ssv6xxx_adapt_opertaion(sc, SSV6XXX_ADAPTIVE_OPS_SET, &param);
    } else {
        snprintf_res(cmd_data, " AT+SET_ADAPT <interval> <hit count> <total count> <threshold>\n\n");
    }
#endif
    return 0;
}

static int ssv_cmd_regr(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32 addr, value, cnt;
    int s, i;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    
    /* regr [address],[value] */
    if (argc == 2) {
    
        s = sscanf(argv[1], "%x,%d", &addr, &cnt);
        if (s != 2) {
            s = sscanf(argv[1], "%x", &addr);
            if (s == 1) {
                cnt = 1;
            } else {
                snprintf_res(cmd_data, " => regr [address],[cnt]\n");
                return 0;
            }
        }
        
        
        for(i = 0; i < cnt; i++, addr += 4) {
            if (SMAC_REG_READ(sc->sh, addr, &value)) ;
            snprintf_res(cmd_data, "0x%08X = 0x%08X\n", addr, value);
        }

        return 0;
    } else {
        snprintf_res(cmd_data, " => regr [address],[value]\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int ssv_cmd_regw(struct ssv_softc *sc, int argc, char *argv[])
{
#ifdef CONFIG_ENABLE_CLI_DBG_TOOL
    u32 addr, value;
    int s;
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    
    /* regw [address],[value] */
    if (argc == 2) {
    
        s = sscanf(argv[1], "%x,%x", &addr, &value);
        if (s != 2) {
            snprintf_res(cmd_data, " => regw [address,value]\n");
            return 0;
        }
        
        if (SMAC_REG_WRITE(sc->sh, addr, value)) ;
        
        snprintf_res(cmd_data, " => write [0x%08x]: 0x%08x\n", addr, value);
        return 0;
    } else {
        snprintf_res(cmd_data, " => regw [address,value]\n");
        return 0;
    }
#else
    return 0;
#endif
}

#ifdef CONFIG_ENABLE_HOST_THERMAL
static int ssv_cmd_disable_host_thermal(struct ssv_softc *sc, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    char *endp;

    if (argc == 2) 
    {
        if ((u32)simple_strtoul(argv[1], &endp, 0) == 1)
        {
            sc->sh->htc_config.exist = 0;
        }
        else
        {
            sc->sh->htc_config.exist = 1;
        }
        
        snprintf_res(cmd_data, "sc->sh->htc_config.exist, %u \n\n", sc->sh->htc_config.exist);
    
    }
    
    return 0;
}


static int ssv_cmd_debug_host_thermal(struct ssv_softc *sc, int argc, char *argv[])
{
    struct ssv_cmd_data *cmd_data = &sc->cmd_data;
    char *endp;

    if (argc == 2) 
    {
        sc->sh->cfg.host_thermal_debug = (u32)simple_strtoul(argv[1], &endp, 0);
        
        snprintf_res(cmd_data, "sc->sh->cfg.host_thermal_debug, %d \n\n", sc->sh->cfg.host_thermal_debug);
    
    }
    
    return 0;
}
#endif

struct ssv_cmd_table cmd_table[] = {
    { "help",   ssv_cmd_help,   "ssv6200 command usage."            , 2048},
    { "-h",     ssv_cmd_help,   "ssv6200 command usage."            , 2048},
    { "--help", ssv_cmd_help,   "ssv6200 command usage."            , 2048},
    { "reg",    ssv_cmd_reg,    "ssv6200 register read/write."      , 4096},
    { "regr",   ssv_cmd_regr,   "ssv6200 register read"             , 4096},
    { "regw",   ssv_cmd_regw,   "ssv6200 register write"            ,  256},
    { "cfg",    ssv_cmd_cfg,    "ssv6200 configuration."            ,  256},
    { "sta",    ssv_cmd_sta,    "svv6200 station info."             , 4096},
    { "dump",   ssv_cmd_dump,   "dump ssv6200 tables."              , 8192},
    { "hwq",    ssv_cmd_hwq,    "hardware queue staus"              ,  512},
	{ "irq",    ssv_cmd_irq,    "get sdio irq status."              ,  256},
    { "mac",    ssv_cmd_mac,    "ieee80211 swmac."                  ,  512},
    { "hci",    ssv_cmd_hci,    "HCI command."                      , 4096},
    { "sdio",   ssv_cmd_sdio,   "SDIO command."                     ,  128},
    { "version",ssv_cmd_version,"version information"               ,  512},
    { "mib",    ssv_cmd_mib,    "mib counter related"               , 2048},
    { "ps",     ssv_cmd_power_saving, "power saving test"           ,  256},
    { "tool",   ssv_cmd_tool,   "ssv6200 tool register read/write." , 4096},
    { "rxtput", ssv_cmd_rxtput, "test rx sdio throughput"           ,  128},
    { "txtput", ssv_cmd_txtput, "test tx sdio throughput"           ,  256},
    { "check",  ssv_cmd_check,  "dump all allocate packet buffer"   ,  128},
    { "directack", ssv_cmd_directack, "directly ack control"        ,  512},
    { "flowctl",ssv_cmd_flowctl,"flow control"                      ,  512},
    { "txrx_skb_q", ssv_cmd_txrx_skb_q, "tx/rx skb queue control"        ,  512},
    { "log",    ssv_cmd_log,    "enable debug log"                  ,  256},
    { "ch",     ssv_cmd_chan,   "change channel by manual"          ,  128},
    { "cali",   ssv_cmd_cali,   "calibration for ssv6006"           , 4096},
    { "init",   ssv_cmd_init,   "re-init "                          ,   64},
    { "rc",     ssv_cmd_rc,     "fix rate set for ssv6006"          , 4096},
	{ "hwmsg",  ssv_cmd_hwinfo, "hw message for ssv6006"            , 4096},
	{ "txgen",  ssv_cmd_txgen,  "auto gen tx "                      ,  128},
	{ "rf",     ssv_cmd_rf,     "change parameters for rf tool"     ,  512},
	{ "hwqlimit",     ssv_cmd_hwq_limit,     "Set software limit for hardware queue"     ,  512},
    { "efuse",  ssv_cmd_efuse,  "efuse read/write."                 , 2048},
    { "fwtxops",ssv_cmd_fwtxops,"set tx ops to fw"                  , 512},
    { "re_test",ssv_cmd_restart,"restart test"                      , 512},
#ifdef CONFIG_ENABLE_HOST_THERMAL
    { "temp", ssv_cmd_get_temp, "get temp."                         , 512},
    { "ctemp", ssv_cmd_temp_calibration, "temp calibration"         , 512},  
    { "disable_host_thermal",ssv_cmd_disable_host_thermal,"disable_host_thermal", 512},
    { "host_thermal_debug", ssv_cmd_debug_host_thermal,"host_thermal_debug", 512},
#endif	
    { "AT+ADAPT",       ssv_cmd_adapt,      "Enable/Disable adaptive"   , 512},
    { "AT+SET_ADAPT",   ssv_cmd_set_adapt,  "Set adaptive"              , 512},
    { NULL, NULL, NULL, 0},
};

int ssv_cmd_submit(struct ssv_cmd_data *cmd_data, char *cmd)
{
    struct ssv_cmd_table *sc_tbl;
    char                 *pch, ch;
    int                   ret, bf_size;
    char                 *sg_cmd_buffer;
    char                 *sg_argv[CLI_ARG_SIZE];
    u32                   sg_argc;
    
    if (cmd_data->cmd_in_proc)
        return -1;

    sg_cmd_buffer = cmd;
    
    for (sg_argc = 0, ch = 0, pch = sg_cmd_buffer;
         (*pch!=0x00) && (sg_argc<CLI_ARG_SIZE); pch++ )
    {
//        if (*pch == 0x0a || *pch == 0x0d)
//            *pch = ' ';
        if ( (ch==0) && (*pch!=' ') )
        {
            ch = 1;
            sg_argv[sg_argc] = pch;
        }
    
        if ( (ch==1) && (*pch==' ') )
        {
            *pch = 0x00;
            ch = 0;
            sg_argc ++;
        }                               
    }
    if ( ch == 1) 
    {
        sg_argc ++;
    }
    else if ( sg_argc > 0 )
    {
        *(pch-1) = ' ';
    }               
    
    if ( sg_argc > 0 )
    {
        /* Dispatch command */
        for( sc_tbl=cmd_table; sc_tbl->cmd; sc_tbl ++ )
        {
            if ( !strcmp(sg_argv[0], sc_tbl->cmd) )
            {
                struct ssv_softc         *sc;
                struct ssv6xxx_hci_info  *hci;

                sc = container_of(cmd_data, struct ssv_softc, cmd_data);
                hci = &sc->sh->hci;

                if (   (sc_tbl->cmd_func_ptr != ssv_cmd_cfg)
                    && (!hci->dev || !hci->if_ops || !sc->platform_dev)) {
                    cmd_data->ssv6xxx_result_buf = (char *)kzalloc(128, GFP_KERNEL);
                    if (!cmd_data->ssv6xxx_result_buf)
                         return -EFAULT;
                    
                    cmd_data->ssv6xxx_result_buf[0] = 0x00; 
					snprintf_res(cmd_data, "Member of ssv6xxx_ifdebug_info is NULL !\n");
					return -1;
				}

                cmd_data->ssv6xxx_result_buf = (char *)kzalloc(sc_tbl->result_buffer_size, GFP_KERNEL);
                if (!cmd_data->ssv6xxx_result_buf)
                    return -EFAULT;
		
                cmd_data->ssv6xxx_result_buf[0] = 0x00;
                cmd_data->rsbuf_len = 0;
                cmd_data->rsbuf_size = sc_tbl->result_buffer_size;
                cmd_data->cmd_in_proc = true;


                down_read(&sc->sta_info_sem);/* START protect sta_info */
                ret = sc_tbl->cmd_func_ptr(sc, sg_argc, sg_argv);
                up_read(&sc->sta_info_sem);/* END protect sta_info */
                if (ret < 0) {
                    strcpy(cmd_data->ssv6xxx_result_buf, "Invalid command !\n");
                }
                
                bf_size = strlen(cmd_data->ssv6xxx_result_buf);
                
                if ((sc_tbl->result_buffer_size -1) <= bf_size){
                    cmd_data->rsbuf_len = 0;
                    snprintf_res(cmd_data,
                       "\nALLOCATED BUFFER %d <= BUFFER USED +1 %d, OVERFLOW!!\n\n",
                       sc_tbl->result_buffer_size, bf_size+1);                   
                } else {
                    snprintf_res(cmd_data, "\nALLOCATED BUFFER %d , BUFFER USED %d\n\n",
                       sc_tbl->result_buffer_size, bf_size); 
                }

                return 0;
            }       

        }
        
        cmd_data->ssv6xxx_result_buf = (char *)kzalloc(64, GFP_KERNEL);
        if (!cmd_data->ssv6xxx_result_buf)
            return -EFAULT;
        
        cmd_data->ssv6xxx_result_buf[0] = 0x00;
        cmd_data->rsbuf_len = 0;
        cmd_data->rsbuf_size = 64;
        cmd_data->cmd_in_proc = true;                                       
	    snprintf_res(cmd_data, "Command not found !\n");
	    return -EFAULT;
	    
    } else {
        cmd_data->ssv6xxx_result_buf = (char *)kzalloc(64, GFP_KERNEL);
        if (!cmd_data->ssv6xxx_result_buf)
            return -EFAULT;
        
        
        cmd_data->ssv6xxx_result_buf[0] = 0x00;
        cmd_data->rsbuf_len = 0;
        cmd_data->rsbuf_size = 64;
        cmd_data->cmd_in_proc = true;
        snprintf_res(cmd_data, "./cli -h\n");
    }
    return 0;
}

