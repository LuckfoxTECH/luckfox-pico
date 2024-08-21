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
#include "dfs_posix.h"
#ifdef RT_USING_EXT_OPUS_DECODER
#include "play_opus.h"
#endif
#ifdef RT_USING_EXT_FLAC_DECODER
#include "play_flac.h"
#endif

#ifdef AUDIO_ENABLE_PLAYER_TEST
#define MUSIC_MAX 5
#define MP3_URL "http://openaudio-app-50.beva.cn/dq/lpyKDWdpRILOAnswKSsJxQGoyRDn.mp3\0"

#define EVENT_STOP      (0x1U << 1)
#define EVENT_CALLBACK  (0x1U << 2)
#define EVENT_EXIT      (0x1U << 3)

void player_callback_test(player_handle_t self, play_info_t info, void *userdata);

static struct rt_event event;
static player_handle_t player_test = NULL;
static rt_timer_t g_timer = NULL;
static play_cfg_t *cfg_test = NULL;
static int play_loop;
static int music_index;
static rt_thread_t callback_handle = NULL;
static player_cfg_t player_cfg =
{
    .preprocess_buf_size = 1024 * 40,
    .decode_buf_size = 1024 * 20,
    .preprocess_stack_size = 2048,
    .decoder_stack_size = 1024 * 12,
    .playback_stack_size = 2048,
    .tag = "one",
    .device = DEFAULT_PLAYBACK_DEVICE,
    .listen = player_callback_test,
};

void player_callback_handle(void *arg)
{
    rt_uint32_t e;

    while (1)
    {
        rt_event_recv(&event, (EVENT_STOP | EVENT_CALLBACK | EVENT_EXIT),
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e);

        if (g_timer)
            rt_timer_stop(g_timer);
        if (e & EVENT_STOP)
        {
            printf("EVENT_STOP\n");
            player_stop(player_test);
        }
        if (e & EVENT_EXIT)
        {
            printf("EVENT_EXIT\n");
            break;
        }
        if (e & EVENT_CALLBACK)
        {
            printf("EVENT_CALLBACK\n");
            if (play_loop)
            {
                music_index++;
                if (music_index >= MUSIC_MAX)
                    music_index = 0;
                sprintf(cfg_test->target, "/music/%d.mp3", music_index);
                player_play(player_test, cfg_test);
                if (g_timer)
                    rt_timer_start(g_timer);
            }
            else
            {
                player_device_stop(player_test, 1);
            }
        }
    }
    rt_event_detach(&event);
    callback_handle = NULL;
}

void player_callback_test(player_handle_t self, play_info_t info, void *userdata)
{
    if (info == PLAY_INFO_IDLE)
        rt_event_send(&event, EVENT_CALLBACK);
}

void show_music_time(void *argv)
{
    uint32_t cur, total;
    cur = player_get_cur_time(player_test);
    total = player_get_total_time(player_test);
    rt_kprintf("\rCur %d.%03ds/%d.%03ds", cur / 1000, cur % 1000, total / 1000, total % 1000);
}

void *create_player(int argc, void *argv[])
{
    if (argc < 2)
    {
        printf("create_player [-D card] [-r resample rate]\n");
        return 0;
    }

    if (player_test)
        return 0;

    rt_event_init(&event, "playcb", RT_IPC_FLAG_FIFO);
    if (callback_handle == NULL)
    {
        callback_handle = rt_thread_create("playcb",
                                           player_callback_handle,
                                           NULL,
                                           1024,
                                           14,
                                           10);
        if (!callback_handle)
        {
            printf("Create callback handle failed\n");
            rt_event_detach(&event);
            return 0;
        }
        rt_thread_startup(callback_handle);
    }
    player_init();
#ifdef RT_USING_EXT_OPUS_DECODER
    play_decoder_t opus_decoder = DEFAULT_OPUS_DECODER;
    player_register_decoder("opus", &opus_decoder);
#endif
#ifdef RT_USING_EXT_FLAC_DECODER
    play_decoder_t flac_decoder = DEFAULT_FLAC_DECODER;
    player_register_decoder("flac", &flac_decoder);
#endif
    player_register_apedec();
    player_register_mp3dec();
    player_list_decoder();
    cfg_test = rt_malloc(sizeof(play_cfg_t));
    cfg_test->target = rt_malloc(1024);
    memset(cfg_test->target, 0x0, 1024);
    player_cfg.name = rt_malloc(10);
    player_cfg.resample_rate = 0;
    player_cfg.out_ch = 2;
    player_cfg.diff_out = 0;
    sprintf(player_cfg.name, "es8311p");
    while (*argv)
    {
        if (strcmp(*argv, "-D") == 0)
        {
            argv++;
            if (*argv)
                strcpy(player_cfg.name, *argv);
        }
        else if (strcmp(*argv, "-r") == 0)
        {
            argv++;
            if (*argv)
                player_cfg.resample_rate = atoi(*argv);
        }
        else if (strcmp(*argv, "-l") == 0)
        {
            player_cfg.listen = player_callback_test;
        }
        else if (strcmp(*argv, "-d") == 0)
        {
            player_cfg.diff_out = 1;
        }
        if (*argv)
            argv++;
    }
    player_test = player_create(&player_cfg);
    printf("Create player %p\n", player_test);
    playback_set_volume(100);

    return 0;
}

void *start_player(int argc, void *argv[])
{
    int got_path = 0;
    int display_time = 0;

    if (argc < 2)
    {
        printf("start_player <path>\n");
        return 0;
    }

    if (!player_test)
    {
        printf("Call create_player first\n");
        return 0;
    }
    memset(cfg_test->target, 0x0, 1024);
    cfg_test->start_time = 0;
    while (*argv)
    {
        if (strcmp(*argv, "-f") == 0)
        {
            argv++;
            if (strcmp(*argv, "loop") == 0)
            {
                play_loop = 1;
                music_index = 0;
                sprintf(cfg_test->target, "/music/%d.mp3", music_index);
            }
            else
            {
                play_loop = 0;
                memcpy(cfg_test->target, *argv, strlen(*argv));
            }
            got_path = 1;
        }
        else if (strcmp(*argv, "-s") == 0)
        {
            argv++;
            cfg_test->start_time = atoi(*argv);
        }
        else if (strcmp(*argv, "-v") == 0)
        {
            display_time = 1;
        }
        if (*argv)
            argv++;
    }
    if (!got_path)
        memcpy(cfg_test->target, MP3_URL, strlen(MP3_URL));
    if (strncmp(cfg_test->target, "http", 4) == 0 || strncmp(cfg_test->target, "HTTP", 4) == 0)
        cfg_test->preprocessor = (play_preprocessor_t)DEFAULT_HTTP_PREPROCESSOR;
    else
        cfg_test->preprocessor = (play_preprocessor_t)DEFAULT_FILE_PREPROCESSOR;
    player_stop(player_test);
    cfg_test->freq_t = PLAY_FREQ_LOCALPLAY;
    cfg_test->need_free = 1;
    cfg_test->info_only = 0;
    printf("%s:now play %s start from %ld\n\n", __func__, cfg_test->target, cfg_test->start_time);
    player_play(player_test, cfg_test);

    if (display_time)
    {
        if (!g_timer)
            g_timer = rt_timer_create("ainfo",
                                      show_music_time,
                                      NULL,
                                      1000,
                                      RT_TIMER_FLAG_PERIODIC);
        if (g_timer)
            rt_timer_start(g_timer);
    }

    return 0;
}

void *stop_player(int argc, void *argv[])
{
    if (!player_test)
        return 0;

    rt_event_send(&event, EVENT_STOP);

    return 0;
}

void *delete_player(int argc, void *argv[])
{
    if (!player_test)
        return 0;

    rt_event_send(&event, EVENT_EXIT);
    player_stop(player_test);
    rt_free(cfg_test->target);
    rt_free(cfg_test);
    cfg_test = NULL;
    rt_free(player_cfg.name);
    player_cfg.name = NULL;
    player_destroy(player_test);
    player_test = NULL;
    player_deinit();
    if (g_timer)
    {
        rt_timer_stop(g_timer);
        rt_timer_delete(g_timer);
        g_timer = NULL;
    }

    return 0;
}

void player_seek_forward(int argc, void *argv[])
{
    int time;
    if (!player_test)
        return;
    time = player_get_cur_time(player_test) / 1000;
    if (argc > 1)
        time += atoi(argv[1]);
    else
        time += 1;
    cfg_test->start_time = time;
    player_stop(player_test);
    player_play(player_test, cfg_test);
}

void player_seek_backward(int argc, void *argv[])
{
    int time;
    if (!player_test)
        return;
    time = player_get_cur_time(player_test) / 1000;
    if (argc > 1)
        time -= atoi(argv[1]);
    else
        time -= 1;
    cfg_test->start_time = time;
    player_stop(player_test);
    player_play(player_test, cfg_test);
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(create_player, create test player);
MSH_CMD_EXPORT(start_player, start test player);
MSH_CMD_EXPORT(stop_player, stop test player);
MSH_CMD_EXPORT(delete_player, delete test player);
MSH_CMD_EXPORT(player_seek_forward, seek forward);
MSH_CMD_EXPORT(player_seek_backward, seek backward);
#endif
#endif

