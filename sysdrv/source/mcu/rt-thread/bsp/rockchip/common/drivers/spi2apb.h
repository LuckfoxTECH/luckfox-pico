/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: BSD-3-Clause
  ******************************************************************************
  * @file    spi2apb.h
  * @author  David Wu
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   spi2apb driver for pisces
  *
  ******************************************************************************
  */

#ifndef __DRV_SPI2APB_H__
#define __DRV_SPI2APB_H__

#include <stdlib.h>
#include <rtthread.h>
#include "hal_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ctrl */
#define RT_DEVICE_CTRL_SPI2APB_CONFIGURATION (1)
#define RT_DEVICE_CTRL_SPI2APB_QUERY_STATUS  (2)
#define RT_DEVICE_CTRL_SPI2APB_REGISTER_CB   (3)
#define RT_DEVICE_CTRL_SPI2APB_READ_REG0     (4)
#define RT_DEVICE_CTRL_SPI2APB_READ_REG1     (5)
#define RT_DEVICE_CTRL_SPI2APB_WRITE_REG2    (6)

/* configuratin */
#define RT_CONFIG_SPI2APB_LSB     SPI2APB_LSB
#define RT_CONFIG_SPI2APB_MSB     SPI2APB_MSB

#define RT_CONFIG_SPI2APB_BIG_ENDIAN     SPI2APB_BIG_ENDIAN
#define RT_CONFIG_SPI2APB_LITTLE_ENDIAN  SPI2APB_LITTLE_ENDIAN

#define RT_CONFIG_SPI2APB_RXCP           SPI2APB_RXCP
#define RT_CONFIG_SPI2APB_RXCP_INVERT    SPI2APB_RXCP_INVERT

#define RT_CONFIG_SPI2APB_TXCP           SPI2APB_TXCP
#define RT_CONFIG_SPI2APB_TXCP_INVERT    SPI2APB_TXCP_INVERT

/* state */
#define RT_STATE_SPI2APB_BUSY           SPI2APB_BUSY
#define RT_STATE_SPI2APB_TX_FULL        SPI2APB_TX_FULL
#define RT_STATE_SPI2APB_TX_EMPTY       SPI2APB_TX_EMPTY
#define RT_STATE_SPI2APB_RX_FULL        SPI2APB_RX_FULL
#define RT_STATE_SPI2APB_RX_EMPTY       SPI2APB_RX_EMPTY

typedef void (*spi2apb_callback)(rt_uint32_t cparam);

/**
 * SPI2APB configuration structure
 */
struct rt_spi2apb_configuration
{
    rt_uint8_t mode;
    rt_uint8_t clock_polarity;
};

/**
 * SPI2APB device structure
 */
struct rt_spi2apb_device
{
    struct rt_device parent;
    struct rt_spi2apb_configuration config;
};

#ifdef __cplusplus
}
#endif

#endif