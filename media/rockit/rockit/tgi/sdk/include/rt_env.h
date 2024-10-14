/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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
 * author: shika.zhou@rock-chips.com
 *   date: 2020/07/27
 */

#ifndef INCLUDE_RT_BASE_RT_ENV_H_
#define INCLUDE_RT_BASE_RT_ENV_H_

#include "rt_header.h"   // NOLINT
#include <string.h>      // NOLINT

#define RT_ENV_GET_U32(name, value, default_value) \
        rt_env_get_u32(name, value, default_value)

#define RT_ENV_GET_STR(name, value, default_value) \
        rt_env_get_str(name, value, default_value)

#define RT_ENV_SET_U32(name, value) \
        rt_env_set_u32(name, value)

#define RT_ENV_SET_STR(name, value) \
        rt_env_set_str(name, value)

#ifdef __cplusplus
extern "C" {
#endif

INT32 rt_env_get_u32(const char *name, UINT32 *value, UINT32 default_value);
INT32 rt_env_get_str(const char *name, const char **value, const char *default_value);
INT32 rt_env_set_u32(const char *name, UINT32 value);
INT32 rt_env_set_str(const char *name, char *value);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_RT_BASE_RT_ENV_H_

