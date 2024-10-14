/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-13     Jair Wu      First version
 *
 */

#include <rtthread.h>

#if defined(ENABLE_DUAL_TRACKING_TEST) && defined(RT_USING_AUDIO_SERVER)
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "dual_track.h"
#include "finsh.h"

static int record_loop;
static int record_time;
static int recording;
static char *file_name;
rt_timer_t rec_timer;
rt_thread_t rec_timeout_tid;
dt_recorder_handle_t dt_recorder_test = NULL;
static record_cfg_t *cfg_test = NULL;
static recorder_cfg_t recorder_cfg =
{
    .record_buf_size = 1024 * 5,
    .encode_buf_size = 1024 * 64,
    .tag = "one",
};

void dt_timeout(void *arg)
{
    while (1)
    {
        while (recording)
        {
            rt_thread_mdelay(100);
        }
        dt_capture_stop(dt_recorder_test);
        printf("%s:now record %s\n", __func__, cfg_test->target);
        dt_capture_start(dt_recorder_test, cfg_test);
        recording = 1;
    }
}

void dt_timer_iqr(void *arg)
{
    recording = 0;
}

int dt_start(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("dt_start <filepath>\n");
        return 0;
    }
    if (!cfg_test)
    {
        cfg_test = rt_malloc(sizeof(record_cfg_t));
        cfg_test->type = rt_malloc(10);
        file_name = rt_malloc(64);
    }
    memcpy(file_name, argv[1], 64);
    if (strstr(file_name, ".wav"))
    {
        sprintf(cfg_test->type, "wav");
    }
    else
    {
        printf("Not support this type:%s.\n", file_name);
        rt_free(cfg_test->type);
        rt_free(cfg_test);
        rt_free(file_name);
        cfg_test = NULL;
        return 0;
    }
    cfg_test->channels = 2;
    cfg_test->samplerate = 16000;
    cfg_test->bits = 16;
    record_loop = 0;
    while (*argv)
    {
        if (strcmp(*argv, "-c") == 0)
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
            record_loop = 1;
            argv++;
            if (*argv)
                record_time = atoi(*argv) * RT_TICK_PER_SECOND;
            if (record_time < 5)
                record_time = 5;
            if (rec_timer)
            {
                rt_timer_stop(rec_timer);
                rt_timer_control(rec_timer, RT_TIMER_CTRL_SET_TIME, &record_time);
            }
            else
            {
                rec_timer = rt_timer_create("rectime", dt_timer_iqr,
                                            RT_NULL, record_time,
                                            RT_TIMER_FLAG_PERIODIC);
            }
        }
        if (*argv)
            argv++;
    }
    if (!dt_recorder_test)
    {
        dt_recorder_test = dt_capture_create(&recorder_cfg);
    }
    else
    {
        dt_capture_stop(dt_recorder_test);
    }
    cfg_test->target = file_name;
    cfg_test->writer = (record_writer_t)DEFAULT_FILE_WRITER;
    cfg_test->freq_t = RECORD_FREQ_LOCAL;
    cfg_test->need_free = RT_FALSE;
    printf("%s:now record %s\n", __func__, cfg_test->target);
    if (record_loop)
    {
        recording = 1;
        if (!rec_timeout_tid)
        {
            rec_timeout_tid = rt_thread_create("rect", dt_timeout, NULL, 512, 18, 10);
            rt_thread_startup(rec_timeout_tid);
        }
        rt_timer_start(rec_timer);
    }
    dt_capture_start(dt_recorder_test, cfg_test);

    return 0;
}

int dt_stop(int argc, char **argv)
{
    if (!dt_recorder_test)
        return 0;
    dt_capture_stop(dt_recorder_test);
    if (rec_timer)
    {
        rt_timer_delete(rec_timer);
        rec_timer = NULL;
    }
    if (rec_timeout_tid)
    {
        rt_thread_delete(rec_timeout_tid);
        rec_timeout_tid = NULL;
    }
    if (strcmp(argv[1], "free") == 0)
    {
        rt_free(file_name);
        file_name = NULL;
        rt_free(cfg_test->type);
        rt_free(cfg_test);
        cfg_test = NULL;
        dt_capture_destroy(dt_recorder_test);
        dt_recorder_test = NULL;
    }

    return 0;
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(dt_start, doubel tracking test start);
MSH_CMD_EXPORT(dt_stop, doubel tracking test stop);
#endif
#endif
