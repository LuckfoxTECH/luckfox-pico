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

#ifndef __TIMER_H__
#define __TIMER_H__

#include "osi/types.h"
#include <stdbool.h>

typedef rt_timer_t osi_timer_t;
typedef uint64_t period_ms_t;
typedef void (*osi_timer_callback_t)(void *parameter);

osi_timer_t osi_timer_create(const char *name, osi_timer_callback_t time_cb, void *parameter, uint16_t time_tck, bool is_periodic);

osi_err_t osi_timer_delete(osi_timer_t timer);

osi_err_t osi_timer_start(osi_timer_t timer);

osi_err_t osi_timer_stop(osi_timer_t timer);


#endif /* __TIMER_H__ */