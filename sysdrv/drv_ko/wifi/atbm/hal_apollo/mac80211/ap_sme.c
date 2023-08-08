/*
 * IBSS mode implementation
 * Copyright 2003-2008, Jouni Malinen <j@w1.fi>
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
 * Copyright 2009, Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifdef ATBM_AP_SME
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <net/atbm_mac80211.h>
#include <asm/unaligned.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"

static void ieee80211_ap_sme_work(struct atbm_work_struct *work);
static void ieee80211_ap_sme_event_work(struct atbm_work_struct *work);
static void ieee80211_ap_sme_timer(unsigned long arg);
static int ieee80211_ap_sme_parse_rsn(const u8 *rsn_ie, size_t rsn_ie_len,
			 struct atbm_wpa_ie_data *data);
static int ieee80211_ap_sme_parse_wpa(const u8 *wpa_ie, size_t wpa_ie_len,
			 struct atbm_wpa_ie_data *data);

static int ieee80211_ap_sme_auth_alg(struct ieee80211_sub_if_data *sdata);


#define ATBM_WLAN_AUTH_CHALLENGE_LEN	128
#define ATBM_WLAN_EID_CHALLENGE 		16
#define ATBM_WLAN_AUTH_OPEN				0
#define ATBM_WLAN_AUTH_SHARED_KEY		1
#define ATBM_WLAN_AUTH_NOT_SUPORT		0

#define ATBM_WLAN_AUTH_OPEN_BIT			(BIT(ATBM_WLAN_AUTH_OPEN))
#define ATBM_WLAN_AUTH_SHARED_KEY_BIT	(BIT(ATBM_WLAN_AUTH_SHARED_KEY))

#define ATBM_WLAN_AUTH_TYPE_TO_BIT(type) (BIT(type))

#define ATBM_WLAN_SDATA_AUTHEN_BIT(sdata)	(sdata->vif.bss_conf.auth_type_bit)

#define ATBM_WLAN_AUTH_OPEN_SUPPORT(sdata,rx_athen)		\
	((ATBM_WLAN_SDATA_AUTHEN_BIT(sdata)&ATBM_WLAN_AUTH_TYPE_TO_BIT(rx_athen))==ATBM_WLAN_AUTH_OPEN_BIT)

#define ATBM_WLAN_AUTH_SHARED_SUPPORT(sdata,rx_athen)	\
	((ATBM_WLAN_SDATA_AUTHEN_BIT(sdata)&ATBM_WLAN_AUTH_TYPE_TO_BIT(rx_athen))==ATBM_WLAN_AUTH_SHARED_KEY_BIT)

#define ATBM_GET_IE_FORM_SRC_IE(eid,des_ie,des_off,src_ie,src_len)		\
	do{																	\
		u8* wanted_ie = NULL;											\
		wanted_ie = (u8*)atbm_ieee80211_find_ie(eid, src_ie,src_len);				\
		if(wanted_ie){													\
			memcpy(des_ie+des_off,wanted_ie,wanted_ie[1]+2);			\
			des_off += wanted_ie[1]+2;									\
			atbm_printk_ap( "%s:add (%s)\n",__func__,#eid);				\
		}																\
	}while(0)

#define ATBM_GET_VENDOR_IE_FORM_SRC_IE(oui,oui_type,des_ie,des_off,src_ie,src_len)		\
	do{ 																				\
			u8* wanted_ie = NULL;															\
			wanted_ie = (u8*)atbm_ieee80211_find_vendor_ie(oui,oui_type,src_ie,src_len);				\
			if(wanted_ie){																\
				memcpy(des_ie+des_off,wanted_ie,wanted_ie[1]+2);					\
				des_off += wanted_ie[1]+2;												\
				atbm_printk_ap("%s:add (%s)\n",__func__,#oui_type);						\
			}																			\
	}while(0)

#define ATBM_SET_SUPP_RATE_IE(des_ie,des_off,src_ie,src_len)				\
	ATBM_GET_IE_FORM_SRC_IE(ATBM_WLAN_EID_SUPP_RATES,des_ie,des_off,src_ie,src_len)
	
#define ATBM_SET_EXT_RATE_IE(des_ie,des_off,src_ie,src_len)				\
	ATBM_GET_IE_FORM_SRC_IE(ATBM_WLAN_EID_EXT_SUPP_RATES,des_ie,des_off,src_ie,src_len)

#define ATBM_SET_HT_CAP_IE(des_ie,des_off,src_ie,src_len)				\
	ATBM_GET_IE_FORM_SRC_IE(ATBM_WLAN_EID_HT_CAPABILITY,des_ie,des_off,src_ie,src_len)

#define ATBM_SET_HT_INFO_IE(des_ie,des_off,src_ie,src_len)					\
	ATBM_GET_IE_FORM_SRC_IE(ATBM_WLAN_EID_HT_INFORMATION,des_ie,des_off,src_ie,src_len)
	
#define ATBM_SET_VENDOR_WPS_IE(des_ie,des_off,src_ie,src_len)					\
	ATBM_GET_VENDOR_IE_FORM_SRC_IE(ATBM_OUI_MICROSOFT,4,des_ie,des_off,src_ie,src_len)

#define ATBM_SET_VENDOR_P2P_IE(des_ie,des_off,src_ie,src_len)					\
	ATBM_GET_VENDOR_IE_FORM_SRC_IE(ATBM_OUI_WFA,ATBM_P2P_OUI_TYPE,des_ie,des_off,src_ie,src_len)
	
#define ATBM_SET_VENDOR_WFD_IE(des_ie,des_off,src_ie,src_len)					\
	ATBM_GET_VENDOR_IE_FORM_SRC_IE(ATBM_OUI_WFA,ATBM_WFD_OUI_TYPE,des_ie,des_off,src_ie,src_len)

#define ATBM_SET_VENDOR_WMM_IE(des_ie,des_off,src_ie,src_len)					\
	ATBM_GET_VENDOR_IE_FORM_SRC_IE(ATBM_OUI_MICROSOFT,ATBM_WMM_OUI_TYPE,des_ie,des_off,src_ie,src_len)
/*
*clear_sta_flag(sta, WLAN_STA_AUTH);
	clear_sta_flag(sta, WLAN_STA_AUTH_STATUS);
	clear_sta_flag(sta, WLAN_STA_AUTHORIZED);
	clear_sta_flag(sta, WLAN_STA_ASSOC);
	clear_sta_flag(sta, WLAN_STA_ASSOC_AP);
	clear_sta_flag(sta, WLAN_STA_ASSOCED);
*/
#define ATBM_AP_SME_CLAER_STA_ASSOC_INFO(_sta)   \
	do{											\
		clear_sta_flag(_sta, WLAN_STA_AUTH);		\
		clear_sta_flag(_sta, WLAN_STA_AUTH_STATUS);  \
		clear_sta_flag(_sta, WLAN_STA_AUTHORIZED);		\
		clear_sta_flag(_sta, WLAN_STA_ASSOC);		\
		clear_sta_flag(_sta, WLAN_STA_ASSOC_AP);		\
		clear_sta_flag(_sta, WLAN_STA_ASSOCED);		\
	}while(0)
		
#define RSN_SELECTOR_GET(s) ATBM_WPA_GET_BE32((const u8 *)s) 

enum ap_sme_rx_mgmt_action {
	/* no action required */
	RX_MGMT_NONE,
	RX_MGMT_REP_AHTEN_SEND,
	RX_MGMT_REP_ASSOC_SEND,
	RX_MGMT_REP_AHTEN_SENDED,
	RX_MGMT_REP_ASSOC_SENDED,
	RX_MGMT_REP_DEAHTEN_SENDED,
	RX_MGMT_REP_DISASSOC_SENDED,
};
void ieee80211_ap_sme_mlme_purge(struct ieee80211_sub_if_data *sdata)
{
	mutex_lock(&sdata->ap_sme_mlme_lock);
	atbm_printk_ap("%s\n",__func__);
	atbm_skb_queue_purge(&sdata->ap_sme_skb_queue);
	mutex_unlock(&sdata->ap_sme_mlme_lock);
}
void ieee80211_ap_sme_event_purge(struct ieee80211_sub_if_data *sdata)
{
	mutex_lock(&sdata->ap_sme_event_lock);
	atbm_printk_ap("%s\n",__func__);
	ieee80211_ap_sme_event_free(sdata,&sdata->ap_sme_event);
	mutex_unlock(&sdata->ap_sme_event_lock);
}
void ieee80211_ap_sme_sta_sync_lock(struct ieee80211_sub_if_data *sdata)
{
	static unsigned int lock_cnt = 0;
	lock_cnt++;
	atbm_printk_ap("%s:lock_cnt(%d)\n",__func__,lock_cnt);
	down(&sdata->sta_mlme_lock);	
}
void ieee80211_ap_sme_sta_sync_unlock(struct ieee80211_sub_if_data *sdata)
{
	static unsigned int unlock_cnt = 0;
	unlock_cnt++;
	atbm_printk_ap("%s:unlock_cnt(%d)\n",__func__,unlock_cnt);
	up(&sdata->sta_mlme_lock);	
}
void ieee80211_ap_sme_sta_sync_lock_init(struct ieee80211_sub_if_data *sdata)
{	
	sema_init(&sdata->sta_mlme_lock, 0);
}

void ieee80211_ap_sme_queue_mgmt_init(struct ieee80211_sub_if_data *sdata)
{
	atbm_printk_ap("%s\n",__func__);
	atbm_skb_queue_head_init(&sdata->ap_sme_skb_queue);
	ATBM_INIT_WORK(&sdata->ap_sme_work, ieee80211_ap_sme_work);
	ieee80211_ap_sme_sta_sync_lock_init(sdata);
	mutex_init(&sdata->ap_sme_mlme_lock);
}
void ieee80211_ap_sme_event_free(struct ieee80211_sub_if_data *sdata,
	struct list_head *event_list)
{
	
	while (!list_empty(event_list)) {
			struct ap_sme_event *event =
				list_first_entry(event_list, struct ap_sme_event,
				list);
			list_del(&event->list);
			atbm_kfree(event);
	}
}
void ieee80211_ap_sme_event_init(struct ieee80211_sub_if_data *sdata)
{
	atbm_printk_ap("%s\n",__func__);
	mutex_init(&sdata->ap_sme_event_lock);
	ATBM_INIT_WORK(&sdata->ap_sme_event_work, ieee80211_ap_sme_event_work);
	mutex_lock(&sdata->ap_sme_event_lock);
	INIT_LIST_HEAD(&sdata->ap_sme_event);
	mutex_unlock(&sdata->ap_sme_event_lock);
}

int ieee80211_ap_sme_queue_event(struct ieee80211_sub_if_data *sdata,
											  enum ap_sme_event_id event_id,u8* addr)
{
	struct ap_sme_event *event = NULL;
	u8 first = 0;
	int ret = -1;
	if(STA_EVENT__MAX<= event_id){
		ret = -1;
		goto queue_event_err;
	}

	if(sdata->local->suspended && !sdata->local->resuming){
		ret = -1;
		goto queue_event_err;
	}
	if (!ieee80211_sdata_running(sdata)){
		atbm_printk_ap("%s:sdata is not running\n",__func__);
		goto queue_event_err;
	}
	
	if(addr == NULL)
	{
		goto queue_event_err;
	}
	if (addr[0] == 0xff && addr[1] == 0xff &&
	    addr[2] == 0xff && addr[3] == 0xff &&
	    addr[4] == 0xff && addr[5] == 0xff) 
	{
		goto queue_event_err;
	}
	event = atbm_kzalloc(sizeof(struct ap_sme_event),GFP_KERNEL);

	if(event == NULL){
		ret = -1;
		goto queue_event_err;
	}
	
	event->event = event_id;
	memcpy(event->sta_addr,addr,ETH_ALEN);
	
	mutex_lock(&sdata->ap_sme_event_lock);
	first = list_empty(&sdata->ap_sme_event);
	list_add_tail(&event->list,&sdata->ap_sme_event);
	mutex_unlock(&sdata->ap_sme_event_lock);
	atbm_printk_ap("%s,event(%d),mac(%pM)\n",__func__,event_id,addr);
	if(first)
		atbm_queue_work(sdata->local->workqueue, &sdata->ap_sme_event_work);
	
	ret = 0;
queue_event_err:
	return ret;
}
int ieee80211_ap_sme_rx_mgmt(struct ieee80211_sub_if_data *sdata,
					   struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	__le16 stype;
	
	if (!ieee80211_sdata_running(sdata)){
		atbm_printk_ap("%s:sdata is not running\n",__func__);
		return -1;
	}
	
	stype  = mgmt->frame_control & cpu_to_le16(IEEE80211_FCTL_STYPE);

	switch (stype) {
	case cpu_to_le16(IEEE80211_STYPE_ASSOC_REQ):
	case cpu_to_le16(IEEE80211_STYPE_REASSOC_REQ):
	case cpu_to_le16(IEEE80211_STYPE_DISASSOC):
	case cpu_to_le16(IEEE80211_STYPE_PROBE_REQ):
	case cpu_to_le16(IEEE80211_STYPE_AUTH):
	case cpu_to_le16(IEEE80211_STYPE_DEAUTH):
		atbm_printk_ap("%s:stype(%x),sdata(%x)\n",__func__,stype,(int)sdata);
		break;
	default:
		return -1;
	}

	skb->pkt_type = IEEE80211_SDATA_QUEUE_TYPE_FRAME;
	atbm_skb_queue_tail(&sdata->ap_sme_skb_queue, skb);
	ieee80211_queue_work(&sdata->local->hw, &sdata->ap_sme_work);
	return 0;
}
/*
struct cfg80211_crypto_settings {
	u32 wpa_versions;
	u32 cipher_group;
	int n_ciphers_pairwise;
	u32 ciphers_pairwise[NL80211_MAX_NR_CIPHER_SUITES];
	int n_akm_suites;
	u32 akm_suites[NL80211_MAX_NR_AKM_SUITES];
	bool control_port;
	__be16 control_port_ethertype;
	bool control_port_no_encrypt;
};

enum nl80211_wpa_versions {
	NL80211_WPA_VERSION_1 = 1 << 0,
	NL80211_WPA_VERSION_2 = 1 << 1,
};

*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
int ieee80211_ap_sme_paras_ap_info(struct ieee80211_sub_if_data *sdata,
	struct beacon_parameters *ap_info)
{
	int i = 0;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	struct atbm_wpa_ie_data *wpa_data = &bss_conf->wpa_ap_data;
	bss_conf->auth_type = 0;
	bss_conf->privacy = 0;
	bss_conf->auth_type_bit = 0;
	bss_conf->auth_type = ap_info->auth_type;
	bss_conf->privacy = ap_info->privacy;
	bss_conf->auth_type_bit = ieee80211_ap_sme_auth_alg(sdata);
	atbm_printk_ap( "ap_info:auth_type(%d),privacy(%d),auth_type_bit(%x)\n",bss_conf->auth_type,
		bss_conf->privacy,bss_conf->auth_type_bit);
	memset(wpa_data,0,sizeof(struct atbm_wpa_ie_data));
	if(ap_info->crypto.wpa_versions&NL80211_WPA_VERSION_1){
		atbm_printk_ap("ap_info: support WPA\n");
		wpa_data->proto |= ATBM_WPA_PROTO_WPA;
	}
	if(ap_info->crypto.wpa_versions&NL80211_WPA_VERSION_2){
		atbm_printk_ap("ap_info: support RSN\n");
		wpa_data->proto |= ATBM_WPA_PROTO_RSN;
	}
	
	for(i = 0;i < ap_info->crypto.n_ciphers_pairwise;i++)
	{
		switch(ap_info->crypto.ciphers_pairwise[i]){
		case WLAN_CIPHER_SUITE_CCMP:
			atbm_printk_ap("ap_info: support CCMP\n");
			wpa_data->pairwise_cipher |= ATBM_WPA_CIPHER_CCMP;
			break;
		#if 0
		case WLAN_CIPHER_SUITE_GCMP:
			atbm_printk_ap( "ap_info: support GCMP\n");
			wpa_data->pairwise_cipher |= ATBM_WPA_CIPHER_GCMP;
			break;
		#endif
		case WLAN_CIPHER_SUITE_TKIP:
			atbm_printk_ap("ap_info: support TKIP\n");
			wpa_data->pairwise_cipher |= ATBM_WPA_CIPHER_TKIP;
			break;
		case WLAN_CIPHER_SUITE_WEP104:
			atbm_printk_ap("ap_info: support WEP104\n");
			wpa_data->pairwise_cipher |= ATBM_WPA_CIPHER_WEP104;
			break;
		case WLAN_CIPHER_SUITE_WEP40:
			atbm_printk_ap("ap_info: support WEP40\n");
			wpa_data->pairwise_cipher |= ATBM_WPA_CIPHER_WEP40;
			break;
		default:
			break;
		}
	}

	
	for (i = 0; i < ap_info->crypto.n_akm_suites; i++) {
		switch (ap_info->crypto.akm_suites[i]) {
		case WLAN_AKM_SUITE_8021X:
			wpa_data->key_mgmt |= ATBM_WPA_KEY_MGMT_IEEE8021X;
			atbm_printk_ap("key_mgmt: support IEEE8021X\n");
			break;
		case WLAN_AKM_SUITE_PSK:
			wpa_data->key_mgmt |= ATBM_WPA_KEY_MGMT_PSK;
			atbm_printk_ap("key_mgmt: support PSK\n");
			break;
		default:
			break;
		}
	}

	switch (ap_info->crypto.cipher_group) {
	case WLAN_CIPHER_SUITE_WEP40:
		wpa_data->group_cipher = ATBM_WPA_CIPHER_WEP40;
		atbm_printk_ap("group_cipher:WEP40\n");
		break;
	case WLAN_CIPHER_SUITE_WEP104:
		atbm_printk_ap("group_cipher:WEP104\n");
		wpa_data->group_cipher = ATBM_WPA_CIPHER_WEP104;
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		atbm_printk_ap("group_cipher:TKIP\n");
		wpa_data->group_cipher = ATBM_WPA_CIPHER_TKIP;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		atbm_printk_ap("group_cipher:CCMP\n");
		wpa_data->group_cipher = ATBM_WPA_CIPHER_CCMP;
		break;
	case WLAN_CIPHER_SUITE_SMS4:
		atbm_printk_ap("group_cipher:SMS4\n");
		wpa_data->group_cipher = ATBM_WPA_CIPHER_SMS4;
		break;
	default:
		atbm_printk_ap("group_cipher:NONE\n");
		wpa_data->group_cipher = ATBM_WPA_CIPHER_NONE;
		break;
	}
	/*
	*rsn: get mgmt protect
	*/
	while(wpa_data->proto&ATBM_WPA_PROTO_RSN)
	{
		u8* rsn_ie = NULL;
		size_t rsn_ie_len = 0;
		struct atbm_wpa_ie_data rsn_data;
		rsn_ie = (u8*)atbm_ieee80211_find_ie(WLAN_EID_RSN, ap_info->tail, ap_info->tail_len);

		if(rsn_ie == NULL)
			break;
		rsn_ie_len = *(rsn_ie + 1);

		/* skip element id and length */
		rsn_ie += 2;

		/* skip version */
		if (rsn_ie_len < 2)
			break;

		if (ieee80211_ap_sme_parse_rsn(rsn_ie,rsn_ie_len,&rsn_data) == 0)
		{
			wpa_data->rsn_capabilities = rsn_data.rsn_capabilities;
			wpa_data->mgmt_group_cipher = rsn_data.mgmt_group_cipher;
		}

		break;
	}

	return 0;
}
#else
int ieee80211_ap_sme_paras_ap_info(struct ieee80211_sub_if_data *sdata,
	struct beacon_parameters *ap_info)
{
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	struct atbm_wpa_ie_data *wpa_data = &bss_conf->wpa_ap_data;
	struct atbm_ieee80211_mgmt *beacon = (struct atbm_ieee80211_mgmt *)ap_info->head;
	struct ieee802_atbm_11_elems ap_elems;
	struct atbm_wpa_ie_data ap_wpa_data;
	struct atbm_wpa_ie_data ap_rsn_data;
	
	bss_conf->auth_type = 0;
	bss_conf->privacy = 0;
	bss_conf->auth_type_bit = 0;

	memset(wpa_data,0,sizeof(struct atbm_wpa_ie_data));
	memset(&ap_wpa_data,0,sizeof(struct atbm_wpa_ie_data));
	memset(&ap_rsn_data,0,sizeof(struct atbm_wpa_ie_data));
	bss_conf->privacy = ATBM_WLAN_CAPABILITY_PRIVACY&beacon->u.beacon.capab_info ? 1 : 0;
	ieee802_11_parse_elems((u8*)ap_info->tail,ap_info->tail_len,&ap_elems);

	if(ap_elems.wpa || ap_elems.rsn){
		bss_conf->auth_type_bit = ATBM_WLAN_AUTH_OPEN_BIT;
	}else if(bss_conf->privacy)
		bss_conf->auth_type_bit = ATBM_WLAN_AUTH_SHARED_KEY_BIT;
	else
		bss_conf->auth_type_bit = ATBM_WLAN_AUTH_OPEN_BIT;

	if(ap_elems.wpa){
		wpa_data->proto |= ATBM_WPA_PROTO_WPA;
		ieee80211_ap_sme_parse_wpa(ap_elems.wpa,ap_elems.wpa_len,&ap_wpa_data);
	}

	if(ap_elems.rsn){
		wpa_data->proto |= ATBM_WPA_PROTO_RSN;
		ieee80211_ap_sme_parse_rsn(ap_elems.rsn,ap_elems.rsn_len,&ap_rsn_data);
	}
	
	wpa_data->pairwise_cipher |= (ap_wpa_data.pairwise_cipher | ap_rsn_data.pairwise_cipher);
	wpa_data->key_mgmt |= (ap_wpa_data.key_mgmt | ap_rsn_data.key_mgmt);
	wpa_data->group_cipher |= (ap_wpa_data.group_cipher | ap_rsn_data.group_cipher);
	wpa_data->rsn_capabilities = ap_rsn_data.rsn_capabilities;
	wpa_data->mgmt_group_cipher = ap_rsn_data.mgmt_group_cipher;
	
}
#endif
int ieee80211_ap_sme_tx_mgmt_status(struct ieee80211_sub_if_data *sdata,
					   struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	__le16 stype;

	if (!ieee80211_sdata_running(sdata)){
		atbm_printk_ap("%s:sdata is not running\n",__func__);
		return -1;
	}
	stype  = mgmt->frame_control & cpu_to_le16(IEEE80211_FCTL_STYPE);

	switch (stype) {
	case cpu_to_le16(IEEE80211_STYPE_ASSOC_RESP):
	case cpu_to_le16(IEEE80211_STYPE_REASSOC_RESP):
	case cpu_to_le16(IEEE80211_STYPE_DISASSOC):
	case cpu_to_le16(IEEE80211_STYPE_PROBE_RESP):
	case cpu_to_le16(IEEE80211_STYPE_AUTH):
	case cpu_to_le16(IEEE80211_STYPE_DEAUTH):
		atbm_printk_ap("%s:stype(%x)\n",__func__,stype);
		break;
	default:
		return -1;
	}

	skb->pkt_type = IEEE80211_SDATA_QUEUE_SEND_STATUS;
	atbm_skb_queue_tail(&sdata->ap_sme_skb_queue, skb);
	ieee80211_queue_work(&sdata->local->hw, &sdata->ap_sme_work);
	return 0;
}
void ieee80211_ap_sme_sta_session_timer_init(struct sta_info *sta)
{
	atbm_init_timer(&sta->sta_session_timer);
	sta->sta_session_timer.data = (unsigned long )sta;
	sta->sta_session_timer.function = ieee80211_ap_sme_timer;
}
u16 ieee80211_ap_sme_free_aid(struct ieee80211_sub_if_data *sdata, struct sta_info *sta)
{
	if(sta->sta.aid == 0)
	{
		atbm_printk_ap("%s:sta aid(%d)\n",__func__,sta->sta.aid);
		return WLAN_STATUS_SUCCESS;
	}

	if(sta->sta.aid>=9)
	{
		atbm_printk_ap("%s:sta aid err(%d)\n",__func__,sta->sta.aid);
		return WLAN_STATUS_UNSPECIFIED_FAILURE;
	}
	sdata->vif.bss_conf.aid_map &= ~BIT(sta->sta.aid-1);
	sta->sta.aid = 0;
	atbm_printk_ap("%s:aid_map(%x)\n",__func__,sdata->vif.bss_conf.aid_map);
	return WLAN_STATUS_SUCCESS;
}

static int ieee80211_ap_sme_auth_alg(struct ieee80211_sub_if_data *sdata)
{
	if(sdata->vif.bss_conf.auth_type == NL80211_AUTHTYPE_OPEN_SYSTEM)
		return ATBM_WLAN_AUTH_OPEN_BIT;
	else if(sdata->vif.bss_conf.auth_type == NL80211_AUTHTYPE_SHARED_KEY)
		return ATBM_WLAN_AUTH_SHARED_KEY_BIT;
	else if(sdata->vif.bss_conf.auth_type == NL80211_AUTHTYPE_AUTOMATIC)
		return ATBM_WLAN_AUTH_SHARED_KEY_BIT | ATBM_WLAN_AUTH_OPEN_BIT;
	return ATBM_WLAN_AUTH_NOT_SUPORT;
}
static void ieee80211_ap_sme_timer(unsigned long arg)
{
	struct sta_info *sta = (struct sta_info *)arg;


	enum ap_sme_event_id event_id = STA_EVENT__FREE;
	if(test_sta_flag(sta, WLAN_STA_ASSOC_AP))
		event_id = STA_EVENT__DEAUTHEN;
	atbm_printk_ap("%s:event_id(%d)\n",__func__,event_id);
	ieee80211_ap_sme_queue_event(sta->sdata,event_id,sta->sta.addr);
}

/* create and insert a dummy station entry */
static int ieee80211_pre_athen(struct ieee80211_sub_if_data *sdata,
				u8 *sa) {
	struct sta_info *sta;
	int err;
	atbm_printk_ap("%s:sta(%pM)\n",__func__,sa);
	sta = sta_info_alloc(sdata, sa, GFP_KERNEL);
	if (!sta)
		return -ENOMEM;

	sta->dummy = true;

	err = sta_info_insert(sta);
	sta = NULL;
	if (err) {
		atbm_printk_ap("%s: failed to insert Dummy STA entry for"
		       " the AP (error %d)\n", sdata->name, err);
		return err;
	}
	return 0;
}
static int ieee80211_ap_sme_auth_shared_key(struct ieee80211_sub_if_data *sdata,
			struct sta_info *sta,u16 transaction,const u8 *challenge)
{
	if (transaction == 1) {
		if (!sta->challenge) {
			sta->challenge = atbm_kmalloc(ATBM_WLAN_AUTH_CHALLENGE_LEN,GFP_KERNEL);
			if (sta->challenge == NULL)
				return WLAN_STATUS_UNSPECIFIED_FAILURE;

		}
		return WLAN_STATUS_SUCCESS;
	}
	if (transaction != 3)
		return WLAN_STATUS_UNSPECIFIED_FAILURE;

	if (!sta->challenge || !challenge ||
	    memcmp(sta->challenge, challenge, ATBM_WLAN_AUTH_CHALLENGE_LEN)) {
		atbm_printk_ap("%s:challenge err\n",__func__);
		return WLAN_STATUS_CHALLENGE_FAIL;
	}
	atbm_kfree(sta->challenge);
	sta->challenge = NULL;
	return WLAN_STATUS_SUCCESS;
}
static void ieee80211_ap_sme_tx_mgmt_authen_status(struct ieee80211_sub_if_data *sdata,struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct sta_info *sta = NULL;
	u16 auth_alg, auth_transaction, status_code;

	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->da);
	if(sta == NULL)
	{
		atbm_printk_ap("%s:sta == NULL\n",__func__);
		goto authen_status_err;
	}
	
	auth_alg = mgmt->u.auth.auth_alg;
	auth_transaction = mgmt->u.auth.auth_transaction;
	status_code =  mgmt->u.auth.status_code;
	atbm_printk_ap("%s:sta(%pM),auth_alg(%d),auth_transaction(%d),status_code(%d),ack(%d)\n",__func__,
		mgmt->da,auth_alg,auth_transaction,status_code,!!(info->flags & IEEE80211_TX_STAT_ACK));
	if(status_code != WLAN_STATUS_SUCCESS)
	{
		clear_sta_flag(sta, WLAN_STA_AUTH);
		goto authen_status_err;
	}
	if(test_sta_flag(sta, WLAN_STA_AUTHORIZED))
	{
		goto authen_status_err;
	}
	if(
		(ATBM_WLAN_AUTH_OPEN_SUPPORT(sdata,auth_alg)&&(auth_transaction == 2))
		||
		(ATBM_WLAN_AUTH_SHARED_SUPPORT(sdata,auth_alg)&&(auth_transaction == 4))
	  )
	{
	  	if(!(info->flags & IEEE80211_TX_STAT_ACK))
			set_sta_flag(sta,WLAN_STA_AUTH_STATUS);
		else
			set_sta_flag(sta,WLAN_STA_AUTHORIZED);
	}
authen_status_err:
	rcu_read_unlock();
	atbm_dev_kfree_skb(skb);
}
static enum ap_sme_rx_mgmt_action __must_check
ieee80211_ap_sme_rx_mgmt_authen(struct ieee80211_sub_if_data *sdata,
				     struct atbm_ieee80211_mgmt *mgmt,
				     size_t len,
				     struct ieee80211_rx_status *rx_status,u16 *status)
{
	struct sta_info *sta = NULL;
	u16 auth_alg, auth_transaction, status_code;
	const u8 *challenge = NULL;
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->sa);
	if(sta == NULL)
	{
		rcu_read_unlock();
		if(ieee80211_pre_athen(sdata,mgmt->sa))
		{
			atbm_printk_ap("%s:sta_info_alloc err\n",__func__);
			return RX_MGMT_NONE;
		}
		rcu_read_lock();
		sta = sta_info_get_rx(sdata, mgmt->sa);

		if(sta == NULL)
		{
			atbm_printk_ap("%s:sta_info_get_bss err\n",__func__);

			goto authen_err;
		}
		/*
		*start connect timer
		*/
	}

	if(test_sta_flag(sta, WLAN_STA_AUTHORIZED) || test_sta_flag(sta,WLAN_STA_ASSOCED)){
		atbm_printk_ap("%s:sta(%pM) has been athened or assoced,what should we do\n",__func__,mgmt->sa);
		ATBM_AP_SME_CLAER_STA_ASSOC_INFO(sta);
	}

	auth_alg = mgmt->u.auth.auth_alg;
	auth_transaction = mgmt->u.auth.auth_transaction;
	status_code =  mgmt->u.auth.status_code;

	if (len >= 24 + sizeof(mgmt->u.auth) +
	    2 + ATBM_WLAN_AUTH_CHALLENGE_LEN &&
	    mgmt->u.auth.variable[0] == ATBM_WLAN_EID_CHALLENGE &&
	    mgmt->u.auth.variable[1] == ATBM_WLAN_AUTH_CHALLENGE_LEN)
		challenge = &mgmt->u.auth.variable[2];

	if(
		!((ATBM_WLAN_AUTH_OPEN_SUPPORT(sdata,auth_alg))
		||
		(ATBM_WLAN_AUTH_SHARED_SUPPORT(sdata,auth_alg)))
	  )
	{
		atbm_printk_ap("%s:auth_alg(%d) bss_conf.auth_type(%d) err \n",__func__,auth_alg,sdata->vif.bss_conf.auth_type);
		*status = WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG;
		goto athen_end;
	}

	
	if (!((auth_transaction == 1 ) ||
		  (auth_alg == ATBM_WLAN_AUTH_SHARED_KEY && auth_transaction == 3))) {
			atbm_printk_ap("Unknown authentication transaction number (%d)\n",
				   auth_transaction);
			*status = WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION;
			goto athen_end;
	}
	atbm_printk_ap("%s:sta(%pM)\n",__func__,mgmt->sa);
	*status = WLAN_STATUS_SUCCESS;
	if (!atbm_timer_pending(&sta->sta_session_timer) ||
	    !time_is_after_jiffies(sta->sta_session_timer.expires))
		atbm_mod_timer(&sta->sta_session_timer, jiffies + 3*HZ);
	set_sta_flag(sta, WLAN_STA_AUTH);
athen_end:
	rcu_read_unlock();
	return RX_MGMT_REP_AHTEN_SEND;
authen_err:
	rcu_read_unlock();
	return RX_MGMT_NONE;
}
static void ieee80211_ap_sme_send_auth(struct ieee80211_sub_if_data *sdata,
			 u16 transaction, u16 auth_alg,
			 u8 *extra, size_t extra_len, const u8 *da,u16 status)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *mgmt;


	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom +
			    sizeof(*mgmt) + 6 + extra_len);
	if (!skb)
		return;

	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	mgmt = (struct atbm_ieee80211_mgmt *) atbm_skb_put(skb, 24 + 6);
	memset(mgmt, 0, 24 + 6);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_AUTH);
	memcpy(mgmt->da, da, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, sdata->vif.addr, ETH_ALEN);
	mgmt->u.auth.auth_alg = cpu_to_le16(auth_alg);
	mgmt->u.auth.auth_transaction = cpu_to_le16(transaction);
	mgmt->u.auth.status_code = cpu_to_le16(status);
	if (extra)
		memcpy(atbm_skb_put(skb, extra_len), extra, extra_len);
 
 	atbm_printk_ap("%s %d transaction =%d \n",__func__,__LINE__,transaction);
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_AP_HANDLE_STATUS;
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_USE_MINRATE;
	if(sdata->vif.p2p)
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_NO_CCK_RATE;
	atbm_printk_ap("%s:sta(%pM),transaction(%d),auth_alg(%d),status(%d)\n",__func__,
		da,transaction,auth_alg,status);
	ieee80211_tx_skb(sdata, skb);
}
static void ieee80211_ap_sme_send_deauth(struct ieee80211_sub_if_data *sdata,
	const u8 *da,struct sta_info * sta)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct atbm_ieee80211_mgmt *mgmt;

	skb = atbm_dev_alloc_skb(local->hw.extra_tx_headroom + sizeof(*mgmt));
	if (!skb)
		return;

	atbm_skb_reserve(skb, local->hw.extra_tx_headroom);

	mgmt = (struct atbm_ieee80211_mgmt *) atbm_skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, da, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, sdata->vif.addr, ETH_ALEN);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DEAUTH);
	if(test_sta_flag(sta,WLAN_STA_WPS))
		mgmt->u.deauth.reason_code = cpu_to_le16(23);
	else
		mgmt->u.deauth.reason_code = cpu_to_le16(3);
	
	atbm_skb_put(skb, 2);
	atbm_printk_ap("%s:sta(%pM),reason_code(%u)\n",__func__,da,mgmt->u.deauth.reason_code);
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_AP_HANDLE_STATUS;
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_USE_MINRATE;
	if(sdata->vif.p2p)
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_NO_CCK_RATE;
	ieee80211_tx_skb(sdata, skb);
}
static void ieee80211_ap_sme_tx_mgmt_authen(struct ieee80211_sub_if_data *sdata,
	u16 status,struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	u16 auth_alg, auth_transaction;
	struct sta_info *sta = NULL;
	u8 *resp_ies = NULL;
	u8 resp_ies_len = 0;
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->sa);

	if(sta == NULL){
		atbm_printk_ap("%s:sta == NULL\n",__func__);
		goto tx_mgmt_authen_err;
	}
	if(!test_sta_flag(sta, WLAN_STA_AUTH)){
		atbm_printk_ap("%s:not in authen state\n",__func__);
		goto tx_mgmt_authen_err;
	}
	
	auth_alg = mgmt->u.auth.auth_alg;
	auth_transaction = mgmt->u.auth.auth_transaction;
	atbm_printk_ap("%s:status(%d)\n",__func__,status);
	if(status == WLAN_STATUS_SUCCESS)
	{
		if(ATBM_WLAN_AUTH_OPEN_SUPPORT(sdata,auth_alg)){
			atbm_printk_ap("%s:open alog\n",__func__);
			set_sta_flag(sta,WLAN_STA_AUTH_STATUS);
			goto tx_mgmt_authen;
		}
		else if(ATBM_WLAN_AUTH_SHARED_SUPPORT(sdata,auth_alg))
		{
			const u8 *challenge = NULL;
			if (skb->len >= 24 + sizeof(mgmt->u.auth) +
	   			 2 + ATBM_WLAN_AUTH_CHALLENGE_LEN &&
	   			 mgmt->u.auth.variable[0] == ATBM_WLAN_EID_CHALLENGE &&
	   			 mgmt->u.auth.variable[1] == ATBM_WLAN_AUTH_CHALLENGE_LEN)
			   challenge = &mgmt->u.auth.variable[2];
			atbm_printk_ap("%s:share key alog\n",__func__);
			status = ieee80211_ap_sme_auth_shared_key(sdata,sta,auth_transaction,challenge);

			while(sta->challenge && (auth_transaction == 1) &&(status == WLAN_STATUS_SUCCESS))
			{
				resp_ies = atbm_kmalloc(ATBM_WLAN_AUTH_CHALLENGE_LEN+2,GFP_KERNEL);
				if(resp_ies == NULL)
				{
					status = WLAN_STATUS_UNSPECIFIED_FAILURE;
					break;
				}
				resp_ies[0] = ATBM_WLAN_EID_CHALLENGE;
				resp_ies[1] = ATBM_WLAN_AUTH_CHALLENGE_LEN;
				memcpy(resp_ies + 2, sta->challenge,
				  	ATBM_WLAN_AUTH_CHALLENGE_LEN);
				resp_ies_len = 2 + ATBM_WLAN_AUTH_CHALLENGE_LEN;
			}
			
			if(status == WLAN_STATUS_SUCCESS){
				if(auth_transaction == 3)
					set_sta_flag(sta,WLAN_STA_AUTH_STATUS);
			}
			goto tx_mgmt_authen;
		}
		else{
			status = WLAN_STATUS_UNSPECIFIED_FAILURE;
		}
	}
tx_mgmt_authen:
	ieee80211_ap_sme_send_auth(sdata,auth_transaction+1,auth_alg,resp_ies,resp_ies_len,mgmt->sa,status);
tx_mgmt_authen_err:
	rcu_read_unlock();
	atbm_dev_kfree_skb(skb);
	if(resp_ies)
		atbm_kfree(resp_ies);
	return;
}
static enum ap_sme_rx_mgmt_action __must_check
ieee80211_ap_sme_rx_mgmt_deauth(struct ieee80211_sub_if_data *sdata,
			 struct atbm_ieee80211_mgmt *mgmt, size_t len)
{

	u16 reason_code;
	struct sta_info *sta = NULL;
	bool is_wps = false;
	if (len < 24 + 2)
		return RX_MGMT_NONE;
	
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->sa);
	if(sta == NULL)
	{
		rcu_read_unlock();
		return RX_MGMT_NONE;
	}
	reason_code = le16_to_cpu(mgmt->u.deauth.reason_code);

	atbm_printk_ap("%s: deauthenticated from %pM (Reason: %u)\n",
			sdata->name, mgmt->sa, reason_code);
	
	ATBM_AP_SME_CLAER_STA_ASSOC_INFO(sta);
	is_wps = test_sta_flag(sta,WLAN_STA_WPS)?true:false;
	rcu_read_unlock();
	if(is_wps == false)
		ieee80211_ap_sme_queue_event(sdata,STA_EVENT__FREE,mgmt->sa);
	return RX_MGMT_NONE;
}
static u16 ieee80211_ap_sme_check_ssid(struct ieee80211_sub_if_data *sdata,u8 *ssid,u8 ssid_len)
{
	if(ssid == NULL)
		return WLAN_STATUS_UNSPECIFIED_FAILURE;
	atbm_printk_ap("%s:bss_conf.ssid_len(%d),ssid_len(%d)\n",__func__,sdata->vif.bss_conf.ssid_len,
		ssid_len);
	if(sdata->vif.bss_conf.ssid_len)
		atbm_printk_ap("%s:bss_conf.ssid(%s)\n",__func__,sdata->vif.bss_conf.ssid);
	if(ssid_len)
		atbm_printk_ap("%s:ssid(%s)\n",__func__,ssid);
	if((sdata->vif.bss_conf.ssid_len != ssid_len) ||
		memcmp(sdata->vif.bss_conf.ssid,ssid,ssid_len) != 0)
	{
		return WLAN_STATUS_UNSPECIFIED_FAILURE;
	}

	return WLAN_STATUS_SUCCESS;
}
static u16 ieee80211_ap_sme_check_wmm(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems,
	struct ieee802_atbm_11_elems *ap_elems)
{
	bool ap_wmm_suport = (!!ap_elems->wmm_info)|(!!ap_elems->wmm_param);
	bool sta_wmm_support = (!!sta_elems->wmm_info)|(!!sta_elems->wmm_param);
	atbm_printk_ap("%s:ap_wmm_suport(%x),sta_wmm_support(%x)\n",__func__,ap_wmm_suport,
		sta_wmm_support);
	if(ap_wmm_suport&&sta_wmm_support){
		set_sta_flag(sta, WLAN_STA_WME);
		sta->sta.wme = true;
	}
	else{
		clear_sta_flag(sta, WLAN_STA_WME);
		sta->sta.wme = false;
	}

	return WLAN_STATUS_SUCCESS;
}

static u16 ieee80211_ap_sme_check_supp_rates(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband;
	u32 rates = 0;
	u8  i = 0;
	u8  j = 0;
	sband = local->hw.wiphy->bands[chan_state->oper_channel->band];
	
	
	if(!sta_elems->supp_rates)
		return WLAN_STATUS_UNSPECIFIED_FAILURE;

	for (i = 0; i < sta_elems->supp_rates_len; i++) {
		int rate = (sta_elems->supp_rates[i] & 0x7f) * 5;
		for (j = 0; j < sband->n_bitrates; j++) {
			if (sband->bitrates[j].bitrate == rate)
				rates |= BIT(j);
		}
	}
	sta->sta.supp_rates[chan_state->oper_channel->band] = rates;

	return WLAN_STATUS_SUCCESS;
}

static u16 ieee80211_ap_sme_check_ht_cap(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems,
	struct ieee802_atbm_11_elems *ap_elems)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sdata);
	struct ieee80211_supported_band *sband;
	
	sband = local->hw.wiphy->bands[chan_state->oper_channel->band];
	atbm_printk_ap("%s:ap_elems->ht_cap_elem(%x),sta_elems->ht_cap_elem(%x)\n",__func__,(int)ap_elems->ht_cap_elem
		,(int)sta_elems->ht_cap_elem);
	if((!ap_elems->ht_cap_elem)&&(sta_elems->ht_cap_elem))
		return WLAN_STATUS_UNSPECIFIED_FAILURE;
	
	if(!sta_elems->ht_cap_elem)		
		return WLAN_STATUS_SUCCESS;
	
	ieee80211_ht_cap_ie_to_sta_ht_cap(sband,
							  sta_elems->ht_cap_elem,
							  &sta->sta.ht_cap);

	return WLAN_STATUS_SUCCESS;
}
static u16 ieee80211_ap_sme_check_vht_cap(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems,
	struct ieee802_atbm_11_elems *ap_elems)
{
	return WLAN_STATUS_SUCCESS;
}
static u16 ieee80211_ap_sme_check_p2p(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems,
	struct ieee802_atbm_11_elems *ap_elems)
{
	return 0;
}

static int rsn_selector_to_bitfield(const u8 *s)
{
	
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_NONE)
		return ATBM_WPA_CIPHER_NONE;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_WEP40)
		return ATBM_WPA_CIPHER_WEP40;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_TKIP)
		return ATBM_WPA_CIPHER_TKIP;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_CCMP)
		return ATBM_WPA_CIPHER_CCMP;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_WEP104)
		return ATBM_WPA_CIPHER_WEP104;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_AES_128_CMAC)
		return ATBM_WPA_CIPHER_AES_128_CMAC;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_CIPHER_SUITE_GCMP)
		return ATBM_WPA_CIPHER_GCMP;
	return 0;
}

static int rsn_key_mgmt_to_bitfield(const u8 *s)
{
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_UNSPEC_802_1X)
		return ATBM_WPA_KEY_MGMT_IEEE8021X;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X)
		return ATBM_WPA_KEY_MGMT_PSK;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_FT_802_1X)
		return ATBM_WPA_KEY_MGMT_FT_IEEE8021X;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_FT_PSK)
		return ATBM_WPA_KEY_MGMT_FT_PSK;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_802_1X_SHA256)
		return ATBM_WPA_KEY_MGMT_IEEE8021X_SHA256;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_PSK_SHA256)
		return ATBM_WPA_KEY_MGMT_PSK_SHA256;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_SAE)
		return ATBM_WPA_KEY_MGMT_SAE;
	if (RSN_SELECTOR_GET(s) == ATBM_RSN_AUTH_KEY_MGMT_FT_SAE)
		return ATBM_WPA_KEY_MGMT_FT_SAE;
	return 0;
}

static int wpa_selector_to_bitfield(const u8 *s)
{
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_CIPHER_SUITE_NONE)
		return ATBM_WPA_CIPHER_NONE;
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_CIPHER_SUITE_WEP40)
		return ATBM_WPA_CIPHER_WEP40;
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_CIPHER_SUITE_TKIP)
		return ATBM_WPA_CIPHER_TKIP;
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_CIPHER_SUITE_CCMP)
		return ATBM_WPA_CIPHER_CCMP;
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_CIPHER_SUITE_WEP104)
		return ATBM_WPA_CIPHER_WEP104;
	return 0;
}

static int wpa_key_mgmt_to_bitfield(const u8 *s)
{
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_AUTH_KEY_MGMT_UNSPEC_802_1X)
		return ATBM_WPA_KEY_MGMT_IEEE8021X;
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X)
		return ATBM_WPA_KEY_MGMT_PSK;
	if (RSN_SELECTOR_GET(s) == ATBM_WPA_AUTH_KEY_MGMT_NONE)
		return ATBM_WPA_KEY_MGMT_WPA_NONE;
	return 0;
}

static int ieee80211_ap_sme_parse_rsn(const u8 *rsn_ie, size_t rsn_ie_len,
			 struct atbm_wpa_ie_data *data)
{
	const u8 *pos;
	int left;
	int i, count;

	memset(data, 0, sizeof(*data));
	data->proto = ATBM_WPA_PROTO_RSN;
	data->pairwise_cipher = ATBM_WPA_CIPHER_CCMP;
	data->group_cipher = ATBM_WPA_CIPHER_CCMP;
	data->key_mgmt = ATBM_WPA_KEY_MGMT_IEEE8021X;
	data->rsn_capabilities = 0;
	data->pmkid = NULL;
	data->num_pmkid = 0;

	pos = rsn_ie+2;
	left = rsn_ie_len -2;

	if (left >= ATBM_RSN_SELECTOR_LEN) {
		data->group_cipher = rsn_selector_to_bitfield(pos);
		
		if (data->group_cipher == ATBM_WPA_CIPHER_AES_128_CMAC) {
			atbm_printk_ap("%s: AES-128-CMAC used as group "
				   "cipher", __func__);
			return -1;
		}
		pos += ATBM_RSN_SELECTOR_LEN;
		left -= ATBM_RSN_SELECTOR_LEN;
	} else if (left > 0) {
		atbm_printk_ap("%s: ie length mismatch, %u too much",
			   __func__, left);
		return -3;
	}

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * ATBM_RSN_SELECTOR_LEN) {
			atbm_printk_ap("%s: ie count botch (pairwise), "
				   "count %u left %u", __func__, count, left);
			return -4;
		}
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= rsn_selector_to_bitfield(pos);
			pos += ATBM_RSN_SELECTOR_LEN;
			left -= ATBM_RSN_SELECTOR_LEN;
		}
		if (data->pairwise_cipher & ATBM_WPA_CIPHER_AES_128_CMAC) {
			atbm_printk_ap("%s: AES-128-CMAC used as "
				   "pairwise cipher", __func__);
			return -1;
		}
	} else if (left == 1) {
		atbm_printk_ap("%s: ie too short (for key mgmt)",
			   __func__);
		return -5;
	}

	if (left >= 2) {
		data->key_mgmt = 0;
		count = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * ATBM_RSN_SELECTOR_LEN) {
			atbm_printk_ap("%s: ie count botch (key mgmt), "
				   "count %u left %u", __func__, count, left);
			return -6;
		}
		for (i = 0; i < count; i++) {
			data->key_mgmt |= rsn_key_mgmt_to_bitfield(pos);
			pos += ATBM_RSN_SELECTOR_LEN;
			left -= ATBM_RSN_SELECTOR_LEN;
		}
	} else if (left == 1) {
		atbm_printk_ap("%s: ie too short (for capabilities)",
			   __func__);
		return -7;
	}

	if (left >= 2) {
		data->rsn_capabilities = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
	}
	
	if (left >= 2) {
		data->num_pmkid = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (left < (int) data->num_pmkid * ATBM_PMKID_LEN) {
			atbm_printk_ap("%s: PMKID underflow "
				   "(num_pmkid=%lu left=%d)",
				   __func__, (unsigned long) data->num_pmkid,
				   left);
			data->num_pmkid = 0;
			return -9;
		} else {
			data->pmkid = pos;
			pos += data->num_pmkid * ATBM_PMKID_LEN;
			left -= data->num_pmkid * ATBM_PMKID_LEN;
		}
	}
	
	if (left >= 4) {
		data->mgmt_group_cipher = rsn_selector_to_bitfield(pos);
		if (data->mgmt_group_cipher != ATBM_WPA_CIPHER_AES_128_CMAC) {
			atbm_printk_ap("%s: Unsupported management "
				   "group cipher 0x%x", __func__,
				   data->mgmt_group_cipher);
			return -10;
		}
		pos += ATBM_RSN_SELECTOR_LEN;
		left -= ATBM_RSN_SELECTOR_LEN;
	}
	
	if (left > 0) {
		atbm_printk_ap("%s: ie has %u trailing bytes - ignored",
			   __func__, left);
	}

	return 0;
}
int ieee80211_ap_sme_parse_wpa(const u8 *wpa_ie, size_t wpa_ie_len,
			 struct atbm_wpa_ie_data *data)
{
	const u8 *pos;
	int left;
	int i, count;

	memset(data, 0, sizeof(*data));
	data->proto = ATBM_WPA_PROTO_WPA;
	data->pairwise_cipher = ATBM_WPA_CIPHER_TKIP;
	data->group_cipher = ATBM_WPA_CIPHER_TKIP;
	data->key_mgmt = ATBM_WPA_KEY_MGMT_IEEE8021X;
	data->wpa_capabilities = 0;
	data->pmkid = NULL;
	data->num_pmkid = 0;
	data->mgmt_group_cipher = 0;

	if (wpa_ie_len == 0) {
		/* No WPA IE - fail silently */
		return -1;
	}

	pos = (const u8 *) (wpa_ie + 6);
	left = wpa_ie_len -6;

	if (left >= ATBM_WPA_SELECTOR_LEN) {
		data->group_cipher = wpa_selector_to_bitfield(pos);
		pos += ATBM_WPA_SELECTOR_LEN;
		left -= ATBM_WPA_SELECTOR_LEN;
	} else if (left > 0) {
		atbm_printk_ap("%s: ie length mismatch, %u too much",
			   __func__, left);
		return -3;
	}

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * ATBM_WPA_SELECTOR_LEN) {
			atbm_printk_ap("%s: ie count botch (pairwise), "
				   "count %u left %u", __func__, count, left);
			return -4;
		}
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= wpa_selector_to_bitfield(pos);
			pos += ATBM_WPA_SELECTOR_LEN;
			left -= ATBM_WPA_SELECTOR_LEN;
		}
	} else if (left == 1) {
		atbm_printk_ap("%s: ie too short (for key mgmt)",
			   __func__);
		return -5;
	}

	if (left >= 2) {
		data->key_mgmt = 0;
		count = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * ATBM_WPA_SELECTOR_LEN) {
			atbm_printk_ap("%s: ie count botch (key mgmt), "
				   "count %u left %u", __func__, count, left);
			return -6;
		}
		for (i = 0; i < count; i++) {
			data->key_mgmt |= wpa_key_mgmt_to_bitfield(pos);
			pos += ATBM_WPA_SELECTOR_LEN;
			left -= ATBM_WPA_SELECTOR_LEN;
		}
	} else if (left == 1) {
		atbm_printk_ap("%s: ie too short (for capabilities)",
			   __func__);
		return -7;
	}

	if (left >= 2) {
		data->wpa_capabilities = ATBM_WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
	}

	if (left > 0) {
		atbm_printk_ap("%s: ie has %u trailing bytes - ignored",
			   __func__, left);
	}

	return 0;
}

static u16 ieee80211_ap_sme_check_wpa(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems,
	struct ieee802_atbm_11_elems *ap_elems)
{
	u8* sta_wpa_ie = NULL;
	u8  sta_wpa_ie_len = 0;
	u8* ap_wpa_ie = NULL;
	u8  ap_wpa_ie_len = 0;
	struct atbm_wpa_ie_data *ap_wpa = &sdata->vif.bss_conf.wpa_ap_data;
	struct atbm_wpa_ie_data *sta_wpa = &sta->sta.wpa_sta_data;
	int res = -1;
	if(ap_elems->wpa&&sta_elems->wpa){
		sta_wpa_ie = sta_elems->wpa;
		sta_wpa_ie_len = sta_elems->wpa_len;
		ap_wpa_ie = ap_elems->wpa;
		ap_wpa_ie_len = ap_elems->wpa_len;
		atbm_printk_ap("%s:use wpa\n",__func__);
	}
	else if(ap_elems->rsn&&sta_elems->rsn){
		sta_wpa_ie = sta_elems->rsn;
		sta_wpa_ie_len = sta_elems->rsn_len;
		ap_wpa_ie = ap_elems->rsn;
		ap_wpa_ie_len = ap_elems->rsn_len;
		atbm_printk_ap("%s:use rsn\n",__func__);
	}
	atbm_printk_ap("%s:sta_wpa_ie(%x),sta_wpa_ie_len(%x)\n",__func__,(int)sta_wpa_ie,(int)sta_wpa_ie_len);
	if((sta_wpa_ie==NULL)&&(ap_wpa_ie==NULL)){
		
		if((sta_elems->wpa || sta_elems->rsn)&&(ap_elems->wpa || ap_elems->rsn))
		{
			atbm_printk_ap("%s:wpa rsn err\n",__func__);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}
		
	}else if(sta_wpa_ie&&ap_wpa_ie){

		atbm_printk_ap("%s:ap_wpa_ie(%x),ap_elems->wpa(%x),ap_elems->rsn(%x)\n",__func__,
			(int)ap_wpa_ie,(int)ap_elems->wpa,(int)ap_elems->rsn);
		if(ap_wpa_ie ==  ap_elems->wpa){
			res = ieee80211_ap_sme_parse_wpa(sta_wpa_ie,sta_wpa_ie_len,sta_wpa);
		}
		else if(ap_wpa_ie == ap_elems->rsn){
			res = ieee80211_ap_sme_parse_rsn(sta_wpa_ie,sta_wpa_ie_len,sta_wpa);
		}

		if(res)
		{
			atbm_printk_ap("%s:parse_rsn or parse_wpa err\n",__func__);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}

		if(sta_wpa->group_cipher != ap_wpa->group_cipher)
		{
			atbm_printk_ap("%s:group_cipher(%d),(%d)\n",__func__,sta_wpa->group_cipher,ap_wpa->group_cipher);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}

		if (!(sta_wpa->key_mgmt & ap_wpa->key_mgmt)) {
			atbm_printk_ap("%s:key_mgmt(%x),(%x)\n",__func__,sta_wpa->key_mgmt,ap_wpa->key_mgmt);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}

		if (!(sta_wpa->pairwise_cipher& ap_wpa->pairwise_cipher)) {
			atbm_printk_ap("%s:pairwise_cipher(%x),(%x)\n",__func__,sta_wpa->pairwise_cipher,ap_wpa->pairwise_cipher);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}

		if(ap_wpa->rsn_capabilities & ATBM_WPA_CAPABILITY_MFPC)
		{
			if(sta_wpa->rsn_capabilities&ATBM_WPA_CAPABILITY_MFPC){
				if (sta_wpa->pairwise_cipher & ATBM_WPA_CIPHER_TKIP) {
					atbm_printk_ap("Management frame protection "
						   "cannot use TKIP");
					return WLAN_STATUS_UNSPECIFIED_FAILURE;
				}

				if (sta_wpa->mgmt_group_cipher != ATBM_WPA_CIPHER_AES_128_CMAC) {
					atbm_printk_ap("Unsupported management group "
						   "cipher %d", sta_wpa->mgmt_group_cipher);
					return WLAN_STATUS_UNSPECIFIED_FAILURE;
				}

				set_sta_flag(sta, WLAN_STA_MFP);
			}
			else{
				clear_sta_flag(sta, WLAN_STA_MFP);
			}
		}
		else
			clear_sta_flag(sta, WLAN_STA_MFP);

		set_sta_flag(sta, WLAN_STA_WPA_RSN);
	}
	return WLAN_STATUS_SUCCESS;
}
static u16 ieee80211_ap_sme_check_wps(struct ieee80211_sub_if_data *sdata,struct sta_info *sta,
	struct ieee802_atbm_11_elems *sta_elems,
	struct ieee802_atbm_11_elems *ap_elems)
{
	bool ap_wps=false;
	bool sta_wps=false;
	if(ap_elems->wps_ie&&ap_elems->wps_ie_len){
		atbm_printk_ap( "%s:ap support wps\n",__func__);
		ap_wps = true;
	}
	if(sta_elems->wps_ie&&sta_elems->wps_ie_len){
		atbm_printk_ap( "%s:sta support wps\n",__func__);
		sta_wps = true;
	}

	if(test_sta_flag(sta, WLAN_STA_WPA_RSN)){
		if(sta_wps==true){
			atbm_printk_ap( "%s:sta support both rsn and wps\n",__func__);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}
	}else if(sta_wps == true){
		if(ap_wps == false){
			atbm_printk_ap( "%s:sta support wps but ap not support\n",__func__);
			return WLAN_STATUS_UNSPECIFIED_FAILURE;
		}
		else
			set_sta_flag(sta, WLAN_STA_WPS);
	}

	return WLAN_STATUS_SUCCESS;
}

static u16 ieee80211_ap_sme_check_assoc_ies(struct ieee80211_sub_if_data *sdata, struct sta_info *sta,
			   const u8 *ies, size_t ies_len, int reassoc,const u8 *ap_ie,size_t ap_ie_len)
{
	struct ieee802_atbm_11_elems sta_elems;
	struct ieee802_atbm_11_elems ap_elems;
	u16 status=WLAN_STATUS_SUCCESS;
		
	memset(&sta_elems,0,sizeof(struct ieee802_atbm_11_elems));
	memset(&ap_elems,0,sizeof(struct ieee802_atbm_11_elems));
	
	clear_sta_flag(sta, WLAN_STA_WPA_RSN);
	clear_sta_flag(sta, WLAN_STA_WPS);
	
	ieee802_11_parse_elems((u8*)ies,ies_len,&sta_elems);
	ieee802_11_parse_elems((u8*)ap_ie,ap_ie_len,&ap_elems);
	
	/*
	*check ssid
	*/
	status = ieee80211_ap_sme_check_ssid(sdata,sta_elems.ssid,sta_elems.ssid_len); 
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:ssid err\n",__func__);
		goto assoc_ies_end;
	}
	/*
	*check wmm
	*/
	status = ieee80211_ap_sme_check_wmm(sdata,sta,&sta_elems,&ap_elems);
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:wmm err\n",__func__);
		goto assoc_ies_end;
	}
	/*
	*check rates
	*/
	status = ieee80211_ap_sme_check_supp_rates(sdata,sta,&sta_elems);
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:supp_rates err\n",__func__);
		goto assoc_ies_end;
	}
	/*
	*check ht cap
	*/
	status = ieee80211_ap_sme_check_ht_cap(sdata,sta,&sta_elems,&ap_elems);
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:check_ht_cap err\n",__func__);
		goto assoc_ies_end;
	}
	/*
	*check vht cap
	*but now only return success
	*/
	status = ieee80211_ap_sme_check_vht_cap(sdata,sta,&sta_elems,&ap_elems);
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:check_ht_cap err\n",__func__);
		goto assoc_ies_end;
	}
	/*
	*check p2p
	*/
	status = ieee80211_ap_sme_check_p2p(sdata,sta,&sta_elems,&ap_elems);
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:check_ht_cap err\n",__func__);
		goto assoc_ies_end;
	}
	/*
	*check wpa
	*/
	status = ieee80211_ap_sme_check_wpa(sdata,sta,&sta_elems,&ap_elems);
	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:check_wpa err\n",__func__);
		goto assoc_ies_end;
	}

	status = ieee80211_ap_sme_check_wps(sdata,sta,&sta_elems,&ap_elems);

	if(status != WLAN_STATUS_SUCCESS)
	{
		atbm_printk_ap( "%s:check_wps err\n",__func__);
		goto assoc_ies_end;
	}
assoc_ies_end:
	
	return status;

}
static u16 ieee80211_ap_sme_alloc_aid(struct ieee80211_sub_if_data *sdata, struct sta_info *sta)
{
	u8 aid = 0;
	if(sta->sta.aid>0){
		atbm_printk_ap( "%s:sta aid(%d)\n",__func__,sta->sta.aid);

		return WLAN_STATUS_SUCCESS;
	}

	if(sdata->vif.bss_conf.aid_map >= (u8)(-1)){
		
		atbm_printk_ap( "%s:aid_map err(%x)\n",__func__,sta->sta.aid);

		return WLAN_STATUS_UNSPECIFIED_FAILURE;
	}
	for(aid=0;aid<8;aid++)
	{
		if(BIT(aid)&(sdata->vif.bss_conf.aid_map))
			continue;
		break;
	}
	sta->sta.aid = aid+1;
	sdata->vif.bss_conf.aid_map |= BIT(aid);
	atbm_printk_ap( "%s:aid_map(%x)\n",__func__,sdata->vif.bss_conf.aid_map);
	return WLAN_STATUS_SUCCESS;
}
static enum ap_sme_rx_mgmt_action __must_check
ieee80211_ap_sme_rx_mgmt_assoc(struct ieee80211_sub_if_data *sdata,
			   struct atbm_ieee80211_mgmt *mgmt, size_t len,struct ieee80211_rx_status *rx_status,int reassoc,u16 *status)
{
	struct sta_info *sta = NULL;
	u16 resp = WLAN_STATUS_UNSPECIFIED_FAILURE;
	u16 capab_info, listen_interval;
	const u8 *pos;
	int left;
	u8* ap_ie = NULL;
	size_t ap_ie_len = 0;
	struct beacon_data *beacon = NULL;
	
	if(len < 24 + (reassoc ? sizeof(mgmt->u.reassoc_req) :
				      sizeof(mgmt->u.assoc_req))){
		atbm_printk_ap( "%s:len err(%d)\n",__func__,len);
		*status = resp;
		return RX_MGMT_NONE;
	}
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->sa);
	
	if(sta == NULL)
	{
		goto mgmt_assoc_err;
	}
	
	/*
	*sta maybe has received the athen success frame, but
	*we not receive the ack,so here change the sta sta;
	**/
	if(test_sta_flag(sta,WLAN_STA_AUTH_STATUS))
		set_sta_flag(sta,WLAN_STA_AUTHORIZED);
	/*
	* if the athen process has not been complete,so do not
	* process the asso.
	**/
	if(!test_sta_flag(sta,WLAN_STA_AUTHORIZED))
		goto mgmt_assoc_err;
	
	if(test_sta_flag(sta, WLAN_STA_ASSOC_AP))
	{
		atbm_printk_ap( "%s:sta has connected with ap , so return err\n",__func__);
		goto mgmt_assoc_err;
	}

	beacon = rcu_dereference(sdata->u.ap.beacon);

	if(beacon == NULL){
		*status = resp;
		goto mgmt_assoc_err;
	}
	ap_ie = beacon->tail;
	ap_ie_len = beacon->tail_len;
	
	if (reassoc) {
		capab_info = le16_to_cpu(mgmt->u.reassoc_req.capab_info);
		listen_interval = le16_to_cpu(mgmt->u.reassoc_req.listen_interval);
		left = len - offsetof(struct atbm_ieee80211_mgmt, u.reassoc_req.variable);
		pos = mgmt->u.reassoc_req.variable;
	} else {
		capab_info = le16_to_cpu(mgmt->u.assoc_req.capab_info);
		listen_interval = le16_to_cpu(
			mgmt->u.assoc_req.listen_interval);
		left = len - offsetof(struct atbm_ieee80211_mgmt, u.assoc_req.variable);
		pos = mgmt->u.assoc_req.variable;
	}
	
	resp = ieee80211_ap_sme_check_assoc_ies(sdata, sta, pos, left, reassoc,ap_ie,ap_ie_len);
	if(resp != WLAN_STATUS_SUCCESS){
		goto mgmt_assoc_err;
	}
	
	if (capab_info & ATBM_WLAN_CAPABILITY_SHORT_PREAMBLE)
		set_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE);
	else
	{
		sdata->vif.bss_conf.num_sta_no_short_preamble++;
		clear_sta_flag(sta, WLAN_STA_SHORT_PREAMBLE);
	}

	if(capab_info & ATBM_WLAN_CAPABILITY_SHORT_SLOT_TIME)
		set_sta_flag(sta, WLAN_STA_SHORT_SLOT);
	else
	{
		sdata->vif.bss_conf.num_sta_no_short_slot_time++;
		clear_sta_flag(sta, WLAN_STA_SHORT_SLOT);
	}
	
	resp = ieee80211_ap_sme_alloc_aid(sdata,sta);
	
	if(resp == WLAN_STATUS_SUCCESS&&(sta->associate_ie == NULL))
	{
		sta->associate_ie = atbm_kmalloc(left,GFP_KERNEL);
		if(sta->associate_ie == NULL){
			resp = WLAN_STATUS_UNSPECIFIED_FAILURE;
			goto mgmt_assoc_err;
		}
		memcpy(sta->associate_ie,pos,left);
		sta->associate_ie_len = left;
		atbm_printk_ap( "%s:associate_ie(%x),associate_ie_len(%d)\n",
			__func__,(int)sta->associate_ie,sta->associate_ie_len);
	}
mgmt_assoc_err:
	rcu_read_unlock();
	*status = resp;
	return RX_MGMT_REP_ASSOC_SEND;
}
static void ieee80211_ap_sme_assoc_resp(struct ieee80211_sub_if_data *sdata,
			 u16 aid, u16 capab_info,u8 *extra, size_t extra_len, 
			 const u8 *da,u8 reassoc,u16 status)
{
	struct atbm_ieee80211_mgmt *reply;
	struct sk_buff *assoc_skb;

	assoc_skb = atbm_dev_alloc_skb(sdata->local->hw.extra_tx_headroom +
			    sizeof(struct atbm_ieee80211_mgmt) + 6 + extra_len);
	if(assoc_skb == NULL){
		atbm_printk_ap( "%s:assoc_skb == NULL\n",__func__);
		return;
	}

	atbm_skb_reserve(assoc_skb, sdata->local->hw.extra_tx_headroom);

	reply = (struct atbm_ieee80211_mgmt *) atbm_skb_put(assoc_skb, 24 + 6);;

	reply->frame_control =cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  (reassoc?IEEE80211_STYPE_REASSOC_RESP:IEEE80211_STYPE_ASSOC_RESP));
	memcpy(reply->da, da, ETH_ALEN);
	memcpy(reply->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(reply->bssid, sdata->vif.addr, ETH_ALEN);
	reply->u.assoc_resp.status_code = cpu_to_le16(status);
	reply->u.assoc_resp.capab_info = cpu_to_le16(capab_info);
	reply->u.assoc_resp.aid = cpu_to_le16(aid| BIT(14) | BIT(15));

	if (extra)
		memcpy(atbm_skb_put(assoc_skb, extra_len), extra, extra_len);
 
 	atbm_printk_ap( "%s %d aid =%d,capab_info %d\n",__func__,__LINE__,aid,capab_info);
	IEEE80211_SKB_CB(assoc_skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	IEEE80211_SKB_CB(assoc_skb)->flags |= IEEE80211_TX_AP_HANDLE_STATUS;
	IEEE80211_SKB_CB(assoc_skb)->flags |= IEEE80211_TX_CTL_USE_MINRATE;
	if(sdata->vif.p2p)
		IEEE80211_SKB_CB(assoc_skb)->flags |= IEEE80211_TX_CTL_NO_CCK_RATE;
	ieee80211_tx_skb(sdata, assoc_skb);
}

static void ieee80211_ap_sme_tx_mgmt_assoc_resp(struct ieee80211_sub_if_data *sdata,
	u16 status,struct sk_buff *skb,u8 reassoc)
{
	struct sta_info *sta = NULL;
	struct atbm_ieee80211_mgmt *mgmt = (struct atbm_ieee80211_mgmt *)skb->data;
	struct beacon_data *beacon;
	u8* rep_ie = NULL;
	size_t rep_ie_len = 0;
	u16 capab_info = 0;
	
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->sa);
	
	if(sta == NULL)
	{
		goto tx_mgmt_assoc_err;
	}

	/*
	* if the athen process has not been complete,so do not
	* process the asso.
	**/
	if(!test_sta_flag(sta,WLAN_STA_AUTHORIZED))
		goto tx_mgmt_assoc_err;

	rep_ie = atbm_kmalloc(1024,GFP_KERNEL);

	if(rep_ie == NULL)
	{
		atbm_printk_ap( "%s:rep_ie == NULL\n",__func__);
		goto tx_mgmt_assoc_err;
	}
	
	if (sdata->vif.bss_conf.privacy)
		capab_info |= ATBM_WLAN_CAPABILITY_PRIVACY;
	if(sdata->vif.bss_conf.use_short_preamble&&
		(sdata->vif.bss_conf.num_sta_no_short_preamble == 0))
		capab_info |= ATBM_WLAN_CAPABILITY_SHORT_PREAMBLE;
	if(sdata->vif.bss_conf.use_short_slot&&
		(sdata->vif.bss_conf.num_sta_no_short_slot_time==0))
		capab_info |= ATBM_WLAN_CAPABILITY_SHORT_SLOT_TIME;

	beacon = rcu_dereference(sdata->u.ap.beacon);
	if(beacon)
	{
		u8* beacon_ie = beacon->tail;
		u32 beacon_ie_len = beacon->tail_len;
		struct associate_data *assoc_ie;
		/*
		*add assoc resp ie as follow
		*support rate ie 
		*externd rate ie
		*ht cap ie
		*ht info ie
		*p2p ie if the interface is p2p go interface
		*wdf ie if the interface is p2p g0 interface
		*wmm if the sta support wmm
		*/
		ATBM_SET_SUPP_RATE_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
		ATBM_SET_EXT_RATE_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
		ATBM_SET_HT_CAP_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
		ATBM_SET_HT_INFO_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);

		if(test_sta_flag(sta, WLAN_STA_WME))
		{
			ATBM_SET_VENDOR_WMM_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
		}

		assoc_ie = rcu_dereference(sdata->u.ap.associate_ie);
		if(assoc_ie){
			/*
			*linux kernel version is after version 3.1,come here.
			*/
			atbm_printk_ap( "%s:set assoc_ie\n",__func__);
			memcpy(rep_ie+rep_ie_len,assoc_ie->associate_ies,assoc_ie->associate_ies_len);
			rep_ie_len += assoc_ie->associate_ies_len;
		}
		else
		{
			/*
			*linux kernel version is before version 3.1,come here.
			*/
			ATBM_SET_VENDOR_WPS_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
			ATBM_SET_VENDOR_P2P_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
			ATBM_SET_VENDOR_WFD_IE(rep_ie,rep_ie_len,beacon_ie,beacon_ie_len);
		}
	}
	else
	{
		atbm_printk_ap( "%s:beacon==NULL\n",__func__);
	}
	
	atbm_printk_ap( "%s:sta(%pM),aid(%d),cap(%x),req_ie(%x),rep_ie_len(%d),status(%d)\n",__func__,
		mgmt->sa,sta->sta.aid,capab_info,(int)rep_ie,rep_ie_len,status);
	if(status == WLAN_STATUS_SUCCESS)
		set_sta_flag(sta, WLAN_STA_ASSOC);
	ieee80211_ap_sme_assoc_resp(sdata,sta->sta.aid,capab_info,rep_ie,
		rep_ie_len,mgmt->sa,reassoc,status);
	
tx_mgmt_assoc_err:	
	rcu_read_unlock();
	atbm_dev_kfree_skb(skb);
	if(rep_ie)
		atbm_kfree(rep_ie);

	return;
}

static void ieee80211_ap_sme_tx_mgmt_assoc_status(struct ieee80211_sub_if_data *sdata,struct sk_buff *skb)
{
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta = NULL;
	u16 status_code;
	
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->da);
	if(sta == NULL)
	{
		atbm_printk_ap( "%s:sta == NULL\n",__func__);
		goto assoc_status_err;
	}

	status_code = mgmt->u.assoc_resp.status_code;

	if(status_code != WLAN_STATUS_SUCCESS)
	{
		clear_sta_flag(sta, WLAN_STA_AUTH);
		clear_sta_flag(sta, WLAN_STA_AUTH_STATUS);
		clear_sta_flag(sta, WLAN_STA_AUTHORIZED);
		atbm_printk_ap( "%s:status_code != WLAN_STATUS_SUCCESS\n",__func__);
		goto assoc_status_err;
	}

	if(!(info->flags & IEEE80211_TX_STAT_ACK))
	{
		clear_sta_flag(sta, WLAN_STA_ASSOC);
		atbm_printk_ap( "%s:no ack\n",__func__);
		goto assoc_status_err;
	}

	if(!test_sta_flag(sta,WLAN_STA_ASSOC))
	{
		atbm_printk_ap( "%s:not assoc\n",__func__);
		goto assoc_status_err;
	}

	if(!sta->associate_ie)
	{
		atbm_printk_ap( "%s:!sta->associate_ie\n",__func__);
		clear_sta_flag(sta, WLAN_STA_ASSOC);
		goto assoc_status_err;
	}
#ifndef CONFIG_RATE_HW_CONTROL
	rate_control_rate_init(sta);
#endif
	set_sta_flag(sta, WLAN_STA_ASSOC_AP);
	rcu_read_unlock();
	atbm_del_timer_sync(&sta->sta_session_timer);
	/*
	*local->sta_mtx will be unlocked at function sta_info_reinsert
	*/
	mutex_lock(&local->sta_mtx);
	if(!sta_info_reinsert(sta))	{
		atbm_printk_ap( "%s:STA_EVENT__ASSOCED\n",__func__);
		ieee80211_ap_sme_queue_event(sdata,STA_EVENT__ASSOCED,mgmt->da);
	}
	else{
		atbm_printk_ap( "%s:STA_EVENT__SEND_DEAUTHEN\n",__func__);
		ieee80211_ap_sme_queue_event(sdata,STA_EVENT__SEND_DEAUTHEN,mgmt->da);
	}
	atbm_dev_kfree_skb(skb);
	return;
assoc_status_err:
	rcu_read_unlock();

	atbm_dev_kfree_skb(skb);
}
static enum ap_sme_rx_mgmt_action __must_check
ieee80211_ap_sme_rx_mgmt_disassoc(struct ieee80211_sub_if_data *sdata,
			   struct atbm_ieee80211_mgmt *mgmt, size_t len)
{
	__le16 reason_code = 0;
	struct sta_info *sta = NULL;
	bool assoced = false;
	
	if (len < 24 + 2)
		return RX_MGMT_NONE;
	
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->sa);
	if(sta == NULL)
	{
		atbm_printk_ap( "%s:sta == NULL\n",__func__);
		rcu_read_unlock();
		goto rx_mgmt_disassoc_err;
	}

	if(!test_sta_flag(sta, WLAN_STA_AUTH))
	{
		atbm_printk_ap( "rx_mgmt_disassoc sa(%pM),not auten\n",mgmt->sa);
	}

	if(!test_sta_flag(sta, WLAN_STA_AUTHORIZED))
	{
		atbm_printk_ap( "rx_mgmt_disassoc sa(%pM),not autened\n",mgmt->sa);
	}

	if(!test_sta_flag(sta, WLAN_STA_ASSOC))
	{
		atbm_printk_ap( "rx_mgmt_disassoc sa(%pM),not assoc\n",mgmt->sa);
	}

	if(!test_sta_flag(sta, WLAN_STA_ASSOC_AP))
	{
		atbm_printk_ap( "rx_mgmt_disassoc sa(%pM),not assoced ap\n",mgmt->sa);
	}
	else
	{
		assoced = true;
	}
	if(assoced == false)
	{
		ATBM_AP_SME_CLAER_STA_ASSOC_INFO(sta);
	}
	reason_code = le16_to_cpu(mgmt->u.disassoc.reason_code);
	atbm_printk_ap( "rx_mgmt_disassoc sa(%pM),reason_code(%d)\n",mgmt->sa,reason_code);
	rcu_read_unlock();
	ieee80211_ap_sme_queue_event(sdata,(assoced == true) ? 
		STA_EVENT__DEAUTHEN:STA_EVENT__FREE,mgmt->sa);
rx_mgmt_disassoc_err:
	return RX_MGMT_NONE;
}
static void ieee80211_ap_sme_deauthen_and_disassoc_send_status(
	struct ieee80211_sub_if_data *sdata,struct sk_buff *skb)
{
	struct sta_info *sta = NULL;
	struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
	bool unlock = false;
	bool is_wps = false;
	
	rcu_read_lock();
	sta = sta_info_get_rx(sdata, mgmt->da);
	if(sta == NULL){
		rcu_read_unlock();
		atbm_printk_ap( "%s:sta == NULL\n",__func__);
		goto status_err;
	}
	
	atbm_printk_ap( "%s:sta(%pM) rx deauthen or disassoc status\n",__func__,mgmt->da);

	ATBM_AP_SME_CLAER_STA_ASSOC_INFO(sta);
	if(test_sta_flag(sta,WLAN_STA_DEAUTHENNING)){
//		set_sta_flag(sta,WLAN_STA_DEAUTHENED);
		clear_sta_flag(sta,WLAN_STA_DEAUTHENNING);				
		unlock = true;
	}
	is_wps = test_sta_flag(sta,WLAN_STA_WPS)?true:false;
	rcu_read_unlock();
	
	if(unlock == true)
		ieee80211_ap_sme_sta_sync_unlock(sdata);
	
	if(is_wps == false)
		ieee80211_ap_sme_queue_event(sdata,STA_EVENT__FREE,mgmt->da);
status_err:
	atbm_dev_kfree_skb(skb);
}
static void ieee80211_ap_sme_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data, ap_sme_work);
	struct sk_buff *skb;
	enum ap_sme_rx_mgmt_action rma = RX_MGMT_NONE;
	u16 status = WLAN_STATUS_UNSPECIFIED_FAILURE;
	u8 reassoc = 0;
	
	if (!ieee80211_sdata_running(sdata)){
		atbm_printk_ap( "%s:sdata not running\n",__func__);
		return;
	}
	
	mutex_lock(&sdata->ap_sme_mlme_lock);
	while ((skb = atbm_skb_dequeue(&sdata->ap_sme_skb_queue))) {
		struct atbm_ieee80211_mgmt *mgmt = (void *)skb->data;
		u16 fc = le16_to_cpu(mgmt->frame_control);
		/*
		*when you nend to change the flow functions NOTICE THAT:
		*the sta_info is protected by rcu,so in the flow functions,we
		*use rcu_read_lock and rcu_read_unlock to sync,that means we MUST handle
		*the process until we release the rcu lock.
		*/
		if(skb->pkt_type == IEEE80211_SDATA_QUEUE_TYPE_FRAME){
			struct ieee80211_rx_status *rx_status = (struct ieee80211_rx_status *) skb->cb;
			switch (fc & IEEE80211_FCTL_STYPE) {
			case IEEE80211_STYPE_ASSOC_REQ:
				reassoc = 0;
				rma = ieee80211_ap_sme_rx_mgmt_assoc(sdata,mgmt,skb->len,rx_status,reassoc,&status);
				break;
			case IEEE80211_STYPE_REASSOC_REQ:
				reassoc = 1;				
				rma = ieee80211_ap_sme_rx_mgmt_assoc(sdata,mgmt,skb->len,rx_status,reassoc,&status);
				break;
			case IEEE80211_STYPE_DISASSOC:
				rma = ieee80211_ap_sme_rx_mgmt_disassoc(sdata,mgmt,skb->len);
				break;
			case IEEE80211_STYPE_PROBE_REQ:
				rma = RX_MGMT_NONE;
				break;
			case IEEE80211_STYPE_AUTH:
				rma = ieee80211_ap_sme_rx_mgmt_authen(sdata,mgmt,skb->len,rx_status,&status);
				break;
			case IEEE80211_STYPE_DEAUTH:
				rma = ieee80211_ap_sme_rx_mgmt_deauth(sdata,mgmt,skb->len);
				break;
			default:
				rma = RX_MGMT_NONE;
				break;
			}
		}
		else if (skb->pkt_type == IEEE80211_SDATA_QUEUE_SEND_STATUS){
			switch (fc & IEEE80211_FCTL_STYPE) {
			case IEEE80211_STYPE_ASSOC_RESP:
			case IEEE80211_STYPE_REASSOC_RESP:
				rma = RX_MGMT_REP_ASSOC_SENDED;
				break;
			case IEEE80211_STYPE_DISASSOC:
				rma = RX_MGMT_REP_DISASSOC_SENDED;
				break;
			case IEEE80211_STYPE_AUTH:
				rma = RX_MGMT_REP_AHTEN_SENDED;
				break;
			case IEEE80211_STYPE_DEAUTH:
				rma = RX_MGMT_REP_DEAHTEN_SENDED;
				break;
			default:
				rma = RX_MGMT_NONE;
				break;
			}
		}
		switch (rma) {
		case RX_MGMT_NONE:
			/* no action */
			atbm_dev_kfree_skb(skb);
			break;
		case RX_MGMT_REP_AHTEN_SEND:
			ieee80211_ap_sme_tx_mgmt_authen(sdata,status,skb);
			break;
		case RX_MGMT_REP_ASSOC_SEND:
			ieee80211_ap_sme_tx_mgmt_assoc_resp(sdata,status,skb,reassoc);
			break;
		case RX_MGMT_REP_AHTEN_SENDED:
			ieee80211_ap_sme_tx_mgmt_authen_status(sdata,skb);
			break;
		case RX_MGMT_REP_ASSOC_SENDED:
			ieee80211_ap_sme_tx_mgmt_assoc_status(sdata,skb);
			break;
		case RX_MGMT_REP_DEAHTEN_SENDED:
		case RX_MGMT_REP_DISASSOC_SENDED:
			ieee80211_ap_sme_deauthen_and_disassoc_send_status(sdata,skb);
			break;
		default:
			WARN(1, "unexpected: %d", rma);
			break;
		}
	}
	mutex_unlock(&sdata->ap_sme_mlme_lock);
}

static void ieee80211_ap_sme_event_work(struct atbm_work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data, ap_sme_event_work);
	struct ieee80211_local *local = sdata->local;
	struct ap_sme_event *event = NULL;
	struct sta_info * sta = NULL;
	
	if (!ieee80211_sdata_running(sdata)){
		atbm_printk_ap( "%s:sdata not running\n",__func__);
		return;
	}
	mutex_lock(&sdata->ap_sme_event_lock);
	list_for_each_entry(event, &sdata->ap_sme_event, list) {
		switch(event->event){
			case STA_EVENT__ASSOCED:
			{
				struct station_info sinfo;
				u8* associate_ie = NULL;
				
				rcu_read_lock();
				sta = sta_info_get_rx(sdata, event->sta_addr);
				if((sta == NULL)||(sta->associate_ie_len == 0)||(sta->associate_ie == NULL)){
					rcu_read_unlock();
					break;
				}

				associate_ie = atbm_kmalloc(sta->associate_ie_len,GFP_KERNEL);
				if(associate_ie == NULL){
					rcu_read_unlock();
					break;
				}
				memcpy(associate_ie,sta->associate_ie,sta->associate_ie_len);
				memset(&sinfo,0,sizeof(struct station_info));
				atbm_printk_ap( "%s:notify 80211 new sta(%pM)\n",__func__,event->sta_addr);
				sinfo.assoc_req_ies = associate_ie;
				sinfo.assoc_req_ies_len = sta->associate_ie_len;
				sinfo.generation = ++local->sta_generation;
				sinfo.filled = STATION_INFO_ASSOC_REQ_IES;
				if(sta->associate_ie){
					atbm_kfree(sta->associate_ie);
					sta->associate_ie = NULL;
					sta->associate_ie_len = 0;
				}
				rcu_read_unlock();
				cfg80211_new_sta(sdata->dev, (const u8*)event->sta_addr, &sinfo, GFP_KERNEL);
				if(associate_ie)
					atbm_kfree(associate_ie);
				break;
			}
			case STA_EVENT__DEAUTHEN:
			{
				cfg80211_del_sta(sdata->dev, (const u8*)event->sta_addr, GFP_KERNEL);
			}
			case STA_EVENT__FREE:
			sta_free:
			{
				atbm_printk_ap( "STA_EVENT__FREE+++++++\n");
				sta_info_destroy_addr(sdata,event->sta_addr);
				atbm_printk_ap( "STA_EVENT__FREE--------\n");
				break;
			}
			case STA_EVENT__SEND_DEAUTHEN:
			{
				rcu_read_lock();
				sta = sta_info_get_rx(sdata, event->sta_addr);
				if(sta == NULL){
					rcu_read_unlock();
					break;
				}
				if(test_sta_flag(sta,WLAN_STA_DEAUTHENNING)){
					ieee80211_ap_sme_send_deauth(sdata,event->sta_addr,sta);
					rcu_read_unlock();
				}
				else{
					rcu_read_unlock();
					goto sta_free;
				}
				break;
			}
			case STA_EVENT__SEND_DEASSOC:
			{
				break;
			}
			case STA_EVENT__MAX:
				break;
			default:
			{
				break;
			}
		}
	}
	ieee80211_ap_sme_event_free(sdata,&sdata->ap_sme_event);
	mutex_unlock(&sdata->ap_sme_event_lock);
}
#endif
