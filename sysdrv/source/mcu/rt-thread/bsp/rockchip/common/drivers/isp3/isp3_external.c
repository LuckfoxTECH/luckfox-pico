/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp3_external.c
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */

#include "isp3_external.h"

#if defined(RT_USING_ISP3)

/**
 * @brief  enable clk.
 * @param gate : The clock gate to enable.
 */
rt_err_t rk_clk_enable1(rk_clk_gate *gate)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return clk_enable(gate);
#else
    return -RT_EEMPTY;
#endif
#elif defined(__RK_OS__)
    return ClkEnable(gate);
#endif
}

/**
 * @brief  disable clk.
 * @param gate : The clock gate to disable.
 */
rt_err_t rk_clk_disable1(rk_clk_gate *gate)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return clk_disable(gate);
#else
    return -RT_EEMPTY;
#endif
#elif defined(__RK_OS__)
    return ClkDisable(gate);
#endif
}

#endif
