/*
 * (C) Copyright 2012 SAMSUNG Electronics
 * Jaehoon Chung <jh80.chung@samsung.com>
 * Rajeshawari Shinde <rajeshwari.s@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <bouncebuf.h>
#include <errno.h>
#include <malloc.h>
#include <memalign.h>
#include <mmc.h>
#include <dwmmc.h>
#ifdef CONFIG_DM_GPIO
#include <asm/gpio.h>
#include <asm-generic/gpio.h>
#endif

#define PAGE_SIZE 4096

/*
 * Currently it supports read/write up to 8*8*4 Bytes per
 * stride as a burst mode. Please note that if you change
 * MAX_STRIDE, you should also update dwmci_memcpy_fromio
 * to augment the groups of {ldm, stm}.
 */
#define MAX_STRIDE 64
#if (CONFIG_ARM && CONFIG_CPU_V7 && !defined(CONFIG_MMC_SIMPLE))
void noinline dwmci_memcpy_fromio(void *buffer, void *fifo_addr)
{
	__asm__ __volatile__ (
		"push {r2, r3, r4, r5, r6, r7, r8, r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"pop {r2, r3, r4, r5, r6,r7,r8,r9}\n"
		:::"memory"
	);
}

void noinline dwmci_memcpy_toio(void *buffer, void *fifo_addr)
{
	__asm__ __volatile__ (
		"push {r2, r3, r4, r5, r6, r7, r8, r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"ldm r0!, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"stm r1, {r2,r3,r4,r5,r6,r7,r8,r9}\n"
		"pop {r2, r3, r4, r5, r6,r7,r8,r9}\n"
		:::"memory"
	);
}
#else
void dwmci_memcpy_fromio(void *buffer, void *fifo_addr) {};
void dwmci_memcpy_toio(void *buffer, void *fifo_addr) {};
#endif

static int dwmci_wait_reset(struct dwmci_host *host, u32 value)
{
	unsigned long timeout = 1000;
	u32 ctrl;

	dwmci_writel(host, DWMCI_CTRL, value);

	while (timeout--) {
		ctrl = dwmci_readl(host, DWMCI_CTRL);
		if (!(ctrl & DWMCI_RESET_ALL))
			return 1;
	}
	return 0;
}

static void dwmci_set_idma_desc(struct dwmci_idmac *idmac,
		u32 desc0, u32 desc1, u32 desc2)
{
	struct dwmci_idmac *desc = idmac;

	desc->flags = desc0;
	desc->cnt = desc1;
	desc->addr = desc2;
	desc->next_addr = (ulong)desc + sizeof(struct dwmci_idmac);
}

static void dwmci_prepare_data(struct dwmci_host *host,
			       struct mmc_data *data,
			       struct dwmci_idmac *cur_idmac,
			       void *bounce_buffer)
{
	unsigned long ctrl;
	unsigned int i = 0, flags, cnt, blk_cnt;
	ulong data_start, data_end;


	blk_cnt = data->blocks;

	dwmci_wait_reset(host, DWMCI_CTRL_FIFO_RESET);

	data_start = (ulong)cur_idmac;
	dwmci_writel(host, DWMCI_DBADDR, (ulong)cur_idmac);

	do {
		flags = DWMCI_IDMAC_OWN | DWMCI_IDMAC_CH ;
		flags |= (i == 0) ? DWMCI_IDMAC_FS : 0;
		if (blk_cnt <= 8) {
			flags |= DWMCI_IDMAC_LD;
			cnt = data->blocksize * blk_cnt;
		} else
			cnt = data->blocksize * 8;

		dwmci_set_idma_desc(cur_idmac, flags, cnt,
				    (ulong)bounce_buffer + (i * PAGE_SIZE));

		if (blk_cnt <= 8)
			break;
		blk_cnt -= 8;
		cur_idmac++;
		i++;
	} while(1);

	data_end = (ulong)cur_idmac;
	flush_dcache_range(data_start, data_end + ARCH_DMA_MINALIGN);

	ctrl = dwmci_readl(host, DWMCI_CTRL);
	ctrl |= DWMCI_IDMAC_EN | DWMCI_DMA_EN;
	dwmci_writel(host, DWMCI_CTRL, ctrl);

	ctrl = dwmci_readl(host, DWMCI_BMOD);
	ctrl |= DWMCI_BMOD_IDMAC_FB | DWMCI_BMOD_IDMAC_EN;
	dwmci_writel(host, DWMCI_BMOD, ctrl);

	dwmci_writel(host, DWMCI_BLKSIZ, data->blocksize);
	dwmci_writel(host, DWMCI_BYTCNT, data->blocksize * data->blocks);
}

static unsigned int dwmci_get_timeout(struct mmc *mmc, const unsigned int size)
{
	unsigned int timeout;

	timeout = size * 8;	/* counting in bits */
	timeout *= 10;		/* wait 10 times as long */
	timeout /= mmc->clock;
	timeout /= mmc->bus_width;
	timeout *= 1000;	/* counting in msec */
	timeout = (timeout < 10000) ? 10000 : timeout;

	return timeout;
}

static int dwmci_data_transfer(struct dwmci_host *host, struct mmc_data *data)
{
	int ret = 0;
	int reset_timeout = 100;
	u32 timeout, status, ctrl, mask, size, i, len = 0;
	u32 *buf = NULL;
	ulong start = get_timer(0);
	u32 fifo_depth = (((host->fifoth_val & RX_WMARK_MASK) >>
			    RX_WMARK_SHIFT) + 1) * 2;
	bool stride;

	size = data->blocksize * data->blocks;
	/* Still use legacy PIO mode if size < 512(128 * 4) Bytes */
	stride = host->stride_pio && size > 128;
	if (data->flags == MMC_DATA_READ)
		buf = (unsigned int *)data->dest;
	else
		buf = (unsigned int *)data->src;

	timeout = dwmci_get_timeout(host->mmc, size);
	size /= 4;

	for (;;) {
		mask = dwmci_readl(host, DWMCI_RINTSTS);
		/* Error during data transfer. */
		if (mask & (DWMCI_DATA_ERR | DWMCI_DATA_TOUT)) {
			debug("%s: DATA ERROR!\n", __func__);
			dwmci_wait_reset(host, DWMCI_RESET_ALL);
			dwmci_writel(host, DWMCI_CMD, DWMCI_CMD_PRV_DAT_WAIT |
				     DWMCI_CMD_UPD_CLK | DWMCI_CMD_START);

			do {
				status = dwmci_readl(host, DWMCI_CMD);
				if (reset_timeout-- < 0)
					break;
				udelay(100);
			} while (status & DWMCI_CMD_START);

			if (!host->fifo_mode) {
				ctrl = dwmci_readl(host, DWMCI_BMOD);
				ctrl |= DWMCI_BMOD_IDMAC_RESET;
				dwmci_writel(host, DWMCI_BMOD, ctrl);
			}

			ret = -EINVAL;
			break;
		}

		if (host->fifo_mode && size) {
			len = 0;
			if (data->flags == MMC_DATA_READ &&
			    (mask & DWMCI_INTMSK_RXDR)) {
				while (size) {
					len = dwmci_readl(host, DWMCI_STATUS);
					len = (len >> DWMCI_FIFO_SHIFT) &
						    DWMCI_FIFO_MASK;
					len = min(size, len);
					if (!stride) {
						/* Legacy pio mode */
						for (i = 0; i < len; i++)
							*buf++ = dwmci_readl(host, DWMCI_DATA);
						goto read_again;
					}

					/* dwmci_memcpy_fromio now bursts 256 Bytes once */
					if (len < MAX_STRIDE)
						continue;

					for (i = 0; i < len / MAX_STRIDE; i++) {
						dwmci_memcpy_fromio(buf, host->ioaddr + DWMCI_DATA);
						buf += MAX_STRIDE;
					}

					len = i * MAX_STRIDE;
read_again:
					size = size > len ? (size - len) : 0;
				}
				dwmci_writel(host, DWMCI_RINTSTS,
					     DWMCI_INTMSK_RXDR);
			} else if (data->flags == MMC_DATA_WRITE &&
				   (mask & DWMCI_INTMSK_TXDR)) {
				while (size) {
					len = dwmci_readl(host, DWMCI_STATUS);
					len = fifo_depth - ((len >>
						   DWMCI_FIFO_SHIFT) &
						   DWMCI_FIFO_MASK);
					len = min(size, len);
					if (!stride) {
						for (i = 0; i < len; i++)
							dwmci_writel(host, DWMCI_DATA,
								     *buf++);
						goto write_again;
					}
					/* dwmci_memcpy_toio now bursts 256 Bytes once */
					if (len < MAX_STRIDE)
						continue;

					for (i = 0; i < len / MAX_STRIDE; i++) {
						dwmci_memcpy_toio(buf, host->ioaddr + DWMCI_DATA);
						buf += MAX_STRIDE;
					}

					len = i * MAX_STRIDE;
write_again:
					size = size > len ? (size - len) : 0;
				}
				dwmci_writel(host, DWMCI_RINTSTS,
					     DWMCI_INTMSK_TXDR);
			}
		}

		/* Data arrived correctly. */
		if (mask & DWMCI_INTMSK_DTO) {
			ret = 0;
			break;
		}

		/* Check for timeout. */
		if (get_timer(start) > timeout) {
			debug("%s: Timeout waiting for data!\n",
			      __func__);
			ret = -ETIMEDOUT;
			break;
		}
	}

	dwmci_writel(host, DWMCI_RINTSTS, mask);

	return ret;
}

static int dwmci_set_transfer_mode(struct dwmci_host *host,
		struct mmc_data *data)
{
	unsigned long mode;

	mode = DWMCI_CMD_DATA_EXP;
	if (data->flags & MMC_DATA_WRITE)
		mode |= DWMCI_CMD_RW;

	return mode;
}

#ifdef CONFIG_DM_MMC
static int dwmci_send_cmd(struct udevice *dev, struct mmc_cmd *cmd,
		   struct mmc_data *data)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
#else
static int dwmci_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
		struct mmc_data *data)
{
#endif
	struct dwmci_host *host = mmc->priv;
	ALLOC_CACHE_ALIGN_BUFFER(struct dwmci_idmac, cur_idmac,
				 data ? DIV_ROUND_UP(data->blocks, 8) : 0);
	int ret = 0, flags = 0, i;
	unsigned int timeout = 500;
	u32 retry = 100000;
	u32 mask, ctrl;
	ulong start = get_timer(0);
	struct bounce_buffer bbstate;

	while (dwmci_readl(host, DWMCI_STATUS) & DWMCI_BUSY) {
		if (get_timer(start) > timeout) {
			debug("%s: Timeout on data busy\n", __func__);
			return -ETIMEDOUT;
		}
	}

	dwmci_writel(host, DWMCI_RINTSTS, DWMCI_INTMSK_ALL);

	if (data) {
		if (host->fifo_mode) {
			dwmci_writel(host, DWMCI_BLKSIZ, data->blocksize);
			dwmci_writel(host, DWMCI_BYTCNT,
				     data->blocksize * data->blocks);
			dwmci_wait_reset(host, DWMCI_CTRL_FIFO_RESET);
		} else {
			if (data->flags == MMC_DATA_READ) {
				ret = bounce_buffer_start(&bbstate,
						(void*)data->dest,
						data->blocksize *
						data->blocks, GEN_BB_WRITE);
			} else {
				ret = bounce_buffer_start(&bbstate,
						(void*)data->src,
						data->blocksize *
						data->blocks, GEN_BB_READ);
			}

			if (ret)
				return ret;

			dwmci_prepare_data(host, data, cur_idmac,
					   bbstate.bounce_buffer);
		}
	}

	dwmci_writel(host, DWMCI_CMDARG, cmd->cmdarg);

	if (data)
		flags = dwmci_set_transfer_mode(host, data);

	if ((cmd->resp_type & MMC_RSP_136) && (cmd->resp_type & MMC_RSP_BUSY))
		return -1;

	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION)
		flags |= DWMCI_CMD_ABORT_STOP;
	else
		flags |= DWMCI_CMD_PRV_DAT_WAIT;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		flags |= DWMCI_CMD_RESP_EXP;
		if (cmd->resp_type & MMC_RSP_136)
			flags |= DWMCI_CMD_RESP_LENGTH;
	}

	if (cmd->resp_type & MMC_RSP_CRC)
		flags |= DWMCI_CMD_CHECK_CRC;

	flags |= (cmd->cmdidx | DWMCI_CMD_START | DWMCI_CMD_USE_HOLD_REG);

	debug("Sending CMD%d\n",cmd->cmdidx);

	dwmci_writel(host, DWMCI_CMD, flags);

	for (i = 0; i < retry; i++) {
		mask = dwmci_readl(host, DWMCI_RINTSTS);
		if (mask & DWMCI_INTMSK_CDONE) {
			if (!data)
				dwmci_writel(host, DWMCI_RINTSTS, mask);
			break;
		}
	}

	if (i == retry) {
		debug("%s: Timeout.\n", __func__);
		return -ETIMEDOUT;
	}

	if (mask & DWMCI_INTMSK_RTO) {
		/*
		 * Timeout here is not necessarily fatal. (e)MMC cards
		 * will splat here when they receive CMD55 as they do
		 * not support this command and that is exactly the way
		 * to tell them apart from SD cards. Thus, this output
		 * below shall be debug(). eMMC cards also do not favor
		 * CMD8, please keep that in mind.
		 */
		debug("%s: Response Timeout.\n", __func__);
		return -ETIMEDOUT;
	} else if (mask & DWMCI_INTMSK_RE) {
		debug("%s: Response Error.\n", __func__);
		return -EIO;
	}


	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			cmd->response[0] = dwmci_readl(host, DWMCI_RESP3);
			cmd->response[1] = dwmci_readl(host, DWMCI_RESP2);
			cmd->response[2] = dwmci_readl(host, DWMCI_RESP1);
			cmd->response[3] = dwmci_readl(host, DWMCI_RESP0);
		} else {
			cmd->response[0] = dwmci_readl(host, DWMCI_RESP0);
		}
	}

	if (data) {
		ret = dwmci_data_transfer(host, data);

		/* only dma mode need it */
		if (!host->fifo_mode) {
			ctrl = dwmci_readl(host, DWMCI_CTRL);
			ctrl &= ~(DWMCI_DMA_EN);
			dwmci_writel(host, DWMCI_CTRL, ctrl);
			bounce_buffer_stop(&bbstate);
		}
	}

	return ret;
}

#ifdef CONFIG_SPL_BLK_READ_PREPARE
#ifdef CONFIG_DM_MMC
static int dwmci_send_cmd_prepare(struct udevice *dev, struct mmc_cmd *cmd,
				  struct mmc_data *data)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
#else
static int dwmci_send_cmd_prepare(struct mmc *mmc, struct mmc_cmd *cmd,
				  struct mmc_data *data)
{
#endif
	struct dwmci_host *host = mmc->priv;
	struct dwmci_idmac *cur_idmac;
	int ret = 0, flags = 0, i;
	unsigned int timeout = 500;
	u32 retry = 100000;
	u32 mask;
	ulong start = get_timer(0);
	struct bounce_buffer bbstate;

	cur_idmac = malloc(ROUND(DIV_ROUND_UP(data->blocks, 8) *
			   sizeof(struct dwmci_idmac),
			   ARCH_DMA_MINALIGN) + ARCH_DMA_MINALIGN - 1);
	if (!cur_idmac)
		return -ENODATA;

	while (dwmci_readl(host, DWMCI_STATUS) & DWMCI_BUSY) {
		if (get_timer(start) > timeout) {
			debug("%s: Timeout on data busy\n", __func__);
			return -ETIMEDOUT;
		}
	}

	dwmci_writel(host, DWMCI_RINTSTS, DWMCI_INTMSK_ALL);

	if (data) {
		if (host->fifo_mode) {
			dwmci_writel(host, DWMCI_BLKSIZ, data->blocksize);
			dwmci_writel(host, DWMCI_BYTCNT,
				     data->blocksize * data->blocks);
			dwmci_wait_reset(host, DWMCI_CTRL_FIFO_RESET);
		} else {
			if (data->flags == MMC_DATA_READ) {
				bounce_buffer_start(&bbstate, (void *)data->dest,
						    data->blocksize *
						    data->blocks, GEN_BB_WRITE);
			} else {
				bounce_buffer_start(&bbstate, (void *)data->src,
						    data->blocksize *
						    data->blocks, GEN_BB_READ);
			}
			dwmci_prepare_data(host, data, cur_idmac,
					   bbstate.bounce_buffer);
		}
	}

	dwmci_writel(host, DWMCI_CMDARG, cmd->cmdarg);

	if (data)
		flags = dwmci_set_transfer_mode(host, data);

	if ((cmd->resp_type & MMC_RSP_136) && (cmd->resp_type & MMC_RSP_BUSY))
		return -1;

	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION)
		flags |= DWMCI_CMD_ABORT_STOP;
	else
		flags |= DWMCI_CMD_PRV_DAT_WAIT;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		flags |= DWMCI_CMD_RESP_EXP;
		if (cmd->resp_type & MMC_RSP_136)
			flags |= DWMCI_CMD_RESP_LENGTH;
	}

	if (cmd->resp_type & MMC_RSP_CRC)
		flags |= DWMCI_CMD_CHECK_CRC;

	flags |= (cmd->cmdidx | DWMCI_CMD_START | DWMCI_CMD_USE_HOLD_REG);

	debug("Sending CMD%d\n", cmd->cmdidx);

	dwmci_writel(host, DWMCI_CMD, flags);

	for (i = 0; i < retry; i++) {
		mask = dwmci_readl(host, DWMCI_RINTSTS);
		if (mask & DWMCI_INTMSK_CDONE) {
			if (!data)
				dwmci_writel(host, DWMCI_RINTSTS, mask);
			break;
		}
	}

	if (i == retry) {
		debug("%s: Timeout.\n", __func__);
		return -ETIMEDOUT;
	}

	if (mask & DWMCI_INTMSK_RTO) {
		/*
		 * Timeout here is not necessarily fatal. (e)MMC cards
		 * will splat here when they receive CMD55 as they do
		 * not support this command and that is exactly the way
		 * to tell them apart from SD cards. Thus, this output
		 * below shall be debug(). eMMC cards also do not favor
		 * CMD8, please keep that in mind.
		 */
		debug("%s: Response Timeout.\n", __func__);
		return -ETIMEDOUT;
	} else if (mask & DWMCI_INTMSK_RE) {
		debug("%s: Response Error.\n", __func__);
		return -EIO;
	}

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			cmd->response[0] = dwmci_readl(host, DWMCI_RESP3);
			cmd->response[1] = dwmci_readl(host, DWMCI_RESP2);
			cmd->response[2] = dwmci_readl(host, DWMCI_RESP1);
			cmd->response[3] = dwmci_readl(host, DWMCI_RESP0);
		} else {
			cmd->response[0] = dwmci_readl(host, DWMCI_RESP0);
		}
	}

	return ret;
}
#endif

static int dwmci_setup_bus(struct dwmci_host *host, u32 freq)
{
	u32 div, status;
	int timeout = 10000;
	unsigned long sclk;

	if (freq == 0)
		return 0;
	/*
	 * If host->get_mmc_clk isn't defined,
	 * then assume that host->bus_hz is source clock value.
	 * host->bus_hz should be set by user.
	 */
	if (host->get_mmc_clk)
		sclk = host->get_mmc_clk(host, freq);
	else if (host->bus_hz)
		sclk = host->bus_hz;
	else {
		debug("%s: Didn't get source clock value.\n", __func__);
		return -EINVAL;
	}

	if (sclk == 0)
		return -EINVAL;

	if (sclk == freq)
		div = 0;	/* bypass mode */
	else
		div = DIV_ROUND_UP(sclk, 2 * freq);

	dwmci_writel(host, DWMCI_CLKENA, 0);
	dwmci_writel(host, DWMCI_CLKSRC, 0);

	dwmci_writel(host, DWMCI_CLKDIV, div);
	dwmci_writel(host, DWMCI_CMD, DWMCI_CMD_PRV_DAT_WAIT |
			DWMCI_CMD_UPD_CLK | DWMCI_CMD_START);

	do {
		status = dwmci_readl(host, DWMCI_CMD);
		if (timeout-- < 0) {
			debug("%s: Timeout!\n", __func__);
			return -ETIMEDOUT;
		}
	} while (status & DWMCI_CMD_START);

	dwmci_writel(host, DWMCI_CLKENA, DWMCI_CLKEN_ENABLE |
			DWMCI_CLKEN_LOW_PWR);

	dwmci_writel(host, DWMCI_CMD, DWMCI_CMD_PRV_DAT_WAIT |
			DWMCI_CMD_UPD_CLK | DWMCI_CMD_START);

	timeout = 10000;
	do {
		status = dwmci_readl(host, DWMCI_CMD);
		if (timeout-- < 0) {
			debug("%s: Timeout!\n", __func__);
			return -ETIMEDOUT;
		}
	} while (status & DWMCI_CMD_START);

	host->clock = freq;

	return 0;
}

#ifdef CONFIG_DM_MMC
static bool dwmci_card_busy(struct udevice *dev)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
#else
static bool dwmci_card_busy(struct mmc *mmc)
{
#endif
	u32 status;
	struct dwmci_host *host = (struct dwmci_host *)mmc->priv;

	/*
	 * Check the busy bit which is low when DAT[3:0]
	 * (the data lines) are 0000
	 */
	status = dwmci_readl(host, DWMCI_STATUS);

	return !!(status & DWMCI_BUSY);
}

#ifdef CONFIG_DM_MMC
static int dwmci_execute_tuning(struct udevice *dev, u32 opcode)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
#else
static int dwmci_execute_tuning(struct mmc *mmc, u32 opcode)
{
#endif
	struct dwmci_host *host = (struct dwmci_host *)mmc->priv;

	if (!host->execute_tuning)
		return -EIO;

	return host->execute_tuning(host, opcode);
}

#ifdef CONFIG_DM_MMC
static int dwmci_set_ios(struct udevice *dev)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
#else
static int dwmci_set_ios(struct mmc *mmc)
{
#endif
	struct dwmci_host *host = (struct dwmci_host *)mmc->priv;
	u32 ctype, regs;

	debug("Buswidth = %d, clock: %d\n", mmc->bus_width, mmc->clock);

	dwmci_setup_bus(host, mmc->clock);
	switch (mmc->bus_width) {
	case 8:
		ctype = DWMCI_CTYPE_8BIT;
		break;
	case 4:
		ctype = DWMCI_CTYPE_4BIT;
		break;
	default:
		ctype = DWMCI_CTYPE_1BIT;
		break;
	}

	dwmci_writel(host, DWMCI_CTYPE, ctype);

	regs = dwmci_readl(host, DWMCI_UHS_REG);
	if (mmc_card_ddr(mmc))
		regs |= DWMCI_DDR_MODE;
	else
		regs &= ~DWMCI_DDR_MODE;

	dwmci_writel(host, DWMCI_UHS_REG, regs);

	if (host->clksel)
		host->clksel(host);

	return 0;
}

static int dwmci_init(struct mmc *mmc)
{
	struct dwmci_host *host = mmc->priv;
	uint32_t use_dma;
	uint32_t verid;

	if (host->board_init)
		host->board_init(host);
#ifdef CONFIG_ARCH_ROCKCHIP
	if (host->dev_index == 0)
		dwmci_writel(host, DWMCI_PWREN, 1);
	else if (host->dev_index == 1)
		dwmci_writel(host, DWMCI_PWREN, 0);
	else
		dwmci_writel(host, DWMCI_PWREN, 1);
#else
	dwmci_writel(host, DWMCI_PWREN, 1);
#endif

	verid = dwmci_readl(host, DWMCI_VERID) & 0x0000ffff;
	if (verid >= DW_MMC_240A)
		dwmci_writel(host, DWMCI_CARDTHRCTL, DWMCI_CDTHRCTRL_CONFIG);

	if (!dwmci_wait_reset(host, DWMCI_RESET_ALL)) {
		debug("%s[%d] Fail-reset!!\n", __func__, __LINE__);
		return -EIO;
	}

	use_dma = SDMMC_GET_TRANS_MODE(dwmci_readl(host, DWMCI_HCON));
	if (use_dma == DMA_INTERFACE_IDMA) {
		host->fifo_mode = 0;
	} else {
		host->fifo_mode = 1;
	}

	/* Enumerate at 400KHz */
	dwmci_setup_bus(host, mmc->cfg->f_min);

	dwmci_writel(host, DWMCI_RINTSTS, 0xFFFFFFFF);
	dwmci_writel(host, DWMCI_INTMASK, 0);

	dwmci_writel(host, DWMCI_TMOUT, 0xFFFFFFFF);

	dwmci_writel(host, DWMCI_IDINTEN, 0);
	dwmci_writel(host, DWMCI_BMOD, 1);

	if (!host->fifoth_val) {
		uint32_t fifo_size;

		fifo_size = dwmci_readl(host, DWMCI_FIFOTH);
		fifo_size = ((fifo_size & RX_WMARK_MASK) >> RX_WMARK_SHIFT) + 1;
		host->fifoth_val = MSIZE(DWMCI_MSIZE) |
				RX_WMARK(fifo_size / 2 - 1) |
				TX_WMARK(fifo_size / 2);
	}
	dwmci_writel(host, DWMCI_FIFOTH, host->fifoth_val);

	dwmci_writel(host, DWMCI_CLKENA, 0);
	dwmci_writel(host, DWMCI_CLKSRC, 0);

	return 0;
}

static int dwmci_get_cd(struct udevice *dev)
{
	int ret = -1;

#if defined(CONFIG_DM_GPIO) && (defined(CONFIG_SPL_GPIO_SUPPORT) || !defined(CONFIG_SPL_BUILD))
	struct gpio_desc detect;

	ret = gpio_request_by_name(dev, "cd-gpios", 0, &detect, GPIOD_IS_IN);
	if (ret) {
		return ret;
	}

	ret = !dm_gpio_get_value(&detect);
	dm_gpio_free(dev, &detect);
#endif
	return ret;
}

#ifdef CONFIG_DM_MMC
int dwmci_probe(struct udevice *dev)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);

	return dwmci_init(mmc);
}

const struct dm_mmc_ops dm_dwmci_ops = {
	.card_busy	= dwmci_card_busy,
	.send_cmd	= dwmci_send_cmd,
#ifdef CONFIG_SPL_BLK_READ_PREPARE
	.send_cmd_prepare = dwmci_send_cmd_prepare,
#endif
	.set_ios	= dwmci_set_ios,
	.get_cd         = dwmci_get_cd,
	.execute_tuning	= dwmci_execute_tuning,
};

#else
static const struct mmc_ops dwmci_ops = {
	.card_busy	= dwmci_card_busy,
	.send_cmd	= dwmci_send_cmd,
	.set_ios	= dwmci_set_ios,
	.get_cd         = dwmci_get_cd,
	.init		= dwmci_init,
	.execute_tuning	= dwmci_execute_tuning,
};
#endif

void dwmci_setup_cfg(struct mmc_config *cfg, struct dwmci_host *host,
		u32 max_clk, u32 min_clk)
{
	cfg->name = host->name;
#ifndef CONFIG_DM_MMC
	cfg->ops = &dwmci_ops;
#endif
	cfg->f_min = min_clk;
	cfg->f_max = max_clk;

	cfg->voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;

	cfg->host_caps = host->caps;

	switch (host->buswidth) {
	case 8:
		cfg->host_caps |= MMC_MODE_8BIT | MMC_MODE_4BIT;
		break;
	case 4:
		cfg->host_caps |= MMC_MODE_4BIT;
		cfg->host_caps &= ~MMC_MODE_8BIT;
		break;
	case 1:
		cfg->host_caps &= ~MMC_MODE_4BIT;
		cfg->host_caps &= ~MMC_MODE_8BIT;
		break;
	default:
		printf("Unsupported bus width: %d\n", host->buswidth);
		break;
	}
	cfg->host_caps |= MMC_MODE_HS | MMC_MODE_HS_52MHz;

	cfg->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;
}

#ifdef CONFIG_BLK
int dwmci_bind(struct udevice *dev, struct mmc *mmc, struct mmc_config *cfg)
{
	return mmc_bind(dev, mmc, cfg);
}
#else
int add_dwmci(struct dwmci_host *host, u32 max_clk, u32 min_clk)
{
	dwmci_setup_cfg(&host->cfg, host, max_clk, min_clk);

	host->mmc = mmc_create(&host->cfg, host);
	if (host->mmc == NULL)
		return -1;

	return 0;
}
#endif
