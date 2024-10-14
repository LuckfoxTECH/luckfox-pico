/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#if defined(RT_USING_RPMSG_CMD) && defined(PRIMARY_CPU) && defined(RT_USING_AUPIPE_ECNR)

#include "parse.h"
#include "appmsg.h"
#include "rpmsg_cmd.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME                            "ECNR"

#define ECNR(obj)                                   ((ApAudioEcnr *)(obj))

static ApList interrupt_buffer_pool;

static rt_sem_t interrupt_sync_sem;
static rt_mutex_t interrupt_sync_mutex;

typedef struct audioecnr_object
{
    ApObject base;

    ApPad *src_pad;
    ApPad *sink_pad;

    rt_sem_t sem;
    rt_thread_t loop;

    int lperiod_size;
    int lperiod_byte;
    int speriod_byte;

    int period_count;
    int current_count;

    int in_channels;
    int out_channels;
    int src_channels;
    int ref_channels;
    int loop_running;

    uint32_t bit_wide;
    uint32_t chan_map;

    char *aud_buffer;
    char *cjson_file;

    RTSkvParam *param;

    ApList obj_buffer_pool;
    rt_mutex_t obj_sync_mutex;
} ApAudioEcnr;

static void ecnr_app_init(void)
{
    struct rpmsg_ept_handle_t *handle;

    handle = &rpmsg_ept_handle[RPMSG_ECN];
    handle->master_id = MASTER_ID;
    handle->remote_id = REMOTE_ID_0;
    handle->endpoint  = EPT_M1R0_ECNR;
    handle->cmd_counter = sizeof(rpmsg_ept_table) / sizeof(struct rpmsg_cmd_table_t);
    handle->cmd_table = rpmsg_ept_table;
    rpmsg_cmd_ept_thread_init(handle, 2048, RT_THREAD_PRIORITY_MAX / 2);
}
INIT_APP_EXPORT(ecnr_app_init);

void ecnr_ept_cb(void *param)
{
    struct rpmsg_cmd_data_t *p_rpmsg_data = (struct rpmsg_cmd_data_t *)param;
    struct rpmsg_ept_handle_t *handle = p_rpmsg_data->handle;
    struct rpmsg_cmd_head_t *head = &p_rpmsg_data->head;
    uint32_t *command = (uint32_t *)head->addr;

    if (ECNR_PROCESS == command[0])
    {
        ApList *list = rt_malloc(sizeof(ApList));
        if (!list)
        {
            return -RT_ERROR;
        }
        list->data = (uint32_t *)head->addr;

        rt_mutex_take(interrupt_sync_mutex, RT_WAITING_FOREVER);
        aupipe_list_insert_head(&interrupt_buffer_pool, list);
        rt_mutex_release(interrupt_sync_mutex);
    }
    else
    {
        rt_sem_release(interrupt_sync_sem);
    }
}

static int ecnr_process(ApPad *pad, ApBuffer *buffer);

static int ecnr_init(ApObject *obj, void *arg)
{
    ApAudioEcnr *object = ECNR(obj);

    int status = RT_EOK;
    Aupipe *aupipe = obj->parent;
    char *parameters = (char *)arg;

    /* lperiod-size:the size of gghg required by the ECNR algorithm */
    status |= aupipe_find_property(parameters, "lperiod-size",
                                   VALUE_TYPE_INT, &object->lperiod_size);
    status |= aupipe_find_property(parameters, "in-channels",
                                   VALUE_TYPE_INT, &object->in_channels);
    status |= aupipe_find_property(parameters, "out-channels",
                                   VALUE_TYPE_INT, &object->out_channels);
    status |= aupipe_find_property(parameters, "src-channels",
                                   VALUE_TYPE_INT, &object->src_channels);
    status |= aupipe_find_property(parameters, "ref-channels",
                                   VALUE_TYPE_INT, &object->ref_channels);
    status |= aupipe_find_property(parameters, "configs-file",
                                   VALUE_TYPE_STRING, &object->cjson_file);
    if (status != RT_EOK)
    {
        LOG_E("%s plug-in parameter initialization failed", object->base.name);
        return -RT_ERROR;
    }

    object->src_pad = aupipe_pad_new(obj, object->out_channels);
    RT_ASSERT(object->src_pad != NULL);

    object->sink_pad = aupipe_pad_new(obj, object->in_channels);
    RT_ASSERT(object->sink_pad != NULL);
    for (int i = 0; i < object->in_channels; i++)
    {
        object->sink_pad[i].process = ecnr_process;
    }

    object->bit_wide = aupipe->bits >> 3;
    object->chan_map = (1 << object->in_channels) - 1;

    /* period_byte size of the entire audio link */
    object->speriod_byte = aupipe->period_size * object->bit_wide;
    /* the size of period_byte required by the ECNR algorithm */
    object->lperiod_byte = object->lperiod_size * object->bit_wide;

    object->period_count = object->lperiod_size / aupipe->period_size;
    object->current_count = 0;

    object->sem = rt_sem_create("sem", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(object->sem != NULL);

    /**
      * object->obj_buffer_pool:records how many frames of data the RTOS has send
      * interrupt_buffer_pool:records how many frames of data the RTOS has received
      * by comparing two buffers_pool to ensure consistent sending and receiving data
      */
    aupipe_list_init(&interrupt_buffer_pool);
    aupipe_list_init(&object->obj_buffer_pool);

    interrupt_sync_sem = rt_sem_create("sem", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(interrupt_sync_sem != NULL);

    /**
      * Used between callback functions and do_ecnr to ensure that
      * only one thread is operating the buffer list at any time
      */
    interrupt_sync_mutex = rt_mutex_create("imutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(interrupt_sync_mutex != NULL);

    /**
      * Used between ecnr_process functions and do_ecnr to ensure
      * that only one thread is operating the buffer list at any time
      */
    object->obj_sync_mutex = rt_mutex_create("omutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(object->obj_sync_mutex != NULL);

    return RT_EOK;
}

static int ecnr_deinit(ApObject *obj)
{
    ApAudioEcnr *object = ECNR(obj);

    rt_free(object->src_pad);
    rt_free(object->sink_pad);

    rt_sem_delete(interrupt_sync_sem);

    rt_mutex_delete(interrupt_sync_mutex);
    rt_mutex_delete(object->obj_sync_mutex);

    return RT_EOK;
}

static int ecnr_process(ApPad *pad, ApBuffer *buffer)
{
    ApAudioEcnr *object = ECNR(pad->parent);

    Aupipe *aupipe = object->base.parent;

    if (0 == object->current_count)
    {
        if (0 == pad->id)
        {
            object->current_count = 0;
            /** do_ecnr thread receiving the processed data frame in the
              * ecnr thread, this segment of memory space will be released
              */
            object->aud_buffer = rt_malloc_shmem(object->lperiod_byte * object->in_channels);
            RT_ASSERT(object->aud_buffer != NULL);
        }
    }

    for (int i = 0; i < aupipe->period_size; i++)
    {
        rt_memcpy(object->aud_buffer + object->current_count * object->speriod_byte *
                  object->in_channels + (object->in_channels * i + pad->id) * object->bit_wide,
                  buffer->data + object->bit_wide * i, object->bit_wide);
    }
    aupipe_buffer_unref(buffer);

    object->chan_map = object->chan_map & (~(1 << pad->id));
    if (object->chan_map != 0)
    {
        return RT_EOK;
    }
    object->chan_map = (1 << object->in_channels) - 1;
    object->current_count++;

    if (object->current_count == object->period_count)
    {
        struct rpmsg_cmd_data_t rpmsg_data;
        struct rpmsg_cmd_head_t *head = &rpmsg_data.head;

        /** do_ecnr thread receiving the processed data frame in the
          * ecnr thread, this segment of memory space will be released
          */
        uint32_t *command = rt_malloc_shmem(sizeof(uint32_t) * 3);
        RT_ASSERT(command != NULL);

        rpmsg_data.handle = &rpmsg_ept_handle[RPMSG_ECN];

        command[0] = ECNR_PROCESS;
        command[1] = (uint32_t)object->aud_buffer;
        command[2] = (uint32_t)rt_malloc_shmem(object->lperiod_byte
                                               * object->out_channels);
        RT_ASSERT((void *)command[2] != NULL);

        head->addr = (uint32_t)command;
        head->type = RPMSG_TYPE_NORMAL;
        head->cmd  = RPMSG_CMD_GET_ECN_USAGE;

        RT_ASSERT(head->addr != RT_NULL);

        rt_err_t ret = rpmsg_cmd_send(&rpmsg_data, RL_BLOCK);
        RT_ASSERT(ret == RL_SUCCESS);

        object->current_count = 0;

        ApList *list = rt_malloc(sizeof(ApList));
        if (!list)
        {
            LOG_E("ApList malloc failed");
            return -RT_ERROR;
        }
        list->data = (uint32_t *)command[1];

        rt_mutex_take(object->obj_sync_mutex, RT_WAITING_FOREVER);
        aupipe_list_insert_head(&object->obj_buffer_pool, list);
        rt_mutex_release(object->obj_sync_mutex);
    }

    return RT_EOK;
}

static void do_ecnr(void *arg)
{
    ApAudioEcnr *obj = ECNR(arg);

    Aupipe *aupipe = obj->base.parent;

    while (obj->loop_running)
    {
        if (aupipe_list_is_empty(&interrupt_buffer_pool))
        {
            rt_thread_mdelay(1);
            continue;
        }

        rt_mutex_take(interrupt_sync_mutex, RT_WAITING_FOREVER);
        ApList *list = aupipe_list_pop_tail(&interrupt_buffer_pool);
        rt_mutex_release(interrupt_sync_mutex);
        uint32_t *command = (uint32_t *)list->data;
        rt_free(list);

        rt_mutex_take(obj->obj_sync_mutex, RT_WAITING_FOREVER);
        ApList *ap_list = aupipe_list_pop_tail(&obj->obj_buffer_pool);
        rt_mutex_release(obj->obj_sync_mutex);
        uint32_t *ap_command = (uint32_t *)ap_list->data;
        rt_free(ap_list);

        /* if the addresses of shmem are not equal, what inter core communication error was sent */
        if ((uint32_t)ap_command != (uint32_t)command[1])
        {
            LOG_E("an error occurred during rpmsg transmission");
            return -RT_ERROR;
        }

        char *send_buffer = (char *)command[1];
        char *recv_buffer = (char *)command[2];

        for (int i = 0; i < obj->period_count; i++)
        {
            for (int j = 0; j < obj->out_channels; j++)
            {
                ApBuffer *buffer = (ApBuffer *)aupipe_buffer_new(DATA_TYPE_AUDIO, obj->speriod_byte);
                if (!buffer)
                {
                    LOG_E("%s malloc %d failed", obj->base.name, obj->speriod_byte);
                    break;
                }

                for (int n = 0; n < aupipe->period_size; n++)
                {
                    rt_memcpy(buffer->data + n * obj->bit_wide, recv_buffer + i * obj->speriod_byte
                              * obj->out_channels + (obj->out_channels * n + j) * obj->bit_wide, obj->bit_wide);
                }

                buffer = aupipe_buffer_ref(buffer);
                aupipe_pad_push(&obj->src_pad[j], buffer);
            }
        }
        /* after usage, free the memory space requested in the ecnr */
        rt_free_shmem(send_buffer);
        rt_free_shmem(recv_buffer);
        rt_free_shmem(command);
    }
    obj->loop_running = 0;

    rt_sem_release(obj->sem);
}

static int ecnr_remote_init(ApAudioEcnr *obj)
{
    SkvFrameParam fparam;
    Aupipe *aupipe = obj->base.parent;

    struct rpmsg_cmd_data_t rpmsg_data;
    struct rpmsg_cmd_head_t *head = &rpmsg_data.head;

    rpmsg_data.handle = &rpmsg_ept_handle[RPMSG_ECN];

    uint32_t size = sizeof(SkvFrameParam) + sizeof(RTSkvParam)
                    + sizeof(SkvAecParam) + sizeof(SkvBeamFormParam)
                    + sizeof(SkvDereverbParam) + sizeof(SkvAesParam)
                    + sizeof(SkvNlpParam) + sizeof(SkvAnrParam)
                    + sizeof(SkvAgcParam) + sizeof(SkvCngParam)
                    + sizeof(SkvDtdParam) + sizeof(SkvEqParam)
                    + obj->src_channels * sizeof(short);

    char *encr_param = rt_malloc_shmem(size);
    RT_ASSERT(encr_param != NULL);

    fparam.bits = aupipe->bits;
    fparam.rate = aupipe->rate;

    fparam.period_size = obj->lperiod_size;

    fparam.in_channels = obj->in_channels;
    fparam.src_channels = obj->src_channels;
    fparam.ref_channels = obj->ref_channels;
    fparam.out_channels = obj->out_channels;

    obj->param = parse_encr_configs(obj->cjson_file);

    /* ecnr algorithm init paramters config*/
    uint32_t offset = 0;
    rt_memcpy(encr_param + offset, &fparam, sizeof(SkvFrameParam));
    offset = offset + sizeof(SkvFrameParam);

    rt_memcpy(encr_param + offset, obj->param, sizeof(RTSkvParam));
    offset = offset + sizeof(RTSkvParam);

    rt_memcpy(encr_param + offset, obj->param->aec, sizeof(SkvAecParam));
    offset = offset + sizeof(SkvAecParam);

    rt_memcpy(encr_param + offset, obj->param->bf, sizeof(SkvBeamFormParam));
    offset = offset + sizeof(SkvBeamFormParam);

    rt_memcpy(encr_param + offset, obj->param->bf->dereverb, sizeof(SkvDereverbParam));
    offset = offset + sizeof(SkvDereverbParam);

    rt_memcpy(encr_param + offset, obj->param->bf->aes, sizeof(SkvAesParam));
    offset = offset + sizeof(SkvAesParam);

    rt_memcpy(encr_param + offset, obj->param->bf->nlp, sizeof(SkvNlpParam));
    offset = offset + sizeof(SkvNlpParam);

    rt_memcpy(encr_param + offset, obj->param->bf->anr, sizeof(SkvAnrParam));
    offset = offset + sizeof(SkvAnrParam);

    rt_memcpy(encr_param + offset, obj->param->bf->agc, sizeof(SkvAgcParam));
    offset = offset + sizeof(SkvAgcParam);

    rt_memcpy(encr_param + offset, obj->param->bf->cng, sizeof(SkvCngParam));
    offset = offset + sizeof(SkvCngParam);

    rt_memcpy(encr_param + offset, obj->param->bf->dtd, sizeof(SkvDtdParam));
    offset = offset + sizeof(SkvDtdParam);

    rt_memcpy(encr_param + offset, obj->param->bf->eq, sizeof(SkvEqParam));
    offset = offset + sizeof(SkvEqParam);

    rt_memcpy(encr_param + offset, obj->param->aec->mic_chns_map,
              obj->src_channels * sizeof(short));

    uint32_t *command = rt_malloc_shmem(sizeof(uint32_t) * 3);

    command[0] = ECNR_INIT;
    command[1] = (uint32_t)encr_param;
    command[2] = (uint32_t)NULL;

    head->addr = (uint32_t)command;
    head->type = RPMSG_TYPE_NORMAL;
    head->cmd  = RPMSG_CMD_GET_ECN_USAGE;
    RT_ASSERT(head->addr != RT_NULL);

    rt_err_t ret = rpmsg_cmd_send(&rpmsg_data, RL_BLOCK);
    RT_ASSERT(ret == RL_SUCCESS);

    rt_sem_take(interrupt_sync_sem, RT_WAITING_FOREVER);

    rt_free_shmem(encr_param);
    rt_free_shmem(command);

    return RT_EOK;
}

static int ecnr_remote_deinit(ApAudioEcnr *obj)
{
    Aupipe *aupipe = obj->base.parent;

    struct rpmsg_cmd_data_t rpmsg_data;
    struct rpmsg_cmd_head_t *head = &rpmsg_data.head;

    rpmsg_data.handle = &rpmsg_ept_handle[RPMSG_ECN];

    uint32_t *command = rt_malloc_shmem(sizeof(uint32_t) * 3);

    command[0] = ECNR_DEINIT;
    command[1] = (uint32_t)NULL;
    command[2] = (uint32_t)NULL;

    head->addr = (uint32_t)command;
    head->type = RPMSG_TYPE_NORMAL;
    head->cmd  = RPMSG_CMD_GET_ECN_USAGE;
    RT_ASSERT(head->addr != RT_NULL);

    rt_err_t ret = rpmsg_cmd_send(&rpmsg_data, RL_BLOCK);
    RT_ASSERT(ret == RL_SUCCESS);

    rt_sem_take(interrupt_sync_sem, RT_WAITING_FOREVER);

    rt_free_shmem(command);

    return RT_EOK;
}

static int ecnr_set_state(ApObject *obj, int state)
{
    ApAudioEcnr *object = ECNR(obj);

    switch (state)
    {
    case STATE_NULL_TO_READY:
        LOG_I("STATE_NULL_TO_READY");
        if (ecnr_remote_init(object) != RT_EOK)
            return -RT_ERROR;
        object->loop = rt_thread_create("ecnr", do_ecnr, object,
                                        4096, RT_THREAD_PRIORITY_MAX / 2, 10);
        if (!object->loop)
        {
            return -RT_ERROR;
        }
        break;
    case STATE_PAUSED_TO_PLAYING:
        LOG_I("STATE_PAUSED_TO_PLAYING");
        object->loop_running = 1;
        rt_thread_startup(object->loop);
        break;
    case STATE_PLAYING_TO_PAUSED:
        LOG_I("STATE_PLAYING_TO_PAUSED");
        if (object->loop_running == 1)
        {
            /* when the link exits, ensure that the RTOS has received all returned data frames */
            while (!aupipe_list_is_empty(&object->obj_buffer_pool));

            /* loop still running */
            object->loop_running = 0;
            if (rt_sem_take(object->sem, 3000))
            {
                /* Timeout, force delete */
                LOG_W("Timeout");
                rt_thread_delete(object->loop);
            }
        }
        break;
    case STATE_READY_TO_NULL:
        LOG_I("STATE_READY_TO_NULL");
        if (ecnr_remote_deinit(object) != RT_EOK)
            return -RT_ERROR;
        break;
    default:
        break;
    }

    return RT_EOK;
}

static int ecnr_set_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static int ecnr_get_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static ApPad *ecnr_get_pad(ApObject *obj, int type, int id)
{
    ApAudioEcnr *object = ECNR(obj);

    if (type == PAD_TYPE_SINK)
        return &object->sink_pad[id];

    return &object->src_pad[id];
}

OBJECT_BASE_DEFINE(ecnr, ApAudioEcnr);

#endif
