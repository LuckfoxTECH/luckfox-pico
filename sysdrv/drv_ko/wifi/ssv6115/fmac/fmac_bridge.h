#ifndef _FMAC_BRIDGE_H_
#define _FMAC_BRIDGE_H_

#define WLAN_ETHHDR_LEN		14
#define NETDEV_HWADDR(vif)		((vif)->ndev->dev_addr) // dev_addr: Interface address info used in eth_type_trans()
#define NET_BR0_AGEING_TIME	120



void ssv_bridge_flush(struct ssv_vif *vif);
struct ssv_vif *ssv_bridge_tx_change(struct sk_buff *skb, struct net_device *dev);
int ssv_bridge_rx_change(struct ssv_vif *ssv_vif, struct sk_buff *skb);

//记录net device mac到br_mac
void br0_netdev_open(struct net_device *netdev);
//申请br0 私有成员数据
int br0_attach(struct ssv_vif *vif);
//释放br0私有数据
void br0_detach(struct ssv_vif *vif);

#define TX_STR  "Tx"
#define RX_STR  "Rx"
void fmac_bridge_dump_skb_info(unsigned char *tag, char *dir, struct ssv_vif *vif, struct sk_buff *skb);
#endif // _FMAC_BRIDGE_H_

