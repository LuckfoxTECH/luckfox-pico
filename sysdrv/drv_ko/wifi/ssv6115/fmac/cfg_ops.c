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
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#include <linux/of.h>
#include <net/cfg80211.h>
#include <linux/etherdevice.h>

#include "fmac.h"
#include "hci/drv_hci_ops.h"
#include "fmac_msg_tx.h"
#include "fmac_defs.h"
#include "fmac_tx.h"
#include "host_msg.h"
#include "netdev_ops.h"
#include "ssv_debug.h"
#include "ssv_cfg.h"
#include <linux/semaphore.h>

#ifdef FMAC_BRIDGE
#include "fmac_bridge.h"
#endif

extern struct ssv6xxx_cfg ssv_cfg;

extern u8 ssv_get_engmode(void);

static const int ssv_ac2hwq[1][NL80211_NUM_ACS] = {
    {
        [NL80211_TXQ_Q_VO] = SSV_HWQ_VO,
        [NL80211_TXQ_Q_VI] = SSV_HWQ_VI,
        [NL80211_TXQ_Q_BE] = SSV_HWQ_BE,
        [NL80211_TXQ_Q_BK] = SSV_HWQ_BK
    }
};

const int ssv_tid2hwq[IEEE80211_NUM_TIDS] = {
    SSV_HWQ_BE,
    SSV_HWQ_BK,
    SSV_HWQ_BK,
    SSV_HWQ_BE,
    SSV_HWQ_VI,
    SSV_HWQ_VI,
    SSV_HWQ_VO,
    SSV_HWQ_VO,
    /* TID_8 is used for management frames */
    SSV_HWQ_VO,
    /* At the moment, all others TID are mapped to BE */
    SSV_HWQ_BE,
    SSV_HWQ_BE,
    SSV_HWQ_BE,
    SSV_HWQ_BE,
    SSV_HWQ_BE,
    SSV_HWQ_BE,
    SSV_HWQ_BE,
};

static const int ssv_hwq2uapsd[NL80211_NUM_ACS] = {
    [SSV_HWQ_VO] = IEEE80211_WMM_IE_STA_QOSINFO_AC_VO,
    [SSV_HWQ_VI] = IEEE80211_WMM_IE_STA_QOSINFO_AC_VI,
    [SSV_HWQ_BE] = IEEE80211_WMM_IE_STA_QOSINFO_AC_BE,
    [SSV_HWQ_BK] = IEEE80211_WMM_IE_STA_QOSINFO_AC_BK,
};

#if defined(CONFIG_WPA_SUPPLICANT_CTL)
struct ssv_softc *g_ssv_sc = NULL;
struct ssv_vif *g_ssv_vif = NULL;
#endif

#ifdef CONFIG_MIFI_LOWPOWER
struct ssv_mgmt_tx_params fake_mgmt_tx_params = {0};
struct probe_resp_info probe_resp_info;
#endif

static bool ssv_port_control(u32 cipher_group)
{
    if ((0 == cipher_group) ||
        (WLAN_CIPHER_SUITE_WEP40 == cipher_group) ||
        (WLAN_CIPHER_SUITE_WEP104 == cipher_group)) 
    {
        return false;
    } else {
        return true;
    }
}

u8 *_ssv_build_bcn(struct ssv_bcn *bcn, struct cfg80211_beacon_data *new)
{
    u8 *buf, *pos;

    if (new->head) {
        u8 *head = kmalloc(new->head_len, GFP_KERNEL);

        if (!head)
            return NULL;

        if (bcn->head)
            kfree(bcn->head);

        bcn->head = head;
        bcn->head_len = new->head_len;
        memcpy(bcn->head, new->head, new->head_len);
    }
    if (new->tail) {
        u8 *tail = kmalloc(new->tail_len, GFP_KERNEL);

        if (!tail)
            return NULL;

        if (bcn->tail)
            kfree(bcn->tail);

        bcn->tail = tail;
        bcn->tail_len = new->tail_len;
        memcpy(bcn->tail, new->tail, new->tail_len);
    }

    if (!bcn->head)
        return NULL;

    bcn->tim_len = 6;
    bcn->len = bcn->head_len + bcn->tail_len + bcn->ies_len + bcn->tim_len;

    buf = kmalloc(bcn->len, GFP_KERNEL);
    if (!buf)
        return NULL;

    // Build the beacon buffer
    pos = buf;
    memcpy(pos, bcn->head, bcn->head_len);
    pos += bcn->head_len;
    *pos++ = WLAN_EID_TIM;
    *pos++ = 4;
    *pos++ = 0;
    *pos++ = bcn->dtim;
    *pos++ = 0;
    *pos++ = 0;
    if (bcn->tail) {
        memcpy(pos, bcn->tail, bcn->tail_len);
        pos += bcn->tail_len;
    }
    if (bcn->ies) {
        memcpy(pos, bcn->ies, bcn->ies_len);
    }

    return buf;
}

static void ssv_del_bcn(struct ssv_bcn *bcn)
{
    if (bcn->head)
    {
        kfree(bcn->head);
        bcn->head = NULL;
    }
    bcn->head_len = 0;

    if (bcn->tail)
    {
        kfree(bcn->tail);
        bcn->tail = NULL;
    }
    bcn->tail_len = 0;

    if (bcn->ies)
    {
        kfree(bcn->ies);
        bcn->ies = NULL;
    }
    bcn->ies_len = 0;
    bcn->tim_len = 0;
    bcn->dtim = 0;
    bcn->len = 0;
}

static void ssv_del_csa(struct ssv_vif *vif)
{
    struct ssv_csa *csa = vif->ap.csa;

    if (!csa)
        return;

#if 0
    if (csa->dma.buf)
    {
        kfree(csa->dma.buf);
    }
#endif
    ssv_del_bcn(&csa->bcn);
    kfree(csa);
    vif->ap.csa = NULL;
}

#if 0
static void ssv_csa_finish(struct work_struct *ws)
{
    struct ssv_csa *csa = container_of(ws, struct ssv_csa, work);
    struct ssv_vif *vif = csa->vif;
    struct ssv_softc *sc = vif->sc;
    int error = csa->status;

    if (!error)
        error = ssv_send_bcn_change(sc, vif->drv_vif_index, csa->dma.buf,
                                    csa->bcn.len, csa->bcn.head_len,
                                    csa->bcn.tim_len, NULL);

    if (error)
        cfg80211_stop_iface(sc->wiphy, &vif->wdev, GFP_KERNEL);
    else
    {
        ssv_chanctx_unlink(vif);
        ssv_chanctx_link(vif, csa->ch_idx, &csa->chandef);
        cfg80211_ch_switch_notify(vif->ndev, &csa->chandef);
    }
    ssv_del_csa(vif);
}
#endif

/**
 * @add_virtual_intf: create a new virtual interface with the given name,
 *	must set the struct wireless_dev's iftype. Beware: You must create
 *	the new netdev in the wiphy's network namespace! Returns the struct
 *	wireless_dev, or an ERR_PTR. For P2P device wdevs, the driver must
 *	also set the address member in the wdev.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0)
static struct wireless_dev* ssv_cfg80211_add_iface(struct wiphy *wiphy,
                                const char *name, unsigned char name_assign_type,
                                enum nl80211_iftype type, struct vif_params *params)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
static struct wireless_dev* ssv_cfg80211_add_iface(struct wiphy *wiphy,
                                const char *name, unsigned char name_assign_type,
                                enum nl80211_iftype type, u32 *flags,
                                struct vif_params *params)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
static struct wireless_dev* ssv_cfg80211_add_iface(struct wiphy *wiphy,
                                const char *name, enum nl80211_iftype type,
                                u32 *flags, struct vif_params *params)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static struct wireless_dev* ssv_cfg80211_add_iface(struct wiphy *wiphy,
                                char *name, enum nl80211_iftype type,
                                u32 *flags, struct vif_params *params)
#else
static struct net_device* ssv_cfg80211_add_iface(struct wiphy *wiphy,
                                char *name, enum nl80211_iftype type,
                                u32 *flags, struct vif_params *params)
#endif
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct wireless_dev *wdev = NULL;
    struct ssv_vif *ssv_vif = NULL;
    bool use_monitor = false;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 12, 0)
    if(!flags){
		use_monitor = false;
    }
    else{
		use_monitor = (*flags & MONITOR_FLAG_COOK_FRAMES) ? true : false;
    }
#else
    use_monitor = (params->flags & MONITOR_FLAG_COOK_FRAMES) ? true : false;
#endif
    
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    wdev = ssv_interface_add(sc, name, type, params);
    if (wdev) {
        ssv_vif = container_of(wdev, struct ssv_vif, wdev);
        ssv_vif->use_monitor = use_monitor;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    return wdev;
#else
    return wdev->netdev;
#endif
}

/**
 * @del_virtual_intf: remove the virtual interface
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static int ssv_cfg80211_del_iface(struct wiphy *wiphy, struct wireless_dev *wdev)
#else
static int ssv_cfg80211_del_iface(struct wiphy *wiphy, struct net_device *dev)
#endif
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    struct net_device *dev = wdev->netdev;
#endif
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_softc *sc = wiphy_priv(wiphy);

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    netdev_info(dev, "Remove Interface");

    if (dev->reg_state == NETREG_REGISTERED) {
        /* Will call ssv_close if interface is UP */
        unregister_netdevice(dev);
    }
    
#ifdef FMAC_BRIDGE
        br0_detach(ssv_vif);
#endif

    mutex_lock(&sc->cb_lock);
    list_del(&ssv_vif->list);
    mutex_unlock(&sc->cb_lock);
    sc->avail_idx_map |= BIT(ssv_vif->drv_vif_index);
    ssv_vif->ndev = NULL;

    /* Clear the priv in adapter */
    dev->ieee80211_ptr = NULL;

    return 0;
}

/**
 * @change_virtual_intf: change type/configuration of virtual interface,
 *	keep the struct wireless_dev's iftype updated.
 */
static int ssv_cfg80211_change_iface(struct wiphy *wiphy,
                   struct net_device *dev,
                   enum nl80211_iftype type,
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 12, 0)
		   u32 *flags,
#endif
                   struct vif_params *params)
{
    struct ssv_vif *vif = netdev_priv(dev);
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if (vif->up)
        return (-EBUSY);

    dev->type = ARPHRD_ETHER;
    switch (type)
    {
    case NL80211_IFTYPE_STATION:
    case NL80211_IFTYPE_P2P_CLIENT:
        vif->sta.ap = NULL;
        vif->sta.tdls_sta = NULL;
        break;
    case NL80211_IFTYPE_MESH_POINT:
        INIT_LIST_HEAD(&vif->ap.mpath_list);
        INIT_LIST_HEAD(&vif->ap.proxy_list);
        vif->ap.create_path = false;
        vif->ap.generation = 0;
	INIT_LIST_HEAD(&vif->ap.sta_list);
	memset(&vif->ap.bcn, 0, sizeof(vif->ap.bcn));
	break;
    case NL80211_IFTYPE_AP:
    case NL80211_IFTYPE_P2P_GO:
        INIT_LIST_HEAD(&vif->ap.sta_list);
        memset(&vif->ap.bcn, 0, sizeof(vif->ap.bcn));
        break;
    case NL80211_IFTYPE_AP_VLAN:
        return -EPERM;
    case NL80211_IFTYPE_MONITOR:
    {
        dev->type = ARPHRD_IEEE80211_RADIOTAP;
        // dev->type = ARPHRD_IEEE80211;
        break;
    }
    default:
        break;
    }

    vif->wdev.iftype = type;
    if (params->use_4addr != -1)
        vif->use_4addr = params->use_4addr;

    return 0;
}

/**
 * @scan: Request to do a scan. If returning zero, the scan request is given
 *	the driver, and will be valid until passed to cfg80211_scan_done().
 *	For scan results, call cfg80211_inform_bss(); you can call this outside
 *	the scan/scan_done bracket too.
 */
static int ssv_cfg80211_scan(struct wiphy *wiphy,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
                                struct net_device *dev,
#endif
                                struct cfg80211_scan_request *request)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    struct ssv_vif *ssv_vif = container_of(request->wdev, struct ssv_vif,
                                             wdev);
#else
    struct ssv_vif *ssv_vif = netdev_priv(dev);
#endif
    int ret = 0;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if(ssv_get_engmode())
    {
        SSV_LOG_DBG("Engineer Mode ignore ssv_cfg80211_scan!\n");
        goto END;
    }
    
    if (0 != (ret = ssv_send_scanu_req(sc, ssv_vif, request)))
    {
        goto END;
    }

    sc->scan_request = request;

END:
    return ret;
}

/**
 * @add_key: add a key with the given parameters. @mac_addr will be %NULL
 *	when adding a group key.
 */
static int ssv_cfg80211_add_key(struct wiphy *wiphy, struct net_device *netdev,
                                 u8 key_index, bool pairwise, const u8 *mac_addr,
                                 struct key_params *params)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *vif = netdev_priv(netdev);
    int i, error = 0;
    struct mm_key_add_cfm key_add_cfm;
    u8_l cipher = 0;
    struct ssv_sta *sta = NULL;
    struct ssv_key *ssv_key;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if (mac_addr) {
        sta = ssv_get_sta(sc, mac_addr);
        if (!sta)
            return -EINVAL;
        ssv_key = &sta->key;
    }
    else
        ssv_key = &vif->key[key_index];

    /* Retrieve the cipher suite selector */
    switch (params->cipher) {
    case WLAN_CIPHER_SUITE_WEP40:
        cipher = MAC_RSNIE_CIPHER_WEP40;
        break;
    case WLAN_CIPHER_SUITE_WEP104:
        cipher = MAC_RSNIE_CIPHER_WEP104;
        break;
    case WLAN_CIPHER_SUITE_TKIP:
        cipher = MAC_RSNIE_CIPHER_TKIP;
        break;
    case WLAN_CIPHER_SUITE_CCMP:
        cipher = MAC_RSNIE_CIPHER_CCMP;
        break;
    case WLAN_CIPHER_SUITE_AES_CMAC:
        cipher = MAC_RSNIE_CIPHER_AES_CMAC;
        break;
    case WLAN_CIPHER_SUITE_SMS4:
    {
        // Need to reverse key order
        u8 tmp, *key = (u8 *)params->key;
        cipher = MAC_RSNIE_CIPHER_SMS4;
        for (i = 0; i < WPI_SUBKEY_LEN/2; i++) {
            tmp = key[i];
            key[i] = key[WPI_SUBKEY_LEN - 1 - i];
            key[WPI_SUBKEY_LEN - 1 - i] = tmp;
        }
        for (i = 0; i < WPI_SUBKEY_LEN/2; i++) {
            tmp = key[i + WPI_SUBKEY_LEN];
            key[i + WPI_SUBKEY_LEN] = key[WPI_KEY_LEN - 1 - i];
            key[WPI_KEY_LEN - 1 - i] = tmp;
        }
        break;
    }
    default:
        return -EINVAL;
    }

	//Fixme: comfirm eapol data packet send before ssv_send_key_add?
	msleep(3);

    if ((error = ssv_send_key_add(sc, vif->drv_vif_index,
                                   (sta ? sta->sta_idx : 0xFF), pairwise,
                                   (u8 *)params->key, params->key_len,
                                   key_index, cipher, &key_add_cfm)))
        return error;

    if (key_add_cfm.status != 0) {
        SSV_LOG_DBG(KERN_CRIT "[%s]: Status Error(%d)\n", __FUNCTION__, key_add_cfm.status);
        return -EIO;
    }

    /* Save the index retrieved from LMAC */
    ssv_key->hw_idx = key_add_cfm.hw_key_idx;
    if(NULL!=sta){
        sta->port_control=false;
    }
    return 0;
}

/**
 * @get_key: get information about the key with the given parameters.
 *	@mac_addr will be %NULL when requesting information for a group
 *	key. All pointers given to the @callback function need not be valid
 *	after it returns. This function should return an error if it is
 *	not possible to retrieve the key, -ENOENT if it doesn't exist.
 *
 */
static int ssv_cfg80211_get_key(struct wiphy *wiphy, struct net_device *netdev,
                                 u8 key_index, bool pairwise, const u8 *mac_addr,
                                 void *cookie,
                                 void (*callback)(void *cookie, struct key_params*))
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    return -1;
}


/**
 * @del_key: remove a key given the @mac_addr (%NULL for a group key)
 *	and @key_index, return -ENOENT if the key doesn't exist.
 */
static int ssv_cfg80211_del_key(struct wiphy *wiphy, struct net_device *netdev,
                                 u8 key_index, bool pairwise, const u8 *mac_addr)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *vif = netdev_priv(netdev);
    int error;
    struct ssv_sta *sta = NULL;
    struct ssv_key *ssv_key;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    if (mac_addr) {
        sta = ssv_get_sta(sc, mac_addr);
        if (!sta)
            return -EINVAL;
        ssv_key = &sta->key;
    }
    else
        ssv_key = &vif->key[key_index];

    error = ssv_send_key_del(sc, ssv_key->hw_idx);
    return error;
}

/**
 * @set_default_key: set the default key on an interface
 */
static int ssv_cfg80211_set_default_key(struct wiphy *wiphy,
                                         struct net_device *netdev,
                                         u8 key_index, bool unicast, bool multicast)
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    return 0;
}

/**
 * @set_default_mgmt_key: set the default management frame key on an interface
 */
static int ssv_cfg80211_set_default_mgmt_key(struct wiphy *wiphy,
                                              struct net_device *netdev,
                                              u8 key_index)
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    return 0;
}

/**
 * @connect: Connect to the ESS with the specified parameters. When connected,
 *	call cfg80211_connect_result() with status code %WLAN_STATUS_SUCCESS.
 *	If the connection fails for some reason, call cfg80211_connect_result()
 *	with the status from the AP.
 *	(invoked with the wireless_dev mutex held)
 */
static int ssv_cfg80211_connect(struct wiphy *wiphy, struct net_device *dev,
                                 struct cfg80211_connect_params *sme)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct sm_connect_cfm sm_connect_cfm;
    int error = 0;

RETRY_CONNECT:
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    
    ssv_vif->need_port_control = ssv_port_control(sme->crypto.cipher_group);
    SSV_LOG_DBG("need port control=%d\r\n",ssv_vif->need_port_control);
#if defined(CONFIG_WPA_SUPPLICANT_CTL)
    if (sme->crypto.n_akm_suites != 0 && 
        sme->crypto.akm_suites[0] == 0x000FAC08 && 
        sme->auth_type == NL80211_AUTHTYPE_OPEN_SYSTEM)
    {
        SSV_LOG_DBG("%s: the actual auth type is SAE!!!\n", __func__);
        sme->auth_type = SSV_NL80211_AUTHTYPE_SAE;
        g_ssv_sc = sc;
        g_ssv_vif = ssv_vif;
    }
#endif
    /* For SHARED-KEY authentication, must install key first */
    if (sme->auth_type == NL80211_AUTHTYPE_SHARED_KEY && sme->key)
    {
        struct key_params key_params;
        key_params.key = (u8 *)sme->key;
        key_params.seq = NULL;
        key_params.key_len = sme->key_len;
        key_params.seq_len = 0;
        key_params.cipher = sme->crypto.cipher_group;
        ssv_cfg80211_add_key(wiphy, dev, sme->key_idx, false, NULL, &key_params);
    }
    //SSV_LOG_DBG("auth_type %d\n",sme->auth_type);
    //SSV_LOG_DBG("ciphers_pairwise 0x%x\n",sme->crypto.ciphers_pairwise[0]);

    
    /* Forward the information to the LMAC */
    if ((error = ssv_send_sm_connect_req(sc, ssv_vif, sme, &sm_connect_cfm)))
        return error;

    // Check the status
    switch (sm_connect_cfm.status)
    {
        case CO_OK:
            error = 0;
            break;
        case CO_BUSY:
            error = -EINPROGRESS;
            break;
        case CO_OP_IN_PROGRESS:
            error = -EALREADY;
            break;
        default:
            error = -EIO;
            break;
    }

#ifdef CONFIG_SSV_CHANNEL_FOLLOW
    if(NL80211_IFTYPE_STATION == SSV_VIF_TYPE(ssv_vif) && sme->channel) {
        ssv_vif->ch_hint = sme->channel->center_freq;
        ssv_vif->chtype_hint = NL80211_CHAN_HT20; //default为HT20
        
        if(sme->channel->flags & IEEE80211_CHAN_NO_HT40) {
            ssv_vif->chtype_hint = NL80211_CHAN_HT20;               
        } else {
            if(sme->channel->flags & IEEE80211_CHAN_NO_HT40PLUS) {
                ssv_vif->chtype_hint = NL80211_CHAN_HT40MINUS;
            }
            if(sme->channel->flags & IEEE80211_CHAN_NO_HT40MINUS) {
                ssv_vif->chtype_hint = NL80211_CHAN_HT40PLUS;
            }
        }        
    } 
#endif /*CONFIG_SSV_CHANNEL_FOLLOW */


    sc->sta_reconnect_info.auth_type = sme->auth_type;
   
    if(ssv_cfg.sta_max_reconnect_times)
    {
         //Retry_Connection doesn't support WPA3
        if(sme->auth_type != NL80211_AUTHTYPE_SAE)
        {
            down(&sc->reconnect_sem);

            /*If connection failed, retry again */
            if(sc->sta_reconnect_info.connect_retry)
            {
                if (sme->crypto.n_ciphers_pairwise &&
                ((sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_WEP40) ||
                (sme->crypto.ciphers_pairwise[0] == WLAN_CIPHER_SUITE_WEP104)))
                {
                    if(sme->auth_type == NL80211_AUTHTYPE_OPEN_SYSTEM)
                        sme->auth_type = NL80211_AUTHTYPE_SHARED_KEY;
                    else if(sme->auth_type == NL80211_AUTHTYPE_SHARED_KEY)
                        sme->auth_type = NL80211_AUTHTYPE_OPEN_SYSTEM;
                }
                SSV_LOG_DBG("goto Retry\n");
                goto RETRY_CONNECT;
            }
        }
    }

    SSV_LOG_DBG("connect end %d\n", error);
    return error;
}

/**
 * @disconnect: Disconnect from the BSS/ESS.
 *	(invoked with the wireless_dev mutex held)
 */
static int ssv_cfg80211_disconnect(struct wiphy *wiphy, struct net_device *dev,
                                    u16 reason_code)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);

    SSV_LOG_DBG("[%s] reason_code %d\n", __FUNCTION__, reason_code);
    return(ssv_send_sm_disconnect_req(sc, ssv_vif, reason_code));
}
/**
 * @add_station: Add a new station.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
static int ssv_cfg80211_add_station(struct wiphy *wiphy, struct net_device *dev,
                                     const u8 *mac, struct station_parameters *params)

#else
static int ssv_cfg80211_add_station(struct wiphy *wiphy, struct net_device *dev,
                                     u8 *mac, struct station_parameters *params)
#endif
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct me_sta_add_cfm me_sta_add_cfm;
    int error = 0;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    WARN_ON(SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_AP_VLAN);

    /* Do not add TDLS station */
    if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
        return 0;

    /* Indicate we are in a STA addition process - This will allow handling
     * potential PS mode change indications correctly
     */
    sc->adding_sta = true;

    /* Forward the information to the LMAC */
    if ((error = ssv_send_me_sta_add(sc, params, mac, ssv_vif->drv_vif_index,
                                      &me_sta_add_cfm)))
        return error;

    // Check the status
    switch (me_sta_add_cfm.status)
    {
        case CO_OK:
        {
            struct ssv_sta *sta = &sc->sta_table[me_sta_add_cfm.sta_idx];
            int tid;
            sta->aid = params->aid;

            sta->sta_idx = me_sta_add_cfm.sta_idx;
            ssv_push_private_msg_to_host(sc, (u32)E_HOST_PRIV_MSG_TYPE_RX_REORD_CREATE, (u8 *)&sta->sta_idx, (u32)sizeof(u8));
            sta->ch_idx = ssv_vif->ch_index;
            sta->vif_idx = ssv_vif->drv_vif_index;
            sta->vlan_idx = sta->vif_idx;
            sta->qos = (params->sta_flags_set & BIT(NL80211_STA_FLAG_WME)) != 0;
            sta->ht = params->ht_capa ? 1 : 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
            sta->vht = params->vht_capa ? 1 : 0;
#endif
            sta->acm = 0;
            sta->probe_timestamp = 0;
            sta->last_rx = jiffies_to_msecs(jiffies);

            for (tid = 0; tid < NX_NB_TXQ_PER_STA; tid++) {
                int uapsd_bit = ssv_hwq2uapsd[ssv_tid2hwq[tid]];
                if (params->uapsd_queues & uapsd_bit)
                    sta->uapsd_tids |= 1 << tid;
                else
                    sta->uapsd_tids &= ~(1 << tid);
            }
            memcpy(sta->mac_addr, mac, ETH_ALEN);
            // ssv_dbgfs_register_rc_stat(sc, sta);

            /* Ensure that we won't process PS change or channel switch ind*/
            mutex_lock(&sc->cb_lock);
            ssv_drv_hci_tx_active_by_sta(sc->hci_priv, sc->hci_ops, sta->sta_idx);    //active hci mng. queue
            ssv_drv_hci_tx_resume_by_sta(sc->hci_priv, sc->hci_ops, sta->sta_idx);    //resume hci mng. queue
            list_add_tail(&sta->list, &ssv_vif->ap.sta_list);
            sta->valid = true;
            ssv_ps_bh_enable(sc, sta, sta->ps.active || me_sta_add_cfm.pm_state);
            sta->port_control=(true==ssv_vif->need_port_control)?true:false;
            mutex_unlock(&sc->cb_lock);

            error = 0;

#ifdef CONFIG_SSV_BFMER
#endif /* CONFIG_SSV_BFMER */

            #define PRINT_STA_FLAG(f)                               \
                (params->sta_flags_set & BIT(NL80211_STA_FLAG_##f) ? "["#f"]" : "")

            netdev_info(dev, "Add sta %d (%pM) flags=%s%s%s%s%s%s"
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                        "%s"
#endif
                        ,
                        sta->sta_idx, mac,
                        PRINT_STA_FLAG(AUTHORIZED),
                        PRINT_STA_FLAG(SHORT_PREAMBLE),
                        PRINT_STA_FLAG(WME),
                        PRINT_STA_FLAG(MFP),
                        PRINT_STA_FLAG(AUTHENTICATED),
                        PRINT_STA_FLAG(TDLS_PEER)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                        ,
                        PRINT_STA_FLAG(ASSOCIATED)
#endif
                        );
            #undef PRINT_STA_FLAG
            break;
        }
        default:
            error = -EBUSY;
            break;
    }

    sc->adding_sta = false;

    return error;
    return 0;
}

/**
 * @del_station: Remove a station
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
static int ssv_cfg80211_del_station(struct wiphy *wiphy, struct net_device *dev,
                              struct station_del_parameters *params)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
static int ssv_cfg80211_del_station(struct wiphy *wiphy, struct net_device *dev,
                              const u8 *mac)
#else
static int ssv_cfg80211_del_station(struct wiphy *wiphy, struct net_device *dev,
                              u8 *mac)
#endif
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_sta *cur, *tmp;
    int error = 0, found = 0;
    int j = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
    const u8 *mac = params->mac;
#endif

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    list_for_each_entry_safe(cur, tmp, &ssv_vif->ap.sta_list, list) {
        if ((!mac) || (!memcmp(cur->mac_addr, mac, ETH_ALEN))) {
            netdev_info(dev, "Del sta %d (%pM)", cur->sta_idx, cur->mac_addr);
            memset(&sc->rx_bysta[cur->sta_idx], 0 , sizeof(struct ssv_rx));
            memset(&sc->tx_bysta[cur->sta_idx], 0 , sizeof(struct ssv_tx));
            /* Ensure that we won't process PS change ind */
            mutex_lock(&sc->cb_lock);
            cur->ps.active = false;
            cur->valid = false;
            cur->port_control = true;
            mutex_unlock(&sc->cb_lock);

            if (cur->vif_idx != cur->vlan_idx) {
                struct ssv_vif *vlan_vif;
                vlan_vif = sc->vif_table[cur->vlan_idx];
                if (vlan_vif->up) {
                    if ((SSV_VIF_TYPE(vlan_vif) == NL80211_IFTYPE_AP_VLAN) &&
                        (vlan_vif->use_4addr)) {
                        vlan_vif->ap_vlan.sta_4a = NULL;
                    } else {
                        WARN(1, "Deleting sta belonging to VLAN other than AP_VLAN 4A");
                    }
                }
            }
            ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, cur->sta_idx);     //pause hci tx queue by sta
            ssv_drv_hci_tx_inactive_by_sta(sc->hci_priv, sc->hci_ops, cur->sta_idx);  //inactive hci tx queue by sta
            ssv_push_private_msg_to_host(sc, (u32)E_HOST_PRIV_MSG_TYPE_RX_REORD_DEL, (u8 *)&cur->sta_idx, (u32)sizeof(u8));
            //Reset station last sequence
            for(j = 0; j < TID_MAX ; j++){
                cur->rx_last_seqcntl[j] = 0xFFFF;
            }
            error = ssv_send_me_sta_del(sc, cur->sta_idx, false);
            if ((error != 0) && (error != -EPIPE))
                return error;

            list_del(&cur->list);
            // ssv_dbgfs_unregister_rc_stat(sc, cur);
            found ++;
            break;
        }
    }

    if (found) {
        SSV_LOG_DBG("Remove old station %d\n", found);
    }

    return 0;
}


#ifdef CONFIG_MIFI_LOWPOWER
#define PB_RESP_TEMPLATE_TYPE_PATTER      0xFF50
#define PB_RESP_TEMPLATE_NO_HIDDEN_PATTER 0x1234
#define PB_RESP_TEMPLATE_HIDDEN_PATTER    0xFFFF

void ssv_gen_fake_mgmt_tx_params(void)
{
    fake_mgmt_tx_params.chan = NULL;
    fake_mgmt_tx_params.offchan = false;
    fake_mgmt_tx_params.wait = 0;
    fake_mgmt_tx_params.buf = probe_resp_info.data;
    fake_mgmt_tx_params.len = probe_resp_info.data_len;
    fake_mgmt_tx_params.no_cck = false;
    fake_mgmt_tx_params.dont_wait_for_ack = true;
    fake_mgmt_tx_params.n_csa_offsets = 0;
    fake_mgmt_tx_params.csa_offsets = NULL;
}

void ssv_gen_probe_resp_template(struct cfg80211_ap_settings *settings, struct ssv_bcn *bcn, uint8_t *bcn_buf)
{
    uint8_t *probe_resp;
    uint8_t probe_resp_len;
    uint8_t *pdata;
    uint8_t *p_skip_tim;
    struct mac_hdr *mac_hdr;
    uint16_t tim_oft;
    uint8_t ssid_oft;
    // struct ieee80211_mgmt *resp;
    // uint8_t *pos;
    SSV_LOG_DBG("ssv_gen_probe_resp\n"); 

    if(settings->hidden_ssid == NL80211_HIDDEN_SSID_NOT_IN_USE)
        probe_resp_len = bcn->len - bcn->tim_len;
    else
        probe_resp_len = bcn->len - bcn->tim_len + settings->ssid_len;
    // SSV_LOG_DBG("original len %ld\n",(unsigned long)bcn->len - bcn->tim_len);
    // SSV_LOG_DBG("probe_resp_len %d\n", probe_resp_len); 
    // SSV_LOG_DBG_DUMP("becon", bcn_buf, bcn->len);

    probe_resp = kzalloc(probe_resp_len, GFP_KERNEL);
    if(NULL == probe_resp)
    {
        SSV_LOG_DBG("[%s][%d] kzalloc() for probe_resp failed!!\n", __FUNCTION__, __LINE__);
        return;
    }

    tim_oft = bcn->head_len;

    pdata = probe_resp;
    if(settings->hidden_ssid == NL80211_HIDDEN_SSID_NOT_IN_USE)
    {
        //copy mac_header~before TIM
        memcpy(pdata, (uint8_t *)bcn_buf, tim_oft);
        pdata = pdata + tim_oft;
    }
    else
    {
        //check kernel 3.4.110是否成立
        ssid_oft = sizeof(struct mac_hdr) + 12; //bcn timestamp+intval+capinfo = 12
        memcpy(pdata, (uint8_t *)bcn_buf, ssid_oft);
        pdata = pdata + ssid_oft;
        *pdata++ = WLAN_EID_SSID;
        *pdata++ = settings->ssid_len;
        memcpy(pdata, settings->ssid, settings->ssid_len);
        pdata = pdata + settings->ssid_len; 
        memcpy(pdata, (uint8_t *)bcn_buf+ssid_oft+2, tim_oft-ssid_oft-2); //ssid element id+length = 2
        pdata = pdata + (tim_oft - ssid_oft - 2) ;
    }

    //skip TIM
    p_skip_tim = (uint8_t *)bcn_buf + tim_oft + bcn->tim_len;

    //copy after TIM ~ End
    memcpy(pdata, p_skip_tim, bcn->len - (tim_oft + bcn->tim_len));   

    //update fctl of mac header
    mac_hdr = (struct mac_hdr *)probe_resp;
    mac_hdr->fctl = PB_RESP_TEMPLATE_TYPE_PATTER;  //MAC_FCTRL_PROBERSP_ST
    if(settings->hidden_ssid == NL80211_HIDDEN_SSID_NOT_IN_USE)
        mac_hdr->durid = PB_RESP_TEMPLATE_NO_HIDDEN_PATTER;
    else
        mac_hdr->durid = PB_RESP_TEMPLATE_HIDDEN_PATTER; 
    

    //SSV_LOG_DBG_DUMP("g_probe_resp", probe_resp, probe_resp_len);

    //add tail to check send out...
    // *(probe_resp + bcn->len-bcn->tim_len) = 0xAA;
    // *(probe_resp + bcn->len-bcn->tim_len+1) = 0xBB;
    // *(probe_resp + bcn->len-bcn->tim_len+2) = 0xCC;
    // *(probe_resp + bcn->len-bcn->tim_len+3) = 0xDD;
    SSV_LOG_DBG_DUMP("update ssdi g_probe_resp", probe_resp, probe_resp_len);

    probe_resp_info.data_len = probe_resp_len;
    probe_resp_info.data = probe_resp;
}
#endif

/**
 * @change_station: Modify a given station. Note that flags changes are not much
 *	validated in cfg80211, in particular the auth/assoc/authorized flags
 *	might come to the driver in invalid combinations -- make sure to check
 *	them, also against the existing state! Drivers must call
 *	cfg80211_check_station_change() to validate the information.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
static int ssv_cfg80211_change_station(struct wiphy *wiphy, struct net_device *dev,
                                        const u8 *mac, struct station_parameters *params)
#else
static int ssv_cfg80211_change_station(struct wiphy *wiphy, struct net_device *dev,
                                        u8 *mac, struct station_parameters *params)
#endif
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *vif = netdev_priv(dev);
    struct ssv_sta *sta;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    sta = ssv_get_sta(sc, mac);
    if (!sta)
    {
        /* Add the TDLS station */
        if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
        {
            struct ssv_vif *ssv_vif = netdev_priv(dev);
            struct me_sta_add_cfm me_sta_add_cfm;
            int error = 0;

            /* Indicate we are in a STA addition process - This will allow handling
             * potential PS mode change indications correctly
             */
            sc->adding_sta = true;

            /* Forward the information to the LMAC */
            if ((error = ssv_send_me_sta_add(sc, params, mac, ssv_vif->drv_vif_index,
                                              &me_sta_add_cfm)))
                return error;

            // Check the status
            switch (me_sta_add_cfm.status)
            {
                case CO_OK:
                {
                    int tid;
                    sta = &sc->sta_table[me_sta_add_cfm.sta_idx];
                    sta->aid = params->aid;
                    sta->sta_idx = me_sta_add_cfm.sta_idx;
                    sta->ch_idx = ssv_vif->ch_index;
                    sta->vif_idx = ssv_vif->drv_vif_index;
                    sta->vlan_idx = sta->vif_idx;
                    sta->qos = (params->sta_flags_set & BIT(NL80211_STA_FLAG_WME)) != 0;
                    sta->ht = params->ht_capa ? 1 : 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
                    sta->vht = params->vht_capa ? 1 : 0;
#endif
                    sta->acm = 0;
                    for (tid = 0; tid < NX_NB_TXQ_PER_STA; tid++) {
                        int uapsd_bit = ssv_hwq2uapsd[ssv_tid2hwq[tid]];
                        if (params->uapsd_queues & uapsd_bit)
                            sta->uapsd_tids |= 1 << tid;
                        else
                            sta->uapsd_tids &= ~(1 << tid);
                    }
                    memcpy(sta->mac_addr, mac, ETH_ALEN);
                    // ssv_dbgfs_register_rc_stat(sc, sta);

                    /* Ensure that we won't process PS change or channel switch ind*/
                    mutex_lock(&sc->cb_lock);
                    sta->valid = true;
                    mutex_unlock(&sc->cb_lock);

                    #define PRINT_STA_FLAG(f)                               \
                        (params->sta_flags_set & BIT(NL80211_STA_FLAG_##f) ? "["#f"]" : "")

                    netdev_info(dev, "Add TDLS sta %d (%pM) flags=%s%s%s%s%s%s"
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                                "%s"
#endif
                                ,
                                sta->sta_idx, mac,
                                PRINT_STA_FLAG(AUTHORIZED),
                                PRINT_STA_FLAG(SHORT_PREAMBLE),
                                PRINT_STA_FLAG(WME),
                                PRINT_STA_FLAG(MFP),
                                PRINT_STA_FLAG(AUTHENTICATED),
                                PRINT_STA_FLAG(TDLS_PEER)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                                ,
                                PRINT_STA_FLAG(ASSOCIATED)
#endif
                                );
                    #undef PRINT_STA_FLAG

                    break;
                }
                default:
                    error = -EBUSY;
                    break;
            }

            sc->adding_sta = false;
        } else  {
            return -EINVAL;
        }
    }

    if (params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED))
        ssv_send_me_set_control_port_req(sc,
                (params->sta_flags_set & BIT(NL80211_STA_FLAG_AUTHORIZED)) != 0,
                sta->sta_idx);

    if (params->vlan) {
        uint8_t vlan_idx;

        vif = netdev_priv(params->vlan);
        vlan_idx = vif->drv_vif_index;

        if (sta->vlan_idx != vlan_idx) {
            struct ssv_vif *old_vif;
            old_vif = sc->vif_table[sta->vlan_idx];
            sta->vlan_idx = vlan_idx;

            if ((SSV_VIF_TYPE(vif) == NL80211_IFTYPE_AP_VLAN) &&
                (vif->use_4addr)) {
                WARN((vif->ap_vlan.sta_4a),
                     "4A AP_VLAN interface with more than one sta");
                vif->ap_vlan.sta_4a = sta;
            }

            if ((SSV_VIF_TYPE(old_vif) == NL80211_IFTYPE_AP_VLAN) &&
                (old_vif->use_4addr)) {
                old_vif->ap_vlan.sta_4a = NULL;
            }
        }
    }

    return 0;
}

#ifdef CONFIG_SSV_CHANNEL_FOLLOW
static void ssv_softap_follow_sta_channel(struct ssv_softc *sc, struct cfg80211_chan_def *chandef)
{
    u16 freq = chandef->chan->center_freq;
    struct ssv_vif *sta_vif = NULL, *ap_vif = NULL, *ssv_vif = NULL;
    struct ssv_sta *sta_mode = NULL;

    // Look for VIF entry check sta mode exist or not
    list_for_each_entry(ssv_vif, &sc->vifs, list) {
    if (ssv_vif->up) {
        if (NL80211_IFTYPE_STATION == SSV_VIF_TYPE(ssv_vif))
            sta_vif = ssv_vif;         
        }
        if (NL80211_IFTYPE_AP == SSV_VIF_TYPE(ssv_vif))
            ap_vif = ssv_vif; 
    }

    if ((NULL == sta_vif) || (NULL == ap_vif)) {
        SSV_LOG_DBG("Not concurrency mode\n");
        return;
    }

    if (sta_vif) {
        sta_mode = sta_vif->sta.ap;
    }
    if (sta_mode && freq != sta_mode->center_freq) {

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
        chandef->chan = ieee80211_get_channel(sta_vif->wdev.wiphy, sta_mode->center_freq);
#endif
        chandef->chan->band = sta_mode->band; 
        chandef->chan->center_freq = sta_mode->center_freq;                               
        chandef->center_freq1 = sta_mode->center_freq1;       
        chandef->center_freq2 = sta_mode->center_freq2;       
        chandef->width = sta_mode->width;
        cfg80211_ch_switch_notify(ap_vif->ndev, chandef);
        SSV_LOG_DBG("SSV notify softap channel switch\n");  
    }
} 
#endif /*CONFIG_SSV_CHANNEL_FOLLOW */

/**
 * @start_ap: Start acting in AP mode defined by the parameters.
 */
static int ssv_cfg80211_start_ap(struct wiphy *wiphy, struct net_device *dev,
                                  struct cfg80211_ap_settings *settings)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct apm_start_cfm apm_start_cfm;
    struct ssv_sta *sta;
    int error = 0;
    u8 *buf = NULL;
#ifdef CONFIG_MIFI_LOWPOWER
    u64 cookie = 0;
#endif

#ifdef CONFIG_SSV_CHANNEL_FOLLOW
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
    struct cfg80211_chan_def chandef;
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    chandef.chan = &ssv_vif->chan_setting;
    switch (ssv_vif->chan_type_setting)
    {
        case NL80211_CHAN_NO_HT:        chandef.width = NL80211_CHAN_WIDTH_20_NOHT; break;
        case NL80211_CHAN_HT20:         chandef.width = NL80211_CHAN_WIDTH_20;      break;
        case NL80211_CHAN_HT40MINUS:    chandef.width = NL80211_CHAN_WIDTH_40;      break;
        case NL80211_CHAN_HT40PLUS:     chandef.width = NL80211_CHAN_WIDTH_40;      break;
        default:                        chandef.width = NL80211_CHAN_WIDTH_20;      break;
    }
    ssv_softap_follow_sta_channel(sc, &chandef);
#else 
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    ssv_softap_follow_sta_channel(sc, &settings->chandef);
#endif    
#endif /*CONFIG_SSV_CHANNEL_FOLLOW */


    /* Forward the information to the LMAC */
    if ((error = ssv_send_apm_start_req(sc, ssv_vif, settings, &apm_start_cfm, &buf)))
    {
        goto err;
    }


    // Check the status
    switch (apm_start_cfm.status)
    {
    case CO_OK:
    {
        ssv_vif->ap.bcmc_index = apm_start_cfm.bcmc_idx;
        ssv_vif->ap.flags = 0;
        ssv_vif->need_port_control = ssv_port_control(settings->crypto.cipher_group);
        SSV_LOG_DBG("need port control=%d\r\n",ssv_vif->need_port_control);
        sta = &sc->sta_table[apm_start_cfm.bcmc_idx];
        sta->valid = true;
        sta->aid = 0;
        sta->sta_idx = apm_start_cfm.bcmc_idx;
        sta->ch_idx = apm_start_cfm.ch_idx;
        sta->vif_idx = ssv_vif->drv_vif_index;
        sta->qos = false;
        sta->acm = 0;
        sta->ps.active = false;
    
        mutex_lock(&sc->cb_lock);
        ssv_chanctx_link(ssv_vif, apm_start_cfm.ch_idx,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
                         &settings->chandef
#elif defined CONFIG_SSV_CHANNEL_FOLLOW
                        &chandef
#else
                         NULL
#endif
                        );
        mutex_unlock(&sc->cb_lock);
       
        netif_tx_start_all_queues(dev);
        netif_carrier_on(dev);
        { // turn on traffic for use_monitor interface
            struct ssv_vif *tmp_vif = NULL;
            
            list_for_each_entry(tmp_vif, &sc->vifs, list) {
                if (tmp_vif->use_monitor) {
                    netif_tx_start_all_queues(tmp_vif->ndev);
                    netif_carrier_on(tmp_vif->ndev);
                    break;
                }
            }
        }
        
        error = 0;
#ifdef CONFIG_MIFI_LOWPOWER
        //Send ProbeResp template to FW for MIFI lowpoer
        ssv_gen_probe_resp_template(settings, &ssv_vif->ap.bcn, buf);
        ssv_gen_fake_mgmt_tx_params();

        if((error = ssv_start_mgmt_xmit(ssv_vif, NULL, &fake_mgmt_tx_params, 0, &cookie)))
        {
            SSV_LOG_DBG("send probe_resp error\n");
        }

        if(fake_mgmt_tx_params.buf)
        {
            kfree(fake_mgmt_tx_params.buf);
            fake_mgmt_tx_params.buf = NULL;
        }
#endif

        break;
    }
    case CO_BUSY:
        error = -EINPROGRESS;
        break;
    case CO_OP_IN_PROGRESS:
        error = -EALREADY;
        break;
    default:
        error = -EIO;
        break;
    }

    if (error)
    {
        netdev_info(dev, "Failed to start AP (%d)", error);
    }
    else
    {
        netdev_info(dev, "AP started: ch=%d, bcmc_idx=%d",
                    ssv_vif->ch_index, ssv_vif->ap.bcmc_index);

           /* Retrieve the cipher suite selector */
        switch (settings->crypto.cipher_group) {
        case WLAN_CIPHER_SUITE_WEP40:
            SSV_LOG_DBG("WLAN_CIPHER_SUITE_WEP40\n");
            break;
        case WLAN_CIPHER_SUITE_WEP104:
            SSV_LOG_DBG("WLAN_CIPHER_SUITE_WEP104\n");
            break;
        case WLAN_CIPHER_SUITE_TKIP:
            SSV_LOG_DBG("WPA%d WLAN_CIPHER_SUITE_TKIP\n", settings->crypto.wpa_versions);
            break;
        case WLAN_CIPHER_SUITE_CCMP:
            SSV_LOG_DBG("WPA%d WLAN_CIPHER_SUITE_CCMP\n", settings->crypto.wpa_versions);
            break;
        case WLAN_CIPHER_SUITE_AES_CMAC:
            SSV_LOG_DBG("WLAN_CIPHER_SUITE_AES_CMAC\n");
            break;
        case WLAN_CIPHER_SUITE_SMS4:
            SSV_LOG_DBG("WLAN_CIPHER_SUITE_SMS4\n");
            break;
        default:
            SSV_LOG_DBG("%s\n", settings->crypto.cipher_group == 0 ? "Open System":"Unknow type");
        }
        
    }

err:
    // Free the buffer used to build the beacon
    if (buf)
        kfree(buf);

    return error;
}


/**
 * @change_beacon: Change the beacon parameters for an access point mode
 *	interface. This should reject the call when AP mode wasn't started.
 */
static int ssv_cfg80211_change_beacon(struct wiphy *wiphy, struct net_device *dev,
                                       struct cfg80211_beacon_data *info)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *vif = netdev_priv(dev);
    struct ssv_bcn *bcn = &vif->ap.bcn;
    u8 *buf;
    int error = 0;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    // Build the beacon
    buf = _ssv_build_bcn(bcn, info);
    if (!buf)
        return -ENOMEM;

    // Forward the information to the LMAC
    error = ssv_send_bcn_change(sc, vif->drv_vif_index, buf,
                                bcn->len, bcn->head_len, bcn->tim_len, NULL);

    kfree(buf);

    return error;
}

/**
 * * @stop_ap: Stop being an AP, including stopping beaconing.
 */
static int ssv_cfg80211_stop_ap(struct wiphy *wiphy, struct net_device *dev)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    struct ssv_sta *sta;
    int mgmt_txq;
    struct ssv_sta *cur, *tmp; 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
    struct station_del_parameters params;
    params.mac = NULL;
#endif

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    sc->is_stoping_apm=true;
    
    { // turn off traffic for use_monitor interface
        struct ssv_vif *tmp_vif = NULL;
        
        list_for_each_entry(tmp_vif, &sc->vifs, list) {
            if (tmp_vif->use_monitor) {
                netif_tx_stop_all_queues(tmp_vif->ndev);
                netif_carrier_off(tmp_vif->ndev);
                break;
            }
        }
    }

    netif_tx_stop_all_queues(dev);
    netif_carrier_off(dev);

    mgmt_txq = (0 == ssv_vif->drv_vif_index) ? SSV_SW_TXQ_ID_MNG0 : SSV_SW_TXQ_ID_MNG1;
    ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, mgmt_txq);     //pause mgmt txq
    ssv_drv_hci_tx_inactive_by_sta(sc->hci_priv, sc->hci_ops, mgmt_txq);  //inactive mgmt txq 
    
    list_for_each_entry_safe(cur, tmp, &ssv_vif->ap.sta_list, list) {
        ssv_drv_hci_tx_pause_by_sta(sc->hci_priv, sc->hci_ops, cur->sta_idx);     //pause hci tx queue by sta
        ssv_drv_hci_tx_inactive_by_sta(sc->hci_priv, sc->hci_ops, cur->sta_idx);  //inactive hci tx queue by sta
    }

    msleep(200);
#if 0    
    ssv_send_apm_stop_req(sc, ssv_vif);
    mutex_lock(&sc->cb_lock);
    ssv_chanctx_unlink(ssv_vif);
    mutex_unlock(&sc->cb_lock);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
    ssv_vif->bchan_setting = false;
#endif

    /* delete any remaining STA*/
    while (!list_empty(&ssv_vif->ap.sta_list))
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
        ssv_cfg80211_del_station(wiphy, dev, &params);
#else
        ssv_cfg80211_del_station(wiphy, dev, NULL);
#endif
    }
#if 1
    //give more times to cousmer the all tx packets
    //msleep(100);

    ssv_send_apm_stop_req(sc, ssv_vif);
    mutex_lock(&sc->cb_lock);
    ssv_chanctx_unlink(ssv_vif);
    mutex_unlock(&sc->cb_lock);
#endif
    /* delete BC/MC STA */
    sta = &sc->sta_table[ssv_vif->ap.bcmc_index];
    ssv_del_bcn(&ssv_vif->ap.bcn);
    ssv_del_csa(ssv_vif);

    
    ssv_drv_hci_tx_active_by_sta(sc->hci_priv, sc->hci_ops, mgmt_txq); //active hci mng. queue
    ssv_drv_hci_tx_resume_by_sta(sc->hci_priv, sc->hci_ops, mgmt_txq); //resume hci mng. queue
    SSV_LOG_DBG(KERN_ERR"AP stop!!\r\n");
    sc->is_stoping_apm=false;
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static struct ssv_vif *ssv_get_sc_to_vif(struct ssv_softc *sc)
{
	struct ssv_vif *ssv_vif;

	ssv_vif = list_first_or_null_rcu(&sc->vifs, typeof(*ssv_vif), list);
	if (!ssv_vif)
		return ERR_PTR(-EINVAL);

	return ssv_vif;
}
#endif

/**
 * @set_monitor_channel: Set the monitor mode channel for the device. If other
 *	interfaces are active this callback should reject the configuration.
 *	If no interfaces are active or the device is down, the channel should
 *	be stored for when a monitor interface becomes active.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static int ssv_cfg80211_set_monitor_channel(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
                                             struct cfg80211_chan_def *chandef
#else
                                             struct ieee80211_channel *chan,
                                             enum nl80211_channel_type channel_type
#endif
)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = ssv_get_sc_to_vif(sc);
    struct me_config_monitor_cfm cfm;
    int ret = 0;
    bool chan_set = true;
    bool uf = false;

    // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
    struct cfg80211_chan_def chandef_tmp, *chandef;
 
    chandef_tmp.chan = chan;
    chandef = &chandef_tmp;
#endif

    if (IS_ERR(ssv_vif)) {
		return PTR_ERR(ssv_vif);
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    if (cfg80211_chandef_identical(&sc->monitor_chandef, chandef))
#else
    if (sc->monitor_chandef.chan == chan)
#endif
    {
		goto END;
    }

    if (0 != (ret = ssv_send_me_config_monitor_req(sc, chandef, chan_set, uf, &cfm)))
    {
        goto END;
    }
    sc->monitor_chandef = *chandef;
    ssv_chanctx_unlink(ssv_vif);
    ssv_chanctx_link(ssv_vif, cfm.chan_index, chandef);

END:
    return ret;
}
#endif

/**
 * @probe_client: probe an associated client, must return a cookie that it
 *	later passes to cfg80211_probe_status().
 */
int ssv_cfg80211_probe_client(struct wiphy *wiphy, struct net_device *dev,
            const u8 *peer, u64 *cookie)
{
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
/**
 * @update_mgmt_frame_registrations: Notify the driver that management frame
 *	registrations were updated. The callback is allowed to sleep.
 */
void ssv_cfg80211_update_mgmt_frame_registrations(struct wiphy *wiphy,
						 struct wireless_dev *wdev,
						 struct mgmt_frame_regs *upd)
{
//    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
}
#else
/**
 * @mgmt_frame_register: Notify driver that a management frame type was
 *	registered. Note that this callback may not sleep, and cannot run
 *	concurrently with itself.
 */
void ssv_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                   struct wireless_dev *wdev,
#else
                   struct net_device *dev,
#endif
                   u16 frame_type, bool reg)
{
//    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
}
#endif

/**
 * @set_wiphy_params: Notify that wiphy parameters have changed;
 *	@changed bitfield (see &enum wiphy_params_flags) describes which values
 *	have changed. The actual parameter values are available in
 *	struct wiphy. If returning an error, no value should be changed.
 */
static int ssv_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    return 0;
}


/**
 * @set_tx_power: set the transmit power according to the parameters,
 *	the power passed is in mBm, to get dBm use MBM_TO_DBM(). The
 *	wdev may be %NULL if power was set for the wiphy, and will
 *	always be %NULL unless the driver supports per-vif TX power
 *	(as advertised by the nl80211 feature flag.)
 */
static int ssv_cfg80211_set_tx_power(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
                                      struct wireless_dev *wdev,
#endif
                                      enum nl80211_tx_power_setting type, int mbm)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *vif;
    s8 pwr;
    int res = 0;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if (type == NL80211_TX_POWER_AUTOMATIC) {
        pwr = 0x7f;
    } else {
        pwr = MBM_TO_DBM(mbm);
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    if (wdev) {
        vif = container_of(wdev, struct ssv_vif, wdev);
        res = ssv_send_set_power(sc, vif->drv_vif_index, pwr, NULL);
    } else {
#endif
        list_for_each_entry(vif, &sc->vifs, list) {
            res = ssv_send_set_power(sc, vif->drv_vif_index, pwr, NULL);
            if (res)
                break;
        }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    }
#endif

    return res;
}

static int ssv_cfg80211_set_txq_params(struct wiphy *wiphy, struct net_device *dev,
                                        struct ieee80211_txq_params *params)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    u8 hw_queue, aifs, cwmin, cwmax;
    u32 param;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    // set wmm param
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
    hw_queue = ssv_ac2hwq[0][params->ac];
#else
    hw_queue = ssv_ac2hwq[0][params->queue];
#endif

    aifs  = params->aifs;
    cwmin = fls(params->cwmin);
    cwmax = fls(params->cwmax);

    /* Store queue information in general structure */
    param  = (u32) (aifs << 0);
    param |= (u32) (cwmin << 4);
    param |= (u32) (cwmax << 8);
    param |= (u32) (params->txop) << 12;

    if (0 == ssv_cfg.wmm_follow_vo) {
        /* Send the MM_SET_EDCA_REQ message to the FW */
        return ssv_send_set_edca(sc, hw_queue, param, false, ssv_vif->drv_vif_index);
    } else {
        int i = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
        if (NL80211_AC_VO != params->ac)
#else
        if (NL80211_TXQ_Q_VO != params->queue)
#endif
        {
            return 0;
        } else {
            SSV_LOG_DBG("Set all txq param follow vo\n");
            for (i = 0; i < NL80211_NUM_ACS; i++) {
                hw_queue = ssv_ac2hwq[0][i];

                /* Send the MM_SET_EDCA_REQ message to the FW */
                if (0 != ssv_send_set_edca(sc, hw_queue, param, false, ssv_vif->drv_vif_index))
                    SSV_LOG_DBG("Fail to send wmm msg\n");

            }
            return 0;
        }
    }
    
}


/**
 * @remain_on_channel: Request the driver to remain awake on the specified
 *	channel for the specified duration to complete an off-channel
 *	operation (e.g., public action frame exchange). When the driver is
 *	ready on the requested channel, it must indicate this with an event
 *	notification by calling cfg80211_ready_on_channel().
 */
static int
ssv_cfg80211_remain_on_channel(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                                struct wireless_dev *wdev,
#else
                                struct net_device *dev,
#endif
                                 struct ieee80211_channel *chan,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
                                enum nl80211_channel_type channel_type,
#endif
                                unsigned int duration, u64 *cookie)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    struct ssv_vif *ssv_vif = netdev_priv(wdev->netdev);
#else
    struct ssv_vif *ssv_vif = netdev_priv(dev);
#endif
    struct ssv_roc_elem *roc_elem;
    int error;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    /* Blocked until no other RoC procedure is on-going */
    while (sc->roc_elem) {
        SSV_LOG_DBG("The last RoC procedure is still on-going\n");
        msleep(10);
    }

    /* Allocate a temporary RoC element */
    roc_elem = kmalloc(sizeof(struct ssv_roc_elem), GFP_KERNEL);

    /* Verify that element has well been allocated */
    if (!roc_elem) {
        SSV_LOG_DBG("%s() roc_elem alloc failed\n", __FUNCTION__);
        return -ENOMEM;
    }

    /* Initialize the RoC information element */
    roc_elem->wdev = &ssv_vif->wdev;
    roc_elem->chan = chan;
    roc_elem->duration = duration;
    roc_elem->mgmt_roc = false;
    roc_elem->on_chan = false;

    sc->roc_elem = roc_elem;
    /* Set the cookie value */
    *cookie = (u64)(sc->roc_cookie_cnt);

    /* Forward the information to the FMAC */
    error = ssv_send_roc(sc, ssv_vif, chan, duration);

    /* If no error, keep all the information for handling of end of procedure */
    if (0 != error) {
        /* Free the allocated element */
        kfree(roc_elem);
        sc->roc_elem = NULL;
        SSV_LOG_DBG("%s() roc_elem send failed, error %d\n", __FUNCTION__, error);
    }

    return error;
}

/**
 * @cancel_remain_on_channel: Cancel an on-going remain-on-channel operation.
 *	This allows the operation to be terminated prior to timeout based on
 *	the duration value.
 */
static int ssv_cfg80211_cancel_remain_on_channel(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                                                  struct wireless_dev *wdev,
#else
                                                  struct net_device *dev,
#endif
                                                  u64 cookie)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    int error;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    /* Check if a RoC procedure is pending */
    if (!sc->roc_elem) {
        return 0;
    }

    /* Forward the information to the FMAC */
    error = ssv_send_cancel_roc(sc);

    return error;
}

/**
 * @dump_survey: get site survey information.
 */
static int ssv_cfg80211_dump_survey(struct wiphy *wiphy, struct net_device *netdev,
                                     int idx, struct survey_info *info)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ieee80211_supported_band *sband;
    struct ssv_survey_info *ssv_survey;

    if (idx >= ARRAY_SIZE(sc->survey))
        return -ENOENT;

    ssv_survey = &sc->survey[idx];

    // Check if provided index matches with a supported 2.4GHz channel
    sband = wiphy->bands[NL80211_BAND_2GHZ];
    if (sband && idx >= sband->n_channels) {
        idx -= sband->n_channels;
        sband = NULL;
		return -ENOENT;
    }

    // Fill the survey
    info->channel = &sband->channels[idx];
    info->filled = ssv_survey->filled;

    if (ssv_survey->filled != 0) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
        info->time = (u64)ssv_survey->chan_time_ms;
        info->time_busy = (u64)ssv_survey->chan_time_busy_ms;
#else
        info->channel_time = (u64)ssv_survey->chan_time_ms;
        info->channel_time_busy = (u64)ssv_survey->chan_time_busy_ms;
#endif
        info->noise = ssv_survey->noise_dbm;

        // Set the survey report as not used
        ssv_survey->filled = 0;
    }

    return 0;
}

/**
 * @set_channel: Set the current operating channel for the virtual interface.
 *
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
static int ssv_cfg80211_set_channel(struct wiphy *wiphy, struct net_device *dev,
                                    struct ieee80211_channel *chan,
                                    enum nl80211_channel_type channel_type)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    int error = 0;
    
    SSV_LOG_DBG("[%s][%d] freq %d  type %d \n", __FUNCTION__, __LINE__, chan->center_freq, channel_type);
    /* For kernel < 3.8, cfg80211 operation start_ap doesn't contain channel context.
     * CFG80211 will pass channel setting by this operation.
     * So, we must store the setting. 
     * When start_ap operation is called, it can get channel param.
     */
    if (SSV_VIF_TYPE(ssv_vif)) {
        memcpy(&ssv_vif->chan_setting, chan, sizeof(struct ieee80211_channel));
        ssv_vif->chan_type_setting = channel_type;
        ssv_vif->bchan_setting = true;
    }

      
    return error;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
static struct ssv_vif *ssv_get_first_up_vif(struct ssv_softc *sc)
{
    struct ssv_vif *ssv_vif = NULL, *tmp_vif = NULL;

    list_for_each_entry(tmp_vif, &sc->vifs, list) {
        if (tmp_vif->up) {
            ssv_vif = tmp_vif;
            break;
        }
    }

    return ssv_vif;
}
#endif

/**
 * @get_channel: Get the current operating channel for the virtual interface.
 *	For monitor interfaces, it should return %NULL unless there's a single
 *	current monitoring channel.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
static int ssv_cfg80211_get_channel(struct wiphy *wiphy,
                                     struct wireless_dev *wdev,
                                     struct cfg80211_chan_def *chandef)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static struct ieee80211_channel *ssv_cfg80211_get_channel(struct wiphy *wiphy,
                                     struct wireless_dev *wdev,
                                     enum nl80211_channel_type *type)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
static struct ieee80211_channel *ssv_cfg80211_get_channel(struct wiphy *wiphy,
                                     enum nl80211_channel_type *type)
#else
static struct ieee80211_channel *ssv_cfg80211_get_channel(struct wiphy *wiphy)
#endif
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    struct ssv_vif *ssv_vif = container_of(wdev, struct ssv_vif, wdev);
#else
    struct ssv_vif *ssv_vif = ssv_get_first_up_vif(sc);
#endif
    struct ssv_chanctx *ctxt;
    //SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if ((ssv_vif == NULL) ||
        !ssv_vif->up ||
        !ssv_chanctx_valid(sc, ssv_vif->ch_index)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
        return -ENODATA;
#else
        return NULL;
#endif
    }

    ctxt = &sc->chanctx_table[ssv_vif->ch_index];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
    *chandef = ctxt->chan_def;
    
    return 0;
#else
    return ctxt->chan_def.chan;
#endif
}

/*
 * @brief Retrieve the ssv_sta object allocated for a given MAC address
 * and a given role.
 */
static struct ssv_sta *ssv_retrieve_sta(struct ssv_softc *sc,
                                        struct ssv_vif *ssv_vif, u8 *addr,
                                        __le16 fc, bool ap)
{
    if (ap)
    {
        /* only deauth, disassoc and action are bufferable MMPDUs */
        bool bufferable = ieee80211_is_deauth(fc) ||
                          ieee80211_is_disassoc(fc) ||
                          ieee80211_is_action(fc);

        /* Check if the packet is bufferable or not */
        if (bufferable)
        {
            /* Check if address is a broadcast or a multicast address */
            if (is_broadcast_ether_addr(addr) || is_multicast_ether_addr(addr))
            {
                /* Returned STA pointer */
                struct ssv_sta *ssv_sta = &sc->sta_table[ssv_vif->ap.bcmc_index];

                if (ssv_sta->valid)
                    return ssv_sta;
            }
            else
            {
                /* Returned STA pointer */
                struct ssv_sta *ssv_sta;

                /* Go through list of STAs linked with the provided VIF */
                list_for_each_entry(ssv_sta, &ssv_vif->ap.sta_list, list)
                {
                    if (ssv_sta->valid &&
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
                        ether_addr_equal(ssv_sta->mac_addr, addr)
#else
                        !compare_ether_addr(ssv_sta->mac_addr, addr)
#endif
                    ) {
                        /* Return the found STA */
                        return ssv_sta;
                    }
                }
            }
        }
    }
    else
    {
        return ssv_vif->sta.ap;
    }

    return NULL;
}

/**
 * @mgmt_tx: Transmit a management frame.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
static int ssv_cfg80211_mgmt_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
                                struct cfg80211_mgmt_tx_params *params,
                                u64 *cookie)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
static int ssv_cfg80211_mgmt_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
                                struct ieee80211_channel *chan, bool offchan,
                                unsigned int wait, const u8 *buf, size_t len,
                                bool no_cck, bool dont_wait_for_ack, u64 *cookie)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static int ssv_cfg80211_mgmt_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
                                struct ieee80211_channel *chan, bool offchan,
                                enum nl80211_channel_type channel_type,
                                bool channel_type_valid,
                                unsigned int wait, const u8 *buf, size_t len,
                                bool no_cck, bool dont_wait_for_ack, u64 *cookie)
#else
static int ssv_cfg80211_mgmt_tx(struct wiphy *wiphy, struct net_device *dev,
                                struct ieee80211_channel *chan, bool offchan,
                                enum nl80211_channel_type channel_type,
                                bool channel_type_valid,
                                unsigned int wait, const u8 *buf, size_t len,
                                bool no_cck, bool dont_wait_for_ack, u64 *cookie)
#endif
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    struct ssv_vif *ssv_vif = netdev_priv(wdev->netdev);
#else
    struct ssv_vif *ssv_vif = netdev_priv(dev);
#endif
    struct ssv_sta *ssv_sta;
    struct ssv_mgmt_tx_params ssv_params = {0};
    // struct ieee80211_mgmt *mgmt = (void *)params->buf;
    struct ieee80211_mgmt *mgmt = NULL;
    int error = 0;
    bool ap = false;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
    ssv_params.chan = params->chan;
    ssv_params.offchan = params->offchan;
    ssv_params.wait = params->wait;
    ssv_params.buf = params->buf;
    ssv_params.len = params->len;
    ssv_params.no_cck = params->no_cck;
    ssv_params.dont_wait_for_ack = params->dont_wait_for_ack;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
    ssv_params.n_csa_offsets = params->n_csa_offsets;
    ssv_params.csa_offsets = params->csa_offsets;
#endif
#else
    ssv_params.chan = chan;
    ssv_params.offchan = offchan;
    ssv_params.wait = wait;
    ssv_params.buf = buf;
    ssv_params.len = len;
    ssv_params.no_cck = no_cck;
    ssv_params.dont_wait_for_ack = dont_wait_for_ack;
#endif
    mgmt = (struct ieee80211_mgmt *)ssv_params.buf;

    do
    {
        /* Check if provided VIF is an AP or a STA one */
        switch (SSV_VIF_TYPE(ssv_vif))
        {
        case NL80211_IFTYPE_AP_VLAN:
            ssv_vif = ssv_vif->ap_vlan.master;
	        ap = true;
	        break;
        case NL80211_IFTYPE_AP:
        case NL80211_IFTYPE_P2P_GO:
        case NL80211_IFTYPE_MESH_POINT:
            ap = true;
            break;
        case NL80211_IFTYPE_STATION:
        case NL80211_IFTYPE_P2P_CLIENT:
        default:
            break;
        }

        /* Get STA on which management frame has to be sent */
        ssv_sta = ssv_retrieve_sta(sc, ssv_vif, mgmt->da,
                                   mgmt->frame_control, ap);

#if 0
        /* For debug purpose (use ftrace kernel option) */
        trace_mgmt_tx((params->chan) ? params->chan->center_freq : 0,
                      ssv_vif->drv_vif_index,
                      (ssv_sta) ? ssv_sta->sta_idx : 0xFF,
                      mgmt);
#endif
        /* If AP, STA have to exist */
        if (!ssv_sta)
        {
            if (!ap)
            {
                /* if no chan params, it should send frame to air. 
                 * Otherwise, wpa_supplicant will get the fail reason.
                 * Ex: WPA3 */
                if (!ssv_params.chan)
                {
                    error = ssv_start_mgmt_xmit(ssv_vif, ssv_sta, &ssv_params, ssv_params.offchan, cookie);
                    return error;
                }

                /* Check that a RoC is already pending */
                if (sc->roc_elem)
                {
                    /* Get VIF used for current ROC */
                    struct ssv_vif *ssv_roc_vif = netdev_priv(sc->roc_elem->wdev->netdev);

                    /* Check if RoC channel is the same than the required one */
                    if ((sc->roc_elem->chan->center_freq != ssv_params.chan->center_freq) || (ssv_vif->drv_vif_index != ssv_roc_vif->drv_vif_index))
                    {
                        error = -EINVAL;
                        break;
                    }
                }
                else
                {
                    u64 cookie;

                    /* Start a ROC procedure for 30ms */
                    error = ssv_cfg80211_remain_on_channel(wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                                                           wdev,
#else
                                                           dev,
#endif
                                                           ssv_params.chan,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 8, 0)
                                                           NL80211_CHAN_NO_HT,
#endif
                                                           30, &cookie);

                    if (error)
                    {
                        break;
                    }

                    /*
                     * Need to keep in mind that RoC has been launched internally in order to
                     * avoid to call the cfg80211 callback once expired
                     */
                    if (sc->roc_elem)
                        sc->roc_elem->mgmt_roc = true;
                }

                ssv_params.offchan = true;
            }
        }

        /* Push the management frame on the TX path */
        error = ssv_start_mgmt_xmit(ssv_vif, ssv_sta, &ssv_params, ssv_params.offchan, cookie);
    } while (0);

    return error;
}

/**
 * @update_ft_ies: Provide updated Fast BSS Transition information to the
 *	driver. If the SME is in the driver/firmware, this information can be
 *	used in building Authentication and Reassociation Request frames.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
static
int ssv_cfg80211_update_ft_ies(struct wiphy *wiphy,
                            struct net_device *dev,
                            struct cfg80211_update_ft_ies_params *ftie)
{
    return 0;
}
#endif

/**
 * @set_cqm_rssi_config: Configure connection quality monitor RSSI threshold.
 */
static
int ssv_cfg80211_set_cqm_rssi_config(struct wiphy *wiphy,
                                  struct net_device *dev,
                                  int32_t rssi_thold, uint32_t rssi_hyst)
{
    return 0;
}

/**
 * @change_bss: Modify parameters for a given BSS (mainly for AP mode).
 */
int ssv_cfg80211_change_bss(struct wiphy *wiphy, struct net_device *dev,
                             struct bss_parameters *params)
{
    struct ssv_vif *ssv_vif = netdev_priv(dev);
    int res = -EOPNOTSUPP;
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if (((SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_AP) ||
         (SSV_VIF_TYPE(ssv_vif) == NL80211_IFTYPE_P2P_GO)) &&
        (params->ap_isolate > -1))
    {

        if (params->ap_isolate)
            ssv_vif->ap.flags |= SSV_AP_ISOLATE;
        else
            ssv_vif->ap.flags &= ~SSV_AP_ISOLATE;

        res = 0;
    }

    return res;
}

/**
 *
 * @channel_switch: initiate channel-switch procedure (with CSA). Driver is
 *	responsible for veryfing if the switch is possible. Since this is
 *	inherently tricky driver may decide to disconnect an interface later
 *	with cfg80211_stop_iface(). This doesn't mean driver can accept
 *	everything. It should do it's best to verify requests and reject them
 *	as soon as possible.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 12, 0)
int ssv_cfg80211_channel_switch(struct wiphy *wiphy,
                                 struct net_device *dev,
                                 struct cfg80211_csa_settings *params)
{
#if 1
    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    return 0;
#else
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *vif = netdev_priv(dev);
    struct ssv_dma_elem elem;
    struct ssv_bcn *bcn, *bcn_after;
    struct ssv_csa *csa;
    u16 csa_oft[BCN_MAX_CSA_CPT];
    u8 *buf;
    int i, error = 0;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);

    if (vif->ap.csa)
        return -EBUSY;

    if (params->n_counter_offsets_beacon > BCN_MAX_CSA_CPT)
        return -EINVAL;

    /* Build the new beacon with CSA IE */
    bcn = &vif->ap.bcn;
    buf = _ssv_build_bcn(bcn, &params->beacon_csa);
    if (!buf)
        return -ENOMEM;

    memset(csa_oft, 0, sizeof(csa_oft));
    for (i = 0; i < params->n_counter_offsets_beacon; i++)
    {
        csa_oft[i] = params->counter_offsets_beacon[i] + bcn->head_len +
                     bcn->tim_len;
    }

    /* If count is set to 0 (i.e anytime after this beacon) force it to 2 */
    if (params->count == 0)
    {
        params->count = 2;
        for (i = 0; i < params->n_counter_offsets_beacon; i++)
        {
            buf[csa_oft[i]] = 2;
        }
    }

    elem.buf = buf;
    elem.len = bcn->len;

    /* Build the beacon to use after CSA. It will only be sent to fw once
       CSA is over. */
    csa = kzalloc(sizeof(struct ssv_csa), GFP_KERNEL);
    if (!csa)
    {
        error = -ENOMEM;
        goto end;
    }

    bcn_after = &csa->bcn;
    buf = _ssv_build_bcn(bcn_after, &params->beacon_after);
    if (!buf)
    {
        error = -ENOMEM;
        ssv_del_csa(vif);
        goto end;
    }

    vif->ap.csa = csa;
    csa->vif = vif;
    csa->chandef = params->chandef;
    csa->dma.buf = buf;
    csa->dma.len = bcn_after->len;

    /* Send new Beacon. FW will extract channel and count from the beacon */
    error = ssv_send_bcn_change(sc, vif->drv_vif_index, elem.buf,
                                bcn->len, bcn->head_len, bcn->tim_len, csa_oft);

    if (error)
    {
        ssv_del_csa(vif);
        goto end;
    }
    else
    {
        INIT_WORK(&csa->work, ssv_csa_finish);
        cfg80211_ch_switch_started_notify(dev, &csa->chandef, params->count);
    }

end:
    kfree(elem.buf);

    return error;
#endif
}
#endif //#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 12, 0)

/**
 * @get_station: get station information.
 */
int ssv_cfg80211_get_station(struct wiphy *wiphy, 
				struct net_device *dev,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
                const u8 *mac,
#else
                u8 *mac,
#endif
				struct station_info *sinfo)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);   
    s8 tmp_averrssi = sc->rssiaccu / RSSI_MAX;

    SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    sinfo->signal = (s8)(tmp_averrssi);
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
    sinfo->filled |= BIT(NL80211_STA_INFO_SIGNAL);
    sinfo->filled |= BIT(NL80211_STA_INFO_INACTIVE_TIME);
#else
    sinfo->filled |= (STATION_INFO_SIGNAL);
    sinfo->filled |= (STATION_INFO_INACTIVE_TIME);
#endif

    if (mac) 
    {
        struct ssv_sta *sta = ssv_get_sta(sc, mac);
        if (sta)
        {
            sinfo->inactive_time = jiffies_to_msecs(jiffies) - sta->last_rx;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)            
            sinfo->filled |= BIT(NL80211_STA_INFO_STA_FLAGS) |
                             BIT(NL80211_STA_INFO_BSS_PARAM) |
                             BIT(NL80211_STA_INFO_CONNECTED_TIME) |
                             BIT(NL80211_STA_INFO_RX_DROP_MISC) |
                             BIT(NL80211_STA_INFO_BEACON_LOSS);
#else
            sinfo->filled |= (STATION_INFO_STA_FLAGS) |
                             (STATION_INFO_BSS_PARAM) |
                             (STATION_INFO_CONNECTED_TIME) |
                             (STATION_INFO_BEACON_LOSS_COUNT);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
        	if (!(sinfo->filled & (BIT(NL80211_STA_INFO_TX_BYTES64) | BIT(NL80211_STA_INFO_TX_BYTES)))) 
        	{
        		sinfo->tx_bytes = sc->tx.tx_count*128;  //fake fix me
        		sinfo->filled |= BIT(NL80211_STA_INFO_TX_BYTES64);
        	}
#else
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
            if (!(sinfo->filled & ((STATION_INFO_TX_BYTES64) | (STATION_INFO_TX_BYTES)))) 
            {
                sinfo->tx_bytes = sc->tx.tx_count*128;  //fake fix me
                sinfo->filled |= STATION_INFO_TX_BYTES64;
            }
        #else
            if (!(sinfo->filled & STATION_INFO_TX_BYTES)) 
            {
                sinfo->tx_bytes = sc->tx.tx_count*128;  //fake fix me
                sinfo->filled |= STATION_INFO_TX_BYTES;
            }
        #endif
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
        	if (!(sinfo->filled & BIT(NL80211_STA_INFO_TX_PACKETS))) 
        	{
        		sinfo->tx_packets = sc->tx.tx_count;
        		sinfo->filled |= BIT(NL80211_STA_INFO_TX_PACKETS);
        	}
#else
            if (!(sinfo->filled & STATION_INFO_TX_PACKETS)) 
            {
                sinfo->tx_packets = sc->tx.tx_count;
                sinfo->filled |= STATION_INFO_TX_PACKETS;
            }
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
        	if (!(sinfo->filled & (BIT(NL80211_STA_INFO_RX_BYTES64) | BIT(NL80211_STA_INFO_RX_BYTES)))) 
        	{
        		sinfo->rx_bytes = sc->rx.rx_count*512;  //fake fix me
        		sinfo->filled |= BIT(NL80211_STA_INFO_RX_BYTES64);
        	}
#else
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
            if (!(sinfo->filled & ((STATION_INFO_RX_BYTES64) | (STATION_INFO_RX_BYTES)))) 
            {
                sinfo->rx_bytes = sc->rx.rx_count*512;  //fake fix me
                sinfo->filled |= STATION_INFO_RX_BYTES64;
            }
        #else
            if (!(sinfo->filled & STATION_INFO_RX_BYTES)) 
            {
                sinfo->rx_bytes = sc->rx.rx_count*512;  //fake fix me
                sinfo->filled |= STATION_INFO_RX_BYTES;
            }
        #endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
        	if (!(sinfo->filled & BIT(NL80211_STA_INFO_RX_PACKETS))) 
        	{
                sinfo->rx_packets = sc->rx.rx_count;
        		sinfo->filled |= BIT(NL80211_STA_INFO_RX_PACKETS);
        	}      
#else
            if (!(sinfo->filled & (STATION_INFO_RX_PACKETS))) 
            {
                sinfo->rx_packets = sc->rx.rx_count;
                sinfo->filled |= STATION_INFO_RX_PACKETS;
            } 
#endif
        }
    }
    //SSV_LOG_DBG("inactive_time = %d ms\n", sinfo->inactive_time);
    return 0;
}

void ssv_external_auth_enable(struct ssv_vif *vif)
{
    vif->sta.flags |= SSV_STA_EXT_AUTH;
}

void ssv_external_auth_disable(struct ssv_vif *vif)
{
    if (!(vif->sta.flags & SSV_STA_EXT_AUTH))
        return;    
    
    vif->sta.flags &= ~SSV_STA_EXT_AUTH;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3))
int ssv_cfg80211_external_auth(struct wiphy *wiphy, struct net_device *dev,
                struct cfg80211_external_auth_params *params)
{
    struct ssv_softc *sc = wiphy_priv(wiphy);
    struct ssv_vif *ssv_vif = netdev_priv(dev);

#if defined(CONFIG_WPA_SUPPLICANT_CTL)
    if(params->status == 0xFF)
        return 0;
#endif
    
    if (!(ssv_vif->sta.flags & SSV_STA_EXT_AUTH))
        return -EINVAL;

    ssv_external_auth_disable(ssv_vif);
    return ssv_send_sm_external_auth_required_rsp(sc, ssv_vif, params->status); 
}
#endif

struct cfg80211_ops ssv_cfg80211_ops = {
    .add_virtual_intf = ssv_cfg80211_add_iface,
    .del_virtual_intf = ssv_cfg80211_del_iface,
    .change_virtual_intf = ssv_cfg80211_change_iface,
    .scan = ssv_cfg80211_scan,
    .connect = ssv_cfg80211_connect,
    .disconnect = ssv_cfg80211_disconnect,
    .add_key = ssv_cfg80211_add_key,
    .get_key = ssv_cfg80211_get_key,
    .del_key = ssv_cfg80211_del_key,
    .set_default_key = ssv_cfg80211_set_default_key,
    .set_default_mgmt_key = ssv_cfg80211_set_default_mgmt_key,
    .add_station = ssv_cfg80211_add_station,
    .del_station = ssv_cfg80211_del_station,
    .change_station = ssv_cfg80211_change_station,
    .mgmt_tx = ssv_cfg80211_mgmt_tx,
    .start_ap = ssv_cfg80211_start_ap,
    .change_beacon = ssv_cfg80211_change_beacon,
    .stop_ap = ssv_cfg80211_stop_ap,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
    .set_monitor_channel = ssv_cfg80211_set_monitor_channel,
#endif
    .probe_client = ssv_cfg80211_probe_client,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
    .update_mgmt_frame_registrations = ssv_cfg80211_update_mgmt_frame_registrations,
#else
    .mgmt_frame_register = ssv_cfg80211_mgmt_frame_register,
#endif
    .set_wiphy_params = ssv_cfg80211_set_wiphy_params,
    .set_txq_params = ssv_cfg80211_set_txq_params,
    .set_tx_power = ssv_cfg80211_set_tx_power,
    .remain_on_channel = ssv_cfg80211_remain_on_channel,
    .cancel_remain_on_channel = ssv_cfg80211_cancel_remain_on_channel,
    .dump_survey = ssv_cfg80211_dump_survey,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
    .set_channel = ssv_cfg80211_set_channel,
#endif
    .get_channel = ssv_cfg80211_get_channel,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
    .update_ft_ies = ssv_cfg80211_update_ft_ies,
#endif
    .set_cqm_rssi_config = ssv_cfg80211_set_cqm_rssi_config,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 12, 0)
    .channel_switch = ssv_cfg80211_channel_switch,
#endif
    .change_bss = ssv_cfg80211_change_bss,
    .get_station = ssv_cfg80211_get_station,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0) || defined(CONFIG_SUPPORT_WPA3))
    .external_auth = ssv_cfg80211_external_auth,
#endif
};

void ssv_wdev_unregister(struct ssv_softc *sc)
{
    struct ssv_vif *ssv_vif, *tmp;

    rtnl_lock();
    list_for_each_entry_safe(ssv_vif, tmp, &sc->vifs, list) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        ssv_cfg80211_del_iface(sc->wiphy, &ssv_vif->wdev);
#else
        ssv_cfg80211_del_iface(sc->wiphy, ssv_vif->ndev);
#endif

#ifdef FMAC_BRIDGE
        br0_detach(ssv_vif);
#endif
    }
    rtnl_unlock();
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
int ssv_stop_ap(struct wiphy *wiphy, struct net_device *dev)
{
    int err;

    err = ssv_cfg80211_stop_ap(wiphy, dev);

    return err;
}
#endif
