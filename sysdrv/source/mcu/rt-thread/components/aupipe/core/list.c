/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "PLIST"

void aupipe_list_init(ApList *list)
{
    list->next = list->last = list;
}

void aupipe_list_insert_head(ApList *list, ApList *new)
{
    new->next = list->next;
    new->last = list;
    list->next->last = new;
    list->next = new;
}

void aupipe_list_append_tail(ApList *list, ApList *new)
{
    new->last = list->last;
    new->next = list;
    list->last->next = new;
    list->last = new;
}

int aupipe_list_is_empty(ApList *list)
{
    return ((list->next == list) && (list->last == list));
}

ApList *aupipe_list_pop_tail(ApList *list)
{
    ApList *tail;

    tail = list->last;

    tail->last->next = tail->next;
    tail->next->last = tail->last;

    return tail;
}

