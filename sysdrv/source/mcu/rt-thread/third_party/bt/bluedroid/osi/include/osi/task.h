// Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __TASK_H__
#define __TASK_H__

#include "osi/types.h"

typedef void (osi_task_handler_t)(void *parameter);
typedef rt_thread_t  osi_task_t;
typedef uint32_t  osi_parameter_t;


osi_err_t osi_task_create(osi_task_handler_t task_handler, const char *name, uint32_t stack_size,
                          void *parameter, uint8_t priority, osi_task_t *task_id, osi_parameter_t value);

osi_err_t osi_task_delete(osi_task_t *task_id);

osi_err_t osi_task_startup(osi_task_t taskid);

void osi_thread_mdelay(uint32_t ms);

#endif

