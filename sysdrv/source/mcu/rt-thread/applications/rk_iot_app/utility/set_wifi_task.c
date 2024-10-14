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
#include <rtdef.h>

#ifdef RT_USING_WLAN_WICED
#include <wlan_dev.h>
#include <wlan_cfg.h>
#include <wlan_mgnt.h>
#include "wlan_dev.h"
#endif

#include "set_wifi_task.h"
#include "sysinfo.h"
#include "globe_airkiss.h"
#include "flash_play.h"
#include "player/globe_system_stat.h"

#define DBG_LEVEL           DBG_LOG
#define DBG_SECTION_NAME    "AIRKISS"
#include "rkdebug.h"


#define GLOBE_SMARTLINK_TIME_OUT_MS   (90 * 1000)
#define GLOBE_AK_TIME_OUT_MS          (30 * 1000)


#define GLOBE_AIRKISS_ENABLE          1
#define GLOBE_VOICE_PRINT_ENABLE      0
#define GLOBE_SYSINFO_SSID_LEN_MAX        (32)
#define GLOBE_SYSINFO_PSK_LEN_MAX         (65)

#define GLOBE_DEF_WIFI_SSID           "testwifi"
#define GLOBE_DEF_WIFI_PSK            "12345678"

static char *airkiss_key = "1234567812345678";
int rtos_wifi_save(void) ;
static rt_thread_t g_thread;

extern struct netif *g_wlan_netif;

#define THREAD_STACK_SIZE       (4 * 1024)
static uint8_t wifi_test_run = 0 ;


static struct sysinfo e_sysinfo_tmp;
static wlan_airkiss_result_t ak_result;
static void wifi_test_task(void *arg)
{
    uint32_t end_time;

    end_time = rt_tick_get() + GLOBE_SMARTLINK_TIME_OUT_MS;
    memset(&ak_result, 0, sizeof(wlan_airkiss_result_t));

    while (wifi_test_run)
    {
        rt_thread_delay(20);

        if ((end_time - rt_tick_get()) < 0)
        {
            goto out1;
        }

        if (wlan_airkiss_get_status() == AIRKISS_STATUS_COMPLETE)
        {
            LOGI(" tone play  TONE_RCV_NET_INFO");
            Rtos_Flash_Fun.gptTonePlay(TONE_RCV_NET_INFO) ;

            LOGD("Play tone TONE_RCV_NET_INFO");
            if (!wlan_airkiss_connect_ack(g_wlan_netif, GLOBE_AK_TIME_OUT_MS, &ak_result))
            {
                LOGD("ssid:%s psk:%s random:%d\n", (char *)ak_result.ssid,
                     (char *)ak_result.passphrase, ak_result.random_num);
                rtos_wifi_save() ;
                Rtos_Flash_Fun.gptTonePlay(TONE_NET_CONNECTED) ;
                g_net_stat = true ;
                LOGI("Airkiss connect ok.......\n");
                goto out;
            }
            else
            {
                goto out1;
                // Rtos_Flash_Fun.gptTonePlay(NET_CONFIG_UNSUCCESSFUL) ;
            }

            wifi_test_run = 0 ;
        }
    }
out:
    RtosSystemStat_Fun.static_rtos_wifi_set_stat = false ;
    LOGD("------------[%s] over--------------\n", __func__);
    wifi_test_run = 0 ;
    wlan_airkiss_stop();
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;

    LOGD("%s()-----------------1--------------\n", __func__);
    g_thread = NULL;
    return ;
out1:
    LOGD("%s()-----------------3--------------\n", __func__);
    RtosSystemStat_Fun.static_rtos_wifi_set_stat = false ;
    wifi_test_run = 0 ;
    wlan_airkiss_stop();

    LOGI("tone play TONE_NET_CONFIG_FAIL");
    Rtos_Flash_Fun.gptTonePlay(TONE_NET_CONFIG_FAIL) ;

// exit:
    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    memset(&e_sysinfo_tmp, 0, sizeof(e_sysinfo_tmp)) ;
    LOGD("########   Re start network ########");
    networkStart(&e_sysinfo_tmp);

    LOGD("%s()-----------------4--------------\n", __func__);
    g_thread = NULL;
}

int rtos_wifi_set_start(void)
{
    int ret = 0;
    wifi_test_run = 1 ;
    wlan_airkiss_status_t ak_status;

    if (g_thread != RT_NULL)
    {
        LOGE(" airkiss thread has exist !!! \n");
        return -1;
    }

    ak_status = wlan_airkiss_start(g_wlan_netif,  airkiss_key);
    if (ak_status != WLAN_AIRKISS_SUCCESS)
    {
        LOGD("airkiss start fiald!\n");
    }

    g_thread = rt_thread_create("wifi_test",
                                wifi_test_task,
                                RT_NULL,
                                THREAD_STACK_SIZE,
                                RT_THREAD_PRIORITY_MAX / 2,
                                10);

    if (g_thread != RT_NULL)
    {
        LOGD("create wifi test task OK");
        rt_thread_startup(g_thread);
    }

    return ret;
}

int rtos_wifi_set_stop(void)
{
    wifi_test_run = 0 ;
    while (g_thread)
    {
        rt_thread_delay(10);
    }

    return 0;
}

int rtos_wifi_save(void)
{
#ifdef RT_USING_WLAN_WICED
#ifdef PRJCONF_NET_EN
    struct sysinfo *e_sysinfo ;
    char tmp_ssid[64] = {0} ;
    char tmp_psk[64] = {0} ;

    struct rt_wlan_info info;
    if (wlan_is_connected())
    {
        rt_wlan_get_info(&info);
    }
    memcpy(tmp_ssid, info.ssid.val, info.ssid.len);
    memcpy(tmp_psk, ak_result.passphrase, WLAN_PASSPHRASE_MAX_LEN);

    e_sysinfo =  sysinfo_get();
    for (int i = 0; i < e_sysinfo->wifi_info_num; ++i)
    {
        LOGI("tmp_ssid = %s ", tmp_ssid);
        if (strcmp(tmp_ssid, (char *)e_sysinfo->wlan_sta_param[i].ssid) == 0)
        {
            memset(e_sysinfo->wlan_sta_param[i].psk, 0, SYSINFO_PSK_LEN_MAX) ;
            strcpy((char *)e_sysinfo->wlan_sta_param[i].psk, tmp_psk) ;
            e_sysinfo->wlan_sta_param[i].ssid_len = strlen((char *)e_sysinfo->wlan_sta_param[i].ssid) ;
            e_sysinfo->wifi_info_index = i ;

            LOGI(" ssid = %s, info_index = %d ", tmp_ssid, e_sysinfo->wifi_info_index);
            sysinfo_save();
            return 0 ;
        }
    }

    // Save wifi info to file
    if (e_sysinfo->wifi_info_num == 0 || e_sysinfo->wifi_info_num == 1)
    {
        LOGI("00 wifi_info_num  = %d ", e_sysinfo->wifi_info_num);
        LOGI("index 0: ssid = %s ", e_sysinfo->wlan_sta_param[0].ssid);

        memset(e_sysinfo->wlan_sta_param[1].ssid, 0, SYSINFO_SSID_LEN_MAX) ;
        memset(e_sysinfo->wlan_sta_param[1].psk, 0, SYSINFO_PSK_LEN_MAX) ;
        strcpy((char *)e_sysinfo->wlan_sta_param[1].ssid, tmp_ssid) ;
        strcpy((char *)e_sysinfo->wlan_sta_param[1].psk, tmp_psk) ;
        e_sysinfo->wlan_sta_param[1].ssid_len = strlen((char *)e_sysinfo->wlan_sta_param[1].ssid) ;
        e_sysinfo->wifi_info_index = 1 ;
        e_sysinfo->wifi_info_num = 2;

//        LOGI("after ssid = %s ", e_sysinfo->wlan_sta_param[0].ssid);
        LOGI("index 1: ssid = %s ", e_sysinfo->wlan_sta_param[1].ssid);

    }
    else if (e_sysinfo->wifi_info_num >= 5)
    {
        LOGI("11 wifi_info_num  = %d ", e_sysinfo->wifi_info_num);
        for (int i = 1; i < 4; i++)
        {
            memset(e_sysinfo->wlan_sta_param[i].ssid, 0, GLOBE_SYSINFO_SSID_LEN_MAX) ;
            memset(e_sysinfo->wlan_sta_param[i].psk, 0, SYSINFO_PSK_LEN_MAX) ;
            e_sysinfo->wlan_sta_param[i].ssid_len = 0 ;

            strcpy((char *)e_sysinfo->wlan_sta_param[i].ssid, (char *)e_sysinfo->wlan_sta_param[i + 1].ssid) ;
            strcpy((char *)e_sysinfo->wlan_sta_param[i].psk, (char *)e_sysinfo->wlan_sta_param[i + 1].psk) ;
            e_sysinfo->wlan_sta_param[i].ssid_len = strlen((char *)e_sysinfo->wlan_sta_param[i].ssid) ;

            LOGI(" %d: ssid = %s ", i, e_sysinfo->wlan_sta_param[i].ssid);
        }
        strcpy((char *)e_sysinfo->wlan_sta_param[4].ssid, tmp_ssid) ;
        strcpy((char *)e_sysinfo->wlan_sta_param[4].psk, tmp_psk) ;

        e_sysinfo->wlan_sta_param[4].ssid_len = strlen((char *)e_sysinfo->wlan_sta_param[4].ssid) ;
        e_sysinfo->wifi_info_index = 5 ;
    }
    else
    {
        LOGI("22 wifi_info_num  = %d ", e_sysinfo->wifi_info_num);
        e_sysinfo->wifi_info_index = e_sysinfo->wifi_info_num - 1 ;

        memset(e_sysinfo->wlan_sta_param[e_sysinfo->wifi_info_num].ssid, 0, SYSINFO_SSID_LEN_MAX) ;
        memset(e_sysinfo->wlan_sta_param[e_sysinfo->wifi_info_num].psk, 0, SYSINFO_PSK_LEN_MAX) ;
        strcpy((char *)e_sysinfo->wlan_sta_param[e_sysinfo->wifi_info_num].ssid, tmp_ssid) ;
        strcpy((char *)e_sysinfo->wlan_sta_param[e_sysinfo->wifi_info_num].psk, tmp_psk) ;
        e_sysinfo->wlan_sta_param[e_sysinfo->wifi_info_num].ssid_len = strlen((char *)e_sysinfo->wlan_sta_param[e_sysinfo->wifi_info_num - 1].ssid) ;
        e_sysinfo->wifi_info_num++ ;
    }

    LOGI(" to save sysinfo .................. \n");
    sysinfo_save();
#endif
#endif
    return 0 ;
}
