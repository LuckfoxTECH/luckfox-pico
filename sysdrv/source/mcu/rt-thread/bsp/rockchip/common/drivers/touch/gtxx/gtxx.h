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

/* GTxx touch HW pin define, these pins must defined in board config */
#ifndef TOUCH_IRQ_PIN
#define TOUCH_IRQ_PIN   0
#endif

#ifndef TOUCH_RST_PIN
#define TOUCH_RST_PIN   0
#endif

#ifndef TOUCH_I2C_DEV
#define TOUCH_I2C_DEV   0
#endif

/* GTxx register define */
#define GTP_ADDR_LENGTH         (2)
#define GTP_CONFIG_MIN_LENGTH   (186)
#define GTP_CONFIG_MAX_LENGTH   (256)

#define GTP_MAX_TOUCH           (5)
#define GTP_POINT_INFO_NUM      (8)

#define GTP_ADDRESS_HIGH        (0x5D)
#define GTP_ADDRESS_LOW         (0x14)

#define GTP_COMMAND             (0x8040)
#define GTP_CONFIG              (0x8050)

#define GTP_PRODUCT_ID          (0x8140)
#define GTP_READ_STATUS         (0x814E)

#define GTP_POINT1_REG          (0x814F)
#define GTP_POINT2_REG          (0X8157)
#define GTP_POINT3_REG          (0X815F)
#define GTP_POINT4_REG          (0X8167)
#define GTP_POINT5_REG          (0X816F)

#define GTP_CHECK_SUM           (0X80FF)

#define CFG_GROUP_LEN(p_cfg_grp)  (sizeof(p_cfg_grp) / sizeof(p_cfg_grp[0]))

#endif
