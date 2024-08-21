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

#ifndef _GLOBE_SYSTEM_STAT_H_
#define _GLOBE_SYSTEM_STAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <rtthread.h>
#include <rtdef.h>


typedef enum _RtosSystemStat_Ctrl
{
    SystemStat_Ctrl_Bat,
    SystemStat_Ctrl_Stat,
    SystemStat_Ctrl_LoopAI,      //  Loop AI
    SystemStat_Ctrl_Sleep,
    SystemStat_Ctrl_PlayHttp,    //  play saved http audio
    SystemStat_Ctrl_Net,
    SystemStat_Ctrl_WechatPlay,  // play wechat message
    SystemStat_Ctrl_LED,
    SystemStat_Ctrl_PlayCtrl,    //  0 - play 1 - pause 2 - previous 3 - next 4 - key control play or pause
    SystemStat_Ctrl_VolumeCtrl,  //  volume control   0--- volume down  1 --- volume up
    SystemStat_Ctrl_KeyCtrl,    //  keyctrl event process
    SystemStat_Ctrl_NULL,
} RtosSystemStat_Ctrl;

typedef enum _SystemStat_Ctrl_KeyCtrl
{
    SystemStat_Ctrl_KeyCtrl_Prev,
    SystemStat_Ctrl_KeyCtrl_Next,
    SystemStat_Ctrl_KeyCtrl_VolAdd,
    SystemStat_Ctrl_KeyCtrl_VolSub,
    SystemStat_Ctrl_KeyCtrl_PauseResume,
    SystemStat_Ctrl_KeyCtrl_MenuChange,
    SystemStat_Ctrl_KeyCtrl_GetCode,
    SystemStat_Ctrl_KeyCtrl_WechatPlay,
    SystemStat_Ctrl_KeyCtrl_SetWifi,
    SystemStat_Ctrl_KeyCtrl_Vad,
    SystemStat_Ctrl_KeyCtrl_ZtoE,
    SystemStat_Ctrl_KeyCtrl_EtoZ,
    SystemStat_Ctrl_KeyCtrl_AIStart,
    SystemStat_Ctrl_KeyCtrl_AIEnd,
    SystemStat_Ctrl_KeyCtrl_WechatStart,
    SystemStat_Ctrl_KeyCtrl_WechatEnd,
    SystemStat_Ctrl_KeyCtrl_LEDCtrl,
    SystemStat_Ctrl_ASR_Wakeup,
    SystemStat_Ctrl_KeyCtrl_NULL,

} SystemStat_Ctrl_KeyCtrl_Def;


typedef enum _SystemStat_Ctrl_LoopAI_Ctrl
{
    SystemStat_Ctrl_LoopAI_Ctrl_True,
    SystemStat_Ctrl_LoopAI_Ctrl_False,
    SystemStat_Ctrl_LoopAI_Ctrl_NULL,

} SystemStat_Ctrl_LoopAI_Ctrl;

typedef enum SystemStat_Cmd
{
    SystemStat_Ctrl_Stat_PowerOn,                //
    SystemStat_Ctrl_Stat_PowerOff,               //  power off
    SystemStat_Ctrl_Stat_Init,                   //  system init
    SystemStat_Ctrl_Stat_MediaPlay,              //  net media play
    SystemStat_Ctrl_Stat_MediaNorFlashPlay,      //  NorFlash audio play
    SystemStat_Ctrl_Stat_MediaSDPlay,            //  SD audio play
    SystemStat_Ctrl_Stat_TTSPlay,                //  TTS audi play
    SystemStat_Ctrl_Stat_WeChatPlay,             //  wecaht voice play
    SystemStat_Ctrl_Stat_AlarmPlay,              //  alert play
    SystemStat_Ctrl_Stat_Standby,                //  stanby
    SystemStat_Ctrl_Stat_AI,                     //  AI dialog recording
    SystemStat_Ctrl_Stat_Wechat,                 //  wechat recording
    SystemStat_Ctrl_Stat_OTA,                    //  OTA statue
    SystemStat_Ctrl_Stat_SetWifi,                //  wifi setting
    SystemStat_Ctrl_Stat_Pause,                  //  pause
    SystemStat_Ctrl_Stat_Test,                   //  PCBA test mode
    SystemStat_Ctrl_Stat_NetInit,                //  net init statue
    SystemStat_Ctrl_Stat_NULL,
} SystemStat_Ctrl_Stat_Cmd;

#define RTOS_URL_LEN_DEF                             512
#define RTOS_TOKEN_LEN_DEF                           64
#define RTOS_WECHAT_URL_LEN_DEF                      256
typedef struct _Http_Music_Info
{
    char Url[RTOS_URL_LEN_DEF] ;
    char Token[RTOS_TOKEN_LEN_DEF] ;
    bool Flag ;
} Http_Music_Info_Def;

// wecht message save
typedef struct _Rtos_Wechat_Data
{
    char Url[RTOS_URL_LEN_DEF] ;
    char Token[RTOS_TOKEN_LEN_DEF] ;
    struct _Rtos_Wechat_Data *Next ;
} Rtos_Wechat_Data_Def;

typedef struct _RtosSystemStat_Fun
{
//  public
    int (*Send)(RtosSystemStat_Ctrl sub, uint32_t data) ;
    SystemStat_Ctrl_Stat_Cmd(*GetSystemStat)(void) ;
    bool LoopAI ;
    uint8_t LoopAI_Type ;
    bool VAD_Flag ;
    bool GetBindCode ;

    uint8_t PlayType ;

    void (*SetHttpUrl)(char *url, char *token) ;
    char *(*GetHttpUrl)(char *url, char *token) ;

    uint8_t Wechat_Data_Num ;
    Rtos_Wechat_Data_Def *WechatDataStart ;
    Rtos_Wechat_Data_Def *WechatDataEnd ;
    bool (*WechatDataGet)(Rtos_Wechat_Data_Def *data) ;
    Rtos_Wechat_Data_Def *(*WechatDataAdd)(char *url, char *token) ;

//  private:
    bool Run ;
    rt_thread_t Thread;
    void *Obs ;
    rt_sem_t Semaphore;

    SystemStat_Ctrl_Stat_Cmd SystemStat ;

    uint16_t MsgCmd ;
    uint32_t MsgData ;

    rt_timer_t Rtos_Sleep_Timer ;
    uint16_t Sleep_Count ;
    bool static_rtos_wifi_set_stat ;

    char ChipID[40] ;
    Http_Music_Info_Def UrlInfo ;               //      Url data

    bool MenuFlag ;
    int Battery ;                               //  curr batter value

    bool KeyPostFlag ;
    bool MQTTFlag ;                             //  conn MQTT flag，true -- conn succ false -- conn fail
    bool NetConnecting ;                        //  flag of net connecting，not allow to press key
} RtosSystemStat_Fun_Def;

extern RtosSystemStat_Fun_Def RtosSystemStat_Fun ;


typedef struct _RtosTestModeFun
{
    bool OTA ;
    bool OTAFlag ;
    bool TestFlag ;
    bool TestMode ;             //  false------unused        true------used

    uint8_t WifiStat ;          //  0------scaning    1------scan success    2------scan fail

} RtosTestModeFun_Def;

extern RtosTestModeFun_Def RtosTestModeFun ;


void RtosSystemStatInit(void);

#endif

