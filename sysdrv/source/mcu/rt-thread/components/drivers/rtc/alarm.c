/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2012-10-27     heyuanjie87       first version.
 * 2013-05-17     aozima            initial alarm event & mutex in system init.
 * 2020-07-22     Joseph Chen       change to "y-m-d-h-m-s" setting
 */

#include <rtthread.h>
#include <rtdevice.h>

#define RT_ALARM_DELAY             2
#define RT_ALARM_STATE_INITED   0x02
#define RT_ALARM_STATE_START    0x01
#define RT_ALARM_STATE_STOP     0x00

#if (defined(RT_USING_RTC) && defined(RT_USING_ALARM))
static struct rt_alarm_container _container;

rt_inline time_t alarm_mkdaysec(struct tm *time)
{
    return mktime(time);
}

static rt_err_t alarm_set(struct rt_alarm *alarm)
{
    rt_device_t device;
    struct rt_rtc_wkalarm wkalarm;
    rt_err_t ret;

    device = rt_device_find("rtc");
    if (device == RT_NULL)
    {
        return (RT_ERROR);
    }
    if (alarm->flag & RT_ALARM_STATE_START)
        wkalarm.enable = RT_TRUE;
    else
        wkalarm.enable = RT_FALSE;

    wkalarm.wktime = alarm->wktime;

    ret = rt_device_control(device, RT_DEVICE_CTRL_RTC_SET_ALARM, &wkalarm);
    if ((ret == RT_EOK) && wkalarm.enable)
    {
        ret = rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_ALARM, &wkalarm);
        if (ret == RT_EOK)
        {
            /*
              some RTC device like RX8025,it's alarms precision is 1 minute.
              in this case,low level RTC driver should set wkalarm->tm_sec to 0.
            */
            alarm->wktime = wkalarm.wktime;
        }
    }

    return (ret);
}

static void alarm_wakeup(struct rt_alarm *alarm, struct tm *now)
{
    rt_uint32_t sec_alarm, sec_now;
    rt_bool_t wakeup = RT_FALSE;
    time_t timestamp;

    sec_alarm = alarm_mkdaysec(&alarm->wktime);
    sec_now = alarm_mkdaysec(now);

    if (alarm->flag & RT_ALARM_STATE_START)
    {
        switch (alarm->flag & 0xFF00)
        {
        case RT_ALARM_ONESHOT:
        {
            sec_alarm = mktime(&alarm->wktime);
            sec_now = mktime(now);
            if (((sec_now - sec_alarm) <= RT_ALARM_DELAY) && (sec_now >= sec_alarm))
            {
                /* stop alarm */
                alarm->flag &= ~RT_ALARM_STATE_START;
                alarm_set(alarm);
                wakeup = RT_TRUE;
            }
        }
        break;
        case RT_ALARM_DAILY:
        {
            if (((sec_now - sec_alarm) <= RT_ALARM_DELAY) && (sec_now >= sec_alarm))
                wakeup = RT_TRUE;
        }
        break;
        case RT_ALARM_WEEKLY:
        {
            /* alarm at wday */
            sec_alarm += alarm->wktime.tm_wday * 24 * 3600;
            sec_now += now->tm_wday * 24 * 3600;

            if (((sec_now - sec_alarm) <= RT_ALARM_DELAY) && (sec_now >= sec_alarm))
                wakeup = RT_TRUE;
        }
        break;
        case RT_ALARM_MONTHLY:
        {
            /* monthly someday generate alarm signals */
            if (alarm->wktime.tm_mday == now->tm_mday)
            {
                if ((sec_now - sec_alarm) <= RT_ALARM_DELAY)
                    wakeup = RT_TRUE;
            }
        }
        break;
        case RT_ALARM_YAERLY:
        {
            if ((alarm->wktime.tm_mday == now->tm_mday) && \
                    (alarm->wktime.tm_mon == now->tm_mon))
            {
                if ((sec_now - sec_alarm) <= RT_ALARM_DELAY)
                    wakeup = RT_TRUE;
            }
        }
        break;
        }

        if ((wakeup == RT_TRUE) && (alarm->callback != RT_NULL))
        {
            timestamp = time(RT_NULL);
            alarm->callback(alarm, timestamp);
        }
    }
}

static void alarm_update(rt_uint32_t event)
{
    struct rt_alarm *alm_next = RT_NULL;
    struct rt_alarm *alarm;
    time_t sec_now, sec_alarm, sec_tmp;
    time_t sec_next = -1;
    time_t timestamp;
    struct tm now;
    rt_list_t *next;

    rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
    if (!rt_list_isempty(&_container.head))
    {
        /* get time of now */
        timestamp = time(RT_NULL);
        localtime_r(&timestamp, &now);

        for (next = _container.head.next; next != &_container.head; next = next->next)
        {
            alarm = rt_list_entry(next, struct rt_alarm, list);
            /* check the overtime alarm */
            alarm_wakeup(alarm, &now);
        }

        timestamp = time(RT_NULL);
        localtime_r(&timestamp, &now);
        sec_now = alarm_mkdaysec(&now);

        for (next = _container.head.next; next != &_container.head; next = next->next)
        {
            alarm = rt_list_entry(next, struct rt_alarm, list);
            /* calculate seconds from 00:00:00 */
            sec_alarm = alarm_mkdaysec(&alarm->wktime);

            if ((alarm->flag & RT_ALARM_STATE_START) && (alarm != _container.current))
            {
                sec_tmp = sec_alarm - sec_now;
                if (sec_tmp > 0)
                {
                    /* find alarm after now(now to 23:59:59) and the most recent */
                    if (sec_next == -1 || sec_tmp < sec_next)
                    {
                        sec_next = sec_tmp;
                        alm_next = alarm;
                    }
                }
            }
        }

        if (alm_next != RT_NULL)
        {
            if (alarm_set(alm_next) == RT_EOK)
                _container.current = alm_next;
        }
    }
    rt_mutex_release(&_container.mutex);
}

static rt_uint32_t days_of_year_month(int tm_year, int tm_mon)
{
    rt_uint32_t ret, year;

    year = tm_year + 1900;
    if (tm_mon == 1)
    {
        ret = 28 + ((!(year % 4) && (year % 100)) || !(year % 400));
    }
    else if (((tm_mon <= 6) && (tm_mon % 2 == 0)) || ((tm_mon > 6) && (tm_mon % 2 == 1)))
    {
        ret = 31;
    }
    else
    {
        ret = 30;
    }

    return (ret);
}

static rt_bool_t is_valid_date(struct tm *date)
{
    if (date->tm_year < 0)
    {
        return (RT_FALSE);
    }

    if ((date->tm_mon < 0) || (date->tm_mon > 11))
    {
        return (RT_FALSE);
    }

    if ((date->tm_mday < 1) || \
            (date->tm_mday > days_of_year_month(date->tm_year, date->tm_mon)))
    {
        return (RT_FALSE);
    }

    return (RT_TRUE);
}

static rt_err_t alarm_setup(rt_alarm_t alarm, struct tm *wktime)
{
    rt_err_t ret = RT_ERROR;
    time_t timestamp;
    struct tm *setup, now;

    setup = &alarm->wktime;
    *setup = *wktime;
    timestamp = time(RT_NULL);
    localtime_r(&timestamp, &now);

    /* if these are a "don't care" value,we set them to now*/
    if ((setup->tm_sec > 59) || (setup->tm_sec < 0))
        setup->tm_sec = now.tm_sec;
    if ((setup->tm_min > 59) || (setup->tm_min < 0))
        setup->tm_min = now.tm_min;
    if ((setup->tm_hour > 23) || (setup->tm_hour < 0))
        setup->tm_hour = now.tm_hour;

    switch (alarm->flag & 0xFF00)
    {
    case RT_ALARM_DAILY:
    {
        /* do nothing but needed */
    }
    break;
    case RT_ALARM_ONESHOT:
    {
        /* if these are "don't care" value we set them to now */
        if (setup->tm_year == RT_ALARM_TM_NOW)
            setup->tm_year = now.tm_year;
        if (setup->tm_mon == RT_ALARM_TM_NOW)
            setup->tm_mon = now.tm_mon;
        if (setup->tm_mday == RT_ALARM_TM_NOW)
            setup->tm_mday = now.tm_mday;
        /* make sure the setup is valid */
        if (!is_valid_date(setup))
            goto _exit;
    }
    break;
    case RT_ALARM_WEEKLY:
    {
        /* if tm_wday is a "don't care" value we set it to now */
        if ((setup->tm_wday < 0) || (setup->tm_wday > 6))
            setup->tm_wday = now.tm_wday;
    }
    break;
    case RT_ALARM_MONTHLY:
    {
        /* if tm_mday is a "don't care" value we set it to now */
        if ((setup->tm_mday < 1) || (setup->tm_mday > 31))
            setup->tm_mday = now.tm_mday;
    }
    break;
    case RT_ALARM_YAERLY:
    {
        /* if tm_mon is a "don't care" value we set it to now */
        if ((setup->tm_mon < 0) || (setup->tm_mon > 11))
            setup->tm_mon = now.tm_mon;

        if (setup->tm_mon == 1)
        {
            /* tm_mon is February */

            /* tm_mday should be 1~29.otherwise,it's a "don't care" value */
            if ((setup->tm_mday < 1) || (setup->tm_mday > 29))
                setup->tm_mday = now.tm_mday;
        }
        else if (((setup->tm_mon <= 6) && (setup->tm_mon % 2 == 0)) || \
                 ((setup->tm_mon > 6) && (setup->tm_mon % 2 == 1)))
        {
            /* Jan,Mar,May,Jul,Aug,Oct,Dec */

            /* tm_mday should be 1~31.otherwise,it's a "don't care" value */
            if ((setup->tm_mday < 1) || (setup->tm_mday > 31))
                setup->tm_mday = now.tm_mday;
        }
        else
        {
            /* tm_mday should be 1~30.otherwise,it's a "don't care" value */
            if ((setup->tm_mday < 1) || (setup->tm_mday > 30))
                setup->tm_mday = now.tm_mday;
        }
    }
    break;
    default:
    {
        goto _exit;
    }
    }

    if ((setup->tm_hour == 23) && (setup->tm_min == 59) && (setup->tm_sec == 59))
    {
        /*
           for insurance purposes, we will generate an alarm
           signal two seconds ahead of.
        */
        setup->tm_sec = 60 - RT_ALARM_DELAY;
    }
    /* set initialized state */
    alarm->flag |= RT_ALARM_STATE_INITED;
    ret = RT_EOK;

_exit:

    return (ret);
}

/** \brief send a rtc alarm event
 *
 * \param dev pointer to RTC device(currently unused,you can ignore it)
 * \param event RTC event(currently unused)
 * \return none
 */
void rt_alarm_update(rt_device_t dev, rt_uint32_t event)
{
    rt_event_send(&_container.event, 1);
}

/** \brief modify the alarm setup
 *
 * \param alarm pointer to alarm
 * \param cmd control command
 * \param arg argument
 */
rt_err_t rt_alarm_control(rt_alarm_t alarm, int cmd, void *arg)
{
    rt_err_t ret = RT_ERROR;

    RT_ASSERT(alarm != RT_NULL);

    rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
    switch (cmd)
    {
    case RT_ALARM_CTRL_MODIFY:
    {
        struct rt_alarm_setup *setup;

        RT_ASSERT(arg != RT_NULL);
        setup = arg;
        rt_alarm_stop(alarm);
        alarm->flag = setup->flag & 0xFF00;
        alarm->wktime = setup->wktime;
        ret = alarm_setup(alarm, &alarm->wktime);
    }
    break;
    }

    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief start an alarm
 *
 * \param alarm pointer to alarm
 * \return RT_EOK
 */
rt_err_t rt_alarm_start(rt_alarm_t alarm)
{
    rt_int32_t sec_now, sec_old, sec_new;
    rt_err_t ret = RT_ERROR;
    time_t timestamp;
    struct tm now;

    if (alarm == RT_NULL)
        return (ret);
    rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
    if (!(alarm->flag & RT_ALARM_STATE_INITED))
    {
        if (alarm_setup(alarm, &alarm->wktime) != RT_EOK)
            goto _exit;
    }
    if ((alarm->flag & 0x01) == RT_ALARM_STATE_STOP)
    {
        timestamp = time(RT_NULL);
        localtime_r(&timestamp, &now);

        alarm->flag |= RT_ALARM_STATE_START;
        /* set alarm */
        if (_container.current == RT_NULL)
        {
            ret = alarm_set(alarm);
        }
        else
        {
            sec_now = alarm_mkdaysec(&now);
            sec_old = alarm_mkdaysec(&_container.current->wktime);
            sec_new = alarm_mkdaysec(&alarm->wktime);

            if ((sec_new < sec_old) && (sec_new > sec_now))
            {
                ret = alarm_set(alarm);
            }
            else if ((sec_new > sec_now) && (sec_old < sec_now))
            {
                ret = alarm_set(alarm);
            }
            else if ((sec_new < sec_old) && (sec_old < sec_now))
            {
                ret = alarm_set(alarm);
            }
            else
            {
                ret = RT_EOK;
                goto _exit;
            }
        }

        if (ret == RT_EOK)
        {
            _container.current = alarm;
        }
    }

_exit:
    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief stop an alarm
 *
 * \param alarm pointer to alarm
 * \return RT_EOK
 */
rt_err_t rt_alarm_stop(rt_alarm_t alarm)
{
    rt_err_t ret = RT_ERROR;

    if (alarm == RT_NULL)
        return (ret);
    rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
    if (!(alarm->flag & RT_ALARM_STATE_START))
        goto _exit;
    /* stop alarm */
    alarm->flag &= ~RT_ALARM_STATE_START;

    if (_container.current == alarm)
    {
        ret = alarm_set(alarm);
        _container.current = RT_NULL;
    }

    if (ret == RT_EOK)
        alarm_update(0);

_exit:
    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief delete an alarm
 *
 * \param alarm pointer to alarm
 * \return RT_EOK
 */
rt_err_t rt_alarm_delete(rt_alarm_t alarm)
{
    rt_err_t ret = RT_ERROR;

    if (alarm == RT_NULL)
        return (ret);
    rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
    /* stop the alarm */
    alarm->flag &= ~RT_ALARM_STATE_START;
    if (_container.current == alarm)
    {
        ret = alarm_set(alarm);
        _container.current = RT_NULL;
        /* set new alarm if necessary */
        alarm_update(0);
    }
    rt_list_remove(&alarm->list);
    rt_free(alarm);

    rt_mutex_release(&_container.mutex);

    return (ret);
}

static int is_valid_alarm(struct rt_alarm_setup setup)
{
    struct tm now;
    time_t timestamp;
    time_t sec_new;
    time_t sec_now;

    timestamp = time(RT_NULL);
    localtime_r(&timestamp, &now);
    sec_now = alarm_mkdaysec(&now);
    setup.wktime.tm_year -= 1900;
    setup.wktime.tm_mon -= 1;
    sec_new = alarm_mkdaysec(&setup.wktime);

    if (sec_new < sec_now)
        return RT_FALSE;

    return RT_TRUE;
}

/** \brief create an alarm
 *
 * \param flag set alarm mode e.g: RT_ALARM_DAILY
 * \param setup pointer to setup infomation
 */
rt_alarm_t rt_alarm_create(rt_alarm_callback_t callback, struct rt_alarm_setup *setup)
{
    struct rt_alarm *alarm;

    if (setup == RT_NULL)
        return (RT_NULL);

    if (!is_valid_alarm(*setup))
        return (RT_NULL);

    alarm = rt_malloc(sizeof(struct rt_alarm));
    if (alarm == RT_NULL)
        return (RT_NULL);
    rt_list_init(&alarm->list);

    alarm->wktime = setup->wktime;
    alarm->wktime.tm_year -= 1900;
    alarm->wktime.tm_mon -= 1;
    alarm->flag = setup->flag & 0xFF00;
    alarm->callback = callback;
    rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
    rt_list_insert_after(&_container.head, &alarm->list);
    rt_mutex_release(&_container.mutex);

    return (alarm);
}

/** \brief rtc alarm service thread entry
 *
 */
static void rt_alarmsvc_thread_init(void *param)
{
    rt_uint32_t recv;

    _container.current = RT_NULL;

    while (1)
    {
        if (rt_event_recv(&_container.event, 0xFFFF,
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, &recv) == RT_EOK)
        {
            alarm_update(recv);
        }
    }
}

/** \brief initialize alarm service system
 *
 * \param none
 * \return none
 */
void rt_alarm_system_init(void)
{
    rt_thread_t tid;

    rt_list_init(&_container.head);
    rt_event_init(&_container.event, "alarmsvc", RT_IPC_FLAG_FIFO);
    rt_mutex_init(&_container.mutex, "alarmsvc", RT_IPC_FLAG_FIFO);

    tid = rt_thread_create("alarmsvc",
                           rt_alarmsvc_thread_init, RT_NULL,
                           1024, 8, 1);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

int rt_alarm_init(void)
{
    rt_alarm_system_init();

    return RT_EOK;
}
INIT_PREV_EXPORT(rt_alarm_init);

#if defined(RT_USING_FINSH) && defined(RT_USING_ALARM_CMD)
void alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    rt_kprintf("Alarm is ringing: %s", asctime(&alarm->wktime));
}

static int alarm_add(uint16_t year, uint8_t month, uint8_t day,
                     uint8_t hour, uint8_t min, uint8_t sec)
{
    struct rt_alarm_setup alarm_setup;
    struct rt_alarm *alarm;
    int ret;

    rt_memset(&alarm_setup.wktime, RT_ALARM_TM_NOW, sizeof(struct tm));
    alarm_setup.flag = RT_ALARM_ONESHOT;
    alarm_setup.wktime.tm_year = year;
    alarm_setup.wktime.tm_mon  = month;
    alarm_setup.wktime.tm_mday = day;
    alarm_setup.wktime.tm_hour = hour;
    alarm_setup.wktime.tm_min  = min;
    alarm_setup.wktime.tm_sec  = sec;

    alarm = rt_alarm_create(alarm_callback, &alarm_setup);
    if (!alarm)
    {
        rt_kprintf("alarm create failed\n");
        return -1;
    }

    ret = rt_alarm_start(alarm);
    if (ret)
    {
        rt_kprintf("alarm start failed, ret=%d\n", ret);
        return -1;
    }

    return RT_EOK;
}

static void alarm(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        struct rt_alarm *alm;
        rt_list_t *next;

        rt_mutex_take(&_container.mutex, RT_WAITING_FOREVER);
        if (_container.current == RT_NULL)
        {
            rt_kprintf("No alarm\n");
            rt_mutex_release(&_container.mutex);
            return;
        }

        rt_kprintf("Alarm list:\n");
        for (next = _container.head.next; next != &_container.head; next = next->next)
        {
            alm = rt_list_entry(next, struct rt_alarm, list);
            rt_kprintf("    %s %s", (alm == _container.current) ? "*" : " ",
                       asctime(&alm->wktime));
        }
        rt_mutex_release(&_container.mutex);
    }
    else if (argc >= 7)
    {
        /* set time and date */
        uint16_t year;
        uint8_t month, day, hour, min, sec;
        year = atoi(argv[1]);
        month = atoi(argv[2]);
        day = atoi(argv[3]);
        hour = atoi(argv[4]);
        min = atoi(argv[5]);
        sec = atoi(argv[6]);
        if (year > 2099 || year < 2000)
        {
            rt_kprintf("year is out of range [2000-2099]\n");
            return;
        }
        if (month == 0 || month > 12)
        {
            rt_kprintf("month is out of range [1-12]\n");
            return;
        }
        if (day == 0 || day > 31)
        {
            rt_kprintf("day is out of range [1-31]\n");
            return;
        }
        if (hour > 23)
        {
            rt_kprintf("hour is out of range [0-23]\n");
            return;
        }
        if (min > 59)
        {
            rt_kprintf("minute is out of range [0-59]\n");
            return;
        }
        if (sec > 59)
        {
            rt_kprintf("second is out of range [0-59]\n");
            return;
        }
        alarm_add(year, month, day, hour, min, sec);
    }
    else
    {
        rt_kprintf("please input: alarm [year month day hour min sec] or date\n");
        rt_kprintf("e.g: alarm 2018 01 01 23 59 59 or date\n");
    }
}

MSH_CMD_EXPORT(alarm, get alarm date and time or set [year month day hour min sec]);
#endif /* defined(RT_USING_FINSH) && defined(RT_USING_ALARM_CMD) */

#endif
