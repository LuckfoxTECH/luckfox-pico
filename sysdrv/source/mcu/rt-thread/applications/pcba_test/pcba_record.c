/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-21 20:16:32
 * @Company: Rockchip
 * @LastEditTime: 2020-05-21 20:21:06
 * @LastEditors: Do not edit
 * @Description:
 */

#include "pcba_app.h"
#ifdef RT_USING_EXT_OPUS_ENCODER
#include "record_opus.h"
#endif

#define DBG_SECTION_NAME    "REC"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define ENCODE_TYPE         PCBA_RECORD_FILE_TYPE
#define RECORD_CARD_NAME    PCBA_CAPTURE_SOUND_CARD
#define RECORD_SAMPLERATE   16000
#define RECORD_CHANNELS     2
#define RECORD_BITS         16

static int disable_callback = 0;

void pcba_record_callback(recorder_handle_t self, record_info_t info, void *userdata)
{
    struct pcba_app_msg send_msg;

    if (disable_callback)
        return;

    LOG_I("%s info %d", __func__, info);
    switch (info)
    {
    case RECORD_INFO_SUCCESS:
        if (g_pcba.record_seglen)
        {
            LOG_I("[0x%08x]Next record", HAL_GetTick());
            send_msg.type = TYPE_RECORD_DONE;
            send_msg.value = info;
            int ret = rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
            if (ret == -3)
                LOG_W("Msg queue full");
        }
        break;
    /* Writer init failed */
    case RECORD_INFO_WRITER:
    /* Encoder init failed */
    case RECORD_INFO_ENCODE:
    /* Sound card init failed */
    case RECORD_INFO_STOP:
    default:
        break;
    }
}

int pcba_recorder_init(void)
{
    recorder_cfg_t recorder_cfg =
    {
        .record_buf_size = 1024 * 10,
        .encode_buf_size = 1024 * 30,
        .recordr_stack_size = 2048,
        /* if opus encode run on cpu, should be 1024 * 80 */
        .encoder_stack_size = 2048,
        .writer_stack_size = 2048,
        .tag = ENCODE_TYPE,
        .device_name = RECORD_CARD_NAME,
        .listen = pcba_record_callback,
        .device = DEFAULT_CAPTURE_DEVICE,
    };
    recorder_init();
#ifdef RT_USING_EXT_OPUS_ENCODER
    record_encoder_t opus_encoder = DEFAULT_OPUS_ENCODER;
    recorder_register_encoder("opus", &opus_encoder);
#endif
    g_pcba.recorder = recorder_create(&recorder_cfg);
    if (!g_pcba.recorder)
    {
        LOG_E("recorder create failed\n");
        return -1;
    }

    return 0;
}

static int pcba_recorder_start(char *file_path)
{
    record_cfg_t *record_cfg;
    if (!g_pcba.recorder)
        return -1;
    recorder_stop(g_pcba.recorder);

    record_cfg = rt_malloc(sizeof(record_cfg_t));
    if (!record_cfg)
    {
        LOG_E("cfg malloc %d failed\n", sizeof(record_cfg_t));
        return -1;
    }

    record_cfg->target = file_path;
    record_cfg->need_free = RT_TRUE;

    record_cfg->type = ENCODE_TYPE;
    record_cfg->samplerate = RECORD_SAMPLERATE;
    record_cfg->channels = RECORD_CHANNELS;
    record_cfg->bits = RECORD_BITS;
    record_cfg->writer = (record_writer_t)DEFAULT_FILE_WRITER;
    record_cfg->freq_t = RECORD_FREQ_LOCAL;
    /* minutes to seconds */
//    record_cfg->duration = get_seglen() * 60;
    record_cfg->duration = 1 * 60;
    disable_callback = 0;
    LOG_I("now record %s\n", record_cfg->target);
    recorder_record(g_pcba.recorder, record_cfg);

    rt_free(record_cfg);

    return 0;
}

void pcba_recorder_stop(void)
{
    if (!g_pcba.recorder)
        return;
    disable_callback = 1;
    recorder_stop(g_pcba.recorder);
}

void pcba_recorder_prepare(char *file_path)
{
    char *dir_path = RECORD_DIR_PATH;
    int ret;

    ret = access(dir_path, F_OK);
    if (ret < 0)
    {
        rt_kprintf("dir not exist\n");
        mkdir(dir_path, 0);
    }

    memset(file_path, 0x0, MAX_FILE_NAME);
    sprintf(file_path, RECORD_DIR_PATH"/%04d-%02d-%02d-%02d%02d%02d."ENCODE_TYPE,
            g_pcba.date->tm_year + 1900,
            g_pcba.date->tm_mon + 1,
            g_pcba.date->tm_mday,
            g_pcba.date->tm_hour,
            g_pcba.date->tm_min,
            g_pcba.date->tm_sec);
    pcba_recorder_start(file_path);
}

void pcba_recorder_deinit()
{
    recorder_destroy(g_pcba.recorder);
    g_pcba.recorder = NULL;
    recorder_deinit();
}
