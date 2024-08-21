/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_als.h
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-10-08     Ziyuan Xu   first implementation
  *
  ******************************************************************************
  */
#ifndef __DRV_ALS_H__
#define __DRV_ALS_H__

#define RK_DEVICE_CTRL_ALS_INIT             (0)
#define RK_DEVICE_CTRL_ALS_DATA_READY       (1)
#define RK_DEVICE_CTRL_ALS_GET_VAL          (2)

#ifdef RT_USING_ALS_LTR308
extern const struct als_desc als_ltr308;
#endif

struct als_desc
{
    char name[RT_NAME_MAX];
    char i2c_bus[RT_NAME_MAX];
    float window_factor;
    uint8_t i2c_addr;
};

#endif
