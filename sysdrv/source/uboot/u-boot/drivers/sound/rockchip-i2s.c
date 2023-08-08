// SPDX-License-Identifier:     GPL-2.0+
/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 */

#include <clk.h>
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <asm/io.h>
#include <sound.h>
#include "rockchip-i2s.h"

#define I2S_FIFO_LENGTH (31)

struct rk_i2s_dev {
	void *regbase;
	struct clk mclk;
};

static inline u32 i2s_reg_readl(struct rk_i2s_dev *dev, u32 offset)
{
	return readl(dev->regbase + offset);
}

static inline void i2s_reg_writel(struct rk_i2s_dev *dev, u32 offset, u32 val)
{
	writel(val, dev->regbase + offset);
}

static inline void i2s_reg_update_bits(struct rk_i2s_dev *dev, u32 offset,
				       u32 mask, u32 val)
{
	u32 tmp, orig;

	orig = readl(dev->regbase + offset);

	tmp = orig & ~mask;
	tmp |= val & mask;

	if (tmp != orig)
		writel(tmp, dev->regbase + offset);
}

static void dump_regs(struct rk_i2s_dev *dev)
{
	int i = 0;

	for (i = 0; i <= I2S_RXDR; i += 4)
		debug("0x%02x: 0x%08x\n", i, readl(dev->regbase + i));
}

static int rk_i2s_hw_params(struct udevice *udev, unsigned int samplerate,
			    unsigned int fmt, unsigned int channels)
{
	struct rk_i2s_dev *dev = dev_get_priv(udev);

	/* set fmt */
	i2s_reg_update_bits(dev, I2S_CKR,
			    I2S_CKR_MSS_MASK, I2S_CKR_MSS_MASTER);
	i2s_reg_update_bits(dev, I2S_TXCR,
			    I2S_TXCR_IBM_MASK, I2S_TXCR_IBM_NORMAL);
	i2s_reg_update_bits(dev, I2S_RXCR,
			    I2S_RXCR_IBM_MASK, I2S_RXCR_IBM_NORMAL);
	/* set div */
	i2s_reg_update_bits(dev, I2S_CKR,
			    I2S_CKR_TSD_MASK | I2S_CKR_RSD_MASK,
			    I2S_CKR_TSD(64) | I2S_CKR_RSD(64));
	i2s_reg_update_bits(dev, I2S_CKR,
			    I2S_CKR_MDIV_MASK, I2S_CKR_MDIV(4));
	/* set hwparams */
	i2s_reg_update_bits(dev, I2S_TXCR,
			    I2S_TXCR_VDW_MASK |
			    I2S_TXCR_CSR_MASK,
			    I2S_TXCR_VDW(16) |
			    I2S_TXCR_CHN_2);
	i2s_reg_update_bits(dev, I2S_RXCR,
			    I2S_RXCR_CSR_MASK |
			    I2S_RXCR_VDW_MASK,
			    I2S_TXCR_VDW(16) |
			    I2S_TXCR_CHN_2);
	i2s_reg_update_bits(dev, I2S_DMACR,
			    I2S_DMACR_TDL_MASK | I2S_DMACR_RDL_MASK,
			    I2S_DMACR_TDL(16) | I2S_DMACR_RDL(16));

	return 0;
}

static void rk_i2s_txctrl(struct rk_i2s_dev *dev, int on)
{
	if (on) {
		i2s_reg_update_bits(dev, I2S_XFER,
				    I2S_XFER_TXS_MASK | I2S_XFER_RXS_MASK,
				    I2S_XFER_TXS_START | I2S_XFER_RXS_START);
	} else {
		i2s_reg_update_bits(dev, I2S_XFER,
				    I2S_XFER_TXS_MASK |
				    I2S_XFER_RXS_MASK,
				    I2S_XFER_TXS_STOP |
				    I2S_XFER_RXS_STOP);

		i2s_reg_update_bits(dev, I2S_CLR,
				    I2S_CLR_TXC_MASK | I2S_CLR_RXC_MASK,
				    I2S_CLR_TXC | I2S_CLR_RXC);
	}
}

static int rk_i2s_transfer_tx_data(struct udevice *udev, unsigned int *data,
				   unsigned long data_size)
{
	struct rk_i2s_dev *dev = dev_get_priv(udev);
	u32 val;

	if (data_size < I2S_FIFO_LENGTH) {
		debug("%s : invalid data size\n", __func__);
		return -EINVAL;
	}

	rk_i2s_txctrl(dev, 1);
	while (data_size > 0) {
		val = i2s_reg_readl(dev, I2S_FIFOLR);
		if (val < I2S_FIFO_LENGTH) {
			i2s_reg_writel(dev, I2S_TXDR, *data++);
			data_size--;
		}
	}

	return 0;
}

static int rk_i2s_set_sysclk(struct udevice *dev, unsigned int freq)
{
	struct rk_i2s_dev *i2s = dev_get_priv(dev);

	clk_set_rate(&i2s->mclk, freq);

	return 0;
}

static const struct snd_soc_dai_ops rockchip_i2s_ops = {
	.hw_params = rk_i2s_hw_params,
	.set_sysclk = rk_i2s_set_sysclk,
	.transfer = rk_i2s_transfer_tx_data,
};

static int rockchip_i2s_probe(struct udevice *dev)
{
	struct rk_i2s_dev *i2s = dev_get_priv(dev);
	int ret;

	i2s->regbase = dev_read_addr_ptr(dev);

	ret = clk_get_by_name(dev, "i2s_clk", &i2s->mclk);
	if (ret) {
		printf("%s get i2s mclk fail!\n", __func__);
		return -EINVAL;
	}

	dump_regs(i2s);
	return 0;
}

static const struct udevice_id rockchip_i2s_ids[] = {
	{ .compatible = "rockchip,px30-i2s", },
	{ .compatible = "rockchip,rk3036-i2s", },
	{ .compatible = "rockchip,rk3066-i2s", },
	{ .compatible = "rockchip,rk3128-i2s", },
	{ .compatible = "rockchip,rk3188-i2s", },
	{ .compatible = "rockchip,rk3288-i2s", },
	{ .compatible = "rockchip,rk3328-i2s", },
	{ .compatible = "rockchip,rk3368-i2s", },
	{ .compatible = "rockchip,rk3399-i2s", },
	{ }
};

U_BOOT_DRIVER(rockchip_i2s) = {
	.name = "rockchip_i2s",
	.id = UCLASS_I2S,
	.of_match = rockchip_i2s_ids,
	.probe = rockchip_i2s_probe,
	.priv_auto_alloc_size = sizeof(struct rk_i2s_dev),
	.ops = &rockchip_i2s_ops,
};

UCLASS_DRIVER(i2s) = {
	.id = UCLASS_I2S,
	.name = "i2s_bus",
};
