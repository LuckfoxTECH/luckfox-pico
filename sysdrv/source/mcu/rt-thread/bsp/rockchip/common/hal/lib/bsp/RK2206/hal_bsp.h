/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __BSP_H__
#define __BSP_H__

#include "hal_base.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/
#ifdef HAL_ACDCDIG_MODULE_ENABLED
extern struct HAL_ACDCDIG_DEV g_acdcDigDev;
#endif

#ifdef HAL_AUDIOPWM_MODULE_ENABLED
extern struct HAL_AUDIOPWM_DEV g_audioPwmDev;
#endif

#ifdef HAL_DWDMA_MODULE_ENABLED
extern struct HAL_DWDMA_DEV g_dwDmaDev;
#endif

#ifdef HAL_HYPERPSRAM_MODULE_ENABLED
extern struct HAL_HYPERPSRAM_DEV g_hyperpsramDev;
#endif

#ifdef HAL_I2C_MODULE_ENABLED
extern const struct HAL_I2C_DEV g_i2c0Dev;
extern const struct HAL_I2C_DEV g_i2c1Dev;
extern const struct HAL_I2C_DEV g_i2c2Dev;
#endif

#ifdef HAL_PDM_MODULE_ENABLED
extern struct HAL_PDM_DEV g_pdm0Dev;
#endif

#if defined(HAL_PINCTRL_MODULE_ENABLED)
extern const struct HAL_PINCTRL_DEV g_pinDev;
#endif

#ifdef HAL_PWM_MODULE_ENABLED
extern const struct HAL_PWM_DEV g_pwm0Dev;
extern const struct HAL_PWM_DEV g_pwm1Dev;
extern const struct HAL_PWM_DEV g_pwm2Dev;
#endif

#ifdef HAL_UART_MODULE_ENABLED
extern const struct HAL_UART_DEV g_uart0Dev;
extern const struct HAL_UART_DEV g_uart1Dev;
extern const struct HAL_UART_DEV g_uart2Dev;
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
extern struct HAL_I2STDM_DEV g_i2sTdm0Dev;
extern struct HAL_I2STDM_DEV g_i2sTdm1Dev;
#endif

#ifdef HAL_TSADC_MODULE_ENABLED
extern const struct HAL_TSADC_DEV g_tsadcDev;
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
extern struct HAL_FSPI_HOST g_fspi0Dev;
extern struct HAL_FSPI_HOST g_fspi1Dev;
#endif

#ifdef HAL_PVTM_MODULE_ENABLED
extern const struct HAL_PVTM_DEV g_pvtmDev;
#endif

#if defined(HAL_PCD_MODULE_ENABLED) || defined(HAL_HCD_MODULE_ENABLED)
extern const struct HAL_USB_DEV g_usbdDev;
#endif

#ifdef HAL_VAD_MODULE_ENABLED
extern struct HAL_VAD_DEV g_vadDev;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
extern const struct HAL_SPI_DEV g_spi0Dev;
extern const struct HAL_SPI_DEV g_spi1Dev;
#endif

/***************************** Function Declare ******************************/

void BSP_Init(void);
void BSP_DeInit(void);

#endif
