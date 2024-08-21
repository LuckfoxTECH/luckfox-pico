/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_coredump_flash.c
  * @author
  * @version V0.1
  * @date    09-April-2018
  * @brief   coredump driver
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_USING_COREDUMP_FLASH

#include <dfs_posix.h>
#include "drv_coredump.h"

#ifndef COREDUMP_FLASH_PARTITION_NAME
#define COREDUMP_FLASH_PARTITION_NAME  "breakpad"
#endif
#ifndef COREDUMP_BUF_SIZE
#define COREDUMP_BUF_SIZE  4096
#endif

static char flash_buf[COREDUMP_BUF_SIZE];
static rt_device_t flash = RT_NULL;
static int sector_count = 0, sector_size = COREDUMP_BUF_SIZE;
static rt_off_t flash_pos = 0, flash_buf_pos = 0;

static int rt_flash_coredump_enter(void)
{
    rt_err_t ret;
    struct rt_device_blk_geometry stat;

    flash = rt_device_find(COREDUMP_FLASH_PARTITION_NAME);
    if (flash == RT_NULL)
    {
        goto exit;
    }

    ret = rt_device_open(flash, RT_DEVICE_OFLAG_WRONLY);
    if (ret != RT_EOK)
    {
        goto exit;
    }

    ret = rt_device_control(flash, RT_DEVICE_CTRL_BLK_GETGEOME, &stat);
    if (ret != RT_EOK || (COREDUMP_BUF_SIZE % stat.bytes_per_sector) != 0)
    {
        sector_size = -RT_ERROR;
    }
    else
    {
        sector_size = stat.bytes_per_sector;
        sector_count = stat.sector_count;
    }

    if (sector_size == -RT_ERROR)
        goto exit;

    return RT_EOK;
exit:
    return -RT_ERROR;
}

static int rt_flash_coredump_data_save(char *buf, rt_size_t size)
{

    rt_size_t writes, n = 0, free = 0;
    char *tmp = buf;

    if (flash_buf_pos + size < COREDUMP_BUF_SIZE)
    {
        rt_memcpy(&flash_buf[flash_buf_pos], tmp, size);
        flash_buf_pos += size;
        return size;
    }

    free = COREDUMP_BUF_SIZE - flash_buf_pos;

    rt_memcpy(&flash_buf[flash_buf_pos], tmp, free);
    flash_buf_pos = 0;
    size -= free;
    tmp += free;
    n += free;
    writes = rt_device_write(flash, flash_pos, (const void *)flash_buf, 1);

    if (writes != 1)
    {
        goto exit;
    }

    flash_pos += writes;

    while (size >= sector_size)
    {
        if ((rt_size_t)tmp > (rt_size_t)_stext && (rt_size_t)tmp < (rt_size_t)_etext)
        {
            rt_memcpy(flash_buf, tmp, COREDUMP_BUF_SIZE);
            writes = rt_device_write(flash, flash_pos, (const void *)flash_buf, 1);
        }
        else
        {
            writes = rt_device_write(flash, flash_pos, (const void *)tmp, 1);
        }

        if (writes != 1)
        {
            goto exit;
        }
        tmp += sector_size;
        size -= sector_size;
        n += sector_size;
        flash_pos += writes;
    }

    if (size == 0)
        return n;

    rt_memcpy(&flash_buf[flash_buf_pos], tmp, size);
    flash_buf_pos += size;
    n += size;
    return n;
exit:
    return -RT_ERROR;
}

static int rt_flash_coredump_exit(void)
{
    if (flash_buf_pos > 0 && flash_buf_pos < COREDUMP_BUF_SIZE)
        rt_device_write(flash, flash_pos, (const void *)flash_buf, 1);

    if (flash)
    {
        rt_device_close(flash);
        flash = RT_NULL;
    }
    return RT_EOK;
}

static coredump_ops_t rt_flash_coredump_ops =
{
    .cd_enter = rt_flash_coredump_enter,
    .cd_data_save = rt_flash_coredump_data_save,
    .cd_exit = rt_flash_coredump_exit,
};

static int rt_flash_coredump_init(void)
{
    register_coredump_ops(CD_FLASH, &rt_flash_coredump_ops);
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_flash_coredump_init);

#endif /* end of RT_USING_COREDUMP_FLASH */