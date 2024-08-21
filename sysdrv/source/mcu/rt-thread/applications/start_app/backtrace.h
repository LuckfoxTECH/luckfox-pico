/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    backtrace.h
  * @version V0.1
  * @brief   backtrace interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-04-01     Cliff.Chen      first implementation
  *
  ******************************************************************************
  */

#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#if defined(RT_USING_CMBACKTRACE) && defined(PKG_USING_CMBACKTRACE)
#error the cmbacktrace of online packages has bug, do not use
#endif

#ifdef RT_USING_CMBACKTRACE
#include "cm_backtrace.h"

void dump_call_stack(void);

#define RT_BACKTRACE_ASSERT(EX)                   \
if (!EX)                                          \
{                                                 \
    cm_backtrace_assert(cmb_get_sp());              \
    while(1);                                     \
}
#else
#define RT_BACKTRACE_ASSERT(EX)
#endif

#endif
