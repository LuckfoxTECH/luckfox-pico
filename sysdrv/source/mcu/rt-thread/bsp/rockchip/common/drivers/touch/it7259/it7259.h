/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_display.c
  * @version V0.1
  * @brief   display driver for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-11-04     Chris.Zhong     the first version
  *
  ******************************************************************************
  */

#ifndef __IT7259_H_
#define __IT7259_H_
#include <rtthread.h>
#include "touch.h"

/* IT7259 touch HW pin define, these pins must defined in board config */
#ifndef TOUCH_IRQ_PIN
#define TOUCH_IRQ_PIN   0
#endif

#ifndef TOUCH_RST_PIN
#define TOUCH_RST_PIN   0
#endif

#ifndef TOUCH_I2C_DEV
#define TOUCH_I2C_DEV   0
#endif

/* register address  */
#define COMMAND_BUFFER_INDEX          0x20
#define QUERY_BUFFER_INDEX            0x80
#define COMMAND_RESPONSE_BUFFER_INDEX 0xA0
#define POINT_BUFFER_INDEX            0xE0

#define TOUCH_SLAVE_ADDRESS     0x46

#define FTS_MAX_TOUCH_POINT     1
#ifndef RT_TOUCH_X_RANGE
#define RT_TOUCH_X_RANGE        454
#endif
#ifndef RT_TOUCH_Y_RANGE
#define RT_TOUCH_Y_RANGE        454
#endif

#endif
