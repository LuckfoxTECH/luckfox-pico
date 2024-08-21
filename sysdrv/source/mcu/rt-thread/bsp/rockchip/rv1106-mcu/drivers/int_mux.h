/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    int_mux.h
  * @author  Jason Zhu
  * @version V0.1
  * @date    11-Jan-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef _INT_MUX_H_
#define _INT_MUX_H_

#include <rthw.h>

#define INT_MUX 0xFF5D0000
#define INT_MUX_GROUP 0x80
#define INT_GROUP 4

void rt_hw_interrupt_active(int vector);
void rt_hw_interrupt_deactive(int vector);
rt_uint32_t rt_hw_interrupt_get_active(void);

#endif
