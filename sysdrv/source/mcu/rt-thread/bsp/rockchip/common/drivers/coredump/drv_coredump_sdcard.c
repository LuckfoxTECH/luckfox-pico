/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_coredump_sdcard.c
  * @author
  * @version V0.1
  * @date    09-April-2018
  * @brief   coredump driver
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_USING_COREDUMP_SDCARD

#include <dfs_posix.h>
#include "drv_coredump.h"

static int rt_sdcard_fd = -1;


static int rt_sdcard_coredump_enter(void)
{
    int fd = 0;
    char name[80];
    time_t now;
    int i = 0;

    now = time(RT_NULL);
    rt_sprintf(name, "/sdcard/coredump/core_dump_%sbin", ctime(&now));

    while (name[i])
    {
        if (name[i] == ' ')
            name[i] = '_';
        if (name[i] == ':')
            name[i] = '_';
        if (name[i] == '\r')
            name[i] = '.';
        if (name[i] == '\n')
            name[i] = '.';
        i++;
    }

    fd = open(name, O_WRONLY | O_CREAT);

    if (fd < 0)
        return fd;

    rt_sdcard_fd = fd;
    return RT_EOK;
}

static int rt_sdcard_coredump_data_save(char *buf, rt_size_t size)
{
    return write(rt_sdcard_fd, buf, size);
}

static int rt_sdcard_coredump_exit(void)
{
    close(rt_sdcard_fd);
    return RT_EOK;
}

static coredump_ops_t rt_sdcard_coredump_ops =
{
    .cd_enter = rt_sdcard_coredump_enter,
    .cd_data_save = rt_sdcard_coredump_data_save,
    .cd_exit = rt_sdcard_coredump_exit,
};

static int rt_sdcard_coredump_init(void)
{
    register_coredump_ops(CD_SDCARD, &rt_sdcard_coredump_ops);
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_sdcard_coredump_init);

#endif /* end of RT_USING_COREDUMP_SDCARD */