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

#include "rkdef.h"
#include "lwip/netif.h"
#include "wlan_api.h"
#include "net_ctrl.h"

#define DBG_SECTION_NAME            "WLAN_AL"
#include  "rkdebug.h"
#ifdef RT_USING_WLAN_WICED
#include "wice_rtt_interface.h"
#include "wlan_mgnt.h"
#endif

struct netif gNetif;
#ifdef RT_USING_WLAN_WICED
WIFI_AP ap;
#endif
wlan_cb_func wlan_callback = NULL;

struct netif *wlan_create(enum wlan_mode mode)
{
    struct netif *nif = NULL;
    nif = (struct netif *)&gNetif;

    /* TODO */
    //......
    LOGD("create.......................");
#ifdef RT_USING_WLAN_WICED
    wice_create();
#endif
    return nif;
}

int wlan_delete(struct netif *nif)
{
    if (!nif)
        return -1;

    /* TODO */
    //......

    rt_free(nif);
    nif = NULL;
    return 0;
}


int wlan_set_state_callback(wlan_cb_func cb)
{
    LOGD("callback.......................");
    wlan_callback = cb;
    return 0;
}

int wlan_set_mac_addr(uint8_t *mac_addr, int mac_len)
{
#ifdef RT_USING_WLAN_WICED
    LOGD("mac address.......................");
    rt_wlan_set_mac(mac_addr);
#endif
    return 0;
}

/* STA */
int wlan_sta_set(uint8_t *ssid, uint8_t ssid_len, uint8_t *psk)
{
#ifdef RT_USING_WLAN_WICED
    //rt_kprintf("\nset sta ssid= %s, psk = %s, ssid_len = %d",ssid, psk, ssid_len);
    memcpy(ap.ap_ssid_value, ssid, ssid_len + 1);
    ap.ap_ssid_len = ssid_len;
    ap.security_key_length = strlen((const char *)psk);
    memcpy(ap.security_key, psk, ap.security_key_length + 1);
#endif
    return 0;
}

int wlan_sta_enable(void)
{
    LOGD("enable.......................");
#ifdef RT_USING_WLAN_WICED
    rt_wlan_connect((const char *)ap.ap_ssid_value, (const char *)ap.security_key);
#endif
    return 0;
}

int wlan_sta_disable(void)
{
    LOGD("disable.......................");
    return 0;
}

int wlan_sta_scan_once(void)
{
#ifdef RT_USING_WLAN_WICED
    struct rt_wlan_scan_result *scan_result = RT_NULL;
    LOGD(" scan once......start");
    rt_wlan_scan_result_clean();
    scan_result  = rt_wlan_scan_sync();
    LOGD("scan once.......end");
    if (scan_result && scan_result->num)
    {
        if (wlan_callback)
            wlan_callback(WLAN_EVENT_SCAN_SUCCESS);
    }
    else
    {
        if (wlan_callback)
            wlan_callback(WLAN_EVENT_SCAN_FAILED);
    }
#endif
    return 0;
}

int wlan_sta_scan_result(wlan_sta_scan_results_t *results)
{
    LOGD("scan result...");
#ifdef RT_USING_WLAN_WICED
    struct rt_wlan_scan_result *rs = rt_wlan_scan_get_result();
    int realnum = rs->num > results->size ? results->size : rs->num;

    for (int i = 0; i < realnum; i++)
    {
        memcpy(results->ap[i].bssid, rs->info[i].bssid, 6);
        memcpy(results->ap[i].ssid.ssid, rs->info[i].ssid.val, rs->info[i].ssid.len);
        results->ap[i].ssid.ssid_len = rs->info[i].ssid.len;
        results->ap[i].channel = rs->info[i].channel;
        results->ap[i].rssi = rs->info[i].rssi;
    }

    results->num = realnum;
#endif
    return 0;
}

int wlan_sta_disconnect(void)
{
    LOGD("disconnect...");
#ifdef RT_USING_WLAN_WICED
    rt_wlan_disconnect();
#endif
    return 0;
}

rt_bool_t wlan_is_connected(void)
{
#ifdef RT_USING_WLAN_WICED
    if (rt_wlan_is_connected() == RT_TRUE)
        return RT_TRUE;
    else
        return RT_FALSE;
#else
    return RT_TRUE;
#endif
}


int wlan_sta_state(wlan_sta_states_t *state)
{
    LOGD("sta state.......................");
#ifdef RT_USING_WLAN_WICED
#ifdef PRJCONF_NET_EN

    if (rt_wlan_is_connected() == RT_TRUE)
    {
        *state = WLAN_STA_STATE_CONNECTED;
        LOGD(" WLAN connected!!! ");
        if (wlan_callback)
            wlan_callback(NET_CTRL_MSG_NETWORK_UP);
    }
    else
    {
        *state = WLAN_STA_STATE_DISCONNECTED;
        LOGD(" WLAN Disconnected!!! ");
        if (wlan_callback)
            wlan_callback(NET_CTRL_MSG_NETWORK_DOWN);
    }
#endif
#endif
    return 0;
}

int wlan_sta_create(void)
{
    //rt_kprintf("\n sta create.......................");
    return 0;
}

int wlan_sta_delete(void)
{
    //rt_kprintf("\n sta delete.......................");
    return 0;
}

