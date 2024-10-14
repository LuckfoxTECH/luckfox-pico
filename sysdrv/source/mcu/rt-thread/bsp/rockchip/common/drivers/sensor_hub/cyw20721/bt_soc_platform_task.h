/*******************************************************************************
* \file cybtsoc_platform_task.h
*
* \brief
* Provides API to access BT SOC Internal Task.
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

#ifndef CYBTSOC_PLATFORM_TASK_H_
#define CYBTSOC_PLATFORM_TASK_H_

#include "bt_soc_platform_hci.h"

/*****************************************************************************
 *                                Constants
 *****************************************************************************/
#define BT_EVT_TASK_PRIORITY                    (CY_RTOS_PRIORITY_ABOVENORMAL)
#define BT_EVT_TASK_STACK_SIZE                  (0x1800)

#define WICED_HCI_TASK_PRIORITY                 (CY_RTOS_PRIORITY_HIGH)
#define WICED_HCI_TASK_STACK_SIZE               (0x1000)

#define  CYBTSOC_TASK_DEFAULT_POOL_SIZE        (2048)

#define WICED_HCI_SEMAPHORE_MAX_COUNT  (1)
#define WICED_HCI_SEMAPHORE_INIT_COUNT (0)

#define  CYBTSOC_INVALID_QUEUE_UTILIZATION     (0xFF)

/*****************************************************************************
 *                           Type Definitions
 *****************************************************************************/

typedef struct
{
    uint16_t         event;    /**< event id */
    uint16_t         length;   /**< payload length */
} WICED_PKT_MSG_HDR;

/**
 * Message structure is used to communicate between tasks
 */
typedef struct
{
    uint16_t         event;    /**< event id */
    uint16_t         length;   /**< payload length */
} WICED_PKT_EVT_HDR;

#define BT_EVT_TASK_QUEUE_COUNT        (32)
#define BT_EVT_TASK_QUEUE_ITEM_SIZE    (sizeof(WICED_PKT_EVT_HDR *))

extern rt_thread_t cybtsoc_hci_task_handle;

extern rt_sem_t cybtsoc_wiced_hci_task_sem;
extern rt_mq_t cybtsoc_bt_evt_task_queue;

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 *                           Function Declarations
 ****************************************************************************/
cybtsoc_result_t cybtsoc_set_sem_to_wiced_hci_task(void);
void cybtsoc_hci_task_irq_sem_mask(void);
void cybtsoc_hci_task_irq_sem_unmask(void);

/**
 *  internal wiced hci task func to handle the rx data from HCI Uart
 */
void cybtsoc_wiced_hci_rx_task(void *arg);

/**
 *  internal bt event handle task func to handle the event/rsp quene from wiced hci task
 */
void cybtsoc_bt_evt_task(void *arg);

cybtsoc_result_t cybtsoc_platform_task_mempool_init(void);
void *cybtsoc_platform_task_mempool_malloc(uint32_t size);
void cybtsoc_platform_task_mempool_free(void *p_mem);
void cybtsoc_platform_task_mempool_deinit(void);

cybtsoc_result_t cybtsoc_platform_task_init(char *port);
void cybtsoc_platform_task_deinit(void);

cybtsoc_result_t cybtsoc_send_msg_to_bt_evt_task(void *p_bt_msg);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CYBTSOC_PLATFORM_TASK_H_ */