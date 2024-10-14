/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    spi2apb_test.c
  * @author  David Wu
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   spi2apb test for pisces
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_SPI2APB

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../drivers/spi2apb.h"

void spi2apb_test_callback(rt_uint32_t value)
{
    rt_kprintf("spi2apb_test_callback reg1: 0x%x\n", value);
}

void spi2apb_test_show_usage()
{
    rt_kprintf("spi2apb_test cmd demo as following:\n\n");
    /* config spi2apb mode polarity */
    rt_kprintf("spi2apb_test config spi2apb 1 1\n");
    /* register spi2apb callback */
    rt_kprintf("spi2apb_test cb spi2apb\n");
    /* config spi2apb read reg0 */
    rt_kprintf("spi2apb_test read spi2apb 0\n");
    /* config spi2apb read reg1 */
    rt_kprintf("spi2apb_test read spi2apb 1\n");
    /* config spi2apb query status */
    rt_kprintf("spi2apb_test query spi2apb\n");
    /* config spi2apb write reg2 calue */
    rt_kprintf("spi2apb_test write spi2apb 0x12345678\n");
}

void spi2apb_test(int argc, char **argv)
{
    rt_device_t spi2apb_device = RT_NULL;
    struct rt_spi2apb_configuration config;
    rt_err_t ret;
    char *cmd;
    int value = 0;

    if (argc < 3)
        goto out;

    spi2apb_device = rt_device_find(argv[2]);
    RT_ASSERT(spi2apb_device != RT_NULL);

    ret = rt_device_open(spi2apb_device, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    cmd = argv[1];
    if (!rt_strcmp(cmd, "config"))
    {
        int mode, polarity;

        rt_memset(&config, 0, sizeof(struct rt_spi2apb_configuration));
        mode = atoi(argv[3]);
        if (mode & RT_CONFIG_SPI2APB_MSB)
            config.mode |= RT_CONFIG_SPI2APB_LSB;
        else
            config.mode |= RT_CONFIG_SPI2APB_LSB;

        if (mode & RT_CONFIG_SPI2APB_LITTLE_ENDIAN)
            config.mode |= RT_CONFIG_SPI2APB_LITTLE_ENDIAN;
        else
            config.mode |= RT_CONFIG_SPI2APB_BIG_ENDIAN;

        polarity = atoi(argv[4]) << 2;
        if (polarity & RT_CONFIG_SPI2APB_RXCP_INVERT)
            config.clock_polarity |= RT_CONFIG_SPI2APB_RXCP_INVERT;
        else
            config.clock_polarity |= RT_CONFIG_SPI2APB_RXCP;

        if (polarity & RT_CONFIG_SPI2APB_TXCP_INVERT)
            config.clock_polarity |= RT_CONFIG_SPI2APB_TXCP_INVERT;
        else
            config.clock_polarity |= RT_CONFIG_SPI2APB_TXCP;

        ret = rt_device_control(spi2apb_device, RT_DEVICE_CTRL_SPI2APB_CONFIGURATION,
                                (void *)&config);
        if (ret)
        {
            rt_kprintf("spi2apb test config failed\n");
            return;
        }

        rt_kprintf("spi2apb config mode: 0x%x, polarity: 0x%x\n",
                   config.mode, config.clock_polarity >> 2);
    }
    else if (!rt_strcmp(cmd, "cb"))
    {
        ret = rt_device_control(spi2apb_device, RT_DEVICE_CTRL_SPI2APB_REGISTER_CB,
                                (void *)spi2apb_test_callback);
        if (ret)
            rt_kprintf("spi2apb test register callback failed\n");
    }
    else if (!rt_strcmp(cmd, "read"))
    {
        int reg;

        reg = atoi(argv[3]);
        if (reg)
            ret = rt_device_control(spi2apb_device, RT_DEVICE_CTRL_SPI2APB_READ_REG1,
                                    (void *)&value);
        else
            ret = rt_device_control(spi2apb_device, RT_DEVICE_CTRL_SPI2APB_READ_REG0,
                                    (void *)&value);
        if (ret)
        {
            rt_kprintf("spi2apb test read reg%d failed\n", (reg) ? 1 : 0);
            return;
        }

        rt_kprintf("read reg%d value: 0x%x\n", (reg) ? 1 : 0, value);
    }
    else if (!rt_strcmp(cmd, "query"))
    {
        ret = rt_device_control(spi2apb_device, RT_DEVICE_CTRL_SPI2APB_QUERY_STATUS,
                                (void *)&value);
        if (ret)
        {
            rt_kprintf("spi2apb test query status failed\n");
            return;
        }

        rt_kprintf("query state: 0x%x\n", value);
    }
    else if (!rt_strcmp(cmd, "write"))
    {
        sscanf(argv[3], "%x", &value);
        ret = rt_device_control(spi2apb_device, RT_DEVICE_CTRL_SPI2APB_WRITE_REG2,
                                (void *)&value);
        if (ret)
        {
            rt_kprintf("spi2apb test write reg2 failed.\n");
            return;
        }
    }
    else
    {
        goto out;
    }

    return;

out:
    spi2apb_test_show_usage();
    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(spi2apb_test, spt2apb test cmd);
#endif

#endif
