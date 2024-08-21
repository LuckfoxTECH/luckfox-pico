/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-08     Ziyuan Xu   first implementation
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

    /*
     * SC3338 can't change the i2c address by hardware, if the dual sc3338 were
     * connected to the same I2C controller. We have to pull low the reset-gpios,
     * so that initialize first camera can change the i2c address via register.
     */
    /*Config GPIO_PIN_C5-MIPI_RST0, GPIO_PIN_D1-MIPI_RST1 and GPIO_PIN_D2-MIPI_PDN0*/
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_C5 | GPIO_PIN_D1 | GPIO_PIN_D2, PIN_CONFIG_MUX_FUNC0);
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_C5 | GPIO_PIN_D1 | GPIO_PIN_D2, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_C5 | GPIO_PIN_D1 | GPIO_PIN_D2, 0);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK0, GPIO_PIN_A1, PIN_CONFIG_MUX_FUNC0);
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_A1, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_A1, 1);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK1, GPIO_PIN_B0, PIN_CONFIG_MUX_FUNC0);
    HAL_GPIO_SetPinDirection(GPIO1, GPIO_PIN_B0, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO1, GPIO_PIN_B0, 1);


}

/**
 * @brief  Config iomux for RV1106
 */
void rt_hw_iomux_config(void)
{
    i2c4_m2_iomux_config();
    uart2_m1_iomux_config();

    cam_iomux_config();
}
