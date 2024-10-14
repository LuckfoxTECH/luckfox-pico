/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-08     HongLin Lan   first implementation
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
    /* Config iomux gpio :MIPI_REFCLK_OUT1 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_C6, PIN_CONFIG_MUX_FUNC3);

    /* Config GPIO_PIN_C5-MIPI_RST0, GPIO_PIN_D1-MIPI_RST1 and GPIO_PIN_D2-MIPI_PDN0 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_C5 | GPIO_PIN_D1 | GPIO_PIN_D2, PIN_CONFIG_MUX_FUNC0);
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_C5 | GPIO_PIN_D1 | GPIO_PIN_D2, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_C5 | GPIO_PIN_D1 | GPIO_PIN_D2, 0);
}

/**
 * @brief  Config iomux for RV1106
 */
void rt_hw_iomux_config(void)
{
    i2c4_m2_iomux_config();
    uart2_m0_iomux_config();

    cam_iomux_config();
}
