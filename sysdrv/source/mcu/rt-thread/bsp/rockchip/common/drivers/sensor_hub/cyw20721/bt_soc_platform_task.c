/*******************************************************************************
* \file cybtsoc_platform_task.c
*
* \brief
* This file provides functions to manage internal task for BT SoC driver
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
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

//#include "cybtsoc_platform.h"
#include "bt_soc_platform_task.h"
#include "bt_soc_platform_hci.h"
#include "bt_soc_uart.h"

/******************************************************
 *                      Macros
 ******************************************************/


/******************************************************
 *                    Constants
 ******************************************************/
/* control the size of bt task mem pool */
#define    BT_Q_MEM_SMALL_POOL_SIZE             (64U)
#define    BT_Q_MEM_SMALL_POOL_COUNT            (32U)

#define    BT_Q_MEM_MIDDLE_POOL_SIZE            (288U)
#define    BT_Q_MEM_MIDDLE_POOL_COUNT           (16U)

#define    BT_Q_MEM_LARGE_POOL_SIZE             (544U)
#define    BT_Q_MEM_LARGE_POOL_COUNT            (8U)

#define    BT_Q_MEM_EXTRA_POOL_SIZE             (1056U)
#define    BT_Q_MEM_EXTRA_POOL_COUNT            (2U)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef enum
{

    MEM_TYPE_SMALL = 0,
    MEM_TYPE_MIDDLE,
    MEM_TYPE_LARGE,
    MEM_TYPE_EXTRA
} btsoc_task_mem_size_type ;

typedef struct
{

    uint8_t status;       /* the block is free or not. 0x00 is idle, others means not free */
    uint8_t type;       /* value refer btsoc_task_mem_size_type */
    uint8_t  data[BT_Q_MEM_SMALL_POOL_SIZE];

} btsoc_task_mem_block_small;

typedef struct
{

    uint8_t status;       /* the block is free or not. 0x00 is idle, others means not free */
    uint8_t type;       /* value refer btsoc_task_mem_size_type */
    uint8_t  data[BT_Q_MEM_MIDDLE_POOL_SIZE];

} btsoc_task_mem_block_middle;

typedef struct
{

    uint8_t status;       /* the block is free or not. 0x00 is idle, others means not free */
    uint8_t type;       /* value refer btsoc_task_mem_size_type */
    uint8_t  data[BT_Q_MEM_LARGE_POOL_SIZE];

} btsoc_task_mem_block_large;

typedef struct
{
    uint8_t status;       /* the block is free or not. 0x00 is idle, others means not free */
    uint8_t type;       /* value refer btsoc_task_mem_size_type */
    uint8_t data[BT_Q_MEM_EXTRA_POOL_SIZE];

} btsoc_task_mem_block_extra;


typedef struct
{
    btsoc_task_mem_block_small mem_pool_small[BT_Q_MEM_SMALL_POOL_COUNT];
    btsoc_task_mem_block_middle mem_pool_middle[BT_Q_MEM_MIDDLE_POOL_COUNT];
    btsoc_task_mem_block_large mem_pool_large[BT_Q_MEM_LARGE_POOL_COUNT];
    btsoc_task_mem_block_extra mem_pool_extra[BT_Q_MEM_EXTRA_POOL_COUNT];

} btsoc_task_mem_all_pool_t;


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
static btsoc_task_mem_all_pool_t btsoc_task_all_mem_pool;

/* current index for each tabel */
static uint32_t mem_pool_small_cur_idx = 0;
static uint32_t mem_pool_middle_cur_idx = 0;
static uint32_t mem_pool_large_cur_idx = 0;
static uint32_t mem_pool_extra_cur_idx = 0;

/* check the size */
const uint32_t btsoc_total_pool_size = sizeof(btsoc_task_mem_all_pool_t);

/******************************************************
 *               Variable Definitions
 ******************************************************/
rt_thread_t cybtsoc_hci_task_handle;
rt_thread_t cybtsoc_bt_task_handle;

rt_sem_t cybtsoc_wiced_hci_task_sem = RT_NULL;
rt_mq_t cybtsoc_bt_evt_task_queue;

rt_mutex_t  cybtsoc_task_mem_pool_mutex;

//void * p_bt_msg;


/******************************************************
 *               Function Definitions
 ******************************************************/
/**
 *  Note: Use a simple mem table to demo, User can also change
 *  to the RTOS mempool mangement function  to replace the
 *  current implement below:
 *   cybtsoc_platform_task_mempool_init(void);
 *   cybtsoc_platform_task_mempool_malloc(uint32_t size);
 *   cybtsoc_platform_task_mempool_free(void * pmem);
 *   cybtsoc_platform_task_mempool_deinit(void);
 */
cybtsoc_result_t cybtsoc_platform_task_mempool_init(void)
{
    /* clear or current index */
    mem_pool_small_cur_idx = 0;
    mem_pool_middle_cur_idx = 0;
    mem_pool_large_cur_idx = 0;
    mem_pool_extra_cur_idx = 0;

    /* clean the table */
    rt_memset((void *)&btsoc_task_all_mem_pool, 0, sizeof(btsoc_task_mem_all_pool_t));

    printf("mem pool size %ld byte\r\n", btsoc_total_pool_size);

    cybtsoc_task_mem_pool_mutex = rt_mutex_create("mem_pool_mutex", RT_IPC_FLAG_FIFO);

    return CYBTSOC_SUCCESS;
}

void *cybtsoc_platform_task_mempool_malloc(uint32_t size)
{
    void *p_mem = NULL;
    btsoc_task_mem_size_type pool_type;

    if (size <= BT_Q_MEM_SMALL_POOL_SIZE)
    {
        pool_type = MEM_TYPE_SMALL;
    }
    else if (size <= BT_Q_MEM_MIDDLE_POOL_SIZE)
    {
        pool_type = MEM_TYPE_MIDDLE;
    }
    else if (size <= BT_Q_MEM_LARGE_POOL_SIZE)
    {
        pool_type = MEM_TYPE_LARGE;
    }
    else if (size <= BT_Q_MEM_EXTRA_POOL_SIZE)
    {
        pool_type = MEM_TYPE_EXTRA;
    }
    else
    {
        return NULL;   /* not support larger than BT_Q_MEM_EXTRA_POOL_SIZE size */
    }

    rt_mutex_take(cybtsoc_task_mem_pool_mutex, RT_WAITING_FOREVER);

    switch (pool_type)
    {
    case MEM_TYPE_SMALL:
    {
        uint32_t next_idx = mem_pool_small_cur_idx + 1;
        if (next_idx == BT_Q_MEM_SMALL_POOL_COUNT)
        {
            next_idx = 0;
        }

        if (btsoc_task_all_mem_pool.mem_pool_small[next_idx].status != 0)
        {
            /* no idle pool to allocate */
            return NULL;
        }
        else
        {
            btsoc_task_all_mem_pool.mem_pool_small[next_idx].status = 1;  /* mark the pool is using */
            p_mem = btsoc_task_all_mem_pool.mem_pool_small[next_idx].data;
            mem_pool_small_cur_idx = next_idx;
        }

    }
    break;

    case MEM_TYPE_MIDDLE:
    {
        uint32_t next_idx = mem_pool_middle_cur_idx + 1;
        if (next_idx == BT_Q_MEM_MIDDLE_POOL_COUNT)
        {
            next_idx = 0;
        }

        if (btsoc_task_all_mem_pool.mem_pool_middle[next_idx].status != 0)
        {
            /* no idle pool to allocate */
            return NULL;
        }
        else
        {
            btsoc_task_all_mem_pool.mem_pool_middle[next_idx].status = 1;  /* mark the pool is using */
            p_mem = btsoc_task_all_mem_pool.mem_pool_middle[next_idx].data;
            mem_pool_middle_cur_idx = next_idx;
        }

    }
    break;

    case MEM_TYPE_LARGE:
    {
        uint32_t next_idx = mem_pool_large_cur_idx + 1;
        if (next_idx == BT_Q_MEM_LARGE_POOL_COUNT)
        {
            next_idx = 0;
        }

        if (btsoc_task_all_mem_pool.mem_pool_large[next_idx].status != 0)
        {
            /* no idle pool to allocate */
            return NULL;
        }
        else
        {
            btsoc_task_all_mem_pool.mem_pool_large[next_idx].status = 1;  /* mark the pool is using */
            p_mem = btsoc_task_all_mem_pool.mem_pool_large[next_idx].data;
            mem_pool_large_cur_idx = next_idx;
        }
    }
    break;

    case MEM_TYPE_EXTRA:
    {
        uint32_t next_idx = mem_pool_extra_cur_idx + 1;
        if (next_idx == BT_Q_MEM_EXTRA_POOL_COUNT)
        {
            next_idx = 0;
        }

        if (btsoc_task_all_mem_pool.mem_pool_extra[next_idx].status != 0)
        {
            /* no idle pool to allocate */
            return NULL;
        }
        else
        {
            btsoc_task_all_mem_pool.mem_pool_extra[next_idx].status = 1;  /* mark the pool is using */
            p_mem = btsoc_task_all_mem_pool.mem_pool_extra[next_idx].data;
            mem_pool_extra_cur_idx = next_idx;
        }
    }
    break;
    }

    rt_mutex_release(cybtsoc_task_mem_pool_mutex);

    return p_mem;
}

void cybtsoc_platform_task_mempool_free(void *p_mem)
{
    /* check p_mem is within the mem_pool */
    if ((void *)btsoc_task_all_mem_pool.mem_pool_small[0].data <= p_mem
            && p_mem <= (void *)btsoc_task_all_mem_pool.mem_pool_extra[BT_Q_MEM_EXTRA_POOL_COUNT - 1].data)
    {
        uint8_t *status = (uint8_t *)p_mem - 2;

        rt_mutex_take(cybtsoc_task_mem_pool_mutex, RT_WAITING_FOREVER);
//        cybtsoc_platform_ops.disable_irq();

        *status = 0;   /* mark the pool is unused */

//        cybtsoc_platform_ops.enable_irq();
        rt_mutex_release(cybtsoc_task_mem_pool_mutex);
    }
    /* otherwise, do nothing */
}

/**
 * may have issue when the task still using the data pool, need stop all relate task first
 * or call in cybtsoc_platform_task_deinit()
 */
void cybtsoc_platform_task_mempool_deinit(void)
{
    rt_mutex_take(cybtsoc_task_mem_pool_mutex, RT_WAITING_FOREVER);
    /* clear the table */
    mem_pool_small_cur_idx = 0;
    mem_pool_middle_cur_idx = 0;
    mem_pool_large_cur_idx = 0;
    mem_pool_extra_cur_idx = 0;

    /* clean the table */
    rt_memset((void *)&btsoc_task_all_mem_pool, 0, sizeof(btsoc_task_mem_all_pool_t));

    /* may have issue when the task still using the data pool, need stop all relate task first */
    rt_mutex_release(cybtsoc_task_mem_pool_mutex);

    rt_mutex_delete(cybtsoc_task_mem_pool_mutex);
}

cybtsoc_result_t cybtsoc_platform_task_init(char *port)
{
    cybtsoc_platform_task_mempool_init();

    cybtsoc_wiced_hci_task_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(cybtsoc_wiced_hci_task_sem != RT_NULL);

    cybtsoc_bt_evt_task_queue = rt_mq_create("bt_evt_mq", sizeof(WICED_PKT_MSG_HDR *), 2, RT_IPC_FLAG_FIFO);
    RT_ASSERT(cybtsoc_bt_evt_task_queue != RT_NULL);

    /* create the hci task to handle uart rx data */
    cybtsoc_hci_task_handle = rt_thread_create("CYBTSOC_HCI_Task", cybtsoc_wiced_hci_rx_task, RT_NULL, WICED_HCI_TASK_STACK_SIZE, 5, 10);
    RT_ASSERT(cybtsoc_hci_task_handle != RT_NULL);

    /* create the bt evt task to handle bt event or rsp
    cybtsoc_bt_task_handle = rt_thread_create("CYBTSOC_BT_Task", cybtsoc_bt_evt_task, RT_NULL, BT_EVT_TASK_STACK_SIZE, 5, 10);
    RT_ASSERT(cybtsoc_hci_task_handle != RT_NULL);
    */

    rk_uart_init(port);

    rt_thread_startup(cybtsoc_hci_task_handle);

    return 0;
}

/**
 *  No Implement so far
 */
void cybtsoc_platform_task_deinit(void)
{

}
