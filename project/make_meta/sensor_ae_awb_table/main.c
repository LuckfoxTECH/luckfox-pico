#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "config.h"

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("usage: start_ae <output file name>");
	}

	char *file_path = argv[1];
	int fd = open(file_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0) {
		printf("create %s failed: %s\n", file_path, strerror(errno));
		return -1;
	}

	int len;
	len = write(fd, &g_2a_init_info, sizeof(g_2a_init_info));

	close(fd);

	if (len < 0) {
		printf("write error:%s\n", strerror(errno));

		return -1;
	}

	printf("make 2a table file [%s] len=%d \n", file_path, len);

	return 0;
}

