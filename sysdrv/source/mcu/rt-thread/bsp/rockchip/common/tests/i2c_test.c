/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    i2c_test.c
  * @author  David Wu
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   i2c test for pisces
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_I2C

#include <stdbool.h>
#include "hal_base.h"

static struct rt_i2c_bus_device *i2c_bus;

int i2c_write(rt_uint16_t addr, void *data_buf, size_t data_len)
{
    struct rt_i2c_msg msgs[1];
    int ret;

    msgs[0].addr  = addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = data_buf;
    msgs[0].len   = data_len;

    ret = rt_i2c_transfer(i2c_bus, msgs, 1);
    if (ret == 1)
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("i2c_write fail: %d\n", ret);
        return -RT_ERROR;
    }
}

int i2c_read(rt_uint16_t addr, void *cmd_buf, size_t cmd_len, void *data_buf, size_t data_len)
{
    struct rt_i2c_msg msgs[2];
    int ret;

    msgs[0].addr  = addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = cmd_buf;
    msgs[0].len   = cmd_len;

    msgs[1].addr  = addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = data_buf;
    msgs[1].len   = data_len;

    ret = rt_i2c_transfer(i2c_bus, msgs, 2);
    if (ret == 2)
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("i2c_read fail: %d\n", ret);
        return -RT_ERROR;
    }
}

void i2c_test_show_usage()
{
    /* send, i2c1, addr: 0x20, regaddr: 0x1234, regaddr_len: 0x2, data: 0xf5f5f5f5, len: 4 */
    rt_kprintf("i2c_test write i2c1 0x20 0x1234 2 0xf5f5f5f5 4\n");
    /* read, i2c2, addr: 0x20, regaddr: 0x1234, regaddr_len: 0x2, len: 32 */
    rt_kprintf("i2c_test read i2c2 0x20 0x1234 2 32\n");
}

void i2c_test(int argc, char **argv)
{
    char *cmd_buf, *cmd, *data_buf = NULL;
    uint32_t regaddr, data, data_len = 0, cmd_len = 0;
    rt_uint16_t addr;
    int i;

    if (argc < 7)
        goto out;

    i2c_bus = (struct rt_i2c_bus_device *)rt_i2c_bus_device_find(argv[2]);
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("Did not find device: %s....\n", argv[2]);
        return;
    }

    cmd = argv[1];
    if (!rt_strcmp(cmd, "write"))
    {
        addr = strtol(argv[3], NULL, 16);
        regaddr = strtol(argv[4], NULL, 16);
        cmd_len = atoi(argv[5]);
        data = strtol(argv[6], NULL, 16);
        data_len = atoi(argv[7]);

        data_buf = (char *)rt_calloc(1, data_len + cmd_len);
        if (!data_buf)
        {
            rt_kprintf("i2c write alloc buf size %d fail\n", data_len);
            return;
        }

        for (i = 0; i < cmd_len; i++)
        {
            data_buf[i] = (regaddr >> (8 * i)) & 0xff;
        }

        for (i = cmd_len; i < data_len + cmd_len; i++)
        {
            data_buf[i] = (data >> (8 * (i - cmd_len))) & 0xff;
        }

        i2c_write(addr, data_buf, data_len + cmd_len);

        rt_free(data_buf);
    }
    else if (!rt_strcmp(cmd, "read"))
    {
        addr = strtol(argv[3], NULL, 16);
        regaddr = strtol(argv[4], NULL, 16);
        cmd_len = atoi(argv[5]);
        data_len = atoi(argv[6]);

        cmd_buf = (char *)rt_calloc(1, cmd_len);
        if (!cmd_buf)
        {
            rt_kprintf("i2c read alloc cmd_buf size %d fail\n", cmd_len);
            return;
        }

        for (i = 0; i < cmd_len; i++)
            cmd_buf[i] = (regaddr >> (8 * i)) & 0xff;

        data_buf = (char *)rt_calloc(1, data_len);
        if (!data_buf)
        {
            rt_kprintf("i2c read alloc buf size %d fail\n", data_len);
            return;
        }

        if (i2c_read(addr, cmd_buf, cmd_len, data_buf, data_len))
        {
            rt_free(data_buf);
            rt_free(cmd_buf);
            return;
        }

        for (i = 0; i < data_len; i++)
        {
            rt_kprintf("recv[%d]: 0x%x\n", i, data_buf[i]);
        }

        rt_free(data_buf);
        rt_free(cmd_buf);
    }
    else
    {
        goto out;
    }

    return;
out:
    i2c_test_show_usage();
    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(i2c_test, i2c test cmd);
#endif

#endif
