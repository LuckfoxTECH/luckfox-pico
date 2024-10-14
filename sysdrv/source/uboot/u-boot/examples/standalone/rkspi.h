/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _DRIVER_SPI_H_
#define _DRIVER_SPI_H_

struct rockchip_spi {
	u32 ctrlr0;
	u32 ctrlr1;
	u32 enr;
	u32 ser;
	u32 baudr;
	u32 txftlr;
	u32 rxftlr;
	u32 txflr;
	u32 rxflr;
	u32 sr;
	u32 ipr;
	u32 imr;
	u32 isr;
	u32 risr;
	u32 icr;
	u32 dmacr;
	u32 dmatdlr;
	u32 dmardlr;		/* 0x44 */
	u32 reserved[0xef];
	u32 txdr[0x100];	/* 0x400 */
	u32 rxdr[0x100];	/* 0x800 */
};

/* CTRLR0 */
enum {
	DFS_SHIFT	= 0,	/* Data Frame Size */
	DFS_MASK	= 3,
	DFS_4BIT	= 0,
	DFS_8BIT,
	DFS_16BIT,
	DFS_RESV,

	CFS_SHIFT	= 2,	/* Control Frame Size */
	CFS_MASK	= 0xf,

	SCPH_SHIFT	= 6,	/* Serial Clock Phase */
	SCPH_MASK	= 1,
	SCPH_TOGMID	= 0,	/* SCLK toggles in middle of first data bit */
	SCPH_TOGSTA,		/* SCLK toggles at start of first data bit */

	SCOL_SHIFT	= 7,	/* Serial Clock Polarity */
	SCOL_MASK	= 1,
	SCOL_LOW	= 0,	/* Inactive state of serial clock is low */
	SCOL_HIGH,		/* Inactive state of serial clock is high */

	CSM_SHIFT	= 8,	/* Chip Select Mode */
	CSM_MASK	= 0x3,
	CSM_KEEP	= 0,	/* ss_n stays low after each frame  */
	CSM_HALF,		/* ss_n high for half sclk_out cycles */
	CSM_ONE,		/* ss_n high for one sclk_out cycle */
	CSM_RESV,

	SSN_DELAY_SHIFT	= 10,	/* SSN to Sclk_out delay */
	SSN_DELAY_MASK	= 1,
	SSN_DELAY_HALF	= 0,	/* 1/2 sclk_out cycle */
	SSN_DELAY_ONE	= 1,	/* 1 sclk_out cycle */

	SEM_SHIFT	= 11,	/* Serial Endian Mode */
	SEM_MASK	= 1,
	SEM_LITTLE	= 0,	/* little endian */
	SEM_BIG,		/* big endian */

	FBM_SHIFT	= 12,	/* First Bit Mode */
	FBM_MASK	= 1,
	FBM_MSB		= 0,	/* first bit is MSB */
	FBM_LSB,		/* first bit in LSB */

	HALF_WORD_TX_SHIFT = 13,	/* Byte and Halfword Transform */
	HALF_WORD_MASK	= 1,
	HALF_WORD_ON	= 0,	/* apb 16bit write/read, spi 8bit write/read */
	HALF_WORD_OFF,		/* apb 8bit write/read, spi 8bit write/read */

	RXDSD_SHIFT	= 14,	/* Rxd Sample Delay, in cycles */
	RXDSD_MASK	= 3,

	FRF_SHIFT	= 16,	/* Frame Format */
	FRF_MASK	= 3,
	FRF_SPI		= 0,	/* Motorola SPI */
	FRF_SSP,			/* Texas Instruments SSP*/
	FRF_MICROWIRE,		/* National Semiconductors Microwire */
	FRF_RESV,

	TMOD_SHIFT	= 18,	/* Transfer Mode */
	TMOD_MASK	= 3,
	TMOD_TR		= 0,	/* xmit & recv */
	TMOD_TO,		/* xmit only */
	TMOD_RO,		/* recv only */
	TMOD_RESV,

	OMOD_SHIFT	= 20,	/* Operation Mode */
	OMOD_MASK	= 1,
	OMOD_MASTER	= 0,	/* Master Mode */
	OMOD_SLAVE,		/* Slave Mode */
};

/* SR */
enum {
	SR_MASK		= 0x7f,
	SR_BUSY		= 1 << 0,
	SR_TF_FULL	= 1 << 1,
	SR_TF_EMPT	= 1 << 2,
	SR_RF_EMPT	= 1 << 3,
	SR_RF_FULL	= 1 << 4,
};

#define ROCKCHIP_SPI_TIMEOUT_MS		1000

#define SPI_XFER_BEGIN		BIT(0)	/* Assert CS before transfer */
#define SPI_XFER_END		BIT(1)	/* Deassert CS after transfer */
#define SPI_XFER_ONCE		(SPI_XFER_BEGIN | SPI_XFER_END)

/* SPI mode flags */
#define SPI_CPHA	BIT(0)			/* clock phase */
#define SPI_CPOL	BIT(1)			/* clock polarity */
#define SPI_MODE_0	(0 | 0)			/* (original MicroWire) */
#define SPI_MODE_1	(0 | SPI_CPHA)
#define SPI_MODE_2	(SPI_CPOL | 0)
#define SPI_MODE_3	(SPI_CPOL | SPI_CPHA)

int rockchip_spi_probe(u8 bus, uintptr_t base_addr, u32 rsd, u32 clock_div, u32 mode);
int rockchip_spi_claim_bus(u8 bus);
void rockchip_spi_release_bus(u8 bus);
int rockchip_spi_xfer(u8 bus, u8 cs, unsigned int bitlen, const void *dout, void *din, unsigned long flags);
int rockchip_spi_write_then_read(u8 bus, u8 cs,
				 const u8 *opcode, size_t n_opcode,
				 const u8 *txbuf, u8 *rxbuf, size_t n_buf);

#endif
