/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-01     tyustli     the first version
 */

#ifndef __S3706_H_
#define __S3706_H_
#include <rtthread.h>
#include "touch.h"

/* S3706 touch HW pin define, these pins must defined in board config */
#ifndef TOUCH_IRQ_PIN
#define TOUCH_IRQ_PIN   0
#endif

#ifndef TOUCH_RST_PIN
#define TOUCH_RST_PIN   0
#endif

#ifndef TOUCH_I2C_DEV
#define TOUCH_I2C_DEV   0
#endif

struct point_info
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint8_t  width_major;
    uint8_t  touch_major;
    uint8_t  status;
    uint8_t  finger_id;
};

typedef struct
{
    uint8_t F01_RMI_QUERY_BASE;
    uint8_t F01_RMI_CMD_BASE;
    uint8_t F01_RMI_CTRL_BASE;
    uint8_t F01_RMI_DATA_BASE;

    uint8_t F01_RMI_QUERY11;
    uint8_t F01_RMI_DATA01;
    uint8_t F01_RMI_CMD00;
    uint8_t F01_RMI_CTRL00;
    uint8_t F01_RMI_CTRL01;
    uint8_t F01_RMI_CTRL02;

    uint8_t F12_2D_QUERY_BASE;
    uint8_t F12_2D_CMD_BASE;
    uint8_t F12_2D_CTRL_BASE;
    uint8_t F12_2D_DATA_BASE;

    uint8_t F12_2D_CTRL08;
    uint8_t F12_2D_CTRL11;
    uint8_t F12_2D_CTRL20;
    uint8_t F12_2D_CTRL23;
    uint8_t F12_2D_CTRL27;
    uint8_t F12_2D_CTRL32;
    uint8_t F12_2D_DATA04;
    uint8_t F12_2D_DATA15;
    uint8_t F12_2D_DATA38;
    uint8_t F12_2D_DATA39;
    uint8_t F12_2D_CMD00;

    uint8_t F34_FLASH_QUERY_BASE;
    uint8_t F34_FLASH_CMD_BASE;
    uint8_t F34_FLASH_CTRL_BASE;
    uint8_t F34_FLASH_DATA_BASE;

    uint8_t SynaF34_FlashControl;
    uint8_t SynaF34Reflash_BlockNum;
    uint8_t SynaF34Reflash_BlockData;
    uint8_t SynaF34ReflashQuery_BootID;
    uint8_t SynaF34ReflashQuery_FlashPropertyQuery;
    uint8_t SynaF34ReflashQuery_FirmwareBlockSize;
    uint8_t SynaF34ReflashQuery_FirmwareBlockCount;
    uint8_t SynaF34ReflashQuery_ConfigBlockSize;
    uint8_t SynaF34ReflashQuery_ConfigBlockCount;

    uint8_t F51_CUSTOM_QUERY_BASE;
    uint8_t F51_CUSTOM_CMD_BASE;
    uint8_t F51_CUSTOM_CTRL_BASE;
    uint8_t F51_CUSTOM_DATA_BASE;

    uint8_t F51_CUSTOM_CTRL00;
    uint8_t F51_CUSTOM_CTRL31;
    uint8_t F51_CUSTOM_CTRL50;
    uint8_t F51_CUSTOM_CTRL04_05;
    uint8_t F51_CUSTOM_CTRL04_06;
    uint8_t F51_CUSTOM_CTRL04_08;
    uint8_t F51_CUSTOM_CTRL13; //Debug enable reg
    uint8_t F51_CUSTOM_CTRL20; //gesture for fingerprint
    uint8_t F51_CUSTOM_DATA;

    uint8_t F54_ANALOG_QUERY_BASE;
    uint8_t F54_ANALOG_COMMAND_BASE;
    uint8_t F54_ANALOG_CONTROL_BASE;
    uint8_t F54_ANALOG_DATA_BASE;

    uint8_t F55_SENSOR_CTRL01;
    uint8_t F55_SENSOR_CTRL02;
} s3706_reg_t;

typedef enum IRQ_TRIGGER_REASON
{
    IRQ_IGNORE      = 0x00,
    IRQ_TOUCH       = 0x01,
    IRQ_GESTURE     = 0x02,
    IRQ_BTN_KEY     = 0x04,
    IRQ_EXCEPTION   = 0x08,
    IRQ_FW_CONFIG   = 0x10,
    IRQ_FW_HEALTH   = 0x20,
    IRQ_FW_AUTO_RESET = 0x40,
    IRQ_FACE_STATE    = 0x80,
    IRQ_FINGERPRINT   = 0x0100,
} irq_reason;

/* bit operation */
#define TP_SET_BIT(data, flag) ((data) |= (flag))
#define TP_CLR_BIT(data, flag) ((data) &= ~(flag))
#define TP_CHK_BIT(data, flag) ((data) & (flag))

#ifndef min  //mod by prife
#define min(x,y) (x<y?x:y)
#endif
#ifndef max
#define max(x,y) (x<y?y:x)
#endif

#define S3706_ADDR          (0x20)
#define MAX_FINGER_NUM      (0x0A)

#define POINT_INFO_LEN      (sizeof(struct point_info))
#define MQ_MEM_POOL_LEN     (50*POINT_INFO_LEN)

typedef struct
{
    struct rt_touch_device    touch_dev;
    struct rt_i2c_client     *i2c_client;
    struct rt_i2c_bus_device *i2c_bus;
    s3706_reg_t reg;

    rt_sem_t    irq_sem;
    rt_mutex_t  read_mutex;

    char   point_buf[8 * MAX_FINGER_NUM];
    struct point_info points[MAX_FINGER_NUM];
    rt_uint32_t obtain_fingers;

} s3706_device_t;

#endif
