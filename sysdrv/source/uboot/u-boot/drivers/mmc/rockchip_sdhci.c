/*
 * (C) Copyright 2016 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Rockchip SD Host Controller Interface
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/hardware.h>
#include <common.h>
#include <dm.h>
#include <dt-structs.h>
#include <linux/libfdt.h>
#include <malloc.h>
#include <mapmem.h>
#include <sdhci.h>
#include <clk.h>
#include <syscon.h>
#include <dm/ofnode.h>
#include <asm/arch/clock.h>

DECLARE_GLOBAL_DATA_PTR;
/* 400KHz is max freq for card ID etc. Use that as min */
#define EMMC_MIN_FREQ	400000
#define KHz	(1000)
#define MHz	(1000 * KHz)

#define PHYCTRL_CALDONE_MASK		0x1
#define PHYCTRL_CALDONE_SHIFT		0x6
#define PHYCTRL_CALDONE_DONE		0x1
#define PHYCTRL_DLLRDY_MASK		0x1
#define PHYCTRL_DLLRDY_SHIFT		0x5
#define PHYCTRL_DLLRDY_DONE		0x1
#define PHYCTRL_FREQSEL_200M		0x0
#define PHYCTRL_FREQSEL_50M		0x1
#define PHYCTRL_FREQSEL_100M		0x2
#define PHYCTRL_FREQSEL_150M		0x3
#define PHYCTRL_DLL_LOCK_WO_TMOUT(x)	\
	((((x) >> PHYCTRL_DLLRDY_SHIFT) & PHYCTRL_DLLRDY_MASK) ==\
	PHYCTRL_DLLRDY_DONE)

#define ARASAN_VENDOR_REGISTER		0x78
#define ARASAN_VENDOR_ENHANCED_STROBE	BIT(0)

/* DWC IP vendor area 1 pointer */
#define DWCMSHC_P_VENDOR_AREA1		0xe8
#define DWCMSHC_AREA1_MASK		GENMASK(11, 0)
/* Rockchip specific Registers */
#define DWCMSHC_CTRL_HS400		0x7
#define DWCMSHC_CARD_IS_EMMC		BIT(0)
#define DWCMSHC_ENHANCED_STROBE		BIT(8)

#define DWCMSHC_HOST_CTRL3		0x508
#define DWCMSHC_EMMC_CONTROL		0x52c
#define DWCMSHC_EMMC_ATCTRL		0x540
#define DWCMSHC_EMMC_DLL_CTRL		0x800
#define DWCMSHC_EMMC_DLL_CTRL_RESET	BIT(1)
#define DWCMSHC_EMMC_DLL_RXCLK		0x804
#define DWCMSHC_EMMC_DLL_TXCLK		0x808
#define DWCMSHC_EMMC_DLL_STRBIN		0x80c
#define DECMSHC_EMMC_DLL_CMDOUT		0x810
#define DWCMSHC_EMMC_DLL_STATUS0	0x840
#define DWCMSHC_EMMC_DLL_STATUS1	0x844
#define DWCMSHC_EMMC_DLL_START		BIT(0)
#define DWCMSHC_EMMC_DLL_START_POINT	16
#define DWCMSHC_EMMC_DLL_START_DEFAULT	5
#define DWCMSHC_EMMC_DLL_INC_VALUE	2
#define DWCMSHC_EMMC_DLL_INC		8
#define DWCMSHC_EMMC_DLL_BYPASS		BIT(24)
#define DWCMSHC_EMMC_DLL_DLYENA		BIT(27)
#define DLL_TXCLK_TAPNUM_DEFAULT	0x10
#define DLL_TXCLK_TAPNUM_90_DEGREES	0x9
#define DLL_STRBIN_TAPNUM_DEFAULT	0x4
#define DLL_STRBIN_DELAY_NUM_OFFSET	16
#define DLL_STRBIN_TAPNUM_FROM_SW	BIT(24)
#define DLL_STRBIN_DELAY_NUM_SEL	BIT(26)
#define DLL_TXCLK_TAPNUM_FROM_SW	BIT(24)
#define DLL_TXCLK_NO_INVERTER		BIT(29)
#define DWCMSHC_EMMC_DLL_LOCKED		BIT(8)
#define DWCMSHC_EMMC_DLL_TIMEOUT	BIT(9)
#define DLL_TAP_VALUE_SEL		BIT(25)
#define DLL_TAP_VALUE_OFFSET		8
#define DLL_RXCLK_NO_INVERTER		BIT(29)
#define DLL_RXCLK_ORI_GATE		BIT(31)
#define DLL_CMDOUT_TAPNUM_90_DEGREES	0x8
#define DLL_CMDOUT_TAPNUM_FROM_SW	BIT(24)
#define DLL_CMDOUT_SRC_CLK_NEG		BIT(28)
#define DLL_CMDOUT_EN_SRC_CLK_NEG	BIT(29)
#define DLL_CMDOUT_BOTH_CLK_EDGE	BIT(30)

#define DWCMSHC_ENHANCED_STROBE		BIT(8)
#define DLL_LOCK_WO_TMOUT(x) \
	((((x) & DWCMSHC_EMMC_DLL_LOCKED) == DWCMSHC_EMMC_DLL_LOCKED) && \
	(((x) & DWCMSHC_EMMC_DLL_TIMEOUT) == 0))
#define ROCKCHIP_MAX_CLKS		3

struct rockchip_sdhc_plat {
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct dtd_rockchip_rk3399_sdhci_5_1 dtplat;
#endif
	struct mmc_config cfg;
	struct mmc mmc;
};

struct rockchip_emmc_phy {
	u32 emmcphy_con[7];
	u32 reserved;
	u32 emmcphy_status;
};

struct rockchip_sdhc {
	struct sdhci_host host;
	struct udevice *dev;
	void *base;
	struct rockchip_emmc_phy *phy;
	struct clk emmc_clk;
};

struct sdhci_data {
	int (*emmc_set_clock)(struct sdhci_host *host, unsigned int clock);
	void (*set_ios_post)(struct sdhci_host *host);
	int (*set_enhanced_strobe)(struct sdhci_host *host);
	int (*get_phy)(struct udevice *dev);
	u32 flags;
#define RK_DLL_CMD_OUT		BIT(1)
#define RK_RXCLK_NO_INVERTER	BIT(2)
#define RK_TAP_VALUE_SEL	BIT(3)

	u8 hs200_tx_tap;
	u8 hs400_tx_tap;
	u8 hs400_cmd_tap;
	u8 hs400_strbin_tap;
	u8 ddr50_strbin_delay_num;
};

static void rk3399_emmc_phy_power_on(struct rockchip_emmc_phy *phy, u32 clock)
{
	u32 caldone, dllrdy, freqsel;
	uint start;

	writel(RK_CLRSETBITS(7 << 4, 0), &phy->emmcphy_con[6]);
	writel(RK_CLRSETBITS(1 << 11, 1 << 11), &phy->emmcphy_con[0]);
	writel(RK_CLRSETBITS(0xf << 7, 6 << 7), &phy->emmcphy_con[0]);

	/*
	 * According to the user manual, calpad calibration
	 * cycle takes more than 2us without the minimal recommended
	 * value, so we may need a little margin here
	 */
	udelay(3);
	writel(RK_CLRSETBITS(1, 1), &phy->emmcphy_con[6]);

	/*
	 * According to the user manual, it asks driver to
	 * wait 5us for calpad busy trimming. But it seems that
	 * 5us of caldone isn't enough for all cases.
	 */
	udelay(500);
	caldone = readl(&phy->emmcphy_status);
	caldone = (caldone >> PHYCTRL_CALDONE_SHIFT) & PHYCTRL_CALDONE_MASK;
	if (caldone != PHYCTRL_CALDONE_DONE) {
		printf("%s: caldone timeout.\n", __func__);
		return;
	}

	/* Set the frequency of the DLL operation */
	if (clock < 75 * MHz)
		freqsel = PHYCTRL_FREQSEL_50M;
	else if (clock < 125 * MHz)
		freqsel = PHYCTRL_FREQSEL_100M;
	else if (clock < 175 * MHz)
		freqsel = PHYCTRL_FREQSEL_150M;
	else
		freqsel = PHYCTRL_FREQSEL_200M;

	/* Set the frequency of the DLL operation */
	writel(RK_CLRSETBITS(3 << 12, freqsel << 12), &phy->emmcphy_con[0]);
	writel(RK_CLRSETBITS(1 << 1, 1 << 1), &phy->emmcphy_con[6]);

	/* REN Enable on STRB Line for HS400 */
	writel(RK_CLRSETBITS(0, 1 << 9), &phy->emmcphy_con[2]);

	start = get_timer(0);

	do {
		udelay(1);
		dllrdy = readl(&phy->emmcphy_status);
		dllrdy = (dllrdy >> PHYCTRL_DLLRDY_SHIFT) & PHYCTRL_DLLRDY_MASK;
		if (dllrdy == PHYCTRL_DLLRDY_DONE)
			break;
	} while (get_timer(start) < 50000);

	if (dllrdy != PHYCTRL_DLLRDY_DONE)
		printf("%s: dllrdy timeout.\n", __func__);
}

static void rk3399_emmc_phy_power_off(struct rockchip_emmc_phy *phy)
{
	writel(RK_CLRSETBITS(1, 0), &phy->emmcphy_con[6]);
	writel(RK_CLRSETBITS(1 << 1, 0), &phy->emmcphy_con[6]);
}

static int rockchip_emmc_set_clock(struct sdhci_host *host, unsigned int clock)
{
	unsigned int div, clk = 0, timeout;
	unsigned int input_clk;
	struct rockchip_sdhc *priv =
			container_of(host, struct rockchip_sdhc, host);

	/* Wait max 20 ms */
	timeout = 200;
	while (sdhci_readl(host, SDHCI_PRESENT_STATE) &
			   (SDHCI_CMD_INHIBIT | SDHCI_DATA_INHIBIT)) {
		if (timeout == 0) {
			printf("%s: Timeout to wait cmd & data inhibit\n",
			       __func__);
			return -EBUSY;
		}

		timeout--;
		udelay(100);
	}
	sdhci_writew(host, 0, SDHCI_CLOCK_CONTROL);

	if (clock == 0)
		return 0;

	input_clk = clk_set_rate(&priv->emmc_clk, clock);
	if (IS_ERR_VALUE(input_clk))
		input_clk = host->max_clk;

	if (SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) {
		/*
		 * Check if the Host Controller supports Programmable Clock
		 * Mode.
		 */
		if (host->clk_mul) {
			for (div = 1; div <= 1024; div++) {
				if ((input_clk / div) <= clock)
					break;
			}

			/*
			 * Set Programmable Clock Mode in the Clock
			 * Control register.
			 */
			clk = SDHCI_PROG_CLOCK_MODE;
			div--;
		} else {
			/* Version 3.00 divisors must be a multiple of 2. */
			if (input_clk <= clock) {
				div = 1;
			} else {
				for (div = 2;
				     div < SDHCI_MAX_DIV_SPEC_300;
				     div += 2) {
					if ((input_clk / div) <= clock)
						break;
				}
			}
			div >>= 1;
		}
	} else {
		/* Version 2.00 divisors must be a power of 2. */
		for (div = 1; div < SDHCI_MAX_DIV_SPEC_200; div *= 2) {
			if ((input_clk / div) <= clock)
				break;
		}
		div >>= 1;
	}

	clk |= (div & SDHCI_DIV_MASK) << SDHCI_DIVIDER_SHIFT;
	clk |= ((div & SDHCI_DIV_HI_MASK) >> SDHCI_DIV_MASK_LEN)
		<< SDHCI_DIVIDER_HI_SHIFT;
	clk |= SDHCI_CLOCK_INT_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	sdhci_enable_clk(host, clk);

	return 0;
}

static int rk3399_emmc_get_phy(struct udevice *dev)
{
	struct rockchip_sdhc *priv = dev_get_priv(dev);

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	priv->phy = (struct rockchip_emmc_phy *)0xff77f780;
#else
	ofnode phy_node;
	void *grf_base;
	u32 grf_phy_offset, phandle;

	phandle = dev_read_u32_default(dev, "phys", 0);
	phy_node = ofnode_get_by_phandle(phandle);
	if (!ofnode_valid(phy_node)) {
		debug("Not found emmc phy device\n");
		return -ENODEV;
	}

	grf_base = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (IS_ERR(grf_base))
		printf("%s Get syscon grf failed", __func__);
	grf_phy_offset = ofnode_read_u32_default(phy_node, "reg", 0);

	priv->phy = (struct rockchip_emmc_phy *)(grf_base + grf_phy_offset);
#endif
	return 0;
}

static int rk3399_sdhci_emmc_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct rockchip_sdhc *priv =
			container_of(host, struct rockchip_sdhc, host);
	int cycle_phy = host->clock != clock &&
			clock > EMMC_MIN_FREQ;

	if (cycle_phy)
		rk3399_emmc_phy_power_off(priv->phy);

	rockchip_emmc_set_clock(host, clock);

	if (cycle_phy)
		rk3399_emmc_phy_power_on(priv->phy, clock);

	return 0;
}

static int dwcmshc_sdhci_emmc_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct rockchip_sdhc *priv = container_of(host, struct rockchip_sdhc, host);
	struct sdhci_data *data = (struct sdhci_data *)dev_get_driver_data(priv->dev);
	u32 txclk_tapnum, extra, dll_lock_value;
	int timeout = 500, ret;

	ret = rockchip_emmc_set_clock(host, clock);

	/* Disable output clock while config DLL */
	sdhci_writew(host, 0, SDHCI_CLOCK_CONTROL);

	if (clock >= 100 * MHz) {
		/* reset DLL */
		sdhci_writel(host, DWCMSHC_EMMC_DLL_CTRL_RESET, DWCMSHC_EMMC_DLL_CTRL);
		udelay(1);
		sdhci_writel(host, 0, DWCMSHC_EMMC_DLL_CTRL);

		extra = 0x1 << 16 | /* tune clock stop en */
			0x2 << 17 | /* pre-change delay */
			0x3 << 19;  /* post-change delay */
		sdhci_writel(host, extra, DWCMSHC_EMMC_ATCTRL);

		/* Init DLL settings */
		extra = DWCMSHC_EMMC_DLL_START_DEFAULT << DWCMSHC_EMMC_DLL_START_POINT |
			DWCMSHC_EMMC_DLL_INC_VALUE << DWCMSHC_EMMC_DLL_INC |
			DWCMSHC_EMMC_DLL_START;
		sdhci_writel(host, extra, DWCMSHC_EMMC_DLL_CTRL);

		while (1) {
			if (timeout < 0) {
				ret = -ETIMEDOUT;
				goto exit;
			}
			if (DLL_LOCK_WO_TMOUT((sdhci_readl(host, DWCMSHC_EMMC_DLL_STATUS0))))
				break;
			udelay(1);
			timeout--;
		}
		dll_lock_value = ((sdhci_readl(host, DWCMSHC_EMMC_DLL_STATUS0) & 0xFF) * 2 ) & 0xFF;
		extra = DWCMSHC_EMMC_DLL_DLYENA | DLL_RXCLK_ORI_GATE;
		if (data->flags & RK_RXCLK_NO_INVERTER)
			extra |= DLL_RXCLK_NO_INVERTER;
		if (data->flags & RK_TAP_VALUE_SEL)
			extra |= DLL_TAP_VALUE_SEL | (dll_lock_value << DLL_TAP_VALUE_OFFSET);
		sdhci_writel(host, extra, DWCMSHC_EMMC_DLL_RXCLK);

		txclk_tapnum = data->hs200_tx_tap;
		if ((data->flags & RK_DLL_CMD_OUT) &&
		    (host->mmc->timing == MMC_TIMING_MMC_HS400 ||
		    host->mmc->timing == MMC_TIMING_MMC_HS400ES)) {
			txclk_tapnum = data->hs400_tx_tap;

			extra = DLL_CMDOUT_SRC_CLK_NEG |
				DLL_CMDOUT_BOTH_CLK_EDGE |
				DWCMSHC_EMMC_DLL_DLYENA |
				data->hs400_cmd_tap |
				DLL_CMDOUT_TAPNUM_FROM_SW;
			if (data->flags & RK_TAP_VALUE_SEL)
				extra |= DLL_TAP_VALUE_SEL | (dll_lock_value << DLL_TAP_VALUE_OFFSET);
			sdhci_writel(host, extra, DECMSHC_EMMC_DLL_CMDOUT);
		}

		extra = DWCMSHC_EMMC_DLL_DLYENA |
			DLL_TXCLK_TAPNUM_FROM_SW |
			DLL_TXCLK_NO_INVERTER|
			txclk_tapnum;
		if (data->flags & RK_TAP_VALUE_SEL)
			extra |= DLL_TAP_VALUE_SEL | (dll_lock_value << DLL_TAP_VALUE_OFFSET);
		sdhci_writel(host, extra, DWCMSHC_EMMC_DLL_TXCLK);

		extra = DWCMSHC_EMMC_DLL_DLYENA |
			data->hs400_strbin_tap |
			DLL_STRBIN_TAPNUM_FROM_SW;
		if (data->flags & RK_TAP_VALUE_SEL)
			extra |= DLL_TAP_VALUE_SEL | (dll_lock_value << DLL_TAP_VALUE_OFFSET);
		sdhci_writel(host, extra, DWCMSHC_EMMC_DLL_STRBIN);
	} else {
		/* disable dll */
		sdhci_writel(host, 0, DWCMSHC_EMMC_DLL_CTRL);

		/* Disable cmd conflict check */
		extra = sdhci_readl(host, DWCMSHC_HOST_CTRL3);
		extra &= ~BIT(0);
		sdhci_writel(host, extra, DWCMSHC_HOST_CTRL3);

		/* reset the clock phase when the frequency is lower than 100MHz */
		sdhci_writel(host, DWCMSHC_EMMC_DLL_BYPASS | DWCMSHC_EMMC_DLL_START, DWCMSHC_EMMC_DLL_CTRL);
		sdhci_writel(host, DLL_RXCLK_ORI_GATE, DWCMSHC_EMMC_DLL_RXCLK);
		sdhci_writel(host, 0, DWCMSHC_EMMC_DLL_TXCLK);
		sdhci_writel(host, 0, DECMSHC_EMMC_DLL_CMDOUT);
		/*
		 * Before switching to hs400es mode, the driver will enable
		 * enhanced strobe first. PHY needs to configure the parameters
		 * of enhanced strobe first.
		 */
		extra = DWCMSHC_EMMC_DLL_DLYENA |
			DLL_STRBIN_DELAY_NUM_SEL |
			data->ddr50_strbin_delay_num << DLL_STRBIN_DELAY_NUM_OFFSET;
		sdhci_writel(host, extra, DWCMSHC_EMMC_DLL_STRBIN);
	}

exit:
	/* enable output clock */
	sdhci_enable_clk(host, 0);

	return ret;
}

static int dwcmshc_sdhci_set_enhanced_strobe(struct sdhci_host *host)
{
	struct mmc *mmc = host->mmc;
	u32 vendor;

	vendor = sdhci_readl(host, DWCMSHC_EMMC_CONTROL);
	if (mmc->timing == MMC_TIMING_MMC_HS400ES)
		vendor |= DWCMSHC_ENHANCED_STROBE;
	else
		vendor &= ~DWCMSHC_ENHANCED_STROBE;
	sdhci_writel(host, vendor, DWCMSHC_EMMC_CONTROL);

	/* some emmc device need a delay before send command */
	udelay(100);

	return 0;
}

static void dwcmshc_sdhci_set_ios_post(struct sdhci_host *host)
{
	u16 ctrl;
	u32 timing = host->mmc->timing;

	if (timing == MMC_TIMING_MMC_HS400 || timing == MMC_TIMING_MMC_HS400ES) {
		ctrl = sdhci_readw(host, SDHCI_HOST_CONTROL2);
		ctrl &= ~SDHCI_CTRL_UHS_MASK;
		ctrl |= DWCMSHC_CTRL_HS400;
		sdhci_writew(host, ctrl, SDHCI_HOST_CONTROL2);

		/* set CARD_IS_EMMC bit to enable Data Strobe for HS400 */
		ctrl = sdhci_readw(host, DWCMSHC_EMMC_CONTROL);
		ctrl |= DWCMSHC_CARD_IS_EMMC;
		sdhci_writew(host, ctrl, DWCMSHC_EMMC_CONTROL);
	}
}

static int dwcmshc_emmc_get_phy(struct udevice *dev)
{
	return 0;
}

static int rockchip_sdhci_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct rockchip_sdhc *priv =
			container_of(host, struct rockchip_sdhc, host);
	struct sdhci_data *data = (struct sdhci_data *)dev_get_driver_data(priv->dev);
	if (!data)
		return -EINVAL;

	return data->emmc_set_clock(host, clock);
}

static void rockchip_sdhci_set_ios_post(struct sdhci_host *host)
{
	struct rockchip_sdhc *priv =
			container_of(host, struct rockchip_sdhc, host);
	struct sdhci_data *data = (struct sdhci_data *)dev_get_driver_data(priv->dev);

	if (data && data->set_ios_post)
		data->set_ios_post(host);
}

static int rockchip_sdhci_set_enhanced_strobe(struct sdhci_host *host)
{
	struct rockchip_sdhc *priv = container_of(host, struct rockchip_sdhc, host);
	struct sdhci_data *data = (struct sdhci_data *)dev_get_driver_data(priv->dev);

	if (data->set_enhanced_strobe)
		return data->set_enhanced_strobe(host);

	return -ENOTSUPP;
}

static struct sdhci_ops rockchip_sdhci_ops = {
	.set_clock	= rockchip_sdhci_set_clock,
	.set_ios_post	= rockchip_sdhci_set_ios_post,
	.set_enhanced_strobe = rockchip_sdhci_set_enhanced_strobe,
};

static int rockchip_sdhci_probe(struct udevice *dev)
{
	struct sdhci_data *data = (struct sdhci_data *)dev_get_driver_data(dev);
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct rockchip_sdhc_plat *plat = dev_get_platdata(dev);
	struct rockchip_sdhc *prv = dev_get_priv(dev);
	struct sdhci_host *host = &prv->host;
	int max_frequency, ret;
	struct clk clk;

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct dtd_rockchip_rk3399_sdhci_5_1 *dtplat = &plat->dtplat;

	host->name = dev->name;
	host->ioaddr = map_sysmem(dtplat->reg[0], dtplat->reg[1]);
	host->host_caps |= MMC_MODE_8BIT;
	max_frequency = dtplat->max_frequency;
	ret = clk_get_by_index_platdata(dev, 0, dtplat->clocks, &clk);
#else
	max_frequency = dev_read_u32_default(dev, "max-frequency", 0);
	switch (dev_read_u32_default(dev, "bus-width", 4)) {
	case 8:
		host->host_caps |= MMC_MODE_8BIT;
		break;
	case 4:
		host->host_caps |= MMC_MODE_4BIT;
		break;
	case 1:
		break;
	default:
		printf("Invalid \"bus-width\" value\n");
		return -EINVAL;
	}
	ret = clk_get_by_index(dev, 0, &clk);
#endif
	if (!ret) {
		ret = clk_set_rate(&clk, max_frequency);
		if (IS_ERR_VALUE(ret))
			printf("%s clk set rate fail!\n", __func__);
	} else {
		printf("%s fail to get clk\n", __func__);
	}

	prv->emmc_clk = clk;
	prv->dev = dev;
	ret = data->get_phy(dev);
	if (ret)
		return ret;

	host->ops = &rockchip_sdhci_ops;

	host->max_clk = max_frequency;

	if (dev_read_bool(dev, "mmc-hs200-1_8v"))
		host->host_caps |= MMC_MODE_HS200;
	else if (dev_read_bool(dev, "mmc-hs400-1_8v"))
		host->host_caps |= MMC_MODE_HS400;

	if (data->set_enhanced_strobe && dev_read_bool(dev, "mmc-hs400-enhanced-strobe"))
		host->host_caps |= MMC_MODE_HS400ES;

	ret = sdhci_setup_cfg(&plat->cfg, host, 0, EMMC_MIN_FREQ);

	plat->cfg.fixed_drv_type = dev_read_u32_default(dev, "fixed-emmc-driver-type", 0);

	host->mmc = &plat->mmc;
	if (ret)
		return ret;
	host->mmc->priv = &prv->host;
	host->mmc->dev = dev;
	upriv->mmc = host->mmc;

	return sdhci_probe(dev);
}

static int rockchip_sdhci_of_to_plat(struct udevice *dev)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	struct sdhci_host *host = dev_get_priv(dev);

	host->name = dev->name;
	host->ioaddr = dev_read_addr_ptr(dev);
#endif

	return 0;
}

static int rockchip_sdhci_bind(struct udevice *dev)
{
	struct rockchip_sdhc_plat *plat = dev_get_platdata(dev);

	return sdhci_bind(dev, &plat->mmc, &plat->cfg);
}

static const struct sdhci_data arasan_data = {
	.emmc_set_clock = rk3399_sdhci_emmc_set_clock,
	.get_phy = rk3399_emmc_get_phy,
};

static const struct sdhci_data rk3568_data = {
	.emmc_set_clock = dwcmshc_sdhci_emmc_set_clock,
	.get_phy = dwcmshc_emmc_get_phy,
	.flags = RK_RXCLK_NO_INVERTER | RK_TAP_VALUE_SEL,
	.hs200_tx_tap = 16,
	.hs400_tx_tap = 8,
	.hs400_cmd_tap = 8,
	.hs400_strbin_tap = 3,
	.ddr50_strbin_delay_num = 16,
};

static const struct sdhci_data rk3588_data = {
	.emmc_set_clock = dwcmshc_sdhci_emmc_set_clock,
	.get_phy = dwcmshc_emmc_get_phy,
	.set_ios_post = dwcmshc_sdhci_set_ios_post,
	.set_enhanced_strobe = dwcmshc_sdhci_set_enhanced_strobe,
	.flags = RK_DLL_CMD_OUT | RK_TAP_VALUE_SEL,
	.hs200_tx_tap = 16,
	.hs400_tx_tap = 9,
	.hs400_cmd_tap = 8,
	.hs400_strbin_tap = 3,
	.ddr50_strbin_delay_num = 16,
};

static const struct sdhci_data rk3528_data = {
	.emmc_set_clock = dwcmshc_sdhci_emmc_set_clock,
	.get_phy = dwcmshc_emmc_get_phy,
	.set_ios_post = dwcmshc_sdhci_set_ios_post,
	.set_enhanced_strobe = dwcmshc_sdhci_set_enhanced_strobe,
	.flags = RK_DLL_CMD_OUT | RK_TAP_VALUE_SEL,
	.hs200_tx_tap = 12,
	.hs400_tx_tap = 6,
	.hs400_cmd_tap = 6,
	.hs400_strbin_tap = 3,
	.ddr50_strbin_delay_num = 10,
};

static const struct sdhci_data rk3562_data = {
	.emmc_set_clock = dwcmshc_sdhci_emmc_set_clock,
	.get_phy = dwcmshc_emmc_get_phy,
	.set_ios_post = dwcmshc_sdhci_set_ios_post,
	.set_enhanced_strobe = dwcmshc_sdhci_set_enhanced_strobe,
	.flags = RK_DLL_CMD_OUT | RK_TAP_VALUE_SEL,
	.hs200_tx_tap = 12,
	.hs400_tx_tap = 6,
	.hs400_cmd_tap = 6,
	.hs400_strbin_tap = 3,
	.ddr50_strbin_delay_num = 10,
};

static const struct sdhci_data rk3576_data = {
	.emmc_set_clock = dwcmshc_sdhci_emmc_set_clock,
	.get_phy = dwcmshc_emmc_get_phy,
	.set_ios_post = dwcmshc_sdhci_set_ios_post,
	.set_enhanced_strobe = dwcmshc_sdhci_set_enhanced_strobe,
	.flags = RK_DLL_CMD_OUT | RK_TAP_VALUE_SEL,
	.hs200_tx_tap = 16,
	.hs400_tx_tap = 7,
	.hs400_cmd_tap = 7,
	.hs400_strbin_tap = 5,
	.ddr50_strbin_delay_num = 16,
};

static const struct udevice_id sdhci_ids[] = {
	{
		.compatible = "arasan,sdhci-5.1",
		.data = (ulong)&arasan_data,
	},
	{
		.compatible = "snps,dwcmshc-sdhci",
		.data = (ulong)&rk3568_data,
	},
	{
		.compatible = "rockchip,rk3528-dwcmshc",
		.data = (ulong)&rk3528_data,
	},
	{
		.compatible = "rockchip,rk3562-dwcmshc",
		.data = (ulong)&rk3562_data,
	},
	{
		.compatible = "rockchip,rk3576-dwcmshc",
		.data = (ulong)&rk3576_data,
	},
	{
		.compatible = "rockchip,rk3588-dwcmshc",
		.data = (ulong)&rk3588_data,
	},
	{ }
};

U_BOOT_DRIVER(arasan_sdhci_drv) = {
	.name		= "rockchip_sdhci_5_1",
	.id		= UCLASS_MMC,
	.of_match	= sdhci_ids,
	.ofdata_to_platdata = rockchip_sdhci_of_to_plat,
	.ops		= &sdhci_ops,
	.bind		= rockchip_sdhci_bind,
	.probe		= rockchip_sdhci_probe,
	.priv_auto_alloc_size = sizeof(struct rockchip_sdhc),
	.platdata_auto_alloc_size = sizeof(struct rockchip_sdhc_plat),
};
