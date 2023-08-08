/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <linux/version.h>
#include <ssv6200.h>
#include <linux/nl80211.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/time.h>
#include <linux/sched.h>

#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
#include <ssv6200.h>
#include "lib.h"
#include "dev.h"
#include "ap.h"
#include "ssv_skb.h"
#include <hal.h>

int ssv6200_bcast_queue_len(struct ssv6xxx_bcast_txq *bcast_txq);

void ssv6x5x_beacon_enable(struct ssv_hw *sh, bool bEnable)
{
    struct ssv_softc *sc = sh->sc;
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    int retval = 0;
    struct ssv6xxx_sw_beacon_params *ptr;

    skb = ssv_skb_alloc(sh->sc, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params));
    if (skb == NULL) {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params));
    host_cmd->c_type = HOST_CMD;
    host_cmd->sub_h_cmd = (bEnable ? SSV6XXX_SOFT_BEACON_START : SSV6XXX_SOFT_BEACON_STOP);
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SOFT_BEACON;
    host_cmd->len = skb->len;
    ptr = (struct ssv6xxx_sw_beacon_params *)host_cmd->un.dat8;
    if (false == bEnable) {
        // only use beacon id 0
        ptr->beacon_id_maps |= 0x1; 
        //ptr->beacon_id_maps |= 0x2; 
    }
    
    if(sh->cfg.ap_force_tim_always_high==1)
    {
        ptr->force_tim_to_high=1;
    }

    retval = HCI_SEND_CMD(sh, skb);
    if (retval) {
        printk("%s(): Fail to send soft beacon cmd\n", __FUNCTION__);
    }
    
    // fw will release pbuf, host just clear related beacon information
    if (false == bEnable) {
        
        if (sc->beacon_buf) {
            dev_kfree_skb_any(sc->beacon_buf);
            sc->beacon_buf = NULL;
        }
    }

    return;
}

void ssv6x5x_beacon_set_interval(struct ssv_hw *sh, u16 beacon_interval, u8 dtim_cnt)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    int retval = 0;
    struct ssv6xxx_sw_beacon_params *ptr;

    if (beacon_interval==0)
        beacon_interval = 100;
    
    skb = ssv_skb_alloc(sh, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params));
    if (skb == NULL) {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params));
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params));
    host_cmd->c_type = HOST_CMD;
    host_cmd->sub_h_cmd = SSV6XXX_SOFT_BEACON_SET_INTERVAL;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SOFT_BEACON;
    host_cmd->len = skb->len;
    ptr = (struct ssv6xxx_sw_beacon_params *)host_cmd->un.dat8;
    ptr->interval = beacon_interval;
    ptr->dtim_cnt = dtim_cnt;
    
    retval = HCI_SEND_CMD(sh, skb);
    if (retval) {
        printk("%s(): Fail to send soft beacon cmd\n", __FUNCTION__);
    }
}

void ssv6xxx_beacon_set(struct ssv_softc *sc, struct sk_buff *beacon_skb, int dtim_offset)
{
    struct sk_buff *skb;
    struct cfg_host_cmd *host_cmd;
    int retval = 0;
    struct ssv6xxx_sw_beacon_params *ptr;

    skb = ssv_skb_alloc_ex(sc->sh, 
            HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params) + beacon_skb->len, GFP_ATOMIC);
    if (skb == NULL) {
        printk("%s:_skb_alloc fail!!!\n", __func__);
        return;
    }

    skb_put(skb, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params) + beacon_skb->len);
    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, HOST_CMD_HDR_LEN + sizeof(struct ssv6xxx_sw_beacon_params) + beacon_skb->len);
    host_cmd->c_type = HOST_CMD;
    host_cmd->sub_h_cmd = SSV6XXX_SOFT_BEACON_FILL_CONTEXT;
    host_cmd->h_cmd = (u8)SSV6XXX_HOST_CMD_SOFT_BEACON;
    host_cmd->len = skb->len;
    ptr = (struct ssv6xxx_sw_beacon_params *)host_cmd->un.dat8;
    ptr->dtim_offset = dtim_offset;
    ptr->dtim_cnt = sc->beacon_dtim_cnt;
    ptr->interval = (0 == sc->beacon_interval) ? 100 : sc->beacon_interval; 
    ptr->beacon_len = beacon_skb->len;
    memcpy((u8 *)ptr->beacon_context, (u8 *)beacon_skb->data, beacon_skb->len);
    
    retval = HCI_SEND_CMD(sc->sh, skb);
    if (retval) {
        printk("%s(): Fail to send soft beacon cmd\n", __FUNCTION__);
    }
}

//need to stop beacon firstly.
void ssv6xxx_beacon_release(struct ssv_softc *sc)
{
	printk("[A] ssv6xxx_beacon_release Enter\n");
    ssv6x5x_beacon_enable(sc->sh, false);
}

void ssv6xxx_beacon_change(struct ssv_softc *sc, struct ieee80211_hw *hw, struct ieee80211_vif *vif, bool aid0_bit_set)
{
	//struct ssv_hw *sh = sc->sh;
	struct sk_buff *skb;
    struct sk_buff *old_skb = NULL;
	u16 tim_offset, tim_length;
    
	
//	printk("[A] ssv6xxx_beacon_change\n");

    if(sc == NULL || hw == NULL || vif == NULL ){
        printk("[Error]........ssv6xxx_beacon_change input error\n");
		return;
    }

    do{
    	skb = ieee80211_beacon_get_tim(hw, vif,
    			&tim_offset, &tim_length);

        if(skb == NULL){
            printk("[Error]........skb is NULL\n");
            break;
        }

    	if (tim_offset && tim_length >= 6) {
    	/* Ignore DTIM count from mac80211:
    		 * firmware handles DTIM internally.
    		 */
    		skb->data[tim_offset + 2] = 0;
            // for sw beacon, set dtim period = 1
            if (!(sc->sh->cfg.hw_caps & SSV6200_HW_CAP_BEACON))
    		    skb->data[tim_offset + 3] = 1;

    		/* Set/reset aid0 bit */
    		if (aid0_bit_set)
    			skb->data[tim_offset + 4] |= 1;
    		else
    			skb->data[tim_offset + 4] &= ~1;
    	}
    	
    	dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_BEACON,
	        "[A] beacon len [%d] tim_offset[%d]\n", skb->len, tim_offset);

    	SSV_FILL_BEACON_TX_DESC(sc, skb);
	
    	dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_BEACON,
	        "[A] beacon len [%d] tim_offset[%d]\n", skb->len, tim_offset);

        if(sc->beacon_buf)
        {
            //struct ssv6200_tx_desc *tx_desc = (struct ssv6200_tx_desc *)sc->beacon_buf->data;                            
            /* 
                    * Compare if content is the same ( not include fcs)
                    */           
            if(memcmp(sc->beacon_buf->data, skb->data, (skb->len-FCS_LEN)) == 0){
                /* no need set new beacon to register*/
                old_skb = skb;
                break;
            }
            else{                
                 old_skb = sc->beacon_buf;
                 sc->beacon_buf = skb;
            }        
        }
        else{
            sc->beacon_buf = skb;
        }
        
    //for debug 
    //	{
    //		int i;
    //		u8 *ptr = &skb->data[tim_offset];
    //		printk("=================DTIM===================\n");	
    //		for(i=0;i<tim_length;i++)
    //			printk("%08x ", *(ptr+i));
    //		printk("\n=======================================\n");
    //	}
    //	

    		
        //ASIC need to know the position of DTIM count. therefore add 2 bytes more.
    	tim_offset+=2;
        
        // set beacon
        sc->beacon_dtim_cnt = vif->bss_conf.dtim_period-1;
    	ssv6xxx_beacon_set(sc, skb, tim_offset);
    }while(0);
    
    if (old_skb)
	    dev_kfree_skb_any(old_skb);

}

void ssv6200_set_tim_work(struct ssv_softc *sc)
{
#ifdef BROADCAST_DEBUG
	printk("%s() enter\n", __FUNCTION__);
#endif    
	ssv6xxx_beacon_change(sc, sc->hw, sc->ap_vif, sc->aid0_bit_set);
#ifdef BROADCAST_DEBUG
    printk("%s() leave\n", __FUNCTION__);
#endif    
}


int ssv6200_bcast_queue_len(struct ssv6xxx_bcast_txq *bcast_txq)
{
	u32 len;
    unsigned long flags;

    spin_lock_irqsave(&bcast_txq->txq_lock, flags);
    len = bcast_txq->cur_qsize;
    spin_unlock_irqrestore(&bcast_txq->txq_lock, flags);

    return len;
}



struct sk_buff* ssv6200_bcast_dequeue(struct ssv6xxx_bcast_txq *bcast_txq, u8 *remain_len)
{
    struct sk_buff *skb = NULL;
    unsigned long flags;
	
    spin_lock_irqsave(&bcast_txq->txq_lock, flags);
    if(bcast_txq->cur_qsize){
        bcast_txq->cur_qsize --;
        if(remain_len)
            *remain_len = bcast_txq->cur_qsize;    
        skb = __skb_dequeue(&bcast_txq->qhead);
    }
    spin_unlock_irqrestore(&bcast_txq->txq_lock, flags);  

    return skb;
}


int ssv6200_bcast_enqueue(struct ssv_softc *sc, struct ssv6xxx_bcast_txq *bcast_txq, 
                                                        struct sk_buff *skb)
{
    unsigned long flags;

    spin_lock_irqsave(&bcast_txq->txq_lock, flags);    
        
	/* Release oldest frame. */
    if (bcast_txq->cur_qsize >= 
                    SSV6200_MAX_BCAST_QUEUE_LEN){
        struct sk_buff *old_skb;
            
		old_skb = __skb_dequeue(&bcast_txq->qhead);
        bcast_txq->cur_qsize --;
        //dev_kfree_skb_any(old_skb);
        ssv6xxx_txbuf_free_skb(old_skb, (void*)sc);
        
        printk("[B] ssv6200_bcast_enqueue - remove oldest queue\n");
    }


    __skb_queue_tail(&bcast_txq->qhead, skb);
    bcast_txq->cur_qsize ++;
    
    spin_unlock_irqrestore(&bcast_txq->txq_lock, flags);
    
    return bcast_txq->cur_qsize;    
}

void ssv6200_bcast_flush(struct ssv_softc *sc, struct ssv6xxx_bcast_txq *bcast_txq)
{
 
    struct sk_buff *skb;
    unsigned long flags;
 
#ifdef  BCAST_DEBUG 
    printk("ssv6200_bcast_flush\n");
#endif 
    spin_lock_irqsave(&bcast_txq->txq_lock, flags);
    while(bcast_txq->cur_qsize > 0) {
        skb = __skb_dequeue(&bcast_txq->qhead);
        bcast_txq->cur_qsize --;
        //dev_kfree_skb_any(skb);
        ssv6xxx_txbuf_free_skb(skb, (void*)sc);
    }
    spin_unlock_irqrestore(&bcast_txq->txq_lock, flags);    
}



static int queue_block_cnt = 0;

/* If buffer any mcast frame, send it. */
//void ssv6200_bcast_timer(unsigned long arg)	
void ssv6200_bcast_tx_work(struct work_struct *work)
{
    struct ssv_softc *sc =
            container_of(work, struct ssv_softc, bcast_tx_work.work);
//	struct ssv_softc *sc = (struct ssv_softc *)arg;


#if 1
    struct sk_buff* skb;
    int i;
    u8 remain_size;
#endif    
    unsigned long flags;
    bool needtimer = true;
    long tmo = sc->bcast_interval;							//Trigger after DTIM

    spin_lock_irqsave(&sc->ps_state_lock, flags);

    do{
#ifdef  BCAST_DEBUG
        printk("[B] bcast_timer: hw_mng_used[%d] HCI_TXQ_EMPTY[%d] bcast_queue_len[%d].....................\n",
               sc->hw_mng_used, HCI_TXQ_EMPTY(sc->sh, 8), ssv6200_bcast_queue_len(&sc->bcast_txq));
#endif
        //HCI_PAUSE_HWSWQ(sc->sh, (TXQ_MGMT));
        
        /* if there is any frame in low layer, stop sending at this time */
        if(sc->hw_mng_used != 0 || 
            false == HCI_TXQ_EMPTY(sc->sh, 8)){
#ifdef  BCAST_DEBUG            
            printk("HW queue still have frames insdide. skip this one hw_mng_used[%d] bEmptyTXQ4[%d]\n", 
                sc->hw_mng_used, HCI_TXQ_EMPTY(sc->sh, 8));
#endif
            queue_block_cnt++;
            /* does hw queue have problem??? flush bcast queue to prevent tx_work drop in an inifate loop*/
            if(queue_block_cnt>5){
                queue_block_cnt = 0;
                ssv6200_bcast_flush(sc, &sc->bcast_txq);
                needtimer = false;
            }
            
            break;
        }

        queue_block_cnt = 0;
       
        for(i=0;i<SSV6200_ID_MANAGER_QUEUE;i++){
		        
            skb = ssv6200_bcast_dequeue(&sc->bcast_txq, &remain_size);            
            if(!skb){
                needtimer = false;
                break;
            }

            if( (0 != remain_size) &&
                (SSV6200_ID_MANAGER_QUEUE-1) != i ){
                /* tell station there are more broadcast frame sending... */
                struct ieee80211_hdr *hdr;
                //struct ssv6200_tx_desc *tx_desc = (struct ssv6200_tx_desc *)skb->data;                
                //hdr = (struct ieee80211_hdr *) ((u8*)tx_desc+tx_desc->hdr_offset);
                //tx_desc->hdr_offset = TXPB_OFFSET for always
                hdr = (struct ieee80211_hdr *) ((u8*)skb->data+TXPB_OFFSET);                      
                hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_MOREDATA);
            }

#ifdef  BCAST_DEBUG
            printk("[B] bcast_timer:tx remain_size[%d] i[%d]\n", remain_size, i);
#endif
			spin_unlock_irqrestore(&sc->ps_state_lock, flags);

            if(HCI_SEND(sc->sh, skb, SSV_SW_TXQ_ID_MNG, false)<0){
                printk("bcast_timer send fail!!!!!!! \n");
                //dev_kfree_skb_any(skb);
                ssv6xxx_txbuf_free_skb(skb, (void*)sc);
                BUG_ON(1);                
            }
			spin_lock_irqsave(&sc->ps_state_lock, flags);

        }     
    }while(0);
        
    if(needtimer){
#ifdef  BCAST_DEBUG        
        printk("[B] bcast_timer:need more timer to tx bcast frame time[%d]\n", sc->bcast_interval);
#endif
        queue_delayed_work(sc->config_wq,
				   &sc->bcast_tx_work,
				   tmo);
     
        //mod_timer(&sc->bcast_timeout, jiffies + tmo);
    }
    else{
#ifdef  BCAST_DEBUG        
       printk("[B] bcast_timer: ssv6200_bcast_stop\n");
#endif       
       ssv6200_bcast_stop(sc);
    }

    spin_unlock_irqrestore(&sc->ps_state_lock, flags);

#ifdef  BCAST_DEBUG
    printk("[B] bcast_timer: leave.....................\n");
#endif
}






/**
  *1. Update DTIM    
  *2. Send Broadcast frame after DTIM 
  */
void ssv6200_bcast_start_work(struct work_struct *work)
{
	struct ssv_softc *sc =
		container_of(work, struct ssv_softc, bcast_start_work);

#ifdef  BCAST_DEBUG		
    printk("[B] ssv6200_bcast_start_work==\n");
#endif
    
	/* Every DTIM launch timer to send b-frames*/
    sc->bcast_interval = (sc->beacon_dtim_cnt+1) *
			(sc->beacon_interval + 20) * HZ / 1000;							//Trigger after DTIM;
    
	if (!sc->aid0_bit_set) {
		sc->aid0_bit_set = true;

        /* 1. Update DTIM  */
		ssv6xxx_beacon_change(sc, sc->hw, 
                        sc->ap_vif, sc->aid0_bit_set);

        /* 2. Send Broadcast frame after DTIM  */
        //mod_timer(&sc->bcast_timeout, jiffies + sc->bcast_interval);
        queue_delayed_work(sc->config_wq,
				   &sc->bcast_tx_work,
				   sc->bcast_interval);

#ifdef  BCAST_DEBUG
        printk("[B] bcast_start_work: Modify timer to DTIM[%d]ms==\n", 
               (sc->beacon_dtim_cnt+1)*(sc->beacon_interval + 20));
#endif        
	}
    
    
}

/**
  *1. Update DTIM.
  *2. Remove timer to send beacon.
  */  
void ssv6200_bcast_stop_work(struct work_struct *work)
{
	struct ssv_softc *sc =
		container_of(work, struct ssv_softc, bcast_stop_work.work);
    long tmo = HZ / 100;//10ms

#ifdef  BCAST_DEBUG
    printk("[B] ssv6200_bcast_stop_work\n");
#endif

    /* expired every 10ms*/
    //sc->bcast_interval = HZ / 10;

	if (sc->aid0_bit_set) {
        if(0== ssv6200_bcast_queue_len(&sc->bcast_txq)){

            /* 1. Remove timer to send beacon. */
//    		del_timer_sync(&sc->bcast_timeout);
            cancel_delayed_work_sync(&sc->bcast_tx_work);
            
    		sc->aid0_bit_set = false;               

            /* 2. Update DTIM. */
    		ssv6xxx_beacon_change(sc, sc->hw, 
                            sc->ap_vif, sc->aid0_bit_set);
#ifdef  BCAST_DEBUG
            printk("remove group bit in DTIM\n");
#endif            
        }
        else{
#ifdef  BCAST_DEBUG            
            printk("bcast_stop_work: bcast queue still have data. just modify timer to 10ms\n");
#endif            
            //mod_timer(&sc->bcast_timeout, jiffies + sc->bcast_interval);
            queue_delayed_work(sc->config_wq,
				   &sc->bcast_tx_work,
				   tmo);
        }
	}
}

void ssv6200_bcast_stop(struct ssv_softc *sc)
{
    //cancel_work_sync(&sc->bcast_start_work);
    queue_delayed_work(sc->config_wq,
						   &sc->bcast_stop_work, sc->beacon_interval*HZ/1024);
}

void ssv6200_bcast_start(struct ssv_softc *sc)
{
    //cancel_delayed_work_sync(&sc->bcast_stop_work);
    queue_work(sc->config_wq, &sc->bcast_start_work);
}


void ssv6200_release_bcast_frame_res(struct ssv_softc *sc, struct ieee80211_vif *vif)
{
    unsigned long flags;
    struct ssv_vif_priv_data *priv_vif = (struct ssv_vif_priv_data *)vif->drv_priv;
    
    //?? no need do this ??
    spin_lock_irqsave(&sc->ps_state_lock, flags);
    /*clear asleep mask to deny new frame insert*/
    priv_vif->sta_asleep_mask = 0;
    spin_unlock_irqrestore(&sc->ps_state_lock, flags);
     

    cancel_work_sync(&sc->bcast_start_work);
    cancel_delayed_work_sync(&sc->bcast_stop_work);
    ssv6200_bcast_flush(sc, &sc->bcast_txq);
    cancel_delayed_work_sync(&sc->bcast_tx_work);
}


//Beacon related end
//----------------------------------------------------------------------------

