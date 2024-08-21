#include <linux/module.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/ip.h>
//#include <asm/system.h>
//#include <asm/io.h>
//#include <asm/irq.h>
#include "rwnx_tx.h"
#include "rwnx_defs.h"
#include "rwnx_compat.h"

#ifdef CONFIG_VNET_MODE
struct net_device *vnet_dev;
 #if  0
static void emulator_rx_packet(struct sk_buff *skb, struct net_device *dev)
{
	unsigned char *type;
	struct iphdr *ih;
	__be32 *saddr, *daddr, tmp;
	unsigned char	tmp_dev_addr[ETH_ALEN];
	struct ethhdr *ethhdr;

	struct sk_buff *rx_skb;

	ethhdr = (struct ethhdr *)skb->data;
	memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);
	memcpy(ethhdr->h_dest, ethhdr->h_source, ETH_ALEN);
	memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

	ih = (struct iphdr *)(skb->data + sizeof(struct ethhdr));
	saddr = &ih->saddr;
	daddr = &ih->daddr;

	tmp = *saddr;
	*saddr = *daddr;
	*daddr = tmp;

	type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);
	printk("tx package type = %02x\n", *type);

	*type = 0;

	ih->check = 0;		   /* and rebuild the checksum (ip needs it) */
	ih->check = ip_fast_csum((unsigned char *)ih,ih->ihl);

	rx_skb = dev_alloc_skb(skb->len + 2);
	skb_reserve(rx_skb, 2); /* align IP on 16B boundary */
	memcpy(skb_put(rx_skb, skb->len), skb->data, skb->len);

	/* Write metadata, and then pass to the receive level */
	rx_skb->dev = dev;
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_UNNECESSARY; /* don't check it */

	netif_rx(rx_skb);
}

 int virt_net_send_packet(struct sk_buff *skb, struct net_device *dev)
{
	static int cnt = 0;
	printk("virt_net_send_packet cnt = %d\n", ++cnt);

	netif_stop_queue(dev);

	emulator_rx_packet(skb, dev);

	dev_kfree_skb_any (skb);
	netif_wake_queue(dev);

	return 0;
}
#endif
extern  void rwnx_netdev_setup(struct net_device *dev);
 int virt_net_init(struct rwnx_vif  *vif)
{
    vnet_dev = alloc_netdev_mqs(0, "vnet%d", 0, rwnx_netdev_setup, NX_NB_NDEV_TXQ, 1);

    if (!vnet_dev)
        return -1;

    printk("virt_net_init %p\n", vnet_dev);

    //vif = netdev_priv(vnet_dev);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
    memcpy(vnet_dev->dev_addr, vif->wdev.address, 6);
#else
    memcpy(vnet_dev->dev_addr, vif->address, 6);
#endif
    vnet_dev->mtu = 1480;
    register_netdev(vnet_dev);

    return 0;
}

void virt_net_exit(void)
{
    //printk("virt_net_exit %p\n", vnet_dev);
    if(vnet_dev && (vnet_dev->reg_state == NETREG_REGISTERED))  {
        unregister_netdev(vnet_dev);
    }
    //free_netdev(vnet_dev);
}
#endif
