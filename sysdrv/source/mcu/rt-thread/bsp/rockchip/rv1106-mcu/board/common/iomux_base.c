/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-08     Steven Liu   first implementation
 */

#include "rtdef.h"
#include "iomux.h"
#include "hal_base.h"

/**
 * @brief  Config iomux m2 for I2C0
 */
RT_WEAK RT_UNUSED void i2c0_m2_iomux_config(void)
{
    /* I2C0 M2 SCL-3A4 SDA-3A5 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3,
                         GPIO_PIN_A4 | GPIO_PIN_A5,
                         PIN_CONFIG_MUX_FUNC3);

    /* pull none smt */
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_A4 | GPIO_PIN_A5,
                         PIN_CONFIG_PUL_NORMAL |
                         PIN_CONFIG_SMT_ENABLE);
}

/**
 * @brief  Config iomux m1 for I2C3
 */
RT_WEAK RT_UNUSED void i2c3_m1_iomux_config(void)
{
    /* I2C0 M2 SCL-1D3 SDA-1D2 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_D2 | GPIO_PIN_D3,
                         PIN_CONFIG_MUX_FUNC3);

    /* pull none smt */
    HAL_PINCTRL_SetParam(GPIO_BANK1,
                         GPIO_PIN_D2 | GPIO_PIN_D3,
                         PIN_CONFIG_PUL_NORMAL |
                         PIN_CONFIG_SMT_ENABLE);
}

/**
 * @brief  Config iomux m2 for I2C3
 */
RT_WEAK RT_UNUSED void i2c3_m2_iomux_config(void)
{
    /* I2C3 M2 SCL-3D1 SDA-3D2 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3,
                         GPIO_PIN_D1 | GPIO_PIN_D2,
                         PIN_CONFIG_MUX_FUNC3);

    /* pull none smt */
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_D1 | GPIO_PIN_D2,
                         PIN_CONFIG_PUL_NORMAL |
                         PIN_CONFIG_SMT_ENABLE);
}

/**
 * @brief  Config iomux m2 for I2C4
 */
RT_WEAK RT_UNUSED void i2c4_m2_iomux_config(void)
{
    /* I2C4 M2 SCL-3C7 SDA-3D0 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3,
                         GPIO_PIN_C7 | GPIO_PIN_D0,
                         PIN_CONFIG_MUX_FUNC3);

    /* pull none smt */
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_C7 | GPIO_PIN_D0,
                         PIN_CONFIG_PUL_NORMAL |
                         PIN_CONFIG_SMT_ENABLE);
}

/**
 * @brief  Config iomux m1 for PWM10
 */
RT_WEAK RT_UNUSED void pwm10_m1_iomux_config(void)
{
    /* PWM10 M1 1C6 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_C6,
                         PIN_CONFIG_MUX_FUNC3);
}

/**
 * @brief  Config iomux m1 for PWM11
 */
RT_WEAK RT_UNUSED void pwm11_m1_iomux_config(void)
{
    /* PWM11 M1 1C7 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_C7,
                         PIN_CONFIG_MUX_FUNC3);
}

/**
 * @brief  Config iomux m1 for UART0
 */
RT_WEAK RT_UNUSED void uart0_m1_iomux_config(void)
{
    /* UART0 M1 RX-2B0 TX-2B1 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK2,
                         GPIO_PIN_B0 |
                         GPIO_PIN_B1,
                         PIN_CONFIG_MUX_FUNC1);
}

/**
 * @brief  Config iomux m0 for UART2
 */
RT_WEAK RT_UNUSED void uart2_m0_iomux_config(void)
{
    /* UART2 M0 RX-3A3 TX-3A2 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3,
                         GPIO_PIN_A2 |
                         GPIO_PIN_A3,
                         PIN_CONFIG_MUX_FUNC2);
}

/**
 * @brief  Config iomux m1 for UART2
 */
RT_WEAK RT_UNUSED void uart2_m1_iomux_config(void)
{
    /* UART2 M1 RX-1B3 TX-1B2 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_B2 |
                         GPIO_PIN_B3,
                         PIN_CONFIG_MUX_FUNC2);
}

/**
 * @brief  Config iomux for RV1106
 */
RT_WEAK RT_UNUSED void rt_hw_iomux_config(void)
{
    uart2_m1_iomux_config();
}
