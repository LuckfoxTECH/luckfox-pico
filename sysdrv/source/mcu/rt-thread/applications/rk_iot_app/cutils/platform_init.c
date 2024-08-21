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

#include <rtthread.h>
#include <rtdef.h>
#include <stdio.h>

#include "version.h"
#include "sysinfo.h"

#ifdef PRJCONF_NET_EN
#include "net_ctrl.h"
#endif

#include "sys_ctrl/sys_ctrl.h"
#include "rkdebug.h"

#include "auto_version.h"

#define PLATFORM_SHOW_INFO      1   /* for internal debug only */
#define PRJCONF_SYS_CTRL_EN     1


#if PLATFORM_SHOW_INFO
static void platform_show_info(void)
{
    extern RT_UNUSED uint8_t firmware_magic_len[];
    extern RT_UNUSED uint8_t _flash_start_addr[];
    extern RT_UNUSED uint8_t  __data_end__[];
    extern RT_UNUSED uint8_t  __bss_start__[];
    extern RT_UNUSED uint8_t  __bss_end__[];
    extern RT_UNUSED uint8_t  __large_start__[];
    extern RT_UNUSED uint8_t  __large_end__[];
    extern RT_UNUSED uint8_t  _large_heap_size[];
    extern RT_UNUSED uint8_t  __HeapLimit[];
    extern RT_UNUSED uint8_t  __StackLimit[];
    extern RT_UNUSED uint8_t  _system_stack_size[];
    extern RT_UNUSED uint8_t  __StackTop[];
    extern RT_UNUSED uint8_t  _ram_fs_size[];
    extern RT_UNUSED uint8_t  _estack[];
    extern RT_UNUSED uint8_t __heap_start__[];

    LOGD("firmware_magic_len %p", firmware_magic_len);
    LOGD("_flash_start_addr %p", _flash_start_addr);
    LOGD("__data_end__  %p", __data_end__);
    LOGD("__bss_start__ %p", __bss_start__);
    LOGD("__bss_end__   %p\n", __bss_end__);
    LOGD("__large_start__   %p", __large_start__);
    LOGD("__large_end__     %p", __large_end__);
    LOGD("_large_heap_size  %p\n", _large_heap_size);
    LOGD("__HeapLimit   %p", __HeapLimit);
    LOGD("__StackLimit  %p", __StackLimit);
    LOGD("_system_stack_size %p", _system_stack_size);
    LOGD("__StackTop    %p\n", __StackTop);
    LOGD("_ram_fs_size  %p", _ram_fs_size);
    LOGD("_estack       %p", _estack);
    LOGD("__heap_start__ %p", __heap_start__);
}
#endif /* PLATFORM_SHOW_INFO */

void platform_init(void)
{
    rt_kprintf("\n");
    rt_kprintf("***************************************************************************\n");
    rt_kprintf("*    Copyright (C) Fuzhou Rockchips Electronics CO.,Ltd                   *\n");
    //rt_kprintf("*            RockChip IoT SDK "SDK_VERSION_STR"                           *\n");
    rt_kprintf("***************************************************************************\n");
    rt_kprintf("\n");

#if PLATFORM_SHOW_INFO
    platform_show_info();
#endif

#if PRJCONF_SYS_CTRL_EN
    sys_ctrl_create();
#ifdef PRJCONF_NET_EN
    net_ctrl_init();
#endif
#endif

    sysinfo_init();
}
