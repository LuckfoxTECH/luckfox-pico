/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    gpio_test.c
  * @author  Jay Xu
  * @version V0.1
  * @date    2019/5/15
  * @brief   gpio test
  *
  ******************************************************************************
  */

#include <rtthread.h>
#include <rtdevice.h>

#if defined(RT_USING_COMMON_TEST_GPIO) && defined(RKMCU_RK2108)

#include <unistd.h>
#include <stdlib.h>
#include "hal_base.h"

static rt_thread_t tgpio_thread;

struct TGPIO_INFO
{
    char *desc;
    struct GPIO_REG *gpio;
    rt_uint32_t mask;
};

struct TGPIO_INFO gpios[] =
{
    { "gpio0", GPIO0, 0x00000000 },
#ifdef GPIO1
    { "gpio1", GPIO1, 0x00000000 },
#endif
#ifdef GPIO2
    { "gpio2", GPIO2, 0x00000000 },
#endif
#ifdef GPIO3
    { "gpio3", GPIO3, 0x00000000 },
#endif
};

#define GPIO_BANK_NUM (HAL_ARRAY_SIZE(gpios))

static HAL_Status tgpio_set_iomux(void)
{
    HAL_Status rc = HAL_OK;
    uint8_t bank = 0;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++)
    {
        rc = HAL_PINCTRL_SetIOMUX(bank, gpios[bank].mask, 0);
        HAL_ASSERT(rc == HAL_OK);
    }

    return rc;
}

static void tgpio_prepare(char *name, rt_uint32_t mask)
{
    uint8_t i = 0;

    for (i = 0; i < GPIO_BANK_NUM; i++)
    {
        if (!strcmp(gpios[i].desc, name))
        {
            gpios[i].mask |= mask;
            rt_kprintf("%s: %s to test pins: 0x%lx\n", __func__, name, mask);
        }
    }

    tgpio_set_iomux();
}

typedef void (*thread_fn_t)(void *);

static void tgpio_check_level(void *arg)
{
    uint32_t oldlevel[GPIO_BANK_NUM], newlevel[GPIO_BANK_NUM];
    uint8_t bank = 0;
    HAL_Status rc = HAL_OK;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++)
    {
        oldlevel[bank] = HAL_GPIO_GetBankLevel(gpios[bank].gpio) & gpios[bank].mask;
        rt_kprintf("GPIO%d pins level 0x%lx ...\n", bank, oldlevel[bank]);
    }

    while (rc == HAL_OK)
    {
        for (bank = 0; bank < GPIO_BANK_NUM; bank++)
        {
            newlevel[bank] = HAL_GPIO_GetBankLevel(gpios[bank].gpio) & gpios[bank].mask;
            HAL_ASSERT(rc == HAL_OK);
        }

        for (bank = 0; bank < GPIO_BANK_NUM; bank++)
        {
            uint8_t pin;

            for (pin = 0; pin < 32; pin++)
            {
                if ((oldlevel[bank] & (1 << pin)) != (newlevel[bank] & (1 << pin)))
                {
                    if (HAL_GPIO_GetPinDirection(gpios[bank].gpio, (1 << pin)) == GPIO_OUT)
                        continue;

                    rc = HAL_GPIO_SetPinsDirection(gpios[bank].gpio, (1 << pin), GPIO_OUT);
                    HAL_ASSERT(rc == HAL_OK);
                    rc = HAL_GPIO_SetPinsLevel(gpios[bank].gpio, (1 << pin), GPIO_LOW);
                    HAL_ASSERT(rc == HAL_OK);
                    rc = HAL_PINCTRL_SetParam(bank, (1 << pin), PIN_CONFIG_DRV_LEVEL3);
                    HAL_ASSERT(rc == HAL_OK);
                }
            }
        }
    }
}

static void tgpio_isr(void *arg)
{
    rt_kprintf("%s\n", __func__);
}

static void tgpio_check_irq(void)
{
    uint8_t bank = 0;
    uint8_t pin;
    uint32_t pin_info;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++)
    {
        for (pin = 0; pin < 32; pin++)
        {
            if (gpios[bank].mask & (1 << pin))
            {
                pin_info = BANK_PIN(bank, pin);
                rt_pin_attach_irq(pin_info, PIN_IRQ_MODE_RISING_FALLING, tgpio_isr, NULL);
                rt_pin_irq_enable(pin_info, PIN_IRQ_ENABLE);
            }
        }
    }
}

static void tgpio_thread_create(thread_fn_t entry)
{
    tgpio_thread = rt_thread_create(
                       "tgpio",
                       entry,
                       RT_NULL,
                       4096,
                       RT_THREAD_PRIORITY_MAX - 1,
                       100);

    if (tgpio_thread)
        rt_thread_startup(tgpio_thread);
}

static void tgpio_thread_exit(void)
{
    if (!tgpio_thread)
        return;

    if (tgpio_thread->stat != RT_THREAD_CLOSE)
    {
        rt_thread_delete(tgpio_thread);
    }
}

static HAL_Status tgpio_set_param(ePINCTRL_configParam param)
{
    HAL_Status rc = HAL_OK;
    rt_uint32_t bank = 0U;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++)
    {
        rc = HAL_PINCTRL_SetParam(bank, gpios[bank].mask, param);
        HAL_ASSERT(rc == HAL_OK);
    }

    return rc;
}

static HAL_Status tgpio_set_level(eGPIO_pinLevel pinLevel)
{
    HAL_Status rc = HAL_OK;
    rt_uint32_t bank = 0U;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++)
    {
        rc = HAL_GPIO_SetPinsLevel(gpios[bank].gpio, gpios[bank].mask, pinLevel);
        HAL_ASSERT(rc == HAL_OK);
    }

    return rc;
}

static HAL_Status tgpio_set_direction(eGPIO_pinDirection pinDir)
{
    HAL_Status rc = HAL_OK;
    rt_uint32_t bank = 0U;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++)
    {
        rc = HAL_GPIO_SetPinsDirection(gpios[bank].gpio, gpios[bank].mask, pinDir);
        HAL_ASSERT(rc == HAL_OK);
    }

    return rc;
}

static void tgpio_usage(void);

static void tgpio_main(int argc, char * *argv)
{
    char *ptr;

    switch (*argv[1])
    {
    case '0':
        tgpio_prepare("gpio0", strtoul(argv[2], &ptr, 0));
        break;
    case '1':
        tgpio_prepare("gpio1", strtoul(argv[2], &ptr, 0));
        break;
    case '2':
        tgpio_prepare("gpio2", strtoul(argv[2], &ptr, 0));
        break;
    case '3':
        tgpio_prepare("gpio3", strtoul(argv[2], &ptr, 0));
        break;
    case 'a':
        tgpio_set_direction(GPIO_IN);
        tgpio_set_param(PIN_CONFIG_PUL_NORMAL);
        break;
    case 'b':
        tgpio_set_direction(GPIO_IN);
        tgpio_set_param(PIN_CONFIG_PUL_UP);
        break;
    case 'c':
        tgpio_set_direction(GPIO_IN);
        tgpio_set_param(PIN_CONFIG_PUL_DOWN);
        break;
    case 'd':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_HIGH);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL0);
        break;
    case 'e':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_LOW);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL0);
        break;
    case 'f':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_HIGH);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL1);
        break;
    case 'g':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_LOW);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL1);
        break;
    case 'h':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_HIGH);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL2);
        break;
    case 'i':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_LOW);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL2);
        break;
    case 'j':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_HIGH);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL3);
        break;
    case 'k':
        tgpio_set_direction(GPIO_OUT);
        tgpio_set_level(GPIO_LOW);
        tgpio_set_param(PIN_CONFIG_DRV_LEVEL3);
        break;
    case 'r'://test interrupt
        tgpio_set_direction(GPIO_IN);
        tgpio_check_irq();
        break;
    case 't'://test level
        tgpio_set_direction(GPIO_IN);
        tgpio_set_param(PIN_CONFIG_PUL_NORMAL);
        tgpio_thread_exit();
        tgpio_thread_create((thread_fn_t)tgpio_check_level);
        break;
    case 'x'://exit
        tgpio_thread_exit();
        tgpio_set_direction(GPIO_IN);
        break;
    default:
        tgpio_usage();
    }
}

static void tgpio_usage(void)
{
    rt_kprintf("\ntgpio cmd help\n");
    rt_kprintf("tgpio 0|1|2|3 <value>\n");
    rt_kprintf("tgpio a|b|c|d|e|f|g|h|i|j|k\n");
    rt_kprintf("tgpio r|t|x\n");
    rt_kprintf("    0 <value>  Set GPIO0 test pins, example GPIO0_PIN_A1 tgpio 0 0x00000002\n");
    rt_kprintf("    1 <value>  Set GPIO1 test pins, example GPIO1_PIN_A1 tgpio 1 0x00000002\n");
    rt_kprintf("    2 <value>  Set GPIO2 test pins, example GPIO2_PIN_A1 tgpio 2 0x00000002\n");
    rt_kprintf("    3 <value>  Set GPIO3 test pins, example GPIO3_PIN_A1 tgpio 3 0x00000002\n");
    rt_kprintf("    a input, pull normal\n");
    rt_kprintf("    b input, pull up\n");
    rt_kprintf("    c input, pull down\n");
    rt_kprintf("    d output high, io drive strength level 0\n");
    rt_kprintf("    e output low , io drive strength level 0\n");
    rt_kprintf("    f output high, io drive strength level 1\n");
    rt_kprintf("    g output low , io drive strength level 1\n");
    rt_kprintf("    h output high, io drive strength level 2\n");
    rt_kprintf("    i output low , io drive strength level 2\n");
    rt_kprintf("    j output high, io drive strength level 3\n");
    rt_kprintf("    k output low , io drive strength level 3\n");
    rt_kprintf("Examples for RK2108:\n");
    rt_kprintf("    tgpio 0 0xffff03ff   Select GPIO0 pins except GPIO_PIN_B2,GPIO_PIN_B3,GPIO_PIN_B4,GPIO_PIN_B5,GPIO_PIN_B6,GPIO_PIN_B7\n");
    rt_kprintf("    tgpio 1 0xfffffff0   Select GPIO1 pins except GPIO_PIN_A0,GPIO_PIN_A1,GPIO_PIN_A2,GPIO_PIN_A3\n");
    rt_kprintf("    tgpio a              TestCase: input, pull normal\n");
    rt_kprintf("    tgpio r              Start thread to check pins interrupt\n");
    rt_kprintf("    tgpio t              Start thread to check pins level change\n");
    rt_kprintf("    tgpio x              Exit from check pins thread\n\n");
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(tgpio_main, tgpio, gpio test cmd);
#endif

#endif
