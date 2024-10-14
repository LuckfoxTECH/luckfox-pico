/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-21 11:17:00
 * @Company: Rockchip
 * @LastEditTime: 2020-06-02 15:47:48
 * @LastEditors: Do not edit
 * @Description:
 */

#include "pcba_app.h"
#if defined(RT_USING_PCBA_TEST)
#include "drv_keyctrl.h"
#include "math.h"

#define DBG_SECTION_NAME    "PCBA_KEY"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

#ifdef RT_USING_KEYCTRL
static rt_device_t keyctrl_dev = RT_NULL;
static char *read_buffer;

static int key_value[] =
{
    0,              // NONE
    KEY_FUNC1,      // FUN_1    press:0x01 long press: 0x08
    KEY_FUNC2,      // FUN_2    press:0x02 long press: 0x09
    KEY_FUNC3,      // FUN_3    press:0x03 long press: 0x0A
    KEY_FUNC4,      // FUN_4    press:0x04 long press: 0x0B
    KEY_FUNC5,      // FUN_5    press:0x05 long press: 0x0C
    KEY_FUNC5,      // FUN_6    press:0x06 long press: 0x0D
    KEY_FUNC5,      // FUN_7    press:0x07 long press: 0x0E
};

static int get_key_value(int rv)
{
    return key_value[rv];
}

static int pcba_key_device_init()
{
    rt_err_t result;
    struct rt_keyctrl_info info;

    keyctrl_dev = rt_device_find("keyctrl");
    RT_ASSERT(keyctrl_dev != RT_NULL);

    result = rt_device_control(keyctrl_dev, RT_KEYCTRL_CTRL_GET_INFO, &info);
    if (result != RT_EOK)
    {
        LOG_E("device : %s cmd RT_KEYCTRL_CTRL_GET_INFO failed",
              keyctrl_dev->parent.name);
        return result;
    }

    read_buffer = rt_malloc(info.count);
    if (read_buffer == RT_NULL)
    {
        LOG_E("no memory for read_buffer!");
        return -RT_ENOMEM;
    }

    result = rt_device_init(keyctrl_dev);
    if (result != RT_EOK)
    {
        LOG_E("To initialize device:%s failed. The error code is %d",
              keyctrl_dev->parent.name, result);
        return result;
    }

    result = rt_device_open(keyctrl_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if (result != RT_EOK)
    {
        LOG_E("To open device:%s failed. The error code is %d",
              keyctrl_dev->parent.name, result);
        return result;
    }

    return RT_EOK;
}

static int pcba_key_device_deinit()
{
    if (read_buffer)
        rt_free(read_buffer);
    rt_device_close(keyctrl_dev);

    return RT_EOK;
}
#endif

static void pcba_key_task(void *arg)
{
    struct pcba_app_msg send_msg;
    rt_err_t ret;

#ifdef RT_USING_KEYCTRL
    uint32_t release_count = 0;
    uint32_t key_table = 0;
    pcba_key_device_init();
#else
    int key_release = 1;
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_B6,
                         PIN_CONFIG_MUX_FUNC0);
#endif

    while (1)
    {
        rt_thread_delay(10);
#ifdef RT_USING_KEYCTRL
        ret = rt_device_read(keyctrl_dev, 0, read_buffer, 1);
        if (ret != 1)
        {
            /* all key release */
            if (!key_table)
                continue;
            /* WIP the flag release_count is only for long press */
            if ((key_table & 0xFF80) > 0)
            {
                release_count++;
                if (release_count < 20)
                    continue;
            }
            int value = (int)(log(key_table) / log(2));
            if (value > 7)
                send_msg.value = get_key_value(value - 7) | KEY_LONG_PRESS_START | KEY_RELEASE;
            else
                send_msg.value = get_key_value(value) | KEY_RELEASE;
            LOG_D("0x%08x release", send_msg.value);
            send_msg.type = TYPE_KEY_PRESS;
            // rt_mq_send(g_app_info.display_mq, (void *)&send_msg, sizeof(struct recpen_app_msg));
            key_table = 0;
            release_count = 0;
            continue;
        }
        release_count = 0;
        /* check if same key */
        if ((key_table & (1 << read_buffer[0])) > 0)
            continue;
        key_table |= 1 << read_buffer[0];
        if (read_buffer[0] > 7)
        {
            send_msg.value = get_key_value((int)read_buffer[0] - 7) | KEY_LONG_PRESS_START;
            LOG_D("0x%08x TYPE_KEY_LONGPRESS", send_msg.value);
        }
        else
        {
            send_msg.value = get_key_value((int)read_buffer[0]);
            LOG_D("0x%08x TYPE_KEY_PRESS", send_msg.value);
        }
        if (send_msg.value)
        {
            send_msg.type = TYPE_KEY_PRESS;
            ret = rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
            if (ret != RT_EOK)
            {
                // todo
            }
        }
#else
        if (HAL_GPIO_GetPinLevel(GPIO1, GPIO_PIN_B6) == GPIO_LOW && key_release)
        {
            key_release = 0;
            send_msg.type = TYPE_KEY_PRESS;
            send_msg.value = KEY_RECORD;
            ret = rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
            if (ret != RT_EOK)
            {
                // todo
            }
        }
        else if (HAL_GPIO_GetPinLevel(GPIO1, GPIO_PIN_B6) == GPIO_HIGH)
        {
            key_release = 1;
        }
#endif
    }

#ifdef RT_USING_KEYCTRL
    pcba_key_device_deinit();
#endif
}

void pcba_key_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("pcba_key",
                           pcba_key_task,
                           RT_NULL,
                           PCBA_APP_KEY_TASK_STACK_SIZE,
                           PCBA_APP_KEY_TASK_PRIORITY,
                           10);

    if (tid != RT_NULL)
    {
        LOG_D("Create pcba_key_task OK\n");
        rt_thread_startup(tid);
        return;
    }

    LOG_E("Create pcba_key_task failed\n");
    RT_ASSERT(0);
}

static void pcba_key_send_usage(char *arg)
{
    rt_kprintf("usage:\n\t%s <value>\n", arg);
    rt_kprintf("value:\n\tKEY_RECORD: 0x%08x\n", KEY_RECORD);
    rt_kprintf("\tKEY_PLAY:   0x%08x\n", KEY_PLAY);
    rt_kprintf("\tKEY_DOWN:   0x%08x\n", KEY_DOWN);
    rt_kprintf("\tKEY_UP:     0x%08x\n", KEY_UP);
    rt_kprintf("\tKEY_MENU:   0x%08x\n", KEY_MENU);
    rt_kprintf("\tKEY_OK:     0x%08x\n", KEY_OK);
    rt_kprintf("\tKEY_CANCEL: 0x%08x\n", KEY_CANCEL);
    rt_kprintf("\tKEY_SELECT: 0x%08x\n", KEY_SELECT);
}

void pcba_key_send(int argc, char *argv[])
{
    struct pcba_app_msg send_msg;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            pcba_key_send_usage(argv[0]);
        }
        else
        {
            send_msg.type = TYPE_KEY_PRESS;
            send_msg.value = strtol(argv[1], NULL, 0);
            rt_mq_send(g_pcba.display_mq, (void *)&send_msg, sizeof(struct pcba_app_msg));
        }
    }
    else
    {
        pcba_key_send_usage(argv[0]);
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(pcba_key_send, send key to pcba test thread);
#endif

#endif