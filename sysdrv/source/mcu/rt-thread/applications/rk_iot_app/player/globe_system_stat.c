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

#include "globe_system_stat.h"
#include "batt_task.h"
#include "flash_play.h"
#include "player_pub.h"
#include "play_rcv_pub.h"
#include "globe_player.h"
#include "net_ctrl.h"
#include "globe_airkiss.h"
#include "wlan_api.h"

#include "sys_queue.h"
#include "sys_ctrl.h"

#include "rkdebug.h"

#define RTOS_SYSTEM_STACK_SIZE       (6 * 1024)
#define RTOS_SLEEP_COUNT             (5 * 60 )   // 5 minites no ops will power off
#define RTOS_SYSTEM_STAT_QUEUE_NUM   (10)

RtosSystemStat_Fun_Def RtosSystemStat_Fun ;
RtosTestModeFun_Def RtosTestModeFun ;

rtos_queue_head_def *rtos_system_stat_queue = NULL ;

typedef struct _rtos_system_stat_msg
{
    uint16_t cmd ;
    uint32_t data ;
} rtos_system_stat_msg_def;


static int RtSystemStat_Ctrl_Stat_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_LoopAI_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_Bat_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_Sleep_Fun(uint32_t data) ;
static void RtSystemStat_PowerOff(void) ;
static SystemStat_Ctrl_Stat_Cmd rtos_system_stat_get(void) ;
void rtos_Sleep_Start(void) ;
// static char get_char(uint8_t data) ;
static int rtos_get_chipid(void) ;
static void RtSystemStat_Ctrl_PlayHttp_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_Net_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_WechatPlay_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_LED_Fun(uint32_t data) ;
static void RtSystemStat_Ctrl_ButtonCtrl_Fun(uint32_t data) ;

extern uint32_t asrCmdId;
extern struct netif *g_wlan_netif;

static void rtos_system_stat_Task(void *arg)
{
    int ret = -1 ;
    rtos_system_stat_msg_def *rec_data = NULL ;

    while (RtosSystemStat_Fun.Run)
    {
        if (rtos_system_stat_queue->non_empty == 0)
        {
            rt_sem_take(RtosSystemStat_Fun.Semaphore, RT_WAITING_FOREVER);
        }

        rt_thread_delay(5);

        LOGD(" ===== rtos_system_stat_Task ===== ");
        rec_data = (rtos_system_stat_msg_def *)rtos_queue_get(rtos_system_stat_queue) ;
        if (rec_data != 0)
        {
            RtosSystemStat_Fun.MsgCmd = rec_data->cmd ;
            RtosSystemStat_Fun.MsgData = rec_data->data ;
            //LOGD("MsgCmd = %d, MsgData = %d  ", rec_data->cmd,rec_data->data);
            ret = rtos_queue_del(rtos_system_stat_queue) ;
            if (ret == -1)
            {
                LOGE("[rtos_System] rtos_queue_del error") ;
            }
            rt_free(rec_data) ;
            rec_data = NULL ;
        }

        if (RtosSystemStat_Fun.MsgCmd == SystemStat_Ctrl_NULL)
        {
            continue ;
        }

        switch (RtosSystemStat_Fun.MsgCmd)
        {
        case SystemStat_Ctrl_Bat :
        {
            //  battery event
            // LOGD(" ==== Battery event start ==== " );
            RtSystemStat_Ctrl_Bat_Fun(RtosSystemStat_Fun.MsgData) ;
            // LOGD(" ==== Battery event end ==== " );
        }
        break ;
        case SystemStat_Ctrl_Stat:
        {
            //  Modify system status
            // LOGD(" ==== Modify system status start ==== " );
            RtSystemStat_Ctrl_Stat_Fun(RtosSystemStat_Fun.MsgData) ;
            // LOGD(" ==== Modify system status end ==== " );

        }
        break ;
        case SystemStat_Ctrl_LoopAI:
        {
            //  Muti Q & A
            // LOGD(" ==== Muti Questions and answers start ==== " );
            RtSystemStat_Ctrl_LoopAI_Fun(RtosSystemStat_Fun.MsgData) ;
            // LOGD(" ==== Muti Questions and answers end ==== " );
        }
        break ;
        case SystemStat_Ctrl_Sleep:
        {
            //  Auto power off
            LOGD("=== Auto Power off start ==== ");
            RtSystemStat_Ctrl_Sleep_Fun(RtosSystemStat_Fun.MsgData) ;
            LOGD(" ==== Auto Power off start end ==== ");
        }
        break ;
        case SystemStat_Ctrl_PlayHttp:
        {
            //  Play http audio
            // LOGD(" ==== Play http audio start ==== " );
            RtSystemStat_Ctrl_PlayHttp_Fun(RtosSystemStat_Fun.MsgData) ;
            // LOGD(" ==== Play http audio end ==== " );

        }
        break ;
        case SystemStat_Ctrl_Net:
        {
            //  Play tone of on or off network
            LOGD(" ==== Play tone of on line or off line start ==== ");
            RtSystemStat_Ctrl_Net_Fun(RtosSystemStat_Fun.MsgData) ;
            LOGD("===== Play tone of on line or off line end ==== ");

        }
        break ;
        case SystemStat_Ctrl_WechatPlay:
        {
            //  Play wechat message
            // LOGD("==== play wechat message start ==== " );
            RtSystemStat_Ctrl_WechatPlay_Fun(RtosSystemStat_Fun.MsgData) ;
            // LOGD("==== play wechat message end ==== " );
        }
        break ;
        case SystemStat_Ctrl_LED:
        {
            //  Light control
            // LOGD("==== light ctrol start ==== " );
            RtSystemStat_Ctrl_LED_Fun(RtosSystemStat_Fun.MsgData) ;
            // LOGD("==== ligth ctrol end ==== " );

        }
        break ;
        case SystemStat_Ctrl_PlayCtrl:
        {
            //  Play control

        } break ;
        case SystemStat_Ctrl_KeyCtrl:
        {
            //  Key msg process
            LOGD(" ==== Key message process start ==== ");
            RtSystemStat_Ctrl_ButtonCtrl_Fun(RtosSystemStat_Fun.MsgData) ;
            LOGD(" ==== Key message process end ==== ");
        }
        break ;
        default:
        {

        } break ;
        }

        RtosSystemStat_Fun.MsgCmd = SystemStat_Ctrl_NULL ;
        RtosSystemStat_Fun.MsgData = 0 ;
    }

    rt_thread_delete(RtosSystemStat_Fun.Thread);
    return ;
}

static void RtSystemStat_Ctrl_ButtonCtrl_Fun(uint32_t data)
{
    LOGI("ButtonCtrl_Fun Enter: data = %d", data);
    switch (data)
    {
    case SystemStat_Ctrl_KeyCtrl_Prev:
    {
        LOGD("/// Prev - Start ///");
        rtos_key_fun_music_prev() ;
        LOGD("/// Prev - End ///");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_Next:
    {
        LOGD("/// Next - Start ///");
        rtos_key_fun_music_next() ;
        LOGD("/// Next - End ///");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_VolAdd:
    {
        LOGD("/// VolAdd - Start ///");
        rtos_key_fun_volume_add() ;
        LOGD("/// VolAdd - End ///");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_VolSub:
    {
        LOGD("/// VolSub - Start ///");
        rtos_key_fun_volume_sub() ;
        LOGD("/// VolSub - End ///");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_PauseResume:
    {
        LOGD("///PauseResume - Start /// ");
        rtos_key_fun_pause_resume() ;
        LOGD("/// PauseResume - End /// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_MenuChange:
    {
        LOGD("//// MenuChange - Start//// ");
        rtos_key_fun_menu_change() ;
        LOGD("//// MenuChange - End //// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_GetCode:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_GetCode - Start//// ");
        rtos_key_fun_get_code() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_GetCode - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_WechatPlay:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_WechatPlay - Start//// ");
        rtos_key_fun_wechat_play() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_WechatPlay - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_SetWifi:
    {
        LOGD("//// SetWifi - Start //// ");
        rtos_key_fun_set_wifi() ;
        LOGD("//// SetWifi - End //// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_Vad:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_Vad - Start//// ");
        rtos_key_fun_vad_start() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_Vad - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_ZtoE:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_ZtoE - Start//// ");
        rtos_key_fun_ZtoE_start() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_ZtoE - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_EtoZ:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_EtoZ - Start//// ");
        rtos_key_fun_EtoZ_start() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_EtoZ - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_AIStart:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_AIStart - Start//// ");
        rtos_key_fun_AI_start() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_AIStart - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_AIEnd:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_AIEnd - Start//// ");
        rtos_key_fun_AI_end() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_AIEnd - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_WechatStart:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_WechatStart - Start//// ");
        rtos_key_fun_wechat_start() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_WechatStart - End//// ");
    }
    break ;
    case SystemStat_Ctrl_KeyCtrl_WechatEnd:
    {
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_WechatEnd - Start//// ");
        rtos_key_fun_wechat_end() ;
        // LOGD("////GptSystemStat_Ctrl_KeyCtrl_WechatEnd - End//// ");
    }
    break ;
    case SystemStat_Ctrl_ASR_Wakeup:
    {
        LOGI("//// asr_wake up - Start //// ");
        rtos_asr_wake_up_fun(asrCmdId);
        LOGI("//// asr_wake up - End //// ");
    }
    break;

    default:
    {
        // no this cmd
    } break ;
    }

    return ;
}

/*
static void RtSystemStat_Ctrl_PlayCtrl_Fun(uint32_t data)
{
    if(data == 0){
        //  Resume play
        LOGD("Resume play ");
        rtos_play_pause_fun_resume() ;

    }else if(data == 1){
        //  Pause play
        LOGD("Pause play ");
        rtos_play_pause_fun_pause() ;

    }else if (data == 2){
        //  Previous
        LOGD("Previous audio ");
        rtos_key_fun_music_prev() ;

    }else if(data == 3){
        //  Next
        LOGD("Next audio ");
        rtos_key_fun_music_next() ;

    }else if(data == 4){
        //  Key ctorl to pause or resume
        LOGD("Key contrl pause/resume ");
        rtos_key_fun_pause_resume() ;

    }else{

    }

    return ;
}
*/

static void RtSystemStat_Ctrl_LED_Fun(uint32_t data)
{
    static bool led_flag = true ;
    // rtos_play_pause_fun_pause() ;
    if (led_flag == false)
    {
        led_flag = true ;
    }
    else
    {
        led_flag = false ;
    }
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    return ;
}

static void RtSystemStat_Ctrl_WechatPlay_Fun(uint32_t data)
{
    if (RtosSystemStat_Fun.Wechat_Data_Num != 0)
    {
        Rtos_Wechat_Data_Def  wechat_tmp ;
        RtosSystemStat_Fun.WechatDataGet(&wechat_tmp) ;
        // LOGD("Wechat-url:%s\nWechat-token:%s ", wechat_tmp.Url , wechat_tmp.Token);
        rtos_player_feed(wechat_tmp.Url, wechat_tmp.Token, TYPE_INTERCOM);
    }

    return ;
}

static void RtSystemStat_Ctrl_Net_Fun(uint32_t data)
{
    if (data == 0)
    {
        LOGD("=========Net is disconnect tone play========== ");

        Rtos_Flash_Fun.gptTonePlay(TONE_NET_CUTOFF) ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    }
    else if (data == 1)
    {
        LOGD("=========Net is connected tone play========== ");
        Rtos_Flash_Fun.gptTonePlay(TONE_NET_CONNECTED) ;

        if (wlan_is_connected() != RT_TRUE)
        {
        }
        else
        {
            g_net_stat = true ;
        }
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    }

    return ;
}

static void RtSystemStat_Ctrl_PlayHttp_Fun(uint32_t data)
{
    // strcpy(RtosSystemStat_Fun.UrlInfo.Url , url) ;
    // strcpy(RtosSystemStat_Fun.UrlInfo.Token , token) ;
    rtos_player_feed(RtosSystemStat_Fun.UrlInfo.Url, RtosSystemStat_Fun.UrlInfo.Token, TYPE_MEDIA) ;
}

/**
 * battery level event process
 * @param data [description]
 */
static void RtSystemStat_Ctrl_Bat_Fun(uint32_t data)
{
    switch (data)
    {
    case BAT_MSG_CHARGE_IN:
    {
        //  DC push in
//            HAL_WDG_Reboot();
    } break ;
    case BAT_MSG_CHARGE_OUT:
    {
        //  DC push out

    } break ;
    case BAT_MSG_CHARGE_FULL:
    {
        //  Chagne full

    } break ;
    case BAT_MSG_LOW_POWER:
    {
        //  Low battery level
        if (RtosSystemStat_Fun.SystemStat == SystemStat_Ctrl_Stat_SetWifi || \
                SystemStat_Ctrl_Stat_OTA == RtosSystemStat_Fun.SystemStat || \
                SystemStat_Ctrl_Stat_Test == RtosSystemStat_Fun.SystemStat)
        {

            break ;
        }
        // LOGD(" ===== low power  ===== ");
        // Rtos_Flash_Fun.gptTonePlay(NOPOWER_AND_CHARGE) ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    }
    break ;
    case BAT_MSG_LOW_POWER_DOWN:
    {
        //  Low power to off
        // LOGD(" ===== power off becouse low power  ===== ");
        RtosSystemStat_Fun.SystemStat = SystemStat_Ctrl_Stat_PowerOff ;
        // Rtos_Flash_Fun.gptTonePlay(NO_POWER_AND_SHUTDOWN) ;
        RtSystemStat_PowerOff() ;
    }
    break ;
    case BAT_MSG_UPDATE_BAT:
    {
        //  update battery level

    } break ;
    default:

        break ;
    }

    return ;
}


static void RtSystemStat_Ctrl_Sleep_Fun(uint32_t data)
{
    // playerCurrentTypeStopProcessor(&rtosPlayerData);
//    OS_MSleep(200) ;
    rt_thread_delay(200);

    RtosSystemStat_Fun.SystemStat = SystemStat_Ctrl_Stat_PowerOff ;
    //LOGD(" ===== Auto Power off ===== ");
    // Rtos_Flash_Fun.gptTonePlay(NEED_BREAK) ;
    // OS_Sleep(1) ;
    RtSystemStat_PowerOff() ;
    return ;
}

static void RtSystemStat_Ctrl_LoopAI_Fun(uint32_t data)
{
    if (RtosSystemStat_Fun.GetBindCode)
    {
        RtosSystemStat_Fun.GetBindCode = false ;
        return ;
    }

    if (data == SystemStat_Ctrl_LoopAI_Ctrl_True)
    {
        RtosSystemStat_Fun.LoopAI = true ;
    }
    else
    {
        RtosSystemStat_Fun.LoopAI = false ;
    }

    return ;
}

static void RtSystemStat_PowerOff(void)
{
    bat_charge_task_deinit() ;

#ifdef RT_USING_KEYCTRL
    key_ctrl_deinit();
#endif
    wlan_sta_disconnect() ;
    wlan_sta_disable() ;

    LOGD("---- Power Off ---- ");
    rt_thread_delay(500);
    //power off ...

    return ;
}

static SystemStat_Ctrl_Stat_Cmd rtos_system_stat_get(void)
{
    return RtosSystemStat_Fun.SystemStat ;
}


//******************************************************************************
static int RtSystemStat_Ctrl_Stat_Fun(uint32_t data)
{
    if (RtosSystemStat_Fun.SystemStat == SystemStat_Ctrl_Stat_Test)
    {
        return 0 ;
    }
    if (RtosSystemStat_Fun.SystemStat == SystemStat_Ctrl_Stat_PowerOff)
    {
        return 0 ;
    }

    if (data == SystemStat_Ctrl_Stat_Standby || data == SystemStat_Ctrl_Stat_Pause)
    {
        rtos_Sleep_Start() ;
    }
    else
    {
        rt_timer_stop(RtosSystemStat_Fun.Rtos_Sleep_Timer);
        RtosSystemStat_Fun.Sleep_Count = 0 ;
    }
    RtosSystemStat_Fun.SystemStat = data ;
    return 0 ;
}

static int RtSystemStat_Ctrl_Stat_Test_Fun(void)
{
    RtosTestModeFun.TestMode = true ;
    rtos_player_set_volume(99);
    return 0 ;
}

//******************************************************************************
static void rtos_system_ctrl(uint32_t event, uint32_t data, void *arg)
{
    LOGD("%s()", __func__);
    rtos_system_stat_msg_def *tmp = rt_malloc(sizeof(rtos_system_stat_msg_def)) ;
    //LOGD("***************[%s]:%d malloc:%d*************** " ,__func__ , __LINE__ , sizeof(rtos_system_stat_msg_def)) ;
    tmp->cmd = EVENT_SUBTYPE(event) ;
    tmp->data = data ;
    //LOGD("%s(): cmd = %d data = %d ",__func__, tmp->cmd,tmp->data);

    int ret = -1 ;
    ret = rtos_queue_add(rtos_system_stat_queue, (uint32_t)tmp) ;
    if (ret == -1)
    {
        LOGE("add to gpt queue Error ") ;
        rt_free(tmp) ;
    }

    rt_sem_release(RtosSystemStat_Fun.Semaphore);
    // if(OS_SemaphoreWait(&RtosSystemStat_Fun.Semaphore , 100) == OS_OK){
    // RtosSystemStat_Fun.MsgCmd = EVENT_SUBTYPE(event) ;
    // RtosSystemStat_Fun.MsgData = data ;
    // }
    return ;
}

static int rtos_system_send(RtosSystemStat_Ctrl sub, uint32_t data)
{
    if (rtos_tone_flag && sub == SystemStat_Ctrl_KeyCtrl)
    {
        // rtos_system_stat_msg_def* tmp = (rtos_system_stat_msg_def*)data ;
        // LOGD("==========tmp->data:%d.......... " , data);
        if (data == SystemStat_Ctrl_KeyCtrl_WechatEnd)
        {

        }
        else
        {
            // LOGD("===== Forbid Key press when play tone.=====");
            return 0;
        }
    }

    LOGD("==== sub = %d, data = %d ====", sub, data);
    return sys_event_send(CTRL_MSG_TYPE_SYSTEM, sub, data, 5) ;
}

void SleepTimerCallback(void *data)
{
    if (RtosSystemStat_Fun.SystemStat == SystemStat_Ctrl_Stat_Standby)
    {
#if 0
        extern int bat_state_charge();
        if (bat_state_charge())  RtosSystemStat_Fun.Sleep_Count = 0;
        else RtosSystemStat_Fun.Sleep_Count++ ;

        LOGD("-----------seelp %d-------- ", RtosSystemStat_Fun.Sleep_Count);
#else   // close for temporary
        return ;
#endif
    }
    else
    {
        RtosSystemStat_Fun.Sleep_Count = 0 ;
    }

    if (RtosSystemStat_Fun.Sleep_Count >= RTOS_SLEEP_COUNT)
    {
        rt_timer_stop(RtosSystemStat_Fun.Rtos_Sleep_Timer);

        RtosSystemStat_Fun.Sleep_Count = 0 ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Sleep, 0) ;
    }

}
void rtos_Sleep_Start(void)
{
    if (RtosSystemStat_Fun.Rtos_Sleep_Timer == RT_NULL)
    {
        RtosSystemStat_Fun.Rtos_Sleep_Timer = rt_timer_create("GPTSleep",
                                              SleepTimerCallback,
                                              RT_NULL,
                                              1000, RT_TIMER_CTRL_SET_PERIODIC);
        if (RtosSystemStat_Fun.Rtos_Sleep_Timer)
            rt_timer_start(RtosSystemStat_Fun.Rtos_Sleep_Timer);
        RtosSystemStat_Fun.Sleep_Count = 0 ;
    }
    else
    {
        rt_timer_start(RtosSystemStat_Fun.Rtos_Sleep_Timer) ;
        RtosSystemStat_Fun.Sleep_Count = 0 ;
    }

    return ;
}

static void rtos_system_set_url(char *url, char *token)
{
    memset(RtosSystemStat_Fun.UrlInfo.Url, 0,  RTOS_URL_LEN_DEF);
    memset(RtosSystemStat_Fun.UrlInfo.Token, 0, RTOS_TOKEN_LEN_DEF);
    RtosSystemStat_Fun.UrlInfo.Flag = false ;

    strcpy(RtosSystemStat_Fun.UrlInfo.Url, url) ;
    strcpy(RtosSystemStat_Fun.UrlInfo.Token, token) ;
    RtosSystemStat_Fun.UrlInfo.Flag = true ;

    return ;
}

static char *rtos_system_get_url(char *url, char *token)
{
    if (RtosSystemStat_Fun.UrlInfo.Flag == false)
    {
        return NULL;
    }

    strcpy(url, RtosSystemStat_Fun.UrlInfo.Url) ;
    strcpy(token, RtosSystemStat_Fun.UrlInfo.Token) ;
    RtosSystemStat_Fun.UrlInfo.Flag = false ;

    return url;
}

static bool rtos_wechat_messag_get(Rtos_Wechat_Data_Def *data) ;
static Rtos_Wechat_Data_Def *rtos_wechat_messag_add(char *url, char *token) ;
void RtosSystemStatInit(void)
{
    RtosTestModeFun.TestMode = false ;
    RtosTestModeFun.WifiStat = 0 ;
    RtosTestModeFun.TestFlag = false ;
    RtosTestModeFun.OTA = false ;
    RtosTestModeFun.OTAFlag = false ;

    RtosSystemStat_Fun.PlayType = TYPE_REUSME ;
    RtosSystemStat_Fun.static_rtos_wifi_set_stat = false ;
    RtosSystemStat_Fun.GetBindCode = false ;
    RtosSystemStat_Fun.Run = true ;
    RtosSystemStat_Fun.LoopAI = false ;
    RtosSystemStat_Fun.LoopAI_Type = 0 ;
    RtosSystemStat_Fun.VAD_Flag = false ;
    RtosSystemStat_Fun.Send = rtos_system_send ;
    RtosSystemStat_Fun.GetSystemStat = rtos_system_stat_get ;
    RtosSystemStat_Fun.SystemStat = SystemStat_Ctrl_Stat_Init ;
    RtosSystemStat_Fun.SetHttpUrl = rtos_system_set_url ;
    RtosSystemStat_Fun.GetHttpUrl = rtos_system_get_url ;
    RtosSystemStat_Fun.Wechat_Data_Num = 0 ;
    RtosSystemStat_Fun.WechatDataStart = NULL ;
    RtosSystemStat_Fun.WechatDataEnd = NULL ;
    RtosSystemStat_Fun.WechatDataGet = rtos_wechat_messag_get ;
    RtosSystemStat_Fun.WechatDataAdd = rtos_wechat_messag_add ;
    RtosSystemStat_Fun.MenuFlag = false ;
    RtosSystemStat_Fun.Battery = 0 ;
    RtosSystemStat_Fun.KeyPostFlag = false ;
    RtosSystemStat_Fun.MQTTFlag = false ;
    RtosSystemStat_Fun.NetConnecting = false ;

    memset(RtosSystemStat_Fun.UrlInfo.Url, 0,  RTOS_URL_LEN_DEF) ;
    memset(RtosSystemStat_Fun.UrlInfo.Token, 0, RTOS_TOKEN_LEN_DEF) ;
    RtosSystemStat_Fun.UrlInfo.Flag = false ;

    rtos_get_chipid() ;
    rtos_Sleep_Start() ;
    rtos_system_stat_queue = rtos_queue_init(RTOS_SYSTEM_STAT_QUEUE_NUM) ;

    RtosSystemStat_Fun.Thread = rt_thread_create("rtos_system_stat",
                                rtos_system_stat_Task,
                                RT_NULL,
                                RTOS_SYSTEM_STACK_SIZE,
                                RT_THREAD_PRIORITY_MAX / 2, 10);
    if (!RtosSystemStat_Fun.Thread)
        return;
    else
        rt_thread_startup(RtosSystemStat_Fun.Thread);

    LOGD(" ++++++ rtos_system_stat start run ++++++  ");
    RtosSystemStat_Fun.Semaphore = rt_sem_create("rtos_system_stat_sem", 0, RT_IPC_FLAG_FIFO);

    if ((observer_base *)RtosSystemStat_Fun.Obs == NULL)
    {
        RtosSystemStat_Fun.Obs = sys_callback_observer_create(CTRL_MSG_TYPE_SYSTEM,
                                 CTRL_MSG_SUB_TYPE_ALL,
                                 rtos_system_ctrl,
                                 NULL);
    }
    sys_ctrl_attach((observer_base *)RtosSystemStat_Fun.Obs);

    return ;
}

void RtSystemStatDeInit(void)
{
    RtosSystemStat_Fun.Run = false ;
    while (RtosSystemStat_Fun.Thread)
        rt_thread_delay(10);
    return ;
}


static bool rtos_wechat_messag_get(Rtos_Wechat_Data_Def *data)
{
    if (RtosSystemStat_Fun.Wechat_Data_Num == 0)
    {
        return false ;
    }

    memset(data, 0, sizeof(Rtos_Wechat_Data_Def)) ;
    strcpy(data->Url, RtosSystemStat_Fun.WechatDataStart->Url) ;
    strcpy(data->Token, RtosSystemStat_Fun.WechatDataStart->Token) ;

    Rtos_Wechat_Data_Def *tmp = RtosSystemStat_Fun.WechatDataStart ;
    RtosSystemStat_Fun.WechatDataStart = tmp->Next ;
    tmp->Next = NULL ;
    rt_free(tmp) ;
    RtosSystemStat_Fun.Wechat_Data_Num-- ;
    if (RtosSystemStat_Fun.Wechat_Data_Num == 0)
    {
        RtosSystemStat_Fun.WechatDataStart = NULL ;
        RtosSystemStat_Fun.WechatDataEnd = NULL ;
    }
    return true ;
}


static Rtos_Wechat_Data_Def *rtos_wechat_messag_add(char *url, char *token)
{
    if (strlen(url) != 0 && strlen(token) != 0)
    {

    }
    else
    {
        return NULL ;
    }
    if (RtosSystemStat_Fun.Wechat_Data_Num >= 10)
    {
        return NULL ;
    }

    Rtos_Wechat_Data_Def *tmp = rt_malloc(sizeof(Rtos_Wechat_Data_Def)) ;
    // LOGD("***************[%s]:%d malloc:%d*************** " ,__func__ , __LINE__ , sizeof(Rtos_Wechat_Data_Def)) ;
    memset(tmp, 0, sizeof(Rtos_Wechat_Data_Def)) ;
    strcpy(tmp->Url, url) ;
    strcpy(tmp->Token, token) ;

    if (RtosSystemStat_Fun.WechatDataStart == NULL)
    {
        RtosSystemStat_Fun.WechatDataStart = tmp ;
        RtosSystemStat_Fun.WechatDataEnd = tmp ;
    }
    else
    {
        RtosSystemStat_Fun.WechatDataEnd->Next = tmp ;
        RtosSystemStat_Fun.WechatDataEnd = tmp ;
    }
    RtosSystemStat_Fun.Wechat_Data_Num++ ;
    return tmp ;
}

// static char get_char(uint8_t data)
// {
//     uint8_t dd = data & 0xf;
//     if(dd < 10 ) return ('0' + dd);
//     else return ('a'+(dd-10));
// }


static int rtos_get_chipid(void)
{
    //uint8_t chipid[16] ={0};
    memset(RtosSystemStat_Fun.ChipID, 0, 40);
    /* TODO: add process .......*/
    // read from efuse
    //
    rt_memcpy(RtosSystemStat_Fun.ChipID, "Rockchip201907010000", 20); //chip SN nuber.
    LOGD(" ========ChipID:%s======== ", RtosSystemStat_Fun.ChipID);

    return 0;
}

