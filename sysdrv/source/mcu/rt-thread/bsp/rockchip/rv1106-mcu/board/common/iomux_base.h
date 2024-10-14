/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-08     Steven Liu   first implementation
 */

#ifndef __IOMUX_BASE_H__
#define __IOMUX_BASE_H__

void i2c0_m2_iomux_config(void);
void i2c3_m1_iomux_config(void);
void i2c3_m2_iomux_config(void);
void i2c4_m2_iomux_config(void);
void pwm10_m1_iomux_config(void);
void pwm11_m1_iomux_config(void);
void uart0_m1_iomux_config(void);
void uart2_m0_iomux_config(void);
void uart2_m1_iomux_config(void);
void rt_hw_iomux_config(void);

#endif
