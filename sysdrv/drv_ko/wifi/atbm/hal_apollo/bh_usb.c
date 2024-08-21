#include <net/atbm_mac80211.h>
#include <linux/kthread.h>

#include "apollo.h"
#include "bh.h"
#include "bh_usb.h"
#include "hwio.h"
#include "wsm.h"
#include "sbus.h"
#include "debug.h"
#include "apollo_plat.h"
#include "sta.h"
#include "ap.h"
#include "scan.h"
//#define DBG_EVENT_LOG
#include "dbg_event.h"
#if defined(CONFIG_ATBM_APOLLO_BH_DEBUG)
#define bh_printk  atbm_printk_always
#else
#define bh_printk(...)
#endif
extern void atbm_hif_status_set(int status);
static int usb_atbm_bh(void *arg);
void atbm_monitor_pc(struct atbm_common *hw_priv);
extern 	int atbm_usb_pm(struct sbus_priv *self, bool  suspend);

int atbm_register_bh(struct atbm_common *hw_priv)
{
	int err = 0;
	struct sched_param param = { .sched_priority = 1 };
	bh_printk( "[BH] register.\n");
//	BUG_ON(hw_priv->bh_thread);
	if(hw_priv->bh_thread){
		atbm_printk_err("%s %d ,ERROR !!! hw_priv->bh_thread is runing\n",__func__,__LINE__);
		WARN_ON(hw_priv->bh_thread);
		return 0;
	}
	atomic_set(&hw_priv->bh_rx, 0);
	atomic_set(&hw_priv->bh_suspend_usb, 1);
	atomic_set(&hw_priv->bh_tx, 0);
	atomic_set(&hw_priv->bh_halt,0);
	atomic_set(&hw_priv->bh_term, 0);
	atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_RESUMED);
	
	hw_priv->wsm_rx_seq = 0;
	hw_priv->wsm_tx_seq = 0;
	hw_priv->buf_id_tx = 0;
	hw_priv->buf_id_rx = 0;
	init_waitqueue_head(&hw_priv->bh_wq);
	init_waitqueue_head(&hw_priv->bh_evt_wq);

	hw_priv->bh_thread = kthread_create(&usb_atbm_bh, hw_priv, ieee80211_alloc_name(hw_priv->hw,"usb_atbm_bh"));

	if (IS_ERR(hw_priv->bh_thread)) {
		err = PTR_ERR(hw_priv->bh_thread);
		hw_priv->bh_thread = NULL;
	} else {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 9, 8))
		sched_set_fifo_low(hw_priv->bh_thread);
#else
		sched_setscheduler(hw_priv->bh_thread,
			SCHED_FIFO, &param);
#endif
#ifdef HAS_PUT_TASK_STRUCT
		get_task_struct(hw_priv->bh_thread);
#endif
		wake_up_process(hw_priv->bh_thread);
	}
	return err;
}

void atbm_unregister_bh(struct atbm_common *hw_priv)
{
	struct task_struct *thread = hw_priv->bh_thread;
	if (WARN_ON(!thread))
		return;

	hw_priv->bh_thread = NULL;
	bh_printk(KERN_ERR "[BH] unregister.\n");
	atomic_add(1, &hw_priv->bh_term);
	wake_up(&hw_priv->bh_wq);
	kthread_stop(thread);
#ifdef HAS_PUT_TASK_STRUCT
	put_task_struct(thread);
#endif
}

int atbm_reset_driver(struct atbm_common *hw_priv)
{
	return 0;
}

int atbm_bh_suspend(struct atbm_common *hw_priv)
{

	bh_printk( "[BH] suspend.\n");
	if (hw_priv->bh_error) {
		atbm_printk_err("BH error -- can't suspend\n");
		return -EINVAL;
	}

	atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_SUSPEND);
	wake_up(&hw_priv->bh_wq);
	return atbm_wait_event_timeout_stay_awake(hw_priv,hw_priv->bh_evt_wq, hw_priv->bh_error ||
		(ATBM_APOLLO_BH_SUSPENDED == atomic_read(&hw_priv->bh_suspend)),
		 1 * HZ,false) ? 0 : -ETIMEDOUT;
}

int atbm_bh_resume(struct atbm_common *hw_priv)
{

	bh_printk( "[BH] resume.\n");
	if (hw_priv->bh_error) {
		atbm_printk_err("BH error -- can't resume\n");
		return -EINVAL;
	}

	atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_RESUME);
	wake_up(&hw_priv->bh_wq);

	return atbm_wait_event_timeout_stay_awake(hw_priv,hw_priv->bh_evt_wq,hw_priv->bh_error ||
		(ATBM_APOLLO_BH_RESUMED == atomic_read(&hw_priv->bh_suspend)),
		1 * HZ,false) ? 0 : -ETIMEDOUT;

}


void wsm_alloc_tx_buffer(struct atbm_common *hw_priv)
{
	spin_lock_bh(&hw_priv->tx_com_lock);
	++hw_priv->hw_bufs_used;
	spin_unlock_bh(&hw_priv->tx_com_lock);
}

int wsm_release_tx_buffer(struct atbm_common *hw_priv, int count)
{
	int ret = 0;

	spin_lock_bh(&hw_priv->tx_com_lock);
	hw_priv->hw_bufs_used -= count;
	spin_unlock_bh(&hw_priv->tx_com_lock);
	if (!(hw_priv->hw_bufs_used )){
		//atbm_usb_pm(hw_priv->sbus_priv,1);
		wake_up(&hw_priv->bh_evt_wq);
	}
	ret = 1;
	return ret;
}


void wsm_alloc_tx_buffer_NoLock(struct atbm_common *hw_priv)
{
	++hw_priv->hw_bufs_used;
}

int wsm_release_tx_buffer_NoLock(struct atbm_common *hw_priv, int count)
{
	int ret = 0;
	
	hw_priv->hw_bufs_used -= count;
	//printk( "wsm_release_tx_buffer(%d)(%d)\n",count,hw_priv->hw_bufs_used);
	if (!(hw_priv->hw_bufs_used )){
		wake_up(&hw_priv->bh_evt_wq);
	}
	ret = 1;
	return ret;
}
int atbm_rx_bh_cb(struct atbm_common *hw_priv,struct sk_buff *skb)
{
	struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;
	u8 wsm_seq;

	wsm = (struct wsm_hdr *)skb->data;

	wsm_len = __le32_to_cpu(wsm->len);

	wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
	wsm_seq = (__le32_to_cpu(wsm->id) >> 13) & 7;

//	BUG_ON(wsm_len > 4096);
	atbm_skb_trim(skb,0);
	atbm_skb_put(skb,wsm_len);

	if (unlikely(wsm_id == 0x0800)) {
		wsm_handle_exception(hw_priv, &skb->data[sizeof(*wsm)],wsm_len - sizeof(*wsm));
		if(wsm_recovery(hw_priv)== RECOVERY_ERR)
		{
			hw_priv->bh_error = 1;
			wake_up(&hw_priv->bh_wq);
		}
		goto __free;
	}
	//
	//add because usb not reset when rmmod driver, just drop error frame  
	//
	if((hw_priv->wsm_caps.firmwareReady==0)
		&&((wsm_seq != hw_priv->wsm_rx_seq)
		||((wsm_id & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) != WSM_STARTUP_IND_ID))){
		atbm_printk_err("rx wsm_seq err1 (rxseq=%d need_rxseq=%d) len(%d) wsm_id %x firmwareReady %d\n",wsm_seq,hw_priv->wsm_rx_seq,skb->len,wsm_id,hw_priv->wsm_caps.firmwareReady);
		//BUG_ON(1);
		if((wsm_id & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) == WSM_STARTUP_IND_ID)
			atbm_bh_halt(hw_priv);
		goto __free;
	}
	else if (WARN_ON(wsm_seq != hw_priv->wsm_rx_seq)) {
		atbm_printk_err("rx wsm_seq error wsm_seq[%d] wsm_rx_seq[%d] skb->len[%d] wsm_id[%x],wsm_len[%d]\n",
			wsm_seq,hw_priv->wsm_rx_seq,skb->len,wsm_id,wsm_len);
		//frame_hexdump("rxdata",wsm,64);
		//BUG_ON(1);
		atbm_hif_status_set(1);
		atbm_bh_halt(hw_priv);
		goto __free;
	}
	hw_priv->wsm_rx_seq = (wsm_seq + 1) & 7;

	//frame_hexdump("dump  wsm rx ->",wsm,32);
	if (wsm_id & 0x0400) {
		int rc = wsm_release_tx_buffer(hw_priv, 1);
		if (WARN_ON(rc < 0))
			goto __free;
	}

	/* atbm_wsm_rx takes care on SKB livetime */
	if (WARN_ON(wsm_handle_rx(hw_priv, wsm_id, wsm,&skb))){
		atbm_printk_err("%s %d \n",__func__,__LINE__);
		goto __free;
	}

__free:
	if(skb != NULL){
		atbm_dev_kfree_skb(skb);
	}

	return 0;
}
int atbm_rx_single_channel_bh_cb(struct atbm_common *hw_priv,struct sk_buff *skb)
{
	struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;

	wsm = (struct wsm_hdr *)skb->data;

	wsm_len = __le16_to_cpu(wsm->len);

	wsm_id	= __le16_to_cpu(wsm->id) & 0xFFF;

	//BUG_ON(wsm_len > 4096);
	atbm_skb_trim(skb,0);
	atbm_skb_put(skb,wsm_len);

	if (unlikely(wsm_id == 0x0800)) {
		wsm_handle_exception(hw_priv, &skb->data[sizeof(*wsm)],wsm_len - sizeof(*wsm));
		if(wsm_recovery(hw_priv)== RECOVERY_ERR)
		{
			hw_priv->bh_error = 1;
			wake_up(&hw_priv->bh_wq);
		}
		goto __free;
	}
	//frame_hexdump("dump  wsm rx ->",wsm,32);
	if (wsm_id & 0x0400) {
		int rc = wsm_release_tx_buffer(hw_priv, 1);
		if (WARN_ON(rc < 0))
			goto __free;
	}

	/* atbm_wsm_rx takes care on SKB livetime */
	if (WARN_ON(wsm_handle_rx(hw_priv, wsm_id, wsm,&skb))){
		atbm_printk_err("%s %d \n",__func__,__LINE__);
		goto __free;
	}

__free:
	if(skb != NULL){
		atbm_dev_kfree_skb(skb);
	}

	return 0;
}
void atbm_tx_tasklet(unsigned long priv)
{
	struct atbm_common *hw_priv  = (struct atbm_common *)priv;
	int status =0;
	hw_priv->sbus_ops->lock(hw_priv->sbus_priv);

	do {

		if(atomic_read(&hw_priv->bh_term)|| hw_priv->bh_error || (hw_priv->bh_thread == NULL))
		{
			hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
			atbm_printk_err("atbm_tx_tasklet term(%d),err(%d)\n",atomic_read(&hw_priv->bh_term),hw_priv->bh_error);
			return;
		}

		/*atbm transmit packet to device*/
		status = hw_priv->sbus_ops->sbus_memcpy_toio(hw_priv->sbus_priv,0x1,NULL,TX_BUFFER_SIZE);
		
	}while(status > 0);
	hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);

}

static u8 atbm_rx_need_alloc_skb(int wsm_id)
{
	int id = (wsm_id  & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX));

	return (id==WSM_RECEIVE_INDICATION_ID)||(id == WSM_SMARTCONFIG_INDICATION_ID);
}
//#define ALLOC_SKB_NULL_TEST
static struct sk_buff *atbm_get_skb(unsigned int length)
{
	#ifdef USB_USE_TASTLET_TXRX
	#define __atbm_get_skb(_length) atbm_dev_alloc_skb(_length)
	#else
	#define __atbm_get_skb(_length) __atbm_dev_alloc_skb(_length,GFP_KERNEL);
	#endif
	#ifdef ALLOC_SKB_NULL_TEST
	static u32 skb_alloc_cnt = 0;

	skb_alloc_cnt++;
	if((skb_alloc_cnt%4096) == 0)
		return NULL;
	#endif
	return __atbm_get_skb(length);
}
static bool atbm_rx_serial(struct atbm_common *hw_priv,u8 wsm_seq,int wsm_id)
{
	bool serial_rx = true;
	
	if(wsm_id == 0x0800){
		goto ret;
	}
	//
	//add because usb not reset when rmmod driver, just drop error frame  
	//
	if((hw_priv->wsm_caps.firmwareReady==0)
		&&((wsm_seq != hw_priv->wsm_rx_seq)
		||((wsm_id & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) != WSM_STARTUP_IND_ID))){
		atbm_printk_err("rx wsm_seq err1 (rxseq=%d need_rxseq=%d)  wsm_id %x firmwareReady %d\n",wsm_seq,hw_priv->wsm_rx_seq,wsm_id,hw_priv->wsm_caps.firmwareReady);
		if((wsm_id & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) != WSM_STARTUP_IND_ID){
			serial_rx = false;
			goto ret;
		}
		atbm_printk_err("startup seq(%d)\n",wsm_seq);
	}
	else if (WARN_ON(wsm_seq != hw_priv->wsm_rx_seq)) {
		atbm_printk_err("rx wsm_seq error wsm_seq[%d] wsm_rx_seq[%d] wsm_id[%x]\n",
			wsm_seq,hw_priv->wsm_rx_seq,wsm_id);

		atbm_printk_err("change hw_priv->wsm_rx_seq form wsm_seq \n");
		hw_priv->wsm_rx_seq = wsm_seq;
		//frame_hexdump("rxdata",wsm,64);
		//BUG_ON(1);
		
		/*******yzh *******
			atbm_hif_status_set(1);
			atbm_bh_halt(hw_priv);
			serial_rx = false;
		
		goto ret;
		*/
	}
	hw_priv->wsm_rx_seq = (wsm_seq + 1) & 7;
ret:
	return serial_rx;
}
static void atbm_rx_multi_rx(struct atbm_common *hw_priv,struct sk_buff *skb,
	int (*rx_func)(struct atbm_common *hw_priv,struct sk_buff *skb),bool serial_check)
{
	
#define RX_ALLOC_BUFF_OFFLOAD (  (36+16)/*RX_DESC_OVERHEAD*/+4/*FCS_LEN*/ -16 /*WSM_HI_RX_IND*/)
	struct wsm_multi_rx *  multi_rx = (struct wsm_multi_rx *)skb->data;			
	int RxFrameNum = multi_rx->RxFrameNum;	
	struct sk_buff *atbm_skb_copy;
	struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;
	int data_len;
	u8 wsm_seq;
	
	data_len = __le32_to_cpu(multi_rx->MsgLen);
	
	data_len -= sizeof(struct wsm_multi_rx);
	wsm = (struct wsm_hdr *)(multi_rx+1);
	wsm_len = __le32_to_cpu(wsm->len);
	wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
	do {
				
		if(data_len < wsm_len){
			atbm_printk_err("skb->len %x,wsm_len %x\n",skb->len,wsm_len);
			break;
		}
		//BUG_ON((wsm_id  & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) !=  WSM_RECEIVE_INDICATION_ID);
		if((wsm_id  & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) !=  WSM_RECEIVE_INDICATION_ID){
			atbm_printk_err("%s %d ,ERROR !!! (wsm_id=%d  & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)) = %d != 0x0804\n",__func__,__LINE__,
				wsm_id,(wsm_id  & ~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX)));
			return;
		}
		
		wsm_seq = (__le32_to_cpu(wsm->id) >> 13) & 7;

		if((serial_check == true)&&
		   (WARN_ON(atbm_rx_serial(hw_priv,wsm_seq,wsm_id) == false))){
		    goto next_skb;
		}
		atbm_skb_copy = atbm_get_skb(wsm_len + 16);
		if (!atbm_skb_copy){
			atbm_printk_err("alloc--skb Error(%d)1,move to next skb\n",wsm_len + 16);
			WARN_ON(1);	
			goto next_skb;
		}
		
		/* In AP mode RXed SKB can be looped back as a broadcast.
		 * Here we reserve enough space for headers. */
		atbm_skb_reserve(atbm_skb_copy,  (8 - (((unsigned long)atbm_skb_copy->data)&7))/*ALIGN 8*/);
		
		memmove(atbm_skb_copy->data, wsm, wsm_len);
		atbm_skb_put(atbm_skb_copy,wsm_len);
		atbm_skb_copy->pkt_type = skb->pkt_type;
		rx_func(hw_priv,atbm_skb_copy);
next_skb:
		data_len -= ALIGN(wsm_len + RX_ALLOC_BUFF_OFFLOAD,4);
		RxFrameNum--;
		wsm = (struct wsm_hdr *)((u8 *)wsm +ALIGN(( wsm_len + RX_ALLOC_BUFF_OFFLOAD),4));
		wsm_len = __le32_to_cpu(wsm->len);
		wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
		
	}while((RxFrameNum>0) && (data_len > 32));
	//BUG_ON(RxFrameNum != 0);
	if(RxFrameNum != 0){
		atbm_printk_err("%s : RxFrameNum = %d £¬ data_len(%d)\n",__FUNCTION__,RxFrameNum,data_len);
	}
}
static bool atbm_process_ieee80211_frame(struct atbm_common *hw_priv,struct sk_buff *skb,
	int (*rx_func)(struct atbm_common *hw_priv,struct sk_buff *skb))
{
	struct sk_buff *atbm_skb_copy;
	struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;
	u8 wsm_seq;

	wsm = (struct wsm_hdr *)skb->data;
	wsm_len = __le32_to_cpu(wsm->len);
	wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
	
	if(wsm_id == WSM_MULTI_RECEIVE_INDICATION_ID){
		/*
		*rx muilt data package
		*/
		atbm_rx_multi_rx(hw_priv,skb,rx_func,true);
		goto processed;
		
	}else if(WSM_SINGLE_CHANNEL_MULTI_RECEIVE_INDICATION_ID == wsm_id){
		/*
		*rx single channel but mult data package
		*/
		struct wsm_multi_rx *singlech_multirx = (struct wsm_multi_rx *)skb->data;
		
		wsm_seq = (__le32_to_cpu(singlech_multirx->MsgId) >> 13) & 7;
		
		if (WARN_ON(atbm_rx_serial(hw_priv,wsm_seq,wsm_id) == false)) {
			atbm_printk_err("rx wsm_seq error wsm_seq[%d] wsm_rx_seq[%d] skb->len[%d] wsm_id[%x],wsm_len[%d]\n",
				wsm_seq,hw_priv->wsm_rx_seq,skb->len,wsm_id,wsm_len);
			
			atbm_hif_status_set(1);
			atbm_bh_halt(hw_priv);
			goto processed;
		}		
		atbm_rx_multi_rx(hw_priv,skb,rx_func,false);
		goto processed;
	} else if (atbm_rx_need_alloc_skb(wsm_id)){
		/*
		*rx single data package
		*/
		wsm_seq = (__le32_to_cpu(wsm->id) >> 13) & 7;
		if (WARN_ON(atbm_rx_serial(hw_priv,wsm_seq,wsm_id) == false)) {
			atbm_printk_err("rx wsm_seq error wsm_seq[%d] wsm_rx_seq[%d] skb->len[%d] wsm_id[%x],wsm_len[%d]\n",
				wsm_seq,hw_priv->wsm_rx_seq,skb->len,wsm_id,wsm_len);
			
			atbm_hif_status_set(1);
			atbm_bh_halt(hw_priv);
			goto processed;
		}		
		atbm_skb_copy = atbm_get_skb(wsm_len + 16);
		if (!atbm_skb_copy){
			atbm_printk_err("alloc--skb Error(%d)\n",wsm_len + 16);
			WARN_ON(1);
			goto processed;
		}
		memmove(atbm_skb_copy->data, wsm, wsm_len);
		atbm_skb_put(atbm_skb_copy,wsm_len);
		atbm_skb_copy->pkt_type = skb->pkt_type;
		rx_func(hw_priv,atbm_skb_copy);
		goto processed;		
	}
	/*
	*others frame is wsm cmd ,not process here
	*/
	return false;
processed:
	return true;
}

static bool atbm_process_wsm_cmd_frame(struct atbm_common *hw_priv,struct sk_buff *skb,
	 int (*rx_func)(struct atbm_common *hw_priv,struct sk_buff *skb))
{
	struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;
	struct sk_buff *atbm_skb_copy;
	
	wsm = (struct wsm_hdr *)skb->data;
	wsm_len = __le32_to_cpu(wsm->len);
	wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;

	WARN_ON(atbm_rx_need_alloc_skb(wsm_id));

	atbm_skb_copy = atbm_skb_get(skb);
	atbm_skb_put(atbm_skb_copy,wsm_len);
	atbm_skb_copy->pkt_type = skb->pkt_type;
	rx_func(hw_priv,atbm_skb_copy);
	atbm_skb_tx_debug(atbm_skb_copy);

	return true;
}

static bool atbm_process_raw_frame(struct atbm_common *hw_priv,struct sk_buff *skb)
{	
	if(atbm_process_ieee80211_frame(hw_priv,skb,atbm_rx_single_channel_bh_cb) == false){
		atbm_process_wsm_cmd_frame(hw_priv,skb,atbm_rx_bh_cb);
	}	
	return true;
}

bool atbm_rx_directly(struct atbm_common *hw_priv,struct sk_buff *skb,
	int (*rx_func)(struct atbm_common *hw_priv,struct sk_buff *skb))
{
	struct wsm_hdr *wsm;
	u8 wsm_seq;
	
	if(atbm_process_ieee80211_frame(hw_priv,skb,rx_func) == true){
		return true;
	}
	
	wsm = (struct wsm_hdr *)skb->data;
	wsm_seq = (__le32_to_cpu(wsm->id) >> 13) & 7;
	
	if (WARN_ON(atbm_rx_serial(hw_priv,wsm_seq,__le32_to_cpu(wsm->id) & 0xFFF) == false)) {			
		atbm_hif_status_set(1);
		atbm_bh_halt(hw_priv);
		return false;
	}
	skb->pkt_type = ATBM_RX_WSM_CMD_FRAME;
	return false;
}

void atbm_rx_tasklet(unsigned long priv)
{
	struct atbm_common *hw_priv  = (struct atbm_common *)priv;
	struct sk_buff *skb ;
	enum atbm_rx_frame_type frame_type;
	struct sk_buff_head local_list;
	unsigned long flags;
	
	__atbm_skb_queue_head_init(&local_list);

	spin_lock_irqsave(&hw_priv->rx_frame_queue.lock,flags);
	hw_priv->bh_running  = true;
restart:	
	bh_printk("%s: restart\n",__func__);
	atbm_skb_queue_splice_tail_init(&hw_priv->rx_frame_queue, &local_list);
	spin_unlock_irqrestore(&hw_priv->rx_frame_queue.lock,flags);

	
	while ((skb = __atbm_skb_dequeue(&local_list)) != NULL) {
		if(atomic_read(&hw_priv->bh_term)|| hw_priv->bh_error || (hw_priv->bh_thread == NULL)||
		   (atomic_read(&hw_priv->atbm_pluged)==0))
		{
			atbm_dev_kfree_skb(skb);
			continue;
		}
		frame_type = (enum atbm_rx_frame_type)skb->pkt_type;
		switch (frame_type) {
		case ATBM_RX_RAW_FRAME:
			bh_printk("%s:RX_RAW_FRAME\n",__func__);
			atbm_process_raw_frame(hw_priv,skb);
			atbm_skb_queue_tail(&hw_priv->rx_frame_free, skb);
			break;
		case ATBM_RX_WSM_CMD_FRAME:
			bh_printk("%s:WSM_CMD_FRAME\n",__func__);
			atbm_process_wsm_cmd_frame(hw_priv,skb,atbm_rx_single_channel_bh_cb);
			atbm_skb_queue_tail(&hw_priv->rx_frame_free, skb);
			break;
		case ATBM_RX_WSM_DATA_FRAME:
		case ATBM_RX_SLOW_MGMT_FRAME:
			bh_printk("%s:RX_SLOW_MGMT_FRAME\n",__func__);
			atbm_rx_single_channel_bh_cb(hw_priv,skb);
			break;
		case ATBM_RX_DERICTLY_DATA_FRAME:
			WARN_ON(1);
			atbm_dev_kfree_skb(skb);
			break;
		case ATBM_RX_WSM_FREE:
			bh_printk("ATBM_RX_WSM_FREE");
			skb->pkt_type = 0;
			atbm_skb_queue_tail(&hw_priv->rx_frame_free, skb);
			break;
		default:
			//BUG_ON(1);
			atbm_printk_err("%s %d , unknow frame_type = %x\n",__func__,__LINE__,frame_type);
			//break;
			atbm_dev_kfree_skb(skb);
			break;;
		}		
	}	
	while ((skb = atbm_skb_dequeue(&hw_priv->rx_frame_free)) != NULL) {
		/*atbm transmit packet to device*/			
		hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
		hw_priv->sbus_ops->sbus_read_async(hw_priv->sbus_priv,0x2,skb,RX_BUFFER_SIZE,NULL);
		hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);	
	}
	
	spin_lock_irqsave(&hw_priv->rx_frame_queue.lock,flags);
	if(!atbm_skb_queue_empty(&hw_priv->rx_frame_queue))
		goto restart;
	hw_priv->bh_running = false;
	spin_unlock_irqrestore(&hw_priv->rx_frame_queue.lock,flags);
	bh_printk("%s:end\n",__func__);
}
int atbm_reset_lmc_cpu(struct atbm_common *hw_priv)
{
	return -1;
}
extern  int wifi_module_exit;
static int usb_atbm_bh(void *arg)
{
	struct atbm_common *hw_priv = arg;
	struct atbm_vif *priv = NULL;
	int rx, tx=0, term, suspend;
	int pending_tx = 0;
	long status;
	bool powersave_enabled;
	int i;
	int prink_test =0;


#define __ALL_HW_BUFS_USED (hw_priv->hw_bufs_used)


	while (1) {

		powersave_enabled = 1;
		atbm_hw_vif_read_lock(&hw_priv->vif_list_lock);
		atbm_for_each_vif_safe(hw_priv, priv, i) {
			if (!priv)
				continue;
			powersave_enabled &= !!priv->powersave_enabled;
		}
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);

		if (!__ALL_HW_BUFS_USED
				&& powersave_enabled
				&& !hw_priv->device_can_sleep
				&& !atomic_read(&hw_priv->recent_scan)) {
			status = HZ*2;
			bh_printk( "[BH] No Device wakedown.\n");

		} else {
			status = HZ*2;
		}
		status = wait_event_interruptible_timeout(hw_priv->bh_wq, ({
				rx = atomic_xchg(&hw_priv->bh_rx, 0);
				tx = atomic_xchg(&hw_priv->bh_tx, 0);
				term = atomic_xchg(&hw_priv->bh_term, 0);
				suspend = pending_tx ?
					0 : atomic_read(&hw_priv->bh_suspend);
				(rx || tx || term || suspend || hw_priv->bh_error||atomic_read(&hw_priv->bh_halt));
			}), status);

		if (status < 0 || term || hw_priv->bh_error){
			atbm_printk_err("%s BH thread break %ld %d %d\n",__func__,status,term,hw_priv->bh_error);
			break;
		}

		if(atomic_read(&hw_priv->bh_halt)){
			atomic_set(&hw_priv->atbm_pluged,0);
			atbm_printk_err("%s:bh_halt\n",__func__);
			if(hw_priv->sbus_ops->lmac_restart(hw_priv->sbus_priv) != 0){
				atomic_xchg(&hw_priv->bh_halt,0);
				hw_priv->bh_error = 1;
				break;
			}
		}
		#if 1
		if(1){
			unsigned long timestamp = jiffies;
			long timeout;
			bool pending = false;
			int i;

			for (i = 0; i < 4; ++i)
				pending |= atbm_queue_get_xmit_timestamp(
						&hw_priv->tx_queue[i],
						&timestamp, -1,
						hw_priv->pending_frame_id);

			/* Check if frame transmission is timed out.
			 * Add an extra second with respect to possible
			 * interrupt loss. */
			timeout = timestamp +
					2 * HZ  -
					jiffies;

			/* And terminate BH tread if the frame is "stuck" */
			if (pending && timeout < 0) {
				extern void atbm_usb_urb_map_show(struct sbus_priv *self);
				atbm_printk_err("tx rx schedule timeout................?\n");
				atbm_usb_urb_map_show(hw_priv->sbus_priv);
			}
		}
		#endif
		if (!status && __ALL_HW_BUFS_USED)
		{
			unsigned long timestamp = jiffies;
			long timeout;
			bool pending = false;
			int i;

			//wiphy_warn(hw_priv->hw->wiphy, "Missed interrupt?\n");
			rx = 1;
			/* Get a timestamp of "oldest" frame */
			for (i = 0; i < 4; ++i)
				pending |= atbm_queue_get_xmit_timestamp(
						&hw_priv->tx_queue[i],
						&timestamp, -1,
						hw_priv->pending_frame_id);

			/* Check if frame transmission is timed out.
			 * Add an extra second with respect to possible
			 * interrupt loss. */
			timeout = timestamp +
					WSM_CMD_LAST_CHANCE_TIMEOUT +
					1 * HZ  -
					jiffies;
			

			/* And terminate BH tread if the frame is "stuck" */
			if (pending && timeout < 0) {
				//wiphy_warn(priv->hw->wiphy,
				//	"Timeout waiting for TX confirm.\n");
				prink_test++;
				//if(prink_test <10){
					atbm_printk_err("atbm_bh: Timeout waiting for TX confirm. hw_bufs_used %d,hw_noconfirm_tx(%d)\n",hw_priv->hw_bufs_used,hw_priv->hw_noconfirm_tx);
			//	}
				wsm_recovery(hw_priv);
				//break;
			}
		} else if (!status) {
			//printk("%s %d ++ !!\n",__func__,__LINE__);

			/* Do not suspend when datapath is not idle */
			if (  !hw_priv->tx_queue[0].num_queued
				   && !hw_priv->tx_queue[1].num_queued
				   && !hw_priv->tx_queue[2].num_queued
				   && !hw_priv->tx_queue[3].num_queued
				   &&(atomic_xchg(&hw_priv->bh_suspend_usb, 1)==0)){
					//usb suspend
					hw_priv->sbus_ops->power_mgmt(hw_priv->sbus_priv, true);
					//printk( "[BH] USB suspend. true.\n");
			}

			if (!hw_priv->device_can_sleep
					&& !atomic_read(&hw_priv->recent_scan)) {
				bh_printk( "[BH] Device wakedown. Timeout.\n");
			}
			continue;
		} else if (suspend) {
			bh_printk( "[BH] Device suspend.\n");
			atbm_printk_err("%s %d ++ !!\n",__func__,__LINE__);
			powersave_enabled = 1;
			atbm_hw_vif_read_lock(&hw_priv->vif_list_lock);
			atbm_for_each_vif_safe(hw_priv, priv, i) {
				if (!priv)
					continue;
				powersave_enabled &= !!priv->powersave_enabled;
			}
			atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
			if (powersave_enabled) {
				bh_printk( "[BH] No Device wakedown. Suspend.\n");
			}

			atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_SUSPENDED);
			wake_up(&hw_priv->bh_evt_wq);
			status = wait_event_interruptible(hw_priv->bh_wq,
					ATBM_APOLLO_BH_RESUME == atomic_read(
						&hw_priv->bh_suspend));
			if (status < 0) {
				atbm_printk_err("Failed to wait for resume: %ld.\n", status);
				break;
			}
			bh_printk( "[BH] Device resume.\n");
			atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_RESUMED);
			//wake_up(&hw_priv->bh_evt_wq);
			//atomic_add(1, &hw_priv->bh_rx);
			continue;
		}
	}

	atbm_printk_exit("%s %d \n",__func__,__LINE__);
	atbm_hif_status_set(1);
	
	//free rx buffer
	atbm_rx_bh_flush(hw_priv);
	atomic_set(&hw_priv->bh_term, term);
	if ((!term)) {
		int loop = 100;
		atbm_printk_exit("%s %d ++ !!\n",__func__,__LINE__);
		atbm_dbg(ATBM_APOLLO_DBG_ERROR, "[BH] Fatal error, exitting.\n");

		if(!wifi_module_exit){
			hw_priv->bh_error = 1;
			while(loop-->0){
				atbm_monitor_pc(hw_priv);
				msleep(1);
			}
			
			#ifdef ATBM_USB_RESET
			hw_priv->bh_error = 0;
			atomic_set(&hw_priv->usb_reset,0);
	//		hw_priv->sbus_ops->wtd_wakeup(hw_priv->sbus_priv);
			hw_priv->sbus_ops->usb_reset(hw_priv->sbus_priv);
			status = HZ;
			status = wait_event_interruptible_timeout(hw_priv->bh_wq, ({
					term = atomic_xchg(&hw_priv->bh_term, 0);
					(term || hw_priv->bh_error);
				}), status);
			if(term)
				goto process_term;
			if(!status)
				atbm_printk_exit("%s:reset time out\n",__func__);
			if(hw_priv->bh_error)
				atbm_printk_exit("%s:hw_priv->bh_error\n",__func__);
			#endif
		}
		else {
			term = 1;
		}
		atbm_hw_vif_read_lock(&hw_priv->vif_list_lock);
		atbm_for_each_vif_safe(hw_priv, priv, i) {
			if (!priv)
				continue;
//			ieee80211_driver_hang_notify(priv->vif, GFP_KERNEL);
		}
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
#ifdef CONFIG_PM
		atbm_pm_stay_awake(&hw_priv->pm_state, 3*HZ);
#endif
		/* TODO: schedule_work(recovery) */
#ifndef HAS_PUT_TASK_STRUCT
		/* The only reason of having this stupid code here is
		 * that __put_task_struct is not exported by kernel. */
		for (;;) {
			int status = wait_event_interruptible(hw_priv->bh_wq, ({
				term = atomic_xchg(&hw_priv->bh_term, 0);
				(term);
				}));

			if (status || term)
				break;
		}
#endif
	}
	atbm_printk_exit("atbm_wifi_BH_thread stop ++\n");
	if(term)
	{
		//clear pendding cmd
		if(!mutex_trylock(&hw_priv->wsm_cmd_mux))
		{
			spin_lock_bh(&hw_priv->wsm_cmd.lock);
			if(hw_priv->wsm_cmd.cmd != 0xFFFF)
			{
				hw_priv->wsm_cmd.ret = -1;
				hw_priv->wsm_cmd.done = 1;
				spin_unlock_bh(&hw_priv->wsm_cmd.lock);
				atbm_printk_exit("cancle current pendding cmd,release wsm_cmd.lock\n");
				wake_up(&hw_priv->wsm_cmd_wq);
				msleep(2);
				spin_lock_bh(&hw_priv->wsm_cmd.lock);
			}
			spin_unlock_bh(&hw_priv->wsm_cmd.lock);
		}
		else
		{
			mutex_unlock(&hw_priv->wsm_cmd_mux);
		}

		/*
		*cancle the current scanning process
		*/
		mutex_lock(&hw_priv->conf_mutex);
		if(atomic_read(&hw_priv->scan.in_progress))
		{
			struct atbm_vif *scan_priv = ABwifi_hwpriv_to_vifpriv(hw_priv,
						hw_priv->scan.if_id);
			bool scanto_running = false;
			atbm_priv_vif_list_read_unlock(&scan_priv->vif_lock);
			mutex_unlock(&hw_priv->conf_mutex);
			scanto_running = atbm_hw_cancel_delayed_work(&hw_priv->scan.timeout,true);
			mutex_lock(&hw_priv->conf_mutex);
			if(scanto_running>0)
			{
				u8 scan_fail=0;
				atbm_printk_exit("scan is running , stop it right now\n");
				/*
				*let scan complete and then unlock wsm_oper_lock.
				*/

				if(hw_priv->scan.status == 0)
				{
					/*
					*scan cmd has been send to lmac ,but we can not
					*receive scan completion.In that case ,we must unlock
					*wsm_oper_lock to make sure the scan process can run in later
					*/
					#ifndef OPER_CLOCK_USE_SEM
					mutex_unlock(&hw_priv->wsm_oper_lock);
					#else
					up(&hw_priv->wsm_oper_lock);
					#endif
				}
				else
				{
					/*
					*scan cmd send fail,so let hmac process the scan fail process in itself.
					*/
					scan_fail = 1;
				}
				hw_priv->scan.curr = hw_priv->scan.end;

				if(scan_fail == 0)
				{
					mutex_unlock(&hw_priv->conf_mutex);
					atbm_printk_exit("%s:atbm_scan_timeout\n",__func__);
					atbm_scan_timeout(&hw_priv->scan.timeout.work);
					mutex_lock(&hw_priv->conf_mutex);

				}
			}

		}
		mutex_unlock(&hw_priv->conf_mutex);
		{
			u8 i = 0;
			//cancel pendding work
			#define ATBM_CANCEL_PENDDING_WORK(work,work_func)			\
				do{														\
					if(atbm_hw_cancel_queue_work(work,true)==true)			\
					{													\
						work_func(work);								\
					}													\
				}														\
				while(0)
					
			if(atbm_hw_cancel_delayed_work(&hw_priv->scan.probe_work,true))
				atbm_probe_work(&hw_priv->scan.probe_work.work);
			if(atbm_hw_cancel_delayed_work(&hw_priv->scan.timeout,true))
				atbm_scan_timeout(&hw_priv->scan.timeout.work);
#ifdef CONFIG_ATBM_SUPPORT_P2P
			if(atbm_hw_cancel_delayed_work(&hw_priv->rem_chan_timeout,true))
				atbm_rem_chan_timeout(&hw_priv->rem_chan_timeout.work);
#endif
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->scan.work,atbm_scan_work);
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->event_handler,atbm_event_handler);
#ifdef CONFIG_ATBM_BA_STAT
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->ba_work,atbm_ba_work);		
			atbm_del_timer_sync(&hw_priv->ba_timer);
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M
#ifdef CONFIG_ATBM_40M_AUTO_CCA
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->get_cca_work,atbm_get_cca_work);
			atbm_del_timer_sync(&hw_priv->chantype_timer);
#endif
#endif
#ifdef ATBM_SUPPORT_SMARTCONFIG
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->scan.smartwork,atbm_smart_scan_work);
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->scan.smartsetChanwork,atbm_smart_setchan_work);
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->scan.smartstopwork,atbm_smart_stop_work);
			atbm_del_timer_sync(&hw_priv->smartconfig_expire_timer);
#endif
#ifdef CONFIG_ATBM_BA_STATUS
			atbm_del_timer_sync(&hw_priv->ba_timer);
#endif
#ifndef CONFIG_RATE_HW_CONTROL
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->tx_policy_upload_work,tx_policy_upload_work);
#endif
			atbm_for_each_vif(hw_priv, priv, i) {
				if(priv == NULL)
					continue;
				ATBM_CANCEL_PENDDING_WORK(&priv->wep_key_work,atbm_wep_key_work);
				ATBM_CANCEL_PENDDING_WORK(&priv->join_work,atbm_join_work);
				ATBM_CANCEL_PENDDING_WORK(&priv->unjoin_work,atbm_unjoin_work);
#ifdef CONFIG_ATBM_SUPPORT_P2P
				ATBM_CANCEL_PENDDING_WORK(&priv->offchannel_work,atbm_offchannel_work);
#endif
				ATBM_CANCEL_PENDDING_WORK(&priv->link_id_work,atbm_link_id_work);
#ifndef CONFIG_TX_NO_CONFIRM
				ATBM_CANCEL_PENDDING_WORK(&priv->tx_failure_work,atbm_tx_failure_work);
#endif
				ATBM_CANCEL_PENDDING_WORK(&priv->set_tim_work, atbm_set_tim_work);
				ATBM_CANCEL_PENDDING_WORK(&priv->multicast_start_work,atbm_multicast_start_work);
				ATBM_CANCEL_PENDDING_WORK(&priv->multicast_stop_work, atbm_multicast_stop_work);
#if 0
				ATBM_CANCEL_PENDDING_WORK(&priv->linkid_reset_work, atbm_link_id_reset);
#endif
#ifdef CONFIG_ATBM_MAC80211_NO_USE
				ATBM_CANCEL_PENDDING_WORK(&priv->update_filtering_work, atbm_update_filtering_work);
#endif
				ATBM_CANCEL_PENDDING_WORK(&priv->set_beacon_wakeup_period_work,
											atbm_set_beacon_wakeup_period_work);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
				ATBM_CANCEL_PENDDING_WORK(&priv->ht_info_update_work, atbm_ht_info_update_work);
#endif
#ifdef ATBM_SUPPORT_WIDTH_40M
#ifdef CONFIG_ATBM_40M_AUTO_CCA
				//ATBM_CANCEL_PENDDING_WORK(&priv->chantype_change_work, atbm_channel_type_change_work);
				if(atbm_hw_cancel_delayed_work(&priv->chantype_change_work,true))
					atbm_channel_type_change_work(&priv->chantype_change_work.work);
#endif
#endif
				
				atbm_hw_cancel_delayed_work(&priv->dhcp_retry_work,true);
#ifndef CONFIG_TX_NO_CONFIRM
				if(atbm_hw_cancel_delayed_work(&priv->bss_loss_work,true))
					atbm_bss_loss_work(&priv->bss_loss_work.work);
				if(atbm_hw_cancel_delayed_work(&priv->connection_loss_work,true))
					atbm_connection_loss_work(&priv->connection_loss_work.work);
#endif
#if 0
				if(atbm_hw_cancel_delayed_work(&priv->set_cts_work,true))
					atbm_set_cts_work(&priv->set_cts_work.work);
#endif
				if(atbm_hw_cancel_delayed_work(&priv->link_id_gc_work,true))
					atbm_link_id_gc_work(&priv->link_id_gc_work.work);
#ifdef CONFIG_ATBM_SUPPORT_P2P
				if(atbm_hw_cancel_delayed_work(&priv->pending_offchanneltx_work,true))
					atbm_pending_offchanneltx_work(&priv->pending_offchanneltx_work.work);
#endif
				if(atbm_hw_cancel_delayed_work(&priv->join_timeout,true))
					atbm_join_timeout(&priv->join_timeout.work);	
				atbm_del_timer_sync(&priv->mcast_timeout);
			}
		}
	}
	/*
	add this code just because 'linux kernel' need kthread not exit ,
	before kthread_stop func call,
	*/
	while(term){
		if(kthread_should_stop()){
			break;
		}
		schedule_timeout_uninterruptible(msecs_to_jiffies(100));
	}
	atbm_printk_exit("atbm_wifi_BH_thread stop --\n");
	return 0;
}



//################################################################################################
//################################################################################################
//################################################################################################
//################################################################################################
//################################################################################################
//################################################################################################

#if 0
#define MAX_TX_TEST_LEN 1600
#define MIN_TX_TEST_LEN 10
struct sk_buff *test_skb[1000];
int test_skb_tx_id=0;
int test_skb_tx_end_id=0;
int test_len =  MAX_TX_TEST_LEN;
int test_skb_rx_id =0;

#define TEST_MAX_CNT 1000000
extern int wsm_test_cmd(struct atbm_common *hw_priv,u8 *buffer,int size);
int atbm_usb_xmit_test(struct sbus_priv *self,unsigned int addr, const void *pdata, int len);


void atbm_device_tx_test_complete(void *priv,void *arg);

int atbm_device_tx_test(struct atbm_common *hw_priv)
{
	int ret = 0;
	struct wsm_hdr_tx *hdr;
	u32 rand =0;

	get_random_bytes(&rand, sizeof(u32));
	rand = rand&0x3ff;
	rand += rand&0x1ff;
	rand += rand&0xff;
	if(rand >MAX_TX_TEST_LEN)
		rand = MAX_TX_TEST_LEN;
	if(rand < MIN_TX_TEST_LEN)
		rand = MIN_TX_TEST_LEN;
	//test_len = rand;
	hdr=(struct wsm_hdr_tx *)test_skb[test_skb_tx_id&0x7]->data;
	hdr->len = test_len;
	//txrx test
	hdr->id = 0x1e;
	//throughput test
	//hdr->id = 0xf;
	
	hw_priv->save_buf=(u8 *)hdr;
	hw_priv->save_buf_len = test_len;
	hw_priv->save_buf_vif_selected = 0;
	//printk("hw_priv->save_buf %x\n",hw_priv->save_buf);
	//printk( "test_tx_id %d len %d\n",test_skb_tx_id,test_len);
	//hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
	ret = hw_priv->sbus_ops->sbus_write_async(hw_priv->sbus_priv,0x1,hdr,hdr->len,atbm_device_tx_test_complete);
	//hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
	if(ret){
		return ret;
	}
	test_skb_tx_id++;
	//test_skb_tx_id++;
	//ret = atbm_usb_xmit_test(hw_priv->sbus_priv,0x2,hdr,hdr->len);
	//test_skb_tx_id++;
	//ret = atbm_usb_xmit_test(hw_priv->sbus_priv,0x3,hdr,hdr->len);
	//test_skb_tx_id++;
	//ret = atbm_usb_xmit_test(hw_priv->sbus_priv,0x4,hdr,hdr->len);
	//test_skb_tx_id++;

	return ret;
}


void atbm_device_tx_test_complete(void *priv,void *arg)
{
	struct atbm_common *hw_priv =priv;

	//printk( "tx_test_complete %d %d \n",test_skb_tx_id,test_skb_tx_end_id);
	if(++test_skb_tx_end_id < TEST_MAX_CNT){
		atbm_device_tx_test(hw_priv);
	}
	else {
		atbm_printk_bh("atbm_device_tx_test_complete %d \n",test_skb_tx_end_id);
		atbm_printk_bh("atbm_device_tx_test_END\n");
	}

}
void atbm_device_rx_test_complete(void *priv,void *arg);
#ifdef USB_BUS

typedef struct usb_reg_bit_t{
  u32 addr;
  u32 endbit;
  u32 startbit;
  u32 data;
}usb_reg_bit;
const usb_reg_bit usb_reg_table[]=
{
 	{0x16200024,	3,	0	,	1},
	{0x1620002c,	7,	0	,	2},
	{0x16400014,	15,	0	,	3},
	{0x1640001c,	15,	0	,	4},
	{0x16600000,	6,	0	,	5},
	{0x16700000,	16,	0	,	6},
	//{0x16700018,	7,	0	,	7},
	{0x16800024,	10,	0	,	8},
	//{0x1680005c,	2,	0 , 9 },
	//{0x1680005c,	6,	4 , 10 },
	//{0x1680005c,	10,	8 ,	11},
	//{0x1680005c,	14,	12,	12},
	//{0x1680005c,	18,	16,	13},
	//{0x1680005c,	22,	20,	14},
	//{0x1680005c,	26,	24,	15},
	//{0x1680005c,	30,	28,	16},
	{0x16900014,	9,	0 , 17 },
	{0x16900024,	12,	0	,	18},
	{0x16a00010,	4,	0 , 19 },
	{0x16a00020,	30,	0	,	20},
	{0x16100040,	31,	0	,	21},
	{0x16100020,	17,	0	,	22},
	{0x0acc01e0,	15,	0	,	23},
	{0x0acc0218,	31,	0	,	24},
	{0x09000000,	31,	0	,	25},
	{0x09003fff,	31,	0	,	26},
	{0x09004000,	31,	0	,	27},
	{0x09004fff,	31,	0	,	28},
	{0x00000000,	31,	0	,	29},
	{0x00003fff,	31,	0	,	30},
	{0x00004000,	31,	0	,	31},
	{0x00007fff,	31,	0	,	32},
	{0x00800000,	31,	0	,	33},
	{0x008027fc,	31,	0	,	34},
	{0x0ab00000,  	17,0,  	 35},
	{0x0ab0011c,	14,0,		36},
	{0x09c00004,	31,0,		37},
	{0x09c000ac,	31,0,		38},
	{0x0ac00004,	6,0,		39},
 	{0x16101004,	11,8,		15},
 	{0x16101004,	3,0,		15},
	{0x1610100c,	21,0,		42},
	//{0x0ac0036c,	31,0,		40},
	{0xff,31,0,0xff},
};
int test_tt =0;
void atbm_test_usb_reg_bit(struct atbm_common *hw_priv,const usb_reg_bit * usb_reg_bit_table)
{
   u32  uiRegValue=0;
   u32  regmask=0;
   u32  startbit,endbit;
   u32 Value,RegAddr;
   u32 const *Ptr;
   int i=0;

	//test_tt = 1;
	atbm_printk_bh("%s\n",__func__);
    Ptr = (u32*)usb_reg_bit_table;
    while (*Ptr != 0xff)
    {
         RegAddr = *Ptr++;
         endbit =  *Ptr++;
         startbit = *Ptr++;
         Value  =  *Ptr++;
	 	 atbm_printk_bh("RegAddr=%x,bit%d~bit%d\n",RegAddr,endbit,startbit);
         hw_priv->sbus_ops->sbus_read_sync(hw_priv->sbus_priv,RegAddr,&uiRegValue,4);
         regmask = ~((1<<startbit) -1);
         regmask &= ((1<<endbit) -1)|(1<<endbit);
         uiRegValue &= ~regmask;
         uiRegValue |= (Value<<startbit)&regmask;
		 atbm_printk_bh("start write reg number=%d,value=0x%x,uiRegValue=%x\n",i,uiRegValue,(Value<<startbit));
		 hw_priv->sbus_ops->sbus_write_sync(hw_priv->sbus_priv,RegAddr,&uiRegValue,4);
		 mdelay(2);
		 hw_priv->sbus_ops->sbus_read_sync(hw_priv->sbus_priv,RegAddr,&uiRegValue,4);
		 atbm_printk_bh("start read reg number=%d,value=0x%x\n",i,uiRegValue);
		 i++;

    };
	}
#endif
int atbm_device_rx_test(struct atbm_common *hw_priv)
{
	int ret = 0;

	test_skb_rx_id++;
	//printk( "test_rx_id %d\n",test_skb_rx_id);

	hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
	hw_priv->sbus_ops->sbus_read_async(hw_priv->sbus_priv,0x2,NULL,RX_BUFFER_SIZE,atbm_device_rx_test_complete);
	hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);

	return ret;
}

void atbm_device_rx_test_complete(void *priv,void *arg)
{
	struct atbm_common *hw_priv =priv;
	struct wsm_hdr *wsm=arg;
	//printk( "rx_test_complete %d  \n",test_skb_rx_id);

	if(wsm->id == 0x2e){
		test_skb_rx_id += wsm->len/4 -1;
	}

	if(test_skb_rx_id < TEST_MAX_CNT){
		atbm_device_rx_test(hw_priv);
	}
	else {
		atbm_printk_bh("atbm_device_rx_test END %d  \n",test_skb_rx_id);
	}
}
void atbm_usb_receive_data_cancel(struct sbus_priv *self);
int atbm_usb_pm(struct sbus_priv *self, bool  suspend);

int atbm_device_txrx_test_init(struct atbm_common *hw_priv,int caseNum)
{
	if(caseNum ==1){
		memset(test_skb,0,sizeof(test_skb));
		test_skb_tx_id=0;
		test_skb_tx_end_id=0;
		test_skb_rx_id = 0;
		test_skb[0] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[1] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[2] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[3] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[4] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[5] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[6] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		test_skb[7] = atbm_dev_alloc_skb(MAX_TX_TEST_LEN+64);
		atbm_device_rx_test(hw_priv);
		atbm_device_rx_test(hw_priv);
		atbm_device_tx_test(hw_priv);
		atbm_device_tx_test(hw_priv);
		atbm_device_tx_test(hw_priv);
		atbm_device_tx_test(hw_priv);
		//atbm_usb_pm(hw_priv->sbus_priv,1);
	}
	else if(caseNum ==0){
		//atbm_test_usb_reg_bit(hw_priv,usb_reg_table);
		//atbm_usb_pm(hw_priv->sbus_priv,0);
	}
	return 0;
}

#endif

