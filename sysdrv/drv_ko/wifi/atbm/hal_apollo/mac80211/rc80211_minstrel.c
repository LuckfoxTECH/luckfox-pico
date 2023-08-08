/*
 * Copyright (C) 2008 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Based on minstrel.c:
 *   Copyright (C) 2005-2007 Derek Smithies <derek@indranet.co.nz>
 *   Sponsored by Indranet Technologies Ltd
 *
 * Based on sample.c:
 *   Copyright (c) 2005 John Bicket
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer,
 *      without modification.
 *   2. Redistributions in binary form must reproduce at minimum a disclaimer
 *      similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *      redistribution must be conditioned upon including a substantially
 *      similar Disclaimer requirement for further binary redistribution.
 *   3. Neither the names of the above-listed copyright holders nor the names
 *      of any contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   Alternatively, this software may be distributed under the terms of the
 *   GNU General Public License ("GPL") version 2 as published by the Free
 *   Software Foundation.
 *
 *   NO WARRANTY
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 *   AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 *   THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 *   OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 *   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *   THE POSSIBILITY OF SUCH DAMAGES.
 */
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/debugfs.h>
#include <linux/random.h>
#include <linux/ieee80211.h>
#include <linux/slab.h>
#include <net/atbm_mac80211.h>
#include "rate.h"
#include "rc80211_minstrel.h"
#define SAMPLE_COLUMNS	10
#define SAMPLE_TBL(_mi, _idx, _col) \
	_mi->sample_table[(_idx * SAMPLE_COLUMNS) + _col]

/* convert mac80211 rate index to local array index */
	static inline int
rix_to_ndx(struct minstrel_sta_info *mi, int rix)
{
	int i = rix;
	for (i = rix; i >= 0; i--)
		if (mi->r[i].rix == rix)
			break;
	return i;
}
#ifdef MINSTREL_RSSI_USED
static void

minstrel_update_stats_calibrate_rate_policy(struct minstrel_priv *mp,
		struct minstrel_sta_info *mi)

{
	int i;

	if(mi->mean_rssi < -80)

	{

		//int table_flag; //0:default table, 1:table1

		//int table_count;

		mi->table_flag = 0;

		mi->table_count = 0;

		//using  default table

	}

	else{     // >-80

		int use_table2_flag = 0;

		for (i = 4; i < mi->n_rates; i++) {

			struct minstrel_rate *mr = &mi->r[i];

			if(mr->att_hist1 > 20)  //packet attempt >20

			{


				if((mr->success_prob1 > 35) && (i == 4))

				{
					use_table2_flag = 1;

					break;

				}
				else if((mr->success_prob1 > 28) && (i == 5))
				{
					use_table2_flag = 1;
					break;
				}
				else if((mr->success_prob1 > 20) && (i == 6)) 

				{



					use_table2_flag = 1;

					break;

				}

				else if ((mr->success_prob1 > 15) && (i > 5)) 

				{



					use_table2_flag = 1;

					break;



				}

				else

				{

					mi->table_flag = 0; 

					mi->table_count = 0;



				}                   



			}

			else
			{



			}

		}

		if (use_table2_flag == 1)

		{ // table2

			mi->table_flag = 1; 

			mi->table_count = 0;

		}

		else

		{   



			if (mi->mean_rssi > -65)

			{

				mi->table_count++;

			}

			else

			{

				mi->table_count = mi->table_count + 1;

			}



			//for table flag

			if( mi->table_count > 4)

			{

				//default table 2

				mi->table_flag = 0; 

				mi->table_count = 0;

			}

			else

			{// table hold same



			}



		}//use_table2_flag == 0

	}// >-80
}



	static void
minstrel_update_stats_per_s(struct minstrel_priv *mp, struct minstrel_sta_info *mi)
{
	int i;

	u32 a,b,success_prob = 0;
	mi->stats_update1 = jiffies;
	//printk("\n");
	for (i = 0; i < mi->n_rates; i++) {
		struct minstrel_rate *mr = &mi->r[i];
		a = mr->succ_hist1;
		b= mr->att_hist1;
		if(b != 0){
			success_prob = (a * 100) / b;
		}else{
			success_prob = 0;
		}
		mr->success_prob1 = success_prob;
#ifdef MINSTREL_PHY_DEBUG_PRINT 
		atbm_printk_always("Rate:%d success_hist1:%llu, att_hist1:%llu, success_prob:%u, cur_tp:%u, prob:%u\n", mr->bitrate/2, mr->succ_hist1, mr->att_hist1, success_prob,mr->cur_tp, mr->probability/180);
#endif		//	mr->succ_hist1 = 0;
		//	mr->att_hist1 = 0;
	}    
	if(mi->rssi_count!= 0)
		mi->mean_rssi = (int) (mi->total_rssi / mi->rssi_count);

	minstrel_update_stats_calibrate_rate_policy(mp, mi);
#ifdef MINSTREL_PHY_DEBUG_PRINT 
	atbm_printk_always("\n mi->rssi_count:%d,  mi->mean_rssi%d  mi->max_rssi :%d, mi->min_rssi:%d tableflag:%d count:%d\n",
			mi->rssi_count, mi->mean_rssi, mi->max_rssi, mi->min_rssi, mi->table_flag, mi->table_count);
	atbm_printk_always("high_not_sample_cnt:%d, high_sample_cnt:%d, low_sample_cnt:%d, not_sample_cnt:%d\n", mi->high_not_sample_cnt, mi->high_sample_cnt,
			mi->low_sample_cnt, mi->not_sample_cnt);
#endif
	for (i = 0; i < mi->n_rates; i++) {
		struct minstrel_rate *mr = &mi->r[i];
		mr->succ_hist1 = 0;
		mr->att_hist1 = 0;

	}
	mi->rssi_count = 0;
	mi->total_rssi = 0;
	mi->mean_rssi = 0;
	mi->max_rssi  = -128;

	mi->min_rssi  = 127;	
}
#endif
	static void
minstrel_update_stats(struct minstrel_priv *mp, struct minstrel_sta_info *mi)
{
	u32 max_tp = 0, index_max_tp = 0, index_max_tp2 = 0;
	u32 max_prob = 0, index_max_prob = 0;
	u32 usecs;
	u32 p = 0;
	int i;

	mi->stats_update = jiffies;
	for (i = 0; i < mi->n_rates; i++) {
		struct minstrel_rate *mr = &mi->r[i];

		usecs = mr->perfect_tx_time;
		if (!usecs)
			usecs = 1000000;

		/* To avoid rounding issues, probabilities scale from 0 (0%)
		 * to 18000 (100%) */
		/*		if (mr->attempts) {
				p = (mr->success * 18000) / mr->attempts;
				mr->succ_hist += mr->success;
				mr->att_hist += mr->attempts;
				mr->succ_hist1 += mr->success;  //update every interval and clear every second
				mr->att_hist1 += mr->attempts;
				mr->cur_prob = p;
				p = ((p * (100 - mp->ewma_level)) + (mr->probability *
				mp->ewma_level)) / 100;
				mr->probability = p;
				mr->cur_tp = p * (1000000 / usecs);
				}*/
#ifdef MINSTREL_RSSI_USED		
		if (mr->attempts) {

			//p = (mr->success * 18000) / mr->attempts;

			mr->succ_hist += mr->success;
			mr->att_hist += mr->attempts;	

			mr->succ_hist1 += mr->success;
			mr->att_hist1 += mr->attempts;


			mr->cur_prob = p;

			if (mr->attempts > 200)
			{
				mr->succ_hist2 = (mr->succ_hist2+mr->success*700+4)/8;
				mr->att_hist2 = (mr->att_hist2+mr->attempts*700+4)/8;
			}
			else if (mr->attempts > 100)
			{ 

				mr->succ_hist2 = (mr->succ_hist2+mr->success*300+2)/4;

				mr->att_hist2 = (mr->att_hist2 + mr->attempts*300+2)/4;

			}



			else if (mr->attempts > 50)

			{ 

				mr->succ_hist2 = (mr->succ_hist2+mr->success*100+1)/2;

				mr->att_hist2 = (mr->att_hist2 + mr->attempts*100+1)/2;

			}

			else if (mr->attempts > 25)

			{

				mr->succ_hist2 = (mr->succ_hist2*3+mr->success*100+2)/4;

				mr->att_hist2 = (mr->att_hist2*3 + mr->attempts*100+2)/4;

			}
			else //if(mr->attempts > 25)
			{
				mr->succ_hist2 = (mr->succ_hist2*7+mr->success*100+4)/8;
				mr->att_hist2 = (mr->att_hist2*7 + mr->attempts*100+4)/8;
			}

			if(mr->att_hist2 != 0)
			{
				p = mr->succ_hist2*18000/mr->att_hist2;
				mr->probability = p;
			}
			mr->cur_tp = mr->probability * (1000000 / usecs);
		}
#else
		if (mr->attempts) {
			p = (mr->success * 18000) / mr->attempts;
			mr->succ_hist += mr->success;
			mr->att_hist += mr->attempts;
			mr->cur_prob = p;
			p = ((p * (100 - mp->ewma_level)) + (mr->probability *
						mp->ewma_level)) / 100;
			mr->probability = p;
			mr->cur_tp = p * (1000000 / usecs);		
		}
#endif


		mr->last_success = mr->success;
		mr->last_attempts = mr->attempts;
		mr->success = 0;
		mr->attempts = 0;

		/* Sample less often below the 10% chance of success.
		 * Sample less often above the 95% chance of success. */
#ifdef MINSTREL_RSSI_USED
			if (mr->probability < 2700)
#else
			if ((mr->probability > 17100) || (mr->probability < 1800))
#endif				
			{
				mr->adjusted_retry_count = mr->retry_count >> 1;
				if (mr->adjusted_retry_count > 2)
					mr->adjusted_retry_count = 2;
				mr->sample_limit = 4;
			} else {
				mr->sample_limit = -1;
#ifdef MINSTREL_RSSI_USED
			if(mr->probability<3600)
			mr->adjusted_retry_count = 3;
			else	
			mr->adjusted_retry_count = mr->retry_count;
#else
				mr->adjusted_retry_count = mr->retry_count;
#endif
			}
			if (!mr->adjusted_retry_count)
				mr->adjusted_retry_count = 2;
		}

		for (i = 0; i < mi->n_rates; i++) {
			struct minstrel_rate *mr = &mi->r[i];
			if (max_tp < mr->cur_tp) {
				index_max_tp = i;
				max_tp = mr->cur_tp;
			}
			if (max_prob < mr->probability) {
				index_max_prob = i;
				max_prob = mr->probability;
			}
		}

		max_tp = 0;
		for (i = 0; i < mi->n_rates; i++) {
			struct minstrel_rate *mr = &mi->r[i];

			if (i == index_max_tp)
				continue;

			if (max_tp < mr->cur_tp) {
				index_max_tp2 = i;
				max_tp = mr->cur_tp;
			}
		}
		mi->max_tp_rate = index_max_tp;

#ifdef MINSTREL_RSSI_USED
		if ((index_max_tp <= 3) && (mi->table_flag == 1))
		{	
			index_max_tp = max(index_max_tp + 2, (u32)4);
			mi->max_tp_rate = index_max_tp;
		}
		if ((index_max_tp2 <= 3) && (mi->table_flag == 1))
		{
			if(index_max_tp >5)
			{
				mi->max_tp_rate2 = max((u32)4, min(index_max_tp2, index_max_tp -1));
			}
			else
			{
				mi->max_tp_rate2= 4; 
			}
		}
		else
		{
			if (index_max_tp > 2)
				mi->max_tp_rate2 = min(max(index_max_tp2, (u32)2), index_max_tp - 1);
			else if(index_max_tp >= 1)
				mi->max_tp_rate2 = max(index_max_tp, index_max_tp2);
			else 
				mi->max_tp_rate2 = index_max_tp2;
		}


		if ((index_max_prob <= 3) && (mi->table_flag == 1)){
			if((index_max_tp > 5)&&(mi->max_tp_rate2 > 5))
			{
				mi->max_prob_rate = max((u32)4, min(index_max_tp - 1, mi->max_tp_rate2 - 1));
			}else
			{
				mi->max_prob_rate = 4;
			}
		}
		else{
			if (mi->max_tp_rate2>=2)
				mi->max_prob_rate = min(max((u32)2, index_max_prob), mi->max_tp_rate2 - 1);
			else if (mi->max_tp_rate2 == 1)
				mi->max_prob_rate = max((u32)1, index_max_prob);
			else 
				mi->max_prob_rate = index_max_prob;
/*
			if ((mi->max_tp_rate2 > 0) &&(mi->max_tp_rate>0)) 
			{
				mi->max_prob_rate = min(min(mi->max_tp_rate - 1, mi->max_tp_rate2 - 1), max(index_max_prob, 2));
				
			}
			else
			mi->max_prob_rate = index_max_prob;
*/		}
#else
		mi->max_tp_rate2 = index_max_tp2;
		mi->max_prob_rate = index_max_prob;

#endif
	}

	static void
		minstrel_tx_status(void *priv, struct ieee80211_supported_band *sband,
				struct ieee80211_sta *sta, void *priv_sta,
				struct sk_buff *skb)
		{
			struct minstrel_sta_info *mi = priv_sta;
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
			struct ieee80211_tx_rate *ar = info->status.rates;
			int i, ndx;
			int success;

			success = !!(info->flags & IEEE80211_TX_STAT_ACK);

			for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
				if (ar[i].idx < 0)
					break;

				ndx = rix_to_ndx(mi, ar[i].idx);
				if (ndx < 0)
					continue;

				mi->r[ndx].attempts += ar[i].count;
				//        if(ar[i].count > 1 )
				//  printk("ndx:%d attempts:%d  ar[i].count::%d \n", ndx, mi->r[ndx].attempts, ar[i].count);

				if ((i != IEEE80211_TX_MAX_RATES - 1) && (ar[i + 1].idx < 0))
					mi->r[ndx].success += success;
			}

			if ((info->flags & IEEE80211_TX_CTL_RATE_CTRL_PROBE) && (i >= 0))
				mi->sample_count++;

			if (mi->sample_deferred > 0)
				mi->sample_deferred--;
		}


	static inline unsigned int
		minstrel_get_retry_count(struct minstrel_rate *mr,
				struct ieee80211_tx_info *info)
		{
			unsigned int retry = mr->adjusted_retry_count;

			if (info->control.rates[0].flags & IEEE80211_TX_RC_USE_RTS_CTS)
				retry = max(2U, min(mr->retry_count_rtscts, retry));
			else if (info->control.rates[0].flags & IEEE80211_TX_RC_USE_CTS_PROTECT)
				retry = max(2U, min(mr->retry_count_cts, retry));
			return retry;
		}


	static int
		minstrel_get_next_sample(struct minstrel_sta_info *mi)
		{
			unsigned int sample_ndx;
			sample_ndx = SAMPLE_TBL(mi, mi->sample_idx, mi->sample_column);
			mi->sample_idx++;
			if ((int) mi->sample_idx > (mi->n_rates - 2)) {
				mi->sample_idx = 0;
				mi->sample_column++;
				if (mi->sample_column >= SAMPLE_COLUMNS)
					mi->sample_column = 0;
			}
			return sample_ndx;
		}

	static void
		minstrel_get_rate(void *priv, struct ieee80211_sta *sta,
				void *priv_sta, struct ieee80211_tx_rate_control *txrc)
		{
			struct sk_buff *skb = txrc->skb;
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
			struct minstrel_sta_info *mi = priv_sta;
			struct minstrel_priv *mp = priv;
			struct ieee80211_tx_rate *ar = info->control.rates;
			unsigned int ndx, sample_ndx = 0;
			bool mrr;
			bool sample_slower = false;
			bool sample = false;
			int i, delta;
			int mrr_ndx[3];
			int sample_rate;

			if (rate_control_send_low(sta, priv_sta, txrc))
				return;

			mrr = mp->has_mrr && !txrc->rts && !txrc->bss_conf->use_cts_prot;

			if (time_after(jiffies, mi->stats_update + (mp->update_interval *
							HZ) / 1000))
				minstrel_update_stats(mp, mi);

#ifdef MINSTREL_RSSI_USED
			if (time_after(jiffies, mi->stats_update1 + (mp->update_interval *
							HZ) / 100))
			{
				minstrel_update_stats_per_s(mp, mi);
				mi->initial_flag = 1;
			}
#endif

			ndx = mi->max_tp_rate;

			if (mrr)
				sample_rate = mp->lookaround_rate_mrr;
			else
				sample_rate = mp->lookaround_rate;

			mi->packet_count++;
			delta = (mi->packet_count * sample_rate / 100) -
				(mi->sample_count + mi->sample_deferred / 2);

			/* delta > 0: sampling required */
			if ((delta > 0) && (mrr || !mi->prev_sample)) {
				struct minstrel_rate *msr;
				if (mi->packet_count >= 10000) {
					mi->sample_deferred = 0;
					mi->sample_count = 0;
					mi->packet_count = 0;
				} else if (delta > mi->n_rates * 2) {
					/* With multi-rate retry, not every planned sample
					 * attempt actually gets used, due to the way the retry
					 * chain is set up - [max_tp,sample,prob,lowest] for
					 * sample_rate < max_tp.
					 *
					 * If there's too much sampling backlog and the link
					 * starts getting worse, minstrel would start bursting
					 * out lots of sampling frames, which would result
					 * in a large throughput loss. */
					mi->sample_count += (delta - mi->n_rates * 2);
				}

				sample_ndx = minstrel_get_next_sample(mi);
				msr = &mi->r[sample_ndx];
				sample = true;
				sample_slower = mrr && (msr->perfect_tx_time >
						mi->r[ndx].perfect_tx_time);

				if (!sample_slower) {
# ifdef MINSTREL_RSSI_USED
					int sample_max_rate;
					if (mi->max_tp_rate == 2)
						sample_max_rate = 6;
					else if (mi->max_tp_rate == 3)
						sample_max_rate = 7;
					else
						sample_max_rate = max((u32)7, mi->max_tp_rate + 2);
					
					if ((msr->success_prob1 < 5) && sample_ndx > sample_max_rate  && (mi->initial_flag == 1)) 
					{
#ifdef MINSTREL_PHY_DEBUG_PRINT
						mi->high_not_sample_cnt++;
#endif
						mi->sample_count++;
						sample = false;
					}
					else if((mi->max_tp_rate <= 1) && (sample_ndx <= 7) && (sample_ndx >= 2) && (mi->initial_flag == 1))
					{
#ifdef MINSTREL_PHY_DEBUG_PRINT
						mi->high_sample_cnt++;
#endif
						ndx = sample_ndx;
						mi->sample_count = mi->sample_count + 2;
					}
					else
					{

#endif
						if (msr->sample_limit != 0) {
							ndx = sample_ndx;
							mi->sample_count++;
#ifdef MINSTREL_PHY_DEBUG_PRINT
							mi->high_sample_cnt++;
#endif
							if (msr->sample_limit > 0)
								msr->sample_limit--;
						} else {
							sample = false;
#ifdef MINSTREL_PHY_DEBUG_PRINT
							mi->high_not_sample_cnt++;
#endif	
						}
# ifdef MINSTREL_RSSI_USED		
					}
#endif
				} else {
					/* Only use IEEE80211_TX_CTL_RATE_CTRL_PROBE to mark
					 * packets that have the sampling rate deferred to the
					 * second MRR stage. Increase the sample counter only
					 * if the deferred sample rate was actually used.
					 * Use the sample_deferred counter to make sure that
					 * the sampling is not done in large bursts */
					info->flags |= IEEE80211_TX_CTL_RATE_CTRL_PROBE;
					mi->sample_deferred++;
#ifdef MINSTREL_PHY_DEBUG_PRINT
					mi->low_sample_cnt++;
#endif
				}
			}
			else
			{
#ifdef MINSTREL_PHY_DEBUG_PRINT
				mi->not_sample_cnt++;
#endif
			}
			mi->prev_sample = sample;

			/* If we're not using MRR and the sampling rate already
			 * has a probability of >95%, we shouldn't be attempting
			 * to use it, as this only wastes precious airtime */
			if (!mrr && sample && (mi->r[ndx].probability > 17100))
				ndx = mi->max_tp_rate;

			ar[0].idx = mi->r[ndx].rix;
#ifdef MINSTREL_RSSI_USED			
			if (sample && (sample_slower==0))
				ar[0].count = 2;
			else
#endif
			{	
				ar[0].count = minstrel_get_retry_count(&mi->r[ndx], info);
#ifdef MINSTREL_RSSI_USED
				if (mi->max_tp_rate <= 1)
				ar[0].count = max((u8)4, ar[0].count);	
				else if ((mi->max_tp_rate <= 3 && mi->r[0].probability< 9000) && (mi->r[ndx].probability>= 2700))
				ar[0].count = max((u8)6,ar[0].count);
#endif	
				
			}		

			if (!mrr) {
				if (!sample)
				{
					ar[0].count = mp->max_retry;
				}
				ar[1].idx = mi->lowest_rix;
				ar[1].count = mp->max_retry;
				return;
			}

			/* MRR setup */
			if (sample) {
				if (sample_slower)
				{		mrr_ndx[0] = sample_ndx;
				}	
				else{
					mrr_ndx[0] = mi->max_tp_rate;
				}
			} else {
				mrr_ndx[0] = mi->max_tp_rate2;
			}
			mrr_ndx[1] = mi->max_prob_rate;
#ifdef MINSTREL_RSSI_USED
			if (mi->table_flag == 1)	
				mrr_ndx[2] = 4;
			else
#endif
				mrr_ndx[2] = 0;


			for (i = 1; i < 4; i++) {
				ar[i].idx = mi->r[mrr_ndx[i - 1]].rix;
				ar[i].count = mi->r[mrr_ndx[i - 1]].adjusted_retry_count;
				//if  ((mrr_ndx[i-1] <= 3 && mi->r[0].probability< 9000) && (mi->r[ndx].probablity>= 1800))

			}
			/*
			   if(mi->table_flag == 1)
			   {
			   ar[3].count = 20;
			   }*/
		}


	static void
		calc_rate_durations(enum ieee80211_band band,
				struct minstrel_rate *d,
				struct ieee80211_rate *rate)
		{
			int erp = !!(rate->flags & IEEE80211_RATE_ERP_G);

			d->perfect_tx_time = ieee80211_frame_duration(band, 1200,
					rate->bitrate, erp, 1);
			d->ack_time = ieee80211_frame_duration(band, 10,
					rate->bitrate, erp, 1);
		}

	static void
		init_sample_table(struct minstrel_sta_info *mi)
		{
			unsigned int i, col, new_idx;
			unsigned int n_srates = mi->n_rates - 1;
			u8 rnd[8];

			mi->sample_column = 0;
			mi->sample_idx = 0;
			memset(mi->sample_table, 0, SAMPLE_COLUMNS * mi->n_rates);

			for (col = 0; col < SAMPLE_COLUMNS; col++) {
				for (i = 0; i < n_srates; i++) {
					get_random_bytes(rnd, sizeof(rnd));
					new_idx = (i + rnd[i & 7]) % n_srates;

					while (SAMPLE_TBL(mi, new_idx, col) != 0)
						new_idx = (new_idx + 1) % n_srates;

					/* Don't sample the slowest rate (i.e. slowest base
					 * rate). We must presume that the slowest rate works
					 * fine, or else other management frames will also be
					 * failing and the link will break */
					SAMPLE_TBL(mi, new_idx, col) = i + 1;
				}
			}


#ifdef MINSTREL_SAMPLE_SKIP_LOW_RATE 
			for (col = 0; col < SAMPLE_COLUMNS; col++) {
				for (i = 0; i < n_srates; i++) {

					if(SAMPLE_TBL(mi, i, col)== 1)
					{
						SAMPLE_TBL(mi, i, col) = 6;  
					}
				}
			}
#endif
#ifdef MINSTREL_RSSI_USED
#ifdef MINSTREL_PHY_DEBUG_PRINT 
			atbm_printk_always("minstrel sample table:\n");
			for (col = 0; col < SAMPLE_COLUMNS; col++) {
				for (i = 0; i < n_srates; i++) {

					atbm_printk_always("%d ", SAMPLE_TBL(mi, i, col));  

				}
				atbm_printk_always("\n");
			}
#endif
#endif
		}

	static void
		minstrel_rate_init(void *priv, struct ieee80211_supported_band *sband,
				struct ieee80211_sta *sta, void *priv_sta)
		{
			struct minstrel_sta_info *mi = priv_sta;
			struct minstrel_priv *mp = priv;
			struct ieee80211_rate *ctl_rate;
			unsigned int i, n = 0;
			unsigned int t_slot = 9; /* FIXME: get real slot time */
			//	printk(KERN_ERR " minstrel 11g rate control init...\n");
			mi->lowest_rix = rate_lowest_index(sband, sta);
			ctl_rate = &sband->bitrates[mi->lowest_rix];
			mi->sp_ack_dur = ieee80211_frame_duration(sband->band, 10,
					ctl_rate->bitrate,
					!!(ctl_rate->flags & IEEE80211_RATE_ERP_G), 1);
#ifdef MINSTREL_PHY_DEBUG_PRINT
			atbm_printk_always("\n retry count: ");
#endif
				for (i = 0; i < sband->n_bitrates; i++) {
					struct minstrel_rate *mr = &mi->r[n];
					unsigned int tx_time = 0, tx_time_cts = 0, tx_time_rtscts = 0;
					unsigned int tx_time_single;
					unsigned int cw = mp->cw_min;

					if (!rate_supported(sta, sband->band, i))
						continue;
					n++;
					memset(mr, 0, sizeof(*mr));

					mr->rix = i;
					mr->bitrate = sband->bitrates[i].bitrate / 5;
					calc_rate_durations(sband->band, mr, &sband->bitrates[i]);

					/* calculate maximum number of retransmissions before
					 * fallback (based on maximum segment size) */
					mr->sample_limit = -1;
					mr->retry_count = 1;
					mr->retry_count_cts = 1;
					mr->retry_count_rtscts = 1;
					tx_time = mr->perfect_tx_time + mi->sp_ack_dur;
					do {
						/* add one retransmission */
						tx_time_single = mr->ack_time + mr->perfect_tx_time;

						/* contention window */
						tx_time_single += (t_slot * cw) >> 1;
						cw = min((cw << 1) | 1, mp->cw_max);

						tx_time += tx_time_single;
						tx_time_cts += tx_time_single + mi->sp_ack_dur;
						tx_time_rtscts += tx_time_single + 2 * mi->sp_ack_dur;
						if ((tx_time_cts < mp->segment_size) &&
								(mr->retry_count_cts < mp->max_retry))
							mr->retry_count_cts++;
						if ((tx_time_rtscts < mp->segment_size) &&
								(mr->retry_count_rtscts < mp->max_retry))
							mr->retry_count_rtscts++;
					} while ((tx_time < mp->segment_size) &&
							(++mr->retry_count < mp->max_retry));
					mr->adjusted_retry_count = mr->retry_count;
#ifdef MINSTREL_PHY_DEBUG_PRINT
					atbm_printk_always("%d, ", mr->retry_count);
					mr->retry_count = 4;
					mr->adjusted_retry_count = 4;
					//printk("%d, ",mr->retry_count);
#endif
				}
#ifdef MINSTREL_PHY_DEBUG_PRINT
			atbm_printk_always("\n");
#endif

				for (i = n; i < sband->n_bitrates; i++) {
					struct minstrel_rate *mr = &mi->r[i];
					mr->rix = -1;
				}

			mi->n_rates = n;
			mi->stats_update = jiffies;
#ifdef MINSTREL_RSSI_USED
			mi->stats_update1 = jiffies;
#endif
			init_sample_table(mi);
		}

	static void *
		minstrel_alloc_sta(void *priv, struct ieee80211_sta *sta, gfp_t gfp)
		{
			struct ieee80211_supported_band *sband;
			struct minstrel_sta_info *mi;
			struct minstrel_priv *mp = priv;
			struct ieee80211_hw *hw = mp->hw;
			int max_rates = 0;
			int i;

			mi = atbm_kzalloc(sizeof(struct minstrel_sta_info), gfp);
			if (!mi)
				return NULL;

			for (i = 0; i < IEEE80211_NUM_BANDS; i++) {
				sband = hw->wiphy->bands[i];
				if (sband && sband->n_bitrates > max_rates)
					max_rates = sband->n_bitrates;
			}

			mi->r = atbm_kzalloc(sizeof(struct minstrel_rate) * max_rates, gfp);
			if (!mi->r)
				goto error;

			mi->sample_table = atbm_kmalloc(SAMPLE_COLUMNS * max_rates, gfp);
			if (!mi->sample_table)
				goto error1;

			mi->stats_update = jiffies;
#ifdef MINSTREL_RSSI_USED
			mi->stats_update1 = jiffies;
			mi->rssi_count = 0;
			mi->total_rssi = 0;
			mi->mean_rssi = 0;
			mi->max_rssi  = -200;
			mi->min_rssi  = 100;
			mi->table_flag = 0;
			mi->table_count = 0;
#endif

			return mi;

error1:
			atbm_kfree(mi->r);
error:
			atbm_kfree(mi);
			return NULL;
		}

	static void
		minstrel_free_sta(void *priv, struct ieee80211_sta *sta, void *priv_sta)
		{
			struct minstrel_sta_info *mi = priv_sta;

			atbm_kfree(mi->sample_table);
			atbm_kfree(mi->r);
			atbm_kfree(mi);
		}

	static void *
		minstrel_alloc(struct ieee80211_hw *hw, struct dentry *debugfsdir)
		{
			struct minstrel_priv *mp;

			mp = atbm_kzalloc(sizeof(struct minstrel_priv), GFP_ATOMIC);
			if (!mp)
				return NULL;

			/* contention window settings
			 * Just an approximation. Using the per-queue values would complicate
			 * the calculations and is probably unnecessary */
			mp->cw_min = 15;
			mp->cw_max = 1023;

			/* number of packets (in %) to use for sampling other rates
			 * sample less often for non-mrr packets, because the overhead
			 * is much higher than with mrr */
			mp->lookaround_rate = 5;
			mp->lookaround_rate_mrr = 10;

			/* moving average weight for EWMA */
			mp->ewma_level = 75;

			/* maximum time that the hw is allowed to stay in one MRR segment */
			mp->segment_size = 6000;

			if (hw->max_rate_tries > 0)
				mp->max_retry = hw->max_rate_tries;
			else
				/* safe default, does not necessarily have to match hw properties */
				mp->max_retry = 7;

			if (hw->max_rates >= 4)
				mp->has_mrr = true;

			mp->hw = hw;
			mp->update_interval = 100;
#ifdef MINSTREL_RSSI_USED
			mp->update_interval = 300;
#endif
#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
			mp->fixed_rate_idx = (u32) -1;
			mp->dbg_fixed_rate = debugfs_create_u32("fixed_rate_idx",
					S_IRUGO | S_IWUGO, debugfsdir, &mp->fixed_rate_idx);
#endif

			return mp;
		}

	static void
		minstrel_free(void *priv)
		{
#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
			debugfs_remove(((struct minstrel_priv *)priv)->dbg_fixed_rate);
#endif
			atbm_kfree(priv);
		}

	struct rate_control_ops mac80211_minstrel = {
		.name = "minstrel",
		.tx_status = minstrel_tx_status,
		.get_rate = minstrel_get_rate,
		.rate_init = minstrel_rate_init,
		.alloc = minstrel_alloc,
		.free = minstrel_free,
		.alloc_sta = minstrel_alloc_sta,
		.free_sta = minstrel_free_sta,
#ifdef CONFIG_MAC80211_ATBM_DEBUGFS
		.add_sta_debugfs = minstrel_add_sta_debugfs,
		.remove_sta_debugfs = minstrel_remove_sta_debugfs,
#endif
	};

	int
		rc80211_minstrel_init(void)
		{
			return ieee80211_rate_control_register(&mac80211_minstrel);
		}

	void
		rc80211_minstrel_exit(void)
		{
			ieee80211_rate_control_unregister(&mac80211_minstrel);
		}

