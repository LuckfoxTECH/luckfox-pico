/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_wdt.h
  * @author  Simon Xue
  * @version V0.1
  * @date    19-Jun-2019
  * @brief   watchdog driver
  *
  ******************************************************************************
  */
#ifndef __KEYCTRL_H__
#define __KEYCTRL_H__

#include <rtthread.h>

rt_err_t dw_wdt_set_top(int top_msec);

#endif
