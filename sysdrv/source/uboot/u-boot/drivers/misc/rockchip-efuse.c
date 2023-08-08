/*
 * eFuse driver for Rockchip devices
 *
 * Copyright 2017, Theobroma Systems Design und Consulting GmbH
 * Written by Philipp Tomsich <philipp.tomsich@theobroma-systems.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/arm-smccc.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <misc.h>
#include <asm/arch/rockchip_smccc.h>

#define T_CSB_P_S		0
#define T_PGENB_P_S		0
#define T_LOAD_P_S		0
#define T_ADDR_P_S		0
#define T_STROBE_P_S		(0 + 110) /* 1.1us */
#define T_CSB_P_L		(0 + 110 + 1000 + 20) /* 200ns */
#define T_PGENB_P_L		(0 + 110 + 1000 + 20)
#define T_LOAD_P_L		(0 + 110 + 1000 + 20)
#define T_ADDR_P_L		(0 + 110 + 1000 + 20)
#define T_STROBE_P_L		(0 + 110 + 1000) /* 10us */
#define T_CSB_R_S		0
#define T_PGENB_R_S		0
#define T_LOAD_R_S		0
#define T_ADDR_R_S		2
#define T_STROBE_R_S		(2 + 3)
#define T_CSB_R_L		(2 + 3 + 3 + 3)
#define T_PGENB_R_L		(2 + 3 + 3 + 3)
#define T_LOAD_R_L		(2 + 3 + 3 + 3)
#define T_ADDR_R_L		(2 + 3 + 3 + 2)
#define T_STROBE_R_L		(2 + 3 + 3)

#define T_CSB_P			0x28
#define T_PGENB_P		0x2c
#define T_LOAD_P		0x30
#define T_ADDR_P		0x34
#define T_STROBE_P		0x38
#define T_CSB_R			0x3c
#define T_PGENB_R		0x40
#define T_LOAD_R		0x44
#define T_ADDR_R		0x48
#define T_STROBE_R		0x4c

#define RK1808_USER_MODE	BIT(0)
#define RK1808_INT_FINISH	BIT(0)
#define RK1808_AUTO_ENB		BIT(0)
#define RK1808_AUTO_RD		BIT(1)
#define RK1808_A_SHIFT		16
#define RK1808_A_MASK		0x3ff
#define RK1808_NBYTES		4

#define RK3399_A_SHIFT          16
#define RK3399_A_MASK           0x3ff
#define RK3399_NFUSES           32
#define RK3399_BYTES_PER_FUSE   4
#define RK3399_STROBSFTSEL      BIT(9)
#define RK3399_RSB              BIT(7)
#define RK3399_PD               BIT(5)
#define RK3399_PGENB            BIT(3)
#define RK3399_LOAD             BIT(2)
#define RK3399_STROBE           BIT(1)
#define RK3399_CSB              BIT(0)

#define RK3288_A_SHIFT          6
#define RK3288_A_MASK           0x3ff
#define RK3288_NFUSES           32
#define RK3288_BYTES_PER_FUSE   1
#define RK3288_PGENB            BIT(3)
#define RK3288_LOAD             BIT(2)
#define RK3288_STROBE           BIT(1)
#define RK3288_CSB              BIT(0)

#define RK3328_INT_STATUS	0x0018
#define RK3328_DOUT		0x0020
#define RK3328_AUTO_CTRL	0x0024
#define RK3328_INT_FINISH	BIT(0)
#define RK3328_AUTO_ENB		BIT(0)
#define RK3328_AUTO_RD		BIT(1)

typedef int (*EFUSE_READ)(struct udevice *dev, int offset, void *buf, int size);

struct rockchip_efuse_regs {
	u32 ctrl;      /* 0x00  efuse control register */
	u32 dout;      /* 0x04  efuse data out register */
	u32 rf;        /* 0x08  efuse redundancy bit used register */
	u32 _rsvd0;
	u32 jtag_pass; /* 0x10  JTAG password */
	u32 strobe_finish_ctrl;
		       /* 0x14	efuse strobe finish control register */
	u32 int_status;/* 0x18 */
	u32 reserved;  /* 0x1c */
	u32 dout2;     /* 0x20 */
	u32 auto_ctrl; /* 0x24 */
};

struct rockchip_efuse_platdata {
	void __iomem *base;
	struct clk *clk;
};

static void rk1808_efuse_timing_init(void __iomem *base)
{
	static bool init;

	if (init)
		return;

	/* enable auto mode */
	writel(readl(base) & (~RK1808_USER_MODE), base);

	/* setup efuse timing */
	writel((T_CSB_P_S << 16) | T_CSB_P_L, base + T_CSB_P);
	writel((T_PGENB_P_S << 16) | T_PGENB_P_L, base + T_PGENB_P);
	writel((T_LOAD_P_S << 16) | T_LOAD_P_L, base + T_LOAD_P);
	writel((T_ADDR_P_S << 16) | T_ADDR_P_L, base + T_ADDR_P);
	writel((T_STROBE_P_S << 16) | T_STROBE_P_L, base + T_STROBE_P);
	writel((T_CSB_R_S << 16) | T_CSB_R_L, base + T_CSB_R);
	writel((T_PGENB_R_S << 16) | T_PGENB_R_L, base + T_PGENB_R);
	writel((T_LOAD_R_S << 16) | T_LOAD_R_L, base + T_LOAD_R);
	writel((T_ADDR_R_S << 16) | T_ADDR_R_L, base + T_ADDR_R);
	writel((T_STROBE_R_S << 16) | T_STROBE_R_L, base + T_STROBE_R);

	init = true;
}

static int rockchip_rk1808_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;
	unsigned int addr_start, addr_end, addr_offset, addr_len;
	u32 out_value, status;
	u8 *buffer;
	int ret = 0, i = 0;

	rk1808_efuse_timing_init(plat->base);

	addr_start = rounddown(offset, RK1808_NBYTES) / RK1808_NBYTES;
	addr_end = roundup(offset + size, RK1808_NBYTES) / RK1808_NBYTES;
	addr_offset = offset % RK1808_NBYTES;
	addr_len = addr_end - addr_start;

	buffer = calloc(1, sizeof(*buffer) * addr_len * RK1808_NBYTES);
	if (!buffer)
		return -ENOMEM;

	while (addr_len--) {
		writel(RK1808_AUTO_RD | RK1808_AUTO_ENB |
		       ((addr_start++ & RK1808_A_MASK) << RK1808_A_SHIFT),
		       &efuse->auto_ctrl);
		udelay(2);
		status = readl(&efuse->int_status);
		if (!(status & RK1808_INT_FINISH)) {
			ret = -EIO;
			goto err;
		}
		out_value = readl(&efuse->dout2);
		writel(RK1808_INT_FINISH, &efuse->int_status);

		memcpy(&buffer[i], &out_value, RK1808_NBYTES);
		i += RK1808_NBYTES;
	}
	memcpy(buf, buffer + addr_offset, size);
err:
	kfree(buffer);

	return ret;
}

#ifndef CONFIG_SPL_BUILD
static int rockchip_rk3368_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;
	u8 *buffer = buf;
	struct arm_smccc_res res;

	/* Switch to read mode */
	sip_smc_secure_reg_write((ulong)&efuse->ctrl,
				 RK3288_LOAD | RK3288_PGENB);
	udelay(1);
	while (size--) {
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl, res.a1 &
					 (~(RK3288_A_MASK << RK3288_A_SHIFT)));
		/* set addr */
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl, res.a1 |
					 ((offset++ & RK3288_A_MASK) <<
					  RK3288_A_SHIFT));
		udelay(1);
		/* strobe low to high */
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl,
					 res.a1 | RK3288_STROBE);
		ndelay(60);
		/* read data */
		res = sip_smc_secure_reg_read((ulong)&efuse->dout);
		*buffer++ = res.a1;
		/* reset strobe to low */
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl,
					 res.a1 & (~RK3288_STROBE));
		udelay(1);
	}

	/* Switch to standby mode */
	sip_smc_secure_reg_write((ulong)&efuse->ctrl,
				 RK3288_PGENB | RK3288_CSB);

	return 0;
}
#endif

static int rockchip_rk3399_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;

	unsigned int addr_start, addr_end, addr_offset;
	u32 out_value;
	u8  bytes[RK3399_NFUSES * RK3399_BYTES_PER_FUSE];
	int i = 0;
	u32 addr;

	addr_start = offset / RK3399_BYTES_PER_FUSE;
	addr_offset = offset % RK3399_BYTES_PER_FUSE;
	addr_end = DIV_ROUND_UP(offset + size, RK3399_BYTES_PER_FUSE);

	/* cap to the size of the efuse block */
	if (addr_end > RK3399_NFUSES)
		addr_end = RK3399_NFUSES;

	writel(RK3399_LOAD | RK3399_PGENB | RK3399_STROBSFTSEL | RK3399_RSB,
	       &efuse->ctrl);
	udelay(1);
	for (addr = addr_start; addr < addr_end; addr++) {
		setbits_le32(&efuse->ctrl,
			     RK3399_STROBE | (addr << RK3399_A_SHIFT));
		udelay(1);
		out_value = readl(&efuse->dout);
		clrbits_le32(&efuse->ctrl, RK3399_STROBE);
		udelay(1);

		memcpy(&bytes[i], &out_value, RK3399_BYTES_PER_FUSE);
		i += RK3399_BYTES_PER_FUSE;
	}

	/* Switch to standby mode */
	writel(RK3399_PD | RK3399_CSB, &efuse->ctrl);

	memcpy(buf, bytes + addr_offset, size);

	return 0;
}

static int rockchip_rk3288_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;
	u8 *buffer = buf;
	int max_size = RK3288_NFUSES * RK3288_BYTES_PER_FUSE;

	if (size > (max_size - offset))
		size = max_size - offset;

	/* Switch to read mode */
	writel(RK3288_LOAD | RK3288_PGENB, &efuse->ctrl);
	udelay(1);

	while (size--) {
		writel(readl(&efuse->ctrl) &
				(~(RK3288_A_MASK << RK3288_A_SHIFT)),
				&efuse->ctrl);
		/* set addr */
		writel(readl(&efuse->ctrl) |
				((offset++ & RK3288_A_MASK) << RK3288_A_SHIFT),
				&efuse->ctrl);
		udelay(1);
		/* strobe low to high */
		writel(readl(&efuse->ctrl) |
				RK3288_STROBE, &efuse->ctrl);
		ndelay(60);
		/* read data */
		*buffer++ = readl(&efuse->dout);
		/* reset strobe to low */
		writel(readl(&efuse->ctrl) &
				(~RK3288_STROBE), &efuse->ctrl);
		udelay(1);
	}

	/* Switch to standby mode */
	writel(RK3288_PGENB | RK3288_CSB, &efuse->ctrl);

	return 0;
}

#ifndef CONFIG_SPL_BUILD
static int rockchip_rk3288_efuse_secure_read(struct udevice *dev, int offset,
					     void *buf, int size)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;
	u8 *buffer = buf;
	int max_size = RK3288_NFUSES * RK3288_BYTES_PER_FUSE;
	struct arm_smccc_res res;

	if (size > (max_size - offset))
		size = max_size - offset;

	/* Switch to read mode */
	sip_smc_secure_reg_write((ulong)&efuse->ctrl,
				 RK3288_LOAD | RK3288_PGENB);
	udelay(1);
	while (size--) {
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl, res.a1 &
					 (~(RK3288_A_MASK << RK3288_A_SHIFT)));
		/* set addr */
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl, res.a1 |
					 ((offset++ & RK3288_A_MASK) <<
					  RK3288_A_SHIFT));
		udelay(1);
		/* strobe low to high */
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl,
					 res.a1 | RK3288_STROBE);
		ndelay(60);
		/* read data */
		res = sip_smc_secure_reg_read((ulong)&efuse->dout);
		*buffer++ = res.a1;
		/* reset strobe to low */
		res = sip_smc_secure_reg_read((ulong)&efuse->ctrl);
		sip_smc_secure_reg_write((ulong)&efuse->ctrl,
					 res.a1 & (~RK3288_STROBE));
		udelay(1);
	}

	/* Switch to standby mode */
	sip_smc_secure_reg_write((ulong)&efuse->ctrl,
				 RK3288_PGENB | RK3288_CSB);

	return 0;
}
#endif

static int rockchip_rk3328_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;
	unsigned int addr_start, addr_end, addr_offset, addr_len;
	u32 out_value, status;
	u8 *buffer;
	int ret = 0, i = 0, j = 0;

	/* Max non-secure Byte */
	if (size > 32)
		size = 32;

	/* 128 Byte efuse, 96 Byte for secure, 32 Byte for non-secure */
	offset += 96;
	addr_start = rounddown(offset, RK3399_BYTES_PER_FUSE) /
						RK3399_BYTES_PER_FUSE;
	addr_end = roundup(offset + size, RK3399_BYTES_PER_FUSE) /
						RK3399_BYTES_PER_FUSE;
	addr_offset = offset % RK3399_BYTES_PER_FUSE;
	addr_len = addr_end - addr_start;

	buffer = calloc(1, sizeof(*buffer) * addr_len * RK3399_BYTES_PER_FUSE);
	if (!buffer)
		return -ENOMEM;

	for (j = 0; j < addr_len; j++) {
		writel(RK3328_AUTO_RD | RK3328_AUTO_ENB |
		       ((addr_start++ & RK3399_A_MASK) << RK3399_A_SHIFT),
		         &efuse->auto_ctrl);
		udelay(5);
		status = readl(&efuse->int_status);
		if (!(status & RK3328_INT_FINISH)) {
			ret = -EIO;
			goto err;
		}
		out_value = readl(&efuse->dout2);
		writel(RK3328_INT_FINISH, &efuse->int_status);

		memcpy(&buffer[i], &out_value, RK3399_BYTES_PER_FUSE);
		i += RK3399_BYTES_PER_FUSE;
	}
	memcpy(buf, buffer + addr_offset, size);
err:
	free(buffer);

	return ret;
}

static int rockchip_efuse_read(struct udevice *dev, int offset,
			       void *buf, int size)
{
	EFUSE_READ efuse_read = NULL;

	efuse_read = (EFUSE_READ)dev_get_driver_data(dev);
	if (!efuse_read)
		return -ENOSYS;

	return (*efuse_read)(dev, offset, buf, size);
}

static int rockchip_efuse_capatiblity(struct udevice *dev, u32 *buf)
{
	*buf = device_is_compatible(dev, "rockchip,rk3288-secure-efuse") ?
	       OTP_S : OTP_NS;

	return 0;
}

static int rockchip_efuse_ioctl(struct udevice *dev, unsigned long request,
				void *buf)
{
	int ret = -EINVAL;

	switch (request) {
	case IOCTL_REQ_CAPABILITY:
		ret = rockchip_efuse_capatiblity(dev, buf);
		break;
	}

	return ret;
}

static const struct misc_ops rockchip_efuse_ops = {
	.read = rockchip_efuse_read,
	.ioctl = rockchip_efuse_ioctl,
};

static int rockchip_efuse_ofdata_to_platdata(struct udevice *dev)
{
	struct rockchip_efuse_platdata *plat = dev_get_platdata(dev);

	plat->base = dev_read_addr_ptr(dev);
	return 0;
}

static const struct udevice_id rockchip_efuse_ids[] = {
	{
		.compatible = "rockchip,rk1808-efuse",
		.data = (ulong)&rockchip_rk1808_efuse_read,
	},
#ifndef CONFIG_SPL_BUILD
	{
		.compatible = "rockchip,rk3288-secure-efuse",
		.data = (ulong)&rockchip_rk3288_efuse_secure_read,
	},
#endif
	{
		.compatible = "rockchip,rk3066a-efuse",
		.data = (ulong)&rockchip_rk3288_efuse_read,
	},
	{
		.compatible = "rockchip,rk3188-efuse",
		.data = (ulong)&rockchip_rk3288_efuse_read,
	},
	{
		.compatible = "rockchip,rk322x-efuse",
		.data = (ulong)&rockchip_rk3288_efuse_read,
	},
	{
		.compatible = "rockchip,rk3328-efuse",
		.data = (ulong)&rockchip_rk3328_efuse_read,
	},
#ifndef CONFIG_SPL_BUILD
	{
		.compatible = "rockchip,rk3368-efuse",
		.data = (ulong)&rockchip_rk3368_efuse_read,
	},
#endif
	{
		.compatible = "rockchip,rk3399-efuse",
		.data = (ulong)&rockchip_rk3399_efuse_read,
	},
	{}
};

U_BOOT_DRIVER(rockchip_efuse) = {
	.name = "rockchip_efuse",
	.id = UCLASS_MISC,
	.of_match = rockchip_efuse_ids,
	.ofdata_to_platdata = rockchip_efuse_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct rockchip_efuse_platdata),
	.ops = &rockchip_efuse_ops,
};
