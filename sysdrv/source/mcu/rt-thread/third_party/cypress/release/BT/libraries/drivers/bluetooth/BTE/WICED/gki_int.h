/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#ifndef GKI_INT_H
#define GKI_INT_H
#include "wiced_rtos.h"
#include "gki_common.h"

/**********************************************************************
 ** OS specific definitions
 */

typedef struct
{
    wiced_mutex_t GKI_mutex;
    wiced_thread_t thread_id[GKI_MAX_TASKS];
    wiced_mutex_t thread_evt_mutex[GKI_MAX_TASKS];
    wiced_queue_t thread_evt_queue[GKI_MAX_TASKS];
} tGKI_OS;

/* Contains common control block as well as OS specific variables */
typedef struct
{
    tGKI_OS os;
    tGKI_COM_CB com;
} tGKI_CB;

#ifdef __cplusplus
extern "C"
{
#endif

#if GKI_DYNAMIC_MEMORY == FALSE
GKI_API extern tGKI_CB gki_cb;
#else
GKI_API extern tGKI_CB *gki_cb_ptr;
#define gki_cb (*gki_cb_ptr)
#endif

#ifdef __cplusplus
}
#endif

#endif
