/**
  * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pstore.c
  * @author
  * @version V0.1
  * @date    11-Mar-2021
  * @brief
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup PSTORE
 *  @{
 */

/** @defgroup PSTORE_How_To_Use How To Use
 *  @{

 @} */

#include "hal_base.h"
#include <rtdevice.h>

#if defined(RT_USING_PSTORE)

#define PERSISTENT_RAM_SIG (0x43474244) /* DBGC */
#define PERSISTENT_BUFFER_SIZE  (PERSISTENT_RAM_SIZE - 12)

struct persistent_ram_buffer
{
    uint32_t    sig;
    uint32_t    start;
    uint32_t    size;
    uint8_t     data[0];
};

static void init_pstore(void)
{
    struct persistent_ram_buffer *rb = (struct persistent_ram_buffer *)PERSISTENT_RAM_ADDR;

    RT_ASSERT(rb != RT_NULL);
    RT_ASSERT(((uintptr_t)rb & 0x3) == 0);

    if (rb->sig != PERSISTENT_RAM_SIG)
    {
        rb->sig = PERSISTENT_RAM_SIG;
        rb->start = 0x0;
        rb->size = 0x0;
    }

    if (rb->size > PERSISTENT_BUFFER_SIZE)
        rb->size = PERSISTENT_BUFFER_SIZE;

    if (rb->start >= PERSISTENT_BUFFER_SIZE)
        rb->start = 0;
}

static void write_log_to_ram(const char *str, int flush)
{
    struct persistent_ram_buffer *rb =
        (struct persistent_ram_buffer *)PERSISTENT_RAM_ADDR;
    uint8_t *src = (uint8_t *)str, *dst;

    dst = rb->data + rb->start;
    while (*src)
    {
        if (*src == '\n')
        {
            *dst++ = '\r';

            if (rb->size < PERSISTENT_BUFFER_SIZE)
                rb->size++;

            rb->start++;
            if (rb->start >= PERSISTENT_BUFFER_SIZE)
            {
                rb->start = 0;
                dst = rb->data;
            }
        }

        *dst++ = *src++;

        if (rb->size < PERSISTENT_BUFFER_SIZE)
            rb->size++;

        rb->start++;
        if (rb->start >= PERSISTENT_BUFFER_SIZE)
        {
            rb->start = 0;
            dst = rb->data;
        }
    }
}

static int pstore_dev_init(void)
{
    init_pstore();
    rt_console_set_output_hook(write_log_to_ram);
    return RT_EOK;
}

INIT_BOARD_EXPORT(pstore_dev_init);

#endif

/** @} */  // PSTORE

/** @} */  // RKBSP_Driver_Reference
