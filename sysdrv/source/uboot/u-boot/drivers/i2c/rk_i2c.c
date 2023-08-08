/*
 * (C) Copyright 2015 Google, Inc
 *
 * (C) Copyright 2008-2014 Rockchip Electronics
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/i2c.h>
#include <asm/arch/periph.h>
#include <dm/pinctrl.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

/* i2c timerout */
#define I2C_TIMEOUT_MS		100
#define I2C_RETRY_COUNT		3

/* rk i2c fifo max transfer bytes */
#define RK_I2C_FIFO_SIZE	32

struct rk_i2c {
	struct clk clk;
	struct i2c_regs *regs;
	unsigned int speed;
	unsigned int cfg;
};

struct i2c_spec_values {
	unsigned int min_low_ns;
	unsigned int min_high_ns;
	unsigned int max_rise_ns;
	unsigned int max_fall_ns;
};

enum {
	RK_I2C_VERSION0 = 0,
	RK_I2C_VERSION1,
	RK_I2C_VERSION5 = 5,
};

/********************* Private Variable Definition ***************************/

static const struct i2c_spec_values standard_mode_spec = {
	.min_low_ns = 4700,
	.min_high_ns = 4000,
	.max_rise_ns = 1000,
	.max_fall_ns = 300,
};

static const struct i2c_spec_values fast_mode_spec = {
	.min_low_ns = 1300,
	.min_high_ns = 600,
	.max_rise_ns = 300,
	.max_fall_ns = 300,
};

static const struct i2c_spec_values fast_modeplus_spec = {
	.min_low_ns = 500,
	.min_high_ns = 260,
	.max_rise_ns = 120,
	.max_fall_ns = 120,
};

static const struct i2c_spec_values *rk_i2c_get_spec(unsigned int speed)
{
	if (speed == 1000)
		return &fast_modeplus_spec;
	else if (speed == 400)
		return &fast_mode_spec;
	else
		return &standard_mode_spec;
}

static void rk_i2c_show_regs(struct i2c_regs *regs)
{
#ifdef DEBUG
	uint i;

	debug("i2c_con: 0x%08x\n", readl(&regs->con));
	debug("i2c_clkdiv: 0x%08x\n", readl(&regs->clkdiv));
	debug("i2c_mrxaddr: 0x%08x\n", readl(&regs->mrxaddr));
	debug("i2c_mrxraddR: 0x%08x\n", readl(&regs->mrxraddr));
	debug("i2c_mtxcnt: 0x%08x\n", readl(&regs->mtxcnt));
	debug("i2c_mrxcnt: 0x%08x\n", readl(&regs->mrxcnt));
	debug("i2c_ien: 0x%08x\n", readl(&regs->ien));
	debug("i2c_ipd: 0x%08x\n", readl(&regs->ipd));
	debug("i2c_fcnt: 0x%08x\n", readl(&regs->fcnt));
	for (i = 0; i < 8; i++)
		debug("i2c_txdata%d: 0x%08x\n", i, readl(&regs->txdata[i]));
	for (i = 0; i < 8; i++)
		debug("i2c_rxdata%d: 0x%08x\n", i, readl(&regs->rxdata[i]));
#endif
}

static inline void rk_i2c_get_div(int div, int *divh, int *divl)
{
	*divl = div / 2;
	if (div % 2 == 0)
		*divh = div / 2;
	else
		*divh = DIV_ROUND_UP(div, 2);
}

/*
 * SCL Divisor = 8 * (CLKDIVL+1 + CLKDIVH+1)
 * SCL = PCLK / SCLK Divisor
 * i2c_rate = PCLK
 */
static void rk_i2c_set_clk(struct rk_i2c *i2c, unsigned int scl_rate)
{
	unsigned int i2c_rate;
	int div, divl, divh;

	/* First get i2c rate from pclk */
	i2c_rate = clk_get_rate(&i2c->clk);

	div = DIV_ROUND_UP(i2c_rate, scl_rate * 8) - 2;
	divh = 0;
	divl = 0;
	if (div >= 0)
		rk_i2c_get_div(div, &divh, &divl);
	writel(I2C_CLKDIV_VAL(divl, divh), &i2c->regs->clkdiv);

	debug("rk_i2c_set_clk: i2c rate = %d, scl rate = %d\n", i2c_rate,
	      scl_rate);
	debug("set i2c clk div = %d, divh = %d, divl = %d\n", div, divh, divl);
	debug("set clk(I2C_CLKDIV: 0x%08x)\n", readl(&i2c->regs->clkdiv));
}

static int rk_i2c_adapter_clk(struct rk_i2c *i2c, unsigned int scl_rate)
{
	const struct i2c_spec_values *spec;
	unsigned int min_total_div, min_low_div, min_high_div, min_hold_div;
	unsigned int low_div, high_div, extra_div, extra_low_div;
	unsigned int min_low_ns, min_high_ns;
	unsigned int start_setup = 0;
	unsigned int i2c_rate = clk_get_rate(&i2c->clk);
	unsigned int speed;

	debug("rk_i2c_set_clk: i2c rate = %d, scl rate = %d\n", i2c_rate,
	      scl_rate);

	if (scl_rate <= 100000 && scl_rate >= 1000) {
		start_setup = 1;
		speed = 100;
	} else if (scl_rate <= 400000 && scl_rate >= 100000) {
		speed = 400;
	} else if (scl_rate <= 1000000 && scl_rate > 400000) {
		speed = 1000;
	} else {
		debug("invalid i2c speed : %d\n", scl_rate);
		return -EINVAL;
	}

	spec = rk_i2c_get_spec(speed);
	i2c_rate = DIV_ROUND_UP(i2c_rate, 1000);
	speed = DIV_ROUND_UP(scl_rate, 1000);

	min_total_div = DIV_ROUND_UP(i2c_rate, speed * 8);

	min_high_ns = spec->max_rise_ns + spec->min_high_ns;
	min_high_div = DIV_ROUND_UP(i2c_rate * min_high_ns, 8 * 1000000);

	min_low_ns = spec->max_fall_ns + spec->min_low_ns;
	min_low_div = DIV_ROUND_UP(i2c_rate * min_low_ns, 8 * 1000000);

	min_high_div = (min_high_div < 1) ? 2 : min_high_div;
	min_low_div = (min_low_div < 1) ? 2 : min_low_div;

	min_hold_div = min_high_div + min_low_div;

	if (min_hold_div >= min_total_div) {
		high_div = min_high_div;
		low_div = min_low_div;
	} else {
		extra_div = min_total_div - min_hold_div;
		extra_low_div = DIV_ROUND_UP(min_low_div * extra_div,
					     min_hold_div);

		low_div = min_low_div + extra_low_div;
		high_div = min_high_div + (extra_div - extra_low_div);
	}

	high_div--;
	low_div--;

	if (high_div > 0xffff || low_div > 0xffff)
		return -EINVAL;

	/* 1 for data hold/setup time is enough */
	i2c->cfg = I2C_CON_SDA_CFG(1) | I2C_CON_STA_CFG(start_setup);
	writel((high_div << I2C_CLK_DIV_HIGH_SHIFT) | low_div,
	       &i2c->regs->clkdiv);

	debug("set clk(I2C_TIMING: 0x%08x)\n", i2c->cfg);
	debug("set clk(I2C_CLKDIV: 0x%08x)\n", readl(&i2c->regs->clkdiv));

	return 0;
}

static int rk_i2c_send_start_bit(struct rk_i2c *i2c, u32 con)
{
	struct i2c_regs *regs = i2c->regs;
	ulong start;

	debug("I2c Send Start bit.\n");
	writel(I2C_IPD_ALL_CLEAN, &regs->ipd);

	writel(I2C_STARTIEN, &regs->ien);
	writel(I2C_CON_EN | I2C_CON_START | i2c->cfg | con, &regs->con);

	start = get_timer(0);
	while (1) {
		if (readl(&regs->ipd) & I2C_STARTIPD) {
			writel(I2C_STARTIPD, &regs->ipd);
			break;
		}
		if (get_timer(start) > I2C_TIMEOUT_MS) {
			debug("I2C Send Start Bit Timeout\n");
			rk_i2c_show_regs(regs);
			return -ETIMEDOUT;
		}
		udelay(1);
	}

	/* clean start bit */
	writel(I2C_CON_EN | i2c->cfg | con, &regs->con);

	return 0;
}

static int rk_i2c_send_stop_bit(struct rk_i2c *i2c)
{
	struct i2c_regs *regs = i2c->regs;
	ulong start;

	debug("I2c Send Stop bit.\n");
	writel(I2C_IPD_ALL_CLEAN, &regs->ipd);

	writel(I2C_CON_EN | i2c->cfg | I2C_CON_STOP, &regs->con);
	writel(I2C_CON_STOP, &regs->ien);

	start = get_timer(0);
	while (1) {
		if (readl(&regs->ipd) & I2C_STOPIPD) {
			writel(I2C_STOPIPD, &regs->ipd);
			break;
		}
		if (get_timer(start) > I2C_TIMEOUT_MS) {
			debug("I2C Send Start Bit Timeout\n");
			rk_i2c_show_regs(regs);
			return -ETIMEDOUT;
		}
		udelay(1);
	}

	udelay(1);
	return 0;
}

static inline void rk_i2c_disable(struct rk_i2c *i2c)
{
	writel(0, &i2c->regs->ien);
	writel(I2C_IPD_ALL_CLEAN, &i2c->regs->ipd);
	writel(0, &i2c->regs->con);
}

static int rk_i2c_read(struct rk_i2c *i2c, uchar chip, uint reg, uint r_len,
		       uchar *buf, uint b_len, bool snd)
{
	struct i2c_regs *regs = i2c->regs;
	uchar *pbuf = buf;
	uint bytes_remain_len = b_len;
	uint bytes_xferred = 0;
	uint words_xferred = 0;
	ulong start;
	uint con = 0;
	uint rxdata;
	uint i, j;
	int err = 0;
	bool snd_chunk = false;

	debug("rk_i2c_read: chip = %d, reg = %d, r_len = %d, b_len = %d\n",
	      chip, reg, r_len, b_len);

	/* If the second message for TRX read, resetting internal state. */
	if (snd)
		writel(0, &regs->con);

	writel(I2C_MRXADDR_SET(1, chip << 1 | 1), &regs->mrxaddr);
	if (r_len == 0) {
		writel(0, &regs->mrxraddr);
	} else if (r_len < 4) {
		writel(I2C_MRXRADDR_SET(r_len, reg), &regs->mrxraddr);
	} else {
		debug("I2C Read: addr len %d not supported\n", r_len);
		return -EIO;
	}

	while (bytes_remain_len) {
		if (bytes_remain_len > RK_I2C_FIFO_SIZE) {
			con = I2C_CON_EN;
			bytes_xferred = 32;
		} else {
			/*
			 * The hw can read up to 32 bytes at a time. If we need
			 * more than one chunk, send an ACK after the last byte.
			 */
			con = I2C_CON_EN | I2C_CON_LASTACK;
			bytes_xferred = bytes_remain_len;
		}
		words_xferred = DIV_ROUND_UP(bytes_xferred, 4);

		/*
		 * make sure we are in plain RX mode if we read a second chunk;
		 * and first rx read need to send start bit.
		 */
		if (snd_chunk) {
			con |= I2C_CON_MOD(I2C_MODE_RX);
			writel(con | i2c->cfg, &regs->con);
		} else {
			con |= I2C_CON_MOD(I2C_MODE_TRX);
			err = rk_i2c_send_start_bit(i2c, con);
			if (err)
				return err;
		}

		writel(I2C_MBRFIEN | I2C_NAKRCVIEN, &regs->ien);
		writel(bytes_xferred, &regs->mrxcnt);

		start = get_timer(0);
		while (1) {
			if (readl(&regs->ipd) & I2C_NAKRCVIPD) {
				writel(I2C_NAKRCVIPD, &regs->ipd);
				err = -EREMOTEIO;
				goto i2c_exit;
			}
			if (readl(&regs->ipd) & I2C_MBRFIPD) {
				writel(I2C_MBRFIPD, &regs->ipd);
				break;
			}
			if (get_timer(start) > I2C_TIMEOUT_MS) {
				debug("I2C Read Data Timeout\n");
				err =  -ETIMEDOUT;
				rk_i2c_show_regs(regs);
				goto i2c_exit;
			}
			udelay(1);
		}

		for (i = 0; i < words_xferred; i++) {
			rxdata = readl(&regs->rxdata[i]);
			debug("I2c Read RXDATA[%d] = 0x%x\n", i, rxdata);
			for (j = 0; j < 4; j++) {
				if ((i * 4 + j) == bytes_xferred)
					break;
				*pbuf++ = (rxdata >> (j * 8)) & 0xff;
			}
		}

		bytes_remain_len -= bytes_xferred;
		snd_chunk = true;
		debug("I2C Read bytes_remain_len %d\n", bytes_remain_len);
	}

i2c_exit:
	return err;
}

static int rk_i2c_write(struct rk_i2c *i2c, uchar chip, uint reg, uint r_len,
			uchar *buf, uint b_len)
{
	struct i2c_regs *regs = i2c->regs;
	int err = 0;
	uchar *pbuf = buf;
	uint bytes_remain_len = b_len + r_len + 1;
	uint bytes_xferred = 0;
	uint words_xferred = 0;
	bool next = false;
	ulong start;
	uint txdata;
	uint i, j;

	debug("rk_i2c_write: chip = %d, reg = %d, r_len = %d, b_len = %d\n",
	      chip, reg, r_len, b_len);

	while (bytes_remain_len) {
		if (bytes_remain_len > RK_I2C_FIFO_SIZE)
			bytes_xferred = RK_I2C_FIFO_SIZE;
		else
			bytes_xferred = bytes_remain_len;
		words_xferred = DIV_ROUND_UP(bytes_xferred, 4);

		for (i = 0; i < words_xferred; i++) {
			txdata = 0;
			for (j = 0; j < 4; j++) {
				if ((i * 4 + j) == bytes_xferred)
					break;

				if (i == 0 && j == 0 && pbuf == buf) {
					txdata |= (chip << 1);
				} else if (i == 0 && j <= r_len && pbuf == buf) {
					txdata |= (reg &
						(0xff << ((j - 1) * 8))) << 8;
				} else {
					txdata |= (*pbuf++)<<(j * 8);
				}
			}
			writel(txdata, &regs->txdata[i]);
			debug("I2c Write TXDATA[%d] = 0x%08x\n", i, txdata);
		}

		/* If the write is the first, need to send start bit */
		if (!next) {
			err = rk_i2c_send_start_bit(i2c, I2C_CON_EN |
					   I2C_CON_MOD(I2C_MODE_TX));
			if (err)
				return err;
			next = true;
		} else {
			writel(I2C_CON_EN | I2C_CON_MOD(I2C_MODE_TX) | i2c->cfg,
			       &regs->con);
		}
		writel(I2C_MBTFIEN | I2C_NAKRCVIEN, &regs->ien);
		writel(bytes_xferred, &regs->mtxcnt);

		start = get_timer(0);
		while (1) {
			if (readl(&regs->ipd) & I2C_NAKRCVIPD) {
				writel(I2C_NAKRCVIPD, &regs->ipd);
				err = -EREMOTEIO;
				goto i2c_exit;
			}
			if (readl(&regs->ipd) & I2C_MBTFIPD) {
				writel(I2C_MBTFIPD, &regs->ipd);
				break;
			}
			if (get_timer(start) > I2C_TIMEOUT_MS) {
				debug("I2C Write Data Timeout\n");
				err =  -ETIMEDOUT;
				rk_i2c_show_regs(regs);
				goto i2c_exit;
			}
			udelay(1);
		}

		bytes_remain_len -= bytes_xferred;
		debug("I2C Write bytes_remain_len %d\n", bytes_remain_len);
	}

i2c_exit:
	return err;
}

static int rockchip_i2c_xfer(struct udevice *bus, struct i2c_msg *msg,
			     int nmsgs)
{
	struct rk_i2c *i2c = dev_get_priv(bus);
	bool snd = false; /* second message for TRX read */
	int ret;
#ifdef CONFIG_IRQ
	ulong flags;
#endif

	debug("i2c_xfer: %d messages\n", nmsgs);
	if (nmsgs > 2 || ((nmsgs == 2) && (msg->flags & I2C_M_RD))) {
		debug("Not support more messages now, split them\n");
		return -EINVAL;
	}

#ifdef CONFIG_IRQ
	local_irq_save(flags);
#endif
	/* Nack enabled */
	i2c->cfg |= I2C_CON_ACTACK;
	for (; nmsgs > 0; nmsgs--, msg++) {
		debug("i2c_xfer: chip=0x%x, len=0x%x\n", msg->addr, msg->len);

		if (msg->flags & I2C_M_RD) {
			/* If snd is true, it is TRX mode. */
			ret = rk_i2c_read(i2c, msg->addr, 0, 0, msg->buf,
					  msg->len, snd);
		} else {
			snd = true;
			ret = rk_i2c_write(i2c, msg->addr, 0, 0, msg->buf,
					   msg->len);
		}

		if (ret) {
			debug("i2c_write: error sending\n");
			goto exit;
		}
	}

exit:
	rk_i2c_send_stop_bit(i2c);
	rk_i2c_disable(i2c);
#ifdef CONFIG_IRQ
	local_irq_restore(flags);
#endif
	return ret;
}

static unsigned int rk3x_i2c_get_version(struct rk_i2c *i2c)
{
	struct i2c_regs *regs = i2c->regs;
	uint version;

	version = readl(&regs->con) & I2C_CON_VERSION;

	return version >>= I2C_CON_VERSION_SHIFT;
}

int rockchip_i2c_set_bus_speed(struct udevice *bus, unsigned int speed)
{
	struct rk_i2c *i2c = dev_get_priv(bus);

	if (rk3x_i2c_get_version(i2c) >= RK_I2C_VERSION1)
		rk_i2c_adapter_clk(i2c, speed);
	else
		rk_i2c_set_clk(i2c, speed);

	return 0;
}

static int rockchip_i2c_ofdata_to_platdata(struct udevice *bus)
{
	struct rk_i2c *priv = dev_get_priv(bus);
	int ret;

	ret = clk_get_by_index(bus, 0, &priv->clk);
	if (ret < 0) {
		debug("%s: Could not get clock for %s: %d\n", __func__,
		      bus->name, ret);
		return ret;
	}

	return 0;
}

static int rockchip_i2c_probe(struct udevice *bus)
{
	struct rk_i2c *priv = dev_get_priv(bus);

	priv->regs = dev_read_addr_ptr(bus);

	return 0;
}

static const struct dm_i2c_ops rockchip_i2c_ops = {
	.xfer		= rockchip_i2c_xfer,
	.set_bus_speed	= rockchip_i2c_set_bus_speed,
};

static const struct udevice_id rockchip_i2c_ids[] = {
	{ .compatible = "rockchip,rk3066-i2c" },
	{ .compatible = "rockchip,rk3188-i2c" },
	{ .compatible = "rockchip,rk3288-i2c" },
	{ .compatible = "rockchip,rk3328-i2c" },
	{ .compatible = "rockchip,rk3399-i2c" },
	{ .compatible = "rockchip,rk3228-i2c" },
	{ .compatible = "rockchip,rv1108-i2c" },
	{ }
};

U_BOOT_DRIVER(i2c_rockchip) = {
	.name	= "i2c_rockchip",
	.id	= UCLASS_I2C,
	.of_match = rockchip_i2c_ids,
	.ofdata_to_platdata = rockchip_i2c_ofdata_to_platdata,
	.probe	= rockchip_i2c_probe,
	.priv_auto_alloc_size = sizeof(struct rk_i2c),
	.ops	= &rockchip_i2c_ops,
};
