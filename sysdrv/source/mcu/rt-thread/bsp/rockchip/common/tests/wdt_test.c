/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    wdt_test.c
  * @author  Simon Xue
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   wdt test
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_WDT
#include "hal_base.h"

void keep_timer_func(void *parameter)
{
    HAL_WDT_KeepAlive();
}

static int wdt_test(int argc, char **argv)
{
    static rt_device_t wdt_dev = RT_NULL;
    rt_err_t ret;
    rt_uint32_t top = 10;
    static rt_timer_t keep_timer = RT_NULL;
    rt_uint32_t type;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc == 3)
            {
                wdt_dev = rt_device_find("dw_wdt");
                RT_ASSERT(wdt_dev != RT_NULL);

                ret = rt_device_init(wdt_dev);
                if (ret != RT_EOK)
                {
                    rt_kprintf("dev : %s, init failed, error : %d\n", wdt_dev->parent.name, ret);
                    return ret;
                }

                /* create 1/8 second timer */
                if (!keep_timer)
                {
                    keep_timer = rt_timer_create("keep", keep_timer_func, RT_NULL,
                                                 RT_TICK_PER_SECOND / 8, RT_TIMER_FLAG_PERIODIC);
                    if (!keep_timer)
                        return RT_ERROR;

                    rt_timer_start(keep_timer);
                }
            }
            else
            {
                rt_kprintf("wdt_test probe <wdt_name>   - probe wdt by name\n");
            }
        }
        else
        {
            if (wdt_dev == RT_NULL)
            {
                rt_kprintf("Please using 'wdt_test probe <wdt_name>' first\n");
                return -RT_ERROR;
            }
            if (!strcmp(argv[1], "settimeout"))
            {
                if (argc != 3)
                {
                    rt_kprintf("Please using 'wdt_test settimeout n\n");
                    return -RT_ERROR;
                }
                top = strtol(argv[2], NULL, 0);
                rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &top);
            }
            else if (!strcmp(argv[1], "start"))
            {
                if (!strcmp(argv[2], "1"))
                    type = 1;
                else
                    type = 0;
                rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_START, &type);
            }
            else if (!strcmp(argv[1], "stop"))
            {
                rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_STOP, RT_NULL);
            }
            else if (!strcmp(argv[1], "keep_alive"))
            {
                rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, RT_NULL);
            }
            else if (!strcmp(argv[1], "reboot"))
            {
                rt_timer_stop(keep_timer);
                rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, RT_NULL);
            }
        }
    }
    else
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("wdt_test probe <wdt_name>   - probe wdt by name\n");
        rt_kprintf("wdt_test settimeout n - set time out to n second\n");
        rt_kprintf("wdt_test start type  - start watchdog, type: 1 -interrupt mode, 0 -normal mode \n");
        rt_kprintf("wdt_test stop  - stop watchdog\n");
        rt_kprintf("wdt_test keep_alive  - kepp alive watchdog\n");
        rt_kprintf("wdt_test reboot   - reboot system\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}

int _at_wdt_test(void)
{
    static rt_device_t wdt_dev = RT_NULL;
    rt_err_t ret;
    rt_uint32_t top = 10;
    rt_uint32_t mode = 0; /* normal mode */
    rt_uint32_t tl1 = 0;
    rt_uint32_t tl2 = 0;

    wdt_dev = rt_device_find("dw_wdt");
    RT_ASSERT(wdt_dev != RT_NULL);

    ret = rt_device_init(wdt_dev);
    if (ret != RT_EOK)
    {
        rt_kprintf("dev : %s, init failed, error : %d\n", wdt_dev->parent.name, ret);
        return ret;
    }

    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &top);
    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_START, &mode);

    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_GET_TIMELEFT, &tl1);
    rt_thread_delay(100);
    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_GET_TIMELEFT, &tl2);

    if (tl1 != tl2)
    {
        rt_kprintf("%s finished\n", __func__);
        ret = RT_EOK;
    }
    else
    {
        rt_kprintf("%s failed\n", __func__);
        ret = -RT_ERROR;
    }

    rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_STOP, NULL);

    return ret;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(wdt_test, wdt drive test. e.g: wdt_test);
FINSH_FUNCTION_EXPORT(_at_wdt_test, watchdog test for auto test);
#endif

#endif
