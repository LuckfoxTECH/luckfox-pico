/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once

#include "wiced_rtos.h"
#include "linked_list.h"

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct bt_buffer      bt_buffer_t;
typedef struct bt_buffer_pool bt_buffer_pool_t;
typedef bt_buffer_pool_t *bt_buffer_pool_handle_t;

/******************************************************
 *                    Structures
 ******************************************************/

struct bt_buffer
{
    linked_list_node_t node;
    bt_buffer_pool_t  *pool;
#ifdef MEM_POOL_DEBUG
    char           alloc_string[50];
    char           free_string[50];
#endif
    uint8_t            data[];
};

struct bt_buffer_pool
{
    linked_list_t pool_list;
    uint8_t      *pool_buffer;
    wiced_mutex_t mutex;
    uint16_t      max_packet_count;
    uint16_t      header_size;
    uint16_t      data_size;
    /*members below this line needed only for debugging*/
    uint16_t      free_count;
#ifdef MEM_POOL_DEBUG
    uint16_t      min_free_count;
#endif
};

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

wiced_result_t bt_buffer_pool_init(bt_buffer_pool_handle_t *pool_handle, uint16_t buffer_count, uint16_t data_size);

wiced_result_t bt_buffer_pool_deinit(bt_buffer_pool_handle_t pool_handle);

#ifdef MEM_POOL_DEBUG
void *bt_buffer_pool_allocate_buffer_func(const char *alloc_string, bt_buffer_pool_handle_t pool_handle);
#define bt_buffer_pool_allocate_buffer(pool_handle)    bt_buffer_pool_allocate_buffer_func( __func__, pool_handle )
#else
void *bt_buffer_pool_allocate_buffer_func(bt_buffer_pool_handle_t pool_handle);
#define bt_buffer_pool_allocate_buffer( pool_handle )  bt_buffer_pool_allocate_buffer_func( pool_handle )
#endif

#ifdef MEM_POOL_DEBUG
wiced_result_t bt_buffer_pool_free_buffer_func(const char *free_string, void *buffer);
#define bt_buffer_pool_free_buffer(buffer)  bt_buffer_pool_free_buffer_func( __func__, buffer )
#else
wiced_result_t bt_buffer_pool_free_buffer_func(void *buffer);
#define bt_buffer_pool_free_buffer(buffer)  bt_buffer_pool_free_buffer_func( buffer )

#endif

uint32_t bt_buffer_pool_get_free_count(bt_buffer_pool_handle_t pool_handle);

void bt_buffer_pool_print_debug_info(bt_buffer_pool_handle_t pool_handle);

#ifdef __cplusplus
} /* extern "C" */
#endif
