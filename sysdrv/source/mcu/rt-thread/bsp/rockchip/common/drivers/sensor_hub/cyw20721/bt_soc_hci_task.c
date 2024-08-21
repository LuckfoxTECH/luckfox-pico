/*******************************************************************************
* \file cybtsoc_hci_task.c
*
* \brief
* This file provides functions for hci task to handle the hci uart read data
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "bt_soc_platform_hci.h"
#include "bt_soc_platform_task.h"
#include "bt_soc_uart.h"

/******************************************************
 *                      Macros
 ******************************************************/


/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               extern Function Declarations
 ******************************************************/


/******************************************************
 *              extern Variable Definitions
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/
static void handle_rx_wiced_pkt(void);
static void handle_rx_unknown_pkt(void);
static void handle_hci_uart_rx_data(void);

/******************************************************
 *               Variable Definitions
 ******************************************************/
static uint8_t isHciTaskSemMasked = 0;

/******************************************************
 *               Function Definitions
 ******************************************************/
cybtsoc_result_t cybtsoc_set_sem_to_wiced_hci_task(void)
{
    rt_sem_release(cybtsoc_wiced_hci_task_sem);

    return CYBTSOC_SUCCESS;
}

/**
 *  Callback from ISR
 *
 */
static rt_err_t hci_uart_rx_ready_callback(rt_device_t dev, rt_size_t size)
{
    cybtsoc_result_t result;

    if (isHciTaskSemMasked == 0)
    {
        result = cybtsoc_set_sem_to_wiced_hci_task();
        if (CYBTSOC_SUCCESS != result)
        {

        }
    }

    return 0;
}

void cybtsoc_hci_task_irq_sem_mask(void)
{
    isHciTaskSemMasked = 1;
}

void cybtsoc_hci_task_irq_sem_unmask(void)
{
    isHciTaskSemMasked = 0;
}

void cybtsoc_wiced_hci_rx_task(void *arg)
{
    cybtsoc_result_t result;

    rk_uart_register_rx_ready_cb(hci_uart_rx_ready_callback);

    printf("wiced hci task start\r\n");
    result = rk_uart_open();
    printf("HCI Uart Up, %d\r\n", result);

    while (1)
    {
        rt_sem_take(cybtsoc_wiced_hci_task_sem, RT_WAITING_FOREVER);

        handle_hci_uart_rx_data();
    }
}

static void handle_rx_wiced_pkt(void)
{
    cybtsoc_result_t result;
    WICED_PKT_MSG_HDR rx_pack = {0};
    uint32_t read_len = sizeof(WICED_PKT_MSG_HDR);
    WICED_PKT_MSG_HDR *p_bt_msg;
    void *p = NULL;

    result = rk_uart_read((char *)&rx_pack, &read_len, 300);
    if (result == CYBTSOC_SUCCESS)
    {
        /* check and get all parameter back */
        read_len = rx_pack.length;

        p_bt_msg = (WICED_PKT_MSG_HDR *)cybtsoc_platform_task_mempool_malloc(sizeof(WICED_PKT_MSG_HDR) + rx_pack.length);
        p_bt_msg->event = rx_pack.event;
        p_bt_msg->length = rx_pack.length;

        if (read_len > 0)
        {
            p = (void *)(p_bt_msg + 1);
            result = rk_uart_read((char *)p, &read_len, 0);
        }

        if (result == CYBTSOC_SUCCESS)
        {
            result = cybtsoc_send_msg_to_bt_evt_task((void *)p_bt_msg);
            if (result != CYBTSOC_SUCCESS)
            {
                cybtsoc_platform_task_mempool_free((void *)p_bt_msg);

                printf("[wiced]Failed(0x%X) to Send Queue\r\n", result);
            }
        }
        else
        {
            printf("[wiced]Failed(0x%X) to Read Event Parameter\r\n", result);
        }
    }
    else
    {
        printf("[wiced]Failed(0x%X) to get pkt header\r\n", result);
    }
}

static void handle_rx_unknown_pkt(void)
{
    /* do nothing so far */
}

static void handle_hci_uart_rx_data(void)
{
    cybtsoc_result_t result;
    uint32_t read_len = 1;
    char pkt_type = 0;

    result = rk_uart_read(&pkt_type, &read_len, 0);
    if (result == CYBTSOC_SUCCESS)
    {
        switch (pkt_type)
        {
        case HCI_PACKET_TYPE_WICED_PKT:
            printf("[wiced]Get Pkt Type\r\n");
            handle_rx_wiced_pkt();
            break;
        default:
            //  printf("[wiced]unknown type\r\n");
            handle_rx_unknown_pkt();
            break;
        }
    }
}