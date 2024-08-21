/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    ota_test.c
  * @version V1.0
  * @brief   ota test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-02-15     Chad.ma         the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_OTA
#include <drivers/mtd_nor.h>
#include <dfs_posix.h>
#include "hal_base.h"
#include "../fwmgr/fw_analysis.h"
#include "../fwmgr/rkpart.h"

#include "ota.h"
#include "ota_file.h"
#include "ota_opt.h"

#define DBG_SECTION_NAME    "OTA_TEST"
#include "rkdebug.h"


void ota_test_main(void *arg)
{
    char *file_url = NULL;
    ota_status ret;
    uint32_t slot_boot_idx;

    LOGD("%s Enter...", __func__);

#ifdef RT_USING_OTA_FROM_LOCAL
    LOGD("This is ota # LOCAL # test ....");
    /* fw file in emmc device. */
    file_url = rt_calloc(1, 256);
    if (!file_url)
    {
        LOGD("malloc memory fail ");
        return;
    }

    /* get current os run seq */
    if (ota_init() != OTA_STATUS_OK)
        goto END;

    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        goto END;

    /* according os run seq to get local fw path (url)*/
    rt_memcpy(file_url, OTA_FW_LOCAL_PATH, rt_strlen(OTA_FW_LOCAL_PATH));

    fw_slot_reset_flag(slot_boot_idx);
    ret = ota_update_image(OTA_PROTOCOL_FILE, file_url);
    if (OTA_STATUS_OK != ret)
    {
        LOGE("ota update image Fail!!!\n");
        goto END;
    }

#ifdef RT_USING_OTA_RECOVERY
    /* if enable recovery mode and running in slot 0, should reboot to slot 1 to
     * run ota task, to do upgrade work.
     */
    if (slot_boot_idx == 0 && OTA_STATUS_OK == ret)
    {
        LOGD("!!! OTA type is recovery mode, will reboot to run Fw2 !!!");
        if (file_url)
            rt_free(file_url);

        if (!fw_slot_change(1 - slot_boot_idx))
            ota_reboot(1 - slot_boot_idx);
    }
#endif  //#ifdef RT_USING_OTA_RECOVERY

#endif  //#ifdef RT_USING_OTA_FROM_LOCAL

END:
    if (file_url)
        rt_free(file_url);
}


#ifdef RT_USING_FINSH
#include <finsh.h>

void ota_test(int argc, char **argv)
{
    LOGD("%s\n", __func__);
    rt_thread_t ota_task_thread;
    ota_task_thread = rt_thread_create("ota_test_task",
                                       ota_test_main,
                                       RT_NULL,
                                       8192 * 2,
                                       15,
                                       20);

    if (!ota_task_thread)
    {
        LOGD("ota test task create failed");
        return;
    }
    else
        rt_thread_startup(ota_task_thread);
}

static void fw_choose_show_usage(void)
{
    rt_kprintf("fw_choose Usage:\n");
    rt_kprintf("\t1. fw_choose: show usage\n");
    rt_kprintf("\t2. fw_choose (1|2): select fw slot (A/B) to boot\n");
}

void fw_choose(int argc, char **argv)
{
    rt_uint8_t slot_idx = 0;
    rt_uint32_t slot_boot_idx = 0;
    int ret;

    if (argc < 2)
        goto out;

    slot_idx = atoi(argv[1]) - 1;

    if (slot_idx > 2 || slot_idx < 0)
        goto out;

    LOGD("%s Enter...", __func__);

    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        goto out;

    LOGD("current slot is %d", slot_boot_idx);
    if (slot_boot_idx == slot_idx)
    {
        LOGD("Have same with current slot fw");
        goto out;
    }

    ret = fw_slot_change(slot_idx);
    if (ret < 0)
        goto out;
    else if (ret > 1)
        return;

    LOGD("%s Exit...", __func__);
    ota_reboot(0);
    return ;
out:
    fw_choose_show_usage();
    return;
}

void which_running(int argc, char **argv)
{
    int now_slot = -1;
    rt_uint32_t slot_boot_idx = 0;
    const char *slot_name[2] = {"A", "B"};
    struct rt_mtd_nor_device *snor_device = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        LOGD("Did not find device: snor....\n");
        return;
    }

    LOGD("%s Enter...", __func__);
    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        return;

    now_slot = slot_boot_idx;
    LOGD("current OS running slot is %d, OS%s", now_slot, slot_name[now_slot]);

#ifdef RT_SUPPORT_ROOT_AB
    if (user_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        return;
#endif

    now_slot = slot_boot_idx;
    LOGD("current USER running slot is %d, root%s", now_slot, slot_name[now_slot]);
}

void fw_slot(int argc, char **argv)
{
    fw_slot_info_dump();
}


MSH_CMD_EXPORT(ota_test, ota test demo);
MSH_CMD_EXPORT(fw_choose, specific os fw slot to boot demo);
MSH_CMD_EXPORT(which_running, list which os and userdata partition running demo);
MSH_CMD_EXPORT(fw_slot, show slot info demo);

#endif

#endif
