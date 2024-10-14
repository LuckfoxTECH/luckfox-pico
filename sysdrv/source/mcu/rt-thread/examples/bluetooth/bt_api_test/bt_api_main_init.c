/******************************************************************************
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author              Notes
 * 2019-05-10     mark.huang && xyp   first implementation
 *
 ******************************************************************************/


#include <stdint.h>
#include <string.h>

#include "rk_log.h"
#include "rk_bt.h"
#include "rk_bt_main.h"
#include "rk_bt_device.h"
#include "osi/types.h"
#include "osi/bt_config.h"
#include "btHwControl.h"
#define TAG          "BT_API"

typedef enum
{
    BLUEDROID_INIT = 0,
    BLUEDROID_DEINIT,
    BLUEDROID_ENABLE,
    BLUEDROID_DISABLE,
} app_api_cmd_t;

#ifdef BT_CHIP_AP6212
extern bt_hw_control_t ap6212_hw_control;
bt_hw_control_t *p_hci_if_main = &ap6212_hw_control;
#else
//koala
extern bt_hw_control_t koala_hw_control;
bt_hw_control_t *p_hci_if_main = &koala_hw_control;
#endif

static void usage(void)
{
    rt_kprintf("Usage: bt_api_main --init\n");
    rt_kprintf("       bt_api_main --deinit\n");
    rt_kprintf("       bt_api_main --enable\n");
    rt_kprintf("       bt_api_main --disable\n");
    rt_kprintf("       bt_api_main --reset <1/0>\n");
    rt_kprintf("       bt_api_main --help\n");
    rt_kprintf("\n");
    rt_kprintf("Miscellaneous:\n");
    rt_kprintf("  --init       init bluedroid\n");
    rt_kprintf("  --deinit      deinit bluedroid\n");
    rt_kprintf("  --enable     enable bluedroid\n");
    rt_kprintf("  --disable    disable bluedroid\n");
    rt_kprintf("  --reset      reset bt,power up(1)/down(0)\n");
    rt_kprintf("  --help       Print help information\n");
}

void bt_reset()
{
    osi_err_t ret;
    rk_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = rk_bt_controller_init(&bt_cfg, p_hci_if_main)) != OSI_EOK)
        rt_kprintf("chipset: koala hw control\n");
    {
        RK_LOGE(TAG, "initialize controller failed\n");
        return;
    }

    if ((ret = rk_bt_controller_enable(RK_BT_MODE_CLASSIC_BT)) != OSI_EOK)
    {
        RK_LOGE(TAG, "enable controller failed:\n");
        return;
    }

    if ((ret = rk_bluedroid_init()) != OSI_EOK)
    {
        RK_LOGE(TAG, "initialize bluedroid failed\n");
        return;
    }

    if ((ret = rk_bluedroid_enable()) != OSI_EOK)
    {
        RK_LOGE(TAG, "enable bluedroid failed\n");
        return;
    }
    rt_thread_delay(500);
    if ((ret = rk_bluedroid_disable()) != OSI_EOK)
    {
        RK_LOGE(TAG, "disable bluedroid failed\n");
    }
    if ((ret = rk_bluedroid_deinit()) != OSI_EOK)
    {
        RK_LOGE(TAG, "deinit bluedroid failed\n");
    }
    if ((ret = rk_bt_controller_disable()) != OSI_EOK)
    {
        RK_LOGE(TAG, "disable controller failed:\n");
    }
    if ((ret = rk_bt_controller_deinit()) != OSI_EOK)
    {
        RK_LOGE(TAG, "initialize controller failed\n");
    }
}

static void bt_api_main(int argc, char **argv)
{
    osi_err_t ret;
    if (argc == 1 || argc > 5)
    {
        rt_kprintf("Please check the command you entered!\n");
        goto __usage;
    }
    else
    {
        if (rt_strcmp(argv[1], "--help") == 0)
        {
            goto __usage;
        }
        else if (rt_strcmp(argv[1], "--init") == 0)
        {
            if ((ret = rk_bluedroid_init()) != OSI_EOK)
            {
                RK_LOGE(TAG, "init bluedroid failed\n");
            }
        }
        else if (rt_strcmp(argv[1], "--deinit") == 0)
        {
            if ((ret = rk_bluedroid_deinit()) != OSI_EOK)
            {
                RK_LOGE(TAG, "deinit bluedroid failed\n");
            }
        }
        else if (rt_strcmp(argv[1], "--enable") == 0)
        {
            if ((ret = rk_bluedroid_enable()) != OSI_EOK)
            {
                RK_LOGE(TAG, "enable bluedroid failed\n");
            }
        }
        else if (rt_strcmp(argv[1], "--disable") == 0)
        {
            if ((ret = rk_bluedroid_disable()) != OSI_EOK)
            {
                RK_LOGE(TAG, "disable bluedroid failed\n");
            }
        }
        else if (rt_strcmp(argv[1], "--reset") == 0)
        {
            if (rt_strcmp(argv[2], "1") == 0)
            {
                rk_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
                if ((ret = rk_bt_controller_init(&bt_cfg, p_hci_if_main)) != OSI_EOK)
                {
                    RK_LOGE(TAG, "initialize controller failed\n");
                    return;
                }

                if ((ret = rk_bt_controller_enable(RK_BT_MODE_CLASSIC_BT)) != OSI_EOK)
                {
                    RK_LOGE(TAG, "enable controller failed:\n");
                    return;
                }
            }
            else if (rt_strcmp(argv[2], "0") == 0)
            {
                if ((ret = rk_bt_controller_disable()) != OSI_EOK)
                {
                    RK_LOGE(TAG, "disable controller failed:\n");
                }
                if ((ret = rk_bt_controller_deinit()) != OSI_EOK)
                {
                    RK_LOGE(TAG, "initialize controller failed\n");
                }
            }
            else
            {
                RK_LOGE(TAG, "start bluetooth reset test 100 times\n");
                int times = 100;
                for (int i = 0; i < times; i++)
                {
                    bt_reset();
                }
            }
        }
        else
        {
            goto __usage;
        }
    }
    RK_LOGE(TAG, "bluedroid status:%d\n", rk_bluedroid_get_status());
    return;

__usage:
    usage();


}

MSH_CMD_EXPORT(bt_api_main, bt bluedroid main api test);
