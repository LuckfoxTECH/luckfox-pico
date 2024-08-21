/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816_rtc.c
  * @version V0.2
  * @brief   rtc interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-12-26     Joseph Chen     first implementation
  * 2020-07-22     Joseph Chen     Add alarm support
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <time.h>
#ifdef RT_USING_RTC_RK816
#include <drivers/alarm.h>
#include "hal_base.h"
#include "drv_pmic.h"
#include "rk816.h"
#include "rk816_interrupt.h"

/* #define RTC_VERBOSE */

#define BCD2BIN(n)  (((((n) >> 4) & 0x0F) * 10) + ((n) & 0x0F))
#define BIN2BCD(n)  ((((n) / 10) << 4) | ((n) % 10))

static struct pwr_i2c_desc *rtc_i2c;

/* Read current time and date in RTC */
static int rk816_rtc_read_time(struct tm *tm)
{
    struct rt_i2c_bus_device *bus = rtc_i2c->bus;
    rt_uint8_t addr = rtc_i2c->addr;
    rt_uint8_t rtc_data[NUM_TIME_REGS];
    int ret;

    ret = pmic_update_bits(bus, addr, RTC_CTRL_REG,
                           BIT_RTC_CTRL_REG_RTC_READSEL_M,
                           0);
    if (ret)
    {
        rt_kprintf("Failed to update bits rtc_ctrl: %d\n", ret);
        return ret;
    }

    rtc_data[0] = pmic_read(bus, addr, SECONDS_REG);
    rtc_data[1] = pmic_read(bus, addr, MINUTES_REG);
    rtc_data[2] = pmic_read(bus, addr, HOURS_REG);
    rtc_data[3] = pmic_read(bus, addr, DAYS_REG);
    rtc_data[4] = pmic_read(bus, addr, MONTHS_REG);
    rtc_data[5] = pmic_read(bus, addr, YEARS_REG);
    rtc_data[6] = pmic_read(bus, addr, WEEKS_REG);

    tm->tm_sec  =  BCD2BIN(rtc_data[0] & SECONDS_REG_MSK);
    tm->tm_min  =  BCD2BIN(rtc_data[1] & MINUTES_REG_MAK);
    tm->tm_hour =  BCD2BIN(rtc_data[2] & HOURS_REG_MSK);
    tm->tm_mday =  BCD2BIN(rtc_data[3] & DAYS_REG_MSK);
    tm->tm_mon  = (BCD2BIN(rtc_data[4] & MONTHS_REG_MSK)) - 1;
    tm->tm_year = (BCD2BIN(rtc_data[5] & YEARS_REG_MSK)) + 100;
    tm->tm_wday =  BCD2BIN(rtc_data[6] & WEEKS_REG_MSK);

#ifdef RTC_VERBOSE
    rt_kprintf("RTC read time %4d-%02d-%02d(%d) %02d:%02d:%02d\n",
               1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
               tm->tm_wday, tm->tm_hour, tm->tm_min, tm->tm_sec);
#endif

    return ret;
}

/* Set current time and date in RTC */
static int rk816_rtc_set_time(struct tm *tm)
{
    struct rt_i2c_bus_device *bus = rtc_i2c->bus;
    rt_uint8_t addr = rtc_i2c->addr;
    rt_uint8_t rtc_data[NUM_TIME_REGS];
    int ret;
#ifdef RTC_VERBOSE
    struct tm r_tm;

    rt_kprintf("RTC set time %4d-%02d-%02d(%d) %02d:%02d:%02d\n",
               1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
               tm->tm_wday, tm->tm_hour, tm->tm_min, tm->tm_sec);
#endif

    rtc_data[0] = BIN2BCD(tm->tm_sec);
    rtc_data[1] = BIN2BCD(tm->tm_min);
    rtc_data[2] = BIN2BCD(tm->tm_hour);
    rtc_data[3] = BIN2BCD(tm->tm_mday);
    rtc_data[4] = BIN2BCD(tm->tm_mon + 1);
    rtc_data[5] = BIN2BCD(tm->tm_year - 100);
    rtc_data[6] = BIN2BCD(tm->tm_wday);

    /* Stop RTC while updating the RTC registers */
    ret = pmic_update_bits(bus, addr, RTC_CTRL_REG,
                           BIT_RTC_CTRL_REG_STOP_RTC_M,
                           BIT_RTC_CTRL_REG_STOP_RTC_M);
    if (ret)
    {
        rt_kprintf("Failed to update RTC control: %d\n", ret);
        return ret;
    }

    pmic_write(bus, addr, SECONDS_REG, rtc_data[0]);
    pmic_write(bus, addr, MINUTES_REG, rtc_data[1]);
    pmic_write(bus, addr, HOURS_REG,   rtc_data[2]);
    pmic_write(bus, addr, DAYS_REG,    rtc_data[3]);
    pmic_write(bus, addr, MONTHS_REG,  rtc_data[4]);
    pmic_write(bus, addr, YEARS_REG,   rtc_data[5]);
    pmic_write(bus, addr, WEEKS_REG,   rtc_data[6]);

    /* Start RTC again */
    ret = pmic_update_bits(bus, addr, RTC_CTRL_REG,
                           BIT_RTC_CTRL_REG_STOP_RTC_M,
                           0);
    if (ret)
    {
        rt_kprintf("Failed to update RTC control: %d\n", ret);
        return ret;
    }

#ifdef RTC_VERBOSE
    rk816_rtc_read_time(&r_tm);
#endif

    return RT_EOK;
}

#ifdef RT_USING_ALARM

/* Read alarm time and date in RTC */
static int rk816_rtc_read_alarm(struct rt_rtc_wkalarm *wkalarm)
{
    struct rt_i2c_bus_device *bus = rtc_i2c->bus;
    struct tm *tm = &wkalarm->wktime;
    rt_uint8_t addr = rtc_i2c->addr;
    rt_uint8_t rtc_data[NUM_ALARM_REGS];

    rtc_data[0] = pmic_read(bus, addr, ALARM_SECONDS_REG);
    rtc_data[1] = pmic_read(bus, addr, ALARM_MINUTES_REG);
    rtc_data[2] = pmic_read(bus, addr, ALARM_HOURS_REG);
    rtc_data[3] = pmic_read(bus, addr, ALARM_DAYS_REG);
    rtc_data[4] = pmic_read(bus, addr, ALARM_MONTHS_REG);
    rtc_data[5] = pmic_read(bus, addr, ALARM_YEARS_REG);

    tm->tm_sec  =  BCD2BIN(rtc_data[0] & SECONDS_REG_MSK);
    tm->tm_min  =  BCD2BIN(rtc_data[1] & MINUTES_REG_MAK);
    tm->tm_hour =  BCD2BIN(rtc_data[2] & HOURS_REG_MSK);
    tm->tm_mday =  BCD2BIN(rtc_data[3] & DAYS_REG_MSK);
    tm->tm_mon  = (BCD2BIN(rtc_data[4] & MONTHS_REG_MSK)) - 1;
    tm->tm_year = (BCD2BIN(rtc_data[5] & YEARS_REG_MSK)) + 100;

#ifdef RTC_VERBOSE
    rt_kprintf("RTC read alarm %4d-%02d-%02d(*) %02d:%02d:%02d\n",
               1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
               tm->tm_hour, tm->tm_min, tm->tm_sec);
#endif

    return RT_EOK;
}

/* Set alarm time and date in RTC */
static int rk816_rtc_set_alarm(struct rt_rtc_wkalarm *wkalarm)
{
    struct rt_i2c_bus_device *bus = rtc_i2c->bus;
    struct tm *tm = &wkalarm->wktime;
    rt_uint8_t addr = rtc_i2c->addr;
    rt_uint8_t rtc_data[NUM_ALARM_REGS];
    int ret;

#ifdef RTC_VERBOSE
    struct rt_rtc_wkalarm r_wkalarm;

    rt_kprintf("RTC set  alarm %4d-%02d-%02d(*) %02d:%02d:%02d\n",
               1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
               tm->tm_hour, tm->tm_min, tm->tm_sec);
#endif

    rtc_data[0] = BIN2BCD(tm->tm_sec);
    rtc_data[1] = BIN2BCD(tm->tm_min);
    rtc_data[2] = BIN2BCD(tm->tm_hour);
    rtc_data[3] = BIN2BCD(tm->tm_mday);
    rtc_data[4] = BIN2BCD(tm->tm_mon + 1);
    rtc_data[5] = BIN2BCD(tm->tm_year - 100);

    /* Stop alarm while updating the alarm registers */
    ret = pmic_update_bits(bus, addr, RTC_INT_REG,
                           BIT_RTC_INT_REG_IT_ALARM_M, 0);
    if (ret)
    {
        rt_kprintf("Failed to update RTC int: %d\n", ret);
        return ret;
    }

    if (!wkalarm->enable)
        return RT_EOK;

    pmic_write(bus, addr, ALARM_SECONDS_REG, rtc_data[0]);
    pmic_write(bus, addr, ALARM_MINUTES_REG, rtc_data[1]);
    pmic_write(bus, addr, ALARM_HOURS_REG,   rtc_data[2]);
    pmic_write(bus, addr, ALARM_DAYS_REG,    rtc_data[3]);
    pmic_write(bus, addr, ALARM_MONTHS_REG,  rtc_data[4]);
    pmic_write(bus, addr, ALARM_YEARS_REG,   rtc_data[5]);

    /* Start alarm again */
    ret = pmic_update_bits(bus, addr, RTC_INT_REG,
                           BIT_RTC_INT_REG_IT_ALARM_M,
                           BIT_RTC_INT_REG_IT_ALARM_M);
    if (ret)
    {
        rt_kprintf("Failed to update RTC int: %d\n", ret);
        return ret;
    }

#ifdef RTC_VERBOSE
    rk816_rtc_read_alarm(&r_wkalarm);
#endif

    return RT_EOK;
}

void rtc_alarm_isr(void *args)
{
    rt_device_t dev = args;

    rt_alarm_update(dev, 1);
}
#endif

static rt_err_t rk816_rtc_init(void)
{
    struct rt_i2c_bus_device *bus = rtc_i2c->bus;
    rt_uint8_t addr = rtc_i2c->addr;
    rt_int32_t ret;

    /* start rtc running by default, and use shadowed timer. */
    ret = pmic_update_bits(bus, addr, RTC_CTRL_REG,
                           BIT_RTC_CTRL_REG_STOP_RTC_M,
                           0);
    if (ret)
    {
        rt_kprintf("Failed to update RTC control: %d\n", ret);
        return ret;
    }

    ret = pmic_write(bus, addr, RTC_STATUS_REG, RTC_STATUS_MASK);
    if (ret)
    {
        rt_kprintf("Failed to update RTC status: %d\n", ret);
        return ret;
    }

    /* set 24 hours mode */
    return pmic_update_bits(rtc_i2c->bus, rtc_i2c->addr, RTC_CTRL_REG,
                            BIT_RTC_CTRL_REG_RTC_AMPM_MODE, 0);
}

static rt_err_t rk816_rtc_control(rt_device_t dev, int cmd, void *args)
{
#ifdef RT_USING_ALARM
    struct rt_rtc_wkalarm wkalarm;
#endif
    struct tm tm, *tm_ptr;
    time_t *time;

    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_RTC_GET_TIME:
    {
        rk816_rtc_read_time(&tm);
        *((rt_time_t *)args) = mktime(&tm);
    }
    break;

    case RT_DEVICE_CTRL_RTC_SET_TIME:
    {
        time = (time_t *)args;
        tm_ptr = localtime(time);
        rk816_rtc_set_time(tm_ptr);
    }
    break;

#ifdef RT_USING_ALARM
    case RT_DEVICE_CTRL_RTC_GET_ALARM:
    {
        rk816_rtc_read_alarm(&wkalarm);
        *((struct rt_rtc_wkalarm *)args) = wkalarm;
    }
    break;

    case RT_DEVICE_CTRL_RTC_SET_ALARM:
    {
        rk816_rtc_set_alarm((struct rt_rtc_wkalarm *)args);
    }
    break;
#endif
    default:
        return RT_EINVAL;
    }

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops rtc_ops =
{
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rk816_rtc_control
};
#endif

int rt_rk816_rtc_init(void)
{
    static struct rt_device rtc_dev;
    int ret;

    rtc_i2c = pmic_get_i2c_desc();
    if (rtc_i2c == RT_NULL)
        return -RT_EINVAL;

    RT_ASSERT(!rt_device_find("rtc"));

    rtc_dev.type    = RT_Device_Class_RTC;
#ifdef RT_USING_DEVICE_OPS
    rtc_dev.ops     = &rtc_ops;
#else
    rtc_dev.init    = RT_NULL;
    rtc_dev.open    = RT_NULL;
    rtc_dev.close   = RT_NULL;
    rtc_dev.read    = RT_NULL;
    rtc_dev.write   = RT_NULL;
    rtc_dev.control = rk816_rtc_control;
#endif

    ret = rt_device_register(&rtc_dev, "rtc", RT_DEVICE_FLAG_RDWR);
    if (ret)
        return ret;

#ifdef RT_USING_ALARM
    rk816_irq_register(RK816_RTC_ALARM_INT_NUM, rtc_alarm_isr, &rtc_dev);
#endif

    return rk816_rtc_init();
}
INIT_DEVICE_EXPORT(rt_rk816_rtc_init);
#endif
