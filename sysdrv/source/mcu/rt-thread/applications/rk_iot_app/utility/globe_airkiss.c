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
#include <rtthread.h>
#include <rtdef.h>

#include "globe_airkiss.h"
#include "globe_player.h"
#include "flash_play.h"
#include "globe_system_stat.h"

#include "wlan_airkiss_api.h"

#define DBG_LEVEL           DBG_LOG
#define DBG_SECTION_NAME    "CONN-WIFI"
#include "rkdebug.h"

// static char *e_airkisskey = "1234567812345678";//
bool g_net_stat = false ;
int wifi_check_result(/*struct sysinfo * info*/) ;
void NetCtrlworkState(uint32_t event, uint32_t data, void *arg) ;

#ifdef PRJCONF_NET_EN

#define CONNECT_WIFI_CTRL_THREAD_STACK_SIZE (4 * 1024)
rt_thread_t connect_wifi_thread = RT_NULL;

uint8_t scan_result = 0;
void connectWifiTask(void *arg)
{
    RtosSystemStat_Fun.NetConnecting = true ;
    struct sysinfo *e_sysinfo = (struct sysinfo *)arg;
    LOGD("Start Config WiFI.");

    LOGD("e_sysinfo->wifi_info_num:%d", e_sysinfo->wifi_info_num);
    LOGD("e_sysinfo->wifi_info_index:%d", e_sysinfo->wifi_info_index);
    for (int i = 0; i < 5; ++i)
    {
        if (strlen((char *)(e_sysinfo->wlan_sta_param[i].ssid)) > 0)
            LOGD("[%d] SSID: %s", i, (char *)(e_sysinfo->wlan_sta_param[i].ssid)) ;
        else
        {
            LOGD("[%d] SSID: NULL", i) ;
        }
    }

    uint8_t scan_count = 4 ;
    uint8_t time_num = 0;
    scan_result = 0 ;
    while (scan_count--)
    {
        int ret = wlan_sta_scan_once();
        scan_result = 0;
        if (ret != 0)
        {
            continue;
        }
        else
        {
            time_num = 0;
            while (scan_result == 0)
            {
                rt_thread_mdelay(100);
                time_num ++;
                if (time_num >= 20)
                {
                    LOGD("\n scan wifi Time out !!!\n");
                    goto CONNECT_ERR;
                }
            }
            if (scan_result == 2)
            {
                if (RtosTestModeFun.TestMode)
                {
                    RtosTestModeFun.WifiStat = 1 ;
                    goto CONNECT_OK ;
                }
                // Get wifi info
                int ret = wifi_check_result(e_sysinfo) ;
                if (ret != -1)
                {
                    // WIFI_PROFILE_DBG("\n-connect ssid: %s ,passphrase:%s\n",wifi_info.result[ret].ssid,wifi_info.result[ret].passphrase);
                    // net_config_conn(&wifi_info.wlan_sta_param[ret]);
                    wlan_sta_set(e_sysinfo->wlan_sta_param[ret].ssid, e_sysinfo->wlan_sta_param[ret].ssid_len, e_sysinfo->wlan_sta_param[ret].psk);
                    wlan_sta_enable();
                    wlan_sta_states_t state = WLAN_STA_STATE_DISCONNECTED;
                    for (int i = 0; state == WLAN_STA_STATE_DISCONNECTED; i++)
                    {
                        if (i >= 50)
                        {
                            LOGD("Connect WiFI failed.....");
                            goto CONNECT_ERR ;
                            // char* network_failed= "0:/music/network_failed.mp3";
                            // rtos_player_warning_feed(network_failed);
                        }
                        LOGD("SSID:%s    SSID_LEN:%d    PSK:%s    PSK_LEN:%d", e_sysinfo->wlan_sta_param[ret].ssid, e_sysinfo->wlan_sta_param[ret].ssid_len, e_sysinfo->wlan_sta_param[ret].psk, strlen((char *)e_sysinfo->wlan_sta_param[ret].psk)) ;
                        wlan_sta_state(&state);
                        // LOGD("Connecting WiFI......");
                        rt_thread_delay(100);
                    }
                    e_sysinfo->wifi_info_index = ret ;
                    g_net_stat = true ;
                    goto CONNECT_OK ;
                }
            }
            else
            {
                continue ;
            }
        }
    }

CONNECT_ERR:
    if (RtosTestModeFun.TestMode)
    {
        RtosTestModeFun.WifiStat = 2 ;
        g_net_stat = false ;
        RtosSystemStat_Fun.NetConnecting = false ;
        //rt_thread_delete(connect_wifi_thread);    //don't do this, will lead to fault
        connect_wifi_thread = RT_NULL;
        return ;
    }

    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_Standby) ;
    Rtos_Flash_Fun.gptTonePlay(TONE_SETUP_NET) ;

#if 0
    Rtos_Flash_Fun.gptMenu(Rtos_Menu_Type_ChildrenSong) ;
#endif

    g_net_stat = false ;
    RtosSystemStat_Fun.MenuFlag = true ;
    RtosSystemStat_Fun.NetConnecting = false ;
    //rt_thread_delete(connect_wifi_thread);     //don't do this, will lead to fault
    connect_wifi_thread = RT_NULL;
    LOGD("\n=== Connect Fail ===\n");

    return ;

CONNECT_OK:
    if (RtosTestModeFun.TestMode)
    {
        RtosTestModeFun.WifiStat = 1 ;
        RtosSystemStat_Fun.NetConnecting = false ;
//        OS_ThreadDelete(&connect_wifi_thread);
        //rt_thread_delete(connect_wifi_thread);
        connect_wifi_thread = RT_NULL;
        return ;
    }
    LOGD("\n NET_CONNECTED \n") ;
    // Rtos_Flash_Fun.gptTonePlay(NET_CONNECTED) ;
    RtosSystemStat_Fun.NetConnecting = false ;
    //rt_thread_delete(connect_wifi_thread);
    connect_wifi_thread = RT_NULL;

    return ;
}

//uint16_t type = EVENT_SUBTYPE(event)
int connectWifi(struct sysinfo *e_sysinfo)
{
    if (connect_wifi_thread)
    {
        rt_kprintf("\n === Connect wifi task exist already ===\n");
        return 0;
    }

    RtosSystemStat_Fun.Send(SystemStat_Ctrl_Stat, SystemStat_Ctrl_Stat_NetInit) ;

    connect_wifi_thread = rt_thread_create("network_state",
                                           connectWifiTask,
                                           (void *)e_sysinfo,
                                           CONNECT_WIFI_CTRL_THREAD_STACK_SIZE,
                                           RT_THREAD_PRIORITY_MAX / 2, 10);
    if (connect_wifi_thread != RT_NULL)
        rt_thread_startup(connect_wifi_thread);
    else
        LOGE("create network state task failed\n");

    LOGD("\n### network_state_obs ! ###\n");
    observer_base *network_info_obs = sys_callback_observer_create(CTRL_MSG_TYPE_WIFI_INFO, \
                                      CTRL_MSG_SUB_TYPE_NETWORK_ALL, NetCtrlworkState, NULL);
    if (network_info_obs == NULL)
    {
        LOGE(("create network_state_obs failed."));
    }
    sys_ctrl_attach(network_info_obs);
    return 0;
}


int wifi_check_result(struct sysinfo *info)
{
    int ret = -1;
    wlan_sta_scan_results_t result;

    LOGD("%s() Enter \n", __func__);
    memset(&result, 0, sizeof(result));
    result.size = 20;
    result.ap = rt_malloc(20 * sizeof(wlan_sta_ap_t));;
    ret = wlan_sta_scan_result(&result);
    if (ret == 0)
    {
        int count = result.num;

        int i = 0, j = 0, index = info->wifi_info_index  ;
        for (i = 0; i < count; i++)
        {
            // rt_kprintf("ssid =  %s  level = %d\n", result.ap[i].ssid.ssid, result.ap[i].level);
            LOGD("index:%d", info->wifi_info_index);
            LOGD("=== SSID-1:%s , SSID-2:%s ===", result.ap[i].ssid.ssid, info->wlan_sta_param[index].ssid);
            if (strcmp((char *)result.ap[i].ssid.ssid, (char *)info->wlan_sta_param[index].ssid) == 0)
            {
                LOGD(" Found ssid in %d", index);
                rt_free(result.ap);
                return index;
            }
        }

        LOGD("Scan result total have %d ssid", count);
        for (j = 0; j < info->wifi_info_num; j++)
        {
            // if(j == info.index) continue;

            for (i = 0; i < count; i++)
            {
                LOGI("---ssid-1:%s , ssid-2:%s", result.ap[i].ssid.ssid, info->wlan_sta_param[j].ssid);
                if (strcmp((char *)(result.ap[i].ssid.ssid), (char *)(info->wlan_sta_param[j].ssid)) == 0)
                {
                    rt_free(result.ap);
                    return j;
                }
            }
        }
    }
    else
    {
        LOGD("wifi get scan resutl error");
    }

    rt_free(result.ap);
    return -1;
}

void disconnectWifi(void)
{
    wlan_sta_states_t state;
    wlan_sta_state(&state);
    if (state == WLAN_STA_STATE_CONNECTED)
        wlan_sta_disconnect();
    g_net_stat = false ;
}

int networkStart(struct sysinfo *e_sysinfo)
{
    int ret;
    LOGD("%s() Enter  000 ", __func__);
    e_sysinfo = sysinfo_get();
    if (e_sysinfo == RT_NULL)
    {
        LOGE(" e_sysinfo == RT_NULL ");
        return -1;
    }

#if 0
    strcpy((char *)e_sysinfo->wlan_sta_param.ssid, "testwifi") ;
    e_sysinfo->wlan_sta_param.ssid_len = strlen("testwifi") ;
    strcpy((char *)e_sysinfo->wlan_sta_param.psk, "88888888") ;
#endif

    if (e_sysinfo->wifi_info_num == 0 || e_sysinfo->wifi_info_num == 1)
    {
        memset(e_sysinfo->wlan_sta_param[0].ssid, 0, SYSINFO_SSID_LEN_MAX) ;
        memset(e_sysinfo->wlan_sta_param[0].psk, 0, SYSINFO_PSK_LEN_MAX) ;
        //strcpy((char *)e_sysinfo->wlan_sta_param[0].ssid, "TP-LINK_383E") ;
        //strcpy((char *)e_sysinfo->wlan_sta_param[0].psk, "rockchip88") ;
        strcpy((char *)e_sysinfo->wlan_sta_param[0].ssid, "testwifi") ;
        strcpy((char *)e_sysinfo->wlan_sta_param[0].psk, "88888888") ;
        e_sysinfo->wlan_sta_param[0].ssid_len = 13 ;
        e_sysinfo->wifi_info_num = 1 ;

        ret = sysinfo_save();
        if (ret != 0)
        {
            LOGE("ret = %d\n", ret);
            return -1;
        }
    }
    LOGD("e_sysinfo->wifi_info_num  :%d\n", e_sysinfo->wifi_info_num);
    LOGD("e_sysinfo->wifi_info_index:%d\n", e_sysinfo->wifi_info_index);
    connectWifi(e_sysinfo);

    LOGD(" --- networkStart Done ---\n");
    return 0;
}

int cleanNetworkInfo(void)
{
    LOGD("### Clean Network Infomation... ### \n");
//    struct sysinfo wifi_info;
    disconnectWifi();
    return 0;
}


#define NETWORK_STATE_CTRL_THREAD_STACK_SIZE    (1 * 1024)
rt_thread_t network_state_thread;

void networkDisconnectPublisherTask(void        *arg)
{
    wlan_sta_states_t state;
    while (1)
    {
        wlan_sta_state(&state);
        if (state == WLAN_STA_STATE_DISCONNECTED)
        {
            if (sys_event_send(CTRL_MSG_TYPE_NETWORK, CTRL_MSG_SUB_TYPE_NETWORK_DISCONNECT, 0, 0) != 0)
            {
                LOGE("send network state error\n");
            }
            rt_thread_delay(30);
        }
        rt_thread_delay(15);
    }

    rt_thread_delete(network_state_thread);
}

void networkDisconnectPublisher(void)
{
    network_state_thread = rt_thread_create("network_state",
                                            networkDisconnectPublisherTask,
                                            RT_NULL,
                                            NETWORK_STATE_CTRL_THREAD_STACK_SIZE,
                                            RT_THREAD_PRIORITY_MAX / 2, 10);
    if (network_state_thread != RT_NULL)
    {
        rt_thread_startup(network_state_thread);
    }
    else
    {
        LOGE("create network state task failed\n");
    }
}

void printNetworkState(uint32_t event, uint32_t data, void *arg)
{
    if (EVENT_SUBTYPE(event) == CTRL_MSG_SUB_TYPE_NETWORK_DISCONNECT)
    {
        LOGD("@@@ disconnet!!! @@@ \n");
    }

    if (EVENT_SUBTYPE(event) == CTRL_MSG_SUB_TYPE_NETWORK_CONNECT)
    {
        LOGD("@@@ Connect !!! @@@ \n");
    }
}
void NetCtrlworkState(uint32_t event, uint32_t data, void *arg)
{
    uint16_t type = EVENT_SUBTYPE(event) ;
    static bool net_up_flag = false ;

    if (type != NET_CTRL_MSG_NETWORK_UP)
    {
        net_up_flag = false ;
    }

    switch (type)
    {
    case NET_CTRL_MSG_WLAN_CONNECTED:
        LOGD("NET_CTRL_MSG_WLAN_CONNECTED") ;
        break;
    case NET_CTRL_MSG_WLAN_DISCONNECTED:
        LOGD("NET_CTRL_MSG_WLAN_DISCONNECTED") ;
        if (g_net_stat == false)
        {
            break ;
        }
        g_net_stat = false ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Net, 0) ;
        break;
    case NET_CTRL_MSG_WLAN_SCAN_SUCCESS:
        LOGD("NET_CTRL_MSG_WLAN_SCAN_SUCCESS") ;
        scan_result = 2 ;
        break;
    case NET_CTRL_MSG_WLAN_SCAN_FAILED:
        LOGD("NET_CTRL_MSG_WLAN_SCAN_FAILED") ;
        scan_result = 1 ;
        break;
    case NET_CTRL_MSG_WLAN_4WAY_HANDSHAKE_FAILED:
        LOGD("NET_CTRL_MSG_WLAN_4WAY_HANDSHAKE_FAILED") ;
        break;
    case NET_CTRL_MSG_WLAN_CONNECT_FAILED:
        LOGD("NET_CTRL_MSG_WLAN_CONNECT_FAILED") ;
        break;
    case NET_CTRL_MSG_CONNECTION_LOSS:
        LOGD("NET_CTRL_MSG_CONNECTION_LOSS") ;
        if (g_net_stat == false)
        {
            break ;
        }
        g_net_stat = false ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Net, 0) ;
        // Rtos_Flash_Fun.gptTonePlay(NET_CUTOFF) ;
        break;
    case NET_CTRL_MSG_NETWORK_UP:
        LOGD("NET_CTRL_MSG_NETWORK_UP") ;
        if (net_up_flag)
        {
            break ;
        }
        net_up_flag = true ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Net, 1) ;
        // Rtos_Flash_Fun.gptTonePlay(NET_CONNECTED) ;
        break;
    case NET_CTRL_MSG_NETWORK_DOWN:
        LOGD("NET_CTRL_MSG_NETWORK_DOWN") ;
        if (g_net_stat == false)
        {
            break ;
        }
        g_net_stat = false ;
        RtosSystemStat_Fun.Send(SystemStat_Ctrl_Net, 0) ;
        break;
    default:
        break;
    }
}

void NetworkStateAttach(void)
{
    LOGD(" create network state observer !!!\n");
    observer_base *network_state_obs = sys_callback_observer_create(CTRL_MSG_TYPE_NETWORK, \
                                       CTRL_MSG_SUB_TYPE_NETWORK_ALL, printNetworkState, NULL);
    if (network_state_obs == NULL)
    {
        LOGE(("create network_state_obs failed."));
    }
    sys_ctrl_attach(network_state_obs);
}

#endif
