/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <dm.h>
#include <part.h>
#include <div64.h>
#include <linux/math64.h>
#include "mmc_private.h"

static ulong mmc_erase_t(struct mmc *mmc, ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	ulong end;
	int err, start_cmd, end_cmd;
	uint erase_mode;

	if (mmc->high_capacity) {
		end = start + blkcnt - 1;
	} else {
		end = (start + blkcnt - 1) * mmc->write_bl_len;
		start *= mmc->write_bl_len;
	}

	/*
	 * The SD card have just one erase mode, the erase command
	 * perform an erase on the sector(s).
	 * The Emmc have four erase mode. We use the trim mode with
	 * cmd.cmdarg equal to MMC_TRIM_ARG which command performs an
	 * erase on the sector(s).
	 */
	if (IS_SD(mmc)) {
		start_cmd = SD_CMD_ERASE_WR_BLK_START;
		end_cmd = SD_CMD_ERASE_WR_BLK_END;
		erase_mode = MMC_ERASE_ARG;
	} else {
		start_cmd = MMC_CMD_ERASE_GROUP_START;
		end_cmd = MMC_CMD_ERASE_GROUP_END;
		if (mmc->esr.mmc_can_trim)
			erase_mode = MMC_TRIM_ARG;
		else
			erase_mode = MMC_ERASE_ARG;
	}

	cmd.cmdidx = start_cmd;
	cmd.cmdarg = start;
	cmd.resp_type = MMC_RSP_R1;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = end_cmd;
	cmd.cmdarg = end;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = erase_mode;
	cmd.resp_type = MMC_RSP_R1b;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	return 0;

err_out:
	puts("mmc erase failed\n");
	return err;
}

#ifdef CONFIG_BLK
ulong mmc_berase(struct udevice *dev, lbaint_t start, lbaint_t blkcnt)
#else
ulong mmc_berase(struct blk_desc *block_dev, lbaint_t start, lbaint_t blkcnt)
#endif
{
#ifdef CONFIG_BLK
	struct blk_desc *block_dev = dev_get_uclass_platdata(dev);
#endif
	int dev_num = block_dev->devnum;
	int err = 0;
	u32 start_rem, blkcnt_rem;
	struct mmc *mmc = find_mmc_device(dev_num);
	lbaint_t blk = 0, blk_r = 0;
	int timeout = 1000;
	int mode = 0;

	if (!mmc)
		return -1;

	if (!blkcnt)
		return 0;

	err = blk_select_hwpart_devnum(IF_TYPE_MMC, dev_num,
				       block_dev->hwpart);

	if (err < 0)
		return -1;

	if (!IS_SD(mmc)) {
		if (mmc->esr.mmc_can_trim)
			mode = 1;
	}

	if (mode) {
		err = mmc_erase_t(mmc, start, blkcnt);
		if (err)
			return err;
		if (mmc_send_status(mmc, timeout))
			return 0;

		return blkcnt;
	} else {
		/*
		 * We want to see if the requested start or total block
		 * count are unaligned.  We discard the whole numbers and
		 * only care about the remainder.
		 */
		err = div_u64_rem(start, mmc->erase_grp_size, &start_rem);
		err = div_u64_rem(blkcnt, mmc->erase_grp_size, &blkcnt_rem);
		if (start_rem || blkcnt_rem)
			printf("\n\nCaution! Your devices Erase group is 0x%x\n"
			       "The erase range would be change to "
			       "0x" LBAF "~0x" LBAF "\n\n",
			       mmc->erase_grp_size,
			       start & ~(mmc->erase_grp_size - 1),
			       ((start + blkcnt + mmc->erase_grp_size)
			       & ~(mmc->erase_grp_size - 1)) - 1);
		while (blk < blkcnt) {
			if (IS_SD(mmc) && mmc->ssr.au) {
				blk_r = ((blkcnt - blk) > mmc->ssr.au) ?
				mmc->ssr.au : (blkcnt - blk);
			} else {
				blk_r = ((blkcnt - blk) > mmc->erase_grp_size) ?
					mmc->erase_grp_size : (blkcnt - blk);
			}
			err = mmc_erase_t(mmc, start + blk, blk_r);
			if (err)
				break;

			blk += blk_r;

			/* Waiting for the ready status */
			if (mmc_send_status(mmc, timeout))
				return 0;
		}
		return blk;
	}
}

static ulong mmc_write_blocks(struct mmc *mmc, lbaint_t start,
		lbaint_t blkcnt, const void *src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;
	ulong writen_cnt = blkcnt;

	if ((start + blkcnt) > mmc_get_blk_desc(mmc)->lba) {
		printf("MMC: block number 0x" LBAF " exceeds max(0x" LBAF ")\n",
		       start + blkcnt, mmc_get_blk_desc(mmc)->lba);
		return 0;
	}

	if (blkcnt == 0)
		return 0;
	else if (blkcnt == 1)
		cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	cmd.resp_type = MMC_RSP_R1;

	data.src = src;
	data.blocks = blkcnt;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;

	if (mmc_send_cmd(mmc, &cmd, &data)) {
		printf("mmc write failed\n");
		writen_cnt = 0;
	}

	/* SPI multiblock writes terminate using a special
	 * token, not a STOP_TRANSMISSION request.
	 */
	if (!mmc_host_is_spi(mmc) && blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			printf("mmc fail to send stop cmd\n");
			return 0;
		}
	}

	/* Waiting for the ready status */
	if (mmc_send_status(mmc, timeout))
		return 0;

	return writen_cnt;
}

#ifdef CONFIG_BLK
ulong mmc_bwrite(struct udevice *dev, lbaint_t start, lbaint_t blkcnt,
		 const void *src)
#else
ulong mmc_bwrite(struct blk_desc *block_dev, lbaint_t start, lbaint_t blkcnt,
		 const void *src)
#endif
{
#ifdef CONFIG_BLK
	struct blk_desc *block_dev = dev_get_uclass_platdata(dev);
#endif
	int dev_num = block_dev->devnum;
	lbaint_t cur, blocks_todo = blkcnt;
	int err;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	err = blk_select_hwpart_devnum(IF_TYPE_MMC, dev_num, block_dev->hwpart);
	if (err < 0)
		return 0;

	if (mmc_set_blocklen(mmc, mmc->write_bl_len))
		return 0;

	do {
		cur = (blocks_todo > mmc->cfg->b_max) ?
			mmc->cfg->b_max : blocks_todo;
		if (mmc_write_blocks(mmc, start, cur, src) != cur) {
			/* retry again with Open-ended Multiple block write */
			if (mmc_write_blocks(mmc, start, cur, src) != cur)
				return 0;
		}
		blocks_todo -= cur;
		start += cur;
		src += cur * mmc->write_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}
