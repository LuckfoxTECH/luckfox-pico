/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "PPAD"

ApPad *aupipe_pad_new(ApObject *parent, int num)
{
    ApPad *pad = rt_malloc(sizeof(ApPad) * num);
    if (!pad)
        return NULL;
    memset(pad, 0x0, sizeof(ApPad) * num);

    for (int i = 0; i < num; i++)
    {
        pad[i].id = i;
        pad[i].parent = parent;
    }

    return pad;
}

int aupipe_pad_push(ApPad *pad, ApBuffer *buffer)
{
    ApPad *peer;

    peer = pad->peer;
    if (!peer)
        return -1;
    return peer->process(peer, buffer);
}

