// SPDX-License-Identifier: GPL-2.0+
/**
 *
 * Driver for ROCKCHIP RK630 Ethernet PHYs
 *
 * Copyright (c) 2020, Rockchip Electronics Co., Ltd
 *
 * David Wu <david.wu@rock-chips.com>
 *
 */

#include <linux/ethtool.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
#include <linux/mii.h>
#include <linux/netdevice.h>
#include <linux/nvmem-consumer.h>
#include <linux/of_irq.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/thermal.h>
#include <linux/wakelock.h>

#define RK630_PHY_ID				0x00441400

/* PAGE 0 */
#define REG_MMD_ACCESS_CONTROL			0x0d
#define REG_MMD_ACCESS_DATA_ADDRESS		0x0e
#define REG_INTERRUPT_STATUS			0X10
#define REG_INTERRUPT_MASK			0X11
#define REG_GLOBAL_CONFIGURATION		0X13
#define REG_MAC_ADDRESS0			0x16
#define REG_MAC_ADDRESS1			0x17
#define REG_MAC_ADDRESS2			0x18

#define REG_PAGE_SEL				0x1F

/* PAGE 1 */
#define REG_PAGE1_APS_CTRL			0x12
#define REG_PAGE1_UAPS_CONFIGURE		0X13
#define REG_PAGE1_EEE_CONFIGURE			0x17

/* PAGE 2 */
#define REG_PAGE2_AFE_CTRL			0x18

/* PAGE 6 */
#define REG_PAGE6_ADC_ANONTROL			0x10
#define REG_PAGE6_GAIN_ANONTROL			0x12
#define REG_PAGE6_AFE_RX_CTRL			0x13
#define REG_PAGE6_AFE_TX_CTRL			0x14
#define REG_PAGE6_AFE_DRIVER2			0x15
#define REG_PAGE6_CP_CURRENT			0x17
#define REG_PAGE6_ADC_OP_BIAS			0x18
#define REG_PAGE6_RX_DECTOR			0x19
#define REG_PAGE6_TX_MOS_DRV			0x1B
#define REG_PAGE6_AFE_PDCW			0x1c

/* PAGE 8 */
#define REG_PAGE8_AFE_CTRL			0x18
#define REG_PAGE8_AUTO_CAL			0x1d

/*
 * Fixed address:
 * Addr: 1 --- RK630@S40
 *       2 --- RV1106@T22
 */
#define PHY_ADDR_S40				1
#define PHY_ADDR_T22				2

#define T22_TX_LEVEL_100M			0x2d
#define T22_TX_LEVEL_10M			0x32

/* Long network cable parameters */
#define RX_DETECT_SCHEDULE_TIME			500 /* ms */
#define RX_DETECT_INIT_WAIT_TIME		2000 /* ms */

#define RX_DETECT_MAX_COUNT			(5000 / RX_DETECT_SCHEDULE_TIME)
#define ALL_RX_DETECT_MAX_COUNT			(2 * RX_DETECT_MAX_COUNT)

#define LINKED_MAX_COUNT			(10000 / RX_DETECT_SCHEDULE_TIME)
#define ALL_LINKED_MAX_COUNT			(2 * LINKED_MAX_COUNT)

#define RX_PACKET_RECEIVED_COUNTS		3 /* packets */
#define RX_PACKET_RECEIVED_LOST			15 /* percent */

#define RX_SIGNAL_DETECT_TEMP			85000

struct rk630_phy_switched {
	/* record state */
	bool config;
	bool config_mode_10M;
	bool finished;

	/* detected process */
	unsigned int detected_count;
	bool config_rx_signal;
	int old_link;

	/* linked process */
	unsigned int linked_count;
	int rx_pkt_cnt;
	int rx_crc_err_cnt;
	int lost_percent;
};

struct rk630_phy_priv {
	struct phy_device *phydev;
	bool ieee;
	int wol_irq;
	struct wake_lock wol_wake_lock;
	int tx_level_100M;
	int tx_level_10M;

	struct rk630_phy_switched switched;
	/* mutex protect variables between notify thread and delayed work */
	struct mutex lock;
	struct delayed_work service_task;
	struct thermal_zone_device *tz;
	bool disable_switch;
};

static void rk630_phy_t22_get_tx_level_from_efuse(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;
	unsigned int tx_level_100M = T22_TX_LEVEL_100M;
	unsigned int tx_level_10M = T22_TX_LEVEL_10M;
	unsigned char *efuse_buf;
	struct nvmem_cell *cell;
	size_t len;

	cell = nvmem_cell_get(&phydev->mdio.dev, "txlevel");
	if (IS_ERR(cell)) {
		phydev_err(phydev, "failed to get txlevel cell: %ld, use default\n",
			   PTR_ERR(cell));
	} else {
		efuse_buf = nvmem_cell_read(cell, &len);
		nvmem_cell_put(cell);
		if (!IS_ERR(efuse_buf)) {
			if (len == 2 && efuse_buf[0] > 0 && efuse_buf[1] > 0) {
				tx_level_100M = efuse_buf[1];
				tx_level_10M = efuse_buf[0];
			}
			kfree(efuse_buf);
		} else {
			phydev_err(phydev, "failed to get efuse buf, use default\n");
		}
	}

	priv->tx_level_100M = tx_level_100M;
	priv->tx_level_10M = tx_level_10M;
}

static void rk630_phy_wol_enable(struct phy_device *phydev)
{
	struct net_device *ndev = phydev->attached_dev;
	u32 value;

	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
	phy_write(phydev, REG_MAC_ADDRESS0, ((u16)ndev->dev_addr[0] << 8) + ndev->dev_addr[1]);
	phy_write(phydev, REG_MAC_ADDRESS1, ((u16)ndev->dev_addr[2] << 8) + ndev->dev_addr[3]);
	phy_write(phydev, REG_MAC_ADDRESS2, ((u16)ndev->dev_addr[4] << 8) + ndev->dev_addr[5]);

	value = phy_read(phydev, REG_GLOBAL_CONFIGURATION);
	value |= BIT(8);
	value &= ~BIT(7);
	value |= BIT(10);
	phy_write(phydev, REG_GLOBAL_CONFIGURATION, value);

	value = phy_read(phydev, REG_INTERRUPT_MASK);
	value |= BIT(14);
	phy_write(phydev, REG_INTERRUPT_MASK, value);
}

static void rk630_phy_wol_disable(struct phy_device *phydev)
{
	u32 value;

	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
	value = phy_read(phydev, REG_GLOBAL_CONFIGURATION);
	value &= ~BIT(10);
	phy_write(phydev, REG_GLOBAL_CONFIGURATION, value);
}

static void rk630_phy_ieee_set(struct phy_device *phydev, bool enable)
{
	u32 value;

	/* Switch to page 1 */
	phy_write(phydev, REG_PAGE_SEL, 0x0100);
	value = phy_read(phydev, REG_PAGE1_EEE_CONFIGURE);
	if (enable)
		value |= BIT(3);
	else
		value &= ~BIT(3);
	phy_write(phydev, REG_PAGE1_EEE_CONFIGURE, value);
	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
}

static void rk630_phy_set_aps(struct phy_device *phydev, bool enable)
{
	u32 value;

	/* Switch to page 1 */
	phy_write(phydev, REG_PAGE_SEL, 0x0100);
	value = phy_read(phydev, REG_PAGE1_APS_CTRL);
	if (enable)
		value |= BIT(15);
	else
		value &= ~BIT(15);
	phy_write(phydev, REG_PAGE1_APS_CTRL, value);
	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
}

static void rk630_phy_set_uaps(struct phy_device *phydev, bool enable)
{
	u32 value;

	/* Switch to page 1 */
	phy_write(phydev, REG_PAGE_SEL, 0x0100);
	value = phy_read(phydev, REG_PAGE1_UAPS_CONFIGURE);
	if (enable)
		value |= BIT(15);
	else
		value &= ~BIT(15);
	phy_write(phydev, REG_PAGE1_UAPS_CONFIGURE, value);
	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
}

static bool rk630_phy_rx_signal_detected(struct phy_device *phydev)
{
	u32 value;

	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
	value = phy_read(phydev, 25);

	return (value & BIT(15)) ? false : true;
}

static void rk630_phy_s40_config_init(struct phy_device *phydev)
{
	phy_write(phydev, 0, phy_read(phydev, 0) & ~BIT(13));

	/* Switch to page 1 */
	phy_write(phydev, REG_PAGE_SEL, 0x0100);
	/* Disable APS */
	phy_write(phydev, REG_PAGE1_APS_CTRL, 0x4824);
	/* Switch to page 2 */
	phy_write(phydev, REG_PAGE_SEL, 0x0200);
	/* PHYAFE TRX optimization */
	phy_write(phydev, REG_PAGE2_AFE_CTRL, 0x0000);
	/* Switch to page 6 */
	phy_write(phydev, REG_PAGE_SEL, 0x0600);
	/* PHYAFE TX optimization */
	phy_write(phydev, REG_PAGE6_AFE_TX_CTRL, 0x708f);
	/* PHYAFE RX optimization */
	phy_write(phydev, REG_PAGE6_AFE_RX_CTRL, 0xf000);
	phy_write(phydev, REG_PAGE6_AFE_DRIVER2, 0x1530);

	/* Switch to page 8 */
	phy_write(phydev, REG_PAGE_SEL, 0x0800);
	/* PHYAFE TRX optimization */
	phy_write(phydev, REG_PAGE8_AFE_CTRL, 0x00bc);

	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
}

static void rk630_phy_t22_config_init(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;

	/* Switch to page 1 */
	phy_write(phydev, REG_PAGE_SEL, 0x0100);
	/* Enable offset clock */
	phy_write(phydev, 0x10, 0xfbfe);
	/* Disable APS & Rx detected time 2s, default is 4s */
	phy_write(phydev, REG_PAGE1_APS_CTRL, 0x4822);
	/* Switch to page 2 */
	phy_write(phydev, REG_PAGE_SEL, 0x0200);
	/* PHYAFE TRX optimization */
	phy_write(phydev, REG_PAGE2_AFE_CTRL, 0x0000);
	/* Switch to page 6 */
	phy_write(phydev, REG_PAGE_SEL, 0x0600);
	/* PHYAFE ADC optimization */
	phy_write(phydev, REG_PAGE6_ADC_ANONTROL, 0x5540);
	/* PHYAFE Gain optimization */
	phy_write(phydev, REG_PAGE6_GAIN_ANONTROL, 0x0400);
	/* PHYAFE EQ optimization */

	if (priv->disable_switch) {
		/* Rx detected default threshold 160 mv */
		phy_write(phydev, REG_PAGE6_AFE_TX_CTRL, 0x1088);
	} else {
		/* Rx detected threshold 260 mv */
		phy_write(phydev, REG_PAGE6_AFE_TX_CTRL, 0x10c8);
		priv->switched.config_rx_signal = true;
	}

	if (priv->tx_level_100M <= 0 || priv->tx_level_10M <= 0)
		rk630_phy_t22_get_tx_level_from_efuse(phydev);

	/* PHYAFE TX optimization */
	phy_write(phydev, REG_PAGE6_AFE_DRIVER2,
		  (priv->tx_level_100M << 8) | priv->tx_level_10M);
	/* PHYAFE CP current optimization */
	phy_write(phydev, REG_PAGE6_CP_CURRENT, 0x0575);
	/* ADC OP BIAS optimization */
	phy_write(phydev, REG_PAGE6_ADC_OP_BIAS, 0x0000);
	/* Rx signal detctor level optimization */
	phy_write(phydev, REG_PAGE6_RX_DECTOR, 0x0408);
	/* PHYAFE PDCW optimization */
	phy_write(phydev, REG_PAGE6_AFE_PDCW, 0x8880);
	/* Add PHY Tx mos drive, reduce power noise/jitter */
	phy_write(phydev, REG_PAGE6_TX_MOS_DRV, 0x888e);

	/* Switch to page 8 */
	phy_write(phydev, REG_PAGE_SEL, 0x0800);
	/* Disable auto-cal */
	phy_write(phydev, REG_PAGE8_AUTO_CAL, 0x0844);
	/* Reatart offset calibration */
	phy_write(phydev, 0x13, 0xc096);

	/* Switch to page 0 */
	phy_write(phydev, REG_PAGE_SEL, 0x0000);

	/* Disable eee mode advertised */
	phy_write(phydev, REG_MMD_ACCESS_CONTROL, 0x0007);
	phy_write(phydev, REG_MMD_ACCESS_DATA_ADDRESS, 0x003c);
	phy_write(phydev, REG_MMD_ACCESS_CONTROL, 0x4007);
	phy_write(phydev, REG_MMD_ACCESS_DATA_ADDRESS, 0x0000);
}

static int rk630_phy_config_init(struct phy_device *phydev)
{
	switch (phydev->mdio.addr) {
	case PHY_ADDR_S40:
		rk630_phy_s40_config_init(phydev);
		/*
		 * Ultra Auto-Power Saving Mode (UAPS) is designed to
		 * save power when cable is not plugged into PHY.
		 */
		rk630_phy_set_uaps(phydev, true);
		break;
	case PHY_ADDR_T22:
		rk630_phy_t22_config_init(phydev);
		rk630_phy_set_aps(phydev, false);
		rk630_phy_set_uaps(phydev, false);
		break;
	default:
		phydev_err(phydev, "Unsupported address for current phy: %d\n",
			   phydev->mdio.addr);
		return -EINVAL;
	}

	rk630_phy_ieee_set(phydev, true);

	return 0;
}

/* config0(default) and config1(0x555e) switched for 100/10M speed */
static bool rk630_phy_switch_config(struct phy_device *phydev, bool config)
{
	struct rk630_phy_priv *priv = phydev->priv;

	if (priv->switched.config != config) {
		int val;

		val = config ? 0x555e : 0x5540;
		phy_write(priv->phydev, REG_PAGE_SEL, 0x0600);
		phy_write(priv->phydev, REG_PAGE6_ADC_ANONTROL, val);
		phy_write(priv->phydev, REG_PAGE_SEL, 0x0000);
		priv->switched.config = config;

		return true;
	}

	return false;
}

/* 10M speed configuration */
static void rk630_phy_10m_switch_config(struct phy_device *phydev, bool config)
{
	struct rk630_phy_priv *priv = phydev->priv;
	unsigned int val;

	if (config == priv->switched.config_mode_10M)
		return;

	phy_write(phydev, REG_PAGE_SEL, 0x0600);
	val = phy_read(phydev, REG_PAGE6_AFE_TX_CTRL);
	val &= ~GENMASK(14, 13);
	if (config && !priv->switched.config_mode_10M)
		val |= BIT(13);

	priv->switched.config_mode_10M = config;
	phy_write(phydev, REG_PAGE6_AFE_TX_CTRL, val);
	phy_write(priv->phydev, REG_PAGE_SEL, 0x0000);
}

static void rk630_phy_switch_rx_signal_config(struct phy_device *phydev,
					      bool config)
{
	struct rk630_phy_priv *priv = phydev->priv;

	if (priv->switched.config_rx_signal != config) {
		int val;

		phy_write(priv->phydev, REG_PAGE_SEL, 0x0600);
		val = phy_read(phydev, REG_PAGE6_AFE_TX_CTRL);
		val &= ~GENMASK(7, 6);
		if (config)
			val |= GENMASK(7, 6);
		else
			val |= BIT(7);
		phy_write(phydev, REG_PAGE6_AFE_TX_CTRL, val);
		phy_write(priv->phydev, REG_PAGE_SEL, 0x0000);

		priv->switched.config_rx_signal = config;
	}
}

static void rk630_phy_packet_statistics(struct phy_device *phydev,
					int *total_cnt, int *crc_err_cnt)
{
	struct rk630_phy_priv *priv = phydev->priv;

	phy_write(priv->phydev, REG_PAGE_SEL, 0x0900);
	*total_cnt = phy_read(priv->phydev, 0x1b) << 16;
	*total_cnt |= phy_read(priv->phydev, 0x1c);
	*crc_err_cnt = phy_read(priv->phydev, 0x1d) << 16;
	*crc_err_cnt |= phy_read(priv->phydev, 0x1e);
	phy_write(phydev, REG_PAGE_SEL, 0x0000);
}

static bool rk630_phy_switch_config_by_packets(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;
	int rx_pkt_cnt, rx_crc_err_cnt;
	int total_cnt, total_crc_err_cnt;
	int lost_percent;

	rk630_phy_packet_statistics(phydev, &total_cnt, &total_crc_err_cnt);

	rx_pkt_cnt = total_cnt - priv->switched.rx_pkt_cnt;
	rx_crc_err_cnt = total_crc_err_cnt - priv->switched.rx_crc_err_cnt;

	priv->switched.rx_pkt_cnt = total_cnt;
	priv->switched.rx_crc_err_cnt = total_crc_err_cnt;

	/* less than the minimal received packets during some time */
	if (rx_pkt_cnt < RX_PACKET_RECEIVED_COUNTS)
		return true;

	/* Percents packets lost is not good during some time */
	lost_percent = (rx_crc_err_cnt * 100 / rx_pkt_cnt) > RX_PACKET_RECEIVED_LOST;

	/* Just compare with config0's packet lost, update config if it is better
	 * than config0.
	 */
	if (((rx_crc_err_cnt * 100 / rx_pkt_cnt) > RX_PACKET_RECEIVED_LOST) &&
	    lost_percent > priv->switched.lost_percent) {
		/* Only save config0 lost percent */
		if (!priv->switched.config)
			priv->switched.lost_percent = lost_percent;
		return true;
	}

	/* Only save config0 lost percent */
	if (!priv->switched.config)
		priv->switched.lost_percent = lost_percent;

	return false;
}

static void rk630_phy_service_task(struct work_struct *work)
{
	struct rk630_phy_priv *priv = container_of(work, struct rk630_phy_priv,
						   service_task.work);
	unsigned int delay_time;
	int ret, temp;

	mutex_lock(&priv->lock);
	if (priv->disable_switch) {
		mutex_unlock(&priv->lock);
		return;
	}

	if (!priv->phydev->link) {
		bool signal_detected;

		signal_detected = rk630_phy_rx_signal_detected(priv->phydev);

		/* Read signal */
		if (!signal_detected) {
			/* Slow schedule work for 2 * SCHEDULE_TIME, if no signal */
			priv->switched.detected_count = 0;
			priv->switched.lost_percent = 0;
			priv->switched.finished = false;
			priv->switched.linked_count = 0;
			delay_time = 2 * RX_DETECT_SCHEDULE_TIME;
			/* Goto default config if no rj45 signal plugin */
			rk630_phy_switch_config(priv->phydev, false);

			/* Also go to 10M default config */
			rk630_phy_10m_switch_config(priv->phydev, false);
		} else {
			priv->switched.detected_count++;
			/* Fast schedule work for 1 * SCHEDULE_TIME, if signal
			 * detected.
			 */
			delay_time = RX_DETECT_SCHEDULE_TIME;
			if (priv->switched.detected_count == RX_DETECT_MAX_COUNT &&
			    !priv->switched.finished) {
				/* After it, there is no link, Might be a long cable,
				 * config1 switched to get better performance during
				 * some time.
				 */
				rk630_phy_switch_config(priv->phydev, true);
			} else if (priv->switched.detected_count == ALL_RX_DETECT_MAX_COUNT &&
				   !priv->switched.finished) {
				/* After another detect, we lost the last chance,
				 * go back to default config0.
				 */
				rk630_phy_switch_config(priv->phydev, false);
				priv->switched.finished = true;
			} else if (priv->switched.detected_count > ALL_RX_DETECT_MAX_COUNT ||
				   priv->switched.finished) {
				/* Slow schedule work for 2 * SCHEDULE_TIME, if
				 * detected finish.
				 */
				delay_time = 2 * RX_DETECT_SCHEDULE_TIME;
			}
		}
	} else {
		/* Detect the packet count and crc error count statistics */
		priv->switched.linked_count++;
		/* Fast schedule work for 1 * SCHEDULE_TIME, if linkup detected */
		delay_time = RX_DETECT_SCHEDULE_TIME;
		if (priv->switched.linked_count == LINKED_MAX_COUNT &&
		    !priv->switched.finished) {
			if (rk630_phy_switch_config_by_packets(priv->phydev)) {
				/* Config1 switched to get better performance */
				rk630_phy_switch_config(priv->phydev, true);

				/* Also go to 10M default config */
				if (priv->switched.config && priv->phydev->speed == SPEED_10)
					rk630_phy_10m_switch_config(priv->phydev, true);
			}
		} else if (priv->switched.linked_count == ALL_LINKED_MAX_COUNT &&
			   !priv->switched.finished) {
			/* If config switched, we lost the last chance, return to
			 * default config0.
			 */
			if (rk630_phy_switch_config_by_packets(priv->phydev)) {
				rk630_phy_switch_config(priv->phydev, false);
				rk630_phy_10m_switch_config(priv->phydev, false);
			}
			priv->switched.finished = true;
		} else if (priv->switched.linked_count > ALL_LINKED_MAX_COUNT ||
			   priv->switched.finished) {
			/* Slow schedule work for 2 * SCHEDULE_TIME, if linkup
			 * detected finish.
			 */
			delay_time = 2 * RX_DETECT_SCHEDULE_TIME;
		}
	}

	if (priv->tz) {
		ret = thermal_zone_get_temp(priv->tz, &temp);
		if (ret || temp == THERMAL_TEMP_INVALID)
			phydev_err(priv->phydev,
				   "failed to read out thermal zone (%d)\n", ret);
		else
			rk630_phy_switch_rx_signal_config(priv->phydev,
					(temp > RX_SIGNAL_DETECT_TEMP) ? false : true);
	}

	schedule_delayed_work(&priv->service_task, msecs_to_jiffies(delay_time));
	mutex_unlock(&priv->lock);
}

static void rk630_phy_link_change_notify(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;

	if (phydev->mdio.addr == PHY_ADDR_T22) {
		mutex_lock(&priv->lock);
		if (priv->disable_switch) {
			mutex_unlock(&priv->lock);
			return;
		}

		if (priv->switched.old_link && !phydev->link) {
			priv->switched.old_link = 0;
			priv->switched.linked_count = 0;
			schedule_delayed_work(&priv->service_task,
					      msecs_to_jiffies(RX_DETECT_SCHEDULE_TIME));
		} else if (!priv->switched.old_link && phydev->link) {
			/* If linked, keep current config, but if the linked is
			 * 10M speed, and config1 has been enabled, also switched
			 * the 10M config.
			 */
			if (priv->switched.config && phydev->speed == SPEED_10)
				rk630_phy_10m_switch_config(phydev, true);

			priv->switched.old_link = 1;
			priv->switched.detected_count = 0;
			/* Record base packet statistics to compare later, if linked */
			if (!priv->switched.linked_count)
				rk630_phy_packet_statistics(priv->phydev,
							    &priv->switched.rx_pkt_cnt,
							    &priv->switched.rx_crc_err_cnt);
			schedule_delayed_work(&priv->service_task,
					      msecs_to_jiffies(RX_DETECT_SCHEDULE_TIME));
		}
		mutex_unlock(&priv->lock);
	}
}

static ssize_t rk630_phy_disable_switch_store(struct device *dev,
					      struct device_attribute *attr,
					      const char *buf, size_t count)
{
	struct phy_device *phydev = to_phy_device(dev);
	struct rk630_phy_priv *priv = phydev->priv;
	int ret;
	bool disabled;

	ret = kstrtobool(buf, &disabled);
	if (ret)
		return count;

	mutex_lock(&priv->lock);
	if (disabled) {
		cancel_delayed_work_sync(&priv->service_task);

		/* Save to default config */
		rk630_phy_10m_switch_config(priv->phydev, false);
		rk630_phy_switch_rx_signal_config(priv->phydev, false);
		rk630_phy_switch_config(priv->phydev, false);

		memset(&priv->switched, 0, sizeof(struct rk630_phy_switched));
	} else {
		priv->switched.old_link = phydev->link;
		/* Rx detected threshold 260 mv */
		rk630_phy_switch_rx_signal_config(priv->phydev, true);

		schedule_delayed_work(&priv->service_task,
				      msecs_to_jiffies(RX_DETECT_INIT_WAIT_TIME));
	}
	priv->disable_switch = disabled;
	dev_info(dev, "rk630 phy disable switch to %s\n", disabled ? "true" : "false");
	mutex_unlock(&priv->lock);

	return count;
}
static DEVICE_ATTR_WO(rk630_phy_disable_switch);

static irqreturn_t rk630_wol_irq_thread(int irq, void *dev_id)
{
	struct rk630_phy_priv *priv = (struct rk630_phy_priv *)dev_id;

	phy_write(priv->phydev, REG_INTERRUPT_STATUS, BIT(14));
	wake_lock_timeout(&priv->wol_wake_lock, msecs_to_jiffies(8000));
	return IRQ_HANDLED;
}

static int rk630_phy_probe(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv;
	const char *tz_name;
	int ret;

	priv = devm_kzalloc(&phydev->mdio.dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	phydev->priv = priv;

	priv->wol_irq = of_irq_get_byname(phydev->mdio.dev.of_node, "wol_irq");
	if (priv->wol_irq == -EPROBE_DEFER)
		return priv->wol_irq;

	if (priv->wol_irq > 0) {
		wake_lock_init(&priv->wol_wake_lock,
			       WAKE_LOCK_SUSPEND, "wol_wake_lock");
		ret = devm_request_threaded_irq(&phydev->mdio.dev, priv->wol_irq,
						NULL, rk630_wol_irq_thread,
						IRQF_TRIGGER_FALLING | IRQF_SHARED | IRQF_ONESHOT,
						"wol_irq", priv);
		if (ret) {
			wake_lock_destroy(&priv->wol_wake_lock);
			phydev_err(phydev, "request wol_irq failed: %d\n", ret);
			return ret;
		}
		disable_irq(priv->wol_irq);
		enable_irq_wake(priv->wol_irq);
	}

	mutex_init(&priv->lock);
	INIT_DELAYED_WORK(&priv->service_task, rk630_phy_service_task);

	priv->disable_switch = of_property_read_bool(phydev->mdio.dev.of_node,
						     "rk630,phy-disable-switch");
	of_property_read_string(phydev->mdio.dev.of_node, "rockchip,thermal-zone",
				&tz_name);
	priv->tz = thermal_zone_get_zone_by_name(tz_name);
	if (IS_ERR(priv->tz)) {
		pr_warn("Error getting thermal zone, not yet ready?\n");
		priv->tz = NULL;
	}

	ret = device_create_file(&phydev->mdio.dev, &dev_attr_rk630_phy_disable_switch);
	if (ret)
		return ret;

	priv->phydev = phydev;

	return 0;
}

static void rk630_phy_remove(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;

	if (priv->wol_irq > 0)
		wake_lock_destroy(&priv->wol_wake_lock);
}

static int rk630_phy_suspend(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;

	if (priv->wol_irq > 0) {
		rk630_phy_wol_enable(phydev);
		phy_write(phydev, REG_INTERRUPT_MASK, BIT(14));
		enable_irq(priv->wol_irq);
	}

	if (!priv->disable_switch)
		cancel_delayed_work_sync(&priv->service_task);

	return genphy_suspend(phydev);
}

static int rk630_phy_resume(struct phy_device *phydev)
{
	struct rk630_phy_priv *priv = phydev->priv;

	if (priv->wol_irq > 0) {
		rk630_phy_wol_disable(phydev);
		phy_write(phydev, REG_INTERRUPT_MASK, 0);
		disable_irq(priv->wol_irq);
	}

	if (!priv->disable_switch)
		schedule_delayed_work(&priv->service_task,
				      msecs_to_jiffies(RX_DETECT_INIT_WAIT_TIME));

	return genphy_resume(phydev);
}

static struct phy_driver rk630_phy_driver[] = {
{
	.phy_id			= RK630_PHY_ID,
	.phy_id_mask		= 0xffffffff,
	.name			= "RK630 PHY",
	.features		= PHY_BASIC_FEATURES,
	.flags			= 0,
	.link_change_notify	= rk630_phy_link_change_notify,
	.probe			= rk630_phy_probe,
	.remove			= rk630_phy_remove,
	.soft_reset		= genphy_soft_reset,
	.config_init		= rk630_phy_config_init,
	.config_aneg		= genphy_config_aneg,
	.read_status		= genphy_read_status,
	.suspend		= rk630_phy_suspend,
	.resume			= rk630_phy_resume,
},
};

static struct mdio_device_id __maybe_unused rk630_phy_tbl[] = {
	{ RK630_PHY_ID, 0xffffffff },
	{ }
};

MODULE_DEVICE_TABLE(mdio, rk630_phy_tbl);

module_phy_driver(rk630_phy_driver);

MODULE_AUTHOR("David Wu <david.wu@rock-chips.com>");
MODULE_DESCRIPTION("Rockchip RK630 Ethernet PHY driver");
MODULE_LICENSE("GPL v2");
