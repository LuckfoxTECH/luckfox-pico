/**
  * Copyright (c) 2021 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    bt_soc_uart.h
  * @version V0.1
  * @brief   bt_soc_uart head file
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-01-04     Chris Zhong     first implementation
  *
  ******************************************************************************
  */
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/serial.h>

rt_err_t rk_uart_write(const char *p_data, uint32_t length);
rt_err_t rk_uart_read(char *p_data, uint32_t *length, uint32_t timeout_ms);
rt_err_t rk_uart_set_baudrate(uint32_t baudrate);
rt_err_t rk_uart_close(void);
rt_err_t rk_uart_open(void);
rt_err_t rk_uart_register_rx_ready_cb(rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size));
rt_err_t rk_uart_init(char *uart_name);