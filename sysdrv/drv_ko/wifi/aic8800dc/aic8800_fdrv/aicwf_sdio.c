/**
 * aicwf_sdmmc.c
 *
 * SDIO function declarations
 *
 * Copyright (C) AICSemi 2018-2020
 */
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/semaphore.h>
#include <linux/debugfs.h>
#include <linux/kthread.h>
#include <linux/sched/types.h>
#include "aicwf_txrxif.h"
#include "aicwf_sdio.h"
#include "sdio_host.h"
#include "rwnx_defs.h"
#include "rwnx_platform.h"
#include "aicwf_rx_prealloc.h"
#include "rwnx_msg_tx.h"
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
#include <linux/pm_wakeirq.h>
#else
#include <linux/pm_wakeup.h>
#endif
#include "rwnx_wakelock.h"

#ifdef CONFIG_INGENIC_T20
#include "mach/jzmmc.h"
#endif /* CONFIG_INGENIC_T20 */
#ifdef CONFIG_PLATFORM_ROCKCHIP
#include <linux/rfkill-wlan.h>
#endif
#ifdef CONFIG_PLATFORM_ROCKCHIP2
#include <linux/rfkill-wlan.h>
#endif

#include "aic_bsp_export.h"
extern uint8_t scanning;

#ifdef CONFIG_SDIO_ADMA
unsigned char sdio_tx_buf_fill[512];
unsigned char sdio_tx_buf_dummy[SDIO_TX_SLIST_MAX][8];
#endif

#ifdef CONFIG_GPIO_WAKEUP
extern int rwnx_send_me_set_lp_level(struct rwnx_hw *rwnx_hw, u8 lp_level,
				     u8 disable_filter);

#ifdef CONFIG_WIFI_SUSPEND_FOR_LINUX
#include <linux/proc_fs.h>
void rwnx_init_wifi_suspend_node(void);
void rwnx_deinit_wifi_suspend_node(void);
void rwnx_set_wifi_suspend(char onoff);
struct proc_dir_entry *wifi_suspend_node;
#endif //CONFIG_WIFI_SUSPEND_FOR_LINUX

#endif //CONFIG_GPIO_WAKEUP

int tx_aggr_counter = 32;
module_param_named(tx_aggr_counter, tx_aggr_counter, int, 0644);

#ifdef CONFIG_TX_NETIF_FLOWCTRL
int tx_fc_low_water = AICWF_SDIO_TX_LOW_WATER;
module_param_named(tx_fc_low_water, tx_fc_low_water, int, 0644);

int tx_fc_high_water = AICWF_SDIO_TX_HIGH_WATER;
module_param_named(tx_fc_high_water, tx_fc_high_water, int, 0644);
#endif

#if 0
void rwnx_data_dump(char* tag, void* data, unsigned long len){
	if(dump){
        unsigned long i = 0;
        uint8_t* data_ = (uint8_t* )data;

        printk("%s %s len:(%lu)\r\n", __func__, tag, len);

        for (i = 0; i < len; i += 16){
        printk("%02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
                data_[0 + i],
                data_[1 + i],
                data_[2 + i],
                data_[3 + i],
                data_[4 + i],
                data_[5 + i],
                data_[6 + i],
                data_[7 + i],
                data_[8 + i],
                data_[9 + i],
                data_[10 + i],
                data_[11 + i],
                data_[12 + i],
                data_[13 + i],
                data_[14 + i],
                data_[15 + i]);
                }
	}
}
#endif

int aicwf_sdio_readb(struct aic_sdio_dev *sdiodev, uint regaddr, u8 *val)
{
	int ret;
	sdio_claim_host(sdiodev->func);
	*val = sdio_readb(sdiodev->func, regaddr, &ret);
	sdio_release_host(sdiodev->func);
	return ret;
}

int aicwf_sdio_writeb(struct aic_sdio_dev *sdiodev, uint regaddr, u8 val)
{
	int ret;
	sdio_claim_host(sdiodev->func);
	sdio_writeb(sdiodev->func, val, regaddr, &ret);
	sdio_release_host(sdiodev->func);
	return ret;
}

int aicwf_sdio_func2_readb(struct aic_sdio_dev *sdiodev, uint regaddr, u8 *val)
{
	int ret;
	sdio_claim_host(sdiodev->func2);
	*val = sdio_readb(sdiodev->func2, regaddr, &ret);
	sdio_release_host(sdiodev->func2);
	return ret;
}

int aicwf_sdio_func2_writeb(struct aic_sdio_dev *sdiodev, uint regaddr, u8 val)
{
	int ret;
	sdio_claim_host(sdiodev->func2);
	sdio_writeb(sdiodev->func2, val, regaddr, &ret);
	sdio_release_host(sdiodev->func2);
	return ret;
}

#ifdef CONFIG_TX_NETIF_FLOWCTRL
void aicwf_sdio_tx_netif_flowctrl(struct rwnx_hw *rwnx_hw, bool state)
{
	struct rwnx_vif *rwnx_vif;
	list_for_each_entry (rwnx_vif, &rwnx_hw->vifs, list) {
		if (!rwnx_vif->up)
			continue;
		if (state)
			netif_tx_stop_all_queues(
				rwnx_vif->ndev); //netif_stop_queue(rwnx_vif->ndev);
		else
			netif_tx_wake_all_queues(
				rwnx_vif->ndev); //netif_wake_queue(rwnx_vif->ndev);
	}
}
#endif

#ifdef CONFIG_TEMP_CONTROL
//int interval = 30;
//module_param(interval, int, 0660);
static int update_state(s8_l value, u8_l current_state)
{
	if (value > TEMP_THD_2)
		return 2;
	else if (value > (TEMP_THD_2 - BUFFERING_V2) && (current_state == 2))
		return 2;
	else if (value > TEMP_THD_1 && current_state != 2)
		return 1;
	else if (value > (TEMP_THD_1 - BUFFERING_V1) && current_state == 1)
		return 1;
	else if (current_state == 0)
		return 0;
	else
		return 1;
}

void aicwf_netif_ctrl(struct aic_sdio_dev *sdiodev, int val)
{
	unsigned long flags;
	struct rwnx_vif *rwnx_vif;

	if (sdiodev->net_stop)
		return;

	spin_lock_irqsave(&sdiodev->tx_flow_lock, flags);
	list_for_each_entry (rwnx_vif, &sdiodev->rwnx_hw->vifs, list) {
		if (!rwnx_vif || !rwnx_vif->ndev || !rwnx_vif->up)
			continue;
		netif_tx_stop_all_queues(
			rwnx_vif->ndev); //netif_stop_queue(rwnx_vif->ndev);
	}
	spin_unlock_irqrestore(&sdiodev->tx_flow_lock, flags);
	sdiodev->net_stop = true;
	mod_timer(&sdiodev->netif_timer, jiffies + msecs_to_jiffies(val));

	return;
}

void aicwf_temp_ctrl(struct aic_sdio_dev *sdiodev)
{
	if (sdiodev->set_level) {
		if (sdiodev->set_level == 1) {
			sdiodev->get_level = 1;
			aicwf_netif_ctrl(
				sdiodev,
				sdiodev->interval_t1 /*TMR_INTERVAL_1*/);
			//mdelay(1);
		} else if (sdiodev->set_level == 2) {
			sdiodev->get_level = 2;
			aicwf_netif_ctrl(
				sdiodev,
				sdiodev->interval_t2 /*TMR_INTERVAL_2*/);
			//mdelay(2);
		}
		return;
	} else {
		if (sdiodev->cur_temp > (TEMP_THD_1 - 8)) {
			//if ((sdiodev->cur_temp > TEMP_THD_1 && sdiodev->cur_temp <= TEMP_THD_2) || (sdiodev->cur_stat == 1)) {
			if (update_state(sdiodev->cur_temp,
					 sdiodev->cur_stat) == 1) {
				sdiodev->get_level = 1;
				sdiodev->cur_stat = 1;
				aicwf_netif_ctrl(
					sdiodev,
					sdiodev->interval_t1 /*TMR_INTERVAL_1*/);
				//mdelay(1);
				//break;
				//} else if ((sdiodev->cur_temp > TEMP_THD_2) || (sdiodev->cur_stat == 2)) {
			} else if (update_state(sdiodev->cur_temp,
						sdiodev->cur_stat) == 2) {
				sdiodev->get_level = 2;
				sdiodev->cur_stat = 2;
				aicwf_netif_ctrl(
					sdiodev,
					sdiodev->interval_t2 /*TMR_INTERVAL_2*/);
				//mdelay(2);
				//break;
			}
			return;
		}

		if (sdiodev->cur_stat) {
			AICWFDBG(LOGINFO, "reset cur_stat");
			sdiodev->cur_stat = 0;
		}

		return;
	}
}

void aicwf_netif_worker(struct work_struct *work)
{
	struct aic_sdio_dev *sdiodev =
		container_of(work, struct aic_sdio_dev, netif_work);
	unsigned long flags;
	struct rwnx_vif *rwnx_vif;
	spin_lock_irqsave(&sdiodev->tx_flow_lock, flags);
	list_for_each_entry (rwnx_vif, &sdiodev->rwnx_hw->vifs, list) {
		if (!rwnx_vif || !rwnx_vif->ndev || !rwnx_vif->up)
			continue;
		netif_tx_wake_all_queues(
			rwnx_vif->ndev); //netif_wake_queue(rwnx_vif->ndev);
	}
	spin_unlock_irqrestore(&sdiodev->tx_flow_lock, flags);
	sdiodev->net_stop = false;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
static void aicwf_netif_timer(ulong data)
#else
static void aicwf_netif_timer(struct timer_list *t)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
	struct aic_sdio_dev *sdiodev = (struct aic_sdio_dev *)data;
#else
	struct aic_sdio_dev *sdiodev = from_timer(sdiodev, t, netif_timer);
#endif

	if (!work_pending(&sdiodev->netif_work))
		schedule_work(&sdiodev->netif_work);

	return;
}

void aicwf_temp_ctrl_worker(struct work_struct *work)
{
	struct rwnx_hw *rwnx_hw;
	struct mm_set_vendor_swconfig_cfm cfm;
	struct aic_sdio_dev *sdiodev =
		container_of(work, struct aic_sdio_dev, tp_ctrl_work);
	rwnx_hw = sdiodev->rwnx_hw;
	//AICWFDBG(LOGINFO, "%s\n", __func__);

	rwnx_hw->started_jiffies = jiffies;

	rwnx_send_get_temp_req(rwnx_hw, &cfm);
	sdiodev->cur_temp = cfm.temp_comp_get_cfm.degree;

	mod_timer(&sdiodev->tp_ctrl_timer,
		  jiffies + msecs_to_jiffies(TEMP_GET_INTERVAL));

	return;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
static void aicwf_temp_ctrl_timer(ulong data)
#else
static void aicwf_temp_ctrl_timer(struct timer_list *t)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
	struct aic_sdio_dev *sdiodev = (struct aic_sdio_dev *)data;
#else
	struct aic_sdio_dev *sdiodev = from_timer(sdiodev, t, tp_ctrl_timer);
#endif

	if (!work_pending(&sdiodev->tp_ctrl_work))
		schedule_work(&sdiodev->tp_ctrl_work);

	return;
}
#endif

int aicwf_sdio_flow_ctrl_msg(struct aic_sdio_dev *sdiodev)
{
	int ret = -1;
	u8 fc_reg = 0;
	u32 count = 0;

	while (true) {
		ret = aicwf_sdio_readb(sdiodev, sdiodev->sdio_reg.flow_ctrl_reg,
				       &fc_reg);
		if (ret) {
			return -1;
		}

		if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
		    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
		    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
			fc_reg = fc_reg & SDIOWIFI_FLOWCTRL_MASK_REG;
		}

		if (fc_reg != 0) {
			ret = fc_reg;
			if (ret > tx_aggr_counter) {
				ret = tx_aggr_counter;
			}
			return ret;
		} else {
			if (count >= FLOW_CTRL_RETRY_COUNT) {
				ret = -fc_reg;
				break;
			}
			count++;
			if (count < 30)
				udelay(200);
			else if (count < 40)
				msleep(2);
			else
				msleep(10);
		}
	}

	return ret;
}

int aicwf_sdio_flow_ctrl(struct aic_sdio_dev *sdiodev)
{
	int ret = -1;
	u8 fc_reg = 0;
	u32 count = 0;

	while (true) {
		ret = aicwf_sdio_readb(sdiodev, sdiodev->sdio_reg.flow_ctrl_reg,
				       &fc_reg);
		if (ret) {
			return -1;
		}

		if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
		    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
		    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
			fc_reg = fc_reg & SDIOWIFI_FLOWCTRL_MASK_REG;
		}

		if (fc_reg > DATA_FLOW_CTRL_THRESH) {
			ret = fc_reg;
			if (ret > tx_aggr_counter) {
				ret = tx_aggr_counter;
			}
			return ret;
		} else {
			if (count >= FLOW_CTRL_RETRY_COUNT) {
				ret = -fc_reg;
				break;
			}
			count++;
			if (count < 30)
				udelay(200);
			else if (count < 40)
				msleep(2);
			else
				msleep(10);
		}
	}

	return ret;
}

int aicwf_sdio_send_pkt(struct aic_sdio_dev *sdiodev, u8 *buf, uint count)
{
	int ret = 0;

	sdio_claim_host(sdiodev->func);
	ret = sdio_writesb(sdiodev->func, sdiodev->sdio_reg.wr_fifo_addr, buf,
			   count);
	sdio_release_host(sdiodev->func);

	return ret;
}

#ifdef CONFIG_PREALLOC_RX_SKB
int aicwf_sdio_recv_pkt(struct aic_sdio_dev *sdiodev, struct rx_buff *rxbuff,
			u32 size)
{
	int ret;

	if ((!rxbuff->data) || (!size)) {
		return -EINVAL;
		;
	}

	sdio_claim_host(sdiodev->func);
	ret = sdio_readsb(sdiodev->func, rxbuff->data,
			  sdiodev->sdio_reg.rd_fifo_addr, size);
	sdio_release_host(sdiodev->func);

	if (ret < 0) {
		return ret;
	}
	rxbuff->len = size;

	return ret;
}
#else
int aicwf_sdio_recv_pkt(struct aic_sdio_dev *sdiodev, struct sk_buff *skbbuf,
			u32 size)
{
	int ret;

	if ((!skbbuf) || (!size)) {
		return -EINVAL;
		;
	}

	sdio_claim_host(sdiodev->func);
	ret = sdio_readsb(sdiodev->func, skbbuf->data,
			  sdiodev->sdio_reg.rd_fifo_addr, size);
	sdio_release_host(sdiodev->func);

	if (ret < 0) {
		return ret;
	}
	skbbuf->len = size;

	return ret;
}
#endif

#ifdef CONFIG_GPIO_WAKEUP
static int wakeup_enable;
static u32 hostwake_irq_num;
#endif //CONFIG_GPIO_WAKEUP

#if LINUX_VERSION_CODE >=                                                      \
	KERNEL_VERSION(4, 14,                                                  \
		       0) //LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
//static struct wakeup_source *ws_rx_sdio;
//static struct wakeup_source *ws_sdio_pwrctrl;
//static struct wakeup_source *ws_tx_sdio;
#ifdef CONFIG_GPIO_WAKEUP
//static struct wakeup_source *ws;
#endif
#else
#ifdef ANDROID_PLATFORM
#ifdef CONFIG_GPIO_WAKEUP
#include <linux/wakelock.h>
static struct wake_lock irq_wakelock;
//struct wake_lock irq_wakelock;
#endif //CONFIG_GPIO_WAKEUP
#endif //ANDROID_PLATFORM
#endif

#ifdef CONFIG_PLATFORM_ALLWINNER
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
extern int sunxi_wlan_get_oob_irq(int *, int *);
#else
extern int sunxi_wlan_get_oob_irq(void);
extern int sunxi_wlan_get_oob_irq_flags(void);
#endif
#endif // CONFIG_PLATFORM_ALLWINNER

#if 0
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
static struct wakeup_source *ws;
#else
#ifdef ANDROID_PLATFORM
#ifdef CONFIG_GPIO_WAKEUP
#include <linux/wakelock.h>
static struct wake_lock irq_wakelock;
#endif //CONFIG_GPIO_WAKEUP
#endif //ANDROID_PLATFORM
#endif
#endif

#if 0
void rwnx_pm_stay_awake(struct aic_sdio_dev *sdiodev){

#ifdef CONFIG_GPIO_WAKEUP
	spin_lock_bh(&sdiodev->wslock);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
		if(ws != NULL){
			__pm_stay_awake(ws);
            AICWFDBG(LOGWAKELOCK, "%s active_count:%d relax_count:%d\r\n", __func__, (int)ws->active_count, (int)ws->relax_count);
		}
#else
#ifdef ANDROID_PLATFORM
#ifdef CONFIG_GPIO_WAKEUP
		wake_lock(&irq_wakelock);
#endif //CONFIG_GPIO_WAKEUP
#endif //ANDROID_PLATFORM
#endif

	spin_unlock_bh(&sdiodev->wslock);
#endif
}

void rwnx_pm_relax(struct aic_sdio_dev *sdiodev){

#ifdef CONFIG_GPIO_WAKEUP
	spin_lock_bh(&sdiodev->wslock);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0)
	if(ws != NULL){
		__pm_relax(ws);
        AICWFDBG(LOGWAKELOCK, "%s active_count:%d relax_count:%d\r\n", __func__, (int)ws->active_count, (int)ws->relax_count);
	}
#else
#ifdef ANDROID_PLATFORM
#ifdef CONFIG_GPIO_WAKEUP
	wake_unlock(&irq_wakelock);
#endif //CONFIG_GPIO_WAKEUP
#endif //ANDROID_PLATFORM
#endif
	spin_unlock_bh(&sdiodev->wslock);
#endif

}
#endif

#ifdef CONFIG_GPIO_WAKEUP

void rwnx_set_wifi_suspend(char onoff);

static irqreturn_t rwnx_hostwake_irq_handler(int irq, void *para)
{
	static int wake_cnt;
	wake_cnt++;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	rwnx_wakeup_lock_timeout(g_rwnx_plat->sdiodev->rwnx_hw->ws_rx, 1000);
#else
#ifdef ANDROID_PLATFORM
	wake_lock_timeout(&irq_wakelock, HZ);
#endif //ANDROID_PLATFORM
#endif

	AICWFDBG(LOGIRQ, "%s(%d): wake_irq_cnt = %d\n", __func__, __LINE__,
		 wake_cnt);

#ifdef CONFIG_OOB
#if 0 //old oob feature
    complete(&g_rwnx_plat->sdiodev->bus_if->busrx_trgg);
#else //new oob feature
	if (g_rwnx_plat->sdiodev->oob_enable) {
		complete(&g_rwnx_plat->sdiodev->bus_if->busirq_trgg);
	}
#endif //old oob feature
#endif

	return IRQ_HANDLED;
}
#endif //CONFIG_GPIO_WAKEUP

#ifdef CONFIG_GPIO_WAKEUP
static int rwnx_disable_hostwake_irq(void);
static int rwnx_enable_hostwake_irq(void);
#endif

static int rwnx_register_hostwake_irq(struct device *dev)
{
	int ret = 0; //-1;
#ifdef CONFIG_GPIO_WAKEUP
	unsigned long flag_edge;
	struct aicbsp_feature_t aicwf_feature;
	int irq_flags;
	//TODO hostwake_irq_num hostwake_irq_num and wakeup_enable

	aicbsp_get_feature(&aicwf_feature, NULL);
	if (aicwf_feature.irqf == 0)
		flag_edge = IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND;
	else
		flag_edge = IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND;

#ifdef CONFIG_PLATFORM_ALLWINNER
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
	hostwake_irq_num = sunxi_wlan_get_oob_irq(&irq_flags, &wakeup_enable);
#else
	hostwake_irq_num = sunxi_wlan_get_oob_irq();
	irq_flags = sunxi_wlan_get_oob_irq_flags();
	wakeup_enable = 1;
#endif
#endif //CONFIG_PLATFORM_ALLWINNER

//For Rockchip
#ifdef CONFIG_PLATFORM_ROCKCHIP
	hostwake_irq_num = rockchip_wifi_get_oob_irq();
	printk("%s hostwake_irq_num:%d \r\n", __func__, hostwake_irq_num);
	irq_flags = (IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL |
		     IORESOURCE_IRQ_SHAREABLE) &
		    IRQF_TRIGGER_MASK;
	printk("%s irq_flags:%d \r\n", __func__, irq_flags);
	wakeup_enable = 1;
#endif //CONFIG_PLATFORM_ROCKCHIP
	//For Rockchip
#ifdef CONFIG_PLATFORM_ROCKCHIP2
	hostwake_irq_num = rockchip_wifi_get_oob_irq();
	printk("%s hostwake_irq_num:%d \r\n", __func__, hostwake_irq_num);
	irq_flags = (IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL |
		     IORESOURCE_IRQ_SHAREABLE) &
		    IRQF_TRIGGER_MASK;
	printk("%s irq_flags:%d \r\n", __func__, irq_flags);
	wakeup_enable = 1;
#endif //CONFIG_PLATFORM_ROCKCHIP

	if (wakeup_enable) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
		//ws = wakeup_source_register(dev, "wifisleep");
		//ws_tx_sdio = wakeup_source_register(dev, "wifi_tx_sleep");
		//ws_rx_sdio = wakeup_source_register(dev, "wifi_rx_sleep");
		//ws_sdio_pwrctrl = wakeup_source_register(dev, "sdio_pwrctrl_sleep");
#else
#ifdef ANDROID_PLATFORM
		wake_lock_init(&irq_wakelock, WAKE_LOCK_SUSPEND, "wifisleep");
#endif
#endif
		ret = device_init_wakeup(dev, true);
		if (ret < 0) {
			pr_err("%s(%d): device init wakeup failed!\n", __func__,
			       __LINE__);
			return ret;
		}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
		ret = dev_pm_set_wake_irq(dev, hostwake_irq_num);
#endif
		if (ret < 0) {
			pr_err("%s(%d): can't enable wakeup src!\n", __func__,
			       __LINE__);
			goto fail1;
		}

		ret = request_irq(hostwake_irq_num, rwnx_hostwake_irq_handler,
				  flag_edge, "rwnx_hostwake_irq", NULL);

		if (ret < 0) {
			pr_err("%s(%d): request_irq fail! ret = %d\n", __func__,
			       __LINE__, ret);
			goto fail2;
		}
	}
	//disable_irq(hostwake_irq_num);
	rwnx_disable_hostwake_irq();
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
	dev_pm_clear_wake_irq(dev);
#endif
	rwnx_enable_hostwake_irq();
	AICWFDBG(LOGINFO, "%s(%d)\n", __func__, __LINE__);
	return ret;

fail2:
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
	dev_pm_clear_wake_irq(dev);
#endif
fail1:
	device_init_wakeup(dev, false);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	//wakeup_source_unregister(ws);
	//wakeup_source_unregister(ws_tx_sdio);
	//wakeup_source_unregister(ws_rx_sdio);
	//wakeup_source_unregister(ws_sdio_pwrctrl);
#else
#ifdef ANDROID_PLATFORM
	wake_lock_destroy(&irq_wakelock);
#endif
#endif
#endif //CONFIG_GPIO_WAKEUP
	return ret;
}

static int rwnx_unregister_hostwake_irq(struct device *dev)
{
#ifdef CONFIG_GPIO_WAKEUP
	rwnx_disable_hostwake_irq();
	if (wakeup_enable) {
		device_init_wakeup(dev, false);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0)
		dev_pm_clear_wake_irq(dev);
#else
		AICWFDBG(LOGERROR, "%s kernel unsupport this feature!\r\n",
			 __func__);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
		//wakeup_source_unregister(ws);
		//wakeup_source_unregister(ws_tx_sdio);
		//wakeup_source_unregister(ws_rx_sdio);
		//wakeup_source_unregister(ws_sdio_pwrctrl);
#else
#ifdef ANDROID_PLATFORM
		wake_lock_destroy(&irq_wakelock);
#endif //ANDROID_PLATFORM
#endif
	}
	free_irq(hostwake_irq_num, NULL);
#endif //CONFIG_GPIO_WAKEUP
	AICWFDBG(LOGINFO, "%s(%d)\n", __func__, __LINE__);
	return 0;
}

#ifdef CONFIG_GPIO_WAKEUP
static int rwnx_enable_hostwake_irq(void)
{
#ifdef CONFIG_GPIO_WAKEUP
	enable_irq(hostwake_irq_num);
	enable_irq_wake(hostwake_irq_num);
#endif //CONFIG_GPIO_WAKEUP
	AICWFDBG(LOGINFO, "%s(%d)\n", __func__, __LINE__);
	return 0;
}

static int rwnx_disable_hostwake_irq(void)
{
	AICWFDBG(LOGINFO, "%s(%d)\n", __func__, __LINE__);
#ifdef CONFIG_GPIO_WAKEUP
	disable_irq_nosync(hostwake_irq_num);
	//disable_irq_wake(hostwake_irq_num);
	//disable_irq(hostwake_irq_num);
#endif //CONFIG_GPIO_WAKEUP
	return 0;
}
#endif

static int aicwf_sdio_chipmatch(struct aic_sdio_dev *sdio_dev, u16_l vid,
				u16_l did)
{
	if (vid == SDIO_VENDOR_ID_AIC8801 && did == SDIO_DEVICE_ID_AIC8801) {
		sdio_dev->chipid = PRODUCT_ID_AIC8801;
		AICWFDBG(LOGINFO, "%s USE AIC8801\r\n", __func__);
		return 0;
	} else if (vid == SDIO_VENDOR_ID_AIC8800DC &&
		   did == SDIO_DEVICE_ID_AIC8800DC) {
		sdio_dev->chipid = PRODUCT_ID_AIC8800DC;
		AICWFDBG(LOGINFO, "%s USE AIC8800DC\r\n", __func__);
		return 0;
	} else if (vid == SDIO_VENDOR_ID_AIC8800D80 &&
		   did == SDIO_DEVICE_ID_AIC8800D80) {
		sdio_dev->chipid = PRODUCT_ID_AIC8800D80;
		AICWFDBG(LOGINFO, "%s USE AIC8800D80\r\n", __func__);
		return 0;
	} else if (vid == SDIO_VENDOR_ID_AIC8800D80X2 &&
		   did == SDIO_DEVICE_ID_AIC8800D80X2) {
		sdio_dev->chipid = PRODUCT_ID_AIC8800D80X2;
		AICWFDBG(LOGINFO, "%s USE AIC8800D80X2\r\n", __func__);
		return 0;
	} else {
		return -1;
	}
}

extern int rwnx_send_me_set_lp_level(struct rwnx_hw *rwnx_hw, u8 lp_level,
				     u8 disable_filter);

static int aicwf_sdio_probe(struct sdio_func *func,
			    const struct sdio_device_id *id)
{
	struct mmc_host *host;
	struct aic_sdio_dev *sdiodev;
	struct aicwf_bus *bus_if;
	int err = -ENODEV;

	AICWFDBG(LOGDEBUG, "%s:%d\n", __func__, func->num);
	AICWFDBG(LOGDEBUG, "Class=%x\n", func->class);
	AICWFDBG(LOGDEBUG, "sdio vendor ID: 0x%04x\n", func->vendor);
	AICWFDBG(LOGDEBUG, "sdio device ID: 0x%04x\n", func->device);
	AICWFDBG(LOGDEBUG, "Function#: %d\n", func->num);

	host = func->card->host;
	if (func->num != 1) {
		return err;
	}

	bus_if = kzalloc(sizeof(struct aicwf_bus), GFP_KERNEL);
	if (!bus_if) {
		sdio_err("alloc bus fail\n");
		return -ENOMEM;
	}

	sdiodev = kzalloc(sizeof(struct aic_sdio_dev), GFP_KERNEL);
	if (!sdiodev) {
		sdio_err("alloc sdiodev fail\n");
		kfree(bus_if);
		return -ENOMEM;
	}

	err = aicwf_sdio_chipmatch(sdiodev, func->vendor, func->device);

	sdiodev->func = func;
	sdiodev->bus_if = bus_if;

#ifdef CONFIG_OOB
	if (sdiodev->chipid == PRODUCT_ID_AIC8801) {
		AICWFDBG(LOGERROR, "%s ERROR!!! 8801 not support OOB \r\n",
			 __func__);
		sdiodev->oob_enable = false;
	} else {
		sdiodev->oob_enable = true;
	}
#else
	sdiodev->oob_enable = false;
#endif

	atomic_set(&sdiodev->is_bus_suspend, 0);
	bus_if->bus_priv.sdio = sdiodev;

	dev_set_drvdata(&func->dev, bus_if);
	sdiodev->dev = &func->dev;

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		sdiodev->func2 = func->card->sdio_func[1];
	}

	//sdio func init start
	if (sdiodev->chipid != PRODUCT_ID_AIC8800D80 &&
	    sdiodev->chipid != PRODUCT_ID_AIC8800D80X2) {
		err = aicwf_sdio_func_init(sdiodev);
	} else {
		err = aicwf_sdiov3_func_init(sdiodev);
	}
	if (err < 0) {
		sdio_err("sdio func init fail\n");
		goto fail;
	}
	//sdio func init end

	if (aicwf_sdio_bus_init(sdiodev) == NULL) {
		sdio_err("sdio bus init fail\n");
		err = -1;
		goto fail;
	}

	host->caps |= MMC_CAP_NONREMOVABLE;
	aicwf_rwnx_sdio_platform_init(sdiodev);
	aicwf_hostif_ready();
	err = rwnx_register_hostwake_irq(sdiodev->dev);
	if (err != 0)
		return err;

#ifdef CONFIG_GPIO_WAKEUP
#ifdef CONFIG_OOB
	if (sdiodev->oob_enable) {
		AICWFDBG(LOGINFO, "%s SDIOWIFI_INTR_CONFIG_REG Disable\n",
			 __func__);
		sdio_claim_host(sdiodev->func);
		//disable sdio interrupt
		err = aicwf_sdio_writeb(sdiodev, SDIOWIFI_INTR_CONFIG_REG, 0x0);
		if (err < 0) {
			sdio_err("reg:%d write failed!\n",
				 SDIOWIFI_INTR_CONFIG_REG);
		}
		sdio_release_irq(sdiodev->func);
		sdio_release_host(sdiodev->func);
#if 0
#if 0 //old oob feature
        sdiodev->oob_enable = true;
#else //new oob feature
        sdiodev->oob_enable = true;
#endif //old oob feature
#endif
	}
#endif

#ifdef CONFIG_WIFI_SUSPEND_FOR_LINUX
	rwnx_init_wifi_suspend_node();
#endif //CONFIG_WIFI_SUSPEND_FOR_LINUX
#endif //CONFIG_GPIO_WAKEUP
	device_disable_async_suspend(sdiodev->dev);

	return 0;
fail:
	aicwf_sdio_func_deinit(sdiodev);
	dev_set_drvdata(&func->dev, NULL);
	kfree(sdiodev);
	kfree(bus_if);
	aicwf_hostif_fail();
	return err;
}

void aicwf_sdio_probe_(struct sdio_func *func, const struct sdio_device_id *id)
{
	aicwf_sdio_probe(func, NULL);
}

static void aicwf_sdio_remove(struct sdio_func *func)
{
	struct mmc_host *host;
	struct aicwf_bus *bus_if = NULL;
	struct aic_sdio_dev *sdiodev = NULL;

	AICWFDBG(LOGINFO, "%s Enter\n", __func__);
	host = func->card->host;
	host->caps &= ~MMC_CAP_NONREMOVABLE;
	bus_if = dev_get_drvdata(&func->dev);
	if (!bus_if) {
		return;
	}

	sdiodev = bus_if->bus_priv.sdio;
	if (!sdiodev) {
		return;
	}

	sdiodev->bus_if->state = BUS_DOWN_ST;
	aicwf_sdio_release(sdiodev);
	aicwf_sdio_func_deinit(sdiodev);
	rwnx_unregister_hostwake_irq(sdiodev->dev);
	dev_set_drvdata(&sdiodev->func->dev, NULL);
	kfree(sdiodev);
	kfree(bus_if);
#ifdef CONFIG_WIFI_SUSPEND_FOR_LINUX
	rwnx_deinit_wifi_suspend_node();
#endif //CONFIG_WIFI_SUSPEND_FOR_LINUX
	AICWFDBG(LOGINFO, "%s done\n", __func__);
}

void aicwf_sdio_remove_(struct sdio_func *func)
{
	aicwf_sdio_remove(func);
}

#if defined(CONFIG_PLATFORM_ROCKCHIP) || defined(CONFIG_PLATFORM_ROCKCHIP2)
#ifdef CONFIG_SHUTDOWN_CALLBACK
int rwnx_close_(struct net_device *dev);

void aicwf_sdio_shutdown(struct device *dev)
{
	struct rwnx_vif *rwnx_vif, *tmp;
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;

	AICWFDBG(LOGINFO, "%s Enter", __func__);
	list_for_each_entry_safe (rwnx_vif, tmp, &sdiodev->rwnx_hw->vifs,
				  list) {
		if (rwnx_vif->ndev &&
		    test_bit(RWNX_DEV_STARTED, &rwnx_vif->drv_flags)) {
			AICWFDBG(LOGINFO, "%s rwnx_close by shutdown",
				 __func__);
			rwnx_close_(rwnx_vif->ndev);

		} else {
			if (!rwnx_vif->ndev) {
				AICWFDBG(LOGERROR,
					 "%s rwnx_vif->ndev is NULL \r\n",
					 __func__);
			}
			if (!test_bit(RWNX_DEV_STARTED, &rwnx_vif->drv_flags)) {
				AICWFDBG(LOGERROR,
					 "%s rwnx_vif->drv_flags close\r\n",
					 __func__);
			}
		}
	}
	AICWFDBG(LOGINFO, "%s Exit", __func__);
}
#endif
#endif

#if defined(CONFIG_AUTO_POWERSAVE)
static int aicwf_wakeup_lock_status(struct rwnx_hw *rwnx_hw)
{
	if (rwnx_hw->ws_tx && rwnx_hw->ws_tx->active)
		return -1;

	if (rwnx_hw->ws_rx && rwnx_hw->ws_rx->active)
		return -1;

	if (rwnx_hw->ws_pwrctrl && rwnx_hw->ws_pwrctrl->active)
		return -1;

	if (rwnx_hw->ws_irqrx && rwnx_hw->ws_irqrx->active)
		return -1;

	return 0;
}
#endif

static int aicwf_sdio_suspend(struct device *dev)
{
	int ret = 0;
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
	mmc_pm_flag_t sdio_flags;
	struct rwnx_vif *rwnx_vif, *tmp;

	sdio_dbg("%s enter\n", __func__);

	list_for_each_entry_safe (rwnx_vif, tmp, &sdiodev->rwnx_hw->vifs,
				  list) {
		if (rwnx_vif->ndev)
			netif_device_detach(rwnx_vif->ndev);
	}

	sdio_flags = sdio_get_host_pm_caps(sdiodev->func);
	if (!(sdio_flags & MMC_PM_KEEP_POWER)) {
		return -EINVAL;
	}
	ret = sdio_set_host_pm_flags(sdiodev->func, MMC_PM_KEEP_POWER);
	if (ret) {
		return ret;
	}

#ifdef CONFIG_TEMP_CONTROL
	del_timer_sync(&sdiodev->tp_ctrl_timer);
	cancel_work_sync(&sdiodev->tp_ctrl_work);

	mod_timer(&sdiodev->tp_ctrl_timer,
		  jiffies + msecs_to_jiffies(TEMP_GET_INTERVAL));

	del_timer_sync(&sdiodev->netif_timer);
	cancel_work_sync(&sdiodev->netif_work);
#endif

#if (defined(CONFIG_AUTO_POWERSAVE) && defined(CONFIG_SDIO_PWRCTRL))
	aicwf_sdio_pwr_stctl(sdiodev, SDIO_ACTIVE_ST);

	if ((sdiodev->chipid == PRODUCT_ID_AIC8800D80) ||
	    (sdiodev->chipid == PRODUCT_ID_AIC8800D80X2)) {
		sdio_dbg("autops set\n");
		ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.wakeup_reg,
					0x8);
		if (ret) {
			sdio_err("sdio set autops fail\n");
		}
	}
#endif

#if (!defined(CONFIG_AUTO_POWERSAVE))
	while (sdiodev->state == SDIO_ACTIVE_ST) {
		if (down_interruptible(&sdiodev->tx_priv->txctl_sema))
			continue;
#if defined(CONFIG_SDIO_PWRCTRL)
		aicwf_sdio_pwr_stctl(sdiodev, SDIO_SLEEP_ST);
#endif
		up(&sdiodev->tx_priv->txctl_sema);
		break;
	}
#else
#if defined(CONFIG_SDIO_PWRCTRL)
	aicwf_sdio_pwr_stctl(sdiodev, SDIO_SLEEP_ST);
#endif
#endif
#ifdef CONFIG_GPIO_WAKEUP
//	rwnx_enable_hostwake_irq();
#endif

#if defined(CONFIG_PLATFORM_ROCKCHIP) || defined(CONFIG_PLATFORM_ROCKCHIP2)
	if (sdiodev->chipid == PRODUCT_ID_AIC8801) {
		sdio_dbg("%s SDIOWIFI_INTR_CONFIG_REG Disable\n", __func__);
		sdio_claim_host(sdiodev->func);
		//disable sdio interrupt
		ret = aicwf_sdio_writeb(sdiodev, SDIOWIFI_INTR_CONFIG_REG, 0x0);
		if (ret < 0) {
			sdio_err("reg:%d write failed!\n",
				 SDIOWIFI_INTR_CONFIG_REG);
		}
		sdio_release_irq(sdiodev->func);
		sdio_release_host(sdiodev->func);
	}
#endif
	atomic_set(&sdiodev->is_bus_suspend, 1);
	//    smp_mb();

#if defined(CONFIG_AUTO_POWERSAVE)
	if (aicwf_wakeup_lock_status(sdiodev->rwnx_hw)) {
		printk("%s ws active dont suspend", __func__);
		aicwf_sdio_pwr_stctl(sdiodev, SDIO_ACTIVE_ST);

		if ((sdiodev->chipid == PRODUCT_ID_AIC8800D80) ||
		    (sdiodev->chipid == PRODUCT_ID_AIC8800D80X2)) {
			sdio_dbg("autops clear\n");
			ret = aicwf_sdio_writeb(
				sdiodev, sdiodev->sdio_reg.wakeup_reg, 0x8);
			if (ret) {
				sdio_err("sdio clear autops fail\n");
			}
		}

		return -EBUSY;
	}
#endif

	sdio_dbg("%s exit\n", __func__);

	return 0;
}

static int aicwf_sdio_resume(struct device *dev)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
	struct rwnx_vif *rwnx_vif, *tmp;
#if defined(CONFIG_PLATFORM_ROCKCHIP) || defined(CONFIG_PLATFORM_ROCKCHIP2) || \
	defined(CONFIG_AUTO_POWERSAVE)
	int ret;
#endif

	sdio_dbg("%s enter \n", __func__);

#ifdef CONFIG_TEMP_CONTROL
	mod_timer(&sdiodev->tp_ctrl_timer,
		  jiffies + msecs_to_jiffies(TEMP_GET_INTERVAL));
#endif

	//#ifdef CONFIG_GPIO_WAKEUP
	//	rwnx_disable_hostwake_irq();
	//#endif
	//dev_pm_clear_wake_irq(dev);
	list_for_each_entry_safe (rwnx_vif, tmp, &sdiodev->rwnx_hw->vifs,
				  list) {
		if (rwnx_vif->ndev)
			netif_device_attach(rwnx_vif->ndev);
	}

#if defined(CONFIG_SDIO_PWRCTRL)
	aicwf_sdio_pwr_stctl(sdiodev, SDIO_ACTIVE_ST);
#endif

#if defined(CONFIG_AUTO_POWERSAVE) && defined(CONFIG_SDIO_PWRCTRL)
	if (sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		sdio_dbg("autops clear\n");
		ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.wakeup_reg,
					0x8);
		if (ret) {
			sdio_err("sdio clear autops fail\n");
		}
	}
#endif

	//	aicwf_sdio_hal_irqhandler(sdiodev->func);

#if defined(CONFIG_PLATFORM_ROCKCHIP) || defined(CONFIG_PLATFORM_ROCKCHIP2)
	if (sdiodev->chipid == PRODUCT_ID_AIC8801) {
		sdio_dbg("%s SDIOWIFI_INTR_CONFIG_REG Enable\n", __func__);
		sdio_claim_host(sdiodev->func);
		sdio_claim_irq(sdiodev->func, aicwf_sdio_hal_irqhandler);

		//enable sdio interrupt
		ret = aicwf_sdio_writeb(sdiodev, SDIOWIFI_INTR_CONFIG_REG,
					0x07);
		if (ret != 0)
			sdio_err("intr register failed:%d\n", ret);
		sdio_release_host(sdiodev->func);
	}
#endif
	atomic_set(&sdiodev->is_bus_suspend, 0);
	//    smp_mb();
#ifdef CONFIG_WIFI_SUSPEND_FOR_LINUX
	rwnx_set_wifi_suspend('0');
#endif //CONFIG_WIFI_SUSPEND_FOR_LINUX

	sdio_dbg("%s exit\n", __func__);
	return 0;
}

static const struct sdio_device_id aicwf_sdmmc_ids[] = {
	{ SDIO_DEVICE(SDIO_VENDOR_ID_AIC8801, SDIO_DEVICE_ID_AIC8801) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_AIC8800DC, SDIO_DEVICE_ID_AIC8800DC) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_AIC8800D80, SDIO_DEVICE_ID_AIC8800D80) },
	{ SDIO_DEVICE(SDIO_VENDOR_ID_AIC8800D80X2,
		      SDIO_DEVICE_ID_AIC8800D80X2) },
	{},
};

MODULE_DEVICE_TABLE(sdio, aicwf_sdmmc_ids);

static const struct dev_pm_ops aicwf_sdio_pm_ops = { SET_SYSTEM_SLEEP_PM_OPS(
	aicwf_sdio_suspend, aicwf_sdio_resume) };

#ifndef CONFIG_FDRV_NO_REG_SDIO
static struct sdio_driver aicwf_sdio_driver = {
	.probe = aicwf_sdio_probe,
	.remove = aicwf_sdio_remove,
	.name = AICWF_SDIO_NAME,
	.id_table = aicwf_sdmmc_ids,
	.drv = {
		.pm = &aicwf_sdio_pm_ops,
#if defined(CONFIG_PLATFORM_ROCKCHIP) || defined(CONFIG_PLATFORM_ROCKCHIP2)
#ifdef CONFIG_SHUTDOWN_CALLBACK
		.shutdown = aicwf_sdio_shutdown,
#endif
#endif
	},
};
#endif

#if 0
#ifdef CONFIG_NANOPI_M4
extern int mmc_rescan_try_freq(struct mmc_host *host, unsigned freq);
extern unsigned  aic_max_freqs;
extern struct mmc_host *aic_host_drv;
extern int __mmc_claim_host(struct mmc_host *host, atomic_t *abort);
extern void mmc_release_host(struct mmc_host *host);
#endif
#endif

#ifdef CONFIG_FDRV_NO_REG_SDIO
extern struct sdio_func *get_sdio_func(void);
void aicwf_sdio_probe_(struct sdio_func *func, const struct sdio_device_id *id);
void aicwf_sdio_remove_(struct sdio_func *func);
#endif

void aicwf_sdio_register(void)
{
#if 0
#ifdef CONFIG_PLATFORM_NANOPI
	extern_wifi_set_enable(0);
	mdelay(200);
	extern_wifi_set_enable(1);
	mdelay(200);
	sdio_reinit();
#endif /*CONFIG_PLATFORM_NANOPI*/

#ifdef CONFIG_PLATFORM_ROCKCHIP
	rockchip_wifi_power(0);
	mdelay(200);
	rockchip_wifi_power(1);
	mdelay(200);
	rockchip_wifi_set_carddetect(1);
#endif /*CONFIG_PLATFORM_ROCKCHIP*/

#ifdef CONFIG_INGENIC_T20
	jzmmc_manual_detect(1, 1);
#endif /* CONFIG_INGENIC_T20 */

#ifdef CONFIG_NANOPI_M4
	if (aic_host_drv->card == NULL) {
		__mmc_claim_host(aic_host_drv, NULL);
		printk("aic: >>>mmc_rescan_try_freq\n");
		mmc_rescan_try_freq(aic_host_drv, aic_max_freqs);
		mmc_release_host(aic_host_drv);
	}
#endif
#endif

#ifndef CONFIG_FDRV_NO_REG_SDIO
	if (sdio_register_driver(&aicwf_sdio_driver)) {
	} else {
		//may add mmc_rescan here
	}
#else
	aicwf_sdio_probe_(get_sdio_func(), NULL);
#endif
}

void aicwf_sdio_exit(void)
{
	if (g_rwnx_plat && g_rwnx_plat->enabled) {
#ifdef CONFIG_TEMP_CONTROL
		if (timer_pending(&g_rwnx_plat->sdiodev->tp_ctrl_timer)) {
			AICWFDBG(LOGINFO, "%s del tp_ctrl_timer\n", __func__);
			del_timer_sync(&g_rwnx_plat->sdiodev->tp_ctrl_timer);
		}
		cancel_work_sync(&g_rwnx_plat->sdiodev->tp_ctrl_work);

		if (timer_pending(&g_rwnx_plat->sdiodev->netif_timer)) {
			AICWFDBG(LOGINFO, "%s del netif_timer\n", __func__);
			del_timer_sync(&g_rwnx_plat->sdiodev->netif_timer);
		}
		cancel_work_sync(&g_rwnx_plat->sdiodev->netif_work);
#endif
		rwnx_platform_deinit(g_rwnx_plat->sdiodev->rwnx_hw);
	} else {
		AICWFDBG(LOGERROR, "%s g_rwnx_plat is not ready \r\n",
			 __func__);
	}

	udelay(500);

#ifndef CONFIG_FDRV_NO_REG_SDIO
	sdio_unregister_driver(&aicwf_sdio_driver);
#else
	aicwf_sdio_remove_(get_sdio_func());
#endif

#if 0
#ifdef CONFIG_PLATFORM_AMLOGIC
	extern_wifi_set_enable(0);
#endif /*CONFIG_PLATFORM_AMLOGIC*/
#endif

#if 0
#ifdef CONFIG_PLATFORM_ROCKCHIP
	rockchip_wifi_set_carddetect(0);
	mdelay(200);
	rockchip_wifi_power(0);
	mdelay(200);
#endif /*CONFIG_PLATFORM_ROCKCHIP*/
#endif

	if (g_rwnx_plat) {
		kfree(g_rwnx_plat);
	}
}

#if defined(CONFIG_SDIO_PWRCTRL)
int aicwf_sdio_wakeup(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;
	int read_retry;
	int write_retry = 1;
	int wakeup_reg_val = 0;

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		wakeup_reg_val = 1;
	} else if (sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
		   sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		wakeup_reg_val = 0x11;
	}

	if (sdiodev->state == SDIO_SLEEP_ST) {
		AICWFDBG(LOGSDPWRC, "%s w\n", __func__);

		//rwnx_pm_stay_awake(sdiodev);

		while (write_retry) {
			ret = aicwf_sdio_writeb(sdiodev,
						sdiodev->sdio_reg.wakeup_reg,
						wakeup_reg_val);
			if (ret) {
				txrx_err("sdio wakeup fail\n");
				ret = -1;
			} else {
				read_retry = 50;
				while (read_retry) {
					u8 val;
					ret = aicwf_sdio_readb(
						sdiodev,
						sdiodev->sdio_reg.wakeup_reg,
						&val);
					if (ret < 0)
						txrx_err(
							"sdio wakeup read fail\n");
					else if ((val & 0x1) == 0) {
						break;
					}
					read_retry--;
					udelay(200);
				}
				if (read_retry != 0)
					break;
			}
			sdio_dbg("write retry:  %d \n", write_retry);
			write_retry--;
			udelay(100);
		}

		sdiodev->state = SDIO_ACTIVE_ST;
		aicwf_sdio_pwrctl_timer(sdiodev, sdiodev->active_duration);
	}
	return ret;
}

int aicwf_sdio_sleep_allow(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;
	struct aicwf_bus *bus_if = sdiodev->bus_if;
	struct rwnx_hw *rwnx_hw = sdiodev->rwnx_hw;
	u8 read_retry;
	u8 val;

	if (bus_if->state == BUS_DOWN_ST) {
		ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.sleep_reg,
					0x10);
		if (ret) {
			sdio_err("Write sleep fail!\n");
		}
		aicwf_sdio_pwrctl_timer(sdiodev, 0);
		return ret;
	}

	sdio_info("sleep: %d, %d\n", sdiodev->state, scanning);
	if (sdiodev->state == SDIO_ACTIVE_ST && !scanning &&
	    !rwnx_hw->is_p2p_alive && !rwnx_hw->is_p2p_connected &&
	    (int)(atomic_read(&sdiodev->tx_priv->tx_pktcnt) <= 0) &&
	    (sdiodev->tx_priv->cmd_txstate == false) &&
	    (int)(atomic_read(&sdiodev->rx_priv->rx_cnt) == 0)) {
		AICWFDBG(LOGSDPWRC, "%s s\n", __func__);
		if (sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
		    sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
			if (aicwf_sdio_writeb(sdiodev,
					      sdiodev->sdio_reg.wakeup_reg,
					      0x02) < 0) {
				sdio_err("reg:%d write failed!\n",
					 sdiodev->sdio_reg.wakeup_reg);
			}
		} else if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
			   sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
			   sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
			if (aicwf_sdio_func2_writeb(
				    sdiodev, sdiodev->sdio_reg.wakeup_reg,
				    0x2) < 0) {
				sdio_err("reg:%d write failed!\n",
					 sdiodev->sdio_reg.wakeup_reg);
			}
			read_retry = 100;
			while (read_retry) {
				val = 0;
				if (aicwf_sdio_func2_readb(
					    sdiodev,
					    sdiodev->sdio_reg.wakeup_reg,
					    &val) < 0) {
					sdio_err("reg %d read fail\n",
						 sdiodev->sdio_reg.wakeup_reg);
				} else if ((val & 0x2) == 0) {
					break;
				} else {
					sdio_err("val:%d\n", val);
				}
				read_retry--;
				if (read_retry < 90)
					printk("warning: read cnt %d\n",
					       read_retry);
				udelay(500);
			}
		}
		sdiodev->state = SDIO_SLEEP_ST;
		aicwf_sdio_pwrctl_timer(sdiodev, 0);
		//rwnx_pm_relax(sdiodev);
	} else {
		aicwf_sdio_pwrctl_timer(sdiodev, sdiodev->active_duration);
	}

	return ret;
}

int aicwf_sdio_pwr_stctl(struct aic_sdio_dev *sdiodev, uint target)
{
	int ret = 0;

	if (sdiodev->bus_if->state == BUS_DOWN_ST) {
		return -1;
	}

	down(&sdiodev->pwrctl_wakeup_sema);

	if (sdiodev->state == target) {
		if (target == SDIO_ACTIVE_ST) {
			aicwf_sdio_pwrctl_timer(sdiodev,
						sdiodev->active_duration);
		}
		up(&sdiodev->pwrctl_wakeup_sema);
		return ret;
	}

	switch (target) {
	case SDIO_ACTIVE_ST:
		aicwf_sdio_wakeup(sdiodev);
		break;
	case SDIO_SLEEP_ST:
		aicwf_sdio_sleep_allow(sdiodev);
		break;
	}

	up(&sdiodev->pwrctl_wakeup_sema);
	return ret;
}
#endif

#if 0
int align_param = 16;
module_param(align_param, int, 0660);
#endif

int aicwf_sdio_txpkt(struct aic_sdio_dev *sdiodev, struct sk_buff *pkt)
{
	int ret = 0;
	u8 *frame;
	u32 len = 0;
	struct aicwf_bus *bus_if = dev_get_drvdata(sdiodev->dev);
#if 0
	int align = 0;
#endif
	if (bus_if->state == BUS_DOWN_ST) {
		sdio_dbg("tx bus is down!\n");
		return -EINVAL;
	}

#if 0
	len = pkt->len;
	len = (len + SDIOWIFI_FUNC_BLOCKSIZE - 1) / SDIOWIFI_FUNC_BLOCKSIZE * SDIOWIFI_FUNC_BLOCKSIZE;

	frame = (u8*)kmalloc(sizeof(u8) * len + align_param, GFP_ATOMIC);
	align = ((unsigned long)(frame)) & (align_param - 1);
	memcpy(frame + (align_param - align), (u8 *) (pkt->data), len);

	ret = aicwf_sdio_send_pkt(sdiodev, frame + (align_param - align), len);
	if (ret)
		sdio_err("aicwf_sdio_send_pkt fail%d\n", ret);

	kfree(frame);
#endif
#if 1
	frame = (u8 *)(pkt->data);
	len = pkt->len;
	len = (len + SDIOWIFI_FUNC_BLOCKSIZE - 1) / SDIOWIFI_FUNC_BLOCKSIZE *
	      SDIOWIFI_FUNC_BLOCKSIZE;

	ret = aicwf_sdio_send_pkt(sdiodev, pkt->data, len);
	if (ret)
		sdio_err("aicwf_sdio_send_pkt fail%d\n", ret);
#endif
	return ret;
}

#ifdef CONFIG_SDIO_ADMA
int aicwf_sdio_txscatterpkt(struct aicwf_tx_priv *tx_priv)
{
	int ret = 0;
	u8 *frame;
	u32 len = 0;
	struct aicwf_bus *bus_if = dev_get_drvdata(tx_priv->sdiodev->dev);
	struct aic_sdio_dev *sdiodev = tx_priv->sdiodev;
	int nents = tx_priv->aggr_segcnt;
	int i = 0;

	if (bus_if->state == BUS_DOWN_ST) {
		sdio_dbg("tx bus is down!\n");
		return -EINVAL;
	}

	//AICWFDBG(LOGTRACE,"%s aggr_segcnt %d len %d \n",__func__,tx_priv->aggr_segcnt, tx_priv->len);
	sdio_claim_host(sdiodev->func);
	ret = sdio_write_sg(sdiodev->func, sdiodev->sdio_reg.wr_fifo_addr,
			    &(tx_priv->sg_list[0]), tx_priv->aggr_segcnt,
			    tx_priv->len); //&(tx_priv->sg_list[0])
	//sdio_writesb(sdiodev->func, sdiodev->sdio_reg.wr_fifo_addr, tx_priv->sg_list[0].buf, tx_priv->sg_list[0].len);
	/*for(i=0;i<1;i++){
		sdio_writesb(sdiodev->func, sdiodev->sdio_reg.wr_fifo_addr, tx_priv->sg_list[i].buf, tx_priv->sg_list[i].len);
	}*/
	sdio_release_host(sdiodev->func);

	return ret;
}

void aicwf_adma_add(struct aicwf_tx_priv *tx_priv, struct sk_buff *pkt)
{
	u8 adj_len = 0;
	u8 *sdio_header = NULL;
	u32 sdio_len = 0;
	u32 curr_len = 0;
	unsigned int align_len = 0;
	u8 *start_ptr = NULL;

	adj_len = ALIGN4_ADJ_LEN((uint32_t)pkt->data);
	start_ptr = skb_push(pkt, adj_len); //skb_push(pkt,adj_len)
	//ASSERT((adj_len==0)||(adj_len==2));        // mgmt-pkt or data-pkt
	//ASSERT((((uint32_t)start_ptr)&3) == 0);    // start_ptr addr word align
	if (!((adj_len == 0) || (adj_len == 2)))
		AICWFDBG(LOGTRACE, "adj_len false %u \n", adj_len);

	if ((((uint32_t)start_ptr) & 3) != 0)
		AICWFDBG(LOGTRACE, "start_ptr %x \n",
			 (((uint32_t)start_ptr) & 3));

	//AICWFDBG(LOGTRACE,"start adj_len %d aggrcnt %d segcnt %d len %d \n",adj_len,atomic_read(&tx_priv->aggr_count),tx_priv->aggr_segcnt,tx_priv->len);

	if (adj_len == 0) {
		sdio_len = pkt->len + SDIO_MGMT_FAKE_LEN;
		if (sdio_len & (TX_ALIGNMENT - 1)) {
			align_len =
				TX_ALIGNMENT - (sdio_len & (TX_ALIGNMENT - 1));
			//#ifdef CONFIG_SDIO_ADMA_ADJ
			//memset(pkt->data + sdio_len,0,align_len);
			//#endif
			//skb_put(pkt,align_len);
			sdio_len += align_len;
		}

		//AICWFDBG(LOGTRACE,"pkt_len %d  \n",pkt->len);
		start_ptr =
			skb_push(pkt, (SDIO_HEADER_LEN + SDIO_MGMT_FAKE_LEN));
		//AICWFDBG(LOGTRACE,"push_pkt_len %d  \n",pkt->len);
		sdio_header = start_ptr;

		sdio_header[0] = ((sdio_len)&0xff);
		sdio_header[1] = (((sdio_len) >> 8) & 0x0f);
		sdio_header[2] = 0x01; //data
		if (tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800D80)
			sdio_header[3] =
				crc8_ponl_107(&sdio_header[0], 3); // crc8
		else
			sdio_header[3] = 0; //reserved

		// fill 4 byte 0x00, need to fasync with data-pkt.
		sdio_header[4] = 0;
		sdio_header[5] = 0;
		sdio_header[6] = 0;
		sdio_header[7] = 0;

		curr_len = sdio_len + SDIO_HEADER_LEN;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf = start_ptr;
		tx_priv->sg_list[tx_priv->aggr_segcnt].len = curr_len;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = pkt;

		//AICWFDBG(LOGTRACE,"curr_len %d \n",curr_len);
		//rwnx_data_dump("adj0",pkt->data,((pkt->len>32) ? 32 : pkt->len));

		tx_priv->aggr_segcnt++;
		atomic_inc(&tx_priv->aggr_count);
		tx_priv->len += curr_len;
	} else if (adj_len == 1) {
		sdio_len = pkt->len + 3;
		if (sdio_len & (TX_ALIGNMENT - 1)) {
			align_len =
				TX_ALIGNMENT - (sdio_len & (TX_ALIGNMENT - 1));
			//#ifdef CONFIG_SDIO_ADMA_ADJ
			//memset(pkt->data + sdio_len,0,align_len);
			//#endif
			//skb_put(pkt,align_len);
			sdio_len += align_len;
		}

		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][0] = ((sdio_len)&0xff);
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][1] =
			(((sdio_len) >> 8) & 0x0f);
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][2] = 0x01; // data
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][3] = 0;
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][4] = 0;
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][5] = 0;
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][6] = 0;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf =
			sdio_tx_buf_dummy[tx_priv->aggr_segcnt];
		tx_priv->sg_list[tx_priv->aggr_segcnt].len =
			SDIO_HEADER_LEN + 3;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = NULL;

		tx_priv->aggr_segcnt++;

		sdio_header = start_ptr;
		sdio_header[0] = 0;

		curr_len = sdio_len - 3;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf = start_ptr;
		tx_priv->sg_list[tx_priv->aggr_segcnt].len = curr_len;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = pkt;

		//AICWFDBG(LOGTRACE,"curr_len %d \n",curr_len);
		//rwnx_data_dump("adj1",start_ptr,((curr_len>32) ? 32 : curr_len));

		tx_priv->aggr_segcnt++;
		atomic_inc(&tx_priv->aggr_count);

		tx_priv->len += curr_len + SDIO_HEADER_LEN + 3;

	} else if (adj_len == 2) { // adj_len==2
		sdio_len = pkt->len + SDIO_DATA_FAKE_LEN; //adj_len +
		if (sdio_len & (TX_ALIGNMENT - 1)) {
			align_len =
				TX_ALIGNMENT - (sdio_len & (TX_ALIGNMENT - 1));
			//#ifdef CONFIG_SDIO_ADMA_ADJ
			//memset(pkt->data + sdio_len,0,align_len);
			//#endif
			//skb_put(pkt,align_len);
			sdio_len += align_len;
		}

		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][0] = ((sdio_len)&0xff);
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][1] =
			(((sdio_len) >> 8) & 0x0f);
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][2] = 0x01; // data
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][3] = 0;
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][4] = 0;
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][5] = 0;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf =
			sdio_tx_buf_dummy[tx_priv->aggr_segcnt];
		tx_priv->sg_list[tx_priv->aggr_segcnt].len =
			SDIO_HEADER_LEN + SDIO_DATA_FAKE_LEN;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = NULL;

		tx_priv->aggr_segcnt++;

		sdio_header = start_ptr;
		sdio_header[0] = 0;
		sdio_header[1] = 0;

		curr_len = sdio_len - SDIO_DATA_FAKE_LEN;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf = start_ptr;
		tx_priv->sg_list[tx_priv->aggr_segcnt].len = curr_len;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = pkt;

		//AICWFDBG(LOGTRACE,"curr_len %d \n",curr_len);
		//rwnx_data_dump("adj2",start_ptr,((curr_len>32) ? 32 : curr_len));

		tx_priv->aggr_segcnt++;
		atomic_inc(&tx_priv->aggr_count);

		tx_priv->len += curr_len + SDIO_HEADER_LEN + SDIO_DATA_FAKE_LEN;
	} else if (adj_len == 3) {
		sdio_len = pkt->len + 1;
		if (sdio_len & (TX_ALIGNMENT - 1)) {
			align_len =
				TX_ALIGNMENT - (sdio_len & (TX_ALIGNMENT - 1));
			//#ifdef CONFIG_SDIO_ADMA_ADJ
			//memset(pkt->data + sdio_len,0,align_len);
			//#endif
			//skb_put(pkt,align_len);
			sdio_len += align_len;
		}

		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][0] = ((sdio_len)&0xff);
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][1] =
			(((sdio_len) >> 8) & 0x0f);
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][2] = 0x01; // data
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][3] = 0;
		sdio_tx_buf_dummy[tx_priv->aggr_segcnt][4] = 0;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf =
			sdio_tx_buf_dummy[tx_priv->aggr_segcnt];
		tx_priv->sg_list[tx_priv->aggr_segcnt].len =
			SDIO_HEADER_LEN + 1;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = NULL;

		tx_priv->aggr_segcnt++;

		sdio_header = start_ptr;
		sdio_header[0] = 0;
		sdio_header[1] = 0;
		sdio_header[2] = 0;

		curr_len = sdio_len - 1;

		tx_priv->sg_list[tx_priv->aggr_segcnt].buf = start_ptr;
		tx_priv->sg_list[tx_priv->aggr_segcnt].len = curr_len;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = pkt;

		//AICWFDBG(LOGTRACE,"curr_len %d \n",curr_len);
		//rwnx_data_dump("adj3",start_ptr,((curr_len>32) ? 32 : curr_len));

		tx_priv->aggr_segcnt++;
		atomic_inc(&tx_priv->aggr_count);

		tx_priv->len += curr_len + SDIO_HEADER_LEN + 1;
	}

	//AICWFDBG(LOGTRACE,"end aggrcnt %d segcnt %d len %d \n",atomic_read(&tx_priv->aggr_count),tx_priv->aggr_segcnt,tx_priv->len);
}

#endif

static int aicwf_sdio_intr_get_len_bytemode(struct aic_sdio_dev *sdiodev,
					    u8 *byte_len)
{
	int ret = 0;

	if (!byte_len)
		return -EBADE;

	if (sdiodev->bus_if->state == BUS_DOWN_ST) {
		*byte_len = 0;
	} else {
		ret = aicwf_sdio_readb(
			sdiodev, sdiodev->sdio_reg.bytemode_len_reg, byte_len);
		sdiodev->rx_priv->data_len = (*byte_len) * 4;
	}

	return ret;
}

static void aicwf_sdio_bus_stop(struct device *dev)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
	int ret = 0;

#if defined(CONFIG_SDIO_PWRCTRL)
	aicwf_sdio_pwrctl_timer(sdiodev, 0);
#endif
	AICWFDBG(LOGINFO, "%s Enter\n", __func__);

	bus_if->state = BUS_DOWN_ST;
	if (sdiodev->tx_priv) {
		ret = down_interruptible(&sdiodev->tx_priv->txctl_sema);
		if (ret)
			AICWFDBG(LOGERROR, "down txctl_sema fail\n");
	}

#if defined(CONFIG_SDIO_PWRCTRL)
	aicwf_sdio_pwr_stctl(sdiodev, SDIO_SLEEP_ST);
#endif

	if (sdiodev->tx_priv) {
		if (!ret)
			up(&sdiodev->tx_priv->txctl_sema);
		aicwf_frame_queue_flush(&sdiodev->tx_priv->txq);
	}
	AICWFDBG(LOGINFO, "%s Exit \n", __func__);
}

#ifdef CONFIG_PREALLOC_RX_SKB
struct rx_buff *aicwf_sdio_readframes(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;
	u32 size = 0;
	struct aicwf_bus *bus_if = dev_get_drvdata(sdiodev->dev);
	struct rx_buff *rxbuff;

	if (bus_if->state == BUS_DOWN_ST) {
		sdio_dbg("bus down\n");
		return NULL;
	}

	size = sdiodev->rx_priv->data_len;
	rxbuff = aicwf_prealloc_rxbuff_alloc(&sdiodev->rx_priv->rxbuff_lock);
	if (rxbuff == NULL) {
		printk("failed to alloc rxbuff\n");
		return NULL;
	}
	rxbuff->len = 0;
	rxbuff->start = rxbuff->data;
	rxbuff->read = rxbuff->start;
	rxbuff->end = rxbuff->data + size;

	ret = aicwf_sdio_recv_pkt(sdiodev, rxbuff, size);
	if (ret) {
		printk("%s %d, sdio recv pkt fail\n", __func__, __LINE__);
		aicwf_prealloc_rxbuff_free(rxbuff,
					   &sdiodev->rx_priv->rxbuff_lock);
		return NULL;
	}

	return rxbuff;
}
#else
struct sk_buff *aicwf_sdio_readframes(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;
	u32 size = 0;
	struct sk_buff *skb = NULL;
	struct aicwf_bus *bus_if = dev_get_drvdata(sdiodev->dev);

	if (bus_if->state == BUS_DOWN_ST) {
		sdio_dbg("bus down\n");
		return NULL;
	}

	size = sdiodev->rx_priv->data_len;
	skb = __dev_alloc_skb(size, GFP_KERNEL);
	if (!skb) {
		return NULL;
	}

	ret = aicwf_sdio_recv_pkt(sdiodev, skb, size);
	if (ret) {
		dev_kfree_skb(skb);
		skb = NULL;
	}

	return skb;
}
#endif

static int aicwf_sdio_tx_msg(struct aic_sdio_dev *sdiodev)
{
	int err = 0;
	u16 len;
	u8 *payload = sdiodev->tx_priv->cmd_buf;
	u16 payload_len = sdiodev->tx_priv->cmd_len;
	u8 adjust_str[4] = { 0, 0, 0, 0 };
	int adjust_len = 0;
	int buffer_cnt = 0;
	u8 retry = 0;

	len = payload_len;
	if ((len % TX_ALIGNMENT) != 0) {
		adjust_len = roundup(len, TX_ALIGNMENT);
		memcpy(payload + payload_len, adjust_str, (adjust_len - len));
		payload_len += (adjust_len - len);
	}
	len = payload_len;

	//link tail is necessary
	if ((len % SDIOWIFI_FUNC_BLOCKSIZE) != 0) {
		memset(payload + payload_len, 0, TAIL_LEN);
		payload_len += TAIL_LEN;
		len = (payload_len / SDIOWIFI_FUNC_BLOCKSIZE + 1) *
		      SDIOWIFI_FUNC_BLOCKSIZE;
	} else
		len = payload_len;

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		buffer_cnt = aicwf_sdio_flow_ctrl_msg(sdiodev);
		while ((buffer_cnt <= 0 ||
			(buffer_cnt > 0 && len > (buffer_cnt * BUFFER_SIZE))) &&
		       retry < 10) {
			retry++;
			buffer_cnt = aicwf_sdio_flow_ctrl_msg(sdiodev);
			printk("buffer_cnt = %d\n", buffer_cnt);
		}
	}
	down(&sdiodev->tx_priv->cmd_txsema);

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		if (buffer_cnt > 0 && len < (buffer_cnt * BUFFER_SIZE)) {
			err = aicwf_sdio_send_pkt(sdiodev, payload, len);
			if (err) {
				sdio_err("aicwf_sdio_send_pkt fail%d\n", err);
			}
		} else {
			sdio_err("tx msg fc retry fail:%d, %d\n", buffer_cnt,
				 len);
			up(&sdiodev->tx_priv->cmd_txsema);
			return -1;
		}
	} else if (sdiodev->chipid == PRODUCT_ID_AIC8800DC) {
		err = aicwf_sdio_send_pkt(sdiodev, payload, len);
		if (err) {
			sdio_err("aicwf_sdio_send_pkt fail%d\n", err);
		}
	} else {
		sdio_err("tx msg fc retry fail:%d, %d\n", buffer_cnt, len);
		up(&sdiodev->tx_priv->cmd_txsema);
		return -1;
	}

	sdiodev->tx_priv->cmd_txstate = false;
	if (!err)
		sdiodev->tx_priv->cmd_tx_succ = true;
	else
		sdiodev->tx_priv->cmd_tx_succ = false;

	up(&sdiodev->tx_priv->cmd_txsema);

	return err;
}

static void aicwf_sdio_tx_process(struct aic_sdio_dev *sdiodev)
{
	int err = 0;
#ifdef CONFIG_TX_NETIF_FLOWCTRL
	unsigned long flags;
#endif

	if (sdiodev->bus_if->state == BUS_DOWN_ST) {
		sdio_err("Bus is down\n");
		return;
	}

#if defined(CONFIG_SDIO_PWRCTRL)
	aicwf_sdio_pwr_stctl(sdiodev, SDIO_ACTIVE_ST);
#endif

	//config
	sdio_info("send cmd\n");
	if (sdiodev->tx_priv->cmd_txstate) {
		if (down_interruptible(&sdiodev->tx_priv->txctl_sema)) {
			txrx_err("txctl down bus->txctl_sema fail\n");
			return;
		}
		if (sdiodev->state != SDIO_ACTIVE_ST) {
			txrx_err("state err\n");
			up(&sdiodev->tx_priv->txctl_sema);
			txrx_err("txctl up bus->txctl_sema fail\n");
			return;
		}
		err = aicwf_sdio_tx_msg(sdiodev);
		up(&sdiodev->tx_priv->txctl_sema);
		if (waitqueue_active(&sdiodev->tx_priv->cmd_txdone_wait))
			wake_up(&sdiodev->tx_priv->cmd_txdone_wait);
	}

	//data
	sdio_info("send data\n");
	if (down_interruptible(&sdiodev->tx_priv->txctl_sema)) {
		txrx_err("txdata down bus->txctl_sema\n");
		return;
	}

	if (sdiodev->state != SDIO_ACTIVE_ST) {
		txrx_err("sdio state err\n");
		up(&sdiodev->tx_priv->txctl_sema);
		return;
	}

	if (!aicwf_is_framequeue_empty(&sdiodev->tx_priv->txq)) {
		sdiodev->tx_priv->fw_avail_bufcnt =
			aicwf_sdio_flow_ctrl(sdiodev);
	}
	while (!aicwf_is_framequeue_empty(&sdiodev->tx_priv->txq)) {
		if (sdiodev->bus_if->state == BUS_DOWN_ST) {
			break;
		}
#ifdef CONFIG_TEMP_CONTROL
		if (sdiodev->on_off)
			aicwf_temp_ctrl(sdiodev);
#endif

		if (sdiodev->tx_priv->fw_avail_bufcnt <=
		    DATA_FLOW_CTRL_THRESH) {
			if (sdiodev->tx_priv->cmd_txstate)
				break;
			sdiodev->tx_priv->fw_avail_bufcnt =
				aicwf_sdio_flow_ctrl(sdiodev);
		} else {
			if (sdiodev->tx_priv->cmd_txstate) {
				aicwf_sdio_send(sdiodev->tx_priv, 1);
				break;
			} else {
				aicwf_sdio_send(sdiodev->tx_priv, 0);
			}
		}
	}

#ifdef CONFIG_TX_NETIF_FLOWCTRL
	spin_lock_irqsave(&sdiodev->tx_flow_lock, flags);
	if (atomic_read(&sdiodev->tx_priv->tx_pktcnt) < tx_fc_low_water) {
		//printk("sdiodev->tx_priv->tx_pktcnt < tx_fc_low_water:%d %d\r\n",
		//	  atomic_read(&sdiodev->tx_priv->tx_pktcnt), tx_fc_low_water);
		if (sdiodev->flowctrl) {
			sdiodev->flowctrl = 0;
			aicwf_sdio_tx_netif_flowctrl(sdiodev->rwnx_hw, false);
		}
	}
	spin_unlock_irqrestore(&sdiodev->tx_flow_lock, flags);
#endif

	up(&sdiodev->tx_priv->txctl_sema);
}

static int aicwf_sdio_bus_txdata(struct device *dev, struct sk_buff *pkt)
{
	uint prio;
	int ret = -EBADE;
	struct rwnx_txhdr *txhdr = NULL;
	int headroom = 0;
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
#ifdef CONFIG_TX_NETIF_FLOWCTRL
	unsigned long flags;
#endif

	if (bus_if->state == BUS_DOWN_ST) {
		sdio_err("bus_if stopped\n");
		txhdr = (struct rwnx_txhdr *)pkt->data;
		headroom = txhdr->sw_hdr->headroom;
		kmem_cache_free(
			txhdr->sw_hdr->rwnx_vif->rwnx_hw->sw_txhdr_cache,
			txhdr->sw_hdr);
		skb_pull(pkt, headroom);
		consume_skb(pkt);
		return -1;
	}

	prio = (pkt->priority & 0x7);
	spin_lock_bh(&sdiodev->tx_priv->txqlock);
	if (!aicwf_frame_enq(sdiodev->dev, &sdiodev->tx_priv->txq, pkt, prio)) {
		txhdr = (struct rwnx_txhdr *)pkt->data;
		headroom = txhdr->sw_hdr->headroom;
		kmem_cache_free(
			txhdr->sw_hdr->rwnx_vif->rwnx_hw->sw_txhdr_cache,
			txhdr->sw_hdr);
		skb_pull(pkt, headroom);
		consume_skb(pkt);
		spin_unlock_bh(&sdiodev->tx_priv->txqlock);
		return -ENOSR;
		goto flowctrl;
	} else {
		ret = 0;
	}

	atomic_inc(&sdiodev->tx_priv->tx_pktcnt);
	spin_unlock_bh(&sdiodev->tx_priv->txqlock);
	complete(&bus_if->bustx_trgg);

flowctrl:
#ifdef CONFIG_TX_NETIF_FLOWCTRL
	spin_lock_irqsave(&sdiodev->tx_flow_lock, flags);
	if (atomic_read(&sdiodev->tx_priv->tx_pktcnt) >= tx_fc_high_water) {
		//printk("sdiodev->tx_priv->tx_pktcnt >= tx_fc_high_water:%d %d\r\n",
		//	atomic_read(&sdiodev->tx_priv->tx_pktcnt), tx_fc_high_water);
		if (!sdiodev->flowctrl) {
			sdiodev->flowctrl = 1;
			aicwf_sdio_tx_netif_flowctrl(sdiodev->rwnx_hw, true);
		}
	}
	spin_unlock_irqrestore(&sdiodev->tx_flow_lock, flags);
#endif

	return ret;
}

static int aicwf_sdio_bus_txmsg(struct device *dev, u8 *msg, uint msglen)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;

	down(&sdiodev->tx_priv->cmd_txsema);
	sdiodev->tx_priv->cmd_txstate = true;
	sdiodev->tx_priv->cmd_tx_succ = false;
	sdiodev->tx_priv->cmd_buf = msg;
	sdiodev->tx_priv->cmd_len = msglen;
	up(&sdiodev->tx_priv->cmd_txsema);

	if (bus_if->state != BUS_UP_ST) {
		sdio_err("bus has stop\n");
		return -1;
	}

	complete(&bus_if->bustx_trgg);
#if 0
	if (sdiodev->tx_priv->cmd_txstate) {
		int timeout = msecs_to_jiffies(CMD_TX_TIMEOUT);
		ret = wait_event_interruptible_timeout(sdiodev->tx_priv->cmd_txdone_wait, \
											!(sdiodev->tx_priv->cmd_txstate), timeout);
	}

	if (!sdiodev->tx_priv->cmd_txstate && sdiodev->tx_priv->cmd_tx_succ) {
		ret = 0;
	} else {
		sdio_err("send faild:%d, %d,%x\n", sdiodev->tx_priv->cmd_txstate, sdiodev->tx_priv->cmd_tx_succ, ret);
		ret = -EIO;
	}
#endif
	return 0;
}

int aicwf_sdio_send(struct aicwf_tx_priv *tx_priv, u8 txnow)
{
	struct sk_buff *pkt;
	struct aic_sdio_dev *sdiodev = tx_priv->sdiodev;
	u32 aggr_len = 0;
#ifdef CONFIG_TX_NETIF_FLOWCTRL
	unsigned long flags;
#endif

#ifdef CONFIG_SDIO_ADMA
	aggr_len = tx_priv->len;
#else
	aggr_len = (tx_priv->tail - tx_priv->head);
#endif

	if (((atomic_read(&tx_priv->aggr_count) == 0) && (aggr_len != 0)) ||
	    ((atomic_read(&tx_priv->aggr_count) != 0) && (aggr_len == 0))) {
		if (aggr_len > 0) {
#ifdef CONFIG_SDIO_ADMA
			printk("len %d  seg_cnt %d aggr_cnt %d avail_cnt %d  \n",
			       aggr_len, tx_priv->aggr_segcnt,
			       atomic_read(&tx_priv->aggr_count),
			       tx_priv->fw_avail_bufcnt);
#endif
			aicwf_sdio_aggrbuf_reset(tx_priv);
#ifdef CONFIG_SDIO_ADMA
			printk("set len %d  seg_cnt %d aggr_cnt %d avail_cnt %d \n",
			       aggr_len, tx_priv->aggr_segcnt,
			       atomic_read(&tx_priv->aggr_count),
			       tx_priv->fw_avail_bufcnt);
#endif
		}

		return 0;
	}

	if (atomic_read(&tx_priv->aggr_count) ==
	    (tx_priv->fw_avail_bufcnt - DATA_FLOW_CTRL_THRESH)) {
		if (atomic_read(&tx_priv->aggr_count) > 0) {
			tx_priv->fw_avail_bufcnt -=
				atomic_read(&tx_priv->aggr_count);
			aicwf_sdio_aggr_send(
				tx_priv); //send and check the next pkt;
		}
		return 0;
	} else {
		spin_lock_bh(&sdiodev->tx_priv->txqlock);
		pkt = aicwf_frame_dequeue(&sdiodev->tx_priv->txq);
		if (pkt == NULL) {
			sdio_err("txq no pkt\n");
			spin_unlock_bh(&sdiodev->tx_priv->txqlock);
			return 0;
		}
		//atomic_dec(&sdiodev->tx_priv->tx_pktcnt);
		spin_unlock_bh(&sdiodev->tx_priv->txqlock);

#ifdef CONFIG_TX_NETIF_FLOWCTRL
		spin_lock_irqsave(&sdiodev->tx_flow_lock, flags);
		if (atomic_read(&sdiodev->tx_priv->tx_pktcnt) <
		    tx_fc_low_water) {
			//printk("sdiodev->tx_priv->tx_pktcnt < tx_fc_low_water:%d %d\r\n",
			//	  atomic_read(&sdiodev->tx_priv->tx_pktcnt), tx_fc_low_water);
			if (sdiodev->flowctrl) {
				sdiodev->flowctrl = 0;
				aicwf_sdio_tx_netif_flowctrl(sdiodev->rwnx_hw,
							     false);
			}
		}
		spin_unlock_irqrestore(&sdiodev->tx_flow_lock, flags);
#endif

#ifndef CONFIG_SDIO_ADMA
		if (tx_priv == NULL || tx_priv->tail == NULL || pkt == NULL)
			txrx_err("null error\n");
#endif
		if (aicwf_sdio_aggr(tx_priv, pkt)) {
			aicwf_sdio_aggrbuf_reset(tx_priv);
			sdio_err("add aggr pkts failed!\n");
			atomic_dec(&sdiodev->tx_priv->tx_pktcnt);
			return 0;
		}

		//when aggr finish or there is cmd to send, just send this aggr pkt to fw
		if ((int)atomic_read(&sdiodev->tx_priv->tx_pktcnt) == 1 ||
		    txnow ||
		    (atomic_read(&tx_priv->aggr_count) ==
		     (tx_priv->fw_avail_bufcnt - DATA_FLOW_CTRL_THRESH))) {
			tx_priv->fw_avail_bufcnt -=
				atomic_read(&tx_priv->aggr_count);
			aicwf_sdio_aggr_send(tx_priv);
			atomic_dec(&sdiodev->tx_priv->tx_pktcnt);
			return 0;
		} else {
			atomic_dec(&sdiodev->tx_priv->tx_pktcnt);
			return 0;
		}
	}
}

int aicwf_sdio_aggr(struct aicwf_tx_priv *tx_priv, struct sk_buff *pkt)
{
#ifdef CONFIG_SDIO_ADMA
	u8 adj_len = 0;
	struct rwnx_txhdr *txhdr = (struct rwnx_txhdr *)pkt->data;
	u8 *sdio_header = NULL;
	u32 sdio_len = 0;
	u32 curr_len = 0;
	int align_len = 0;
	int headroom;
	u32 pkt_len = 0;
	struct txdesc_api tmp_txdesc;
	struct sk_buff *tmp_skb;

	//AICWFDBG(LOGTRACE,"%s \n",__func__);
	//pkt (txhdr + data),aggr only need txhdr->sw_hdr->desc and data
	memcpy(&tmp_txdesc, (u8 *)(long)&txhdr->sw_hdr->desc,
	       sizeof(struct txdesc_api));

	if (txhdr->sw_hdr->need_cfm) {
		tmp_skb = skb_copy(
			pkt,
			GFP_ATOMIC); //save pkt when cfm get,then release,send skb is copy
		if (!tmp_skb) {
			AICWFDBG(LOGERROR, "skb copy fail");
			return -ENOMEM;
		}
		//AICWFDBG(LOGTRACE,"tmp_len %d pkt_len %d \n",tmp_skb->len,pkt->len);

		skb_pull(tmp_skb, txhdr->sw_hdr->headroom);

		//AICWFDBG(LOGTRACE,"pull_tmp_len %d  \n",tmp_skb->len);

		skb_push(tmp_skb,
			 sizeof(struct txdesc_api)); //skb header is enough

		//AICWFDBG(LOGTRACE,"push_tmp_len %d  \n",tmp_skb->len);

		memcpy(tmp_skb->data, (u8 *)(long)&tmp_txdesc,
		       sizeof(struct txdesc_api));
		AICWFDBG(LOGTRACE, "need cfm len %d \n", tmp_skb->len);

		//printk("cfm alloc %p \n",tmp_skb);
		//printk("delay free %p \n",pkt);
		aicwf_adma_add(tx_priv, tmp_skb);
		tx_priv->copyd[tx_priv->aggr_segcnt - 1] = true;
	} else {
		headroom = txhdr->sw_hdr->headroom;
		kmem_cache_free(
			txhdr->sw_hdr->rwnx_vif->rwnx_hw->sw_txhdr_cache,
			txhdr->sw_hdr);
		skb_pull(pkt, headroom);

		//AICWFDBG(LOGTRACE,"pull_pkt_len %d  \n",pkt->len);

		skb_push(pkt, sizeof(struct txdesc_api)); //skb header is enough

		//AICWFDBG(LOGTRACE,"push_pkt_len %d  \n",pkt->len);

		memcpy(pkt->data, (u8 *)(long)&tmp_txdesc,
		       sizeof(struct txdesc_api));

		//rwnx_data_dump("data",pkt->data,((pkt->len>64) ? 64 : pkt->len));

		//printk("alloc %p \n",pkt);

		aicwf_adma_add(tx_priv, pkt);
		tx_priv->copyd[tx_priv->aggr_segcnt - 1] = false;
	}

#else

	struct rwnx_txhdr *txhdr = (struct rwnx_txhdr *)pkt->data;
	u8 *start_ptr = tx_priv->tail;
	u8 sdio_header[4];
	u8 adjust_str[4] = { 0, 0, 0, 0 };
	u32 curr_len = 0;
	int allign_len = 0;
	int headroom;

	sdio_header[0] = ((pkt->len - txhdr->sw_hdr->headroom +
			   sizeof(struct txdesc_api)) &
			  0xff);
	sdio_header[1] = (((pkt->len - txhdr->sw_hdr->headroom +
			    sizeof(struct txdesc_api)) >>
			   8) &
			  0x0f);
	sdio_header[2] = 0x01; //data
	if (tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800DW)
		sdio_header[3] = 0; //reserved
	else if (tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
		 tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800D80X2)
		sdio_header[3] = crc8_ponl_107(&sdio_header[0], 3); // crc8

	memcpy(tx_priv->tail, (u8 *)&sdio_header, sizeof(sdio_header));
	tx_priv->tail += sizeof(sdio_header);
	//payload
	memcpy(tx_priv->tail, (u8 *)(long)&txhdr->sw_hdr->desc,
	       sizeof(struct txdesc_api));
	tx_priv->tail += sizeof(struct txdesc_api); //hostdesc
	memcpy(tx_priv->tail, (u8 *)((u8 *)txhdr + txhdr->sw_hdr->headroom),
	       pkt->len - txhdr->sw_hdr->headroom);
	tx_priv->tail += (pkt->len - txhdr->sw_hdr->headroom);

	//word alignment
	curr_len = tx_priv->tail - tx_priv->head;
	if (curr_len & (TX_ALIGNMENT - 1)) {
		allign_len = roundup(curr_len, TX_ALIGNMENT) - curr_len;
		memcpy(tx_priv->tail, adjust_str, allign_len);
		tx_priv->tail += allign_len;
	}

	if (tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    tx_priv->sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		start_ptr[0] = ((tx_priv->tail - start_ptr - 4) & 0xff);
		start_ptr[1] = (((tx_priv->tail - start_ptr - 4) >> 8) & 0x0f);
	}
	tx_priv->aggr_buf->dev = pkt->dev;

	if (!txhdr->sw_hdr->need_cfm) {
		headroom = txhdr->sw_hdr->headroom;
		kmem_cache_free(
			txhdr->sw_hdr->rwnx_vif->rwnx_hw->sw_txhdr_cache,
			txhdr->sw_hdr);
		skb_pull(pkt, headroom);
		consume_skb(pkt);
	}

	atomic_inc(&tx_priv->aggr_count);
#endif /* CONFIG_SDIO_ADMA */

	return 0;
}

void aicwf_sdio_aggr_send(struct aicwf_tx_priv *tx_priv)
{
#ifdef CONFIG_SDIO_ADMA
	int ret = 0;
	int i = 0;
	int nents = tx_priv->aggr_segcnt;
	//struct sk_buff *scatter_buf;

	if (tx_priv->len & (TXPKT_BLOCKSIZE - 1)) {
		uint16_t alen = TXPKT_BLOCKSIZE -
				(tx_priv->len & (TXPKT_BLOCKSIZE - 1));

		memset(sdio_tx_buf_fill, 0, 32);
		tx_priv->sg_list[tx_priv->aggr_segcnt].buf = sdio_tx_buf_fill;
		tx_priv->sg_list[tx_priv->aggr_segcnt].len = alen;
		tx_priv->free_buf[tx_priv->aggr_segcnt] = NULL;
		tx_priv->aggr_segcnt += 1;
		tx_priv->len += alen;
	}

	//AICWFDBG(LOGTRACE,"nents %d \n",tx_priv->aggr_segcnt);
	/*for(i=0;i<tx_priv->aggr_segcnt;i++){
		if(tx_priv->sg_list[i].buf != NULL){
			rwnx_data_dump("send",tx_priv->sg_list[i].buf,((tx_priv->sg_list[i].len>64) ? 64 : tx_priv->sg_list[i].len));
		}
	}*/

	ret = aicwf_sdio_txscatterpkt(tx_priv);
	if (ret < 0) {
		sdio_err("fail to send scatter pkt %d !\n", ret);
	}

#else

	struct sk_buff *tx_buf = tx_priv->aggr_buf;
	int ret = 0;
	int curr_len = 0;

	//link tail is necessary
	curr_len = tx_priv->tail - tx_priv->head;
	if ((curr_len % TXPKT_BLOCKSIZE) != 0) {
		memset(tx_priv->tail, 0, TAIL_LEN);
		tx_priv->tail += TAIL_LEN;
	}

	tx_buf->len = tx_priv->tail - tx_priv->head;
	ret = aicwf_sdio_txpkt(tx_priv->sdiodev, tx_buf);
	if (ret < 0) {
		sdio_err("fail to send aggr pkt!\n");
	}
#endif /* CONFIG_SDIO_ADMA */

	aicwf_sdio_aggrbuf_reset(tx_priv);
}

void aicwf_sdio_aggrbuf_reset(struct aicwf_tx_priv *tx_priv)
{
#ifdef CONFIG_SDIO_ADMA
	struct sk_buff *scatter_buf;
	int i = 0;
	int nents = tx_priv->aggr_segcnt;

	for (i = 0; i < nents; i++) {
		scatter_buf = (struct sk_buff *)tx_priv->free_buf[i];
		if (scatter_buf != NULL) {
			//printk("free %p \n",scatter_buf);
			if (tx_priv->copyd[i]) {
				kfree_skb(scatter_buf);
				tx_priv->copyd[i] = false;
			} else
				consume_skb(scatter_buf);
		}
	}

	tx_priv->len = 0;
	tx_priv->aggr_segcnt = 0;
	atomic_set(&tx_priv->aggr_count, 0);
	AICWFDBG(LOGTRACE, "reset len %d seg_cnt %d aggr_cnt %d \n",
		 tx_priv->len, tx_priv->aggr_segcnt,
		 atomic_read(&tx_priv->aggr_count));
#else

	struct sk_buff *aggr_buf = tx_priv->aggr_buf;

	tx_priv->tail = tx_priv->head;
	aggr_buf->len = 0;
	atomic_set(&tx_priv->aggr_count, 0);
#endif /* CONFIG_SDIO_ADMA */
}

extern void set_irq_handler(void *fn);

static int aicwf_sdio_bus_start(struct device *dev)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
	int ret = 0;

	sdio_claim_host(sdiodev->func);
#ifndef CONFIG_FDRV_NO_REG_SDIO
	sdio_claim_irq(sdiodev->func, aicwf_sdio_hal_irqhandler);
#else
	set_irq_handler(aicwf_sdio_hal_irqhandler);
#endif
	if (sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		sdio_f0_writeb(sdiodev->func, 0x07, 0x04, &ret);
		if (ret) {
			sdio_err("set func0 int en fail %d\n", ret);
		}
	}
	sdio_release_host(sdiodev->func);

	//enable sdio interrupt
	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.intr_config_reg,
				0x07);
	if (ret != 0)
		sdio_err("intr register failed:%d\n", ret);

	bus_if->state = BUS_UP_ST;

	return ret;
}

#ifdef CONFIG_TXRX_THREAD_PRIO

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
#include "uapi/linux/sched/types.h"
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
#include "linux/sched/types.h"
#else
#include "linux/sched/rt.h"
#endif

int bustx_thread_prio = 1;
module_param_named(bustx_thread_prio, bustx_thread_prio, int, 0644);
//module_param(bustx_thread_prio, int, 0);
int busrx_thread_prio = 1;
module_param_named(busrx_thread_prio, busrx_thread_prio, int, 0644);
//module_param(busrx_thread_prio, int, 0);
#endif

static inline void aic_thread_wait_stop(void)
{
#if 1 // PLATFORM_LINUX
#if 0
	while (!kthread_should_stop()){
        AICWFDBG(LOGINFO, "%s waiting for thread_stop notify \r\n", __func__);
		msleep(100);
    }
#else
	set_current_state(TASK_INTERRUPTIBLE);
	while (!kthread_should_stop()) {
		AICWFDBG(LOGINFO, "%s waiting for thread_stop notify \r\n",
			 __func__);
		schedule();
		set_current_state(TASK_INTERRUPTIBLE);
	}
	__set_current_state(TASK_RUNNING);
#endif
#endif
}

#ifdef CONFIG_OOB
int rx_thread_wait_to = 1000;
module_param_named(rx_thread_wait_to, rx_thread_wait_to, int, 0644);

//new oob feature
int sdio_busirq_thread(void *data)
{
	struct aicwf_rx_priv *rx_priv = (struct aicwf_rx_priv *)data;
	struct aicwf_bus *bus_if = rx_priv->sdiodev->bus_if;
#if 0
#ifdef CONFIG_THREAD_INFO_IN_TASK
        int set_cpu_ret = 0;
       
        AICWFDBG(LOGINFO, "%s the cpu is:%d\n", __func__, current->cpu);
        set_cpu_ret = set_cpus_allowed_ptr(current, cpumask_of(0));
        AICWFDBG(LOGINFO, "%s set_cpu_ret is:%d\n", __func__, set_cpu_ret);
        AICWFDBG(LOGINFO, "%s change cpu to:%d\n", __func__, current->cpu);
#endif
#endif

#ifdef CONFIG_TXRX_THREAD_PRIO
	if (busrx_thread_prio > 0) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 9, 0))
		sched_set_fifo_low(current);
#else
		struct sched_param param;
		param.sched_priority = (busrx_thread_prio - 1 < MAX_RT_PRIO) ?
					       busrx_thread_prio :
					       (MAX_RT_PRIO - 1);
		sched_setscheduler(current, SCHED_FIFO, &param);
#endif
	}
#endif

	AICWFDBG(LOGINFO, "%s the policy of current thread is:%d\n", __func__,
		 current->policy);
	AICWFDBG(LOGINFO, "%s the rt_priority of current thread is:%d\n",
		 __func__, current->rt_priority);
	AICWFDBG(LOGINFO, "%s the current pid is:%d\n", __func__, current->pid);

	while (1) {
#if 0
            if (kthread_should_stop()) {
                AICWFDBG(LOGERROR, "sdio busirq thread stop\n");
                break;
            }
#endif
		if (!wait_for_completion_timeout(
			    &bus_if->busirq_trgg,
			    msecs_to_jiffies(rx_thread_wait_to))) {
			AICWFDBG(LOGRXPOLL,
				 "%s wait for completion timout \r\n",
				 __func__);
		}

		if (bus_if->state == BUS_DOWN_ST) {
			AICWFDBG(LOGERROR, "%s bus down thread exit \r\n",
				 __func__);
			break;
		}
		//continue;
#if 1
#ifdef CONFIG_SDIO_PWRCTRL
		while (atomic_read(&bus_if->bus_priv.sdio->is_bus_suspend) ==
		       1) {
			AICWFDBG(LOGDEBUG,
				 "%s waiting for sdio bus resume \r\n",
				 __func__);
			msleep(100);
		}
		aicwf_sdio_pwr_stctl(bus_if->bus_priv.sdio, SDIO_ACTIVE_ST);
#endif //CONFIG_SDIO_PWRCTRL
#endif
		aicwf_sdio_hal_irqhandler(bus_if->bus_priv.sdio->func);
	}

	aic_thread_wait_stop();

	return 0;
}

#endif //CONFIG_OOB

#if 0
#include <linux/sched.h>
#endif
int sdio_bustx_thread(void *data)
{
	struct aicwf_bus *bus = (struct aicwf_bus *)data;
	struct aic_sdio_dev *sdiodev = bus->bus_priv.sdio;
#if 0
#ifdef CONFIG_THREAD_INFO_IN_TASK
    int set_cpu_ret = 0;

    AICWFDBG(LOGINFO, "%s the cpu is:%d\n", __func__, current->cpu);
    set_cpu_ret = set_cpus_allowed_ptr(current, cpumask_of(1));
    AICWFDBG(LOGINFO, "%s set_cpu_ret is:%d\n", __func__, set_cpu_ret);
    AICWFDBG(LOGINFO, "%s change cpu to:%d\n", __func__, current->cpu);
#endif
#endif

#if 0
	struct cpumask cpumask;
	cpumask_clear(&cpumask);
	cpumask_set_cpu(1, &cpumask);
	cpumask_set_cpu(2, &cpumask);
	cpumask_set_cpu(3, &cpumask);
	sched_setaffinity(0, &cpumask);//need to add EXPORT_SYMBOL_GPL(sched_setaffinity) in kernel/sched/core.c
#endif
#ifdef CONFIG_TXRX_THREAD_PRIO
	if (bustx_thread_prio > 0) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 9, 0))
		sched_set_fifo_low(current);
#else
		struct sched_param param;
		param.sched_priority = (bustx_thread_prio < MAX_RT_PRIO) ?
					       bustx_thread_prio :
					       (MAX_RT_PRIO - 1);
		sched_setscheduler(current, SCHED_FIFO, &param);
#endif
	}
#endif

	AICWFDBG(LOGINFO, "%s the policy of current thread is:%d\n", __func__,
		 current->policy);
	AICWFDBG(LOGINFO, "%s the rt_priority of current thread is:%d\n",
		 __func__, current->rt_priority);
	AICWFDBG(LOGINFO, "%s the current pid is:%d\n", __func__, current->pid);

	while (1) {
#if 0
		if (kthread_should_stop()) {
			AICWFDBG(LOGERROR, "sdio bustx thread stop\n");
			break;
		}
#endif
		if (!wait_for_completion_interruptible(&bus->bustx_trgg)) {
			if (sdiodev->bus_if->state == BUS_DOWN_ST)
				break;
			//continue;

			rwnx_wakeup_lock(sdiodev->rwnx_hw->ws_tx);
			if ((int)(atomic_read(&sdiodev->tx_priv->tx_pktcnt) >
				  0) ||
			    (sdiodev->tx_priv->cmd_txstate == true)) {
				aicwf_sdio_tx_process(sdiodev);
			}
			rwnx_wakeup_unlock(sdiodev->rwnx_hw->ws_tx);
		}
	}

	aic_thread_wait_stop();
	AICWFDBG(LOGINFO, "%s Exit\r\n", __func__);
	return 0;
}

#if 0 //old oob feature
int sdio_busrx_thread(void *data)
{
    struct aicwf_rx_priv *rx_priv = (struct aicwf_rx_priv *)data;
    struct aicwf_bus *bus_if = rx_priv->sdiodev->bus_if;
#if 0
	struct cpumask cpumask;
	cpumask_clear(&cpumask);
	cpumask_set_cpu(1, &cpumask);
	cpumask_set_cpu(2, &cpumask);
	cpumask_set_cpu(3, &cpumask);
	sched_setaffinity(0, &cpumask);
#endif
#ifdef CONFIG_TXRX_THREAD_PRIO
    if (busrx_thread_prio > 0) {
            struct sched_param param;
            param.sched_priority = (busrx_thread_prio < MAX_RT_PRIO)?busrx_thread_prio:(MAX_RT_PRIO-1);
            sched_setscheduler(current, SCHED_FIFO, &param);
    }
#endif
    
    AICWFDBG(LOGINFO, "%s the policy of current thread is:%d\n", __func__, current->policy);
    AICWFDBG(LOGINFO, "%s the rt_priority of current thread is:%d\n", __func__, current->rt_priority);
    AICWFDBG(LOGINFO, "%s the current pid is:%d\n", __func__, current->pid);
    
while (1) {
    if (kthread_should_stop()) {
        AICWFDBG(LOGERROR, "sdio busrx thread stop\n");
        break;
    }
#ifndef CONFIG_OOB
        if (!wait_for_completion_interruptible(&bus_if->busrx_trgg)) {
#else
        if(!wait_for_completion_timeout(&bus_if->busrx_trgg, msecs_to_jiffies(rx_thread_wait_to))){
            AICWFDBG(LOGDEBUG, "%s wait for completion timout \r\n", __func__);
        }
#endif
        if (bus_if->state == BUS_DOWN_ST)
            continue;
#ifdef CONFIG_OOB
#ifdef CONFIG_SDIO_PWRCTRL
        while(atomic_read(&bus_if->bus_priv.sdio->is_bus_suspend) == 1){
            AICWFDBG(LOGDEBUG, "%s waiting for sdio bus resume \r\n", __func__);
            msleep(100);
        }
        aicwf_sdio_pwr_stctl(bus_if->bus_priv.sdio, SDIO_ACTIVE_ST);
#endif //CONFIG_SDIO_PWRCTRL
        aicwf_sdio_hal_irqhandler(bus_if->bus_priv.sdio->func);
#endif //CONFIG_OOB
        rwnx_wakeup_lock(rx_priv->sdiodev->rwnx_hw->ws_rx);
        aicwf_process_rxframes(rx_priv);
        rwnx_wakeup_unlock(rx_priv->sdiodev->rwnx_hw->ws_rx);
#ifndef CONFIG_OOB
        }
#endif
    }
    
    return 0;

}
#else //new oob feature
int sdio_busrx_thread(void *data)
{
	struct aicwf_rx_priv *rx_priv = (struct aicwf_rx_priv *)data;
	struct aicwf_bus *bus_if = rx_priv->sdiodev->bus_if;

#if 0
	struct cpumask cpumask;
	cpumask_clear(&cpumask);
	cpumask_set_cpu(1, &cpumask);
	cpumask_set_cpu(2, &cpumask);
	cpumask_set_cpu(3, &cpumask);
	sched_setaffinity(0, &cpumask);
#endif
#if 0
#ifdef CONFIG_THREAD_INFO_IN_TASK
    int set_cpu_ret = 0;

    AICWFDBG(LOGINFO, "%s the cpu is:%d\n", __func__, current->cpu);
    set_cpu_ret = set_cpus_allowed_ptr(current, cpumask_of(2));
    AICWFDBG(LOGINFO, "%s set_cpu_ret is:%d\n", __func__, set_cpu_ret);
    AICWFDBG(LOGINFO, "%s change cpu to:%d\n", __func__, current->cpu);
#endif
#endif
#ifdef CONFIG_TXRX_THREAD_PRIO
	if (busrx_thread_prio > 0) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 9, 0))
		sched_set_fifo_low(current);
#else
		struct sched_param param;
		param.sched_priority = (busrx_thread_prio < MAX_RT_PRIO) ?
					       busrx_thread_prio :
					       (MAX_RT_PRIO - 1);
		sched_setscheduler(current, SCHED_FIFO, &param);
#endif
	}
#endif

	AICWFDBG(LOGINFO, "%s the policy of current thread is:%d\n", __func__,
		 current->policy);
	AICWFDBG(LOGINFO, "%s the rt_priority of current thread is:%d\n",
		 __func__, current->rt_priority);
	AICWFDBG(LOGINFO, "%s the current pid is:%d\n", __func__, current->pid);

	while (1) {
#if 0
        if (kthread_should_stop()) {
            AICWFDBG(LOGERROR, "sdio busrx thread stop\n");
            break;
        }
#endif
		if (!wait_for_completion_interruptible(&bus_if->busrx_trgg)) {
			if (bus_if->state == BUS_DOWN_ST)
				break;
			//continue;
			rwnx_wakeup_lock(rx_priv->sdiodev->rwnx_hw->ws_rx);
			aicwf_process_rxframes(rx_priv);
			rwnx_wakeup_unlock(rx_priv->sdiodev->rwnx_hw->ws_rx);
		}
	}
	aic_thread_wait_stop();
	AICWFDBG(LOGINFO, "%s Exit\r\n", __func__);
	return 0;
}

#endif //old oob feature

#if defined(CONFIG_SDIO_PWRCTRL)
static int aicwf_sdio_pwrctl_thread(void *data)
{
	struct aic_sdio_dev *sdiodev = (struct aic_sdio_dev *)data;

	while (1) {
		if (kthread_should_stop()) {
			sdio_err("sdio pwrctl thread stop\n");
			break;
		}
		if (!wait_for_completion_interruptible(
			    &sdiodev->pwrctrl_trgg)) {
			//printk("%s working\r\n", __func__);

			if (sdiodev->bus_if->state == BUS_DOWN_ST)
				continue;

			rwnx_wakeup_lock(sdiodev->rwnx_hw->ws_pwrctrl);

			if ((int)(atomic_read(&sdiodev->tx_priv->tx_pktcnt) <=
				  0) &&
			    (sdiodev->tx_priv->cmd_txstate == false) &&
			    atomic_read(&sdiodev->rx_priv->rx_cnt) == 0)
				aicwf_sdio_pwr_stctl(sdiodev, SDIO_SLEEP_ST);
			else
				aicwf_sdio_pwrctl_timer(
					sdiodev, sdiodev->active_duration);

			rwnx_wakeup_unlock(sdiodev->rwnx_hw->ws_pwrctrl);
		}
	}

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
static void aicwf_sdio_bus_pwrctl(ulong data)
#else
static void aicwf_sdio_bus_pwrctl(struct timer_list *t)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
	struct aic_sdio_dev *sdiodev = (struct aic_sdio_dev *)data;
#else
	struct aic_sdio_dev *sdiodev = from_timer(sdiodev, t, timer);
#endif

	if (sdiodev->bus_if->state == BUS_DOWN_ST) {
		sdio_err("bus down\n");
		return;
	}

	if (sdiodev->pwrctl_tsk) {
		complete(&sdiodev->pwrctrl_trgg);
	}
}
#endif

#ifdef CONFIG_PREALLOC_RX_SKB
static void aicwf_sdio_enq_rxpkt(struct aic_sdio_dev *sdiodev,
				 struct rx_buff *pkt)
#else
static void aicwf_sdio_enq_rxpkt(struct aic_sdio_dev *sdiodev,
				 struct sk_buff *pkt)
#endif
{
	struct aicwf_rx_priv *rx_priv = sdiodev->rx_priv;
	unsigned long flags = 0;

	spin_lock_irqsave(&rx_priv->rxqlock, flags);
#ifdef CONFIG_PREALLOC_RX_SKB
	if (!aicwf_rxbuff_enqueue(sdiodev->dev, &rx_priv->rxq, pkt)) {
		spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
		printk("%s %d, enqueue rxq fail\n", __func__, __LINE__);
		aicwf_prealloc_rxbuff_free(pkt, &rx_priv->rxbuff_lock);
		return;
	}
#else
	if (!aicwf_rxframe_enqueue(sdiodev->dev, &rx_priv->rxq, pkt)) {
		spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
		aicwf_dev_skb_free(pkt);
		return;
	}
#endif
	spin_unlock_irqrestore(&rx_priv->rxqlock, flags);

	atomic_inc(&rx_priv->rx_cnt);
}

#define SDIO_OTHER_INTERRUPT (0x1ul << 7)

void aicwf_sdio_hal_irqhandler(struct sdio_func *func)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(&func->dev);
	struct aic_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
	u8 intstatus = 0;
	u8 byte_len = 0;
#ifdef CONFIG_PREALLOC_RX_SKB
	struct rx_buff *pkt = NULL;
#else
	struct sk_buff *pkt = NULL;
#endif
	int ret;

	if ((sdiodev->rwnx_hw) == NULL) {
		sdio_err("waiting for rwnx_hw->irq_enable is true\r\n");
		return;
	}

	//AICWFDBG(LOGDEBUG, "fdrv %s enter \r\n", __func__);
	rwnx_wakeup_lock(sdiodev->rwnx_hw->ws_irqrx);

	if (!bus_if || bus_if->state == BUS_DOWN_ST) {
		sdio_err("bus err\n");
		rwnx_wakeup_unlock(sdiodev->rwnx_hw->ws_irqrx);
		return;
	}

#ifdef CONFIG_PREALLOC_RX_SKB
	if (list_empty(&aic_rx_buff_list.rxbuff_list)) {
		printk("%s %d, rxbuff list is empty\n", __func__, __LINE__);
		rwnx_wakeup_unlock(sdiodev->rwnx_hw->ws_irqrx);
		return;
	}
#endif

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		ret = aicwf_sdio_readb(sdiodev, sdiodev->sdio_reg.block_cnt_reg,
				       &intstatus);
		while (ret || (intstatus & SDIO_OTHER_INTERRUPT)) {
			sdio_err("ret=%d, intstatus=%x\r\n", ret, intstatus);
			ret = aicwf_sdio_readb(sdiodev,
					       sdiodev->sdio_reg.block_cnt_reg,
					       &intstatus);
			if (!ret) {
				return;
			}
		}
		sdiodev->rx_priv->data_len =
			intstatus * SDIOWIFI_FUNC_BLOCKSIZE;

		if (intstatus > 0) {
			if (intstatus < 64) {
				pkt = aicwf_sdio_readframes(sdiodev);
			} else {
				aicwf_sdio_intr_get_len_bytemode(
					sdiodev,
					&byte_len); //byte_len must<= 128
				sdio_info("byte mode len=%d\r\n", byte_len);
				pkt = aicwf_sdio_readframes(sdiodev);
			}
		} else {
#ifndef CONFIG_PLATFORM_ALLWINNER
//	sdio_err("Interrupt but no data\n");
#endif
		}

		if (pkt)
			aicwf_sdio_enq_rxpkt(sdiodev, pkt);

		if (atomic_read(&sdiodev->rx_priv->rx_cnt) == 1) {
			complete(&bus_if->busrx_trgg);
		}

	} else if (sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
		   sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		do {
			ret = aicwf_sdio_readb(
				sdiodev, sdiodev->sdio_reg.misc_int_status_reg,
				&intstatus);
			if (!ret) {
				break;
			}
			sdio_err("ret=%d, intstatus=%x\r\n", ret, intstatus);
			return;
		} while (1);
		if (intstatus & SDIO_OTHER_INTERRUPT) {
			u8 int_pending;
			ret = aicwf_sdio_readb(sdiodev,
					       sdiodev->sdio_reg.sleep_reg,
					       &int_pending);
			if (ret < 0) {
				sdio_err("reg:%d read failed!\n",
					 sdiodev->sdio_reg.sleep_reg);
			}
			int_pending &= ~0x01; // dev to host soft irq
			ret = aicwf_sdio_writeb(sdiodev,
						sdiodev->sdio_reg.sleep_reg,
						int_pending);
			if (ret < 0) {
				sdio_err("reg:%d write failed!\n",
					 sdiodev->sdio_reg.sleep_reg);
			}
		}

		if (intstatus > 0) {
			uint8_t intmaskf2 = intstatus | (0x1UL << 3);
			if (intmaskf2 > 120U) { // func2
				if (intmaskf2 == 127U) { // byte mode
					//aicwf_sdio_intr_get_len_bytemode(sdiodev, &byte_len, 1);//byte_len must<= 128
					aicwf_sdio_intr_get_len_bytemode(
						sdiodev,
						&byte_len); //byte_len must<= 128
					sdio_info("byte mode len=%d\r\n",
						  byte_len);
					//pkt = aicwf_sdio_readframes(sdiodev, 1);
					pkt = aicwf_sdio_readframes(sdiodev);
				} else { // block mode
					sdiodev->rx_priv->data_len =
						(intstatus & 0x7U) *
						SDIOWIFI_FUNC_BLOCKSIZE;
					//pkt = aicwf_sdio_readframes(sdiodev, 1);
					pkt = aicwf_sdio_readframes(sdiodev);
				}
			} else { // func1
				if (intstatus == 120U) { // byte mode
					//aicwf_sdio_intr_get_len_bytemode(sdiodev, &byte_len, 0);//byte_len must<= 128
					aicwf_sdio_intr_get_len_bytemode(
						sdiodev,
						&byte_len); //byte_len must<= 128
					sdio_info("byte mode len=%d\r\n",
						  byte_len);
					//pkt = aicwf_sdio_readframes(sdiodev, 0);
					pkt = aicwf_sdio_readframes(sdiodev);
				} else { // block mode
					sdiodev->rx_priv->data_len =
						(intstatus & 0x7FU) *
						SDIOWIFI_FUNC_BLOCKSIZE;
					//pkt = aicwf_sdio_readframes(sdiodev, 0);
					pkt = aicwf_sdio_readframes(sdiodev);
				}
			}
		} else {
#ifndef CONFIG_PLATFORM_ALLWINNER
			//sdio_err("Interrupt but no data\n");
#endif
		}

		if (pkt)
			aicwf_sdio_enq_rxpkt(sdiodev, pkt);

		if (atomic_read(&sdiodev->rx_priv->rx_cnt) == 1) {
			complete(&bus_if->busrx_trgg);
		}
	}

	rwnx_wakeup_unlock(sdiodev->rwnx_hw->ws_irqrx);
}

#if defined(CONFIG_SDIO_PWRCTRL)
void aicwf_sdio_pwrctl_timer(struct aic_sdio_dev *sdiodev, uint duration)
{
	uint timeout;

	//printk("%s duration:%d\r\n", __func__, duration);
	if (sdiodev->bus_if->state == BUS_DOWN_ST && duration)
		return;

	spin_lock_bh(&sdiodev->pwrctl_lock);
	if (!duration) {
		if (timer_pending(&sdiodev->timer))
			del_timer_sync(&sdiodev->timer);
	} else {
		sdiodev->active_duration = duration;
		timeout = msecs_to_jiffies(sdiodev->active_duration);
		mod_timer(&sdiodev->timer, jiffies + timeout);
	}
	spin_unlock_bh(&sdiodev->pwrctl_lock);
}
#endif

static struct aicwf_bus_ops aicwf_sdio_bus_ops = {
	.stop = aicwf_sdio_bus_stop,
	.start = aicwf_sdio_bus_start,
	.txdata = aicwf_sdio_bus_txdata,
	.txmsg = aicwf_sdio_bus_txmsg,
};

void aicwf_sdio_release(struct aic_sdio_dev *sdiodev)
{
	struct aicwf_bus *bus_if;
	int ret = 0;
	AICWFDBG(LOGINFO, "%s Enter\n", __func__);

	bus_if = dev_get_drvdata(sdiodev->dev);
	bus_if->state = BUS_DOWN_ST;
#ifdef CONFIG_OOB
	if (sdiodev->oob_enable) {
		sdio_claim_host(sdiodev->func);
		//disable sdio interrupt
		ret = aicwf_sdio_writeb(sdiodev,
					sdiodev->sdio_reg.intr_config_reg, 0x0);
		if (ret < 0) {
			AICWFDBG(LOGERROR, "reg:%d write failed!\n",
				 sdiodev->sdio_reg.intr_config_reg);
		}
		sdio_release_irq(sdiodev->func);
		sdio_release_host(sdiodev->func);
	}
#else
	sdio_claim_host(sdiodev->func);
	//disable sdio interrupt
	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.intr_config_reg,
				0x0);
	if (ret < 0) {
		sdio_err("reg:%d write failed!\n",
			 sdiodev->sdio_reg.intr_config_reg);
	}
	sdio_release_irq(sdiodev->func);
	sdio_release_host(sdiodev->func);
#endif
	if (sdiodev->dev)
		aicwf_bus_deinit(sdiodev->dev);

	if (sdiodev->tx_priv)
		aicwf_tx_deinit(sdiodev->tx_priv);

	if (sdiodev->rx_priv)
		aicwf_rx_deinit(sdiodev->rx_priv);

#if defined(CONFIG_SDIO_PWRCTRL)
	if (sdiodev->pwrctl_tsk) {
		complete_all(&sdiodev->pwrctrl_trgg);
		kthread_stop(sdiodev->pwrctl_tsk);
		sdiodev->pwrctl_tsk = NULL;
	}

	AICWFDBG(LOGINFO, "%s:pwrctl stopped\n", __func__);
#endif

	if (sdiodev->cmd_mgr.state == RWNX_CMD_MGR_STATE_INITED)
		rwnx_cmd_mgr_deinit(&sdiodev->cmd_mgr);
	AICWFDBG(LOGINFO, "%s Exit\n", __func__);
}

void aicwf_sdio_reg_init(struct aic_sdio_dev *sdiodev)
{
	sdio_dbg("%s\n", __func__);

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		sdiodev->sdio_reg.bytemode_len_reg = SDIOWIFI_BYTEMODE_LEN_REG;
		sdiodev->sdio_reg.intr_config_reg = SDIOWIFI_INTR_CONFIG_REG;
		sdiodev->sdio_reg.sleep_reg = SDIOWIFI_SLEEP_REG;
		sdiodev->sdio_reg.wakeup_reg = SDIOWIFI_WAKEUP_REG;
		sdiodev->sdio_reg.flow_ctrl_reg = SDIOWIFI_FLOW_CTRL_REG;
		sdiodev->sdio_reg.register_block = SDIOWIFI_REGISTER_BLOCK;
		sdiodev->sdio_reg.bytemode_enable_reg =
			SDIOWIFI_BYTEMODE_ENABLE_REG;
		sdiodev->sdio_reg.block_cnt_reg = SDIOWIFI_BLOCK_CNT_REG;
		sdiodev->sdio_reg.rd_fifo_addr = SDIOWIFI_RD_FIFO_ADDR;
		sdiodev->sdio_reg.wr_fifo_addr = SDIOWIFI_WR_FIFO_ADDR;
	} else if (sdiodev->chipid == PRODUCT_ID_AIC8800D80 ||
		   sdiodev->chipid == PRODUCT_ID_AIC8800D80X2) {
		sdiodev->sdio_reg.bytemode_len_reg =
			SDIOWIFI_BYTEMODE_LEN_REG_V3;
		sdiodev->sdio_reg.intr_config_reg = SDIOWIFI_INTR_ENABLE_REG_V3;
		sdiodev->sdio_reg.sleep_reg = SDIOWIFI_INTR_PENDING_REG_V3;
		sdiodev->sdio_reg.wakeup_reg = SDIOWIFI_INTR_TO_DEVICE_REG_V3;
		sdiodev->sdio_reg.flow_ctrl_reg = SDIOWIFI_FLOW_CTRL_Q1_REG_V3;
		sdiodev->sdio_reg.bytemode_enable_reg =
			SDIOWIFI_BYTEMODE_ENABLE_REG_V3;
		sdiodev->sdio_reg.misc_int_status_reg =
			SDIOWIFI_MISC_INT_STATUS_REG_V3;
		sdiodev->sdio_reg.rd_fifo_addr = SDIOWIFI_RD_FIFO_ADDR_V3;
		sdiodev->sdio_reg.wr_fifo_addr = SDIOWIFI_WR_FIFO_ADDR_V3;
	}
}

int aicwf_sdio_func_init(struct aic_sdio_dev *sdiodev)
{
	struct mmc_host *host;
	u8 block_bit0 = 0x1;
	u8 byte_mode_disable = 0x1; //1: no byte mode
	int ret = 0;
	struct aicbsp_feature_t feature;
	//u8 val = 0;

	aicbsp_get_feature(&feature, NULL);
	aicwf_sdio_reg_init(sdiodev);

	host = sdiodev->func->card->host;

	sdio_claim_host(sdiodev->func);
#if 0 //SDIO PHASE SETTING
	sdiodev->func->card->quirks |= MMC_QUIRK_LENIENT_FN0;
	sdio_f0_writeb(sdiodev->func, feature.sdio_phase, 0x13, &ret);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "write func0 fail %d\n", ret);
         sdio_release_host(sdiodev->func);
		return ret;
	}
#endif
	ret = sdio_set_block_size(sdiodev->func, SDIOWIFI_FUNC_BLOCKSIZE);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "set blocksize fail %d\n", ret);
		sdio_release_host(sdiodev->func);
		return ret;
	}
	ret = sdio_enable_func(sdiodev->func);
	if (ret < 0) {
		sdio_release_host(sdiodev->func);
		AICWFDBG(LOGERROR, "enable func fail %d.\n", ret);
		return ret;
	}

#if 1 //SDIO CLOCK SETTING
	if (feature.sdio_clock > 0) {
		host->ios.clock = feature.sdio_clock;
		host->ops->set_ios(host, &host->ios);
		AICWFDBG(LOGINFO, "Set SDIO Clock %d MHz\n",
			 host->ios.clock / 1000000);
	}
#endif

	sdio_release_host(sdiodev->func);

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		sdio_claim_host(sdiodev->func2);
		//set sdio blocksize
		ret = sdio_set_block_size(sdiodev->func2,
					  SDIOWIFI_FUNC_BLOCKSIZE);
		if (ret < 0) {
			AICWFDBG(LOGERROR, "set func2 blocksize fail %d\n",
				 ret);
			sdio_release_host(sdiodev->func2);
			return ret;
		}

		//set sdio enable func
		ret = sdio_enable_func(sdiodev->func2);
		if (ret < 0) {
			sdio_release_host(sdiodev->func2);
			AICWFDBG(LOGERROR, "enable func2 fail %d.\n", ret);
			return ret;
		}

		sdio_release_host(sdiodev->func2);

		ret = aicwf_sdio_func2_writeb(
			sdiodev, sdiodev->sdio_reg.register_block, block_bit0);
		if (ret < 0) {
			AICWFDBG(LOGERROR, "reg:%d write failed!\n",
				 sdiodev->sdio_reg.register_block);
			return ret;
		}

		//1: no byte mode
		ret = aicwf_sdio_func2_writeb(
			sdiodev, sdiodev->sdio_reg.bytemode_enable_reg,
			byte_mode_disable);
		if (ret < 0) {
			AICWFDBG(LOGERROR, "reg:%d write failed!\n",
				 sdiodev->sdio_reg.bytemode_enable_reg);
			return ret;
		}
	}

	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.register_block,
				block_bit0);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "reg:%d write failed!\n",
			 sdiodev->sdio_reg.register_block);
		return ret;
	}

	//1: no byte mode
	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.bytemode_enable_reg,
				byte_mode_disable);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "reg:%d write failed!\n",
			 sdiodev->sdio_reg.bytemode_enable_reg);
		return ret;
	}

#if 0
	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.wakeup_reg, 1);
	if (ret < 0) {
        AICWFDBG(LOGERROR, "reg:%d write failed!\n", sdiodev->sdio_reg.wakeup_reg);
		return ret;
	}

	mdelay(5);
	ret = aicwf_sdio_readb(sdiodev, sdiodev->sdio_reg.sleep_reg, &val);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "reg:%d read failed!\n", sdiodev->sdio_reg.sleep_reg);
		return ret;
	}

	if(!(val & 0x10)){
		AICWFDBG(LOGERROR, "wakeup fail\n");
	}else{
		AICWFDBG(LOGINFO, "sdio ready\n");
	}
#else
	mdelay(10);
#endif
	return ret;
}

int aicwf_sdiov3_func_init(struct aic_sdio_dev *sdiodev)
{
	struct mmc_host *host;
	u8 byte_mode_disable = 0x1; //1: no byte mode
	int ret = 0;
	struct aicbsp_feature_t feature;
	//u8 val = 0;
	u8 val1 = 0;

	aicbsp_get_feature(&feature, NULL);
	aicwf_sdio_reg_init(sdiodev);

	host = sdiodev->func->card->host;

	sdio_claim_host(sdiodev->func);
	sdiodev->func->card->quirks |= MMC_QUIRK_LENIENT_FN0;

	ret = sdio_set_block_size(sdiodev->func, SDIOWIFI_FUNC_BLOCKSIZE);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "set blocksize fail %d\n", ret);
		sdio_release_host(sdiodev->func);
		return ret;
	}
	ret = sdio_enable_func(sdiodev->func);
	if (ret < 0) {
		sdio_release_host(sdiodev->func);
		AICWFDBG(LOGERROR, "enable func fail %d.\n", ret);
		return ret;
	}

	sdio_f0_writeb(sdiodev->func, 0x7F, 0xF2, &ret);
	if (ret) {
		sdio_err("set fn0 0xF2 fail %d\n", ret);
		sdio_release_host(sdiodev->func);
		return ret;
	}
#if 0
    if (host->ios.timing == MMC_TIMING_UHS_DDR50) {
        val = 0x21;//0x1D;//0x5;
    } else {
        val = 0x01;//0x19;//0x1;
    }
    val |= SDIOCLK_FREE_RUNNING_BIT;
    sdio_f0_writeb(sdiodev->func, val, 0xF0, &ret);
    if (ret) {
        sdio_err("set iopad ctrl fail %d\n", ret);
        sdio_release_host(sdiodev->func);
        return ret;
    }
    sdio_f0_writeb(sdiodev->func, 0x0, 0xF8, &ret);
    if (ret) {
        sdio_err("set iopad delay2 fail %d\n", ret);
        sdio_release_host(sdiodev->func);
        return ret;
    }
    sdio_f0_writeb(sdiodev->func, 0x20, 0xF1, &ret);
    if (ret) {
        sdio_err("set iopad delay1 fail %d\n", ret);
        sdio_release_host(sdiodev->func);
        return ret;
    }
    msleep(1);
#if 1 //SDIO CLOCK SETTING
	if ((feature.sdio_clock > 0) && (host->ios.timing != MMC_TIMING_UHS_DDR50)) {
		host->ios.clock = feature.sdio_clock;
		host->ops->set_ios(host, &host->ios);
		AICWFDBG(LOGINFO, "Set SDIO Clock %d MHz\n", host->ios.clock/1000000);
	}
#endif
#endif
	sdio_release_host(sdiodev->func);

	//1: no byte mode
	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.bytemode_enable_reg,
				byte_mode_disable);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "reg:%d write failed!\n",
			 sdiodev->sdio_reg.bytemode_enable_reg);
		return ret;
	}

	ret = aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.wakeup_reg, 0x11);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "reg:%d write failed!\n",
			 sdiodev->sdio_reg.wakeup_reg);
		return ret;
	}

#if 1
	mdelay(5);
	ret = aicwf_sdio_readb(sdiodev, sdiodev->sdio_reg.sleep_reg, &val1);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "reg:%d read failed!\n",
			 sdiodev->sdio_reg.sleep_reg);
		return ret;
	}

	if (!(val1 & 0x10)) {
		AICWFDBG(LOGERROR, "wakeup fail\n");
	} else {
		AICWFDBG(LOGINFO, "sdio ready\n");
	}
#endif
	return ret;
}

void aicwf_sdio_func_deinit(struct aic_sdio_dev *sdiodev)
{
	sdio_claim_host(sdiodev->func);
	sdio_disable_func(sdiodev->func);
	sdio_release_host(sdiodev->func);

	if (sdiodev->chipid == PRODUCT_ID_AIC8801 ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
	    sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
		sdio_claim_host(sdiodev->func2);
		sdio_disable_func(sdiodev->func2);
		sdio_release_host(sdiodev->func2);
	}
}

void *aicwf_sdio_bus_init(struct aic_sdio_dev *sdiodev)
{
	int ret;
	struct aicwf_bus *bus_if;
	struct aicwf_rx_priv *rx_priv;
	struct aicwf_tx_priv *tx_priv;

#if defined(CONFIG_SDIO_PWRCTRL)
	spin_lock_init(&sdiodev->pwrctl_lock);
	sema_init(&sdiodev->pwrctl_wakeup_sema, 1);
#endif

	bus_if = sdiodev->bus_if;
	bus_if->dev = sdiodev->dev;
	bus_if->ops = &aicwf_sdio_bus_ops;
	bus_if->state = BUS_DOWN_ST;
#if defined(CONFIG_SDIO_PWRCTRL)
	sdiodev->state = SDIO_SLEEP_ST;
	sdiodev->active_duration = SDIOWIFI_PWR_CTRL_INTERVAL;
#else
	sdiodev->state = SDIO_ACTIVE_ST;
#endif

	rx_priv = aicwf_rx_init(sdiodev);
	if (!rx_priv) {
		sdio_err("rx init fail\n");
		goto fail;
	}
	sdiodev->rx_priv = rx_priv;

	tx_priv = aicwf_tx_init(sdiodev);
	if (!tx_priv) {
		sdio_err("tx init fail\n");
		goto fail;
	}
	sdiodev->tx_priv = tx_priv;
	aicwf_frame_queue_init(&tx_priv->txq, 8, TXQLEN);
	spin_lock_init(&tx_priv->txqlock);
	sema_init(&tx_priv->txctl_sema, 1);
	sema_init(&tx_priv->cmd_txsema, 1);
	init_waitqueue_head(&tx_priv->cmd_txdone_wait);
	atomic_set(&tx_priv->tx_pktcnt, 0);

#if defined(CONFIG_SDIO_PWRCTRL)
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
	init_timer(&sdiodev->timer);
	sdiodev->timer.data = (ulong)sdiodev;
	sdiodev->timer.function = aicwf_sdio_bus_pwrctl;
#else
	timer_setup(&sdiodev->timer, aicwf_sdio_bus_pwrctl, 0);
#endif
	init_completion(&sdiodev->pwrctrl_trgg);
#ifdef AICWF_SDIO_SUPPORT
	sdiodev->pwrctl_tsk =
		kthread_run(aicwf_sdio_pwrctl_thread, sdiodev, "aicwf_pwrctl");
#endif
	if (IS_ERR(sdiodev->pwrctl_tsk)) {
		sdiodev->pwrctl_tsk = NULL;
	}
#endif
#ifdef CONFIG_TX_NETIF_FLOWCTRL
	sdiodev->flowctrl = 0;
	spin_lock_init(&sdiodev->tx_flow_lock);
#endif

#ifdef CONFIG_TEMP_CONTROL
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
	init_timer(&sdiodev->tp_ctrl_timer);
	sdiodev->tp_ctrl_timer.data = (ulong)sdiodev;
	sdiodev->tp_ctrl_timer.function = aicwf_temp_ctrl_timer;
	init_timer(&sdiodev->netif_timer);
	sdiodev->netif_timer.data = (ulong)sdiodev;
	sdiodev->netif_timer.function = aicwf_netif_timer;
#else
	timer_setup(&sdiodev->tp_ctrl_timer, aicwf_temp_ctrl_timer, 0);
	timer_setup(&sdiodev->netif_timer, aicwf_netif_timer, 0);
#endif
	INIT_WORK(&sdiodev->tp_ctrl_work, aicwf_temp_ctrl_worker);
	INIT_WORK(&sdiodev->netif_work, aicwf_netif_worker);
	mod_timer(&sdiodev->tp_ctrl_timer,
		  jiffies + msecs_to_jiffies(TEMP_GET_INTERVAL));
	sdiodev->net_stop = false;
	;
	sdiodev->on_off = true;
	sdiodev->cur_temp = 0;
	sdiodev->get_level = 0;
	sdiodev->set_level = 0;
	sdiodev->interval_t1 = TMR_INTERVAL_1;
	sdiodev->interval_t2 = TMR_INTERVAL_2;
	sdiodev->cur_stat = 0;
#endif

	ret = aicwf_bus_init(0, sdiodev->dev);
	if (ret < 0) {
		sdio_err("bus init fail\n");
		goto fail;
	}

	ret = aicwf_bus_start(bus_if);
	if (ret != 0) {
		sdio_err("bus start fail\n");
		goto fail;
	}

	return sdiodev;

fail:
	aicwf_sdio_release(sdiodev);
	return NULL;
}

uint8_t crc8_ponl_107(uint8_t *p_buffer, uint16_t cal_size)
{
	uint8_t i;
	uint8_t crc = 0;
	if (cal_size == 0) {
		return crc;
	}
	while (cal_size--) {
		for (i = 0x80; i > 0; i /= 2) {
			if (crc & 0x80) {
				crc *= 2;
				crc ^= 0x07; //polynomial X8 + X2 + X + 1,(0x107)
			} else {
				crc *= 2;
			}
			if ((*p_buffer) & i) {
				crc ^= 0x07;
			}
		}
		p_buffer++;
	}

	return crc;
}

#ifdef CONFIG_WIFI_SUSPEND_FOR_LINUX
void rwnx_set_wifi_suspend(char onoff)
{
	int ret = 0;
	if (onoff == '0') {
		printk("%s resume \r\n", __func__);
		rwnx_send_me_set_lp_level(g_rwnx_plat->sdiodev->rwnx_hw, 0, 1);
	} else {
		printk("%s suspend \r\n", __func__);
		ret = rwnx_send_me_set_lp_level(g_rwnx_plat->sdiodev->rwnx_hw,
						1, 0);
		if (!ret) {
			aicwf_sdio_pwr_stctl(g_rwnx_plat->sdiodev,
					     SDIO_SLEEP_ST);
			ret = aicwf_sdio_writeb(g_rwnx_plat->sdiodev,
						SDIOWIFI_WAKEUP_REG, 2);
			if (ret < 0) {
				sdio_err("reg:%d write failed!\n",
					 SDIOWIFI_WAKEUP_REG);
			}
		}
	}
}

static ssize_t rwnx_wifi_suspend_write_proc(struct file *file,
					    const char __user *buffer,
					    size_t count, loff_t *pos)
{
	char onoff;

	if (count < 1)
		return -EINVAL;

	if (copy_from_user(&onoff, buffer, 1))
		return -EFAULT;

	rwnx_set_wifi_suspend(onoff);

	return count;
}

static const struct file_operations wifi_suspend_fops = {
	.owner = THIS_MODULE,
	.write = rwnx_wifi_suspend_write_proc,
};

void rwnx_init_wifi_suspend_node(void)
{
	struct proc_dir_entry *ent;

	wifi_suspend_node = proc_mkdir("wifi_suspend", NULL);
	if (wifi_suspend_node == NULL) {
		printk("Unable to create /proc/wifi_suspend directory");
	}

	ent = proc_create("suspend", 0660, wifi_suspend_node,
			  &wifi_suspend_fops);
	if (ent == NULL) {
		printk("Unable to create /proc/wifi_suspend/suspend");
	}
}

void rwnx_deinit_wifi_suspend_node(void)
{
	remove_proc_entry("suspend", wifi_suspend_node);
	remove_proc_entry("wifi_suspend", 0);
}
#endif //CONFIG_WIFI_SUSPEND_FOR_LINUX
