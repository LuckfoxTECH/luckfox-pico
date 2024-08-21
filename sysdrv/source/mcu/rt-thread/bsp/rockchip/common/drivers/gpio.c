/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    gpio.c
  * @author
  * @version V0.1
  * @date    2019/10/25
  * @brief   set gpio mux/direction/data
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_DEBUG_USING_GPIO

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal_base.h"

#define GPIO(x)     GPIO##x

static void usage(void)
{
    rt_kprintf("GPIO utility - $Revision: 1.0 $\n\n");
    rt_kprintf("gpio <bank> <pin> <i|o|h|l>\n\n");
    rt_kprintf("    bank       gpio bank id, '0' for GPIO0, '1' for GPIO1\n");
    rt_kprintf("    pin        gpio pin id, rang from 0~31\n");
    rt_kprintf("    i          input\n");
    rt_kprintf("    o          output\n");
    rt_kprintf("    h          output high level\n");
    rt_kprintf("    l          output low level\n\n");
    rt_kprintf("Examples:\n");
    rt_kprintf("    gpio 0 2 i       # set gpio0-2 to input\n");
    rt_kprintf("    gpio 0 2 h       # set gpio0-2 to output high\n");
}

static int gpio_set(int argc, char **argv)
{
    struct GPIO_REG *pGPIO = NULL;
    char *endptr;
    int bank, pin;
    char *val;

    opterr = 0;
    optind = 0;

    optind++;
    bank = strtoul(argv[optind], &endptr, 0);
    if (*endptr)
    {
        rt_kprintf("Bad <bank> value '%s'\n", argv[optind]);
        usage();
        return 0;
    }

    switch (bank)
    {
#ifdef GPIO0
    case 0:
        pGPIO = GPIO(0);
        break;
#endif
#ifdef GPIO1
    case 1:
        pGPIO = GPIO(1);
        break;
#endif
#ifdef GPIO2
    case 2:
        pGPIO = GPIO(2);
        break;
#endif
#ifdef GPIO3
    case 3:
        pGPIO = GPIO(3);
        break;
#endif
#ifdef GPIO4
    case 4:
        pGPIO = GPIO(4);
        break;
#endif
    default:
        rt_kprintf("Bad <GPIOx> value '%s'\n", argv[optind]);
        usage();
        break;
    }

    optind++;
    pin = strtoul(argv[optind], &endptr, 0);
    if (*endptr)
    {
        rt_kprintf("Bad <pin> value '%s'\n", argv[optind]);
        usage();
        return 0;
    }

    optind++;
    val = argv[optind];
    if (!strcmp(val, "i"))
    {
        rt_pin_mode(BANK_PIN(bank, pin), PIN_MODE_INPUT);
    }
    else if (!strcmp(val, "o"))
    {
        rt_pin_mode(BANK_PIN(bank, pin), PIN_MODE_OUTPUT);
    }
    else if (!strcmp(val, "h"))
    {
        rt_pin_mode(BANK_PIN(bank, pin), PIN_MODE_OUTPUT);
        rt_pin_write(BANK_PIN(bank, pin), GPIO_HIGH);
    }
    else if (!strcmp(val, "l"))
    {
        rt_pin_mode(BANK_PIN(bank, pin), PIN_MODE_OUTPUT);
        rt_pin_write(BANK_PIN(bank, pin), GPIO_LOW);
    }
    else
    {
        rt_kprintf("Bad <Direction/Data> value '%s'\n", argv[optind]);
        usage();
    }

    return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(gpio_set, gpio, gpio set mux / direction / data);
#endif
#endif
