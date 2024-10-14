/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

ApLink *aupipe_link_new(int nums, int *type)
{
    ApLink *link = rt_malloc(sizeof(ApLink));
    if (!link)
        goto null_link;

    link->nums = nums;
    link->type = rt_malloc(sizeof(int) * nums);
    if (!link->type)
        goto null_type;
    link->desc = rt_malloc(sizeof(void *) * nums);
    if (!link->desc)
        goto null_desc;
    memset(link->desc, 0, sizeof(void *) * nums);

    for (int i = 0; i < nums; i++)
    {
        void *link_desc;
        switch (type[i])
        {
        case DATA_TYPE_AUDIO:
            link_desc = rt_malloc(sizeof(ApAudioLink));
            break;
        default:
            link_desc = NULL;
            break;
        }
        if (!link_desc)
            goto null_link_desc;
        link->type[i] = type[i];
        link->desc[i] = link_desc;
    }

    return link;

null_link_desc:
    for (int i = 0; i < nums; i++)
    {
        if (link->desc[i])
            rt_free(link->desc[i]);
    }
    rt_free(link->desc);
null_desc:
    rt_free(link->type);
null_type:
    rt_free(link);
null_link:

    return NULL;
}

void aupipe_link_delete(ApLink *link)
{
    if (!link)
        return;

    for (int i = 0; i < link->nums; i++)
    {
        if (link->desc[i])
            rt_free(link->desc[i]);
    }
    rt_free(link->desc);
    rt_free(link->type);
    rt_free(link);
}

