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
*  SSV6030 Hardware Capabilities:
*  HW_CAP_AMPDU_RX
*  HW_CAP_AMPDU_TX
*/
#define HW_CAP_AMPDU_RX             (0x00000001)
#define HW_CAP_AMPDU_TX             (0x00000002)
#define HW_CAP_HCI_RX_AGGR          (0x00000004)
#define HW_CAP_HCI_TX_AGGR          (0x00000008)


#define SSV_FIRMWARE_PATH_MAX       (256)
#define SSV_FIRMWARE_NAME_MAX       (128)

/**
*  SSV Module test settings:
*/
#define SSV_MOD_TEST_HCI_RX         (0x00000001)
#define SSV_MOD_TEST_HCI_TX         (0x00000002)

#define SSV_MOD_TEST_DELAY_FIXED    (0x00000000)
#define SSV_MOD_TEST_DELAY_RANDOM   (0x00000001)

/**
*  SSV Performance Watch Setting:
*/
#define SSV_WATCH_CPU_THROUGHT_TIME_MS 1000

struct ssv6xxx_cfg {
    u8      maddr[2][6];
    u32     hw_caps;    //Hardware Capabilities
    u32     scan_period;
    u8      firmware_path[SSV_FIRMWARE_PATH_MAX];
    u8      firmware_name[SSV_FIRMWARE_NAME_MAX];
    u32     firmware_choice;
    bool    hci_trigger_en;
    u32     hci_trigger_qlen;
    u32     hci_idle_period;
    // performance tuning
    bool    flowctl;
    u32     flowctl_low_threshold;
    u32     flowctl_high_threshold;
    bool    cca;
    bool    greentx_en;
    u32     disable_fw_thermal;
    u32     sta_max_reconnect_times;
#ifdef SSV_MODULE_TEST
    // module test
    u32     mod_test_mask;
    u32     mod_test_delay_mode;
    u32     mod_test_delay_min;
    u32     mod_test_delay_max;
#endif
    u32     filter_duplicate_rx;
    u32     wmm_follow_vo;

    //for fw reset use
    bool fw_reset;
    u32 fw_status_idle_cnt;
    u32 ignore_firmware_version;

    //for log dbg
    u32 dbg_level;
    u32 dump_fmac_msg_rx;

    // directly ACK control
    bool directly_ack_ctrl;

    u32 xtal_clock;
    int thermal_xtal_offset[5];
    int thermal_wifi_gain_offset[5];
    int thermal_ble_gain_offset[5];

    int thermal_boundary[4];

    u32     ipc_tx_use_one_hwq;

    //for set rts method
    u8 set_rts_method;

    //config ampdu tx lifetime for sw retry
    u32 ampdu_tx_lft;
    u32 txq_credit_boundary;

    // for ble
    u32  ble_replace_scan_interval;
    u32  ble_replace_scan_win;
    u32  ble_dtm;
    
	//support mifi feature
    u32 mifi_feature;
    u32 mifi_no_traffic_duration;
    
    // for bandwidth HT20/HT40
    bool use_2040;
    bool use_ep0_rw_reg;

    //custom regdom-channel bitwise
    u32 channel_list_2p4g;

    // for he on/off
    bool he_on;
    
    // mac address rule
    u32  mac_low_mask;
    u32  mac_high_mask;
};

#endif /* _SSV_CFG_H_ */

