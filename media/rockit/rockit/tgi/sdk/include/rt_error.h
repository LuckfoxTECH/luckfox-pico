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

#ifndef INCLUDE_RT_BASE_RT_ERROR_H_
#define INCLUDE_RT_BASE_RT_ERROR_H_

typedef enum _RT_RET {
    RT_OK                      = 0,

    RT_ERR_BAD                 = -1,
    RT_ERR_UNKNOWN             = -2,
    RT_ERR_NULL_PTR            = -3,
    RT_ERR_MALLOC              = -4,
    RT_ERR_OPEN_FILE           = -5,
    RT_ERR_VALUE               = -6,
    RT_ERR_READ_BIT            = -7,
    RT_ERR_TIMEOUT             = -8,
    RT_ERR_UNIMPLIMENTED       = -9,
    RT_ERR_UNSUPPORT           = -10,
    RT_ERR_SUSPEND             = -11,
    RT_ERR_NO_BUFFER           = -12,
    RT_ERR_HW_UNSUPPORT        = -13,
    RT_ERR_RETRY               = -14,

    RT_ERR_BASE                = -64,

    /* The error in stream processing */
    RT_ERR_INIT                = RT_ERR_BASE - 1,
    RT_ERR_FATAL_THREAD        = RT_ERR_BASE - 2,
    RT_ERR_NOMEM               = RT_ERR_BASE - 3,
    RT_ERR_OUTOF_RANGE         = RT_ERR_BASE - 4,
    RT_ERR_END_OF_STREAM       = RT_ERR_BASE - 5,

    /* The error in list */
    RT_ERR_LIST_BASE           = -128,
    RT_ERR_LIST_EMPTY          = RT_ERR_LIST_BASE - 1,
    RT_ERR_LIST_FULL           = RT_ERR_LIST_BASE - 2,
    RT_ERR_LIST_OUTOF_RANGE    = RT_ERR_LIST_BASE - 3,
} RT_RET;

#endif  // INCLUDE_RT_BASE_RT_ERROR_H_
