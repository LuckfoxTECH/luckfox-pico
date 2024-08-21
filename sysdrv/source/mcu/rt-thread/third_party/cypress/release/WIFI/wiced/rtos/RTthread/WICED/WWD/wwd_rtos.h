/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */
#pragma once

#include <rtthread.h>
#include <rtdebug.h>
#include <stdint.h>

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define RTOS_HIGHER_PRIORTIY_THAN(x)     ((x) > RTOS_HIGHEST_PRIORITY ? (x)-1 : RTOS_HIGHEST_PRIORITY)
#define RTOS_LOWER_PRIORTIY_THAN(x)      ((x) < RTOS_LOWEST_PRIORITY  ? (x)+1 : RTOS_LOWEST_PRIORITY )
#define RTOS_LOWEST_PRIORITY             (255)
#define RTOS_HIGHEST_PRIORITY            (31)
#define RTOS_DEFAULT_THREAD_PRIORITY     (RTOS_HIGHEST_PRIORITY - 10)

#define RTOS_USE_STATIC_THREAD_STACK

/* The number of system ticks per second */
#define SYSTICK_FREQUENCY  RT_TICK_PER_SECOND

#ifndef WWD_LOGGING_STDOUT_ENABLE
#ifdef DEBUG
#define WWD_THREAD_STACK_SIZE        (1248 + 1400) /*(632)*/   /* Stack checking requires a larger stack */
#else /* ifdef DEBUG */
#define WWD_THREAD_STACK_SIZE        (1024 + 1400) /*(544)*/
#endif /* ifdef DEBUG */
#else /* if WWD_LOGGING_STDOUT_ENABLE */
#define WWD_THREAD_STACK_SIZE        (544 + 4096 + 1400)   /* WWD_LOG uses printf and requires minimum 4K stack space */
#endif /* WWD_LOGGING_STDOUT_ENABLE */

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef struct
{
    uint32 time_slice;    /* 0 to disable timeslice among same prio threads; other value to specify slice length */
    uint32 arg;
} host_rtos_thread_config_type_t;

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct rt_semaphore     host_semaphore_type_t; /** RTthread definition of a semaphore */
typedef struct rt_mutex         host_mutex_type_t;      /** RTthread definition of a mutex */
typedef struct rt_thread        host_thread_type_t;     /** RTthread definition of a thread handle */
typedef struct rt_messagequeue  host_queue_type_t;      /** RTthread definition of a message queue */

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
