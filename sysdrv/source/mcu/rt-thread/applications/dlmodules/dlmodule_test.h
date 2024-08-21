/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __DLMODULE_TEST_H__
#define __DLMODULE_TEST_H__
#include <rtthread.h>

/* dl_event define */
#define EVENT_DL_EXIT          (0x01UL << 0)
#define EVENT_DL_EXITED        (0x01UL << 1)

extern rt_event_t dl_event;
#endif
