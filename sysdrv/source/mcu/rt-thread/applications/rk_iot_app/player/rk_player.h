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

#ifndef _RKPLAYER_H_
#define _RKPLAYER_H_

#include <stdlib.h>
#include <stdio.h>
#include "rtthread.h"
#include "rtdef.h"
#include <stdint.h>
#include "audio_server.h"

#ifndef PRJCONF_PLAYBACK_SOUND_CARD
#define PRJCONF_PLAYBACK_SOUND_CARD "es8311p"
#endif

enum MediaEventType
{
    PLAYER_MEDIA_NOP = 0, //interface test message
    PLAYER_MEDIA_PREPARED,
    PLAYER_MEDIA_PLAYBACK_COMPLETE,
    PLAYER_MEDIA_BUFFERING_UPDATE,
    PLAYER_MEDIA_SEEK_COMPLETE,
    PLAYER_MEDIA_SET_VIDEO_SIZE,
    PLAYER_MEDIA_STARTED,
    PLAYER_MEDIA_PAUSED,
    PLAYER_MEDIA_STOPPED,
    PLAYER_MEDIA_SKIPPED,
    PLAYER_MEDIA_TIMED_TEXT,
    PLAYER_MEDIA_ERROR,
    PLAYER_MEDIA_INFO,
    PLAYER_MEDIA_SUBTITLE_DATA,
    PLAYER_MEDIA_CHANGE_URL,
    PLAYER_MEDIA_LOG_RECORDER,
    PLAYER_EXTEND_MEDIA_INFO,
    PLAYER_MEDIA_META_DATA,
    PLAYER_MEDIA_EVENT_MAX,
};

enum MediaInfoType
{
    MEDIA_INFO_UNKNOWN = 1,
    MEDIA_INFO_STARTED_AS_NEXT = 2,
    MEDIA_INFO_RENDERING_START = 3,
    MEDIA_INFO_BUFFERING_START = 701,
    MEDIA_INFO_BUFFERING_END = 702,

    MEDIA_INFO_NOT_SEEKABLE = 801,

    MEDIA_INFO_DOWNLOAD_START  = 10000,
    MEDIA_INFO_DOWNLOAD_END   = 10001,
    MEDIA_INFO_DOWNLOAD_ERROR  = 10002,
};

typedef int (* mplayer_callback)(void *, int, int, void *);

typedef struct _mplayer
{
    struct _mplayer *next;
    void *mutex;
    int player_state;
    player_handle_t h_player;
    mplayer_callback fun;
    void *arg;
    char *url;
    char is_http;
    char is_loop;
} mplayer_t;

mplayer_t *MPlayerCreate();

void MPlayerDestroy(mplayer_t *p);

int MPlayerSetDataSourceUrl(mplayer_t *p, const char *pUrl);

int MPlayerPrepare(mplayer_t *p);

int MPlayerPrepareAsync(mplayer_t *p);

int MPlayerStart(mplayer_t *p);

int MPlayerStop(mplayer_t *p);

int MPlayerPause(mplayer_t *p);

int MPlayerIsPlaying(mplayer_t *p);

int MPlayerSeekTo(mplayer_t *p, int nSeekTimeMs);

int MPlayerGetCurrentPosition(mplayer_t *p, int *msec);

int MPlayerGetDuration(mplayer_t *p, int *msec);

int MPlayerReset(mplayer_t *p);

int MPlayerSetLooping(mplayer_t *p, int bLoop);

int MPlayerInitCheck(mplayer_t *p);

int MPlayerSetNotifyCallback(mplayer_t *p, int (* fun)(void *, int, int, void *), void *);

int MPlayerSetHttpBuffer(mplayer_t *p, int max_level, int min_level);
#endif