// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "rk_debug.h"
#include "uart.h"

#define PRINT(fmt, ...) \
    do{ \
        printf("[%s:%d->%s] "fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        printf("\n"); \
    } while(0)

static void set_baud_rate(int fd, int speed) {
    int i;
    int status;
    int speed_arr[] = {
        B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
        B38400,  B19200, B9600,  B4800, B2400, B1200, B300,
    };
    int name_arr[] = {
        115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,
        38400,  19200, 9600,  4800, 2400, 1200, 300,
    };
    struct termios Opt;

    tcgetattr(fd, &Opt);
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
        if (speed == name_arr[i]) {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt);
            if (status != 0)
              PRINT("set baud rate tcsetattr fd");
            return;
        }
        tcflush(fd, TCIOFLUSH);
    }
}

static int set_parity(int fd, int databits, int stopbits, int parity) {
    struct termios options;

    if (tcgetattr(fd, &options) != 0) {
        PRINT("set parity setup serial 1");
        return RK_FAILURE;
    }

    options.c_cflag &= ~CSIZE;

    switch (databits) {
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        PRINT("setParity Unsupported data size");
        return RK_FAILURE;
    }

    switch (parity) {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 'S':
    case 's':
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        PRINT("setParity Unsupported parity");
        return RK_FAILURE;
    }

    switch (stopbits) {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        PRINT("setParity Unsupported stop bits");
        return RK_FAILURE;
    }

    if (parity != 'n') {
        options.c_iflag |= INPCK;
    }

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF | IGNCR);

    options.c_cc[VTIME] = 150;
    options.c_cc[VMIN] = 10;

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        PRINT("setParity SetupSerial 3");
        return RK_FAILURE;
    }
    fcntl(fd,F_SETFL,0);
    return RK_SUCCESS;
}

static int open_uart_note(char *dev) {
    int fd = -1;
    int i;
    int max_uart_dev_node = 10;

    for (i = 0; i < max_uart_dev_node; i++) {
        sprintf(dev, "/dev/ttyS%d", i);
        fd = open(dev, O_RDWR);
        if (fd != -1)
            return fd;
    }

    PRINT("Can't Find Serial Port\n");
    return -1;
}

int smart_door_uart(void) {
    int fd;
    char dev_node[15];
    char *dev = dev_node;
    const char *test = "RK Smart Door Uart Test!\r\n";
    int bytes;
    char buf[11];

    fd = open_uart_note(dev);
    if (-1 == fd) {
        PRINT("Can't Open Serial Port :%s\n", dev);
        return -1;
    }
    set_baud_rate(fd, 115200);
    if(set_parity(fd, 8, 1, 'n') == RK_FAILURE) {
        return RK_FAILURE;
    }

    bytes = write(fd, test, strlen(test));
    if (bytes <= 0) {
       PRINT("write error %d", bytes);
       return RK_FAILURE;
    }

    PRINT("please input 10 bytes data by %s", dev);

    bytes = read(fd, buf, 10);
    if (bytes <= 0) {
        PRINT("read error %d", bytes);
        return RK_FAILURE;
    }
    buf[10] = '\0';
    PRINT("read data by uart:");
    PRINT("%s", buf);
    return RK_SUCCESS;
}

static int uart_fd = -1;
static pthread_t uart_tid = 0;
static char uart_g_run_flag = 0;

#define MAX_BUFF_CNT (256)
static void *uart_recv_thread(void *arg)
{
	PRINT("%s\n", __func__);
	uart_recv_callback cb = arg;
	int count;
	char buffer[MAX_BUFF_CNT];
	int ret = -EINVAL;
	int fd_result;
	int maxfd = 0;
	fd_set	recv_fds;
	struct	timeval tv;

	memset(buffer, 0, sizeof(buffer));
	memset(&tv, 0x00, sizeof(tv));
	tv.tv_usec  = 100000;

	if(uart_fd > maxfd)
		maxfd = uart_fd;

	uart_g_run_flag = 1;

	while(uart_g_run_flag) {
		if (uart_fd < 0)
			break;

		FD_ZERO(&recv_fds);
		FD_SET(uart_fd, &recv_fds);

		fd_result = select(maxfd + 1, &recv_fds, NULL, NULL, &tv);
		if(fd_result < 0) {
			PRINT("select err");
			int ret = -EINVAL;
			goto exit;
		} else if(fd_result == 0) {
			usleep(10000);
			continue;
		} else  {
			if(FD_ISSET(uart_fd, &recv_fds))
				count = read(uart_fd, buffer, 1);
				if (count < 0) {
					PRINT("%s error\n", __func__);
					continue;
				}
				if(cb)
					for (int i = 0; i < count; i++)
						cb(buffer[i]);
		}
	}

	ret = 0;
exit:
	if (ret)
		pthread_exit("uart_recv_thread exit failed.");
	else
		pthread_exit("uart_recv_thread exit success.");
}

void smart_door_uart_sendbyte(uint8_t byte)
{
	if (uart_fd < 0) {
		PRINT("uart no init\n");
		return;
	}

	int bytes = write(uart_fd, &byte, 1);
	if (bytes <= 0) {
		PRINT("write error %d", bytes);
		return;
	}

	return;
}

int smart_door_uart_init(void *param)
{
	PRINT("%s\n", __func__);
	char dev_node[15];
	char *dev = dev_node;

	uart_fd = open_uart_note(dev);
	if (-1 == uart_fd) {
		PRINT("Can't Open Serial Port :%s\n", dev);
		return -1;
	}
	set_baud_rate(uart_fd, 115200);
	if(set_parity(uart_fd, 8, 1, 'n') == RK_FAILURE) {
		return RK_FAILURE;
	}

	pthread_create(&uart_tid, NULL, uart_recv_thread, param);

	return RK_SUCCESS;
}

int smart_door_uart_deinit(void)
{
	void *thread_result;

	int ret = 0;
	uart_g_run_flag = 0;

	if (uart_tid) {
		PRINT("wait uart_tid thread join.\n");
		ret = pthread_join(uart_tid, &thread_result);
		PRINT("thread joined, it returned %s\n", (char *)thread_result);
		uart_tid = 0;
	}

	if (uart_fd > 0) {
		PRINT("close uart\n");
		close(uart_fd);
		uart_fd = -1;
	}

	return ret;
}
