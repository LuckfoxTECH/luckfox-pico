/**
 * @file tuya_memory_heap.h
 * @brief TUYA memory heap management
 * @version 0.1
 * @date 2021-05-05
 *
 * @copyright Copyright 2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_MEMORY_HEAP_H__
#define __TUYA_MEMORY_HEAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MEM_HEAP_LIST_NUM (4)

typedef struct {
	void (*enter_critical)(void);
	void (*exit_critical)(void);
	void (*printf)(char *format, ...);
} heap_context_t;

int tuya_memory_heap_init(heap_context_t *ctx);
int tuya_memory_heap_create(void *start_addr, unsigned int size, unsigned int *handle);
int tuya_memory_heap_delete(unsigned int handle);
void *tuya_memory_heap_malloc(unsigned int handle, unsigned int size);
void tuya_memory_heap_free(unsigned int handle, void *ptr);
int tuya_memory_heap_available(unsigned int handle);

void *tuya_memory_heap_debug_malloc(unsigned int handle, unsigned int size, char *filename,
                                    int line);
int tuya_memory_heap_diagnose(unsigned int handle);

#ifdef __cplusplus
}
#endif

#endif //__TUYA_MEMORY_HEAP_H__
