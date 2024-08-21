/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_keyctrl.c
  * @author  Simon Xue
  * @version V0.1
  * @date    19-Jun-2019
  * @brief   keyctrl driver
  *
  ******************************************************************************
  */
#include <rtdevice.h>
#include <rthw.h>

#if defined(RT_USING_KEYCTRL)
#include "hal_base.h"
#include "drv_keyctrl.h"

#define KEYCTRL_BUFFER_SIZE     32
#define KEYCTRL_DET_TH          0xa
#define KEYCTRL_CAL_PERIOD_TH   0x3a980
#define KEYCTRL_FIL_TH          0xa

#define KEYCTRL_KEY_MAP_CODE0   0x1
#define KEYCTRL_KEY_MAP_CODE1   0x2
#define KEYCTRL_KEY_MAP_CODE2   0x3
#define KEYCTRL_KEY_MAP_CODE3   0x4
#define KEYCTRL_KEY_MAP_CODE4   0x5
#define KEYCTRL_KEY_MAP_CODE5   0x6
#define KEYCTRL_KEY_MAP_CODE6   0x7

#define KEYCTRL_KEY_MAP_VAL0    0x2d90
#define KEYCTRL_KEY_MAP_VAL1    0x1875
#define KEYCTRL_KEY_MAP_VAL2    0xcd8
#define KEYCTRL_KEY_MAP_VAL3    0x6a0
#define KEYCTRL_KEY_MAP_VAL4    0x375
#define KEYCTRL_KEY_MAP_VAL5    0x1c5
#define KEYCTRL_KEY_MAP_VAL6    0xf0

#define KEYCTRL_KEY_MAP_LONG_TIME_CODE0   0x8
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE1   0x9
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE2   0xa
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE3   0xb
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE4   0xc
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE5   0xd
#define KEYCTRL_KEY_MAP_LONG_TIME_CODE6   0xe

#define KEYCTRL_KEY_MAP_COMBINATION_VAL0    (KEYCTRL_KEY_MAP_VAL0 + KEYCTRL_KEY_MAP_VAL1)
#define KEYCTRL_KEY_MAP_COMBINATION_CODE0   0xf

/* According to test, it need about 8~10 timer callback
 * to get zero key value after keyctrl interrupt triggerd.
 * So we take 0x14 as a long time press threshold
 */
#define KEYCTRL_LONG_TIME_PRESS_THRESHOLD   0x14
#define KEYCTRL_LONG_TIME_PRESS_REPEAT      0x10

#define KEYCTRL_KEY_RANGE_DIV               0x5

struct rt_keyctrl_map
{
    rt_uint32_t value;
    rt_uint8_t keycode;
};

struct rt_keyctrl_long_time_press_map
{
    rt_uint8_t keycode;
    rt_uint8_t long_time_press_key_code;
};

struct rt_keyctrl_dev
{
    struct rt_device dev;
    struct rt_keyctrl_info info;

    rt_uint8_t  buffer[KEYCTRL_BUFFER_SIZE];
    rt_uint32_t  read_index, save_index;

    rt_uint32_t  first_value;
    rt_uint32_t delay_count;
    rt_uint32_t key_release;
    rt_timer_t key_scan_timer;
    rt_uint8_t long_time_press_key_code;
    rt_uint8_t combination_key_code;
};

static struct rt_keyctrl_dev *keyctrl_dev = RT_NULL;

static const struct rt_keyctrl_map key_map[] =
{
    {
        KEYCTRL_KEY_MAP_VAL0, KEYCTRL_KEY_MAP_CODE0,
    },
    {
        KEYCTRL_KEY_MAP_VAL1, KEYCTRL_KEY_MAP_CODE1,
    },
    {
        KEYCTRL_KEY_MAP_VAL2, KEYCTRL_KEY_MAP_CODE2,
    },
    {
        KEYCTRL_KEY_MAP_VAL3, KEYCTRL_KEY_MAP_CODE3,
    },
    {
        KEYCTRL_KEY_MAP_VAL4, KEYCTRL_KEY_MAP_CODE4,
    },
    {
        KEYCTRL_KEY_MAP_VAL5, KEYCTRL_KEY_MAP_CODE5,
    },
    {
        KEYCTRL_KEY_MAP_VAL6, KEYCTRL_KEY_MAP_CODE6,
    },
};

static const struct rt_keyctrl_long_time_press_map long_time_press_key_map[] =
{
    {
        KEYCTRL_KEY_MAP_CODE0, KEYCTRL_KEY_MAP_LONG_TIME_CODE0,
    },
    {
        KEYCTRL_KEY_MAP_CODE1, KEYCTRL_KEY_MAP_LONG_TIME_CODE1,
    },
    {
        KEYCTRL_KEY_MAP_CODE2, KEYCTRL_KEY_MAP_LONG_TIME_CODE2,
    },
    {
        KEYCTRL_KEY_MAP_CODE3, KEYCTRL_KEY_MAP_LONG_TIME_CODE3,
    },
    {
        KEYCTRL_KEY_MAP_CODE4, KEYCTRL_KEY_MAP_LONG_TIME_CODE4,
    },
    {
        KEYCTRL_KEY_MAP_CODE5, KEYCTRL_KEY_MAP_LONG_TIME_CODE5,
    },
    {
        KEYCTRL_KEY_MAP_CODE6, KEYCTRL_KEY_MAP_LONG_TIME_CODE6,
    },
};

static rt_bool_t rt_keyctrl_check_range(rt_uint32_t key_value, rt_uint32_t target)
{
    return ((key_value > (target - target / KEYCTRL_KEY_RANGE_DIV)) &&
            (key_value < (target + target / KEYCTRL_KEY_RANGE_DIV)));
}

static rt_uint8_t rt_keyctrl_map_long_time_keycode(rt_uint8_t keycode)
{
    rt_uint32_t i;

    for (i = 0; i < HAL_ARRAY_SIZE(long_time_press_key_map); i++)
    {
        if (long_time_press_key_map[i].keycode == keycode)
            return long_time_press_key_map[i].long_time_press_key_code;
    }

    return 0;
}

static rt_uint8_t rt_keyctrl_map_keycode(rt_uint32_t key_value)
{
    rt_uint32_t i;
    rt_uint32_t diff, closest = 0xffffffff;
    rt_uint8_t keycode = 0;

    for (i = 0; i < HAL_ARRAY_SIZE(key_map); i++)
    {
        diff = abs(key_map[i].value - key_value);
        if (diff < closest)
        {
            closest = diff;
            keycode = key_map[i].keycode;
        }
    }

    return keycode;
}

static void rt_keyctrl_save_key_code(char key_code)
{
    rt_base_t level;

    level = rt_hw_interrupt_disable();

    keyctrl_dev->buffer[keyctrl_dev->save_index] = key_code;
    keyctrl_dev->save_index++;
    if (keyctrl_dev->save_index >= KEYCTRL_BUFFER_SIZE)
        keyctrl_dev->save_index = 0;

    /* If the next position is read index, discard this 'read char' */
    if (keyctrl_dev->save_index == keyctrl_dev->read_index)
    {
        keyctrl_dev->read_index++;
        if (keyctrl_dev->read_index >= KEYCTRL_BUFFER_SIZE)
            keyctrl_dev->read_index = 0;
    }

    rt_hw_interrupt_enable(level);
}

static void rt_keyctrl_ind_callback(void)
{
    if (keyctrl_dev->dev.rx_indicate != RT_NULL)
    {
        rt_size_t rx_length;

        /* get rx length */
        rx_length = keyctrl_dev->read_index > keyctrl_dev->save_index ?
                    KEYCTRL_BUFFER_SIZE - keyctrl_dev->read_index + keyctrl_dev->save_index :
                    keyctrl_dev->save_index - keyctrl_dev->read_index;

        keyctrl_dev->dev.rx_indicate(&keyctrl_dev->dev, rx_length);
    }
}

static void rt_keyctrl_scan_timer_func(void *parameter)
{
    rt_uint32_t key_value = HAL_KeyCtrl_GetValue(KEY_CTRL);
    rt_uint8_t key_code = 0;

    if (key_value != 0)
    {
        keyctrl_dev->delay_count++;
        if ((keyctrl_dev->delay_count > KEYCTRL_LONG_TIME_PRESS_THRESHOLD) &&
                ((keyctrl_dev->delay_count - KEYCTRL_LONG_TIME_PRESS_THRESHOLD) % KEYCTRL_LONG_TIME_PRESS_REPEAT == 0))
        {
            /* Check if it is the combination key value.
             * Just save key code when combination key action released.
             */
            if (rt_keyctrl_check_range(key_value, KEYCTRL_KEY_MAP_COMBINATION_VAL0))
            {
                keyctrl_dev->combination_key_code = KEYCTRL_KEY_MAP_COMBINATION_CODE0;
            }
            /* Check if it is single key long time press.
             * Save key code each KEYCTRL_LONG_TIME_PRESS_REPEAT timer sample.
             */
            else
            {
                /* If one key still pressed after combination key released,
                 * don't send the "single long time press key code".
                 */
                if (keyctrl_dev->combination_key_code == 0)
                {
                    key_code = rt_keyctrl_map_keycode(key_value);
                    if (key_code == rt_keyctrl_map_keycode(keyctrl_dev->first_value))
                    {
                        keyctrl_dev->long_time_press_key_code = rt_keyctrl_map_long_time_keycode(key_code);
                        rt_keyctrl_save_key_code(keyctrl_dev->long_time_press_key_code);
                        rt_keyctrl_ind_callback();
                    }
                }
            }
            /* We don't need to check the uncomplete sampled value, just ignore them. */
        }
    }
    else
    {
        if (keyctrl_dev->combination_key_code == 0 && keyctrl_dev->long_time_press_key_code == 0)
        {
            key_code = rt_keyctrl_map_keycode(keyctrl_dev->first_value);
        }
        else if (keyctrl_dev->combination_key_code)
        {
            key_code = keyctrl_dev->combination_key_code;
            keyctrl_dev->combination_key_code = 0;
        }

        if (key_code)
        {
            rt_keyctrl_save_key_code(key_code);
        }

        rt_keyctrl_ind_callback();

        keyctrl_dev->long_time_press_key_code = 0;
        keyctrl_dev->delay_count = 0;
        keyctrl_dev->key_release = 1;
        rt_timer_stop(keyctrl_dev->key_scan_timer);
    }
}

static void rt_keyctrl_irqhandler(void)
{
    rt_interrupt_enter();

    if (keyctrl_dev->key_release)
    {
        keyctrl_dev->key_release = 0;
        keyctrl_dev->first_value = HAL_KeyCtrl_GetValue(KEY_CTRL);
        rt_timer_start(keyctrl_dev->key_scan_timer);
    }

    HAL_KeyCtrl_ClearInt(KEY_CTRL);

    rt_interrupt_leave();
}

static rt_err_t rt_keyctrl_init(rt_device_t dev)
{
    if (!(dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
        if (dev->flag & RT_DEVICE_FLAG_INT_RX)
        {
            rt_memset(keyctrl_dev->buffer, 0, sizeof(keyctrl_dev->buffer));
            keyctrl_dev->read_index = keyctrl_dev->save_index = 0;
        }

        dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
    }

    HAL_KeyCtrl_Init(KEY_CTRL, KEYCTRL_DET_TH, KEYCTRL_CAL_PERIOD_TH, KEYCTRL_FIL_TH);

    HAL_NVIC_SetIRQHandler(KEY_CTRL_IRQn, rt_keyctrl_irqhandler);
    HAL_NVIC_EnableIRQ(KEY_CTRL_IRQn);

    if (!keyctrl_dev->key_scan_timer)
    {
        keyctrl_dev->key_scan_timer = rt_timer_create("key_scan", rt_keyctrl_scan_timer_func, RT_NULL,
                                      10, RT_TIMER_FLAG_PERIODIC);
        if (!keyctrl_dev->key_scan_timer)
            return RT_ERROR;
    }

    return RT_EOK;
}

static rt_size_t rt_keyctrl_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_uint8_t *ptr;
    rt_err_t err_code;
    rt_base_t level;

    ptr = buffer;
    err_code = RT_EOK;

    /* Interrupt mode Rx */
    while (size)
    {
        if (keyctrl_dev->read_index != keyctrl_dev->save_index)
        {
            *ptr++ = keyctrl_dev->buffer[keyctrl_dev->read_index];
            size--;

            level = rt_hw_interrupt_disable();

            keyctrl_dev->read_index++;
            if (keyctrl_dev->read_index >= KEYCTRL_BUFFER_SIZE)
                keyctrl_dev->read_index = 0;

            rt_hw_interrupt_enable(level);
        }
        else
        {
            err_code = -RT_EEMPTY;
            break;
        }
    }

    rt_set_errno(err_code);
    return (rt_uint32_t)ptr - (rt_uint32_t)buffer;
}

static rt_err_t rt_keyctrl_control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
    case RT_KEYCTRL_CTRL_GET_INFO:
        rt_memcpy(args, &keyctrl_dev->info, sizeof(struct rt_keyctrl_info));
        break;
    default:
        break;
    }
    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops keyctrl_dev_ops =
{
    rt_keyctrl_init,
    RT_NULL,
    RT_NULL,
    rt_keyctrl_read,
    RT_NULL,
    rt_keyctrl_control
};
#endif

#if defined(RT_USING_PM)
static int rt_keyctl_pm_suspend(const struct rt_device *device)
{
    return RT_EOK;
}

static void rt_keyctl_pm_resume(const struct rt_device *device)
{
    HAL_KeyCtrl_Init(KEY_CTRL, KEYCTRL_DET_TH, KEYCTRL_CAL_PERIOD_TH, KEYCTRL_FIL_TH);
}

static struct rt_device_pm_ops rk_keyctl_pm_ops =
{
    .suspend = rt_keyctl_pm_suspend,
    .resume = rt_keyctl_pm_resume,
};
#endif

static int rt_keyctrl_dev_init(void)
{
    rt_err_t ret;

    ret = HAL_CRU_ClkEnable(CLK_KEY_GATE);
    if (ret)
        return -RT_EINVAL;
    ret = HAL_CRU_ClkEnable(PCLK_KEY_GATE);
    if (ret)
        goto clk_key_disable;

    keyctrl_dev = (struct rt_keyctrl_dev *)rt_calloc(1, sizeof(struct rt_keyctrl_dev));
    if (keyctrl_dev == RT_NULL)
    {
        ret = -RT_ENOMEM;
        goto pclk_key_disable;
    }

    keyctrl_dev->info.count = KEYCTRL_BUFFER_SIZE;
    keyctrl_dev->dev.type = RT_Device_Class_Char;

#ifdef RT_USING_DEVICE_OPS
    keyctrl_dev->dev.ops = &keyctrl_dev_ops;
#else
    keyctrl_dev->dev.init = rt_keyctrl_init;
    keyctrl_dev->dev.read = rt_keyctrl_read;
    keyctrl_dev->dev.control = rt_keyctrl_control;
#endif

    keyctrl_dev->key_release = 1;

#if defined(RT_USING_PM)
    rt_pm_register_device(&keyctrl_dev->dev, &rk_keyctl_pm_ops);
#endif

    rt_device_register(&(keyctrl_dev->dev), "keyctrl", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    return RT_EOK;

pclk_key_disable:
    HAL_CRU_ClkDisable(PCLK_KEY_GATE);
clk_key_disable:
    HAL_CRU_ClkDisable(CLK_KEY_GATE);

    return ret;
}

INIT_DEVICE_EXPORT(rt_keyctrl_dev_init);

#endif
