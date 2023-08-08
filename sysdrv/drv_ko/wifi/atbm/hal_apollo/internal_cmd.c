#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/dcache.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include "apollo.h"
#include "bh.h"
#include "hwio.h"
#include "wsm.h"
#include "sbus.h"
#include "debug.h"
#include "apollo_plat.h"
#include "sta.h"
#include "ap.h"
#include "scan.h"
#include "internal_cmd.h"
#include "svn_version.h"
static struct atbm_gpio_config atbm_gpio_table[]=
{
	{
		.gpio = 4,
		.flags = 0,
		.fun_ctrl = {.base_addr	= 0x1740000C,.start_bit  = 0,.width		= 4,.val = 0,},
		.pup_ctrl = {.base_addr	= 0x1740000C,.start_bit  = 11,.width	= 1,.val = 0,},
		.pdu_ctrl = {.base_addr	= 0x1740000C,.start_bit  = 12,.width	= 1,.val = 0,},
		.dir_ctrl = {.base_addr	= 0x16800028,.start_bit  = 4,.width		= 1,.val = 0,},
		.out_val  = {.base_addr	= 0x16800024,.start_bit  = 4,.width		= 1,.val = 0,},
		.out_val  = {.base_addr	= 0x16800020,.start_bit  = 4,.width		= 1,.val = 0,},
	},
	{
		.gpio = 20,
		.flags = 0,
		.fun_ctrl = {.base_addr	= 0x1740002C,.start_bit  = 0,.width		= 4,.val = 0,},
		.pup_ctrl = {.base_addr	= 0x1740002C,.start_bit  = 11,.width	= 1,.val = 0,},
		.pdu_ctrl = {.base_addr	= 0x1740002C,.start_bit  = 12,.width	= 1,.val = 0,},
		.dir_ctrl = {.base_addr	= 0x16800028,.start_bit  = 20,.width	= 1,.val = 0,},
		.out_val  = {.base_addr	= 0x16800024,.start_bit  = 20,.width	= 1,.val = 0,},
		.in_val  = {.base_addr	= 0x16800020,.start_bit  = 20,.width	= 1,.val = 0,},
	},
	{
		.gpio = 21,
		.flags = 0,
		.fun_ctrl = {.base_addr	= 0x1740002C,.start_bit  = 16,.width	= 4,.val = 0,},
		.pup_ctrl = {.base_addr	= 0x1740002C,.start_bit  = 27,.width	= 1,.val = 0,},
		.pdu_ctrl = {.base_addr	= 0x1740002C,.start_bit  = 28,.width	= 1,.val = 0,},
		.dir_ctrl = {.base_addr	= 0x16800028,.start_bit  = 21,.width	= 1,.val = 0,},
		.out_val  = {.base_addr	= 0x16800024,.start_bit  = 21,.width	= 1,.val = 0,},
		.in_val  = {.base_addr	= 0x16800020,.start_bit  = 21,.width	= 1,.val = 0,},
	},
	{
		.gpio = 22,
		.flags = 0,
		.fun_ctrl = {.base_addr	= 0x17400030,.start_bit  = 0,.width		= 4,.val = 0,},
		.pup_ctrl = {.base_addr	= 0x17400030,.start_bit  = 11,.width	= 1,.val = 0,},
		.pdu_ctrl = {.base_addr	= 0x17400030,.start_bit  = 12,.width	= 1,.val = 0,},
		.dir_ctrl = {.base_addr	= 0x16800028,.start_bit  = 22,.width	= 1,.val = 0,},
		.out_val  = {.base_addr	= 0x16800024,.start_bit  = 22,.width	= 1,.val = 0,},
		.in_val  = {.base_addr	= 0x16800020,.start_bit  = 22,.width	= 1,.val = 0,},
	},
	{
		.gpio = 23,
		.flags = 0,
		.fun_ctrl = {.base_addr	= 0x17400030,.start_bit  = 16,.width	= 4,.val = 0,},
		.pup_ctrl = {.base_addr	= 0x17400030,.start_bit  = 27,.width	= 1,.val = 0,},
		.pdu_ctrl = {.base_addr	= 0x17400030,.start_bit  = 28,.width	= 1,.val = 0,},
		.dir_ctrl = {.base_addr	= 0x16800028,.start_bit  = 23,.width	= 1,.val = 0,},
		.out_val  = {.base_addr	= 0x16800024,.start_bit  = 23,.width	= 1,.val = 0,},
		.in_val  = {.base_addr	= 0x16800020,.start_bit  = 23,.width	= 1,.val = 0,},
	},
};

#define ATBM_WSM_ADAPTIVE		"set_adaptive"ATBM_SPACE_STR
#define ATBM_WSM_TXPWR_DCXO		"set_txpwr_and_dcxo"ATBM_SPACE_STR
#define ATBM_WSM_TXPWR			"set_txpower"ATBM_SPACE_STR
#define ATBM_WSM_SET_FREQ		"set_freq"ATBM_SPACE_STR"%d"ATBM_SPACE_STR"%d"
#define ATBM_WSM_FIX_RATE		"lmac_rate"ATBM_SPACE_STR"%d"
#define ATBM_WSM_TOP_RATE		"lmac_max_rate"ATBM_SPACE_STR"%d"
#define ATBM_WSM_MIN_RATE		"lmac_min_rate"ATBM_SPACE_STR"%d"
#define ATBM_WSM_SET_RATE_POWER	"set_spec_rate_txpower_mode"ATBM_SPACE_STR"%d"ATBM_SPACE_STR"%d"

#ifdef CONFIG_ATBM_MONITOR_SPECIAL_MAC
#define ATBM_WSM_MONITOR_MAC	"set_sta_monitor"ATBM_SPACE_STR"%d"ATBM_SPACE_STR"%d"ATBM_SPACE_STR"%x,%x,%x,%x,%x,%x"
#endif
#define ATBM_WSM_CMD_LEN		1680
const char *chip_6038  = "6038";
const char *chip_6032  = "6032";
const char *chip_6032i = "6032i";
const char *chip_101B  = "101B";


unsigned char char2Hex(const char chart)
{
	unsigned char ret = 0;
	if((chart>='0')&&(chart<='9')){
		ret = chart-'0';		
	}else if((chart>='a')&&(chart<='f')){
		ret = chart - 'a'+0x0a;		
	}else if((chart>='A')&&(chart<='F')){
		ret = chart - 'A'+0x0a;
	}
	return ret;
}

/*
Func: str2mac
Param: 
	str->string format of MAC address
	i.e. 00:11:22:33:44:55
Return: 
	error -1
	OK 0
*/
int str2mac(char *dst_mac, char *src_str)
{
	int i;
	
	if(dst_mac == NULL || src_str == NULL)
		return -1;

	for(i=0; i<6; i++){
		dst_mac[i] = (char2Hex(src_str[i*3]) << 4) + (char2Hex(src_str[i*3 + 1]));
		atbm_printk_wext("str2mac: %x\n", dst_mac[i]);
	}

	return 0;	
}
/*
	add by yuzhihuang
	
	20220224 xiongmai_question
*/

#include <linux/sched.h>
#include <asm/siginfo.h>
#include <linux/pid_namespace.h>
#include <linux/pid.h>

void send_signal(int sig_num,int user_pid)
{
	   struct task_struct *current_task = NULL;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 60))
       struct kernel_siginfo info;
#else
	   struct siginfo info;
#endif
       int ret;
       

       printk("%s,%d.sending signal %d to owner %d\n",__func__, __LINE__, sig_num, user_pid);

       memset(&info, 0, sizeof(struct siginfo));
       info.si_signo = sig_num;
       info.si_code = 0;
       info.si_int = 1234;
       if (current_task == NULL){
               rcu_read_lock();
               current_task = pid_task(find_vpid(user_pid), PIDTYPE_PID);
               rcu_read_unlock();
       }
       ret = send_sig_info(sig_num, &info, current_task);
       if (ret < 0) {
               printk("error sending signal\n");
       }

}

int ieee80211_set_channel(struct wiphy *wiphy,
				 struct net_device *netdev,
				 struct ieee80211_channel *chan,
				 enum nl80211_channel_type channel_type);

static void atbm_internal_cmd_scan_dump(struct ieee80211_internal_scan_request *scan_req)
{
	int i;
	if(scan_req->n_ssids){
		for(i = 0;i<scan_req->n_ssids;i++){
			atbm_printk_debug("%s: ssid[%s][%d]\n",__func__,scan_req->ssids[i].ssid,scan_req->ssids[i].ssid_len);
		}
	}	
	if(scan_req->n_channels){
		for(i = 0;i<scan_req->n_channels;i++){
			atbm_printk_debug("%s: channel[%d]\n",__func__,scan_req->channels[i]);
		}
	}
	if(scan_req->n_macs){
		for(i = 0;i<scan_req->n_macs;i++){
			atbm_printk_debug("%s: mac[%pM]\n",__func__,scan_req->macs[i].mac);
		}
	}
	atbm_printk_debug("%s: ie_len[%d]\n",__func__,scan_req->ie_len);
}

bool  atbm_internal_cmd_scan_build(struct ieee80211_local *local,struct ieee80211_internal_scan_request *req,
											   u8* channels,int n_channels,struct cfg80211_ssid *ssids,int n_ssids,
											   struct ieee80211_internal_mac *macs,int n_macs)
{
	u8* local_scan_ie;
	u8* scan_ie;
	int ie_len;
	/*
	*use default internal handle
	*/
	req->result_handle = NULL;
	req->priv = NULL;

	req->channels = channels;
	req->n_channels = n_channels;
	
	req->ssids =  ssids;
	req->n_ssids = n_ssids;

	req->macs = macs;
	req->n_macs = n_macs;

	req->no_cck = true;
	
	rcu_read_lock();
	local_scan_ie = rcu_dereference(local->internal_scan_ie);
	ie_len  = local->internal_scan_ie_len;

	if(local_scan_ie && ie_len){
		scan_ie = atbm_kzalloc(ie_len,GFP_ATOMIC);

		if(scan_ie == NULL){
			rcu_read_unlock();
			return false;
		}
		memcpy(scan_ie,local_scan_ie,ie_len);
		req->ies = scan_ie;
		req->ie_len = ie_len;
	}else {
		req->ies = NULL;
		req->ie_len = 0;
	}
	rcu_read_unlock();

	return true;
}
bool atbm_internal_cmd_scan_triger(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_scan_request *req)
{
	struct cfg80211_scan_request *scan_req = NULL;
	struct ieee80211_local *local  = sdata->local;
	u8 n_channels = 0;
	int i;
	struct wiphy *wiphy = local->hw.wiphy;
	u8 index;
	void *pos;
	void *pos_end;
	long status = 20*HZ;
	
	ASSERT_RTNL();
	ieee80211_scan_cancel(local);
	atbm_flush_workqueue(local->workqueue);
	
	mutex_lock(&local->mtx);

	if(!ieee80211_sdata_running(sdata)){
		atbm_printk_scan("%s:%d\n",__func__,__LINE__);
		goto err;
	}
	
	if (local->scan_req)
	{
		atbm_printk_scan("%s:%d\n",__func__,__LINE__);
		goto err;
	}
#ifdef CONFIG_ATBM_SUPPORT_P2P
	if (!list_empty(&local->roc_list))
	{
		goto err;
	}
#endif
	if (!list_empty(&local->work_list)) {
		
		atbm_printk_scan("%s(%s):work_list is not empty,pend scan\n",__func__,sdata->name);
		goto err;
	}
	
	if(atbm_ieee80211_suspend(sdata->local)==true){
		
		atbm_printk_err("ieee80211_scan drop:suspend\n");
		goto err;
	}
	
	if(req->n_channels == 0){
		for (i = 0; i < IEEE80211_NUM_BANDS; i++)
			if (wiphy->bands[i])
				n_channels += wiphy->bands[i]->n_channels;
	}else {
		n_channels = req->n_channels;
	}
	scan_req = atbm_kzalloc(sizeof(*scan_req)
			+ sizeof(*scan_req->ssids) * req->n_ssids
			+ sizeof(*scan_req->channels) * n_channels
			+ req->ie_len + req->n_channels + sizeof(struct ieee80211_internal_mac)*req->n_macs, GFP_KERNEL);
	
	if(scan_req == NULL){
		atbm_printk_scan("%s:atbm_kzalloc scan_req err\n",__func__);
		goto err;
	}
	pos = (void *)&scan_req->channels[n_channels];
	pos_end = (void*)((u8*)pos+sizeof(*scan_req->ssids) * req->n_ssids+
			  req->ie_len + req->n_channels + sizeof(struct ieee80211_internal_mac)*req->n_macs);
	/*
	*set channel
	*/
	if(req->n_channels){
		int freq;
		for (i = 0;i<req->n_channels;i++){
			
			if(req->channels[i] <= 14){
				freq = 2412+(req->channels[i] - 1)*5;
				if(req->channels[i] == 14)
						freq = 2484;
			}else {
				freq = 5000 + (5*req->channels[i]);
			}

			atbm_printk_debug("%s:channel(%d),freq(%d)\n",__func__,req->channels[i],freq);

			scan_req->channels[i] = ieee80211_get_channel(wiphy,freq);

			if(scan_req->channels[i] == NULL){
				goto err;
			}
		}
	}else {
		enum ieee80211_band band;
		i = 0;
		/* all channels */
		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			int j;
			if (!wiphy->bands[band])
				continue;
			for (j = 0; j < wiphy->bands[band]->n_channels; j++) {
				scan_req->channels[i] =  &wiphy->bands[band]->channels[j];
				i++;
			}
		}
	}
	scan_req->n_channels = n_channels;
	/*
	*set ssid
	*/
	if( req->n_ssids){
		scan_req->ssids = (void *)pos;
		for(i=0;i<req->n_ssids;i++){			
			atbm_printk_debug("%s:scan ssid(%s)(%d)\n",__func__,req->ssids[i].ssid,req->ssids[i].ssid_len);
			scan_req->ssids[i].ssid_len = req->ssids[i].ssid_len;
			memcpy(scan_req->ssids[i].ssid,req->ssids[i].ssid,req->ssids[i].ssid_len);
		}
		pos = scan_req->ssids+req->n_ssids;
	}
	scan_req->n_ssids = req->n_ssids;
	/*
	*set macs
	*/
	local->internal_scan.req.n_macs = req->n_macs;	
	if(req->n_macs){
		local->internal_scan.req.macs = pos;
		memcpy(local->internal_scan.req.macs, req->macs,sizeof(struct ieee80211_internal_mac)*req->n_macs);
		pos = local->internal_scan.req.macs + req->n_macs;
	}
	/*
	*set ie
	*/
	if (req->ie_len) {		
		scan_req->ie = (void *)pos;
		memcpy((void*)scan_req->ie,req->ies,req->ie_len);
		scan_req->ie_len = req->ie_len;
		pos = (u8*)scan_req->ie+req->ie_len;
	}

	/*
	*set channel
	*/
	if(req->channels){
		local->internal_scan.req.channels = pos;
		memcpy(local->internal_scan.req.channels,req->channels,req->n_channels);
	    pos = local->internal_scan.req.channels+req->n_channels;
	}
	WARN_ON(pos != pos_end);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	for (i = 0; i < IEEE80211_NUM_BANDS; i++)
		if (wiphy->bands[i])
			scan_req->rates[i] =
				(1 << wiphy->bands[i]->n_bitrates) - 1;
		
	scan_req->no_cck = req->no_cck;
#endif
	
	scan_req->wiphy = wiphy;

	local->internal_scan.req.n_channels = req->n_channels;	
	local->internal_scan.req.ies = (u8*)scan_req->ie;
	local->internal_scan.req.ie_len = scan_req->ie_len;
	local->internal_scan.req.ssids = scan_req->ssids;
	local->internal_scan.req.n_ssids = scan_req->n_ssids;
	memcpy(local->internal_scan.req.bssid,req->bssid,6);

	local->internal_scan.req.req_flags = req->req_flags;
	
	rcu_assign_pointer(local->internal_scan.req.result_handle,req->result_handle);
	rcu_assign_pointer(local->internal_scan.req.priv,req->priv);

	atbm_common_hash_list_init(local->internal_scan.mac_hash_list,IEEE80211_INTERNAL_SCAN_HASHENTRIES);
	
	for(index = 0;index<local->internal_scan.req.n_macs;index++){
		int hash_index = atbm_hash_index(local->internal_scan.req.macs[index].mac,6,IEEE80211_INTERNAL_SCAN_HASHBITS);
		struct hlist_head *hlist = &local->internal_scan.mac_hash_list[hash_index];
		hlist_add_head(&local->internal_scan.req.macs[index].hnode,hlist);
	}
	
	atbm_internal_cmd_scan_dump(&local->internal_scan.req);
	
	if(ieee80211_internal_scan_triger(sdata,scan_req) == false){
		atbm_printk_scan("%s scan triger err\n",__func__);
		
		for(index = 0;index<local->internal_scan.req.n_macs;index++){
			hlist_del(&local->internal_scan.req.macs[index].hnode);
		}
		rcu_assign_pointer(local->internal_scan.req.result_handle,NULL);
		rcu_assign_pointer(local->internal_scan.req.priv,NULL);
		memset(&local->internal_scan.req,0,sizeof(struct ieee80211_internal_scan_sta));
		goto err;
	}	
	if(local->scan_req_wrap.flags & IEEE80211_SCAN_REQ_SPILT){
		status = 60*HZ;
	}
	mutex_unlock(&local->mtx);

	status = wait_event_timeout(local->internal_scan_wq,atomic_read(&local->internal_scan_status) != IEEE80211_INTERNAL_SCAN_STATUS__WAIT,status);

	if(status == 0){
		return false;
	}

	atbm_printk_debug("%s: status(%ld)\n",__func__,status);

	if(atomic_read(&local->internal_scan_status) == IEEE80211_INTERNAL_SCAN_STATUS__ABORT)
		return false;
	
	return true;
err:
	if(scan_req)
		atbm_kfree(scan_req);
	mutex_unlock(&local->mtx);

	return false;
}

bool atbm_internal_cmd_stainfo(struct ieee80211_local *local,struct ieee80211_internal_sta_req *sta_req)
{
	struct ieee80211_internal_sta_info stainfo;
	struct sta_info *sta;
	u8 index = 0;
	struct hlist_head *hhead;
	struct hlist_node *node;
	struct ieee80211_internal_mac *mac_node;
	unsigned int hash_index = 0;
	bool (__rcu *sta_handle)(struct ieee80211_internal_sta_info *stainfo,void *priv);	
	struct hlist_head atbm_sta_mac_hlist[ATBM_COMMON_HASHENTRIES];

	
	memset(&stainfo,0,sizeof(struct ieee80211_internal_sta_info));	
	
	WARN_ON(sta_req->sta_handle == NULL);
	BUG_ON((sta_req->n_macs != 0)&&(sta_req->macs == NULL));
	
	atbm_common_hash_list_init(atbm_sta_mac_hlist,ATBM_COMMON_HASHENTRIES);

	for(index = 0;index<sta_req->n_macs;index++){
		hash_index = atbm_hash_index(sta_req->macs[index].mac,
								 6,ATBM_COMMON_HASHBITS);

		hhead = &atbm_sta_mac_hlist[hash_index];
		hlist_add_head(&sta_req->macs[index].hnode,&atbm_sta_mac_hlist[hash_index]);
	}
	
	mutex_lock(&local->sta_mtx);
	sta_handle = rcu_dereference(sta_req->sta_handle);
	list_for_each_entry_rcu(sta, &local->sta_list, list) {
		struct ieee80211_channel_state *chan_state = ieee80211_get_channel_state(local, sta->sdata);

		if(sta->sdata->vif.type != sta_req->type){
			continue;
		}
		
		if(sta->uploaded == false){
			continue;
		}
		
		if(sta->dead == true){
			continue;
		}
		
		if(sta_req->n_macs){
			u8 sta_needed = false;
			
			hash_index = atbm_hash_index(sta->sta.addr,6,ATBM_COMMON_HASHBITS);
			hhead = &atbm_sta_mac_hlist[hash_index];
			hlist_for_each(node,hhead){
				mac_node = hlist_entry(node,struct ieee80211_internal_mac,hnode);
				if (memcmp(mac_node->mac,sta->sta.addr,6) == 0){
					sta_needed = true;
					break;
				}
			}
			
			if(sta_needed == false){
				continue;
			}
		}
		stainfo.sdata = sta->sdata;
		
		if(sta_req->req_flag&IEEE80211_INTERNAL_STA_FLAGS_CHANNEL){
			stainfo.channel = channel_hw_value(chan_state->oper_channel);
			stainfo.channel_type = !!(test_sta_flag(sta,WLAN_STA_40M_CH)&&!test_sta_flag(sta,WLAN_STA_40M_CH_SEND_20M));
		}
		
		if(sta_req->req_flag&IEEE80211_INTERNAL_STA_FLAGS_SIGNAL){
			stainfo.signal = sta->last_signal2;
			stainfo.avg_signal = (s8) -atbm_ewma_read(&sta->avg_signal2);
		}
		
		if(sta_req->req_flag&IEEE80211_INTERNAL_STA_FLAGS_TXRXBYTE){
			stainfo.rx_bytes = sta->rx_bytes;
			stainfo.tx_bytes = sta->tx_bytes;
		}

		if(sta_req->req_flag&IEEE80211_INTERNAL_STA_FLAGS_TOPRATE){			
			struct atbm_common *hw_priv = (struct atbm_common *)local->hw.priv;
			struct atbm_vif *priv = (struct atbm_vif *)sta->sdata->vif.drv_priv;
			if(sta->sdata->vif.type == NL80211_IFTYPE_STATION){				
				wsm_read_mib(hw_priv, WSM_MIB_ID_GET_RATE, &stainfo.top_rate, sizeof(unsigned int), priv->if_id);
			}else if(sta->sdata->vif.type == NL80211_IFTYPE_AP){
				struct atbm_sta_priv *sta_priv = (struct atbm_sta_priv *)&sta->sta.drv_priv;
				u8 sta_id = (u8)sta_priv->link_id;
				if(sta_id != 0){					
					wsm_write_mib(hw_priv, WSM_MIB_ID_GET_RATE, &sta_id, 1,priv->if_id);
					wsm_read_mib(hw_priv, WSM_MIB_ID_GET_RATE, &stainfo.top_rate, sizeof(unsigned int), priv->if_id);
				}
			}
			stainfo.top_rate = stainfo.top_rate/2;
		}

		if(sta_req->req_flag&IEEE80211_INTERNAL_STA_FLAGS_SSID){
			rcu_read_lock();
			
			stainfo.ssid_len = 0;
			memset(stainfo.ssid,0,IEEE80211_MAX_SSID_LEN);
			
			if(sta->sdata->vif.type == NL80211_IFTYPE_STATION){
				struct cfg80211_bss *cbss = sta->sdata->u.mgd.associated;
				
				if(cbss){
					const char *ssid = NULL;
                    ssid = ieee80211_bss_get_ie(cbss, ATBM_WLAN_EID_SSID);
                    if(ssid){						
                        memcpy(stainfo.ssid, &ssid[2], ssid[1]);
                        stainfo.ssid_len = ssid[1];
                    }
				}				
			}else if(sta->sdata->vif.type == NL80211_IFTYPE_AP){
				struct ieee80211_bss_conf *bss_conf = &sta->sdata->vif.bss_conf;
				stainfo.ssid_len = bss_conf->ssid_len;
				if(stainfo.ssid_len)
					memcpy(stainfo.ssid,bss_conf->ssid,stainfo.ssid_len);
				
			}else {
				WARN_ON(1);
			}
			rcu_read_unlock();
		}
		memcpy(stainfo.mac,sta->sta.addr,6);
		stainfo.filled = sta_req->req_flag;
		if(sta_handle)
			sta_handle(&stainfo,sta_req->priv);
		
		memset(&stainfo,0,sizeof(struct ieee80211_internal_sta_info));
	}
	mutex_unlock(&local->sta_mtx);

	return true;
}
bool atbm_internal_cmd_monitor_req(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_monitor_req *monitor_req)
{
	struct ieee80211_local *local  = sdata->local;
	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
	bool res = false;
	unsigned int freq;
	struct ieee80211_sub_if_data *other_sdata;
	
	struct ieee80211_channel *chan;
	enum nl80211_iftype old_type = sdata->vif.type;
	
	if(!ieee80211_sdata_running(sdata)){
		return false;
	}
	
	if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_PASSIVE){
		return false;
	}

	list_for_each_entry(other_sdata, &local->interfaces, list){

		if(!ieee80211_sdata_running(other_sdata)){
			continue;
		}

		priv = (struct atbm_vif *)other_sdata->vif.drv_priv;

		if(priv->join_status != ATBM_APOLLO_JOIN_STATUS_PASSIVE){
			return false;
		}
	}
	if(ieee8011_channel_valid(&local->hw,monitor_req->ch) == false){
		return false;
	}
	
	switch(monitor_req->chtype){
	case NL80211_CHAN_NO_HT:
	case NL80211_CHAN_HT20:
	case NL80211_CHAN_HT40MINUS:
	case NL80211_CHAN_HT40PLUS:
		break;
	default:
		atbm_printk_err("error, %d\n", monitor_req->chtype);
		return false;
	}

	if(monitor_req->ch <= 14){
		freq = 2412+(monitor_req->ch - 1)*5;
	}else {
		freq = 5000 + (5*monitor_req->ch);
	}
	chan = ieee80211_get_channel(local->hw.wiphy, freq);

	if(chan == NULL){
		return false;
	}
	
	local->internal_monitor.req.ch = monitor_req->ch;
	local->internal_monitor.req.chtype = monitor_req->chtype;
	
	rcu_assign_pointer(local->internal_monitor.req.monitor_rx,monitor_req->monitor_rx);
	rcu_assign_pointer(local->internal_monitor.req.priv,monitor_req->priv);
	
	atbm_printk_debug("%s:[%s] channel %d\n",__func__,sdata->name,local->internal_monitor.req.ch);
	if(ieee80211_if_change_type(sdata, NL80211_IFTYPE_MONITOR)){
		res  = false;
		goto err;
	}

	if(ieee80211_set_channel(local->hw.wiphy,sdata->dev,chan,monitor_req->chtype)){
		goto err;
	}

	return true;
err:
	ieee80211_if_change_type(sdata,old_type);
	rcu_assign_pointer(local->internal_monitor.req.monitor_rx,NULL);
	rcu_assign_pointer(local->internal_monitor.req.priv,NULL);
	local->internal_monitor.req.ch = 0;
	
	return res;
}

bool atbm_internal_cmd_stop_monitor(struct ieee80211_sub_if_data *sdata)
{
	if(!ieee80211_sdata_running(sdata)){
		return false;
	}

	if(sdata->vif.type != NL80211_IFTYPE_MONITOR){
		return false;
	}

	ieee80211_if_change_type(sdata,NL80211_IFTYPE_STATION);

	rcu_assign_pointer(sdata->local->internal_monitor.req.monitor_rx,NULL);
	rcu_assign_pointer(sdata->local->internal_monitor.req.priv,NULL);

	synchronize_rcu();
	sdata->local->internal_monitor.req.ch = 0;
	sdata->local->internal_monitor.req.chtype = 0;
	
	return true;
}
bool atbm_internal_cmd_req_iftype(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_iftype_req *req)
{
	enum nl80211_iftype new_iftype;
	enum nl80211_iftype old_iftype = sdata->vif.type;
	struct ieee80211_local *local = sdata->local;
	bool change_channel = true;
	bool change_iftype  = true;
	
	ASSERT_RTNL();
	atbm_printk_debug("%s:type(%d),channel(%d)\n",__func__,req->if_type,req->channel);
	
	if (sdata->vif.type == NL80211_IFTYPE_STATION && sdata->u.mgd.associated){
		
		goto params_err;
	}
	
	if (sdata->vif.type == NL80211_IFTYPE_AP && sdata->u.ap.beacon){
		
		goto params_err;
	}
	
	switch(req->if_type){
	case IEEE80211_INTERNAL_IFTYPE_REQ__MANAGED:
		new_iftype = NL80211_IFTYPE_STATION;
		if(new_iftype == sdata->vif.type){
			change_iftype  = false;
		}
		change_channel = false;
		break;
	case IEEE80211_INTERNAL_IFTYPE_REQ__MONITOR:
		new_iftype = NL80211_IFTYPE_MONITOR;		
		if(new_iftype == sdata->vif.type){
			change_iftype = false;
		}
		break;
	default:
		goto params_err;
	}

	if(change_iftype == true){
		if(ieee80211_if_change_type(sdata, new_iftype)){
			goto params_err;
		}
	}
	if(change_channel == true) {
		struct ieee80211_channel *chan = ieee8011_chnum_to_channel(&sdata->local->hw,req->channel);

		if(chan == NULL){
			goto interface_err;
		}

		if(ieee80211_set_channel(local->hw.wiphy,sdata->dev,chan,NL80211_CHAN_HT20)){
			goto interface_err;
		}
	}

	return true;
interface_err:
	ieee80211_if_change_type(sdata,old_iftype);
params_err:
	return false;
}
bool atbm_internal_wsm_adaptive(struct atbm_common *hw_priv,struct ieee80211_internal_wsm_adaptive *adaptive)
{
	char* cmd = NULL;
	int len;
	bool res = true;
	
	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		res = false;
		goto err;
	}
	
	len = snprintf(cmd,ATBM_WSM_CMD_LEN,ATBM_WSM_ADAPTIVE"%d",adaptive->enable);

	if(len<=0){
		res = false;
		goto err;
	}
	if(len+1>ATBM_WSM_CMD_LEN){
		res = false;
		goto err;
	}
	atbm_printk_debug("%s:wsm [%s][%d]\n",__func__,cmd,len);
	
	if( wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1,0) < 0){
		res = false;
	}
	
err:
	if(cmd)
		atbm_kfree(cmd);
	return res;
}

bool atbm_internal_wsm_txpwr_dcxo(struct atbm_common *hw_priv,struct ieee80211_internal_wsm_txpwr_dcxo *txpwr_dcxo)
{
	int len;
	char* cmd = NULL;
	bool res = true;
	
	if(txpwr_dcxo->txpwr_L > 32 || txpwr_dcxo->txpwr_L < -32){
		atbm_printk_err("error, txpwr_L %d\n", txpwr_dcxo->txpwr_L);
		res = false;
		goto err;
	}
	
	if(txpwr_dcxo->txpwr_M > 32 || txpwr_dcxo->txpwr_M < -32){
		atbm_printk_err("error, txpwr_M %d\n", txpwr_dcxo->txpwr_M);
		res = false;
		goto err;
	}
	
	if(txpwr_dcxo->txpwr_H > 32 || txpwr_dcxo->txpwr_H < -32){
		atbm_printk_err("error, txpwr_H %d\n", txpwr_dcxo->txpwr_H);
		res = false;
		goto err;
	}
	
	if(txpwr_dcxo->dcxo > 127 || txpwr_dcxo->dcxo < 0){
		atbm_printk_err("error, dcxo %d\n", txpwr_dcxo->dcxo);
		res = false;
		goto err;
	}

	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		res = false;
		goto err;
	}

	len = snprintf(cmd, ATBM_WSM_CMD_LEN,"set_txpwr_and_dcxo,%d,%d,%d,%d ",
		           txpwr_dcxo->txpwr_L,txpwr_dcxo->txpwr_M, txpwr_dcxo->txpwr_H, txpwr_dcxo->dcxo);

	if(len<=0){
		res = false;
		goto err;
	}
	if(len+1>ATBM_WSM_CMD_LEN){
		res = false;
		goto err;
	}
	atbm_printk_debug("%s:wsm [%s][%d]\n",__func__,cmd,len);
	if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
		res = false;
	}
err:
	if(cmd)
		atbm_kfree(cmd);
	return res;
}

bool atbm_internal_wsm_txpwr(struct atbm_common *hw_priv,struct ieee80211_internal_wsm_txpwr *txpwr)
{
	int len;
	char* cmd = NULL;
	bool res = true;
	/*
	*0,3,15,63
	*/
	if(txpwr->txpwr_indx != 0 && 
	   txpwr->txpwr_indx != 3 &&
	   txpwr->txpwr_indx != 15 &&
	   txpwr->txpwr_indx != 63){
		atbm_printk_err("error, txpwr_indx %d\n", txpwr->txpwr_indx);
		res = false;
		goto err;
	}

	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		res = false;
		goto err;
	}

	len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_TXPWR"%d",txpwr->txpwr_indx);

	if(len<=0){
		res = false;
		goto err;
	}

	if(len+1>ATBM_WSM_CMD_LEN){
		res = false;
		goto err;
	}
	atbm_printk_debug("%s:wsm [%s][%d]\n",__func__,cmd,len);
	if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
		res = false;
	}
err:
	if(cmd)
		atbm_kfree(cmd);
	return res;
}
bool atbm_internal_wsm_set_rate(struct atbm_common *hw_priv,struct ieee80211_internal_rate_req *req)
{
	int len;
	char* cmd = NULL;
	bool res = true;

	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		res = false;
		goto err;
	}

	if(req->flags & IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_TX_RATE){
		len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_FIX_RATE,0);

		if(len<=0){
			res = false;
			goto err;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			res = false;
			goto err;
		}

		memset(cmd,0,ATBM_WSM_CMD_LEN);
	}

	if(req->flags & IEEE80211_INTERNAL_RATE_FLAGS_CLEAE_TOP_RATE){
		len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_TOP_RATE,0);

		if(len<=0){
			res = false;
			goto err;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			res = false;
			goto err;
		}

		memset(cmd,0,ATBM_WSM_CMD_LEN);
	}

	if(req->flags & IEEE80211_INTERNAL_RATE_FLAGS_CLEAR_MIN_RATE){
		len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_MIN_RATE,0);

		if(len<=0){
			res = false;
			goto err;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			res = false;
			goto err;
		}

		memset(cmd,0,ATBM_WSM_CMD_LEN);
	}
	
	if(req->flags & IEEE80211_INTERNAL_RATE_FLAGS_SET_TX_RATE){
		len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_FIX_RATE,req->rate);

		if(len<=0){
			res = false;
			goto err;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			res = false;
			goto err;
		}

		memset(cmd,0,ATBM_WSM_CMD_LEN);
	}

	if(req->flags & IEEE80211_INTERNAL_RATE_FLAGS_SET_TOP_RATE){
		len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_TOP_RATE,req->rate);

		if(len<=0){
			res = false;
			goto err;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			res = false;
			goto err;
		}

		memset(cmd,0,ATBM_WSM_CMD_LEN);
	}

	if(req->flags & IEEE80211_INTERNAL_RATE_FLAGS_SET_MIN_RATE){
		len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_MIN_RATE,req->rate);

		if(len<=0){
			res = false;
			goto err;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			res = false;
			goto err;
		}

		memset(cmd,0,ATBM_WSM_CMD_LEN);
	}
err:
	if(cmd)
		atbm_kfree(cmd);
	return res;
}

bool atbm_internal_wsm_set_rate_power(struct atbm_common *hw_priv,
												   struct ieee80211_internal_rate_power_req *req)
{
	#define MIN_RATE_INDEX	(0)
	#define MAX_RATE_INDEX	(10)
	#define MIN_POWER		(-16)
	#define MAX_POWER		(16)

	bool ret = true;
	char* cmd = NULL;
	int len = 0;
	
	if((req->rate_index < MIN_RATE_INDEX) ||(req->rate_index > MAX_RATE_INDEX)){
		ret = false;
		goto exit;
	}

	if((req->power < MIN_POWER) ||(req->power > MAX_POWER)){
		ret = false;
		goto exit;
	}
	
	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		ret = false;
		goto exit;
	}

	len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_SET_RATE_POWER,req->rate_index,req->power);

	if(len <= 0){
		ret = false;
		goto exit;
	}

	if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
		ret = false;
		goto exit;
	}
	
exit:
	if(cmd)
		atbm_kfree(cmd);

	return ret;

	#undef MIN_RATE_INDEX
	#undef MAX_RATE_INDEX
	#undef MIN_POWER
	#undef MAX_POWER
}
static char spec_oui_buf[256];
static char *spec_oui = "NULL";
module_param(spec_oui,charp,0644);
MODULE_PARM_DESC(spec_oui,"special oui");
void atbm_set_special_oui(struct atbm_common *hw_priv, char *pdata, int len)
{
    memset(spec_oui_buf, 0, 256);
    memcpy(spec_oui_buf, pdata, len);
    spec_oui = spec_oui_buf;
}
static int wifi_tx_pw = 0;
static char wifi_txpw_buf[64]={0};
static char *wifi_txpw = "NULL";
module_param(wifi_txpw,charp,0644);
MODULE_PARM_DESC(wifi_txpw,"wifi tx power");

int atbm_get_tx_power(void)
{
	return wifi_tx_pw;
}

void atbm_set_tx_power(struct atbm_common *hw_priv, int txpw)
{
	char *p20, *p40, *pHT;
	
	wifi_tx_pw = txpw;

	if(wifi_tx_pw & BIT(0))
		p20 = "20M-High ";
	else
		p20 = "20M-Normal ";


	if(wifi_tx_pw & BIT(1))
		p40 = "40M-High ";
	else
		p40 = "40M-Normal ";

	if((hw_priv->channel_type == NL80211_CHAN_HT20)||(hw_priv->channel_type == NL80211_CHAN_NO_HT))
		pHT = "20M-Mode";
	else
		pHT = "40M-Mode";

	memset(wifi_txpw_buf, 0, sizeof(wifi_txpw_buf));
	sprintf(wifi_txpw_buf, "%s, %s, %s", p20, p40, pHT);
	wifi_txpw = wifi_txpw_buf;

	return;
}												   
#define ATBM_SPECIAL_FREQ_MAX_LEN		128
static char wifi_freq_buf[ATBM_SPECIAL_FREQ_MAX_LEN]={0};
static char *wifi_freq = "NULL";
module_param(wifi_freq,charp,0644);
MODULE_PARM_DESC(wifi_freq,"wifi freq");
void atbm_set_freq(struct ieee80211_local *local)
{

   struct hlist_head *hlist;
   struct hlist_node *node;
   struct hlist_node *node_temp;
   struct ieee80211_special_freq *freq_node;
   int hash_index = 0;
   int n_freqs = 0;
   int len = 0;
   int total_len = 0;
   char *freq_show = wifi_freq_buf;
   
   memset(freq_show,0,ATBM_SPECIAL_FREQ_MAX_LEN);
   
   for(hash_index = 0;hash_index<ATBM_COMMON_HASHENTRIES;hash_index++){
	   hlist = &local->special_freq_list[hash_index];
	   hlist_for_each_safe(node,node_temp,hlist){
		   freq_node = hlist_entry(node,struct ieee80211_special_freq,hnode);
		   n_freqs ++ ;
		   len = scnprintf(freq_show+total_len,ATBM_SPECIAL_FREQ_MAX_LEN-total_len,"ch:%d, freq:%d \n",
			   channel_hw_value(freq_node->channel),freq_node->freq);
		   total_len += len;
	   }
   }

   if(n_freqs == 0){
	   wifi_freq = "NULL";
   }else {
	   wifi_freq = wifi_freq_buf;
   }
   
#if 0
   int i;
   
   memset(wifi_freq_buf, 0, sizeof(wifi_freq_buf));
   for(i=0; i<CHANNEL_NUM; i++){
	   if(pdata[i].flag == 1){
		   sprintf(wifi_freq_buf+strlen(wifi_freq_buf), "ch:%d, freq:%d \n", i+1, pdata[i].special_freq);
	   }
   }
   
   wifi_freq = wifi_freq_buf;

   return;
#endif
}
bool atbm_internal_freq_set(struct ieee80211_hw *hw,struct ieee80211_internal_set_freq_req *req)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct atbm_common *hw_priv = (struct atbm_common *)hw->priv;
	struct ieee80211_channel *channel;
	char* cmd = NULL;
	int len;
	bool res = true;
	struct ieee80211_special_freq special_req;
	
	ASSERT_RTNL();

	channel = ieee8011_chnum_to_channel(hw,req->channel_num);

	if(channel == NULL){
		res = false;
		goto out;
	}
	
	if(req->set == false){
		req->freq = channel_center_freq(channel);
	}
	
	if((req->freq < 2300) || (req->freq>2600)){
		res = false;
		goto out;
	}
	
	mutex_lock(&local->mtx);
	__ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);

	if((local->hw.conf.flags & IEEE80211_CONF_IDLE) == 0){
		res = false;
		goto out;
	}

	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		res = false;
		goto out;
	}

	len = snprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_SET_FREQ,req->channel_num,(int)req->freq);

	if(len <= 0){
		res = false;
		goto out;
	}

	if(len+1>ATBM_WSM_CMD_LEN){
		res = false;
		goto out;
	}
	special_req.channel = channel;
	special_req.freq    = req->freq;
	
	if(channel_center_freq(channel) != req->freq){		
		if(ieee80211_special_freq_update(local,&special_req) == false){
			res = false;
			goto out;
		}
	}else {
		ieee80211_special_freq_clear(local,&special_req);
	}
	if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
		ieee80211_special_freq_clear(local,&special_req);
		res = false;
		goto out;
	}
out:
	if(cmd)
		atbm_kfree(cmd);

	return res;
}
bool atbm_internal_channel_auto_select(struct ieee80211_sub_if_data *sdata,
													  struct ieee80211_internal_channel_auto_select_req *req)
{
	struct ieee80211_internal_scan_request scan_req;
	
	scan_req.req_flags = IEEE80211_INTERNAL_SCAN_FLAGS__CCA;
	/*
	*all off supported channel will be scanned
	*/
	scan_req.channels   = NULL;
	scan_req.n_channels = 0;
	scan_req.macs       = NULL;
	scan_req.n_macs     = 0;
	scan_req.ies		= NULL;
	scan_req.ie_len		= 0;
	scan_req.no_cck     = false;
	scan_req.priv		= NULL;
	scan_req.result_handle = NULL;
	scan_req.ssids      = NULL;
	scan_req.n_ssids    = 0;

	return atbm_internal_cmd_scan_triger(sdata,&scan_req);
}

static bool atbm_internal_channel_auto_select_results_handle(struct ieee80211_hw *hw,struct atbm_internal_scan_results_req *req,struct ieee80211_internal_scan_sta *sta_info)
{
	struct ieee80211_internal_channel_auto_select_results *cca_results = (struct ieee80211_internal_channel_auto_select_results *)req->priv;
	s8 signal = (s8)sta_info->signal;
	u8 cur_channel = sta_info->channel;
	u8 index = 0;
	struct ieee80211_channel *channel;
	
	if(ieee8011_channel_valid(hw,cur_channel) == false){
		return false;
	}

	if(sta_info->cca == false){
		return false;
	}
	
	req->n_stas ++;
	cca_results->n_aps[cur_channel-1]++;
	
	if(cca_results->version == 1)
		cca_results->weight[cur_channel-1] += ieee80211_rssi_weight(signal);
	else 
		cca_results->weight[cur_channel-1]++;
	
	channel = ieee8011_chnum_to_channel(hw,cur_channel);

	if(channel_in_special(channel) == true){
		return true;
	}
	/*
	*2.4G channel
	*/
	atbm_printk_debug("ssid[%s],channel[%d],signal(%d)\n",sta_info->ssid,cur_channel,signal);
	/*
	*channel 1-13
	*weight[x] +=  val[x] + val[x-1] + val[x-2] + val[x-3] + val[x+1] + val[x+2] + val[x+3]
	*/
	if(cur_channel<=13){
		u8 low;
		u8 high;

		low = cur_channel>=4?cur_channel-3:1;
		high = cur_channel<= 10 ? cur_channel+3:13;
		
		for(index=cur_channel+1;index<=high;index++){
			channel = ieee8011_chnum_to_channel(hw,index);
			/*
			*skip special freq
			*/
			if(channel_in_special(channel) == true){
				atbm_printk_debug("%s:skip special freq(%d)\n",__func__,channel_hw_value(channel));
				continue;
			}
			
			if(cca_results->version == 1)
				cca_results->weight[index-1] += ieee80211_rssi_weight(signal - 2*(index-cur_channel));
			else 
				cca_results->weight[index-1] ++;
		}

		for(index=cur_channel-1;index>=low;index--){
			channel = ieee8011_chnum_to_channel(hw,index);
			/*
			*skip special freq
			*/
			if(channel_in_special(channel) == true){
				atbm_printk_debug("%s:skip special freq(%d)\n",__func__,channel_hw_value(channel));
				continue;
			}
			if(cca_results->version == 1)
				cca_results->weight[index-1] += ieee80211_rssi_weight(signal - 2*(cur_channel-index));
			else 
				cca_results->weight[index-1] ++;
		}
	}
	/*
	*channel 14
	*/
	else if(cur_channel == 14){
		
	}
	/*
	*5G channel
	*/
	else {
		
	}

	for(index = 0;index<IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX;index++){
		atbm_printk_debug("weight[%d]=[%d]\n",index,cca_results->weight[index]);
	}
	return true;
}
bool atbm_internal_channel_auto_select_results(struct ieee80211_sub_if_data *sdata,
												struct ieee80211_internal_channel_auto_select_results *results)
{
	#define ATBM_BUSY_RATIO_MIN		100
	struct atbm_internal_scan_results_req results_req;
	struct ieee80211_local *local = sdata->local;
	u8 *busy_ratio;
	u8 i;
	u32 min_ap_num = (u32)(-1);
	u8  min_busy_ratio = 128;
	u8  min_ap_num_ration = 128;
	u8 channel = 0;
	int band;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0))
	u32 ignore_flags = IEEE80211_CHAN_DISABLED;
#endif
	struct ieee80211_supported_band *sband;
	u8 ignor_channel_mask[IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX];
	u8 channel_mask[IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX];

	results_req.n_stas = 0;
	results_req.flush = true;
	results_req.priv = results;
	results_req.result_handle = atbm_internal_channel_auto_select_results_handle;
	busy_ratio = ieee80211_scan_cca_val_get(&local->hw);
	
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0))
	ignore_flags |= IEEE80211_CHAN_NO_OFDM;
#endif
	if(ieee80211_scan_internal_req_results(local,&results_req) == false){
		goto err;
	}
	
	for(i = 0;i<14;i++){
		atbm_printk_debug("busy_ratio[%d]=[%d]\n",i,busy_ratio[i]);
	}
	
	memset(ignor_channel_mask,0,IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX);
	memset(channel_mask,1,IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX);

	for(i= 0;i<results->ignore_n_channels;i++){
		
		BUG_ON(results->ignore_channels == NULL);
		
		if(ieee8011_channel_valid(&local->hw,results->ignore_channels[i]) == false){
			goto err;
		}
		ignor_channel_mask[results->ignore_channels[i]-1] = 1;
		
		atbm_printk_debug("%s channel %d ignored\n",__func__,results->ignore_channels[i]);
	}

	if(results->n_channels){
		memset(channel_mask,0,IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX);
		for(i = 0;i<results->n_channels;i++){
			BUG_ON(results->channels == NULL);
			if(ieee8011_channel_valid(&local->hw,results->channels[i]) == false){
				goto err;
			}

			channel_mask[results->channels[i]-1] = 1;
		}
	}
	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		
		sband = local->hw.wiphy->bands[band];
		
		if (!sband)
			continue;
		/*
		*2.4G channel and 5G
		*/
		for(i = 0;i<sband->n_channels;i++){
			/*
			*0 means that the channel do not process cca
			*/
			if(busy_ratio[channel_hw_value(&sband->channels[i])-1] == 0){
				continue;
			}
			
			if(ignor_channel_mask[channel_hw_value(&sband->channels[i])-1] == 1){
				continue;
			}

			if(channel_mask[channel_hw_value(&sband->channels[i])-1] == 0){
				continue;
			}
			/*
			*special freq must be skiped
			*/
			if(channel_in_special(&sband->channels[i])){
				atbm_printk_debug("%s:skip special freq(%d)\n",__func__,channel_hw_value(&sband->channels[i]));
				continue;
			}
			/*
			*some disabled channel must be skiped
			*/
			/**
			if(ignore_flags&sband->channels[i].flags){
				atbm_printk_debug("%s: channel[%d] not support ofdm\n",__func__,channel_hw_value(&sband->channels[i]));
				continue;
			}
			*/
		//	atbm_printk_err("\n");
	//		atbm_printk_err("channel[%d] min_ap_num [%d]  min_ap_num_ration[%d] min_busy_ratio[%d] \n",channel,min_ap_num,min_ap_num_ration,min_busy_ratio);
	//		atbm_printk_err("i = %d , busy_ratio[%d] = %d \n",i,channel_hw_value(&sband->channels[i])-1,busy_ratio[channel_hw_value(&sband->channels[i])-1]);
			
			if(busy_ratio[channel_hw_value(&sband->channels[i])-1]<ATBM_BUSY_RATIO_MIN){

				if(results->weight[channel_hw_value(&sband->channels[i])-1]<=min_ap_num){
					if(results->weight[channel_hw_value(&sband->channels[i])-1]==min_ap_num){
						if(busy_ratio[channel_hw_value(&sband->channels[i])-1]<=min_ap_num_ration){
							min_ap_num = results->weight[channel_hw_value(&sband->channels[i])-1];
							channel = channel_hw_value(&sband->channels[i]);
							min_ap_num_ration = busy_ratio[channel_hw_value(&sband->channels[i])-1];
						}
					}else {
						min_ap_num = results->weight[channel_hw_value(&sband->channels[i])-1];
						channel = channel_hw_value(&sband->channels[i]);
						min_ap_num_ration = busy_ratio[channel_hw_value(&sband->channels[i])-1];
					}
				}
				
			}else if(min_ap_num == (u32)(-1)){
				if(busy_ratio[channel_hw_value(&sband->channels[i])-1]<min_busy_ratio){
					min_busy_ratio = busy_ratio[channel_hw_value(&sband->channels[i])-1];
					channel = channel_hw_value(&sband->channels[i]);
				}
			}
		}			
	}

	if(channel == 0){
		//WARN_ON(channel == 0);
		atbm_printk_err("auto select fail! \n");
		goto err;
	}
	atbm_printk_debug("auto_select channel %d\n",channel);
	memcpy(results->busy_ratio,busy_ratio,IEEE80211_ATBM_MAX_SCAN_CHANNEL_INDEX);
	results->susgest_channel = channel;
	ieee80211_scan_cca_val_put(&local->hw);
	return true;
err:
	ieee80211_scan_cca_val_put(&local->hw);
	return false;
}
#ifdef CONFIG_ATBM_MONITOR_SPECIAL_MAC
bool atbm_internal_mac_monitor(struct ieee80211_hw *hw,struct ieee80211_internal_mac_monitor *monitor)
{
	
	struct atbm_common *hw_priv = (struct atbm_common *)hw->priv;
	char* cmd = NULL;
	int len = 0;
	bool ret = true;

	cmd = atbm_kzalloc(ATBM_WSM_CMD_LEN,GFP_KERNEL);

	if(cmd == NULL){
		ret = false;
		goto exit;
	}
	
	if(monitor->flags & (IEEE80211_INTERNAL_MAC_MONITOR_START | IEEE80211_INTERNAL_MAC_MONITOR_STOP)){

		atbm_printk_err("mac_monitor:enable(%d),mac[%pM]\n",__func__,
						!!(monitor->flags&IEEE80211_INTERNAL_MAC_MONITOR_START),
						monitor->mac);
		len = scnprintf(cmd, ATBM_WSM_CMD_LEN, ATBM_WSM_MONITOR_MAC,monitor->index,
						!!(monitor->flags&IEEE80211_INTERNAL_MAC_MONITOR_START),
						monitor->mac[0],monitor->mac[1],
						monitor->mac[2],monitor->mac[3],
						monitor->mac[4],monitor->mac[5]);

		if(len<=0){
			ret = false;
			goto exit;
		}

		if(wsm_write_mib(hw_priv, WSM_MIB_ID_FW_CMD, cmd, len+1, 0) < 0){
			ret = false;
			goto exit;
		}
	}

	if(monitor->flags & IEEE80211_INTERNAL_MAC_MONITOR_RESULTS){
		
		int i = 0;
		if(wsm_read_mib(hw_priv,WSM_MIB_ID_GET_MONITOR_MAC_STATUS,cmd,ATBM_WSM_CMD_LEN,0) != 0){
			ret = false;
			goto exit;
		}

		for (i = 0;i<IEEE80211_INTERNAL_MAC_MONITOR_RESULTS;i++){
			monitor->reults[i].found     = 	*cmd++;
			monitor->reults[i].rssi      = 	*cmd++;
			monitor->reults[i].forcestop =	*cmd++;
			monitor->reults[i].used   	 =	*cmd++;
			monitor->reults[i].index     =  *cmd++;
			monitor->reults[i].enabled   =  *cmd++; 
			memcpy(monitor->reults[i].mac,cmd,6); cmd += 6;
			monitor->reults[i].delta_time = __le32_to_cpu(*((u32*)cmd)); cmd += 4;

			if(monitor->reults[i].used == 0){
				monitor->reults[i].used = 1;
				break;
			}
		}
	}
	
exit:
	if(cmd)
		atbm_kfree(cmd);
	return ret;
}
#endif
bool atbm_internal_request_chip_cap(struct ieee80211_hw *hw,struct ieee80211_internal_req_chip *req)
{
	struct atbm_common *hw_priv = (struct atbm_common *)hw->priv;

	if(req->flags & IEEE80211_INTERNAL_REQ_CHIP_FLAGS__CHIP_VER){
		if(hw_priv->wsm_caps.firmwareCap &CAPABILITIES_EFUSE8){
			req->chip_version = chip_6038;
		}else if(hw_priv->wsm_caps.firmwareCap &CAPABILITIES_EFUSEI){
			req->chip_version = chip_6032i;
		}else if(hw_priv->wsm_caps.firmwareCap &CAPABILITIES_EFUSEB){
			req->chip_version = chip_101B;
		}else {
			req->chip_version = chip_6032i;
		}
	}

	/*other code */

	return true;
}
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG
bool atbm_internal_update_ap_conf(struct ieee80211_sub_if_data *sdata,
									     struct ieee80211_internal_ap_conf *conf_req,bool clear)
{
	
	if(!ieee80211_sdata_running(sdata)){
		atbm_printk_scan("%s:%d\n",__func__,__LINE__);
		goto err;
	}

	if(conf_req&&conf_req->channel){
		if(ieee8011_channel_valid(&sdata->local->hw,(int)conf_req->channel) == false){
			goto err;
		}
	}

	return ieee80211_update_ap_config(sdata,conf_req,clear);
err:
	return false;
}
#endif
int atbm_internal_addr_read_bit(struct atbm_common *hw_priv,u32 addr,u8 endBit,
	u8 startBit,u32 *data )
{                                                              
	u32	reg_val=0;                                        
	u32 regmask=0;
	int ret = 0;
	
	ret=atbm_direct_read_reg_32(hw_priv,addr,&reg_val); 
	if(ret<0){
		goto rw_end;
	}                             
	regmask = ~((1<<startBit) -1);                               
	regmask &= ((1<<endBit) -1)|(1<<endBit);                     
	reg_val &= regmask;                                      
	reg_val >>= startBit; 
	
	*data = reg_val;
rw_end:
	return ret;
}   

int atbm_internal_addr_write_bit(struct atbm_common *hw_priv,u32 addr,u8 endBit,
											u8 startBit,u32 data)
{                                                              
	u32	reg_val=0;                                        
	u32 regmask=0;
	int ret = 0;
	
	ret = atbm_direct_read_reg_32(hw_priv,addr,&reg_val); 
	
	if(ret<0){
		atbm_printk_err("%s:read err\n",__func__);
		goto rw_end;
	} 
	atbm_printk_err("%s:ret(%d)\n",__func__,ret);
	regmask = ~((1<<startBit) -1);                               
	regmask &= ((1<<endBit) -1)|(1<<endBit);                     
	reg_val &= ~regmask;                                      
	reg_val |= (data <<startBit)&regmask;                     
	ret = atbm_direct_write_reg_32(hw_priv,addr,reg_val);
	
	if(ret<0)
	{
		atbm_printk_err("%s:write err\n",__func__);
		goto rw_end;
	}
	
	if(ret)
		ret = 0;
rw_end:
	atbm_printk_err("%s:ret(%d)\n",__func__,ret);

	return ret;
}  

static int atbm_internal_gpio_set(struct atbm_common *hw_priv,struct atbm_ctr_addr *gpio_addr)
{
	unsigned int status = -1; 
	
	if(atbm_bh_is_term(hw_priv)){
		atbm_printk_err("%s:atbm term\n",__func__);
		goto exit;
	}
	
	status = atbm_internal_addr_write_bit(hw_priv,gpio_addr->base_addr,
			gpio_addr->start_bit+gpio_addr->width,gpio_addr->start_bit,gpio_addr->val);
exit:
	return status;
}

static int atbm_internal_gpio_get(struct atbm_common *hw_priv,struct atbm_ctr_addr *gpio_addr)
{
	unsigned int status = -1; 
	
	if(atbm_bh_is_term(hw_priv)){
		atbm_printk_err("%s:atbm term\n",__func__);
		goto exit;
	}
	
	status = atbm_internal_addr_read_bit(hw_priv,gpio_addr->base_addr,
			gpio_addr->start_bit+gpio_addr->width-1,gpio_addr->start_bit,&gpio_addr->val);
exit:
	return status;
}

static struct atbm_gpio_config *atbm_internal_gpio_reqest(struct atbm_common *hw_priv,int gpio)
{
	int i = 0;
	struct atbm_gpio_config *gpio_dev = NULL;
	
	for(i = 0;i < ARRAY_SIZE(atbm_gpio_table);i++){
		gpio_dev = &atbm_gpio_table[i];
		if(gpio_dev->gpio == gpio){
			return gpio_dev;
		}
	}

	return NULL;
	
}
bool atbm_internal_gpio_config(struct atbm_common *hw_priv,int gpio,bool dir ,bool pu,bool default_val)
{
	struct atbm_gpio_config *gpio_dev = NULL;
	bool ret = true;
	int status = -1;
	
	gpio_dev = atbm_internal_gpio_reqest(hw_priv,gpio);

	if(gpio_dev == NULL){
		atbm_printk_err("%s:gpio (%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	gpio_dev->fun_ctrl.val = 3;
	gpio_dev->dir_ctrl.val = dir == true ? 1:0;
	gpio_dev->pup_ctrl.val = pu  == true ? 1:0;
	gpio_dev->pdu_ctrl.val = pu  == false ? 1:0;

	status = atbm_internal_gpio_set(hw_priv,&gpio_dev->fun_ctrl);

	if(status){
		atbm_printk_err("%s:gpio function(%d)(%d) is err\n",__func__,gpio,status);
		ret =  false;
		goto exit;
	}

	status = atbm_internal_gpio_set(hw_priv,&gpio_dev->dir_ctrl);
	
	if(status){
		atbm_printk_err("%s:gpio dir(%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	status = atbm_internal_gpio_set(hw_priv,&gpio_dev->pup_ctrl);
	
	if(status){
		atbm_printk_err("%s:gpio pup(%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	status = atbm_internal_gpio_set(hw_priv,&gpio_dev->pup_ctrl);
	
	if(status){
		atbm_printk_err("%s:gpio pdu(%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	if(dir == true){
		gpio_dev->out_val.val = default_val == true ? 1:0;
		status = atbm_internal_gpio_set(hw_priv,&gpio_dev->out_val);
		if(status){
			atbm_printk_err("%s:gpio out(%d) is err\n",__func__,gpio);
			ret =  false;
			goto exit;
		}
	}

	gpio_dev->flags = ATBM_GPIO_CONFIG__FUNCTION_CONFIGD;

	if(dir == true)
		gpio_dev->flags |= ATBM_GPIO_CONFIG__OUTPUT;
	else
		gpio_dev->flags |= ATBM_GPIO_CONFIG__INPUT;

	if(pu)
		gpio_dev->flags |= ATBM_GPIO_CONFIG__PUP;
	else
		gpio_dev->flags |= ATBM_GPIO_CONFIG__PUD;
exit:	
	return ret;
}

bool atbm_internal_gpio_output(struct atbm_common *hw_priv,int gpio,bool set)
{
	struct atbm_gpio_config *gpio_dev = NULL;
	bool ret =true;
	
	gpio_dev = atbm_internal_gpio_reqest(hw_priv,gpio);

	if(gpio_dev == NULL){
		atbm_printk_err("%s:gpio (%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	if(!(gpio_dev->flags & ATBM_GPIO_CONFIG__FUNCTION_CONFIGD)){
		atbm_printk_err("%s:gpio (%d) is not configed\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	if(!(gpio_dev->flags & ATBM_GPIO_CONFIG__OUTPUT)){
		atbm_printk_err("%s:gpio (%d) is not output mode\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	gpio_dev->out_val.val = set == true ? 1:0;
	
	if(atbm_internal_gpio_set(hw_priv,&gpio_dev->out_val)){
		atbm_printk_err("%s:gpio out(%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}
exit:
	return ret;
}

bool atbm_internal_gpio_input(struct atbm_common *hw_priv,int gpio,bool *set)
{
	struct atbm_gpio_config *gpio_dev = NULL;
	bool ret =true;
	
	gpio_dev = atbm_internal_gpio_reqest(hw_priv,gpio);

	if(gpio_dev == NULL){
		atbm_printk_err("%s:gpio (%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	if(!(gpio_dev->flags & ATBM_GPIO_CONFIG__FUNCTION_CONFIGD)){
		atbm_printk_err("%s:gpio (%d) is not configed\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	if(!(gpio_dev->flags & ATBM_GPIO_CONFIG__INPUT)){
		atbm_printk_err("%s:gpio (%d) is not input mode\n",__func__,gpio);
		ret =  false;
		goto exit;
	}
	
	if(atbm_internal_gpio_get(hw_priv,&gpio_dev->in_val)){
		atbm_printk_err("%s:gpio out(%d) is err\n",__func__,gpio);
		ret =  false;
		goto exit;
	}

	*set = gpio_dev->in_val.val ? true:false;
exit:
	return ret;
}
/*
WSM_EDCA_SET(&priv->edca, queue, params->aifs,
                                params->cw_min, params->cw_max, params->txop, 0xc8,
                                params->uapsd);

*/
bool atbm_internal_edca_update(struct ieee80211_sub_if_data *sdata,int queue,int aifs,int cw_win,int cw_max,int txop)
{
	bool ret = false;
	struct atbm_vif *priv = (struct atbm_vif *)sdata->vif.drv_priv;
	
	if(!ieee80211_sdata_running(sdata)){
		atbm_printk_scan("%s:%d\n",__func__,__LINE__);
		goto exit;
	}

	if(atomic_read(&priv->enabled) == 0){
		atbm_printk_err("%s:disabled\n",__func__);
		goto exit;
	}

	WSM_EDCA_SET(&priv->edca, queue, aifs,
                 cw_win, cw_max, txop, 0xc8,
                 priv->edca.params[queue].uapsdEnable);
	ret = wsm_set_edca_params(priv->hw_priv, &priv->edca, priv->if_id);
	if (ret) {
		atbm_printk_err("%s:wsm_set_edca_params\n",__func__);
		goto exit;
	}

	ret = true;
exit:
	
	return ret;
}
