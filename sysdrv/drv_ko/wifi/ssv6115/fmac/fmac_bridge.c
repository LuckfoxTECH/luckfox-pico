/*
 ****************************************************************************************
 *
 * Project: wifi station bridge
 *
 * Description:
 *     wifi station bridge
 *
 *
 ****************************************************************************************
 */



#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/bitmap.h>
#include <linux/rcupdate.h>
#include <linux/export.h>
#include <net/net_namespace.h>
//#include <net/ieee80211_radiotap.h>
#include <net/cfg80211.h>
#include <asm/unaligned.h>
#include <linux/udp.h>	 

#include <linux/if_arp.h>
#include <net/ip.h>
#include <net/ipx.h>
#include <linux/atalk.h>
#include <linux/udp.h>
#include <linux/if_pppox.h>
#include <linux/netdevice.h>

#include "fmac.h"
#include "fmac_defs.h"
#include "fmac_bridge.h"
#include "ssv_debug.h"
#include "fmac_utils.h"

//for debug define 
//#define FMAC_BR_DEBUG

#ifdef FMAC_BR_DEBUG
static const char *const frame_type2string[SSV_DATA_UNKNOW + 1] = {
    [SSV_ARP_REPLY] = "ARP_REPLY",
    [SSV_ARP_REQUEST] = "ARP_REQUEST",
    [SSV_ICMP_ECHO] = "ICMP_ECHO",
    [SSV_ICMP_ECHOREPLY] = "ICMP_ECHOREPLY",
    [SSV_DHCP_DISCOVER] = "DHCP_DISCOVER",
    [SSV_DHCP_OFFER] = "DHCP_OFFER",
    [SSV_DHCP_REQUEST] = "DHCP_REQUEST",
    [SSV_DHCP_ACK] = "DHCP_ACK",
    [SSV_EAPOL] = "EAPOL",

    [SSV_DATA_UNKNOW] = "UNKNOW",
};


static const char *const frame_proto2string[0xffff]=
{
	[ETH_P_ARP]		=		"ARP",
	[ETH_P_IP]		=		"IP",
	[ETH_P_PAE]		=		"PAE",
	[ETH_P_IPV6]	=		"IPv6",
};
#endif /* FMAC_BR_DEBUG */


typedef struct list_head queue_entry;

enum
{
    BR0_POLICY_OPEN     = 0,
    BR0_POLICY_ALLOW    = 1,
    BR0_POLICY_DENY     = 2,
};

#define BR0_HASHSIZE    	32
#define NETINFO_MAX_LEN		12

struct br0_info {
	u8   macaddr[6];
	u8   ipaddr[4];
};

struct NET_BR0_INFO_ENTRY
{
    queue_entry   	br0_list;
    queue_entry     br0_hash;
	unsigned long ageing_timer;
	union {
    	u8     br0_netinfo[NETINFO_MAX_LEN];
 		struct br0_info info;
	}u;
};

struct NETWIFI_S_BRIDGE
{
	spinlock_t						br_ext_lock;
    int         					as_policy;
    queue_entry   					as_list;
    queue_entry       				as_hash[BR0_HASHSIZE];
	unsigned long 					ageing_timer;

	unsigned char					fast_mac[6];
	unsigned char					fast_ip[4];
	struct NET_BR0_INFO_ENTRY		*fast_entry;
	unsigned char					br_mac[6];
	unsigned char					br_ip[4];

};



//static void br0_free_all_locked(struct NETWIFI_S_BRIDGE *br0_priv);
static int br0_free_all(struct ssv_vif *vif);

void br0_info_expire(struct ssv_vif *vif);

//void frame_hexdump(char *prefix, u8 *data, int len);

/**
 * br0_add_queue - add a new queue_entry in queue tail
 * @list: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new queue_entry in queue tail.
 */
static inline void br0_add_queue(queue_entry *list, queue_entry *head)
{
    list_add_tail(list, head);
}

static inline void br0_add_queue_head(queue_entry *list, queue_entry *head)
{
    list_add(list, head);
}

/**
 * br0_del_queue - deletes queue_entry from queue and reinitialize it.
 * @list: the element to delete from the queue.
 */
static inline void br0_del_queue(queue_entry *list)
{
    list_del_init(list);
}

/**
 * br0_queue_empty - tests whether a queue is empty
 * @head: the head of the queue.
 */
static inline int br0_queue_empty(queue_entry *head)
{
    return list_empty(head);
}


#define BR0_LOCK_DESTROY(_as) 
#define BR0_LOCK(_as)           spin_lock_bh(&(_as)->br_ext_lock)
#define BR0_UNLOCK(_as)         spin_unlock_bh(&(_as)->br_ext_lock)


int br0_attach(struct ssv_vif *vif)
{
    struct NETWIFI_S_BRIDGE *br0_priv;
    int num = 0;

	br0_priv= kmalloc(sizeof(struct NETWIFI_S_BRIDGE), GFP_KERNEL);

    if (br0_priv == NULL)
    {
    	SSV_LOG_DBG("ERROR br0_attach\n\n");
        return 0;
    }
	memset(br0_priv, 0, sizeof(struct NETWIFI_S_BRIDGE));
	spin_lock_init(&br0_priv->br_ext_lock);
    INIT_LIST_HEAD(&br0_priv->as_list);
    for (num = 0; num < BR0_HASHSIZE; num++)
    {
        INIT_LIST_HEAD(&br0_priv->as_hash[num]);
    }
	br0_priv->ageing_timer = jiffies;
    br0_priv->as_policy = BR0_POLICY_OPEN;
    vif->bridge_priv = br0_priv;
    return 1;
}

void br0_detach(struct ssv_vif *vif)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;

	if (br0_priv == NULL)
		return;

    br0_free_all(vif);
    BR0_LOCK_DESTROY(br0_priv);
    kfree(vif->bridge_priv);
    vif->bridge_priv = NULL;
}

static int BR0_HASH(const u8 *ipaddr)
{
	unsigned long x;

	x = ipaddr[3]  ^ ipaddr[2];

	return x % BR0_HASHSIZE;
}

#if 0
static __inline struct NET_BR0_INFO_ENTRY *
_br0_find_netinfo(struct NETWIFI_S_BRIDGE *br0_priv, const u8 *macaddr, const u8 *ipaddr)
{
    struct NET_BR0_INFO_ENTRY *br_info;
    int hash;

    hash = BR0_HASH(ipaddr);
    list_for_each_entry(br_info, &br0_priv->as_hash[hash], br0_hash)
    {
        if (memcmp(br_info->u.info.macaddr, macaddr, ETH_ALEN) == 0)
            return br_info;
    }
    return NULL;
}
#endif

static __inline struct NET_BR0_INFO_ENTRY *
_br0_find_netinfo_ip(struct NETWIFI_S_BRIDGE *br0_priv, const u8 *ipaddr)
{
    struct NET_BR0_INFO_ENTRY *br_info;
    int hash;

    hash = BR0_HASH(ipaddr);
    list_for_each_entry(br_info, &br0_priv->as_hash[hash], br0_hash)
    {
        if (memcmp(br_info->u.info.ipaddr, ipaddr, 4) == 0)
            return br_info;
    }
    return NULL;
}

static void
_br0_free(struct NETWIFI_S_BRIDGE *br0_priv, struct NET_BR0_INFO_ENTRY *br_info)
{
   // BR0_LOCK_ASSERT(br0_priv);

    br0_del_queue(&br_info->br0_list);
    br0_del_queue(&br_info->br0_hash);
	// LIST_REMOVE(NET_BR0_INFO_ENTRY, br0_hash);
    kfree(br_info);
}

static int
br0_add(struct ssv_vif *vif, const u8* mac, const u8 *ipaddr)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
    struct NET_BR0_INFO_ENTRY *br_info, *new;
    int hash;
	
	br0_info_expire(vif);
	new = kmalloc(sizeof(struct NET_BR0_INFO_ENTRY), GFP_KERNEL); 
    if (new == NULL)
    {
    	SSV_LOG_DBG("ERROR br0_add\n\n");
        return 0;
    }

	memset(new, 0, sizeof(struct NET_BR0_INFO_ENTRY));

    BR0_LOCK(br0_priv);
    hash = BR0_HASH(ipaddr);
    /*SSV_LOG_DBG("Br0:add new node %x:%x:%x:%x:%x:%x, ip %d.%d.%d.%d,hash:%d\n",
              mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3],hash); */

    list_for_each_entry(br_info, &br0_priv->as_hash[hash], br0_hash)
    {
        if (memcmp(br_info->u.info.ipaddr, ipaddr, 4) == 0)
        {
			br_info->ageing_timer = jiffies;
			memcpy(br_info->u.info.macaddr, mac, ETH_ALEN);
            BR0_UNLOCK(br0_priv);
            kfree(new);
            return EEXIST;
        }
    }
    memcpy(new->u.info.macaddr, mac, ETH_ALEN);
    memcpy(new->u.info.ipaddr, ipaddr, 4);
    br0_add_queue(&new->br0_list, &br0_priv->as_list);

    br0_add_queue_head(&new->br0_hash, &br0_priv->as_hash[hash]);
    BR0_UNLOCK(br0_priv);

    return 0;
}

#if 0
static int
br0_remove(struct ssv_vif *vif, const u8 *ipaddr)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
    struct NET_BR0_INFO_ENTRY *br_info;

    BR0_LOCK(br0_priv);
    br_info = _br0_find_netinfo_ip(br0_priv, ipaddr);
    if (br_info != NULL)
        _br0_free(br0_priv, br_info);
    BR0_UNLOCK(br0_priv);

    return (br_info == NULL ? ENOENT : 0);
}
#endif

static void
br0_free_all_locked(struct NETWIFI_S_BRIDGE *br0_priv)
{
    struct NET_BR0_INFO_ENTRY *br_info;

    BR0_LOCK(br0_priv);
    while (!br0_queue_empty(&br0_priv->as_list))
    {
        br_info = list_first_entry(&br0_priv->as_list, struct NET_BR0_INFO_ENTRY, br0_list);
        _br0_free(br0_priv, br_info);
    }
	br0_priv->fast_entry = NULL;
	
	memset(br0_priv->fast_ip, 0, 4);
	memset(br0_priv->fast_mac, 0, ETH_ALEN);
    BR0_UNLOCK(br0_priv);
}

static int
br0_free_all(struct ssv_vif *vif)
{
    br0_free_all_locked(vif->bridge_priv);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define __vlan_hdr_del() \
{\
	struct ethhdr *ehdr = (struct ethhdr *)skb->data; \
	if (ehdr->h_proto == __constant_htons(ETH_P_8021Q)) { \
		b_vlan_frame = 1;									\
		vlan_hdr = *((unsigned short *)(&ehdr[1]));			\
		memmove(skb->data+4, skb->data, ETH_ALEN*2);			\
		skb_pull(skb, 4);									\
		ehdr = (struct ethhdr *)skb->data;					\
	}	\
}


#define __vlan_hdr_add() \
{ \
	if (b_vlan_frame) {\
		skb_push(skb, 4);\
		ehdr = (struct ethhdr *)skb->data;			\
		memmove(skb->data, skb->data+4, ETH_ALEN*2);		\
		ehdr->h_proto = __constant_htons(ETH_P_8021Q);\
		*((unsigned short *)(&ehdr[1]))= vlan_hdr;\
	}\
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static unsigned long __br0_timeout(void)
{
	unsigned long timeout;

	timeout = jiffies - NET_BR0_AGEING_TIME*HZ;

	return timeout;
}
static int __br0_has_expired(struct NET_BR0_INFO_ENTRY *fdb)
{
	if (time_before_eq(fdb->ageing_timer, __br0_timeout()))
		return 1;

	return 0;
}

void br0_info_expire(struct ssv_vif *vif)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
    struct NET_BR0_INFO_ENTRY *br_info, *br_info_next;
	if (time_before_eq(br0_priv->ageing_timer, __br0_timeout()) == 0) {
		return;
	}

    BR0_LOCK(br0_priv);
    list_for_each_entry_safe(br_info, br_info_next, &br0_priv->as_list, br0_list)
    {
		if (__br0_has_expired(br_info)) {
			u8 *mac = br_info->u.info.macaddr;
			u8 *ipaddr = br_info->u.info.ipaddr;
			SSV_LOG_DBG("Br0:<WARNING> br0_info_expire %x:%x:%x:%x:%x:%x, ip %d.%d.%d.%d\n",
									mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
			_br0_free(br0_priv, br_info);
			if (br_info == br0_priv->fast_entry) {
				br0_priv->fast_entry = NULL;				
				memset(br0_priv->fast_ip, 0, 4);
				memset(br0_priv->fast_mac, 0, ETH_ALEN);
			}
		}
    }
    BR0_UNLOCK(br0_priv);
	br0_priv->ageing_timer = jiffies;
}

static int ssv_bridge_network_find_and_replace(struct ssv_vif *vif,
				struct sk_buff *skb, unsigned char *ipaddr)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
	struct NET_BR0_INFO_ENTRY *entry;
	int ret = 0;

    BR0_LOCK(br0_priv);
	entry = _br0_find_netinfo_ip(br0_priv, ipaddr); //根据br0 IP地址找到bridge entry结构体
	if (entry)
	{
		if (!__br0_has_expired(entry))
		{
			// replace the destination mac address
			memcpy(skb->data, entry->u.info.macaddr, ETH_ALEN);  //skb->data ?= skb->h_dest
			#ifdef FMAC_BR_DEBUG
			SSV_LOG_DBG("[%s]:Br0:rx change node [%x:%x:%x:%x:%x:%x]->[%x:%x:%x:%x:%x:%x], ip %d.%d.%d.%d\n",vif->ndev->name,
				skb->data[0],skb->data[1],skb->data[2],skb->data[3],skb->data[4],skb->data[5],				
											 entry->u.info.macaddr[0], 
											 entry->u.info.macaddr[1], 
											 entry->u.info.macaddr[2], 
											 entry->u.info.macaddr[3], 
											 entry->u.info.macaddr[4], 
											 entry->u.info.macaddr[5],
											 ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
			#endif /* FMAC_BR_DEBUG */								 
			entry->ageing_timer = jiffies;
		}
		else {
			//SSV_LOG_DBG("<WARNING>__br0_has_expired\n");	
		}
		ret = 1;
	}
    BR0_UNLOCK(br0_priv);
	return ret;
}

static int __ssv_bridge_change_rxhdr(struct ssv_vif *vif, struct sk_buff *skb)
{
	struct NETWIFI_S_BRIDGE *priv = vif->bridge_priv;
	struct ethhdr *ehdr = (struct ethhdr *)skb->data;

	if (skb == NULL)
		return -1;

    //SSV_LOG_DBG("rxhdr in (0x%x),daddr[%pM],saddr[%pM]\n",ntohs(ehdr->h_proto), skb->data, skb->data + 6);
	switch (ehdr->h_proto) {

		case __constant_htons(ETH_P_IP):
		{
			struct iphdr* iph = (struct iphdr *)(ehdr + 1);

			if (!ssv_bridge_network_find_and_replace(vif, skb, (unsigned char *) &iph->daddr)) {
				if (*((unsigned char *)&iph->daddr + 3) == 0xff) {
					// L2 is unicast but L3 is broadcast, make L2 bacome broadcast
					memset(skb->data, 0xff, ETH_ALEN);
				}
				else {
					#ifdef FMAC_BR_DEBUG
					{
						u8 * ipaddr = (unsigned char *) &iph->daddr;
						// forward unknow IP packet to upper TCP/IP					
						SSV_LOG_DBG("br0: Replace DA with BR's MAC [%x:%x:%x:%x:%x:%x]->[%x:%x:%x:%x:%x:%x], ip %d.%d.%d.%d\n",
							skb->data[0],skb->data[1],skb->data[2],skb->data[3],skb->data[4],skb->data[5],				
												priv->br_mac[0], 
												priv->br_mac[1], 
												priv->br_mac[2], 
												priv->br_mac[3], 
												priv->br_mac[4], 
												priv->br_mac[5],
												ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
					}
					
					#endif /* FMAC_BR_DEBUG */
					memcpy(skb->data, priv->br_mac, ETH_ALEN);			
				}
			}
			break;
		}
		case __constant_htons(ETH_P_ARP):
		{
			/*|                                                                                                                       |
			* |     14B         2B          2B       1B   1B   2B          6B            4B                 6B              4B        |
			* +------------+-----------+-----------+----+----+----+---------------+-----------------+---------------+-----------------+
			* |   ethhdr   | hard type | prot type | hs | ps | op |Eth source addr|Proto source addr|Eth target addr|Proto target addr|
			* +------------+-----------+-----------+----+----+----+---------------+-----------------+---------------+-----------------+
			*  			   |<------------------------  28 ARP request/reply --------------------------------------------------------->|
			*/
			
			struct arphdr *arp = (struct arphdr *)(ehdr + 1);
			__be32 src_ipaddr, tgt_ipaddr;
			u8 *sip, *tip;
			u8 *src_devaddr, *tgt_devaddr;
			char *arpptr = (char *)(arp + 1);
			
			src_devaddr = arpptr;
			arpptr += ETH_ALEN;
            sip = arpptr;//modify
			memcpy(&src_ipaddr, arpptr, sizeof(u32));
			arpptr += sizeof(u32);
			tgt_devaddr = arpptr;
			arpptr += ETH_ALEN;
            tip = arpptr;//modify
			memcpy(&tgt_ipaddr, arpptr, sizeof(u32));
#ifdef FMAC_BR_DEBUG
           SSV_LOG_DBG("arp: before ehdr chg [%pM][%pM],src_devaddr[%pM],tgt_devaddr[%pM],src_ipaddr[%d.%d.%d.%d],tgt_ipaddr[%d.%d.%d.%d]\n",skb->data,
                    skb->data+6,src_devaddr,tgt_devaddr,sip[0],sip[1],sip[2],sip[3],tip[0],tip[1],tip[2],tip[3]);
#endif /* FMAC_BR_DEBUG */
			ssv_bridge_network_find_and_replace(vif, skb, (unsigned char *) &tgt_ipaddr);
				// change to ARP target mac address to Lookup result
				memcpy(tgt_devaddr, skb->data, ETH_ALEN);

#ifdef FMAC_BR_DEBUG
			SSV_LOG_DBG("arp:after ehdr chg [%pM][%pM],src_devaddr[%pM],tgt_devaddr[%pM],src_ipaddr[%d.%d.%d.%d],tgt_ipaddr[%d.%d.%d.%d]\n",skb->data,
                    skb->data+6,src_devaddr,tgt_devaddr,sip[0],sip[1],sip[2],sip[3],tip[0],tip[1],tip[2],tip[3]);
			SSV_LOG_DBG("\33[31m%s():%d\33[0m\r\n",__FUNCTION__,__LINE__);
#endif /* FMAC_BR_DEBUG */

			break;
		}

		default:
			break;
	}
   // SSV_LOG_DBG("rxhdr in (0x%x),daddr[%pM],saddr[%pM]\n",ntohs(ehdr->h_proto), skb->data, skb->data + 6);	
	return 0;
}

int ssv_bridge_hash_update(struct ssv_vif *vif, struct sk_buff *skb)
{
	struct ethhdr *ehdr = (struct ethhdr *)skb->data;

	if (skb == NULL)
		return -1;

	switch (ehdr->h_proto ) {

		case  __constant_htons(ETH_P_IP):
		{
			struct iphdr* iph = (struct iphdr *)(ehdr + 1);

			if (((unsigned char*)(iph) + (iph->ihl<<2)) >= (skb->data + ETH_HLEN + skb->len))
			{
				SSV_LOG_DBG("IP packet len error!!\n");
				return -1;
			}


			//some muticast with source IP is all zero, maybe other case is illegal
			//in class A, B, C, host address is all zero or all one is illegal
			if (iph->saddr == 0)
				break;
			//record source IP address and , source mac address into database
			br0_add(vif, skb->data + ETH_ALEN, (u8 *)&iph->saddr);

			break;			
		}

		case  __constant_htons(ETH_P_ARP):
		{
			struct arphdr *arp = (struct arphdr *)(ehdr + 1);
			__be32 src_ipaddr, tgt_ipaddr;
			char *src_devaddr, *tgt_devaddr;
			const char *arpptr = (char *)(arp + 1);
			
			src_devaddr = (char *)arpptr;
			arpptr += ETH_ALEN;
			memcpy(&src_ipaddr, arpptr, sizeof(u32));
			arpptr += sizeof(u32);
			tgt_devaddr = (char *)arpptr;
			arpptr += ETH_ALEN;
			memcpy(&tgt_ipaddr, arpptr, sizeof(u32));

			if (arp->ar_pro != __constant_htons(ETH_P_IP))
			{
				SSV_LOG_DBG("BR0: arp protocol unknown (%4x)!\n", htons(arp->ar_pro));
				return -1;
			}
			//some muticast with source IP is all zero 
			if (src_ipaddr == 0)
			{
				break;
			}
				
			//if (memcmp(src_devaddr,NETDEV_HWADDR(sdata),6)) {
				//record source
				// change to ARP sender mac address to wlan STA address
                br0_add(vif, src_devaddr, (u8*)&src_ipaddr);
		        memcpy(src_devaddr, NETDEV_HWADDR(vif), ETH_ALEN);
				//SSV_LOG_DBG(" src_devaddr replace by [%pM]\n", src_devaddr);	

			//}

			break;
		}

		default:
			break;
	}
	
	return 0;
}

static int ssv_bridge_change_rxhdr(struct ssv_vif *vif, struct sk_buff *skb)
{
	struct ethhdr *ehdr = (struct ethhdr *)skb->data;
	int b_vlan_frame = 0;
	int ret = 0;
	u16 vlan_hdr = 0;
	void *br_port = NULL;
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
	int need_look = 1;
	
	if (br0_priv == 0)
		return -2;

	rcu_read_lock();
	br_port = rcu_dereference(vif->ndev->rx_handler_data);
	rcu_read_unlock();

	if (br_port &&
		( (*(u32 *)br0_priv->br_mac) == 0 && (*(u16 *)(br0_priv->br_mac + 4)) == 0 )) {
		SSV_LOG_DBG("Re-init br0_netdev_open() due to br_mac==0!\n");
		br0_netdev_open(vif->ndev);
	}
	
	if (br_port) {

		__vlan_hdr_del();

		if (!is_valid_ether_addr(ehdr->h_source)) 
			return -2;
		
		if ((NL80211_IFTYPE_STATION == SSV_VIF_TYPE(vif)) &&	 //sta recv unicast packet, sta change source addr to bridge mac?
			(is_unicast_ether_addr(ehdr->h_dest))) {			
			/*
				*	This function look up the destination network address from
				*	the NAT2.5 database. Return value = -1 means that the
				*	corresponding network protocol is NOT support.
				*/
			if (ehdr->h_proto == __constant_htons(ETH_P_IP)) {
				struct iphdr* iph = (struct iphdr *)(ehdr + 1);
				if (br0_priv->fast_entry && 
					!memcmp(br0_priv->fast_ip, &iph->daddr, 4)) { 
					memcpy(skb->data, br0_priv->fast_mac, ETH_ALEN);
					br0_priv->fast_entry->ageing_timer = jiffies;
					need_look = 0;	
					//SSV_LOG_DBG("%s %d fast_entry.\n",__func__,__LINE__);		
				}
			}	
			if (need_look) {
				//SSV_LOG_DBG("%s %d need_look.\n",__func__,__LINE__);	
				ret = __ssv_bridge_change_rxhdr(vif, skb);
			}
		}
		
		//Return true if address is link local reserved addr (01:80:c2:00:00:0X) 
		if (unlikely(is_link_local_ether_addr(ehdr->h_dest))) { //such as ASUS2G send 802.1d frame cause system die!
			//SSV_LOG_DBG("RX DROP: 802.1d.\n");
			return -1;
		}

		__vlan_hdr_add();

	}

	/// br0
	#if 1
	if (memcmp(br0_priv->br_mac, ehdr->h_source, ETH_ALEN) == 0) {

		/*SSV_LOG_DBG("BR0[%s]: sourceHw==br0Hw (%2x:%2x)! h_proto %x, iftype %d.\n",
					vif->ndev->name, br0_priv->br_mac[4], br0_priv->br_mac[5], ntohs(ehdr->h_proto), SSV_VIF_TYPE(vif)); */
		return -1;
	}
	#endif
	return ret;
}

void *ssv_bridge_updata_fast_info(struct ssv_vif *vif, unsigned char *mac, unsigned char *ipaddr)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
	struct NET_BR0_INFO_ENTRY *entry;

	entry = _br0_find_netinfo_ip(br0_priv, ipaddr);

	if (entry) {
		//if ip addr is the same, but mac addr is not the same ,return NULL;
		if (memcmp(mac, entry->u.info.macaddr, ETH_ALEN)) {
			entry = NULL;
		}			
	}
	return entry;
}
/////////////////////////////////////////////////////////////////////////
//#include "dhcpd.h"

#define DHCP_MAGIC			0x63825363
#define BROADCAST_FLAG		0x8000
struct dhcpMessage {
	u_int8_t op;
	u_int8_t htype;
	u_int8_t hlen;
	u_int8_t hops;
	u_int32_t xid;
	u_int16_t secs;
	u_int16_t flags;
	u_int32_t ciaddr;
	u_int32_t yiaddr;
	u_int32_t siaddr;
	u_int32_t giaddr;
	u_int8_t chaddr[16];
	u_int8_t sname[64];
	u_int8_t file[128];
	u_int32_t cookie;
	u_int8_t options[308]; /* 312 - cookie */
};


#define IS_BOOTP_PORT(src_port, des_port) ((((src_port) == 67)&&((des_port) == 68)) || \
										   (((src_port) == 68)&&((des_port) == 67)))

static int is_dhcp_frame(struct sk_buff *skb)
{
	const struct iphdr *ip;
	struct ethhdr *ehdr = (struct ethhdr *) skb->data;

	ip = (struct iphdr *)((u8*)ehdr + sizeof(struct ethhdr));
	if (ehdr->h_proto == htons(ETH_P_IP)) {
		if (IPPROTO_UDP == ip->protocol) {
			struct udphdr *udph = (struct udphdr *)((u8*)ip + (ip->ihl<<2));
			if (IS_BOOTP_PORT(ntohs(udph->source), ntohs(udph->dest))) {
				return 1;
			}
		}
	}
	return 0;
}

void ssv_tx_set_dhcp_bcast_flag(struct ssv_vif *vif, struct sk_buff *skb)
{
	//struct ethhdr *ehdr = (struct ethhdr *)skb->data;
	if (skb == NULL)
		return;

	if (is_dhcp_frame(skb)) // DHCP request
	{
		struct iphdr* iph = (struct iphdr *)(skb->data + ETH_HLEN);
		struct udphdr *udph = (struct udphdr *)((u8 *)iph + (iph->ihl << 2));
		struct dhcpMessage *dhcph =	(struct dhcpMessage *)((u8 *)udph + sizeof(struct udphdr));

		if (dhcph->cookie == __constant_htonl(DHCP_MAGIC)) // match magic word
		{
			if (!(dhcph->flags & htons(BROADCAST_FLAG))) // if not broadcast
			{
				register int sum = 0;

				dhcph->flags |= htons(BROADCAST_FLAG);
				// recalculate checksum
				sum = ~(udph->check) & 0xffff;
				sum += dhcph->flags;
				while(sum >> 16)
					sum = (sum & 0xffff) + (sum >> 16);
				udph->check = ~sum;
			}
		}
	}
}


//struct ssv_sta *ssv_get_sta(struct ssv_softc *sc, const u8 *mac_addr)
#if 0
static struct ssv_sta *sta_info_get_by_addr(struct ssv_softc *sc, const u8 *sta_addr)
{
	struct ssv_sta *sta = NULL;
	int i;

	
	//for (i = 0; i < (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX); i++) //是否需要13个sta?
	for (i = 0; i < NX_REMOTE_STA_MAX; i++)
	{
		sta = &sc->sta_table[i];
		if (!sta->valid)
			continue;

		SSV_LOG_DBG("sta table [%d] mac[%pM]\n", i, sta->mac_addr);
		//if (memcmp(sta_addr, sta->mac_addr, ETH_ALEN) == 0 && (i != 11)) //11为softap itself? 实际印12, MAC为全0
		if (memcmp(sta_addr, sta->mac_addr, ETH_ALEN) == 0)
		{
			return sta;
		}
	}
	return NULL;
} 
#endif

/////////////////////////////////////////////////////////////////////////
//check 可以转发出去的vif，此设计通过sta br0
static struct ssv_vif *ssv_bridge_vif_check(struct sk_buff *skb, struct net_device *dev)
{
	//struct net_device *dev = source_vif->ndev;
	struct ethhdr *ehdr = (struct ethhdr *)skb->data;
	struct ssv_vif *source_vif = netdev_priv(dev);
    struct ssv_softc *sc = source_vif->sc;
	struct ssv_vif *vif = source_vif;
	struct ssv_vif *sta_vif = NULL;
	struct ssv_vif *temp_vif = NULL;
	struct NETWIFI_S_BRIDGE *br0_priv = NULL;
	
	rcu_read_lock();

	if (rcu_dereference(dev->rx_handler_data) == NULL) {
		goto exit_rcu;
	}

	if (memcmp(ehdr->h_dest, ehdr->h_source, ETH_ALEN) == 0) {
		goto exit_rcu;
	}		
	
	list_for_each_entry_rcu(temp_vif, &sc->vifs, list) {
		if (NL80211_IFTYPE_STATION != SSV_VIF_TYPE(temp_vif)) { // find sta interface
			continue;
		}

		if ((temp_vif->sta.ap == NULL) || (temp_vif->sta.ap->valid == false)) //sta interface valid
			continue;

		sta_vif = temp_vif;
		break;
	}
	
	if (sta_vif == NULL) {
		goto exit_rcu;
	}		

	br0_priv = sta_vif->bridge_priv;
	//SSV_LOG_DBG("[%s] sta_vif->drv_vif_index %d.\n", __func__, sta_vif->drv_vif_index);

	//	__vlan_hdr_del();
    BR0_LOCK(br0_priv);
	if (ehdr->h_proto == __constant_htons(ETH_P_IP)) {
		struct iphdr *iph = (struct iphdr *)(ehdr + 1);
		u8* daip = (u8*)&iph->daddr;
		struct NET_BR0_INFO_ENTRY *entry = NULL;		
		struct ssv_sta *sta = NULL;

		//如果是单播封包，修改h_dest的目标地址为home AP或 本网桥下挂载的sta ip
		if (is_unicast_ether_addr(ehdr->h_dest)) {	
			entry = _br0_find_netinfo_ip(br0_priv, daip);
			if (entry) {
				sta = ssv_get_sta(sc, entry->u.info.macaddr);
				//SSV_LOG_DBG("entry->u.info.macaddr");
			}
			if (sta == NULL) {
				sta = ssv_get_sta(sc, ehdr->h_dest);
			}
			if (sta == NULL) {
				vif = sta_vif; //br0 sta
				memcpy(ehdr->h_dest, vif->sta.ap->mac_addr, ETH_ALEN); //homeap bssid?
	        } else {
				vif = sc->vif_table[sta->vif_idx]; //找到的sta,可能是br0记录的，可能是根据macaddr找到的
				memcpy(ehdr->h_dest, sta->mac_addr, ETH_ALEN);
	        }
			
		} else {
			
		}
#ifdef FMAC_BR_DEBUG
		{
			u8* saip = (u8*)&iph->saddr;
			SSV_LOG_DBG("%s:ehdr->h_dest [%pM], ehdr->h_source[%pM]\n",__func__, ehdr->h_dest, ehdr->h_source);
			SSV_LOG_DBG("%s:dip[%d:%d:%d:%d],sip[%d:%d:%d:%d]\n",__func__, daip[0], daip[1], daip[2], daip[3], saip[0], saip[1], saip[2], saip[3]);
			SSV_LOG_DBG("%s:[%s]->[%s],[%pM],sta[%p],entry[%p]\n",__func__, source_vif->ndev->name, vif->ndev->name, ehdr->h_dest, sta, entry);
		}
			
	#endif /* FMAC_BR_DEBUG */
	} else if (ehdr->h_proto == __constant_htons(ETH_P_ARP)) {
		struct arphdr *arp = (struct arphdr *)(ehdr + 1);
		const char *arpptr = (char *)(arp + 1);
		u8* damac = (char *)arpptr + ETH_ALEN + 4;
		u8* daip = (char *)arpptr + ETH_ALEN * 2 + 4;
		struct NET_BR0_INFO_ENTRY *entry = _br0_find_netinfo_ip(br0_priv, (unsigned char *)daip);
		u8 broadcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
		struct ssv_sta *sta = NULL;

		//ip entry find sta
		if (entry) {
			sta = ssv_get_sta(sc, entry->u.info.macaddr);
			//SSV_LOG_DBG("entry sta daip[%d:%d:%d:%d], macaddr[%pM]\n",daip[0], daip[1], daip[2], daip[3], entry->u.info.macaddr);
		}
		//da mac find sta
		if (sta == NULL) {
			sta = ssv_get_sta(sc, damac);
			//SSV_LOG_DBG("sta damac[%pM]\n", damac);
		}
		if (sta == NULL) {
			vif = sta_vif;
			damac = (char *)arpptr + ETH_ALEN + 4;
			//SSV_LOG_DBG("%s:mac[%pM] not in sta list\n", __func__, damac);
		} else {
			vif = sc->vif_table[sta->vif_idx];
			damac = sta->mac_addr;
		}
		
		if (arp->ar_op == htons(ARPOP_REQUEST)) {
			memcpy(ehdr->h_dest, broadcast, ETH_ALEN);
		} else if (arp->ar_op == htons(ARPOP_REPLY)) {
			memcpy(ehdr->h_dest, damac, ETH_ALEN);
		}					
		//SSV_LOG_DBG("arp:dip[%d:%d:%d:%d],mac[%pM]\n",daip[0], daip[1], daip[2], daip[3], damac);

	} else if (ehdr->h_proto == __constant_htons(ETH_P_PAE)) {

		struct ssv_sta *sta = NULL;		
		sta = ssv_get_sta(sc, ehdr->h_dest);		
		if (sta == NULL) {
			goto exit_unlock;
		}

		if (sc->vif_table[sta->vif_idx] == source_vif) {
			goto exit_unlock;
		}

		memcpy(ehdr->h_dest, sta->mac_addr, ETH_ALEN);
		vif = sc->vif_table[sta->vif_idx];
	}

exit_unlock:
    BR0_UNLOCK(br0_priv);
exit_rcu:
	rcu_read_unlock();

	return vif;
}

static int ssv_bridge_change_txhdr(struct ssv_vif *vif, struct sk_buff *skb)
{
	struct net_device *dev = vif->ndev;
	struct ethhdr *ehdr = (struct ethhdr *)skb->data;
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;
	void *br_port = NULL;
	u16 vlan_hdr = 0;
	int b_vlan_frame = 0;
	int need_insert = 1;

	//SSV_LOG_DBG("%s %d\n",__func__,__LINE__);
	rcu_read_lock();
	br_port = rcu_dereference(dev->rx_handler_data);
	rcu_read_unlock();

	if (br_port)
	{
		if (br0_priv == NULL)
			return 0;
		
		{			
			__vlan_hdr_del();

			if (ehdr->h_proto == __constant_htons(ETH_P_IP)) {
				struct iphdr* iph = (struct iphdr *)(ehdr + 1);
				if ((memcmp(ehdr->h_source, br0_priv->fast_mac, ETH_ALEN) == 0)  //fast_mac means br0 local addr
					&& (memcmp(&iph->saddr, br0_priv->fast_ip, 4) == 0)) {
					if (br0_priv->fast_entry) {
						br0_priv->fast_entry->ageing_timer = jiffies;
						need_insert = 0;
						//SSV_LOG_DBG("br0: br0_priv->fast_mac [%pM]\n",br0_priv->fast_mac);
					}
					else {
						memset(br0_priv->fast_mac, 0, ETH_ALEN);
						memset(br0_priv->fast_ip, 0, 4);
					}
				} else {
					br0_priv->fast_entry = (struct NET_BR0_INFO_ENTRY *)ssv_bridge_updata_fast_info(vif, ehdr->h_source, (unsigned char*)&iph->saddr);
					if (br0_priv->fast_entry != NULL) {
						memcpy(br0_priv->fast_mac, ehdr->h_source, ETH_ALEN);
						memcpy(br0_priv->fast_ip, &iph->saddr, 4);
						br0_priv->fast_entry->ageing_timer = jiffies;
						#ifdef FMAC_BR_DEBUG
						SSV_LOG_DBG("br0: update fast_mac [%x:%x:%x:%x:%x:%x], ip %d.%d.%d.%d\n",			
																br0_priv->fast_mac[0], 
																br0_priv->fast_mac[1], 
																br0_priv->fast_mac[2], 
																br0_priv->fast_mac[3], 
																br0_priv->fast_mac[4], 
																br0_priv->fast_mac[5],
																br0_priv->fast_ip[0],
																br0_priv->fast_ip[1],
																br0_priv->fast_ip[2],
																br0_priv->fast_ip[3]);
						#endif /* FMAC_BR_DEBUG */										
						need_insert = 0;							
					}
				}
			}

			if (need_insert) {
				if (ssv_bridge_hash_update(vif, skb) < 0) {
					SSV_LOG_DBG("TX DROP: ssv_bridge_change_txhdr fail!\n");
					return -1;
				}
			}

			__vlan_hdr_add();

		}
		//if SA == br_mac && skb== IP  => copy SIP to br_ip 
		if (!memcmp(ehdr->h_source, br0_priv->br_mac, ETH_ALEN) &&
			(ehdr->h_proto == __constant_htons(ETH_P_IP))) {
				memcpy(br0_priv->br_ip, skb->data + WLAN_ETHHDR_LEN + 12, 4);
		}
			
		if (memcmp(ehdr->h_dest, NETDEV_HWADDR(vif), ETH_ALEN) == 0) {
			SSV_LOG_DBG("[%s] may error![%s]\n", __func__,vif->ndev->name);
			return 0;
		}
		//change source mac to station macaddr
		memcpy(ehdr->h_source, NETDEV_HWADDR(vif), ETH_ALEN);
		ssv_tx_set_dhcp_bcast_flag(vif, skb); //对所有的dhcp packet设定bcast flag
	}

	return 0;
}

void ssv_bridge_flush(struct ssv_vif *vif)
{
    struct NETWIFI_S_BRIDGE *br0_priv = vif->bridge_priv;

	if (br0_priv == NULL) {
		return;
	}		

    br0_free_all_locked(br0_priv);	
}

#define CONFIG_BR_EXT_BRNAME "br0"
static struct net_device *ieee80211_get_br_dev(struct net_device *netdev)
{
	return netdev_master_upper_dev_get_rcu(netdev); /* required to get upper dev */
}

void br0_netdev_open(struct net_device *netdev)
{
	struct ssv_vif *ssv_vif = netdev_priv(netdev);
	struct NETWIFI_S_BRIDGE *br0_priv  = ssv_vif->bridge_priv;

	if (br0_priv == NULL)
		return;
	
	rcu_read_lock();

	{
		if (rcu_dereference(netdev->rx_handler_data))
		{
			struct net_device *br_netdev;
#if 1			
			br_netdev = ieee80211_get_br_dev(netdev);

			if (br_netdev == NULL) {
				SSV_LOG_DBG("br_netdev failed!");
			} else {
				SSV_LOG_DBG("%s,br_netdev[%s]\n",__func__,br_netdev->name);
				memcpy(br0_priv->br_mac, br_netdev->dev_addr, ETH_ALEN);
			}
#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
			br_netdev = dev_get_by_name(CONFIG_BR_EXT_BRNAME);
#else	// (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
			struct net *devnet = dev_net(netdev);
			br_netdev = dev_get_by_name(devnet, CONFIG_BR_EXT_BRNAME);
#endif	// (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))

			if (br_netdev) {
				memcpy(br0_priv->br_mac, br_netdev->dev_addr, ETH_ALEN);
				dev_put(br_netdev);
			} else {
				SSV_LOG_DBG("%s()-%d: dev_get_by_name(%s) failed!", __FUNCTION__, __LINE__, CONFIG_BR_EXT_BRNAME);
			}
#endif
		} else {
			SSV_LOG_DBG("%s()-%d: dev_get_by_name(%s) failed2!", __FUNCTION__, __LINE__, CONFIG_BR_EXT_BRNAME);
		}
		
	}

	rcu_read_unlock();
}

#if 1
int deliver_dhcp_skb(struct ssv_softc *sc, struct ssv_vif *vif, struct sk_buff *skb)
{
    struct ssv_vif *deliver_vif;
    struct ethhdr *ehdr_deliver = NULL;
    struct sk_buff *deliver_skb = NULL;

    list_for_each_entry_rcu(deliver_vif, &sc->vifs, list) {

        struct net_device *deliver_dev = deliver_vif->ndev;

        if (!netif_carrier_ok(deliver_dev)) {
            continue;
        }

        if (deliver_vif == vif) {
			continue;
		}
           
        if ((NL80211_IFTYPE_AP != SSV_VIF_TYPE(deliver_vif)) &&
            (NL80211_IFTYPE_STATION != SSV_VIF_TYPE(deliver_vif))) {
            continue;
        }
        if (is_dhcp_frame(skb)) {
            /*
			* send multicast frames both to higher layers in
			* local net stack and back to the wireless medium
			*/
            deliver_skb = skb_copy(skb, GFP_ATOMIC);
            deliver_skb->dev = deliver_vif->ndev;
            ehdr_deliver = (struct ethhdr *)deliver_skb->data;
            memcpy(ehdr_deliver->h_source, &sc->maddr[vif->drv_vif_index][0], ETH_ALEN);
			break;
        }
    }

    if (deliver_skb) {
        /* send to wireless media */
        deliver_skb->protocol = htons(ETH_P_802_3);
        skb_reset_network_header(deliver_skb);
        skb_reset_mac_header(deliver_skb);
        //SSV_LOG_DBG("deliver skb iftype22 %d \n",SSV_VIF_TYPE(deliver_vif));
        local_bh_disable();
        if (ssv_start_xmit(deliver_skb, deliver_vif->ndev) == NETDEV_TX_OK) {
            //SSV_LOG_DBG("[%s][%d] success!\n",__func__,__LINE__);
			local_bh_enable();
			return 0;           
        } else {
            kfree_skb(deliver_skb);
			//SSV_LOG_DBG("[%s][%d] fail!\n",__func__,__LINE__);
        }
        local_bh_enable();
    }
    return -1;
}
#endif

void fmac_bridge_dump_skb_info(unsigned char *tag, char *dir, struct ssv_vif *vif, struct sk_buff *skb)
{
#ifdef FMAC_BR_DEBUG
	//struct net_device *dev = vif->ndev;
	struct ethhdr *ehdr = (struct ethhdr *)skb->data;
	enum data_frame_types data_type = SSV_DATA_UNKNOW;

	data_type = ssv_get_data_frame_type(skb);

	if (tag != NULL) {
		SSV_LOG_DBG("====== [%s] ======\n", tag);
	}
		
	SSV_LOG_DBG("interface [%s] --> %s:\n", vif->ndev->name, dir);
	SSV_LOG_DBG("ehdr->h_source MAC [%pM]\n", ehdr->h_source);
	SSV_LOG_DBG("ehdr->h_dest MAC [%pM]\n", ehdr->h_dest);
	SSV_LOG_DBG("eth proto [%s]\n", frame_proto2string[ntohs(ehdr->h_proto)]);
	if (data_type != SSV_DATA_UNKNOW) {
		SSV_LOG_DBG("IP proto  [%s]\n", frame_type2string[data_type]);
	}		
#endif /* FMAC_BR_DEBUG */
}

struct ssv_vif *ssv_bridge_tx_change(struct sk_buff *skb, struct net_device *dev)
{
	struct ssv_vif *ssv_vif = netdev_priv(dev);
	struct ssv_vif *tmp_vif = NULL;

	tmp_vif = ssv_bridge_vif_check(skb, dev);
	//SSV_LOG_DBG("[%s] ssv_vif index %d [%s], tmp_vif index %d [%s].\n", __func__, ssv_vif->drv_vif_index, ssv_vif->ndev->name, tmp_vif->drv_vif_index, tmp_vif->ndev->name);
	fmac_bridge_dump_skb_info("before txhdr", TX_STR, ssv_vif, skb);

	if (tmp_vif != ssv_vif) {
		ssv_vif = tmp_vif;
		dev = ssv_vif->ndev;
	}

	if (NL80211_IFTYPE_STATION == SSV_VIF_TYPE(ssv_vif))
	{		
		if (ssv_bridge_change_txhdr(ssv_vif, skb) == -1) {//-1: TX DROP: ssv_bridge_change_txhdr fail
			SSV_LOG_DBG("[%s] [%s] drop frame.\n", __func__, ssv_vif->ndev->name);
			dev_kfree_skb_any(skb);
			skb = NULL;
		} else {
			fmac_bridge_dump_skb_info("after txhdr", TX_STR, ssv_vif, skb);
		}								            
	}
	
	return ssv_vif;
} 

int ssv_bridge_rx_change(struct ssv_vif *ssv_vif, struct sk_buff *skb)
{
	struct ssv_softc *sc = (struct ssv_softc *)ssv_vif->sc;

	fmac_bridge_dump_skb_info("before rxhdr", RX_STR, ssv_vif, skb);
	if (ssv_bridge_change_rxhdr(ssv_vif, skb) < 0) {
		//SSV_LOG_DBG("[%s] [%s] drop frame.\n", __func__,ssv_vif->ndev->name);
		return -1;
	} else {
		fmac_bridge_dump_skb_info("after rxhdr", RX_STR, ssv_vif, skb);
	}
	
	if (skb) {
		if (deliver_dhcp_skb(sc, ssv_vif, skb) == 0) {
			fmac_bridge_dump_skb_info("after change dhcp rxhdr", RX_STR, ssv_vif, skb);
		}                    
	}                  
	return 0;	
}