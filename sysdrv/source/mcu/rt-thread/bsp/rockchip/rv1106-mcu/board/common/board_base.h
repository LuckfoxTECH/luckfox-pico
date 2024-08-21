/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-11     Jason Zhu    first implementation
 * 2022-07-08     Steven Liu   Add rt_hw_iomux_config
 */

#ifndef __BOARD_BASE_H__
#define __BOARD_BASE_H__

#include "drv_uart.h"
#include "int_mux.h"
#include "riscv_csr_encoding.h"
#include "timer.h"
#include "hal_base.h"

#ifdef RT_USING_CAMERA
#include "board_cam.h"
#endif

#define MAX_HANDLERS       NUM_INTERRUPTS

extern  void *_end;
extern  void *_heap_end;
#define HEAP_BEGIN  &_end
#define HEAP_END    &_heap_end

#define INTERRUPT_SEPERATE 8

#define CAMERA_PWDN_GPIO_BANK               GPIO_BANK1
#define CAMERA_PWDN_GPIO_GROUP              GPIO1
#define CAMERA_PWDN_GPIO_PIN                GPIO_PIN_D4

#define CAMERA_RST_GPIO_BANK                GPIO_BANK3
#define CAMERA_RST_GPIO_GROUP               GPIO3
#define CAMERA_RST_GPIO_PIN                 GPIO_PIN_C5

#define CAMERA_MCLK_GPIO_BANK               GPIO_BANK2
#define CAMERA_MCLK_GPIO_GROUP              GPIO2
#define CAMERA_MCLK_GPIO_PIN                GPIO_PIN_A3

#define CAMERA_PW_EN_BANK                   GPIO_BANK3
#define CAMERA_PW_EN_GROUP                  GPIO3
#define CAMERA_PW_EN_GPIO_PIN               GPIO_PIN_A6

#define CAMERA_CLK_REF                      CLK_REF_MIPI0

#define IRCUT_ENB_GPIO_BANK                 GPIO_BANK2
#define IRCUT_ENB_GPIO_GROUP                GPIO2
#define IRCUT_ENB_GPIO_PIN                  GPIO_PIN_D4

#define IRCUT_FBC_GPIO_BANK                 GPIO_BANK2
#define IRCUT_FBC_GPIO_GROUP                GPIO2
#define IRCUT_FBC_GPIO_PIN                  GPIO_PIN_D5

#define IR_LED_EN_GPIO_BANK                 GPIO_BANK2
#define IR_LED_EN_GPIO_GROUP                GPIO2
#define IR_LED_EN_GPIO_PIN                  GPIO_PIN_B2

#define IR_LED_PWM_GPIO_BANK                GPIO_BANK2
#define IR_LED_PWM_GPIO_GROUP               GPIO2
#define IR_LED_PWM_GPIO_PIN                 GPIO_PIN_A6

#define RED_LED_PWM_GPIO_BANK               GPIO_BANK2
#define RED_LED_PWM_GPIO_GROUP              GPIO2
#define RED_LED_PWM_GPIO_PIN                GPIO_PIN_B1

#define WHITE_LED_EN_GPIO_BANK              GPIO_BANK2
#define WHITE_LED_EN_GPIO_GROUP             GPIO2
#define WHITE_LED_EN_GPIO_PIN               GPIO_PIN_B3

#define WHITE_LED_PWM_GPIO_BANK             GPIO_BANK2
#define WHITE_LED_PWM_GPIO_GROUP            GPIO2
#define WHITE_LED_PWM_GPIO_PIN              GPIO_PIN_A7

#define BLUE_LED_PWM_GPIO_BANK              GPIO_BANK2
#define BLUE_LED_PWM_GPIO_GROUP             GPIO2
#define BLUE_LED_PWM_GPIO_PIN               GPIO_PIN_B0

#define LIGHTS_SENSOR_ADC_DEVICE_NAME       "rk_adc0"
#define LIGHTS_SENSOR_ADC_DEVICE_CH         2
#define SENSOR_DEVICE_NAME                  "sensor_0"

void rt_hw_board_init(void);
rt_bool_t sirq_status(void);

#endif
