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

#ifndef _GLOBE_PLAYER_H
#define _GLOBE_PLAYER_H

#include <rtthread.h>
#include <rtdef.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "audio_server.h"
#include "keyctrl_task.h"
#include "rk_player.h"
#include "rk_util.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum PLAYER_STATUS
{
    STATUS_STOPPED   = 0,
    STATUS_PREPARING = 1,
    STATUS_PREPARED  = 2,
    STATUS_PLAYING   = 3,
    STATUS_PAUSED    = 4,
    STATUS_SEEKING   = 5,
} PLAYER_STATUS;

typedef enum PLAYER_TYPE
{
    TYPE_NULL,
    //  play audio
    TYPE_MEDIA,
    //  NorFlash play audio
    TYPE_MEDIA_NORFLASH,
    //  SD card play audio
    TYPE_MEDIA_SD,
    //  AI dialog
    TYPE_DIALOG,
    //  wechat
    TYPE_INTERCOM,
    //  alert
    TYPE_ALERT,
    //  tone
    TYPE_TONE,
    //  resume play
    TYPE_REUSME,
} PLAYER_TYPE;

typedef struct
{
    int cnt;
    int wait;
    rt_sem_t sem;
} sem_t;

typedef struct RtosPlayerContext
{
    mplayer_t      *mPlayer;
    int             mPreStatus;
    int             mStatus;
    int             mSeekable;
    int             mError;
    rt_mutex_t      mMutex;
    rt_sem_t        mPrepared;
    rt_sem_t        mFinished;
    rt_sem_t        gptPrepared ;
} RtosPlayer;

//* define commands for user control.
typedef struct Command
{
    const char *strCommand;
    int         nCommandId;
    const char *strHelpMsg;
} Command;

typedef struct playerData
{
    char currentToken[64];
    char previousToken[64];
    char currentUrl[256];
    char previousUrl[256];
    PLAYER_TYPE currentType;
    PLAYER_TYPE previousType;
} playerData;


#define COMMAND_HELP            0x1     //* show help message.
#define COMMAND_QUIT            0x2     //* quit this program.

#define COMMAND_SET_SOURCE      0x101   //* set url of media file.
#define COMMAND_PLAY            0x102   //* start playback.
#define COMMAND_PAUSE           0x103   //* pause the playback.
#define COMMAND_STOP            0x104   //* stop the playback.
#define COMMAND_SEEKTO          0x105   //* seek to posion, in unit of second.
#define COMMAND_SHOW_MEDIAINFO  0x106   //* show media information.
#define COMMAND_SHOW_DURATION   0x107   //* show media duration, in unit of second.
#define COMMAND_SHOW_POSITION   0x108   //* show current play position, in unit of second.
#define COMMAND_SWITCH_AUDIO    0x109   //* switch audio track.
#define COMMAND_SETSPEED        0x10a

extern bool rtos_tone_flag;
extern RtosPlayer *gRtosPlayer;

extern RtosPlayer *rtos_player_create(void);
extern void rtos_player_set_source(RtosPlayer *rtosPlayer, char *pUrl);
extern void rtos_player_play(RtosPlayer *rtosPlayer);
extern void rtos_player_pause(RtosPlayer *rtosPlayer);
extern void rtos_player_stop(RtosPlayer *rtosPlayer);
extern int rtos_player_seekTo(RtosPlayer *rtosPlayer, int nSeekTimeMs);
extern int rtos_player_current_position(RtosPlayer *rtosPlayer);
extern int rtos_player_get_duration(RtosPlayer *p);
extern int rtos_player_set_looping(RtosPlayer *p, int bLoop);

extern void show_playback_progress(void);
extern void rtos_player_destory();
extern int rtos_player_feed(char *url, char *token, PLAYER_TYPE player_type);
extern int rtos_player_tone_feed(char *url, char *token, PLAYER_TYPE player_type) ;
extern int rtos_player_warning_feed(char *url);
extern void rtos_player_media_pause(void);
extern void rtos_player_media_resume(void);
extern void rtos_player_set_volume(int volume);
int rtos_player_task_init(/*int volume, int dev*/);


void rtos_play_pause_fun_pause(void) ;
void rtos_play_pause_fun_resume(void) ;
extern bool get_tone_play_stat(void) ;

void rtos_key_fun_music_prev(void) ;
void rtos_key_fun_music_next(void) ;
void rtos_key_fun_volume_add(void) ;
void rtos_key_fun_volume_sub(void) ;
void rtos_key_fun_pause_resume(void) ;
void rtos_key_fun_menu_change(void) ;
void rtos_key_fun_get_code(void) ;
void rtos_key_fun_wechat_play(void) ;
void rtos_key_fun_set_wifi(void) ;
void rtos_key_fun_vad_start(void) ;
void rtos_key_fun_ZtoE_start(void) ;
void rtos_key_fun_EtoZ_start(void) ;

//below function must be use paired appearances
void rtos_key_fun_AI_start(void) ;
void rtos_key_fun_AI_end(void) ;

void rtos_key_fun_wechat_start(void) ;
void rtos_key_fun_wechat_end(void) ;

void rtos_asr_wake_up_fun(uint32_t cmdId);
void asr_wakeup_callback(void);


void rtos_mqtt_connect(void) ;
void rtos_post_battery(int val) ;
void rtos_post_wechat_message(void) ;


#ifdef __cplusplus
}
#endif

#endif // _GLOBE_PLAYER_H

