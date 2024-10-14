/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "audio_server.h"
#ifdef RT_USING_EXT_OPUS_ENCODER
#include "record_opus.h"
#endif

#ifdef AUDIO_ENABLE_RECORDER_TEST
void recorder_callback(recorder_handle_t self, record_info_t info, void *userdata);

static char *file_name;
recorder_handle_t recorder_test = NULL;
static record_cfg_t *cfg_test = NULL;
static recorder_cfg_t recorder_cfg =
{
    .record_buf_size = 1024 * 5,
    .encode_buf_size = 1024 * 5,
    .recordr_stack_size = 2048,
    .encoder_stack_size = 2048,
    .writer_stack_size = 2048,
    .tag = "one",
    .listen = recorder_callback,
    .device = DEFAULT_CAPTURE_DEVICE,
};

void recorder_callback(recorder_handle_t self, record_info_t info, void *userdata)
{
}

void record_start(int argc, char **argv)
{
    int record_time = 0;

    if (argc < 2)
    {
        printf("record_start <filepath>\n");
        return;
    }
    if (!cfg_test)
    {
        cfg_test = malloc(sizeof(record_cfg_t));
        cfg_test->type = malloc(10);
        file_name = malloc(64);
    }
    memcpy(file_name, argv[1], 64);
    if (strstr(file_name, ".pcm"))
    {
        sprintf(cfg_test->type, "pcm");
    }
    else if (strstr(file_name, ".wav"))
    {
        sprintf(cfg_test->type, "wav");
    }
    else if (strstr(file_name, ".opus"))
    {
        sprintf(cfg_test->type, "opus");
    }
    else
    {
        printf("Not support this type:%s.\n", file_name);
        free(cfg_test->type);
        free(cfg_test);
        free(file_name);
        cfg_test = NULL;
        return;
    }
    cfg_test->channels = 2;
    cfg_test->samplerate = 16000;
    cfg_test->bits = 16;
    if (recorder_cfg.device_name == NULL)
    {
        recorder_cfg.device_name = malloc(10);
        strcpy(recorder_cfg.device_name, "adcc");
    }
    while (*argv)
    {
        if (strcmp(*argv, "-D") == 0)
        {
            argv++;
            if (*argv)
                strcpy(recorder_cfg.device_name, *argv);
        }
        else if (strcmp(*argv, "-c") == 0)
        {
            argv++;
            if (*argv)
                cfg_test->channels = atoi(*argv);
        }
        else if (strcmp(*argv, "-r") == 0)
        {
            argv++;
            if (*argv)
                cfg_test->samplerate = atoi(*argv);
        }
        else if (strcmp(*argv, "-b") == 0)
        {
            argv++;
            if (*argv)
                cfg_test->bits = atoi(*argv);
        }
        else if (strcmp(*argv, "-l") == 0)
        {
            argv++;
            if (*argv)
                record_time = atoi(*argv);
        }
        if (*argv)
            argv++;
    }
    if (!recorder_test)
    {
        recorder_init();
#ifdef RT_USING_EXT_OPUS_ENCODER
        record_encoder_t opus_encoder = DEFAULT_OPUS_ENCODER;
        recorder_register_encoder("opus", &opus_encoder);
#endif
        recorder_list_encoder();
        recorder_test = recorder_create(&recorder_cfg);
    }
    else
    {
        recorder_stop(recorder_test);
    }
    cfg_test->target = file_name;
    cfg_test->writer = (record_writer_t)DEFAULT_FILE_WRITER;
    cfg_test->freq_t = RECORD_FREQ_LOCAL;
    cfg_test->need_free = 0;
    cfg_test->duration = record_time;
    printf("%s:now record %s\n", __func__, cfg_test->target);
    recorder_record(recorder_test, cfg_test);
}

void record_stop(int argc, char **argv)
{
    if (!recorder_test)
        return;
    recorder_stop(recorder_test);
    if (strcmp(argv[1], "free") == 0)
    {
        free(file_name);
        file_name = NULL;
        free(cfg_test->type);
        free(cfg_test);
        cfg_test = NULL;
        free(recorder_cfg.device_name);
        recorder_cfg.device_name = NULL;
        recorder_destroy(recorder_test);
        recorder_test = NULL;
        recorder_deinit();
        recorder_list_encoder();
    }
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(record_start, record test start);
MSH_CMD_EXPORT(record_stop, record test stop);
#endif
#endif
