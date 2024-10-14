/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _GPT_RTC_CLOCK_H_
#define _GPT_RTC_CLOCK_H_

#define GPT_RTC_CLOCK_MSG_SYNC_RTC          1
#define GPT_RTC_CLOCK_MSG_SAVE_CLOCK        2
#define GPT_RTC_CLOCK_MSG_DEL_CLOCK         3
#define GPT_RTC_CLOCK_MSG_ALARM_SECOND      5
#define GPT_RTC_CLOCK_MSG_ALARM_WEEK        6
#define GPT_RTC_CLOCK_MSG_POWEROFF          10


#define GPT_RTC_CLOCK_CHECK_TIME_CLOCK_ONCE(clock_var)    (((clock_var).wday == 0) ? 1 : 0)

#define GPT_RTC_CLOCK_DEBUG(clock)        \
    rt_kprintf("EN:%i,YMD:%i-%i-%i,HMS:%i-%i-%i,week:%X",    \
                (clock).clock_enable, (clock).year, \
                (clock).month, (clock).mday,\
                (clock).hour, (clock).minute,\
                (clock).second, (clock).wday)

#define GPT_RTC_CLOCK_LOG(clock)        \
    rt_kprintf("EN:%i,YMD:%i-%i-%i,HMS:%i-%i-%i,week:%X",    \
                (clock).clock_enable, (clock).year, \
                (clock).month, (clock).mday,\
                (clock).hour, (clock).minute,\
                (clock).second, (clock).wday)

#define GPT_DEBUG_PRINT rt_kprintf
#define GPT_PRINT_LOG rt_kprintf
#define GPT_DEBUG_ASSERT(x)

#define GPT_RTC_CLOCK_SAVE_MEMORY_MODE_ENABLE        0

#define GPT_RTC_CLOCK_CLOCK_TIMES    10

typedef struct
{
#pragma pack(1)
    uint8_t clock_enable;
    uint8_t year;   /* years since 1900-1900 */
    uint8_t month;  /* months since January - [0,11] */
    uint8_t mday;    /* day of the month - [1,31] */
    uint8_t hour;   /* hours since midnight - [0,23] */
    uint8_t minute; /* minutes after the hour - [0,59] */
    uint8_t second; /* second after the minutes - [0,59] */
    uint8_t wday;
} gpt_clock_t;

typedef struct
{
    uint8_t msg;
    gpt_clock_t clock;
    char *audio_url;
} gpt_rtc_clock_msg_t;

//extern uint8_t g_rtc_alarm_is_running;
void gpt_rtc_clock_init(void);
void gpt_rtc_clock_ctrl(uint8_t msg, gpt_clock_t clock, char *audio_url);
void gpt_rtc_clock_set_sleep_state(void);
void gpt_rtc_clock_deinit(void);
int time_is_synced(void);

#endif

