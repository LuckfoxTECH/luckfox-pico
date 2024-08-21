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

#define TAG          "BT_API"
typedef enum
{
    DEV_GET_ADDRESS = 0,
    DEV_SET_NAME,
} app_api_cmd_t;
static void usage(void)
{
    rt_kprintf("Usage: bt_api_device -s <name> \n");
    rt_kprintf("       bt_api_device -g\n");
    rt_kprintf("       bt_api_device --help\n");
    rt_kprintf("\n");
    rt_kprintf("Miscellaneous:\n");
    rt_kprintf("  -g           get bt local device address\n");
    rt_kprintf("  -s           set bt local device name\n");
    rt_kprintf("  --help       Print help information\n");
}
static void bt_api_device(int argc, char **argv)
{
    osi_err_t ret;
    RK_LOGE(TAG, "argv:%s\n", *argv);

    char *address = NULL;
    if (rk_bluedroid_get_status() != RK_BLUEDROID_STATUS_ENABLED)
    {
        RK_LOGE(TAG, "bluedroid is disable,please check!\n");
        return;
    }
    if (argc == 1 || argc > 7)
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
        else if (rt_strcmp(argv[1], "-g") == 0)
        {
            address = (char *)rk_bt_dev_get_address();
            RK_LOGE(TAG, "rk_bt_dev_get_address:%02x:%02x:%02x:%02x:%02x:%02x \n",
                    address[0], address[1], address[2], address[3], address[4], address[5]);
            return;
        }
        else if (rt_strcmp(argv[1], "-s") == 0)
        {
            if ((ret = rk_bt_dev_set_device_name(argv[2])) != OSI_EOK)
            {
                RK_LOGE(TAG, "fail to set device name:%s\n", argv[2]);
            }
            RK_LOGE(TAG, "scuess to set device name:%s\n", argv[2]);
            return;
        }
        else
        {
            goto __usage;
        }
    }
__usage:
    usage();

}

MSH_CMD_EXPORT(bt_api_device, bt bluedroid device api test);
