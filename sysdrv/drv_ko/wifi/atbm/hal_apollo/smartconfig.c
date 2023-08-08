/* -*-C-*-
*******************************************************************************
* altobeam
* Reproduction and Communication of this document is strictly prohibited
* unless specifically authorized in writing by altobeam
******************************************************************************/
#include <linux/skbuff.h>
#include <net/atbm_mac80211.h>
#include <linux/kthread.h>

#include "apollo.h"
#include "wsm.h"
#include "bh.h"
#include "debug.h"
#include "hwio.h"
#include "sbus.h"
#ifdef ATBM_SUPPORT_SMARTCONFIG
int baselen = 0;
short * st_payload_buf= NULL;
static int flag = 0;
#define FLAG_ORIGINAL_VALUE 0
#define FLAG_RECEIVE_BROADCAST 1
#define FLAG_RECEIVE_MULTICAST 2

#define ORIGINAL_NUM_OF_CNT 0
#define WAIT_NUM_OF_CNT 10

//extern struct hmac_configure hmac_cfg;
extern int smartconfig_scan(struct atbm_common *priv);
extern 	int fun_recv_magic(struct atbm_common *hw_priv,short rxdata);
extern int fun_recv_payload(short rxdata, int turn_id);
extern int atbm_smartconfig_stop(struct atbm_common *hw_priv);
int atbm_smart_scan_start(struct atbm_vif *priv, struct wsm_scan *scan)
{
        int ret, i;
        int tmo = 5000;

        struct atbm_common *hw_priv = ABwifi_vifpriv_to_hwpriv(priv);

        for (i = 0; i < scan->numOfChannels; ++i)
                tmo += scan->ch[i].maxChannelTime + 10;

        atomic_set(&hw_priv->scan.in_progress, 1);
		#ifdef CONFIG_PM
        atbm_pm_stay_awake(&hw_priv->pm_state, tmo * HZ / 1000);
		#endif
        ret = wsm_scan(hw_priv, scan, priv->if_id);
        if (unlikely(ret)) {
                atomic_set(&hw_priv->scan.in_progress, 0);
        }
        return ret;
}
void atbm_smart_scan_work(struct atbm_work_struct *work)
{
	struct atbm_common *hw_priv = container_of(work,
                                                struct atbm_common,
                                                scan.smartwork);

	int i;
	struct atbm_vif *priv;
	//struct atbm_vif *vif;

	//const u32 ProbeRequestTime  = 2;
    //const u32 ChannelRemainTime = 15;
	
    	struct wsm_scan scan = {
		.scanType = WSM_SCAN_TYPE_FOREGROUND,
		.scanFlags =0,
		.numOfProbeRequests = 1,
		.probeDelay = 100,
		.numOfChannels = 14,		
	};
	
	priv = __ABwifi_hwpriv_to_vifpriv(hw_priv, hw_priv->scan.if_id);

        /*TODO: COMBO: introduce locking so vif is not removed in meanwhile */

	if (!priv) {
	 	atbm_printk_scan("%s:[SCAN] interface removed\n",__func__);
	    return ;
	}
		
	hw_priv->scan.status = 0;
	hw_priv->scan.if_id = 0;
	scan.maxTransmitRate = WSM_TRANSMIT_RATE_1;

	mutex_lock(&hw_priv->conf_mutex);
	if (hw_priv->scan.direct_probe)
		scan.maxTransmitRate = WSM_TRANSMIT_RATE_6;
	
	scan.band =  WSM_PHY_BAND_2_4G;
	if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA) {
		scan.scanType = WSM_SCAN_TYPE_BACKGROUND;
		scan.scanFlags = WSM_SCAN_FLAG_FORCE_BACKGROUND;
		if (priv->if_id)
			scan.scanFlags |= WSM_FLAG_MAC_INSTANCE_1;
		else
			scan.scanFlags &= ~WSM_FLAG_MAC_INSTANCE_1;
	}
	scan.autoScanInterval = (0xba << 24)|(120 * 1024); /* 30 seconds, -70 rssi */
	scan.numOfSSIDs = 1;
	scan.ssids =  &hw_priv->scan.ssids[0];

	//if scan_smartconfig scan not send probereq frame
	if(hw_priv->scan.scan_smartconfig){
		scan.numOfProbeRequests = 0; 
		scan.scanFlags |= WSM_FLAG_START_SMARTCONFIG; 
		
	}
	scan.ch = atbm_kzalloc(sizeof(struct wsm_scan_ch[scan.numOfChannels]),GFP_KERNEL);
	if (!scan.ch) {
		hw_priv->scan.status = -ENOMEM;
		goto fail;
	}
	for (i = 0; i < scan.numOfChannels; i++) {
		if(hw_priv->scan.scan_smartconfig){
			scan.ch[i].minChannelTime = hw_priv->st_cfg.magic_time;
			scan.ch[i].maxChannelTime = hw_priv->st_cfg.magic_time+5;
		}
		else {
			scan.ch[i].minChannelTime = 45;
			scan.ch[i].maxChannelTime = 75;
		}
		scan.ch[i].number =i+1;//atbm_band_2ghz.channels[i].hw_value;
		scan.ch[i].txPowerLevel =30; //atbm_band_2ghz.channels[i].max_power;
	}

	hw_priv->scan.status = atbm_smart_scan_start(priv, &scan);
	if (WARN_ON(hw_priv->scan.status))
           goto fail;

	atbm_kfree(scan.ch);
	mutex_unlock(&hw_priv->conf_mutex);
	return ;
fail:
	mutex_unlock(&hw_priv->conf_mutex);
        atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartwork);
        return ;
}
void atbm_smart_setchan_work(struct atbm_work_struct *work)
{	
	struct atbm_common *hw_priv = container_of(work,
                                                struct atbm_common, 
                                                scan.smartsetChanwork);
		struct wsm_set_chantype arg = {
		.band = 0,			//0:2.4G,1:5G
		.flag = BIT(WSM_SET_CHANTYPE_FLAGS__SMARTCONFIG),			//no use
		.channelNumber = hw_priv->st_configchannel,	// channel number
		.channelType = NL80211_CHAN_HT20,	// channel type
		};
		atbm_printk_smt("hw_priv->st_configchannel = %d flag %x\n", hw_priv->st_configchannel,arg.flag);
		wsm_set_chantype_func(hw_priv,&arg,0);
}


void atbm_smart_stop_work(struct atbm_work_struct *work)
{
	struct atbm_common *hw_priv = container_of(work,
                                                struct atbm_common, 
                                                scan.smartstopwork);

	//printk("%s:%d\n",__func__,__LINE__);
	struct wsm_reset reset;

	hw_priv->st_status = CONFIG_ST_IDLE;
	hw_priv->scan.scan_smartconfig = 0;
	reset.reset_statistics = true;
	atbm_del_timer_sync(&hw_priv->smartconfig_expire_timer);  
	if(st_payload_buf){
		 atbm_kfree(st_payload_buf);
		 st_payload_buf= NULL;
	}
	wsm_reset(hw_priv,&reset,0);
	return ;

}
void smartconfig_scan_start(struct atbm_common *hw_priv)
{
	//printk("%s %d\n", __FUNCTION__, __LINE__);
	struct atbm_vif *priv = ABwifi_hwpriv_to_vifpriv(hw_priv,hw_priv->scan.if_id);
	if (unlikely(!priv)) {
		WARN_ON(1);
		return;
	}
	atbm_priv_vif_list_read_unlock(&priv->vif_lock);

	hw_priv->scan_ret.len = 0;	
	priv->scan_expire = 2;
	priv->scan_no_connect_back = 1;
	priv->scan_no_connect = 1;
	/*start smartconfig*/
	hw_priv->scan.scan_smartconfig = 1;
	hw_priv->st_status = CONFIG_ST_START;
	baselen=0;
	if(st_payload_buf==NULL){
		st_payload_buf = atbm_kmalloc(sizeof(short)*ST_PAYLOAD_BUF_SZ,GFP_KERNEL | GFP_DMA);
	}
	else {
		memset(st_payload_buf,0,sizeof(short)*ST_PAYLOAD_BUF_SZ);
	}
	atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartwork);
	hw_priv->st_status = CONFIG_ST_GET_MAGIC;
	//printk("%s:%d\n",__func__,__LINE__);

}
int smartconfig_start(struct atbm_common *hw_priv,struct smartconfig_config * st_cfg)
{	
	//printk("%s:%d\n",__func__,__LINE__);
	//int Istate=0;
	if(st_cfg == NULL){
		atbm_printk_err("unsupport smartconfig st_cfg NULL!");
		return CONFIG_ST_STARTCONFIG_ERR;
	}
	if((st_cfg->magic_cnt<1)||(st_cfg->magic_cnt>10)){
		atbm_printk_err("unsupport smartconfig imagic_cnt %d!",st_cfg->magic_cnt);
		return CONFIG_ST_STARTCONFIG_ERR;
	}
	if((st_cfg->magic_time<20)||(st_cfg->magic_time>500)){
		atbm_printk_err("unsupport smartconfig magic_time!");
		return CONFIG_ST_STARTCONFIG_ERR;
	}
	if((st_cfg->payload_time<500)||(st_cfg->payload_time>30000)){
		atbm_printk_err("unsupport smartconfig payload_time!");
		return CONFIG_ST_STARTCONFIG_ERR;
	}
	 memcpy(&hw_priv->st_cfg,st_cfg,sizeof(struct smartconfig_config ));
	 /*disconnect AP*/
	// AT_WDisConnect(NULL);
	 atbm_mod_timer(&hw_priv->smartconfig_expire_timer,st_cfg->payload_time + st_cfg->magic_time*14);
	 atbm_printk_err("st_cfg->payload_time + st_cfg->magic_time*14 = %d\n", (st_cfg->payload_time + st_cfg->magic_time*14));
	 if(hw_priv->scan_ret.info){
		spin_lock_bh(&hw_priv->spinlock_smart);
		hw_priv->scan_ret.info = NULL;
		hw_priv->scan_ret.len =0;
		spin_unlock_bh(&hw_priv->spinlock_smart);
	 }	
	 smartconfig_scan_start(hw_priv);
	 return CONFIG_ST_START;
}

int atbm_smartconfig_start(struct atbm_common *hw_priv,int enable)
{	
	if (enable){
		atbm_printk_smt("%s %d\n", __func__, hw_priv->scan.scan_smartconfig);
		if(hw_priv->scan.scan_smartconfig){
			atbm_printk_smt("scan_smartconfig now!please try later!\n");
			return 0 ;
	   }else
	   {
		struct smartconfig_config st_cfg = {
		.type = CONFIG_TP_ATBM_SMART,
		.magic_cnt = 1,
		.magic_time = 50,
		.payload_time = 8000,
		};
	    smartconfig_start(hw_priv,&st_cfg);
	   }
	}else{
		atbm_smartconfig_stop(hw_priv);
	}

	return 0;
}
int atbm_smartconfig_stop(struct atbm_common *hw_priv)
{	
	atbm_printk_smt("%s:%d\n",__func__,__LINE__);
	if(hw_priv->scan.scan_smartconfig){
		smartconfig_stop(hw_priv);
   }
	return 0;
}
/*cnt : 2 ~ 100*/
int smartconfig_magic_channel_cnt(int cnt)
{
	return 0;
}

/*ms : 20ms ~ 1000ms*/
int smartconfig_magic_channel_timeout(int ms)
{
	return 0;
}
/*ms : 500ms ~ 10000ms*/
int smartconfig_payload_timeout(int ms)
{
	return 0;
}
int smartconfig_stop(struct atbm_common *hw_priv)
{
	atbm_printk_smt("%s:%d\n",__func__,__LINE__);
	atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartstopwork);
	return hw_priv->st_status;
}
int smartconfig_status(struct atbm_common *hw_priv)
{
	return hw_priv->st_status;
}

int smartconfig_start_timer_func(struct atbm_common *hw_priv )
{		
	atbm_printk_smt("smartconfig timeout\n");

	if(hw_priv->scan.scan_smartconfig){
		//cancel smartconfig
		atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartstopwork);
		hw_priv->st_status = CONFIG_ST_DONE_FAIL;
	}
	return 1;
}

int smartconfig_magic_scan_done(struct atbm_common *hw_priv)
{
	//printk("smartconfig_magic_scan_done %d\n",hw_priv->st_status);

	if(hw_priv->st_status == CONFIG_ST_SWITCH_PAYLOAD_CH ){
		atbm_printk_smt("st_configchannel %d\n",hw_priv->st_configchannel);
		atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartsetChanwork);
		hw_priv->st_status = CONFIG_ST_GET_PAYLOAD;
	}
	else {
		//printk("st_status %d\n",hw_priv->st_status);
		if(hw_priv->st_status == CONFIG_ST_GET_MAGIC){
			//may need start scan again
			atomic_set(&hw_priv->scan.in_progress, 0);
			smartconfig_scan_start(hw_priv);
	    }
	}
	return 0;
}


const u8 atbm_smart_config_magic_mac[4][6] ={
		{0x01,0x00,0x5e,0x7f,0xff,0x01},//magic
		{0x01,0x00,0x5e,0x7f,0xff,0x02}, //checksum
		{0x01,0x00,0x5e,0x7f,0xff,0x03}, //payload
		{0x01,0x00,0x5e,0x7f,0xff,0xfa} //payload
};

int atbm_ieee80211_build_preq_ies(struct atbm_vif *priv,u8 *buffer,
			     const u8 *ie, size_t ie_len,
			     u8 channel)
{
	u8 *pos;

	pos = buffer;

	/* SSID */
	*pos++ = WLAN_EID_SSID;
	*pos++ = 0;
	return pos - buffer;
}

struct sk_buff *atbm_ieee80211_build_probe_req(struct atbm_vif *priv, 
					  u8 *dst,
					  const u8 *ie, size_t ie_len)
{
	struct atbm_common * hw_priv = priv->hw_priv;
	struct sk_buff *skb=NULL;
	struct atbm_ieee80211_mgmt *mgmt;
	//size_t buf_len;
	//u8 *buf;
	struct ieee80211_hdr_3addr *hdr;
	 
	skb = atbm_dev_alloc_skb(1024);
	if (!skb)
		return NULL;
	
	hdr = (struct ieee80211_hdr_3addr *) atbm_skb_put(skb, sizeof(*hdr));
	//printk("hmac:build_probe_req ++<%p>\n",hdr);
	memset(hdr, 0, sizeof(*hdr));
	hdr->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					 IEEE80211_STYPE_PROBE_REQ);
	memcpy(hdr->addr2, hw_priv->mac_addr, ETH_ALEN);
	hdr->seq_ctrl  = 0;
	hdr->duration_id = 0;
	if (dst) {
		mgmt = (struct atbm_ieee80211_mgmt *) hdr;
		memcpy(mgmt->da, dst, ETH_ALEN);
		memcpy(mgmt->bssid, dst, ETH_ALEN);
	}
	else {
		memset(hdr->addr1, 0xff, ETH_ALEN);
		memset(hdr->addr3, 0xff, ETH_ALEN);
	}
	//buf = (u8 *)(hdr + 1);

	//buf_len = atbm_ieee80211_build_preq_ies(priv, buf, ie, ie_len,
	//				  hw_priv->st_configchannel);

//	atbm_skb_put(skb, buf_len);
	
	IEEE80211_SKB_CB(skb)->flags = IEEE80211_TX_INTFL_DONT_ENCRYPT;

	return skb;
}


struct sk_buff * atbm_ieee80211_send_probe_req(struct atbm_vif *priv, u8 *dst,
			      const u8 *ie, size_t ie_len, bool no_cck)
{
	struct sk_buff *skb;

	skb = atbm_ieee80211_build_probe_req(priv, dst,ie, ie_len);
	if (skb) {
		if (no_cck)
			IEEE80211_SKB_CB(skb)->flags |=
				IEEE80211_TX_CTL_NO_CCK_RATE;
	}
	return skb;
}
int smartconfig_scan(struct atbm_common *hw_priv)
{	
	//printk("%s:%d\n",__func__,__LINE__);
	int ret;
	struct atbm_vif *priv;
	struct wsm_template_frame frame = {
		.frame_type = WSM_FRAME_TYPE_PROBE_REQUEST,
	};
	priv = ABwifi_hwpriv_to_vifpriv(hw_priv, hw_priv->scan.if_id);
	if (unlikely(!priv)) {
		atbm_printk_err("%s:%d\n",__func__,__LINE__);
		return 0;
	}
	atbm_priv_vif_list_read_unlock(&priv->vif_lock);
	//printk("hmac:sta_scan ++ scan_expire %ds\n",priv->scan_expire);
	if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA)
		return -EOPNOTSUPP;
	frame.skb = atbm_ieee80211_send_probe_req(priv,NULL,NULL,0,0);
	//printk("%s:%d\n",__func__,__LINE__);

	if (!frame.skb)
		return -ENOMEM;

	ret = wsm_set_template_frame(hw_priv, &frame,
			priv->if_id);
	atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartwork);

	if (frame.skb)
		atbm_dev_kfree_skb(frame.skb);

	return ret;
}
#if 0
int start_connect_ap(struct atbm_vif*priv,u8 *essid,int essid_len,u8 *key,int key_len,int key_mgmt,int keyid)
{
	//struct wpa_supplicant *wpa_s = (struct wpa_supplicant *)priv->appdata;
	struct hmac_configure *config = priv->hw_priv->config;
	
	//int essid_len = strlen((const char*)essid);
	//int key_len = strlen((const char*)key);

	//wifi_printk(WIFI_WPA,"wpa  (%s) key(%d)\n",essid,key_mgmt);
	priv->scan_no_connect_back = priv->scan_no_connect = 0;
	priv->auto_connect_when_lost = 1;

	if(essid_len>IEEE80211_MAX_SSID_LEN)	{
		essid_len = IEEE80211_MAX_SSID_LEN;
	}
	
	if(key_len>32)	{
		key_len = 32;
	}
	
	if(key != NULL){
		memcpy(config->password,key,key_len);
		config->password[key_len] = 0;
		config->password_len = key_len;
	}
	if(essid!= NULL){
		memcpy(config->ssid,essid,essid_len);
		os_memcpy(priv->ssid,config->ssid ,essid_len);
		config->ssid[essid_len] = 0;
		priv->ssid[essid_len] = 0;
		config->ssid_len = essid_len;
		priv->ssid_length = config->ssid_len;
	}

	if(key_mgmt == KEY_WPA)
	{
		config->key_mgmt = WPA_KEY_MGMT_PSK;
		config->wpa = WPA_PROTO_WPA;
		config->auth_alg = WLAN_AUTH_OPEN;
		config->pairwise_cipher = WPA_CIPHER_TKIP;
		config->group_cipher = WPA_CIPHER_TKIP;
	}
	else if(key_mgmt == KEY_WPA2)
	{
		config->key_mgmt = WPA_KEY_MGMT_PSK;
		config->wpa = WPA_PROTO_RSN;
		config->auth_alg = WLAN_AUTH_OPEN;
		config->pairwise_cipher = WPA_CIPHER_CCMP;
		config->group_cipher = WPA_CIPHER_CCMP;
	}
	else if((key_mgmt == KEY_WEP) ||(key_mgmt == KEY_WEP_SHARE))
	{
		if((key_len == 5)||(key_len == 13))
		{
			config->auth_alg = (key_mgmt == KEY_WEP)? WLAN_AUTH_OPEN:WLAN_AUTH_SHARED_KEY;
			config->group_cipher = (key_len == 5) ? WPA_CIPHER_WEP40 : WPA_CIPHER_WEP104;
			wpa_common_install_wepkey(priv,(char *)key,config->group_cipher,keyid);

			wifi_printk(WIFI_WPA,"wpa scan (%s),auth(%x),gcipher(%x)\n",
									  priv->ssid,config->auth_alg,config->group_cipher);
		}

	} 
	else
	{
		config->key_mgmt = WPA_KEY_MGMT_NONE;
		config->wpa = 0;
		config->auth_alg = WLAN_AUTH_OPEN;
		config->pairwise_cipher = WPA_CIPHER_NONE;
		config->group_cipher = WPA_CIPHER_NONE;		
		if((key_len > 0)||(key_mgmt != KEY_NONE)){
			wifi_printk(WIFI_WPA,"<ERROR> connect_ap keymgm(%d) keyl(%d) ERR!\n",
									  key_mgmt,key_len);
			return -1;
		}
	}
	if (config->key_mgmt & WPA_KEY_MGMT_PSK) {
		
		//pbkdf2_sha1((const char*)ssid->psk, (const char*)ssid->ssid, ssid->ssid_len, 8, wpa_s->wpa->pmksa[0].id, PMKID_LEN);
		config->psk_set = 0;
		if(config->psk_set){
			memcpy(config->psk,config->password,PMK_LEN);
		}
		else {
			wifi_printk(WIFI_WPA,"psk pwd(%s),ssid(%s)\n",config->password,config->ssid);
			pbkdf2_sha1((const char*)config->password, (const char*)config->ssid, config->ssid_len,4096,config->psk, PMK_LEN);	
		}
		config->psk_set = 1;
	}
	
	smartconfig_scan(priv);
	//os_free(priv->extra_ie);
	priv->extra_ie = NULL;
	priv->extra_ie_len = 0;
	return 0;
}
#endif
int smartconfig_step_1(struct atbm_common *hw_priv,struct sk_buff *skb,int channel )
{
	int len  = skb->len;
	char * data  = skb->data;
	static int CNT_BROAD = 0;

	//printk("step_1_rx len %d %x\n",len,data[4+12]);
	
	//add code to get magic
	if((memcmp(atbm_smart_config_magic_mac[0],&data[4+12],6)) 
		&& (memcmp(atbm_smart_config_magic_mac[0],&data[4],6))){
		//printk("rxmac %x %x %x %x %x %x\n",data[4+12],data[4+13],data[4+14],data[4+15],data[4+16],data[4+17]);
		return 0;
	}

	if(flag == FLAG_ORIGINAL_VALUE)
		CNT_BROAD++;

	if((flag == FLAG_ORIGINAL_VALUE) && (!memcmp(atbm_smart_config_magic_mac[0],&data[4],6))){
		flag = FLAG_RECEIVE_BROADCAST;
		CNT_BROAD = ORIGINAL_NUM_OF_CNT;
		atbm_printk_smt("smartconfig:FLAG_RECEIVE_BROADCAST \n");
	}
	else if((flag == FLAG_ORIGINAL_VALUE) && (CNT_BROAD > WAIT_NUM_OF_CNT)){
		flag = FLAG_RECEIVE_MULTICAST;
		CNT_BROAD = ORIGINAL_NUM_OF_CNT;
		atbm_printk_smt("smartconfig:FLAG_RECEIVE_MULTICAST \n");
	}

	//printk("******** counter: %d********\n", CNT_BROAD);
	if((flag == FLAG_ORIGINAL_VALUE) && (CNT_BROAD <= WAIT_NUM_OF_CNT))
		return 0;

	//fun_recv_magic(hw_priv,len);

	if((flag == FLAG_RECEIVE_BROADCAST) && (!memcmp(atbm_smart_config_magic_mac[0],&data[4],6))){

		if(fun_recv_magic(hw_priv,len)/*magic done*/){
			hw_priv->st_status = CONFIG_ST_SWITCH_PAYLOAD_CH;
			hw_priv->st_configchannel = channel;
			atbm_printk_smt("FLAG_RECEIVE_BROADCAST\n");
		}
	}
	else if((flag == FLAG_RECEIVE_MULTICAST) && (!memcmp(atbm_smart_config_magic_mac[0],&data[4+12],6))){
	
		if(fun_recv_magic(hw_priv,len)/*magic done*/){
			hw_priv->st_status = CONFIG_ST_SWITCH_PAYLOAD_CH;
			hw_priv->st_configchannel = channel;
			//printk("CONFIG_ST_SWITCH_PAYLOAD_CH\n");
		}
	}

	//fun_recv_magic(hw_priv,len);
	//
	//if(fun_recv_magic(hw_priv,len)/*magic done*/){
		//hw_priv->st_status = CONFIG_ST_SWITCH_PAYLOAD_CH;
		//hw_priv->st_configchannel = channel;
	//}
	return 0;
}

int smartconfig_step_2(struct atbm_common *hw_priv,struct sk_buff *skb)
{
	int turn_id = 0;
	
	int len  = skb->len;
	char * data  = skb->data;
	
	struct atbm_vif *priv = ABwifi_hwpriv_to_vifpriv(hw_priv,
					hw_priv->scan.if_id);
	if (unlikely(!priv)) {
		WARN_ON(1);
		return 0;
	}
	atbm_priv_vif_list_read_unlock(&priv->vif_lock);
	
	//add code to get payload
	if((memcmp(atbm_smart_config_magic_mac[1],&data[4+12],5))
		&& (memcmp(atbm_smart_config_magic_mac[1],&data[4],5))){
			//printk("rxmac %x %x %x %x %x %x\n",data[4+12],data[4+13],data[4+14],data[4+15],data[4+16],data[4+17]);
			return -1;
	}	
	if((!memcmp(atbm_smart_config_magic_mac[3], &data[4+12], 6))){
		//printk("rxmac %x %x %x %x %x %x\n",data[4+12],data[4+13],data[4+14],data[4+15],data[4+16],data[4+17]);
		return -1;
	}

	if((!memcmp(atbm_smart_config_magic_mac[3], &data[4], 6)))
		return -1;
	
	atbm_printk_smt("data: %x %x %x %x %x %x\n",data[4],data[4+1],data[4+2],data[4+3],data[4+4],data[4+5]);	
	//printk("data: %x %x %x %x %x %x\n",data[4+12],data[4+13],data[4+14],data[4+15],data[4+16],data[4+17]);
	//printk("step_2_rx len %x \n",len);
#if 1
	if(flag == FLAG_RECEIVE_BROADCAST){
		if(memcmp(atbm_smart_config_magic_mac[2],&data[4],5))
			return -1;
		if((!memcmp(atbm_smart_config_magic_mac[2],&data[4],5)) 
			&& (data[4+5] >= atbm_smart_config_magic_mac[2][5]))
		{
			turn_id = data[4+5] - atbm_smart_config_magic_mac[2][5];
			//iot_printf("1\n");
			atbm_printk_smt("receive broadcast\n");
		}
	}
	else if(flag == FLAG_RECEIVE_MULTICAST){
		if(memcmp(atbm_smart_config_magic_mac[2],&data[4+12],5))
			return -1;
		if((!memcmp(atbm_smart_config_magic_mac[2],&data[4+12],5)) 
			&& (data[4+17] >= atbm_smart_config_magic_mac[2][5]))
		{
			turn_id = data[4+17] - atbm_smart_config_magic_mac[2][5];
			//iot_printf("2\n");
			atbm_printk_smt("receive multicast\n");
		}
	}
#else
	if(data[4+17] >= atbm_smart_config_magic_mac[2][5]){
		turn_id = data[4+17] - atbm_smart_config_magic_mac[2][5];
		if(turn_id >= 10)
			return -1;
	}
#endif
		

	atbm_printk_smt("step_2_rx len %d %d,turn_id = %d\n",len,data[4+17],turn_id);

	if( fun_recv_payload((short)(len-baselen+1), turn_id)== CONFIG_ST_DONE_SUCCESS/*payload done*/){
		atbm_printk_smt("SMART_SUCCESS \n");
		//cancel smartconfig
		atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartstopwork);
		hw_priv->st_status = CONFIG_ST_DONE_SUCCESS;
		//connect AP		
		//start_connect_ap(priv,hmac_cfg.ssid,hmac_cfg.ssid_len,hmac_cfg.password,hmac_cfg.password_len,hmac_cfg.key_mgmt,hmac_cfg.key_id );
		//smartconfig_scan(hw_priv);
	}
	
	return 0;
}

int smartconfig_start_rx(struct atbm_common *hw_priv,struct sk_buff *skb,int channel )
{
	if(hw_priv->st_status == CONFIG_ST_GET_MAGIC){
			smartconfig_step_1(hw_priv,skb,channel);
	}
	else if(hw_priv->st_status == CONFIG_ST_GET_PAYLOAD){
			smartconfig_step_2(hw_priv,skb);
	}
	
	return 0;
}
void atbm_smartconfig_expire_timer(unsigned long arg)
{
	//printk("%s:%d\n",__func__,__LINE__);
	struct atbm_common *hw_priv =
		(struct atbm_common *)arg;
	if (atomic_read(&hw_priv->scan.in_progress)){
		atbm_printk_smt("%s:%d\n",__func__,__LINE__);
		//cancel smartconfig
		atbm_hw_priv_queue_work(hw_priv, &hw_priv->scan.smartstopwork);
	}
}
#endif

