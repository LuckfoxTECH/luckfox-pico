/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-15 14:21:20
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:58:12
 * @LastEditors: Do not edit
 * @Description:
 */

#include "pcba_app.h"
#include "pcba_def.h"
#include "pcba_public.h"

#ifdef RT_USING_PCBA_TEST
#define DBG_SECTION_NAME    "PCBA_APP"
#define DBG_LEVEL           DBG_INFO

#include "rtdbg.h"

struct pcba_app_st g_pcba;

static void pcba_audio_files_refresh(uint32_t *cur_file, uint32_t *total_file)
{
    static uint32_t last_refresh = 0;

    /* Refresh per seconds */
    if (last_refresh && (HAL_GetTick() - last_refresh < 1000))
        return;

    last_refresh = HAL_GetTick();
    *total_file = scan_audio(RECORD_DIR_PATH);
    if (*cur_file == 0 && *total_file != 0)
        *cur_file = 1;
    if (*cur_file > *total_file)
        *cur_file = *total_file;
}

static void info_refresh(void *arg)
{
    int value;
    uint32_t cur_file, total_file;
    struct pcba_app_msg send_msg;

    send_msg.type = TYPE_REFRESH;
    while (g_pcba.state == PCBA_TEST_STATE_RUN)
    {
        rt_thread_mdelay(200);
        send_msg.value = REFRESH_DISP_TICK;

        /* Get local time */
        g_pcba.time = time(RT_NULL);
        g_pcba.date = localtime(&g_pcba.time);
        LOG_D("%d-%d-%d %d:%d:%d\n",
              g_pcba.date->tm_year + 1900,
              g_pcba.date->tm_mon + 1,
              g_pcba.date->tm_mday,
              g_pcba.date->tm_hour,
              g_pcba.date->tm_min,
              g_pcba.date->tm_sec);

        if (g_pcba.now_page == MENU_RTC_TEST_PAGE)
            send_msg.value |= REFRESH_DATE;

        /* Get number of files */
        cur_file = g_pcba.file.cur_file;
        total_file = g_pcba.file.total_file;
        pcba_audio_files_refresh(&cur_file, &total_file);

        /* Get audio info */
        if (g_pcba.now_page == MENU_PHONE_SPEAK_TEST_PAGE)
        {
            DBG_HERE
            if (cur_file != g_pcba.file.cur_file ||
                total_file != g_pcba.file.total_file)
            {
                DBG_HERE
                value = 0;
                LOG_I("File changed:%d->%d", g_pcba.file.total_file, total_file);
                LOG_I("Cur file changed:%d->%d", g_pcba.file.cur_file, cur_file);
                /* Cureent file changed or total file decrease */
                if (g_pcba.file.cur_file != cur_file || g_pcba.file.total_file > total_file)
                {
                    if (get_audio(RECORD_DIR_PATH, g_pcba.file.cur_file_name, cur_file) == RT_EOK)
                    {
                        g_pcba.player_total_time = pcba_player_get_total_time(g_pcba.file.cur_file_name);
                    }
                    else
                    {
                        g_pcba.player_total_time = 0;
                        cur_file = 0;
                    }
                }
                //if (g_pcba.now_page == MENU_MAIN_PAGE)
                    send_msg.value |= REFRESH_FILE_INFO | REFRESH_MEDIA_INFO;

                g_pcba.file.cur_file = cur_file;
                g_pcba.file.total_file = total_file;
                //save_app_info();
            }
            else
            {
                DBG_HERE
                value = player_get_cur_time(g_pcba.player);
            }

            if (g_pcba.player_cur_time != value)
            {
                g_pcba.player_cur_time = value;
                // if (g_pcba.now_page == MENU_MAIN_PAGE)
                    send_msg.value |= REFRESH_MEDIA_INFO;
            }
        }

        if (g_pcba.now_page == MENU_RECORD_TEST_PAGE)
        {
            DBG_HERE
            value = recorder_get_cur_time(g_pcba.recorder);
            if (g_pcba.record_time != value)
            {
                g_pcba.record_time = value;
                send_msg.value |= REFRESH_MEDIA_INFO;
            }
        }

        rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
    }
}

void pcba_app(void *args)
{
    struct pcba_app_msg rcv_msg;
    int ret;

    // memset((void *)&g_pcba, 0x0, sizeof(struct pcba_app_info));
    // get_app_info();

    // g_pcba.state = PCBA_APP_STATE_INIT;
    g_pcba.main_mq = rt_mq_create("pcba_main", sizeof(struct pcba_app_msg), 3, RT_IPC_FLAG_FIFO);
    g_pcba.display_mq = rt_mq_create("pcba_disp", sizeof(struct pcba_app_msg), 10, RT_IPC_FLAG_FIFO);
    g_pcba.bl_device = rt_device_find("backlight");
    g_pcba.backlight = 1;

    pcba_display_init();
    pcba_player_init();
    pcba_recorder_init();
    pcba_key_init();

#ifdef RT_CYPRESS_BT
    extern void bt_audio_enable_bluetooth(void);
    bt_audio_enable_bluetooth();
#endif
    // save_app_info();

    rt_thread_mdelay(1000);
    g_pcba.time = time(RT_NULL);
    g_pcba.date = localtime(&g_pcba.time);

    display_pcba_page(MENU_MAIN_PAGE);
    g_pcba.state = PCBA_TEST_STATE_RUN;

    {
        rt_thread_t tid;
        tid = rt_thread_create("pcba_info",
                               info_refresh,
                               RT_NULL,
                               PCBA_APP_DISP_TASK_STACK_SIZE,
                               PCBA_APP_DISP_TASK_PRIORITY,
                               10);
        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }

    while (1)
    {
        /* try to get msg */
        ret = rt_mq_recv(g_pcba.main_mq, (void *)&rcv_msg, sizeof(struct pcba_app_msg), RT_WAITING_FOREVER);
        if (ret == RT_EOK)
        {
            LOG_I("main rev msg:%d %d\n", rcv_msg.type, rcv_msg.value);
            switch (rcv_msg.type)
            {
            default:
                break;
            }
        }
        rt_thread_mdelay(100);
    }
}

int pcba_app_init(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("pcba_test_app", pcba_app, NULL, PCBA_APP_MAIN_TASK_STACK_SIZE, PCBA_APP_MAIN_TASK_PRIORITY, 10);
    if (tid)
        rt_thread_startup(tid);

    return RT_EOK;
}

INIT_APP_EXPORT(pcba_app_init);

#endif
