/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

struct obj_template
{
    ApObject *template;
    int ref;
    size_t size;
};

struct object_factory
{
    struct obj_template *objs;
    rt_mutex_t lock;
    size_t size;
    size_t count;
};

static struct object_factory *factory = NULL;

int aupipe_init(void)
{
    factory = rt_malloc(sizeof(struct object_factory));
    if (!factory)
    {
        LOG_E("malloc factory failed");
        goto new_factory_err;
    }
    factory->lock = rt_mutex_create("factory", RT_IPC_FLAG_FIFO);
    if (!factory->lock)
    {
        LOG_E("create factory lock failed");
        goto new_factory_lock_err;
    }
    factory->size = 5;
    factory->count = 0;
    factory->objs = rt_malloc(sizeof(struct obj_template) * factory->size);
    if (!factory->objs)
    {
        LOG_E("malloc failed");
        goto new_factory_objs_err;
    }

    return RT_EOK;

new_factory_objs_err:
    rt_mutex_delete(factory->lock);
new_factory_lock_err:
    rt_free(factory);
    factory = NULL;
new_factory_err:

    return -RT_ERROR;
}
INIT_COMPONENT_EXPORT(aupipe_init);

Aupipe *aupipe_new(void)
{
    Aupipe *aupipe = rt_malloc(sizeof(Aupipe));
    if (!aupipe)
    {
        LOG_E("Create aupipe failed");
        goto null_aupipe;
    }
    memset(aupipe, 0, sizeof(Aupipe));

    aupipe->event = aupipe_event_new();
    if (!aupipe->event)
    {
        LOG_E("Create aupipe event failed");
        goto null_event;
    }

    aupipe_list_init(&aupipe->objs);
    aupipe->state = AUPIPE_NULL;
    aupipe->rate  = AUPIPE_DEFAULT_RATE;
    aupipe->bits  = AUPIPE_DEFAULT_BITS;
    aupipe->period_size = AUPIPE_DEFAULT_PERIOD_SIZE;

    return aupipe;

null_event:
    rt_free(aupipe);
null_aupipe:
    return NULL;
}

static int call_obj_set_state(ApObject *obj)
{
    Aupipe *aupipe = obj->parent;
    return obj->set_state(obj, aupipe->state_change);
}

extern void aupipe_dump_list(ApList *list);
static int aupipe_poll_objs(Aupipe *aupipe, poll_func func)
{
    ApList *list = &aupipe->objs;
    ApList *last = aupipe->objs.last;

    do
    {
        list = list->next;
        if (func((ApObject *)list->data) != RT_EOK)
        {
            return -RT_ERROR;
        }
    }
    while (list != last);

    return RT_EOK;
}

ApEventMsg *aupipe_wait_event(Aupipe *aupipe)
{
    return aupipe_event_pull(aupipe->event);
}

int aupipe_new_event(Aupipe *aupipe, ApEventMsg *msg)
{
    return aupipe_event_push(aupipe->event, msg);
}

int aupipe_set_state(Aupipe *aupipe, uint32_t state)
{
    uint32_t new_state;
    uint32_t state_change;

    if (aupipe->state != state)
    {
        if (aupipe->state < state)
        {
            do
            {
                new_state = aupipe->state << 1;
                LOG_I("%s %x => %x, %x", __func__, aupipe->state, new_state, state);
                state_change = STATE_CHANGE(aupipe->state, new_state);
                aupipe->state_change = state_change;
                if (aupipe_poll_objs(aupipe, call_obj_set_state) != RT_EOK)
                    return -RT_ERROR;
                aupipe->state = new_state;
            }
            while (aupipe->state < state);
        }
        else
        {
            do
            {
                new_state = aupipe->state >> 1;
                LOG_I("%s %x => %x, %x", __func__, aupipe->state, new_state, state);
                state_change = STATE_CHANGE(aupipe->state, new_state);
                aupipe->state_change = state_change;
                if (aupipe_poll_objs(aupipe, call_obj_set_state) != RT_EOK)
                    return -RT_ERROR;
                aupipe->state = new_state;
            }
            while (aupipe->state > state);
        }
    }

    return RT_EOK;
}

void aupipe_delete(Aupipe *aupipe)
{
    ApList *list;

    while (!aupipe_list_is_empty(&aupipe->objs))
    {
        list = aupipe_list_pop_tail(&aupipe->objs);
        rt_free(list);
    }

    aupipe_event_delete(aupipe->event);
    rt_free(aupipe);
}

int aupipe_add_object(Aupipe *aupipe, ApObject *obj, ...)
{
    va_list valist;
    ApList *list;

    va_start(valist, obj);
    while (obj != NULL)
    {
        obj->parent = aupipe;
        list = rt_malloc(sizeof(ApList));
        if (!list)
            return -RT_ERROR;
        list->data = obj;
        aupipe_list_append_tail(&aupipe->objs, list);
        obj = va_arg(valist, ApObject *);
    }
    va_end(valist);

    return RT_EOK;
}

ApObject *aupipe_find_object(Aupipe *aupipe, char *name)
{
    ApList *list;
    ApObject *obj;

    list = &aupipe->objs;

    while(1)
    {
        list = list->last;

        if (list == &aupipe->objs)
        {
            LOG_E("%s is not in the link", name);
            return NULL;
        }

        obj = (ApObject *)list->data;

        if (!strcmp(name, obj->name))
        {
            return obj;
        }
    }
}

int aupipe_link_object(Aupipe *aupipe, struct obj *object)
{
    ApObject *front, *rear;
    ApPad *front_pad, *rear_pad;

    front = aupipe_find_object(aupipe, object->name);

    if (!front->get_pad)
    {
        LOG_E("%s no found get_pad", front->name);
        return -RT_ERROR;
    }

    for (int i = 0; i < object->sinks; i++)
    {
        rear = aupipe_find_object(aupipe, object->sink[i].name);

        if (!rear->get_pad)
        {
            LOG_E("%s no found get_pad", rear->name);
            return -RT_ERROR;
        }

        printf("%s -> %s\n", front->name, rear->name);

        for (int j = 0; j < object->sink[i].ch; j++)
        {
            if (object->sink[i].bypass)
            {
                front_pad = front->get_pad(front, PAD_TYPE_SRC, j);
                rear_pad = rear->get_pad(rear, PAD_TYPE_SINK, j);
                printf("%s src_pad %d -> %s sink_pad %d\n", front->name, j, rear->name, j);
            }
            else
            {
                front_pad = front->get_pad(front, PAD_TYPE_SRC, (object->sink[i].ch_map[j] >> 8) & 0xff);
                rear_pad = rear->get_pad(rear, PAD_TYPE_SINK, object->sink[i].ch_map[j] & 0xff);
                printf("%s src_pad %d -> %s sink_pad %d\n", front->name, (object->sink[i].ch_map[j] >> 8) &0xff,\
                                                                   rear->name, object->sink[i].ch_map[j] & 0xff);
            }

            if (!front_pad || !rear_pad)
            {
                LOG_E("cannot get pad");
                return -RT_ERROR;
            }
            front_pad->peer = rear_pad;
            rear_pad->peer = front_pad;
        }
    }

    return RT_EOK;
}

int aupipe_create_link(Aupipe *aupipe, char *args)
{
    char *buf;
    size_t size;
    int element_num;
    cJSON *root = NULL;
    cJSON *_root = NULL;
    struct element *elements = NULL;

    FILE *fd = fopen(args, "rb");
    if (!fd)
    {
        printf("%s open fail!\n", args);
        return -RT_ERROR;
    }

    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    buf = malloc(size);
    if (!buf)
    {
        fclose(fd);
        return -RT_ERROR;
    }
    fread(buf, 1, size, fd);
    fclose(fd);

    _root = root = cJSON_Parse(buf);
    if (!root->string && cJSON_IsObject(root) && root->child)
    {
        root = root->child;
        printf("switch to child: %s\n", root->string);
    }

    element_num = cJSON_GetArraySize(root);

    for (int i = 0; i < element_num; i++)
    {
        cJSON *node = cJSON_GetArrayItem(root, i);
        if (!strcmp(node->string, "rate"))
        {
            aupipe->rate = (int)cJSON_GetNumberValue(node);
        }
        else if (!strcmp(node->string, "bits"))
        {
            aupipe->bits = (int)cJSON_GetNumberValue(node);
        }
        else if (!strcmp(node->string, "period-size"))
        {
            aupipe->period_size = (int)cJSON_GetNumberValue(node);
        }
        else
        {
            continue;
        }
        cJSON_Delete(cJSON_DetachItemViaPointer(root, node));
        element_num--;
        i--;
    }

    if (element_num > 0)
    {
        elements = calloc(element_num, sizeof(struct element));
        for (int i = 0; i < element_num; i++)
        {
            elements[i].json = cJSON_GetArrayItem(root, i);
            elements[i].obj = new_obj_from_json(elements[i].json);

            ApObject *object = aupipe_object_new(elements[i].obj->factory, NULL);
            if (!object)
            {
                LOG_E("Create %s failed", elements[i].obj->name);
            }
            LOG_I("Create %p %s", object, object->name);

            aupipe_add_object(aupipe, object, NULL);

            object->init(object, elements[i].obj->para);
        }

        for (int i = 0; i < element_num; i++)
        {
            for (int j = 0; j < elements[i].obj->sinks; j++)
            {
                elements[i].obj->sink[j].obj = find_sink(elements, element_num,\
                                                 elements[i].obj->sink[j].name);
            }
            aupipe_link_object(aupipe, elements[i].obj);
            print_obj(elements[i].obj);
        }
    }
    else
    {
        printf("error: root is not array %x\n", root->type);
    }

    cJSON_Delete(_root);

    free(elements);
    free(buf);
}

int aupipe_delete_link(Aupipe *aupipe)
{
    ApList *list;
    ApObject *object;

    list = &aupipe->objs;

    while(1)
    {
        list = list->last;

        if (list == &aupipe->objs)
        {
            break;
        }

        object = (ApObject *)list->data;

        object->deinit(object);
        aupipe_object_delete(object);
    }
    aupipe_delete(aupipe);

    return RT_EOK;
}

int aupipe_find_property(char *src, char *sub, int type, void *args)
{
    char *ptr;
    char *src_str, *sub_str;
    char *in_str = NULL, *out_str = NULL;
    src_str = rt_malloc(strlen(src) + 1);
    rt_memcpy(src_str, src, strlen(src) + 1);

    sub_str = strstr(src_str, sub);
    if (sub_str == NULL)
    {
        rt_free(src_str);

        LOG_E("property not found");
        return -RT_ERROR;
    }

    ptr = strtok_r(sub_str, ",", &out_str);
    if (ptr == NULL)
    {
        rt_free(src_str);

        LOG_E("property extraction failed");
        return -RT_ERROR;
    }
    else
    {
        if (strtok_r(ptr, "=", &in_str) == NULL)
        {
            rt_free(src_str);

            LOG_E("property value extraction failed");
            return -RT_ERROR;
        }
        else
        {
            switch(type)
            {
                case VALUE_TYPE_INT:
                {
                    int *val = (int *)args;
                    *val = atoi(in_str);
                    break;
                }
                case VALUE_TYPE_FLOAT:
                {
                    float *val = (float *)args;
                    *val = atof(in_str);
                    break;
                }
                case VALUE_TYPE_STRING:
                {
                    char **val = (char **)args;
                    *val = strdup(in_str);
                    break;
                }
                default:
                {
                    LOG_E("Error value type");
                    break;
                }
            }
        }
    }

    rt_free(src_str);

    return RT_EOK;
}

void aupipe_list_object_template(void)
{
    for (int i = 0; i < factory->count; i++)
        printf("[%d] %s\n", i, factory->objs[i].template->name);
}

ApObject *aupipe_object_new(char *temp_name, char *name)
{
    ApObject *obj;
    char temp[32];

    if (factory == NULL)
    {
        LOG_E("factory is null");
        return NULL;
    }

    for (int i = 0; i < factory->count; i++)
    {
        if (strcmp(temp_name, factory->objs[i].template->name) == 0)
        {
            obj = rt_malloc(factory->objs[i].size);
            if (!obj)
            {
                LOG_E("malloc failed");
                return NULL;
            }
            memset(obj, 0, factory->objs[i].size);
            memcpy(obj, factory->objs[i].template, sizeof(ApObject));
            obj->src_links = obj->sink_links = NULL;
            if (name != NULL)
            {
                strcpy(obj->name, name);
            }
            else
            {
                snprintf(temp, sizeof(temp), "%s%d%c",
                         factory->objs[i].template->name,
                         factory->objs[i].ref, '\0');
                memcpy(obj->name, temp, sizeof(obj->name));
            }
            factory->objs[i].ref++;

            return obj;
        }
    }

    LOG_W("No object named %s", temp_name);

    return NULL;
}

void aupipe_object_delete(ApObject *obj)
{
    for (int i = 0; i < factory->count; i++)
    {
        if (strstr(obj->name, factory->objs[i].template->name) != RT_NULL)
        {
            RT_ASSERT(factory->objs[i].ref >= 0);
            factory->objs[i].ref--;
        }
    }
    rt_free(obj);
}

int aupipe_object_register(ApObject *obj, size_t obj_size)
{
    struct obj_template *new_objs;

    if (factory == NULL)
    {
        LOG_E("factory is null");
        return -RT_ERROR;
    }

    rt_mutex_take(factory->lock, RT_WAITING_FOREVER);

    for (int i = 0; i < factory->count; i++)
    {
        if (strcmp(obj->name, factory->objs[i].template->name) == 0)
        {
            LOG_E("%s is already registered", obj->name);
            return -RT_ERROR;
        }
    }
    if (factory->count == factory->size)
    {
        factory->size *= 2;
        new_objs = (struct obj_template *)rt_realloc(factory->objs, sizeof(struct obj_template) * factory->size);
        if (!new_objs)
        {
            LOG_E("expansion failed");
            return -RT_ERROR;
        }
        factory->objs = new_objs;
    }
    factory->objs[factory->count].template = obj;
    factory->objs[factory->count].size = obj_size;
    factory->objs[factory->count].ref = 0;
    factory->count++;

    rt_mutex_release(factory->lock);

    return RT_EOK;
}

void aupipe_value_set(ApValue *value, void *arg)
{
    switch (value->type)
    {
    case VALUE_TYPE_INT:
    {
        int *val = (int *)arg;
        value->v.int_v = *val;
        break;
    }
    case VALUE_TYPE_FLOAT:
    {
        float *val = (float *)arg;
        value->v.float_v = *val;
        break;
    }
    case VALUE_TYPE_STRING:
    {
        char **val = (char **)arg;
        value->v.str_v = *val;
        break;
    }
    case VALUE_TYPE_POINTER:
    {
        void **val = (void **)arg;
        value->v.pointer_v = *val;
        break;
    }
    default:
    {
        LOG_E("Error value type");
        break;
    }
    }
}

void aupipe_value_get(ApValue *value, void *arg)
{
    switch (value->type)
    {
    case VALUE_TYPE_INT:
    {
        int *val = (int *)arg;
        *val = value->v.int_v;
        break;
    }
    case VALUE_TYPE_FLOAT:
    {
        float *val = (float *)arg;
        *val = value->v.float_v;
        break;
    }
    case VALUE_TYPE_STRING:
    {
        char **val = (char **)arg;
        *val = value->v.str_v;
        break;
    }
    case VALUE_TYPE_POINTER:
    {
        void **val = (void **)arg;
        *val = value->v.pointer_v;
        break;
    }
    default:
    {
        LOG_E("Error value type");
        break;
    }
    }
}

ApBuffer *aupipe_buffer_new(int type, size_t size)
{
    int tsize;

    tsize = sizeof(ApBuffer);

    ApBuffer *pbuffer = rt_malloc(tsize);
    if (!pbuffer)
        goto no_pbuffer;

    pbuffer->type = type;
    pbuffer->len = size;
    pbuffer->data = rt_malloc(size);
    if (!pbuffer->data)
        goto no_databuf;

    pbuffer->ref = 0;
    pbuffer->ref_lock = rt_mutex_create("ref_lock", RT_IPC_FLAG_FIFO);
    if (!pbuffer->ref_lock)
        goto no_lock;

    return pbuffer;

no_lock:
    rt_free(pbuffer->data);
no_databuf:
    rt_free(pbuffer);
no_pbuffer:

    return NULL;
}

ApBuffer *aupipe_buffer_ref(ApBuffer *buffer)
{
    rt_mutex_take(buffer->ref_lock, RT_WAITING_FOREVER);
    buffer->ref++;
    rt_mutex_release(buffer->ref_lock);

    return buffer;
}

void aupipe_buffer_unref(ApBuffer *buffer)
{
    rt_mutex_take(buffer->ref_lock, RT_WAITING_FOREVER);
    if (buffer->ref)
        buffer->ref--;
    rt_mutex_release(buffer->ref_lock);

    if (!buffer->ref)
    {
        rt_free(buffer->data);
        rt_mutex_delete(buffer->ref_lock);
        rt_free(buffer);
    }
}

