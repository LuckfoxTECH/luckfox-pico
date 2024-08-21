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

#include <string.h>

#include "rk_bt.h"
#include "rk_hf_client_api.h"

static void usage(void)
{
    rt_kprintf("Usage: hfp_client_api --answer\n");
    rt_kprintf("       hfp_client_api --reject\n");
    rt_kprintf("       hfp_client_api --volume  0-15\n");
    rt_kprintf("       hfp_client_api --dial phone_number\n");
    rt_kprintf("\n");
    rt_kprintf("Miscellaneous:\n");
    rt_kprintf("  --answer      Answer an incoming call\n");
    rt_kprintf("  --reject      Reject an incoming call\n");
    rt_kprintf("  --volume      Volume synchronization with AG\n");
    rt_kprintf("  --dial        Place a call with a specified number\n");
}


void hfp_client_api(int argc, char **argv)
{
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
        else if (rt_strcmp(argv[1], "--answer") == 0)
        {
            rk_hf_client_answer_call();
        }
        else if (rt_strcmp(argv[1], "--reject") == 0)
        {
            rk_hf_client_reject_call();
        }
        else if (rt_strcmp(argv[1], "--volume") == 0)
        {
            int volume = atoi(argv[3]);
            if (volume >= 0 && volume <= 15)
            {
                rk_hf_client_volume_update(RK_HF_VOLUME_CONTROL_TARGET_SPK, volume);
            }
            else
            {
                rt_kprintf("volume value %d is error, ranges 0 to 15 \r\n", volume);
            }
        }
        else if (rt_strcmp(argv[1], "--dial") == 0)
        {
            rk_hf_client_dial(argv[2]);
        }
        else
        {
            goto __usage;
        }

    }
    return ;

__usage:
    usage();
}

MSH_CMD_EXPORT(hfp_client_api, hfp client api test);
