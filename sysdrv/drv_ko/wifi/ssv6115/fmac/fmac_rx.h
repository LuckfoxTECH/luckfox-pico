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


#ifndef __FMAC_RX_H__
#define __FMAC_RX_H__

#include <linux/mutex.h>
/*******************************************************************************
 *         Include Files
 ******************************************************************************/


/*******************************************************************************
 *         Defines
 ******************************************************************************/
/*
 * Decryption status subfields.
 * {
 */
#define SSV_RX_HD_DECR_UNENC            (0) // Frame unencrypted
#define SSV_RX_HD_DECR_ICVFAIL          (1) // WEP/TKIP ICV failure
#define SSV_RX_HD_DECR_CCMPFAIL         (2) // CCMP failure
#define SSV_RX_HD_DECR_AMSDUDISCARD     (3) // A-MSDU discarded at HW
#define SSV_RX_HD_DECR_NULLKEY          (4) // NULL key found
#define SSV_RX_HD_DECR_WEPSUCCESS       (5) // Security type WEP
#define SSV_RX_HD_DECR_TKIPSUCCESS      (6) // Security type TKIP
#define SSV_RX_HD_DECR_CCMPSUCCESS      (7) // Security type CCMP

#define SSV_FC_GET_TYPE(fc)	            (((fc) & 0x000c) >> 2)
#define SSV_FC_GET_STYPE(fc)	        (((fc) & 0x00f0) >> 4)

#define AGG_REORD_MSG_MAX_NUM           (4096)

/*******************************************************************************
 *         Enumerations
 ******************************************************************************/
enum rx_status_bits
{
    /// The buffer can be forwarded to the networking stack
    RX_STAT_FORWARD = 1 << 0,
    /// A new buffer has to be allocated
    RX_STAT_ALLOC = 1 << 1,
    /// The buffer has to be deleted
    RX_STAT_DELETE = 1 << 2,
    /// The length of the buffer has to be updated
    RX_STAT_LEN_UPDATE = 1 << 3,
    /// The length in the Ethernet header has to be updated
    RX_STAT_ETH_LEN_UPDATE = 1 << 4,
    /// Simple copy
    RX_STAT_COPY = 1 << 5,
};


/*******************************************************************************
 *         Structures
 ******************************************************************************/
struct hw_vect {
    /** Total length for the MPDU transfer */
    u32 len                   :16;

    u32 reserved              : 8;

    /** AMPDU Status Information */
    u32 mpdu_cnt              : 6;
    u32 ampdu_cnt             : 2;


    /** TSF Low */
    __le32 tsf_lo;
    /** TSF High */
    __le32 tsf_hi;

    /** Receive Vector 1a */
    u32    leg_length         :12;
    u32    leg_rate           : 4;
    u32    ht_length          :16;

    /** Receive Vector 1b */
    u32    _ht_length         : 4; // FIXME
    u32    short_gi           : 1;
    u32    stbc               : 2;
    u32    smoothing          : 1;
    u32    mcs                : 7;
    u32    pre_type           : 1;
    u32    format_mod         : 3;
    u32    ch_bw              : 2;
    u32    n_sts              : 3;
    u32    lsig_valid         : 1;
    u32    sounding           : 1;
    u32    num_extn_ss        : 2;
    u32    aggregation        : 1;
    u32    fec_coding         : 1;
    u32    dyn_bw             : 1;
    u32    doze_not_allowed   : 1;

    /** Receive Vector 1c */
    u32    antenna_set        : 8;
    u32    partial_aid        : 9;
    u32    group_id           : 6;
    u32    reserved_1c        : 1;
    s32    rssi1              : 8;

    /** Receive Vector 1d */
    s32    rssi2              : 8;
    s32    rssi3              : 8;
    s32    rssi4              : 8;
    u32    reserved_1d        : 8;

    /** Receive Vector 2a */
    u32    rcpi               : 8;
    u32    evm1               : 8;
    u32    evm2               : 8;
    u32    evm3               : 8;

    /** Receive Vector 2b */
    u32    evm4               : 8;
    u32    reserved2b_1       : 8;
    u32    reserved2b_2       : 8;
    u32    reserved2b_3       : 8;

    /** Status **/
    u32    rx_vect2_valid     : 1;
    u32    resp_frame         : 1;
    /** Decryption Status */
    u32    decr_status        : 3;
    u32    rx_fifo_oflow      : 1;

    /** Frame Unsuccessful */
    u32    undef_err          : 1;
    u32    phy_err            : 1;
    u32    fcs_err            : 1;
    u32    addr_mismatch      : 1;
    u32    ga_frame           : 1;
    u32    current_ac         : 2;

    u32    frm_successful_rx  : 1;
    /** Descriptor Done  */
    u32    desc_done_rx       : 1;
    /** Key Storage RAM Index */
    u32    key_sram_index     : 10;
    /** Key Storage RAM Index Valid */
    u32    key_sram_v         : 1;
    u32    type               : 2;
    u32    subtype            : 4;
};

struct hw_rxhdr {
    /** RX vector */
    struct hw_vect hwvect;

    /** PHY channel information 1 */
    u32    phy_band           : 8;
    u32    phy_channel_type   : 8;
    u32    phy_prim20_freq    : 16;
    /** PHY channel information 2 */
    u32    phy_center1_freq   : 16;
    u32    phy_center2_freq   : 16;
    /** RX flags */
    u32    flags_is_amsdu     : 1;
    u32    flags_is_80211_mpdu: 1;
    u32    flags_is_4addr     : 1;
    u32    flags_new_peer     : 1;
    u32    flags_user_prio    : 3;
    u32    flags_rsvd0        : 1;
    u32    flags_vif_idx      : 8;    // 0xFF if invalid VIF index
    u32    flags_sta_idx      : 8;    // 0xFF if invalid STA index
    u32    flags_dst_idx      : 8;    // 0xFF if unknown destination STA
    u16    sn;
	u16	   tid;
}__packed;

struct ssv_agg_reodr_msg {
    u16 sn;
    u8 sta_idx;
    u8 tid;
    u8 status;
	u8 num;
}__packed;


/// 802.11 Sequence Control definition
#define MAC_SEQCTRL_NUM_OFT             4
#define MAC_SEQCTRL_NUM_MSK             0xFFF0
#define MAC_SEQCTRL_NUM_MAX             (MAC_SEQCTRL_NUM_MSK >> MAC_SEQCTRL_NUM_OFT)

#define RX_CNTRL_REORD_WIN_SIZE 16
#define RX_HOST_REORD_PATTERN   0xFEFADECA

/// Structure containing the information about RX reordering if host handle reordering 
struct host_reorder_info
{
    uint32_t    upattern;
    uint8_t     frame_info;
    uint8_t     sta_idx;
    uint8_t     tid;
    uint8_t     baw_size;
    uint64_t    pn;
    uint16_t    win_start;
    uint16_t    sn;
    uint32_t    cookie;
};

/// Bit field positions for frame_info value (struct rxu_cntrl_rx_status)
enum rxu_cntrl_frame_info_pos
{
#define CO_BIT(pos) (1U<<(pos))
    /// Received frame was encrypted using TKIP method, so a MIC check is required
    RXU_CNTRL_MIC_CHECK_NEEDED = CO_BIT(0),
    /// Received frame has to pass the PN replay check
    RXU_CNTRL_PN_CHECK_NEEDED = CO_BIT(1),
    /// Received frame is a Mesh Beacon received from an unknown Mesh STA
    RXU_CNTRL_NEW_MESH_PEER   = CO_BIT(2),
    /// Received frame was group addressed
    RXU_CNTRL_GROUP_ADDRESSED = CO_BIT(3),
    /// Received BAR frame was delivered to host 
    RXU_CNTRL_BAR_TO_HOST = CO_BIT(6),
    /// Received no rx BA frame  
    RXU_CNTRL_NO_RX_BA_TO_HOST = CO_BIT(7),
};

/// Structure describing an element of the RX reordering table
struct rxu_cntrl_reord_elt
{
    /// Packet number of the received packet (used for replay check)
    uint64_t pn;
    /// Host Buffer Address
    struct sk_buff *skb;
    /// flag indicating if the PN must be verified
    bool pn_check;
};

struct rxu_cntrl_reord
{
    uint32_t sn_rx_time;
    /**
     * Lowest expected to be received (from 0 to 4096)
     *     = WinStartB IEEE Std 802.11-2012 section 9.21.7.6.1
     */
    uint64_t pn;
    uint16_t win_start;
    uint16_t first_win_start;
    uint16_t win_end;
    /// Current position in the RX packet status array
    uint8_t rx_status_pos;
    /// Number of packet received out of order and waiting for next sn
    uint8_t ooo_pkt_cnt;
    /// TID of the reordering structure
    uint8_t tid;
    /// Reordering buffer elements
    struct rxu_cntrl_reord_elt elt[RX_CNTRL_REORD_WIN_SIZE];
    /// Flag indicating if the reordering agreement is active or not
    bool active;

    bool upload;
};

/// STA Info Table
struct reord_info_tag
{
    /// For RX BA agreements per TID
    struct rxu_cntrl_reord ba_agmts_rx[TID_MAX];
    bool connect;
    struct mutex data_mutex;
    uint8_t lock_en;
    uint8_t level;
};

/*******************************************************************************
 *         Variables
 ******************************************************************************/
extern const u8 legrates_lut[];


/*******************************************************************************
 *         Functions
 ******************************************************************************/
int ssv_rx_packet_ind(void *app_param, struct sk_buff *skb);
void ssv_rxreord_timeout_cb(struct ssv_softc *sc);
void ssv_rxreord_create(struct ssv_softc *sc, u8 sta_idx);
void ssv_rxreord_delete(struct ssv_softc *sc, u8 sta_idx);
void ssv_rxreord_all_dump(struct ssv_softc *sc, uint16_t  sta_idx);
int ssv_push_private_msg_to_host(struct ssv_softc *sc, u32 msgid, u8 *data, u32 datalen);

#endif /* __FMAC_RX_H__ */

