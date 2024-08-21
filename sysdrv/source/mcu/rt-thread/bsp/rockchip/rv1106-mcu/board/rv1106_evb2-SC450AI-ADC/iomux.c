/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-16     HongLin Lan  first implementation
 */

#include "rtdef.h"
#include "iomux.h"
#include "hal_base.h"

/**
 * @brief  Config iomux for RV1106's camera
 */
static void cam_iomux_config(void)
{
    /* Config iomux gpio :MIPI_REFCLK_OUT0 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_C4, PIN_CONFIG_MUX_FUNC2);
}

/**
 * @brief  Config iomux for RV1106's gpio
 */
static void gpio_iomux_config(void)
{
    /* Config iomux gpio3A2 : Analog ambient light sensor enable gpio */
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_A2, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_A2, GPIO_LOW);
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_A2, PIN_CONFIG_MUX_FUNC0);
}

/**
 * @brief  Config iomux for RV1106
 */
void rt_hw_iomux_config(void)
{
    gpio_iomux_config();
    i2c4_m2_iomux_config();
    uart2_m1_iomux_config();

    cam_iomux_config();
}
