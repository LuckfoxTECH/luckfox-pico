/*
 * This file is part of the CmBacktrace Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2016-12-15
 */

#ifndef _CMB_CFG_H_
#define _CMB_CFG_H_

/* print line, must config by user */
#include <rtthread.h>
#ifndef RT_USING_ULOG
#ifndef CMB_USING_FLASH_LOG_BACKUP
#define cmb_println(...)               rt_kprintf(__VA_ARGS__);rt_kprintf("\r\n")
#else
extern void cmb_flash_log_println(const char *fmt, ...);
#define cmb_println(...)               rt_kprintf(__VA_ARGS__);rt_kprintf("\r\n");cmb_flash_log_println(__VA_ARGS__)
#endif /* CMB_USING_FLASH_LOG_BACKUP */
#else
#include <ulog.h>
#define CMB_LOG_TAG                    "cmb"
#define cmb_println(...)               ulog_e(CMB_LOG_TAG, __VA_ARGS__);ulog_flush()
#endif /* RT_USING_ULOG */

#ifdef RT_USNING_FAULT_DUMP_HOOK
#undef cmb_println(...)
#define cmb_println(...)               hook_println(__VA_ARGS__)
void hook_println(const char *format, ...);
typedef void (* fault_println)(const char *format);
typedef void (* dump_end)(void);
void cmb_set_hook(fault_println, dump_end);
#endif
/* enable bare metal(no OS) platform */
/* #define CMB_USING_BARE_METAL_PLATFORM */
/* enable OS platform */
#define CMB_USING_OS_PLATFORM
/* OS platform type, must config when CMB_USING_OS_PLATFORM is enable */
#define CMB_OS_PLATFORM_TYPE           CMB_OS_PLATFORM_RTT
/* cpu platform type, must config by user */
#if defined(PKG_CMBACKTRACE_PLATFORM_M0_M0PLUS)
    #define CMB_CPU_PLATFORM_TYPE      CMB_CPU_ARM_CORTEX_M0
#elif defined(PKG_CMBACKTRACE_PLATFORM_M3)
    #define CMB_CPU_PLATFORM_TYPE      CMB_CPU_ARM_CORTEX_M3
#elif defined(PKG_CMBACKTRACE_PLATFORM_M4)
    #define CMB_CPU_PLATFORM_TYPE      CMB_CPU_ARM_CORTEX_M4
#elif defined(PKG_CMBACKTRACE_PLATFORM_M7)
    #define CMB_CPU_PLATFORM_TYPE      CMB_CPU_ARM_CORTEX_M7
#else
    #error "You must select a CPU platform on menuconfig"
#endif /* PKG_CMBACKTRACE_PLATFORM_M0_M0PLUS */
/* enable dump stack information */
#if defined(PKG_CMBACKTRACE_DUMP_STACK)
    #define CMB_USING_DUMP_STACK_INFO
#endif
/* language of print information */
#if defined(PKG_CMBACKTRACE_PRINT_ENGLISH)
    #define CMB_PRINT_LANGUAGE         CMB_PRINT_LANGUAGE_ENGLISH
#elif defined(PKG_CMBACKTRACE_PRINT_CHINESE)
    #define CMB_PRINT_LANGUAGE         CMB_PRINT_LANGUAGE_CHINESE
#endif /* PKG_CMBACKTRACE_PRINT_ENGLISH */
#endif /* _CMB_CFG_H_ */
