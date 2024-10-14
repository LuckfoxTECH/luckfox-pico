
/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp_interface.h
  * @author  ISP_TEAM
  * @version V0.0.1
  * @date
  * @brief   image signal processing (ISP) interface
  *
  ******************************************************************************
  */
#ifndef __ISP_INTERFACE_H__
#define __ISP_INTERFACE_H__

/* Includes ------------------------------------------------------------------*/
#ifdef __RT_THREAD__
#include <rthw.h>
#include <rtthread.h>
#include "rtconfig.h"
#endif

#if defined(RT_USING_ISP)

extern rt_event_t event_isp;

#define EVENT_3A_FLAG (0x625aaa)

rt_err_t rt_do_rkisp_create_aaa_event();

#endif

#endif

