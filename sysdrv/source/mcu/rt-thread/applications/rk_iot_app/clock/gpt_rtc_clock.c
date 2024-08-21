/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */
#include "rtthread.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include "gpt_rtc_clock.h"
#include "sysinfo.h"
#include "hal_base.h"
#include "hal_cru.h"
#include "wlan_mgnt.h"

rt_thread_t g_rtc_clock_thread;
rt_mq_t g_rtc_clock_queue;
static int time_synced = 0;

/*
time-a.nist.gov 129.6.15.28
time-b.nist.gov    129.6.15.29
time-a.timefreq.bldrdoc.gov    132.163.4.101
time-b.timefreq.bldrdoc    132.163.4.102
time-c.timefreq.bldrdoc.gov    132.163.4.103
utcnist.colorado.edu    128.138.140.44
time.nist.gov    192.43.244.18
time-nw.nist.gov    131.107.1.10
nist1.datum.com    66.243.43.21
nist1-dc.glassey.com    216.200.93.8
nist1-ny.glassey.com    208.184.49.9
nist1-sj.glassey.com    207.126.98.204
nist1.aol-ca.truetime.com    207.200.81.113
nist1.aol-va.truetime.com    205.188.185.33
*/


//#define ALARM_CLOCK_ADDR ((8 << 20) - (16 << 10)) //8M norflash
//#define ALARM_CLOCK_LEN 4096

//flash
#define GPT_ALARM_CLOCK_ADDR_8M            ((8 << 20) - (8 << 10))
#define GPT_ALARM_CLOCK_ADDR_4M            ((4 << 20) - (8 << 10))

#define IS_LEAP_YEAR(year) (!((year) % 4) && (((year) % 100) || !((year) % 400)))

#define GPT_ALARM_CLOCK_ADDR        \
    ((gpt_system_info_data.Flash_Size == 0)? GPT_ALARM_CLOCK_ADDR_4M : GPT_ALARM_CLOCK_ADDR_8M)


#define GPT_RTC_CLOCK_COUNT        2
static gpt_clock_t g_alarm_array[GPT_RTC_CLOCK_COUNT];

//static char *g_alarm_url;

static char g_alarm_url[GPT_RTC_CLOCK_COUNT][256];

uint8_t g_rtc_alarm_is_running;
uint8_t g_rtc_clock_sleep_state = 0;

uint8_t g_alarm_times;
//debug
static void gpt_rtc_clock_array_debug_print(void)
{
    int i;
    for (i = 0; i < GPT_RTC_CLOCK_COUNT; i++)
    {
        GPT_RTC_CLOCK_DEBUG(g_alarm_array[i]);
        /*
        GPT_DEBUG_PRINT("EN:%i,YMD:%i-%i-%i,HMS:%i-%i-%i,week:%i",
                g_alarm_array[i].clock_enable,
                g_alarm_array[i].year,
                g_alarm_array[i].month,
                g_alarm_array[i].mday,
                g_alarm_array[i].hour,
                g_alarm_array[i].minute,
                g_alarm_array[i].second,
                g_alarm_array[i].wday
                );
                */
    }
}

void gpt_rtc_clock_set_sleep_state(void)
{
    g_rtc_clock_sleep_state = 1;
}

static int8_t gpt_rtc_clock_compare(gpt_clock_t *clock1, gpt_clock_t *clock2);
uint8_t g_min_second_clock_index;
uint8_t g_min_week_clock_index;

void gpt_rtc_clock_callback(void *arg)
{
    GPT_DEBUG_PRINT("clock alarm in IRQ:%i", (uint32_t)arg);
    gpt_rtc_clock_msg_t clock_msg;
    clock_msg.msg = ((uint32_t)arg == 1) ?
                    GPT_RTC_CLOCK_MSG_ALARM_SECOND : GPT_RTC_CLOCK_MSG_ALARM_WEEK;
    rt_mq_send(g_rtc_clock_queue, &clock_msg, sizeof(clock_msg));
}

static void gpt_rtc_clock_write_flash(void);
static void gpt_rtc_clock_refresh_setting(void)
{
    //HAL_RTC_StopSecAlarm();
    //HAL_RTC_StopWDayAlarm();
    gpt_clock_t current_clock;
    uint8_t isLeapYear;
    //HAL_RTC_GetYYMMDD(&isLeapYear, &current_clock.year,
    //        &current_clock.month, &current_clock.mday);
    //HAL_RTC_GetDDHHMMSS(&current_clock.wday, &current_clock.hour,
    //        &current_clock.minute, &current_clock.second);

    uint8_t i;
    uint8_t is_modify = 0;
    g_min_second_clock_index = GPT_RTC_CLOCK_COUNT;
    g_min_week_clock_index = GPT_RTC_CLOCK_COUNT;

    for (i = 0; i < GPT_RTC_CLOCK_COUNT; i++)
    {
        if (g_alarm_array[i].clock_enable != 1)
        {
            continue;
        }
        if (GPT_RTC_CLOCK_CHECK_TIME_CLOCK_ONCE(g_alarm_array[i]))
        {
            gpt_clock_t compare_clock = current_clock;
            compare_clock.wday = 0;
            if (gpt_rtc_clock_compare(g_alarm_array + i, &compare_clock) <= 0)
            {
                g_alarm_array[i].clock_enable = 0;
                is_modify = 1;
                continue;
            }
            if (g_min_second_clock_index == GPT_RTC_CLOCK_COUNT)
            {
                g_min_second_clock_index = i;
            }
            else
            {
                if (gpt_rtc_clock_compare(g_alarm_array + g_min_second_clock_index,
                                          g_alarm_array + i) > 0)
                {
                    g_min_second_clock_index = i;
                }
            }
        }
        else
        {
            if (g_alarm_array[g_min_week_clock_index].wday &
                    (1 << current_clock.wday))
            {
                continue;
            }
            if (g_min_week_clock_index == GPT_RTC_CLOCK_COUNT)
            {
                //else
                g_min_week_clock_index = i;
            }
            else
            {
                uint32_t prior_sel_second;
                uint32_t index_sel_second;
                uint32_t current_sel_second;

                prior_sel_second =
                    g_alarm_array[g_min_week_clock_index].hour * 3600 +
                    g_alarm_array[g_min_week_clock_index].minute * 60;
                index_sel_second = g_alarm_array[i].hour * 3600 + g_alarm_array[i].minute * 60;
                current_sel_second = current_clock.hour * 3600 + current_clock.minute * 60;
                if (prior_sel_second > current_sel_second &&
                        index_sel_second > current_sel_second)
                {
                    if (index_sel_second < prior_sel_second)
                    {
                        g_min_week_clock_index = i;
                    }
                }
                else if (prior_sel_second > current_sel_second &&
                         index_sel_second < current_sel_second)
                {
                }
                else if (prior_sel_second < current_sel_second &&
                         index_sel_second > current_sel_second)
                {
                    g_min_week_clock_index = i;
                }
                else if (prior_sel_second < current_sel_second &&
                         index_sel_second < current_sel_second)
                {
                    if (index_sel_second < prior_sel_second)
                    {
                        g_min_week_clock_index = i;
                    }
                }
            }
        }
    }
    GPT_DEBUG_PRINT("Search:%i,%i", g_min_second_clock_index, g_min_week_clock_index);
    gpt_rtc_clock_array_debug_print();

    g_rtc_alarm_is_running = 0;
#if GPT_RTC_CLOCK_RTC_ENABLE
    //RTC start
    if (g_min_second_clock_index < GPT_RTC_CLOCK_COUNT)
    {
        struct tm t;
        time_t clock_time;
        memset(&t, 0, sizeof(t));
        t.tm_year = g_alarm_array[g_min_second_clock_index].year;
        t.tm_mon = g_alarm_array[g_min_second_clock_index].month;
        t.tm_mday = g_alarm_array[g_min_second_clock_index].mday;
        t.tm_hour = g_alarm_array[g_min_second_clock_index].hour;
        t.tm_min = g_alarm_array[g_min_second_clock_index].minute;
        t.tm_sec = g_alarm_array[g_min_second_clock_index].second;
        clock_time = mktime(&t);

        t.tm_year = current_clock.year;
        t.tm_mon = current_clock.month;
        t.tm_mday = current_clock.mday;
        t.tm_hour = current_clock.hour;
        t.tm_min = current_clock.minute;
        t.tm_sec = current_clock.second;
        clock_time -= mktime(&t);

        RTC_SecAlarmStartParam param;
        param.alarmSeconds = clock_time;
        param.callback = gpt_rtc_clock_callback;
        param.arg = (void *)1;
        g_rtc_alarm_is_running = 1;
        HAL_RTC_StartSecAlarm(&param);
    }
    if (g_min_week_clock_index < GPT_RTC_CLOCK_COUNT)
    {
        RTC_WDayAlarmStartParam param;
        param.alarmHour = g_alarm_array[g_min_week_clock_index].hour;
        param.alarmMinute = g_alarm_array[g_min_week_clock_index].minute;
        param.alarmSecond = 0;
        param.alarmWDayMask = g_alarm_array[g_min_week_clock_index].wday;
        param.callback = gpt_rtc_clock_callback;
        param.arg = (void *)2;
        g_rtc_alarm_is_running = 1;
        HAL_RTC_StartWDayAlarm(&param);
    }
    if (is_modify == 1)
    {
        gpt_rtc_clock_write_flash();
    }
#endif
}


static void gpt_rtc_clock_read_flash(void)
{
#if (GPT_RTC_CLOCK_SAVE_MEMORY_MODE_ENABLE != 0)
    fdcm_handle_t *hd = NULL;
    uint32_t len;
    hd = fdcm_open(0, GPT_ALARM_CLOCK_ADDR, 4096);
    if (hd == NULL)
    {
        GPT_DEBUG_PRINT("hd == NULL");
        return;
    }
    len = fdcm_read(hd, g_alarm_array, sizeof(g_alarm_array));
    if (len != sizeof(g_alarm_array))
    {
        GPT_DEBUG_PRINT("len:%i,but size:%i", len, sizeof(g_alarm_array));
    }
    (void)len;
    fdcm_close(hd);
    GPT_DEBUG_PRINT("Read RTC:");
    gpt_rtc_clock_array_debug_print();
#endif
}

static void gpt_rtc_clock_write_flash(void)
{
#if (GPT_RTC_CLOCK_SAVE_MEMORY_MODE_ENABLE != 0)
    fdcm_handle_t *hd = NULL;
    uint32_t len;
    hd = fdcm_open(0, GPT_ALARM_CLOCK_ADDR, 4096);
    GPT_DEBUG_ASSERT(hd);
    len = fdcm_write(hd, g_alarm_array, sizeof(g_alarm_array));
    GPT_DEBUG_ASSERT(len == sizeof(g_alarm_array));
    (void)len;
    fdcm_close(hd);
    GPT_DEBUG_PRINT("Write RTC:");
    gpt_rtc_clock_array_debug_print();
#endif
}

void gpt_rtc_clock_ctrl(uint8_t msg, gpt_clock_t clock, char *audio_url)
{
    gpt_rtc_clock_msg_t clock_msg;
    clock_msg.msg = msg;
    clock_msg.clock = clock;
    clock_msg.audio_url = audio_url;
    if (rt_mq_send(g_rtc_clock_queue, &clock_msg, sizeof(clock_msg)) != RT_EOK)
    {
        GPT_DEBUG_PRINT("send error");
    }
}


/*
void HAL_RTC_SetYYMMDD(uint8_t isLeapYear, uint8_t year, uint8_t month, uint8_t mday);
void HAL_RTC_SetDDHHMMSS(RTC_WeekDay wday, uint8_t hour, uint8_t minute, uint8_t second);

void HAL_RTC_GetYYMMDD(uint8_t *isLeapYear, uint8_t *year, uint8_t *month, uint8_t *mday);
void HAL_RTC_GetDDHHMMSS(RTC_WeekDay *wday, uint8_t *hour, uint8_t *minute, uint8_t *second) ;

void HAL_RTC_StartSecAlarm(const RTC_SecAlarmStartParam *param);
void HAL_RTC_StopSecAlarm(void);

void HAL_RTC_StartWDayAlarm(const RTC_WDayAlarmStartParam *param);
void HAL_RTC_StopWDayAlarm(void);

uint64_t HAL_RTC_GetFreeRunTime(void);
*/



static void gpt_rtc_clock_sync(gpt_clock_t *save_clock)
{
    //uint8_t leap = IS_LEAP_YEAR(save_clock->year);
    uint8_t leap = IS_LEAP_YEAR(save_clock->year + 1900);
#if GPT_RTC_CLOCK_RTC_ENABLE
    HAL_RTC_SetYYMMDD(leap, save_clock->year, save_clock->month, save_clock->mday);
    HAL_RTC_SetDDHHMMSS(save_clock->wday, save_clock->hour, save_clock->minute, save_clock->second);
#endif
    GPT_PRINT_LOG("Sync:");
    GPT_RTC_CLOCK_LOG(*save_clock);
    //wlan_sta_ap_info
    gpt_rtc_clock_refresh_setting();

}

static uint8_t gpt_rtc_clock_check_valid(gpt_clock_t *check_clock)
{
#if GPT_RTC_CLOCK_RTC_ENABLE
    uint8_t is_once_clock;
    is_once_clock = (check_clock->wday == 0) ? 1 : 0;
    if (is_once_clock)
    {
        if (check_clock->year <= RTC_YEAR_MAX &&
                (check_clock->month >= RTC_MONTH_MIN && check_clock->month <= RTC_MONTH_MAX) &&
                (check_clock->mday >= RTC_MDAY_MIN && check_clock->month <= RTC_MDAY_MAX) &&
                (check_clock->hour >= RTC_HOUR_MIN && check_clock->hour <= RTC_HOUR_MAX) &&
                (check_clock->minute >= RTC_MINUTE_MIN && check_clock->minute <= RTC_MINUTE_MAX) &&
                (check_clock->second >= RTC_SECOND_MIN && check_clock->second <= RTC_SECOND_MAX)
           )
        {
            return 1;
        }
    }
    else
    {
        if ((check_clock->hour >= RTC_HOUR_MIN && check_clock->hour <= RTC_HOUR_MAX) &&
                (check_clock->minute >= RTC_MINUTE_MIN && check_clock->minute <= RTC_MINUTE_MAX) &&
                (check_clock->second >= RTC_SECOND_MIN && check_clock->second <= RTC_SECOND_MAX)
           )
        {
            return 1;
        }
    }
    return 0;
#else
    return 1;
#endif
}



#define GPT_RTC_CLOCK_COMPARE_ITEM(a,b)        ((a)>(b) ? (1) : (((a)<(b) ? (-1) : (0))))
static int8_t gpt_rtc_clock_compare(gpt_clock_t *clock1, gpt_clock_t *clock2)
{
    uint8_t *cmp_addr1 = &clock1->year;
    uint8_t *cmp_addr2 = &clock2->year;
    uint8_t i;
    if (clock1->wday == 0 && clock2->wday == 0)
    {
        for (i = 0; i < 6; i++) //year -> second
        {
            register uint8_t cmp_result = GPT_RTC_CLOCK_COMPARE_ITEM(*cmp_addr1, *cmp_addr2);
            if (cmp_result == 0)
            {
                cmp_addr1++;
                cmp_addr2++;
                continue;
            }
            return cmp_result;
        }
    }
    else
    {
        cmp_addr1 = &clock1->hour;
        cmp_addr2 = &clock2->hour;
        //GPT_DEBUG_PRINT("every week:");
        //GPT_RTC_CLOCK_DEBUG(*clock1);
        //GPT_RTC_CLOCK_DEBUG(*clock2);
        for (i = 0; i < 4; i++) //hour -> wday
        {
            register uint8_t cmp_result = GPT_RTC_CLOCK_COMPARE_ITEM(*cmp_addr1, *cmp_addr2);
            //GPT_DEBUG_PRINT("cmp:%i-%i, result:%i", *cmp_addr1, *cmp_addr2, cmp_result);
            if (cmp_result == 0)
            {
                cmp_addr1++;
                cmp_addr2++;
                continue;
            }
            return cmp_result;
        }
    }
    return 0;
}


static void gpt_rtc_clock_save_clock(gpt_clock_t *save_clock, char *audio_url)
{
    int i;
    if (gpt_rtc_clock_check_valid(save_clock) == 0)
    {
        GPT_PRINT_LOG("Error:is not valid rtc_clock");
        GPT_RTC_CLOCK_LOG(*save_clock);
        return;
    }

    for (i = 0; i < GPT_RTC_CLOCK_COUNT; i++)
    {
        if (g_alarm_array[i].clock_enable == 1)
        {
            if (gpt_rtc_clock_compare(g_alarm_array + i, save_clock) == 0)
            {
                GPT_PRINT_LOG("Warning: save_clock is exist!");
                return;
            }
        }
    }

    for (i = 0; i < GPT_RTC_CLOCK_COUNT; i++)
    {
        if (g_alarm_array[i].clock_enable != 1)
        {
            memcpy(g_alarm_array + i, save_clock, sizeof(gpt_clock_t));
            g_alarm_array[i].clock_enable = 1;
            break;
        }
    }

    if (i == GPT_RTC_CLOCK_COUNT)
    {
        return;
    }
    if (audio_url)
    {
        strcpy(g_alarm_url[i], audio_url);
    }
    else
    {
        strcpy(g_alarm_url[i], "");
    }
    gpt_rtc_clock_write_flash();
    gpt_rtc_clock_refresh_setting();
}

static void gpt_rtc_clock_del_clock(gpt_clock_t *del_clock)
{
    int i;
    GPT_PRINT_LOG("delete:");
    GPT_RTC_CLOCK_LOG(*del_clock);
    for (i = 0; i < GPT_RTC_CLOCK_COUNT; i++)
    {
        if (g_alarm_array[i].clock_enable != 1)
        {
            continue;
        }
        if (gpt_rtc_clock_compare(g_alarm_array + i, del_clock) == 0)
        {
            g_alarm_array[i].clock_enable = 0;
            strcpy(g_alarm_url[i], "");
            GPT_DEBUG_PRINT("delete success:%i", i);
            gpt_rtc_clock_refresh_setting();
            return;
        }
    }
    GPT_DEBUG_PRINT("delete failed!");
}

static void gpt_rtc_set_wakeup_timer(void)
{
    gpt_clock_t current_clock;
    uint8_t isLeapYear;

#if GPT_RTC_CLOCK_RTC_ENABLE
    HAL_RTC_StopSecAlarm();
    HAL_RTC_StopWDayAlarm();
    HAL_RTC_GetYYMMDD(&isLeapYear, &current_clock.year,
                      &current_clock.month, &current_clock.mday);
    HAL_RTC_GetDDHHMMSS(&current_clock.wday, &current_clock.hour,
                        &current_clock.minute, &current_clock.second);
#endif

    GPT_DEBUG_PRINT("Current:");
    GPT_RTC_CLOCK_LOG(current_clock);


    uint32_t second_clock_distance = 0xFFFFFFFF;
    uint32_t week_clock_distance = 0xFFFFFFFF;
    uint32_t current_second = current_clock.hour * 3600 + current_clock.minute * 60;
    //RTC start
    if (g_min_second_clock_index < GPT_RTC_CLOCK_COUNT)
    {
        uint32_t clock_second =
            g_alarm_array[g_min_second_clock_index].hour * 3600 +
            g_alarm_array[g_min_second_clock_index].minute * 60;
        if (current_second < clock_second)
        {
            second_clock_distance = clock_second - current_second;
            GPT_DEBUG_PRINT("second clock enable:%i!", second_clock_distance);
        }
    }
    if (g_min_week_clock_index < GPT_RTC_CLOCK_COUNT)
    {
        uint32_t week_second =
            g_alarm_array[g_min_week_clock_index].hour * 3600 +
            g_alarm_array[g_min_week_clock_index].minute * 60;
        if (current_second < week_second)
        {
            week_clock_distance = week_second - current_second;
            GPT_DEBUG_PRINT("week clock enable:%i!", week_clock_distance);
        }
    }


    GPT_DEBUG_ASSERT(second_clock_distance != 0xFFFFFFFF || week_clock_distance != 0xFFFFFFFF);
    uint32_t wakeup_ms =
        (week_clock_distance < second_clock_distance ?
         week_clock_distance : second_clock_distance) * 1000;

    GPT_DEBUG_PRINT("wakeup_ms:%i!", wakeup_ms);
    int i;
    GPT_DEBUG_ASSERT(g_rtc_alarm_is_running != 0);

#if GPT_RTC_CLOCK_RTC_ENABLE
    HAL_Wakeup_Init();
    i = HAL_Wakeup_SetTimer_mS(wakeup_ms);
    if (i)
    {
        GPT_PRINT_LOG("Error setTimer:%i", i);
    }
#endif
}

static void gpt_rtc_clock_hibernation(void)
{
#if GPT_RTC_CLOCK_RTC_ENABLE
    gpt_system_info_data.System_Stat = GPT_SYSTEM_STAT_POWER_OFF;
    aud_mgr_handler(AUDIO_DEVICE_MANAGER_MUTE, AUDIO_OUT_DEV_SPEAKER, 1);
    GPT_PRINT_LOG("Will De-init->wlan_sta_disconnect");
    wlan_sta_disconnect();
    net_config(g_wlan_netif, 0);
    wlan_sta_disable();
    net_sys_onoff(0);
    //net_sys_stop();

    GPT_PRINT_LOG("Will De-init->gpt_led_event_deinit");
    gpt_led_event_deinit();
    GPT_PRINT_LOG("Will De-init->bat_charge_task_deinit");
    bat_charge_task_deinit() ;
    GPT_PRINT_LOG("Will De-init->ad_button_deinit");
    ad_button_deinit() ;
    GPT_PRINT_LOG("Will De-init->gpt_Media_Play_deinit");
    gpt_Media_Play_deinit();
    //GPT_PRINT_LOG("Will De-init->player_task_deinit");
    //player_task_deinit();
    GPT_PRINT_LOG("Will De-init->HAL_ADC_DeInit");
    HAL_ADC_DeInit();
    VOLUME_CTRL_DEINIT();
    GPT_PRINT_LOG("Will De-init->aud_mgr_deinit");
    aud_mgr_deinit();
    GPT_PRINT_LOG("Will De-init->HAL_CODEC_DeInit");
    HAL_CODEC_DeInit();

    OS_ThreadSuspendScheduler();

    gpt_rtc_set_wakeup_timer();
    GPT_PRINT_LOG("Will enter->PM_MODE_HIBERNATION");
    OS_Sleep(50);
    pm_enter_mode(PM_MODE_HIBERNATION);
    GPT_DEBUG_PRINT("Wake up failed... system will reboot...");
    OS_Sleep(100);
    GPT_ARM_REBOOT();
    while (1)
    {
        OS_Sleep(100);
    }
#endif
}

static void gpt_rtc_clock_poweroff(void)
{
    if (g_rtc_alarm_is_running)
    {
        gpt_rtc_clock_hibernation();
    }

    gpt_rtc_clock_deinit();
}

void gpt_rtc_clock_task(void *arg)
{
    gpt_rtc_clock_msg_t clock_msg;
    for (int i = 0; i < GPT_RTC_CLOCK_COUNT; i++)
    {
        g_alarm_array[i].clock_enable = 0;
    }
    gpt_rtc_clock_read_flash();
    GPT_DEBUG_PRINT("clock task running");
    while (1)
    {
        rt_mq_recv(g_rtc_clock_queue, &clock_msg, sizeof(clock_msg), RT_WAITING_FOREVER);
        rt_kprintf("clock_msg.msg:%i", clock_msg.msg);
        switch (clock_msg.msg)
        {
        case GPT_RTC_CLOCK_MSG_SYNC_RTC:
        {
            gpt_rtc_clock_sync(&clock_msg.clock);
            break;
        }
        case GPT_RTC_CLOCK_MSG_SAVE_CLOCK:
        {
            gpt_rtc_clock_save_clock(&clock_msg.clock, clock_msg.audio_url);
            break;
        }
        case GPT_RTC_CLOCK_MSG_DEL_CLOCK:
        {
            gpt_rtc_clock_del_clock(&clock_msg.clock);
            break;
        }
        case GPT_RTC_CLOCK_MSG_ALARM_WEEK:
        {
#ifdef GPT_RTC_CLOCK_RTC_ENABLE
            while (1)
            {
                if (gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_STANDBY ||
                        gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_AI ||
                        gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_PLAYING ||
                        gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_PAUSE)
                {
                    break;
                }
                else
                {
                    GPT_DEBUG_PRINT("RTC Wait system stat..");
                    OS_Sleep(1);
                }
            }
            Gpt_Player_Fun_Public_Data.Audio_Pause();
            gpt_system_info_data.System_Stat = GPT_SYSTEM_STAT_STANDBY;

            //GPT_DEBUG_PRINT("Clock alarm in task mode!");
            GPT_DEBUG_PRINT("g_min_week_clock_index:%i", g_min_week_clock_index);
            if (g_min_week_clock_index < GPT_RTC_CLOCK_COUNT &&
                    strlen(g_alarm_url[g_min_week_clock_index]) &&
                    netif_is_up(g_wlan_netif) && netif_is_link_up(g_wlan_netif))
            {
                g_tts_flag = TTS_FLAG_ALARM;
                gpt_Media_Play_Set_Url(g_alarm_url[g_min_week_clock_index],
                                       strlen(g_alarm_url[g_min_week_clock_index]));
                gpt_Media_Play_Ctrl_Fun(GPT_AUDIO_CMD_PLAY, GPT_AUDIO_TYPE_TTS, 0, 0);
            }
            else
            {
                GPT_DEBUG_PRINT("will play tone--");
                g_alarm_times = GPT_RTC_CLOCK_CLOCK_TIMES;
                gpt_Media_Play_Ctrl_Fun(GPT_AUDIO_CMD_PLAY,
                                        GPT_AUDIO_TYPE_NORFLASH_TONE, TONE_ALARM, 0);
            }
            gpt_rtc_clock_refresh_setting();
#endif

            break;
        }
        case GPT_RTC_CLOCK_MSG_ALARM_SECOND:
        {
#ifdef GPT_RTC_CLOCK_RTC_ENABLE
            while (1)
            {
                if (gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_STANDBY ||
                        gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_AI ||
                        gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_PLAYING ||
                        gpt_system_info_data.System_Stat == GPT_SYSTEM_STAT_PAUSE)
                {
                    break;
                }
                else
                {
                    GPT_DEBUG_PRINT("RTC Wait system stat..");
                    OS_Sleep(1);
                }
            }

            Gpt_Player_Fun_Public_Data.Audio_Pause() ;
            gpt_system_info_data.System_Stat = GPT_SYSTEM_STAT_STANDBY ;
            //GPT_DEBUG_PRINT("Clock alarm in task mode!");
            GPT_DEBUG_PRINT("g_min_second_clock_index:%i", g_min_second_clock_index);
            if (g_min_second_clock_index < GPT_RTC_CLOCK_COUNT &&
                    strlen(g_alarm_url[g_min_second_clock_index]) &&
                    netif_is_up(g_wlan_netif) && netif_is_link_up(g_wlan_netif))
            {
                g_tts_flag = TTS_FLAG_ALARM;
                gpt_Media_Play_Set_Url(g_alarm_url[g_min_second_clock_index],
                                       strlen(g_alarm_url[g_min_second_clock_index]));
                gpt_Media_Play_Ctrl_Fun(GPT_AUDIO_CMD_PLAY, GPT_AUDIO_TYPE_TTS, 0, 0);
                strcpy(g_alarm_url[g_min_second_clock_index], "");
            }
            else
            {
                GPT_DEBUG_PRINT("will play tone--");
                g_alarm_times = GPT_RTC_CLOCK_CLOCK_TIMES;
                gpt_Media_Play_Ctrl_Fun(GPT_AUDIO_CMD_PLAY,
                                        GPT_AUDIO_TYPE_NORFLASH_TONE, TONE_ALARM, 0);
            }
            gpt_rtc_clock_refresh_setting();
#endif

        }
        break;

        case GPT_RTC_CLOCK_MSG_POWEROFF:
        {
            gpt_rtc_clock_poweroff();
            break;
        }
        }
    }
}


void bbt_addr_init(void *param);


void gpt_rtc_clock_init(void)
{
    g_alarm_times = GPT_RTC_CLOCK_CLOCK_TIMES;
    bbt_addr_init(NULL);

#ifdef GPT_RTC_CLOCK_RTC_ENABLE
    g_rtc_clock_queue = rt_mq_create("gpt clock", sizeof(gpt_rtc_clock_msg_t), 5, RT_IPC_FLAG_FIFO);

    if (g_rtc_clock_queue != NULL)
    {
        g_rtc_clock_thread = rt_thread_create("gpt clock",
                                              gpt_rtc_clock_task,
                                              NULL,
                                              BBT_ADDR_STACK_SIZE,
                                              20,
                                              10);
        if (g_rtc_clock_thread)
            rt_thread_startup(g_rtc_clock_thread);
    }
#endif
    return;
}

void gpt_rtc_clock_deinit(void)
{
    if (g_rtc_clock_thread != NULL)
    {
        rt_mq_delete(g_rtc_clock_queue);
        rt_thread_delete(g_rtc_clock_thread);
    }
}

/*******************************************************************************
 *@brief: Inplementation of oldtree http file upload
 *@author: zhujinjian
 *@date: 2017/03/08
*******************************************************************************/
#define SERVER_HOST "www.baidu.com"
#define SERVER_PORT "80"
#define API_URL "/service/getIpInfo.php?ip=myip"
#define BUFFER_SIZE   2048
#define HOST_PORT     80
//#define RECV_TO       15000    // ms
#define RECV_TO       3000    // ms



static rt_thread_t g_bbt_addr_thread;
//#define BBT_ADDR_STACK_SIZE (2*1024)
#define BBT_ADDR_STACK_SIZE (1*1024)

#define BBT_ADDR_DBG_ENABLE        1
#define HAL_LOG(flags, fmt, arg...)    \
    do {                                \
        if (flags)                         \
            printf(fmt, ##arg);        \
    } while (0)

#define BBT_ADDR_DBG(fmt, arg...)    \
            HAL_LOG(BBT_ADDR_DBG_ENABLE, "[BBT ADDR] "fmt, ##arg)

//const char *wday_str[] = {
//"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"
//};
const char *wday_str[] =
{
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char *mon_str[] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"
};

const int8_t g_mday[2][12] =
{
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, /* normal year */
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }    /* leap year */
};


int is_gmt(char *s)
{
    if (strcmp(s, "GMT") == 0)
    {
        //printf("---IS GMT----\n");
        return 1;
    }
    //printf("--- not GMT --\n");
    return 0;
}
int get_week(char *s)
{
    int i;
    uint8_t cmp_len = strlen(s);
    //GPT_DEBUG_ASSERT(cmp_len <= 6);
    if (cmp_len > 6)
    {
        cmp_len = 6;
    }
    for (i = 0; i < 7; i++)
    {
        if (memcmp(s, wday_str[i], cmp_len) == 0)
        {
            return i;
        }
        //if(strcmp(s,wday_str[i]) == 0)
        //{
        //return i;
        //}
    }
    return 7;
}

int get_month(char *s)
{
    int i;
    for (i = 0; i < 12; i++)
    {
        if (strcmp(s, mon_str[i]) == 0)
        {
            return (i + 1);
        }
    }
    return 0;
}

static void bbt_addr_task(void *param)
{
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd = -1, bytes = 0;
    char *pbuffer = NULL;

    // MainTask_GetStatus(MAINTASK_WIFI_CONNECT_OK, MAX_DELAY);
    while (rt_wlan_is_ready() != RT_TRUE)
    {
        rt_thread_delay(10);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        BBT_ADDR_DBG("ERROR: socket\n");
        goto exit;
    }
    else
    {

        int recv_timeout_ms = RECV_TO;
#if defined(LWIP_SO_SNDRCVTIMEO_NONSTANDARD) && (LWIP_SO_SNDRCVTIMEO_NONSTANDARD == 0)    // lwip 1.5.0
        struct timeval recv_timeout;
        recv_timeout.tv_sec = recv_timeout_ms / 1000;
        recv_timeout.tv_usec = recv_timeout_ms % 1000 * 1000;
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &recv_timeout, sizeof(recv_timeout));
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));

#else    // lwip 1.4.1
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &recv_timeout_ms, sizeof(recv_timeout_ms));
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout_ms, sizeof(recv_timeout_ms));

#endif

    }

    server = gethostbyname(SERVER_HOST);

    if (server == NULL)
    {
        BBT_ADDR_DBG("[ERROR] [xskh] Get host ip failed\n");
        goto exit;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(HOST_PORT);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    BBT_ADDR_DBG("------start connect --- \n");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        BBT_ADDR_DBG("[ERROR] [xskh] connect failed\n");
        goto exit;
    }
    BBT_ADDR_DBG("------ connected --- \n");
    pbuffer = malloc(BUFFER_SIZE + 1);
    if (pbuffer == NULL)
    {
        BBT_ADDR_DBG("malloc buffer failed\n");
        goto exit;
    }

    memset(pbuffer, 0, BUFFER_SIZE + 1);
    strcpy(pbuffer, "POST ");
    strcat(pbuffer, API_URL);
    strcat(pbuffer, " HTTP/1.1\r\nHost: ");

    strcat(pbuffer, SERVER_HOST);

    strcat(pbuffer, ":");
    strcat(pbuffer, SERVER_PORT);
    strcat(pbuffer, "\r\n\r\n");
    BBT_ADDR_DBG("\n [xskh] Request: header\n%s\n", pbuffer);

    bytes = send(sockfd, pbuffer, strlen(pbuffer), 0);
    if (bytes < 0)
    {
        BBT_ADDR_DBG("[ERROR] [xskh] send header failed\n");
        goto exit;
    }
    bytes = 0 ;
    int32_t rec_len = 0;
    while ((rec_len = recv(sockfd, pbuffer + bytes, BUFFER_SIZE - bytes, 0)) > 0)
    {
        BBT_ADDR_DBG("recv len %d\n", rec_len);
        bytes += rec_len ;
        char *endstr = strstr(pbuffer, "}}");
        if (endstr != NULL)
        {
            break;
        }
    }
    BBT_ADDR_DBG("pbuffer:%s", pbuffer);
    if (bytes > 0)
    {
        pbuffer[bytes] = 0;
        char data_data[64] = {0};
        char month_str[16] = {0};
        char week_str[16] = {0};
        char mday_str[8] = {0};
        char year_str[8] = {0};
        char hour_str[8] = {0};
        char minute_str[8] = {0};
        char second_str[8] = {0};
        char gmt_str[8] = {0};


        //BBT_ADDR_DBG("\n[xskh] recv: %d - %s\n\n\n", bytes, pbuffer);
        char *data = strstr(pbuffer, "Date:");
        if (data == NULL)
        {
            rt_kprintf("gpt rtc clock: not found Date\n");
            goto exit;
        }
        //BBT_ADDR_DBG("\n[xskh] DATA-- %s\n\n\n", data);
        char *data_end = strstr(data, "\r\n");
        //BBT_ADDR_DBG("\n[xskh] DATAEND-- %s\n\n\n", data_end);
        int data_len = (data_end -  data);
        memcpy(data_data, data, data_len);
        //BBT_ADDR_DBG("\n[xskh] DATADATA-- %s\n\n\n", data_data);


        char *temp_start = strstr(data_data, " ");
        char *temp_end = strstr(temp_start + 1, " ");
        int temp_len = temp_end - temp_start;
        memcpy(week_str, temp_start + 1, temp_len - 2);

        temp_start = strstr(temp_end + 1, " ");
        temp_len = temp_start - temp_end;
        memcpy(mday_str, temp_end + 1, temp_len - 1);

        temp_end = strstr(temp_start + 1, " ");
        temp_len = temp_end - temp_start;
        memcpy(month_str, temp_start + 1, temp_len - 1);

        temp_start = strstr(temp_end + 1, " ");
        temp_len = temp_start - temp_end;
        memcpy(year_str, temp_end + 1, temp_len - 1);

        temp_end = strstr(temp_start + 1, ":");
        temp_len = temp_end - temp_start;
        memcpy(hour_str, temp_start + 1, temp_len - 1);

        temp_start = strstr(temp_end + 1, ":");
        temp_len = temp_start - temp_end;
        memcpy(minute_str, temp_end + 1, temp_len - 1);

        temp_end = strstr(temp_start + 1, " ");
        temp_len = temp_end - temp_start;
        memcpy(second_str, temp_start + 1, temp_len - 1);

        memcpy(gmt_str, temp_end + 1, data_len - (temp_end + 1 - data_data));
        //printf("\n\n---%s---len:%d\n\n",gmt_str,strlen(gmt_str));

        BBT_ADDR_DBG("\n[xskh] ------- %s-%s-%s-%s-%s:%s:%s %s\n\n\n", year_str, month_str, mday_str, week_str, hour_str, minute_str, second_str, gmt_str);

        SysYear = atoi(year_str);
        SysMonth =  get_month(month_str);
        SysDay = atoi(mday_str);
        SysWeek = get_week(week_str);
        //GPT_DEBUG_PRINT("week_str:%s, %i", week_str, t.tm_wday);
        SysHour = atoi(hour_str);

        if (is_gmt(gmt_str) == 1)
        {
            SysHour = SysHour + 8;
            //printf("--- t.tm_hour :%d\n",t.tm_hour);
            if (SysHour >= 24)
            {
                //printf("---is GMT---\n");
                SysHour = 0;
                SysWeek += 1;
                int leap = IS_LEAP_YEAR(SysYear + 1900);
                //if(t.tm_mday > g_mday[leap][t.tm_mon])
                if (SysDay > g_mday[leap][SysMonth - 1])
                {
                    SysDay = 1;
                    SysMonth ++;
                    if (SysMonth > 12)
                    {
                        SysMonth = 1;
                        SysYear ++;
                    }
                }
            }
        }

        SysMin = atoi(minute_str);
        SysSecond = atoi(second_str);

        //GPT_DEBUG_PRINT("will send msg");
        //gpt_rtc_clock_ctrl(GPT_RTC_CLOCK_MSG_SYNC_RTC, sync_clock, NULL);
        time_synced = 1;
        // MainTask_SetStatus(MAINTASK_APP_TIME_CHANGE, 1);

        //rtc_time_set(t);
        //struct rtc_timer tim;
        //rtc_time_get(&tim);


        data = strstr(pbuffer, "\"ip\":");
        //BBT_ADDR_DBG("--- %s \n",data);
        if (data != NULL)
        {
            data_end = strstr(data + 6, "\"");
            //BBT_ADDR_DBG("--- %s \n",data_end);
            if (data_end != NULL)
            {
                memset(data_data, 0, 64);
                data_len = (data_end - data);
                //BBT_ADDR_DBG("--- ip len: %d \n",data_len);
                data_len = data_len - 6;

                memcpy(data_data, data + 6, data_len);
                BBT_ADDR_DBG("--- ip : %s \n", data_data);
            }
        }
    }

exit:
    if (sockfd >= 0)
    {
        closesocket(sockfd);
        sockfd = -1;
    }
    if (pbuffer != NULL)
    {
        free(pbuffer);
        pbuffer = NULL;
    }

    // rt_thread_delete(g_bbt_addr_thread);
}

int time_is_synced(void)
{
    return time_synced;
}

void bbt_addr_init(void *param)
{
    if (g_bbt_addr_thread != NULL)
    {
        BBT_ADDR_DBG("\n--- bbt addr exist break!\n");
        return ;
    }

    g_bbt_addr_thread = rt_thread_create("bbt_addr",
                                         bbt_addr_task,
                                         param,
                                         BBT_ADDR_STACK_SIZE,
                                         20,
                                         10);

    if (g_bbt_addr_thread == NULL)
    {
        BBT_ADDR_DBG("create bbt addr task failed\n");
    }
    else
    {
        rt_thread_startup(g_bbt_addr_thread);
    }
}

void systime_sync(void)
{
    uint32_t sysTick;
    uint32_t us;
    if (HAL_GetTick() == sysTickCounterSave)
    {
        sysTick =  HAL_GetTick();
        if (sysTickSave > sysTick)
        {
            sysClk += sysTickSave - sysTick;
            sysTickSave = sysTick;
            us = ((long long)sysClk) * 1000 * 1000 / PLL_INPUT_OSC_RATE;
            sysUsecond = us % 1000;
            us = us / 1000;
            SysMsecond += us;
        }
    }
    else
    {
        SysMsecond += HAL_GetTick() - sysTickCounterSave;
        sysTickCounterSave = HAL_GetTick();
        sysTickSave = HAL_GetTick();
        sysClk = 0;
    }

    if (SysMsecond >= 1000)
    {
        SysMsecond = SysMsecond % 1000;
        SysSecond += 1;
    }

    if (SysSecond >= 60)
    {
        SysSecond = 0;
        SysMin += 1;
    }

    if (SysMin >= 60)
    {
        SysMin = 0;
        SysHour += 1;
    }

    if (SysHour >= 24)
    {
        SysHour = 0;
        SysDay++;
    }

    if ((SysMonth == 1) || (SysMonth == 3) || (SysMonth == 5)
            || (SysMonth == 7) || (SysMonth == 8)
            || (SysMonth == 10) || (SysMonth == 12))
    {
        if (SysDay >= 32) //31 day
        {
            SysDay = 1;
            SysMonth += 1;
        }
    }
    else if ((((SysYear % 4) == 0) && ((SysYear % 100) != 0)) || ((SysYear % 400) == 0))
    {
        if (SysMonth == 2)
        {
            if (SysDay >= 30) //29 day
            {
                SysDay = 1;
                SysMonth += 1;
            }
        }
        else
        {
            if (SysDay >= 31) //30 day
            {
                SysDay = 1;
                SysMonth += 1;
            }
        }
    }
    else if (SysMonth == 2)
    {
        if (SysDay >= 29) //28 day
        {
            SysDay = 1;
            SysMonth += 1;
        }
    }
    else
    {
        if (SysDay >= 31) //30 day
        {
            SysDay = 1;
            SysMonth += 1;
        }
    }

    if (SysMonth >= 13)
    {
        SysMonth = 1;
        SysYear++;
    }
}

int _gettimeofday(struct timeval *tv, struct timezone *tz)
{
    tv->tv_sec = 0;
    for (uint32_t i = 1970; i < SysYear; i++)
    {
        if ((((i % 4) == 0) && ((i % 100) != 0)) || ((i % 400) == 0))
        {
            tv->tv_sec +=  31622400;
        }
        else
        {
            tv->tv_sec +=  31536000;
        }
    }

    for (uint32_t i = 1; i < SysMonth; i++)
    {
        if ((i == 1) || (i == 3) || (i == 5)
                || (i == 7) || (i == 8)
                || (i == 10) || (i == 12))
        {
            tv->tv_sec += 2678400; //31 day
        }
        else if ((((SysYear % 4) == 0) && ((SysYear % 100) != 0)) || ((SysYear % 400) == 0))
        {
            if (SysMonth == 2)
            {
                tv->tv_sec += 2505600; //29 day
            }
            else
            {
                tv->tv_sec += 2592000; //30 day
            }
        }
        else if (SysMonth == 2)
        {
            tv->tv_sec += 2419200; //28 day
        }
        else
        {
            tv->tv_sec += 2592000; //30 day
        }
    }

    for (uint32_t i = 1; i < SysDay; i++)
        tv->tv_sec += 86400; // 1day

    for (uint32_t i = 0; i < SysHour; i++)
        tv->tv_sec += 3600; // 1 hour

    for (uint32_t i = 0; i < SysMin; i++)
        tv->tv_sec += 60; //  1 min

    tv->tv_sec += SysSecond;
    tv->tv_usec = SysMsecond * 1000 + sysUsecond;

    tv->tv_sec -= 8 * 60 * 60;  // GMT - > BeiJing time
    printf("tv->sec = %ld, tv->tv_usec = %ld", tv->tv_sec, tv->tv_usec);
    printf(" [time] %04ld-%02ld-%02ld-%02ld-%02ld:%02ld:%02ld\n",
           SysYear, SysMonth, SysDay, SysWeek,
           SysHour, SysMin, SysSecond);
    return 0;
}
