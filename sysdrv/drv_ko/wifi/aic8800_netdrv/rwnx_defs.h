/**
 ******************************************************************************
 *
 * @file rwnx_defs.h
 *
 * @brief Main driver structure declarations for fullmac driver
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */

#ifndef _RWNX_DEFS_H_
#define _RWNX_DEFS_H_

#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/dmapool.h>
#include <linux/skbuff.h>
#include <net/cfg80211.h>
#include <linux/slab.h>

#include "rwnx_debugfs.h"
#include "rwnx_tx.h"
#include "rwnx_rx.h"
#include "rwnx_radar.h"
#include "rwnx_utils.h"
#include "rwnx_mu_group.h"
#include "rwnx_platform.h"
#include "rwnx_cmds.h"

#ifdef AICWF_SDIO_SUPPORT
#include "aicwf_sdio.h"
#include "sdio_host.h"
#endif

#ifdef AICWF_USB_SUPPORT
#include "usb_host.h"
#endif

#define TERM_BUFFER_SIZE 4096

/**
 * struct rwnx_term - Term device data
 *
 * @semaphore: Semaphore to ensure that no msg will be sent until previous one
 * is acknowledged by fw. Acknowledge is done in tasklet so cannot use mutex.
 * @cdev: Check device structure
 * @dev: device structure
 * @buf_lock: Lock to read/write debug msg buffer
 * @buf: Share buffer to store debug msg reported by fw
 * @buf_idx: Total number of bytes written in the share buffer since
 * device creation
 * @queue: Wait queue for readers
 * @stream_list: List of streams opened on this terminal
 */
struct rwnx_term {
    struct semaphore sem;
    struct cdev cdev;
    struct device *dev;

    spinlock_t buf_lock;
    char buf[TERM_BUFFER_SIZE];
    int buf_idx;
    wait_queue_head_t queue;
    struct list_head stream_list;
};

/**
 * struct rwnx_term_stream - Private structure for one stream
 *
 * @list: Node for term->stream_list
 * @term: Pointer to &struct rwnx_term structure
 * @buf: Private buffer for the stream
 * @read: Read pointer on private buffer
 * @write: Write pointer on private buffer
 * @avail: Number of bytes availalbe in private buffer
 * @last_idx: Value of &struct rwnx_term.buf_idx at last read
 */
struct rwnx_term_stream
{
    struct list_head list;
    struct rwnx_term *term;
    char buf[2 * TERM_BUFFER_SIZE];
    char *read, *write;
    int avail, last_idx;
};

/**
 * struct rwnx_trace - Trace device data
 *
 * @cdev: Check device structure
 * @dev: device structure
 * @fw: Fw trace control structure
 */
struct rwnx_trace {
    struct cdev cdev;
    struct device *dev;
    struct rwnx_fw_trace fw;
};

#define WPI_HDR_LEN    18
#define WPI_PN_LEN     16
#define WPI_PN_OFST     2
#define WPI_MIC_LEN    16
#define WPI_KEY_LEN    32
#define WPI_SUBKEY_LEN 16 // WPI key is actually two 16bytes key

#define LEGACY_PS_ID   0
#define UAPSD_ID       1

#define PS_SP_INTERRUPTED  255
#define MAC_ADDR_LEN 6
/**
 * struct rwnx_bcn - Information of the beacon in used (AP mode)
 *
 * @head: head portion of beacon (before TIM IE)
 * @tail: tail portion of beacon (after TIM IE)
 * @ies: extra IEs (not used ?)
 * @head_len: length of head data
 * @tail_len: length of tail data
 * @ies_len: length of extra IEs data
 * @tim_len: length of TIM IE
 * @len: Total beacon len (head + tim + tail + extra)
 * @dtim: dtim period
 */
struct rwnx_bcn {
    u8 *head;
    u8 *tail;
    u8 *ies;
    size_t head_len;
    size_t tail_len;
    size_t ies_len;
    size_t tim_len;
    size_t len;
    u8 dtim;
};

/**
 * struct rwnx_key - Key information
 *
 * @hw_idx: Idx of the key from hardware point of view
 */
struct rwnx_key {
    u8 hw_idx;
};

/**
 * Structure containing information about a Mesh Path
 */
struct rwnx_mesh_path {
    struct list_head list;          /* For rwnx_vif.mesh_paths */
    u8 path_idx;                    /* Path Index */
    struct mac_addr tgt_mac_addr;   /* Target MAC Address */
    struct rwnx_sta *p_nhop_sta;    /* Pointer to the Next Hop STA */
};

struct rwnx_mesh_proxy {
    struct list_head list;          /* For rwnx_vif.mesh_proxy */
    struct mac_addr ext_sta_addr;   /* Address of the External STA */
    struct mac_addr proxy_addr;     /* Proxy MAC Address */
    bool local;                     /* Indicate if interface is a proxy for the device */
};

#if 0
/**
 * struct rwnx_csa - Information for CSA (Channel Switch Announcement)
 *
 * @vif: Pointer to the vif doing the CSA
 * @bcn: Beacon to use after CSA
 * @elem: IPC buffer to send the new beacon to the fw
 * @chandef: defines the channel to use after the switch
 * @count: Current csa counter
 * @status: Status of the CSA at fw level
 * @ch_idx: Index of the new channel context
 * @work: work scheduled at the end of CSA
 */
struct rwnx_csa {
    struct rwnx_vif *vif;
    struct rwnx_bcn bcn;
    struct rwnx_ipc_elem_var elem;
    struct cfg80211_chan_def chandef;
    int count;
    int status;
    int ch_idx;
    struct work_struct work;
};
#endif
/// Possible States of the TDLS link.
enum tdls_status_tag {
        /// TDLS link is not active (no TDLS peer connected)
        TDLS_LINK_IDLE,
        /// TDLS Setup Request transmitted
        TDLS_SETUP_REQ_TX,
        /// TDLS Setup Response transmitted
        TDLS_SETUP_RSP_TX,
        /// TDLS link is active (TDLS peer connected)
        TDLS_LINK_ACTIVE,
        /// TDLS Max Number of states.
        TDLS_STATE_MAX
};

/*
 * Structure used to save information relative to the TDLS peer.
 * This is also linked within the rwnx_hw vifs list.
 *
 */
struct rwnx_tdls {
    bool active;                /* Indicate if TDLS link is active */
    bool initiator;             /* Indicate if TDLS peer is the TDLS initiator */
    bool chsw_en;               /* Indicate if channel switch is enabled */
    u8 last_tid;                /* TID of the latest MPDU transmitted over the
                                   TDLS direct link to the TDLS STA */
    u16 last_sn;                /* Sequence number of the latest MPDU transmitted
                                   over the TDLS direct link to the TDLS STA */
    bool ps_on;                 /* Indicate if the power save is enabled on the
                                   TDLS STA */
    bool chsw_allowed;          /* Indicate if TDLS channel switch is allowed */
};


/**
 * enum rwnx_ap_flags - AP flags
 *
 * @RWNX_AP_ISOLATE Isolate clients (i.e. Don't brige packets transmitted by
 *                                   one client for another one)
 */
enum rwnx_ap_flags {
    RWNX_AP_ISOLATE = BIT(0),
};

/*
 * Structure used to save information relative to the managed interfaces.
 * This is also linked within the rwnx_hw vifs list.
 *
 */
struct rwnx_vif {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 7, 0)
    u8 address[ETH_ALEN] __aligned(sizeof(u16));
#endif
    struct list_head list;
    struct rwnx_hw *rwnx_hw;
    struct wireless_dev wdev;
    struct net_device *ndev;
    struct net_device_stats net_stats;
    struct rwnx_key key[6];
    u8 drv_vif_index;           /* Identifier of the VIF in driver */
    u8 vif_index;               /* Identifier of the station in FW */
    bool up;                    /* Indicate if associated netdev is up
                                   (i.e. Interface is created at fw level) */
};

#define RWNX_VIF_TYPE(rwnx_vif) (rwnx_vif->wdev.iftype)

/**
 * Structure used to store information relative to PS mode.
 *
 * @active: True when the sta is in PS mode.
 *          If false, other values should be ignored
 * @pkt_ready: Number of packets buffered for the sta in drv's txq
 *             (1 counter for Legacy PS and 1 for U-APSD)
 * @sp_cnt: Number of packets that remain to be pushed in the service period.
 *          0 means that no service period is in progress
 *          (1 counter for Legacy PS and 1 for U-APSD)
 */
struct rwnx_sta_ps {
    bool active;
    u16 pkt_ready[2];
    u16 sp_cnt[2];
};

/**
 * struct rwnx_rx_rate_stats - Store statistics for RX rates
 *
 * @table: Table indicating how many frame has been receive which each
 * rate index. Rate index is the same as the one used by RC algo for TX
 * @size: Size of the table array
 * @cpt: number of frames received
 */
struct rwnx_rx_rate_stats {
    int *table;
    int size;
    int cpt;
    int rate_cnt;
};

/**
 * struct rwnx_sta_stats - Structure Used to store statistics specific to a STA
 *
 * @last_rx: Hardware vector of the last received frame
 * @rx_rate: Statistics of the received rates
 */
struct rwnx_sta_stats {
//#ifdef CONFIG_RWNX_DEBUGFS
    struct hw_vect last_rx;
    struct rwnx_rx_rate_stats rx_rate;
//#endif
};

/*
 * Structure used to save information relative to the managed stations.
 */
struct rwnx_sta {
    struct list_head list;
    u16 aid;                /* association ID */
    u8 sta_idx;             /* Identifier of the station */
    u8 vif_idx;             /* Identifier of the VIF (fw id) the station
                               belongs to */
    u8 vlan_idx;            /* Identifier of the VLAN VIF (fw id) the station
                               belongs to (= vif_idx if no vlan in used) */
    //enum nl80211_band band; /* Band */
    //enum nl80211_chan_width width; /* Channel width */
    u16 center_freq;        /* Center frequency */
    u32 center_freq1;       /* Center frequency 1 */
    u32 center_freq2;       /* Center frequency 2 */
    u8 ch_idx;              /* Identifier of the channel
                               context the station belongs to */
    bool qos;               /* Flag indicating if the station
                               supports QoS */
    u8 acm;                 /* Bitfield indicating which queues
                               have AC mandatory */
    u16 uapsd_tids;         /* Bitfield indicating which tids are subject to
                               UAPSD */
    u8 mac_addr[ETH_ALEN];  /* MAC address of the station */
    struct rwnx_key key;
    bool valid;             /* Flag indicating if the entry is valid */
    struct rwnx_sta_ps ps;  /* Information when STA is in PS (AP only) */
#ifdef CONFIG_RWNX_BFMER
    struct rwnx_bfmer_report *bfm_report;     /* Beamforming report to be used for
                                                 VHT TX Beamforming */
#ifdef CONFIG_RWNX_MUMIMO_TX
    struct rwnx_sta_group_info group_info; /* MU grouping information for the STA */
#endif /* CONFIG_RWNX_MUMIMO_TX */
#endif /* CONFIG_RWNX_BFMER */

    bool ht;               /* Flag indicating if the station
                               supports HT */
    bool vht;               /* Flag indicating if the station
                               supports VHT */
    u32 ac_param[AC_MAX];  /* EDCA parameters */
    struct rwnx_tdls tdls; /* TDLS station information */
    struct rwnx_sta_stats stats;
    //enum nl80211_mesh_power_mode mesh_pm; /*  link-specific mesh power save mode */
};

static inline const u8 *rwnx_sta_addr(struct rwnx_sta *rwnx_sta) {
    return rwnx_sta->mac_addr;
}

#ifdef CONFIG_RWNX_SPLIT_TX_BUF
struct rwnx_amsdu_stats {
    int done;
    int failed;
};
#endif

struct rwnx_stats {
    int cfm_balance[NX_TXQ_CNT];
    unsigned long last_rx, last_tx; /* jiffies */
    int ampdus_tx[IEEE80211_MAX_AMPDU_BUF];
    int ampdus_rx[IEEE80211_MAX_AMPDU_BUF];
    int ampdus_rx_map[4];
    int ampdus_rx_miss;
#ifdef CONFIG_RWNX_SPLIT_TX_BUF
    struct rwnx_amsdu_stats amsdus[NX_TX_PAYLOAD_MAX];
#endif
    int amsdus_rx[64];
};

struct rwnx_sec_phy_chan {
    u16 prim20_freq;
    u16 center_freq1;
    u16 center_freq2;
    enum nl80211_band band;
    u8 type;
};

/* Structure that will contains all RoC information received from cfg80211 */
struct rwnx_roc_elem {
    struct wireless_dev *wdev;
    struct ieee80211_channel *chan;
    unsigned int duration;
    /* Used to avoid call of CFG80211 callback upon expiration of RoC */
    bool mgmt_roc;
    /* Indicate if we have switch on the RoC channel */
    bool on_chan;
};

/* Structure containing channel survey information received from MAC */
struct rwnx_survey_info {
    // Filled
    u32 filled;
    // Amount of time in ms the radio spent on the channel
    u32 chan_time_ms;
    // Amount of time the primary channel was sensed busy
    u32 chan_time_busy_ms;
    // Noise in dbm
    s8 noise_dbm;
};

#define RWNX_CH_NOT_SET 0xFF
#define RWNX_INVALID_VIF 0xFF
#define RWNX_INVALID_STA 0xFF

#if 0
/* Structure containing channel context information */
struct rwnx_chanctx {
    struct cfg80211_chan_def chan_def; /* channel description */
    u8 count;                          /* number of vif using this ctxt */
};
#endif

/**
 * rwnx_phy_info - Phy information
 *
 * @phy_cnt: Number of phy interface
 * @cfg: Configuration send to firmware
 * @sec_chan: Channel configuration of the second phy interface (if phy_cnt > 1)
 * @limit_bw: Set to true to limit BW on requested channel. Only set to use
 * VHT with old radio that don't support 80MHz (deprecated)
 */
struct rwnx_phy_info {
    u8 cnt;
    struct phy_cfg_tag cfg;
    struct rwnx_sec_phy_chan sec_chan;
    bool limit_bw;
};

struct rwnx_hw {
    struct device *dev;
    struct rwnx_plat *plat;
    struct tasklet_struct task;
    struct class* class;
    int major;

    struct rwnx_term term;
    struct rwnx_trace trace;

    spinlock_t cb_lock;

    //struct device *dev;
#ifdef AICWF_SDIO_SUPPORT
    struct aic_sdio_dev *sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
    struct aic_usb_dev *usbdev;
#endif
    struct wiphy *wiphy;
    struct list_head vifs;
    struct rwnx_vif *vif_table[NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX]; /* indexed with fw id */
    struct rwnx_sta sta_table[NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX];
    struct rwnx_survey_info survey[SCAN_CHANNEL_MAX];
    struct cfg80211_scan_request *scan_request;
    //struct rwnx_chanctx chanctx_table[NX_CHAN_CTXT_CNT];
    u8 cur_chanctx;

    u8 monitor_vif; /* FW id of the monitor interface, RWNX_INVALID_VIF if no monitor vif at fw level */

    /* RoC Management */
    struct rwnx_roc_elem *roc_elem;             /* Information provided by cfg80211 in its remain on channel request */
    u32 roc_cookie_cnt;                         /* Counter used to identify RoC request sent by cfg80211 */

    struct rwnx_cmd_mgr *cmd_mgr;

    unsigned long drv_flags;
    //struct rwnx_plat *plat;

    spinlock_t tx_lock;
    //spinlock_t cb_lock;
    struct mutex mutex;                         /* per-device perimeter lock */

    //struct tasklet_struct task;
    struct mm_version_cfm version_cfm;          /* Lower layers versions - obtained via MM_VERSION_REQ */

    u32 tcp_pacing_shift;

    /* IPC */
   // struct ipc_host_env_tag *ipc_env;
#ifdef AICWF_SDIO_SUPPORT
    struct sdio_host_env_tag sdio_env;
#endif
#ifdef AICWF_USB_SUPPORT
    struct usb_host_env_tag usb_env;
#endif

    struct rwnx_ipc_elem_pool e2amsgs_pool;
    struct rwnx_ipc_elem_pool dbgmsgs_pool;
    struct rwnx_ipc_elem_pool e2aradars_pool;
    struct rwnx_ipc_elem_var pattern_elem;
    struct rwnx_ipc_dbgdump_elem dbgdump_elem;
    struct rwnx_ipc_elem_pool e2arxdesc_pool;
    struct rwnx_ipc_skb_elem *e2aunsuprxvec_elems;
    struct rwnx_ipc_rxbuf_elems rxbuf_elems;
    struct rwnx_ipc_elem_var scan_ie;

    struct kmem_cache      *sw_txhdr_cache;

    struct rwnx_debugfs     debugfs;
    struct rwnx_stats       stats;

    //struct rwnx_txq txq[NX_NB_TXQ];
    //struct rwnx_hwq hwq[NX_TXQ_CNT];

    u8 avail_idx_map;
    u8 vif_started;
    bool adding_sta;
    struct rwnx_phy_info phy;

    struct rwnx_radar radar;

    /* extended capabilities supported */
    u8 ext_capa[8];

#ifdef CONFIG_RWNX_MUMIMO_TX
    struct rwnx_mu_info mu;
#endif
    u8 is_p2p_alive;
    u8 is_p2p_connected;
    struct timer_list p2p_alive_timer;
    struct rwnx_vif *p2p_dev_vif;
    atomic_t p2p_alive_timer_count;
};

#if 0
u8 *rwnx_build_bcn(struct rwnx_bcn *bcn, struct cfg80211_beacon_data *new);

void rwnx_chanctx_link(struct rwnx_vif *vif, u8 idx,
                        struct cfg80211_chan_def *chandef);
void rwnx_chanctx_unlink(struct rwnx_vif *vif);
int  rwnx_chanctx_valid(struct rwnx_hw *rwnx_hw, u8 idx);
#endif
static inline bool is_multicast_sta(int sta_idx)
{
    return (sta_idx >= NX_REMOTE_STA_MAX);
}
struct rwnx_sta *rwnx_get_sta(struct rwnx_hw *rwnx_hw, const u8 *mac_addr);

void rwnx_external_auth_enable(struct rwnx_vif *vif);
void rwnx_external_auth_disable(struct rwnx_vif *vif);

/////////
void rwnx_term_stream_write_mod(struct rwnx_term_stream *stream, char *src, unsigned int size);
void rwnx_term_write_shared(struct rwnx_term *term, char *data, size_t size);
void rwnx_term_ops_exit(void);
void rwnx_trace_work(struct work_struct *ws);
int rwnx_trace_pre_reload(struct rwnx_hw *rwnx_hw);
void rwnx_trace_post_reload(struct rwnx_hw *rwnx_hw);
void rwnx_fw_trace_dump(struct rwnx_hw *rwnx_hw);

int rwnx_fhost_send_msg(struct rwnx_hw *rwnx_hw, struct rwnx_term_stream *stream, const char *msg, char *resp);
//void rwnx_fhost_rx_handle_msg(struct rwnx_hw *rwnx_hw, struct ipc_fhost_msg *msg);

#endif /* _RWNX_DEFS_H_ */
