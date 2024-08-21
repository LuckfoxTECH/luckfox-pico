/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-10     Cliff      the first version
 *
 */

#include <stdio.h>

#include <rtthread.h>
#include <board.h>

#ifdef  RT_USING_FINSH
#include <finsh.h>
#include <shell.h>
#endif

#ifdef RT_USING_DFS_RAMFS
#include <dfs_fs.h>
#include "dfs_ramfs.h"
extern int __ramfs_start__, __ramfs_end__;
#endif

#ifdef RT_USING_RK_IOT_APP
#include "rk_iot_app.h"
#endif

#include "auto_version.h"

extern rt_thread_t ota_task_start(void);
extern int fw_slot_get_current_running(uint32_t *cur_slot);
extern int fw_slot_set_active(uint32_t slot);
extern void fw_slot_info_dump(void);


static void rt_init_thread_entry(void *parameter)
{
#ifdef RT_USING_DFS_RAMFS
    uint32_t ramfs_size = (uint32_t)&__ramfs_end__ - (uint32_t)&__ramfs_start__;
#endif

    /* Initialization RT-Thread Components */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_init();
#elif defined(RT_USING_FINSH)
    finsh_system_init();
#endif

#if defined(RT_USING_RK_IOT_APP) && defined(PRJCONF_BOOT_AUTOMATICALLY)
    rk_iot_app_init(NULL);
#endif

#ifdef RT_USING_DFS_RAMFS
    if (ramfs_size > 0)
    {
        rt_kprintf("create ramfs: start=%p, end=%p, size=%d\n",
                   &__ramfs_start__, &__ramfs_end__, ramfs_size);
        dfs_init();
#ifndef RT_USING_COMPONENTS_INIT
        dfs_ramfs_init();
#endif
        dfs_ramfs_create((rt_uint8_t *)&__ramfs_start__, ramfs_size);
        if (dfs_mount(NULL, "/", "ram", 1, (rt_uint8_t *)&__ramfs_start__))
            rt_kprintf("mount ramfs failed\n");
    }
#endif

#ifdef RT_USING_FWANALYSIS
    uint32_t boot_slot = -1;
    if (fw_slot_get_current_running(&boot_slot) != 0)
        return;

    if (boot_slot == 0)
    {
        rt_kprintf("%s: running slot A Fw.\n", __func__);
        fw_slot_set_active(boot_slot);

#if 0
        rt_thread_delay(10000);

        /*Just for aging test */
        extern void ota_test_loop();
        extern void ota_test_task(void *arg);
        ota_test_loop(0, NULL);
#endif
    }
    if (boot_slot == 1)
    {
        fw_slot_set_active(boot_slot);
#ifdef RT_USING_OTA_RECOVERY
        rt_kprintf("%s: running slot B recovery mode.\n", __func__);
        fw_slot_info_dump();

        ota_task_start();
#else
        rt_kprintf("%s: running slot B FW.\n", __func__);
#endif
    }
#endif
}

static void rt_show_firmware_info(void)
{
    rt_kprintf("\nversion info: %s\n", FIRMWARE_AUTO_VERSION);
}

int rt_application_init()
{
    rt_thread_t init_thread;

    rt_show_firmware_info();

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 80, 20);
#endif
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    return 0;
}
