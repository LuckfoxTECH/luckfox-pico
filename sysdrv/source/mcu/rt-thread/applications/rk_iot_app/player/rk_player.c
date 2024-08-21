/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "rtthread.h"
#include <stdlib.h>
#include <string.h>
#include "rk_player.h"
#include "rkdebug.h"

void player_callback(player_handle_t self, play_info_t info, void *userdata);

static player_cfg_t player_cfg =
{
    .preprocess_buf_size = 1024 * 20,
    .decode_buf_size = 1024 * 5,
    .tag = "two",
    .name = PRJCONF_PLAYBACK_SOUND_CARD,
    .device = DEFAULT_PLAYBACK_DEVICE,
    .listen = player_callback,
    .resample_rate = 48000,
};

static mplayer_t *p_mplayer_list = NULL;

mplayer_t *get_mplayer_handler(player_handle_t player)
{
    mplayer_t *p;
    rt_enter_critical();
    p = p_mplayer_list;
    while (p)
    {
        if (p->h_player == player)
        {
            rt_exit_critical();
            return p;
        }
        p = p->next;
    }
    rt_exit_critical();
    return NULL;
}

mplayer_t *add_player_to_mplayer_lit(player_handle_t player)
{
    mplayer_t *p;
    p = rt_malloc(sizeof(mplayer_t));
    if (p == NULL)
        return p;
    memset(p, 0, sizeof(mplayer_t));
    p->h_player = player;
    p->mutex = rt_mutex_create("mplayer", RT_IPC_FLAG_FIFO);
    p->next = NULL;
    rt_enter_critical();
    if (p_mplayer_list == NULL)
    {
        p_mplayer_list = p;
        rt_exit_critical();
        return p;
    }
    else
    {
        p->next = p_mplayer_list;
        p_mplayer_list = p;
        rt_exit_critical();
        return p;
    }
}

void remove_player_from_mplayer_lit(mplayer_t *xplayer)
{
    mplayer_t *p, *pl;
    pl = NULL;
    p = p_mplayer_list;
    rt_enter_critical();
    while (p)
    {
        if (p == xplayer)
        {
            if (pl)
            {
                pl->next = p->next;
            }
            else
            {
                p_mplayer_list = p->next;
            }
            rt_mutex_delete(p->mutex);
            rt_free(p);
            rt_exit_critical();
            return;
        }
        pl = p;
        p = p->next;
    }
    rt_exit_critical();
}


void player_callback(player_handle_t self, play_info_t info, void *userdata)
{
    mplayer_t *xplayer = get_mplayer_handler(self);
    if (xplayer == NULL)
        return;

    rt_mutex_take(xplayer->mutex, RT_WAITING_FOREVER);

    LOGD("playmenu info = %d, pstPlayMenuData->PlayerState = %d", info, xplayer->player_state);

    //audio event need first process
    if (xplayer->player_state == PLAYER_STATE_RUNNING)
    {
        if (xplayer->is_loop)
        {
            //must create a thread..
        }
    }

    if (xplayer->fun)
    {
        if (info == PLAY_INFO_IDLE && xplayer->player_state == PLAYER_STATE_RUNNING)
            xplayer->fun(xplayer, PLAYER_MEDIA_PLAYBACK_COMPLETE, 0, xplayer->arg);
    }

    rt_mutex_release(xplayer->mutex);
}

mplayer_t *MPlayerCreate()
{
    player_handle_t h_player;
    h_player = player_create(&player_cfg);
    if (h_player == NULL)
        return NULL;
    return add_player_to_mplayer_lit(h_player);
}

void MPlayerDestroy(mplayer_t *p)
{
    if (p)
    {
        player_stop(p->h_player);
        rt_thread_delay(10);
        player_destroy(p->h_player);
        player_deinit();
        remove_player_from_mplayer_lit(p);
    }
}

int MPlayerSetDataSourceUrl(mplayer_t *p, const char *pUrl)
{
    if (p->url)
        rt_free(p->url);
    p->url = rt_malloc(strlen(pUrl) + 1);

    LOGD("p->url = %08x, len = %d ", p->url, strlen(pUrl));

    if (p->url == NULL)
        return -1;
    memcpy(p->url, pUrl, strlen(pUrl) + 1);
    p->is_http = 0;
    return 0;
}

int MPlayerPrepare(mplayer_t *p)
{
    return 0;
}

int MPlayerPrepareAsync(mplayer_t *p)
{
    p->is_http = 1;
    return 0;
}

int MPlayerStart(mplayer_t *p)
{
    LOGD("%s Enter! ", __func__);
    if (p->player_state == PLAYER_STATE_PAUSED)
    {
        LOGD("%s Player resume", __func__);
        player_resume(p->h_player);
        p->player_state = PLAYER_STATE_RUNNING;
        return 0;
    }

    LOGD("%s Start with new target", __func__);
    play_cfg_t play_cfg;
    memset((void *)&play_cfg, 0, sizeof(play_cfg_t));
    if (!p->is_http)
    {
        LOGD("%s local play", __func__);
        p->player_state = PLAYER_STATE_IDLE;
        player_stop(p->h_player);
        play_cfg.preprocessor = (play_preprocessor_t)DEFAULT_FILE_PREPROCESSOR;
        play_cfg.target = p->url;
        play_cfg.need_free = RT_TRUE;
        play_cfg.freq_t = PLAY_FREQ_LOCALPLAY;
    }
    else
    {
        LOGD("%s net play", __func__);
        p->player_state = PLAYER_STATE_IDLE;
        player_stop(p->h_player);
        play_cfg.preprocessor = (play_preprocessor_t)DEFAULT_HTTP_PREPROCESSOR;
        play_cfg.target = p->url;
        play_cfg.need_free = RT_TRUE;
        play_cfg.freq_t = PLAY_FREQ_NETPLAY;
    }

    //player_set_seek(0);
    player_play(p->h_player, &play_cfg);
    LOGD("%s Wait player start", __func__);
    while ((player_get_state(p->h_player) != PLAYER_STATE_RUNNING) && (player_get_state(p->h_player) != PLAYER_STATE_ERROR))
    {
        rt_thread_delay(10);
    }
    p->player_state = PLAYER_STATE_RUNNING; //play
    return 0;
}

int MPlayerStop(mplayer_t *p)
{
    player_stop(p->h_player);
    p->player_state = PLAYER_STATE_IDLE;
    return 0;
}

int MPlayerPause(mplayer_t *p)
{
    if (player_get_state(p->h_player) == PLAYER_STATE_RUNNING)
    {
        player_pause(p->h_player);
        p->player_state = PLAYER_STATE_PAUSED;
    }
    return 0;
}

int MPlayerIsPlaying(mplayer_t *p)
{
    if (player_get_state(p->h_player) == PLAYER_STATE_RUNNING)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int MPlayerSeekTo(mplayer_t *p, int nSeekTimeMs)
{
    LOGD("%s Enter!", __func__);

    return 0;
}

int MPlayerGetCurrentPosition(mplayer_t *p, int *msec)
{

    return 0;
}

int MPlayerGetDuration(mplayer_t *p, int *msec)
{
    return 0;
}

int MPlayerReset(mplayer_t *p)
{
    LOGD("%s Enter! ", __func__);
    return 0;
}

int MPlayerSetLooping(mplayer_t *p, int bLoop)
{
    p->is_loop = bLoop;
    return 0;
}

int MPlayerInitCheck(mplayer_t *p)
{
    return 0;
}

int MPlayerSetNotifyCallback(mplayer_t *p, mplayer_callback fun, void *arg)
{
    rt_mutex_take(p->mutex, RT_WAITING_FOREVER);
    p->fun = fun;
    p->arg = arg;
    rt_mutex_release(p->mutex);
    return 0;
}

//int MPlayerSetCallback(mplayer_t *p, player_listen_cb listen_fun, void *arg)
//{
//    rt_mutex_take(p->mutex, RT_WAITING_FOREVER);
//    p->h_player->listen = listen_fun;
//    p->arg = arg;
//    rt_mutex_release(p->mutex);
//    return 0;
//}

int MPlayerSetHttpBuffer(mplayer_t *p, int max_level, int min_level)
{
    return 0;
}
