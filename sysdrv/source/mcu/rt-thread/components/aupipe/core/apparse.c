/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "apparse.h"

void print_obj(struct obj *obj)
{
    printf("###########\n");
    printf("obj:%s @ %p\n", obj->name, obj);
    printf("factory:%s\n", obj->factory);
    printf("para:%s\n", obj->para ? obj->para : "NULL");
    printf("sinks:%d\n", obj->sinks);
    if (obj->sinks) {
        for (int i = 0; i < obj->sinks; i++) {
            printf("  sink%d %s @ %p, ", i, obj->sink[i].name, obj->sink[i].obj);
            printf("bypass %s\n", obj->sink[i].bypass ? "true" : "false");
            if (!obj->sink[i].bypass) {
                printf("        map size %d, <", obj->sink[i].ch);
                for (int j = 0; j < obj->sink[i].ch - 1; j++) {
                    printf("%04x, ", obj->sink[i].ch_map[j]);
                }
                printf("%04x>\n", obj->sink[i].ch_map[obj->sink[i].ch - 1]);
            }
        }
    }
}

struct obj *find_sink(struct element* elements, int nums, char *sink)
{
    if (!sink)
        return NULL;
    for (int i = 0; i < nums; i++)
        if (!strcmp(elements[i].obj->name, sink))
            return elements[i].obj;

    printf("WARNING: lost sink %s\n", sink);
    return NULL;
}

static int format_number(double v, char *buf, int bufsz)
{
    char _s[128];
    char *s = _s;
    int l = sizeof(_s);

    if (buf) {
        s = buf;
        l = bufsz;
    }
    if ((v - (int)v) > 0)
        snprintf(s, l, "%f%c", v, '\0');
    else
        snprintf(s, l, "%d%c", (int)v, '\0');

    return strlen(s);
}

static char *format_parameter(cJSON *json)
{
    cJSON *_first = cJSON_GetObjectItem(json, "factory");
    cJSON *first = _first->next;
    char *para;
    char *_para;
    int para_size = 0;

    if (!first)
        return NULL;
    /* caculate the para buffer size */
    do {
        if (strcmp(first->string, "factory") &&
            strcmp(first->string, "sinks")) {
            para_size += strlen(first->string) + 1;
            if (cJSON_IsBool(first)) {
                para_size += cJSON_IsTrue(first) ? 4 : 5;
            } else if (cJSON_IsNumber(first)) {
                para_size += format_number(cJSON_GetNumberValue(first), NULL, 0);
            } else if (cJSON_IsString(first)) {
                para_size += strlen(cJSON_GetStringValue(first));
            } else {
                printf("skip type %x\n", first->type);
            }
        }
        first = first->next;
        if (!first || first == _first)
            break;
        para_size += 1;
    } while (1);

    _para = para = malloc(para_size + 1);
    first = _first->next;
    do {
        if (strcmp(first->string, "factory") &&
            strcmp(first->string, "sinks")) {
            if (first != _first->next) {
                snprintf(_para, para_size, ",");
                _para++;
                para_size--;
            }
            para_size += strlen(first->string) + 2;
            snprintf(_para, para_size, "%s=", first->string);
            _para += strlen(first->string) + 1;
            para_size -= strlen(first->string) + 1;
            if (cJSON_IsBool(first)) {
                snprintf(_para, para_size, "%s",
                         cJSON_IsTrue(first) ? "true" : "false");
                _para += 5;
                para_size -= 5;
            } else if (cJSON_IsNumber(first)) {
                int len = format_number(cJSON_GetNumberValue(first),
                                        _para, para_size);
                _para += len;
                para_size -= len;
            } else if (cJSON_IsString(first)) {
                int len = strlen(cJSON_GetStringValue(first));
                snprintf(_para, para_size, "%s", cJSON_GetStringValue(first));
                _para += len;
                para_size -= len;
            } else {
                printf("skip type %x\n", first->type);
            }
        }
        first = first->next;
        if (!first || first == _first)
            break;
    } while (1);

    return para;
}

static int parse_chmap(cJSON *json, struct sink *sink)
{
    cJSON *first = cJSON_GetObjectItem(json, "sink_ch");
    if (!first) {
        printf("no sink_ch in %s\n", json->string);
        return -1;
    }
    cJSON *_first = first->next;
    int i = 0;

    sink->ch = cJSON_GetNumberValue(first);

    if (!sink->bypass)
    {
        sink->ch_map = calloc(sink->ch, sizeof(uint16_t));

        do
        {
            if (!strncmp(_first->string, "binding.", 8)) {
                sink->ch_map[i++] = CH_MAP(atoi(_first->string + 8),
                                        (uint32_t)cJSON_GetNumberValue(_first));
            }
            _first = _first->next;
        } while (_first && (_first != first));
    }

    return 0;
}

struct obj *new_obj_from_json(cJSON *json)
{
    struct obj *obj;
    cJSON *sinks;
    cJSON *_sink;
    cJSON *sinkname;

    obj = calloc(1, sizeof(struct obj));
    obj->name = json->string;
    obj->factory = cJSON_GetStringValue(cJSON_GetObjectItem(json, "factory"));
    obj->para = format_parameter(json);
    if (sinks = cJSON_GetObjectItem(json, "sinks")) {
        obj->sinks = cJSON_GetArraySize(sinks);
        obj->sink = calloc(obj->sinks, sizeof(struct sink));
        for (int i = 0; i < obj->sinks; i++) {
            _sink = cJSON_GetArrayItem(sinks, i);
            sinkname = cJSON_GetObjectItem(_sink, "name");
            obj->sink[i].name = cJSON_GetStringValue(sinkname);
            obj->sink[i].bypass = cJSON_IsTrue(cJSON_GetObjectItem(_sink,
                                                                   "bypass"));

            parse_chmap(_sink, &obj->sink[i]);
        }
    }

    return obj;
}
