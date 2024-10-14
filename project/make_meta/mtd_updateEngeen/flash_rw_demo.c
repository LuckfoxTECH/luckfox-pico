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

static NORETURN void showusage(void)
{
	fprintf(stderr, "usage: %1$s <device> <start> <length>\n",
			PROGRAM_NAME);
	exit(EXIT_FAILURE);
}

int main(int argc, char * const argv[])
{
	int start, size;
	FILE *file;
	char *pwrite, *pread;
	int i, ret;

	printf("flash_rw_demo\n");

	if (argc < 4) {
		showusage();
		return -1;
	}
	start = strtoll(argv[2], NULL, 0);
	size = strtoll(argv[3], NULL, 0);
	pwrite = malloc(size);
	pread = malloc(size);

	for (i = 0; i < size; i++) {
		pwrite[i] = i & 0xff;
	}

	ret = flash_write_buf(argv[1], pwrite, start, size);
	if (ret) {
		printf("write failed, ret=%d\n", ret);
		return ret;
	}

	memset(pread, 0, size);
	ret = flash_read_buf(argv[1], pread, start, size);
	if (ret) {
		printf("read failed, ret=%d\n", ret);
		return ret;
	}
	if (memcmp(pwrite, pread, size)) {
		printf("compare failed\n");
		return ret;
	}

	free(pwrite);
	free(pread);

	return 0;
}
