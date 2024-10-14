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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globe_player.h"
#include "globe_airkiss.h"
#include "flash_play.h"
#include "globe_system_stat.h"
#include "globe_record.h"
#include "set_wifi_task.h"
#include "sys_queue.h"
#include "rk_player.h"
#include "player_pub.h"
#include "play_rcv_pub.h"

#include "sysinfo.h"
#include "hal_base.h"

#ifdef RT_USING_IOT_DISPLAY
#include "globe_display.h"
#endif
#define DBG_SECTION_NAME    "G_PLAYER"
#include "rkdebug.h"

RtosPlayer *gRtosPlayer = NULL;
extern struct playerData rtosPlayerData;
extern int playerCurrentTypeStopProcessor(playerData *PlayerData);

bool rtos_tone_flag = false ;
bool playerWarningFlag = false;

typedef struct _rtos_play_pause_fun
{
    bool stat ;
    char url[256] ;
    char token[64] ;
    PLAYER_TYPE type ;
    int seek_ms ;
} rtos_play_pause_fun_def;

//current play type is whether tone
bool current_tone  = false ;
sem_t semDialog;

uint32_t asrCmdId;
static rtos_play_pause_fun_def rtos_play_pause_fun ;

static int CallbackForMPlayer(void *pUserData, int msg, int ext1, void *param);

#define PLAYER_THREAD_STACK_SIZE    1024 * 4
rt_thread_t player_task_thread;
rt_mq_t player_task_queue;

typedef struct player_msg
{
    int play_stat;
} player_msg_t;


static void send_msg_rtos_play(int type)
{
    player_msg_t msg ;
    msg.play_stat = type;
    rtos_msg_send(player_task_queue, &msg, sizeof(player_msg_t), 5) ;
}


void rtos_play_pause_fun_clear(void) ;
static void rtos_play_pause_fun_save(char *url, char *token, PLAYER_TYPE type) ;
void rtos_music_stop(void) ;
void rtos_play_pause_fun_pause(void)
{
    int msec = 0 ;
    if (gRtosPlayer->mStatus != STATUS_PLAYING && STATUS_PREPARING != gRtosPlayer->mStatus)
    {
        // printf("===================Play Stat:%d============= ",gRtosPlayer->mStatus );
        return ;
    }
    rtos_play_pause_fun.stat = 1 ;
    MPlayerGetCurrentPosition(gRtosPlayer->mPlayer, &msec);
    rtos_play_pause_fun.seek_ms = msec ;
    // rtos_player_media_pause() ;
    rtos_music_stop() ;
    // playerCurrentTypeStopProcessor(&rtosPlayerData);
    // printf("-------------------save seek:%d ", rtos_play_pause_fun.seek_ms);
    return ;
}

void rtos_play_pause_fun_resume(void)
{
    if (rtos_play_pause_fun.stat != 1)
        return ;
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_MediaPlay) ;
    // printf("url:%s\ntoken:%s ", rtos_play_pause_fun.url , rtos_play_pause_fun.token);
    rtos_player_feed(rtos_play_pause_fun.url, rtos_play_pause_fun.token, TYPE_REUSME) ;

    return ;
}

static void rtos_play_pause_fun_save(char *url, char *token, PLAYER_TYPE type)
{
    LOGD("url:%s\ntoken:%s", url, token);
    if (type == TYPE_REUSME)
    {
        // rtos_play_pause_fun.seek_ms = 0 ;
        // rtos_play_pause_fun_clear() ;
        return ;
    }
    memset(&rtos_play_pause_fun.url, 0, 256);
    memset(&rtos_play_pause_fun.token, 0, 64);
    memcpy(&rtos_play_pause_fun.url, url, strlen(url)) ;
    memcpy(&rtos_play_pause_fun.token, token, strlen(token)) ;
    rtos_play_pause_fun.type = type ;
    rtos_play_pause_fun.seek_ms = 0 ;
    return ;
}

void rtos_play_pause_fun_clear(void)
{
    memset(rtos_play_pause_fun.url, 0, 256) ;
    memset(rtos_play_pause_fun.token, 0, 64) ;
}

void rtos_post_battery(int val)
{
    rt_thread_delay(2000);
    /*
        httpc_post_entry(&gdev_info, BATTERY_CHANGED,val);
    */
}

void rtos_player_set_source(RtosPlayer *rtosPlayer, char *pUrl)
{
    rtos_record_send(RtosRecordCmd_Cannel) ;
    rt_thread_delay(10);
    if (rtosPlayer == NULL || pUrl == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return;
    }
    LOGD(" >>> url: %s >>> \n", pUrl);
    rtosPlayer->mSeekable = 1;

    //* set url to the xPlayer.
    if (MPlayerSetDataSourceUrl(rtosPlayer->mPlayer, (const char *)pUrl) != 0)
    {
        LOGE("error:");
        LOGE("RkPlayer::setDataSource() return fail.");
        return;
    }
    LOGD("setDataSource end");

    if ((!strncmp(pUrl, "http://", 7)))
    {
        if (MPlayerPrepareAsync(rtosPlayer->mPlayer) != 0)
        {
            LOGE("[%s] Line:%d", __func__, __LINE__) ;
            LOGE("error:");
            LOGE(" xPlayer::prepareAsync() return fail.");
            // pthread_mutex_unlock(&rtosPlayer->mMutex);
            return;
        }
    }
    else
    {
        send_msg_rtos_play(PLAYER_MEDIA_PREPARED);
    }

    //* start preparing.
    rt_mutex_take(rtosPlayer->mMutex, RT_WAITING_FOREVER);
    rtosPlayer->mStatus    = STATUS_PREPARING;
    rt_mutex_release(rtosPlayer->mMutex);
    LOGD("preparing...");
}

void rtos_player_play(RtosPlayer *rtosPlayer)
{
    if (rtosPlayer == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return;
    }

    //rtos_record_send(RtosRecordCmd_Cannel) ;
    rt_thread_delay(10);
    if (MPlayerStart(rtosPlayer->mPlayer) != 0)
    {
        LOGE("error: ");
        LOGE("AwPlayer::start() return fail. ");
        rtosPlayer->mPreStatus = rtosPlayer->mStatus;
        rtosPlayer->mStatus = STATUS_STOPPED;
        rtos_music_stop() ;
        return ;
    }
    // LOGD("###seek:%d###", rtos_play_pause_fun.seek_ms);
    if (rtos_play_pause_fun.seek_ms != 0 && rtos_tone_flag == false)
    {
        LOGD("###seek:%d###", rtos_play_pause_fun.seek_ms);
        MPlayerSeekTo(rtosPlayer->mPlayer, rtos_play_pause_fun.seek_ms);
    }

    rtosPlayer->mPreStatus = rtosPlayer->mStatus;
    rtosPlayer->mStatus = STATUS_PLAYING;
    LOGD("==== playing. ==== \n");
}

void rtos_player_pause(RtosPlayer *rtosPlayer)
{
    if (rtosPlayer == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return;
    }
    if (MPlayerPause(rtosPlayer->mPlayer) != 0)
    {
        LOGE("error: ");
        LOGE(" RKPlayer::pause() return fail.");
        return;
    }
    rtosPlayer->mPreStatus = rtosPlayer->mStatus;
    rtosPlayer->mStatus = STATUS_PAUSED;
    LOGD(" ==== PAUSE ====\n");
}

void rtos_player_stop(RtosPlayer *rtosPlayer)
{
    LOGD("%s Enter!", __func__);
    if (rtosPlayer == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return;
    }
    if (MPlayerReset(rtosPlayer->mPlayer) != 0)
    {
        LOGE("error: ");
        LOGE("Player::reset() return fail.");
        return;
    }
    rt_thread_delay(10);

    rtosPlayer->mPreStatus = rtosPlayer->mStatus;
    rtosPlayer->mStatus    = STATUS_STOPPED;
    rt_sem_release(gRtosPlayer->mFinished);

    LOGD("stopped.");
}

int rtos_player_seekTo(RtosPlayer *p, int nSeekTimesMs)
{
    if (p == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return -1;
    }
    return MPlayerSeekTo(p->mPlayer, nSeekTimesMs);
}

int rtos_player_current_position(RtosPlayer *p)
{
    int msec = 0;
    return msec ;
    if (p == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return -1;
    }
    MPlayerGetCurrentPosition(p->mPlayer, &msec);
    return msec;
}

int rtos_player_get_duration(RtosPlayer *p)
{
    int msec = 0;

    if (p == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return -1;
    }
    MPlayerGetDuration(p->mPlayer, &msec);
    return msec;
}

int rtos_player_set_looping(RtosPlayer *p, int bLoop)
{
    if (p == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return -1;
    }
    return MPlayerSetLooping(p->mPlayer, bLoop);
}

typedef struct _time
{
    int32_t _day;
    int32_t _hur;
    int32_t _min;
    int32_t _sec;
    int32_t _msec;
} _Time;

void show_playback_progress(void)
{
    /*
    if (gRtosPlayer->mStatus == STATUS_PLAYING) {
        int _ms = rtos_player_current_position(gRtosPlayer);
        httpc_post_entry(&gdev_info, MEDIA_PLAYER_PROGRESS_CHANGER, _ms );
    }
    else
       httpc_post_entry(&gdev_info, DEVICE_HEARTBEAT, 0);
    */
}
extern int playerFinishedProcessor(playerData *PlayerData);

int playStatus;
bool playFinished;
//* a callback for awplayer.
static int CallbackForMPlayer(void *pUserData, int msg, int ext1, void *param)
{
    RtosPlayer *prtosPlayer = (RtosPlayer *)param;

    switch (msg)
    {
    case PLAYER_MEDIA_INFO:
    {
        switch (ext1)
        {
        case MEDIA_INFO_NOT_SEEKABLE:
        {
            prtosPlayer->mSeekable = 0;
            LOGD("info: media source is unseekable.");
            break;
        }
        case MEDIA_INFO_RENDERING_START:
        {
            LOGD("info: start to show pictures.");
            break;
        }
        }
        break;
    }

    case PLAYER_MEDIA_ERROR:
    {
        LOGD("Error 1");
        rt_mutex_take(prtosPlayer->mMutex, RT_WAITING_FOREVER);
        prtosPlayer->mStatus = STATUS_STOPPED;
        prtosPlayer->mPreStatus = STATUS_STOPPED;
        rt_mutex_release(prtosPlayer->mMutex);
        LOGE("error: open media source fail.");
        prtosPlayer->mError = 1;
        rtos_tone_flag = false ;
        playStatus = 0;
        playFinished = 1;
        send_msg_rtos_play(PLAYER_MEDIA_ERROR);
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        //playerpub_send(PLAYER_EVENT_TYPE_PLAY, PLAYER_EVENT_SUB_ERROR, 0,0);
        LOGE(" error : how to deal with it");
        break;
    }

    case PLAYER_MEDIA_PREPARED:
    {
        //logd("info : preared");
        prtosPlayer->mPreStatus = prtosPlayer->mStatus;
        prtosPlayer->mStatus = STATUS_PREPARED;
        // sem_post(&prtosPlayer->mPrepared);
        //OS_SemaphoreRelease(&gRtosPlayer->gptPrepared) ;
        //playerpub_send(PLAYER_EVENT_TYPE_PLAY, PLAYER_EVENT_SUB_PREPARED, 0,0);
        send_msg_rtos_play(PLAYER_MEDIA_PREPARED);
        LOGD("info: prepare ok.");
        break;
    }

    case PLAYER_MEDIA_PLAYBACK_COMPLETE:
    {
        LOGD(" MEDIA_PLAYBACK_COMPLETE ");
        prtosPlayer->mStatus = STATUS_STOPPED;
        if (playerWarningFlag)
        {
            playerWarningFlag = 0;
            //sem_post(&prtosPlayer->mFinished);//* stop the player.
            //OS_SemaphoreRelease(&gRtosPlayer->gptPrepared) ;
            break;
        }
        send_msg_rtos_play(PLAYER_MEDIA_PLAYBACK_COMPLETE);
        rtos_tone_flag = false ;
        //playerFinishedProcessor(&rtosPlayerData);
        //sem_post(&prtosPlayer->mSem);
        //* TODO
        break;
    }

    case PLAYER_MEDIA_SEEK_COMPLETE:
    {
        rt_mutex_take(prtosPlayer->mMutex, RT_WAITING_FOREVER);
        prtosPlayer->mStatus = prtosPlayer->mPreStatus;
        LOGD("info: seek ok.");
        rt_mutex_release(prtosPlayer->mMutex);
        break;
    }

    default:
    {
        //LOGE("warning: unknown callback from AwPlayer.");
        break;
    }
    }
    return 0;
}

void AwParserInit(void);
void AwStreamInit(void);

uint32_t g_stop_xplayer = 0;
extern volatile uint8_t button_enable;
// extern volatile uint8_t button_pressed;
extern int volume;
extern struct sysinfo e_sysinfo;

extern char intercom_url[256];
static struct sysinfo e_sysinfo_tmp;

void asr_wakeup_callback(void)
{
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_ASR_Wakeup) ;
}

//static struct sysinfo e_sysinfo_tmp;
static void key_contorl(KEYCTRL_CMD_INFO *key)
{
    LOGD("KEY CTRL id: %d  cmd: %d", key->id + 1, key->cmd);
    switch (key->id)
    {
    case KEY_CTRL_0:
        //  Short press - Previous music      Repeat--Volume up
        if (key->cmd == KEY_CTRL_CMD_SHORT_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_VolSub) ;
        }
        else if (key->cmd == KEY_CTRL_CMD_RELEASE)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_Prev) ;
        }
        break;
    //  Short press - Next music      Repeat -- Volumn donw
    case KEY_CTRL_1:
        if (key->cmd == KEY_CTRL_CMD_SHORT_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_VolAdd) ;
        }
        else if (key->cmd == KEY_CTRL_CMD_RELEASE)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_Next) ;
        }
        break;

    //  Short press -- Ligth control      Long press-- wifi Setting
    case KEY_CTRL_3:
    {
        if (key->cmd == KEY_CTRL_CMD_LONG_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_SetWifi) ;
        }
        else if (key->cmd == KEY_CTRL_CMD_SHORT_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_LEDCtrl) ;
        }
    }
    break ;

    //  Short press -- Play pause      Long press -- Menu
    case KEY_CTRL_4:
    {
        if (key->cmd == KEY_CTRL_CMD_SHORT_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_PauseResume) ;
        }
        else if (key->cmd == KEY_CTRL_CMD_SHORT_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_MenuChange) ;
        }
    }
    break ;

#ifdef RT_USING_BOARD_AUDIO_DEMO_RK2108_V10
    case KEY_CTRL_NUM:
    {
        LOGI("----------------------Recieved recovery key------------------------\n");
        if (key->cmd == KEY_CTRL_CMD_SHORT_PRESS)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_SetWifi) ;
        }
    }
    break;
#endif

    default:
        break;

    }
}


extern bool get_tone_play_stat(void)
{
    return rtos_tone_flag ;
}

//extern httpc_token_info token_info;
extern volatile uint8_t button_enable;
// extern volatile uint8_t button_pressed;

static void rtos_button_ctrl(uint32_t event, uint32_t data, void *arg)
{
    LOGD("%s Enter", __func__);
    //AD_Button_Cmd_Info* ad_bt = NULL;
    KEYCTRL_CMD_INFO *keyctrl_bt = NULL;
    //GPIO_Button_Cmd_Info *gpio_bt = NULL;
    if (EVENT_SUBTYPE(event) == CTRL_MSG_SUB_TYPE_AD_BUTTON)
    {
        keyctrl_bt = (KEYCTRL_CMD_INFO *)data;
        if (keyctrl_bt == NULL)
        {
            LOGE("invalid parameter.");
            return;
        }
        key_contorl(keyctrl_bt);
    }
    else if (EVENT_SUBTYPE(event) == CTRL_MSG_SUB_TYPE_ASR)
    {
        /* should according to data to process...*/
        asrCmdId = data;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_ASR_Wakeup);
    }
}

static bool g_wechat_flag = false ;
static uint32_t wechat_button_time_old = 0 ;


void rtos_key_fun_wechat_start(void)
{
    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        //Rtos_Flash_Fun.gptTonePlay(NEED_LOGIN_TO_CHAT) ;
        return ;
    }
    g_wechat_flag = true ;
    RtosSystemStat_Fun.SystemStat = SystemStat_Ctrl_Stat_Wechat ;
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Wechat) ;
    if (RtosSystemStat_Fun.PlayType == TYPE_MEDIA)
    {
        playerCurrentTypeStopProcessor(&rtosPlayerData);
    }
    else
    {
        rtos_music_stop() ;
    }
    LOGD("Wechat Key start");
    // playerCurrentTypeStopProcessor(&rtosPlayerData);
    // Rtos_Flash_Fun.gptTonePlay(SEND_WEBCHAT_START) ;
    rtos_record_send(RtosRecordCmd_WechatStart) ;

    wechat_button_time_old = rt_tick_get();

    return ;
}

void rtos_key_fun_wechat_end(void)
{
    LOGD("Wechat key press stop");
    g_wechat_flag = false ;

    if (rt_tick_get() - wechat_button_time_old >= 1100)
    {
        rtos_record_send(RtosRecordCmd_Stop) ;
    }
    else
    {
        rtos_record_send(RtosRecordCmd_Cannel) ;
    }
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;

    return ;
}

void rtos_key_fun_AI_start(void)
{
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_AI) ;
    if (RtosSystemStat_Fun.PlayType == TYPE_MEDIA)
    {
        playerCurrentTypeStopProcessor(&rtosPlayerData);
    }
    else
    {
        rtos_music_stop() ;
    }
    return ;
}

void rtos_key_fun_AI_end(void)
{
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;

    return ;
}

void rtos_key_fun_vad_start(void)
{
    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        return ;
    }

    RtosSystemStat_Fun.Send(SystemStat_Ctrl_LoopAI, SystemStat_Ctrl_LoopAI_Ctrl_False) ;
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_AI) ;
    rtos_record_send(RtosRecordCmd_AIStart) ;

    return ;
}

//  Chinese translate to English
void rtos_key_fun_ZtoE_start(void)
{
    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        return ;
    }


    RtosSystemStat_Fun.Send(SystemStat_Ctrl_LoopAI, SystemStat_Ctrl_LoopAI_Ctrl_False) ;
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_AI) ;
    rtos_record_send(RtosRecordCmd_ZToEStart) ;

    return ;
}

//  English translate to Chinese
void rtos_key_fun_EtoZ_start(void)
{
    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        return ;
    }

    RtosSystemStat_Fun.Send(SystemStat_Ctrl_LoopAI, SystemStat_Ctrl_LoopAI_Ctrl_False) ;
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_AI) ;
    rtos_record_send(RtosRecordCmd_EtoZStart) ;

    return ;
}

void rtos_key_fun_set_wifi(void)
{
#ifdef PRJCONF_NET_EN
    if (RtosSystemStat_Fun.static_rtos_wifi_set_stat == false)
    {
        LOGD("==== Start Config Network ====");
        RtosSystemStat_Fun.SystemStat = SystemStat_Ctrl_Stat_SetWifi ;

        if (!(netif_is_up(g_wlan_netif) && netif_is_link_up(g_wlan_netif)))
        {
        }

        g_net_stat = false ;
        playerCurrentTypeStopProcessor(&rtosPlayerData);
        RtosSystemStat_Fun.static_rtos_wifi_set_stat = true ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_SetWifi) ;

        Rtos_Flash_Fun.gptTonePlay(TONE_NET_MATCHING) ;
        rtos_wifi_set_start() ;
    }
    else
    {
        LOGD("==== Stop Net work config ====");
        RtosSystemStat_Fun.static_rtos_wifi_set_stat = false ;
        rtos_wifi_set_stop() ;

        RtosSystemStat_Fun.SystemStat = SystemStat_Ctrl_Stat_Standby ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;

        Rtos_Flash_Fun.gptTonePlay(TONE_QUIT_WIFI_SET) ;

        memset(&e_sysinfo_tmp, 0, sizeof(struct sysinfo)) ;
        networkStart(&e_sysinfo_tmp);
    }
#endif

    return ;
}

//  Play wechat voice
void rtos_key_fun_wechat_play(void)
{
    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        return ;
    }
    if (RtosSystemStat_Fun.Wechat_Data_Num == 0)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    }
    else
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_WechatPlay, 0) ;
    }

    return ;
}

//  Get Device Code
void rtos_key_fun_get_code(void)
{
    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        return ;
    }
    //  play device code
    static uint32_t old_time = 0 ;
    uint32_t current_time = rt_tick_get();
    if (current_time - old_time > 5000)
    {
        old_time = current_time ;
    }
    else
    {
        return ;
    }

    if (RtosSystemStat_Fun.MQTTFlag == false)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        return ;
    }
    RtosSystemStat_Fun.GetBindCode = true ;

    return ;
}

//  Menu switch function
void rtos_key_fun_menu_change(void)
{
    playerCurrentTypeStopProcessor(&rtosPlayerData);
    Rtos_Flash_Fun.gptMenu(RtosSystemStat_Fun.MenuFlag == false ? Rtos_Menu_Type_ChildrenSong : Rtos_Menu_Type_NULL) ;
    RtosSystemStat_Fun.MenuFlag = true ;
    return ;
}

void rtos_key_fun_pause_resume(void)
{
    if (gRtosPlayer->mStatus == STATUS_PLAYING)
    {
        LOGD(" ==== playing --> pause ===");
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Pause) ;
        rtos_player_pause(gRtosPlayer);
        return;
    }
    else if (gRtosPlayer->mStatus == STATUS_PAUSED)
    {
        LOGD(" ==== pause --> playing ===");
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_MediaPlay) ;
        rtos_player_play(gRtosPlayer);
        return;
    }

    LOGD("====== Player status:%d======", gRtosPlayer->mStatus);

    return ;
}

//  Volume down
void rtos_key_fun_volume_sub(void)
{
    if (gRtosPlayer->mStatus != STATUS_PLAYING)
        return;
    if (volume - 10 >= 0)
    {
        RtosSystemStat_Fun.KeyPostFlag = true ;
        volume = volume - 10;
        rtos_player_set_volume(volume);
        LOGD("volume - 10 = ##%d##", volume);
    }
    else
    {
        volume = 0 ;
        LOGD("##WRN: The volume has been set to a minimum");
    }

    return ;
}

//  Volumn up
void rtos_key_fun_volume_add(void)
{
    if (gRtosPlayer->mStatus != STATUS_PLAYING)
        return ;
    if (volume + 10 <= 100)
    {
        RtosSystemStat_Fun.KeyPostFlag = true ;
        volume = volume + 10;
        if (volume > 90)
        {
            volume = 100 ;
        }
        rtos_player_set_volume(volume);
        LOGD("volume + 10 = ##%d##", volume);
    }
    else
    {
        LOGD("##WRN:The volume has been set to maximum");
        volume = 100 ;
        rtos_player_set_volume(volume);
    }

    return ;
}

//  Next music
void rtos_key_fun_music_next(void)
{
    LOGD("%s(): Enter, cur type = %d", __func__, rtosPlayerData.currentType);
    playerCurrentTypeStopProcessor(&rtosPlayerData);
    Rtos_Flash_Fun.gptPlayNextPrev(Rtos_Play_Mode_Next) ;
    return ;
}


//  Previous music
void rtos_key_fun_music_prev(void)
{
    LOGD("%s(): Enter, cur type = %d", __func__, rtosPlayerData.currentType);
    playerCurrentTypeStopProcessor(&rtosPlayerData);
    Rtos_Flash_Fun.gptPlayNextPrev(Rtos_Play_Mode_Prev) ;
    return ;
}

#ifdef RT_USING_IOT_DISPLAY
extern const char *asr_ask_respond_str[];
extern const char *asr_cmd_respond_str[];
extern display_callback display_cb;
#endif

extern Tone_Name g_toneName[TONE_NUMBER_MAX];
void rtos_asr_wake_up_fun(uint32_t cmdId)
{
    uint32_t id;
#ifndef PRJCONF_AI_CUSTOM
    cmdId -= 1; /* wakeup word is 0, other cmd is larger 1*/
#endif
    LOGI("%s(), cmdId = %d", __func__, cmdId);
    if (cmdId < 0)
    {
        LOGE("Error cmd id");
        return;
    }

    if (cmdId == 0)
    {
        id = GetRandom(5);   // play tone 1 - 4 random to play

        LOGD("%s : asr wake up, play /notice/%d.mp3", __func__, id);
        rtos_music_stop();
        Rtos_Flash_Fun.gptTonePlay(id);

#ifdef RT_USING_IOT_DISPLAY
        /* display wake up respond string */
        if (display_cb)
        {
            LOGI("%d : %s  len = %d", id,  asr_ask_respond_str[id], rt_strlen(asr_ask_respond_str[id]));
            display_cb(id, (char *)asr_ask_respond_str[id], rt_strlen(asr_ask_respond_str[id]));
        }
#endif
    }

#if defined(PRJCONF_AI_SPEECH) || defined(PRJCONF_AI_CUSTOM)
    if (cmdId >= 1)
    {
        id = cmdId + TONE_ANSWER4;
        LOGI("cmd Id :%d, cmd: ## %s ##", cmdId, g_toneName[id].cmd_str);
        LOGI("asr wake up, play /notice/%s", g_toneName[id].tone_name);
        rtos_music_stop();
        Rtos_Flash_Fun.gptTonePlay(id);

#ifdef RT_USING_IOT_DISPLAY
        /* display cmd respond string */
        if (display_cb)
        {
            LOGI("%d: %s , len = %d", cmdId, asr_cmd_respond_str[cmdId], rt_strlen(asr_cmd_respond_str[cmdId]));
            display_cb(cmdId, (char *)asr_cmd_respond_str[cmdId], rt_strlen(asr_cmd_respond_str[cmdId]));
        }
#endif
    }
#endif
}

RtosPlayer *rtos_player_create(void)
{
    //* create a player.
    gRtosPlayer = rt_malloc(sizeof(RtosPlayer));
    if (gRtosPlayer == NULL)
        LOGE("Allocate RtosPlayer object failed.");
    memset(gRtosPlayer, 0, sizeof(RtosPlayer));
    gRtosPlayer->mStatus = STATUS_STOPPED;
    gRtosPlayer->mMutex = rt_mutex_create("RtosPlayer_mut", RT_IPC_FLAG_FIFO);
    gRtosPlayer->mPrepared = rt_sem_create("RtosPlayer_Prepared", 0, RT_IPC_FLAG_FIFO);
    gRtosPlayer->mFinished = rt_sem_create("RtosPlayer_Finished", 0, RT_IPC_FLAG_FIFO);

    MPlayerSetHttpBuffer(gRtosPlayer->mPlayer, 20 * 1024, 10 * 1024);

    gRtosPlayer->mPlayer = MPlayerCreate();
    if (gRtosPlayer->mPlayer == NULL)
    {
        LOGE("can not create Player, quit.");
        free(gRtosPlayer);
        return NULL;
    }
    else
    {
        LOGD("create Player success.");
    }

    //* set callback to player.
    MPlayerSetNotifyCallback(gRtosPlayer->mPlayer, CallbackForMPlayer, (void *)gRtosPlayer);

    //* check if the player work.
    if (MPlayerInitCheck(gRtosPlayer->mPlayer) != 0)
    {
        LOGE("initCheck of the player fail, quit.");
        MPlayerDestroy(gRtosPlayer->mPlayer);
        gRtosPlayer->mPlayer = NULL;
        rt_free(gRtosPlayer);
        gRtosPlayer = NULL;
        return NULL;
    }
    else
        LOGD("AwPlayer check success.");

    return gRtosPlayer;
}

void rtos_player_destory()
{
    RtosPlayer *rtosPlayer = gRtosPlayer;
    if (rtosPlayer == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return;
    }
    LOGD("destroy AwPlayer.");
}
int rtos_player_warning_feed(char *url)
{
    if (url == NULL)
    {
        LOGE("%s:Invalid parameter, please check!", __func__);
        return -1;
    }
    rtos_record_send(RtosRecordCmd_Cannel) ;
    LOGD("%s start \n", __func__);
    playerWarningFlag = 1;
    rtos_player_stop(gRtosPlayer);
    rtos_player_set_source(gRtosPlayer, url);
    LOGD("%s end \n", __func__);
    return 0;
}

void rtos_post_wechat_message(void)
{
    return ;
}

void rtos_player_media_pause(void)
{
    if (playStatus)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        rtos_player_pause(gRtosPlayer);//pause
    }
}

void rtos_player_media_resume(void)
{
    if (playStatus)
    {
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_MediaPlay) ;
        rtos_player_play(gRtosPlayer);
    }
}

rt_timer_t RTOS_Vol_Timer;


/* static */int globe_vol = 0 ;
void VolTimerCallback(void *data)
{
    if (RtosSystemStat_Fun.KeyPostFlag == true)
    {
        /*httpc_post_entry(&gdev_info, RtosSystemStat_Fun.KeyPostFlag == true ? VOLUME_CHANGE_DEVICE : VOLUME_CHANGED, globe_vol);*/
    }
    RtosSystemStat_Fun.KeyPostFlag = false ;
    rt_timer_stop(RTOS_Vol_Timer);
    return ;
}
void rtos_player_set_volume(int volume)
{
    /* TODO: to do something ......*/
    //
}


struct playerData rtosPlayerData;
bool cyclicFlag;


int rtos_player_feed(char *url, char *token, PLAYER_TYPE player_type)
{
    //if current system status is wifi or wechat record or vad record, don't allow to play
    if (RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_Wechat || \
            RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_SetWifi || \
            RtosSystemStat_Fun.GetSystemStat() == SystemStat_Ctrl_Stat_PowerOff)
    {
        return -1 ;
    }

    rtos_music_stop() ;

    rtos_tone_flag = false ;
    if (TYPE_REUSME == player_type)
    {
        memset(&rtosPlayerData, 0, sizeof(struct playerData));
        memcpy(&rtosPlayerData.currentToken, rtos_play_pause_fun.token, strlen(rtos_play_pause_fun.token));
        memcpy(&rtosPlayerData.currentUrl, rtos_play_pause_fun.url, strlen(rtos_play_pause_fun.url));
        rtosPlayerData.currentType = rtos_play_pause_fun.type ;
        RtosSystemStat_Fun.PlayType = rtos_play_pause_fun.type ;
    }
    else if (player_type == TYPE_TONE)
    {
        rtos_tone_flag = true ;
        memset(&rtosPlayerData, 0, sizeof(struct playerData));
        memcpy(&rtosPlayerData.currentUrl, url, strlen(url));
        rtosPlayerData.currentType = player_type;
    }
    else
    {
        memset(&rtosPlayerData, 0, sizeof(struct playerData));
        memcpy(&rtosPlayerData.currentToken, token, strlen(token));
        memcpy(&rtosPlayerData.currentUrl, url, strlen(url));
        rtosPlayerData.currentType = player_type;

        RtosSystemStat_Fun.PlayType = player_type ;
    }
    rtos_play_pause_fun.stat = 0 ;
    // rtos_record_stop() ;
    rtos_record_send(RtosRecordCmd_Cannel) ;
    rt_thread_delay(10);

    if (player_type == TYPE_TONE)
    {
        // RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat,SystemStat_Ctrl_Stat_Standby) ;
        current_tone = true ;
    }
    else
    {
        current_tone = false ;
        // RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat,SystemStat_Ctrl_Stat_MediaPlay) ;
    }

    send_msg_rtos_play(PLAYER_MEDIA_STARTED);
    return 0;
}

int rtos_player_tone_feed(char *url, char *token, PLAYER_TYPE player_type)
{
    if (player_type == TYPE_TONE)
    {
        current_tone = true ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    }
    else
    {
        current_tone = false ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_MediaPlay) ;
    }

    if (RtosSystemStat_Fun.PlayType == TYPE_MEDIA)
    {
        playerCurrentTypeStopProcessor(&rtosPlayerData);
    }
    else
    {
        rtos_music_stop() ;
    }

    rtos_tone_flag = false ;
    if (TYPE_REUSME == player_type)
    {
        memset(&rtosPlayerData, 0, sizeof(struct playerData));
        memcpy(&rtosPlayerData.currentToken, rtos_play_pause_fun.token, strlen(rtos_play_pause_fun.token));
        memcpy(&rtosPlayerData.currentUrl, rtos_play_pause_fun.url, strlen(rtos_play_pause_fun.url));
        rtosPlayerData.currentType = rtos_play_pause_fun.type ;
        RtosSystemStat_Fun.PlayType = rtos_play_pause_fun.type ;
    }
    else if (player_type == TYPE_TONE)
    {
        rtos_tone_flag = true;
        memset(&rtosPlayerData, 0, sizeof(struct playerData));
        memcpy(&rtosPlayerData.currentUrl, url, strlen(url));
        rtosPlayerData.currentType = player_type;
    }
    else
    {
        memset(&rtosPlayerData, 0, sizeof(struct playerData));
        memcpy(&rtosPlayerData.currentToken, token, strlen(token));
        memcpy(&rtosPlayerData.currentUrl, url, strlen(url));
        rtosPlayerData.currentType = player_type;
        RtosSystemStat_Fun.PlayType = player_type ;
    }

    rtos_play_pause_fun.stat = 0 ;
    rtos_record_send(RtosRecordCmd_Cannel) ;
    if (player_type != TYPE_TONE)
        rtos_play_pause_fun_save(url, token, player_type) ;
    LOGD("=====Tone Play=====");

    send_msg_rtos_play(PLAYER_MEDIA_STARTED);
    return 0;
}


void player_type_ctrl(uint32_t event, uint32_t data, void *arg)
{
}

int playerPreviousTypeStopProcessor(playerData *PlayerData)
{
    if (PlayerData == NULL)
    {
        LOGE("PlayerData is NULL!!!");
        return -1;
    }
    LOGD(">>>>playerPreviouTypeStopProcessor<<<<<");
    if (PlayerData->previousType == TYPE_NULL)
        return 0;
    switch (PlayerData->previousType)
    {
    /*case TYPE_MEDIA:
        httpc_post_entry(&gdev_info, MEDIA_PLAYER_STOPPED,
                            rtos_player_current_position(gRtosPlayer));
        break;
    case TYPE_DIALOG:
        httpc_post_entry(&gdev_info, AI_DIALOG_STOPPED, 0);
        break;
    case TYPE_INTERCOM:
        httpc_post_entry(&gdev_info, INTERCOM_STOPPED, 0);
        break;
    case TYPE_ALERT:
        httpc_post_entry(&gdev_info, ALERT_TERMINATED, 0);
        break;
    */
    default:
        break;
    }
    return 0;
}
bool currentStop;
bool alertStop;
bool alertPlaying;
int optWait;


void rtos_music_stop(void)
{
    cyclicFlag = 0;
    rtos_player_stop(gRtosPlayer);
    currentStop = 1;
    playStatus = 0;
    return ;
}


int playerCurrentTypeStopProcessor(playerData *PlayerData)
{
    if (PlayerData == NULL)
    {
        LOGE("PlayerData is NULL!!!");
        return -1;
    }
    cyclicFlag = 0;
    LOGD(">>>>playerCurrentTypeStopProcessor<<<<<");
    rtos_player_stop(gRtosPlayer);
    switch (PlayerData->currentType)
    {
    case TYPE_MEDIA:
        currentStop = 1;
        playStatus = 0;
        break;
    case TYPE_MEDIA_NORFLASH:
        currentStop = 1;
        playStatus = 0;
        break;
    case TYPE_MEDIA_SD:
        currentStop = 1;
        playStatus = 0;
        break;
    case TYPE_TONE:
        currentStop = 1;
        playStatus = 0;
        break;
    case TYPE_DIALOG:
        optWait = 0;
        break;
    case TYPE_INTERCOM:
        break;
    case TYPE_ALERT:
        if (alertPlaying)
        {
            alertStop = 1;
        }
        break;
    default:
        break;
    }
    return 0;
}


int playerFinishedProcessor(playerData *PlayerData)
{
    if (PlayerData == NULL)
    {
        LOGE("PlayerData is NULL!!!");
        return -1;
    }
    if (PlayerData->currentType == TYPE_NULL)
        return 0;

    LOGD(">>>>playerFinishedProcessor<<<<<");
    switch (PlayerData->currentType)
    {
    case TYPE_MEDIA:
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
        /*
        httpc_post_entry(&gdev_info, MEDIA_PLAYER_FINISHED, \
                           rtos_player_current_position(gRtosPlayer));
        */
        playStatus = 0;
        playFinished = 1;
        // httpc_post_entry(&gdev_info, MEDIA_PLAYER_NEXT_DEVICE, 0);
        // RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl,SystemStat_Ctrl_KeyCtrl_Next) ;
        break;
    case TYPE_MEDIA_NORFLASH:
        // RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat,SystemStat_Ctrl_Stat_Standby) ;
        playStatus = 0;
        playFinished = 1;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_Next) ;
        // Rtos_Flash_Fun.gptPlayNextPrev(Rtos_Play_Mode_Next) ;
        break;
    case TYPE_MEDIA_SD:
        // RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat,SystemStat_Ctrl_Stat_Standby) ;
        playStatus = 0;
        playFinished = 1;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_Next) ;
        // Rtos_SD_Fun.gptPlayNextPrev(Rtos_Play_Mode_Next) ;
        break;
    case TYPE_TONE:
        playStatus = 0;
        playFinished = 1;
        rtos_tone_flag = false ;
        break ;
    case TYPE_DIALOG:
        playStatus = 0;
        playFinished = 1;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
//            httpc_post_entry(&gdev_info, AI_DIALOG_FINISHED, 0);
//            sem_post(&semDialog);
        optWait = 0;
        if (RtosSystemStat_Fun.UrlInfo.Flag)
        {
            RtosSystemStat_Fun.UrlInfo.Flag = false ;
            RtosSystemStat_Fun.LoopAI = false ;
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_PlayHttp, 0) ;

            break ;
        }
        if (RtosSystemStat_Fun.LoopAI)
        {
            //imitate_vad() ;
        }
        else
        {

        }
        break;
    case TYPE_INTERCOM:
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
//            httpc_post_entry(&gdev_info, INTERCOM_FINISHED, 0);
        if (RtosSystemStat_Fun.Wechat_Data_Num != 0)
        {
            RtosSystemStat_Fun.Send(SystemStat_Ctrl_WechatPlay, 0) ;
        }

        break;
    case TYPE_ALERT:
//            httpc_post_entry(&gdev_info, ALERT_FINISHED, 0);s
        break;
    default:
        break;
    }
    return 0;
}


void rtos_player_task()
{
    int ret = 0;
    int rtos_play_run = 1;
    player_msg_t msg  ;
    while (rtos_play_run)
    {
        memset(&msg, 0, sizeof(player_msg_t)) ;
        ret = rtos_msg_recv(player_task_queue, &msg, RT_WAITING_FOREVER) ;
        if (ret != 0)
        {
            rt_thread_delay(20);
            continue;
        }

        if (msg.play_stat == PLAYER_MEDIA_STARTED)   //set url and start play
        {
            switch (rtosPlayerData.currentType)
            {
            case TYPE_MEDIA:
                if (!playFinished && !currentStop)
                {
                    /*
                    httpc_post_entry(&gdev_info, MEDIA_PLAYER_STOPPED, \
                           rtos_player_current_position(gRtosPlayer));
                    */
                }
                else
                {
                    playFinished = 0;
                    currentStop = 0;
                }
                rtos_player_stop(gRtosPlayer);
                rtos_player_set_source(gRtosPlayer, rtosPlayerData.currentUrl);
                break;
            case TYPE_MEDIA_NORFLASH:
                if (!playFinished && !currentStop)
                {
                }
                else
                {
                    playFinished = 0;
                    currentStop = 0;
                }
                rtos_player_stop(gRtosPlayer);
                rtos_player_set_source(gRtosPlayer, rtosPlayerData.currentUrl);
                break;
            case TYPE_MEDIA_SD:
                if (!playFinished && !currentStop)
                {
                }
                else
                {
                    playFinished = 0;
                    currentStop = 0;
                }
                rtos_player_stop(gRtosPlayer);
                rtos_player_set_source(gRtosPlayer, rtosPlayerData.currentUrl);
                break;
            case TYPE_TONE:
                LOGD("TYPE_TONE start");
                if (!playFinished && !currentStop)
                {
                }
                else
                {
                    playFinished = 0;
                    currentStop = 0;
                    LOGD("TYPE_TONE -----------------------");
                }
                rtos_player_stop(gRtosPlayer);
                rtos_player_set_source(gRtosPlayer, rtosPlayerData.currentUrl);
                LOGD("TYPE_TONE end");
                break;
            case TYPE_DIALOG:
                if (!playFinished && !currentStop)
                {
                }
                else
                {
                    playFinished = 0;
                    currentStop = 0;
                }
                rtos_player_stop(gRtosPlayer);
                rtos_player_set_source(gRtosPlayer, rtosPlayerData.currentUrl);
                break;
            case TYPE_INTERCOM:
                rtos_player_stop(gRtosPlayer);
                rtos_player_set_source(gRtosPlayer, rtosPlayerData.currentUrl);
                break;
            case TYPE_ALERT:
            {
                break;
            }
            default:
                break;
            }
        }
        else if (msg.play_stat == PLAYER_MEDIA_PREPARED)     //read to play
        {
            LOGI("--------- prepared to play ---------");
            rtos_player_play(gRtosPlayer);
            if (current_tone == true)
            {
                LOGI("--------- current_tone = true ---------");
            }
            else
            {
                RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_MediaPlay) ;
            }
            if (playerWarningFlag == 1)   //rtos_player_warning_feed
            {
                playerWarningFlag = 0;
            }
            else
            {
                switch (rtosPlayerData.currentType)
                {
                case TYPE_MEDIA:
                case TYPE_MEDIA_NORFLASH:
                case TYPE_MEDIA_SD:
                case TYPE_TONE:
                    //rtos_player_play(gRtosPlayer);
                    playStatus = 1;

                    break;
                case TYPE_INTERCOM:
                    //rtos_player_play(gRtosPlayer);
                    break;
                case TYPE_ALERT:
                    break;
                default:
                    break;
                }
            }
        }
        else if (msg.play_stat == PLAYER_MEDIA_ERROR)
        {
            //nothing to do
        }
        else if (msg.play_stat == PLAYER_MEDIA_PLAYBACK_COMPLETE)
        {
            LOGD("================== playback complete ===================");
            rtos_tone_flag = false ;
            playerFinishedProcessor(&rtosPlayerData);
        }
    }

    rt_thread_delete(player_task_thread);
}

int rtos_player_task_init(/*int volume, int dev*/)
{
    player_init();
    player_register_mp3dec();
    rtos_player_create();
    rtos_msg_init(&player_task_queue, "rtos_player_task", 10, sizeof(player_msg_t));
    //LOGD("============ player_task_queue = 0x%08x ==========",player_task_queue);

    player_task_thread = rt_thread_create("rtos_player_task",
                                          rtos_player_task,
                                          RT_NULL,
                                          PLAYER_THREAD_STACK_SIZE,
                                          RT_THREAD_PRIORITY_MAX / 2, 10);
    if (player_task_thread)
    {
        LOGD("rtos_player_task thread create OK");
        rt_thread_startup(player_task_thread);
    }
    else
    {
        if (player_task_queue)
        {
            LOGD("do nothing");
        }
        LOGD("rtos_player_task_init thread create error");
        return -1;
    }

    //  Key ctrl event process
    /**< create a observer */
    observer_base *record_button_obs = sys_callback_observer_create(CTRL_MSG_TYPE_VKEY,
                                       CTRL_MSG_SUB_TYPE_ALL, rtos_button_ctrl, NULL);
    if (record_button_obs == NULL)
    {
        LOGD("create record_button_obs failed.");
    }
    sys_ctrl_attach(record_button_obs);
    return 0;
}

