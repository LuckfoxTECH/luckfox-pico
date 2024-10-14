/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __ECNR_H__
#define __ECNR_H__

#if defined(RT_USING_RPMSG_CMD) && defined(PRIMARY_CPU) && defined(RT_USING_AUPIPE_ECNR)

enum
{
    ECNR_INIT = 0,
    ECNR_DEINIT,
    ECNR_PROCESS,
    ECNR_RESETARG,
};

void ecnr_ept_cb(void *param);

#endif

#endif
