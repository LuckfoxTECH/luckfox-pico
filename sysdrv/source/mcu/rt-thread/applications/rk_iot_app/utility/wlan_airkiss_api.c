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
#include <stdint.h>

#include "wlan_airkiss_api.h"
#ifdef RT_USING_WLAN_WICED
#include "wice_rtt_interface.h"
#include "wlan_mgnt.h"
#endif
#include "lwip/sockets.h"
#include "wlan_api.h"


#define DBG_SECTION_NAME            "AIRKISS_AL"
#include  "rkdebug.h"

wlan_airkiss_result_t airkiss;

wlan_airkiss_status_t wlan_airkiss_start(struct netif *nif, char *key)
{
    if (wlan_is_connected())
        wlan_sta_disconnect();
#ifdef RT_USING_WLAN_WICED
    rk_wifi_smartconfig();
#endif
    return WLAN_AIRKISS_SUCCESS;
}

airkiss_status_t wlan_airkiss_get_status(void)
{
#ifdef RT_USING_WLAN_WICED
    WIFI_AP *ap;
    ap = rk_wifi_smartconfig_get();
    if (ap == NULL)
    {
        return AIRKISS_STATUS_CONTINUE;
    }
    else
    {
        //rt_kprintf("status = %d", ap->ap_ssid_len);
        memcpy(airkiss.ssid, ap->ap_ssid_value, ap->ap_ssid_len);
        airkiss.ssid_len = ap->ap_ssid_len;
        airkiss.ssid[ap->ap_ssid_len] = 0;
        memcpy(airkiss.passphrase, ap->security_key, ap->security_key_length);
        airkiss.passphrase[ap->security_key_length] = 0;
        airkiss.random_num = ap->random;
        return AIRKISS_STATUS_COMPLETE;
    }
#else
    return 0;
#endif
}
wlan_airkiss_status_t wlan_airkiss_get_result(wlan_airkiss_result_t *result)
{
    memcpy(result, &airkiss, sizeof(wlan_airkiss_result_t));
    return WLAN_AIRKISS_SUCCESS;
}
wlan_airkiss_status_t wlan_airkiss_wait(uint32_t timeout_ms)
{
    //rt_kprintf("get wait");
    return WLAN_AIRKISS_SUCCESS;
}

wlan_airkiss_status_t
wlan_airkiss_connect_ack(struct netif *nif, uint32_t timeout_ms,
                         wlan_airkiss_result_t *result)
{
#ifdef RT_USING_WLAN_WICED
    //rt_kprintf("get connect ack");
    rt_wlan_connect((const char *)airkiss.ssid, (const char *)airkiss.passphrase);

    struct sockaddr_in localaddr_ack;
    int brocat_cast_ack = 1;
    int ret = -1;
    int timeout_ack = 3 * 1000;

    //recv socket
    //rk_printf("recv socket");
    int ack_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //rk_printf("airkiss_udp_socket = [ %d ]",airkiss_udp_socket);
    if (ack_socket < 0)
    {
        //rk_printf("brocadrecv socket fail");
        return WLAN_AIRKISS_FAIL;
    }

    memset(&localaddr_ack, 0, sizeof(localaddr_ack));
    localaddr_ack.sin_family = AF_INET;
    localaddr_ack.sin_port = htons(10000);
    localaddr_ack.sin_addr.s_addr = htonl(IPADDR_BROADCAST);

    ret = setsockopt(ack_socket, SOL_SOCKET, SO_BROADCAST, &brocat_cast_ack, sizeof(brocat_cast_ack));
    if (ret < 0)
    {
        //rk_printf("brocadrecv setopt fail");
        close(ack_socket);
        return WLAN_AIRKISS_FAIL;
    }

    //ÉèÖÃ³¬Ê±
    //rk_printf("set recv");
    ret = setsockopt(ack_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout_ack, sizeof(timeout_ack));
    if (ret < 0)
    {
        //rk_printf("brocadrecv set time out fail");
        close(ack_socket);
        return WLAN_AIRKISS_FAIL;
    }

    for (uint32_t i = 0; i < 10; i++)
    {
        int sendBytes;
        if ((sendBytes = sendto(ack_socket, &airkiss.random_num, 1, 0,
                                (struct sockaddr *)&localaddr_ack, sizeof(struct sockaddr))) == -1)
        {
            rt_kprintf("sendto fail, errno\n");
            close(ack_socket);
            return WLAN_AIRKISS_FAIL;
        }
        rt_thread_delay(100);
    }
    close(ack_socket);
    memcpy(result, &airkiss, sizeof(wlan_airkiss_result_t));
#endif
    return WLAN_AIRKISS_SUCCESS;
}

int wlan_airkiss_stop(void)
{
    LOGI("airkiss stop");
#ifdef RT_USING_WLAN_WICED
    rk_smartconfig_stop();
#endif
    return 0;
}
