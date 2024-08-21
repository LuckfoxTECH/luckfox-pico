/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_keyctrl.h
  * @author  Simon Xue
  * @version V0.1
  * @date    19-Jun-2019
  * @brief   keyctrl driver
  *
  ******************************************************************************
  */
#ifndef __KEYCTRL_H__
#define __KEYCTRL_H__

#include <rtthread.h>

#define RT_KEYCTRL_CTRL_GET_INFO                     (1)

struct rt_keyctrl_info
{
    rt_uint32_t    count;                   /* Buffer count */
};

#endif
