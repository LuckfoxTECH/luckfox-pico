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

#ifndef _SSV_CFG_H_
#define _SSV_CFG_H_



/**
* SSV6200 Hardware Capabilities:
*
* @ SSV6200_HW_CAP_HT: hardware supports HT capability.
* @ SSV6200_HW_CAP_LDPC:
* @ SSV6200_HW_CAP_2GHZ:
* @ SSV6200_HW_CAP_5GHZ:
* @ SSV6200_HW_CAP_DFS:
* @ SSV6200_HW_CAP_SECUR:
*/
#define SSV6200_HW_CAP_HT                   0x00000001
#define SSV6200_HW_CAP_GF                   0x00000002
#define SSV6200_HW_CAP_2GHZ                 0x00000004
#define SSV6200_HW_CAP_5GHZ                 0x00000008
#define SSV6200_HW_CAP_SECURITY             0x00000010
#define SSV6200_HW_CAP_SGI                  0x00000020
#define SSV6200_HW_CAP_HT40                 0x00000040
#define SSV6200_HW_CAP_AP                   0x00000080
#define SSV6200_HW_CAP_P2P                  0x00000100
#define SSV6200_HW_CAP_AMPDU_RX             0x00000200
#define SSV6200_HW_CAP_AMPDU_TX             0x00000400
#define SSV6200_HW_CAP_TDLS                 0x00000800
#define SSV6200_HW_CAP_STBC                 0x00001000
#define SSV6200_HW_CAP_HCI_RX_AGGR          0x00002000
#define SSV6200_HW_CAP_BEACON               0x00004000
#define SSV6200_HW_CAP_KRACK                0x00008000
#define SSV6200_HW_CAP_WOW                  0x00010000
/* SSV6200_HW_CAP_BQ4
	This is a legacy workaround for Cabrio(6051), due to SSV_UNHALT_MNGQ_UNTIL_DTIM and SSV_HALT_MNGQ_UNTIL_DTIM is only available in ssv6051.
	While BQ4=true, hw_txqid_3 instead of hw_txqid_4_mgmt is assigned to management frames, but hw_txqid_0/1/2/3 are blocked for off_channel.
	P2P utilizes off_channel to process management frames(Probe_Req/Resp), therefore, BQ4 can NOT be true for P2P.
	Only apply BQ4 for Cabrio to prevent from blocking Softap+P2P development for Turismo.
*/
#define SSV6200_HW_CAP_BQ4                  0x00020000
#define SSV6200_HW_CAP_HCI_TX_AGGR          0x00040000

#define SSV6200_HW_CAP_REPORT_TX_ACK        0x00080000

#define EXTERNEL_CONFIG_SUPPORT             64


/* define USB HW RESOURCE CHECK*/
#define USB_HW_RESOURCE_CHK_NONE            0x00000000
#define USB_HW_RESOURCE_CHK_TXID            0x00000001
#define USB_HW_RESOURCE_CHK_TXPAGE          0x00000002
#define USB_HW_RESOURCE_CHK_SCAN            0x00000004
#define USB_HW_RESOURCE_CHK_FORCE_OFF       0x00000008

/* define Online Reset */
#define ONLINE_RESET_ENABLE                 0x00000100
#define ONLINE_RESET_EDCA_THRESHOLD_MASK    0x000000ff
#define ONLINE_RESET_EDCA_THRESHOLD_SFT     0

/* define use_sw_cipher configure to use mac80221 sw encrypt/decrypt*/
/* use mac80211 cipher should use tx as defaults, rx can be turned off */
#define USE_MAC80211_TRX                    0x1
#define USE_MAC80211_TX_ONLY                0x2
#define USE_MAC80211_RX(_sh)                ((_sh->cfg.use_sw_cipher & USE_MAC80211_TRX) && (!(_sh->cfg.use_sw_cipher & USE_MAC80211_TX_ONLY)))
#define USE_MAC80211_CIPHER(_sh)            (_sh->cfg.use_sw_cipher != 0)


/***************************************************************************
 * ssv6xxx driver configuration
 * 
 * Note:
 *   1. Use u32 for flag because the compiled code is more efficient then using
 *      u8.
 ***************************************************************************/
struct ssv6xxx_cfg {
    /**
     * ssv6200 hardware capabilities sets.
     */
    u32     hw_caps;

    /**
     * The default channel once the wifi system is up.
     */
    u32     def_chan;

    //0-26M 1-40M 2-24M
    u32     crystal_type;
    //(DCDC-0 LDO-1)
    u32     volt_regulator;
    u32     force_chip_identity;

    u32     ignore_firmware_version;
	
    /**
     * The mac address of Wifi STA .
     */
    u8      maddr[2][6];
    u32     n_maddr;
    // Force to use sw cipher only
    u32     use_sw_cipher;
    // Force to use WPA2 only such that all virtual interfaces use hardware security.
    u32     use_wpa2_only;
    // online reset for rx stuck
    // bit[8] disable/enable, bit[0~7] edca threshold
    u32     online_reset;
    // detect tx stuck
    bool    tx_stuck_detect; 

    //E-fuse configuration
    u32     r_calbration_result;
    u32     sar_result;
    u32     crystal_frequency_offset;
    //u16 iq_calbration_result;
    u32     tx_power_index_1;
    u32     tx_power_index_2;
    u32     chip_identity;
    u32     rate_table_1;
    u32     rate_table_2;

    u32     wifi_tx_gain_level_gn;
    u32     wifi_tx_gain_level_b;

    u32     configuration[EXTERNEL_CONFIG_SUPPORT+1][2];

    //Firmware path
    //Read from wifi.cfg
    u8      firmware_path[128];
    u8      flash_bin_path[128];
    u8      external_firmware_name[128];

    //MAC address
    //Read from wifi.cfg
    u8      mac_address_path[128];
    u8      mac_output_path[128];
    u32     ignore_efuse_mac;
    // bit0 b mode, bit1 legacy mode, bit2 ht20, bit3 ht40
    u32     efuse_rate_gain_mask;
    u32     mac_address_mode;
    
    u32     beacon_rssi_minimal;
   
    // turismo fix rate control
    u32     auto_rate_enable;
    u32     rc_rate_idx_set;
    u32     rc_retry_set;
    u32     rc_mf;
    u32     rc_long_short;
    u32     rc_ht40;
    u32     rc_phy_mode;
    u32		rc_log;

	//tx buff threshold
	u32 	tx_page_threshold;
	
	// ampdu releated 
	u32     max_rx_aggr_size;
    u32     ampdu_pkt_max_size; // unit byte

    //HW rx aggregation
    u32     hw_rx_agg_cnt;
    bool    hw_rx_agg_method_3;
    u32     hw_rx_agg_timer_reload;
    u32     rx_max_recv_cnt;

    //SW take care of the HW resource for USB
    u32     usb_hw_resource;    // bit 0: check hci resource, bit 1: check tx page resouce.

    // clock source
    bool    clk_src_80m;
    
    // rts threshold len
    u32     rts_thres_len;
    
    //hwq limit
    u32     bk_txq_size;
    u32     be_txq_size;
    u32     vi_txq_size;
    u32     vo_txq_size;
    u32     manage_txq_size;
    
    // performance tuning
    u32 rx_threshold;
    bool flowctl;
    u32 flowctl_low_threshold;
    u32 flowctl_high_threshold;
    
    // auto sgi
    u32  auto_sgi;   
        
    // external pa settings
    bool external_pa;
    u32 external_lna_2g;        /* bit[0:7]: lna trigger,  bit[8:15]: on_offset, bit[16:23]: off_offset*/
    u32 external_lna_5g;        /* bit[0:7]: lna trigger,  bit[8:15]: on_offset, bit[16:23]: off_offset*/
    
    // auto edcca
    bool auto_edcca;
    
    // enhance rx throughput by align IP payload address to 4n+2
    bool rx_ip_align;

    // enhance rx throughput by turn off IP checksum
    bool rx_ip_csum_off;

    // max continuous processed rx skb num under non_preempt linux
    u32 rx_non_preempt_num;

    // country code
    u8  ctry[12];

    // directly ACK control
    bool directly_ack_ctrl;
    // low power mode control
    u32 powermode;
    u32 lp_keepalive;
    u32 lp_userdtim;
    u32 lp_wakeuppin;
    u32 lp_ignoredisconnection;
    u32 lp_dataretx_guard;
    u32 lp_beaconloss_guard;
    u32 lp_beaconloss_maxcnt;

    // all queues to follow the VO setting.
    bool wmm_follow_vo;
    bool backoff_enable;     
    u32  backoff_value;     

    //rc mask
    u32 rc_mask;
    // scan home/off channel period
    u32 scan_hc_period;
    u32 scan_oc_period;
    // fw status check
    bool fw_reset;
    u32 fw_status_idle_cnt;
    // fw tx ops
    u32 fw_tx_waitnum;
    u32 fw_tx_chkhwqnum;
    u32 fw_tx_duration;
    u32 fw_tx_duration_period;
    // ampdu rx size capability, 0: 8K, 1: 16K, 2: 32K, 3:64K
    u32 ampdu_rx_size_cap;
    // GPIO control for low power
    u32 sleep_gpio_ctrl;    //bit mask, 0: pull down; 1: pull up
    u32 dormant_gpio_ctrl;  //bit mask, 0: pull down; 1: pull up
    // trigger hci task
    bool hci_trigger_en;
    u32 hci_trigger_qlen;
    u32 hci_trigger_pkt_size;
    u32 hci_task_timeout;
    // disable fw temperature 
    u32 disable_fw_thermal; 
    s32 rf_band5100_gain_offset;
    s32 rf_band5500_gain_offset;
    s32 rf_band5700_gain_offset;
    s32 rf_band5900_gain_offset;
    s32 rf_band_gain_offset;
    s32 rf_b_rate_offset;
    s32 rf_g_rate_offset;
    s32 rf_ht20_rate_offset;
    s32 rf_ht40_rate_offset;
    // bit 0: rt, bit 1: lt, bit 2: ht
    u32 padpd;
    bool disable_greentx;
    bool disable_cci;

#ifdef CONFIG_ENABLE_HOST_THERMAL
    u32 disable_host_thermal; 
    s32 host_thermal_debug; 
    u8	temp_compensation_table[512];
#endif	
    //force TIM map to 0xFF always
    u32 ap_force_tim_always_high;
};

#endif /* _SSV_CFG_H_ */

