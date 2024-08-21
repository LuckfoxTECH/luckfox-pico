/*
 * Copyright 2018 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * author: martin.cheng@rock-chips.com
 *   date: 20180704
 */

#ifndef INCLUDE_RT_BASE_RT_MEM_H_
#define INCLUDE_RT_BASE_RT_MEM_H_

#include <stdlib.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define rt_malloc(type)  \
    reinterpret_cast<type *>( \
        rt_mem_malloc(sizeof(type), __FUNCTION__))

#define rt_malloc_array(type, count)  \
    reinterpret_cast<type *>( \
        rt_mem_malloc(sizeof(type) * (count), __FUNCTION__))

#define rt_malloc_size(type, size)  \
    reinterpret_cast<type *>( \
        rt_mem_malloc(size, __FUNCTION__))

#define rt_calloc_size(type, size)  \
    reinterpret_cast<type *>( \
        rt_mem_calloc(size, __FUNCTION__))

#define rt_calloc(type, count)  \
    reinterpret_cast<type *>( \
        rt_mem_calloc(sizeof(type) * (count), __FUNCTION__))

#define rt_realloc(ptr, type, count) \
    reinterpret_cast<type *>( \
        rt_mem_realloc(ptr, sizeof(type) * (count), __FUNCTION__))

#define rt_free(ptr) \
    rt_mem_free(ptr, __FUNCTION__)

#define rt_safe_free(ptr) \
    rt_mem_safe_free( \
        reinterpret_cast<void **>(&(ptr)), __FUNCTION__)

#ifndef rt_safe_delete
#define rt_safe_delete(p) { if (p) {delete(p); (p)=NULL;} }
#endif

#ifndef rt_safe_release
#define rt_safe_release(p) { if (p) {p->release(); p=NULL;} }
#endif

void *rt_mem_malloc(size_t size, const char *caller);
void *rt_mem_calloc(size_t size, const char *caller);
void *rt_mem_realloc(void *ptr, size_t size, const char *caller);
void rt_mem_free(void *ptr, const char *caller);
void rt_mem_safe_free(void **ptr, const char *caller);
void *rt_memset(void *ptr, int c, size_t n);
void *rt_memcpy(void *dst, const void *src, size_t n);

void rt_dump_callstack(size_t deep);

// utility functions for MemService.
void rt_mem_record_dump();
void rt_mem_record_reset();
void rt_mem_record_add_ref();
void rt_mem_record_sub_ref();

// utility functions for memory statistics
uint64_t rt_mem_get_total_size();
uint32_t rt_mem_get_total_count();

#ifdef  __cplusplus
}
#endif

extern "C" {
void *av_mem_malloc(size_t size);
void *av_mem_memalign(size_t align, size_t size);
int   av_mem_posix_memalign(void **ptr, size_t align, size_t size);
void *av_mem_realloc(void *ptr, size_t size);
void  av_mem_free(void *ptr);
}

#endif  // INCLUDE_RT_BASE_RT_MEM_H_
