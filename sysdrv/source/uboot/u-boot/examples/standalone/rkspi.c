/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <exports.h>

#include "rkspi.h"

/* Change to 1 to output registers at the start of each transaction */
#define DEBUG_RK_SPI	0

struct rockchip_spi_priv {
	struct rockchip_spi *regs;
	unsigned int mode;
	u8 bits_per_word;		/* max 16 bits per word */
	u8 n_bytes;
	unsigned int clock_div;
	uint cr0;
	u32 rsd;			/* Rx sample delay cycles */
};

#define RK_SPI_BUS_MAX 5
static struct rockchip_spi_priv spi_bus[RK_SPI_BUS_MAX];

static inline struct rockchip_spi_priv *get_spi_bus(u8 bus)
{
	return &spi_bus[bus];
}

#define SPI_FIFO_DEPTH		32
#define SPI_CR0_RSD_MAX		0x3

static inline void writel(u32 val, void *addr)
{
	*(volatile u32 *)addr = val;
}

static inline u32 readl(void *addr)
{
	return *(volatile u32 *)addr;
}

static void rkspi_dump_regs(struct rockchip_spi *regs)
{
	debug("ctrl0: \t\t0x%08x\n", readl(&regs->ctrlr0));
	debug("ctrl1: \t\t0x%08x\n", readl(&regs->ctrlr1));
	debug("ssienr: \t\t0x%08x\n", readl(&regs->enr));
	debug("ser: \t\t0x%08x\n", readl(&regs->ser));
	debug("baudr: \t\t0x%08x\n", readl(&regs->baudr));
	debug("txftlr: \t\t0x%08x\n", readl(&regs->txftlr));
	debug("rxftlr: \t\t0x%08x\n", readl(&regs->rxftlr));
	debug("txflr: \t\t0x%08x\n", readl(&regs->txflr));
	debug("rxflr: \t\t0x%08x\n", readl(&regs->rxflr));
	debug("sr: \t\t0x%08x\n", readl(&regs->sr));
	debug("imr: \t\t0x%08x\n", readl(&regs->imr));
	debug("isr: \t\t0x%08x\n", readl(&regs->isr));
	debug("dmacr: \t\t0x%08x\n", readl(&regs->dmacr));
	debug("dmatdlr: \t0x%08x\n", readl(&regs->dmatdlr));
	debug("dmardlr: \t0x%08x\n", readl(&regs->dmardlr));
}

static void rkspi_enable_chip(struct rockchip_spi *regs, bool enable)
{
	writel(enable ? 1 : 0, &regs->enr);
}

static void rkspi_set_baudr(struct rockchip_spi_priv *priv, uint clk_div)
{
	writel(clk_div, &priv->regs->baudr);
}

static int rkspi_wait_till_not_busy(struct rockchip_spi *regs)
{
	unsigned long start;

	start = get_timer(0);
	while (readl(&regs->sr) & SR_BUSY) {
		if (get_timer(start) > ROCKCHIP_SPI_TIMEOUT_MS) {
			debug("RK SPI: Status keeps busy for 1000us after a read/write!\n");
			return -ETIMEDOUT;
		}
	}

	return 0;
}

static void spi_cs_activate(u8 bus, u8 cs)
{
	struct rockchip_spi_priv *priv = get_spi_bus(bus);
	struct rockchip_spi *regs = priv->regs;

	debug("activate cs%u\n", cs);
	writel(1 << cs, &regs->ser);
}

static void spi_cs_deactivate(u8 bus, u8 cs)
{
	struct rockchip_spi_priv *priv = get_spi_bus(bus);
	struct rockchip_spi *regs = priv->regs;

	debug("deactivate cs%u\n", cs);
	writel(0, &regs->ser);
}

int rockchip_spi_probe(u8 bus, uintptr_t base_addr, u32 rsd, u32 clock_div, u32 mode)
{
	struct rockchip_spi_priv *priv;

	if (bus >= RK_SPI_BUS_MAX) {
		printf("%s bus %d is out of max num(%d)\n", __func__, bus, RK_SPI_BUS_MAX);
		return -1;
	}

	debug("%s: probe\n", __func__);

	priv = &spi_bus[bus];

	priv->regs = (struct rockchip_spi *)base_addr;
	priv->rsd = rsd;
	priv->mode = mode;
	if (clock_div % 2 || clock_div < 2) {
		printf("%s div should be even num, and at least 2\n", __func__);

		return -1;
	}
	priv->clock_div = clock_div;
	priv->bits_per_word = 8;

	return 0;
}

int rockchip_spi_claim_bus(u8 bus)
{
	struct rockchip_spi_priv *priv = get_spi_bus(bus);
	struct rockchip_spi *regs = priv->regs;
	u8 spi_dfs, spi_tf;
	uint ctrlr0;

	/* Disable the SPI hardware */
	rkspi_enable_chip(regs, 0);

	switch (priv->bits_per_word) {
	case 8:
		priv->n_bytes = 1;
		spi_dfs = DFS_8BIT;
		spi_tf = HALF_WORD_OFF;
		break;
	case 16:
		priv->n_bytes = 2;
		spi_dfs = DFS_16BIT;
		spi_tf = HALF_WORD_ON;
		break;
	default:
		debug("%s: unsupported bits: %dbits\n", __func__,
		      priv->bits_per_word);
		return -EPROTONOSUPPORT;
	}

	rkspi_set_baudr(priv, priv->clock_div);

	/* Operation Mode */
	ctrlr0 = OMOD_MASTER << OMOD_SHIFT;

	/* Data Frame Size */
	ctrlr0 |= spi_dfs << DFS_SHIFT;

	/* set SPI mode 0..3 */
	if (priv->mode & SPI_CPOL)
		ctrlr0 |= SCOL_HIGH << SCOL_SHIFT;
	if (priv->mode & SPI_CPHA)
		ctrlr0 |= SCPH_TOGSTA << SCPH_SHIFT;

	/* Chip Select Mode */
	ctrlr0 |= CSM_KEEP << CSM_SHIFT;

	/* SSN to Sclk_out delay */
	ctrlr0 |= SSN_DELAY_ONE << SSN_DELAY_SHIFT;

	/* Serial Endian Mode */
	ctrlr0 |= SEM_LITTLE << SEM_SHIFT;

	/* First Bit Mode */
	ctrlr0 |= FBM_MSB << FBM_SHIFT;

	/* Byte and Halfword Transform */
	ctrlr0 |= spi_tf << HALF_WORD_TX_SHIFT;

	/* Rxd Sample Delay */
	ctrlr0 |= priv->rsd << RXDSD_SHIFT;

	/* Frame Format */
	ctrlr0 |= FRF_SPI << FRF_SHIFT;

	/* Save static configuration */
	priv->cr0 = ctrlr0;

	writel(ctrlr0, &regs->ctrlr0);

	return 0;
}

int rockchip_spi_config(struct rockchip_spi_priv *priv, const void *dout)
{
	struct rockchip_spi *regs = priv->regs;
	uint ctrlr0 = priv->cr0;
	u32 tmod;

	if (dout)
		tmod = TMOD_TR;
	else
		tmod = TMOD_RO;

	ctrlr0 |= (tmod & TMOD_MASK) << TMOD_SHIFT;
	writel(ctrlr0, &regs->ctrlr0);

	return 0;
}

void rockchip_spi_release_bus(u8 bus)
{
	struct rockchip_spi_priv *priv = get_spi_bus(bus);

	rkspi_enable_chip(priv->regs, false);
}

int rockchip_spi_xfer(u8 bus, u8 cs, unsigned int bitlen, const void *dout, void *din, unsigned long flags)
{
	struct rockchip_spi_priv *priv = get_spi_bus(bus);
	struct rockchip_spi *regs = priv->regs;
	int len = bitlen >> 3;
	const u8 *out = dout;
	u8 *in = din;
	int toread, towrite;
	int ret;

	rockchip_spi_config(priv, dout);

	debug("%s: dout=%p, din=%p, len=%x, flags=%lx\n", __func__, dout, din,
	      len, flags);
	if (DEBUG_RK_SPI)
		rkspi_dump_regs(regs);

	/* Assert CS before transfer */
	if (flags & SPI_XFER_BEGIN)
		spi_cs_activate(bus, cs);

	while (len > 0) {
		int todo = min(len, 0xffff);

		rkspi_enable_chip(regs, false);
		writel(todo - 1, &regs->ctrlr1);
		rkspi_enable_chip(regs, true);

		toread = todo;
		towrite = todo;
		while (toread || towrite) {
			u32 status = readl(&regs->sr);

			if (towrite && !(status & SR_TF_FULL)) {
				if (out)
					writel(out ? *out++ : 0, regs->txdr);
				towrite--;
			}
			if (toread && !(status & SR_RF_EMPT)) {
				u32 byte = readl(regs->rxdr);

				if (in)
					*in++ = byte;
				toread--;
			}
		}
		ret = rkspi_wait_till_not_busy(regs);
		if (ret)
			break;
		len -= todo;
	}

	/* Deassert CS after transfer */
	if (flags & SPI_XFER_END)
		spi_cs_deactivate(bus, cs);

	rkspi_enable_chip(regs, false);

	return ret;
}

int rockchip_spi_write_then_read(u8 bus, u8 cs, const u8 *opcode,
				 size_t n_opcode, const u8 *txbuf, u8 *rxbuf,
			size_t n_buf)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (n_buf == 0)
		flags |= SPI_XFER_END;

	ret = rockchip_spi_xfer(bus, cs, n_opcode * 8, opcode, NULL, flags);
	if (ret) {
		debug("spi: failed to send command (%zu bytes): %d\n",
		      n_opcode, ret);
	} else if (n_buf != 0) {
		ret = rockchip_spi_xfer(bus, cs, n_buf * 8, txbuf, rxbuf, SPI_XFER_END);
		if (ret)
			debug("spi: failed to transfer %zu bytes of data: %d\n",
			      n_buf, ret);
	}

	return ret;
}

void dbg_print_hex(char *s, void *buf, u32 width, u32 len)
{
	u32 i, j;
	unsigned char *p8 = (unsigned char *)buf;
	unsigned short *p16 = (unsigned short *)buf;
	u32 *p32 = (u32 *)buf;

	j = 0;

	for (i = 0; i < len; i++) {
		if (j == 0)
			printf("%s %p + 0x%x:", s, buf, i * width);

		if (width == 4)
			printf("0x%08x,", p32[i]);
		else if (width == 2)
			printf("0x%04x,", p16[i]);
		else
			printf("0x%02x,", p8[i]);

		if (++j >= (16 / width)) {
			j = 0;
			printf("\n");
		}
	}

	printf("\n");
}

static void spi_hw_init(void)
{
	/* Setting the clock and iomux */
	/* todo */
}

int main(int argc, char * const argv[])
{
	int i;

	/* Print the ABI version */
	app_startup(argv);
	printf("Example expects ABI version %d\n", XF_VERSION);
	printf("Actual U-Boot ABI version %d\n", (int)get_version());

	printf("rk_spi standalone version\n");

	printf("argc = %d\n", argc);

	for (i = 0; i <= argc; ++i) {
		printf("argv[%d] = \"%s\"\n",
		       i,
			argv[i] ? argv[i] : "<NULL>");
	}

	/*
	 * spi test demo
	 */
#if 1
	u8 bus, cs;
	unsigned char *pread, *pwrite;
	u32 test_size = 0x100;
	int ret;

	pread = malloc(test_size);
	if (!pread)
		printf("%s pread malloc fail\n", __func__);
	pwrite = malloc(test_size);
	if (!pwrite) {
		printf("%s pwrite malloc fail\n", __func__);
		free(pwrite);

		return -1;
	}

	for (i = 0; i < test_size; i++)
		pwrite[i] = i;

	bus = 4;
	cs = 0;

	spi_hw_init();

	rockchip_spi_probe(bus, 0xfecb0000, 0, 20, SPI_MODE_3);
	rockchip_spi_claim_bus(bus);

	/*
	 * SPI write
	 */
	ret = rockchip_spi_xfer(bus, cs, test_size * 8, pwrite, NULL, SPI_XFER_ONCE);
	if (ret) {
		printf("rockchip_spi_xfer fail ret=%d\n", ret);

		return ret;
	}

	/*
	 * SPI read
	 */
	rockchip_spi_xfer(bus, cs, test_size * 8, NULL, pread, SPI_XFER_ONCE);
	if (ret) {
		printf("rockchip_spi_xfer fail ret=%d\n", ret);

		return ret;
	}
	dbg_print_hex("spi_read:", pread, 4, test_size / 4);

	/*
	 * SPI duplex
	 */
	rockchip_spi_xfer(bus, cs, test_size * 8, pwrite, pread, SPI_XFER_ONCE);
	if (ret) {
		printf("rockchip_spi_xfer fail ret=%d\n", ret);

		return ret;
	}
	dbg_print_hex("spi_duplex:", pread, 4, test_size / 4);

	/*
	 * SPI write then read
	 */
	rockchip_spi_write_then_read(bus, cs, pwrite, 1, NULL, pread, test_size);
	if (ret) {
		printf("rockchip_spi_xfer fail ret=%d\n", ret);

		return ret;
	}
	dbg_print_hex("spi_write_then_read:", pread, 4, test_size / 4);

	rockchip_spi_release_bus(bus);
#endif

	printf("Hit any key to exit ... ");
	while (!tstc())
		;
	/* consume input */
	(void)getc();

	printf("\n\n");

	return 0;
}
