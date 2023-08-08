// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2008-2018 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Author: Wyon Bi <bivvy.bi@rock-chips.com>
 */

#include <config.h>
#include <common.h>
#include <errno.h>
#include <dm.h>
#include <asm/io.h>
#include <linux/iopoll.h>

#include "rockchip_phy.h"

/* Register: 0x0030 */
#define DISABLE_PLL	BIT(3)
/* Register: 0x003c */
#define PLL_LOCK	BIT(1)
/* Register: 0x0084 */
#define ENABLE_TX	BIT(7)

struct inno_video_phy {
	void __iomem *base;
	enum phy_mode mode;
	bool dual_channel;
};

struct reg_sequence {
	unsigned int reg;
	unsigned int def;
	unsigned int delay_us;
};

static const struct reg_sequence ttl_mode[] = {
	{ 0x0000, 0x7f },
	{ 0x0004, 0x3f },
	{ 0x0008, 0x80 },
	{ 0x0010, 0x3f },
	{ 0x0014, 0x3f },
	{ 0x0080, 0x44 },

	{ 0x0100, 0x7f },
	{ 0x0104, 0x3f },
	{ 0x0108, 0x80 },
	{ 0x0110, 0x3f },
	{ 0x0114, 0x3f },
	{ 0x0180, 0x44 },
};

static const struct reg_sequence lvds_mode_single_channel[] = {
	{ 0x0000, 0xbf },
	{ 0x0004, 0x3f },
	{ 0x0008, 0xfe },
	{ 0x0010, 0x00 },
	{ 0x0014, 0x00 },
	{ 0x0080, 0x44 },

	{ 0x0100, 0x00 },
	{ 0x0104, 0x00 },
	{ 0x0108, 0x00 },
	{ 0x0110, 0x00 },
	{ 0x0114, 0x00 },
	{ 0x0180, 0x44 },
};

static const struct reg_sequence lvds_mode_dual_channel[] = {
	{ 0x0000, 0xbf },
	{ 0x0004, 0x3f },
	{ 0x0008, 0xfe },
	{ 0x0010, 0x00 },
	{ 0x0014, 0x00 },
	{ 0x0080, 0x44 },

	{ 0x0100, 0xbf },
	{ 0x0104, 0x3f },
	{ 0x0108, 0xfe },
	{ 0x0110, 0x00 },
	{ 0x0114, 0x00 },
	{ 0x0180, 0x44 },
};

static inline void phy_write(struct inno_video_phy *inno, u32 reg, u32 val)
{
	writel(val, inno->base + reg);
}

static inline u32 phy_read(struct inno_video_phy *inno, u32 reg)
{
	return readl(inno->base + reg);
}

static inline void phy_update_bits(struct inno_video_phy *inno,
				   u32 reg, u32 mask, u32 val)
{
	u32 tmp, orig;

	orig = phy_read(inno, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	phy_write(inno, reg, tmp);
}

static void phy_multi_write(struct inno_video_phy *inno,
			    const struct reg_sequence *regs, int num_regs)
{
	int i;

	for (i = 0; i < num_regs; i++) {
		phy_write(inno, regs[i].reg, regs[i].def);

		if (regs[i].delay_us)
			udelay(regs[i].delay_us);
	}
}

static int inno_video_phy_power_on(struct rockchip_phy *phy)
{
	struct inno_video_phy *inno = dev_get_priv(phy->dev);
	const struct reg_sequence *wseq;
	int nregs;
	u32 status;
	int ret;

	switch (inno->mode) {
	case PHY_MODE_VIDEO_LVDS:
		if (inno->dual_channel) {
			wseq = lvds_mode_dual_channel;
			nregs = ARRAY_SIZE(lvds_mode_dual_channel);
		} else {
			wseq = lvds_mode_single_channel;
			nregs = ARRAY_SIZE(lvds_mode_single_channel);
		}
		break;
	case PHY_MODE_VIDEO_TTL:
		wseq = ttl_mode;
		nregs = ARRAY_SIZE(ttl_mode);
		break;
	default:
		return -EINVAL;
	}

	phy_multi_write(inno, wseq, nregs);

	phy_update_bits(inno, 0x0030, DISABLE_PLL, 0);
	ret = readl_poll_timeout(inno->base + 0x003c, status,
				 status & PLL_LOCK, 100000);
	if (ret) {
		dev_err(phy->dev, "PLL is not lock\n");
		return ret;
	}

	phy_update_bits(inno, 0x0084, ENABLE_TX, ENABLE_TX);

	return 0;
}

static int inno_video_phy_power_off(struct rockchip_phy *phy)
{
	struct inno_video_phy *inno = dev_get_priv(phy->dev);

	phy_update_bits(inno, 0x0084, ENABLE_TX, 0);
	phy_update_bits(inno, 0x0030, DISABLE_PLL, DISABLE_PLL);

	return 0;
}

static int inno_video_phy_set_mode(struct rockchip_phy *phy,
				   enum phy_mode mode)
{
	struct inno_video_phy *inno = dev_get_priv(phy->dev);

	switch (mode) {
	case PHY_MODE_VIDEO_LVDS:
	case PHY_MODE_VIDEO_TTL:
		inno->mode = mode;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int
inno_video_phy_set_bus_width(struct rockchip_phy *phy, u32 bus_width)
{
	struct inno_video_phy *inno = dev_get_priv(phy->dev);

	inno->dual_channel = (bus_width == 2) ? true : false;

	return 0;
}

static const struct rockchip_phy_funcs inno_video_phy_funcs = {
	.power_on = inno_video_phy_power_on,
	.power_off = inno_video_phy_power_off,
	.set_mode = inno_video_phy_set_mode,
	.set_bus_width = inno_video_phy_set_bus_width,
};

static int inno_video_phy_probe(struct udevice *dev)
{
	struct inno_video_phy *inno = dev_get_priv(dev);
	struct rockchip_phy *phy =
		(struct rockchip_phy *)dev_get_driver_data(dev);

	inno->base = dev_read_addr_ptr(dev);
	phy->dev = dev;

	return 0;
}

static struct rockchip_phy inno_video_phy_driver_data = {
	 .funcs = &inno_video_phy_funcs,
};

static const struct udevice_id inno_video_phy_ids[] = {
	{
		.compatible = "rockchip,rk3288-video-phy",
		.data = (ulong)&inno_video_phy_driver_data,
	},
	{}
};

U_BOOT_DRIVER(inno_video_phy) = {
	.name = "inno_video_phy",
	.id = UCLASS_PHY,
	.of_match = inno_video_phy_ids,
	.probe = inno_video_phy_probe,
	.priv_auto_alloc_size = sizeof(struct inno_video_phy),
};
