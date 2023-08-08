/*
 * Device handling thread implementation for mac80211 altobeam APOLLO drivers
 *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 * Based on:
 * Atbm UMAC CW1200 driver, which is
 * Copyright (c) 2010, ST-Ericsson
 * Author: Ajitpal Singh <ajitpal.singh@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
//#undef CONFIG_ATBM_APOLLO_USE_GPIO_IRQ
#include <net/atbm_mac80211.h>
#include <linux/kthread.h>
#include <linux/prefetch.h>


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
#if defined(CONFIG_ATBM_APOLLO_BH_DEBUG)
#define bh_printk  atbm_printk_always
#else
#define bh_printk(...)
#endif
#define IS_SMALL_MSG(wsm_id) (wsm_id & 0x1000)
static int atbm_bh(void *arg);
extern void atbm_monitor_pc(struct atbm_common *hw_priv);

int atbm_bh_read_ctrl_reg(struct atbm_common *hw_priv,
					  u16 *ctrl_reg);
int atbm_bh_read_ctrl_reg_unlock(struct atbm_common *hw_priv,
					  u16 *ctrl_reg);

static struct sk_buff *atbm_get_skb(struct atbm_common *hw_priv, u32 len);
int atbm_rx_tasklet(struct atbm_common *hw_priv, int id,
		  struct wsm_hdr *wsm, struct sk_buff **skb_p);

typedef int (*reed_ctrl_handler_t)(struct atbm_common *hw_priv,u16 *ctrl_reg);
typedef int (*reed_data_handler_t)(struct atbm_common *hw_priv, void *buf, u32 buf_len);

/* TODO: Verify these numbers with WSM specification. */
#define DOWNLOAD_BLOCK_SIZE_WR	(0x4000 - 4)
/* an SPI message cannot be bigger than (2"12-1)*2 bytes
 * "*2" to cvt to bytes */
#define MAX_SZ_RD_WR_BUFFERS	(DOWNLOAD_BLOCK_SIZE_WR*2)
#define PIGGYBACK_CTRL_REG	(2)
#define EFFECTIVE_BUF_SIZE	(MAX_SZ_RD_WR_BUFFERS - PIGGYBACK_CTRL_REG)
#define ATBM_MAX_OVERFLOW_SIZE	(64)
typedef int (*atbm_wsm_handler)(struct atbm_common *hw_priv,
	u8 *data, size_t size);

#ifdef MCAST_FWDING
int wsm_release_buffer_to_fw(struct atbm_vif *priv, int count);
#endif
/*Os wake lock*/
#define ATBM_OS_WAKE_LOCK(WAKELOCK)			atbm_os_wake_lock(WAKELOCK)
#define ATBM_OS_WAKE_UNLOCK(WAKELOCK)		atbm_os_wake_unlock(WAKELOCK)
/*BH wake lock*/
#define ATBM_BH_WAKE_LOCK(WAKELOCK)         atbm_bh_wake_lock(WAKELOCK)
#define ATBM_BH_WAKE_UNLOCK(WAKELOCK)         atbm_bh_wake_unlock(WAKELOCK)
int atbm_os_check_wakelock(struct atbm_common *hw_priv)
{
	if (!hw_priv)
		return 0;
#ifdef CONFIG_HAS_WAKELOCK
	/* Indicate to the SD Host to avoid going to suspend if internal locks are up */
	if (wake_lock_active(&hw_priv->hw_wake) ||
		(wake_lock_active(&hw_priv->bh_wake)))
		return 1;
#endif
	return 0;
}

int atbm_os_wake_lock(struct atbm_common *hw_priv)
{
	unsigned long flags;
	int ret = 0;
	ret=atbm_os_check_wakelock(hw_priv);
	if(ret){
		return 0;
	}
	if (hw_priv) {
		spin_lock_irqsave(&hw_priv->wakelock_spinlock, flags);
		if (hw_priv->wakelock_hw_counter == 0) {
#ifdef CONFIG_HAS_WAKELOCK
			wake_lock(&hw_priv->hw_wake);
#elif defined(SDIO_BUS) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
			//pm_stay_awake(hw_priv);
#endif
		}
		hw_priv->wakelock_hw_counter++;
		ret = hw_priv->wakelock_hw_counter;
		spin_unlock_irqrestore(&hw_priv->wakelock_spinlock, flags);
	}
	return ret;
}

int atbm_os_wake_unlock(struct atbm_common *hw_priv)
{
	unsigned long flags;
	int ret = 0;

	if (hw_priv) {
		spin_lock_irqsave(&hw_priv->wakelock_spinlock, flags);
		if (hw_priv->wakelock_hw_counter > 0) {
			hw_priv->wakelock_hw_counter--;
			if (hw_priv->wakelock_hw_counter == 0) {
#ifdef CONFIG_HAS_WAKELOCK
				wake_unlock(&hw_priv->hw_wake);
#elif defined(SDIO_BUS) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
				//pm_relax(hw_priv);
#endif
			}
			ret = hw_priv->wakelock_hw_counter;
		}
		spin_unlock_irqrestore(&hw_priv->wakelock_spinlock, flags);
	}
	return ret;
}
int atbm_bh_wake_lock(struct atbm_common *hw_priv)
{
	unsigned long flags;
	int ret;
	ret=atbm_os_check_wakelock(hw_priv);
	if(ret){
		return 0;
	}
	if (hw_priv) {
		spin_lock_irqsave(&hw_priv->wakelock_spinlock, flags);
		if (hw_priv->wakelock_bh_counter == 0) {
#ifdef CONFIG_HAS_WAKELOCK
			wake_lock(&hw_priv->bh_wake);
#elif defined(SDIO_BUS) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
			//pm_stay_awake(hw_priv);
#endif		
			hw_priv->wakelock_bh_counter++;
		}
		spin_unlock_irqrestore(&hw_priv->wakelock_spinlock, flags);
	}
	return 0;
}

void atbm_bh_wake_unlock(struct atbm_common *hw_priv)
{
	unsigned long flags;
	if (hw_priv) {
		spin_lock_irqsave(&hw_priv->wakelock_spinlock, flags);
		if (hw_priv->wakelock_bh_counter > 0) {
			hw_priv->wakelock_bh_counter = 0;
#ifdef CONFIG_HAS_WAKELOCK
		wake_unlock(&hw_priv->bh_wake);
#elif defined(SDIO_BUS) && (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 36))
		//pm_relax(hw_priv);
#endif
		}
		spin_unlock_irqrestore(&hw_priv->wakelock_spinlock, flags);
	}

}
int atbm_register_bh(struct atbm_common *hw_priv)
{
	int err = 0;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 9, 0))
	struct sched_param param = { .sched_priority = 1 };
#endif
	atbm_printk_init("[BH] register.\n");
	BUG_ON(hw_priv->bh_thread);
	atomic_set(&hw_priv->bh_rx, 0);
	atomic_set(&hw_priv->bh_tx, 0);
	atomic_set(&hw_priv->bh_term, 0);
	atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_RESUMED);
	hw_priv->buf_id_tx = 0;
	hw_priv->buf_id_rx = 0;
	hw_priv->syncChanl_done=1;
	init_waitqueue_head(&hw_priv->bh_wq);
	init_waitqueue_head(&hw_priv->bh_evt_wq);
	hw_priv->bh_thread = kthread_create(&atbm_bh, hw_priv, ieee80211_alloc_name(hw_priv->hw,"atbm_bh"));
	if (IS_ERR(hw_priv->bh_thread)) {
		err = PTR_ERR(hw_priv->bh_thread);
		hw_priv->bh_thread = NULL;
	} else {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 9, 0))
        	sched_set_fifo_low(current);
#else
		WARN_ON(sched_setscheduler(hw_priv->bh_thread,
			SCHED_FIFO, &param));
#endif
#ifdef HAS_PUT_TASK_STRUCT
		get_task_struct(hw_priv->bh_thread);
#endif
		wake_up_process(hw_priv->bh_thread);
	}
	return err;
}

/*
static void atbm_hw_buff_reset(struct atbm_common *hw_priv)
{
	int i;
	hw_priv->wsm_tx_seq = 0;
	hw_priv->buf_id_tx = 0;
	hw_priv->wsm_rx_seq = 0;
	hw_priv->hw_bufs_used = 0;
	hw_priv->save_buf = NULL;
	hw_priv->save_buf_len = 0;
	hw_priv->save_buf_vif_selected = -1;
	hw_priv->buf_id_rx = 0;
	for (i = 0; i < ATBM_WIFI_MAX_VIFS; i++)
		hw_priv->hw_bufs_used_vif[i] = 0;
}
*/

void atbm_unregister_bh(struct atbm_common *hw_priv)
{
	struct task_struct *thread = hw_priv->bh_thread;
	if (WARN_ON(!thread))
		return;

	hw_priv->bh_thread = NULL;
	atbm_printk_exit( "[BH] unregister.\n");
	atomic_add(1, &hw_priv->bh_term);
	wake_up(&hw_priv->bh_wq);
	kthread_stop(thread);
#ifdef HAS_PUT_TASK_STRUCT
	put_task_struct(thread);
#endif
}
static int atbm_rx_directly(struct atbm_common *hw_priv,struct sk_buff *skb)
{
	u8 *data;
	struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;
	u8 wsm_seq;
	static int rx_resync = 1;
	int status = 0;;
	
	data = skb->data;
	wsm = (struct wsm_hdr *)data;
	
	wsm_len = __le32_to_cpu(wsm->len);
	wsm_id  = __le32_to_cpu(wsm->id) & 0xFFF;
	wsm_seq = (__le32_to_cpu(wsm->id) >> 13) & 7;
	
	atbm_skb_trim(skb, wsm_len);
	if (unlikely(wsm_id == 0x0800)) {
		wsm_handle_exception(hw_priv,
			 &data[sizeof(*wsm)],
			wsm_len - sizeof(*wsm));
			status = -1;
			atbm_hif_status_set(1);
			atbm_bh_halt(hw_priv);
			goto exit;
	} else if (unlikely(!rx_resync)) {
		if (WARN_ON(wsm_seq != hw_priv->wsm_rx_seq)) {
			status = -2;
			atbm_hif_status_set(1);
			atbm_bh_halt(hw_priv);
			goto exit;
		}
	}
	hw_priv->wsm_rx_seq = (wsm_seq + 1) & 7;
	rx_resync = 0;

	if (wsm_id & 0x0400) {
		int rc = wsm_release_tx_buffer(hw_priv, 1);
		if (WARN_ON(rc < 0)){
			status = -3;
			goto exit;
		}else if (rc > 0){
			atbm_bh_wakeup(hw_priv);
		}
	}
	/* atbm_wsm_rx takes care on SKB livetime */
	if (WARN_ON(atbm_rx_tasklet(hw_priv, wsm_id, wsm,
				  &skb))){
		status = -4;
		goto exit;
	}
exit:
	if (skb) {
		atbm_dev_kfree_skb(skb);
	}
	return status;
}
#ifdef CONFIG_SDIO_IRQ_THREAD_PROCESS_DATA
static int atbm_sdio_submit_skb(struct atbm_common *hw_priv,struct sk_buff *skb)
{
	bool bh_running = false;
	__u32 queue_len = 0;
	
	spin_lock_bh(&hw_priv->rx_frame_queue.lock);
	__atbm_skb_queue_tail(&hw_priv->rx_frame_queue, skb);
	bh_running = hw_priv->bh_running;
	queue_len = atbm_skb_queue_len(&hw_priv->rx_frame_queue);
	spin_unlock_bh(&hw_priv->rx_frame_queue.lock);

	if (bh_running == true)
		;
	else if (hw_priv->sbus_ops->sbus_rev_schedule)
		hw_priv->sbus_ops->sbus_rev_schedule(hw_priv->sbus_priv);
	else if (atomic_add_return(1, &hw_priv->bh_rx) == 1){
		wake_up(&hw_priv->bh_wq);
	}

	return (queue_len >= ATBM_MAX_OVERFLOW_SIZE) && (bh_running == false);
}
static int atbm_sdio_submit_skb_in_thread(struct atbm_common *hw_priv,struct sk_buff *skb)
{
	__u32 queue_len = 0;

	spin_lock_bh(&hw_priv->rx_frame_queue.lock);
	__atbm_skb_queue_tail(&hw_priv->rx_frame_queue, skb);
	queue_len = atbm_skb_queue_len(&hw_priv->rx_frame_queue);
	spin_unlock_bh(&hw_priv->rx_frame_queue.lock);

	return queue_len >= ATBM_MAX_OVERFLOW_SIZE;
}
#endif

static int atbm_sdio_process_read_data(struct atbm_common *hw_priv,reed_ctrl_handler_t read_ctrl_func,
	reed_data_handler_t read_data_func ,int (*rx_handle)(struct atbm_common *hw_priv,struct sk_buff *skb),enum atbm_rx_frame_type frame_type)
{
	#define LMAC_MAX_RX_BUFF	(24)
	u32 read_len_lsb = 0;
	u32 read_len_msb = 0;
	u32 read_len;
	struct sk_buff *skb_rx = NULL;
	u16 ctrl_reg = 0;
	u32 alloc_len;
	u8 *data;
	u8 rx_continue_cnt = 0;

	BUG_ON(read_ctrl_func == NULL);
	BUG_ON(read_data_func == NULL);
	BUG_ON(rx_handle == NULL);
rx_check:
	if (WARN_ON(read_ctrl_func(
			hw_priv, &ctrl_reg))){
			goto err;
	}
rx_continue:
	read_len_lsb = (ctrl_reg & ATBM_HIFREG_CONT_NEXT_LEN_LSB_MASK)*2;
	read_len_msb = (ctrl_reg & ATBM_HIFREG_CONT_NEXT_LEN_MSB_MASK)*2;
	read_len=((read_len_msb>>2)+read_len_lsb);
	if (!read_len) {
		return rx_continue_cnt;
	}

	if (WARN_ON((read_len < sizeof(struct wsm_hdr)) ||
			(read_len > EFFECTIVE_BUF_SIZE))) {
			atbm_printk_err("Invalid read len: %d,read_cnt(%d)\n",
				read_len,rx_continue_cnt);
		
		atbm_bh_halt(hw_priv);
		goto err;
	}
	/* Add SIZE of PIGGYBACK reg (CONTROL Reg)
	 * to the NEXT Message length + 2 Bytes for SKB */
	read_len = read_len + 2;
	alloc_len = read_len;
	if (alloc_len % SDIO_BLOCK_SIZE ) {
		alloc_len -= (alloc_len % SDIO_BLOCK_SIZE );
		alloc_len += SDIO_BLOCK_SIZE;
	}
	/* Check if not exceeding CW1200 capabilities */
	if (WARN_ON_ONCE(alloc_len > EFFECTIVE_BUF_SIZE)) {
		atbm_printk_err("Read aligned len: %d\n",
			alloc_len);
	}
	skb_rx = atbm_get_skb(hw_priv, alloc_len);
	if (WARN_ON(!skb_rx)){
		goto err;
	}

	atbm_skb_trim(skb_rx, 0);
	atbm_skb_put(skb_rx, read_len);
	data = skb_rx->data;
	
	if (WARN_ON(!data)){
		goto err;
	}
	
	if (WARN_ON(read_data_func(hw_priv, data, alloc_len))){
		atbm_bh_halt(hw_priv);
		goto err;
	}
	
	/* Piggyback */
	ctrl_reg = __le16_to_cpu(
		((__le16 *)data)[alloc_len / 2 - 1]);
	
	if(atbm_bh_is_term(hw_priv) || atomic_read(&hw_priv->bh_term)){
		goto err;
	}
	
	hw_priv->irq_timestamp = jiffies;

	skb_rx->pkt_type = frame_type;
	rx_continue_cnt++;

	if(rx_handle(hw_priv,skb_rx) != 0){
		return rx_continue_cnt;
	}
	
	read_len_lsb = (ctrl_reg & ATBM_HIFREG_CONT_NEXT_LEN_LSB_MASK)*2;
	read_len_msb = (ctrl_reg & ATBM_HIFREG_CONT_NEXT_LEN_MSB_MASK)*2;
	read_len=((read_len_msb>>2)+read_len_lsb);
	
	if(read_len)
		goto rx_continue;
	goto rx_check;
	
	return rx_continue_cnt;
err:
	if(skb_rx)
		atbm_dev_kfree_skb(skb_rx);
	return -1;
}
#define MAX_POOL_BUFF_NUM	4
static bool atbm_sdio_wait_enough_space(struct atbm_common	*hw_priv,u32 n_needs)
{
#define MAX_LOOP_POLL_CNT  (2*3000)
	u32 hw_xmited = 0;
	bool enough = false;
	int ret = 0;
	int loop = 0;
	u32 print = 0;

	spin_lock_bh(&hw_priv->tx_com_lock);
	enough = hw_priv->hw_bufs_free >= n_needs ? true : false;
	spin_unlock_bh(&hw_priv->tx_com_lock);
	
	while(enough == false){
		
		if(atbm_bh_is_term(hw_priv)){
			atbm_printk_err("%s:bh term\n",__func__);
			return false;
		}
#ifdef CONFIG_TX_NO_CONFIRM
#ifndef CONFIG_ATBM_SDIO_TX_HOLD
		hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
#endif
		ret = atbm_direct_read_unlock(hw_priv,hw_priv->wsm_caps.NumOfHwXmitedAddr,&hw_xmited);
#ifndef CONFIG_ATBM_SDIO_TX_HOLD
		hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
#endif

		if(ret){
			enough = false;
			break;
		}

		spin_lock_bh(&hw_priv->tx_com_lock);
		if((int)hw_priv->n_xmits < (int)hw_xmited ||
		   (int)(hw_priv->n_xmits - hw_xmited) > hw_priv->wsm_caps.numInpChBufs ||
		   (int)(hw_priv->n_xmits - hw_xmited)<0){
		   	enough = false;
		}else {
			hw_priv->hw_xmits = hw_xmited;
			hw_priv->hw_bufs_free =  (hw_priv->wsm_caps.numInpChBufs) - 
									 (hw_priv->n_xmits-hw_xmited);
			hw_priv->hw_bufs_free_init = hw_priv->hw_bufs_free;
			enough = hw_priv->hw_bufs_free >= n_needs ? true : false;
		}
		spin_unlock_bh(&hw_priv->tx_com_lock);

		if(enough == false){
			loop ++;
			if(loop % MAX_POOL_BUFF_NUM)
				continue;
			if(loop>=MAX_LOOP_POLL_CNT)
				break;
			if((loop >= 3)&&(print == 0)){			
				atbm_printk_debug("%s:n_xmits(%d),hw_xmited(%d),need(%d)\n",__func__,
					hw_priv->n_xmits,hw_xmited,n_needs);
				print = 1;
			}
#ifdef CONFIG_ATBM_SDIO_TX_HOLD
			hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
#endif
			schedule_timeout_interruptible(msecs_to_jiffies(2));
#ifdef CONFIG_ATBM_SDIO_TX_HOLD
			hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
#endif
		}
#else
		spin_lock_bh(&hw_priv->tx_com_lock);
		hw_priv->hw_bufs_free = hw_priv->wsm_caps.numInpChBufs - hw_priv->hw_bufs_used;
		hw_priv->hw_bufs_free_init = hw_priv->hw_bufs_free;
		enough = hw_priv->hw_bufs_free >= n_needs ? true : false;
		spin_unlock_bh(&hw_priv->tx_com_lock);
		if(enough == false){
			return false;
		}
#endif
	}

	return enough;
}

static bool atbm_sdio_have_enough_space(struct atbm_common	*hw_priv,u32 n_needs)
{
	u32 hw_xmited = 0;
	bool enough = false;
	int ret = 0;
	int n_pools = 0;
	
	spin_lock_bh(&hw_priv->tx_com_lock);
	enough = hw_priv->hw_bufs_free >= n_needs ? true : false;
	spin_unlock_bh(&hw_priv->tx_com_lock);

	if(enough == false){
		
		if(atbm_bh_is_term(hw_priv)){
			atbm_printk_err("%s:bh term\n",__func__);
			spin_lock_bh(&hw_priv->tx_com_lock);
			hw_priv->hw_bufs_free = n_needs;
			spin_unlock_bh(&hw_priv->tx_com_lock);
			return true;
		}
		
pool_buffs:
		n_pools ++;		
#ifdef CONFIG_TX_NO_CONFIRM
#ifndef CONFIG_ATBM_SDIO_TX_HOLD	
		hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
#endif
		ret = atbm_direct_read_unlock(hw_priv,hw_priv->wsm_caps.NumOfHwXmitedAddr,&hw_xmited);
#ifndef CONFIG_ATBM_SDIO_TX_HOLD
		hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
#endif
		if(ret){
			return false;			
		}
		
		spin_lock_bh(&hw_priv->tx_com_lock);
		if((int)hw_priv->n_xmits < (int)hw_xmited ||
		   (int)(hw_priv->n_xmits - hw_xmited) > hw_priv->wsm_caps.numInpChBufs ||
		   (int)(hw_priv->n_xmits - hw_xmited)<0){
		   	enough = false;
		}else {
			hw_priv->hw_xmits = hw_xmited;
			hw_priv->hw_bufs_free =  (hw_priv->wsm_caps.numInpChBufs) - 
									 (hw_priv->n_xmits-hw_xmited);
			hw_priv->hw_bufs_free_init = hw_priv->hw_bufs_free;
			enough = hw_priv->hw_bufs_free >= n_needs ? true : false;
		}
		spin_unlock_bh(&hw_priv->tx_com_lock);
		
		if((enough == false) && (n_pools%MAX_POOL_BUFF_NUM)){
			goto pool_buffs;
		}
#else
		spin_lock_bh(&hw_priv->tx_com_lock);
		hw_priv->hw_bufs_free = hw_priv->wsm_caps.numInpChBufs - hw_priv->hw_bufs_used;
		enough = hw_priv->hw_bufs_free >= n_needs ? true : false;
		hw_priv->hw_bufs_free_init = hw_priv->hw_bufs_free;
		spin_unlock_bh(&hw_priv->tx_com_lock);
#endif
	}

	return enough;
}
static void atbm_sdio_release_err_data(struct atbm_common	*hw_priv,struct wsm_tx *wsm)
{
	struct atbm_queue *queue;
	u8 queue_id;
	struct sk_buff *skb;
	const struct atbm_txpriv *txpriv;
	
	BUG_ON(wsm == NULL);
	queue_id = atbm_queue_get_queue_id(wsm->packetID);

	BUG_ON(queue_id >= 4);
	queue = &hw_priv->tx_queue[queue_id];
	BUG_ON(queue == NULL);
	
	wsm_release_tx_buffer(hw_priv, 1);
	if(!WARN_ON(atbm_queue_get_skb(queue, wsm->packetID, &skb, &txpriv))) {

		struct ieee80211_tx_info *tx = IEEE80211_SKB_CB(skb);
		//int tx_count = 0;
		int i;
		wsm_release_vif_tx_buffer(hw_priv,txpriv->if_id,1);
		tx->flags |= IEEE80211_TX_STAT_ACK;
		tx->status.rates[0].count = 1;
		for (i = 1; i < IEEE80211_TX_MAX_RATES; ++i) {
			tx->status.rates[i].count = 0;
			tx->status.rates[i].idx = -1;
		}
#ifdef CONFIG_ATBM_APOLLO_TESTMODE
		atbm_queue_remove(hw_priv, queue, wsm->packetID);
#else
		atbm_queue_remove(queue, wsm->packetID);
#endif
	}else {
		wsm_release_vif_tx_buffer(hw_priv,atbm_queue_get_if_id(wsm->packetID),1);
	}
}
static int atbm_sdio_free_tx_wsm(struct atbm_common	*hw_priv,struct wsm_tx *wsm)
{
	if((wsm) && (!(wsm->htTxParameters&__cpu_to_le32(WSM_NEED_TX_CONFIRM)))){
		
		struct atbm_queue *queue;
		u8 queue_id;
		struct sk_buff *skb;
		const struct atbm_txpriv *txpriv;

		queue_id = atbm_queue_get_queue_id(wsm->packetID);

		BUG_ON(queue_id >= 4);

		queue = &hw_priv->tx_queue[queue_id];
		BUG_ON(queue == NULL);

		if(!WARN_ON(atbm_queue_get_skb(queue, wsm->packetID, &skb, &txpriv))) {

			struct ieee80211_tx_info *tx = IEEE80211_SKB_CB(skb);
			//int tx_count = 0;
			int i;

			wsm_release_vif_tx_buffer(hw_priv,txpriv->if_id,1);
			wsm_release_tx_buffer(hw_priv, 1);
			
			tx->flags |= IEEE80211_TX_STAT_ACK;
			tx->status.rates[0].count = 1;
			for (i = 1; i < IEEE80211_TX_MAX_RATES; ++i) {
				tx->status.rates[i].count = 0;
				tx->status.rates[i].idx = -1;
			}

#ifdef CONFIG_ATBM_APOLLO_TESTMODE
			atbm_queue_remove(hw_priv, queue, wsm->packetID);
#else
			atbm_queue_remove(queue, wsm->packetID);
#endif
		}else {
			wsm_release_vif_tx_buffer(hw_priv,atbm_queue_get_if_id(wsm->packetID),1);
			wsm_release_tx_buffer(hw_priv, 1);
		}
		return 1;
	}
	return 0;
}
static void atbm_sdio_force_free_wsm(struct atbm_common	*hw_priv,struct wsm_tx *wsm)
{
	int wsm_id;

	if(wsm == NULL)
		return;
	
	wsm_id = __le16_to_cpu(wsm->hdr.id) & 0x3F;

	switch(wsm_id){
	case WSM_FIRMWARE_CHECK_ID:
		wsm_release_tx_buffer(hw_priv, 1);
		break;
	case WSM_TRANSMIT_REQ_MSG_ID:
		atbm_sdio_release_err_data(hw_priv,wsm);
		break;
	default:
		wsm_release_tx_buffer(hw_priv, 1);
		spin_lock_bh(&hw_priv->wsm_cmd.lock);
		if(hw_priv->wsm_cmd.cmd != 0XFFFF){
			hw_priv->wsm_cmd.ret = -1;
			hw_priv->wsm_cmd.done = 1;
			hw_priv->wsm_cmd.cmd = 0xFFFF;
			hw_priv->wsm_cmd.ptr = NULL;
			hw_priv->wsm_cmd.arg = NULL;
			wake_up(&hw_priv->wsm_cmd_wq);		
		}	
		spin_unlock_bh(&hw_priv->wsm_cmd.lock);
		break;
	}
}
void atbm_sdio_tx_bh(struct atbm_common *hw_priv)
{
#ifdef  ATBM_WSM_SDIO_TX_MULT
#define WSM_SDIO_TX_MULT_BLOCK_SIZE	(6*SDIO_BLOCK_SIZE)
#else
#define WSM_SDIO_TX_MULT_BLOCK_SIZE	(SDIO_BLOCK_SIZE)
#endif
#define ATBM_SDIO_FREE_BUFF_ERR(condition,free,prev_free,xmiteds,hw_xmiteds)	\
	do{																			\
		if(condition)	{																\
			atbm_printk_err("%s[%d]:free(%x),prev_free(%x),xmiteds(%x),hw_xmiteds(%x)\n",__func__,__LINE__,free,prev_free,xmiteds,hw_xmiteds);	\
			BUG_ON(1);			\
		}\
	}while(0)
#ifdef CONFIG_TX_NO_CONFIRM
	static u8 loop = 1;
#else
	static u8 loop = 0;
#endif
	int tx_burst;
	struct wsm_hdr_tx *wsm_tx;
	int vif_selected;
	u32 tx_len=0;
	u32 putLen=0;
	u8 *data;
	u8 *need_confirm = NULL;
	int ret=0;
	int txMutiFrameCount=0;
#if (PROJ_TYPE>=ARES_A)
	u32 wsm_flag_u32 = 0;
	u16 wsm_len_u16[2];
	u16 wsm_len_sum;
#endif	//	(PROJ_TYPE==ARES_A)	
	bool enough = false;

	prefetchw(hw_priv->xmit_buff);
	
xmit_continue:

	txMutiFrameCount = 0;
	putLen = 0;
	enough = false;
	need_confirm = NULL;
	do {
		
		enough = atbm_sdio_have_enough_space(hw_priv,1);
		
		if(enough == false){
			if(txMutiFrameCount > 0)
				break;
			else
				goto xmit_wait;
		}
		
		ret = wsm_get_tx(hw_priv, &data, &tx_len, &tx_burst,&vif_selected);
		
		if (ret <= 0) {
			if(txMutiFrameCount > 0)
				break;
			else
				goto xmit_finished;
		}
		
		txMutiFrameCount++;
		wsm_tx = (struct wsm_hdr_tx *)data;
		BUG_ON(tx_len < sizeof(*wsm_tx));
		BUG_ON(__le32_to_cpu(wsm_tx->len) != tx_len);
		
#if (PROJ_TYPE>=ARES_A)
		wsm_flag_u32 = (tx_len) & 0xffff;
		wsm_len_u16[0] = wsm_flag_u32 & 0xff;
		wsm_len_u16[1] = (wsm_flag_u32 >> 8)& 0xff;
		wsm_len_sum = wsm_len_u16[0] + wsm_len_u16[1];
		if (wsm_len_sum & BIT(8)){
			wsm_flag_u32 |= ((wsm_len_sum + 1) & 0xff) << 24;
		}else {
			wsm_flag_u32 |= (wsm_len_sum & 0xff) << 24;
		}
		wsm_tx->flag=__cpu_to_le32(wsm_flag_u32);
#endif //(PROJ_TYPE==ARES_A)

		if (tx_len <= 8)
			tx_len = 16;

		if (tx_len % (WSM_SDIO_TX_MULT_BLOCK_SIZE) ) {
			tx_len -= (tx_len % (WSM_SDIO_TX_MULT_BLOCK_SIZE) );
			tx_len += WSM_SDIO_TX_MULT_BLOCK_SIZE;
		}

		/* Check if not exceeding atbm
		capabilities */
		if (WARN_ON_ONCE(tx_len > EFFECTIVE_BUF_SIZE)) {
			atbm_printk_err("Write aligned len:"
			" %d\n", tx_len);
		}
		
#if (PROJ_TYPE<ARES_A)					
		wsm_tx->flag=(((tx_len/SDIO_BLOCK_SIZE)&0xff)-1);
#endif //(PROJ_TYPE==ARES_A)
		wsm_tx->id &= __cpu_to_le32(~WSM_TX_SEQ(WSM_TX_SEQ_MAX));
		wsm_tx->id |= cpu_to_le32(WSM_TX_SEQ(hw_priv->wsm_tx_seq));
#ifdef  ATBM_WSM_SDIO_TX_MULT
		wsm_tx->tx_len = tx_len;
		wsm_tx->tx_id = wsm_tx->id;
#endif
		wsm_alloc_tx_buffer(hw_priv);

		spin_lock_bh(&hw_priv->tx_com_lock);
		ATBM_SDIO_FREE_BUFF_ERR(hw_priv->hw_bufs_free <= 0,hw_priv->hw_bufs_free,hw_priv->hw_bufs_free_init,hw_priv->n_xmits,hw_priv->hw_xmits);
		hw_priv->n_xmits ++;
		hw_priv->hw_bufs_free --;		
		ATBM_SDIO_FREE_BUFF_ERR(hw_priv->hw_bufs_free < 0,hw_priv->hw_bufs_free,hw_priv->hw_bufs_free_init,hw_priv->n_xmits,hw_priv->hw_xmits);

		if (vif_selected != -1) {
			hw_priv->hw_bufs_used_vif[vif_selected]++;
		}

		spin_unlock_bh(&hw_priv->tx_com_lock);
		
		atbm_xmit_linearize(hw_priv,(struct wsm_tx *)data,&hw_priv->xmit_buff[putLen],wsm_tx->len);
		
		putLen += tx_len;
		hw_priv->wsm_tx_seq = (hw_priv->wsm_tx_seq + 1) & WSM_TX_SEQ_MAX;

		if(wsm_txed(hw_priv, data)){
			need_confirm = data;
			atbm_printk_debug("%s:cmd free(%d),used(%d)\n",__func__,hw_priv->hw_bufs_free,hw_priv->hw_bufs_used);
			break;
		}else {
			hw_priv->wsm_txframe_num++;
#ifdef CONFIG_TX_NO_CONFIRM
			if(atbm_sdio_free_tx_wsm(hw_priv,(struct wsm_tx *)data) == 0){
				need_confirm = data;
				atbm_printk_debug("%s:confirm free(%d),used(%d)\n",__func__,hw_priv->hw_bufs_free,hw_priv->hw_bufs_used);
				break;
			}
#else
			need_confirm = data;
#endif
		}
		
		if (putLen+hw_priv->wsm_caps.sizeInpChBuf>SDIO_TX_MAXLEN){
			break;
		}
	}while(loop);
	BUG_ON(putLen == 0);
	hw_priv->buf_id_offset = txMutiFrameCount;
	atomic_add(1, &hw_priv->bh_tx);

	if(atbm_bh_is_term(hw_priv)){
		atbm_printk_err("%s:bh term\n",__func__);
		atbm_sdio_force_free_wsm(hw_priv,(struct wsm_tx *)need_confirm);
		goto xmit_continue;
	}
#ifdef CONFIG_ATBM_SDIO_TX_HOLD	
	if (WARN_ON(atbm_data_write_unlock(hw_priv,hw_priv->xmit_buff, putLen))) {		
		atbm_printk_err("%s: xmit data err\n",__func__);
		goto xmit_err;
	}
#else
	if (WARN_ON(atbm_data_write(hw_priv,hw_priv->xmit_buff, putLen))) {		
		atbm_printk_err("%s: xmit data err\n",__func__);
		goto xmit_err;
	}
#endif
xmit_wait:	
	if((enough == false)&&(atbm_sdio_wait_enough_space(hw_priv,1) == false)){
#ifdef CONFIG_TX_NO_CONFIRM
		atbm_printk_err("%s: wait space timeout\n",__func__);
		goto xmit_err;
#else
		goto xmit_finished;
#endif
	}
	
	goto xmit_continue;
	
xmit_finished:	
	return;
xmit_err:
	atbm_sdio_force_free_wsm(hw_priv,(struct wsm_tx *)need_confirm);
	atbm_bh_halt(hw_priv);
	goto xmit_continue;
}

void atbm_sdio_rx_bh(struct atbm_common *hw_priv)
{
	static bool hard_irq = true;
	
	if(hw_priv->hard_irq == false){
#ifdef CONFIG_SDIO_IRQ_THREAD_PROCESS_DATA	
		/*
		*irq bh has read the lmac packages
		*/
		struct sk_buff *skb;
		struct sk_buff_head local_list;		
		u16 ctrl_reg = 0;
		
		hard_irq = false;
		__atbm_skb_queue_head_init(&local_list);
		spin_lock_bh(&hw_priv->rx_frame_queue.lock);
		hw_priv->bh_running = true;
restart:
		atbm_skb_queue_splice_tail_init(&hw_priv->rx_frame_queue, &local_list);
		spin_unlock_bh(&hw_priv->rx_frame_queue.lock);

		while ((skb = __atbm_skb_dequeue(&local_list)) != NULL){
			if(atomic_read(&hw_priv->bh_term)|| hw_priv->bh_error || (hw_priv->bh_thread == NULL)||
		   		(atomic_read(&hw_priv->atbm_pluged)==0)){
				atbm_dev_kfree_skb(skb);
				continue;
			}
			atbm_rx_directly(hw_priv,skb);
		}


		ctrl_reg = 0;
		/*
		*the operation of reading reg can clear irq
		*/
		hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
		atbm_bh_read_ctrl_reg_unlock(hw_priv, &ctrl_reg);
		if(ctrl_reg & ATBM_HIFREG_CONT_NEXT_LEN_MASK){
			atbm_sdio_process_read_data(hw_priv,atbm_bh_read_ctrl_reg_unlock,
										atbm_data_read_unlock,atbm_sdio_submit_skb_in_thread,ATBM_RX_DERICTLY_DATA_FRAME);
		}
		hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);

		
		spin_lock_bh(&hw_priv->rx_frame_queue.lock);
		if(!atbm_skb_queue_empty(&hw_priv->rx_frame_queue))
			goto restart;
		hw_priv->bh_running = false;
		spin_unlock_bh(&hw_priv->rx_frame_queue.lock);
		return;
#endif
	}else {
		WARN_ON(hard_irq == false);
	}
//	hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
	atbm_sdio_process_read_data(hw_priv,atbm_bh_read_ctrl_reg,atbm_data_read,atbm_rx_directly,ATBM_RX_RAW_FRAME);
//	hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
}

void atbm_irq_handler(struct atbm_common *hw_priv)
{
	/* To force the device to be always-on, the host sets WLAN_UP to 1 */

	if(!hw_priv->init_done){
		atbm_printk_err("[BH] irq. init_done =0 drop\n");
		return;
	}

	if (atbm_bh_is_term(hw_priv))
		return;
	
	hw_priv->hard_irq = !in_interrupt() ? false : true;
	if(hw_priv->hard_irq == false)	{
		int rx_counter = 0;
		
		__atbm_irq_enable(hw_priv,0);
rx_continue:
#ifdef CONFIG_SDIO_IRQ_THREAD_PROCESS_DATA
		rx_counter = atbm_sdio_process_read_data(hw_priv,atbm_bh_read_ctrl_reg_unlock,atbm_data_read_unlock,
									atbm_sdio_submit_skb,ATBM_RX_DERICTLY_DATA_FRAME);
#else 
		rx_counter = 0;
#endif
		if(rx_counter == 0){
			/*
			*triger rx to make sure that we can not miss irq
			*/
			atbm_printk_debug("%s:no package triger rx\n",__func__);
			goto triger_rx;
		}else if(rx_counter >= ATBM_MAX_OVERFLOW_SIZE){
			hw_priv->sbus_ops->unlock(hw_priv->sbus_priv);
			atbm_printk_debug("%s:over flow\n",__func__);
			schedule_timeout_interruptible(msecs_to_jiffies(10));			
			hw_priv->sbus_ops->lock(hw_priv->sbus_priv);
			goto rx_continue;
		}
		if (atbm_bh_is_term(hw_priv) || atomic_read(&hw_priv->bh_term)){
			wake_up(&hw_priv->bh_wq);
		}
		return;
	}
triger_rx:	
	if(hw_priv->sbus_ops->sbus_rev_schedule)
		hw_priv->sbus_ops->sbus_rev_schedule(hw_priv->sbus_priv);
	else if (atomic_add_return(1, &hw_priv->bh_rx) == 1){
		wake_up(&hw_priv->bh_wq);
	}
	return;
}

int atbm_bh_suspend(struct atbm_common *hw_priv)
{
	int i =0;
	int ret = 0;
	struct atbm_vif *priv = NULL;
	atbm_printk_pm("[BH] try suspend.\n");
	if (hw_priv->bh_error) {
		atbm_printk_warn("BH error -- can't suspend\n");
		return -EINVAL;
	}
#ifdef MCAST_FWDING

 	atbm_for_each_vif(hw_priv, priv, i) {
		if (!priv)
			continue;
		if ( (priv->multicast_filter.enable)
			&& (priv->join_status == ATBM_APOLLO_JOIN_STATUS_AP) ) {
			wsm_release_buffer_to_fw(priv,
				(hw_priv->wsm_caps.numInpChBufs - 1));
			break;
		}
	}
#endif
	atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_SUSPEND);
	wake_up(&hw_priv->bh_wq);
	ret = atbm_wait_event_timeout_stay_awake(hw_priv,hw_priv->bh_evt_wq, hw_priv->bh_error ||
		(ATBM_APOLLO_BH_SUSPENDED == atomic_read(&hw_priv->bh_suspend)),
		 60 * HZ,false) ? 0 : -ETIMEDOUT;
	
	if((ret == 0)&&(hw_priv->sbus_ops->sbus_bh_suspend))
		ret = hw_priv->sbus_ops->sbus_bh_suspend(hw_priv->sbus_priv);

	return ret;
}

int atbm_bh_resume(struct atbm_common *hw_priv)
{
	int i =0;
	int ret;
#ifdef MCAST_FWDING
	struct atbm_vif *priv =NULL;
#endif

	atbm_printk_pm("[BH] try resume.\n");
	if (hw_priv->bh_error) {
		atbm_printk_warn("BH error -- can't resume\n");
		return -EINVAL;
	}

	atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_RESUME);
	wake_up(&hw_priv->bh_wq);
    atbm_printk_pm("wakeup bh,wait evt_wq\n");
#ifdef MCAST_FWDING
	ret = atbm_wait_event_timeout_stay_awake(hw_priv,hw_priv->bh_evt_wq, hw_priv->bh_error ||
				(ATBM_APOLLO_BH_RESUMED == atomic_read(&hw_priv->bh_suspend)),
				1 * HZ,false) ? 0 : -ETIMEDOUT;

	atbm_for_each_vif(hw_priv, priv, i) {
		if (!priv)
			continue;
		if ((priv->join_status == ATBM_APOLLO_JOIN_STATUS_AP)
				&& (priv->multicast_filter.enable)) {
			u8 count = 0;
			WARN_ON(wsm_request_buffer_request(priv, &count));
			bh_printk(
				"[BH] BH resume. Reclaim Buff %d \n",count);
			break;
		}
	}
#else
	ret = atbm_wait_event_timeout_stay_awake(hw_priv,hw_priv->bh_evt_wq,hw_priv->bh_error ||
		(ATBM_APOLLO_BH_RESUMED == atomic_read(&hw_priv->bh_suspend)),
		100 * HZ,false) ? 0 : -ETIMEDOUT;
#endif
	if((ret == 0)&&(hw_priv->sbus_ops->sbus_bh_resume))
		ret = hw_priv->sbus_ops->sbus_bh_resume(hw_priv->sbus_priv);
	return ret;
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
	int hw_bufs_used;
	spin_lock_bh(&hw_priv->tx_com_lock);
	hw_bufs_used = hw_priv->hw_bufs_used;
	hw_priv->hw_bufs_used -= count;

	if (WARN_ON(hw_priv->hw_bufs_used < 0))
		//ret = -1;
		hw_priv->hw_bufs_used=0;
	/* Tx data patch stops when all but one hw buffers are used.
	   So, re-start tx path in case we find hw_bufs_used equals
	   numInputChBufs - 1.
	 */
	else if (hw_bufs_used >= (hw_priv->wsm_caps.numInpChBufs - 1))
		ret = 1;
	spin_unlock_bh(&hw_priv->tx_com_lock);
	if (!(hw_priv->hw_bufs_used))
		wake_up(&hw_priv->bh_evt_wq);
	return ret;
}

#ifdef MCAST_FWDING
#ifndef USB_BUS

//just for sdio
int wsm_release_buffer_to_fw(struct atbm_vif *priv, int count)
{
	int i;
	u8 flags;
	struct wsm_buf *buf;
	u32 buf_len;
	struct wsm_hdr_tx *wsm;
	struct atbm_common *hw_priv = priv->hw_priv;


	if (priv->join_status != ATBM_APOLLO_JOIN_STATUS_AP) {
		return 0;
	}

	bh_printk( "Rel buffer to FW %d, %d\n", count, hw_priv->hw_bufs_used);

	for (i = 0; i < count; i++) {
		if ((hw_priv->hw_bufs_used + 1) < hw_priv->wsm_caps.numInpChBufs) {
			flags = i ? 0: 0x1;

			wsm_alloc_tx_buffer(hw_priv);

			buf = &hw_priv->wsm_release_buf[i];
			buf_len = buf->data - buf->begin;

			/* Add sequence number */
			wsm = (struct wsm_hdr_tx *)buf->begin;
			BUG_ON(buf_len < sizeof(*wsm));

			wsm->id &= __cpu_to_le32(
				~WSM_TX_SEQ(WSM_TX_SEQ_MAX));
			wsm->id |= cpu_to_le32(
				WSM_TX_SEQ(hw_priv->wsm_tx_seq));

			atbm_printk_bh("REL %d\n", hw_priv->wsm_tx_seq);

			if (WARN_ON(atbm_data_write(hw_priv,
				buf->begin, buf_len))) {
				break;
			}


			hw_priv->buf_released = 1;
			hw_priv->wsm_tx_seq = (hw_priv->wsm_tx_seq + 1) & WSM_TX_SEQ_MAX;
		} else
			break;
	}

	if (i == count) {
		return 0;
	}

	/* Should not be here */
	atbm_printk_err("[BH] Less HW buf %d,%d.\n", hw_priv->hw_bufs_used,
			hw_priv->wsm_caps.numInpChBufs);
	WARN_ON(1);

	return -1;
}
#endif //USB_BUS
#endif
static struct sk_buff *atbm_get_skb(struct atbm_common *hw_priv, u32 len)
{
	struct sk_buff *skb;
	u32 alloc_len = (len > SDIO_BLOCK_SIZE) ? len : SDIO_BLOCK_SIZE;

	if (len > SDIO_BLOCK_SIZE || !hw_priv->skb_cache) {
		skb = __atbm_dev_alloc_skb(alloc_len
				+ WSM_TX_EXTRA_HEADROOM
				+ 8  /* TKIP IV */
				+ 12 /* TKIP ICV + MIC */
				- 2  /* Piggyback */,GFP_KERNEL);
		BUG_ON(skb==NULL);
		/* In AP mode RXed SKB can be looped back as a broadcast.
		 * Here we reserve enough space for headers. */
#if 0
		atbm_skb_reserve(skb, WSM_TX_EXTRA_HEADROOM
				+ 8 /* TKIP IV */
				- WSM_RX_EXTRA_HEADROOM);
#endif
		atbm_skb_reserve(skb, ALIGN(WSM_TX_EXTRA_HEADROOM
   			+ 8 /* TKIP IV */
    		- WSM_RX_EXTRA_HEADROOM,64));
	} else {
		skb = hw_priv->skb_cache;
		hw_priv->skb_cache = NULL;
	}
	return skb;
}


void atbm_put_skb(struct atbm_common *hw_priv, struct sk_buff *skb)
{
	if (hw_priv->skb_cache){
		//printk("%s atbm_kfree_skb skb=%p\n",__func__,skb);
		atbm_dev_kfree_skb(skb);
	}
	else
		hw_priv->skb_cache = skb;
}

int atbm_bh_read_ctrl_reg(struct atbm_common *hw_priv,
					  u16 *ctrl_reg)
{
	int ret=0,retry=0;
	while (retry <= MAX_RETRY) {
		ret = atbm_reg_read_16(hw_priv,
				ATBM_HIFREG_CONTROL_REG_ID, ctrl_reg);
		if(!ret){
				break;
		}else{
			/*reset sdio internel reg by send cmd52 to abort*/
			WARN_ON(hw_priv->sbus_ops->abort(hw_priv->sbus_priv));
			retry++;
			mdelay(retry);
			atbm_printk_err(
				"[BH] Failed to read control register.ret=%x\n",ret);
		}
	}
	return ret;
}
int atbm_bh_read_ctrl_reg_unlock(struct atbm_common *hw_priv,
					  u16 *ctrl_reg)
{
	int ret=0,retry=0;
	while (retry <= MAX_RETRY) {
		ret = atbm_reg_read_16_unlock(hw_priv,
				ATBM_HIFREG_CONTROL_REG_ID, ctrl_reg);
		if(!ret){
				break;
		}else{
			/*reset sdio internel reg by send cmd52 to abort*/
			WARN_ON(hw_priv->sbus_ops->abort(hw_priv->sbus_priv));
			retry++;
			mdelay(retry);
			atbm_printk_err(
				"[BH] Failed to read control register.ret=%x\n",ret);
		}
	}
	return ret;
}

//just ARESB have this function
//used this function to clear sdio rtl bug register
// if not do this sdio direct mode (wr/read reigster) will not work
// this function is the same to atbm_data_force_write (used queue mode clear bit to clear)
// 
int atbm_powerave_sdio_sync(struct atbm_common *hw_priv)
{
	int ret=0;
	//int retry=0;
	u32 config_reg;
	ret = atbm_reg_read_unlock_32(hw_priv, ATBM_HIFREG_CONFIG_REG_ID, &config_reg);
	if (ret < 0) {
		atbm_printk_err("%s: enable_irq: can't read config register.\n", __func__);
	}

	if(config_reg & ATBM_HIFREG_PS_SYNC_SDIO_FLAG)
	{
		atbm_printk_err("%s:%d\n",__func__,__LINE__);
		//atbm_hw_buff_reset(hw_priv);
		config_reg |= ATBM_HIFREG_PS_SYNC_SDIO_CLEAN;
		atbm_reg_write_unlock_32(hw_priv,ATBM_HIFREG_CONFIG_REG_ID,config_reg);
	}
	return ret;
}
int atbm_device_wakeup(struct atbm_common *hw_priv)
{
	u16 ctrl_reg;
	int ret=0;
	int loop = 1;

#ifdef PS_SETUP

	/* To force the device to be always-on, the host sets WLAN_UP to 1 */
	ret = atbm_reg_write_16(hw_priv, ATBM_HIFREG_CONTROL_REG_ID,
			ATBM_HIFREG_CONT_WUP_BIT);
	if (WARN_ON(ret))
		return ret;
#endif
	while(1){
		mdelay(5);
		ret = atbm_bh_read_ctrl_reg(hw_priv, &ctrl_reg);
		if (WARN_ON(ret)){
		}
		/* If the device returns WLAN_RDY as 1, the device is active and will
		 * remain active. */
		atbm_printk_bh("Rdy =%x\n",ctrl_reg);
		if (ctrl_reg & ATBM_HIFREG_CONT_RDY_BIT) {
			atbm_printk_bh("[BH] Device awake.<%d>\n",loop);
			ret= 1;
			break;
		}
	}
	return ret;
}
int rxMutiCnt[17]={0};
int rxCnt=0;
int rxMutiCnt_Num;
unsigned long g_printf_count = 0;
int atbm_rx_tasklet(struct atbm_common *hw_priv, int id,
		  struct wsm_hdr *wsm, struct sk_buff **skb_p)
{
	struct sk_buff *skb = *skb_p;
	struct sk_buff *atbm_skb_copy;
	//struct wsm_hdr *wsm;
	u32 wsm_len;
	int wsm_id;
	int data_len;
	int ret=0;
#define RX_ALLOC_BUFF_OFFLOAD (  (40+16)/*RX_DESC_OVERHEAD*/ -16 /*WSM_HI_RX_IND*/)

		wsm_len = __le32_to_cpu(wsm->len);
		wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
		if((wsm_id == WSM_MULTI_RECEIVE_INDICATION_ID)||
			(WSM_SINGLE_CHANNEL_MULTI_RECEIVE_INDICATION_ID == wsm_id)){
			struct wsm_multi_rx *  multi_rx = (struct wsm_multi_rx *)skb->data;			
			int RxFrameNum = multi_rx->RxFrameNum;
			
			data_len = wsm_len ;
			data_len -= sizeof(struct wsm_multi_rx);
			
			rxMutiCnt[ALIGN(wsm_len,1024)/1024]++;
			rxMutiCnt_Num+=RxFrameNum;
			atbm_bh_multrx_trace(hw_priv,RxFrameNum);
			wsm = (struct wsm_hdr *)(multi_rx+1);
			wsm_len = __le32_to_cpu(wsm->len);
			wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
			
			//frame_hexdump("dump sdio wsm rx ->",wsm,32);
			do {

				if(data_len < wsm_len){
					atbm_printk_err("skb->len %x,wsm_len %x data_len %x\n",skb->len,wsm_len,data_len);
					//frame_hexdump("dump sdio wsm rx ->",skb->data,64);
					break;
				}
				WARN_ON((wsm_id  & (~WSM_TX_LINK_ID(WSM_TX_LINK_ID_MAX))) !=  WSM_RECEIVE_INDICATION_ID);
				atbm_skb_copy = __atbm_dev_alloc_skb(wsm_len + 16,GFP_KERNEL);
				BUG_ON(atbm_skb_copy == NULL);
				atbm_skb_reserve(atbm_skb_copy,  (8 - (((unsigned long)atbm_skb_copy->data)&7))/*ALIGN 8*/);
				atbm_skb_copy->pkt_type = skb->pkt_type;
				memmove(atbm_skb_copy->data, wsm, wsm_len);
				atbm_skb_put(atbm_skb_copy,wsm_len);
				ret=wsm_handle_rx(hw_priv,wsm_id,wsm,&atbm_skb_copy);
				if(ret){
					rxMutiCnt_Num=0;
					memset(rxMutiCnt,0,sizeof(rxMutiCnt));
					if(atbm_skb_copy != NULL){
						atbm_dev_kfree_skb(atbm_skb_copy);
					}
					return ret;
				}
				data_len -= ALIGN(wsm_len + RX_ALLOC_BUFF_OFFLOAD,4);
				RxFrameNum--;

				wsm = (struct wsm_hdr *)((u8 *)wsm +ALIGN(( wsm_len + RX_ALLOC_BUFF_OFFLOAD),4));
				wsm_len = __le32_to_cpu(wsm->len);
				wsm_id	= __le32_to_cpu(wsm->id) & 0xFFF;
				
				if(atbm_skb_copy != NULL){
					atbm_dev_kfree_skb(atbm_skb_copy);
				}
			}while((RxFrameNum>0) && (data_len > 32));
			BUG_ON(RxFrameNum != 0);
			
		}
		else {
			//rxMutiCnt[ALIGN(wsm_len,1024)/1024]++;
			rxCnt++;
			ret=wsm_handle_rx(hw_priv,id,wsm,skb_p);
		}
		return ret;

}
//#define DEBUG_SDIO
#ifdef ATBM_SDIO_PATCH
#define CHECKSUM_LEN 4
static u32 GloId_array[64]={0};
u16 atbm_CalCheckSum(const u8 *data,u16 len)
{
  u16 t;
  const u8 *dataptr;
  const u8 *last_byte;
  u16 sum = 0;
  dataptr = data;
  last_byte = data + len - 1;
  while(dataptr < last_byte) {	/* At least two more bytes */
    t = (dataptr[0] << 8) + dataptr[1];
    sum += t;
    if(sum < t) {
      sum++;		/* carry */
    }
    dataptr += 2;
  }
  
  if(dataptr == last_byte) {
    t = (dataptr[0] << 8) + 0;
    sum += t;
    if(sum < t) {
      sum++;		/* carry */
    }
  }
  sum=(~sum)&0xffff;

  /* Return sum in host byte order. */
  return sum;
}
void atbm_packetId_to_seq(struct atbm_common *hw_priv,u32 packetId)
{
	GloId_array[hw_priv->SdioSeq]=packetId;
}
int atbm_seq_to_packetId(struct atbm_common *hw_priv,u32 seq)
{
	u32 packetId;
	packetId=GloId_array[seq];
	GloId_array[seq]=0;
	return packetId;
}
#endif //ATBM_SDIO_PATCH
static int atbm_bh(void *arg)
{
	struct atbm_common *hw_priv = arg;
	struct atbm_vif *priv = NULL;
	int rx, tx=0, term, suspend;
	u16 ctrl_reg = 0;
	int pending_tx = 0;
	long status;
	bool powersave_enabled;
	int i;
	int ret_flush;				

	
#define __ALL_HW_BUFS_USED (hw_priv->hw_bufs_used)
	while (1) {
		powersave_enabled = 1;
		atbm_hw_vif_read_lock(&hw_priv->vif_list_lock);
		atbm_for_each_vif_safe(hw_priv, priv, i) 
		{
			if (!priv)
				continue;
			powersave_enabled &= !!priv->powersave_enabled;
		}
		atbm_hw_vif_read_unlock(&hw_priv->vif_list_lock);
		if (!__ALL_HW_BUFS_USED
				&& powersave_enabled
				&& !hw_priv->device_can_sleep
				&& !atomic_read(&hw_priv->recent_scan)) {
			status = HZ/8;
			bh_printk( "[BH] No Device wakedown.\n");
#ifdef PS_SETUP
			WARN_ON(atbm_reg_write_16(hw_priv,
						ATBM_HIFREG_CONTROL_REG_ID, 0));
			hw_priv->device_can_sleep = true;
#endif
		} else if (__ALL_HW_BUFS_USED)
			/* Interrupt loss detection */
			status = HZ;
		else
			status = HZ/4;
		/* If a packet has already been txed to the device then read the
		   control register for a probable interrupt miss before going
		   further to wait for interrupt; if the read length is non-zero
		   then it means there is some data to be received */
		status = wait_event_interruptible_timeout(hw_priv->bh_wq, ({
				rx = atomic_xchg(&hw_priv->bh_rx, 0);
				tx = atomic_xchg(&hw_priv->bh_tx, 0);
				term = atomic_xchg(&hw_priv->bh_term, 0);
				suspend = pending_tx ?
					0 : atomic_read(&hw_priv->bh_suspend);
				(rx || tx || term || suspend || hw_priv->bh_error || atomic_read(&hw_priv->bh_halt));
			}), status);
		
		if (status < 0 || term || hw_priv->bh_error){
			atbm_bh_read_ctrl_reg(hw_priv, &ctrl_reg);
			//printk(" ++ctrl_reg= %x,\n",ctrl_reg);
			atbm_printk_err("%s BH thread break %ld %d %d ctrl_reg=%x\n",__func__,status,term,hw_priv->bh_error,ctrl_reg);
			break;
		}

		if(atomic_read(&hw_priv->bh_halt)){
			atomic_set(&hw_priv->atbm_pluged,0);
			atbm_printk_err("%s:bh_halt\n",__func__);
			if(hw_priv->sbus_ops->lmac_restart(hw_priv->sbus_priv) != 0){
				atomic_xchg(&hw_priv->bh_halt,0);
				atomic_set(&hw_priv->bh_term,1);
				hw_priv->bh_error = 1;
				break;
			}
		}
		
		if (0)
		{
			unsigned long timestamp = jiffies;
			long timeout;
			bool pending = false;
			int i;
			
			atbm_printk_warn("Missed interrupt Status =%d, buffused=%d\n",(int)status,(int)__ALL_HW_BUFS_USED);
			rx = 1;
			atbm_printk_debug("[bh] next wsm_rx_seq %d wsm_tx_seq %d\n",hw_priv->wsm_rx_seq,hw_priv->wsm_tx_seq);
			atbm_printk_debug("[bh] wsm_hiftx_cmd_num %d wsm_hif_cmd_conf_num %d\n",hw_priv->wsm_hiftx_num,hw_priv->wsm_hifconfirm_num);
			atbm_printk_debug("[bh] wsm_txframe_num %d wsm_txconfirm_num %d\n",hw_priv->wsm_txframe_num,hw_priv->wsm_txconfirm_num);
			atbm_printk_debug("[bh] num_pending[0]=%d num_pending[1]=%d pending[2]=%d pending[3]=%d\n",
															hw_priv->tx_queue[0].num_pending,
															hw_priv->tx_queue[1].num_pending,
															hw_priv->tx_queue[2].num_pending,
															hw_priv->tx_queue[3].num_pending);
			//atbm_monitor_pc(hw_priv);

			atbm_bh_read_ctrl_reg(hw_priv, &ctrl_reg);
			atbm_printk_err(" ++ctrl_reg= %x,\n",ctrl_reg);

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
				ret_flush=wsm_sync_channle_process(hw_priv,IN_BH);
				if(ret_flush==RECOVERY_ERR){
					atbm_printk_err("RESET CHANN ERR %d\n",__LINE__);
					break;
				}else{
					ctrl_reg=0;
					continue;
				}
			}
		} else if (!status) {
			if (!hw_priv->device_can_sleep
					&& !atomic_read(&hw_priv->recent_scan)) {
			        bh_printk(KERN_ERR "[BH] Device wakedown. Timeout.\n");
#ifdef PS_SETUP
				WARN_ON(atbm_reg_write_16(hw_priv,
						ATBM_HIFREG_CONTROL_REG_ID, 0));
				hw_priv->device_can_sleep = true;
#endif//#ifdef PS_SETUP
			}
			continue;
		} else if (suspend) {
			atbm_printk_err("[BH] Device suspend.\n");
			powersave_enabled = 1;
			//if in recovery clear reg
			if(hw_priv->syncChanl_done==0){
			     ctrl_reg=0;
			}
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
			atbm_printk_err("[BH] wait resume..\n");
			status = wait_event_interruptible(hw_priv->bh_wq,
					ATBM_APOLLO_BH_RESUME == atomic_read(
						&hw_priv->bh_suspend));
			if (status < 0) {
				atbm_printk_err("%s: Failed to wait for resume: %ld.\n",
					__func__, status);
				break;
			}
			atbm_printk_err("[BH] Device resume.\n");
			atomic_set(&hw_priv->bh_suspend, ATBM_APOLLO_BH_RESUMED);
			wake_up(&hw_priv->bh_evt_wq);
			atomic_add(1, &hw_priv->bh_rx);
			continue;
		}
	}

	if (!term) {
		int loop = 3;
		atbm_dbg(ATBM_APOLLO_DBG_ERROR, "[BH] Fatal error, exitting.\n");
		hw_priv->bh_error = 1;
		while(loop-->0){
			atbm_monitor_pc(hw_priv);
			msleep(10);
		}

		hw_priv->sbus_ops->wtd_wakeup(hw_priv->sbus_priv);
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
		atbm_dbg(ATBM_APOLLO_DBG_ERROR, "[BH] Fatal error, exitting.1\n");
		/* TODO: schedule_work(recovery) */
#ifndef HAS_PUT_TASK_STRUCT
		/* The only reason of having this stupid code here is
		 * that __put_task_struct is not exported by kernel. */
		for (;;) {
			int status = wait_event_interruptible(hw_priv->bh_wq, ({
				term = atomic_xchg(&hw_priv->bh_term, 0);
				(term);
				}));
			
			atbm_dbg(ATBM_APOLLO_DBG_ERROR, "[BH] Fatal error, exitting.2\n");
			if (status || term)
				break;
		}
#endif
	}
	atbm_printk_exit("atbm_wifi_BH_thread stop ++\n");
	/*
	add this code just because 'linux kernel' need kthread not exit ,
	before kthread_stop func call,
	*/
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
				hw_priv->scan.curr = hw_priv->scan.end;
				mutex_unlock(&hw_priv->conf_mutex);
				atbm_scan_timeout(&hw_priv->scan.timeout.work);
				mutex_lock(&hw_priv->conf_mutex);
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
#ifdef CONFIG_ATBM_SUPPORT_P2P
			if(atbm_hw_cancel_delayed_work(&hw_priv->rem_chan_timeout,true))
				atbm_rem_chan_timeout(&hw_priv->rem_chan_timeout.work);
#endif
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->scan.work,atbm_scan_work);
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->event_handler,atbm_event_handler);
#ifdef CONFIG_ATBM_BA_STATUS
			ATBM_CANCEL_PENDDING_WORK(&hw_priv->ba_work,atbm_ba_work);
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
				//ATBM_CANCEL_PENDDING_WORK(&priv->chantype_change_work, atbm_channel_type_change_work);
				
#ifdef CONFIG_ATBM_40M_AUTO_CCA
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
				if(atbm_cancle_delayed_work(&priv->set_cts_work,true))
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
	while(term){
		if(kthread_should_stop()){
			break;
		}
		schedule_timeout_uninterruptible(msecs_to_jiffies(100));
	}
	atbm_printk_exit("atbm_wifi_BH_thread stop --\n");
	return 0;
}
