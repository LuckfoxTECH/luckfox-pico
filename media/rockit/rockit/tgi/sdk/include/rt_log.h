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

#ifndef INCLUDE_RT_BASE_RT_LOG_H_
#define INCLUDE_RT_BASE_RT_LOG_H_

#include "rt_header.h"  // NOLINT
#include <stdarg.h>
#include <string>

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

#define RT_LOG_PRINT      0   /* print                                */
#define RT_LOG_FATAL      1   /* fatal error                          */
#define RT_LOG_ERR        2   /* error conditions                     */
#define RT_LOG_WARN       3   /* warning conditions                   */
#define RT_LOG_INFO       4   /* informational                        */
#define RT_LOG_DEBUG      5   /* debug-level messages                 */
#define RT_LOG_VERBOSE    6   /* verbose                              */

#define LOG_LEVEL         RT_LOG_WARN
#define LOG_FLAG          0

//! super macro.
#define RT_LOGD_IF(condition, format, ...) if (condition > 0) LOG_TRACE(RT_LOG_DEBUG, format, ##__VA_ARGS__)
#define RT_LOGE_IF(condition, format, ...) if (condition > 0) LOG_ERROR(RT_LOG_ERR, format, ##__VA_ARGS__)

//! super macro.
#define RT_LOGP(format, ...) LOG_ERROR(RT_LOG_PRINT, format, ##__VA_ARGS__)
#define RT_LOGF(format, ...) LOG_ERROR(RT_LOG_FATAL, format, ##__VA_ARGS__)
#define RT_LOGE(format, ...) LOG_ERROR(RT_LOG_ERR, format, ##__VA_ARGS__)
#define RT_LOGW(format, ...) LOG_TRACE(RT_LOG_WARN, format, ##__VA_ARGS__)
#define RT_LOGI(format, ...) LOG_TRACE(RT_LOG_INFO, format, ##__VA_ARGS__)
#define RT_LOGD(format, ...) LOG_TRACE(RT_LOG_DEBUG, format, ##__VA_ARGS__)
#define RT_LOGV(format, ...) LOG_TRACE(RT_LOG_VERBOSE, format, ##__VA_ARGS__)

//! super macro.
#define LOG_TRACE(level, fmt, ...) \
        rt_log(level, LOG_TAG, fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(level, fmt, ...) \
        rt_err(level, LOG_TAG, fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

//! log function marcro.
// external api
#define RT_LOG_API_ENTER()   RT_LOGD_IF(LOG_FLAG, "%s called enter", __FUNCTION__)
#define RT_LOG_API_LEAVE()   RT_LOGD_IF(LOG_FLAG, "%s called leave", __FUNCTION__)
#define RT_LOG_API_CALLED()  RT_LOGD_IF(LOG_FLAG, "%s called", __FUNCTION__)

// internal func
#define RT_LOG_FUNC_ENTER()  RT_LOGD_IF(LOG_FLAG, "%s enter", __FUNCTION__)
#define RT_LOG_FUNC_LEAVE()  RT_LOGD_IF(LOG_FLAG, "%s leave", __FUNCTION__)
#define RT_LOG_FUNC_CALLED() RT_LOGD_IF(LOG_FLAG, "%s called", __FUNCTION__)

// construct/destruct
#define RT_LOG_CONSTRUCT_IN(thiz)    RT_LOGD_IF(LOG_FLAG, "%s(%p) construct", __FUNCTION__, thiz)
#define RT_LOG_CONSTRUCT_DONE(thiz)  RT_LOGD_IF(LOG_FLAG, "%s(%p) construct ok", __FUNCTION__, thiz)
#define RT_LOG_DESTRUCT_IN(thiz)     RT_LOGD_IF(LOG_FLAG, "%s(%p) destructor", __FUNCTION__, thiz)
#define RT_LOG_DESTRUCT_DONE(thiz)   RT_LOGD_IF(LOG_FLAG, "%s(%p) destructor ok", __FUNCTION__, thiz)

#ifdef __cplusplus
extern "C" {
#endif

INT32 rt_get_log_level();
void  rt_set_log_level(INT32 level);

void rt_log(INT32 level, const char *tag, const char *fmt, const char *fname,
                            const UINT16 row, ...);
void rt_err(INT32 level, const char *tag, const char *fmt, const char *fname,
                            const UINT16 row, ...);
void rt_log_va(INT32 level, const char *tag, const char *fmt, const char *fname,
                            const UINT16 row, va_list args);

#ifdef __cplusplus
}
#endif
#endif  // INCLUDE_RT_BASE_RT_LOG_H_
