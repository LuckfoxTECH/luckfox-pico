/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/netlink.h>
#include "ssv_smartlink.h"
#include "qqlink-lib-mipsel/include/TXWifisync.h"
static uint8_t gBuf[MAX_PAYLOAD]={0};
static uint32_t gBufLen=0;
static char _sz_ssid[QLMAX_SSID_LEN];
static char _sz_password[QLMAX_PSWD_LEN];
static char *ssv_wpa_conf_file = "%s/conf/wpa_supplicant.conf";
static char *ssv_enter_qqlink_mode = "cd %s/scripts/ && ./startsmartlink.sh";
static char *ssv_start_sta_mode = "cd %s/         && ./sta.sh";
static uint32_t gChan=SSV_MIN_CHANNEL;
void on_wifi_sync_notify(tx_wifi_sync_param *pwifi_sync_param, void *puserdata)
{
    printf("received said: %s\n", pwifi_sync_param->sz_ssid);
    printf("received password: %s\n", pwifi_sync_param->sz_password);
    strcpy(_sz_ssid, pwifi_sync_param->sz_ssid);
    strcpy(_sz_password, pwifi_sync_param->sz_password);
}
static void _ssv_qqlink_setchannel_callback(int signum)
{
    (void)signum;
    ssv_smartlink_set_channel(gChan);
    wifi_sync_notify_hop(gChan);
    gChan++;
    if (gChan > SSV_MAX_CHANNEL)
    {
        gChan = SSV_MIN_CHANNEL;
    }
}
static int _ssv_qqlink_enable_setchannel_timer(void)
{
    struct itimerval t;
    t.it_interval.tv_usec = 100000;
    t.it_interval.tv_sec = 0;
    t.it_value.tv_usec = 100000;
    t.it_value.tv_sec = 0;
    if (setitimer(ITIMER_REAL, &t, NULL) < 0)
    {
        printf("%s\n", strerror(errno));
        printf("setitimer error!\n");
        return -1;
    }
    signal(SIGALRM, _ssv_qqlink_setchannel_callback);
    return 0;
}
static int _ssv_qqlink_disable_setchannel_timer(void)
{
    struct itimerval t;
    getitimer(ITIMER_REAL, &t );
    t.it_interval.tv_usec = 0;
    t.it_interval.tv_sec = 0;
    t.it_value.tv_usec = 0;
    t.it_value.tv_sec = 0;
    if (setitimer(ITIMER_REAL, &t, NULL) < 0)
    {
        printf("%s\n", strerror(errno));
        printf("setitimer error!\n");
        return -1;
    }
    return 0;
}
static void _ssv_sig_int(int signum)
{
    (void)ssv_smartlink_stop();
    return;
}
static int _ssv_qqlink_write_wpa_config_file(char *pFileName, char *pSSID, char *pPWD)
{
    FILE *fptr=NULL;
    if (!pFileName || !pSSID || !pPWD)
    {
        printf("Parameter error!\n");
        return -1;
    }
    fptr = fopen(pFileName, "w");
    if (fptr == NULL)
    {
        printf("Open %s failed: %s\n", pFileName, strerror(errno));
        return -2;
    }
    fprintf(fptr, "ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fptr, "ap_scan=1\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "network={\n");
    fprintf(fptr, "\tssid=\"%s\"\n", pSSID);
    fprintf(fptr, "\tpsk=\"%s\"\n", pPWD);
    fprintf(fptr, "}\n");
    fclose(fptr);
    printf("Write %s success.\n", pFileName);
    return 0;
}
int main(int argc, char *argv[])
{
    int ret=-1;
    int channel=0;
    static int locked=0;
    char *file_path_getcwd=NULL;
    const char *key="Wechatiothardwav";
    char cmdBuf[160]={0};
    memset(_sz_ssid, 0, QLMAX_SSID_LEN);
    memset(_sz_password, 0, QLMAX_PSWD_LEN);
    file_path_getcwd = (char *)malloc(80);
    getcwd(file_path_getcwd, 80);
    printf("==Current Path: %s==\n", file_path_getcwd);
 sprintf(cmdBuf, ssv_enter_qqlink_mode, file_path_getcwd);
    ret = system(cmdBuf);
    if (ret != 0)
    {
        if (file_path_getcwd)
        {
            free(file_path_getcwd);
        }
        return ret;
    }
    ret = init_wifi_sync(on_wifi_sync_notify, "SZSSV01234567890", "SSV1234567890");
    if (ret != QLERROR_INIT_SUCCESS)
    {
        printf("qqlink init failed\n");
        goto out;
    }
    printf("%s\n", ssv_smartlink_version());
    ret = ssv_smartlink_start();
    if (ret < 0)
    {
        printf("ssv_smartlink_start error: %d\n", ret);
        goto out;
    }
    ret = ssv_smartlink_set_promisc(1);
    if (ret < 0)
    {
        printf("ssv_smartlink_set_promisc error: %d\n", ret);
        goto out;
    }
    ret = _ssv_qqlink_enable_setchannel_timer();
    if (ret < 0)
    {
        goto out;
    }
    signal(SIGINT, _ssv_sig_int);
    while (1)
    {
        gBufLen = 0;
        memset(gBuf, 0, sizeof(gBuf));
        ret = ssv_smartlink_recv_packet(gBuf, &gBufLen);
        if (ret < 0)
        {
            printf("ssv_smartlink_recv_packet error: %d\n", ret);
            goto out;
        }
        ret = fill_80211_frame(gBuf, gBufLen, 0, &channel);
        if (ret == QLERROR_SUCCESS)
        {
            ret = ssv_smartlink_set_promisc(0);
            if (ret < 0)
            {
                goto out;
            }
            break;
        }
        else if (ret == QLERROR_LOCK)
        {
            if (!locked)
            {
                locked = 1;
                ret = _ssv_qqlink_disable_setchannel_timer();
                if (ret < 0)
                {
                    goto out;
                }
                gChan = gChan - 1;
                if (gChan < SSV_MIN_CHANNEL)
                {
                    gChan = SSV_MIN_CHANNEL;
                }
                printf("Channel locked to %d!\n", gChan);
            }
        }
        else if (ret == QLERROR_HOP)
        {
        }
        else
        {
        }
    }
    ret = 0;
out:
    (void)ssv_smartlink_stop();
    if (ret == 0)
    {
        sprintf(cmdBuf, ssv_wpa_conf_file, file_path_getcwd);
        if (0 == _ssv_qqlink_write_wpa_config_file(cmdBuf, _sz_ssid, _sz_password))
        {
            char buf[64];
            printf("Config file: %s\n", cmdBuf);
            printf("----------------------------------------\n");
            sprintf(buf, "cat %s", cmdBuf);
            system(buf);
            printf("----------------------------------------\n");
            sprintf(cmdBuf, ssv_start_sta_mode, file_path_getcwd);
            system(cmdBuf);
        }
    }
    if (file_path_getcwd)
    {
        free(file_path_getcwd);
    }
    return ret;
}
