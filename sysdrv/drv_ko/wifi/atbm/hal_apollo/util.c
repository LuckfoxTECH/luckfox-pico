
#include <net/atbm_mac80211.h>
#include <linux/kthread.h>
#include <linux/dma-mapping.h>
#include <linux/skbuff.h>

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
/* Must be called from BH thraed. */
void atbm_enable_powersave(struct atbm_vif *priv,
			     bool enable)
{
	atbm_dbg(ATBM_APOLLO_DBG_MSG, "[BH] Powerave is %s.\n",
			enable ? "enabled" : "disabled");
	priv->powersave_enabled = enable;
}

void atbm_bh_wakeup(struct atbm_common *hw_priv)
{
	atbm_dbg(ATBM_APOLLO_DBG_MSG, "[BH] wakeup.\n");
	if(hw_priv->sbus_ops->sbus_xmit_schedule)
		hw_priv->sbus_ops->sbus_xmit_schedule(hw_priv->sbus_priv);
	else if (atomic_add_return(1, &hw_priv->bh_tx) == 1){
		wake_up(&hw_priv->bh_wq);
	}
}

int wsm_release_vif_tx_buffer(struct atbm_common *hw_priv, int if_id,
				int count)
{
	int ret = 0;

	spin_lock_bh(&hw_priv->tx_com_lock);
	hw_priv->hw_bufs_used_vif[if_id] -= count;

	if (WARN_ON(hw_priv->hw_bufs_used_vif[if_id] < 0)){
		atbm_printk_err( "%s:[%d][%d]\n",__func__,if_id,hw_priv->hw_bufs_used_vif[if_id]);
		hw_priv->hw_bufs_used_vif[if_id] = 0;
		//BUG_ON(1);
		//ret = -1;
	}

	spin_unlock_bh(&hw_priv->tx_com_lock);

	if (!hw_priv->hw_bufs_used_vif[if_id])
		wake_up(&hw_priv->bh_evt_wq);

	return ret;
}


int wsm_release_vif_tx_buffer_Nolock(struct atbm_common *hw_priv, int if_id,
				int count)
{
	int ret = 0;

	hw_priv->hw_bufs_used_vif[if_id] -= count;

	if (!hw_priv->hw_bufs_used_vif[if_id])
		wake_up(&hw_priv->bh_evt_wq);

	if (WARN_ON(hw_priv->hw_bufs_used_vif[if_id] < 0)){
		atbm_printk_err( "%s:[%d][%d]\n",__func__,if_id,hw_priv->hw_bufs_used_vif[if_id]);
		hw_priv->hw_bufs_used_vif[if_id] =0;
		//BUG_ON(1);
		//ret = -1;
	}

	return ret;
}
void atbm_monitor_pc(struct atbm_common *hw_priv)
{
	u32 testreg1[10] = {0};
	u32 testreg_pc = 0;
	u32 testreg_ipc = 0;
	u32 val28;
	u32 val20;
	int i = 0;

	atbm_direct_write_reg_32(hw_priv,0x16100050,1);
	//atbm_direct_read_reg_32(hw_priv,0x18e00014,&testreg1);
	atbm_direct_read_reg_32(hw_priv,0x16100054,&testreg_pc);
	atbm_direct_read_reg_32(hw_priv,0x16100058,&testreg_ipc);
	atbm_direct_read_reg_32(hw_priv,0x16101028,&val28);
	atbm_direct_read_reg_32(hw_priv,0x16101020,&val20);
	//atbm_direct_read_reg_32(hw_priv,0x16400000,&testreg_uart);

	for(i=0;i<10;i++){
		atbm_direct_read_reg_32(hw_priv,hw_priv->wsm_caps.exceptionaddr+4*i+4,&testreg1[i]);
	}
	atbm_direct_write_reg_32(hw_priv,0x16100050,0);

	atbm_printk_err("ERROR !! pc:[%x],ipc[%x] \n",testreg_pc,testreg_ipc);
	atbm_printk_err("ERROR !! reg0:[%x],reg1[%x],reg2[%x],reg3[%x],reg4[%x],reg5[%x],reg6[%x] \n",
														testreg1[0],
														testreg1[1],
														testreg1[2],
														testreg1[3],
														testreg1[4],
														testreg1[5],
														testreg1[6]);

	atbm_printk_err( "[PC]:0x16101028(%x)\n",val28);	
	atbm_printk_err( "[PC]:0x16101020(%x)\n",val20);
	
}


#ifdef	ATBM_WIFI_QUEUE_LOCK_BUG

void atbm_set_priv_queue_cap(struct atbm_vif *priv)
{
	struct atbm_common	*hw_priv = priv->hw_priv;
	struct atbm_vif *other_priv;
	u8 i = 0;

	priv->queue_cap = ATBM_QUEUE_SINGLE_CAP;
	atbm_for_each_vif(hw_priv, other_priv, i) {
		if(other_priv == NULL)
			continue;
		if(other_priv == priv)
			continue;
		if(other_priv->join_status <=  ATBM_APOLLO_JOIN_STATUS_MONITOR)
			continue;
		other_priv->queue_cap = ATBM_QUEUE_COMB_CAP;
		priv->queue_cap = ATBM_QUEUE_COMB_CAP;
	}

}

void atbm_clear_priv_queue_cap(struct atbm_vif *priv)
{
	struct atbm_common	*hw_priv = priv->hw_priv;
	struct atbm_vif *other_priv;
	struct atbm_vif *prev_priv = NULL;
	u8 i = 0;

	priv->queue_cap = ATBM_QUEUE_DEFAULT_CAP;

	atbm_for_each_vif(hw_priv, other_priv, i) {

		if(other_priv == NULL)
			continue;
		if(other_priv == priv)
			continue;
		if(other_priv->join_status <=  ATBM_APOLLO_JOIN_STATUS_MONITOR)
			continue;
		
		other_priv->queue_cap = ATBM_QUEUE_SINGLE_CAP;

		if(prev_priv == NULL){
			prev_priv = other_priv;
			continue;
		}

		prev_priv->queue_cap = ATBM_QUEUE_COMB_CAP;
		other_priv->queue_cap = ATBM_QUEUE_COMB_CAP;
		prev_priv = other_priv;
		
	}
}
#endif

void atbm_xmit_linearize(struct atbm_common	*hw_priv,
	 struct wsm_tx *wsm,char *xmit,int xmit_len)
{
	int wsm_id = __le16_to_cpu(wsm->hdr.id) & 0x3F;	
	
	while(wsm_id == WSM_TRANSMIT_REQ_MSG_ID){
		
		u8 queueId = wsm_queue_id_to_linux(wsm->queueId & 0x03);
		struct atbm_queue *queue = &hw_priv->tx_queue[queueId];
		const struct atbm_txpriv *txpriv = NULL;
		struct sk_buff *skb = NULL;
		int sg_len = 0;
		int sg = 0;
		struct ieee80211_tx_info *tx_info;
		
		if(atbm_queue_get_skb(queue,wsm->packetID,
				&skb, &txpriv) != 0){
			WARN_ON(1);
			break;
		}

		BUG_ON((void *)skb->data != (void *)wsm);

		if(!skb_is_nonlinear(skb)){
			break;
		}

		tx_info = IEEE80211_SKB_CB(skb);
		
		printk_once(KERN_ERR "sg process\n");

		memcpy(xmit,skb->data,skb_headlen(skb));

		sg_len += skb_headlen(skb);
		xmit += sg_len;
		
		for (sg = 0; sg < skb_shinfo(skb)->nr_frags; sg++){
			
			skb_frag_t *frag = &skb_shinfo(skb)->frags[sg];
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 0, 0))
			memcpy(xmit,page_address(frag->bv_page) + frag->bv_offset,frag->bv_len);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0))		
			memcpy(xmit,page_address(frag->page) + frag->page_offset,frag->size);
#else
			memcpy(xmit,page_address(frag->page.p) + frag->page_offset,frag->size);
#endif
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5, 0, 0))
			xmit += frag->bv_len;
			sg_len += frag->bv_len;
#else
			xmit += frag->size;
			sg_len += frag->size;
#endif
		}
		
		if(tx_info->sg_tailneed){
			printk_once(KERN_ERR "sg_tailneed(%d)\n",tx_info->sg_tailneed);
			memset(xmit,0,tx_info->sg_tailneed);
			if(sg_len + tx_info->sg_tailneed != xmit_len){
				atbm_printk_err("sg_len(%d),xmit_len(%d),sg_tailneed(%d)\n",sg_len,xmit_len,tx_info->sg_tailneed);
				WARN_ON_ONCE(1);
			}

			sg_len += tx_info->sg_tailneed;
		}
		WARN_ON_ONCE(sg_len != xmit_len);
		return;		
	}

	memcpy(xmit,wsm,xmit_len);
}

int atbm_save_efuse(struct atbm_common *hw_priv,struct efuse_headr *efuse_save)
{
	int ret = 0;
	int iResult=0;
	//struct atbm_vif *vif;
	struct efuse_headr efuse_bak;
	
	/*
	*LMC_STATUS_CODE__EFUSE_VERSION_CHANGE	failed because efuse version change  
	*LMC_STATUS_CODE__EFUSE_FIRST_WRITE, 		failed because efuse by first write   
	*LMC_STATUS_CODE__EFUSE_PARSE_FAILED,		failed because efuse data wrong, cannot be parase
	*LMC_STATUS_CODE__EFUSE_FULL,				failed because efuse have be writen full
	*/
	ret = wsm_efuse_change_data_cmd(hw_priv, efuse_save,0);
	if (ret == LMC_STATUS_CODE__EFUSE_FIRST_WRITE)
	{
		atbm_printk_err("first write\n");
		iResult = -3;
	}else if (ret == LMC_STATUS_CODE__EFUSE_PARSE_FAILED)
	{
		atbm_printk_err("parse failed\n");
		iResult = -4;
	}else if (ret == LMC_STATUS_CODE__EFUSE_FULL)
	{
		atbm_printk_err("efuse full\n");
		iResult = -5;
	}else if (ret == LMC_STATUS_CODE__EFUSE_VERSION_CHANGE)
	{
		atbm_printk_err("efuse version change\n");
		iResult = -6;
	}else
	{
		iResult = 0;
	}
	if (iResult == 0)
	{
		//frame_hexdump("efuse_d", efuse_save, sizeof(struct efuse_headr));
		memset(&efuse_bak,0,sizeof(struct efuse_headr));
		wsm_get_efuse_data(hw_priv,(void *)&efuse_bak, sizeof(struct efuse_headr));

		if(efuse_bak.specific != 0)
		{
			//sigmastar oid
			efuse_save->specific = efuse_bak.specific;
		}
		
		if(memcmp((void *)&efuse_bak, efuse_save, sizeof(struct efuse_headr)) !=0)
		{
			frame_hexdump("efuse_bak", (u8 *)&efuse_bak, sizeof(struct efuse_headr));
			iResult = -2;
		}else
		{
			iResult = 0;
		}
	}
	return iResult;
}
void atbm_destroy_wsm_cmd(struct atbm_common *hw_priv)
{
	/*
	*flush work
	*/
	atbm_flush_workqueue(hw_priv->workqueue);
	atbm_printk_exit("Flush hw_priv->workqueue\n");
	/*
	*try to release wsm_oper_unlock
	*/
#ifdef OPER_CLOCK_USE_SEM
	atbm_del_timer_sync(&hw_priv->wsm_pm_timer);
	spin_lock_bh(&hw_priv->wsm_pm_spin_lock);
	if(atomic_read(&hw_priv->wsm_pm_running) == 1){
		atomic_set(&hw_priv->wsm_pm_running, 0);
		wsm_oper_unlock(hw_priv);
		atbm_release_suspend(hw_priv);
		atbm_printk_exit("%s,up pm lock\n",__func__);
	}
	spin_unlock_bh(&hw_priv->wsm_pm_spin_lock);
#endif
	/*
	*release scan 
	*/
	if(atomic_read(&hw_priv->scan.in_progress)){
		/*
		*maybe scan work is runing,here flush it.
		*/
		atbm_printk_exit("scan running,maybe need cancle\n");
		atbm_flush_workqueue(hw_priv->workqueue);
		if(atomic_read(&hw_priv->scan.in_progress)&&(hw_priv->scan.status != -ETIMEDOUT) &&
		   (atbm_hw_cancel_delayed_work(&hw_priv->scan.timeout,true) > 0)){
		    atbm_printk_exit("scan running,try to cancle\n");
			atbm_scan_timeout(&hw_priv->scan.timeout.work);
		}
	}
	atbm_printk_exit("Flush pm and scan\n");
	atbm_flush_workqueue(hw_to_local(hw_priv->hw)->workqueue);
	atbm_flush_workqueue(hw_priv->workqueue);
	
	synchronize_net();
}

int atbm_reinit_firmware(struct atbm_common *hw_priv)
{
	struct wsm_operational_mode mode = {
		.power_mode = wsm_power_mode_quiescent,
		.disableMoreFlagUsage = true,
	};
	int ret = 0;
	u8 if_id = 0;

	/*
	*load firmware
	*/
	hw_priv->wsm_caps.firmwareReady = 0;
	ret = atbm_load_firmware(hw_priv);
	if (ret){
		atbm_printk_err( "atbm_load_firmware ERROR!\n");
		goto error_reload;
	}
	atbm_printk_init("mdelay wait wsm_startup_done  !!\n");
	if (wait_event_interruptible_timeout(hw_priv->wsm_startup_done,
			hw_priv->wsm_caps.firmwareReady,3*HZ)<=0){
		atbm_printk_err("%s: reload fw err\n",__func__);
		goto error_reload;
	}
	atomic_xchg(&hw_priv->bh_halt,0);
	atbm_firmware_init_check(hw_priv);
	for (if_id = 0; if_id < ABwifi_get_nr_hw_ifaces(hw_priv); if_id++) {
		/* Set low-power mode. */
		ret = wsm_set_operational_mode(hw_priv, &mode, if_id);
		if (ret) {
			WARN_ON(1);
			goto error_reload;
		}
		/* Enable multi-TX confirmation */
		ret = wsm_use_multi_tx_conf(hw_priv, true, if_id);
		if (ret) {
#ifndef CONFIG_TX_NO_CONFIRM
			WARN_ON(1);
			goto error_reload;
#else //CONFIG_TX_NO_CONFIRM
			ret = 0;
#endif
		}
	}
	
error_reload:
	return ret;
}
int atbm_rx_bh_flush(struct atbm_common *hw_priv)
{
	struct sk_buff *skb ;

	while ((skb = atbm_skb_dequeue(&hw_priv->rx_frame_queue)) != NULL) {
		//printk("test=====>atbm_kfree skb %p \n",skb);
		atbm_dev_kfree_skb(skb);
	}
	return 0;
}
void atbm_bh_halt(struct atbm_common *hw_priv)
{
	if (atomic_add_return(1, &hw_priv->bh_halt) == 1){
		atomic_set(&hw_priv->atbm_pluged,0);
		wake_up(&hw_priv->bh_wq);
	}
#if 0	
	spin_lock_bh(&hw_priv->wsm_cmd.lock);
	if(hw_priv->wsm_cmd.ptr == NULL){
		if(hw_priv->wsm_cmd.cmd != 0xFFFF){
			atbm_printk_err("%s:release cmd [%x]\n",__func__,hw_priv->wsm_cmd.cmd);
			hw_priv->wsm_cmd.ret = -1;
			hw_priv->wsm_cmd.done = 1;
			wake_up(&hw_priv->wsm_cmd_wq);
		}else {
			
		}
	}else {
		atbm_printk_err("%s:cmd[%x] not send\n",__func__,hw_priv->wsm_cmd.cmd);
	}
	spin_unlock_bh(&hw_priv->wsm_cmd.lock);
#endif
}
void  atbm_bh_multrx_trace(struct atbm_common *hw_priv,u8 n_rx)
{
	hw_priv->multrx_trace[hw_priv->multrx_index++] = n_rx;
	hw_priv->multrx_index &= (64-1);
}