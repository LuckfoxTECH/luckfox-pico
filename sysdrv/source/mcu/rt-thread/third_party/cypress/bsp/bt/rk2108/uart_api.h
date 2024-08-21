/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-20     chd.ma   the first version
 */

#ifndef _UART_API_H_
#define _UART_API_H_

#define  USING_POSIX

#ifdef RT_USING_POSIX_TERMIOS
int open_comm(const char *name);
void close_comm(int fd);
void config_comm(int fd, int speed_baud_rate, char parity, int data_bits, int stop_bits);
int recv_comm(int fd, unsigned char *buffer, rt_size_t size, struct timeval *timeout);
int send_comm(int fd, const unsigned char *buffer, rt_size_t size);
int flush_comm(int fd);
//void set_comm_flow_control(int fd, int enable);

#else
rt_size_t serial_write(rt_device_t dev, const void *buffer, rt_size_t size);
rt_size_t serial_read(rt_device_t dev, void *buffer, rt_size_t size);
#endif

#endif

