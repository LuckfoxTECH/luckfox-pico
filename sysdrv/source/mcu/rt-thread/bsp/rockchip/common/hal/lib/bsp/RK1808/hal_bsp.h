/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __BSP_H__
#define __BSP_H__

#include "hal_base.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

#ifdef HAL_I2C_MODULE_ENABLED
extern const struct HAL_I2C_DEV g_i2c0Dev;
extern const struct HAL_I2C_DEV g_i2c1Dev;
extern const struct HAL_I2C_DEV g_i2c2Dev;
extern const struct HAL_I2C_DEV g_i2c3Dev;
extern const struct HAL_I2C_DEV g_i2c4Dev;
extern const struct HAL_I2C_DEV g_i2c5Dev;
#endif

#ifdef HAL_I2S_MODULE_ENABLED
extern struct HAL_I2S_DEV g_i2s1Dev;
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
extern struct HAL_I2STDM_DEV g_i2sTdm0Dev;
#endif

#ifdef HAL_PDM_MODULE_ENABLED
extern struct HAL_PDM_DEV g_pdm0Dev;
#endif

#if defined(HAL_PINCTRL_MODULE_ENABLED)
extern const struct HAL_PINCTRL_DEV g_pinDev;
#endif

#ifdef HAL_PL330_MODULE_ENABLED
extern struct HAL_PL330_DEV g_pl330Dev;
#endif

#ifdef HAL_VAD_MODULE_ENABLED
extern struct HAL_VAD_DEV g_vadDev;
#endif

#ifdef HAL_UART_MODULE_ENABLED
extern const struct HAL_UART_DEV g_uart0Dev;
extern const struct HAL_UART_DEV g_uart1Dev;
extern const struct HAL_UART_DEV g_uart2Dev;
extern const struct HAL_UART_DEV g_uart3Dev;
extern const struct HAL_UART_DEV g_uart4Dev;
extern const struct HAL_UART_DEV g_uart5Dev;
extern const struct HAL_UART_DEV g_uart6Dev;
extern const struct HAL_UART_DEV g_uart7Dev;
#endif

#if defined(HAL_EHCI_MODULE_ENABLED) || defined(HAL_OHCI_MODULE_ENABLED)
extern const struct HAL_USBH_DEV g_usbhDev;
#endif

/***************************** Function Declare ******************************/

void BSP_Init(void);
void BSP_DeInit(void);

#endif
