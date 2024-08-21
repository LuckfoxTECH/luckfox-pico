/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    lvgl_clock.h
  * @version V0.1
  * @brief   clock display
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-09-02     Tony.zheng      first implementation
  *
  ******************************************************************************
  */

#ifndef __LVGL_CLOCK_H__
#define __LVGL_CLOCK_H__
#include <rtthread.h>

/**
 * Callback for app to display text content.
 */
rt_err_t lvgl_clock_content_update_callback(uint32_t cmd, void *parm, uint32_t parmlen);

#endif
