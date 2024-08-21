/*
 * File      : drv_wifi.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2017, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 */
//#include <skbuff.h>
#include <rtdef.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdef.h>
#include <wlan_dev.h>
#include <wlan_prot.h>
#include "board.h"
#include <string.h>

#include "drv_wlan.h"
#include "drv_wifi.h"
//#include "wwd_network_interface.h"
#include "wwd_wifi.h"
#include "pbuf.h"

#define DBG_LEVEL DBG_INFO
#define DBG_SECTION_NAME  "WIFI"
#include <rtdbg.h>

#define MAX_ADDR_LEN          (6)
#define RT_WLAN_SSID_MAX_LEN  (32)

#define WIFI_INIT_FLAG        (0x1 << 0)
#define WIFI_MAC_FLAG         (0x1 << 1)
#define WIFI_TYPE_STA         (0)
#define WIFI_TYPE_AP          (1)

struct cy_wiced_wifi
{
    struct rt_wlan_device *wlan;
    rt_uint8_t dev_addr[MAX_ADDR_LEN];
    rt_uint8_t flag;
    int connected;
    int type;
};



static const struct rt_wlan_dev_ops wiced_wifi_ops;

static struct cy_wiced_wifi wiced_sta;
static struct cy_wiced_wifi wiced_ap;

static struct rt_wlan_device wiced_wlan_sta;
static struct rt_wlan_device wiced_wlan_ap;

rt_inline struct cy_wiced_wifi *rthw_wifi_get_dev(int idx)
{
    if (idx == 0) return &wiced_sta;
    if (idx == 1) return &wiced_ap;
    return RT_NULL;
}

rt_inline int rthw_wifi_get_idx(struct cy_wiced_wifi *wifi)
{
    int mode = rthw_wifi_mode_get();

    if (mode == 1) return 0;
    if (mode == 2) return 1;
    return wifi->type;
}

int rthw_wifi_register(struct cy_wiced_wifi *wifi)
{
    struct rt_wlan_device *wlan = wifi->wlan;

    if ((wifi->flag & WIFI_INIT_FLAG) == 0)
    {
        if (wifi->type == WIFI_TYPE_STA)
        {
            rt_wlan_dev_register(wlan, RT_WLAN_DEVICE_STA_NAME, &wiced_wifi_ops, 0, wifi);
        }
        if (wifi->type == WIFI_TYPE_AP)
        {
            rt_wlan_dev_register(wlan, RT_WLAN_DEVICE_AP_NAME, &wiced_wifi_ops, 0, wifi);
        }

        wifi->flag |= WIFI_INIT_FLAG;
        wifi->wlan = wlan;
        rt_kprintf("\n Yingbo::  F:%s L:%d wifi: 0x%08x wlan: 0x%08x\n", __FUNCTION__, __LINE__, wifi, wlan);
    }
    return RT_EOK;
}



void rthw_wlan_set_netif_info(int idx, rt_device_t *dev, rt_uint8_t *dev_addr)
{
    struct cy_wiced_wifi *wifi = RT_NULL;


    wifi = rthw_wifi_get_dev(idx);
    if (wifi == RT_NULL)
        return;
    LOG_E("F:%s L:%d idx %d  wifi:0x%08x  flag:0x%x \n", __FUNCTION__, __LINE__, idx, wifi, wifi->flag);
    rthw_wifi_register(wifi);
    *dev = &wifi->wlan->device;
    LOG_E("wifi type:%d \n", wifi->type);
}

void rthw_connect_callback(void)
{
    struct cy_wiced_wifi *wifi = &wiced_sta;

    LOG_E("Yingbo :: L:%d wifi connect callback \n\n", __LINE__);


    if (wifi->connected == 0)
    {
        if (wwd_wifi_is_ready_to_transceive(0) == 0)
        {
            wifi->connected = 1;
            rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_CONNECT, RT_NULL);
        }
        else
        {
            rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_CONNECT_FAIL, RT_NULL);
        }
    }
    else
    {
        if (wwd_wifi_is_ready_to_transceive(0))
        {
            wifi->connected = 0;
            rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_DISCONNECT, RT_NULL);
        }
    }
}


static void rtw_connect_fail_callbackfn(char *buf, int len, int flags, void *user_data)
{
    struct cy_wiced_wifi *wifi = user_data;

    LOG_D("L:%d wifi connect callback flags:%d", __LINE__, flags);
    wifi->connected = 0;
    rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_CONNECT_FAIL, RT_NULL);
}

static void rtw_disconnect_callbackfn(char *buf, int len, int flags, void *user_data)
{
    struct cy_wiced_wifi *wifi = user_data;

    LOG_D("L:%d wifi disconnect callback flags:%d", __LINE__, flags);
    wifi->connected = 0;
    rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_DISCONNECT, RT_NULL);
}

static void rtw_sta_assoc_callbackfn(char *buf, int len, int flags, void *user_data)
{
    LOG_D("L:%d wifi sta assoc callback flags:%d", __LINE__, flags);
}

static void rtw_sta_disassoc_callbackfn(char *buf, int len, int flags, void *user_data)
{
    LOG_D("L:%d wifi sta assoc callback flags:%d buf:%08x %08x", __LINE__, flags, *((rt_uint32_t *)buf), *((rt_uint32_t *)buf + 4));
}

static wiced_security_t security_map_from_rtthread(rt_wlan_security_t security)
{
    wiced_security_t result = WICED_SECURITY_OPEN;

    switch (security)
    {
    case SECURITY_OPEN:
        result = WICED_SECURITY_OPEN;
        break;
    case SECURITY_WEP_PSK:
        result = WICED_SECURITY_WEP_PSK;
        break;
    case SECURITY_WEP_SHARED:
        result = WICED_SECURITY_WEP_SHARED;
        break;
    case SECURITY_WPA_TKIP_PSK:
        result = WICED_SECURITY_WPA_TKIP_PSK;
        break;
    case SECURITY_WPA_AES_PSK:
        result = WICED_SECURITY_WPA_AES_PSK;
        break;
    case SECURITY_WPA2_AES_PSK:
        result = WICED_SECURITY_WPA2_AES_PSK;
        break;
    case SECURITY_WPA2_TKIP_PSK:
        result = WICED_SECURITY_WPA2_TKIP_PSK;
        break;
    case SECURITY_WPA2_MIXED_PSK:
        result = WICED_SECURITY_WPA2_MIXED_PSK;
        break;
    case SECURITY_WPS_OPEN:
        result = WICED_SECURITY_WPS_OPEN;
        break;
    case SECURITY_WPS_SECURE:
        result = WICED_SECURITY_WPS_SECURE;
        break;
    default:
        result = -1;
        break;
    }

    return result;
}

static void rthw_wlan_monitor_callback(wiced_buffer_t buf, int interface)
{
    struct rt_wlan_device *wlan = NULL;
    struct pbuf *p = buf;


    if (interface == WWD_AP_INTERFACE)
    {
        wlan = wiced_ap.wlan;
    }
    else if (interface == WWD_STA_INTERFACE)
    {
        wlan = wiced_sta.wlan;
    }
    else
        return;

    rt_wlan_dev_promisc_handler(wlan, p->payload, p->len);
    pbuf_free(p);
}

static rt_err_t rthw_wlan_init(struct rt_wlan_device *wlan)
{
    struct cy_wiced_wifi *wifi = wlan->user_data;

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    //rthw_wifi_start(RTHW_MODE_STA_AP);
    if (wifi->type == WIFI_TYPE_AP)
        rthw_wifi_start(RTHW_MODE_AP);
    else if (wifi->type == WIFI_TYPE_STA)
        rthw_wifi_start(RTHW_MODE_STA);

    return RT_EOK;
}

rthw_mode_t wifi_mode;
static rt_err_t rthw_wlan_mode(struct rt_wlan_device *wlan, rt_wlan_mode_t mode)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d mode:%d", __FUNCTION__, __LINE__, mode);

    if (mode == RT_WLAN_STATION)
    {
        if (wifi->type != WIFI_TYPE_STA)
        {
            LOG_D("this wlan not support sta mode");
            return -RT_ERROR;
        }
    }
    else if (mode == RT_WLAN_AP)
    {
        if (wifi->type != WIFI_TYPE_AP)
        {
            LOG_D("this wlan not support ap mode");
            return -RT_ERROR;
        }
    }
    wifi_mode = mode;

    return RT_EOK;
}

static rt_err_t rthw_wlan_scan(struct rt_wlan_device *wlan, struct rt_scan_info *scan_info)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    LOG_E("F:%s L:%d wifi:0x%08x type:0x%x", __FUNCTION__, __LINE__, wifi, wifi->type);
    LOG_E("F:%s L:%d wlan:0x%08x", __FUNCTION__, __LINE__, wlan);

    if (wifi->type != WIFI_TYPE_STA)
    {
        LOG_D("this wlan not support scan mode");
        return -RT_ERROR;
    }
    if (rthw_wifi_mode_get() == RTHW_MODE_NONE)
    {
        if (rthw_wifi_start(RTHW_MODE_STA_AP) != RT_EOK)
        {
            LOG_D("L:%d wifistart failed...", __LINE__);
            return -1;
        }
    }
    rthw_wifi_scan((void *)wlan, scan_info);
    return RT_EOK;
}

static rt_err_t rthw_wlan_join(struct rt_wlan_device *wlan, struct rt_sta_info *sta_info)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);
    int result = 0, i;
    char *ssid = RT_NULL, *key = RT_NULL;

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    if (wifi->type != WIFI_TYPE_STA)
    {
        LOG_E("this wlan not support sta mode");
        return -RT_ERROR;
    }
    if ((rthw_wifi_mode_get() != RTHW_MODE_STA) && (rthw_wifi_mode_get() != RTHW_MODE_STA_AP))
    {
        rthw_wifi_stop();
        rt_thread_delay(RT_TICK_PER_SECOND / 10);
        if (rthw_wifi_start(RTHW_MODE_STA_AP) != RT_EOK)
        {
            LOG_E("wifi on failed, join fail");
            return -RT_ERROR;
        }
    }
    for (i = 0; i < RT_WLAN_BSSID_MAX_LENGTH; i++)
    {
        if (sta_info->bssid[i] != 0xff || sta_info->bssid[i] != 0x00)
            break;
    }
    /*    if (i < RT_WLAN_BSSID_MAX_LENGTH)
        {
            if (sta_info->ssid.len > 0)
                ssid = &sta_info->ssid.val[0];
            if (sta_info->key.len > 0)
                key = &sta_info->key.val[0];
                LOG_D("bssid connect bssid: %02x:%02x:%02x:%02x:%02x:%02x ssid:%s ssid_len:%d key:%s key_len%d",
                sta_info->bssid[0],sta_info->bssid[1],sta_info->bssid[2],sta_info->bssid[3],sta_info->bssid[4],sta_info->bssid[5],
                ssid,
                sta_info->ssid.len,
                key,
                sta_info->key.len
                );
            result = rthw_wifi_connect_bssid(sta_info->bssid, ssid, sta_info->ssid.len, key, sta_info->key.len, security_map_from_rtthread(sta_info->security));
        }
        else */
    {
        LOG_E("rthw_wifi_connect  ssid %s, key_len %d, key %s i %d \n", sta_info->ssid.val, sta_info->key.len, sta_info->key.val, i);
        if (sta_info->key.len == 0)
            sta_info->security = RTHW_SECURITY_OPEN;
        else if (sta_info->key.len >= 8)
            sta_info->security = RTHW_SECURITY_WPA2_MIXED_PSK;
        result = rthw_wifi_connect(sta_info->ssid.val, sta_info->ssid.len, sta_info->key.val, sta_info->key.len, security_map_from_rtthread(sta_info->security));
    }
    if (result != 0)
    {
        LOG_E("cypress wiced_wifi_connect failed...");
        return -RT_ERROR;
    }
    // netif_set_connected((struct cy_wiced_wifi *)wlan, 1);
    LOG_D("cypress wiced_wifi_connect do");
    return RT_EOK;
}

static rt_err_t rthw_wlan_softap(struct rt_wlan_device *wlan, struct rt_ap_info *ap_info)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);
    char *psk = NULL;

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    if (wifi->type != WIFI_TYPE_AP)
    {
        LOG_E("this wlan not support ap mode");
        return -RT_ERROR;
    }

    if (ap_info->security != 0)
        psk = &ap_info->key.val[0];

    if (rthw_wifi_ap_start(&ap_info->ssid.val[0], psk, ap_info->channel) != 0)
    {
        rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_AP_STOP, RT_NULL);
        wifi->connected = 0;
        return -RT_ERROR;
    }
    rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_AP_START, RT_NULL);
    wifi->connected = 1;
    return RT_EOK;
}

static rt_err_t rthw_wlan_disconnect(struct rt_wlan_device *wlan)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    if (wifi->type != WIFI_TYPE_STA)
    {
        LOG_E("this wlan not support sta mode");
        return -RT_ERROR;
    }
    wifi->connected = 0;
    rthw_wifi_sta_disconnect();
    rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_DISCONNECT, RT_NULL);
    return RT_EOK;
}

static rt_err_t rthw_wlan_ap_stop(struct rt_wlan_device *wlan)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    if (wifi->type != WIFI_TYPE_AP)
    {
        LOG_E("this wlan not support ap mode");
        return -RT_ERROR;
    }

    rthw_wifi_ap_disconnect();
    rt_wlan_dev_indicate_event_handle(wifi->wlan, RT_WLAN_DEV_EVT_AP_STOP, RT_NULL);
    return RT_EOK;
}

static rt_err_t rthw_wlan_ap_deauth(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    return RT_EOK;
}

static rt_err_t rthw_wlan_scan_stop(struct rt_wlan_device *wlan)
{
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    if (rthw_wifi_scan_stop() == 0)
        return RT_EOK;
    else
        return -RT_ERROR;
}

static int rthw_wlan_get_rssi(struct rt_wlan_device *wlan)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    if (wifi->type != WIFI_TYPE_STA)
    {
        LOG_E("this wlan not support sta mode");
        return -RT_ERROR;
    }

    return rthw_wifi_rssi_get();
}

static rt_err_t rthw_wlan_set_powersave(struct rt_wlan_device *wlan, int level)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    if (wifi->type != WIFI_TYPE_STA)
    {
        LOG_E("this wlan not support sta mode");
        return -RT_ERROR;
    }

    return rthw_wifi_set_powersave_enable(level);
}

static int rthw_wlan_get_powersave(struct rt_wlan_device *wlan)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);

    if (wifi->type != WIFI_TYPE_STA)
    {
        LOG_E("this wlan not support sta mode");
        return -RT_ERROR;
    }

    return rthw_wifi_get_powersave_mode();
}

static rt_err_t rthw_wlan_cfg_promisc(struct rt_wlan_device *wlan, rt_bool_t start)
{
    LOG_E("F:%s L:%d enable %d \n", __FUNCTION__, __LINE__, start);

    if (start)
    {
        // rthw_wifi_monitor_callback_set(rthw_wlan_monitor_callback);
        //rthw_wifi_monitor_enable(1);
        wwd_wifi_set_raw_packet_processor(rthw_wlan_monitor_callback);
        wwd_wifi_enable_monitor_mode();
    }
    else
    {
        // rthw_wifi_monitor_callback_set(NULL);
        // rthw_wifi_monitor_enable(0);
        wwd_wifi_set_raw_packet_processor(NULL);
        wwd_wifi_disable_monitor_mode();
    }
    return RT_EOK;
}

static rt_err_t rthw_wlan_cfg_filter(struct rt_wlan_device *wlan, struct rt_wlan_filter *filter)
{
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    return RT_EOK;
}

static rt_err_t rthw_wlan_set_channel(struct rt_wlan_device *wlan, int channel)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    rthw_wifi_channel_set(wifi->type, channel);

    return RT_EOK;
}

static int rthw_wlan_get_channel(struct rt_wlan_device *wlan)
{
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)(wlan->user_data);

    return rthw_wifi_channel_get(wifi->type);
}

static rt_err_t rthw_wlan_set_country(struct rt_wlan_device *wlan, rt_country_code_t country_code)
{
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    return RT_EOK;
}

static rt_country_code_t rthw_wlan_get_country(struct rt_wlan_device *wlan)
{
    LOG_D("F:%s L:%d\n", __FUNCTION__, __LINE__);
    return RT_EOK;
}

static rt_err_t rthw_wlan_set_mac(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)wlan->user_data;
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    wiced_mac_t wcd_mac;

    memcpy(&wcd_mac.octet[0], mac, MAX_ADDR_LEN);

    if (wwd_wifi_set_mac_address(wcd_mac, wifi->type) == WWD_SUCCESS)
    {
        LOG_E("\n interface %d mac:  0x%02x - 0x%02x - 0x%02x - 0x%02x - 0x%02x - 0x%02x\n", wifi->type, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return RT_EOK;
    }

    return -RT_ERROR;
}

static rt_err_t rthw_wlan_get_mac(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)wlan->user_data;
    wwd_interface_t idx = wifi->type;
    wiced_mac_t wcd_mac;

    LOG_E("F:%s L:%d  idx %d", __FUNCTION__, __LINE__, idx);
    if (mac == RT_NULL)
    {
        return -RT_ERROR;
    }

    if (wwd_wifi_get_mac_address(&wcd_mac, idx) == WWD_SUCCESS)
    {
        memcpy(mac, &wcd_mac.octet[0], MAX_ADDR_LEN);
        LOG_E("\n interface %d mac:  0x%02x - 0x%02x - 0x%02x - 0x%02x - 0x%02x - 0x%02x\n", idx, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return RT_EOK;
    }

    return -RT_ERROR;
}

static int rthw_wlan_recv(struct rt_wlan_device *wlan, void *buff, int len)
{
    LOG_D("F:%s L:%d", __FUNCTION__, __LINE__);
    return RT_EOK;
}

static int rthw_wlan_send(struct rt_wlan_device *wlan, void *buff, int len)
{
    struct cy_wiced_wifi *wifi = (struct cy_wiced_wifi *)wlan->user_data;
    wwd_interface_t idx = wifi->type;
    rt_base_t level;
    struct pbuf *p = buff;

    //  LOG_E("F:%s L:%d pbuf %p  type %d end", __FUNCTION__, __LINE__, buff, wifi->type);

    if (!wifi->connected)
    {
        // LOG_E("Yingbo ::  exit  wifi haven't connected\n", __FUNCTION__, __LINE__, len);
        return RT_EOK;
    }

    pbuf_ref(p);

    wwd_network_send_ethernet_data(p, idx);

    //LOG_E("F:%s L:%d end \n", __FUNCTION__, __LINE__);

    return RT_EOK;
}

void rthw_wlan_netif_rx(int idx, void *buff)
{
    struct cy_wiced_wifi *wifi = rthw_wifi_get_dev(idx);
    struct pbuf *p = buff;


    //  LOG_E("F:%s L:%d idx:%d len:%d, buf %p", __FUNCTION__, __LINE__, idx, p->len, buff);

    if (!wifi->connected)
    {
//      LOG_E("F:%s L:%d wifi don't connected", __FUNCTION__, __LINE__, wifi->connected );
        pbuf_free(p);
        return;
    }

    if (!p)
    {
        LOG_D("Yingbo :: rthw_wlan_netif_rx error ------ Buffer is NULL\n\n");
        return;
    }
    //  pbuf_ref( p );
    rt_wlan_dev_transfer_prot(wifi->wlan, p, p->len);
//   LOG_E("F:%s L:%d end \n", __FUNCTION__, __LINE__);
}


static const struct rt_wlan_dev_ops wiced_wifi_ops =
{
    .wlan_init             =     rthw_wlan_init,
    .wlan_mode             =     rthw_wlan_mode,
    .wlan_scan             =     rthw_wlan_scan,
    .wlan_join             =     rthw_wlan_join,
    .wlan_softap           =     rthw_wlan_softap,
    .wlan_disconnect       =     rthw_wlan_disconnect,
    .wlan_ap_stop          =     rthw_wlan_ap_stop,
    .wlan_ap_deauth        =     rthw_wlan_ap_deauth,
    .wlan_scan_stop        =     rthw_wlan_scan_stop,
    .wlan_get_rssi         =     rthw_wlan_get_rssi,
    .wlan_set_powersave    =     rthw_wlan_set_powersave,
    .wlan_get_powersave    =     rthw_wlan_get_powersave,
    .wlan_cfg_promisc      =     rthw_wlan_cfg_promisc,
    .wlan_cfg_filter       =     rthw_wlan_cfg_filter,
    .wlan_set_channel      =     rthw_wlan_set_channel,
    .wlan_get_channel      =     rthw_wlan_get_channel,
    .wlan_set_country      =     rthw_wlan_set_country,
    .wlan_get_country      =     rthw_wlan_get_country,
    .wlan_set_mac          =     rthw_wlan_set_mac,
    .wlan_get_mac          =     rthw_wlan_get_mac,
    .wlan_recv             =     rthw_wlan_recv,
    .wlan_send             =     rthw_wlan_send,
};

int rthw_wifi_low_init(void)
{
    static rt_int8_t _init_flag = 0;

    if (_init_flag)
    {
        return 1;
    }
    rt_memset(&wiced_ap, 0, sizeof(wiced_ap));
    wiced_ap.type = WIFI_TYPE_AP;
    wiced_ap.wlan = &wiced_wlan_ap;
    wiced_wlan_ap.mode = RT_WLAN_AP;


    rt_memset(&wiced_sta, 0, sizeof(wiced_sta));
    wiced_sta.type = WIFI_TYPE_STA;
    wiced_sta.wlan = &wiced_wlan_sta;
    wiced_wlan_sta.mode = RT_WLAN_STATION;

    _init_flag = 1;

    return 0;
}

INIT_DEVICE_EXPORT(rthw_wifi_low_init);
