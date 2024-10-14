/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APPMSG_H__
#define __APPMSG_H__

#include "ecnr.h"
#include "aupipe.h"

#if defined(RT_USING_RPMSG_CMD) && defined(PRIMARY_CPU)

#include "rpmsg_cmd.h"

enum
{
    RPMSG_ECN = 0,
    RPMSG_MAX,
};

static struct rpmsg_cmd_table_t rpmsg_ept_table[RPMSG_MAX] =
{
    { RPMSG_ACK_GET_ECN_USAGE, ecnr_ept_cb },
};
static struct rpmsg_ept_handle_t rpmsg_ept_handle[RPMSG_MAX];

#endif

#endif
