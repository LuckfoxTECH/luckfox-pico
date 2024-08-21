/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */
#include "rtthread.h"
#include "hal_base.h"

#if defined(RT_USING_COMMON_STRESS_TEST)

#include "dfs_fs.h"
#include "dfs_file.h"
#include "dfs_posix.h"
#include "coremark.h"
#include "memtester.h"

static rt_thread_t reboot_thread = RT_NULL;

extern void rt_hw_cpu_reset(void);

static void usage()
{
    rt_kprintf("stress test suite\n");
    rt_kprintf("stress_test case_name\n\n");
    rt_kprintf("example:\n");
    rt_kprintf("stress_test coremark \n");
    rt_kprintf("stress_test reboot \n");
    rt_kprintf("stress_test memtest \n");
    rt_kprintf("stress_test flash \n");
}

void handle_reboot()
{
    struct stat st;

    if (!dfs_file_stat("/st_reboot", &st))
    {
        rt_kprintf("find st_reboot, the dev will reboot after 10 seconds\n");
        rt_kprintf("if you want to stop test, using: rm /st_reboot \n");
        rt_thread_delay(rt_tick_from_millisecond(10000));
        rt_hw_cpu_reset();
    }
}

int reboot_thread_init()
{
    reboot_thread = rt_thread_create("reboot", handle_reboot, RT_NULL, 1024, 10, 20);
    RT_ASSERT(reboot_thread);
    rt_thread_startup(reboot_thread);

    return RT_EOK;
}

void st_coremark()
{
    uint32_t count = 0;

    do
    {
        rt_kprintf("run %d: \n\n", ++count);
        coremark_main();
    }
    while (1);
}

void st_reboot()
{
    int fd;
    struct statfs st;

    if (dfs_statfs("/", &st))
    {
        dfs_mkfs("elm", "root");
        dfs_mount("root", "/", "elm", RT_NULL, RT_NULL);
    }

    fd = open("/st_reboot", O_CREAT | O_TRUNC);
    if (fd < 0)
    {
        rt_kprintf("can not create st_reboot\n");
        return;
    }

    close(fd);

    rt_kprintf("the dev will reboot after 10 seconds\n");
    rt_thread_delay(rt_tick_from_millisecond(10000));
    rt_hw_cpu_reset();
}

void st_memtest()
{
    char *cmd[4] = {"memtester", "0x1c000000", "0x800000", "1"};

    memtester(4, cmd);
}

void st_flash()
{
}

void stress_test(int argc, char **argv)
{
    if (argc != 2)
    {
        usage();
        return;
    }

    if (!strcmp(argv[1], "coremark"))
    {
        st_coremark();
    }
    else if (!strcmp(argv[1], "reboot"))
    {
        st_reboot();
    }
    else if (!strcmp(argv[1], "memtest"))
    {
        st_memtest();
    }
    else if (!strcmp(argv[1], "flash"))
    {
        st_flash();
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(stress_test, stress test);
#endif

INIT_APP_EXPORT(reboot_thread_init);

#endif
