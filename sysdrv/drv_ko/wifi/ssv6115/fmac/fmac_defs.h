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

/*
 * Common defines and declarations for host driver and firmware for all
 * platforms.
 */


#ifndef __FMAC_DEFS_H__
#define __FMAC_DEFS_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <net/cfg80211.h>

#include "fmac_mod_params.h"
#include "fmac_cmds.h"
#include "fmac_rx.h"
//#include "fmac_tx.h"
#include "compat.h"


/*******************************************************************************
 *         Defines
 ******************************************************************************/
#define WPI_HDR_LEN    18
#define WPI_PN_LEN     16
#define WPI_PN_OFST     2
#define WPI_MIC_LEN    16
#define WPI_KEY_LEN    32
#define WPI_SUBKEY_LEN 16 // WPI key is actually two 16bytes key

#define LEGACY_PS_ID   0
#define UAPSD_ID       1

#define SSV_VIF_TYPE(ssv_vif)               (ssv_vif->wdev.iftype)

#define SSV_CH_NOT_SET                      (0xFF)

/** 2K buf size */
#define SSV_TX_DATA_BUF_SIZE_16K            (16*1024)
#define SSV_RX_DATA_BUF_SIZE_16K            (16*1024)

#define SSV_TX_DATA_BUF_SIZE_32K            (32*1024)

#define SSV_SDIO_MP_AGGR_PKT_LIMIT_MAX      (8)
#define SSV_SDIO_MPA_ADDR_BASE              (0x1000)

#define SSV_TX_HDR_SIZE (sizeof(struct tx_bmu_desc) /* + RESERVED_TX_SIZE */ +sizeof(struct txdesc_api) + sizeof(struct sdio_hdr) + SSV_SWTXHDR_ALIGN_SZ)

/*******************************************************************************
 *         Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Structures
 ******************************************************************************/
/**
 * struct ssv_bcn - Information of the beacon in used (AP mode)
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
struct ssv_bcn {
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
 * struct ssv_key - Key information
 *
 * @hw_idx: Idx of the key from hardware point of view
 */
struct ssv_key {
    u8 hw_idx;
};

/**
 * struct ssv_csa - Information for CSA (Channel Switch Announcement)
 *
 * @vif: Pointer to the vif doing the CSA
 * @bcn: Beacon to use after CSA
 * @dma: DMA descriptor to send the new beacon to the fw
 * @chandef: defines the channel to use after the switch
 * @count: Current csa counter
 * @status: Status of the CSA at fw level
 * @ch_idx: Index of the new channel context
 * @work: work scheduled at the end of CSA
 */
struct ssv_csa {
    struct ssv_vif *vif;
    struct ssv_bcn bcn;
    // struct ssv_dma_elem dma;
    struct cfg80211_chan_def chandef;
    int count;
    int status;
    int ch_idx;
    struct work_struct work;
};

/*
 * Structure used to save information relative to the managed interfaces.
 * This is also linked within the ssv_hw vifs list.
 *
 */
struct ssv_vif {
    struct list_head list;
    struct ssv_softc *sc;
    struct wireless_dev wdev;
    struct net_device *ndev;
    struct net_device_stats net_stats;
    bool bchan_setting;
    struct ieee80211_channel chan_setting;
    enum nl80211_channel_type chan_type_setting;
    struct ssv_key key[6];
    u8 drv_vif_index;           /* Identifier of the VIF in driver */
    //u8 vif_index;               /* Identifier of the station in FW */
    u8 ch_index;                /* Channel context identifier */
    bool up;                    /* Indicate if associated netdev is up
                                   (i.e. Interface is created at fw level) */
    bool use_4addr;             /* Should we use 4addresses mode */
    bool is_resending;          /* Indicate if a frame is being resent on this interface */
    bool user_mpm;              /* In case of Mesh Point VIF, indicate if MPM is handled by userspace */
    bool roc_tdls;              /* Indicate if the ROC has been called by a
                                   TDLS station */
    u8 tdls_status;             /* Status of the TDLS link */
    bool need_port_control;     /* IEEE 802.1X */
    bool use_monitor;
    union
    {
        struct
        {
            u32 flags;
            struct ssv_sta *ap; /* Pointer to the peer STA entry allocated for
                                    the AP */
            struct ssv_sta *tdls_sta; /* Pointer to the TDLS station */
        } sta;
        struct
        {
            u16 flags;                 /* see bl_ap_flags */
            struct list_head sta_list; /* List of STA connected to the AP */
            struct ssv_bcn bcn;       /* beacon */
            u8 bcmc_index;             /* Index of the BCMC sta to use */
            struct ssv_csa *csa;

            struct list_head mpath_list; /* List of Mesh Paths used on this interface */
            struct list_head proxy_list; /* List of Proxies Information used on this interface */
            bool create_path;            /* Indicate if we are waiting for a MESH_CREATE_PATH_CFM
                                            message */
            int generation;              /* Increased each time the list of Mesh Paths is updated */
        } ap;
        struct
        {
            struct ssv_vif *master;   /* pointer on master interface */
            struct ssv_sta *sta_4a;
        } ap_vlan;
    };
    u16 ch_hint;
    u8 chtype_hint;
#ifdef FMAC_BRIDGE
	void *bridge_priv;
#endif
    //for trx stats
    unsigned long rx_total_cnt;
    unsigned long rx_total_byte;
    unsigned long tx_total_cnt;
    unsigned long tx_total_byte;
};

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
struct ssv_sta_ps {
    bool active;
    u16 pkt_ready[2];
    u16 sp_cnt[2];
};

/*
 * Structure used to save information relative to the managed stations.
 */
struct ssv_sta {
    struct list_head list;
    u16 aid;                /* association ID */
    u8 sta_idx;             /* Identifier of the station */
    u8 vif_idx;             /* Identifier of the VIF (fw id) the station
                               belongs to */
    u8 vlan_idx;            /* Identifier of the VLAN VIF (fw id) the station
                               belongs to (= vif_idx if no vlan in used) */
    enum nl80211_band band; /* Band */
    enum nl80211_chan_width width; /* Channel width */
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
    struct ssv_key key;
    bool valid;             /* Flag indicating if the entry is valid */
    struct ssv_sta_ps ps;  /* Information when STA is in PS (AP only) */
    bool ht;               /* Flag indicating if the station
                               supports HT */
    bool vht;               /* Flag indicating if the station
                               supports VHT */
    //u32 ac_param[AC_MAX];  /* EDCA parameters */
    u32 ac_param[4];  /* EDCA parameters */
    struct reord_info_tag reord_info;
    uint16_t rx_last_seqcntl[TID_MAX];

    u32 last_rx;
    unsigned long probe_timestamp;
    bool port_control; /* Flag for IEEE 802.1X */
};

struct ssv_sec_phy_chan {
    u16 prim20_freq;
    u16 center_freq1;
    u16 center_freq2;
    enum nl80211_band band;
    u8 type;
};

/* Structure that will contains all RoC information received from cfg80211 */
struct ssv_roc_elem {
    struct wireless_dev *wdev;
    struct ieee80211_channel *chan;
    unsigned int duration;
    /* Used to avoid call of CFG80211 callback upon expiration of RoC */
    bool mgmt_roc;
    /* Indicate if we have switch on the RoC channel */
    bool on_chan;
};

/* Structure containing channel survey information received from MAC */
struct ssv_survey_info {
    // Filled
    u32 filled;
    // Amount of time in ms the radio spent on the channel
    u32 chan_time_ms;
    // Amount of time the primary channel was sensed busy
    u32 chan_time_busy_ms;
    // Noise in dbm
    s8 noise_dbm;
};

/**
 * enum ssv_ap_flags - AP flags
 *
 * @SSV_AP_ISOLATE Isolate clients (i.e. Don't brige packets transmitted by
 *                                   one client for another one)
 */
enum ssv_ap_flags
{
    SSV_AP_ISOLATE = BIT(0),
};

/* 
 * enum ssv_sta_flags - STATION flags
 * 
 * @SSV_STA_EXT_AUTH: External authentication is in progress
 */
enum rwnx_sta_flags {
    SSV_STA_EXT_AUTH = BIT(0),
};

struct ssv_agg_reord_pkt {
	struct list_head list;
	struct sk_buff *skb;
	u16 sn;
};

/* Structure containing channel context information */
struct ssv_chanctx {
    struct cfg80211_chan_def chan_def; /* channel description */
    u8 count;                          /* number of vif using this ctxt */
};


/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Functions
 ******************************************************************************/
static inline bool _ssv_is_multicast_sta(int sta_idx)
{
    return (sta_idx >= NX_REMOTE_STA_MAX);
}

static inline uint8_t _ssv_master_vif_idx(struct ssv_vif *vif)
{
    if (unlikely(vif->wdev.iftype == NL80211_IFTYPE_AP_VLAN)) {
        return vif->ap_vlan.master->drv_vif_index;
    } else {
        return vif->drv_vif_index;
    }
}

u8 *_ssv_build_bcn(struct ssv_bcn *bcn, struct cfg80211_beacon_data *new);
void ssv_external_auth_enable(struct ssv_vif *vif);
void ssv_external_auth_disable(struct ssv_vif *vif);


#endif /* __FMAC_DEFS_H__ */
