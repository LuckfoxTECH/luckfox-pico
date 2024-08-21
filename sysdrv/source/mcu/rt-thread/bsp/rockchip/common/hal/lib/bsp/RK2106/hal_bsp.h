/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __BSP_H__
#define __BSP_H__

#include "hal_base.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

#ifdef HAL_ACODEC_MODULE_ENABLED
extern struct HAL_ACODEC_DEV g_acodecDev;
#endif

#ifdef HAL_DWDMA_MODULE_ENABLED
extern struct HAL_DWDMA_DEV g_dwDma1Dev;
#endif

#ifdef HAL_I2S_MODULE_ENABLED
extern struct HAL_I2S_DEV g_i2s0Dev;
extern struct HAL_I2S_DEV g_i2s1Dev;
#endif

#ifdef HAL_UART_MODULE_ENABLED
extern const struct HAL_UART_DEV g_uart0Dev;
extern const struct HAL_UART_DEV g_uart1Dev;
extern const struct HAL_UART_DEV g_uart2Dev;
extern const struct HAL_UART_DEV g_uart3Dev;
extern const struct HAL_UART_DEV g_uart4Dev;
extern const struct HAL_UART_DEV g_uart5Dev;
#endif

#ifdef HAL_PCD_MODULE_ENABLED
extern const struct HAL_USB_DEV g_usbdDev;
#endif

/***************************** Function Declare ******************************/

void BSP_Init(void);

#endif
