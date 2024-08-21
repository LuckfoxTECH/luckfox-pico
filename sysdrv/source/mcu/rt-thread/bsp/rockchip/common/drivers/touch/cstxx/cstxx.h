/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-01     tyustli     the first version
 */

#ifndef __GTXX_H_
#define __GTXX_H_
#include <rtthread.h>
#include "touch.h"

/* CSTxx touch HW pin define, these pins must defined in board config */
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
#define FTS_REG_CHIP_ID         0xA7    //chip ID
#define FTS_REG_LOW_POWER       0xE5    //low power
#define FTS_REG_FW_VER          0xA5    //FW version
#define FTS_REG_WENDOR_ID       0xAA    //TP vendor ID
#define FTS_REG_POINT_RATE      0x88    //report rate

#define CHIP_59_ENTER_SLEEP     0x03
#define CHIP_OTHER_ENTER_SLEEP      0x03

#define TOUCH_SLAVE_ADDRESS     0x15

#define FTS_MAX_TOUCH_POINT     1
#ifndef RT_TOUCH_X_RANGE
#define RT_TOUCH_X_RANGE        240
#endif
#ifndef RT_TOUCH_Y_RANGE
#define RT_TOUCH_Y_RANGE        240
#endif

#endif
