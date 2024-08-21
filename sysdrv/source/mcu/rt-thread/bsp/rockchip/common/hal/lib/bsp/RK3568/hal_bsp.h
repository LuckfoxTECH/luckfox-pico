/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __BSP_H__
#define __BSP_H__

#include "hal_base.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/
#ifdef HAL_PL330_MODULE_ENABLED
extern struct HAL_PL330_DEV g_pl330Dev0;
extern struct HAL_PL330_DEV g_pl330Dev1;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
extern const struct HAL_SPI_DEV g_spi0Dev;
extern const struct HAL_SPI_DEV g_spi1Dev;
extern const struct HAL_SPI_DEV g_spi2Dev;
extern const struct HAL_SPI_DEV g_spi3Dev;
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
extern const struct HAL_UART_DEV g_uart8Dev;
extern const struct HAL_UART_DEV g_uart9Dev;
#endif

#ifdef HAL_I2C_MODULE_ENABLED
extern const struct HAL_I2C_DEV g_i2c0Dev;
extern const struct HAL_I2C_DEV g_i2c1Dev;
extern const struct HAL_I2C_DEV g_i2c2Dev;
extern const struct HAL_I2C_DEV g_i2c3Dev;
extern const struct HAL_I2C_DEV g_i2c4Dev;
extern const struct HAL_I2C_DEV g_i2c5Dev;
#endif

#ifdef HAL_FSPI_MODULE_ENABLED
extern struct HAL_FSPI_HOST g_fspi0Dev;
#endif
#ifdef HAL_CANFD_MODULE_ENABLED
extern const struct HAL_CANFD_DEV g_can0Dev;
extern const struct HAL_CANFD_DEV g_can1Dev;
extern const struct HAL_CANFD_DEV g_can2Dev;
#endif

#ifdef HAL_GMAC_MODULE_ENABLED
extern const struct HAL_GMAC_DEV g_gmac0Dev;
extern const struct HAL_GMAC_DEV g_gmac1Dev;
#endif

#ifdef HAL_PCIE_MODULE_ENABLED
extern struct HAL_PCIE_DEV g_pcieDev;
#endif

#ifdef HAL_PWM_MODULE_ENABLED
extern const struct HAL_PWM_DEV g_pwm0Dev;
extern const struct HAL_PWM_DEV g_pwm1Dev;
extern const struct HAL_PWM_DEV g_pwm2Dev;
extern const struct HAL_PWM_DEV g_pwm3Dev;
#endif
/***************************** Function Declare ******************************/
void BSP_Init(void);

#endif
