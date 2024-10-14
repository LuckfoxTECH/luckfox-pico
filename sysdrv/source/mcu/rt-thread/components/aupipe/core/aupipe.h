/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __AUPIPE_H__
#define __AUPIPE_H__

#include <math.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DBG_SECTION_NAME    "AUPIPE"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"
#include "apparse.h"

#define AUPIPE_DEFAULT_RATE         48000
#define AUPIPE_DEFAULT_BITS         16
#define AUPIPE_DEFAULT_PERIOD_SIZE  64

typedef struct aupipe Aupipe;
typedef struct aupipe_object ApObject;
typedef struct aupipe_pad ApPad;
typedef struct aupipe_pad_link ApPadLink;
typedef struct aupipe_event ApEvent;
typedef struct aupipe_event_msg ApEventMsg;
typedef struct aupipe_buffer ApBuffer;
typedef struct aupipe_property ApProperty;
typedef struct aupipe_value ApValue;
typedef struct aupipe_link ApLink;
typedef struct aupipe_audio_link ApAudioLink;
typedef struct aupipe_list ApList;
typedef struct aupipe_tree ApTree;

typedef int (*object_init)(ApObject *obj, void *arg);
typedef int (*object_deinit)(ApObject *obj);
typedef int (*object_set_state)(ApObject *obj, int state);
typedef int (*object_set_property)(ApObject *obj, char *name, void *arg);
typedef int (*object_get_property)(ApObject *obj, char *name, void *arg);
typedef ApPad *(*object_get_pad)(ApObject *obj, int type, int id);

typedef int (*poll_func)(ApObject *obj);

typedef int (*pad_process)(ApPad *pad, ApBuffer *buffer);

#define OBJECT_BASE_DEFINE(base_name, type)                             \
ApObject base_name##_base = {                                           \
    .name = #base_name,                                                 \
    .init = base_name##_init,                                           \
    .deinit = base_name##_deinit,                                       \
    .set_state = base_name##_set_state,                                 \
    .set_property = base_name##_set_property,                           \
    .get_property = base_name##_get_property,                           \
    .get_pad = base_name##_get_pad,                                     \
};                                                                      \
static int base_name##_base_register(void)                              \
{                                                                       \
    return aupipe_object_register(&base_name##_base, sizeof(type));     \
}                                                                       \
INIT_ENV_EXPORT(base_name##_base_register);

enum AUPIPE_STATE
{
    AUPIPE_NULL       = 0x1,
    AUPIPE_READY      = 0x2,
    AUPIPE_PAUSED     = 0x4,
    AUPIPE_PLAYING    = 0x8,
};

#define STATE_CHANGE(a, b)      (((uint32_t)(a) << 16) | (uint32_t)(b))
#define STATE_NULL_TO_READY     STATE_CHANGE(AUPIPE_NULL, AUPIPE_READY)
#define STATE_READY_TO_PAUSED   STATE_CHANGE(AUPIPE_READY, AUPIPE_PAUSED)
#define STATE_PAUSED_TO_PLAYING STATE_CHANGE(AUPIPE_PAUSED, AUPIPE_PLAYING)
#define STATE_PLAYING_TO_PAUSED STATE_CHANGE(AUPIPE_PLAYING, AUPIPE_PAUSED)
#define STATE_PAUSED_TO_READY   STATE_CHANGE(AUPIPE_PAUSED, AUPIPE_READY)
#define STATE_READY_TO_NULL     STATE_CHANGE(AUPIPE_READY, AUPIPE_NULL)

enum VALUE_TYPE
{
    VALUE_TYPE_INT,
    VALUE_TYPE_FLOAT,
    VALUE_TYPE_STRING,
    VALUE_TYPE_POINTER,
};

enum DATE_TYPE
{
    DATA_TYPE_AUDIO,
};

enum PAD_TYPE
{
    PAD_TYPE_SRC,
    PAD_TYPE_SINK,
};

enum EVENT_TYPE
{
    EVENT_TYPE_EOS,
    EVENT_TYPE_PAUSE,
    EVENT_TYPE_RESUME,
    EVENT_TYPE_STOP,
    EVENT_TYPE_ERROR,
};

struct aupipe_value
{
    int type;
    union
    {
        int int_v;
        float float_v;
        char *str_v;
        void *pointer_v;
    } v;
};

struct aupipe_property
{
    char name[16];
    ApValue value;
};

struct aupipe_buffer
{
    int type;
    void *data;
    size_t len;
    size_t ref;
    rt_mutex_t ref_lock;
};

struct aupipe_link
{
    int nums;
    int *type;
    void **desc;
};

struct aupipe_pad_link
{
    int sink;
    int src;
};

struct aupipe_audio_link
{
    int in_rate;
    int out_rate;
    int in_bits;
    int out_bits;
    int in_channels;
    int out_channels;
    uint16_t *channel_map;
};

struct aupipe_pad
{
    ApObject *parent;
    ApPad *peer;
    pad_process process;
    enum PAD_TYPE type;

    int id;
    void *priv_data;
};

struct aupipe_tree
{
    int chan;
    int *branch;
};

struct aupipe_object
{
    char name[16];
    Aupipe *parent;
    object_init init;
    object_deinit deinit;
    object_set_state set_state;
    object_set_property set_property;
    object_get_property get_property;
    object_get_pad get_pad;

    ApLink *src_links;
    ApLink *sink_links;
};

struct aupipe_list
{
    ApList *last;
    ApList *next;
    void *data;
};

struct aupipe_event_msg
{
    int type;
    int need_free;
};

struct aupipe_event
{
    ApList msg;
    rt_mutex_t lock;
    rt_sem_t sem;
    int wait_cnt;
};

struct aupipe
{
    ApList objs;
    ApEvent *event;
    uint32_t state;
    uint32_t state_change;

    uint32_t rate;
    uint16_t bits;
    uint16_t period_size;
};

int aupipe_init(void);
Aupipe *aupipe_new(void);
ApEventMsg *aupipe_wait_event(Aupipe *aupipe);
int aupipe_new_event(Aupipe *aupipe, ApEventMsg *msg);
int aupipe_set_state(Aupipe *aupipe, uint32_t state);
void aupipe_delete(Aupipe *aupipe);
int aupipe_add_object(Aupipe *aupipe, ApObject *obj, ...);
ApObject *aupipe_find_object(Aupipe *aupipe, char *name);
int aupipe_link_object(Aupipe *aupipe, struct obj *object);
int aupipe_create_link(Aupipe *aupipe, char *args);
int aupipe_delete_link(Aupipe *aupipe);
int aupipe_find_property(char *src, char *sub, int type, void *args);
void aupipe_list_object_template(void);

ApObject *aupipe_object_new(char *temp_name, char *name);
void aupipe_object_delete(ApObject *obj);
int aupipe_object_register(ApObject *obj, size_t obj_size);

void aupipe_value_set(ApValue *value, void *arg);
void aupipe_value_get(ApValue *value, void *arg);

ApBuffer *aupipe_buffer_new(int type, size_t size);
ApBuffer *aupipe_buffer_ref(ApBuffer *buffer);
void aupipe_buffer_unref(ApBuffer *buffer);

ApPad *aupipe_pad_new(ApObject *parent, int num);
int aupipe_pad_push(ApPad *pad, ApBuffer *buffer);

ApEvent *aupipe_event_new(void);
int aupipe_event_push(ApEvent *event, ApEventMsg *msg);
ApEventMsg *aupipe_event_pull(ApEvent *event);
void aupipe_event_delete(ApEvent *event);

void aupipe_list_init(ApList *list);
void aupipe_list_insert_head(ApList *list, ApList *new);
int aupipe_list_is_empty(ApList *list);
ApList *aupipe_list_pop_tail(ApList *list);

ApLink *aupipe_link_new(int nums, int *type);
void aupipe_link_delete(ApLink *link);

#endif

