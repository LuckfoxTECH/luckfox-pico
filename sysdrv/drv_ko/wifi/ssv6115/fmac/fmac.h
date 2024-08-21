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

#ifndef _FMAC_H_
#define _FMAC_H_


#include "utils/debugfs.h"
#include "fmac/fmac_defs.h"
#include "fmac/fmac_tx.h"
#include "hci/ssv_hci.h"

struct ssv6xxx_ping {
    // ping
#if 0
    struct task_struct *ping_tsk;
    unsigned long ping_count;
#endif
    volatile bool result;
    unsigned long start;
    unsigned int seq;
};

struct _ssv6xxx_txtput{
    struct task_struct *txtput_tsk;
    struct sk_buff *skb;
    u32 size_per_frame;
    u32 loop_times;
    u32 occupied_tx_pages;
};

/**
* struct ssv_tx - tx queue for outgoing frames through interface.
* Each AC queue uniquely associates with a hardware tx queue.
*/
struct ssv_tx {
    u32 tx_count;
    u32 tx_mgmt_count;
    u32 tx_data_count;
    u32 tx_auth_count;
    u32 tx_deauth_count;
    u32 tx_assoc_req_count;
    u32 tx_assoc_resp_count;
    u32 tx_probe_req_count;
    u32 tx_probe_resp_count;
    u32 tx_arp_req_count;
    u32 tx_arp_reply_count;
    u32 tx_icmp_echo;
    u32 tx_icmp_echoreply;
    u32 tx_dhcp_discv;
    u32 tx_dhcp_offer;
    u32 tx_dhcp_req;
    u32 tx_dhcp_ack;
    u32 tx_eapol;
    u32 flowctl_stop_count;
    u32 flowctl_wake_count;
};

/**
* struct ssv_rx, mib count 
*/
struct ssv_rx {
    u32 rx_count;
    u32 rx_data_count;
    u32 rx_mgmt_count;
    u32 rx_bcn_count;
    u32 rx_proberesp_count;
    u32 rx_probereq_count;
    u32 rx_assoc_req_count;
    u32 rx_assoc_resp_count;
    u32 rx_auth_count;
    u32 rx_disassoc_count;
    u32 rx_deauth_count;
    u32 rx_reord_count;
    u32 rx_arp_req_count;
    u32 rx_arp_reply_count;
    u32 rx_icmp_echo;
    u32 rx_icmp_echoreply;
    u32 rx_dhcp_discv;
    u32 rx_dhcp_offer;
    u32 rx_dhcp_ack;
    u32 rx_dhcp_req;
    u32 rx_eapol;
};

enum data_frame_types {
    SSV_ARP_REPLY = 1,
    SSV_ARP_REQUEST = 2,
    SSV_ICMP_ECHO = 3,
    SSV_ICMP_ECHOREPLY = 4,
    SSV_DHCP_DISCOVER = 5,
    SSV_DHCP_OFFER = 6,
    SSV_DHCP_REQUEST = 7,
    SSV_DHCP_ACK = 8,
    SSV_EAPOL = 9,

    SSV_DATA_UNKNOW = 0xFF, //keep last
};

struct fw_reset_cmd {
    struct list_head list;
    u32 msg_type;
    u32 msg_len;
    u8 data[0];
}__packed;


struct ssv_sta_reconnect_info{
    u32 auth_type;
    bool connect_retry;
    int retry_times;

};

/**
* struct ssv_softc - hold the whole wifi driver data structure.
*
*/
struct ssv_softc {
    struct wiphy                *wiphy;
    struct device               *dev;
    void                        *hci_priv;
    struct ssv6xxx_hci_ops      *hci_ops;

 #if 1 //fmac related.
    struct ssv_mod_params *mod_params;
    bool use_phy_bw_tweaks;
    struct list_head vifs;
    struct ssv_vif *vif_table[NX_VIRT_DEV_MAX]; /* indexed with fw id */
    struct ssv_sta sta_table[NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX];
    struct ssv_survey_info survey[SCAN_CHANNEL_MAX];
    struct cfg80211_scan_request *scan_request;
    struct ssv_chanctx chanctx_table[NX_CHAN_CTXT_CNT];
    u8 cur_chanctx;
    struct ssv_sta_reconnect_info sta_reconnect_info;
    struct semaphore reconnect_sem;

    /* RoC Management */
    struct ssv_roc_elem *roc_elem;              /* Information provided by cfg80211 in its remain on channel request */
    u32 roc_cookie_cnt;                         /* Counter used to identify RoC request sent by cfg80211 */

    struct ssv_cmd_mgr cmd_mgr;

    unsigned long drv_flags;
    struct ipc_host_env_tag *ipc_env;           /* store the IPC environment */

    struct mutex cmd_lock;
    struct mutex cb_lock;       //mutex to protect code from FW confirmation/indication message and cfg80211 operation

    struct mm_version_cfm version_cfm;          /* Lower layers versions - obtained via MM_VERSION_REQ */
    u32 cmd_sent;

    struct ssv_sec_phy_chan sec_phy_chan;
    u8 phy_cnt;
    u8 avail_idx_map;
    u8 vif_started;
    bool adding_sta;
    struct phy_cfg_tag phy_config;

    u8 recovery_flag; //temp add for fw recovery flag
 #endif

    int8_t rssival[RSSI_MAX];
    s16 rssiaccu;
    u8 rssicnt;
    u8 rssiidx;

    u16 rate_config;    //[15:14] Protection, [13:11] Modulation, [10:9] Guard Interval, [8:7] Bandwidth, [6:0] MCS index   
    u16 rate_reserve; //[7:6] phy mode, [5] ht40, [4] long/short GI, [3:0] rate index
    /* extended capabilities supported */

    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
    u8 ext_capa[10];
    #else
    u8 ext_capa[8];
    #endif

   //flow control
   bool flowctl_txq_stop;

   //for txtput
   struct _ssv6xxx_txtput ssv_txtput;
   // rxtput
   unsigned long   throughput_timestamp;
   unsigned long   rx_evt_size;
   //for ping
   struct ssv6xxx_ping ssv_ping;

    u16 now_ack_seq;
    struct sk_buff_head wait_sw_ack_q;

   struct ssv_tx tx;
   struct ssv_rx rx;
   struct ssv_tx tx_bysta[NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX];
   struct ssv_rx rx_bysta[NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX];
   u8 dump_level;
   int (*pktrec)(struct sk_buff *rx_skb, struct sk_buff *tx_skb);

   u8 phy_ch_bw;
   struct cfg80211_chan_def monitor_chandef; /* channel description */
   u8      maddr[2][6];
   u8      if_mode[2][2];

    //for fw reset use, softap need
   u16 center_freq;   
   enum nl80211_channel_type chan_type;
   //for fw reset use, check fw alive need
   int fw_cur_status_idle_time;
   u32 fw_cur_status_cnt;
   u32 fw_pre_status_cnt;
   //for fw reset use, reset process need
   u8 fw_reset_run;
   struct work_struct fw_reset_work;
   struct workqueue_struct *fw_reset_wq;
   struct fw_reset_cmd reset_cmd;
   //for fw reset use, debug cmd
   bool skip_ke;
#ifdef SSV_PERFORMANCE_WATCH
   //for performance watch use
   bool skip_fmac_to_hci;
   u32 watch_time;
   u32 watch_type;
   u32 watch_stop;
   struct task_struct *watch_tsk;
   unsigned long tx_run_cnt;
   unsigned long tx_run_size;
   unsigned long tx_run_time;
   u32 sw_txq_res[SSV_SW_TXQ_NUM][6];
#endif
    
   //stoping apm
   bool is_stoping_apm;

   //firmware version
   u32 fw_version;
};

#endif
