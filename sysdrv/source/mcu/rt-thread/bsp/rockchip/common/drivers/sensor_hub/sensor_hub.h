/**
  * Copyright (c) 2021 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    sensor_hub.h
  * @version V0.1
  * @brief   sensor_hub head file
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-01-04     Chris Zhong     first implementation
  *
  ******************************************************************************
  */
#ifndef __SENSOR_HUB_H__
#define __SENSOR_HUB_H__

#define HCI_CONTROL_GROUP_MISC                                0xFF

#define HCI_CONTROL_MISC_COMMAND_PING                       ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x01 )    /* Ping controller */
#define HCI_CONTROL_MISC_COMMAND_GET_VERSION                ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x02 )    /* Get SDK Version */
#define HCI_CONTROL_MISC_COMMAND_GET_RTC_TIME               ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x03 )    /* Get RTC time */
#define HCI_CONTROL_MISC_COMMAND_SET_RTC_TIME               ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x04 )    /* Set RTC time */
#define HCI_CONTROL_MISC_COMMAND_GET_SYS_TICK               ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x05 )    /* Get system tick */
#define HCI_CONTROL_MISC_COMMAND_SET_TIMER                  ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x06 )    /* Set system timeout */
#define HCI_CONTROL_MISC_COMMAND_GET_EVENT                  ( ( HCI_CONTROL_GROUP_MISC << 8 ) | 0x07 )    /* Set wakeup event */

typedef struct
{
    /// seconds (0 - 59), not support leap seconds
    uint16_t  second;

    /// minutes (0 - 59),
    uint16_t  minute;

    /// hours (0 - 23)
    uint16_t  hour;

    /// day of the month (1 - 31)
    uint16_t  day;

    /// month (0 - 11, 0=January)
    uint16_t  month;

    /// year
    /// should larger then 2010
    uint16_t  year;

} RtcTime;

#endif /* __SENSOR_HUB_H__ */