/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-04     Jair Wu      First version
 *
 */

#include "recpen.h"
#ifdef RT_USING_RECORDING_PEN_APP
#define DBG_SECTION_NAME    "RECPEN"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"
#include "setting_backlight.h"
#include "drv_usbd.h"

struct recpen_app_info g_app_info;
static struct rt_event rp_event;
static int g_power_off_check = 0;
static int g_pPage = MENU_NONE_PAGE;

static const char format_prompt[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "格式化 U盘？",
    "Format Udisk?"
};

static const char poweroff_prompt[RECPEN_APP_LANG_MAX][MAX_LABEL_LEN] =
{
    "是否关机？",
    "Power OFF?"
};

#ifdef RT_USING_PMIC
#include "drv_pmic.h"

void vbus_in_hook(void)
{
    rt_event_send(&rp_event, EVENT_VBUS_PLUGIN);
}

void vbus_out_hook(void)
{
    rt_event_send(&rp_event, EVENT_VBUS_PLUGOUT);
}

void power_key_raise_hook(void)
{
    if (g_power_off_check == 0)
        rt_event_send(&rp_event, EVENT_POWER_KEY_RAISE);
}

void power_key_lpress_hook(void)
{
    rt_event_send(&rp_event, EVENT_POWER_KEY_LPRESS);
}
#endif

static void power_off(int ret)
{
    if (ret == DIALOG_BOX_CONFIRM)
    {
        recpen_recorder_stop();
        recpen_player_stop();
        save_app_info();
        pmic_power_off();
    }

    g_power_off_check = 0;
}

static void event_detect(void *arg)
{
    struct recpen_app_msg send_msg;
    rt_uint32_t event;

    while (1)
    {
        rt_event_recv(&rp_event,
                      (EVENT_VBUS_PLUGIN | EVENT_VBUS_PLUGOUT |
                       EVENT_POWER_KEY_RAISE | EVENT_POWER_KEY_FALL |
                       EVENT_POWER_KEY_LPRESS),
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &event);
        if (event & EVENT_VBUS_PLUGIN)
        {
            LOG_I("vbus plug in");
            g_app_info.charging = 1;
            if (g_app_info.bl_timer)
                rt_timer_start(g_app_info.bl_timer);
            if (g_app_info.backlight == 0)
                g_app_info.backlight = 1;
            if (usb_get_bc_info() != PCD_BCD_DEDICATED_CHARGING_PORT)
            {
                if (g_app_info.now_page == MENU_REC_PAGE)
                {
                    send_msg.type = TYPE_KEY_PRESS;
                    send_msg.value = KEY_RECORD;
                    rt_mq_send(g_app_info.display_mq, (void *)&send_msg,
                               sizeof(struct recpen_app_msg));
                }
                recpen_player_stop();
                dialog_box_key(TYPE_KEY_PRESS, KEY_CANCEL);
                g_pPage = g_app_info.now_page;
                display_page(MENU_UDISK_PAGE);
            }
        }
        if (event & EVENT_VBUS_PLUGOUT)
        {
            LOG_I("vbus plug out");
            g_app_info.charging = 0;
            if (g_app_info.bl_timer)
                rt_timer_start(g_app_info.bl_timer);
            if (g_app_info.backlight == 0)
                g_app_info.backlight = 1;

            if (g_app_info.now_page == MENU_UDISK_PAGE)
            {
                display_page(g_pPage);
                g_pPage = MENU_NONE_PAGE;
            }
        }
        if (event & EVENT_POWER_KEY_RAISE)
        {
            if (g_app_info.backlight)
            {
                g_app_info.backlight = 0;
                if (g_app_info.bl_timer)
                    rt_timer_stop(g_app_info.bl_timer);
            }
            else
            {
                g_app_info.backlight = 1;
                if (g_app_info.bl_timer)
                    rt_timer_start(g_app_info.bl_timer);
            }
        }
        if (event & EVENT_POWER_KEY_LPRESS)
        {
            g_power_off_check = 1;
            dialog_box_init(poweroff_prompt[g_app_info.language], power_off);
        }
    }
}

static void files_refresh(uint32_t *cur_file, uint32_t *total_file)
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

static int battery_refresh(void)
{
    static uint32_t last_refresh = 0;
    int value;

    if (usb_get_bc_info() != PCD_BCD_DEFAULT_STATE)
    {
        g_app_info.charging = 1;
    }
    else
    {
        g_app_info.charging = 0;
    }
    value = g_app_info.bat;
    /* Refresh per second */
    if (!g_app_info.charging && last_refresh && (HAL_GetTick() - last_refresh < 1000))
        return value;
    last_refresh = HAL_GetTick();
    if (g_app_info.charging && value < 100)
    {
        value = g_app_info.bat + 25; // show charging animotion
        if (value > 100)
            value = 100;
    }
    else
    {
        if (g_app_info.fuel_gauge)
        {
            rt_device_control(g_app_info.fuel_gauge, RT_DEVICE_CTRL_FG_GET, &g_app_info.fg_status);
            LOG_D("soc %d fcc %ldmAh current %ld voltage %ldmV present %d\n",
                  g_app_info.fg_status.soc,
                  g_app_info.fg_status.fcc,
                  g_app_info.fg_status.current,
                  g_app_info.fg_status.voltage,
                  g_app_info.fg_status.present);
            /* Check if battery exist */
            if (g_app_info.fg_status.present)
                value = g_app_info.fg_status.soc;
            else
                value = 0;
        }
        else
        {
            value = 0;
        }
    }

    return value;
}

static void info_refresh(void *arg)
{
    struct recpen_app_msg send_msg;
    uint32_t cur_file, total_file;
    uint32_t value = 0;

    send_msg.type = TYPE_REFRESH;
    while (g_app_info.state >= RECPEN_APP_STATE_IDLE)
    {
        rt_thread_mdelay(200);
        send_msg.value = REFRESH_DISP_TICK;

        /* Get local time */
        g_app_info.time = time(RT_NULL);
        g_app_info.date = localtime(&g_app_info.time);
        LOG_D("%d-%d-%d %d:%d:%d\n",
              g_app_info.date->tm_year + 1900,
              g_app_info.date->tm_mon + 1,
              g_app_info.date->tm_mday,
              g_app_info.date->tm_hour,
              g_app_info.date->tm_min,
              g_app_info.date->tm_sec);
        if (g_app_info.now_page == MENU_MAIN_PAGE)
            send_msg.value |= REFRESH_DATE;

        /* Get bettery status */
        value = battery_refresh();
        if (g_app_info.bat != value)
        {
            g_app_info.bat = value;
            if (g_app_info.now_page == MENU_MAIN_PAGE)
                send_msg.value |= REFRESH_BAT;
        }

        if (g_app_info.now_page == MENU_UDISK_PAGE)
        {
            rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
            continue;
        }

        /* Get number of files */
        cur_file = g_app_info.file.cur_file;
        total_file = g_app_info.file.total_file;
        files_refresh(&cur_file, &total_file);

        /* Check player status */
        if (g_app_info.player_state == PLAYER_STATE_RUNNING)
            g_app_info.state = RECPEN_APP_STATE_BUSY;
        else if (g_app_info.now_page == MENU_MAIN_PAGE)
            g_app_info.state = RECPEN_APP_STATE_IDLE;

        /* Get audio info */
        if (g_app_info.now_page != MENU_REC_PAGE)
        {
            if (cur_file != g_app_info.file.cur_file ||
                    total_file != g_app_info.file.total_file)
            {
                value = 0;
                LOG_I("File changed:%d->%d", g_app_info.file.total_file, total_file);
                LOG_I("Cur file changed:%d->%d", g_app_info.file.cur_file, cur_file);
                /* Cureent file changed or total file decrease */
                if (g_app_info.file.cur_file != cur_file || g_app_info.file.total_file > total_file)
                {
                    if (get_audio(RECORD_DIR_PATH, g_app_info.file.cur_file_name, cur_file) == RT_EOK)
                    {
                        g_app_info.player_total_time = recpen_player_get_total_time(g_app_info.file.cur_file_name);
                    }
                    else
                    {
                        g_app_info.player_total_time = 0;
                        cur_file = 0;
                    }
                }
                if (g_app_info.now_page == MENU_MAIN_PAGE)
                    send_msg.value |= REFRESH_FILE_INFO | REFRESH_MEDIA_INFO;

                g_app_info.file.cur_file = cur_file;
                g_app_info.file.total_file = total_file;
                save_app_info();
            }
            else
            {
                value = player_get_cur_time(g_app_info.player);
            }
            if (g_app_info.player_cur_time != value)
            {
                g_app_info.player_cur_time = value;
                if (g_app_info.now_page == MENU_MAIN_PAGE)
                    send_msg.value |= REFRESH_MEDIA_INFO;
            }
        }

        if (g_app_info.now_page == MENU_REC_PAGE)
        {
            g_app_info.state = RECPEN_APP_STATE_BUSY;
            value = recorder_get_cur_time(g_app_info.recorder);
            if (g_app_info.record_time != value)
            {
                g_app_info.record_time = value;
                send_msg.value |= REFRESH_MEDIA_INFO;
            }
        }

        rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
    }
}

void recpen_backlight_timeout(void *arg)
{
    g_app_info.backlight = 0;
}

static void format_sd(int ret)
{
    if (ret == DIALOG_BOX_CONFIRM)
    {
        dfs_mkfs("elm", "sd0");
        LOG_I("format sd0 done");
#ifdef RT_USING_RESET
        LOG_I("Reboot");
        rt_hw_cpu_reset();
#else
        struct recpen_app_msg send_msg;
        /* Set reboot prompt */
        send_msg.type = TYPE_REBOOT;
        rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
        while (1)
            rt_thread_mdelay(100);
#endif
    }
    rt_sem_release(g_app_info.wait_sem);
}

void recpen_app(void *args)
{
    struct recpen_app_msg rcv_msg;
    struct recpen_app_msg send_msg;
    int32_t time_out;
    int ret;

    memset((void *)&g_app_info, 0x0, sizeof(struct recpen_app_info));
    get_app_info();
    g_app_info.state = RECPEN_APP_STATE_INIT;
    g_app_info.main_mq = rt_mq_create("rpmain", sizeof(struct recpen_app_msg), 3, RT_IPC_FLAG_FIFO);
    g_app_info.display_mq = rt_mq_create("rpdisplay", sizeof(struct recpen_app_msg), 10, RT_IPC_FLAG_FIFO);
    g_app_info.wait_sem = rt_sem_create("rp_wait", 0, RT_IPC_FLAG_PRIO);
    g_app_info.bl_device = rt_device_find("backlight");
    g_app_info.backlight = 1;
    recpen_display_init();
    recpen_key_init();
#ifdef RT_USING_PMIC
    pmic_plug_in_sethook(vbus_in_hook);
    pmic_plug_out_sethook(vbus_out_hook);
    pmic_poweron_rise_sethook(power_key_raise_hook);
    pmic_poweron_long_sethook(power_key_lpress_hook);
#endif
    {
        rt_event_init(&rp_event, "rp_event", RT_IPC_FLAG_FIFO);
        rt_thread_t tid;
        tid = rt_thread_create("rp_event",
                               event_detect,
                               RT_NULL,
                               1024,
                               16,
                               10);
        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }

    ret = rootfs_check();
    switch (usb_get_bc_info())
    {
    case PCD_BCD_DEFAULT_STATE:
        g_app_info.charging = 0;
        break;
    case PCD_BCD_STD_DOWNSTREAM_PORT:
        g_app_info.charging = 1;
        g_pPage = MENU_MAIN_PAGE;
        display_page(MENU_UDISK_PAGE);
        time_out = 100;
        while (g_app_info.now_page != MENU_UDISK_PAGE)
        {
            rt_thread_mdelay(20);
            time_out--;
            if (time_out <= 0)
                break;
        }
        break;
    case PCD_BCD_DEDICATED_CHARGING_PORT:
        g_app_info.charging = 1;
        if (ret & SD0_NO_MOUNTED)
        {
            dialog_box_init(format_prompt[g_app_info.language], format_sd);
            send_msg.type = TYPE_REFRESH;
            send_msg.value = REFRESH_DISP_TICK;
            rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
        }
        break;
    default:
        g_app_info.charging = 1;
        break;
    }

    recpen_player_init();
    recpen_recorder_init();

    g_app_info.time = time(RT_NULL);
    g_app_info.date = localtime(&g_app_info.time);
    g_app_info.file.total_file = scan_audio(RECORD_DIR_PATH);
    if (g_app_info.file.cur_file == 0)
        g_app_info.file.cur_file = 1;
    else if (g_app_info.file.cur_file > g_app_info.file.total_file)
        g_app_info.file.cur_file = g_app_info.file.total_file;
    if (get_audio(RECORD_DIR_PATH, g_app_info.file.cur_file_name, g_app_info.file.cur_file) == RT_EOK)
    {
        g_app_info.player_total_time = recpen_player_get_total_time(g_app_info.file.cur_file_name);
        g_app_info.player_cur_time = 0;
    }
    else
    {
        g_app_info.file.cur_file = 0;
        g_app_info.player_total_time = 0;
        g_app_info.player_cur_time = 0;
    }
    save_app_info();
    g_app_info.fuel_gauge = rt_device_find(FUEL_GAUGE_DEVICE_NAME);
    if (!g_app_info.fuel_gauge)
    {
        LOG_W("No fuel gauge\n");
        g_app_info.bat = 0;
    }
    else
    {
        rt_device_control(g_app_info.fuel_gauge, RT_DEVICE_CTRL_FG_GET, &g_app_info.fg_status);
        g_app_info.bat = g_app_info.fg_status.soc;
    }
    if (g_app_info.bl_device)
    {
        g_app_info.backlight_timeout = get_bl_timeout();
        g_app_info.bl_timer = rt_timer_create("bl_timer",
                                              recpen_backlight_timeout,
                                              NULL,
                                              g_app_info.backlight_timeout * 1000, // s to ms
                                              RT_TIMER_FLAG_ONE_SHOT);
        if (g_app_info.bl_timer)
            rt_timer_start(g_app_info.bl_timer);
        else
            LOG_W("Create bl_timer failed");
    }

    if (g_app_info.now_page == MENU_WELCOME_PAGE)
        display_page(MENU_MAIN_PAGE);
    g_app_info.state = RECPEN_APP_STATE_IDLE;

    {
        rt_thread_t tid;
        tid = rt_thread_create("rp_info",
                               info_refresh,
                               RT_NULL,
                               2048,
                               20,
                               10);
        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }
    while (1)
    {
        /* try to get msg */
        ret = rt_mq_recv(g_app_info.main_mq, (void *)&rcv_msg, sizeof(struct recpen_app_msg), RT_WAITING_FOREVER);
        if (ret == RT_EOK)
        {
            LOG_I("main rev msg:%d %d\n", rcv_msg.type, rcv_msg.value);
            switch (rcv_msg.type)
            {
            case TYPE_PLAYBACK_DONE:
                if (recpen_next_file() == RT_EOK)
                {
                    send_msg.type = TYPE_KEY_PRESS;
                    send_msg.value = KEY_PLAY;
                    ret = rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
                    RT_ASSERT(ret == RT_EOK);
                }
                break;
            default:
                break;
            }
        }
        rt_thread_mdelay(100);
    }
}

int recpen_app_init(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("rp_main", recpen_app, NULL, RECPEN_MAIN_TASK_STACK_SIZE, RECPEN_MAIN_TASK_PRIORITY, 10);
    if (tid)
        rt_thread_startup(tid);

    return RT_EOK;
}

INIT_APP_EXPORT(recpen_app_init);
#endif

