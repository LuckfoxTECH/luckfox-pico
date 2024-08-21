/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APPARSE_H__
#define __APPARSE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#define CH_MAP(src, sink)   (((src & 0xff) << 8) | ((sink) & 0xff))

struct obj;

struct sink {
    char *name;
    uint8_t bypass;
    uint8_t ch;
    uint16_t *ch_map;
    struct obj *obj;
};

struct obj {
    char *name;
    char *factory;
    char *para;
    uint8_t sinks;
    struct sink *sink;
};

struct element {
    cJSON *json;
    struct obj *obj;
};

void print_obj(struct obj *obj);
struct obj *new_obj_from_json(cJSON *json);
struct obj *find_sink(struct element* elements, int nums, char *sink);

#endif
