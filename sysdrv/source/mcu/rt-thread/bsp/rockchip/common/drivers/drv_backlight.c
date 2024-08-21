/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_backlight.c
  * @version V0.1
  * @brief   backlight driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-10     Chris Zhong     first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <stdbool.h>
#ifdef RT_USING_BACKLIGHT
#include "board.h"

#if defined(LCD_BACKLIGHT_REGULATOR)
#include "drv_regulator.h"
#endif

#define BL_MAX 100

int current_brightness;

static void backlight_on(bool on)
{
#if defined(LCD_BACKLIGHT_REGULATOR)
    struct regulator_desc *ldo;

    ldo = regulator_get_desc_by_pwrid(LCD_BACKLIGHT_REGULATOR);
    if (!ldo)
        return;

    if (on)
        regulator_enable(ldo);
    else
        regulator_disable(ldo);

#elif defined(LCD_BACKLIGHT_PWM)
    struct rt_device_pwm *pwm_dev = (struct rt_device_pwm *)rt_device_find(LCD_BACKLIGHT_PWM);
    rt_uint32_t period, pulse;
    rt_uint8_t brightness;

    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("%s failed! can't find %s device!\n", __func__, LCD_BACKLIGHT_PWM);
        return;
    }

    /* Restore backlight brightness or set it to 0 */
    if (on)
        brightness = current_brightness;
    else
        brightness = 0;

#ifdef LCD_BACKLIGHT_PWM_INVERT
    brightness = BL_MAX - brightness;
#endif

    period = 1000000;
    pulse = (BL_MAX - brightness) * period / BL_MAX;

    rt_pwm_set(pwm_dev, LCD_BACKLIGHT_PWM_CHANNEL, period, pulse);
#endif
}

static void backlight_update_brightness(rt_uint8_t brightness)
{
    if (brightness > BL_MAX)
        brightness = BL_MAX;

#if defined(LCD_BACKLIGHT_REGULATOR)
    struct regulator_desc *ldo;
    uint32_t vol;

    ldo = regulator_get_desc_by_pwrid(LCD_BACKLIGHT_REGULATOR);
    if (!ldo)
        return;

    vol = (LCD_BACKLIGHT_MAX_VOLT - LCD_BACKLIGHT_MIN_VOLT) * brightness / BL_MAX;
    vol += LCD_BACKLIGHT_MIN_VOLT;

    regulator_set_voltage(ldo, vol);

#elif defined(LCD_BACKLIGHT_PWM)
    struct rt_device_pwm *pwm_dev = (struct rt_device_pwm *)rt_device_find(LCD_BACKLIGHT_PWM);
    rt_uint32_t period, pulse;
    rt_uint8_t bl;

    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("%s failed! can't find %s device!\n", __func__, LCD_BACKLIGHT_PWM);
        return;
    }

#ifdef LCD_BACKLIGHT_PWM_INVERT
    bl = BL_MAX - brightness;
#else
    bl = brightness;
#endif

    period = 1000000;
    pulse = (BL_MAX - bl) * period / BL_MAX;

    rt_pwm_set(pwm_dev, LCD_BACKLIGHT_PWM_CHANNEL, period, pulse);

    /* The pwm only enable once */
    if (current_brightness == -1)
        rt_pwm_enable(pwm_dev, LCD_BACKLIGHT_PWM_CHANNEL);

    current_brightness = brightness;
#endif
}

static rt_err_t backlight_control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
        backlight_update_brightness(*(rt_uint8_t *)args);
        break;
    case RTGRAPHIC_CTRL_SET_MODE:
        /* TODO: */
        break;
    case RTGRAPHIC_CTRL_POWERON:
    case RTGRAPHIC_CTRL_POWEROFF:
        backlight_on(cmd == RTGRAPHIC_CTRL_POWERON);
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
        *(int *)args = current_brightness;
        break;
    default:
        rt_kprintf("Unsupport backlight_control control cmd: 0x%lx\n", cmd);
        break;
    }

    return 0;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops backlight_ops =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    backlight_control,
};
#endif

int rt_backlight_init(void)
{
    struct rt_device *rt_bl;

    rt_bl = rt_malloc(sizeof(struct rt_device));
    if (!rt_bl)
    {
        rt_kprintf("alloc backlight rt_device failed\n");
        return -RT_ENOMEM;
    }

    /* init rt_device structure */
    rt_bl->type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    rt_bl->ops = &backlight_ops;
#else
    rt_bl->init = NULL;
    rt_bl->open = NULL;
    rt_bl->close = NULL;
    rt_bl->read = NULL;
    rt_bl->write = NULL;
    rt_bl->control = backlight_control;
#endif
    current_brightness = -1;

    /* register rt_bl device to RT-Thread */
    rt_device_register(rt_bl, "backlight", RT_DEVICE_FLAG_RDWR);

    return 0;
}

INIT_DEVICE_EXPORT(rt_backlight_init);
#endif
