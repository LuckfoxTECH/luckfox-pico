#include <net/atbm_mac80211.h>
#include <net/rtnetlink.h>

#include "ieee80211_i.h"
#include "mesh.h"
#include "driver-ops.h"
#include "led.h"
#if defined (ATBM_SUSPEND_REMOVE_INTERFACE) || defined (ATBM_SUPPORT_WOW)
/* return value indicates whether the driver should be further notified */
static bool ieee80211_quiesce(struct ieee80211_sub_if_data *sdata)
{
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
		ieee80211_sta_quiesce(sdata);
		return true;
#ifdef CONFIG_ATBM_SUPPORT_IBSS
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_quiesce(sdata);
		return true;
#endif
#ifdef CONFIG_MAC80211_ATBM_MESH
	case NL80211_IFTYPE_MESH_POINT:
		ieee80211_mesh_quiesce(sdata);
		return true;
#endif
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_MONITOR:
		/* don't tell driver about this */
		return false;
	default:
		return true;
	}
}
#endif
static void ieee80211_suspend_sta_disconnect(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;

	if(sdata->vif.type == NL80211_IFTYPE_STATION){
		if(!(sdata->flags & IEEE80211_SDATA_DISCONNECT_RESUME))
			ieee80211_connection_loss(&sdata->vif);
	}else if(sdata->vif.type == NL80211_IFTYPE_AP){	
		mutex_lock(&local->sta_mtx);
		list_for_each_entry(sta, &local->sta_list, list) {
			if ((sta->uploaded)&&(sta->sdata == sdata)) {				
				WARN_ON(ieee80211_rx_sta_cook_deauthen(sta)==false);
				WARN_ON(ieee80211_tx_sta_deauthen(sta)==false);		
			}
		}
		mutex_unlock(&local->sta_mtx);		
	}

	atbm_flush_workqueue(local->workqueue);

	drv_flush(local, sdata, false);
	sta_info_flush(local, sdata);
	synchronize_net();
}
int __ieee80211_suspend(struct ieee80211_hw *hw, struct cfg80211_wowlan *wowlan)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;

	/* PM code has a watchdog to trigger a BUG when
	 * suspend callback is not returning in several seconds.
	 * Some WLAN hardware has longer timeouts for non-interruptible
	 * configuration-related operations, leading to the watchdog
	 * timeout while ieee80211_scan_cancel is waiting on the mutex.
	 *
	 * The code below checks if interface mutex is already held
	 * and rejects suspend if there is a possibility of locking.
	 *
	 * It's a bit racy, but handles most of cases.
	 */
	if (mutex_trylock(&local->mtx))
		mutex_unlock(&local->mtx);
	else {
		atbm_printk_warn("Suspend when operation is in progress. Suspend aborted.\n");
		return -EBUSY;
	}

	if (!local->open_count)
		goto suspend;

	ieee80211_scan_cancel(local);
	
	list_for_each_entry(sdata, &local->interfaces, list) {
		
		atbm_cancel_work_sync(&sdata->work);
	
		if (!ieee80211_sdata_running(sdata))
			continue;

		if(local->pending_scan_sdata&&(local->pending_scan_sdata == sdata))
		{
			local->scan_sdata = local->pending_scan_sdata;
			local->scan_req = local->pending_scan_req;
			local->pending_scan_sdata = NULL;
			local->pending_scan_req = NULL;
			ieee80211_scan_cancel(local);
		}
		
		ieee80211_work_purge(sdata);
#ifdef CONFIG_ATBM_SUPPORT_P2P
		ieee80211_roc_purge(sdata);
#endif
#ifdef ATBM_SUPPORT_WOW
		if(wowlan == NULL)
#endif
		{
			ieee80211_suspend_sta_disconnect(sdata);
		}
	}
	if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
		mutex_lock(&local->sta_mtx);
		list_for_each_entry(sta, &local->sta_list, list) {
			set_sta_flag(sta, WLAN_STA_BLOCK_BA);
			ieee80211_sta_tear_down_BA_sessions(sta, true);
		}
		mutex_unlock(&local->sta_mtx);
	}

	ieee80211_stop_queues_by_reason(hw,
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);

	/* flush out all packets */
	synchronize_net();
	list_for_each_entry(sdata, &local->interfaces, list)
		drv_flush(local, sdata, false);

	local->quiescing = true;
	/* make quiescing visible to timers everywhere */
	mb();

	atbm_flush_workqueue(local->workqueue);

	/* Don't try to run timers while suspended. */
	atbm_del_timer_sync(&local->sta_cleanup);
#ifdef CONFIG_ATBM_MAC80211_NO_USE
	 /*
	 * Note that this particular timer doesn't need to be
	 * restarted at resume.
	 */
	list_for_each_entry(sdata, &local->interfaces, list) {
		atbm_cancel_work_sync(&sdata->dynamic_ps_enable_work);
		atbm_del_timer_sync(&sdata->dynamic_ps_timer);
	}
#endif
	local->wowlan = wowlan && local->open_count;
	atbm_printk_pm( "enable wowlan(%d)\n",local->wowlan);
#ifndef ATBM_SUPPORT_WOW
	local->wowlan = 0;
#endif
#ifdef ATBM_SUPPORT_WOW
	if (local->wowlan) {
		int err = drv_suspend(local, wowlan);
		if (err < 0) {
			local->quiescing = false;
			ieee80211_wake_queues_by_reason(hw,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND);
			return err;
		} else if (err > 0) {
			WARN_ON(err != 1);
			local->wowlan = false;
		} else {
			list_for_each_entry(sdata, &local->interfaces, list) {
				atbm_cancel_work_sync(&sdata->work);
				ieee80211_quiesce(sdata);
			}
			goto suspend;
		}
		list_for_each_entry(sdata, &local->interfaces, list) {
			atbm_cancel_work_sync(&sdata->work);
		}
		goto suspend;
	}
#endif
#ifdef ATBM_SUSPEND_REMOVE_INTERFACE
	/* disable keys */
	#pragma message("Suspend Remove Interface")
	list_for_each_entry(sdata, &local->interfaces, list)
		ieee80211_disable_keys(sdata);

	/* tear down aggregation sessions and remove STAs */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		if (sta->uploaded) {
			sdata = sta->sdata;
			if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
				sdata = container_of(sdata->bss,
					     struct ieee80211_sub_if_data,
					     u.ap);

			drv_sta_remove(local, sdata, &sta->sta);
		}

		mesh_plink_quiesce(sta);
	}
	mutex_unlock(&local->sta_mtx);

	/* remove all interfaces */
	list_for_each_entry(sdata, &local->interfaces, list) {
		atbm_cancel_work_sync(&sdata->work);

		if (!ieee80211_quiesce(sdata))
			continue;

		if (!ieee80211_sdata_running(sdata))
			continue;

		/* disable beaconing */
		ieee80211_bss_info_change_notify(sdata,
			BSS_CHANGED_BEACON_ENABLED);

		drv_remove_interface(local, &sdata->vif);
	}

	/* stop hardware - this must stop RX */
	if (local->open_count)
		ieee80211_stop_device(local);
#else
	if(local->wowlan == 0){
		int err = 0;
		list_for_each_entry(sdata, &local->interfaces, list) {
			atbm_cancel_work_sync(&sdata->work);
		
			if (!ieee80211_sdata_running(sdata))
				continue;
			ieee80211_work_purge(sdata);
#ifdef CONFIG_ATBM_SUPPORT_P2P
			ieee80211_roc_purge(sdata);
#endif
		}
		ieee80211_scan_cancel(local);
		err = drv_suspend(local, NULL);
	}
#endif
 suspend:
	local->suspended = true;
	/* need suspended to be visible before quiescing is false */
	barrier();
	local->quiescing = false;
#ifdef CONFIG_ATBM_SUPPORT_SCHED_SCAN
#ifdef ROAM_OFFLOAD
	local->sched_scanning = true;
#endif /*ROAM_OFFLOAD*/
#endif
	return 0;
}

/*
 * __ieee80211_resume() is a static inline which just calls
 * ieee80211_reconfig(), which is also needed for hardware
 * hang/firmware failure/etc. recovery.
 */
