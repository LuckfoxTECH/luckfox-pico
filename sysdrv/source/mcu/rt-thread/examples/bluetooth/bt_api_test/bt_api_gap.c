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
#include "rk_gap_bt_api.h"
#include "osi/types.h"
#include "osi/bt_config.h"

#define TAG          "BT_API"

static void usage(void)
{
    rt_kprintf("Usage: bt_api_gap -s <1/0>\n");
    rt_kprintf("       bt_api_gap -c\n");
    rt_kprintf("       bt_api_gap -d\n");
    rt_kprintf("       bt_api_gap --help\n");
    rt_kprintf("\n");
    rt_kprintf("Miscellaneous:\n");
    rt_kprintf("  -p <bd_addr>          rk_bt_gap_start_pair(bd_addr)\n");
    rt_kprintf("  -q <bd_addr>          rk_bt_gap_stop_pair(bd_addr)\n");
    rt_kprintf("  -d           rk_bt_gap_start_discovery()\n");
    rt_kprintf("  -c           rk_bt_gap_cancel_discovery()\n");
    rt_kprintf("  -s           rk_bt_gap_set_scan_mode(),1:enable,0:disable\n");
    rt_kprintf("  --help       Print help information\n");
}

static bool str2bdaddr(rk_bd_addr_t *addr, char *string)
{
    rk_bd_addr_t new_addr;
    uint8_t *ptr = new_addr;
    memset(addr, 0, 6);
    uint32_t ptr_32[6];
    bool ret  = sscanf(string, "%02x:%02x:%02x:%02x:%02x:%02x",
                       &ptr_32[0], &ptr_32[1], &ptr_32[2], &ptr_32[3], &ptr_32[4], &ptr_32[5]) == 6;
    if (ret)
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            ptr[i] = (uint8_t) ptr_32[i];
        }
        memcpy(addr, &new_addr, sizeof(rk_bd_addr_t));
    }

    return ret;
}

static void bt_api_gap(int argc, char **argv)
{
    rk_bd_addr_t bda = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
    RK_LOGE(TAG, "argv:%s\n", *argv);

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
        else if (rt_strcmp(argv[1], "-s") == 0)
        {
            if (rt_strcmp(argv[2], "1") == 0)
            {
                /* set discoverable and connectable mode, wait to be connected */
                rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);
                RK_LOGE(TAG, "set discoverable and connectable mode. \n");
            }
            else
            {
                rk_bt_gap_set_scan_mode(RK_BT_NON_CONNECTABLE, RK_BT_NON_DISCOVERABLE);
                RK_LOGE(TAG, "set undiscoverable and unconnectable mode close scan now. \n");
            }
            return;
        }
        else if (rt_strcmp(argv[1], "-d") == 0)
        {
            rk_bt_gap_start_discovery(RK_BT_INQ_MODE_GENERAL_INQUIRY, RK_BT_GAP_MAX_INQ_LEN, 0);
            RK_LOGE(TAG, "Discovery started.\n");
            return;
        }
        else if (rt_strcmp(argv[1], "-c") == 0)
        {
            rk_bt_gap_cancel_discovery();
            RK_LOGE(TAG, "Discovery cancel.\n");
            return;
        }
        else if (rt_strcmp(argv[1], "-p") == 0)
        {
            RK_LOGE(TAG, "start pair device:%s.\n", argv[2]);
            str2bdaddr(&bda, argv[2]);
            RK_LOGE(TAG, "start pair device 2222222.\n");
            rk_bt_gap_start_pair(bda);
            RK_LOGE(TAG, "start pair device done.\n");
            return;
        }
        else if (rt_strcmp(argv[1], "-q") == 0)
        {
            RK_LOGE(TAG, "stop paired device:%s.\n", argv[2]);
            str2bdaddr(&bda, argv[2]);
            rk_bt_gap_stop_pair(bda);
            RK_LOGE(TAG, "stop paired device done.\n");
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

MSH_CMD_EXPORT(bt_api_gap, bt bluedroid gap api test);
