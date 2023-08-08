#include <ssv6200.h>
#include "dev.h"
#include "init.h"
#ifdef CONFIG_SSV_SUPPORT_ANDROID

#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>
#endif
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 4, 0)
#include <linux/notifier.h>
#include <linux/fb.h>
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND) || (LINUX_VERSION_CODE > KERNEL_VERSION(3, 4, 0))
#ifdef CONFIG_HAS_EARLYSUSPEND
void ssv6xxx_early_suspend(struct early_suspend *h)
#else
void ssv6xxx_early_suspend(void)
#endif

{
#ifdef CONFIG_HAS_EARLYSUSPEND
    struct ssv_softc *sc = container_of(h, struct ssv_softc, early_suspend);
#else
    struct ssv_softc *sc = ssv6xxx_driver_attach(SSV_DRVER_NAME);
#endif

    sc->ps_status = PWRSV_PREPARE;

    printk(KERN_INFO "ssv6xxx_early_suspend\n");

}

#define DEAUTH_DISASSOC_LEN	(24 /* hdr */ + 2 /* reason */)
void ssv6xxx_send_deauth_toself(struct ssv_softc *sc,const u8 *bssid,const u8 *self_addr)
{
    struct sk_buff *deauth_skb ;
	u8 frame_buf[DEAUTH_DISASSOC_LEN];
    struct ieee80211_mgmt *mgmt = (void *)frame_buf;

	/* build frame */
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DEAUTH);
	mgmt->duration = 0; /* initialize only */
	mgmt->seq_ctrl = 0; /* initialize only */
	memcpy(mgmt->da, self_addr, ETH_ALEN);
	memcpy(mgmt->sa, bssid, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	/* u.deauth.reason_code == u.disassoc.reason_code */
	mgmt->u.deauth.reason_code = cpu_to_le16(2);

	{
		deauth_skb = dev_alloc_skb(DEAUTH_DISASSOC_LEN);
		if (!deauth_skb)
			return;

		//skb_reserve(skb, local->hw.extra_tx_headroom);

		/* copy in frame */
		memcpy(skb_put(deauth_skb, DEAUTH_DISASSOC_LEN),
		       mgmt, DEAUTH_DISASSOC_LEN);

        #if defined(USE_THREAD_RX) && !defined(IRQ_PROC_RX_DATA)
        local_bh_disable();
        ieee80211_rx(sc->hw, deauth_skb);
        local_bh_enable();
        #else
        ieee80211_rx_irqsafe(sc->hw, deauth_skb);
        #endif // USE_THREAD_RX
	    return;
	}
}

#ifdef CONFIG_HAS_EARLYSUSPEND
void ssv6xxx_late_resume(struct early_suspend *h)
#else
void ssv6xxx_late_resume(void)
#endif
{
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct ssv_softc *sc = container_of(h, struct ssv_softc, early_suspend);
#else
	struct ssv_softc *sc = ssv6xxx_driver_attach(SSV_DRVER_NAME);
#endif

    if(sc->ps_status == PWRSV_ENABLE)
    {
        if(sc->vif_info[0].vif)
        {
            if(sc->vif_info[0].vif->bss_conf.assoc)
            {
                printk("sc->vif_info[0].vif->bss_conf.assoc\n");
                if ((sc->vif_info[0].vif->type == NL80211_IFTYPE_STATION) || (sc->vif_info[0].vif->p2p))
                {
                    printk("sc->vif_info[0].vif->bss_conf.assoc ssv6xxx_send_deauth_toself\n");
                    ssv6xxx_send_deauth_toself(sc, sc->vif_info[0].vif->bss_conf.bssid, sc->vif_info[0].vif->addr);
                }
            }
        }
        if(sc->vif_info[1].vif)
        {
            if(sc->vif_info[1].vif->bss_conf.assoc)
            {
                printk("sc->vif_info[1].vif->bss_conf.assoc\n");
                if ((sc->vif_info[1].vif->type == NL80211_IFTYPE_STATION) || (sc->vif_info[1].vif->p2p))
                {
                    printk("sc->vif_info[1].vif->bss_conf.assoc ssv6xxx_send_deauth_toself\n");
                    ssv6xxx_send_deauth_toself(sc, sc->vif_info[1].vif->bss_conf.bssid, sc->vif_info[1].vif->addr);
                }
            }
        }
    }

    if(sc)
        ssv6xxx_disable_ps(sc);
    else
        printk(KERN_INFO "ssv6xxx_late_resume,sc=NULL\n");

    printk(KERN_INFO "ssv6xxx_late_resume\n");
}

#ifndef CONFIG_HAS_EARLYSUSPEND
static int ssv_wlan_fb_event_notify(struct notifier_block *self,
                       unsigned long action, void *data)
{

        struct fb_event *event = data;
        int blank_mode = *((int *)event->data);

        if (action == FB_EARLY_EVENT_BLANK) {
                switch (blank_mode) {
                case FB_BLANK_UNBLANK:
                        break;
                default:
						ssv6xxx_early_suspend();
                        break;
                }
        }
 		else if (action == FB_EVENT_BLANK) {
                switch (blank_mode) {
                case FB_BLANK_UNBLANK:
						ssv6xxx_late_resume();
                        break;
                default:
                        break;
                }
        }
        return NOTIFY_OK;
}

struct notifier_block ssv_wlan_fb_notifier = {
    .notifier_call = ssv_wlan_fb_event_notify,
};

#endif
#endif //CONFIG_HAS_EARLYSUSPEND

void ssv_wakelock_init(struct ssv_softc *sc)
{
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_init(&sc->ssv_wake_lock_, WAKE_LOCK_SUSPEND, "ssv6051");
#endif
#ifndef CONFIG_HAS_EARLYSUSPEND
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
	fb_register_client(&ssv_wlan_fb_notifier);
#endif
#endif

}

void ssv_wakelock_destroy(struct ssv_softc *sc)
{
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock_destroy(&sc->ssv_wake_lock_);
#endif
#ifndef CONFIG_HAS_EARLYSUSPEND
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
	fb_unregister_client(&ssv_wlan_fb_notifier);
#endif
#endif

}

void ssv_wake_lock(struct ssv_softc *sc)
{
#ifdef CONFIG_HAS_WAKELOCK
    wake_lock(&sc->ssv_wake_lock_);
#endif
}

void ssv_wake_timeout(struct ssv_softc *sc, int secs)
{
#ifdef CONFIG_HAS_WAKELOCK
    //printk("ssv_wake_timeout\n");
    wake_lock_timeout(&sc->ssv_wake_lock_,secs*HZ);
#endif
}

void ssv_wake_unlock(struct ssv_softc *sc)
{
#ifdef CONFIG_HAS_WAKELOCK
    wake_unlock(&sc->ssv_wake_lock_);
#endif
}
#endif // CONFIG_SSV_SUPPORT_ANDROID
