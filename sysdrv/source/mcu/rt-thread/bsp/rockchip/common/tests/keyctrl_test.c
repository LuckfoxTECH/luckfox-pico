/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    keyctrl_test.c
  * @author  Simon Xue
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   keyctrl test
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_KEYCTRL
#include "hal_base.h"
#include "drv_keyctrl.h"

static rt_err_t keyctrl_getchar_rx_ind(rt_device_t dev, rt_size_t size)
{
    uint8_t ch;
    rt_size_t i;
    rt_err_t ret;

    for (i = 0; i < size; i++)
    {
        /* read a char */
        if (rt_device_read(dev, 0, &ch, 1))
        {
            rt_kprintf("dev : %s read 0x%x\n", dev->parent.name, ch);
        }
        else
        {
            ret = rt_get_errno();
            rt_kprintf("dev : %s read error %d\n", dev->parent.name, ret);
        }
    }

    return RT_EOK;
}

static int keyctrl_test(int argc, char **argv)
{
    static rt_device_t keyctrl_dev = RT_NULL;
    rt_err_t result;
    static rt_uint8_t *read_buffer  = RT_NULL;
    struct rt_keyctrl_info info;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            keyctrl_dev = rt_device_find(argv[2]);
            RT_ASSERT(keyctrl_dev != RT_NULL);

            result = rt_device_control(keyctrl_dev, RT_KEYCTRL_CTRL_GET_INFO, &info);
            if (result != RT_EOK)
            {
                rt_kprintf("device : %s cmd RT_KEYCTRL_CTRL_GET_INFO failed\n",
                           keyctrl_dev->parent.name);
                return result;
            }

            read_buffer = rt_malloc(info.count);
            if (read_buffer == RT_NULL)
            {
                rt_kprintf("no memory for read_buffer!\r\n");
                return -RT_ENOMEM;
            }

            result = rt_device_init(keyctrl_dev);
            if (result != RT_EOK)
            {
                rt_kprintf("To initialize device:%s failed. The error code is %d\n",
                           keyctrl_dev->parent.name, result);
                return result;
            }

            result = rt_device_open(keyctrl_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
            if (result != RT_EOK)
            {
                rt_kprintf("To open device:%s failed. The error code is %d\n",
                           keyctrl_dev->parent.name, result);
                return result;
            }
        }
        else
        {
            if (keyctrl_dev == RT_NULL)
            {
                rt_kprintf("Please using 'keyctrl_test probe <keyctrl_name>' first\n");
                return -RT_ERROR;
            }

            if (!strcmp(argv[1], "read"))
            {
                if (argc == 2)
                {
                    result = rt_device_read(keyctrl_dev, 0, read_buffer, 1);
                    if (result != 1)
                    {
                        rt_kprintf("read device :%s failed, error: %d, result = %d\n",
                                   keyctrl_dev->parent.name, rt_get_errno(), result);
                        return -RT_ERROR;
                    }
                    rt_kprintf("device: %s read value is 0x%x\n",
                               keyctrl_dev->parent.name, read_buffer[0]);
                }
                else
                {
                    rt_kprintf("keyctrl     - read keyctrl value\n");
                }
            }
            else if (!strcmp(argv[1], "set_ind"))
            {
                if (!strcmp(argv[2], "null"))
                {
                    rt_device_set_rx_indicate(keyctrl_dev, NULL);
                }
                else
                {
                    rt_device_set_rx_indicate(keyctrl_dev, keyctrl_getchar_rx_ind);
                }
            }
            else if (!strcmp(argv[1], "close"))
            {
                rt_device_set_rx_indicate(keyctrl_dev, NULL);
                rt_device_close(keyctrl_dev);
                keyctrl_dev = NULL;
                rt_free(read_buffer);
            }
            else
            {
                rt_kprintf("Unknown command. Please enter 'keyctrl_test' for help\n");
            }
        }
    }
    else
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("keyctrl_test probe <keyctrl_name>   - probe keyctrl by name\n");
        rt_kprintf("keyctrl_test read     - read keyctrl value\n");
        rt_kprintf("keyctrl_test close     - close test, need probe to restart test\n");
        rt_kprintf("keyctrl_test set_ind callback     - set keyctrl indicate callback, set null to cancel\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}

static rt_timer_t timer;
static struct rt_event event;

void timer_func(void *parameter)
{
    rt_err_t result;
    char read_buffer = 0;
    rt_device_t keyctrl_dev = (rt_device_t)parameter;

    result = rt_device_read(keyctrl_dev, 0, &read_buffer, 1);
    if (result == RT_EOK)
    {
        if (read_buffer != 0)
        {
            rt_kprintf("keyctrl read value is 0x%x\n", read_buffer);
            rt_timer_stop(timer);
            rt_event_send(&event, 0x1);
        }
    }
}

int _at_keyctrl_test(void)
{
    static rt_device_t keyctrl_dev = RT_NULL;
    rt_err_t result;
    rt_uint32_t status;

    keyctrl_dev = rt_device_find("keyctrl");
    RT_ASSERT(keyctrl_dev != RT_NULL);

    result = rt_device_init(keyctrl_dev);
    if (result != RT_EOK)
        return -RT_ERROR;

    result = rt_device_open(keyctrl_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if (result != RT_EOK)
        return -RT_ERROR;

    if (!timer)
    {
        timer = rt_timer_create("keyctrl_read", timer_func, keyctrl_dev,
                                10, RT_TIMER_FLAG_PERIODIC);
        if (!timer)
            return RT_ERROR;

        rt_timer_start(timer);
    }

    rt_kprintf("start keyctrl auto test : \n");

    /* TODO : simulate a keyctrl event */

    /* wait keyctrl 2 seconds to trigger interrupt */
    if (rt_event_recv(&event, 0xffffffff, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      rt_tick_from_millisecond(2000), &status) != RT_EOK)
    {
        rt_kprintf("wait keyctrl timeout");
        rt_timer_stop(timer);
        return -RT_ETIMEOUT;
    }

    if (status != 0x1)
        return -RT_ERROR;

    return RT_EOK;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(keyctrl_test, keyctrl drive test. e.g: keyctrl_test);
FINSH_FUNCTION_EXPORT(_at_keyctrl_test, keyctrl test for auto test);
#endif

#endif
