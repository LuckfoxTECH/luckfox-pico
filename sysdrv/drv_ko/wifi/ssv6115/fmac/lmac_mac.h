/**
 ****************************************************************************************
 *
 * @file lmac_mac.h
 *
 * @brief MAC related definitions.
 *
 * Copyright (C) BouffaloLab 2017-2018
 *
 ****************************************************************************************
 */

#ifndef _MAC_H_
#define _MAC_H_

/**
 ****************************************************************************************
 * @defgroup MAC MAC
 * @ingroup COMMON
 * @brief  Common defines,structures
 *
 * This module contains defines commonaly used for MAC
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#ifndef __KERNEL__
// standard includes
#include <stdbool.h>
#else
#include <linux/types.h>
#endif


/*
 * DEFINES
 ****************************************************************************************
 */
/// duration of a Time Unit in microseconds
#define TU_DURATION                     1024

/// max number of channels in the 2.4 GHZ band
#define MAC_DOMAINCHANNEL_24G_MAX       14

/// max number of channels in the 5 GHZ band
#define MAC_DOMAINCHANNEL_5G_MAX        45

/// Mask to test if it's a basic rate - BIT(7)
#define MAC_BASIC_RATE                  0x80
/// Mask for extracting/checking word alignment
#define WORD_ALIGN                      3

#define MAX_AMSDU_LENGTH                7935

/// Traffic ID enumeration
enum
{
    TID_0,
    TID_1,
    TID_2,
    TID_3,
    TID_4,
    TID_5,
    TID_6,
    TID_7,
    TID_MGT,
    TID_NOQOS = TID_MGT,
    TID_MAX
};

/// Access Category enumeration
enum
{
    AC_BK = 0,
    AC_BE,
    AC_VI,
    AC_VO,
    AC_MAX
};

/// SCAN type
enum
{
    SCAN_PASSIVE,
    SCAN_ACTIVE
};

/// rates
enum
{
    MAC_RATE_1MBPS = 2,
    MAC_RATE_2MBPS = 4,
    MAC_RATE_5_5MBPS = 11,
    MAC_RATE_6MBPS = 12,
    MAC_RATE_9MBPS = 18,
    MAC_RATE_11MBPS = 22,
    MAC_RATE_12MBPS = 24,
    MAC_RATE_18MBPS = 36,
    MAC_RATE_24MBPS = 48,
    MAC_RATE_36MBPS = 72,
    MAC_RATE_48MBPS = 96,
    MAC_RATE_54MBPS = 108
};

/// Station flags
enum
{
    /// Bit indicating that a STA has QoS (WMM) capability
    STA_QOS_CAPA = 1 << 0,
    /// Bit indicating that a STA has HT capability
    STA_HT_CAPA = 1 << 1,
    /// Bit indicating that a STA has VHT capability
    STA_VHT_CAPA = 1 << 2,
    /// Bit indicating that a STA has MFP capability
    STA_MFP_CAPA = 1 << 3,
    /// Bit indicating that the STA included the Operation Notification IE
    STA_OPMOD_NOTIF = 1 << 4,
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
    /// Bit indicating that a STA has HE capability
    STA_HE_CAPA = 1 << 5,
//#endif
};

/// Connection flags
enum
{
    /// Flag indicating whether the control port is controlled by host or not
    CONTROL_PORT_HOST = 1 << 0,
    /// Flag indicating whether the control port frame shall be sent unencrypted
    CONTROL_PORT_NO_ENC = 1 << 1,
    /// Flag indicating whether HT shall be disabled or not
    DISABLE_HT = 1 << 2,
    /// Flag indicating whether WPA or WPA2 authentication is in use
    WPA_WPA2_IN_USE = 1 << 3,
    /// Flag indicating whether MFP is in use
    MFP_IN_USE = 1 << 4,
};

/// Authentication algorithm definition
#define MAC_AUTH_ALGO_OPEN      0
#define MAC_AUTH_ALGO_SHARED    1
#define MAC_AUTH_ALGO_FT        2
#define MAC_AUTH_ALGO_SAE       3

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * Compare two MAC addresses.
 * The MAC addresses MUST be 16 bit aligned.
 * @param[in] addr1_ptr Pointer to the first MAC address.
 * @param[in] addr2_ptr Pointer to the second MAC address.
 * @return True if equal, false if not.
 ****************************************************************************************
 */
#define MAC_ADDR_CMP(addr1_ptr, addr2_ptr)                                              \
    ((*(((u16*)(addr1_ptr)) + 0) == *(((u16*)(addr2_ptr)) + 0)) &&            \
     (*(((u16*)(addr1_ptr)) + 1) == *(((u16*)(addr2_ptr)) + 1)) &&            \
     (*(((u16*)(addr1_ptr)) + 2) == *(((u16*)(addr2_ptr)) + 2)))

/**
 ****************************************************************************************
 * Compare two MAC addresses whose alignment is not known.
 * @param[in] __a1 Pointer to the first MAC address.
 * @param[in] __a2 Pointer to the second MAC address.
 * @return True if equal, false if not.
 ****************************************************************************************
 */
#define MAC_ADDR_CMP_PACKED(__a1, __a2)                                                 \
    (memcmp(__a1, __a2, MAC_ADDR_LEN) == 0)

/**
 ****************************************************************************************
 * Copy a MAC address.
 * The MAC addresses MUST be 16 bit aligned.
 * @param[in] addr1_ptr Pointer to the destination MAC address.
 * @param[in] addr2_ptr Pointer to the source MAC address.
 ****************************************************************************************
 */
#define MAC_ADDR_CPY(addr1_ptr, addr2_ptr)                                              \
    *(((u16*)(addr1_ptr)) + 0) = *(((u16*)(addr2_ptr)) + 0);                  \
    *(((u16*)(addr1_ptr)) + 1) = *(((u16*)(addr2_ptr)) + 1);                  \
    *(((u16*)(addr1_ptr)) + 2) = *(((u16*)(addr2_ptr)) + 2)

/**
 ****************************************************************************************
 * Compare two SSID.
 * @param ssid1_ptr Pointer to the first SSID structure.
 * @param ssid2_ptr Pointer to the second SSID structure.
 * @return True if equal, false if not.
 ****************************************************************************************
 */
#define MAC_SSID_CMP(ssid1_ptr,ssid2_ptr)                                               \
    (((ssid1_ptr)->length == (ssid2_ptr)->length) &&                                    \
     (memcmp((&(ssid1_ptr)->array[0]), (&(ssid2_ptr)->array[0]), (ssid1_ptr)->length) == 0))

/// Check if MAC address is a group address: test the multicast bit.
#define MAC_ADDR_GROUP(mac_addr_ptr) ((*(mac_addr_ptr)) & 1)

/// MAC address length in bytes.
#define MAC_ADDR_LEN 6
/// MAC address structure.
struct mac_addr
{
    /// Array of bytes that make up the MAC address.
    u16_l array[MAC_ADDR_LEN/2];
} __packed;

/// SSID maximum length.
#define MAC_SSID_LEN 32

/// SSID.
struct mac_ssid
{
    /// Actual length of the SSID.
    u8_l length;
    /// Array containing the SSID name.
    u8_l array[MAC_SSID_LEN];
} __packed;

/// MAC RATE-SET
#define MAC_RATESET_LEN             12
#define MAC_OFDM_PHY_RATESET_LEN    8
#define MAC_EXT_RATES_OFF      8
struct mac_rateset
{
    u8_l     length;
    u8_l     array[MAC_RATESET_LEN];
} __packed;

/// MAC RATES
#define MAC_MCS_WORD_CNT            3
struct mac_rates
{
    /// MCS 0 to 76
    u32 mcs[MAC_MCS_WORD_CNT];
    /// Legacy rates (1Mbps to 54Mbps)
    u16 legacy;
};

/// Structure of a Data/Management frame MAC header
struct mac_hdr
{
    /// Frame control
    uint16_t fctl;
    /// Duration/ID
    uint16_t durid;
    /// Address 1
    struct mac_addr addr1;
    /// Address 2
    struct mac_addr addr2;
    /// Address 3
    struct mac_addr addr3;
    /// Sequence control
    uint16_t seq;
} __packed;

/// IV/EIV data
#define MAC_IV_LEN  4
#define MAC_EIV_LEN 4
struct rx_seciv
{
    u8 iv[MAC_IV_LEN];
    u8 ext_iv[MAC_EIV_LEN];
};

/// MAC MCS SET
#define MAX_MCS_LEN 16 // 16 * 8 = 128
struct mac_mcsset
{
    u8 length;
    u8 array[MAX_MCS_LEN];
};

/// MAC Secret Key
#define MAC_WEP_KEY_CNT          4  // Number of WEP keys per virtual device
#define MAC_WEP_KEY_LEN         13  // Max size of a WEP key (104/8 = 13)
struct mac_wep_key
{
    u8 array[MAC_WEP_KEY_LEN]; // Key material
};


/// MAC Secret Key
#define MAC_SEC_KEY_LEN         32  // TKIP keys 256 bits (max length) with MIC keys
struct mac_sec_key
{
    u8_l length;                         // Key material length
    u32_l array[MAC_SEC_KEY_LEN/4];        // Key material
}__packed;

/// MAC channel list
/// @todo: fix that number
#define MAC_MAX_CH 40
struct mac_ch_list
{
    /// Number of channels in channel list.
    u16 nbr;
    /// List of the channels.
    u8 list[MAC_MAX_CH];
};


struct mac_country_subband
{
    // First channel number of the triplet.
    u8 first_chn;
    // Max number of channel number for the triplet.
    u8 nbr_of_chn;
    // Maximum allowed transmit power.
    u8 max_tx_power;
};

#define MAX_COUNTRY_LEN         3
#define MAX_COUNTRY_SUBBAND     5
struct mac_country
{
    // Length of the country string
    u8 length;
    // Country  string 2 char.
    u8 string[MAX_COUNTRY_LEN];
    // channel info triplet
    struct mac_country_subband subband[MAX_COUNTRY_SUBBAND];
};

/// MAC HT CAPABILITY
struct mac_htcapability
{
    /// HT capability information
    uint16_t ht_capa_info;
    /// A-MPDU parameters
    uint8_t a_mpdu_param;
    /// Supported MCS
    uint8_t mcs_rate[MAX_MCS_LEN];
    /// HT extended capability information
    uint16_t ht_extended_capa;
    /// Beamforming capability information
    uint32_t tx_beamforming_capa;
    /// Antenna selection capability information
    uint8_t asel_capa;
} __packed;

/// MAC VHT CAPABILITY
struct mac_vhtcapability
{
    /// VHT capability information
    uint32_t vht_capa_info;
    /// RX MCS map
    uint16_t rx_mcs_map;
    /// RX highest data rate
    uint16_t rx_highest;
    /// TX MCS map
    uint16_t tx_mcs_map;
    /// TX highest data rate
    uint16_t tx_highest;
} __packed;

/// Length (in bytes) of the MAC HE capability field
#define MAC_HE_MAC_CAPA_LEN 6
/// Length (in bytes) of the PHY HE capability field
#define MAC_HE_PHY_CAPA_LEN 11
/// Maximum length (in bytes) of the PPE threshold data
#define MAC_HE_PPE_THRES_MAX_LEN 25

/// Structure listing the per-NSS, per-BW supported MCS combinations
struct mac_he_mcs_nss_supp
{
    /// per-NSS supported MCS in RX, for BW <= 80MHz
    uint16_t rx_mcs_80;
    /// per-NSS supported MCS in TX, for BW <= 80MHz
    uint16_t tx_mcs_80;
    /// per-NSS supported MCS in RX, for BW = 160MHz
    uint16_t rx_mcs_160;
    /// per-NSS supported MCS in TX, for BW = 160MHz
    uint16_t tx_mcs_160;
    /// per-NSS supported MCS in RX, for BW = 80+80MHz
    uint16_t rx_mcs_80p80;
    /// per-NSS supported MCS in TX, for BW = 80+80MHz
    uint16_t tx_mcs_80p80;
} __packed;

/// MAC HE capability information element
struct mac_hecapability
{
    /// MAC HE capabilities
    uint8_t mac_cap_info[MAC_HE_MAC_CAPA_LEN];
    /// PHY HE capabilities
    uint8_t phy_cap_info[MAC_HE_PHY_CAPA_LEN];
    /// Supported MCS combinations
    struct mac_he_mcs_nss_supp mcs_supp;
    /// PPE Thresholds data
    uint8_t ppe_thres[MAC_HE_PPE_THRES_MAX_LEN];
} __packed;

/**
 * @name HE MAC Capability Information Element definitions
 * @{
 ****************************************************************************************
 */
/// 802.11 HE MAC capability definition
#define HE_MAC_CAPA_HTC_HE_POS                             0
#define HE_MAC_CAPA_TWT_REQ_POS                            1
#define HE_MAC_CAPA_TWT_RES_POS                            2

#define HE_MAC_CAPA_DYNAMIC_FRAG_OFT                       3
#define HE_MAC_CAPA_DYNAMIC_FRAG_WIDTH                     2
#define HE_MAC_CAPA_DYNAMIC_FRAG_NOT_SUPP                         0x00
#define HE_MAC_CAPA_DYNAMIC_FRAG_LEVEL_1                          0x01
#define HE_MAC_CAPA_DYNAMIC_FRAG_LEVEL_2                          0x02
#define HE_MAC_CAPA_DYNAMIC_FRAG_LEVEL_3                          0x03

#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_OFT                  5
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_WIDTH                3
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_1                           0x00
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_2                           0x01
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_4                           0x02
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_8                           0x03
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_16                          0x04
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_32                          0x05
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_64                          0x06
#define HE_MAC_CAPA_MAX_NUM_FRAG_MSDU_UNLIMITED                   0x07

#define HE_MAC_CAPA_MIN_FRAG_SIZE_OFT                      8
#define HE_MAC_CAPA_MIN_FRAG_SIZE_WIDTH                    2
#define HE_MAC_CAPA_MIN_FRAG_SIZE_UNLIMITED                       0x00
#define HE_MAC_CAPA_MIN_FRAG_SIZE_128                             0x01
#define HE_MAC_CAPA_MIN_FRAG_SIZE_256                             0x02
#define HE_MAC_CAPA_MIN_FRAG_SIZE_512                             0x03

#define HE_MAC_CAPA_TF_MAC_PAD_DUR_OFT                     10
#define HE_MAC_CAPA_TF_MAC_PAD_DUR_WIDTH                   2
#define HE_MAC_CAPA_TF_MAC_PAD_DUR_0US                            0x00
#define HE_MAC_CAPA_TF_MAC_PAD_DUR_8US                            0x01
#define HE_MAC_CAPA_TF_MAC_PAD_DUR_16US                           0x02

#define HE_MAC_CAPA_MULTI_TID_AGG_RX_OFT                   12
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_WIDTH                 3
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_1                            0x00
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_2                            0x01
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_3                            0x02
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_4                            0x03
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_5                            0x04
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_6                            0x05
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_7                            0x06
#define HE_MAC_CAPA_MULTI_TID_AGG_RX_8                            0x07

#define HE_MAC_CAPA_LINK_ADAPTATION_OFT                    15
#define HE_MAC_CAPA_LINK_ADAPTATION_WIDTH                  2
#define HE_MAC_CAPA_LINK_ADAPTATION_NONE                          0x00
#define HE_MAC_CAPA_LINK_ADAPTATION_RSVD                          0x01
#define HE_MAC_CAPA_LINK_ADAPTATION_UNSOL                         0x02
#define HE_MAC_CAPA_LINK_ADAPTATION_BOTH                          0x03

#define HE_MAC_CAPA_ALL_ACK_POS                            17
#define HE_MAC_CAPA_TSR_POS                                18
#define HE_MAC_CAPA_BSR_POS                                19
#define HE_MAC_CAPA_BCAST_TWT_POS                          20
#define HE_MAC_CAPA_32BIT_BA_BITMAP_POS                    21
#define HE_MAC_CAPA_MU_CASCADING_POS                       22
#define HE_MAC_CAPA_ACK_EN_POS                             23
#define HE_MAC_CAPA_OM_CONTROL_POS                         25
#define HE_MAC_CAPA_OFDMA_RA_POS                           26

#define HE_MAC_CAPA_MAX_A_AMPDU_LEN_EXP_OFT                27
#define HE_MAC_CAPA_MAX_A_AMPDU_LEN_EXP_WIDTH              2
#define HE_MAC_CAPA_MAX_A_AMPDU_LEN_EXP_USE_VHT                   0x00
#define HE_MAC_CAPA_MAX_A_AMPDU_LEN_EXP_VHT_1                     0x01
#define HE_MAC_CAPA_MAX_A_AMPDU_LEN_EXP_VHT_2                     0x02
#define HE_MAC_CAPA_MAX_A_AMPDU_LEN_EXP_RSVD                      0x03

#define HE_MAC_CAPA_A_AMSDU_FRAG_POS                       29
#define HE_MAC_CAPA_FLEX_TWT_SCHED_POS                     30
#define HE_MAC_CAPA_RX_CTRL_FRAME_TO_MULTIBSS_POS          31
#define HE_MAC_CAPA_BSRP_BQRP_A_MPDU_AGG_POS               32
#define HE_MAC_CAPA_QTP_POS                                33
#define HE_MAC_CAPA_BQR_POS                                34
#define HE_MAC_CAPA_SRP_RESP_POS                           35
#define HE_MAC_CAPA_NDP_FB_REP_POS                         36
#define HE_MAC_CAPA_OPS_POS                                37
#define HE_MAC_CAPA_AMDSU_IN_AMPDU_POS                     38

#define HE_MAC_CAPA_MULTI_TID_AGG_TX_OFT                   39
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_WIDTH                 3
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_1                            0x00
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_2                            0x01
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_3                            0x02
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_4                            0x03
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_5                            0x04
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_6                            0x05
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_7                            0x06
#define HE_MAC_CAPA_MULTI_TID_AGG_TX_8                            0x07

#define HE_MAC_CAPA_SUB_CHNL_SEL_TX_POS                    42
#define HE_MAC_CAPA_UL_2_996_TONE_RU_POS                   43
#define HE_MAC_CAPA_OM_CONTROL_UL_MU_DIS_RX_POS            44
#define HE_MAC_CAPA_DYN_SMPS_POS                           45

/** @} */

/**
 * @name HE PHY Capability Information Element definitions
 * @{
 ****************************************************************************************
 */
/// 802.11 HE PHY capability definition
#define HE_PHY_CAPA_CHAN_WIDTH_SET_OFT                     1
#define HE_PHY_CAPA_CHAN_WIDTH_SET_WIDTH                   7
#define HE_PHY_CAPA_CHAN_WIDTH_SET_40MHZ_IN_2G                    0x01
#define HE_PHY_CAPA_CHAN_WIDTH_SET_40MHZ_80MHZ_IN_5G              0x02
#define HE_PHY_CAPA_CHAN_WIDTH_SET_160MHZ_IN_5G                   0x04
#define HE_PHY_CAPA_CHAN_WIDTH_SET_80PLUS80_MHZ_IN_5G             0x08
#define HE_PHY_CAPA_CHAN_WIDTH_SET_RU_MAPPING_IN_2G               0x10
#define HE_PHY_CAPA_CHAN_WIDTH_SET_RU_MAPPING_IN_5G               0x20
#define HE_PHY_CAPA_CHAN_WIDTH_SET_RU_MAPPING_40MHZ_IN_2G          HE_PHY_CAPA_CHAN_WIDTH_SET_40MHZ_IN_2G | HE_PHY_CAPA_CHAN_WIDTH_SET_RU_MAPPING_IN_2G


#define HE_PHY_CAPA_PREAMBLE_PUNC_RX_OFT                   8
#define HE_PHY_CAPA_PREAMBLE_PUNC_RX_WIDTH                 4
#define HE_PHY_CAPA_PREAMBLE_PUNC_RX_80M_ONLY_SCND_20M            0x01
#define HE_PHY_CAPA_PREAMBLE_PUNC_RX_80M_ONLY_SCND_40M            0x02
#define HE_PHY_CAPA_PREAMBLE_PUNC_RX_160M_ONLY_SCND_20M           0x04
#define HE_PHY_CAPA_PREAMBLE_PUNC_RX_160M_ONLY_SCND_40M           0x08

#define HE_PHY_CAPA_DEVICE_CLASS_A_POS                     12
#define HE_PHY_CAPA_LDPC_CODING_IN_PAYLOAD_POS             13
#define HE_PHY_CAPA_HE_SU_PPDU_1x_LTF_AND_GI_0_8US_POS     14

#define HE_PHY_CAPA_MIDAMBLE_RX_MAX_NSTS_OFT               15
#define HE_PHY_CAPA_MIDAMBLE_RX_MAX_NSTS_WIDTH             2
#define HE_PHY_CAPA_MIDAMBLE_RX_MAX_NSTS_1_STS                    0x00
#define HE_PHY_CAPA_MIDAMBLE_RX_MAX_NSTS_2_STS                    0x01
#define HE_PHY_CAPA_MIDAMBLE_RX_MAX_NSTS_3_STS                    0x02
#define HE_PHY_CAPA_MIDAMBLE_RX_MAX_NSTS_4_STS                    0x03

#define HE_PHY_CAPA_NDP_4x_LTF_AND_3_2US_POS               17
#define HE_PHY_CAPA_STBC_TX_UNDER_80MHZ_POS                18
#define HE_PHY_CAPA_STBC_RX_UNDER_80MHZ_POS                19
#define HE_PHY_CAPA_DOPPLER_TX_POS                         20
#define HE_PHY_CAPA_DOPPLER_RX_POS                         21
#define HE_PHY_CAPA_FULL_BW_UL_MU_MIMO_POS                 22
#define HE_PHY_CAPA_PARTIAL_BW_UL_MU_MIMO_POS              23

#define HE_PHY_CAPA_DCM_MAX_CONST_TX_OFT                   24
#define HE_PHY_CAPA_DCM_MAX_CONST_TX_WIDTH                 2
#define HE_PHY_CAPA_DCM_MAX_CONST_TX_NO_DCM                       0x00
#define HE_PHY_CAPA_DCM_MAX_CONST_TX_BPSK                         0x01
#define HE_PHY_CAPA_DCM_MAX_CONST_TX_QPSK                         0x02
#define HE_PHY_CAPA_DCM_MAX_CONST_TX_16_QAM                       0x03

#define HE_PHY_CAPA_DCM_MAX_NSS_TX_POS                     26

#define HE_PHY_CAPA_DCM_MAX_CONST_RX_OFT                   27
#define HE_PHY_CAPA_DCM_MAX_CONST_RX_WIDTH                 2
#define HE_PHY_CAPA_DCM_MAX_CONST_RX_NO_DCM                       0x00
#define HE_PHY_CAPA_DCM_MAX_CONST_RX_BPSK                         0x01
#define HE_PHY_CAPA_DCM_MAX_CONST_RX_QPSK                         0x02
#define HE_PHY_CAPA_DCM_MAX_CONST_RX_16_QAM                       0x03

#define HE_PHY_CAPA_DCM_MAX_NSS_RX_POS                     29
#define HE_PHY_CAPA_RX_HE_MU_PPDU_FRM_NON_AP_POS           30
#define HE_PHY_CAPA_SU_BEAMFORMER_POS                      31
#define HE_PHY_CAPA_SU_BEAMFORMEE_POS                      32
#define HE_PHY_CAPA_MU_BEAMFORMER_POS                      33

#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_OFT          34
#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_WIDTH         3
#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_4                   0x03
#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_5                   0x04
#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_6                   0x05
#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_7                   0x06
#define HE_PHY_CAPA_BFMEE_MAX_STS_UNDER_80MHZ_8                   0x07

#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_OFT          37
#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_WIDTH         3
#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_4                   0x03
#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_5                   0x04
#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_6                   0x05
#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_7                   0x06
#define HE_PHY_CAPA_BFMEE_MAX_STS_ABOVE_80MHZ_8                   0x07

#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_OFT      40
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_WIDTH     3
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_1               0x00
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_2               0x01
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_3               0x02
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_4               0x03
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_5               0x04
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_6               0x05
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_7               0x06
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_UNDER_80MHZ_8               0x07

#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_OFT      43
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_WIDTH     3
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_1               0x00
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_2               0x01
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_3               0x02
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_4               0x03
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_5               0x04
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_6               0x05
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_7               0x06
#define HE_PHY_CAPA_BFMEE_NUM_SND_DIM_ABOVE_80MHZ_8               0x07

#define HE_PHY_CAPA_NG16_SU_FEEDBACK_POS                   46
#define HE_PHY_CAPA_NG16_MU_FEEDBACK_POS                   47
#define HE_PHY_CAPA_CODEBOOK_SIZE_42_SU_POS                48
#define HE_PHY_CAPA_CODEBOOK_SIZE_75_MU_POS                49
#define HE_PHY_CAPA_TRIG_SU_BEAMFORMER_FB_POS              50
#define HE_PHY_CAPA_TRIG_MU_BEAMFORMER_FB_POS              51
#define HE_PHY_CAPA_TRIG_CQI_FB_POS                        52
#define HE_PHY_CAPA_PARTIAL_BW_EXT_RANGE_POS               53
#define HE_PHY_CAPA_PARTIAL_BW_DL_MUMIMO_POS               54
#define HE_PHY_CAPA_PPE_THRESHOLD_PRESENT_POS              55
#define HE_PHY_CAPA_SRP_BASED_SR_POS                       56
#define HE_PHY_CAPA_POWER_BOOST_FACTOR_AR_POS              57
#define HE_PHY_CAPA_HE_SU_MU_PPDU_4x_LTF_AND_08_US_GI_POS  58

#define HE_PHY_CAPA_MAX_NC_OFT                             59
#define HE_PHY_CAPA_MAX_NC_WIDTH                           3
#define HE_PHY_CAPA_MAX_NC_1                                      0x01
#define HE_PHY_CAPA_MAX_NC_2                                      0x02
#define HE_PHY_CAPA_MAX_NC_3                                      0x03
#define HE_PHY_CAPA_MAX_NC_4                                      0x04
#define HE_PHY_CAPA_MAX_NC_5                                      0x05
#define HE_PHY_CAPA_MAX_NC_6                                      0x06
#define HE_PHY_CAPA_MAX_NC_7                                      0x07

#define HE_PHY_CAPA_STBC_TX_ABOVE_80MHZ_POS                62
#define HE_PHY_CAPA_STBC_RX_ABOVE_80MHZ_POS                63
#define HE_PHY_CAPA_HE_ER_SU_PPDU_4x_LTF_AND_08_US_GI_POS  64
#define HE_PHY_CAPA_20MHZ_IN_40MHZ_HE_PPDU_IN_2G_POS       65
#define HE_PHY_CAPA_20MHZ_IN_160MHZ_HE_PPDU_POS            66
#define HE_PHY_CAPA_80MHZ_IN_160MHZ_HE_PPDU_POS            67
#define HE_PHY_CAPA_HE_ER_SU_1x_LTF_AND_08_US_GI_POS       68
#define HE_PHY_CAPA_MIDAMBLE_RX_2x_AND_1x_LTF_POS          69

#define HE_PHY_CAPA_DCM_MAX_BW_OFT                         70
#define HE_PHY_CAPA_DCM_MAX_BW_WIDTH                        2
#define HE_PHY_CAPA_DCM_MAX_BW_20MHZ                              0x00
#define HE_PHY_CAPA_DCM_MAX_BW_40MHZ                              0x01
#define HE_PHY_CAPA_DCM_MAX_BW_80MHZ                              0x02
#define HE_PHY_CAPA_DCM_MAX_BW_160MHZ_80P80MHZ                    0x03

#define HE_PHY_CAPA_16PLUS_HE_SIGB_OFDM_SYM_POS            72
#define HE_PHY_CAPA_NON_TRIG_CQI_FEEDBACK_POS              73
#define HE_PHY_CAPA_TX_1024QAM_LESS_242_RU_POS             74
#define HE_PHY_CAPA_RX_1024QAM_LESS_242_RU_POS             75
#define HE_PHY_CAPA_RX_FULL_BW_SU_COMP_SIGB_POS            76
#define HE_PHY_CAPA_RX_FULL_BW_SU_NON_COMP_SIGB_POS        77

#define HE_PHY_CAPA_NOMINAL_PACKET_PADDING_OFT             78
#define HE_PHY_CAPA_NOMINAL_PACKET_PADDING_WIDTH           2
#define HE_PHY_CAPA_NOMINAL_PACKET_PADDING_0US                    0x00
#define HE_PHY_CAPA_NOMINAL_PACKET_PADDING_8US                    0x01
#define HE_PHY_CAPA_NOMINAL_PACKET_PADDING_16US                   0x02
#define HE_PHY_CAPA_NOMINAL_PACKET_PADDING_RESERVED         HE_PHY_CAPA_NOMINAL_PACKET_PADDING_8US | HE_PHY_CAPA_NOMINAL_PACKET_PADDING_16US

/** @} */

/**
 * @name HE MAC Packet And Padding Extension definitions
 * @{
 ****************************************************************************************
 */
#define HE_PPE_CAPA_NSTS_OFT            0
#define HE_PPE_CAPA_NSTS_WIDTH          3

#define HE_PPE_CAPA_RU_INDEX_BITMAP_OFT   3
#define HE_PPE_CAPA_RU_INDEX_BITMAP_WIDTH 4

#define HE_PPE_CAPA_PPE_THRES_INFO_OFT   7

#define HE_PPE_CAPA_RU_242   CO_BIT(0)
#define HE_PPE_CAPA_RU_484   CO_BIT(1)
#define HE_PPE_CAPA_RU_996   CO_BIT(2)
#define HE_PPE_CAPA_RU_2x996 CO_BIT(3)

#define HE_PPE_CAPA_BPSK    0
#define HE_PPE_CAPA_QPSK    1
#define HE_PPE_CAPA_16QAM   2
#define HE_PPE_CAPA_64QAM   3
#define HE_PPE_CAPA_256QAM  4
#define HE_PPE_CAPA_1024QAM 5
#define HE_PPE_CAPA_NONE    7

/** @} */

/**
 * @name HE MCS MAP definitions
 * @{
 ****************************************************************************************
 */
/// 802.11 HE MCS map definition
#define MAC_HE_MCS_MAP_MSK                                 0x03
#define MAC_HE_MCS_MAP_0_7                                 0x00
#define MAC_HE_MCS_MAP_0_9                                 0x01
#define MAC_HE_MCS_MAP_0_11                                0x02
#define MAC_HE_MCS_MAP_NONE                                0x03
/** @} */

/**
 ****************************************************************************************
 * Set the value of a bit field in the HE MAC capability element.
 * @param[in] he_cap Pointer to the HE capability structure
 * @param[in] field Bit field to be written
 * @param[in] val The value of the field
 ****************************************************************************************
 */
#define HE_MAC_CAPA_VAL_SET(he_cap, field, val) _ssv_co_val_set((he_cap)->mac_cap_info,       \
                                                           HE_MAC_CAPA_##field##_OFT,    \
                                                           HE_MAC_CAPA_##field##_WIDTH,  \
                                                           HE_MAC_CAPA_##field##_##val)

/**
 ****************************************************************************************
 * Set a bit in the HE MAC capability element.
 * @param[in] he_cap Pointer to the HE capability structure
 * @param[in] bit    Bit to be set
 ****************************************************************************************
 */
#define HE_MAC_CAPA_BIT_SET(he_cap, bit)      _ssv_co_bit_set((he_cap)->mac_cap_info,         \
                                                          HE_MAC_CAPA_##bit##_POS)

/**
 ****************************************************************************************
 * Set the value of a bit field in the HE PHY capability element.
 * @param[in] he_cap Pointer to the HE capability structure
 * @param[in] field Bit field to be written
 * @param[in] val The value of the field
 ****************************************************************************************
 */
#define HE_PHY_CAPA_VAL_SET(he_cap, field, val) _ssv_co_val_set((he_cap)->phy_cap_info,       \
                                                           HE_PHY_CAPA_##field##_OFT,    \
                                                           HE_PHY_CAPA_##field##_WIDTH,  \
                                                           HE_PHY_CAPA_##field##_##val)

/**
 ****************************************************************************************
 * Set a bit in the HE PHY capability element.
 * @param[in] he_cap Pointer to the HE capability structure
 * @param[in] bit    Bit to be set
 ****************************************************************************************
 */
#define HE_PHY_CAPA_BIT_SET(he_cap, bit)      _ssv_co_bit_set((he_cap)->phy_cap_info,         \
                                                         HE_PHY_CAPA_##bit##_POS)


/// MAC HT operation element
struct mac_htoprnelmt
{
    /// Primary channel information
    uint8_t prim_channel;
    /// HT operation information 1
    uint8_t ht_oper_1;
    /// HT operation information 2
    uint16_t ht_oper_2;
    /// HT operation information 3
    uint16_t ht_oper_3;
    /// Basic MCS set
    uint8_t mcs_rate[MAX_MCS_LEN];

} __packed;

/// MAC QOS CAPABILITY
struct mac_qoscapability
{
    u8  qos_info;
};

/// RSN information element
#define MAC_RAW_RSN_IE_LEN 34
struct mac_raw_rsn_ie
{
    u8 data[2 + MAC_RAW_RSN_IE_LEN];
};

#define MAC_RAW_ENC_LEN 0x1A
struct mac_wpa_frame
{
    u8 array[MAC_RAW_ENC_LEN];
};

#define MAC_WME_PARAM_LEN          16
struct mac_wmm_frame
{
    u8 array [MAC_WME_PARAM_LEN];
};

/// BSS load element
struct mac_bss_load
{
    u16 sta_cnt;
    u8  ch_utilization;
    u16 avail_adm_capacity;
};

/// EDCA Parameter Set Element
struct mac_edca_param_set
{
    /// Per-AC EDCA parameters
    uint32_t ac_param[AC_MAX];
    /// QoS information
    uint8_t qos_info;
    /// Admission Control Mandatory bitfield
    uint8_t acm;
    /// Parameter set counter
    uint8_t param_set_cnt;
} __packed;

///MAC Twenty Forty BSS

struct mac_twenty_fourty_bss
{
    u8 bss_coexistence;
};

/// MAC BA PARAMETERS
struct mac_ba_param
{
    struct mac_addr   peer_sta_address;     ///< Peer STA MAC Address to which BA is Setup
    u16          buffer_size;          ///< Number of buffers available for this BA
    u16          start_sequence_number;///< Start Sequence Number of BA
    u16          ba_timeout;           ///< BA Setup timeout value
    u8           dev_type;             ///< BA Device Type Originator/Responder
    u8           block_ack_policy;     ///< BLOCK-ACK Policy Setup Immedaite/Delayed
    u8           buffer_cnt;           ///< Number of buffers required for BA Setup
};

/// MAC TS INFO field
struct mac_ts_info
{
    u8   traffic_type;
    u8   ack_policy;
    u8   access_policy;
    u8   dir;
    u8   tsid;
    u8   user_priority;
    bool      aggregation;
    bool      apsd;
    bool      schedule;
};

/// MAC TSPEC PARAMETERS
struct mac_tspec_param
{
    struct mac_ts_info ts_info;
    u16  nominal_msdu_size;
    u16  max_msdu_size;
    u32  min_service_interval;
    u32  max_service_interval;
    u32  inactivity_interval;
    u32  short_inactivity_interval;
    u32  service_start_time;
    u32  max_burst_size;
    u32  min_data_rate;
    u32  mean_data_rate;
    u32  min_phy_rate;
    u32  peak_data_rate;
    u32  delay_bound;
    u16  medium_time;
    u8   surplusbwallowance;
};

/// Primary Channel definition
struct mac_chan_def
{
    /// Frequency of the channel (in MHz)
    uint16_t freq;
    /// RF band (@ref mac_chan_band)
    uint8_t band;
    /// Additional information (@ref mac_chan_flags)
    uint8_t flags;
    /// Max transmit power allowed on this channel (dBm)
    int8_t tx_power;
} __packed;

/// Operating Channel
struct mac_chan_op
{
    /// Band (@ref mac_chan_band)
    uint8_t band;
    /// Channel type (@ref mac_chan_bandwidth)
    uint8_t type;
    /// Frequency for Primary 20MHz channel (in MHz)
    uint16_t prim20_freq;
    /// Frequency center of the contiguous channel or center of Primary 80+80 (in MHz)
    uint16_t center1_freq;
    /// Frequency center of the non-contiguous secondary 80+80 (in MHz)
    uint16_t center2_freq;
    /// Max transmit power allowed on this channel (dBm)
    int8_t tx_power;
    /// Additional information (@ref mac_chan_flags)
    uint8_t flags;
} __packed;

/// Scan result element, parsed from beacon or probe response frames.
struct mac_scan_result
{
    /// Scan result is valid
    bool valid_flag;
    /// Network BSSID.
    struct mac_addr bssid;
    /// Network name.
    struct mac_ssid ssid;
    /// Network type (@ref mac_bss_type).
    uint16_t bsstype;
    /// Network channel.
    struct mac_chan_def *chan;
    /// Supported AKM (bit-field of @ref mac_akm_suite)
    uint32_t akm;
    /// Group cipher (bit-field of @ref mac_cipher_suite)
    uint16_t group_cipher;
    /// Group cipher (bit-field of @ref mac_cipher_suite)
    uint16_t pairwise_cipher;
    /// RSSI of the scanned BSS (in dBm)
    int8_t rssi;
    /// Multi-BSSID index (0 if this is the reference (i.e. transmitted) BSSID)
    uint8_t multi_bssid_index;
    /// Maximum BSSID indicator
    uint8_t max_bssid_indicator;
} __packed;

/// Structure containing the information required to perform a measurement request
struct mac_request_set
{

    u8             mode;       ///<As specified by standard
    u8             type;       ///< 0: Basic request, 1: CCA request, 2: RPI histogram request
    u16            duration;   ///< In TU
    uint64_t            start_time; ///< TSF time
    u8             ch_number;  ///< channel to be measured
};

/// Structure containing the information returned from a measurement process
struct mac_report_set
{
    u8             mode;       ///<As specified by standard
    u8             type;       ///< 0: Basic request, 1: CCA request, 2: RPI histogram request
    u16            duration;   ///< In TU
    uint64_t            start_time; ///< TSF time
    u8             ch_number;  ///< channel to be measured
    u8             map;        ///< As specified by standard
    u8             cca_busy_fraction;  ///<As specified by standard
    u8             rpi_histogram[8];   ///<As specified by standard
};

/// Structure containing the MAC SW and MAC HW version information
struct mac_version
{
    char mac_sw_version[16];
    char mac_sw_version_date[48];
    char mac_sw_build_date[48];
    u32 mac_hw_version1;
    u32 mac_hw_version2;
};

/// Structure containing some of the properties of a BSS. @todo Add required fields during
/// AP/IBSS mode implementation
struct mac_bss_conf
{
    /// Flags (ERP, QoS, etc.).
    u32 flags;
    /// Beacon period
    u16 beacon_period;
};

#define RX_BUF_HEADROOM_SIZE            (24)

/// Maximum number MSDUs supported in one received A-MSDU
#define NX_MAX_MSDU_PER_RX_AMSDU        (8)

#define RX_INFO_LEN                     sizeof(struct rx_info)

/// Packet contains an A-MSDU
#define RX_FLAGS_IS_AMSDU_BIT         CO_BIT(0)
/// Packet contains a 802.11 MPDU
#define RX_FLAGS_IS_MPDU_BIT          CO_BIT(1)
/// Packet contains 4 addresses
#define RX_FLAGS_4_ADDR_BIT           CO_BIT(2)
/// Packet is a Mesh Beacon received from an unknown Mesh STA
#define RX_FLAGS_NEW_MESH_PEER_BIT    CO_BIT(3)
/// Bitmask indicating that a received packet is not a MSDU
#define RX_FLAGS_NON_MSDU_MSK        (RX_FLAGS_IS_MPDU_BIT)
/// Offset of the User priority index field
#define RX_FLAGS_USER_PRIO_INDEX_OFT 4
/// Mask of the User priority index field
#define RX_FLAGS_USER_PRIO_INDEX_MSK (0x7 << RX_FLAGS_USER_PRIO_INDEX_OFT)
/// Offset of the VIF index field
#define RX_FLAGS_VIF_INDEX_OFT  8
/// Mask of the VIF index field
#define RX_FLAGS_VIF_INDEX_MSK  (0xFF << RX_FLAGS_VIF_INDEX_OFT)
/// Offset of the STA index field
#define RX_FLAGS_STA_INDEX_OFT  16
/// Mask of the STA index field
#define RX_FLAGS_STA_INDEX_MSK  (0xFF << RX_FLAGS_STA_INDEX_OFT)
/// Offset of the destination STA index field
#define RX_FLAGS_DST_INDEX_OFT  24
/// Mask of the destination STA index field
#define RX_FLAGS_DST_INDEX_MSK  (0xFF << RX_FLAGS_DST_INDEX_OFT)

/// @name PHY INFO related definitions.
/// @{

/// Macro retrieving the band of the phy channel info
/// @param[in] __x phy channel info 1 value.
#define PHY_INFO_BAND(__x) ((__x.info1) & 0x000000FF)

/// Macro retrieving the bandwidth of the phy channel info
/// @param[in] __x phy channel info 1 value.
#define PHY_INFO_BW(__x) (((__x.info1) & 0x0000FF00) >> 8)

/// Macro retrieving the channel of the phy channel info
/// @param[in] __x phy channel info 1 value.
#define PHY_INFO_CHAN(__x) (((__x.info1) & 0xFFFF0000) >> 16)

/// Macro retrieving the center frequency 1 of the phy channel info
/// @param[in] __x phy channel info 2 value.
#define PHY_INFO_CENT_FREQ1(__x) ((__x.info2) & 0x0000FFFF)

/// Macro retrieving the center frequency 2 of the phy channel info
/// @param[in] __x phy channel info 2 value.
#define PHY_INFO_CENT_FREQ2(__x) (((__x.info2) & 0xFFFF0000) >> 16)

/// @}


/// Receive Vector specific part for NON-HT and NON-HT-DUP-OFDM frames
struct rx_vect_1_leg
{
    /// Dynamic Bandwidth
    uint8_t    dyn_bw_in_non_ht     : 1;
    /// Channel Bandwidth
    uint8_t    chn_bw_in_non_ht     : 2;
    /// Not used (offset only)
    uint8_t    rsvd_nht             : 4;
    /// L-SIG Valid
    uint8_t    lsig_valid           : 1;
} __packed;

/// Receive Vector specific part for HT frames
struct rx_vect_1_ht
{
    /// Sounding bit
    uint16_t   sounding             : 1;
    /// Smoothing bit
    uint16_t   smoothing            : 1;
    /// Guard Interval Type bit
    uint16_t   short_gi             : 1;
    /// MPDU Aggregate bit
    uint16_t   aggregation          : 1;
    /// Space Time Block Coding bit
    uint16_t   stbc                 : 1;
    /// Number of Extension Spatial Streams
    uint16_t   num_extn_ss          : 2;
    /// L-SIG Valid
    uint16_t   lsig_valid           : 1;
    /// Modulation Coding Scheme
    uint16_t   mcs                  : 7;
    /// FEC Coding
    uint16_t   fec                  : 1;
    /// Lenght of HT PPDU
    uint16_t   length               :16;
} __packed;

/// Receive Vector specific part for VHT frames
struct rx_vect_1_vht
{
    /// Sounding bit
    uint8_t   sounding              : 1;
    /// BeamFormed bit
    uint8_t   beamformed            : 1;
    /// Guard Interval Type
    uint8_t   short_gi              : 1;
    /// Not used (offset only)
    uint8_t   rsvd_vht1             : 1;
    /// Space Time Block Coding
    uint8_t   stbc                  : 1;
    /// TXOP PS Not Allowed
    uint8_t   doze_not_allowed      : 1;
    /// First User
    uint8_t   first_user            : 1;
    /// Not used (offset only)
    uint8_t   rsvd_vht2             : 1;
    /// Partial AID
    uint16_t  partial_aid           : 9;
    /// Group ID
    uint16_t  group_id              : 6;
    /// Not used (offset only)
    uint16_t  rsvd_vht3             : 1;
    /// Modulation Coding Scheme
    uint32_t  mcs                   : 4;
    /// Number of Spatial Streams
    uint32_t  nss                   : 3;
    /// FEC Coding
    uint32_t  fec                   : 1;
    /// Lenght of VHT PPDU
    uint32_t  length                :20;
    /// Not used (offset only)
    uint32_t  rsvd_vht4             : 4;
} __packed;

/// Receive Vector specific part for HE frames
struct rx_vect_1_he
{
    /// Sounding bit
    uint8_t   sounding              : 1;
    /// BeamFormed bit
    uint8_t   beamformed            : 1;
    /// Guard Interval Type
    uint8_t   gi_type               : 2;
    /// Space Time Block Coding
    uint8_t   stbc                  : 1;
    /// Not Used (offset only)
    uint8_t   rsvd_he1              : 3;
    /// UP link Flag
    uint8_t   uplink_flag           : 1;
    /// Beam Change
    uint8_t   beam_change           : 1;
    /// Dual Carrier Modulation
    uint8_t   dcm                   : 1;
    /// Type of HE-LTF
    uint8_t   he_ltf_type           : 2;
    /// Doppler bit
    uint8_t   doppler               : 1;
    /// Not Used (offset only)
    uint8_t   rsvd_he2              : 2;
    /// BSS Color
    uint8_t   bss_color             : 6;
    /// Not Used (offset only)
    uint8_t   rsvd_he3              : 2;
    /// Duration of TX OP
    uint8_t   txop_duration         : 7;
    /// Not Used (offset only)
    uint8_t   rsvd_he4              : 1;
    /// Packet Extension Duration
    uint8_t   pe_duration           : 4;
    /// Spatial Reuse
    uint8_t   spatial_reuse         : 4;

    /// SIG-B Compression Mode
    uint8_t   sig_b_comp_mode       : 1;
    /// SIG-B Dual Carrier Modulation
    uint8_t   dcm_sig_b             : 1;
    /// SIG-B Modulation Coding Scheme
    uint8_t   mcs_sig_b             : 3;
    /// RU Size
    uint8_t   ru_size               : 3;

    /// Modulation Coding Scheme
    uint32_t  mcs                   : 4;
    /// Number of Spatial Streams
    uint32_t  nss                   : 3;
    /// FEC Coding
    uint32_t  fec                   : 1;
    /// Length of PPDU
    uint32_t  length                :20;
    /// Not Used (offset only)
    uint32_t  rsvd_he6              : 4;
} __packed;


struct rx_vector_1
{
    /// Format Modulation
    uint8_t     format_mod         : 4;
    /// Channel Bandwidth
    uint8_t     ch_bw              : 3;
    /// Preamble Type
    uint8_t     pre_type           : 1;
    /// Antenna Set
    uint8_t     antenna_set        : 8;
    /// RSSI Legacy
    int32_t     rssi_leg           : 8;
    /// Legacy Length
    uint32_t    leg_length         :12;
    /// Legacy rate
    uint32_t    leg_rate           : 4;
    /// RSSI
    int32_t     rssi1              : 8;
    union
    {
        /// non-ht and non-ht-dup-ofdm bitmap
        struct rx_vect_1_leg leg;
        /// ht-mm and ht-gf bitmap
        struct rx_vect_1_ht ht;
        /// vht bitmap
        struct rx_vect_1_vht vht;
        /// he bitmap
        struct rx_vect_1_he he;
    };
} __packed;

/// Structure for receive Vector 2
struct rx_vector_2
{
    /// Contains the bytes 4 - 1 of Receive Vector 2
    uint32_t            recvec2a;
    ///  Contains the bytes 8 - 5 of Receive Vector 2
    uint32_t            recvec2b;
};

/// Structure containing information about the received frame (length, timestamp, rate, etc.)
struct rx_vector
{
    /// Total length of the received MPDU
    uint16_t            frmlen;
    /// AMPDU status information
    uint16_t            ampdu_stat_info;
    /// TSF Low
    uint32_t            tsflo;
    /// TSF High
    uint32_t            tsfhi;
    /// Receive Vector 1
    struct rx_vector_1  rx_vec_1;
    /// Receive Vector 2
    struct rx_vector_2  rx_vec_2;
    /// MPDU status information
    uint32_t            statinfo;
};

/// Structure containing the information about the PHY channel that was used for this RX
struct phy_channel_info
{
    /// PHY channel information 1
    uint32_t info1;
    /// PHY channel information 2
    uint32_t info2;
};

/// Structure containing the information about the received payload
struct rx_info
{
    /// Rx header descriptor (this element MUST be the first of the structure)
    struct rx_vector vect;
    /// Structure containing the information about the PHY channel that was used for this RX
    struct phy_channel_info phy_info;
    /// Word containing some SW flags about the RX packet
    uint32_t flags;
    /// Array of host buffer identifiers for the other A-MSDU subframes
    uint32_t amsdu_hostids[NX_MAX_MSDU_PER_RX_AMSDU - 1];
    /// Spare room for LMAC FW to write a pattern when last DMA is sent
    uint32_t pattern;
};

/** @name LLC field definitions
 * LLC = DSAP + SSAP + CTRL
 * @note: The 0xAA values indicate the presence of the SNAP
 */
#define LLC_LLC_LEN                  3
#define LLC_DSAP                     0xAA
#define LLC_SSAP                     0xAA
#define LLC_CTRL                     0x03

/** @name SNAP field definitions
 * SNAP = OUI + ETHER_TYPE
 */
#define LLC_SNAP_LEN                 5
#define LLC_OUI_LEN                  3

/** @name 802.2 Header definitions
 * The 802.2 Header is the LLC + the SNAP
 */
#define LLC_802_2_HDR_LEN            (LLC_LLC_LEN + LLC_SNAP_LEN)

/** @name 802.2 Ethernet definitions */
/// Ethernet MTU
#define LLC_ETHER_MTU                1500
/// Size of the Ethernet header
#define LLC_ETHER_HDR_LEN            14
/// Size of the Ethernet type
#define LLC_ETHERTYPE_LEN            2

/// Maximum A-MSDU subframe length we support (Ethernet Header + LLC/SNAP + Ethernet MTU)
#define RX_MAX_AMSDU_SUBFRAME_LEN (LLC_ETHER_MTU + LLC_ETHER_HDR_LEN + LLC_802_2_HDR_LEN)

/// Size of a RX buffer
#define FHOST_RX_BUF_SIZE           (RX_MAX_AMSDU_SUBFRAME_LEN + 1)

/*
* GLOBAL VARIABLES
****************************************************************************************
*/
extern const u8 mac_tid2ac[];

extern const u8 mac_id2rate[];

extern const u16 mac_mcs_params_20[];

extern const u16 mac_mcs_params_40[];

/// @}

#endif // _MAC_H_
