/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <rtthread.h>

#if defined(RT_USING_LARGE_HEAP) && defined(RT_USING_COMMON_TEST_LARGE_HEAP)

#include <stdint.h>
#include <stdlib.h>

#include "drv_heap.h"

extern int _large_heap_size;
extern int memcheck(void);

void large_heap_test(void)
{
    void **ptr;
    rt_size_t size;
    uint32_t i, num, frees, allocs;
    rt_size_t heap_size;

    heap_size = (rt_size_t)&_large_heap_size;
    num = heap_size / RT_LARGE_MALLOC_THRRESH + 1;
    ptr = (void **)rt_malloc(sizeof(void *) * num);
    if (!ptr)
    {
        rt_kprintf("malloc fail\n");
        return;
    }
    rt_memset(ptr, 0, sizeof(void *) * num);

    i = 0;
    do
    {
        srand(rt_tick_get());
        size = rand() % heap_size;
        size = size > RT_LARGE_MALLOC_THRRESH ? size : RT_LARGE_MALLOC_THRRESH;

        ptr[i] = rt_malloc_large(size);
        if (ptr[i])
        {
            rt_kprintf("malloc large success, size=%d, addr=%p\n", size,  ptr[i]);
            memcheck();
            heap_size -= (size + 24);
            i++;
        }
        else
        {
            rt_kprintf("malloc large fail, size=%d, heap_size = %d\n", size, heap_size);
            break;
        }

    }
    while (heap_size > (RT_LARGE_MALLOC_THRRESH + 36));

    allocs = i;
    frees = 0;
    do
    {
        srand(rt_tick_get());
        i = rand() % allocs;
        if (ptr[i])
        {
            rt_free_large(ptr[i]);
            rt_kprintf("free large success, addr=%p\n", ptr[i]);
            memcheck();
            frees++;
            ptr[i] = NULL;
        }
    }
    while (frees < allocs);
    rt_kprintf("rt_malloc_large test pass\n");

    heap_size = (rt_size_t)&_large_heap_size;
    num = heap_size / RT_LARGE_MALLOC_THRRESH + 1;
    rt_memset(ptr, 0, sizeof(void *) * num);

    i = 0;
    do
    {
        srand(rt_tick_get());
        size = rand() % heap_size;
        size = size > RT_LARGE_MALLOC_THRRESH ? size : RT_LARGE_MALLOC_THRRESH;

        ptr[i] = rt_dma_malloc_large(size);
        if (ptr[i])
        {
            rt_kprintf("dma malloc large success, size=%d, addr=%p\n", size,  ptr[i]);
            memcheck();
            heap_size -= (size + 88);
            i++;
        }
        else
        {
            rt_kprintf("dma malloc large fail, size=%d, heap_size = %d\n", size, heap_size);
            break;
        }

    }
    while (heap_size > (RT_LARGE_MALLOC_THRRESH + 100));

    allocs = i;
    frees = 0;
    do
    {
        srand(rt_tick_get());
        i = rand() % allocs;
        if (ptr[i])
        {
            rt_kprintf("dma free large success, addr=%p\n", ptr[i]);
            rt_dma_free_large(ptr[i]);
            memcheck();
            frees++;
            ptr[i] = NULL;
        }
    }
    while (frees < allocs);
    rt_kprintf("rt_dma_malloc_large test pass\n");

    rt_free(ptr);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(large_heap_test, large heap test);
#endif

#endif
