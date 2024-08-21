/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-21 20:22:58
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:44:18
 * @LastEditors: Do not edit
 * @Description:
 */

#include "pcba_app.h"

#ifdef RT_USING_EXT_OPUS_DECODER
#include "play_opus.h"
#endif

#define DBG_SECTION_NAME    "PLAY"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#define PLAYBACK_CARD_NAME    PCBA_PLAYBACK_SOUND_CARD

static void pcba_play_callback(player_handle_t self, play_info_t info, void *userdata)
{
    struct pcba_app_msg send_msg;
    int ret;

    send_msg.type = TYPE_PLAYBACK_DONE;
    send_msg.value = info;
    ret = rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
    RT_ASSERT(ret == RT_EOK);
}

int pcba_player_init(void)
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
        .listen = pcba_play_callback,
        .device = DEFAULT_PLAYBACK_DEVICE,
    };
    player_init();
#ifdef RT_USING_EXT_OPUS_DECODER
    play_decoder_t opus_decoder = DEFAULT_OPUS_DECODER;
    player_register_decoder("opus", &opus_decoder);
#endif
    g_pcba.player = player_create(&player_cfg);
    if (!g_pcba.player)
    {
        LOG_E("player create failed\n");
        return -1;
    }

    return 0;
}

static int pcba_player_start(char *file_path, int info_only)
{
    play_cfg_t *play_cfg;
    if (!g_pcba.player)
        return -1;
    player_stop(g_pcba.player);

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
    g_pcba.player_disable_callback = 0;
    LOG_I("now play %s\n", play_cfg->target);
    player_play(g_pcba.player, play_cfg);

    rt_free(play_cfg);

    return 0;
}

void pcba_player_stop(void)
{
    if (!g_pcba.player)
        return;
    g_pcba.player_disable_callback = 1;
    player_stop(g_pcba.player);
    g_pcba.player_state = PLAYER_STATE_IDLE;
}

void pcba_player_prepare(char *file_path, int info_only)
{
    char str[256];
    int ret;

    rt_snprintf(str, sizeof(str), "%s/%s", RECORD_DIR_PATH, file_path);
    ret = access(str, F_OK);
    if (ret < 0)
    {
        rt_kprintf("file not exist\n");
        return;
    }

    pcba_player_start(str, info_only);
}

uint32_t pcba_player_get_total_time(char *file_path)
{
#ifdef RT_USING_EXT_OPUS_DECODER
    char str[256];

    snprintf(str, sizeof(str), "%s/%s%c", RECORD_DIR_PATH, file_path, '\0');
    return opus_get_duration(str);
#else
    g_pcba.player_disable_callback = 1;
    pcba_player_prepare(file_path, 1);
    return player_get_total_time(g_pcba.player);
#endif
}

void pcba_player_deinit()
{
    player_destroy(g_pcba.player);
    g_pcba.player = NULL;
    player_deinit();
}
