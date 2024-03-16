#include"aicwf_tcp_ack.h"
//#include"rwnx_tx.h"
//#include "aicwf_tcp_ack.h"
#include"rwnx_defs.h"
extern int intf_tx(struct rwnx_hw *priv,struct msg_buf *msg);
struct msg_buf *intf_tcp_alloc_msg(struct msg_buf *msg)
{
	//printk("%s \n",__func__);
	int len=sizeof(struct msg_buf) ;
	msg = kzalloc(len , GFP_KERNEL);
	if(!msg)
		printk("%s: alloc failed \n", __func__);
	memset(msg,0,len);
	return msg;
}
						
void intf_tcp_drop_msg(struct rwnx_hw *priv,
					    struct msg_buf *msg)
{
	//printk("%s \n",__func__);
	if (msg->skb)
		dev_kfree_skb_any(msg->skb);

	kfree(msg);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0) 
void tcp_ack_timeout(unsigned long data)
#else
void tcp_ack_timeout(struct timer_list *t)
#endif
{
	//printk("%s \n",__func__);
	struct tcp_ack_info *ack_info;
	struct msg_buf *msg;
	struct tcp_ack_manage *ack_m = NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0) 
	ack_info = (struct tcp_ack_info *)data;
#else
	ack_info = container_of(t,struct tcp_ack_info,timer);
#endif

	ack_m = container_of(ack_info, struct tcp_ack_manage,
			     ack_info[ack_info->ack_info_num]);

	write_seqlock_bh(&ack_info->seqlock);
	msg = ack_info->msgbuf;
	if (ack_info->busy && msg && !ack_info->in_send_msg) {
		ack_info->msgbuf = NULL;
		ack_info->drop_cnt = 0;
		ack_info->in_send_msg = msg;
		write_sequnlock_bh(&ack_info->seqlock);
		intf_tx(ack_m->priv, msg);//send skb
		//ack_info->in_send_msg = NULL;//add by dwx
		//write_sequnlock_bh(&ack_info->seqlock);
		//intf_tx(ack_m->priv, msg);
		return;
	}
	write_sequnlock_bh(&ack_info->seqlock);
}

void tcp_ack_init(struct rwnx_hw *priv)
{
	int i;
	struct tcp_ack_info *ack_info;
	struct tcp_ack_manage *ack_m = &priv->ack_m;

	printk("%s \n",__func__);
	memset(ack_m, 0, sizeof(struct tcp_ack_manage));
	ack_m->priv = priv;
	spin_lock_init(&ack_m->lock);
	atomic_set(&ack_m->max_drop_cnt, TCP_ACK_DROP_CNT);
	ack_m->last_time = jiffies;
	ack_m->timeout = msecs_to_jiffies(ACK_OLD_TIME);

	for (i = 0; i < TCP_ACK_NUM; i++) {
		ack_info = &ack_m->ack_info[i];
		ack_info->ack_info_num = i;
		seqlock_init(&ack_info->seqlock);
		ack_info->last_time = jiffies;
		ack_info->timeout = msecs_to_jiffies(ACK_OLD_TIME);

		#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0) 
			setup_timer(&ack_info->timer, tcp_ack_timeout,
				    (unsigned long)ack_info);
		#else
			timer_setup(&ack_info->timer,tcp_ack_timeout,0);
		#endif
	}

	atomic_set(&ack_m->enable, 1);
	ack_m->ack_winsize = MIN_WIN;
}

void tcp_ack_deinit(struct rwnx_hw *priv)
{
	int i;
	struct tcp_ack_manage *ack_m = &priv->ack_m;
	struct msg_buf *drop_msg = NULL;

	printk("%s \n",__func__);
	atomic_set(&ack_m->enable, 0);

	for (i = 0; i < TCP_ACK_NUM; i++) {
		drop_msg = NULL;

		write_seqlock_bh(&ack_m->ack_info[i].seqlock);
		del_timer(&ack_m->ack_info[i].timer);
		drop_msg = ack_m->ack_info[i].msgbuf;
		ack_m->ack_info[i].msgbuf = NULL;
		write_sequnlock_bh(&ack_m->ack_info[i].seqlock);

		if (drop_msg)
			intf_tcp_drop_msg(priv, drop_msg);//drop skb
	}
}

int tcp_check_quick_ack(unsigned char *buf,
				      struct tcp_ack_msg *msg)
{
	int ip_hdr_len;
	unsigned char *temp;
	struct ethhdr *ethhdr;
	struct iphdr *iphdr;
	struct tcphdr *tcphdr;

	ethhdr = (struct ethhdr *)buf;
	if (ethhdr->h_proto != htons(ETH_P_IP))
		return 0;
	iphdr = (struct iphdr *)(ethhdr + 1);
	if (iphdr->version != 4 || iphdr->protocol != IPPROTO_TCP)
		return 0;
	ip_hdr_len = iphdr->ihl * 4;
	temp = (unsigned char *)(iphdr) + ip_hdr_len;
	tcphdr = (struct tcphdr *)temp;
	/* TCP_FLAG_ACK */
	if (!(temp[13] & 0x10))
		return 0;

	if (temp[13] & 0x8) {
		msg->saddr = iphdr->daddr;
		msg->daddr = iphdr->saddr;
		msg->source = tcphdr->dest;
		msg->dest = tcphdr->source;
		msg->seq = ntohl(tcphdr->seq);
		return 1;
	}

	return 0;
}

int is_drop_tcp_ack(struct tcphdr *tcphdr, int tcp_tot_len,
				unsigned short *win_scale)
{
	//printk("%s \n",__func__);
	int drop = 1;
	int len = tcphdr->doff * 4;
	unsigned char *ptr;

	if(tcp_tot_len > len) {
		drop = 0;
	} else {
		len -= sizeof(struct tcphdr);
		ptr = (unsigned char *)(tcphdr + 1);

		while ((len > 0) && drop) {
			int opcode = *ptr++;
			int opsize;

			switch (opcode) {
			case TCPOPT_EOL:
				break;
			case TCPOPT_NOP:
				len--;
				continue;
			default:
				opsize = *ptr++;
				if (opsize < 2)
					break;
				if (opsize > len)
					break;

				switch (opcode) {
				/* TODO: Add other ignore opt */
				case TCPOPT_TIMESTAMP:
					break;
				case TCPOPT_WINDOW:
					if (*ptr < 15)
						*win_scale = (1 << (*ptr));
					printk("%d\n",*win_scale);
					break;
				default:
					drop = 2;
				}

				ptr += opsize - 2;
				len -= opsize;
			}
		}
	}

	return drop;
}


/* flag:0 for not tcp ack
 *	1 for ack which can be drop
 *	2 for other ack whith more info
 */

int tcp_check_ack(unsigned char *buf,
				struct tcp_ack_msg *msg,
				unsigned short *win_scale)
{
	int ret;
	int ip_hdr_len;
	int tcp_tot_len;
	unsigned char *temp;
	struct ethhdr *ethhdr;
	struct iphdr *iphdr;
	struct tcphdr *tcphdr;

	ethhdr =(struct ethhdr *)buf;
	if (ethhdr->h_proto != htons(ETH_P_IP))
		return 0;

	iphdr = (struct iphdr *)(ethhdr + 1);
	if (iphdr->version != 4 || iphdr->protocol != IPPROTO_TCP)
		return 0;

	ip_hdr_len = iphdr->ihl * 4;
	temp = (unsigned char *)(iphdr) + ip_hdr_len;
	tcphdr = (struct tcphdr *)temp;
	/* TCP_FLAG_ACK */
	if (!(temp[13] & 0x10))
		return 0;

	tcp_tot_len = ntohs(iphdr->tot_len) - ip_hdr_len;// tcp total len
	ret = is_drop_tcp_ack(tcphdr, tcp_tot_len, win_scale);
	//printk("is drop:%d \n",ret);

	if (ret > 0) {
		msg->saddr = iphdr->saddr;
		msg->daddr = iphdr->daddr;
		msg->source = tcphdr->source;
		msg->dest = tcphdr->dest;
		msg->seq = ntohl(tcphdr->ack_seq);
		msg->win = ntohs(tcphdr->window);
	}
	
	return ret;
}

/* return val: -1 for not match, others for match */
int tcp_ack_match(struct tcp_ack_manage *ack_m,
				struct tcp_ack_msg *ack_msg)
{
	int i, ret = -1;
	unsigned start;
	struct tcp_ack_info *ack_info;
	struct tcp_ack_msg *ack;

	for (i = 0; ((ret < 0) && (i < TCP_ACK_NUM)); i++) {
		ack_info = &ack_m->ack_info[i];
		do {
			start = read_seqbegin(&ack_info->seqlock);
			ret = -1;

			ack = &ack_info->ack_msg;
			if (ack_info->busy &&
			    ack->dest == ack_msg->dest &&
			    ack->source == ack_msg->source &&
			    ack->saddr == ack_msg->saddr &&
			    ack->daddr == ack_msg->daddr)
				ret = i;
		} while(read_seqretry(&ack_info->seqlock, start));
	}

	return ret;
}


void tcp_ack_update(struct tcp_ack_manage *ack_m)
{
	int i;
	struct tcp_ack_info *ack_info;

	if (time_after(jiffies, ack_m->last_time + ack_m->timeout)) {
		spin_lock_bh(&ack_m->lock);
		ack_m->last_time = jiffies;
		for (i = TCP_ACK_NUM - 1; i >= 0; i--) {
			ack_info = &ack_m->ack_info[i];
			write_seqlock_bh(&ack_info->seqlock);
			if (ack_info->busy &&
			    time_after(jiffies, ack_info->last_time +
				       ack_info->timeout)) {
				ack_m->free_index = i;
				ack_m->max_num--;
				ack_info->busy = 0;
			}
			write_sequnlock_bh(&ack_info->seqlock);
		}
		spin_unlock_bh(&ack_m->lock);
	}
}

/* return val: -1 for no index, others for index */
int tcp_ack_alloc_index(struct tcp_ack_manage *ack_m)
{
	int i, ret = -1;
	struct tcp_ack_info *ack_info;
	unsigned start;

	spin_lock_bh(&ack_m->lock);
	if (ack_m->max_num == TCP_ACK_NUM) {
		spin_unlock_bh(&ack_m->lock);
		return -1;
	}

	if (ack_m->free_index >= 0) {
		i = ack_m->free_index;
		ack_m->free_index = -1;
		ack_m->max_num++;
		spin_unlock_bh(&ack_m->lock);
		return i;
	}

	for (i = 0; ((ret < 0) && (i < TCP_ACK_NUM)); i++) {
		ack_info = &ack_m->ack_info[i];
		do {
			start = read_seqbegin(&ack_info->seqlock);
			ret = -1;
			if (!ack_info->busy) {
				ack_m->free_index = -1;
				ack_m->max_num++;
				ret = i;
			}
		} while(read_seqretry(&ack_info->seqlock, start));
	}
	spin_unlock_bh(&ack_m->lock);

	return ret;
}


/* return val: 0 for not handle tx, 1 for handle tx */
int tcp_ack_handle(struct msg_buf *new_msgbuf,
			  struct tcp_ack_manage *ack_m,
			  struct tcp_ack_info *ack_info,
			  struct tcp_ack_msg *ack_msg,
			  int type)
{
	int quick_ack = 0;
	struct tcp_ack_msg *ack;
	int ret = 0;
	struct msg_buf *drop_msg = NULL;

	//printk("%s %d",__func__,type);
	write_seqlock_bh(&ack_info->seqlock);

	ack_info->last_time = jiffies;
	ack = &ack_info->ack_msg;

	if (type == 2) {
		if (U32_BEFORE(ack->seq, ack_msg->seq)) {
			ack->seq = ack_msg->seq;
			if (ack_info->psh_flag &&
			    !U32_BEFORE(ack_msg->seq,
					       ack_info->psh_seq)) {
				ack_info->psh_flag = 0;
			}

			if (ack_info->msgbuf) {
				//printk("%lx \n",ack_info->msgbuf);
				drop_msg = ack_info->msgbuf;
				ack_info->msgbuf = NULL;
				del_timer(&ack_info->timer);
			}else{
				//printk("msgbuf is NULL \n");
			}

			ack_info->in_send_msg = NULL;
			ack_info->drop_cnt = atomic_read(&ack_m->max_drop_cnt);
		} else {
			printk("%s before abnormal ack: %d, %d\n",
			       __func__, ack->seq, ack_msg->seq);
			drop_msg = new_msgbuf;
			ret = 1;
		}
	} else if (U32_BEFORE(ack->seq, ack_msg->seq)) {
		if (ack_info->msgbuf) {
			drop_msg = ack_info->msgbuf;
			ack_info->msgbuf = NULL;
		}

		if (ack_info->psh_flag &&
		    !U32_BEFORE(ack_msg->seq, ack_info->psh_seq)) {
			ack_info->psh_flag = 0;
			quick_ack = 1;
		} else {
			ack_info->drop_cnt++;
		}

		ack->seq = ack_msg->seq;

		if (quick_ack || (!ack_info->in_send_msg &&
				  (ack_info->drop_cnt >=
				   atomic_read(&ack_m->max_drop_cnt)))) {
			ack_info->drop_cnt = 0;
			ack_info->in_send_msg = new_msgbuf;
			del_timer(&ack_info->timer);
		} else {
			ret = 1;
			ack_info->msgbuf = new_msgbuf;
			if (!timer_pending(&ack_info->timer))
				mod_timer(&ack_info->timer,
					  (jiffies + msecs_to_jiffies(5)));
		}
	} else {
		printk("%s before ack: %d, %d\n",
		       __func__, ack->seq, ack_msg->seq);
		drop_msg = new_msgbuf;
		ret = 1;
	}

	write_sequnlock_bh(&ack_info->seqlock);

	if (drop_msg)
		intf_tcp_drop_msg(ack_m->priv, drop_msg);// drop skb

	return ret;
}

int tcp_ack_handle_new(struct msg_buf *new_msgbuf,
			  struct tcp_ack_manage *ack_m,
			  struct tcp_ack_info *ack_info,
			  struct tcp_ack_msg *ack_msg,
			  int type)
{
	int quick_ack = 0;
	struct tcp_ack_msg *ack;
	int ret = 0;
	struct msg_buf *drop_msg = NULL;
	struct msg_buf * send_msg = NULL;
	//printk("",);
	write_seqlock_bh(&ack_info->seqlock);

        ack_info->last_time = jiffies;
        ack = &ack_info->ack_msg;

	if(U32_BEFORE(ack->seq, ack_msg->seq)){
		if (ack_info->msgbuf) {
			drop_msg = ack_info->msgbuf;
			ack_info->msgbuf = NULL;
			//ack_info->drop_cnt++;
		}

		if (ack_info->psh_flag &&
		    !U32_BEFORE(ack_msg->seq, ack_info->psh_seq)) {
			ack_info->psh_flag = 0;
			quick_ack = 1;
		} else {
			ack_info->drop_cnt++;
		}

		ack->seq = ack_msg->seq;

		if(quick_ack || (!ack_info->in_send_msg &&
				  (ack_info->drop_cnt >=
				   atomic_read(&ack_m->max_drop_cnt)))){
			ack_info->drop_cnt = 0;
			send_msg = new_msgbuf;
			ack_info->in_send_msg = send_msg;
			del_timer(&ack_info->timer);
		}else{
			ret = 1;
			ack_info->msgbuf = new_msgbuf;
			if (!timer_pending(&ack_info->timer))
				mod_timer(&ack_info->timer,
					  (jiffies + msecs_to_jiffies(5)));
		}
		
		//ret = 1;
	}else {
		printk("%s before ack: %d, %d\n",
		       __func__, ack->seq, ack_msg->seq);
		drop_msg = new_msgbuf;
		ret = 1;
	}

	/*if(send_msg){
		intf_tx(ack_m->priv,send_msg);
		ack_info->in_send_msg=NULL;
	}*/

	//ack_info->in_send_msg=NULL;
	
	write_sequnlock_bh(&ack_info->seqlock);

    	/*if(send_msg){
            intf_tx(ack_m->priv,send_msg);
            //ack_info->in_send_msg=NULL;
    	}*/

	if (drop_msg)
		intf_tcp_drop_msg(ack_m->priv, drop_msg);// drop skb

	return ret;

}

void filter_rx_tcp_ack(struct rwnx_hw *priv,
			      unsigned char *buf, unsigned plen)
{
	int index;
	struct tcp_ack_msg ack_msg;
	struct tcp_ack_info *ack_info;
	struct tcp_ack_manage *ack_m = &priv->ack_m;

	if (!atomic_read(&ack_m->enable))
		return;

	if ((plen > MAX_TCP_ACK) ||
	    !tcp_check_quick_ack(buf, &ack_msg))
		return;

	index = tcp_ack_match(ack_m, &ack_msg);
	if (index >= 0) {
		ack_info = ack_m->ack_info + index;
		write_seqlock_bh(&ack_info->seqlock);
		ack_info->psh_flag = 1;
		ack_info->psh_seq = ack_msg.seq;
		write_sequnlock_bh(&ack_info->seqlock);
	}
}

/* return val: 0 for not filter, 1 for filter */
int filter_send_tcp_ack(struct rwnx_hw *priv,
			       struct msg_buf *msgbuf,
			       unsigned char *buf, unsigned int plen)
{
	//printk("%s \n",__func__);
	int ret = 0;
	int index, drop;
	unsigned short win_scale = 0;
	unsigned int win = 0;
	struct tcp_ack_msg ack_msg;
	struct tcp_ack_msg *ack;
	struct tcp_ack_info *ack_info;
	struct tcp_ack_manage *ack_m = &priv->ack_m;

	if (plen > MAX_TCP_ACK)
		return 0;

	tcp_ack_update(ack_m);
	drop = tcp_check_ack(buf, &ack_msg, &win_scale);
	//printk("drop:%d win_scale:%d",drop,win_scale);
	if (!drop && (0 == win_scale))
		return 0;

	index = tcp_ack_match(ack_m, &ack_msg);
	if (index >= 0) {
		ack_info = ack_m->ack_info + index;
		if ((0 != win_scale) &&
			(ack_info->win_scale != win_scale)) {
			write_seqlock_bh(&ack_info->seqlock);
			ack_info->win_scale = win_scale;
			write_sequnlock_bh(&ack_info->seqlock);
		}

		if (drop > 0 && atomic_read(&ack_m->enable)) {
			win = ack_info->win_scale * ack_msg.win;
			if ((win_scale!=0) && (win < (ack_m->ack_winsize * SIZE_KB)))
			{	
				drop = 2;
				printk("%d %d %d",win_scale,win,(ack_m->ack_winsize * SIZE_KB));
			}
			ret = tcp_ack_handle_new(msgbuf, ack_m, ack_info,
						&ack_msg, drop);
		}

		goto out;
	}

	index = tcp_ack_alloc_index(ack_m);
	if (index >= 0) {
		write_seqlock_bh(&ack_m->ack_info[index].seqlock);
		ack_m->ack_info[index].busy = 1;
		ack_m->ack_info[index].psh_flag = 0;
		ack_m->ack_info[index].last_time = jiffies;
		ack_m->ack_info[index].drop_cnt =
			atomic_read(&ack_m->max_drop_cnt);
		ack_m->ack_info[index].win_scale =
			(win_scale != 0) ? win_scale : 1;
		
		//ack_m->ack_info[index].msgbuf = NULL;
		//ack_m->ack_info[index].in_send_msg = NULL;
		ack = &ack_m->ack_info[index].ack_msg;
		ack->dest = ack_msg.dest;
		ack->source = ack_msg.source;
		ack->saddr = ack_msg.saddr;
		ack->daddr = ack_msg.daddr;
		ack->seq = ack_msg.seq;
		write_sequnlock_bh(&ack_m->ack_info[index].seqlock);
	}

out:
	return ret;
}

void move_tcpack_msg(struct rwnx_hw *priv,
			    struct msg_buf *msg)
{
	struct tcp_ack_info *ack_info;
	struct tcp_ack_manage *ack_m = &priv->ack_m;
	int i = 0;

	if (!atomic_read(&ack_m->enable))
		return;

	//if (msg->len > MAX_TCP_ACK)
	//	return;

	for (i = 0; i < TCP_ACK_NUM; i++) {
		ack_info = &ack_m->ack_info[i];
		write_seqlock_bh(&ack_info->seqlock);
		if (ack_info->busy && (ack_info->in_send_msg == msg))
			ack_info->in_send_msg = NULL;
		write_sequnlock_bh(&ack_info->seqlock);
	}
}

