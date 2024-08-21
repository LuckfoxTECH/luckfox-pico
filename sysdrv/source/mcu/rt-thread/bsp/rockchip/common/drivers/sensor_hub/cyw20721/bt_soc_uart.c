/**
  * Copyright (c) 2021 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    bt_soc_uart.c
  * @version V0.1
  * @brief   bt_soc_uart driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-01-04     Chris Zhong     first implementation
  *
  ******************************************************************************
  */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "hal_base.h"
#include "bt_soc_uart.h"

static rt_device_t serial;

static void uart2_rts_iomux_config(int uart_en)
{
    if (uart_en)
    {
        HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                             GPIO_PIN_A3,
                             PIN_CONFIG_MUX_FUNC4);
    }
    else
    {
        HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                             GPIO_PIN_A3,
                             PIN_CONFIG_MUX_FUNC0);
        HAL_GPIO_SetPinDirection(GPIO1, GPIO_PIN_A3, GPIO_OUT);
        HAL_GPIO_SetPinLevel(GPIO1, GPIO_PIN_A3, GPIO_HIGH);
    }
}

rt_err_t rk_uart_write(const char *p_data, uint32_t length)
{
    /* 发送字符串 */
    rt_device_write(serial, 0, p_data, length);
    return 0;
}

rt_err_t rk_uart_read(char *p_data, uint32_t *length, uint32_t timeout_ms)
{
    if (rt_device_read(serial, 0, p_data, *length) == *length)
        return 0;
    else
        return -1;
}

rt_err_t rk_uart_set_baudrate(uint32_t baudrate)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    config.baud_rate = baudrate;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.bufsz     = 128;
    config.parity    = PARITY_NONE;

    return rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
}

rt_err_t rk_uart_close(void)
{
    rt_device_close(serial);
    uart2_rts_iomux_config(0);

    return 0;
}

rt_err_t rk_uart_open(void)
{
    uart2_rts_iomux_config(1);
    return rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
}

rt_err_t rk_uart_register_rx_ready_cb(rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size))
{
    return rt_device_set_rx_indicate(serial, rx_ind);
}

rt_err_t rk_uart_init(char *uart_name)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */

    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    config.baud_rate = BAUD_RATE_3000000;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.bufsz     = 128;
    config.parity    = PARITY_NONE;
    config.flow_ctrl = 1;

    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

    return 0;
}