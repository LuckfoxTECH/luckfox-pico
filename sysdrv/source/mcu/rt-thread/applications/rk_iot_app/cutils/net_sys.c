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
#include <string.h>
#include <stdlib.h>
//#include "kernel/os/os.h"
//#include "sys/ducc/ducc_net.h"
//#include "sys/ducc/ducc_app.h"
//#include "net/wlan/wlan.h"
#ifdef PRJCONF_NET_EN

#include "net/wlan/wlan_defs.h"
#include "lwip/tcpip.h"
#include "net/udhcp/usr_dhcpd.h"
#include "wlan_api.h"

#include "sysinfo.h"
// #include "common/board/board.h"
#include "net_ctrl.h"
#include "net_ctrl_debug.h"

// #include "driver/chip/hal_prcm.h"

struct netif *g_wlan_netif = NULL;

static void netif_tcpip_init_done(void *arg)
{
//  NET_DBG("%s()\n", __func__);
}

void net_sys_init(void)
{
    tcpip_init(netif_tcpip_init_done, NULL); /* Init lwip module */
}

#if LWIP_XR_DEINIT
void net_sys_deinit(void)
{
    tcpip_deinit(); /* DeInit lwip module */
}
#endif

static int net_sys_callback(uint32_t param0)
{
//  switch (param0) {
//  case WLAN_EVENT_CONNECTED:
//  case WLAN_EVENT_DISCONNECTED:
//  case WLAN_EVENT_SCAN_SUCCESS:
//  case WLAN_EVENT_SCAN_FAILED:
//  case WLAN_EVENT_4WAY_HANDSHAKE_FAILED:
//  case WLAN_EVENT_CONNECT_FAILED:
//  case WLAN_EVENT_CONNECTION_LOSS:
//      net_ctrl_msg_send(param0, 0);
//      break;
//  default:
//      break;
//  }

    net_ctrl_msg_send(param0, 0);
    return 0;
}

int net_sys_start(enum wlan_mode mode)
{
    struct sysinfo *sysinfo = sysinfo_get();
    if (sysinfo == NULL)
    {
        NET_ERR("failed to get sysinfo %p\n", sysinfo);
        return -1;
    }

    NET_DBG(" MODE = %d \n", mode);
    wlan_set_state_callback(net_sys_callback);
    wlan_set_mac_addr(sysinfo->mac_addr, SYSINFO_MAC_ADDR_LEN);

    g_wlan_netif = net_open(mode);
    return 0;
}

int net_sys_stop(void)
{
    net_close(g_wlan_netif);
    g_wlan_netif = NULL;

    return 0;
}

int net_sys_onoff(unsigned int enable)
{
    struct sysinfo *sysinfo = sysinfo_get();
    if (sysinfo == NULL)
    {
        NET_ERR("failed to get sysinfo %p\n", sysinfo);
        return -1;
    }

    printf("%s set net to power%s\n", __func__, enable ? "on" : "off");

    if (enable)
    {
        net_sys_start(sysinfo->wlan_mode);
#ifdef CONFIG_AUTO_RECONNECT_AP
        net_ctrl_connect_ap(NULL);
#endif
    }
    else
    {
#ifdef CONFIG_AUTO_RECONNECT_AP
        net_ctrl_disconnect_ap(NULL, 1);
#endif
        net_sys_stop();
    }

    return 0;
}

#endif /* PRJCONF_NET_EN */
