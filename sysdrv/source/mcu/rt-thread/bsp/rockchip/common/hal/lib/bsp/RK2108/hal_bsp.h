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

#ifdef HAL_I2C_MODULE_ENABLED
extern const struct HAL_I2C_DEV g_i2c0Dev;
extern const struct HAL_I2C_DEV g_i2c1Dev;
extern const struct HAL_I2C_DEV g_i2c2Dev;
#endif

#ifdef HAL_I2STDM_MODULE_ENABLED
extern struct HAL_I2STDM_DEV g_i2sTdm0Dev;
extern struct HAL_I2STDM_DEV g_i2sTdm1Dev;
#endif

#ifdef HAL_PDM_MODULE_ENABLED
extern struct HAL_PDM_DEV g_pdm0Dev;
#endif

#ifdef HAL_PWM_MODULE_ENABLED
extern const struct HAL_PWM_DEV g_pwm0Dev;
#endif

#ifdef HAL_PL330_MODULE_ENABLED
extern struct HAL_PL330_DEV g_pl330Dev;
#endif

#ifdef HAL_VAD_MODULE_ENABLED
extern struct HAL_VAD_DEV g_vadDev;
#endif

#if defined(HAL_PINCTRL_MODULE_ENABLED)
extern const struct HAL_PINCTRL_DEV g_pinDev;
#endif

#ifdef HAL_UART_MODULE_ENABLED
extern const struct HAL_UART_DEV g_uart0Dev;
extern const struct HAL_UART_DEV g_uart1Dev;
extern const struct HAL_UART_DEV g_uart2Dev;
#endif

#ifdef HAL_PCD_MODULE_ENABLED
extern const struct HAL_USB_DEV g_usbdDev;
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
extern struct HAL_FSPI_HOST g_fspi0Dev;
extern struct HAL_FSPI_HOST g_fspi1Dev;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
extern const struct HAL_SPI_DEV g_spi1Dev;
extern const struct HAL_SPI_DEV g_spi2Dev;
#endif

/***************************** Function Declare ******************************/

void BSP_MPU_Init(void);
void BSP_Init(void);
void BSP_DeInit(void);
void BSP_SetLoaderFlag(void);

#endif
