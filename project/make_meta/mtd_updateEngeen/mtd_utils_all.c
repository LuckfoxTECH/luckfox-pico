/*
 * Copyright (c) 2024 Rockchip Electronics Co. Ltd.
 * Written by Jon Lin <jon.lin@rock-chips.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of other contributors
 *	  may be used to endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define PROGRAM_NAME "mtd_debug"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <getopt.h>

#include <asm/types.h>
#include "mtd-user.h"
#include "common.h"
#include <libmtd.h>

#include <mtd_utils_all.h>

#define MTD_UPDATEENGEEN_VERSION "v1.0.0"

#define MTD_UPDATEENGEEN_HEADFILE_MAGIC_NUM 8
#define MTD_UPDATEENGEEN_UBIFS_MAGIC 0x23494255
#define MTD_UPDATEENGEEN_JFFS2_MAGIC 0xe0011985
#define MTD_UPDATEENGEEN_JFFS_MAGIC 0x20031985


#define CHECK_FILE "/tmp/.mtd_updateEngeen_check.bak"

int mtd_updateEngeen_ota_one(char *mtd_dev, char *file)
{
	unsigned int imglen = 0, erase_aligned = 0;
	struct mtd_dev_info mtd;
	libmtd_t mtd_desc;
	int fd = -1, ifd = -1;
	struct stat st;
	int type = MTD_ABSENT;
	unsigned char *md5_hash_src, *md5_hash_dst;
	int ret;
	FILE *filp;
	char magic[MTD_UPDATEENGEEN_HEADFILE_MAGIC_NUM];
	bool erase_whole_part;

	printf("ota: prograom %s to %s\n", file, mtd_dev);

	if ((fd = open(mtd_dev, O_RDWR)) == -1)
		sys_errmsg_die("%s", mtd_dev);

	mtd_desc = libmtd_open();
	if (!mtd_desc) {
		errmsg_die("can't initialize libmtd");
		goto closeall;
	}

	/* Fill in MTD device capability structure */
	if (mtd_get_dev_info(mtd_desc, mtd_dev, &mtd) < 0)
		errmsg_die("mtd_get_dev_info failed");

	ifd = open(file, O_RDONLY);
	if (ifd == -1) {
		perror(file);
		goto closeall;
	}

	if (fstat(ifd, &st)) {
		sys_errmsg("unable to stat input image");
		goto closeall;
	}

	filp = fopen(file, "r");
	if (filp == NULL) {
		errmsg_die("fopen %s failed", file);
		goto closeall;
	}

	if (fgets(magic, MTD_UPDATEENGEEN_HEADFILE_MAGIC_NUM, filp) != NULL) {
	} else {
		errmsg_die("fget %s failed", file);
	}
	switch (((u_int32_t *)magic)[0]) {
	case MTD_UPDATEENGEEN_UBIFS_MAGIC:
		printf("file_type=UBIFS\n");
		erase_whole_part = true;
		break;
	case MTD_UPDATEENGEEN_JFFS2_MAGIC:
		printf("file_type=JFFS2\n");
		erase_whole_part = true;
		break;
	case MTD_UPDATEENGEEN_JFFS_MAGIC:
		printf("file_type=JFFS\n");
		erase_whole_part = true;
		break;
	default:
		printf("file_type=common\n");
		erase_whole_part = false;
		break;
	}

	imglen = st.st_size;

	printf("mtdinfo:\n");
	printf("name=%s\n", mtd.name);
	printf("type=%s\n", mtd.type_str);
	printf("size=%llx\n", mtd.size);
	printf("eb_cnt=%x\n", mtd.eb_cnt);
	printf("eb_size=%x\n", mtd.eb_size);
	printf("min_io_size=%x\n", mtd.min_io_size);
	printf("imglen=%x\n", st.st_size);

	if (erase_whole_part)
		erase_aligned = mtd.size;
	else
		erase_aligned = (imglen + mtd.eb_size - 1) / mtd.eb_size * mtd.eb_size;
	if (strcmp("nor", mtd.type_str) == 0) {
		type = MTD_NORFLASH;
		ret = mtd_debug_erase(mtd_dev, 0, erase_aligned);
		if (ret) {
			mtd_debug_erase(mtd_dev, 0, erase_aligned);
		}
		ret = mtd_debug_write(mtd_dev, 0, imglen, file);
		if (ret) {
			mtd_debug_erase(mtd_dev, 0, erase_aligned);
			mtd_debug_write(mtd_dev, 0, imglen, file);
		}
		mtd_debug_read(mtd_dev, 0, imglen, CHECK_FILE);
	} else if (strcmp("nand", mtd.type_str) == 0) {
		type = MTD_NANDFLASH;
		ret = flash_erase(mtd_dev, 0, erase_aligned);
		if (ret) {
			flash_erase(mtd_dev, 0, erase_aligned);
		}
		ret = nandwrite(mtd_dev, file, type);
		if (ret) {
			flash_erase(mtd_dev, 0, erase_aligned);
			nandwrite(mtd_dev, file, type);
		}
		nanddump(mtd_dev, imglen, CHECK_FILE, type);
	} else {
		sys_errmsg("mtd type is unsupported!");
	}

#if 0
	calculate_file_md5(file, md5_hash_src);
	printf("md5_hash_src %s\n", md5_hash_src);
	calculate_file_md5(CHECK_FILE, md5_hash_dst);
	printf("md5_hash_dst %s\n", md5_hash_dst);
#endif

closeall:
	if (ifd > 0 && ifd != STDIN_FILENO)
		close(ifd);
	if (!mtd_desc)
		libmtd_close(mtd_desc);
	close(fd);
	return -1;
}
