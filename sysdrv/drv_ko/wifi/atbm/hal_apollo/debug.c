/*
 * mac80211 glue code for mac80211 altobeam APOLLO drivers
 * DebugFS code
 * *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 * Based on apollo code
 * Copyright (c) 2010, ST-Ericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifdef CONFIG_ATBM_APOLLO_DEBUG

/*Linux version 3.4.0 compilation*/
//#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0))
#include<linux/module.h>
//#endif
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/kobject.h>

#include "apollo.h"
#include "debug.h"

//#define DBG_EVENT_LOG
#include "dbg_event.h"
#ifdef CONFIG_WIRELESS_EXT
extern unsigned int atbm_get_tx_power(void);
#endif
extern int atbm_wifi_run_status_get(void);

/* join_status */
static const char * const atbm_debug_join_status[] = {
	"passive",
	"monitor",
	"station",
	"access point",
};

/* WSM_JOIN_PREAMBLE_... */
static const char * const atbm_debug_preamble[] = {
	"long",
	"short",
	"long on 1 and 2 Mbps",
};

static const char * const atbm_debug_fw_types[] = {
	"ETF",
	"WFM",
	"WSM",
	"HI test",
	"Platform test",
};

static const char * const atbm_debug_link_id[] = {
	"OFF",
	"REQ",
	"SOFT",
	"HARD",
};

static const char *atbm_debug_mode(int mode)
{
	switch (mode) {
	case NL80211_IFTYPE_UNSPECIFIED:
		return "unspecified";
	case NL80211_IFTYPE_MONITOR:
		return "monitor";
	case NL80211_IFTYPE_STATION:
		return "station";
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
		return "ad-hok";
#endif
	case NL80211_IFTYPE_MESH_POINT:
		return "mesh point";
	case NL80211_IFTYPE_AP:
		return "access point";
	case NL80211_IFTYPE_P2P_CLIENT:
		return "p2p client";
	case NL80211_IFTYPE_P2P_GO:
		return "p2p go";
	default:
		return "unsupported";
	}
}

static void atbm_queue_status_show(P_VDEBUG_SEQFILE seq,
				     struct atbm_queue *q)
{
	int i, if_id;
	VDEBUG_PRINTF(seq, "Queue       %d:\n", q->queue_id);
	VDEBUG_PRINTF(seq, "  capacity: %d\n", q->capacity);
	VDEBUG_PRINTF(seq, "  queued:   %d\n", q->num_queued);
	VDEBUG_PRINTF(seq, "  pending:  %d\n", q->num_pending);
	VDEBUG_PRINTF(seq, "  sent:     %d\n", q->num_sent);
#ifndef	ATBM_WIFI_QUEUE_LOCK_BUG
	VDEBUG_PRINTF(seq, "  locked:   %s\n", q->tx_locked_cnt ? "yes" : "no");
#else
	VDEBUG_PRINTF(seq, "  locked[0]:   %s\n", q->tx_locked_cnt[0] ? "yes" : "no");
	VDEBUG_PRINTF(seq, "  locked[1]:   %s\n", q->tx_locked_cnt[1] ? "yes" : "no");
#endif
#ifndef	ATBM_WIFI_QUEUE_LOCK_BUG
	VDEBUG_PRINTF(seq, "  overfull: %s\n", q->overfull ? "yes" : "no");
#else
	VDEBUG_PRINTF(seq, "  overfull[0]: %s\n", q->overfull[0] ? "yes" : "no");
	VDEBUG_PRINTF(seq, "  overfull[1]: %s\n", q->overfull[1] ? "yes" : "no");
#endif
	VDEBUG_PUTS(seq,   "  link map: 0-> ");
	for (if_id = 0; if_id < ATBM_WIFI_MAX_VIFS; if_id++) {
		for (i = 0; i < q->stats->map_capacity; ++i)
			VDEBUG_PRINTF(seq, "%.2d ", q->link_map_cache[if_id][i]);
		VDEBUG_PRINTF(seq, "<-%d\n", q->stats->map_capacity);
	}
}

static void atbm_debug_print_map(P_VDEBUG_SEQFILE seq,
				   struct atbm_vif *priv,
				   const char *label,
				   u32 map)
{
	int i;
	VDEBUG_PRINTF(seq, "%s0-> ", label);
	for (i = 0; i < priv->hw_priv->tx_queue_stats.map_capacity; ++i)
		VDEBUG_PRINTF(seq, "%s ", (map & BIT(i)) ? "**" : "..");
	VDEBUG_PRINTF(seq, "<-%d\n",
		   priv->hw_priv->tx_queue_stats.map_capacity - 1);
}

int atbm_ht_show_info(P_VDEBUG_SEQFILE seq, void *v)
{
	unsigned int txpower = 0;
	struct atbm_common *hw_priv = VDEBUG_PRIV(seq);

	VDEBUG_PUTS(seq,   "tx power info: \n");
#ifdef CONFIG_WIRELESS_EXT
	txpower = atbm_get_tx_power();
#endif
	if(txpower & BIT(0))
		VDEBUG_PUTS(seq,   "    20M high txpower\n");
	else
		VDEBUG_PUTS(seq,   "    20M normal txpower\n");


	if(txpower & BIT(1))
		VDEBUG_PUTS(seq,   "    40M high txpower\n");
	else
		VDEBUG_PUTS(seq,   "    40M normal txpower\n");

	VDEBUG_PUTS(seq,   "ht info: \n");
	if((hw_priv->channel_type == NL80211_CHAN_HT20)||(hw_priv->channel_type == NL80211_CHAN_NO_HT))
		VDEBUG_PUTS(seq,   "    20M\n");
	else
		VDEBUG_PUTS(seq,   "    40M\n");

	return 0;
}
int atbm_wifi_show_status(P_VDEBUG_SEQFILE seq, void *v)
{
	int status;
	//struct atbm_common *hw_priv = VDEBUG_PRIV(seq);

	VDEBUG_PUTS(seq,   "wifi run status(1: Exception  0: Normal): \n");

	status = atbm_wifi_run_status_get();
	if(status == 1)
		VDEBUG_PUTS(seq,   "    1\n");
	else
		VDEBUG_PUTS(seq,   "    0\n");

	return 0;
}

int atbm_status_show_common(P_VDEBUG_SEQFILE seq, void *v)
{
	int i;
	struct list_head *item;
	struct atbm_common *hw_priv = VDEBUG_PRIV(seq);
	struct atbm_debug_common *d = hw_priv->debug;
#ifdef CONFIG_ATBM_BA_STATUS
	int ba_cnt, ba_acc, ba_cnt_rx, ba_acc_rx, ba_avg = 0, ba_avg_rx = 0;
	bool ba_ena;

	spin_lock_bh(&hw_priv->ba_lock);
	ba_cnt = hw_priv->debug->ba_cnt;
	ba_acc = hw_priv->debug->ba_acc;
	ba_cnt_rx = hw_priv->debug->ba_cnt_rx;
	ba_acc_rx = hw_priv->debug->ba_acc_rx;
	ba_ena = hw_priv->ba_ena;
	if (ba_cnt)
		ba_avg = ba_acc / ba_cnt;
	if (ba_cnt_rx)
		ba_avg_rx = ba_acc_rx / ba_cnt_rx;
	spin_unlock_bh(&hw_priv->ba_lock);
#endif
	VDEBUG_PUTS(seq,   "apollo wifi Wireless LAN driver status\n");
	VDEBUG_PRINTF(seq, "Hardware:   %d.%d\n",
		hw_priv->wsm_caps.hardwareId,
		hw_priv->wsm_caps.hardwareSubId);
	VDEBUG_PRINTF(seq, "Firmware:   %s %d.%d\n",
		atbm_debug_fw_types[hw_priv->wsm_caps.firmwareType],
		hw_priv->wsm_caps.firmwareVersion,
		hw_priv->wsm_caps.firmwareBuildNumber);
	VDEBUG_PRINTF(seq, "FW API:     %d\n",
		hw_priv->wsm_caps.firmwareApiVer);
	VDEBUG_PRINTF(seq, "FW caps:    0x%.4X\n",
		hw_priv->wsm_caps.firmwareCap);
	if (hw_priv->channel)
		VDEBUG_PRINTF(seq, "Channel:    %d%s\n",
			channel_hw_value(hw_priv->channel),
			hw_priv->channel_switch_in_progress ?
			" (switching)" : "");
	VDEBUG_PRINTF(seq, "HT:         %s\n",
		atbm_is_ht(&hw_priv->ht_info) ? "on" : "off");
	if (atbm_is_ht(&hw_priv->ht_info)) {
		VDEBUG_PRINTF(seq, "Greenfield: %s\n",
			atbm_ht_greenfield(&hw_priv->ht_info) ? "yes" : "no");
		VDEBUG_PRINTF(seq, "AMPDU dens: %d\n",
			atbm_ht_ampdu_density(&hw_priv->ht_info));
	}
#ifndef CONFIG_RATE_HW_CONTROL
	spin_lock_bh(&hw_priv->tx_policy_cache.lock);
	i = 0;
	list_for_each(item, &hw_priv->tx_policy_cache.used)
		++i;
	spin_unlock_bh(&hw_priv->tx_policy_cache.lock);
#endif
	VDEBUG_PRINTF(seq, "RC in use:  %d\n", i);
#ifdef CONFIG_ATBM_BA_STATUS
	VDEBUG_PRINTF(seq, "BA stat:    %d, %d (%d)\n",
		ba_cnt, ba_acc, ba_avg);
	VDEBUG_PRINTF(seq, "BA RX stat:    %d, %d (%d)\n",
		ba_cnt_rx, ba_acc_rx, ba_avg_rx);
	VDEBUG_PRINTF(seq, "Block ACK:  %s\n", ba_ena ? "on" : "off");

	VDEBUG_PUTS(seq, "\n");
#endif
	for (i = 0; i < 4; ++i) {
		atbm_queue_status_show(seq, &hw_priv->tx_queue[i]);
		VDEBUG_PUTS(seq, "\n");
	}
	VDEBUG_PRINTF(seq, "TX burst:   %d\n",
		d->tx_burst);
	VDEBUG_PRINTF(seq, "RX burst:   %d\n",
		d->rx_burst);
	VDEBUG_PRINTF(seq, "TX miss:    %d\n",
		d->tx_cache_miss);
	VDEBUG_PRINTF(seq, "Long retr:  %d\n",
		hw_priv->long_frame_max_tx_count);
	VDEBUG_PRINTF(seq, "Short retr: %d\n",
		hw_priv->short_frame_max_tx_count);

	VDEBUG_PRINTF(seq, "BH status:  %s\n",
		atomic_read(&hw_priv->bh_term) ? "terminated" : "alive");
	VDEBUG_PRINTF(seq, "Pending RX: %d\n",
		atomic_read(&hw_priv->bh_rx));
	VDEBUG_PRINTF(seq, "Pending TX: %d\n",
		atomic_read(&hw_priv->bh_tx));
	if (hw_priv->bh_error)
		VDEBUG_PRINTF(seq, "BH errcode: %d\n",
			hw_priv->bh_error);
	VDEBUG_PRINTF(seq, "TX bufs:    %d x %d bytes\n",
		hw_priv->wsm_caps.numInpChBufs,
		hw_priv->wsm_caps.sizeInpChBuf);
	VDEBUG_PRINTF(seq, "Used bufs:  %d\n",
		hw_priv->hw_bufs_used);
	VDEBUG_PRINTF(seq, "Device:     %s\n",
		hw_priv->device_can_sleep ? "alseep" : "awake");

	spin_lock_bh(&hw_priv->wsm_cmd.lock);
	VDEBUG_PRINTF(seq, "WSM status: %s\n",
		hw_priv->wsm_cmd.done ? "idle" : "active");
	VDEBUG_PRINTF(seq, "WSM cmd:    0x%.4X (%d bytes)\n",
		hw_priv->wsm_cmd.cmd, hw_priv->wsm_cmd.len);
	VDEBUG_PRINTF(seq, "WSM retval: %d\n",
		hw_priv->wsm_cmd.ret);
	spin_unlock_bh(&hw_priv->wsm_cmd.lock);

	VDEBUG_PRINTF(seq, "Datapath:   %s\n",
		atomic_read(&hw_priv->tx_lock) ? "locked" : "unlocked");
	if (atomic_read(&hw_priv->tx_lock))
		VDEBUG_PRINTF(seq, "TXlock cnt: %d\n",
			atomic_read(&hw_priv->tx_lock));

	VDEBUG_PRINTF(seq, "Scan:       %s\n",
		atomic_read(&hw_priv->scan.in_progress) ? "active" : "idle");
	VDEBUG_PRINTF(seq, "Led state:  0x%.2X\n",
		hw_priv->softled_state);

	return 0;
}

int atbm_statistics_show(P_VDEBUG_SEQFILE seq, void *v)
{
	int ret;
	struct atbm_common *hw_priv = VDEBUG_PRIV(seq);
	struct wsm_statistics_table statistics;

	ret = wsm_get_statistics_table(hw_priv, &statistics);
	if (ret)
		return ret;
	VDEBUG_PRINTF(seq, "%s:" "\t\t" "%d\n", "latestRssi", statistics.latestRssi);

	return 0;
}

int atbm_pkt_show(P_VDEBUG_SEQFILE seq, void *v)
{
	int ret;
	struct atbm_common *hw_priv = VDEBUG_PRIV(seq);
	struct wsm_counters_table counters;

	ret = wsm_get_counters_table(hw_priv, &counters);
	if (ret)
		return ret;

#define CAT_STR(x, y) x ## y
#define PUT_COUNTER(tab, name) \
	VDEBUG_PRINTF(seq, "%s:" tab "%d\n", #name, \
		__le32_to_cpu(counters.CAT_STR(count, name)))

	PUT_COUNTER("\t\t", TxPackets);
	PUT_COUNTER("\t\t", RxPackets);
	PUT_COUNTER("\t\t", RxPacketErrors);
	PUT_COUNTER("\t",   TxFrameFailures);

#undef PUT_COUNTER
#undef CAT_STR

	return 0;
}

int atbm_counters_show(P_VDEBUG_SEQFILE seq, void *v)
{
	int ret;
	struct atbm_common *hw_priv = VDEBUG_PRIV(seq);
	struct wsm_counters_table counters;

	ret = wsm_get_counters_table(hw_priv, &counters);
	if (ret)
		return ret;

#define CAT_STR(x, y) x ## y
#define PUT_COUNTER(tab, name) \
	VDEBUG_PRINTF(seq, "%s:" tab "%d\n", #name, \
		__le32_to_cpu(counters.CAT_STR(count, name)))

	PUT_COUNTER("\t\t", PlcpErrors);
	PUT_COUNTER("\t\t", FcsErrors);
	PUT_COUNTER("\t\t", TxPackets);
	PUT_COUNTER("\t\t", RxPackets);
	PUT_COUNTER("\t\t", RxPacketErrors);
	PUT_COUNTER("\t",   RxDecryptionFailures);
	PUT_COUNTER("\t\t", RxMicFailures);
	PUT_COUNTER("\t",   RxNoKeyFailures);
	PUT_COUNTER("\t",   TxMulticastFrames);
	PUT_COUNTER("\t",   TxFramesSuccess);
	PUT_COUNTER("\t",   TxFrameFailures);
	PUT_COUNTER("\t",   TxFramesRetried);
	PUT_COUNTER("\t",   TxFramesMultiRetried);
	PUT_COUNTER("\t",   RxFrameDuplicates);
	PUT_COUNTER("\t\t", RtsSuccess);
	PUT_COUNTER("\t\t", RtsFailures);
	PUT_COUNTER("\t\t", AckFailures);
	PUT_COUNTER("\t",   RxMulticastFrames);
	PUT_COUNTER("\t",   RxFramesSuccess);
	PUT_COUNTER("\t",   RxCMACICVErrors);
	PUT_COUNTER("\t\t", RxCMACReplays);
	PUT_COUNTER("\t",   RxMgmtCCMPReplays);
	PUT_COUNTER("\t",   RxBIPMICErrors);

#undef PUT_COUNTER
#undef CAT_STR

	return 0;
}





ssize_t atbm_11n_write(struct file *file,
	const char __user *user_buf, size_t count, loff_t *ppos)
{
	struct atbm_common *hw_priv = file->private_data;
	struct ieee80211_supported_band *band[2] = {
		hw_priv->hw->wiphy->bands[IEEE80211_BAND_2GHZ],
		hw_priv->hw->wiphy->bands[IEEE80211_BAND_5GHZ],
	};
	char buf[1];
	int ena = 0;

	if (!count)
		return -EINVAL;
	if (copy_from_user(buf, user_buf, 1))
		return -EFAULT;
	if (buf[0] == 1)
		ena = 1;

	band[0]->ht_cap.ht_supported = ena;
#ifdef CONFIG_ATBM_APOLLO_5GHZ_SUPPORT
	band[1]->ht_cap.ht_supported = ena;
#endif /* CONFIG_ATBM_APOLLO_5GHZ_SUPPORT */

	return count;
}


ssize_t atbm_wsm_dumps(struct file *file,
	const char __user *user_buf, size_t count, loff_t *ppos)
{
	struct atbm_common *hw_priv = file->private_data;
	char buf[1];

	if (!count)
		return -EINVAL;
	if (copy_from_user(buf, user_buf, 1))
		return -EFAULT;

	if (buf[0] == '1')
		hw_priv->wsm_enable_wsm_dumps = 1;
	else
		hw_priv->wsm_enable_wsm_dumps = 0;

	return count;
}


#if defined(CONFIG_ATBM_APOLLO_WSM_DUMPS_SHORT)
static ssize_t atbm_short_dump_read(struct file *file,
	char __user *user_buf, size_t count, loff_t *ppos)
{
	struct atbm_common *hw_priv = file->private_data;
	char buf[20];
	size_t size = 0;

	sprintf(buf, "Size: %u\n", hw_priv->wsm_dump_max_size);
	size = strlen(buf);

	return simple_read_from_buffer(user_buf, count, ppos,
					buf, size);
}

static ssize_t atbm_short_dump_write(struct file *file,
	const char __user *user_buf, size_t count, loff_t *ppos)
{
	struct atbm_common *priv = file->private_data;
	char buf[20];
	unsigned long dump_size = 0;

	if (!count || count > 20)
		return -EINVAL;
	if (copy_from_user(buf, user_buf, count))
		return -EFAULT;

	if (kstrtoul(buf, 10, &dump_size))
		return -EINVAL;
	atbm_printk_always("%s get %lu\n", __func__, dump_size);

	priv->wsm_dump_max_size = dump_size;

	return count;
}

#endif /* CONFIG_ATBM_APOLLO_WSM_DUMPS_SHORT */


int atbm_status_show_priv(P_VDEBUG_SEQFILE seq, void *v)
{
	int i;
	struct atbm_vif *priv = VDEBUG_PRIV(seq);
	struct atbm_debug_priv *d = &priv->debug;

	VDEBUG_PRINTF(seq, "Mode:       %s%s\n",
		atbm_debug_mode(priv->mode),
		priv->listening ? " (listening)" : "");
	VDEBUG_PRINTF(seq, "Assoc:      %s\n",
		atbm_debug_join_status[priv->join_status]);
	if (priv->rx_filter.promiscuous)
		VDEBUG_PUTS(seq,   "Filter:     promisc\n");
	else if (priv->rx_filter.fcs)
		VDEBUG_PUTS(seq,   "Filter:     fcs\n");
	if (priv->rx_filter.bssid)
		VDEBUG_PUTS(seq,   "Filter:     bssid\n");
	if (priv->bf_control.bcn_count)
		VDEBUG_PUTS(seq,   "Filter:     beacons\n");

	if (priv->enable_beacon ||
			priv->mode == NL80211_IFTYPE_AP ||
			priv->mode == NL80211_IFTYPE_ADHOC ||
			priv->mode == NL80211_IFTYPE_MESH_POINT ||
			priv->mode == NL80211_IFTYPE_P2P_GO)
		VDEBUG_PRINTF(seq, "Beaconing:  %s\n",
			priv->enable_beacon ?
			"enabled" : "disabled");
	if (priv->ssid_length ||
			priv->mode == NL80211_IFTYPE_AP ||
			priv->mode == NL80211_IFTYPE_ADHOC ||
			priv->mode == NL80211_IFTYPE_MESH_POINT ||
			priv->mode == NL80211_IFTYPE_P2P_GO)
		VDEBUG_PRINTF(seq, "SSID:       %.*s\n",
			priv->ssid_length, priv->ssid);

	for (i = 0; i < 4; ++i) {
		VDEBUG_PRINTF(seq, "EDCA(%d):    %d, %d, %d, %d, %d\n", i,
			priv->edca.params[i].cwMin,
			priv->edca.params[i].cwMax,
			priv->edca.params[i].aifns,
			priv->edca.params[i].txOpLimit,
			priv->edca.params[i].maxReceiveLifetime);
	}
	if (priv->join_status == ATBM_APOLLO_JOIN_STATUS_STA) {
		static const char *pmMode = "unknown";
		switch (priv->powersave_mode.pmMode) {
		case WSM_PSM_ACTIVE:
			pmMode = "off";
			break;
		case WSM_PSM_PS:
			pmMode = "on";
			break;
		case WSM_PSM_FAST_PS:
			pmMode = "dynamic";
			break;
		}
		VDEBUG_PRINTF(seq, "Preamble:   %s\n",
			atbm_debug_preamble[
			priv->association_mode.preambleType]);
		VDEBUG_PRINTF(seq, "AMPDU spcn: %d\n",
			priv->association_mode.mpduStartSpacing);
		VDEBUG_PRINTF(seq, "Basic rate: 0x%.8X\n",
			le32_to_cpu(priv->association_mode.basicRateSet));
		VDEBUG_PRINTF(seq, "Bss lost:   %d beacons\n",
			priv->bss_params.beaconLostCount);
		VDEBUG_PRINTF(seq, "AID:        %d\n",
			priv->bss_params.aid);
		VDEBUG_PRINTF(seq, "Rates:      0x%.8X\n",
			priv->bss_params.operationalRateSet);
		VDEBUG_PRINTF(seq, "Powersave:  %s\n", pmMode);
	}
	VDEBUG_PRINTF(seq, "RSSI thold: %d\n",
		priv->cqm_rssi_thold);
	VDEBUG_PRINTF(seq, "RSSI hyst:  %d\n",
		priv->cqm_rssi_hyst);
	VDEBUG_PRINTF(seq, "TXFL thold: %d\n",
		priv->cqm_tx_failure_thold);
	VDEBUG_PRINTF(seq, "Linkloss:   %d\n",
		priv->cqm_link_loss_count);
	VDEBUG_PRINTF(seq, "Bcnloss:    %d\n",
		priv->cqm_beacon_loss_count);

	atbm_debug_print_map(seq, priv, "Link map:   ",
		priv->link_id_map);
	atbm_debug_print_map(seq, priv, "Asleep map: ",
		priv->sta_asleep_mask);
	atbm_debug_print_map(seq, priv, "PSPOLL map: ",
		priv->pspoll_mask);

	VDEBUG_PUTS(seq, "\n");

	for (i = 0; i < ATBMWIFI_MAX_STA_IN_AP_MODE; ++i) {
		if (priv->link_id_db[i].status) {
			VDEBUG_PRINTF(seq, "Link %d:     %s, %pM\n",
				i + 1, atbm_debug_link_id[
				priv->link_id_db[i].status],
				priv->link_id_db[i].mac);
		}
	}

	VDEBUG_PUTS(seq, "\n");

	VDEBUG_PRINTF(seq, "Powermgmt:  %s\n",
		priv->powersave_enabled ? "on" : "off");

	VDEBUG_PRINTF(seq, "TXed:       %d\n",
		d->tx);
	VDEBUG_PRINTF(seq, "AGG TXed:   %d\n",
		d->tx_agg);
	VDEBUG_PRINTF(seq, "MULTI TXed: %d (%d)\n",
		d->tx_multi, d->tx_multi_frames);
	VDEBUG_PRINTF(seq, "RXed:       %d\n",
		d->rx);
	VDEBUG_PRINTF(seq, "AGG RXed:   %d\n",
		d->rx_agg);
	VDEBUG_PRINTF(seq, "TX align:   %d\n",
		d->tx_align);
	VDEBUG_PRINTF(seq, "TX TTL:     %d\n",
		d->tx_ttl);
	return 0;
}
int atbm_print_fw_version(struct atbm_common *hw_priv, u8* buf, size_t len)
{
	return snprintf(buf, len, "%s %d.%d",
			atbm_debug_fw_types[hw_priv->wsm_caps.firmwareType],
			hw_priv->wsm_caps.firmwareVersion,
			hw_priv->wsm_caps.firmwareBuildNumber);
}

#ifdef CONFIG_ATBM_APOLLO_DEBUGFS
static int atbm_counters_open(struct inode *inode, struct file *file)
{
	return single_open(file, &atbm_counters_show,
		inode->i_private);
}
static const struct file_operations fops_counters = {
	.open = atbm_counters_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.owner = THIS_MODULE,
};
static int atbm_generic_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}
static ssize_t atbm_11n_read(struct file *file,
	char __user *user_buf, size_t count, loff_t *ppos)
{
	struct atbm_common *hw_priv = file->private_data;
	struct ieee80211_supported_band *band =
		hw_priv->hw->wiphy->bands[IEEE80211_BAND_2GHZ];
	return simple_read_from_buffer(user_buf, count, ppos,
		band->ht_cap.ht_supported ? "1\n" : "0\n", 2);
}

static int atbm_ht_open_info(struct inode *inode, struct file *file)
{
	return single_open(file, &atbm_ht_show_info,
		inode->i_private);
}
static int atbm_wifi_run_status(struct inode *inode, struct file *file)
{
	return single_open(file, &atbm_wifi_show_status,
		inode->i_private);
}

static int atbm_status_open_common(struct inode *inode, struct file *file)
{
	return single_open(file, &atbm_status_show_common,
		inode->i_private);
}
static const struct file_operations fops_11n = {
	.open = atbm_generic_open,
	.read = atbm_11n_read,
	.write = atbm_11n_write,
	.llseek = default_llseek,
};
static const struct file_operations fops_wsm_dumps = {
	.open = atbm_generic_open,
	.write = atbm_wsm_dumps,
	.llseek = default_llseek,
};
#if defined(CONFIG_ATBM_APOLLO_WSM_DUMPS_SHORT)
static const struct file_operations fops_short_dump = {
	.open = atbm_generic_open,
	.write = atbm_short_dump_write,
	.read = atbm_short_dump_read,
	.llseek = default_llseek,
};
#endif
static const struct file_operations fops_status_common = {
	.open = atbm_status_open_common,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.owner = THIS_MODULE,
};

static const struct file_operations fops_ht_info = {
	.open = atbm_ht_open_info,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.owner = THIS_MODULE,
};

static const struct file_operations fops_wifi_run_status = {
	.open = atbm_wifi_run_status,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.owner = THIS_MODULE,
};

#else
#endif /*CONFIG_ATBM_APOLLO_DEBUGFS*/

int atbm_debug_init_priv(struct atbm_common *hw_priv,
			   struct atbm_vif *priv)
{
	memset(&priv->debug,0,sizeof(priv->debug));
	return 0;
}

void atbm_debug_release_priv(struct atbm_vif *priv)
{
	memset(&priv->debug,0,sizeof(priv->debug));
}
int atbm_debug_init_common(struct atbm_common *hw_priv)
{
	int ret = -ENOMEM;
	struct atbm_debug_common *d =
		atbm_kzalloc(sizeof(struct atbm_debug_common), GFP_KERNEL);
	hw_priv->debug = d;
	if (!d)
		return ret;
#ifdef CONFIG_ATBM_APOLLO_DEBUGFS

	atbm_printk_init("atbm_debug_init_common: create dir............ \n\n");

	d->debugfs_phy = debugfs_create_dir("apollo",
			hw_priv->hw->wiphy->debugfsdir);
	if (!d->debugfs_phy)
		goto err;

	if (!debugfs_create_file("ht", S_IRUSR, d->debugfs_phy,
			hw_priv, &fops_ht_info))
		goto err;
	
	if (!debugfs_create_file("run_sta", S_IRUSR, d->debugfs_phy,
			hw_priv, &fops_wifi_run_status))
		goto err;

	if (!debugfs_create_file("status", S_IRUSR, d->debugfs_phy,
			hw_priv, &fops_status_common))
		goto err;

	if (!debugfs_create_file("counters", S_IRUSR, d->debugfs_phy,
			hw_priv, &fops_counters))
		goto err;

	if (!debugfs_create_file("11n", S_IRUSR | S_IWUSR,
			d->debugfs_phy, hw_priv, &fops_11n))
		goto err;

	if (!debugfs_create_file("wsm_dumps", S_IWUSR, d->debugfs_phy,
			hw_priv, &fops_wsm_dumps))
		goto err;

#if defined(CONFIG_ATBM_APOLLO_WSM_DUMPS_SHORT)
	if (!debugfs_create_file("wsm_dump_size", S_IRUSR | S_IWUSR,
			d->debugfs_phy, hw_priv, &fops_short_dump))
		goto err;
#endif /* CONFIG_ATBM_APOLLO_WSM_DUMPS_SHORT */
#endif
	EELOG_INIT();
	EELOG_Start();

	return 0;
#ifdef CONFIG_ATBM_APOLLO_DEBUGFS
	err:
		hw_priv->debug = NULL;
#endif
#ifdef CONFIG_ATBM_APOLLO_DEBUGFS
	debugfs_remove_recursive(d->debugfs_phy);
#endif /* CONFIG_ATBM_APOLLO_DEBUGFS*/
	atbm_kfree(d);
	return ret;
}

void atbm_debug_release_common(struct atbm_common *hw_priv)
{
	struct atbm_debug_common *d = hw_priv->debug;
	if (d) {
		hw_priv->debug = NULL;
		atbm_kfree(d);
	}
}

#endif
