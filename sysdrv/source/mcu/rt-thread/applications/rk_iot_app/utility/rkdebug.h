/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RK_DBG_H__
#define RK_DBG_H__

/* DEBUG level */
#define DBG_ERROR           0
#define DBG_WARNING         1
#define DBG_INFO            2
#define DBG_LOG             3

#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "APP"
#endif

#ifndef DBG_LEVEL
#define DBG_LEVEL         DBG_INFO
#endif

#define _DBG_LOG_HDR(lvl_name)                    \
    rt_kprintf("["lvl_name"/"DBG_SECTION_NAME"] ")
#define _DBG_LOG_X_END                                     \
    rt_kprintf("\n")

#define dbg_here                                            \
    if ((DBG_LEVEL) <= DBG_LOG){                            \
        rt_kprintf(DBG_SECTION_NAME " Here %s:%d\n",        \
            __FUNCTION__, __LINE__);                        \
    }

#define dbg_log_line(lvl, fmt, ...)                \
    do                                                      \
    {                                                       \
        _DBG_LOG_HDR(lvl);                         \
        rt_kprintf(fmt, ##__VA_ARGS__);                     \
        _DBG_LOG_X_END;                                     \
    }                                                       \
    while (0)

#if (DBG_LEVEL >= DBG_LOG)
#define LOGD(fmt, ...)      dbg_log_line("D", fmt, ##__VA_ARGS__)
#else
#define LOGD(...)
#endif

#if (DBG_LEVEL >= DBG_INFO)
#define LOGI(fmt, ...)      dbg_log_line("I", fmt, ##__VA_ARGS__)
#else
#define LOGI(...)
#endif

#if (DBG_LEVEL >= DBG_WARNING)
#define LOGW(fmt, ...)      dbg_log_line("W", fmt, ##__VA_ARGS__)
#else
#define LOGW(...)
#endif

#if (DBG_LEVEL >= DBG_ERROR)
#define LOGE(fmt, ...)      dbg_log_line("E", fmt, ##__VA_ARGS__)
#else
#define LOGE(...)
#endif

#endif