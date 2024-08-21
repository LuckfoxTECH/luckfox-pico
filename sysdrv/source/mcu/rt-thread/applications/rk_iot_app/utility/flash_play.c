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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "flash_play.h"
#include "globe_player.h"
#include "hal_base.h"

#define DBG_LEVEL               DBG_LOG
#define DGB_SECTION_NAME        "FLASH_PLAY"
#include "rkdebug.h"

Rtos_Flash_Fun_Def Rtos_Flash_Fun ;

static Rtos_Menu_Type_Def rtos_flash_menu(Rtos_Menu_Type_Def val) ;
static int rtos_flash_tone_play(uint8_t index) ;
static int rtos_flash_get_tone_path(uint8_t index, char *path) ;
static int rtos_flash_get_music_path(uint8_t index, char *path) ;
static int rtos_flash_music_next_prev(Rtos_Play_Mode_Def val) ;

Tone_Name g_toneName[TONE_NUMBER_MAX] =
{
    {TONE_WELCOME,  "welcome.mp3",  ""},
    {TONE_ANSWER1,  "iamhere.mp3",  ""},
    {TONE_ANSWER2,  "here.mp3",     ""},
    {TONE_ANSWER3,  "tellme.mp3",   ""},
    {TONE_ANSWER4,  "pls speak.mp3", ""},
#if defined(PRJCONF_AI_SPEECH)
    {TONE_TURN_ON_HEATER,           "turnonHeater.mp3",     "turn on heater"},
    {TONE_TURN_OFF_HEATER,          "turnoffHeater.mp3",    "turn off heater"},
    {TONE_RISE_TEMP,                "higher.mp3",           "rise temperature"},
    {TONE_LOWER_TEMP,               "lower.mp3",            "lower temperature"},
    {TONE_45_DEGREE,                "45degree.mp3",         "set to 45 degree"},
    {TONE_65_DEGREE,                "65degree.mp3",         "set to 65 degree"},
    {TONE_75_DEGREE,                "75degree.mp3",         "set to 75 degree"},
    {TONE_TURN_ON_ANTIBACTERIAL,    "turnon-Anti.mp3",      "turn on anti-bacterial"},
    {TONE_TURN_OFF_ANTIBACTERIAL,   "turnoff-Anti.mp3",     "turn off anti-bacterial"},
    {TONE_TURN_ON_MORNING_BATH,     "turnon-Morningbath.mp3",   "turn on morning bath"},
    {TONE_TURN_OFF_MORNING_BATH,    "turnoff-Morningbath.mp3",  "turn off morning bath"},
    {TONE_TURN_ON_NIGHT_BATH,       "turnon-Nightbath.mp3",     "turn on night bath"},
    {TONE_TURN_OFF_NIGHT_BATH,      "turnoff-Nightbath.mp3",    "turn off night bath"},
    {TONE_BATH,                     "bath.mp3",                 "i would like to have a bath"},
    {TONE_TURN_ON_RAPID_HEATING,    "turnon-Rapidheating.mp3",  "turn on rapid heating and add water"},
    {TONE_TURN_OFF_RAPID_HEATING,   "turnoff-Rapidheating.mp3", "turn off rapid heating and add water"},
#elif defined(PRJCONF_AI_CUSTOM)
    {TONE_TURN_ON_AIR_CONDITIONING,     "ktydk.mp3",    "turn on air conditioning"},
    {TONE_TURN_ON,                      "ykj.mp3",      "turn on"},
    {TONE_TURN_OFF,                     "ygj.mp3",      "turn off"},
    {TONE_COOLING_MODE,                 "zlms.mp3",     "cooling mode"},
    {TONE_DEHUMIDIFICATION_MODE,        "csms.mp3",     "dehumidification mode"},
    {TONE_AIR_SUPPLY_MODE,              "sfms.mp3",     "air_supply mode"},
    {TONE_HEATING_MODE,                 "jrms.mp3",     "heating mode"},
    {TONE_RISE_TEMP,                    "sgwd.mp3",     "rise temp"},
    {TONE_LOWER_TEMP,                   "jdwd.mp3",     "lower temp"},
    {TONE_INCREASE_WIND_SPEED,          "zdfs.mp3",     "increase wind speed"},
    {TONE_REDUCE_WIND_SPEED,            "jxfs.mp3",     "reduce wind speed"},
    {TONE_TURN_ON_SWEEP_LEFT_AND_RIGHT, "zysfydk.mp3",  "turn on sweep left and right"},
    {TONE_TURN_ON_SWEEP_UP_AND_DOWN,    "sxsfydk.mp3",  "turn on sweep up and down"},
    {TONE_TURN_OFF_SWEEP_LEFT_AND_RIGHT, "zysfygb.mp3", "turn off sweep left and right"},
    {TONE_TURN_OFF_SWEEP_UP_AND_DOWN,   "sxsfygb.mp3",  "turn off sweep up and down"},
#endif
    {TONE_NET_NOT_CONNETCTED,   "net_not_connected.mp3",    ""},
    {TONE_SETUP_NET,            "setup net.mp3",    ""},
    {TONE_SETUP_NET_CODE,       "set net.mp3",      ""},
    {TONE_NET_CONNECTED,        "net connected.mp3", ""},
    {TONE_NET_CUTOFF,           "net cutoff.mp3",   ""},
    {TONE_NET_CONFIG_FAIL,      "net config fail.mp3", ""},
    {TONE_NET_MATCHING,         "net matching.mp3", ""},
    {TONE_QUIT_WIFI_SET,        "net set stop.mp3", ""},
    {TONE_RCV_NET_INFO,         "rcv net info.mp3", ""},

    {TONE_SONG,     "erge.mp3", ""},
    {TONE_STORY,    "story.mp3", ""},
    {TONE_SINOLOGY, "guoxue.mp3", ""},
    {TONE_ENGLISH,  "english.mp3", ""},
    {TONE_LOVE,     "lovest.mp3", ""},
};

#ifdef RT_USING_IOT_DISPLAY
const char *asr_ask_respond_str[] =
{
    RT_NULL,
    "我在",
    "在的",
    "请吩咐",
    "您请说",
};

const char *asr_cmd_respond_str[] =
{
    " ",
#ifdef PRJCONF_AI_SPEECH
    "已为您打开热水器",
    "已为您关闭热水器",
    "已为您调高温度",
    "已为您调低温度",
    "已为您调至45°",
    "已为您调至65°",
    "已为您调至75°",
    "已为您打开抑菌",
    "已为您关闭抑菌",
    "已为您打开晨浴",
    "已为您关闭晨浴",
    "已为您打开晚浴",
    "已为您关闭晚浴",
    "好的,已为您打开热水器",
    "已为您打开速热补水",
    "已为您关闭速热补水",
#elif defined(PRJCONF_AI_CUSTOM)
    "空调已打开",
    "已开机",
    "已关机",
    "制冷模式",
    "除湿模式",
    "送风模式",
    "加热模式",
    "升高温度",
    "降低温度",
    "增大风速",
    "减小风速",
    "左右扫风已打开",
    "上下扫风已打开",
    "左右扫风已关闭",
    "上下扫风已关闭",
#endif
    RT_NULL,
};
#endif

static int rtos_flash_music_next_prev(Rtos_Play_Mode_Def val)
{
    char tmp_buf[100] = {0} ;

    LOGD("%s enter ", __func__);
    if (val == Rtos_Play_Mode_Prev)
    {
        if (Rtos_Flash_Fun.MenuMusicIndex == 1 || Rtos_Flash_Fun.MenuMusicIndex == 0)
        {
            Rtos_Flash_Fun.MenuMusicIndex = Rtos_Flash_Fun.MusicTotal ;
        }
        else
        {
            Rtos_Flash_Fun.MenuMusicIndex-- ;
        }
        LOGD("%s enter :  //// PREV //// Ind = %d", __func__, Rtos_Flash_Fun.MenuMusicIndex);
    }
    else if (val == Rtos_Play_Mode_Current)
    {
        LOGD("%s enter :  ///// Current ////", __func__);
    }
    else
    {
        if (Rtos_Flash_Fun.MenuMusicIndex >= Rtos_Flash_Fun.MusicTotal)
        {
            Rtos_Flash_Fun.MenuMusicIndex = 1 ;
        }
        else
        {
            Rtos_Flash_Fun.MenuMusicIndex++ ;
        }

        LOGD("%s enter :  //// NEXT //// Ind = %d", __func__, Rtos_Flash_Fun.MenuMusicIndex);
    }
    rtos_flash_get_music_path(Rtos_Flash_Fun.MenuMusicIndex, tmp_buf) ;
    rtos_player_feed(tmp_buf, NULL, TYPE_MEDIA_NORFLASH) ;
    return val ;
}

static int rtos_flash_menu_music(Rtos_Menu_Type_Def val)
{
    char tmp_buf[100] = {0} ;
    rtos_flash_get_music_path(val * 2 + 1, tmp_buf) ;
    Rtos_Flash_Fun.MenuMusicIndex = val * 2 + 1;
    rtos_player_feed(tmp_buf, NULL, TYPE_MEDIA_NORFLASH) ;
    return 0 ;
}

/**
 * change current music dirctory .
 * @param  val  if small than Rtos_Menu_Type_NULL, set current dir;
                    biger than Rtos_Menu_Type_NULL, current dir incress.
 * @return     return current dir.
 */
static Rtos_Menu_Type_Def rtos_flash_menu(Rtos_Menu_Type_Def val)
{
    if (val >= Rtos_Menu_Type_NULL)
    {
        if (Rtos_Flash_Fun.MenuMusicIndex == 1 || Rtos_Flash_Fun.MenuMusicIndex == 2)
        {
            Rtos_Flash_Fun.MenuType = Rtos_Menu_Type_Story ;
        }
        else if (Rtos_Flash_Fun.MenuMusicIndex == 3 || Rtos_Flash_Fun.MenuMusicIndex == 4)
        {
            Rtos_Flash_Fun.MenuType = Rtos_Menu_Type_NationalStudies ;
        }
        else if (Rtos_Flash_Fun.MenuMusicIndex == 5 || Rtos_Flash_Fun.MenuMusicIndex == 6)
        {
            Rtos_Flash_Fun.MenuType = Rtos_Menu_Type_English ;
        }
        else if (Rtos_Flash_Fun.MenuMusicIndex == 7 || Rtos_Flash_Fun.MenuMusicIndex == 8)
        {
            Rtos_Flash_Fun.MenuType = Rtos_Menu_Type_ChildrenSong ;
        }
        // Rtos_Flash_Fun.MenuType++ ;
    }
    else
    {
        Rtos_Flash_Fun.MenuType = val ;
    }

    if (Rtos_Flash_Fun.MenuType >= Rtos_Menu_Type_NULL)
    {
        Rtos_Flash_Fun.MenuType = Rtos_Menu_Type_ChildrenSong ;
    }

#ifdef PRJCONF_AI_RK
    rtos_flash_tone_play(TONE_SONG + Rtos_Flash_Fun.MenuType) ;
#endif
    rtos_flash_menu_music(Rtos_Flash_Fun.MenuType) ;

    return Rtos_Flash_Fun.MenuType ;
}
/**
 * play tone file in flash.
 * @param  index :the tone index number.
 * @return       NULL
 */
static int rtos_flash_tone_play(uint8_t index)
{
    char tmp_buf[100] = {0};
    Rtos_Flash_Fun.getTonePath(index, tmp_buf);
#ifndef RT_USING_IOT_DISPLAY
    rtos_player_tone_feed(tmp_buf, NULL, TYPE_TONE);
#else
    rtos_tone_flag = false;
    return 0;
#endif

    int i = 0 ;
    while (rtos_tone_flag)
    {
        rt_thread_delay(10) ;
        i++ ;
        if (i >= 500)
        {
            rtos_tone_flag = false ;
            break ;
        }
    }
    return 0 ;
}

/**
 * get tone file path in flash.
 * @param  path return tone file path
 * @return      return path of tone file path, if -1 means get fail.
 */
static int rtos_flash_get_tone_path(uint8_t index, char *path)
{
    if (Rtos_Flash_Fun.ToneTotal == 0)
    {
        return -1;
    }

    if (index >= TONE_NUMBER_MAX)
    {
        return -1;
    }

    sprintf(path, "/notice/%s", g_toneName[index].tone_name);
    //sprintf(path, "/notice/test_out.wav");

    LOGW("Tone path:%s", path);
    return strlen(path);
}

/**
 * get musci path
 * @param  path :return path ot tone
 * @return      return length of path of tone. fail return -1.
 */
static int rtos_flash_get_music_path(uint8_t index, char *path)
{
    if (Rtos_Flash_Fun.MusicTotal == 0)
    {
        LOGD("MusicTotal 0.....\n");
        return -1 ;
    }
    if (index == 0)
    {
        LOGD("Music ID 0.....\n");
        return -1 ;
    }
    else if (index > Rtos_Flash_Fun.MusicTotal)
    {
        LOGD("Music ID 0.....\n");
        return -1 ;
    }
    uint8_t id = index;
    sprintf(path, "/music/%d.mp3", id - 1) ;
    LOGD("Music path:%s\n", path);
    return strlen(path) ;
}

/**
 * rtos flash function init
 */
void rtos_flash_fun_init(void)
{
    Rtos_Flash_Fun.ToneTotal = TONE_NUMBER_MAX;
    Rtos_Flash_Fun.MusicTotal = 0 ;
    Rtos_Flash_Fun.getTonePath = rtos_flash_get_tone_path ;
    Rtos_Flash_Fun.getMusicPath = rtos_flash_get_music_path ;
    Rtos_Flash_Fun.gptTonePlay = rtos_flash_tone_play ;
    Rtos_Flash_Fun.gptMenu = rtos_flash_menu ;
    Rtos_Flash_Fun.gptPlayNextPrev = rtos_flash_music_next_prev ;
    Rtos_Flash_Fun.MenuType = Rtos_Menu_Type_ChildrenSong ;
    Rtos_Flash_Fun.MenuMusicIndex = 1 ;

    LOGD("%s Enter", __func__);
    Rtos_Flash_Fun.MusicTotal = ScanFiles("/music");
    LOGD("%s music_total: %ld", __func__, Rtos_Flash_Fun.MusicTotal);
    return ;
}
