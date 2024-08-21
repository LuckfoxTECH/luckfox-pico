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

#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform_init.h"
#include "./include/driver/component/component_def.h"
#include "globe_system_stat.h"
#include "globe_airkiss.h"
#include "globe_player.h"

#include "rec_pub.h"
#include "batt_task.h"
#include "flash_play.h"
#include "keyctrl_task.h"

#include "sysinfo.h"
#ifdef PRJCONF_NET_EN
#include "net_ctrl.h"
#endif
#ifdef RT_USING_IOT_DISPLAY
#include "globe_display.h"
#endif
#include"rk_iot_app.h"
#include "rkdebug.h"

#ifdef RT_USING_DSP
#include "asr/asr.h"
#include "hal_base.h"
#endif

struct sysinfo e_sysinfo;
int volume = 50;
uint32_t SysSeed;
uint32_t SysYear = 1970;
uint32_t SysMonth = 1;
uint32_t SysDay = 1;
uint32_t SysWeek = 4;
uint32_t SysHour;
uint32_t SysMin;
uint32_t SysSecond;
uint32_t SysMsecond;
uint32_t sysUsecond;
uint32_t sysClk;
uint32_t sysTickSave;
uint32_t sysTickCounterSave;

#ifdef RT_USING_IOT_DISPLAY
extern rt_err_t lvgl_clock_content_update_callback(uint32_t cmd, void *parm, uint32_t parmlen);
#endif

void rk_iot_app_init(void *arg)
{
    platform_init();
    RtosSystemStatInit();
    rtos_flash_fun_init();

#ifdef RT_USING_IOT_DISPLAY
    rtos_display_callback_register(lvgl_clock_content_update_callback);
#endif

    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Init) ;
    rtos_player_task_init();

#ifdef RT_USING_KEYCTRL
    key_ctrl_init();
#endif

    if (RtosTestModeFun.TestFlag)
    {
        rt_thread_delay(2000) ;
        RtosTestModeFun.TestFlag = false ;
    }
    if (RtosTestModeFun.OTAFlag)
    {
        rt_thread_delay(2000) ;
        RtosTestModeFun.OTAFlag = false ;
    }
    // rtos_player_set_volume(volume);
    if (RtosTestModeFun.TestMode == true || RtosTestModeFun.OTA == true)
    {
    }
    else
    {
        LOGD("==========ChipID:%s========== ", RtosSystemStat_Fun.ChipID);
#ifndef RT_USING_IOT_DISPLAY
        Rtos_Flash_Fun.gptTonePlay(TONE_WELCOME) ;
#else
//        Rtos_Flash_Fun.gptPlayNextPrev(Rtos_Play_Mode_Current) ;
#endif
    }

#ifdef PRJCONF_NET_EN
    LOGD("Start Config WiFI ");
    extern int net_sys_start(enum wlan_mode mode);

    struct sysinfo *sysInfo = sysinfo_get();
    net_sys_start(sysInfo->wifi_info_num);

    rt_thread_mdelay(200);
    networkStart(&e_sysinfo);
#endif /* PRJCONF_NET_EN */

#ifdef RT_USING_DSP
    asr_start_app();
    rt_thread_mdelay(500);
    asr_load();
#endif

    /*
       mqtt_connect_wlan() ;
    */

    while (1)
    {
        rt_thread_delay(10);
    }

    rtos_player_destory();
}


void app_key(int argc, char **argv)
{
    if (argc == 1 || argc > 2)
    {
        rt_kprintf("Useage: \n");
        rt_kprintf("use \"app_key 0\" to play prevoious  song in /music\n");
        rt_kprintf("use \"app_key 1\" to play next song in /music:\n");
        rt_kprintf("use \"app_key 2\" to pause or resume play in /music:\n");
        rt_kprintf("use \"app_key 3\" to start airkiss to set wifi. /music:\n");
        return;
    }

    if (0 == strcmp(argv[1], "0"))
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_Prev) ;
    else if (0 == strcmp(argv[1], "1"))
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_Next) ;
    else if (0 == strcmp(argv[1], "2"))
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_PauseResume) ;
    else if (0 == strcmp(argv[1], "3"))
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_KeyCtrl, SystemStat_Ctrl_KeyCtrl_SetWifi) ;

}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(app_key, app key demo);
#endif

int app(void)
{
    rt_thread_startup(rt_thread_create("app", rk_iot_app_init, RT_NULL, 4096, 10, 10));
    return 0;
}

MSH_CMD_EXPORT(app, app init);
