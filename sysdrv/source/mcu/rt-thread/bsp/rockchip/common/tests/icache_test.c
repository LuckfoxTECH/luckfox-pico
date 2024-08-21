/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */
#include "rtthread.h"
#include "hal_base.h"

#if defined(RT_USING_COMMON_TEST_ICACHE)

/*-----------------------------------------------------------------------------
 *      Test implementation
 * Note: it must be compiled with -O0, otherwise the variable will be instead
 * with constant.
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *      Test cases
 *----------------------------------------------------------------------------*/

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_ICache_InvalidateAll()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();
    /* now the value has been write back */

    // WHEN a value is written
    *value_alias = 0x4711U;

    // ... and the cache is invalidated
    HAL_ICACHE_Invalidate();
    RT_ASSERT(*value_alias == 0x4711U);

    value = 0x55aaU;
    HAL_DCACHE_CleanByRange((uint32_t)&value, sizeof(value));

    // THEN the new value has been lost
    RT_ASSERT(*value_alias == 0x4711U);

    HAL_ICACHE_Invalidate();
    RT_ASSERT(*value_alias == 0x55aaU);

    /* tear down */
    __ISB();
}

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_ICache_InvalidateByRange()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();
    /* now the value has been write back */

    // WHEN a value is written
    *value_alias = 0x4711U;

    // ... and the cache is invalidated
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x4711U);

    value = 0x55aaU;
    HAL_DCACHE_CleanByRange((uint32_t)&value, sizeof(value));

    // THEN the new value has been lost
    RT_ASSERT(*value_alias == 0x4711U);

    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x55aaU);

    /* tear down */
    __ISB();
}

void TC_ICache_Disable()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    // icache & sram was the same value from now
    *value_alias = 0x4711U;

    // sram was modified
    value = 0x55aaU;
    HAL_DCACHE_CleanByRange((uint32_t)&value, sizeof(value));

    HAL_ICACHE_Disable();
    RT_ASSERT(*value_alias == 0x4711U);
}

void TC_Cache_Range()
{
    uint32_t start_addr[] = {0, 0x04000000, 0x18000000};
    uint32_t size[] = {0x2000, 0x100000, 0x1000000};
    uint32_t *ptr;
    uint32_t i, j, len;

    for (i = 0; i < (sizeof(start_addr) / sizeof(uint32_t)); i++)
    {
        ptr = (uint32_t *)start_addr[i];
        len = size[i] / sizeof(uint32_t);
        rt_kprintf("testing: ptr=%p, len=%d\n", ptr, len);
        for (j = 0; j < len; j++)
        {
            if (ptr[j] == 0x55aa)
                rt_kprintf("%p: %x\n", &ptr[j], ptr[j]);
        }
    }
}

void test_icache()
{
    TC_ICache_InvalidateAll();
    TC_ICache_InvalidateByRange();
    TC_ICache_Disable();
    TC_Cache_Range();
    rt_kprintf("icache test pass\n");
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(test_icache, icache test);
#endif

#endif
