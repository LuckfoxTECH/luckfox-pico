/**
  * Copyright (c) 2021 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    cyw20721_test.c
  * @version V0.1
  * @brief   cyw20721 test case
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-01-04     Chris Zhong     first implementation
  *
  ******************************************************************************
  */
#include <stdio.h>
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "bt_soc_platform_hci.h"
#include "bt_soc_platform_task.h"
#include "bt_soc_firmware.h"
#include "bt_soc_uart.h"
#include "hci_control_api.h"

cybtsoc_result_t cyw_get_version(void *version)
{
    uint16_t cmd = HCI_CONTROL_MISC_COMMAND_GET_VERSION;

    cyw_wakeup_ctrl(1);

    cyw_wait_cnt_increase();
    wiced_hci_send_command(cmd, 0, 0);
    cyw_wait_replay(cmd, version);
    cyw_wait_cnt_decrease();

    cyw_wakeup_ctrl(0);

    return 0;
}

cybtsoc_result_t cyw_get_rtc_time(void *time)
{
    uint16_t cmd = HCI_CONTROL_MISC_COMMAND_GET_TIME;

    cyw_wakeup_ctrl(1);

    cyw_wait_cnt_increase();
    wiced_hci_send_command(cmd, 0, 0);
    cyw_wait_replay(cmd, time);
    cyw_wait_cnt_decrease();

    cyw_wakeup_ctrl(0);

    return 0;
}

cybtsoc_result_t cyw_set_rtc_time(void *time)
{
    uint16_t cmd = HCI_CONTROL_MISC_COMMAND_SET_TIME;

    cyw_wakeup_ctrl(1);

    cyw_wait_cnt_increase();
    wiced_hci_send_command(cmd, time, 12);
    cyw_wait_replay(cmd, time);
    cyw_wait_cnt_decrease();

    cyw_wakeup_ctrl(0);

    return 0;
}

#ifdef RT_USING_FINSH
void cy_send(int argc, char **argv)
{
    uint16_t len = 0, cmd = 0;
    char buffer[16];
    int i;

    if (argc < 2)
    {
        rt_kprintf("Usage: cy_send cmd arg ...\n");
        rt_kprintf("such as: \nget version: cy_send ff02\n");
        rt_kprintf("get rtc time: cy_send ff03\n");
        //rt_kprintf("set rtc time 01:29:44: cy_send ff04 2C 00 1D 00 01 00 01 00 00 00 DA 07\n");
        rt_kprintf("get system tick in us: cy_send ff05\n");
        rt_kprintf("set a timeout event in 1000ms(0x03e8): cy_send ff06 e8 03 00 00\n");
        rt_kprintf("get system wakeup event, 0: power key; 1: timer; 2: sensor irq; 3: BT: cy_send ff07\n");
        return;
    }

    if (argc > 1)
        cmd = strtol(argv[1], NULL, 16);

    if (argc > 2)
    {
        for (i = 0; i < argc - 2; i++)
            buffer[i] = strtol(argv[2 + i], NULL, 16);
        len = argc - 2;
    }

    rt_kprintf("cy_send 0x%x, 0x%x, %d\n", cmd, buffer, len);

    cyw_wakeup_ctrl(1);

    cyw_wait_cnt_increase();
    wiced_hci_send_command(cmd, buffer, len);
    cyw_wait_replay(cmd, buffer);
    cyw_wait_cnt_decrease();

    cyw_wakeup_ctrl(0);
}

void cy_version(int argc, char **argv)
{
    uint8_t ver[15];

    cyw_get_version(ver);
}

void cy_update(int argc, char **argv)
{
    uint32_t rate = BAUD_RATE_3000000;
    if (argc > 1)
        rate = BAUD_RATE_115200;

    cyw_wakeup_ctrl(1);
    cybtsoc_bt_start_firmware_dowload(rate);
}

void shut_cmd(int argc, char **argv)
{
    cyw_wakeup_ctrl(1);
    wiced_hci_send_command(0xa0ff, 0, 0);
}

#include <finsh.h>
MSH_CMD_EXPORT(cy_send, cy20721 send cmd. e.g: cy_test 0xa0ff);
MSH_CMD_EXPORT(cy_version, get cy20721 version. e.g: cy_version);
MSH_CMD_EXPORT(cy_update, cy20721 firmware update. e.g: cy_update);
MSH_CMD_EXPORT(shut_cmd, cy20721 shut down rk 2108 by uart cmd. e.g: shut_cmd);
#endif