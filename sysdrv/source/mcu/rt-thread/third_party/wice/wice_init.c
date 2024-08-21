/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    wice_init.c
  * @author  aaron sun
  * @version V0.1
  * @date    25-JUN-2019
  * @brief   wice for rtt wlan struct interface
  *
  ******************************************************************************
  */
#include "rtthread.h"
#include "rtdevice.h"
#include "hal_base.h"
#include "wice_rtt_interface.h"

#ifndef RT_USING_WICE_MFG_TEST
static void usage(void)
{
    rt_kprintf("wice command\n\n");
    rt_kprintf("wice [-c | -d | -a | -s | -e | -q]\n\n");
    rt_kprintf("    -c         create wice driver\n");
    rt_kprintf("    -d         delete wice driver\n");
    rt_kprintf("    -a         auto create and delete wice\n");
    rt_kprintf("    -q         quit create and delete wice\n");
    rt_kprintf("    -s         start airkiss\n");
    rt_kprintf("    -e         end airkiss\n");
    rt_kprintf("    -g         get airkiss result\n");
    rt_kprintf("Examples:\n");
    rt_kprintf("    wice                  start wice\n");
    rt_kprintf("    wice -s               start wice\n");
}

int cnt;
int time_delay;
int force_stop;
static void wice_auto_test(void)
{
    while (cnt--)
    {
        if (force_stop)
        {
            break;
        }
        rt_thread_startup(rt_thread_create("wifi", (void (*)(void *))wice_create, NULL, 8192, 10, 0));
        rt_thread_delay(time_delay);
        rt_thread_startup(rt_thread_create("wifi", (void (*)(void *))wice_delete, NULL, 8192, 10, 0));
        rt_thread_delay(time_delay);
    }
}

int wice_cmd(int argc, char *argv[])
{
    if (argc == 1)
    {
        rt_thread_startup(rt_thread_create("wifi", (void (*)(void *))wice_create, NULL, 8192, 10, 0));
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-c") == 0)
        {
            rt_thread_startup(rt_thread_create("wifi", (void (*)(void *))wice_create, NULL, 8192, 10, 0));
        }
        else if (strcmp(argv[1], "-d") == 0)
        {
            rt_thread_startup(rt_thread_create("wifi", (void (*)(void *))wice_delete, NULL, 8192, 10, 0));
        }
        else if (strcmp(argv[1], "-a") == 0)
        {
            cnt = -1;
            time_delay = 10000;
            force_stop = 0;
            rt_thread_startup(rt_thread_create("wifi", (void (*)(void *))wice_auto_test, NULL, 8192, 10, 0));
        }
        else if (strcmp(argv[1], "-q") == 0)
        {
            force_stop = 1;
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            rk_wifi_smartconfig();
        }
        else if (strcmp(argv[1], "-e") == 0)
        {
            rk_smartconfig_stop();
        }
        else if (strcmp(argv[1], "-g") == 0)
        {
            WIFI_AP *ap;
            ap = rk_wifi_smartconfig_get();
            if (ap)
            {
                ap->ap_ssid_value[ap->ap_ssid_len] = 0;
                rt_kprintf("ssid: %s\n", ap->ap_ssid_value);
                ap->security_key[ap->security_key_length] = 0;
                rt_kprintf("password: %s\n", ap->security_key);
            }
        }
        else if (strcmp(argv[1], "-h") == 0)
        {
            usage();
        }
        else
        {
            usage();
        }
    }
    else
    {
        usage();
    }
    return 0;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(wice_cmd, wice, start wice wlan driver);
#endif
#endif

#ifdef RT_USING_WICE_MFG_TEST
extern void mfgcmd(char *pstr);
int wl(int argc, char *argv[])
{
    int len;

    if (argc == 1)
    {
        return 0;
    }

    char *buf = malloc(1024);
    memset(buf, 0, 1024);

    len = 0;
    for (int i = 1; i < argc; i++)
    {
        memcpy(buf + len, argv[i], strlen(argv[i]));
        len += strlen(argv[i]);
        buf[len] = 0x20;
        len++;
    }
    buf[len - 1] = 0;
    mfgcmd(buf + 3);
    return 0;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(wl, wl, rkos cmd wifi factory test);
#endif
#endif