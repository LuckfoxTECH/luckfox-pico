/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp3_external.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef __ISP3_EXTERNAL_H_
#define __ISP3_EXTERNAL_H_

#include "drv_isp_module.h"
#include "drv_isp3.h"

#if defined(RT_USING_ISP3)

typedef struct
{
    char name[12];
    int clk_gate;
} rk_clk_gate1;

typedef struct clk_gate                         rk_clk_gate;

rt_err_t rk_clk_enable1(rk_clk_gate *gate);
rt_err_t rk_clk_disable1(rk_clk_gate *gate);

#endif

#endif
