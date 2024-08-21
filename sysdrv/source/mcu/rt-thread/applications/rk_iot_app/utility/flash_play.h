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

#ifndef _FLASH_PLAY_H_
#define _FLASH_PLAY_H_

#include <rtthread.h>
#include <stdio.h>

enum
{
    TONE_WELCOME,   // 欢迎使用智能语音助手
    TONE_ANSWER1,   // 我在
    TONE_ANSWER2,   // 在的
    TONE_ANSWER3,   // 请吩咐
    TONE_ANSWER4,   // 您请说
#if defined(PRJCONF_AI_SPEECH)
    TONE_TURN_ON_HEATER,                // 已为您打开热水器
    TONE_TURN_OFF_HEATER,               // 已为您关闭热水器
    TONE_RISE_TEMP,                     // 已为您调高温度
    TONE_LOWER_TEMP,                    // 已为您调低温度
    TONE_45_DEGREE,                     // 已为您调至45°
    TONE_65_DEGREE,                     // 已为您调至65°
    TONE_75_DEGREE,                     // 已为您调至75°
    TONE_TURN_ON_ANTIBACTERIAL,         // 已为您打开抑菌
    TONE_TURN_OFF_ANTIBACTERIAL,        // 已为您关闭抑菌
    TONE_TURN_ON_MORNING_BATH,          // 已为您打开晨浴
    TONE_TURN_OFF_MORNING_BATH,         // 已为您关闭晨浴
    TONE_TURN_ON_NIGHT_BATH,            // 已为您打开晚浴
    TONE_TURN_OFF_NIGHT_BATH,           // 已为您关闭晚浴
    TONE_BATH,                          // 好的,已为您打开热水器
    TONE_TURN_ON_RAPID_HEATING,         // 已为您打开速热补水
    TONE_TURN_OFF_RAPID_HEATING,        // 已为您关闭速热补水
#elif defined(PRJCONF_AI_CUSTOM)
    TONE_TURN_ON_AIR_CONDITIONING,      // 空调已打开
    TONE_TURN_ON,                       // 已开机
    TONE_TURN_OFF,                      // 已关机
    TONE_COOLING_MODE,                  // 制冷模式
    TONE_DEHUMIDIFICATION_MODE,         // 除湿模式
    TONE_AIR_SUPPLY_MODE,               // 送风模式
    TONE_HEATING_MODE,                  // 加热模式
    TONE_RISE_TEMP,                     // 升高温度
    TONE_LOWER_TEMP,                    // 降低温度
    TONE_INCREASE_WIND_SPEED,           // 增大风速
    TONE_REDUCE_WIND_SPEED,             // 减小风速
    TONE_TURN_ON_SWEEP_LEFT_AND_RIGHT,  // 左右扫风已打开
    TONE_TURN_ON_SWEEP_UP_AND_DOWN,     // 上下扫风已打开
    TONE_TURN_OFF_SWEEP_LEFT_AND_RIGHT, // 左右扫风已关闭
    TONE_TURN_OFF_SWEEP_UP_AND_DOWN,    // 上下扫风已关闭
#endif
    TONE_NET_NOT_CONNETCTED,            // 未连接网络
    TONE_SETUP_NET,                     // 请帮我设置网络吧
    TONE_SETUP_NET_CODE,                // 请扫描二维码开始给我配网吧
    TONE_NET_CONNECTED,                 // 联网已成功
    TONE_NET_CUTOFF,                    // 网络断开啦
    TONE_NET_CONFIG_FAIL,               // 网络配置失败
    TONE_NET_MATCHING,                  // 网络匹配中
    TONE_QUIT_WIFI_SET,                 // 停止配置网络
    TONE_RCV_NET_INFO,                  // 收到网络信息
    TONE_SONG,                          // 儿歌
    TONE_STORY,                         // 故事
    TONE_SINOLOGY,                      // 国学
    TONE_ENGLISH,                       // 英语
    TONE_LOVE,                          // 宝宝最爱
    TONE_NUMBER_MAX,
};

typedef enum _Rtos_Play_Mode
{
    Rtos_Play_Mode_Prev,
    Rtos_Play_Mode_Next,
    Rtos_Play_Mode_Current,

    Rtos_Play_Mode_NULL,

} Rtos_Play_Mode_Def;

typedef enum _Rtos_Menu_Type
{
    Rtos_Menu_Type_ChildrenSong,             //  儿歌
    Rtos_Menu_Type_Story,                    //  故事
    Rtos_Menu_Type_NationalStudies,          //  国学
    Rtos_Menu_Type_English,                  //  英语
    // Rtos_Menu_Type_Love ,                 //  最爱

    Rtos_Menu_Type_NULL,

} Rtos_Menu_Type_Def;


typedef struct _Flash_Play_Fun
{
    int ToneTotal ;                 //  提示音总数
    int MusicTotal ;                //  内置音乐总数

    int (*getTonePath)(uint8_t index, char *path);      //  获取内置提示音音频的地址
    int (*getMusicPath)(uint8_t index, char *path)  ;   //  获取内置音乐音频的地址
    int (*gptTonePlay)(uint8_t index) ;                 //  播放提示音
    int (*gptMusicPlay)(uint8_t index) ;                //  播放music
    int (*gptPlayNextPrev)(Rtos_Play_Mode_Def val) ;
    Rtos_Menu_Type_Def(*gptMenu)(Rtos_Menu_Type_Def val) ;
//    uint32_t *ToneNorFlashData ;
//    uint32_t *MusicNorFlashData ;

    // PLAYER_TYPE MusicType ;                              //  当前音频的类型 TYPE_MEDIA--服务器端音频  TYPE_MEDIA_NORFLASH--NorFlash音频  TYPE_MEDIA_SD--SD卡音频
    Rtos_Menu_Type_Def MenuType ;                        //  当前音频目录
    uint16_t MenuMusicIndex ;                           //  当前音频目录音乐编号

} Rtos_Flash_Fun_Def;

typedef struct _Tone_Name
{
    uint32_t tone_id;
    char    *tone_name;
    char    *cmd_str;
} Tone_Name;

extern Rtos_Flash_Fun_Def Rtos_Flash_Fun ;
void rtos_flash_fun_init(void) ;

#endif  //_FLASH_PLAY_H_