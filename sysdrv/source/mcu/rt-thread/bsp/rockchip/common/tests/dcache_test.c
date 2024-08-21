/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */
#include "rtthread.h"
#include "hal_base.h"

#if defined(RT_USING_COMMON_TEST_DCACHE)
#if defined (__GNUC__)
#pragma GCC optimize 0
#endif

#define CACHE_LINE_ALIGN  ALIGN(CACHE_LINE_SIZE)

#ifdef ARCH_RISCV
#define __ISB()
#endif


#if defined(RKMCU_RK2108) || defined(RKMCU_PISCES)
/*-----------------------------------------------------------------------------
 *      Test implementation
 * Note: it must be compiled with -O0, otherwise the variable will be instead
 * with constant.
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *      Test cases
 *----------------------------------------------------------------------------*/

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_DCache_InvalidateAll()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();

    /* test cached value gets lost */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is invalidated
    HAL_DCACHE_Invalidate();

    // THEN the new value has been lost
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x0815U);

    /* tear down */
    __ISB();
}

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_DCache_InvalidateByRange()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();

    /* test cached value gets lost */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is invalidated
    HAL_DCACHE_InvalidateByRange((uint32_t)&value, sizeof(value));


    // THEN the new value has been lost
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x0815U);

    /* tear down */
    __ISB();
}

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_DCache_CleanByRange()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();

    /* test cached value is preserved */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is clean
    HAL_DCACHE_CleanByRange((uint32_t)&value, sizeof(value));

    // THEN the new value has been saved
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x4711U);

    /* tear down */
    __ISB();
}

void TC_DCache_CleanInvalidateByRange()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();

    /* test cached value is preserved */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is clean
    HAL_DCACHE_CleanInvalidateByRange((uint32_t)&value, sizeof(value));

    // THEN the new value has been saved
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x4711U);

    /* write to cache */
    value = 0x0815U;
    RT_ASSERT(value == 0x0815U);
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x4711U);

    /* tear down */
    __ISB();
}

void TC_DCache_CleanInvalidate()
{
    /* setup */
    uint32_t value = 0x0815U;
    uint32_t *value_alias = (uint32_t *)((uint32_t)&value - 0x1c000000);

    HAL_DCACHE_CleanInvalidate();

    /* test cached value is gets lost */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is clean
    HAL_DCACHE_CleanInvalidate();

    // THEN the new value has been lost
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x4711U);

    /* write to memory */
    value = 0x0815U;
    RT_ASSERT(value == 0x0815U);
    HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
    RT_ASSERT(*value_alias == 0x4711U);

    /* tear down */
    __ISB();
}

void TC_DCache_MaintainRange()
{
    /* setup */
    uint8_t value[64];
    uint8_t *value_alias = (uint8_t *)((uint32_t)value - 0x1c000000);
    uint32_t i, j, k, m, n, start, len;

    for (i = 0; i < sizeof(value); i++)
    {
        start = i;
        for (j = 0; j < sizeof(value) - start; j++)
        {
            len = j;
            //rt_kprintf("addr=%p, start=%d, len=%d\n", value, start, len);
            for (k = 0; k < sizeof(value); k++)
                value[k] = k;

            HAL_DCACHE_CleanInvalidate();

            // WHEN a value is written
            for (m = start; m < len; m++)
                value[m] = 'a' + m;

            // ... and the cache is invalidated
            HAL_DCACHE_CleanByRange((uint32_t)&value[start], len);


            // THEN the new value has been lost
            HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, sizeof(value));
            for (m = start; m < len; m++)
                RT_ASSERT(value_alias[m] == ('a' + m));
            // AND the other data are unchanged
            for (n = len; n < sizeof(value) - len; n++)
                RT_ASSERT(value_alias[n] == n);

            /* tear down */
            __ISB();
        }
    }
}

void TC_DCache_MaintainRange1()
{
    /* setup */
    uint8_t *value = NULL;
    uint8_t *value_alias = NULL;
    uint32_t size = 64;
    uint32_t i, j, k, m, n, start, len;

    value = rt_malloc_align(size, 32);
    if (!value)
    {
        rt_kprintf("malloc fail\n");
        return;
    }
    value_alias = (uint8_t *)((uint32_t)value - 0x1c000000);

    for (i = 0; i < size; i++)
    {
        start = i;
        for (j = 0; j < size - start; j++)
        {
            len = j;
            //rt_kprintf("addr=%p, start=%d, len=%d\n", value, start, len);
            for (k = 0; k < size; k++)
                value[k] = k;

            HAL_DCACHE_CleanInvalidate();

            // WHEN a value is written
            for (m = start; m < len; m++)
                value[m] = 'a' + m;

            // ... and the cache is invalidated
            HAL_DCACHE_CleanByRange((uint32_t)&value[start], len);


            // THEN the new value has been lost
            HAL_ICACHE_InvalidateByRange((uint32_t)value_alias, size);
            for (m = start; m < len; m++)
                RT_ASSERT(value_alias[m] == ('a' + m));
            // AND the other data are unchanged
            for (n = len; n < size - len; n++)
                RT_ASSERT(value_alias[n] == n);

            /* tear down */
            __ISB();
        }
    }

    rt_free_align(value);
}
#elif defined(SOC_SWALLOW) || defined(RKMCU_RK3588_NPU) || defined(RKMCU_RK3588_PMU) || defined(SOC_RV1106)

void TC_DCache_InvalidateAll()
{
    HAL_Status ret;

    ret = HAL_DCACHE_Invalidate();
    RT_ASSERT(ret != HAL_OK);

    rt_kprintf("%s pass\n", __FUNCTION__);
}

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_DCache_InvalidateByRange()
{
    /* setup */
    CACHE_LINE_ALIGN uint32_t value = 0x0815U;

    HAL_DCACHE_CleanInvalidate();

    /* test cached value gets lost */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is invalidated
    HAL_DCACHE_InvalidateByRange((uint32_t)&value, sizeof(value));


    // THEN the new value has been lost
    HAL_DCACHE_Disable();
    RT_ASSERT(value == 0x0815U);

    // FINAL resume cache
    HAL_DCACHE_Enable();

    /* tear down */
    __ISB();

    rt_kprintf("%s pass\n", __FUNCTION__);
}

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
void TC_DCache_CleanByRange()
{
    /* setup */
    uint32_t value = 0x0815U;

    HAL_DCACHE_CleanInvalidate();

    /* test cached value is preserved */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is clean
    HAL_DCACHE_CleanByRange((uint32_t)&value, sizeof(value));

    // THEN the new value has been saved
    HAL_DCACHE_Disable();
    RT_ASSERT(value == 0x4711U);

    // FINAL resume cache
    HAL_DCACHE_Enable();

    /* tear down */
    __ISB();

    rt_kprintf("%s pass\n", __FUNCTION__);
}

void TC_DCache_CleanInvalidateByRange()
{
    /* setup */
    uint32_t value = 0x0815U;

    HAL_DCACHE_CleanInvalidate();

    /* test cached value is preserved */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is clean
    HAL_DCACHE_CleanInvalidateByRange((uint32_t)&value, sizeof(value));

    // THEN the new value has been saved
    HAL_DCACHE_Disable();
    RT_ASSERT(value == 0x4711U);


    // FINAL resume cache
    HAL_DCACHE_Enable();

    /* tear down */
    __ISB();

    rt_kprintf("%s pass\n", __FUNCTION__);
}

void TC_DCache_CleanInvalidate()
{
    /* setup */
    uint32_t value = 0x0815U;

    HAL_DCACHE_CleanInvalidate();

    /* test cached value is preserved */

    // WHEN a value is written
    value = 0x4711U;

    // ... and the cache is clean
    HAL_DCACHE_CleanInvalidate();

    // THEN the new value has been saved
    HAL_DCACHE_Disable();
    RT_ASSERT(value == 0x4711U);


    // FINAL resume cache
    HAL_DCACHE_Enable();

    /* tear down */
    __ISB();

    rt_kprintf("%s pass\n", __FUNCTION__);
}

void TC_DCache_MaintainRange()
{
    /* setup */
    uint8_t value[64];
    uint32_t i, j, k, m, n, start, len;

    for (i = 0; i < sizeof(value); i++)
    {
        start = i;
        for (j = 0; j < sizeof(value) - start; j++)
        {
            len = j;
            //rt_kprintf("addr=%p, start=%d, len=%d\n", value, start, len);
            for (k = 0; k < sizeof(value); k++)
                value[k] = k;

            HAL_DCACHE_CleanInvalidate();

            // WHEN a value is written
            for (m = start; m < len; m++)
                value[m] = 'a' + m;

            // ... and the cache is clean
            HAL_DCACHE_CleanByRange((uint32_t)&value[start], len);


            // THEN the new value has been lost
            HAL_DCACHE_Disable();
            for (m = start; m < len; m++)
                RT_ASSERT(value[m] == ('a' + m));
            // AND the other data are unchanged
            for (n = len; n < sizeof(value) - len; n++)
                RT_ASSERT(value[n] == n);

            HAL_DCACHE_Enable();

            /* tear down */
            __ISB();
        }
    }

    rt_kprintf("%s pass\n", __FUNCTION__);
}

void TC_DCache_MaintainRange1()
{
    /* setup */
    uint8_t *value = NULL;
    uint32_t size = 64;
    uint32_t i, j, k, m, n, start, len;

    value = rt_malloc_align(size, 32);
    if (!value)
    {
        rt_kprintf("malloc fail\n");
        return;
    }

    for (i = 0; i < size; i++)
    {
        start = i;
        for (j = 0; j < size - start; j++)
        {
            len = j;
            //rt_kprintf("addr=%p, start=%d, len=%d\n", value, start, len);
            for (k = 0; k < size; k++)
                value[k] = k;

            HAL_DCACHE_CleanInvalidate();

            // WHEN a value is written
            for (m = start; m < len; m++)
                value[m] = 'a' + m;

            // ... and the cache is clean
            HAL_DCACHE_CleanByRange((uint32_t)&value[start], len);


            // THEN the new value has been save
            HAL_DCACHE_Disable();
            for (m = start; m < len; m++)
                RT_ASSERT(value[m] == ('a' + m));
            // AND the other data are unchanged
            for (n = len; n < size - len; n++)
                RT_ASSERT(value[n] == n);

            HAL_DCACHE_Enable();

            /* tear down */
            __ISB();
        }
    }

    rt_free_align(value);

    rt_kprintf("%s pass\n", __FUNCTION__);
}

#endif

void test_dcache()
{
    TC_DCache_InvalidateAll();
    TC_DCache_InvalidateByRange();
    TC_DCache_CleanByRange();
    TC_DCache_CleanInvalidateByRange();
    TC_DCache_CleanInvalidate();
    TC_DCache_MaintainRange();
    TC_DCache_MaintainRange1();
    rt_kprintf("dcache test pass\n");
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(test_dcache, dcache test);
#endif

#endif
