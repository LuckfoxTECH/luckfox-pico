#ifndef _MAC80211_BR_H_
#define _MAC80211_BR_H_

#define WLAN_ETHHDR_LEN		14
#define NETDEV_HWADDR(sdata)		((sdata)->dev->dev_addr)
#define NET_BR0_AGEING_TIME	120



void ieee80211_brigde_flush(struct ieee80211_sub_if_data *sdata);
int ieee80211_brigde_change_rxhdr(struct ieee80211_sub_if_data *sdata, struct sk_buff *skb);
int ieee80211_brigde_change_txhdr(struct ieee80211_sub_if_data *sdata, struct sk_buff **pskb);
struct ieee80211_sub_if_data *ieee80211_brigde_sdata_check(struct ieee80211_local *local,struct sk_buff **pskb,struct ieee80211_sub_if_data *source_sdata);

void br0_netdev_open(struct net_device *netdev);
int br0_attach(struct ieee80211_sub_if_data *sdata);
void br0_detach(struct ieee80211_sub_if_data *sdata);

#endif // _MAC80211_BR_H_

