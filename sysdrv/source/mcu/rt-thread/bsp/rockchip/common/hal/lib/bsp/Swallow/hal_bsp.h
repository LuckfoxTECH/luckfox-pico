/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __BSP_H__
#define __BSP_H__

#include "hal_base.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

#ifdef HAL_DWDMA_MODULE_ENABLED
extern struct HAL_DWDMA_DEV g_dwDmaDev;
#endif

#ifdef HAL_UART_MODULE_ENABLED
extern const struct HAL_UART_DEV g_uart0Dev;
#endif

#ifdef HAL_I2C_MODULE_ENABLED
extern const struct HAL_I2C_DEV g_i2c0Dev;
extern const struct HAL_I2C_DEV g_i2c1Dev;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
extern const struct HAL_SPI_DEV g_spi0Dev;
extern const struct HAL_SPI_DEV g_spi1Dev;
#endif

#ifdef HAL_PCD_MODULE_ENABLED
extern const struct HAL_USB_DEV g_usbdDev;
#endif

#ifdef HAL_PWM_MODULE_ENABLED
extern const struct HAL_PWM_DEV g_pwm0Dev;
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
extern struct HAL_FSPI_HOST g_fspi0Dev;
#endif

#ifdef HAL_CKCAL_MODULE_ENABLED
extern const struct HAL_CKCAL_DEV g_ckcalDev;
#endif

/***************************** Function Declare ******************************/
void BSP_Init(void);
void BSP_SetLoaderFlag(void);

#endif
