/*
 * (C) Copyright 2015 Sjoerd Simons <sjoerd.simons@collabora.co.uk>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Rockchip GMAC ethernet IP driver for U-Boot
 */

#include <common.h>
#include <dm.h>
#include <clk.h>
#include <misc.h>
#include <phy.h>
#include <reset.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/arch/periph.h>
#include <asm/arch/clock.h>
#include <asm/arch/hardware.h>
#ifdef CONFIG_DWC_ETH_QOS
#include <asm/arch/grf_rk3528.h>
#include <asm/arch/grf_rk3562.h>
#include <asm/arch/ioc_rk3562.h>
#include <asm/arch/grf_rk3568.h>
#include <asm/arch/grf_rk3588.h>
#include <asm/arch/grf_rv1106.h>
#include <asm/arch/grf_rv1126.h>
#include "dwc_eth_qos.h"
#else
#include <asm/arch/grf_px30.h>
#include <asm/arch/grf_rk1808.h>
#include <asm/arch/grf_rk322x.h>
#include <asm/arch/grf_rk3288.h>
#include <asm/arch/grf_rk3308.h>
#include <asm/arch/grf_rk3328.h>
#include <asm/arch/grf_rk3368.h>
#include <asm/arch/grf_rk3399.h>
#include <asm/arch/grf_rv1108.h>
#include "designware.h"
#include <dt-bindings/clock/rk3288-cru.h>
#endif
#include <dm/pinctrl.h>
#include <dm/of_access.h>

DECLARE_GLOBAL_DATA_PTR;

struct rockchip_eth_dev {
#ifdef CONFIG_DWC_ETH_QOS
	struct eqos_priv eqos;
#else
	struct dw_eth_dev dw;
#endif
	int phy_interface;
};

/*
 * Platform data for the gmac
 *
 * dw_eth_pdata: Required platform data for designware driver (must be first)
 */
struct gmac_rockchip_platdata {
#ifndef CONFIG_DWC_ETH_QOS
	struct dw_eth_pdata dw_eth_pdata;
#else
	struct eth_pdata eth_pdata;
#endif
	struct reset_ctl phy_reset;
	bool integrated_phy;
	bool clock_input;
	int phy_interface;
	int tx_delay;
	int rx_delay;
	int bus_id;
};

struct rk_gmac_ops {
#ifdef CONFIG_DWC_ETH_QOS
	const struct eqos_config config;
#endif
	int (*fix_mac_speed)(struct gmac_rockchip_platdata *pdata,
			     struct rockchip_eth_dev *dev);
	void (*set_to_rmii)(struct gmac_rockchip_platdata *pdata);
	void (*set_to_rgmii)(struct gmac_rockchip_platdata *pdata);
	void (*set_clock_selection)(struct gmac_rockchip_platdata *pdata);
	void (*integrated_phy_powerup)(struct gmac_rockchip_platdata *pdata);
};

#ifdef CONFIG_DWC_ETH_QOS
static const struct eqos_config eqos_rockchip_config = {
	.reg_access_always_ok = false,
	.mdio_wait = 10000,
	.swr_wait = 200,
	.config_mac = EQOS_MAC_RXQ_CTRL0_RXQ0EN_NOT_ENABLED,
	.config_mac_mdio = EQOS_MAC_MDIO_ADDRESS_CR_100_150,
	.ops = &eqos_rockchip_ops,
};
#endif

void gmac_set_rgmii(struct udevice *dev, u32 tx_delay, u32 rx_delay)
{
	struct gmac_rockchip_platdata *pdata = dev_get_platdata(dev);
	struct rk_gmac_ops *ops =
		(struct rk_gmac_ops *)dev_get_driver_data(dev);

	pdata->tx_delay = tx_delay;
	pdata->rx_delay = rx_delay;

	ops->set_to_rgmii(pdata);
}

static int gmac_rockchip_ofdata_to_platdata(struct udevice *dev)
{
	struct gmac_rockchip_platdata *pdata = dev_get_platdata(dev);
	struct ofnode_phandle_args args;
	struct udevice *phydev;
	const char *string;
	int ret;

	string = dev_read_string(dev, "clock_in_out");
	if (!strcmp(string, "input"))
		pdata->clock_input = true;
	else
		pdata->clock_input = false;

	/* If phy-handle property is passed from DT, use it as the PHY */
	ret = dev_read_phandle_with_args(dev, "phy-handle", NULL, 0, 0, &args);
	if (ret) {
		debug("Cannot get phy phandle: ret=%d\n", ret);
		pdata->integrated_phy = dev_read_bool(dev, "phy-is-integrated");
	} else {
		debug("Found phy-handle subnode\n");
		pdata->integrated_phy = ofnode_read_bool(args.node,
							 "phy-is-integrated");
	}

	if (pdata->integrated_phy) {
		ret = reset_get_by_name(dev, "mac-phy", &pdata->phy_reset);
		if (ret) {
			ret = uclass_get_device_by_ofnode(UCLASS_ETH_PHY, args.node, &phydev);
			if (ret) {
				debug("Get phydev by ofnode failed: err=%d\n", ret);
				return ret;
			}

			ret = reset_get_by_index(phydev, 0, &pdata->phy_reset);
			if (ret) {
				debug("No PHY reset control found: ret=%d\n", ret);
				return ret;
			}
		}
	}

	/* Check the new naming-style first... */
	pdata->tx_delay = dev_read_u32_default(dev, "tx_delay", -ENOENT);
	pdata->rx_delay = dev_read_u32_default(dev, "rx_delay", -ENOENT);

	/* ... and fall back to the old naming style or default, if necessary */
	if (pdata->tx_delay == -ENOENT)
		pdata->tx_delay = dev_read_u32_default(dev, "tx-delay", 0x30);
	if (pdata->rx_delay == -ENOENT)
		pdata->rx_delay = dev_read_u32_default(dev, "rx-delay", 0x10);

#ifdef CONFIG_DWC_ETH_QOS
	return 0;
#else
	return designware_eth_ofdata_to_platdata(dev);
#endif
}

#ifndef CONFIG_DWC_ETH_QOS
static int px30_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				   struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct px30_grf *grf;
	struct clk clk_speed;
	int speed, ret;
	enum {
		PX30_GMAC_SPEED_SHIFT = 0x2,
		PX30_GMAC_SPEED_MASK  = BIT(2),
		PX30_GMAC_SPEED_10M   = 0,
		PX30_GMAC_SPEED_100M  = BIT(2),
	};

	ret = clk_get_by_name(priv->phydev->dev, "clk_mac_speed",
			      &clk_speed);
	if (ret)
		return ret;

	switch (priv->phydev->speed) {
	case 10:
		speed = PX30_GMAC_SPEED_10M;
		ret = clk_set_rate(&clk_speed, 2500000);
		if (ret)
			return ret;
		break;
	case 100:
		speed = PX30_GMAC_SPEED_100M;
		ret = clk_set_rate(&clk_speed, 25000000);
		if (ret)
			return ret;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con1, PX30_GMAC_SPEED_MASK, speed);

	return 0;
}

static int rk1808_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct clk clk_speed;
	int ret;

	ret = clk_get_by_name(priv->phydev->dev, "clk_mac_speed",
			      &clk_speed);
	if (ret)
		return ret;

	switch (priv->phydev->speed) {
	case 10:
		ret = clk_set_rate(&clk_speed, 2500000);
		if (ret)
			return ret;
		break;
	case 100:
		ret = clk_set_rate(&clk_speed, 25000000);
		if (ret)
			return ret;
		break;
	case 1000:
		ret = clk_set_rate(&clk_speed, 125000000);
		if (ret)
			return ret;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	return 0;
}

static int rk3228_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rk322x_grf *grf;
	int clk;
	enum {
		RK3228_GMAC_CLK_SEL_SHIFT = 8,
		RK3228_GMAC_CLK_SEL_MASK  = GENMASK(9, 8),
		RK3228_GMAC_CLK_SEL_125M  = 0 << 8,
		RK3228_GMAC_CLK_SEL_25M   = 3 << 8,
		RK3228_GMAC_CLK_SEL_2_5M  = 2 << 8,

		RK3228_GMAC_RMII_CLK_MASK   = BIT(7),
		RK3228_GMAC_RMII_CLK_2_5M   = 0,
		RK3228_GMAC_RMII_CLK_25M    = BIT(7),

		RK3228_GMAC_RMII_SPEED_MASK = BIT(2),
		RK3228_GMAC_RMII_SPEED_10   = 0,
		RK3228_GMAC_RMII_SPEED_100  = BIT(2),
	};

	switch (priv->phydev->speed) {
	case 10:
		clk = (pdata->phy_interface == PHY_INTERFACE_MODE_RMII) ?
		       (RK3228_GMAC_RMII_CLK_2_5M | RK3228_GMAC_RMII_SPEED_10) :
		       RK3228_GMAC_CLK_SEL_2_5M;
		break;
	case 100:
		clk = (pdata->phy_interface == PHY_INTERFACE_MODE_RMII) ?
		       (RK3228_GMAC_RMII_CLK_25M | RK3228_GMAC_RMII_SPEED_100) :
		       RK3228_GMAC_CLK_SEL_25M;
		break;
	case 1000:
		clk = RK3228_GMAC_CLK_SEL_125M;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con[1],
		     RK3228_GMAC_CLK_SEL_MASK |
		     RK3228_GMAC_RMII_CLK_MASK |
		     RK3228_GMAC_RMII_SPEED_MASK,
		     clk);

	return 0;
}

static int rk3288_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rk3288_grf *grf;
	int clk;

	switch (priv->phydev->speed) {
	case 10:
		clk = RK3288_GMAC_CLK_SEL_2_5M;
		break;
	case 100:
		clk = RK3288_GMAC_CLK_SEL_25M;
		break;
	case 1000:
		clk = RK3288_GMAC_CLK_SEL_125M;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->soc_con1, RK3288_GMAC_CLK_SEL_MASK, clk);

	return 0;
}

static int rk3308_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rk3308_grf *grf;
	struct clk clk_speed;
	int speed, ret;
	enum {
		RK3308_GMAC_SPEED_SHIFT = 0x0,
		RK3308_GMAC_SPEED_MASK  = BIT(0),
		RK3308_GMAC_SPEED_10M   = 0,
		RK3308_GMAC_SPEED_100M  = BIT(0),
	};

	ret = clk_get_by_name(priv->phydev->dev, "clk_mac_speed",
			      &clk_speed);
	if (ret)
		return ret;

	switch (priv->phydev->speed) {
	case 10:
		speed = RK3308_GMAC_SPEED_10M;
		ret = clk_set_rate(&clk_speed, 2500000);
		if (ret)
			return ret;
		break;
	case 100:
		speed = RK3308_GMAC_SPEED_100M;
		ret = clk_set_rate(&clk_speed, 25000000);
		if (ret)
			return ret;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con0, RK3308_GMAC_SPEED_MASK, speed);

	return 0;
}

static int rk3328_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rk3328_grf_regs *grf;
	int clk;
	enum {
		RK3328_GMAC_CLK_SEL_SHIFT = 11,
		RK3328_GMAC_CLK_SEL_MASK  = GENMASK(12, 11),
		RK3328_GMAC_CLK_SEL_125M  = 0 << 11,
		RK3328_GMAC_CLK_SEL_25M   = 3 << 11,
		RK3328_GMAC_CLK_SEL_2_5M  = 2 << 11,

		RK3328_GMAC_RMII_CLK_MASK   = BIT(7),
		RK3328_GMAC_RMII_CLK_2_5M   = 0,
		RK3328_GMAC_RMII_CLK_25M    = BIT(7),

		RK3328_GMAC_RMII_SPEED_MASK = BIT(2),
		RK3328_GMAC_RMII_SPEED_10   = 0,
		RK3328_GMAC_RMII_SPEED_100  = BIT(2),
	};

	switch (priv->phydev->speed) {
	case 10:
		clk = (pdata->phy_interface == PHY_INTERFACE_MODE_RMII) ?
		       (RK3328_GMAC_RMII_CLK_2_5M | RK3328_GMAC_RMII_SPEED_10) :
		       RK3328_GMAC_CLK_SEL_2_5M;
		break;
	case 100:
		clk = (pdata->phy_interface == PHY_INTERFACE_MODE_RMII) ?
		       (RK3328_GMAC_RMII_CLK_25M | RK3328_GMAC_RMII_SPEED_100) :
		       RK3328_GMAC_CLK_SEL_25M;
		break;
	case 1000:
		clk = RK3328_GMAC_CLK_SEL_125M;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(pdata->integrated_phy ? &grf->mac_con[2] : &grf->mac_con[1],
		     RK3328_GMAC_CLK_SEL_MASK |
		     RK3328_GMAC_RMII_CLK_MASK |
		     RK3328_GMAC_RMII_SPEED_MASK,
		     clk);

	return 0;
}

static int rk3368_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rk3368_grf *grf;
	int clk;
	enum {
		RK3368_GMAC_CLK_SEL_2_5M = 2 << 4,
		RK3368_GMAC_CLK_SEL_25M = 3 << 4,
		RK3368_GMAC_CLK_SEL_125M = 0 << 4,
		RK3368_GMAC_CLK_SEL_MASK = GENMASK(5, 4),
	};

	switch (priv->phydev->speed) {
	case 10:
		clk = RK3368_GMAC_CLK_SEL_2_5M;
		break;
	case 100:
		clk = RK3368_GMAC_CLK_SEL_25M;
		break;
	case 1000:
		clk = RK3368_GMAC_CLK_SEL_125M;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->soc_con15, RK3368_GMAC_CLK_SEL_MASK, clk);

	return 0;
}

static int rk3399_gmac_fix_mac_speed(struct gmac_rockchip_platdata *pdata,
				     struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rk3399_grf_regs *grf;
	int clk;

	switch (priv->phydev->speed) {
	case 10:
		clk = RK3399_GMAC_CLK_SEL_2_5M;
		break;
	case 100:
		clk = RK3399_GMAC_CLK_SEL_25M;
		break;
	case 1000:
		clk = RK3399_GMAC_CLK_SEL_125M;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->soc_con5, RK3399_GMAC_CLK_SEL_MASK, clk);

	return 0;
}

static int rv1108_set_rmii_speed(struct gmac_rockchip_platdata *pdata,
				 struct rockchip_eth_dev *dev)
{
	struct dw_eth_dev *priv = &dev->dw;
	struct rv1108_grf *grf;
	int clk, speed;
	enum {
		RV1108_GMAC_SPEED_MASK		= BIT(2),
		RV1108_GMAC_SPEED_10M		= 0 << 2,
		RV1108_GMAC_SPEED_100M		= 1 << 2,
		RV1108_GMAC_CLK_SEL_MASK	= BIT(7),
		RV1108_GMAC_CLK_SEL_2_5M	= 0 << 7,
		RV1108_GMAC_CLK_SEL_25M		= 1 << 7,
	};

	switch (priv->phydev->speed) {
	case 10:
		clk = RV1108_GMAC_CLK_SEL_2_5M;
		speed = RV1108_GMAC_SPEED_10M;
		break;
	case 100:
		clk = RV1108_GMAC_CLK_SEL_25M;
		speed = RV1108_GMAC_SPEED_100M;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phydev->speed);
		return -EINVAL;
	}

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->gmac_con0,
		     RV1108_GMAC_CLK_SEL_MASK | RV1108_GMAC_SPEED_MASK,
		     clk | speed);

	return 0;
}
#else
static int rk3528_set_rgmii_speed(struct gmac_rockchip_platdata *pdata,
				  struct rockchip_eth_dev *dev)
{
	struct eqos_priv *priv = &dev->eqos;
	struct rk3528_grf *grf;
	unsigned int div;

	enum {
		RK3528_GMAC0_CLK_RMII_DIV_SHIFT = 3,
		RK3528_GMAC0_CLK_RMII_DIV_MASK = GENMASK(4, 3),
		RK3528_GMAC0_CLK_RMII_DIV2 = BIT(3),
		RK3528_GMAC0_CLK_RMII_DIV20 = 0,
	};

	enum {
		RK3528_GMAC1_CLK_RGMII_DIV_SHIFT = 10,
		RK3528_GMAC1_CLK_RGMII_DIV_MASK = GENMASK(11, 10),
		RK3528_GMAC1_CLK_RGMII_DIV1 = 0,
		RK3528_GMAC1_CLK_RGMII_DIV5 = GENMASK(11, 10),
		RK3528_GMAC1_CLK_RGMII_DIV50 = BIT(11),
		RK3528_GMAC1_CLK_RMII_DIV2 = BIT(11),
		RK3528_GMAC1_CLK_RMII_DIV20 = 0,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	switch (priv->phy->speed) {
	case 10:
		if (pdata->phy_interface == PHY_INTERFACE_MODE_RMII)
			div = pdata->bus_id ? RK3528_GMAC1_CLK_RMII_DIV20 :
					      RK3528_GMAC0_CLK_RMII_DIV20;
		else
			div = RK3528_GMAC1_CLK_RGMII_DIV50;
		break;
	case 100:
		if (pdata->phy_interface == PHY_INTERFACE_MODE_RMII)
			div = pdata->bus_id ? RK3528_GMAC1_CLK_RMII_DIV2 :
					      RK3528_GMAC0_CLK_RMII_DIV2;
		else
			div = RK3528_GMAC1_CLK_RGMII_DIV5;
		break;
	case 1000:
		if (pdata->phy_interface != PHY_INTERFACE_MODE_RMII)
			div = RK3528_GMAC1_CLK_RGMII_DIV1;
		else
			return -EINVAL;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phy->speed);
		return -EINVAL;
	}

	if (pdata->bus_id)
		rk_clrsetreg(&grf->gmac1_con0, RK3528_GMAC1_CLK_RGMII_DIV_MASK, div);
	else
		rk_clrsetreg(&grf->gmac0_con, RK3528_GMAC0_CLK_RMII_DIV_MASK, div);

	return 0;
}

static int rk3562_set_gmac_speed(struct gmac_rockchip_platdata *pdata,
				 struct rockchip_eth_dev *dev)
{
	struct eqos_priv *priv = &dev->eqos;
	struct rk3562_grf *grf;
	unsigned int div;

	enum {
		RK3562_GMAC0_CLK_RGMII_DIV_SHIFT = 7,
		RK3562_GMAC0_CLK_RGMII_DIV_MASK = GENMASK(8, 7),
		RK3562_GMAC0_CLK_RGMII_DIV1 = 0,
		RK3562_GMAC0_CLK_RGMII_DIV5 = GENMASK(8, 7),
		RK3562_GMAC0_CLK_RGMII_DIV50 = BIT(8),
		RK3562_GMAC0_CLK_RMII_DIV2 = BIT(7),
		RK3562_GMAC0_CLK_RMII_DIV20 = 0,
	};

	enum {
		RK3562_GMAC1_SPEED_SHIFT = 0x0,
		RK3562_GMAC1_SPEED_MASK  = BIT(0),
		RK3562_GMAC1_SPEED_10M   = 0,
		RK3562_GMAC1_SPEED_100M  = BIT(0),
	};

	enum {
		RK3562_GMAC1_CLK_RMII_DIV_SHIFT = 13,
		RK3562_GMAC1_CLK_RMII_DIV_MASK = BIT(13),
		RK3562_GMAC1_CLK_RMII_DIV2 = BIT(13),
		RK3562_GMAC1_CLK_RMII_DIV20 = 0,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	switch (priv->phy->speed) {
	case 10:
		if (pdata->phy_interface == PHY_INTERFACE_MODE_RMII) {
			if (pdata->bus_id > 0) {
				div = RK3562_GMAC1_CLK_RMII_DIV20;
				rk_clrsetreg(&grf->soc_con[0],
					     RK3562_GMAC1_SPEED_MASK,
					     RK3562_GMAC1_SPEED_10M);
			} else {
				div = RK3562_GMAC0_CLK_RMII_DIV20;
			}
		} else {
			div = RK3562_GMAC0_CLK_RGMII_DIV50;
		}
		break;
	case 100:
		if (pdata->phy_interface == PHY_INTERFACE_MODE_RMII) {
			if (pdata->bus_id > 0) {
				div = RK3562_GMAC1_CLK_RMII_DIV2;
				rk_clrsetreg(&grf->soc_con[0],
					     RK3562_GMAC1_SPEED_MASK,
					     RK3562_GMAC1_SPEED_100M);
			} else {
				div = RK3562_GMAC0_CLK_RMII_DIV2;
			}
		} else {
			div = RK3562_GMAC0_CLK_RGMII_DIV5;
		}
		break;
	case 1000:
		if (pdata->phy_interface != PHY_INTERFACE_MODE_RMII)
			div = RK3562_GMAC0_CLK_RGMII_DIV1;
		else
			return -EINVAL;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phy->speed);
		return -EINVAL;
	}

	if (pdata->bus_id)
		rk_clrsetreg(&grf->soc_con[1], RK3562_GMAC1_CLK_RMII_DIV_MASK, div);
	else
		rk_clrsetreg(&grf->soc_con[0], RK3562_GMAC0_CLK_RGMII_DIV_MASK, div);

	return 0;
}

static int rk3588_set_rgmii_speed(struct gmac_rockchip_platdata *pdata,
				  struct rockchip_eth_dev *dev)
{
	struct eqos_priv *priv = &dev->eqos;
	struct rk3588_php_grf *php_grf;
	unsigned int div, div_mask;

	enum {
		RK3588_GMAC_CLK_RGMII_DIV_SHIFT = 2,
		RK3588_GMAC_CLK_RGMII_DIV_MASK = GENMASK(3, 2),
		RK3588_GMAC_CLK_RGMII_DIV1 = 0,
		RK3588_GMAC_CLK_RGMII_DIV5 = GENMASK(3, 2),
		RK3588_GMAC_CLK_RGMII_DIV50 = BIT(3),
		RK3588_GMAC_CLK_RMII_DIV2 = BIT(2),
		RK3588_GMAC_CLK_RMII_DIV20 = 0,
		RK3588_GMAC1_ID_SHIFT = 5,
	};

	php_grf = syscon_get_first_range(ROCKCHIP_SYSCON_PHP_GRF);

	switch (priv->phy->speed) {
	case 10:
		if (pdata->phy_interface == PHY_INTERFACE_MODE_RMII)
			div = RK3588_GMAC_CLK_RMII_DIV20;
		else
			div = RK3588_GMAC_CLK_RGMII_DIV50;
		break;
	case 100:
		if (pdata->phy_interface == PHY_INTERFACE_MODE_RMII)
			div = RK3588_GMAC_CLK_RMII_DIV2;
		else
			div = RK3588_GMAC_CLK_RGMII_DIV5;
		break;
	case 1000:
		if (pdata->phy_interface != PHY_INTERFACE_MODE_RMII)
			div = RK3588_GMAC_CLK_RGMII_DIV1;
		else
			return -EINVAL;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phy->speed);
		return -EINVAL;
	}

	if (pdata->bus_id == 1) {
		div <<= 5;
		div_mask = RK3588_GMAC_CLK_RGMII_DIV_MASK << 5;
	}

	div <<= pdata->bus_id ? RK3588_GMAC1_ID_SHIFT : 0;
	div_mask = pdata->bus_id ? (RK3588_GMAC_CLK_RGMII_DIV_MASK << 5) :
		   RK3588_GMAC_CLK_RGMII_DIV_MASK;

	rk_clrsetreg(&php_grf->clk_con1, div_mask, div);

	return 0;
}

static int rv1106_set_rmii_speed(struct gmac_rockchip_platdata *pdata,
				 struct rockchip_eth_dev *dev)
{
	struct eqos_priv *priv = &dev->eqos;
	struct rv1106_grf *grf;
	unsigned int div;

	enum {
		RV1106_GMAC_CLK_RMII_DIV_SHIFT = 2,
		RV1106_GMAC_CLK_RMII_DIV_MASK = GENMASK(3, 2),
		RV1106_GMAC_CLK_RMII_DIV2 = BIT(2),
		RV1106_GMAC_CLK_RMII_DIV20 = 0,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	switch (priv->phy->speed) {
	case 10:
		div = RV1106_GMAC_CLK_RMII_DIV20;
		break;
	case 100:
		div = RV1106_GMAC_CLK_RMII_DIV2;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phy->speed);
		return -EINVAL;
	}

	rk_clrsetreg(&grf->gmac_clk_con, RV1106_GMAC_CLK_RMII_DIV_MASK, div);

	return 0;
}

static int rv1126_set_rgmii_speed(struct gmac_rockchip_platdata *pdata,
				  struct rockchip_eth_dev *dev)
{
	struct eqos_priv *priv = &dev->eqos;
	struct clk clk_speed;
	int ret;

	ret = clk_get_by_name(priv->phy->dev, "clk_mac_speed",
			      &clk_speed);
	if (ret) {
		printf("%s can't get clk_mac_speed clock (ret=%d):\n",
		       __func__, ret);
		return ret;
	}

	switch ( priv->phy->speed) {
	case 10:
		ret = clk_set_rate(&clk_speed, 2500000);
		if (ret)
			return ret;
		break;
	case 100:
		ret = clk_set_rate(&clk_speed, 25000000);
		if (ret)
			return ret;
		break;
	case 1000:
		ret = clk_set_rate(&clk_speed, 125000000);
		if (ret)
			return ret;
		break;
	default:
		debug("Unknown phy speed: %d\n", priv->phy->speed);
		return -EINVAL;
	}

	return 0;
}
#endif

#ifndef CONFIG_DWC_ETH_QOS
static void px30_gmac_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct px30_grf *grf;
	enum {
		px30_GMAC_PHY_INTF_SEL_SHIFT = 4,
		px30_GMAC_PHY_INTF_SEL_MASK  = GENMASK(4, 6),
		px30_GMAC_PHY_INTF_SEL_RMII  = BIT(6),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	rk_clrsetreg(&grf->mac_con1,
		     px30_GMAC_PHY_INTF_SEL_MASK,
		     px30_GMAC_PHY_INTF_SEL_RMII);
}

static void rk1808_gmac_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk1808_grf *grf;
	enum {
		RK1808_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RK1808_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RK1808_GMAC_PHY_INTF_SEL_RGMII = BIT(4),

		RK1808_RXCLK_DLY_ENA_GMAC_MASK = BIT(1),
		RK1808_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK1808_RXCLK_DLY_ENA_GMAC_ENABLE = BIT(1),

		RK1808_TXCLK_DLY_ENA_GMAC_MASK = BIT(0),
		RK1808_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK1808_TXCLK_DLY_ENA_GMAC_ENABLE = BIT(0),
	};
	enum {
		RK1808_CLK_RX_DL_CFG_GMAC_SHIFT = 0x8,
		RK1808_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(15, 7),

		RK1808_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RK1808_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(7, 0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con1,
		     RK1808_GMAC_PHY_INTF_SEL_MASK |
		     RK1808_RXCLK_DLY_ENA_GMAC_MASK |
		     RK1808_TXCLK_DLY_ENA_GMAC_MASK,
		     RK1808_GMAC_PHY_INTF_SEL_RGMII |
		     RK1808_RXCLK_DLY_ENA_GMAC_ENABLE |
		     RK1808_TXCLK_DLY_ENA_GMAC_ENABLE);

	rk_clrsetreg(&grf->mac_con0,
		     RK1808_CLK_RX_DL_CFG_GMAC_MASK |
		     RK1808_CLK_TX_DL_CFG_GMAC_MASK,
		     (pdata->rx_delay << RK1808_CLK_RX_DL_CFG_GMAC_SHIFT) |
		     (pdata->tx_delay << RK1808_CLK_TX_DL_CFG_GMAC_SHIFT));
}

static void rk3228_gmac_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk322x_grf *grf;
	enum {
		RK3228_RMII_MODE_SHIFT = 10,
		RK3228_RMII_MODE_MASK  = BIT(10),

		RK3228_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RK3228_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RK3228_GMAC_PHY_INTF_SEL_RGMII = BIT(4),

		RK3228_RXCLK_DLY_ENA_GMAC_MASK = BIT(1),
		RK3228_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3228_RXCLK_DLY_ENA_GMAC_ENABLE = BIT(1),

		RK3228_TXCLK_DLY_ENA_GMAC_MASK = BIT(0),
		RK3228_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3228_TXCLK_DLY_ENA_GMAC_ENABLE = BIT(0),
	};
	enum {
		RK3228_CLK_RX_DL_CFG_GMAC_SHIFT = 0x7,
		RK3228_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(13, 7),

		RK3228_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RK3228_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(6, 0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con[1],
		     RK3228_RMII_MODE_MASK |
		     RK3228_GMAC_PHY_INTF_SEL_MASK |
		     RK3228_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3228_TXCLK_DLY_ENA_GMAC_MASK,
		     RK3228_GMAC_PHY_INTF_SEL_RGMII |
		     RK3228_RXCLK_DLY_ENA_GMAC_ENABLE |
		     RK3228_TXCLK_DLY_ENA_GMAC_ENABLE);

	rk_clrsetreg(&grf->mac_con[0],
		     RK3228_CLK_RX_DL_CFG_GMAC_MASK |
		     RK3228_CLK_TX_DL_CFG_GMAC_MASK,
		     pdata->rx_delay << RK3228_CLK_RX_DL_CFG_GMAC_SHIFT |
		     pdata->tx_delay << RK3228_CLK_TX_DL_CFG_GMAC_SHIFT);
}

static void rk3228_gmac_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk322x_grf *grf;
	enum {
		RK3228_GRF_CON_RMII_MODE_MASK = BIT(11),
		RK3228_GRF_CON_RMII_MODE_SEL = BIT(11),
		RK3228_RMII_MODE_MASK = BIT(10),
		RK3228_RMII_MODE_SEL = BIT(10),
		RK3228_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RK3228_GMAC_PHY_INTF_SEL_RMII = BIT(6),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con[1],
		     RK3228_GRF_CON_RMII_MODE_MASK |
		     RK3228_RMII_MODE_MASK |
		     RK3228_GMAC_PHY_INTF_SEL_MASK,
		     RK3228_GRF_CON_RMII_MODE_SEL |
		     RK3228_RMII_MODE_SEL |
		     RK3228_GMAC_PHY_INTF_SEL_RMII);
}

static void rk3288_gmac_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3288_grf *grf;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->soc_con1,
		     RK3288_RMII_MODE_MASK | RK3288_GMAC_PHY_INTF_SEL_MASK,
		     RK3288_GMAC_PHY_INTF_SEL_RGMII);

	rk_clrsetreg(&grf->soc_con3,
		     RK3288_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3288_TXCLK_DLY_ENA_GMAC_MASK |
		     RK3288_CLK_RX_DL_CFG_GMAC_MASK |
		     RK3288_CLK_TX_DL_CFG_GMAC_MASK,
		     RK3288_RXCLK_DLY_ENA_GMAC_ENABLE |
		     RK3288_TXCLK_DLY_ENA_GMAC_ENABLE |
		     pdata->rx_delay << RK3288_CLK_RX_DL_CFG_GMAC_SHIFT |
		     pdata->tx_delay << RK3288_CLK_TX_DL_CFG_GMAC_SHIFT);
}

static void rk3308_gmac_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3308_grf *grf;
	enum {
		RK3308_GMAC_PHY_INTF_SEL_SHIFT = 2,
		RK3308_GMAC_PHY_INTF_SEL_MASK  = GENMASK(4, 2),
		RK3308_GMAC_PHY_INTF_SEL_RMII  = BIT(4),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	rk_clrsetreg(&grf->mac_con0,
		     RK3308_GMAC_PHY_INTF_SEL_MASK,
		     RK3308_GMAC_PHY_INTF_SEL_RMII);
}

static void rk3328_gmac_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3328_grf_regs *grf;
	enum {
		RK3328_RMII_MODE_SHIFT = 9,
		RK3328_RMII_MODE_MASK  = BIT(9),

		RK3328_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RK3328_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RK3328_GMAC_PHY_INTF_SEL_RGMII = BIT(4),

		RK3328_RXCLK_DLY_ENA_GMAC_MASK = BIT(1),
		RK3328_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3328_RXCLK_DLY_ENA_GMAC_ENABLE = BIT(1),

		RK3328_TXCLK_DLY_ENA_GMAC_MASK = BIT(0),
		RK3328_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3328_TXCLK_DLY_ENA_GMAC_ENABLE = BIT(0),
	};
	enum {
		RK3328_CLK_RX_DL_CFG_GMAC_SHIFT = 0x7,
		RK3328_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(13, 7),

		RK3328_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RK3328_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(6, 0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->mac_con[1],
		     RK3328_RMII_MODE_MASK |
		     RK3328_GMAC_PHY_INTF_SEL_MASK |
		     RK3328_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3328_TXCLK_DLY_ENA_GMAC_MASK,
		     RK3328_GMAC_PHY_INTF_SEL_RGMII |
		     RK3328_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3328_TXCLK_DLY_ENA_GMAC_ENABLE);

	rk_clrsetreg(&grf->mac_con[0],
		     RK3328_CLK_RX_DL_CFG_GMAC_MASK |
		     RK3328_CLK_TX_DL_CFG_GMAC_MASK,
		     pdata->rx_delay << RK3328_CLK_RX_DL_CFG_GMAC_SHIFT |
		     pdata->tx_delay << RK3328_CLK_TX_DL_CFG_GMAC_SHIFT);
}

static void rk3328_gmac_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3328_grf_regs *grf;
	enum {
		RK3328_RMII_MODE_MASK  = BIT(9),
		RK3328_RMII_MODE = BIT(9),

		RK3328_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RK3328_GMAC_PHY_INTF_SEL_RMII = BIT(6),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(pdata->integrated_phy ? &grf->mac_con[2] : &grf->mac_con[1],
		     RK3328_RMII_MODE_MASK |
		     RK3328_GMAC_PHY_INTF_SEL_MASK,
		     RK3328_GMAC_PHY_INTF_SEL_RMII |
		     RK3328_RMII_MODE);
}

static void rk3368_gmac_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3368_grf *grf;
	enum {
		RK3368_GMAC_PHY_INTF_SEL_RGMII = 1 << 9,
		RK3368_GMAC_PHY_INTF_SEL_MASK = GENMASK(11, 9),
		RK3368_RMII_MODE_MASK  = BIT(6),
		RK3368_RMII_MODE       = BIT(6),
	};
	enum {
		RK3368_RXCLK_DLY_ENA_GMAC_MASK = BIT(15),
		RK3368_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3368_RXCLK_DLY_ENA_GMAC_ENABLE = BIT(15),
		RK3368_TXCLK_DLY_ENA_GMAC_MASK = BIT(7),
		RK3368_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3368_TXCLK_DLY_ENA_GMAC_ENABLE = BIT(7),
		RK3368_CLK_RX_DL_CFG_GMAC_SHIFT = 8,
		RK3368_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(14, 8),
		RK3368_CLK_TX_DL_CFG_GMAC_SHIFT = 0,
		RK3368_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(6, 0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->soc_con15,
		     RK3368_RMII_MODE_MASK | RK3368_GMAC_PHY_INTF_SEL_MASK,
		     RK3368_GMAC_PHY_INTF_SEL_RGMII);

	rk_clrsetreg(&grf->soc_con16,
		     RK3368_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3368_TXCLK_DLY_ENA_GMAC_MASK |
		     RK3368_CLK_RX_DL_CFG_GMAC_MASK |
		     RK3368_CLK_TX_DL_CFG_GMAC_MASK,
		     RK3368_RXCLK_DLY_ENA_GMAC_ENABLE |
		     RK3368_TXCLK_DLY_ENA_GMAC_ENABLE |
		     (pdata->rx_delay << RK3368_CLK_RX_DL_CFG_GMAC_SHIFT) |
		     (pdata->tx_delay << RK3368_CLK_TX_DL_CFG_GMAC_SHIFT));
}

static void rk3399_gmac_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3399_grf_regs *grf;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	rk_clrsetreg(&grf->soc_con5,
		     RK3399_GMAC_PHY_INTF_SEL_MASK,
		     RK3399_GMAC_PHY_INTF_SEL_RGMII);

	rk_clrsetreg(&grf->soc_con6,
		     RK3399_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3399_TXCLK_DLY_ENA_GMAC_MASK |
		     RK3399_CLK_RX_DL_CFG_GMAC_MASK |
		     RK3399_CLK_TX_DL_CFG_GMAC_MASK,
		     RK3399_RXCLK_DLY_ENA_GMAC_ENABLE |
		     RK3399_TXCLK_DLY_ENA_GMAC_ENABLE |
		     (pdata->rx_delay << RK3399_CLK_RX_DL_CFG_GMAC_SHIFT) |
		     (pdata->tx_delay << RK3399_CLK_TX_DL_CFG_GMAC_SHIFT));
}

static void rv1108_gmac_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rv1108_grf *grf;

	enum {
		RV1108_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RV1108_GMAC_PHY_INTF_SEL_RMII  = 4 << 4,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->gmac_con0,
		     RV1108_GMAC_PHY_INTF_SEL_MASK,
		     RV1108_GMAC_PHY_INTF_SEL_RMII);
}

static void rk3228_gmac_integrated_phy_powerup(struct gmac_rockchip_platdata *pdata)
{
	struct rk322x_grf *grf;
	enum {
		RK3228_GRF_CON_MUX_GMAC_INTEGRATED_PHY_MASK = BIT(15),
		RK3228_GRF_CON_MUX_GMAC_INTEGRATED_PHY = BIT(15),
	};
	enum {
		RK3228_MACPHY_CFG_CLK_50M_MASK = BIT(14),
		RK3228_MACPHY_CFG_CLK_50M = BIT(14),

		RK3228_MACPHY_RMII_MODE_MASK = GENMASK(7, 6),
		RK3228_MACPHY_RMII_MODE = BIT(6),

		RK3228_MACPHY_ENABLE_MASK = BIT(0),
		RK3228_MACPHY_DISENABLE = 0,
		RK3228_MACPHY_ENABLE = BIT(0),
	};
	enum {
		RK3228_RK_GRF_CON2_MACPHY_ID_MASK = GENMASK(6, 0),
		RK3228_RK_GRF_CON2_MACPHY_ID = 0x1234,
	};
	enum {
		RK3228_RK_GRF_CON3_MACPHY_ID_MASK = GENMASK(5, 0),
		RK3228_RK_GRF_CON3_MACPHY_ID = 0x35,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->con_iomux,
		     RK3228_GRF_CON_MUX_GMAC_INTEGRATED_PHY_MASK,
		     RK3228_GRF_CON_MUX_GMAC_INTEGRATED_PHY);

	rk_clrsetreg(&grf->macphy_con[2],
		     RK3228_RK_GRF_CON2_MACPHY_ID_MASK,
		     RK3228_RK_GRF_CON2_MACPHY_ID);

	rk_clrsetreg(&grf->macphy_con[3],
		     RK3228_RK_GRF_CON3_MACPHY_ID_MASK,
		     RK3228_RK_GRF_CON3_MACPHY_ID);

	/* disabled before trying to reset it &*/
	rk_clrsetreg(&grf->macphy_con[0],
		     RK3228_MACPHY_CFG_CLK_50M_MASK |
		     RK3228_MACPHY_RMII_MODE_MASK |
		     RK3228_MACPHY_ENABLE_MASK,
		     RK3228_MACPHY_CFG_CLK_50M |
		     RK3228_MACPHY_RMII_MODE |
		     RK3228_MACPHY_DISENABLE);

	reset_assert(&pdata->phy_reset);
	udelay(10);
	reset_deassert(&pdata->phy_reset);
	udelay(10);

	rk_clrsetreg(&grf->macphy_con[0],
		     RK3228_MACPHY_ENABLE_MASK,
		     RK3228_MACPHY_ENABLE);
	udelay(30 * 1000);
}

static void rk3328_gmac_integrated_phy_powerup(struct gmac_rockchip_platdata *pdata)
{
	struct rk3328_grf_regs *grf;
	enum {
		RK3328_GRF_CON_RMII_MODE_MASK = BIT(9),
		RK3328_GRF_CON_RMII_MODE = BIT(9),
	};
	enum {
		RK3328_MACPHY_CFG_CLK_50M_MASK = BIT(14),
		RK3328_MACPHY_CFG_CLK_50M = BIT(14),

		RK3328_MACPHY_RMII_MODE_MASK = GENMASK(7, 6),
		RK3328_MACPHY_RMII_MODE = BIT(6),

		RK3328_MACPHY_ENABLE_MASK = BIT(0),
		RK3328_MACPHY_DISENABLE = 0,
		RK3328_MACPHY_ENABLE = BIT(0),
	};
	enum {
		RK3328_RK_GRF_CON2_MACPHY_ID_MASK = GENMASK(6, 0),
		RK3328_RK_GRF_CON2_MACPHY_ID = 0x1234,
	};
	enum {
		RK3328_RK_GRF_CON3_MACPHY_ID_MASK = GENMASK(5, 0),
		RK3328_RK_GRF_CON3_MACPHY_ID = 0x35,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->macphy_con[1],
		     RK3328_GRF_CON_RMII_MODE_MASK,
		     RK3328_GRF_CON_RMII_MODE);

	rk_clrsetreg(&grf->macphy_con[2],
		     RK3328_RK_GRF_CON2_MACPHY_ID_MASK,
		     RK3328_RK_GRF_CON2_MACPHY_ID);

	rk_clrsetreg(&grf->macphy_con[3],
		     RK3328_RK_GRF_CON3_MACPHY_ID_MASK,
		     RK3328_RK_GRF_CON3_MACPHY_ID);

	/* disabled before trying to reset it &*/
	rk_clrsetreg(&grf->macphy_con[0],
		     RK3328_MACPHY_CFG_CLK_50M_MASK |
		     RK3328_MACPHY_RMII_MODE_MASK |
		     RK3328_MACPHY_ENABLE_MASK,
		     RK3328_MACPHY_CFG_CLK_50M |
		     RK3328_MACPHY_RMII_MODE |
		     RK3328_MACPHY_DISENABLE);

	reset_assert(&pdata->phy_reset);
	udelay(10);
	reset_deassert(&pdata->phy_reset);
	udelay(10);

	rk_clrsetreg(&grf->macphy_con[0],
		     RK3328_MACPHY_ENABLE_MASK,
		     RK3328_MACPHY_ENABLE);
	udelay(30 * 1000);
}

#else
static void rk3528_gmac_integrated_phy_powerup(struct gmac_rockchip_platdata *pdata)
{
	struct rk3528_grf *grf;
	unsigned char bgs[1] = {0};

	enum {
		RK3528_MACPHY_ENABLE_MASK = BIT(1),
		RK3528_MACPHY_DISENABLE = BIT(1),
		RK3528_MACPHY_ENABLE = 0,
		RK3528_MACPHY_XMII_SEL_MASK = GENMASK(6, 5),
		RK3528_MACPHY_XMII_SEL = BIT(6),
		RK3528_MACPHY_24M_CLK_SEL_MASK = GENMASK(9, 7),
		RK3528_MACPHY_24M_CLK_SEL_24M = (BIT(8) | BIT(9)),
		RK3528_MACPHY_PHY_ID_MASK = GENMASK(14, 10),
		RK3528_MACPHY_PHY_ID = BIT(11),
	};

	enum {
		RK3528_MACPHY_BGS_MASK = GENMASK(3, 0),
	};

#if defined(CONFIG_ROCKCHIP_EFUSE) || defined(CONFIG_ROCKCHIP_OTP)
	struct udevice *dev;
	u32 regs[2] = {0};
	ofnode node;
	int ret = 0;

	/* retrieve the device */
	if (IS_ENABLED(CONFIG_ROCKCHIP_EFUSE))
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_efuse),
						  &dev);
	else
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_otp),
						  &dev);
	if (!ret) {
		node = dev_read_subnode(dev, "macphy-bgs");
		if (ofnode_valid(node)) {
			if (!ofnode_read_u32_array(node, "reg", regs, 2)) {
				/* read the bgs from the efuses */
				ret = misc_read(dev, regs[0], &bgs, 1);
				if (ret) {
					printf("read bgs from efuse/otp failed, ret=%d\n",
					       ret);
					bgs[0] = 0;
				}
			}
		}
	}
#endif

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	reset_assert(&pdata->phy_reset);
	udelay(20);
	rk_clrsetreg(&grf->macphy_con0,
		     RK3528_MACPHY_ENABLE_MASK |
		     RK3528_MACPHY_XMII_SEL_MASK |
		     RK3528_MACPHY_24M_CLK_SEL_MASK |
		     RK3528_MACPHY_PHY_ID_MASK,
		     RK3528_MACPHY_ENABLE |
		     RK3528_MACPHY_XMII_SEL |
		     RK3528_MACPHY_24M_CLK_SEL_24M |
		     RK3528_MACPHY_PHY_ID);

	rk_clrsetreg(&grf->macphy_con1,
		     RK3528_MACPHY_BGS_MASK,
		     bgs[0]);
	udelay(20);
	reset_deassert(&pdata->phy_reset);
	udelay(30 * 1000);
}

static void rk3528_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	unsigned int clk_mode;
	struct rk3528_grf *grf;

	enum {
		RK3528_GMAC0_CLK_RMII_MODE_SHIFT = 0x1,
		RK3528_GMAC0_CLK_RMII_MODE_MASK = BIT(1),
		RK3528_GMAC0_CLK_RMII_MODE = 0x1,
	};

	enum {
		RK3528_GMAC1_CLK_RMII_MODE_SHIFT = 0x8,
		RK3528_GMAC1_CLK_RMII_MODE_MASK = BIT(8),
		RK3528_GMAC1_CLK_RMII_MODE = 0x1,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (pdata->bus_id == 1) {
		clk_mode = RK3528_GMAC1_CLK_RMII_MODE << RK3528_GMAC1_CLK_RMII_MODE_SHIFT;
		rk_clrsetreg(&grf->gmac1_con1, RK3528_GMAC1_CLK_RMII_MODE_MASK, clk_mode);
	} else {
		clk_mode = RK3528_GMAC0_CLK_RMII_MODE << RK3528_GMAC0_CLK_RMII_MODE_SHIFT;
		rk_clrsetreg(&grf->gmac0_con, RK3528_GMAC0_CLK_RMII_MODE_MASK, clk_mode);
	}
}

static void rk3528_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	unsigned int rx_enable;
	unsigned int rx_delay;
	struct rk3528_grf *grf;

	enum {
		RK3528_GMAC1_RGMII_MODE_SHIFT = 0x8,
		RK3528_GMAC1_RGMII_MODE_MASK = BIT(8),
		RK3528_GMAC1_RGMII_MODE = 0x0,

		RK3528_GMAC1_TXCLK_DLY_ENA_MASK = BIT(14),
		RK3528_GMAC1_TXCLK_DLY_ENA_DISABLE = 0,
		RK3528_GMAC1_TXCLK_DLY_ENA_ENABLE = BIT(14),

		RK3528_GMAC1_RXCLK_DLY_ENA_MASK = BIT(15),
		RK3528_GMAC1_RXCLK_DLY_ENA_DISABLE = 0,
		RK3528_GMAC1_RXCLK_DLY_ENA_ENABLE = BIT(15),
	};

	enum {
		RK3528_GMAC1_RX_DL_CFG_SHIFT = 0x8,
		RK3528_GMAC1_RX_DL_CFG_MASK = GENMASK(15, 8),

		RK3528_GMAC1_TX_DL_CFG_SHIFT = 0x0,
		RK3528_GMAC1_TX_DL_CFG_MASK = GENMASK(7, 0),
	};

	if (!pdata->bus_id)
		return;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (pdata->rx_delay < 0) {
		rx_enable = RK3528_GMAC1_RXCLK_DLY_ENA_DISABLE;
		rx_delay = 0;
	} else {
		rx_enable = RK3528_GMAC1_RXCLK_DLY_ENA_ENABLE;
		rx_delay = pdata->rx_delay << RK3528_GMAC1_RX_DL_CFG_SHIFT;
	}

	rk_clrsetreg(&grf->gmac1_con0,
		     RK3528_GMAC1_TXCLK_DLY_ENA_MASK |
		     RK3528_GMAC1_RXCLK_DLY_ENA_MASK |
		     RK3528_GMAC1_RGMII_MODE_MASK,
		     rx_enable | RK3528_GMAC1_TXCLK_DLY_ENA_ENABLE |
		     (RK3528_GMAC1_RGMII_MODE << RK3528_GMAC1_RGMII_MODE_SHIFT));

	rk_clrsetreg(&grf->gmac1_con1,
		     RK3528_GMAC1_RX_DL_CFG_MASK |
		     RK3528_GMAC1_TX_DL_CFG_MASK,
		     (pdata->tx_delay << RK3528_GMAC1_TX_DL_CFG_SHIFT) |
		     rx_delay);
}

static void rk3562_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3562_grf *grf;
	unsigned int mode;

	enum {
		RK3562_GMAC0_RMII_MODE_SHIFT = 0x5,
		RK3562_GMAC0_RMII_MODE_MASK = BIT(5),
		RK3562_GMAC0_RMII_MODE = 0x1,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (!pdata->bus_id) {
		mode = RK3562_GMAC0_RMII_MODE << RK3562_GMAC0_RMII_MODE_SHIFT;
		rk_clrsetreg(&grf->soc_con[0], RK3562_GMAC0_RMII_MODE_MASK, mode);
	}
}

static void rk3562_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3562_grf *grf;
	struct rk3562_ioc *ioc;
	unsigned int rx_enable;
	unsigned int rx_delay;

	enum {
		RK3562_GMAC0_RGMII_MODE_SHIFT = 0x5,
		RK3562_GMAC0_RGMII_MODE_MASK = BIT(5),
		RK3562_GMAC0_RGMII_MODE = 0x0,

		RK3562_GMAC0_TXCLK_DLY_ENA_MASK = BIT(0),
		RK3562_GMAC0_TXCLK_DLY_ENA_DISABLE = 0,
		RK3562_GMAC0_TXCLK_DLY_ENA_ENABLE = BIT(0),

		RK3562_GMAC0_RXCLK_DLY_ENA_MASK = BIT(1),
		RK3562_GMAC0_RXCLK_DLY_ENA_DISABLE = 0,
		RK3562_GMAC0_RXCLK_DLY_ENA_ENABLE = BIT(1),
	};

	enum {
		RK3562_GMAC0_RX_DL_CFG_SHIFT = 0x8,
		RK3562_GMAC0_RX_DL_CFG_MASK = GENMASK(15, 8),

		RK3562_GMAC0_TX_DL_CFG_SHIFT = 0x0,
		RK3562_GMAC0_TX_DL_CFG_MASK = GENMASK(7, 0),
	};

	if (pdata->bus_id)
		return;

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	ioc = syscon_get_first_range(ROCKCHIP_SYSCON_IOC);

	rk_clrsetreg(&grf->soc_con[0], RK3562_GMAC0_RGMII_MODE_MASK,
		     RK3562_GMAC0_RGMII_MODE << RK3562_GMAC0_RGMII_MODE_SHIFT);

	if (pdata->rx_delay < 0) {
		rx_enable = RK3562_GMAC0_RXCLK_DLY_ENA_DISABLE;
		rx_delay = 0;
	} else {
		rx_enable = RK3562_GMAC0_RXCLK_DLY_ENA_ENABLE;
		rx_delay = pdata->rx_delay << RK3562_GMAC0_RX_DL_CFG_SHIFT;
	}

	rk_clrsetreg(&ioc->mac0_io_con1,
		     RK3562_GMAC0_TXCLK_DLY_ENA_MASK |
		     RK3562_GMAC0_RXCLK_DLY_ENA_MASK,
		     rx_enable | RK3562_GMAC0_TXCLK_DLY_ENA_ENABLE);

	rk_clrsetreg(&ioc->mac0_io_con0,
		     RK3562_GMAC0_RX_DL_CFG_MASK |
		     RK3562_GMAC0_TX_DL_CFG_MASK,
		     (pdata->tx_delay << RK3562_GMAC0_TX_DL_CFG_SHIFT) |
		     rx_delay);

	rk_clrsetreg(&ioc->mac1_io_con1,
		     RK3562_GMAC0_TXCLK_DLY_ENA_MASK |
		     RK3562_GMAC0_RXCLK_DLY_ENA_MASK,
		     rx_enable | RK3562_GMAC0_TXCLK_DLY_ENA_ENABLE);

	rk_clrsetreg(&ioc->mac1_io_con0,
		     RK3562_GMAC0_RX_DL_CFG_MASK |
		     RK3562_GMAC0_TX_DL_CFG_MASK,
		     (pdata->tx_delay << RK3562_GMAC0_TX_DL_CFG_SHIFT) |
		     rx_delay);
}

static void rk3568_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3568_grf *grf;
	void *con1;

	enum {
		RK3568_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RK3568_GMAC_PHY_INTF_SEL_MASK = GENMASK(6, 4),
		RK3568_GMAC_PHY_INTF_SEL_RMII = BIT(6),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (pdata->bus_id == 1)
		con1 = &grf->mac1_con1;
	else
		con1 = &grf->mac0_con1;

	rk_clrsetreg(con1,
		     RK3568_GMAC_PHY_INTF_SEL_MASK,
		     RK3568_GMAC_PHY_INTF_SEL_RMII);
}

static void rk3568_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rk3568_grf *grf;
	void *con0, *con1;

	enum {
		RK3568_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RK3568_GMAC_PHY_INTF_SEL_MASK = GENMASK(6, 4),
		RK3568_GMAC_PHY_INTF_SEL_RGMII = BIT(4),

		RK3568_RXCLK_DLY_ENA_GMAC_MASK = BIT(1),
		RK3568_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3568_RXCLK_DLY_ENA_GMAC_ENABLE = BIT(1),

		RK3568_TXCLK_DLY_ENA_GMAC_MASK = BIT(0),
		RK3568_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3568_TXCLK_DLY_ENA_GMAC_ENABLE = BIT(0),
	};

	enum {
		RK3568_CLK_RX_DL_CFG_GMAC_SHIFT = 0x8,
		RK3568_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(15, 8),

		RK3568_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RK3568_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(7, 0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (pdata->bus_id == 1) {
		con0 = &grf->mac1_con0;
		con1 = &grf->mac1_con1;
	} else {
		con0 = &grf->mac0_con0;
		con1 = &grf->mac0_con1;
	}

	rk_clrsetreg(con0,
		     RK3568_CLK_RX_DL_CFG_GMAC_MASK |
		     RK3568_CLK_TX_DL_CFG_GMAC_MASK,
		     (pdata->rx_delay << RK3568_CLK_RX_DL_CFG_GMAC_SHIFT) |
		     (pdata->tx_delay << RK3568_CLK_TX_DL_CFG_GMAC_SHIFT));

	rk_clrsetreg(con1,
		     RK3568_TXCLK_DLY_ENA_GMAC_MASK |
		     RK3568_RXCLK_DLY_ENA_GMAC_MASK |
		     RK3568_GMAC_PHY_INTF_SEL_MASK,
		     RK3568_TXCLK_DLY_ENA_GMAC_ENABLE |
		     RK3568_RXCLK_DLY_ENA_GMAC_ENABLE |
		     RK3568_GMAC_PHY_INTF_SEL_RGMII);
}

static void rk3588_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	unsigned int intf_sel, intf_sel_mask;
	unsigned int clk_mode, clk_mode_mask;
	struct rk3588_php_grf *php_grf;

	enum {
		RK3588_GMAC_PHY_INTF_SEL_SHIFT = 3,
		RK3588_GMAC_PHY_INTF_SEL_MASK = GENMASK(5, 3),
		RK3588_GMAC_PHY_INTF_SEL_RMII = BIT(5),
	};

	enum {
		RK3588_GMAC_CLK_RMII_MODE_SHIFT = 0x0,
		RK3588_GMAC_CLK_RMII_MODE_MASK = BIT(0),
		RK3588_GMAC_CLK_RMII_MODE = 0x1,
	};

	php_grf = syscon_get_first_range(ROCKCHIP_SYSCON_PHP_GRF);

	if (pdata->bus_id == 1) {
		intf_sel = RK3588_GMAC_PHY_INTF_SEL_RMII << 6;
		intf_sel_mask = RK3588_GMAC_PHY_INTF_SEL_MASK << 6;
		clk_mode = RK3588_GMAC_CLK_RMII_MODE << 5;
		clk_mode_mask = RK3588_GMAC_CLK_RMII_MODE_MASK << 5;
	} else {
		intf_sel = RK3588_GMAC_PHY_INTF_SEL_RMII;
		intf_sel_mask = RK3588_GMAC_PHY_INTF_SEL_MASK;
		clk_mode = RK3588_GMAC_CLK_RMII_MODE;
		clk_mode_mask = RK3588_GMAC_CLK_RMII_MODE_MASK;
	}

	rk_clrsetreg(&php_grf->gmac_con0, intf_sel_mask, intf_sel);
	rk_clrsetreg(&php_grf->clk_con1, clk_mode_mask, clk_mode);
}

static void rk3588_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	unsigned int rx_enable, rx_enable_mask, tx_enable, tx_enable_mask;
	unsigned int intf_sel, intf_sel_mask;
	unsigned int clk_mode, clk_mode_mask;
	unsigned int rx_delay;
	struct rk3588_php_grf *php_grf;
	struct rk3588_sys_grf *grf;
	void *offset_con;

	enum {
		RK3588_GMAC_PHY_INTF_SEL_SHIFT = 3,
		RK3588_GMAC_PHY_INTF_SEL_MASK = GENMASK(5, 3),
		RK3588_GMAC_PHY_INTF_SEL_RGMII = BIT(3),

		RK3588_RXCLK_DLY_ENA_GMAC_MASK = BIT(3),
		RK3588_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3588_RXCLK_DLY_ENA_GMAC_ENABLE = BIT(3),

		RK3588_TXCLK_DLY_ENA_GMAC_MASK = BIT(2),
		RK3588_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
		RK3588_TXCLK_DLY_ENA_GMAC_ENABLE = BIT(2),
	};

	enum {
		RK3588_CLK_RX_DL_CFG_GMAC_SHIFT = 0x8,
		RK3588_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(15, 8),

		RK3588_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RK3588_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(7, 0),
	};

	enum {
		RK3588_GMAC_CLK_RGMII_MODE_SHIFT = 0x0,
		RK3588_GMAC_CLK_RGMII_MODE_MASK = BIT(0),
		RK3588_GMAC_CLK_RGMII_MODE = 0x0,
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	php_grf = syscon_get_first_range(ROCKCHIP_SYSCON_PHP_GRF);

	if (pdata->rx_delay < 0) {
		rx_enable = RK3588_RXCLK_DLY_ENA_GMAC_DISABLE;
		rx_delay = 0;
	} else {
		rx_enable = RK3588_RXCLK_DLY_ENA_GMAC_ENABLE;
		rx_delay = pdata->rx_delay << RK3588_CLK_RX_DL_CFG_GMAC_SHIFT;
	}

	if (pdata->bus_id == 1) {
		offset_con = &grf->soc_con9;
		rx_enable = rx_delay << 2;
		rx_enable_mask = RK3588_RXCLK_DLY_ENA_GMAC_MASK << 2;
		tx_enable = RK3588_TXCLK_DLY_ENA_GMAC_ENABLE << 2;
		tx_enable_mask = RK3588_TXCLK_DLY_ENA_GMAC_MASK << 2;
		intf_sel = RK3588_GMAC_PHY_INTF_SEL_RGMII << 6;
		intf_sel_mask = RK3588_GMAC_PHY_INTF_SEL_MASK << 6;
		clk_mode = RK3588_GMAC_CLK_RGMII_MODE << 5;
		clk_mode_mask = RK3588_GMAC_CLK_RGMII_MODE_MASK << 5;
	} else {
		offset_con = &grf->soc_con8;
		rx_enable_mask = RK3588_RXCLK_DLY_ENA_GMAC_MASK;
		tx_enable = RK3588_TXCLK_DLY_ENA_GMAC_ENABLE;
		tx_enable_mask = RK3588_TXCLK_DLY_ENA_GMAC_MASK;
		intf_sel = RK3588_GMAC_PHY_INTF_SEL_RGMII;
		intf_sel_mask = RK3588_GMAC_PHY_INTF_SEL_MASK;
		clk_mode = RK3588_GMAC_CLK_RGMII_MODE;
		clk_mode_mask = RK3588_GMAC_CLK_RGMII_MODE_MASK;
	}

	rk_clrsetreg(offset_con,
		     RK3588_CLK_TX_DL_CFG_GMAC_MASK |
		     RK3588_CLK_RX_DL_CFG_GMAC_MASK,
		     (pdata->tx_delay << RK3588_CLK_TX_DL_CFG_GMAC_SHIFT) |
		     rx_delay);

	rk_clrsetreg(&grf->soc_con7, tx_enable_mask | rx_enable_mask,
		     tx_enable | rx_enable);

	rk_clrsetreg(&php_grf->gmac_con0, intf_sel_mask, intf_sel);
	rk_clrsetreg(&php_grf->clk_con1, clk_mode_mask, clk_mode);
}

static void rv1106_gmac_integrated_phy_powerup(struct gmac_rockchip_platdata *pdata)
{
	struct rv1106_grf *grf;
	unsigned char bgs[1] = {0};

	enum {
		RV1106_VOGRF_GMAC_CLK_RMII_MODE_MASK = BIT(0),
		RV1106_VOGRF_GMAC_CLK_RMII_MODE = BIT(0),
	};

	enum {
		RV1106_MACPHY_ENABLE_MASK = BIT(1),
		RV1106_MACPHY_DISENABLE = BIT(1),
		RV1106_MACPHY_ENABLE = 0,
		RV1106_MACPHY_XMII_SEL_MASK = GENMASK(6, 5),
		RV1106_MACPHY_XMII_SEL = BIT(6),
		RV1106_MACPHY_24M_CLK_SEL_MASK = GENMASK(9, 7),
		RV1106_MACPHY_24M_CLK_SEL_24M = (BIT(8) | BIT(9)),
		RV1106_MACPHY_PHY_ID_MASK = GENMASK(14, 10),
		RV1106_MACPHY_PHY_ID = BIT(11),
	};

	enum {
		RV1106_MACPHY_BGS_MASK = GENMASK(3, 0),
		RV1106_MACPHY_BGS = BIT(2),
	};

#if defined(CONFIG_ROCKCHIP_EFUSE) || defined(CONFIG_ROCKCHIP_OTP)
	struct udevice *dev;
	u32 regs[2] = {0};
	ofnode node;
	int ret = 0;

	/* retrieve the device */
	if (IS_ENABLED(CONFIG_ROCKCHIP_EFUSE))
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_efuse),
						  &dev);
	else
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_otp),
						  &dev);
	if (!ret) {
		node = dev_read_subnode(dev, "macphy-bgs");
		if (ofnode_valid(node)) {
			if (!ofnode_read_u32_array(node, "reg", regs, 2)) {
				/* read the bgs from the efuses */
				ret = misc_read(dev, regs[0], &bgs, 1);
				if (ret) {
					printf("read bgs from efuse/otp failed, ret=%d\n",
					       ret);
					bgs[0] = 0;
				}
			}
		}
	}
#endif

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	reset_assert(&pdata->phy_reset);
	udelay(20);
	rk_clrsetreg(&grf->macphy_con0,
		     RV1106_MACPHY_ENABLE_MASK |
		     RV1106_MACPHY_XMII_SEL_MASK |
		     RV1106_MACPHY_24M_CLK_SEL_MASK |
		     RV1106_MACPHY_PHY_ID_MASK,
		     RV1106_MACPHY_ENABLE |
		     RV1106_MACPHY_XMII_SEL |
		     RV1106_MACPHY_24M_CLK_SEL_24M |
		     RV1106_MACPHY_PHY_ID);

	rk_clrsetreg(&grf->macphy_con1,
		     RV1106_MACPHY_BGS_MASK,
		     bgs[0]);
	udelay(20);
	reset_deassert(&pdata->phy_reset);
	udelay(30 * 1000);
}

static void rv1106_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rv1106_grf *grf;
	enum {
		RV1106_VOGRF_GMAC_CLK_RMII_MODE_MASK = BIT(0),
		RV1106_VOGRF_GMAC_CLK_RMII_MODE = BIT(0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	rk_clrsetreg(&grf->gmac_clk_con,
		     RV1106_VOGRF_GMAC_CLK_RMII_MODE_MASK,
		     RV1106_VOGRF_GMAC_CLK_RMII_MODE);
};

static void rv1126_set_to_rmii(struct gmac_rockchip_platdata *pdata)
{
	struct rv1126_grf *grf;

	enum {
		RV1126_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RV1126_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RV1126_GMAC_PHY_INTF_SEL_RMII = BIT(6),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	rk_clrsetreg(&grf->mac_con0,
		     RV1126_GMAC_PHY_INTF_SEL_MASK,
		     RV1126_GMAC_PHY_INTF_SEL_RMII);
}

static void rv1126_set_to_rgmii(struct gmac_rockchip_platdata *pdata)
{
	struct rv1126_grf *grf;

	enum {
		RV1126_GMAC_PHY_INTF_SEL_SHIFT = 4,
		RV1126_GMAC_PHY_INTF_SEL_MASK  = GENMASK(6, 4),
		RV1126_GMAC_PHY_INTF_SEL_RGMII = BIT(4),

		RV1126_RXCLK_M1_DLY_ENA_GMAC_MASK = BIT(3),
		RV1126_RXCLK_M1_DLY_ENA_GMAC_DISABLE = 0,
		RV1126_RXCLK_M1_DLY_ENA_GMAC_ENABLE = BIT(3),

		RV1126_TXCLK_M1_DLY_ENA_GMAC_MASK = BIT(2),
		RV1126_TXCLK_M1_DLY_ENA_GMAC_DISABLE = 0,
		RV1126_TXCLK_M1_DLY_ENA_GMAC_ENABLE = BIT(2),

		RV1126_RXCLK_M0_DLY_ENA_GMAC_MASK = BIT(1),
		RV1126_RXCLK_M0_DLY_ENA_GMAC_DISABLE = 0,
		RV1126_RXCLK_M0_DLY_ENA_GMAC_ENABLE = BIT(1),

		RV1126_TXCLK_M0_DLY_ENA_GMAC_MASK = BIT(0),
		RV1126_TXCLK_M0_DLY_ENA_GMAC_DISABLE = 0,
		RV1126_TXCLK_M0_DLY_ENA_GMAC_ENABLE = BIT(0),
	};
	enum {
		RV1126_M0_CLK_RX_DL_CFG_GMAC_SHIFT = 0x8,
		RV1126_M0_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(14, 8),

		RV1126_M0_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RV1126_M0_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(6, 0),
	};
	enum {
		RV1126_M1_CLK_RX_DL_CFG_GMAC_SHIFT = 0x8,
		RV1126_M1_CLK_RX_DL_CFG_GMAC_MASK = GENMASK(14, 8),

		RV1126_M1_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
		RV1126_M1_CLK_TX_DL_CFG_GMAC_MASK = GENMASK(6, 0),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	rk_clrsetreg(&grf->mac_con0,
		     RV1126_TXCLK_M0_DLY_ENA_GMAC_MASK |
		     RV1126_RXCLK_M0_DLY_ENA_GMAC_MASK |
		     RV1126_TXCLK_M1_DLY_ENA_GMAC_MASK |
		     RV1126_RXCLK_M1_DLY_ENA_GMAC_MASK |
		     RV1126_GMAC_PHY_INTF_SEL_MASK,
		     RV1126_TXCLK_M0_DLY_ENA_GMAC_ENABLE |
		     RV1126_RXCLK_M0_DLY_ENA_GMAC_ENABLE |
		     RV1126_TXCLK_M1_DLY_ENA_GMAC_ENABLE |
		     RV1126_RXCLK_M1_DLY_ENA_GMAC_ENABLE |
		     RV1126_GMAC_PHY_INTF_SEL_RGMII);

	rk_clrsetreg(&grf->mac_con1,
		     RV1126_M0_CLK_RX_DL_CFG_GMAC_MASK |
		     RV1126_M0_CLK_TX_DL_CFG_GMAC_MASK,
		     (pdata->rx_delay << RV1126_M0_CLK_RX_DL_CFG_GMAC_SHIFT) |
		     (pdata->tx_delay << RV1126_M0_CLK_TX_DL_CFG_GMAC_SHIFT));

	rk_clrsetreg(&grf->mac_con2,
		     RV1126_M1_CLK_RX_DL_CFG_GMAC_MASK |
		     RV1126_M1_CLK_TX_DL_CFG_GMAC_MASK,
		     (pdata->rx_delay << RV1126_M1_CLK_RX_DL_CFG_GMAC_SHIFT) |
		     (pdata->tx_delay << RV1126_M1_CLK_TX_DL_CFG_GMAC_SHIFT));
}
#endif

#ifdef CONFIG_DWC_ETH_QOS
static void rk3528_set_clock_selection(struct gmac_rockchip_platdata *pdata)
{
	struct rk3528_grf *grf;
	unsigned int val;

	enum {
		RK3528_GMAC1_CLK_SELET_SHIFT = 0x12,
		RK3528_GMAC1_CLK_SELET_MASK = BIT(12),
		RK3528_GMAC1_CLK_SELET_CRU = 0,
		RK3528_GMAC1_CLK_SELET_IO = BIT(12),
	};

	if (!pdata->bus_id)
		return;
	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	val = pdata->clock_input ? RK3528_GMAC1_CLK_SELET_IO :
				   RK3528_GMAC1_CLK_SELET_CRU;
	rk_clrsetreg(&grf->gmac1_con0, RK3528_GMAC1_CLK_SELET_MASK, val);
}

static void rk3562_set_clock_selection(struct gmac_rockchip_platdata *pdata)
{
	struct rk3562_grf *grf;
	struct rk3562_ioc *ioc;
	unsigned int val;

	enum {
		RK3562_GMAC0_CLK_SELET_SHIFT = 0x9,
		RK3562_GMAC0_CLK_SELET_MASK = BIT(9),
		RK3562_GMAC0_CLK_SELET_CRU = 0,
		RK3562_GMAC0_CLK_SELET_IO = BIT(9),
	};

	enum {
		RK3562_GMAC1_CLK_SELET_SHIFT = 15,
		RK3562_GMAC1_CLK_SELET_MASK = BIT(15),
		RK3562_GMAC1_CLK_SELET_CRU = 0,
		RK3562_GMAC1_CLK_SELET_IO = BIT(15),
	};

	enum {
		RK3562_GMAC0_IO_EXTCLK_SELET_SHIFT = 0x2,
		RK3562_GMAC0_IO_EXTCLK_SELET_MASK = BIT(2),
		RK3562_GMAC0_IO_EXTCLK_SELET_CRU = 0,
		RK3562_GMAC0_IO_EXTCLK_SELET_IO = BIT(2),
	};

	enum {
		RK3562_GMAC1_IO_EXTCLK_SELET_SHIFT = 0x3,
		RK3562_GMAC1_IO_EXTCLK_SELET_MASK = BIT(3),
		RK3562_GMAC1_IO_EXTCLK_SELET_CRU = 0,
		RK3562_GMAC1_IO_EXTCLK_SELET_IO = BIT(3),
	};

	grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	ioc = syscon_get_first_range(ROCKCHIP_SYSCON_IOC);

	if (!pdata->bus_id) {
		val = pdata->clock_input ? RK3562_GMAC0_CLK_SELET_IO :
					   RK3562_GMAC0_CLK_SELET_CRU;
		rk_clrsetreg(&grf->soc_con[0], RK3562_GMAC0_CLK_SELET_MASK, val);
		val = pdata->clock_input ? RK3562_GMAC0_IO_EXTCLK_SELET_IO :
					   RK3562_GMAC0_IO_EXTCLK_SELET_CRU;
		rk_clrsetreg(&ioc->mac1_io_con1,
			     RK3562_GMAC0_IO_EXTCLK_SELET_MASK, val);
		rk_clrsetreg(&ioc->mac0_io_con1,
			     RK3562_GMAC0_IO_EXTCLK_SELET_MASK, val);

	} else {
		val = pdata->clock_input ? RK3562_GMAC1_CLK_SELET_IO :
					   RK3562_GMAC1_CLK_SELET_CRU;
		rk_clrsetreg(&grf->soc_con[1], RK3562_GMAC1_CLK_SELET_MASK, val);
		val = pdata->clock_input ? RK3562_GMAC1_IO_EXTCLK_SELET_IO :
					   RK3562_GMAC1_IO_EXTCLK_SELET_CRU;
		rk_clrsetreg(&ioc->mac1_io_con1,
			     RK3562_GMAC1_IO_EXTCLK_SELET_MASK, val);
	}
}

static void rk3588_set_clock_selection(struct gmac_rockchip_platdata *pdata)
{
	struct rk3588_php_grf *php_grf;
	unsigned int val, mask;

	enum {
		RK3588_GMAC_CLK_SELET_SHIFT = 0x4,
		RK3588_GMAC_CLK_SELET_MASK = BIT(4),
		RK3588_GMAC_CLK_SELET_CRU = BIT(4),
		RK3588_GMAC_CLK_SELET_IO = 0,
	};

	php_grf = syscon_get_first_range(ROCKCHIP_SYSCON_PHP_GRF);
	val = pdata->clock_input ? RK3588_GMAC_CLK_SELET_IO :
				   RK3588_GMAC_CLK_SELET_CRU;
	mask = RK3588_GMAC_CLK_SELET_MASK;

	if (pdata->bus_id == 1) {
		val <<= 5;
		mask <<= 5;
	}

	rk_clrsetreg(&php_grf->clk_con1, mask, val);
}
#endif

static int gmac_rockchip_probe(struct udevice *dev)
{
	struct gmac_rockchip_platdata *pdata = dev_get_platdata(dev);
	struct rk_gmac_ops *ops =
		(struct rk_gmac_ops *)dev_get_driver_data(dev);
#ifdef CONFIG_DWC_ETH_QOS
	struct eqos_config *config;
#else
	struct dw_eth_pdata *dw_pdata;
#endif
	struct eth_pdata *eth_pdata;
	struct clk clk;
	ulong rate;
	int ret;

#ifdef CONFIG_DWC_ETH_QOS
	eth_pdata = &pdata->eth_pdata;
	config = (struct eqos_config *)&ops->config;
	memcpy(config, &eqos_rockchip_config, sizeof(struct eqos_config));
	eth_pdata->phy_interface = config->ops->eqos_get_interface(dev);
#else
	dw_pdata = &pdata->dw_eth_pdata;
	eth_pdata = &dw_pdata->eth_pdata;
#endif
	pdata->bus_id = dev->seq;

	/* Process 'assigned-{clocks/clock-parents/clock-rates}' properties */
	ret = clk_set_defaults(dev);
	if (ret)
		debug("%s clk_set_defaults failed %d\n", __func__, ret);

	ret = clk_get_by_index(dev, 0, &clk);
	if (ret)
		return ret;

	pdata->phy_interface = eth_pdata->phy_interface;

	if (ops->set_clock_selection)
		ops->set_clock_selection(pdata);

	if (pdata->integrated_phy && ops->integrated_phy_powerup)
		ops->integrated_phy_powerup(pdata);

	switch (eth_pdata->phy_interface) {
	case PHY_INTERFACE_MODE_RGMII:
	case PHY_INTERFACE_MODE_RGMII_RXID:
		/*
		 * If the gmac clock is from internal pll, need to set and
		 * check the return value for gmac clock at RGMII mode. If
		 * the gmac clock is from external source, the clock rate
		 * is not set, because of it is bypassed.
		 */
		if (!pdata->clock_input) {
			rate = clk_set_rate(&clk, 125000000);
			if (rate != 125000000)
				return -EINVAL;
		}

		if (eth_pdata->phy_interface == PHY_INTERFACE_MODE_RGMII_RXID)
			pdata->rx_delay = -1;

		/* Set to RGMII mode */
		if (ops->set_to_rgmii)
			ops->set_to_rgmii(pdata);
		else
			return -EPERM;

		break;
	case PHY_INTERFACE_MODE_RMII:
		/* The commet is the same as RGMII mode */
		if (!pdata->clock_input) {
			rate = clk_set_rate(&clk, 50000000);
			if (rate != 50000000)
				return -EINVAL;
		}

		/* Set to RMII mode */
		if (ops->set_to_rmii)
			ops->set_to_rmii(pdata);

		break;
	default:
		debug("NO interface defined!\n");
		return -ENXIO;
	}

#ifdef CONFIG_DWC_ETH_QOS
	return eqos_probe(dev);
#else
	return designware_eth_probe(dev);
#endif
}

static int gmac_rockchip_eth_write_hwaddr(struct udevice *dev)
{
#if defined(CONFIG_DWC_ETH_QOS)
	return eqos_write_hwaddr(dev);
#else
	return designware_eth_write_hwaddr(dev);
#endif
}

static int gmac_rockchip_eth_free_pkt(struct udevice *dev, uchar *packet,
				      int length)
{
#ifdef CONFIG_DWC_ETH_QOS
	return eqos_free_pkt(dev, packet, length);
#else
	return designware_eth_free_pkt(dev, packet, length);
#endif
}

static int gmac_rockchip_eth_send(struct udevice *dev, void *packet,
				  int length)
{
#ifdef CONFIG_DWC_ETH_QOS
	return eqos_send(dev, packet, length);
#else
	return designware_eth_send(dev, packet, length);
#endif
}

static int gmac_rockchip_eth_recv(struct udevice *dev, int flags,
				  uchar **packetp)
{
#ifdef CONFIG_DWC_ETH_QOS
	return eqos_recv(dev, flags, packetp);
#else
	return designware_eth_recv(dev, flags, packetp);
#endif
}

static int gmac_rockchip_eth_start(struct udevice *dev)
{
	struct rockchip_eth_dev *priv = dev_get_priv(dev);
	struct rk_gmac_ops *ops =
		(struct rk_gmac_ops *)dev_get_driver_data(dev);
	struct gmac_rockchip_platdata *pdata = dev_get_platdata(dev);
#ifndef CONFIG_DWC_ETH_QOS
	struct dw_eth_pdata *dw_pdata;
	struct eth_pdata *eth_pdata;
#endif
	int ret;

#ifdef CONFIG_DWC_ETH_QOS
	ret = eqos_init(dev);
#else
	dw_pdata = &pdata->dw_eth_pdata;
	eth_pdata = &dw_pdata->eth_pdata;
	ret = designware_eth_init((struct dw_eth_dev *)priv,
				  eth_pdata->enetaddr);
#endif
	if (ret)
		return ret;
	ret = ops->fix_mac_speed(pdata, priv);
	if (ret)
		return ret;

#ifdef CONFIG_DWC_ETH_QOS
	eqos_enable(dev);
#else
	ret = designware_eth_enable((struct dw_eth_dev *)priv);
	if (ret)
		return ret;
#endif

	return 0;
}

static void gmac_rockchip_eth_stop(struct udevice *dev)
{
#ifdef CONFIG_DWC_ETH_QOS
	eqos_stop(dev);
#else
	designware_eth_stop(dev);
#endif
}

const struct eth_ops gmac_rockchip_eth_ops = {
	.start			= gmac_rockchip_eth_start,
	.send			= gmac_rockchip_eth_send,
	.recv			= gmac_rockchip_eth_recv,
	.free_pkt		= gmac_rockchip_eth_free_pkt,
	.stop			= gmac_rockchip_eth_stop,
	.write_hwaddr		= gmac_rockchip_eth_write_hwaddr,
};

#ifndef CONFIG_DWC_ETH_QOS
const struct rk_gmac_ops px30_gmac_ops = {
	.fix_mac_speed = px30_gmac_fix_mac_speed,
	.set_to_rmii = px30_gmac_set_to_rmii,
};

const struct rk_gmac_ops rk1808_gmac_ops = {
	.fix_mac_speed = rk1808_gmac_fix_mac_speed,
	.set_to_rgmii = rk1808_gmac_set_to_rgmii,
};

const struct rk_gmac_ops rk3228_gmac_ops = {
	.fix_mac_speed = rk3228_gmac_fix_mac_speed,
	.set_to_rmii = rk3228_gmac_set_to_rmii,
	.set_to_rgmii = rk3228_gmac_set_to_rgmii,
	.integrated_phy_powerup = rk3228_gmac_integrated_phy_powerup,
};

const struct rk_gmac_ops rk3288_gmac_ops = {
	.fix_mac_speed = rk3288_gmac_fix_mac_speed,
	.set_to_rgmii = rk3288_gmac_set_to_rgmii,
};

const struct rk_gmac_ops rk3308_gmac_ops = {
	.fix_mac_speed = rk3308_gmac_fix_mac_speed,
	.set_to_rmii = rk3308_gmac_set_to_rmii,
};

const struct rk_gmac_ops rk3328_gmac_ops = {
	.fix_mac_speed = rk3328_gmac_fix_mac_speed,
	.set_to_rmii = rk3328_gmac_set_to_rmii,
	.set_to_rgmii = rk3328_gmac_set_to_rgmii,
	.integrated_phy_powerup = rk3328_gmac_integrated_phy_powerup,
};

const struct rk_gmac_ops rk3368_gmac_ops = {
	.fix_mac_speed = rk3368_gmac_fix_mac_speed,
	.set_to_rgmii = rk3368_gmac_set_to_rgmii,
};

const struct rk_gmac_ops rk3399_gmac_ops = {
	.fix_mac_speed = rk3399_gmac_fix_mac_speed,
	.set_to_rgmii = rk3399_gmac_set_to_rgmii,
};

const struct rk_gmac_ops rv1108_gmac_ops = {
	.fix_mac_speed = rv1108_set_rmii_speed,
	.set_to_rmii = rv1108_gmac_set_to_rmii,
};
#else
const struct rk_gmac_ops rk3528_gmac_ops = {
	.fix_mac_speed = rk3528_set_rgmii_speed,
	.set_to_rgmii = rk3528_set_to_rgmii,
	.set_to_rmii = rk3528_set_to_rmii,
	.set_clock_selection = rk3528_set_clock_selection,
	.integrated_phy_powerup = rk3528_gmac_integrated_phy_powerup,
};

const struct rk_gmac_ops rk3562_gmac_ops = {
	.fix_mac_speed = rk3562_set_gmac_speed,
	.set_to_rgmii = rk3562_set_to_rgmii,
	.set_to_rmii = rk3562_set_to_rmii,
	.set_clock_selection = rk3562_set_clock_selection,
};

const struct rk_gmac_ops rk3568_gmac_ops = {
	.fix_mac_speed = rv1126_set_rgmii_speed,
	.set_to_rgmii = rk3568_set_to_rgmii,
	.set_to_rmii = rk3568_set_to_rmii,
};

const struct rk_gmac_ops rk3588_gmac_ops = {
	.fix_mac_speed = rk3588_set_rgmii_speed,
	.set_to_rgmii = rk3588_set_to_rgmii,
	.set_to_rmii = rk3588_set_to_rmii,
	.set_clock_selection = rk3588_set_clock_selection,
};

const struct rk_gmac_ops rv1106_gmac_ops = {
	.fix_mac_speed = rv1106_set_rmii_speed,
	.set_to_rmii = rv1106_set_to_rmii,
	.integrated_phy_powerup = rv1106_gmac_integrated_phy_powerup,
};

const struct rk_gmac_ops rv1126_gmac_ops = {
	.fix_mac_speed = rv1126_set_rgmii_speed,
	.set_to_rgmii = rv1126_set_to_rgmii,
	.set_to_rmii = rv1126_set_to_rmii,
};
#endif

static const struct udevice_id rockchip_gmac_ids[] = {
#ifndef CONFIG_DWC_ETH_QOS
#ifdef CONFIG_ROCKCHIP_PX30
	{ .compatible = "rockchip,px30-gmac",
	  .data = (ulong)&px30_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK1808
	{ .compatible = "rockchip,rk1808-gmac",
	  .data = (ulong)&rk1808_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3228
	{ .compatible = "rockchip,rk3228-gmac",
	  .data = (ulong)&rk3228_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3288
	{ .compatible = "rockchip,rk3288-gmac",
	  .data = (ulong)&rk3288_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3308
	{ .compatible = "rockchip,rk3308-mac",
	  .data = (ulong)&rk3308_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3328
	{ .compatible = "rockchip,rk3328-gmac",
	  .data = (ulong)&rk3328_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3368
	{ .compatible = "rockchip,rk3368-gmac",
	  .data = (ulong)&rk3368_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3399
	{ .compatible = "rockchip,rk3399-gmac",
	  .data = (ulong)&rk3399_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RV1108
	{ .compatible = "rockchip,rv1108-gmac",
	  .data = (ulong)&rv1108_gmac_ops },
#endif
#else
#ifdef CONFIG_ROCKCHIP_RK3528
	{ .compatible = "rockchip,rk3528-gmac",
	  .data = (ulong)&rk3528_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3562
	{ .compatible = "rockchip,rk3562-gmac",
	  .data = (ulong)&rk3562_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3568
	{ .compatible = "rockchip,rk3568-gmac",
	  .data = (ulong)&rk3568_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RK3588
	{ .compatible = "rockchip,rk3588-gmac",
	  .data = (ulong)&rk3588_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RV1106
	{ .compatible = "rockchip,rv1106-gmac",
	  .data = (ulong)&rv1106_gmac_ops },
#endif

#ifdef CONFIG_ROCKCHIP_RV1126
	{ .compatible = "rockchip,rv1126-gmac",
	  .data = (ulong)&rv1126_gmac_ops },
#endif
#endif
	{ }
};

U_BOOT_DRIVER(eth_gmac_rockchip) = {
	.name	= "gmac_rockchip",
	.id	= UCLASS_ETH,
	.of_match = rockchip_gmac_ids,
	.ofdata_to_platdata = gmac_rockchip_ofdata_to_platdata,
	.probe	= gmac_rockchip_probe,
	.ops	= &gmac_rockchip_eth_ops,
	.priv_auto_alloc_size = sizeof(struct rockchip_eth_dev),
	.platdata_auto_alloc_size = sizeof(struct gmac_rockchip_platdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};
