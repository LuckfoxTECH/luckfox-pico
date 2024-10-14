/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-24     Jair Wu      First version
 *
 */

#include "recpen.h"

#ifdef RT_USING_EXT_OPUS_DECODER
#include "play_opus.h"
#endif

#define DBG_SECTION_NAME    "PLAY"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define PLAYBACK_CARD_NAME    RECPEN_PLAYBACK_SOUND_CARD

static void recpen_play_callback(player_handle_t self, play_info_t info, void *userdata)
{
    struct recpen_app_msg send_msg;
    int ret;

    if (info == PLAY_INFO_IDLE)
    {
        send_msg.type = TYPE_PLAYBACK_DONE;
        send_msg.value = info;
        ret = rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
        if (ret == -3)
            LOG_W("Msg queue full");
    }
    else
    {
        send_msg.type = TYPE_REFRESH;
        send_msg.value = REFRESH_PLAY_STATE;
        g_app_info.player_state = PLAYER_STATE_IDLE;
        ret = rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
        if (ret == -3)
            LOG_W("Msg queue full");
    }
}

int recpen_player_init(void)
{
    player_cfg_t player_cfg =
    {
        .preprocess_buf_size = 1024 * 10,
        .decode_buf_size = 1024 * 20,
        .preprocess_stack_size = 2048,
        .decoder_stack_size = 1024 * 20,
        .playback_stack_size = 2048,
        .tag = "default",
        .name = PLAYBACK_CARD_NAME,
        .listen = recpen_play_callback,
        .device = DEFAULT_PLAYBACK_DEVICE,
    };
    player_init();
#ifdef RT_USING_EXT_OPUS_DECODER
    play_decoder_t opus_decoder = DEFAULT_OPUS_DECODER;
    player_register_decoder("opus", &opus_decoder);
#endif
    g_app_info.player = player_create(&player_cfg);
    if (!g_app_info.player)
    {
        LOG_E("player create failed\n");
        return -1;
    }

    return 0;
}

static int recpen_player_start(char *file_path, int info_only)
{
    play_cfg_t *play_cfg;
    if (!g_app_info.player)
        return -1;
    player_stop(g_app_info.player);

    play_cfg = rt_malloc(sizeof(play_cfg_t));
    if (!play_cfg)
    {
        LOG_E("cfg malloc %d failed\n", sizeof(play_cfg_t));
        return -1;
    }

    play_cfg->target = file_path;
    play_cfg->need_free = RT_TRUE;
    play_cfg->preprocessor = (play_preprocessor_t)DEFAULT_FILE_PREPROCESSOR;
    play_cfg->freq_t = PLAY_FREQ_LOCALPLAY;
    play_cfg->info_only = info_only;
    play_cfg->start_time = 0;
    LOG_I("now play %s\n", play_cfg->target);
    player_play(g_app_info.player, play_cfg);

    rt_free(play_cfg);

    return 0;
}

void recpen_player_stop(void)
{
    if (!g_app_info.player)
        return;
    player_stop(g_app_info.player);
    g_app_info.player_state = PLAYER_STATE_IDLE;
}

void recpen_player_prepare(char *file_path, int info_only)
{
    char str[256];
    int ret;

    sprintf(str, "%s/%s", RECORD_DIR_PATH, file_path);
    ret = access(str, F_OK);
    if (ret < 0)
    {
        rt_kprintf("file not exist\n");
        return;
    }

    recpen_player_start(str, info_only);
}

uint32_t recpen_player_get_total_time(char *file_path)
{
#ifdef RT_USING_EXT_OPUS_DECODER
    char str[256];

    snprintf(str, sizeof(str), "%s/%s%c", RECORD_DIR_PATH, file_path, '\0');
    return opus_get_duration(str);
#else
    g_app_info.player_disable_callback = 1;
    recpen_player_prepare(file_path, 1);
    return player_get_total_time(g_app_info.player);
#endif
}

void recpen_player_deinit()
{
    player_destroy(g_app_info.player);
    g_app_info.player = NULL;
    player_deinit();
}
