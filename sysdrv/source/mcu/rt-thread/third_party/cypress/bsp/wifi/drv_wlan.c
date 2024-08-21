/*
 * File      : drv_wlan.c
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
 * 2018-7-10      tyx          the first version
 */

//#include "wifi_structures.h"
//#include "wifi_constants.h"
//#include <wifi/wifi_util.h>
//#include <wifi/wifi_conf.h>
#include <rtdef.h>
#include <rtthread.h>
#include <rthw.h>
#include <stdint.h>
#include <string.h>
#include <wlan_dev.h>

#include "wwd_constants.h"
#include "wwd_wifi.h"
#include "wwd_management.h"
#include "wwd_structures.h"
#include "board.h"

#include <wlan_dev.h>
#include "drv_wlan.h"
#include "drv_wifi.h"
#include "wlan_mgnt.h"

#include "wiced_utilities.h"

// #define SCAN_WAIT_TIME       (10000)

rthw_mode_t wifi_mode;

rthw_mode_t rthw_wifi_mode_get(void)
{
    return wifi_mode;
}

int rthw_wifi_stop(void)
{
    //turn 1;
    return wwd_management_wifi_off();
}

int rthw_wifi_start(rthw_mode_t mode)
{
    wwd_result_t result;
    result = wwd_management_wifi_on(WICED_COUNTRY_CHINA);
    return result == WWD_SUCCESS ? 0 : -1;
}
extern void rthw_connect_callback(void);


int rthw_wifi_connect(char *ssid, int ssid_len, char *password, int pass_len, rthw_security_t security_type)
{
    int mode;

    mode = rthw_wifi_mode_get();
    if ((mode != RTHW_MODE_STA) && (mode != RTHW_MODE_STA_AP))
    {
        return -1;
    }

    wiced_ssid_t wcd_ssid;
    wcd_ssid.length = ssid_len;
    rt_memcpy(wcd_ssid.value, ssid, ssid_len);

    wwd_wifi_register_link_update_callback(rthw_connect_callback);
    wwd_wifi_join(&wcd_ssid, security_type, password, pass_len, NULL, mode - 1);
    wwd_wifi_set_listen_interval(10, 0);
    wwd_wifi_set_listen_interval(10, 1);
    return 0;
}

int rthw_wifi_connect_bssid(char *bssid, char *ssid, int ssid_len, char *password, int pass_len, rthw_security_t security_type)
{
    int mode;
    //rtw_wifi_setting_t setting;

    mode = rthw_wifi_mode_get();
    if ((mode != RTHW_MODE_STA) && (mode != RTHW_MODE_STA_AP))
    {
        return -1;
    }
    /*  if(wext_get_mode(WLAN0_NAME, &mode) < 0)
      {
          rt_kprintf("L:%d wifi get mode err\n", __LINE__);
          return -1;
      }

      if(wifi_connect_bssid(bssid, ssid, security_type, password, 6, ssid_len, pass_len, -1, NULL) != RTW_SUCCESS)
      {
          rt_kprintf("wifi connect fail\n");
          return -1;
      }
      rt_kprintf("wifi connect success\n");
      rt_kprintf("Show Wi-Fi information\n");
      wifi_get_setting(WLAN0_NAME,&setting);
      wifi_show_setting(WLAN0_NAME,&setting);*/
    return 0;
}

int rthw_wifi_ap_start(char *ssid, char *password, int channel)
{
    int mode = 0, timeout = 20;
    wiced_security_t security_type = WICED_SECURITY_WPA2_AES_PSK;
    char *name = RT_NULL;
    wiced_ssid_t wcd_ssid;


    mode = rthw_wifi_mode_get();


    wifi_mode = RTHW_MODE_AP;
#if 0
    if (mode == RTHW_MODE_AP)
    {
        name = WLAN0_NAME;
    }
    else if (mode == RTHW_MODE_STA_AP)
    {
        name = WLAN1_NAME;
    }
    else
    {
        return -1;
    }


    if (wext_get_mode(name, &mode) < 0)
    {
        rt_kprintf("L:%d wifi get mode err\n", __LINE__);
        return -1;
    }
    if (password == RT_NULL)
    {
        security_type = RTW_SECURITY_OPEN;
    }

    if (wifi_start_ap(ssid, security_type, password, rt_strlen(ssid), rt_strlen(password), 6) != 0)
    {
        rt_kprintf("ERROR: wifi_start_ap failed\n");
        return -1;
    }


    while (1)
    {
        char essid[33];
        if (wext_get_ssid(name, (unsigned char *) essid) > 0)
        {
            if (strcmp((const char *) essid, (const char *)ssid) == 0)
            {
                rt_kprintf("%s started\n", ssid);
                break;
            }
        }
        if (timeout == 0)
        {
            rt_kprintf("Start AP timeout\n");
            return -1;
        }
        rt_thread_delay(1 * RT_TICK_PER_SECOND);
        timeout --;
    }
#else
    rt_kprintf("%s is run ssid:%s password[%p]:%s \n", __FUNCTION__, ssid, password, password);
    if (password == RT_NULL)
    {
        security_type = WICED_SECURITY_OPEN;
    }

//          rt_kprintf("\n=============Yingbo :: wwd_wifi_start_ap  ssid %s  pw %s, chanl %d\n", ssid, password, channel);

    wcd_ssid.length =  rt_strlen(ssid);
    memcpy(wcd_ssid.value, ssid, wcd_ssid.length);
    if (wwd_wifi_start_ap(&wcd_ssid, security_type, password, rt_strlen(password), channel) != WWD_SUCCESS)
    {
        //  WPRINT_APP_INFO(( "Error: wwd_wifi_start_ap failed\n" ));
        //   WICED_POWER_LOGGER( EVENT_PROC_ID_WIFI, EVENT_ID_WIFI_DATA, EVENT_DESC_WIFI_IDLE );
        rt_kprintf("Start AP failed\n");
        return -1;
    }

#endif

    return 0;
}


int rthw_wifi_sta_disconnect(void)
{
    int mode = 0;
    char *name = RT_NULL;

    return wwd_wifi_leave(WWD_STA_INTERFACE);
}

int rthw_wifi_ap_disconnect(void)
{
    int mode = 0;
    char *name = RT_NULL;

    return wwd_wifi_deauth_all_associated_client_stas(WWD_DOT11_RC_UNSPECIFIED, WWD_AP_INTERFACE);
}

int rthw_wifi_rssi_get(void)
{
    int rssi = 0;
    wwd_wifi_get_rssi(&rssi);
    return rssi;
}

void rthw_wifi_channel_set(int interface, int channel)
{
    wwd_wifi_set_channel(interface, channel);
}

int rthw_wifi_channel_get(int interface)
{
    int channel;
    wwd_wifi_get_channel(interface, &channel);
    return channel;
}

#define SCAN_BSSID_LIST_LENGTH  100
static wiced_mac_t  bssid_list[SCAN_BSSID_LIST_LENGTH]; /* List of BSSID (AP MAC addresses) that have been scanned */
static int  bssid_list_length = 0;

static struct rt_semaphore   rthw_scan_results_semaphore;
static wiced_scan_result_t *result_buff_ptr = NULL;
static wiced_scan_result_t *result_ptr = NULL;
static uint16_t     result_buff_write_pos = 0;
static uint16_t     result_buff_read_pos  = 0;
#define RTHW_WIFI_SCANRESULT_BUFF_SIZE   (SCAN_BSSID_LIST_LENGTH)


static rt_wlan_security_t security_map_from_ameba(wiced_security_t security)
{
    wiced_security_t result = SECURITY_OPEN;

    switch (security)
    {
    case WICED_SECURITY_OPEN:
        result = SECURITY_OPEN;
        break;
    case WICED_SECURITY_WEP_PSK:
        result = SECURITY_WEP_PSK;
        break;
    case WICED_SECURITY_WEP_SHARED:
        result = SECURITY_WEP_SHARED;
        break;
    case WICED_SECURITY_WPA_TKIP_PSK:
        result = SECURITY_WPA_TKIP_PSK;
        break;
    case WICED_SECURITY_WPA_AES_PSK:
        result = SECURITY_WPA_AES_PSK;
        break;
    case WICED_SECURITY_WPA2_AES_PSK:
        result = SECURITY_WPA2_AES_PSK;
        break;
    case WICED_SECURITY_WPA2_TKIP_PSK:
        result = SECURITY_WPA2_TKIP_PSK;
        break;
    case WICED_SECURITY_WPA2_MIXED_PSK:
        result = SECURITY_WPA2_MIXED_PSK;
        break;
    case WICED_SECURITY_WPS_OPEN:
        result = SECURITY_WPS_OPEN;
        break;
    case WICED_SECURITY_WPS_SECURE:
        result = SECURITY_WPS_SECURE;
        break;
    default:
        result = -1;
        break;
    }

    return result;
}

static void scan_results_handler(wiced_scan_result_t **result_ptr, void *user_data, wiced_scan_status_t status)
{
    if (result_ptr == NULL)
    {
        /* finished */
        rt_kprintf("[rthw scan cb]: End of scan results\n");
        result_buff_ptr[result_buff_write_pos].channel = 0xff;
        rt_sem_release(&rthw_scan_results_semaphore);
        return;
    }

    wiced_scan_result_t *record = (*result_ptr);

    /* Check the list of BSSID values which have already been printed */
    wiced_mac_t *tmp_mac = bssid_list;
    bssid_list_length = 0;
    while (NULL_MAC(tmp_mac->octet) == WICED_FALSE)
    {
        if (CMP_MAC(tmp_mac->octet, record->BSSID.octet) == WICED_TRUE)
        {
            /* already seen this BSSID */
            return;
        }
        tmp_mac = &bssid_list[bssid_list_length++];

        if (bssid_list_length >= RTHW_WIFI_SCANRESULT_BUFF_SIZE)
            return;
    }


//  rt_kprintf("[rthw scan cb]: %02x:%02x:%02x:%02x:%02x:%02x \n", record->BSSID.octet[0],record->BSSID.octet[1],record->BSSID.octet[2],record->BSSID.octet[3],record->BSSID.octet[4],record->BSSID.octet[5]);

    /* New BSSID - add it to the list */
    memcpy(&tmp_mac->octet, record->BSSID.octet, sizeof(wiced_mac_t));

    /* Add the result to the list and set the pointer for the next result */
    result_buff_write_pos++;
    if (result_buff_write_pos >= RTHW_WIFI_SCANRESULT_BUFF_SIZE)
    {
        result_buff_write_pos = 0;
    }
    *result_ptr = &result_buff_ptr[result_buff_write_pos];
    rt_sem_release(&rthw_scan_results_semaphore);
}

void rthw_scanresult_cb(int event, struct rt_wlan_buff *buff, void *parameter)
{
    if (event == RT_WLAN_EVT_SCAN_REPORT)
    {
//       rt_kprintf("[rthw scancb] free scaninfo :0x%08x\n", buff->data);
        if (buff && buff->data)
            rt_free(buff->data);
    }
}

int rthw_wifi_scan_stop()
{
    wiced_result_t result = (wiced_result_t)wwd_wifi_abort_scan();

    /* If there's a failure; something went wrong. Abort req without active scan looks like a no-op */
    if (WICED_SUCCESS != result)
    {
        return -RT_ERROR;
    }
    else
        return RT_EOK;
}


int rthw_wifi_scan(void *content, struct rt_scan_info *info)
{
    wiced_scan_result_t *record;
    rt_err_t              status;
    int                   rt = 0;

#if 0
    wiced_ssid_t          optional_ssid;
    wiced_mac_t           optional_mac = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint16_t              optional_channel_list;
    wiced_scan_extended_params_t *optional_extended_params;
#endif

    if (result_buff_ptr != NULL)
    {
        rt_free(result_buff_ptr);
        result_buff_ptr = NULL;
    }

    result_buff_ptr = rt_malloc(sizeof(wiced_scan_result_t) * RTHW_WIFI_SCANRESULT_BUFF_SIZE);
    if (result_buff_ptr == NULL)
    {
        rt_kprintf("[rthw scan]: no memory\n");
        rt = -1;
        goto exit;
    }

    status = rt_sem_init(&rthw_scan_results_semaphore, "wscan", 0, RT_IPC_FLAG_FIFO);
    if (status != RT_EOK)
    {
        rt_kprintf("[rthw scan]: fail to create semaphore\n");
        rt = -2;
        goto exit;
    }

    //rt_kprintf("[rthw scan]: registering scan-report event\n");
    status = rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_REPORT, rthw_scanresult_cb, NULL);
    if (status != RT_EOK)
    {
        rt_kprintf("[rthw scan]: fail to register scan-report eventcb\n");
        rt = -2;
        goto exit;
    }

    /* Clear list of BSSID addresses already parsed */
    memset(&bssid_list, 0, sizeof(bssid_list));

    bssid_list_length = 0;
    result_buff_read_pos = 0;
    result_buff_write_pos = 0;
    result_ptr = result_buff_ptr;
    if (info != NULL)
    {
    }

    if (WWD_SUCCESS != wwd_wifi_scan(WICED_SCAN_TYPE_ACTIVE,
                                     WICED_BSS_TYPE_ANY,
                                     NULL, NULL, NULL, NULL,
                                     scan_results_handler,
                                     (wiced_scan_result_t **) &result_ptr,
                                     NULL,
                                     WWD_STA_INTERFACE))
    {
        rt_kprintf("[rthw scan] fail to start scan\n");
        rt = -3;
        goto exit;
    }

    while (rt_sem_take(&rthw_scan_results_semaphore, RT_WAITING_FOREVER) == RT_EOK)
    {
        struct rt_wlan_buff buff;
        struct rt_wlan_info *rthw_scaninfo_ptr = NULL;
        struct rt_wlan_device *wlan = (struct rt_wlan_device *)content;
        int k;

        //rt_kprintf("[rthw scan] wlan:0x%08x\n", wlan);
        record = &result_buff_ptr[result_buff_read_pos];

        /*TODO: change 0xff to a defined flag */
        if (record->channel == (uint8_t) 0xff)
        {
            /* Scan completed */
//           rt_kprintf("[rthw scan] complete  %d\n", result_buff_read_pos);
            rt_wlan_dev_indicate_event_handle(wlan, RT_WLAN_DEV_EVT_SCAN_DONE, RT_NULL);
            break;
        }


        rthw_scaninfo_ptr = (struct rt_wlan_info *)rt_malloc(sizeof(struct rt_wlan_info));
        if (rthw_scaninfo_ptr == NULL)
        {
            break;
        }
        rt_memset(rthw_scaninfo_ptr, 0x0, sizeof(struct rt_wlan_info));

        rthw_scaninfo_ptr->ssid.len = record->SSID.length;
        if (rthw_scaninfo_ptr->ssid.len > RT_WLAN_SSID_MAX_LENGTH)
            rthw_scaninfo_ptr->ssid.len = RT_WLAN_SSID_MAX_LENGTH;
        rt_memset((char *)&rthw_scaninfo_ptr->ssid.val[0], 0, RT_WLAN_SSID_MAX_LENGTH);
        for (k = 0; k < (int)record->SSID.length; k++)
            rthw_scaninfo_ptr->ssid.val[k] = record->SSID.value[k];
        //rt_memcpy( (char *)&rthw_scaninfo_ptr->ssid.val[0], record->SSID.value, rthw_scaninfo_ptr->ssid.len);
        rthw_scaninfo_ptr->ssid.val[RT_WLAN_SSID_MAX_LENGTH] = 0;

        for (k = 0; k < 6; k++)
            rthw_scaninfo_ptr->bssid[k] = record->BSSID.octet[k];
        //rt_memcpy( (char *)&rthw_scaninfo_ptr->bssid[0], record->BSSID.octet, 6);
        rthw_scaninfo_ptr->rssi = record->signal_strength;
        rthw_scaninfo_ptr->datarate = record->max_data_rate * 1000;
        rthw_scaninfo_ptr->security = security_map_from_ameba(record->security);
        rthw_scaninfo_ptr->band = (record->band == WICED_802_11_BAND_5GHZ ? RTHW_802_11_BAND_2_4GHZ : RTHW_802_11_BAND_5GHZ);
        rthw_scaninfo_ptr->channel = record->channel;

        buff.data = rthw_scaninfo_ptr;
        buff.len = sizeof(struct rt_wlan_info);
//        rt_kprintf("[rthw scan] alloced rthw_scan_info_ptr:0x%08x \n", rthw_scaninfo_ptr);
        rt_wlan_dev_indicate_event_handle(wlan, RT_WLAN_DEV_EVT_SCAN_REPORT, &buff);

        result_buff_read_pos++;
        if (result_buff_read_pos >= (uint16_t) RTHW_WIFI_SCANRESULT_BUFF_SIZE)
        {
            result_buff_read_pos = 0;
        }

    }

    /* Done! */
    rt_kprintf(("\nEnd of scan\n"));

exit:
    if (result_buff_ptr != NULL)
        rt_free(result_buff_ptr);
    result_buff_ptr = NULL;

    rt_sem_detach(&rthw_scan_results_semaphore);
    status = rt_wlan_unregister_event_handler(RT_WLAN_EVT_SCAN_REPORT);
    return rt;
}



void rthw_wifi_monitor_enable(int enable)
{
    if (enable)
    {
        wwd_wifi_enable_monitor_mode();
    }
    else
    {
        wwd_wifi_disable_monitor_mode();
    }
}


int rthw_wifi_set_powersave_enable(int enable)
{
    if (enable)
    {
        if (wwd_wifi_enable_powersave() != WWD_SUCCESS)
        {
            return -RT_ERROR;
        }
    }
    else
    {
        if (wwd_wifi_disable_powersave() != WWD_SUCCESS)
        {
            return -RT_ERROR;
        }
    }
    return RT_EOK;
}

int rthw_wifi_get_powersave_mode()
{

    return wiced_wifi_get_powersave_mode();
}


