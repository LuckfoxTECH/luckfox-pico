/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    scr1_specific.h
  * @author  Jason Zhu
  * @version V0.1
  * @date    11-Jan-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef __SCR1__SPECIFIC
#define __SCR1__SPECIFIC

#define mcounten    0x7E0
#define mtime_ctrl  0x00490000
#define mtime_div   0x00490004
#define mtime       0x00490008
#define mtimeh      0x0049000C
#define mtimecmp    0x00490010
#define mtimecmph   0x00490014

#define SCR1_MTIME_CTRL_EN      0
#define SCR1_MTIME_CTRL_CLKSRC      1

#define SCR1_MTIME_CTRL_WR_MASK     0x3
#define SCR1_MTIME_DIV_WR_MASK      0x3FF

#endif
