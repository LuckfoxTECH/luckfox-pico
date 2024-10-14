/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "rtthread.h"

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
	#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

/*-----------------------------------------------------------*/

void *pvPortMalloc( size_t xWantedSize )
{
    return rt_malloc(xWantedSize);
}
/*-----------------------------------------------------------*/

void vPortFree( void *pv )
{
    return rt_free(pv);
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize( void )
{
#ifdef RT_MEM_STATS
	rt_uint32_t total, used, max_used, free;

	rt_memory_info(&total, &used, &max_used);
	free = total - used;
	return free;
#endif
	return -1;
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize( void )
{
#ifdef RT_MEM_STATS
	rt_uint32_t total, used, max_used, free;

	rt_memory_info(&total, &used, &max_used);
	free = total - used;
	return free;
#endif
	return -1;
}

/*-----------------------------------------------------------*/

void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions )
{

}
/*-----------------------------------------------------------*/

void vPortGetHeapStats( HeapStats_t *pxHeapStats )
{
#ifdef RT_MEM_STATS
	rt_uint32_t total, used, max_used;

	rt_memory_info(&total, &used, &max_used);
	pxHeapStats->xAvailableHeapSpaceInBytes = total - used;
	pxHeapStats->xMinimumEverFreeBytesRemaining = total - max_used;
#endif
}

