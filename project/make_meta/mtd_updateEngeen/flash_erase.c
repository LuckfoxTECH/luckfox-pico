/* flash_erase.c -- erase MTD devices

   Copyright (C) 2000 Arcom Control System Ltd
   Copyright (C) 2010 Mike Frysinger <vapier@gentoo.org>
   Copyright 2021 NXP

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#define PROGRAM_NAME "flash_erase"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <common.h>
#include <crc32.h>
#include <libmtd.h>

#include <mtd/mtd-user.h>

static const char *mtd_device;

static int quiet;		/* true -- don't output progress */
static int noskipbad;		/* do not skip bad blocks */
static int unlock;		/* unlock sectors before erasing */

int target_endian = __BYTE_ORDER;

static void show_progress(off_t start, int eb, int eb_start, int eb_cnt, int step)
{
	bareverbose(!quiet, "\rErasing %d Kibyte @ %llx -- %2i %% complete ",
		step / 1024, (unsigned long long)start, ((eb - eb_start) * 100) / eb_cnt);
	fflush(stdout);
}

static void display_help(void)
{
	printf("Usage: %s [options] MTD_DEVICE <start offset> <block count>\n"
			"Erase blocks of the specified MTD device.\n"
			"Specify a count of 0 to erase to end of device.\n"
			"\n"
			"  -N, --noskipbad         don't skip bad blocks\n"
			"  -u, --unlock            unlock sectors before erasing\n"
			"  -q, --quiet             do not display progress messages\n"
			"      --silent            same as --quiet\n"
			"      --help              display this help and exit\n"
			"      --version           output version information and exit\n",
			"\n"
			"  MTD_DEVICE  MTD device node or 'mtd:<name>'\n"
			PROGRAM_NAME);
}

static void display_version(void)
{
	common_print_version();
	printf("Copyright (C) 2000 Arcom Control Systems Ltd\n"
			"\n"
			"%1$s comes with NO WARRANTY\n"
			"to the extent permitted by law.\n"
			"\n"
			"You may redistribute copies of %1$s\n"
			"under the terms of the GNU General Public Licence.\n"
			"See the file `COPYING' for more information.\n",
			PROGRAM_NAME);
}

int flash_erase(char *mtd_dev, size_t start, size_t length)
{
	libmtd_t mtd_desc;
	struct mtd_dev_info mtd;
	int fd, cmlen = 8;
	unsigned int eb, eb_start, eb_cnt = 0;
	bool isNAND, erase_chip = false;
	int error = 0;
	off_t offset = 0;

	printf("%s start=0x%x(Bytes) length=0x%x(Bytes)\n", __func__, start, length);

	mtd_device = mtd_find_dev_node(mtd_dev);
	if (!mtd_device)
		return errmsg("Can't find MTD device %s", mtd_dev);

	if (error)
		return errmsg("Try `--help' for more information");

	/*
	 * Locate MTD and prepare for erasure
	 */
	mtd_desc = libmtd_open();
	if (mtd_desc == NULL)
		return errmsg("can't initialize libmtd");

	fd = open(mtd_device, O_RDWR);
	if (fd < 0)
		return sys_errmsg("%s", mtd_device);

	if (mtd_get_dev_info(mtd_desc, mtd_device, &mtd) < 0)
		return errmsg("mtd_get_dev_info failed");

	if (length > mtd.size)
		return errmsg("erase size(%dBytes) out of partition size(%dBytes)", length, mtd.size);

	eb_start = start / mtd.eb_size;
	eb_cnt = (length + mtd.eb_size - 1) / mtd.eb_size; /* aligned */

	isNAND = mtd.type == MTD_NANDFLASH || mtd.type == MTD_MLCNANDFLASH;

	/*
	 * Now do the actual erasing of the MTD device
	 */
	if (eb_cnt == 0)
		eb_cnt = (mtd.size / mtd.eb_size) - eb_start;

erase_each_sector:
	for (eb = eb_start; eb < eb_start + eb_cnt; eb++) {
		offset = (off_t)eb * mtd.eb_size;

		if (!noskipbad) {
			int ret = mtd_is_bad(&mtd, fd, eb);

			if (ret > 0) {
				verbose(!quiet, "Skipping bad block at %08llx", (unsigned long long)offset);
				continue;
			} else if (ret < 0) {
				if (errno == EOPNOTSUPP) {
					noskipbad = 1;
					if (isNAND)
						return errmsg("%s: Bad block check not available", mtd_device);
				} else
					return sys_errmsg("%s: MTD get bad block failed", mtd_device);
			}
		}

		show_progress(offset, eb, eb_start, eb_cnt, mtd.eb_size);

		if (unlock) {
			if (mtd_unlock(&mtd, fd, eb) != 0) {
				sys_errmsg("%s: MTD unlock failure", mtd_device);
				continue;
			}
		}

		if (mtd_erase(mtd_desc, &mtd, fd, eb) != 0) {
			sys_errmsg("%s: MTD Erase failure", mtd_device);
			continue;
		}
	}
	show_progress(offset, eb, eb_start, eb_cnt, mtd.eb_size);
out:
	bareverbose(!quiet, "\n");

	return 0;
}
