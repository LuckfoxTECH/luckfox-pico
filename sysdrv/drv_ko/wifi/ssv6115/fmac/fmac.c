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

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kthread.h>
#include <linux/inetdevice.h>
#include <linux/of.h>
#include <net/cfg80211.h>
#include <linux/etherdevice.h>

#include "ssv_cfg.h"
#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "fmac_tx.h"
#include "fmac_rx.h"
#include "fmac_msg_tx.h"
#include "netdev_ops.h"
#include "ssv_debug.h"
#include <linux/semaphore.h>

#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
#include <linux/icmp.h>

/*******************************************************************************
 *         Local Structures
 ******************************************************************************/
struct dhcp_data {
	u8 op;
	u8 htype;
	u8 hlen;
	u8 hops;
	u32 xid;
	u16 secs;
	u16 flags;
	u32 client_ip;
	u32 your_ip;
	u32 server_ip;
	u32 relay_ip;
	u8 hw_addr[16];
	u8 serv_name[64];
	u8 boot_file[128];
} STRUCT_PACKED;

/*******************************************************************************
 *         Local Variables
 ******************************************************************************/
/* DHCP message types */
#define DHCPDISCOVER	1
#define DHCPOFFER	    2
#define DHCPREQUEST 	3
#define DHCPDECLINE 	4
#define DHCPACK		    5
#define DHCPNAK		    6
#define DHCPRELEASE 	7
#define DHCPINFORM  	8

#define SSV_HT_CAPABILITIES                                    \
{                                                               \
    .ht_supported   = true,                                     \
    .cap            = 0,                                        \
    .ampdu_factor   = IEEE80211_HT_MAX_AMPDU_64K,               \
    .ampdu_density  = IEEE80211_HT_MPDU_DENSITY_16,             \
    .mcs        = {                                             \
        .rx_mask = { 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, },        \
        .rx_highest = cpu_to_le16(65),                          \
        .tx_params = IEEE80211_HT_MCS_TX_DEFINED,               \
    },                                                          \
}

#define SSV_VHT_CAPABILITIES                                   \
{                                                               \
    .vht_supported = true,                                     \
    .cap       =                                                \
      (7 << IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_SHIFT),\
    .vht_mcs       = {                                          \
        .rx_mcs_map = cpu_to_le16(                              \
                      IEEE80211_VHT_MCS_SUPPORT_0_9    << 0  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 2  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 4  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 6  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 8  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 10 |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 12 |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 14),  \
        .tx_mcs_map = cpu_to_le16(                              \
                      IEEE80211_VHT_MCS_SUPPORT_0_9    << 0  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 2  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 4  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 6  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 8  |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 10 |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 12 |  \
                      IEEE80211_VHT_MCS_NOT_SUPPORTED  << 14),  \
    }                                                           \
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
#define SSV_HE_CAPABILITIES                                    \
{                                                               \
    .has_he = false,                                            \
    .he_cap_elem = {                                            \
        .mac_cap_info[0] = 0,                                   \
        .mac_cap_info[1] = 0,                                   \
        .mac_cap_info[2] = 0,                                   \
        .mac_cap_info[3] = 0,                                   \
        .mac_cap_info[4] = 0,                                   \
        .mac_cap_info[5] = 0,                                   \
        .phy_cap_info[0] = 0,                                   \
        .phy_cap_info[1] = 0,                                   \
        .phy_cap_info[2] = 0,                                   \
        .phy_cap_info[3] = 0,                                   \
        .phy_cap_info[4] = 0,                                   \
        .phy_cap_info[5] = 0,                                   \
        .phy_cap_info[6] = 0,                                   \
        .phy_cap_info[7] = 0,                                   \
        .phy_cap_info[8] = 0,                                   \
        .phy_cap_info[9] = 0,                                   \
        .phy_cap_info[10] = 0,                                  \
    },                                                          \
    .he_mcs_nss_supp = {                                        \
        .rx_mcs_80 = cpu_to_le16(0xfffa),                       \
        .tx_mcs_80 = cpu_to_le16(0xfffa),                       \
        .rx_mcs_160 = cpu_to_le16(0xffff),                      \
        .tx_mcs_160 = cpu_to_le16(0xffff),                      \
        .rx_mcs_80p80 = cpu_to_le16(0xffff),                    \
        .tx_mcs_80p80 = cpu_to_le16(0xffff),                    \
    },                                                          \
    .ppe_thres = {0x00},                                        \
}
#endif

#define RATE(_bitrate, _hw_rate, _flags) {      \
    .bitrate    = (_bitrate),                   \
    .flags      = (_flags),                     \
    .hw_value   = (_hw_rate),                   \
}

#define CHAN(_freq) {                           \
    .center_freq    = (_freq),                  \
    .max_power  = 30, /* FIXME */               \
}

static struct ieee80211_rate ssv_ratetable[] = {
    RATE(10,  0x00, 0),
    RATE(20,  0x01, IEEE80211_RATE_SHORT_PREAMBLE),
    RATE(55,  0x02, IEEE80211_RATE_SHORT_PREAMBLE),
    RATE(110, 0x03, IEEE80211_RATE_SHORT_PREAMBLE),
    RATE(60,  0x04, 0),
    RATE(90,  0x05, 0),
    RATE(120, 0x06, 0),
    RATE(180, 0x07, 0),
    RATE(240, 0x08, 0),
    RATE(360, 0x09, 0),
    RATE(480, 0x0A, 0),
    RATE(540, 0x0B, 0),
};

/* The channels indexes here are not used anymore */
static struct ieee80211_channel ssv_2ghz_channels[] = {
    CHAN(2412),
    CHAN(2417),
    CHAN(2422),
    CHAN(2427),
    CHAN(2432),
    CHAN(2437),
    CHAN(2442),
    CHAN(2447),
    CHAN(2452),
    CHAN(2457),
    CHAN(2462),
    CHAN(2467),
    CHAN(2472),
    CHAN(2484),
};

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
static struct ieee80211_sband_iftype_data ssv_he_capa = {
    .types_mask = BIT(NL80211_IFTYPE_STATION) | BIT(NL80211_IFTYPE_AP),
    .he_cap = SSV_HE_CAPABILITIES,
};
#endif

static struct ieee80211_supported_band ssv_band_2GHz = {
    .channels   = ssv_2ghz_channels,
    .n_channels = ARRAY_SIZE(ssv_2ghz_channels),
    .bitrates   = ssv_ratetable,
    .n_bitrates = ARRAY_SIZE(ssv_ratetable),
    .ht_cap     = SSV_HT_CAPABILITIES,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    .vht_cap    = SSV_VHT_CAPABILITIES,
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)) || defined(IEEE80211_HE_MAC_CAP2_TRS)
    .iftype_data = &ssv_he_capa,
    .n_iftype_data = 1,
#endif
};

static struct ieee80211_iface_limit ssv_limits[] = {
    { .max = NX_VIRT_DEV_MAX, .types = BIT(NL80211_IFTYPE_AP) |
                                       BIT(NL80211_IFTYPE_STATION)}
};

static struct ieee80211_iface_limit ssv_limits_dfs[] = {
    { .max = NX_VIRT_DEV_MAX, .types = BIT(NL80211_IFTYPE_AP)}
};

static const struct ieee80211_iface_combination ssv_combinations[] = {
    {
        .limits                 = ssv_limits,
        .n_limits               = ARRAY_SIZE(ssv_limits),
        .num_different_channels = NX_CHAN_CTXT_CNT,
        .max_interfaces         = NX_VIRT_DEV_MAX,
    },
    /* Keep this combination as the last one */
    {
        .limits                 = ssv_limits_dfs,
        .n_limits               = ARRAY_SIZE(ssv_limits_dfs),
        .num_different_channels = 1,
        .max_interfaces         = NX_VIRT_DEV_MAX,
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 8, 0)
        .radar_detect_widths = (BIT(NL80211_CHAN_WIDTH_20_NOHT) |
                                BIT(NL80211_CHAN_WIDTH_20) |
                                BIT(NL80211_CHAN_WIDTH_40) |
                                BIT(NL80211_CHAN_WIDTH_80)),
#endif
    }
};

/* There isn't a lot of sense in it, but you can transmit anything you like */
static struct ieee80211_txrx_stypes
ssv_default_mgmt_stypes[NUM_NL80211_IFTYPES] = {
    [NL80211_IFTYPE_STATION] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3)) || defined(CONFIG_WPA_SUPPLICANT_CTL)
            BIT(IEEE80211_STYPE_AUTH >> 4) |
#endif
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
    },
    [NL80211_IFTYPE_AP] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
            BIT(IEEE80211_STYPE_DISASSOC >> 4) |
            BIT(IEEE80211_STYPE_AUTH >> 4) |
            BIT(IEEE80211_STYPE_DEAUTH >> 4) |
            BIT(IEEE80211_STYPE_ACTION >> 4),
    },
    [NL80211_IFTYPE_AP_VLAN] = {
        /* copy AP */
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
            BIT(IEEE80211_STYPE_DISASSOC >> 4) |
            BIT(IEEE80211_STYPE_AUTH >> 4) |
            BIT(IEEE80211_STYPE_DEAUTH >> 4) |
            BIT(IEEE80211_STYPE_ACTION >> 4),
    },
    [NL80211_IFTYPE_P2P_CLIENT] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
    },
    [NL80211_IFTYPE_P2P_GO] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
            BIT(IEEE80211_STYPE_DISASSOC >> 4) |
            BIT(IEEE80211_STYPE_AUTH >> 4) |
            BIT(IEEE80211_STYPE_DEAUTH >> 4) |
            BIT(IEEE80211_STYPE_ACTION >> 4),
    },
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
    [NL80211_IFTYPE_P2P_DEVICE] = {
        .tx = 0xffff,
        .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
            BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
    },
#endif
};


static u32 cipher_suites[] = {
    WLAN_CIPHER_SUITE_WEP40,
    WLAN_CIPHER_SUITE_WEP104,
    WLAN_CIPHER_SUITE_TKIP,
    WLAN_CIPHER_SUITE_CCMP,
    0, // reserved entries to enable AES-CMAC and/or SMS4
    0,
};
#define NB_RESERVED_CIPHER 2;


extern struct ssv6xxx_cfg ssv_cfg;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
extern int ssv_netdev_notifier_register(void);
extern void ssv_netdev_notifier_unregister(void);
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
static void ssv_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request)
#else
static int ssv_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request)
#endif
{
    // TODO
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
    return;
#else
    return 0;
#endif
}

static void ssv6xxx_init_wiphy(struct ssv_softc *sc)
{
    struct wiphy *wiphy = sc->wiphy;

    // set mac address
    memcpy((void *)wiphy->perm_addr, (const void *)&sc->maddr[0][0], ETH_ALEN);
    //memcpy(wiphy->perm_addr, init_conf.mac_addr, ETH_ALEN);
    wiphy->mgmt_stypes = ssv_default_mgmt_stypes;
    wiphy->bands[NL80211_BAND_2GHZ] = &ssv_band_2GHz;
    wiphy->interface_modes =
            BIT(NL80211_IFTYPE_STATION)     |
            BIT(NL80211_IFTYPE_AP)          |
            BIT(NL80211_IFTYPE_AP_VLAN)     |
            BIT(NL80211_IFTYPE_MONITOR)     |
            BIT(NL80211_IFTYPE_P2P_CLIENT)  |
            BIT(NL80211_IFTYPE_P2P_GO);

    wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL |
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 12, 0)
            WIPHY_FLAG_HAS_CHANNEL_SWITCH |
#endif
            WIPHY_FLAG_4ADDR_STATION |
            WIPHY_FLAG_4ADDR_AP;

    //wiphy->max_num_csa_counters = BCN_MAX_CSA_CPT;

    wiphy->max_remain_on_channel_duration = ssv_mod_params.roc_dur_max;

#ifdef NEED_OBSS_SCAN
    wiphy->features |= NL80211_FEATURE_NEED_OBSS_SCAN;
#endif
    wiphy->features |= NL80211_FEATURE_SK_TX_STATUS;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    wiphy->features |= NL80211_FEATURE_VIF_TXPOWER;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
    wiphy->features |= NL80211_FEATURE_AP_MODE_CHAN_WIDTH_CHANGE;
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3))
    wiphy->features |= NL80211_FEATURE_SAE; 
#elif defined(CONFIG_WPA_SUPPLICANT_CTL)
    wiphy->features |= SSV_NL80211_FEATURE_SAE;
#endif

    wiphy->iface_combinations   = ssv_combinations;
    /*  -1 not to include combination with radar detection, will be re-added in
     *      bl_handle_dynparams if supported */
    wiphy->n_iface_combinations = ARRAY_SIZE(ssv_combinations) - 1;
    wiphy->reg_notifier = ssv_reg_notifier;
    wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
    wiphy->cipher_suites = cipher_suites;
    wiphy->n_cipher_suites = ARRAY_SIZE(cipher_suites) - NB_RESERVED_CIPHER;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
    sc->ext_capa[0] = WLAN_EXT_CAPA1_EXT_CHANNEL_SWITCHING;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
    sc->ext_capa[7] = WLAN_EXT_CAPA8_OPMODE_NOTIF;
    wiphy->extended_capabilities = sc->ext_capa;
    wiphy->extended_capabilities_mask = sc->ext_capa;
    wiphy->extended_capabilities_len = ARRAY_SIZE(sc->ext_capa);
#endif
}

struct ssv_sta *ssv_get_sta(struct ssv_softc *sc, const u8 *mac_addr)
{
    int i;

    for (i = 0; i < NX_REMOTE_STA_MAX; i++) {
        struct ssv_sta *sta = &sc->sta_table[i];
        if (sta->valid && (memcmp(mac_addr, &sta->mac_addr, 6) == 0))
            return sta;
    }

    return NULL;
}


enum data_frame_types ssv_get_data_frame_type(struct sk_buff *skb)
{
    enum data_frame_types type = SSV_DATA_UNKNOW;
    struct ethhdr *ethhdr;
    struct iphdr *iphdr;
    struct arphdr *arphdr;
    struct udphdr *udphdr;
    struct icmphdr *icmphdr;
       
    ethhdr = (struct ethhdr *)skb->data;
    //SSV_LOG_DBG( "ethhdr->h_proto = 0x%04x\n", ntohs(ethhdr->h_proto));
    
    /* |                                                                |
     * |     14B         20B         8B                                 |
     * +------------+-----------+-----------+---------------------------+
     * |   ethhdr   |   iphdr   |  udphdr   |           DATA            |
     * +------------+-----------+-----------+---------------------------+
     *
    */

    switch(ntohs(ethhdr->h_proto)){
        case ETH_P_IP : //htons(ETH_P_IP): // 0x0800
        {                   
            iphdr = (struct iphdr *)(skb->data + sizeof(struct ethhdr)); //sizeof(struct ethhdr)=14
            //SSV_LOG_DBG( "iphdr->protocol = %0x, iphdr->daddr = 0x%x, iphdr->saddr = 0x%x\n", iphdr->protocol, ntohl(iphdr->daddr), ntohl(iphdr->saddr)); 
            
            switch(iphdr->protocol){
                case IPPROTO_UDP: // IPPROTO_UDP = 17
    		        {
                        struct dhcp_data *dhcp;
                        u8 *opt;
                        u8 msgtype = 0;
                        udphdr = (struct udphdr *)(skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr));
                                            
    		            if(udphdr->dest == htons(67) || udphdr->source == htons(67)) 
                        {
                            dhcp = (struct dhcp_data *)(skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
                            opt = (u8 *)(dhcp + 1);
                            opt += 4; //RFC1048 magic cookie?
                            *opt++ = 53;
                            *opt++ = 1;
                            msgtype = *opt;
                            if(msgtype == DHCPDISCOVER){
                               type = SSV_DHCP_DISCOVER; 
                            }else if(msgtype == DHCPOFFER){
                                type = SSV_DHCP_OFFER;
                            }else if(msgtype == DHCPREQUEST){
                                type = SSV_DHCP_REQUEST;
                            }else if(msgtype == DHCPACK){
                                type = SSV_DHCP_ACK;
                            }else{
                                break;
                            }                                                
                        }
                        break;
                    }

                case IPPROTO_ICMP: //IPPROTO_ICMP = 1
                    {
                        icmphdr = (struct icmphdr *)(skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr));
                        if(icmphdr->type == ICMP_ECHO){
                            type = SSV_ICMP_ECHO; //ICMP_ECHO = 8
                            break; 
                        }                           
                        if(icmphdr->type == ICMP_ECHOREPLY){
                            type = SSV_ICMP_ECHOREPLY; //ICMP_ECHOREPLY = 0
                            break; 
                        }                             
                    }
                default:
                    break;    
            }
        break;
        }       
        
        case ETH_P_ARP : //htons(ETH_P_ARP): //0x0806
        {
            arphdr = (struct arphdr *)(skb->data + sizeof(struct ethhdr));
            /* check whether the ARP packet carries a valid IP information */
	        if (arphdr->ar_hrd != htons(ARPHRD_ETHER))
		        break;
	        if (arphdr->ar_pro != htons(ETH_P_IP))
		        break;
	        if (arphdr->ar_hln != ETH_ALEN)
		        break;
	        if (arphdr->ar_pln != 4)
		        break;
            if (arphdr->ar_op == htons(ARPOP_REPLY)){
                type = SSV_ARP_REPLY; //ARPOP_REPLY = 2
                break;
            } 
            if (arphdr->ar_op == htons(ARPOP_REQUEST)){
                type = SSV_ARP_REQUEST; //ARPOP_REQUEST = 1
                break;
            }
            break;            
        }

        case ETH_P_PAE : //0x888e
            type = SSV_EAPOL;
            break;
        default:
            break;
    }
       
    return type; 
}

void ssv_enable_wapi(struct ssv_softc *sc)
{
    cipher_suites[sc->wiphy->n_cipher_suites] = WLAN_CIPHER_SUITE_SMS4;
    sc->wiphy->n_cipher_suites++;
    sc->wiphy->flags |= WIPHY_FLAG_CONTROL_PORT_PROTOCOL;
}

void ssv_enable_mfp(struct ssv_softc *sc)
{
    cipher_suites[sc->wiphy->n_cipher_suites] = WLAN_CIPHER_SUITE_AES_CMAC;
    sc->wiphy->n_cipher_suites++;
}

#ifndef DIS_NETDEV_INIT
#ifdef FW_RESET_AT_INIT
static void ssv_set_vers(struct ssv_softc *sc)
{
    u32 vers = sc->version_cfm.version_lmac;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    snprintf(sc->wiphy->fw_version,
             sizeof(sc->wiphy->fw_version), "%d.%d.%d.%d",
             (vers & (0xff << 24)) >> 24, (vers & (0xff << 16)) >> 16,
             (vers & (0xff <<  8)) >>  8, (vers & (0xff <<  0)) >>  0);
}
#endif
#endif

static void ssv_overwrite_modparams(struct ssv_softc *sc)
{
    sc->mod_params->use_2040 = ssv_cfg.use_2040; 
    sc->mod_params->tx_lft = ssv_cfg.ampdu_tx_lft;
    sc->mod_params->he_on = ssv_cfg.he_on;
}

extern struct cfg80211_ops ssv_cfg80211_ops;
extern void ssv_wdev_unregister(struct ssv_softc *sc);
/* ssv6xxx_fmac_init must hook hci callback function
 * ex: ssv6xxx_hci_proc_rx_register()
 * */
int ssv6xxx_fmac_init(void **plat_sc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, u8 *maddr, struct device *dev)
{
    struct ssv_softc *sc = NULL;
    struct wiphy *wiphy;
#ifndef DIS_NETDEV_INIT
    struct wireless_dev *wdev;
    int i = 0,j = 0;
#endif
    int ret;

    /* create a new wiphy for use with cfg80211 */
    wiphy = wiphy_new(&ssv_cfg80211_ops, sizeof(struct ssv_softc));
    if (!wiphy) {
        SSV_LOG_DBG("No memory for wiphy\n");
        ret = -ENOMEM;
        goto err;
    }

    sc = wiphy_priv(wiphy);
    memset((void *)sc, 0, sizeof(struct ssv_softc));
    sc->wiphy = wiphy;
    sc->hci_priv = hci_priv;
    sc->hci_ops = hci_ops;
    sc->dev = dev;

    memcpy(sc->maddr,maddr,sizeof(sc->maddr));
    
    /* set device pointer for wiphy */
    set_wiphy_dev(wiphy, sc->dev);
    wiphy->privid = sc;

    sc->now_ack_seq=0;
    skb_queue_head_init(&sc->wait_sw_ack_q);

    ssv6xxx_init_wiphy(sc);
#ifndef DIS_NETDEV_INIT
    sc->mod_params = &ssv_mod_params;
    ssv_overwrite_modparams(sc);
    
    for (i = 0; i < NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX; i++)
    {
        sc->avail_idx_map |= BIT(i);
    }

#if 0 ///@FIXME: phy_config
    {
        /// PHY configuration (This is only valid for KARST radio)
        u32_l phy_cfg[] = {0x01000000, 0x01000000, 0x01000000, 0x01000000,
                           0x01000000, 0x01000000, 0x01000000, 0x01000000,
                           0x0};
        memcpy((void *)&sc->phy_config, (const void *)&phy_cfg[0], 33);  
    }
#endif

    for (i = 0; i < NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX; i++)
    {
        for(j = 0; j < TID_MAX ; j++){
            sc->sta_table[i].rx_last_seqcntl[j] = 0xFFFF;
        }
    }

    if ((ret = ssv_ipc_init(sc)))
    {
        goto err_platon;
    }

    INIT_LIST_HEAD(&sc->vifs);

    mutex_init(&sc->cmd_lock);
    mutex_init(&sc->cb_lock);
    sema_init( &sc->reconnect_sem, 0);

    sc->cmd_sent = false;
    INIT_LIST_HEAD(&sc->reset_cmd.list);
    sc->recovery_flag = true;
    
    // Set rx path callback after initializing vifs list
    // Because it access vifs list in rx path
    ssv_drv_hci_set_trigger_conf(sc->hci_priv, sc->hci_ops, ssv_cfg.hci_trigger_en, ssv_cfg.hci_trigger_qlen);
#ifndef DIS_NETDEV_INIT
    // HCI TX aggregation setting
    ssv_drv_hci_set_cap(sc->hci_priv, sc->hci_ops, HCI_CAP_TX_AGGR, ((ssv_cfg.hw_caps&HW_CAP_HCI_TX_AGGR)?true:false));
    // HCI RX aggregation setting
    ssv_drv_hci_set_cap(sc->hci_priv, sc->hci_ops, HCI_CAP_RX_AGGR, true); // Always enable HCI RX aggregation.
    ssv_send_io_aggr_setting(sc, true, ((ssv_cfg.hw_caps&HW_CAP_HCI_RX_AGGR)?true:false));
    ssv6xxx_hci_proc_rx_register((void *)hci_priv, ssv_rx_packet_ind, (void *)sc);
#endif
    ssv6xxx_check_resource_register((void *)hci_priv, ssv_tx_check_resource, (void *)sc);

#ifdef FW_RESET_AT_INIT
    /* Reset FW */
    if ((ret = ssv_send_reset(sc)))
        goto err_lmac_reqs;

    if ((ret = ssv_send_version_req(sc, &sc->version_cfm)))
        goto err_lmac_reqs;
    ssv_set_vers(sc);

    if ((ret = ssv_handle_dynparams(sc, sc->wiphy)))
        goto err_lmac_reqs;

    /* Set parameters to firmware */
    ssv_send_me_config_req(sc);
#endif
#endif //#ifndef DIS_NETDEV_INIT
    if ((ret = wiphy_register(wiphy))) {
        SSV_LOG_DBG("Could not register wiphy device\n");
        ret = -ENODEV;
        goto err_register_wiphy;
    }

    /* Set channel parameters to firmware (must be done after WiPHY registration) */
    ssv_send_me_chan_config_req(sc);

    *plat_sc = sc;
    SSV_LOG_DBG("ssv wiphy %s\n", wiphy_name(sc->wiphy));

 #ifndef DIS_NETDEV_INIT
    rtnl_lock();
    /* Add an initial station interface */
    wdev = ssv_interface_add(sc, "wlan%d", NL80211_IFTYPE_STATION, NULL);
    rtnl_unlock();

    if (!wdev) {
        wiphy_err(wiphy, "Failed to instantiate a network device\n");
        ret = -ENOMEM;
        goto err_add_interface;
    }

    rtnl_lock();
    wdev = ssv_interface_add(sc, "p2p0", NL80211_IFTYPE_STATION, NULL);
    rtnl_unlock();

    if (!wdev) {
        wiphy_err(wiphy, "Failed to instantiate a network device\n");
        ret = -ENOMEM;
        goto err_add_interface;
    }

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    wiphy_info(wiphy, "New interface create %s", wdev->netdev->name);
#endif //#ifndef DIS_NETDEV_INIT

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
    /*
     * Register netdev notifier to listen netdev state for Kernel < 3.6, 
     * and call stop_ap while netdev(AP mode) is going down, 
     * as this version of cfg80211 lacks this operation.
     */
    ret = ssv_netdev_notifier_register();
    if (ret) {
        SSV_LOG_DBG("Fail to register netdev notifier\n");
        goto err_netdev_notifier;
    }
#endif

#if 1
    //set ampdu tx rx
    ssv_send_priv_msg_ampdu_setting(sc, ssv_cfg.hw_caps);
#endif
    // filter duplicate rx
    ssv_send_filter_duplicate_rx(sc, ssv_cfg.filter_duplicate_rx);
    //ssv_send_wmm_follow_vo(sc, ssv_cfg.wmm_follow_vo);
    ssv_send_ipc_tx_use_one_hwq(sc, ssv_cfg.ipc_tx_use_one_hwq);
    ssv_send_macaddr(sc, (u8 *)&sc->maddr[0][0], (u8 *)&sc->maddr[1][0]);
    ssv_send_set_policy_tbl(sc, ssv_cfg.set_rts_method);
    ssv_send_txq_credit_boundary(sc);
    ssv_send_mac_addr_rule_setting(sc, ssv_cfg.mac_low_mask,ssv_cfg.mac_high_mask);
#ifdef CONFIG_MIFI_LOWPOWER
    ssv_send_priv_msg_mifi_setting(sc, ssv_cfg.mifi_feature,  ssv_cfg.mifi_no_traffic_duration);
#endif
    sc->recovery_flag = false;
    return 0;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
err_netdev_notifier:
    ssv_wdev_unregister(sc);
#endif
#ifndef DIS_NETDEV_INIT
err_add_interface:
#endif
    wiphy_unregister(sc->wiphy);
err_register_wiphy:
#ifndef DIS_NETDEV_INIT
#ifdef FW_RESET_AT_INIT
err_lmac_reqs:
#endif
    ssv_ipc_deinit(sc);
err_platon:
    ssv6xxx_hci_proc_rx_unregister((void *)sc->hci_priv, ssv_rx_packet_ind, NULL);
#endif
    wiphy_free(wiphy);
err:
    return ret;
}
EXPORT_SYMBOL(ssv6xxx_fmac_init);

static void ssv6xxx_fmac_stop_all_running_threads(struct ssv_softc *sc)
{
    if (sc->ssv_txtput.txtput_tsk) {
        SSV_LOG_DBG("Stopping txtput task...\n");
        kthread_stop(sc->ssv_txtput.txtput_tsk);
        while (sc->ssv_txtput.txtput_tsk != NULL) {
            msleep(1);
        }
        SSV_LOG_DBG("txtput task is stopped.\n");
    }
}

void ssv6xxx_fmac_deinit(void *plat_sc)
{
    struct ssv_softc *sc = (struct ssv_softc *)plat_sc;
    struct sk_buff *clone_skb;
    struct fw_reset_cmd *cur, *tmp;
    ssv6xxx_fmac_stop_all_running_threads(sc);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
    ssv_netdev_notifier_unregister();
#endif

#ifndef DIS_NETDEV_INIT
    ssv_wdev_unregister(sc);
#endif

    wiphy_unregister(sc->wiphy);

#ifndef  DIS_NETDEV_INIT
    ssv6xxx_hci_proc_rx_unregister((void *)sc->hci_priv, ssv_rx_packet_ind, NULL);
    ssv_ipc_deinit(sc);
    ssv_drv_hci_stop(sc->hci_priv, sc->hci_ops); // pause all hci wifi sw txq
#endif

    while( (clone_skb = skb_dequeue(&sc->wait_sw_ack_q)) )
    {
        dev_kfree_skb_any(clone_skb);
    }

    list_for_each_entry_safe(cur, tmp, &sc->reset_cmd.list, list) {
        list_del(&cur->list);
        kfree(cur);
    }
    
    //this will free sc, because sc is private member of wiphy
    wiphy_free(sc->wiphy); 
    return;
}
EXPORT_SYMBOL(ssv6xxx_fmac_deinit);
