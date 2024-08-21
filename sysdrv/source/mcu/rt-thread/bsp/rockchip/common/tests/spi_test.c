/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    spi_test.c
  * @author  David Wu
  * @version V0.1
  * @date    10-Mar-2019
  * @brief   spi test for pisces
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_SPI

#include <stdbool.h>
#include <drivers/spi.h>
#include "hal_base.h"

struct spi_test_data
{
    struct rt_spi_device *spi_device;
    uint32_t mode;
    uint32_t spi_mode;
    uint32_t bit_first;
    uint32_t max_speed_hz;
};

static struct spi_test_data g_spi_test_data;

static int spi_test_write(const void *txbuf, size_t n)
{
    struct spi_test_data *data = &g_spi_test_data;
    struct rt_spi_device *spi_device = data->spi_device;
    int ret = -RT_ERROR;

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = data->mode | data->spi_mode | data->bit_first;
        cfg.max_hz = data->max_speed_hz;
        rt_spi_configure(spi_device, &cfg);
    }

    /* send data */
    ret = rt_spi_transfer(spi_device, txbuf, RT_NULL, n);

    return (ret == n) ? RT_EOK : ret;
}

static int spi_test_read(void *rxbuf, size_t n)
{
    struct spi_test_data *data = &g_spi_test_data;
    struct rt_spi_device *spi_device = data->spi_device;
    int ret = -RT_ERROR;
    struct rt_spi_configuration cfg;

    cfg.data_width = 8;
    cfg.mode = data->mode | data->spi_mode | data->bit_first;
    cfg.max_hz = data->max_speed_hz;
    rt_spi_configure(spi_device, &cfg);

    /* send data */
    ret = rt_spi_transfer(spi_device, RT_NULL, rxbuf, n);

    return (ret == n) ? RT_EOK : ret;
}

static int spi_test_write_and_read(const void *tx_buf, void *rx_buf, size_t len)
{
    struct spi_test_data *data = &g_spi_test_data;
    struct rt_spi_device *spi_device = data->spi_device;
    int ret = -RT_ERROR;

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = data->mode | data->spi_mode | data->bit_first;
        cfg.max_hz = data->max_speed_hz;
        rt_spi_configure(spi_device, &cfg);
    }

    ret = rt_spi_transfer(spi_device, tx_buf, rx_buf, len);

    return (ret == len) ? RT_EOK : -RT_EIO;
}

static int __unused spi_test_write_then_read(const void *txbuf, size_t tx_n,
        void *rxbuf, size_t rx_n)
{
    struct spi_test_data *data = &g_spi_test_data;
    struct rt_spi_device *spi_device = data->spi_device;
    int ret = -RT_ERROR;

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = data->mode | data->spi_mode | data->bit_first;
        cfg.max_hz = data->max_speed_hz;
        rt_spi_configure(spi_device, &cfg);
    }

    ret = rt_spi_send_then_recv(spi_device, txbuf, tx_n, rxbuf, rx_n);

    return (ret == rx_n) ? RT_EOK : -RT_EIO;
}

static void spi_test_show_usage()
{
    rt_kprintf("1. spi_test config <spi_device> <is_slave: 1-slave 0-master> <spi_mode> <bit_first> <speed>\n");
    rt_kprintf("2. spi_test read   <spi_device> <loops> <size>\n");
    rt_kprintf("3. spi_test write  <spi_device> <loops> <size>\n");
    rt_kprintf("4. spi_test loop   <spi_device> <loops> <size>\n");
    rt_kprintf("5. spi_test cs     <spi_device> <cs_state:1-take, 0-release>\n");
    rt_kprintf("6. spi_test bus    <spi_device> <bus_state:1-take, 0-release>\n");
    rt_kprintf("7. spi_test rate   <spi_device>\n");
    rt_kprintf("like:\n");
    rt_kprintf("\tspi_test config spi1_0 0 3 0 24000000\n");
    rt_kprintf("\tspi_test read   spi1_0 1 256\n");
    rt_kprintf("\tspi_test write  spi1_0 1 256\n");
    rt_kprintf("\tspi_test loop   spi1_0 1 25\n");
    rt_kprintf("\tspi_test cs     spi1_0 1\n");
    rt_kprintf("\tspi_test bus    spi1_0 1\n");
    rt_kprintf("\tspi_test rate   spi1_0\n");
}

static void spi_test_write_loop(uint32_t loops, void *txbuf, uint32_t size)
{
    uint32_t i, bytes, start_time, end_time, cost_time;

    start_time = rt_tick_get();
    for (i = 0; i < loops; i++)
        spi_test_write(txbuf, size);
    end_time = rt_tick_get();
    cost_time = (end_time - start_time) * 1000 / RT_TICK_PER_SECOND;

    bytes = size * loops * 1;
    bytes = bytes / cost_time;
    rt_kprintf("spi %s write %d*%d cost %ldms speed:%ldKB/S\n", HAL_IS_CACHELINE_ALIGNED(size) ? "DMA" : "CPU", size, loops, cost_time, bytes);
}

static void spi_test_read_loop(uint32_t loops, void *rxbuf, uint32_t size)
{
    uint32_t i, bytes, start_time, end_time, cost_time;

    start_time = rt_tick_get();
    for (i = 0; i < loops; i++)
        spi_test_read(rxbuf, size);
    end_time = rt_tick_get();
    cost_time = (end_time - start_time) * 1000 / RT_TICK_PER_SECOND;

    bytes = size * loops * 1;
    bytes = bytes / cost_time;
    rt_kprintf("spi %s read %d*%d cost %ldms speed:%ldKB/S\n", HAL_IS_CACHELINE_ALIGNED(size) ? "DMA" : "CPU", size, loops, cost_time, bytes);
}

void spi_test(int argc, char **argv)
{
    struct spi_test_data *data = &g_spi_test_data;
    struct rt_spi_device *spi_device = RT_NULL;
    char *cmd, *txbuf = NULL, *rxbuf = NULL;
    uint32_t bytes, start_time, end_time, cost_time;
    uint32_t loops = 0, size = 0, temp;
    int i;

    if (argc < 3)
        goto out;

    spi_device = (struct rt_spi_device *)rt_device_find(argv[2]);
    if (spi_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: %s....\n", argv[2]);
        return;
    }
    data->spi_device = spi_device;

    cmd = argv[1];
    if (!rt_strcmp(cmd, "config"))
    {
        int mode, bit_first;

        if (argc < 7)
            goto out;

        mode = atoi(argv[3]);
        if (mode == 0)
            data->mode = RT_SPI_MASTER;
        else
            data->mode = RT_SPI_SLAVE;

        data->spi_mode = atoi(argv[4]);
        bit_first = atoi(argv[5]);
        if (bit_first)
            data->bit_first = RT_SPI_MSB;
        else
            data->bit_first = RT_SPI_LSB;
        data->max_speed_hz = atoi(argv[6]);

        rt_kprintf("spi %s, mode%d, %s, %dHz speed\n",
                   mode ? "slave" : "master", data->spi_mode,
                   bit_first ? "MSB" : "LSB", data->max_speed_hz);
    }
    else if (!rt_strcmp(cmd, "write"))
    {
        loops = atoi(argv[3]);
        size = atoi(argv[4]);

        txbuf = (char *)rt_malloc_align(size, 32);
        if (!txbuf)
        {
            rt_kprintf("spi write alloc buf size %d fail\n", size);
            return;
        }

        for (i = 0; i < size; i++)
            txbuf[i] = i % 256;

        spi_test_write_loop(loops, txbuf, size);

        rt_free_align(txbuf);
    }
    else if (!rt_strcmp(cmd, "read"))
    {
        loops = atoi(argv[3]);
        size = atoi(argv[4]);

        rxbuf = (char *)rt_malloc_align(size, 32);
        if (!rxbuf)
        {
            rt_kprintf("spi read alloc buf size %d fail\n", size);
            return;
        }
        spi_test_read_loop(loops, rxbuf, size);

        rt_free_align(rxbuf);
    }
    else if (!rt_strcmp(cmd, "loop"))
    {
        loops = atoi(argv[3]);
        size = atoi(argv[4]);

        txbuf = (char *)rt_malloc_align(size, 32);
        if (!txbuf)
        {
            rt_kprintf("spi write alloc buf size %d fail\n", size);
            return;
        }

        rxbuf = (char *)rt_malloc_align(size, 32);
        if (!rxbuf)
        {
            rt_kprintf("spi read alloc buf size %d fail\n", size);
            rt_free_align(txbuf);
            return;
        }

        for (i = 0; i < size; i++)
            txbuf[i] = i % 256;

        start_time = rt_tick_get();
        for (i = 0; i < loops; i++)
            spi_test_write_and_read(txbuf, rxbuf, size);
        end_time = rt_tick_get();
        cost_time = (end_time - start_time) * 1000 / RT_TICK_PER_SECOND;

        if (rt_memcmp(txbuf, rxbuf, size))
        {
            for (i = 0; i < size; i++)
            {
                if (txbuf[i] !=  rxbuf[i])
                    rt_kprintf("send[%d]: 0x%x, recv[%d]: 0x%x\n", i, txbuf[i], i, rxbuf[i]);
            }
            rt_kprintf("spi loop test fail\n");
        }

        bytes = size * loops;
        bytes = bytes / cost_time;
        rt_kprintf("spi loop %d*%d cost %ldms speed:%ldKB/S\n", size, loops, cost_time, bytes);

        rt_free_align(txbuf);
        rt_free_align(rxbuf);
    }
    else if (!rt_strcmp(cmd, "cs"))
    {
        temp = atoi(argv[3]);
        if (temp == 1)
        {
            rt_spi_take(spi_device);
        }
        else
        {
            rt_spi_release(spi_device);
        }
    }
    else if (!rt_strcmp(cmd, "bus"))
    {
        struct rt_spi_configuration cfg;

        temp = atoi(argv[3]);

        cfg.data_width = 8;
        cfg.mode = data->mode | data->spi_mode | data->bit_first;
        cfg.max_hz = data->max_speed_hz;
        rt_spi_configure(spi_device, &cfg);

        if (temp == 1)
        {
            rt_spi_take_bus(spi_device);
        }
        else
        {
            rt_spi_release_bus(spi_device);
        }
    }
    else if (!rt_strcmp(cmd, "rate"))
    {
        txbuf = (char *)rt_malloc_align(4096, 32);
        if (!txbuf)
        {
            rt_kprintf("spi write alloc buf size %d fail\n", size);
            return;
        }

        loops = 1000;

        size = 1;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 32;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 64;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 65;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 128;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 129;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 256;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 257;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 512;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 513;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 1024;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 1025;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 2048;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 2049;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 4096;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        size = 4097;
        spi_test_read_loop(loops, txbuf, size);
        spi_test_write_loop(loops, txbuf, size);

        rt_free_align(txbuf);
    }
    else
    {
        goto out;
    }

    return;
out:
    spi_test_show_usage();
    return;
}

int _at_spi_test(void)
{
    int ret;
    char *buffer;
    size_t size;
    struct spi_test_data *data = &g_spi_test_data;
    struct rt_spi_device *spi_device = RT_NULL;

    spi_device = (struct rt_spi_device *)rt_device_find("spi1_0");
    if (spi_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: spi1_0....\n");
        return RT_ERROR;
    }
    data->spi_device = spi_device;
    data->mode = RT_SPI_MASTER;
    data->max_speed_hz = 50000000;

    size = 2048;
    buffer = (char *)rt_malloc_align(size, 32);
    if (!buffer)
    {
        rt_kprintf("spi buffer alloc buf size %d fail\n", size);
        return RT_ERROR;
    }

    for (size = 1024; size < 1036; size++)
    {
        ret = spi_test_write(buffer, size);
        if (ret)
        {
            rt_kprintf("spi_test_write %d fail %d\n", size, ret);
            goto out;
        }
        ret = spi_test_read(buffer, size);
        if (ret)
        {
            rt_kprintf("spi_test_read %d fail %d\n", size, ret);
            goto out;
        }
    }
    rt_kprintf("%s test succress\n", __func__);
out:
    rt_free_align(buffer);
    return ret;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(spi_test, spt test cmd);
FINSH_FUNCTION_EXPORT(_at_spi_test, spi test for auto test);
#endif

#endif
